#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "emit.h"
#include "emit_bb.h"
DESCR_t rt_call_proc_descr(const char *name, int nargs);
void rt_arg_stage(int idx, DESCR_t v);
int  rt_proc_is_registered(const char *name);
int  bb_slot_get(IR_t * nd);
int  bb_slot_alloc16(IR_t * nd);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * bb_chain_terminal_staged(IR_t * entry) { IR_t * n = entry; int guard = 0;
    while (n && n->γ && n->γ->op != IR_SUCCEED && n->γ->op != IR_FAIL && guard++ < 4096) n = n->γ;
    return n;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static bb_label_t * bb_call_staged_beta_target() {
    for (int i = 0; i < g_emit.xa_bb_emit_pair_n; i++)
        if (g_emit.xa_bb_emit_pair_define[i] == _.lbl_β_p && g_emit.xa_bb_emit_pair_jmp[i])
            return g_emit.xa_bb_emit_pair_jmp[i];
    return _.lbl_ω_p;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bcps_bin_arm() { int off = bb_slot_alloc16(_.node); bb_label_t * beta_tgt = bb_call_staged_beta_target(); IR_graph_t ** argblks = (IR_graph_t **)(intptr_t)_.op_counter; uint64_t stage_fp; { void (*fp)(int, DESCR_t) = rt_arg_stage; stage_fp = (uint64_t)(uintptr_t)(void*)fp; } uint64_t fptr; { DESCR_t (*fp)(const char *, int) = rt_call_proc_descr; fptr = (uint64_t)(uintptr_t)(void*)fp; }
    return FOR(0, (int)_.op_ival, [&](int i) { IR_t * prod = bb_chain_terminal_staged(argblks && argblks[i] ? argblks[i]->entry : NULL); int slot = prod ? bb_slot_get(prod) : -1; if (slot < 0) slot = 0; return x86("mov32", "edi", (long)i) + x86_frame_load64("rsi", slot) + x86_frame_load64("rdx", slot + 8) + x86("call", "rt_arg_stage", stage_fp); })
         + x86("mov", "rdi", (uint64_t)(uintptr_t)(_.op_sval ? _.op_sval : ""))
         + x86("mov32", "esi", (long)_.op_ival)
         + x86("call", "rt_call_proc_descr", fptr)
         + x86_frame_store64(off, "rax")
         + x86_frame_store64(off + 8, "rdx")
         + x86("cmp", "eax", (long)99)
         + x86("je", "ω")
         + x86("jmp", "γ")
         + x86("def", "β")
         + (beta_tgt == _.lbl_ω_p ? x86("jmp", "ω") : x86_pair_jmp(0));
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bcps_txt_arm() { int off = bb_slot_alloc16(_.node); bb_label_t * beta_tgt = bb_call_staged_beta_target(); IR_graph_t ** argblks = (IR_graph_t **)(intptr_t)_.op_counter;
    return x86("label", _.lbl_α)
         + x86("directive", ".section .rodata")
         + x86("directive", std::string(".Lcall") + std::to_string(_.nid) + "_pname: .string \"" + std::string(_.op_sval ? _.op_sval : "") + "\"")
         + x86("directive", ".section .text")
         + x86("directive", ".intel_syntax noprefix")
         + FOR(0, (int)_.op_ival, [&](int i) { IR_t * prod = bb_chain_terminal_staged(argblks && argblks[i] ? argblks[i]->entry : NULL); int slot = prod ? bb_slot_get(prod) : -1; if (slot < 0) slot = 0; return x86("ins2", "mov edi,", std::to_string(i)) + x86("ins2", "mov rsi,", "[r12+" + std::to_string(slot) + "]") + x86("ins2", "mov rdx,", "[r12+" + std::to_string(slot + 8) + "]") + x86("ins2", "call", "rt_arg_stage@PLT"); })
         + x86("ins2", "lea rdi,", std::string("[rip + .Lcall") + std::to_string(_.nid) + "_pname]")
         + x86("ins2", "mov esi,", std::to_string((int)_.op_ival))
         + x86("ins2", "call", "rt_call_proc_descr@PLT")
         + x86("ins2", "mov", "[r12+" + std::to_string(off) + "], rax")
         + x86("ins2", "mov", "[r12+" + std::to_string(off + 8) + "], rdx")
         + x86("ins2", "cmp", "eax, 99")
         + x86("ins2", "je", _.lbl_ω)
         + x86("ins2", "jmp", _.lbl_γ)
         + x86("Lins1", std::string(_.lbl_β) + ":", "")
         + x86("ins2", "jmp", beta_tgt ? beta_tgt->name : _.lbl_ω);
}
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_call_proc_staged_str(IR_t * pBB) {
    if (!PLATFORM_X86) return std::string();
    if (MEDIUM_BINARY) return bcps_bin_arm();
    if (MEDIUM_TEXT) return bcps_txt_arm();
    return std::string();
}
