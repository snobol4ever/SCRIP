#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "lower.h"
#include "box_state.h"
#define SNO_MAXSTMTS 2048
typedef struct {
    IR_graph_t * g;
    IR_t * PSUCC;
    IR_t * PFAIL;
    IR_t * PRET;
    IR_t * PFRET;
    IR_t ** labels;
    int     nlabels;
    const char ** lname;
    int           lstmt[SNO_MAXSTMTS];
    int           nlmap;
} snx_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void γ_to(IR_t * nd, IR_t * t) { lc_γ_to(nd, t); }
static void ω_to(IR_t * nd, IR_t * t) { lc_ω_to(nd, t); }
static IR_t * build(snx_t * cx, IR_e op, IR_t * γ, IR_t * ω) { return lc_build(cx->g, op, γ, ω); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_is_end(const tree_t * s) {
    if (!s) return 0;
    if (s->t == TT_END) return 1;
    if (s->t != TT_STMT) return 0;
    for (int i = 0; i < s->n; i++) {
        const tree_t * a = s->c[i];
        if (a && a->t == TT_ATTR && a->v.sval && !strcmp(a->v.sval, ":end")) return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * sno_attr(const tree_t * s, const char * tag) {
    for (int i = 0; i < s->n; i++) {
        const tree_t * a = s->c[i];
        if (a && a->t == TT_ATTR && a->v.sval && !strcmp(a->v.sval, tag))
            return (a->n > 0) ? a->c[0] : NULL;
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_has_attr(const tree_t * s, const char * tag) {
    for (int i = 0; i < s->n; i++) {
        const tree_t * a = s->c[i];
        if (a && a->t == TT_ATTR && a->v.sval && !strcmp(a->v.sval, tag)) return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * sno_goto_node(const tree_t * s, tree_e kind, const char * tag) {
    for (int i = 0; i < s->n; i++) {
        const tree_t * a = s->c[i]; if (!a) continue;
        if (a->t == kind) return a;
        if (a->t == TT_ATTR && a->v.sval && !strcmp(a->v.sval, tag)) return a;
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * sno_goto_label(const tree_t * ch) {
    if (!ch || !ch->n || !ch->c[0]) return NULL; return ch->c[0]->v.sval; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * sno_stmt_label(const tree_t * s) {
    const tree_t * lbl = sno_attr(s, ":lbl"); return lbl ? lbl->v.sval : NULL; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int32_t sno_stmt_stno(const tree_t * s) {
    const tree_t * a = sno_attr(s, ":stno"); return (a && a->v.sval) ? (int32_t)atoi(a->v.sval) : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_pat_builtin(const char * nm) {
    if (!nm) return 0;
    return !strcmp(nm,"REM") || !strcmp(nm,"rem") || !strcmp(nm,"ARB") || !strcmp(nm,"arb") || !strcmp(nm,"FENCE") || !strcmp(nm,"fence")
        || !strcmp(nm,"ABORT") || !strcmp(nm,"abort") || !strcmp(nm,"BAL") || !strcmp(nm,"bal") || !strcmp(nm,"FAIL") || !strcmp(nm,"fail"); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * resolve(snx_t * cx, const char * name) {
    if (!name || !name[0]) return NULL;
    if (!strcmp(name, "END"))     return cx->PSUCC;
    if (!strcmp(name, "RETURN"))  return cx->PRET;
    if (!strcmp(name, "NRETURN")) return cx->PRET;
    if (!strcmp(name, "FRETURN")) return cx->PFRET;
    for (int i = 0; i < cx->nlmap; i++)
        if (cx->lname[i] && !strcmp(cx->lname[i], name)) return cx->labels[cx->lstmt[i]];
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * next_label(snx_t * cx, int i) {
    return (i + 1 < cx->nlabels) ? cx->labels[i + 1] : cx->PSUCC; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_sno_unop(tree_e tt) {
    switch (tt) {
    case TT_MNS: case TT_PLS: case TT_NONNULL: case TT_NOT: case TT_INTERROGATE:
    case TT_NAME: case TT_INDIRECT: return 1;
    default: return 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_indirect_resolvable(const tree_t * a) {
    if (!a || a->t != TT_INDIRECT || a->n != 1 || !a->c[0]) return 0;
    const tree_t * in = a->c[0];
    if (in->t == TT_QLIT && in->v.sval) return 1;
    if (in->t == TT_NAME && in->n > 0 && in->c[0]) {
        const tree_t * g = in->c[0];
        if (g->t == TT_VAR && g->v.sval) return 1;
        if (g->t == TT_IDX && g->n >= 2 && g->c[0] && g->c[0]->t == TT_VAR && g->c[1] && (g->c[1]->t == TT_ILIT || g->c[1]->t == TT_VAR || g->c[1]->t == TT_QLIT)) return 1;
    }
    return 0;
}
static IR_t * lower_expr(snx_t * cx, const tree_t * t, IR_t * cont, IR_t * nxt, IR_t ** res);
static IR_graph_t * sno_arg_block(void * vcx, const tree_t * a);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sno_seq_node(snx_t * cx, const tree_t * t, IR_t * cont, IR_t * nxt) {
    IR_t * seq = build(cx, IR_SEQ, cont, nxt); IR_LIT(seq).dval = 1.0;
    (void)(sno_arg_block(cx, (t->n > 0) ? t->c[0] : NULL));
    IR_LIT(seq).ival     = (int64_t)(intptr_t) sno_arg_block(cx, (t->n > 1) ? t->c[1] : NULL);
    return seq;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sno_arg_lower(void * vcx, const tree_t * a, IR_t * F) {
    snx_t * cx = (snx_t *) vcx; IR_t * r = NULL;
    return (a && a->t == TT_SEQ) ? sno_seq_node(cx, a, NULL, F) : lower_expr(cx, a, NULL, F, &r);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_graph_t * sno_arg_block(void * vcx, const tree_t * a) { return lc_arg_block(&((snx_t *) vcx)->g, IR_LANG_SNO, sno_arg_lower, vcx, a); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_call_channels(snx_t * cx, IR_t * call, const tree_t * t) {
    int is_def = (t->v.sval && !strcmp(t->v.sval, "DEFINE"));
    lc_call_argblks(call, is_def ? 5.0 : 2.0, t->n, sno_arg_block, cx, (const tree_t * const *) t->c);
    if (is_def) call->op = IR_CALL_DEFINE;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_expr(snx_t * cx, const tree_t * t, IR_t * cont, IR_t * nxt, IR_t ** res) {
    IR_t * dummy = NULL; if (!res) res = &dummy;
    if (!t) { IR_t * s = build(cx, IR_SUCCEED, cont, nxt); *res = s; return s; }
    if (lc_is_binop(t->t) && t->n >= 2) {
        IR_t * op = build(cx, IR_BINOP, cont, nxt); IR_LIT(op).ival = lc_binop_code(t->t);
        IR_t * lr = NULL, * rr = NULL;
        IR_t * ea = lower_expr(cx, t->c[0], NULL, nxt, &lr);
        IR_t * eb = lower_expr(cx, t->c[1], op, nxt, &rr);
        γ_to(lr, eb); ir_operand_push(op, lr); ir_operand_push(op, rr); *res = op; return ea;
    }
    if (t->t == TT_NAME && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR && t->c[0]->v.sval) {
        IR_t * nd = build(cx, IR_LIT_S, cont, nxt); IR_LIT(nd).sval = t->c[0]->v.sval; *res = nd; return nd; }
    if (sno_indirect_resolvable(t)) {
        const tree_t * in = t->c[0];
        if (in->t == TT_QLIT) { IR_t * nd = build(cx, IR_VAR, cont, nxt); IR_LIT(nd).sval = in->v.sval; *res = nd; return nd; }
        const tree_t * g = in->c[0];
        if (g->t == TT_VAR) { IR_t * nd = build(cx, IR_VAR, cont, nxt); IR_LIT(nd).sval = g->v.sval; *res = nd; return nd; }
        return lower_expr(cx, g, cont, nxt, res);
    }
    if (is_sno_unop(t->t)) {
        IR_t * op = build(cx, IR_UNOP, cont, nxt); IR_LIT(op).ival = (long long) t->t;
        IR_t * orr = NULL; IR_t * ea = lower_expr(cx, t->c[0], op, nxt, &orr);
        ir_operand_push(op, orr); *res = op; return ea;
    }
    switch (t->t) {
    case TT_ILIT:    { IR_t * nd = build(cx, IR_LIT_I,   cont, nxt); IR_LIT(nd).ival  = t->v.ival; *res = nd; return nd; }
    case TT_FLIT:    { IR_t * nd = build(cx, IR_LIT_F,   cont, nxt); IR_LIT(nd).dval  = t->v.dval; *res = nd; return nd; }
    case TT_QLIT:    { IR_t * nd = build(cx, IR_LIT_S,   cont, nxt); IR_LIT(nd).sval  = t->v.sval; *res = nd; return nd; }
    case TT_NUL:     { IR_t * nd = build(cx, IR_LIT_NUL, cont, nxt);                                *res = nd; return nd; }
    case TT_VAR:     { IR_t * nd = build(cx, IR_VAR,     cont, nxt); IR_LIT(nd).sval  = t->v.sval; *res = nd; return nd; }
    case TT_KEYWORD: { IR_t * nd = build(cx, IR_KEYWORD, cont, nxt); IR_LIT(nd).sval  = t->v.sval; *res = nd; return nd; }
    case TT_FNC:     {
        const char * nm = t->v.sval ? t->v.sval : "?";
        IR_t * nd = build(cx, IR_CALL, cont, nxt); IR_LIT(nd).sval = nm; IR_LIT(nd).ival = (long long) t->n;
        sno_call_channels(cx, nd, t);
        *res = nd; return nd;
    }
    case TT_IDX:     {
        if (t->n >= 2 && t->c[0] && t->c[0]->t == TT_VAR && t->c[1] && (t->c[1]->t == TT_ILIT || t->c[1]->t == TT_VAR || t->c[1]->t == TT_QLIT)) {
            IR_t * idx = build(cx, IR_IDX, cont, nxt);
            IR_t * base_box = build(cx, IR_VAR, NULL, NULL); IR_LIT(base_box).sval = t->c[0]->v.sval;
            IR_t * key_box;
            if (t->c[1]->t == TT_ILIT) { key_box = build(cx, IR_LIT_I, NULL, NULL); IR_LIT(key_box).ival = t->c[1]->v.ival; }
            else if (t->c[1]->t == TT_QLIT) { key_box = build(cx, IR_LIT_S, NULL, NULL); IR_LIT(key_box).sval = t->c[1]->v.sval ? t->c[1]->v.sval : ""; }
            else { key_box = build(cx, IR_VAR, NULL, NULL); IR_LIT(key_box).sval = t->c[1]->v.sval; }
            ir_operand_push(idx, base_box); ir_operand_push(idx, key_box);
            *res = idx; return idx;
        }
        IR_t * s = build(cx, IR_SUCCEED, cont, nxt); *res = s; return s;
    }
    default: { IR_t * s = build(cx, IR_SUCCEED, cont, nxt); *res = s; return s; }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_pat_consumer(IR_e op) {
    switch (op) {
    case IR_MATCH_LEN: case IR_MATCH_TAB: case IR_MATCH_RTAB: case IR_MATCH_REM: case IR_MATCH_BREAK:
    case IR_MATCH_BREAKX: case IR_MATCH_SPAN: case IR_MATCH_ANY: case IR_MATCH_NOTANY: case IR_MATCH_LIT: case IR_MATCH_ARB: case IR_MATCH_ARBNO: return 1;
    default: return 0; }
}
static IR_t * lower_pat_node(IR_graph_t * pg, const tree_t * t, IR_t * succ, IR_t * fail);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_pat_node_tail(IR_graph_t * pg, const tree_t * t, IR_t * succ, IR_t * fail, IR_t ** tail_out) {
    int before = pg->n;
    IR_t * entry = lower_pat_node(pg, t, succ, fail);
    if (tail_out) *tail_out = (before < pg->n) ? pg->all[before] : entry;
    return entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_pat_node(IR_graph_t * pg, const tree_t * t, IR_t * succ, IR_t * fail) {
    if (!t) return succ;
    switch (t->t) {
    case TT_QLIT: {
        IR_t * nd = IR_node_alloc(pg, IR_MATCH_LIT); γ_to(nd, succ); ω_to(nd, fail);
        IR_LIT(nd).sval = t->v.sval; return nd; }
    case TT_VAR: {
        const char * nm = t->v.sval;
        if (nm) {
            if (!strcmp(nm,"REM")  || !strcmp(nm,"rem"))   { IR_t * nd = IR_node_alloc(pg, IR_MATCH_REM);   γ_to(nd, succ); ω_to(nd, fail); return nd; }
            if (!strcmp(nm,"ARB")  || !strcmp(nm,"arb"))   { IR_t * nd = IR_node_alloc(pg, IR_MATCH_ARB);   γ_to(nd, succ); ω_to(nd, fail); return nd; }
            if (!strcmp(nm,"FENCE")|| !strcmp(nm,"fence"))  { IR_t * nd = IR_node_alloc(pg, IR_MATCH_FENCE); γ_to(nd, succ); ω_to(nd, fail); IR_LIT(nd).ival = 1; return nd; }
            if (!strcmp(nm,"ABORT")|| !strcmp(nm,"abort"))  { IR_t * nd = IR_node_alloc(pg, IR_MATCH_ABORT); γ_to(nd, succ); ω_to(nd, fail); return nd; }
            if (!strcmp(nm,"BAL")  || !strcmp(nm,"bal"))    { IR_t * nd = IR_node_alloc(pg, IR_MATCH_BAL);   γ_to(nd, succ); ω_to(nd, fail); return nd; }
            if (!strcmp(nm,"FAIL") || !strcmp(nm,"fail"))   { IR_t * nd = IR_node_alloc(pg, IR_FAIL);      γ_to(nd, succ); ω_to(nd, fail); return nd; }
        }
        IR_t * nd = IR_node_alloc(pg, IR_MATCH_DEFER); γ_to(nd, succ); ω_to(nd, fail);
        IR_LIT(nd).sval = t->v.sval; return nd; }
    case TT_ARB: {
        IR_t * nd = IR_node_alloc(pg, IR_MATCH_ARB); γ_to(nd, succ); ω_to(nd, fail); return nd; }
    case TT_REM: {
        IR_t * nd = IR_node_alloc(pg, IR_MATCH_REM); γ_to(nd, succ); ω_to(nd, fail); return nd; }
    case TT_BREAK: case TT_BREAKX: {
        IR_e op = (t->t == TT_BREAK) ? IR_MATCH_BREAK : IR_MATCH_BREAKX;
        IR_t * nd = IR_node_alloc(pg, op); γ_to(nd, succ); ω_to(nd, fail);
        if (t->n > 0 && t->c[0]) {
            const tree_t * arg = t->c[0];
            if (arg->t == TT_VAR || arg->t == TT_KEYWORD) { IR_LIT(nd).sval = arg->v.sval; IR_LIT(nd).dval = 1.0; }
            else IR_LIT(nd).sval = arg->v.sval; }
        return nd; }
    case TT_SPAN: {
        IR_t * nd = IR_node_alloc(pg, IR_MATCH_SPAN); γ_to(nd, succ); ω_to(nd, fail);
        if (t->n > 0 && t->c[0]) {
            const tree_t * arg = t->c[0];
            if (arg->t == TT_VAR || arg->t == TT_KEYWORD) { IR_LIT(nd).sval = arg->v.sval; IR_LIT(nd).ival = 1; }
            else IR_LIT(nd).sval = arg->v.sval; }
        return nd; }
    case TT_ANY: {
        IR_t * nd = IR_node_alloc(pg, IR_MATCH_ANY); γ_to(nd, succ); ω_to(nd, fail);
        if (t->n > 0 && t->c[0]) {
            const tree_t * arg = t->c[0];
            if (arg->t == TT_VAR || arg->t == TT_KEYWORD) { IR_LIT(nd).sval = arg->v.sval; IR_LIT(nd).dval = 1.0; }
            else IR_LIT(nd).sval = arg->v.sval; }
        return nd; }
    case TT_NOTANY: {
        IR_t * nd = IR_node_alloc(pg, IR_MATCH_NOTANY); γ_to(nd, succ); ω_to(nd, fail);
        if (t->n > 0 && t->c[0]) {
            const tree_t * arg = t->c[0];
            if (arg->t == TT_VAR || arg->t == TT_KEYWORD) { IR_LIT(nd).sval = arg->v.sval; IR_LIT(nd).dval = 1.0; }
            else IR_LIT(nd).sval = arg->v.sval; }
        return nd; }
    case TT_POS: {
        IR_t * nd = IR_node_alloc(pg, IR_MATCH_POS); γ_to(nd, succ); ω_to(nd, fail);
        if (t->n > 0 && t->c[0]) {
            const tree_t * arg = t->c[0];
            if (arg->t == TT_VAR || arg->t == TT_KEYWORD) { IR_LIT(nd).sval = arg->v.sval; IR_LIT(nd).dval = 2.0; }
            else IR_LIT(nd).ival = arg->v.ival; }
        return nd; }
    case TT_RPOS: {
        IR_t * nd = IR_node_alloc(pg, IR_MATCH_POS); γ_to(nd, succ); ω_to(nd, fail);
        IR_LIT(nd).sval = "r";
        if (t->n > 0 && t->c[0]) {
            const tree_t * arg = t->c[0];
            if (arg->t == TT_VAR || arg->t == TT_KEYWORD) { IR_LIT(nd).sval = arg->v.sval; IR_LIT(nd).dval = 1.0; }
            else IR_LIT(nd).ival = arg->v.ival; }
        return nd; }
    case TT_LEN: {
        IR_t * nd = IR_node_alloc(pg, IR_MATCH_LEN); γ_to(nd, succ); ω_to(nd, fail);
        if (t->n > 0 && t->c[0]) {
            const tree_t * arg = t->c[0];
            if (arg->t == TT_VAR || arg->t == TT_KEYWORD) { IR_LIT(nd).sval = arg->v.sval; IR_LIT(nd).dval = 1.0; }
            else IR_LIT(nd).ival = arg->v.ival; }
        return nd; }
    case TT_TAB: {
        IR_t * nd = IR_node_alloc(pg, IR_MATCH_TAB); γ_to(nd, succ); ω_to(nd, fail);
        if (t->n > 0 && t->c[0]) {
            const tree_t * arg = t->c[0];
            if (arg->t == TT_VAR || arg->t == TT_KEYWORD) { IR_LIT(nd).sval = arg->v.sval; IR_LIT(nd).dval = 2.0; }
            else IR_LIT(nd).ival = arg->v.ival; }
        return nd; }
    case TT_RTAB: {
        IR_t * nd = IR_node_alloc(pg, IR_MATCH_RTAB); γ_to(nd, succ); ω_to(nd, fail);
        IR_LIT(nd).sval = "r";
        if (t->n > 0 && t->c[0]) {
            const tree_t * arg = t->c[0];
            if (arg->t == TT_VAR || arg->t == TT_KEYWORD) { IR_LIT(nd).sval = arg->v.sval; IR_LIT(nd).dval = 1.0; }
            else IR_LIT(nd).ival = arg->v.ival; }
        return nd; }
    case TT_ARBNO: {
        IR_t * nd = IR_node_alloc(pg, IR_MATCH_ARBNO); γ_to(nd, succ); ω_to(nd, fail);
        bb_arbno_state_t * az = (bb_arbno_state_t *) calloc(1, sizeof *az);
        if (az) {
            extern IR_graph_t * sno_pat_graph_fwd(const tree_t *);
            az->inner = sno_pat_graph_fwd((t->n > 0) ? t->c[0] : NULL);
            az->cap = 64; az->pos_stack = (int *) calloc(64, sizeof(int));
            (void)(az);
        }
        return nd; }
    case TT_ALT: {
        lc_vec av; lc_vec_init(&av, (int) sizeof(const tree_t *));
        const tree_t * cur = t;
        while (cur && cur->t == TT_ALT) {
            const tree_t * lc2 = (cur->n > 0) ? cur->c[0] : NULL;
            const tree_t * rc2 = (cur->n > 1) ? cur->c[1] : NULL;
            lc_vec_push(&av, &rc2); cur = lc2;
        }
        if (cur) lc_vec_push(&av, &cur);
        const tree_t ** alts = (const tree_t **) av.data; int na = av.n;
        for (int li = 0, ri = na-1; li < ri; li++, ri--) {
            const tree_t * tmp = alts[li]; alts[li] = alts[ri]; alts[ri] = tmp; }
        if (na < 2) return lower_pat_node(pg, alts[0], succ, fail);
        IR_t * cont = succ;
        IR_t * final_alt = IR_node_alloc(pg, IR_MATCH_ALT); γ_to(final_alt, succ); ω_to(final_alt, fail);
        IR_t * last_entry = lower_pat_node(pg, alts[na-1], final_alt, fail);
        IR_t * prev_entry = last_entry;
        IR_t * prev_alt = final_alt;
        for (int i = na-2; i >= 1; i--) {
            IR_t * alt_nd = IR_node_alloc(pg, IR_MATCH_ALT); γ_to(alt_nd, final_alt); ω_to(alt_nd, prev_entry);
            IR_t * e = lower_pat_node(pg, alts[i], alt_nd, prev_entry);
            prev_entry = e; prev_alt = alt_nd; }
        (void)prev_alt;
        IR_t * first_entry = lower_pat_node(pg, alts[0], prev_alt, prev_entry);
        return first_entry; }
    case TT_CAPT_COND_ASGN: {
        IR_t * nd = IR_node_alloc(pg, IR_MATCH_ASSIGN_COND); γ_to(nd, succ); ω_to(nd, fail);
        const char * vn = (t->n > 1 && t->c[1]) ? t->c[1]->v.sval : "";
        IR_LIT(nd).sval = (char *) vn;
        if (t->n > 0 && t->c[0]) {
            IR_t * pe = lower_pat_node(pg, t->c[0], nd, fail);
            ir_operand_push(nd, pe);
        }
        return nd; }
    case TT_CAPT_IMMED_ASGN: {
        IR_t * nd = IR_node_alloc(pg, IR_MATCH_ASSIGN_IMM); γ_to(nd, succ); ω_to(nd, fail);
        const char * vn = (t->n > 1 && t->c[1]) ? t->c[1]->v.sval : "";
        IR_LIT(nd).sval = (char *) vn;
        if (t->n > 0 && t->c[0]) {
            IR_t * pe = lower_pat_node(pg, t->c[0], nd, fail);
            ir_operand_push(nd, pe);
        }
        return nd; }
    case TT_SEQ: {
        const tree_t * rc = (t->n > 1) ? t->c[1] : NULL;
        const tree_t * lc = (t->n > 0) ? t->c[0] : NULL;
        int rc_is_capture = rc && (rc->t == TT_CAPT_COND_ASGN || rc->t == TT_CAPT_IMMED_ASGN);
        int lc_is_capture = lc && (lc->t == TT_CAPT_COND_ASGN || lc->t == TT_CAPT_IMMED_ASGN);
        int rc_is_fail = rc && rc->t == TT_VAR && rc->v.sval && (!strcmp(rc->v.sval,"FAIL")||!strcmp(rc->v.sval,"fail"));
        if (rc_is_capture || rc_is_fail) {
            int need_cat = succ && succ->op == IR_SUCCEED && succ == pg->all[0];
            IR_t * cat = need_cat ? IR_node_alloc(pg, IR_MATCH_CAT) : succ;
            if (need_cat) { γ_to(cat, succ); ω_to(cat, fail); }
            IR_t * re = lower_pat_node(pg, rc, cat, fail);
            IR_t * le_tail = NULL;
            IR_t * le = lower_pat_node_tail(pg, lc, re ? re : cat, fail, &le_tail);
            if (re && le_tail) {
                if (is_pat_consumer(le_tail->op)) ω_to(re, le_tail);
                else if ((le_tail->op == IR_MATCH_ASSIGN_COND || le_tail->op == IR_MATCH_ASSIGN_IMM) && le_tail->n_operands > 0)
                    ω_to(re, le_tail->operands[0]);
            }
            return le;
        }
        if (lc_is_capture) {
            int need_cat = succ && succ->op == IR_SUCCEED && succ == pg->all[0];
            IR_t * cat = need_cat ? IR_node_alloc(pg, IR_MATCH_CAT) : succ;
            if (need_cat) { γ_to(cat, succ); ω_to(cat, fail); }
            IR_t * re = lower_pat_node(pg, rc, cat, fail);
            IR_t * le = lower_pat_node(pg, lc, re ? re : cat, fail);
            if (le && re) {
                IR_t * btgt = (le->n_operands > 0) ? le->operands[0] : NULL;
                if (btgt) ω_to(re, btgt);
            }
            return le;
        }
        int lc_has_capture = 0;
        if (lc) { const tree_t * q = lc;
            while (q && q->t == TT_SEQ) q = (q->n > 1) ? q->c[1] : NULL;
            if (q && (q->t == TT_CAPT_COND_ASGN || q->t == TT_CAPT_IMMED_ASGN || q->t == TT_DEFER)) lc_has_capture = 1;
            if (q && q->t == TT_VAR && q->v.sval && !sno_pat_builtin(q->v.sval)) lc_has_capture = 1; }
        if (succ && succ->op == IR_SUCCEED && succ == pg->all[0]) lc_has_capture = 1;
        if (lc_has_capture) {
            IR_t * cat = IR_node_alloc(pg, IR_MATCH_CAT); γ_to(cat, succ); ω_to(cat, fail);
            IR_t * re_tail = NULL;
            IR_t * re = lower_pat_node_tail(pg, rc, cat, fail, &re_tail);
            IR_t * le_tail = NULL;
            IR_t * le = lower_pat_node_tail(pg, lc, re ? re : cat, fail, &le_tail);
            if (re_tail && le_tail) {
                IR_t * btgt = NULL;
                if ((le_tail->op == IR_MATCH_ASSIGN_COND || le_tail->op == IR_MATCH_ASSIGN_IMM) && le_tail->n_operands > 0)
                    btgt = le_tail->operands[0];
                else if (le_tail->op == IR_MATCH_DEFER)
                    btgt = le_tail;
                else if (is_pat_consumer(le_tail->op))
                    btgt = le_tail;
                if (btgt) ω_to(re_tail, btgt);
            }
            return le;
        }
        IR_t * le_tail = NULL;
        IR_t * re_tail = NULL;
        IR_t * re = lower_pat_node_tail(pg, rc, succ, fail, &re_tail);
        IR_t * le = lower_pat_node_tail(pg, lc, re ? re : succ, fail, &le_tail);
        if (re_tail && le_tail && is_pat_consumer(le_tail->op)) ω_to(re_tail, le_tail);
        return le; }
    case TT_FENCE: {
        if (t->n > 0 && t->c[0]) {
            IR_t * seal = IR_node_alloc(pg, IR_MATCH_FENCE); γ_to(seal, succ); ω_to(seal, fail);
            return lower_pat_node(pg, t->c[0], seal, fail); }
        IR_t * nd = IR_node_alloc(pg, IR_MATCH_FENCE); γ_to(nd, succ); ω_to(nd, fail); return nd; }
    case TT_DEFER: {
        IR_t * nd = IR_node_alloc(pg, IR_MATCH_DEFER); γ_to(nd, succ); ω_to(nd, fail);
        const char * nm = (t->n > 0 && t->c[0]) ? t->c[0]->v.sval : "?";
        IR_LIT(nd).sval = (char *) nm; IR_LIT(nd).ival = 1; return nd; }
    case TT_FNC: {
        IR_t * nd = IR_node_alloc(pg, IR_MATCH_DEFER); γ_to(nd, succ); ω_to(nd, fail);
        IR_LIT(nd).sval = t->v.sval; return nd; }
    default: {
        IR_t * nd = IR_node_alloc(pg, IR_MATCH_DEFER); γ_to(nd, succ); ω_to(nd, fail);
        IR_LIT(nd).sval = "?"; return nd; }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_pat_has_fnc(const tree_t * t) {
    if (!t) return 0;
    if (t->t == TT_FNC) return 1;
    for (int i = 0; i < t->n; i++) if (sno_pat_has_fnc(t->c[i])) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_graph_t * lower_pat_graph(const tree_t * pat) {
    IR_graph_t * pg = IR_alloc(256, IR_LANG_SNO);
    IR_t * succ = IR_node_alloc(pg, IR_SUCCEED);
    IR_t * fail = IR_node_alloc(pg, IR_FAIL);
    IR_t * entry = lower_pat_node(pg, pat, succ, fail);
    pg->entry = entry ? entry : succ;
    return pg;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * sno_pat_graph_fwd(const tree_t * pat) { return lower_pat_graph(pat); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_graph_t * lower_subj_graph(const char * vname) {
    IR_graph_t * sg = IR_alloc(32, IR_LANG_SNO);
    IR_t * fail = IR_node_alloc(sg, IR_FAIL);
    IR_t * var  = IR_node_alloc(sg, IR_VAR);
    ω_to(var, fail); IR_LIT(var).sval = (char *) vname;
    sg->entry = var;
    return sg;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_graph_t * lower_subj_graph_lit(const char * text) {
    IR_graph_t * sg = IR_alloc(32, IR_LANG_SNO);
    IR_t * fail = IR_node_alloc(sg, IR_FAIL);
    IR_t * lit  = IR_node_alloc(sg, IR_LIT_S);
    ω_to(lit, fail); IR_LIT(lit).sval = (char *) text;
    sg->entry = lit;
    return sg;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_graph_t * lower_repl_graph(const tree_t * repl) {
    IR_graph_t * rg = IR_alloc(32, IR_LANG_SNO);
    IR_t * fail = IR_node_alloc(rg, IR_FAIL);
    IR_t * lit  = IR_node_alloc(rg, IR_LIT_S);
    ω_to(lit, fail); IR_LIT(lit).sval = (repl && repl->v.sval) ? repl->v.sval : "";
    rg->entry = lit;
    return rg;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_is_pat_elem(tree_e tt) {
    switch (tt) {
    case TT_POS: case TT_RPOS: case TT_ARB: case TT_ARBNO: case TT_REM:
    case TT_ANY: case TT_NOTANY: case TT_BREAK: case TT_SPAN: case TT_BREAKX:
    case TT_LEN: case TT_TAB: case TT_RTAB: case TT_BAL: case TT_FENCE: case TT_ABORT:
    case TT_CAPT_COND_ASGN: case TT_CAPT_IMMED_ASGN: case TT_CAPT_CURSOR: case TT_ALT:
        return 1;
    default: return 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_has_pat(const tree_t * t) {
    if (!t) return 0;
    if (sno_is_pat_elem(t->t)) return 1;
    for (int i = 0; i < t->n; i++) if (sno_has_pat(t->c[i])) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_has_dc(const tree_t * t) {
    if (!t) return 0;
    if (t->t == TT_DEFER || t->t == TT_CAPT_COND_ASGN || t->t == TT_CAPT_IMMED_ASGN || t->t == TT_CAPT_CURSOR) return 1;
    for (int i = 0; i < t->n; i++) if (sno_has_dc(t->c[i])) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_has_idx(const tree_t * t) {
    if (!t) return 0;
    if (t->t == TT_IDX || t->t == TT_INDIRECT) return 1;
    for (int i = 0; i < t->n; i++) if (sno_has_idx(t->c[i])) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_idx_operand_ok(const tree_t * o) {
    if (!o) return 0;
    if (o->t == TT_VAR || o->t == TT_KEYWORD || o->t == TT_ILIT || o->t == TT_FLIT) return 1;
    if (o->t == TT_IDX && o->n >= 2 && o->c[0] && o->c[0]->t == TT_VAR && o->c[1] && (o->c[1]->t == TT_ILIT || o->c[1]->t == TT_VAR)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_arith_idx_lowerable(const tree_t * t) {
    if (!t || !lc_is_binop(t->t) || t->n < 2) return 0;
    return sno_idx_operand_ok(t->c[0]) && sno_idx_operand_ok(t->c[1]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_pat_kind(const tree_t * t) {
    const char * s; int k0, k1;
    if (!t) return 1;
    switch (t->t) {
    case TT_QLIT: case TT_ILIT: case TT_FLIT: case TT_CSET: case TT_NUL: case TT_ARB: case TT_REM: case TT_FAIL: case TT_SUCCEED: case TT_FENCE: case TT_ABORT: case TT_BAL: case TT_CAPT_CURSOR: return 1;
    case TT_DEFER: return 2;
    case TT_INDIRECT: case TT_FNC: return 3;
    case TT_VAR: s = t->v.sval; return (s && (!strcmp(s,"REM")||!strcmp(s,"rem")||!strcmp(s,"ARB")||!strcmp(s,"arb")||!strcmp(s,"FAIL")||!strcmp(s,"fail")||!strcmp(s,"SUCCEED")||!strcmp(s,"succeed")||!strcmp(s,"FENCE")||!strcmp(s,"fence")||!strcmp(s,"ABORT")||!strcmp(s,"abort")||!strcmp(s,"BAL")||!strcmp(s,"bal"))) ? 1 : 3;
    case TT_SPAN: case TT_ANY: case TT_NOTANY: case TT_BREAK: case TT_BREAKX: case TT_LEN: case TT_POS: case TT_RPOS: case TT_TAB: case TT_RTAB: return (t->n >= 1 && t->c[0] && (t->c[0]->t == TT_QLIT || t->c[0]->t == TT_ILIT || t->c[0]->t == TT_CSET)) ? 1 : ((t->n >= 1 && t->c[0]) ? 2 : 1);
    case TT_SEQ: case TT_CAT: case TT_ALT: k0 = (t->n >= 1) ? sno_pat_kind(t->c[0]) : 1; k1 = (t->n >= 2) ? sno_pat_kind(t->c[1]) : 1; return (k0 > k1) ? k0 : k1;
    case TT_ARBNO: return (t->n >= 1) ? sno_pat_kind(t->c[0]) : 1;
    case TT_CAPT_COND_ASGN: case TT_CAPT_IMMED_ASGN: return (t->n >= 1 && t->c[0]) ? sno_pat_kind(t->c[0]) : 1;
    default: return 3;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_island_walk(const tree_t * t, int parent_variant, int * islands) {
    if (!t) return;
    if (sno_pat_kind(t) < 3) { if (parent_variant) (*islands)++; return; }
    switch (t->t) {
    case TT_SEQ: case TT_CAT: case TT_ALT: if (t->n >= 1) sno_island_walk(t->c[0], 1, islands); if (t->n >= 2) sno_island_walk(t->c[1], 1, islands); break;
    case TT_ARBNO: if (t->n >= 1) sno_island_walk(t->c[0], 1, islands); break;
    case TT_CAPT_COND_ASGN: case TT_CAPT_IMMED_ASGN: if (t->n >= 1) sno_island_walk(t->c[0], 1, islands); break;
    default: break;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_pat_islands(const tree_t * rhs) {
    int islands = 0;
    if (!rhs) return 0;
    if (sno_pat_kind(rhs) < 3) return 1;
    sno_island_walk(rhs, 1, &islands);
    return islands;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_leaf_buildable(const tree_t * t) {
    const char * s; int hit;
    if (!t) return 0;
    if (t->t == TT_QLIT) return 1;
    if ((t->t == TT_CAPT_COND_ASGN || t->t == TT_CAPT_IMMED_ASGN) && t->n > 0 && t->c[0]) return sno_leaf_buildable(t->c[0]);
    if (t->t == TT_VAR && (s = t->v.sval)) {
        hit  = (!strcmp(s,"REM")||!strcmp(s,"rem")||!strcmp(s,"ARB")||!strcmp(s,"arb")||!strcmp(s,"FAIL")||!strcmp(s,"fail"));
        hit |= (!strcmp(s,"SUCCEED")||!strcmp(s,"succeed")||!strcmp(s,"FENCE")||!strcmp(s,"fence")||!strcmp(s,"ABORT")||!strcmp(s,"abort"));
        return hit;
    }
    if (t->n==1 && t->c[0] && t->c[0]->t==TT_QLIT) {
        switch (t->t) { case TT_SPAN: case TT_ANY: case TT_NOTANY: case TT_BREAK: case TT_BREAKX: return 1; default: break; }
    }
    if (t->n==1 && t->c[0] && t->c[0]->t==TT_ILIT) {
        switch (t->t) { case TT_LEN: case TT_POS: case TT_RPOS: case TT_TAB: case TT_RTAB: return 1; default: break; }
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_seq_buildable(const tree_t * t) {
    if (!t) return 0;
    if (t->t == TT_SEQ) return (t->n >= 1 && sno_seq_buildable(t->c[0])) && (t->n >= 2 && sno_leaf_buildable(t->c[1]));
    return sno_leaf_buildable(t);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_seq_has_pat_leaf(const tree_t * t) {
    if (!t) return 0;
    if (t->t == TT_SEQ) return sno_seq_has_pat_leaf(t->c[0]) || (t->n > 1 && t->c[1] && t->c[1]->t != TT_QLIT);
    return (t->t != TT_QLIT);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sno_build_leaf_ir(snx_t * cx, const tree_t * t, IR_t * g, IR_t * w) {
    if (!t) return NULL;
    if ((t->t == TT_CAPT_COND_ASGN || t->t == TT_CAPT_IMMED_ASGN) && t->n > 0 && t->c[0]) {
        IR_t * cap = build(cx, IR_PATTERN_CAPTURE, g, w);
        IR_LIT(cap).sval = (t->n > 1 && t->c[1]) ? t->c[1]->v.sval : (char *) "";
        IR_LIT(cap).ival = (t->t == TT_CAPT_IMMED_ASGN) ? 1 : 0;
        IR_t * inner = sno_build_leaf_ir(cx, t->c[0], cap, w);
        ir_operand_push(cap, inner);
        γ_to(inner, cap);
        return inner;
    }
    if (t->t == TT_QLIT) { IR_t * nd = build(cx, IR_PATTERN_LIT, g, w); IR_LIT(nd).sval = t->v.sval; return nd; }
    if (t->t == TT_VAR && t->v.sval) {
        const char * s = t->v.sval; IR_e pe = (IR_e)0; int hit = 1;
        if      (!strcmp(s,"REM")||!strcmp(s,"rem"))         pe = IR_PATTERN_REM;
        else if (!strcmp(s,"ARB")||!strcmp(s,"arb"))         pe = IR_PATTERN_ARB;
        else if (!strcmp(s,"FAIL")||!strcmp(s,"fail"))       pe = IR_PATTERN_FAIL;
        else if (!strcmp(s,"SUCCEED")||!strcmp(s,"succeed")) pe = IR_PATTERN_SUCCEED;
        else if (!strcmp(s,"FENCE")||!strcmp(s,"fence"))     pe = IR_PATTERN_FENCE;
        else if (!strcmp(s,"ABORT")||!strcmp(s,"abort"))     pe = IR_PATTERN_ABORT;
        else hit = 0;
        if (hit) return build(cx, pe, g, w);
    }
    if (t->n==1 && t->c[0] && t->c[0]->t==TT_QLIT) {
        IR_e pe = (IR_e)0; int hit = 1;
        switch (t->t) { case TT_SPAN: pe=IR_PATTERN_SPAN; break; case TT_ANY: pe=IR_PATTERN_ANY; break;
            case TT_NOTANY: pe=IR_PATTERN_NOTANY; break; case TT_BREAK: pe=IR_PATTERN_BREAK; break;
            case TT_BREAKX: pe=IR_PATTERN_BREAKX; break; default: hit=0; break; }
        if (hit) { IR_t * nd = build(cx, pe, g, w); IR_LIT(nd).sval = t->c[0]->v.sval; return nd; }
    }
    if (t->n==1 && t->c[0] && t->c[0]->t==TT_ILIT) {
        IR_e pe = (IR_e)0; int hit = 1;
        switch (t->t) { case TT_LEN: pe=IR_PATTERN_LEN; break; case TT_POS: pe=IR_PATTERN_POS; break;
            case TT_RPOS: pe=IR_PATTERN_RPOS; break; case TT_TAB: pe=IR_PATTERN_TAB; break;
            case TT_RTAB: pe=IR_PATTERN_RTAB; break; default: hit=0; break; }
        if (hit) { IR_t * nd = build(cx, pe, g, w); IR_LIT(nd).ival = t->c[0]->v.ival; return nd; }
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_seq_flatten_ops(const tree_t * t, lc_vec * out, int * nonleaf) {
    if (!t) return;
    if (t->t == TT_SEQ) { sno_seq_flatten_ops((t->n > 0) ? t->c[0] : NULL, out, nonleaf); sno_seq_flatten_ops((t->n > 1) ? t->c[1] : NULL, out, nonleaf); return; }
    lc_vec_push(out, &t);
    if (t->t == TT_FNC || lc_is_binop(t->t) || is_sno_unop(t->t) || t->t == TT_KEYWORD) *nonleaf = 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sno_concat_chain(snx_t * cx, const tree_t ** ops, int n, IR_t * cont, IR_t * nxt, IR_t ** res) {
    if (n == 1) return lower_expr(cx, ops[0], cont, nxt, res);
    IR_t * op = build(cx, IR_BINOP, cont, nxt); IR_LIT(op).ival = (long long) lc_binop_code(TT_CAT);
    IR_t * lr = NULL, * rr = NULL;
    IR_t * ea = sno_concat_chain(cx, ops, n - 1, NULL, nxt, &lr);
    IR_t * eb = lower_expr(cx, ops[n - 1], op, nxt, &rr);
    γ_to(lr, eb);
    { ir_operand_push(op, lr); ir_operand_push(op, rr); }
    *res = op; return ea;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sno_freeze_break_cap_lit_ir(snx_t * cx, const char * cset, const char * capvar, const char * lit) {
    IR_graph_t * g = cx->g;
    IR_t * PSUCC = IR_node_alloc(g, IR_SUCCEED);
    IR_t * PFAIL = IR_node_alloc(g, IR_FAIL);
    IR_t * brk   = IR_node_alloc(g, IR_MATCH_BREAK);
    IR_t * cap   = IR_node_alloc(g, IR_MATCH_ASSIGN_COND);
    IR_t * litn  = IR_node_alloc(g, IR_MATCH_LIT);
    IR_t * cat   = IR_node_alloc(g, IR_MATCH_CAT);
    IR_LIT(brk).sval = (char *) cset; γ_to(brk, PSUCC); ω_to(brk, PFAIL);
    IR_LIT(cap).sval = (char *) capvar; IR_LIT(cap).ival = 0; ir_operand_push(cap, brk); γ_to(cap, PSUCC); ω_to(cap, PFAIL);
    IR_LIT(litn).sval = (char *) lit; γ_to(litn, PSUCC); ω_to(litn, PFAIL);
    γ_to(cat, PSUCC); ω_to(cat, PFAIL);
    IR_t ** kids = (IR_t **) calloc(2, sizeof(IR_t *)); kids[0] = cap; kids[1] = litn;
    bb_match_kids_state_t * zk = (bb_match_kids_state_t *) calloc(1, sizeof(*zk)); zk->kids = kids; zk->nkids = 2;
    (void)(zk);
    return cat;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_freeze_kids_attach(IR_t * nd, IR_t ** kids, int nkids) {
    IR_t ** kk = (IR_t **) calloc((size_t) nkids, sizeof(IR_t *)); for (int i = 0; i < nkids; i++) kk[i] = kids[i];
    bb_match_kids_state_t * zk = (bb_match_kids_state_t *) calloc(1, sizeof(*zk)); zk->kids = kk; zk->nkids = nkids;
    (void)(zk);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sno_freeze_pat_ir(IR_graph_t * g, const tree_t * t, IR_t * PSUCC, IR_t * PFAIL) {
    if (!t) return PSUCC;
    switch (t->t) {
    case TT_QLIT: { IR_t * nd = IR_node_alloc(g, IR_MATCH_LIT); IR_LIT(nd).sval = t->v.sval; γ_to(nd, PSUCC); ω_to(nd, PFAIL); return nd; }
    case TT_SPAN: case TT_ANY: case TT_NOTANY: case TT_BREAK: case TT_BREAKX: {
        IR_e op = (t->t==TT_SPAN)?IR_MATCH_SPAN:(t->t==TT_ANY)?IR_MATCH_ANY:(t->t==TT_NOTANY)?IR_MATCH_NOTANY:(t->t==TT_BREAK)?IR_MATCH_BREAK:IR_MATCH_BREAKX;
        IR_t * nd = IR_node_alloc(g, op); if (t->n>0 && t->c[0]) IR_LIT(nd).sval = t->c[0]->v.sval; γ_to(nd, PSUCC); ω_to(nd, PFAIL); return nd; }
    case TT_LEN: case TT_POS: case TT_RPOS: case TT_TAB: case TT_RTAB: {
        IR_e op = (t->t==TT_LEN)?IR_MATCH_LEN:(t->t==TT_POS)?IR_MATCH_POS:(t->t==TT_RPOS)?IR_MATCH_POS:(t->t==TT_TAB)?IR_MATCH_TAB:IR_MATCH_RTAB;
        IR_t * nd = IR_node_alloc(g, op); if (t->t==TT_RPOS || t->t==TT_RTAB) IR_LIT(nd).sval = "r"; if (t->n>0 && t->c[0]) IR_LIT(nd).ival = t->c[0]->v.ival; γ_to(nd, PSUCC); ω_to(nd, PFAIL); return nd; }
    case TT_ARB: { IR_t * nd = IR_node_alloc(g, IR_MATCH_ARB); γ_to(nd, PSUCC); ω_to(nd, PFAIL); return nd; }
    case TT_REM: { IR_t * nd = IR_node_alloc(g, IR_MATCH_REM); γ_to(nd, PSUCC); ω_to(nd, PFAIL); return nd; }
    case TT_VAR: { const char * s = t->v.sval ? t->v.sval : ""; IR_e op = IR_MATCH_REM; int hit = 1;
        if      (!strcmp(s,"REM")||!strcmp(s,"rem")) op = IR_MATCH_REM; else if (!strcmp(s,"ARB")||!strcmp(s,"arb")) op = IR_MATCH_ARB;
        else if (!strcmp(s,"FAIL")||!strcmp(s,"fail")) op = IR_FAIL; else if (!strcmp(s,"FENCE")||!strcmp(s,"fence")) op = IR_MATCH_FENCE;
        else if (!strcmp(s,"ABORT")||!strcmp(s,"abort")) op = IR_MATCH_ABORT; else hit = 0;
        if (hit) { IR_t * nd = IR_node_alloc(g, op); γ_to(nd, PSUCC); ω_to(nd, PFAIL); return nd; } return PSUCC; }
    case TT_CAPT_COND_ASGN: case TT_CAPT_IMMED_ASGN: {
        IR_t * nd = IR_node_alloc(g, (t->t==TT_CAPT_IMMED_ASGN)?IR_MATCH_ASSIGN_IMM:IR_MATCH_ASSIGN_COND);
        IR_LIT(nd).sval = (t->n>1 && t->c[1]) ? t->c[1]->v.sval : (char *) ""; γ_to(nd, PSUCC); ω_to(nd, PFAIL);
        IR_t * inner = sno_freeze_pat_ir(g, (t->n>0)?t->c[0]:NULL, nd, PFAIL); ir_operand_push(nd, inner); return nd; }
    case TT_ALT: {
        lc_vec av; lc_vec_init(&av, (int) sizeof(const tree_t *)); const tree_t * cur = t;
        while (cur && cur->t == TT_ALT) { lc_vec_push(&av, (cur->n>1)?&cur->c[1]:&cur); cur = (cur->n>0)?cur->c[0]:NULL; }
        if (cur) lc_vec_push(&av, &cur);
        const tree_t ** alts = (const tree_t **) av.data; int na = av.n;
        for (int li = 0, ri = na-1; li < ri; li++, ri--) { const tree_t * tmp = alts[li]; alts[li] = alts[ri]; alts[ri] = tmp; }
        if (na < 2) return sno_freeze_pat_ir(g, alts[0], PSUCC, PFAIL);
        IR_t * nd = IR_node_alloc(g, IR_MATCH_ALT); γ_to(nd, PSUCC); ω_to(nd, PFAIL);
        IR_t ** kids = (IR_t **) alloca((size_t) na * sizeof(IR_t *));
        for (int i = 0; i < na; i++) kids[i] = sno_freeze_pat_ir(g, alts[i], PSUCC, PFAIL);
        sno_freeze_kids_attach(nd, kids, na); return nd; }
    case TT_SEQ: case TT_CAT: {
        lc_vec lv; lc_vec_init(&lv, (int) sizeof(const tree_t *));
        lc_vec st; lc_vec_init(&st, (int) sizeof(const tree_t *)); lc_vec_push(&st, &t);
        while (st.n > 0) { st.n--; const tree_t * nx = LC_AT(&st, const tree_t *, st.n); if (!nx) break;
            if (nx->t == TT_SEQ || nx->t == TT_CAT) { if (nx->n>1) lc_vec_push(&st, &nx->c[1]); if (nx->n>0) lc_vec_push(&st, &nx->c[0]); }
            else lc_vec_push(&lv, &nx); }
        const tree_t ** parts = (const tree_t **) lv.data; int np = lv.n;
        if (np < 2) return sno_freeze_pat_ir(g, (np==1)?parts[0]:NULL, PSUCC, PFAIL);
        IR_t * nd = IR_node_alloc(g, IR_MATCH_CAT); γ_to(nd, PSUCC); ω_to(nd, PFAIL);
        IR_t ** kids = (IR_t **) alloca((size_t) np * sizeof(IR_t *));
        for (int i = 0; i < np; i++) kids[i] = sno_freeze_pat_ir(g, parts[i], PSUCC, PFAIL);
        sno_freeze_kids_attach(nd, kids, np); return nd; }
    default: { IR_t * nd = IR_node_alloc(g, IR_MATCH_REM); γ_to(nd, PSUCC); ω_to(nd, PFAIL); return nd; }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sno_freeze_pat_graph_entry(IR_graph_t * g, const tree_t * rhs) {
    IR_t * PSUCC = IR_node_alloc(g, IR_SUCCEED); IR_t * PFAIL = IR_node_alloc(g, IR_FAIL);
    return sno_freeze_pat_ir(g, rhs, PSUCC, PFAIL);
}
#define FZ5_MAX 1024
static const char * g_fz5_name[FZ5_MAX];
static IR_t       * g_fz5_head[FZ5_MAX];
static int          g_fz5_acount[FZ5_MAX];
static int          g_fz5_inl[FZ5_MAX];
static int          g_fz5_n;
static int          g_fz5_unsafe;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void fz5_reset(void) { g_fz5_n = 0; g_fz5_unsafe = 0; }
static int fz5_find(const char * nm) { for (int i = 0; i < g_fz5_n; i++) if (g_fz5_name[i] && nm && !strcmp(g_fz5_name[i], nm)) return i; return -1; }
static int fz5_intern(const char * nm) { int k = fz5_find(nm); if (k >= 0) return k; if (g_fz5_n >= FZ5_MAX) return -1; k = g_fz5_n++; g_fz5_name[k] = nm; g_fz5_head[k] = (IR_t *) 0; g_fz5_acount[k] = 0; g_fz5_inl[k] = 0; return k; }
static void fz5_count_assign(const char * nm) { if (!nm || !nm[0]) return; int k = fz5_intern(nm); if (k >= 0) g_fz5_acount[k]++; }
static void fz5_note_frozen(const char * nm, IR_t * head) { if (!nm || !nm[0]) return; int k = fz5_intern(nm); if (k >= 0) g_fz5_head[k] = head; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void fz5_finalize(IR_graph_t * g) {
    for (int i = 0; g && i < g->n; i++) { IR_t * nd = g->all[i]; if (!nd) continue;
        if (nd->op == IR_INDIRECT_ASSIGN_LIT_S) g_fz5_unsafe = 1;
        if (ir_is_call_kind(nd->op)) { const char * cn = IR_LIT(nd).sval; if (cn && (!strcmp(cn,"EVAL")||!strcmp(cn,"eval")||!strcmp(cn,"CODE")||!strcmp(cn,"code")||!strcmp(cn,"CONVERT")||!strcmp(cn,"convert"))) g_fz5_unsafe = 1; } }
    for (int i = 0; i < g_fz5_n; i++) g_fz5_inl[i] = (!g_fz5_unsafe && g_fz5_head[i] && g_fz5_acount[i] == 1) ? 1 : 0;
    if (getenv("SCRIP_FZ_DEBUG")) { fprintf(stderr, "FZ5-ANALYZE unsafe=%d names=%d\n", g_fz5_unsafe, g_fz5_n); for (int i = 0; i < g_fz5_n; i++) if (g_fz5_head[i]) fprintf(stderr, "FZ5  name=%s assigns=%d frozen=1 inlinable=%d\n", g_fz5_name[i], g_fz5_acount[i], g_fz5_inl[i]); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * fz_inlinable_head(const char * nm) { int k = fz5_find(nm); return (k >= 0 && g_fz5_inl[k]) ? g_fz5_head[k] : (IR_t *) 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_assign(snx_t * cx, const char * lhs, const tree_t * rhs, IR_t * γ, IR_t * ω, int is_kw) {
    fz5_count_assign(lhs);
    if (rhs && getenv("SCRIP_FZ_DEBUG")) fprintf(stderr, "FZ-CLASSIFY lhs=%s tt=%d kind=%d islands=%d\n", lhs ? lhs : "?", (int) rhs->t, sno_pat_kind(rhs), sno_pat_islands(rhs));
    if (rhs && rhs->t == TT_ALT) {
        lc_vec qv; lc_vec_init(&qv, (int) sizeof(const tree_t *)); int allq = 1;
        lc_vec st2; lc_vec_init(&st2, (int) sizeof(const tree_t *)); lc_vec_push(&st2, &rhs);
        while (st2.n > 0 && allq) {
            st2.n--; const tree_t * nd = LC_AT(&st2, const tree_t *, st2.n); if (!nd) { allq = 0; break; }
            if (nd->t == TT_ALT) {
                if (nd->n > 1) lc_vec_push(&st2, &nd->c[1]);
                if (nd->n > 0) lc_vec_push(&st2, &nd->c[0]);
            } else if (nd->t == TT_QLIT) { lc_vec_push(&qv, &nd); }
            else { allq = 0; }
        }
        const tree_t ** qleaves = (const tree_t **) qv.data; int nq = qv.n;
        if (allq && nq >= 2) {
            IR_t * dtp = build(cx, IR_DTP_ASSIGN, γ, ω); IR_LIT(dtp).sval = (char *) lhs;
            IR_t ** lits = (IR_t **) calloc((size_t) nq, sizeof(IR_t *));
            lits[0] = build(cx, IR_PATTERN_LIT, ω, ω); IR_LIT(lits[0]).sval = qleaves[0]->v.sval;
            lits[1] = build(cx, IR_PATTERN_LIT, ω, ω); IR_LIT(lits[1]).sval = qleaves[1]->v.sval;
            IR_t * cur_alt = build(cx, IR_PATTERN_ALT, dtp, ω);
            ir_operand_push(cur_alt, lits[0]); ir_operand_push(cur_alt, lits[1]);
            γ_to(lits[0], lits[1]); γ_to(lits[1], cur_alt);
            for (int i = 2; i < nq; i++) {
                lits[i] = build(cx, IR_PATTERN_LIT, ω, ω); IR_LIT(lits[i]).sval = qleaves[i]->v.sval;
                γ_to(cur_alt, lits[i]);
                IR_t * next_alt = build(cx, IR_PATTERN_ALT, dtp, ω);
                ir_operand_push(next_alt, cur_alt); ir_operand_push(next_alt, lits[i]);
                γ_to(lits[i], next_alt); cur_alt = next_alt;
            }
            γ_to(cur_alt, dtp); ir_operand_push(dtp, cur_alt);
            return lits[0];
        }
    }
    if (rhs && rhs->t == TT_ALT && rhs->n == 2) {
        const tree_t * a0 = rhs->c[0]; const tree_t * a1 = rhs->c[1];
        int dirty0 = sno_has_dc(a0), dirty1 = sno_has_dc(a1);
        if (a0 && a1 && a0->t == TT_SEQ && a1->t == TT_SEQ && !dirty0 && !dirty1) {
            IR_t * asn = build(cx, IR_ASSIGN, γ, ω); IR_LIT(asn).sval = (char *) lhs;
            IR_t * alt = build(cx, IR_ALT, asn, ω);
            IR_t * s1  = build(cx, IR_SEQ, alt, ω);
            (void)(sno_arg_block(cx, (a1->n > 0) ? a1->c[0] : NULL));
            IR_LIT(s1).ival     = (int64_t)(intptr_t) sno_arg_block(cx, (a1->n > 1) ? a1->c[1] : NULL);
            IR_t * s0  = build(cx, IR_SEQ, alt, s1);
            (void)(sno_arg_block(cx, (a0->n > 0) ? a0->c[0] : NULL));
            IR_LIT(s0).ival     = (int64_t)(intptr_t) sno_arg_block(cx, (a0->n > 1) ? a0->c[1] : NULL);
            return s0;
        }
        IR_t * asn = IR_node_alloc(cx->g, IR_ASSIGN); IR_LIT(asn).sval = (char *) lhs;
        IR_node_alloc(cx->g, IR_ALT);
        if (a1 && a1->t == TT_SEQ) IR_node_alloc(cx->g, IR_SEQ);
        return NULL;
    }
    if (rhs && rhs->n == 1 && rhs->c[0] && rhs->c[0]->t == TT_QLIT) {
        IR_e pe = IR_ALT; int pehit = 1;
        switch (rhs->t) {
        case TT_SPAN: pe = IR_PATTERN_SPAN; break; case TT_ANY: pe = IR_PATTERN_ANY; break;
        case TT_NOTANY: pe = IR_PATTERN_NOTANY; break; case TT_BREAK: pe = IR_PATTERN_BREAK; break;
        case TT_BREAKX: pe = IR_PATTERN_BREAKX; break; default: pehit = 0; break; }
        if (pehit) {
            IR_t * dtp = build(cx, IR_DTP_ASSIGN, γ, ω); IR_LIT(dtp).sval = (char *) lhs;
            IR_t * pat = build(cx, pe, dtp, ω); IR_LIT(pat).sval = rhs->c[0]->v.sval;
            ir_operand_push(dtp, pat);
            return pat;
        }
    }
    if (rhs && rhs->n == 1 && rhs->c[0] && rhs->c[0]->t == TT_VAR && rhs->c[0]->v.sval) {
        IR_e pe = IR_ALT; int pehit = 1;
        switch (rhs->t) {
        case TT_SPAN: pe = IR_PATTERN_SPAN; break; case TT_ANY: pe = IR_PATTERN_ANY; break;
        case TT_NOTANY: pe = IR_PATTERN_NOTANY; break; case TT_BREAK: pe = IR_PATTERN_BREAK; break;
        case TT_BREAKX: pe = IR_PATTERN_BREAKX; break; default: pehit = 0; break; }
        if (pehit) {
            IR_t * dtp = build(cx, IR_DTP_ASSIGN, γ, ω); IR_LIT(dtp).sval = (char *) lhs;
            IR_t * pat = build(cx, pe, dtp, ω); IR_LIT(pat).sval = rhs->c[0]->v.sval; IR_LIT(pat).dval = 1.0;
            ir_operand_push(dtp, pat);
            return pat;
        }
    }
    if (rhs && rhs->n == 1 && rhs->c[0] && rhs->c[0]->t == TT_ILIT) {
        IR_e pe = IR_ALT; int pehit = 1;
        switch (rhs->t) {
        case TT_LEN: pe = IR_PATTERN_LEN; break; case TT_POS: pe = IR_PATTERN_POS; break;
        case TT_RPOS: pe = IR_PATTERN_RPOS; break; case TT_TAB: pe = IR_PATTERN_TAB; break;
        case TT_RTAB: pe = IR_PATTERN_RTAB; break; default: pehit = 0; break; }
        if (pehit) {
            IR_t * dtp = build(cx, IR_DTP_ASSIGN, γ, ω); IR_LIT(dtp).sval = (char *) lhs;
            IR_t * pat = build(cx, pe, dtp, ω); IR_LIT(pat).ival = rhs->c[0]->v.ival;
            ir_operand_push(dtp, pat);
            return pat;
        }
    }
    if (rhs && (rhs->t == TT_CAPT_COND_ASGN || rhs->t == TT_CAPT_IMMED_ASGN)) {
        if (sno_leaf_buildable(rhs)) {
            IR_t * head = sno_freeze_pat_graph_entry(cx->g, rhs);
            IR_t * ref = build(cx, IR_REF_INVARIANT, γ, ω); IR_LIT(ref).sval = (char *) lhs;
            bb_operand_aux_set(cx->g, ref, &head, 1);
            fz5_note_frozen(lhs, head);
            return ref;
        }
        IR_t * asn = IR_node_alloc(cx->g, IR_ASSIGN); IR_LIT(asn).sval = (char *) lhs;
        return NULL;
    }
    if (rhs && rhs->t == TT_SEQ && sno_has_pat(rhs) && sno_pat_kind(rhs) == 1 && !sno_seq_buildable(rhs)) {
        IR_t * head = sno_freeze_pat_graph_entry(cx->g, rhs);
        IR_t * ref = build(cx, IR_REF_INVARIANT, γ, ω); IR_LIT(ref).sval = (char *) lhs;
        bb_operand_aux_set(cx->g, ref, &head, 1);
        fz5_note_frozen(lhs, head);
        return ref;
    }
    if (rhs && rhs->t == TT_SEQ && sno_seq_buildable(rhs) && sno_seq_has_pat_leaf(rhs)) {
        lc_vec lv3; lc_vec_init(&lv3, (int) sizeof(const tree_t *));
        lc_vec st3; lc_vec_init(&st3, (int) sizeof(const tree_t *)); lc_vec_push(&st3, &rhs);
        while (st3.n > 0) {
            st3.n--; const tree_t * nd = LC_AT(&st3, const tree_t *, st3.n); if (!nd) break;
            if (nd->t == TT_SEQ) {
                if (nd->n > 1) lc_vec_push(&st3, &nd->c[1]);
                if (nd->n > 0) lc_vec_push(&st3, &nd->c[0]);
            } else { lc_vec_push(&lv3, &nd); }
        }
        const tree_t ** leaves = (const tree_t **) lv3.data; int nl = lv3.n;
        if (nl == 2) {
            const tree_t * l0 = leaves[0]; const tree_t * l1 = leaves[1];
            if (l0 && l1 && l0->t == TT_CAPT_COND_ASGN && l0->n >= 2 && l0->c[0] && l0->c[0]->t == TT_BREAK
                && l0->c[0]->n == 1 && l0->c[0]->c[0] && l0->c[0]->c[0]->t == TT_QLIT && l1->t == TT_QLIT) {
                const char * cset = l0->c[0]->c[0]->v.sval; const char * capvar = l0->c[1] ? l0->c[1]->v.sval : ""; const char * lit = l1->v.sval;
                IR_t * cat = sno_freeze_break_cap_lit_ir(cx, cset, capvar, lit);
                IR_t * ref = build(cx, IR_REF_INVARIANT, γ, ω); IR_LIT(ref).sval = (char *) lhs;
                bb_operand_aux_set(cx->g, ref, &cat, 1);
                fz5_note_frozen(lhs, cat);
                return ref;
            }
        }
        if (nl >= 2) {
            IR_t * dtp = build(cx, IR_DTP_ASSIGN, γ, ω); IR_LIT(dtp).sval = (char *) lhs;
            IR_t ** pats = (IR_t **) calloc((size_t) nl, sizeof(IR_t *));
            pats[0] = sno_build_leaf_ir(cx, leaves[0], ω, ω);
            pats[1] = sno_build_leaf_ir(cx, leaves[1], ω, ω);
            IR_t * cur_cat = build(cx, IR_PATTERN_CAT, dtp, ω);
            ir_operand_push(cur_cat, pats[0]); ir_operand_push(cur_cat, pats[1]);
            γ_to(pats[0], pats[1]); γ_to(pats[1], cur_cat);
            for (int i = 2; i < nl; i++) {
                pats[i] = sno_build_leaf_ir(cx, leaves[i], ω, ω);
                γ_to(cur_cat, pats[i]);
                IR_t * next_cat = build(cx, IR_PATTERN_CAT, dtp, ω);
                ir_operand_push(next_cat, cur_cat); ir_operand_push(next_cat, pats[i]);
                γ_to(pats[i], next_cat); cur_cat = next_cat;
            }
            γ_to(cur_cat, dtp); ir_operand_push(dtp, cur_cat);
            return pats[0];
        }
    }
    if (rhs && sno_has_pat(rhs)) {
        IR_t * asn = IR_node_alloc(cx->g, IR_ASSIGN_CONCAT); IR_LIT(asn).sval = (char *) lhs;
        IR_node_alloc(cx->g, IR_SEQ);
        return NULL;
    }
    if (rhs && rhs->t == TT_VAR && rhs->v.sval) {
        IR_e pe = (IR_e)0; int pehit = 1;
        if      (!strcmp(rhs->v.sval,"REM")     || !strcmp(rhs->v.sval,"rem"))     pe = IR_PATTERN_REM;
        else if (!strcmp(rhs->v.sval,"FAIL")    || !strcmp(rhs->v.sval,"fail"))    pe = IR_PATTERN_FAIL;
        else if (!strcmp(rhs->v.sval,"SUCCEED") || !strcmp(rhs->v.sval,"succeed")) pe = IR_PATTERN_SUCCEED;
        else if (!strcmp(rhs->v.sval,"FENCE")   || !strcmp(rhs->v.sval,"fence"))   pe = IR_PATTERN_FENCE;
        else if (!strcmp(rhs->v.sval,"ABORT")   || !strcmp(rhs->v.sval,"abort"))   pe = IR_PATTERN_ABORT;
        else if (!strcmp(rhs->v.sval,"ARB")     || !strcmp(rhs->v.sval,"arb"))     pe = IR_PATTERN_ARB;
        else pehit = 0;
        if (pehit) {
            IR_t * dtp = build(cx, IR_DTP_ASSIGN, γ, ω); IR_LIT(dtp).sval = (char *) lhs;
            IR_t * pat = build(cx, pe, dtp, ω);
            ir_operand_push(dtp, pat);
            return pat;
        }
    }
    if (!rhs) {
        IR_e op = is_kw ? IR_ASSIGN : IR_ASSIGN_LIT_S;
        IR_t * asn = build(cx, op, γ, ω); IR_LIT(asn).sval = (char *) lhs;
        IR_t * lit = build(cx, IR_LIT_S, asn, ω); IR_LIT(lit).sval = "";
        return lit;
    }
    switch (rhs->t) {
    case TT_QLIT: {
        IR_e op = is_kw ? IR_ASSIGN : IR_ASSIGN_LIT_S;
        IR_t * asn = build(cx, op, γ, ω); IR_LIT(asn).sval = (char *) lhs;
        IR_t * lit = build(cx, IR_LIT_S, asn, ω); IR_LIT(lit).sval = rhs->v.sval;
        return lit; }
    case TT_ILIT: {
        IR_e op = is_kw ? IR_ASSIGN : IR_ASSIGN_LIT_I;
        IR_t * asn = build(cx, op, γ, ω); IR_LIT(asn).sval = (char *) lhs;
        IR_t * lit = build(cx, IR_LIT_I, asn, ω); IR_LIT(lit).ival = rhs->v.ival;
        return lit; }
    case TT_FLIT: {
        IR_t * asn = build(cx, IR_ASSIGN, γ, ω); IR_LIT(asn).sval = (char *) lhs;
        IR_t * lit = build(cx, IR_LIT_F, asn, ω); IR_LIT(lit).dval = rhs->v.dval;
        return lit; }
    case TT_NUL: {
        IR_e op = is_kw ? IR_ASSIGN : IR_ASSIGN_LIT_S;
        IR_t * asn = build(cx, op, γ, ω); IR_LIT(asn).sval = (char *) lhs;
        IR_t * lit = build(cx, IR_LIT_NUL, asn, ω); return lit; }
    case TT_VAR: {
        IR_e op = is_kw ? IR_ASSIGN : IR_ASSIGN_VAR;
        IR_t * asn = build(cx, op, γ, ω); IR_LIT(asn).sval = (char *) lhs;
        IR_t * var = build(cx, IR_VAR, asn, ω); IR_LIT(var).sval = rhs->v.sval;
        return var; }
    case TT_NAME: {
        IR_e op = is_kw ? IR_ASSIGN : IR_ASSIGN_VAR;
        IR_t * asn = build(cx, op, γ, ω); IR_LIT(asn).sval = (char *) lhs;
        IR_t * var = build(cx, IR_VAR, asn, ω); IR_LIT(var).sval = "";
        return var; }
    case TT_FNC: {
        const char * nm = rhs->v.sval ? rhs->v.sval : "?";
        IR_t * asn = build(cx, IR_ASSIGN_CALL, γ, ω); IR_LIT(asn).sval = (char *) lhs;
        IR_t * call = build(cx, IR_CALL, asn, ω); IR_LIT(call).sval = nm; IR_LIT(call).ival = (long long) rhs->n;
        sno_call_channels(cx, call, rhs);
        return call; }
    case TT_SEQ: {
        if (sno_has_idx(rhs)) {
            IR_t * asn = IR_node_alloc(cx->g, IR_ASSIGN_CONCAT); IR_LIT(asn).sval = (char *) lhs;
            IR_node_alloc(cx->g, IR_SEQ);
            return NULL;
        }
        { lc_vec ov; lc_vec_init(&ov, (int) sizeof(const tree_t *)); int nonleaf = 0; sno_seq_flatten_ops(rhs, &ov, &nonleaf);
          if (nonleaf && ov.n >= 1) { const tree_t ** ops = (const tree_t **) ov.data; IR_t * asn = build(cx, IR_ASSIGN, γ, ω); IR_LIT(asn).sval = (char *) lhs; IR_t * res = NULL; IR_t * entry = sno_concat_chain(cx, ops, ov.n, asn, ω, &res); ir_operand_push(asn, res); return entry; } }
        lc_vec lv; lc_vec_init(&lv, (int) sizeof(const tree_t *)); int fold = 1;
        lc_vec stv; lc_vec_init(&stv, (int) sizeof(const tree_t *)); lc_vec_push(&stv, &rhs);
        while (stv.n > 0 && fold) {
            stv.n--; const tree_t * nd = LC_AT(&stv, const tree_t *, stv.n); if (!nd) { fold = 0; break; }
            if (nd->t == TT_SEQ) {
                const tree_t * r1 = (nd->n > 1) ? nd->c[1] : NULL; lc_vec_push(&stv, &r1);
                const tree_t * r0 = (nd->n > 0) ? nd->c[0] : NULL; lc_vec_push(&stv, &r0);
            } else if (nd->t == TT_QLIT) { lc_vec_push(&lv, &nd); }
            else { fold = 0; }
        }
        const tree_t ** leaves = (const tree_t **) lv.data; int nl = lv.n;
        IR_t * asn = build(cx, IR_ASSIGN_CONCAT, γ, ω); IR_LIT(asn).sval = (char *) lhs;
        if (fold && nl > 0) {
            int total = 0;
            for (int i = 0; i < nl; i++) if (leaves[i]->v.sval) total += (int)strlen(leaves[i]->v.sval);
            char * buf = (char *) malloc(total + 1); buf[0] = 0;
            for (int i = 0; i < nl; i++) if (leaves[i]->v.sval) strcat(buf, leaves[i]->v.sval);
            IR_t * lit = build(cx, IR_LIT_S, asn, ω); IR_LIT(lit).sval = buf;
            return lit;
        }
        IR_t * seq = build(cx, IR_SEQ, asn, ω); IR_LIT(seq).dval = 1.0;
        (void)(sno_arg_block(cx, (rhs->n > 0) ? rhs->c[0] : NULL));
        IR_LIT(seq).ival     = (int64_t)(intptr_t) sno_arg_block(cx, (rhs->n > 1) ? rhs->c[1] : NULL);
        return seq; }
    default: {
        if (rhs->t == TT_IDX && rhs->n >= 2 && rhs->c[0] && rhs->c[0]->t == TT_VAR && rhs->c[1]
            && (rhs->c[1]->t == TT_ILIT || rhs->c[1]->t == TT_VAR || rhs->c[1]->t == TT_QLIT)) {
            IR_t * asn = build(cx, IR_ASSIGN, γ, ω); IR_LIT(asn).sval = (char *) lhs;
            IR_t * idx = build(cx, IR_IDX, asn, ω);
            IR_t * base_box = build(cx, IR_VAR, NULL, NULL); IR_LIT(base_box).sval = rhs->c[0]->v.sval;
            IR_t * key_box;
            if (rhs->c[1]->t == TT_ILIT) { key_box = build(cx, IR_LIT_I, NULL, NULL); IR_LIT(key_box).ival = rhs->c[1]->v.ival; }
            else if (rhs->c[1]->t == TT_QLIT) { key_box = build(cx, IR_LIT_S, NULL, NULL); IR_LIT(key_box).sval = rhs->c[1]->v.sval ? rhs->c[1]->v.sval : ""; }
            else { key_box = build(cx, IR_VAR, NULL, NULL); IR_LIT(key_box).sval = rhs->c[1]->v.sval; }
            ir_operand_push(idx, base_box); ir_operand_push(idx, key_box); ir_operand_push(asn, idx);
            return idx;
        }
        if (rhs->t == TT_IDX || rhs->t == TT_INDIRECT || rhs->t == TT_VLIST || rhs->t == TT_DEFER) {
            IR_t * asn = IR_node_alloc(cx->g, IR_ASSIGN); IR_LIT(asn).sval = (char *) lhs;
            return NULL;
        }
        if (lc_is_binop(rhs->t) && sno_has_idx(rhs) && !sno_arith_idx_lowerable(rhs)) {
            IR_t * asn = IR_node_alloc(cx->g, IR_ASSIGN); IR_LIT(asn).sval = (char *) lhs;
            IR_t * bop = IR_node_alloc(cx->g, IR_BINOP); IR_LIT(bop).ival = lc_binop_code(rhs->t);
            const tree_t * lop = (rhs->n > 0) ? rhs->c[0] : NULL;
            if (lop && (lop->t == TT_VAR || lop->t == TT_KEYWORD) && !sno_has_idx(lop)) {
                IR_t * v = IR_node_alloc(cx->g, IR_VAR); IR_LIT(v).sval = lop->v.sval; ω_to(v, ω);
            }
            return NULL;
        }
        IR_t * asn = build(cx, IR_ASSIGN, γ, ω); IR_LIT(asn).sval = (char *) lhs;
        IR_t * xbox = build(cx, is_kw ? IR_KEYWORD : IR_VAR, NULL, ω); IR_LIT(xbox).sval = (char *) lhs;
        IR_t * vr = NULL; IR_t * entry = lower_expr(cx, rhs, asn, ω, &vr);
        ir_operand_push(asn, xbox); ir_operand_push(asn, vr);
        return entry; }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_stmt_body(snx_t * cx, const tree_t * s, IR_t * γ_tgt, IR_t * ω_tgt) {
    const tree_t * subj = sno_attr(s, ":subj");
    if (!subj) return NULL;
    if (subj->t == TT_ASSIGN) {
        const tree_t * alhs = subj->c[0]; const tree_t * arhs = (subj->n > 1) ? subj->c[1] : NULL;
        if (!alhs || (alhs->t != TT_VAR && alhs->t != TT_KEYWORD)) return NULL;
        return lower_assign(cx, alhs->v.sval, arhs, γ_tgt, ω_tgt, (alhs->t == TT_KEYWORD) ? 1 : 0);
    }
    int has_eq = sno_has_attr(s, ":eq");
    if (has_eq) {
        const tree_t * repl = sno_attr(s, ":repl");
        if (subj->t == TT_SCAN) {
            const tree_t * sv = (subj->n > 0) ? subj->c[0] : NULL;
            const tree_t * pt = (subj->n > 1) ? subj->c[1] : NULL;
            const char * vname = (sv && sv->v.sval) ? sv->v.sval : "";
            IR_graph_t * pg = lower_pat_graph(pt);
            IR_graph_t * sg = lower_subj_graph(vname);
            IR_graph_t * rg = lower_repl_graph(repl);
            IR_t * scan = build(cx, IR_SCAN, γ_tgt, ω_tgt);
            IR_LIT(scan).sval = (char *) vname; IR_LIT(scan).ival = 1;
            (void)(pg);
            ir_operand_push(scan, (IR_t *)(void *) sg);
            ir_operand_push(scan, (IR_t *)(void *) rg);
            IR_t * lit = build(cx, IR_LIT_S, scan, ω_tgt);
            IR_LIT(lit).sval = (repl && repl->v.sval) ? repl->v.sval : "";
            return lit;
        }
        if (subj->t == TT_INDIRECT && subj->n == 1 && subj->c[0] && subj->c[0]->t == TT_QLIT && subj->c[0]->v.sval)
            return lower_assign(cx, subj->c[0]->v.sval, repl, γ_tgt, ω_tgt, 0);
        if (subj->t == TT_INDIRECT && subj->n == 1 && subj->c[0] && subj->c[0]->t == TT_VAR && subj->c[0]->v.sval && repl && repl->t == TT_QLIT) {
            IR_t * asn = build(cx, IR_INDIRECT_ASSIGN_LIT_S, γ_tgt, ω_tgt); IR_LIT(asn).sval = (char *) subj->c[0]->v.sval;
            IR_t * lit = build(cx, IR_LIT_S, asn, ω_tgt); IR_LIT(lit).sval = repl->v.sval ? repl->v.sval : "";
            return lit;
        }
        if (subj->t == TT_INDIRECT && subj->n == 1 && subj->c[0] && subj->c[0]->t == TT_VAR && subj->c[0]->v.sval && repl && repl->t == TT_VAR && repl->v.sval) {
            IR_t * asn = build(cx, IR_INDIRECT_ASSIGN_VAR, γ_tgt, ω_tgt); IR_LIT(asn).sval = (char *) subj->c[0]->v.sval; IR_LIT(asn).dval = 0.0;
            IR_t * lit = build(cx, IR_LIT_S, asn, ω_tgt); IR_LIT(lit).sval = repl->v.sval;
            return lit;
        }
        if (subj->t == TT_IDX && subj->n >= 2 && subj->c[0] && subj->c[0]->t == TT_VAR && subj->c[1]
            && (subj->c[1]->t == TT_ILIT || subj->c[1]->t == TT_VAR)
            && repl && lc_is_binop(repl->t) && sno_idx_operand_ok(repl->c[0]) && sno_idx_operand_ok(repl->c[1])) {
            IR_t * st = build(cx, IR_IDX_SET, γ_tgt, ω_tgt);
            IR_t * base_box = build(cx, IR_VAR, NULL, NULL); IR_LIT(base_box).sval = subj->c[0]->v.sval;
            IR_t * key_box; if (subj->c[1]->t == TT_ILIT) { key_box = build(cx, IR_LIT_I, NULL, NULL); IR_LIT(key_box).ival = subj->c[1]->v.ival; }
            else { key_box = build(cx, IR_VAR, NULL, NULL); IR_LIT(key_box).sval = subj->c[1]->v.sval; }
            IR_t * vr = NULL; IR_t * ventry = lower_expr(cx, repl, st, ω_tgt, &vr);
            ir_operand_push(st, base_box); ir_operand_push(st, key_box); ir_operand_push(st, vr);
            return ventry;
        }
        if (subj->t == TT_IDX && subj->n >= 2 && subj->c[0] && subj->c[0]->t == TT_VAR && subj->c[1]
            && (subj->c[1]->t == TT_ILIT || subj->c[1]->t == TT_VAR || subj->c[1]->t == TT_QLIT || subj->c[1]->t == TT_FLIT)
            && repl && (repl->t == TT_ILIT || repl->t == TT_VAR || repl->t == TT_QLIT || repl->t == TT_FLIT)) {
            IR_t * st = build(cx, IR_IDX_SET, γ_tgt, ω_tgt);
            IR_t * base_box = build(cx, IR_VAR, NULL, NULL); IR_LIT(base_box).sval = subj->c[0]->v.sval;
            IR_t * key_box;
            if (subj->c[1]->t == TT_ILIT) { key_box = build(cx, IR_LIT_I, NULL, NULL); IR_LIT(key_box).ival = subj->c[1]->v.ival; }
            else if (subj->c[1]->t == TT_QLIT) { key_box = build(cx, IR_LIT_S, NULL, NULL); IR_LIT(key_box).sval = subj->c[1]->v.sval ? subj->c[1]->v.sval : ""; }
            else if (subj->c[1]->t == TT_FLIT) { key_box = build(cx, IR_LIT_F, NULL, NULL); IR_LIT(key_box).dval = subj->c[1]->v.dval; }
            else { key_box = build(cx, IR_VAR, NULL, NULL); IR_LIT(key_box).sval = subj->c[1]->v.sval; }
            IR_t * val_box;
            if (repl->t == TT_ILIT) { val_box = build(cx, IR_LIT_I, NULL, NULL); IR_LIT(val_box).ival = repl->v.ival; }
            else if (repl->t == TT_QLIT) { val_box = build(cx, IR_LIT_S, NULL, NULL); IR_LIT(val_box).sval = repl->v.sval ? repl->v.sval : ""; }
            else if (repl->t == TT_FLIT) { val_box = build(cx, IR_LIT_F, NULL, NULL); IR_LIT(val_box).dval = repl->v.dval; }
            else { val_box = build(cx, IR_VAR, NULL, NULL); IR_LIT(val_box).sval = repl->v.sval; }
            ir_operand_push(st, base_box); ir_operand_push(st, key_box); ir_operand_push(st, val_box);
            return st;
        }
        if (subj->t == TT_FNC && subj->n == 1 && subj->c[0] && subj->c[0]->t == TT_VAR && subj->v.sval
            && repl && (repl->t == TT_ILIT || repl->t == TT_VAR || repl->t == TT_QLIT || repl->t == TT_FLIT)) {
            IR_t * st = build(cx, IR_IDX_SET, γ_tgt, ω_tgt);
            IR_t * base_box = build(cx, IR_VAR, NULL, NULL); IR_LIT(base_box).sval = subj->c[0]->v.sval;
            IR_t * key_box = build(cx, IR_LIT_S, NULL, NULL); IR_LIT(key_box).sval = subj->v.sval;
            IR_t * val_box;
            if (repl->t == TT_ILIT) { val_box = build(cx, IR_LIT_I, NULL, NULL); IR_LIT(val_box).ival = repl->v.ival; }
            else if (repl->t == TT_QLIT) { val_box = build(cx, IR_LIT_S, NULL, NULL); IR_LIT(val_box).sval = repl->v.sval ? repl->v.sval : ""; }
            else if (repl->t == TT_FLIT) { val_box = build(cx, IR_LIT_F, NULL, NULL); IR_LIT(val_box).dval = repl->v.dval; }
            else { val_box = build(cx, IR_VAR, NULL, NULL); IR_LIT(val_box).sval = repl->v.sval; }
            ir_operand_push(st, base_box); ir_operand_push(st, key_box); ir_operand_push(st, val_box);
            return st;
        }
        if (subj->t != TT_VAR && subj->t != TT_KEYWORD) return NULL;
        const char * lhs = subj->v.sval;
        int is_kw = (subj->t == TT_KEYWORD) ? 1 : 0;
        return lower_assign(cx, lhs, repl, γ_tgt, ω_tgt, is_kw);
    }
    switch (subj->t) {
    case TT_FNC: {
        const char * nm = subj->v.sval ? subj->v.sval : "?";
        int complex_arg = 0;
        for (int ai = 0; ai < subj->n && !complex_arg; ai++) {
            const tree_t * a = subj->c[ai];
            if (a && a->t == TT_OPSYN) complex_arg = 1;
            else if (a && a->t == TT_INDIRECT && !sno_indirect_resolvable(a)) complex_arg = 1;
            else if (a && a->t == TT_IDX && !(a->n >= 2 && a->c[0] && a->c[0]->t == TT_VAR && a->c[1] && (a->c[1]->t == TT_ILIT || a->c[1]->t == TT_VAR || a->c[1]->t == TT_QLIT))) complex_arg = 1;
        }
        if (complex_arg) {
            IR_t * nd = IR_node_alloc(cx->g, IR_CALL);
            IR_LIT(nd).sval = nm; IR_LIT(nd).ival = (long long) subj->n;
            return NULL;
        }
        IR_t * nd = build(cx, IR_CALL, γ_tgt, ω_tgt);
        IR_LIT(nd).sval = nm; IR_LIT(nd).ival = (long long) subj->n;
        sno_call_channels(cx, nd, subj);
        return nd; }
    case TT_SCAN: {
        const tree_t * sv = (subj->n > 0) ? subj->c[0] : NULL;
        const tree_t * pt = (subj->n > 1) ? subj->c[1] : NULL;
        const char * vname = (sv && sv->v.sval) ? sv->v.sval : "";
        if (sno_pat_has_fnc(pt)) { IR_node_alloc(cx->g, IR_SCAN); return NULL; }
        if (pt && (pt->t == TT_ASSIGN || pt->t == TT_OPSYN)) { IR_node_alloc(cx->g, IR_SCAN); return NULL; }
        if (sv && sv->t == TT_QLIT) {
            IR_graph_t * pg = lower_pat_graph(pt);
            IR_graph_t * sg = lower_subj_graph_lit(sv->v.sval ? sv->v.sval : "");
            IR_t * scan = build(cx, IR_SCAN, γ_tgt, ω_tgt);
            (void)(pg);
            ir_operand_push(scan, (IR_t *)(void *) sg);
            IR_t * lit = build(cx, IR_LIT_S, scan, ω_tgt);
            IR_LIT(lit).sval = sv->v.sval;
            return lit; }
        IR_graph_t * pg = lower_pat_graph(pt);
        IR_graph_t * sg = lower_subj_graph(vname);
        IR_t * scan = build(cx, IR_SCAN, γ_tgt, ω_tgt);
        IR_LIT(scan).sval = (char *) vname;
        (void)(pg);
        ir_operand_push(scan, (IR_t *)(void *) sg);
        IR_t * var = build(cx, IR_VAR, scan, ω_tgt);
        IR_LIT(var).sval = (char *) vname;
        return var; }
    case TT_SEQ: { IR_node_alloc(cx->g, IR_SEQ); return NULL; }
    case TT_IDX: {
        if (subj->n >= 2 && subj->c[0] && subj->c[0]->t == TT_VAR && subj->c[1] && (subj->c[1]->t == TT_ILIT || subj->c[1]->t == TT_VAR || subj->c[1]->t == TT_QLIT)) {
            IR_t * idx = build(cx, IR_IDX, γ_tgt, ω_tgt);
            IR_t * base_box = build(cx, IR_VAR, NULL, NULL); IR_LIT(base_box).sval = subj->c[0]->v.sval;
            IR_t * key_box;
            if (subj->c[1]->t == TT_ILIT) { key_box = build(cx, IR_LIT_I, NULL, NULL); IR_LIT(key_box).ival = subj->c[1]->v.ival; }
            else if (subj->c[1]->t == TT_QLIT) { key_box = build(cx, IR_LIT_S, NULL, NULL); IR_LIT(key_box).sval = subj->c[1]->v.sval ? subj->c[1]->v.sval : ""; }
            else { key_box = build(cx, IR_VAR, NULL, NULL); IR_LIT(key_box).sval = subj->c[1]->v.sval; }
            ir_operand_push(idx, base_box); ir_operand_push(idx, key_box);
            return idx; }
        return NULL; }
    case TT_IF:    { IR_node_alloc(cx->g, IR_IF);    return NULL; }
    case TT_WHILE: {
        IR_t * w = IR_node_alloc(cx->g, IR_WHILE);
        const tree_t * c0 = (subj->n > 0) ? subj->c[0] : NULL;
        if (c0 && c0->t == TT_SCAN) IR_node_alloc(cx->g, IR_SCAN);
        else if (c0 && c0->t == TT_ASSIGN) {
            const tree_t * al = c0->c[0]; const tree_t * ar = (c0->n > 1) ? c0->c[1] : NULL;
            if (al && (al->t == TT_VAR || al->t == TT_KEYWORD)) lower_assign(cx, al->v.sval, ar, NULL, w, (al->t == TT_KEYWORD) ? 1 : 0);
        }
        else if (c0) lower_expr(cx, c0, NULL, w, NULL);
        return NULL; }
    case TT_DO_WHILE: case TT_FOR: case TT_UNTIL:
    case TT_REPEAT: case TT_CASE: case TT_DEFINE: case TT_PROGRAM:
        return NULL;
    case TT_VAR:
        if (subj->v.sval && !strcmp(subj->v.sval, "RETURN"))  return cx->PRET;
        if (subj->v.sval && !strcmp(subj->v.sval, "FRETURN")) return cx->PFRET;
        if (subj->v.sval && !strcmp(subj->v.sval, "NRETURN")) return cx->PRET;
        return lower_expr(cx, subj, γ_tgt, ω_tgt, NULL);
    default:
        return lower_expr(cx, subj, γ_tgt, ω_tgt, NULL);
    }
}
static IR_t *       g_sno4_lab_nodes[SNO_MAXSTMTS];
static const char * g_sno4_lab_names[SNO_MAXSTMTS];
static int          g_sno4_lab_n = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * lower_snobol4_label(const char * name) {
    if (!name || !name[0]) return NULL;
    for (int i = 0; i < g_sno4_lab_n; i++) if (g_sno4_lab_names[i] && !strcmp(g_sno4_lab_names[i], name)) return g_sno4_lab_nodes[i];
    return NULL; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int lower_snobol4_labels(const char *** names, IR_t *** nodes) { *names = g_sno4_lab_names; *nodes = g_sno4_lab_nodes; return g_sno4_lab_n; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * lower_snobol4(const tree_t * prog) {
    const tree_t * stmts[SNO_MAXSTMTS]; int N = 0;
    for (int i = 0; i < prog->n && N < SNO_MAXSTMTS - 1; i++) {
        const tree_t * s = prog->c[i]; if (!s || sno_is_end(s)) continue; stmts[N++] = s; }
    IR_graph_t * g = IR_alloc(8192, IR_LANG_SNO);
    snx_t cx_s; snx_t * cx = &cx_s; memset(cx, 0, sizeof *cx); cx->g = g;
    cx->PSUCC = IR_node_alloc(g, IR_SUCCEED);
    cx->PFAIL = IR_node_alloc(g, IR_FAIL);
    cx->PRET  = IR_node_alloc(g, IR_RETURN);
    cx->PFRET = IR_node_alloc(g, IR_RETURN);
    IR_LIT(cx->PRET).dval = 1.0; IR_LIT(cx->PFRET).dval = 2.0;
    ω_to(cx->PRET, cx->PFAIL); ω_to(cx->PFRET, cx->PFAIL);
    IR_t * lbuf[SNO_MAXSTMTS];
    cx->labels = lbuf; cx->nlabels = N;
    for (int i = 0; i < N; i++) { lbuf[i] = IR_node_alloc(g, IR_SUCCEED); IR_LIT(lbuf[i]).ival = sno_stmt_stno(stmts[i]); }
    const char * lname_buf[SNO_MAXSTMTS]; cx->lname = lname_buf;
    g_sno4_lab_n = 0;
    for (int i = 0; i < N; i++) {
        const char * nm = sno_stmt_label(stmts[i]);
        if (nm && nm[0]) { cx->lname[cx->nlmap] = nm; cx->lstmt[cx->nlmap] = i; cx->nlmap++;
            if (g_sno4_lab_n < SNO_MAXSTMTS) { g_sno4_lab_names[g_sno4_lab_n] = nm; g_sno4_lab_nodes[g_sno4_lab_n] = lbuf[i]; g_sno4_lab_n++; } } }
    g->entry = (N > 0) ? lbuf[0] : cx->PSUCC;
    fz5_reset();
    for (int i = 0; i < N; i++) {
        const tree_t * s = stmts[i];
        const tree_t * go_s = sno_goto_node(s, TT_GOTO_S, ":goS");
        const tree_t * go_f = sno_goto_node(s, TT_GOTO_F, ":goF");
        const tree_t * go_u = sno_goto_node(s, TT_GOTO_U, ":go");
        IR_t * nxt = next_label(cx, i);
        IR_t * go_tgt_u = go_u ? resolve(cx, sno_goto_label(go_u)) : NULL;
        if (go_u && !go_tgt_u) { const char * gdn = sno_goto_label(go_u); IR_t * gd = build(cx, IR_GOTO_DYN, nxt, nxt); IR_LIT(gd).sval = (gdn && gdn[0]) ? strdup(gdn) : NULL; go_tgt_u = gd; }
        IR_t * go_tgt_s = go_s ? resolve(cx, sno_goto_label(go_s)) : NULL;
        IR_t * go_tgt_f = go_f ? resolve(cx, sno_goto_label(go_f)) : NULL;
        IR_t * γ_tgt, * ω_tgt;
        if (go_u) { γ_tgt = ω_tgt = go_tgt_u ? go_tgt_u : nxt; }
        else { γ_tgt = go_tgt_s ? go_tgt_s : nxt; ω_tgt = go_tgt_f ? go_tgt_f : nxt; }
        int n_before = cx->g->n;
        IR_t * entry = lower_stmt_body(cx, s, γ_tgt, ω_tgt);
        γ_to(lbuf[i], entry ? entry : (cx->g->n == n_before && go_u && go_tgt_u && go_tgt_u->op != IR_RETURN ? go_tgt_u : nxt));
    }
    fz5_finalize(g);
    return g;
}
#include <ctype.h>
#include "stage2.h"
#include "../parser/snobol4/scrip_cc.h"
#include "bb_program.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_parse_define_proto(const char *proto, char fname[64],
                                  char params[STAGE2_FRAME_SLOT_MAX][64], int *np,
                                  char locals[STAGE2_FRAME_SLOT_MAX][64], int *nl) {
    *np = 0; *nl = 0; if (fname) fname[0] = '\0';
    if (!proto || !fname) return 0;
    const char *p = proto;
    while (*p && isspace((unsigned char) *p)) p++;
    int fi = 0;
    while (*p && *p != '(' && !isspace((unsigned char) *p) && fi < 63) fname[fi++] = *p++;
    fname[fi] = '\0';
    if (fi == 0) return 0;
    while (*p && isspace((unsigned char) *p)) p++;
    if (*p == '(') {
        p++;
        while (*p && *p != ')') {
            while (*p && (isspace((unsigned char) *p) || *p == ',')) p++;
            if (!*p || *p == ')') break;
            int ci = 0; char buf[64];
            while (*p && *p != ',' && *p != ')' && !isspace((unsigned char) *p) && ci < 63) buf[ci++] = *p++;
            buf[ci] = '\0';
            if (ci > 0 && *np < STAGE2_FRAME_SLOT_MAX) { memcpy(params[*np], buf, (size_t) ci + 1); (*np)++; }
            while (*p && isspace((unsigned char) *p)) p++;
        }
        if (*p == ')') p++;
    }
    while (*p) {
        while (*p && (isspace((unsigned char) *p) || *p == ',')) p++;
        if (!*p) break;
        int ci = 0; char buf[64];
        while (*p && *p != ',' && !isspace((unsigned char) *p) && ci < 63) buf[ci++] = *p++;
        buf[ci] = '\0';
        if (ci > 0 && *nl < STAGE2_FRAME_SLOT_MAX) { memcpy(locals[*nl], buf, (size_t) ci + 1); (*nl)++; }
    }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int lower_sno_stage2(const tree_t *prog) {
    int sno_n = 0;
    for (int i = 0; i < prog->n; i++) {
        const tree_t *s = prog->c[i];
        if (!s || s->t != TT_STMT) continue;
        if (lp_s_int(s, ":lang") != LANG_SNO) return 0;
        sno_n++;
    }
    if (sno_n == 0) return 0;
    IR_graph_t *g = lower_snobol4(prog);
    if (!g || !g->entry) return 0;
    const char ** lnames = NULL; IR_t ** lnodes = NULL;
    int nlab = lower_snobol4_labels(&lnames, &lnodes);
    bb_label_registry_reset();
    for (int i = 0; i < nlab; i++) bb_label_registry_add(lnames[i], lnodes[i]);
    int bb_idx = bb_program_add(&g_stage2.bbp, g);
    int pi = stage2_proc_grow(&g_stage2);
    g_stage2.proc_table[pi].name     = "main";
    g_stage2.proc_table[pi].proc     = NULL;
    g_stage2.proc_table[pi].entry_pc = -1;
    g_stage2.proc_table[pi].bb_idx   = bb_idx;
    g_stage2.proc_table[pi].nparams  = 0;
    g_stage2.proc_table[pi].proc_entry_node = NULL;
    for (int di = 0; di < prog->n; di++) {
        const tree_t *ds = prog->c[di];
        if (!ds || ds->t != TT_STMT) continue;
        tree_t *dsubj = lp_s_expr(ds, ":subj");
        if (!dsubj || dsubj->t != TT_FNC || !dsubj->v.sval || strcmp(dsubj->v.sval, "DEFINE")) continue;
        if (dsubj->n < 1 || !dsubj->c[0] || dsubj->c[0]->t != TT_QLIT || !dsubj->c[0]->v.sval) continue;
        char fname[64];
        char params[STAGE2_FRAME_SLOT_MAX][64]; int np = 0;
        char locals[STAGE2_FRAME_SLOT_MAX][64]; int nl = 0;
        if (!sno_parse_define_proto(dsubj->c[0]->v.sval, fname, params, &np, locals, &nl)) continue;
        const char *entry_lbl = fname;
        if (dsubj->n >= 2 && dsubj->c[1]) {
            const tree_t *a1 = dsubj->c[1];
            if (a1->t == TT_NAME && a1->n > 0 && a1->c[0] && a1->c[0]->v.sval) entry_lbl = a1->c[0]->v.sval;
            else if (a1->t == TT_VAR  && a1->v.sval) entry_lbl = a1->v.sval;
            else if (a1->t == TT_QLIT && a1->v.sval) entry_lbl = a1->v.sval;
        }
        IR_t *body = lower_snobol4_label(entry_lbl);
        if (!body) continue;
        int fpi  = stage2_proc_grow(&g_stage2);
        g_stage2.proc_table[fpi].name     = strdup(fname);
        g_stage2.proc_table[fpi].proc     = NULL;
        g_stage2.proc_table[fpi].entry_pc = -1;
        g_stage2.proc_table[fpi].bb_idx   = bb_idx;
        g_stage2.proc_table[fpi].nparams  = np + nl;
        g_stage2.proc_table[fpi].proc_entry_node = body;
        Scope *sc = &g_stage2.proc_table[fpi].lower_sc;
        sc->n = 0;
        for (int k = 0; k < np && sc->n < STAGE2_FRAME_SLOT_MAX; k++) { sc->e[sc->n].name = strdup(params[k]); sc->e[sc->n].slot = sc->n; sc->n++; }
        for (int k = 0; k < nl && sc->n < STAGE2_FRAME_SLOT_MAX; k++) { sc->e[sc->n].name = strdup(locals[k]); sc->e[sc->n].slot = sc->n; sc->n++; }
        if (sc->n < STAGE2_FRAME_SLOT_MAX) { sc->e[sc->n].name = strdup(fname); sc->e[sc->n].slot = sc->n; sc->n++; }
    }
    return 1;
}
