/* bb_pat_tab.cpp — BB template for TAB and RTAB. x86() self-encoding (template-revamp, 2026-06-02, Opus 4.8).
   TAB(N) matches forward to absolute cursor N; RTAB(N) matches to N chars from the right, i.e. target = Δ − N
   (SPITBOL Manual ch.18: "TAB(N) — matches up to and including the Nth character"; "RTAB(N) — matches up to
   but not including the character N positions from the end of the subject"). Per the mode-2 oracle (bb_exec.c
   IR_PAT_TAB): target = N (TAB) | Σlen − N (RTAB); the box FAILS if δ > target (cursor already past target —
   TAB/RTAB cannot match backward); on success δ advances to target and the box matches that span. On β both
   fail (an anchor-style cursor move matches at most once), restoring δ. RTAB is distinguished by sval[0]=='r'
   per lower_pat_dcg.c (TT_RTAB / XRTB) — `ival != 0` is NOT authoritative (it misclassifies RTAB(0)/TAB(N>0)).
   REG-3 registers: cursor δ=R14d, length Δ=R15d (ratified, established by BB_MATCH α per REG-0) — the legacy
   [r10] cursor cell and the &Σlen movabs/rip bake are GONE (δ read straight from r14d, Δ-len from r15d).
   x86 arm: ONE return, pure x86() concat, NO bb_bin_t, medium invisible. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline long tabN()    { return (long)(int)_.op_ival; }
static inline int  is_rtab() { return _.op_sval && _.op_sval[0] == 'r'; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_pat_tab_str() {
    int nid = _.nid; (void)nid;
    if (PLATFORM_X86) {
        return IF(MEDIUM_TEXT,
                   s_1asm(std::string(_.lbl_α) + ":")
                 + s_comment(is_rtab() ? "# BOX RTAB()  [REG-3 δ=r14 Δ=r15, x86() self-encoding]"
                                       : "# BOX TAB()  [REG-3 δ=r14, x86() self-encoding]"))
             + (is_rtab()
                  ? ( x86("mov",   "ecx", "r15d")          /* ecx = Δlen          */
                    + x86("sub",   "ecx", tabN())          /* ecx = Δlen − N      */
                    + x86("cmp",   "r14d", "ecx")          /* δ vs (Δlen − N)     */
                    + x86("jg",    PORT_OMEGA)             /* δ > target → fail   */
                    + x86("mov",   "r14d", "ecx") )        /* δ = target          */
                  : ( x86("cmp",   "r14d", tabN())         /* δ vs N              */
                    + x86("jg",    PORT_OMEGA)             /* δ > N → fail        */
                    + x86("mov32", "r14d", tabN()) ))      /* δ = N               */
             + x86("jmp",  PORT_GAMMA)
             + x86("def",  PORT_BETA)
             + x86("jmp",  PORT_OMEGA);
    }
    if (PLATFORM_JVM) {
        int rtab = is_rtab(); int sid = 0;
        const char *nm = rtab ? "rtab" : "tab";
        std::string tag_s = emit_fmt("%s_%d_%d", nm, sid, _.nid);
        std::string tag_fail = tag_s + "_ω";
        std::string result = jvm_class_hdr_str(nm)
                           + s_directive(".field private final n I")
                           + s_directive(".field private final dyn Ljava/util/function/IntSupplier;")
                           + s_directive(".field private advance I")
                           + jvm_init_ms_int_str(nm, "n")
                           + jvm_val_helper_str(nm)
                           + s_directive(".method public α()Lbb/bb_box$Spec;")
                           + s_directive(".limit stack 5")
                           + s_1asm(emit_fmt("    .limit locals %d", rtab ? 4 : 3));
        if (rtab) {
            result += s_1asm("aload_0")
                    + s_2asm("getfield", "bb/bb_rtab/ms Lbb/bb_box$MatchState;")
                    + s_2asm("getfield", "bb/bb_box$MatchState/ω I")
                    + s_1asm("aload_0")
                    + s_2asm("invokevirtual", "bb/bb_rtab/val()I")
                    + s_1asm("isub") + s_1asm("istore_1")
                    + s_1asm("aload_0")
                    + s_2asm("getfield", "bb/bb_rtab/ms Lbb/bb_box$MatchState;")
                    + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
                    + s_1asm("iload_1")
                    + s_1asm("    if_icmpgt " + tag_fail)
                    + s_1asm("iload_1") + s_1asm("aload_0")
                    + s_2asm("getfield", "bb/bb_rtab/ms Lbb/bb_box$MatchState;")
                    + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
                    + s_1asm("isub") + s_1asm("istore_2")
                    + s_1asm("aload_0") + s_1asm("iload_2")
                    + s_2asm("putfield", "bb/bb_rtab/advance I")
                    + s_1asm("aload_0")
                    + s_2asm("getfield", "bb/bb_rtab/ms Lbb/bb_box$MatchState;")
                    + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
                    + s_1asm("istore_3")
                    + s_1asm("aload_0")
                    + s_2asm("getfield", "bb/bb_rtab/ms Lbb/bb_box$MatchState;")
                    + s_1asm("iload_1")
                    + s_2asm("putfield", "bb/bb_box$MatchState/delta I")
                    + s_2asm("new", "bb/bb_box$Spec") + s_1asm("dup")
                    + s_1asm("iload_3") + s_1asm("iload_2")
                    + s_2asm("invokespecial", "bb/bb_box$Spec/<init>(II)V") + s_1asm("areturn")
                    + std::string(" ") + s_L1asm(tag_fail + ":", "")
                    + s_1asm("aconst_null") + s_1asm("areturn")
                    + s_directive(".end method")
                    + s_directive(".method public β()Lbb/bb_box$Spec;")
                    + s_directive(".limit stack 4") + s_directive(".limit locals 1")
                    + s_1asm("aload_0")
                    + s_2asm("getfield", "bb/bb_rtab/ms Lbb/bb_box$MatchState;")
                    + s_1asm("dup")
                    + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
                    + s_1asm("aload_0")
                    + s_2asm("getfield", "bb/bb_rtab/advance I")
                    + s_1asm("isub")
                    + s_2asm("putfield", "bb/bb_box$MatchState/delta I")
                    + s_1asm("aconst_null") + s_1asm("areturn")
                    + s_directive(".end method");
        } else {
            result += s_1asm("aload_0")
                    + s_2asm("getfield", "bb/bb_tab/ms Lbb/bb_box$MatchState;")
                    + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
                    + s_1asm("aload_0")
                    + s_2asm("invokevirtual", "bb/bb_tab/val()I")
                    + s_1asm("    if_icmpgt " + tag_fail)
                    + s_1asm("aload_0")
                    + s_2asm("invokevirtual", "bb/bb_tab/val()I")
                    + s_1asm("aload_0")
                    + s_2asm("getfield", "bb/bb_tab/ms Lbb/bb_box$MatchState;")
                    + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
                    + s_1asm("isub") + s_1asm("istore_1")
                    + s_1asm("aload_0") + s_1asm("iload_1")
                    + s_2asm("putfield", "bb/bb_tab/advance I")
                    + s_1asm("aload_0")
                    + s_2asm("getfield", "bb/bb_tab/ms Lbb/bb_box$MatchState;")
                    + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
                    + s_1asm("istore_2")
                    + s_1asm("aload_0")
                    + s_2asm("getfield", "bb/bb_tab/ms Lbb/bb_box$MatchState;")
                    + s_1asm("aload_0")
                    + s_2asm("invokevirtual", "bb/bb_tab/val()I")
                    + s_2asm("putfield", "bb/bb_box$MatchState/delta I")
                    + s_2asm("new", "bb/bb_box$Spec") + s_1asm("dup")
                    + s_1asm("iload_2") + s_1asm("iload_1")
                    + s_2asm("invokespecial", "bb/bb_box$Spec/<init>(II)V") + s_1asm("areturn")
                    + std::string(" ") + s_L1asm(tag_fail + ":", "")
                    + s_1asm("aconst_null") + s_1asm("areturn")
                    + s_directive(".end method")
                    + s_directive(".method public β()Lbb/bb_box$Spec;")
                    + s_directive(".limit stack 4") + s_directive(".limit locals 1")
                    + s_1asm("aload_0")
                    + s_2asm("getfield", "bb/bb_tab/ms Lbb/bb_box$MatchState;")
                    + s_1asm("dup")
                    + s_2asm("getfield", "bb/bb_box$MatchState/delta I")
                    + s_1asm("aload_0")
                    + s_2asm("getfield", "bb/bb_tab/advance I")
                    + s_1asm("isub")
                    + s_2asm("putfield", "bb/bb_box$MatchState/delta I")
                    + s_1asm("aconst_null") + s_1asm("areturn")
                    + s_directive(".end method");
        }
        return result;
    }
    if (PLATFORM_JS) {
        int rtab = is_rtab(); int64_t n = _.op_ival;
        std::string alpha = rtab
            ? emit_fmt("α() { const tgt = ms.omega - n; if (ms.delta > tgt) { self.fail.α(); return"
                "; } delta = tgt - ms.delta; const r = ms.sigma.slice(ms.delta, ms.delta + delta); ms.delta += delta; self.succ.α(); return r; },\n")
            : emit_fmt("α() { if (ms.delta > n || ms.delta > ms.omega) { self.fail.α(); return; } delta = n - ms.delta"
                "; if (ms.delta + delta > ms.omega) delta = ms.omega - ms.delta; const r = ms.sigma.slice(ms.delta, ms.delta + delta); ms.delta += delta; self.succ.α(); return r; },\n");
        return emit_fmt("function make_pat_%d_%d(ms) { const n = %ld; let delta = 0; let self = { succ: null, fail: null,\n", (int)_.op_ival, _.nid, (long)n)
             + alpha
             + "β() { ms.delta -= delta; self.fail.α(); }\n}; return self; }\n";
    }
    if (PLATFORM_NET) {
        int rtab = is_rtab(); int sid = 0; int n = (int)_.op_ival;
        const char *lbl = rtab ? "RTAB" : "TAB";
        std::string r = net_class_hdr_str(sid, _.nid)
            + s_directive(".field private int32 _n")
            + s_directive(".field private int32 _advance")
            + s_directive(".method public specialname rtspecialname instance void .ctor(int32 n) cil managed")
            + s_1asm("{") + s_2asm(".maxstack", "2")
            + s_1asm("ldarg.0") + s_2asm("call", "instance void [mscorlib]System.Object::.ctor()")
            + s_1asm("ldarg.0") + s_1asm("ldarg.1")
            + s_1asm(emit_fmt("    stfld      int32 pat_%d_%d::_n", sid, _.nid))
            + s_1asm("ret") + s_1asm("}");
        if (rtab) {
            r += net_α_hdr_str() + s_2asm(".maxstack", "4")
               + s_2asm(".locals", "init (int32 V_target, valuetype [boxes]Snobol4.Runtime.Boxes.Spec V_r)")
               + s_1asm("ldarg.1") + net_ms_length_str() + s_1asm("ldarg.0")
               + s_1asm(emit_fmt("    ldfld      int32 pat_%d_%d::_n", sid, _.nid))
               + s_1asm("sub") + s_1asm("stloc.0")
               + net_cursor_load_str() + s_1asm("ldloc.0")
               + s_1asm(emit_fmt("    bgt        %s_%d_%d_FAIL", lbl, sid, _.nid))
               + s_1asm("ldarg.0")
               + s_1asm("ldloc.0") + net_cursor_load_str() + s_1asm("sub")
               + s_1asm(emit_fmt("    stfld      int32 pat_%d_%d::_advance", sid, _.nid))
               + net_cursor_load_str() + s_1asm("ldarg.0")
               + s_1asm(emit_fmt("    ldfld      int32 pat_%d_%d::_advance", sid, _.nid))
               + net_spec_of_str() + s_1asm("stloc.1")
               + s_1asm("ldarg.1") + s_1asm("ldloc.0")
               + s_2asm("stfld", "int32 [boxes]Snobol4.Runtime.Boxes.MatchState::Cursor")
               + s_1asm("ldloc.1") + s_1asm("ret");
        } else {
            r += net_α_hdr_str() + s_2asm(".maxstack", "3")
               + s_2asm(".locals", "init (valuetype [boxes]Snobol4.Runtime.Boxes.Spec V_r)")
               + net_cursor_load_str() + s_1asm("ldarg.0")
               + s_1asm(emit_fmt("    ldfld      int32 pat_%d_%d::_n", sid, _.nid))
               + s_1asm(emit_fmt("    bgt        %s_%d_%d_FAIL", lbl, sid, _.nid))
               + s_1asm("ldarg.0") + s_1asm("ldarg.0")
               + s_1asm(emit_fmt("    ldfld      int32 pat_%d_%d::_n", sid, _.nid))
               + net_cursor_load_str() + s_1asm("sub")
               + s_1asm(emit_fmt("    stfld      int32 pat_%d_%d::_advance", sid, _.nid))
               + net_cursor_load_str() + s_1asm("ldarg.0")
               + s_1asm(emit_fmt("    ldfld      int32 pat_%d_%d::_advance", sid, _.nid))
               + net_spec_of_str() + s_1asm("stloc.0")
               + s_1asm("ldarg.1") + s_1asm("ldarg.0")
               + s_1asm(emit_fmt("    ldfld      int32 pat_%d_%d::_n", sid, _.nid))
               + s_2asm("stfld", "int32 [boxes]Snobol4.Runtime.Boxes.MatchState::Cursor")
               + s_1asm("ldloc.0") + s_1asm("ret");
        }
        r += s_1asm(emit_fmt("  %s_%d_%d_FAIL:", lbl, sid, _.nid)) + net_fail_ret_str() + s_1asm("}")
           + net_β_hdr_str() + s_2asm(".maxstack", "3")
           + s_1asm("ldarg.1") + s_1asm("ldarg.1") + net_cursor_load_str()
           + s_1asm("ldarg.0")
           + s_1asm(emit_fmt("    ldfld      int32 pat_%d_%d::_advance", sid, _.nid))
           + s_1asm("sub")
           + s_2asm("stfld", "int32 [boxes]Snobol4.Runtime.Boxes.MatchState::Cursor")
           + net_fail_ret_str() + s_1asm("}") + s_1asm("}")
           + net_push_i4_str(n)
           + s_1asm(emit_fmt("    newobj     instance void pat_%d_%d::.ctor(int32)", sid, _.nid));
        return r;
    }
    if (PLATFORM_WASM) {
        return emit_fmt("          (call $bb_%s_new)\n", is_rtab() ? "rtab" : "tab");
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_pat_tab(void) { bb_emit_x86(bb_pat_tab_str()); }
