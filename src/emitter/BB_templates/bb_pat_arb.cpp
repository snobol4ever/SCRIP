/* bb_pat_arb.cpp — BB template for ARB.  x86() self-encoding (template-revamp, 2026-06-02).
   ARB matches an arbitrary number of characters, shortest-first (SPITBOL Manual ch.18: "matches
   the shortest possible substring, including the null string … behaves like a spring, expanding as
   needed").  REG-3/4 registers: Σ=R13/δ=R14d/Δ=R15d/ζ=R12 (established by BB_MATCH α per REG-0).
   Two ζ-frame dwords: z (matched length) @ [r12+off], zo (origin δ) @ [r12+off+4] — claimed by
   bb_slot_claim(8), reached register-relative so BINARY==TEXT (no process-global deque + movabs,
   no rip-rel .data).
   α: z=0; zo=δ; → γ (offer 0-char match first).
   β: z++; if (zo+z > Δ) → ω; δ = zo+z; → γ (extend by one char each retry).
   No internal loop label — purely a two-entry generator (like POS/RPOS: β either succeeds or fails).
   x86 arm: ONE return, pure x86() concat, NO bb_bin_t, medium invisible. */
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
static inline int zoff()  { return _.x86_scratch_off; }
static inline int zooff() { return _.x86_scratch_off + 4; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_pat_arb_str() {
    if (PLATFORM_X86) {
        return IF(MEDIUM_TEXT,
                   s_1asm(std::string(_.lbl_α) + ":")
                 + s_comment("# BOX ARB()  [REG-4 Σ=r13 δ=r14 Δ=r15, ζ-frame z/zo, x86() self-encoding]"))
             /* α: z = 0 */
             + x86("mov",    FR(zoff()), (long)0)
             /* α: zo = δ (save origin) */
             + x86("mov",    "eax", "r14d")
             + x86("mov",    FR(zooff()), "eax")
             /* α: → γ (offer 0-char match) */
             + x86("jmp",    PORT_GAMMA)
             /* β: */
             + x86("def",    PORT_BETA)
             /* β: z++ */
             + x86("add",    FR(zoff()), (long)1)
             /* β: eax = zo + z */
             + x86("mov",    "eax", FR(zooff()))
             + x86("add",    "eax", FR(zoff()))
             /* β: if (zo+z > Δ) → ω */
             + x86("cmp",    "eax", "r15d")
             + x86("jg",     PORT_OMEGA)
             /* β: δ = zo+z */
             + x86("mov",    "r14d", "eax")
             /* β: → γ */
             + x86("jmp",    PORT_GAMMA);
    }
    if (PLATFORM_JVM) {
        return jvm_class_hdr_str("arb")
             + s_directive(".field private arb_count I")
             + s_directive(".field private arb_start I")
             + jvm_init_ms_only_str("arb")
             + s_directive(".method public \316\261()Lbb/bb_box$Spec;")
             + s_directive(".limit stack 5") + s_directive(".limit locals 1")
             + s_1asm("aload_0") + s_1asm("iconst_0")
             + s_2asm("putfield", "bb/bb_arb/arb_count I")
             + s_1asm("aload_0") + s_1asm("aload_0")
             + s_2asm("getfield", "bb/bb_arb/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
             + s_2asm("putfield", "bb/bb_arb/arb_start I")
             + s_2asm("new", "bb/bb_box$Spec") + s_1asm("dup") + s_1asm("aload_0")
             + s_2asm("getfield", "bb/bb_arb/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
             + s_1asm("iconst_0")
             + s_2asm("invokespecial", "bb/bb_box$Spec/<init>(II)V")
             + s_1asm("areturn") + s_directive(".end method")
             + s_directive(".method public \316\262()Lbb/bb_box$Spec;")
             + s_directive(".limit stack 5") + s_directive(".limit locals 1")
             + s_1asm("aload_0") + s_1asm("dup")
             + s_2asm("getfield", "bb/bb_arb/arb_count I")
             + s_1asm("iconst_1") + s_1asm("iadd")
             + s_2asm("putfield", "bb/bb_arb/arb_count I")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_arb/arb_start I")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_arb/arb_count I")
             + s_1asm("iadd") + s_1asm("aload_0")
             + s_2asm("getfield", "bb/bb_arb/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/\317\211 I")
             + s_1asm(std::string("    if_icmpgt arb_") + std::to_string(_.nid) + "_\317\211")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_arb/ms Lbb/bb_box$MatchState;")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_arb/arb_start I")
             + s_2asm("putfield", "bb/bb_box$MatchState/delta I")
             + s_2asm("new", "bb/bb_box$Spec") + s_1asm("dup") + s_1asm("aload_0")
             + s_2asm("getfield", "bb/bb_arb/ms Lbb/bb_box$MatchState;")
             + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_arb/arb_count I")
             + s_2asm("invokespecial", "bb/bb_box$Spec/<init>(II)V")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_arb/ms Lbb/bb_box$MatchState;")
             + s_1asm("dup") + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
             + s_1asm("aload_0") + s_2asm("getfield", "bb/bb_arb/arb_count I")
             + s_1asm("iadd") + s_2asm("putfield", "bb/bb_box$MatchState/delta I")
             + s_1asm("areturn")
             + s_1asm(std::string("arb_") + std::to_string(_.nid) + "_\317\211:")
             + s_1asm("aconst_null") + s_1asm("areturn")
             + s_directive(".end method");
    }
    if (PLATFORM_JS) {
        return emit_fmt("function make_pat_%d_%d(ms) { let delta = 0; let self = { succ: null, fail: null,\n", (int)_.op_ival, _.nid)
             + "α() { delta = ms.omega - ms.delta; const r = ms.sigma.slice(ms.delta, ms.delta + delta); ms.delta += delta; self.succ.α(); return r; },\n"
               "β() { if (delta <= 0) { self.fail.α(); return; } delta--; ms.delta--; const r = ms.sigma.slice(ms.delta, ms.delta + delta + 1); return r; }\n"
               "}; return self; }\n";
    }
    if (PLATFORM_NET) {
        int sid = 0;
        return net_class_hdr_str(sid, _.nid)
             + s_directive(".field private int32 _count")
             + s_directive(".field private int32 _start")
             + net_ctor_none_str(sid, _.nid)
             + net_α_hdr_str() + s_2asm(".maxstack", "2")
             + s_1asm("ldarg.0") + s_1asm("ldc.i4.0")
             + s_1asm(emit_fmt("    stfld      int32 pat_%d_%d::_count", sid, _.nid))
             + s_1asm("ldarg.0") + s_1asm("ldarg.1")
             + s_2asm("ldfld", "int32 [boxes]Snobol4.Runtime.Boxes.MatchState::Cursor")
             + s_1asm(emit_fmt("    stfld      int32 pat_%d_%d::_start", sid, _.nid))
             + net_cursor_load_str() + net_spec_zw_str() + s_1asm("ret")
             + s_1asm("}") + net_β_hdr_str() + s_2asm(".maxstack", "3")
             + s_1asm("ldarg.0") + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("    ldfld      int32 pat_%d_%d::_count", sid, _.nid))
             + s_1asm("ldc.i4.1") + s_1asm("add")
             + s_1asm(emit_fmt("    stfld      int32 pat_%d_%d::_count", sid, _.nid))
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("    ldfld      int32 pat_%d_%d::_start", sid, _.nid))
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("    ldfld      int32 pat_%d_%d::_count", sid, _.nid))
             + s_1asm("add") + s_1asm("ldarg.1") + net_ms_length_str()
             + s_1asm(emit_fmt("    bgt        ARB_%d_%d_FAIL", sid, _.nid))
             + s_1asm("ldarg.1") + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("    ldfld      int32 pat_%d_%d::_start", sid, _.nid))
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("    ldfld      int32 pat_%d_%d::_count", sid, _.nid))
             + s_1asm("add")
             + s_2asm("stfld", "int32 [boxes]Snobol4.Runtime.Boxes.MatchState::Cursor")
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("    ldfld      int32 pat_%d_%d::_start", sid, _.nid))
             + s_1asm("ldarg.0")
             + s_1asm(emit_fmt("    ldfld      int32 pat_%d_%d::_count", sid, _.nid))
             + net_spec_of_str() + s_1asm("ret")
             + s_1asm(emit_fmt("  ARB_%d_%d_FAIL:", sid, _.nid)) + net_fail_ret_str() + s_1asm("}")
             + s_1asm("}")
             + s_1asm(emit_fmt("    newobj     instance void pat_%d_%d::.ctor()", sid, _.nid));
    }
    if (PLATFORM_WASM) { return std::string("          (call $bb_arb_new)\n"); }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_pat_arb(void) {
    _.x86_scratch_off = bb_slot_claim(8);           /* z @ [r12+off], zo @ [r12+off+4] */
    bb_emit_x86(bb_pat_arb_str());
}
