#pragma once

#include <cstdint>
#include <ostream>
#include <optional>
#include <map>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

namespace lue::concepts {

/**
A versatile C++ class for representing and manipulating both file system paths and URLs in a unified way. It extends the functionality of std::filesystem::path to support schemes (e.g., http, https, file, ftp), query parameters, fragments, and authentication, while maintaining compatibility with local file paths.

@see https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3420.html
*/
class uri {
public:
    std::filesystem::path m_p;
    // --- Constructors ---
    uri();
    explicit uri(const fs::path& path);
    uri& operator=(const fs::path& path);

    // --- Scheme ---
    std::string                 scheme              () const;
    uri&                        scheme              (const std::string& new_scheme);

    // --- Authority (User Info + Host + Port) ---
    std::optional<std::string>  user_info           () const;
    uri&                        user_info           (const std::string& user, const std::string& pass = "");

    std::string                 host                () const;
    uri&                        host                (const std::string& new_host);

    std::optional<uint16_t>     port                () const;
    uri&                        port                (uint16_t new_port);
    uri&                        port                (std::optional<uint16_t> new_port);

    // --- Path ---
    fs::path                    path                () const;
    uri&                        path                (const fs::path& new_path);

    uri&                        operator/=          (const std::string& segment);
    uri                         operator/           (const std::string& segment) const;

    // --- Query Parameters ---
    using query_params_t = std::vector<std::pair<std::string, std::string>>;
    query_params_t              query_params        () const;
    uri&                        add_query_param     (const std::string& key, const std::string& value);
    uri&                        remove_query_param  (const std::string& key);

    // --- Fragment ---
    std::optional<std::string>  fragment            () const;
    uri&                        fragment            (const std::string& new_fragment);

    // --- Conversion ---
    std::string                 string              () const;
    fs::path                    to_filesystem_path  () const;

    // --- Comparison ---
    auto                        operator<=>         (const uri&) const = default;

    // --- Utility ---
    bool                        empty               () const;
    bool                        is_local            () const;
    bool                        is_remote           () const;
    uri                         resolve             (const uri& relative) const;

private:
    // --- Internal Representation ---
    std::string                 scheme_;
    std::optional<std::string>  user_info_;
    std::string                 host_;
    std::optional<uint16_t>     port_;
    fs::path                    path_;
    query_params_t              query_params_;
    std::optional<std::string>  fragment_;

    // --- Helper Functions ---
    static std::string percent_encode(const std::string& str);
    static std::string percent_decode(const std::string& str);
    static std::map<std::string, std::string> parse_query(const std::string& query_str);
    static std::string build_query(const query_params_t& params);
    void parse_uri(const std::string& uri_str);
    static fs::path normalize_path(const fs::path& path);
};

/** '<<' operator, Write uri to stream.
    \param[in] os Output stream to write to.
    \param[in] u Uri to write.
    \return Reference to (modified) ostream. */
inline std::ostream&	operator <<(std::ostream& os, const lue::concepts::uri& u)
{
    os << u.string();
    return os;
}


} // namespace lue::concepts

/// @see User-Defined Literals in C++: https://gist.github.com/MangaD/14780c2c092c4ea5fa466ae2d474e9c0
namespace lue::litterals {
// --- User-defined Literal to create uri from string literal ---
inline lue::concepts::uri operator""_uri(const char* str, size_t len)
{
    return lue::concepts::uri{std::string(str, len)};
}

} // namespace lue::litterals



namespace std {
template<>
struct hash<lue::concepts::uri>
{
    std::size_t operator()(const lue::concepts::uri& uri) const noexcept
    {
        return std::hash<std::string>{}(uri.string());
    }
};

} // namespace std
