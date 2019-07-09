#include "cesl_vector_s.h"
#include <cesl_compiler/cesl_atomic.h>
#include <string.h>
#include<stdint.h>

// -----------------------------
// PRIVATE: Helper functions ---
// -----------------------------
// ---------------------
// PUBLIC: functions ---
// ---------------------

int cesl_vector_s_create(cesl_vector_s_t* self, size_t elem_max_size, size_t elems_max_count, char* fifo_buffer)
{
    // Check that elems_max_count is a power of 2!
    if( (elems_max_count & (elems_max_count-1)) != 0) {
        return 0;
    }

    self->elem_max_size_ = elem_max_size;
    self->elems_max_count_ = elems_max_count;
    self->fifo_buffer_ = fifo_buffer;
    return 1;
}

int cesl_vector_s_empty (cesl_vector_s_t* self)
{
    return 0;
}

char*  cesl_vector_s_front (cesl_vector_s_t* self)
{
    return 0;
}

int cesl_vector_s_full (cesl_vector_s_t* self)
{
    return 0;
}

size_t cesl_vector_s_size(cesl_vector_s_t* self)
{
    return 0;
}

int cesl_vector_s_push (cesl_vector_s_t* self, const char* elm_ptr, size_t elem_size )
{
    return 0;
}

int cesl_vector_s_pop (cesl_vector_s_t* self)
{
    return 0;
}
