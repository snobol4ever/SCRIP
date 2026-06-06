#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
}
#include "x86_asm.h"
extern "C" int rt_unify_terms(void *l, void *r);
extern "C" int rt_pl_unify_cell_const(void *cell_term, int kind, long ival, const char *sval);
extern "C" int rt_pl_unify_struct_gz(void *frame, const void *lnd, const void *rnd);
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_cell_unify_str() {
    if (PLATFORM_X86) {
        x86_begin();
        int lk = _.bb_lk, rk = _.bb_rk;
        long li = (long)_.bb_li, ri = (long)_.bb_ri;
        const IR_t *ln = (const IR_t *)_.bb_ln, *rn = (const IR_t *)_.bb_rn;
        const char *ls = ln ? ln->sval : (const char *)0, *rs = rn ? rn->sval : (const char *)0;
        std::string tail = x86("test", "eax", "eax") + x86("je", PORT_OMEGA) + x86("jmp", PORT_GAMMA) + x86("def", PORT_BETA) + x86("jmp", PORT_OMEGA);
        if (lk == IR_STRUCT || rk == IR_STRUCT)
            return IF(MEDIUM_TEXT, x86("label", _.lbl_α) + x86("comment", "BOX CELL_UNIFY struct-unify  [PL-GZ-9a: rdi=r12 GZ frame, rsi/rdx=sealed IR_t* trees [rip+disp]; rt_pl_unify_struct_gz builds Term*s from frame cells then unifies]"))
                 + x86("mov", "rdi", "r12")
                 + x86_ro_load_q("rsi", 0)
                 + x86_ro_load_q("rdx", 1)
                 + x86("call", "rt_pl_unify_struct_gz", (uint64_t)(uintptr_t)(void *)rt_pl_unify_struct_gz)
                 + tail
                 + x86_ro_seal_q(0, (uint64_t)(uintptr_t)(const void *)ln)
                 + x86_ro_seal_q(1, (uint64_t)(uintptr_t)(const void *)rn);
        if (lk == IR_LOGICVAR && rk == IR_LOGICVAR && li == ri)
            return IF(MEDIUM_TEXT, x86("label", _.lbl_α) + x86("comment", "BOX CELL_UNIFY cell=cell self — vacuous success  [PL-GZ-3 frame-cell unify, x86() self-encoding]"))
                 + x86("jmp", PORT_GAMMA)
                 + x86("def", PORT_BETA)
                 + x86("jmp", PORT_OMEGA);
        if (lk == IR_LOGICVAR && rk == IR_LOGICVAR)
            return IF(MEDIUM_TEXT, x86("label", _.lbl_α) + x86("comment", "BOX CELL_UNIFY cell↔cell  [PL-GZ-3: Term* from [ζ+off], ONE rt VALUE call, binding trailed in rt]"))
                 + x86("mov", "rdi", FRQ(GZ_CELL_OFF((int)li)))
                 + x86("mov", "rsi", FRQ(GZ_CELL_OFF((int)ri)))
                 + x86("call", "rt_unify_terms", (uint64_t)(uintptr_t)(void *)rt_unify_terms)
                 + tail;
        {
            int slot = -1, ck = 0; long ci = 0; const char *cs = (const char *)0;
            if      (lk == IR_LOGICVAR && (rk == IR_ATOM || rk == IR_LIT_I)) { slot = (int)li; ck = rk; ci = ri; cs = rs; }
            else if (rk == IR_LOGICVAR && (lk == IR_ATOM || lk == IR_LIT_I)) { slot = (int)ri; ck = lk; ci = li; cs = ls; }
            if (slot >= 0)
                return IF(MEDIUM_TEXT, x86("label", _.lbl_α) + x86("comment", "BOX CELL_UNIFY cell↔const  [PL-GZ-3: const sealed RO in-box [rip+disp], binding trailed in rt]"))
                     + x86("mov", "rdi", FRQ(GZ_CELL_OFF(slot)))
                     + x86("mov", "esi", (long)ck)
                     + x86("mov", "rdx", ci)
                     + (cs ? x86_ro_load_q("rcx", 0) : x86("mov", "ecx", (long)0))
                     + x86("call", "rt_pl_unify_cell_const", (uint64_t)(uintptr_t)(void *)rt_pl_unify_cell_const)
                     + tail
                     + (cs ? x86_ro_seal_str(0, cs) : std::string());
        }
        {
            int lkc = (lk == IR_ATOM || lk == IR_LIT_I), rkc = (rk == IR_ATOM || rk == IR_LIT_I);
            if (lkc && rkc) {
                int eq = (lk == rk) && ((lk == IR_LIT_I) ? (li == ri) : (ls && rs && strcmp(ls, rs) == 0));
                if (eq)
                    return IF(MEDIUM_TEXT, x86("label", _.lbl_α) + x86("comment", "BOX CELL_UNIFY const=const — folded vacuous success at emit time  [PL-GZ-3b fact head-unify]"))
                         + x86("jmp", PORT_GAMMA)
                         + x86("def", PORT_BETA)
                         + x86("jmp", PORT_OMEGA);
                return IF(MEDIUM_TEXT, x86("label", _.lbl_α) + x86("comment", "BOX CELL_UNIFY const≠const — folded fail at emit time  [PL-GZ-3b fact head-unify]"))
                     + x86("jmp", PORT_OMEGA)
                     + x86("def", PORT_BETA)
                     + x86("jmp", PORT_OMEGA);
            }
        }
        return x86_bomb("bb_cell_unify: unadmitted operand shape reached the emitter");
    }
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_cell_unify(void) { bb_emit_x86(bb_cell_unify_str()); }
