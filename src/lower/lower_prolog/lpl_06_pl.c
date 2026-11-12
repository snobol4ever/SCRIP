#include "lower/lower_prolog/lpl_head.h"
#include <stdio.h>
#include "runtime/core/g_runtime.h"
#include <stdlib.h>
#include "ir/ir_graph.h"
#include "ir/stage2.h"
#include "lower/lower_prolog.h"
#include "lower/lower_prolog_chr.h"
#include "parsers/snobol4/scrip_cc.h"
#include "lower/lower_common.h"
#include "lower/lower_prolog/lpl_01_pl.h"
#include "lower/lower_prolog/lpl_02_pl.h"
#include "lower/lower_prolog/lpl_04_pl.h"
#include "lower/lower_prolog/lpl_05_pl.h"
#include "lower/lower_prolog/lpl_06_pl.h"
#include "lower/lower_prolog/lpl_07_pl.h"
int pl_tree_is_nil(const tree_t * t) { if (!t) return 0; if (t->t == TT_MAKELIST) return t->n == 0; return (t->t == TT_QLIT || t->t == TT_NAME) && t->v.sval && !strcmp(t->v.sval, "[]"); }
const char * const pl_decl_directives[] = { "multifile", "discontiguous", "ensure_loaded", "use_module", "module", "meta_predicate", "dynamic", "table", "use_subsumptive_tabling",
    "module_transparent", "$module_restore", "public", "mode", NULL };
static void pl_decl_dynamic_record(stage2_t * s2, tree_t * spec, tree_t * marker) {
    if (!spec) return;
    if (spec->t == TT_FNC && spec->v.sval && !strcmp(spec->v.sval, ",") && spec->n == 2) { pl_decl_dynamic_record(s2, spec->c[0], marker); pl_decl_dynamic_record(s2, spec->c[1], marker); return; }
    if (spec->t == TT_MAKELIST) { for (int i = 0; i < spec->n; i++) pl_decl_dynamic_record(s2, spec->c[i], marker); return; }
    if (spec->t == TT_FNC && spec->v.sval && !strcmp(spec->v.sval, "as") && spec->n == 2) {
        if (pl_table_opt_kind(spec->c[1]) & 3) pl_hook_dyn_record(spec->c[0]);
        pl_decl_dynamic_record(s2, spec->c[0], marker);
        return;
    }
    if (spec->t == TT_FNC && spec->v.sval && !strcmp(spec->v.sval, ":") && spec->n == 2) { pl_decl_dynamic_record(s2, spec->c[1], marker); return; }
    if (spec->t == TT_FNC && spec->v.sval && !strcmp(spec->v.sval, "/") && spec->n == 2 && spec->c[0] && (spec->c[0]->t == TT_QLIT || spec->c[0]->t == TT_NAME) && spec->c[0]->v.sval && spec->c[1] &&
        spec->c[1]->t == TT_ILIT) {
        char key[fmt_len("%s/%d", spec->c[0]->v.sval, (int) spec->c[1]->v.ival)];
        snprintf(key, sizeof key, "%s/%d", spec->c[0]->v.sval, (int) spec->c[1]->v.ival);
        pl_decl_dyn_mark(spec->c[0]->v.sval, (int) spec->c[1]->v.ival);
        if (!resolve_pred_table_lookup(&s2->resolve_pred_table, key)) resolve_pred_table_insert(&s2->resolve_pred_table, ct_strdup(key), marker);
        return;
    }
    if ((spec->t == TT_QLIT || spec->t == TT_NAME) && spec->v.sval) {
        fprintf(stderr, "scrip: prolog: dynamic/1: type_error(predicate_indicator, %s) -- a bare atom declares nothing; the directive is reported and ignored as swipl reports and ignores it\n",
            spec->v.sval);
        return;
    }
    pl_refuse("dynamic declaration shape", spec->v.sval ? spec->v.sval : "?", 10);
}
static int pl_flag_directive_is_advisory(const tree_t * subj) {
    static const char * const advisory[] = { "optimise", "last_call_optimisation", "generate_debug_info", "agc_margin", "stack_limit", "xref", "verbose", "report_error", "autoload", NULL };
    const tree_t * f = subj->c[0];
    if (!f || !(f->t == TT_QLIT || f->t == TT_NAME) || !f->v.sval) return 0;
    for (int i = 0; advisory[i]; i++) if (!strcmp(f->v.sval, advisory[i])) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_swi_library_leaf(const char * nm, int ar) {
    static const struct {
        const char * nm;
        int ar;
    } lib[] = { { "display", 1 }, { "display", 2 }, { "numbervars", 1 }, { "string_to_atom", 2 }, { "concat_atom", 2 }, { "concat_atom", 3 }, { "read_term_from_chars", 3 }, { "read_term_from_codes",
        3 }, { "unget_char", 1 }, { "unget_char", 2 }, { "unget_code", 1 }, { "unget_byte", 1 }, { "unget_byte", 2 }, { "wall_us", 1 }, { "wall_ms", 1 }, { "sort", 1 }, { "msort", 1 }, { "keysort",
        1 }, { "last_read_start_line_column", 2 }, { "working_directory", 1 }, { "change_directory", 1 }, { "file_exists", 1 }, { "prolog_pid", 1 }, { "pipe", 2 }, { "print", 1 }, { "print", 2 }, { 0,
        0 } };
    for (int i = 0; lib[i].nm; i++) if (lib[i].ar == ar && !strcmp(lib[i].nm, nm)) return 1;
    return 0;
}
static int pl_synth_have(const char * key) { extern int rt_proc_is_registered(const char *); return g_stage2.harness_warm == 2 && rt_proc_is_registered(key); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_swi_absent_leaf(const char * nm, int ar) {
    return g_stage2.pl_dialect == PL_DIALECT_SWI && (ar == 1 || ar == 2) && (!strcmp(nm, "unget_char") || !strcmp(nm, "unget_code") || !strcmp(nm, "unget_byte"));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_attv_rt_armed(void) {
    extern int rt_proc_is_registered(const char *);
    if (!g_stage2.pl_attv_armed && g_stage2.harness_warm != 2 && rt_proc_is_registered("$wakeup/0")) g_stage2.pl_attv_armed = 1;
    return g_stage2.pl_attv_armed;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void pl_dir_number_vars(tree_t * t, const char ** names, int * n, int scope0) {
    if (!t) return;
    if (t->t == TT_VAR) {
        const char * nm = t->v.sval;
        int slot = *n;
        if (nm && strcmp(nm, "_") != 0) { for (int i = scope0; i < *n; i++) if (names[i] && !strcmp(names[i], nm)) { slot = i; break; } }
        if (slot == *n) { names[*n] = nm; (*n)++; }
        t->v.ival = slot;
        return;
    }
    for (int i = 0; i < t->n; i++) pl_dir_number_vars(t->c[i], names, n, scope0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * pl_dir_catch_wrap(tree_t * dirgoal, const char ** dvn, int * dvc) {
    int dv_scope0 = *dvc;
    tree_t * eb = ast_node_new(TT_VAR);
    eb->v.sval = (char *) "$DirBall";
    tree_t * eb2 = ast_node_new(TT_VAR);
    eb2->v.sval = (char *) "$DirBall";
    tree_t * fargs = ast_node_new(TT_MAKELIST);
    fargs->v.ival = 0;
    ast_push(fargs, eb2);
    tree_t * rep = ast_node_new(TT_FNC);
    rep->v.sval = (char *) "format";
    ast_push(rep, (tree_t *) pl_atom_goal("user_error"));
    ast_push(rep, (tree_t *) pl_atom_goal("Warning: directive raised: ~q~n"));
    ast_push(rep, fargs);
    tree_t * cat = ast_node_new(TT_FNC);
    cat->v.sval = (char *) "catch";
    ast_push(cat, dirgoal);
    ast_push(cat, eb);
    ast_push(cat, rep);
    tree_t * ig = pl_cc_fnc1("ignore", cat);
    pl_dir_number_vars(ig, dvn, dvc, dv_scope0);
    return ig;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_db_decls_words(long long ** out) { *out = g_stage2.db_decls_words; return g_stage2.db_decls_words ? g_stage2.db_decls_n : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_table_index_mode(const tree_t * m) { return !m || m->t == TT_VAR || ((m->t == TT_QLIT || m->t == TT_NAME) && m->v.sval && (!strcmp(m->v.sval, "index") || !strcmp(m->v.sval, "+"))); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_table_specs(const tree_t * a, const char ** keys, const tree_t ** specs, int n, int cap) {
    if (!a || a->t != TT_FNC || !a->v.sval || a->n < 1) return n;
    if (a->n == 2 && !strcmp(a->v.sval, ",")) return pl_table_specs(a->c[1], keys, specs, pl_table_specs(a->c[0], keys, specs, n, cap), cap);
    if (a->n == 2 && !strcmp(a->v.sval, "as")) return pl_table_specs(a->c[0], keys, specs, n, cap);
    if (a->n == 2 && (!strcmp(a->v.sval, "/") || !strcmp(a->v.sval, "//"))) {
        if (!a->c[0] || (a->c[0]->t != TT_QLIT && a->c[0]->t != TT_NAME) || !a->c[0]->v.sval || !a->c[1] || a->c[1]->t != TT_ILIT) return n;
        if (keys && n < cap) {
            char b[strlen(a->c[0]->v.sval) + 24];
            snprintf(b, sizeof b, "%s/%lld", a->c[0]->v.sval, a->c[1]->v.ival + (a->v.sval[1] == '/' ? 2 : 0));
            keys[n] = lp_strdup(b);
            specs[n] = NULL;
        }
        return n + 1;
    }
    if (!strcmp(a->v.sval, ":") || !strcmp(a->v.sval, "/") || !strcmp(a->v.sval, "//")) return n;
    if (keys && n < cap) {
        int moded = 0;
        for (int i = 0; i < a->n; i++) if (!pl_table_index_mode(a->c[i])) moded = 1;
        char b[strlen(a->v.sval) + 24];
        snprintf(b, sizeof b, "%s/%d", a->v.sval, a->n);
        keys[n] = lp_strdup(b);
        specs[n] = moded ? a : NULL;
    }
    return n + 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_table_opt_kind(const tree_t * o) {
    if (!o || !o->v.sval) return 0;
    if (o->t == TT_FNC && o->n == 2 && !strcmp(o->v.sval, ",")) return pl_table_opt_kind(o->c[0]) | pl_table_opt_kind(o->c[1]);
    if (o->t != TT_QLIT && o->t != TT_NAME) return 0;
    const char * const nm[] = { "incremental", "monotonic", "lazy", "subsumptive", "variant" };
    for (int i = 0; i < 5; i++) if (!strcmp(o->v.sval, nm[i])) return 1 << i;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_table_kind(const tree_t * a, const char ** keys, int nf, unsigned char * kind) {
    if (!a || a->t != TT_FNC || !a->v.sval || a->n != 2) return;
    if (!strcmp(a->v.sval, ",")) { pl_table_kind(a->c[0], keys, nf, kind); pl_table_kind(a->c[1], keys, nf, kind); return; }
    int b = strcmp(a->v.sval, "as") ? 0 : pl_table_opt_kind(a->c[1]);
    if (!b) return;
    int m = pl_table_specs(a->c[0], NULL, NULL, 0, 0);
    const char * ks[m > 0 ? m : 1];
    const tree_t * sp[m > 0 ? m : 1];
    m = pl_table_specs(a->c[0], ks, sp, 0, m);
    for (int j = 0; j < m; j++) for (int k = 0; k < nf; k++) if (!strcmp(ks[j], keys[k])) kind[k] |= (unsigned char) b;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_table_subs_kind(const tree_t * s, const char ** keys, int nf, unsigned char * kind) {
    const tree_t * subj = (s && s->t == TT_STMT) ? lp_s_expr(s, ":subj") : NULL;
    if (!subj || !subj->v.sval || strcmp(subj->v.sval, "use_subsumptive_tabling")) return;
    if (subj->t == TT_QLIT || subj->t == TT_NAME) { for (int k = 0; k < nf; k++) if (!(kind[k] & 23)) kind[k] |= 8; return; }
    if (subj->t != TT_FNC || subj->n < 1) return;
    int m = pl_table_specs(subj->c[0], NULL, NULL, 0, 0);
    const char * ks[m > 0 ? m : 1];
    const tree_t * sp[m > 0 ? m : 1];
    m = pl_table_specs(subj->c[0], ks, sp, 0, m);
    for (int j = 0; j < m; j++) for (int k = 0; k < nf; k++) if (!strcmp(ks[j], keys[k])) kind[k] |= 8;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const tree_t * pl_table_dir(const tree_t * s) {
    const tree_t * subj = (s && s->t == TT_STMT) ? lp_s_expr(s, ":subj") : NULL;
    return (subj && subj->t == TT_FNC && subj->v.sval && (!strcmp(subj->v.sval, "table") || !strcmp(subj->v.sval, "use_subsumptive_tabling")) && subj->n >= 1) ? subj->c[0] : NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t * pl_table_goal(const char * f, int ar) {
    tree_t * g = ast_node_new(ar > 0 ? TT_FNC : TT_QLIT);
    g->v.sval = (char *) f;
    for (int i = 0; i < ar; i++) { tree_t * v = ast_node_new(TT_VAR); v->v.ival = i; ast_push(g, v); }
    return g;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * pl_table_mode_copy(const tree_t * t) {
    tree_t * c = ast_node_new(t->t == TT_VAR ? TT_QLIT : t->t);
    c->line = t->line;
    if (t->t == TT_VAR) { c->v.sval = (char *) "index"; return c; }
    c->v = t->v;
    for (int i = 0; i < t->n; i++) ast_push(c, pl_table_mode_copy(t->c[i]));
    return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * pl_table_wrapper(const char * key, const tree_t * spec, int kind) {
    const char * sl = strrchr(key, '/');
    int ar = atoi(sl + 1);
    char nm[sl - key + 1], tn[sl - key + 6];
    memcpy(nm, key, (size_t)(sl - key));
    nm[sl - key] = 0;
    snprintf(tn, sizeof tn, "$tbl %s", nm);
    tree_t * cl = ast_node_new(TT_CLAUSE);
    cl->v.dval = ar;
    for (int i = 0; i < ar; i++) { tree_t * v = ast_node_new(TT_VAR); v->v.ival = i; ast_push(cl, v); }
    tree_t * call = ast_node_new(TT_FNC);
    const char * tk = (kind & 4) ? "lazy" : (kind & 2) ? "mono" : (kind & 1) ? "incr" : (kind & 8) ? "subs" : (const char *) 0;
    call->v.sval = (char *) (spec ? "$tbl_mcall" : !tk ? "$tbl_call" : (kind & 14) ? "$tbl_kcall" : "$tbl_icall");
    ast_push(call, pl_table_goal(lp_strdup(nm), ar));
    ast_push(call, pl_table_goal(lp_strdup(tn), ar));
    if (spec) ast_push(call, pl_table_mode_copy(spec));
    if (tk && (spec || (kind & 14))) ast_push(call, (tree_t *) pl_atom_goal(tk));
    ast_push(cl, call);
    tree_t * ch = ast_node_new(TT_CHOICE);
    ch->v.sval = (char *) lp_strdup(key);
    ast_push(ch, cl);
    return ch;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_attv_nobind(const char * nm, int ar) {
    static const char * const a0[] = { "true", "fail", "false", "nl", "halt", "$wake_pending", "$wakeup", "listing", "$tbl_hdr", "$tbh_close", "$tbh_leading", NULL };
    static const char * const a1[] = { "var", "nonvar", "atom", "number", "integer", "float", "atomic", "compound", "callable", "is_list", "ground", "attvar", "write", "writeq", "print", "writeln",
        "write_canonical", "throw", "halt", "tab", "nl", "assert", "asserta", "assertz", "retractall", NULL };
    static const char * const a2[] = { "==", "\\==", "@<", "@>", "@=<", "@>=", "<", ">", "=<", ">=", "=:=", "=\\=", "write", "writeq", "print", "nl", "dif", "freeze", "when", "put_attr", "del_attr",
        NULL };
    const char * const * t = ar == 0 ? a0 : ar == 1 ? a1 : ar == 2 ? a2 : NULL;
    if (!t) return 0;
    for (int i = 0; t[i]; i++) if (!strcmp(t[i], nm)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * pl_attv_goal(const tree_t * g) {
    if (!g) return NULL;
    if (g->t == TT_CUT || g->t == TT_MAKELIST) return (tree_t *) g;
    if (g->t == TT_VAR || g->t == TT_UNIFY) return pl_cc_fnc2(",", (tree_t *) g, pl_attv_w());
    if (g->t == TT_IF && g->n == 3) { tree_t * n = ast_node_new(TT_IF); n->v = g->v; n->line = g->line; for (int i = 0; i < 3; i++) ast_push(n, pl_attv_goal(g->c[i])); return n; }
    if (g->t == TT_PROGRAM) { tree_t * n = ast_node_new(TT_PROGRAM); n->v = g->v; n->line = g->line; for (int i = 0; i < g->n; i++) ast_push(n, pl_attv_goal(g->c[i])); return n; }
    if (g->t == TT_QLIT || g->t == TT_NAME) {
        const char * nm = g->v.sval ? g->v.sval : "";
        if (pl_attv_user(nm, 0) || pl_attv_nobind(nm, 0)) return (tree_t *) g;
        return pl_cc_fnc2(",", (tree_t *) g, pl_attv_w());
    }
    if (g->t != TT_FNC || !g->v.sval) return (tree_t *) g;
    if (g->n == 2 && !strcmp(g->v.sval, "\\=")) return pl_attv_goal(pl_cc_fnc1("\\+", pl_cc_fnc2("=", (tree_t *) g->c[0], (tree_t *) g->c[1])));
    const char * nm = g->v.sval;
    int n = g->n;
    int ctl2 = n == 2 && (!strcmp(nm, ",") || !strcmp(nm, ";") || !strcmp(nm, "->") || !strcmp(nm, "*->"));
    int ctl1 = n == 1 && (!strcmp(nm, "\\+") || !strcmp(nm, "not") || !strcmp(nm, "once") || !strcmp(nm, "ignore") || (!strcmp(nm, "call") && g->c[0] && g->c[0]->t != TT_VAR));
    int modq = n == 2 && !strcmp(nm, ":") && g->c[0] && (g->c[0]->t == TT_QLIT || g->c[0]->t == TT_NAME);
    if (ctl2 || ctl1 || modq) {
        tree_t * c = ast_node_new(TT_FNC);
        c->v = g->v;
        c->line = g->line;
        for (int i = 0; i < n; i++) ast_push(c, (modq && i == 0) ? (tree_t *) g->c[i] : pl_attv_goal(g->c[i]));
        return c;
    }
    int gi0 = -1, gi1 = -1, w = 1;
    if ((n == 3 || n == 4) && !strcmp(nm, "findall")) gi0 = 1;
    else if (n == 3 && !strcmp(nm, "aggregate_all")) gi0 = 1;
    else if (n == 2 && !strcmp(nm, "forall")) { gi0 = 0; gi1 = 1; w = 0; } else if (n == 3 && !strcmp(nm, "catch")) { gi0 = 0; gi1 = 2; }
    if (gi0 >= 0) {
        tree_t * c = ast_node_new(TT_FNC);
        c->v = g->v;
        c->line = g->line;
        for (int i = 0; i < n; i++) ast_push(c, (i == gi0 || i == gi1) && g->c[i] ? pl_attv_goal(g->c[i]) : (tree_t *) g->c[i]);
        return w ? pl_cc_fnc2(",", c, pl_attv_w()) : c;
    }
    if (pl_attv_user(nm, n) || pl_attv_nobind(nm, n)) return (tree_t *) g;
    return pl_cc_fnc2(",", (tree_t *) g, pl_attv_w());
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * pl_attv_clause(const tree_t * cl, const tree_t * mod, int lift) {
    int ar = (int) cl->v.dval;
    const tree_t * hd = lift ? cl->c[1] : NULL;
    tree_t * nc = ast_node_new(TT_CLAUSE);
    nc->v = cl->v;
    nc->v.dval = lift ? 1 + hd->n : (mod ? ar + 1 : ar);
    nc->line = cl->line;
    if (lift) {
        ast_push(nc, (tree_t *) cl->c[0]);
        for (int i = 0; i < hd->n; i++) ast_push(nc, (tree_t *) hd->c[i]);
    } else {
        if (mod) ast_push(nc, (tree_t *) mod);
        for (int i = 0; i < ar; i++) ast_push(nc, (tree_t *) cl->c[i]);
    }
    for (int i = ar; i < cl->n; i++) ast_push(nc, (tree_t *) cl->c[i]);
    return nc;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_attv_on(void) { return g_stage2.pl_attv_armed || pl_attv_rt_armed(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * pl_attv_dir(tree_t * g) { return pl_attv_on() ? pl_attv_goal(g) : g; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_attv_body(const tree_t * cl, int ar, const tree_t ** out, int cap) {
    int trivial = 1, n = 0;
    for (int i = 0; i < ar && trivial; i++) {
        if (!cl->c[i] || cl->c[i]->t != TT_VAR) { trivial = 0; break; }
        for (int j = 0; j < i; j++) if (cl->c[j]->t == TT_VAR && cl->c[j]->v.ival == cl->c[i]->v.ival) { trivial = 0; break; }
    }
    if (!trivial && n < cap) out[n++] = pl_attv_w();
    for (int i = ar; i < cl->n && n < cap; i++) out[n++] = pl_attv_goal(cl->c[i]);
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * pl_attv_fact1(const tree_t * a) { tree_t * c = ast_node_new(TT_CLAUSE); c->v.dval = 1; ast_push(c, (tree_t *) a); return c; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_attv_is_key(const tree_t * subj, const char * key) { return subj && subj->t == TT_CHOICE && subj->v.sval && !strcmp(subj->v.sval, key); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_attv_hook_head(const tree_t * cl) {
    return cl && cl->t == TT_CLAUSE && (int) cl->v.dval == 2 && cl->c[0] && (cl->c[0]->t == TT_QLIT || cl->c[0]->t == TT_NAME) && cl->c[1] && cl->c[1]->t == TT_FNC && cl->c[1]->v.sval &&
        !strcmp(cl->c[1]->v.sval, "attr_unify_hook") && cl->c[1]->n == 2;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_attv_goals_head(const tree_t * cl) {
    return cl && cl->t == TT_CLAUSE && (int) cl->v.dval == 2 && cl->c[0] && (cl->c[0]->t == TT_QLIT || cl->c[0]->t == TT_NAME) && cl->c[1] && cl->c[1]->t == TT_FNC && cl->c[1]->v.sval &&
        !strcmp(cl->c[1]->v.sval, "attribute_goals") && cl->c[1]->n == 3;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * pl_attv_rewrite(const tree_t * prog) {
    int top = 0;
    for (int i = 0; i < prog->n; i++) { const tree_t * s = prog->c[i]; const tree_t * subj = (s && s->t == TT_STMT) ? lp_s_expr(s, ":subj") : NULL; if (pl_attv_is_key(subj, "$wakeup/0")) top = 1; }
    if (!top) return prog;
    if (getenv("SCRIP_PL_ATTV_TRACE")) fprintf(stderr, "scrip: prolog attributes armed\n");
    int has_ct3 = 0;
    for (int i = 0; i < prog->n; i++) {
        const tree_t * s = prog->c[i];
        const tree_t * subj = (s && s->t == TT_STMT) ? lp_s_expr(s, ":subj") : NULL;
        if (pl_attv_is_key(subj, "copy_term/3")) has_ct3 = 1;
    }
    g_stage2.pl_attv_armed = 1;
    tree_t * np = ast_node_new(prog->t);
    np->v = prog->v;
    np->line = prog->line;
    tree_t * hooks = ast_node_new(TT_CHOICE);
    hooks->v.sval = (char *) "$attr_unify_hook/3";
    tree_t * mods = ast_node_new(TT_CHOICE);
    mods->v.sval = (char *) "$attr_hook_mod/1";
    tree_t * goals = ast_node_new(TT_CHOICE);
    goals->v.sval = (char *) "$attribute_goals/4";
    tree_t * gmods = ast_node_new(TT_CHOICE);
    gmods->v.sval = (char *) "$attr_goals_mod/1";
    const tree_t * cur_mod = pl_atom_goal("user");
    const tree_t * last = NULL;
    for (int i = 0; i < prog->n; i++) {
        const tree_t * s = prog->c[i];
        const tree_t * subj = (s && s->t == TT_STMT) ? lp_s_expr(s, ":subj") : NULL;
        if (subj && subj->t == TT_FNC && subj->v.sval && (!strcmp(subj->v.sval, "module") || !strcmp(subj->v.sval, "$module_restore")) && subj->n >= 1 && subj->c[0] &&
            (subj->c[0]->t == TT_QLIT || subj->c[0]->t == TT_NAME)) cur_mod = subj->c[0];
        if (!subj || subj->t != TT_CHOICE) { ast_push(np, (tree_t *) s); continue; }
        last = s;
        int hk = pl_attv_is_key(subj, "attr_unify_hook/2");
        int qk = pl_attv_is_key(subj, ":/2");
        int gk = has_ct3 && pl_attv_is_key(subj, "attribute_goals/3");
        if (!hk && !qk && !gk) { ast_push(np, (tree_t *) s); continue; }
        tree_t * ch = ast_node_new(TT_CHOICE);
        ch->v = subj->v;
        ch->line = subj->line;
        for (int k = 0; k < subj->n; k++) {
            const tree_t * cl = subj->c[k];
            if (!cl || cl->t != TT_CLAUSE) { ast_push(ch, (tree_t *) cl); continue; }
            if (hk) { ast_push(hooks, pl_attv_clause(cl, cur_mod, 0)); ast_push(mods, pl_attv_fact1(cur_mod)); continue; }
            if (gk) { ast_push(goals, pl_attv_clause(cl, cur_mod, 0)); ast_push(gmods, pl_attv_fact1(cur_mod)); continue; }
            if (pl_attv_hook_head(cl)) { ast_push(hooks, pl_attv_clause(cl, NULL, 1)); ast_push(mods, pl_attv_fact1(cl->c[0])); continue; }
            if (has_ct3 && pl_attv_goals_head(cl)) { ast_push(goals, pl_attv_clause(cl, NULL, 1)); ast_push(gmods, pl_attv_fact1(cl->c[0])); continue; }
            ast_push(ch, (tree_t *) cl);
        }
        if (ch->n > 0) ast_push(np, pl_table_stmt(s, ch));
    }
    if (last) {
        tree_t * none = ast_node_new(TT_CLAUSE);
        none->v.dval = 3;
        ast_push(none, (tree_t *) pl_atom_goal("$none"));
        { tree_t * v = ast_node_new(TT_VAR); v->v.ival = 0; ast_push(none, v); }
        { tree_t * v = ast_node_new(TT_VAR); v->v.ival = 1; ast_push(none, v); }
        ast_push(none, (tree_t *) pl_atom_goal("fail"));
        ast_push(hooks, none);
        ast_push(mods, pl_attv_fact1(pl_atom_goal("$none")));
        ast_push(np, pl_table_stmt(last, hooks));
        ast_push(np, pl_table_stmt(last, mods));
        if (has_ct3) {
            tree_t * gnone = ast_node_new(TT_CLAUSE);
            gnone->v.dval = 4;
            ast_push(gnone, (tree_t *) pl_atom_goal("$none"));
            for (int k = 0; k < 3; k++) { tree_t * v = ast_node_new(TT_VAR); v->v.ival = k; ast_push(gnone, v); }
            ast_push(gnone, (tree_t *) pl_atom_goal("fail"));
            ast_push(goals, gnone);
            ast_push(gmods, pl_attv_fact1(pl_atom_goal("$none")));
            ast_push(np, pl_table_stmt(last, goals));
            ast_push(np, pl_table_stmt(last, gmods));
        }
    }
    return np;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * pl_blockop_rewrite(const tree_t * t) {
    if (!t) return t;
    {
        const tree_t * k[t->n + 1];
        int ch = 0;
        for (int i = 0; i < t->n; i++) { k[i] = pl_blockop_rewrite(t->c[i]); ch |= k[i] != t->c[i]; }
        if (t->t == TT_IDX && t->n == 2 && t->v.sval) { tree_t * f = ast_node_new(TT_FNC); f->v = t->v; f->line = t->line; ast_push(f, (tree_t *) k[1]); ast_push(f, (tree_t *) k[0]); return f; }
        if (!ch) return t;
        { tree_t * c = ast_node_new(t->t); c->v = t->v; c->line = t->line; c->slen = t->slen; for (int i = 0; i < t->n; i++) ast_push(c, (tree_t *) k[i]); return c; }
    }
}
static const tree_t * pl_table_rewrite(const tree_t * prog) {
    int nk = 0;
    for (int i = 0; i < prog->n; i++) nk = pl_table_specs(pl_table_dir(prog->c[i]), NULL, NULL, nk, 0);
    if (!nk) return prog;
    const char * keys[nk];
    const tree_t * specs[nk];
    unsigned char done[nk], kind[nk];
    int nf = 0;
    memset(done, 0, sizeof done);
    memset(kind, 0, sizeof kind);
    for (int i = 0; i < prog->n; i++) nf = pl_table_specs(pl_table_dir(prog->c[i]), keys, specs, nf, nk);
    for (int i = 0; i < prog->n; i++) pl_table_kind(pl_table_dir(prog->c[i]), keys, nf, kind);
    for (int i = 0; i < prog->n; i++) pl_table_subs_kind(prog->c[i], keys, nf, kind);
    for (int i = 0; i < prog->n; i++) {
        const tree_t * ds = (prog->c[i] && prog->c[i]->t == TT_STMT) ? lp_s_expr(prog->c[i], ":subj") : NULL;
        if (ds && ds->t == TT_FNC && ds->v.sval && !strcmp(ds->v.sval, "dynamic")) for (int q = 0; q < (ds->n == 2 ? 1 : ds->n); q++) pl_rs_dyn(ds->c[q]);
    }
    tree_t * np = ast_node_new(prog->t);
    np->v = prog->v;
    np->line = prog->line;
    for (int i = 0; i < prog->n; i++) {
        const tree_t * s = prog->c[i];
        const tree_t * subj = (s && s->t == TT_STMT) ? lp_s_expr(s, ":subj") : NULL;
        int k = -1;
        if (subj && (subj->t == TT_CHOICE || subj->t == TT_CLAUSE) && subj->v.sval) for (int j = 0; j < nf; j++) if (!strcmp(keys[j], subj->v.sval)) k = j;
        if (k < 0) { ast_push(np, (tree_t *) s); continue; }
        tree_t * rn = pl_tree_copy(subj), * rsx[512];
        char b[strlen(keys[k]) + 6];
        int nrs = 0;
        snprintf(b, sizeof b, "$tbl %s", keys[k]);
        rn->v.sval = (char *) lp_strdup(b);
        if ((kind[k] & 6) && rn->t == TT_CHOICE) for (int ci = 0; ci < rn->n; ci++) nrs = pl_rs_clause(rn, ci, keys[k], keys, nf, rsx, nrs);
        ast_push(np, pl_table_stmt(s, rn));
        for (int q = 0; q < nrs; q++) ast_push(np, pl_table_stmt(s, rsx[q]));
        if (!done[k]) { done[k] = 1; ast_push(np, pl_table_stmt(s, pl_table_wrapper(keys[k], specs[k], kind[k]))); }
    }
    return np;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_graph_t * pl_pa_graph(int i, int bb0) {
    int bi = g_stage2.proc_table[i].bb_idx;
    const char * pk = g_stage2.proc_table[i].name;
    return (pk && strrchr(pk, '/') && bi >= bb0 && bi < g_stage2.bbp.count) ? g_stage2.bbp.table[bi] : (IR_graph_t *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_pa_edges(int i, int bb0, int * out) {
    IR_graph_t * g = pl_pa_graph(i, bb0);
    int k = 0;
    if (g) for (int j = 0; j < g->n; j++) {
        const IR_t * nd = g->all[j];
        int c = (nd && nd->op == IR_CALL_PROC_STAGED && IR_LIT(nd).sval) ? bb_proc_find(IR_LIT(nd).sval) : -1;
        if (c >= 0 && pl_pa_graph(c, bb0)) { if (out) out[k] = c; k++; }
    }
    return k;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_poll_alpha_mark(int bb0) {
    int n = g_stage2.proc_count, t = 0, sp = 0;
    if (n <= 0) return;
    int * beg = (int *) ct_zalloc((size_t) n + 1, sizeof(int));
    for (int i = 0; i < n; i++) beg[i + 1] = beg[i] + pl_pa_edges(i, bb0, (int *)0);
    int * adj = (int *) ct_zalloc((size_t) beg[n] + 1, sizeof(int)), * num = (int *) ct_zalloc((size_t) n, sizeof(int)), * low = (int *) ct_zalloc((size_t) n, sizeof(int));
    int * stk = (int *) ct_zalloc((size_t) n, sizeof(int)), * ons = (int *) ct_zalloc((size_t) n, sizeof(int)), * cst = (int *) ct_zalloc((size_t) n, sizeof(int));
    int * cpos = (int *) ct_zalloc((size_t) n, sizeof(int));
    for (int i = 0; i < n; i++) (void) pl_pa_edges(i, bb0, adj + beg[i]);
    for (int r = 0; r < n; r++) {
        if (num[r] || !pl_pa_graph(r, bb0)) continue;
        int dp = 0;
        cst[0] = r;
        cpos[0] = beg[r];
        num[r] = low[r] = ++t;
        stk[sp++] = r;
        ons[r] = 1;
        while (dp >= 0) {
            int v = cst[dp];
            if (cpos[dp] < beg[v + 1]) {
                int w = adj[cpos[dp]++];
                if (!num[w]) { dp++; cst[dp] = w; cpos[dp] = beg[w]; num[w] = low[w] = ++t; stk[sp++] = w; ons[w] = 1; } else if (ons[w] && num[w] < low[v]) low[v] = num[w];
                continue;
            }
            if (low[v] == num[v]) {
                int k = sp - 1;
                while (stk[k] != v) k--;
                int cyc = sp - k > 1;
                for (int e = beg[v]; e < beg[v + 1] && !cyc; e++) cyc = adj[e] == v;
                for (int m = k; m < sp; m++) { ons[stk[m]] = 0; if (cyc) pl_pa_graph(stk[m], bb0)->poll_alpha = 1; }
                sp = k;
            }
            dp--;
            if (dp >= 0 && low[v] < low[cst[dp]]) low[cst[dp]] = low[v];
        }
    }
    ct_drop(cpos);
    ct_drop(cst);
    ct_drop(ons);
    ct_drop(stk);
    ct_drop(low);
    ct_drop(num);
    ct_drop(adj);
    ct_drop(beg);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_choice_all_prelude(const tree_t * ch) {
    if (!ch) return 0;
    if (ch->t == TT_CLAUSE) return ch->line == 0;
    if (ch->t != TT_CHOICE) return 0;
    int k = 0;
    for (int j = 0; j < ch->n; j++) if (ch->c[j] && ch->c[j]->t == TT_CLAUSE) { if (ch->c[j]->line != 0) return 0; k++; }
    return k > 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
stage2_t *lower_pl_stage2(const tree_t *prog) {
    prog = pl_chr_rewrite(prog);
    prog = pl_blockop_rewrite(prog);
    if (g_stage2.pl_dialect == PL_DIALECT_SWI) prog = pl_clauseq_rewrite(prog);
    prog = pl_ssu_rewrite(prog);
    prog = pl_det_rewrite(prog);
    prog = pl_dict_rewrite(prog);
    prog = pl_cutto_rewrite(prog);
    prog = pl_src_file_rewrite(prog);
    prog = pl_meta_rt_rewrite(prog);
    prog = pl_auto_table_rewrite(prog);
    prog = pl_table_rewrite(prog);
    prog = pl_scc_rewrite(prog);
    prog = pl_mq_rewrite(prog);
    for (int i = 0; i < prog->n; i++) {
        const tree_t * s = prog->c[i], * subj = (s && s->t == TT_STMT) ? lp_s_expr(s, ":subj") : NULL;
        if (pl_attv_is_key(subj, "$undo_run/0")) g_stage2.pl_undo_armed = 1;
    }
    prog = pl_attv_rewrite(prog);
    if (!g_stage2.depth_counted && pl_names_depth_limit(prog) && pl_prog_defines(prog, "$dl_findall/3")) g_stage2.depth_counted = 1;
    const tree_t * prog_src = prog;
    int load_h = pl_load_hook_line(prog, NULL);
    int load_cap = load_h > 0 ? pl_load_capacity(prog) : 1;
    pl_load_item_t load_it[load_cap];
    int load_n = 0;
    if (load_h > 0) prog = pl_load_defer(prog, load_it, &load_n);
    int _pl_bb0 = g_stage2.bbp.count;
    pl_register_program(&g_stage2, prog);
    int ndirs = 1, ndvn = 1;
    for (int i = 0; i < prog->n; i++) {
        const tree_t * s = prog->c[i];
        const tree_t * subj = (s && s->t == TT_STMT) ? lp_s_expr(s, ":subj") : NULL;
        if (subj && subj->t != TT_CHOICE && subj->t != TT_CLAUSE) { ndirs++; ndvn += pl_var_nodes(subj) + 2; }
    }
    const tree_t * init_goals[ndirs];
    int ninit = 0;
    const tree_t * main_goals[ndirs];
    int nmain = 0;
    const tree_t * dir_goals[ndirs];
    const tree_t * early_goals[ndirs];
    int nearly = 0;
    int ndir = 0;
    const char * dvn[ndvn];
    int dvc = 0;
    g_lower.pl.decl_dyn_n = 0;
    g_lower.pl.hook_dyn_n = 0;
    g_lower.pl.decl_other_n = 0;
    g_stage2.pl_decl_multifile.len = 0;
    g_stage2.pl_decl_meta.len = 0;
    for (int i = 0; i < prog->n; i++) {
        const tree_t *s = prog->c[i];
        if (!s || s->t != TT_STMT) continue;
        const tree_t *subj = lp_s_expr(s, ":subj");
        if (!subj || subj->t == TT_CHOICE || subj->t == TT_CLAUSE) continue;
        if (subj->t == TT_FNC && subj->v.sval && !strcmp(subj->v.sval, "initialization") && subj->n >= 1) {
            const tree_t *gt = subj->c[0];
            if (gt && ((gt->t == TT_QLIT || gt->t == TT_NAME || gt->t == TT_FNC) && gt->v.sval)) {
                int iv_scope0 = dvc;
                if (subj->n == 1) { init_goals[ninit++] = pl_dir_catch_wrap(pl_attv_dir((tree_t *) gt), dvn, &dvc); continue; }
                pl_dir_number_vars((tree_t *) gt, dvn, &dvc, iv_scope0);
                if (subj->c[1] && (subj->c[1]->t == TT_QLIT || subj->c[1]->t == TT_NAME) && subj->c[1]->v.sval && !strcmp(subj->c[1]->v.sval, "main")) {
                    nmain = 0;
                    main_goals[nmain++] = pl_cc_fnc2(";", pl_cc_fnc2("->", pl_attv_dir((tree_t *) gt), (tree_t *) pl_atom_goal("true")), pl_cc_fnc1("halt", pl_cc_ilit(1)));
                    continue;
                }
                init_goals[ninit++] = pl_attv_dir((tree_t *) gt);
                continue;
            }
        }
        if (subj->t == TT_FNC && subj->v.sval && !strcmp(subj->v.sval, "dynamic") && subj->n >= 1) {
            for (int k = 0; k < (subj->n == 2 ? 1 : subj->n); k++) {
                pl_decl_dynamic_record(&g_stage2, subj->c[k], (tree_t *) subj);
                pl_dyn_spec_file(subj->c[k], stmt_attr_str(stmt_attr_find(s, ":file")));
            }
            continue;
        }
        if (subj->t == TT_FNC && subj->v.sval && (!strcmp(subj->v.sval, "multifile") || !strcmp(subj->v.sval, "discontiguous")) && subj->n >= 1) {
            for (int k = 0; k < subj->n; k++) pl_decl_other_record(subj->c[k], !strcmp(subj->v.sval, "multifile"));
            continue;
        }
        if (subj->t == TT_FNC && subj->v.sval && (!strcmp(subj->v.sval, "meta_predicate") || !strcmp(subj->v.sval, "$meta_spec")) && subj->n >= 1) {
            for (int k = 0; k < subj->n; k++) pl_decl_meta_record(subj->c[k]);
            continue;
        }
        if (subj->t == TT_FNC && subj->v.sval && (!strcmp(subj->v.sval, "public") || !strcmp(subj->v.sval, "det")) && subj->n == 1 && !pl_file_defines(subj->v.sval, 1)) {
            init_goals[ninit++] = pl_dir_catch_wrap((tree_t *) subj, dvn, &dvc);
            continue;
        }
        if (subj->t == TT_FNC && subj->v.sval && pl_name_in(subj->v.sval, pl_decl_directives) && !pl_file_defines(subj->v.sval, subj->n)) continue;
        if (subj->t == TT_FNC && subj->v.sval && !strcmp(subj->v.sval, "op") && subj->n == 3) { init_goals[ninit++] = pl_dir_catch_wrap((tree_t *) subj, dvn, &dvc); continue; }
        {
            tree_t * dirgoal = (tree_t *) subj;
            if (subj->t == TT_FNC && subj->v.sval && !strcmp(subj->v.sval, "set_prolog_flag") && subj->n == 2) {
                if (pl_flag_directive_is_advisory(subj)) continue;
                dirgoal = pl_cc_fnc2("$set_prolog_flag_declare", subj->c[0], subj->c[1]);
                if (subj->c[0] && (subj->c[0]->t == TT_QLIT || subj->c[0]->t == TT_NAME) && subj->c[0]->v.sval && !strcmp(subj->c[0]->v.sval, "$dialect")) early_goals[nearly++] =
                    pl_dir_catch_wrap(dirgoal, dvn, &dvc);
            }
            dir_goals[ndir++] = pl_dir_catch_wrap(pl_attv_dir(dirgoal), dvn, &dvc);
            continue;
        }
    }
    IR_graph_t * top;
    {
        const tree_t * all_goals[ndir + nearly + ninit + nmain + 3 + (load_n + PL_SEED_CHUNK - 1) / PL_SEED_CHUNK + g_lower.pl.hook_dyn_n];
        int nall = 0;
        int nseedv = 1;
        { tree_t * dt = ast_node_new(TT_FNC); dt->v.sval = (char *) "$db_decls"; ast_push(dt, pl_cc_ilit(0)); all_goals[nall++] = dt; }
        for (int i = 0; i < nearly; i++) all_goals[nall++] = early_goals[i];
        for (int i = 0; i < g_lower.pl.hook_dyn_n; i++) {
            char hk[fmt_len("%s/%d", g_lower.pl.hook_dyn_name[i], g_lower.pl.hook_dyn_arity[i])];
            snprintf(hk, sizeof hk, "%s/%d", g_lower.pl.hook_dyn_name[i], g_lower.pl.hook_dyn_arity[i]);
            all_goals[nall++] = pl_cc_fnc2("$set_prolog_flag_declare", (tree_t *) pl_atom_goal("$tbl_hook"), (tree_t *) pl_atom_goal(lp_strdup(hk)));
        }
        for (int di = 0; di < g_stage2.pl_dyn_n; di++) {
            const char * dn = g_stage2.pl_dyn_name[di];
            int da = g_stage2.pl_dyn_arity[di];
            if (!dn || da < 0 || pl_name_is_scrip_own(dn, da) || !pl_file_defines(dn, da)) continue;
            {
                char key[fmt_len("%s/%d", dn, da)];
                snprintf(key, sizeof key, "%s/%d", dn, da);
                const tree_t * ch = resolve_pred_table_lookup(&g_stage2.resolve_pred_table, key);
                nseedv += ch ? ((ch->t == TT_CHOICE) ? ch->n : 1) : 0;
            }
        }
        {
            const tree_t * seeds[nseedv];
            int nseed = 0;
            int save_base = g_lower.pl.seed_var_base;
            g_lower.pl.seed_var_base = 0;
            for (int di = 0; di < g_stage2.pl_dyn_n; di++) {
                const char * dn = g_stage2.pl_dyn_name[di];
                int da = g_stage2.pl_dyn_arity[di];
                if (!dn || da < 0 || pl_name_is_scrip_own(dn, da)) continue;
                if (pl_file_defines(dn, da)) {
                    char key[fmt_len("%s/%d", dn, da)];
                    snprintf(key, sizeof key, "%s/%d", dn, da);
                    const tree_t * ch = resolve_pred_table_lookup(&g_stage2.resolve_pred_table, key);
                    int n = ch ? ((ch->t == TT_CHOICE) ? ch->n : 1) : 0;
                    for (int i = 0; i < n; i++) {
                        const tree_t * cl = (ch->t == TT_CHOICE) ? ch->c[i] : ch;
                        seeds[nseed++] =
                            pl_cc_fnc5("$db_seed_once", pl_cc_ilit(di), pl_cc_ilit(i), pl_static_clause_term(cl, dn, da, 0), pl_seed_file_term(pl_pred_file(dn, da)), pl_seed_line_term(cl));
                    }
                }
            }
            g_lower.pl.seed_var_base = save_base;
            if (nseed > 0) {
                IR_graph_t * sg = pl_body_graph(seeds, nseed);
                int bb_idx = bb_program_add(&g_stage2.bbp, sg);
                if (bb_idx >= 0) { pl_bb_register("$db_seed/0", 0, bb_idx); pl_new_proc("$db_seed/0", 0, bb_idx); all_goals[nall++] = pl_atom_goal("$db_seed"); }
            }
        }
        {
            cv_t sk = { 0 };
            cv_t sp = { 0 };
            int sall = 0;
            for (int i = 0; i < prog->n; i++) { const tree_t * st = prog->c[i]; if (st && st->t == TT_STMT) pl_clause_term_walk(lp_s_expr(st, ":subj"), &sk, &sall); }
            if (sall) {
                for (int bi = 0; bi < STAGE2_PL_PRED_TABLE_SIZE; bi++) for (Resolve_PredEntry * pe = g_stage2.resolve_pred_table.buckets[bi]; pe; pe = pe->next) if (pe->key)
                    pl_static_seed_pred(pe->key, pe->choice, &sp);
            } else for (uint32_t i = 0; i < sk.len; i++) {
                const char * kk = CV_AT(sk, const char *, i);
                pl_static_seed_pred(kk, resolve_pred_table_lookup(&g_stage2.resolve_pred_table, kk), &sp);
            }
            if (sp.len > 0) {
                const tree_t ** dg = (const tree_t **) ct_zalloc(sp.len, sizeof(const tree_t *));
                for (uint32_t i = 0; i < sp.len; i++) dg[i] = pl_atom_goal(CV_AT(sp, const char *, i));
                {
                    IR_graph_t * g = pl_body_graph(dg, (int) sp.len);
                    int bb_idx = bb_program_add(&g_stage2.bbp, g);
                    if (bb_idx >= 0) { pl_bb_register("$db_seedS/0", 0, bb_idx); pl_new_proc("$db_seedS/0", 0, bb_idx); all_goals[nall++] = pl_atom_goal("$db_seedS"); }
                }
                ct_drop(dg);
            }
            ct_drop(sk.p);
            ct_drop(sp.p);
        }
        for (int i = 0; i < ndir; i++) all_goals[nall++] = dir_goals[i];
        if (load_n > 0) {
            cv_t lp = { 0 };
            pl_load_procs(prog_src, load_it, load_n, &lp);
            for (uint32_t i = 0; i < lp.len; i++) all_goals[nall++] = pl_atom_goal(CV_AT(lp, const char *, i));
            ct_drop(lp.p);
        }
        for (int i = 0; i < ninit; i++) all_goals[nall++] = init_goals[i];
        for (int i = 0; i < nmain; i++) all_goals[nall++] = main_goals[i];
        top = pl_body_graph(all_goals, nall);
    }
    top->root_graph = 1;
    int top_idx = bb_program_add(&g_stage2.bbp, top);
    pl_new_proc("main", 0, top_idx);
    for (int bi = 0; bi < STAGE2_PL_PRED_TABLE_SIZE; bi++) {
        for (Resolve_PredEntry *pe = g_stage2.resolve_pred_table.buckets[bi]; pe; pe = pe->next) {
            if (!pe->key || !pe->choice) continue;
            const char *slash = strrchr(pe->key, '/');
            int ar = slash ? atoi(slash + 1) : 0;
            if (pl_bb_lookup(pe->key, ar)) continue;
            int pre = pl_choice_all_prelude(pe->choice);
            int bb_idx = lower_pl_pred_graph(pe->key, pe->choice);
            if (bb_idx < 0) continue;
            pl_bb_register(pe->key, ar, bb_idx);
            int pi = pl_new_proc(pe->key, ar, bb_idx);
            g_stage2.proc_table[pi].origin = pre;
        }
    }
    {
        extern tree_t * pl_runtime_clause_tree(tree_t *);
        {
            const char * fk = "$fc/3";
            if (!pl_bb_lookup(fk, 3) && !resolve_pred_table_lookup(&g_stage2.resolve_pred_table, fk) && !pl_synth_have(fk)) {
                tree_t * ch = ast_node_new(TT_CHOICE);
                ch->v.sval = ct_strdup(fk);
                int nb = 0;
                pl_bb_register(fk, 3, -1);
                for (int bi = 0; bi < 4; bi++) {
                    tree_t * hd = ast_node_new(TT_FNC);
                    tree_t * body;
                    tree_t * raw;
                    tree_t * cl;
                    hd->v.sval = ct_strdup("$fc");
                    if (bi == 0) {
                        ast_push(hd, pl_meta_var("G"));
                        ast_push(hd, pl_meta_var("_"));
                        ast_push(hd, pl_meta_var("_"));
                        body = pl_cc_fnc2(",", pl_cc_fnc2(",", pl_cc_fnc1("var", pl_meta_var("G")), (tree_t *) pl_atom_goal("!")), (tree_t *) pl_atom_goal("fail"));
                    } else if (bi == 1) {
                        ast_push(hd, pl_meta_var("G"));
                        ast_push(hd, (tree_t *) pl_atom_goal("true"));
                        ast_push(hd, (tree_t *) pl_atom_goal("true"));
                        body = pl_cc_fnc2(",", pl_cc_fnc2("==", pl_meta_var("G"), (tree_t *) pl_atom_goal("!")), (tree_t *) pl_atom_goal("!"));
                    } else if (bi == 2) {
                        ast_push(hd, pl_cc_fnc2(",", pl_meta_var("A"), pl_meta_var("B")));
                        ast_push(hd, pl_meta_var("Pre"));
                        ast_push(hd, pl_meta_var("Post"));
                        {
                            tree_t * s1 = pl_cc_fnc2(",", pl_cc_fnc3("$fc", pl_meta_var("A"), pl_meta_var("PA"), pl_meta_var("QA")), (tree_t *) pl_atom_goal("!"));
                            tree_t * s2 = pl_cc_fnc2(",", s1, pl_cc_fnc2("=", pl_meta_var("Pre"), pl_meta_var("PA")));
                            body = pl_cc_fnc2(",", s2, pl_cc_fnc2("=", pl_meta_var("Post"), pl_cc_fnc2(",", pl_meta_var("QA"), pl_meta_var("B"))));
                        }
                    } else {
                        ast_push(hd, pl_cc_fnc2(",", pl_meta_var("A"), pl_meta_var("B")));
                        ast_push(hd, pl_meta_var("Pre"));
                        ast_push(hd, pl_meta_var("Post"));
                        {
                            tree_t * s1 = pl_cc_fnc2(",", pl_cc_fnc3("$fc", pl_meta_var("B"), pl_meta_var("PB"), pl_meta_var("QB")), (tree_t *) pl_atom_goal("!"));
                            tree_t * s2 = pl_cc_fnc2(",", s1, pl_cc_fnc2("=", pl_meta_var("Pre"), pl_cc_fnc2(",", pl_meta_var("A"), pl_meta_var("PB"))));
                            body = pl_cc_fnc2(",", s2, pl_cc_fnc2("=", pl_meta_var("Post"), pl_meta_var("QB")));
                        }
                    }
                    raw = ast_node_new(TT_FNC);
                    raw->v.sval = (char *) ":-";
                    ast_push(raw, hd);
                    ast_push(raw, body);
                    cl = pl_runtime_clause_tree(raw);
                    if (!cl) continue;
                    ast_push(ch, cl);
                    nb++;
                }
                if (nb) { int bb_idx = lower_pl_pred_graph(fk, ch); if (bb_idx >= 0) { pl_bb_register(fk, 3, bb_idx); pl_new_proc(fk, 3, bb_idx); } }
            }
        }
        static const char * const pl_meta_ctrl[] = { ",", ";", "->", "*->" };
        for (size_t wi = 0; wi < sizeof pl_meta_ctrl / sizeof pl_meta_ctrl[0]; wi++) {
            const char * wn = pl_meta_ctrl[wi];
            char key[fmt_len("%s/2", wn)];
            int nclause;
            snprintf(key, sizeof key, "%s/2", wn);
            if (pl_bb_lookup(key, 2) || pl_synth_have(key)) continue;
            if (resolve_pred_table_lookup(&g_stage2.resolve_pred_table, key)) continue;
            nclause = !strcmp(wn, ";") ? 6 : !strcmp(wn, ",") ? 2 : 1;
            {
                tree_t * ch = ast_node_new(TT_CHOICE);
                ch->v.sval = ct_strdup(key);
                int nb = 0;
                int cc = !strcmp(wn, ",") ? 1 : 0;
                for (int bk = 0; bk <= nclause; bk++) {
                    tree_t * hd = ast_node_new(TT_FNC);
                    tree_t * body = (tree_t *) 0;
                    tree_t * raw;
                    tree_t * cl;
                    int bi = bk - (bk > cc);
                    hd->v.sval = ct_strdup(wn);
                    ast_push(hd, pl_meta_var("A"));
                    ast_push(hd, pl_meta_var("B"));
                    if (bk == cc) body =
                        pl_cc_fnc2(",", pl_cc_fnc2(",", pl_cc_fnc2("$cutcall", pl_cc_fnc2(wn, pl_meta_var("A"), pl_meta_var("B")), pl_meta_var("NG")), (tree_t *) pl_atom_goal("!")),
                        pl_cc_fnc1("call", pl_meta_var("NG")));
                    else if (!strcmp(wn, ",")) {
                        tree_t * f1 =
                            pl_cc_fnc2(",", pl_cc_fnc1("$shc", pl_cc_fnc2(",", pl_meta_var("A"), pl_meta_var("B"))),
                            pl_cc_fnc3("$fc", pl_cc_fnc2(",", pl_meta_var("A"), pl_meta_var("B")), pl_meta_var("Pre"), pl_meta_var("Post")));
                        tree_t * f2 = pl_cc_fnc2(",", pl_cc_fnc2(",", f1, (tree_t *) pl_atom_goal("!")), pl_cc_fnc2(",", pl_cc_fnc1("call", pl_meta_var("Pre")), (tree_t *) pl_atom_goal("!")));
                        body = bi == 0 ? pl_cc_fnc2(",", f2, pl_cc_fnc1("call", pl_meta_var("Post"))) :
                            pl_cc_fnc2(",", pl_cc_fnc1("call", pl_meta_var("A")), pl_cc_fnc1("$conj_nocut", pl_meta_var("B")));
                    } else if (!strcmp(wn, "->")) body = pl_cc_fnc2(",", pl_cc_fnc2(",", pl_cc_fnc1("call", pl_meta_var("A")), (tree_t *) pl_atom_goal("!")), pl_cc_fnc1("call", pl_meta_var("B")));
                    else if (!strcmp(wn, "*->")) body = pl_cc_fnc2(",", pl_cc_fnc1("call", pl_meta_var("A")), pl_cc_fnc1("call", pl_meta_var("B")));
                    else if (bi == 0) body =
                        pl_cc_fnc2(",", pl_cc_fnc2("=", pl_meta_var("A"), pl_cc_fnc2("->", pl_meta_var("C"), pl_meta_var("T"))),
                        pl_cc_fnc2(",", (tree_t *) pl_atom_goal("!"), pl_cc_ite(pl_cc_fnc1("call", pl_meta_var("C")), pl_cc_fnc1("call", pl_meta_var("T")), pl_cc_fnc1("call", pl_meta_var("B")))));
                    else if (bi == 1) body =
                        pl_cc_fnc2(",", pl_cc_fnc2("=", pl_meta_var("A"), pl_cc_fnc2("*->", pl_meta_var("C"), pl_meta_var("T"))),
                        pl_cc_fnc2(",", (tree_t *) pl_atom_goal("!"), pl_cc_scite(pl_cc_fnc1("call", pl_meta_var("C")), pl_cc_fnc1("call", pl_meta_var("T")), pl_cc_fnc1("call", pl_meta_var("B")))));
                    else if (bi == 2) {
                        tree_t * g1 = pl_cc_fnc2(",", pl_cc_fnc3("$fc", pl_meta_var("A"), pl_meta_var("Pre"), pl_meta_var("Post")), pl_cc_fnc1("call", pl_meta_var("Pre")));
                        body = pl_cc_fnc2(",", pl_cc_fnc2(",", g1, (tree_t *) pl_atom_goal("!")), pl_cc_fnc1("call", pl_meta_var("Post")));
                    } else if (bi == 3) body = pl_cc_fnc1("call", pl_meta_var("A"));
                    else if (bi == 4) {
                        tree_t * g1 = pl_cc_fnc2(",", pl_cc_fnc3("$fc", pl_meta_var("B"), pl_meta_var("Pre"), pl_meta_var("Post")), (tree_t *) pl_atom_goal("!"));
                        body = pl_cc_fnc2(",", pl_cc_fnc2(",", g1, pl_cc_fnc2(",", pl_cc_fnc1("call", pl_meta_var("Pre")), (tree_t *) pl_atom_goal("!"))), pl_cc_fnc1("call", pl_meta_var("Post")));
                    } else body = pl_cc_fnc1("call", pl_meta_var("B"));
                    raw = ast_node_new(TT_FNC);
                    raw->v.sval = (char *) ":-";
                    ast_push(raw, hd);
                    ast_push(raw, body);
                    cl = pl_runtime_clause_tree(raw);
                    if (!cl) continue;
                    ast_push(ch, cl);
                    nb++;
                }
                if (!nb) continue;
                { int bb_idx = lower_pl_pred_graph(key, ch); if (bb_idx < 0) continue; pl_bb_register(key, 2, bb_idx); pl_new_proc(key, 2, bb_idx); }
            }
        }
        {
            const char * pk = "print/1";
            if ((pl_file_defines("portray", 1) || pl_db_owned("portray", 1)) && !pl_bb_lookup(pk, 1) && !resolve_pred_table_lookup(&g_stage2.resolve_pred_table, pk) && !pl_decl_dyn_is("print", 1)) {
                tree_t * ch = ast_node_new(TT_CHOICE);
                ch->v.sval = ct_strdup(pk);
                tree_t * hd = ast_node_new(TT_FNC);
                hd->v.sval = ct_strdup("print");
                ast_push(hd, pl_meta_var("A"));
                tree_t * body = pl_cc_print_via_format((tree_t *) 0, pl_meta_var("A"));
                tree_t * raw = ast_node_new(TT_FNC);
                raw->v.sval = (char *) ":-";
                ast_push(raw, hd);
                ast_push(raw, body);
                tree_t * cl = pl_runtime_clause_tree(raw);
                if (cl) { ast_push(ch, cl); { int bb_idx = lower_pl_pred_graph(pk, ch); if (bb_idx >= 0) { pl_bb_register(pk, 1, bb_idx); pl_new_proc(pk, 1, bb_idx); } } }
            }
        }
    }
    {
        extern tree_t * pl_runtime_clause_tree(tree_t *);
        static const pl_det_leaf_t pl_meta_early[] = { { "write", 1, "$write" }, { "nl", 0, "$nl" }, { "true", 0, "$true" }, { "!", 0, "$true" }, { "fail", 0, "$fail" }, { "false", 0, "$fail" },
            { "throw", 1, "$throw" }, { "=", 2, "$unify" }, { "is", 2, "$is_v" }, { ">", 2, "$cmp_gt" }, { "assert", 1, "$db_assertz_t" }, { "asserta", 1, "$db_asserta_t" }, { "assertz", 1,
            "$db_assertz_t" }, { "retract", 1, "$db_erase_t" }, { "retractall", 1, "$db_retractall_t" }, { "abolish", 1, "$db_abolish_t" }, { "clause", 2, "$db_at_t" }, { "", 0, "" } };
        for (int tbl = 0; tbl < 2; tbl++) for (int li = 0; (tbl ? pl_meta_early[li].nm[0] : pl_det_leaves[li].nm[0]); li++) {
            const char * bn = tbl ? pl_meta_early[li].nm : pl_det_leaves[li].nm;
            int ba = tbl ? pl_meta_early[li].ar : pl_det_leaves[li].ar;
            if (!strcmp(bn, "halt") || bn[0] == '$' || pl_swi_absent_leaf(bn, ba)) continue;
            char key[fmt_len("%s/%d", bn, ba)];
            snprintf(key, sizeof key, "%s/%d", bn, ba);
            if (pl_bb_lookup(key, ba) || pl_synth_have(key)) continue;
            if (resolve_pred_table_lookup(&g_stage2.resolve_pred_table, key)) continue;
            {
                tree_t * hd;
                tree_t * body;
                tree_t * raw;
                tree_t * cl;
                tree_t * ch;
                static const char * const an[] = { "A", "B", "C", "D", "E" };
                if (ba > 5) continue;
                if (ba > 0) {
                    hd = ast_node_new(TT_FNC);
                    hd->v.sval = ct_strdup(bn);
                    body = ast_node_new(TT_FNC);
                    body->v.sval = ct_strdup(bn);
                    for (int k = 0; k < ba; k++) { ast_push(hd, pl_meta_var(an[k])); ast_push(body, pl_meta_var(an[k])); }
                } else {
                    hd = ast_node_new(TT_QLIT);
                    hd->v.sval = ct_strdup(bn);
                    body = ast_node_new(TT_QLIT);
                    body->v.sval = ct_strdup(bn);
                }
                raw = ast_node_new(TT_FNC);
                raw->v.sval = (char *) ":-";
                ast_push(raw, hd);
                ast_push(raw, body);
                cl = pl_runtime_clause_tree(raw);
                if (!cl) continue;
                ch = ast_node_new(TT_CHOICE);
                ch->v.sval = ct_strdup(key);
                ast_push(ch, cl);
                { int bb_idx = lower_pl_pred_graph(key, ch); if (bb_idx < 0) continue; pl_bb_register(key, ba, bb_idx); pl_new_proc(key, ba, bb_idx); }
            }
        }
    }
    for (int di = 0; di < g_stage2.pl_dyn_n; di++) {
        const char * dn = g_stage2.pl_dyn_name[di];
        int da = g_stage2.pl_dyn_arity[di];
        if (!dn || da < 0 || pl_name_is_scrip_own(dn, da)) continue;
        {
            char key[fmt_len("%s/%d", dn, da)];
            snprintf(key, sizeof key, "%s/%d", dn, da);
            if (pl_bb_lookup(key, da)) continue;
            {
                extern tree_t * pl_runtime_clause_tree(tree_t *);
                tree_t * hd;
                tree_t * raw;
                tree_t * cl;
                tree_t * ch;
                if (da > 0) {
                    hd = ast_node_new(TT_FNC);
                    hd->v.sval = ct_strdup(dn);
                    for (int i = 0; i < da; i++) { tree_t * v = ast_node_new(TT_VAR); v->v.sval = (char *) "_"; ast_push(hd, v); }
                } else {
                    hd = ast_node_new(TT_QLIT);
                    hd->v.sval = ct_strdup(dn);
                }
                { tree_t * fb = ast_node_new(TT_QLIT); fb->v.sval = (char *) "fail"; raw = ast_node_new(TT_FNC); raw->v.sval = (char *) ":-"; ast_push(raw, hd); ast_push(raw, fb); }
                cl = pl_runtime_clause_tree(raw);
                if (!cl) continue;
                ch = ast_node_new(TT_CHOICE);
                ch->v.sval = ct_strdup(key);
                ast_push(ch, cl);
                { int bb_idx = lower_pl_pred_graph(key, ch); if (bb_idx < 0) continue; pl_bb_register(key, da, bb_idx); pl_new_proc(key, da, bb_idx); }
            }
        }
    }
    for (int _gi = _pl_bb0; _gi < g_stage2.bbp.count; _gi++) if (g_stage2.bbp.table[_gi]) {
        g_stage2.bbp.table[_gi]->resumable_callable = 1;
        g_stage2.bbp.table[_gi]->unbound_serial = 1;
        g_stage2.bbp.table[_gi]->depth_counted = g_stage2.depth_counted;
    }
    for (int i = 0; i < g_stage2.proc_count; i++) {
        const char * pk = g_stage2.proc_table[i].name;
        int bi = g_stage2.proc_table[i].bb_idx;
        const char * sl = pk ? strrchr(pk, '/') : (const char *)0;
        if (!sl || bi < _pl_bb0 || bi >= g_stage2.bbp.count || !g_stage2.bbp.table[bi]) continue;
        {
            char nm[sl - pk + 1];
            memcpy(nm, pk, (size_t)(sl - pk));
            nm[sl - pk] = 0;
            int ar = atoi(sl + 1);
            if (pl_dyn_index(nm, ar) >= 0 || pl_decl_dyn_is(nm, ar)) g_stage2.bbp.table[bi]->rt_redefinable = 1;
        }
    }
    pl_poll_alpha_mark(_pl_bb0);
    { extern int rt_pl_db_frame_cells(void); top->standing_cells = rt_pl_db_frame_cells() + 1; }
    pl_db_decls_capture();
    return &g_stage2;
}
