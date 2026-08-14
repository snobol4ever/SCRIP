#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
extern "C" void * rt_zcol_push(void ** ptr_cell, int * cap_cell, int i, long elem_sz);
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ZB-ITER-3 (s85): zv() = the chain arm's ELEMENT VIEW register — ___ borrowed from the MATCH_BEGIN frame base.  FRQ() reads in the chain arm are ___-relative (depth-immune under the pinned MATCH_BEGIN frame),
 * so the sigma/phi paths read element-frame slots correctly at ANY rsp depth.  W-1c.2: the view-restore guards (arbno_u2_frame) are made UNCONDITIONAL — nested body execution (inner ARBNO beta, DEFINE frames)
 * clobbers ___; re-deriving it via `lea ___,[rsp+(24-op_sa)]` at PAIR(2) (rsp==element frontier on body success) and saving/restoring it at exhaust L(2) via a dedicated frame slot is always correct.  The slot+32
 * grant in zeta_storage.c is now UNCONDITIONAL (matching: template always writes slot+32, grant always allocates it; SCRIP_U2 condition deleted).  arbno_u2_frame() deleted — its guards fire unconditionally. */
static inline const char * zv() { return "rsp"; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
   /* M-2 BUG-5 FIX: raw machine-___ dword (twin of ___RAWQ in x86_asm.h); [___# + N] escapes x86_parse's FR classifier -- always encodes as a direct reg+disp, never re-canonicalized through the frame base.  Used in bb_match_arbno_tail PAIR(2) to reach the element header after MATCH_END's CAS restore. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string arbno_zero_window(long op_sb) { std::string r; for (long k = 24; k < op_sb; k += 8) r += x86("mov", RSP((int)k), "rax"); return r; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int arbno_nofill(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_ARBNO_NOFILL"); v = e ? (atoi(e) != 0) : 1; } return v; }   /* ARBNO-NOFILL (s141): default ON; =0 restores the eager per-element window zero (kill-switch, SCRIP_PAT_NOFILL precedent) */
static int arbno_poison(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_ZLS_POISON"); v = e ? (atoi(e) != 0) : 0; } return v; }   /* verification lane twin of xa_flat's: fill the SKIPPED window 0xA5 at compile time so any read-before-write in the body subgraph surfaces loudly */

static std::string arbno_fill_cells(void) { std::string r; for (int i = 0; i < _.op_arbno_nzq; i++) { int w = _.op_arbno_zq[i] - _.op_sa + 24; r += x86("mov", RSP(w), "rax"); } return r; }   /* zero the cap BUF QUAD of each body SAVE cell at its window-relative offset — gen/sp self-heal via the generation stamp (pattern_match.c rt_cap_push) */
static std::string arbno_fill_window(long op_sb) {
    return (!arbno_nofill() || _.op_arbno_nzq > 8)
             ? x86("mov", "eax", 0L) + arbno_zero_window(op_sb)
         : arbno_poison()
             ? x86("movabs", "rax", (uint64_t)0xA5A5A5A5A5A5A5A5ULL) + arbno_zero_window(op_sb) + (_.op_arbno_nzq > 0 ? x86("mov", "eax", 0L) + arbno_fill_cells() : std::string())
         : _.op_arbno_nzq > 0
             ? x86("mov", "eax", 0L) + arbno_fill_cells()
             : std::string();
}   /* ARBNO-NOFILL (s141): the chain-β per-element window [24,op_sb) is the body subgraph's per-iteration ζ region — box grants are first-write-wins by the four-port contract (rt.c:1402), the same
     * class the s139 SPD-NOFILL proved for PAT$ activation frames; the eager fill was IR_MATCH_ARBNO's 58% treebank Ir share (s140 bbprof, 19-21 quads per element on the inner group ARBNO).  Poison
     * lane proof (this session): the ONLY implicit-zero citizens across all 311 crosscheck programs are body IR_MATCH_ASSIGN_SAVE cells (066/145, rt_cap head cells empty==zero BY DESIGN) — those are
     * staged per-node (zls_arbno_zq → op_arbno_zq/nzq) and zeroed targeted; >8 captures (nzq==9 sentinel) falls back to the eager blanket. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline const char * trd(int off) { static char b[8][40]; static int i; i = (i + 1) & 7; snprintf(b[i], 40, "dword ptr [rsp + %d]", off); return b[i]; }
static inline const char * trq(int off) { static char b[8][40]; static int i; i = (i + 1) & 7; snprintf(b[i], 40, "qword ptr [rsp + %d]", off); return b[i]; }
static std::string tail_zero(int lo, int hi, const char * zr64) { std::string r; for (int k = lo; k < hi; k += 8) r += x86("mov", trq(k), zr64); return r; }
static std::string tail_cap_zero8(int base, int n, const char * zr64) { std::string r; for (int j = 0; j < n; j++) r += x86("mov", trq(base + 16 * j + 8), zr64); return r; }
static std::string tail_cap_copy(int dst, int src, int n) { std::string r; for (int j = 0; j < n; j++) r += x86("mov", "rax", trq(src + 16 * j)) + x86("mov", trq(dst + 16 * j), "rax"); return r; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int arbno_lon(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_ARBNO_FRAMELESS"); v = e ? (atoi(e) != 0) : 1; } return v; }   /* ⭐ ARBNO-LON killswitch (default ON): =0 reverts the nested-K0 class to the legacy chain arm byte-identically */
static int arbno_rootspine(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_ARBNO_ROOTSPINE"); v = e ? (atoi(e) != 0) : 0; } return v; }   /* ⭐ ROOT-SPINE (FF-1 SCAFFOLD, Lon 2026-08-11 "RESTORE NOTHING — proper RSP/___ FORTH-style stack"): ⛔ DEFAULT 0 — MEASURED INSUFFICIENT ALONE: root-spine α makes the ROOT per-activation but every yield exit (ε-γ and each PAIR(2) prev-view restore) then terminates the view chain at root-op_off instead of the statement frame, so downstream FR readers leak a view as ___ — claws5-match (defer-free survivor) SEGV'd under default 1.  The COMPLETE arm additionally restores true ___ from [root+32] at every γ exit AND re-derives the view at as/af entry (see FINDING 2026-08-11 FF-0: blob γ/ω/res never touch ___ — view continuity across defer suspension is the ACTUAL defect; this α is the correct first half).  =1 = chain-arm ROOT header lives in a 48B alpha-carved SPINE record (per-activation BY CONSTRUCTION), view0 = root - op_off so every FR(op_off+X) spelling resolves [root+X] unchanged; =0 restores the FR frame-slot root for same-build A/B.  WHY: FR root state was per-activation ONLY under the pinned-___ contract (one fresh ___ per BLOB-GRANT frame); DEL-T1 (1af93e3a) deleted that contract for blob interiors, so every nested activation of the same node shared ONE statement-frame header — pattern_match.c:624's single-cell defect in frame clothing.  gdb conviction: treebank-match 327B witness, inner *group re-entry aliased its header writes onto the outer element window (op_off+24 == op_sa-24 == +88 collide), as-arm restored a clobbered cursor (___=31) as the view base.  Exhaust self-releases the root: [root+24] stores pre-carve rsp (root+48), so L(2)'s pointer restore reclaims record and elements in one move — the FORTH way, no size arithmetic. */
static int arbno_fprpop(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_ARBNO_FPRPOP"); v = e ? (atoi(e) != 0) : 0; } return v; }   /* ⭐ TWO-CALCULATORS RETIRED (N24 s38, default 0 = beta does NOT pop; =1 restores the legacy double-pop for same-build A/B bisect).  THE RIGHT SPINE OWNS THE RELEASE OF ITS OWN CELLS: ZD-5b POS/RPOS CONST-WPOP (emit.cpp:1448, op_wpop+=16, promoted to THE ONE AUTHORITY at s32) already pops the right-spine LIT_INT 16B on the failure edge INTO beta -- measured identical in N02 and N24 (`add rsp,16; jmp arbno_β` in match_rpos omega).  N02-FIX (L156) then popped it a SECOND time, so beta arrived at element_base+16 and every header read was displaced: yield read at +52 = HIGH DWORD of the saved quad at +48 = 0x7fff (gdb-measured, N24 m4).  m3 read 0 there instead, which is accidentally the correct delta for the FIRST instance -- hence N23 passed m3 and SEGV began at 2 instances (delta frozen -> unbounded chain growth -> rsp runaway), while m4 failed from 1 instance. */
static inline int kkN(void) { return _.op_arbno_body_kk; }   /* staged body-span ΣK, THE ONE K AUTHORITY zd_k summed by the emit.cpp ARB-LON-K16 prelude -- the one-level-in static offset of the previous cell at the σ junction.  Killswitches SCRIP_ARBNO_K16 / SCRIP_ARBNO_FRAMELESS are read in that prelude ONLY (single routing authority); =0 stages kk=0 and every byte reverts to the legacy routes. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_match_arbno_frameless_k() {
    /* ⭐ ARB-LON-K16 (widens ARBNO-LON past body-K0 per the LIVE CURSOR rung): the ONE 16B cell {DELTA0 dword at +0, yield dword at +4} is RE-HOMED to the frontier at each σ commit instead of updated in
     * place, so with a SEQUENCE-ONLY STATIC-K body (SAVE cells etc.; kk = ΣK over the span, staged from zd_k) every read site is a compile-time offset: σ reads the PREVIOUS cell at [rsp+kk] -- exactly one
     * level in, the op_zread depth-difference shape -- copies DELTA0 forward into a fresh cell at [rsp+0..15], and yields; exhaust arrives (per the unwind law, every attempt carve already freed by the
     * body's own βs) with the CURRENT cell at [rsp+0]; retract discards the current copy (add rsp,16) landing rsp exactly on the last committed instance's body frontier, which is the state PAIR(1)'s body
     * resume β expects -- when that instance fully retreats its member βs free own K and rsp arrives at the PREVIOUS cell: the invariant self-maintains with zero counters, links, views, or chains.  δ==Δ0
     * at [rsp+0] iff nothing remains committed (σ guarantees every committed instance advanced; the cascade restores δ instance-by-instance), so the Δ-assert at ω holds by construction, as in the K0 arm.
     * Committed growth (kk+16 per instance) is NON-POPPING by THE MODEL -- released only by the bracket whacks (MATCH_END absolute restore / FENCE commit / STATEMENT_END), never by ARBNO. */
    return x86("comment", "IR_MATCH_ARBNO_FRAMELESS_K (ARB-LON-K16: re-homed cell, one-level static offsets)")
         + x86_alpha()
         + x86("sub", "rsp", 16L)
         + x86("mov", RDD("rsp", 0), "r14d")
         + x86("mov", RDD("rsp", 4), "r14d")
         + x86_gamma()
         + x86_beta()
         + x86("jmp", PAIR(0))
         + x86("def", PAIR(2))
         + x86("mov", "eax", RDD("rsp", kkN() + 4))
         + x86("cmp", "r14d", "eax")
         + x86("je",  PAIR(1))
         + x86("mov", "eax", RDD("rsp", kkN()))
         + x86("sub", "rsp", 16L)
         + x86("mov", RDD("rsp", 0), "eax")
         + x86("mov", RDD("rsp", 4), "r14d")
         + x86_gamma()
         + x86("def", PAIR(3))
         + x86("mov", "eax", RDD("rsp", 0))
         + x86("cmp", "r14d", "eax")
         + x86("jne", L(3))
         + x86("add", "rsp", 16L)
         + x86_omega()
         + x86("def", L(3))
         + x86("add", "rsp", 16L)
         + x86("jmp", PAIR(1));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_match_arbno_frameless() {
    /* ⭐ ARBNO-LON (Lon rulings 2026-08-06: "ARBNO needs NOT its own ___ frame ... We never need to know the stack depth for ARBNO; just let it grow ... freed on either a FINAL MATCH_END or DURING the
     * match on a FENCED construct ... no chain is needed.  When will that chain be traversed?  Never.  ARBNO will have ONE BB local, the DELTA subject cursor").  The whole construct: ONE 16B alpha-carved
     * rsp cell {DELTA0 dword at +0, yield-cursor dword at +4} and TWO compares.  No counter, no element links, no saved-rsp mark, no view register, no U2 quad -- the counter machinery is what MEASURABLY
     * aliased and killed the 5 nested probes (X02 gdb 2026-08-06), and under exact unwind it was bookkeeping someone else's allocator (RULING 2).  Instance k's sigma entry IS the (k-1)th yield -- one
     * rolling datum, the shipping suite-validated semantics -- so beta (extend) stores nothing: the baseline is already in the cell.  Exhaust needs no depth test: sigma guarantees every committed
     * instance advanced delta, the un-match cascade restores delta instance-by-instance on retract, so delta==DELTA0 iff nothing remains -- and delta arrives at omega already == DELTA0, the Delta-assert
     * by construction.  Gated to the nested-K0 class (op_arbno_framed && op_arbno_body_k0): the frontier never moves inside the activation, so the cell is [rsp+0]/[rsp+4] from EVERY site.  Growth
     * release is the bracket constructs' job (MATCH_END whack / FENCE commit / STATEMENT_END), per the frame census {STATEMENT, FUNCTION, MATCH_BEGIN, FENCE1} -- ARBNO dropped out. */
    return x86("comment", "IR_MATCH_ARBNO_FRAMELESS (ARBNO-LON: one cell, two compares, no chain)")
         + x86_alpha()
         + x86("sub", "rsp", 16L)
         + x86("mov", RDD("rsp", 0), "r14d")
         + x86("mov", RDD("rsp", 4), "r14d")
         + x86_gamma()
         + x86_beta()
         + x86("jmp", PAIR(0))
         + x86("def", PAIR(2))
         + x86("mov", "eax", RDD("rsp", 4))
         + x86("cmp", "r14d", "eax")
         + x86("je",  PAIR(1))
         + x86("mov", RDD("rsp", 4), "r14d")
         + x86_gamma()
         + x86("def", PAIR(3))
         + x86("mov", "eax", RDD("rsp", 0))
         + x86("cmp", "r14d", "eax")
         + x86("jne", PAIR(1))
         + x86("add", "rsp", 16L)
         + x86_omega();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_match_arbno_tail() {
    /* R12-EXIT-1 CARRY-THE-TAIL (Lon s68 static-size proof; design of record at zeta_storage.c fc_tail_*).  Element = [0,span) body window + [span,span+rspan) right-spine window + 16B header
     * {entry-cursor@+0, yield-cursor@+4, elem0-flag@+8} + 16B bracket copy {patstk mark@+16, rsp mark@+24} + one 16B WRAP-CAPTURE slot per ARBNO(body).V pair at [HDRB+32+16j] (delta at slot+0, alpha
     * copies pair j from its flat SAVE cell at [KA+16j], beta carries the current element's slot forward, the wrap COND reads [HDRA+32+16j] at the uniform yield depth -- fcc-registered at finalize,
     * zero capture-template change); op_sb = align16(span+rspan+32+16*ncap); HDRB = span+rspan (elem-relative header base, staged in
     * op_sa); HDRA = fp_body + HDRB (header base from the UNIFORM yield depth rsp = elem - fp_body, which alpha's phantom body pad establishes for the epsilon yield and S10c gamma-suspension
     * maintains for every extension).  alpha pushes op_sb+fp_body copying the bracket from HEAD's cell at [rsp + KA + fp_left + k]; beta pushes op_sb copying it from the current element; the
     * fail-glue pop `add rsp, op_sb` lands EXACTLY on the previous element's yield frontier (LIFO + fixed size = arithmetic, never indirection); exhaust pops op_sb ONLY (the resumed-epsilon cascade's
     * box-omega pops already consumed the FPB phantom pad -- popping KA here overshot flat by FPB, the s71 measured SEGV: oracle-identical trace then exit 139 at first FPB>0 exhaust) and omega runs at flat depth.
     * Every reference is [rsp + compile-time-const] at element depth -- EXCEPT PAIR(2)/PAIR(3) which run post-MATCH_END CAS restore (rsp back to pre-head entry level).  Those use ___-relative
     * addressing: the hfc pin (bb_match_begin rpin()=1) sets ___=rsp AFTER sub rsp,80, so ___ = original_rsp - 80.  At ARBNO alpha, rsp = ___ - FPL - KA; element header HDRA+N is at
     * [___ + ____HDR + N] where ____HDR = -(FPL + KA - HDRA) = -(FPL + op_sb + FPB - HDRA).  This is stable across MATCH_END's whack. */
    /* L2 FENCE SEAL (s71; the flat path's PAIR(1)->na_f re-aim with the element scheme's depth fixed): resume a committed iteration ABANDONS it.  Every PAIR(1) departure in this template -- the sigma
     * null-progress je and phi's post-pop resume -- runs at the UNIFORM yield depth rsp = elem - FPB (the alpha phantom pad + S10c suspension invariant), and external resume routes to this box's beta
     * (extend), never PAIR(1); so the whole seal is ONE glue: L(3) arithmetic-pops the dead suspended body (its alternatives are forbidden -- skipping the sealed boxes' omegas IS the seal) and falls
     * through into phi, whose elem0 dance then cascades pop-by-pop to exhaust exactly as SPITBOL cuts left (manual ln 4716).  The epsilon element exits at the flag check before any body code runs. */
    int HDRB = _.op_sa, FPB = _.op_tail_fpb, FPL = _.op_tail_fpl, SEAL = _.op_tail_seal, NCAP = _.op_tail_ncap;
    int KA = (int)_.op_sb + FPB, HDRA = FPB + HDRB;
    int FB5_HDR = -(FPL + KA - HDRA);   /* M-2 BUG-5 FIX: ___-relative base for element header, post-MATCH_END CAS restore.
                                          * ___ = original_rsp - 80 (hfc pin); element header HDRA+N at [___ + ____HDR + N].
                                          * ____HDR = -(FPL + KA - HDRA): negative offset from pin base to element header top. */
    int FPR_RSP = _.op_tail_fpr_rsp;   /* ⭐ N02-FIX: actual rsp-push depth of right-spine nodes -- same BUG-5 law as FPL.
                                         * Right-spine flat-allocated nodes (LIT_INTEGER for RPOS/POS arg) push K=16 rsp cells
                                         * before gamma; on failure those cells stay on the stack and beta arrives with
                                         * rsp = element_base - FPR_RSP instead of element_base.  Beta pops FPR_RSP before
                                         * reading element header slots.  N01 (empty right-spine) FPR_RSP=0, byte-identical. */
    return x86("comment", "IR_MATCH_ARBNO_TAIL (R12-EXIT-1 carry-the-tail rsp elements)")
         + x86_alpha()
         + x86("sub", "rsp", (long)KA)
         + x86("mov", "eax", 0L)
         + tail_zero(_.op_tail_dfr ? 0 : FPB, FPB + HDRB, "rax")   /* PS-3 s153: defer-bearing candidates zero the PHANTOM FPB pad too -- the guarded defer β's zero-discriminator must be deterministic (dirty stack otherwise); defer-free candidates keep [FPB, FPB+HDRB) byte-verbatim */
         + tail_cap_zero8(HDRA + 32, NCAP, "rax")
         + x86("mov", trd(HDRA + 0), "r14d")
         + x86("mov", trd(HDRA + 4), "r14d")
         + x86("stk32", (long)(HDRA + 8), 1L)
         + x86("mov", "rax", trq(KA + FPL + 8))
         + x86("mov", trq(HDRA + 16), "rax")
         + x86("mov", "rax", trq(KA + FPL + 16))
         + x86("mov", trq(HDRA + 24), "rax")
         + tail_cap_copy(HDRA + 32, KA, NCAP)
         + x86_gamma()
         + x86_beta()
         + IF(FPR_RSP > 0 && arbno_fprpop(), x86("add", "rsp", (long)FPR_RSP))   /* ⭐ N02-FIX: pop right-spine cells before reading element header -- right-spine failure (RPOS/POS literal arg) leaves FPR_RSP bytes on the stack displacing element_base; add restores the invariant rsp==element_base so trd(HDRA+N) addresses correctly.  N01 FPR_RSP=0, IF gate is dead, byte-identical.  ⛔ SCRIP_ARBNO_FPRPOP=0 bisects the double-pop against ZD-5b (emit.cpp:1448). */
         + x86("mov", "r14d", trd(HDRA + 4))
         + x86("mov", "rax", trq(HDRA + 16))
         + x86("mov", "rcx", trq(HDRA + 24))
         + x86("sub", "rsp", (long)_.op_sb)
         + x86("mov", "edx", 0L)
         + tail_zero(0, HDRB, "rdx")
         + x86("mov", trd(HDRB + 0), "r14d")
         + x86("mov", trd(HDRB + 4), "r14d")
         + x86("stk32", (long)(HDRB + 8), 0L)
         + x86("mov", trq(HDRB + 16), "rax")
         + x86("mov", trq(HDRB + 24), "rcx")
         + tail_cap_zero8(HDRB + 32, NCAP, "rdx")
         + tail_cap_copy(HDRB + 32, (int)_.op_sb + HDRA + 32, NCAP)
         + x86("jmp", PAIR(0))
         + x86("def", PAIR(2))
         /* N02-FIX: PAIR(2) is a body-success handler — rsp = current_element_base at every entry.
          * The BUG-5 ___RAWD fix assumed "PAIR(2) fires post-MATCH_END CAS restore" but that is
          * wrong: MATCH_END success jumps to STATEMENT_END, never through PAIR(2).  PAIR(2) is
          * reached ONLY from body γ (rsp = current_element_base), so trd() = [rsp+HDRA+N] is the
          * correct addressing for any element (ε or extended).  ___RAWD(____HDR+N) only matches
          * [element_base+N] for the ε element (element_base = ___-FPL-KA); for the Nth extension
          * element_base shifts by -(N-1)*op_sb, making ___RAWD read ε's slot instead of the current
          * element's — null-progress false positive → cursor clobbered → SIGSEGV.
          * trd(HDRA+0) = [rsp+HDRA] = [current_element_base+HDRA] ← always correct. */
         + x86("mov", "eax", trd(HDRA + 0))
         + x86("cmp", "r14d", "eax")
         + x86("je",  SEAL ? L(3) : PAIR(1))
         + x86("mov", trd(HDRA + 4), "r14d")
         + x86_gamma()
         + IF(SEAL, x86("def", L(3)) + IF(FPB > 0, x86("add", "rsp", (long)FPB)))
         + x86("def", PAIR(3))
         /* M-2 BUG-5 FIX: PAIR(3) phi path runs at element yield depth (rsp = elem - FPB); elem0-flag at
          * HDRB+8 is inside current element so trd() is correct here.  Entry cursor at HDRB+0 is also within
          * the current element (β's freshly-pushed copy), so trd(HDRB+0) at L(2) is correct. */
         + x86("mov", "eax", trd(HDRB + 8))
         + x86("test", "eax", "eax")
         + x86("jnz", L(2))
         + x86("add", "rsp", (long)_.op_sb)
         + x86("jmp", SEAL ? L(3) : PAIR(1))
         + x86("def", L(2))
         + x86("mov", "r14d", trd(HDRB + 0))
         + x86("add", "rsp", (long)_.op_sb)
         + IF(_.flat_deep_arrival, x86("note", "old_base") + std::string(""))   /* M-2 BUG-6 FIX: BRACKET-GATE ___ restore moved here from release_pump (bb_match_end.cpp !op_tail gate). At L(2) omega rsp=cas_rsp_mark-32-FPL; old____ was saved by MATCH_BEGIN at [cas_rsp_mark+40]=[rsp+FPL+72]. Gated flat_deep_arrival matching bb_match_begin's save gate. */
         + x86_omega();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_match_arbno_rbp() {
    /* ⭐ ARBNO-RBP (Lon in-chat 2026-08-14, verbatim: "For ARBNO, ALWAYS create an RBP on ALPHA to carve out RESULT and ONE LOCAL, an integer counter of # instances which when goes negative then causes
     * exhaustion thru OMEGA.  So in BB ARBNO at BETA, the BB LOCAL is directly accessed thru RBP+16 or +0 whichever.").  Layout is the IR_TO shape on an rbp base: α pushes caller rbp (frame chain --
     * nested/sibling ARBNOs restore LIFO by the four-port retreat order), carves 24, sets rbp=rsp, so RESULT rides [rbp+0..15] {Δ0 dword@+0, yield dword@+4}, the LOCAL counter rides [rbp+16], and the
     * saved caller rbp rides [rbp+24]; total Δ32, C-call 16-alignment preserved, zd_k(ARBNO)=32 under emit_arbno_rbp() (THE ONE AUTHORITY, emit.cpp).  Every read site is a FIXED rbp offset -- depth-
     * immune at ANY ζ frontier -- which retires by construction the k0=0-body-on-FRAMELESS class (arb1 FINDING-2026-08-11e: the [rsp+4] dword cursor write assumed a frontier that K>0 members, DEFER
     * records, and instance peeling all move; measured stomp on a 64-bit resume address).  Counter protocol: σ +1 per committed instance (null-progress je pumps the body without counting, exactly the
     * frameless law), φ -1 per peel; a retry cycle is φ then σ, net 0, so count always == #currently-committed; count going NEGATIVE at φ == the ε (null-match) baseline itself retried away == Lon's
     * goes-negative exhaustion -- the deleted NARY arm's count==0 pre-test restructured as sub+js, sign flag free from the decrement.  φ pops nothing: failed attempts' members freed own K retreating
     * (THE MODEL), committed members' K stays on the spine per the non-popping law (released by bracket whacks / the op_stmt_dyn terminal), and ARBNO itself pushes no per-instance cell -- the counter
     * replaces the ζ-displacement-as-count trick, dissolving the D-1 zero-width-body constraint.  Exhaust ω restores δ:=Δ0 (the Δ-assert made explicit, NARY's law), then lea rsp,[rbp+24] + pop rbp:
     * the whole-frame WHACK, robust to any residual drift below.  Suspension holds rbp live across γ (β/σ/φ re-enter through it, per the directive); rbp is callee-saved so C crossings and the
     * balanced FN_RBP function bracket preserve it; MATCH-RBP frames never coexist (emit_match_rbp excludes the ARBNO family wholesale).  Killswitch SCRIP_ARBNO_RBP=0: legacy byte-identical. */
    return x86("comment", "IR_MATCH_ARBNO_RBP (Lon 2026-08-14: rbp frame, RESULT@[rbp+0..15], count@[rbp+16], negative=>exhaust)")
         + x86_alpha()
         + x86("push", "rbp")
         + x86("sub", "rsp", 24L)
         + x86("mov", "rbp", "rsp")
         + x86("mov", RDD("rbp", 0), "r14d")
         + x86("mov", RDD("rbp", 4), "r14d")
         + x86("mov", RDD("rbp", 16), 0L)
         + x86_gamma()
         + x86_beta()
         + x86("jmp", PAIR(0))
         + x86("def", PAIR(2))
         + x86("mov", "eax", RDD("rbp", 4))
         + x86("cmp", "r14d", "eax")
         + x86("je",  PAIR(1))
         + x86("mov", RDD("rbp", 4), "r14d")
         + x86("add", RDD("rbp", 16), 1L)
         + x86_gamma()
         + x86("def", PAIR(3))
         + x86("mov", "eax", RDD("rbp", 16))
         + x86("sub", "eax", 1L)
         + x86("js",  L(2))
         + x86("mov", RDD("rbp", 16), "eax")
         + x86("jmp", PAIR(1))
         + x86("def", L(2))
         + x86("mov", "r14d", RDD("rbp", 0))
         + x86("lea", "rsp", RDQ("rbp", 24))
         + x86("pop", "rbp")
         + x86_omega();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void arbno_arm_diag(const char * arm) { static int d = -1; if (d < 0) { const char * e = getenv("SCRIP_ARBNO_DIAG"); d = (e && *e == '1') ? 1 : 0; } if (d) fprintf(stderr, "[ARBNO-ARM] %s\n", arm); }
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_arbno() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    /* ⛔ D-1 DELETE (Lon-directed, delete-first): ARBNO IS WIRING + ONE CELL.  The TAIL / DT / NARY-CHAIN arms are GONE -- every ARBNO now rides the ARBNO-LON frameless pair, which differ only in whether
     * the body's staged ΣK is zero.  THE ONE THING ARBNO MAY NOT DELEGATE is a NONZERO per-instance frontier delta: the ζ displacement IS the committed-instance count (no counter exists), so a zero-width
     * body makes the retract cascade bail at the base test without unwinding a single committed instance -- measured on corpus/probe/bb/test_sno_arbno_csl1a.c, CELLSZ=0 leaves Δ=3 where 0 is correct.
     * zd_k(ARBNO)=16 is that floor and it is already THE ONE AUTHORITY; frameless_k bills committed growth as kk+16 per instance, so a K0 body still advances by ARBNO's own cell. */
    arbno_arm_diag(_.op_arbno_body_kk > 0 ? (_.op_arbno_rbp ? "RBP(kk)" : "FRAMELESS_K") : _.op_off < 0 ? "bomb-slot" : (_.op_sa < 0 || _.op_sb <= 0) ? "bomb-geom" : _.op_arbno_body_defer_unsafe ? "bomb-defer-unsafe" : (_.op_arbno_rbp ? "RBP" : "FRAMELESS"));
    return _.op_arbno_body_kk > 0
             ? (_.op_arbno_rbp ? bb_match_arbno_rbp() : bb_match_arbno_frameless_k())   /* ⭐ ARBNO-RBP (Lon 2026-08-14): both live arms reroute on the ONE staged predicate; bombs 2/3/4 below stay IN FRONT (same population declines under both arms -- defer-unsafe lift is a separate measured step); OFF = legacy byte-identical */
         : _.op_off < 0
             ? x86_alpha() + x86_bomb("IR_MATCH_ARBNO: slot not granted (zls)")
         : (_.op_sa < 0 || _.op_sb <= 0)
             ? x86_alpha() + x86_bomb("IR_MATCH_ARBNO: COLLECTION geometry not staged (zls_arbno_geom)")
         : _.op_arbno_body_defer_unsafe
             /* W-7 INTERIM GUARD (HOME-WIRES s38): body carries an IR_MATCH_DEFER with pat_static==0 -- can transitively recurse
              * back through this ARBNO's activation (manual p.122's *X idiom), which the plain-frameless arm below does not model:
              * that arm's [rsp+4] yield-cursor write assumes the frontier never moves inside the activation.  D12/D13 are this
              * class (gdb-confirmed stomp on a live CLASS-D resume-record landing address, FINDING-2026-08-12k).  Declining here
              * converts silent stack corruption into a loud compile-time refusal; the real fix needs an anchor-relative ARBNO
              * cell (W-4's arena layout).  Floor-neutral: the 16-probe pat_static==1 passer set never sets this flag (see
              * emit.h field note), so this arm is never reached for them.
              * PAIR(1)/PAIR(2)/PAIR(3) (β/na_s/na_f) are all registered UNCONDITIONALLY for every node (betas[i] in the generic
              * per-node label pass, emit.cpp ~2412) and for every IR_MATCH_ARBNO with operands (na_s/na_f, flat_drive_match_alt,
              * emit.cpp ~1201) -- sibling boxes' forward references to them exist independent of which arm this dispatcher
              * picks, so the bomb path must still `def` all three (dead code after `ud2`, never reached, but required for
              * bb_emit_end's forward-reference resolution to succeed at all). */
             ? x86_alpha() + x86_bomb("IR_MATCH_ARBNO: body contains a suspend-capable DEFER (pat_static=0) -- anchor-relative slot not yet implemented (W-4)")
                            + x86_beta() + x86_bomb("IR_MATCH_ARBNO: unreachable beta (defer-unsafe decline)")
                            + x86("def", PAIR(2)) + x86("def", PAIR(3))
             : (_.op_arbno_rbp ? bb_match_arbno_rbp() : bb_match_arbno_frameless());
}
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_match_arbno_DELETED_ARMS() {
    return _.op_tail
             ? bb_match_arbno_tail()
         : (_.op_arbno_dt && _.op_arbno_dt_susp == 0)
             /* PS-3 SLICE 2 (s155) RECORD-CARRY DEFER-TAIL — the susp-free form; uniformity gate + recursion barrier DISSOLVED (the FINDING-2026-07-25 slice-2 design, pad ruling A).  The s152 arm's ONE
              * size-dependent datum was the element entry cursor, stack-parked BELOW the blob and read back over it at [rsp+susp] — requiring the registry-uniform compile-time SUSP.  Here the entry
              * cursor lives in the flat quad FR(off+4) while the element is IN FLIGHT (β reads it there and leaves it untouched; only ONE element is ever in flight per activation, and recursion is safe
              * because FR is per-activation by the pinned-___ contract), and at yield-accept it rides a 16B CARRY RECORD pushed ON TOP of the blob's own frontier record: {[rsp+0]=&L(4) glue,
              * [rsp+8]=entry cursor} — a JUMPABLE record, so the universal retreat `jmp [rsp+0]` composes: L(4) restores the cursor into FR(off+4), pops itself, and falls through to the blob's record.
              * The exhaust needs NO arithmetic at all: the blob's ω is the ABSOLUTE `lea rsp,[___+kt]` self-release (xa_flat epilogue, pat arm), landing rsp exactly on the previous element's carry
              * record (or the ARBNO frontier at elem0) — pop = pointer restore, never a size add.  ANY arriving blob admits: unregistered, non-uniform, recursive (*group), even the slow rt_defer path
              * (whose own {L6} exhaust record is 16B and self-popping — the carry composes over it identically).  σ/count/exhaust bookkeeping mirrors the s152 arm move-for-move. */
             ? x86("comment", "IR_MATCH_ARBNO_DT (PS-3 s155 record-carry: susp-free, recursion-safe)")
             + x86_alpha()
             + x86("mov", FR(_.op_off), "r14d")
             + x86("mov", FR(_.op_off + 4), "r14d")
             + x86("mov", FR(_.op_off + 8), 0L)
             + x86("mov", FRQ(_.op_off + 24), "rsp")
             + x86_gamma()
             + x86_beta()
             + x86("mov", "r14d", FR(_.op_off + 4))
             + x86("jmp", PAIR(0))
             + x86("def", PAIR(2))
             + x86("mov", "eax", FR(_.op_off + 4))
             + x86("cmp", "r14d", "eax")
             + x86("je",  PAIR(1))
             + x86("mov", "rcx", "rax")
             + x86_lea_id("rax", 4)
             + x86("sub", "rsp", 16L)
             + x86("mov", trq(0), "rax")
             + x86("mov", trq(8), "rcx")
             + x86("mov", "eax", FR(_.op_off + 8))
             + x86("add", "eax", 1L)
             + x86("mov", FR(_.op_off + 8), "eax")
             + x86("mov", FR(_.op_off + 4), "r14d")
             + x86_gamma()
             + x86("def", L(4))
             + x86("mov", "rax", trq(8))
             + x86("mov", FR(_.op_off + 4), "eax")
             + x86("add", "rsp", 16L)
             + x86_jmp_mem("rsp", 0)
             + x86("def", PAIR(3))
             + x86("mov", "ecx", FR(_.op_off + 8))
             + x86("test", "ecx", "ecx")
             + x86("jz",  L(2))
             + x86("sub", "ecx", 1L)
             + x86("mov", FR(_.op_off + 8), "ecx")
             + x86("jmp", PAIR(1))
             + x86("def", L(2))
             + x86("mov", "r14d", FR(_.op_off))
             + x86("mov", "rsp", FRQ(_.op_off + 24))
             + x86_omega()
         : _.op_arbno_dt
             /* PS-3 (s152) DEFER-TAIL (Lon directive: solve ARBNO(*P); ARCH-SNOBOL4 §ARBNO eradication ruling).  The body is ONE write-once prologue-bound defer: its blob SELF-allocates, γ-suspends
              * leaving the 16B {res,___} frontier record at [rsp+0] (t1.s ground truth), β = jmp [rsp+0] self-pops, ω restores rsp to the blob's entry frontier sweeping every interior carve (lea
              * rsp,[___+K]).  So the blob IS the iteration frame: no link cell, no view repoint, no element window -- the chain ceremony deletes wholesale.  The ONLY per-element datum is the entry
              * cursor (σ's null-progress baseline survives a resume-and-re-yield only per-element; the flat yield quad is clobbered by the first yield), stored in a 16B header ABOVE the blob carve
              * and read from the uniform yield depth at [rsp + op_arbno_dt_susp] -- the compile-time ζ size align16(32+fb)+fp+16 from the emit_patzeta registry, constant across yields by the UNIFORM
              * license (no interior ARBNO/DEFER/VALUE).  Cursor bookkeeping, counter dance, and exhaust restore mirror the chain arm verbatim (same flat header quads); φ pops the 16B header only --
              * the exhausted blob already released itself -- landing exactly on the previous frontier for PAIR(1)'s jmp [rsp+0]. */
             ? x86("comment", "IR_MATCH_ARBNO_DT (PS-3 defer-tail: blob-owned frames, zero linkage)")
             + x86_alpha()
             + x86("mov", FR(_.op_off), "r14d")
             + x86("mov", FR(_.op_off + 4), "r14d")
             + x86("mov", FR(_.op_off + 8), 0L)
             + x86("mov", FRQ(_.op_off + 24), "rsp")
             + x86_gamma()
             + x86_beta()
             + x86("mov", "r14d", FR(_.op_off + 4))
             + x86("sub", "rsp", 16L)
             + x86("mov", trq(0), "r14")
             + x86("mov", trq(8), 0L)
             + x86("jmp", PAIR(0))
             + x86("def", PAIR(2))
             + x86("mov", "eax", trd(_.op_arbno_dt_susp))
             + x86("cmp", "r14d", "eax")
             + x86("je",  PAIR(1))
             + x86("mov", "eax", FR(_.op_off + 8))
             + x86("add", "eax", 1L)
             + x86("mov", FR(_.op_off + 8), "eax")
             + x86("mov", FR(_.op_off + 4), "r14d")
             + x86_gamma()
             + x86("def", PAIR(3))
             + x86("add", "rsp", 16L)
             + x86("mov", "ecx", FR(_.op_off + 8))
             + x86("test", "ecx", "ecx")
             + x86("jz",  L(2))
             + x86("sub", "ecx", 1L)
             + x86("mov", FR(_.op_off + 8), "ecx")
             + x86("jmp", PAIR(1))
             + x86("def", L(2))
             + x86("mov", "r14d", FR(_.op_off))
             + x86("mov", "rsp", FRQ(_.op_off + 24))
             + x86_omega()
         : _.op_arbno_body_kk > 0
             ? bb_match_arbno_frameless_k()   /* ⭐ ARB-LON-K16: routing decided ONCE in emit.cpp's prelude (env killswitches read there only); this arm reads nothing but the staged kk, so it precedes the slot/geometry bombs which its class never stages */
         : _.op_off < 0
             ? x86_alpha() + x86_bomb("IR_MATCH_ARBNO: slot not granted (zls)")
         : (_.op_sa < 0 || _.op_sb <= 0)
             ? x86_alpha() + x86_bomb("IR_MATCH_ARBNO: COLLECTION geometry not staged (zls_arbno_geom)")
         : (_.op_arbno_body_k0 && arbno_lon())
             ? bb_match_arbno_frameless()   /* ⭐ TOP-LEVEL-K0 CONVERGENCE (2026-08-06b): the framed requirement was scoping conservatism from the nested-SIGSEGV landing -- the arm is pure-rsp and its +16 success-out delta is billed by zd_k(ARBNO)=16 (THE ONE AUTHORITY), so the static-distance laws hold at depth 0 exactly as they hold nested.  Non-tail candidates only: op_tail wins above, keeping the carry-the-tail class on its proven arm. */
         : _.op_arbno_chain
             ? x86("comment", "IR_MATCH_ARBNO_NARY (ZB-FC-4 rsp linked-frame-chain)")
             + x86_alpha()
             + IF(arbno_rootspine(),
                   x86("sub", "rsp", 48L)                                    /* ROOT-SPINE α: 48B per-activation root record {0:anchor 4:yield 8:count 16:chainhead 24:exhaust-rsp 32:saved-___} */
                 + x86("mov", trq(32), "rsp")                                /* VIEW-SAVE into the RECORD (not a frame slot) BEFORE the repoint; L(2)'s FRQ(op_off+32) read resolves here via view0 */
                 + x86("mov", zv(), "rsp")
                 + x86("add", zv(), (long)(-_.op_off))                       /* view0 = root - op_off: every FR(op_off+X) spelling below lands [root+X]; zero downstream spelling changes */
                 + x86("lea", "rax", RDQ("rsp", 48)))                        /* pre-carve rsp: exhaust pointer-restore reclaims the root record itself */
             + x86("mov", FR(_.op_off), "r14d")
             + x86("mov", FR(_.op_off + 4), "r14d")
             + x86("mov", FR(_.op_off + 8), 0L)
             + IF(arbno_rootspine(),  x86("mov", FRQ(_.op_off + 24), "rax"))
             + IF(!arbno_rootspine(), x86("mov", FRQ(_.op_off + 24), "rsp"))
             + IF(!arbno_rootspine(), x86("mov", FRQ(_.op_off + 32), "rsp"))   /* VIEW-SAVE α (legacy FR-root): save MATCH_BEGIN ___ before beta repoints it as element view. Restored unconditionally at exhaust L(2) to re-establish callee-saved ABI ___. W-1c.2: unconditional (was IF(arbno_u2_frame())). */
             + x86("mov", FRQ(_.op_off + 16), 0L)
             + x86_gamma()
             + x86_beta()
             + x86("mov", "r14d", FR(_.op_off + 4))
             + x86("mov", "rax", FRQ(_.op_off + 16))
             + x86("sub", "rsp", (long)_.op_sb)
             + x86("mov", RSP(0), zv())
             + x86("mov", RSP(8), "r14")
             + x86("mov", RSP(16), "rax")
             + arbno_fill_window((long)_.op_sb)
             + x86("mov", FRQ(_.op_off + 16), "rsp")
             + x86("mov", zv(), "rsp")
             + x86("add", zv(), (long)(24 - _.op_sa))
             + x86("jmp", PAIR(0))
             + x86("def", PAIR(2))
             + x86("mov", "eax", FR(_.op_sa - 16))
             + x86("cmp", "r14d", "eax")
             + x86("je",  PAIR(1))
             + x86("mov", zv(), FRQ(_.op_sa - 24))
             + x86("mov", "eax", FR(_.op_off + 8))
             + x86("add", "eax", 1L)
             + x86("mov", FR(_.op_off + 8), "eax")
             + x86("mov", FR(_.op_off + 4), "r14d")
             + x86_gamma()
             + x86("def", PAIR(3))
             /* ZB-ITER-3 φ (amendment (c)+(d)): element reads FIRST (prev link, prev view — both via the live view), then ONE depth-immune lea abandons the element AND every blob cell below it —
              * rsp lands at elem+op_sb = the exact pre-push frontier, where the previous iteration's suspended body left its resume record, so the jmp PAIR(1) → defer-β `jmp [rsp+0]` composes with
              * NO resume slot.  The old `add rsp,op_sb` was frontier-correct only when φ arrived at rsp==elem; the lea off the view is correct at ANY arrival depth (the U2b self-referential spirit). */
             + x86("mov", "rax", FRQ(_.op_sa - 8))
             + x86("mov", "rdx", FRQ(_.op_sa - 24))
             + x86("lea", "rsp", RDQ(zv(), (int)(_.op_sa - 24 + _.op_sb)))
             + x86("mov", zv(), "rdx")
             + x86("mov", "ecx", FR(_.op_off + 8))
             + x86("test", "ecx", "ecx")
             + x86("jz",  L(2))
             + x86("sub", "ecx", 1L)
             + x86("mov", FR(_.op_off + 8), "ecx")
             + x86("mov", FRQ(_.op_off + 16), "rax")
             + x86("lea", zv(), RDQ("rax", 24 - _.op_sa))
             + x86("jmp", PAIR(1))
             + x86("def", L(2))
             + x86("mov", "r14d", FR(_.op_off))
             + std::string("")   /* VIEW-RESTORE exhaust: re-establish MATCH_BEGIN ___ before rsp restore; callee-saved ABI. W-1c.2: unconditional. */
             + x86("mov", "rsp", FRQ(_.op_off + 24))
             + x86_omega()
         : x86("comment", "IR_MATCH_ARBNO_NARY")
             + x86_alpha()
             + x86("mov", FR(_.op_off), "r14d")
             + x86("mov", FR(_.op_off + 4), "r14d")
             + x86("mov", FR(_.op_off + 8), 0L)
             + x86_gamma()
             + x86_beta()
             + x86("mov", "r14d", FR(_.op_off + 4))
             + x86("lea", "rdi", FR(_.op_off + 16))
             + x86("lea", "rsi", FR(_.op_off + 12))
             + x86("mov", "edx", FR(_.op_off + 8))
             + x86("mov", "rcx", (long)_.op_sb)
             + x86_align_enter()
             + x86("call", "rt_zcol_push", (uint64_t)(uintptr_t)(void *)(void * (*)(void **, int *, int, long))rt_zcol_push)
             + x86_align_leave()
             + x86("mov", RDQ("rax", 0), x86_zr())
             + x86("mov", "ecx", "r14d")
             + x86("mov", RDQ("rax", 8), "rcx")
             + x86("lea", x86_zr(), RDQ("rax", 16 - _.op_sa))
             + x86("jmp", PAIR(0))
             + x86("def", PAIR(2))
             + x86("mov", "rax", FRQ(_.op_off + 16))
             + x86("mov", "ecx", FR(_.op_off + 8))
             + x86("mov", "rdx", (long)_.op_sb)
             + x86("imul", "rcx", "rdx")
             + x86("add", "rax", "rcx")
             + x86("lea", x86_zr(), RDQ("rax", 16 - _.op_sa))
             + x86("mov", "eax", FR(_.op_sa - 8))
             + x86("cmp", "r14d", "eax")
             + x86("je",  PAIR(1))
             + x86("mov", x86_zr(), FRQ(_.op_sa - 16))
             + x86("mov", "eax", FR(_.op_off + 8))
             + x86("add", "eax", 1L)
             + x86("mov", FR(_.op_off + 8), "eax")
             + x86("mov", FR(_.op_off + 4), "r14d")
             + x86_gamma()
             + x86("def", PAIR(3))
             + x86("mov", x86_zr(), FRQ(_.op_sa - 16))
             + x86("mov", "eax", FR(_.op_off + 8))
             + x86("test", "eax", "eax")
             + x86("jz",  L(2))
             + x86("sub", "eax", 1L)
             + x86("mov", FR(_.op_off + 8), "eax")
             + x86("mov", "rax", FRQ(_.op_off + 16))
             + x86("mov", "ecx", FR(_.op_off + 8))
             + x86("mov", "rdx", (long)_.op_sb)
             + x86("imul", "rcx", "rdx")
             + x86("add", "rax", "rcx")
             + x86("lea", x86_zr(), RDQ("rax", 16 - _.op_sa))
             + x86("jmp", PAIR(1))
             + x86("def", L(2))
             + x86("mov", "r14d", FR(_.op_off))
             + x86_omega();
}
