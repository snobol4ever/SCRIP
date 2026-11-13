#include "lower/lower_snobol4/lsn_head.h"
#include "runtime/core/g_runtime.h"
#include <stdlib.h>
#include "ir/ir_graph.h"
#include "ir/stage2.h"
#include "lower/lower_snobol4.h"
#include "parsers/snobol4/scrip_cc.h"
#include "lower/lower_common.h"
#include "ir/dtp.h"
#include "runtime/snobol4_system_fns.h"
#include "lower/lower_snobol4/lsn_01_sno.h"
#include "lower/lower_snobol4/lsn_02_sno.h"
#include "lower/lower_snobol4/lsn_03_sno.h"
#include "lower/lower_snobol4/lsn_04_sno.h"
#include "lower/lower_snobol4/lsn_05_sno.h"
#include "lower/lower_snobol4/lsn_06_sh.h"
#include "lower/lower_snobol4/lsn_07_sno.h"
extern int stage2_proc_grow(stage2_t * s2);
static void sno_entry_seen_push(cv_t * seen, const char * el);
extern int snobol4_noexec(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_setexit_on(void) { const char * e = getenv("SCRIP_SETEXIT"); return (e && e[0] == '0') ? 0 : 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sno_define_entry_computed(const tree_t * t, int argbase) {
    const tree_t * en = (t && t->n > argbase + 1) ? t->c[argbase + 1] : NULL;
    return en && en->t != TT_QLIT && !(en->t == TT_NAME && en->n > 0 && en->c[0] && en->c[0]->t == TT_VAR);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sno_is_xname(const tree_t * t) { return t && t->t == TT_NAME && t->n > 0 && t->c[0] && t->c[0]->t == TT_DEFER && !(t->c[0]->n > 0 && t->c[0]->c[0] && t->c[0]->c[0]->t == TT_VAR); }
static void sno_scan_xname_use(const tree_t * t) {
    if (!t || g_lower.sno.uses_xname) return;
    if (sno_is_xname(t)) { g_lower.sno.uses_xname = 1; return; }
    if (t->t == TT_FNC) {
        const char * fn = t->v.sval;
        if (!fn && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR) fn = t->c[0]->v.sval;
        if (fn && (!strcmp(fn, "EVAL") || !strcmp(fn, "CODE"))) { g_lower.sno.uses_xname = 1; return; }
    }
    for (int i = 0; i < t->n; i++) sno_scan_xname_use(t->c[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_scan_code_use(const tree_t * t) {
    if (!t || g_lower.sno.uses_code) return;
    if (t->t == TT_DEFINE) { const tree_t * pr = t->n > 1 ? t->c[1] : NULL; if (!pr || pr->t != TT_QLIT || sno_define_entry_computed(t, 1)) { g_lower.sno.uses_code = 1; return; } }
    if (t->t == TT_FNC) {
        const char * fn = t->v.sval;
        if (!fn && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR) fn = t->c[0]->v.sval;
        if (fn && !strcmp(fn, "CODE")) { g_lower.sno.uses_code = 1; g_lower.sno.calls_code = 1; return; }
        if (fn && sno_setexit_on() && !strcmp(fn, "SETEXIT")) { g_lower.sno.uses_code = 1; return; }
        if (fn && !strcmp(fn, "DEFINE")) {
            int ab = t->v.sval ? 0 : 1;
            if (t->n <= ab || !t->c[ab] || t->c[ab]->t != TT_QLIT) { g_lower.sno.uses_code = 1; return; }
            if (sno_define_entry_computed(t, ab)) { g_lower.sno.uses_code = 1; return; }
        }
        if (fn && (!strcmp(fn, "OPSYN") || !strcmp(fn, "APPLY"))) {
            int ab = t->v.sval ? 0 : 1, k = ab + (fn[0] == 'O' ? 1 : 0);
            const tree_t * src = t->n > k ? t->c[k] : NULL;
            if (src && (fn[0] == 'O' ? src->t != TT_QLIT : 0)) { g_lower.sno.uses_code = 1; return; }
            if (src && src->t == TT_QLIT && src->v.sval && !strcmp(src->v.sval, "DEFINE")) { g_lower.sno.uses_code = 1; return; }
        }
    }
    if ((t->t == TT_GOTO_U || t->t == TT_GOTO_S || t->t == TT_GOTO_F) && t->n > 0 && t->c[0]) {
        const tree_t * g0 = t->c[0];
        if (g0->t != TT_QLIT || (g0->v.sval && g0->v.sval[0] == '$')) { g_lower.sno.uses_code = 1; return; }
    }
    for (int i = 0; i < t->n; i++) sno_scan_code_use(t->c[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sno_expr_eq(const tree_t * a, const tree_t * b) {
    if (a == b) return 1;
    if (!a || !b) return 0;
    if (a->t != b->t || a->n != b->n) return 0;
    if (a->v.ival != b->v.ival) return 0;
    for (int i = 0; i < a->n; i++) if (!sno_expr_eq(a->c[i], b->c[i])) return 0;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sno_expr_is_thunk(const char * nm) { for (int i = 0; i < (int) g_lower.sno.exprs.len; i++) if (!strcmp(CV_AT(g_lower.sno.exprs, sno_expr_ent_t, i).name, nm)) return 1; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * sno_expr_collect(const tree_t * expr) {
    if (!expr) sno_fatal("unevaluated-expression operator (*) with no operand", NULL);
    for (int i = 0; i < (int) g_lower.sno.exprs.len; i++)
        if (CV_AT(g_lower.sno.exprs, sno_expr_ent_t, i).salt == g_lower.sno.expr_salt && strncmp(CV_AT(g_lower.sno.exprs, sno_expr_ent_t, i).name, "EXPRNM$", 7) &&
        sno_expr_eq(CV_AT(g_lower.sno.exprs, sno_expr_ent_t, i).expr, expr)) return CV_AT(g_lower.sno.exprs, sno_expr_ent_t, i).name;
    const char * xv = (expr->t == TT_VAR && expr->v.sval && expr->v.sval[0] && !strchr(expr->v.sval, '$') && strlen(expr->v.sval) < 100) ? expr->v.sval : "";
    char buf[fmt_len("EXPR$%dF%d$%s", (int) g_lower.sno.exprs.len, g_lower.sno.expr_salt, xv)];
    if (g_lower.sno.expr_salt) snprintf(buf, sizeof buf, "EXPR$%dF%d", (int) g_lower.sno.exprs.len, g_lower.sno.expr_salt);
    else snprintf(buf, sizeof buf, "EXPR$%d", (int) g_lower.sno.exprs.len);
    if (*xv) { size_t bl = strlen(buf); snprintf(buf + bl, sizeof buf - bl, "$%s", xv); }
    {
        sno_expr_ent_t x;
        x.name = lp_strdup(buf);
        x.expr = expr;
        x.salt = g_lower.sno.expr_salt;
        x.want_name = 0;
        x.nm = 0;
        x.svar = *xv ? lp_strdup(xv) : (const char *)0;
        CV_PUSH(g_lower.sno.exprs, sno_expr_ent_t) = x;
        return x.name;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * sno_expr_collect_nm(const tree_t * expr) {
    if (!expr) sno_fatal("unevaluated-expression operator (*) with no operand", NULL);
    for (int i = 0; i < (int) g_lower.sno.exprs.len; i++)
        if (CV_AT(g_lower.sno.exprs, sno_expr_ent_t, i).salt == g_lower.sno.expr_salt && CV_AT(g_lower.sno.exprs, sno_expr_ent_t, i).want_name &&
        sno_expr_eq(CV_AT(g_lower.sno.exprs, sno_expr_ent_t, i).expr, expr)) return CV_AT(g_lower.sno.exprs, sno_expr_ent_t, i).name;
    char buf[40];
    if (g_lower.sno.expr_salt) snprintf(buf, sizeof buf, "EXPRNM$%dF%d", (int) g_lower.sno.exprs.len, g_lower.sno.expr_salt);
    else snprintf(buf, sizeof buf, "EXPRNM$%d", (int) g_lower.sno.exprs.len);
    {
        sno_expr_ent_t x;
        x.name = lp_strdup(buf);
        x.expr = expr;
        x.salt = g_lower.sno.expr_salt;
        x.want_name = 1;
        x.nm = 1;
        x.svar = (const char *)0;
        CV_PUSH(g_lower.sno.exprs, sno_expr_ent_t) = x;
        return x.name;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * sno_expr_collect_wn(const tree_t * expr) {
    const char * nm = sno_expr_collect(expr);
    for (int i = 0; i < (int) g_lower.sno.exprs.len; i++) if (CV_AT(g_lower.sno.exprs, sno_expr_ent_t, i).name == nm) { CV_AT(g_lower.sno.exprs, sno_expr_ent_t, i).want_name = 1; break; }
    return nm;
}
int sno_def_entry_absent(const tree_t * subj, int argbase) {
    const char * entry_opt = NULL;
    sno_def_t d;
    if (!subj || subj->n <= argbase || !subj->c[argbase] || !sno_qlit_fold(subj->c[argbase])) return 0;
    if (subj->n > argbase + 1 && subj->c[argbase + 1]) {
        const tree_t * ea = subj->c[argbase + 1];
        if (ea->t == TT_QLIT && ea->v.sval) entry_opt = ea->v.sval;
        else if (ea->t == TT_NAME && ea->n > 0 && ea->c[0] && ea->c[0]->t == TT_VAR && ea->c[0]->v.sval) entry_opt = ea->c[0]->v.sval;
        else return 0;
    }
    sno_parse_define(sno_qlit_fold(subj->c[argbase]), entry_opt, &d);
    if (sn4_sysfn_protected(d.fname)) return 1;
    if (!g_lower.sno.uses_code) return 0;
    { const char * el = (d.entry && d.entry[0]) ? d.entry : d.fname; return (el && el[0] && !bb_label_landing(el)) ? 1 : 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * sno_define_entry_opt(const tree_t * dsub, int argbase) {
    if (!dsub || dsub->n <= argbase + 1 || !dsub->c[argbase + 1]) return NULL;
    const tree_t * ea = dsub->c[argbase + 1];
    if (ea->t == TT_QLIT && ea->v.sval) return ea->v.sval;
    if (ea->t == TT_NAME && ea->n > 0 && ea->c[0] && ea->c[0]->t == TT_VAR && ea->c[0]->v.sval) return ea->c[0]->v.sval;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sno_fname_is_multiproto(const char * fname) { for (uint32_t k = 0; k < g_lower.sno.multiproto.len; k++) if (!strcmp(CV_AT(g_lower.sno.multiproto, const char *, k), fname)) return 1; return 0; }
static const char * sno_proto_encode(const sno_def_t * d) {
    size_t need = (size_t) fmt_len("%d|", d->nformals);
    for (int k = 0; k < d->nnames; k++) need += (size_t) fmt_len("%s%s", k ? "," : "", d->names[k]) - 1;
    char e[need];
    int n = snprintf(e, need, "%d|", d->nformals);
    for (int k = 0; k < d->nnames && (size_t)n < need - 1; k++) n += snprintf(e + n, need - (size_t)n, "%s%s", k ? "," : "", d->names[k]);
    return lp_strdup(e);
}
static void sno_proto_note(const sno_def_t * d) {
    const char * enc = sno_proto_encode(d);
    for (uint32_t k = 0; k < g_lower.sno.proto_fn.len; k++) if (!strcmp(CV_AT(g_lower.sno.proto_fn, const char *, k), d->fname)) {
        if (strcmp(CV_AT(g_lower.sno.proto_enc, const char *, k), enc) && !sno_fname_is_multiproto(d->fname)) CV_PUSH(g_lower.sno.multiproto, const char *) = lp_strdup(d->fname);
        CV_AT(g_lower.sno.proto_enc, const char *, k) = enc;
        return;
    }
    { const char * fn = lp_strdup(d->fname); CV_PUSH(g_lower.sno.proto_fn, const char *) = fn; CV_PUSH(g_lower.sno.proto_enc, const char *) = enc; }
}
void sno_bind_attach_proto(IR_graph_t * g, IR_t * bind, const sno_def_t * d, IR_t * fail) {
    if (!sno_fname_is_multiproto(d->fname)) return;
    IR_t * pr = lc_build(g, IR_LIT_STRING, bind, fail);
    IR_LIT(pr).sval = (char *) sno_proto_encode(d);
    ir_operand_push(bind, pr);
}
void sno_bind_attach_entry(IR_graph_t * g, IR_t * bind, const char * entry, IR_t * fail) {
    if (!g || !bind || !entry || !entry[0]) return;
    IR_t * en = lc_build(g, IR_LIT_NAME, bind, fail);
    IR_LIT(en).sval = lp_strdup(entry);
    ir_operand_push(bind, en);
    bind->pat_static = 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void sno_parse_define(const char * spec, const char * entry_opt, sno_def_t * d) {
    char buf[strlen(spec) + 1];
    int bn = 0;
    for (const char * p = spec; *p; p++) if (*p != ' ' && *p != '\t') buf[bn++] = *p;
    buf[bn] = 0;
    char * par = strchr(buf, '(');
    char * cls = par ? strchr(par, ')') : NULL;
    if (!par || !cls) sno_fatal("DEFINE prototype missing parameter parentheses", spec);
    *par = 0;
    *cls = 0;
    if (!buf[0]) sno_fatal("DEFINE prototype missing function name", spec);
    d->fname = lp_strdup(buf);
    d->entry = (entry_opt && entry_opt[0]) ? lp_strdup(entry_opt) : d->fname;
    d->result_name = NULL;
    d->nnames = 0;
    {
        int cap = 2;
        for (const char * q = par + 1; q < cls; q++) cap += (*q == ',');
        for (const char * q = cls + 1; *q; q++) cap += (*q == ',');
        d->names = (const char **) ct_zalloc((size_t) cap, sizeof(const char *));
    }
    for (char * seg = par + 1; seg && *seg; ) { char * cm = strchr(seg, ','); if (cm) *cm = 0; if (*seg) d->names[d->nnames++] = lp_strdup(seg); seg = cm ? cm + 1 : NULL; }
    d->nformals = d->nnames;
    for (char * seg = cls + 1; seg && *seg; ) { char * cm = strchr(seg, ','); if (cm) *cm = 0; if (*seg) d->names[d->nnames++] = lp_strdup(seg); seg = cm ? cm + 1 : NULL; }
    sno_reg_var(d->fname);
    for (int k = 0; k < d->nnames; k++) sno_reg_var(d->names[k]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_exprdef_note(cv_t * v, const char * f) { if (!f) return; for (uint32_t k = 0; k < v->len; k++) if (!strcmp(CV_AT(*v, const char *, k), f)) return; CV_PUSH(*v, const char *) = f; }
static int sno_exprdef_seen(const cv_t * v, const char * f) { if (!f) return 0; for (uint32_t k = 0; k < v->len; k++) if (!strcmp(CV_AT(*v, const char *, k), f)) return 1; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_register_program(stage2_t * s2, const tree_t * prog) {
    extern int polyglot_module_open(stage2_t * s2, const tree_t * s);
    extern void polyglot_module_extend(stage2_t * s2, int mod_idx, const tree_t * s);
    int mod_idx = -1;
    for (int _ci = 0; _ci < prog->n; _ci++) {
        const tree_t * s = prog->c[_ci];
        if (!s || (s->t != TT_STMT && s->t != TT_END)) continue;
        if (mod_idx < 0) mod_idx = polyglot_module_open(s2, s);
        polyglot_module_extend(s2, mod_idx, s);
        const tree_t * subj = stmt_attr_expr(stmt_attr_find(s, ":subj"));
        if (!subj) continue;
        const char * lbl = stmt_attr_str(stmt_attr_find(s, ":lbl"));
        if (mod_idx >= 0 && lbl && *lbl) s2->module_registry.mods[mod_idx].core_label_count++;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * sno_litname(const tree_t * a) {
    if (!a) return NULL;
    if (a->t == TT_QLIT && a->v.sval) return a->v.sval;
    if (a->t == TT_NAME && a->n > 0 && a->c[0] && a->c[0]->t == TT_VAR && a->c[0]->v.sval) return a->c[0]->v.sval;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_def_put(cv_t * defs, cv_t * bodies, const sno_def_t * d, const tree_t * body) { sno_def_t c = *d; CV_PUSH(*defs, sno_def_t) = c; CV_PUSH(*bodies, const tree_t *) = body; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_prescan_expr(const tree_t * t, cv_t * defs, cv_t * bodies, cv_t * exprdef_names, cv_t * entries) {
    if (!t) return;
    if (t->t == TT_FNC) {
        const char * name = t->v.sval;
        int argbase = 0;
        if (!name && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR) { name = t->c[0]->v.sval; argbase = 1; }
        if (name && !strcmp(name, "DATA") && t->n > argbase && t->c[argbase] && t->c[argbase]->t == TT_QLIT && t->c[argbase]->v.sval) {
            extern void * dat_register(const char * spec);
            extern void * dat_find_type(const char * name);
            extern void dat_set_live(const char * name, int live);
            extern void dat_set_nohook(const char * name);
            extern int dat_spec_is_current(const char * spec);
            const char * sp = t->c[argbase]->v.sval;
            char nb[strlen(sp) + 1];
            int k = 0;
            for (; sp[k] && sp[k] != '('; k++) nb[k] = sp[k];
            nb[k] = 0;
            int prot = (nb[0] && sn4_sysfn_protected(nb)) ? 1 : 0;
            if (!prot && sp[k] == '(') {
                const char * fs = sp + k + 1;
                const char * fe = strchr(fs, ')');
                if (!fe) fe = fs + strlen(fs);
                while (fs < fe && !prot) {
                    const char * cm = fs;
                    while (cm < fe && *cm != ',') cm++;
                    const char * b = fs;
                    while (b < cm && (*b == ' ' || *b == '\t')) b++;
                    const char * e2 = cm;
                    while (e2 > b && (e2[-1] == ' ' || e2[-1] == '\t')) e2--;
                    size_t fl = (size_t)(e2 - b);
                    if (fl > 0) { char fb[fl + 1]; memcpy(fb, b, fl); fb[fl] = 0; if (sn4_sysfn_protected(fb)) prot = 1; }
                    fs = (cm < fe) ? cm + 1 : fe;
                }
            }
            if (nb[0] && !prot && !dat_spec_is_current(sp)) { dat_register(sp); dat_set_live(nb, 0); dat_set_nohook(nb); }
        }
        if (name && !strcmp(name, "OPSYN") && t->n - argbase == 2) {
            const char * an = sno_litname(t->c[argbase]);
            const char * on = sno_litname(t->c[argbase + 1]);
            if (an && on && !sn4_sysfn_protected(an)) {
                int fo = -1, fa = -1;
                for (uint32_t k = 0; k < defs->len; k++) if (!strcmp(CV_AT(*defs, sno_def_t, k).fname, on)) { fo = (int) k; break; }
                for (uint32_t k = 0; k < defs->len; k++) if (!strcmp(CV_AT(*defs, sno_def_t, k).fname, an)) { fa = (int) k; break; }
                if (fa >= 0 && !CV_AT(*defs, sno_def_t, fa).result_name) fo = -2;
                if (fo >= 0) {
                    sno_def_t d = CV_AT(*defs, sno_def_t, fo);
                    d.result_name = d.result_name ? d.result_name : d.fname;
                    d.fname = lp_strdup(an);
                    sno_reg_var(d.fname);
                    sno_def_put(defs, bodies, &d, NULL);
                } else if (fo == -1) {
                    extern void rt_builtin_synonym_add(const char *, const char *);
                    rt_builtin_synonym_add(lp_strdup(an), lp_strdup(on));
                }
            }
        }
        if (name && !strcmp(name, "DEFINE") && t->n > argbase && t->c[argbase] && t->c[argbase]->t == TT_QLIT && t->c[argbase]->v.sval && !sno_define_entry_computed(t, argbase)) {
            const char * entry_opt = NULL;
            if (t->n > argbase + 1 && t->c[argbase + 1]) {
                const tree_t * ea = t->c[argbase + 1];
                if (ea->t == TT_QLIT && ea->v.sval) entry_opt = ea->v.sval;
                else if (ea->t == TT_NAME && ea->n > 0 && ea->c[0] && ea->c[0]->t == TT_VAR && ea->c[0]->v.sval) entry_opt = ea->c[0]->v.sval;
            }
            sno_def_t d;
            sno_parse_define(t->c[argbase]->v.sval, entry_opt, &d);
            if (sn4_sysfn_protected(d.fname)) return;
            sno_proto_note(&d);
            sno_entry_seen_push(entries, d.entry);
            if (t != g_lower.sno.prescan_top) { g_lower.sno.expr_define_seen = 1; sno_exprdef_note(exprdef_names, d.fname); }
            sno_predef_note(d.fname);
            int fo = -1;
            for (uint32_t k = 0; k < defs->len; k++) if (!strcmp(CV_AT(*defs, sno_def_t, k).fname, d.fname)) { fo = (int) k; break; }
            if (fo >= 0) CV_AT(*defs, sno_def_t, fo) = d;
            else sno_def_put(defs, bodies, &d, NULL);
        }
    }
    for (int i = 0; i < t->n; i++) sno_prescan_expr(t->c[i], defs, bodies, exprdef_names, entries);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_prescan_repl_defines(const tree_t * t, cv_t * defs, cv_t * bodies, cv_t * exprdef_names, cv_t * entries) {
    if (!t) return;
    if (t->t == TT_FNC) {
        const char * name = t->v.sval;
        int argbase = 0;
        if (!name && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR) { name = t->c[0]->v.sval; argbase = 1; }
        (void) argbase;
        if (name && !strcmp(name, "DEFINE")) {
            const tree_t * top = g_lower.sno.prescan_top;
            g_lower.sno.prescan_top = NULL;
            sno_prescan_expr(t, defs, bodies, exprdef_names, entries);
            g_lower.sno.prescan_top = top;
            return;
        }
    }
    for (int i = 0; i < t->n; i++) sno_prescan_repl_defines(t->c[i], defs, bodies, exprdef_names, entries);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void sno_expr_thunks_build(int x0) {
    int sv = g_lower.sno.in_patproc;
    g_lower.sno.in_patproc = 1;
    for (int xi = x0; xi < (int) g_lower.sno.exprs.len; xi++) {
        {
            if (g_lower.sno.sno_expr_thunks_build_xd < 0) g_lower.sno.sno_expr_thunks_build_xd = getenv("SCRIP_EXPR_DBG") ? 1 : 0;
            if (g_lower.sno.sno_expr_thunks_build_xd)
                fprintf(stderr, "[EXPRDBG] %s want_name=%d expr.t=%d expr.sval=%.32s\n", CV_AT(g_lower.sno.exprs, sno_expr_ent_t, xi).name, CV_AT(g_lower.sno.exprs, sno_expr_ent_t, xi).want_name,
                CV_AT(g_lower.sno.exprs, sno_expr_ent_t, xi).expr ? (int)CV_AT(g_lower.sno.exprs, sno_expr_ent_t, xi).expr->t : -1,
                (CV_AT(g_lower.sno.exprs, sno_expr_ent_t, xi).expr && CV_AT(g_lower.sno.exprs, sno_expr_ent_t, xi).expr->v.sval) ? CV_AT(g_lower.sno.exprs, sno_expr_ent_t, xi).expr->v.sval : "?");
        }
        IR_graph_t * gx = IR_alloc(256);
        scx_t ex;
        ex.g = gx;
        ex.wn_tree = NULL;
        ex.loop_exit = NULL;
        ex.loop_next = NULL;
        ex.result_name = CV_AT(g_lower.sno.exprs, sno_expr_ent_t, xi).name;
        ex.pat_fail = NULL;
        ex.pat_seal = NULL;
        ex.npre = 0;
        ex.snap = 0;
        ex.capro = NULL;
        ex.capro_cand = NULL;
        ex.ppos = 0;
        ex.prog_nstmt = 0;
        ex.stno_base = 0;
        IR_t * ok = lc_build(gx, IR_SUCCEED, NULL, NULL);
        IR_t * no = lc_build(gx, IR_FAIL, NULL, NULL);
        IR_t * sJ = lc_build(gx, IR_GOTO, ok, NULL);
        IR_t * fJ = lc_build(gx, IR_GOTO, no, NULL);
        sno_reg_var(CV_AT(g_lower.sno.exprs, sno_expr_ent_t, xi).name);
        IR_t * asn = lc_build(gx, IR_ASSIGN, sJ, fJ);
        IR_LIT(asn).sval = (char *) CV_AT(g_lower.sno.exprs, sno_expr_ent_t, xi).name;
        const tree_t * xe = CV_AT(g_lower.sno.exprs, sno_expr_ent_t, xi).expr;
        if (CV_AT(g_lower.sno.exprs, sno_expr_ent_t, xi).want_name && xe && xe->t == TT_INDIRECT && xe->n > 0 && xe->c[0] && CV_AT(g_lower.sno.exprs, sno_expr_ent_t, xi).name &&
            !strncmp(CV_AT(g_lower.sno.exprs, sno_expr_ent_t, xi).name, "EXPRNM$", 7)) xe = xe->c[0];
        if (CV_AT(g_lower.sno.exprs, sno_expr_ent_t, xi).want_name && xe && (sno_item_argbase(xe) >= 0 || (xe->t == TT_IDX && xe->n >= 2))) {
            tree_t * nmn = ast_node_new(TT_NAME);
            ast_push(nmn, (tree_t *) xe);
            xe = nmn;
        }
        IR_t * vr = NULL;
        int xe_is_idx_wn = CV_AT(g_lower.sno.exprs, sno_expr_ent_t, xi).want_name && xe && xe->t == TT_IDX;
        IR_t * e = xe_is_idx_wn ? sx_idx_container(&ex, xe, fJ, &vr) : sx_lower(&ex, xe, asn, fJ, &vr);
        if (xe_is_idx_wn) lc_γ_to(vr, asn);
        ir_operand_push(asn, vr);
        if (CV_AT(g_lower.sno.exprs, sno_expr_ent_t, xi).want_name) {
            IR_t * wn_lit = lc_build(gx, IR_LIT_STRING, NULL, fJ);
            IR_LIT(wn_lit).sval = (char *) "";
            IR_t * wn_call = lc_build(gx, IR_CALL, NULL, fJ);
            IR_LIT(wn_call).sval = (char *) "SNO$WANTNM";
            lc_γ_to(wn_lit, wn_call);
            lc_γ_to(wn_call, e);
            ir_operand_push(wn_call, wn_lit);
            gx->entry = wn_lit;
        } else {
            gx->entry = e;
        }
        { IR_t * ad = lc_build(gx, IR_DEFINE_ADOPT, gx->entry, fJ); gx->entry = ad; }
        int xpi = stage2_proc_grow(&g_stage2);
        g_stage2.proc_table[xpi].name = CV_AT(g_lower.sno.exprs, sno_expr_ent_t, xi).name;
        g_stage2.proc_table[xpi].proc = NULL;
        g_stage2.proc_table[xpi].entry_pc = -1;
        g_stage2.proc_table[xpi].nparams = 0;
        g_stage2.proc_table[xpi].lower_sc.n = 0;
        g_stage2.proc_table[xpi].is_generator = 0;
        g_stage2.proc_table[xpi].dyn_scope = 1;
        g_stage2.proc_table[xpi].thunk_kind = PROC_THUNK_EXPR;
        g_stage2.proc_table[xpi].thunk_flags =
            SNO_DSTAR_THUNK | (CV_AT(g_lower.sno.exprs, sno_expr_ent_t, xi).nm ? SNO_DSTAR_EXPRNM : 0u) | (CV_AT(g_lower.sno.exprs, sno_expr_ent_t, xi).svar ? SNO_DSTAR_STAGEVAR : 0u) |
            ((CV_AT(g_lower.sno.exprs, sno_expr_ent_t, xi).expr &&
            (CV_AT(g_lower.sno.exprs, sno_expr_ent_t, xi).expr->t == TT_FNC || sno_pe_patfn(CV_AT(g_lower.sno.exprs, sno_expr_ent_t, xi).expr->t))) ? SNO_DSTAR_CALL : 0u);
        g_stage2.proc_table[xpi].stage_var = CV_AT(g_lower.sno.exprs, sno_expr_ent_t, xi).svar;
        g_stage2.proc_table[xpi].result_name = CV_AT(g_lower.sno.exprs, sno_expr_ent_t, xi).name;
        g_stage2.proc_table[xpi].bb_idx = bb_program_add(&g_stage2.bbp, gx);
    }
    g_lower.sno.in_patproc = sv;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sno_pat_count(void) { return (int) g_lower.sno.pats.len; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_register_entry_label(const char * el, int bb_idx) {
    if (!el || !el[0]) return;
    IR_t * anchor = bb_label_landing(el);
    if (!anchor) return;
    char lname[strlen(el) + 6];
    snprintf(lname, sizeof lname, "LBL__%s", el);
    for (int q = 0; q < g_stage2.proc_count; q++) if (g_stage2.proc_table[q].name && !strcmp(g_stage2.proc_table[q].name, lname)) return;
    int lpi = stage2_proc_grow(&g_stage2);
    g_stage2.proc_table[lpi].name = lp_strdup(lname);
    g_stage2.proc_table[lpi].proc = NULL;
    g_stage2.proc_table[lpi].entry_pc = -1;
    g_stage2.proc_table[lpi].nparams = 0;
    g_stage2.proc_table[lpi].lower_sc.n = 0;
    g_stage2.proc_table[lpi].is_generator = 0;
    g_stage2.proc_table[lpi].dyn_scope = 0;
    g_stage2.proc_table[lpi].result_name = NULL;
    g_stage2.proc_table[lpi].proc_entry_node = anchor;
    g_stage2.proc_table[lpi].bb_idx = bb_idx;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_entry_seen_push(cv_t * seen, const char * el) {
    if (!el || !el[0]) return;
    for (uint32_t k = 0; k < seen->len; k++) if (!strcmp(CV_AT(*seen, const char *, k), el)) return;
    CV_PUSH(*seen, const char *) = el;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_indirect_lit_to_var(tree_t * t) {
    if (!t || t->t == TT_GOTO_U || t->t == TT_GOTO_S || t->t == TT_GOTO_F) return;
    if (t->t == TT_NAME && t->n == 1 && t->c[0] && t->c[0]->t == TT_DEFER && t->c[0]->n == 1 && t->c[0]->c[0] && t->c[0]->c[0]->t == TT_INDIRECT) return;
    if ((t->t == TT_CAPT_COND_ASGN || t->t == TT_CAPT_IMMED_ASGN) && t->n == 2 && t->c[1] && t->c[1]->t == TT_INDIRECT && t->c[1]->n == 1) {
        tree_t * nm = t->c[1]->c[0];
        if (nm && nm->t == TT_NAME && nm->n == 1 && nm->c[0] && nm->c[0]->t == TT_DEFER && nm->c[0]->n == 1) {
            tree_t * de = nm->c[0]->c[0];
            if (de && de->t != TT_VAR && sno_name_wrappable(de)) nm->c[0] = de;
        }
    }
    if (t->t == TT_INDIRECT && t->n == 1 && t->c[0] && t->c[0]->t == TT_QLIT && t->c[0]->v.sval && t->c[0]->v.sval[0] && t->c[0]->v.sval[0] != '&' && t->c[0]->v.sval[0] != '*') {
        t->v.sval = t->c[0]->v.sval;
        t->t = TT_VAR;
        t->n = 0;
        return;
    }
    for (int i = 0; i < t->n; i++) sno_indirect_lit_to_var(t->c[i]);
}
static int sno_rsv_program(const tree_t * prog, int quiet) { int bad = 0; for (int i = 0; prog && i < prog->n; i++) bad += sno_rsv_walk((tree_t *) prog->c[i], NULL, quiet); return bad; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
stage2_t * lower_sno_stage2(const tree_t * prog) {
    { extern void gva_keyword_refuse_seed_snobol4(void); gva_keyword_refuse_seed_snobol4(); }
    g_lower.sno.expr_define_seen = 0;
    g_lower.sno.prescan_top = NULL;
    g_lower.sno.seal_enabled = 1;
    if (!prog || prog->t != TT_PROGRAM) return NULL;
    g_stage2.ball_exit_by_code = 1;
    const tree_t * list_prog = prog;
    if (snobol4_noexec()) prog = sno_noexec_program(prog);
    for (int i = 0; i < prog->n; i++) sno_indirect_lit_to_var((tree_t *) prog->c[i]);
    { int rsv = sno_rsv_program(prog, 0); if (rsv) { fprintf(stderr, "scrip: %d reserved-constant error(s) -- no code generated\n", rsv); exit(1); } }
    g_lower.sno.exprs.len = 0;
    g_lower.sno.pats.len = 0;
    g_lower.sno.uses_stmtkw = 0;
    g_lower.sno.traces_a_label = 0;
    g_lower.sno.uses_code = 0;
    g_lower.sno.uses_xname = 0;
    g_lower.sno.calls_code = 0;
    g_lower.sno.multiproto.len = 0;
    g_lower.sno.proto_fn.len = 0;
    g_lower.sno.proto_enc.len = 0;
    for (int i = 0; i < prog->n; i++) if (prog->c[i]) sno_scan_code_use(prog->c[i]);
    for (int i = 0; i < prog->n; i++) if (prog->c[i]) sno_scan_xname_use(prog->c[i]);
    { const char * _sk = getenv("SCRIP_SNO_STMTKW"); if (_sk && *_sk == '1') { g_lower.sno.uses_stmtkw = 1; g_lower.sno.traces_a_label = 1; } }
    sno_register_program(&g_stage2, prog);
    int nst = 0;
    for (int i = 0; i < prog->n; i++) if (prog->c[i] && prog->c[i]->t == TT_STMT) nst++;
    const tree_t ** st = (const tree_t **) ct_zalloc((size_t) nst, sizeof(tree_t *));
    { int k = 0; for (int i = 0; i < prog->n; i++) if (prog->c[i] && prog->c[i]->t == TT_STMT) st[k++] = prog->c[i]; }
    sno_prog_scan(st, nst);
    cv_t defs = {0}, def_body = {0};
    g_lower.sno.predef.len = 0;
    g_lower.sno.tdef.len = 0;
    cv_t def_entry_all = {0};
    int * is_def = (int *) ct_zalloc((size_t) nst, sizeof(int));
    cv_t stmt_bind_fname = {0};
    cv_t exprdef_names = {0};
    for (int i = 0; i < nst; i++) {
        g_lower.sno.prescan_top = lc_stmt_subj(st[i]);
        sno_prescan_expr(g_lower.sno.prescan_top, &defs, &def_body, &exprdef_names, &def_entry_all);
        { const tree_t * rp = sfind_expr(st[i], ":repl"); if (rp) sno_prescan_repl_defines(rp, &defs, &def_body, &exprdef_names, &def_entry_all); }
        const tree_t * dfn = lc_stmt_subj(st[i]);
        if (dfn && dfn->t == TT_DEFINE) {
            is_def[i] = 1;
            const tree_t * pnode = (dfn->n > 1) ? dfn->c[1] : NULL;
            if (!pnode || pnode->t != TT_QLIT || !pnode->v.sval) sno_fatal("TT_DEFINE missing literal prototype string", NULL);
            sno_def_t d;
            sno_parse_define(pnode->v.sval, NULL, &d);
            if (sn4_sysfn_protected(d.fname)) continue;
            sno_tdef_note(d.fname);
            const tree_t * body = (dfn->n > 2) ? dfn->c[2] : NULL;
            int found = -1;
            for (uint32_t k = 0; k < defs.len; k++) if (!strcmp(CV_AT(defs, sno_def_t, k).fname, d.fname)) { found = (int) k; break; }
            if (!body) sno_entry_seen_push(&def_entry_all, d.entry);
            if (found >= 0) { CV_AT(defs, sno_def_t, found) = d; CV_AT(def_body, const tree_t *, found) = body; } else sno_def_put(&defs, &def_body, &d, body);
            CV_PUSH(stmt_bind_fname, const char *) = d.fname;
            continue;
        }
        int argbase = 0;
        const tree_t * dsub = sno_stmt_define(st[i], &argbase);
        if (!dsub) continue;
        is_def[i] = 1;
        const char * entry_opt = NULL;
        if (dsub->n > argbase + 1 && dsub->c[argbase + 1]) {
            const tree_t * ea = dsub->c[argbase + 1];
            if (ea->t == TT_QLIT && ea->v.sval) entry_opt = ea->v.sval;
            else if (ea->t == TT_NAME && ea->n > 0 && ea->c[0] && ea->c[0]->t == TT_VAR && ea->c[0]->v.sval) entry_opt = ea->c[0]->v.sval;
        }
        sno_def_t d;
        sno_parse_define(sno_qlit_fold(dsub->c[argbase]), entry_opt, &d);
        if (sn4_sysfn_protected(d.fname)) continue;
        int found = -1;
        for (uint32_t k = 0; k < defs.len; k++) if (!strcmp(CV_AT(defs, sno_def_t, k).fname, d.fname)) { found = (int) k; break; }
        sno_entry_seen_push(&def_entry_all, d.entry);
        sno_proto_note(&d);
        if (found >= 0) CV_AT(defs, sno_def_t, found) = d;
        else sno_def_put(&defs, &def_body, &d, NULL);
        CV_PUSH(stmt_bind_fname, const char *) = d.fname;
    }
    int main_entry_idx = 0;
    for (int i = 0; i < prog->n; i++) {
        if (!prog->c[i] || prog->c[i]->t != TT_END) continue;
        const char * end_entry = sfind_str(prog->c[i], ":entry");
        if (!end_entry || !end_entry[0]) break;
        for (int k = 0; k < nst; k++) { const char * klbl = sfind_str(st[k], ":lbl"); if (klbl && !strcmp(klbl, end_entry)) { main_entry_idx = k; break; } }
        break;
    }
    long _end_line = 0;
    for (int i = 0; i < prog->n; i++) if (prog->c[i] && prog->c[i]->t == TT_END) { _end_line = (long) lp_s_int(prog->c[i], ":line"); break; }
    long _code_line = _end_line;
    if (!_code_line) for (int i = 0; i < prog->n; i++) if (prog->c[i] && prog->c[i]->t == TT_STMT && lp_s_int(prog->c[i], ":line") > _code_line) _code_line = (long) lp_s_int(prog->c[i], ":line");
    IR_graph_t * g = sno_build_graph(st, nst, main_entry_idx, is_def, NULL, 0, _end_line, _code_line);
    {
        IR_t * prelude_head = NULL;
        IR_t * prelude_tail = NULL;
        for (uint32_t di = 0; di < defs.len; di++) {
            const char * dfn = CV_AT(defs, sno_def_t, di).fname;
            int covered = 0;
            for (uint32_t k = 0; k < stmt_bind_fname.len; k++) if (!strcmp(CV_AT(stmt_bind_fname, const char *, k), dfn)) { covered = 1; break; }
            if (covered || !sno_exprdef_seen(&exprdef_names, dfn)) continue;
            IR_t * pfail = lc_build(g, IR_FAIL, NULL, NULL);
            IR_t * pbind = lc_build(g, IR_DEFINE_BIND, NULL, pfail);
            IR_LIT(pbind).sval = lp_strdup(dfn);
            if (!prelude_head) prelude_head = pbind;
            else lc_γ_to(prelude_tail, pbind);
            prelude_tail = pbind;
        }
        if (prelude_head) { lc_γ_to(prelude_tail, g->entry); g->entry = prelude_head; }
        { IR_t * qt = lc_build(g, IR_CALL, g->entry, g->entry); IR_LIT(qt).sval = (char *) "$quit_trap_320"; g->entry = qt; }
        { IR_t * fpm = lc_build(g, IR_CALL, g->entry, g->entry); IR_LIT(fpm).sval = (char *) "$fp_model_spitbol"; g->entry = fpm; }
        {
            char * listing = sno_list_build_listing(list_prog);
            if (listing && listing[0]) {
                IR_t * lit = lc_build(g, IR_LIT_STRING, g->entry, g->entry);
                IR_LIT(lit).sval = listing;
                IR_t * call = lc_build(g, IR_CALL, g->entry, g->entry);
                IR_LIT(call).sval = (char *) "SNO$LIST";
                lc_γ_to(lit, call);
                ir_operand_push(call, lit);
                g->entry = lit;
            }
        }
    }
    int pi = stage2_proc_grow(&g_stage2);
    g_stage2.proc_table[pi].name = "main";
    g_stage2.proc_table[pi].proc = NULL;
    g_stage2.proc_table[pi].entry_pc = -1;
    g_stage2.proc_table[pi].nparams = 0;
    g_stage2.proc_table[pi].is_generator = 0;
    g_stage2.proc_table[pi].dyn_scope = 0;
    g_stage2.proc_table[pi].result_name = NULL;
    g_stage2.proc_table[pi].bb_idx = bb_program_add(&g_stage2.bbp, g);
    { int mi = g_stage2.module_registry.nmod - 1; if (mi >= 0) { g_stage2.module_registry.mods[mi].nprocs++; if (g_stage2.module_registry.main_mod < 0) g_stage2.module_registry.main_mod = mi; } }
    if (g_lower.sno.uses_code) {
        int main_bb_idx = g_stage2.proc_table[pi].bb_idx;
        for (int i = 0; i < nst; i++) {
            const char * lbl = sfind_str(st[i], ":lbl");
            if (!lbl || !lbl[0]) continue;
            IR_t * anchor = bb_label_landing(lbl);
            if (!anchor) continue;
            char lname[strlen(lbl) + 6];
            snprintf(lname, sizeof lname, "LBL__%s", lbl);
            int lpi = stage2_proc_grow(&g_stage2);
            g_stage2.proc_table[lpi].name = lp_strdup(lname);
            g_stage2.proc_table[lpi].proc = NULL;
            g_stage2.proc_table[lpi].entry_pc = -1;
            g_stage2.proc_table[lpi].nparams = 0;
            g_stage2.proc_table[lpi].lower_sc.n = 0;
            g_stage2.proc_table[lpi].is_generator = 0;
            g_stage2.proc_table[lpi].dyn_scope = 0;
            g_stage2.proc_table[lpi].result_name = NULL;
            g_stage2.proc_table[lpi].proc_entry_node = anchor;
            g_stage2.proc_table[lpi].bb_idx = main_bb_idx;
        }
    }
    if (!g_lower.sno.uses_code) {
        int main_bb_idx2 = g_stage2.proc_table[pi].bb_idx;
        for (uint32_t di = 0; di < defs.len; di++) if (!CV_AT(def_body, const tree_t *, di)) sno_register_entry_label(CV_AT(defs, sno_def_t, di).entry, main_bb_idx2);
        for (uint32_t ei = 0; ei < def_entry_all.len; ei++) sno_register_entry_label(CV_AT(def_entry_all, const char *, ei), main_bb_idx2);
    }
    for (uint32_t di = 0; di < defs.len; di++) {
        const sno_def_t d0 = CV_AT(defs, sno_def_t, di);
        const tree_t * dbody = CV_AT(def_body, const tree_t *, di);
        IR_graph_t * gf;
        const char * rn = d0.result_name ? d0.result_name : d0.fname;
        if (dbody) {
            const tree_t * bp = dbody;
            int bn = 0;
            for (int i = 0; i < bp->n; i++) if (bp->c[i] && bp->c[i]->t == TT_STMT) bn++;
            const tree_t ** bst = (const tree_t **) ct_zalloc((size_t)(bn > 0 ? bn : 1), sizeof(tree_t *));
            int bk = 0;
            for (int i = 0; i < bp->n; i++) if (bp->c[i] && bp->c[i]->t == TT_STMT) bst[bk++] = bp->c[i];
            int * bis = (int *) ct_zalloc((size_t)(bn > 0 ? bn : 1), sizeof(int));
            gf = sno_build_graph(bst, bn, 0, bis, rn, 0, 0, _code_line);
            ct_drop((void *) bst);
            ct_drop(bis);
        } else {
            int eidx = -1;
            for (int i = 0; i < nst; i++) { const char * lbl = sfind_str(st[i], ":lbl"); if (lbl && !strcmp(lbl, d0.entry)) { eidx = i; break; } }
            if (eidx < 0) continue;
            gf = sno_build_call_stub(d0.entry, d0.fname);
        }
        int fpi = stage2_proc_grow(&g_stage2);
        g_stage2.proc_table[fpi].name = d0.fname;
        g_stage2.proc_table[fpi].proc = NULL;
        g_stage2.proc_table[fpi].entry_pc = -1;
        g_stage2.proc_table[fpi].nparams = d0.nnames;
        g_stage2.proc_table[fpi].nformals = d0.nformals;
        stage2_scope_reserve(&g_stage2.proc_table[fpi].lower_sc, d0.nnames);
        for (int k = 0; k < d0.nnames; k++) g_stage2.proc_table[fpi].lower_sc.e[k].name = d0.names[k];
        g_stage2.proc_table[fpi].lower_sc.n = d0.nnames;
        g_stage2.proc_table[fpi].is_generator = 0;
        g_stage2.proc_table[fpi].dyn_scope = 1;
        g_stage2.proc_table[fpi].result_name = rn;
        gf->multi_proto = sno_fname_is_multiproto(d0.fname);
        g_stage2.proc_table[fpi].bb_idx = bb_program_add(&g_stage2.bbp, gf);
        if (dbody && g_lower.sno.uses_code) {
            int fbi = g_stage2.proc_table[fpi].bb_idx;
            for (int i = 0; i < dbody->n; i++) if (dbody->c[i] && dbody->c[i]->t == TT_STMT) sno_register_entry_label(sfind_str(dbody->c[i], ":lbl"), fbi);
        }
    }
    sno_expr_thunks_build(0);
    {
        int xdone = sno_expr_mark();
        int pdone = 0;
        for (;;) {
            int pn = sno_pat_count();
            if (pn > pdone) { sno_pat_thunks_build(pdone); pdone = pn; }
            int xn = sno_expr_mark();
            if (xn > xdone) { sno_expr_thunks_build(xdone); xdone = xn; }
            if (sno_pat_count() == pdone && sno_expr_mark() == xdone) break;
        }
    }
    ct_drop((void *) st);
    ct_drop(is_def);
    return &g_stage2;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * sno_lower_fragment_at(const tree_t * prog, int entry_idx, long stno_base, long code_line) {
    if (!prog || prog->t != TT_PROGRAM || sno_rsv_program(prog, 1)) return NULL;
    { extern void zls_reset(void); zls_reset(); }
    int nst = 0;
    for (int i = 0; i < prog->n; i++) if (prog->c[i] && prog->c[i]->t == TT_STMT) nst++;
    if (nst == 0 || entry_idx < 0 || entry_idx >= nst) return NULL;
    const tree_t ** st = (const tree_t **) ct_zalloc((size_t) nst, sizeof(tree_t *));
    { int k = 0; for (int i = 0; i < prog->n; i++) if (prog->c[i] && prog->c[i]->t == TT_STMT) st[k++] = prog->c[i]; }
    g_lower.sno.predef.len = 0;
    g_lower.sno.tdef.len = 0;
    g_lower.sno.t4.len = 0;
    g_lower.sno.t4_unsafe = 1;
    int seal_sv = g_lower.sno.seal_enabled;
    g_lower.sno.seal_enabled = 0;
    int * is_def = (int *) ct_zalloc((size_t) nst, sizeof(int));
    IR_graph_t * g = sno_build_graph(st, nst, entry_idx, is_def, NULL, stno_base, 0, code_line);
    { extern void optimizer_run(IR_graph_t *); if (g) optimizer_run(g); }
    ct_drop((void *) st);
    ct_drop(is_def);
    g_lower.sno.seal_enabled = seal_sv;
    return g;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * lower_snobol4(const tree_t * prog) { return sno_lower_fragment_at(prog, 0, 0, 0); }
