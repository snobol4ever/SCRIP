/* bb_pat_pos.cpp — BB template for POS and RPOS. x86() self-encoding (template-revamp, 2026-06-01, Opus 4.8).
   POS(N) matches the empty string iff the cursor is at absolute position N; RPOS(N) iff the cursor is at N from
   the right, i.e. δ == Δ − N (SPITBOL Manual ch.18). RPOS is distinguished by sval[0]=='r' per lower_pat_dcg.c
   (TT_RPOS / XRPSI) — `ival != 0` is NOT authoritative (it misclassifies RPOS(0) and POS(N>0)). Both fail on β
   (an anchor matches at most once). REG-3 registers: cursor δ=R14d, length Δ=R15d (ratified, established by
   BB_MATCH α per REG-0) — the legacy [r10] cursor cell and the &Σlen movabs/rip bake are GONE (δ/Δ read straight
   from the callee-saved regs). x86 arm: ONE return, pure x86() concat, NO bb_bin_t, medium invisible. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline long posN()    { return (long)(int)_.op_ival; }
static inline int  is_rpos() { return _.op_sval && _.op_sval[0] == 'r'; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_pat_pos_str() {
    int nid = _.nid; (void)nid;
    if (PLATFORM_X86) {
        return IF(MEDIUM_TEXT,
                   s_1asm(std::string(_.lbl_α) + ":")
                 + s_comment(is_rpos() ? "# BOX RPOS()  [REG-3 δ=r14 Δ=r15, x86() self-encoding]"
                                       : "# BOX POS()  [REG-3 δ=r14, x86() self-encoding]"))
             + (is_rpos()
                  ? ( x86("mov", "ecx", "r15d")          /* ecx = Δ            */
                    + x86("sub", "ecx", posN())          /* ecx = Δ − N        */
                    + x86("cmp", "r14d", "ecx") )        /* δ vs (Δ − N)       */
                  : ( x86("cmp", "r14d", posN()) ))       /* δ vs N             */
             + x86("jne", PORT_OMEGA)
             + x86("jmp", PORT_GAMMA)
             + x86("def", PORT_BETA)
             + x86("jmp", PORT_OMEGA);
    }
    if (PLATFORM_JVM) {
        int rpos = is_rpos(); int sid = 0;
        const char *nm = rpos ? "rpos" : "pos";
        std::string tag_s = emit_fmt("%s_%d_%d", nm, sid, _.nid);
        std::string r = jvm_class_hdr_str(nm)
            + s_directive(".field private final n I")
            + s_directive(".field private final dyn Ljava/util/function/IntSupplier;")
            + jvm_init_ms_int_str(nm, "n") + jvm_val_helper_str(nm)
            + s_directive(".method public α()Lbb/bb_box$Spec;")
            + s_directive(".limit stack 5")
            + s_directive(".limit locals 1");
        if (rpos) {
            r += s_1asm("aload_0") + s_2asm("getfield", "bb/bb_rpos/ms Lbb/bb_box$MatchState;")
               + s_2asm("getfield", "bb/bb_box$MatchState/delta I") + s_1asm("aload_0")
               + s_2asm("getfield", "bb/bb_rpos/ms Lbb/bb_box$MatchState;")
               + s_2asm("getfield", "bb/bb_box$MatchState/ω I") + s_1asm("aload_0")
               + s_2asm("invokevirtual", "bb/bb_rpos/val()I") + s_1asm("isub")
               + s_1asm("    if_icmpne " + tag_s + "_ω");
        } else {
            r += s_1asm("aload_0") + s_2asm("getfield", "bb/bb_pos/ms Lbb/bb_box$MatchState;")
               + s_2asm("getfield", "bb/bb_box$MatchState/delta I") + s_1asm("aload_0")
               + s_2asm("invokevirtual", "bb/bb_pos/val()I")
               + s_1asm("    if_icmpne " + tag_s + "_ω");
        }
        r += s_2asm("new", "bb/bb_box$Spec") + s_1asm("dup") + s_1asm("aload_0")
           + s_1asm(emit_fmt("    getfield bb/bb_%s/ms Lbb/bb_box$MatchState;", nm))
           + s_2asm("getfield", "bb/bb_box$MatchState/delta I") + s_1asm("iconst_0")
           + s_2asm("invokespecial", "bb/bb_box$Spec/<init>(II)V") + s_1asm("areturn")
           + s_1asm(tag_s + "_ω:") + s_1asm("aconst_null") + s_1asm("areturn")
           + s_directive(".end method") + s_directive(".method public β()Lbb/bb_box$Spec;")
           + s_directive(".limit stack 1") + s_directive(".limit locals 1")
           + s_1asm("aconst_null") + s_1asm("areturn") + s_directive(".end method");
        return r;
    }
    if (PLATFORM_JS) {
        int rpos = is_rpos(); int64_t n = _.op_ival;
        return emit_fmt("function make_pat_%d_%d(ms) { const n = %ld; let self = { succ: null, fail: null,\n", (int)_.op_ival, _.nid, (long)n)
             + (rpos ? std::string("α() { if (ms.delta !== ms.omega - n) { self.fail.α(); return; } self.succ.α(); return ''; },\n")
                     : std::string("α() { if (ms.delta !== n) { self.fail.α(); return; } self.succ.α(); return ''; },\n"))
             + "β() { self.fail.α(); }\n}; return self; }\n";
    }
    if (PLATFORM_NET) {
        int rpos = is_rpos(); int sid = 0; int n = (int)_.op_ival;
        const char *lbl = rpos ? "RPOS" : "POS";
        std::string r = net_class_hdr_str(sid, _.nid)
            + s_directive(".field private int32 _n")
            + s_directive(".method public specialname rtspecialname instance void .ctor(int32 n) cil managed")
            + s_1asm("{") + s_2asm(".maxstack", "2")
            + s_1asm("ldarg.0") + s_2asm("call", "instance void [mscorlib]System.Object::.ctor()")
            + s_1asm("ldarg.0") + s_1asm("ldarg.1")
            + s_1asm(emit_fmt("    stfld      int32 pat_%d_%d::_n", sid, _.nid))
            + s_1asm("ret") + s_1asm("}")
            + net_α_hdr_str() + s_2asm(".maxstack", "3") + net_cursor_load_str();
        if (rpos) {
            r += s_1asm("ldarg.1") + net_ms_length_str() + s_1asm("ldarg.0")
               + s_1asm(emit_fmt("    ldfld      int32 pat_%d_%d::_n", sid, _.nid))
               + s_1asm("sub");
        } else {
            r += s_1asm("ldarg.0")
               + s_1asm(emit_fmt("    ldfld      int32 pat_%d_%d::_n", sid, _.nid));
        }
        r += s_1asm(emit_fmt("    bne.un     %s_%d_%d_FAIL", lbl, sid, _.nid))
          + net_cursor_load_str() + net_spec_zw_str() + s_1asm("ret")
          + s_1asm(emit_fmt("  %s_%d_%d_FAIL:", lbl, sid, _.nid)) + net_fail_ret_str() + s_1asm("}")
          + net_β_hdr_str() + s_2asm(".maxstack", "1") + net_fail_ret_str() + s_1asm("}")
          + s_1asm("}")
          + net_push_i4_str(n) + s_1asm(emit_fmt("    newobj     instance void pat_%d_%d::.ctor(int32)", sid, _.nid));
        return r;
    }
    if (PLATFORM_WASM) {
        return emit_fmt("          (call $bb_%s_new)\n", is_rpos() ? "rpos" : "pos");
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_pat_pos(void) { bb_emit_x86(bb_pat_pos_str()); }
