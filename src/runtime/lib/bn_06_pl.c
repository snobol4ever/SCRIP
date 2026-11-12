#include "runtime/lib/bn_head.h"
#include "runtime/by_name_dispatch.h"
#include "runtime/rt/rt_arena.h"
#include "runtime/rt/prolog_atom.h"
#include "runtime/core/runtime_shim.h"
#include "driver/driver_globals.h"
#include "runtime/pl_rat.h"
#include <ctype.h>
#include "runtime/rt_pl_reader.h"
#include "runtime/lib/bn_05_pl.h"
#include "runtime/lib/bn_06_pl.h"
#include "runtime/lib/bn_09_rk.h"
#include "runtime/lib/bn_11_pl.h"
#include "runtime/lib/bn_27_pl.h"
#include "runtime/lib/bn_30_pl.h"
#include "runtime/lib/bn_31_pl.h"
#include "runtime/lib/bn_34_pl.h"
extern void *rt_ws_alloc_descr(size_t);
int rt_pl_term_string_cell(void *, void *, int, pl_tr_ctx_t *);
extern int rt_pl_eng_by_id(int64_t, DESCR_t *);
extern int prolog_token_extent(const char *, int, int, int *, int *);
DESCR_t rt_pl_fresh_var_ref(uint64_t r14) { DESCR_t *j = (DESCR_t *)rt_ws_alloc_descr(1); pl_tr_ctx_t rcx = { 0 }; rcx.r14 = r14; pl_var_make(j, &rcx, 0); return pl_make_ref(j, 0); }
const char *pl_list_text_heap(DESCR_t v) {
    extern void *rt_wsb_alloc(size_t);
    extern void *rt_wsb_realloc(void *, size_t);
    size_t cap = 64, k = 0;
    char *buf;
    DESCR_t d = rt_pl_deref_val(v);
    if (!pl_is_nil(d)) { const char *as = pl_atom_str(d); if (as) return as; }
    buf = (char *)rt_wsb_alloc(cap);
    while (pl_is_cons(d)) {
        DESCR_t *kids = (DESCR_t *)d.p;
        DESCR_t h = rt_pl_deref_val(kids[0]);
        if (k + 5 > cap) { cap *= 2; buf = (char *)rt_wsb_realloc(buf, cap); }
        if (h.v == DT_I) {
            if (h.i < 0 || h.i > 0x10FFFF) return (const char *)0;
            k += (size_t)rt_pl_u8_put(buf + k, (int)h.i);
        } else {
            const char *hs = pl_atom_str(h);
            int adv = 0;
            if (!hs || !hs[0]) return (const char *)0;
            (void)rt_pl_u8_get(hs, &adv);
            if (hs[adv]) return (const char *)0;
            memcpy(buf + k, hs, (size_t)adv);
            k += (size_t)adv;
        }
        d = rt_pl_deref_val(kids[1]);
    }
    if (!pl_is_nil(d)) return (const char *)0;
    buf[k] = 0;
    return buf;
}
DESCR_t rt_pl_dop_pl_engine(DESCR_t *args, int nargs) {
    DESCR_t d, eh;
    if (nargs != 1) return FAILDESCR;
    d = rt_pl_deref_val(args[0]);
    if (!IS_STR_fn(d) || !d.s) return FAILDESCR;
    if (rt_pl_eng_by_id(strtoll(d.s, (char **)0, 10), &eh)) return eh;
    return pl_mk_atom("$dead_engine");
}
DESCR_t rt_pl_dop_big(DESCR_t *args, int nargs) {
    if (nargs != 1) return FAILDESCR;
    {
        extern DESCR_t rt_big_from_str(const char *);
        DESCR_t d = rt_pl_deref_val(args[0]);
        if (!IS_STR_fn(d) || !d.s) return FAILDESCR;
        if (strchr(d.s, 'r')) return pl_rat_parse(d.s);
        {
            const char *q = d.s + (*d.s == '-');
            extern DESCR_t rt_big_from_str_base(const char *, int);
            extern DESCR_t rt_big_neg(DESCR_t);
            if (q[0] == '0' && (q[1] == 'x' || q[1] == 'o' || q[1] == 'b')) {
                DESCR_t r = rt_big_from_str_base(q + 2, q[1] == 'x' ? 16 : q[1] == 'o' ? 8 : 2);
                return (IS_FAIL_fn(r) || q == d.s) ? r : rt_big_neg(r);
            }
        }
        return rt_big_from_str(d.s);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
PL_CX_LEAF_HEAD(get_edin, 1)
    {
        extern FILE *fh_cur_in_fp(void);
        int c;
        FILE *f = fh_cur_in_fp();
        do { c = fgetc(f); } while (c != EOF && (unsigned char)c <= ' ');
        ok = plw_unify_vals(args[0], INTVAL(c == EOF ? -1LL : (long long)c), cx);
    }
PL_CX_LEAF_TAIL
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
PL_CX_LEAF_HEAD(skip, 1)
    {
        extern FILE *fh_cur_in_fp(void);
        DESCR_t v = rt_pl_deref_val(args[0]);
        FILE *f = fh_cur_in_fp();
        int c;
        ok = 0;
        if (v.v == (DTYPE_t)DT_I) { do { c = fgetc(f); } while (c != EOF && c != (int)v.i); ok = 1; }
    }
PL_CX_LEAF_TAIL
static int pl_vt_occ(const tree_t *t) { int c = 0; if (!t) return 0; if (t->t == TT_VAR) c++; for (int i = 0; i < t->n; i++) c += pl_vt_occ(t->c[i]); return c; }
static unsigned pl_vt_hash(const char *s) { unsigned h = 2166136261u; for (; *s; s++) h = (h ^ (unsigned char)*s) * 16777619u; return h; }
static DESCR_t pl_tree_cell(const tree_t *t, pl_vtab_t *vt) {
    extern int prolog_atom_intern(const char *);
    extern DESCR_t rt_pl_fresh_var_ref(uint64_t);
    if (!t) return pl_mk_atom("?");
    switch (t->t) {
        case TT_ILIT:
        return INTVAL((long long)t->v.ival);
        case TT_FLIT:
        return REALVAL(t->v.dval);
        case TT_QLIT:
        case TT_NAME:
        return pl_mk_atom_dup(t->v.sval ? t->v.sval : "?", t->v.sval && t->slen > 0 ? (size_t)t->slen : strlen(t->v.sval ? t->v.sval : "?"));
        case TT_VAR:
        {
            const char *nm = t->v.sval;
            int anon = !nm || !strcmp(nm, "_");
            unsigned h = anon ? 0u : pl_vt_hash(nm) & vt->hmask;
            if (!anon) for (; vt->hs[h] >= 0; h = (h + 1) & vt->hmask) if (!strcmp(vt->nm[vt->hs[h]], nm)) { vt->cnt[vt->hs[h]]++; return vt->v[vt->hs[h]]; }
            if (vt->n >= vt->cap) { extern void rt_bomb(const char *msg); rt_bomb("pl_tree_cell: a term holds more variables than its tree counted"); }
            if (!anon) vt->hs[h] = vt->n;
            vt->nm[vt->n] = anon ? (const char *)0 : nm;
            vt->v[vt->n] = rt_pl_fresh_var_ref(0);
            vt->cnt[vt->n] = 1;
            return vt->v[vt->n++];
        }
        case TT_CUT:
        return pl_mk_atom("!");
        case TT_DQLIT:
        {
            extern int rt_pl_double_quotes_mode(void);
            const char *b = (t->n > 0 && t->c[0] && t->c[0]->v.sval) ? t->c[0]->v.sval : "";
            size_t bl = (t->n > 0 && t->c[0] && t->c[0]->v.sval && t->c[0]->slen > 0) ? (size_t)t->c[0]->slen : strlen(b);
            int m = rt_pl_double_quotes_mode();
            if (m == 3) return rt_pl_make_string_cell_n(b, bl);
            if (m == 0) return pl_mk_atom_dup(b, bl);
            {
                extern int rt_pl_u8_get(const char *s, int *adv);
                DESCR_t acc = pl_nil();
                for (size_t i = bl; i > 0;) {
                    size_t p = i - 1;
                    int a, cp;
                    while (p > 0 && i - p < 4 && ((unsigned char)b[p] & 0xC0) == 0x80) p--;
                    cp = rt_pl_u8_get(b + p, &a);
                    if ((size_t)a != i - p) { p = i - 1; cp = (unsigned char)b[p]; a = 1; }
                    acc = pl_cons(m == 2 ? INTVAL(cp) : pl_mk_atom_dup(b + p, (size_t)a), acc);
                    i = p;
                }
                return acc;
            }
        }
        case TT_MAKELIST:
        {
            int bar = (t->v.ival == 1 && t->n > 0), ne = bar ? t->n - 1 : t->n;
            DESCR_t *el = (DESCR_t *)rt_ws_alloc_descr((size_t)(ne > 0 ? ne : 1)), acc;
            for (int i = 0; i < ne; i++) el[i] = pl_tree_cell(t->c[i], vt);
            acc = bar ? pl_tree_cell(t->c[t->n - 1], vt) : pl_nil();
            for (int i = ne - 1; i >= 0; i--) acc = pl_cons(el[i], acc);
            return acc;
        }
        case TT_IDX:
        if (t->n == 2 && t->v.sval) {
            DESCR_t *kids = (DESCR_t *)rt_ws_alloc_descr(2), c = {0};
            kids[0] = pl_tree_cell(t->c[1], vt);
            kids[1] = pl_tree_cell(t->c[0], vt);
            c.v = (DTYPE_t)DT_PLREF;
            c.slen = (uint32_t)prolog_functor_intern(prolog_atom_intern(t->v.sval), 2);
            c.p = (void *)kids;
            return c;
        }
        return pl_mk_atom("?");
        case TT_FNC:
        case TT_UNIFY:
        case TT_IF:
        {
            const char *nm = t->t == TT_UNIFY ? "=" : t->t == TT_IF ? "->" : (t->v.sval ? t->v.sval : "?");
            if (t->n == 0 && (t->t != TT_FNC || !strcmp(nm, "{}"))) return pl_mk_atom_dup(nm, strlen(nm));
            if (t->n == 0) {
                DESCR_t *one = (DESCR_t *)rt_ws_alloc_descr(1), c = {0};
                one[0] = INTVAL(0);
                c.v = (DTYPE_t)DT_PLREF;
                c.slen = (uint32_t)prolog_functor_intern(prolog_atom_intern(nm), 0);
                c.p = (void *)one;
                return c;
            }
            if (t->t == TT_FNC && t->n == 1 && !strcmp(nm, "$pl_big") && t->c[0] && t->c[0]->t == TT_QLIT && t->c[0]->v.sval) { DESCR_t a = STRVAL(t->c[0]->v.sval); return rt_pl_dop_big(&a, 1); }
            if (t->t == TT_FNC && t->n == 1 && !strcmp(nm, "$pl_engine") && t->c[0] && t->c[0]->t == TT_ILIT) {
                DESCR_t eh;
                if (rt_pl_eng_by_id(t->c[0]->v.ival, &eh)) return eh;
                return pl_mk_atom_dup("$dead_engine", 12);
            }
            {
                DESCR_t *kids = (DESCR_t *)rt_ws_alloc_descr((size_t)t->n);
                for (int i = 0; i < t->n; i++) kids[i] = pl_tree_cell(t->c[i], vt);
                DESCR_t c = {0};
                c.v = (DTYPE_t)DT_PLREF;
                c.slen = (uint32_t)prolog_functor_intern(prolog_atom_intern(nm), t->n);
                c.p = (void *)kids;
                return c;
            }
        }
        default:
        return pl_mk_atom("?");
    }
}
static int pl_text_is_unbalanced(const char *s, const char **why) {
    size_t nopen = 1;
    *why = (const char *)0;
    for (const char *q = s; *q; q++) if (*q == '(' || *q == '[' || *q == '{') nopen++;
    char st[nopen];
    int sp = 0;
    size_t i;
    for (i = 0; s[i]; i++) {
        char c = s[i];
        if (c == '%') { while (s[i] && s[i] != '\n') i++; if (!s[i]) break; continue; }
        if (c == '/' && s[i + 1] == '*') { i += 2; while (s[i] && !(s[i] == '*' && s[i + 1] == '/')) i++; if (!s[i]) { *why = "end_of_file_in_block_comment"; return 1; } i++; continue; }
        if (c == '0' && s[i + 1] == '\'' && s[i + 2] && (i == 0 || !(isalnum((unsigned char)s[i - 1]) || s[i - 1] == '_'))) {
            i += 2;
            if (s[i] == '\\') {
                i++;
                if (!s[i]) { *why = "end_of_file"; return 1; }
                if (s[i] >= '0' && s[i] <= '7') {
                    while (s[i + 1] >= '0' && s[i + 1] <= '7') i++;
                    if (s[i + 1] == '\\') i++;
                } else if (s[i] == 'x' || s[i] == 'X') {
                    while (isxdigit((unsigned char)s[i + 1])) i++;
                    if (s[i + 1] == '\\') i++;
                }
            } else if (s[i] == '\'' && s[i + 1] == '\'') i++;
            continue;
        }
        if (c == '\'' && !s[i + 1] && i > 0 && isdigit((unsigned char)s[i - 1]) && (i == 1 || !(isalnum((unsigned char)s[i - 2]) || s[i - 2] == '_'))) { *why = "end_of_file"; return 1; }
        if (c == '\'' || c == '"' || c == '`') {
            char q = c;
            i++;
            for (;;) {
                if (!s[i]) { *why = q == '\'' ? "end_of_file_in_quoted\001'" : q == '"' ? "end_of_file_in_quoted\001\"" : "end_of_file_in_quoted\001`"; return 1; }
                if (s[i] == '\\' && s[i + 1] >= '0' && s[i + 1] <= '7') { size_t j = i + 1; while (s[j] >= '0' && s[j] <= '7') j++; if (s[j] == '\\') j++; i = j; continue; }
                if (s[i] == '\\' && (s[i + 1] == 'x' || s[i + 1] == 'X')) { size_t j = i + 2; while (isxdigit((unsigned char)s[j])) j++; if (s[j] == '\\') j++; i = j; continue; }
                if (s[i] == '\\' && s[i + 1]) { i += 2; continue; }
                if (s[i] == q) { if (s[i + 1] == q) { i += 2; continue; } break; }
                i++;
            }
            continue;
        }
        if (c == '(' || c == '[' || c == '{') { st[sp++] = c; continue; }
        if (c == ')' || c == ']' || c == '}') { char want = c == ')' ? '(' : c == ']' ? '[' : '{'; if (sp == 0 || st[sp - 1] != want) { *why = "cannot_start_term"; return 1; } sp--; continue; }
    }
    if (sp != 0) {
        size_t e = i;
        while (e > 0 && (s[e - 1] == ' ' || s[e - 1] == '\n' || s[e - 1] == '\t' || s[e - 1] == '\r')) e--;
        *why = e > 0 &&
            (s[e - 1] == ',' || s[e - 1] == '|' ||
            ((s[e - 1] == '(' || s[e - 1] == '[' || s[e - 1] == '{') &&
            !(e > 2 && (s[e - 2] == ' ' || s[e - 2] == '\t') &&
            (isalnum((unsigned char)s[e - 3]) || s[e - 3] == '_' || s[e - 3] == ')' || s[e - 3] == ']' || s[e - 3] == '}' || s[e - 3] == '\'' || s[e - 3] == '"')))) ? "end_of_clause" :
            "operator_expected";
    }
    return sp != 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_text_is_blank(const char *s) {
    for (;;) {
        s = pl_skip_layout(s);
        if (!*s) return 1;
        if (*s == '%') { while (*s && *s != '\n') s++; continue; }
        if (s[0] == '/' && s[1] == '*') { const char *c = strstr(s + 2, "*/"); if (!c) return 0; s = c + 2; continue; }
        return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_parse_term_text(const char *txt, DESCR_t *out, pl_vtab_t *vt, PlProgram **pg_out) {
    size_t L = strlen(txt);
    char *text;
    size_t e;
    vt->err = (const char *)0;
    vt->tr = (const tree_t *)0;
    vt->base = 0;
    vt->cm = (const int *)0;
    vt->ncm = 0;
    if (pl_text_is_blank(txt)) { vt->n = 0; if (pg_out) *pg_out = (PlProgram *)0; *out = pl_mk_atom("end_of_file"); return 1; }
    text = (char *)rt_wsb_alloc(L + 20);
    { extern int rt_pl_dialect_swi(void); const char *why; if (pl_text_is_unbalanced(txt, &why)) { if (why && rt_pl_dialect_swi()) vt->err = why; return 0; } }
    memcpy(text, "'$rd'((", 7);
    memcpy(text + 7, txt, L);
    e = 7 + L;
    while (e > 7 && (text[e - 1] == ' ' || text[e - 1] == '\n' || text[e - 1] == '\t' || text[e - 1] == '\r')) e--;
    if (e > 7 && text[e - 1] == '.') e--;
    text[e] = ')';
    text[e + 1] = ')';
    text[e + 2] = '.';
    text[e + 3] = '\n';
    text[e + 4] = 0;
    {
        extern PlProgram *prolog_parse_ex(const char *, const char *, int);
        PlProgram *pg = prolog_parse_ex(text, "user_input", 1);
        const tree_t *tr;
        if (pg && pg->head) prolog_fold_pieces(pg->head->tr);
        tr = (pg && pg->head) ? pg->head->tr : (const tree_t *)0;
        if (tr && tr->t == TT_CLAUSE && tr->n >= 1) tr = tr->c[0];
        if (pg_out) *pg_out = pg;
        if (!pg || pg->nerrors || !tr || tr->t != TT_FNC || tr->n != 1) { if (pg) vt->err = pg->synerr; return 0; }
        pl_vt_init(vt, pl_vt_occ(tr->c[0]));
        { extern const tree_t *pl_dict_place(const tree_t *); const tree_t *pv = pl_dict_place(tr->c[0]); *out = pl_tree_cell(tr->c[0], vt); if (pv != tr->c[0]) *out = pl_tree_cell(pv, vt); }
        vt->tr = tr->c[0];
        vt->src = text;
        vt->shift = 7;
        vt->cm = pg->cm;
        vt->ncm = pg->ncm;
        return 1;
    }
}
PL_CX_LEAF_HEAD(read, 1)
    {
        extern FILE *fh_cur_in_fp(void);
        char *text = (char *)0;
        size_t tn = 0;
        long off0;
        int got;
        DESCR_t t;
        pl_vtab_t vt;
        extern void *rt_pl_ball_kind1(const char *, const char *);
        if ((cx->ball = pl_text_cur_ball())) { ok = 0; rt_pl_tr_gc_sync(cx->tr); return FAILDESCR; }
        off0 = ftell(fh_cur_in_fp());
        got = pl_read_term_text(&text, &tn);
        if (got == 0) ok = plw_unify_vals(args[0], pl_mk_atom("end_of_file"), cx);
        else if (got != 1) {
            const char *g = got < 0 ? (const char *)0 : pl_gp_read_err(off0);
            cx->ball = rt_pl_ball_kind1("syntax_error", g ? g : got < 0 ? "term_too_long" : "end_of_file");
            ok = 0;
        } else if (!pl_parse_term_text(text, &t, &vt, (PlProgram **)0)) {
            const char *g = pl_gp_read_err(off0);
            cx->ball = rt_pl_ball_kind1("syntax_error", g ? g : PL_SYNERR(vt));
            ok = 0;
        } else ok = plw_unify_vals(args[0], t, cx);
    }
PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(atom_to_term, 3)
    {
        char b[65536];
        const char *txt;
        DESCR_t t;
        pl_vtab_t vt;
        if (pl_val_unbound(rt_pl_deref_val(args[0]))) {
            extern void *rt_pl_ball_instantiation(void);
            cx->ball = rt_pl_ball_instantiation();
            ok = 0;
        } else if (!pl_cell_text(args[0], b, sizeof b, &txt)) ok = 0;
        else if (!pl_parse_term_text(txt, &t, &vt, (PlProgram **)0)) {
            extern void *rt_pl_ball_kind1(const char *, const char *);
            cx->ball = rt_pl_ball_kind1("syntax_error", PL_SYNERR(vt));
            ok = 0;
        } else {
            DESCR_t el[vt.n > 0 ? vt.n : 1];
            int n = 0;
            extern int prolog_atom_intern(const char *);
            for (int i = 0; i < vt.n; i++) {
                if (!vt.nm[i]) continue;
                DESCR_t *kids = (DESCR_t *)rt_ws_alloc_descr(2);
                kids[0] = pl_mk_atom_dup(vt.nm[i], strlen(vt.nm[i]));
                kids[1] = vt.v[i];
                { DESCR_t c = {0}; c.v = (DTYPE_t)DT_PLREF; c.slen = (uint32_t)prolog_functor_intern(prolog_atom_intern("="), 2); c.p = (void *)kids; el[n++] = c; }
            }
            ok = plw_unify_vals(args[1], t, cx) && plw_unify_vals(args[2], pl_list_from_arr(el, n), cx);
        }
    }
PL_CX_LEAF_TAIL
static int pl_pos_off(const pl_vtab_t *vt, int off) { int c = 0; for (int i = 0; i < off && vt->src[i]; i++) if (((unsigned char)vt->src[i] & 0xC0) != 0x80) c++; return c - vt->shift + vt->base; }
static DESCR_t pl_pos_cmp(const char *nm, int ar, const DESCR_t *kids) {
    extern int prolog_atom_intern(const char *);
    DESCR_t *k = (DESCR_t *)rt_ws_alloc_descr(ar);
    DESCR_t c = {0};
    for (int i = 0; i < ar; i++) k[i] = kids[i];
    c.v = (DTYPE_t)DT_PLREF;
    c.slen = (uint32_t)prolog_functor_intern(prolog_atom_intern(nm), ar);
    c.p = (void *)k;
    return c;
}
static DESCR_t pl_pos_ft(const pl_vtab_t *vt, int a, int b) { DESCR_t k[2] = { INTVAL((int64_t)pl_pos_off(vt, a)), INTVAL((int64_t)pl_pos_off(vt, b)) }; return pl_pos_cmp("-", 2, k); }
static DESCR_t pl_pos_term(const pl_vtab_t *vt, const tree_t *n) {
    DESCR_t k[5];
    if (!n || n->end <= n->start) return pl_pos_ft(vt, 0, 0);
    int a = n->start, b = n->end;
    if (n->t == TT_DQLIT || (n->t == TT_MAKELIST && vt->src[a] == '`')) {
        k[0] = INTVAL((int64_t)pl_pos_off(vt, a));
        k[1] = INTVAL((int64_t)pl_pos_off(vt, b));
        return pl_pos_cmp("string_position", 2, k);
    }
    if (n->t == TT_MAKELIST && n->n > 0) {
        int nel = n->v.ival ? n->n - 1 : n->n;
        DESCR_t el[nel > 0 ? nel : 1];
        for (int i = 0; i < nel; i++) el[i] = pl_pos_term(vt, n->c[i]);
        k[0] = INTVAL((int64_t)pl_pos_off(vt, a));
        k[1] = INTVAL((int64_t)pl_pos_off(vt, b));
        k[2] = pl_list_from_arr(el, nel);
        k[3] = n->v.ival ? pl_pos_term(vt, n->c[n->n - 1]) : pl_mk_atom("none");
        return pl_pos_cmp("list_position", 4, k);
    }
    if (n->t == TT_FNC && n->v.sval && !strcmp(n->v.sval, "$dict_lit") && (n->n & 1)) {
        int np = (n->n - 1) / 2;
        DESCR_t kv[np > 0 ? np : 1];
        for (int i = 0; i < np; i++) {
            const tree_t *ky = n->c[1 + 2 * i], *vl = n->c[2 + 2 * i];
            int ss = ky->end, se = ky->end;
            if (!prolog_token_extent(vt->src, ky->end, 1, &ss, &se)) { ss = ky->end; se = ky->end; }
            DESCR_t q[7];
            q[0] = INTVAL((int64_t)pl_pos_off(vt, ky->start));
            q[1] = INTVAL((int64_t)pl_pos_off(vt, vl->end));
            q[2] = INTVAL((int64_t)pl_pos_off(vt, ss));
            q[3] = INTVAL((int64_t)pl_pos_off(vt, se));
            q[4] = pl_tree_cell(ky, (pl_vtab_t *)vt);
            q[5] = pl_pos_term(vt, ky);
            q[6] = pl_pos_term(vt, vl);
            kv[i] = pl_pos_cmp("key_value_position", 7, q);
        }
        k[0] = INTVAL((int64_t)pl_pos_off(vt, a));
        k[1] = INTVAL((int64_t)pl_pos_off(vt, b));
        k[2] = INTVAL((int64_t)pl_pos_off(vt, n->c[0]->start));
        k[3] = INTVAL((int64_t)pl_pos_off(vt, n->c[0]->end));
        k[4] = pl_list_from_arr(kv, np);
        return pl_pos_cmp("dict_position", 5, k);
    }
    if (n->t == TT_FNC && n->v.sval && (n->n >= 1 || strcmp(n->v.sval, "{}")) && strcmp(n->v.sval, "$pl_big") && strcmp(n->v.sval, "$dict_lit") && strcmp(n->v.sval, "$dot")) {
        if (!strcmp(n->v.sval, "{}") && n->n == 1) {
            k[0] = INTVAL((int64_t)pl_pos_off(vt, a));
            k[1] = INTVAL((int64_t)pl_pos_off(vt, b));
            k[2] = pl_pos_term(vt, n->c[0]);
            return pl_pos_cmp("brace_term_position", 3, k);
        }
        int fs = a, fe = a, ok;
        int infix = n->n == 2 && n->c[0] && n->c[0]->start == a, postfix = n->n == 1 && n->c[0] && n->c[0]->start == a;
        if (infix) ok = prolog_token_extent(vt->src, n->c[0]->end, 1, &fs, &fe);
        else if (postfix) ok = prolog_token_extent(vt->src, n->c[0]->end, 1, &fs, &fe);
        else ok = prolog_token_extent(vt->src, a, 0, &fs, &fe);
        if (!ok) { fs = a; fe = a; }
        DESCR_t av[n->n];
        for (int i = 0; i < n->n; i++) av[i] = pl_pos_term(vt, n->c[i]);
        k[0] = INTVAL((int64_t)pl_pos_off(vt, a));
        k[1] = INTVAL((int64_t)pl_pos_off(vt, b));
        k[2] = INTVAL((int64_t)pl_pos_off(vt, fs));
        k[3] = INTVAL((int64_t)pl_pos_off(vt, fe));
        k[4] = pl_list_from_arr(av, n->n);
        return pl_pos_cmp("term_position", 5, k);
    }
    return pl_pos_ft(vt, a, b);
}
static DESCR_t pl_pos_comments(const pl_vtab_t *vt) {
    int nc = vt->ncm / 2;
    DESCR_t el[nc > 0 ? nc : 1];
    int ne = 0;
    for (int i = 0; i < nc; i++) {
        int a = vt->cm[2 * i], b = vt->cm[2 * i + 1], line = 0, lstart = vt->shift;
        char tx[(b > a ? b - a : 0) + 1];
        DESCR_t sp[4], pr[2];
        for (int j = vt->shift; j < a; j++) if (vt->src[j] == '\n') { line++; lstart = j + 1; }
        if (a < vt->shift) continue;
        memcpy(tx, vt->src + a, (size_t)(b - a));
        tx[b - a] = 0;
        sp[0] = INTVAL((int64_t)pl_pos_off(vt, a));
        sp[1] = INTVAL((int64_t)line);
        sp[2] = INTVAL((int64_t)(pl_pos_off(vt, a) - pl_pos_off(vt, lstart)));
        sp[3] = INTVAL((int64_t)0);
        pr[0] = pl_pos_cmp("$stream_position", 4, sp);
        pr[1] = rt_pl_make_string_cell(tx);
        el[ne++] = pl_pos_cmp("-", 2, pr);
    }
    return pl_list_from_arr(el, ne);
}
int pl_read_term_options_cell(DESCR_t opts, pl_vtab_t *vt, pl_tr_ctx_t *cx) {
    extern int prolog_atom_intern(const char *);
    DESCR_t o = rt_pl_deref_val(opts);
    while (pl_is_cons(o)) {
        DESCR_t *kids = (DESCR_t *)o.p;
        DESCR_t opt = rt_pl_deref_val(kids[0]);
        if (opt.v == (DTYPE_t)DT_PLREF && plc_fid_arity(opt.slen) == 1) {
            DESCR_t *oa = (DESCR_t *)opt.p;
            int fid = plc_fid_name(opt.slen);
            int is_vars = fid == prolog_atom_intern("variables"), is_vn = fid == prolog_atom_intern("variable_names"), is_sing = fid == prolog_atom_intern("singletons");
            if (fid == prolog_atom_intern("subterm_positions") && vt->tr && !plw_unify_vals(oa[0], pl_pos_term(vt, vt->tr), cx)) return 0;
            if (fid == prolog_atom_intern("comments") && vt->tr && !plw_unify_vals(oa[0], pl_pos_comments(vt), cx)) return 0;
            if (is_vars || is_vn || is_sing) {
                gv_t el;
                int n = 0;
                el.p = 0;
                el.len = 0;
                el.cap = 0;
                el.esz = 0;
                el.kind = 0;
                el.pad = 0;
                for (int i = 0; i < vt->n; i++) {
                    if (is_vars) { *(DESCR_t *)gv_push(&el, (uint16_t)HB_DVEC, (uint32_t)sizeof(DESCR_t), "read_term_vars") = vt->v[i]; n++; continue; }
                    if (!vt->nm[i] || (is_sing && vt->cnt[i] != 1)) continue;
                    {
                        DESCR_t *k2 = (DESCR_t *)rt_ws_alloc_descr(2);
                        k2[0] = pl_mk_atom_dup(vt->nm[i], strlen(vt->nm[i]));
                        k2[1] = vt->v[i];
                        DESCR_t c = {0};
                        c.v = (DTYPE_t)DT_PLREF;
                        c.slen = (uint32_t)prolog_functor_intern(prolog_atom_intern("="), 2);
                        c.p = (void *)k2;
                        *(DESCR_t *)gv_push(&el, (uint16_t)HB_DVEC, (uint32_t)sizeof(DESCR_t), "read_term_vars") = c;
                        n++;
                    }
                }
                if (!plw_unify_vals(oa[0], pl_list_from_arr((DESCR_t *)el.p, n), cx)) return 0;
            }
        }
        o = rt_pl_deref_val(kids[1]);
    }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *pl_read_opts_ball(DESCR_t opts) {
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    extern void *rt_pl_ball_instantiation(void);
    extern const char *prolog_atom_name(int);
    static const char *const vk[] = { "variables", "variable_names", "singletons", 0 };
    static const char *const ok1[] = { "syntax_errors", "double_quotes", "backquoted_string", "term_position", "subterm_positions", "comments", "module", "cycles", "dotlists", "var_prefix",
        "quasi_quotations", "variable", 0 };
    DESCR_t o = rt_pl_deref_val(opts), cur, e, a;
    const char *fn;
    int i;
    for (cur = o; pl_is_cons(cur); cur = rt_pl_deref_val(((DESCR_t *)cur.p)[1])) if (pl_val_unbound(rt_pl_deref_val(((DESCR_t *)cur.p)[0]))) return rt_pl_ball_instantiation();
    if (pl_val_unbound(cur)) return rt_pl_ball_instantiation();
    if (!pl_is_nil(cur)) return rt_pl_ball_kind2("type_error", "list", o);
    for (cur = o; pl_is_cons(cur); cur = rt_pl_deref_val(((DESCR_t *)cur.p)[1])) {
        e = rt_pl_deref_val(((DESCR_t *)cur.p)[0]);
        fn = (e.v == (DTYPE_t)DT_PLREF && plc_fid_arity(e.slen) == 1) ? prolog_atom_name(plc_fid_name(e.slen)) : (const char *)0;
        for (i = 0; fn && vk[i]; i++) if (!strcmp(fn, vk[i])) break;
        if (fn && vk[i]) {
            for (a = rt_pl_deref_val(((DESCR_t *)e.p)[0]); pl_is_cons(a); a = rt_pl_deref_val(((DESCR_t *)a.p)[1])) ;
            if (!pl_val_unbound(a) && !pl_is_nil(a)) return rt_pl_ball_kind2("domain_error", "read_option", e);
            continue;
        }
        if (fn && !strcmp(fn, "syntax_error")) {
            const char *as;
            a = rt_pl_deref_val(((DESCR_t *)e.p)[0]);
            if (pl_val_unbound(a)) return rt_pl_ball_instantiation();
            as = pl_atom_str(a);
            if (!as || (strcmp(as, "error") && strcmp(as, "fail") && strcmp(as, "warning"))) return rt_pl_ball_kind2("domain_error", "read_option", e);
            continue;
        }
        for (i = 0; fn && ok1[i]; i++) if (!strcmp(fn, ok1[i])) break;
        if (!fn || !ok1[i]) return rt_pl_ball_kind2("domain_error", "read_option", e);
    }
    return (void *)0;
}
static int pl_read_term_pos_opt(DESCR_t opts, pl_tr_ctx_t *cx) {
    extern int fh_last_read_pos(long *, long *, long *, long *);
    extern int prolog_atom_intern(const char *);
    long ch = 0, ln = 0, lp = 0, by = 0;
    for (DESCR_t o = rt_pl_deref_val(opts); pl_is_cons(o); o = rt_pl_deref_val(((DESCR_t *)o.p)[1])) {
        DESCR_t e = rt_pl_deref_val(((DESCR_t *)o.p)[0]);
        if (e.v != (DTYPE_t)DT_PLREF || plc_fid_arity(e.slen) != 1 || plc_fid_name(e.slen) != prolog_atom_intern("term_position")) continue;
        if (!fh_last_read_pos(&ch, &ln, &lp, &by)) return 1;
        DESCR_t sp[4] = { INTVAL((int64_t)ch), INTVAL((int64_t)ln), INTVAL((int64_t)lp), INTVAL((int64_t)by) };
        if (!plw_unify_vals(((DESCR_t *)e.p)[0], pl_pos_cmp("$stream_position", 4, sp), cx)) return 0;
    }
    return 1;
}
PL_CX_LEAF_HEAD(read_term_opts, 2)
    {
        char *text = (char *)0;
        size_t tn = 0;
        int got, sm;
        const char *se;
        DESCR_t t;
        pl_vtab_t vt;
        void *ob = pl_read_opts_ball(args[1]);
        extern void *rt_pl_ball_kind1(const char *, const char *);
        extern FILE *fh_cur_in_fp(void);
        if (ob || (ob = pl_text_cur_ball())) { cx->ball = ob; ok = 0; rt_pl_tr_gc_sync(cx->tr); return FAILDESCR; }
        sm = pl_read_syntax_mode(args[1]);
        for (;;) {
            long off0 = ftell(fh_cur_in_fp());
            got = pl_read_term_text(&text, &tn);
            se = (const char *)0;
            if (got == 0) { vt.n = 0; vt.tr = (const tree_t *)0; vt.ncm = 0; ok = plw_unify_vals(args[0], pl_mk_atom("end_of_file"), cx) && pl_read_term_options_cell(args[1], &vt, cx); break; }
            if (got < 0) { cx->ball = rt_pl_ball_kind1("syntax_error", "term_too_long"); ok = 0; break; }
            if (got != 1) se = "end_of_file";
            else if (!pl_parse_term_text(text, &t, &vt, (PlProgram **)0)) se = PL_SYNERR(vt);
            if (!se) {
                extern int fh_last_read_pos(long *, long *, long *, long *);
                long ch = 0, ln = 0, lp = 0, by = 0;
                if (vt.tr && fh_last_read_pos(&ch, &ln, &lp, &by)) vt.base = (int)ch - pl_pos_off(&vt, vt.tr->start);
                ok = plw_unify_vals(args[0], t, cx) && pl_read_term_options_cell(args[1], &vt, cx) && pl_read_term_pos_opt(args[1], cx);
                break;
            }
            { const char *g = pl_gp_read_err(off0); if (g) se = g; }
            if (sm == 2) printf("warning: syntax error: %s\n", se);
            if (sm == 3) { fprintf(stderr, "warning: syntax error: %s\n", se); continue; }
            if (!sm) cx->ball = rt_pl_ball_kind1("syntax_error", se);
            ok = 0;
            break;
        }
    }
PL_CX_LEAF_TAIL
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
PL_CX_LEAF_HEAD(put_byte, 1)
    {
        extern FILE *fh_cur_out_fp(void);
        extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
        DESCR_t v = rt_pl_deref_val(args[0]);
        ok = 0;
        if (pl_val_unbound(v)) {
            extern void *rt_pl_ball_instantiation(void);
            cx->ball = rt_pl_ball_instantiation();
        } else if (v.v != (DTYPE_t)DT_I) cx->ball = rt_pl_ball_kind2("type_error", "byte", v);
        else if (v.i < 0 || v.i > 255) cx->ball = rt_pl_ball_kind2("type_error", "byte", v);
        else if ((cx->ball = pl_byte_cur_ball(1))) ok = 0;
        else { fputc((int)v.i, fh_cur_out_fp()); ok = 1; }
    }
PL_CX_LEAF_TAIL
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
PL_CX_LEAF_HEAD(put_edin, 1)
    {
        extern FILE *fh_cur_out_fp(void);
        extern int rt_pl_u8_put(char *, int);
        extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
        DESCR_t v = rt_pl_deref_val(args[0]);
        const char *s = (const char *)0;
        size_t n = 0;
        ok = 0;
        if (pl_val_unbound(v)) {
            extern void *rt_pl_ball_instantiation(void);
            cx->ball = rt_pl_ball_instantiation();
        } else if (v.v == (DTYPE_t)DT_I) {
            if (v.i >= 0 && v.i <= 0x10FFFF) {
                char cb[8];
                int bl = rt_pl_u8_put(cb, (int)v.i);
                fwrite(cb, 1, (size_t)bl, fh_cur_out_fp());
                ok = 1;
            } else if (pl_gnu_dialect()) {
                extern void *rt_pl_ball_kind1(const char *, const char *);
                cx->ball = rt_pl_ball_kind1("representation_error", "character_code");
            } else cx->ball = rt_pl_ball_kind2("type_error", "character", v);
        } else if (pl_gnu_dialect()) cx->ball = rt_pl_ball_kind2("type_error", "integer", v);
        else {
            s = pl_atom_str_n(v, &n);
            if (!s && pl_is_cons(v) && (s = pl_list_text_heap(v))) n = strlen(s);
            unsigned char c0 = s && n ? (unsigned char)s[0] : 0;
            size_t w = c0 < 0x80 ? 1 : c0 >= 0xF0 ? 4 : c0 >= 0xE0 ? 3 : c0 >= 0xC0 ? 2 : 0;
            if (s && n && n == w) { fwrite(s, 1, n, fh_cur_out_fp()); ok = 1; } else cx->ball = rt_pl_ball_kind2("type_error", "character", v);
        }
    }
PL_CX_LEAF_TAIL
static int pl_text_nul_quoted(const char *s, size_t n, const char **out) {
    char *o = (char *)rt_wsb_alloc(3 * n + 1);
    size_t k = 0;
    char q = 0;
    for (size_t i = 0; i < n; i++) {
        char c = s[i];
        if (!c) { if (!q && !(i >= 2 && s[i - 1] == '\'' && s[i - 2] == '0')) return 0; memcpy(o + k, "\\0\\", 3); k += 3; continue; }
        o[k++] = c;
        if (q) {
            if (c == '\\' && i + 1 < n && s[i + 1]) o[k++] = s[++i];
            else if (c == q) q = 0;
        } else if (c == '%') {
            while (i + 1 < n && s[i + 1] && s[i + 1] != '\n') o[k++] = s[++i];
        } else if (c == '/' && i + 1 < n && s[i + 1] == '*') {
            size_t j = ++i;
            o[k++] = s[i];
            while (i + 1 < n && s[i + 1] && !(s[i + 1] == '/' && s[i] == '*' && i > j)) o[k++] = s[++i];
        } else if (c == '"' || c == '`' || (c == '\'' && !(i >= 1 && s[i - 1] == '0' && (i < 2 || !(isalnum((unsigned char)s[i - 2]) || s[i - 2] == '_'))))) q = c;
    }
    o[k] = 0;
    *out = o;
    return 1;
}
static int pl_term_text_leaf(DESCR_t *args, int as_str, pl_tr_ctx_t *cx) {
    char b[65536];
    const char *txt;
    size_t tl = 0;
    DESCR_t t;
    pl_vtab_t vt;
    if (pl_cell_text_n(args[1], b, sizeof b, &txt, &tl)) {
        if (tl > strlen(txt) && !pl_text_nul_quoted(txt, tl, &txt)) {
            extern void *rt_pl_ball_kind1(const char *, const char *);
            cx->ball = rt_pl_ball_kind1("syntax_error", "illegal_character");
            return 0;
        }
        if (!pl_parse_term_text(txt, &t, &vt, (PlProgram **)0)) { extern void *rt_pl_ball_kind1(const char *, const char *); cx->ball = rt_pl_ball_kind1("syntax_error", PL_SYNERR(vt)); return 0; }
        return plw_unify_vals(args[0], t, cx);
    }
    return rt_pl_term_string_cell(&args[0], &args[1], as_str, cx);
}
PL_CX_LEAF_HEAD(term_string, 2) ok = pl_term_text_leaf(args, 1, cx); PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(term_to_atom, 2) ok = pl_term_text_leaf(args, 0, cx); PL_CX_LEAF_TAIL
