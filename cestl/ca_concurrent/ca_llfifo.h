/**@file
 * ca_llfifo Lockless FIFO (ring) buffer in C supporting
 * one producer and one consumer.
*/
#ifndef CA_CONCURRENT_CA_LLFIFO_H
#define CA_CONCURRENT_CA_LLFIFO_H

#include <ca_compiler/ca_extern_c.h>
#include <ca_compiler/ca_align_macros.h>
#include <stddef.h>

CA_ALIGNED_TYPE(size_t, 64) ca_llfifo_aligned64_size_t;

// -----------------
// PUBLIC: types ---
// -----------------

typedef struct {
    ca_llfifo_aligned64_size_t  write_index_;   // Aligning to avoid "false sharing"
    ca_llfifo_aligned64_size_t  read_index_;    // Aligning to avoid "false sharing"
    char*                       fifo_buffer_;
    size_t                      elem_max_size_;
    size_t                      elems_max_count_;
} llfifo_t;

// ---------------------
// PUBLIC: functions ---
// ---------------------

extern_C int    llfifo_create   (llfifo_t* self, size_t elem_max_size,
                                 size_t elems_max_count, char* fifo_buffer);
extern_C int    llfifo_empty    (llfifo_t* self);
extern_C char*  llfifo_front    (llfifo_t* self);
extern_C int    llfifo_full     (llfifo_t* self);
extern_C size_t llfifo_size     (llfifo_t* self);
extern_C int    llfifo_push     (llfifo_t* self, const char* elm_ptr, size_t elem_size);
extern_C int    llfifo_pop      (llfifo_t* self);

#endif //CA_CONCURRENT_CA_LLFIFO_H
