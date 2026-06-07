/*====================================================================================================================*/
/*====================================================================================================================*/
#include "lower_internal.h"
#include "IR_interp_state.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gc/gc.h>
/*--------------------------------------------------------------------------------------------------------------------*/
int g_icn_postfix_resume = 0;
int g_icn_globals_nv     = 1;
/*====================================================================================================================*/
/*====================================================================================================================*/
static int icn_proc_is_generator(const char * name) {
    if (!name) return 0;
    for (int i = 0; i < g_stage2.proc_count; i++)
        if (g_stage2.proc_table[i].name && strcmp(g_stage2.proc_table[i].name, name) == 0)
            return g_stage2.proc_table[i].is_generator;
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int icn_is_global(const char * name) { return name ? is_global(name) : 0; }
/*--------------------------------------------------------------------------------------------------------------------*/
static lcx_t icn_bounded(lcx_t cx) { cx.bounded = 1; return cx; }
/*--------------------------------------------------------------------------------------------------------------------*/
static lcx_t icn_with_loop(lcx_t cx, IR_t * lω, IR_t * lnext) { cx.loop_ω = lω; cx.loop_next = lnext; return cx; }
/*--------------------------------------------------------------------------------------------------------------------*/
/* icn_subgraph DELETED — use lower_value_subgraph() directly (BUG-A fix 2026-06-06) */
/*====================================================================================================================*/
/*====================================================================================================================*/
static tree_e icn_augop_binop_tt(AugOp_e a) {
    switch (a) {
    case AUGOP_ADD: return TT_ADD;  case AUGOP_SUB: return TT_SUB;  case AUGOP_MUL: return TT_MUL;
    case AUGOP_DIV: return TT_DIV;  case AUGOP_MOD: return TT_MOD;  case AUGOP_POW: return TT_POW;
    case AUGOP_CONCAT: return TT_CAT;
    case AUGOP_EQ: return TT_EQ;    case AUGOP_LT: return TT_LT;    case AUGOP_LE: return TT_LE;
    case AUGOP_GT: return TT_GT;    case AUGOP_GE: return TT_GE;    case AUGOP_NE: return TT_NE;
    case AUGOP_SEQ: return TT_LEQ;  case AUGOP_SLT: return TT_LLT;  case AUGOP_SLE: return TT_LLE;
    case AUGOP_SGT: return TT_LGT;  case AUGOP_SGE: return TT_LGE;  case AUGOP_SNE: return TT_LNE;
    default: return (tree_e)0;
    }
}
/*====================================================================================================================*/
/*====================================================================================================================*/
/* icn_every DELETED — v_every in lower.c handles TT_EVERY for all languages (BUG-B fix 2026-06-06) */
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * icn_loop_break(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    (void) γ_in; (void) e;
    IR_t * br = nalloc(cx, IR_BREAK);
    if (!br) return NULL;
    IR_t * tgt = cx.loop_ω ? cx.loop_ω : ω_in;
    set_succ_fail(br, tgt, tgt);
    return ret(br, α_out, β_out, br, tgt);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * icn_loop_next(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    (void) γ_in; (void) e;
    IR_t * nx = nalloc(cx, IR_NEXT);
    if (!nx) return NULL;
    IR_t * tgt = cx.loop_next ? cx.loop_next : ω_in;
    set_succ_fail(nx, tgt, tgt);
    return ret(nx, α_out, β_out, nx, tgt);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * icn_det_call(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (e->n < 1 || !e->c[0] || e->c[0]->t != TT_VAR || !e->c[0]->v.sval) return NULL;
    const char * fn = e->c[0]->v.sval;
    IR_t * call = nalloc(cx, IR_CALL);
    if (!call) return NULL;
    IR_LIT(call).sval = fn;
    int nargs = e->n - 1;
    IR_LIT(call).ival = (int64_t) nargs;
    IR_LIT(call).dval = 3.0;
    int allow_gen = icn_proc_is_generator(fn);
    if (nargs > 0) {
        IR_graph_t ** blks = (IR_graph_t **) calloc((size_t) nargs, sizeof(IR_graph_t *));
        if (!blks) return NULL;
        lcx_t mv = icn_bounded(cx);
        for (int i = 0; i < nargs; i++) {
            blks[i] = lower_value_subgraph(mv, e->c[i + 1]);
            if (!blks[i]) { free(blks); return NULL; }
        }
        IR_EXEC(call).counter = (int64_t)(intptr_t) blks;
    }
    IR_t * call_beta = allow_gen ? call : ω_in;
    set_succ_fail(call, γ_in, ω_in);
    return ret(call, α_out, β_out, call, call_beta);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * icn_assign(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    const tree_t * lhs_t = NULL, * rhs_t = NULL;
    if (!tm(e, TT_ASSIGN, 2, &lhs_t, &rhs_t)) return NULL;
    if (!lhs_t || !rhs_t) return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    if (lhs_t->t == TT_IDX && lhs_t->n >= 2 && lhs_t->c[0] && lhs_t->c[1]) {
        IR_t * idx = nalloc(cx, IR_IDX_SET);
        if (!idx) return NULL;
        IR_t * bα = NULL, * bβ = NULL;
        IR_t * base = lower_program(icn_bounded(cx), lhs_t->c[0], idx, ω_in, &bα, &bβ);
        if (!base) return NULL;
        idx->α = bα ? bα : base;
        IR_t * kα = NULL, * kβ = NULL;
        IR_t * key = lower_program(icn_bounded(cx), lhs_t->c[1], idx, ω_in, &kα, &kβ);
        if (!key) return NULL;
        idx->β = kα ? kα : key;
        IR_t * rα = NULL, * rβ = NULL;
        IR_t * rhs = lower_program(icn_bounded(cx), rhs_t, idx, ω_in, &rα, &rβ);
        if (!rhs) return NULL;
        idx->β->γ = rα ? rα : rhs;
        set_succ_fail(idx, γ_in, ω_in);
        return ret(idx, α_out, β_out, idx->α, ω_in);
    }
    if (lhs_t->t == TT_FIELD && lhs_t->n >= 2 && lhs_t->c[0] && lhs_t->c[1] && lhs_t->c[1]->v.sval) {
        IR_t * fs = nalloc(cx, IR_FIELD_SET);
        if (!fs) return NULL;
        IR_LIT(fs).sval = lhs_t->c[1]->v.sval;
        IR_t * aα = NULL, * aβ = NULL;
        IR_t * obj = lower_program(icn_bounded(cx), lhs_t->c[0], fs, ω_in, &aα, &aβ);
        if (!obj) return NULL;
        fs->α = aα ? aα : obj;
        IR_t * rα = NULL, * rβ = NULL;
        IR_t * rhs = lower_program(icn_bounded(cx), rhs_t, fs, ω_in, &rα, &rβ);
        if (!rhs) return NULL;
        fs->β = rα ? rα : rhs;
        set_succ_fail(fs, γ_in, ω_in);
        return ret(fs, α_out, β_out, fs->α, ω_in);
    }
    if (lhs_t->t != TT_VAR) return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    IR_t * as = nalloc(cx, IR_ASSIGN);
    if (!as) return NULL;
    IR_LIT(as).sval = lhs_t->v.sval ? lhs_t->v.sval : "";
    IR_t * rα = NULL, * rβ = NULL;
    IR_t * rhs = lower_program(cx, rhs_t, as, ω_in, &rα, &rβ);
    if (!rhs) return NULL;
    as->α = rhs;
    set_succ_fail(as, γ_in, ω_in);
    IR_t * resume = (!cx.bounded && rβ && rβ != ω_in) ? rβ : ω_in;
    return ret(as, α_out, β_out, rα, resume);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * icn_scan(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    const tree_t * isubj_t = NULL, * ibody_t = NULL;
    if (!tm(e, TT_SCAN, 2, &isubj_t, &ibody_t) || !isubj_t || !ibody_t) return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    IR_t * gs = nalloc(cx, IR_GEN_SCAN);
    if (!gs) return NULL;
    IR_graph_t * subj_sg = lower_value_subgraph(cx, isubj_t);
    if (!subj_sg) return NULL;
    IR_graph_t * body_sg = lower_value_subgraph(cx, ibody_t);
    if (!body_sg) return NULL;
    IR_EXEC(gs).counter = (int64_t)(intptr_t) subj_sg;
    IR_LIT(gs).ival    = (int64_t)(intptr_t) body_sg;
    IR_LIT(gs).dval    = 1.0;
    set_succ_fail(gs, γ_in, ω_in);
    return ret(gs, α_out, β_out, gs, ω_in);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * icn_return(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    IR_t * rn = nalloc(cx, IR_RETURN);
    if (!rn) return NULL;
    IR_LIT(rn).dval = 0.0;
    IR_t * vα = NULL, * vβ = NULL;
    if (e->n >= 1 && e->c[0]) {
        lcx_t vc = cx; vc.role = ROLE_VALUE;
        IR_t * v = lower_program(vc, e->c[0], rn, ω_in, &vα, &vβ);
        if (!v) return NULL;
        rn->α = v;
    }
    set_succ_fail(rn, γ_in, ω_in);
    return ret(rn, α_out, β_out, vα ? vα : rn, ω_in);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * icn_suspend(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    IR_t * sn = nalloc(cx, IR_SUSPEND);
    if (!sn) return NULL;
    IR_LIT(sn).dval = 1.0;
    if (e->n >= 1 && e->c[0]) {
        IR_graph_t * eblk = lower_value_subgraph(cx, e->c[0]);
        if (!eblk) return NULL;
        IR_EXEC(sn).counter = (int64_t)(intptr_t) eblk;
    }
    if (e->n >= 2 && e->c[1]) {
        IR_graph_t * bblk = lower_value_subgraph(cx, e->c[1]);
        if (!bblk) return NULL;
        IR_LIT(sn).ival = (int64_t)(intptr_t) bblk;
    }
    set_succ_fail(sn, γ_in, ω_in);
    return ret(sn, α_out, β_out, sn, ω_in);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * icn_initial(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    IR_t * ini = nalloc(cx, IR_INITIAL);
    if (!ini) return NULL;
    if (e->n >= 1 && e->c[0]) {
        IR_t * bα = NULL, * bβ = NULL;
        lcx_t bc = icn_bounded(cx);
        IR_t * body = lower_program(bc, e->c[0], NULL, ω_in, &bα, &bβ);
        if (!body) return NULL;
        ini->α = bα ? bα : body;
        if (!body->γ) body->γ = γ_in;
        if (!body->ω) body->ω = γ_in;
    }
    set_succ_fail(ini, γ_in, ω_in);
    return ret(ini, α_out, β_out, ini, ω_in);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * icn_limit(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (e->n < 2 || !e->c[0] || !e->c[1]) return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    IR_t * lim = nalloc(cx, IR_LIMIT);
    if (!lim) return NULL;
    IR_t * bα = NULL, * bβ = NULL;
    IR_t * body = lower_program(cx, e->c[0], lim, ω_in, &bα, &bβ);
    if (!body) return NULL;
    lim->α = bα ? bα : body;
    IR_t * cα = NULL, * cβ = NULL;
    IR_t * cnt = lower_program(icn_bounded(cx), e->c[1], lim, ω_in, &cα, &cβ);
    if (!cnt) return NULL;
    lim->β = cnt;
    set_succ_fail(lim, γ_in, ω_in);
    IR_t * entry = cα ? cα : cnt;
    return ret(lim, α_out, β_out, entry, lim);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * icn_case(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (e->n < 1 || !e->c[0]) return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    IR_t * cas = nalloc(cx, IR_CASE);
    if (!cas) return NULL;
    lcx_t bv = icn_bounded(cx);
    IR_t * sα = NULL, * sβ = NULL;
    IR_t * sel = lower_program(bv, e->c[0], NULL, ω_in, &sα, &sβ);
    if (!sel) return NULL;
    IR_t * sel_entry = sα ? sα : sel;
    cas->α = sel_entry;
    sel->γ = cas;
    IR_t * chain = NULL;
    IR_t * prev_key = NULL;
    int i = 1;
    while (i < e->n) {
        int remaining = e->n - i;
        if (remaining == 1) {
            const tree_t * def_t = e->c[i]; i++;
            if (!def_t) continue;
            IR_t * dα = NULL, * dβ = NULL;
            IR_t * def_nd = lower_program(bv, def_t, NULL, ω_in, &dα, &dβ);
            if (!def_nd) continue;
            def_nd->γ = γ_in;
            IR_t * def_key = nalloc(cx, IR_LIT_NUL);
            if (!def_key) continue;
            def_key->γ = dα ? dα : def_nd;
            def_key->ω = NULL;
            if (!chain) chain = def_key;
            if (prev_key) prev_key->ω = def_key;
            prev_key = def_key;
        } else {
            const tree_t * key_t = e->c[i++];
            const tree_t * val_t = (i < e->n) ? e->c[i++] : NULL;
            if (!key_t) continue;
            IR_t * kα = NULL, * kβ = NULL;
            IR_t * key_nd = lower_program(bv, key_t, NULL, ω_in, &kα, &kβ);
            if (!key_nd) continue;
            IR_t * key_entry = kα ? kα : key_nd;
            IR_t * val_entry = NULL;
            if (val_t) {
                IR_t * vα = NULL, * vβ = NULL;
                IR_t * val_nd = lower_program(bv, val_t, NULL, ω_in, &vα, &vβ);
                if (val_nd) {
                    val_nd->γ = γ_in;
                    val_entry = vα ? vα : val_nd;
                }
            }
            key_nd->γ = NULL;
            key_nd->ω = NULL;
            IR_t * arm_key = nalloc(cx, IR_LIT_NUL);
            if (!arm_key) continue;
            arm_key->γ = key_entry;
            arm_key->β = val_entry;
            arm_key->ω = NULL;
            if (!chain) chain = arm_key;
            if (prev_key) prev_key->ω = arm_key;
            prev_key = arm_key;
        }
    }
    cas->β = chain;
    set_succ_fail(cas, γ_in, ω_in);
    return ret(cas, α_out, β_out, sel_entry, ω_in);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * icn_swap(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (e->n < 2 || !e->c[0] || !e->c[1]) return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    IR_t * sw = nalloc(cx, IR_SWAP);
    if (!sw) return NULL;
    IR_t * lv = nalloc(cx, IR_VAR);
    if (!lv) return NULL;
    IR_LIT(lv).sval = (e->c[0]->t == TT_VAR && e->c[0]->v.sval) ? e->c[0]->v.sval : "";
    lv->γ = sw; lv->ω = ω_in;
    IR_t * rv = nalloc(cx, IR_VAR);
    if (!rv) return NULL;
    IR_LIT(rv).sval = (e->c[1]->t == TT_VAR && e->c[1]->v.sval) ? e->c[1]->v.sval : "";
    rv->γ = sw; rv->ω = ω_in;
    sw->α = lv;
    sw->β = rv;
    set_succ_fail(sw, γ_in, ω_in);
    return ret(sw, α_out, β_out, lv, ω_in);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
static IR_t * icn_field_get(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    const char * fname = NULL;
    const tree_t * obj_t = NULL;
    if (e->n >= 2 && e->c[0] && e->c[1] && e->c[1]->v.sval) { obj_t = e->c[0]; fname = e->c[1]->v.sval; }
    else if (e->n >= 1 && e->c[0] && e->v.sval)              { obj_t = e->c[0]; fname = e->v.sval; }
    if (!fname || !obj_t) return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    IR_t * fg = nalloc(cx, IR_FIELD_GET);
    if (!fg) return NULL;
    IR_LIT(fg).sval = fname;
    IR_t * aα = NULL, * aβ = NULL;
    IR_t * obj = lower_program(icn_bounded(cx), obj_t, fg, ω_in, &aα, &aβ);
    if (!obj) return NULL;
    fg->α = aα ? aα : obj;
    set_succ_fail(fg, γ_in, ω_in);
    return ret(fg, α_out, β_out, fg->α, ω_in);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * icn_section(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (e->n < 3 || !e->c[0] || !e->c[1] || !e->c[2]) return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    IR_t * sec = nalloc(cx, IR_SECTION);
    if (!sec) return NULL;
    IR_LIT(sec).ival = (e->t == TT_SECTION_PLUS) ? 1 : (e->t == TT_SECTION_MINUS) ? 2 : 0;
    IR_t * aα = NULL, * aβ = NULL;
    IR_t * obj = lower_program(icn_bounded(cx), e->c[0], sec, ω_in, &aα, &aβ);
    if (!obj) return NULL;
    sec->α = aα ? aα : obj;
    IR_t * bα = NULL, * bβ = NULL;
    IR_t * i1 = lower_program(icn_bounded(cx), e->c[1], sec, ω_in, &bα, &bβ);
    if (!i1) return NULL;
    sec->β = bα ? bα : i1;
    IR_t * cα = NULL, * cβ = NULL;
    IR_t * i2 = lower_program(icn_bounded(cx), e->c[2], sec, ω_in, &cα, &cβ);
    if (!i2) return NULL;
    sec->β->γ = cα ? cα : i2;
    set_succ_fail(sec, γ_in, ω_in);
    return ret(sec, α_out, β_out, sec->α, ω_in);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * icn_idx(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (e->n < 2 || !e->c[0] || !e->c[1]) return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    IR_t * call = nalloc(cx, IR_CALL);
    if (!call) return NULL;
    IR_LIT(call).sval = "[]";
    IR_LIT(call).ival = 2;
    IR_LIT(call).dval = 2.0;
    IR_graph_t ** blks = (IR_graph_t **) calloc(2, sizeof(IR_graph_t *));
    if (!blks) return NULL;
    lcx_t mv = icn_bounded(cx);
    blks[0] = lower_value_subgraph(mv, e->c[0]);
    blks[1] = lower_value_subgraph(mv, e->c[1]);
    if (!blks[0] || !blks[1]) { free(blks); return NULL; }
    IR_EXEC(call).counter = (int64_t)(intptr_t) blks;
    set_succ_fail(call, γ_in, ω_in);
    return ret(call, α_out, β_out, call, ω_in);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * icn_makelist(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    int n = e->n;
    IR_t * ml = nalloc(cx, IR_CALL);
    if (!ml) return NULL;
    IR_LIT(ml).sval = GC_strdup("MAKELIST");
    IR_LIT(ml).ival = (int64_t) n;
    IR_LIT(ml).dval = 2.0;
    IR_graph_t ** blks = (IR_graph_t **) calloc((size_t)(n > 0 ? n : 1), sizeof(IR_graph_t *));
    if (!blks) return NULL;
    lcx_t mv = icn_bounded(cx);
    for (int i = 0; i < n; i++) {
        blks[i] = lower_value_subgraph(mv, e->c[i] ? e->c[i] : e->c[0]);
        if (!blks[i]) { free(blks); return NULL; }
    }
    IR_EXEC(ml).counter = (int64_t)(intptr_t) blks;
    set_succ_fail(ml, γ_in, ω_in);
    return ret(ml, α_out, β_out, ml, ω_in);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * icn_cset_binop(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (e->n < 2 || !e->c[0] || !e->c[1]) return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    const char * fn = (e->t == TT_CSET_UNION) ? "++" : (e->t == TT_CSET_DIFF) ? "--" : "**";
    IR_t * call = nalloc(cx, IR_CALL);
    if (!call) return NULL;
    IR_LIT(call).sval = fn;
    IR_LIT(call).ival = 2;
    IR_LIT(call).dval = 2.0;
    IR_graph_t ** blks = (IR_graph_t **) calloc(2, sizeof(IR_graph_t *));
    if (!blks) return NULL;
    lcx_t mv = icn_bounded(cx);
    blks[0] = lower_value_subgraph(mv, e->c[0]);
    blks[1] = lower_value_subgraph(mv, e->c[1]);
    if (!blks[0] || !blks[1]) { free(blks); return NULL; }
    IR_EXEC(call).counter = (int64_t)(intptr_t) blks;
    set_succ_fail(call, γ_in, ω_in);
    return ret(call, α_out, β_out, call, ω_in);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_t * lower_icn(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!e) return NULL;
    if (e->t == TT_AUGOP && (AugOp_e) e->v.ival == AUGOP_SCAN && e->n >= 2 && e->c[0] && e->c[0]->t == TT_VAR && e->c[1]) {
        tree_t * sc = ast_node_new(TT_SCAN);  ast_push(sc, (tree_t *) e->c[0]); ast_push(sc, (tree_t *) e->c[1]);
        tree_t * as = ast_node_new(TT_ASSIGN); ast_push(as, (tree_t *) e->c[0]); ast_push(as, sc);
        e = as;
    }
    if (e->t == TT_AUGOP && e->n >= 2 && e->c[0] && e->c[0]->t == TT_VAR && e->c[1] && icn_augop_binop_tt((AugOp_e) e->v.ival) != (tree_e)0) {
        tree_t * bo = ast_node_new(icn_augop_binop_tt((AugOp_e) e->v.ival));
        ast_push(bo, (tree_t *) e->c[0]); ast_push(bo, (tree_t *) e->c[1]);
        tree_t * as = ast_node_new(TT_ASSIGN); ast_push(as, (tree_t *) e->c[0]); ast_push(as, bo);
        e = as;
    }
    switch (e->t) {
    case TT_ILIT: { IR_t * n = nalloc(cx, IR_LIT_I); if (n) IR_LIT(n).ival = e->v.ival; return emit_leaf(cx, n, γ_in, ω_in, α_out, β_out); }
    case TT_FLIT: { IR_t * n = nalloc(cx, IR_LIT_F); if (n) IR_LIT(n).dval = e->v.dval; return emit_leaf(cx, n, γ_in, ω_in, α_out, β_out); }
    case TT_QLIT: case TT_CSET: { IR_t * n = nalloc(cx, IR_LIT_S); if (n) IR_LIT(n).sval = e->v.sval ? e->v.sval : ""; return emit_leaf(cx, n, γ_in, ω_in, α_out, β_out); }
    case TT_NUL: case TT_NULL: { IR_t * n = nalloc(cx, IR_LIT_NUL); return emit_leaf(cx, n, γ_in, ω_in, α_out, β_out); }
    case TT_VAR: case TT_NAME: { IR_t * n = nalloc(cx, IR_VAR); if (n) { IR_LIT(n).sval = e->v.sval; if (icn_is_global(IR_LIT(n).sval)) IR_EXEC(n).state = 1; } return emit_leaf(cx, n, γ_in, ω_in, α_out, β_out); }
    case TT_KEYWORD: { IR_t * n = nalloc(cx, IR_KEYWORD); if (n) IR_LIT(n).sval = e->v.sval; return emit_leaf(cx, n, γ_in, ω_in, α_out, β_out); }
    case TT_LOOP_BREAK:
        return icn_loop_break(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_LOOP_NEXT:
        return icn_loop_next(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_ASSIGN:
        return icn_assign(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_SCAN:
        return icn_scan(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_RETURN: case TT_NRETURN:
        return icn_return(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_SUSPEND:
        return icn_suspend(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_INITIAL:
        return icn_initial(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_LIMIT:
        return icn_limit(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_CASE:
        return icn_case(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_REVASSIGN: case TT_REVSWAP: case TT_SWAP:
        return icn_swap(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_FIELD:
        return icn_field_get(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_SECTION: case TT_SECTION_PLUS: case TT_SECTION_MINUS:
        return icn_section(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_IDX:
        return icn_idx(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_MAKELIST: case TT_VLIST:
        return icn_makelist(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_CSET_UNION: case TT_CSET_DIFF: case TT_CSET_INTER:
        return icn_cset_binop(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_MATCH_UNARY:
        if (e->n >= 1 && e->c[0]) {
            tree_t * mfn = ast_node_new(TT_VAR); mfn->v.sval = "match";
            tree_t * mc  = ast_node_new(TT_FNC); ast_push(mc, mfn); ast_push(mc, (tree_t *) e->c[0]);
            tree_t * tfn = ast_node_new(TT_VAR); tfn->v.sval = "tab";
            tree_t * tc  = ast_node_new(TT_FNC); ast_push(tc, tfn); ast_push(tc, mc);
            return icn_det_call(cx, tc, γ_in, ω_in, α_out, β_out);
        }
        return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_FNC: case TT_PROC_FAIL: case TT_AUGOP:
        if (e->n >= 1 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval) {
            const char * fn = e->c[0]->v.sval;
            if (e->n == 2 && (!strcmp(fn, "write") || !strcmp(fn, "writes")))
                return wire_det_builtin1(cx, e->c[1], fn, γ_in, ω_in, α_out, β_out);
            return icn_det_call(cx, e, γ_in, ω_in, α_out, β_out);
        }
        return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    case TT_LOCAL: case TT_GLOBAL: case TT_STATIC_DECL: {
        IR_t * nop = nalloc(cx, IR_SUCCEED);
        if (!nop) return NULL;
        set_succ_fail(nop, γ_in, ω_in);
        return ret(nop, α_out, β_out, nop, ω_in);
    }
    default:
        return lower_value_shared(cx, e, γ_in, ω_in, α_out, β_out);
    }
}
