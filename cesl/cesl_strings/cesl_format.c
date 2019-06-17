#include "cesl_format.h"
#include "cesl_number_to_string.h"
#include <cesl_debug/cesl_debug.h>
#include <string.h>

// -----------------------------
// PRIVATE: Helper functions ---
// -----------------------------
// ---------------------
// PUBLIC: functions ---
// ---------------------

cesl_format_t* cesl_format_create(cesl_format_t* self, size_t buf_max_size, char* format_buffer)
{
    self->buf_max_size = buf_max_size;
    self->cur_pos = 0;
    self->error_code = 0;
    self->buf = format_buffer;
    self->buf[0] = '\0';
    self->buf[buf_max_size-1] = '\0';

    // Assign "member" functions
    self->rst = cesl_format_rst;
    self->dbg = cesl_format_dbg;
    self->print = cesl_format_print;
    self->s = cesl_format_s;
    self->i64 = cesl_format_i64;
    self->i32 = cesl_format_i32;
    self->l = cesl_format_l;
    self->i = cesl_format_i;
    self->ui64 = cesl_format_ui64;
    self->ui32 = cesl_format_ui32;
    self->ul = cesl_format_ul;
    self->ui = cesl_format_ui;
    return self;
}


cesl_format_t* cesl_format_rst(cesl_format_t* self)
{
    self->cur_pos = 0;
    self->error_code = 0;
    self->buf[0] = '\0';
    self->buf[self->buf_max_size-1] = '\0';
    return self;
}

cesl_format_t* cesl_format_dbg(cesl_format_t* self)
{
    cesl_format_rst(self);
    self->print = cesl_format_print;
    return self;
}

// Default print goes to cesl_dprintf
cesl_format_t* cesl_format_print(cesl_format_t* self)
{
    cesl_dprintf(self->buf);
    return self;
}

cesl_format_t *cesl_format_s(cesl_format_t* self, const char* string)
{
    const size_t max = self->buf_max_size - self->cur_pos - 1;

    size_t src_pos = 0;
    size_t dst_pos = self->cur_pos;
    self->error_code = cesl_format_buffer_full_error;
    for (; dst_pos < max; ++dst_pos, ++src_pos) {
        if (string[src_pos] == '\0') {
            self->error_code = 0;
            break;
        }
        self->buf[dst_pos] = string[src_pos];
    }
    self->buf[dst_pos] = '\0';
    self->cur_pos = dst_pos;

    return self;
}

// ----------------------------
// Signed integer functions ---
// ----------------------------
cesl_format_t *cesl_format_i64(cesl_format_t *self, int64_t value)
{
    const size_t min_required_size = (sizeof(int64_t) * 8) / 3 + 3; // 1 for rounding, 1 for sign, 1 for '\0'

    if (self->cur_pos + min_required_size >= self->buf_max_size) {
        self->error_code = cesl_format_buffer_full_error;
        return self;
    }
    self->cur_pos += cesl_int64_to_str (value, &(self->buf[self->cur_pos]), 10);
    return self;
}

cesl_format_t *cesl_format_i32(cesl_format_t *self, int32_t value)
{
    const size_t min_required_size = (sizeof(int32_t) * 8) / 3 + 3; // 1 for rounding, 1 for sign, 1 for '\0'

    if (self->cur_pos + min_required_size >= self->buf_max_size) {
        self->error_code = cesl_format_buffer_full_error;
        return self;
    }
    self->cur_pos += cesl_int32_to_str (value, &(self->buf[self->cur_pos]), 10);
    return self;
}

cesl_format_t *cesl_format_l(cesl_format_t *self, long value)
{
    const size_t min_required_size = (sizeof(long) * 8) / 3 + 3; // 1 for rounding, 1 for sign, 1 for '\0'

    if (self->cur_pos + min_required_size >= self->buf_max_size) {
        self->error_code = cesl_format_buffer_full_error;
        return self;
    }
    self->cur_pos += cesl_long_to_str (value, &(self->buf[self->cur_pos]), 10);
    return self;
}

cesl_format_t *cesl_format_i(cesl_format_t *self, int value)
{
    const size_t min_required_size = (sizeof(int) * 8) / 3 + 3; // 1 for rounding, 1 for sign, 1 for '\0'

    if (self->cur_pos + min_required_size >= self->buf_max_size) {
        self->error_code = cesl_format_buffer_full_error;
        return self;
    }
    self->cur_pos += cesl_int_to_str (value, &(self->buf[self->cur_pos]), 10);
    return self;
}

// ------------------------------
// Unsigned integer functions ---
// ------------------------------
cesl_format_t *cesl_format_ui64(cesl_format_t *self, uint64_t value)
{
    const size_t min_required_size = (sizeof(uint64_t) * 8) / 3 + 3; // 1 for rounding, 1 for sign, 1 for '\0'

    if (self->cur_pos + min_required_size >= self->buf_max_size) {
        self->error_code = cesl_format_buffer_full_error;
        return self;
    }
    self->cur_pos += cesl_uint64_to_str (value, &(self->buf[self->cur_pos]), 10);
    return self;
}

cesl_format_t *cesl_format_ui32(cesl_format_t *self, uint32_t value)
{
    const size_t min_required_size = (sizeof(uint32_t) * 8) / 3 + 3; // 1 for rounding, 1 for sign, 1 for '\0'

    if (self->cur_pos + min_required_size >= self->buf_max_size) {
        self->error_code = cesl_format_buffer_full_error;
        return self;
    }
    self->cur_pos += cesl_uint32_to_str (value, &(self->buf[self->cur_pos]), 10);
    return self;
}

cesl_format_t *cesl_format_ul(cesl_format_t *self, unsigned long value)
{
    const size_t min_required_size = (sizeof(unsigned long) * 8) / 3 + 3; // 1 for rounding, 1 for sign, 1 for '\0'

    if (self->cur_pos + min_required_size >= self->buf_max_size) {
        self->error_code = cesl_format_buffer_full_error;
        return self;
    }
    self->cur_pos += cesl_ulong_to_str (value, &(self->buf[self->cur_pos]), 10);
    return self;
}

cesl_format_t *cesl_format_ui(cesl_format_t *self, unsigned int value)
{
    const size_t min_required_size = (sizeof(unsigned int) * 8) / 3 + 3; // 1 for rounding, 1 for sign, 1 for '\0'

    if (self->cur_pos + min_required_size >= self->buf_max_size) {
        self->error_code = cesl_format_buffer_full_error;
        return self;
    }
    self->cur_pos += cesl_uint_to_str (value, &(self->buf[self->cur_pos]), 10);
    return self;
}

cesl_format_t* rst(cesl_format_t* self)
{
    return cesl_format_rst(self);
}

cesl_format_t* dbg(cesl_format_t* self)
{
    return cesl_format_dbg(self);
}
