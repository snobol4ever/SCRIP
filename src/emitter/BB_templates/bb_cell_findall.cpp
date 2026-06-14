#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
#include "IR.h"
}
#include "x86_asm.h"
extern std::string gzu_build(const IR_t *nd);
extern "C" void * rt_enter(void **slot, int nslots);
extern "C" void * rt_pl_findall_begin(void);
extern "C" void rt_pl_findall_collect(void *acc, void *tmpl_term);
extern "C" int rt_pl_findall_finish(void *acc, void *result_term);
extern "C" int rt_pl_agg_count_finish(void *acc, void *result_term);
extern "C" int rt_pl_agg_sum_finish(void *acc, void *result_term);
extern "C" int rt_pl_agg_max_finish(void *acc, void *result_term);
extern "C" int rt_pl_agg_min_finish(void *acc, void *result_term);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *bcfa_areg(int i) { static const char * t[3] = { "rsi", "rdx", "rcx" }; return t[i]; }
static int bcfa_child() { return (int)_.op_parts_ival[0]; }
static int bcfa_nargs() { return (int)_.op_parts_ival[1]; }
static int bcfa_callee_nslots() { return (int)_.op_parts_ival[2]; }
static int bcfa_result() { return (int)_.op_parts_ival[6]; }
static int bcfa_acc() { return (int)_.op_parts_ival[7]; }
static int bcfa_is_fail() { return (int)_.op_parts_ival[8]; }
static int bcfa_agg() { return (int)_.op_parts_ival[10]; }
static std::string bcfa_finish_call() {
    switch (bcfa_agg()) {
    case 1: return x86("call", "rt_pl_agg_count_finish", (uint64_t)(uintptr_t)(void *)rt_pl_agg_count_finish);
    case 2: return x86("call", "rt_pl_agg_sum_finish",   (uint64_t)(uintptr_t)(void *)rt_pl_agg_sum_finish);
    case 3: return x86("call", "rt_pl_agg_max_finish",   (uint64_t)(uintptr_t)(void *)rt_pl_agg_max_finish);
    case 4: return x86("call", "rt_pl_agg_min_finish",   (uint64_t)(uintptr_t)(void *)rt_pl_agg_min_finish);
    default: return x86("call", "rt_pl_findall_finish",  (uint64_t)(uintptr_t)(void *)rt_pl_findall_finish);
    }
}
static std::string bcfa_finish() {
    return x86("mov", "rdi", FRQ(GZ_CELL_OFF(bcfa_acc())))
         + x86("mov", "rsi", FRQ(GZ_CELL_OFF(bcfa_result())))
         + bcfa_finish_call()
         + x86("test", "eax", "eax")
         + x86("je", "ω")
         + x86("jmp", "γ")
         + x86("def", "β")
         + x86("jmp", "ω");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_cell_findall() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    if (bcfa_acc() < 0 || bcfa_result() < 0) return x86_bomb("bb_cell_findall: unadmitted findall shape reached the emitter");
    const IR_t *tmpl = (const IR_t *)(intptr_t)_.op_parts_ival[9];
    std::string head = x86("label", _.lbl_α)
                     + x86("comment", "IR_CELL_FINDALL")
                     + x86("call", "rt_pl_findall_begin", (uint64_t)(uintptr_t)(void *)rt_pl_findall_begin)
                     + x86("mov", FRQ(GZ_CELL_OFF(bcfa_acc())), "rax");
    if (bcfa_is_fail())
        return head + bcfa_finish();
    int na = bcfa_nargs() < 3 ? bcfa_nargs() : 3;
    return head
         + x86("lea", "rdi", FR(GZ_CELL_OFF(bcfa_child())))
         + x86("mov32", "esi", (long)bcfa_callee_nslots())
         + x86("call", "rt_enter", (uint64_t)(uintptr_t)(void *)rt_enter)
         + x86("mov", "rdi", "rax")
         + FOR(0, na, [&](int i) { return x86("mov", bcfa_areg(i), FRQ(GZ_CELL_OFF((int)_.op_parts_ival[3 + i]))); })
         + x86_call_tgt(X86T_TGT0)
         + x86("def", L(0))
         + x86("test", "eax", "eax")
         + x86("je", L(1))
         + gzu_build(tmpl)
         + x86("mov", "rsi", "rax")
         + x86("mov", "rdi", FRQ(GZ_CELL_OFF(bcfa_acc())))
         + x86("call", "rt_pl_findall_collect", (uint64_t)(uintptr_t)(void *)rt_pl_findall_collect)
         + x86("mov", "rdi", FRQ(GZ_CELL_OFF(bcfa_child())))
         + x86_call_tgt(X86T_TGT1)
         + x86("jmp", L(0))
         + x86("def", L(1))
         + bcfa_finish();
}
