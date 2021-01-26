#include <cesl_debug/cesl_debug.h>
#include <cesl_containers/cesl_vector_s.h>
#include <string.h>

#define my_fifo_elem_max_size 1024
#define my_fifo_elems_max_count 32

static char my_vector_buffer_g[my_fifo_elems_max_count][my_fifo_elem_max_size];
static cesl_vector_s_t my_vector_g;

static int test_vector_s_push_back_size(const char* str)
{
    return cesl_vector_s_push_back_size(&my_vector_g, str, strlen(str));
}

static int test_vector_s_insert_size(size_t index, const char* str)
{
    return cesl_vector_s_insert_size(&my_vector_g, index, str, strlen(str));
}

static void test_print_vector_s(cesl_vector_s_t* self)
{
    for (size_t i = 0; i < cesl_vector_s_size(self); ++i) {
        cesl_dprintf("my_vec[%d]: '%s'\n", i, cesl_vector_s_get(self, i));
    }
}

int main()
{
    cesl_dprintf("cesl_vector_s playground\n");
    const int res = cesl_vector_s_create(&my_vector_g, my_fifo_elem_max_size, my_fifo_elems_max_count, (char*)my_vector_buffer_g);
    cesl_dprintf("cesl_vector_s_create()         : '%d'\n", res);
    cesl_dprintf("cesl_vector_s_empty()          : '%d'  cesl_vector_s_full(): '%d'\n", cesl_vector_s_empty(&my_vector_g), cesl_vector_s_full(&my_vector_g));

    cesl_dprintf("cesl_vector_s_push('first')    : '%d'\n", test_vector_s_push_back_size("first"));
    cesl_dprintf("cesl_vector_s_empty()          : '%d'  cesl_vector_s_full(): '%d', back: '%s'\n", cesl_vector_s_empty(&my_vector_g), cesl_vector_s_full(&my_vector_g), cesl_vector_s_back(&my_vector_g));
    cesl_dprintf("cesl_vector_s_push('second')   : '%d'\n", test_vector_s_push_back_size("second"));
    cesl_dprintf("cesl_vector_s_push('third')    : '%d'\n", test_vector_s_push_back_size("third"));
    cesl_dprintf("cesl_vector_s_push('fourth')   : '%d'\n", test_vector_s_push_back_size("fourth"));
    cesl_dprintf("cesl_vector_s_empty()          : '%d'  cesl_vector_s_full(): '%d'\n", cesl_vector_s_empty(&my_vector_g), cesl_vector_s_full(&my_vector_g));
    cesl_dprintf("cesl_vector_s_size()           : '%d', back: '%s'\n\n", cesl_vector_s_size(&my_vector_g), cesl_vector_s_back(&my_vector_g));

    cesl_dprintf("cesl_vector_s_front()          : '%s'\n", cesl_vector_s_front(&my_vector_g));
    cesl_vector_s_pop_back(&my_vector_g);
    cesl_dprintf("cesl_vector_s_size()           : '%d', back: '%s'\n\n", cesl_vector_s_size(&my_vector_g), cesl_vector_s_back(&my_vector_g));

    cesl_dprintf("cesl_vector_s_empty()          : '%d'  cesl_vector_s_full(): '%d'\n", cesl_vector_s_empty(&my_vector_g), cesl_vector_s_full(&my_vector_g));
    cesl_dprintf("cesl_vector_s_front()          : '%s'\n", cesl_vector_s_front(&my_vector_g));
    cesl_dprintf("cesl_vector_s_push('fourth')   : '%d'\n", test_vector_s_push_back_size("fourth"));
    cesl_dprintf("cesl_vector_s_empty()          : '%d'  cesl_vector_s_full(): '%d'\n", cesl_vector_s_empty(&my_vector_g), cesl_vector_s_full(&my_vector_g));
    cesl_dprintf("cesl_vector_s_size()           : '%d', back: '%s'\n\n", cesl_vector_s_size(&my_vector_g), cesl_vector_s_back(&my_vector_g));

    cesl_vector_s_pop_back(&my_vector_g);
    cesl_vector_s_pop_back(&my_vector_g);
    cesl_vector_s_pop_back(&my_vector_g);
    cesl_dprintf("cesl_vector_s_empty()          : '%d'  cesl_vector_s_full(): '%d'\n", cesl_vector_s_empty(&my_vector_g), cesl_vector_s_full(&my_vector_g));
    cesl_dprintf("cesl_vector_s_front()          : '%s'\n", cesl_vector_s_front(&my_vector_g));
    cesl_dprintf("cesl_vector_s_size()           : '%d', back: '%s'\n\n", cesl_vector_s_size(&my_vector_g), cesl_vector_s_back(&my_vector_g));
    cesl_vector_s_pop_back(&my_vector_g);
    cesl_dprintf("cesl_vector_s_empty()          : '%d'  cesl_vector_s_full(): '%d'\n", cesl_vector_s_empty(&my_vector_g), cesl_vector_s_full(&my_vector_g));
    cesl_dprintf("cesl_vector_s_size()           : '%d', back: '%s'\n\n", cesl_vector_s_size(&my_vector_g), cesl_vector_s_back(&my_vector_g));

    cesl_dprintf("*** cesl_vector_s: Test erase ***\n");
    cesl_dprintf("cesl_vector_s_push('first')    : '%d'\n", test_vector_s_push_back_size("first"));
    cesl_dprintf("cesl_vector_s_push('second')   : '%d'\n", test_vector_s_push_back_size("second"));
    cesl_dprintf("cesl_vector_s_push('third')    : '%d'\n", test_vector_s_push_back_size("third"));
    cesl_dprintf("cesl_vector_s_push('fourth')   : '%d'\n", test_vector_s_push_back_size("fourth"));
    cesl_dprintf("cesl_vector_s_empty()          : '%d'  cesl_vector_s_full(): '%d'\n", cesl_vector_s_empty(&my_vector_g), cesl_vector_s_full(&my_vector_g));
    cesl_dprintf("cesl_vector_s_size()           : '%d', back: '%s'\n\n", cesl_vector_s_size(&my_vector_g), cesl_vector_s_back(&my_vector_g));


    test_print_vector_s(&my_vector_g);
    cesl_dprintf("*** cesl_vector_s: Try erasing pos 1 ***\n");
    cesl_vector_s_erase_at(&my_vector_g, 1);
    test_print_vector_s(&my_vector_g);

    cesl_dprintf("*** cesl_vector_s: Try erasing pos 0 ***\n");
    cesl_vector_s_erase_at(&my_vector_g, 0);
    test_print_vector_s(&my_vector_g);

    cesl_dprintf("*** cesl_vector_s: Try inserting 'insert 0' at pos 0 ***\n");
    test_vector_s_insert_size(0, "insert 0");
    test_print_vector_s(&my_vector_g);
    cesl_dprintf("*** cesl_vector_s: Try inserting 'insert 1' at pos 1 ***\n");
    test_vector_s_insert_size(1, "insert 1");
    test_print_vector_s(&my_vector_g);
    cesl_dprintf("*** cesl_vector_s: Try inserting 'insert 2' at pos 2 ***\n");
    test_vector_s_insert_size(2, "insert 2");
    test_print_vector_s(&my_vector_g);
    cesl_dprintf("*** cesl_vector_s: Try inserting 'insert 4' at pos 4 ***\n");
    test_vector_s_insert_size(4, "insert 4");
    test_print_vector_s(&my_vector_g);

    cesl_dprintf("*** cesl_vector_s: Try inserting 'insert 6' at pos 6 ***\n");
    test_vector_s_insert_size(6, "insert 6");
    test_print_vector_s(&my_vector_g);

    cesl_dprintf("*** cesl_vector_s: Try erasing pos 3 ***\n");
    cesl_vector_s_erase_at(&my_vector_g, 3);
    test_print_vector_s(&my_vector_g);

    cesl_dprintf("*** cesl_vector_s: Try erasing pos 0 ***\n");
    cesl_vector_s_erase_at(&my_vector_g, 0);
    test_print_vector_s(&my_vector_g);
    return 0;
}
