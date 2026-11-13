#include "lower/lower_raku/lrk_head.h"
#include <string.h>
#include <stdlib.h>
#include "ir/stage2.h"
#include "lower/lower_raku.h"
#include "parsers/snobol4/scrip_cc.h"
#include "lower/lower_common.h"
#include "lower/lower_raku/lrk_01_sh.h"
#include "lower/lower_raku/lrk_03_rk.h"
#include "lower/lower_raku/lrk_07_rk.h"
#include "lower/lower_raku/lrk_12_rk.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_ss_is_check(const tree_t * d) { return d && d->t == TT_SUB_DECL && d->n > 0 && d->c[0] && d->c[0]->v.sval && !strncmp(d->c[0]->v.sval, "__ssf_", 6); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_ss_hoist(tree_t * list) {
    extern tree_t *ast_stmt_new(tree_e kind);
    extern tree_t *ast_attr_int(const char *tag, int ival);
    extern tree_t *ast_attr_expr(const char *tag, tree_t *expr);
    if (!list) return;
    for (int i = 0; i < list->n; i++) {
        tree_t * st = list->c[i];
        tree_t * pk = rk_top_subj(st);
        if (pk && (pk->t == TT_CLASS_DECL || pk->t == TT_ROLE_DECL || pk->t == TT_GRAMMAR_DECL)) {
            for (int k = 1; k < pk->n; k++) {
                if (!rk_ss_is_check(pk->c[k])) continue;
                tree_t * chk = pk->c[k];
                for (int y = k + 1; y < pk->n; y++) pk->c[y - 1] = pk->c[y];
                pk->n--;
                k--;
                tree_t * ins = ast_stmt_new(TT_STMT);
                ast_push(ins, ast_attr_int(":line", 0));
                ast_push(ins, ast_attr_int(":stno", 0));
                ast_push(ins, ast_attr_expr(":subj", chk));
                ast_push(list, NULL);
                for (int y = list->n - 1; y > i; y--) list->c[y] = list->c[y - 1];
                list->c[i] = ins;
                i++;
            }
        }
        if (pk && pk != st) rk_ss_hoist(pk);
        else if (st && st->t != TT_STMT) rk_ss_hoist(st);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_eval_host_classes(const tree_t * t) {
    extern void * dat_find_type(const char * name);
    if (!t) return;
    const char * nm = (t->t == TT_VAR && t->v.sval) ? t->v.sval + (t->v.sval[0] == ':' && t->v.sval[1] == ':' ? 2 : 0) : NULL;
    if (nm && nm[0] >= 'A' && nm[0] <= 'Z' && !rk_is_class_name(nm) && dat_find_type(nm)) CV_PUSH(g_lower.rk.class_names, const char *) = nm;
    for (int i = 0; i < t->n; i++) rk_eval_host_classes(t->c[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_module_names(const tree_t * t, rk_ns_t * out) {
    if (!t) return;
    if (t->t == TT_MODULE_DECL && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR && t->c[0]->v.sval) rk_ns_add(out, t->c[0]->v.sval);
    for (int i = 0; i < t->n; i++) rk_module_names(t->c[i], out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_invocant_params(tree_t * t, const tree_t * pkg) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_invocant_params(t->c[i], t->t == TT_CLASS_DECL || t->t == TT_ROLE_DECL || t->t == TT_GRAMMAR_DECL ? t : NULL);
    if (!pkg || t->t != TT_SUB_DECL || t->n < 2 || t->v.ival < 2) return;
    tree_t * pv = t->c[1];
    if (!pv || pv->t != TT_VAR || !(pv->slen & RK_PV_INV)) return;
    const char * nm = pv->v.sval && pv->v.sval[0] && strcmp(pv->v.sval, "$") && strcmp(pv->v.sval, "self") ? pv->v.sval : NULL;
    if (nm && (nm[0] == '@' || nm[0] == '%' || nm[0] == '&')) return;
    const char * cap = NULL, * ty = NULL;
    for (int k = 0; k < pv->n; k++) {
        const tree_t * q = pv->c[k];
        if (!q || q->t != TT_QLIT || !q->v.sval || !q->v.sval[0]) continue;
        if (strncmp(q->v.sval, "::", 2)) ty = q->v.sval;
        else if (q->v.sval[2] && q->v.sval[2] != '?') cap = q->v.sval + 2;
    }
    const char * tn = NULL;
    if (ty && !strstr(ty, ":U")) {
        char tb[strlen(ty) + 1];
        snprintf(tb, sizeof tb, "%.*s", (int) strcspn(ty, ":"), ty);
        if (tb[0] && !(pkg->n && pkg->c[0] && pkg->c[0]->v.sval && !strcmp(tb, pkg->c[0]->v.sval))) tn = intern(tb);
    }
    if (tn && !nm) nm = "__rk_inv";
    memmove(t->c + 1, t->c + 2, (size_t) (t->n - 2) * sizeof(tree_t *));
    t->n--;
    t->v.ival--;
    int at = (int) t->v.ival;
    for (int k = 0; k < 2; k++) {
        const char * who = k ? cap : nm;
        if (!who) continue;
        tree_t * as = ast_node_new(!k && tn ? TT_DECL : TT_ASSIGN), * dv = leaf_sval2(TT_VAR, who), * sv = leaf_sval2(TT_VAR, "self");
        dv->slen |= 4;
        if (!k && tn) ast_push(as, leaf_sval2(TT_VAR, tn));
        ast_push(as, dv);
        if (k) { tree_t * w = ast_node_new(TT_METHCALL); ast_push(w, sv); ast_push(w, leaf_sval2(TT_QLIT, "WHAT")); sv = w; }
        ast_push(as, sv);
        ast_push(t, as);
        memmove(t->c + at + 1, t->c + at, (size_t) (t->n - 1 - at) * sizeof(tree_t *));
        t->c[at++] = as;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_sub_topic(tree_t * t, int in_pkg) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_sub_topic(t->c[i], t->t == TT_CLASS_DECL || t->t == TT_ROLE_DECL || t->t == TT_GRAMMAR_DECL);
    if (t->t != TT_SUB_DECL || t->n < 1) return;
    int body = in_pkg ? (int) t->v.ival : 1 + (int) t->v.ival;
    if (body < 1 || body >= t->n) return;
    int reads = 0;
    for (int i = 1; i < t->n && !reads; i++) {
        if (i < body && t->c[i] && t->c[i]->t == TT_VAR && t->c[i]->v.sval && !strcmp(t->c[i]->v.sval, "_")) return;
        if (i >= body && lrk_rk_reads_var(t->c[i], "_")) reads = 1;
    }
    if (!reads) return;
    for (int k = 0, np = (int) t->v.ival; k < np && body < t->n && rk_prologue_target(t->c[body]); k++) body++;
    tree_t * as = ast_node_new(TT_ASSIGN), * nil = ast_node_new(TT_FNC), * dv = leaf_sval2(TT_VAR, "_");
    nil->v.sval = (char *) "__rk_typeobj";
    ast_push(nil, leaf_sval2(TT_VAR, "__rk_typeobj"));
    ast_push(nil, leaf_sval2(TT_QLIT, "Any"));
    dv->slen |= 4;
    ast_push(as, dv);
    ast_push(as, nil);
    ast_push(t, as);
    memmove(t->c + body + 1, t->c + body, (size_t) (t->n - 1 - body) * sizeof(tree_t *));
    t->c[body] = as;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_701_decls(const tree_t * t, const char * nm, const tree_t ** one) {
    if (!t) return 0;
    int n = 0;
    if (t->t == TT_SUB_DECL && t->n > 0 && t->c[0] && t->c[0]->v.sval && !strcmp(t->c[0]->v.sval, nm)) { n++; *one = t; }
    for (int i = 0; i < t->n; i++) n += rk_701_decls(t->c[i], nm, one);
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_701_has_default(const tree_t * sd) {
    for (int i = 1; i < sd->n; i++) {
        const tree_t * u = sd->c[i];
        if (!u || u->t != TT_UNLESS || u->n < 1 || !u->c[0] || u->c[0]->t != TT_METHCALL || u->c[0]->n < 2) continue;
        const tree_t * q = u->c[0]->c[1];
        if (u->c[0]->c[0] && u->c[0]->c[0]->t == TT_VAR && q && q->t == TT_QLIT && q->v.sval && !strcmp(q->v.sval, "defined")) return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_701_numeric_text(const char * s) { if (!s) return 0; char * e = NULL; if (*s == '+' || *s == '-') s++; if (!*s) return 0; strtod(s, &e); return e && e != s; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * rk_701_lit_type(const tree_t * a) { return !a ? "Any" : a->t == TT_ILIT ? "Int" : a->t == TT_QLIT ? "Str" : a->t == TT_FLIT ? "Num" : "Any"; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_701_never(const char * ty, const tree_t * a) {
    if (!ty || !a) return 0;
    int numeric = !strcmp(ty, "Int") || !strcmp(ty, "Num") || !strcmp(ty, "Rat") || !strcmp(ty, "Numeric");
    if (numeric && a->t == TT_QLIT && !rk_701_numeric_text(a->v.sval)) return 1;
    if (!strcmp(ty, "Str") && a->t == TT_ILIT) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * rk_701_find(const tree_t * prog, const tree_t * t, int line, int * at) {
    if (!t) return NULL;
    if (t->line > 0) line = t->line;
    int lit = 0;
    for (int k = 1; t->t == TT_FNC && k < t->n; k++) if (t->c[k] && (t->c[k]->t == TT_QLIT || t->c[k]->t == TT_ILIT)) lit = 1;
    if (lit && t->v.sval && strncmp(t->v.sval, "__", 2) && t->n >= 2 && t->c[0] && t->c[0]->t == TT_VAR && t->c[0]->v.sval && !strcmp(t->c[0]->v.sval, t->v.sval)) {
        const tree_t * sd = NULL;
        int nd = rk_701_decls(prog, t->v.sval, &sd);
        int np = 0, ok = nd == 1 && sd && !rk_701_has_default(sd);
        for (int k = 1; ok && k < sd->n && sd->c[k] && sd->c[k]->t == TT_VAR; k++) {
            const tree_t * pv = sd->c[k];
            if (!pv->v.sval || !*pv->v.sval || strchr("@%&", pv->v.sval[0])) ok = 0;
            if (pv->n >= 1 && pv->c[0] && pv->c[0]->v.sval && strpbrk(pv->c[0]->v.sval, ":()*|\\")) ok = 0;
            np++;
        }
        if (ok && np == t->n - 1) for (int k = 1; k <= np; k++) {
            const tree_t * pv = sd->c[k];
            const char * ty = (pv->n >= 1 && pv->c[0]) ? pv->c[0]->v.sval : NULL;
            if (rk_701_never(ty, t->c[k])) { *at = line; return t; }
        }
    }
    for (int i = 0; i < t->n; i++) { const tree_t * r = rk_701_find(prog, t->c[i], line, at); if (r) return r; }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int lower_raku_refuse(const tree_t * prog, const char * file) {
    int line = 0;
    const tree_t * call = rk_701_find(prog, prog, 0, &line);
    if (!call) return 0;
    const tree_t * sd = NULL;
    rk_701_decls(prog, call->v.sval, &sd);
    size_t an = 1, sn = 1;
    for (int k = 1; k < call->n; k++) an += strlen(rk_701_lit_type(call->c[k])) + 2;
    for (int k = 1; k < sd->n && sd->c[k] && sd->c[k]->t == TT_VAR; k++) {
        const tree_t * pv = sd->c[k];
        sn += strlen(pv->v.sval) + 4 + ((pv->n >= 1 && pv->c[0] && pv->c[0]->v.sval) ? strlen(pv->c[0]->v.sval) + 1 : 0);
    }
    char args[an], sig[sn];
    size_t al = 0, sl = 0;
    args[0] = sig[0] = 0;
    for (int k = 1; k < call->n; k++) al += (size_t) snprintf(args + al, an - al, "%s%s", k > 1 ? ", " : "", rk_701_lit_type(call->c[k]));
    for (int k = 1; k < sd->n && sd->c[k] && sd->c[k]->t == TT_VAR; k++) {
        const tree_t * pv = sd->c[k];
        const char * ty = (pv->n >= 1 && pv->c[0] && pv->c[0]->v.sval) ? pv->c[0]->v.sval : NULL;
        sl += (size_t) snprintf(sig + sl, sn - sl, "%s%s%s$%s", k > 1 ? ", " : "", ty ? ty : "", ty ? " " : "", pv->v.sval);
    }
    char msg[fmt_len("call %s(%s) can never succeed against the declared signature (%s)", call->v.sval, args, sig)];
    snprintf(msg, sizeof msg, "call %s(%s) can never succeed against the declared signature (%s)", call->v.sval, args, sig);
    extern void core_error_voice_at(int code, const char *msg, const char *file, long line, long stno);
    core_error_voice_at(701, msg, file, line, 0);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_source_terms(tree_t * t, const char * given, const char * absolute, const char * pkg) {
    for (int i = 0; t && i < t->n; i++) {
        tree_t * c = t->c[i];
        int scoped = c && (c->t == TT_MODULE_DECL || c->t == TT_CLASS_DECL || c->t == TT_ROLE_DECL || c->t == TT_GRAMMAR_DECL) && c->n > 0 && c->c[0] && c->c[0]->t == TT_VAR && c->c[0]->v.sval;
        rk_source_terms(c, given, absolute, scoped ? c->c[0]->v.sval : pkg);
        if (!c || c->t != TT_VAR || !c->v.sval) continue;
        const char * nm = c->v.sval;
        if (!strcmp(nm, "?BITS") || !strcmp(nm, "?TABSTOP")) { tree_t * k = ast_node_new(TT_ILIT); k->line = c->line; k->v.ival = nm[1] == 'B' ? 64 : 8; t->c[i] = k; continue; }
        if (!strcmp(nm, "?PACKAGE") || !strcmp(nm, "?MODULE") || !strcmp(nm, "?CLASS") || !strcmp(nm, "?ROLE")) {
            tree_t * f = ast_node_new(TT_FNC);
            f->line = c->line;
            f->v.sval = (char *) "__rk_typeobj";
            ast_push(f, leaf_sval2(TT_VAR, "__rk_typeobj"));
            ast_push(f, leaf_sval2(TT_QLIT, pkg ? pkg : "GLOBAL"));
            t->c[i] = f;
            continue;
        }
        int prog = !strcmp(nm, "*PROGRAM"), name = !strcmp(nm, "*PROGRAM-NAME"), file = !strcmp(nm, "?FILE");
        if (!prog && !name && !file) continue;
        tree_t * f = ast_node_new(TT_FNC);
        f->line = c->line;
        f->v.sval = (char *) "__rk_pre_src";
        ast_push(f, leaf_sval2(TT_VAR, "__rk_pre_src"));
        ast_push(f, leaf_sval2(TT_QLIT, prog ? "*PROGRAM" : "*PROGRAM-NAME"));
        ast_push(f, leaf_sval2(TT_QLIT, file ? absolute : given));
        t->c[i] = f;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_lax_reads(const tree_t * t, const rk_ns_t * assigned, rk_ns_t * out) {
    if (!t) return;
    if (t->t == TT_VAR && !t->n && t->v.sval &&
        (t->v.sval[0] == '@' || t->v.sval[0] == '%' || (!(t->slen & 1) && (isalpha((unsigned char) t->v.sval[0]) || t->v.sval[0] == '_') && strcmp(t->v.sval, "_"))) &&
        !strpbrk(t->v.sval, ":*?!.^=") && !rk_ns_has(assigned, t->v.sval) && strncmp(t->v.sval, "__", 2)) rk_ns_add(out, t->v.sval);
    for (int i = 0; i < t->n; i++) rk_lax_reads(t->c[i], assigned, out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_declare_lax(tree_t * prog) {
    int lax = 0;
    for (int i = 0; i < prog->n; i++) {
        const tree_t * st = prog->c[i];
        if (st && st->t == TT_STMT) st = lrk_stmt_subj(st);
        if (st && st->t == TT_USE_DECL && st->v.sval && !strcmp(st->v.sval, "no-strict")) lax = 1;
    }
    if (!lax) return;
    rk_ns_t assigned = { 0 }, reads = { 0 };
    rk_lax_reads(prog, &assigned, &reads);
    for (int i = reads.n - 1; i >= 0; i--) {
        const char * nm = reads.v[i];
        tree_t * as = ast_node_new(TT_ASSIGN), * init = ast_node_new(TT_FNC);
        init->v.sval = (char *) (nm[0] == '%' ? "__rk_hash" : nm[0] == '@' ? "__rk_arr" : "__rk_undef");
        ast_push(init, leaf_sval2(TT_VAR, init->v.sval));
        tree_t * dv = leaf_sval2(TT_VAR, nm);
        dv->slen |= 4;
        ast_push(as, dv);
        ast_push(as, init);
        ast_push(prog, as);
        memmove(prog->c + 1, prog->c, (size_t) (prog->n - 1) * sizeof(tree_t *));
        prog->c[0] = as;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int lrk_rk_reads_var(const tree_t * t, const char * nm) {
    if (!t) return 0;
    if (t->t == TT_VAR && t->v.sval && !strcmp(t->v.sval, nm)) return 1;
    for (int i = 0; i < t->n; i++) if (lrk_rk_reads_var(t->c[i], nm)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_pod_list(tree_t * prog) {
    for (int i = 0; i < prog->n; i++) {
        const tree_t * st = prog->c[i];
        if (st && st->t == TT_STMT) st = lrk_stmt_subj(st);
        if (!st || st->t != TT_FNC || !st->v.sval || strcmp(st->v.sval, "__rk_pods")) continue;
        tree_t * list = (tree_t *) st;
        memmove(prog->c + i, prog->c + i + 1, (size_t) (prog->n - i - 1) * sizeof(tree_t *));
        prog->n--;
        return list;
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_why_index(const tree_t * pods, const char * nm) {
    for (int i = 1; pods && i < pods->n; i++) {
        const tree_t * d = pods->c[i];
        if (!d || d->t != TT_NEW || d->n < 3 || !d->c[0]->v.sval || strcmp(d->c[0]->v.sval, "Pod::Block::Declarator")) continue;
        for (int k = 1; k + 1 < d->n; k += 2) if (d->c[k]->v.sval && !strcmp(d->c[k]->v.sval, "WHEREFORE") && d->c[k + 1]->v.sval && !strcmp(d->c[k + 1]->v.sval, nm)) return i - 1;
    }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_why_walk(tree_t * t, const tree_t * pods) {
    int hit = 0;
    for (int i = 0; i < t->n; i++) {
        tree_t * c = t->c[i];
        if (!c) continue;
        if (c->t == TT_METHCALL && c->n == 2 && c->c[0] && c->c[0]->t == TT_VAR && c->c[0]->v.sval && c->c[0]->v.sval[0] == '&' && c->c[1] && c->c[1]->t == TT_QLIT && c->c[1]->v.sval && !strcmp(c->c[1]->v.sval, "WHY")) {
            int k = rk_why_index(pods, c->c[0]->v.sval + 1);
            tree_t * r = leaf_sval2(TT_VAR, "Nil");
            if (k >= 0) { r = ast_node_new(TT_ARR_GET); tree_t * ix = ast_node_new(TT_ILIT); ix->v.ival = k; ast_push(r, leaf_sval2(TT_VAR, "=pod")); ast_push(r, ix); }
            t->c[i] = r;
            hit = 1;
            continue;
        }
        hit |= rk_why_walk(c, pods);
    }
    return hit;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_declare_pod(tree_t * prog) {
    tree_t * pods = rk_pod_list(prog);
    int why = rk_why_walk(prog, pods);
    if (!why && !lrk_rk_reads_var(prog, "=pod")) return;
    tree_t * arr = ast_node_new(TT_FNC), * as = ast_node_new(TT_ASSIGN);
    arr->v.sval = (char *) "__rk_arr_lit";
    ast_push(arr, leaf_sval2(TT_VAR, "__rk_arr_lit"));
    for (int i = 1; pods && i < pods->n; i++) ast_push(arr, pods->c[i]);
    tree_t * pv = leaf_sval2(TT_VAR, "=pod");
    pv->slen |= 4;
    ast_push(as, pv);
    ast_push(as, arr);
    ast_push(prog, as);
    memmove(prog->c + 1, prog->c, (size_t) (prog->n - 1) * sizeof(tree_t *));
    prog->c[0] = as;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void lower_raku_source(tree_t * prog, const char * file) {
    if (!prog || !file || !*file) return;
    const char * absolute = file;
    if (file[0] != '/') {
        char * cwd = NULL;
        for (size_t n = 256; n < ((size_t) 1 << 24) && !cwd; n *= 2) { char * b = (char *) ct_alloc(n); if (getcwd(b, n)) cwd = b; else ct_drop(b); }
        if (cwd) {
            const char * rel = (file[0] == '.' && file[1] == '/') ? file + 2 : file;
            char joined[fmt_len("%s/%s", cwd, rel)];
            snprintf(joined, sizeof joined, "%s/%s", cwd, rel);
            absolute = lp_strdup(joined);
            ct_drop(cwd);
        }
    }
    rk_source_terms(prog, lp_strdup(file), absolute, NULL);
    rk_declare_pod(prog);
    rk_declare_lax(prog);
}
