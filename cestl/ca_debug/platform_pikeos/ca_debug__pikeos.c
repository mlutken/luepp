#include "../ca_debug.h"
#include <vm.h>
#include <stdarg.h>

/** TODO: Not tested on target platform yet! */
int ca_dprintf(const char *fmt, ...)
{
    int             len;
    va_list         args;

    va_start(args, fmt);
    len = vm_cprintf(fmt, args);
    va_end(args);

    return len;
}

/*
 * If you really need to call a fgunction that does not have a v-version like vprintf,
 * see https://stackoverflow.com/questions/5977326/call-printf-using-va-list
 *     void *arg = __builtin_apply_args();
    void *ret = __builtin_apply((void*)printf, arg, 100);
    __builtin_return(ret);
*/
