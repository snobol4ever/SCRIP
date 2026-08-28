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
/* IR-IDENT/DIFFER slice 3 (Lon directive s199).  ONE family, ONE mechanism -- this file is bb_ident.cpp with the final jcc polarity
   inverted (DIFFER succeeds when NOT identical), nothing else differs; see bb_ident.cpp for the full design note, the measured call-
   overhead basis, and the redefinition-guard finding (both members share it via lower_snobol4.c's sx_ident_differ, per NO-PER-OP-FILTER). */
std::string bb_differ() {
    x86_begin();
    if (_.op_zres)
        return x86("comment", "IR_DIFFER zd -> descr_identical (direct call, no by-name dispatch)")
             + x86_alpha()
             + x86("note", ZOPN(0))
             + x86("mov", "rdi", ZOPQ(0, 0))
             + x86("note", ZOPN(0))
             + x86("mov", "rsi", ZOPQ(0, 8))
             + x86("note", ZOPN(1))
             + x86("mov", "rdx", ZOPQ(1, 0))
             + x86("note", ZOPN(1))
             + x86("mov", "rcx", ZOPQ(1, 8))
             + x86("call", "descr_identical", (uint64_t)(uintptr_t)(void *)descr_identical)
             + x86("test", "eax", "eax")
             + x86_omega("jne")
             + IF(_.op_res_live, x86("note", ZRESN())
                               + x86("mov", ZRES(0), (long)0)
                               + x86("note", ZRESN())
                               + x86("mov", ZRES(8), (long)0))
             + x86_gamma()
             + x86_beta_trampoline();
    return (_.op_sa >= 0 && _.op_sb >= 0 && _.op_off >= 0)
             ? x86("comment", "IR_DIFFER -> descr_identical (direct call, no by-name dispatch)")
             + x86_alpha()
             + x86("mov", "rdi", FRQ(_.op_sa))
             + x86("mov", "rsi", FRQ(_.op_sa + 8))
             + x86("mov", "rdx", FRQ(_.op_sb))
             + x86("mov", "rcx", FRQ(_.op_sb + 8))
             + x86("call", "descr_identical", (uint64_t)(uintptr_t)(void *)descr_identical)
             + x86("test", "eax", "eax")
             + x86_omega("jne")
             + IF(_.op_res_live, x86("mov", FRQ(_.op_off),     (long)0)
                               + x86("mov", FRQ(_.op_off + 8), (long)0))
             + x86_gamma()
             + x86_beta_trampoline()
         : x86_bomb("bb_differ: needs two operand slots (op_sa/op_sb) + own value slot (op_off)");
}
