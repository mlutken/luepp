#include <stdio.h>
#include <iostream>
#include <string>
#include <gtest/gtest.h>
#include <ca_strings/ca_format.h>

using namespace std;

TEST(ca_format_unittest, buffer_full_integer)
{
    static const unsigned buf_max = 20;
    char format_buf[buf_max];

    ca_format_t fmt;
    ca_format_t* f = ca_format_create(&fmt, buf_max, format_buf);
    f->rst(f)->i32(f, -12345678);
    EXPECT_EQ(9, f->cur_pos);
    EXPECT_EQ(string("-12345678"), f->buf);
    EXPECT_EQ(0, f->error_code);
    f->i32(f, 1);
    EXPECT_EQ(string("-12345678"), f->buf);
    EXPECT_EQ(9, f->cur_pos);
    EXPECT_EQ(1, f->error_code);
}
