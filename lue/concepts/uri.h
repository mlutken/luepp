#pragma once

#include <cstdint>
#include <map>
#include <filesystem>

namespace lue::concepts {

/**
A versatile C++ class for representing and manipulating both file system paths and URLs in a unified way. It extends the functionality of std::filesystem::path to support schemes (e.g., http, https, file, ftp), query parameters, fragments, and authentication, while maintaining compatibility with local file paths.
*/
class uri {
public:
    using path_t = std::filesystem::path;

    uri() = default;
    explicit uri(const std::string& uri);

    uri&                operator/=      (const uri& p);
    uri&                operator/=      (const std::string_view& p);

    std::string         string          () const;
    std::string         parameter_string() const;

    const std::string&  scheme          () const;
    void                scheme          (const std::string& scheme);

    const std::string&  host            () const;
    void                host            (const std::string& host_name);

    uint16_t            port            () const;
    void                port            (std::uint16_t port);

    const path_t&       path            () const;
    void                path            (const path_t& path);

    std::string         parameter       (const std::string& key) const;
    void                parameter       (const std::string& key, const std::string& value);

private:
    using parameters_map_t = std::map<std::string, std::string>;
    std::filesystem::path   path_           {};
    std::string             host_           {};
    std::string             scheme_       {};
    parameters_map_t        parameters_     {};
    uint16_t                port_           {};
};

} // namespace lue::concepts
