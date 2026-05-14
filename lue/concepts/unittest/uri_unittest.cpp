#include <gtest/gtest.h>
#include "concepts/uri.h"

using namespace testing;
using namespace lue::concepts;

class UriUnitTest : public testing::Test
{
public:
    UriUnitTest()  = default;
    ~UriUnitTest() override = default;

    void SetUp() override {}
    void TearDown() override {}
};

// -------------------
// -- Constructors --
// -------------------
TEST_F(UriUnitTest, construct_from_http_uri_with_user_info)
{
    uri u{"http://user:password@example.com/path"};
    EXPECT_EQ("http", u.scheme());
    EXPECT_TRUE(u.user_info().has_value());
    EXPECT_EQ("user:password", u.user_info().value());
    EXPECT_EQ("example.com", u.host());
}

TEST_F(UriUnitTest, default_constructor)
{
    uri u{};
    EXPECT_EQ("", u.string());
    EXPECT_EQ("", u.scheme());
    EXPECT_EQ("", u.host());
    EXPECT_FALSE(u.port().has_value());
    EXPECT_EQ("", u.path().string());
}

TEST_F(UriUnitTest, construct_from_path)
{
    uri u{fs::path{"/some/path/to/file"}};
    EXPECT_EQ("", u.scheme());
    EXPECT_EQ("/some/path/to/file", u.path().string());
}

TEST_F(UriUnitTest, construct_from_local_path_string)
{
    uri u{"some/local/path"};
    EXPECT_EQ("", u.scheme());
    EXPECT_EQ("some/local/path", u.path().string());
}

TEST_F(UriUnitTest, construct_from_file_uri)
{
    uri u{"file:///absolute/path/to/file"};
    EXPECT_EQ("file", u.scheme());
    EXPECT_EQ("", u.host());
    EXPECT_EQ("/absolute/path/to/file", u.path().string());
}

TEST_F(UriUnitTest, construct_from_http_uri)
{
    uri u{"http://example.com/path?key=value"};
    EXPECT_EQ("http", u.scheme());
    EXPECT_EQ("example.com", u.host());
    EXPECT_EQ("/path", u.path().string());
    EXPECT_FALSE(u.port().has_value());
    
    auto params = u.query_params();
    ASSERT_EQ(1, params.size());
    EXPECT_EQ("key", params[0].first);
    EXPECT_EQ("value", params[0].second);
}

TEST_F(UriUnitTest, construct_from_http_uri_with_port)
{
    uri u{"http://example.com:8080/path"};
    EXPECT_EQ("http", u.scheme());
    EXPECT_EQ("example.com", u.host());
    EXPECT_TRUE(u.port().has_value());
    EXPECT_EQ(8080, u.port().value());
    EXPECT_EQ("/path", u.path().string());
}


TEST_F(UriUnitTest, construct_from_uri_with_fragment)
{
    uri u{"http://example.com/path#section"};
    EXPECT_EQ("http", u.scheme());
    EXPECT_EQ("example.com", u.host());
    EXPECT_EQ("/path", u.path().string());
    EXPECT_TRUE(u.fragment().has_value());
    EXPECT_EQ("section", u.fragment().value());
}

TEST_F(UriUnitTest, construct_from_https_uri)
{
    uri u{"https://secure.example.com/api/v1/data"};
    EXPECT_EQ("https", u.scheme());
    EXPECT_EQ("secure.example.com", u.host());
    EXPECT_EQ("/api/v1/data", u.path().string());
}

// ----------
// -- Path --
// ----------

TEST_F(UriUnitTest, path_getter_setter)
{
    uri u;
    u.scheme("file");
    u.path("/new/path");
    EXPECT_EQ("/new/path", u.path().string());
}

TEST_F(UriUnitTest, operator_slash_equals)
{
    uri u{"file:///base"};
    u /= "subdir";
    EXPECT_EQ("/base/subdir", u.path().string());
}

TEST_F(UriUnitTest, operator_slash)
{
    uri u{"file:///base"};
    uri v = u / "subdir";
    EXPECT_EQ("/base/subdir", v.path().string());
    EXPECT_EQ("/base", u.path().string()); // u unchanged
}

// -----------
// -- Scheme --
// -----------

TEST_F(UriUnitTest, scheme_getter_setter)
{
    uri u{"http://example.com"};
    EXPECT_EQ("http", u.scheme());
    
    u.scheme("https");
    EXPECT_EQ("https", u.scheme());
}

// -------
// -- Host --
// -------

TEST_F(UriUnitTest, host_getter_setter)
{
    uri u;
    EXPECT_EQ("", u.host());
    
    u.host("example.com");
    EXPECT_EQ("example.com", u.host());
}

// -------
// -- Port --
// -------

TEST_F(UriUnitTest, port_getter_setter)
{
    uri u;
    EXPECT_FALSE(u.port().has_value());
    
    u.port(8080);
    EXPECT_TRUE(u.port().has_value());
    EXPECT_EQ(8080, u.port().value());
    
    u.port(0);
    EXPECT_TRUE(u.port().has_value());
    EXPECT_EQ(0, u.port().value());
}

// ----------------
// -- User Info --
// ----------------

TEST_F(UriUnitTest, user_info_getter_setter)
{
    uri u;
    EXPECT_FALSE(u.user_info().has_value());
    
    u.user_info("username", "password");
    EXPECT_TRUE(u.user_info().has_value());
    EXPECT_EQ("username:password", u.user_info().value());
    
    u.user_info("username");
    EXPECT_TRUE(u.user_info().has_value());
    EXPECT_EQ("username", u.user_info().value());
}

// -------------------
// -- Query Params --
// -------------------

TEST_F(UriUnitTest, query_params_empty)
{
    uri u{"http://example.com/path"};
    EXPECT_TRUE(u.query_params().empty());
}

TEST_F(UriUnitTest, add_query_param)
{
    uri u{"http://example.com/path"};
    u.add_query_param("key1", "value1");
    u.add_query_param("key2", "value2");
    
    auto params = u.query_params();
    ASSERT_EQ(2, params.size());
    EXPECT_EQ("key1", params[0].first);
    EXPECT_EQ("value1", params[0].second);
    EXPECT_EQ("key2", params[1].first);
    EXPECT_EQ("value2", params[1].second);
}

TEST_F(UriUnitTest, remove_query_param)
{
    uri u{"http://example.com/path?key1=value1&key2=value2"};
    u.remove_query_param("key1");
    
    auto params = u.query_params();
    ASSERT_EQ(1, params.size());
    EXPECT_EQ("key2", params[0].first);
}

TEST_F(UriUnitTest, query_params_from_constructor)
{
    uri u{"http://example.com/path?a=1&b=2&c=3"};
    auto params = u.query_params();
    ASSERT_EQ(3, params.size());
}

// --------------
// -- Fragment --
// --------------

TEST_F(UriUnitTest, fragment_getter_setter)
{
    uri u;
    EXPECT_FALSE(u.fragment().has_value());
    
    u.fragment("section1");
    EXPECT_TRUE(u.fragment().has_value());
    EXPECT_EQ("section1", u.fragment().value());
}

// ---------------
// -- Stringify --
// ---------------

TEST_F(UriUnitTest, string_empty_uri)
{
    uri u{};
    EXPECT_EQ("", u.string());
}

TEST_F(UriUnitTest, string_local_path)
{
    uri u{"local/path"};
    EXPECT_EQ("local/path", u.string());
}

TEST_F(UriUnitTest, string_file_uri)
{
    uri u{"file:///path/to/file"};
    EXPECT_EQ("file:///path/to/file", u.string());
}

TEST_F(UriUnitTest, string_http_uri_full)
{
    uri u;
    u.scheme("http");
    u.host("example.com");
    u.port(8080);
    u.path("/api/data");
    u.add_query_param("format", "json");
    u.fragment("results");
    
    std::string str = u.string();
    EXPECT_TRUE(str.find("http://") != std::string::npos);
    EXPECT_TRUE(str.find("example.com") != std::string::npos);
    EXPECT_TRUE(str.find(":8080") != std::string::npos);
    EXPECT_TRUE(str.find("/api/data") != std::string::npos);
    EXPECT_TRUE(str.find("format=json") != std::string::npos);
    EXPECT_TRUE(str.find("#results") != std::string::npos);
}

// -----------------------
// -- Comparison --
// -----------------------

TEST_F(UriUnitTest, operator_equal)
{
    uri u1{"http://example.com/path"};
    uri u2{"http://example.com/path"};
    EXPECT_EQ(u1, u2);
}

TEST_F(UriUnitTest, operator_not_equal)
{
    uri u1{"http://example.com/path1"};
    uri u2{"http://example.com/path2"};
    EXPECT_NE(u1, u2);
}

// -----------------------
// -- Utility --
// -----------------------

TEST_F(UriUnitTest, is_local)
{
    uri local_path{"relative/path"};
    EXPECT_TRUE(local_path.is_local());
    
    uri file_uri{"file:///absolute/path"};
    EXPECT_TRUE(file_uri.is_local());
    
    uri http_uri{"http://example.com"};
    EXPECT_FALSE(http_uri.is_local());
}

TEST_F(UriUnitTest, is_remote)
{
    uri http_uri{"http://example.com"};
    EXPECT_TRUE(http_uri.is_remote());
    
    uri https_uri{"https://example.com"};
    EXPECT_TRUE(https_uri.is_remote());
    
    uri local_path{"relative/path"};
    EXPECT_FALSE(local_path.is_remote());
}

TEST_F(UriUnitTest, resolve_relative_uri)
{
    uri base{"http://example.com/base/path/"};
    uri relative{"subdir/file"};
    uri resolved = base.resolve(relative);
    
    EXPECT_EQ("http", resolved.scheme());
    EXPECT_EQ("example.com", resolved.host());
    EXPECT_EQ("/base/path/subdir/file", resolved.path().string());
}

// -----------------------
// -- Complex URIs --
// -----------------------

TEST_F(UriUnitTest, complex_uri_parsing)
{
    // ftp://user:password@host:21/path/to/file?mode=binary#top
    uri u{"ftp://user:password@host:21/path/to/file?mode=binary#top"};
    
    EXPECT_EQ("ftp", u.scheme());
    EXPECT_TRUE(u.user_info().has_value());
    EXPECT_EQ("user:password", u.user_info().value());
    EXPECT_EQ("host", u.host());
    EXPECT_TRUE(u.port().has_value());
    EXPECT_EQ(21, u.port().value());
    EXPECT_EQ("/path/to/file", u.path().string());
    
    auto params = u.query_params();
    ASSERT_EQ(1, params.size());
    EXPECT_EQ("mode", params[0].first);
    EXPECT_EQ("binary", params[0].second);
    
    EXPECT_TRUE(u.fragment().has_value());
    EXPECT_EQ("top", u.fragment().value());
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
