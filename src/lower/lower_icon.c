#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "lower.h"
extern int icn_builtin_is_known(const char *);
extern int icn_builtin_is_generator(const char *);
int g_postfix_resume = 0;
static int icn_const_step(const tree_t * s, int64_t * bits, int * isr);
static IR_t * icn_arm_result(IR_t * rv);
typedef struct {
    IR_graph_t * g; IR_t * psucc; IR_t * pfail; const char ** pn; int npn; const char ** ln; int nln; const char ** gn; int ngn; const char * pname;
    IR_t * last_gen; IR_t * loop_exit; IR_t * loop_break_beta;     IR_t * loop_next; IR_t * beta; IR_t * conj_resumable;
    IR_t * loop_stk_exit[64]; IR_t * loop_stk_next[64]; IR_t * loop_stk_fail[64]; IR_t * loop_fail; int loop_sp; IR_t * scan_stk_enter[16]; int scan_sp; int loop_next_ssp;
} icx_t;
#define ICN_LOOP_STK_MAX 64
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_is_local(const icx_t * cx, const char * nm) { if (!nm) return 0; for (int i = 0; i < cx->nln; i++) if (cx->ln[i] && !strcmp(cx->ln[i], nm)) return 1; return 0; }
static int icn_kw_assignable(const char * kw) { const char * id = (kw && kw[0] == '&') ? kw + 1 : kw; return id && (!strcmp(id, "subject") || !strcmp(id, "pos") || !strcmp(id, "random") || !strcmp(id, "trace") || !strcmp(id, "error") || !strcmp(id, "dump")); }
static const char * icn_variable_lit(const icx_t * cx, const tree_t * t) { if (!t || t->t != TT_FNC || t->n != 2 || !t->c[0] || !t->c[1]) return NULL; const tree_t * f = t->c[0]; if (f->t != TT_VAR || !f->v.sval || strcmp(f->v.sval, "variable") || icn_is_local(cx, "variable")) return NULL; const tree_t * g = t->c[1]; return (g->t == TT_QLIT && g->v.sval && g->v.sval[0]) ? g->v.sval : NULL; }
static tree_t * icn_variable_lit_tree(const char * nm) { tree_t * v = ast_node_new(nm[0] == '&' ? TT_KEYWORD : TT_VAR); v->v.sval = (char *) nm; return v; }
static lc_vec g_icn_synth_excl;
static int icn_is_own_global(const icx_t * cx, const char * nm) { for (int i = 0; i < cx->ngn; i++) if (cx->gn[i] && !strcmp(cx->gn[i], nm)) return 1; return 0; }
static const char * icn_static_mangled(const icx_t * cx, const char * nm) { if (!cx->pname) return NULL; size_t pl = strlen(cx->pname), nl = strlen(nm); for (int i = 0; i < g_icn_synth_excl.n; i++) { const char * m = LC_AT(&g_icn_synth_excl, const char *, i); if (m && !strncmp(m, cx->pname, pl) && !strncmp(m + pl, "__STATIC__", 10) && !strcmp(m + pl + 10, nm) && strlen(m) == pl + 10 + nl) return m; } return NULL; }
static int icn_is_proc(const char * nm) { for (int i = 0; i < g_stage2.proc_count; i++) if (g_stage2.proc_table[i].name && !strcmp(g_stage2.proc_table[i].name, nm)) return 1; return 0; }
static tree_t * icn_variable_lit_target(const icx_t * cx, const char * vn) { if (vn[0] == '&') return icn_kw_assignable(vn) ? icn_variable_lit_tree(vn) : NULL; if (icn_is_local(cx, vn)) return icn_variable_lit_tree(vn); { const char * m = icn_static_mangled(cx, vn); if (m) return icn_variable_lit_tree(m); } if (icn_is_own_global(cx, vn) || icn_is_proc(vn)) return icn_variable_lit_tree(vn); return NULL; }
static int icn_gen_wiring(const IR_t * t) {
    if (!t) return 0;
    if (ir_is_generator_kind(t->op)) return 1;
    if (t->op == IR_SCAN_TAB || t->op == IR_SCAN_MOVE) return 1;
    return t->op == IR_CALL && IR_LIT(t).sval && (!strcmp(IR_LIT(t).sval, "tab") || !strcmp(IR_LIT(t).sval, "move"));
}
static void γ_to(IR_t * nd, IR_t * t) { if (t && icn_gen_wiring(t)) lc_γ_to_β(nd, t); else lc_γ_to(nd, t); }
static void icn_mark_γ_fail_conduit(IR_t * nd) { if (nd) { memcpy(nd->γ.sz, "φ", 3); nd->γ.sz[3] = 0; } }
static int icn_γ_is_fail_conduit(const IR_t * nd) { return nd && (unsigned char) nd->γ.sz[0] == 0xcf && (unsigned char) nd->γ.sz[1] == 0x86; }
static void ω_to(IR_t * nd, IR_t * t) { if (t && icn_gen_wiring(t)) lc_ω_to_β(nd, t); else lc_ω_to(nd, t); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * build(icx_t * cx, IR_e op, IR_t * γ, IR_t * ω) {
    IR_t * nd = lc_build(cx->g, op, γ, ω);
    if (γ && icn_gen_wiring(γ)) lc_γ_to_β(nd, γ);
    if (ω && icn_gen_wiring(ω)) lc_ω_to_β(nd, ω);
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * stmt_subj(const tree_t * s) { return lc_stmt_subj(s); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * icn_cset_canon(const char * s, int len, int * out_len) {
    if (!s) { if (out_len) *out_len = 0; return s; }
    unsigned char seen[256]; memset(seen, 0, sizeof seen);
    for (int i = 0; i < len; i++) seen[(unsigned char) s[i]] = 1;
    char buf[257]; int n = 0;
    for (int c = 0; c < 256; c++) if (seen[c]) buf[n++] = (char) c;
    buf[n] = 0; if (out_len) *out_len = n;
    { char * p = (char *) malloc((size_t) n + 1); if (p) { memcpy(p, buf, (size_t) n); p[n] = 0; } return p; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void icn_attach_lit_len(icx_t * cx, IR_t * nd, int len) {
    IR_t * ln = build(cx, IR_LIT_INTEGER, NULL, NULL); IR_LIT(ln).ival = (int64_t) len;
    ir_operand_push(nd, ln);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_proc_is_generator(const char * name) {
    if (!name) return 0; for (int i = 0; i < g_stage2.proc_count; i++) if (g_stage2.proc_table[i].name && !strcmp(g_stage2.proc_table[i].name, name)) return g_stage2.proc_table[i].is_generator;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_call_allow_gen(const char * name) { return name && (icn_proc_is_generator(name) || !strcmp(name, "find") || !strcmp(name, "upto") || !strcmp(name, "key")); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int augop_code(int aop) {
    switch (aop) {
    case AUGOP_ADD: return 0; case AUGOP_SUB: return 1; case AUGOP_MUL: return 2; case AUGOP_DIV: return 3; case AUGOP_MOD: return 4;
    case AUGOP_POW: return 18; case AUGOP_CONCAT: return 11; case AUGOP_EQ: return 9; case AUGOP_SEQ: return 16;
    case AUGOP_LT: return 5; case AUGOP_LE: return 6; case AUGOP_GT: return 7; case AUGOP_GE: return 8; case AUGOP_NE: return 10;
    case AUGOP_SLT: return 12; case AUGOP_SLE: return 13; case AUGOP_SGT: return 14; case AUGOP_SGE: return 15; case AUGOP_SNE: return 17;
    case AUGOP_CSET_UNION: return 19; case AUGOP_CSET_DIFF: return 20; case AUGOP_CSET_INTER: return 21;
    default: return 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_unop_tt(tree_e tt) {
    switch (tt) {
    case TT_MNS: case TT_PLS: case TT_SIZE: case TT_NONNULL: case TT_CSET_COMPL: case TT_INTERROGATE: return 1;
    default: return 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_resumable(const tree_t * t) {
    if (!t) return 0; if (t->t == TT_STMT) t = stmt_subj(t); if (!t) return 0;
    if (t->t == TT_BANG_BINARY) return 1;
    if (t->t == TT_FIELD || t->t == TT_NULL || t->t == TT_NONNULL) return (t->n > 0) ? is_resumable(t->c[0]) : 0;
    if (is_unop_tt(t->t)) return (t->n > 0) ? is_resumable(t->c[0]) : 0;
    if (t->t == TT_FNC) { const char * nm = (t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR) ? t->c[0]->v.sval : NULL; return nm ? icn_call_allow_gen(nm) : 1; }
    if (lc_is_binop(t->t)) { for (int i = 0; i < t->n; i++) if (is_resumable(t->c[i])) return 1; return 0; }
    if (t->t == TT_IDX) { for (int i = 0; i < t->n; i++) if (is_resumable(t->c[i])) return 1; return 0; }
    if (t->t == TT_ASSIGN) { if (t->n > 0 && t->c[0] && t->c[0]->t == TT_ITERATE) return 1; return (t->n > 1) ? is_resumable(t->c[1]) : 0; }
    if (t->t == TT_SWAP) { for (int i = 0; i < t->n; i++) if (is_resumable(t->c[i])) return 1; return 0; }
    switch (t->t) {
    case TT_IF: case TT_SCAN: case TT_EVERY: case TT_TO: case TT_TO_BY: case TT_ALTERNATE: case TT_REPEAT: case TT_WHILE: case TT_UNTIL: case TT_REVASSIGN: case TT_ITERATE: return 1;
    default: return 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_tree_is_cursor_mover(const tree_t * a) {
    if (!a) return 0; if (a->t == TT_STMT) a = stmt_subj(a); if (!a) return 0;
    if (a->t == TT_MATCH_UNARY) return 1;
    if (a->t == TT_REVASSIGN && a->n >= 2) return icn_tree_is_cursor_mover(a->c[1]);
    if (a->t != TT_FNC) return 0;
    const char * nm = (a->n > 0 && a->c[0] && a->c[0]->t == TT_VAR) ? a->c[0]->v.sval : NULL;
    return nm && (!strcmp(nm, "tab") || !strcmp(nm, "move"));
}
static IR_t * lower(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
static IR_t * lower_seq(icx_t * cx, const tree_t * t, int argbase, int nargs, IR_t * γ, IR_t * ω, IR_t ** res);
static IR_t * lower_key(icx_t * cx, const tree_t * t, int argbase, int nargs, IR_t * γ, IR_t * ω, IR_t ** res);
static IR_t * lower_lvalue_var(icx_t * cx, const tree_t * t, IR_t * ω, IR_t ** var_res);
static int icn_subtree_has_suspend(const tree_t *n);
static IR_t * lower_if(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
static IR_t * lower_while(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
static IR_t * lower_to(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
static IR_t * lower_make_list(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
static IR_t * lower_every(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
static IR_t * lower_until(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
static IR_t * lower_repeat(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
static IR_t * lower_not(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
static IR_t * lower_alt(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
static IR_t * lower_alt_lv(icx_t * cx, const tree_t * t, IR_t * ω, IR_t ** var_res);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * icn_arg_lower(void * vcx, const tree_t * a, IR_t * F) {
    icx_t * cx = (icx_t *) vcx; IR_t * sps = cx->psucc; IR_t * spf = cx->pfail;
    cx->psucc = NULL; cx->pfail = F;
    IR_t * r = NULL; IR_t * e = lower(cx, a, NULL, F, &r);
    cx->psucc = sps; cx->pfail = spf;
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_graph_t * arg_block(void * vcx, const tree_t * a) { return lc_arg_block(&((icx_t *) vcx)->g, icn_arg_lower, vcx, a); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_arg_is_scan_fn(const tree_t * a) {
    if (!a) return 0; if (a->t == TT_STMT) a = stmt_subj(a); if (!a || a->t != TT_FNC) return 0; const char * nm = (a->n > 0 && a->c[0] && a->c[0]->t == TT_VAR) ? a->c[0]->v.sval : NULL;
    return nm && (!strcmp(nm, "tab") || !strcmp(nm, "move") || !strcmp(nm, "pos") || !strcmp(nm, "any") || !strcmp(nm, "match")
                  || !strcmp(nm, "many") || !strcmp(nm, "upto") || !strcmp(nm, "find") || !strcmp(nm, "bal"));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_call(icx_t * cx, const char * name, const tree_t * t, int argbase, int nargs, IR_t * γ, IR_t * ω, IR_t ** res) {
    if (name && !strcmp(name, "seq")) { IR_t * sq = lower_seq(cx, t, argbase, nargs, γ, ω, res); if (sq) return sq; }
    if (name && !strcmp(name, "key") && nargs == 1) { IR_t * kg = lower_key(cx, t, argbase, nargs, γ, ω, res); if (kg) return kg; }
    if (name && !strcmp(name, "name") && nargs == 1) {
        const tree_t * na = t->c[argbase];
        if (na && na->t == TT_ALTERNATE && na->n >= 1) {
            tree_t * alt = ast_node_new(TT_ALTERNATE);
            for (int k = 0; k < na->n; k++) { tree_t * call = ast_node_new(TT_FNC); tree_t * fnv = ast_node_new(TT_VAR); fnv->v.sval = (char *) "name"; ast_push(call, fnv); ast_push(call, (tree_t *) na->c[k]); ast_push(alt, call); }
            return lower(cx, alt, γ, ω, res);
        }
        if (na && (na->t == TT_VAR || na->t == TT_KEYWORD) && na->v.sval && na->v.sval[0] == '&') {
            IR_t * nd = build(cx, IR_LIT_STRING, γ, ω); IR_LIT(nd).sval = na->v.sval; if (res) *res = nd; return nd;
        }
        if (na && na->t == TT_VAR && na->v.sval && na->v.sval[0] != '&') {
            const char * vn = na->v.sval; const char * st = strstr(vn, "__STATIC__"); if (st && cx->pname && !strncmp(vn, cx->pname, strlen(cx->pname)) && st == vn + strlen(cx->pname)) vn = st + 10;
            IR_t * nd = build(cx, IR_LIT_STRING, γ, ω); IR_LIT(nd).sval = (char *) vn; if (res) *res = nd; return nd;
        }
        IR_t * vr = NULL; IR_t * ve = lower_lvalue_var(cx, na, ω, &vr);
        if (ve && vr) {
            IR_t * ncall = build(cx, IR_CALL, γ, ω); IR_LIT(ncall).sval = (char *) name; if (res) *res = ncall;
            IR_t * argβ = cx->beta;
            lc_γ_to(vr, ncall); ir_operand_push(ncall, vr);
            if (argβ && argβ != ω) { ω_to(ncall, argβ); cx->beta = argβ; }
            else cx->beta = ω;
            return ve;
        }
    }
    int gb = name && ((nargs >= 2 && nargs <= 4 && (!strcmp(name, "find") || !strcmp(name, "upto")))
                   || (nargs == 1 && (!strcmp(name, "find") || !strcmp(name, "upto") || !strcmp(name, "bal"))));
    int is_cursor_mover = name && (!strcmp(name, "tab") || !strcmp(name, "move"));
    int fill_scan_defaults = gb && nargs == 1 && cx->scan_sp == 0 && (!strcmp(name, "find") || !strcmp(name, "upto"));
    IR_t * call = build(cx, icn_proc_is_generator(name) ? IR_PROC_GEN : (gb ? IR_CALL_BUILTIN_GEN : IR_CALL), γ, ω); IR_LIT(call).sval = (char *) name;
    if (res) *res = call;
    int chains = name && (!strcmp(name, "write") || !strcmp(name, "writes"));
    if (!chains) { for (int k = 0; k < nargs; k++) if (is_resumable(t->c[argbase + k])) { if (is_cursor_mover && icn_arg_is_scan_fn(t->c[argbase + k])) continue; chains = 1; break; } }
    IR_t * prev = NULL; IR_t * entry = call; IR_t * aω = ω; IR_t * last_ar = NULL;
    for (int k = 0; k < nargs; k++) {
        const tree_t * a = t->c[argbase + k]; IR_t * ar = NULL;
        IR_t * ae = lower(cx, a, (k == nargs - 1 && !fill_scan_defaults) ? call : NULL, aω, &ar); aω = cx->beta;
        if (k == 0) entry = ae;
        if (prev) lc_γ_to(prev, ae);
        prev = ar;
        if (ar) { ir_operand_push(call, ar); last_ar = ar; }
    }
    if (fill_scan_defaults) {
        IR_t * ks = build(cx, IR_KW_ICON, NULL, aω); IR_LIT(ks).sval = (char *) "&subject";
        IR_t * kp = build(cx, IR_KW_ICON, NULL, aω); IR_LIT(kp).sval = (char *) "&pos";
        if (prev) lc_γ_to(prev, ks); else entry = ks;
        lc_γ_to(ks, kp); ir_operand_push(call, ks); ir_operand_push(call, kp); prev = kp; last_ar = kp;
    }
    if ((icn_proc_is_generator(name) || gb || is_cursor_mover) && last_ar) lc_γ_to(last_ar, call);
    const tree_t * la = (nargs > 0) ? t->c[argbase + nargs - 1] : NULL;
    int la_res = la && is_resumable(la) && !(is_cursor_mover && icn_arg_is_scan_fn(la));
    int chain_live = (aω != ω);
    if (la_res || chain_live) ω_to(call, aω);
    cx->beta = (icn_proc_is_generator(name) || gb || is_cursor_mover) ? call : ((la_res || chain_live) ? aω : (g_postfix_resume ? aω : ω));
    return entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void icn_cat_flatten(const tree_t * t, const tree_t ** elems, int * ne) {
    if (!t) return;
    if (t->t == TT_CAT && t->n == 2) { icn_cat_flatten(t->c[0], elems, ne); icn_cat_flatten(t->c[1], elems, ne); return; }
    if (*ne < 64) elems[(*ne)++] = t;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * icn_scan_seq_nary(icx_t * cx, const tree_t ** elems, int ne, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_graph_t * g = cx->g;
    IR_t * S = lc_build(g, IR_SCAN_SEQUENCE, γ, NULL);
    lc_ω_to(S, ω);
    for (int i = 0; i < ne; i++) {
        int before = g->n;
        IR_t * evz = NULL; IR_t * ei = lower(cx, elems[i], S, S, &evz); (void) evz;
        IR_t * ri = (before < g->n) ? g->all[before] : ei;
        for (int k = before; k < g->n; k++) {
            IR_t * x = g->all[k];
            if (!x) continue;
            if (x->ω.node == S) { memcpy(x->ω.sz, "φ", 3); x->ω.sz[3] = 0; }
            if (x->γ.node == S) { if ((x->op == IR_GOTO && x->ω.node == S) || icn_γ_is_fail_conduit(x)) { memcpy(x->γ.sz, "φ", 3); } else { memcpy(x->γ.sz, "σ", 3); } x->γ.sz[3] = 0; }
        }
        ir_operand_push(S, ei);
        ir_operand_push(S, ri);
    }
    IR_LIT(S).ival = (long) ne;
    cx->beta = S; *res = S;
    return S;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_idx_var(icx_t * cx, const tree_t * t, IR_t * ω, IR_t ** var_res) {
    if (t->n < 2 || !t->c[0]) { IR_t * su = build(cx, IR_SUCCEED, NULL, ω); *var_res = su; return su; }
    IR_t * br = NULL; IR_t * entry;
    const tree_t * b0 = t->c[0];
    if (b0->t == TT_VAR && b0->v.sval && b0->v.sval[0] != '&') {
        IR_t * vr = build(cx, IR_VAR_REF, NULL, ω); IR_LIT(vr).sval = b0->v.sval; br = vr; entry = vr;
    } else if (b0->t == TT_IDX) {
        entry = lower_idx_var(cx, b0, ω, &br);
    } else entry = lower(cx, b0, NULL, ω, &br);
    IR_t * cur = br; IR_t * hook = br; IR_t * prevβ = (b0->t == TT_VAR || b0->t == TT_IDX) ? NULL : cx->beta;
    for (int k = 1; k < t->n; k++) {
        IR_t * ir = NULL; IR_t * ie = lower(cx, t->c[k], NULL, prevβ ? prevβ : ω, &ir); prevβ = cx->beta;
        lc_γ_to(hook, ie);
        IR_t * sub = build(cx, IR_SUBSCRIPT, NULL, prevβ ? prevβ : ω);
        γ_to(ir, sub);
        ir_operand_push(sub, cur); ir_operand_push(sub, ir);
        cur = sub; hook = sub;
    }
    *var_res = cur; return entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_lvalue_var(icx_t * cx, const tree_t * t, IR_t * ω, IR_t ** var_res) {
    if (!t) return NULL;
    if (t->t == TT_ALTERNATE && t->n >= 1) return lower_alt_lv(cx, t, ω, var_res);
    if (t->t == TT_FNC && t->n == 2 && t->c[0] && t->c[0]->t == TT_VAR && t->c[0]->v.sval && !strcmp(t->c[0]->v.sval, "variable") && !icn_is_local(cx, "variable")) { fprintf(stderr, "icon: REFUSE: variable(expr) with a computed name is not an assignable variable in SCRIP yet (a literal name is) -- line %d\n", t->line); exit(2); }
    if (t->t == TT_VAR && t->v.sval && t->v.sval[0] != '&') {
        IR_t * vr = build(cx, IR_VAR_REF, NULL, ω); IR_LIT(vr).sval = t->v.sval; *var_res = vr; return vr;
    }
    if (t->t == TT_IDX) return lower_idx_var(cx, t, ω, var_res);
    if (t->t == TT_ITERATE && t->n > 0 && t->c[0]) {
        IR_t * it = build(cx, IR_ITERATE, NULL, ω); IR_LIT(it).ival = 0; IR_LIT(it).sval = "lv";
        const tree_t * b0 = t->c[0]; IR_t * ar = NULL; IR_t * ae;
        if (b0->t == TT_VAR && b0->v.sval && b0->v.sval[0] != '&') { IR_t * vr = build(cx, IR_VAR_REF, NULL, ω); IR_LIT(vr).sval = b0->v.sval; ar = vr; ae = vr; }
        else ae = lower(cx, b0, NULL, ω, &ar);
        lc_γ_to(ar, it); ir_operand_push(it, ar);
        cx->beta = it;
        *var_res = it; return ae;
    }
    if (t->t == TT_RANDOM && t->n > 0 && t->c[0]) {
        IR_t * rn = build(cx, IR_RANDOM, NULL, ω);
        const tree_t * b0 = t->c[0]; IR_t * ar = NULL; IR_t * ae;
        if (b0->t == TT_VAR && b0->v.sval && b0->v.sval[0] != '&') { IR_t * vr = build(cx, IR_VAR_REF, NULL, ω); IR_LIT(vr).sval = b0->v.sval; ar = vr; ae = vr; }
        else ae = lower(cx, b0, NULL, ω, &ar);
        lc_γ_to(ar, rn); ir_operand_push(rn, ar);
        *var_res = rn; return ae;
    }
    if ((t->t == TT_SECTION || t->t == TT_SECTION_PLUS || t->t == TT_SECTION_MINUS) && t->n >= 3 && t->c[0] && t->c[1] && t->c[2]) {
        int sec_variant = (t->t == TT_SECTION_PLUS) ? 1 : (t->t == TT_SECTION_MINUS) ? 2 : 0;
        IR_t * sec = build(cx, IR_SUBSCRIPT, NULL, ω); IR_LIT(sec).ival = 0; IR_LIT(sec).sval = "lv";
        IR_t * ar = NULL; IR_t * ae; const tree_t * b0 = t->c[0];
        if (b0->t == TT_VAR && b0->v.sval && b0->v.sval[0] != '&') { IR_t * vr = build(cx, IR_VAR_REF, NULL, ω); IR_LIT(vr).sval = b0->v.sval; ar = vr; ae = vr; }
        else if (b0->t == TT_SECTION || b0->t == TT_SECTION_PLUS || b0->t == TT_SECTION_MINUS || b0->t == TT_IDX) ae = lower_lvalue_var(cx, b0, ω, &ar);
        else ae = lower(cx, b0, NULL, ω, &ar);
        IR_t * br = NULL; IR_t * be = lower(cx, t->c[1], NULL, ω, &br); γ_to(ar, be);
        IR_t * i1β = cx->beta;
        IR_t * cr = NULL; IR_t * ce = lower(cx, t->c[2], sec_variant ? NULL : sec, i1β, &cr); γ_to(br, ce);
        if (sec_variant) {
            IR_t * op = build(cx, IR_BINOP, sec, i1β); IR_LIT(op).ival = (sec_variant == 1) ? BINOP_ADD : BINOP_SUB; ir_operand_push(op, br); ir_operand_push(op, cr); γ_to(cr, op); cr = op;
        }
        ir_operand_push(sec, ar); ir_operand_push(sec, br); ir_operand_push(sec, cr);
        lc_γ_to(cr, sec);
        *var_res = sec; return ae;
    }
    if (t->t == TT_FIELD && t->n > 0 && t->c[0]) {
        IR_t * fg = build(cx, IR_FIELD_VAR, NULL, ω);
        IR_LIT(fg).sval = (t->n > 1 && t->c[1]) ? t->c[1]->v.sval : t->v.sval;
        IR_t * br = NULL; IR_t * be = lower(cx, t->c[0], NULL, ω, &br);
        lc_γ_to(br, fg); ir_operand_push(fg, br);
        *var_res = fg; return be;
    }
    if ((t->t == TT_NULL || t->t == TT_NONNULL) && t->n > 0 && t->c[0]) {
        IR_t * clv = NULL; IR_t * ce = lower_lvalue_var(cx, t->c[0], ω, &clv);
        if (!ce || !clv) return NULL;
        IR_t * ut = build(cx, IR_NULLTEST_VAR, NULL, ω); IR_LIT(ut).sval = (t->t == TT_NONNULL) ? "nonnull" : "null";
        ir_operand_push(ut, clv); lc_γ_to(clv, ut);
        if (clv && ir_is_generator_kind(clv->op)) lc_ω_to_β(ut, clv);
        *var_res = ut; return ce;
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_scan_kind_for(const char * s) {
    if (!s) return 0;
    if (!strcmp(s,"tab"))   return (int)IR_SCAN_TAB;
    if (!strcmp(s,"move"))  return (int)IR_SCAN_MOVE;
    if (!strcmp(s,"upto"))  return (int)IR_SCAN_UPTO;
    if (!strcmp(s,"any"))   return (int)IR_SCAN_ANY;
    if (!strcmp(s,"many"))  return (int)IR_SCAN_MANY;
    if (!strcmp(s,"find"))  return (int)IR_SCAN_FIND;
    if (!strcmp(s,"match")) return (int)IR_SCAN_MATCH;
    if (!strcmp(s,"pos"))   return (int)IR_SCAN_POS;
    if (!strcmp(s,"bal"))   return (int)IR_SCAN_BAL;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void icn_retag_scan_body(IR_graph_t * g, int depth) {
    if (!g || !g->all || depth > 16) return;
    for (int i = 0; i < g->n; i++) {
        IR_t * nd = g->all[i];
        if (!nd) continue;
        if ((nd->op == IR_CALL || nd->op == IR_CALL_BUILTIN || nd->op == IR_CALL_ICON || nd->op == IR_CALL_BUILTIN_GEN) && IR_LIT(nd).sval && nd->n_operands == 1) { int k = icn_scan_kind_for(IR_LIT(nd).sval); if (k) nd->op = (IR_e) k; }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lc_key(icx_t * cx, const tree_t * t, const char * kw, IR_t * γ, IR_t * ω, IR_t ** res) {
    const char * id = (kw && kw[0] == '&') ? kw + 1 : kw;
    if (id && !strcmp(id, "line")) { IR_t * nd = build(cx, IR_LIT_INTEGER, γ, ω); IR_LIT(nd).ival = (t && t->line > 0) ? t->line : 0; *res = nd; return nd; }
    if (id && !strcmp(id, "progname")) { char pb[1024]; extern void icn_pp_source_base(char *, size_t); icn_pp_source_base(pb, sizeof pb); IR_t * nd = build(cx, IR_LIT_STRING, γ, ω); IR_LIT(nd).sval = strdup(pb); *res = nd; return nd; }
    if (id && !strcmp(id, "file")) { IR_t * nd = build(cx, IR_LIT_STRING, γ, ω); IR_LIT(nd).sval = (char *) ""; *res = nd; return nd; }
    if (id) {
        const char * cs = !strcmp(id, "ucase") ? "ABCDEFGHIJKLMNOPQRSTUVWXYZ" : !strcmp(id, "lcase") ? "abcdefghijklmnopqrstuvwxyz" : !strcmp(id, "digits") ? "0123456789" : NULL;
        if (!cs && !strcmp(id, "letters")) cs = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
        if (cs) { int cn; const char * canon = icn_cset_canon(cs, (int) strlen(cs), &cn); IR_t * nd = build(cx, IR_LIT_CHARSET, γ, ω); IR_LIT(nd).sval = canon; icn_attach_lit_len(cx, nd, cn); *res = nd; return nd; }
    }
    int is_gen_kw = id && (!strcmp(id, "features") || !strcmp(id, "regions") || !strcmp(id, "storage") || !strcmp(id, "collections") || !strcmp(id, "allocated"));
    IR_t * nd = build(cx, is_gen_kw ? IR_KW_ICON_GEN : IR_KW_ICON, γ, ω); IR_LIT(nd).sval = (char *) kw;
    if (is_gen_kw) {
        cx->beta = nd; *res = nd; return nd;
    }
    *res = nd; return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_t * dummy = NULL; if (!res) res = &dummy;
    cx->beta = ω;
    if (!t) { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    if (lc_is_binop(t->t)) {
        if (t->t == TT_CAT && t->n == 2 && (icn_arg_is_scan_fn(t->c[0]) || icn_arg_is_scan_fn(t->c[1]))) {
            const tree_t * elems[64]; int ne = 0; icn_cat_flatten(t, elems, &ne);
            int all_scan = (ne >= 2); for (int i = 0; i < ne; i++) if (!icn_arg_is_scan_fn(elems[i])) { all_scan = 0; break; }
            if (all_scan) return icn_scan_seq_nary(cx, elems, ne, γ, ω, res);
        }
        { int64_t fb = 0; int fr = 0; if (icn_const_step(t, &fb, &fr) && fr) { IR_t * nd = build(cx, IR_LIT_REAL, γ, ω); double d; memcpy(&d, &fb, 8); IR_LIT(nd).dval = d; *res = nd; return nd; } }
        int64_t bcode = lc_binop_code(t->t); if (bcode == BINOP_CONCAT) bcode = BINOP_CONCAT_FRACDIGIT;
        int is_relop = (bcode >= BINOP_LT && bcode <= BINOP_NE) || (bcode >= BINOP_SLT && bcode <= BINOP_SNE) || bcode == BINOP_EQV || bcode == BINOP_NEQV;
        int is_arith = (bcode >= BINOP_ADD && bcode <= BINOP_MOD) || bcode == BINOP_POW;
        int alit = 0, blit = 0; { int64_t fb = 0; int fr = 0; alit = icn_const_step(t->c[0], &fb, &fr); fb = 0; fr = 0; blit = icn_const_step(t->c[1], &fb, &fr); }
        IR_t * op = build(cx, is_relop ? IR_BINOP_TEST : IR_BINOP, γ, ω); IR_LIT(op).ival = bcode;
        IR_t * cb2 = (is_arith && !blit) ? build(cx, IR_COERCE_NUMERIC, op, ω) : NULL; if (cb2) IR_LIT(cb2).ival = 0;
        IR_t * ca2 = (is_arith && !alit) ? build(cx, IR_COERCE_NUMERIC, cb2 ? cb2 : op, ω) : NULL; if (ca2) IR_LIT(ca2).ival = 0;
        IR_t * bsucc = ca2 ? ca2 : (cb2 ? cb2 : op);
        IR_t * lr = NULL, * rr = NULL; IR_t * ea = lower(cx, t->c[0], NULL, ω, &lr); IR_t * lβ = cx->beta; IR_t * eb = lower(cx, t->c[1], bsucc, lβ, &rr);
        IR_t * rβ = cx->beta;
        IR_t * opfail = (rβ && rβ != ω && rβ != op) ? rβ : ((lβ && lβ != ω && lβ != op) ? lβ : NULL);
        if (is_relop && opfail) ω_to(op, opfail);
        lc_γ_to(lr, eb);
        if (ca2) { ir_operand_push(ca2, lr); ir_operand_push(ca2, rr); }
        if (cb2) { ir_operand_push(cb2, rr); ir_operand_push(cb2, lr); }
        ir_operand_push(op, ca2 ? ca2 : lr); ir_operand_push(op, cb2 ? cb2 : rr);
        cx->beta = (rβ && rβ != ω && rβ != op) ? rβ : ((lβ && lβ != ω && lβ != op) ? lβ : ω);
        *res = op; return ea; }
    if (is_unop_tt(t->t)) {
        {
            int64_t fb = 0; int fr = 0;
            if (icn_const_step(t, &fb, &fr)) {
                if (fr) {
                    IR_t * nd = build(cx, IR_LIT_REAL, γ, ω); double d; memcpy(&d, &fb, 8); IR_LIT(nd).dval = d; *res = nd; return nd;
                }
                IR_t * nd = build(cx, IR_LIT_INTEGER, γ, ω); IR_LIT(nd).ival = fb; *res = nd; return nd;
            }
        }
        IR_e uop_kind = (t->t == TT_NONNULL) ? IR_UNOP_TEST : IR_UNOP;
        IR_t * op = build(cx, uop_kind, γ, ω); IR_LIT(op).ival = (long long) t->t; IR_t * orr = NULL;
        if (t->t == TT_MNS || t->t == TT_PLS) {
            IR_t * co = build(cx, IR_COERCE_NUMERIC, op, ω); IR_LIT(co).ival = 0;
            IR_t * ea = lower(cx, t->c[0], co, ω, &orr);
            ir_operand_push(co, orr); ir_operand_push(op, co); *res = op; return ea;
        }
        IR_t * ea = lower(cx, t->c[0], op, ω, &orr); ir_operand_push(op, orr); *res = op;
        IR_t * uβ = cx->beta;
        if (uop_kind == IR_UNOP_TEST && uβ && uβ != ω && uβ != op) { ω_to(op, uβ); cx->beta = uβ; }
        return ea; }
    switch (t->t) {
    case TT_ILIT: { IR_t * nd = build(cx, IR_LIT_INTEGER, γ, ω); IR_LIT(nd).ival = t->v.ival; *res = nd; return nd; }
    case TT_FLIT: { IR_t * nd = build(cx, IR_LIT_REAL, γ, ω); IR_LIT(nd).dval = t->v.dval; *res = nd; return nd; }
    case TT_QLIT: { IR_t * nd = build(cx, IR_LIT_STRING, γ, ω); IR_LIT(nd).sval = t->v.sval; icn_attach_lit_len(cx, nd, t->slen); *res = nd; return nd; }
    case TT_CSET: { int cn; const char * canon = icn_cset_canon(t->v.sval, t->slen, &cn); IR_t * nd = build(cx, IR_LIT_CHARSET, γ, ω); IR_LIT(nd).sval = canon; icn_attach_lit_len(cx, nd, cn); *res = nd; return nd; }
    case TT_NULL: {
        if (t->n > 0 && t->c[0]) {
            IR_t * op = build(cx, IR_UNOP_TEST, γ, ω); IR_LIT(op).ival = (long long) TT_NULL; IR_t * orr = NULL; IR_t * ea = lower(cx, t->c[0], op, ω, &orr); ir_operand_push(op, orr); *res = op;
            IR_t * nβ = cx->beta;
            if (nβ && nβ != ω && nβ != op) { ω_to(op, nβ); cx->beta = nβ; }
            return ea;
        } IR_t * nd = build(cx, IR_FAIL, γ, ω);
        *res = nd; return nd;
    }
    case TT_BANG_BINARY: {
        const tree_t * lhs = (t->n > 0) ? t->c[0] : NULL;
        const tree_t * rhs = (t->n > 1) ? t->c[1] : NULL;
        tree_t * callee;
        if (lhs && lhs->t == TT_VAR && lhs->v.sval && lhs->v.sval[0] != '&' && !icn_is_local(cx, lhs->v.sval)) {
            callee = ast_node_new(TT_QLIT); callee->v.sval = lhs->v.sval; callee->slen = (int) strlen(lhs->v.sval);
        } else callee = (tree_t *) lhs;
        IR_t * cr = NULL; IR_t * ce = lower(cx, callee, NULL, ω, &cr);
        IR_t * prevβ = cx->beta;
        IR_t * nd = build(cx, IR_CALL_VALUE, γ, NULL);
        ir_operand_push(nd, cr);
        if (rhs) {
            IR_LIT(nd).sval = (char *) "apply";
            IR_t * ar = NULL; IR_t * ae = lower(cx, rhs, NULL, prevβ ? prevβ : ω, &ar); prevβ = cx->beta;
            lc_γ_to(cr, ae);
            ir_operand_push(nd, ar);
            ω_to(nd, prevβ ? prevβ : ω);
            lc_γ_to(ar, nd);
        } else {
            ω_to(nd, prevβ ? prevβ : ω);
            lc_γ_to(cr, nd);
        }
        cx->beta = nd;
        *res = nd; return ce;
    }
    case TT_VAR: { if (t->v.sval && t->v.sval[0] == '&') return lc_key(cx, t, t->v.sval, γ, ω, res); IR_t * nd = build(cx, IR_VAR, γ, ω); IR_LIT(nd).sval = t->v.sval; *res = nd; return nd; }
    case TT_KEYWORD: return lc_key(cx, t, t->v.sval, γ, ω, res);
    case TT_NUL: { IR_t * nd = build(cx, IR_VAR, γ, ω); IR_LIT(nd).sval = (char *) "&null"; *res = nd; return nd; }
    case TT_FIELD: { IR_t * nd = build(cx, IR_FIELD_GET, γ, ω);
        IR_LIT(nd).sval = (t->n > 1 && t->c[1]) ? t->c[1]->v.sval : t->v.sval;
        IR_t * br = NULL; IR_t * ea = lower(cx, t->c[0], nd, ω, &br); ir_operand_push(nd, br); *res = nd; return ea; }
    case TT_FNC: { const tree_t * fn = (t->n > 0) ? t->c[0] : NULL;
        { const char * vn = icn_variable_lit(cx, t);
          if (vn) { tree_t * tg = icn_variable_lit_target(cx, vn); if (!tg) return lc_key(cx, t, "&fail", γ, ω, res); if (tg->t == TT_KEYWORD) return lc_key(cx, t, tg->v.sval, γ, ω, res); return lower(cx, tg, γ, ω, res); } }
        if (!fn || (fn->t == TT_VAR && fn->v.sval && fn->v.sval[0] != '&' && !icn_is_local(cx, fn->v.sval))) {
            const char * nm = (fn && fn->t == TT_VAR) ? fn->v.sval : "?";
            return lower_call(cx, nm, t, 1, t->n - 1, γ, ω, res);
        }
        IR_t * cr = NULL; IR_t * ce = lower(cx, fn, NULL, ω, &cr);
        IR_t * prevβ = cx->beta;
        IR_t * nd = build(cx, IR_CALL_VALUE, γ, NULL);
        IR_t * prev = cr;
        ir_operand_push(nd, cr);
        for (int i = 1; i < t->n; i++) {
            IR_t * ar = NULL; IR_t * ae = lower(cx, t->c[i], NULL, prevβ ? prevβ : ω, &ar); prevβ = cx->beta;
            lc_γ_to(prev, ae); prev = ar;
            ir_operand_push(nd, ar);
        }
        ω_to(nd, prevβ ? prevβ : ω);
        lc_γ_to(prev, nd);
        cx->beta = nd;
        *res = nd; return ce; }
    case TT_RANDOM: {
        IR_t * rn = build(cx, IR_RANDOM, NULL, ω);
        const tree_t * b0 = (t->n > 0) ? t->c[0] : NULL; if (!b0) { IR_t * f = build(cx, IR_FAIL, γ, ω); *res = f; return f; }
        IR_t * ar = NULL; IR_t * ae;
        if (b0->t == TT_VAR && b0->v.sval && b0->v.sval[0] != '&') { IR_t * vr = build(cx, IR_VAR_REF, NULL, ω); IR_LIT(vr).sval = b0->v.sval; ar = vr; ae = vr; }
        else ae = lower(cx, b0, NULL, ω, &ar);
        lc_γ_to(ar, rn); ir_operand_push(rn, ar);
        IR_t * drf = build(cx, IR_DEREF, γ, ω); lc_γ_to(rn, drf); ir_operand_push(drf, rn);
        *res = drf; return ae; }
    case TT_IDX: {
        IR_t * vr = NULL; IR_t * e = lower_idx_var(cx, t, ω, &vr);
        IR_t * idxβ = cx->beta;
        IR_t * drf = build(cx, IR_DEREF, γ, idxβ ? idxβ : ω); lc_γ_to(vr, drf); ir_operand_push(drf, vr);
        *res = drf; return e; }
    case TT_DEREF: {
        const tree_t * b0 = (t->n > 0) ? t->c[0] : NULL; if (!b0) { IR_t * f = build(cx, IR_FAIL, γ, ω); *res = f; return f; }
        IR_t * ar = NULL; IR_t * ae = lower(cx, b0, NULL, ω, &ar);
        IR_t * drβ = cx->beta;
        IR_t * drf = build(cx, IR_DEREF, γ, drβ ? drβ : ω); lc_γ_to(ar, drf); ir_operand_push(drf, ar);
        *res = drf; return ae; }
    case TT_MAKELIST: case TT_VLIST: return lower_make_list(cx, t, γ, ω, res);
    case TT_ASSIGN: {
        const tree_t * lhs = t->c[0]; const tree_t * rhs = t->c[1];
        { const char * vn = icn_variable_lit(cx, lhs);
          if (vn) { tree_t * tg = icn_variable_lit_target(cx, vn); if (!tg) return lc_key(cx, t, "&fail", γ, ω, res);
                    tree_t * as = ast_node_new(TT_ASSIGN); ast_push(as, tg); ast_push(as, (tree_t *) rhs); return lower(cx, as, γ, ω, res); } }
        if (lhs && (lhs->t == TT_VAR || lhs->t == TT_KEYWORD) && lhs->v.sval && lhs->v.sval[0] == '&') {
            IR_t * ka = build(cx, IR_KW_ASSIGN, γ, ω); IR_LIT(ka).sval = lhs->v.sval;
            IR_t * vr = NULL; IR_t * entry = lower(cx, rhs, ka, ω, &vr); ir_operand_push(ka, vr); *res = ka; return entry; }
        if (lhs && lhs->t == TT_VAR) { IR_t * asn = build(cx, IR_ASSIGN, γ, ω); IR_LIT(asn).sval = lhs->v.sval;
            IR_t * vr = NULL; IR_t * entry = lower(cx, rhs, asn, ω, &vr); ir_operand_push(asn, vr); *res = asn; return entry; }
        { IR_t * b4 = cx->beta;
          IR_t * lv = NULL; IR_t * lve = lhs ? lower_lvalue_var(cx, lhs, ω, &lv) : NULL;
          if (lve && lv) {
            IR_t * lvbeta = (cx->beta != b4) ? cx->beta : NULL;
            IR_t * asn = build(cx, IR_ASSIGN_VAR, γ, ω);
            IR_t * rr = NULL; IR_t * re = lower(cx, rhs, asn, lvbeta ? lvbeta : ω, &rr);
            lc_γ_to(lv, re);
            ir_operand_push(asn, lv); ir_operand_push(asn, rr);
            *res = asn; return lve;
          } }
        IR_t * asn = build(cx, IR_ASSIGN, γ, ω); IR_t * lr = NULL, * rr = NULL;
        IR_t * eb = lower(cx, rhs, asn, ω, &rr); IR_t * ea = lower(cx, lhs, eb, ω, &lr);
        ir_operand_push(asn, rr); ir_operand_push(asn, lr); *res = asn; return ea;
    }
    case TT_AUGOP: {
        const tree_t * lhs = t->c[0]; const tree_t * rhs = t->c[1]; int bc = augop_code((int) t->v.ival);
        if ((int) t->v.ival == AUGOP_SCAN && lhs && rhs) {
            tree_t * scn = ast_node_new(TT_SCAN); ast_push(scn, (tree_t *) lhs); ast_push(scn, (tree_t *) rhs);
            tree_t * as = ast_node_new(TT_ASSIGN); ast_push(as, (tree_t *) lhs); ast_push(as, scn);
            return lower(cx, as, γ, ω, res);
        }
        if (lhs && lhs->t == TT_VAR && rhs && augop_binop_tt((int) t->v.ival) != (tree_e) 0) {
            tree_t * bo = ast_node_new(augop_binop_tt((int) t->v.ival)); ast_push(bo, (tree_t *) lhs); ast_push(bo, (tree_t *) rhs);
            tree_t * as = ast_node_new(TT_ASSIGN); ast_push(as, (tree_t *) lhs); ast_push(as, bo);
            return lower(cx, as, γ, ω, res);
        }
        { IR_t * b4 = cx->beta;
          IR_t * lv = NULL; IR_t * lve = (lhs && rhs) ? lower_lvalue_var(cx, lhs, ω, &lv) : NULL;
          if (lve && lv) {
            IR_t * asn = build(cx, IR_ASSIGN_VAR, γ, ω);
            IR_t * op = build(cx, IR_BINOP, asn, ω); IR_LIT(op).ival = bc;
            IR_t * dr = build(cx, IR_DEREF, NULL, ω);
            ir_operand_push(dr, lv);
            lc_γ_to(lv, dr);
            IR_t * lvbeta = (cx->beta != b4) ? cx->beta : NULL;
            IR_t * rr = NULL; IR_t * re = lower(cx, rhs, op, lvbeta ? lvbeta : ω, &rr);
            lc_γ_to(dr, re);
            ir_operand_push(op, dr); ir_operand_push(op, rr);
            ir_operand_push(asn, lv); ir_operand_push(asn, op);
            *res = asn; return lve;
          }
        }
        IR_t * op = build(cx, IR_BINOP, γ, ω); IR_LIT(op).ival = bc; IR_t * lr = NULL, * rr = NULL;
        IR_t * ea = lower(cx, lhs, NULL, ω, &lr); IR_t * eb = lower(cx, rhs, op, ω, &rr); γ_to(lr, eb); *res = op; return ea;
    }
    case TT_RETURN: { IR_t * ret = build(cx, IR_RETURN, cx->psucc ? cx->psucc : γ, ω);
        if (t->n > 0 && t->c[0]) { IR_t * vr = NULL; IR_t * entry = lower(cx, t->c[0], ret, cx->pfail ? cx->pfail : ω, &vr); ir_operand_push(ret, vr); *res = ret; return entry; }
        *res = ret; return ret; }
    case TT_PROC_FAIL: { IR_t * nd = build(cx, IR_FAIL, γ, ω); *res = nd; return nd; }
    case TT_LOOP_BREAK: {
        int k = 1; const tree_t * ch = (t->n >= 1) ? t->c[0] : NULL;
        while (ch && ch->t == TT_LOOP_BREAK) { k++; ch = (ch->n >= 1) ? ch->c[0] : NULL; }
        if (!ch) {
            int idx = cx->loop_sp - k;
            IR_t * lx = (idx >= 0 && idx < ICN_LOOP_STK_MAX) ? cx->loop_stk_exit[idx] : NULL;
            if (!lx) lx = cx->loop_exit;
            if (!lx) { IR_t * nd = build(cx, IR_FAIL, γ, ω); *res = nd; return nd; }
            IR_t * nullv = build(cx, IR_VAR, NULL, lx); IR_LIT(nullv).sval = (char *) "&null";
            IR_t * asn0 = build(cx, IR_ASSIGN, lx, lx); IR_LIT(asn0).sval = (char *) "__break_result";
            ir_operand_push(asn0, nullv); γ_to(nullv, asn0);
            if (cx->scan_sp > cx->loop_next_ssp) { IR_t * tgt = nullv;
                for (int _k = cx->loop_next_ssp; _k < cx->scan_sp && _k < 16; _k++) { IR_t * lv = build(cx, IR_SCAN, NULL, NULL); icn_mark_γ_fail_conduit(lv); IR_LIT(lv).dval = 3.0; ir_operand_push(lv, cx->scan_stk_enter[_k]); lc_γ_to(lv, tgt); lc_ω_to(lv, tgt); tgt = lv; }
                *res = tgt; return tgt; }
            *res = nullv; return nullv;
        } else {
            IR_t * cur_exit = cx->loop_exit;
            IR_t * exit_goto = cur_exit ? build(cx, IR_GOTO, cur_exit, cur_exit) : build(cx, IR_FAIL, γ, ω);
            IR_t * cur_fail = cx->loop_fail;
            IR_t * fail_goto = cur_fail ? build(cx, IR_GOTO, cur_fail, cur_fail) : exit_goto;
            int idx = cx->loop_sp - 2; IR_t * se = cx->loop_exit, * sn = cx->loop_next, * sf = cx->loop_fail;
            if (idx >= 0 && idx < ICN_LOOP_STK_MAX) { cx->loop_exit = cx->loop_stk_exit[idx]; cx->loop_next = cx->loop_stk_next[idx]; cx->loop_fail = cx->loop_stk_fail[idx]; }
            IR_t * ev = NULL; IR_t * en = lower(cx, ch, NULL, fail_goto, &ev);
            cx->loop_exit = se; cx->loop_next = sn; cx->loop_fail = sf;
            IR_t * asn = build(cx, IR_ASSIGN, exit_goto, exit_goto); IR_LIT(asn).sval = (char *) "__break_result";
            { IR_t * evf = icn_arm_result(ev); if (evf) { ir_operand_push(asn, evf); γ_to(evf, asn); } else if (!ev) γ_to(en, asn); }
            cx->loop_break_beta = cx->beta;
            if (cx->scan_sp > cx->loop_next_ssp) { IR_t * tgt = en;
                for (int _k = cx->loop_next_ssp; _k < cx->scan_sp && _k < 16; _k++) { IR_t * lv = build(cx, IR_SCAN, NULL, NULL); icn_mark_γ_fail_conduit(lv); IR_LIT(lv).dval = 3.0; ir_operand_push(lv, cx->scan_stk_enter[_k]); lc_γ_to(lv, tgt); lc_ω_to(lv, tgt); tgt = lv; }
                *res = tgt; return tgt; }
            *res = en; return en;
        }
    }
    case TT_LOOP_NEXT: { IR_t * ln = cx->loop_next; IR_t * nd;
        if (ln && cx->scan_sp > cx->loop_next_ssp) {
            for (int _k = cx->loop_next_ssp; _k < cx->scan_sp && _k < 16; _k++) { IR_t * lv = build(cx, IR_SCAN, NULL, NULL); icn_mark_γ_fail_conduit(lv); ir_operand_push(lv, cx->scan_stk_enter[_k]); lc_γ_to(lv, ln); lc_ω_to(lv, ln); ln = lv; }
            nd = build(cx, IR_GOTO, NULL, NULL); lc_γ_to(nd, ln); lc_ω_to(nd, ln);
        } else nd = ln ? build(cx, IR_GOTO, ln, ln) : build(cx, IR_FAIL, γ, ω);
        *res = nd; return nd; }
    case TT_LOCAL: case TT_STATIC_DECL: { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    case TT_INITIAL: {
        IR_t * ini = build(cx, IR_INITIAL, NULL, γ);
        if (t->n > 0 && t->c[0]) {
            IR_t * br = NULL;
            IR_t * be = lower(cx, t->c[0], γ, γ, &br);
            lc_γ_to(ini, be);
            ir_operand_push(ini, br);
        } else {
            lc_γ_to(ini, γ);
        }
        *res = ini; return ini; }
    case TT_SUSPEND: { IR_t * sn = build(cx, IR_SUSPEND, cx->psucc ? cx->psucc : γ, ω); IR_LIT(sn).dval = 1.0;
        IR_t * ev = NULL; IR_t * e_entry = sn; IR_t * eβ = NULL;
        if (t->n > 0 && t->c[0]) { cx->beta = ω; e_entry = lower(cx, t->c[0], sn, ω, &ev); if (cx->beta && cx->beta != ω) eβ = cx->beta; }
        ir_operand_push(sn, ev);
        IR_t * rrt; if (t->n > 1 && t->c[1]) { IR_t * dv = NULL; rrt = lower(cx, t->c[1], eβ ? eβ : γ, eβ ? eβ : γ, &dv); }
        else rrt = eβ ? eβ : γ;
        if (cx->scan_sp > 0) {
            IR_t * ytgt = cx->psucc ? cx->psucc : γ;
            IR_t * lvs[16]; int nlv = 0;
            for (int _k = cx->scan_sp - 1; _k >= 0; _k--) {
                IR_t * lv = build(cx, IR_SCAN, NULL, NULL);
                IR_LIT(cx->scan_stk_enter[_k]).dval = 3.0;
                ir_operand_push(lv, cx->scan_stk_enter[_k]); ir_operand_push(lv, NULL); ir_operand_push(lv, NULL);
                if (nlv) { lc_γ_to(lvs[nlv - 1], lv); lc_ω_to(lvs[nlv - 1], lv); }
                lvs[nlv++] = lv;
            }
            lc_γ_to(lvs[nlv - 1], ytgt); lc_ω_to(lvs[nlv - 1], ytgt);
            for (int _i = 0; _i < nlv; _i++) lvs[_i]->operands[2] = (_i > 0) ? lvs[_i - 1] : rrt;
            lc_γ_to(sn, lvs[0]);
            rrt = lvs[nlv - 1];
        }
        ir_operand_push(sn, rrt);
        *res = sn; return e_entry; }
    case TT_CASE: {
        if (t->n < 1 || !t->c[0]) { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
        static const char * CVAR = "__case_result";
        IR_t * cvar = build(cx, IR_VAR, γ, ω); IR_LIT(cvar).sval = (char *) CVAR;
        IR_t * sr = NULL; IR_t * se = lower(cx, t->c[0], NULL, ω, &sr);
        int nc = t->n - 1;
        if (nc <= 0) { cx->beta = ω; *res = cvar; return se; }
        int npairs   = nc / 2;
        int has_dflt = (nc % 2 == 1);
        IR_t * chain_next = ω;
        if (has_dflt) {
            IR_t * dv = NULL; IR_t * de = lower(cx, t->c[t->n - 1], NULL, ω, &dv);
            IR_t * dasn = build(cx, IR_ASSIGN, cvar, ω); IR_LIT(dasn).sval = (char *) CVAR;
            { IR_t * dvf = icn_arm_result(dv); if (dvf) ir_operand_push(dasn, dvf); }
            { IR_t * dvf = icn_arm_result(dv); if (dvf) γ_to(dvf, dasn); else if (!dv) γ_to(de, dasn); }
            chain_next = de;
        }
        for (int i = npairs - 1; i >= 0; i--) {
            int ki = 1 + i * 2; int bi = ki + 1;
            cx->beta = ω;
            IR_t * ksel_ω = is_resumable(t->c[ki]) ? chain_next : ω;
            IR_t * kn = NULL; IR_t * ke = lower(cx, t->c[ki], NULL, ksel_ω, &kn);
            IR_t * kβ = cx->beta;
            IR_t * bv = NULL; IR_t * be = lower(cx, t->c[bi], NULL, ω, &bv);
            IR_t * asn = build(cx, IR_ASSIGN, cvar, ω); IR_LIT(asn).sval = (char *) CVAR;
            { IR_t * bvf = icn_arm_result(bv); if (bvf) ir_operand_push(asn, bvf); }
            { IR_t * bvf = icn_arm_result(bv); if (bvf) γ_to(bvf, asn); else if (!bv) γ_to(be, asn); }
            IR_t * idc = build(cx, IR_CALL_BUILTIN, be, chain_next);
            IR_LIT(idc).sval = (char *) "IDENTICAL";
            if (is_resumable(t->c[bi]) || (be && ir_is_generator_kind(be->op))) lc_γ_to_α(idc, be);
            ir_operand_push(idc, sr);
            ir_operand_push(idc, kn);
            γ_to(kn, idc);
            if (kβ && kβ != ω) lc_ω_to_β(idc, kβ);
            IR_t * ke_target = ke ? ke : idc;
            if (ke && is_resumable(t->c[ki])) {
                IR_t * KENT = build(cx, IR_GOTO, NULL, NULL);
                lc_γ_to_α(KENT, ke); lc_ω_to_α(KENT, ke);
                ke_target = KENT;
            }
            chain_next = ke_target;
        }
        γ_to(sr, chain_next);
        cx->beta = ω; *res = cvar; return se; }
    case TT_CONJ:
    case TT_SEQ_EXPR: {
        lc_vec Sv; lc_vec_init(&Sv, (int) sizeof(const tree_t *));
        for (int i = 0; i < t->n; i++) { const tree_t * s = t->c[i]; if (s && s->t == TT_STMT) s = stmt_subj(s); if (s) lc_vec_push(&Sv, &s); }
        const tree_t ** S = (const tree_t **) Sv.data; int k = Sv.n;
        if (k == 0) { IR_t * su = build(cx, IR_SUCCEED, γ, ω); *res = su; return su; }
        if (k == 1) return lower(cx, S[0], γ, ω, res);
        IR_t * SEQX = build(cx, IR_CONJUNCTION, γ, ω);
        IR_t ** val = (IR_t **) calloc((size_t) k, sizeof(IR_t *)); IR_t ** ent = (IR_t **) calloc((size_t) k, sizeof(IR_t *)); IR_t * succ = SEQX;
        if (t->t == TT_SEQ_EXPR) {
            IR_t * failt = ω; IR_t * last_beta = ω; IR_t * rb = NULL;
            for (int i = k - 1; i >= 0; i--) {
                val[i] = NULL; ent[i] = lower(cx, S[i], succ, failt, &val[i]); if (i == k - 1) last_beta = cx->beta; if (!rb && is_resumable(S[i])) rb = cx->beta;
                if (val[i] && val[i]->γ.node == succ) lc_γ_to(val[i], succ); succ = ent[i]; failt = ent[i];
                if (i > 0 && ent[i] && ir_is_generator_kind(ent[i]->op)) {
                    IR_t * SENT = build(cx, IR_GOTO, NULL, NULL);
                    lc_γ_to(SENT, ent[i]); lc_ω_to(SENT, ent[i]);
                    succ = SENT; failt = SENT;
                }
            }
            if (val[k - 1]) ir_operand_push(SEQX, val[k - 1]);
            cx->conj_resumable = rb; cx->beta = last_beta; *res = SEQX; return ent[0];
        }
        IR_t * last_beta = ω; IR_t * rb = NULL;
        IR_t ** bet = (IR_t **) calloc((size_t) k, sizeof(IR_t *));
        IR_t ** jn = (IR_t **) calloc((size_t) k, sizeof(IR_t *));
        for (int i = k - 1; i >= 0; i--) { val[i] = NULL; cx->beta = ω;
            IR_t * failt = ω; if (i > 0) { jn[i] = build(cx, IR_GOTO, ω, ω); failt = jn[i]; }
            ent[i] = lower(cx, S[i], succ, failt, &val[i]); bet[i] = cx->beta;
            if (i == k - 1) last_beta = cx->beta;
            if (!rb && is_resumable(S[i])) rb = cx->beta;
            if (val[i] && val[i]->γ.node == succ) lc_γ_to(val[i], succ);
            succ = ent[i];
        }
        int lr = -1; int lr_cm = 0;
        for (int i = 0; i < k; i++) {
            if (i > 0 && jn[i]) {
                IR_t * tgt = ω; if (lr >= 0) tgt = (bet[lr] && bet[lr] != ω) ? bet[lr] : val[lr];
                if (lr >= 0 && lr_cm && tgt && tgt != ω) { lc_γ_to_β(jn[i], tgt); lc_ω_to_β(jn[i], tgt); }
                else { γ_to(jn[i], tgt); ω_to(jn[i], tgt); }
            }
            if (is_resumable(S[i]) || icn_tree_is_cursor_mover(S[i])) { lr = i; lr_cm = (!is_resumable(S[i]) && icn_tree_is_cursor_mover(S[i])); }
        }
        if (val[k - 1]) ir_operand_push(SEQX, val[k - 1]);
        cx->conj_resumable = rb; cx->beta = last_beta; *res = SEQX; return ent[0];
    }
    case TT_SECTION: case TT_SECTION_PLUS: case TT_SECTION_MINUS: {
        if (t->n < 3 || !t->c[0] || !t->c[1] || !t->c[2]) { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
        int sec_variant = (t->t == TT_SECTION_PLUS) ? 1 : (t->t == TT_SECTION_MINUS) ? 2 : 0;
        IR_t * sec = build(cx, IR_SUBSCRIPT, γ, ω);
        if (sec_variant == 1) IR_LIT(sec).sval = "+";
        else if (sec_variant == 2) IR_LIT(sec).sval = "-";
        IR_t * ar = NULL; IR_t * ae = lower(cx, t->c[0], NULL, ω, &ar); IR_t * aβ = cx->beta;
        IR_t * ωa = (aβ && aβ != ω && aβ != sec) ? aβ : ω;
        IR_t * br = NULL; IR_t * be = lower(cx, t->c[1], NULL, ωa, &br); γ_to(ar, be); IR_t * bβ = cx->beta;
        IR_t * ωb = (bβ && bβ != ωa && bβ != sec) ? bβ : ωa;
        IR_t * cr = NULL; IR_t * ce = lower(cx, t->c[2], sec_variant ? NULL : sec, ωb, &cr); γ_to(br, ce); IR_t * cβ = cx->beta;
        IR_t * ωc = (cβ && cβ != ωb && cβ != sec) ? cβ : ωb;
        if (sec_variant) {
            IR_t * op = build(cx, IR_BINOP, sec, ωc); IR_LIT(op).ival = (sec_variant == 1) ? BINOP_ADD : BINOP_SUB;
            ir_operand_push(op, br); ir_operand_push(op, cr);
            γ_to(cr, op); cr = op;
        }
        ir_operand_push(sec, ar);
        ir_operand_push(sec, br);
        ir_operand_push(sec, cr);
        if (ωc != ω) ω_to(sec, ωc);
        cx->beta = ωc; *res = sec; return ae; }
    case TT_NOT: return lower_not(cx, t, γ, ω, res);
    case TT_ALTERNATE: return lower_alt(cx, t, γ, ω, res);
    case TT_ITERATE: {
        IR_t * nd = build(cx, IR_ITERATE, γ, ω);
        IR_t * orr = NULL; IR_t * ee = lower(cx, (t->n > 0) ? t->c[0] : NULL, NULL, ω, &orr);
        IR_t * opβ = cx->beta;
        ir_operand_push(nd, orr);
        lc_γ_to(orr, nd);
        if (opβ && opβ != ω) lc_ω_to_β(nd, opβ);
        cx->beta = nd; *res = nd; return ee; }
    case TT_IF: return lower_if(cx, t, γ, ω, res);
    case TT_WHILE: return lower_while(cx, t, γ, ω, res);
    case TT_UNTIL: return lower_until(cx, t, γ, ω, res);
    case TT_REPEAT: return lower_repeat(cx, t, γ, ω, res);
    case TT_TO: case TT_TO_BY: return lower_to(cx, t, γ, ω, res);
    case TT_EVERY: return lower_every(cx, t, γ, ω, res);
    case TT_SCAN: {
        if (t->n < 2 || !t->c[0] || !t->c[1]) {
            IR_t * gs = build(cx, IR_FAIL, γ, ω); *res = gs; return gs;
        }
        IR_t * enter = build(cx, IR_SCAN_ENTER, NULL, ω);
        IR_t * leave_succ = build(cx, IR_SCAN, γ, ω);
        IR_t * leave_fail = build(cx, IR_SCAN, ω, ω);
        icn_mark_γ_fail_conduit(leave_fail);
        ir_operand_push(leave_succ, enter);
        ir_operand_push(leave_fail, enter);
        IR_t * succ_tramp = IR_node_alloc(cx->g, IR_GOTO); lc_γ_to(succ_tramp, leave_succ); lc_ω_to(succ_tramp, leave_succ);
        IR_t * fail_tramp = IR_node_alloc(cx->g, IR_GOTO); lc_γ_to(fail_tramp, leave_fail); lc_ω_to(fail_tramp, leave_fail);
        int scan_body_lo = cx->g->n;
        if (cx->scan_sp < 16) cx->scan_stk_enter[cx->scan_sp] = enter; cx->scan_sp++;
        IR_t * bv = NULL; IR_t * b_entry = lower(cx, t->c[1], succ_tramp, fail_tramp, &bv);
        cx->scan_sp--;
        if (IR_LIT(enter).dval == 3.0) { IR_LIT(leave_succ).dval = 3.0; IR_LIT(leave_fail).dval = 3.0; }
        IR_t * body_beta = cx->beta;
        for (int _si = scan_body_lo; _si < cx->g->n; _si++) if (cx->g->all[_si]) cx->g->all[_si]->in_scan = 1;
        if (bv) ir_operand_push(leave_succ, bv);
        icn_retag_scan_body(cx->g, 0);
        lc_γ_to(enter, b_entry);
        cx->beta = ω;
        IR_t * sr = NULL; IR_t * s_entry = lower(cx, t->c[0], enter, ω, &sr);
        ir_operand_push(enter, sr);
        if (sr && sr->op == IR_SCAN && IR_LIT(sr).dval != 3.0) IR_LIT(sr).dval = 4.0;
        IR_t * subj_beta = cx->beta;
        if (subj_beta && subj_beta != ω) { γ_to(leave_fail, subj_beta); ω_to(leave_fail, subj_beta); }
        int body_resumes = (bv && icn_gen_wiring(bv)) || (body_beta && body_beta != ω && body_beta != fail_tramp && body_beta != succ_tramp);
        if (body_resumes && !(bv && icn_gen_wiring(bv))) ir_operand_push(leave_succ, body_beta);
        cx->beta = body_resumes ? leave_succ : ((subj_beta && subj_beta != ω) ? subj_beta : ω);
        *res = leave_succ; return s_entry; }
    case TT_STMT: { const tree_t * sub = stmt_subj(t); if (sub) return lower(cx, sub, γ, ω, res); IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    case TT_CREATE: {
        IR_t * nd = build(cx, IR_CREATE, γ, ω);
        IR_t * coret = build(cx, IR_CORET, NULL, NULL);
        IR_t * cofail = build(cx, IR_COFAIL, NULL, NULL);
        IR_t * bv = NULL; IR_t * b_entry = lower(cx, (t->n > 0) ? t->c[0] : NULL, coret, cofail, &bv);
        IR_t * body_beta = cx->beta;
        if (body_beta) γ_to(coret, body_beta);
        ir_operand_push(coret, bv);
        ir_operand_push(nd, b_entry);
        cx->beta = ω; *res = nd; return nd; }
    case TT_ACTIVATE: {
        IR_t * nd = build(cx, IR_ACTIVATE, γ, ω);
        const tree_t * xt = (t->n > 1) ? t->c[0] : NULL;
        const tree_t * ct = (t->n > 1) ? t->c[1] : t->c[0];
        IR_t * cr = NULL; IR_t * c_entry = lower(cx, ct, nd, ω, &cr);
        IR_t * entry = c_entry;
        ir_operand_push(nd, cr);
        if (xt) { IR_t * xr = NULL; entry = lower(cx, xt, c_entry, ω, &xr); ir_operand_push(nd, xr); }
        cx->beta = ω; *res = nd; return entry; }
    case TT_REPALT: {
        IR_t * nd = build(cx, IR_REPALT, γ, ω);
        lc_γ_to(nd, γ);
        IR_t * er = NULL; IR_t * ee = lower(cx, (t->n > 0) ? t->c[0] : NULL, NULL, ω, &er);
        ir_operand_push(nd, er);
        ir_operand_push(nd, ee);
        cx->beta = nd; *res = nd; return nd; }
    case TT_LIMIT: {
        IR_t * lim = build(cx, IR_LIMIT, γ, ω);
        IR_t * lr = NULL; IR_t * ee = lower(cx, (t->n > 1) ? t->c[1] : NULL, lim, ω, &lr);
        IR_t * inner_beta = cx->beta;
        IR_t * er = NULL; IR_t * ge = lower(cx, (t->n > 0) ? t->c[0] : NULL, lim, ω, &er);
        IR_t * gen_beta = cx->beta;
        ir_operand_push(lim, er);
        ir_operand_push(lim, lr);
        γ_to(lr, ge);
        if (lr && lr->γ.node == ge) lc_γ_to(lr, ge);
        if (er) lc_γ_to(er, lim);
        (void)inner_beta; cx->beta = gen_beta;
        *res = lim; return ee; }
    case TT_LCONCAT: {
        if (t->n < 2 || !t->c[0] || !t->c[1]) { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
        IR_t * op = build(cx, IR_BINOP, γ, ω); IR_LIT(op).ival = BINOP_CONCAT_FRACDIGIT;
        IR_t * lr = NULL; IR_t * ee = lower(cx, t->c[0], NULL, ω, &lr); IR_t * lβ = cx->beta;
        IR_t * rr = NULL; IR_t * re = lower(cx, t->c[1], op, lβ, &rr); γ_to(lr, re);
        ir_operand_push(op, lr); ir_operand_push(op, rr);
        *res = op; return ee; }
    case TT_SWAP: {
        const tree_t * lt = (t->n > 0) ? t->c[0] : NULL; const tree_t * rt2 = (t->n > 1) ? t->c[1] : NULL;
        int plain_l = lt && lt->t == TT_VAR && lt->v.sval && lt->v.sval[0] != '&';
        int plain_r = rt2 && rt2->t == TT_VAR && rt2->v.sval && rt2->v.sval[0] != '&';
        int kw_l = lt && (lt->t == TT_VAR || lt->t == TT_KEYWORD) && lt->v.sval && lt->v.sval[0] == '&';
        int kw_r = rt2 && (rt2->t == TT_VAR || rt2->t == TT_KEYWORD) && rt2->v.sval && rt2->v.sval[0] == '&';
        if (kw_l && kw_r) {
            IR_t * lv_old = build(cx, IR_KW_ICON, NULL, ω); IR_LIT(lv_old).sval = (char *) lt->v.sval;
            IR_t * rv_old = build(cx, IR_KW_ICON, NULL, ω); IR_LIT(rv_old).sval = (char *) rt2->v.sval;
            lc_γ_to(lv_old, rv_old);
            IR_t * write_l = build(cx, IR_KW_ASSIGN, NULL, ω); IR_LIT(write_l).sval = (char *) lt->v.sval;  ir_operand_push(write_l, rv_old);
            IR_t * write_r = build(cx, IR_KW_ASSIGN, γ,    ω); IR_LIT(write_r).sval = (char *) rt2->v.sval; ir_operand_push(write_r, lv_old);
            lc_γ_to(rv_old, write_l); lc_γ_to(write_l, write_r);
            *res = write_l; return lv_old;
        }
        if (kw_l || kw_r) {
            IR_t * kv_old = build(cx, IR_KW_ICON, NULL, ω);
            IR_t * pv_old = build(cx, IR_VAR, NULL, ω);
            const tree_t * kw_tree = kw_l ? lt : rt2;
            const tree_t * pl_tree = kw_l ? rt2 : lt;
            IR_LIT(kv_old).sval = (char *) kw_tree->v.sval;
            IR_LIT(pv_old).sval = (char *) pl_tree->v.sval;
            lc_γ_to(kv_old, pv_old);
            if (kw_l) {
                IR_t * write_kw    = build(cx, IR_KW_ASSIGN, NULL, ω); IR_LIT(write_kw).sval    = kw_tree->v.sval; ir_operand_push(write_kw, pv_old);
                IR_t * write_plain = build(cx, IR_ASSIGN,          γ,    ω); IR_LIT(write_plain).sval = pl_tree->v.sval; ir_operand_push(write_plain, kv_old);
                lc_γ_to(pv_old, write_kw); lc_γ_to(write_kw, write_plain);
                *res = write_kw; return kv_old;
            } else {
                IR_t * write_plain = build(cx, IR_ASSIGN,          NULL, ω); IR_LIT(write_plain).sval = pl_tree->v.sval; ir_operand_push(write_plain, kv_old);
                IR_t * write_kw    = build(cx, IR_KW_ASSIGN,  γ,    ω); IR_LIT(write_kw).sval    = kw_tree->v.sval; ir_operand_push(write_kw, pv_old);
                lc_γ_to(pv_old, write_plain); lc_γ_to(write_plain, write_kw);
                *res = write_plain; return kv_old;
            } }
        if (!(plain_l && plain_r)) {
            IR_t * xr = NULL; IR_t * xe = lower_lvalue_var(cx, lt, ω, &xr);
            IR_t * yr = NULL; IR_t * ye = xe ? lower_lvalue_var(cx, rt2, ω, &yr) : NULL;
            if (xe && ye) {
                lc_γ_to(xr, ye);
                IR_t * nd = build(cx, IR_SWAP_VAR, γ, ω);
                lc_γ_to(yr, nd);
                ir_operand_push(nd, xr); ir_operand_push(nd, yr);
                *res = nd; return xe;
            }
        }
        IR_t * nd = build(cx, IR_SWAP, γ, ω);
        IR_t * lr = NULL; lower(cx, lt, nd, ω, &lr);
        IR_t * rr = NULL; lower(cx, rt2, nd, ω, &rr);
        ir_operand_push(nd, lr); ir_operand_push(nd, rr); *res = nd; return nd; }
    case TT_REVASSIGN: {
        const tree_t * lhs = t->c[0]; const tree_t * rhs = (t->n > 1) ? t->c[1] : NULL;
        if (lhs && (lhs->t == TT_IDX || lhs->t == TT_ITERATE || lhs->t == TT_SECTION || lhs->t == TT_SECTION_PLUS || lhs->t == TT_SECTION_MINUS || lhs->t == TT_FIELD || lhs->t == TT_RANDOM || lhs->t == TT_NULL || lhs->t == TT_NONNULL)) {
            IR_t * b4 = cx->beta;
            IR_t * vr = NULL; IR_t * entry = lower_lvalue_var(cx, lhs, ω, &vr);
            IR_t * lvbeta = (cx->beta != b4) ? cx->beta : NULL;
            IR_t * nd = build(cx, IR_REV_ASSIGN_VAR, γ, lvbeta ? lvbeta : ω);
            IR_t * b5 = cx->beta;
            IR_t * rr = NULL; IR_t * re = lower(cx, rhs, NULL, lvbeta ? lvbeta : ω, &rr);
            IR_t * rbeta = (cx->beta != b5) ? cx->beta : NULL;
            if (rbeta) ω_to(nd, rbeta);
            lc_γ_to(vr, re);
            lc_γ_to(rr, nd);
            ir_operand_push(nd, vr);
            ir_operand_push(nd, rr);
            cx->beta = nd; *res = nd; return entry;
        }
        if (lhs && lhs->t == TT_VAR) {
            IR_t * nd = build(cx, IR_REV_ASSIGN, γ, ω);
            IR_t * lr = NULL; IR_t * le = lower(cx, lhs, NULL, ω, &lr);
            IR_t * b4 = cx->beta;
            IR_t * rr = NULL; IR_t * re = lower(cx, rhs, NULL, ω, &rr);
            IR_t * rbeta = (cx->beta != b4) ? cx->beta : NULL;
            if (rbeta) ω_to(nd, rbeta);
            else if (rr && icn_tree_is_cursor_mover(rhs)) lc_ω_to_β(nd, rr);
            γ_to(lr, re);
            lc_γ_to(rr, nd);
            ir_operand_push(nd, rr);
            ir_operand_push(nd, lr);
            cx->beta = nd; *res = nd; return le;
        }
        IR_t * nd = build(cx, IR_FAIL, γ, ω);
        IR_t * lr = NULL; lower(cx, lhs, nd, ω, &lr);
        IR_t * rr = NULL; lower(cx, rhs, nd, ω, &rr);
        ir_operand_push(nd, lr); ir_operand_push(nd, rr); *res = nd; return nd; }
    case TT_REVSWAP: {
        const tree_t * lt = (t->n > 0) ? t->c[0] : NULL; const tree_t * rt2 = (t->n > 1) ? t->c[1] : NULL;
        int name_l = lt && (lt->t == TT_VAR || lt->t == TT_KEYWORD) && lt->v.sval;
        int name_r = rt2 && (rt2->t == TT_VAR || rt2->t == TT_KEYWORD) && rt2->v.sval;
        if (name_l && name_r) {
            IR_t * nd = build(cx, IR_REV_SWAP, γ, ω); IR_LIT(nd).sval = (char *) lt->v.sval;
            IR_t * rc = build(cx, IR_LIT_STRING, NULL, NULL); IR_LIT(rc).sval = (char *) rt2->v.sval;
            ir_operand_push(nd, rc);
            cx->beta = nd; *res = nd; return nd;
        }
        IR_t * nd = build(cx, IR_FAIL, γ, ω);
        IR_t * lr = NULL; lower(cx, lt, nd, ω, &lr);
        IR_t * rr = NULL; lower(cx, rt2, nd, ω, &rr);
        ir_operand_push(nd, lr); ir_operand_push(nd, rr); *res = nd; return nd; }
    case TT_RECORD: {
        extern void record_register(const char *spec);
        const char * rname = t->v.sval;
        if (rname) {
            char spec[2048]; int pos = 0;
            pos += snprintf(spec + pos, sizeof spec - pos, "%s(", rname);
            for (int k = 0; k < t->n; k++) {
                if (k) pos += snprintf(spec + pos, sizeof spec - pos, ",");
                if (t->c[k] && t->c[k]->v.sval) pos += snprintf(spec + pos, sizeof spec - pos, "%s", t->c[k]->v.sval);
            }
            snprintf(spec + pos, sizeof spec - pos, ")");
            record_register(spec);
        }
        IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    case TT_INVOCABLE:
    case TT_LINK:
    default: { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_while(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    const tree_t * C = (t->n > 0) ? t->c[0] : NULL; const tree_t * B = (t->n > 1) ? t->c[1] : NULL;
    IR_t * W = build(cx, IR_GOTO, ω, ω); γ_to(W, ω); ω_to(W, ω);
    IR_t * slb = cx->loop_break_beta; cx->loop_break_beta = NULL; IR_t * slf = cx->loop_fail; cx->loop_fail = ω; IR_t * sle = cx->loop_exit; IR_t * sln = cx->loop_next; int slns = cx->loop_next_ssp; IR_t * bres = build(cx, IR_VAR, γ, ω); IR_LIT(bres).sval = (char *) "__break_result";
    cx->loop_exit = bres;
    IR_t * cval = NULL; IR_t * centry = lower(cx, C, NULL, W, &cval);
    IR_t * CENT = build(cx, IR_GOTO, NULL, NULL); lc_γ_to_α(CENT, centry); lc_ω_to_α(CENT, centry);
    cx->loop_next = CENT; cx->loop_next_ssp = cx->scan_sp;
    if (cx->loop_sp < ICN_LOOP_STK_MAX) { cx->loop_stk_exit[cx->loop_sp] = cx->loop_exit; cx->loop_stk_next[cx->loop_sp] = cx->loop_next; cx->loop_stk_fail[cx->loop_sp] = cx->loop_fail; } cx->loop_sp++;
    IR_t * bval = NULL; IR_t * b_entry = lower(cx, B, CENT, CENT, &bval);
    cx->loop_sp--;
    lc_γ_to(cval, b_entry);
    cx->loop_exit = sle; cx->loop_next = sln; cx->loop_fail = slf; cx->loop_next_ssp = slns;
    { IR_t * lbb = cx->loop_break_beta; cx->loop_break_beta = slb; if (lbb) { cx->beta = lbb; *res = bres; return centry; } }
    *res = bres; return centry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_until(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    const tree_t * C = (t->n > 0) ? t->c[0] : NULL; const tree_t * B = (t->n > 1) ? t->c[1] : NULL;
    IR_t * U = build(cx, IR_GOTO, ω, ω); γ_to(U, ω); ω_to(U, ω);
    IR_t * BENT = build(cx, IR_GOTO, γ, ω);
    IR_t * slb = cx->loop_break_beta; cx->loop_break_beta = NULL; IR_t * slf = cx->loop_fail; cx->loop_fail = ω; IR_t * sle = cx->loop_exit; IR_t * sln = cx->loop_next; int slns = cx->loop_next_ssp; IR_t * bres = build(cx, IR_VAR, γ, ω); IR_LIT(bres).sval = (char *) "__break_result";
    cx->loop_exit = bres;
    IR_t * cval = NULL; IR_t * centry = lower(cx, C, U, BENT, &cval);
    IR_t * CENT = build(cx, IR_GOTO, NULL, NULL); lc_γ_to_α(CENT, centry); lc_ω_to_α(CENT, centry);
    cx->loop_next = CENT; cx->loop_next_ssp = cx->scan_sp;
    IR_t * b_entry; if (B) { if (cx->loop_sp < ICN_LOOP_STK_MAX) { cx->loop_stk_exit[cx->loop_sp] = cx->loop_exit; cx->loop_stk_next[cx->loop_sp] = cx->loop_next; cx->loop_stk_fail[cx->loop_sp] = cx->loop_fail; } cx->loop_sp++; IR_t * bval = NULL; b_entry = lower(cx, B, CENT, CENT, &bval); cx->loop_sp--; } else b_entry = CENT;
    lc_γ_to(BENT, b_entry); lc_ω_to(BENT, b_entry);
    cx->loop_exit = sle; cx->loop_next = sln; cx->loop_fail = slf; cx->loop_next_ssp = slns;
    { IR_t * lbb = cx->loop_break_beta; cx->loop_break_beta = slb; if (lbb) { cx->beta = lbb; *res = bres; return centry; } }
    *res = bres; return centry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_repeat(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    const tree_t * B = (t->n > 0) ? t->c[0] : NULL;
    IR_t * H = build(cx, IR_GOTO, NULL, ω);
    IR_t * slb = cx->loop_break_beta; cx->loop_break_beta = NULL; IR_t * slf = cx->loop_fail; cx->loop_fail = ω; IR_t * sle = cx->loop_exit; IR_t * sln = cx->loop_next; int slns = cx->loop_next_ssp; IR_t * bres = build(cx, IR_VAR, γ, ω); IR_LIT(bres).sval = (char *) "__break_result";
    cx->loop_exit = bres; cx->loop_next = H; cx->loop_next_ssp = cx->scan_sp;
    if (cx->loop_sp < ICN_LOOP_STK_MAX) { cx->loop_stk_exit[cx->loop_sp] = cx->loop_exit; cx->loop_stk_next[cx->loop_sp] = cx->loop_next; cx->loop_stk_fail[cx->loop_sp] = cx->loop_fail; } cx->loop_sp++;
    IR_t * bval = NULL; IR_t * b_entry = lower(cx, B, H, H, &bval);
    cx->loop_sp--;
    lc_γ_to(H, b_entry); lc_ω_to(H, b_entry);
    cx->loop_exit = sle; cx->loop_next = sln; cx->loop_fail = slf; cx->loop_next_ssp = slns;
    { IR_t * lbb = cx->loop_break_beta; cx->loop_break_beta = slb; if (lbb) { cx->beta = lbb; *res = bres; return H; } }
    cx->beta = γ; *res = bres; return H;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_not(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_t * nullv = build(cx, IR_VAR, γ, ω); IR_LIT(nullv).sval = (char *) "&null";
    IR_graph_t * g = cx->g; int before = g->n;
    IR_t * cr = NULL; IR_t * ce = lower(cx, (t->n > 0) ? t->c[0] : NULL, ω, nullv, &cr);
    for (int k = before; k < g->n; k++) { IR_t * x = g->all[k]; if (x && x->γ.node == ω) icn_mark_γ_fail_conduit(x); }
    cx->beta = ω; *res = nullv; return ce;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * icn_arm_result(IR_t * rv) {
    if (rv) switch (rv->op) { case IR_GOTO: case IR_SUCCEED: case IR_FAIL: case IR_RETURN: case IR_SUSPEND: case IR_CORET: case IR_COFAIL: return NULL; default: break; }
    return rv;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * icn_dj_α_entry(IR_graph_t * g, IR_t * dj) {
    (void) g;
    return dj;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_alt_impl(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res, int lval) {
    int n = t->n; if (n < 1) { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    IR_graph_t * g = cx->g;
    IR_t * dj = lc_build(g, IR_DISJUNCTION, NULL, NULL);
    γ_to(dj, γ); ω_to(dj, ω);
    IR_t * resv[64];
    for (int j = 0; j < n && j < 64; j++) {
        int before = g->n;
        IR_t * ar = NULL; cx->beta = dj;
        IR_t * ej = lval ? lower_lvalue_var(cx, t->c[j], dj, &ar) : lower(cx, t->c[j], dj, dj, &ar);
        if (lval && ar) lc_γ_to(ar, dj);
        IR_t * ab = cx->beta;
        int ab_in_arm = 0; if (ab && ab != dj) for (int k = before; k < g->n; k++) if (g->all[k] == ab) { ab_in_arm = 1; break; }
        IR_t * rj = ab_in_arm ? ab : dj;
        for (int k = before; k < g->n; k++) {
            IR_t * x = g->all[k];
            if (!x) continue;
            if (x->ω.node == dj) { memcpy(x->ω.sz, "φ", 3); x->ω.sz[3] = 0; }
            if (x->γ.node == dj) { if ((x->op == IR_GOTO && x->ω.node == dj) || icn_γ_is_fail_conduit(x)) { memcpy(x->γ.sz, "φ", 3); } else { memcpy(x->γ.sz, "σ", 3); } x->γ.sz[3] = 0; }
        }
        ir_operand_push(dj, ej);
        ir_operand_push(dj, rj);
        resv[j] = ar;
    }
    for (int j = 0; j < n && j < 64; j++) ir_operand_push(dj, icn_arm_result(resv[j]));
    IR_LIT(dj).ival = (long) (n < 64 ? n : 64);
    cx->beta = dj; *res = dj;
    return icn_dj_α_entry(g, dj);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_alt(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) { return lower_alt_impl(cx, t, γ, ω, res, 0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_alt_lv(icx_t * cx, const tree_t * t, IR_t * ω, IR_t ** var_res) { return lower_alt_impl(cx, t, NULL, ω, var_res, 1); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_if(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    const tree_t * C = (t->n > 0) ? t->c[0] : NULL; const tree_t * TH = (t->n > 1) ? t->c[1] : NULL; const tree_t * EL = (t->n > 2) ? t->c[2] : NULL;
    IR_graph_t * g = cx->g;
    int n = EL ? 2 : 1;
    IR_t * dj = lc_build(g, IR_DISJUNCTION, NULL, NULL);
    γ_to(dj, γ); ω_to(dj, ω);
    IR_t * fs = IR_node_alloc(g, IR_FAIL);
    IR_t * entv[2]; IR_t * resumev[2]; IR_t * resv[2];
    for (int j = 0; j < n; j++) {
        const tree_t * ARM = j ? EL : TH;
        int before = g->n;
        IR_t * ar = NULL; cx->beta = dj;
        IR_t * aent = lower(cx, ARM, dj, ω, &ar);
        IR_t * ab = cx->beta; int arm_end = g->n;
        int ab_in_arm = 0; if (ab && ab != dj) for (int k = before; k < arm_end; k++) if (g->all[k] == ab) { ab_in_arm = 1; break; }
        if (j == 0) {
            IR_t * aent0 = aent; int cbefore = g->n;
            IR_t * cval = NULL; aent = lower(cx, C, aent, dj, &cval); (void) cval;
            for (int k = cbefore; k < g->n; k++) { IR_t * x = g->all[k]; if (!x) continue;
                if (x->γ.node == aent0 && (unsigned char) x->γ.sz[0] == 0xce && (unsigned char) x->γ.sz[1] == 0xb2) memcpy(x->γ.sz, "α!", 4);
                if (x->ω.node == aent0 && (unsigned char) x->ω.sz[0] == 0xce && (unsigned char) x->ω.sz[1] == 0xb2) memcpy(x->ω.sz, "α!", 4); }
        }
        for (int k = before; k < g->n; k++) {
            IR_t * x = g->all[k];
            if (!x) continue;
            if (x->ω.node == dj) { memcpy(x->ω.sz, "φ", 3); x->ω.sz[3] = 0; }
            if (x->γ.node == dj) { if ((x->op == IR_GOTO && x->ω.node == dj) || icn_γ_is_fail_conduit(x)) { memcpy(x->γ.sz, "φ", 3); } else { memcpy(x->γ.sz, "σ", 3); } x->γ.sz[3] = 0; }
        }
        entv[j] = aent; resumev[j] = ab_in_arm ? ab : fs; resv[j] = ar;
    }
    for (int j = 0; j < n; j++) { ir_operand_push(dj, entv[j]); ir_operand_push(dj, resumev[j]); }
    for (int j = 0; j < n; j++) ir_operand_push(dj, icn_arm_result(resv[j]));
    IR_LIT(dj).ival = (long) n;
    cx->beta = dj; *res = dj;
    return icn_dj_α_entry(g, dj);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_const_step(const tree_t * s, int64_t * bits, int * isr) {
    if (!s) return 0;
    if (s->t == TT_ILIT) { *bits = s->v.ival; *isr = 0; return 1; }
    if (s->t == TT_FLIT) { double d = s->v.dval; memcpy(bits, &d, 8); *isr = 1; return 1; }
    if ((s->t == TT_MNS || s->t == TT_PLS) && s->n >= 1 && s->c[0]) {
        if (!icn_const_step(s->c[0], bits, isr)) return 0;
        if (s->t == TT_MNS) { if (*isr) { double d; memcpy(&d, bits, 8); d = -d; memcpy(bits, &d, 8); } else *bits = -(*bits); }
        return 1;
    }
    if (s->t == TT_POW && s->n >= 2 && s->c[0] && s->c[1]) {
        int64_t lb = 0, rb = 0; int li = 0, ri = 0;
        if (!icn_const_step(s->c[0], &lb, &li) || !icn_const_step(s->c[1], &rb, &ri)) return 0;
        if (!li && !ri) {
            if (rb >= 0) { int64_t acc = 1; for (int64_t k = 0; k < rb; k++) acc *= lb; *bits = acc; *isr = 0; return 1; }
            if (lb == 1)  { *bits = 1; *isr = 0; return 1; }
            if (lb == -1) { *bits = (rb & 1) ? -1 : 1; *isr = 0; return 1; }
            if (lb == 0)  return 0;
            *bits = 0; *isr = 0; return 1;
        }
        double la, ra, rv; if (li) memcpy(&la, &lb, 8); else la = (double) lb; if (ri) memcpy(&ra, &rb, 8); else ra = (double) rb;
        rv = pow(la, ra); memcpy(bits, &rv, 8); *isr = 1; return 1;
    }
    if ((s->t == TT_ADD || s->t == TT_SUB || s->t == TT_MUL || s->t == TT_DIV || s->t == TT_MOD) && s->n >= 2 && s->c[0] && s->c[1]) {
        int64_t lb = 0, rb = 0; int li = 0, ri = 0;
        if (!icn_const_step(s->c[0], &lb, &li) || !icn_const_step(s->c[1], &rb, &ri)) return 0;
        if (!li && !ri) {
            int64_t r;
            if (s->t == TT_ADD) r = lb + rb;
            else if (s->t == TT_SUB) r = lb - rb;
            else if (s->t == TT_MUL) r = lb * rb;
            else if (s->t == TT_DIV) { if (rb == 0) return 0; r = lb / rb; }
            else { if (rb == 0) return 0; r = lb % rb; }
            *bits = r; *isr = 0; return 1;
        }
        double la, ra, rv; if (li) memcpy(&la, &lb, 8); else la = (double) lb; if (ri) memcpy(&ra, &rb, 8); else ra = (double) rb;
        if (s->t == TT_ADD) rv = la + ra; else if (s->t == TT_SUB) rv = la - ra; else if (s->t == TT_MUL) rv = la * ra; else if (s->t == TT_DIV) {
            if (ra == 0.0) return 0; rv = la / ra;
        } else { if (ra == 0.0) return 0; rv = fmod(la, ra); }
        memcpy(bits, &rv, 8); *isr = 1; return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_seq(icx_t * cx, const tree_t * t, int argbase, int nargs, IR_t * γ, IR_t * ω, IR_t ** res) {
    int64_t by = 1; const tree_t * byt = (nargs > 1) ? t->c[argbase + 1] : NULL;
    if (byt) { int64_t bb = 1; int isr = 0; if (!icn_const_step(byt, &bb, &isr) || isr) return NULL; by = bb; }
    if (by != 1) return NULL;
    IR_t * to = build(cx, IR_TO, γ, ω); IR_LIT(to).sval = (char *) "ag"; cx->last_gen = to;
    const tree_t * fromt = (nargs > 0) ? t->c[argbase] : NULL; IR_t * lr; IR_t * ea; IR_t * lβ;
    if (fromt) { ea = lower(cx, fromt, NULL, ω, &lr); lβ = cx->beta; } else { lr = build(cx, IR_LIT_INTEGER, NULL, ω); IR_LIT(lr).ival = 1; ea = lr; lβ = ω; }
    IR_t * mr = build(cx, IR_LIT_INTEGER, to, lβ); IR_LIT(mr).ival = (by > 0) ? INT64_MAX : INT64_MIN; γ_to(lr, mr);
    lc_γ_to_α(mr, to);
    ir_operand_push(to, lr); ir_operand_push(to, mr); cx->beta = to; *res = to; return ea;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_key(icx_t * cx, const tree_t * t, int argbase, int nargs, IR_t * γ, IR_t * ω, IR_t ** res) {
    (void) nargs;
    IR_t * kg = build(cx, IR_ITERATE, γ, ω);
    IR_LIT(kg).sval = "key";
    IR_t * orr = NULL; IR_t * ee = lower(cx, t->c[argbase], NULL, ω, &orr);
    ir_operand_push(kg, orr);
    lc_γ_to(orr, kg);
    cx->beta = kg; *res = kg; return ee;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_make_list(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_t * ml = build(cx, IR_MAKE_LIST, γ, ω);
    if (res) *res = ml;
    IR_t * prev = NULL; IR_t * entry = ml; IR_t * aω = ω;
    for (int k = 0; k < t->n; k++) {
        const tree_t * a = t->c[k]; IR_t * ar = NULL;
        IR_t * ae = lower(cx, a, (k == t->n - 1) ? ml : NULL, aω, &ar); aω = cx->beta;
        if (k == 0) entry = ae;
        if (prev) γ_to(prev, ae);
        prev = ar;
        if (ar) ir_operand_push(ml, ar);
    }
    cx->beta = g_postfix_resume ? aω : ω;
    return entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_to(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    int by = (t->t == TT_TO_BY && t->n > 2 && t->c[2]) ? 1 : 0;
    IR_t * to = build(cx, by ? IR_TO_BY : IR_TO, γ, ω); IR_LIT(to).sval = (char *) "ag"; cx->last_gen = to;
    IR_t * lr = NULL; IR_t * ea = lower(cx, t->c[0], NULL, ω, &lr); IR_t * lβ = cx->beta;
    IR_t * mr = NULL; IR_t * em = lower(cx, t->c[1], by ? NULL : to, lβ, &mr); γ_to(lr, em);
    ir_operand_push(to, lr); ir_operand_push(to, mr); IR_t * last_op = mr;
    if (by) {
        IR_t * mβ = cx->beta; IR_t * br = NULL; IR_t * eb = lower(cx, t->c[2], to, mβ, &br); γ_to(mr, eb); (void) eb;
        if (br && ir_is_generator_kind(to->op)) lc_γ_to(br, to);
        ir_operand_push(to, br); last_op = br;
        if ((lr && lr->op == IR_LIT_REAL) || (mr && mr->op == IR_LIT_REAL) || (br && br->op == IR_LIT_REAL)) IR_LIT(to).sval = (char *) "ar";
    } else if (mr && ir_is_generator_kind(to->op)) lc_γ_to(mr, to);
    { IR_t * resume_op = NULL;
      if (by && last_op && ir_is_generator_kind(last_op->op)) resume_op = last_op;
      else if (mr && ir_is_generator_kind(mr->op)) resume_op = mr;
      else if (lr && ir_is_generator_kind(lr->op)) resume_op = lr;
      if (resume_op) lc_ω_to_β(to, resume_op); }
    cx->beta = to; *res = to; return ea;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_every(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    (void) γ;
    const tree_t * E = (t->n > 0) ? t->c[0] : NULL; const tree_t * B = (t->n > 1) ? t->c[1] : NULL;
    IR_t * eval = NULL; IR_t * e_entry = lower(cx, E, NULL, ω, &eval); IR_t * gen_beta = cx->beta;
    if (gen_beta == ω) gen_beta = build(cx, IR_GOTO, ω, ω);
    IR_t * sle = cx->loop_exit; IR_t * sln = cx->loop_next; IR_t * slf = cx->loop_fail; int slns = cx->loop_next_ssp; cx->loop_exit = ω; cx->loop_next = gen_beta; cx->loop_fail = ω; cx->loop_next_ssp = cx->scan_sp;
    IR_t * bval = NULL; (void) bval; IR_t * b_entry;
    if (B) {
        IR_t * mark = build(cx, IR_BOUND, NULL, NULL);
        IR_t * unmk = build(cx, IR_UNMARK, gen_beta, gen_beta);
        ir_operand_push(unmk, mark);
        cx->loop_next = unmk; cx->loop_next_ssp = cx->scan_sp;
        if (cx->loop_sp < ICN_LOOP_STK_MAX) { cx->loop_stk_exit[cx->loop_sp] = cx->loop_exit; cx->loop_stk_next[cx->loop_sp] = cx->loop_next; cx->loop_stk_fail[cx->loop_sp] = cx->loop_fail; } cx->loop_sp++;
        b_entry = lower(cx, B, unmk, unmk, &bval);
        cx->loop_sp--;
        lc_γ_to_α(mark, b_entry); lc_ω_to_α(mark, b_entry);
        b_entry = mark;
    }
    else { b_entry = gen_beta; }
    cx->loop_exit = sle; cx->loop_next = sln; cx->loop_fail = slf; cx->loop_next_ssp = slns;
    if (!(eval && eval->op == IR_SUSPEND)) γ_to(eval, b_entry);
    cx->beta = ω; *res = NULL; return e_entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_graph_t * lower_proc_body(icx_t * cx, const tree_t * body) {
    IR_graph_t * g = IR_alloc(8192); cx->g = g;
    IR_t * PSUCC = IR_node_alloc(g, IR_SUCCEED); IR_t * PFAIL = IR_node_alloc(g, IR_FAIL);
    cx->psucc = PSUCC; cx->pfail = PFAIL;
    IR_t * succ = PFAIL; IR_t * fail = PFAIL;
    for (int i = body->n - 1; i >= 0; i--) {
        const tree_t * s = body->c[i]; if (s && s->t == TT_STMT) { const tree_t * sub = stmt_subj(s); if (!sub) continue; s = sub; } if (!s) continue;
        IR_t * r = NULL; IR_t * entry = lower(cx, s, succ, fail, &r); if (r && r->γ.node == succ) lc_γ_to(r, succ);
        if (entry && ir_is_generator_kind(entry->op)) {
            IR_t * tramp = IR_node_alloc(g, IR_GOTO); lc_γ_to(tramp, entry); lc_ω_to(tramp, entry);
            entry = tramp;
        }
        succ = entry; fail = entry;
    }
    g->entry = succ;
    g->icn_cells_graph = 1;
    return g;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void collect_procs_vec(const tree_t * t, lc_vec * out) {
    if (!t) return;
    if (t->t == TT_STMT) { collect_procs_vec(stmt_subj(t), out); return; }
    if (t->t == TT_PROC_DECL) { lc_vec_push(out, &t); return; }
    for (int i = 0; i < t->n; i++) collect_procs_vec(t->c[i], out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void fill_pnames(const tree_t * prog, lc_vec * pn) {
    lc_vec ps; lc_vec_init(&ps, (int) sizeof(const tree_t *));
    collect_procs_vec(prog, &ps);
    for (int i = 0; i < ps.n; i++) if (LC_AT(&ps, const tree_t *, i)->v.sval) lc_vec_push(pn, &LC_AT(&ps, const tree_t *, i)->v.sval);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void icn_rename_statics_walk(tree_t * n, const char ** names, char ** mangled, int cnt) {
    if (!n) return;
    if (n->t == TT_PROC_DECL || n->t == TT_STATIC_DECL) return;
    if (n->t == TT_VAR && n->v.sval) { for (int k = 0; k < cnt; k++) if (!strcmp(n->v.sval, names[k])) { n->v.sval = mangled[k]; return; } }
    for (int i = 0; i < n->n; i++) icn_rename_statics_walk(n->c[i], names, mangled, cnt);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static lc_vec g_icn_synth_excl;
static void icn_statics_prepass(tree_t * body, const char * pname) {
    if (!body) return;
    const char * names[64]; char * mangled[64]; int cnt = 0; int inits = 0;
    for (int i = 0; i < body->n; i++) {
        tree_t * st = body->c[i]; if (st && st->t == TT_STMT) st = (tree_t *) stmt_subj(st);
        if (st && st->t == TT_STATIC_DECL)
            for (int k = 0; k < st->n; k++) if (st->c[k] && st->c[k]->v.sval && cnt < 64) {
                names[cnt] = st->c[k]->v.sval;
                char * m = malloc(strlen(pname) + strlen(names[cnt]) + 12); sprintf(m, "%s__STATIC__%s", pname, names[cnt]);
                { extern void global_register(const char *); global_register(m); }
                { const char * mc = m; lc_vec_push(&g_icn_synth_excl, &mc); }
                mangled[cnt] = m; cnt++;
            }
    }
    if (cnt) icn_rename_statics_walk(body, names, mangled, cnt);
    for (int i = 0; i < body->n; i++) {
        tree_t * st = body->c[i]; if (st && st->t == TT_STMT) st = (tree_t *) stmt_subj(st);
        if (st && st->t == TT_INITIAL) {
            char * f = malloc(strlen(pname) + 20); sprintf(f, "%s__INITFLAG__%d", pname, inits++);
            { extern void global_register(const char *); global_register(f); }
            { const char * fc = f; lc_vec_push(&g_icn_synth_excl, &fc); }
            tree_t * fv = ast_node_new(TT_VAR); fv->v.sval = f;
            tree_t * nz = ast_node_new(TT_NULL); ast_push(nz, fv);
            tree_t * one = ast_node_new(TT_ILIT); one->v.ival = 1;
            tree_t * asn = ast_node_new(TT_ASSIGN); ast_push(asn, nz); ast_push(asn, one);
            tree_t * child = (st->n > 0) ? st->c[0] : NULL;
            st->t = TT_IF; st->n = 0;
            ast_push(st, asn);
            if (child) ast_push(st, child); else { tree_t * s1 = ast_node_new(TT_ILIT); s1->v.ival = 1; ast_push(st, s1); }
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void icn_collect_own_globals(const tree_t * prog, lc_vec * out) {
    for (int i = 0; prog && i < prog->n; i++) { const tree_t * s = prog->c[i]; if (!s) continue; if (s->t == TT_STMT) s = stmt_subj(s); if (!s) continue;
        if (s->t == TT_GLOBAL) for (int k = 0; k < s->n; k++) if (s->c[k] && s->c[k]->v.sval) lc_vec_push(out, &s->c[k]->v.sval); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void icn_collect_implicit_locals(const tree_t * n, const char ** excl, int nexcl, lc_vec * out) {
    if (!n) return;
    if (n->t == TT_PROC_DECL || n->t == TT_LOCAL || n->t == TT_STATIC_DECL || n->t == TT_GLOBAL) return;
    if (n->t == TT_VAR && n->v.sval) {
        const char * nm = n->v.sval; int found = 0;
        for (int k = 0; k < nexcl && !found; k++) if (excl[k] && !strcmp(excl[k], nm)) found = 1;
        for (int k = 0; k < out->n && !found; k++) if (!strcmp(LC_AT(out, const char *, k), nm)) found = 1;
        if (!found) { extern int icn_builtin_is_known(const char *); extern void * dat_find_type(const char *);
            if (!icn_builtin_is_known(nm) && !dat_find_type(nm)) lc_vec_push(out, &nm); }
    }
    for (int i = 0; i < n->n; i++) icn_collect_implicit_locals(n->c[i], excl, nexcl, out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * lower_icon_proc(const tree_t * prog, const tree_t * pd) {
    lc_vec_init(&g_icn_synth_excl, (int) sizeof(const char *));
    static lc_vec pnv; lc_vec_init(&pnv, (int) sizeof(const char *)); fill_pnames(prog, &pnv);
    icx_t cx; memset(&cx, 0, sizeof cx); cx.pn = (const char **) pnv.data; cx.npn = pnv.n;
    static lc_vec lnv; lc_vec_init(&lnv, (int) sizeof(const char *)); lnv.n = 0;
    if (pd) {
        const tree_t * plist = (pd->n > 1) ? pd->c[1] : NULL;
        for (int i = 0; plist && i < plist->n; i++) if (plist->c[i] && plist->c[i]->v.sval) lc_vec_push(&lnv, &plist->c[i]->v.sval);
        const tree_t * body = (pd->n > 2) ? pd->c[2] : NULL;
        icn_statics_prepass((tree_t *) body, pd->v.sval ? pd->v.sval : "anon");
        for (int i = 0; body && i < body->n; i++) {
            const tree_t * st = body->c[i]; if (st && st->t == TT_STMT) st = stmt_subj(st);
            if (st && st->t == TT_LOCAL)
                for (int k = 0; k < st->n; k++) if (st->c[k] && st->c[k]->v.sval) lc_vec_push(&lnv, &st->c[k]->v.sval);
        }
    }
    cx.ln = (const char **) lnv.data; cx.nln = lnv.n;
    { static lc_vec gnv; lc_vec_init(&gnv, (int) sizeof(const char *)); gnv.n = 0; icn_collect_own_globals(prog, &gnv); cx.gn = (const char **) gnv.data; cx.ngn = gnv.n; cx.pname = (pd && pd->v.sval) ? pd->v.sval : "anon"; }
    if (pd && pd->n > 2 && pd->c[2]) { IR_graph_t * g = lower_proc_body(&cx, pd->c[2]); if (g) { int np = pd->n > 1 && pd->c[1] ? pd->c[1]->n : 0; g->nparams = np; g->pnames = np > 0 ? (const char **)lnv.data : NULL; g->nlocals = lnv.n - np; g->lnames = (lnv.n - np) > 0 ? (const char **)lnv.data + np : NULL; } return g; }
    IR_graph_t * g = IR_alloc(64); cx.g = g; IR_t * s = build(&cx, IR_SUCCEED, 0, 0); g->entry = s;
    g->icn_cells_graph = 1;
    return g;
}
#include "bb_program.h"
#include "emit.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_subtree_has_suspend(const tree_t *n) {
    if (!n) return 0;
    if (n->t == TT_SUSPEND) return 1;
    if (n->t == TT_PROC_DECL || n->t == TT_SUB_DECL) return 0;
    for (int i = 0; i < n->n; i++) if (icn_subtree_has_suspend(n->c[i])) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_body_has_suspend(const tree_t *proc) {
    if (!proc) return 0;
    for (int i = 0; i < proc->n; i++) if (icn_subtree_has_suspend(proc->c[i])) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int lower_icon_body(const tree_t *prog, const tree_t *proc) {
    IR_graph_t * ng = lower_icon_proc(prog, proc);
    if (!ng || !ng->entry) return -1;
    return bb_program_add(&g_stage2.bbp, ng);
}
void lower_icon_resolve_call_kinds(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void icon_register_program(stage2_t * s2, const tree_t * prog) {
    extern int polyglot_module_open(stage2_t * s2, const tree_t * s);
    extern void polyglot_module_extend(stage2_t * s2, int mod_idx, const tree_t * s);
    extern void record_register(const char * spec);
    int mod_idx = -1;
    for (int _ci = 0; _ci < prog->n; _ci++) {
        const tree_t * s = prog->c[_ci];
        if (!s || (s->t != TT_STMT && s->t != TT_END)) continue;
        if (mod_idx < 0) mod_idx = polyglot_module_open(s2, s);
        polyglot_module_extend(s2, mod_idx, s);
        tree_t * proc = stmt_attr_expr(stmt_attr_find(s, ":subj"));
        if (!proc) continue;
        if (proc->t == TT_GLOBAL) {
            for (int _gi = 0; _gi < proc->n; _gi++)
                if (proc->c[_gi] && proc->c[_gi]->v.sval)
                    global_register(proc->c[_gi]->v.sval);
        }
        if (proc->t == TT_RECORD && proc->v.sval && *proc->v.sval) {
            char spec[256]; int pos = 0;
            pos += snprintf(spec+pos, sizeof(spec)-pos, "%s(", proc->v.sval);
            for (int _ri = 0; _ri < proc->n && pos < (int)sizeof(spec)-2; _ri++) {
                if (_ri > 0) spec[pos++] = ',';
                const char *fn2 = (proc->c[_ri] && proc->c[_ri]->v.sval) ? proc->c[_ri]->v.sval : "";
                pos += snprintf(spec+pos, sizeof(spec)-pos, "%s", fn2);
            }
            if (pos < (int)sizeof(spec)-1) spec[pos++] = ')';
            spec[pos] = '\0';
            record_register(spec);
        }
        if (proc->t == TT_FNC || proc->t == TT_PROC_DECL || proc->t == TT_SUB_DECL) {
            const char *name = NULL;
            if (proc->t == TT_SUB_DECL) {
                if (proc->n > 0 && proc->c[0] && proc->c[0]->t == TT_VAR && proc->c[0]->v.sval && *proc->c[0]->v.sval) name = proc->c[0]->v.sval;
            } else {
                name = (proc->v.sval && *proc->v.sval) ? proc->v.sval : ((proc->n > 0 && proc->c[0] && proc->c[0]->t == TT_VAR && proc->c[0]->v.sval && *proc->c[0]->v.sval) ? proc->c[0]->v.sval : NULL);
            }
            if (name) {
                int _dup = 0;
                for (int _qi = 0; _qi < s2->proc_count; _qi++) if (s2->proc_table[_qi].name && !strcmp(s2->proc_table[_qi].name, name)) { _dup = 1; break; }
                if (_dup) continue;
                int _pi = stage2_proc_grow(s2);
                s2->proc_table[_pi].name     = name;
                s2->proc_table[_pi].proc     = proc;
                s2->proc_table[_pi].entry_pc = -1;
                s2->proc_table[_pi].bb_idx   = -1;
                s2->proc_table[_pi].nparams  = (proc->t == TT_PROC_DECL && proc->n >= 2 ? proc->c[1]->n : 0);
                s2->proc_table[_pi].is_variadic = (proc->t == TT_PROC_DECL && proc->n >= 2 && proc->c[1] ? (int)proc->c[1]->v.ival : 0);
                s2->proc_table[_pi].byref_mask = 0;
                if (mod_idx >= 0) s2->module_registry.mods[mod_idx].nprocs++;
                if (strcmp(name, "main") == 0 && s2->module_registry.main_mod < 0) s2->module_registry.main_mod = mod_idx;
            }
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
stage2_t *lower_icon_stage2(const tree_t *prog) {
    int _icn_bb0 = g_stage2.bbp.count;
    icon_register_program(&g_stage2, prog);
    lc_vec _icn_own_globals; lc_vec_init(&_icn_own_globals, (int) sizeof(const char *)); icn_collect_own_globals(prog, &_icn_own_globals);
    for (int pi = 0; pi < g_stage2.proc_count; pi++) if (g_stage2.proc_table[pi].name) lc_vec_push(&_icn_own_globals, &g_stage2.proc_table[pi].name);
    for (int pi = 0; pi < g_stage2.proc_count; pi++) {
        const tree_t *proc = (const tree_t *) g_stage2.proc_table[pi].proc;
        if (!proc || proc->t != TT_PROC_DECL) continue;
        g_stage2.proc_table[pi].is_generator = icn_body_has_suspend(proc);
    }
    for (int pi = 0; pi < g_stage2.proc_count; pi++) {
        const tree_t *proc = (const tree_t *) g_stage2.proc_table[pi].proc;
        if (!proc || proc->t != TT_PROC_DECL) continue;
        if (g_stage2.proc_table[pi].bb_idx >= 0) continue;
        int bb_idx = lower_icon_body(prog, proc);
        if (bb_idx >= 0) {
            g_stage2.proc_table[pi].bb_idx = bb_idx;
            g_stage2.bbp.table[bb_idx]->nparams = g_stage2.proc_table[pi].nparams;
            const tree_t *plist = (proc->n >= 2) ? proc->c[1] : NULL;
            int np = g_stage2.proc_table[pi].nparams;
            Scope *sc = &g_stage2.proc_table[pi].lower_sc;
            sc->n = 0;
            for (int k = 0; k < np && plist && k < plist->n && sc->n < STAGE2_FRAME_SLOT_MAX; k++) {
                const tree_t *pv = plist->c[k];
                if (!pv || !pv->v.sval) continue;
                sc->e[sc->n].name = lp_strdup(pv->v.sval);
                sc->e[sc->n].slot = sc->n;
                sc->n++;
            }
            if (np > 0) {
                const char ** _pn = (const char **)calloc((size_t)np, sizeof(const char *));
                if (_pn) { for (int k = 0; k < np && k < sc->n; k++) _pn[k] = sc->e[k].name; g_stage2.bbp.table[bb_idx]->pnames = _pn; }
            }
            { const tree_t *body = (proc->n > 2) ? proc->c[2] : NULL; int nl = 0;
              for (int i = 0; body && i < body->n; i++) { const tree_t *st = body->c[i]; if (st && st->t == TT_STMT) st = stmt_subj(st);
                  if (st && st->t == TT_LOCAL) for (int k = 0; k < st->n; k++) if (st->c[k] && st->c[k]->v.sval) nl++; }
              const char ** _ln = NULL; int w = 0;
              if (nl > 0) { _ln = (const char **)calloc((size_t)nl, sizeof(const char *));
                  for (int i = 0; body && i < body->n && _ln; i++) { const tree_t *st = body->c[i]; if (st && st->t == TT_STMT) st = stmt_subj(st);
                      if (st && st->t == TT_LOCAL) for (int k = 0; k < st->n; k++) if (st->c[k] && st->c[k]->v.sval) _ln[w++] = lp_strdup(st->c[k]->v.sval); } }
              lc_vec _excl; lc_vec_init(&_excl, (int) sizeof(const char *));
              for (int k = 0; k < _icn_own_globals.n; k++) lc_vec_push(&_excl, &LC_AT(&_icn_own_globals, const char *, k));
              for (int k = 0; k < sc->n; k++) lc_vec_push(&_excl, &sc->e[k].name);
              for (int k = 0; k < w; k++) lc_vec_push(&_excl, &_ln[k]);
              for (int k = 0; k < g_icn_synth_excl.n; k++) lc_vec_push(&_excl, &LC_AT(&g_icn_synth_excl, const char *, k));
              lc_vec _impl; lc_vec_init(&_impl, (int) sizeof(const char *));
              icn_collect_implicit_locals(body, (const char **)_excl.data, _excl.n, &_impl);
              if (w > 0 || _impl.n > 0) {
                  int total = w + _impl.n;
                  const char ** _all = (const char **)calloc((size_t)total, sizeof(const char *)); int a = 0;
                  if (_all) { for (int k = 0; k < w; k++) _all[a++] = _ln[k]; for (int k = 0; k < _impl.n; k++) _all[a++] = LC_AT(&_impl, const char *, k);
                      g_stage2.bbp.table[bb_idx]->lnames = _all; g_stage2.bbp.table[bb_idx]->nlocals = total; } } }
        }
    }
    lower_icon_resolve_call_kinds();
    return &g_stage2;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_callable_proc_index(const char * fn) {
    for (int pi = 0; pi < g_stage2.proc_count; pi++) {
        const char * pname = g_stage2.proc_table[pi].name;
        if (!pname || strcmp(pname, fn) != 0) continue;
        if (strcmp(pname, "main") == 0) return -1;
        int idx = g_stage2.proc_table[pi].bb_idx;
        if (idx < 0 || idx >= g_stage2.bbp.count || !g_stage2.bbp.table[idx] || !g_stage2.bbp.table[idx]->entry) return -1;
        return pi;
    }
    return -1;
}
int g_icon_write_reassignable = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void icn_scan_write_reassignable(void) {
    g_icon_write_reassignable = 0;
    for (int gi = 0; gi < g_stage2.bbp.count && !g_icon_write_reassignable; gi++) {
        IR_graph_t * g = g_stage2.bbp.table[gi];
        if (!g) continue;
        for (int i = 0; i < g->n; i++) {
            IR_t * nd = g->all[i];
            if (!nd) continue;
            const char * tgt = 0;
            if (nd->op == IR_ASSIGN) tgt = IR_LIT(nd).sval;
            else if (nd->op == IR_REV_ASSIGN && nd->n_operands > 1 && nd->operands[1]) tgt = IR_LIT(nd->operands[1]).sval;
            if (tgt && (!strcmp(tgt, "write") || !strcmp(tgt, "writes"))) { g_icon_write_reassignable = 1; break; }
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void lower_icon_resolve_call_kinds(void) {
    extern int rt_builtin_is_generator(const char *);
    extern int rt_builtin_is_known(const char *);
    extern int is_global(const char *);
    icn_scan_write_reassignable();
    for (int gi = 0; gi < g_stage2.bbp.count; gi++) {
        IR_graph_t * g = g_stage2.bbp.table[gi];
        if (!g || !g->icn_cells_graph) continue;
        for (int i = 0; i < g->n; i++) {
            IR_t * nd = g->all[i];
            if (!nd || nd->op != IR_CALL) continue;
            const char * fn = IR_LIT(nd).sval;
            if (!fn || !fn[0]) continue;
            int pi = icn_callable_proc_index(fn);
            if (pi >= 0 && g_stage2.proc_table[pi].is_generator) nd->op = IR_PROC_GEN;
            else if (pi >= 0) nd->op = IR_CALL_PROC_STAGED;
            else if (icn_builtin_is_generator(fn)) nd->op = IR_CALL_ICON;
            else if (icn_builtin_is_known(fn)) nd->op = IR_CALL_ICON;
        }
        for (int i = 0; i < g->n; i++) {
            IR_t * nd = g->all[i];
            if (!nd || nd->op != IR_VAR) continue;
            const char * vn = IR_LIT(nd).sval;
            if (!vn || !vn[0] || vn[0] == '&') continue;
            if (is_global(vn)) continue;
            if (graph_has_local(g, vn)) continue;
            int skip = 0;
            for (int k = 0; !skip && g->pnames && k < g->nparams; k++) if (g->pnames[k] && !strcmp(g->pnames[k], vn)) skip = 1;
            for (int k = 0; !skip && k < g->n; k++) {
                IR_t * a = g->all[k]; if (!a) continue;
                if (a->op == IR_ASSIGN && IR_LIT(a).sval && !strcmp(IR_LIT(a).sval, vn)) skip = 1;
                if (a->op == IR_REV_ASSIGN && a->n_operands > 1 && a->operands[1] && IR_LIT(a->operands[1]).sval && !strcmp(IR_LIT(a->operands[1]).sval, vn)) skip = 1;
            }
            if (skip) continue;
            int isproc = 0;
            for (int pi = 0; pi < g_stage2.proc_count; pi++) if (g_stage2.proc_table[pi].name && !strcmp(g_stage2.proc_table[pi].name, vn)) { isproc = 1; break; }
            if (isproc || rt_builtin_is_known(vn) || rt_builtin_is_generator(vn) || !strcmp(vn, "push") || !strcmp(vn, "put")) nd->op = IR_PROC_VALUE;
        }
    }
}
