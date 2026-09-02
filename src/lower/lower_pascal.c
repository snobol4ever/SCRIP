#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lower.h"
#include "../parsers/pascal/pascal_driver.h"
#define PAS_MAX_SCOPE 64
typedef struct pas_scope_s {
    const char *        names[PAS_MAX_SCOPE];
    int                 n;
    int                 nparams;
    long long           byref;
    int                 has_children;
    const char *        proc_name;
    struct pas_scope_s * outer;
} pas_scope_t;
typedef struct { const char * name; IR_t * node; } pas_label_t;
typedef struct {
    IR_graph_t * g;
    pas_scope_t  sc;
    lc_vec       labels;
    int          npbt;
} pcx_t;
static lc_vec g_pas_proc_list   = { NULL, 0, 0, (int) sizeof(const tree_t *) };
static lc_vec g_pas_proc_parent = { NULL, 0, 0, (int) sizeof(const tree_t *) };
static int g_pas_has_nesting = 0;
#define PAS_PROC(i)   LC_AT(&g_pas_proc_list, const tree_t *, (i))
#define PAS_PARENT(i) LC_AT(&g_pas_proc_parent, const tree_t *, (i))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void γ_to(IR_t * nd, IR_t * t) { lc_γ_to(nd, t); }
static void ω_to(IR_t * nd, IR_t * t) { lc_ω_to(nd, t); }
static IR_t * build(pcx_t * cx, IR_e op, IR_t * γ, IR_t * ω) { return lc_build(cx->g, op, γ, ω); }
extern void global_register(const char * name);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pas_reg_var(const char * nm) { if (nm && nm[0]) global_register(lp_strdup(nm)); }
static int pas_in_real_proc(const pcx_t * cx);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pas_in_real_proc(const pcx_t * cx) { return cx && cx->sc.proc_name && strcmp(cx->sc.proc_name, "main") != 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int scope_slot(const pas_scope_t * sc, const char * name) {
    if (!name) return -1;
    for (int i = 0; i < sc->n; i++) if (sc->names[i] && !strcmp(sc->names[i], name)) return i;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * label_find(pcx_t * cx, const char * name) {
    if (!name) return NULL;
    for (int i = 0; i < cx->labels.n; i++) if (LC_AT(&cx->labels, pas_label_t, i).name && !strcmp(LC_AT(&cx->labels, pas_label_t, i).name, name)) return LC_AT(&cx->labels, pas_label_t, i).node;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_relop(tree_e tt) {
    switch (tt) { case TT_LT: case TT_LE: case TT_GT: case TT_GE: case TT_EQ: case TT_NE: return 1; default: return 0; }
}
static IR_t * lower(pcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pas_name_is_byref(pcx_t * cx, const char * name) {
    for (const pas_scope_t * s = &cx->sc; s; s = s->outer) { int sl = scope_slot(s, name); if (sl >= 0) return (int)((s->byref >> sl) & 1LL); }
    return 0;
}
extern int pas_is_agg_local(const char * name);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { int slot; int uplevel; int owner_level; const char * owner_proc; } pas_res_t;
static int pas_resolve2(pcx_t * cx, const char * name, pas_res_t * r) {
    r->slot = -1; r->uplevel = 0; r->owner_level = 0; r->owner_proc = NULL;
    const pas_scope_t * found_sc = NULL;
    for (const pas_scope_t * s = &cx->sc; s; s = s->outer) { int sl = scope_slot(s, name); if (sl >= 0) { r->slot = sl; found_sc = s; break; } }
    if (found_sc && found_sc != &cx->sc && found_sc->proc_name) { int lvl = 0; for (const pas_scope_t * s2 = found_sc; s2; s2 = s2->outer) lvl++; r->uplevel = 1; r->owner_level = lvl; r->owner_proc = found_sc->proc_name; }
    return r->slot >= 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pas_frame_node(pcx_t * cx, IR_e op, const char * name, const pas_res_t * r, IR_t * γ, IR_t * ω) {
    IR_t * nd = build(cx, op, γ, ω); IR_LIT(nd).sval = name; nd->seal = r->owner_level;
    IR_t * own = IR_node_alloc(cx->g, IR_LIT_NAME); IR_LIT(own).sval = r->owner_proc;
    ir_operand_push(nd, own);
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pas_read_node(pcx_t * cx, const char * name, IR_t * γ, IR_t * ω) {
    pas_res_t r; pas_resolve2(cx, name, &r);
    if (r.uplevel) return pas_frame_node(cx, IR_VAR_FRAME, name, &r, γ, ω);
    if (r.slot < 0 && !(pas_in_real_proc(cx) && !strncmp(name, "__pas_vptmp_", 12))) pas_reg_var(name);
    IR_t * nd = build(cx, IR_VAR, γ, ω); IR_LIT(nd).sval = name; return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_var(pcx_t * cx, const char * name, IR_t * γ, IR_t * ω) {
    return pas_read_node(cx, name, γ, ω);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_var_r(pcx_t * cx, const char * name, IR_t * γ, IR_t * ω, IR_t ** res) {
    if (pas_name_is_byref(cx, name)) {
        IR_t * dr = build(cx, IR_DEREF, γ, ω);
        IR_t * v = pas_read_node(cx, name, dr, ω);
        ir_operand_push(dr, v);
        if (res) *res = dr;
        return v;
    }
    IR_t * nd = lower_var(cx, name, γ, ω);
    if (res) *res = nd;
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_assign_var(pcx_t * cx, const char * name, IR_t * γ, IR_t * ω) {
    pas_res_t r; pas_resolve2(cx, name, &r);
    if (r.uplevel) return pas_frame_node(cx, IR_ASSIGN_FRAME, name, &r, γ, ω);
    if (r.slot < 0 && !(pas_in_real_proc(cx) && !strncmp(name, "__pas_vptmp_", 12))) pas_reg_var(name);
    IR_t * nd = build(cx, IR_ASSIGN, γ, ω); IR_LIT(nd).sval = name; return nd;
}
static IR_t * pas_cond(pcx_t * cx, const tree_t * t, IR_t * T, IR_t * F, IR_t ** res);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pas_mat(pcx_t * cx, const tree_t * e, IR_t * ω, IR_t ** res) {
    char * nm = (char *) malloc(16);
    snprintf(nm, 16, "__pbt%d", cx->npbt++);
    IR_t * v  = build(cx, IR_VAR, NULL, ω); IR_LIT(v).sval = nm;
    IR_t * at = build(cx, IR_ASSIGN, v, ω); IR_LIT(at).sval = nm;
    IR_t * af = build(cx, IR_ASSIGN, v, ω); IR_LIT(af).sval = nm;
    IR_t * n1 = build(cx, IR_LIT_INTEGER, at, ω); IR_LIT(n1).ival = 1; ir_operand_push(at, n1);
    IR_t * n0 = build(cx, IR_LIT_INTEGER, af, ω); IR_LIT(n0).ival = 0; ir_operand_push(af, n0);
    IR_t * ce = pas_cond(cx, e, n1, n0, NULL);
    *res = v;
    return ce ? ce : n1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pas_mat_rv(pcx_t * cx, const tree_t * e, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_t * v = NULL; IR_t * e2 = pas_mat(cx, e, ω, &v); γ_to(v, γ); *res = v; return e2;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_condish(const tree_t * t) { return t && (is_relop(t->t) || t->t == TT_NOT); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pas_cond(pcx_t * cx, const tree_t * t, IR_t * T, IR_t * F, IR_t ** res) {
    if (t && t->t == TT_NOT && t->n > 0) return pas_cond(cx, t->c[0], F, T, res);
    if (t && is_relop(t->t)) {
        IR_t * op = build(cx, IR_BINOP_TEST, T, F); IR_LIT(op).ival = lc_binop_code(t->t);
        IR_t * lr = NULL, * rr = NULL;
        IR_t * ea = lower(cx, (t->n > 0) ? t->c[0] : NULL, NULL, F, &lr);
        IR_t * eb = lower(cx, (t->n > 1) ? t->c[1] : NULL, op, F, &rr);
        γ_to(lr, eb);
        ir_operand_push(op, lr); ir_operand_push(op, rr);
        if (res) *res = op;
        return ea;
    }
    IR_t * op = build(cx, IR_BINOP_TEST, T, F); IR_LIT(op).ival = 10;
    IR_t * er = NULL;
    IR_t * ee = lower(cx, t, NULL, F, &er);
    IR_t * z = build(cx, IR_LIT_INTEGER, op, F); IR_LIT(z).ival = 0;
    γ_to(er, z);
    ir_operand_push(op, er); ir_operand_push(op, z);
    if (res) *res = op;
    return ee ? ee : z;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_binop(pcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    if (is_condish(t)) { IR_t * v = NULL; IR_t * e = pas_mat_rv(cx, t, γ, ω, &v); *res = v; return e; }
    const tree_t * lt = (t->n > 0) ? t->c[0] : NULL;
    const tree_t * rt = (t->n > 1) ? t->c[1] : NULL;
    int lm = is_condish(lt);
    int rm = is_condish(rt);
    IR_t * op = build(cx, IR_BINOP, γ, ω);
    IR_LIT(op).ival = lc_binop_code(t->t);
    IR_t * lr = NULL, * rr = NULL; IR_t * ea, * eb;
    if (lm) { IR_t * lv = NULL; ea = pas_mat_rv(cx, lt, NULL, ω, &lv); lr = lv; }
    else ea = lower(cx, lt, NULL, ω, &lr);
    if (rm) { IR_t * rv = NULL; eb = pas_mat_rv(cx, rt, op, ω, &rv); rr = rv; }
    else eb = lower(cx, rt, op, ω, &rr);
    γ_to(lr, eb);
    ir_operand_push(op, lr); ir_operand_push(op, rr);
    *res = op;
    return ea;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_unop(pcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    if (t->t == TT_NOT) { IR_t * v = NULL; IR_t * e = pas_mat_rv(cx, t, γ, ω, &v); *res = v; return e; }
    if (t->t == TT_PLS) return lower(cx, (t->n > 0) ? t->c[0] : NULL, γ, ω, res);
    if (t->t == TT_MNS) {
        IR_t * op = build(cx, IR_BINOP, γ, ω);
        IR_LIT(op).ival = lc_binop_code(TT_SUB);
        IR_t * z = build(cx, IR_LIT_INTEGER, NULL, ω); IR_LIT(z).ival = 0;
        IR_t * rr = NULL;
        IR_t * re = lower(cx, (t->n > 0) ? t->c[0] : NULL, op, ω, &rr);
        γ_to(z, re ? re : op);
        ir_operand_push(op, z); ir_operand_push(op, rr);
        *res = op;
        return z;
    }
    IR_t * op = build(cx, IR_UNOP, γ, ω);
    IR_LIT(op).ival = (long long) t->t;
    IR_t * cr = NULL;
    IR_t * child = lower(cx, (t->n > 0) ? t->c[0] : NULL, op, ω, &cr);
    ir_operand_push(op, cr);
    *res = op;
    return child ? child : op;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * pas_lc_leaf(tree_e k, const char * s) { tree_t * e = ast_node_new(k); e->v.sval = (char *) (s ? s : ""); return e; }
static tree_t * pas_lc_bin(tree_e k, tree_t * a, tree_t * b) { tree_t * e = ast_node_new(k); ast_push(e, a); ast_push(e, b); return e; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * pas_lc_clone(const tree_t * e) {
    if (!e) return NULL; tree_t * c = ast_node_new(e->t); c->v = e->v; if ((e->t == TT_VAR || e->t == TT_QLIT) && e->v.sval) c->v.sval = (char *) lp_strdup(e->v.sval);
    for (int i = 0; i < e->n; i++) ast_push(c, pas_lc_clone(e->c[i])); return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static uint64_t pas_callee_byref_mask(const char * name) {
    if (!name) return 0;
    for (int pi = 0; pi < g_stage2.proc_count; pi++) if (g_stage2.proc_table[pi].name && !strcmp(g_stage2.proc_table[pi].name, name)) return g_stage2.proc_table[pi].byref_mask;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * pas_vptmp_var(void) {
    static int g_pas_vptmp_n = 0;
    char buf[32]; snprintf(buf, sizeof buf, "__pas_vptmp_%d", g_pas_vptmp_n++);
    tree_t * v = ast_node_new(TT_VAR); v->v.sval = lp_strdup(buf); return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pas_call_args_brm(pcx_t * cx, IR_t * call, uint64_t brm, const tree_t * const * args, int nargs, IR_t * ω) {
    IR_t * entry = call; IR_t * prevres = NULL;
    for (int k = 0; k < nargs; k++) {
        IR_t * ar = NULL; IR_t * ae;
        if (((brm >> k) & 1ULL) && args[k] && args[k]->t == TT_VAR && args[k]->v.sval && !pas_name_is_byref(cx, args[k]->v.sval)) {
            IR_t * vr = build(cx, IR_VAR_REF, (k == nargs - 1) ? call : NULL, ω); IR_LIT(vr).sval = args[k]->v.sval;
            ae = vr; ar = vr;
        } else if (((brm >> k) & 1ULL) && args[k] && args[k]->t == TT_VAR && args[k]->v.sval) {
            IR_t * vr = build(cx, IR_VAR, (k == nargs - 1) ? call : NULL, ω); IR_LIT(vr).sval = args[k]->v.sval;
            ae = vr; ar = vr;
        } else ae = lower(cx, args[k], (k == nargs - 1) ? call : NULL, ω, &ar);
        if (k == 0) entry = ae ? ae : call;
        if (prevres) γ_to(prevres, ae ? ae : call);
        ir_operand_push(call, ar);
        prevres = ar;
    }
    return entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pas_call_args(pcx_t * cx, IR_t * call, double dv, const tree_t * const * args, int nargs, IR_t * ω) {
    (void) dv;
    return pas_call_args_brm(cx, call, 0, args, nargs, ω);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_call(pcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    const tree_t * c0 = (t->n > 0) ? t->c[0] : NULL;
    if (c0 && c0->v.sval && !strcmp(c0->v.sval, "arr_make")) {
        IR_t * nd = build(cx, IR_CALL, γ, ω); IR_LIT(nd).sval = "arr_make";
        IR_t * e = pas_call_args(cx, nd, 2.0, (const tree_t * const *) (t->n > 1 ? &t->c[1] : NULL), (t->n > 0) ? t->n - 1 : 0, ω);
        *res = nd; return e;
    }
    uint64_t brm = pas_callee_byref_mask(c0 ? c0->v.sval : NULL);
    if (brm) { int rw = 0; for (int i = 1; i < t->n; i++) { if (((brm >> (i - 1)) & 1ULL) && t->c[i] && t->c[i]->t == TT_IDX) { rw = 1; break; } }
        if (rw) {
            tree_t * seq = ast_node_new(TT_SEQ_EXPR);
            tree_t * call = ast_node_new(TT_FNC); ast_push(call, pas_lc_leaf(TT_VAR, c0 && c0->v.sval ? c0->v.sval : ""));
            tree_t * outs[64]; int nout = 0;
            for (int i = 1; i < t->n; i++) {
                tree_t * arg = t->c[i];
                if (((brm >> (i - 1)) & 1ULL) && arg && arg->t == TT_IDX && nout < 64) {
                    tree_t * tv = pas_vptmp_var();
                    ast_push(seq, pas_lc_bin(TT_ASSIGN, tv, arg));
                    ast_push(call, tv);
                    outs[nout++] = pas_lc_bin(TT_ASSIGN, pas_lc_clone(arg), pas_lc_clone(tv));
                } else ast_push(call, arg);
            }
            ast_push(seq, call);
            for (int i = 0; i < nout; i++) ast_push(seq, outs[i]);
            return lower(cx, seq, γ, ω, res);
        }
    }
    IR_t * nd = build(cx, IR_CALL, γ, ω);
    IR_t * e = pas_call_args_brm(cx, nd, brm, (const tree_t * const *) (t->n > 1 ? &t->c[1] : NULL), (t->n > 0) ? t->n - 1 : 0, ω);
    IR_LIT(nd).sval = (c0 && c0->v.sval) ? c0->v.sval : NULL;
    *res = nd; return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pas_lower_idx_assign_curried(pcx_t * cx, const tree_t * lhs, const tree_t * rhs, IR_t * γ, IR_t * ω, IR_t ** res) {
    const tree_t * base = lhs->c[0];
    const tree_t * idx  = (lhs->n > 1) ? lhs->c[1] : NULL;
    tree_t * tv = pas_vptmp_var();
    tree_t * read_asn = pas_lc_bin(TT_ASSIGN, pas_lc_clone(tv), pas_lc_clone(base));
    tree_t * idx1 = ast_node_new(TT_IDX); ast_push(idx1, pas_lc_clone(tv)); ast_push(idx1, pas_lc_clone(idx));
    tree_t * inner_asn = pas_lc_bin(TT_ASSIGN, idx1, pas_lc_clone(rhs));
    tree_t * writeback_asn = pas_lc_bin(TT_ASSIGN, pas_lc_clone(base), pas_lc_clone(tv));
    tree_t * seq = ast_node_new(TT_SEQ_EXPR);
    ast_push(seq, read_asn); ast_push(seq, inner_asn); ast_push(seq, writeback_asn);
    return lower(cx, seq, γ, ω, res);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_assign(pcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    const tree_t * lhs = (t->n > 0) ? t->c[0] : NULL;
    const tree_t * rhs = (t->n > 1) ? t->c[1] : NULL;
    const char * vname = NULL;
    if (lhs && lhs->t == TT_IDX) {
        const tree_t * base = (lhs->n > 0) ? lhs->c[0] : NULL;
        if (base && base->t == TT_FNC && base->n >= 2 && base->c[0] && base->c[0]->v.sval && !strcmp(base->c[0]->v.sval, "__pas_deref")) {
            IR_t * call = build(cx, IR_CALL, γ, ω); IR_LIT(call).sval = "__pas_field_set";
            const tree_t * av[3]; av[0] = (base->n > 1) ? base->c[1] : NULL; av[1] = (lhs->n > 1) ? lhs->c[1] : NULL; av[2] = rhs;
            IR_t * e = pas_call_args(cx, call, 2.0, av, 3, ω);
            *res = call; return e;
        }
        if (base && base->t == TT_IDX && !pas_is_nrec_idx(lhs)) return pas_lower_idx_assign_curried(cx, lhs, rhs, γ, ω, res);
        const char * bname = (base && base->t == TT_VAR) ? base->v.sval : NULL;
        if (bname && pas_name_is_byref(cx, bname)) {
            IR_t * asn = build(cx, IR_ASSIGN_VAR, γ, ω);
            IR_t * vr = pas_read_node(cx, bname, NULL, ω);
            IR_t * call = build(cx, IR_CALL, asn, ω); IR_LIT(call).sval = "arr_set_pure";
            IR_t * e;
            {
                const tree_t ** av = (const tree_t **) calloc((size_t) lhs->n + 1, sizeof(const tree_t *)); int an = 0;
                for (int k = 0; k < lhs->n; k++) av[an++] = lhs->c[k];
                av[an++] = rhs;
                e = pas_call_args(cx, call, 2.0, av, an, ω);
            }
            γ_to(vr, e ? e : asn);
            ir_operand_push(asn, vr); ir_operand_push(asn, call);
            *res = asn; return vr;
        }
        IR_t * asn = lower_assign_var(cx, bname, γ, ω);
        IR_t * call = build(cx, IR_CALL, asn, ω); IR_LIT(call).sval = "arr_set_pure";
        IR_t * e;
        {
            const tree_t ** av = (const tree_t **) calloc((size_t) lhs->n + 1, sizeof(const tree_t *)); int an = 0;
            for (int k = 0; k < lhs->n; k++) av[an++] = lhs->c[k];
            av[an++] = rhs;
            e = pas_call_args(cx, call, 2.0, av, an, ω);
        }
        ir_operand_push(asn, call);
        *res = asn; return e;
    }
    if (lhs && lhs->t == TT_FNC && lhs->n > 0 && lhs->c[0] && lhs->c[0]->v.sval && !strcmp(lhs->c[0]->v.sval, "__pas_deref")) {
        IR_t * call = build(cx, IR_CALL, γ, ω); IR_LIT(call).sval = "__pas_deref_set";
        const tree_t * av[2]; av[0] = (lhs->n > 1) ? lhs->c[1] : NULL; av[1] = rhs;
        IR_t * e = pas_call_args(cx, call, 2.0, av, 2, ω);
        *res = call; return e;
    }
    if (lhs && lhs->t == TT_VAR) vname = lhs->v.sval;
    else if (lhs && lhs->t == TT_FNC && lhs->n > 0 && lhs->c[0]) vname = lhs->c[0]->v.sval;
    if (vname && pas_name_is_byref(cx, vname)) {
        IR_t * asn = build(cx, IR_ASSIGN_VAR, γ, ω);
        IR_t * vr = pas_read_node(cx, vname, NULL, ω);
        IR_t * rr = NULL; IR_t * re;
        if (is_condish(rhs)) { re = pas_mat_rv(cx, rhs, asn, ω, &rr); }
        else re = lower(cx, rhs, asn, ω, &rr);
        γ_to(vr, re ? re : asn);
        ir_operand_push(asn, vr); ir_operand_push(asn, rr);
        *res = asn; return vr;
    }
    if (is_condish(rhs)) {
        IR_t * v = NULL;
        IR_t * asn = lower_assign_var(cx, vname, γ, ω);
        IR_t * me = pas_mat_rv(cx, rhs, asn, ω, &v);
        ir_operand_push(asn, v);
        *res = asn; return me;
    }
    IR_t * asn = lower_assign_var(cx, vname, γ, ω);
    IR_t * rr = NULL;
    IR_t * rentry = lower(cx, rhs, asn, ω, &rr);
    ir_operand_push(asn, rr);
    *res = asn;
    return rentry ? rentry : asn;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_if(pcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    const tree_t * cond   = (t->n > 0) ? t->c[0] : NULL;
    const tree_t * then_t = (t->n > 1) ? t->c[1] : NULL;
    const tree_t * els_t  = (t->n > 2) ? t->c[2] : NULL;
    IR_t * then_entry = lower(cx, then_t, γ, ω, NULL);
    IR_t * else_entry = els_t ? lower(cx, els_t, γ, ω, NULL) : γ;
    IR_t * cr = NULL;
    IR_t * ce = pas_cond(cx, cond, then_entry ? then_entry : γ, else_entry ? else_entry : γ, &cr);
    if (res) *res = cr;
    return ce;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_while(pcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    const tree_t * cond = (t->n > 0) ? t->c[0] : NULL;
    const tree_t * body = (t->n > 1) ? t->c[1] : NULL;
    IR_t * W = build(cx, IR_GOTO, γ, γ);
    IR_t * cr = NULL;
    IR_t * ce = pas_cond(cx, cond, NULL, W, &cr);
    IR_t * be = lower(cx, body, ce, ce, NULL);
    γ_to(cr, be ? be : ce);
    if (res) *res = W;
    return ce;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_for(pcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    const tree_t * var  = (t->n > 0) ? t->c[0] : NULL;
    const tree_t * from = (t->n > 1) ? t->c[1] : NULL;
    const tree_t * to   = (t->n > 2) ? t->c[2] : NULL;
    const tree_t * body = (t->n > 3) ? t->c[3] : NULL;
    const char * vname  = (var && var->t == TT_VAR) ? var->v.sval : NULL;
    int is_downto = (t->v.ival == 1);
    IR_t * cmp = build(cx, IR_BINOP_TEST, NULL, γ);
    IR_LIT(cmp).ival = is_downto ? 8 : 6;
    IR_t * lim_var = lower_var(cx, vname, NULL, ω);
    IR_t * tr = NULL;
    IR_t * to_entry = lower(cx, to, cmp, ω, &tr);
    γ_to(lim_var, to_entry ? to_entry : cmp);
    ir_operand_push(cmp, lim_var); ir_operand_push(cmp, tr);
    IR_t * inc_asn = lower_assign_var(cx, vname, lim_var, ω);
    IR_t * inc_op  = build(cx, IR_BINOP, inc_asn, ω);
    IR_LIT(inc_op).ival = is_downto ? 1 : 0;
    IR_t * iv  = lower_var(cx, vname, NULL, ω);
    IR_t * one = build(cx, IR_LIT_INTEGER, inc_op, ω); IR_LIT(one).ival = 1;
    γ_to(iv, one);
    ir_operand_push(inc_op, iv); ir_operand_push(inc_op, one);
    ir_operand_push(inc_asn, inc_op);
    IR_t * be = lower(cx, body, iv, iv, NULL);
    γ_to(cmp, be ? be : iv);
    IR_t * init_asn = lower_assign_var(cx, vname, lim_var, ω);
    IR_t * fr = NULL;
    IR_t * fe = lower(cx, from, init_asn, ω, &fr);
    ir_operand_push(init_asn, fr);
    if (res) *res = cmp;
    return fe ? fe : init_asn;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_repeat(pcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    const tree_t * body = (t->n > 0) ? t->c[0] : NULL;
    const tree_t * cond = (t->n > 1) ? t->c[1] : NULL;
    IR_t * cr = NULL;
    IR_t * ce = pas_cond(cx, cond, γ, NULL, &cr);
    IR_t * be = lower(cx, body, ce, ω, NULL);
    ω_to(cr, be ? be : ce);
    if (res) *res = cr;
    return be ? be : ce;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void seq_flatten(const tree_t * t, lc_vec * out) {
    for (int i = 0; i < t->n; i++) {
        const tree_t * s = t->c[i];
        if (s && s->t == TT_SEQ_EXPR) seq_flatten(s, out);
        else if (s) lc_vec_push(out, &s);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_seq(pcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    lc_vec stv; lc_vec_init(&stv, (int) sizeof(const tree_t *));
    seq_flatten(t, &stv);
    const tree_t ** st = (const tree_t **) stv.data; int k = stv.n;
    if (k == 0) { IR_t * s = build(cx, IR_GOTO, γ, γ); if (res) *res = s; return s; }
    IR_t * succ = γ; IR_t * entry = NULL; IR_t * lastres = NULL;
    for (int i = k - 1; i >= 0; i--) {
        IR_t * r = NULL;
        IR_t * e = lower(cx, st[i], succ, ω, &r);
        if (i == k - 1) lastres = r;
        if (e) { entry = e; succ = e; }
    }
    if (res) *res = lastres;
    return entry ? entry : build(cx, IR_SUCCEED, γ, ω);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_block(pcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω) {
    lc_vec stmv; lc_vec_init(&stmv, (int) sizeof(const tree_t *));
    for (int i = 0; i < t->n; i++) {
        const tree_t * s = t->c[i];
        if (!s) continue;
        if (s->t == TT_PROC_DECL) continue;
        if (s->t == TT_STMT) {
            const tree_t * sub = NULL;
            for (int j = 0; j < s->n; j++) {
                const tree_t * a = s->c[j];
                if (a && a->t == TT_ATTR && a->v.sval && !strcmp(a->v.sval, ":subj")) { sub = (a->n > 0) ? a->c[0] : NULL; break; }
            }
            if (!sub) continue;
            s = sub;
        }
        lc_vec_push(&stmv, &s);
    }
    const tree_t ** stmts = (const tree_t **) stmv.data; int k = stmv.n;
    if (k == 0) return build(cx, IR_GOTO, γ, γ);
    IR_t * succ = γ; IR_t * entry = NULL;
    for (int i = k - 1; i >= 0; i--) { entry = lower(cx, stmts[i], succ, ω, NULL); if (entry) succ = entry; }
    return entry ? entry : build(cx, IR_GOTO, γ, γ);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void scan_labels(pcx_t * cx, const tree_t * t, IR_t * fail) {
    if (!t) return;
    if (t->t == TT_LABEL_DEF && t->v.sval) {
        IR_t * lnd = IR_node_alloc(cx->g, IR_GOTO);
        ω_to(lnd, fail);
        pas_label_t e; e.name = t->v.sval; e.node = lnd;
        lc_vec_push(&cx->labels, &e);
    }
    if (t->t == TT_STMT) {
        for (int i = 0; i < t->n; i++) {
            const tree_t * a = t->c[i];
            if (a && a->t == TT_ATTR && a->v.sval && !strcmp(a->v.sval, ":lbl")) {
                for (int j = 0; j < a->n; j++) {
                    const tree_t * ln = a->c[j];
                    if (ln && ln->v.sval) {
                        IR_t * lnd = IR_node_alloc(cx->g, IR_GOTO);
                        ω_to(lnd, fail);
                        pas_label_t e; e.name = ln->v.sval; e.node = lnd;
                        lc_vec_push(&cx->labels, &e);
                    }
                }
            }
        }
    }
    for (int i = 0; i < t->n; i++) scan_labels(cx, t->c[i], fail);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower(pcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_t * dummy = NULL; if (!res) res = &dummy;
    if (!t) { IR_t * s = build(cx, IR_GOTO, γ, γ); *res = s; return s; }
    switch (t->t) {
    case TT_ILIT: { IR_t * nd = build(cx, IR_LIT_INTEGER, γ, ω); IR_LIT(nd).ival = t->v.ival; *res = nd; return nd; }
    case TT_FLIT: { IR_t * nd = build(cx, IR_LIT_REAL, γ, ω); IR_LIT(nd).dval = t->v.dval; *res = nd; return nd; }
    case TT_QLIT: { IR_t * nd = build(cx, IR_LIT_STRING, γ, ω); IR_LIT(nd).sval = t->v.sval; *res = nd; return nd; }
    case TT_VAR:  return lower_var_r(cx, t->v.sval, γ, ω, res);
    case TT_ADD: case TT_SUB: case TT_MUL: case TT_DIV: case TT_MOD: case TT_POW:
    case TT_LT:  case TT_LE:  case TT_GT:  case TT_GE:  case TT_EQ:  case TT_NE:
        return lower_binop(cx, t, γ, ω, res);
    case TT_MNS: case TT_PLS: case TT_NOT: case TT_SIZE:
        return lower_unop(cx, t, γ, ω, res);
    case TT_ASSIGN: return lower_assign(cx, t, γ, ω, res);
    case TT_IDX: {
        if (pas_is_nrec_idx(t) && t->n == 2 && t->c[0] && t->c[0]->t == TT_IDX && t->c[0]->n == 2) {
            const tree_t *inner = t->c[0];
            const tree_t *base_node = inner->c[0]; const tree_t *fi_node = inner->c[1]; const tree_t *ei_node = t->c[1];
            IR_t *nd = build(cx, IR_CALL, γ, ω); IR_LIT(nd).sval = "__pas_nrec_get";
            const tree_t *av[3]; av[0] = base_node; av[1] = fi_node; av[2] = ei_node;
            IR_t * e = pas_call_args(cx, nd, 2.0, av, 3, ω);
            *res = nd; return e;
        }
        IR_t *nd = build(cx, IR_CALL, γ, ω); IR_LIT(nd).sval = "arr_get";
        IR_t * e = pas_call_args(cx, nd, 2.0, (const tree_t * const *) t->c, t->n, ω);
        *res = nd; return e; }
    case TT_FNC:    return lower_call(cx, t, γ, ω, res);
    case TT_IF: case TT_UNLESS: return lower_if(cx, t, γ, ω, res);
    case TT_WHILE:  return lower_while(cx, t, γ, ω, res);
    case TT_FOR:    return lower_for(cx, t, γ, ω, res);
    case TT_REPEAT: return lower_repeat(cx, t, γ, ω, res);
    case TT_SEQ: case TT_SEQ_EXPR: return lower_seq(cx, t, γ, ω, res);
    case TT_PROGRAM: return lower_block(cx, t, γ, ω);
    case TT_STMT: {
        const tree_t * sub = NULL;
        IR_t * goto_s = NULL; IR_t * goto_f = NULL; IR_t * goto_u = NULL;
        const char * lbl = NULL;
        for (int i = 0; i < t->n; i++) {
            const tree_t * a = t->c[i];
            if (!a || a->t != TT_ATTR) continue;
            if (!a->v.sval) continue;
            if (!strcmp(a->v.sval, ":subj")) sub = (a->n > 0) ? a->c[0] : NULL;
            else if (!strcmp(a->v.sval, ":lbl") && a->n > 0 && a->c[0]) lbl = a->c[0]->v.sval;
            else if (!strcmp(a->v.sval, ":goS") && a->n > 0 && a->c[0]) { IR_t * tgt = label_find(cx, a->c[0]->v.sval); goto_s = tgt; }
            else if (!strcmp(a->v.sval, ":goF") && a->n > 0 && a->c[0]) { IR_t * tgt = label_find(cx, a->c[0]->v.sval); goto_f = tgt; }
            else if (!strcmp(a->v.sval, ":go")  && a->n > 0 && a->c[0]) { IR_t * tgt = label_find(cx, a->c[0]->v.sval); goto_u = tgt; }
        }
        IR_t * lbl_node = lbl ? label_find(cx, lbl) : NULL;
        if (goto_u && !sub) { if (lbl_node) γ_to(lbl_node, goto_u); *res = lbl_node ? lbl_node : goto_u; return lbl_node ? lbl_node : goto_u; }
        IR_t * s_γ = goto_s ? goto_s : γ;
        IR_t * f_ω = goto_f ? goto_f : ω;
        IR_t * result = sub ? lower(cx, sub, s_γ, f_ω, res) : build(cx, IR_SUCCEED, s_γ, f_ω);
        if (lbl_node) γ_to(lbl_node, result ? result : s_γ);
        return result;
    }
    case TT_LABEL_DEF: {
        IR_t * lnd = label_find(cx, t->v.sval);
        const tree_t * child = (t->n > 0) ? t->c[0] : NULL;
        IR_t * entry = child ? lower(cx, child, γ, ω, res) : NULL;
        if (lnd) { γ_to(lnd, entry ? entry : γ); return lnd; }
        return entry ? entry : build(cx, IR_SUCCEED, γ, ω);
    }
    case TT_GOTO_U: {
        const char * name = t->v.sval ? t->v.sval : (t->n > 0 && t->c[0] ? t->c[0]->v.sval : NULL);
        IR_t * tgt = label_find(cx, name);
        if (tgt) { IR_t * jmp = build(cx, IR_GOTO, tgt, tgt); *res = jmp; return jmp; }
        return build(cx, IR_SUCCEED, γ, ω);
    }
    default: { IR_t * s = build(cx, IR_GOTO, γ, γ); *res = s; return s; }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void build_scope(pas_scope_t * sc, const tree_t * pd, pas_scope_t * outer) {
    memset(sc, 0, sizeof *sc);
    sc->outer = outer;
    if (!pd || pd->t != TT_PROC_DECL) return;
    sc->proc_name = pd->v.sval ? pd->v.sval : ((pd->n > 0 && pd->c[0] && pd->c[0]->t == TT_VAR) ? pd->c[0]->v.sval : NULL);
    const tree_t * params = (pd->n > 1) ? pd->c[1] : NULL;
    const tree_t * locals = (pd->n > 0) ? pd->c[pd->n - 1] : NULL;
    if (locals && locals->t != TT_VLIST) locals = NULL;
    sc->byref = (params && params->t == TT_VLIST) ? params->v.ival : 0;
    if (params && params->t == TT_VLIST) for (int i = 0; i < params->n && sc->n < PAS_MAX_SCOPE; i++) { if (params->c[i] && params->c[i]->v.sval) sc->names[sc->n++] = params->c[i]->v.sval; }
    sc->nparams = sc->n;
    if (locals) for (int i = 0; i < locals->n && sc->n < PAS_MAX_SCOPE; i++) { if (locals->c[i] && locals->c[i]->v.sval) sc->names[sc->n++] = locals->c[i]->v.sval; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void collect_procs(const tree_t * body, const tree_t * parent) {
    if (!body) return;
    for (int i = 0; i < body->n; i++) {
        const tree_t * s = body->c[i];
        if (!s) continue;
        const tree_t * pd = NULL;
        if (s->t == TT_PROC_DECL) pd = s;
        else if (s->t == TT_STMT) {
            for (int j = 0; j < s->n; j++) {
                const tree_t * a = s->c[j];
                if (a && a->t == TT_ATTR && a->v.sval && !strcmp(a->v.sval, ":subj") && a->n > 0 && a->c[0] && a->c[0]->t == TT_PROC_DECL) { pd = a->c[0]; break; }
            }
        }
        if (pd) {
            lc_vec_push(&g_pas_proc_list, &pd);
            lc_vec_push(&g_pas_proc_parent, &parent);
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int proc_decl_level(const tree_t * pd) {
    if (!pd || pd->t != TT_PROC_DECL || pd->n < 1) return 1;
    const tree_t * locals = pd->c[pd->n - 1];
    if (locals && locals->t == TT_VLIST) return (int) locals->v.ival;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void assign_parents(void) {
    for (int i = 0; i < g_pas_proc_list.n; i++) {
        int my_level = proc_decl_level(PAS_PROC(i));
        PAS_PARENT(i) = NULL;
        if (my_level <= 1) continue;
        for (int j = i + 1; j < g_pas_proc_list.n; j++) {
            if (proc_decl_level(PAS_PROC(j)) == my_level - 1) { PAS_PARENT(i) = PAS_PROC(j); break; }
        }
        if (!PAS_PARENT(i)) {
            for (int j = i - 1; j >= 0; j--) {
                if (proc_decl_level(PAS_PROC(j)) == my_level - 1) { PAS_PARENT(i) = PAS_PROC(j); break; }
            }
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int lower_pascal_enum(const tree_t * prog, const tree_t ** out, int max) {
    g_pas_proc_list.n = 0; g_pas_proc_parent.n = 0;
    if (!prog) return 0;
    collect_procs(prog, NULL);
    assign_parents();
    if (!out) return g_pas_proc_list.n;
    int n = (g_pas_proc_list.n < max) ? g_pas_proc_list.n : max;
    for (int i = 0; i < n; i++) out[i] = PAS_PROC(i);
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static pas_scope_t * build_scope_chain(const tree_t * pd) {
    if (!pd) return NULL;
    const tree_t * parent_pd = NULL;
    for (int i = 0; i < g_pas_proc_list.n; i++) {
        if (PAS_PROC(i) == pd) { parent_pd = PAS_PARENT(i); break; }
    }
    pas_scope_t * outer = parent_pd ? build_scope_chain(parent_pd) : NULL;
    pas_scope_t * sc = (pas_scope_t *) calloc(1, sizeof(pas_scope_t));
    build_scope(sc, pd, outer);
    for (int i = 0; i < g_pas_proc_list.n; i++) if (PAS_PARENT(i) == pd) { sc->has_children = 1; break; }
    return sc;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * lower_pascal_proc(const tree_t * prog, const tree_t * pd) {
    IR_graph_t * g = IR_alloc(8192); pcx_t cx; memset(&cx, 0, sizeof cx); cx.g = g; lc_vec_init(&cx.labels, (int) sizeof(pas_label_t));
    scan_labels(&cx, pd, NULL);
    IR_t * succ = IR_node_alloc(g, IR_SUCCEED); IR_t * fail = IR_node_alloc(g, IR_FAIL);
    for (int li = 0; li < cx.labels.n; li++) ω_to(LC_AT(&cx.labels, pas_label_t, li).node, fail);
    pas_scope_t * sc = build_scope_chain(pd);
    if (sc) cx.sc = *sc;
    const tree_t * body = (pd->n > 2) ? pd->c[2] : NULL;
    int is_func = (pd->n > 3) && pd->c[3] && (pd->c[3]->t == TT_VAR);
    IR_t * top = succ;
    if (is_func) { IR_t * ret = build(&cx, IR_RETURN, succ, succ); IR_t * rv = build(&cx, IR_VAR, ret, fail); IR_LIT(rv).sval = pd->v.sval; ir_operand_push(ret, rv); top = rv; }
    else { IR_t * safe = build(&cx, IR_LIT_INTEGER, succ, succ); IR_LIT(safe).ival = 0; top = safe; }
    IR_t * body_gamma = top;
    IR_t * entry = lower(&cx, body, body_gamma, fail, NULL);
    if (!entry) entry = build(&cx, IR_GOTO, body_gamma, body_gamma);
    g->entry = entry; return g;
}
#include "stage2.h"
#include "bb_program.h"
extern int scope_get(Scope *sc, const char *name);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int lower_pascal_body(const tree_t *prog, const tree_t *proc) {
    IR_graph_t * ng = lower_pascal_proc(prog, proc);
    if (!ng || !ng->entry) return -1;
    return bb_program_add(&g_stage2.bbp, ng);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pascal_register_program(stage2_t * s2, const tree_t * prog) {
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
                int _pi = stage2_proc_grow(s2);
                s2->proc_table[_pi].name     = name;
                s2->proc_table[_pi].proc     = proc;
                s2->proc_table[_pi].entry_pc = -1;
                s2->proc_table[_pi].bb_idx   = -1;
                s2->proc_table[_pi].nparams  = (proc->t == TT_PROC_DECL && proc->n >= 2 ? proc->c[1]->n : 0);
                s2->proc_table[_pi].byref_mask = (proc->t == TT_PROC_DECL && proc->n >= 2 && proc->c[1]) ? (uint64_t)proc->c[1]->v.ival : 0;
                if (mod_idx >= 0) s2->module_registry.mods[mod_idx].nprocs++;
                if (strcmp(name, "main") == 0 && s2->module_registry.main_mod < 0) s2->module_registry.main_mod = mod_idx;
            }
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
stage2_t *lower_pascal_stage2(const tree_t *prog) {
    pascal_register_program(&g_stage2, prog);
    lower_pascal_enum(prog, NULL, 0);
    g_pas_has_nesting = 0;
    for (int pi = 0; pi < g_stage2.proc_count; pi++) {
        const tree_t *proc = (const tree_t *) g_stage2.proc_table[pi].proc;
        if (proc && proc->t == TT_PROC_DECL && (proc_decl_level(proc) > 1 || g_stage2.proc_table[pi].byref_mask)) { g_pas_has_nesting = 1; break; }
    }
    for (int pi = 0; pi < g_stage2.proc_count; pi++) {
        const tree_t *proc = (const tree_t *) g_stage2.proc_table[pi].proc;
        if (!proc || proc->t != TT_PROC_DECL) continue;
        if (g_stage2.proc_table[pi].bb_idx >= 0) continue;
        int bb_idx = lower_pascal_body(prog, proc);
        if (bb_idx >= 0) {
            g_stage2.proc_table[pi].bb_idx = bb_idx;
            g_stage2.bbp.table[bb_idx]->decl_level = (g_stage2.proc_table[pi].name && strcmp(g_stage2.proc_table[pi].name, "main") == 0) ? 0 : proc_decl_level(proc);
            g_stage2.proc_table[pi].proc_entry_node = g_stage2.bbp.table[bb_idx]->entry;
            const tree_t *plist = (proc->n >= 2) ? proc->c[1] : NULL;
            g_stage2.proc_table[pi].nparams = plist ? plist->n : 0;
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
            const tree_t *locals = (proc->n >= 1) ? proc->c[proc->n - 1] : NULL;
            if (locals && locals->t == TT_VLIST) {
                g_stage2.proc_table[pi].decl_level = (int) locals->v.ival;
                for (int k = 0; k < locals->n && sc->n < STAGE2_FRAME_SLOT_MAX; k++) {
                    const tree_t *lv = locals->c[k];
                    if (!lv || !lv->v.sval) continue;
                    if (scope_get(sc, lv->v.sval) >= 0) continue;
                    sc->e[sc->n].name = lp_strdup(lv->v.sval);
                    sc->e[sc->n].slot = sc->n;
                    sc->n++;
                }
            }
            g_stage2.bbp.table[bb_idx]->nparams = np;
            if (np > 0) {
                const char ** _pn = (const char **) calloc((size_t) np, sizeof(const char *));
                if (_pn) { for (int k = 0; k < np && k < sc->n; k++) _pn[k] = sc->e[k].name; g_stage2.bbp.table[bb_idx]->pnames = _pn; }
            }
        }
    }
    { static int _zf = -1; if (_zf < 0) { const char *_e = getenv("SCRIP_PAS_ZFRAME"); _zf = (_e && *_e == '0') ? 0 : 1; }
      if (_zf) {
          int _mx = -1; for (int _pi = 0; _pi < g_stage2.proc_count; _pi++) if (g_stage2.proc_table[_pi].name && strcmp(g_stage2.proc_table[_pi].name, "main") == 0) { _mx = g_stage2.proc_table[_pi].bb_idx; break; }
          for (int _gi = 0; _gi < g_stage2.bbp.count; _gi++) if (g_stage2.bbp.table[_gi] && _gi != _mx) g_stage2.bbp.table[_gi]->zframe_graph = 1; } }
    return &g_stage2;
}
