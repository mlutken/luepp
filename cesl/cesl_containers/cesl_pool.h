/**@file
 * cesl_pool: Pool for managing resources like memory or objects.
 * Supports allocating and freeing "elements".
 * Allocating and freeing a resource is linear complexity (of max elements).
*/
#ifndef CESL_CONTAINERS_POOL_H
#define CESL_CONTAINERS_POOL_H

#include <cesl_compiler/cesl_extern_c.h>
#include <stddef.h>


// -----------------
// PUBLIC: types ---
// -----------------

typedef struct {
    char*                           fifo_buffer_;
    size_t                          elem_max_size_;
    size_t                          elems_max_count_;
} cesl_pool_t;

// ---------------------
// PUBLIC: functions ---
// ---------------------

extern_C int    cesl_pool_create   (cesl_pool_t* self, size_t elem_max_size,
                                 size_t elems_max_count, char* fifo_buffer);
extern_C int    cesl_pool_empty    (cesl_pool_t* self);
extern_C char*  cesl_pool_front    (cesl_pool_t* self);
extern_C int    cesl_pool_full     (cesl_pool_t* self);
extern_C size_t cesl_pool_size     (cesl_pool_t* self);
extern_C int    cesl_pool_push     (cesl_pool_t* self, const char* elm_ptr, size_t elem_size);
extern_C int    cesl_pool_pop      (cesl_pool_t* self);

#endif //CESL_CONTAINERS_POOL_H
