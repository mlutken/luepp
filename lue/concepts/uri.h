#pragma once

#include <cstdint>
#include <map>
#include <filesystem>

namespace lue::concepts {

class uri {
public:
    using path_t = std::filesystem::path;

    uri() = default;
    explicit uri(const std::string& uri);

    uri&                operator/=      (const uri& p);
    uri&                operator/=      (const std::string_view& p);

    std::string         string          () const;
    std::string         parameter_string() const;

    const std::string&  protocol        () const;
    void                protocol_set    (const std::string& protocol);

    uint16_t            port            () const;
    void                port_set        (std::uint16_t port);

    const path_t&       path            () const;
    void                path_set        (const path_t& path);

    std::string         parameter       (const std::string& key) const;
    void                parameter_set   (const std::string& key, const std::string& value);

private:
    using parameters_map_t = std::map<std::string, std::string>;
    std::filesystem::path   path_           {};
    std::string             protocol_       {};
    parameters_map_t        parameters_     {};
    uint16_t                port_           {};
};

} // namespace lue::concepts
