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
static IR_t * bb_chain_terminal_staged(IR_t * entry) {
    IR_t * n = entry; int guard = 0;
    while (n && n->γ && n->γ->t != IR_SUCCEED && n->γ->t != IR_FAIL && guard++ < 4096) n = n->γ;
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
std::string bb_call_proc_staged_str(IR_t * pBB) {
    if (!PLATFORM_X86) return std::string();
    const char * fn   = pBB->sval ? pBB->sval : "";
    int64_t      narg = pBB->ival;
    IR_graph_t ** argblks = (IR_graph_t **)(intptr_t) pBB->counter;
    int off = bb_slot_alloc16(pBB);
    bb_label_t * beta_tgt = bb_call_staged_beta_target();
    if (MEDIUM_BINARY) {
        uint64_t stage_fp; { void (*fp)(int, DESCR_t) = rt_arg_stage; stage_fp = (uint64_t)(uintptr_t)(void*)fp; }
        std::string stage;
        for (int i = 0; i < (int)narg; i++) {
            IR_t * prod = bb_chain_terminal_staged(argblks && argblks[i] ? argblks[i]->entry : NULL);
            int slot = prod ? bb_slot_get(prod) : -1;
            if (slot < 0) slot = 0;
            stage += x86("mov32", "edi", (long)i);
            stage += x86_frame_load64("rsi", slot);
            stage += x86_frame_load64("rdx", slot + 8);
            stage += x86("call", "rt_arg_stage", stage_fp);
        }
        uint64_t fptr; { DESCR_t (*fp)(const char *, int) = rt_call_proc_descr; fptr = (uint64_t)(uintptr_t)(void*)fp; }
        std::string tail;
        tail += x86("mov", "rdi", (uint64_t)(uintptr_t)fn);
        tail += x86("mov32", "esi", (long)narg);
        tail += x86("call", "rt_call_proc_descr", fptr);
        tail += x86_frame_store64(off, "rax");
        tail += x86_frame_store64(off + 8, "rdx");
        tail += x86("cmp", "eax", (long)99);
        tail += x86("je", PORT_OMEGA);
        tail += x86("jmp", PORT_GAMMA);
        tail += x86("def", PORT_BETA);
        if (beta_tgt == _.lbl_ω_p) {
            tail += x86("jmp", PORT_OMEGA);
        } else {
            tail += x86_pair_jmp(0);
        }
        return stage + tail;
    }
    if (MEDIUM_TEXT) {
        int id2 = bb_node_id(pBB);
        std::string nl = emit_fmt(".Lcall%d_pname", id2);
        std::string s = s_1asm(emit_fmt("%s:", _.lbl_α))
                       + s_directive(".section .rodata")
                       + s_directive(nl + ": .string \"" + std::string(fn) + "\"")
                       + s_directive(".section .text")
                       + s_directive(".intel_syntax noprefix");
        for (int i = 0; i < (int)narg; i++) {
            IR_t * prod = bb_chain_terminal_staged(argblks && argblks[i] ? argblks[i]->entry : NULL);
            int slot = prod ? bb_slot_get(prod) : -1;
            if (slot < 0) slot = 0;
            s += s_2asm("mov edi,",  emit_fmt("%d", i))
              +  s_2asm("mov rsi,",  emit_fmt("[r12+%d]", slot))
              +  s_2asm("mov rdx,",  emit_fmt("[r12+%d]", slot + 8))
              +  s_2asm("call",      "rt_arg_stage@PLT");
        }
        s += s_2asm("lea rdi,", "[rip + " + nl + "]")
          +  s_2asm("mov esi,",  emit_fmt("%d", (int)narg))
          +  s_2asm("call",      "rt_call_proc_descr@PLT")
          +  s_2asm("mov",       emit_fmt("[r12+%d], rax", off))
          +  s_2asm("mov",       emit_fmt("[r12+%d], rdx", off + 8))
          +  s_2asm("cmp",       "eax, 99")
          +  s_2asm("je",        _.lbl_ω)
          +  s_2asm("jmp",       _.lbl_γ)
          +  s_L1asm(emit_fmt("%s:", _.lbl_β), "")
          +  s_2asm("jmp",       beta_tgt ? beta_tgt->name : _.lbl_ω);
        return s;
    }
    return std::string();
}
