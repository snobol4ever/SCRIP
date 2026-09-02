#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "portcount.h"
#define PC_CHUNK 4096
#define PC_PORTS 4
static uint64_t ** g_pc_chunk; static long g_pc_nchunk;
static char     ** g_pc_name;  static long g_pc_nname;
static long        g_pc_nslot;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_port_counts_on(void) { static int p = -1; if (p < 0) { const char * e = getenv("SCRIP_PORT_COUNTS"); p = (e && *e && *e != '0') ? 1 : 0; } return p; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
uint64_t * rt_port_counts_cell(int uid, int port, const char * label)
{
    if (uid < 0 || port < 0 || port >= PC_PORTS) return NULL;
    long s = (long)uid * PC_PORTS + port, ci = s / PC_CHUNK, off = s % PC_CHUNK;
    if (ci >= g_pc_nchunk) {
        long n = ci + 1; uint64_t ** t = (uint64_t **)realloc(g_pc_chunk, (size_t)n * sizeof *t);
        if (!t) return NULL;
        g_pc_chunk = t; for (long i = g_pc_nchunk; i < n; i++) g_pc_chunk[i] = (uint64_t *)calloc(PC_CHUNK, sizeof(uint64_t));
        g_pc_nchunk = n;
    }
    if (!g_pc_chunk[ci]) return NULL;
    if (s + 1 > g_pc_nslot) g_pc_nslot = s + 1;
    if (label && *label) {
        if (s >= g_pc_nname) {
            long n = s + 1; char ** t = (char **)realloc(g_pc_name, (size_t)n * sizeof *t);
            if (t) { g_pc_name = t; for (long i = g_pc_nname; i < n; i++) g_pc_name[i] = NULL; g_pc_nname = n; }
        }
        if (s < g_pc_nname && !g_pc_name[s]) g_pc_name[s] = strdup(label);
    }
    return &g_pc_chunk[ci][off];
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
uint64_t * rt_port_counts_slot(int uid, int port, const char * label)
{
    if (!rt_port_counts_on() || (port != 0 && port != 1)) return NULL;
    { static int hooked; if (!hooked) { hooked = 1; atexit(rt_port_counts_dump); } }
    return rt_port_counts_cell(uid, port, label);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static uint64_t pc_get(long s) { if (s < 0 || s >= g_pc_nslot) return 0; long ci = s / PC_CHUNK; return (ci < g_pc_nchunk && g_pc_chunk[ci]) ? g_pc_chunk[ci][s % PC_CHUNK] : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pc_box_name(long b, char * out, size_t n)
{
    const char * l = NULL;
    for (int p = 0; p < PC_PORTS && !l; p++) { long s = b * PC_PORTS + p; if (s < g_pc_nname && g_pc_name[s]) l = g_pc_name[s]; }
    if (!l) { snprintf(out, n, "n%ld_?", b); return; }
    size_t len = strlen(l);
    if (len > 3 && l[len - 3] == '_' && (unsigned char)l[len - 2] == 0xCE) len -= 3;
    if (len >= n) len = n - 1;
    memcpy(out, l, len); out[len] = 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static double pc_ratio(uint64_t num, uint64_t den) { return den ? (double)num / (double)den : 0.0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pc_row(int four, long b, const char * nm)
{
    unsigned long long a = pc_get(b * PC_PORTS), q = pc_get(b * PC_PORTS + 1), g = pc_get(b * PC_PORTS + 2), w = pc_get(b * PC_PORTS + 3);
    if (four) fprintf(stderr, "[PORTCOUNTS] %12llu %12llu %12llu %12llu %8.3f %8.3f  %s\n", a, q, g, w, pc_ratio(q, a), pc_ratio(w, g), nm);
    else      fprintf(stderr, "[PORTCOUNTS] %14llu %14llu %8.3f  %s\n", a, q, pc_ratio(q, a), nm);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pc_csv(int four)
{
    long nbox = (g_pc_nslot + PC_PORTS - 1) / PC_PORTS;
    for (long b = 0; b < nbox; b++) {
        unsigned long long a = pc_get(b * PC_PORTS), q = pc_get(b * PC_PORTS + 1), g = pc_get(b * PC_PORTS + 2), w = pc_get(b * PC_PORTS + 3); char nm[128];
        if (!a && !q && !g && !w) continue;
        pc_box_name(b, nm, sizeof nm);
        if (four) fprintf(stderr, "[PCCSV] %llu %llu %llu %llu %s\n", a, q, g, w, nm);
        else      fprintf(stderr, "[PCCSV] %llu %llu %s\n", a, q, nm);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_port_counts_report(const char * tag, int four)
{
    if (g_pc_nslot == 0) {
        fprintf(stderr, "[PORTCOUNTS] ⛔ REFUSE: armed, but ZERO port cells were registered -- no box was instrumented. This is NOT a measurement of 'no port activity'.\n");
        return;
    }
    long nbox = (g_pc_nslot + PC_PORTS - 1) / PC_PORTS, hot = 0; uint64_t ta = 0, tb = 0, tg = 0, tw = 0;
    for (long b = 0; b < nbox; b++) {
        uint64_t a = pc_get(b * PC_PORTS); ta += a; tb += pc_get(b * PC_PORTS + 1); tg += pc_get(b * PC_PORTS + 2); tw += pc_get(b * PC_PORTS + 3);
        if (a) hot++;
    }
    fprintf(stderr, "[PORTCOUNTS] %s port entries -- boxes=%ld alpha-entered=%ld (%.1f%%) alpha-total=%llu beta-total=%llu", tag, nbox, hot,
            nbox ? 100.0 * (double)hot / (double)nbox : 0.0, (unsigned long long)ta, (unsigned long long)tb);
    if (four) fprintf(stderr, " gamma-total=%llu omega-total=%llu", (unsigned long long)tg, (unsigned long long)tw);
    fprintf(stderr, "\n[PORTCOUNTS] B/A is recedes-per-proceed (backtrack pressure)%s. Ratios are NOT speed multiples -- never an 'x'.\n",
            four ? "; W/G is concedes-per-succeed (clause tries that failed after an exit)" : "");
    if (ta == 0) {
        fprintf(stderr, "[PORTCOUNTS] ⛔ REFUSE: %ld cells registered but EVERY alpha count is zero -- the increments were emitted but never executed, or never reached the cells.\n", g_pc_nslot);
        return;
    }
    { long * ord = (long *)malloc((size_t)nbox * sizeof(long)); if (!ord) return;
      for (long b = 0; b < nbox; b++) ord[b] = b;
      for (long i = 1; i < nbox; i++) { long k = ord[i], j = i - 1; while (j >= 0 && pc_get(ord[j] * PC_PORTS) < pc_get(k * PC_PORTS)) { ord[j + 1] = ord[j]; j--; } ord[j + 1] = k; }
      if (four) fprintf(stderr, "[PORTCOUNTS] %12s %12s %12s %12s %8s %8s  %s\n", "alpha", "beta", "gamma", "omega", "B/A", "W/G", "box");
      else      fprintf(stderr, "[PORTCOUNTS] %14s %14s %8s  %s\n", "alpha", "beta", "B/A", "box");
      for (long i = 0, shown = 0; i < nbox && shown < 25; i++) {
          char nm[128];
          if (pc_get(ord[i] * PC_PORTS) == 0) break;
          pc_box_name(ord[i], nm, sizeof nm);
          pc_row(four, ord[i], nm); shown++;
      }
      free(ord); }
    if (getenv("SCRIP_PORT_COUNTS_CSV")) pc_csv(four);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
__attribute__((force_align_arg_pointer)) void rt_port_counts_dump(void)
{
    if (getenv("SCRIP_PORT_COUNTS_DIAG"))
        fprintf(stderr, "[PCDIAG] dump entered: on=%d nslot=%ld nchunk=%ld env=%s\n", rt_port_counts_on(), g_pc_nslot, g_pc_nchunk,
                getenv("SCRIP_PORT_COUNTS") ? getenv("SCRIP_PORT_COUNTS") : "<unset>");
    if (!rt_port_counts_on()) return;
    rt_port_counts_report("mode-3 EXACT", 0);
}
