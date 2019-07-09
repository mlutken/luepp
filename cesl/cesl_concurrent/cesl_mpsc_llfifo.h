/**@file
 * cesl_mpsc_llfifo Multiple producer, single consumer Lockless
 * FIFO (ring) buffer in C supporting mutiple producers and one consumer.
*/
#ifndef CESL_CONCURRENT_MPSC_CESL_MPSC_LLFIFO_H
#define CESL_CONCURRENT_MPSC_CESL_MPSC_LLFIFO_H

#include <cesl_compiler/cesl_extern_c.h>
#include <cesl_compiler/cesl_align_macros.h>
#include <stddef.h>

CESL_ALIGNED_TYPE(size_t, 64) cesl_cesl_mpsc_llfifo_aligned64_size_t;

// -----------------
// PUBLIC: types ---
// -----------------

typedef struct {
    cesl_cesl_mpsc_llfifo_aligned64_size_t      write_index_;   // Aligning to avoid "false sharing"
    cesl_cesl_mpsc_llfifo_aligned64_size_t      read_index_;    // Aligning to avoid "false sharing"
    char*                                       fifo_buffer_;
    size_t                                      elem_max_size_;
    size_t                                      elems_max_count_;
} cesl_mpsc_llfifo_t;

// ---------------------
// PUBLIC: functions ---
// ---------------------

extern_C int    cesl_mpsc_llfifo_create   (cesl_mpsc_llfifo_t* self, size_t elem_max_size,
                                           size_t elems_max_count, char* fifo_buffer);
extern_C int    cesl_mpsc_llfifo_empty    (cesl_mpsc_llfifo_t* self);
extern_C char*  cesl_mpsc_llfifo_front    (cesl_mpsc_llfifo_t* self);
extern_C int    cesl_mpsc_llfifo_full     (cesl_mpsc_llfifo_t* self);
extern_C size_t cesl_mpsc_llfifo_size     (cesl_mpsc_llfifo_t* self);
extern_C int    cesl_mpsc_llfifo_push     (cesl_mpsc_llfifo_t* self, const char* elm_ptr, size_t elem_size);
extern_C int    cesl_mpsc_llfifo_pop      (cesl_mpsc_llfifo_t* self);

#endif //CESL_CONCURRENT_MPSC_CESL_MPSC_LLFIFO_H
