#include "lower_pat_dcg.h"
#include "BB.h"
#include "../ast/ast.h"
#include "core.h"
#include <gc/gc.h>
#include <string.h>
static int count_tree(const tree_t * t) {
    if (!t) return 0;
    int n = 1;
    for (int i = 0; i < t->n; i++) n += count_tree(t->c[i]);
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * cset_kw_value(const char * kw) {
    if (!kw) return NULL;
    if (strcmp(kw, "UCASE")  == 0) return "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if (strcmp(kw, "LCASE")  == 0) return "abcdefghijklmnopqrstuvwxyz";
    if (strcmp(kw, "DIGITS") == 0) return "0123456789";
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int cset_fold_len(const tree_t * t) {
    if (!t) return -1;
    if (t->t == TT_QLIT) return (int)strlen(t->v.sval ? t->v.sval : "");
    if (t->t == TT_KEYWORD) { const char * v = cset_kw_value(t->v.sval); return v ? (int)strlen(v) : -1; }
    if (t->t == TT_SEQ || t->t == TT_CAT) { int total = 0; for (int i = 0; i < t->n; i++) { int l = cset_fold_len(t->c[i]); if (l < 0) return -1; total += l; } return total; }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int cset_fold_fill(const tree_t * t, char * buf, int * pos) {
    if (!t) return 0;
    if (t->t == TT_QLIT) { const char * s = t->v.sval ? t->v.sval : ""; int l = (int)strlen(s); memcpy(buf + *pos, s, (size_t)l); *pos += l; return 1; }
    if (t->t == TT_KEYWORD) { const char * v = cset_kw_value(t->v.sval); if (!v) return 0; int l = (int)strlen(v); memcpy(buf + *pos, v, (size_t)l); *pos += l; return 1; }
    if (t->t == TT_SEQ || t->t == TT_CAT) { for (int i = 0; i < t->n; i++) if (!cset_fold_fill(t->c[i], buf, pos)) return 0; return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char * cset_try_fold(const tree_t * t) {
    int n = cset_fold_len(t);
    if (n < 0) return NULL;
    char * buf = (char *)GC_malloc((size_t)n + 1);
    if (!buf) return NULL;
    int pos = 0;
    if (!cset_fold_fill(t, buf, &pos)) return NULL;
    buf[pos] = 0;
    return buf;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int flatten_cat_count(const tree_t * t) {
    if (!t) return 0;
    if (t->t == TT_SEQ || t->t == TT_CAT) {
        int n = 0;
        for (int i = 0; i < t->n; i++) n += flatten_cat_count(t->c[i]);
        return n;
    }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int flatten_cat_fill(const tree_t * t, const tree_t ** out, int idx) {
    if (!t) return idx;
    if (t->t == TT_SEQ || t->t == TT_CAT) {
        for (int i = 0; i < t->n; i++) idx = flatten_cat_fill(t->c[i], out, idx);
        return idx;
    }
    out[idx++] = t;
    return idx;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_graph_t * build_node(IR_graph_t * bbg, const tree_t * t, IR_t * sp, IR_t * fp);
static IR_graph_t * build_node(IR_graph_t * bbg, const tree_t * t, IR_t * sp, IR_t * fp) {
    if (!t) return sp;
    IR_t * bb = NULL;
    switch (t->t) {
    case TT_QLIT: {
        bb = BB_node_alloc(bbg, BB_PAT_LIT);
        bb->sval = t->v.sval ? t->v.sval : "";
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case TT_ARB: {
        bb = BB_node_alloc(bbg, BB_PAT_ARB);
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case TT_REM: {
        bb = BB_node_alloc(bbg, BB_PAT_REM);
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case TT_ABORT: {
        bb = BB_node_alloc(bbg, BB_PAT_ABORT);
        bb->α = bb; bb->β = fp; bb->γ = fp; bb->ω = fp;
        return bb;
    }
    case TT_SPAN: {
        if (t->n < 1 || !t->c[0]) return NULL;
        if (t->c[0]->t != TT_QLIT && t->c[0]->t != TT_VAR) {
            char * cs = cset_try_fold(t->c[0]); if (!cs) return NULL;
            bb = BB_node_alloc(bbg, BB_PAT_SPAN);
            bb->sval = cs; bb->ival = 0;
            bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
            return bb;
        }
        bb = BB_node_alloc(bbg, BB_PAT_SPAN);
        bb->sval = t->c[0]->v.sval ? t->c[0]->v.sval : "";
        bb->ival = (t->c[0]->t == TT_VAR) ? 1 : 0;
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case TT_ANY: {
        if (t->n < 1 || !t->c[0]) return NULL;
        if (t->c[0]->t != TT_QLIT && t->c[0]->t != TT_VAR) {
            char * cs = cset_try_fold(t->c[0]); if (!cs) return NULL;
            bb = BB_node_alloc(bbg, BB_PAT_ANY);
            bb->sval = cs; bb->dval = 0.0;
            bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
            return bb;
        }
        bb = BB_node_alloc(bbg, BB_PAT_ANY);
        bb->sval = t->c[0]->v.sval ? t->c[0]->v.sval : "";
        bb->dval = (t->c[0]->t == TT_VAR) ? 1.0 : 0.0;
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case TT_BREAK: {
        if (t->n < 1 || !t->c[0]) return NULL;
        if (t->c[0]->t != TT_QLIT && t->c[0]->t != TT_VAR) {
            char * cs = cset_try_fold(t->c[0]); if (!cs) return NULL;
            bb = BB_node_alloc(bbg, BB_PAT_BREAK);
            bb->sval = cs; bb->ival = 0; bb->dval = 0.0;
            bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
            return bb;
        }
        bb = BB_node_alloc(bbg, BB_PAT_BREAK);
        bb->sval = t->c[0]->v.sval ? t->c[0]->v.sval : "";
        bb->ival = 0;
        bb->dval = (t->c[0]->t == TT_VAR) ? 1.0 : 0.0;
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case TT_BREAKX: {
        if (t->n < 1 || !t->c[0]) return NULL;
        if (t->c[0]->t != TT_QLIT && t->c[0]->t != TT_VAR) {
            char * cs = cset_try_fold(t->c[0]); if (!cs) return NULL;
            bb = BB_node_alloc(bbg, BB_PAT_BREAK);
            bb->sval = cs; bb->ival = 1; bb->dval = 0.0;
            bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
            return bb;
        }
        bb = BB_node_alloc(bbg, BB_PAT_BREAK);
        bb->sval = t->c[0]->v.sval ? t->c[0]->v.sval : "";
        bb->ival = 1;
        bb->dval = (t->c[0]->t == TT_VAR) ? 1.0 : 0.0;
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case TT_FENCE: {
        IR_t * inner = (t->n > 0 && t->c[0]) ? build_node(bbg, t->c[0], sp, fp) : sp;
        if (t->n > 0 && !inner) return NULL;
        bb = BB_node_alloc(bbg, BB_PAT_FENCE);
        bb->α = bb; bb->β = bb;
        bb->γ = inner ? inner : sp; bb->ω = fp;
        return bb;
    }
    case TT_ARBNO: {
        if (t->n < 1 || !t->c[0]) return NULL;
        int inner_cap = count_tree(t->c[0]) * 8 + 16;
        IR_graph_t * inner_blk = BB_alloc(inner_cap, BB_LANG_SNO);
        if (!inner_blk) return NULL;
        IR_t * inner_entry = build_node(inner_blk, t->c[0], NULL, NULL);
        if (!inner_entry) { BB_free(inner_blk); return NULL; }
        inner_blk->entry = inner_entry;
        bb = BB_node_alloc(bbg, BB_PAT_ARBNO);
        int stack_cap = 64;
        bb_arbno_state_t * az = (bb_arbno_state_t *)GC_MALLOC(sizeof *az);
        az->inner = inner_blk;
        az->pos_stack = (int *)GC_MALLOC((size_t)stack_cap * sizeof(int));
        az->cap = stack_cap;
        az->saved_delta = 0;
        bb->counter = (int64_t)(intptr_t)az;
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case TT_SEQ:
    case TT_CAT: {
        int nflat = flatten_cat_count(t);
        if (nflat == 0) return sp;
        const tree_t ** leaves = (const tree_t **)GC_malloc(nflat * sizeof(const tree_t *));
        flatten_cat_fill(t, leaves, 0);
        if (nflat == 1) return build_node(bbg, leaves[0], sp, fp);
        IR_t * chain = sp;
        IR_t ** entries = (IR_t **)GC_malloc(nflat * sizeof(IR_t *));
        for (int i = nflat - 1; i >= 0; i--) {
            IR_t * e = build_node(bbg, leaves[i], chain, fp);
            if (!e) return NULL;
            entries[i] = e;
            chain = e;
        }
        for (int i = 0; i < nflat - 1; i++) {
            IR_t * a = entries[i], * b = entries[i+1];
            if (a && b && b->ω == fp) b->ω = a->β ? a->β : fp;
        }
        return entries[0];
    }
    case TT_ALT: {
        if (t->n == 0) return fp;
        if (t->n == 1) return build_node(bbg, t->c[0], sp, fp);
        IR_t * alt_fail = fp;
        IR_t * first    = NULL;
        for (int i = t->n - 1; i >= 0; i--) {
            IR_t * e = build_node(bbg, t->c[i], sp, alt_fail);
            if (!e) return NULL;
            first    = e;
            alt_fail = e;
        }
        return first;
    }
    case TT_CAPT_COND_ASGN: {
        if (t->n < 1) return NULL;
        bb = BB_node_alloc(bbg, BB_PAT_ASSIGN_COND);
        bb->sval = (t->n > 1 && t->c[1] && t->c[1]->v.sval) ? t->c[1]->v.sval : NULL;
        bb->γ = sp;
        bb->ω = fp;
        IR_t * inner = build_node(bbg, t->c[0], bb, fp);
        if (!inner) return NULL;
        bb->α = inner;
        bb->β = inner->β;
        return bb;
    }
    case TT_CAPT_IMMED_ASGN: {
        if (t->n < 1) return NULL;
        bb = BB_node_alloc(bbg, BB_PAT_ASSIGN_IMM);
        bb->sval = (t->n > 1 && t->c[1] && t->c[1]->v.sval) ? t->c[1]->v.sval : NULL;
        bb->γ = sp;
        bb->ω = fp;
        IR_t * inner = build_node(bbg, t->c[0], bb, fp);
        if (!inner) return NULL;
        bb->α = inner;
        bb->β = inner->β;
        return bb;
    }
    case TT_CAPT_CURSOR: {
        if (t->n < 1 || !t->c[0] || !t->c[0]->v.sval) return NULL;
        bb = BB_node_alloc(bbg, BB_PAT_ATP);
        bb->sval = t->c[0]->v.sval;
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case TT_LEN: {
        if (t->n < 1 || !t->c[0]) return NULL;
        if (t->c[0]->t != TT_ILIT && t->c[0]->t != TT_VAR) return NULL;
        bb = BB_node_alloc(bbg, BB_PAT_LEN);
        if (t->c[0]->t == TT_VAR) { bb->sval = t->c[0]->v.sval; bb->dval = 1.0; }
        else { bb->ival = t->c[0]->v.ival; bb->dval = 0.0; }
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case TT_NOTANY: {
        if (t->n < 1 || !t->c[0]) return NULL;
        if (t->c[0]->t != TT_QLIT && t->c[0]->t != TT_VAR) {
            char * cs = cset_try_fold(t->c[0]); if (!cs) return NULL;
            bb = BB_node_alloc(bbg, BB_PAT_NOTANY);
            bb->sval = cs; bb->dval = 0.0;
            bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
            return bb;
        }
        bb = BB_node_alloc(bbg, BB_PAT_NOTANY);
        bb->sval = t->c[0]->v.sval ? t->c[0]->v.sval : "";
        bb->dval = (t->c[0]->t == TT_VAR) ? 1.0 : 0.0;
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case TT_POS: {
        if (t->n < 1 || !t->c[0]) return NULL;
        if (t->c[0]->t != TT_ILIT && t->c[0]->t != TT_VAR) return NULL;
        bb = BB_node_alloc(bbg, BB_PAT_POS);
        if (t->c[0]->t == TT_VAR) { bb->sval = t->c[0]->v.sval; bb->dval = 2.0; }
        else { bb->ival = t->c[0]->v.ival; bb->sval = NULL; bb->dval = 0.0; }
        bb->α = bb; bb->β = fp; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case TT_RPOS: {
        if (t->n < 1 || !t->c[0]) return NULL;
        if (t->c[0]->t != TT_ILIT && t->c[0]->t != TT_VAR) return NULL;
        bb = BB_node_alloc(bbg, BB_PAT_POS);
        if (t->c[0]->t == TT_VAR) { bb->sval = t->c[0]->v.sval; bb->dval = 1.0; }
        else { bb->ival = t->c[0]->v.ival; bb->sval = "r"; bb->dval = 0.0; }
        bb->α = bb; bb->β = fp; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case TT_TAB: {
        if (t->n < 1 || !t->c[0]) return NULL;
        if (t->c[0]->t != TT_ILIT && t->c[0]->t != TT_VAR) return NULL;
        bb = BB_node_alloc(bbg, BB_PAT_TAB);
        if (t->c[0]->t == TT_VAR) { bb->sval = t->c[0]->v.sval; bb->dval = 2.0; }
        else { bb->ival = t->c[0]->v.ival; bb->sval = NULL; bb->dval = 0.0; }
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case TT_RTAB: {
        if (t->n < 1 || !t->c[0]) return NULL;
        if (t->c[0]->t != TT_ILIT && t->c[0]->t != TT_VAR) return NULL;
        bb = BB_node_alloc(bbg, BB_PAT_TAB);
        if (t->c[0]->t == TT_VAR) { bb->sval = t->c[0]->v.sval; bb->dval = 1.0; }
        else { bb->ival = t->c[0]->v.ival; bb->sval = "r"; bb->dval = 0.0; }
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case TT_FNC: {
        if (!t->v.sval || t->n < 1 || !t->c[0]) return NULL;
        const char *fn = t->v.sval;
        const tree_t *arg = t->c[0];
        const char *sarg = (arg->t == TT_QLIT && arg->v.sval) ? arg->v.sval : NULL;
        int64_t iarg = (arg->t == TT_ILIT) ? arg->v.ival : 0;
        if (!strcmp(fn, "SPAN") && sarg) {
            bb = BB_node_alloc(bbg, BB_PAT_SPAN);
            bb->sval = sarg; bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp; return bb;
        }
        if (!strcmp(fn, "ANY") && sarg) {
            bb = BB_node_alloc(bbg, BB_PAT_ANY);
            bb->sval = sarg; bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp; return bb;
        }
        if (!strcmp(fn, "BREAK") && sarg) {
            bb = BB_node_alloc(bbg, BB_PAT_BREAK);
            bb->sval = sarg; bb->ival = 0; bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp; return bb;
        }
        if (!strcmp(fn, "BREAKX") && sarg) {
            bb = BB_node_alloc(bbg, BB_PAT_BREAK);
            bb->sval = sarg; bb->ival = 1; bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp; return bb;
        }
        if (!strcmp(fn, "NOTANY") && sarg) {
            bb = BB_node_alloc(bbg, BB_PAT_NOTANY);
            bb->sval = sarg; bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp; return bb;
        }
        if (!strcmp(fn, "LEN")) {
            bb = BB_node_alloc(bbg, BB_PAT_LEN);
            bb->ival = (arg->t == TT_ILIT) ? iarg : 0;
            bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp; return bb;
        }
        if (!strcmp(fn, "POS")) {
            bb = BB_node_alloc(bbg, BB_PAT_POS);
            bb->ival = iarg; bb->sval = NULL;
            bb->α = bb; bb->β = fp; bb->γ = sp; bb->ω = fp; return bb;
        }
        if (!strcmp(fn, "TAB")) {
            bb = BB_node_alloc(bbg, BB_PAT_TAB);
            bb->ival = iarg; bb->sval = NULL;
            bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp; return bb;
        }
        if (!strcmp(fn, "ARBNO") && t->n == 1) {
            IR_graph_t *inner_blk = BB_alloc(count_tree(arg) * 8 + 32, BB_LANG_SNO);
            if (!inner_blk) return NULL;
            IR_t *inner_entry = build_node(inner_blk, arg, NULL, NULL);
            if (!inner_entry) { BB_free(inner_blk); return NULL; }
            inner_blk->entry = inner_entry;
            bb = BB_node_alloc(bbg, BB_PAT_ARBNO);
            bb_arbno_state_t * az = (bb_arbno_state_t *)GC_MALLOC(sizeof *az);
            az->inner = inner_blk;
            az->pos_stack = (int *)GC_MALLOC(64 * sizeof(int));
            az->cap = 64;
            az->saved_delta = 0;
            bb->counter = (int64_t)(intptr_t)az;
            bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp; return bb;
        }
        return NULL;
    }
    case TT_VAR: {
        if (!t->v.sval || !t->v.sval[0]) return NULL;
        bb = BB_node_alloc(bbg, BB_PAT_DEFER);
        bb->sval = t->v.sval;
        bb->ival = 0;
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case TT_DEFER: {
        if (t->n < 1 || !t->c[0] || !t->c[0]->v.sval) return NULL;
        bb = BB_node_alloc(bbg, BB_PAT_DEFER);
        bb->sval = t->c[0]->v.sval;
        bb->ival = 1;
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    default:
        return NULL;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * BB_lower_pat(const tree_t * pat_tree) {
    if (!pat_tree) return NULL;
    int cap = count_tree(pat_tree) * 8 + 32;
    IR_graph_t * bbg = BB_alloc(cap, BB_LANG_SNO);
    if (!bbg) return NULL;
    IR_t * entry = build_node(bbg, pat_tree, NULL, NULL);
    if (!entry) { BB_free(bbg); return NULL; }
    bbg->entry = entry;
    return bbg;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int count_patnd(const PATND_t * p) {
    if (!p) return 0;
    int n = 1;
    for (int i = 0; i < p->nchildren; i++) n += count_patnd(p->children ? p->children[i] : NULL);
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * build_patnd(IR_graph_t * bbg, PATND_t * pp, IR_t * sp, IR_t * fp) {
    if (!pp) return sp;
    IR_t * bb = NULL;
    switch (pp->kind) {
    case XCHR: {
        bb = BB_node_alloc(bbg, BB_PAT_LIT);
        bb->sval = pp->STRVAL_fn ? pp->STRVAL_fn : "";
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case XSPNC: {
        bb = BB_node_alloc(bbg, BB_PAT_SPAN);
        bb->sval = pp->STRVAL_fn ? pp->STRVAL_fn : "";
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case XBRKC: {
        bb = BB_node_alloc(bbg, BB_PAT_BREAK);
        bb->sval = pp->STRVAL_fn ? pp->STRVAL_fn : "";
        bb->ival = 0;
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case XBRKX: {
        bb = BB_node_alloc(bbg, BB_PAT_BREAK);
        bb->sval = pp->STRVAL_fn ? pp->STRVAL_fn : "";
        bb->ival = 1;
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case XANYC: {
        bb = BB_node_alloc(bbg, BB_PAT_ANY);
        bb->sval = pp->STRVAL_fn ? pp->STRVAL_fn : "";
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case XNNYC: {
        bb = BB_node_alloc(bbg, BB_PAT_NOTANY);
        bb->sval = pp->STRVAL_fn ? pp->STRVAL_fn : "";
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case XLNTH: {
        bb = BB_node_alloc(bbg, BB_PAT_LEN);
        bb->ival = pp->num;
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case XPOSI: {
        bb = BB_node_alloc(bbg, BB_PAT_POS);
        bb->ival = pp->num; bb->sval = NULL;
        bb->α = bb; bb->β = fp; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case XRPSI: {
        bb = BB_node_alloc(bbg, BB_PAT_POS);
        bb->ival = pp->num; bb->sval = "r";
        bb->α = bb; bb->β = fp; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case XTB: {
        bb = BB_node_alloc(bbg, BB_PAT_TAB);
        bb->ival = pp->num; bb->sval = NULL;
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case XRTB: {
        bb = BB_node_alloc(bbg, BB_PAT_TAB);
        bb->ival = pp->num; bb->sval = "r";
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case XFARB: {
        bb = BB_node_alloc(bbg, BB_PAT_ARB);
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case XSTAR: {
        bb = BB_node_alloc(bbg, BB_PAT_REM);
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case XEPS: {
        bb = BB_node_alloc(bbg, BB_PAT_LIT);
        bb->sval = "";
        bb->α = bb; bb->β = fp; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case XFAIL: {
        bb = BB_node_alloc(bbg, BB_FAIL);
        bb->α = bb; bb->β = fp; bb->γ = fp; bb->ω = fp;
        return bb;
    }
    case XABRT: {
        bb = BB_node_alloc(bbg, BB_PAT_ABORT);
        bb->α = bb; bb->β = fp; bb->γ = fp; bb->ω = fp;
        return bb;
    }
    case XFNCE: {
        IR_t * inner = sp;
        if (pp->nchildren > 0 && pp->children && pp->children[0]) {
            inner = build_patnd(bbg, pp->children[0], sp, fp);
            if (!inner) return NULL;
        }
        bb = BB_node_alloc(bbg, BB_PAT_FENCE);
        bb->α = bb; bb->β = bb;
        bb->γ = inner ? inner : sp; bb->ω = fp;
        return bb;
    }
    case XCAT: {
        if (pp->nchildren == 0) return sp;
        if (pp->nchildren == 1) return build_patnd(bbg, pp->children[0], sp, fp);
        IR_t * chain = sp;
        IR_t ** entries = (IR_t **)GC_malloc(pp->nchildren * sizeof(IR_t *));
        for (int i = pp->nchildren - 1; i >= 0; i--) {
            IR_t * e = build_patnd(bbg, pp->children[i], chain, fp);
            if (!e) return NULL;
            entries[i] = e;
            chain = e;
        }
        for (int i = 0; i < pp->nchildren - 1; i++) {
            IR_t * a = entries[i], * b = entries[i+1];
            if (a && b && b->ω == fp) b->ω = a->β ? a->β : fp;
        }
        return entries[0];
    }
    case XOR: {
        if (pp->nchildren == 0) return fp;
        if (pp->nchildren == 1) return build_patnd(bbg, pp->children[0], sp, fp);
        IR_t * alt_fail = fp;
        IR_t * first    = NULL;
        for (int i = pp->nchildren - 1; i >= 0; i--) {
            IR_t * e = build_patnd(bbg, pp->children[i], sp, alt_fail);
            if (!e) return NULL;
            first    = e;
            alt_fail = e;
        }
        return first;
    }
    case XARBN: {
        if (pp->nchildren < 1 || !pp->children || !pp->children[0]) return NULL;
        int inner_cap = count_patnd(pp->children[0]) * 8 + 16;
        IR_graph_t * inner_blk = BB_alloc(inner_cap, BB_LANG_SNO);
        if (!inner_blk) return NULL;
        IR_t * inner_entry = build_patnd(inner_blk, pp->children[0], NULL, NULL);
        if (!inner_entry) { BB_free(inner_blk); return NULL; }
        inner_blk->entry = inner_entry;
        bb = BB_node_alloc(bbg, BB_PAT_ARBNO);
        bb_arbno_state_t * az = (bb_arbno_state_t *)GC_MALLOC(sizeof *az);
        az->inner = inner_blk;
        az->pos_stack = (int *)GC_MALLOC(64 * sizeof(int));
        az->cap = 64;
        az->saved_delta = 0;
        bb->counter = (int64_t)(intptr_t)az;
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case XDSAR: {
        bb = BB_node_alloc(bbg, BB_PAT_DEFER);
        bb->sval = pp->STRVAL_fn ? pp->STRVAL_fn : "";
        bb->ival = 0;
        bb->α = bb; bb->β = fp; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    case XFNME: {
        if (pp->nchildren < 1 || !pp->children || !pp->children[0]) return NULL;
        bb = BB_node_alloc(bbg, BB_PAT_ASSIGN_IMM);
        bb->sval = (pp->STRVAL_fn && pp->STRVAL_fn[0]) ? pp->STRVAL_fn :
                   ((pp->var.v == DT_N && pp->var.s) ? pp->var.s : NULL);
        bb->γ = sp;
        bb->ω = fp;
        IR_t * inner = build_patnd(bbg, pp->children[0], bb, fp);
        if (!inner) return NULL;
        bb->α = inner;
        bb->β = inner->β;
        return bb;
    }
    case XNME: {
        if (pp->nchildren < 1 || !pp->children || !pp->children[0]) return NULL;
        bb = BB_node_alloc(bbg, BB_PAT_ASSIGN_COND);
        bb->sval = (pp->STRVAL_fn && pp->STRVAL_fn[0]) ? pp->STRVAL_fn :
                   ((pp->var.v == DT_N && pp->var.s) ? pp->var.s : NULL);
        bb->γ = sp;
        bb->ω = fp;
        IR_t * inner = build_patnd(bbg, pp->children[0], bb, fp);
        if (!inner) return NULL;
        bb->α = inner;
        bb->β = inner->β;
        return bb;
    }
    case XATP: {
        if (!pp->STRVAL_fn || pp->STRVAL_fn[0] != '@') return NULL;
        if (pp->nargs < 1 || !pp->args || !pp->args[0].s || !pp->args[0].s[0]) return NULL;
        bb = BB_node_alloc(bbg, BB_PAT_ATP);
        bb->sval = pp->args[0].s;
        bb->α = bb; bb->β = bb; bb->γ = sp; bb->ω = fp;
        return bb;
    }
    default:
        return NULL;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * patnd_to_bb_graph(PATND_t * pp) {
    if (!pp) return NULL;
    int cap = count_patnd(pp) * 8 + 32;
    IR_graph_t * bbg = BB_alloc(cap, BB_LANG_SNO);
    if (!bbg) return NULL;
    IR_t * entry = build_patnd(bbg, pp, NULL, NULL);
    if (!entry) { BB_free(bbg); return NULL; }
    bbg->entry = entry;
    return bbg;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void tree_set_kids(IR_t * nd, IR_t ** ch, int n) {
    if (!nd || n <= 0) return;
    bb_pat_kids_state_t * zk = (bb_pat_kids_state_t *)GC_MALLOC(sizeof *zk);
    if (!zk) return;
    zk->kids = (IR_t **)GC_MALLOC((size_t)n * sizeof(IR_t *));
    if (!zk->kids) return;
    for (int i = 0; i < n; i++) zk->kids[i] = ch[i];
    zk->nkids = n;
    nd->counter = (int64_t)(intptr_t)zk;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * build_patnd_tree(IR_graph_t * bbg, PATND_t * pp) {
    if (!pp) return NULL;
    switch (pp->kind) {
    case XCAT: {
        if (pp->nchildren == 0) return NULL;
        if (pp->nchildren == 1) return build_patnd_tree(bbg, pp->children[0]);
        IR_t * nd = BB_node_alloc(bbg, BB_PAT_CAT);
        if (!nd) return NULL;
        IR_t ** kids = (IR_t **)GC_MALLOC((size_t)pp->nchildren * sizeof(IR_t *));
        if (!kids) return NULL;
        for (int i = 0; i < pp->nchildren; i++) {
            kids[i] = build_patnd_tree(bbg, pp->children[i]);
            if (!kids[i]) return NULL;
        }
        tree_set_kids(nd, kids, pp->nchildren);
        nd->α = nd; nd->β = nd; nd->γ = NULL; nd->ω = NULL;
        return nd;
    }
    case XOR: {
        if (pp->nchildren == 0) return NULL;
        if (pp->nchildren == 1) return build_patnd_tree(bbg, pp->children[0]);
        IR_t * nd = BB_node_alloc(bbg, BB_PAT_ALT);
        if (!nd) return NULL;
        IR_t ** kids = (IR_t **)GC_MALLOC((size_t)pp->nchildren * sizeof(IR_t *));
        if (!kids) return NULL;
        for (int i = 0; i < pp->nchildren; i++) {
            kids[i] = build_patnd_tree(bbg, pp->children[i]);
            if (!kids[i]) return NULL;
        }
        tree_set_kids(nd, kids, pp->nchildren);
        nd->α = nd; nd->β = nd; nd->γ = NULL; nd->ω = NULL;
        return nd;
    }
    case XFNCE: {
        IR_t * nd = BB_node_alloc(bbg, BB_PAT_FENCE);
        if (!nd) return NULL;
        nd->α = nd; nd->β = nd; nd->γ = NULL; nd->ω = NULL;
        if (pp->nchildren > 0 && pp->children && pp->children[0]) {
            IR_t * inner = build_patnd_tree(bbg, pp->children[0]);
            if (!inner) return NULL;
            IR_t * one[1] = { inner };
            tree_set_kids(nd, one, 1);
        }
        return nd;
    }
    case XFNME: {
        if (pp->nchildren < 1 || !pp->children || !pp->children[0]) return NULL;
        IR_t * inner = build_patnd_tree(bbg, pp->children[0]);
        if (!inner) return NULL;
        IR_t * nd = BB_node_alloc(bbg, BB_PAT_ASSIGN_IMM);
        if (!nd) return NULL;
        nd->sval = (pp->STRVAL_fn && pp->STRVAL_fn[0]) ? pp->STRVAL_fn :
                   ((pp->var.v == DT_N && pp->var.s) ? pp->var.s : NULL);
        nd->α = nd; nd->β = nd; nd->γ = NULL; nd->ω = NULL;
        IR_t * one[1] = { inner };
        tree_set_kids(nd, one, 1);
        return nd;
    }
    case XNME: {
        if (pp->nchildren < 1 || !pp->children || !pp->children[0]) return NULL;
        IR_t * inner = build_patnd_tree(bbg, pp->children[0]);
        if (!inner) return NULL;
        IR_t * nd = BB_node_alloc(bbg, BB_PAT_ASSIGN_COND);
        if (!nd) return NULL;
        nd->sval = (pp->STRVAL_fn && pp->STRVAL_fn[0]) ? pp->STRVAL_fn :
                   ((pp->var.v == DT_N && pp->var.s) ? pp->var.s : NULL);
        nd->α = nd; nd->β = nd; nd->γ = NULL; nd->ω = NULL;
        IR_t * one[1] = { inner };
        tree_set_kids(nd, one, 1);
        return nd;
    }
    case XARBN: {
        if (pp->nchildren < 1 || !pp->children || !pp->children[0]) return NULL;
        int inner_cap = count_patnd(pp->children[0]) * 8 + 16;
        IR_graph_t * inner_blk = BB_alloc(inner_cap, BB_LANG_SNO);
        if (!inner_blk) return NULL;
        IR_t * inner_entry = build_patnd_tree(inner_blk, pp->children[0]);
        if (!inner_entry) { BB_free(inner_blk); return NULL; }
        inner_blk->entry = inner_entry;
        IR_t * nd = BB_node_alloc(bbg, BB_PAT_ARBNO);
        if (!nd) return NULL;
        bb_arbno_state_t * az = (bb_arbno_state_t *)GC_MALLOC(sizeof *az);
        if (!az) return NULL;
        az->kids = (IR_t **)GC_MALLOC(sizeof(IR_t *));
        if (!az->kids) return NULL;
        az->kids[0] = inner_entry;
        az->nkids = 1;
        az->inner = inner_blk;
        az->pos_stack = (int *)GC_MALLOC(64 * sizeof(int));
        az->cap = 64;
        az->saved_delta = 0;
        nd->counter = (int64_t)(intptr_t)az;
        nd->α = nd; nd->β = nd; nd->γ = NULL; nd->ω = NULL;
        return nd;
    }
    default:
        return build_patnd(bbg, pp, NULL, NULL);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * patnd_to_bb_tree(PATND_t * pp) {
    if (!pp) return NULL;
    int cap = count_patnd(pp) * 8 + 32;
    IR_graph_t * bbg = BB_alloc(cap, BB_LANG_SNO);
    if (!bbg) return NULL;
    IR_t * entry = build_patnd_tree(bbg, pp);
    if (!entry) { BB_free(bbg); return NULL; }
    bbg->entry = entry;
    return bbg;
}
