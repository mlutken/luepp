#include <cesl_strings/cesl_format.h>

/** @file Example showing how to extend the format system
 * with custom logging capabilities.
 *
 *
 *
*/

// ------------------------------------
// HEADER: types (cesl_log_format.h ) ---
// ------------------------------------

struct cesl_log_format_t
{
    struct cesl_format_t base;    // "Derive" from cesl_format_t
    struct cesl_log_format_t* (*log)(struct cesl_log_format_t* self);
};
typedef struct cesl_log_format_t cesl_log_format_t;

// ----------------------------------------
// HEADER: functions (cesl_log_format.h ) ---
// ----------------------------------------

extern_C cesl_log_format_t* cesl_log_format_create(cesl_log_format_t* self, size_t buf_max_size,
                                               char* format_buffer);

extern_C cesl_log_format_t* cesl_log_format_log (cesl_log_format_t* self, uint32_t log_level);

// ------------------------------------------------
// IMPLEMENTATION: functions (cesl_log_format.c ) ---
// ------------------------------------------------

cesl_log_format_t* cesl_log_format_create(cesl_log_format_t* self, size_t buf_max_size, char* format_buffer)
{
    cesl_format_create((cesl_format_t*)self, buf_max_size, format_buffer);
    // Assign derived "member" functions
    self->log = cesl_log_format_log;

    return self;
}

cesl_log_format_t* cesl_log_format_log (cesl_log_format_t* self)
{
    cesl_dprintf(self->buf);
    return self;
}
