#include <stdlib.h>
#include <stdio.h>
#include "lower.h"
/*====================================================================================================================*/
/* ICON-ONLY BUILD GLUE: the non-Icon lowerers (lower_snobol4/raku/pascal/prolog.c) are OUT of the Makefile build per  */
/* the Icon-only pivot; their .c files remain intact on disk. Surviving non-Icon call sites (runtime_eval.c code(),    */
/* lower_common.c stage2 dispatch, parser/prolog + scrip.c pl_gz) still link against these symbols. Icon never reaches */
/* any of them, so each aborts loudly if invoked — EXCEPT lower_sno_stage2, which the lower_common.c dispatcher    */
/* calls unconditionally when the SNO bit is set (the polyglot mask sets it alongside ICN) and which the REAL      */
/* lower_sno_stage2 returns 0 from for any non-SNO program — so its stub is a faithful no-op, not a decline.       */
/* Delete this TU when the non-Icon callers are excised.                                                          */
/*--------------------------------------------------------------------------------------------------------------------*/
static void noicon_abort(const char *sym) {
    fprintf(stderr, "FATAL lower_noicon_stub: %s invoked, but its lowerer is OUT of the Icon-only build. This path is non-Icon and must not be reached. Excise the caller or re-add the lowerer.\n", sym);
    abort();
}
/*--------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * lower_snobol4(const tree_t * prog) { (void)prog; noicon_abort("lower_snobol4"); return 0; }
/*--------------------------------------------------------------------------------------------------------------------*/
int lower_sno_stage2(const tree_t * prog) { (void)prog; return 0; }
/*--------------------------------------------------------------------------------------------------------------------*/
void lower_pascal_stage2(const tree_t * prog) { (void)prog; noicon_abort("lower_pascal_stage2"); }
/*--------------------------------------------------------------------------------------------------------------------*/
void lower_pl_stage2(const tree_t * prog) { (void)prog; noicon_abort("lower_pl_stage2"); }
/*--------------------------------------------------------------------------------------------------------------------*/
void lower_raku_stage2(const tree_t * prog) { (void)prog; noicon_abort("lower_raku_stage2"); }
/*--------------------------------------------------------------------------------------------------------------------*/
void pl_dyn_mark(const char * name, int arity) { (void)name; (void)arity; noicon_abort("pl_dyn_mark"); }
/*--------------------------------------------------------------------------------------------------------------------*/
int pl_dyn_is_marked(const char * name, int arity) { (void)name; (void)arity; noicon_abort("pl_dyn_is_marked"); return 0; }
