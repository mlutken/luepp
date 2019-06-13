/**@file
*/
#ifndef CA_STRINGS_CA_FORMAT_H
#define CA_STRINGS_CA_FORMAT_H

#include <ca_compiler/ca_extern_c.h>
#include <ca_compiler/ca_align_macros.h>
#include <stddef.h>
#include <stdint.h>

// -----------------
// PUBLIC: types ---
// -----------------

#define ca_format_buffer_full_error 1


struct ca_format_t
{
    size_t      buf_max_size;
    size_t      cur_pos;
    int32_t     error_code;
    char*       buf;

    struct ca_format_t* (*rst)(struct ca_format_t* self);
    struct ca_format_t* (*dbg)(struct ca_format_t* self);
    struct ca_format_t* (*print)(struct ca_format_t* self);
    struct ca_format_t* (*s)(struct ca_format_t* self, const char* string);
    struct ca_format_t* (*i64)(struct ca_format_t* self, int64_t value);
    struct ca_format_t* (*i32)(struct ca_format_t* self, int32_t value);
    struct ca_format_t* (*i)(struct ca_format_t* self, int value);
    struct ca_format_t* (*l)(struct ca_format_t* self, long value);
    struct ca_format_t* (*ui64)(struct ca_format_t* self, uint64_t value);
    struct ca_format_t* (*ui32)(struct ca_format_t* self, uint32_t value);
    struct ca_format_t* (*ui)(struct ca_format_t* self, unsigned int value);
    struct ca_format_t* (*ul)(struct ca_format_t* self, unsigned long value);
};
typedef struct ca_format_t ca_format_t;

// ---------------------
// PUBLIC: functions ---
// ---------------------

extern_C ca_format_t*   ca_format_create    (ca_format_t* self, size_t buf_max_size,
                                             char* format_buffer);
extern_C ca_format_t*   ca_format_rst       (ca_format_t* self);
extern_C ca_format_t*   ca_format_dbg       (ca_format_t* self);
extern_C ca_format_t*   ca_format_print     (ca_format_t* self);
extern_C ca_format_t*   ca_format_s         (ca_format_t* self, const char* string);
extern_C ca_format_t*   ca_format_i64       (ca_format_t* self, int64_t value);
extern_C ca_format_t*   ca_format_i32       (ca_format_t* self, int32_t value);
extern_C ca_format_t*   ca_format_l         (ca_format_t *self, long value);
extern_C ca_format_t*   ca_format_i         (ca_format_t *self, int value);
extern_C ca_format_t*   ca_format_ui64      (ca_format_t *self, uint64_t value);
extern_C ca_format_t*   ca_format_ui32      (ca_format_t *self, uint32_t value);
extern_C ca_format_t*   ca_format_ul        (ca_format_t *self, unsigned long value);
extern_C ca_format_t*   ca_format_ui        (ca_format_t *self, unsigned int value);

// PUBLIC: Convenience functions
extern_C ca_format_t*   rst                 (ca_format_t* self);
extern_C ca_format_t*   dbg                 (ca_format_t* self);

#endif //CA_STRINGS_CA_FORMAT_H
