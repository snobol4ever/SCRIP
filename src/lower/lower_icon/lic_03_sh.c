#include "lower/lower_icon/lic_head.h"
#include "lower/lower_icon/lic_01_sh.h"
#include "lower/lower_icon/lic_02_sh.h"
#include "lower/lower_icon/lic_03_sh.h"
#include "lower/lower_icon/lic_04_sh.h"
extern int icn_builtin_is_known(const char *);
extern int icn_builtin_arity(const char *);
extern int icn_builtin_is_generator(const char *);
static IR_t * icn_trace_call_wrap(icx_t * cx, const char * name, IR_t * body_entry, IR_t * ω);
static int icn_subtree_has_suspend(const tree_t *n);
extern const char * icn_builtin_nth(int);
static int icn_progname_used(const tree_t * n);
void lower_icon_resolve_call_kinds(int unicon);
extern void icn_expand_classes(tree_t * prog);
extern void icn_expand_deferred(tree_t * prog);
const char * icn_static_mangled(const icx_t * cx, const char * nm) {
    if (!cx->pname) return NULL;
    size_t pl = strlen(cx->pname), nl = strlen(nm);
    for (int i = 0; i < g_lower.icn.synth_excl.n; i++) {
        const char * m = LC_AT(&g_lower.icn.synth_excl, const char *, i);
        if (m && !strncmp(m, cx->pname, pl) && !strncmp(m + pl, "__STATIC__", 10) && !strcmp(m + pl + 10, nm) && strlen(m) == pl + 10 + nl) return m;
    }
    return NULL;
}
void icn_mark_γ_fail_conduit(IR_t * nd) { if (nd) { memcpy(nd->γ.sz, "φ", 3); nd->γ.sz[3] = 0; } }
int icn_γ_is_fail_conduit(const IR_t * nd) { return nd && (unsigned char) nd->γ.sz[0] == 0xcf && (unsigned char) nd->γ.sz[1] == 0x86; }
static void icn_settle_fail_conduits(IR_graph_t * g) {
    for (int k = 0; k < g->n; k++) {
        IR_t * x = g->all[k];
        if (!x || !icn_γ_is_fail_conduit(x) || !x->γ.node || x->γ.node != x->ω.node) continue;
        IR_t * t = x->γ.node;
        for (int h = 0; t && t->op == IR_GOTO && h < 128; h++) t = t->γ.node;
        if (t && t->op != IR_DISJUNCTION) memcpy(x->γ.sz, x->ω.sz, sizeof x->γ.sz);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * lic_build(icx_t * cx, IR_e op, IR_t * γ, IR_t * ω) {
    IR_t * nd = lc_build(cx->g, op, γ, ω);
    nd->strict = 1;
    if (γ && icn_gen_wiring(γ)) lc_γ_to_β(nd, γ);
    if (ω && icn_gen_wiring(ω)) lc_ω_to_β(nd, ω);
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const tree_t * lic_stmt_subj(const tree_t * s) { return lc_stmt_subj(s); }
static void icn_reassigned_walk(const tree_t * n) {
    if (!n) return;
    if (n->t == TT_STMT) { icn_reassigned_walk(lic_stmt_subj(n)); return; }
    if (n->t == TT_ASSIGN || n->t == TT_REVASSIGN || n->t == TT_AUGOP || n->t == TT_SWAP || n->t == TT_REVSWAP) for (int i = 0; i < n->n; i++) {
        const tree_t * c = n->c[i];
        if (c && c->t == TT_VAR && c->v.sval && !icn_name_assigned(c->v.sval)) lc_vec_push(&g_lower.icn.reassigned, &c->v.sval);
        if (n->t == TT_ASSIGN || n->t == TT_REVASSIGN) break;
    }
    for (int i = 0; i < n->n; i++) icn_reassigned_walk(n->c[i]);
}
static int icn_is_rtname_variable(const tree_t * c) {
    return c && c->t == TT_FNC && c->n == 2 && c->c[0] && c->c[0]->t == TT_VAR && c->c[0]->v.sval && !strcmp(c->c[0]->v.sval, "variable") && c->c[1] && c->c[1]->t != TT_QLIT;
}
static int icn_tree_has_rtname_lv(const tree_t * n) {
    if (!n) return 0;
    if (n->t == TT_ASSIGN || n->t == TT_REVASSIGN || n->t == TT_AUGOP || n->t == TT_SWAP || n->t == TT_REVSWAP) for (int i = 0; i < n->n; i++) {
        if (icn_is_rtname_variable(n->c[i])) return 1;
        if (n->t == TT_ASSIGN || n->t == TT_REVASSIGN) break;
    }
    for (int i = 0; i < n->n; i++) if (icn_tree_has_rtname_lv(n->c[i])) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_global_declared(const tree_t * prog, const char * nm) {
    for (int i = 0; i < prog->n; i++) {
        const tree_t * d = prog->c[i];
        if (d && d->t == TT_STMT) d = lic_stmt_subj(d);
        if (d && d->t == TT_GLOBAL) for (int k = 0; k < d->n; k++) if (d->c[k] && d->c[k]->v.sval && !strcmp(d->c[k]->v.sval, nm)) return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void icn_collect_reassigned_procs(const tree_t * prog) {
    if (g_lower.icn.reassigned_prog == prog) return;
    lc_vec_init(&g_lower.icn.reassigned, (int) sizeof(const char *));
    g_lower.icn.reassigned.n = 0;
    icn_reassigned_walk(prog);
    g_lower.icn.reassigned_prog = prog;
    if (icn_tree_has_rtname_lv(prog)) for (int i = 0; i < prog->n; i++) {
        const tree_t * d = prog->c[i];
        if (d && d->t == TT_STMT) d = lic_stmt_subj(d);
        if (d && d->t == TT_PROC_DECL && d->v.sval && !icn_name_assigned(d->v.sval)) lc_vec_push(&g_lower.icn.reassigned, &d->v.sval);
    }
    for (int i = 0; i < prog->n; i++) {
        const tree_t * d = prog->c[i];
        if (d && d->t == TT_STMT) d = lic_stmt_subj(d);
        if (d && d->t == TT_PROC_DECL && d->v.sval && !icn_name_assigned(d->v.sval) && icn_global_declared(prog, d->v.sval)) lc_vec_push(&g_lower.icn.reassigned, &d->v.sval);
    }
}
int icn_builtin_reassigned(const char * nm) {
    if (!nm || !g_lower.icn.breassigned_prog) return 0;
    for (int k = 0; k < g_lower.icn.breassigned.n; k++) if (!strcmp(LC_AT(&g_lower.icn.breassigned, const char *, k), nm)) return 1;
    return 0;
}
int icn_never_pattern(const tree_t * a) {
    if (!a) return 0;
    if (a->t == TT_QLIT || a->t == TT_CSET || a->t == TT_ILIT || a->t == TT_TO || a->t == TT_TO_BY) return 1;
    if (a->t != TT_ALTERNATE || a->n == 0) return 0;
    for (int k = 0; k < a->n; k++) if (!icn_never_pattern(a->c[k])) return 0;
    return 1;
}
static int icn_decl_list_has(const tree_t * d, const char * nm) { for (int k = 0; d && k < d->n; k++) if (d->c[k] && d->c[k]->v.sval && !strcmp(d->c[k]->v.sval, nm)) return 1; return 0; }
static int icn_proc_declares(const tree_t * pd, const char * nm) {
    if (pd->n > 1 && icn_decl_list_has(pd->c[1], nm)) return 1;
    const tree_t * body = (pd->n > 2) ? pd->c[2] : NULL;
    for (int i = 0; body && i < body->n; i++) {
        const tree_t * st = body->c[i];
        if (st && st->t == TT_STMT) st = lic_stmt_subj(st);
        if (st && (st->t == TT_LOCAL || st->t == TT_STATIC_DECL) && icn_decl_list_has(st, nm)) return 1;
    }
    return 0;
}
static void icn_breassigned_note(const tree_t * prog, const tree_t * pd, const char * nm) {
    extern int icn_builtin_is_known(const char *);
    extern void * dat_find_type(const char *);
    if (nm && (icn_builtin_is_known(nm) || icn_is_icon_function(nm)) && !icn_is_proc(nm) && !dat_find_type(nm) && !icn_proc_declares(pd, nm) && !icn_builtin_reassigned(nm)) {
        int glob = 0;
        for (int g = 0; g < prog->n && !glob; g++) { const tree_t * d = prog->c[g]; if (d && d->t == TT_STMT) d = lic_stmt_subj(d); if (d && d->t == TT_GLOBAL && icn_decl_list_has(d, nm)) glob = 1; }
        if (!glob) lc_vec_push(&g_lower.icn.breassigned, &nm);
    }
}
static void icn_breassigned_walk(const tree_t * prog, const tree_t * pd, const tree_t * n, int every_ref) {
    if (!n) return;
    if (n->t == TT_STMT) { icn_breassigned_walk(prog, pd, lic_stmt_subj(n), every_ref); return; }
    if (every_ref && n->t == TT_VAR && n->v.sval && n->v.sval[0] != '&') icn_breassigned_note(prog, pd, n->v.sval);
    if (n->t == TT_ASSIGN || n->t == TT_REVASSIGN || n->t == TT_AUGOP || n->t == TT_SWAP || n->t == TT_REVSWAP) for (int i = 0; i < n->n; i++) {
        const tree_t * c = n->c[i];
        icn_breassigned_note(prog, pd, (c && c->t == TT_VAR) ? c->v.sval : NULL);
        if (n->t == TT_ASSIGN || n->t == TT_REVASSIGN) break;
    }
    for (int i = 0; i < n->n; i++) icn_breassigned_walk(prog, pd, n->c[i], every_ref);
}
static void icn_collect_reassigned_builtins(const tree_t * prog) {
    if (g_lower.icn.breassigned_prog == prog) return;
    lc_vec_init(&g_lower.icn.breassigned, (int) sizeof(const char *));
    g_lower.icn.breassigned.n = 0;
    g_lower.icn.breassigned_prog = prog;
    int every_ref = icn_tree_has_rtname_lv(prog);
    for (int i = 0; prog && i < prog->n; i++) {
        const tree_t * d = prog->c[i];
        if (d && d->t == TT_STMT) d = lic_stmt_subj(d);
        if (d && d->t == TT_PROC_DECL && d->n > 2) icn_breassigned_walk(prog, d, d->c[2], every_ref);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * icn_cset_canon(const char * s, int len, int * out_len) {
    if (!s) { if (out_len) *out_len = 0; return s; }
    unsigned char seen[256];
    memset(seen, 0, sizeof seen);
    for (int i = 0; i < len; i++) seen[(unsigned char) s[i]] = 1;
    char buf[257];
    int n = 0;
    for (int c = 0; c < 256; c++) if (seen[c]) buf[n++] = (char) c;
    buf[n] = 0;
    if (out_len) *out_len = n;
    { char * b = (char *) ct_alloc((size_t) CSET_BLK_HDR + (size_t) n + 1); return b ? cset_block_init(b, buf, n) : b; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void icn_attach_lit_len(icx_t * cx, IR_t * nd, int len) { IR_t * ln = lic_build(cx, IR_LIT_INTEGER, NULL, NULL); IR_LIT(ln).ival = (int64_t) len; ir_operand_push(nd, ln); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int icn_arg_is_scan_fn(const tree_t * a) {
    if (!a) return 0;
    if (a->t == TT_STMT) a = lic_stmt_subj(a);
    if (!a || a->t != TT_FNC) return 0;
    const char * nm = (a->n > 0 && a->c[0] && a->c[0]->t == TT_VAR) ? a->c[0]->v.sval : NULL;
    return nm &&
        (!strcmp(nm, "tab") || !strcmp(nm, "move") || !strcmp(nm, "pos") || !strcmp(nm, "any") || !strcmp(nm, "match") || !strcmp(nm, "many") || !strcmp(nm, "upto") || !strcmp(nm, "find") ||
        !strcmp(nm, "bal"));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void icn_note_record_name(const char * nm) { if (nm && g_lower.icn.icn_record_name_count < 512) g_lower.icn.icn_record_names[g_lower.icn.icn_record_name_count++] = nm; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * lc_key(icx_t * cx, const tree_t * t, const char * kw, IR_t * γ, IR_t * ω, IR_t ** res) {
    const char * id = (kw && kw[0] == '&') ? kw + 1 : kw;
    if (id && !strcmp(id, "line")) {
        IR_t * nd = lic_build(cx, IR_LIT_INTEGER, γ, ω);
        IR_LIT(nd).ival = cx->key_after_line > 0 ? cx->key_after_line : (t && t->line > 0) ? t->line : 0;
        *res = nd;
        return nd;
    }
    if (id && !strcmp(id, "file")) { IR_t * nd = lic_build(cx, IR_LIT_STRING, γ, ω); IR_LIT(nd).sval = (char *) cx->file; *res = nd; return nd; }
    if (id) {
        const char * cs = !strcmp(id, "ucase") ? "ABCDEFGHIJKLMNOPQRSTUVWXYZ" : !strcmp(id, "lcase") ? "abcdefghijklmnopqrstuvwxyz" : !strcmp(id, "digits") ? "0123456789" : NULL;
        if (!cs && !strcmp(id, "letters")) cs = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
        if (cs) {
            int cn;
            const char * canon = icn_cset_canon(cs, (int) strlen(cs), &cn);
            IR_t * nd = lic_build(cx, IR_LIT_CHARSET, γ, ω);
            IR_LIT(nd).sval = canon;
            icn_attach_lit_len(cx, nd, cn);
            *res = nd;
            return nd;
        }
    }
    int is_gen_kw = id && (!strcmp(id, "features") || !strcmp(id, "regions") || !strcmp(id, "storage") || !strcmp(id, "collections") || !strcmp(id, "allocated"));
    IR_t * nd = lic_build(cx, is_gen_kw ? IR_KW_ICON_GEN : IR_KW_ICON, γ, ω);
    IR_LIT(nd).sval = (char *) (is_gen_kw && !strcmp(id, "features") && cx->prog && cx->prog->v.ival ? "&ufeatures" : kw);
    if (is_gen_kw) { cx->beta = nd; *res = nd; return nd; }
    *res = nd;
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * lower_not(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_t * nullv = lic_build(cx, IR_VAR, γ, ω);
    IR_LIT(nullv).sval = (char *) "&null";
    IR_graph_t * g = cx->g;
    int before = g->n;
    IR_t * cr = NULL;
    IR_t * ce = lic_lower(cx, (t->n > 0) ? t->c[0] : NULL, ω, nullv, &cr);
    for (int k = before; k < g->n; k++) { IR_t * x = g->all[k]; if (x && x->γ.node == ω) icn_mark_γ_fail_conduit(x); }
    cx->beta = ω;
    *res = nullv;
    return ce;
}
int icn_is_icon_function(const char * nm) { for (int k = 0; nm && icn_builtin_nth(k); k++) if (!strcmp(icn_builtin_nth(k), nm)) return 1; return 0; }
IR_t * icn_line_mark(icx_t * cx, int line, IR_t * next) {
    IR_t * mark = lic_build(cx, IR_LINE_MARK, NULL, NULL);
    lc_γ_to(mark, next);
    lc_ω_to(mark, next);
    mark->pat_static = line;
    if (cx->links) IR_LIT(mark).sval = (char *) cx->file;
    return mark;
}
IR_t * icn_line_hook(icx_t * cx, int line, IR_t * next) {
    IR_t * mark = lic_build(cx, IR_LINE_MARK, NULL, NULL);
    lc_γ_to(mark, next);
    lc_ω_to(mark, next);
    mark->pat_static = line;
    IR_LIT(mark).sval = (char *) cx->file;
    return mark;
}
static int icn_trace_wanted(icx_t * cx) { return cx->want_lines || g_runtime.abi.trace_budget != 0; }
IR_t * icn_trace_stmt_wrap(icx_t * cx, long line, IR_t * stmt_entry, IR_t * ω) {
    if (line <= 0 || g_runtime.abi.trace_budget == 0) return stmt_entry;
    IR_t * call = lic_build(cx, IR_CALL, stmt_entry, ω);
    IR_LIT(call).sval = "__trace_stmt";
    IR_t * lit = lic_build(cx, IR_LIT_INTEGER, call, ω);
    IR_LIT(lit).ival = line;
    ir_operand_push(call, lit);
    return lit;
}
static IR_t * icn_trace_call_wrap(icx_t * cx, const char * name, IR_t * body_entry, IR_t * ω) {
    if (!name || !*name || !icn_trace_wanted(cx)) return body_entry;
    IR_t * call = lic_build(cx, IR_CALL, body_entry, ω);
    IR_LIT(call).sval = "__trace_call";
    IR_t * nm = lic_build(cx, IR_LIT_STRING, call, ω);
    IR_LIT(nm).sval = (char *) name;
    ir_operand_push(call, nm);
    return nm;
}
IR_t * icn_trace_named_prep(icx_t * cx, const char * hook, const char * name, IR_t * γ, IR_t * ω, IR_t ** call_out) {
    if (!name || !*name || !icn_trace_wanted(cx)) { *call_out = NULL; return NULL; }
    IR_t * call = lic_build(cx, IR_CALL, γ, ω);
    IR_LIT(call).sval = (char *) hook;
    IR_t * nm = lic_build(cx, IR_LIT_STRING, call, ω);
    IR_LIT(nm).sval = (char *) name;
    ir_operand_push(call, nm);
    *call_out = call;
    return nm;
}
static IR_graph_t * lower_proc_body(icx_t * cx, const tree_t * body) {
    IR_graph_t * g = IR_alloc(8192);
    cx->g = g;
    IR_t * PSUCC = IR_node_alloc(g, IR_SUCCEED);
    IR_t * PFAIL = IR_node_alloc(g, IR_FAIL);
    cx->psucc = PSUCC;
    cx->pfail = PFAIL;
    IR_t * succ = PFAIL;
    IR_t * fail = PFAIL;
    if (cx->want_lines && cx->end_line > 0) { succ = icn_line_hook(cx, cx->end_line, PFAIL); fail = succ; }
    for (int i = body->n - 1; i >= 0; i--) {
        const tree_t * s = body->c[i];
        int sline = s ? s->line : 0;
        if (s && s->t == TT_STMT) { const tree_t * sub = lic_stmt_subj(s); if (!sub) continue; s = sub; }
        if (!s) continue;
        if (sline <= 0) sline = s->line;
        IR_t * r = NULL;
        IR_t * entry = lic_lower(cx, s, succ, fail, &r);
        if (r && r->γ.node == succ) lc_γ_to(r, succ);
        if (entry && ir_is_generator_kind(entry->op)) { IR_t * tramp = IR_node_alloc(g, IR_GOTO); lc_γ_to(tramp, entry); lc_ω_to(tramp, entry); entry = tramp; }
        if (entry && sline > 0) { entry = (cx->want_lines || i == 0) ? icn_line_hook(cx, sline, entry) : icn_line_mark(cx, sline, entry); bb_src_note(entry, "stmt", sline); }
        if (entry && sline > 0 && s->t != TT_LOCAL && s->t != TT_STATIC_DECL && s->t != TT_INITIAL && !(s->t == TT_IF && s->v.ival == ICN_IF_FROM_INITIAL)) entry =
            icn_trace_stmt_wrap(cx, sline, entry, PFAIL);
        succ = entry;
        fail = entry;
    }
    if (cx->pname && !strcmp(cx->pname, "main") && cx->prog && cx->prog->v.ival) {
        IR_t * dr = NULL;
        tree_t * dv = pm_var("ICN$UDIGITS");
        dv->slen = (int) strlen(dv->v.sval);
        IR_t * dn = lic_lower(cx, pm_node(TT_FNC, dv, pm_int(16), NULL), succ, succ, &dr);
        if (dr && dr->γ.node == succ) lc_γ_to(dr, succ);
        if (dn) succ = dn;
    }
    g->entry = icn_trace_call_wrap(cx, cx->pname, succ, PFAIL);
    if (cx->pname && !strcmp(cx->pname, "main")) { IR_t * qt = lic_build(cx, IR_CALL, g->entry, g->entry); IR_LIT(qt).sval = (char *) "$quit_trap_300"; g->entry = qt; }
    icn_retag_scan_body(cx, 0);
    icn_settle_fail_conduits(g);
    g->trace_taps = cx->want_lines;
    g->cell_frame = 1;
    g->spine_cells = 1;
    g->scan_regs = 1;
    g->home_frame = 1;
    return g;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void icn_vec_reuse(lc_vec * v, int esz) { if (v->data && v->esz == esz) v->n = 0; else lc_vec_init(v, esz); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void collect_procs_vec(const tree_t * t, lc_vec * out) {
    if (!t) return;
    if (t->t == TT_STMT) { collect_procs_vec(lic_stmt_subj(t), out); return; }
    if (t->t == TT_PROC_DECL) { lc_vec_push(out, &t); return; }
    for (int i = 0; i < t->n; i++) collect_procs_vec(t->c[i], out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void fill_pnames(const tree_t * prog, lc_vec * pn) {
    icn_vec_reuse(&g_lower.icn.fill_pnames_ps, (int) sizeof(const tree_t *));
    collect_procs_vec(prog, &g_lower.icn.fill_pnames_ps);
    for (int i = 0; i < g_lower.icn.fill_pnames_ps.n; i++) if (LC_AT(&g_lower.icn.fill_pnames_ps, const tree_t *, i)->v.sval)
        lc_vec_push(pn, &LC_AT(&g_lower.icn.fill_pnames_ps, const tree_t *, i)->v.sval);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void icn_rename_statics_walk(tree_t * n, const char ** names, char ** mangled, int cnt) {
    if (!n) return;
    if (n->t == TT_PROC_DECL || n->t == TT_STATIC_DECL) return;
    if (n->t == TT_FIELD) { if (n->n > 0) icn_rename_statics_walk(n->c[0], names, mangled, cnt); return; }
    if (n->t == TT_VAR && n->v.sval) { for (int k = 0; k < cnt; k++) if (!strcmp(n->v.sval, names[k])) { n->v.sval = mangled[k]; return; } }
    for (int i = 0; i < n->n; i++) icn_rename_statics_walk(n->c[i], names, mangled, cnt);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void icn_statics_prepass(tree_t * body, const char * pname) {
    if (!body) return;
    int nst = 1;
    for (int i = 0; i < body->n; i++) { tree_t * st = body->c[i]; if (st && st->t == TT_STMT) st = (tree_t *) lic_stmt_subj(st); if (st && st->t == TT_STATIC_DECL) nst += st->n; }
    const char * names[nst];
    char * mangled[nst];
    int cnt = 0;
    int inits = 0;
    for (int i = 0; i < body->n; i++) {
        tree_t * st = body->c[i];
        if (st && st->t == TT_STMT) st = (tree_t *) lic_stmt_subj(st);
        if (st && st->t == TT_STATIC_DECL) for (int k = 0; k < st->n; k++) if (st->c[k] && st->c[k]->v.sval) {
            names[cnt] = st->c[k]->v.sval;
            char * m = ct_alloc(strlen(pname) + strlen(names[cnt]) + 12);
            sprintf(m, "%s__STATIC__%s", pname, names[cnt]);
            { extern void global_register(const char *); global_register(m); }
            { const char * mc = m; lc_vec_push(&g_lower.icn.synth_excl, &mc); }
            mangled[cnt] = m;
            cnt++;
        }
    }
    if (cnt) icn_rename_statics_walk(body, names, mangled, cnt);
    for (int i = 0; i < body->n; i++) {
        tree_t * st = body->c[i];
        if (st && st->t == TT_STMT) st = (tree_t *) lic_stmt_subj(st);
        if (st && st->t == TT_INITIAL) {
            char * f = ct_alloc(strlen(pname) + 20);
            sprintf(f, "%s__INITFLAG__%d", pname, inits++);
            { extern void global_register(const char *); global_register(f); }
            { const char * fc = f; lc_vec_push(&g_lower.icn.synth_excl, &fc); }
            tree_t * fv = ast_node_new(TT_VAR);
            fv->v.sval = f;
            tree_t * nz = ast_node_new(TT_NULL);
            ast_push(nz, fv);
            tree_t * fv2 = ast_node_new(TT_VAR);
            fv2->v.sval = f;
            tree_t * one = ast_node_new(TT_ILIT);
            one->v.ival = 1;
            tree_t * asn = ast_node_new(TT_ASSIGN);
            ast_push(asn, fv2);
            ast_push(asn, one);
            tree_t * child = (st->n > 0) ? st->c[0] : NULL;
            if (!child) { child = ast_node_new(TT_ILIT); child->v.ival = 1; child->line = st->line; }
            if (child->t != TT_SEQ_EXPR) { tree_t * sq = ast_node_new(TT_SEQ_EXPR); sq->line = (child->line > 0) ? child->line : st->line; ast_push(sq, child); child = sq; }
            child->v.ival = ICN_IF_FROM_INITIAL;
            tree_t * body = ast_node_new(TT_CONJ);
            ast_push(body, asn);
            ast_push(body, child);
            st->t = TT_IF;
            st->n = 0;
            st->v.ival = ICN_IF_FROM_INITIAL;
            ast_push(st, nz);
            ast_push(st, body);
        }
    }
}
static int icn_progname_used_once(const tree_t * prog) {
    if (prog != g_lower.icn.icn_progname_as_global_done_prog) {
        g_lower.icn.icn_progname_as_global_done_prog = prog;
        g_lower.icn.icn_progname_as_global_used = icn_progname_used(prog);
        if (g_lower.icn.icn_progname_as_global_used) global_register("__icn_progname");
    }
    return g_lower.icn.icn_progname_as_global_used;
}
static void icn_collect_own_globals(const tree_t * prog, lc_vec * out) {
    for (int i = 0; prog && i < prog->n; i++) {
        const tree_t * s = prog->c[i];
        if (!s) continue;
        if (s->t == TT_STMT) s = lic_stmt_subj(s);
        if (!s) continue;
        if (s->t == TT_GLOBAL) for (int k = 0; k < s->n; k++) if (s->c[k] && s->c[k]->v.sval) lc_vec_push(out, &s->c[k]->v.sval);
    }
    if (icn_progname_used_once(prog)) { const char * pg = "__icn_progname"; lc_vec_push(out, &pg); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_progname_used(const tree_t * n) {
    if (!n) return 0;
    if ((n->t == TT_VAR || n->t == TT_KEYWORD) && n->v.sval && !strcmp(n->v.sval, "&progname")) return 1;
    for (int i = 0; i < n->n; i++) if (icn_progname_used(n->c[i])) return 1;
    return 0;
}
static void icn_rewrite_progname(tree_t * n) {
    if (!n) return;
    if ((n->t == TT_VAR || n->t == TT_KEYWORD) && n->v.sval && !strcmp(n->v.sval, "&progname")) { n->t = TT_VAR; n->v.sval = (char *) "__icn_progname"; }
    for (int i = 0; i < n->n; i++) icn_rewrite_progname(n->c[i]);
}
static void icn_progname_as_global(const tree_t * prog, const tree_t * pd) {
    if (!icn_progname_used_once(prog) || !pd || pd->n < 3 || !pd->c[2]) return;
    tree_t * body = (tree_t *) pd->c[2];
    icn_rewrite_progname(body);
    if (!pd->v.sval || strcmp(pd->v.sval, "main")) return;
    for (int i = 0; i < body->n; i++) {
        const tree_t * st = body->c[i];
        if (st && st->t == TT_ASSIGN && st->n == 2 && st->c[0] && st->c[0]->v.sval && !strcmp(st->c[0]->v.sval, "__icn_progname") && st->c[1] && st->c[1]->t == TT_KEYWORD && st->line == -1) return;
    }
    tree_t * lhs = ast_node_new(TT_VAR);
    lhs->v.sval = (char *) "__icn_progname";
    tree_t * rhs = ast_node_new(TT_KEYWORD);
    rhs->v.sval = (char *) "&progname";
    tree_t * as = ast_node_new(TT_ASSIGN);
    ast_push(as, lhs);
    ast_push(as, rhs);
    as->line = -1;
    ast_push(body, as);
    for (int i = body->n - 1; i > 0; i--) body->c[i] = body->c[i - 1];
    body->c[0] = as;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void icn_init_reassigned_procs(icx_t * cx, IR_graph_t * g) {
    for (int k = 0; k < g_lower.icn.reassigned.n + g_lower.icn.breassigned.n; k++) {
        const char * nm = (k < g_lower.icn.reassigned.n) ? LC_AT(&g_lower.icn.reassigned, const char *, k) : LC_AT(&g_lower.icn.breassigned, const char *, k - g_lower.icn.reassigned.n);
        if (k < g_lower.icn.reassigned.n && !icn_is_proc(nm)) continue;
        IR_t * asn = lic_build(cx, IR_ASSIGN, g->entry, g->entry);
        IR_LIT(asn).sval = (char *) nm;
        IR_t * pv = lic_build(cx, IR_PROC_VALUE, asn, asn);
        IR_LIT(pv).sval = (char *) nm;
        ir_operand_push(asn, pv);
        g->entry = pv;
    }
}
IR_graph_t * lower_icon_proc(const tree_t * prog, const tree_t * pd) {
    icn_collect_reassigned_procs(prog);
    icn_collect_reassigned_builtins(prog);
    icn_vec_reuse(&g_lower.icn.synth_excl, (int) sizeof(const char *));
    icn_progname_as_global(prog, pd);
    icn_vec_reuse(&g_lower.icn.lower_icon_proc_pnv, (int) sizeof(const char *));
    fill_pnames(prog, &g_lower.icn.lower_icon_proc_pnv);
    icx_t cx;
    memset(&cx, 0, sizeof cx);
    cx.prog = prog;
    cx.pn = (const char **) g_lower.icn.lower_icon_proc_pnv.data;
    cx.npn = g_lower.icn.lower_icon_proc_pnv.n;
    lc_vec_init(&g_lower.icn.lower_icon_proc_lnv, (int) sizeof(const char *));
    g_lower.icn.lower_icon_proc_lnv.n = 0;
    lc_vec_init(&g_lower.icn.synth_locals, (int) sizeof(const char *));
    g_lower.icn.synth_locals.n = 0;
    if (pd) {
        const tree_t * plist = (pd->n > 1) ? pd->c[1] : NULL;
        for (int i = 0; plist && i < plist->n; i++) if (plist->c[i] && plist->c[i]->v.sval) lc_vec_push(&g_lower.icn.lower_icon_proc_lnv, &plist->c[i]->v.sval);
        const tree_t * body = (pd->n > 2) ? pd->c[2] : NULL;
        cx.body = body;
        icn_statics_prepass((tree_t *) body, pd->v.sval ? pd->v.sval : "anon");
        for (int i = 0; body && i < body->n; i++) {
            const tree_t * st = body->c[i];
            if (st && st->t == TT_STMT) st = lic_stmt_subj(st);
            if (st && st->t == TT_LOCAL) for (int k = 0; k < st->n; k++) if (st->c[k] && st->c[k]->v.sval) lc_vec_push(&g_lower.icn.lower_icon_proc_lnv, &st->c[k]->v.sval);
        }
    }
    cx.ln = (const char **) g_lower.icn.lower_icon_proc_lnv.data;
    cx.nln = g_lower.icn.lower_icon_proc_lnv.n;
    {
        icn_vec_reuse(&g_lower.icn.lower_icon_proc_gnv, (int) sizeof(const char *));
        icn_collect_own_globals(prog, &g_lower.icn.lower_icon_proc_gnv);
        cx.gn = (const char **) g_lower.icn.lower_icon_proc_gnv.data;
        cx.ngn = g_lower.icn.lower_icon_proc_gnv.n;
        cx.pname = (pd && pd->v.sval) ? pd->v.sval : "anon";
    }
    cx.want_lines = icn_tree_mentions_kw(prog, "&trace");
    cx.has_trap = prog && prog->v.ival && icn_tree_mentions_kw(prog, "trap");
    cx.end_line = pd ? pd->slen : 0;
    {
        extern const char * stmt_src_get_file(void);
        const char * mf = stmt_src_get_file();
        cx.file = mf ? mf : "";
        for (int i = 0; prog && i < prog->n; i++) {
            const tree_t * s = prog->c[i];
            if (!s || s->t != TT_STMT) continue;
            const tree_t * fl = lp_s_expr(s, ":file");
            if (!fl || !fl->v.sval) continue;
            cx.links = 1;
            if (lic_stmt_subj(s) == pd) cx.file = fl->v.sval;
        }
    }
    if (pd && pd->n > 2 && pd->c[2]) {
        IR_graph_t * g = lower_proc_body(&cx, pd->c[2]);
        if (g && cx.pname && !strcmp(cx.pname, "main")) icn_init_reassigned_procs(&cx, g);
        if (g) {
            int np = pd->n > 1 && pd->c[1] ? pd->c[1]->n : 0;
            g->nparams = np;
            g->pnames = np > 0 ? (const char **)g_lower.icn.lower_icon_proc_lnv.data : NULL;
            g->nlocals = g_lower.icn.lower_icon_proc_lnv.n - np;
            g->lnames = (g_lower.icn.lower_icon_proc_lnv.n - np) > 0 ? (const char **)g_lower.icn.lower_icon_proc_lnv.data + np : NULL;
            if (pd->v.sval && !strcmp(pd->v.sval, "main")) g->root_graph = 1;
        }
        return g;
    }
    IR_graph_t * g = IR_alloc(64);
    cx.g = g;
    IR_t * s = lic_build(&cx, IR_SUCCEED, 0, 0);
    g->entry = s;
    g->cell_frame = 1;
    g->spine_cells = 1;
    g->scan_regs = 1;
    return g;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_subtree_has_suspend(const tree_t *n) {
    if (!n) return 0;
    if (n->t == TT_SUSPEND) return 1;
    if (n->t == TT_PROC_DECL || n->t == TT_SUB_DECL) return 0;
    for (int i = 0; i < n->n; i++) if (icn_subtree_has_suspend(n->c[i])) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_body_has_suspend(const tree_t *proc) { if (!proc) return 0; for (int i = 0; i < proc->n; i++) if (icn_subtree_has_suspend(proc->c[i])) return 1; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int lower_icon_body(const tree_t *prog, const tree_t *proc) { IR_graph_t * ng = lower_icon_proc(prog, proc); if (!ng || !ng->entry) return -1; return bb_program_add(&g_stage2.bbp, ng); }
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
        if (proc->t == TT_GLOBAL) { for (int _gi = 0; _gi < proc->n; _gi++) if (proc->c[_gi] && proc->c[_gi]->v.sval) global_register(proc->c[_gi]->v.sval); }
        if (proc->t == TT_RECORD && proc->v.sval && *proc->v.sval) {
            icn_note_record_name(proc->v.sval);
            size_t cap = strlen(proc->v.sval) + 3;
            for (int _ri = 0; _ri < proc->n; _ri++) cap += ((proc->c[_ri] && proc->c[_ri]->v.sval) ? strlen(proc->c[_ri]->v.sval) : 0) + 1;
            char * spec = (char *) ct_alloc(cap);
            size_t pos = 0;
            pos += (size_t) snprintf(spec + pos, cap - pos, "%s(", proc->v.sval);
            for (int _ri = 0; _ri < proc->n; _ri++) {
                if (_ri > 0) spec[pos++] = ',';
                const char *fn2 = (proc->c[_ri] && proc->c[_ri]->v.sval) ? proc->c[_ri]->v.sval : "";
                pos += (size_t) snprintf(spec + pos, cap - pos, "%s", fn2);
            }
            spec[pos++] = ')';
            spec[pos] = '\0';
            record_register(spec);
            ct_drop(spec);
        }
        if (proc->t == TT_FNC || proc->t == TT_PROC_DECL || proc->t == TT_SUB_DECL) {
            const char *name = NULL;
            if (proc->t == TT_SUB_DECL) {
                if (proc->n > 0 && proc->c[0] && proc->c[0]->t == TT_VAR && proc->c[0]->v.sval && *proc->c[0]->v.sval) name = proc->c[0]->v.sval;
            } else {
                name = (proc->v.sval && *proc->v.sval) ? proc->v.sval :
                    ((proc->n > 0 && proc->c[0] && proc->c[0]->t == TT_VAR && proc->c[0]->v.sval && *proc->c[0]->v.sval) ? proc->c[0]->v.sval : NULL);
            }
            if (name) {
                int _dup = 0;
                for (int _qi = 0; _qi < s2->proc_count; _qi++) if (s2->proc_table[_qi].name && !strcmp(s2->proc_table[_qi].name, name)) { _dup = 1; break; }
                if (_dup) continue;
                int _pi = stage2_proc_grow(s2);
                s2->proc_table[_pi].name = name;
                s2->proc_table[_pi].proc = proc;
                s2->proc_table[_pi].entry_pc = -1;
                s2->proc_table[_pi].bb_idx = -1;
                s2->proc_table[_pi].nparams = (proc->t == TT_PROC_DECL && proc->n >= 2 ? proc->c[1]->n : 0);
                s2->proc_table[_pi].is_variadic = (proc->t == TT_PROC_DECL && proc->n >= 2 && proc->c[1] ? (int)proc->c[1]->v.ival : 0);
                s2->proc_table[_pi].byref_mask = 0;
                if (mod_idx >= 0) s2->module_registry.mods[mod_idx].nprocs++;
                if (strcmp(name, "main") == 0 && s2->module_registry.main_mod < 0) s2->module_registry.main_mod = mod_idx;
            }
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void icn_display_homes_every_local(void) {
    extern void zls_home_local_declare(const IR_graph_t *, const char *, int);
    int on = 0;
    for (int gi = 0; gi < g_stage2.bbp.count && !on; gi++) {
        const IR_graph_t * g = g_stage2.bbp.table[gi];
        for (int i = 0; g && i < g->n && !on; i++) {
            const IR_t * x = g->all[i];
            if (x && (x->op == IR_CALL || x->op == IR_VAR || x->op == IR_VAR_REF) && IR_LIT(x).sval && !strcmp(IR_LIT(x).sval, "display")) on = 1;
        }
    }
    for (int gi = 0; gi < g_stage2.bbp.count; gi++) {
        const IR_graph_t * g = g_stage2.bbp.table[gi];
        int here = on;
        for (int i = 0; g && i < g->n && !here; i++) here = g->all[i] && g->all[i]->op == IR_MATCH_CASMARK;
        for (int k = 0; here && g->lnames && k < g->nlocals; k++) if (g->lnames[k] && g->lnames[k][0] != '&') zls_home_local_declare(g, g->lnames[k], 9);
    }
}
stage2_t *lower_icon_stage2(const tree_t *prog) {
    icn_expand_classes((tree_t *) prog);
    icn_expand_deferred((tree_t *) prog);
    icn_collect_reassigned_procs(prog);
    int _icn_bb0 = g_stage2.bbp.count;
    icon_register_program(&g_stage2, prog);
    for (int _rk = 0; _rk < g_lower.icn.reassigned.n; _rk++) { const char * _rn = LC_AT(&g_lower.icn.reassigned, const char *, _rk); if (icn_is_proc(_rn)) global_register(_rn); }
    icn_collect_reassigned_builtins(prog);
    for (int _rk = 0; _rk < g_lower.icn.breassigned.n; _rk++) global_register(LC_AT(&g_lower.icn.breassigned, const char *, _rk));
    lc_vec _icn_own_globals;
    lc_vec_init(&_icn_own_globals, (int) sizeof(const char *));
    icn_collect_own_globals(prog, &_icn_own_globals);
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
            for (int k = 0; k < np && plist && k < plist->n; k++) {
                stage2_scope_reserve(sc, sc->n + 1);
                const tree_t *pv = plist->c[k];
                if (!pv || !pv->v.sval) continue;
                sc->e[sc->n].name = lp_strdup(pv->v.sval);
                sc->e[sc->n].slot = sc->n;
                sc->n++;
            }
            if (np > 0) {
                const char ** _pn = (const char **)ct_zalloc((size_t)np, sizeof(const char *));
                if (_pn) { for (int k = 0; k < np && k < sc->n; k++) _pn[k] = sc->e[k].name; g_stage2.bbp.table[bb_idx]->pnames = _pn; }
            }
            {
                const tree_t *body = (proc->n > 2) ? proc->c[2] : NULL;
                int nl = 0;
                for (int i = 0; body && i < body->n; i++) {
                    const tree_t *st = body->c[i];
                    if (st && st->t == TT_STMT) st = lic_stmt_subj(st);
                    if (st && st->t == TT_LOCAL) for (int k = 0; k < st->n; k++) if (st->c[k] && st->c[k]->v.sval) nl++;
                }
                const char ** _ln = NULL;
                int w = 0;
                if (nl > 0) {
                    _ln = (const char **)ct_zalloc((size_t)nl, sizeof(const char *));
                    for (int i = 0; body && i < body->n && _ln; i++) {
                        const tree_t *st = body->c[i];
                        if (st && st->t == TT_STMT) st = lic_stmt_subj(st);
                        if (st && st->t == TT_LOCAL) for (int k = 0; k < st->n; k++) if (st->c[k] && st->c[k]->v.sval) _ln[w++] = lp_strdup(st->c[k]->v.sval);
                    }
                }
                icn_vec_reuse(&g_lower.icn.lower_icon_stage2_excl, (int) sizeof(const char *));
                for (int k = 0; k < _icn_own_globals.n; k++) lc_vec_push(&g_lower.icn.lower_icon_stage2_excl, &LC_AT(&_icn_own_globals, const char *, k));
                for (int k = 0; k < sc->n; k++) lc_vec_push(&g_lower.icn.lower_icon_stage2_excl, &sc->e[k].name);
                for (int k = 0; k < w; k++) lc_vec_push(&g_lower.icn.lower_icon_stage2_excl, &_ln[k]);
                for (int k = 0; k < g_lower.icn.synth_excl.n; k++) lc_vec_push(&g_lower.icn.lower_icon_stage2_excl, &LC_AT(&g_lower.icn.synth_excl, const char *, k));
                icn_vec_reuse(&g_lower.icn.lower_icon_stage2_impl, (int) sizeof(const char *));
                icn_collect_implicit_locals(body, (const char **)g_lower.icn.lower_icon_stage2_excl.data, g_lower.icn.lower_icon_stage2_excl.n, &g_lower.icn.lower_icon_stage2_impl,
                    prog && prog->v.ival);
                if (w > 0 || g_lower.icn.lower_icon_stage2_impl.n > 0 || g_lower.icn.synth_locals.n > 0) {
                    int total = w + g_lower.icn.lower_icon_stage2_impl.n + g_lower.icn.synth_locals.n;
                    const char ** _all = (const char **)ct_zalloc((size_t)total, sizeof(const char *));
                    int a = 0;
                    if (_all) {
                        for (int k = 0; k < w; k++) _all[a++] = _ln[k];
                        for (int k = 0; k < g_lower.icn.lower_icon_stage2_impl.n; k++) _all[a++] = LC_AT(&g_lower.icn.lower_icon_stage2_impl, const char *, k);
                        for (int k = 0; k < g_lower.icn.synth_locals.n; k++) _all[a++] = LC_AT(&g_lower.icn.synth_locals, const char *, k);
                        g_stage2.bbp.table[bb_idx]->lnames = _all;
                        g_stage2.bbp.table[bb_idx]->nlocals = total;
                    }
                }
            }
        }
    }
    icn_display_homes_every_local();
    lower_icon_resolve_call_kinds(prog && prog->v.ival);
    return &g_stage2;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int icn_callable_proc_index(const char * fn) {
    for (int pi = 0; pi < g_stage2.proc_count; pi++) {
        const char * pname = g_stage2.proc_table[pi].name;
        if (!pname || strcmp(pname, fn) != 0) continue;
        if (strcmp(pname, "main") == 0 && g_stage2.proc_table[pi].is_generator) return -1;
        int idx = g_stage2.proc_table[pi].bb_idx;
        if (idx < 0 || idx >= g_stage2.bbp.count || !g_stage2.bbp.table[idx] || !g_stage2.bbp.table[idx]->entry) return -1;
        return pi;
    }
    return -1;
}
static void icn_register_reassigned_builtin_globals(void) {
    extern int rt_builtin_is_known(const char *);
    extern int rt_builtin_is_generator(const char *);
    extern int is_global(const char *);
    extern void global_register(const char *);
    extern void rt_note_reassigned_builtin(const char *);
    for (int gi = 0; gi < g_stage2.bbp.count; gi++) {
        IR_graph_t * g = g_stage2.bbp.table[gi];
        if (!g) continue;
        for (int i = 0; i < g->n; i++) {
            IR_t * nd = g->all[i];
            if (!nd) continue;
            const char * tgt = 0;
            if (nd->op == IR_ASSIGN) tgt = IR_LIT(nd).sval;
            else if (nd->op == IR_REV_ASSIGN && nd->n_operands > 1 && nd->operands[1]) tgt = IR_LIT(nd->operands[1]).sval;
            if (!tgt || !tgt[0] || tgt[0] == '&') continue;
            if (graph_has_local(g, tgt) || is_global(tgt)) continue;
            int isproc = 0;
            for (int pi = 0; pi < g_stage2.proc_count; pi++) if (g_stage2.proc_table[pi].name && !strcmp(g_stage2.proc_table[pi].name, tgt)) { isproc = 1; break; }
            if (isproc || rt_builtin_is_known(tgt) || rt_builtin_is_generator(tgt) || icn_builtin_arity(tgt) != -99) { global_register(tgt); rt_note_reassigned_builtin(tgt); }
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void lower_icon_resolve_call_kinds(int unicon) {
    extern int rt_builtin_is_generator(const char *);
    extern int rt_builtin_is_known(const char *);
    extern int is_global(const char *);
    icn_register_reassigned_builtin_globals();
    { extern void rt_icn_global_note(const char *); for (int _pi = 0; _pi < g_stage2.proc_count; _pi++) if (g_stage2.proc_table[_pi].name) rt_icn_global_note(g_stage2.proc_table[_pi].name); }
    for (int gi = 0; gi < g_stage2.bbp.count; gi++) {
        IR_graph_t * g = g_stage2.bbp.table[gi];
        if (!g || !g->cell_frame) continue;
        for (int i = 0; i < g->n; i++) {
            IR_t * nd = g->all[i];
            if (!nd || nd->op != IR_CALL) continue;
            const char * fn = IR_LIT(nd).sval;
            if (!fn || !fn[0]) continue;
            {
                extern void rt_icn_global_note(const char *);
                if (strncmp(fn, "__trace_", 8) != 0 && (icn_callable_proc_index(fn) >= 0 || rt_builtin_is_known(fn) || rt_builtin_is_generator(fn) || icn_builtin_arity(fn) != -99))
                    rt_icn_global_note(fn);
            }
            int pi = icn_callable_proc_index(fn);
            if (pi >= 0 && g_stage2.proc_table[pi].is_generator) nd->op = IR_PROC_GEN;
            else if (pi >= 0) nd->op = IR_CALL_PROC_STAGED;
            else if (icn_builtin_is_generator(fn)) nd->op = IR_CALL_ICON;
            else if (!strcmp(fn, "system")) { IR_LIT(nd).sval = "$system"; nd->op = IR_CALL_ICON; } else if (icn_builtin_is_known(fn)) nd->op = IR_CALL_ICON;
        }
        for (int i = 0; i < g->n; i++) {
            IR_t * nd = g->all[i];
            if (!nd || (nd->op != IR_VAR && nd->op != IR_VAR_REF)) continue;
            const char * vn = IR_LIT(nd).sval;
            if (!vn || !vn[0] || vn[0] == '&') continue;
            if (is_global(vn)) continue;
            if (graph_has_local(g, vn)) continue;
            int skip = 0;
            for (int k = 0; !skip && g->pnames && k < g->nparams; k++) if (g->pnames[k] && !strcmp(g->pnames[k], vn)) skip = 1;
            for (int k = 0; !skip && k < g->n; k++) {
                IR_t * a = g->all[k];
                if (!a) continue;
                if (a->op == IR_ASSIGN && IR_LIT(a).sval && !strcmp(IR_LIT(a).sval, vn)) skip = 1;
                if (a->op == IR_REV_ASSIGN && a->n_operands > 1 && a->operands[1] && IR_LIT(a->operands[1]).sval && !strcmp(IR_LIT(a->operands[1]).sval, vn)) skip = 1;
            }
            if (skip || icn_proc_reassigned(vn)) continue;
            int isproc = 0;
            for (int pi = 0; pi < g_stage2.proc_count; pi++) if (g_stage2.proc_table[pi].name && !strcmp(g_stage2.proc_table[pi].name, vn)) { isproc = 1; break; }
            { char * un = (unicon && !isproc && (!strcmp(vn, "max") || !strcmp(vn, "min"))) ? icn_unicon_row(vn) : NULL; if (un) { IR_LIT(nd).sval = un; nd->op = IR_PROC_VALUE; continue; } }
            if (isproc || rt_builtin_is_known(vn) || rt_builtin_is_generator(vn) || icn_builtin_arity(vn) != -99 || !strcmp(vn, "push") || !strcmp(vn, "put")) {
                extern void rt_icn_global_note(const char *);
                rt_icn_global_note(vn);
                nd->op = IR_PROC_VALUE;
            }
        }
    }
}
