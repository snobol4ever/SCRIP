#ifndef GEN_RUNTIME_ETC_H
#define GEN_RUNTIME_ETC_H
#include "ir/ast.h"
#include "runtime/builtins/gen_expr_t_defined.h"
#include "ir/stage2_procentry.h"
#include "runtime/builtins/gen_runtime_scansubjregs.h"
#ifdef __cplusplus
extern "C" {
#endif
ScanSubjRegs rt_match_enter(uint64_t lo, uint64_t hi);
uint64_t rt_match_ctx_restore(uint64_t sig, uint64_t len, uint64_t cell);
DESCR_t rt_substr(const char *sigma, int64_t a, int64_t b);
int frame_lookup(tree_t *n, long *out);
int frame_lookup_sv(tree_t *n, long *out, const char **sv);
int is_suspendable(tree_t *e);
int descr_identical(DESCR_t a, DESCR_t b);
const char *cset_complement(const char *cs);
#ifdef __cplusplus
}
#endif
#endif
