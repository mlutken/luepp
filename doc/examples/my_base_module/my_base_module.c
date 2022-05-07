#include <math.h>

#include "my_base_module.h"
// -----------------------------------------------------
// --- PRIVATE Helper functions forward declarations ---
// -----------------------------------------------------
static int int_square_root(int val);

// ----------------------------------------
// --- PUBLIC functions implementations ---
// ----------------------------------------
int int_multiply(int lhs, int rhs)
{
    return lhs*rhs;
}

int vec3_len2(int x, int y, int z)
{
    const int x2 = int_multiply(x,x);
    const int y2 = int_multiply(y,y);
    const int z2 = int_multiply(z,z);
    return x2 + y2 + z2;
}

int vec3_len(int x, int y, int z)
{
    const int len2 = vec3_len2(x, y, z);
    return int_square_root(len2);
}

// ------------------------------------------------
// --- PRIVATE Helper functions implementations ---
// ------------------------------------------------

/**
 * @brief int_square_root   Integer square root function
 * @param val               Value to take the square root of.
 * @return int              Return rounded (integer) square root.
 *
 * Function to compute the square root of an integer value!
 */
static int int_square_root(int val)
{
    return (int)(sqrt((double)val) + 0.5);
}
