/**@file
*/
#ifndef CESL_STRINGS_CESL_FORMAT_H
#define CESL_STRINGS_CESL_FORMAT_H

#include <cesl_compiler/cesl_extern_c.h>
#include <cesl_compiler/cesl_align_macros.h>
#include <stddef.h>
#include <stdint.h>

// -----------------
// PUBLIC: types ---
// -----------------

#define cesl_format_buffer_full_error 1


struct cesl_format_t
{
    size_t      buf_max_size;
    size_t      cur_pos;
    int32_t     error_code;
    char*       buf;

    struct cesl_format_t* (*rst)(struct cesl_format_t* self);
    struct cesl_format_t* (*dbg)(struct cesl_format_t* self);
    struct cesl_format_t* (*print)(struct cesl_format_t* self);
    struct cesl_format_t* (*s)(struct cesl_format_t* self, const char* string);
    struct cesl_format_t* (*i64)(struct cesl_format_t* self, int64_t value);
    struct cesl_format_t* (*i32)(struct cesl_format_t* self, int32_t value);
    struct cesl_format_t* (*i)(struct cesl_format_t* self, int value);
    struct cesl_format_t* (*l)(struct cesl_format_t* self, long value);
    struct cesl_format_t* (*ui64)(struct cesl_format_t* self, uint64_t value);
    struct cesl_format_t* (*ui32)(struct cesl_format_t* self, uint32_t value);
    struct cesl_format_t* (*ui)(struct cesl_format_t* self, unsigned int value);
    struct cesl_format_t* (*ul)(struct cesl_format_t* self, unsigned long value);
};
typedef struct cesl_format_t cesl_format_t;

// ---------------------
// PUBLIC: functions ---
// ---------------------

extern_C cesl_format_t*   cesl_format_create    (cesl_format_t* self, size_t buf_max_size,
                                             char* format_buffer);
extern_C cesl_format_t*   cesl_format_rst       (cesl_format_t* self);
extern_C cesl_format_t*   cesl_format_dbg       (cesl_format_t* self);
extern_C cesl_format_t*   cesl_format_print     (cesl_format_t* self);
extern_C cesl_format_t*   cesl_format_s         (cesl_format_t* self, const char* string);
extern_C cesl_format_t*   cesl_format_i64       (cesl_format_t* self, int64_t value);
extern_C cesl_format_t*   cesl_format_i32       (cesl_format_t* self, int32_t value);
extern_C cesl_format_t*   cesl_format_l         (cesl_format_t *self, long value);
extern_C cesl_format_t*   cesl_format_i         (cesl_format_t *self, int value);
extern_C cesl_format_t*   cesl_format_ui64      (cesl_format_t *self, uint64_t value);
extern_C cesl_format_t*   cesl_format_ui32      (cesl_format_t *self, uint32_t value);
extern_C cesl_format_t*   cesl_format_ul        (cesl_format_t *self, unsigned long value);
extern_C cesl_format_t*   cesl_format_ui        (cesl_format_t *self, unsigned int value);

// PUBLIC: Convenience functions
extern_C cesl_format_t*   rst                 (cesl_format_t* self);
extern_C cesl_format_t*   dbg                 (cesl_format_t* self);

#endif //CESL_STRINGS_CESL_FORMAT_H
