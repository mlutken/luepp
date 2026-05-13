#include "uri.h"

#include <regex>
#include <cassert>

using namespace std;
using namespace std::string_view_literals;

namespace lue::concepts {

namespace {
    // Helper: Trim whitespace from a string
    // std::string trim(const std::string& str) {
    //     auto start = str.begin();
    //     while (start != str.end() && std::isspace(*start)) start++;
    //     auto end = str.end();
    //     do {
    //         end--;
    //     } while (std::distance(start, end) > 0 && std::isspace(*end));
    //     return std::string(start, end + 1);
    // }

    // Helper: Split a string by a delimiter
    std::vector<std::string> split(const std::string& str, char delim) {
        std::vector<std::string> tokens;
        // delim = ',';
        std::string token;
        std::istringstream token_stream(str);
        while (std::getline(token_stream, token, delim)) {
            if (!token.empty()) tokens.push_back(token);
        }
        return tokens;
    }
}

uri::uri()
    : m_scheme(""), m_host(""), m_path(""), m_fragment(std::nullopt) {}

uri::uri(const std::string& uri_str) {
    parse_uri(uri_str);
}

uri::uri(const fs::path& path) {
    m_scheme = "file";
    m_path = normalize_path(path);
}

std::string uri::scheme() const {
    return m_scheme;
}

uri& uri::scheme(const std::string& new_scheme) {
    if (!new_scheme.empty()) {
        // Validate scheme (must be alphanumeric + "+-.")
        if (!std::regex_match(new_scheme, std::regex("^[a-zA-Z][a-zA-Z0-9+\\-.]*$"))) {
            throw std::invalid_argument("Invalid scheme: " + new_scheme);
        }
    }
    m_scheme = new_scheme;
    return *this;
}

std::optional<std::string> uri::user_info() const {
    return m_user_info;
}

uri& uri::user_info(const std::string& user, const std::string& pass) {
    if (user.empty() && pass.empty()) {
        m_user_info = std::nullopt;
    } else {
        m_user_info = user + (pass.empty() ? "" : ":" + pass);
    }
    return *this;
}

std::string uri::host() const {
    return m_host;
}

uri& uri::host(const std::string& new_host) {
    if (!new_host.empty()) {
        // Basic host validation (simplified)
        if (new_host.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-.") != std::string::npos) {
            throw std::invalid_argument("Invalid host: " + new_host);
        }
    }
    m_host = new_host;
    return *this;
}

std::optional<uint16_t> uri::port() const {
    return m_port;
}

uri& uri::port(uint16_t new_port) {
    m_port = new_port;
    return *this;
}

fs::path uri::path() const {
    return m_path;
}

uri& uri::path(const fs::path& new_path) {
    m_path = normalize_path(new_path);
    return *this;
}

uri& uri::operator/=(const std::string& segment) {
    m_path /= segment;
    m_path = normalize_path(m_path);
    return *this;
}

uri uri::operator/(const std::string& segment) const {
    uri result = *this;
    result /= segment;
    return result;
}

uri::query_params_t uri::query_params() const {
    return m_query_params;
}

uri& uri::add_query_param(const std::string& key, const std::string& value) {
    // Remove existing param with the same key
    m_query_params.erase(
        std::remove_if(
            m_query_params.begin(),
            m_query_params.end(),
            [&key](const auto& param) { return param.first == key; }
        ),
        m_query_params.end()
    );
    m_query_params.emplace_back(key, value);
    return *this;
}

uri& uri::remove_query_param(const std::string& key) {
    m_query_params.erase(
        std::remove_if(
            m_query_params.begin(),
            m_query_params.end(),
            [&key](const auto& param) { return param.first == key; }
        ),
        m_query_params.end()
    );
    return *this;
}

std::optional<std::string> uri::fragment() const {
    return m_fragment;
}

uri& uri::fragment(const std::string& new_fragment) {
    m_fragment = new_fragment.empty() ? std::nullopt : std::optional<std::string>(new_fragment);
    return *this;
}

std::string uri::string() const {
    std::ostringstream oss;

    // Scheme
    if (!m_scheme.empty()) {
        oss << m_scheme << "://";
    }

    // Authority (user info, host, port)
    if (!m_host.empty() || m_user_info || m_port) {
        if (m_user_info) {
            oss << *m_user_info << "@";
        }
        oss << m_host;
        if (m_port) {
            oss << ":" << *m_port;
        }
    }

    // Path
    std::string path_str = m_path.string();
    if (m_scheme.empty() && m_host.empty()) {
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
    if (!m_query_params.empty()) {
        oss << "?" << build_query(m_query_params);
    }

    // Fragment
    if (m_fragment) {
        oss << "#" << percent_encode(*m_fragment);
    }

    return oss.str();
}

fs::path uri::to_filesystem_path() const {
    if (is_local()) {
        return m_path;
    } else if (m_scheme == "file") {
        // Convert "file:///path/to/file" to "/path/to/file"
        std::string path_str = m_path.string();
        if (path_str.size() >= 3 && path_str.substr(0, 3) == "/:/") {
            // Windows: "file:///C:/path" -> "C:/path"
            return fs::path(path_str.substr(2));
        } else if (!path_str.empty() && path_str[0] == '/') {
            // Unix: "file:///path" -> "/path"
            return fs::path(path_str);
        }
    }
    throw std::runtime_error("Cannot convert remote URI to filesystem path: " + string());
}

bool uri::operator==(const uri& other) const {
    return m_scheme == other.m_scheme &&
           m_user_info == other.m_user_info &&
           m_host == other.m_host &&
           m_port == other.m_port &&
           m_path == other.m_path &&
           m_query_params == other.m_query_params &&
           m_fragment == other.m_fragment;
}

bool uri::operator!=(const uri& other) const {
    return !(*this == other);
}

bool uri::is_local() const {
    return m_scheme.empty() || m_scheme == "file";
}

bool uri::is_remote() const {
    return !is_local() && !m_scheme.empty();
}

uri uri::resolve(const uri& relative) const {
    if (relative.m_scheme.empty() && relative.m_host.empty()) {
        // Relative URI: resolve against this URI
        uri result = *this;

        if (relative.m_path.string().starts_with("/")) {
            // Absolute path: replace path
            result.m_path = relative.m_path;
        } else {
            // Relative path: append to current path
            result.m_path = normalize_path(result.m_path / relative.m_path);
        }

        // Merge query params (relative overrides base)
        for (const auto& [key, value] : relative.m_query_params) {
            result.add_query_param(key, value);
        }

        // Override fragment
        if (relative.m_fragment) {
            result.m_fragment = relative.m_fragment;
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
    m_scheme.clear();
    m_user_info = std::nullopt;
    m_host.clear();
    m_port = std::nullopt;
    m_path = "";
    m_query_params.clear();
    m_fragment = std::nullopt;

    // Regex to parse URI components
    std::regex uri_regex(
        R"regex(([a-zA-Z][a-zA-Z0-9+\-.]*):\/\/)?)regex"  // Scheme
        R"regex(([^:@\/?#]+)(?::([^@\/?#]*))?@)?)regex"    // User info (user:pass@)
        R"regex(([^\/?#:]*)(?::(\d+))?)?)regex"           // Host and port
        R"regex((\/[^\?#]*)?)?)regex"                     // Path
        R"regex(\?([^#]*))?)regex"                        // Query
        R"regex(#(.*))?)regex"                           // Fragment
        );

    std::smatch matches;
    if (!std::regex_match(uri_str, matches, uri_regex)) {
        throw std::invalid_argument("Invalid URI: " + uri_str);
    }

    // Extract scheme
    if (matches[1].matched) {
        m_scheme = matches[1].str();
    }

    // Extract user info
    if (matches[3].matched) {
        std::string user = matches[3].str();
        std::string pass = matches[4].matched ? matches[4].str() : "";
        user_info(user, pass);
    }

    // Extract host and port
    if (matches[5].matched) {
        m_host = matches[5].str();
    }
    if (matches[6].matched) {
        m_port = static_cast<uint16_t>(std::stoi(matches[6].str()));
    }

    // Extract path
    if (matches[7].matched) {
        std::string path_str = matches[7].str();
        if (path_str.empty()) {
            m_path = "/";
        } else {
            m_path = normalize_path(fs::path(path_str));
        }
    }

    // Extract query
    if (matches[8].matched) {
        std::string query_str = matches[8].str();
        auto params = parse_query(query_str);
        for (const auto& [key, value] : params) {
            m_query_params.emplace_back(key, value);
        }
    }

    // Extract fragment
    if (matches[9].matched) {
        m_fragment = matches[9].str();
    }

    // Handle local paths (no scheme, no host)
    if (m_scheme.empty() && m_host.empty()) {
        // Treat as a local file path
        m_path = normalize_path(fs::path(uri_str));
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
