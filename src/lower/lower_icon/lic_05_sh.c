#include "lower/lower_icon/lic_head.h"
#include <string.h>
#include "ir/cset_block.h"
#include <math.h>
#include "ir/ir_node.h"
#include "ir/stage2_sm_prog_api.h"
#include "parsers/snobol4/scrip_cc.h"
#include "lower/lower_common.h"
#include "lower/lower_icon/lic_01_sh.h"
#include "lower/lower_icon/lic_03_sh.h"
#include "lower/lower_icon/lic_04_sh.h"
#include "lower/lower_icon/lic_05_sh.h"
#include "ir/ast_tt_capt_immed_asgn.h"
#include "ir/ast.h"
#include "ir/ast_node_new.h"
#include "runtime/builtins/gen_expr_t_defined.h"
extern const char * icn_builtin_nth(int);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int icn_field_hint(const tree_t * prog, const char * fn) {
    static const char * const lf[] = { "frame_elems", "frame_size", "gen_type", "frame_cap" };
    int at = -1;
    if (!prog || !fn) return 0;
    for (int i = 0; i < 4; i++) if (!strcmp(fn, lf[i])) return 0;
    for (int ci = 0; ci < prog->n; ci++) {
        const tree_t * s = prog->c[ci];
        if (!s || (s->t != TT_STMT && s->t != TT_END)) continue;
        const tree_t * r = stmt_attr_expr(stmt_attr_find(s, ":subj"));
        if (!r || r->t != TT_RECORD) continue;
        for (int k = 0; k < r->n; k++) if (r->c[k] && r->c[k]->v.sval && !strcmp(r->c[k]->v.sval, fn)) { if (at >= 0 && at != k) return 0; at = k; }
    }
    return at + 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void icn_field_var_store_only(IR_t * lv) { if (lv && lv->op == IR_FIELD_VAR && lv->seal > 0 && lv->seal < ICN_FIELD_PLAIN) lv->seal |= ICN_FIELD_PLAIN; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int icn_op_hands_line(tree_e k) {
    switch (k) {
        case TT_ASSIGN:
        case TT_REVASSIGN:
        case TT_SWAP:
        case TT_REVSWAP:
        case TT_AUGOP:
        case TT_MAKELIST:
        case TT_MNS:
        case TT_PLS:
        case TT_SIZE:
        case TT_NONNULL:
        case TT_NULL:
        case TT_NOT:
        case TT_RANDOM:
        case TT_CSET_COMPL:
        case TT_DEREF:
        case TT_ITERATE:
        return 1;
        default:
        return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int icn_const_step(const tree_t * s, int64_t * bits, int * isr) {
    if (!s) return 0;
    if (s->t == TT_ILIT) { *bits = s->v.ival; *isr = 0; return 1; }
    if (s->t == TT_FLIT) { double d = s->v.dval; memcpy(bits, &d, 8); *isr = 1; return 1; }
    if ((s->t == TT_MNS || s->t == TT_PLS) && s->n >= 1 && s->c[0]) {
        if (!icn_const_step(s->c[0], bits, isr)) return 0;
        if (s->t == TT_MNS) { if (*isr) { double d; memcpy(&d, bits, 8); d = -d; memcpy(bits, &d, 8); } else *bits = -(*bits); }
        return 1;
    }
    if (s->t == TT_POW && s->n >= 2 && s->c[0] && s->c[1]) {
        int64_t lb = 0, rb = 0;
        int li = 0, ri = 0;
        if (!icn_const_step(s->c[0], &lb, &li) || !icn_const_step(s->c[1], &rb, &ri)) return 0;
        if (!li && !ri) {
            if (lb == 0 && rb <= 0) return 0;
            if (rb >= 0) { int64_t acc = 1; for (int64_t k = 0; k < rb; k++) if (__builtin_mul_overflow(acc, lb, &acc)) return 0; *bits = acc; *isr = 0; return 1; }
            if (lb == 1) { *bits = 1; *isr = 0; return 1; }
            if (lb == -1) { *bits = (rb & 1) ? -1 : 1; *isr = 0; return 1; }
            *bits = 0;
            *isr = 0;
            return 1;
        }
        double la, ra, rv;
        if (li) memcpy(&la, &lb, 8);
        else la = (double) lb;
        if (ri) memcpy(&ra, &rb, 8);
        else ra = (double) rb;
        if (la < 0.0 || (la == 0.0 && ra <= 0.0)) return 0;
        if (!ri) { int64_t n = rb; double r = la; rv = 1.0; if (n < 0) { n = (-1) - n; r = 1.0 / r; rv = r; } while (n > 0) { if (n & 1) rv *= r; r *= r; n >>= 1; } } else rv = pow(la, ra);
        if (!isfinite(rv)) return 0;
        memcpy(bits, &rv, 8);
        *isr = 1;
        return 1;
    }
    if ((s->t == TT_ADD || s->t == TT_SUB || s->t == TT_MUL || s->t == TT_DIV || s->t == TT_MOD) && s->n >= 2 && s->c[0] && s->c[1]) {
        int64_t lb = 0, rb = 0;
        int li = 0, ri = 0;
        if (!icn_const_step(s->c[0], &lb, &li) || !icn_const_step(s->c[1], &rb, &ri)) return 0;
        if (!li && !ri) {
            int64_t r;
            if (s->t == TT_ADD) {
                if (__builtin_add_overflow(lb, rb, &r)) return 0;
            } else if (s->t == TT_SUB) {
                if (__builtin_sub_overflow(lb, rb, &r)) return 0;
            } else if (s->t == TT_MUL) {
                if (__builtin_mul_overflow(lb, rb, &r)) return 0;
            } else if (s->t == TT_DIV) {
                if (rb == 0 || (rb == -1 && lb == INT64_MIN)) return 0;
                r = lb / rb;
            } else {
                if (rb == 0 || (rb == -1 && lb == INT64_MIN)) return 0;
                r = lb % rb;
            }
            *bits = r;
            *isr = 0;
            return 1;
        }
        double la, ra, rv;
        if (li) memcpy(&la, &lb, 8);
        else la = (double) lb;
        if (ri) memcpy(&ra, &rb, 8);
        else ra = (double) rb;
        if (s->t == TT_ADD) rv = la + ra;
        else if (s->t == TT_SUB) rv = la - ra;
        else if (s->t == TT_MUL) rv = la * ra;
        else if (s->t == TT_DIV) { if (ra == 0.0) return 0; rv = la / ra; } else { if (ra == 0.0) return 0; rv = fmod(la, ra); }
        if (!isfinite(rv)) return 0;
        memcpy(bits, &rv, 8);
        *isr = 1;
        return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_arg_is_null(const tree_t * a) { return a && (a->t == TT_VAR || a->t == TT_KEYWORD) && a->v.sval && (!strcmp(a->v.sval, "&null") || !strcmp(a->v.sval, "null")); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * icn_seq_leaf(tree_e k, const char * s, int64_t iv) { tree_t * e = ast_node_new(k); if (s) { e->v.sval = (char *) s; e->slen = (int) strlen(s); } else e->v.ival = iv; return e; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_seq_variable_step(icx_t * cx, const tree_t * t, int argbase, int nargs, const tree_t * byt, IR_t * γ, IR_t * ω, IR_t ** res) {
    if (byt->t != TT_VAR) return NULL;
    const tree_t * fromt = (nargs > 0 && !icn_arg_is_null(t->c[argbase])) ? t->c[argbase] : NULL;
    tree_t * from = fromt ? (tree_t *) fromt : icn_seq_leaf(TT_ILIT, NULL, 1);
    tree_t * neg = ast_node_new(TT_LT);
    ast_push(neg, (tree_t *) byt);
    ast_push(neg, icn_seq_leaf(TT_ILIT, NULL, 0));
    tree_t * bound = ast_node_new(TT_IF);
    ast_push(bound, neg);
    ast_push(bound, icn_seq_leaf(TT_ILIT, NULL, INT64_MIN));
    ast_push(bound, icn_seq_leaf(TT_ILIT, NULL, INT64_MAX));
    tree_t * tby = ast_node_new(TT_TO_BY);
    ast_push(tby, from);
    ast_push(tby, bound);
    ast_push(tby, (tree_t *) byt);
    tby->line = t->line;
    return lower_to(cx, tby, γ, ω, res);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * lic_lower_seq(icx_t * cx, const tree_t * t, int argbase, int nargs, IR_t * γ, IR_t * ω, IR_t ** res) {
    int64_t by = 1;
    const tree_t * byt = (nargs > 1) ? t->c[argbase + 1] : NULL;
    if (icn_arg_is_null(byt)) byt = NULL;
    if (byt) { int64_t bb = 1; int isr = 0; if (!icn_const_step(byt, &bb, &isr)) return lower_seq_variable_step(cx, t, argbase, nargs, byt, γ, ω, res); if (isr) return NULL; by = bb; }
    int use_by = (by != 1);
    IR_t * to = lic_build(cx, use_by ? IR_TO_BY : IR_TO, γ, ω);
    IR_LIT(to).sval = (char *) "ag:int";
    cx->last_gen = to;
    const tree_t * fromt = (nargs > 0) ? t->c[argbase] : NULL;
    IR_t * lr;
    IR_t * ea;
    IR_t * lβ;
    if (icn_arg_is_null(fromt)) fromt = NULL;
    if (fromt) { ea = lic_lower(cx, fromt, NULL, ω, &lr); lβ = cx->beta; } else { lr = lic_build(cx, IR_LIT_INTEGER, NULL, ω); IR_LIT(lr).ival = 1; ea = lr; lβ = ω; }
    IR_t * mr = lic_build(cx, IR_LIT_INTEGER, to, lβ);
    IR_LIT(mr).ival = (by > 0) ? INT64_MAX : INT64_MIN;
    lic_γ_to(lr, mr);
    if (use_by) {
        IR_t * br = lic_build(cx, IR_LIT_INTEGER, to, lβ);
        IR_LIT(br).ival = by;
        lic_γ_to(mr, br);
        lc_γ_to_α(br, to);
        ir_operand_push(to, lr);
        ir_operand_push(to, mr);
        ir_operand_push(to, br);
    } else {
        lc_γ_to_α(mr, to);
        ir_operand_push(to, lr);
        ir_operand_push(to, mr);
    }
    cx->beta = to;
    *res = to;
    return ea;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * lower_key(icx_t * cx, const tree_t * t, int argbase, int nargs, IR_t * γ, IR_t * ω, IR_t ** res) {
    (void) nargs;
    IR_t * kg = lic_build(cx, IR_ITERATE, γ, ω);
    IR_LIT(kg).sval = (char *) (cx->prog && cx->prog->v.ival ? "ukey" : "key");
    IR_t * orr = NULL;
    IR_t * ee = lic_lower(cx, t->c[argbase], NULL, ω, &orr);
    ir_operand_push(kg, orr);
    lc_γ_to(orr, kg);
    cx->beta = kg;
    *res = kg;
    return ee;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * lower_function_gen(icx_t * cx, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_t * fg = lic_build(cx, IR_ITERATE, γ, ω);
    IR_t * ml = lic_build(cx, IR_MAKE_LIST, NULL, ω);
    IR_t * prev = NULL;
    IR_t * entry = NULL;
    for (int k = 0; icn_builtin_nth(k); k++) {
        IR_t * sn = lic_build(cx, IR_LIT_STRING, icn_builtin_nth(k + 1) ? NULL : ml, ω);
        IR_LIT(sn).sval = (char *) icn_builtin_nth(k);
        if (!entry) entry = sn;
        if (prev) lc_γ_to(prev, sn);
        prev = sn;
        ir_operand_push(ml, sn);
    }
    ir_operand_push(fg, ml);
    lc_γ_to(ml, fg);
    cx->beta = fg;
    *res = fg;
    return entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * lower_globalnames_gen(icx_t * cx, IR_t * γ, IR_t * ω, IR_t ** res) {
    int np = g_stage2.proc_count;
    int total = 0;
    for (int i = 0; i < np; i++) if (g_stage2.proc_table[i].name) total++;
    for (int i = 0; i < cx->ngn; i++) if (cx->gn[i] && !icn_is_proc(cx->gn[i])) total++;
    if (!total) return NULL;
    IR_t * fg = lic_build(cx, IR_ITERATE, γ, ω);
    IR_t * ml = lic_build(cx, IR_MAKE_LIST, NULL, ω);
    IR_t * prev = NULL;
    IR_t * entry = NULL;
    int emitted = 0;
    for (int k = 0; k < np + cx->ngn; k++) {
        const char * nm = k < np ? g_stage2.proc_table[k].name : (cx->gn[k - np] && !icn_is_proc(cx->gn[k - np]) ? cx->gn[k - np] : NULL);
        if (!nm) continue;
        IR_t * sn = lic_build(cx, IR_LIT_STRING, ++emitted == total ? ml : NULL, ω);
        IR_LIT(sn).sval = (char *) nm;
        if (!entry) entry = sn;
        if (prev) lc_γ_to(prev, sn);
        prev = sn;
        ir_operand_push(ml, sn);
    }
    ir_operand_push(fg, ml);
    lc_γ_to(ml, fg);
    cx->beta = fg;
    *res = fg;
    return entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * lower_make_list(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_t * ml = lic_build(cx, IR_MAKE_LIST, γ, ω);
    if (res) *res = ml;
    IR_t * prev = NULL;
    IR_t * entry = ml;
    IR_t * aω = ω;
    int nstage = 0;
    for (int k = 0; k < t->n; k++) if (icn_arg_stages(cx, t->c[k])) nstage++;
    IR_t * args_r[t->n > 0 ? t->n : 1];
    int staged[t->n > 0 ? t->n : 1];
    int entry_set = 0;
    for (int k = 0; k < t->n; k++) {
        const tree_t * a = t->c[k];
        IR_t * ar = NULL;
        IR_t * ae;
        staged[k] = 0;
        if (nstage && icn_tree_is_kw_var(a)) { staged[k] = 2; args_r[k] = NULL; continue; }
        if (nstage && icn_arg_stages(cx, a)) { cx->beta = aω; ae = lower_lvalue_var(cx, a, aω, &ar); if (ae && ar) staged[k] = 1; }
        IR_t * b4 = cx->beta;
        if (!staged[k]) {
            const tree_t * na = k < t->n - 1 ? t->c[k + 1] : NULL;
            if (na && na->line > 0 && (na->t == TT_KEYWORD || (na->t == TT_VAR && na->v.sval && na->v.sval[0] == '&'))) cx->after_line = na->line;
            ae = lic_lower(cx, a, (k == t->n - 1 && !nstage) ? ml : NULL, aω, &ar);
        }
        aω = (cx->beta == b4 && ar && !staged[k] && icn_tree_is_cursor_mover(a)) ? ar : cx->beta;
        if (!entry_set) { entry = ae; entry_set = 1; }
        if (prev) lc_γ_to(prev, ae);
        prev = ar;
        args_r[k] = ar;
    }
    for (int k = 0; k < t->n; k++) if (staged[k] == 2 || (staged[k] && args_r[k])) {
        IR_t * drf;
        if (icn_tree_is_kw_var(t->c[k])) {
            drf = lic_build(cx, IR_KW_ICON, NULL, aω);
            IR_LIT(drf).sval = (char *) t->c[k]->v.sval;
        } else {
            drf = lic_build(cx, IR_DEREF, NULL, aω);
            ir_operand_push(drf, args_r[k]);
        }
        if (prev) lc_γ_to(prev, drf);
        else entry = drf;
        prev = drf;
        args_r[k] = drf;
    }
    for (int k = 0; k < t->n; k++) if (args_r[k]) ir_operand_push(ml, args_r[k]);
    if (nstage && prev) lc_γ_to(prev, ml);
    cx->beta = aω;
    return entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * lower_to(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    int by = (t->t == TT_TO_BY && t->n > 2 && t->c[2]) ? 1 : 0;
    int nu = by && cx->prog && cx->prog->v.ival;
    IR_t * to = lic_build(cx, by ? IR_TO_BY : IR_TO, γ, ω);
    IR_LIT(to).sval = (char *) (nu ? "ar" : "ag:int");
    cx->last_gen = to;
    IR_t * lr = NULL;
    IR_t * ea = lic_lower(cx, t->c[0], NULL, ω, &lr);
    IR_t * lβ = cx->beta;
    IR_t * mr = NULL;
    IR_t * em = lic_lower(cx, t->c[1], by ? NULL : to, lβ, &mr);
    IR_t * cl = nu ? lic_build(cx, IR_COERCE_NUMERIC, NULL, ω) : lr;
    IR_t * cm = nu ? lic_build(cx, IR_COERCE_NUMERIC, NULL, ω) : mr;
    if (nu) {
        IR_LIT(cl).ival = 102 | COERCE_ERR_FAILURE_CONVERTIBLE;
        IR_LIT(cm).ival = 102 | COERCE_ERR_FAILURE_CONVERTIBLE;
        ir_operand_push(cl, lr);
        ir_operand_push(cm, mr);
        lc_γ_to(lr, cl);
        lc_γ_to(mr, cm);
    }
    lc_γ_to(cl, em);
    ir_operand_push(to, cl);
    ir_operand_push(to, cm);
    IR_t * last_op = mr;
    if (by) {
        IR_t * mβ = cx->beta;
        IR_t * br = NULL;
        IR_t * eb = lic_lower(cx, t->c[2], nu ? NULL : to, mβ, &br);
        IR_t * cb = nu ? lic_build(cx, IR_COERCE_NUMERIC, NULL, ω) : br;
        if (nu) { IR_LIT(cb).ival = 102 | COERCE_ERR_FAILURE_CONVERTIBLE; ir_operand_push(cb, br); lc_γ_to(br, cb); }
        lc_γ_to(cm, eb);
        (void) eb;
        if (cb && ir_is_generator_kind(to->op)) lc_γ_to(cb, to);
        ir_operand_push(to, cb);
        last_op = br;
    } else if (mr && ir_is_generator_kind(to->op)) lc_γ_to(mr, to);
    {
        IR_t * resume_op = NULL;
        if (by && last_op && ir_is_generator_kind(last_op->op)) resume_op = last_op;
        else if (mr && ir_is_generator_kind(mr->op)) resume_op = mr;
        else if (lr && ir_is_generator_kind(lr->op)) resume_op = lr;
        if (resume_op) lc_ω_to_β(to, resume_op);
    }
    cx->beta = to;
    *res = to;
    return ea;
}
