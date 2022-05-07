/**
 * @file my_base_module
 * Collection of primitive integer math and geometry functions.
 *
*/
#ifndef EXAMPLES_MY_BASE_MODULE_H
#define EXAMPLES_MY_BASE_MODULE_H

// For extern_C macro
#include <cesl_compiler/cesl_extern_c.h>

/**
 * @brief int_multiply  Multiply two integers
 * @param lhs           Left hand side factor
 * @param rhs           Right hand side factor
 * @return int          Returns the multiplication of the two arguments.
 *
 * There is no check for overflow!
 */
extern_C int int_multiply(int lhs, int rhs);

/**
 * @brief vec3_len2     Length of vector squared
 * @param x             X-coordinate
 * @param y             Y-coordinate
 * @param z             Z-coordinate
 * @return int          Squared length of the vector.
 * @sa vec3_len         Get the actual (rounded) length of the vector.
 *
 * Function to compute the squared length of a 3D vector!
 */
extern_C int vec3_len2(int x, int y, int z);

/**
 * @brief vec3_len      Length of vector rounded to nearest integer
 * @param x             X-coordinate
 * @param y             Y-coordinate
 * @param z             Z-coordinate
 * @return int          Squared length of the vector.
 * @sa vec3_len2        Get the squared length of the vector (faster).
 *
 * Function to compute the length of a 3D vector rounding the result to
 * nearest integer!
 */
extern_C int vec3_len(int x, int y, int z);


#endif /* EXAMPLES_MY_BASE_MODULE_H */
