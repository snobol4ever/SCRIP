#include <string>
#include <cstring>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
#include "IR.h"
#include "IR_interp_state.h"
}
#include "x86_asm.h"
extern "C" int  rt_trail_mark(void);
extern "C" void rt_trail_unwind(int mark);
extern "C" int  rt_pl_unify_cell_const(void *cell_term, int kind, long ival, const char *sval);
/*--------------------------------------------------------------------------------------------------------------------*/
static int cc_consts_match(const IR_t *a, const IR_t *c) {
    if (!a || !c || a->op != c->op) return 0;
    if (a->op == IR_LIT_I) return IR_LIT(a).ival == IR_LIT(c).ival;
    if (a->op == IR_ATOM)  return IR_LIT(a).sval && IR_LIT(c).sval && strcmp(IR_LIT(a).sval, IR_LIT(c).sval) == 0;
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static const pl_gz_choice_state_t *bcch_st() { const IR_t *nd = (const IR_t *)_.bb_zn; return nd ? (const pl_gz_choice_state_t *)(intptr_t)IR_LIT(nd).ival : (const pl_gz_choice_state_t *)0; }
static int bcch_N() { return bcch_st()->nclauses; }
static int bcch_A() { return bcch_st()->arity; }
static int bcch_mark_off() { return GZ_CELL_OFF(bcch_st()->mark_slot); }
static int bcch_cur_off()  { return GZ_CELL_OFF(bcch_st()->mark_slot + 1); }
static int bcch_clause_dead(int k) {
    for (int j = 0; j < bcch_A(); j++) {
        const IR_t *a = bcch_st()->args[j], *c = bcch_st()->consts[k][j];
        if (a && c && a->op != IR_LOGICVAR && !cc_consts_match(a, c)) return 1;
    }
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bcch_arg_unify(const IR_t *a, const IR_t *c, int lk, int ro_id) {
    if (a->op != IR_LOGICVAR) return std::string();
    return x86("mov", "rdi", FRQ(GZ_CELL_OFF((int)IR_LIT(a).ival)))
         + x86("mov", "esi", (long)c->op)
         + x86("mov", "rdx", (c->op == IR_LIT_I) ? (long)IR_LIT(c).ival : 0L)
         + (c->op == IR_ATOM ? x86_ro_load_q("rcx", ro_id) : x86("mov", "ecx", (long)0))
         + x86("call", "rt_pl_unify_cell_const", (uint64_t)(uintptr_t)(void *)rt_pl_unify_cell_const)
         + x86("test", "eax", "eax")
         + x86("je", L(lk));
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bcch_clause(int k, int *ro_id, std::string *seals) {
    if (bcch_clause_dead(k)) return x86("jmp", L(k));
    std::string out;
    for (int j = 0; j < bcch_A(); j++) {
        const IR_t *a = bcch_st()->args[j], *c = bcch_st()->consts[k][j];
        if (!a || !c) return x86_bomb("bb_cell_choice: missing arg/const node");
        out += bcch_arg_unify(a, c, k, *ro_id);
        if (a->op == IR_LOGICVAR && c->op == IR_ATOM) { *seals += x86_ro_seal_str(*ro_id, IR_LIT(c).sval); (*ro_id)++; }
    }
    return out + x86("jmp", "γ");
}
static std::string bcch_clause_tail(int k) {
    if (k + 1 < bcch_N())
        return x86("def", L(k))
             + x86("mov", FR(bcch_cur_off()), (long)(k + 2))
             + x86("mov", "edi", FR(bcch_mark_off()))
             + x86("call", "rt_trail_unwind", (uint64_t)(uintptr_t)(void *)rt_trail_unwind);
    return x86("def", L(k))
         + x86("mov", "edi", FR(bcch_mark_off()))
         + x86("call", "rt_trail_unwind", (uint64_t)(uintptr_t)(void *)rt_trail_unwind)
         + x86("jmp", "ω");
}
static std::string bcch_beta_dispatch() {
    return FOR(0, bcch_N() - 1, [&](int k) {
        return x86("mov", "eax", FR(bcch_cur_off())) + x86("cmp", "eax", (long)(k + 1)) + x86("je", L(k));
    }) + x86("jmp", L(bcch_N() - 1));
}
static std::string bcch_build() {
    std::string seals;
    int ro_id = bcch_N();
    std::string s = IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                     + x86("comment", "BOX CELL_CHOICE  [PL-GZ-4 seed transcription: 1-based cursor + trail-mark in the box's OWN frame row; redo unwinds to entry mark; clause k fail falls"
                       " into clause k+1]"))
        + x86("call", "rt_trail_mark", (uint64_t)(uintptr_t)(void *)rt_trail_mark)
        + x86("mov", FR(bcch_mark_off()), "eax")
        + x86("mov", FR(bcch_cur_off()), (long)1)
        + FOR(0, bcch_N(), [&](int k) { return bcch_clause(k, &ro_id, &seals) + bcch_clause_tail(k); })
        + x86("def", "β")
        + bcch_beta_dispatch();
    return s + seals;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_cell_choice_str() {
    if (!PLATFORM_X86) return std::string();
    x86_begin();
    if (!bcch_st() || bcch_N() < 2 || bcch_N() > 4 || bcch_A() < 0 || bcch_A() > 2)
        return x86_bomb("bb_cell_choice: unadmitted choice shape reached the emitter");
    return bcch_build();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_cell_choice(void) { bb_emit_x86(bb_cell_choice_str()); }
