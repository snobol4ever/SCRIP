/* bb_arith.cpp — BB template for IR_ARITH: Prolog arithmetic (Y is X+2). x86() self-encoding (template-revamp PL-RV-2,
   2026-06-02, Opus 4.8). NOTE: this executed-box path is PROVABLY DEAD in all three modes — `is/2` arithmetic is emitted by
   bb_builtin's `is` arm (PLG-9a/9h), and an IR_ARITH node only ever appears as a TERM operand built by emit_build_compound_term,
   never dispatched as an executed leaf (verified: zero TEXT/BINARY fires across the full rung suite + direct `X is 2+3` in
   interp/run/compile). The box is kept correct against rt_pl_arith for any future direct-IR_ARITH dispatch. pBB-free: the op
   string comes from _.op_sval (a direct nd->sval promotion at the emit_core dispatch point); the operand kind/value scalars and
   the operand-string presence come from g_emit slots promoted driver-side in bb_prepare_pl (the established Prolog neighbor-
   resolution pattern — _.bb_lk/_.bb_li/_.bb_rk/_.bb_ri/_.bb_ls/_.bb_rs, with bb_lk==-1 the missing-operand sentinel). Reads
   ONLY _. rt_pl_arith(lk,li,ls, rk,ri,rs, op): SysV edi=lk rsi=li rdx=ls ecx=rk r8=ri r9=rs, 7th arg (op) pushed; box-entry rsp
   is 8-misaligned, one 8-byte push realigns to 16, add rsp,8 restores. */
#include <string>
#include <cstring>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
extern long rt_pl_arith(int lk, long li, const char *ls, int rk, long ri, const char *rs, const char *op);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline const char * op_str()  { return _.op_sval ? _.op_sval : "+"; }
static inline int          vacuous() { return _.bb_lk < 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_arith_str() {
    if (PLATFORM_X86) {
        if (vacuous())
            return IF(MEDIUM_TEXT, s_1asm(std::string(_.lbl_α) + ":")
                                 + s_comment(emit_fmt("# BOX RESOLVE_ARITH(op='%s')  vacuous [x86() self-encoding]", op_str())))
                 + x86("jmp", PORT_GAMMA)
                 + x86("def", PORT_BETA)
                 + x86("jmp", PORT_OMEGA);
        return IF(MEDIUM_TEXT, s_1asm(std::string(_.lbl_α) + ":")
                             + s_comment(emit_fmt("# BOX RESOLVE_ARITH(op='%s')  [x86() self-encoding]", op_str())))
             + x86("mov", "edi", (long)_.bb_lk)
             + x86("mov", "esi", (long)_.bb_li)
             + (_.bb_ls ? x86("lea", "rdx", "[rip + __]", (uint64_t)(uintptr_t)_.bb_ls, _.bb_ls) : x86("mov", "edx", (long)0))
             + x86("mov", "ecx", (long)_.bb_rk)
             + x86("mov", "r8",  (long)_.bb_ri)
             + (_.bb_rs ? x86("lea", "r9", "[rip + __]", (uint64_t)(uintptr_t)_.bb_rs, _.bb_rs) : x86("mov", "r9d", (long)0))
             + x86("lea", "rax", "[rip + __]", (uint64_t)(uintptr_t)op_str(), _.bb_op_lbl)
             + x86("push", "rax")
             + x86("call", "rt_pl_arith", (uint64_t)(uintptr_t)(void*)rt_pl_arith)
             + x86("add", "rsp", (long)8)
             + x86("jmp", PORT_GAMMA)
             + x86("def", PORT_BETA)
             + x86("jmp", PORT_OMEGA);
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_arith(void) { bb_emit_x86(bb_arith_str()); }
