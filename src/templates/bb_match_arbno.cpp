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
/* ZB-ITER-3 (s85): zv() = the chain arm's ELEMENT VIEW register, named by the scheme itself — rbp under ZC_FRAME_RSP (the s83-measured borrow, statement-bracketed by the head's +40 save), x86_zr()
 * elsewhere (legacy byte-identity).  Decoupling the view from the op_anchored delivery makes the REG-7 U4 zr-arm flip a NO-OP for this template: the chain dance never consults the window predicate. */
static inline const char * zv() { return ZC_FRAME == ZC_FRAME_RSP ? "rbp" : x86_zr(); }
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
static std::string bb_match_arbno_tail() {
    /* R12-EXIT-1 CARRY-THE-TAIL (Lon s68 static-size proof; design of record at zeta_storage.c fc_tail_*).  Element = [0,span) body window + [span,span+rspan) right-spine window + 16B header
     * {entry-cursor@+0, yield-cursor@+4, elem0-flag@+8} + 16B bracket copy {patstk mark@+16, rsp mark@+24} + one 16B WRAP-CAPTURE slot per ARBNO(body).V pair at [HDRB+32+16j] (delta at slot+0, alpha
     * copies pair j from its flat SAVE cell at [KA+16j], beta carries the current element's slot forward, the wrap COND reads [HDRA+32+16j] at the uniform yield depth -- fcc-registered at finalize,
     * zero capture-template change); op_sb = align16(span+rspan+32+16*ncap); HDRB = span+rspan (elem-relative header base, staged in
     * op_sa); HDRA = fp_body + HDRB (header base from the UNIFORM yield depth rsp = elem - fp_body, which alpha's phantom body pad establishes for the epsilon yield and S10c gamma-suspension
     * maintains for every extension).  alpha pushes op_sb+fp_body copying the bracket from HEAD's cell at [rsp + KA + fp_left + k]; beta pushes op_sb copying it from the current element; the
     * fail-glue pop `add rsp, op_sb` lands EXACTLY on the previous element's yield frontier (LIFO + fixed size = arithmetic, never indirection); exhaust pops op_sb ONLY (the resumed-epsilon cascade's
     * box-omega pops already consumed the FPB phantom pad -- popping KA here overshot flat by FPB, the s71 measured SEGV: oracle-identical trace then exit 139 at first FPB>0 exhaust) and omega runs at flat depth.
     * Every reference is [rsp + compile-time-const] -- no view register, no dynamic count in any address. */
    /* L2 FENCE SEAL (s71; the flat path's PAIR(1)->na_f re-aim with the element scheme's depth fixed): resume a committed iteration ABANDONS it.  Every PAIR(1) departure in this template -- the sigma
     * null-progress je and phi's post-pop resume -- runs at the UNIFORM yield depth rsp = elem - FPB (the alpha phantom pad + S10c suspension invariant), and external resume routes to this box's beta
     * (extend), never PAIR(1); so the whole seal is ONE glue: L(3) arithmetic-pops the dead suspended body (its alternatives are forbidden -- skipping the sealed boxes' omegas IS the seal) and falls
     * through into phi, whose elem0 dance then cascades pop-by-pop to exhaust exactly as SPITBOL cuts left (manual ln 4716).  The epsilon element exits at the flag check before any body code runs. */
    int HDRB = _.op_sa, FPB = _.op_tail_fpb, FPL = _.op_tail_fpl, SEAL = _.op_tail_seal, NCAP = _.op_tail_ncap;
    int KA = (int)_.op_sb + FPB, HDRA = FPB + HDRB;
    return x86("comment", "IR_MATCH_ARBNO_TAIL (R12-EXIT-1 carry-the-tail rsp elements)")
         + x86_alpha()
         + x86("sub", "rsp", (long)KA)
         + x86("mov", "eax", 0L)
         + tail_zero(FPB, FPB + HDRB, "rax")
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
         + x86("mov", "eax", trd(HDRA + 0))
         + x86("cmp", "r14d", "eax")
         + x86("je",  SEAL ? L(3) : PAIR(1))
         + x86("mov", trd(HDRA + 4), "r14d")
         + x86_gamma()
         + IF(SEAL, x86("def", L(3)) + IF(FPB > 0, x86("add", "rsp", (long)FPB)))
         + x86("def", PAIR(3))
         + x86("mov", "eax", trd(HDRB + 8))
         + x86("test", "eax", "eax")
         + x86("jnz", L(2))
         + x86("add", "rsp", (long)_.op_sb)
         + x86("jmp", SEAL ? L(3) : PAIR(1))
         + x86("def", L(2))
         + x86("mov", "r14d", trd(HDRB + 0))
         + x86("add", "rsp", (long)_.op_sb)
         + x86_omega();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_arbno() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    return _.op_tail
             ? bb_match_arbno_tail()
         : _.op_off < 0
             ? x86_alpha() + x86_bomb("IR_MATCH_ARBNO: slot not granted (zls)")
         : (_.op_sa < 0 || _.op_sb <= 0)
             ? x86_alpha() + x86_bomb("IR_MATCH_ARBNO: COLLECTION geometry not staged (zls_arbno_geom)")
         : _.op_arbno_chain
             ? x86("comment", "IR_MATCH_ARBNO_NARY (ZB-FC-4 rsp linked-frame-chain)")
             + x86_alpha()
             + x86("mov", FR(_.op_off), "r14d")
             + x86("mov", FR(_.op_off + 4), "r14d")
             + x86("mov", FR(_.op_off + 8), 0L)
             + x86("mov", FRQ(_.op_off + 24), "rsp")
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
