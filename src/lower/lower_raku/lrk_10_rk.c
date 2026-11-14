#include "lower/lower_raku/lrk_head.h"
#include <string.h>
#include "ir/stage2_sm_prog_api.h"
#include "parsers/snobol4/scrip_cc.h"
#include "lower/lower_raku/lrk_03_rk.h"
#include "lower/lower_raku/lrk_07_rk.h"
#include "lower/lower_raku/lrk_10_rk.h"
#include "ir/ast_tt_capt_immed_asgn.h"
#include "ir/ast.h"
#include "ir/ast_node_new.h"
#include "ir/ct_arena.h"
const char * rk_ph_of(const tree_t * n) { if (!n || n->t != TT_FNC || !n->v.sval || strncmp(n->v.sval, "__rk_phaser_", 12) != 0) return NULL; return n->v.sval + 12; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_ph_body(tree_t * n) { return (n && n->n >= 2) ? n->c[n->n - 1] : ast_node_new(TT_SEQ_EXPR); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_ph_is_loop(const char * p) { return !strcmp(p, "FIRST") || !strcmp(p, "LAST") || !strcmp(p, "NEXT") || !strcmp(p, "once"); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_ph_rank(const char * p, int mainline) {
    if (!strcmp(p, "BEGIN")) return 0;
    if (!strcmp(p, "CHECK")) return 1;
    if (!strcmp(p, "INIT")) return 2;
    if (!strcmp(p, "ENTER") || !strcmp(p, "PRE")) return 3;
    if (!strcmp(p, "LEAVE") || !strcmp(p, "POST")) return 5;
    if (!strcmp(p, "KEEP")) return mainline ? -1 : 6;
    if (!strcmp(p, "UNDO")) return mainline ? 6 : -1;
    if (!strcmp(p, "END")) return 7;
    if (!strcmp(p, "TEMP") || !strcmp(p, "CLOSE")) return -1;
    if (rk_ph_is_loop(p)) return -2;
    return 4;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_ph_unplaced(const char * p) {
    extern void rt_script_die_surface(const char * msg);
    char m[fmt_len("%s { } is implemented at mainline scope and as a loop phaser at the top level of a loop body, not here", p)];
    snprintf(m, sizeof m, "%s { } is implemented at mainline scope and as a loop phaser at the top level of a loop body, not here", p);
    rt_script_die_surface(m);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_ph_inline(tree_t * n) {
    if (!n) return;
    for (int i = 0; i < n->n; i++) { const char * p = rk_ph_of(n->c[i]); if (p) { if (rk_ph_is_loop(p)) rk_ph_unplaced(p); n->c[i] = rk_ph_body(n->c[i]); } rk_ph_inline(n->c[i]); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_ph_place(tree_t * seq, int from, int mainline) {
    int any = 0, n = seq->n, nin = 0;
    tree_t * tail_ret = NULL;
    for (int i = from; i < n; i++) if (rk_ph_of(seq->c[i])) { any = 1; break; }
    if (!any) return;
    tree_t ** in = (tree_t **)ct_alloc(sizeof(tree_t *) * (size_t)(n - from + 1));
    for (int i = from; i < n; i++) in[nin++] = seq->c[i];
    if (!mainline && nin > 0 && in[nin - 1] && in[nin - 1]->t == TT_RETURN) tail_ret = in[--nin];
    seq->n = from;
    for (int rank = 0; rank <= 7; rank++) for (int i = 0; i < nin; i++) {
        tree_t * it = in[i];
        const char * p = rk_ph_of(it);
        int r = p ? rk_ph_rank(p, mainline) : 4;
        if (r == -1) continue;
        if (r == -2 && !mainline) { if (rank == 4) ast_push(seq, it); continue; }
        if (r == -2) r = 4;
        if (r != rank) continue;
        ast_push(seq, p ? rk_ph_body(it) : it);
    }
    if (tail_ret) ast_push(seq, tail_ret);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_ph_join(tree_t * a, tree_t * c) { if (!a) return c; tree_t * s = ast_node_new(TT_SEQ_EXPR); ast_push(s, a); ast_push(s, c); return s; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_ph_store(const char * g, int v) {
    tree_t * a = ast_node_new(TT_ASSIGN);
    ast_push(a, leaf_sval2(TT_VAR, g));
    tree_t * z = ast_node_new(TT_ILIT);
    z->v.ival = v;
    ast_push(a, z);
    return a;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_ph_loop(tree_t * loop, tree_t * body, int * uid) {
    tree_t * first = NULL, * nxt = NULL, * last = NULL;
    int keep = 0;
    for (int i = 0; i < body->n; i++) {
        const char * p = rk_ph_of(body->c[i]);
        if (p && (!strcmp(p, "FIRST") || !strcmp(p, "once"))) first = rk_ph_join(first, rk_ph_body(body->c[i]));
        else if (p && !strcmp(p, "NEXT")) nxt = rk_ph_join(nxt, rk_ph_body(body->c[i]));
        else if (p && !strcmp(p, "LAST")) last = rk_ph_join(last, rk_ph_body(body->c[i]));
        else body->c[keep++] = body->c[i];
    }
    if (keep == body->n) return loop;
    char gb[48];
    snprintf(gb, sizeof gb, "__rk_ph_g%d", (*uid)++);
    const char * g = intern(gb);
    tree_t * inner = ast_node_new(TT_SEQ_EXPR);
    if (first) {
        tree_t * t = ast_node_new(TT_SEQ_EXPR);
        ast_push(t, rk_ph_store(g, 0));
        ast_push(t, first);
        tree_t * iff = ast_node_new(TT_IF);
        ast_push(iff, leaf_sval2(TT_VAR, g));
        ast_push(iff, t);
        ast_push(inner, iff);
    } else if (last) ast_push(inner, rk_ph_store(g, 0));
    for (int i = 0; i < keep; i++) ast_push(inner, body->c[i]);
    if (nxt) ast_push(inner, nxt);
    body->n = 0;
    for (int i = 0; i < inner->n; i++) ast_push(body, inner->c[i]);
    if (!first && !last) return loop;
    tree_t * outer = ast_node_new(TT_SEQ_EXPR);
    ast_push(outer, rk_ph_store(g, 1));
    ast_push(outer, loop);
    if (last) { tree_t * u = ast_node_new(TT_UNLESS); ast_push(u, leaf_sval2(TT_VAR, g)); tree_t * s = ast_node_new(TT_SEQ_EXPR); ast_push(s, last); ast_push(u, s); ast_push(outer, u); }
    return outer;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_ph_walk(tree_t * n, int in_class, int * uid) {
    if (!n) return n;
    int cls = n->t == TT_CLASS_DECL || n->t == TT_ROLE_DECL || n->t == TT_GRAMMAR_DECL || n->t == TT_MODULE_DECL;
    for (int i = 0; i < n->n; i++) n->c[i] = rk_ph_walk(n->c[i], cls, uid);
    if (n->t == TT_SEQ_EXPR) rk_ph_place(n, 0, 0);
    else if (n->t == TT_SUB_DECL) {
        int bs = (int) n->v.ival + (in_class ? 0 : 1);
        if (bs < 1) bs = 1;
        if (bs > n->n) bs = n->n;
        rk_ph_place(n, bs, 0);
    } else if (n->t == TT_WHILE || n->t == TT_UNTIL || n->t == TT_REPEAT || n->t == TT_CLOOP || n->t == TT_EVERY || n->t == TT_FOR_RANGE || n->t == TT_DO_WHILE) {
        tree_t * body = NULL;
        for (int i = n->n - 1; i >= 0; i--) if (n->c[i] && n->c[i]->t == TT_SEQ_EXPR) { body = n->c[i]; break; }
        if (body) return rk_ph_loop(n, body, uid);
    }
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_ph_subj_attr(tree_t * st) {
    if (!st || st->t != TT_STMT) return NULL;
    for (int i = 0; i < st->n; i++) { tree_t * a = st->c[i]; if (a && a->t == TT_ATTR && a->v.sval && !strcmp(a->v.sval, ":subj") && a->n == 1) return a; }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_ph_collect_vars(const tree_t * t, rk_ns_t * out) {
    if (!t) return;
    if (t->t == TT_VAR && t->v.sval) rk_ns_add(out, t->v.sval);
    for (int i = 0; i < t->n; i++) rk_ph_collect_vars(t->c[i], out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_ph_bare_asg(const tree_t * e) {
    return e && e->t == TT_ASSIGN && e->n == 2 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval && (e->c[0]->slen & 4) && e->c[1] &&
        (e->c[1]->t == TT_NUL || (e->c[1]->t == TT_FNC && e->c[1]->v.sval && !strcmp(e->c[1]->v.sval, "__rk_undef")));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_ph_bare_decl_hits(const tree_t * e, const rk_ns_t * set) {
    if (!e) return 0;
    if (rk_ph_bare_asg(e)) return rk_ns_has(set, e->c[0]->v.sval);
    if (e->t == TT_SEQ_EXPR && e->n >= 2 && e->c[0] && e->c[0]->t == TT_ASSIGN && e->c[0]->n == 2 && e->c[0]->c[0] && e->c[0]->c[0]->t == TT_VAR && e->c[0]->c[0]->v.sval &&
        !strncmp(e->c[0]->c[0]->v.sval, "__destr_", 8)) for (int i = 1; i < e->n; i++)
        if (e->c[i] && e->c[i]->t == TT_ASSIGN && e->c[i]->n == 2 && e->c[i]->c[0] && e->c[i]->c[0]->t == TT_VAR && e->c[i]->c[0]->v.sval && (e->c[i]->c[0]->slen & 4) &&
        rk_ns_has(set, e->c[i]->c[0]->v.sval)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_place_phasers(tree_t * prog) {
    int uid = 0, any = 0;
    if (!prog) return;
    rk_ph_walk(prog, 0, &uid);
    for (int i = 0; i < prog->n; i++) { tree_t * a = rk_ph_subj_attr(prog->c[i]); if (a && rk_ph_of(a->c[0])) { any = 1; break; } }
    if (any) {
        int n = prog->n;
        tree_t ** in = (tree_t **)ct_alloc(sizeof(tree_t *) * (size_t)(n + 1));
        tree_t ** bd = (tree_t **)ct_alloc(sizeof(tree_t *) * (size_t)(n + 1));
        int * rk = (int *)ct_alloc(sizeof(int) * (size_t)(n + 1));
        for (int i = 0; i < n; i++) {
            in[i] = prog->c[i];
            tree_t * a = rk_ph_subj_attr(in[i]);
            tree_t * e = a ? a->c[0] : NULL;
            const char * p = rk_ph_of(e);
            rk[i] = p ? rk_ph_rank(p, 1) : 4;
            if (rk[i] == -2) rk[i] = 4;
            bd[i] = p ? rk_ph_body(e) : NULL;
        }
        {
            rk_ns_t seen = { 0 };
            for (int i = 0; i < n; i++) if (bd[i] && rk[i] <= 3) rk_ph_collect_vars(bd[i], &seen);
            for (int i = 0; i < n; i++) if (!bd[i] && rk[i] == 4) { tree_t * a = rk_ph_subj_attr(in[i]); if (a && rk_ph_bare_decl_hits(a->c[0], &seen)) rk[i] = 0; }
        }
        prog->n = 0;
        for (int rank = 0; rank <= 7; rank++) for (int j = 0; j < n; j++) {
            int i = rank == 7 ? n - 1 - j : j;
            if (rk[i] != rank) continue;
            if (bd[i]) rk_ph_subj_attr(in[i])->c[0] = bd[i];
            ast_push(prog, in[i]);
        }
    }
    for (int i = 0; i < prog->n; i++) { tree_t * a = rk_ph_subj_attr(prog->c[i]); if (a) rk_ph_inline(a->c[0]); }
}
static int rk_subtree_has_any_suspend(const tree_t * n) {
    if (!n) return 0;
    if (n->t == TT_SUSPEND) return 1;
    if (n->t == TT_GATHER) return 0;
    for (int i = 0; i < n->n; i++) if (rk_subtree_has_any_suspend(n->c[i])) return 1;
    return 0;
}
static int rk_subtree_has_nested_suspend(const tree_t * n) {
    if (!n) return 0;
    if (n->t == TT_GATHER) return 0;
    if (n->t == TT_ANON_BLOCK || n->t == TT_SUB_DECL) return rk_subtree_has_any_suspend(n);
    for (int i = 0; i < n->n; i++) if (rk_subtree_has_nested_suspend(n->c[i])) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_proc_is_generator(const char * name) {
    if (!name) return 0;
    for (int i = 0; i < g_stage2.proc_count; i++) if (g_stage2.proc_table[i].name && !strcmp(g_stage2.proc_table[i].name, name)) return g_stage2.proc_table[i].is_generator;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_mark_generators(void) {
    for (int pi = 0; pi < g_stage2.proc_count; pi++) {
        const tree_t * proc = (const tree_t *) g_stage2.proc_table[pi].proc;
        if (!proc || proc->t != TT_SUB_DECL) continue;
        int np = (int) proc->v.ival;
        g_stage2.proc_table[pi].is_generator = (proc->slen & 64) != 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_gather_blockify(tree_t * t) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_gather_blockify(t->c[i]);
    if (t->t == TT_GATHER && t->n >= 1 && t->c[0] && t->c[0]->t != TT_ANON_BLOCK) {
        tree_t * body = ast_node_new(TT_SEQ_EXPR);
        ast_push(body, t->c[0]);
        tree_t * blk = ast_node_new(TT_ANON_BLOCK);
        blk->line = t->line;
        ast_push(blk, body);
        t->c[0] = blk;
    }
    if (t->t == TT_GATHER && t->n >= 1 && t->c[0] && t->c[0]->t == TT_ANON_BLOCK) {
        t->c[0]->slen |= 64;
        for (int k = 0; k < t->c[0]->n; k++) if (rk_subtree_has_nested_suspend(t->c[0]->c[k])) t->slen |= 128;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_supply_emits(tree_t * t) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_supply_emits(t->c[i]);
    if (t->t != TT_FNC || !t->v.sval || strcmp(t->v.sval, "emit") || t->n < 2) return;
    tree_t * a = t->c[1];
    if (t->n > 2) { a = ast_node_new(TT_FNC); a->v.sval = (char *) "__rk_arr"; ast_push(a, leaf_sval2(TT_VAR, "__rk_arr")); for (int i = 1; i < t->n; i++) ast_push(a, t->c[i]); }
    t->t = TT_SUSPEND;
    t->v.sval = NULL;
    t->n = 0;
    ast_push(t, a);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_supply_nodes(tree_t * t, int user_emit) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_supply_nodes(t->c[i], user_emit);
    if (t->t != TT_GATHER || t->v.ival != 2 || t->n < 1) return;
    tree_t * body = t->c[0];
    if (!user_emit) rk_supply_emits(body);
    tree_t * g = ast_node_new(TT_GATHER);
    g->line = t->line;
    ast_push(g, body);
    tree_t * sq = ast_node_new(TT_SEQ_EXPR);
    ast_push(sq, g);
    tree_t * blk = ast_node_new(TT_ANON_BLOCK);
    blk->line = t->line;
    ast_push(blk, sq);
    t->t = TT_NEW;
    t->v.ival = 0;
    t->n = 0;
    ast_push(t, leaf_sval2(TT_QLIT, "Supply"));
    ast_push(t, leaf_sval2(TT_QLIT, "gen"));
    ast_push(t, blk);
}
