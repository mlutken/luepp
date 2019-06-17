#include <ca_strings/ca_format.h>

/** @file Example showing how to extend the format system
 * with custom logging capabilities.
 *
 *
 *
*/

// ------------------------------------
// HEADER: types (ca_log_format.h ) ---
// ------------------------------------

struct ca_log_format_t
{
    struct ca_format_t base;    // "Derive" from ca_format_t
    struct ca_log_format_t* (*log)(struct ca_log_format_t* self);
};
typedef struct ca_log_format_t ca_log_format_t;

// ----------------------------------------
// HEADER: functions (ca_log_format.h ) ---
// ----------------------------------------

extern_C ca_log_format_t* ca_log_format_create(ca_log_format_t* self, size_t buf_max_size,
                                               char* format_buffer);

extern_C ca_log_format_t* ca_log_format_log (ca_log_format_t* self, uint32_t log_level);

// ------------------------------------------------
// IMPLEMENTATION: functions (ca_log_format.c ) ---
// ------------------------------------------------

ca_log_format_t* ca_log_format_create(ca_log_format_t* self, size_t buf_max_size, char* format_buffer)
{
    ca_format_create((ca_format_t*)self, buf_max_size, format_buffer);
    // Assign derived "member" functions
    self->log = ca_log_format_log;

    return self;
}

ca_log_format_t* ca_log_format_log (ca_log_format_t* self)
{
    ca_dprintf(self->buf);
    return self;
}
