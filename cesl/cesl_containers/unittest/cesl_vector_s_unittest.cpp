#include <stdio.h>
#include <iostream>
#include <string>
#include <gtest/gtest.h>
#include <cesl_containers/cesl_vector_s.h>

const int my_vector_elem_max_size = 1024;
const int my_vector_elems_max_count = 4;

static char my_vector_buffer_g[my_vector_elems_max_count][my_vector_elem_max_size];
static cesl_vector_s_t my_vector_g;

//static int test_cesl_vector_s_push(const char* str)
//{
//    return cesl_vector_s_push(&my_vector_g, str, strlen(str));
//}

// static char* test_cesl_vector_s_front_strcpy(char* dst_str)
// {
//     return strncpy(dst_str, cesl_vector_s_front(&my_vector_g), my_vector_elem_max_size);
// }

TEST(cesl_vector_s_unittest, create)
{
    const auto res = cesl_vector_s_create(&my_vector_g, my_vector_elem_max_size, my_vector_elems_max_count, (char*)my_vector_buffer_g);
    EXPECT_EQ(1, res);
    EXPECT_EQ(0, cesl_vector_s_size(&my_vector_g));
    EXPECT_EQ(0, cesl_vector_s_full(&my_vector_g));
    EXPECT_EQ(1, cesl_vector_s_empty(&my_vector_g));
}

