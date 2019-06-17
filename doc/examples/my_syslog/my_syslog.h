/**
 * @file my_syslog
 * Example showing how to customixe "derive" cesl_format to support
 * an easy MISRA complient way of doing logging.
 *
*/
#ifndef EXAMPLES_MY_SYSLOG_H
#define EXAMPLES_MY_SYSLOG_H

#include <cesl_compiler/cesl_extern_c.h>
#include <cesl_strings/cesl_format.h>

// -----------------
// PUBLIC: types ---
// -----------------
#define log_err 1 << 0
#define log_warn 1 << 1
#define log_info 1 << 2

struct my_syslog_format_t
{
    struct cesl_format_t b;    // "Derive" from cesl_format_t

    uint32_t level;
};
typedef struct my_syslog_format_t my_syslog_format_t;

// ----------------------------------------
// HEADER: functions (cesl_log_format.h ) ---
// ----------------------------------------

extern_C cesl_format_t* my_syslog_format_create   (my_syslog_format_t* self, size_t buf_max_size,
                                                 char* format_buffer);
extern_C cesl_format_t* my_syslog_print           (cesl_format_t* base);
extern_C cesl_format_t* my_syslog_log             (cesl_format_t* base, uint32_t level);

extern_C cesl_format_t* syslog                    (cesl_format_t* base, uint32_t level);


#endif /* EXAMPLES_MY_SYSLOG_H */
