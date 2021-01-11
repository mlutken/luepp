#include "cesl_fifo.h"
#include <cesl_compiler/cesl_atomic.h>
#include <string.h>
#include <stdint.h>

// NOT ready!!!!

// -----------------------------
// PRIVATE: Helper functions ---
// -----------------------------
size_t  cesl_fifo_inc_index (cesl_fifo_t* self, size_t index)
{
    // NOTE: For modulo of power of two it can be expressed like this:
    // x % 2 inpower n == x & (2 inpower n - 1).

    return (index +1) & (self->elems_max_count_ - 1);
}

/** Get pointer (inside fifo_buffer) to the index requested */
char*  cesl_fifo_get_ptr_to_index (cesl_fifo_t* self, size_t index)
{
    return self->buffer_ + (index * self->elem_max_size_);
}

/** Get read_index atomically */
static inline size_t  cesl_fifo_read_index (cesl_fifo_t* self)
{
    return self->read_index_;
}

/** Get write_index atomically */
static inline size_t  cesl_fifo_write_index (cesl_fifo_t* self)
{
    return cesl_atomic_load(&(self->write_index_));
}

/** Set read_index atomically */
static inline void  cesl_fifo_read_index_set (cesl_fifo_t* self, size_t read_index )
{
    cesl_atomic_store(&(self->read_index_), read_index);
}

/** Set write_index atomically */
static inline void cesl_fifo_write_index_set (cesl_fifo_t* self, size_t write_index )
{
    cesl_atomic_store(&(self->write_index_), write_index);
}

// ---------------------
// PUBLIC: functions ---
// ---------------------

int cesl_fifo_create(cesl_fifo_t* self, size_t elem_max_size, size_t elems_max_count, char* fifo_buffer)
{
    // Check that elems_max_count is a power of 2!
    if( (elems_max_count & (elems_max_count-1)) != 0) {
        return 0;
    }

    self->elem_max_size_ = elem_max_size;
    self->elems_max_count_ = elems_max_count;
    self->buffer_ = fifo_buffer;
    cesl_fifo_write_index_set(self, 0);
    cesl_fifo_read_index_set(self, 0);
    return 1;
}

int cesl_fifo_empty (cesl_fifo_t* self)
{
    return (cesl_fifo_write_index(self) == cesl_fifo_read_index(self));
}

char*  cesl_fifo_front (cesl_fifo_t* self)
{
    return cesl_fifo_get_ptr_to_index(self, cesl_fifo_read_index(self));
}

int cesl_fifo_full (cesl_fifo_t* self)
{
    size_t write_check = cesl_fifo_inc_index(self, cesl_fifo_write_index(self));
    return (write_check == cesl_fifo_read_index(self));
}

size_t cesl_fifo_size(cesl_fifo_t* self)
{
    int64_t sz = (int64_t)cesl_fifo_write_index(self) - (int64_t)cesl_fifo_read_index(self);
    return sz >= 0 ? (size_t)sz : (size_t)(- sz -1);
}

int cesl_fifo_push (cesl_fifo_t* self, const char* elm_ptr, size_t elem_size )
{
    elem_size = elem_size == 0 ? self->elem_max_size_ : elem_size;

    size_t next_write_index = cesl_fifo_inc_index(self, cesl_fifo_write_index(self));
    if ( next_write_index != cesl_fifo_read_index(self) ) {
        char* dst_ptr = cesl_fifo_get_ptr_to_index(self, cesl_fifo_write_index(self));
        memcpy (dst_ptr, elm_ptr, elem_size);
        cesl_fifo_write_index_set(self, next_write_index);
        return 1;
    }
    return 0;
}

int cesl_fifo_pop (cesl_fifo_t* self)
{
    if (cesl_fifo_empty (self)) {
        return 0;
    }
    cesl_fifo_read_index_set(self, cesl_fifo_inc_index(self, cesl_fifo_read_index(self)));
    return 1;
}
