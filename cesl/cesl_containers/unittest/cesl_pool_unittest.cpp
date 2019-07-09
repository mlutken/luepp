#include <stdio.h>
#include <iostream>
#include <string>
#include <gtest/gtest.h>
#include <cesl_containers/cesl_pool.h>

constexpr int my_pool_elem_max_size = 1024;
constexpr int my_pool_elems_max_count = 4;

static char my_pool_buffer_g[my_pool_elems_max_count][my_pool_elem_max_size];
static cesl_pool_t my_pool_g;

static int test_llfifo_push(const char* str)
{
    return llfifo_push(&my_pool_g, str, strlen(str));
}

// static char* test_llfifo_front_strcpy(char* dst_str)
// {
//     return strncpy(dst_str, llfifo_front(&my_pool_g), my_pool_elem_max_size);
// }

TEST(cesl_pool_unittest, create)
{
    const auto res = llfifo_create(&my_pool_g, my_pool_elem_max_size, my_pool_elems_max_count, (char*)my_pool_buffer_g);
    EXPECT_EQ(1, res);
    EXPECT_EQ(0, llfifo_size(&my_pool_g));
    EXPECT_EQ(0, llfifo_full(&my_pool_g));
    EXPECT_EQ(1, llfifo_empty(&my_pool_g));
}

