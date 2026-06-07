/*====================================================================================================================*/
/*====================================================================================================================*/
#include "lower_internal.h"
#include "IR_interp_state.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gc/gc.h>
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_t * v_raku_for(lcx_t cx, const tree_t * range_t, const char * var, const tree_t * body_t, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!range_t || !var) return NULL;
    IR_t * bind = nalloc(cx, IR_ASSIGN);
    if (!bind) return NULL;
    bind->sval = var;
    IR_t * gα = NULL, * gβ = NULL;
    IR_t * gen = lower(cx, range_t, bind  , γ_in  , &gα, &gβ);
    if (!gen) return NULL;
    IR_t * bα = NULL, * bβ = NULL;
    IR_t * body = body_t ? lower(bounded(cx), body_t, gβ  , gβ  , &bα, &bβ) : NULL;
    if (body_t && !body) return NULL;
    set_succ_fail(bind, body_t ? bα : gβ  , ω_in  );
    return ret(gen, α_out, β_out, gα  , ω_in  );
}
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_t * v_raku_gather(lcx_t cx, const tree_t * body_t, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!body_t) return NULL;
    int n = 0;
    if (body_t->t == TT_SEQ_EXPR) {
        for (int i = 0; i < body_t->n; i++) if (body_t->c[i] && body_t->c[i]->t == TT_SUSPEND) n++;
    } else if (body_t->t == TT_SUSPEND) {
        n = 1;
    }
    IR_t * g = nalloc(cx, IR_GATHER);
    if (!g) return NULL;
    g->ival = n;
    if (n > 0) {
        IR_graph_t ** subs = (IR_graph_t **) calloc((size_t) n, sizeof(IR_graph_t *));
        if (!subs) return NULL;
        int k = 0;
        if (body_t->t == TT_SEQ_EXPR) {
            for (int i = 0; i < body_t->n; i++) {
                const tree_t * s = body_t->c[i];
                if (!s || s->t != TT_SUSPEND) continue;
                const tree_t * payload = (s->n >= 1) ? s->c[0] : NULL;
                if (!payload) { free(subs); return NULL; }
                subs[k] = lower_value_subgraph(cx, payload);
                if (!subs[k]) { free(subs); return NULL; }
                k++;
            }
        } else {
            const tree_t * payload = (body_t->n >= 1) ? body_t->c[0] : NULL;
            if (!payload) { free(subs); return NULL; }
            subs[0] = lower_value_subgraph(cx, payload);
            if (!subs[0]) { free(subs); return NULL; }
        }
        g->counter = (int64_t)(intptr_t) subs;
    }
    set_succ_fail(g, γ_in, ω_in);
    return ret(g, α_out, β_out, g  , g  );
}
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_t * v_raku_map_grep(lcx_t cx, int is_grep, const tree_t * closure_t, const tree_t * src_t, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!closure_t || !src_t) return NULL;
    IR_t * mg = nalloc(cx, is_grep ? IR_GREP : IR_MAP);
    if (!mg) return NULL;
    IR_graph_t * src_sg = lower_value_subgraph(cx, src_t);
    if (!src_sg) return NULL;
    IR_graph_t * body_sg = lower_value_subgraph(cx, closure_t);
    if (!body_sg) { IR_free(src_sg); return NULL; }
    mg->counter = (int64_t)(intptr_t) src_sg;
    mg->ival    = (int64_t)(intptr_t) body_sg;
    mg->state   = 0;
    set_succ_fail(mg, γ_in, ω_in);
    return ret(mg, α_out, β_out, mg  , mg  );
}
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_t * v_raku_pop(lcx_t cx, const char * dst, const tree_t * arr, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    if (!dst || !arr || arr->t != TT_VAR || !arr->v.sval) return NULL;
    const tree_t * k[1] = { arr };
    IR_t * o2α = NULL, * o2β = NULL;
    IR_t * op2 = v_raku_mutate_writeback(cx, arr->v.sval, "arr_init", k, 1, γ_in  , ω_in, &o2α, &o2β);
    if (!op2) return NULL;
    (void) o2β;
    IR_t * as = nalloc(cx, IR_ASSIGN);
    if (!as) return NULL;
    as->sval = GC_strdup(dst);
    IR_t * cα = NULL, * cβ = NULL;
    IR_t * call = v_raku_det_call(cx, "arr_last", k, 1, as  , ω_in, &cα, &cβ);
    if (!call) return NULL;
    (void) cβ;
    set_succ_fail(as, o2α  , ω_in);
    return ret(as, α_out, β_out, cα  , ω_in);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_t * rku_fnc_junction(lcx_t cx, const char * flav, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    char jfn[32];
    snprintf(jfn, sizeof jfn, "__rk_jct_%s", flav);
    int nmembers = e->n - 1;
    IR_t * call = nalloc(cx, IR_CALL);
    if (!call) return NULL;
    call->sval = GC_strdup(jfn);
    call->ival = nmembers;
    call->dval = 2.0;
    IR_graph_t ** blks = (IR_graph_t **) calloc((size_t) nmembers, sizeof(IR_graph_t *));
    if (!blks) return NULL;
    lcx_t mv = cx; mv.role = ROLE_VALUE;
    for (int i = 1; i <= nmembers; i++) {
        blks[i - 1] = lower_value_subgraph(mv, e->c[i]);
        if (!blks[i - 1]) { free(blks); return NULL; }
    }
    call->counter = (int64_t)(intptr_t) blks;
    set_succ_fail(call, γ_in, ω_in);
    return ret(call, α_out, β_out, call  , ω_in  );
}
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_t * rku_fnc_calls(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out, int * handled) {
    const char * fn = e->c[0]->v.sval;
    {
        static const struct { const char * name; const char * pure; int minargs; } RK_MUT[] = {
            { "push", "push_pure", 3 }, { "hash_set", "hash_set_pure", 4 },
            { "hash_delete", "hash_delete_pure", 3 }, { "arr_set", "arr_set_pure", 4 }, { NULL, NULL, 0 } };
        for (int i = 0; RK_MUT[i].name; i++) {
            if (strcmp(fn, RK_MUT[i].name)) continue;
            if (e->n < RK_MUT[i].minargs || !e->c[1] || e->c[1]->t != TT_VAR || !e->c[1]->v.sval) break;
            int nk = e->n - 1;
            const tree_t * kids[16];
            if (nk > 16) nk = 16;
            for (int j = 0; j < nk; j++) kids[j] = e->c[j + 1];
            *handled = 1;
            return v_raku_mutate_writeback(cx, e->c[1]->v.sval, RK_MUT[i].pure, kids, nk, γ_in, ω_in, α_out, β_out);
        }
    }
    static const char * const RK_PURE[] = {
        "__rk_arr", "elems", "reverse", "sort", "array_sort", "arr_get",
        "hash_get", "hash_exists", "hash_keys", "hash_values", "hash_pairs",
        "join", "sum", "unique", "head", "tail", "chars", "length",
        "lc", "uc", "trim", "substr", "index", "rindex", NULL };
    int is_pure = 0;
    for (int i = 0; RK_PURE[i]; i++) if (!strcmp(fn, RK_PURE[i])) { is_pure = 1; break; }
    if (is_pure) {
        int nk = e->n - 1;
        const tree_t * kids[16];
        if (nk > 16) nk = 16;
        for (int i = 0; i < nk; i++) kids[i] = e->c[i + 1];
        *handled = 1;
        return v_raku_det_call(cx, fn, kids, nk, γ_in, ω_in, α_out, β_out);
    }
    *handled = 0;
    return NULL;
}
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_t * rku_say_print(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    const char * fn = (e->t == TT_SAY) ? "write" : "writes";
    return wire_det_builtin1(cx, e->c[0], fn, γ_in, ω_in, α_out, β_out);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_t * rku_for_range(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    const tree_t * lo = e->c[1]; const tree_t * hi = e->c[2]; const tree_t * body = e->c[3];
    int ex = (e->n >= 5 && e->c[4] && e->c[4]->t == TT_ILIT) ? (int) e->c[4]->v.ival : 0;
    tree_t * hi_eff = (tree_t *) hi;
    if (ex) {
        tree_t * one = ast_node_new(TT_ILIT); one->v.ival = 1;
        hi_eff = ast_node_new(TT_SUB); ast_push(hi_eff, (tree_t *) hi); ast_push(hi_eff, one);
    }
    tree_t * rng = ast_node_new(TT_TO); ast_push(rng, (tree_t *) lo); ast_push(rng, hi_eff);
    return v_raku_for(cx, rng, e->c[0]->v.sval, body, γ_in, ω_in, α_out, β_out);
}
