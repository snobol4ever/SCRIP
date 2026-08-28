/* portcount.c -- EXACT per-box α/β port execution counters for MODE 3 (slice 4 route (a), Lon's port-count addendum).
 *
 * ⛔⭐⭐ THE GLOBALS BELOW ARE THE ONES LON GRANTED IN-CHAT VIA CEO ON 2026-08-28 (GOAL-CEO CEO-75, verbatim: "Yes I
 * grant the global"), after hq_P raised the mandated ⛔ NO-NEW-GLOBALS banner and RECOMMENDED HE DECLINE. He ruled
 * the other way; this is that ruling implemented. ⛔ The grant is SESSION-CITED and covers exactly this: a per-box
 * u64 port-counter array reachable from emitted mode-3 code. It is not a licence for other globals.
 *
 * WHY A GLOBAL WAS UNAVOIDABLE HERE, recorded so the grant is not re-spent by someone who thinks it was lazy:
 * a mode-3 box's `rbp` is ITS OWN activation frame, never the root, so a counter that must outlive every box
 * activation and be reachable from arbitrary nesting depth cannot ride the stack without a chain walk (far too
 * expensive on a port executed millions of times), a pinned register (the "pinned VA slot" the law names by name --
 * and r10/r11 are the wires), or an address baked at emission (at which point it is not on the stack anyway).
 * ⛔ ceo's ζ-STANDING root-frame shape was evaluated and does NOT exist: ZDP_TIER_STANDING has three references in
 * the whole tree, no allocator and no accessor -- it CLASSIFIES which nodes need match-lifetime storage and its one
 * real consumer carves that slot in the ACTIVATION frame. It is a TIER, not a REGION.
 *
 * ⭐ WHY THE STORAGE IS CHUNKED AND NEVER REALLOC'd: the emitter resolves a cell's ADDRESS at EMISSION time and bakes
 * it into the instruction stream as an absolute immediate. A realloc that moved the array would leave every already
 * emitted box incrementing freed memory -- silently, and only for the boxes emitted before the growth. Chunks are
 * allocated once and never moved, so a baked address is valid for the life of the process. The NAME table may move
 * freely: it is read only at dump time, never by emitted code.
 *
 * ⛔ TELEMETRY DEFAULTS OFF, and that is not a violation of Instrument Law 1. Law 1 governs a CURE behind a flag --
 * a default-OFF cure is a deletion with a comment. This is not a cure; it is an instrument that PERTURBS what it
 * measures (clause 6), so arming it by default would make every shipped binary pay for a measurement nobody asked
 * for. Absent when off: with SCRIP_PORT_COUNTS unset the emitter adds no instruction at all.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "portcount.h"
#define PC_CHUNK 4096
static uint64_t ** g_pc_chunk; static long g_pc_nchunk;                        /* GRANTED: the counter array, chunked so cells never move */
static char     ** g_pc_name;  static long g_pc_nname;                         /* GRANTED: parallel label table, dump-time only */
static long        g_pc_nslot;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_port_counts_on(void) { static int p = -1; if (p < 0) { const char * e = getenv("SCRIP_PORT_COUNTS"); p = (e && *e && *e != '0') ? 1 : 0; } return p; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
uint64_t * rt_port_counts_slot(int uid, int port, const char * label)
{
    if (!rt_port_counts_on() || uid < 0 || (port != 0 && port != 1)) return NULL;
    long s = (long)uid * 2 + port, ci = s / PC_CHUNK, off = s % PC_CHUNK;
    if (ci >= g_pc_nchunk) {
        long n = ci + 1; uint64_t ** t = (uint64_t **)realloc(g_pc_chunk, (size_t)n * sizeof *t);
        if (!t) return NULL;
        g_pc_chunk = t; for (long i = g_pc_nchunk; i < n; i++) g_pc_chunk[i] = (uint64_t *)calloc(PC_CHUNK, sizeof(uint64_t));
        g_pc_nchunk = n;
    }
    if (!g_pc_chunk[ci]) return NULL;
    /* ⛔⭐⭐ atexit, NOT a call site in the driver -- MEASURED, and it is why the BBPROF bones have never reported.
       scrip.c's `run_done:` label looks like the mode-3 exit and is NOT: an emitted SNOBOL4 program terminates the
       PROCESS itself, so control never returns to the driver and everything hung on run_done is dead code that
       reads as live. `bbprof_report()` has sat on that label the whole time, which is the real cause of the
       "never records" defect -- it records fine, it just never gets to say so. ⭐ A teardown hook placed after a
       call that never returns is indistinguishable, in the source, from one that runs. */
    { static int hooked; if (!hooked) { hooked = 1; atexit(rt_port_counts_dump); } }
    if (s + 1 > g_pc_nslot) g_pc_nslot = s + 1;
    if (label && *label) {
        if (s >= g_pc_nname) { long n = s + 1; char ** t = (char **)realloc(g_pc_name, (size_t)n * sizeof *t); if (t) { g_pc_name = t; for (long i = g_pc_nname; i < n; i++) g_pc_name[i] = NULL; g_pc_nname = n; } }
        if (s < g_pc_nname && !g_pc_name[s]) g_pc_name[s] = strdup(label);
    }
    return &g_pc_chunk[ci][off];
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static uint64_t pc_get(long s) { if (s < 0 || s >= g_pc_nslot) return 0; long ci = s / PC_CHUNK; return (ci < g_pc_nchunk && g_pc_chunk[ci]) ? g_pc_chunk[ci][s % PC_CHUNK] : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* pc_box_name: the stored label is the full port label ("n12_match_begin_α"); strip the trailing "_<greek>" so α and
   β rows share one box name. ⛔ α and β are LITERAL UTF-8 GREEK -- two bytes each, so the suffix is THREE bytes, not
   two. An ASCII-width assumption here truncates the family name by one byte and prints a plausible wrong label
   (Instrument Law 15: an ASCII regex over UTF-8-labelled emission is not a narrower measurement, it is a wrong one). */
static void pc_box_name(long s, char * out, size_t n)
{
    const char * l = (s < g_pc_nname && g_pc_name[s]) ? g_pc_name[s] : NULL;
    if (!l) { snprintf(out, n, "n%ld_?", s / 2); return; }
    size_t len = strlen(l);
    if (len > 3 && l[len - 3] == '_' && (unsigned char)l[len - 2] == 0xCE) len -= 3;
    if (len >= n) len = n - 1;
    memcpy(out, l, len); out[len] = 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_port_counts_dump(void)
{
    if (getenv("SCRIP_PORT_COUNTS_DIAG")) fprintf(stderr, "[PCDIAG] dump entered: on=%d nslot=%ld nchunk=%ld env=%s\n", rt_port_counts_on(), g_pc_nslot, g_pc_nchunk, getenv("SCRIP_PORT_COUNTS") ? getenv("SCRIP_PORT_COUNTS") : "<unset>");
    if (!rt_port_counts_on()) return;
    /* ⛔ INSTRUMENT LAW 2: armed-and-empty must NOT share an output shape with armed-and-measured. An empty port
       table is not a measurement of "no port activity" -- it means nothing was instrumented, and saying so is the
       whole point of refusing rather than printing a tidy table of zeros. */
    if (g_pc_nslot == 0) { fprintf(stderr, "[PORTCOUNTS] ⛔ REFUSE: armed, but ZERO port cells were registered -- no box was instrumented. This is NOT a measurement of 'no port activity'.\n"); return; }
    long nbox = (g_pc_nslot + 1) / 2, hot = 0; uint64_t ta = 0, tb = 0;
    for (long b = 0; b < nbox; b++) { uint64_t a = pc_get(b * 2); ta += a; tb += pc_get(b * 2 + 1); if (a) hot++; }
    fprintf(stderr, "[PORTCOUNTS] mode-3 EXACT port entries -- boxes=%ld alpha-entered=%ld (%.1f%%) alpha-total=%llu beta-total=%llu\n",
            nbox, hot, nbox ? 100.0 * (double)hot / (double)nbox : 0.0, (unsigned long long)ta, (unsigned long long)tb);
    fprintf(stderr, "[PORTCOUNTS] B/A is recedes-per-proceed (backtrack pressure). It is NOT a speed multiple -- never an 'x'.\n");
    if (ta == 0) { fprintf(stderr, "[PORTCOUNTS] ⛔ REFUSE: %ld cells registered but EVERY alpha count is zero -- the increments were emitted but never executed, or never reached the cells.\n", g_pc_nslot); return; }
    { long * ord = (long *)malloc((size_t)nbox * sizeof(long)); if (!ord) return;
      for (long b = 0; b < nbox; b++) ord[b] = b;
      for (long i = 1; i < nbox; i++) { long k = ord[i], j = i - 1; while (j >= 0 && pc_get(ord[j] * 2) < pc_get(k * 2)) { ord[j + 1] = ord[j]; j--; } ord[j + 1] = k; }
      fprintf(stderr, "[PORTCOUNTS] %14s %14s %8s  %s\n", "alpha", "beta", "B/A", "box");
      for (long i = 0, shown = 0; i < nbox && shown < 25; i++) {
          uint64_t a = pc_get(ord[i] * 2), b = pc_get(ord[i] * 2 + 1); char nm[128];
          if (a == 0) break;
          pc_box_name(ord[i] * 2, nm, sizeof nm);
          fprintf(stderr, "[PORTCOUNTS] %14llu %14llu %8.3f  %s\n", (unsigned long long)a, (unsigned long long)b, (double)b / (double)a, nm); shown++;
      }
      free(ord); }
    /* machine-comparable tail: one line per box, so an m3 run can be diffed against scripts/util_port_counts.py's m4 table */
    if (getenv("SCRIP_PORT_COUNTS_CSV")) for (long b = 0; b < nbox; b++) { uint64_t a = pc_get(b * 2), q = pc_get(b * 2 + 1); char nm[128]; if (!a && !q) continue; pc_box_name(b * 2, nm, sizeof nm); fprintf(stderr, "[PCCSV] %llu %llu %s\n", (unsigned long long)a, (unsigned long long)q, nm); }
}
