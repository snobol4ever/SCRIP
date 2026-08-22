#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
int descr_identical(DESCR_t a, DESCR_t b);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* IR-IDENT/DIFFER slice 3 (Lon directive s199).  ONE family, ONE mechanism -- bb_differ.cpp is this file with the final jcc polarity
   inverted, nothing else differs.  Both replace the by-name dispatch chain (bid_of hash + DTAX cache + switch: measured s199 at 121.55
   units/call, 1-arg, vs the body's own ~4.6 units) with a direct call to the SAME descr_identical() the slow path (bn_identdiffer) already
   calls -- zero semantic drift, this is the call-overhead fix the FINDING asked for, not a reimplementation of the comparison.  This box
   never sees a redefined IDENT/DIFFER: the lowerer (sx_ident_differ, lower_snobol4.c) refuses the fast path whenever the compiling
   program's own source DEFINEs the name (sno_predef_registered), so a redefining program never reaches here at all -- see that site for
   the full redefinition-guard note.  1-arg IDENT(x) arrives as 2 operands -- LOWER synthesizes a literal "" for the missing 2nd arg,
   matching bn_identdiffer's own nargs==1 -> compare-against-NULVCL rule -- so this box never branches on arg count: the 1/2-arg split is
   an operand count, never an admission test.  TWO EMISSION SHAPES, same semantics: op_zres selects the "ZD" operand-array convention
   (ZOPQ/ZRES) some graphs use instead of ordinary ζ-spine slots (op_sa/op_sb/FRQ) -- both must be implemented, no other IR_CMP_TEST-family
   opcode gets to skip either one. */
std::string bb_ident() {
    x86_begin();
    if (PLATFORM_X86) {
        if (_.op_zres)
            return x86("comment", "IR_IDENT zd -> descr_identical (direct call, no by-name dispatch)")
                 + x86_alpha()
                 + x86("note", ZOPN(0)) + x86("mov", "rdi", ZOPQ(0, 0))
                 + x86("note", ZOPN(0)) + x86("mov", "rsi", ZOPQ(0, 8))
                 + x86("note", ZOPN(1)) + x86("mov", "rdx", ZOPQ(1, 0))
                 + x86("note", ZOPN(1)) + x86("mov", "rcx", ZOPQ(1, 8))
                 + x86("call", "descr_identical", (uint64_t)(uintptr_t)(void *)descr_identical)
                 + x86("test", "eax", "eax")
                 + x86_omega("je")
                 + IF(_.op_res_live, x86("note", ZRESN()) + x86("mov", ZRES(0), (long)0)
                                   + x86("note", ZRESN()) + x86("mov", ZRES(8), (long)0))
                 + x86_gamma()
                 + x86_beta_trampoline();
        return (_.op_sa >= 0 && _.op_sb >= 0 && _.op_off >= 0)
                 ? x86("comment", "IR_IDENT -> descr_identical (direct call, no by-name dispatch)")
                 + x86_alpha()
                 + x86("mov", "rdi", FRQ(_.op_sa))
                 + x86("mov", "rsi", FRQ(_.op_sa + 8))
                 + x86("mov", "rdx", FRQ(_.op_sb))
                 + x86("mov", "rcx", FRQ(_.op_sb + 8))
                 + x86("call", "descr_identical", (uint64_t)(uintptr_t)(void *)descr_identical)
                 + x86("test", "eax", "eax")
                 + x86_omega("je")
                 + IF(_.op_res_live, x86("mov", FRQ(_.op_off),     (long)0)
                                   + x86("mov", FRQ(_.op_off + 8), (long)0))
                 + x86_gamma()
                 + x86_beta_trampoline()
             : x86_bomb("bb_ident: needs two operand slots (op_sa/op_sb) + own value slot (op_off)");
    }
    return std::string();
}
