#ifndef EXAMPLES_MY_BASE_MODULE_MOCK_H
#define EXAMPLES_MY_BASE_MODULE_MOCK_H
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <cesl_gmock/cesl_gmock_free_func.h>


// ---------------------------------------------
// --- PUBLIC Mocking for my_base_module ---
// ---------------------------------------------
#define FUNCTION_LIST_my_base_module_(F, ...) \
    F(2, int_multiply,              int,    int,    int,            ## __VA_ARGS__)\
    F(3, vec3_len2,                 int,    int,    int,    int,    ## __VA_ARGS__)\
    F(3, vec3_len,                  int,    int,    int,    int,    ## __VA_ARGS__)

GMOCK_FREE_FUNC_DECLARATION(my_base_module_)


#endif /* EXAMPLES_MY_BASE_MODULE_MOCK_H */
