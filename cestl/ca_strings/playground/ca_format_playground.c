#include <cesl_debug/cesl_debug.h>
#include <cesl_strings/cesl_format.h>
#include <cesl_concurrent/cesl_llfifo.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>

#define my_format_buf_max_size 1024

static char my_format_buffer_g[my_format_buf_max_size];

int main()
{
    cesl_format_t my_format_g;
    cesl_dprintf("String format playground\n");
    cesl_format_t* f = cesl_format_create(&my_format_g, my_format_buf_max_size, my_format_buffer_g);
    cesl_dprintf("--- Simple style ---\n");
    cesl_format_s(&my_format_g, "1");
    cesl_format_s(&my_format_g, "2");
    cesl_format_s(&my_format_g, " Some int: ");
    cesl_format_i(&my_format_g, 34);
    cesl_format_s(&my_format_g, " After int");
    cesl_dprintf("Final: '%s'\n", my_format_g.buf);


    // Stream style
    cesl_dprintf("\n--- Stream style ---\n");

    f->rst(f)->s(f, "1")->s(f, "2")->s(f, " Some int: ")->i(f, 34)
            ->s(f, " After int");
    cesl_dprintf("Final: '%s'\n", f->buf);

    //
    cesl_dprintf("\n--- Logging examples ---\n");
    cesl_dprintf("Max buffer size of %d reached\n", 2048);
    dbg(f)->s(f, "Max buffer size of ")->i(f, 2048)->s(f, " reached\n")->print(f);
//    cout<< "Max buffer size of " << (f, 2048)->s(f, " reached\n")->dprint(f);

    cesl_dprintf("Error %d on line %d character %d in file %s\n", 12, 55, 30, "myfile.c");
    dbg(f)->s(f, "Error ")->i(f, 12)->s(f, " on line ")->i(f, 55)->s(f, " character ")
            ->i(f, 30)->s(f, " in file ")->s(f, "myfile.c\n")->print(f);


    cesl_dprintf("Negative number %d\n", -45);
    dbg(f)->s(f, "Negative number ")->i(f, -45)->s(f, "\n")->print(f);

//    f->rst(f)->s(f, "Error ")->i(f, 12)->s(f, " on line ")->i(f, 55)->s(f, " character ");
//    f->i(f, 30)->s(f, " in file ")->s(f, "myfile.c\n")->dprint(f)->log(f, LOG_LVL_2);
    dbg(f)->s(f, "Numbers concatenate: ")->i(f, 33)->i(f, 44)->i(f, 55)->i(f, -99)->print(f);


    return 0;
}
