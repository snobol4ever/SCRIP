#define BB_DEFINE_NAMES
#include "rt/rt_arena.h"
#include "lower.h"
#include "emit.h"
#include "bb_program.h"
#include "../runtime/core/coerce.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <ctype.h>
extern int junction_is(DESCR_t v);
extern int junction_collapse(DESCR_t scalar, DESCR_t jct, int op, int numeric);
typedef struct { const char * name; IR_t * landing; } bb_label_entry_t;
static lc_vec g_bb_labels = { NULL, 0, 0, (int) sizeof(bb_label_entry_t) };
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_label_registry_reset(void) { g_bb_labels.n = 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_label_registry_add(const char * name, IR_t * landing) {
    if (!name || !landing) return;
    bb_label_entry_t e; e.name = name; e.landing = landing;
    lc_vec_push(&g_bb_labels, &e);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * bb_label_landing(const char * name) {
    if (!name) return NULL;
    for (int i = 0; i < g_bb_labels.n; i++) { bb_label_entry_t * e = &LC_AT(&g_bb_labels, bb_label_entry_t, i); if (e->name && !strcmp(e->name, name)) return e->landing; }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int bb_label_registry_count(void) { return g_bb_labels.n; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * bb_label_registry_get(int i, IR_t ** landing) {
    if (i < 0 || i >= g_bb_labels.n) { if (landing) *landing = NULL; return NULL; }
    bb_label_entry_t * e = &LC_AT(&g_bb_labels, bb_label_entry_t, i);
    if (landing) *landing = e->landing;
    return e->name;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int lp_s_int(const tree_t *s, const char *tag) { const char *v = stmt_attr_str(stmt_attr_find(s, tag)); return v ? atoi(v) : 0; }
tree_t *lp_s_expr(const tree_t *s, const char *tag) { return stmt_attr_expr(stmt_attr_find(s, tag)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *lp_strdup(const char *s) {
    if (!s) return NULL;
    { extern char *rt_ws_strdup(const char *); return rt_ws_strdup(s); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * norm_charseq(DESCR_t d) {
    if (d.v == DT_SNUL) return "";
    if (!IS_STR_fn(d)) return NULL;
    const char * s = d.s ? d.s : "";
    if (!strchr(s, '\x01')) return s;
    size_t cap = strlen(s) + 1; char * out = (char *) rt_ws_alloc(cap); size_t oi = 0;
    const char * seg = s;
    for (;;) {
        const char * nx = strchr(seg, '\x01');
        size_t slen = nx ? (size_t)(nx - seg) : strlen(seg);
        int allnum = (slen > 0); for (size_t k = 0; k < slen; k++) if (!isdigit((unsigned char) seg[k])) { allnum = 0; break; }
        if (allnum) { long v = strtol(seg, NULL, 10); if (v != 0 && oi + 1 < cap) out[oi++] = (char)(v & 0xFF); }
        else { for (size_t k = 0; k < slen && oi + 1 < cap; k++) out[oi++] = seg[k]; }
        if (!nx) break; seg = nx + 1;
    }
    out[oi] = '\0'; return out;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t binop_apply(BinopKind op, DESCR_t lv, DESCR_t rv, int *rel_fail) {
    *rel_fail = 0;
    if (IS_FAIL_fn(lv) || IS_FAIL_fn(rv)) return FAILDESCR;
    {
        int lj = junction_is(lv), rj = junction_is(rv);
        int num_rel = (op == BINOP_EQ || op == BINOP_NE || op == BINOP_LT ||
                       op == BINOP_LE || op == BINOP_GT || op == BINOP_GE);
        int str_rel = (op == BINOP_SEQ || op == BINOP_SNE || op == BINOP_SLT ||
                       op == BINOP_SLE || op == BINOP_SGT || op == BINOP_SGE);
        if ((lj || rj) && (num_rel || str_rel)) {
            DESCR_t jct    = lj ? lv : rv;
            DESCR_t scalar = lj ? rv : lv;
            int tt_op = (op == BINOP_EQ || op == BINOP_SEQ) ? TT_EQ : (op == BINOP_NE || op == BINOP_SNE) ? TT_NE :
                        (op == BINOP_LT || op == BINOP_SLT) ? TT_LT : (op == BINOP_LE || op == BINOP_SLE) ? TT_LE :
                        (op == BINOP_GT || op == BINOP_SGT) ? TT_GT : TT_GE;
            int numeric = str_rel ? 0 : (IS_INT_fn(scalar) || IS_REAL_fn(scalar));
            int truth = junction_collapse(scalar, jct, tt_op, numeric);
            *rel_fail = !truth;
            return truth ? rv : FAILDESCR;
        }
    }
    if ((op == BINOP_EQ || op == BINOP_NE || op == BINOP_LT || op == BINOP_LE || op == BINOP_GT || op == BINOP_GE) && IS_STR_fn(lv) && IS_STR_fn(rv) && !IS_CSET_fn(lv) && !IS_CSET_fn(rv)) {
        const char *ls = (lv.v == DT_S && lv.s) ? lv.s : "";
        const char *rs = (rv.v == DT_S && rv.s) ? rv.s : "";
        size_t ll = (lv.v == DT_S && lv.slen > 0) ? (size_t)lv.slen : strlen(ls);
        size_t rl = (rv.v == DT_S && rv.slen > 0) ? (size_t)rv.slen : strlen(rs);
        if (strchr(ls, '\x01')) { const char * n = norm_charseq(lv); if (n) { ls = n; ll = strlen(ls); } }
        if (strchr(rs, '\x01')) { const char * n = norm_charseq(rv); if (n) { rs = n; rl = strlen(rs); } }
        size_t nn = ll < rl ? ll : rl;
        int cmp = nn ? memcmp(ls, rs, nn) : 0;
        if (!cmp) cmp = (ll > rl) - (ll < rl);
        int ok;
        switch (op) {
        case BINOP_LT: ok = (cmp <  0); break;
        case BINOP_LE: ok = (cmp <= 0); break;
        case BINOP_GT: ok = (cmp >  0); break;
        case BINOP_GE: ok = (cmp >= 0); break;
        case BINOP_EQ: ok = (cmp == 0); break;
        default:       ok = (cmp != 0); break;
        }
        *rel_fail = !ok;
        return ok ? rv : FAILDESCR;
    }
    int either_real = (IS_REAL_fn(lv) || IS_REAL_fn(rv));
    double ld = IS_REAL_fn(lv) ? lv.r : (double)(IS_INT_fn(lv) ? lv.i : 0);
    double rd = IS_REAL_fn(rv) ? rv.r : (double)(IS_INT_fn(rv) ? rv.i : 0);
    long   li = IS_INT_fn(lv) ? lv.i : (long)lv.r;
    long   ri = IS_INT_fn(rv) ? rv.i : (long)rv.r;
    DESCR_t real_result;
    switch (op) {
        case BINOP_ADD: if (either_real) { real_result.v=DT_R; real_result.r=ld+rd; return real_result; } return INTVAL(li + ri);
        case BINOP_SUB: if (either_real) { real_result.v=DT_R; real_result.r=ld-rd; return real_result; } return INTVAL(li - ri);
        case BINOP_MUL: if (either_real) { real_result.v=DT_R; real_result.r=ld*rd; return real_result; } return INTVAL(li * ri);
        case BINOP_DIV: if (either_real) { if (rd == 0.0) return FAILDESCR; real_result.v=DT_R; real_result.r=ld/rd; return real_result; } return ri ? INTVAL(li / ri) : FAILDESCR;
        case BINOP_MOD: return ri ? INTVAL(li % ri) : FAILDESCR;
        case BINOP_POW: { extern DESCR_t POWER_fn(DESCR_t, DESCR_t); return POWER_fn(lv, rv); }
        case BINOP_LT: *rel_fail = !(either_real ? ld <  rd : li <  ri); return *rel_fail ? FAILDESCR : rv;
        case BINOP_LE: *rel_fail = !(either_real ? ld <= rd : li <= ri); return *rel_fail ? FAILDESCR : rv;
        case BINOP_GT: *rel_fail = !(either_real ? ld >  rd : li >  ri); return *rel_fail ? FAILDESCR : rv;
        case BINOP_GE: *rel_fail = !(either_real ? ld >= rd : li >= ri); return *rel_fail ? FAILDESCR : rv;
        case BINOP_EQ: *rel_fail = !(either_real ? ld == rd : li == ri); return *rel_fail ? FAILDESCR : rv;
        case BINOP_NE: *rel_fail = !(either_real ? ld != rd : li != ri); return *rel_fail ? FAILDESCR : rv;
        case BINOP_CONCAT: case BINOP_CONCAT_FRACDIGIT: {
            DESCR_t ls_d; ls_d = (op == BINOP_CONCAT_FRACDIGIT) ? descr_to_str_fracdigit(lv) : descr_to_str(lv);
            DESCR_t rs_d; rs_d = (op == BINOP_CONCAT_FRACDIGIT) ? descr_to_str_fracdigit(rv) : descr_to_str(rv);
            if (IS_FAIL_fn(ls_d) || IS_FAIL_fn(rs_d)) return FAILDESCR;
            const char *ls = ls_d.s ? ls_d.s : "";
            const char *rs = rs_d.s ? rs_d.s : "";
            size_t ll = ls_d.slen > 0 ? (size_t)ls_d.slen : strlen(ls);
            size_t rl = rs_d.slen > 0 ? (size_t)rs_d.slen : strlen(rs);
            char *buf = rt_ws_alloc(ll + rl + 1);
            memcpy(buf, ls, ll); memcpy(buf + ll, rs, rl); buf[ll + rl] = '\0';
            { DESCR_t r2; r2.v = DT_S; r2.slen = (int)(ll + rl); r2.s = buf; return r2; }
        }
        case BINOP_XREP: {
            DESCR_t ls_d; ls_d = descr_to_str(lv);
            if (IS_FAIL_fn(ls_d)) return FAILDESCR;
            const char *ls = ls_d.s ? ls_d.s : "";
            size_t ll = ls_d.slen > 0 ? (size_t)ls_d.slen : strlen(ls);
            long cnt = IS_INT_fn(rv) ? rv.i : (IS_REAL_fn(rv) ? (long)rv.r : 0);
            if (cnt < 1 || ll == 0) { DESCR_t re; re.v = DT_S; re.slen = 0; re.s = rt_ws_alloc(1); re.s[0] = '\0'; return re; }
            size_t total = ll * (size_t)cnt;
            char *buf = rt_ws_alloc(total + 1);
            for (long k = 0; k < cnt; k++) memcpy(buf + (size_t)k * ll, ls, ll);
            buf[total] = '\0';
            { DESCR_t r2; r2.v = DT_S; r2.slen = (int)total; r2.s = buf; return r2; }
        }
        case BINOP_SLT: case BINOP_SLE: case BINOP_SGT:
        case BINOP_SGE: case BINOP_SEQ: case BINOP_SNE: {
            DESCR_t ls_d = descr_to_str(lv);
            DESCR_t rs_d = descr_to_str(rv);
            const char *ls = (!IS_FAIL_fn(ls_d) && ls_d.s) ? ls_d.s : "";
            const char *rs = (!IS_FAIL_fn(rs_d) && rs_d.s) ? rs_d.s : "";
            int cmp = strcmp(ls, rs);
            int ok;
            switch (op) {
            case BINOP_SLT: ok = (cmp <  0); break;
            case BINOP_SLE: ok = (cmp <= 0); break;
            case BINOP_SGT: ok = (cmp >  0); break;
            case BINOP_SGE: ok = (cmp >= 0); break;
            case BINOP_SEQ: ok = (cmp == 0); break;
            case BINOP_SNE: ok = (cmp != 0); break;
            default:            ok = 0;           break;
            }
            *rel_fail = !ok;
            return ok ? rv : FAILDESCR;
        }
        default: return FAILDESCR;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void lc_γ_to(IR_t * nd, IR_t * t) { if (nd) { nd->γ.node = t; memcpy(nd->γ.sz, "α", 3); nd->γ.sz[3] = 0; } }
void lc_ω_to(IR_t * nd, IR_t * t) { if (nd) { nd->ω.node = t; memcpy(nd->ω.sz, "α", 3); nd->ω.sz[3] = 0; } }
void lc_γ_to_β(IR_t * nd, IR_t * t) { if (nd) { nd->γ.node = t; memcpy(nd->γ.sz, "β", 3); nd->γ.sz[3] = 0; } }
void lc_γ_tag_β(IR_t * nd) { if (nd) { memcpy(nd->γ.sz, "β", 3); nd->γ.sz[3] = 0; } }
void lc_ω_to_β(IR_t * nd, IR_t * t) { if (nd) { nd->ω.node = t; memcpy(nd->ω.sz, "β", 3); nd->ω.sz[3] = 0; } }
void lc_γ_to_α(IR_t * nd, IR_t * t) { if (nd) { nd->γ.node = t; memcpy(nd->γ.sz, "α!", 4); } }
void lc_ω_to_α(IR_t * nd, IR_t * t) { if (nd) { nd->ω.node = t; memcpy(nd->ω.sz, "α!", 4); } }
IR_t * lc_build(IR_graph_t * g, IR_e op, IR_t * γ, IR_t * ω) { IR_t * nd = IR_node_alloc(g, op); lc_γ_to(nd, γ); lc_ω_to(nd, ω); return nd; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const tree_t * lc_stmt_subj(const tree_t * s) {
    for (int i = 0; i < s->n; i++) {
        const tree_t * a = s->c[i];
        if (a && a->t == TT_ATTR && a->v.sval && !strcmp(a->v.sval, ":subj")) return (a->n > 0) ? a->c[0] : NULL;
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void lc_vec_init(lc_vec * v, int esz) { v->data = NULL; v->n = 0; v->cap = 0; v->esz = esz; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void * lc_vec_push(lc_vec * v, const void * elem) {
    if (v->n >= v->cap) {
        int nc = v->cap ? v->cap * 2 : 8;
        void * nd = v->data ? rt_ws_realloc(v->data, (size_t) nc * (size_t) v->esz) : rt_ws_alloc((size_t) nc * (size_t) v->esz);
        if (!nd) return NULL;
        v->data = nd; v->cap = nc;
    }
    char * slot = (char *) v->data + (size_t) v->n * (size_t) v->esz;
    if (elem) memcpy(slot, elem, (size_t) v->esz); else memset(slot, 0, (size_t) v->esz);
    v->n++;
    return slot;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int lc_binop_code(tree_e tt) {
    switch (tt) {
    case TT_ADD: return 0; case TT_SUB: return 1; case TT_MUL: return 2; case TT_DIV: return 3; case TT_MOD: return 4;
    case TT_LT: return 5; case TT_LE: return 6; case TT_GT: return 7; case TT_GE: return 8;
    case TT_EQ: return 9; case TT_NE: return 10; case TT_CAT: return 11;
    case TT_LLT: return 12; case TT_LLE: return 13; case TT_LGT: return 14; case TT_LGE: return 15;
    case TT_LEQ: return 16; case TT_LNE: return 17; case TT_POW: return 18;
    case TT_CSET_UNION: return 19; case TT_CSET_DIFF: return 20; case TT_CSET_INTER: return 21;
    case TT_IDENTICAL: return 22; case TT_NIDENTICAL: return 23; case TT_XREP: return 24; default: return 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int lc_is_binop(tree_e tt) {
    switch (tt) {
    case TT_ADD: case TT_SUB: case TT_MUL: case TT_DIV: case TT_MOD: case TT_POW: case TT_LT: case TT_LE: case TT_GT: case TT_GE:
    case TT_EQ: case TT_NE: case TT_CAT: case TT_LLT: case TT_LLE: case TT_LGT: case TT_LGE: case TT_LEQ: case TT_LNE: return 1;
    case TT_CSET_UNION: case TT_CSET_DIFF: case TT_CSET_INTER: return 1;
    case TT_IDENTICAL: case TT_NIDENTICAL: return 1;
    case TT_XREP: return 1;
    default: return 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * lc_arg_block(IR_graph_t ** gslot, lc_lower_fn fn, void * cx, const tree_t * a) {
    IR_graph_t * saved = *gslot;
    IR_graph_t * g2 = IR_alloc(256); *gslot = g2;
    IR_t * F = IR_node_alloc(g2, IR_FAIL);
    IR_t * e = fn(cx, a, F);
    g2->entry = e;
    *gslot = saved;
    return g2;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void lc_call_argblks(IR_t * call, double dv, int nargs, lc_argblk_fn mk, void * cx, const tree_t * const * args) {
    IR_LIT(call).dval = dv;
    if (nargs <= 0) return;
    IR_graph_t ** blks = (IR_graph_t **) calloc((size_t) nargs, sizeof(IR_graph_t *));
    if (!blks) return;
    for (int k = 0; k < nargs; k++) blks[k] = mk(cx, args[k]);
    (void)(blks);
}
static struct { const IR_t ** nd; const char ** src; int n; int max; } g_bb_src = { 0, 0, 0, 0 };
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_src_note(const IR_t * nd, const char * src) {
    if (!nd || !src || !src[0]) return;
    for (int i = 0; i < g_bb_src.n; i++) {
        if (g_bb_src.nd[i] != nd) continue;
        { const char * h = g_bb_src.src[i]; size_t ls = strlen(src);
          while (h) { const char * e = strchr(h, '\n'); size_t seg = e ? (size_t)(e - h) : strlen(h);
                      if (seg == ls && !memcmp(h, src, ls)) return; h = e ? e + 1 : 0; } }
        size_t la = strlen(g_bb_src.src[i]);
        size_t lb = strlen(src);
        char * j = (char *) malloc(la + lb + 2);
        if (!j) return;
        memcpy(j, g_bb_src.src[i], la); j[la] = '\n'; memcpy(j + la + 1, src, lb); j[la + 1 + lb] = 0;
        g_bb_src.src[i] = lp_strdup(j);
        free(j);
        return;
    }
    if (g_bb_src.n >= g_bb_src.max) {
        int m = g_bb_src.max ? g_bb_src.max * 2 : 256;
        const IR_t ** a = (const IR_t **) realloc((void *) g_bb_src.nd, (size_t) m * sizeof(const IR_t *));
        const char ** b = (const char **) realloc((void *) g_bb_src.src, (size_t) m * sizeof(const char *));
        if (!a || !b) return;
        g_bb_src.nd = a; g_bb_src.src = b; g_bb_src.max = m;
    }
    g_bb_src.nd[g_bb_src.n] = nd;
    g_bb_src.src[g_bb_src.n] = lp_strdup(src);
    g_bb_src.n++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * bb_src_of(const IR_t * nd) {
    if (!nd) return 0;
    { static int _sd = -1; if (_sd < 0) { const char * e = getenv("SCRIP_SRC_DIAG"); _sd = (e && e[0] == '1') ? 1 : 0; }
      for (int i = 0; i < g_bb_src.n; i++) { if (g_bb_src.nd[i] != nd) continue;
          if (_sd) fprintf(stderr, "[SRC] hit nd=%p i=%d/%d src=%.44s\n", (const void *) nd, i, g_bb_src.n, g_bb_src.src[i] ? g_bb_src.src[i] : "-");
          return g_bb_src.src[i]; } }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_src_reset(void) {
    static int _sr = -1;
    if (_sr < 0) { const char * e = getenv("SCRIP_SRC_RESET"); _sr = (e && e[0] == '0') ? 0 : 1; }
    if (_sr) g_bb_src.n = 0;
}
