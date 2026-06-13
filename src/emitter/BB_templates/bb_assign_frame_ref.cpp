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
std::string bb_assign_frame_ref() {
    if (PLATFORM_X86) return IF(!g_gvar_flat_chain, x86_bomb("bb_assign_frame_ref: gvar flat-chain only"))
                           + IF(_.bb_lk == 7 && _.op_a_slot < 0, x86_bomb("bb_assign_frame_ref int-binop: op_a_slot==-1 (binop slot not promoted)"))
                           + IF(_.bb_lk == 8 && _.op_a_slot < 0, x86_bomb("bb_assign_frame_ref call-result: op_a_slot==-1 (call result slot not promoted)"))
                           + IF(_.bb_lk == 0, x86_bomb("bb_assign_frame_ref: unhandled rhs shape"))
                           + IF(_.bb_lk != 0,
                             x86("label", _.lbl_α)
                           + x86("comment", "IR_ASSIGN_FRAME_REF"))
                           + IF(_.bb_lk == 1,
                             x86("lea", "rcx", FRQ(0))
                           + FOR(0, (int) _.op_dval, [&](int h) { (void) h; return x86("mov", "rcx", RDQ("rcx", 0)); })
                           + x86("mov", "rcx", RDQ("rcx", 16 + (int) _.op_ival * 16 + 8))
                           + x86("mov", RDQ("rcx", 0), 6L)
                           + x86("movabs", "rax", (uint64_t) _.op_a_ival_sg)
                           + x86("mov", RDQ("rcx", 8), "rax"))
                           + IF(_.bb_lk == 2,
                             x86("lea", "rcx", FRQ(0))
                           + FOR(0, (int) _.op_dval, [&](int h) { (void) h; return x86("mov", "rcx", RDQ("rcx", 0)); })
                           + x86("mov", "rcx", RDQ("rcx", 16 + (int) _.op_ival * 16 + 8))
                           + x86("mov", RDQ("rcx", 0), 0L)
                           + x86("mov", RDQ("rcx", 8), 0L))
                           + IF(_.bb_lk == 3,
                             x86("lea", "rcx", FRQ(0))
                           + FOR(0, (int) _.op_dval, [&](int h) { (void) h; return x86("mov", "rcx", RDQ("rcx", 0)); })
                           + x86("mov", "rcx", RDQ("rcx", 16 + (int) _.op_ival * 16 + 8))
                           + x86("mov", RDQ("rcx", 0), 1L)
                           + x86("lea", "rax", "[rip + __]", (uint64_t)(uintptr_t)(_.op_a_sval ? _.op_a_sval : ""), _.bb_ls)
                           + x86("mov", RDQ("rcx", 8), "rax"))
                           + IF(_.bb_lk == 4,
                             x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)(_.op_a_sval ? _.op_a_sval : ""), _.bb_ls)
                           + x86("call", "rt_gvar_get_descr", (uint64_t)(uintptr_t)(void *) rt_gvar_get_descr)
                           + x86("lea", "rcx", FRQ(0))
                           + FOR(0, (int) _.op_dval, [&](int h) { (void) h; return x86("mov", "rcx", RDQ("rcx", 0)); })
                           + x86("mov", "rcx", RDQ("rcx", 16 + (int) _.op_ival * 16 + 8))
                           + x86("mov", RDQ("rcx", 0), "rax")
                           + x86("mov", RDQ("rcx", 8), "rdx"))
                           + IF(_.bb_lk == 5,
                             x86("lea", "rax", FRQ(0))
                           + FOR(0, (int) _.op_a_dval, [&](int h) { (void) h; return x86("mov", "rax", RDQ("rax", 0)); })
                           + x86("mov", "rsi", RDQ("rax", 16 + (int) _.op_a_ival_sg * 16 + 0))
                           + x86("mov", "rdi", RDQ("rax", 16 + (int) _.op_a_ival_sg * 16 + 8))
                           + x86("lea", "rcx", FRQ(0))
                           + FOR(0, (int) _.op_dval, [&](int h) { (void) h; return x86("mov", "rcx", RDQ("rcx", 0)); })
                           + x86("mov", "rcx", RDQ("rcx", 16 + (int) _.op_ival * 16 + 8))
                           + x86("mov", RDQ("rcx", 0), "rsi")
                           + x86("mov", RDQ("rcx", 8), "rdi"))
                           + IF(_.bb_lk == 6,
                             x86("lea", "rax", FRQ(0))
                           + FOR(0, (int) _.op_a_dval, [&](int h) { (void) h; return x86("mov", "rax", RDQ("rax", 0)); })
                           + x86("mov", "rax", RDQ("rax", 16 + (int) _.op_a_ival_sg * 16 + 8))
                           + x86("mov", "rsi", RDQ("rax", 0))
                           + x86("mov", "rdi", RDQ("rax", 8))
                           + x86("lea", "rcx", FRQ(0))
                           + FOR(0, (int) _.op_dval, [&](int h) { (void) h; return x86("mov", "rcx", RDQ("rcx", 0)); })
                           + x86("mov", "rcx", RDQ("rcx", 16 + (int) _.op_ival * 16 + 8))
                           + x86("mov", RDQ("rcx", 0), "rsi")
                           + x86("mov", RDQ("rcx", 8), "rdi"))
                           + IF(_.bb_lk == 7,
                             x86("mov", "rax", FRQ(_.op_a_slot))
                           + x86("lea", "rcx", FRQ(0))
                           + FOR(0, (int) _.op_dval, [&](int h) { (void) h; return x86("mov", "rcx", RDQ("rcx", 0)); })
                           + x86("mov", "rcx", RDQ("rcx", 16 + (int) _.op_ival * 16 + 8))
                           + x86("mov", RDQ("rcx", 0), 6L)
                           + x86("mov", RDQ("rcx", 8), "rax"))
                           + IF(_.bb_lk == 8,
                             x86("mov", "rax", FRQ(_.op_a_slot))
                           + x86("mov", "rdx", FRQ(_.op_a_slot + 8))
                           + x86("lea", "rcx", FRQ(0))
                           + FOR(0, (int) _.op_dval, [&](int h) { (void) h; return x86("mov", "rcx", RDQ("rcx", 0)); })
                           + x86("mov", "rcx", RDQ("rcx", 16 + (int) _.op_ival * 16 + 8))
                           + x86("mov", RDQ("rcx", 0), "rax")
                           + x86("mov", RDQ("rcx", 8), "rdx"))
                           + IF(_.bb_lk != 0,
                             x86("jmp", "γ")
                           + x86("def", "β")
                           + x86("jmp", "ω"));
    return std::string();
}
