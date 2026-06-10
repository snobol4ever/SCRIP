#include <string>
#include <cstdint>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
extern int g_gvar_flat_chain;
#include "emit.h"
#include "descr.h"
DESCR_t rt_gvar_get_descr(const char * name);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bafr_voff() { return 16 + (int) _.op_ival * 16; }
static int bafr_soff(int extra) { return 16 + (int) _.op_a_ival_sg * 16 + extra; }
static std::string bafr_hop() {
    return x86("lea", "rcx", FRQ(0))
         + FOR(0, (int) _.op_dval, [&](int h) { (void) h; return x86("mov", "rcx", RDQ("rcx", 0)); })
         + x86("mov", "rcx", RDQ("rcx", bafr_voff() + 8));
}
static std::string bafr_srchop() {
    return x86("lea", "rax", FRQ(0))
         + FOR(0, (int) _.op_a_dval, [&](int h) { (void) h; return x86("mov", "rax", RDQ("rax", 0)); });
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_assign_frame_ref_str() {
    if (PLATFORM_X86) return IF(!g_gvar_flat_chain, x86_bomb("bb_assign_frame_ref: gvar flat-chain only"))
                           + IF(_.bb_lk == 7 && _.op_a_slot < 0, x86_bomb("bb_assign_frame_ref int-binop: op_a_slot==-1 (binop slot not promoted)"))
                           + IF(_.bb_lk == 8 && _.op_a_slot < 0, x86_bomb("bb_assign_frame_ref call-result: op_a_slot==-1 (call result slot not promoted)"))
                           + IF(_.bb_lk == 0, x86_bomb("bb_assign_frame_ref: unhandled rhs shape"))
                           + IF(_.bb_lk != 0,
                             x86("label", _.lbl_α)
                           + x86("comment", "IR_ASSIGN_FRAME_REF"))
                           + IF(_.bb_lk == 1,
                             bafr_hop()
                           + x86("mov", RDQ("rcx", 0), 6L)
                           + x86("movabs", "rax", (uint64_t) _.op_a_ival_sg)
                           + x86("mov", RDQ("rcx", 8), "rax"))
                           + IF(_.bb_lk == 2,
                             bafr_hop()
                           + x86("mov", RDQ("rcx", 0), 0L)
                           + x86("mov", RDQ("rcx", 8), 0L))
                           + IF(_.bb_lk == 3,
                             bafr_hop()
                           + x86("mov", RDQ("rcx", 0), 1L)
                           + x86("lea", "rax", "[rip + __]", (uint64_t)(uintptr_t)(_.op_a_sval ? _.op_a_sval : ""), _.bb_ls)
                           + x86("mov", RDQ("rcx", 8), "rax"))
                           + IF(_.bb_lk == 4,
                             x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)(_.op_a_sval ? _.op_a_sval : ""), _.bb_ls)
                           + x86("call", "rt_gvar_get_descr", (uint64_t)(uintptr_t)(void *) rt_gvar_get_descr)
                           + bafr_hop()
                           + x86("mov", RDQ("rcx", 0), "rax")
                           + x86("mov", RDQ("rcx", 8), "rdx"))
                           + IF(_.bb_lk == 5,
                             bafr_srchop()
                           + x86("mov", "rsi", RDQ("rax", bafr_soff(0)))
                           + x86("mov", "rdi", RDQ("rax", bafr_soff(8)))
                           + bafr_hop()
                           + x86("mov", RDQ("rcx", 0), "rsi")
                           + x86("mov", RDQ("rcx", 8), "rdi"))
                           + IF(_.bb_lk == 6,
                             bafr_srchop()
                           + x86("mov", "rax", RDQ("rax", bafr_soff(8)))
                           + x86("mov", "rsi", RDQ("rax", 0))
                           + x86("mov", "rdi", RDQ("rax", 8))
                           + bafr_hop()
                           + x86("mov", RDQ("rcx", 0), "rsi")
                           + x86("mov", RDQ("rcx", 8), "rdi"))
                           + IF(_.bb_lk == 7,
                             x86("mov", "rax", FRQ(_.op_a_slot))
                           + bafr_hop()
                           + x86("mov", RDQ("rcx", 0), 6L)
                           + x86("mov", RDQ("rcx", 8), "rax"))
                           + IF(_.bb_lk == 8,
                             x86("mov", "rax", FRQ(_.op_a_slot))
                           + x86("mov", "rdx", FRQ(_.op_a_slot + 8))
                           + bafr_hop()
                           + x86("mov", RDQ("rcx", 0), "rax")
                           + x86("mov", RDQ("rcx", 8), "rdx"))
                           + IF(_.bb_lk != 0,
                             x86("jmp", "γ")
                           + x86("def", "β")
                           + x86("jmp", "ω"));
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_assign_frame_ref(void) { bb_emit_x86(bb_assign_frame_ref_str()); }
