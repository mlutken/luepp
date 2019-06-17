#include <cesl_debug/cesl_debug.h>
#include <cesl_strings/cesl_number_to_string.h>

int main()
{
    cesl_dprintf("cesl_number_to_string_playground\n");
    char dst_str[128];
    cesl_int64_to_str(23, dst_str, 10);
    cesl_dprintf("cesl_int64_to_str(23): %s\n", dst_str);
    cesl_int64_to_str(-45677, dst_str, 10);
    cesl_dprintf("cesl_int64_to_str(-45677): %s\n", dst_str);

    cesl_uint64_to_str(63, dst_str, 10);
    cesl_dprintf("cesl_int64_to_str(63): %s\n", dst_str);

    cesl_int_to_str(123, dst_str, 10);
    cesl_dprintf("cesl_int_to_str(123): %s\n", dst_str);

    cesl_int_to_str(-123, dst_str, 10);
    cesl_dprintf("cesl_int_to_str(-123): %s\n", dst_str);
    return 0;
}
