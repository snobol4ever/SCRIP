/* bb_rk_gather.cpp — BB template for IR_GATHER (Raku `gather { take E0; take E1; ... }`).
   x86() self-encoding (template-revamp, 2026-06-02, Opus 4.8).  One file per BB kind.

   IR_GATHER is the Raku resumable Seq PRODUCER (FLAT-take model; docs.raku.org/syntax/gather%20take).
   Layout set by lower.c v_raku_gather:
       .ival    = take COUNT (n)
       .counter = IR_graph_t* array of take-payload value sub-graphs (FLAT model: each entry is IR_LIT_I)
       .state   = (unused at run time after revamp — the cursor lives in the ζ-frame at [r12+cursoff])
   Ports: γ = the yielded element delivered to the consumer (the for-loop bind); ω = Seq drained.

   ⛔ NO RESET ON α — the flat-chain wiring (codegen_flat_chain_body) routes BOTH the initial loop entry
   AND the for-loop body's re-pump back-edge to α (the BFS resolves a γ-edge to the target node's α, never
   its β).  So α must NOT reset the cursor to 0 — it must YIELD-THEN-ADVANCE from the current cursor exactly
   as the resume does.  First-entry correctness rides on the ζ-frame cursor slot being zero (the per-sequence
   ζ frame is calloc-zeroed once at entry).  β is a safety landing that re-enters the same check.  This
   matches the mode-2 oracle (bb_exec.c IR_GATHER): yield vals[cursor] then cursor++, and walking past the
   last take (cursor>=n) FAILs to ω.  SCOPE: single evaluation (the smoke + corpus shape); a gather
   re-evaluated fresh from an OUTER loop wants a true α-reset — a later refinement on this same node.

   ⛔ NO VALUE STACK — the yielded element is written as a 16-byte DESCR (DT_I tag + value) into this box's
   OWN ζ result slot [r12+resoff], which the consumer (bb_assign GZ-7) reads via bb_slot_get.  The cursor
   is a 64-bit dword at [r12+cursoff] — both pure ζ=r12 register-relative accesses, so BINARY==TEXT (no
   process-ptr movabs, no .data quad).  The take literals are READ-ONLY constants extracted at EMIT time (no
   runtime bb-graph walking — the oracle's bb_exec is NOT called) and sealed: in TEXT via a .rodata table
   reached lea[rip+label]; in BINARY via x86_load_ro's movabs of the in-process emit-side vals buffer.

   x86 arm: ONE return, pure x86() concat, NO bb_bin_t, medium invisible.  The TEXT-only .rodata vals table
   is emitted as directives BEFORE the code (guarded MEDIUM_TEXT so the BINARY record walker never sees it). */
#include <string>
#include <cstring>
#include <cstdint>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "emit.h"
int  bb_slot_claim(int bytes);
int  bb_slot_alloc16(IR_t * nd);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Per-box static cache the extern fills before bb_rk_gather_str() runs (single-threaded emitter; lives    */
/* for the duration of the emit_core dispatch call).  Pure parameterless accessors read it so the x86 arm  */
/* stays local-free / pBB-free per the template-revamp FACT RULE.                                          */
#define RK_GATHER_MAX_TAKES 256
static int64_t  s_rkg_vals[RK_GATHER_MAX_TAKES];
static int      s_rkg_n;
static uint64_t s_rkg_vals_ptr;
static char     s_rkg_lbl[64];
static int      s_rkg_cursoff;
static int      s_rkg_resoff;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int          gatherN() { return s_rkg_n; }
static inline uint64_t     valsPtr() { return s_rkg_vals_ptr; }
static inline const char * valsLbl() { return s_rkg_lbl; }
static inline int          cursoff() { return s_rkg_cursoff; }
static inline int          resoff()  { return s_rkg_resoff; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* TEXT-only .rodata vals table directives: ".Lrkg<id>_vals: .quad v0, v1, ...".  Emitted before the code  */
/* (GAS reorders by section anyway).  An empty gather emits one zero quad so the label is defined.         */
static std::string rkg_vals_rodata() {
    std::string q = std::string(valsLbl()) + ":";
    if (gatherN() > 0) {
        q += " .quad ";
        for (int i = 0; i < gatherN(); i++) q += emit_fmt("%s%lld", (i ? ", " : ""), (long long)s_rkg_vals[i]);
    } else {
        q += " .quad 0";
    }
    return s_directive(".section .rodata") + s_directive(q)
         + s_directive(".section .text") + s_directive(".intel_syntax noprefix");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_rk_gather_str() {
    if (!PLATFORM_X86) return std::string();
    return IF(MEDIUM_TEXT,
               s_1asm(std::string(_.lbl_α) + ":")
             + s_comment(emit_fmt("# BOX IR_GATHER n=%d [x86() self-encoding, ζ-frame cursor]", gatherN()))
             + rkg_vals_rodata())
    /* chk: load cursor, compare against n */
           + x86("def",    L(0))
           + x86("mov",    "rcx", FRQ(cursoff()))
           + x86("cmp64",  "rcx", (long)gatherN())
           + x86("jge",    PORT_OMEGA)
    /* load vals[cursor] into rsi (rdx = &vals; rsi = vals[rcx]) */
           + x86("lea",    "rdx", "[rip + __]", valsPtr(), valsLbl())
           + x86("mov",    "rsi", "rdx", "rcx")
    /* store DT_I DESCR at [r12+resoff]: tag=6, value=rsi */
           + x86("mov",    FRQ(resoff()), (long)6)
           + x86("mov",    FRQ(resoff() + 8), "rsi")
    /* cursor++ */
           + x86("inc",    FRQ(cursoff()))
           + x86("jmp",    PORT_GAMMA)
    /* β: re-enter chk */
           + x86("def",    PORT_BETA)
           + x86("jmp",    L(0));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_rk_gather(IR_t * pBB) {
    if (!PLATFORM_X86) { return; }
    /* Gather emit-time vals from the node (pBB is read HERE in the extern, never in the x86 _str arm).    */
    /* FLAT-take model: each take payload must be a literal int — anything else FALLS LOUD (missing-arm).  */
    int n = (int)pBB->ival;
    IR_graph_t ** subs = (IR_graph_t **)(intptr_t)pBB->counter;
    int ok = (n >= 0 && n <= RK_GATHER_MAX_TAKES && (n == 0 || subs != NULL));
    for (int i = 0; ok && i < n; i++) {
        IR_t * lf = subs[i] ? subs[i]->entry : NULL;
        if (!lf || lf->t != IR_LIT_I) { ok = 0; break; }
        s_rkg_vals[i] = lf->ival;
    }
    if (!ok) {
        fprintf(stderr, "[RK] FATAL bb_rk_gather: gather requires 0..%d literal-int take payloads (FLAT-take model); n=%d\n",
                RK_GATHER_MAX_TAKES, n);
        abort();
    }
    s_rkg_n        = n;
    s_rkg_vals_ptr = (uint64_t)(uintptr_t)(const void *)s_rkg_vals;
    snprintf(s_rkg_lbl, sizeof(s_rkg_lbl), ".Lrkg%d_vals", bb_node_id(pBB));
    /* ζ-frame slots: the RESULT DESCR (16 bytes) is NODE-KEYED (bb_slot_alloc16(pBB)) so the consumer       */
    /* (bb_assign GZ-7) recovers it via bb_slot_get(gather_node); the cursor (8 bytes, private to this box)  */
    /* is bb_slot_claim'd.  Both bump the SAME g_flat_slot_count so offsets never collide.  x86_begin() sets  */
    /* the internal-label uid for L(0) (the chk loop target).                                                */
    x86_begin();
    s_rkg_resoff  = bb_slot_alloc16(pBB);
    s_rkg_cursoff = bb_slot_claim(8);
    bb_emit_x86(bb_rk_gather_str());
}
