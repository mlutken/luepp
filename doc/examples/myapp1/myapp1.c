#include <foolib/foos_a.h>

//#include <stdio.h>

#if 1
int main (int argc, char *argv[])
{
//    printf("Hello from myapp1\n");
    int i = squareme(argc);
//    printf("Call function foo1() in libfoo: %d\n", foo1());
    return i;
}

#endif

#if 0
/*-------------------------- FILE PROLOGUE ---------------------------*/


/* ------------------------- FILE INCLUSION -------------------------- */
/* PSSW API. */
#include <vm.h>
#ifdef PIKEOSDEBUG
/* init_gdbstub(), gdb_breakpoint(). */
#include <vm_debug.h>
#endif

/* ----------------------- OBJECT DECLARATIONS ----------------------- */

/* -------------------- FUNCTION DECLARATIONS ------------------------ */

extern int main(void);

/* ----------------------- LOCAL FUNCTIONS --------------------------- */

/* --------------- GLOBAL FUNCTION DEFINITIONS ----------------------- */

/**
 * @purpose "Main" function of the server
 */
extern int main(void)
{
    P4_uid_t me;
    P4_timeout_t sleeptime;

    int i = foo1();

    /* get my own PikeOS unique Id */
    me = p4_my_uid();

#ifdef PIKEOSDEBUG
    gdb_breakpoint();
#endif

    /* Startup message. */
    vm_cputs("Hello World, starting up.\n");
    vm_cprintf("Call function foo1() in libfoo: %d\n", i);

    /* Do the real work */
    sleeptime = P4_SEC(5);
    for (;;) {
        vm_cprintf("Hello World, this is task %d, thread %d\n",
            P4_UID_GET_TASK(me), P4_UID_GET_THREAD(me));
        p4_sleep(sleeptime);
    }

    return i;
}

#endif
