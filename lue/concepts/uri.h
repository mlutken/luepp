#pragma once

#include <cstdint>
#include <optional>
#include <map>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

namespace lue::concepts {

/**
A versatile C++ class for representing and manipulating both file system paths and URLs in a unified way. It extends the functionality of std::filesystem::path to support schemes (e.g., http, https, file, ftp), query parameters, fragments, and authentication, while maintaining compatibility with local file paths.
*/
class uri {
public:
    std::filesystem::path m_p;
    // --- Constructors ---
    uri();
    explicit uri(const std::string& uri_str);
    // explicit uri(const fs::path& path);

    // --- Scheme ---
    std::string scheme() const;
    uri& scheme(const std::string& new_scheme);

    // --- Authority (User Info + Host + Port) ---
    std::optional<std::string> user_info() const;
    uri& user_info(const std::string& user, const std::string& pass = "");

    std::string host() const;
    uri& host(const std::string& new_host);

    std::optional<uint16_t> port() const;
    uri& port(uint16_t new_port);
    uri& port(std::optional<uint16_t> new_port);

    // --- Path ---
    fs::path path() const;
    uri& path(const fs::path& new_path);

    uri& operator/=(const std::string& segment);
    uri operator/(const std::string& segment) const;

    // --- Query Parameters ---
    using query_params_t = std::vector<std::pair<std::string, std::string>>;
    query_params_t query_params() const;
    uri& add_query_param(const std::string& key, const std::string& value);
    uri& remove_query_param(const std::string& key);

    // --- Fragment ---
    std::optional<std::string> fragment() const;
    uri& fragment(const std::string& new_fragment);

    // --- Conversion ---
    std::string string() const;
    fs::path to_filesystem_path() const;

    // --- Comparison ---
    bool operator==(const uri& other) const;
    bool operator!=(const uri& other) const;

    // --- Utility ---
    bool empty      () const;
    bool is_local   () const;
    bool is_remote  () const;
    uri resolve     (const uri& relative) const;

private:
    // --- Internal Representation ---
    std::string                 m_scheme;
    std::optional<std::string>  m_user_info;
    std::string                 m_host;
    std::optional<uint16_t>     m_port;
    fs::path                    m_path;
    query_params_t              m_query_params;
    std::optional<std::string>  m_fragment;

    // --- Helper Functions ---
    static std::string percent_encode(const std::string& str);
    static std::string percent_decode(const std::string& str);
    static std::map<std::string, std::string> parse_query(const std::string& query_str);
    static std::string build_query(const query_params_t& params);
    void parse_uri(const std::string& uri_str);
    static fs::path normalize_path(const fs::path& path);
};

} // namespace lue::concepts
