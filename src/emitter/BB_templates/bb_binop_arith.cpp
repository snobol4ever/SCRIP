/* bb_binop_arith.cpp — BB box: IR_BINOP integer arithmetic, stackless slot→slot DESCR.
   GZ-9 (Icon). x86() self-encoding (template-revamp, 2026-06-02). pBB-FREE: reads ONLY g_emit
   (FACT RULE — no neighbor reads, no fusion). The driver (emit_bb.c case IR_BINOP) resolves the
   operand slots and result slot and deposits them as scalars: op_ival (the op), op_sa / op_sb
   (operand DESCR frame offsets), op_off (this box's result DESCR slot). op_off>=0 is the driver's
   verdict "this IS the arith case" — the ADD/SUB/MUL/DIV/MOD decision lives ONLY in the driver, so
   it is not duplicated here. Both operands are producer boxes (VAR / nested binop) that already
   wrote a 16-byte DESCR into their own ζ=r12 frame slot; this box reads each operand's int payload
   at [r12+slot+8], computes register-to-register (rax=lhs, rcx=rhs, result→rax), and writes a DESCR
   {v:DT_I, payload:result} into [r12+off] so a consumer reads it by its slot. test_icon.c named-slot
   model — NO value stack, NO ring. `i := i + 1` of a while/until loop lowers to exactly this. The x86
   arm is ONE return, a pure x86() concat: the only per-medium difference (BINARY bytes vs GAS text)
   lives inside the x86_* encoders. Grounded in Icon oarith.r integer arithmetic. */
#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "SM.h"
#include "ast.h"
#include "descr.h"
#include "../../runtime/interp/gen.h"
extern int g_icn_flat_chain;
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_binop_arith_str() {
    if (!PLATFORM_X86) return std::string();
    if (!(g_icn_flat_chain && _.op_off >= 0)) return std::string();
    int64_t op  = _.op_ival;
    int     sa  = _.op_sa, sb = _.op_sb, off = _.op_off;
    std::string opb;
    switch (op) {
    case BINOP_ADD: opb = x86("add",  "rax", "rcx"); break;
    case BINOP_SUB: opb = x86("sub",  "rax", "rcx"); break;
    case BINOP_MUL: opb = x86("imul", "rax", "rcx"); break;
    case BINOP_DIV: opb = x86("cqo") + x86("idiv", "rcx"); break;
    case BINOP_MOD: opb = x86("cqo") + x86("idiv", "rcx") + x86("mov", "rax", "rdx"); break;
    default:        opb = x86("add",  "rax", "rcx"); break;
    }
    return IF(MEDIUM_TEXT, s_1asm(std::string(_.lbl_α) + ":")
                          + s_comment(emit_fmt("# BOX IR_BINOP arith op=%lld [GZ-9 x86() self-encoding, stackless slot->slot DESCR]", (long long)op)))
         + x86("mov", "rax", FRQ(sa + 8))
         + x86("mov", "rcx", FRQ(sb + 8))
         + opb
         + x86("mov", FRQ(off),     (long)DT_I)
         + x86("mov", FRQ(off + 8), "rax")
         + x86("jmp", PORT_GAMMA)
         + x86("def", PORT_BETA)
         + x86("jmp", PORT_OMEGA);
}
