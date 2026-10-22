#include "rt/rt.h"
#include "g_runtime.h"
#include "dtp.h"
#include "core.h"
#include "IR.h"
#include "bb_pool.h"
#include "rt/prolog_atom.h"
#include <stdio.h>
#include <stdlib.h>
#include "ct_vec.h"
#include "rt_diag.h"
#if RT_DIAG
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_zdp_probe_report(unsigned long rsp, long op, long node, long expected, long portflags, long measured) {
    const char * port = (portflags & 1) ? "alpha" : "beta";
    if (expected == -1L) { fprintf(stderr, "[ZDP-TOP] port=%s op=%ld node=%ld measured=%ld rsp=%lu\n", port, op, node, measured, rsp); return; }
    if (measured != expected) { fprintf(stderr, "[ZDP-RSP] port=%s op=%ld node=%ld expected=%ld measured=%ld skew=%ld\n", port, op, node, expected, measured, measured - expected); return; }
    fprintf(stderr, "[ZDP-RBP] port=%s op=%ld node=%ld rbp_mismatch (rsp agreed at delta=%ld)\n", port, op, node, measured);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_zdp_report(unsigned long rsp, long op, long node, unsigned long expected) {
    fprintf(stderr, "[ZDPANCHOR] op=%ld node=%ld expected=%lu actual=%lu delta=%ld\n", op, node, expected, rsp, (long)(expected - rsp));
}
void rt_bomb(const char *msg);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_zdp_rbp_report(long node, unsigned long expected, unsigned long actual, long kind) {
    if (kind == 2) { g_runtime.zsm.zdp_rbp_nobank++; fprintf(stderr, "[ZDP-RBP] node=%ld NO-BANK (this node's alpha never ran, or its slot was aliased by recursion/collision)\n", node); return; }
    g_runtime.zsm.zdp_rbp_violations++;
    {
        char b[256];
        if (kind == 1)
            snprintf(b, sizeof b, "ZDP-RBP node=%ld OMEGA IMBALANCE: alpha banked enclosing rbp=0x%lx; at omega (teardown) rbp=0x%lx -- this box left without restoring the frame it found", node,
            expected, actual);
        else snprintf(b, sizeof b,
            "ZDP-RBP node=%ld BETA FRAME LOST: alpha banked enclosing rbp=0x%lx; at beta rbp=0x%lx whose saved parent is not that value -- the activation this box is standing in is not the one its a"
            "lpha established", node, expected, actual);
        rt_bomb(b);
    }
}
enum { ZSM_FRESH = 0, ZSM_LIVE = 1, ZSM_SUSPENDED = 2, ZSM_RESUMED = 3, ZSM_DEAD = 4 };
#define g_zsm_seen_nodes ((unsigned long *)g_runtime.zsm.zsm_seen.p)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zsm_census(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_ZSM_CENSUS"); v = (e && *e == '1') ? 1 : 0; } return v; }
static int zsm_leak_report_on(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_ZSM_LEAK_REPORT"); v = (e && *e == '1') ? 1 : 0; } return v; }
static int zsm_overpop_on(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_ZSM_OVERPOP"); v = (e && *e == '1') ? 1 : 0; } return v; }
static int zsm_bskew_on(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_ZSM_BSKEW"); v = (e && *e == '1') ? 1 : 0; } return v; }
static int zsm_aexp_on(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_ZSM_AEXP"); v = (e && *e == '0') ? 0 : 1; } return v; }
void rt_bomb(const char *msg);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void zsm_bombf(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
static void zsm_bombf(const char *fmt, ...) { va_list ap; va_start(ap, fmt); char b[vfmt_len(fmt, ap)]; vsnprintf(b, sizeof b, fmt, ap); va_end(ap); rt_bomb(b); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * zsm_kn(long k) {
    return k == 0 ? "ORIGIN" : k == 1 ? "α" : k == 2 ? "β" : k == 3 ? "ω" : k == 4 ? "γ" : k == 5 ? "α·" : k == 6 ? "β·" : k == 7 ? "ω·" : k == 8 ? "γ·" : "?";
}
static const char * zsm_sn(int s) { return s == ZSM_FRESH ? "FRESH" : s == ZSM_LIVE ? "LIVE" : s == ZSM_SUSPENDED ? "SUSPENDED" : s == ZSM_RESUMED ? "RESUMED" : s == ZSM_DEAD ? "DEAD" : "?"; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void zsm_dump(void) {
    unsigned long n = g_runtime.zsm.zsm_tr_n < ZSM_TRACE ? g_runtime.zsm.zsm_tr_n : ZSM_TRACE, i, first = g_runtime.zsm.zsm_tr_n < ZSM_TRACE ? 0 : g_runtime.zsm.zsm_tr_n % ZSM_TRACE;
    fprintf(stderr, "[ZSM] --- last %lu ports (oldest first), event #%lu ---\n", n, g_runtime.zsm.zsm_events);
    for (i = 0; i < n; i++) {
        unsigned long j = (first + i) % ZSM_TRACE;
        {
            long _k = g_runtime.zsm.zsm_tr_kind[j] & 0xFFL, _o = (g_runtime.zsm.zsm_tr_kind[j] >> 8) & 0xFFFFL;
            fprintf(stderr, "[ZSM]   %-6s op=%-5ld node=%-8lu rbp=0x%lx rsp=0x%lx depth=%ld st=%ld\n", zsm_kn(_k), _o, g_runtime.zsm.zsm_tr_node[j], g_runtime.zsm.zsm_tr_rbp[j],
                g_runtime.zsm.zsm_tr_rsp[j], g_runtime.zsm.zsm_tr_depth[j], g_runtime.zsm.zsm_tr_stno[j]);
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zsm_decl_on(void) {
    if (!g_runtime.misc.zdecl_mode) { const char * e = getenv("SCRIP_ZSM_DECL"); g_runtime.misc.zdecl_mode = (e && *e == '1') ? 2 : 1; }
    return g_runtime.misc.zdecl_mode == 2;
}
static long zsm_decl_field(long decl, int f) { return (long)(short)((decl >> (16 * f)) & 0xFFFFL); }
static const char * zsm_decl_port(int p) { return p == 0 ? "beta" : p == 1 ? "gamma" : p == 2 ? "omega" : "record"; }
static void zsm_decl_check(int port, long zop, unsigned long node, long declared, long actual) {
    int op = (zop >= 0L && zop < 256L) ? (int)zop : 255;
    g_runtime.misc.zdecl_chk[port][op]++;
    if (declared == actual) return;
    if (++g_runtime.misc.zdecl_bad[port][op] <= 3UL)
        fprintf(stderr, "[ZSM-DECL] port=%s op=%ld(%s) node=%lu declared=%ld actual=%ld st=%ld\n", zsm_decl_port(port), zop, bb_op_name((IR_e)zop), node, declared, actual,
        (long)g_runtime.misc.kw_stcount);
}
static unsigned long zsm_ek_slot(unsigned long node, unsigned long rbp) { return (((node * 0x9E3779B97F4A7C15UL) ^ (rbp >> 3) * 0xC2B2AE3D27D4EB4FUL) >> 40) & (ZSM_N / 4 - 1); }
static void zsm_ek_push(unsigned long node, unsigned long rbp, unsigned long e) {
    unsigned long k = zsm_ek_slot(node, rbp);
    unsigned long * st = g_runtime.misc.zsm_ek_e[k];
    if (g_runtime.misc.zsm_ek_node[k] != node || g_runtime.misc.zsm_ek_rbp[k] != rbp) {
        g_runtime.misc.zsm_ek_node[k] = node;
        g_runtime.misc.zsm_ek_rbp[k] = rbp;
        g_runtime.misc.zsm_ek_n[k] = 0;
        g_runtime.misc.zsm_ek_cut[k] = 0;
    }
    int n = g_runtime.misc.zsm_ek_n[k];
    while (n > 0 && st[n - 1] <= e) n--;
    if (n == 8) { memmove(st, st + 1, 7 * sizeof *st); n = 7; g_runtime.misc.zsm_ek_cut[k] = 1; }
    st[n++] = e;
    g_runtime.misc.zsm_ek_n[k] = (unsigned char)n;
}
static void zsm_ek_depart(unsigned long node, unsigned long rbp, int port, long zop, long want_e, long declared, unsigned long rsp, int pop) {
    unsigned long k = zsm_ek_slot(node, rbp);
    unsigned long * st = g_runtime.misc.zsm_ek_e[k];
    int n = g_runtime.misc.zsm_ek_node[k] == node && g_runtime.misc.zsm_ek_rbp[k] == rbp ? g_runtime.misc.zsm_ek_n[k] : -1;
    if (n <= 0) { g_runtime.misc.zdecl_unanchored++; return; }
    for (int q = n - 1; q >= 0; q--) { if ((long)st[q] != want_e) continue; zsm_decl_check(port, zop, node, declared, declared); if (pop) g_runtime.misc.zsm_ek_n[k] = (unsigned char)q; return; }
    if (g_runtime.misc.zsm_ek_cut[k]) { g_runtime.misc.zdecl_unanchored++; return; }
    zsm_decl_check(port, zop, node, declared, declared + (long)st[n - 1] - want_e);
}
static void zsm_decl_event(unsigned long node, unsigned long rbp, unsigned long rsp, long kind, long zop, long fl, long decl) {
    long K = zsm_decl_field(decl, 0), gp = zsm_decl_field(decl, 1), wp = zsm_decl_field(decl, 2);
    if (kind == 1) { if (g_runtime.misc.zsm_pre_node != node) return; zsm_ek_push(node, rbp, g_runtime.misc.zsm_pre_rsp); if (!fl) zsm_ek_push(node, rsp - 8UL, g_runtime.misc.zsm_pre_rsp); return; }
    if (kind == 2 && ((decl >> 54) & 1L)) zsm_ek_depart(node, rbp, 0, zop, (long)rsp + K, K, rsp, 0);
    if (kind == 4 && ((decl >> 49) & 3L) == 3L) { if ((decl >> 52) & 1L) zsm_decl_check(3, zop, node, 0L, 0L); else zsm_ek_depart(node, rbp, 1, zop, (long)rsp + K - gp, K - gp, rsp, 0); }
    if (kind == 3 && ((decl >> 49) & 5L) == 5L) { if ((decl >> 53) & 1L) zsm_decl_check(3, zop, node, 0L, 0L); else zsm_ek_depart(node, rbp, 2, zop, (long)rsp - wp, wp, rsp, 1); }
}
static void zsm_decl_report(void) {
    unsigned long chk = 0, bad = 0;
    for (int p = 0; p < 4; p++) {
        for (int op = 0; op < 256; op++) {
            if (!g_runtime.misc.zdecl_chk[p][op]) continue;
            chk += g_runtime.misc.zdecl_chk[p][op];
            bad += g_runtime.misc.zdecl_bad[p][op];
            fprintf(stderr, "[ZSM-DECL-SUM] op=%s port=%s checked=%lu bad=%lu\n", bb_op_name((IR_e)op), zsm_decl_port(p), g_runtime.misc.zdecl_chk[p][op], g_runtime.misc.zdecl_bad[p][op]);
        }
    }
    fprintf(stderr, "[ZSM-DECL-TOTAL] checked=%lu bad=%lu events=%lu unanchored=%lu\n", chk, bad, g_runtime.zsm.zsm_events, g_runtime.misc.zdecl_unanchored);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void zsm_leak_report(void) {
    unsigned long i;
    if (getenv("SCRIP_ZSM_RING")) zsm_dump();
    if (zsm_leak_report_on()) {
        for (i = 0; i < g_runtime.zsm.zsm_seen_n; i++) {
            zsm_ent * e = &g_runtime.zsm.zsm[g_zsm_seen_nodes[i] & (ZSM_N - 1)];
            if (e->state == ZSM_LIVE || e->state == ZSM_SUSPENDED || e->state == ZSM_RESUMED) {
                g_runtime.zsm.zsm_leaked_at_exit++;
                fprintf(stderr, "[ZSM-LEAK] node=%lu final_state=%s -- activation never reached DEAD (omega) by process end\n", e->node, zsm_sn(e->state));
            }
        }
        if (g_runtime.zsm.zsm_leaked_at_exit) fprintf(stderr, "[ZSM-LEAK] total leaked activations at exit: %lu\n", g_runtime.zsm.zsm_leaked_at_exit);
    }
    if (g_runtime.zsm.zsm_alpha_while_live) fprintf(stderr, "[ZSM-ALPHA] total alpha-while-live events (see MATCH_END cross-node whack note): %lu\n", g_runtime.zsm.zsm_alpha_while_live);
    if (zsm_decl_on()) zsm_decl_report();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_zdp_sm_init(void) { static int done = 0; if (done) return; done = 1; atexit(zsm_leak_report); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_zdp_sm_event(unsigned long node, unsigned long rbp, unsigned long rsp, long kind, long exp_depth, long decl) {
    rt_zdp_sm_init();
    if ((kind & 0xFFL) == 9L) { g_runtime.misc.zsm_pre_node = node; g_runtime.misc.zsm_pre_rsp = rsp; return; }
    long zop = (kind >> 8) & 0xFFFFL;
    kind &= 0xFFL;
    zsm_ent * e = &g_runtime.zsm.zsm[node & (ZSM_N - 1)];
    unsigned long t = g_runtime.zsm.zsm_tr_n % ZSM_TRACE;
    long depth = g_runtime.zsm.zsm_rsp0 ? (long)(g_runtime.zsm.zsm_rsp0 - rsp) : 0L;
    g_runtime.zsm.zsm_tr_node[t] = node;
    g_runtime.zsm.zsm_tr_rbp[t] = rbp;
    g_runtime.zsm.zsm_tr_rsp[t] = rsp;
    g_runtime.zsm.zsm_tr_kind[t] = kind | (zop << 8);
    g_runtime.zsm.zsm_tr_depth[t] = depth;
    g_runtime.zsm.zsm_tr_stno[t] = (long)g_runtime.misc.kw_stcount;
    g_runtime.zsm.zsm_tr_n++;
    g_runtime.zsm.zsm_events++;
    if (zsm_census()) fprintf(stderr, "[ZSM-DEPTH] %-6s op=%-4ld node=%-8lu depth=%ld rsp=0x%lx rbp=0x%lx state=%s\n", zsm_kn(kind), zop, node, depth, rsp, rbp, zsm_sn(e->state));
    if (kind == 0) { if (exp_depth >= 0L) g_runtime.zsm.zsm_rsp0 = rsp + (unsigned long)exp_depth; else if (!g_runtime.zsm.zsm_rsp0) g_runtime.zsm.zsm_rsp0 = rsp; return; }
    long fl = (kind >= 5L && kind <= 8L) ? 1L : 0L;
    if (fl) kind -= 4L;
    if (zsm_decl_on() && ((decl >> 48) & 1L)) zsm_decl_event(node, rbp, rsp, kind, zop, fl, decl);
    if (e->node != node && zsm_leak_report_on()) {
        cv_reserve(&g_runtime.zsm.zsm_seen, (uint32_t)sizeof(unsigned long), (uint64_t)g_runtime.zsm.zsm_seen_n + 1, "g_runtime.zsm.zsm_seen");
        g_zsm_seen_nodes[g_runtime.zsm.zsm_seen_n++] = node;
    }
    if (kind == 1 && (e->node == node) && (e->state == ZSM_LIVE || e->state == ZSM_SUSPENDED || e->state == ZSM_RESUMED)) {
        g_runtime.zsm.zsm_alpha_while_live++;
        if (getenv("SCRIP_ZSM_ALPHA_LOG")) fprintf(stderr, "[ZSM] α node=%lu WHILE state=%s (rbp=0x%lx) -- counted, not fatal; see MATCH_END cross-node whack note\n", node, zsm_sn(e->state), rbp);
    }
    if (kind == 1) {
        if (zop == 113L) g_runtime.zsm.zsm_stmt0 = rsp;
        if (exp_depth >= 0L && g_runtime.zsm.zsm_rsp0 && zsm_aexp_on()) {
            long d = (long)(g_runtime.zsm.zsm_rsp0 - rsp);
            if (d >= 0L && d != exp_depth)
                fprintf(stderr,
                "[ZSM-AEXP] α%s node=%lu op=%ld(%s) RSP EXPECTED-VALUE MISMATCH AT ALPHA (Lon s196: RSP-RSP0 must equal the operand offset the emitter baked): statement-anchor depth=%ld but op_zdep"
                "th=%ld (delta=%ld) -- the operand access that follows resolves [rsp + off + %ld] and reads %ld bytes off; rsp=0x%lx rsp0=0x%lx st=%ld\n", fl ? "·" : "", node, zop,
                bb_op_name((IR_e)zop), d, exp_depth, d - exp_depth, exp_depth, d - exp_depth, rsp, g_runtime.zsm.zsm_rsp0, (long)g_runtime.misc.kw_stcount);
        }
        e->node = node;
        e->E = rbp;
        e->F = fl ? rbp : rsp - 8UL;
        e->rsp_a = rsp;
        e->live = 1UL;
        e->state = ZSM_LIVE;
        return;
    }
    if (e->node != node || !e->live) {
        if (kind == 2) {
            g_runtime.zsm.zsm_beta_no_alpha++;
            fprintf(stderr, "[ZSM] β node=%lu WITH NO LIVE α (rbp=0x%lx rsp=0x%lx) -- claimed impossible; counted, not fatal, so one occurrence cannot mask the census\n", node, rbp, rsp);
        }
        if (kind == 4 && (e->node != node || e->state == ZSM_FRESH || e->state == ZSM_DEAD)) {
            g_runtime.zsm.zsm_fsm_illegal++;
            if (!fl) {
                zsm_dump();
                zsm_bombf("ZSM node=%lu FSM ILLEGAL: gamma with no live α (state=%s) -- success transfer from a box that was never entered", node, zsm_sn(e->node == node ? e->state : ZSM_FRESH));
            } else if (g_runtime.zsm.zsm_fsm_illegal <= 8UL)
                fprintf(stderr, "[ZSM] γ· node=%lu with no live α (state=%s) -- counted (frameless arm)\n", node, zsm_sn(e->node == node ? e->state : ZSM_FRESH));
        }
        return;
    }
    if (kind == 2) {
        if (rsp != e->rsp_a && zsm_bskew_on()) {
            fprintf(stderr, "[ZSM-BSKEW] β%s node=%lu op=%ld(%s) rsp at β differs from α by %ld bytes (α rsp=0x%lx β rsp=0x%lx) st=%ld\n", fl ? "·" : "", node, zop, bb_op_name((IR_e)zop),
                (long)(e->rsp_a - rsp), e->rsp_a, rsp, (long)g_runtime.misc.kw_stcount);
        }
        if (e->state != ZSM_SUSPENDED) {
            g_runtime.zsm.zsm_fsm_illegal++;
            if (!fl) {
                zsm_dump();
                zsm_bombf("ZSM β node=%lu FSM ILLEGAL: beta while state=%s (need SUSPENDED) -- backtrack arrival at a box that never left via gamma", node, zsm_sn(e->state));
            } else if (g_runtime.zsm.zsm_fsm_illegal <= 8UL) fprintf(stderr, "[ZSM] β· node=%lu while state=%s -- counted (frameless arm)\n", node, zsm_sn(e->state));
        }
        if (rbp != e->F) {
            g_runtime.zsm.zsm_violations++;
            if (!fl) {
                zsm_dump();
                { char b[256]; snprintf(b, sizeof b, "ZSM β node=%lu FRAME LOST: α established F=0x%lx (E=0x%lx); at β rbp=0x%lx skew=%ld", node, e->F, e->E, rbp, (long)(rbp - e->F)); rt_bomb(b); }
            } else if (g_runtime.zsm.zsm_violations <= 8UL)
                fprintf(stderr,
                "[ZSM] β· node=%lu rbp moved α→β: F=0x%lx at β rbp=0x%lx skew=%ld -- counted, not fatal (the γ arm's whack-owner exemption mirrored per s195 caveat (b): a whack-owning box le"
                "gally retires the construct frame before a frameless sibling's β; the FRAMED tier still bombs)\n", node, e->F, rbp, (long)(rbp - e->F));
        }
        e->state = ZSM_RESUMED;
        return;
    }
    if (kind == 4) {
        g_runtime.zsm.zsm_gamma_events++;
        if (e->state != ZSM_LIVE && e->state != ZSM_RESUMED) {
            g_runtime.zsm.zsm_fsm_illegal++;
            if (!fl) {
                zsm_dump();
                zsm_bombf("ZSM γ node=%lu FSM ILLEGAL: gamma while state=%s (need LIVE or RESUMED) -- success transfer from a box not currently entered", node, zsm_sn(e->state));
            } else if (g_runtime.zsm.zsm_fsm_illegal <= 8UL) fprintf(stderr, "[ZSM] γ· node=%lu while state=%s -- counted (frameless arm)\n", node, zsm_sn(e->state));
        }
        if (rbp != e->F) {
            g_runtime.zsm.zsm_violations++;
            if (!fl) {
                zsm_dump();
                {
                    char b[256];
                    snprintf(b, sizeof b,
                        "ZSM γ node=%lu FRAME LOST AT SUCCESS: α established F=0x%lx (E=0x%lx); at γ rbp=0x%lx skew=%ld -- localizes to the DEPARTING box, one hop earlier than the same defect wou"
                        "ld surface at the next β", node, e->F, e->E, rbp, (long)(rbp - e->F));
                    rt_bomb(b);
                }
            } else if (g_runtime.zsm.zsm_violations <= 8UL)
                fprintf(stderr,
                "[ZSM] γ· node=%lu rbp moved α→γ: α=0x%lx γ=0x%lx skew=%ld -- counted, not fatal (a WHACK-OWNING box legally retires the construct frame between its own α and γ, the MATCH_"
                "END shape; audit measured this on the FIRST green witness, s179 -- a fatal γ arm convicts passing programs and an instrument that convicts passing programs cannot testify about fai"
                "ling ones)\n", node, e->E, rbp, (long)(rbp - e->F));
        }
        e->state = ZSM_SUSPENDED;
        return;
    }
    if (kind == 3) {
        e->live = 0UL;
        e->state = ZSM_DEAD;
        if (rbp != e->E) {
            g_runtime.zsm.zsm_violations++;
            if (!fl) {
                zsm_dump();
                {
                    char b[256];
                    snprintf(b, sizeof b, "ZSM ω node=%lu IMBALANCE: α banked enclosing E=0x%lx; at ω rbp=0x%lx skew=%ld -- teardown did not give the frame back", node, e->E, rbp,
                        (long)(rbp - e->E));
                    rt_bomb(b);
                }
            } else if (g_runtime.zsm.zsm_violations <= 8UL)
                fprintf(stderr, "[ZSM] ω· node=%lu rbp moved α→ω: α=0x%lx ω=0x%lx skew=%ld -- counted (frameless arm, whack-owner shape possible)\n", node, e->E, rbp, (long)(rbp - e->E));
        }
        if (rsp > e->rsp_a && zsm_overpop_on())
            fprintf(stderr,
            "[ZSM-OVERPOP] ω%s node=%lu op=%ld released %ld bytes MORE than α carved: α rsp=0x%lx ω rsp=0x%lx rbp=0x%lx st=%ld -- read as a CANDIDATE, not a verdict (whack-owners legally retire "
            "an enclosing frame here)\n", fl ? "·" : "", node, zop, (long)(rsp - e->rsp_a), e->rsp_a, rsp, rbp, (long)g_runtime.misc.kw_stcount);
        if (rsp < e->rsp_a) {
            g_runtime.zsm.zsm_violations++;
            if (!fl) {
                zsm_dump();
                {
                    char b[256];
                    snprintf(b, sizeof b, "ZSM ω node=%lu RSP LEAK: α rsp=0x%lx; at ω rsp=0x%lx still %ld bytes low -- carved and never released", node, e->rsp_a, rsp, (long)(e->rsp_a - rsp));
                    rt_bomb(b);
                }
            } else if (g_runtime.zsm.zsm_violations <= 8UL) fprintf(stderr, "[ZSM] ω· node=%lu rsp still %ld low vs α -- counted (frameless arm)\n", node, (long)(e->rsp_a - rsp));
        }
    }
}
#else
void rt_zdp_sm_init(void) { }
#endif
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_bomb(const char *msg) { fflush(NULL); fprintf(stderr, "libscrip_rt: BOMB — %s\n", msg ? msg : "(no message)"); abort(); }
