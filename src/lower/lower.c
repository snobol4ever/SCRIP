/*====================================================================================================================*/
/*====================================================================================================================*/
#define BB_DEFINE_NAMES
#include "lower.h"
#include "IR_interp_state.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <gc/gc.h>
/*====================================================================================================================*/
/*====================================================================================================================*/
#include "lower_internal.h"
/*--------------------------------------------------------------------------------------------------------------------*/
/* wire_det_builtin1 forward decl removed — now non-static, declared in lower_internal.h */
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_t * nalloc(lcx_t cx, IR_e kind) { return IR_node_alloc(cx.bbg, kind); }
/*--------------------------------------------------------------------------------------------------------------------*/
void set_succ_fail(IR_t * n, IR_t * γ_in, IR_t * ω_in) {
    if (!n) return;
    if (!n->γ && γ_in) n->γ = γ_in;
    if (!n->ω && ω_in) n->ω = ω_in;
}
/*--------------------------------------------------------------------------------------------------------------------*/
IR_t * ret(IR_t * n, IR_t ** α_out, IR_t ** β_out, IR_t * α, IR_t * β) {
    if (α_out) *α_out = α;
    if (β_out) *β_out = β;
    return n;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
typedef struct { const char * name; IR_t * landing; } bb_label_entry_t;
static bb_label_entry_t g_bb_labels[1024];
static int              g_bb_label_n = 0;
/*--------------------------------------------------------------------------------------------------------------------*/
void bb_label_registry_reset(void) { g_bb_label_n = 0; }
/*--------------------------------------------------------------------------------------------------------------------*/
void bb_label_registry_add(const char * name, IR_t * landing) {
    if (!name || !landing || g_bb_label_n >= 1024) return;
    g_bb_labels[g_bb_label_n].name = name; g_bb_labels[g_bb_label_n].landing = landing; g_bb_label_n++;
}
/*--------------------------------------------------------------------------------------------------------------------*/
IR_t * bb_label_landing(const char * name) {
    if (!name) return NULL;
    for (int i = 0; i < g_bb_label_n; i++)
        if (g_bb_labels[i].name && !strcmp(g_bb_labels[i].name, name)) return g_bb_labels[i].landing;
    if (!strcmp(name, "END")) return NULL;
    return NULL;
}
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_t * lower(lcx_t cx, const tree_t * e, IR_t * γ, IR_t * ω, IR_ref_t * α, IR_ref_t * β) {
    if (!e) return iref(NULL, α, β, γ, ω);
    if (cx.lang == IR_LANG_ICN) { IR_t * aα = NULL, * aβ = NULL; IR_t * r = lower_icn(cx, e, γ, ω, &aα, &aβ); return iref(r, α, β, aα, aβ); }
    if (cx.lang == IR_LANG_SNO || cx.lang == IR_LANG_SCO || cx.lang == IR_LANG_REB) return lower_sno(cx, e, γ, ω, α, β);
    if (cx.lang == IR_LANG_RKU) return lower_rku(cx, e, γ, ω, α, β);
    if (cx.lang == IR_LANG_PAS) return lower_pas(cx, e, γ, ω, α, β);
    IR_t * aα = NULL, * aβ = NULL;
    IR_t * r;
    switch (cx.role) {
    case ROLE_PATTERN: r = lower_pattern(cx, e, γ, ω, &aα, &aβ); break;
    case ROLE_VALUE:
    default:           r = lower_value_shared(cx, e, γ, ω, &aα, &aβ); break;
    }
    return iref(r, α, β, aα, aβ);
}
/*--------------------------------------------------------------------------------------------------------------------*/
IR_t * lower_program(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    IR_ref_t rα = {0}, rβ = {0};
    IR_t * r = lower(cx, e, γ_in, ω_in, &rα, &rβ);
    if (α_out) *α_out = rα.node;
    if (β_out) *β_out = rβ.node;
    return r;
}
/*====================================================================================================================*/
/*====================================================================================================================*/
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_t * lower_unhandled(lcx_t cx, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    (void) γ_in; (void) ω_in;
    fprintf(stderr, "[lower] UNHANDLED role=%d kind=%d\n", (int)cx.role, e ? (int)e->t : -1);
    return ret(NULL, α_out, β_out, NULL, NULL);
}
/*====================================================================================================================*/
/*====================================================================================================================*/
IR_t * lower_value_entry(IR_graph_t * bbg, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    lcx_t cx = { bbg, ROLE_VALUE, 0, bbg ? bbg->lang : 0, NULL, NULL };
    return lower_program(cx, e, γ_in, ω_in, α_out, β_out);
}
/*--------------------------------------------------------------------------------------------------------------------*/
IR_t * lower_subject_entry(IR_graph_t * bbg, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    lcx_t cx = { bbg, ROLE_VALUE, 0, bbg ? bbg->lang : 0, NULL, NULL };
    IR_t * subj = nalloc(cx, IR_SUBJECT);
    if (!subj) return NULL;
    IR_t * oα = NULL, * oβ = NULL;
    IR_t * op = lower_program(cx, e, subj, ω_in, &oα, &oβ);
    if (!op) return NULL;
    (void) oβ;
    set_succ_fail(subj, γ_in, ω_in);
    return ret(subj, α_out, β_out, oα ? oα : subj, ω_in);
}
/*--------------------------------------------------------------------------------------------------------------------*/
IR_t * lower_pat_build_entry(IR_graph_t * bbg, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    lcx_t cx = { bbg, ROLE_PATTERN, 0, bbg ? bbg->lang : 0, NULL, NULL };
    if (!e || e->t != TT_QLIT) return lower_unhandled(cx, e, γ_in, ω_in, α_out, β_out);
    const char * lit = e->v.sval ? e->v.sval : "";
    IR_t * sealed = nalloc(cx, IR_PAT_LIT);
    if (!sealed) return NULL;
    IR_LIT(sealed).sval = lit;
    IR_t * ref = nalloc(cx, IR_REF_INVARIANT);
    if (!ref) return NULL;
    IR_LIT(ref).sval = lit;
    if (!ir_operand_push(ref, sealed)) return NULL;
    lcx_t bx = cx; bx.bounded = 1;
    return emit_leaf(bx, ref, γ_in, ω_in, α_out, β_out);
}
/*--------------------------------------------------------------------------------------------------------------------*/
IR_t * lower_match_entry(IR_graph_t * bbg, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    lcx_t cx = { bbg, ROLE_PATTERN, 0, bbg ? bbg->lang : 0, NULL, NULL };
    IR_t * m = nalloc(cx, IR_PAT_MATCH);
    if (!m) return NULL;
    IR_t * eα = NULL, * eβ = NULL;
    IR_t * el = lower_program(cx, e, m, m, &eα, &eβ);
    if (!el) return NULL;
    (void) eβ;
    IR_t * entry = eα ? eα : el;
    if (!ir_operand_push(m, entry)) return NULL;
    set_succ_fail(m, γ_in, ω_in);
    return ret(m, α_out, β_out, m, ω_in);
}
/*--------------------------------------------------------------------------------------------------------------------*/
IR_t * lower_pattern_entry(IR_graph_t * bbg, const tree_t * e, IR_t * γ_in, IR_t * ω_in, IR_t ** α_out, IR_t ** β_out) {
    lcx_t cx = { bbg, ROLE_PATTERN, 0, bbg ? bbg->lang : 0, NULL, NULL };
    return lower_program(cx, e, γ_in, ω_in, α_out, β_out);
}