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
std::string bb_var_ref_frame() {
    x86_begin();
    int lvl = _.node ? _.node->seal : 0;
    const char * dreg = lvl == 1 ? "r13" : lvl == 2 ? "r14" : lvl == 3 ? "r15" : (const char *)0;
    int off = (_.op_a_sval && _.op_sval) ? stage2_owner_varslot(_.op_a_sval, _.op_sval) : -1;
    if (off < 0 || _.op_off < 0) return x86_alpha() + x86_bomb("bb_var_ref_frame: owner vslot unresolved") + x86_beta_trampoline();
    if (!dreg) {
        int disp_off = -1;
        const char * l3proc = _.op_a_sval ? stage2_owner_l3_ancestor(_.op_a_sval) : (const char *)0;
        if (l3proc && lvl > 3) { char nm[32]; snprintf(nm, sizeof nm, "__pas_display_%d", lvl); disp_off = stage2_owner_varslot(l3proc, nm); }
        if (disp_off < 0) return x86_alpha() + x86_bomb("bb_var_ref_frame: PAS-DISPLAY level>3 slot unresolved") + x86_beta_trampoline();
        return x86("comment", "IR_VAR_REF uplevel: NAMETRAP{DT_N,slen=1,&slot in the owner frame via the level>3 spilled display}")
             + x86_alpha()
             + x86("mov", "rax", (long)((long)1 << 32 | (long)DT_N))
             + x86("mov", "rcx", RDQ("r15", disp_off))
             + x86("lea", "rdx", RDQ("rcx", off))
             + x86("mov", FRQ(_.op_off),     "rax")
             + x86("mov", FRQ(_.op_off + 8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline();
    }
    return x86("comment", "IR_VAR_REF uplevel: NAMETRAP{DT_N,slen=1,&slot in the owner frame via display}")
         + x86_alpha()
         + x86("mov", "rax", (long)((long)1 << 32 | (long)DT_N))
         + x86("lea", "rdx", RDQ(dreg, off))
         + x86("mov", FRQ(_.op_off),     "rax")
         + x86("mov", FRQ(_.op_off + 8), "rdx")
         + x86_gamma()
         + x86_beta_trampoline();
}
std::string bb_var_ref() {
    x86_begin();
    if (_.op_off == -1) return x86_alpha() + x86_bomb("bb_var_ref: needs own slot");
    if (_.op_gva_k < 0 && _.op_sa == -1) {
        return x86("comment", "IR_VAR_REF anon: rt_pl_fresh_var_ref -> PLJ PLVAR cell")
             + x86_alpha()
             + x86("call", "rt_pl_fresh_var_ref", (uint64_t)(uintptr_t)(void *)rt_pl_fresh_var_ref)
             + x86("mov", FRQ(_.op_off),     "rax")
             + x86("mov", FRQ(_.op_off + 8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline();
    }
    if (_.op_var_named == 1 && _.op_sval && (_.op_sa >= 0 || _.op_gva_k >= 0)) {
        std::string fl = std::string(".L") + x86_boxkind() + "_vrnm" + std::to_string(g_flat_node_id++);
        std::string s = x86("comment", "IR_VAR_REF named: a value-call argument carries its identifier so name() through a value can answer -> rt_var_ref_cell_named(&cell, \"id\")")
                      + x86_alpha()
                      + (_.op_gva_k >= 0
                          ? x86("note", gva_name(_.op_gva_k)) + x86("mov", "rdi", (long)(RT_GVA_VA + _.op_gva_k * 16))
                          : x86("lea", "rdi", FRQ(_.op_sa)))
                      + x86("directive", ".section .rodata")
                      + x86("directive", (fl + ": .string \"" + std::string(_.op_sval) + "\"").c_str())
                      + x86("directive", ".section .text")
                      + x86("directive", ".intel_syntax noprefix")
                      + x86("lea", "rsi", "[rip + __]", (uint64_t)(uintptr_t)_.op_sval, fl.c_str())
                      + x86("call", "rt_var_ref_cell_named", (uint64_t)(uintptr_t)(void *)rt_var_ref_cell_named);
        if (_.op_zres) s += x86("note", ZRESN()) + x86("mov", ZRES(0), "rax") + x86("note", ZRESN()) + x86("mov", ZRES(8), "rdx");
        else           s += x86("mov", FRQ(_.op_off), "rax") + x86("mov", FRQ(_.op_off + 8), "rdx");
        return s + x86_gamma() + x86_beta_trampoline();
    }
    if (_.op_zres && (_.op_sa >= 0 || _.op_gva_k >= 0))
        return x86("comment", "IR_VAR_REF icn cells zd: NAMETRAP{DT_N,slen=1,&____slot} -> ZRES")
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
             + x86_beta_trampoline();
    return x86("comment", "IR_VAR_REF")
         + x86_alpha()
         + x86("mov", "rax", (long)((long)1 << 32 | (long)DT_N))
         + (_.op_gva_k >= 0
             ? x86("note", gva_name(_.op_gva_k))
             + x86("mov", "rdx", (long)(RT_GVA_VA + _.op_gva_k * 16))
             : x86("lea", "rdx", FRQ(_.op_sa)))
         + x86("mov",     FRQ(_.op_off),     "rax")
         + x86("mov",     FRQ(_.op_off + 8), "rdx")
         + x86_gamma()
         + x86_beta_trampoline();
}
