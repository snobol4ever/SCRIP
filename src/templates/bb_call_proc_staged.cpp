#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
DESCR_t rt_call_proc_descr(const char *name, int nargs);
DESCR_t rt_proc_call_gen_h(const char *name, int nargs, void **act_slot);
DESCR_t rt_proc_resume_frame(void *act);
DESCR_t rt_proc_resume_frame_h(void **hslot);
int  rt_proc_is_generator(const char *name);
void rt_arg_stage(int idx, DESCR_t v);
int  rt_proc_is_registered(const char *name);
int  bb_slot_get(IR_t * nd);
void bb_slot_register(IR_t * nd, int off);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * bb_chain_terminal_staged(IR_t * entry) { IR_t * n = entry; int guard = 0;
    while (n && n->γ.node && n->γ.node->op != IR_SUCCEED && n->γ.node->op != IR_FAIL && guard++ < 4096) n = n->γ.node;
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static bb_label_t * bb_call_staged_beta_target() {
    for (int i = 0; i < g_emit.xa_bb_emit_pair_n; i++)
        if (g_emit.xa_bb_emit_pair_define[i] == _.lbl_β_p && g_emit.xa_bb_emit_pair_jmp[i])
            return g_emit.xa_bb_emit_pair_jmp[i];
    return _.lbl_ω_p;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bcps_arg_slot(IR_t * call, IR_graph_t ** argblks, int i) {
    IR_t * a = ir_call_arg(call, i);
    if (a) { int s = bb_slot_get(a); if (s < 0) s = zls_off(a); if (s >= 0) return s; }
    IR_t * prod = bb_chain_terminal_staged(argblks && argblks[i] ? argblks[i]->entry : NULL); int s = prod ? bb_slot_get(prod) : -1; return s < 0 ? 0 : s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bcps_result_slot() {
    IR_t * nd = _.node;
    { int _s = nd ? zls_off(nd) : -1; if (_s >= 0) { if (bb_slot_get(nd) < 0) bb_slot_register(nd, _s); return _s; } }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcps_bin_arm() {
    int off = bcps_result_slot(); if (off < 0) return x86_bomb("bb_call_proc_staged: no LOWER slot grant (TMP-ERADICATE)");
    bb_label_t * beta_tgt = bb_call_staged_beta_target(); IR_graph_t ** argblks = (IR_graph_t **)(intptr_t)_.op_counter;
    uint64_t stage_fp; { void (*fp)(int, DESCR_t) = rt_arg_stage; stage_fp = (uint64_t)(uintptr_t)(void*)fp; }
    uint64_t fptr; { DESCR_t (*fp)(const char *, int) = rt_call_proc_descr; fptr = (uint64_t)(uintptr_t)(void*)fp; }
    return x86_alpha()
         + FOR(0, (int)_.op_ival, [&](int i) {
        int slot = bcps_arg_slot(_.node, argblks, i);
        return x86("mov32", "edi", (long)i) + x86_frame_load64("rsi", slot) + x86_frame_load64("rdx", slot + 8) + x86("call", "rt_arg_stage", stage_fp);
    })
         + x86("mov", "rdi", (uint64_t)(uintptr_t)(_.op_sval ? _.op_sval : ""))
         + x86("mov32", "esi", (long)_.op_ival)
         + x86("call", "rt_call_proc_descr", fptr)
         + x86_frame_store64(off, "rax")
         + x86_frame_store64(off + 8, "rdx")
         + x86("cmp", "eax", (long)99)
         + x86_omega("je")
         + x86_gamma()
         + x86_beta()
         + (beta_tgt == _.lbl_ω_p ? x86_omega() : x86_pair_jmp(0));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcps_txt_arm() {
    int off = bcps_result_slot(); if (off < 0) return x86_bomb("bb_call_proc_staged: no LOWER slot grant (TMP-ERADICATE)");
    bb_label_t * beta_tgt = bb_call_staged_beta_target(); IR_graph_t ** argblks = (IR_graph_t **)(intptr_t)_.op_counter;
    return x86_alpha()
         + x86("directive", ".section .rodata")
         + x86("directive", std::string(".Lcall") + std::to_string(_.nid) + "_pname: .string \"" + std::string(_.op_sval ? _.op_sval : "") + "\"")
         + x86("directive", ".section .text")
         + x86("directive", ".intel_syntax noprefix")
         + FOR(0, (int)_.op_ival, [&](int i) {
             int slot = bcps_arg_slot(_.node, argblks, i);
             return x86("mov", "edi", std::to_string(i)) + x86("mov", "rsi", FRQ(slot)) + x86("mov", "rdx", FRQ(slot + 8)) + x86("call", "rt_arg_stage@PLT");
         })
         + x86("directive", (std::string(" lea rdi, [rip + .Lcall") + std::to_string(_.nid) + "_pname]").c_str())
         + x86("mov", "esi", std::to_string((int)_.op_ival))
         + x86("call", "rt_call_proc_descr@PLT")
         + x86("mov", FRQ(off), "rax")
         + x86("mov", FRQ(off + 8), "rdx")
         + x86("cmp", "eax", "99")
         + x86_omega("je")
         + x86_gamma()
         + x86("label", _.lbl_β)
         + x86("jmp", beta_tgt ? beta_tgt->name : _.lbl_ω);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcps_bin_gen_arm() {
    int off = bcps_result_slot(); if (off < 0) return x86_bomb("bb_call_proc_staged: no LOWER slot grant (TMP-ERADICATE)");
    int act = off + 16 * (1 + (int)_.op_ival);
    IR_graph_t ** argblks = (IR_graph_t **)(intptr_t)_.op_counter;
    uint64_t stage_fp; { void (*fp)(int, DESCR_t) = rt_arg_stage; stage_fp = (uint64_t)(uintptr_t)(void*)fp; }
    uint64_t callg_fp; { DESCR_t (*fp)(const char *, int, void **) = rt_proc_call_gen_h; callg_fp = (uint64_t)(uintptr_t)(void*)fp; }
    uint64_t resumeg_fp; { DESCR_t (*fp)(void **) = rt_proc_resume_frame_h; resumeg_fp = (uint64_t)(uintptr_t)(void*)fp; }
    return x86_alpha()
         + FOR(0, (int)_.op_ival, [&](int i) {
        int slot = bcps_arg_slot(_.node, argblks, i);
        return x86("mov32", "edi", (long)i) + x86_frame_load64("rsi", slot) + x86_frame_load64("rdx", slot + 8) + x86("call", "rt_arg_stage", stage_fp);
    })
         + x86("mov", "rdi", (uint64_t)(uintptr_t)(_.op_sval ? _.op_sval : ""))
         + x86("mov32", "esi", (long)_.op_ival)
         + x86_frame_lea("rdx", act)
         + x86("call", "rt_proc_call_gen_h", callg_fp)
         + x86_frame_store64(off, "rax")
         + x86_frame_store64(off + 8, "rdx")
         + x86("cmp", "eax", (long)99)
         + x86_omega("je")
         + x86_gamma()
         + x86_beta()
         + x86_frame_lea("rdi", act)
         + x86("call", "rt_proc_resume_frame_h", resumeg_fp)
         + x86_frame_store64(off, "rax")
         + x86_frame_store64(off + 8, "rdx")
         + x86("cmp", "eax", (long)99)
         + x86_omega("je")
         + x86_gamma();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcps_txt_gen_arm() {
    int off = bcps_result_slot(); if (off < 0) return x86_bomb("bb_call_proc_staged: no LOWER slot grant (TMP-ERADICATE)");
    int act = off + 16 * (1 + (int)_.op_ival);
    IR_graph_t ** argblks = (IR_graph_t **)(intptr_t)_.op_counter;
    return x86_alpha()
         + x86("directive", ".section .rodata")
         + x86("directive", std::string(".Lcall") + std::to_string(_.nid) + "_pname: .string \"" + std::string(_.op_sval ? _.op_sval : "") + "\"")
         + x86("directive", ".section .text")
         + x86("directive", ".intel_syntax noprefix")
         + FOR(0, (int)_.op_ival, [&](int i) {
             int slot = bcps_arg_slot(_.node, argblks, i);
             return x86("mov", "edi", std::to_string(i)) + x86("mov", "rsi", FRQ(slot)) + x86("mov", "rdx", FRQ(slot + 8)) + x86("call", "rt_arg_stage@PLT");
         })
         + x86("directive", (std::string(" lea rdi, [rip + .Lcall") + std::to_string(_.nid) + "_pname]").c_str())
         + x86("mov", "esi", std::to_string((int)_.op_ival))
         + x86_frame_lea("rdx", act)
         + x86("call", "rt_proc_call_gen_h@PLT")
         + x86("mov", FRQ(off), "rax")
         + x86("mov", FRQ(off + 8), "rdx")
         + x86("cmp", "eax", "99")
         + x86_omega("je")
         + x86_gamma()
         + x86("label", _.lbl_β)
         + x86_frame_lea("rdi", act)
         + x86("call", "rt_proc_resume_frame_h@PLT")
         + x86("mov", FRQ(off), "rax")
         + x86("mov", FRQ(off + 8), "rdx")
         + x86("cmp", "eax", "99")
         + x86_omega("je")
         + x86_gamma();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_call_proc_staged_str(IR_t * pBB) {
    if (!PLATFORM_X86) return std::string();
    int is_gen = _.op_sval && rt_proc_is_generator(_.op_sval);
    if (is_gen && _.op_node_kind != (int)IR_PROC_GEN && _.op_node_kind != (int)IR_CALL_PROC_STAGED) return x86_alpha() + x86_bomb("bb_call_proc_staged: generator call on an op kind without a callgen.act ZLS2 handle grant (zeta_storage.c widens only IR_PROC_GEN / IR_CALL_PROC_STAGED)");
    if (MEDIUM_BINARY) return is_gen ? bcps_bin_gen_arm() : bcps_bin_arm();
    if (MEDIUM_TEXT) return is_gen ? bcps_txt_gen_arm() : bcps_txt_arm();
    return std::string();
}
