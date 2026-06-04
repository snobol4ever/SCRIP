#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "emit.h"
int bb_slot_claim(int bytes);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static int g_saved_off, g_depth_off, g_stack_base;
static inline int soff()  { return g_saved_off; }
static inline int doff()  { return g_depth_off; }
static inline int sboff() { return g_stack_base; }
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_pat_arbno_str() {
    if (!PLATFORM_X86) return std::string();
    const char *child_lbl = g_emit.bb_child_lbl;
    if (!child_lbl || !child_lbl[0])
        return x86_bomb("bb_pat_arbno: no child label");
    if (MEDIUM_BINARY)
        return x86_bomb("bb_pat_arbno: BINARY arm not yet implemented");
    static const int MAX_DEPTH = 64;
    int nid = _.nid;
    std::string saved = emit_fmt(".Larbno%d_saved", nid);
    std::string depth = emit_fmt(".Larbno%d_depth", nid);
    std::string stack = emit_fmt(".Larbno%d_stack", nid);
    std::string lp    = emit_fmt(".Larbno%d_loop",  nid);
    std::string done  = emit_fmt(".Larbno%d_done",  nid);
    std::string omega = emit_fmt(".Larbno%d_omega", nid);
    return IF(MEDIUM_TEXT,
                s_1asm(std::string(_.lbl_α) + ":")
              + s_comment("# BOX ARBNO() [BROK-2, δ=r14d, Σ=r13, Δ=r15]")
              + s_directive(".section .data")
              + s_directive(saved + ": .long 0")
              + s_directive(depth + ": .long 0")
              + s_directive(stack + ": .skip " + emit_fmt("%d", MAX_DEPTH * 4))
              + s_directive(".section .text")
              + s_directive(".intel_syntax noprefix")
              + s_2asm("mov", "dword ptr [rip + " + saved + "], r14d")
              + s_2asm("mov", "dword ptr [rip + " + depth + "], 0")
              + s_1asm(lp + ":")
              + s_2asm("mov", "ecx, dword ptr [rip + " + depth + "]")
              + s_2asm("cmp", emit_fmt("ecx, %d", MAX_DEPTH))
              + s_2asm("jge", done)
              + s_2asm("mov", "ebx, r14d")
              + s_2asm("mov", "rdi, r12")
              + s_2asm("push", "r10")
              + s_2asm("call", child_lbl)
              + s_2asm("pop", "r10")
              + s_2asm("cmp", "eax, 99")
              + s_2asm("je", done)
              + s_2asm("cmp", "r14d, ebx")
              + s_2asm("je", done)
              + s_2asm("mov", "ecx, dword ptr [rip + " + depth + "]")
              + s_2asm("lea", "rdx, [rip + " + stack + "]")
              + s_2asm("mov", "[rdx + rcx*4], r14d")
              + s_2asm("add", "ecx, 1")
              + s_2asm("mov", "dword ptr [rip + " + depth + "], ecx")
              + s_2asm("jmp", lp)
              + s_1asm(done + ":")
              + s_2asm("jmp", _.lbl_γ)
              + s_1asm(std::string(_.lbl_β) + ":")
              + s_2asm("mov", "ecx, dword ptr [rip + " + depth + "]")
              + s_2asm("sub", "ecx, 1")
              + s_2asm("jl", omega)
              + s_2asm("mov", "dword ptr [rip + " + depth + "], ecx")
              + s_2asm("cmp", "ecx, 0")
              + s_2asm("jg", emit_fmt(".Larbno%d_pop", nid))
              + s_2asm("mov", "r14d, dword ptr [rip + " + saved + "]")
              + s_2asm("jmp", emit_fmt(".Larbno%d_restore", nid))
              + s_1asm(emit_fmt(".Larbno%d_pop:", nid))
              + s_2asm("lea", "rdx, [rip + " + stack + "]")
              + s_2asm("sub", "ecx, 1")
              + s_2asm("mov", "r14d, [rdx + rcx*4]")
              + s_1asm(emit_fmt(".Larbno%d_restore:", nid))
              + s_2asm("jmp", _.lbl_γ)
              + s_1asm(omega + ":")
              + s_2asm("jmp", _.lbl_ω));
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_pat_arbno(IR_t *pBB) {
    (void)pBB;
    g_saved_off  = bb_slot_claim(4);
    g_depth_off  = bb_slot_claim(4);
    g_stack_base = bb_slot_claim(64 * 4);
    bb_emit_x86(bb_pat_arbno_str());
}
