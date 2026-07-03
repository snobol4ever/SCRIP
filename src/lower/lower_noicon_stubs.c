#include <stdlib.h>
#include <stdio.h>
#include "lower.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void noicon_abort(const char *sym) {
    fprintf(stderr, "FATAL lower_noicon_stub: %s invoked, but its lowerer is OUT of the Icon-only build. This path is non-Icon "
                    "and must not be reached. Excise the caller or re-add the lowerer.\n", sym);
    abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void lower_pascal_stage2(const tree_t * prog) { (void)prog; noicon_abort("lower_pascal_stage2"); }
void lower_pl_stage2(const tree_t * prog) { (void)prog; noicon_abort("lower_pl_stage2"); }
void lower_raku_stage2(const tree_t * prog) { (void)prog; noicon_abort("lower_raku_stage2"); }
void pl_dyn_mark(const char * name, int arity) { (void)name; (void)arity; noicon_abort("pl_dyn_mark"); }
int pl_dyn_is_marked(const char * name, int arity) { (void)name; (void)arity; noicon_abort("pl_dyn_is_marked"); return 0; }
