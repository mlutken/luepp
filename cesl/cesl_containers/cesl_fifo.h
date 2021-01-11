#ifndef CESL_CONTAINERS_FIFO_H
#define CESL_CONTAINERS_FIFO_H

#include <cesl_compiler/cesl_extern_c.h>
#include <stddef.h>

/**@file
 * cesl_fifo: FIFO/ringbuffer for managing objects. Statically allocated.
 * @todo NOT READY !!!
*/


// -----------------
// PUBLIC: types ---
// -----------------

typedef struct {
    char*                           fifo_buffer_;
    size_t                          elem_max_size_;
    size_t                          elems_max_count_;
} cesl_fifo_t;

// ---------------------
// PUBLIC: functions ---
// ---------------------

extern_C int    cesl_fifo_create   (cesl_fifo_t* self, size_t elem_max_size,
                                    size_t elems_max_count, char* fifo_buffer);
extern_C int    cesl_fifo_empty    (cesl_fifo_t* self);
extern_C char*  cesl_fifo_front    (cesl_fifo_t* self);
extern_C int    cesl_fifo_full     (cesl_fifo_t* self);
extern_C size_t cesl_fifo_size     (cesl_fifo_t* self);
extern_C int    cesl_fifo_push     (cesl_fifo_t* self, const char* elm_ptr, size_t elem_size);
extern_C int    cesl_fifo_pop      (cesl_fifo_t* self);

#endif //CESL_CONTAINERS_FIFO_H
