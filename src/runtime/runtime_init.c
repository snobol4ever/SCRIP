#include "rt/rt.h"
#include "dtp.h"
#include "core.h"
#include "builtins/resolution.h"
#include "bb_pool.h"
#include "../parser/prolog/prolog_atom.h"
#include <stdio.h>
#include <stdlib.h>
static int     g_halt_rc  = 0;
static int     g_halt_set = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_bomb(const char *msg)
{
    fflush(NULL);   /* s58 DESCENT-MAP: the bomb is a measurement instrument — everything the program printed before dying must reach the pipe, or the sweep's output-prefix classification reads an empty buffer as empty output */
    fprintf(stderr, "libscrip_rt: BOMB — %s\n", msg ? msg : "(no message)");
    abort();
}
