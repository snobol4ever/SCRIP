#include "lower/lower_snobol4/lsn_head.h"
#include "lower/lower_snobol4/lsn_01_sno.h"
#include "lower/lower_snobol4/lsn_03_sno.h"
#include "lower/lower_snobol4/lsn_04_sno.h"
#include "lower/lower_snobol4/lsn_05_sno.h"
#include "lower/lower_snobol4/lsn_06_sh.h"
#include "lower/lower_snobol4/lsn_07_sno.h"
#include "lower/lower_snobol4/lsn_08_sno.h"
#include "lower/lower_snobol4/lsn_09_sno.h"
int rt_kw_index(const char * kw);
extern int stage2_proc_grow(stage2_t * s2);
static int sno_pat_right_sealed(const tree_t * t);
static int sno_null_lit(const tree_t * t);
int sno_predef_registered(const char * fname) {
    if (!fname) return 0;
    for (uint32_t k = 0; k < g_lower.sno.predef.len; k++) if (!strcmp(CV_AT(g_lower.sno.predef, const char *, k), fname)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_e sno_pat_eff_kind(const tree_t * t) {
    if (!t) return TT_VAR;
    if ((t->t == TT_ARB || t->t == TT_BAL || t->t == TT_REM || t->t == TT_FAIL || t->t == TT_SUCCEED || t->t == TT_ABORT) && t->v.sval && sno_predef_registered(t->v.sval)) return TT_FNC;
    if ((t->t != TT_VAR && t->t != TT_KEYWORD) || !t->v.sval) return t->t;
    static const struct {
        const char * n;
        tree_e k;
    } m[] = { { "ABORT", TT_ABORT }, { "ARB", TT_ARB }, { "BAL", TT_BAL }, { "FAIL", TT_FAIL }, { "FENCE", TT_FENCE }, { "FLUSH", TT_FLUSH }, { "REM", TT_REM }, { "SUCCEED", TT_SUCCEED }, { NULL,
        TT_VAR } };
    const char * nm = t->v.sval[0] == '&' ? t->v.sval + 1 : t->v.sval;
    for (int i = 0; m[i].n; i++) if (!strcmp(nm, m[i].n)) return m[i].k;
    return t->t;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sno_is_fence(const tree_t * t) { if (!t) return 0; const tree_e k = sno_pat_eff_kind(t); return k == TT_FENCE || k == TT_FLUSH; }
int sno_is_flush(const tree_t * t) { return t && sno_pat_eff_kind(t) == TT_FLUSH; }
int sno_is_fence1(const tree_t * t) { return t && t->t == TT_FENCE && t->n > 0; }
int sno_is_fence0(const tree_t * t) { return sno_is_fence(t) && !sno_is_fence1(t); }
static void sno_tvec_push(sno_tvec_t * e, const tree_t * t) {
    if (e->n >= e->cap) { e->cap = e->cap ? e->cap * 2 : 16; e->v = (const tree_t **) ct_grow((void *) e->v, (size_t) e->cap * sizeof *e->v); }
    e->v[e->n++] = t;
}
static void sno_tvec_push_rc(sno_tvec_t * e, const tree_t * t, int rc) {
    if (e->n >= e->cap) {
        e->cap = e->cap ? e->cap * 2 : 16;
        e->v = (const tree_t **) ct_grow((void *) e->v, (size_t) e->cap * sizeof *e->v);
        e->rc = (char *) ct_grow((void *) e->rc, (size_t) e->cap);
    }
    e->rc[e->n] = (char) rc;
    e->v[e->n++] = t;
}
static void sno_seq_flatten_rc(const tree_t * t, int rc, int first, sno_tvec_t * e) {
    if (!t) return;
    if (t->t == TT_SEQ) {
        for (int i = 0; i < t->n; i++) {
            const tree_t * c = t->c[i];
            if (c && c->t == TT_SEQ && i == 0 && !first) sno_tvec_push_rc(e, c, 0);
            else sno_seq_flatten_rc(c, i > 0 && i == t->n - 1, first && i == 0, e);
        }
        return;
    }
    sno_tvec_push_rc(e, t, rc);
}
void sno_seq_flatten_pat(const tree_t * t, sno_tvec_t * e) { sno_seq_flatten_rc(t, 1, 1, e); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_seal_note(const char * nm, const tree_t * pat) {
    if (!nm || !pat) return;
    for (uint32_t i = 0; i < g_lower.sno.seal.len; i++) if (!strcmp(CV_AT(g_lower.sno.seal, sno_seal_ent_t, i).name, nm)) return;
    { sno_seal_ent_t x; x.name = nm; x.pat = pat; x.val = NULL; CV_PUSH(g_lower.sno.seal, sno_seal_ent_t) = x; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_const_feature(int set_off) {
    if (set_off) { g_lower.sno.sno_const_feature_cs = 0; return 0; }
    if (g_lower.sno.sno_const_feature_cs < 0) { const char * e = getenv("SCRIP_CONST_STATIC"); g_lower.sno.sno_const_feature_cs = (e && *e == '0') ? 0 : 1; }
    return g_lower.sno.sno_const_feature_cs;
}
int sno_const_static_on(void) { return sno_const_feature(0); }
const tree_t * sno_const_pat(const char * ck) {
    if (!sno_const_static_on() || !g_lower.sno.seal_enabled || !ck) return NULL;
    for (int i = 0; i < (int) g_lower.sno.seal.len; i++) if (!strcmp(CV_AT(g_lower.sno.seal, sno_seal_ent_t, i).name, ck)) return CV_AT(g_lower.sno.seal, sno_seal_ent_t, i).pat;
    return NULL;
}
static int sno_const_t1_on(void) {
    if (g_lower.sno.sno_const_t1_on_t < 0) { const char * e = getenv("SCRIP_CONST_T1"); g_lower.sno.sno_const_t1_on_t = (e && *e == '0') ? 0 : 1; }
    return g_lower.sno.sno_const_t1_on_t;
}
static int sno_const_scalar_tree(const tree_t * t) { return t && (t->t == TT_ILIT || t->t == TT_FLIT || t->t == TT_QLIT); }
static void sno_const_note_val(const char * nm, const tree_t * val) {
    if (!nm || !val) return;
    for (uint32_t i = 0; i < g_lower.sno.seal.len; i++) if (!strcmp(CV_AT(g_lower.sno.seal, sno_seal_ent_t, i).name, nm)) return;
    { sno_seal_ent_t x; x.name = nm; x.pat = NULL; x.val = val; CV_PUSH(g_lower.sno.seal, sno_seal_ent_t) = x; }
}
static const tree_t * sno_protected_kw_const(const char * ck) {
    const char * v = !strcmp(ck, "&UCASE") ? "ABCDEFGHIJKLMNOPQRSTUVWXYZ" : !strcmp(ck, "&LCASE") ? "abcdefghijklmnopqrstuvwxyz" : (const char *) 0;
    if (!v) return (const tree_t *) 0;
    { tree_t * q = ast_node_new(TT_QLIT); q->v.sval = (char *) v; return q; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const tree_t * sno_const_val(const char * ck) {
    if (ck) { const tree_t * pk = sno_protected_kw_const(ck); if (pk) return pk; }
    if (!sno_const_static_on() || !sno_const_t1_on() || !g_lower.sno.seal_enabled || !ck) return NULL;
    if (rt_kw_index(ck) >= 0) return NULL;
    for (int i = 0; i < (int) g_lower.sno.seal.len; i++) if (!strcmp(CV_AT(g_lower.sno.seal, sno_seal_ent_t, i).name, ck)) return CV_AT(g_lower.sno.seal, sno_seal_ent_t, i).val;
    return NULL;
}
int sno_name_prologue_bound(const char * nm) { if (!nm) return 0; for (uint32_t i = 0; i < g_lower.sno.pro.len; i++) if (!strcmp(CV_AT(g_lower.sno.pro, const char *, i), nm)) return 1; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_t4_on(void) { if (g_lower.sno.sno_t4_on_v < 0) { const char * e = getenv("SCRIP_OPSYN_FOLD"); g_lower.sno.sno_t4_on_v = (e && *e == '0') ? 0 : 1; } return g_lower.sno.sno_t4_on_v; }
static int sno_t4_opchar(char c) { return !((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_' || c == '.'); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_t4_scan(const tree_t * t) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) sno_t4_scan(t->c[i]);
    if (t->t != TT_FNC) return;
    const char * nm = t->v.sval;
    int ab = 0;
    if (!nm && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR) { nm = t->c[0]->v.sval; ab = 1; }
    if (!nm || strcmp(nm, "OPSYN")) return;
    {
        int na = t->n - ab;
        const tree_t * a0 = (na > 0) ? t->c[ab] : NULL;
        const tree_t * a1 = (na > 1) ? t->c[ab + 1] : NULL;
        const tree_t * a2 = (na > 2) ? t->c[ab + 2] : NULL;
        const char * op = (a0 && a0->t == TT_QLIT) ? a0->v.sval : NULL;
        const char * tg = (a1 && a1->t == TT_QLIT) ? a1->v.sval : NULL;
        long ar = (a2 && a2->t == TT_ILIT) ? a2->v.ival : -1;
        if (op && tg && *op && sno_t4_opchar(*op)) {
            if (na == 3 && (ar == 1 || ar == 2)) {
                for (int i = 0; i < (int) g_lower.sno.t4.len; i++) if (!strcmp(CV_AT(g_lower.sno.t4, sno_t4_t, i).op, op)) {
                    if (strcmp(CV_AT(g_lower.sno.t4, sno_t4_t, i).tgt, tg) || CV_AT(g_lower.sno.t4, sno_t4_t, i).arity != (int) ar) CV_AT(g_lower.sno.t4, sno_t4_t, i).poisoned = 1;
                    return;
                }
                { sno_t4_t x; x.op = op; x.tgt = tg; x.arity = (int) ar; x.poisoned = 0; CV_PUSH(g_lower.sno.t4, sno_t4_t) = x; }
                return;
            }
            g_lower.sno.t4_unsafe = 1;
            return;
        }
        if (op && tg) return;
        g_lower.sno.t4_unsafe = 1;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * sno_t4_target(const char * op, int nops) {
    if (!sno_t4_on() || g_lower.sno.t4_unsafe || !op) return NULL;
    for (int i = 0; i < (int) g_lower.sno.t4.len; i++)
        if (!CV_AT(g_lower.sno.t4, sno_t4_t, i).poisoned && !strcmp(CV_AT(g_lower.sno.t4, sno_t4_t, i).op, op) && CV_AT(g_lower.sno.t4, sno_t4_t, i).arity == nops)
        return CV_AT(g_lower.sno.t4, sno_t4_t, i).tgt;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void sno_prog_scan(const tree_t ** st, int nst) {
    g_lower.sno.pro.len = 0;
    g_lower.sno.t4.len = 0;
    g_lower.sno.t4_unsafe = 0;
    for (int i = 0; i < nst; i++) {
        const tree_t * s = st[i];
        if (!s) continue;
        sno_t4_scan(s);
        const tree_t * subj = lc_stmt_subj(s);
        const tree_t * repl = sfind_expr(s, ":repl");
        int has_eq = sfind(s, ":eq") != NULL;
        if (!has_eq || !subj || subj->t != TT_KEYWORD || !subj->v.sval) continue;
        if (repl) {
            const char * kn = subj->v.sval[0] == '&' ? subj->v.sval + 1 : subj->v.sval;
            if (!strcasecmp(kn, "USER_DECLARED_CONSTANTS") && repl->t == TT_ILIT && repl->v.ival == 0) sno_const_feature(1);
        }
        if (sno_const_static_on() && repl && sno_is_pattern_rhs(repl) && sno_pat_supported(repl)) {
            char cb[130];
            snprintf(cb, sizeof cb, "&%s", subj->v.sval[0] == '&' ? subj->v.sval + 1 : subj->v.sval);
            sno_seal_note(lp_strdup(cb), repl);
        }
        if (sno_const_static_on() && sno_const_t1_on() && sno_const_scalar_tree(repl)) {
            char vb[130];
            snprintf(vb, sizeof vb, "&%s", subj->v.sval[0] == '&' ? subj->v.sval + 1 : subj->v.sval);
            if (rt_kw_index(vb) < 0) sno_const_note_val(lp_strdup(vb), repl);
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sno_cap_name_strict(void) {
    if (g_lower.sno.sno_cap_name_strict_v < 0) { const char * e = getenv("SCRIP_CAP_NAME_STRICT"); g_lower.sno.sno_cap_name_strict_v = (e && *e == '0') ? 0 : 1; }
    return g_lower.sno.sno_cap_name_strict_v;
}
static int sno_rtseq_resume(void) {
    if (g_lower.sno.sno_rtseq_resume_v < 0) { const char * e = getenv("SCRIP_RTSEQ_RESUME"); g_lower.sno.sno_rtseq_resume_v = (e && *e == '0') ? 0 : 1; }
    return g_lower.sno.sno_rtseq_resume_v;
}
static int sno_defer_resume(void) {
    if (g_lower.sno.sno_defer_resume_v < 0) { const char * e = getenv("SCRIP_DEFER_RESUME"); g_lower.sno.sno_defer_resume_v = (e && *e == '0') ? 0 : 1; }
    return g_lower.sno.sno_defer_resume_v;
}
int sno_seq_tail(void) {
    if (g_lower.sno.sno_seq_tail_v < 0) { const char * e = getenv("SCRIP_SEQ_TAIL"); g_lower.sno.sno_seq_tail_v = (e && *e == '0') ? 0 : 1; }
    return g_lower.sno.sno_seq_tail_v;
}
int sno_alt_tail(void) { const char * e = getenv("SCRIP_ALT_TAIL"); return (e && *e == '0') ? 0 : 1; }
static int sno_fence_rtail(void) { const char * e = getenv("SCRIP_FENCE_RTAIL"); return (e && *e == '0') ? 0 : 1; }
static int sno_pat_contains_fence(const tree_t * t, int depth) {
    if (!t || depth > 64) return 0;
    if (sno_is_fence(t)) return 1;
    for (int i = 0; i < t->n; i++) if (sno_pat_contains_fence(t->c[i], depth + 1)) return 1;
    return 0;
}
static int sno_pat_contains_fence0(const tree_t * t, int depth) {
    if (!t || depth > 64) return 0;
    if (sno_is_fence0(t)) return 1;
    for (int i = 0; i < t->n; i++) if (sno_pat_contains_fence0(t->c[i], depth + 1)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * sno_seq_nary(scx_t * cx, const tree_t ** elems, const char * rc, int ne, IR_t * succ, IR_t * fail, IR_t ** out_rtail) {
    IR_graph_t * g = cx->g;
    IR_t * S = lc_build(g, IR_GOTO, succ, NULL);
    sno_ω_to(S, fail);
    IR_t ** ent = (IR_t **) ct_alloc((size_t) (ne > 0 ? ne : 1) * sizeof *ent);
    IR_t ** res = (IR_t **) ct_alloc((size_t) (ne > 0 ? ne : 1) * sizeof *res);
    int * lo = (int *) ct_alloc((size_t) (ne > 0 ? ne : 1) * sizeof *lo);
    for (int i = 0; i < ne; i++) {
        int before = g->n;
        cx->ppos = rc[i] ? 9 : 8;
        IR_t * ei = sno_pat_node(cx, elems[i], S, S);
        int _rb = before;
        while (_rb < g->n && g->all[_rb] && g->all[_rb]->op == IR_GOTO && g->all[_rb]->γ.node == S && g->all[_rb]->ω.node == S && g->all[_rb]->n_operands == 0) _rb++;
        IR_t * ri = (_rb < g->n) ? g->all[_rb] : ei;
        IR_t * ti = NULL;
        for (int k = before; k < g->n; k++) {
            IR_t * x = g->all[k];
            if (!x) continue;
            if (x->ω.node == S) { memcpy(x->ω.sz, "φ", 3); x->ω.sz[3] = 0; }
            if (x->γ.node == S) { if (x->op == IR_GOTO && x->ω.node == S) { memcpy(x->γ.sz, "φ", 3); } else { memcpy(x->γ.sz, "σ", 3); ti = x; } x->γ.sz[3] = 0; }
        }
        if (sno_seq_tail() && ti) ri = ti;
        ent[i] = ei;
        res[i] = ri;
        lo[i] = before;
    }
    for (int i = 0; i < ne; i++) {
        IR_t * nxt = (i + 1 < ne) ? ent[i + 1] : succ;
        IR_t * prv = (i > 0 && (res[i - 1]->op != IR_MATCH_DEFER || (sno_defer_resume() && res[i - 1]->seal != 1))) ? res[i - 1] : fail;
        int lo_i = lo[i];
        int hi_i = (i + 1 < ne) ? lo[i + 1] : g->n;
        for (int k = (lo_i > 0 ? lo_i : 0); k < g->n && k < hi_i; k++) {
            IR_t * x = g->all[k];
            if (!x || x == S) continue;
            if (x->ω.node == S && x->ω.sz[0] == (char)0xcf && (unsigned char)x->ω.sz[1] == 0x86) { x->ω.node = prv; memcpy(x->ω.sz, "β", 3); x->ω.sz[3] = 0; }
            if (x->γ.node == S && x->γ.sz[0] == (char)0xcf && (unsigned char)x->γ.sz[1] == 0x86) { x->γ.node = prv; memcpy(x->γ.sz, "β", 3); x->γ.sz[3] = 0; }
            if (x->γ.node == S && x->γ.sz[0] == (char)0xcf && (unsigned char)x->γ.sz[1] == 0x83) { x->γ.node = nxt; x->γ.sz[0] = 0; }
        }
    }
    for (int k = 0; k < g->n; k++) {
        IR_t * x = g->all[k];
        if (!x || x == S) continue;
        if (x->ω.node == S && x->ω.sz[0] == (char)0xcf && (unsigned char)x->ω.sz[1] == 0x86) { x->ω.node = fail; memcpy(x->ω.sz, "β", 3); x->ω.sz[3] = 0; }
        if (x->γ.node == S && x->γ.sz[0] == (char)0xcf && (unsigned char)x->γ.sz[1] == 0x86) { x->γ.node = fail; memcpy(x->γ.sz, "β", 3); x->γ.sz[3] = 0; }
        if (x->γ.node == S && x->γ.sz[0] == (char)0xcf && (unsigned char)x->γ.sz[1] == 0x83) { x->γ.node = succ; x->γ.sz[0] = 0; }
    }
    S->γ.node = succ;
    S->γ.sz[0] = 0;
    if (out_rtail) *out_rtail = (ne > 0) ? res[ne - 1] : NULL;
    { IR_t * first = (ne > 0) ? ent[0] : succ; ct_drop(lo); ct_drop(res); ct_drop(ent); return first; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * sno_capt_body(scx_t * cx, const tree_t * t, IR_t * succ, IR_t * fail, IR_t ** out_itail) {
    IR_graph_t * g = cx->g;
    const tree_t * eff = t;
    if (sno_pat_eff_kind(eff) == TT_SEQ) {
        sno_tvec_t ev = {0};
        sno_seq_flatten_pat(eff, &ev);
        const tree_t ** elems = ev.v;
        int ne = ev.n;
        int nf = ne;
        for (int i = 0; i < ne; i++) if (sno_is_fence(elems[i])) { nf = i; break; }
        if (nf == ne && ne > 1) { IR_t * rt = NULL; IR_t * pe = sno_seq_nary(cx, elems, ev.rc, ne, succ, fail, &rt); ct_drop(ev.v); ct_drop(ev.rc); *out_itail = rt ? rt : pe; return pe; }
        ct_drop(ev.v);
        ct_drop(ev.rc);
    }
    int before = g->n;
    IR_t * pe = sno_pat_node(cx, t, succ, fail);
    IR_t * raw = (before < g->n) ? g->all[before] : pe;
    *out_itail = (raw && raw->op == IR_GOTO && raw->n_operands == 0 && raw->γ.node == succ && raw->ω.node == fail) ? pe : raw;
    return pe;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sno_pat_supported(const tree_t * t) {
    if (!t) return 0;
    const tree_e k = sno_pat_eff_kind(t);
    if (k == TT_FLUSH) return 1;
    if (k == TT_FENCE) return t->n == 0 || sno_pat_supported(t->c[0]);
    if (k == TT_QLIT) return 1;
    if (k == TT_ANY || k == TT_NOTANY) return t->n > 0 && t->c[0] && (t->c[0]->t != TT_DEFER || t->c[0]->n > 0);
    if (k == TT_SPAN) return t->n > 0 && t->c[0] && (t->c[0]->t != TT_DEFER || t->c[0]->n > 0);
    if (k == TT_BREAK || k == TT_BREAKX) return t->n > 0 && t->c[0] && (t->c[0]->t != TT_DEFER || t->c[0]->n > 0);
    if (k == TT_TAB || k == TT_RTAB) return t->n > 0 && t->c[0] != NULL;
    if (k == TT_POS || k == TT_RPOS) return t->n > 0 && t->c[0] != NULL;
    if (k == TT_REM || k == TT_ARB) return 1;
    if (k == TT_ABORT || k == TT_FAIL) return 1;
    if (k == TT_BAL) return 1;
    if (k == TT_SUCCEED) return 0;
    if (k == TT_ARBNO) return t->n > 0 && t->c[0] && sno_pat_supported(t->c[0]);
    if (k == TT_VAR) return t->v.sval != NULL;
    if (k == TT_KEYWORD) return t->v.sval != NULL;
    if (k == TT_DEFER) return t->n > 0 && t->c[0] != NULL;
    if (k == TT_LEN) return t->n > 0 && t->c[0] && (t->c[0]->t != TT_DEFER || t->c[0]->n > 0);
    if (k == TT_CAPT_COND_ASGN) return t->n > 1 && t->c[1] && (sno_capt_name(t->c[1]) != NULL || t->c[1]->t == TT_DEFER || sno_item_argbase(t->c[1]) >= 0 || sno_capt_elem_tg(t->c[1], 0)) &&
        sno_pat_supported(t->c[0]);
    if (k == TT_CAPT_IMMED_ASGN) return t->n > 1 && t->c[1] && (sno_capt_name(t->c[1]) != NULL || t->c[1]->t == TT_DEFER || sno_item_argbase(t->c[1]) >= 0 || sno_capt_elem_tg(t->c[1], 0)) &&
        sno_pat_supported(t->c[0]);
    if (k == TT_CAPT_CURSOR) return t->n > 0 && t->c[0] && ((t->c[0]->t == TT_VAR && t->c[0]->v.sval) || (t->c[0]->t == TT_DEFER && t->c[0]->n > 0) || sno_capt_elem_tg(t->c[0], 1));
    if (k == TT_SEQ) return sno_pat_supported((t->n > 0) ? t->c[0] : NULL) && sno_pat_supported((t->n > 1) ? t->c[1] : NULL);
    if (k == TT_ALT) return sno_pat_supported((t->n > 0) ? t->c[0] : NULL) && sno_pat_supported((t->n > 1) ? t->c[1] : NULL);
    if (k == TT_FNC) return 1;
    return 0;
}
static int sno_cat_runtime_shape(const tree_t * t, int * ndefer) {
    if (!t) return 0;
    if (t->t == TT_DEFER) { (*ndefer)++; return 1; }
    if (t->t == TT_VAR || sno_null_lit(t)) return 1;
    if ((t->t == TT_SEQ || t->t == TT_CAT) && t->n > 1) return sno_cat_runtime_shape(t->c[0], ndefer) && sno_cat_runtime_shape(t->c[1], ndefer);
    return 0;
}
int sno_is_pattern_rhs(const tree_t * t) {
    if (!t) return 0;
    switch (sno_pat_eff_kind(t)) {
        case TT_ABORT:
        case TT_SUCCEED:
        case TT_ALT:
        case TT_FENCE:
        case TT_FLUSH:
        case TT_ARBNO:
        case TT_ANY:
        case TT_NOTANY:
        case TT_SPAN:
        case TT_BREAK:
        case TT_BREAKX:
        case TT_LEN:
        case TT_TAB:
        case TT_RTAB:
        case TT_POS:
        case TT_RPOS:
        case TT_ARB:
        case TT_REM:
        case TT_BAL:
        case TT_CAPT_COND_ASGN:
        case TT_CAPT_IMMED_ASGN:
        return 1;
        case TT_CAPT_CURSOR:
        return t->n > 0 && sno_capt_elem_tg(t->c[0], 1);
        case TT_SEQ:
        case TT_CAT:
        {
            const tree_t * a = (t->n > 0) ? t->c[0] : NULL;
            const tree_t * b = (t->n > 1) ? t->c[1] : NULL;
            { int nd = 0; if (sno_cat_runtime_shape(t, &nd) && nd > 0) return 0; }
            if ((a && a->t == TT_DEFER) || (b && b->t == TT_DEFER)) return 1;
            return sno_is_pattern_rhs(a) || sno_is_pattern_rhs(b);
        }
        case TT_KEYWORD:
        {
            if (!sno_const_static_on() || g_lower.sno.sno_is_pattern_rhs_kdepth >= 32 || !t->v.sval) return 0;
            char cb[130];
            snprintf(cb, sizeof cb, "&%s", t->v.sval[0] == '&' ? t->v.sval + 1 : t->v.sval);
            const tree_t * p = sno_const_pat(cb);
            if (!p) return 0;
            g_lower.sno.sno_is_pattern_rhs_kdepth++;
            int r = sno_is_pattern_rhs(p);
            g_lower.sno.sno_is_pattern_rhs_kdepth--;
            return r;
        }
        default:
        return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_null_lit(const tree_t * t) { return t && ((t->t == TT_QLIT && (!t->v.sval || !t->v.sval[0])) || t->t == TT_NUL); }
static int sno_seq_is_chain(const tree_t * t) { return t && (t->t == TT_SEQ || t->t == TT_CAT) && t->n > 1; }
static const tree_t * sno_seq_last_leaf(const tree_t * t) { while (sno_seq_is_chain(t)) t = t->c[t->n - 1]; return t; }
static int sno_seq_rtl(const tree_t * t, int first, int (*g)(const tree_t *)) {
    if (sno_seq_is_chain(t)) { for (int i = t->n - 1; i >= 0; i--) { int r = sno_seq_rtl(t->c[i], first && i == 0, g); if (r >= 0) return r; } return -1; }
    return (first || !sno_is_fence1(t)) ? g(t) : -1;
}
int sno_mkpat_here(const tree_t * t) {
    if (g_lower.sno.sno_mkpat_here_on < 0) { const char * e = getenv("SCRIP_MKPAT_ANYWHERE"); g_lower.sno.sno_mkpat_here_on = (e && *e == '0') ? 0 : 1; }
    if (!g_lower.sno.sno_mkpat_here_on || !t || t->t == TT_VAR || t->t == TT_KEYWORD || t->t == TT_INDIRECT || t->t == TT_DEFER || !sno_is_pattern_rhs(t) || !sno_pat_supported(t)) return 0;
    if (sno_seq_is_chain(t) && (sno_null_lit(sno_seq_last_leaf(t)) || (sno_null_lit(t->c[0]) && !sno_seq_is_chain(t->c[1])))) return 0;
    IR_graph_t * tg = IR_alloc(256);
    scx_t tx;
    tx.g = tg;
    tx.wn_tree = NULL;
    tx.loop_exit = NULL;
    tx.loop_next = NULL;
    tx.result_name = NULL;
    tx.pat_fail = NULL;
    tx.pat_seal = NULL;
    tx.npre = 0;
    tx.snap = 1;
    tx.capro = NULL;
    tx.capro_cand = NULL;
    tx.ppos = 0;
    tx.prog_nstmt = 0;
    tx.stno_base = 0;
    IR_t * tok = lc_build(tg, IR_SUCCEED, NULL, NULL);
    IR_t * tno = lc_build(tg, IR_FAIL, NULL, NULL);
    tx.pat_fail = tno;
    tx.pat_seal = sno_thunk_abort_exit(tg, tno);
    sno_pat_node(&tx, t, tok, tno);
    for (int pk = 0; pk < tx.npre; pk++) if (tx.pre[pk].snapg != sno_elem_snapg) return 0;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_pat_right_sealed(const tree_t * t) {
    if (!t) return 0;
    if (sno_is_fence(t)) return 1;
    if (sno_seq_is_chain(t)) return sno_seq_rtl(t, 1, sno_pat_right_sealed) > 0;
    if ((t->t == TT_CAPT_COND_ASGN || t->t == TT_CAPT_IMMED_ASGN) && t->n > 0) return sno_pat_right_sealed(t->c[0]);
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * sno_capt_elem_target(const tree_t * tg, sno_tvec_t * pro) {
    char nb[32];
    snprintf(nb, sizeof nb, "SN4$CAPN%d", g_lower.sno.sno_lower_match_snapctr++);
    char * nm = lp_strdup(nb);
    sno_reg_var(nm);
    tree_t * lv = ast_node_new(TT_VAR);
    lv->v.sval = nm;
    tree_t * nx = ast_node_new(TT_NAME);
    ast_push(nx, (tree_t *) tg);
    tree_t * asg = ast_node_new(TT_ASSIGN);
    ast_push(asg, lv);
    ast_push(asg, nx);
    sno_tvec_push(pro, asg);
    tree_t * rv = ast_node_new(TT_VAR);
    rv->v.sval = nm;
    tree_t * ind = ast_node_new(TT_INDIRECT);
    ast_push(ind, rv);
    tree_t * df = ast_node_new(TT_DEFER);
    ast_push(df, ind);
    return df;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const tree_t * sno_capt_elem_rewrite(const tree_t * t, sno_tvec_t * pro) {
    if (!t) return t;
    const int capt = (t->t == TT_CAPT_COND_ASGN || t->t == TT_CAPT_IMMED_ASGN) && t->n > 1;
    const int curs = t->t == TT_CAPT_CURSOR && t->n > 0;
    if (!capt && !curs && t->t != TT_SEQ && t->t != TT_CAT && t->t != TT_ALT && t->t != TT_ARBNO && t->t != TT_FENCE) return t;
    const tree_t * tg = capt ? t->c[1] : curs ? t->c[0] : NULL;
    const int elem = sno_capt_elem_tg(tg, curs);
    tree_t * u = NULL;
    for (int i = 0; i < t->n; i++) {
        const tree_t * ci = t->c[i];
        const tree_t * ri = (elem && ci == tg) ? sno_capt_elem_target(tg, pro) : (ci == tg) ? ci : sno_capt_elem_rewrite(ci, pro);
        if (ri != ci && !u) { u = ast_node_new(t->t); u->v = t->v; u->line = t->line; u->slen = t->slen; for (int j = 0; j < i; j++) ast_push(u, t->c[j]); }
        if (u) ast_push(u, (tree_t *) ri);
    }
    return u ? u : t;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * sno_capt_prologue(scx_t * cx, sno_tvec_t * pro, IR_t * entry, IR_t * fail) {
    for (int k = pro->n - 1; k >= 0; k--) { IR_t * r = NULL; if (pro->v[k]) entry = sx_lower(cx, pro->v[k], entry, fail, &r); }
    ct_drop(pro->v);
    pro->v = NULL;
    pro->n = 0;
    pro->cap = 0;
    return entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * sx_scan(scx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    sno_tvec_t cpro = { 0 };
    const tree_t * p0 = (t->n > 1) ? t->c[1] : NULL;
    const tree_t * p1 = sno_capt_elem_rewrite(p0, &cpro);
    if (p1 != p0) { extern tree_t * ast_stmt_new(tree_e kind); tree_t * sc = ast_stmt_new(TT_SCAN); ast_push(sc, t->c[0]); ast_push(sc, (tree_t *) p1); t = sc; }
    sno_tvec_t * svc = cx->capro_cand;
    cx->capro_cand = &cpro;
    IR_t * e = sx_scan_body(cx, t, γ, ω, res);
    cx->capro_cand = svc;
    return sno_capt_prologue(cx, &cpro, e, ω);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * sx_assign(scx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    extern tree_t * ast_stmt_new(tree_e kind);
    sno_tvec_t apro = { 0 };
    const tree_t * lt = (t->n > 0) ? t->c[0] : NULL;
    const tree_t * rt = (t->n > 1) ? t->c[1] : NULL;
    const tree_t * l1 = lt;
    const tree_t * r1 = rt;
    if (lt && lt->t == TT_SCAN && lt->n > 1) {
        const tree_t * p = sno_capt_elem_rewrite(lt->c[1], &apro);
        if (p != lt->c[1]) { tree_t * sc = ast_stmt_new(TT_SCAN); ast_push(sc, lt->c[0]); ast_push(sc, (tree_t *) p); l1 = sc; }
    } else if (lt && lt->t == TT_SEQ && lt->n > 1) {
        tree_t * sq = NULL;
        for (int k = 0; k < lt->n; k++) {
            const tree_t * ck = lt->c[k];
            const tree_t * rk = (k > 0) ? sno_capt_elem_rewrite(ck, &apro) : ck;
            if (rk != ck && !sq) { sq = ast_stmt_new(TT_SEQ); sq->v = lt->v; for (int m = 0; m < k; m++) ast_push(sq, lt->c[m]); }
            if (sq) ast_push(sq, (tree_t *) rk);
        }
        if (sq) l1 = sq;
    }
    if (rt && rt->t == TT_SCAN && rt->n > 1) {
        const tree_t * p = sno_capt_elem_rewrite(rt->c[1], &apro);
        if (p != rt->c[1]) { tree_t * sc = ast_stmt_new(TT_SCAN); ast_push(sc, rt->c[0]); ast_push(sc, (tree_t *) p); r1 = sc; }
    }
    if (l1 != lt || r1 != rt) {
        tree_t * as = ast_stmt_new(TT_ASSIGN);
        as->v = t->v;
        as->line = t->line;
        ast_push(as, (tree_t *) l1);
        ast_push(as, (tree_t *) r1);
        for (int k = 2; k < t->n; k++) ast_push(as, t->c[k]);
        t = as;
    }
    sno_tvec_t * svc = cx->capro_cand;
    cx->capro_cand = &apro;
    IR_t * e = sx_assign_body(cx, t, γ, ω, res);
    cx->capro_cand = svc;
    return sno_capt_prologue(cx, &apro, e, ω);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_prim_defer_named(const IR_t * x) {
    switch (x->op) {
        case IR_MATCH_ANY:
        case IR_MATCH_NOTANY:
        case IR_MATCH_SPAN:
        case IR_MATCH_BREAK:
        case IR_MATCH_BREAKX:
        return x->pat_static && x->n_operands == 0 && IR_LIT(x).sval;
        case IR_MATCH_TAB:
        case IR_MATCH_RTAB:
        case IR_MATCH_POS:
        case IR_MATCH_RPOS:
        case IR_MATCH_LEN:
        return x->n_operands == 0 && (uintptr_t)IR_LIT(x).ival > (uintptr_t)0xFFFFU && IR_LIT(x).sval[0] == '*';
        default:
        return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_match_traps_insert(IR_graph_t * g, int p0, IR_t * seal) {
    for (int q = p0; q < g->n && seal; q++) {
        IR_t * x = g->all[q];
        if (!x || !sno_prim_defer_named(x) || !x->ω.node || x->ω.node == seal) continue;
        IR_t * tp = lc_build(g, IR_MATCH_TRAP, NULL, NULL);
        tp->γ = x->ω;
        tp->ω = x->ω;
        ir_operand_push(tp, seal);
        lc_ω_to(x, tp);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * sno_lower_match(scx_t * cx, const tree_t * subj, const tree_t * repl_t, int has_repl, IR_t * sJ, IR_t * fJ, IR_t ** out_land) {
    const int p0 = cx->g->n;
    IR_t * e = sno_lower_match_body(cx, subj, repl_t, has_repl, sJ, fJ, out_land);
    sno_match_traps_insert(cx->g, p0, cx->pat_seal);
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sno_expr_mark(void) { return (int) g_lower.sno.exprs.len; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sno_pat_carrier_build(scx_t * px, const tree_t * pat, IR_t * ok, IR_t * no, IR_t ** out_brt, int * out_pfenced) {
    IR_t * pe;
    *out_brt = NULL;
    int pfenced = sno_pat_contains_fence(pat, 0);
    if (getenv("SCRIP_FENCE_IGNORE")) pfenced = 0;
    {
        sno_tvec_t fv = {0};
        sno_seq_flatten_pat(pat, &fv);
        const tree_t ** fel = fv.v;
        int fne = fv.n;
        int topf = 0;
        for (int i = 0; i < fne; i++) if (sno_is_fence(fel[i])) { topf = 1; break; }
        if (fne > 1 && !topf && !sno_pat_contains_fence0(pat, 0)) pfenced = 0;
        px->seq_rlast = NULL;
        px->seq_rlast_for = NULL;
        px->seq_rtail = NULL;
        pe = (sno_defer_resume() && fne > 1 && (!pfenced || (sno_fence_rtail() && !topf))) ? sno_seq_nary(px, fel, fv.rc, fne, ok, no, out_brt) : sno_pat_node(px, pat, ok, no);
        if (topf && !*out_brt && px->seq_rlast_for == pe && px->seq_rtail) *out_brt = px->seq_rtail;
        ct_drop(fv.v);
        ct_drop(fv.rc);
    }
    *out_pfenced = pfenced;
    return pe;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_pat_right_fence0(const tree_t * t) {
    if (!t) return 0;
    if (sno_is_fence0(t) || sno_is_flush(t)) return 1;
    if (sno_seq_is_chain(t)) return sno_seq_rtl(t, 1, sno_pat_right_fence0) > 0;
    if ((t->t == TT_CAPT_COND_ASGN || t->t == TT_CAPT_IMMED_ASGN) && t->n > 0) return sno_pat_right_fence0(t->c[0]);
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_pat_publish_body_root(IR_graph_t * gp, int before_pat, const tree_t * pat, IR_t * brt, int pfenced, const char * dbgname, IR_t * abort_nd) {
    IR_t * rn = NULL;
    if (sno_defer_resume() && !pfenced) {
        rn = brt;
        if (!rn) for (int k2 = before_pat; k2 < gp->n; k2++) { IR_t * x2 = gp->all[k2]; if (x2 && !(x2->op == IR_GOTO && x2->n_operands == 0)) { rn = x2; break; } }
    } else if (sno_defer_resume() && pfenced) {
        extern int zdp_seam_tier(const IR_t *);
        const char * _fre = getenv("SCRIP_FENCE_RESUME");
        if (!(_fre && *_fre == '0')) {
            IR_t * _c = (sno_fence_rtail() && brt) ? brt : NULL;
            if (!_c) for (int k2 = before_pat; k2 < gp->n; k2++) { IR_t * x2 = gp->all[k2]; if (x2 && !(x2->op == IR_GOTO && x2->n_operands == 0)) { _c = x2; break; } }
            { int _t2 = zdp_seam_tier(_c); if (_t2 == 1 || _t2 == 2 || _t2 == 3) rn = _c; }
        }
    }
    int rs = sno_pat_right_sealed(pat) ? 1 : 0;
    gp->body_root = (gp->n > before_pat && !rs) ? ((sno_defer_resume() && pfenced && !rn) ? NULL : (rn ? rn : gp->all[before_pat])) : NULL;
    if (abort_nd && sno_pat_right_fence0(pat)) gp->body_root = abort_nd;
    if (getenv("SCRIP_RESUME_WHY") && !gp->body_root) {
        extern int zdp_seam_tier(const IR_t *);
        const IR_t * _fb = NULL;
        for (int k3 = before_pat; k3 < gp->n; k3++) { IR_t * x3 = gp->all[k3]; if (x3 && !(x3->op == IR_GOTO && x3->n_operands == 0)) { _fb = x3; break; } }
        fprintf(stderr, "[RESUME-NIL] pat=%s empty=%d right_sealed=%d pfenced=%d rn=%d brt=%d fb=%s fbtier=%d chain=%s|%s|%s|%s\n", dbgname ? dbgname : "?", !(gp->n > before_pat), rs, pfenced,
            rn ? 1 : 0, brt ? 1 : 0, _fb ? bb_op_name(_fb->op) : "-", zdp_seam_tier(_fb), (before_pat + 0 < gp->n) ? bb_op_name(gp->all[before_pat + 0]->op) : "-",
            (before_pat + 1 < gp->n) ? bb_op_name(gp->all[before_pat + 1]->op) : "-", (before_pat + 2 < gp->n) ? bb_op_name(gp->all[before_pat + 2]->op) : "-",
            (before_pat + 3 < gp->n) ? bb_op_name(gp->all[before_pat + 3]->op) : "-");
    }
    if (getenv("SCRIP_RESUME_WHY"))
        fprintf(stderr, "[RTGRAPH] pat=%s pfenced=%d brt=%d rn=%d body_root_op=%d\n", dbgname ? dbgname : "?", pfenced, brt ? 1 : 0, rn ? 1 : 0, gp->body_root ? (int) gp->body_root->op : -1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void sno_pat_thunks_build(int p0) {
    int sv = g_lower.sno.in_patproc;
    g_lower.sno.in_patproc = 1;
    for (int pi2 = p0; pi2 < (int) g_lower.sno.pats.len; pi2++) {
        IR_graph_t * gp = IR_alloc(512);
        scx_t px;
        px.g = gp;
        px.wn_tree = NULL;
        px.loop_exit = NULL;
        px.loop_next = NULL;
        px.result_name = NULL;
        px.pat_fail = NULL;
        px.pat_seal = NULL;
        px.npre = 0;
        px.snap = 1;
        px.capro = NULL;
        px.capro_cand = NULL;
        px.ppos = 0;
        px.prog_nstmt = 0;
        px.stno_base = 0;
        IR_t * ok = lc_build(gp, IR_SUCCEED, NULL, NULL);
        IR_t * no = lc_build(gp, IR_FAIL, NULL, NULL);
        px.pat_fail = no;
        px.pat_seal = sno_thunk_abort_exit(gp, no);
        int before_pat = gp->n;
        IR_t * brt = NULL;
        int pfenced = 0;
        IR_t * pe = sno_pat_carrier_build(&px, CV_AT(g_lower.sno.pats, sno_pat_ent_t, pi2).pat, ok, no, &brt, &pfenced);
        {
            extern tree_t *ast_stmt_new(tree_e kind);
            IR_t * paft = pe;
            for (int api = 0; api < px.npre; api++) {
                if (px.pre[api].snapg) {
                    char vbuf[fmt_len("%s$V%d", CV_AT(g_lower.sno.pats, sno_pat_ent_t, pi2).name, api)];
                    snprintf(vbuf, sizeof vbuf, "%s$V%d", CV_AT(g_lower.sno.pats, sno_pat_ent_t, pi2).name, api);
                    char * vg = lp_strdup(vbuf);
                    sno_reg_var(vg);
                    IR_LIT(px.pre[api].prim).sval = vg;
                    if (px.pre[api].snapg == sno_elem_snapg && px.pre[api].prim2) IR_LIT(px.pre[api].prim2).sval = vg;
                    continue;
                }
                IR_t * co = lc_build(gp, px.pre[api].str ? IR_COERCE_STRING : IR_COERCE_INTEGER, paft, no);
                IR_LIT(co).ival = px.pre[api].codes;
                char abuf[fmt_len("%s$V%d", CV_AT(g_lower.sno.pats, sno_pat_ent_t, pi2).name, api)];
                snprintf(abuf, sizeof abuf, "%s$V%d", CV_AT(g_lower.sno.pats, sno_pat_ent_t, pi2).name, api);
                tree_t * tv = ast_stmt_new(TT_VAR);
                tv->v.sval = lp_strdup(abuf);
                IR_t * av = NULL;
                IR_t * ae = sx_lower(&px, tv, co, no, &av);
                ir_operand_push(co, av);
                ir_operand_push(px.pre[api].prim, co);
                paft = ae;
            }
            px.npre = 0;
            px.ppos = 0;
            pe = paft;
        }
        gp->entry = pe;
        gp->resumable_callable = 1;
        sno_match_traps_insert(gp, before_pat, px.pat_seal);
        sno_pat_publish_body_root(gp, before_pat, CV_AT(g_lower.sno.pats, sno_pat_ent_t, pi2).pat, brt, pfenced, CV_AT(g_lower.sno.pats, sno_pat_ent_t, pi2).name, px.pat_seal);
        int ppi = stage2_proc_grow(&g_stage2);
        g_stage2.proc_table[ppi].name = CV_AT(g_lower.sno.pats, sno_pat_ent_t, pi2).name;
        g_stage2.proc_table[ppi].proc = NULL;
        g_stage2.proc_table[ppi].entry_pc = -1;
        g_stage2.proc_table[ppi].nparams = 0;
        g_stage2.proc_table[ppi].lower_sc.n = 0;
        g_stage2.proc_table[ppi].is_generator = 0;
        g_stage2.proc_table[ppi].dyn_scope = 0;
        g_stage2.proc_table[ppi].thunk_kind = PROC_THUNK_PATTERN;
        g_stage2.proc_table[ppi].result_name = NULL;
        g_stage2.proc_table[ppi].bb_idx = bb_program_add(&g_stage2.bbp, gp);
    }
    g_lower.sno.in_patproc = sv;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * sno_pat_tree_graph_rt(const tree_t * pat) {
    IR_graph_t * gp = IR_alloc(512);
    scx_t px;
    px.g = gp;
    px.wn_tree = NULL;
    px.loop_exit = NULL;
    px.loop_next = NULL;
    px.result_name = NULL;
    px.pat_fail = NULL;
    px.pat_seal = NULL;
    px.npre = 0;
    px.snap = 0;
    px.capro = NULL;
    px.capro_cand = NULL;
    px.ppos = 0;
    px.prog_nstmt = 0;
    px.stno_base = 0;
    IR_t * ok = lc_build(gp, IR_SUCCEED, NULL, NULL);
    IR_t * no = lc_build(gp, IR_FAIL, NULL, NULL);
    px.pat_fail = no;
    px.pat_seal = sno_thunk_abort_exit(gp, no);
    int before_pat = gp->n;
    int rtc = sno_rtseq_resume();
    int rt_xmark = sno_expr_mark();
    IR_t * brt = NULL;
    int pfenced = 0;
    IR_t * pe = rtc ? sno_pat_carrier_build(&px, pat, ok, no, &brt, &pfenced) : sno_pat_node(&px, pat, ok, no);
    if (px.npre > 0) sno_fatal("runtime-operand primitive reached the RT recipe graph builder — recipes must bake literal args (B-RE contract)", NULL);
    if (sno_expr_mark() != rt_xmark) sno_fatal("deferred pattern-primitive argument reached the RT recipe graph builder — its expression thunk has no builder on this path (B-RE contract)", NULL);
    gp->entry = pe;
    gp->resumable_callable = 1;
    if (rtc) sno_pat_publish_body_root(gp, before_pat, pat, brt, pfenced, "RT$", px.pat_seal);
    else gp->body_root = (gp->n > before_pat && !sno_pat_right_sealed(pat)) ? gp->all[before_pat] : NULL;
    return gp;
}
