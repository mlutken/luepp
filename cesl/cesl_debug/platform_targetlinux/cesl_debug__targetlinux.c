#include "../cesl_debug.h"
#include <stdarg.h>
#include <stdio.h>

int cesl_dprintf(const char *fmt, ...)
{
    int             len;
    va_list         args;

    va_start(args, fmt);
    len = vprintf(fmt, args);
    va_end(args);

    return len;
}
