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
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_bomb(const char *msg)
{
    fflush(NULL);   /* s58 DESCENT-MAP: the bomb is a measurement instrument — everything the program printed before dying must reach the pipe, or the sweep's output-prefix classification reads an empty buffer as empty output */
    fprintf(stderr, "libscrip_rt: BOMB — %s\n", msg ? msg : "(no message)");
    abort();
}
