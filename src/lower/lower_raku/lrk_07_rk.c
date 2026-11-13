#include "lower/lower_raku/lrk_head.h"
#include <string.h>
#include "ir/stage2.h"
#include "parsers/snobol4/scrip_cc.h"
#include "runtime/builtins/gen_runtime.h"
#include "lower/lower_common.h"
#include "lower/lower_raku/lrk_01_sh.h"
#include "lower/lower_raku/lrk_03_rk.h"
#include "lower/lower_raku/lrk_05_rk.h"
#include "lower/lower_raku/lrk_06_rk.h"
#include "lower/lower_raku/lrk_07_rk.h"
#include "lower/lower_raku/lrk_11_rk.h"
static void rk_cap_walk(tree_t * t, const rk_ns_t * encl);
static void rk_cap_block(tree_t * parent, int idx, const rk_ns_t * encl);
static void rk_cap_proc_m(tree_t * sd, int mth);
static void rk_bind_names(const tree_t * t, rk_ns_t * out, const rk_ns_t * skip);
static int rk_cap_mutated(const tree_t * t, const char * v);
static void rk_shd_enter(tree_t * s, const rk_shd_t * up, const rk_ns_t * callees, int * ctr);
static void rk_shd_walk(tree_t * t, const rk_shd_t * sc, const rk_ns_t * callees, int * ctr);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_proc_slurpy_only(const char * name, int nref) {
    for (int i = 0; name && i < g_stage2.proc_count; i++) if (g_stage2.proc_table[i].name && !strcmp(g_stage2.proc_table[i].name, name)) return g_stage2.proc_table[i].is_variadic &&
        !g_stage2.proc_table[i].named_rest && g_stage2.proc_table[i].nparams == nref + 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_ns_has(const rk_ns_t * s, const char * x) { for (int i = 0; s && i < s->n; i++) if (!strcmp(s->v[i], x)) return 1; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_ns_addf(rk_ns_t * s, const char * x, int f) {
    if (!x) return;
    for (int i = 0; i < s->n; i++) if (!strcmp(s->v[i], x)) { s->fl[i] |= (unsigned char) f; return; }
    if (s->n >= s->cap) {
        s->cap = s->cap ? s->cap * 2 : 8;
        s->v = (const char **) ct_grow((void *) s->v, sizeof(const char *) * (size_t) s->cap);
        s->fl = (unsigned char *) ct_grow((void *) s->fl, (size_t) s->cap);
    }
    s->fl[s->n] = (unsigned char) f;
    s->v[s->n++] = x;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_ns_add(rk_ns_t * s, const char * x) { rk_ns_addf(s, x, 0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_ns_flag(const rk_ns_t * s, const char * x) { for (int i = 0; s && x && i < s->n; i++) if (!strcmp(s->v[i], x)) return s->fl[i]; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_tree_size(const tree_t * t) { int n = 1; for (int i = 0; t && i < t->n; i++) n += rk_tree_size(t->c[i]); return t ? n : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * rk_cap_pname(const tree_t * pv) { if (pv && pv->t == TT_ASSIGN && pv->n) pv = pv->c[0]; return (pv && pv->t == TT_VAR) ? pv->v.sval : NULL; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_cap_declared(const tree_t * t, rk_ns_t * out) {
    if (!t || t->t == TT_ANON_BLOCK || t->t == TT_SUB_DECL) return;
    if (t->t == TT_VAR && (t->slen & 4) && t->v.sval) rk_ns_add(out, t->v.sval);
    if (t->t == TT_ITERATE && t->v.sval && strcmp(t->v.sval, "_")) rk_ns_add(out, t->v.sval);
    if (t->t == TT_FOR_RANGE && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR && t->c[0]->v.sval) rk_ns_add(out, t->c[0]->v.sval);
    for (int i = 0; i < t->n; i++) rk_cap_declared(t->c[i], out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_cap_uses(const tree_t * t, rk_ns_t * out) {
    if (!t || t->t == TT_ANON_BLOCK || t->t == TT_SUB_DECL) return;
    if (t->t == TT_VAR && t->v.sval) rk_ns_add(out, t->v.sval);
    if (t->t == TT_TWIGIL_FIELD) rk_ns_add(out, intern("self"));
    for (int i = (t->t == TT_FNC ? 1 : 0); i < t->n; i++) rk_cap_uses(t->c[i], out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_cap_synchronous(const tree_t * parent, int idx) {
    static const char * const meth[] = { "map", "grep", "first", "sort", "reduce", "min", "max", "classify", "categorize", "produce", NULL };
    static const char * const fn[] = { "map", "grep", "first", "sort", "reduce", NULL };
    if (!parent) return 0;
    if (parent->t == TT_METHCALL && idx >= 2 && parent->n > 1 && parent->c[1] && parent->c[1]->v.sval) { for (int i = 0; meth[i]; i++) if (!strcmp(parent->c[1]->v.sval, meth[i])) return 1; return 0; }
    if (parent->t == TT_FNC && idx >= 1 && parent->n > 0 && parent->c[0] && parent->c[0]->v.sval) { for (int i = 0; fn[i]; i++) if (!strcmp(parent->c[0]->v.sval, fn[i])) return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_cap_assign_through(tree_t * t, const rk_ns_t * F) {
    if (!t || t->t == TT_ANON_BLOCK || t->t == TT_SUB_DECL) return;
    for (int i = 0; i < t->n; i++) rk_cap_assign_through(t->c[i], F);
    if (rk_is_scalar_bind(t) || (t->t == TT_ASSIGN && t->v.ival == 2 && t->n > 1 && t->c[0] && t->c[0]->t == TT_TWIGIL_FIELD)) return;
    if (t->t == TT_ASSIGN && t->n > 1 && t->c[1] && t->c[1]->t == TT_VAR && t->c[1]->v.sval && rk_ns_has(F, t->c[1]->v.sval)) {
        tree_t * rd = ast_node_new(TT_FNC);
        rd->v.sval = (char *) "__rk_deref";
        ast_push(rd, leaf_sval2(TT_VAR, "__rk_deref"));
        ast_push(rd, t->c[1]);
        t->c[1] = rd;
    }
    if (t->t == TT_METHCALL && t->n > 2 && t->c[0] && t->c[0]->t == TT_VAR && t->c[0]->v.sval && rk_ns_has(F, t->c[0]->v.sval) && t->c[1] && t->c[1]->v.sval &&
        (!strcmp(t->c[1]->v.sval, "push") || !strcmp(t->c[1]->v.sval, "append") || !strcmp(t->c[1]->v.sval, "unshift") || !strcmp(t->c[1]->v.sval, "prepend"))) {
        const char * mn = t->c[1]->v.sval;
        const char * pf = !strcmp(mn, "push") ? "push_pure" : !strcmp(mn, "append") ? "append_pure" : !strcmp(mn, "unshift") ? "unshift_pure" : "prepend_pure";
        tree_t * recv = t->c[0];
        int line = t->line;
        tree_t * inner = ast_node_new(TT_FNC);
        inner->v.sval = (char *) pf;
        ast_push(inner, leaf_sval2(TT_VAR, pf));
        ast_push(inner, leaf_sval2(TT_VAR, recv->v.sval));
        for (int i = 2; i < t->n; i++) ast_push(inner, t->c[i]);
        t->t = TT_FNC;
        t->v.sval = (char *) "__rk_byref_assign";
        t->n = 0;
        t->line = line;
        ast_push(t, leaf_sval2(TT_VAR, "__rk_byref_assign"));
        ast_push(t, recv);
        ast_push(t, inner);
        return;
    }
    if (t->t == TT_HASH_SET && t->n > 2 && t->c[0] && t->c[0]->t == TT_VAR && t->c[0]->v.sval && rk_ns_has(F, t->c[0]->v.sval)) {
        tree_t * inner = ast_node_new(TT_FNC);
        inner->v.sval = (char *) "hash_set_pure";
        ast_push(inner, leaf_sval2(TT_VAR, "hash_set_pure"));
        for (int i = 0; i < t->n; i++) ast_push(inner, t->c[i]);
        tree_t * recv = t->c[0];
        int line = t->line;
        t->t = TT_FNC;
        t->v.sval = (char *) "__rk_byref_assign";
        t->n = 0;
        t->line = line;
        ast_push(t, leaf_sval2(TT_VAR, "__rk_byref_assign"));
        ast_push(t, recv);
        ast_push(t, inner);
        return;
    }
    if (t->t == TT_ASSIGN && t->n > 1 && t->c[0] && t->c[0]->t == TT_VAR && t->c[0]->v.sval && !(t->c[0]->slen & 4) && rk_ns_has(F, t->c[0]->v.sval)) {
        tree_t * lhs = t->c[0], * rhs = t->c[1];
        int line = t->line;
        tree_t tmp = *t;
        (void) tmp;
        t->t = TT_FNC;
        t->v.sval = (char *) "__rk_byref_assign";
        t->n = 0;
        t->line = line;
        ast_push(t, leaf_sval2(TT_VAR, "__rk_byref_assign"));
        ast_push(t, lhs);
        ast_push(t, rhs);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_cap_uses_deep(const tree_t * t, rk_ns_t * out) {
    if (!t) return;
    if (t->t == TT_VAR && t->v.sval) rk_ns_add(out, t->v.sval);
    for (int i = (t->t == TT_FNC ? 1 : 0); i < t->n; i++) rk_cap_uses_deep(t->c[i], out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_cap_declared_deep(const tree_t * t, rk_ns_t * out) {
    if (!t) return;
    if (t->t == TT_VAR && (t->slen & 4) && t->v.sval) rk_ns_add(out, t->v.sval);
    if (t->t == TT_ITERATE && t->v.sval) rk_ns_add(out, t->v.sval);
    if (t->t == TT_FOR_RANGE && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR && t->c[0]->v.sval) rk_ns_add(out, t->c[0]->v.sval);
    if (t->t == TT_ANON_BLOCK) for (int k = 1; k < t->n; k++) { const char * pn = rk_cap_pname(t->c[k]); if (pn) rk_ns_add(out, pn); }
    if (t->t == TT_SUB_DECL) { int np = (int) t->v.ival; for (int k = 0; k < np && 1 + k < t->n; k++) { const char * pn = rk_cap_pname(t->c[1 + k]); if (pn) rk_ns_add(out, pn); } }
    for (int i = 0; i < t->n; i++) rk_cap_declared_deep(t->c[i], out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_cap_is_callee(const tree_t * t, const char * name) { return t && t->t == TT_FNC && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR && t->c[0]->v.sval && !strcmp(t->c[0]->v.sval, name); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_cap_name_escapes(const tree_t * t, const char * name, const tree_t * decl) {
    if (!t || t == decl) return 0;
    if (t->t == TT_VAR && t->v.sval && (!strcmp(t->v.sval, name) || (t->v.sval[0] == '&' && !strcmp(t->v.sval + 1, name)))) return 1;
    for (int i = (rk_cap_is_callee(t, name) ? 1 : 0); i < t->n; i++) if (rk_cap_name_escapes(t->c[i], name, decl)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_cap_name_bare(const tree_t * t, const char * name, const tree_t * decl) {
    if (!t || t == decl) return 0;
    if (t->t == TT_VAR && t->v.sval && !strcmp(t->v.sval, name)) return 1;
    for (int i = (rk_cap_is_callee(t, name) ? 1 : 0); i < t->n; i++) if (rk_cap_name_bare(t->c[i], name, decl)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_cap_any_mutated(const tree_t * sd, int np, const rk_ns_t * F) {
    for (int j = 1 + np; j < sd->n; j++) for (int k = 0; k < F->n; k++) if (rk_cap_mutated(sd->c[j], F->v[k])) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_cap_ref_close(tree_t * t, const char * name, const tree_t * decl, const rk_ns_t * F) {
    if (!t || t == decl) return;
    for (int i = 0; i < t->n; i++) {
        tree_t * c = t->c[i];
        if (c && c != decl && c->t == TT_VAR && c->v.sval && c->v.sval[0] == '&' && !strcmp(c->v.sval + 1, name)) {
            tree_t * cl = ast_node_new(TT_FNC);
            cl->line = c->line;
            cl->v.sval = (char *) "__blk_close";
            ast_push(cl, leaf_sval2(TT_VAR, "__blk_close"));
            ast_push(cl, c);
            for (int k = 0; k < F->n; k++) {
                tree_t * bv = ast_node_new(TT_FNC);
                bv->v.sval = (char *) "__rk_byval";
                ast_push(bv, leaf_sval2(TT_VAR, "__rk_byval"));
                ast_push(bv, leaf_sval2(TT_VAR, F->v[k]));
                ast_push(cl, bv);
            }
            t->c[i] = cl;
        } else rk_cap_ref_close(c, name, decl, F);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_cap_calls_prepend(tree_t * t, const char * name, const rk_ns_t * F) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_cap_calls_prepend(t->c[i], name, F);
    if (rk_cap_is_callee(t, name)) {
        tree_t ** nc = (tree_t **) ct_alloc(sizeof(tree_t *) * (size_t) (t->n + F->n + 1));
        int k = 0;
        nc[k++] = t->c[0];
        for (int i = 0; i < F->n; i++) nc[k++] = leaf_sval2(TT_VAR, F->v[i]);
        for (int i = 1; i < t->n; i++) nc[k++] = t->c[i];
        t->n = 0;
        for (int i = 0; i < k; i++) ast_push(t, nc[i]);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_cap_nested_find(tree_t * t, tree_t * sd, int np, const rk_ns_t * d) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) {
        tree_t * c = t->c[i];
        if (!c) continue;
        if (c->t == TT_SUB_DECL && c->n > 0 && c->c[0] && c->c[0]->v.sval && *c->c[0]->v.sval && c->c[0]->t == TT_VAR && c->v.ival >= 0 && c->v.ival < 12) {
            const char * nm = c->c[0]->v.sval;
            int cnp = (int) c->v.ival;
            rk_ns_t used = { 0 }, decl = { 0 }, F = { 0 };
            for (int k = cnp + 1; k < c->n; k++) rk_cap_uses_deep(c->c[k], &used);
            for (int k = 1; k <= cnp && k < c->n; k++) {
                const tree_t * pv = c->c[k];
                if (pv && pv->t == TT_ASSIGN && pv->n) rk_cap_uses_deep(pv->c[pv->n - 1], &used);
                { const char * pn = rk_cap_pname(pv); if (pn) rk_ns_add(&decl, pn); }
            }
            for (int k = cnp + 1; k < c->n; k++) rk_cap_declared_deep(c->c[k], &decl);
            for (int k = 0; k < used.n; k++) {
                const char * u = used.v[k];
                if (!strcmp(u, "_") || u[0] == '^' || !strncmp(u, "__", 2) || u[0] == '&' || rk_ns_has(&decl, u) || !rk_ns_has(d, u)) continue;
                rk_ns_add(&F, u);
            }
            int esc = rk_cap_name_escapes(sd, nm, c);
            int refonly = esc && F.n && !rk_cap_name_bare(sd, nm, c) && !rk_cap_any_mutated(sd, np, &F);
            if (F.n && F.n + cnp <= 12 && (!esc || refonly)) {
                rk_cap_calls_prepend(sd, nm, &F);
                tree_t ** nc = (tree_t **) ct_alloc(sizeof(tree_t *) * (size_t) (c->n + F.n + 1));
                int k = 0;
                nc[k++] = c->c[0];
                for (int j = 0; j < F.n; j++) { tree_t * pv = leaf_sval2(TT_VAR, F.v[j]); if (!refonly) ast_push(pv, leaf_sval2(TT_QLIT, "@")); nc[k++] = pv; }
                for (int j = 1; j < c->n; j++) nc[k++] = c->c[j];
                c->n = 0;
                for (int j = 0; j < k; j++) ast_push(c, nc[j]);
                c->v.ival = cnp + F.n;
                if (refonly) rk_cap_ref_close(sd, nm, c, &F);
                else for (int j = 1 + c->v.ival; j < c->n; j++) rk_cap_assign_through(c->c[j], &F);
            }
            rk_cap_nested_find(c, sd, np, d);
            continue;
        }
        if (c->t == TT_ANON_BLOCK) continue;
        rk_cap_nested_find(c, sd, np, d);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_cap_mut_method(const char * m) {
    static const char * const k[] = { "push", "append", "unshift", "prepend", "pop", "shift", "splice", NULL };
    for (int i = 0; m && k[i]; i++) if (!strcmp(m, k[i])) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_cap_is_var(const tree_t * t, const char * v) { return t && t->t == TT_VAR && t->v.sval && !strcmp(t->v.sval, v); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_cap_mutated(const tree_t * t, const char * v) {
    if (!t) return 0;
    if ((t->t == TT_ASSIGN || t->t == TT_ARR_SET || t->t == TT_HASH_SET) && t->n > 0 && rk_cap_is_var(t->c[0], v) && !(t->t == TT_ASSIGN && (t->c[0]->slen & 4))) return 1;
    if (t->t == TT_ASSIGN && t->n > 1 && rk_cap_is_var(t->c[0], v) && (t->c[0]->slen & 4)) { rk_ns_t ru = { 0 }; rk_cap_uses_deep(t->c[1], &ru); if (rk_ns_has(&ru, v)) return 1; }
    if (t->t == TT_METHCALL && t->n > 1 && rk_cap_is_var(t->c[0], v) && t->c[1] && rk_cap_mut_method(t->c[1]->v.sval)) return 1;
    if (t->t == TT_FNC && t->n > 1 && t->c[0] && rk_cap_mut_method(t->c[0]->v.sval) && rk_cap_is_var(t->c[1], v)) return 1;
    for (int i = 0; i < t->n; i++) if (rk_cap_mutated(t->c[i], v)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_cap_decl_assigns(const tree_t * t, rk_ns_t * out) {
    if (!t || t->t == TT_ANON_BLOCK || t->t == TT_SUB_DECL) return;
    if (t->t == TT_ASSIGN && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR && (t->c[0]->slen & 4)) rk_ns_add(out, t->c[0]->v.sval);
    if (t->t == TT_DECL && t->n > 1 && t->c[1] && t->c[1]->t == TT_VAR && (t->c[1]->slen & 4) && t->c[1]->v.sval) rk_ns_add(out, t->c[1]->v.sval);
    for (int i = 0; i < t->n; i++) rk_cap_decl_assigns(t->c[i], out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_cap_boxes(const tree_t * t, const tree_t * sd, int np, const rk_ns_t * own, const rk_ns_t * boxable, rk_ns_t * out) {
    for (int i = 0; t && i < t->n; i++) {
        const tree_t * c = t->c[i];
        if (!c || c->t == TT_SUB_DECL) continue;
        if (c->t == TT_ANON_BLOCK && !rk_cap_synchronous(t, i)) {
            rk_ns_t used = { 0 }, inner = { 0 };
            rk_cap_uses_deep(c, &used);
            rk_cap_declared_deep(c, &inner);
            for (int k = 0; k < used.n; k++) {
                const char * u = used.v[k];
                if (rk_ns_has(&inner, u) || !rk_ns_has(own, u) || !rk_ns_has(boxable, u)) continue;
                int mut = (u[0] == '@' || u[0] == '%');
                for (int j = 1 + np; !mut && j < sd->n; j++) mut = rk_cap_mutated(sd->c[j], u);
                if (mut) rk_ns_add(out, u);
            }
        }
        rk_cap_boxes(c, sd, np, own, boxable, out);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_cap_flag_boxed(tree_t * t, const rk_ns_t * Bx) {
    if (!t) return;
    if (t->t == TT_VAR && t->v.sval && rk_ns_has(Bx, t->v.sval)) t->slen |= RK_VF_BOXED;
    for (int i = 0; i < t->n; i++) { tree_t * c = t->c[i]; if (c && (c->t == TT_ANON_BLOCK || c->t == TT_SUB_DECL)) continue; rk_cap_flag_boxed(c, Bx); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_cap_box_decls(tree_t * t, const rk_ns_t * Bx) {
    if (!t || t->t == TT_ANON_BLOCK || t->t == TT_SUB_DECL) return;
    for (int i = 0; i < t->n; i++) rk_cap_box_decls(t->c[i], Bx);
    if (t->t == TT_DECL && t->n >= 2 && t->c[1] && t->c[1]->t == TT_VAR && (t->c[1]->slen & 4) && t->c[1]->v.sval && rk_ns_has(Bx, t->c[1]->v.sval)) {
        tree_t * var = t->c[1];
        tree_t * val = (t->n > 2 && t->c[2]) ? t->c[2] : rk_fnc2("__rk_undef", NULL, NULL);
        rk_ns_t ru = { 0 };
        rk_cap_uses_deep(val, &ru);
        if (!rk_ns_has(&ru, var->v.sval)) { tree_t * bx = rk_fnc2("__rk_box", val, NULL); t->t = TT_ASSIGN; t->n = 0; ast_push(t, var); ast_push(t, bx); return; }
    }
    if (t->t == TT_ASSIGN && t->v.ival != 2 && t->n > 1 && t->c[0] && t->c[0]->t == TT_VAR && (t->c[0]->slen & 4) && rk_ns_has(Bx, t->c[0]->v.sval)) {
        const char * v = t->c[0]->v.sval;
        rk_ns_t ru = { 0 };
        rk_cap_uses_deep(t->c[1], &ru);
        tree_t * bx = ast_node_new(TT_FNC);
        bx->line = t->line;
        bx->v.sval = (char *) "__rk_box";
        ast_push(bx, leaf_sval2(TT_VAR, "__rk_box"));
        if (!rk_ns_has(&ru, v)) { ast_push(bx, t->c[1]); t->c[1] = bx; return; }
        tree_t * un = ast_node_new(TT_FNC);
        un->v.sval = (char *) "__rk_undef";
        ast_push(un, leaf_sval2(TT_VAR, "__rk_undef"));
        ast_push(bx, un);
        tree_t * rhs = t->c[1], * lhs = t->c[0];
        tree_t * lv = leaf_sval2(TT_VAR, v);
        lv->slen |= RK_VF_BOXED;
        tree_t * st = ast_node_new(TT_FNC);
        st->line = t->line;
        st->v.sval = (char *) "__rk_byref_assign";
        ast_push(st, leaf_sval2(TT_VAR, "__rk_byref_assign"));
        ast_push(st, lv);
        ast_push(st, rhs);
        t->c[1] = bx;
        tree_t * decl = ast_node_new(TT_ASSIGN);
        decl->line = t->line;
        ast_push(decl, lhs);
        ast_push(decl, bx);
        t->t = TT_SEQ_EXPR;
        t->n = 0;
        ast_push(t, decl);
        ast_push(t, st);
    }
}
static int rk_cap_is_package(const tree_t * t) { return t && (t->t == TT_CLASS_DECL || t->t == TT_ROLE_DECL); }
static void rk_cap_proc(tree_t * sd) { rk_cap_proc_m(sd, 0); }
static void rk_cap_proc_m(tree_t * sd, int mth) {
    rk_ns_t d = { 0 };
    int np = (int) sd->v.ival - (mth ? 1 : 0);
    if (mth) rk_ns_add(&d, intern("self"));
    for (int k = 0; k < np && 1 + k < sd->n; k++) { const char * pn = rk_cap_pname(sd->c[1 + k]); if (pn) rk_ns_add(&d, pn); }
    for (int i = 1 + np; i < sd->n; i++) rk_cap_declared(sd->c[i], &d);
    rk_ns_t boxable = { 0 }, Bx = { 0 };
    for (int k = 0; k < np && 1 + k < sd->n; k++) { const char * pn = rk_cap_pname(sd->c[1 + k]); if (pn) rk_ns_add(&boxable, pn); }
    for (int i = 1 + np; i < sd->n; i++) rk_cap_decl_assigns(sd->c[i], &boxable);
    rk_cap_boxes(sd, sd, np, &d, &boxable, &Bx);
    for (int i = 1 + np; i < sd->n; i++) rk_bind_names(sd->c[i], &Bx, NULL);
    for (int k = 0; k < np && 1 + k < sd->n; k++) { const char * pn = rk_cap_pname(sd->c[1 + k]); if (pn && rk_param_is_rw(sd->c[1 + k])) rk_ns_add(&Bx, pn); }
    for (int i = 0; i < Bx.n; i++) rk_ns_addf(&d, Bx.v[i], 1);
    rk_cap_nested_find(sd, sd, np, &d);
    for (int i = 1 + np; i < sd->n; i++) {
        tree_t * c = sd->c[i];
        if (!c) continue;
        if (c->t == TT_SUB_DECL) rk_cap_proc(c);
        else if (c->t == TT_ANON_BLOCK) rk_cap_block(sd, i, &d);
        else rk_cap_walk(c, &d);
    }
    if (!Bx.n) return;
    for (int i = 1 + np; i < sd->n; i++) { rk_cap_assign_through(sd->c[i], &Bx); rk_cap_flag_boxed(sd->c[i], &Bx); rk_cap_box_decls(sd->c[i], &Bx); rk_bind_unbox(sd->c[i], &Bx); }
    int added = 0;
    for (int k = 0; k < np && 1 + k < sd->n; k++) {
        const char * pn = rk_cap_pname(sd->c[1 + k]);
        if (!pn || !rk_ns_has(&Bx, pn)) continue;
        const char * wrap = rk_param_is_rw(sd->c[1 + k]) ? "__rk_cell_of" : "__rk_box";
        tree_t * bx = ast_node_new(TT_FNC);
        bx->line = sd->line;
        bx->v.sval = (char *) wrap;
        ast_push(bx, leaf_sval2(TT_VAR, wrap));
        ast_push(bx, leaf_sval2(TT_VAR, pn));
        tree_t * as = ast_node_new(TT_ASSIGN);
        as->line = sd->line;
        ast_push(as, leaf_sval2(TT_VAR, pn));
        ast_push(as, bx);
        ast_push(sd, as);
        for (int j = sd->n - 1; j > 1 + np + added; j--) sd->c[j] = sd->c[j - 1];
        sd->c[1 + np + added] = as;
        added++;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_cap_block(tree_t * parent, int idx, const rk_ns_t * encl) {
    tree_t * B = parent->c[idx];
    rk_ns_t dB = { 0 };
    for (int k = 1; k < B->n; k++) { const char * pn = rk_cap_pname(B->c[k]); if (pn) rk_ns_add(&dB, pn); }
    if (B->n > 0) rk_cap_declared(B->c[0], &dB);
    int wl = rk_cap_synchronous(parent, idx);
    rk_ns_t e2 = { 0 };
    for (int i = 0; encl && i < encl->n; i++) rk_ns_addf(&e2, encl->v[i], encl->fl[i]);
    for (int i = 0; i < dB.n; i++) rk_ns_addf(&e2, dB.v[i], 0);
    if (!wl) for (int i = 0; i < e2.n; i++) e2.fl[i] |= 4;
    if (B->n > 0 && B->c[0]) rk_cap_walk(B->c[0], &e2);
    if (!encl || !encl->n) return;
    rk_ns_t used = { 0 };
    if (B->n > 0) rk_cap_uses(B->c[0], &used);
    rk_ns_t F = { 0 }, FR = { 0 };
    for (int i = 0; i < used.n; i++) {
        const char * u = used.v[i];
        if (!strcmp(u, "_") || u[0] == '^' || !strncmp(u, "__", 2) || rk_ns_has(&dB, u) || !rk_ns_has(encl, u)) continue;
        if (wl && (rk_ns_flag(encl, u) & 15) == 2) continue;
        rk_ns_add(&F, u);
        if (wl || (rk_ns_flag(encl, u) & 1)) rk_ns_add(&FR, u);
    }
    if (!F.n) return;
    const tree_t * body = B->c[0];
    tree_t ** pl = (tree_t **) ct_alloc(sizeof(tree_t *) * (size_t) (F.n + B->n + 24));
    int np = 0;
    for (int i = 0; i < F.n; i++) { tree_t * pv = leaf_sval2(TT_VAR, F.v[i]); if (rk_ns_has(&FR, F.v[i])) ast_push(pv, leaf_sval2(TT_QLIT, "@")); pl[np++] = pv; }
    if (B->n > 1) {
        for (int k = 1; k < B->n; k++) pl[np++] = B->c[k];
    } else {
        int topic = 0, nph = 0, phmax = rk_tree_size(body) + 1;
        const char ** ph = (const char **) ct_alloc(sizeof(const char *) * (size_t) phmax);
        rk_scan_implicit_params(body, &topic, ph, &nph, phmax);
        for (int a = 1; a < nph; a++) { const char * key = ph[a]; int b = a - 1; while (b >= 0 && rk_ph_cmp(ph[b], key) > 0) { ph[b + 1] = ph[b]; b--; } ph[b + 1] = key; }
        if (nph > 0) { for (int k = 0; k < nph; k++) pl[np++] = leaf_sval2(TT_VAR, ph[k]); } else if (topic) pl[np++] = leaf_sval2(TT_VAR, intern("_"));
    }
    rk_cap_assign_through(B->c[0], &FR);
    B->n = 1;
    for (int k = 0; k < np; k++) ast_push(B, pl[k]);
    tree_t * cl = ast_node_new(TT_FNC);
    cl->line = B->line;
    cl->v.sval = (char *) "__blk_close";
    ast_push(cl, leaf_sval2(TT_VAR, "__blk_close"));
    ast_push(cl, B);
    for (int i = 0; i < F.n; i++) {
        tree_t * av = leaf_sval2(TT_VAR, F.v[i]);
        if (rk_ns_has(&FR, F.v[i])) ast_push(cl, av);
        else { tree_t * bv = ast_node_new(TT_FNC); bv->v.sval = (char *) "__rk_byval"; ast_push(bv, leaf_sval2(TT_VAR, "__rk_byval")); ast_push(bv, av); ast_push(cl, bv); }
    }
    parent->c[idx] = cl;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_cap_walk(tree_t * t, const rk_ns_t * encl) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) {
        tree_t * c = t->c[i];
        if (!c) continue;
        if (c->t == TT_SUB_DECL) rk_cap_proc_m(c, rk_cap_is_package(t));
        else if (c->t == TT_ANON_BLOCK) rk_cap_block(t, i, encl);
        else rk_cap_walk(c, encl);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_cap_mutated_names(const tree_t * t, rk_ns_t * out) {
    if (!t) return;
    if ((t->t == TT_ASSIGN || t->t == TT_ARR_SET || t->t == TT_HASH_SET) && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR && t->c[0]->v.sval && !(t->t == TT_ASSIGN && (t->c[0]->slen & 4)))
        rk_ns_add(out, t->c[0]->v.sval);
    if (t->t == TT_METHCALL && t->n > 1 && t->c[0] && t->c[0]->t == TT_VAR && t->c[1] && rk_cap_mut_method(t->c[1]->v.sval)) rk_ns_add(out, t->c[0]->v.sval);
    if (t->t == TT_FNC && t->n > 1 && t->c[0] && rk_cap_mut_method(t->c[0]->v.sval) && t->c[1] && t->c[1]->t == TT_VAR) rk_ns_add(out, t->c[1]->v.sval);
    for (int i = 0; i < t->n; i++) rk_cap_mutated_names(t->c[i], out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_cap_block_written(const tree_t * t, const rk_ns_t * fsd, rk_ns_t * out) {
    if (!t || t->t == TT_SUB_DECL) return;
    if (t->t == TT_ANON_BLOCK) {
        rk_ns_t m = { 0 }, inner = { 0 };
        rk_cap_mutated_names(t, &m);
        rk_cap_declared_deep(t, &inner);
        for (int i = 0; i < m.n; i++) if (rk_ns_has(fsd, m.v[i]) && !rk_ns_has(&inner, m.v[i])) rk_ns_add(out, m.v[i]);
        return;
    }
    for (int i = 0; i < t->n; i++) rk_cap_block_written(t->c[i], fsd, out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_cap_is_loop(const tree_t * t) {
    return t && (t->t == TT_WHILE || t->t == TT_UNTIL || t->t == TT_REPEAT || t->t == TT_FOR || t->t == TT_EVERY || t->t == TT_FOR_RANGE || t->t == TT_DO_WHILE || t->t == TT_CLOOP);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_cap_iter_decls(const tree_t * t, int lp, rk_ns_t * out, rk_ns_t * lvs) {
    if (!t || t->t == TT_SUB_DECL || t->t == TT_ANON_BLOCK) return;
    int lp2 = lp || (rk_cap_is_loop(t) && t->t != TT_CLOOP);
    if (lp && t->t == TT_VAR && (t->slen & 4) && t->v.sval) rk_ns_add(out, t->v.sval);
    if (lp && t->t == TT_ITERATE && t->v.sval && strcmp(t->v.sval, "_")) { rk_ns_add(out, t->v.sval); rk_ns_add(lvs, t->v.sval); }
    if (lp2 && t->t == TT_FOR_RANGE && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR && t->c[0]->v.sval) { rk_ns_add(out, t->c[0]->v.sval); rk_ns_add(lvs, t->c[0]->v.sval); }
    for (int i = 0; i < t->n; i++) rk_cap_iter_decls(t->c[i], lp2 || (t->t == TT_CLOOP && i == 3), out, lvs);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_cap_sub_free(const tree_t * t, rk_ns_t * out) {
    if (!t) return;
    if (t->t == TT_SUB_DECL) {
        rk_ns_t u = { 0 }, d = { 0 };
        rk_cap_uses_deep(t, &u);
        rk_cap_declared_deep(t, &d);
        for (int i = 0; i < u.n; i++) if (!rk_ns_has(&d, u.v[i])) rk_ns_add(out, u.v[i]);
        return;
    }
    for (int i = 0; i < t->n; i++) rk_cap_sub_free(t->c[i], out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_cap_file_scope(tree_t * prog) {
    rk_ns_t it = { 0 }, lvs = { 0 }, subn = { 0 }, own = { 0 }, boxable = { 0 }, Bx = { 0 };
    rk_cap_iter_decls(prog, 0, &it, &lvs);
    rk_cap_sub_free(prog, &subn);
    rk_ns_t fsd = { 0 }, bw = { 0 };
    for (int i = 0; i < prog->n; i++) rk_cap_declared(prog->c[i], &fsd);
    rk_cap_block_written(prog, &fsd, &bw);
    for (int i = 0; i < it.n; i++) if (!rk_ns_has(&subn, it.v[i])) rk_ns_addf(&own, it.v[i], 2);
    for (int i = 0; i < bw.n; i++) if (!rk_ns_has(&subn, bw.v[i])) rk_ns_addf(&own, bw.v[i], 10);
    rk_cap_decl_assigns(prog, &boxable);
    for (int i = 0; i < boxable.n; ) { if (rk_ns_has(&lvs, boxable.v[i])) { boxable.v[i] = boxable.v[boxable.n - 1]; boxable.n--; } else i++; }
    rk_cap_boxes(prog, prog, -1, &own, &boxable, &Bx);
    rk_bind_names(prog, &Bx, &subn);
    for (int i = 0; i < Bx.n; i++) rk_ns_addf(&own, Bx.v[i], 1);
    rk_cap_walk(prog, &own);
    if (!Bx.n) { rk_bind_unbox(prog, &Bx); return; }
    rk_cap_assign_through(prog, &Bx);
    rk_cap_flag_boxed(prog, &Bx);
    rk_cap_box_decls(prog, &Bx);
    rk_bind_unbox(prog, &Bx);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_gw_boxed(const tree_t * t, rk_ns_t * out) {
    if (!t) return;
    if (t->t == TT_VAR && t->v.sval && (t->slen & RK_VF_BOXED)) rk_ns_add(out, t->v.sval);
    for (int i = 0; i < t->n; i++) rk_gw_boxed(t->c[i], out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_gw_collect(const tree_t * t, const rk_ns_t * fsd, const rk_ns_t * excl, rk_ns_t * W) {
    if (!t) return;
    if (t->t == TT_SUB_DECL) {
        rk_ns_t d = { 0 };
        rk_cap_declared_deep(t, &d);
        for (int k = 0; k < fsd->n; k++) {
            const char * v = fsd->v[k];
            if (rk_ns_has(&d, v) || rk_ns_has(excl, v) || rk_ns_has(W, v)) continue;
            for (int j = 1; j < t->n; j++) if (rk_cap_mutated(t->c[j], v)) { rk_ns_add(W, v); break; }
        }
    }
    for (int i = 0; i < t->n; i++) rk_gw_collect(t->c[i], fsd, excl, W);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_gw_rename(tree_t * t, const char * v, const char * to) {
    if (!t) return;
    if (t->t == TT_SUB_DECL || t->t == TT_ANON_BLOCK) { rk_ns_t d = { 0 }; rk_cap_declared_deep(t, &d); if (rk_ns_has(&d, v)) return; }
    if (t->t == TT_VAR && t->v.sval && !strcmp(t->v.sval, v)) t->v.sval = (char *) to;
    for (int i = (t->t == TT_FNC || t->t == TT_SUB_DECL) ? 1 : 0; i < t->n; i++) rk_gw_rename(t->c[i], v, to);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_globalize_file_scope_writes(tree_t * prog) {
    extern void global_register(const char * name);
    rk_ns_t fsd = { 0 }, it = { 0 }, lvs = { 0 }, W = { 0 };
    rk_cap_iter_decls(prog, 0, &it, &lvs);
    rk_gw_boxed(prog, &it);
    for (int i = 0; i < prog->n; i++) rk_cap_declared(prog->c[i], &fsd);
    for (int i = 0; i < fsd.n; ) {
        const char * v = fsd.v[i];
        if (!*v || v[0] == '&' || v[0] == '_' || v[0] == '*' || v[0] == '?' || v[0] == '!' || v[0] == '.' || !strncmp(v, "__", 2)) { fsd.v[i] = fsd.v[fsd.n - 1]; fsd.n--; } else i++;
    }
    rk_gw_collect(prog, &fsd, &it, &W);
    for (int i = 0; i < W.n; i++) { const char * v = W.v[i]; char nb[strlen(v) + 8]; snprintf(nb, sizeof nb, "%s__fs", v); char * to = lp_strdup(nb); rk_gw_rename(prog, v, to); global_register(to); }
}
static int rk_is_multi_decl(const tree_t * c) { return c && c->t == TT_SUB_DECL && c->n > 0 && c->c[0] && c->c[0]->v.sval && strchr(c->c[0]->v.sval, '$'); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_multi_captures(const tree_t * c, const tree_t * encl) {
    if (!encl) return 0;
    rk_ns_t used = { 0 }, own = { 0 }, outer = { 0 };
    for (int k = 1; k < c->n; k++) rk_cap_uses_deep(c->c[k], &used);
    rk_cap_declared_deep(c, &own);
    rk_cap_declared_deep(encl, &outer);
    for (int k = 0; k < used.n; k++) if (!rk_ns_has(&own, used.v[k]) && rk_ns_has(&outer, used.v[k])) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_hoist_multis_walk(rk_mhoist_t * h, tree_t * t, const tree_t * encl) {
    if (!t || t->t == TT_CLASS_DECL || t->t == TT_ROLE_DECL || t->t == TT_GRAMMAR_DECL) return;
    if (t->t == TT_SUB_DECL) encl = t;
    for (int i = 0; i < t->n; i++) {
        tree_t * c = t->c[i];
        if (rk_is_multi_decl(c)) {
            int top = 0;
            for (int k = 0; k < h->ntop; k++) if (h->top[k] == c) top = 1;
            if (!top && !rk_multi_captures(c, encl)) { h->out[h->nout++] = c; t->c[i] = ast_node_new(TT_SEQ_EXPR); rk_hoist_multis_walk(h, c, NULL); continue; }
        }
        rk_hoist_multis_walk(h, c, encl);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_count_multi_decls(const tree_t * t) { if (!t) return 0; int n = rk_is_multi_decl(t) ? 1 : 0; for (int i = 0; i < t->n; i++) n += rk_count_multi_decls(t->c[i]); return n; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_hoist_nested_multis(tree_t * prog) {
    if (!prog) return;
    int cap = rk_count_multi_decls(prog) + 1;
    const tree_t * top[cap];
    tree_t * out[cap];
    rk_mhoist_t h = { top, 0, out, 0 };
    for (int i = 0; i < prog->n; i++) {
        const tree_t * d = prog->c[i];
        if (d && d->t == TT_STMT) { const tree_t * sub = lrk_stmt_subj(d); if (sub) d = sub; }
        if (rk_is_multi_decl(d)) top[h.ntop++] = d;
    }
    rk_hoist_multis_walk(&h, prog, NULL);
    for (int i = 0; i < h.nout; i++) ast_push(prog, out[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_tv_classes(const tree_t * t, rk_ns_t * out) {
    if (!t) return;
    if ((t->t == TT_CLASS_DECL || t->t == TT_ROLE_DECL) && t->n > 0 && t->c[0] && t->c[0]->v.sval) rk_ns_add(out, t->c[0]->v.sval);
    for (int i = 0; i < t->n; i++) rk_tv_classes(t->c[i], out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_typed_vars(tree_t * prog) {
    rk_tvenv_t e = { 0 };
    rk_tv_classes(prog, &e.classes);
    rk_ss_collect(prog, &e.subsets);
    for (int i = 0; i < e.subsets.n; i++) rk_ns_add(&e.classes, e.subsets.v[i]);
    rk_tv_walk(prog, &e);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_bind_names(const tree_t * t, rk_ns_t * out, const rk_ns_t * skip) {
    if (!t || t->t == TT_ANON_BLOCK || t->t == TT_SUB_DECL) return;
    if (t->t == TT_VAR && (t->slen & RK_VF_RAW) && t->v.sval && !(skip && rk_ns_has(skip, t->v.sval))) rk_ns_add(out, t->v.sval);
    for (int i = 0; i < t->n; i++) rk_bind_names(t->c[i], out, skip);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_ss_collect(const tree_t * t, rk_ns_t * out) {
    if (!t) return;
    if (t->t == TT_SUB_DECL && t->n > 0 && t->c[0] && t->c[0]->v.sval && !strncmp(t->c[0]->v.sval, "__ssf_", 6)) rk_ns_add(out, t->c[0]->v.sval + 6);
    for (int i = 0; i < t->n; i++) rk_ss_collect(t->c[i], out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_shd_proc(const tree_t * t) { return t && (t->t == TT_SUB_DECL || t->t == TT_ANON_BLOCK || t->t == TT_CLASS_DECL || t->t == TT_ROLE_DECL || t->t == TT_GRAMMAR_DECL); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_shd_scope(const tree_t * t) { return rk_shd_proc(t) || (t && (t->t == TT_SEQ_EXPR || t->t == TT_CASE)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_shd_plain(const char * nm) {
    if (!nm) return 0;
    const char * b = (nm[0] == '@' || nm[0] == '%') ? nm + 1 : nm;
    if (!(isalpha((unsigned char) b[0]) || b[0] == '_') || (b[0] == '_' && (!b[1] || b[1] == '_'))) return 0;
    for (const char * c = b; *c; c++) if (!(isalnum((unsigned char) *c) || *c == '_' || *c == '-' || ((unsigned char) *c) >= 0x80)) return 0;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_shd_callees(const tree_t * t, rk_ns_t * out, int * dyn) {
    if (!t) return;
    if (t->t == TT_FNC && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR && t->c[0]->v.sval) {
        rk_ns_add(out, t->c[0]->v.sval);
        if (!strcmp(t->c[0]->v.sval, "EVAL") && !(t->n > 1 && t->c[1] && t->c[1]->t == TT_QLIT && t->c[1]->v.sval)) *dyn = 1;
    }
    for (int i = 0; i < t->n; i++) rk_shd_callees(t->c[i], out, dyn);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_shd_group(const tree_t * t, int * from) {
    if (!t || t->t != TT_FNC || !t->v.sval) return 0;
    if (!strcmp(t->v.sval, "__rk_destructure")) { *from = 2; return 1; }
    if (!strcmp(t->v.sval, "__rk_enum")) { *from = 1; return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_shd_collect(const tree_t * t, rk_ns_t * out, int deep) {
    int gf = 0;
    if (!t || rk_shd_proc(t) || ((t->t == TT_SEQ_EXPR || t->t == TT_CASE) && !deep)) return;
    if (rk_shd_group(t, &gf)) for (int i = gf; i < t->n; i++) if (t->c[i] && t->c[i]->t == TT_VAR && (t->c[i]->slen & 4) && rk_shd_plain(t->c[i]->v.sval)) rk_ns_add(out, t->c[i]->v.sval);
    if (t->t == TT_ASSIGN && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR && (t->c[0]->slen & 4) && rk_shd_plain(t->c[0]->v.sval)) rk_ns_add(out, t->c[0]->v.sval);
    if (t->t == TT_DECL && t->n > 1 && t->c[1] && t->c[1]->t == TT_VAR && (t->c[1]->slen & 4) && rk_shd_plain(t->c[1]->v.sval)) rk_ns_add(out, t->c[1]->v.sval);
    if (deep && t->t == TT_FOR_RANGE && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR && rk_shd_plain(t->c[0]->v.sval)) rk_ns_add(out, t->c[0]->v.sval);
    if (deep && t->t == TT_ITERATE && rk_shd_plain(t->v.sval)) rk_ns_add(out, t->v.sval);
    for (int i = 0; i < t->n; i++) rk_shd_collect(t->c[i], out, deep);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_shd_collect_body(const tree_t * c, int body, rk_ns_t * out, int deep) {
    if (body && c && c->t == TT_SEQ_EXPR) { for (int j = 0; j < c->n; j++) rk_shd_collect(c->c[j], out, deep); } else rk_shd_collect(c, out, deep);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * rk_shd_find(const rk_shd_t * sc, const char * nm) {
    for (; sc; sc = sc->up) for (int i = 0; i < sc->decl.n; i++) if (!strcmp(sc->decl.v[i], nm)) return sc->alias[i] ? sc->alias[i] : nm;
    return nm;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_shd_hides(const rk_shd_t * up, const char * nm, int beyond) {
    for (const rk_shd_t * e = up; e; e = e->up) { if (beyond ? (e->proc && rk_ns_has(&e->flat, nm)) : rk_ns_has(&e->decl, nm)) return 1; if (e->proc) beyond = 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_shd_child(tree_t * c, const rk_shd_t * sc, const rk_ns_t * callees, int * ctr) { if (rk_shd_scope(c)) rk_shd_enter(c, sc, callees, ctr); else rk_shd_walk(c, sc, callees, ctr); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_shd_idch(const char * s) { return isalnum((unsigned char) s[0]) || s[0] == '_' || ((unsigned char) s[0]) >= 0x80 || (s[0] == '-' && (isalpha((unsigned char) s[1]) || s[1] == '_')); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * rk_shd_eval_rewrite(const rk_shd_t * sc, const char * src) {
    size_t n = strlen(src), o = 0;
    char * out = (char *) ct_alloc(n * 8 + 64);
    for (size_t i = 0; i < n; ) {
        if ((src[i] == '$' || src[i] == '@' || src[i] == '%') && (isalpha((unsigned char) src[i + 1]) || src[i + 1] == '_' || ((unsigned char) src[i + 1]) >= 0x80)) {
            size_t k = i + 1;
            while (rk_shd_idch(src + k)) k++;
            char key[k - i + 2];
            const char * r;
            key[0] = src[i] == '$' ? 0 : src[i];
            memcpy(key + 1, src + i + 1, k - i - 1);
            key[k - i] = 0;
            r = rk_shd_find(sc, key[0] ? key : key + 1);
            if (r != (key[0] ? key : key + 1)) {
                out[o++] = src[i];
                for (const char * q = (r[0] == '@' || r[0] == '%') ? r + 1 : r; *q; q++) out[o++] = *q;
            } else {
                memcpy(out + o, src + i, k - i);
                o += k - i;
            }
            i = k;
            continue;
        }
        out[o++] = src[i++];
    }
    out[o] = 0;
    return out;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_shd_loopy(const tree_t * t) {
    if (!t || t->n < 2 || !t->c[0]) return 0;
    if (t->t == TT_FOR_RANGE) return t->n >= 4 && t->c[0]->t == TT_VAR && rk_shd_plain(t->c[0]->v.sval);
    return t->t == TT_EVERY && t->c[0]->t == TT_ITERATE && rk_shd_plain(t->c[0]->v.sval);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_shd_loop(tree_t * t, const rk_shd_t * sc, const rk_ns_t * callees, int * ctr) {
    tree_t * vn = t->c[0];
    const char * nm = vn->v.sval, * al[1] = { NULL };
    int from = t->t == TT_FOR_RANGE ? 3 : 1;
    rk_shd_t lp = { 0 };
    lp.up = sc;
    lp.alias = al;
    lp.dyn = sc->dyn;
    rk_ns_add(&lp.decl, nm);
    if (!sc->dyn && rk_shd_hides(sc, nm, 0) && !rk_ns_has(callees, nm)) { char nb[strlen(nm) + 24]; snprintf(nb, sizeof nb, "%s__shd%d", nm, ++*ctr); al[0] = intern(nb); vn->v.sval = (char *) al[0]; }
    for (int i = 0; i < vn->n; i++) rk_shd_child(vn->c[i], sc, callees, ctr);
    for (int i = 1; i < from && i < t->n; i++) rk_shd_child(t->c[i], sc, callees, ctr);
    for (int i = from; i < t->n; i++) rk_shd_child(t->c[i], &lp, callees, ctr);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_shd_walk(tree_t * t, const rk_shd_t * sc, const rk_ns_t * callees, int * ctr) {
    if (!t) return;
    if (rk_shd_loopy(t)) { rk_shd_loop(t, sc, callees, ctr); return; }
    if (t->t == TT_FNC && t->n > 1 && t->c[0] && t->c[0]->t == TT_VAR && t->c[0]->v.sval && !strcmp(t->c[0]->v.sval, "EVAL") && t->c[1] && t->c[1]->t == TT_QLIT && t->c[1]->v.sval) t->c[1]->v.sval =
        (char *) rk_shd_eval_rewrite(sc, t->c[1]->v.sval);
    if ((t->t == TT_VAR || t->t == TT_ITERATE) && rk_shd_plain(t->v.sval)) t->v.sval = (char *) rk_shd_find(sc, t->v.sval);
    for (int i = 0; i < t->n; i++) { if (t->t == TT_VAR && t->c[i] && t->c[i]->t == TT_SEQ_EXPR) rk_shd_walk(t->c[i], sc, callees, ctr); else rk_shd_child(t->c[i], sc, callees, ctr); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_shd_enter(tree_t * s, const rk_shd_t * up, const rk_ns_t * callees, int * ctr) {
    rk_shd_t me = { 0 };
    rk_ns_t mine = { 0 };
    int cas = s->t == TT_CASE, blk = s->t == TT_SEQ_EXPR || cas, abk = s->t == TT_ANON_BLOCK, bfrom = (blk && !cas) ? 0 : 1, bto = s->n, pfrom = 0, pto = 0;
    me.up = up;
    me.dyn = up ? up->dyn : 0;
    me.proc = !blk;
    if (s->t == TT_SUB_DECL) { pfrom = bfrom; while (bfrom < s->n && s->c[bfrom] && s->c[bfrom]->t == TT_VAR) bfrom++; pto = bfrom; }
    if (abk) { bfrom = 0; bto = 1; pfrom = 1; pto = 1; while (pto < s->n && s->c[pto] && s->c[pto]->t == TT_VAR) pto++; }
    for (int i = pfrom; i < pto; i++) {
        rk_ns_add(&me.decl, s->c[i]->v.sval);
        for (int j = 0; j < s->c[i]->n; j++) if (s->c[i]->c[j] && s->c[i]->c[j]->t == TT_VAR && (s->c[i]->c[j]->slen & RK_PV_ALIAS)) rk_ns_add(&me.decl, s->c[i]->c[j]->v.sval);
        rk_shd_subsig(s->c[i], &me.decl);
    }
    for (int i = 0; me.proc && i < me.decl.n; i++) rk_ns_add(&me.flat, me.decl.v[i]);
    for (int i = bfrom; i < bto && i < s->n; i++) rk_shd_collect_body(s->c[i], abk, &mine, 0);
    for (int i = bfrom; me.proc && i < bto && i < s->n; i++) rk_shd_collect_body(s->c[i], abk, &me.flat, 1);
    int np = me.decl.n;
    for (int i = 0; i < mine.n; i++) rk_ns_add(&me.decl, mine.v[i]);
    const char * alias[me.decl.n + 1];
    memset(alias, 0, sizeof alias);
    me.alias = alias;
    for (int i = np; i < me.decl.n; i++) {
        const char * nm = me.decl.v[i];
        if (!rk_shd_hides(up, nm, me.proc) || rk_ns_has(callees, nm) || (me.dyn && !me.proc)) continue;
        char nb[strlen(nm) + 24];
        snprintf(nb, sizeof nb, "%s__shd%d", nm, ++*ctr);
        alias[i] = intern(nb);
    }
    for (int i = 0; i < s->n; i++) {
        if (abk && i == 0 && s->c[i] && s->c[i]->t == TT_SEQ_EXPR) {
            for (int j = 0; j < s->c[i]->n; j++) rk_shd_child(s->c[i]->c[j], &me, callees, ctr);
        } else rk_shd_child(s->c[i], (cas && i == 0) ? up : &me, callees, ctr);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_shadow_rename(tree_t * prog) {
    rk_shd_t top = { 0 };
    rk_ns_t callees = { 0 };
    int ctr = 0;
    top.proc = 1;
    rk_shd_callees(prog, &callees, &top.dyn);
    for (int i = 0; prog && i < prog->n; i++) { rk_shd_collect(prog->c[i], &top.decl, 0); rk_shd_collect(prog->c[i], &top.flat, 1); }
    const char * alias[top.decl.n + 1];
    memset(alias, 0, sizeof alias);
    top.alias = alias;
    for (int i = 0; prog && i < prog->n; i++) rk_shd_child(prog->c[i], &top, &callees, &ctr);
}
