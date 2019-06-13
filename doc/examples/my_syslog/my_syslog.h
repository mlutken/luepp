/**
 * @file my_syslog
 * Example showing how to customixe "derive" ca_format to support
 * an easy MISRA complient way of doing logging.
 *
*/
#ifndef EXAMPLES_MY_SYSLOG_H
#define EXAMPLES_MY_SYSLOG_H

#include <ca_compiler/ca_extern_c.h>
#include <ca_strings/ca_format.h>

// -----------------
// PUBLIC: types ---
// -----------------
#define log_err 1 << 0
#define log_warn 1 << 1
#define log_info 1 << 2

struct my_syslog_format_t
{
    struct ca_format_t b;    // "Derive" from ca_format_t

    uint32_t level;
};
typedef struct my_syslog_format_t my_syslog_format_t;

// ----------------------------------------
// HEADER: functions (ca_log_format.h ) ---
// ----------------------------------------

extern_C ca_format_t* my_syslog_format_create   (my_syslog_format_t* self, size_t buf_max_size,
                                                 char* format_buffer);
extern_C ca_format_t* my_syslog_print           (ca_format_t* base);
extern_C ca_format_t* my_syslog_log             (ca_format_t* base, uint32_t level);

extern_C ca_format_t* syslog                    (ca_format_t* base, uint32_t level);


#endif /* EXAMPLES_MY_SYSLOG_H */
