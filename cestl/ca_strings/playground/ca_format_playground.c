#include <ca_debug/ca_debug.h>
#include <ca_strings/ca_format.h>
#include <ca_concurrent/ca_llfifo.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>

#define my_format_buf_max_size 1024

static char my_format_buffer_g[my_format_buf_max_size];

int main()
{
    ca_format_t my_format_g;
    ca_dprintf("String format playground\n");
    ca_format_t* f = ca_format_create(&my_format_g, my_format_buf_max_size, my_format_buffer_g);
    ca_dprintf("--- Simple style ---\n");
    ca_format_s(&my_format_g, "1");
    ca_format_s(&my_format_g, "2");
    ca_format_s(&my_format_g, " Some int: ");
    ca_format_i(&my_format_g, 34);
    ca_format_s(&my_format_g, " After int");
    ca_dprintf("Final: '%s'\n", my_format_g.buf);


    // Stream style
    ca_dprintf("\n--- Stream style ---\n");

    f->rst(f)->s(f, "1")->s(f, "2")->s(f, " Some int: ")->i(f, 34)
            ->s(f, " After int");
    ca_dprintf("Final: '%s'\n", f->buf);

    //
    ca_dprintf("\n--- Logging examples ---\n");
    ca_dprintf("Max buffer size of %d reached\n", 2048);
    dbg(f)->s(f, "Max buffer size of ")->i(f, 2048)->s(f, " reached\n")->print(f);
//    cout<< "Max buffer size of " << (f, 2048)->s(f, " reached\n")->dprint(f);

    ca_dprintf("Error %d on line %d character %d in file %s\n", 12, 55, 30, "myfile.c");
    dbg(f)->s(f, "Error ")->i(f, 12)->s(f, " on line ")->i(f, 55)->s(f, " character ")
            ->i(f, 30)->s(f, " in file ")->s(f, "myfile.c\n")->print(f);


    ca_dprintf("Negative number %d\n", -45);
    dbg(f)->s(f, "Negative number ")->i(f, -45)->s(f, "\n")->print(f);

//    f->rst(f)->s(f, "Error ")->i(f, 12)->s(f, " on line ")->i(f, 55)->s(f, " character ");
//    f->i(f, 30)->s(f, " in file ")->s(f, "myfile.c\n")->dprint(f)->log(f, LOG_LVL_2);
    dbg(f)->s(f, "Numbers concatenate: ")->i(f, 33)->i(f, 44)->i(f, 55)->i(f, -99)->print(f);


    return 0;
}
