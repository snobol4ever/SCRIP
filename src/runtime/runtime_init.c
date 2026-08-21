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
__attribute__((visibility("hidden"))) unsigned long g_zdp_anchor_rsp = 0UL;
__attribute__((visibility("hidden"))) unsigned long g_zdp_anchor_rbp = 0UL;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_zdp_probe_report(unsigned long rsp, long op, long node, long expected, long portflags, long measured)
{
    const char * port = (portflags & 1) ? "alpha" : "beta";
    if (expected == -1L) { fprintf(stderr, "[ZDP-TOP] port=%s op=%ld node=%ld measured=%ld rsp=%lu\n", port, op, node, measured, rsp); return; }
    if (measured != expected) { fprintf(stderr, "[ZDP-RSP] port=%s op=%ld node=%ld expected=%ld measured=%ld skew=%ld\n", port, op, node, expected, measured, measured - expected); return; }
    fprintf(stderr, "[ZDP-RBP] port=%s op=%ld node=%ld rbp_mismatch (rsp agreed at delta=%ld)\n", port, op, node, measured);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_zdp_report(unsigned long rsp, long op, long node, unsigned long expected)
{
    fprintf(stderr, "[ZDPANCHOR] op=%ld node=%ld expected=%lu actual=%lu delta=%ld\n", op, node, expected, rsp, (long)(expected - rsp));
}
#define ZDP_RBP_TAB_N 65536
__attribute__((visibility("hidden"))) unsigned long g_zdp_rbp_tab[ZDP_RBP_TAB_N * 2];
unsigned long g_zdp_rbp_violations = 0UL;
unsigned long g_zdp_rbp_nobank     = 0UL;
void rt_bomb(const char *msg);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_zdp_rbp_report(long node, unsigned long expected, unsigned long actual, long kind)
{
    if (kind == 2) { g_zdp_rbp_nobank++; fprintf(stderr, "[ZDP-RBP] node=%ld NO-BANK (this node's alpha never ran, or its slot was aliased by recursion/collision)\n", node); return; }
    g_zdp_rbp_violations++;
    { char b[256];
      if (kind == 1) snprintf(b, sizeof b, "ZDP-RBP node=%ld OMEGA IMBALANCE: alpha banked enclosing rbp=0x%lx; at omega (teardown) rbp=0x%lx -- this box left without restoring the frame it found", node, expected, actual);
      else           snprintf(b, sizeof b, "ZDP-RBP node=%ld BETA FRAME LOST: alpha banked enclosing rbp=0x%lx; at beta rbp=0x%lx whose saved parent is not that value -- the activation this box is standing in is not the one its alpha established", node, expected, actual);
      rt_bomb(b); }
}
#define ZSM_N 65536
#define ZSM_TRACE 2048
enum { ZSM_FRESH = 0, ZSM_LIVE = 1, ZSM_SUSPENDED = 2, ZSM_RESUMED = 3, ZSM_DEAD = 4 };
typedef struct { unsigned long node, E, F, rsp_a, live; int state; } zsm_ent;
static zsm_ent      g_zsm[ZSM_N];
static unsigned long g_zsm_tr_node[ZSM_TRACE], g_zsm_tr_rbp[ZSM_TRACE], g_zsm_tr_rsp[ZSM_TRACE];
static long          g_zsm_tr_kind[ZSM_TRACE], g_zsm_tr_depth[ZSM_TRACE], g_zsm_tr_stno[ZSM_TRACE];
static unsigned long g_zsm_tr_n = 0UL;
unsigned long g_zsm_violations = 0UL, g_zsm_beta_no_alpha = 0UL, g_zsm_events = 0UL;
unsigned long g_zsm_fsm_illegal = 0UL, g_zsm_gamma_events = 0UL, g_zsm_leaked_at_exit = 0UL, g_zsm_alpha_while_live = 0UL;
static unsigned long g_zsm_rsp0 = 0UL;
static unsigned long g_zsm_seen_nodes[ZSM_N];
static unsigned long g_zsm_seen_n = 0UL;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zsm_census(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_ZSM_CENSUS"); v = (e && *e == '1') ? 1 : 0; } return v; }
static int zsm_leak_report_on(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_ZSM_LEAK_REPORT"); v = (e && *e == '1') ? 1 : 0; } return v; }
static int zsm_overpop_on(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_ZSM_OVERPOP"); v = (e && *e == '1') ? 1 : 0; } return v; }
void rt_bomb(const char *msg);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * zsm_kn(long k) { return k == 0 ? "ORIGIN" : k == 1 ? "α" : k == 2 ? "β" : k == 3 ? "ω" : k == 4 ? "γ" : k == 5 ? "α·" : k == 6 ? "β·" : k == 7 ? "ω·" : k == 8 ? "γ·" : "?"; }
static const char * zsm_sn(int s) { return s == ZSM_FRESH ? "FRESH" : s == ZSM_LIVE ? "LIVE" : s == ZSM_SUSPENDED ? "SUSPENDED" : s == ZSM_RESUMED ? "RESUMED" : s == ZSM_DEAD ? "DEAD" : "?"; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void zsm_dump(void)
{
    unsigned long n = g_zsm_tr_n < ZSM_TRACE ? g_zsm_tr_n : ZSM_TRACE, i, first = g_zsm_tr_n < ZSM_TRACE ? 0 : g_zsm_tr_n % ZSM_TRACE;
    fprintf(stderr, "[ZSM] --- last %lu ports (oldest first), event #%lu ---\n", n, g_zsm_events);
    for (i = 0; i < n; i++) { unsigned long j = (first + i) % ZSM_TRACE;
        { long _k = g_zsm_tr_kind[j] & 0xFFL, _o = (g_zsm_tr_kind[j] >> 8) & 0xFFFFL;
          fprintf(stderr, "[ZSM]   %-6s op=%-5ld node=%-8lu rbp=0x%lx rsp=0x%lx depth=%ld st=%ld\n", zsm_kn(_k), _o, g_zsm_tr_node[j], g_zsm_tr_rbp[j], g_zsm_tr_rsp[j], g_zsm_tr_depth[j], g_zsm_tr_stno[j]); } }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void zsm_leak_report(void)
{
    unsigned long i;
    if (getenv("SCRIP_ZSM_RING")) zsm_dump();
    if (zsm_leak_report_on()) {
        for (i = 0; i < g_zsm_seen_n; i++) {
            zsm_ent * e = &g_zsm[g_zsm_seen_nodes[i] & (ZSM_N - 1)];
            if (e->state == ZSM_LIVE || e->state == ZSM_SUSPENDED || e->state == ZSM_RESUMED) {
                g_zsm_leaked_at_exit++;
                fprintf(stderr, "[ZSM-LEAK] node=%lu final_state=%s -- activation never reached DEAD (omega) by process end\n", e->node, zsm_sn(e->state));
            }
        }
        if (g_zsm_leaked_at_exit) fprintf(stderr, "[ZSM-LEAK] total leaked activations at exit: %lu\n", g_zsm_leaked_at_exit);
    }
    if (g_zsm_alpha_while_live) fprintf(stderr, "[ZSM-ALPHA] total alpha-while-live events (see MATCH_END cross-node whack note): %lu\n", g_zsm_alpha_while_live);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_zdp_sm_init(void)
{
    static int done = 0;
    if (done) return;
    done = 1;
    atexit(zsm_leak_report);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_zdp_sm_event(unsigned long node, unsigned long rbp, unsigned long rsp, long kind)
{
    long zop = (kind >> 8) & 0xFFFFL; kind &= 0xFFL;
    zsm_ent * e = &g_zsm[node & (ZSM_N - 1)];
    unsigned long t = g_zsm_tr_n % ZSM_TRACE;
    long depth = g_zsm_rsp0 ? (long)(g_zsm_rsp0 - rsp) : 0L;
    g_zsm_tr_node[t] = node; g_zsm_tr_rbp[t] = rbp; g_zsm_tr_rsp[t] = rsp; g_zsm_tr_kind[t] = kind | (zop << 8); g_zsm_tr_depth[t] = depth; g_zsm_tr_stno[t] = (long)kw_stcount; g_zsm_tr_n++; g_zsm_events++;
    if (zsm_census()) fprintf(stderr, "[ZSM-DEPTH] %-6s op=%-4ld node=%-8lu depth=%ld rsp=0x%lx rbp=0x%lx state=%s\n", zsm_kn(kind), zop, node, depth, rsp, rbp, zsm_sn(e->state));
    if (kind == 0) { g_zsm_rsp0 = rsp; return; }
    long fl = (kind >= 5L && kind <= 8L) ? 1L : 0L;
    if (fl) kind -= 4L;
    if (e->node != node && zsm_leak_report_on()) {
        if (g_zsm_seen_n < ZSM_N) g_zsm_seen_nodes[g_zsm_seen_n++] = node;
    }
    if (kind == 1 && (e->node == node) && (e->state == ZSM_LIVE || e->state == ZSM_SUSPENDED || e->state == ZSM_RESUMED)) {
        g_zsm_alpha_while_live++;
        if (getenv("SCRIP_ZSM_ALPHA_LOG")) fprintf(stderr, "[ZSM] α node=%lu WHILE state=%s (rbp=0x%lx) -- counted, not fatal; see MATCH_END cross-node whack note\n", node, zsm_sn(e->state), rbp);
    }
    if (kind == 1) { e->node = node; e->E = rbp; e->F = fl ? rbp : rsp - 8UL; e->rsp_a = rsp; e->live = 1UL; e->state = ZSM_LIVE; return; }
    if (e->node != node || !e->live) {
        if (kind == 2) { g_zsm_beta_no_alpha++; fprintf(stderr, "[ZSM] β node=%lu WITH NO LIVE α (rbp=0x%lx rsp=0x%lx) -- claimed impossible; counted, not fatal, so one occurrence cannot mask the census\n", node, rbp, rsp); }
        if (kind == 4 && (e->node != node || e->state == ZSM_FRESH || e->state == ZSM_DEAD)) {
            g_zsm_fsm_illegal++;
            if (!fl) { zsm_dump();
              { char b[256]; snprintf(b, sizeof b, "ZSM node=%lu FSM ILLEGAL: gamma with no live α (state=%s) -- success transfer from a box that was never entered", node, zsm_sn(e->node == node ? e->state : ZSM_FRESH)); rt_bomb(b); } }
            else if (g_zsm_fsm_illegal <= 8UL) fprintf(stderr, "[ZSM] γ· node=%lu with no live α (state=%s) -- counted (frameless arm)\n", node, zsm_sn(e->node == node ? e->state : ZSM_FRESH));
        }
        return;
    }
    if (kind == 2) {
        if (e->state != ZSM_SUSPENDED) { g_zsm_fsm_illegal++;
            if (!fl) { zsm_dump();
              { char b[256]; snprintf(b, sizeof b, "ZSM β node=%lu FSM ILLEGAL: beta while state=%s (need SUSPENDED) -- backtrack arrival at a box that never left via gamma", node, zsm_sn(e->state)); rt_bomb(b); } }
            else if (g_zsm_fsm_illegal <= 8UL) fprintf(stderr, "[ZSM] β· node=%lu while state=%s -- counted (frameless arm)\n", node, zsm_sn(e->state)); }
        if (rbp != e->F) { g_zsm_violations++; zsm_dump();
            { char b[256]; snprintf(b, sizeof b, "ZSM β node=%lu FRAME LOST: α established F=0x%lx (E=0x%lx); at β rbp=0x%lx skew=%ld", node, e->F, e->E, rbp, (long)(rbp - e->F)); rt_bomb(b); } }
        e->state = ZSM_RESUMED;
        return;
    }
    if (kind == 4) {
        g_zsm_gamma_events++;
        if (e->state != ZSM_LIVE && e->state != ZSM_RESUMED) { g_zsm_fsm_illegal++;
            if (!fl) { zsm_dump();
              { char b[256]; snprintf(b, sizeof b, "ZSM γ node=%lu FSM ILLEGAL: gamma while state=%s (need LIVE or RESUMED) -- success transfer from a box not currently entered", node, zsm_sn(e->state)); rt_bomb(b); } }
            else if (g_zsm_fsm_illegal <= 8UL) fprintf(stderr, "[ZSM] γ· node=%lu while state=%s -- counted (frameless arm)\n", node, zsm_sn(e->state)); }
        if (rbp != e->F) { g_zsm_violations++;
            if (!fl) { zsm_dump();
              { char b[256]; snprintf(b, sizeof b, "ZSM γ node=%lu FRAME LOST AT SUCCESS: α established F=0x%lx (E=0x%lx); at γ rbp=0x%lx skew=%ld -- localizes to the DEPARTING box, one hop earlier than the same defect would surface at the next β", node, e->F, e->E, rbp, (long)(rbp - e->F)); rt_bomb(b); } }
            else if (g_zsm_violations <= 8UL) fprintf(stderr, "[ZSM] γ· node=%lu rbp moved α→γ: α=0x%lx γ=0x%lx skew=%ld -- counted, not fatal (a WHACK-OWNING box legally retires the construct frame between its own α and γ, the MATCH_END shape; audit measured this on the FIRST green witness, s179 -- a fatal γ arm convicts passing programs and an instrument that convicts passing programs cannot testify about failing ones)\n", node, e->E, rbp, (long)(rbp - e->F)); }
        e->state = ZSM_SUSPENDED;
        return;
    }
    if (kind == 3) {
        e->live = 0UL;
        e->state = ZSM_DEAD;
        if (rbp != e->E) { g_zsm_violations++;
            if (!fl) { zsm_dump();
              { char b[256]; snprintf(b, sizeof b, "ZSM ω node=%lu IMBALANCE: α banked enclosing E=0x%lx; at ω rbp=0x%lx skew=%ld -- teardown did not give the frame back", node, e->E, rbp, (long)(rbp - e->E)); rt_bomb(b); } }
            else if (g_zsm_violations <= 8UL) fprintf(stderr, "[ZSM] ω· node=%lu rbp moved α→ω: α=0x%lx ω=0x%lx skew=%ld -- counted (frameless arm, whack-owner shape possible)\n", node, e->E, rbp, (long)(rbp - e->E)); }
        if (rsp > e->rsp_a && zsm_overpop_on()) fprintf(stderr, "[ZSM-OVERPOP] ω%s node=%lu op=%ld released %ld bytes MORE than α carved: α rsp=0x%lx ω rsp=0x%lx rbp=0x%lx st=%ld -- read as a CANDIDATE, not a verdict (whack-owners legally retire an enclosing frame here)\n", fl ? "·" : "", node, zop, (long)(rsp - e->rsp_a), e->rsp_a, rsp, rbp, (long)kw_stcount);
        if (rsp < e->rsp_a) { g_zsm_violations++;
            if (!fl) { zsm_dump();
              { char b[256]; snprintf(b, sizeof b, "ZSM ω node=%lu RSP LEAK: α rsp=0x%lx; at ω rsp=0x%lx still %ld bytes low -- carved and never released", node, e->rsp_a, rsp, (long)(e->rsp_a - rsp)); rt_bomb(b); } }
            else if (g_zsm_violations <= 8UL) fprintf(stderr, "[ZSM] ω· node=%lu rsp still %ld low vs α -- counted (frameless arm)\n", node, (long)(e->rsp_a - rsp)); }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_bomb(const char *msg)
{
    fflush(NULL);
    fprintf(stderr, "libscrip_rt: BOMB — %s\n", msg ? msg : "(no message)");
    abort();
}
