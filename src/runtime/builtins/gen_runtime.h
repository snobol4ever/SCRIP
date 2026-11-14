#ifndef GEN_RUNTIME_H
#define GEN_RUNTIME_H
#include "ir/ir_fwd.h"
#include "ir/stage2_procentry.h"
#include "runtime/builtins/gen_runtime_scansubjregs.h"
#ifdef __cplusplus
extern "C" {
#endif
IR_graph_t *bb_graph_of_proc(const ProcEntry *e);
IR_t *bb_proc_entry(const ProcEntry *e);
ScanSubjRegs rt_icn_needle_coerce(uint64_t lo, uint64_t hi);
DESCR_t rt_match_capture(uint64_t sigma, int64_t start, int64_t end, const char *var);
#ifdef __cplusplus
}
#endif
#endif
