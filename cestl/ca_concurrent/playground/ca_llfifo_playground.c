#include <ca_debug/ca_debug.h>
#include <ca_concurrent/ca_llfifo.h>
#include <string.h>

#define my_fifo_elem_max_size 1024
#define my_fifo_elems_max_count 4

static char my_fifo_buffer_g[my_fifo_elems_max_count][my_fifo_elem_max_size];
static llfifo_t my_fifo_g;

static int test_llfifo_push(const char* str)
{
    return llfifo_push(&my_fifo_g, str, strlen(str));
}

static char* test_llfifo_front_strcpy(char* dst_str)
{
    return strncpy(dst_str, llfifo_front(&my_fifo_g), my_fifo_elem_max_size);
}


int main()
{
    ca_dprintf("Lockless FIFO playground\n");
    const int res = llfifo_create(&my_fifo_g, my_fifo_elem_max_size, my_fifo_elems_max_count, (char*)my_fifo_buffer_g);
    ca_dprintf("llfifo_create()         : '%d'\n", res);
    ca_dprintf("llfifo_empty()          : '%d'  llfifo_full(): '%d'\n", llfifo_empty(&my_fifo_g), llfifo_full(&my_fifo_g));

    ca_dprintf("llfifo_push('first')    : '%d'\n", test_llfifo_push("first"));
    ca_dprintf("llfifo_empty()          : '%d'  llfifo_full(): '%d'\n", llfifo_empty(&my_fifo_g), llfifo_full(&my_fifo_g));
    ca_dprintf("llfifo_push('second')   : '%d'\n", test_llfifo_push("second"));
    ca_dprintf("llfifo_empty()          : '%d'  llfifo_full(): '%d'\n", llfifo_empty(&my_fifo_g), llfifo_full(&my_fifo_g));
    ca_dprintf("llfifo_push('third')    : '%d'\n", test_llfifo_push("third"));
    ca_dprintf("llfifo_empty()          : '%d'  llfifo_full(): '%d'\n", llfifo_empty(&my_fifo_g), llfifo_full(&my_fifo_g));
    ca_dprintf("llfifo_push('fourth')   : '%d'\n", test_llfifo_push("fourth"));
    ca_dprintf("llfifo_empty()          : '%d'  llfifo_full(): '%d'\n", llfifo_empty(&my_fifo_g), llfifo_full(&my_fifo_g));

    ca_dprintf("llfifo_front()          : '%s'\n", llfifo_front(&my_fifo_g));
    llfifo_pop(&my_fifo_g);
    ca_dprintf("llfifo_empty()          : '%d'  llfifo_full(): '%d'\n", llfifo_empty(&my_fifo_g), llfifo_full(&my_fifo_g));
    ca_dprintf("llfifo_front()          : '%s'\n", llfifo_front(&my_fifo_g));
    ca_dprintf("llfifo_push('fourth')   : '%d'\n", test_llfifo_push("fourth"));
    ca_dprintf("llfifo_empty()          : '%d'  llfifo_full(): '%d'\n", llfifo_empty(&my_fifo_g), llfifo_full(&my_fifo_g));

    llfifo_pop(&my_fifo_g);
    ca_dprintf("llfifo_empty()          : '%d'  llfifo_full(): '%d'\n", llfifo_empty(&my_fifo_g), llfifo_full(&my_fifo_g));
    ca_dprintf("llfifo_front()          : '%s'\n", llfifo_front(&my_fifo_g));

    llfifo_pop(&my_fifo_g);
    ca_dprintf("llfifo_empty()          : '%d'  llfifo_full(): '%d'\n", llfifo_empty(&my_fifo_g), llfifo_full(&my_fifo_g));
    ca_dprintf("llfifo_front()          : '%s'\n", llfifo_front(&my_fifo_g));

    llfifo_pop(&my_fifo_g);
    ca_dprintf("llfifo_empty()          : '%d'  llfifo_full(): '%d'\n", llfifo_empty(&my_fifo_g), llfifo_full(&my_fifo_g));
    ca_dprintf("llfifo_front()          : '%s'\n", llfifo_front(&my_fifo_g));

    ca_dprintf("llfifo_push('fifth')   : '%d'\n", test_llfifo_push("fifth"));
    ca_dprintf("llfifo_empty()          : '%d'  llfifo_full(): '%d'\n", llfifo_empty(&my_fifo_g), llfifo_full(&my_fifo_g));
    ca_dprintf("llfifo_front()          : '%s'\n", llfifo_front(&my_fifo_g));

    return 0;
}
