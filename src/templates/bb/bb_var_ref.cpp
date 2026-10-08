#include <string>
#include <stdint.h>
#include <string.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern DESCR_t rt_var_ref_cell(DESCR_t *cellp);
extern DESCR_t rt_var_ref_cell_named(DESCR_t *cellp, const char *name);
extern DESCR_t rt_pl_fresh_var_ref(void);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" { int stage2_owner_varslot(const char * proc, const char * var); const char * stage2_owner_l3_ancestor(const char * proc); }
#define VRF_BAD() (_.op_sa < 0 || _.op_off < 0)
std::string bb_var_ref_frame() {
    x86_begin();
    return IF(VRF_BAD(),
               x86_alpha() + x86_bomb("bb_var_ref_frame: owner vslot unresolved") + x86_beta_trampoline())
         + IF(!VRF_BAD() && !_.op_name1 && _.op_sb < 0,
               x86_alpha() + x86_bomb("bb_var_ref_frame: PAS-DISPLAY level>3 slot unresolved") + x86_beta_trampoline())
         + IF(!VRF_BAD() && !_.op_name1 && _.op_sb >= 0,
               x86("comment", "IR_VAR_REF uplevel: NAMETRAP{DT_N,slen=1,&slot in the owner frame via the level>3 spilled display}")
             + x86_alpha()
             + x86("mov", "rax", (long)((long)1 << 32 | (long)DT_N))
             + x86("mov", "rcx", RDQ("r15", _.op_sb))
             + x86("lea", "rdx", RDQ("rcx", _.op_sa))
             + x86("mov", FRQ(_.op_off), "rax")
             + x86("mov", FRQ(_.op_off + 8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline())
         + IF(!VRF_BAD() && _.op_name1,
               x86("comment", "IR_VAR_REF uplevel: NAMETRAP{DT_N,slen=1,&slot in the owner frame via display}")
             + x86_alpha()
             + x86("mov", "rax", (long)((long)1 << 32 | (long)DT_N))
             + x86("lea", "rdx", RDQ(_.op_name1, _.op_sa))
             + x86("mov", FRQ(_.op_off), "rax")
             + x86("mov", FRQ(_.op_off + 8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline());
}
#define VR_ANON() (_.op_gva_k < 0 && _.op_sa == -1)
#define VR_NAMED() (_.op_var_named == 1 && _.op_sval && (_.op_sa >= 0 || _.op_gva_k >= 0))
#define VR_ICNZD() (_.op_zres && (_.op_sa >= 0 || _.op_gva_k >= 0))
std::string bb_var_ref() {
    x86_begin();
    return IF(_.op_off == -1, x86_alpha() + x86_bomb("bb_var_ref: needs own slot"))
         + IF(_.op_off != -1 && VR_ANON(),
               x86("comment", "IR_VAR_REF anon: rt_pl_fresh_var_ref -> PLJ PLVAR cell")
             + x86_alpha()
             + x86("call", "rt_pl_fresh_var_ref", (uint64_t)(uintptr_t)(void *)rt_pl_fresh_var_ref)
             + x86("mov", FRQ(_.op_off), "rax")
             + x86("mov", FRQ(_.op_off + 8), "rdx")
             + x86_rt_gc_poll()
             + x86_gamma()
             + x86_beta_trampoline())
         + IF(_.op_off != -1 && !VR_ANON() && VR_NAMED(),
               x86("comment", "IR_VAR_REF named: a value-call argument carries its identifier so name() through a value can answer -> rt_var_ref_cell_named(&cell, \"id\")")
             + x86_alpha()
             + (_.op_gva_k >= 0
                 ? x86("note", gva_name(_.op_gva_k))
                 + x86("mov", "rdi", (long)(RT_GVA_VA + _.op_gva_k * 16))
                 : x86("lea", "rdi", FRQ(_.op_sa)))
             + x86_rodata_str_lea("rsi", _.op_sval, "_vrnm")
             + x86("call", "rt_var_ref_cell_named", (uint64_t)(uintptr_t)(void *)rt_var_ref_cell_named)
             + IF(_.op_zres,
                  x86("note", ZRESN())
                + x86("mov", ZRES(0), "rax")
                + x86("note", ZRESN())
                + x86("mov", ZRES(8), "rdx"))
             + IF(!_.op_zres,
                  x86("mov", FRQ(_.op_off), "rax")
                + x86("mov", FRQ(_.op_off + 8), "rdx"))
             + x86_rt_gc_poll()
             + x86_gamma()
             + x86_beta_trampoline())
         + IF(_.op_off != -1 && !VR_ANON() && !VR_NAMED() && VR_ICNZD(),
               x86("comment", "IR_VAR_REF icn cells zd: NAMETRAP{DT_N,slen=1,&____slot} -> ZRES")
             + x86_alpha()
             + x86("mov", "rax", (long)((long)1 << 32 | (long)DT_N))
             + x86("note", ZRESN())
             + x86("mov", ZRES(0), "rax")
             + (_.op_gva_k >= 0
                 ? x86("note", gva_name(_.op_gva_k))
                 + x86("mov", "rax", (long)(RT_GVA_VA + _.op_gva_k * 16))
                 : x86("lea", "rax", FRQ(_.op_sa)))
             + x86("note", ZRESN())
             + x86("mov", ZRES(8), "rax")
             + x86_gamma()
             + x86_beta_trampoline())
         + IF(_.op_off != -1 && !VR_ANON() && !VR_NAMED() && !VR_ICNZD(),
               x86("comment", "IR_VAR_REF")
             + x86_alpha()
             + x86("mov", "rax", (long)((long)1 << 32 | (long)DT_N))
             + (_.op_gva_k >= 0
                 ? x86("note", gva_name(_.op_gva_k))
                 + x86("mov", "rdx", (long)(RT_GVA_VA + _.op_gva_k * 16))
                 : x86("lea", "rdx", FRQ(_.op_sa)))
             + x86("mov", FRQ(_.op_off), "rax")
             + x86("mov", FRQ(_.op_off + 8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline());
}
