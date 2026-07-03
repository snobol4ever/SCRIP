#include <stdlib.h>
#include <stdio.h>
#include "lower.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rt_noicon_abort(const char *sym) {
    fprintf(stderr, "FATAL rt_noicon_stub: %s invoked in the Icon-only runtime lib; its TU is excluded from libscrip_rt.so. This path is non-Icon and must not be reached.\n", sym);
    abort();
}
struct PlProgram; struct CODE_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
struct CODE_t * prolog_lower(struct PlProgram * pl_prog) { (void)pl_prog; rt_noicon_abort("prolog_lower"); return 0; }
void rebus_compile(const char * src, const char * filename, tree_t ** out_ast) { (void)src; (void)filename; (void)out_ast; rt_noicon_abort("rebus_compile"); }
void bb_gather_prepare(IR_t * nd) { (void)nd; rt_noicon_abort("bb_gather_prepare"); }
void bb_mapgrep_prepare(IR_t * nd) { (void)nd; rt_noicon_abort("bb_mapgrep_prepare"); }
