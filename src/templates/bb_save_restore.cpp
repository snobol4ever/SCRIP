#include <string>
#include <cstdint>
#include "emit.h"
extern "C" int bb_tiny_shim_ok(const char *, int);   /* s59 ONE-AUTHORITY: shim emits iff the shared site predicate (bb_call_proc_staged.cpp) says so — a jmp <fn>_alpha can never dangle */
static int fnrbp(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_FN_RBP"); v = e ? atoi(e) : 2; if (v == 1) v = 2; if (v < 0 || v > 2) v = 2; } return v; }   /* ⭐⭐⭐ S-2 (RBP-EARN s78, Lon in-chat: "remove RBP's used by FUNCTION linkage"): DEFAULT FLIPPED 1 -> 2 and the RBP-FUNCTION arm RETIRED -- `=1` now ALIASES to 2 so an inherited env or script cannot resurrect a deleted regime.  THE GATE s63/s64 SET WAS "when the =2 board == the =1 board, flip and delete", and S-1 (FLOATER-CUT, this session) is what closed it: the =2 arm was 11 programs short ONLY because every DEFINE-bearing statement leaked its goto-out depth and the RBP arm's `mov rsp,rbp` was silently absorbing it -- measured 136 -> 146 PASS on the 165-program two-arm sweep once the statement released its own K.  RBP's remaining job is the MATCH INTERIOR alone (ARBNO zeta-FRAMEs + the mrbp match frame); FUNCTION linkage now rides the RSP depth-invariance law exactly as the s64 writer/reader pair always claimed it could.  `=0` (the s58 BOMB descent instrument) is UNTOUCHED and still reachable. */
static int fnsig(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_FN_SIG"); v = (e && *e == '0') ? 0 : 1; } return v; }   /* SIG s66: MUST agree with bcps_fnsig (bb_call_proc_staged.cpp) — same env read, same default, two coherent worlds never mixed */   /* s63/s64 FUNCTION-linkage arms: 0 = s58 BOMB floaters (descent instrument) · 1 = RBP bracket (s63, proven) · 2 = RSP-ONLY (s64, Lon's challenge): the shim pushes the 16B {γ,ω} pair at TOS and the floaters find it by DEPTH-INVARIANCE — no anchor register; sound iff every statement boundary in the body is depth-neutral, so under =2 a leaking statement shape pops junk as a code address and dies LOUD at the jmp: the failure list IS the leak census, program-granular.  One predicate gates writer + readers, drift-proof. */
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "ab_abi.h"
#include "pin_va.h"
long  rt_proc_call_open_slim(const char *name, int np, int nargs);
int   bb_scc_probe(const char *fname, int nargs, int *np_out, int *nsave_out, int *gk_out, int *res_gk_out);
int   bb_scc_handoff_arm(void);
int   bb_scc_handoff_room(void);
int   rt_proc_nformals(const char *);
void  bb_scc_handoff_pending_clear(void);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* IR_SAVE_RESTORE — SN4-FLAT-PROC (s176) functional linkage family.  SPITBOL manual Ch.8: a DEFINE'd function saves the fname variable, formals, and locals on a pushdown stack at entry and restores
 * them when the body transfers to the reserved labels RETURN (yield fname's value), FRETURN (signal failure), or NRETURN (Ch.9: yield the NAME held in fname).  In the flat regime the body statements
 * live ONLY in the one main chain, so the return labels are program-wide FLOATER boxes and the save/restore protocol rides the pcall record; roles discriminate on op_ival:
 *   3 WIRE-ADOPT — first box of a DEFINE stub blob, runs right after the jmp-entry prologue: ___ = this activation's base, the prologue parked outside-γ at [___+kt-24], outside-ω at [___+kt-16],
 *     caller ___ at [___+kt-8] (xa_flat header contract), and blob-entry rsp = ___+kt (pre-carve).  Marshal all four into rt_flat_wire_adopt, which writes the OPEN pcall record's wire quad — the way
 *     home from any depth.  γ continues to the IR_GOTO_DEFERRED entry transfer.
 *   1 RETURN floater / 2 FRETURN floater — reached as ordinary labels through the registry (any goto, incl. $-computed).  rt_flat_ret_snap PEEKS (never pops) the open record's wire quad into a static
 *     buffer {γ-wire@0, ω-wire@8, rsp@16, ___@24} and returns its address; the tail restores ___/rsp and jmps the port's wire with rax:rdx riding untouched, landing exactly on the wire the opener
 *     installed (rt_proc_enter's landings on the C path) — the landing's epilogue leaf then performs the pop + name-restore + result protocol VERBATIM, so semantics are byte-identical to the old
 *     extracted-body exit.  Discarded intermediate C frames (rt_chain_enter / rt_goto_transfer) hold only callee-saved pins that are global invariants in emitted code, so the wholesale rsp restore is
 *     sound.  Level-0 transfer and wire-less activations die loudly inside the leaf.
 *   0 site OPEN — reserved for the emitted flat call site (next slice); bombs if reached. */
std::string bb_save_restore() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    long role = (long)_.op_ival;
    if (role == 3) {
        /* ⛔⭐⭐⭐ LON RULING s54 (in-chat, verbatim substance): "delete all of that global variables. GONE. DONE.
         * We do not do that here. We have registers R10 and R11 for linkage, and plan on using the stack to
         * store things, not global variables."  The rt_flat_wire_adopt call — which banked the wire quad into
         * the g_pcall global array — is DELETED.  Wires ride rΓ=r10 · rΩ=r11 (LADDER WREG assignment, locked
         * s12).  Entry-rsp banking moves to the SPINE (stack-resident record), not a parallel global — that
         * half is the next seat's WREG-3 shape; until it lands, RETURN-from-interior-growth is KNOWN BROKEN
         * (Lon: "worry not about breakage").  git revert is the undo. */
        /* ⭐ FUNCTION LINKAGE s55 (Lon in-chat): "Can you not use R10 and R11 for success and fail return address
         * for FUNCTION linkage?  Just like any BB BLOB interface."  YES — the call sites now speak r10/r11 DIRECTLY
         * (bb_glue_pass_wires_blob), so the rcx/rdx→r10/r11 adopt hop is DELETED.  This box emits nothing: the wire
         * pair arrives already seated.  rcx/rdx remain the chain contract (rt_chain_enter, EVAL/CODE) only. */
        /* ⭐⭐⭐ SHIM DELETED s58 (Lon in-chat: "Remove stupid TEST_shim and make it real. Do not use g_call_args, instead push on the stack via RSP.").  The <FN>_shim and its g_call_args transport
         * are GONE: tiny sites (bb_call_proc_staged TINY-REAL arm) now carry the whole going-in protocol themselves — save-set spill to an RSP pushdown block, direct actual→formal GVA install,
         * locals/result NULVCL, wires, one jmp to the body α.  This stub blob remains only for the goto-transfer customers (EVAL/CODE/$X) flowing α→γ into IR_GOTO_DEFERRED. */
        return x86("comment", "IR_SAVE_RESTORE wire-adopt (s58: EMPTY — shim deleted, tiny sites carry the pushdown protocol; s55: sites set r10/r11 directly)")
             + x86_alpha()
             + x86_gamma();
    }
    if (role == 4) {
        /* ⭐⭐⭐ TINY-REAL s58 SHIM (Lon in-chat: "Remove stupid TEST_shim and make it real... push on the stack via RSP"; "the SHIM... swaps all the globals with actuals, and save the globals in those
         * same stack locations"; "push val, push val, then push 2 if two args are passed. And the shim handles the 4 parameters from the 2 it was given and extends the stack in place").  Emitted ONCE
         * per DEFINE, AT the DEFINE statement (role-4 IR_SAVE_RESTORE spliced by the lowerer; unreachable inline — the statement's goto skips it; entered only by name).  THE PROTOCOL — site pushes
         * {K}{succ,fail conts}{actual_i at [32+i*16]} and jmps <fn>_alpha with r10/r11 UNTOUCHED (they are the ENCLOSING activation's return ports and ride the record).  alpha: read K, extend the
         * block in place to full arity (max carve + give back K*16 — no shl encoder needed), spill+null locals/result into its own tail, bank the enclosing wires + a K copy, then per formal i ONE
         * cmp/jbe: supplied ⇒ SWAP actual↔old-global in the site's own slot (manual Ch.8 p.104 pushdown), missing ⇒ old-global into its extension slot + formal←null; set r10=<fn>_beta r11=<fn>_omega
         * (the floaters stay bare jmp r10/r11) and jmp the body α.  beta: capture the result from the result-name cell BEFORE restoring, mirror-restore every cell, re-establish the enclosing wires
         * (recursion is LIFO by construction — this is what kills the measured return-ladder), fetch the site cont, release the CONSTANT frame (T+32+nf*16, since D+K=nf), jmp cont.  omega: same with
         * FAILDESCR.  Record from alpha's post-carve rsp: [0..extra*16) locals+result olds | [extra*16]={r10,r11} | [+16]={K} || [T..T+D*16) missing-formal olds | {K}{conts}{K supplied actuals}.
         * Addressing base r8 = extension base (= site rsp − nf*16): missing_i=[r8+i*16] (valid iff i>=K), count=[r8+nf*16], conts=[+16,+24], supplied_i=[r8+nf*16+32+i*16] — every disp positive+static.
         * Over-arity (K>nf, manual: extras evaluated+ignored) stays on the classic arm this rung — owed.  TEXT-only this seat (m3 cross-chain body-α, same owed class as fold slice-2).  Hatch
         * SCRIP_NO_TINY=1 empties this box (sites fall to slim which never jmps here). */
        const char * fn4 = _.op_sval; const char * en4 = _.lbl_t0 ? _.lbl_t0 : fn4;
        int np4 = 0, ns4 = 0, rg4 = -1; int gk4[64];
        int ok4 = (fn4 && en4 && bb_tiny_shim_ok(fn4, 0)) ? bb_scc_probe(fn4, 0, &np4, &ns4, gk4, &rg4) : 0;   /* R-1 s94 (Fable 5): BOTH MEDIA -- the MEDIUM_TEXT conjunct is lifted; the shim's faces are same-chain extlbl, its body transfer rides the body$<ENTRY> cell (x86_jmp_via_cell) */
        int nf4 = ok4 ? rt_proc_nformals(fn4) : 0;
        if (!(ok4 && nf4 >= 0 && nf4 <= np4 && nf4 <= 29)) return   /* nf<=29: L-id budget (one-byte ids, [0,250)); wider DEFINEs decline to slim */ x86("comment", "IR_SAVE_RESTORE role 4: shim declined (hatch, non-TEXT, or probe/formals shape) — sites fall to the slim arm") + x86_alpha() + x86_gamma();
        int xt4 = ns4 - nf4;   /* extra = locals + unshadowed result name (probe layout: gk[0..np)=formals then locals, gk[np..ns)=result iff unshadowed) */
        long T4 = 16L * xt4 + 32;   /* tail: extras' olds + {r10,r11} + {K,spare} */
        int rgx = rg4 < 0 ? 0 : rg4;
        auto GQ = [&](int gk, int w) { return (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(gk, w) : ABSQ(RT_GVA_VA + (unsigned long)gk * 16 + (unsigned long)w); };
        auto R8Q = [&](long d) { return std::string("[r8 + ") + std::to_string(d) + "]"; };
        std::string la = std::string(fn4) + "_alpha", lb = std::string(fn4) + "_gamma", lo = std::string(fn4) + "_omega";   /* s58 Lon: the RETURN landing is the activation-blob GAMMA (success exit protocol), not a backtrack — named accordingly */
        std::string blb = std::string(en4) + "_body";   /* BARE-CHAIN (Lon s62): the body chain's one label — proc_LBL__<FN>_α wrapper is gone */
        const struct bb_label_t * lbl_b = emit_label_intern(lb.c_str()); const struct bb_label_t * lbl_o = emit_label_intern(lo.c_str());   /* R-1 s94: the gamma/omega faces interned ONCE -- lea (extlbl) and def share the object, both media */
        uint64_t body_cell = (uint64_t)(uintptr_t)bb_ab_fn_cell_ptr((std::string("body$") + en4).c_str());   /* R-1 s94: filled by the m3 driver after main seals (LBL__ registration twin); TEXT never reads it */
        auto SCALE16 = [&]() { return x86("mov", "rax", "rcx") + x86("add", "rax", "rax") + x86("add", "rax", "rax") + x86("add", "rax", "rax") + x86("add", "rax", "rax"); };   /* rax = K*16, no shl encoder */
        /* s63 fnrbp: SCRIP_FN_RBP=0 restores the s58 BOMB floaters (the descent instrument) and the frameless shim tail — one predicate, both halves of the bracket, drift-proof */
        auto RESTORE4 = [&](int lid) {   /* shared beta/omega restore body: needs rsp at tail base; leaves rcx=K r8=ext-base; clobbers rax; preserves rdi/rsi; lid = per-instantiation label-id base (beta/omega emit this twice in ONE box — ids must not collide) */
            return x86_rsp_load64("rcx", (int)(16 * xt4 + 16))
                 + SCALE16()
                 + x86("cmp", "rcx", (long)nf4) + x86_jcc_id("jbe", lid - 5) + x86("mov32", "eax", 16L * nf4) + x86_deflabel_id(lid - 5)   /* r8 = site_rsp - nf*16 in ALL arities: clamp like alpha */
                 + x86("lea", "r8", std::string("[rsp + ") + std::to_string(T4) + "]") + x86("sub", "r8", "rax")
                 + FOR(0, xt4, [&](int j) { int k = xt4 - 1 - j;
                       return x86_rsp_load64("rax", 16 * k) + x86("note", gva_name(gk4[nf4 + k])) + x86("mov", GQ(gk4[nf4 + k], 0), "rax")
                            + x86_rsp_load64("rax", 16 * k + 8) + x86("mov", GQ(gk4[nf4 + k], 8), "rax"); })
                 + FOR(0, nf4, [&](int i) {
                       return x86("cmp", "rcx", (long)i) + x86_jcc_id("jbe", lid + i)
                            + x86("mov", "rax", R8Q(16L * nf4 + 32 + 16L * i)) + x86("note", gva_name(gk4[i])) + x86("mov", GQ(gk4[i], 0), "rax")
                            + x86("mov", "rax", R8Q(16L * nf4 + 32 + 16L * i + 8)) + x86("mov", GQ(gk4[i], 8), "rax")
                            + x86_jmp_id(lid + 30 + i)
                            + x86_deflabel_id(lid + i)
                            + x86("mov", "rax", R8Q(16L * i)) + x86("note", gva_name(gk4[i])) + x86("mov", GQ(gk4[i], 0), "rax")
                            + x86("mov", "rax", R8Q(16L * i + 8)) + x86("mov", GQ(gk4[i], 8), "rax")
                            + x86_deflabel_id(lid + 30 + i); })
                 + x86_rsp_load64("r10", 16 * xt4) + x86_rsp_load64("r11", 16 * xt4 + 8); };
        if (fnsig()) {
            /* ⭐⭐⭐ SIG s66 SHIM (Lon in-chat: the site publishes "a STATIC MAP of how to reach them all" and the shim "can REACH DOWN the stack himself and avoid the middle man pushes").  Protocol:
             * site sets rcx = &sig {[0]=K, [8]=γcont, [16]=ωcont, [24+8i]=off_i entry-rsp-relative} and jmps here; the actuals never moved — they sit in the CALLER'S OWN operand cells at [entry+off_i],
             * and entry rsp IS the reference point (= rsp at the site's jmp = this frame's base + F4).  WHAT DIES vs the s58 record shim: the K-dependent max-carve + clamp + give-back (was recomputed on
             * ALL THREE faces α/γ/ω), the dual r8 derivations, and the two-arm release constant — the frame is a per-DEFINE CONSTANT F4 = T4 + nf*16 (extras+wires+sigbank+pad, then nf extension slots
             * used only for missing formals), and the release is one immediate.  The swap stays the manual-Ch.8 pushdown-by-swap, relocated: supplied formal i ⇒ SWAP [entry+off_i] ↔ formal-GVA (old
             * formal parks in the caller's own cell, restored one-way at γ/ω exactly as the record copy was); missing ⇒ old formal into extension slot [rsp+T4+16i], formal←null.  Over-arity: i≥nf sig
             * entries are simply never read; the extras stay caller-resident and statement_end releases them with every other operand cell.  Recursion is LIFO by construction (each activation swaps its
             * OWN caller's cells and restores them before that caller resumes).  Registers: rcx=sig (banked at [16*xt4+16], replacing the K bank), rdx=K, r8=entry rsp, rdi/rsi swap scratch in α (r11
             * scratch in γ/ω before its reload); result rides rdi:rsi across the γ restore as before.  SCRIP_FN_SIG=0 restores the s58 record shim below, byte-identical. */
            long F4 = T4 + 16L * nf4;
            auto SIGQ = [&](long d) { return std::string("[rcx + ") + std::to_string(d) + "]"; };
            auto EXTQ = [&](long d) { return std::string("[rsp + ") + std::to_string(T4 + d) + "]"; };
            auto R8AT = [&]() { return x86("lea", "r8", std::string("[rsp + ") + std::to_string(F4) + "]"); };
            auto FRESTORE = [&](int lid) {   /* shared γ/ω: sig from bank, K, r8=entry, extras reversed, formals via sig offsets (supplied) or extension (missing); leaves rcx=sig; r11 scratch pre-reload */
                return x86_rsp_load64("rcx", (int)(16 * xt4 + 16))
                     + x86("mov", "rdx", SIGQ(0))
                     + R8AT()
                     + FOR(0, xt4, [&](int j) { int k = xt4 - 1 - j;
                           return x86_rsp_load64("rax", 16 * k) + x86("note", gva_name(gk4[nf4 + k])) + x86("mov", GQ(gk4[nf4 + k], 0), "rax")
                                + x86_rsp_load64("rax", 16 * k + 8) + x86("mov", GQ(gk4[nf4 + k], 8), "rax"); })
                     + FOR(0, nf4, [&](int i) {
                           return x86("cmp", "rdx", (long)i) + x86_jcc_id("jbe", lid + i)
                                + x86("mov", "r11", SIGQ(24 + 8L * i)) + x86("add", "r11", "r8")
                                + x86("mov", "rax", "[r11 + 0]") + x86("note", gva_name(gk4[i])) + x86("mov", GQ(gk4[i], 0), "rax")
                                + x86("mov", "rax", "[r11 + 8]") + x86("mov", GQ(gk4[i], 8), "rax")
                                + x86_jmp_id(lid + 30 + i)
                                + x86_deflabel_id(lid + i)
                                + x86("mov", "rax", EXTQ(16L * i).c_str()) + x86("note", gva_name(gk4[i])) + x86("mov", GQ(gk4[i], 0), "rax")
                                + x86("mov", "rax", EXTQ(16L * i + 8).c_str()) + x86("mov", GQ(gk4[i], 8), "rax")
                                + x86_deflabel_id(lid + 30 + i); })
                     + x86_rsp_load64("r10", 16 * xt4) + x86_rsp_load64("r11", 16 * xt4 + 8); };
            return x86("comment", "IR_SAVE_RESTORE role 4: SIG s66 per-DEFINE shim (alpha=swap-by-map, gamma/omega=restore-by-map, CONSTANT frame)")
                 + x86_alpha()
                 + x86_def_ext(emit_label_intern(la.c_str()))
                 + x86("sub", "rsp", F4)
                 + FOR(0, xt4, [&](int k) {
                       return x86("note", gva_name(gk4[nf4 + k])) + x86("mov", "rax", GQ(gk4[nf4 + k], 0)) + x86_rsp_store64(16 * k, "rax")
                            + x86("mov", "rax", GQ(gk4[nf4 + k], 8)) + x86_rsp_store64(16 * k + 8, "rax")
                            + x86("mov", GQ(gk4[nf4 + k], 0), (long)DT_SNUL) + x86("mov", GQ(gk4[nf4 + k], 8), (long)0); })
                 + x86_rsp_store64(16 * xt4, "r10") + x86_rsp_store64(16 * xt4 + 8, "r11") + x86_rsp_store64(16 * xt4 + 16, "rcx")
                 + x86("mov", "rdx", SIGQ(0))
                 + R8AT()
                 + FOR(0, nf4, [&](int i) {
                       return x86("cmp", "rdx", (long)i) + x86_jcc_id("jbe", 10 + i)
                            + x86("mov", "rdi", SIGQ(24 + 8L * i)) + x86("add", "rdi", "r8")
                            + x86("mov", "rax", "[rdi + 0]") + x86("note", gva_name(gk4[i])) + x86("mov", "rsi", GQ(gk4[i], 0)) + x86("mov", GQ(gk4[i], 0), "rax") + x86("mov", "[rdi + 0]", "rsi")
                            + x86("mov", "rax", "[rdi + 8]") + x86("mov", "rsi", GQ(gk4[i], 8)) + x86("mov", GQ(gk4[i], 8), "rax") + x86("mov", "[rdi + 8]", "rsi")
                            + x86_jmp_id(41 + i)
                            + x86_deflabel_id(10 + i)
                            + x86("note", gva_name(gk4[i])) + x86("mov", "rax", GQ(gk4[i], 0)) + x86("mov", EXTQ(16L * i).c_str(), "rax")
                            + x86("mov", "rax", GQ(gk4[i], 8)) + x86("mov", EXTQ(16L * i + 8).c_str(), "rax")
                            + x86("mov", GQ(gk4[i], 0), (long)DT_SNUL) + x86("mov", GQ(gk4[i], 8), (long)0)
                            + x86_deflabel_id(41 + i); })
                 + x86("lea", "r10", "extlbl", (uint64_t)(uintptr_t)lbl_b)
                 + x86("lea", "r11", "extlbl", (uint64_t)(uintptr_t)lbl_o)
                 + IF(fnrbp() == 1, x86("comment", "s63 RBP-FUNCTION WRITER (see the s58 arm's full comment — unchanged under SIG)")
                             + x86("sub", "rsp", (long)8)
                             + x86("push", "r11")
                             + x86("push", "r10")
                             + x86("push", "rbp")
                             + x86("mov", "rbp", "rsp"))
                 + IF(fnrbp() == 2, x86("comment", "s64 RSP-ONLY WRITER (see the s58 arm's full comment — unchanged under SIG)")
                             + x86("push", "r11")
                             + x86("push", "r10"))
                 + x86("jmp", "[rip@cell + __]", body_cell, blb.c_str())
                 + x86_def_ext(lbl_b)
                 + x86("note", gva_name(rgx)) + x86("mov", "rdi", GQ(rgx, 0)) + x86("mov", "rsi", GQ(rgx, 8))
                 + FRESTORE(80)
                 + x86("mov", "rcx", SIGQ(8))
                 + x86("add", "rsp", F4)
                 + x86("mov", "rax", "rdi") + x86("mov", "rdx", "rsi")
                 + x86("jmp", "rcx")
                 + x86_def_ext(lbl_o)
                 + FRESTORE(150)
                 + x86("mov", "rcx", SIGQ(16))
                 + x86("add", "rsp", F4)
                 + x86("mov32", "eax", (long)DT_FAIL) + x86("xor", "edx", "edx")
                 + x86("jmp", "rcx")
                 + x86_gamma();
        }
        return x86("comment", "IR_SAVE_RESTORE role 4: TINY-REAL s58 per-DEFINE shim (alpha=swap/extend, beta/omega=restore)")
             + x86_alpha()
             + x86_def_ext(emit_label_intern(la.c_str()))
             + x86_rsp_load64("rcx", 0)
             + x86("mov", "r8", "rsp") + x86("sub", "r8", 16L * nf4)
             + SCALE16()
             + x86("cmp", "rcx", (long)nf4) + x86_jcc_id("jbe", 2) + x86("mov32", "eax", 16L * nf4) + x86_deflabel_id(2)   /* over-arity (K>nf, manual: extras evaluated+ignored): no extension exists — clamp the give-back to nf*16 */
             + x86("sub", "rsp", T4 + 16L * nf4)
             + x86("add", "rsp", "rax")
             + FOR(0, xt4, [&](int k) {
                   return x86("note", gva_name(gk4[nf4 + k])) + x86("mov", "rax", GQ(gk4[nf4 + k], 0)) + x86_rsp_store64(16 * k, "rax")
                        + x86("mov", "rax", GQ(gk4[nf4 + k], 8)) + x86_rsp_store64(16 * k + 8, "rax")
                        + x86("mov", GQ(gk4[nf4 + k], 0), (long)DT_SNUL) + x86("mov", GQ(gk4[nf4 + k], 8), (long)0); })
             + x86_rsp_store64(16 * xt4, "r10") + x86_rsp_store64(16 * xt4 + 8, "r11") + x86_rsp_store64(16 * xt4 + 16, "rcx")
             + FOR(0, nf4, [&](int i) {
                   return x86("cmp", "rcx", (long)i) + x86_jcc_id("jbe", 10 + i)
                        + x86("mov", "rax", R8Q(16L * nf4 + 32 + 16L * i)) + x86("note", gva_name(gk4[i])) + x86("mov", "rdx", GQ(gk4[i], 0)) + x86("mov", GQ(gk4[i], 0), "rax") + x86("mov", R8Q(16L * nf4 + 32 + 16L * i).c_str(), "rdx")
                        + x86("mov", "rax", R8Q(16L * nf4 + 32 + 16L * i + 8)) + x86("mov", "rdx", GQ(gk4[i], 8)) + x86("mov", GQ(gk4[i], 8), "rax") + x86("mov", R8Q(16L * nf4 + 32 + 16L * i + 8).c_str(), "rdx")
                        + x86_jmp_id(41 + i)
                        + x86_deflabel_id(10 + i)
                        + x86("note", gva_name(gk4[i])) + x86("mov", "rax", GQ(gk4[i], 0)) + x86("mov", R8Q(16L * i).c_str(), "rax")
                        + x86("mov", "rax", GQ(gk4[i], 8)) + x86("mov", R8Q(16L * i + 8).c_str(), "rax")
                        + x86("mov", GQ(gk4[i], 0), (long)DT_SNUL) + x86("mov", GQ(gk4[i], 8), (long)0)
                        + x86_deflabel_id(41 + i); })
             + x86("lea", "r10", "extlbl", (uint64_t)(uintptr_t)lbl_b)
             + x86("lea", "r11", "extlbl", (uint64_t)(uintptr_t)lbl_o)
             + IF(fnrbp() == 1, x86("comment", "s63 RBP-FUNCTION WRITER (Lon: ONE frame, FUNCTION linkage): pin the return point.  Frame [rbp+0]=enclosing rbp (recursion chain, LIFO)  [rbp+8]=gamma  [rbp+16]=omega  [rbp+24]=pad (reserved: RESULT base).  32B keeps C-call 16-alignment parity for the whole body.  rbp is SysV callee-saved: survives every C crossing; the ONLY other rbp writers product-wide are nested instances of this same bracket.  Readers: the RETURN/FRETURN/NRETURN floaters — mov rsp,rbp restores the UNKNOWN body depth to alpha-end depth P, pops the frame, jmps the banked landing; gamma/omega are pure functions of rsp==P (RESTORE4 rederives rcx=K and r8 from rsp).  Interior carves stay SILENT-NO-WHACK: nothing between here and the floater releases anything, the frame pop discards it wholesale.")
                         + x86("sub", "rsp", (long)8)
                         + x86("push", "r11")
                         + x86("push", "r10")
                         + x86("push", "rbp")
                         + x86("mov", "rbp", "rsp"))
             + IF(fnrbp() == 2, x86("comment", "s64 RSP-ONLY WRITER (Lon challenge: zero RBP): push the 16B {gamma,omega} pair at TOS — [rsp+0]=gamma [rsp+8]=omega, body entered at P-16 (16-parity kept).  NO anchor register: the floaters find the pair by the DEPTH-INVARIANCE LAW — control transfers only at depth-neutral statement boundaries; MATCH banks its own mark in the r12 arena; the alpha-sub/omega-add pairing releases statement temporaries.  A statement shape that leaks (the s58 -16 census class) breaks the law and dies loud at the floater's jmp — under this arm the red set IS the leak census.")
                         + x86("push", "r11")
                         + x86("push", "r10"))
             + x86("jmp", "[rip@cell + __]", body_cell, blb.c_str())
             + x86_def_ext(lbl_b)
             + x86("note", gva_name(rgx)) + x86("mov", "rdi", GQ(rgx, 0)) + x86("mov", "rsi", GQ(rgx, 8))
             + RESTORE4(80)
             + x86("mov32", "eax", T4 + 32 + 16L * nf4)
             + x86("cmp", "rcx", (long)nf4) + x86_jcc_id("jbe", 3) + SCALE16() + x86("add", "rax", T4 + 32) + x86_deflabel_id(3)   /* over-arity release: T+32+K*16 */
             + x86("mov", "rcx", R8Q(16L * nf4 + 16))
             + x86("add", "rsp", "rax")
             + x86("mov", "rax", "rdi") + x86("mov", "rdx", "rsi")
             + x86("jmp", "rcx")
             + x86_def_ext(lbl_o)
             + RESTORE4(150)
             + x86("mov32", "eax", T4 + 32 + 16L * nf4)
             + x86("cmp", "rcx", (long)nf4) + x86_jcc_id("jbe", 4) + SCALE16() + x86("add", "rax", T4 + 32) + x86_deflabel_id(4)
             + x86("mov", "rcx", R8Q(16L * nf4 + 24))
             + x86("add", "rsp", "rax")
             + x86("mov32", "eax", (long)DT_FAIL) + x86("xor", "edx", "edx")
             + x86("jmp", "rcx")
             + x86_gamma();
    }
    if (role == 1 || role == 2 || role == -1 /* NRETURN */) {
        /* ⛔⭐⭐⭐ LON RULING s54: globals GONE.  The RT_AB_ANCHOR global read and the rt_flat_ret_snap
         * (g_pcall peek) legacy path are DELETED from the floaters.  RETURN/NRETURN = jmp r10 (γ wire);
         * FRETURN = jmp r11 (ω wire) — LADDER WREG blob-exit spelling, now the floater spelling too.
         * ⛔ KNOWN BROKEN until WREG-3 lands: (a) rsp is NOT restored to entry depth here — the FORTH
         * paired-release discipline must hold interior-to-exit, and defers that survive on the spine break
         * it; (b) r10/r11 are SysV caller-saved — every C crossing in the body clobbers them until the
         * per-activation spine capture (WREG-3's law: pendings capture {r10,r11} at push, β restores).
         * Lon: "worry not about breakage."  The save-set restore stays caller-side (role 0's own rsp slots
         * + the landing epilogue) — already stack-resident. */
        /* ⭐⭐⭐ s58 RSP-ONLY DESCENT MODE (Lon in-chat: "put BOMBS in RETURN, FRETURN, and NRETURN... We want to run the entire test suite in RSP only mode... be left with only all the spots that
         * need RBP" — maybe RBP, maybe something else; the bombs make every UNKNOWN-STACK-DEPTH spot self-evident).  The coming-out side is FROZEN: RETURN arrives at a depth no one can know without a
         * frame anchor (the measured −16 class), so the jmp-r10/r11 wires are replaced by loud named bombs.  Reaching the RETURN bomb IS the descent test passing: args installed, body ran, transfer
         * arrived.  git revert is the undo when the depth mechanism lands. */
        if (fnrbp() == 1)
            /* s63 RBP-FUNCTION READERS (Lon: "RETURN, FRETURN, and NRETURN can restore an unknown stack depth and immediately return to the SHIM at the proper stack depth by POPPING the RBP stack frame").
             * rbp = innermost activation's pin (the shim's writer); [rbp+0]=enclosing rbp  [rbp+8]=gamma  [rbp+16]=omega  [rbp+24]=pad.  After the full 32B pop rsp == alpha-end depth P — exactly what
             * gamma/omega's rsp-derived RESTORE4 + constant release assume.  The floaters are program-wide singletons and discover the callee DYNAMICALLY through rbp — this is the whole point of the frame.
             * NRETURN rides gamma (manual: success by name; the result cell holds a NAME descr, deref is the consumer's business — 1013 is the witness).  rcx is free here (gamma reloads K itself).
             * ⛔ KNOWN EDGE: a top-level :(RETURN) with no active frame jumps through whatever rbp holds — pre-rung it bombed; SPITBOL errors there too ("&FNCLEVEL==0" class).  Accepted, noted. */
            return x86("comment", role == 1 ? "IR_SAVE_RESTORE RETURN floater (s63: pop the RBP frame, land on gamma)" :
                                   role == 2 ? "IR_SAVE_RESTORE FRETURN floater (s63: pop the RBP frame, land on omega)" :
                                               "IR_SAVE_RESTORE NRETURN floater (s63: pop the RBP frame, land on gamma — by-name result)")
                 + x86_alpha()
                 + x86("mov", "rsp", "rbp")
                 + x86("pop", "rbp")
                 + (role == 2 ? x86("add", "rsp", (long)8) + x86("pop", "rcx") + x86("add", "rsp", (long)8)
                              : x86("pop", "rcx") + x86("add", "rsp", (long)16))
                 + x86("jmp", "rcx");
        if (fnrbp() == 2)
            /* s64 RSP-ONLY READERS: by the depth-invariance law rsp HERE == P-16 (body-entry), so the pair is AT TOS.
             * [rsp+0]=gamma [rsp+8]=omega; pop/skip to land at P — exactly what gamma/omega's rsp-derived RESTORE4 +
             * constant release assume.  Recursion: each nested call pushed its own pair deeper, LIFO by stack discipline.
             * A leaking body arrives BELOW P-16 and jmps junk — loud, named, the census working as designed. */
            return x86("comment", role == 1 ? "IR_SAVE_RESTORE RETURN floater (s64 RSP-ONLY: pop {gamma,omega} pair at TOS — depth IS the anchor)" :
                                   role == 2 ? "IR_SAVE_RESTORE FRETURN floater (s64 RSP-ONLY: skip gamma, pop omega — depth IS the anchor)" :
                                               "IR_SAVE_RESTORE NRETURN floater (s64 RSP-ONLY: pop gamma — by-name result)")
                 + x86_alpha()
                 + (role == 2 ? x86("add", "rsp", (long)8) + x86("pop", "rcx")
                              : x86("pop", "rcx") + x86("add", "rsp", (long)8))
                 + x86("jmp", "rcx");
        return x86("comment", role == 1 ? "IR_SAVE_RESTORE RETURN floater (s58: BOMB — coming-out frozen)" :
                               role == 2 ? "IR_SAVE_RESTORE FRETURN floater (s58: BOMB — coming-out frozen)" :
                                           "IR_SAVE_RESTORE NRETURN floater (s58: BOMB — coming-out frozen)")
             + x86_alpha()
             + (role == 1 ? x86_bomb("BOMB-RETURN: descent complete, coming-out frozen (s58 RSP-only) — UNKNOWN STACK DEPTH: the rsp-resident record cannot be found from here without a frame anchor")
              : role == 2 ? x86_bomb("BOMB-FRETURN: descent complete, coming-out frozen (s58 RSP-only) — UNKNOWN STACK DEPTH: the rsp-resident record cannot be found from here without a frame anchor")
                          : x86_bomb("BOMB-NRETURN: descent complete, coming-out frozen (s58 RSP-only) — UNKNOWN STACK DEPTH: the rsp-resident record cannot be found from here without a frame anchor"));
    }
    /* role 0 — CALL2BB slice 2 (Lon s21x-c: "Have each BB allocate its RESULT value… its LOCAL STORAGE needs… by one instruction, decrement RSP"; "DEFINE, when CONSTANT FOLDED, emits exactly TWO BBs:
     * an IR_SAVE_RESTORE and an IR_CALL").  THE CALL-SITE SAVE/INSTALL BOX: this box's LOCALS are the save-set slots — carved by its OWN single `sub rsp` (never a whole-graph carve, never ___-indexed),
     * offsets slide from RSP.  Body = the BP-7 SCC prefix migrated out of bb_call_proc_staged: (1) spill the DEFINE save-set's old GVA cell values (fname/formals/locals per manual Ch.8 p.104 + Ch.19
     * DEFINE: "saved prior to function entry, and restored upon function return") into the own slots; (2) open_slim — ALL runtime guards re-checked before ANY side effect, so a 0 return needs nothing
     * undone (the spilled copies are pure reads, released by the decline's add); (3) committed: install the staged args into the formals' NV GLOBAL cells (SNOBOL4's one namespace — no ζ in the call
     * process, Lon SCOPE LAW).  The outcome rides rax into the adjacent staged call box (1 = record open + block LIVE across the edge for its landings to restore+release; 0 = declined + block released
     * → the call box's classic fallback at base depth).  Handoff via bb_scc_handoff_set = the emit-order contract (sr0.γ → call, chain-adjacent); non-eligible sites and any unresolved arg slot fall to
     * a pass-through so the call box keeps its full merged body — behavior identical to the gate-off world at those sites.  FRQB(slot, sb) self-compensates the live carve (the 083 FLATDISP-LIVE-BUMP
     * law); the fc_hit window arm lives inside FRQB, self-disabling here since DRIVE_FILL reset this node's grant fields. */
    int c2np = 0, c2nsave = 0, c2res_gk = -1; int c2gk[64]; long c2nargs = (long)_.op_arg_slot_n;
    int elig = bb_scc_probe(_.op_sval, (int)c2nargs, &c2np, &c2nsave, c2gk, &c2res_gk);
    for (int i = 0; elig && i < (int)c2nargs; i++) if (_.op_arg_slot[i] < 0) elig = 0;
    if (elig && !bb_scc_handoff_room()) elig = 0;   /* pending missing (drive shape surprise) or table full: decline safely — the call box keeps its merged body */
    if (!elig) { bb_scc_handoff_pending_clear(); return x86("comment", "IR_SAVE_RESTORE role 0: site not SCC-eligible (or an arg slot unresolved, or no handoff room) — pass-through; the staged call box runs its full merged body")
             + x86_alpha()
             + x86_gamma(); }
    long sb = 16L * (long)c2nsave;
    uint64_t slim_fp; { long (*fp)(const char *, int, int) = rt_proc_call_open_slim; slim_fp = (uint64_t)(uintptr_t)(void *)fp; }
    std::string s = x86("comment", "IR_SAVE_RESTORE role 0 (CALL2BB slice 2): own sub-rsp carve, save-set spill (GVA -> own slots), open_slim, staged args -> NV globals; outcome rides rax into the staged call box")
         + x86_alpha()
         + x86_zclaim(sb)
         + FOR(0, c2nsave, [&](int k) {
               return x86("note", gva_name(c2gk[k])) + x86("mov", "rax", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(c2gk[k], 0) : ABSQ(RT_GVA_VA + (unsigned long)c2gk[k] * 16)) + x86_rsp_store64(16 * k, "rax")
                    + x86("note", gva_name(c2gk[k])) + x86("mov", "rax", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(c2gk[k], 8) : ABSQ(RT_GVA_VA + (unsigned long)c2gk[k] * 16 + 8)) + x86_rsp_store64(16 * k + 8, "rax"); })
         + x86_scan_sync_out()
         + x86_ro_load_q("rdi", 0)
         + x86("mov32", "esi", (long)c2np)
         + x86("mov32", "edx", c2nargs)
         + x86("call", "rt_proc_call_open_slim", slim_fp)
         + x86_scan_sync_in_rr()
         + x86("test", "rax", "rax")
         + x86("jne", L(1))
         + x86_zrelease(sb)
         + x86("jmp", L(2))
         + x86("def", L(1))
         + FOR(0, (int)c2nargs, [&](int i) { int slot = _.op_arg_slot[i];
               return x86("mov", "rax", FRQB(slot, (int)sb))
                    + x86("note", gva_name(c2gk[i])) + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(c2gk[i], 0) : ABSQ(RT_GVA_VA + (unsigned long)c2gk[i] * 16), "rax")
                    + x86("mov", "rax", FRQB(slot + 8, (int)sb))
                    + x86("note", gva_name(c2gk[i])) + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(c2gk[i], 8) : ABSQ(RT_GVA_VA + (unsigned long)c2gk[i] * 16 + 8), "rax"); })
         + x86("mov32", "eax", 1L)
         + x86("def", L(2))
         + x86_gamma()
         + x86_ro_seal_str(0, _.op_sval ? _.op_sval : "");
    if (!bb_scc_handoff_arm()) return x86_alpha() + x86_bomb("IR_SAVE_RESTORE role 0: handoff arm failed after the room check passed (impossible unless the drive deposit raced)");
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_nreturn_mark() {
    extern int rt_g_ret_by_name;
    return x86_alpha()
         + x86("comment", "NRETURN floater: by-name mark (manual p.133); rt_nret_fix in the call epilogue reads+clears it; depth-agnostic — zero [rsp+K], zero calls; glue continues at RETURN")
         + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&rt_g_ret_by_name, "rt_g_ret_by_name")
         + x86("mov", RDD("rax", 0), (long)1)
         + x86_gamma();
}
