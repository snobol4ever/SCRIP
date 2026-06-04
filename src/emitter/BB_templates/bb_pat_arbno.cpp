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
                x86("label", _.lbl_α)
              + x86("comment", "BOX ARBNO() [BROK-2, δ=r14d, Σ=r13, Δ=r15]")
              + x86("directive", ".section .data")
              + x86("directive", (saved + ": .long 0").c_str())
              + x86("directive", (depth + ": .long 0").c_str())
              + x86("directive", stack + ": .skip " + emit_fmt("%d", MAX_DEPTH * 4))
              + x86("directive", ".section .text")
              + x86("directive", ".intel_syntax noprefix")
              + x86("ins2", "mov", "dword ptr [rip + " + saved + "], r14d")
              + x86("ins2", "mov", "dword ptr [rip + " + depth + "], 0")
              + x86("label", (lp).c_str())
              + x86("ins2", "mov", "ecx, dword ptr [rip + " + depth + "]")
              + x86("ins2", "cmp", emit_fmt("ecx, %d", MAX_DEPTH))
              + x86("ins2", "jge", done)
              + x86("ins2", "mov", "ebx, r14d")
              + x86("ins2", "mov", "rdi, r12")
              + x86("ins2", "push", "r10")
              + x86("ins2", "call", child_lbl)
              + x86("ins2", "pop", "r10")
              + x86("ins2", "cmp", "eax, 99")
              + x86("ins2", "je", done)
              + x86("ins2", "cmp", "r14d, ebx")
              + x86("ins2", "je", done)
              + x86("ins2", "mov", "ecx, dword ptr [rip + " + depth + "]")
              + x86("ins2", "lea", "rdx, [rip + " + stack + "]")
              + x86("ins2", "mov", "[rdx + rcx*4], r14d")
              + x86("ins2", "add", "ecx, 1")
              + x86("ins2", "mov", "dword ptr [rip + " + depth + "], ecx")
              + x86("ins2", "jmp", lp)
              + x86("label", (done).c_str())
              + x86("ins2", "jmp", _.lbl_γ)
              + x86("label", _.lbl_β)
              + x86("ins2", "mov", "ecx, dword ptr [rip + " + depth + "]")
              + x86("ins2", "sub", "ecx, 1")
              + x86("ins2", "jl", omega)
              + x86("ins2", "mov", "dword ptr [rip + " + depth + "], ecx")
              + x86("ins2", "cmp", "ecx, 0")
              + x86("ins2", "jg", emit_fmt(".Larbno%d_pop", nid))
              + x86("ins2", "mov", "r14d, dword ptr [rip + " + saved + "]")
              + x86("ins2", "jmp", emit_fmt(".Larbno%d_restore", nid))
              + x86("label", emit_fmt(".Larbno%d_pop", nid))
              + x86("ins2", "lea", "rdx, [rip + " + stack + "]")
              + x86("ins2", "sub", "ecx, 1")
              + x86("ins2", "mov", "r14d, [rdx + rcx*4]")
              + x86("label", emit_fmt(".Larbno%d_restore", nid))
              + x86("ins2", "jmp", _.lbl_γ)
              + x86("label", (omega).c_str())
              + x86("ins2", "jmp", _.lbl_ω));
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_pat_arbno(IR_t *pBB) {
    (void)pBB;
    g_saved_off  = bb_slot_claim(4);
    g_depth_off  = bb_slot_claim(4);
    g_stack_base = bb_slot_claim(64 * 4);
    bb_emit_x86(bb_pat_arbno_str());
}
