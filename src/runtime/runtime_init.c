#include "rt/rt.h"
#include "dtp.h"
#include "core.h"
#include "builtins/resolution.h"
#include "bb_pool.h"
#include "../parser/prolog/prolog_atom.h"
#include <stdio.h>
#include <stdlib.h>
#include <gc/gc.h>
/*====================================================================================================================*/
void rt_gc_init(void)
{
    GC_INIT();
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_main_init(void)
{
    extern Trail g_resolve_trail;
    setvbuf(stdout, NULL, _IOLBF, 0);
    bb_pool_init();
    pat_pool_init();
    trail_init(&g_resolve_trail);
    prolog_atom_init();
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_set_lang(int lang)
{
    extern int g_lang;
    g_lang = lang;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int     g_halt_rc  = 0;
static int     g_halt_set = 0;
int rt_finalize(void)
{
    return g_halt_set ? g_halt_rc : 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_bomb(const char *msg)
{
    fprintf(stderr, "libscrip_rt: BOMB — %s\n", msg ? msg : "(no message)");
    abort();
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_unhandled_op(int op)
{
    fprintf(stderr,
        "libscrip_rt: unhandled SM opcode %d reached in emitted code.\n"
        "  (scrip --dump-sm to identify; subsequent EM-N rungs shrink the set)\n",
        op);
    abort();
}
