#include <cesl_debug/cesl_debug.h>
#include <my_base_module/my_base_module.h>
#include <string.h>


int main()
{
    cesl_dprintf("Hello from my_base_module_playground\n");

    cesl_dprintf("int_multiply(12,3): %d\n", int_multiply(12,3));
    cesl_dprintf("vec3_len2(2,3,4)  : %d\n", vec3_len2(2,3,4));
    cesl_dprintf("vec3_len(2,3,4)   : %d\n", vec3_len(2,3,4));

    return 0;
}
