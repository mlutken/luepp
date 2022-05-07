#include "my_syslog.h"
#include <cesl_debug/cesl_debug.h>

// ----------------------------------------
// --- PUBLIC functions implementations ---
// ----------------------------------------

cesl_format_t* my_syslog_format_create(my_syslog_format_t* self, size_t buf_max_size, char* format_buffer)
{
    cesl_format_t* base = (cesl_format_t*)self;
    cesl_format_create(base, buf_max_size, format_buffer);
    return base;
}

cesl_format_t* my_syslog_log(cesl_format_t* base, uint32_t level)
{
    cesl_format_rst(base);
    my_syslog_format_t* self = (my_syslog_format_t*)base;
    self->level = level;
    base->print = my_syslog_print;
    return base;
}

cesl_format_t* syslog(cesl_format_t* base, uint32_t level)
{
    return my_syslog_log(base, level);
}

/** Simple demo of custom printing for a syslog.
In a real system one could for example use the lockless FIFO
buffer to push into and then have a separate thread do the actual
writing to dik or where the log file needs to end up.*/
cesl_format_t* my_syslog_print(cesl_format_t* base)
{
    my_syslog_format_t* self= (my_syslog_format_t*)base;
    switch (self->level) {
    case log_err : cesl_dprintf("ERROR: "); break;
    case log_warn : cesl_dprintf("WARNING: "); break;
    case log_info : cesl_dprintf("INFO: "); break;
    default:
        cesl_dprintf("UNKNOWN: "); break;
    }
    cesl_dprintf(base->buf);
    return base;
}


