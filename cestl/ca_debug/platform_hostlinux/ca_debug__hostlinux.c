#ifndef PRODUCTION_RELEASE

#include "../ca_debug.h"
#include <stdarg.h>
#include <stdio.h>

int ca_dprintf(const char *fmt, ...)
{
    va_list         args;
    va_start(args, fmt);
    int len = vprintf(fmt, args);
    va_end(args);
    return len;
}
#endif
