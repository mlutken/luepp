#include "cesl_llfifo.h"
#include <cesl_compiler/cesl_atomic.h>
#include <string.h>
#include<stdint.h>

// -----------------------------
// PRIVATE: Helper functions ---
// -----------------------------

// ---------------------
// PUBLIC: functions ---
// ---------------------

int cesl_pool_create(cesl_pool_t* self, size_t elem_max_size, size_t elems_max_count, char* fifo_buffer)
{
    // Check that elems_max_count is a power of 2!
    if( (elems_max_count & (elems_max_count-1)) != 0) {
        return 0;
    }

    self->elem_max_size_ = elem_max_size;
    self->elems_max_count_ = elems_max_count;
    self->fifo_buffer_ = fifo_buffer;
    cesl_pool_write_index_set(self, 0);
    cesl_pool_read_index_set(self, 0);
    return 1;
}

int cesl_pool_empty (cesl_pool_t* self)
{
    return (cesl_pool_write_index(self) == cesl_pool_read_index(self));
}

char*  cesl_pool_front (cesl_pool_t* self)
{
    return cesl_pool_get_ptr_to_index(self, cesl_pool_read_index(self));
}

int cesl_pool_full (cesl_pool_t* self)
{
    size_t write_check = cesl_pool_inc_index(self, cesl_pool_write_index(self));
    return (write_check == cesl_pool_read_index(self));
}

size_t cesl_pool_size(cesl_pool_t* self)
{
    int64_t sz = (int64_t)cesl_pool_write_index(self) - (int64_t)cesl_pool_read_index(self);
    return sz >= 0 ? (size_t)sz : (size_t)(- sz -1);
}

int cesl_pool_push (cesl_pool_t* self, const char* elm_ptr, size_t elem_size )
{
    elem_size = elem_size == 0 ? self->elem_max_size_ : elem_size;

    size_t next_write_index = cesl_pool_inc_index(self, cesl_pool_write_index(self));
    if ( next_write_index != cesl_pool_read_index(self) ) {
        char* dst_ptr = cesl_pool_get_ptr_to_index(self, cesl_pool_write_index(self));
        memcpy (dst_ptr, elm_ptr, elem_size);
        cesl_pool_write_index_set(self, next_write_index);
        return 1;
    }
    return 0;
}

int cesl_pool_pop (cesl_pool_t* self)
{
    if (cesl_pool_empty (self)) {
        return 0;
    }
    cesl_pool_read_index_set(self, cesl_pool_inc_index(self, cesl_pool_read_index(self)));
    return 1;
}
