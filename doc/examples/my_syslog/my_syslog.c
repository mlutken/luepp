#include "my_syslog.h"
#include <ca_debug/ca_debug.h>

// ----------------------------------------
// --- PUBLIC functions implementations ---
// ----------------------------------------

ca_format_t* my_syslog_format_create(my_syslog_format_t* self, size_t buf_max_size, char* format_buffer)
{
    ca_format_t* base = (ca_format_t*)self;
    ca_format_create(base, buf_max_size, format_buffer);
    return base;
}

ca_format_t* my_syslog_log(ca_format_t* base, uint32_t level)
{
    ca_format_rst(base);
    my_syslog_format_t* self = (my_syslog_format_t*)base;
    self->level = level;
    base->print = my_syslog_print;
    return base;
}

ca_format_t* syslog(ca_format_t* base, uint32_t level)
{
    return my_syslog_log(base, level);
}

/** Simple demo of custom printing for a syslog.
In a real system one could for example use the lockless FIFO
buffer to push into and then have a separate thread do the actual
writing to dik or where the log file needs to end up.*/
ca_format_t* my_syslog_print(ca_format_t* base)
{
    my_syslog_format_t* self= (my_syslog_format_t*)base;
    switch (self->level) {
    case log_err : ca_dprintf("ERROR: "); break;
    case log_warn : ca_dprintf("WARNING: "); break;
    case log_info : ca_dprintf("INFO: "); break;
    default:
        ca_dprintf("UNKNOWN: "); break;
    }
    ca_dprintf(base->buf);
    return base;
}


