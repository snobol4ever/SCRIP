#ifndef GEN_RUNTIME_H
#define GEN_RUNTIME_H
#include "ir/ast.h"
#include "parsers/snobol4/scrip_cc.h"
#include "ir/bb_box.h"
#include "gen.h"
#include "ir/ir_fwd.h"
#include "ir/SM.h"
#include "ir/stage2.h"
struct GeneratorState;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t *bb_graph_of_proc(const ProcEntry *e);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *bb_proc_entry(const ProcEntry *e);
typedef struct { uint64_t ptr; uint64_t len; } ScanSubjRegs;
ScanSubjRegs rt_icn_needle_coerce(uint64_t lo, uint64_t hi);
ScanSubjRegs rt_match_enter(uint64_t lo, uint64_t hi);
uint64_t rt_match_ctx_restore(uint64_t sig, uint64_t len, uint64_t cell);
DESCR_t rt_match_capture(uint64_t sigma, int64_t start, int64_t end, const char *var);
DESCR_t rt_substr(const char *sigma, int64_t a, int64_t b);
int frame_lookup(tree_t *n, long *out);
int frame_lookup_sv(tree_t *n, long *out, const char **sv);
int is_global(const char *name);
void global_register(const char *name);
int scope_add(Scope *sc, const char *name);
int scope_get(Scope *sc, const char *name);
DESCR_t proc_table_call(int pi, DESCR_t *args, int nargs);
int is_suspendable(tree_t *e);
const char *real_str(double r, char *buf, int bufsz);
int descr_identical(DESCR_t a, DESCR_t b);
int c_descr_identical(DESCR_t a, DESCR_t b);
const char *cset_complement(const char *cs);
const char *cset_union(const char *a, int alen, int ablk, const char *b, int blen, int bblk, int *outlen);
const char *cset_diff(const char *a, int alen, int ablk, const char *b, int blen, int bblk, int *outlen);
const char *cset_inter(const char *a, int alen, int ablk, const char *b, int blen, int bblk, int *outlen);
const char *cset_canonical(const char *cs, int len);
#include "runtime/keywords.h"
#endif
