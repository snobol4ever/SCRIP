#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
int subscript_set(DESCR_t arr, DESCR_t idx, DESCR_t val);
DESCR_t subscript_get(DESCR_t arr, DESCR_t idx);
extern int g_descr_flat_chain;
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* IR_RASGN — Icon reversible assignment `x <- v` (canonical oasgn.r: GeneralAsgn(x,v); suspend x; on resume
   GeneralAsgn(x,saved); fail).  Two shapes, selected by op_ival.  op_ival==0: simple variable lvalue — the
   value lives in a frame slot; save old (op_sb->op_sc), write new (op_a_slot->op_sb), yield, restore on β.
   op_ival==1: SUBSCRIPT lvalue arr[i] — base/key/value slots in op_a_slot/op_sb/op_sc, save slot op_off.  α:
   subscript_get(base,key) -> save slot; subscript_set(base,key,value); jmp γ.  β: subscript_set(base,key,saved);
   jmp ω.  DESCRs pass arr in rdi:rsi, idx in rdx:rcx, val in r8:r9 (the bb_idx_set convention). */
static std::string bb_rasgn_subs() {
    if (!(_.op_a_slot >= 0 && _.op_sb >= 0 && _.op_sc >= 0 && _.op_off >= 0))
        return x86_bomb("bb_rasgn: arr[i]<-v needs base/key/value/save slots");
    x86_begin();
    return x86("label", _.lbl_α)
         + x86("comment", "IR_RASGN arr[i]<-v: save old elem, set new, suspend; restore on resume")
         + x86("mov", "rdi", FRQ(_.op_a_slot))
         + x86("mov", "rsi", FRQ(_.op_a_slot + 8))
         + x86("mov", "rdx", FRQ(_.op_sb))
         + x86("mov", "rcx", FRQ(_.op_sb + 8))
         + x86("call", "subscript_get", (uint64_t)(uintptr_t)(void *) subscript_get)
         + x86("mov", FRQ(_.op_off),     "rax")
         + x86("mov", FRQ(_.op_off + 8), "rdx")
         + x86("mov", "rdi", FRQ(_.op_a_slot))
         + x86("mov", "rsi", FRQ(_.op_a_slot + 8))
         + x86("mov", "rdx", FRQ(_.op_sb))
         + x86("mov", "rcx", FRQ(_.op_sb + 8))
         + x86("mov", "r8",  FRQ(_.op_sc))
         + x86("mov", "r9",  FRQ(_.op_sc + 8))
         + x86("call", "subscript_set", (uint64_t)(uintptr_t)(void *) subscript_set)
         + x86("jmp", "γ")
         + x86("def", "β")
         + x86("mov", "rdi", FRQ(_.op_a_slot))
         + x86("mov", "rsi", FRQ(_.op_a_slot + 8))
         + x86("mov", "rdx", FRQ(_.op_sb))
         + x86("mov", "rcx", FRQ(_.op_sb + 8))
         + x86("mov", "r8",  FRQ(_.op_off))
         + x86("mov", "r9",  FRQ(_.op_off + 8))
         + x86("call", "subscript_set", (uint64_t)(uintptr_t)(void *) subscript_set)
         + x86("jmp", "ω");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_rasgn() {
    if (!PLATFORM_X86) return std::string();
    if (_.op_ival == 1) return bb_rasgn_subs();
    return IF(!(_.op_sb >= 0 && _.op_a_slot >= 0 && _.op_sc >= 0 && _.op_off >= 0), x86_bomb("bb_rasgn: x<-v needs varslot + rhs slot + save slot + own slot"))
                           + IF(_.op_sb >= 0 && _.op_a_slot >= 0 && _.op_sc >= 0 && _.op_off >= 0,
                             x86("label", _.lbl_α)
                           + x86("comment", "IR_RASGN x<-v")
                           + x86("mov", "rax", FRQ(_.op_sb))
                           + x86("mov", "rdx", FRQ(_.op_sb + 8))
                           + x86("mov", FRQ(_.op_sc), "rax")
                           + x86("mov", FRQ(_.op_sc + 8), "rdx")
                           + x86("mov", "rcx", FRQ(_.op_a_slot))
                           + x86("mov", "rsi", FRQ(_.op_a_slot + 8))
                           + x86("mov", FRQ(_.op_sb), "rcx")
                           + x86("mov", FRQ(_.op_sb + 8), "rsi")
                           + x86("mov", FRQ(_.op_off), "rcx")
                           + x86("mov", FRQ(_.op_off + 8), "rsi")
                           + x86("jmp", "γ")
                           + x86("def", "β")
                           + x86("mov", "rax", FRQ(_.op_sc))
                           + x86("mov", "rdx", FRQ(_.op_sc + 8))
                           + x86("mov", FRQ(_.op_sb), "rax")
                           + x86("mov", FRQ(_.op_sb + 8), "rdx")
                           + x86("jmp", "ω"));
}
