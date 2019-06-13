#include <ca_debug/ca_debug.h>
#include <ca_strings/ca_number_to_string.h>

int main()
{
    ca_dprintf("ca_number_to_string_playground\n");
    char dst_str[128];
    ca_int64_to_str(23, dst_str, 10);
    ca_dprintf("ca_int64_to_str(23): %s\n", dst_str);
    ca_int64_to_str(-45677, dst_str, 10);
    ca_dprintf("ca_int64_to_str(-45677): %s\n", dst_str);

    ca_uint64_to_str(63, dst_str, 10);
    ca_dprintf("ca_int64_to_str(63): %s\n", dst_str);

    ca_int_to_str(123, dst_str, 10);
    ca_dprintf("ca_int_to_str(123): %s\n", dst_str);

    ca_int_to_str(-123, dst_str, 10);
    ca_dprintf("ca_int_to_str(-123): %s\n", dst_str);
    return 0;
}
