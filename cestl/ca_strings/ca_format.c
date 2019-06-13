#include "ca_format.h"
#include "ca_number_to_string.h"
#include <ca_debug/ca_debug.h>
#include <string.h>

// -----------------------------
// PRIVATE: Helper functions ---
// -----------------------------
// ---------------------
// PUBLIC: functions ---
// ---------------------

ca_format_t* ca_format_create(ca_format_t* self, size_t buf_max_size, char* format_buffer)
{
    self->buf_max_size = buf_max_size;
    self->cur_pos = 0;
    self->error_code = 0;
    self->buf = format_buffer;
    self->buf[0] = '\0';
    self->buf[buf_max_size-1] = '\0';

    // Assign "member" functions
    self->rst = ca_format_rst;
    self->dbg = ca_format_dbg;
    self->print = ca_format_print;
    self->s = ca_format_s;
    self->i64 = ca_format_i64;
    self->i32 = ca_format_i32;
    self->l = ca_format_l;
    self->i = ca_format_i;
    self->ui64 = ca_format_ui64;
    self->ui32 = ca_format_ui32;
    self->ul = ca_format_ul;
    self->ui = ca_format_ui;
    return self;
}


ca_format_t* ca_format_rst(ca_format_t* self)
{
    self->cur_pos = 0;
    self->error_code = 0;
    self->buf[0] = '\0';
    self->buf[self->buf_max_size-1] = '\0';
    return self;
}

ca_format_t* ca_format_dbg(ca_format_t* self)
{
    ca_format_rst(self);
    self->print = ca_format_print;
    return self;
}

// Default print goes to ca_dprintf
ca_format_t* ca_format_print(ca_format_t* self)
{
    ca_dprintf(self->buf);
    return self;
}

ca_format_t *ca_format_s(ca_format_t* self, const char* string)
{
    const size_t max = self->buf_max_size - self->cur_pos - 1;

    size_t src_pos = 0;
    size_t dst_pos = self->cur_pos;
    self->error_code = ca_format_buffer_full_error;
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
ca_format_t *ca_format_i64(ca_format_t *self, int64_t value)
{
    const size_t min_required_size = (sizeof(int64_t) * 8) / 3 + 3; // 1 for rounding, 1 for sign, 1 for '\0'

    if (self->cur_pos + min_required_size >= self->buf_max_size) {
        self->error_code = ca_format_buffer_full_error;
        return self;
    }
    self->cur_pos += ca_int64_to_str (value, &(self->buf[self->cur_pos]), 10);
    return self;
}

ca_format_t *ca_format_i32(ca_format_t *self, int32_t value)
{
    const size_t min_required_size = (sizeof(int32_t) * 8) / 3 + 3; // 1 for rounding, 1 for sign, 1 for '\0'

    if (self->cur_pos + min_required_size >= self->buf_max_size) {
        self->error_code = ca_format_buffer_full_error;
        return self;
    }
    self->cur_pos += ca_int32_to_str (value, &(self->buf[self->cur_pos]), 10);
    return self;
}

ca_format_t *ca_format_l(ca_format_t *self, long value)
{
    const size_t min_required_size = (sizeof(long) * 8) / 3 + 3; // 1 for rounding, 1 for sign, 1 for '\0'

    if (self->cur_pos + min_required_size >= self->buf_max_size) {
        self->error_code = ca_format_buffer_full_error;
        return self;
    }
    self->cur_pos += ca_long_to_str (value, &(self->buf[self->cur_pos]), 10);
    return self;
}

ca_format_t *ca_format_i(ca_format_t *self, int value)
{
    const size_t min_required_size = (sizeof(int) * 8) / 3 + 3; // 1 for rounding, 1 for sign, 1 for '\0'

    if (self->cur_pos + min_required_size >= self->buf_max_size) {
        self->error_code = ca_format_buffer_full_error;
        return self;
    }
    self->cur_pos += ca_int_to_str (value, &(self->buf[self->cur_pos]), 10);
    return self;
}

// ------------------------------
// Unsigned integer functions ---
// ------------------------------
ca_format_t *ca_format_ui64(ca_format_t *self, uint64_t value)
{
    const size_t min_required_size = (sizeof(uint64_t) * 8) / 3 + 3; // 1 for rounding, 1 for sign, 1 for '\0'

    if (self->cur_pos + min_required_size >= self->buf_max_size) {
        self->error_code = ca_format_buffer_full_error;
        return self;
    }
    self->cur_pos += ca_uint64_to_str (value, &(self->buf[self->cur_pos]), 10);
    return self;
}

ca_format_t *ca_format_ui32(ca_format_t *self, uint32_t value)
{
    const size_t min_required_size = (sizeof(uint32_t) * 8) / 3 + 3; // 1 for rounding, 1 for sign, 1 for '\0'

    if (self->cur_pos + min_required_size >= self->buf_max_size) {
        self->error_code = ca_format_buffer_full_error;
        return self;
    }
    self->cur_pos += ca_uint32_to_str (value, &(self->buf[self->cur_pos]), 10);
    return self;
}

ca_format_t *ca_format_ul(ca_format_t *self, unsigned long value)
{
    const size_t min_required_size = (sizeof(unsigned long) * 8) / 3 + 3; // 1 for rounding, 1 for sign, 1 for '\0'

    if (self->cur_pos + min_required_size >= self->buf_max_size) {
        self->error_code = ca_format_buffer_full_error;
        return self;
    }
    self->cur_pos += ca_ulong_to_str (value, &(self->buf[self->cur_pos]), 10);
    return self;
}

ca_format_t *ca_format_ui(ca_format_t *self, unsigned int value)
{
    const size_t min_required_size = (sizeof(unsigned int) * 8) / 3 + 3; // 1 for rounding, 1 for sign, 1 for '\0'

    if (self->cur_pos + min_required_size >= self->buf_max_size) {
        self->error_code = ca_format_buffer_full_error;
        return self;
    }
    self->cur_pos += ca_uint_to_str (value, &(self->buf[self->cur_pos]), 10);
    return self;
}

ca_format_t* rst(ca_format_t* self)
{
    return ca_format_rst(self);
}

ca_format_t* dbg(ca_format_t* self)
{
    return ca_format_dbg(self);
}
