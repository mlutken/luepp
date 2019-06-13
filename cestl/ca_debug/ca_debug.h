/**@file
 * ca_debug Debugging functions
 */
#ifndef CA_STD_CA_DEBUG_H
#define CA_STD_CA_DEBUG_H
#include <ca_compiler/ca_extern_c.h>
#include <assert.h>

#ifdef PRODUCTION_RELEASE
// TODO: Perhaps support a way to get this assert message to a tracelog in production mode
#   define ca_assert(expression, message)
#   define ca_dprintf(fmt, ...)
#else
#   define ca_assert(expression, message) assert((expression)&&(message))
    extern_C int ca_dprintf(const char *fmt, ...);
#endif

#endif // CA_STD_CA_DEBUG_H
