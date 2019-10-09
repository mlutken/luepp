#include "cesl_llfifo.h"
#include <cesl_compiler/cesl_atomic.h>
#include <string.h>
#include<stdint.h>

// -----------------------------
// PRIVATE: Helper functions ---
// -----------------------------
size_t llfifo_inc_index (llfifo_t* self, size_t index)
{
    // NOTE: For modulo of power of two it can be expressed like this:
    // x % 2 inpower n == x & (2 inpower n - 1).

    return (index +1) & (self->elems_max_count_ - 1);
}

/** Get pointer (inside fifo_buffer) to the index requested */
char* llfifo_get_ptr_to_index (llfifo_t* self, size_t index)
{
    return self->fifo_buffer_ + (index * self->elem_max_size_);
}

/** Get read_index atomically */
static inline size_t llfifo_read_index (llfifo_t* self)
{
    return cesl_atomic_load(&(self->read_index_));
}

/** Get write_index atomically */
static inline size_t llfifo_write_index (llfifo_t* self)
{
    return cesl_atomic_load(&(self->write_index_));
}

/** Set read_index atomically */
static inline void llfifo_read_index_set (llfifo_t* self, size_t read_index )
{
    cesl_atomic_store(&(self->read_index_), read_index);
}

/** Set write_index atomically */
static inline void llfifo_write_index_set (llfifo_t* self, size_t write_index )
{
    cesl_atomic_store(&(self->write_index_), write_index);
}

// ---------------------
// PUBLIC: functions ---
// ---------------------

int llfifo_create(llfifo_t* self, size_t elem_max_size, size_t elems_max_count, char* fifo_buffer)
{
    // Check that elems_max_count is a power of 2!
    if( (elems_max_count & (elems_max_count-1)) != 0) {
        return 0;
    }

    self->elem_max_size_ = elem_max_size;
    self->elems_max_count_ = elems_max_count;
    self->fifo_buffer_ = fifo_buffer;
    llfifo_write_index_set(self, 0);
    llfifo_read_index_set(self, 0);
    return 1;
}

int llfifo_empty (llfifo_t* self)
{
    return (llfifo_write_index(self) == llfifo_read_index(self));
}

char* llfifo_front (llfifo_t* self)
{
    return llfifo_get_ptr_to_index(self, llfifo_read_index(self));
}

char* llfifo_element_at (llfifo_t* self, size_t index)
{
    return llfifo_get_ptr_to_index(self, index);
}


int llfifo_full (llfifo_t* self)
{
    size_t write_check = llfifo_inc_index(self, llfifo_write_index(self));
    return (write_check == llfifo_read_index(self));
}

size_t llfifo_size(llfifo_t* self)
{
    int64_t sz = (int64_t)llfifo_write_index(self) - (int64_t)llfifo_read_index(self);
    return sz >= 0 ? (size_t)sz : (size_t)(- sz -1);
}

// TODO: Change this to not take the size parameter
int llfifo_push (llfifo_t* self, const char* elm_ptr, size_t elem_size )
{
    size_t next_write_index = llfifo_inc_index(self, llfifo_write_index(self));
    if ( next_write_index != llfifo_read_index(self) ) {
        elem_size = elem_size == 0 ? self->elem_max_size_ : elem_size;
        char* dst_ptr = llfifo_get_ptr_to_index(self, llfifo_write_index(self));
        memcpy (dst_ptr, elm_ptr, elem_size);
        llfifo_write_index_set(self, next_write_index);
        return 1;
    }
    return 0;
}

int llfifo_push_size (llfifo_t* self, const char* elm_ptr, size_t elem_size )
{
    size_t next_write_index = llfifo_inc_index(self, llfifo_write_index(self));
    if ( next_write_index != llfifo_read_index(self) ) {
        elem_size = elem_size == 0 ? self->elem_max_size_ : elem_size;
        char* dst_ptr = llfifo_get_ptr_to_index(self, llfifo_write_index(self));
        memcpy (dst_ptr, elm_ptr, elem_size);
        llfifo_write_index_set(self, next_write_index);
        return 1;
    }
    return 0;
}

int llfifo_start_push (llfifo_t* self, size_t*  write_index)
{
    *write_index = llfifo_inc_index(self, llfifo_write_index(self));
    if ( *write_index != llfifo_read_index(self) ) {
        return 1;
    }
    return 0;
}

int llfifo_end_push (llfifo_t* self, size_t write_index)
{
    llfifo_write_index_set(self, write_index);
    return 1;
}

int llfifo_pop (llfifo_t* self)
{
    if (llfifo_empty (self)) {
        return 0;
    }
    llfifo_read_index_set(self, llfifo_inc_index(self, llfifo_read_index(self)));
    return 1;
}
