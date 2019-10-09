/**@file
 * cesl_llfifo Lockless FIFO (ring) buffer in C supporting
 * one producer and one consumer.
*/
#ifndef CESL_CONCURRENT_LLFIFO_H
#define CESL_CONCURRENT_LLFIFO_H

#include <cesl_compiler/cesl_extern_c.h>
#include <cesl_compiler/cesl_align_macros.h>
#include <stddef.h>

CESL_ALIGNED_TYPE(size_t, 64) cesl_llfifo_aligned64_size_t;

// -----------------
// PUBLIC: types ---
// -----------------

typedef struct {
    cesl_llfifo_aligned64_size_t    write_index_;   // Aligning to avoid "false sharing"
    cesl_llfifo_aligned64_size_t    read_index_;    // Aligning to avoid "false sharing"
    char*                           fifo_buffer_;
    size_t                          elem_max_size_;
    size_t                          elems_max_count_;
} llfifo_t;

// ---------------------
// PUBLIC: functions ---
// ---------------------

extern_C int    llfifo_create       (llfifo_t* self, size_t elem_max_size,
                                     size_t elems_max_count, char* fifo_buffer);
extern_C int    llfifo_empty        (llfifo_t* self);
extern_C char*  llfifo_front        (llfifo_t* self);
extern_C char*  llfifo_element      (llfifo_t* self, size_t index);
extern_C int    llfifo_full         (llfifo_t* self);
extern_C size_t llfifo_size         (llfifo_t* self);
extern_C int    llfifo_push         (llfifo_t* self, const char* elm_ptr, size_t elem_size);
extern_C int    llfifo_push_size    (llfifo_t* self, const char* elm_ptr, size_t elem_size);
extern_C int    llfifo_start_push   (llfifo_t* self, size_t* write_index);
extern_C int    llfifo_end_push     (llfifo_t* self, size_t write_index);
extern_C int    llfifo_pop          (llfifo_t* self);

#endif //CESL_CONCURRENT_LLFIFO_H
