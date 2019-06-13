#include "../ca_debug.h"
#include <stdarg.h>
#include <stdio.h>

/** TODO: Not tested on target platform yet! */
int ca_dprintf(const char *fmt, ...)
{
    int             len;
    va_list         args;

    va_start(args, fmt);
    len = vprintf(fmt, args);
    va_end(args);

    return len;
}
