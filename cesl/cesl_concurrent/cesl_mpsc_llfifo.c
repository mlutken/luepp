#include "cesl_mpsc_llfifo.h"
#include <cesl_compiler/cesl_atomic.h>
#include <string.h>
#include<stdint.h>

// -----------------------------
// PRIVATE: Helper functions ---
// -----------------------------
size_t cesl_mpsc_llfifo_inc_index (cesl_mpsc_llfifo_t* self, size_t index)
{
    // NOTE: For modulo of power of two it can be expressed like this:
    // x % 2 inpower n == x & (2 inpower n - 1).

    return (index +1) & (self->elems_max_count_ - 1);
}

/** Get pointer (inside fifo_buffer) to the atomic write flag for the element*/
char* cesl_mpsc_llfifo_get_ptr_to_write_index_flag (cesl_mpsc_llfifo_t* self, size_t index)
{
    return self->fifo_buffer_ + (index * self->elem_max_size_);
}

/** Get pointer (inside fifo_buffer) to the index requested */
char* cesl_mpsc_llfifo_get_ptr_to_index (cesl_mpsc_llfifo_t* self, size_t index)
{
    return self->fifo_buffer_ + (index * self->elem_max_size_) + cesl_sizeof_atomic_flag;
}

/** Get read_index atomically */
static inline size_t cesl_mpsc_llfifo_read_index (cesl_mpsc_llfifo_t* self)
{
    return cesl_atomic_load(&(self->read_index_));
}

/** Get write_index atomically */
static inline size_t cesl_mpsc_llfifo_write_index (cesl_mpsc_llfifo_t* self)
{
    return cesl_atomic_load(&(self->write_index_));
}

/** Set read_index atomically */
static inline void cesl_mpsc_llfifo_read_index_set (cesl_mpsc_llfifo_t* self, size_t read_index )
{
    cesl_atomic_store(&(self->read_index_), read_index);
}

/** Set write_index atomically */
static inline void cesl_mpsc_llfifo_write_index_set (cesl_mpsc_llfifo_t* self, size_t write_index )
{
    cesl_atomic_store(&(self->write_index_), write_index);
}

// ---------------------
// PUBLIC: functions ---
// ---------------------

int cesl_mpsc_llfifo_create(cesl_mpsc_llfifo_t* self, size_t elem_max_size, size_t elems_max_count, char* fifo_buffer)
{
    // Check that elems_max_count is a power of 2!
    if( (elems_max_count & (elems_max_count-1)) != 0) {
        return 0;
    }

    self->elem_max_size_ = elem_max_size;
    self->elems_max_count_ = elems_max_count;
    self->fifo_buffer_ = fifo_buffer;
    cesl_mpsc_llfifo_write_index_set(self, 0);
    cesl_mpsc_llfifo_read_index_set(self, 0);
    return 1;
}

int cesl_mpsc_llfifo_empty (cesl_mpsc_llfifo_t* self)
{
    return (cesl_mpsc_llfifo_write_index(self) == cesl_mpsc_llfifo_read_index(self));
}

char*  cesl_mpsc_llfifo_front (cesl_mpsc_llfifo_t* self)
{
    return cesl_mpsc_llfifo_get_ptr_to_index(self, cesl_mpsc_llfifo_read_index(self));
}

int cesl_mpsc_llfifo_full (cesl_mpsc_llfifo_t* self)
{
    size_t write_check = cesl_mpsc_llfifo_inc_index(self, cesl_mpsc_llfifo_write_index(self));
    return (write_check == cesl_mpsc_llfifo_read_index(self));
}

size_t cesl_mpsc_llfifo_size(cesl_mpsc_llfifo_t* self)
{
    int64_t sz = (int64_t)cesl_mpsc_llfifo_write_index(self) - (int64_t)cesl_mpsc_llfifo_read_index(self);
    return sz >= 0 ? (size_t)sz : (size_t)(- sz -1);
}

int cesl_mpsc_llfifo_push (cesl_mpsc_llfifo_t* self, const char* elm_ptr, size_t elem_size )
{
    elem_size = elem_size == 0 ? self->elem_max_size_ : elem_size;

    size_t next_write_index = cesl_mpsc_llfifo_inc_index(self, cesl_mpsc_llfifo_write_index(self));
    if ( next_write_index != cesl_mpsc_llfifo_read_index(self) ) {
        char* dst_ptr = cesl_mpsc_llfifo_get_ptr_to_index(self, cesl_mpsc_llfifo_write_index(self));
        memcpy (dst_ptr, elm_ptr, elem_size);
        cesl_mpsc_llfifo_write_index_set(self, next_write_index);
        return 1;
    }
    return 0;
}

int cesl_mpsc_llfifo_pop (cesl_mpsc_llfifo_t* self)
{
    if (cesl_mpsc_llfifo_empty (self)) {
        return 0;
    }
    cesl_mpsc_llfifo_read_index_set(self, cesl_mpsc_llfifo_inc_index(self, cesl_mpsc_llfifo_read_index(self)));
    return 1;
}
