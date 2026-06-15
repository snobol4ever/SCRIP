#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "lower.h"
#include "box_state.h"
/*====================================================================================================================================================================================================*/
#define SNO_MAXSTMTS 2048
typedef struct {
    IR_graph_t * g;
    IR_t * PSUCC;  /* [0] */
    IR_t * PFAIL;  /* [1] */
    IR_t * PRET;   /* [2] :RETURN  */
    IR_t * PFRET;  /* [3] :FRETURN */
    IR_t ** labels;
    int     nlabels;
    const char ** lname;
    int           lstmt[SNO_MAXSTMTS];
    int           nlmap;
} snx_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void γ_to(IR_t * nd, IR_t * t) { lc_γ_to(nd, t); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void ω_to(IR_t * nd, IR_t * t) { lc_ω_to(nd, t); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * build(snx_t * cx, IR_e op, IR_t * γ, IR_t * ω) { return lc_build(cx->g, op, γ, ω); }
/*====================================================================================================================================================================================================*/
/* ── AST helpers ──────────────────────────────────────────────────── */
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
static const tree_t * sno_attr(const tree_t * s, const char * tag) {
    for (int i = 0; i < s->n; i++) {
        const tree_t * a = s->c[i];
        if (a && a->t == TT_ATTR && a->v.sval && !strcmp(a->v.sval, tag))
            return (a->n > 0) ? a->c[0] : NULL;
    }
    return NULL;
}
static int sno_has_attr(const tree_t * s, const char * tag) {
    for (int i = 0; i < s->n; i++) {
        const tree_t * a = s->c[i];
        if (a && a->t == TT_ATTR && a->v.sval && !strcmp(a->v.sval, tag)) return 1;
    }
    return 0;
}
static const tree_t * sno_goto_node(const tree_t * s, tree_e kind, const char * tag) {
    for (int i = 0; i < s->n; i++) {
        const tree_t * a = s->c[i]; if (!a) continue;
        if (a->t == kind) return a;
        if (a->t == TT_ATTR && a->v.sval && !strcmp(a->v.sval, tag)) return a;
    }
    return NULL;
}
static const char * sno_goto_label(const tree_t * ch) {
    if (!ch || !ch->n || !ch->c[0]) return NULL; return ch->c[0]->v.sval; }
static const char * sno_stmt_label(const tree_t * s) {
    const tree_t * lbl = sno_attr(s, ":lbl"); return lbl ? lbl->v.sval : NULL; }
static int sno_pat_builtin(const char * nm) {
    if (!nm) return 0;
    return !strcmp(nm,"REM") || !strcmp(nm,"rem") || !strcmp(nm,"ARB") || !strcmp(nm,"arb") || !strcmp(nm,"FENCE") || !strcmp(nm,"fence")
        || !strcmp(nm,"ABORT") || !strcmp(nm,"abort") || !strcmp(nm,"BAL") || !strcmp(nm,"bal") || !strcmp(nm,"FAIL") || !strcmp(nm,"fail"); }
/*====================================================================================================================================================================================================*/
/* resolve label name → IR_t * (NULL if unknown; caller uses nxt as fallback) */
static IR_t * resolve(snx_t * cx, const char * name) {
    if (!name || !name[0]) return NULL;
    if (!strcmp(name, "END"))     return cx->PSUCC;
    if (!strcmp(name, "RETURN"))  return cx->PRET;
    if (!strcmp(name, "NRETURN")) return cx->PRET;
    if (!strcmp(name, "FRETURN")) return cx->PFRET;
    for (int i = 0; i < cx->nlmap; i++)
        if (cx->lname[i] && !strcmp(cx->lname[i], name)) return cx->labels[cx->lstmt[i]];
    return NULL;  /* unknown → caller uses nxt */
}
static IR_t * next_label(snx_t * cx, int i) {
    return (i + 1 < cx->nlabels) ? cx->labels[i + 1] : cx->PSUCC; }
/*====================================================================================================================================================================================================*/
/* ── binop helpers ─────────────────────────────────────────────────── */
static int is_sno_unop(tree_e tt) {
    switch (tt) {
    case TT_MNS: case TT_PLS: case TT_NONNULL: case TT_NOT: case TT_INTERROGATE:
    case TT_NAME: case TT_INDIRECT: return 1;
    default: return 0; }
}
/*====================================================================================================================================================================================================*/
/* ── expression lowerer ─────────────────────────────────────────────── */
static IR_t * lower_expr(snx_t * cx, const tree_t * t, IR_t * cont, IR_t * nxt, IR_t ** res);
static IR_graph_t * sno_arg_block(void * vcx, const tree_t * a);
static IR_t * sno_seq_node(snx_t * cx, const tree_t * t, IR_t * cont, IR_t * nxt) {
    IR_t * seq = build(cx, IR_SEQ, cont, nxt); IR_LIT(seq).dval = 1.0;
    IR_EXEC(seq).counter = (int64_t)(intptr_t) sno_arg_block(cx, (t->n > 0) ? t->c[0] : NULL);
    IR_LIT(seq).ival     = (int64_t)(intptr_t) sno_arg_block(cx, (t->n > 1) ? t->c[1] : NULL);
    return seq;
}
static IR_t * sno_arg_lower(void * vcx, const tree_t * a, IR_t * F) {
    snx_t * cx = (snx_t *) vcx; IR_t * r = NULL;
    return (a && a->t == TT_SEQ) ? sno_seq_node(cx, a, NULL, F) : lower_expr(cx, a, NULL, F, &r);
}
static IR_graph_t * sno_arg_block(void * vcx, const tree_t * a) { return lc_arg_block(&((snx_t *) vcx)->g, IR_LANG_SNO, sno_arg_lower, vcx, a); }
static void sno_call_channels(snx_t * cx, IR_t * call, const tree_t * t) {
    int is_def = (t->v.sval && !strcmp(t->v.sval, "DEFINE"));
    lc_call_argblks(call, is_def ? 5.0 : 2.0, t->n, sno_arg_block, cx, (const tree_t * const *) t->c);
    if (is_def) call->op = IR_CALL_DEFINE;
}
static IR_t * lower_expr(snx_t * cx, const tree_t * t, IR_t * cont, IR_t * nxt, IR_t ** res) {
    IR_t * dummy = NULL; if (!res) res = &dummy;
    if (!t) { IR_t * s = build(cx, IR_SUCCEED, cont, nxt); *res = s; return s; }
    if (lc_is_binop(t->t)) {
        IR_t * op = build(cx, IR_BINOP, cont, nxt); IR_LIT(op).ival = lc_binop_code(t->t);
        IR_t * lr = NULL, * rr = NULL;
        IR_t * ea = lower_expr(cx, t->c[0], NULL, nxt, &lr);
        IR_t * eb = lower_expr(cx, t->c[1], op, nxt, &rr);
        γ_to(lr, eb); { IR_t * ax[2]; ax[0] = lr; ax[1] = rr; bb_operand_aux_set(cx->g, op, ax, 2); } ir_operand_push(op, lr); ir_operand_push(op, rr); *res = op; return ea;
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
    default: { IR_t * s = build(cx, IR_SUCCEED, cont, nxt); *res = s; return s; }
    }
}
/*====================================================================================================================================================================================================*/
/* ── pattern sub-graph builder ──────────────────────────────────────── */
static int is_pat_consumer(IR_e op) {
    switch (op) {
    case IR_PAT_LEN: case IR_PAT_TAB: case IR_PAT_RTAB: case IR_PAT_REM: case IR_PAT_BREAK:
    case IR_PAT_BREAKX: case IR_PAT_SPAN: case IR_PAT_ANY: case IR_PAT_NOTANY: case IR_PAT_LIT: case IR_PAT_ARB: case IR_PAT_ARBNO: return 1;
    default: return 0; }
}
static IR_t * lower_pat_node(IR_graph_t * pg, const tree_t * t, IR_t * succ, IR_t * fail);
/* lower_pat_node_tail: lower t and return the tail node (first-allocated = rightmost leaf) via tail_out */
/* allocation order is right-to-left, so the first node allocated by lower_pat_node(lc, ...) is the tail */
static IR_t * lower_pat_node_tail(IR_graph_t * pg, const tree_t * t, IR_t * succ, IR_t * fail, IR_t ** tail_out) {
    int before = pg->n;
    IR_t * entry = lower_pat_node(pg, t, succ, fail);
    if (tail_out) *tail_out = (before < pg->n) ? pg->all[before] : entry;
    return entry;
}
static IR_t * lower_pat_node(IR_graph_t * pg, const tree_t * t, IR_t * succ, IR_t * fail) {
    if (!t) return succ;
    switch (t->t) {
    case TT_QLIT: {
        IR_t * nd = IR_node_alloc(pg, IR_PAT_LIT); γ_to(nd, succ); ω_to(nd, fail);
        IR_LIT(nd).sval = t->v.sval; return nd; }
    case TT_VAR: {
        /* built-in SNOBOL4 pattern names → specific opcodes */
        const char * nm = t->v.sval;
        if (nm) {
            if (!strcmp(nm,"REM")  || !strcmp(nm,"rem"))   { IR_t * nd = IR_node_alloc(pg, IR_PAT_REM);   γ_to(nd, succ); ω_to(nd, fail); return nd; }
            if (!strcmp(nm,"ARB")  || !strcmp(nm,"arb"))   { IR_t * nd = IR_node_alloc(pg, IR_PAT_ARB);   γ_to(nd, succ); ω_to(nd, fail); return nd; }
            if (!strcmp(nm,"FENCE")|| !strcmp(nm,"fence"))  { IR_t * nd = IR_node_alloc(pg, IR_PAT_FENCE); γ_to(nd, succ); ω_to(nd, fail); IR_LIT(nd).ival = 1; return nd; }
            if (!strcmp(nm,"ABORT")|| !strcmp(nm,"abort"))  { IR_t * nd = IR_node_alloc(pg, IR_PAT_ABORT); γ_to(nd, succ); ω_to(nd, fail); return nd; }
            if (!strcmp(nm,"BAL")  || !strcmp(nm,"bal"))    { IR_t * nd = IR_node_alloc(pg, IR_PAT_BAL);   γ_to(nd, succ); ω_to(nd, fail); return nd; }
            if (!strcmp(nm,"FAIL") || !strcmp(nm,"fail"))   { IR_t * nd = IR_node_alloc(pg, IR_FAIL);      γ_to(nd, succ); ω_to(nd, fail); return nd; }
        }
        IR_t * nd = IR_node_alloc(pg, IR_PAT_DEFER); γ_to(nd, succ); ω_to(nd, fail);
        IR_LIT(nd).sval = t->v.sval; return nd; }
    case TT_ARB: {
        IR_t * nd = IR_node_alloc(pg, IR_PAT_ARB); γ_to(nd, succ); ω_to(nd, fail); return nd; }
    case TT_REM: {
        IR_t * nd = IR_node_alloc(pg, IR_PAT_REM); γ_to(nd, succ); ω_to(nd, fail); return nd; }
    case TT_BREAK: case TT_BREAKX: {
        IR_e op = (t->t == TT_BREAK) ? IR_PAT_BREAK : IR_PAT_BREAKX;
        IR_t * nd = IR_node_alloc(pg, op); γ_to(nd, succ); ω_to(nd, fail);
        if (t->n > 0 && t->c[0]) {
            const tree_t * arg = t->c[0];
            if (arg->t == TT_VAR || arg->t == TT_KEYWORD) { IR_LIT(nd).sval = arg->v.sval; IR_LIT(nd).dval = 1.0; }
            else IR_LIT(nd).sval = arg->v.sval; }
        return nd; }
    case TT_SPAN: {
        IR_t * nd = IR_node_alloc(pg, IR_PAT_SPAN); γ_to(nd, succ); ω_to(nd, fail);
        if (t->n > 0 && t->c[0]) {
            const tree_t * arg = t->c[0];
            if (arg->t == TT_VAR || arg->t == TT_KEYWORD) { IR_LIT(nd).sval = arg->v.sval; IR_LIT(nd).ival = 1; }
            else IR_LIT(nd).sval = arg->v.sval; }
        return nd; }
    case TT_ANY: {
        IR_t * nd = IR_node_alloc(pg, IR_PAT_ANY); γ_to(nd, succ); ω_to(nd, fail);
        if (t->n > 0 && t->c[0]) {
            const tree_t * arg = t->c[0];
            if (arg->t == TT_VAR || arg->t == TT_KEYWORD) { IR_LIT(nd).sval = arg->v.sval; IR_LIT(nd).dval = 1.0; }
            else IR_LIT(nd).sval = arg->v.sval; }
        return nd; }
    case TT_NOTANY: {
        IR_t * nd = IR_node_alloc(pg, IR_PAT_NOTANY); γ_to(nd, succ); ω_to(nd, fail);
        if (t->n > 0 && t->c[0]) {
            const tree_t * arg = t->c[0];
            if (arg->t == TT_VAR || arg->t == TT_KEYWORD) { IR_LIT(nd).sval = arg->v.sval; IR_LIT(nd).dval = 1.0; }
            else IR_LIT(nd).sval = arg->v.sval; }
        return nd; }
    case TT_POS: {
        IR_t * nd = IR_node_alloc(pg, IR_PAT_POS); γ_to(nd, succ); ω_to(nd, fail);
        if (t->n > 0 && t->c[0]) {
            const tree_t * arg = t->c[0];
            if (arg->t == TT_VAR || arg->t == TT_KEYWORD) { IR_LIT(nd).sval = arg->v.sval; IR_LIT(nd).dval = 2.0; }
            else IR_LIT(nd).ival = arg->v.ival; }
        return nd; }
    case TT_RPOS: {
        IR_t * nd = IR_node_alloc(pg, IR_PAT_POS); γ_to(nd, succ); ω_to(nd, fail);
        IR_LIT(nd).sval = "r";
        if (t->n > 0 && t->c[0]) {
            const tree_t * arg = t->c[0];
            if (arg->t == TT_VAR || arg->t == TT_KEYWORD) { IR_LIT(nd).sval = arg->v.sval; IR_LIT(nd).dval = 1.0; }  /* overwrite "r" */
            else IR_LIT(nd).ival = arg->v.ival; }
        return nd; }
    case TT_LEN: {
        IR_t * nd = IR_node_alloc(pg, IR_PAT_LEN); γ_to(nd, succ); ω_to(nd, fail);
        if (t->n > 0 && t->c[0]) {
            const tree_t * arg = t->c[0];
            if (arg->t == TT_VAR || arg->t == TT_KEYWORD) { IR_LIT(nd).sval = arg->v.sval; IR_LIT(nd).dval = 1.0; }
            else IR_LIT(nd).ival = arg->v.ival; }
        return nd; }
    case TT_TAB: {
        IR_t * nd = IR_node_alloc(pg, IR_PAT_TAB); γ_to(nd, succ); ω_to(nd, fail);
        if (t->n > 0 && t->c[0]) {
            const tree_t * arg = t->c[0];
            if (arg->t == TT_VAR || arg->t == TT_KEYWORD) { IR_LIT(nd).sval = arg->v.sval; IR_LIT(nd).dval = 2.0; }
            else IR_LIT(nd).ival = arg->v.ival; }
        return nd; }
    case TT_RTAB: {
        IR_t * nd = IR_node_alloc(pg, IR_PAT_RTAB); γ_to(nd, succ); ω_to(nd, fail);
        IR_LIT(nd).sval = "r";
        if (t->n > 0 && t->c[0]) {
            const tree_t * arg = t->c[0];
            if (arg->t == TT_VAR || arg->t == TT_KEYWORD) { IR_LIT(nd).sval = arg->v.sval; IR_LIT(nd).dval = 1.0; }
            else IR_LIT(nd).ival = arg->v.ival; }
        return nd; }
    case TT_ARBNO: {
        IR_t * nd = IR_node_alloc(pg, IR_PAT_ARBNO); γ_to(nd, succ); ω_to(nd, fail);
        bb_arbno_state_t * az = (bb_arbno_state_t *) calloc(1, sizeof *az);
        if (az) {
            extern IR_graph_t * sno_pat_graph_fwd(const tree_t *);
            az->inner = sno_pat_graph_fwd((t->n > 0) ? t->c[0] : NULL);
            az->cap = 64; az->pos_stack = (int *) calloc(64, sizeof(int));
            IR_EXEC(nd).counter = (int64_t)(intptr_t) az;
        }
        return nd; }
    case TT_ALT: {
        /* collect alternatives left-recursively into flat list */
        lc_vec av; lc_vec_init(&av, (int) sizeof(const tree_t *));
        const tree_t * cur = t;
        while (cur && cur->t == TT_ALT) {
            const tree_t * lc2 = (cur->n > 0) ? cur->c[0] : NULL;
            const tree_t * rc2 = (cur->n > 1) ? cur->c[1] : NULL;
            /* push right child onto alts stack; recurse left */
            lc_vec_push(&av, &rc2); cur = lc2;
        }
        if (cur) lc_vec_push(&av, &cur); /* leftmost leaf */
        const tree_t ** alts = (const tree_t **) av.data; int na = av.n;
        /* reverse so alts[0]=leftmost, alts[na-1]=rightmost */
        for (int li = 0, ri = na-1; li < ri; li++, ri--) {
            const tree_t * tmp = alts[li]; alts[li] = alts[ri]; alts[ri] = tmp; }
        /* build right-to-left: allocate final PAT_ALT first, then each alt */
        /* oracle shape: PAT_ALT(final) γ=succ, then alts right→left each γ=PAT_ALT, ω=next_right */
        if (na < 2) return lower_pat_node(pg, alts[0], succ, fail);
        /* for n alts: need (n-1) PAT_ALT nodes; rightmost = allocated first */
        IR_t * cont = succ; /* continuation for successful match */
        /* allocate the chain of PAT_ALT nodes right-to-left */
        /* final PAT_ALT (for last pair): γ=succ, ω=fail */
        IR_t * final_alt = IR_node_alloc(pg, IR_PAT_ALT); γ_to(final_alt, succ); ω_to(final_alt, fail);
        /* build from rightmost alt backwards; each non-last alt gets its own PAT_ALT continuation */
        /* alts[na-1] is last (rightmost): γ→final_alt, ω→fail */
        IR_t * last_entry = lower_pat_node(pg, alts[na-1], final_alt, fail);
        /* alts[na-2] down to alts[1]: each gets a new PAT_ALT node as continuation */
        IR_t * prev_entry = last_entry;
        IR_t * prev_alt = final_alt;
        for (int i = na-2; i >= 1; i--) {
            IR_t * alt_nd = IR_node_alloc(pg, IR_PAT_ALT); γ_to(alt_nd, final_alt); ω_to(alt_nd, prev_entry);
            IR_t * e = lower_pat_node(pg, alts[i], alt_nd, prev_entry);
            prev_entry = e; prev_alt = alt_nd; }
        (void)prev_alt;
        /* alts[0] is leftmost: γ→its PAT_ALT, ω→alts[1] entry */
        IR_t * first_entry = lower_pat_node(pg, alts[0], prev_alt, prev_entry);
        return first_entry; }
    case TT_CAPT_COND_ASGN: {  /* pat . var */
        IR_t * nd = IR_node_alloc(pg, IR_PAT_ASSIGN_COND); γ_to(nd, succ); ω_to(nd, fail);
        const char * vn = (t->n > 1 && t->c[1]) ? t->c[1]->v.sval : "";
        IR_LIT(nd).sval = (char *) vn;
        if (t->n > 0 && t->c[0]) {
            IR_t * pe = lower_pat_node(pg, t->c[0], nd, fail);
            ir_operand_push(nd, pe);
        }
        return nd; }
    case TT_CAPT_IMMED_ASGN: { /* pat $ var */
        IR_t * nd = IR_node_alloc(pg, IR_PAT_ASSIGN_IMM); γ_to(nd, succ); ω_to(nd, fail);
        const char * vn = (t->n > 1 && t->c[1]) ? t->c[1]->v.sval : "";
        IR_LIT(nd).sval = (char *) vn;
        if (t->n > 0 && t->c[0]) {
            IR_t * pe = lower_pat_node(pg, t->c[0], nd, fail);
            ir_operand_push(nd, pe);
        }
        return nd; }
    case TT_SEQ: {  /* pattern concatenation: left success → right entry */
        const tree_t * rc = (t->n > 1) ? t->c[1] : NULL;
        const tree_t * lc = (t->n > 0) ? t->c[0] : NULL;
        int rc_is_capture = rc && (rc->t == TT_CAPT_COND_ASGN || rc->t == TT_CAPT_IMMED_ASGN);
        int lc_is_capture = lc && (lc->t == TT_CAPT_COND_ASGN || lc->t == TT_CAPT_IMMED_ASGN);
        /* check if rc is the FAIL builtin (TT_VAR "FAIL") */
        int rc_is_fail = rc && rc->t == TT_VAR && rc->v.sval && (!strcmp(rc->v.sval,"FAIL")||!strcmp(rc->v.sval,"fail"));
        /* when RIGHT child is a capture or FAIL: PAT_CAT is allocated FIRST, then right child */
        if (rc_is_capture || rc_is_fail) {
            /* only allocate PAT_CAT if succ is the graph's SUCCEED exit (idx=0) */
            /* any other succ means there's already a non-trivial continuation — no PAT_CAT needed */
            int need_cat = succ && succ->op == IR_SUCCEED && succ == pg->all[0];
            IR_t * cat = need_cat ? IR_node_alloc(pg, IR_PAT_CAT) : succ;
            if (need_cat) { γ_to(cat, succ); ω_to(cat, fail); }
            IR_t * re = lower_pat_node(pg, rc, cat, fail);
            /* lower lc; capture/FAIL.ω → tail of lc (the immediately preceding element or its inner operand) */
            IR_t * le_tail = NULL;
            IR_t * le = lower_pat_node_tail(pg, lc, re ? re : cat, fail, &le_tail);
            /* backtrack-ω: rc node.ω → le_tail (if consumer) or le_tail's inner operand (if capture) */
            if (re && le_tail) {
                if (is_pat_consumer(le_tail->op)) ω_to(re, le_tail);
                else if ((le_tail->op == IR_PAT_ASSIGN_COND || le_tail->op == IR_PAT_ASSIGN_IMM) && le_tail->n_operands > 0)
                    ω_to(re, le_tail->operands[0]);
            }
            return le;
        }
        /* when LEFT child is a capture: PAT_CAT allocated first, THEN lower rc with succ=PAT_CAT */
        if (lc_is_capture) {
            /* only a fresh PAT_CAT when succ is the graph SUCCEED exit; an inner concat */
            /* (succ already a real continuation, e.g. 060 outer REM-capture) gets none */
            int need_cat = succ && succ->op == IR_SUCCEED && succ == pg->all[0];
            IR_t * cat = need_cat ? IR_node_alloc(pg, IR_PAT_CAT) : succ;
            if (need_cat) { γ_to(cat, succ); ω_to(cat, fail); }
            /* rc is lowered with succ=PAT_CAT (PAT_LEN.γ→PAT_CAT in 049) */
            IR_t * re = lower_pat_node(pg, rc, cat, fail);
            /* lc (capture) lowered with succ=re (capture.γ→PAT_LEN in 049) */
            IR_t * le = lower_pat_node(pg, lc, re ? re : cat, fail);
            /* backtrack-ω: rc.ω → the resumable predecessor of lc (lc's inner operand if capture, else lc) */
            /* for 049: PAT_LEN.ω → PAT_ARB (lc's inner child) */
            if (le && re) {
                IR_t * btgt = (le->n_operands > 0) ? le->operands[0] : NULL;
                if (btgt) ω_to(re, btgt);
            }
            return le;
        }
        /* plain concat: check if lc contains a capture (needs PAT_CAT allocated before rc) */
        /* if so, lower lc first for allocation order, then rc, fixup lc-tail.γ → rc.entry */
        int lc_has_capture = 0;
        if (lc) { const tree_t * q = lc;
            while (q && q->t == TT_SEQ) q = (q->n > 1) ? q->c[1] : NULL;
            if (q && (q->t == TT_CAPT_COND_ASGN || q->t == TT_CAPT_IMMED_ASGN || q->t == TT_DEFER)) lc_has_capture = 1;
            if (q && q->t == TT_VAR && q->v.sval && !sno_pat_builtin(q->v.sval)) lc_has_capture = 1; }
        if (succ && succ->op == IR_SUCCEED && succ == pg->all[0]) lc_has_capture = 1;
        if (lc_has_capture) {
            /* oracle allocation order: PAT_CAT first, then rc, then lc */
            /* pre-allocate PAT_CAT so it gets a lower index than both rc and lc nodes */
            IR_t * cat = IR_node_alloc(pg, IR_PAT_CAT); γ_to(cat, succ); ω_to(cat, fail);
            /* lower rc with succ=PAT_CAT (e.g. RPOS.γ→PAT_CAT) */
            IR_t * re_tail = NULL;
            IR_t * re = lower_pat_node_tail(pg, rc, cat, fail, &re_tail);
            /* lower lc with succ=re (lc's rightmost leaf chains into rc entry) */
            IR_t * le_tail = NULL;
            IR_t * le = lower_pat_node_tail(pg, lc, re ? re : cat, fail, &le_tail);
            /* backtrack-ω: re tail (e.g. RPOS) ω → lc's innermost resumable element */
            if (re_tail && le_tail) {
                IR_t * btgt = NULL;
                if ((le_tail->op == IR_PAT_ASSIGN_COND || le_tail->op == IR_PAT_ASSIGN_IMM) && le_tail->n_operands > 0)
                    btgt = le_tail->operands[0];
                else if (le_tail->op == IR_PAT_DEFER)
                    btgt = le_tail;
                else if (is_pat_consumer(le_tail->op))
                    btgt = le_tail;
                if (btgt) ω_to(re_tail, btgt);
            }
            return le;
        }
        /* standard right-first plain concat */
        IR_t * le_tail = NULL;
        IR_t * re_tail = NULL;
        IR_t * re = lower_pat_node_tail(pg, rc, succ, fail, &re_tail);
        IR_t * le = lower_pat_node_tail(pg, lc, re ? re : succ, fail, &le_tail);
        /* general backtrack-ω: re.tail.ω → le.tail if le.tail is consumer */
        if (re_tail && le_tail && is_pat_consumer(le_tail->op)) ω_to(re_tail, le_tail);
        return le; }
    case TT_FENCE: {
        if (t->n > 0 && t->c[0]) {
            IR_t * seal = IR_node_alloc(pg, IR_PAT_FENCE); γ_to(seal, succ); ω_to(seal, fail);
            return lower_pat_node(pg, t->c[0], seal, fail); }
        IR_t * nd = IR_node_alloc(pg, IR_PAT_FENCE); γ_to(nd, succ); ω_to(nd, fail); return nd; }
    case TT_DEFER: {  /* *VAR deferred pattern reference */
        IR_t * nd = IR_node_alloc(pg, IR_PAT_DEFER); γ_to(nd, succ); ω_to(nd, fail);
        const char * nm = (t->n > 0 && t->c[0]) ? t->c[0]->v.sval : "?";
        IR_LIT(nd).sval = (char *) nm; IR_LIT(nd).ival = 1; return nd; }
    case TT_FNC: {  /* functional pattern node: use PAT_DEFER as fallback */
        IR_t * nd = IR_node_alloc(pg, IR_PAT_DEFER); γ_to(nd, succ); ω_to(nd, fail);
        IR_LIT(nd).sval = t->v.sval; return nd; }
    default: {
        IR_t * nd = IR_node_alloc(pg, IR_PAT_DEFER); γ_to(nd, succ); ω_to(nd, fail);
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
static IR_graph_t * lower_pat_graph(const tree_t * pat) {
    IR_graph_t * pg = IR_alloc(256, IR_LANG_SNO);
    IR_t * succ = IR_node_alloc(pg, IR_SUCCEED);  /* [0] */
    IR_t * fail = IR_node_alloc(pg, IR_FAIL);      /* [1] */
    IR_t * entry = lower_pat_node(pg, pat, succ, fail);
    pg->entry = entry ? entry : succ;
    return pg;
}
IR_graph_t * sno_pat_graph_fwd(const tree_t * pat) { return lower_pat_graph(pat); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_graph_t * lower_subj_graph(const char * vname) {
    IR_graph_t * sg = IR_alloc(32, IR_LANG_SNO);
    IR_t * fail = IR_node_alloc(sg, IR_FAIL);   /* [0] */
    IR_t * var  = IR_node_alloc(sg, IR_VAR);    /* [1] */
    ω_to(var, fail); IR_LIT(var).sval = (char *) vname;
    sg->entry = var;
    return sg;
}
static IR_graph_t * lower_subj_graph_lit(const char * text) {
    IR_graph_t * sg = IR_alloc(32, IR_LANG_SNO);
    IR_t * fail = IR_node_alloc(sg, IR_FAIL);   /* [0] */
    IR_t * lit  = IR_node_alloc(sg, IR_LIT_S);  /* [1] */
    ω_to(lit, fail); IR_LIT(lit).sval = (char *) text;
    sg->entry = lit;
    return sg;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_graph_t * lower_repl_graph(const tree_t * repl) {
    IR_graph_t * rg = IR_alloc(32, IR_LANG_SNO);
    IR_t * fail = IR_node_alloc(rg, IR_FAIL);   /* [0] */
    IR_t * lit  = IR_node_alloc(rg, IR_LIT_S);  /* [1] */
    ω_to(lit, fail); IR_LIT(lit).sval = (repl && repl->v.sval) ? repl->v.sval : "";
    rg->entry = lit;
    return rg;
}
/*====================================================================================================================================================================================================*/
/* ── pattern-expression detector ────────────────────────────────── */
static int sno_is_pat_elem(tree_e tt) {
    switch (tt) {
    case TT_POS: case TT_RPOS: case TT_ARB: case TT_ARBNO: case TT_REM:
    case TT_ANY: case TT_NOTANY: case TT_BREAK: case TT_SPAN: case TT_BREAKX:
    case TT_LEN: case TT_TAB: case TT_RTAB: case TT_BAL: case TT_FENCE: case TT_ABORT:
    case TT_CAPT_COND_ASGN: case TT_CAPT_IMMED_ASGN: case TT_CAPT_CURSOR: case TT_ALT:
        return 1;
    default: return 0; }
}
static int sno_has_pat(const tree_t * t) {
    if (!t) return 0;
    if (sno_is_pat_elem(t->t)) return 1;
    for (int i = 0; i < t->n; i++) if (sno_has_pat(t->c[i])) return 1;
    return 0;
}
static int sno_has_dc(const tree_t * t) {
    if (!t) return 0;
    if (t->t == TT_DEFER || t->t == TT_CAPT_COND_ASGN || t->t == TT_CAPT_IMMED_ASGN || t->t == TT_CAPT_CURSOR) return 1;
    for (int i = 0; i < t->n; i++) if (sno_has_dc(t->c[i])) return 1;
    return 0;
}
static int sno_has_idx(const tree_t * t) {
    if (!t) return 0;
    if (t->t == TT_IDX || t->t == TT_INDIRECT) return 1;
    for (int i = 0; i < t->n; i++) if (sno_has_idx(t->c[i])) return 1;
    return 0;
}
static int sno_leaf_buildable(const tree_t * t) {
    const char * s; int hit;
    if (!t) return 0;
    if (t->t == TT_QLIT) return 1;
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
static int sno_seq_buildable(const tree_t * t) {
    if (!t) return 0;
    if (t->t == TT_SEQ) return (t->n >= 1 && sno_seq_buildable(t->c[0])) && (t->n >= 2 && sno_leaf_buildable(t->c[1]));
    return sno_leaf_buildable(t);
}
static int sno_seq_has_pat_leaf(const tree_t * t) {
    if (!t) return 0;
    if (t->t == TT_SEQ) return sno_seq_has_pat_leaf(t->c[0]) || (t->n > 1 && t->c[1] && t->c[1]->t != TT_QLIT);
    return (t->t != TT_QLIT);
}
static IR_t * sno_build_leaf_ir(snx_t * cx, const tree_t * t, IR_t * g, IR_t * w) {
    if (!t) return NULL;
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
/*====================================================================================================================================================================================================*/
/* ── concat: wire each part as an operand box feeding a binary cat (seq = cat(seq, part)), exactly like lower_expr's binop ── */
static void sno_seq_flatten_ops(const tree_t * t, lc_vec * out, int * nonleaf) {
    if (!t) return;
    if (t->t == TT_SEQ) { sno_seq_flatten_ops((t->n > 0) ? t->c[0] : NULL, out, nonleaf); sno_seq_flatten_ops((t->n > 1) ? t->c[1] : NULL, out, nonleaf); return; }
    lc_vec_push(out, &t);
    if (t->t == TT_FNC || lc_is_binop(t->t) || is_sno_unop(t->t)) *nonleaf = 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * sno_concat_chain(snx_t * cx, const tree_t ** ops, int n, IR_t * cont, IR_t * nxt, IR_t ** res) {
    if (n == 1) return lower_expr(cx, ops[0], cont, nxt, res);
    IR_t * op = build(cx, IR_BINOP, cont, nxt); IR_LIT(op).ival = (long long) lc_binop_code(TT_CAT);
    IR_t * lr = NULL, * rr = NULL;
    IR_t * ea = sno_concat_chain(cx, ops, n - 1, NULL, nxt, &lr);
    IR_t * eb = lower_expr(cx, ops[n - 1], op, nxt, &rr);
    γ_to(lr, eb);
    { IR_t * ax[2]; ax[0] = lr; ax[1] = rr; bb_operand_aux_set(cx->g, op, ax, 2); ir_operand_push(op, lr); ir_operand_push(op, rr); }
    *res = op; return ea;
}
/*====================================================================================================================================================================================================*/
/* ── assignment lowerer ──────────────────────────────────────────────── */
static IR_t * lower_assign(snx_t * cx, const char * lhs, const tree_t * rhs, IR_t * γ, IR_t * ω, int is_kw) {
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
            /* allocate left-associative: lit[0], lit[1], alt(0,1), lit[2], alt(prev,2), ... */
            lits[0] = build(cx, IR_PATTERN_LIT, ω, ω); IR_LIT(lits[0]).sval = qleaves[0]->v.sval;
            lits[1] = build(cx, IR_PATTERN_LIT, ω, ω); IR_LIT(lits[1]).sval = qleaves[1]->v.sval;
            IR_t * cur_alt = build(cx, IR_PATTERN_ALT, dtp, ω);
            ir_operand_push(cur_alt, lits[0]); ir_operand_push(cur_alt, lits[1]);
            γ_to(lits[0], lits[1]); γ_to(lits[1], cur_alt);
            for (int i = 2; i < nq; i++) {
                lits[i] = build(cx, IR_PATTERN_LIT, ω, ω); IR_LIT(lits[i]).sval = qleaves[i]->v.sval;
                /* intermediate cur_alt.γ → this literal (not DTP_ASSIGN) */
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
        /* general pattern-ALT value-assign: both arms clean (no defer/capture) TT_SEQ → LIVE ASSIGN+ALT+SEQ(arm2)+SEQ(arm1); else ORPHAN ASSIGN+ALT (+SEQ if arm2 is TT_SEQ) */
        const tree_t * a0 = rhs->c[0]; const tree_t * a1 = rhs->c[1];
        int dirty0 = sno_has_dc(a0), dirty1 = sno_has_dc(a1);
        if (a0 && a1 && a0->t == TT_SEQ && a1->t == TT_SEQ && !dirty0 && !dirty1) {
            IR_t * asn = build(cx, IR_ASSIGN, γ, ω); IR_LIT(asn).sval = (char *) lhs;
            IR_t * alt = build(cx, IR_ALT, asn, ω);
            IR_t * s1  = build(cx, IR_SEQ, alt, ω);
            IR_EXEC(s1).counter = (int64_t)(intptr_t) sno_arg_block(cx, (a1->n > 0) ? a1->c[0] : NULL);
            IR_LIT(s1).ival     = (int64_t)(intptr_t) sno_arg_block(cx, (a1->n > 1) ? a1->c[1] : NULL);
            IR_t * s0  = build(cx, IR_SEQ, alt, s1);
            IR_EXEC(s0).counter = (int64_t)(intptr_t) sno_arg_block(cx, (a0->n > 0) ? a0->c[0] : NULL);
            IR_LIT(s0).ival     = (int64_t)(intptr_t) sno_arg_block(cx, (a0->n > 1) ? a0->c[1] : NULL);
            return s0;
        }
        IR_t * asn = IR_node_alloc(cx->g, IR_ASSIGN); IR_LIT(asn).sval = (char *) lhs;
        IR_node_alloc(cx->g, IR_ALT);
        if (a1 && a1->t == TT_SEQ) IR_node_alloc(cx->g, IR_SEQ);
        return NULL;
    }
    if (rhs && rhs->n == 1 && rhs->c[0] && rhs->c[0]->t == TT_QLIT) {
        /* single pattern primitive with QLIT arg → LIVE DTP_ASSIGN + PATTERN_* (entry = pattern node) */
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
        /* single cset primitive with VARIABLE arg → LIVE DTP_ASSIGN + PATTERN_* (dval=1.0 var-flag; cset evaluated-and-baked at construction time per SPITBOL stored-pattern binding) */
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
        /* single pattern primitive with ILIT arg → LIVE DTP_ASSIGN + PATTERN_* with ival */
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
        /* capture as value-assign RHS → ORPHAN plain ASSIGN only (oracle bails before any pattern node) */
        IR_t * asn = IR_node_alloc(cx->g, IR_ASSIGN); IR_LIT(asn).sval = (char *) lhs;
        return NULL;
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
    /* pattern expression in RHS → ORPHAN ASSIGN_CONCAT + SEQ (oracle behaviour) */
    if (rhs && sno_has_pat(rhs)) {
        IR_t * asn = IR_node_alloc(cx->g, IR_ASSIGN_CONCAT); IR_LIT(asn).sval = (char *) lhs;
        IR_node_alloc(cx->g, IR_SEQ);
        return NULL;
    }
    /* predefined nullary patterns as bare var names (TT_VAR "REM"/"FAIL"/"SUCCEED"/"FENCE"/"ABORT"/"ARB") */
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
        /* V = func(...) → ASSIGN_CALL + CALL */
        const char * nm = rhs->v.sval ? rhs->v.sval : "?";
        IR_t * asn = build(cx, IR_ASSIGN_CALL, γ, ω); IR_LIT(asn).sval = (char *) lhs;
        IR_t * call = build(cx, IR_CALL, asn, ω); IR_LIT(call).sval = nm; IR_LIT(call).ival = (long long) rhs->n;
        sno_call_channels(cx, call, rhs);
        return call; }
    case TT_SEQ: {
        /* concat containing array ref / indirection → ORPHAN ASSIGN_CONCAT + bare SEQ (oracle bails; label chains nxt) */
        if (sno_has_idx(rhs)) {
            IR_t * asn = IR_node_alloc(cx->g, IR_ASSIGN_CONCAT); IR_LIT(asn).sval = (char *) lhs;
            IR_node_alloc(cx->g, IR_SEQ);
            return NULL;
        }
        /* non-leaf part (CALL/ARITH) present → wire parts as operand boxes feeding binary cat; all-leaf stays on op_parts fast path */
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
        IR_EXEC(seq).counter = (int64_t)(intptr_t) sno_arg_block(cx, (rhs->n > 0) ? rhs->c[0] : NULL);
        IR_LIT(seq).ival     = (int64_t)(intptr_t) sno_arg_block(cx, (rhs->n > 1) ? rhs->c[1] : NULL);
        return seq; }
    default: {
        /* TT_IDX/TT_INDIRECT/TT_VLIST/TT_DEFER as RHS: oracle emits orphan ASSIGN (no γ/ω), label chains to nxt */
        if (rhs->t == TT_IDX || rhs->t == TT_INDIRECT || rhs->t == TT_VLIST || rhs->t == TT_DEFER) {
            IR_t * asn = IR_node_alloc(cx->g, IR_ASSIGN); IR_LIT(asn).sval = (char *) lhs;
            return NULL;
        }
        /* binop RHS containing array ref → PARTIAL ORPHAN: edgeless ASSIGN + edgeless BINOP(ival) + left VAR (ω only); oracle bails mid-lower, label chains nxt */
        if (lc_is_binop(rhs->t) && sno_has_idx(rhs)) {
            IR_t * asn = IR_node_alloc(cx->g, IR_ASSIGN); IR_LIT(asn).sval = (char *) lhs;
            IR_t * bop = IR_node_alloc(cx->g, IR_BINOP); IR_LIT(bop).ival = lc_binop_code(rhs->t);
            const tree_t * lop = (rhs->n > 0) ? rhs->c[0] : NULL;
            if (lop && (lop->t == TT_VAR || lop->t == TT_KEYWORD) && !sno_has_idx(lop)) {
                IR_t * v = IR_node_alloc(cx->g, IR_VAR); IR_LIT(v).sval = lop->v.sval; ω_to(v, ω);
            }
            return NULL;
        }
        IR_t * asn = build(cx, IR_ASSIGN, γ, ω); IR_LIT(asn).sval = (char *) lhs;
        return lower_expr(cx, rhs, asn, ω, NULL); }
    }
}
/*====================================================================================================================================================================================================*/
/* ── statement body lowerer ──────────────────────────────────────────── */
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
        /* pattern replacement:  SUBJ PAT = REPL  → SCAN node ival=1 + 3 sub-graphs */
        if (subj->t == TT_SCAN) {
            const tree_t * sv = (subj->n > 0) ? subj->c[0] : NULL;
            const tree_t * pt = (subj->n > 1) ? subj->c[1] : NULL;
            const char * vname = (sv && sv->v.sval) ? sv->v.sval : "";
            IR_graph_t * pg = lower_pat_graph(pt);
            IR_graph_t * sg = lower_subj_graph(vname);
            IR_graph_t * rg = lower_repl_graph(repl);
            IR_t * scan = build(cx, IR_SCAN, γ_tgt, ω_tgt);
            IR_LIT(scan).sval = (char *) vname; IR_LIT(scan).ival = 1;
            IR_EXEC(scan).counter = (int64_t)(intptr_t) pg;
            ir_operand_push(scan, (IR_t *)(void *) sg);
            ir_operand_push(scan, (IR_t *)(void *) rg);
            IR_t * lit = build(cx, IR_LIT_S, scan, ω_tgt);
            IR_LIT(lit).sval = (repl && repl->v.sval) ? repl->v.sval : "";
            return lit;
        }
        /* indirect assignment, compile-time-constant name:  $'lit' = RHS  ≡  lit = RHS  (SPITBOL Ch.7 indirect reference) */
        if (subj->t == TT_INDIRECT && subj->n == 1 && subj->c[0] && subj->c[0]->t == TT_QLIT && subj->c[0]->v.sval)
            return lower_assign(cx, subj->c[0]->v.sval, repl, γ_tgt, ω_tgt, 0);
        /* indirect assignment, runtime name held in a variable:  $V = 'lit'  → resolve V's value to a name then assign the string (SPITBOL Ch.7 indirect reference) */
        if (subj->t == TT_INDIRECT && subj->n == 1 && subj->c[0] && subj->c[0]->t == TT_VAR && subj->c[0]->v.sval && repl && repl->t == TT_QLIT) {
            IR_t * asn = build(cx, IR_INDIRECT_ASSIGN_LIT_S, γ_tgt, ω_tgt); IR_LIT(asn).sval = (char *) subj->c[0]->v.sval;
            IR_t * lit = build(cx, IR_LIT_S, asn, ω_tgt); IR_LIT(lit).sval = repl->v.sval ? repl->v.sval : "";
            return lit;
        }
        /* assignment:  LHS = RHS  (plain VAR or KEYWORD LHS only) */
        if (subj->t != TT_VAR && subj->t != TT_KEYWORD) return NULL;
        const char * lhs = subj->v.sval;
        int is_kw = (subj->t == TT_KEYWORD) ? 1 : 0;
        return lower_assign(cx, lhs, repl, γ_tgt, ω_tgt, is_kw);
    }
    switch (subj->t) {
    case TT_FNC: {
        const char * nm = subj->v.sval ? subj->v.sval : "?";
        /* orphan CALL when any arg is TT_IDX or TT_INDIRECT (oracle behaviour) */
        int complex_arg = 0;
        for (int ai = 0; ai < subj->n && !complex_arg; ai++) {
            const tree_t * a = subj->c[ai];
            if (a && (a->t == TT_IDX || a->t == TT_INDIRECT || a->t == TT_OPSYN)) complex_arg = 1;
        }
        if (complex_arg) {
            IR_t * nd = IR_node_alloc(cx->g, IR_CALL);
            IR_LIT(nd).sval = nm; IR_LIT(nd).ival = (long long) subj->n;
            return NULL;  /* orphan: no γ/ω; label chains to nxt */
        }
        IR_t * nd = build(cx, IR_CALL, γ_tgt, ω_tgt);
        IR_LIT(nd).sval = nm; IR_LIT(nd).ival = (long long) subj->n;
        sno_call_channels(cx, nd, subj);
        return nd; }
    case TT_SCAN: {
        /* TT_SCAN children: c[0]=subject expr, c[1]=pattern expr */
        const tree_t * sv = (subj->n > 0) ? subj->c[0] : NULL;
        const tree_t * pt = (subj->n > 1) ? subj->c[1] : NULL;
        const char * vname = (sv && sv->v.sval) ? sv->v.sval : "";
        /* user function call in pattern position: oracle punts the whole statement to an ORPHAN SCAN (no edges, no VAR) and label-chains to the lexically-next stmt, ignoring :S and :F */
        if (sno_pat_has_fnc(pt)) { IR_node_alloc(cx->g, IR_SCAN); return NULL; }
        /* TT_ASSIGN/TT_OPSYN-shaped pattern (value-assign or @ cursor op inside pattern): oracle punts via ORPHAN SCAN, label chains nxt */
        if (pt && (pt->t == TT_ASSIGN || pt->t == TT_OPSYN)) { IR_node_alloc(cx->g, IR_SCAN); return NULL; }
        /* literal subject: SCAN carries no sval, entry is LIT_S, subject block FAIL+LIT_S (oracle shape) */
        if (sv && sv->t == TT_QLIT) {
            IR_graph_t * pg = lower_pat_graph(pt);
            IR_graph_t * sg = lower_subj_graph_lit(sv->v.sval ? sv->v.sval : "");
            IR_t * scan = build(cx, IR_SCAN, γ_tgt, ω_tgt);
            IR_EXEC(scan).counter = (int64_t)(intptr_t) pg;
            ir_operand_push(scan, (IR_t *)(void *) sg);
            IR_t * lit = build(cx, IR_LIT_S, scan, ω_tgt);
            IR_LIT(lit).sval = sv->v.sval;
            return lit; }
        /* allocate sub-graphs */
        IR_graph_t * pg = lower_pat_graph(pt);
        IR_graph_t * sg = lower_subj_graph(vname);
        /* emit SCAN node + VAR entry in main graph */
        IR_t * scan = build(cx, IR_SCAN, γ_tgt, ω_tgt);
        IR_LIT(scan).sval = (char *) vname;
        IR_EXEC(scan).counter = (int64_t)(intptr_t) pg;
        ir_operand_push(scan, (IR_t *)(void *) sg);
        /* VAR entry: γ→scan, ω→failure_target */
        IR_t * var = build(cx, IR_VAR, scan, ω_tgt);
        IR_LIT(var).sval = (char *) vname;
        return var; }
    case TT_SEQ: { IR_node_alloc(cx->g, IR_SEQ); return NULL; }
    case TT_IDX: return NULL;
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
        /* bare RETURN/FRETURN/NRETURN statement: spine jumps straight to the landing node, no node emitted (oracle shape) */
        if (subj->v.sval && !strcmp(subj->v.sval, "RETURN"))  return cx->PRET;
        if (subj->v.sval && !strcmp(subj->v.sval, "FRETURN")) return cx->PFRET;
        if (subj->v.sval && !strcmp(subj->v.sval, "NRETURN")) return cx->PRET;
        return lower_expr(cx, subj, γ_tgt, ω_tgt, NULL);
    default:
        return lower_expr(cx, subj, γ_tgt, ω_tgt, NULL);
    }
}
/*====================================================================================================================================================================================================*/
static IR_t *       g_sno4_lab_nodes[SNO_MAXSTMTS];
static const char * g_sno4_lab_names[SNO_MAXSTMTS];
static int          g_sno4_lab_n = 0;
IR_t * lower_snobol4_label(const char * name) {
    if (!name || !name[0]) return NULL;
    for (int i = 0; i < g_sno4_lab_n; i++) if (g_sno4_lab_names[i] && !strcmp(g_sno4_lab_names[i], name)) return g_sno4_lab_nodes[i];
    return NULL; }
int lower_snobol4_labels(const char *** names, IR_t *** nodes) { *names = g_sno4_lab_names; *nodes = g_sno4_lab_nodes; return g_sno4_lab_n; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * lower_snobol4(const tree_t * prog) {
    /* 1. collect non-end stmts */
    const tree_t * stmts[SNO_MAXSTMTS]; int N = 0;
    for (int i = 0; i < prog->n && N < SNO_MAXSTMTS - 1; i++) {
        const tree_t * s = prog->c[i]; if (!s || sno_is_end(s)) continue; stmts[N++] = s; }
    /* 2. allocate graph + fixed prefix */
    IR_graph_t * g = IR_alloc(8192, IR_LANG_SNO);
    snx_t cx_s; snx_t * cx = &cx_s; memset(cx, 0, sizeof *cx); cx->g = g;
    cx->PSUCC = IR_node_alloc(g, IR_SUCCEED);
    cx->PFAIL = IR_node_alloc(g, IR_FAIL);
    cx->PRET  = IR_node_alloc(g, IR_RETURN);
    cx->PFRET = IR_node_alloc(g, IR_RETURN);
    IR_LIT(cx->PRET).dval = 1.0; IR_LIT(cx->PFRET).dval = 2.0;
    ω_to(cx->PRET, cx->PFAIL); ω_to(cx->PFRET, cx->PFAIL);
    /* 3. label SUCCEED nodes [4..4+N-1] */
    IR_t * lbuf[SNO_MAXSTMTS];
    cx->labels = lbuf; cx->nlabels = N;
    for (int i = 0; i < N; i++) lbuf[i] = IR_node_alloc(g, IR_SUCCEED);
    /* 4. label name → stmt-index map */
    const char * lname_buf[SNO_MAXSTMTS]; cx->lname = lname_buf;
    g_sno4_lab_n = 0;
    for (int i = 0; i < N; i++) {
        const char * nm = sno_stmt_label(stmts[i]);
        if (nm && nm[0]) { cx->lname[cx->nlmap] = nm; cx->lstmt[cx->nlmap] = i; cx->nlmap++;
            if (g_sno4_lab_n < SNO_MAXSTMTS) { g_sno4_lab_names[g_sno4_lab_n] = nm; g_sno4_lab_nodes[g_sno4_lab_n] = lbuf[i]; g_sno4_lab_n++; } } }
    /* 5. entry */
    g->entry = (N > 0) ? lbuf[0] : cx->PSUCC;
    /* 6. lower each stmt */
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
    return g;
}

/*====================================================================================================================*/
/* stage2 entry — relocated from lower_program.c (lower_common rung)                                                  */
/*====================================================================================================================*/
#include <ctype.h>
#include "stage2.h"
#include "../parser/snobol4/scrip_cc.h"
#include "bb_program.h"
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
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
    g_stage2.proc_table[pi].proc_entry_idx = -1;
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
        IR_t *body = lower_snobol4_label(fname);
        if (!body) continue;
        int fpi  = stage2_proc_grow(&g_stage2);
        g_stage2.proc_table[fpi].name     = strdup(fname);
        g_stage2.proc_table[fpi].proc     = NULL;
        g_stage2.proc_table[fpi].entry_pc = -1;
        g_stage2.proc_table[fpi].bb_idx   = bb_idx;
        g_stage2.proc_table[fpi].nparams  = np;
        g_stage2.proc_table[fpi].proc_entry_idx = body->idx;
        Scope *sc = &g_stage2.proc_table[fpi].lower_sc;
        sc->n = 0;
        for (int k = 0; k < np && sc->n < STAGE2_FRAME_SLOT_MAX; k++) { sc->e[sc->n].name = strdup(params[k]); sc->e[sc->n].slot = sc->n; sc->n++; }
        for (int k = 0; k < nl && sc->n < STAGE2_FRAME_SLOT_MAX; k++) { sc->e[sc->n].name = strdup(locals[k]); sc->e[sc->n].slot = sc->n; sc->n++; }
        if (sc->n < STAGE2_FRAME_SLOT_MAX) { sc->e[sc->n].name = strdup(fname); sc->e[sc->n].slot = sc->n; sc->n++; }
    }
    return 1;
}
