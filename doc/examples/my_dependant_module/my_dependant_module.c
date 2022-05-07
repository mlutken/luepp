#include "my_dependant_module.h"
#include <my_base_module/my_base_module.h>

// ----------------------------------------
// --- PUBLIC functions implementations ---
// ----------------------------------------
int my_funky_calculation(int width, int height)
{
    const int funky1 = int_multiply(width, height);
    const int funky2 = vec3_len2(width, height, funky1);
    return vec3_len(width, height, funky2);
}
