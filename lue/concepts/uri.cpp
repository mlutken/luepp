#include "uri.h"

#include <sstream>
#include <string>
#include <algorithm>

using namespace std;
using namespace std::string_view_literals;

namespace lue::concepts {

uri::uri(const std::string& uri_string)
{
    // Parse protocol
    size_t protocol_end = uri_string.find("://");
    if (protocol_end != std::string::npos) {
        scheme_ = uri_string.substr(0, protocol_end);
        size_t start = protocol_end + 3; // Skip "://"

        // Parse host:port/path?query
        size_t path_start = uri_string.find('/', start);
        size_t query_start = uri_string.find('?', start);

        size_t host_port_end = std::min(path_start, query_start);

        std::string host_port = uri_string.substr(start, host_port_end - start);

        // Parse port
        size_t port_pos = host_port.find(':');
        if (port_pos != std::string::npos) {
            std::string port_str = host_port.substr(port_pos + 1);
            port_ = static_cast<uint16_t>(std::stoi(port_str));
        }

        // Parse path
        if (path_start != std::string::npos) {
            size_t path_end = (query_start != std::string::npos) ? query_start : uri_string.length();
            std::string path_str = uri_string.substr(path_start, path_end - path_start);
            path_ = path_str;
        }

        // Parse query parameters
        if (query_start != std::string::npos) {
            std::string query = uri_string.substr(query_start + 1);
            std::istringstream iss(query);
            std::string pair;
            while (std::getline(iss, pair, '&')) {
                size_t equals = pair.find('=');
                if (equals != std::string::npos) {
                    std::string key = pair.substr(0, equals);
                    std::string value = pair.substr(equals + 1);
                    parameters_[key] = value;
                }
            }
        }
    } else {
        // No protocol, treat as a local path
        path_ = uri_string;
    }
}

uri& uri::operator/=(const uri& other)
{
    if (!other.scheme_.empty()) {
        scheme_ = other.scheme_;
    }
    if (other.port_ != 0) {
        port_ = other.port_;
    }
    path_ /= other.path_;
    parameters_.insert(other.parameters_.begin(), other.parameters_.end());
    return *this;
}

uri& uri::operator/=(const std::string_view& path)
{
    path_ /= path;
    return *this;
}

std::string uri::string() const
{
    std::ostringstream oss;

    if (!scheme_.empty()) {
        oss << scheme_ << "://";
        // Add host if applicable
        if (port_ != 0) {
            oss << ":" << port_;
        }
    }

    oss << path_.string();

    std::string param_str = parameter_string();
    if (!param_str.empty()) {
        oss << "?" << param_str;
    }

    return oss.str();
}

std::string uri::parameter_string() const
{
    if (parameters_.empty()) {
        return {};
    }

    std::ostringstream oss;
    bool first = true;
    for (const auto& [key, value] : parameters_) {
        if (!first) {
            oss << "&";
        }
        oss << key << "=" << value;
        first = false;
    }
    return oss.str();
}

const std::string& uri::scheme() const
{
    return scheme_;
}

void uri::scheme(const std::string& scheme)
{
    scheme_ = scheme;
}

uint16_t uri::port() const
{
    return port_;
}

void uri::port(std::uint16_t port)
{
    port_ = port;
}

const uri::path_t& uri::path() const
{
    return path_;
}

void uri::path(const path_t& path)
{
    path_ = path;
}

std::string uri::parameter(const std::string& key) const
{
    auto it = parameters_.find(key);
    if (it != parameters_.end()) {
        return it->second;
    }
    return {};
}

void uri::parameter(const std::string& key, const std::string& value)
{
    parameters_[key] = value;
}

} // namespace lue::concepts
