#include <cesl_debug/cesl_debug.h>
#include <cesl_containers/cesl_fifo.h>
#include <string.h>

#define my_fifo_elem_max_size 1024
#define my_fifo_elems_max_count 4

static char my_fifo_buffer_g[my_fifo_elems_max_count][my_fifo_elem_max_size];
static cesl_fifo_t my_fifo_g;

static int test_cesl_fifo_push(const char* str)
{
    return cesl_fifo_push(&my_fifo_g, str, strlen(str));
}

// static char* test_cesl_fifo_front_strcpy(char* dst_str)
// {
//     return strncpy(dst_str, cesl_fifo_front(&my_fifo_g), my_fifo_elem_max_size);
// }


int main()
{
    cesl_dprintf("Lockless FIFO playground\n");
    const int res = cesl_fifo_create(&my_fifo_g, my_fifo_elem_max_size, my_fifo_elems_max_count, (char*)my_fifo_buffer_g);
    cesl_dprintf("cesl_fifo_create()         : '%d'\n", res);
    cesl_dprintf("cesl_fifo_empty()          : '%d'  cesl_fifo_full(): '%d'\n", cesl_fifo_empty(&my_fifo_g), cesl_fifo_full(&my_fifo_g));

    cesl_dprintf("cesl_fifo_push('first')    : '%d'\n", test_cesl_fifo_push("first"));
    cesl_dprintf("cesl_fifo_empty()          : '%d'  cesl_fifo_full(): '%d'\n", cesl_fifo_empty(&my_fifo_g), cesl_fifo_full(&my_fifo_g));
    cesl_dprintf("cesl_fifo_push('second')   : '%d'\n", test_cesl_fifo_push("second"));
    cesl_dprintf("cesl_fifo_empty()          : '%d'  cesl_fifo_full(): '%d'\n", cesl_fifo_empty(&my_fifo_g), cesl_fifo_full(&my_fifo_g));
    cesl_dprintf("cesl_fifo_push('third')    : '%d'\n", test_cesl_fifo_push("third"));
    cesl_dprintf("cesl_fifo_empty()          : '%d'  cesl_fifo_full(): '%d'\n", cesl_fifo_empty(&my_fifo_g), cesl_fifo_full(&my_fifo_g));
    cesl_dprintf("cesl_fifo_push('fourth')   : '%d'\n", test_cesl_fifo_push("fourth"));
    cesl_dprintf("cesl_fifo_empty()          : '%d'  cesl_fifo_full(): '%d'\n", cesl_fifo_empty(&my_fifo_g), cesl_fifo_full(&my_fifo_g));

    cesl_dprintf("cesl_fifo_front()          : '%s'\n", cesl_fifo_front(&my_fifo_g));
    cesl_fifo_pop(&my_fifo_g);
    cesl_dprintf("cesl_fifo_empty()          : '%d'  cesl_fifo_full(): '%d'\n", cesl_fifo_empty(&my_fifo_g), cesl_fifo_full(&my_fifo_g));
    cesl_dprintf("cesl_fifo_front()          : '%s'\n", cesl_fifo_front(&my_fifo_g));
    cesl_dprintf("cesl_fifo_push('fourth')   : '%d'\n", test_cesl_fifo_push("fourth"));
    cesl_dprintf("cesl_fifo_empty()          : '%d'  cesl_fifo_full(): '%d'\n", cesl_fifo_empty(&my_fifo_g), cesl_fifo_full(&my_fifo_g));

    cesl_fifo_pop(&my_fifo_g);
    cesl_dprintf("cesl_fifo_empty()          : '%d'  cesl_fifo_full(): '%d'\n", cesl_fifo_empty(&my_fifo_g), cesl_fifo_full(&my_fifo_g));
    cesl_dprintf("cesl_fifo_front()          : '%s'\n", cesl_fifo_front(&my_fifo_g));

    cesl_fifo_pop(&my_fifo_g);
    cesl_dprintf("cesl_fifo_empty()          : '%d'  cesl_fifo_full(): '%d'\n", cesl_fifo_empty(&my_fifo_g), cesl_fifo_full(&my_fifo_g));
    cesl_dprintf("cesl_fifo_front()          : '%s'\n", cesl_fifo_front(&my_fifo_g));

    cesl_fifo_pop(&my_fifo_g);
    cesl_dprintf("cesl_fifo_empty()          : '%d'  cesl_fifo_full(): '%d'\n", cesl_fifo_empty(&my_fifo_g), cesl_fifo_full(&my_fifo_g));
    cesl_dprintf("cesl_fifo_front()          : '%s'\n", cesl_fifo_front(&my_fifo_g));

    cesl_dprintf("cesl_fifo_push('fifth')   : '%d'\n", test_cesl_fifo_push("fifth"));
    cesl_dprintf("cesl_fifo_empty()          : '%d'  cesl_fifo_full(): '%d'\n", cesl_fifo_empty(&my_fifo_g), cesl_fifo_full(&my_fifo_g));
    cesl_dprintf("cesl_fifo_front()          : '%s'\n", cesl_fifo_front(&my_fifo_g));

    return 0;
}
