#include "rt/rt.h"
#include "dtp.h"
#include "core.h"
#include "builtins/resolution.h"
#include "bb_pool.h"
#include "../parser/prolog/prolog_atom.h"
#include <stdio.h>
#include <stdlib.h>
static int     g_halt_rc  = 0;
static int     g_halt_set = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
__attribute__((visibility("hidden"))) unsigned long g_zdp_anchor_rsp = 0UL;   /* HIDDEN so rtx_zdp.S can reach it PC-relative inside the .so — the same reason RTX_GATE_DEF marks its own cells .hidden; without it the R_X86_64_PC32 relocation is rejected for a shared object.  ⛔ NEW GLOBAL, GRANTED IN-CHAT BY LON s135 ("go for the global since it is easier and this is all temporary until we get to 100%"). THE ANCHOR DATUM: rsp observed at the FIRST anchor of the run. Lon's model s135: an anchor is a point where the graph's stack is TOTALLY EMPTY, so every anchor must observe the SAME rsp — STATEMENT_BEGIN and MATCH_BEGIN are the re-basing points. Written/read ONLY by rt_zdp_anchor below, ONLY under SCRIP_ZDP_TEARDOWN=1. NEVER consulted by codegen: it is a reporter, never an input to a decision, so no emitted byte depends on it. Deleted at 100% per the grant. */
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
__attribute__((visibility("hidden"))) unsigned long g_zdp_anchor_rbp = 0UL;   /* ⛔⭐⭐⭐ NEW GLOBAL — BANNER ASK PENDING (s136).  The α-side bank for LON'S RBP-EQUALITY CHECK ("have BB's that use RBP check at BETA for equality to saved RBP").  HIDDEN for the same PC-relative reason g_zdp_anchor_rsp is.  Exists ONLY under SCRIP_ZDP_TEARDOWN=1 (default OFF, byte-identical); NOT COMMITTED until Lon grants it in-chat, per the NO-NEW-GLOBALS FACT RULE. */
void rt_zdp_probe_report(unsigned long rsp, long op, long node, long expected, long portflags, long measured)
{   /* ⭐⭐⭐ LON'S EVERY-PORT PROBE (s136), THE COLD SINK — LOG, NOT TRAP (s134 staging law: "a trap-first instrument dies on program #1 and tells you about exactly one site; the log tells you about all of them").  The AGREEING path never reaches C: rtx_zdp.S compares in hand asm and returns having touched rax and rflags only. */
    const char * port = (portflags & 1) ? "alpha" : "beta";
    if (expected == -1L) { fprintf(stderr, "[ZDP-TOP] port=%s op=%ld node=%ld measured=%ld rsp=%lu\n", port, op, node, measured, rsp); return; }   /* TOP RECORD: the lattice refused a prediction.  CONSTANT across the census = PRECISION bug (refusing the spine for nothing); VARYING = the TOP was earned.  Rows 3/4 of Lon's mutual-validation table. */
    if (measured != expected) { fprintf(stderr, "[ZDP-RSP] port=%s op=%ld node=%ld expected=%ld measured=%ld skew=%ld\n", port, op, node, expected, measured, measured - expected); return; }   /* SOUNDNESS: the transfer function mis-calculated, or the box leaked / over-freed. */
    fprintf(stderr, "[ZDP-RBP] port=%s op=%ld node=%ld rbp_mismatch (rsp agreed at delta=%ld)\n", port, op, node, measured);   /* rsp agreed and we still got here => the only remaining caller is the rbp-equality arm: the activation's frame did not come back the way it went in. */
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_zdp_report(unsigned long rsp, long op, long node, unsigned long expected)
{   /* ⭐ LON'S TEARDOWN EQUATION, RUNTIME HALF (s134 spec, s135 anchor model) — THE COLD PATH ONLY. The AGREEING path never reaches C at all: rtx_zdp.S compares in hand asm and returns, touching nothing but rax/rflags, so an anchor that is sound costs the measured program ZERO perturbation (Lon s135 "use asm not C" — a C call would force the whole SysV caller-saved clobber set, which includes the r10/r11 γ/ω WIRES and the r8/r9 RTCC slots, and RULES.md forbids scratching those even in RTX hand asm). This sink runs ONLY where the anchor already disagreed, i.e. at a site that is already a defect, and the asm caller saves the full caller-saved set around it so even the reporting path cannot corrupt the run. LOG MODE, NOT A TRAP (s134 build spec: "a trap-first instrument dies on program #1 and tells you about exactly one site; the log tells you about all of them"). PRE-REGISTERED PREDICTION (static half, 654 programs): IR_STATEMENT_BEGIN 3933 · IR_MATCH_BEGIN 5 · IR_STATEMENT_END 0. */
    fprintf(stderr, "[ZDPANCHOR] op=%ld node=%ld expected=%lu actual=%lu delta=%ld\n", op, node, expected, rsp, (long)(expected - rsp));   /* delta>0 = the anchor was reached with the stack NOT empty: bytes carved and never released — a box whose ω path did not restore the frontier it found, which is zdp_out_omega's own named bug class. delta<0 = over-free. */
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define ZDP_RBP_TAB_N 65536
__attribute__((visibility("hidden"))) unsigned long g_zdp_rbp_tab[ZDP_RBP_TAB_N * 2];   /* ⛔ NEW GLOBAL, GRANTED IN-CHAT THIS SESSION (Lon: "store/save RBP on ALPHA, check RBP on BETA").  ZDP-RBP-TAB, NODE-KEYED: pairs {node, banked_rbp} at index (node & 65535)*2, so a beta is compared ONLY against ITS OWN node's alpha.  ⛔ THIS REPLACES THIS RUNG'S OWN FIRST DESIGN, A GLOBAL LIFO STACK, WHICH WAS WRONG AND WHOSE NUMBERS ARE VOID: a LIFO assumes alpha and beta pair 1:1, but a box runs alpha ONCE, exits through gamma, and its beta then re-fires MANY times on backtrack -- pushes and pops never balance, the stack drains, and the comparisons that survive are a beta against SOME OTHER box's banked alpha.  Measured under that design on beauty: 38 "MISMATCH" and 1437 "UNDERFLOW" -- the underflow is the 1-alpha-to-N-beta arity, NOT evidence of a beta reached without an alpha, and the mismatches are cross-box noise.  Node-keying removes both artifacts: no cross-box pairing can occur, and a beta whose node was never banked is now a REAL signal rather than a drained-stack artifact.  KNOWN LIMIT, NAMED NOT DISCOVERED: recursion and hash collision both alias one slot -- the innermost/last alpha wins -- so this instrument under-reports on recursive activations and cannot be read as a total census.  HIDDEN for the PC-relative reason g_zdp_anchor_rsp is.  Written/read ONLY by rt_zdp_rbp_bank/rt_zdp_rbp_check (hand asm, rtx_zdp.S) under SCRIP_ZDP_RBP_CANARY=1.  NEVER consulted by codegen. */
unsigned long g_zdp_rbp_violations = 0UL;
unsigned long g_zdp_rbp_nobank     = 0UL;
void rt_bomb(const char *msg);   /* forward decl -- defined below; the mismatch arm below needs it ahead of its own definition */
void rt_zdp_rbp_report(long node, unsigned long expected, unsigned long actual, long kind)
{   /* ⭐ ZDP-RBP, THE COLD SINK — CHECK-AND-ABORT AT THE FIRST VIOLATION (Lon in-chat, this session).  ⛔ THE PORT MODEL WAS CORRECTED BY LON IN-CHAT AND THIS RUNG'S EARLIER NUMBERS ARE VOID: the activation frame's lifetime is α→ω, NOT α→β.  β is an ENTRY port -- control arrives there to RETRY and may go on to γ -- so a box standing at β is SUPPOSED to still hold its frame; it is the β→ω transition that tears down.  Checking rbp-equality at β therefore FAILED CORRECTLY-WORKING BOXES.  The three arms below are the corrected model: kind=0 β lost its frame · kind=1 ω did not restore the balance (the real teardown test, never instrumented before this edit because the x86_deflabel seam only reaches label DEFINES = α/β, while γ/ω are TRANSFERS) · kind=2 no bank.  0 and 1 abort; 2 logs, since it is the empirical test of "β before α is impossible" and one occurrence must not mask the census. */
    if (kind == 2) { g_zdp_rbp_nobank++; fprintf(stderr, "[ZDP-RBP] node=%ld NO-BANK (this node's alpha never ran, or its slot was aliased by recursion/collision)\n", node); return; }
    g_zdp_rbp_violations++;
    { char b[256];
      if (kind == 1) snprintf(b, sizeof b, "ZDP-RBP node=%ld OMEGA IMBALANCE: alpha banked enclosing rbp=0x%lx; at omega (teardown) rbp=0x%lx -- this box left without restoring the frame it found", node, expected, actual);
      else           snprintf(b, sizeof b, "ZDP-RBP node=%ld BETA FRAME LOST: alpha banked enclosing rbp=0x%lx; at beta rbp=0x%lx whose saved parent is not that value -- the activation this box is standing in is not the one its alpha established", node, expected, actual);
      rt_bomb(b); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ⭐⭐⭐ ZETA SM — THE SELF-MEASURING MONITOR (Lon in-chat, this session: "build a monitor harness around the BB
 * execution ... a state machine that as the reports come in through callbacks from alpha and beta in each box
 * you can trace the execution ... A system that measures itself").
 *
 * ⛔ WHY THE DATUM IS GLOBAL AND NOT FRAME-RESIDENT (Lon's correction, and it is decisive): an earlier cut of
 * this rung banked the frame identity at [rbp-8] and re-read it at beta.  That is circular -- if rbp is WRONG
 * at beta then the memory it addresses is precisely what cannot be trusted, so the check needs rbp to already
 * be right in order to discover that it is not.  Chicken and egg.  The shadow state therefore lives HERE, in
 * side storage keyed by NODE ID, touched only by this instrument and never addressed through rbp.
 *
 * ⛔ AND THE PORT MODEL IS LON'S, NOT THIS RUNG'S EARLIER GUESS: the activation's lifetime is alpha..omega.
 * BETA IS AN ENTRY PORT -- it may go on to gamma -- so at beta the box is SUPPOSED to still hold its frame;
 * it is the transition OUT to omega that tears down.  Every number this rung reported before that correction
 * is VOID (they were measured with an equality test at beta, which convicts correctly-working boxes).
 *
 * STATE PER LIVE ACTIVATION, established at alpha, keyed by node:
 *     E  = rbp observed at alpha   -- the ENCLOSING activation, what omega must give back
 *     F  = the frame this node establishes = (rsp at alpha) - 8, because the template's first act is
 *          `push rbp` (rsp -= 8) then `mov rbp,rsp`.  ⛔ F IS A PREDICTION, NOT A READING, and it is
 *          VALIDATED BY THE FALSE-POSITIVE GATE, never asserted: if the prediction is wrong the
 *          known-PASSING probes light up, which is the whole point of running them first.
 * CHECKS:  beta  -> an activation for this node must be LIVE (a beta with no alpha is the "impossible"
 *                   case Lon named; if it is truly impossible this counter reads zero) AND rbp == F.
 *          omega -> rbp == E (balance restored) and rsp >= rsp_at_alpha (frame actually released).
 * KNOWN LIMIT, NAMED NOT DISCOVERED: one slot per (node & 65535), so RECURSION and hash collision alias --
 * the innermost alpha wins and this instrument UNDER-reports on recursive activations.  It is not a census.
 */
#define ZSM_N 65536
#define ZSM_TRACE 64
typedef struct { unsigned long node, E, F, rsp_a, live; } zsm_ent;
static zsm_ent      g_zsm[ZSM_N];
static unsigned long g_zsm_tr_node[ZSM_TRACE], g_zsm_tr_rbp[ZSM_TRACE], g_zsm_tr_rsp[ZSM_TRACE];
static long          g_zsm_tr_kind[ZSM_TRACE], g_zsm_tr_depth[ZSM_TRACE];
static unsigned long g_zsm_tr_n = 0UL;
unsigned long g_zsm_violations = 0UL, g_zsm_beta_no_alpha = 0UL, g_zsm_events = 0UL;
static unsigned long g_zsm_rsp0 = 0UL;   /* ⭐ RSP0 — THE GRAPH-ENTRY DATUM (Lon in-chat: "physically measure RSP minus RSP-saved at graph entry so that you can debug your offset problems").  Stamped by kind=0 at each graph's first BB.  ⛔ THIS IS A MEASUREMENT, NOT A VERDICT: nothing is compared against a predicted value, so it can never go ⊤ the way the s136 lattice probe does (11720 ⊤ on beauty = the lattice declining to predict), and it never runs zdp_analyze, so it cannot inherit that pass's measured perturbation (FINDING s136: SCRIP_ZDP=1 alone moves 81 of 656 programs).  KNOWN LIMIT, Lon's own ruling s136 verbatim: "I know RSP changes into a function but just ignore all that" -- one cell, so a nested/recursive graph's origin overwrites its caller's and the caller's remaining ports read against the callee's datum; that noise is bounded to post-call ports and is READ AS NOISE. */
static int zsm_census(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_ZSM_CENSUS"); v = (e && *e == '1') ? 1 : 0; } return v; }   /* census mode: print EVERY port with its measured depth, agreements included -- s135's lesson that an instrument silent both when it agrees and when it never ran cannot be told from a dead one. */
void rt_bomb(const char *msg);
static const char * zsm_kn(long k) { return k == 0 ? "ORIGIN" : k == 1 ? "α" : k == 2 ? "β" : k == 3 ? "ω" : "?"; }
static void zsm_dump(void)
{   /* the trace ring: the last ZSM_TRACE ports in execution order, so a violation arrives WITH the path that produced it rather than as a bare coordinate. */
    unsigned long n = g_zsm_tr_n < ZSM_TRACE ? g_zsm_tr_n : ZSM_TRACE, i, first = g_zsm_tr_n < ZSM_TRACE ? 0 : g_zsm_tr_n % ZSM_TRACE;
    fprintf(stderr, "[ZSM] --- last %lu ports (oldest first), event #%lu ---\n", n, g_zsm_events);
    for (i = 0; i < n; i++) { unsigned long j = (first + i) % ZSM_TRACE;
        fprintf(stderr, "[ZSM]   %-6s node=%-8lu rbp=0x%lx rsp=0x%lx depth=%ld\n", zsm_kn(g_zsm_tr_kind[j]), g_zsm_tr_node[j], g_zsm_tr_rbp[j], g_zsm_tr_rsp[j], g_zsm_tr_depth[j]); }
}
void rt_zdp_sm_event(unsigned long node, unsigned long rbp, unsigned long rsp, long kind)
{
    zsm_ent * e = &g_zsm[node & (ZSM_N - 1)];
    unsigned long t = g_zsm_tr_n % ZSM_TRACE;
    long depth = g_zsm_rsp0 ? (long)(g_zsm_rsp0 - rsp) : 0L;   /* ⭐ THE PHYSICAL MEASUREMENT: bytes carved below this graph's entry frontier and still standing at this port.  Reported raw; no expected value is consulted. */
    g_zsm_tr_node[t] = node; g_zsm_tr_rbp[t] = rbp; g_zsm_tr_rsp[t] = rsp; g_zsm_tr_kind[t] = kind; g_zsm_tr_depth[t] = depth; g_zsm_tr_n++; g_zsm_events++;
    if (zsm_census()) fprintf(stderr, "[ZSM-DEPTH] %-6s node=%-8lu depth=%ld rsp=0x%lx rbp=0x%lx\n", zsm_kn(kind), node, depth, rsp, rbp);
    if (kind == 0) { g_zsm_rsp0 = rsp; return; }   /* graph entry: establish the datum.  Each graph RE-BASES, so a nested graph legitimately overwrites it. */
    if (kind == 1) { e->node = node; e->E = rbp; e->F = rsp - 8UL; e->rsp_a = rsp; e->live = 1UL; return; }
    if (e->node != node || !e->live) {   /* β or ω with no live activation for this node */
        if (kind == 2) { g_zsm_beta_no_alpha++; fprintf(stderr, "[ZSM] β node=%lu WITH NO LIVE α (rbp=0x%lx rsp=0x%lx) -- claimed impossible; counted, not fatal, so one occurrence cannot mask the census\n", node, rbp, rsp); }
        return;
    }
    if (kind == 2) {
        if (rbp != e->F) { g_zsm_violations++; zsm_dump();
            { char b[256]; snprintf(b, sizeof b, "ZSM β node=%lu FRAME LOST: α established F=0x%lx (E=0x%lx); at β rbp=0x%lx skew=%ld", node, e->F, e->E, rbp, (long)(rbp - e->F)); rt_bomb(b); } }
        return;
    }
    if (kind == 3) {
        e->live = 0UL;
        if (rbp != e->E) { g_zsm_violations++; zsm_dump();
            { char b[256]; snprintf(b, sizeof b, "ZSM ω node=%lu IMBALANCE: α banked enclosing E=0x%lx; at ω rbp=0x%lx skew=%ld -- teardown did not give the frame back", node, e->E, rbp, (long)(rbp - e->E)); rt_bomb(b); } }
        if (rsp < e->rsp_a) { g_zsm_violations++; zsm_dump();
            { char b[256]; snprintf(b, sizeof b, "ZSM ω node=%lu RSP LEAK: α rsp=0x%lx; at ω rsp=0x%lx still %ld bytes low -- carved and never released", node, e->rsp_a, rsp, (long)(e->rsp_a - rsp)); rt_bomb(b); } }
    }
}
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_bomb(const char *msg)
{
    fflush(NULL);   /* s58 DESCENT-MAP: the bomb is a measurement instrument — everything the program printed before dying must reach the pipe, or the sweep's output-prefix classification reads an empty buffer as empty output */
    fprintf(stderr, "libscrip_rt: BOMB — %s\n", msg ? msg : "(no message)");
    abort();
}
