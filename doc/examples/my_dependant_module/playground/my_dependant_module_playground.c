#include <my_dependant_module/my_dependant_module.h>
#include <ca_debug/ca_debug.h>
#include <string.h>


int main()
{
    ca_dprintf("Hello from my_dependant_module_playground\n");

    ca_dprintf("my_funky_calculation(2,3): %d\n", my_funky_calculation(2,3));
    ca_dprintf("my_funky_calculation(3,5): %d\n", my_funky_calculation(3,5));
    ca_dprintf("my_funky_calculation(3,3): %d\n", my_funky_calculation(3,3));
    return 0;
}
