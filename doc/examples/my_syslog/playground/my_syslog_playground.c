#include <my_syslog/my_syslog.h>
#include <ca_debug/ca_debug.h>
#include <string.h>

#define my_syslog_buf_max_size 1024

static char my_syslog_buffer_g[my_syslog_buf_max_size];

int main()
{
    ca_dprintf("Hello from my_syslog_playground\n");
    my_syslog_format_t my_syslog_format_g;
    ca_dprintf("String format playground\n");
    ca_format_t* f = my_syslog_format_create(&my_syslog_format_g, my_syslog_buf_max_size, my_syslog_buffer_g);


    // Stream style
    ca_dprintf("\n--- Stream style ---\n");

    syslog(f, log_info)->s(f, "System booted in ")->ui(f, 23)->s(f, " seconds\n")->print(f);

    syslog(f, log_err)->s(f, "Buffer size exeeded by ")->ui(f, 12)->s(f, " bytes\n")->print(f);

    return 0;
}
