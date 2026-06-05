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
    if (!a || !c || a->t != c->t) return 0;
    if (a->t == IR_LIT_I) return a->ival == c->ival;
    if (a->t == IR_ATOM)  return a->sval && c->sval && strcmp(a->sval, c->sval) == 0;
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_cell_choice_str() {
    if (!PLATFORM_X86) return std::string();
    x86_begin();
    const IR_t *nd = (const IR_t *)_.bb_zn;
    const pl_gz_choice_state_t *st = nd ? (const pl_gz_choice_state_t *)(intptr_t)nd->ival : (const pl_gz_choice_state_t *)0;
    if (!st || st->nclauses < 2 || st->nclauses > 4 || st->arity < 0 || st->arity > 2)
        return x86_bomb("bb_cell_choice: unadmitted choice shape reached the emitter");
    int N = st->nclauses, A = st->arity;
    int mark_off = GZ_CELL_OFF(st->mark_slot), cur_off = GZ_CELL_OFF(st->mark_slot + 1);
    int ro_id = N;
    std::string seals;
    std::string s = IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                     + x86("comment", "BOX CELL_CHOICE  [PL-GZ-4 seed transcription: 1-based cursor + trail-mark in the box's OWN frame row; redo unwinds to entry mark; clause k fail falls"
                       " into clause k+1]"))
        + x86("call", "rt_trail_mark", (uint64_t)(uintptr_t)(void *)rt_trail_mark)
        + x86("mov", FR(mark_off), "eax")
        + x86("mov", FR(cur_off), (long)1);
    for (int k = 0; k < N; k++) {
        int dead = 0;
        for (int j = 0; j < A; j++) {
            const IR_t *a = st->args[j], *c = st->consts[k][j];
            if (!a || !c) return x86_bomb("bb_cell_choice: missing arg/const node");
            if (a->t != IR_LOGICVAR && !cc_consts_match(a, c)) dead = 1;
        }
        if (dead) {
            s += x86("jmp", L(k));
        } else {
            for (int j = 0; j < A; j++) {
                const IR_t *a = st->args[j], *c = st->consts[k][j];
                if (a->t != IR_LOGICVAR) continue;
                long ck = (long)c->t, ci = (c->t == IR_LIT_I) ? (long)c->ival : 0;
                s += x86("mov", "rdi", FRQ(GZ_CELL_OFF((int)a->ival)))
                   + x86("mov", "esi", ck)
                   + x86("mov", "rdx", ci)
                   + (c->t == IR_ATOM ? x86_ro_load_q("rcx", ro_id) : x86("mov", "ecx", (long)0))
                   + x86("call", "rt_pl_unify_cell_const", (uint64_t)(uintptr_t)(void *)rt_pl_unify_cell_const)
                   + x86("test", "eax", "eax")
                   + x86("je", L(k));
                if (c->t == IR_ATOM) { seals += x86_ro_seal_str(ro_id, c->sval); ro_id++; }
            }
            s += x86("jmp", PORT_GAMMA);
        }
        s += x86("def", L(k));
        if (k + 1 < N) s += x86("mov", FR(cur_off), (long)(k + 2))
                          + x86("mov", "edi", FR(mark_off))
                          + x86("call", "rt_trail_unwind", (uint64_t)(uintptr_t)(void *)rt_trail_unwind);
        else           s += x86("mov", "edi", FR(mark_off))
                          + x86("call", "rt_trail_unwind", (uint64_t)(uintptr_t)(void *)rt_trail_unwind)
                          + x86("jmp", PORT_OMEGA);
    }
    s += x86("def", PORT_BETA);
    for (int k = 0; k + 1 < N; k++)
        s += x86("mov", "eax", FR(cur_off)) + x86("cmp", "eax", (long)(k + 1)) + x86("je", L(k));
    s += x86("jmp", L(N - 1));
    return s + seals;
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_cell_choice(void) { bb_emit_x86(bb_cell_choice_str()); }
