#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern int g_gva_active;
DESCR_t NV_SET_fn(const char * name, DESCR_t val);
DESCR_t NV_GET_fn(const char * name);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_rev_assign_global() {
    if (!(_.op_a_slot >= 0 && _.op_sc >= 0 && _.op_off >= 0))
        return x86_alpha() + x86_bomb("bb_rev_assign_global: g<-v needs rhs slot + save slot + own slot");
    return IF(g_gva_active && _.op_gva_k >= 0,
              x86("comment", "IR_REV_ASSIGN g<-v gva: save the global into the frame save slot, store the rhs, restore on recede")
            + x86_alpha()
            + x86("note", gva_name(_.op_gva_k))
            + x86("mov", "rax", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(_.op_gva_k, 0) : ABSQ(RT_GVA_VA + _.op_gva_k * 16 + 0))
            + x86("note", gva_name(_.op_gva_k))
            + x86("mov", "rdx", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(_.op_gva_k, 8) : ABSQ(RT_GVA_VA + _.op_gva_k * 16 + 8))
            + x86("mov", FRQ(_.op_sc), "rax")
            + x86("mov", FRQ(_.op_sc + 8), "rdx")
            + x86("mov", "rcx", FRQ(_.op_a_slot))
            + x86("mov", "rsi", FRQ(_.op_a_slot + 8))
            + x86("note", gva_name(_.op_gva_k))
            + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(_.op_gva_k, 0) : ABSQ(RT_GVA_VA + _.op_gva_k * 16 + 0), "rcx")
            + x86("note", gva_name(_.op_gva_k))
            + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(_.op_gva_k, 8) : ABSQ(RT_GVA_VA + _.op_gva_k * 16 + 8), "rsi")
            + x86("mov", FRQ(_.op_off), "rcx")
            + x86("mov", FRQ(_.op_off + 8), "rsi")
            + x86_gamma()
            + x86_beta()
            + x86("mov", "rax", FRQ(_.op_sc))
            + x86("mov", "rdx", FRQ(_.op_sc + 8))
            + x86("note", gva_name(_.op_gva_k))
            + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(_.op_gva_k, 0) : ABSQ(RT_GVA_VA + _.op_gva_k * 16 + 0), "rax")
            + x86("note", gva_name(_.op_gva_k))
            + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(_.op_gva_k, 8) : ABSQ(RT_GVA_VA + _.op_gva_k * 16 + 8), "rdx")
            + x86_omega())
         + IF(!(g_gva_active && _.op_gva_k >= 0),
              x86("comment", "IR_REV_ASSIGN g<-v by name: NV_GET to save, NV_SET to store, NV_SET again on recede")
            + x86_alpha()
            + x86("mov", "rdi", ROQ(0))
            + x86("call", "NV_GET_fn", (uint64_t)(uintptr_t)(void *)NV_GET_fn)
            + x86("mov", FRQ(_.op_sc), "rax")
            + x86("mov", FRQ(_.op_sc + 8), "rdx")
            + x86("mov", "rsi", FRQ(_.op_a_slot))
            + x86("mov", "rdx", FRQ(_.op_a_slot + 8))
            + x86("mov", "rdi", ROQ(0))
            + x86("rtcc_wb")
            + x86("call_bare", "NV_SET_fn", (uint64_t)(uintptr_t)(void *)(DESCR_t (*)(const char *, DESCR_t))NV_SET_fn)
            + x86("rtcc_rl")
            + x86("mov", "rcx", FRQ(_.op_a_slot))
            + x86("mov", "rsi", FRQ(_.op_a_slot + 8))
            + x86("mov", FRQ(_.op_off), "rcx")
            + x86("mov", FRQ(_.op_off + 8), "rsi")
            + x86_gamma()
            + x86_beta()
            + x86("mov", "rsi", FRQ(_.op_sc))
            + x86("mov", "rdx", FRQ(_.op_sc + 8))
            + x86("mov", "rdi", ROQ(0))
            + x86("rtcc_wb")
            + x86("call_bare", "NV_SET_fn", (uint64_t)(uintptr_t)(void *)(DESCR_t (*)(const char *, DESCR_t))NV_SET_fn)
            + x86("rtcc_rl")
            + x86_omega()
            + x86("def", L(0))
            + x86(".quad", LS(0), _.op_sval)
            + x86("label", LS(0))
            + x86(".string", _.op_sval));
}
