/**@file
 * cesl_vector_s: STL like vector with a max size defined at
 * creation time of the vector.
*/
#ifndef CESL_CONCURRENT_cesl_vector_s_H
#define CESL_CONCURRENT_cesl_vector_s_H

#include <cesl_compiler/cesl_extern_c.h>
#include <stddef.h>

// -----------------
// PUBLIC: types ---
// -----------------

typedef struct {
    char*                           buffer_;
    size_t                          elem_max_size_;
    size_t                          elems_max_count_;
    size_t                          elems_count_;
} cesl_vector_s_t;

// ---------------------
// PUBLIC: functions ---
// ---------------------

extern_C int    cesl_vector_s_create            (cesl_vector_s_t* self, size_t elem_max_size,
                                                 size_t elems_max_count, char* fifo_buffer);
extern_C void   cesl_vector_s_clear             (cesl_vector_s_t* self);
extern_C int    cesl_vector_s_empty             (cesl_vector_s_t* self);
extern_C char*  cesl_vector_s_front             (cesl_vector_s_t* self);
extern_C char*  cesl_vector_s_back              (cesl_vector_s_t* self);
extern_C char*  cesl_vector_s_at                (cesl_vector_s_t* self, size_t index);
extern_C char*  cesl_vector_s_get               (cesl_vector_s_t *self, size_t index);
extern_C int    cesl_vector_s_full              (cesl_vector_s_t* self);
extern_C size_t cesl_vector_s_size              (cesl_vector_s_t* self);
extern_C int    cesl_vector_s_insert            (cesl_vector_s_t* self, size_t index, const char* elm_ptr);
extern_C int    cesl_vector_s_insert_size       (cesl_vector_s_t* self, size_t index, const char* elm_ptr, size_t elem_size);
//extern_C int    cesl_vector_s_insert_range      (cesl_vector_s_t* self, size_t index, const char* elm_ptr, size_t count);
extern_C int    cesl_vector_s_push_back         (cesl_vector_s_t* self, const char* elm_ptr);
extern_C int    cesl_vector_s_push_back_size    (cesl_vector_s_t* self, const char* elm_ptr, size_t elem_size);
extern_C void   cesl_vector_s_pop_back          (cesl_vector_s_t* self);

#endif //CESL_CONCURRENT_cesl_vector_s_H
