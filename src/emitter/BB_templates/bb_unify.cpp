/* bb_unify.cpp — BB template for IR_UNIFY: Prolog unification (X = Y).  x86() self-encoding (template-revamp
   PL-RV-5, 2026-06-02, Opus 4.8).  Converts the SCALAR shapes; compound + float operands fall loud (deferred).
   pBB-FREE per the revamp FACT RULES: reads ONLY _ (g_emit).  All operand data is promoted at the single
   emit_core dispatch point by bb_prepare_pl (emit_bb.c): kinds _.bb_lk/_.bb_rk, ivals _.bb_li/_.bb_ri, and the
   interned .rodata labels _.bb_ls/_.bb_rs for ATOM operands (NULL otherwise).  _.bb_lk<0 is the missing-operand
   sentinel (vacuous success).  The four ports α/β/γ/ω come from _.lbl_*.  The runtime EFFECT/CONVERSION helpers
   on the KEEP side of PJ-RT-PURGE are reached through the x86() RO-call encoder (each returns plain 1/0 or a
   Term* — none makes a jump; the emitted x86 itself takes γ on success / ω on fail):
     · rt_pl_unify_const(slot,kind,ival,sval,dval) — WAM-CP-7 var-vs-const head match (one call replaces
       node_to_term×2 + unify_terms; reads/vivifies env[slot], scalar-compares a bound var or binds+trails).
     · rt_pl_node_to_term(kind,ival,sval,dval) + rt_pl_unify_terms(l,r) — the general scalar-scalar arm.
   THREE SCALAR SHAPES + ONE ELISION:
     (a) vacuous success — missing operand(s) (bb_exec.c IR_UNIFY fallthrough): α→γ, β→ω.
     (b) WAM-CP-7 self-unify — LOGICVAR(i)=LOGICVAR(i) hits unify()'s t1==t2 short-circuit (no bind, no trail);
         elided to a bare success jump (env[i] vivification is idempotent/deferred-not-lost), reuses (a)'s shape.
     (c) WAM-CP-7 var-const — one LOGICVAR slot + one atom/int const → single rt_pl_unify_const, then γ/ω tail.
     (d) general scalar-scalar — build L Term→rax, stash in rsp scratch, build R→rsi, reload L→rdi, unify_terms.
   DEFERRED (fall loud via x86_bomb — these need substrate not in this increment):
     · IR_STRUCT / IR_ARITH operands (compound term in unify position) — the PL-HY-1a compound-build substrate
       (delete the emit_build_compound_term twin walkers in favour of a serialized rt_pl_compound_build_n
       call); the corpus reaches these via head-argument unification.
     · IR_LIT_F operand — needs the xmm0 64-bit float-bit load path (no corpus rung unifies a var with a float
       literal directly; floats reach the runtime via is/2 arith in bb_builtin, not here).
   x86 only per Invariant #14 (JVM/JS/NET/WASM stubbed).  ONE return per PLATFORM_*, pure x86() concat, NO
   bb_bin_t, medium invisible (the dead MEDIUM_BINARY twin — which hand-counted rel32 via b.size() — is gone;
   Prolog's live unify path is mode-4 TEXT, mode-3 routes the oracle, so the encoder's BINARY arm never fires). */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
}
extern "C" void *rt_pl_node_to_term(int kind, long ival, const char *sval, double dval);
extern "C" int   rt_pl_unify_terms(void *l, void *r);
extern "C" int   rt_pl_unify_const(int slot, int kind, long ival, const char *sval, double dval);
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Parameterless accessors over _ (g_emit) — the sanctioned local-free, English-readable operand reads.       */
static inline int  u_present()        { return _.bb_lk >= 0; }
static inline int  u_const_kind(int k){ return k == IR_ATOM || k == IR_LIT_I; }
static inline int  u_deferred()       { int lk = _.bb_lk, rk = _.bb_rk;
                                        return lk == IR_STRUCT || rk == IR_STRUCT || lk == IR_ARITH || rk == IR_ARITH
                                            || lk == IR_LIT_F  || rk == IR_LIT_F; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* α-label + comment header (TEXT-only annotation, no byte form — the sanctioned carve-out).                   */
static inline std::string u_head(const char *msg) {
    return IF(MEDIUM_TEXT, s_1asm(std::string(_.lbl_α) + ":") + s_comment(msg));
}
/* Vacuous-success body (shapes a + b): α falls to γ; β tombstones to ω (a leaf has no retry).                */
static inline std::string u_vacuous(const char *msg) {
    return u_head(msg) + x86("jmp", PORT_GAMMA) + x86("def", PORT_BETA) + x86("jmp", PORT_OMEGA);
}
/* Port tail after a 1/0-returning unify call: test the result, je ω / jmp γ; β: jmp ω.                        */
static inline std::string u_tail() {
    return x86("test", "eax", "eax") + x86("je", PORT_OMEGA) + x86("jmp", PORT_GAMMA)
         + x86("def", PORT_BETA) + x86("jmp", PORT_OMEGA);
}
/* Build one SCALAR operand Term -> rax via rt_pl_node_to_term(kind, ival, sval-label, 0.0).  SysV: edi=kind   */
/* rsi=ival rdx=sval-label-ptr (0 if none) xmm0=dval (always 0 here — floats are deferred).                   */
static inline std::string u_build(int kind, long ival, const char *lbl) {
    return x86("mov", "edi", (long)kind)
         + x86("mov", "rsi", ival)
         + (lbl ? x86("lea", "rdx", "[rip + __]", (uint64_t)(uintptr_t)lbl, lbl) : x86("mov", "edx", (long)0))
         + x86("xorps", "xmm0", "xmm0")
         + x86("call", "rt_pl_node_to_term", (uint64_t)(uintptr_t)(void*)rt_pl_node_to_term);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_unify_str() {
    if (PLATFORM_X86) {
        if (!u_present())
            return u_vacuous("# BOX RESOLVE_UNIFY: missing children — vacuous success  [x86() self-encoding]");
        if (u_deferred())
            return x86_bomb("bb_unify: compound/float operand — deferred (PL-HY-1a compound substrate + float-unify)");
        int  lk = _.bb_lk, rk = _.bb_rk;
        long li = (long)_.bb_li, ri = (long)_.bb_ri;
        const char *ls = _.bb_ls, *rs = _.bb_rs;
        /* (b) self-unify x=x. */
        if (lk == IR_LOGICVAR && rk == IR_LOGICVAR && li == ri)
            return u_vacuous("# BOX RESOLVE_UNIFY (WAM-CP-7 self-unify x=x — vacuous success)  [x86() self-encoding]");
        /* (c) var-const. */
        {
            int slot = -1, ck = 0; long ci = 0; const char *clbl = (const char *)0;
            if (lk == IR_LOGICVAR && u_const_kind(rk))      { slot = (int)li; ck = rk; ci = ri; clbl = rs; }
            else if (rk == IR_LOGICVAR && u_const_kind(lk)) { slot = (int)ri; ck = lk; ci = li; clbl = ls; }
            if (slot >= 0)
                return u_head("# BOX RESOLVE_UNIFY (WAM-CP-7 var-const)  [x86() self-encoding]")
                     + x86("mov", "edi", (long)slot)
                     + x86("mov", "esi", (long)ck)
                     + x86("mov", "rdx", ci)
                     + (clbl ? x86("lea", "rcx", "[rip + __]", (uint64_t)(uintptr_t)clbl, clbl) : x86("mov", "ecx", (long)0))
                     + x86("xorps", "xmm0", "xmm0")
                     + x86("call", "rt_pl_unify_const", (uint64_t)(uintptr_t)(void*)rt_pl_unify_const)
                     + u_tail();
        }
        /* (d) general scalar-scalar: rsp scratch survives the second node_to_term call (box-local, not a value
           stack); 16-byte reservation keeps rsp 16-aligned across both calls exactly as the original arm did. */
        return u_head("# BOX RESOLVE_UNIFY  [x86() self-encoding]")
             + x86("sub", "rsp", (long)16)
             + u_build(lk, li, ls)
             + x86("mov", RSP(0), "rax")
             + u_build(rk, ri, rs)
             + x86("mov", "rsi", "rax")
             + x86("mov", "rdi", RSP(0))
             + x86("add", "rsp", (long)16)
             + x86("call", "rt_pl_unify_terms", (uint64_t)(uintptr_t)(void*)rt_pl_unify_terms)
             + u_tail();
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_unify(void) { bb_emit_x86(bb_unify_str()); }
