/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "rt/rt.h"
#include "dtp.h"
#include "core.h"
#include "builtins/resolution.h"
#include "bb_pool.h"
#include "../parser/prolog/prolog_atom.h"
#include <stdio.h>
#include <stdlib.h>
#include <gc/gc.h>
static int     g_halt_rc  = 0;
static int     g_halt_set = 0;
void rt_bomb(const char *msg)
{
    fprintf(stderr, "libscrip_rt: BOMB — %s\n", msg ? msg : "(no message)");
    abort();
}
