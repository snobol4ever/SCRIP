#include "../core/core.h"
#include "../rt/rt_arena.h"
__attribute__((visibility("hidden"))) unsigned char rtx_gate_str = 0;
DESCR_t c_str_concat_d(DESCR_t a, DESCR_t b);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t str_concat_d(DESCR_t a, DESCR_t b) { return c_str_concat_d(a, b); }
char *c_VARVAL_fn(DESCR_t v);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
char *VARVAL_fn(DESCR_t v) { return c_VARVAL_fn(v); }
