#include <my_dependant_module/my_dependant_module.h>
#include <cesl_debug/cesl_debug.h>
#include <string.h>


int main()
{
    cesl_dprintf("Hello from my_dependant_module_playground\n");

    cesl_dprintf("my_funky_calculation(2,3): %d\n", my_funky_calculation(2,3));
    cesl_dprintf("my_funky_calculation(3,5): %d\n", my_funky_calculation(3,5));
    cesl_dprintf("my_funky_calculation(3,3): %d\n", my_funky_calculation(3,3));
    return 0;
}
