#include "uri.h"

#include <regex>
#include <cassert>

#include "strings/split.h"

using namespace std;
using namespace std::string_view_literals;
using namespace lue::strings;

namespace lue::concepts {

uri::uri()
    : scheme_(""), host_(""), path_(""), fragment_(std::nullopt) {}

/// \note This constructor is not used as it is ambiguous with the one that takes a std::filesystem::path
/// uri::uri(const std::string& uri_str) {
///     parse_uri(uri_str);
/// }

uri::uri(const fs::path& path)
{
    parse_uri(path.string());
}

uri& uri::operator=(const std::filesystem::path& path)
{
    parse_uri(path.string());
    return *this;
}

std::string uri::scheme() const {
    return scheme_;
}

uri& uri::scheme(const std::string& new_scheme) {
    if (!new_scheme.empty()) {
        // Validate scheme (must be alphanumeric + "+-.")
        if (!std::regex_match(new_scheme, std::regex("^[a-zA-Z][a-zA-Z0-9+\\-.]*$"))) {
            throw std::invalid_argument("Invalid scheme: " + new_scheme);
        }
    }
    scheme_ = new_scheme;
    return *this;
}

std::optional<std::string> uri::user_info() const {
    return user_info_;
}

uri& uri::user_info(const std::string& user, const std::string& pass) {
    if (user.empty() && pass.empty()) {
        user_info_ = std::nullopt;
    } else {
        user_info_ = user + (pass.empty() ? "" : ":" + pass);
    }
    return *this;
}

std::string uri::host() const {
    return host_;
}

uri& uri::host(const std::string& new_host) {
    if (!new_host.empty()) {
        // Basic host validation (simplified)
        if (new_host.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-.") != std::string::npos) {
            throw std::invalid_argument("Invalid host: " + new_host);
        }
    }
    host_ = new_host;
    return *this;
}

std::optional<uint16_t> uri::port() const {
    return port_;
}

uri& uri::port(uint16_t new_port) {
    port_ = new_port;
    return *this;
}

uri& uri::port(std::optional<uint16_t> new_port)
{
    port_ = new_port;
    return *this;
}

fs::path uri::path() const {
    return path_;
}

uri& uri::path(const fs::path& new_path) {
    path_ = normalize_path(new_path);
    return *this;
}

uri& uri::operator/=(const std::string& segment) {
    path_ /= segment;
    path_ = normalize_path(path_);
    return *this;
}

uri uri::operator/(const std::string& segment) const {
    uri result = *this;
    result /= segment;
    return result;
}

uri::query_params_t uri::query_params() const {
    return query_params_;
}

uri& uri::add_query_param(const std::string& key, const std::string& value) {
    // Remove existing param with the same key
    query_params_.erase(
        std::remove_if(
            query_params_.begin(),
            query_params_.end(),
            [&key](const auto& param) { return param.first == key; }
        ),
        query_params_.end()
    );
    query_params_.emplace_back(key, value);
    return *this;
}

uri& uri::remove_query_param(const std::string& key) {
    query_params_.erase(
        std::remove_if(
            query_params_.begin(),
            query_params_.end(),
            [&key](const auto& param) { return param.first == key; }
        ),
        query_params_.end()
    );
    return *this;
}

std::optional<std::string> uri::fragment() const {
    return fragment_;
}

uri& uri::fragment(const std::string& new_fragment) {
    fragment_ = new_fragment.empty() ? std::nullopt : std::optional<std::string>(new_fragment);
    return *this;
}

std::string uri::string() const {
    std::ostringstream oss;

    // Scheme
    if (!scheme_.empty()) {
        oss << scheme_ << "://";
    }

    // Authority (user info, host, port)
    if (!host_.empty() || user_info_ || port_) {
        if (user_info_) {
            oss << *user_info_ << "@";
        }
        oss << host_;
        if (port_) {
            oss << ":" << *port_;
        }
    }

    // Path
    std::string path_str = path_.string();
    if (scheme_.empty() && host_.empty()) {
        // Local path: preserve as-is (e.g., "/home/user" or "C:\path")
        oss << path_str;
    } else {
        // URL path: ensure it starts with "/"
        if (path_str.empty() || path_str[0] != '/') {
            oss << "/";
        }
        oss << path_str;
    }

    // Query
    if (!query_params_.empty()) {
        oss << "?" << build_query(query_params_);
    }

    // Fragment
    if (fragment_) {
        oss << "#" << percent_encode(*fragment_);
    }

    return oss.str();
}

fs::path uri::to_filesystem_path() const {
    if (is_local()) {
        return path_;
    } else if (scheme_ == "file") {
        // Convert "file:///path/to/file" to "/path/to/file"
        std::string path_str = path_.string();
        if (path_str.size() >= 3 && path_str.substr(0, 3) == "/:/") {
            // Windows: "file:///C:/path" -> "C:/path"
            return fs::path(path_str.substr(2));
        } else if (!path_str.empty() && path_str[0] == '/') {
            // Unix: "file:///path" -> "/path"
            return fs::path(path_str);
        }
    }
    throw std::runtime_error("Cannot convert remote URI to filesystem path: " + this->string());
}

bool uri::empty() const
{
    return this->string().empty();
}

bool uri::is_local() const {
    return scheme_.empty() || scheme_ == "file";
}

bool uri::is_remote() const {
    return !is_local() && !scheme_.empty();
}

uri uri::resolve(const uri& relative) const {
    if (relative.scheme_.empty() && relative.host_.empty()) {
        // Relative URI: resolve against this URI
        uri result = *this;

        if (relative.path_.string().starts_with("/")) {
            // Absolute path: replace path
            result.path_ = relative.path_;
        } else {
            // Relative path: append to current path
            result.path_ = normalize_path(result.path_ / relative.path_);
        }

        // Merge query params (relative overrides base)
        for (const auto& [key, value] : relative.query_params_) {
            result.add_query_param(key, value);
        }

        // Override fragment
        if (relative.fragment_) {
            result.fragment_ = relative.fragment_;
        }

        return result;
    } else {
        // Absolute URI: return as-is
        return relative;
    }
}

// --- Helper Functions ---
std::string uri::percent_encode(const std::string& str) {
    std::ostringstream oss;
    for (char c : str) {
        if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            oss << c;
        } else {
            oss << "%" << std::hex << std::uppercase << static_cast<int>(c);
        }
    }
    return oss.str();
}

std::string uri::percent_decode(const std::string& str) {
    std::ostringstream oss;
    for (size_t i = 0; i < str.size(); ++i) {
        if (str[i] == '%' && i + 2 < str.size()) {
            std::string hex_str = str.substr(i + 1, 2);
            try {
                int value = std::stoi(hex_str, nullptr, 16);
                oss << static_cast<char>(value);
                i += 2;
            } catch (...) {
                oss << str[i];
            }
        } else {
            oss << str[i];
        }
    }
    return oss.str();
}

std::map<std::string, std::string> uri::parse_query(const std::string& query_str) {
    std::map<std::string, std::string> params;
    if (query_str.empty()) return params;

    for (const auto& pair : split(query_str, '&')) {
        size_t eq_pos = pair.find('=');
        if (eq_pos != std::string::npos) {
            std::string key = percent_decode(pair.substr(0, eq_pos));
            std::string value = percent_decode(pair.substr(eq_pos + 1));
            params[key] = value;
        } else {
            params[percent_decode(pair)] = "";
        }
    }
    return params;
}

std::string uri::build_query(const query_params_t& params) {
    std::ostringstream oss;
    for (size_t i = 0; i < params.size(); ++i) {
        if (i > 0) oss << "&";
        oss << percent_encode(params[i].first);
        if (!params[i].second.empty()) {
            oss << "=" << percent_encode(params[i].second);
        }
    }
    return oss.str();
}

void uri::parse_uri(const std::string& uri_str) {
    // Reset all members
    scheme_.clear();
    user_info_ = std::nullopt;
    host_.clear();
    port_ = std::nullopt;
    path_ = "/";
    query_params_.clear();
    fragment_ = std::nullopt;

    // Regex to parse URI components (RFC 3986)
    // Note: Escaped properly for C++ string literals
    std::string regex_str =
        "([a-zA-Z][a-zA-Z0-9+\\-.]*):\\/\\/"  // Scheme (e.g., http:)
        "([^:@\\/?#]+(?::[^@\\/?#]*)?@)?"     // User info (e.g., user:pass@)
        "([^\\/?#:]+(?::\\d+)?)?"             // Host and port (e.g., example.com:8080)
        "(\\/[^\\?#]*)?"                      // Path (e.g., /path/to/resource)
        "(\\?[^#]*)?"                         // Query (e.g., ?q=12&sort=asc)
        "(#.*)?";                             // Fragment (e.g., #header)

    std::regex uri_regex(regex_str);

    std::smatch matches;
    if (std::regex_match(uri_str, matches, uri_regex)) {
        // Extract scheme
        if (matches[1].matched) {
            scheme_ = matches[1].str();
        }

        // Extract user info
        if (matches[2].matched) {
            std::string user_info_str = matches[2].str();
            size_t colon_pos = user_info_str.find(':');
            if (colon_pos != std::string::npos) {
                std::string user = user_info_str.substr(0, colon_pos);
                const auto pass_pos = colon_pos + 1;
                const auto pass_len = user_info_str.size() - pass_pos - 1;
                std::string pass = user_info_str.substr(pass_pos , pass_len);
                user_info(user, pass);
            } else {
                user_info(user_info_str, "");
            }
        }

        // Extract host and port
        if (matches[3].matched) {
            std::string host_port_str = matches[3].str();
            size_t colon_pos = host_port_str.find(':');
            if (colon_pos != std::string::npos) {
                host_ = host_port_str.substr(0, colon_pos);
                port_ = static_cast<uint16_t>(std::stoi(host_port_str.substr(colon_pos + 1)));
            } else {
                host_ = host_port_str;
            }
        }

        // Extract path
        if (matches[4].matched) {
            std::string path_str = matches[4].str();
            path_ = path_str.empty() ? "/" : normalize_path(fs::path(path_str));
        }

        // Extract query
        if (matches[5].matched) {
            std::string query_str = matches[5].str().substr(1); // Remove leading '?'
            auto params = parse_query(query_str);
            for (const auto& [key, value] : params) {
                query_params_.emplace_back(key, value);
            }
        }

        // Extract fragment
        if (matches[6].matched) {
            fragment_ = matches[6].str().substr(1); // Remove leading '#'
        }
    } else {
        // Fallback for local paths (no scheme, no host)
        path_ = normalize_path(fs::path(uri_str));
    }

    // Handle local paths (no scheme, no host)
    if (scheme_.empty() && host_.empty()) {
        path_ = normalize_path(fs::path(uri_str));
    }
}

fs::path uri::normalize_path(const fs::path& path) {
    fs::path normalized;
    for (const auto& part : path) {
        if (part == ".") {
            continue;
        } else if (part == "..") {
            if (!normalized.empty() && normalized.filename() != "..") {
                normalized = normalized.parent_path();
            } else {
                normalized /= part;
            }
        } else {
            normalized /= part;
        }
    }
    return normalized;
}
} // namespace lue::concepts
