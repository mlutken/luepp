#include <ca_debug/ca_debug.h>
#include <my_base_module/my_base_module.h>
#include <string.h>


int main()
{
    ca_dprintf("Hello from my_base_module_playground\n");

    ca_dprintf("int_multiply(12,3): %d\n", int_multiply(12,3));
    ca_dprintf("vec3_len2(2,3,4)  : %d\n", vec3_len2(2,3,4));
    ca_dprintf("vec3_len(2,3,4)   : %d\n", vec3_len(2,3,4));

    return 0;
}
