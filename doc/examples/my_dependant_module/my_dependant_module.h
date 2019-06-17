/**
 * @file my_dependant_module
 * Collection of xxx functions.
 *
*/
#ifndef EXAMPLES_MY_DEPENDANT_MODULE_H
#define EXAMPLES_MY_DEPENDANT_MODULE_H

// For extern_C macro
#include <cesl_compiler/cesl_extern_c.h>

 /**
  * @brief my_funky_calculation A funky useless calculation for demo only.
  * Width of foo
  * @param lhs           Height of foo
  * @return int          Returns the result of the funky calculation.
  * Simply meant to demonstratate how we use functions from the base library
  * (my_base_module) and how we mock out functions from my_base_module
  * in the unit test for this function.
  */
extern_C int my_funky_calculation(int width, int height);

#endif /* EXAMPLES_MY_DEPENDANT_MODULE_H */
