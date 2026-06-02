/* bb_pat_fence.cpp — BB template for FENCE. x86() self-encoding (template-revamp, 2026-06-02, Opus 4.8).
   FENCE (primitive, no argument) matches the null string when the scanner is moving left to right, but fails
   if the scanner has to back up through it (SPITBOL Manual ch.18/Tutorial: "matches the null string and
   succeeds when the scanner is moving left to right in the pattern, but fails if the scanner has to back up
   through it, seeking alternatives"). Mode-2 oracle (bb_exec.c IR_PAT_FENCE): state==0 → α: save Δ(cursor)
   to counter, return γ; state>0 → β: restore Δ from counter, return ω.
   REG-3 registers: cursor δ=R14d (ratified, established by BB_MATCH α per REG-0). One ζ-frame dword:
   saved_δ @ [r12+off] — claimed by bb_slot_claim(4), BINARY==TEXT (no movabs, no rip-rel .data).
   α: save δ to ζ-slot; → γ (null match, always succeeds on forward pass).
   β: restore δ from ζ-slot; → ω (fail — the fence effect, prevents backtracking through).
   Loop-free single-shot leaf (like POS/TAB). x86 arm: ONE return, pure x86() concat, NO bb_bin_t,
   medium invisible. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "emit.h"
int bb_slot_claim(int bytes);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int sdoff() { return _.x86_scratch_off; }   /* saved_δ dword @ [r12+sdoff()] */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_pat_fence_str() {
    if (PLATFORM_X86) {
        return IF(MEDIUM_TEXT,
                   s_1asm(std::string(_.lbl_α) + ":")
                 + s_comment("# BOX FENCE()  [REG-3 δ=r14, ζ-frame saved_δ, x86() self-encoding]"))
             /* α: save δ to ζ-frame slot */
             + x86("mov", FR(sdoff()), "r14d")   /* mov dword ptr [r12+off], r14d  — save δ */
             + x86("jmp", PORT_GAMMA)             /* → γ (null match, succeeds)              */
             /* β: restore δ from ζ-frame slot, then fail (the fence effect) */
             + x86("def", PORT_BETA)
             + x86("mov", "r14d", FR(sdoff()))   /* mov r14d, dword ptr [r12+off]  — restore δ */
             + x86("jmp", PORT_OMEGA);            /* → ω (fence: fail on backtrack)             */
    }
    if (PLATFORM_JVM) {
        return jvm_class_hdr_str("fence")
             + jvm_init_ms_only_str("fence")
             + s_directive(".method public \316\261()Lbb/bb_box$Spec;")
             + s_directive(".limit stack 5")
             + s_directive(".limit locals 1")
             + s_1asm("aload_0")
             + s_2asm("getfield", "bb/bb_fence/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
             + s_1asm("iconst_0")
             + s_2asm("invokespecial", "bb/bb_box$Spec/<init>(II)V")
             + s_1asm("areturn")
             + s_directive(".end method")
             + s_directive(".method public \316\262()Lbb/bb_box$Spec;")
             + s_directive(".limit stack 1")
             + s_directive(".limit locals 1")
             + s_1asm("aconst_null")
             + s_1asm("areturn")
             + s_directive(".end method");
    }
    if (PLATFORM_JS) {
        return std::string("function make_pat_fence(ms) { let saved = 0; let self = { succ: null, fail: null,\n")
             + "\316\261() { saved = ms.delta; self.succ.\316\261(); return ''; },\n"
             + "\316\262() { ms.delta = saved; self.fail.\316\261(); return null; }\n}; return self; }\n";
    }
    if (PLATFORM_NET) {
        int sid = 0;
        return net_class_hdr_str(sid, _.nid)
             + net_ctor_none_str(sid, _.nid)
             + net_α_hdr_str()
             + s_2asm(".maxstack", "1")
             + net_cursor_load_str()
             + net_spec_zw_str()
             + s_1asm("ret")
             + s_1asm("}")
             + net_β_hdr_str()
             + s_2asm(".maxstack", "1")
             + net_fail_ret_str()
             + s_1asm("}")
             + s_1asm("}")
             + s_1asm(emit_fmt("    newobj     instance void pat_%d_%d::.ctor()", sid, _.nid));
    }
    if (PLATFORM_WASM) { return std::string("          (call $bb_fence_new)\n"); }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_pat_fence(void) {
    _.x86_scratch_off = bb_slot_claim(4);   /* saved_δ dword @ [r12+off] */
    bb_emit_x86(bb_pat_fence_str());
}
