/**@file
 * cesl_debug Debugging functions
 */
#ifndef CESL_DEBUG_CESL_DEBUG_H
#define CESL_DEBUG_CESL_DEBUG_H
#include <cesl_compiler/cesl_extern_c.h>
#include <assert.h>

#ifdef PRODUCTION_RELEASE
// TODO: Perhaps support a way to get this assert message to a tracelog in production mode
#   define cesl_assert(expression, message)
#   define cesl_dprintf(fmt, ...)
#else
#   define cesl_assert(expression, message) assert((expression)&&(message))
    extern_C int cesl_dprintf(const char *fmt, ...);
#endif

#endif // CESL_DEBUG_CESL_DEBUG_H
