#include <stdio.h>
#include <stdlib.h>
#include "core.h"
typedef struct GeneratorState GeneratorState;
GeneratorState *generator_state_new_proc(int pi, DESCR_t *args, int nargs)
{
    (void)pi; (void)args; (void)nargs;
    fprintf(stderr, "[SMX] generator_state_new_proc: SM generator pumping deleted — "
                    "this language has not crossed onto Byrd Boxes. Aborting.\n");
    abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int bb_broker_drive_sm_one(GeneratorState *gs, DESCR_t *out)
{
    (void)gs; (void)out;
    fprintf(stderr, "[SMX] bb_broker_drive_sm_one: SM generator pumping deleted — "
                    "this language has not crossed onto Byrd Boxes. Aborting.\n");
    abort();
}
