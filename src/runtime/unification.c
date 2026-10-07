#include <math.h>
#include <errno.h>
#include "rt/rt_arena.h"
#include "ct_arena.h"
#include "rt/rt.h"
#include "rt/gc_heap.h"
#include "core.h"
#include "core/utf8.h"
#include "bb_pool.h"
#include "rt/prolog_atom.h"
#include "../ir/IR.h"
#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "../parsers/prolog/pl_cell.h"
#include "rt/rt_pl_trail.h"
#include "rt/pl_rational.h"
void rt_pl_ball_report(void *ball);
#define PL_CELL_ALLOC(n) (rt_ws_alloc_descr((size_t)(n) / sizeof(pl_cell_t)))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_unify_into_cell(pl_cell_t *dst, pl_cell_t val);
extern int plw_unify_cell_val(DESCR_t *, DESCR_t, pl_tr_ctx_t *); extern int plw_unify_cells_x(DESCR_t *, DESCR_t *, pl_tr_ctx_t *); extern void plw_bind_x(DESCR_t *, DESCR_t, pl_tr_ctx_t *);
static int plc_unify_into_cell_cx(pl_cell_t *dst, pl_cell_t val, pl_tr_ctx_t *cx) { return cx ? plw_unify_cell_val(dst, val, cx) : plc_unify_into_cell(dst, val); }
static int plc_unify_cells_cx(pl_cell_t *a, pl_cell_t *b, pl_tr_ctx_t *cx) { return cx ? plw_unify_cells_x(a, b, cx) : pl_unify(a, b); }
static void plc_bind_cx(pl_cell_t *cell, pl_cell_t word, pl_tr_ctx_t *cx) { if (cx) plw_bind_x(pl_deref(cell), word, cx); else pl_bind(cell, word); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_unify_terms(void *l, void *r)
{
    pl_cell_t *a = (pl_cell_t *)l, *b = (pl_cell_t *)r;
    if (!a || !b) return 0;
    if (!pl_unify(a, b)) { return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_unify_const(int slot, int kind, long ival, const char *sval, double dval)
{
    (void)slot; (void)kind; (void)ival; (void)sval; (void)dval; return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_unify_var_var(int lslot, int rslot)
{
    (void)lslot; (void)rslot; return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_cells_init(void **cells, int n)
{
    char *base = (char *)cells;
    for (int i = 0; i < n; i++) pl_init_var((pl_cell_t *)(base + (size_t)16 * (size_t)i), i);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_gz_init(void *frame, int nslots)
{
    prolog_atom_init();
    char *base = (char *)frame;
    for (int i = 0; i < nslots; i++) pl_init_var((pl_cell_t *)(base + 8 + (size_t)16 * (size_t)i), i);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_compound_cell(const char *functor_name, int arity, void *arg_words)
{
    pl_cell_t *src = (pl_cell_t *)arg_words;
    pl_cell_t *blk = (pl_cell_t *)PL_CELL_ALLOC((size_t)(arity > 0 ? arity : 1) * sizeof(pl_cell_t));
    for (int i = 0; i < arity; i++) blk[i] = src[i];
    int fid = prolog_atom_intern(functor_name ? functor_name : "[]");
    pl_cell_t *out = (pl_cell_t *)PL_CELL_ALLOC(sizeof(pl_cell_t));
    *out = pl_make_compound(fid, arity, blk);
    return out;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static FILE *plc_out(void) { extern FILE *fh_cur_out_fp(void); FILE *f = fh_cur_out_fp(); return f ? f : stdout; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct plc_vmap plc_vmap;
#define PLC_WQ_NOESC 2
#define PLC_WQ_NONASCII 4
struct plc_vmap { pl_cell_t *seen[1024]; int n; FILE *fp; pl_cell_t *vnv[256]; const char *vnn[256]; int vnc; int (*portray)(pl_cell_t *, plc_vmap *); pl_tr_ctx_t *cx; void *pthrown; long pheld; };
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *plc_vname(plc_vmap *m, pl_cell_t *d)
{
    for (int i = m->vnc - 1; i >= 0; i--) if (m->vnv[i] == d) return m->vnn[i];
    return (const char *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_vindex(plc_vmap *m, pl_cell_t *d)
{
    for (int i = 0; i < m->n; i++) if (m->seen[i] == d) return i;
    if (m->n < 1024) { m->seen[m->n] = d; return m->n++; }
    return m->n - 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *plc_atom_text(pl_cell_t *d)
{
    if ((int)d->v == DT_S) return d->s ? d->s : "";
    const char *n = prolog_atom_name((int)d->i);
    return n ? n : "?";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_user_op(const char *fn, int ar, int *prec, int *ra)
{
    extern int prolog_op_user_count(void); extern int prolog_op_user_get(int, const char **, int *, const char **);
    int n = prolog_op_user_count();
    for (int i = 0; i < n; i++) {
        const char *nm = 0; const char *ty = 0; int pr = 0;
        if (!prolog_op_user_get(i, &nm, &pr, &ty) || !nm || !ty || strcmp(nm, fn)) continue;
        if (ar == 2 && (!strcmp(ty, "xfx") || !strcmp(ty, "xfy") || !strcmp(ty, "yfx"))) { *prec = pr; *ra = !strcmp(ty, "xfy"); return 1; }
        if (ar == 1 && (!strcmp(ty, "fy") || !strcmp(ty, "fx"))) { *prec = pr; *ra = 0; return 1; }
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define PLC_WC_IGNORE_OPS 1
static int plc_op_info_id(int nid, int arity, int *prec, int *lmax, int *rmax)
{
    extern int prolog_op_col_get(int, int, int *, int *); extern int prolog_op_col_user(int, int);
    int p = 0, t = 0;
    if (nid < 0) return 0;
    if (arity == 2) { if (!prolog_op_col_get(nid, 1, &p, &t)) return 0; }
    else if (arity == 1) {
        if (prolog_op_col_get(nid, 0, &p, &t) && prolog_op_col_user(nid, 0)) {}
        else if (prolog_op_col_get(nid, 2, &p, &t) && prolog_op_col_user(nid, 2)) {}
        else if (!prolog_op_col_get(nid, 0, &p, &t)) return 0; }
    else return 0;
    *prec = p;
    if (t == 1) { *lmax = p - 1; *rmax = p - 1; }
    else if (t == 2) { *lmax = p - 1; *rmax = p; }
    else if (t == 3) { *lmax = p; *rmax = p - 1; }
    else if (t == 4) { *lmax = 0; *rmax = p; }
    else if (t == 5) { *lmax = 0; *rmax = p - 1; }
    else if (t == 6) { *lmax = p; *rmax = 0; }
    else { *lmax = p - 1; *rmax = 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_op_info(const char *name, int arity, int *prec, int *lmax, int *rmax) { return name ? plc_op_info_id(prolog_atom_intern(name), arity, prec, lmax, rmax) : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_op_is_postfix_id(int nid)
{
    extern int prolog_op_col_get(int, int, int *, int *); extern int prolog_op_col_user(int, int); int p = 0, t = 0;
    if (nid < 0 || !prolog_op_col_get(nid, 2, &p, &t) || !prolog_op_col_user(nid, 2)) return 0;
    { int pp = 0, lm = 0, rm = 0; return !(plc_op_info_id(nid, 1, &pp, &lm, &rm) && rm > 0); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_op_is_postfix(const char *name) { return name ? plc_op_is_postfix_id(prolog_atom_intern(name)) : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_is_nil(pl_cell_t *d)
{
    extern int ATOM_NIL;
    if ((int)d->v == DT_PLATOM) return (int)d->i == ATOM_NIL;
    if ((int)d->v == DT_S) return d->s && strcmp(d->s, "[]") == 0;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_atom_needs_quoting(const char *name)
{
    static const char *const graphic = "#$&*+-./:<=>?@\\^~";
    if (!name || !name[0]) return 1;
    if (!strcmp(name, "[]") || !strcmp(name, "{}") || !strcmp(name, "!") || !strcmp(name, ";")) return 0;
    if (isupper((unsigned char)name[0]) || name[0] == '_') return 1;
    { extern int prolog_u_letter(const char *, int *); int adv, k = ((unsigned char)name[0] >= 0x80) ? prolog_u_letter(name, &adv) : 0;
      if (islower((unsigned char)name[0]) || k == 2) {
        for (const char *q = name + (k ? adv : 1); *q; ) { if (isalnum((unsigned char)*q) || *q == '_') { q++; continue; }
            if ((unsigned char)*q >= 0x80 && prolog_u_letter(q, &adv)) { q += adv; continue; } return 1; }
        return 0; }
      if (k == 1) return 1; }
    { int all_graphic = 1;
      for (const char *q = name; *q; q++) if (!strchr(graphic, *q)) { all_graphic = 0; break; }
      if (all_graphic) { if (!strcmp(name, ".")) return 1; if (name[0] == '/' && name[1] == '*') return 1; return 0; } }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_utf8_run(const char *q) { int n, k;
    if ((unsigned char)*q < 0x80) return 1;
    n = utf8_seqlen((unsigned char)*q);
    if (n < 2) return 1;
    for (k = 1; k < n; k++) if (((unsigned char)q[k] & 0xC0) != 0x80) return 1;
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_has_non_ascii(const char *s) { for (; *s; s++) if ((unsigned char)*s >= 0x80) return 1; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_wt_atom(FILE *fp, const char *name, int quoted)
{
    int noesc = quoted & PLC_WQ_NOESC, qna = quoted & PLC_WQ_NONASCII;
    quoted &= 1;
    if (!name) name = "?";
    if (quoted && (plc_atom_needs_quoting(name) || (qna && plc_has_non_ascii(name)))) {
        fputc('\'', fp);
        for (const char *q = name; *q; q++) {
            if (noesc && *q != '\'') { fputc(*q, fp); continue; }
            switch (*q) {
            case '\'': fputs("''", fp); break;
            case '\\': fputs("\\\\", fp); break;
            case '\n': fputs("\\n", fp); break;
            case '\t': fputs("\\t", fp); break;
            case '\r': fputs("\\r", fp); break;
            case '\a': fputs("\\a", fp); break;
            case '\b': fputs("\\b", fp); break;
            case '\f': fputs("\\f", fp); break;
            case '\v': fputs("\\v", fp); break;
            case '\0': fputs("\\0", fp); break;
            default: { int _sl = plc_utf8_run(q);
                if (_sl > 1) { for (int _k = 0; _k < _sl; _k++) fputc(q[_k], fp); q += _sl - 1; }
                else if (!isprint((unsigned char)*q)) fprintf(fp, "\\x%x\\", (unsigned)(unsigned char)*q);
                else fputc(*q, fp); break; } }
        }
        fputc('\'', fp);
    }
    else fprintf(fp, "%s", name);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_wt_string(FILE *fp, pl_cell_t *d, int quoted)
{
    const char *b = d->s ? d->s : ""; uint32_t n = d->s ? d->slen : 0;
    if (!(quoted & 1)) { fwrite(b, 1, n, fp); return; }
    fputc('"', fp);
    for (uint32_t i = 0; i < n; i++) {
        unsigned char c = (unsigned char)b[i];
        if ((quoted & PLC_WQ_NOESC) && c != '"') { fputc(c, fp); continue; }
        switch (c) {
        case '"': fputs("\\\"", fp); break;
        case '\\': fputs("\\\\", fp); break;
        case '\n': fputs("\\n", fp); break;
        case '\t': fputs("\\t", fp); break;
        case '\r': fputs("\\r", fp); break;
        case '\a': fputs("\\a", fp); break;
        case '\b': fputs("\\b", fp); break;
        case '\f': fputs("\\f", fp); break;
        case '\v': fputs("\\v", fp); break;
        case '\0': fputs("\\0\\", fp); break;
        default: if (c < 0x20 || c == 0x7f) fprintf(fp, "\\x%x\\", (unsigned)c); else fputc(c, fp); }
    }
    fputc('"', fp);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_wt_num(FILE *fp, pl_cell_t *d)
{
    double fv = d->r; char fb[64];
    for (int pr = 15; pr <= 17; pr++) { snprintf(fb, sizeof fb, "%.*g", pr, fv); if (strtod(fb, NULL) == fv) break; }
    if (!strpbrk(fb, ".eEnN")) { size_t n = strlen(fb); if (n+2 < sizeof fb) { fb[n]='.'; fb[n+1]='0'; fb[n+2]='\0'; } }
    else if (!strchr(fb, '.')) { char *e = strpbrk(fb, "eE"); char t[64];
        if (e && (size_t)(e - fb) + strlen(e) + 2 < sizeof t) { size_t k = (size_t)(e - fb); memcpy(t, fb, k); memcpy(t + k, ".0", 2); strcpy(t + k + 2, e); memcpy(fb, t, strlen(t) + 1); } }
    fputs(fb, fp);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_term_prio(pl_cell_t *c, int ignore_ops)
{
    extern int ATOM_DOT;
    pl_cell_t *d = c ? pl_deref(c) : (pl_cell_t *)0;
    int pr = 0, lm = 0, rm = 0, ar;
    if (!d || ignore_ops || (int)d->v != DT_PLREF) return 0;
    ar = plc_fid_arity(d->slen);
    if (plc_fid_name(d->slen) == ATOM_DOT && ar == 2) return 0;
    if (ar != 1 && ar != 2) return 0;
    { int nid = plc_fid_name(d->slen); const char *fn = prolog_atom_name(nid);
      if (fn && !strcmp(fn, "{}") && ar == 1) return 0;
      if (fn && plc_op_info_id(nid, ar, &pr, &lm, &rm)) return pr; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_is_graphic_char(int ch) { static const char *const g = "#$&*+-./:<=>?@\\^~"; return ch && strchr(g, ch) != (const char *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_is_op_atom(pl_cell_t *d) { int p = 0, l = 0, r = 0; int nid = (int)d->v == DT_PLATOM ? (int)d->i : (int)d->v == DT_S ? prolog_atom_intern(plc_atom_text(d)) : -1;
    return nid >= 0 && (plc_op_info_id(nid, 2, &p, &l, &r) || plc_op_info_id(nid, 1, &p, &l, &r)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_first_char(pl_cell_t *c, int quoted, int ignore_ops, int numbervars)
{
    extern int ATOM_DOT;
    pl_cell_t *d = c ? pl_deref(c) : (pl_cell_t *)0;
    int pr = 0, lm = 0, rm = 0;
    if (!d) return '[';
    if (pl_cell_unbound(d)) return '_';
    if ((int)d->v == DT_I) return d->i < 0 ? '-' : '0';
    if ((int)d->v == DT_R) return d->r < 0 ? '-' : '0';
    if ((int)d->v == DT_PLATOM || (int)d->v == DT_S) { const char *n = plc_atom_text(d);
        if (!n || !n[0]) return quoted ? '\'' : ' ';
        return (quoted && plc_atom_needs_quoting(n)) ? '\'' : (unsigned char)n[0]; }
    if ((int)d->v != DT_PLREF) return '_';
    { int ar = plc_fid_arity(d->slen); int nid = plc_fid_name(d->slen); const char *fn = prolog_atom_name(nid);
      pl_cell_t *aa = (pl_cell_t *)d->p;
      if (!fn) return 'a';
      if (numbervars && ar == 1 && !strcmp(fn, "$VAR")) { pl_cell_t *n = pl_deref(&aa[0]); if ((int)n->v == DT_I) return 'A';
          if ((int)n->v == DT_PLATOM || (int)n->v == DT_S) { const char *t = plc_atom_text(n); return t ? (unsigned char)*t : 0; } }
      if (!ignore_ops && plc_fid_name(d->slen) == ATOM_DOT && ar == 2) return '[';
      if (!ignore_ops && ar == 1 && !strcmp(fn, "{}")) return '{';
      if (!ignore_ops && (ar == 1 || ar == 2) && plc_op_info_id(nid, ar, &pr, &lm, &rm)) {
          if (pr > 1200) return (unsigned char)fn[0];
          if (ar == 2 && plc_is_op_atom(pl_deref(&aa[0]))) return '(';
          if (ar == 2 || plc_op_is_postfix_id(nid)) return plc_first_char(&aa[0], quoted, ignore_ops, numbervars);
          return (quoted && plc_atom_needs_quoting(fn)) ? '\'' : (unsigned char)fn[0]; }
      return (quoted && plc_atom_needs_quoting(fn)) ? '\'' : (unsigned char)fn[0]; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { long k; pl_cell_t *p; } plc_wsref;
static DESCR_t *plc_ws_vec(const pl_tr_ctx_t *cx) { return (cx && cx->tr) ? *(DESCR_t **)pl_tr_wslot_slot(pl_tr_base_of(cx->tr)) : (DESCR_t *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long plc_ws_mark(const pl_tr_ctx_t *cx) { DESCR_t *v; if (!cx || !cx->tr) return -1; v = plc_ws_vec(cx); return v ? (long)v[0].i : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_ws_end(const pl_tr_ctx_t *cx, long mark)
{
    DESCR_t *v = plc_ws_vec(cx); long n;
    if (mark < 0 || !v) return;
    n = (long)v[0].i;
    for (long i = mark + 1; i <= n; i++) memset(&v[i], 0, sizeof v[i]);
    v[0].i = mark;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long plc_ws_push(const pl_tr_ctx_t *cx, DESCR_t x)
{
    void **w = pl_tr_wslot_slot(pl_tr_base_of(cx->tr)); DESCR_t *v = (DESCR_t *)*w; long n = v ? (long)v[0].i : 0, cap = v ? (long)v[0].slen : 0;
    if (n + 1 >= cap) { long nc = cap ? 2 * cap : 32; DESCR_t *nv = (DESCR_t *)rt_ws_alloc_descr((size_t)nc);
        if (v) memcpy(nv, v, sizeof(DESCR_t) * (size_t)(n + 1));
        nv[0].v = (DTYPE_t)DT_I; nv[0].slen = (uint32_t)nc; nv[0].i = n; v = nv; *w = (void *)v; }
    v[n + 1] = x; v[0].i = n + 1;
    return n + 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_wsref_set(const pl_tr_ctx_t *cx, plc_wsref *r, pl_cell_t *c)
{
    extern int rt_gc_in_arena(const char *); DESCR_t x;
    r->p = c;
    if (!cx || !cx->tr) { r->k = -1; return; }
    memset(&x, 0, sizeof x);
    if (c && rt_gc_in_arena((const char *)c)) { x.v = (DTYPE_t)DT_PLVAR; x.p = (void *)c; }
    if (r->k < 0) r->k = plc_ws_push(cx, x); else plc_ws_vec(cx)[r->k] = x;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static pl_cell_t *plc_wsref_get(const pl_tr_ctx_t *cx, plc_wsref *r)
{
    if (r->k >= 0) { DESCR_t *v = plc_ws_vec(cx); if (v && (int)v[r->k].v == DT_PLVAR) r->p = (pl_cell_t *)v[r->k].p; }
    return r->p;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static pl_cell_t plc_atom_id_cell(int id);
static int plc_portray_hit(pl_cell_t *d, plc_vmap *m)
{
    extern DESCR_t rt_pl_goal_gen_h_c(DESCR_t goal, DESCR_t *argv, int n, void **hslot, void **ball, pl_tr_ctx_t *cx);
    extern int fh_alloc(FILE *); extern void fh_free(int); extern int fh_current_output(void); extern void fh_set_output(int);
    if (m->pthrown || !d) return 0;
    { pl_cell_t g = plc_atom_id_cell(prolog_atom_intern("portray")); DESCR_t a = *d; void *h = (void *)0; void *b = (void *)0;
      int sv = fh_current_output(); int slot = fh_alloc(m->fp); DESCR_t r;
      if (slot < 0) return 0;
      fflush(m->fp); fh_set_output(slot);
      r = rt_pl_goal_gen_h_c(g, &a, 1, &h, &b, m->cx);
      if (!b && IS_FAIL_fn(r)) { extern void *rt_pl_ball_take(void); b = rt_pl_ball_take(); }
      fflush(m->fp); fh_set_output(sv); fh_free(slot);
      if (h) { extern void rt_proc_drop_frame_h(void **hslot); m->pheld++; rt_proc_drop_frame_h(&h); }
      if (b) { m->pthrown = b; return 0; }
      return !IS_FAIL_fn(r); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_wt(pl_cell_t *c, int quoted, int ignore_ops, int numbervars, long max_depth, long depth, int maxp, plc_vmap *m);
#define PLC_KID(cl, dp, mp) do { if (wx) plc_wt((cl), quoted, ignore_ops, numbervars, max_depth, (dp), (mp), m); else plc_wt_lv((cl), quoted, ignore_ops, numbervars, max_depth, (dp), (mp), m); } while (0)
static void plc_wt_lv(pl_cell_t *c, int quoted, int ignore_ops, int numbervars, long max_depth, long depth, int maxp, plc_vmap *m)
{
    extern int ATOM_DOT;
    FILE *fp = m->fp; const pl_tr_ctx_t *wx = m->portray ? m->cx : (const pl_tr_ctx_t *)0; plc_wsref rd = { -1, (pl_cell_t *)0 };
    if (!c) { plc_wt_atom(fp, "[]", quoted); return; }
    pl_cell_t *d = pl_deref(c);
    if (wx) plc_wsref_set(wx, &rd, d);
    if (m->portray && m->portray(d, m)) return;
    if (wx) d = plc_wsref_get(wx, &rd);
    if (max_depth > 0 && depth >= max_depth) { fprintf(fp, "..."); return; }
    int tg = (int)d->v;
    if (pl_cell_unbound(d)) { { const char *vn = plc_vname(m, d); if (vn) fputs(vn, fp); else fprintf(fp, "_G%d", plc_vindex(m, d)); } return; }
    if (tg == DT_S) { plc_wt_string(fp, d, quoted); return; }
    if (tg == DT_PLATOM) { plc_wt_atom(fp, plc_atom_text(d), quoted); return; }
    if (tg == DT_I) { fprintf(fp, "%ld", (long)d->i); return; }
    if (tg == DT_R) { plc_wt_num(fp, d); return; }
    if (tg == DT_BIG) { extern char *rt_big_str(DESCR_t); char *bs = rt_big_str(*d); fputs(bs ? bs : "0", fp); return; }
    if (tg != DT_PLREF) { { const char *vn = plc_vname(m, d); if (vn) fputs(vn, fp); else fprintf(fp, "_G%d", plc_vindex(m, d)); } return; }
    int fnid = plc_fid_name(d->slen), ar = plc_fid_arity(d->slen);
    pl_cell_t *aa = (pl_cell_t *)d->p;
    const char *fn = prolog_atom_name(fnid);
    if (!fn) fn = "?";
    if (numbervars && strcmp(fn, "$VAR") == 0 && ar == 1) {
        pl_cell_t *n = pl_deref(&aa[0]);
        if ((int)n->v == DT_I) { long num = (long)n->i; int letter = (int)(num % 26); long suf = num / 26;
            if (suf == 0) fprintf(fp, "%c", 'A' + letter); else fprintf(fp, "%c%ld", 'A' + letter, suf); return; }
        if ((int)n->v == DT_PLATOM || (int)n->v == DT_S) { const char *vn = plc_atom_text(n); fputs(vn ? vn : "", fp); return; }
    }
    if (numbervars && strcmp(fn, "$VARNAME") == 0 && ar == 1) {
        pl_cell_t *n = pl_deref(&aa[0]);
        if ((int)n->v == DT_PLATOM || (int)n->v == DT_S) { const char *vn = plc_atom_text(n); fprintf(fp, "%s", vn ? vn : "_"); return; }
    }
    if (ignore_ops != 1 && fnid == ATOM_DOT && ar == 2) {
        extern void *rt_pl_ball_kind1(const char *, const char *);
        pl_cell_t *cur = d; long n = 0; pl_cell_t *tort = d; plc_wsref rc = { -1, (pl_cell_t *)0 }, rtt = { -1, (pl_cell_t *)0 };
        if (wx) { plc_wsref_set(wx, &rc, cur); plc_wsref_set(wx, &rtt, tort); }
        fputc('[', fp);
        for (;;) {
            pl_cell_t *ca = (pl_cell_t *)cur->p; pl_cell_t *tl;
            if (max_depth > 0 && n >= max_depth) { fprintf(fp, "|..."); break; }
            if (n) fputc(',', fp);
            PLC_KID(&ca[0], depth, 999);
            n++;
            if (wx) { cur = plc_wsref_get(wx, &rc); tort = plc_wsref_get(wx, &rtt); ca = (pl_cell_t *)cur->p; }
            tl = pl_deref(&ca[1]);
            if ((int)tl->v == DT_PLREF && plc_fid_name(tl->slen) == ATOM_DOT && plc_fid_arity(tl->slen) == 2 && tl->p) {
                cur = tl;
                if (cur == tort) { fprintf(fp, "|..."); if (!m->pthrown) m->pthrown = rt_pl_ball_kind1("resource_error", "cyclic_term"); break; }
                if ((n & (n - 1)) == 0) tort = cur;
                if (wx) { plc_wsref_set(wx, &rc, cur); plc_wsref_set(wx, &rtt, tort); }
                continue; }
            if (!plc_is_nil(tl)) { fputc('|', fp); PLC_KID(tl, depth, 999); }
            break;
        }
        fputc(']', fp); return;
    }
    if (!ignore_ops && ar == 1 && strcmp(fn, "{}") == 0) {
        fprintf(fp, "{"); PLC_KID(&aa[0], depth+1, 1200); fprintf(fp, "}"); return;
    }
    { int op_prec = 0, lmax = 0, rmax = 0;
      if (!ignore_ops && (ar == 1 || ar == 2) && plc_op_info(fn, ar, &op_prec, &lmax, &rmax)) {
        int wrap = op_prec > maxp;
        if (wrap) fputc('(', fp);
        if (ar == 2) {
            int lop = plc_is_op_atom(pl_deref(&aa[0])), rop = plc_is_op_atom(pl_deref(&aa[1]));
            if (lop) fputc('(', fp);
            PLC_KID(&aa[0], depth+1, lop ? 1200 : lmax);
            if (wx) { d = plc_wsref_get(wx, &rd); aa = (pl_cell_t *)d->p; }
            if (lop) fputc(')', fp);
            if (isalnum((unsigned char)fn[0]) || fn[0] == '_') fprintf(fp, " %s ", fn);
            else if (!strcmp(fn, ",") || !strcmp(fn, "|")) fputc(fn[0], fp);
            else { plc_wt_atom(fp, fn, quoted); if (!rop && plc_is_graphic_char(plc_first_char(&aa[1], quoted, ignore_ops, numbervars))) fputc(' ', fp); }
            if (rop) fputc('(', fp);
            PLC_KID(&aa[1], depth+1, rop ? 1200 : rmax);
            if (rop) fputc(')', fp);
        } else if (plc_op_is_postfix(fn)) {
            PLC_KID(&aa[0], depth+1, lmax);
            if (isalnum((unsigned char)fn[0]) || fn[0] == '_') fprintf(fp, " %s", fn); else plc_wt_atom(fp, fn, quoted);
        } else {
            { pl_cell_t *a0 = pl_deref(&aa[0]); int ap = plc_term_prio(a0, ignore_ops);
              int alnum_op = isalnum((unsigned char)fn[0]) || fn[0] == '_';
              int a_num = ((int)a0->v == DT_I && a0->i >= 0) || ((int)a0->v == DT_R && a0->r >= 0);
              int a_opatom = plc_is_op_atom(a0);
              int needp = (ap > rmax) || a_opatom || (a_num && !strcmp(fn, "-"));
              int sep = needp || (!alnum_op && plc_is_graphic_char(plc_first_char(&aa[0], quoted, ignore_ops, numbervars)));
              if (alnum_op) fprintf(fp, "%s ", fn); else plc_wt_atom(fp, fn, quoted);
              if (sep && !alnum_op) fputc(' ', fp);
              if (needp) fputc('(', fp);
              PLC_KID(&aa[0], depth+1, needp ? 1200 : rmax);
              if (needp) fputc(')', fp); }
        }
        if (wrap) fputc(')', fp);
        return; } }
    plc_wt_atom(fp, fn, quoted || (fn[0] == '.' && fn[1] == 0));
    fprintf(fp, "(");
    for (int i = 0; i < ar; i++) { if (i) fprintf(fp, ","); PLC_KID(&aa[i], depth+1, 999); if (wx) { d = plc_wsref_get(wx, &rd); aa = (pl_cell_t *)d->p; } }
    fprintf(fp, ")");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#undef PLC_KID
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { void *blk; pl_cell_t *nb; int done; } plc_fz_ent_t;
typedef struct { plr_stk_t ag, lg; pl_cell_t *s; pl_cell_t *d; long next_k; int named; pl_cell_t knodes; plc_fz_ent_t *tab; long mask; } plc_fz_t;
static pl_cell_t plc_fz_var(plc_fz_t *z, long k)
{
    if (z->named) { char nb[32]; pl_cell_t *a = (pl_cell_t *)rt_ws_alloc_descr(1); snprintf(nb, sizeof nb, "S_%ld", k); *a = pl_make_atom(prolog_atom_intern(nb));
        return pl_make_compound(prolog_atom_intern("$VAR"), 1, a); }
    { pl_cell_t *v = (pl_cell_t *)rt_ws_alloc_descr(1); pl_init_var(v, -1); return pl_make_ref(v, (int)v->slen); }
}
static plc_fz_ent_t *plc_fz_slot(plc_fz_t *z, void *blk)
{
    long i = (long)(((uintptr_t)blk >> 4) * 0x9E3779B97F4A7C15ull >> 20) & z->mask;
    while (z->tab[i].blk && z->tab[i].blk != blk) i = (i + 1) & z->mask;
    return &z->tab[i];
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_fz_run(void *wv)
{
    plc_fz_t *w = (plc_fz_t *)wv;
    for (;;) {
        pl_cell_t *d = plr_resolve(pl_deref(w->s));
        int ar = (int)d->v == DT_PLREF ? plc_fid_arity(d->slen) : 0; plc_fz_ent_t *e = ar > 0 ? plc_fz_slot(w, d->p) : (plc_fz_ent_t *)0;
        if (e && e->blk) {
            pl_cell_t *nb = e->nb;
            if (!e->done && nb[ar].v == DT_SNUL) {
                pl_cell_t *kc = (pl_cell_t *)rt_ws_alloc_descr(2);
                nb[ar] = plc_fz_var(w, ++w->next_k); kc[0] = pl_make_compound(plc_fid_name(d->slen), ar, nb); kc[1] = w->knodes; w->knodes = pl_make_compound(ATOM_DOT, 2, kc);
            }
            *w->d = nb[ar].v != DT_SNUL ? nb[ar] : pl_make_compound(plc_fid_name(d->slen), ar, nb);
        } else if (e) {
            pl_cell_t *nb;
            if (!plr_room(&w->ag)) return plr_more(&w->ag, &w->lg, plc_fz_run, w);
            nb = (pl_cell_t *)rt_ws_alloc_descr((size_t)ar + 3);
            memset(&nb[ar], 0, sizeof nb[ar]); nb[ar + 1] = pl_make_int((int64_t)(intptr_t)w->d); nb[ar + 2] = pl_make_int((int64_t)(e - w->tab));
            e->blk = d->p; e->nb = nb; e->done = 0;
            *w->d = pl_make_compound(plc_fid_name(d->slen), ar, nb);
            plr_push(&w->ag, d->p, nb, (uint64_t)ar);
        } else *w->d = pl_cell_unbound(d) ? pl_make_ref(d, (int)d->slen) : *d;
        for (;;) {
            plr_ent_t *t;
            if (plr_empty(&w->ag)) return 0;
            t = plr_peek(&w->ag);
            if (!t->n) { w->tab[((pl_cell_t *)t->y)[2].i].done = 1; plr_drop(&w->ag); continue; }
            w->s = (pl_cell_t *)t->x; w->d = (pl_cell_t *)t->y; t->x = (void *)(w->s + 1); t->y = (void *)(w->d + 1); t->n--;
            break;
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_fz_count_visit(pl_cell_t *d, void *ctx) { if ((int)d->v == DT_PLREF) ++*(long *)ctx; return PLR_V_GO; }
PLR_WALK1(plc_fz_count_walk, plc_fz_count_visit, pl_deref)
static pl_cell_t plc_factorize(pl_cell_t *c, int named)
{
    long n = 0, cap = 64; pl_cell_t out, subs = pl_make_atom(ATOM_NIL), *args = (pl_cell_t *)rt_ws_alloc_descr(2); plc_fz_t w;
    (void)plc_fz_count_walk(c, (void *)&n);
    while (cap < 2 * n + 2) cap <<= 1;
    { plc_fz_ent_t tab[cap];
      memset(tab, 0, sizeof tab); memset(&out, 0, sizeof out); plr_stk_init(&w.ag); plr_stk_init(&w.lg);
      w.s = c; w.d = &out; w.next_k = 0; w.named = named; w.knodes = pl_make_atom(ATOM_NIL); w.tab = tab; w.mask = cap - 1;
      (void)plc_fz_run(&w); }
    for (pl_cell_t kl = w.knodes; (int)kl.v == DT_PLREF; kl = ((pl_cell_t *)kl.p)[1]) {
        pl_cell_t node = ((pl_cell_t *)kl.p)[0], *nb = (pl_cell_t *)node.p, *eq = (pl_cell_t *)rt_ws_alloc_descr(2), *cons = (pl_cell_t *)rt_ws_alloc_descr(2);
        int ar = plc_fid_arity(node.slen);
        *(pl_cell_t *)(intptr_t)nb[ar + 1].i = nb[ar];
        eq[0] = nb[ar]; eq[1] = node; cons[0] = pl_make_compound(prolog_atom_intern("="), 2, eq); cons[1] = subs; subs = pl_make_compound(ATOM_DOT, 2, cons);
    }
    args[0] = out; args[1] = subs;
    return pl_make_compound(prolog_atom_intern("@"), 2, args);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_acyclic_walk(pl_cell_t *c);
static void plc_wt(pl_cell_t *c, int quoted, int ignore_ops, int numbervars, long max_depth, long depth, int maxp, plc_vmap *m)
{
    const pl_tr_ctx_t *wx = m->portray ? m->cx : (const pl_tr_ctx_t *)0; long mark = plc_ws_mark(wx);
    if (!depth && max_depth <= 0 && c && !pl_acyclic_walk(c)) {
        pl_cell_t *f = (pl_cell_t *)rt_ws_alloc_descr(1); plc_wsref rf = { -1, (pl_cell_t *)0 };
        *f = plc_factorize(c, numbervars);
        if (wx) plc_wsref_set(wx, &rf, f);
        fputs("@(", m->fp); plc_wt_lv(&((pl_cell_t *)f->p)[0], quoted, ignore_ops, numbervars, 0, 1, 999, m);
        if (wx) f = plc_wsref_get(wx, &rf);
        fputc(',', m->fp); plc_wt_lv(&((pl_cell_t *)f->p)[1], quoted, ignore_ops, numbervars, 0, 1, 999, m); fputc(')', m->fp);
        plc_ws_end(wx, mark); return;
    }
    plc_wt_lv(c, quoted, ignore_ops, numbervars, max_depth, depth, maxp, m);
    plc_ws_end(wx, mark);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_write(pl_cell_t *c, plc_vmap *m) { plc_wt(c, 0, 0, 1, 0, 0, 1200, m); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_writeq(pl_cell_t *c, plc_vmap *m) { plc_wt(c, 1, 0, 1, 0, 0, 1200, m); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_write_canonical(pl_cell_t *c, plc_vmap *m) { plc_wt(c, 1, PLC_WC_IGNORE_OPS, 0, 0, 0, 1200, m); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_atoms_ready(void) { extern int ATOM_DOT; extern void prolog_atom_init(void); if (ATOM_DOT < 0) prolog_atom_init(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_write_cell(void *cell)
{
    plc_atoms_ready();
    plc_vmap m; m.n = 0; m.vnc = 0; m.fp = plc_out(); m.portray = (int (*)(pl_cell_t *, plc_vmap *))0; m.pthrown = (void *)0; m.cx = (pl_tr_ctx_t *)0; m.pheld = 0;
    plc_write((pl_cell_t *)cell, &m);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_write_cell_fp(void *cell, FILE *fp)
{
    plc_atoms_ready();
    plc_vmap m; m.n = 0; m.vnc = 0; m.fp = fp ? fp : plc_out(); m.portray = (int (*)(pl_cell_t *, plc_vmap *))0; m.pthrown = (void *)0; m.cx = (pl_tr_ctx_t *)0; m.pheld = 0;
    plc_write((pl_cell_t *)cell, &m);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_writeq_cell(void *cell)
{
    plc_atoms_ready();
    plc_vmap m; m.n = 0; m.vnc = 0; m.fp = plc_out(); m.portray = (int (*)(pl_cell_t *, plc_vmap *))0; m.pthrown = (void *)0; m.cx = (pl_tr_ctx_t *)0; m.pheld = 0;
    plc_writeq((pl_cell_t *)cell, &m);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_write_canonical_cell(void *cell)
{
    plc_atoms_ready();
    plc_vmap m; m.n = 0; m.vnc = 0; m.fp = plc_out(); m.portray = (int (*)(pl_cell_t *, plc_vmap *))0; m.pthrown = (void *)0; m.cx = (pl_tr_ctx_t *)0; m.pheld = 0;
    plc_write_canonical((pl_cell_t *)cell, &m);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_display_cell_ball(void *cell)
{
    plc_atoms_ready();
    plc_vmap m; m.n = 0; m.vnc = 0; m.fp = plc_out(); m.portray = (int (*)(pl_cell_t *, plc_vmap *))0; m.pthrown = (void *)0; m.cx = (pl_tr_ctx_t *)0; m.pheld = 0;
    plc_wt((pl_cell_t *)cell, 0, 1, 0, 0, 0, 1200, &m);
    return m.pthrown;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_display_cell(void *cell) { rt_pl_display_cell_ball(cell); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_opt_is_true(pl_cell_t *o)
{
    if (!o) return 0;
    o = pl_deref(o);
    if ((int)o->v == DT_PLREF && plc_fid_arity(o->slen) == 1) {
        pl_cell_t *a = pl_deref((pl_cell_t *)o->p);
        return ((int)a->v == DT_PLATOM || (int)a->v == DT_S) && strcmp(plc_atom_text(a), "true") == 0;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_write_term_cell(void *term_cell, void *opts_cell)
{
    plc_atoms_ready();
    plc_vmap m; m.n = 0; m.vnc = 0; m.fp = plc_out(); m.portray = (int (*)(pl_cell_t *, plc_vmap *))0; m.pthrown = (void *)0; m.cx = (pl_tr_ctx_t *)0; m.pheld = 0;
    int quoted = 0, ignore_ops = 0, numbervars = 0, prio = 1200, noesc = 0, qna = 0; long max_depth = 0;
    pl_cell_t *lst = opts_cell ? pl_deref((pl_cell_t *)opts_cell) : (pl_cell_t *)0;
    while (lst && (int)lst->v == DT_PLREF && plc_fid_arity(lst->slen) == 2) {
        pl_cell_t *pair = (pl_cell_t *)lst->p;
        pl_cell_t *o = pl_deref(&pair[0]);
        if ((int)o->v == DT_PLREF && plc_fid_arity(o->slen) == 1) {
            const char *on = prolog_atom_name(plc_fid_name(o->slen));
            pl_cell_t *oa = (pl_cell_t *)o->p;
            if (on && !strcmp(on, "quoted")) quoted = plc_opt_is_true(o);
            else if (on && !strcmp(on, "character_escapes")) noesc = !plc_opt_is_true(o);
            else if (on && !strcmp(on, "quote_non_ascii")) qna = plc_opt_is_true(o);
            else if (on && !strcmp(on, "priority")) { pl_cell_t *a = pl_deref(&oa[0]); if ((int)a->v == DT_I && a->i >= 0 && a->i <= 1200) prio = (int)a->i; }
            else if (on && !strcmp(on, "ignore_ops")) ignore_ops = plc_opt_is_true(o);
            else if (on && !strcmp(on, "numbervars")) numbervars = plc_opt_is_true(o);
            else if (on && !strcmp(on, "max_depth")) { pl_cell_t *a = pl_deref(&oa[0]); if ((int)a->v == DT_I) max_depth = (long)a->i; }
            else if (on && !strcmp(on, "variable_names")) { pl_cell_t *vl = pl_deref(&oa[0]); int base = m.vnc;
                while ((int)vl->v == DT_PLREF && plc_fid_arity(vl->slen) == 2 && vl->p) { pl_cell_t *vp = (pl_cell_t *)vl->p; pl_cell_t *e = pl_deref(&vp[0]);
                    if ((int)e->v == DT_PLREF && plc_fid_arity(e->slen) == 2 && e->p) { pl_cell_t *ea = (pl_cell_t *)e->p;
                        pl_cell_t *nm = pl_deref(&ea[0]); pl_cell_t *vr = pl_deref(&ea[1]); int dup = 0;
                        for (int k = base; k < m.vnc; k++) if (m.vnv[k] == vr) { dup = 1; break; }
                        if (!dup && pl_cell_unbound(vr) && ((int)nm->v == DT_PLATOM || (int)nm->v == DT_S) && m.vnc < 256) { m.vnv[m.vnc] = vr; m.vnn[m.vnc] = plc_atom_text(nm); m.vnc++; } }
                    vl = pl_deref(&vp[1]); } }
        }
        lst = pl_deref(&pair[1]);
    }
    plc_wt((pl_cell_t *)term_cell, quoted ? (1 | (noesc ? PLC_WQ_NOESC : 0) | (qna ? PLC_WQ_NONASCII : 0)) : 0, ignore_ops, numbervars, max_depth, 0, prio, &m);
}
extern const char *prolog_atom_name(int id);
extern int    rt_last_ok(void);
extern long   rt_arith(int lk, long li, const char *ls, int rk, long ri, const char *rs, const char *op);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_atom_id_of(pl_cell_t *d)
{
    if ((int)d->v == DT_PLATOM) return (int)d->i;
    if ((int)d->v == DT_S) { extern int prolog_atom_intern(const char *); return prolog_atom_intern(d->s ? d->s : ""); }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_is_atomlike(pl_cell_t *d) { return (int)d->v == DT_PLATOM || (int)d->v == DT_S; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static pl_cell_t plc_atom_id_cell(int id) { return pl_make_atom(id); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_ground_visit(pl_cell_t *d, void *ctx)
{
    (void)ctx;
    if (pl_cell_unbound(d)) return PLR_V_STOP;
    return ((int)d->v == DT_PLREF && plc_fid_arity(d->slen) > 0 && !d->p) ? PLR_V_STOP : PLR_V_GO;
}
PLR_WALK1(plc_ground_walk, plc_ground_visit, pl_deref)
static int plc_cell_is_ground(pl_cell_t *c) { return c && !plc_ground_walk(c, (void *)0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_cell_is_proper_list(pl_cell_t *c)
{
    extern int ATOM_DOT, ATOM_NIL;
    pl_cell_t *d = c ? pl_deref(c) : (pl_cell_t *)0; pl_cell_t *tort = d; unsigned long n = 0;
    while (d) {
        if (plc_is_atomlike(d) && plc_atom_id_of(d) == ATOM_NIL) return 1;
        if ((int)d->v == DT_PLREF && plc_fid_name(d->slen) == ATOM_DOT && plc_fid_arity(d->slen) == 2 && d->p) {
            d = pl_deref(&((pl_cell_t *)d->p)[1]); n++;
            if (d == tort) return 0;
            if ((n & (n - 1)) == 0) tort = d;
            continue; }
        return 0;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_type_test_cell(void *cell_term, const char *fn)
{
    pl_cell_t *d = cell_term ? pl_deref((pl_cell_t *)cell_term) : (pl_cell_t *)0;
    int isvar = (!d || pl_cell_unbound(d));
    int isatom = (d && !isvar && (int)d->v == DT_PLATOM);
    int isstr = (d && !isvar && (int)d->v == DT_S);
    int isint = (d && !isvar && ((int)d->v == DT_I || (int)d->v == DT_BIG));
    int isfloat = (d && !isvar && (int)d->v == DT_R);
    int iscomp = (d && !isvar && (int)d->v == DT_PLREF);
    if (!fn) return 0;
    if (strcmp(fn, "var")      == 0) return  isvar ? 1 : 0;
    if (strcmp(fn, "nonvar")   == 0) return !isvar ? 1 : 0;
    if (strcmp(fn, "atom")     == 0) return isatom ? 1 : 0;
    if (strcmp(fn, "integer")  == 0) return isint ? 1 : 0;
    if (strcmp(fn, "float")    == 0) return isfloat ? 1 : 0;
    if (strcmp(fn, "number")   == 0) return (isint || isfloat) ? 1 : 0;
    if (strcmp(fn, "atomic")   == 0) return (isatom || isstr || isint || isfloat) ? 1 : 0;
    if (strcmp(fn, "string")   == 0) return isstr ? 1 : 0;
    if (strcmp(fn, "compound") == 0) return iscomp ? 1 : 0;
    if (strcmp(fn, "callable") == 0) return (isatom || iscomp) ? 1 : 0;
    if (strcmp(fn, "ground")   == 0) return plc_cell_is_ground((pl_cell_t *)cell_term) ? 1 : 0;
    if (strcmp(fn, "is_list")  == 0) return plc_cell_is_proper_list((pl_cell_t *)cell_term) ? 1 : 0;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_functor_cell(void *t0_cell, void *name_cell, void *arity_cell, pl_tr_ctx_t *cx)
{
    pl_cell_t *d0 = t0_cell ? pl_deref((pl_cell_t *)t0_cell) : (pl_cell_t *)0;
    pl_cell_t *d1 = name_cell ? pl_deref((pl_cell_t *)name_cell) : (pl_cell_t *)0;
    if (d0 && !pl_cell_unbound(d0)) {
        pl_cell_t nameV, arityV;
        if ((int)d0->v == DT_PLREF)   { nameV = plc_atom_id_cell(plc_fid_name(d0->slen)); arityV = pl_make_int((int64_t)plc_fid_arity(d0->slen)); }
        else if ((int)d0->v == DT_S)  { nameV = *d0; arityV = pl_make_int(0); }
        else if (plc_is_atomlike(d0)) { nameV = plc_atom_id_cell(plc_atom_id_of(d0)); arityV = pl_make_int(0); }
        else if ((int)d0->v == DT_I)  { nameV = pl_make_int(d0->i); arityV = pl_make_int(0); }
        else if ((int)d0->v == DT_R)  { nameV = pl_make_float(d0->r); arityV = pl_make_int(0); }
        else { return 0; }
        if (!plc_unify_cells_cx((pl_cell_t *)name_cell, &nameV, cx) ||
            !plc_unify_cells_cx((pl_cell_t *)arity_cell, &arityV, cx)) { return 0; }
        return 1;
    }
    pl_cell_t *d2 = arity_cell ? pl_deref((pl_cell_t *)arity_cell) : (pl_cell_t *)0;
    if (!d2 || (int)d2->v != DT_I) { return 0; }
    long ar = (long)d2->i;
    pl_cell_t built;
    if (ar == 0) { built = d1 ? *d1 : plc_atom_id_cell(prolog_atom_intern("[]")); }
    else {
        if (!d1 || !plc_is_atomlike(d1)) { return 0; }
        pl_cell_t *args = (pl_cell_t *)PL_CELL_ALLOC((size_t)ar * sizeof(pl_cell_t));
        for (long i = 0; i < ar; i++) pl_init_var(&args[i], -1);
        built = pl_make_compound(plc_atom_id_of(d1), (int)ar, args);
    }
    if (!plc_unify_cells_cx((pl_cell_t *)t0_cell, &built, cx)) { return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_arg_cell(void *n_cell, void *t_cell, void *arg_cell, pl_tr_ctx_t *cx)
{
    pl_cell_t *dN = n_cell ? pl_deref((pl_cell_t *)n_cell) : (pl_cell_t *)0;
    pl_cell_t *dT = t_cell ? pl_deref((pl_cell_t *)t_cell) : (pl_cell_t *)0;
    if (!dN || (int)dN->v != DT_I || !dT || (int)dT->v != DT_PLREF || !dT->p) { return 0; }
    long n = (long)dN->i;
    if (n < 1 || n > (long)plc_fid_arity(dT->slen)) { return 0; }
    if (!plc_unify_cells_cx((pl_cell_t *)arg_cell, &((pl_cell_t *)dT->p)[n - 1], cx)) { return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long plc_list_cells(void *list_cell, int dot_id);
int rt_pl_univ_cell(void *t0_cell, void *list_cell, pl_tr_ctx_t *cx)
{
    extern int ATOM_DOT;
    pl_cell_t *d0 = t0_cell ? pl_deref((pl_cell_t *)t0_cell) : (pl_cell_t *)0;
    if (d0 && !pl_cell_unbound(d0)) {
        pl_cell_t lst = plc_atom_id_cell(prolog_atom_intern("[]"));
        if ((int)d0->v == DT_PLREF) {
            int ar = plc_fid_arity(d0->slen); pl_cell_t *aa = (pl_cell_t *)d0->p;
            for (int i = ar - 1; i >= 0; i--) {
                pl_cell_t *c = (pl_cell_t *)PL_CELL_ALLOC(2 * sizeof(pl_cell_t));
                c[0] = aa[i]; c[1] = lst;
                lst = pl_make_compound(ATOM_DOT, 2, c);
            }
            pl_cell_t *c = (pl_cell_t *)PL_CELL_ALLOC(2 * sizeof(pl_cell_t));
            c[0] = plc_atom_id_cell(plc_fid_name(d0->slen)); c[1] = lst;
            lst = pl_make_compound(ATOM_DOT, 2, c);
        } else {
            pl_cell_t *c = (pl_cell_t *)PL_CELL_ALLOC(2 * sizeof(pl_cell_t));
            c[0] = *d0; c[1] = lst;
            lst = pl_make_compound(ATOM_DOT, 2, c);
        }
        if (!plc_unify_cells_cx((pl_cell_t *)list_cell, &lst, cx)) { return 0; }
        return 1;
    }
    extern int ATOM_NIL; extern void *rt_pl_ball_instantiation(void); extern void *rt_pl_ball_kind1(const char *, const char *);
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    pl_cell_t *l0 = list_cell ? pl_deref((pl_cell_t *)list_cell) : (pl_cell_t *)0, *cur = l0; void *b = (void *)0; int ne = 0;
    long nl = plc_list_cells(list_cell, ATOM_DOT);
    while (nl >= 0 && cur && (int)cur->v == DT_PLREF && plc_fid_name(cur->slen) == ATOM_DOT && plc_fid_arity(cur->slen) == 2 && cur->p) { ne++; cur = pl_deref(&((pl_cell_t *)cur->p)[1]); }
    if (nl < 0) b = rt_pl_ball_kind2("type_error", "list", *l0);
    else if (!cur || pl_cell_unbound(cur)) b = rt_pl_ball_instantiation();
    else if (!(plc_is_atomlike(cur) && plc_atom_id_of(cur) == ATOM_NIL)) b = rt_pl_ball_kind2("type_error", "list", *l0);
    else if (ne == 0) b = rt_pl_ball_kind2("domain_error", "non_empty_list", *l0);
    if (b) { if (cx && !cx->ball) cx->ball = b; return 0; }
    pl_cell_t *h = pl_deref(&((pl_cell_t *)l0->p)[0]);
    if (pl_cell_unbound(h)) b = rt_pl_ball_instantiation();
    else if (ne == 1 && (int)h->v == DT_PLREF) b = rt_pl_ball_kind2("type_error", "atomic", *h);
    else if (ne > 1 && !plc_is_atomlike(h)) b = rt_pl_ball_kind2("type_error", "atom", *h);
    else if (ne - 1 > 1024) b = rt_pl_ball_kind1("representation_error", "max_arity");
    if (b) { if (cx && !cx->ball) cx->ball = b; return 0; }
    pl_cell_t built;
    if (ne == 1) { built = *h; }
    else {
        pl_cell_t *args = (pl_cell_t *)PL_CELL_ALLOC((size_t)(ne - 1) * sizeof(pl_cell_t)); int i = 0;
        for (cur = pl_deref(&((pl_cell_t *)l0->p)[1]); i < ne - 1; cur = pl_deref(&((pl_cell_t *)cur->p)[1])) args[i++] = *pl_deref(&((pl_cell_t *)cur->p)[0]);
        built = pl_make_compound(plc_atom_id_of(h), ne - 1, args);
    }
    if (!plc_unify_cells_cx((pl_cell_t *)t0_cell, &built, cx)) { return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_sp_ball(pl_tr_ctx_t *cx, void *b) { if (cx && !cx->ball) cx->ball = b; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_sp_int_arg(pl_cell_t *d, pl_tr_ctx_t *cx, int *bound)
{
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    *bound = 0;
    if (!d || pl_cell_unbound(d)) return 1;
    if ((int)d->v != DT_I && (int)d->v != DT_BIG) { plc_sp_ball(cx, rt_pl_ball_kind2("type_error", "integer", *d)); return 0; }
    *bound = 1; return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_sp_not_negative(pl_cell_t *d, pl_tr_ctx_t *cx)
{
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    extern int rt_big_sign(DESCR_t);
    if (d && ((int)d->v == DT_I || (int)d->v == DT_BIG) && rt_big_sign(*d) < 0) { plc_sp_ball(cx, rt_pl_ball_kind2("domain_error", "not_less_than_zero", *d)); return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static pl_cell_t plc_sp_addsub(pl_cell_t a, pl_cell_t b, int sub)
{
    extern DESCR_t rt_big_add(DESCR_t, DESCR_t); extern DESCR_t rt_big_sub(DESCR_t, DESCR_t);
    long long r;
    if ((int)a.v == DT_I && (int)b.v == DT_I
        && !(sub ? __builtin_sub_overflow(a.i, b.i, &r) : __builtin_add_overflow(a.i, b.i, &r))) return pl_make_int(r);
    return sub ? rt_big_sub(a, b) : rt_big_add(a, b);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_wall_clock_cell(int ms, void *a_cell, pl_tr_ctx_t *cx)
{
    struct timespec _ts; clock_gettime(CLOCK_MONOTONIC, &_ts);
    long long us = (long long)_ts.tv_sec * 1000000LL + (long long)_ts.tv_nsec / 1000LL;
    pl_cell_t v = pl_make_int(ms ? us / 1000LL : us);
    return plc_unify_cells_cx((pl_cell_t *)a_cell, &v, cx) ? 1 : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_succ_plus_cell(long arity, void *a_cell, void *b_cell, void *c_cell, pl_tr_ctx_t *cx)
{
    extern void *rt_pl_ball_instantiation(void);
    pl_cell_t *da = a_cell ? pl_deref((pl_cell_t *)a_cell) : (pl_cell_t *)0;
    pl_cell_t *db = b_cell ? pl_deref((pl_cell_t *)b_cell) : (pl_cell_t *)0;
    pl_cell_t *dc = c_cell ? pl_deref((pl_cell_t *)c_cell) : (pl_cell_t *)0;
    if (arity == 2) {
        int va, vb;
        if (!plc_sp_int_arg(da, cx, &va) || !plc_sp_int_arg(db, cx, &vb)) return 0;
        if (!va && !vb) { plc_sp_ball(cx, rt_pl_ball_instantiation()); return 0; }
        if (!plc_sp_not_negative(da, cx) || !plc_sp_not_negative(db, cx)) return 0;
        if (va) { pl_cell_t v = plc_sp_addsub(*da, pl_make_int(1), 0);
                  return plc_unify_cells_cx((pl_cell_t *)b_cell, &v, cx) ? 1 : 0; }
        if ((int)db->v == DT_I && db->i == 0) return 0;
        { pl_cell_t v = plc_sp_addsub(*db, pl_make_int(1), 1); return plc_unify_cells_cx((pl_cell_t *)a_cell, &v, cx) ? 1 : 0; }
    }
    if (arity == 3) {
        int va, vb, vc;
        if (!plc_sp_int_arg(da, cx, &va) || !plc_sp_int_arg(db, cx, &vb) || !plc_sp_int_arg(dc, cx, &vc)) return 0;
        if (va + vb + vc < 2) { plc_sp_ball(cx, rt_pl_ball_instantiation()); return 0; }
        if (va && vb)      { pl_cell_t v = plc_sp_addsub(*da, *db, 0); return plc_unify_cells_cx((pl_cell_t *)c_cell, &v, cx) ? 1 : 0; }
        if (va && vc)      { pl_cell_t v = plc_sp_addsub(*dc, *da, 1); return plc_unify_cells_cx((pl_cell_t *)b_cell, &v, cx) ? 1 : 0; }
        { pl_cell_t v = plc_sp_addsub(*dc, *db, 1); return plc_unify_cells_cx((pl_cell_t *)a_cell, &v, cx) ? 1 : 0; }
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *plc_atom_op_list_text_heap(pl_cell_t *d);
static const char *plc_atom_op_text(pl_cell_t *t, char *buf, size_t bufsz)
{
    if (!t) return (const char *)0;
    pl_cell_t *d = pl_deref(t);
    if ((int)d->v == DT_PLATOM || (int)d->v == DT_S) return plc_atom_text(d);
    if ((int)d->v == DT_I) { snprintf(buf, bufsz, "%ld", (long)d->i); return buf; }
    if ((int)d->v == DT_BIG) { extern char *rt_big_str(DESCR_t); return rt_big_str(*d); }
    if ((int)d->v == DT_R) { extern const char *pl_real_iso_str(double, char *, int); return pl_real_iso_str(d->r, buf, (int)bufsz); }
    if ((int)d->v == DT_PLREF) { const char *lt = plc_atom_op_list_text_heap(d); if (lt) return lt; }
    return (const char *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_num_of_text(const char *s, pl_cell_t *out)
{
    char *end; long iv; errno = 0; iv = strtol(s, &end, 10);
    if (end != s && *end == '\0') {
        if (errno != ERANGE) { *out = pl_make_int(iv); return 1; }
        { extern DESCR_t rt_big_from_str(const char *); DESCR_t bg = rt_big_from_str(s); if ((int)bg.v == DT_FAIL) return 0; *out = bg; return 1; } }
    { double dv = strtod(s, &end); if (end != s && *end == '\0') { *out = pl_make_float(dv); return 1; } }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static pl_cell_t plc_make_atom_cell(const char *name)
{
    int id = prolog_atom_intern(name ? name : "");
    const char *nm = prolog_atom_name(id);
    (void)nm; return pl_make_atom(id);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_unify_into_cell(pl_cell_t *dst, pl_cell_t val)
{
    pl_cell_t tmp = val;
    return pl_unify(dst, &tmp);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
pl_cell_t rt_pl_make_string_cell(const char *s)
{
    extern char *rt_heap_strdup_c(const char *); pl_cell_t c = {0};
    c.v = (DTYPE_t)DT_S; c.s = rt_heap_strdup_c(s ? s : ""); c.slen = (uint32_t)strlen(c.s); return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_text_out(void *cell, const char *s, int as_str, pl_tr_ctx_t *cx)
{
    pl_cell_t *d = cell ? pl_deref((pl_cell_t *)cell) : (pl_cell_t *)0;
    if (d && !pl_cell_unbound(d) && ((int)d->v == DT_S || (int)d->v == DT_PLATOM)) return !strcmp(plc_atom_text(d), s);
    return plc_unify_into_cell_cx((pl_cell_t *)cell, as_str ? rt_pl_make_string_cell(s) : plc_make_atom_cell(s), cx);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_u8_get(const char *s, int *adv) {
    const unsigned char *u = (const unsigned char *)s; unsigned c = u[0];
    if (c < 0x80) { *adv = 1; return (int)c; }
    if ((c & 0xE0) == 0xC0 && (u[1] & 0xC0) == 0x80) { *adv = 2; return (int)(((c & 0x1Fu) << 6) | (u[1] & 0x3Fu)); }
    if ((c & 0xF0) == 0xE0 && (u[1] & 0xC0) == 0x80 && (u[2] & 0xC0) == 0x80) { *adv = 3; return (int)(((c & 0x0Fu) << 12) | ((u[1] & 0x3Fu) << 6) | (u[2] & 0x3Fu)); }
    if ((c & 0xF8) == 0xF0 && (u[1] & 0xC0) == 0x80 && (u[2] & 0xC0) == 0x80 && (u[3] & 0xC0) == 0x80)
        { *adv = 4; return (int)(((c & 0x07u) << 18) | ((u[1] & 0x3Fu) << 12) | ((u[2] & 0x3Fu) << 6) | (u[3] & 0x3Fu)); }
    *adv = 1; return (int)c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static size_t pl_u8_count(const char *s) { size_t n = 0; int a; while (*s) { (void)rt_pl_u8_get(s, &a); s += a; n++; } return n; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_u8_put(char *o, int cp) {
    if (cp < 0) return 0;
    if (cp < 0x80) { o[0] = (char)cp; return 1; }
    if (cp < 0x800) { o[0] = (char)(0xC0 | (cp >> 6)); o[1] = (char)(0x80 | (cp & 0x3F)); return 2; }
    if (cp < 0x10000) { o[0] = (char)(0xE0 | (cp >> 12)); o[1] = (char)(0x80 | ((cp >> 6) & 0x3F)); o[2] = (char)(0x80 | (cp & 0x3F)); return 3; }
    o[0] = (char)(0xF0 | (cp >> 18)); o[1] = (char)(0x80 | ((cp >> 12) & 0x3F)); o[2] = (char)(0x80 | ((cp >> 6) & 0x3F)); o[3] = (char)(0x80 | (cp & 0x3F)); return 4;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *plc_atom_op_list_text_heap(pl_cell_t *d)
{
    extern int ATOM_DOT, ATOM_NIL;
    extern void *rt_wsb_alloc(size_t); extern void *rt_wsb_realloc(void *, size_t);
    size_t oi = 0, cap = 64; char *buf = (char *)rt_wsb_alloc(cap);
    while (d && (int)d->v == DT_PLREF && plc_fid_name(d->slen) == ATOM_DOT && plc_fid_arity(d->slen) == 2 && d->p) {
        pl_cell_t *pr = (pl_cell_t *)d->p;
        pl_cell_t *el = pl_deref(&pr[0]);
        if ((int)el->v == DT_I) { if (oi + 8 >= cap) { cap *= 2; buf = (char *)rt_wsb_realloc(buf, cap); } oi += (size_t)rt_pl_u8_put(buf + oi, (int)el->i); }
        else if (plc_is_atomlike(el)) { const char *cn = plc_atom_text(el); size_t cl = cn ? strlen(cn) : 0;
            if (!cn || !cl) return (const char *)0;
            while (oi + cl + 1 > cap) cap *= 2;
            buf = (char *)rt_wsb_realloc(buf, cap); memcpy(buf + oi, cn, cl); oi += cl; }
        else return (const char *)0;
        d = pl_deref(&pr[1]);
    }
    if (!(d && plc_is_atomlike(d) && plc_atom_id_of(d) == ATOM_NIL)) return (const char *)0;
    buf[oi] = '\0';
    return buf;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_atom_op_cell(const char *fn, void *a0_cell, void *a1_cell, void *a2_cell, pl_tr_ctx_t *cx)
{
    pl_cell_t *t0 = a0_cell ? pl_deref((pl_cell_t *)a0_cell) : (pl_cell_t *)0;
    pl_cell_t *t1 = a1_cell ? pl_deref((pl_cell_t *)a1_cell) : (pl_cell_t *)0;
    pl_cell_t *t2 = a2_cell ? pl_deref((pl_cell_t *)a2_cell) : (pl_cell_t *)0;
    char buf0[512], buf1[512];
    if (!strcmp(fn, "atom_length")) {
        const char *s = plc_atom_op_text(t0, buf0, sizeof buf0);
        if (!s) { return 0; }
        if (!plc_unify_into_cell_cx((pl_cell_t *)a1_cell, pl_make_int((int64_t)pl_u8_count(s)), cx)) { return 0; }
        return 1;
    }
    if (!strcmp(fn, "atomic_concat")) {
        const char *s0 = plc_atom_op_text(t0, buf0, sizeof buf0);
        const char *s1 = plc_atom_op_text(t1, buf1, sizeof buf1);
        if (!s0 || !s1) { return 0; }
        { size_t l0 = strlen(s0), l1 = strlen(s1);
          char *cat = (char *)rt_wsb_alloc(l0 + l1 + 1); memcpy(cat, s0, l0); memcpy(cat + l0, s1, l1); cat[l0 + l1] = '\0';
          return plc_unify_into_cell_cx((pl_cell_t *)a2_cell, plc_make_atom_cell(cat), cx) ? 1 : 0; }
    }
    if (!strcmp(fn, "atom_concat")) {
        const char *s0 = plc_atom_op_text(t0, buf0, sizeof buf0);
        const char *s1 = plc_atom_op_text(t1, buf1, sizeof buf1);
        if (!s0 || !s1) {
            char bufz[512]; const char *sz = plc_atom_op_text(t2, bufz, sizeof bufz);
            if (!sz) { return 0; }
            { size_t lz = strlen(sz);
              if (s0 && !s1) { size_t la = strlen(s0);
                  if (la > lz || memcmp(sz, s0, la)) { return 0; }
                  return plc_unify_into_cell_cx((pl_cell_t *)a1_cell, plc_make_atom_cell(sz + la), cx) ? 1 : 0; }
              if (!s0 && s1) { size_t lb = strlen(s1);
                  if (lb > lz || memcmp(sz + (lz - lb), s1, lb)) { return 0; }
                  { char pre[lz - lb + 1]; memcpy(pre, sz, lz - lb); pre[lz - lb] = '\0';
                    return plc_unify_into_cell_cx((pl_cell_t *)a0_cell, plc_make_atom_cell(pre), cx) ? 1 : 0; } } }
            return 0; }
        size_t l0 = strlen(s0), l1 = strlen(s1);
        char cat[l0 + l1 + 1]; memcpy(cat, s0, l0); memcpy(cat + l0, s1, l1); cat[l0 + l1] = '\0';
        if (!plc_unify_into_cell_cx((pl_cell_t *)a2_cell, plc_make_atom_cell(cat), cx)) { return 0; }
        return 1;
    }
    if (!strcmp(fn, "upcase_atom") || !strcmp(fn, "downcase_atom")) {
        const char *s = plc_atom_op_text(t0, buf0, sizeof buf0);
        if (!s) { return 0; }
        size_t n = strlen(s); char out[n + 1];
        int up = (!strcmp(fn, "upcase_atom"));
        for (size_t i = 0; i < n; i++) out[i] = up ? (char)toupper((unsigned char)s[i]) : (char)tolower((unsigned char)s[i]);
        out[n] = '\0';
        if (!plc_unify_into_cell_cx((pl_cell_t *)a1_cell, plc_make_atom_cell(out), cx)) { return 0; }
        return 1;
    }
    int as_codes = (!strcmp(fn, "atom_codes") || !strcmp(fn, "string_codes"));
    if (!strcmp(fn, "atom_chars") || !strcmp(fn, "string_chars") || as_codes) {
        if (t0 && !pl_cell_unbound(t0)) {
            const char *s = plc_atom_op_text(t0, buf0, sizeof buf0);
            if (!s) { return 0; }
            if (t1 && (int)t1->v == DT_S) return !strcmp(plc_atom_text(t1), s);
            int *cps = (int *)rt_wsb_alloc((strlen(s) + 1) * sizeof(int)); int cn = 0, adv;
            for (const char *q = s; *q; q += adv) { cps[cn] = rt_pl_u8_get(q, &adv); cn++; }
            pl_cell_t lst = plc_make_atom_cell("[]");
            for (int i = cn; i > 0; i--) {
                pl_cell_t el;
                if (as_codes) el = pl_make_int((int64_t)cps[i - 1]);
                else { char cs[8]; int bl = rt_pl_u8_put(cs, cps[i - 1]); cs[bl] = '\0'; el = plc_make_atom_cell(cs); }
                pl_cell_t *pair = (pl_cell_t *)PL_CELL_ALLOC(2 * sizeof(pl_cell_t));
                pair[0] = el; pair[1] = lst;
                lst = pl_make_compound(ATOM_DOT, 2, pair);
            }
            if (!plc_unify_into_cell_cx((pl_cell_t *)a1_cell, lst, cx)) { return 0; }
            return 1;
        }
        if (t1 && (int)t1->v == DT_S) return plc_text_out(a0_cell, plc_atom_text(t1), fn[0] == 's', cx);
        pl_cell_t *cur = t1;
        size_t ocap = 256; char *out = (char *)rt_wsb_alloc(ocap); size_t oi = 0;
        while (cur && (int)cur->v == DT_PLREF && plc_fid_name(cur->slen) == ATOM_DOT && plc_fid_arity(cur->slen) == 2) {
            pl_cell_t *pr = (pl_cell_t *)cur->p;
            pl_cell_t *el = pl_deref(&pr[0]);
            if (as_codes) { if ((int)el->v != DT_I) { return 0; } if (oi + 8 >= ocap) { ocap *= 2; out = (char *)rt_wsb_realloc(out, ocap); } oi += (size_t)rt_pl_u8_put(out + oi, (int)el->i); }
            else { if ((int)el->v != DT_PLATOM && (int)el->v != DT_S) { return 0; } const char *cn = plc_atom_text(el);
                   if (!cn) { if (oi + 2 > ocap) { ocap *= 2; out = (char *)rt_wsb_realloc(out, ocap); } out[oi++] = '?'; }
                   else { size_t cl = strlen(cn); while (oi + cl + 1 > ocap) ocap *= 2; out = (char *)rt_wsb_realloc(out, ocap); memcpy(out + oi, cn, cl); oi += cl; } }
            cur = pl_deref(&pr[1]);
        }
        out[oi] = '\0';
        return plc_text_out(a0_cell, out, fn[0] == 's', cx);
    }
    if (!strcmp(fn, "string_length")) {
        const char *s = plc_atom_op_text(t0, buf0, sizeof buf0);
        if (!s) { return 0; }
        if (!plc_unify_into_cell_cx((pl_cell_t *)a1_cell, pl_make_int((int64_t)pl_u8_count(s)), cx)) { return 0; }
        return 1;
    }
    if (!strcmp(fn, "string_upper") || !strcmp(fn, "string_lower")) {
        const char *s = plc_atom_op_text(t0, buf0, sizeof buf0);
        if (!s) { return 0; }
        size_t n = strlen(s); char out[n + 1];
        int up = (!strcmp(fn, "string_upper"));
        for (size_t i = 0; i < n; i++) out[i] = up ? (char)toupper((unsigned char)s[i]) : (char)tolower((unsigned char)s[i]);
        out[n] = '\0';
        return plc_text_out(a1_cell, out, 1, cx);
    }
    if (!strcmp(fn, "atom_string") || !strcmp(fn, "string_to_atom")) {
        int a0_str = !strcmp(fn, "string_to_atom");
        if (t0 && !pl_cell_unbound(t0)) {
            const char *s = plc_atom_op_text(t0, buf0, sizeof buf0);
            if (!s) { return 0; }
            return plc_text_out(a1_cell, s, !a0_str, cx);
        }
        const char *s = plc_atom_op_text(t1, buf1, sizeof buf1);
        if (!s) { return 0; }
        return plc_text_out(a0_cell, s, a0_str, cx);
    }
    if (!strcmp(fn, "number_string")) {
        if (t0 && !pl_cell_unbound(t0) && (!t1 || pl_cell_unbound(t1))) {
            const char *s = plc_atom_op_text(t0, buf0, sizeof buf0);
            if (!s) { return 0; }
            return plc_text_out(a1_cell, s, 1, cx);
        }
        const char *s = plc_atom_op_text(t1, buf1, sizeof buf1);
        if (!s) { return 0; }
        { pl_cell_t nv; if (!plc_num_of_text(s, &nv)) return 0; return plc_unify_into_cell_cx((pl_cell_t *)a0_cell, nv, cx) ? 1 : 0; }
    }
    if (!strcmp(fn, "atom_number")) {
        if (t0 && !pl_cell_unbound(t0)) {
            const char *s = plc_atom_op_text(t0, buf0, sizeof buf0);
            if (!s) { return 0; }
            { pl_cell_t nv; if (!plc_num_of_text(s, &nv)) return 0; return plc_unify_into_cell_cx((pl_cell_t *)a1_cell, nv, cx) ? 1 : 0; }
        }
        const char *s = plc_atom_op_text(t1, buf1, sizeof buf1);
        if (!s) { return 0; }
        if (!plc_unify_into_cell_cx((pl_cell_t *)a0_cell, plc_make_atom_cell(s), cx)) { return 0; }
        return 1;
    }
    if (!strcmp(fn, "string_concat")) {
        const char *s0 = plc_atom_op_text(t0, buf0, sizeof buf0);
        const char *s1 = plc_atom_op_text(t1, buf1, sizeof buf1);
        if (!s0 || !s1) { return 0; }
        size_t l0 = strlen(s0), l1 = strlen(s1);
        char cat[l0 + l1 + 1]; memcpy(cat, s0, l0); memcpy(cat + l0, s1, l1); cat[l0 + l1] = '\0';
        return plc_text_out(a2_cell, cat, 1, cx);
    }
    if (!strcmp(fn, "atomic_list_concat") || !strcmp(fn, "concat_atom")) {
        const char *sep = (t1 && ((int)t1->v == DT_PLATOM || (int)t1->v == DT_S)) ? plc_atom_text(t1) : "";
        void *result_cell = t2 ? a2_cell : a1_cell;
        size_t sl = (sep && sep[0]) ? strlen(sep) : 0, need = 1; long ne = 0;
        for (pl_cell_t *lst = t0; lst && (int)lst->v == DT_PLREF && plc_fid_arity(lst->slen) == 2; lst = pl_deref(&((pl_cell_t *)lst->p)[1])) {
            const char *es = plc_atom_op_text(pl_deref(&((pl_cell_t *)lst->p)[0]), buf0, sizeof buf0);
            if (!es) { return 0; }
            need += strlen(es) + (ne++ ? sl : 0);
        }
        char out[need]; size_t oi = 0; ne = 0;
        for (pl_cell_t *lst = t0; lst && (int)lst->v == DT_PLREF && plc_fid_arity(lst->slen) == 2; lst = pl_deref(&((pl_cell_t *)lst->p)[1])) {
            const char *es = plc_atom_op_text(pl_deref(&((pl_cell_t *)lst->p)[0]), buf0, sizeof buf0); size_t el_len = strlen(es);
            if (ne++ && sl) { memcpy(out + oi, sep, sl); oi += sl; }
            memcpy(out + oi, es, el_len); oi += el_len;
        }
        out[oi] = '\0';
        if (!plc_unify_into_cell_cx((pl_cell_t *)result_cell, plc_make_atom_cell(out), cx)) { return 0; }
        return 1;
    }
    (void)t2;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static pl_cell_t *plc_fmt_next_arg(pl_cell_t **args) {
    pl_cell_t *a = *args ? pl_deref(*args) : (pl_cell_t *)0;
    if (a && (int)a->v == DT_PLREF && pl_arity(a) == 2) { pl_cell_t *aa = (pl_cell_t *)pl_compound_heap(a); pl_cell_t *h = pl_deref(&aa[0]); *args = pl_deref(&aa[1]); return h; }
    return (pl_cell_t *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { char *b; size_t n; size_t cap; size_t seg; size_t fpos[256]; int fchr[256]; int nf; pl_tr_ctx_t *cx; } plc_fb;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_fb_raw(plc_fb *f, const char *s, size_t n)
{
    if (!n) return;
    if (f->n + n + 1 > f->cap) { size_t c = f->cap ? f->cap : 512; while (c < f->n + n + 1) c *= 2; char *nb = (char *)ct_grow(f->b, c); if (!nb) return; f->b = nb; f->cap = c; }
    memcpy(f->b + f->n, s, n); f->n += n; f->b[f->n] = '\0';
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_fb_add(plc_fb *f, const char *s, size_t n)
{
    size_t at = f->n; plc_fb_raw(f, s, n);
    for (size_t k = at; k < f->n; k++) if (f->b[k] == '\n') { f->seg = k + 1; f->nf = 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static size_t plc_fb_colat(plc_fb *f, size_t at) { size_t k = at, col = 0; while (k > 0 && f->b[k - 1] != '\n') k--;
    for (; k < at; k++) if (((unsigned char)f->b[k] & 0xC0u) != 0x80u) col++;
    return col; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_fb_u8(plc_fb *f, unsigned cp) { char u[4];
    if (cp < 0x80u) { u[0] = (char)cp; plc_fb_raw(f, u, 1); return; }
    if (cp < 0x800u) { u[0] = (char)(0xC0u | (cp >> 6)); u[1] = (char)(0x80u | (cp & 0x3Fu)); plc_fb_raw(f, u, 2); return; }
    if (cp < 0x10000u) { u[0] = (char)(0xE0u | (cp >> 12)); u[1] = (char)(0x80u | ((cp >> 6) & 0x3Fu)); u[2] = (char)(0x80u | (cp & 0x3Fu)); plc_fb_raw(f, u, 3); return; }
    u[0] = (char)(0xF0u | (cp >> 18)); u[1] = (char)(0x80u | ((cp >> 12) & 0x3Fu)); u[2] = (char)(0x80u | ((cp >> 6) & 0x3Fu)); u[3] = (char)(0x80u | (cp & 0x3Fu)); plc_fb_raw(f, u, 4); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_fb_tab(plc_fb *f, int fill) { if (f->nf < 256) { f->fpos[f->nf] = f->n; f->fchr[f->nf] = fill; f->nf++; } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_fb_stop(plc_fb *f, long target)
{
    long cur = (long)plc_fb_colat(f, f->n), pad = target - cur;
    if (pad > 0 && f->nf == 0) { for (long i = 0; i < pad; i++) plc_fb_raw(f, " ", 1); }
    else if (pad > 0) {
        size_t base = f->seg, segn = f->n - f->seg; int nf = f->nf, fi = 0; char seg[segn + 1];
        if (segn) memcpy(seg, f->b + base, segn); f->n = base; if (f->b) f->b[f->n] = '\0';
        for (size_t k = 0; k <= segn; k++) {
            while (fi < nf && f->fpos[fi] - base == k) {
                long hi = ((long)(fi + 1) * pad) / nf, lo = ((long)fi * pad) / nf;
                for (long j = lo; j < hi; j++) plc_fb_u8(f, (unsigned)f->fchr[fi]);
                fi++;
            }
            if (k < segn) plc_fb_raw(f, seg + k, 1);
        }
    }
    f->seg = f->n; f->nf = 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void *plc_fb_term(plc_fb *f, pl_cell_t *t, int kind, int quoted, int ignore_ops)
{
    extern FILE *fh_memsink_open(char **, size_t *); char *bp = (char *)0; size_t bn = 0; FILE *ms = fh_memsink_open(&bp, &bn); plc_vmap m;
    if (!ms) return (void *)0;
    m.n = 0; m.vnc = 0; m.fp = ms; m.portray = (int (*)(pl_cell_t *, plc_vmap *))0; m.pthrown = (void *)0; m.cx = (pl_tr_ctx_t *)0; m.pheld = 0;
    if (kind == 4) { extern int rt_proc_is_registered(const char *); m.portray = rt_proc_is_registered("portray/1") ? plc_portray_hit : (int (*)(pl_cell_t *, plc_vmap *))0; m.cx = f->cx; plc_write(t, &m); }
    else if (kind == 0) plc_write(t, &m);
    else if (kind == 1) plc_writeq(t, &m);
    else if (kind == 2) plc_write_canonical(t, &m);
    else plc_wt(t, quoted, ignore_ops, 1, -1, 0, 1200, &m);
    fclose(ms);
    plc_fb_add(f, bp ? bp : "", bn);
    return m.pthrown;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_fmt_is_atom(pl_cell_t *d) { return d && ((int)d->v == DT_PLATOM || (int)d->v == DT_S) && !pl_cell_unbound(d); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_fmt_eval(pl_cell_t *h, DESCR_t *out, void **ball)
{
    extern int rt_pl_ax_eval_val(DESCR_t, DESCR_t *, void **);
    if (!h) return 0;
    if (pl_cell_unbound(h)) { extern void *rt_pl_ball_instantiation(void); if (!*ball) *ball = rt_pl_ball_instantiation(); return 0; }
    return rt_pl_ax_eval_val(*h, out, ball);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_fmt_int(pl_cell_t *h, long *out, void **ball)
{
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    DESCR_t v;
    if (!plc_fmt_eval(h, &v, ball)) return 0;
    if (v.v != DT_I) { if (!*ball) *ball = rt_pl_ball_kind2("type_error", "integer", v); return 0; }
    *out = (long)v.i; return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_fmt_dbl(pl_cell_t *h, double *out, void **ball)
{
    DESCR_t v;
    if (!plc_fmt_eval(h, &v, ball)) return 0;
    *out = (v.v == DT_I) ? (double)v.i : v.r; return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_fmt_text(pl_cell_t *h, char **out, size_t *len, void **ball)
{
    extern void *rt_pl_ball_instantiation(void); extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    size_t cap = 64, n = 0; char *b; pl_cell_t *lst;
    if (!h) return 0;
    if (pl_cell_unbound(h)) { if (!*ball) *ball = rt_pl_ball_instantiation(); return 0; }
    if (plc_fmt_is_atom(h)) { const char *s = plc_atom_text(h); *len = strlen(s); *out = (char *)ct_alloc(*len + 1); if (!*out) return 0; memcpy(*out, s, *len + 1); return 1; }
    b = (char *)ct_alloc(cap); if (!b) return 0;
    lst = pl_deref(h);
    while (lst && (int)lst->v == DT_PLREF && pl_arity(lst) == 2) {
        pl_cell_t *aa = (pl_cell_t *)pl_compound_heap(lst); pl_cell_t *e = pl_deref(&aa[0]); int ch = -1;
        if (pl_cell_unbound(e)) { ct_drop(b); if (!*ball) *ball = rt_pl_ball_instantiation(); return 0; }
        if (pl_is_int(e)) ch = (int)pl_int_val(e);
        else if (plc_fmt_is_atom(e)) { const char *s = plc_atom_text(e); ch = s ? (unsigned char)s[0] : 0; }
        else { ct_drop(b); if (!*ball) *ball = rt_pl_ball_kind2("type_error", "text", *e); return 0; }
        if (n + 2 > cap) { cap *= 2; char *nb = (char *)ct_grow(b, cap); if (!nb) { ct_drop(b); return 0; } b = nb; }
        b[n++] = (char)ch; lst = pl_deref(&aa[1]);
    }
    if (lst && pl_cell_unbound(lst)) { ct_drop(b); if (!*ball) *ball = rt_pl_ball_instantiation(); return 0; }
    if (!lst || !plc_is_nil(lst)) { ct_drop(b); if (!*ball) *ball = rt_pl_ball_kind2("type_error", "text", lst ? *lst : *h); return 0; }
    b[n] = '\0'; *out = b; *len = n; return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_fmt_digits(plc_fb *f, int neg, const char *d, long nd, long places, int group)
{
    long tot = nd > places ? nd : places + 1, ip = tot - places, i;
    if (neg) plc_fb_add(f, "-", 1);
    for (i = 0; i < tot; i++) { char c = i < tot - nd ? '0' : d[i - (tot - nd)];
        if (i == ip && places > 0) plc_fb_add(f, ".", 1);
        if (group && i && i < ip && !((ip - i) % 3)) plc_fb_add(f, ",", 1);
        plc_fb_add(f, &c, 1); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_fmt_dec(plc_fb *f, long long iv, long places, int group)
{
    char dig[24]; int nd = 0, neg = iv < 0; unsigned long long u = neg ? (unsigned long long)(-(iv + 1)) + 1ull : (unsigned long long)iv;
    if (!u) dig[nd++] = '0';
    while (u) { dig[nd++] = (char)('0' + (int)(u % 10ull)); u /= 10ull; }
    for (int i = 0; i < nd / 2; i++) { char t = dig[i]; dig[i] = dig[nd - 1 - i]; dig[nd - 1 - i] = t; }
    plc_fmt_digits(f, neg, dig, nd, places, group);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_fmt_radix(plc_fb *f, long long iv, int base, int upper)
{
    const char *d = upper ? "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ" : "0123456789abcdefghijklmnopqrstuvwxyz";
    char buf[80]; int bi = 0, oi = 0; char outb[82]; unsigned long long u = iv < 0 ? (unsigned long long)(-(iv + 1)) + 1ull : (unsigned long long)iv;
    if (!u) buf[bi++] = '0';
    while (u) { buf[bi++] = d[(int)(u % (unsigned long long)base)]; u /= (unsigned long long)base; }
    if (iv < 0) outb[oi++] = '-';
    while (bi) outb[oi++] = buf[--bi];
    plc_fb_add(f, outb, (size_t)oi);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_format_run(const char *fmt, void *list_cell, pl_tr_ctx_t *cx)
{
    extern void *rt_pl_ball_instantiation(void); extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    plc_fb f; void *ball = (void *)0; pl_cell_t *args = list_cell ? pl_deref((pl_cell_t *)list_cell) : (pl_cell_t *)0; pl_cell_t all = args ? *args : pl_make_int(0); const char *p; FILE *fd;
    plc_wsref ra = { -1, (pl_cell_t *)0 }, rall = { -1, (pl_cell_t *)0 }; long mark = -1; pl_cell_t *args0 = args;
    plc_atoms_ready();
    memset(&f, 0, sizeof f); f.cx = cx;
    if (!fmt) return (void *)0;
    size_t fl = strlen(fmt); char fmtc[fl + 1]; memcpy(fmtc, fmt, fl + 1); fmt = fmtc;
    for (p = fmt; *p && !ball; p++) {
        long nval = 0; int have_n = 0, fill = ' ';
        if (*p != '~') { plc_fb_add(&f, p, 1); continue; }
        p++;
        if (*p == '`' && p[1]) { fill = (unsigned char)p[1]; have_n = 1; nval = fill; p += 2; }
        else if (*p == '*') { pl_cell_t *h = plc_fmt_next_arg(&args); p++; if (!h) { ball = rt_pl_ball_kind2("domain_error", "format_arguments", all); break; } if (!plc_fmt_int(h, &nval, &ball)) break; have_n = 1; fill = (int)nval; }
        else { while (*p >= '0' && *p <= '9') { nval = nval * 10 + (*p - '0'); have_n = 1; p++; } if (have_n) fill = (int)nval; }
        if (!*p) break;
        if (*p == '~') { plc_fb_add(&f, "~", 1); continue; }
        if (*p == 'n') { long r = have_n ? nval : 1; for (long i = 0; i < r; i++) plc_fb_add(&f, "\n", 1); continue; }
        if (*p == 'N') { if (plc_fb_colat(&f, f.n)) plc_fb_add(&f, "\n", 1); continue; }
        if (*p == 't') { plc_fb_tab(&f, have_n ? fill : ' '); continue; }
        if (*p == '|') { plc_fb_stop(&f, have_n ? nval : (long)plc_fb_colat(&f, f.n)); continue; }
        if (*p == '+') { plc_fb_stop(&f, (long)plc_fb_colat(&f, f.seg) + (have_n ? nval : 8)); continue; }
        { pl_cell_t *h = plc_fmt_next_arg(&args);
          if (!h) { ball = rt_pl_ball_kind2("domain_error", "format_arguments", all); break; }
          if (*p == 'w') plc_fb_term(&f, h, 0, 0, 0);
          else if (*p == 'p') { void *tb;
              if (mark < 0 && cx && cx->tr) { mark = plc_ws_mark(cx); plc_wsref_set(cx, &rall, args0); }
              if (mark >= 0) plc_wsref_set(cx, &ra, args);
              tb = plc_fb_term(&f, h, 4, 0, 0);
              if (mark >= 0) { args = plc_wsref_get(cx, &ra); args0 = plc_wsref_get(cx, &rall); if (args0) all = *args0; }
              if (tb) { ball = tb; break; } }
          else if (*p == 'q') plc_fb_term(&f, h, 1, 0, 0);
          else if (*p == 'k') plc_fb_term(&f, h, 2, 0, 0);
          else if (*p == 'i') { }
          else if (*p == 'a') {
              if (pl_cell_unbound(h)) { ball = rt_pl_ball_instantiation(); break; }
              if (!plc_fmt_is_atom(h)) { ball = rt_pl_ball_kind2("type_error", "atom", *h); break; }
              { const char *s = plc_atom_text(h); plc_fb_add(&f, s, strlen(s)); }
          }
          else if (*p == 'c') { long cv; if (!plc_fmt_int(h, &cv, &ball)) break; { long r = have_n ? nval : 1; char c = (char)cv; for (long i = 0; i < r; i++) plc_fb_add(&f, &c, 1); } }
          else if (*p == 'd' || *p == 'D') { long iv; if (have_n && nval < 0) { ball = rt_pl_ball_kind2("domain_error", "format_argument", pl_make_int(nval)); break; } { DESCR_t bv; if (plc_fmt_eval(h, &bv, &ball) && bv.v == DT_BIG) { extern char *rt_big_str(DESCR_t); char *bs = rt_big_str(bv); int ng = bs && bs[0] == '-';
              plc_fmt_digits(&f, ng, bs ? bs + ng : "0", bs ? (long)strlen(bs + ng) : 1L, have_n ? nval : 0, *p == 'D'); continue; } }
            if (!plc_fmt_int(h, &iv, &ball)) break; plc_fmt_dec(&f, (long long)iv, have_n ? nval : 0, *p == 'D'); }
          else if (*p == 'r' || *p == 'R') { long iv; long base = have_n ? nval : 8; if (base < 2 || base > 36) { ball = rt_pl_ball_kind2("domain_error", "radix", pl_make_int(base)); break; } if (!plc_fmt_int(h, &iv, &ball)) break; plc_fmt_radix(&f, (long long)iv, (int)base, *p == 'R'); }
          else if (*p == 'f' || *p == 'e' || *p == 'E' || *p == 'g' || *p == 'G') {
              double dv; char spec[8];
              if (!plc_fmt_dbl(h, &dv, &ball)) break;
              spec[0] = '%'; spec[1] = '.'; spec[2] = '*'; spec[3] = (char)*p; spec[4] = '\0';
              { int prec = have_n ? (int)nval : 6; char nb[fmt_len(spec, prec, dv)]; snprintf(nb, sizeof nb, spec, prec, dv); plc_fb_add(&f, nb, strlen(nb)); }
          }
          else if (*p == 's') {
              char *tx = (char *)0; size_t tn = 0;
              if (!plc_fmt_text(h, &tx, &tn, &ball)) break;
              if (have_n && (size_t)nval < tn) tn = (size_t)(nval < 0 ? 0 : nval);
              plc_fb_add(&f, tx, tn);
              if (have_n && (size_t)nval > tn) for (size_t i = tn; i < (size_t)nval; i++) plc_fb_add(&f, " ", 1);
              ct_drop(tx);
          }
          else if (*p == 'W') {
              pl_cell_t *o = plc_fmt_next_arg(&args); int quoted = 0, iops = 0; pl_cell_t *lst;
              if (!o) { ball = rt_pl_ball_kind2("domain_error", "format_arguments", all); break; }
              if (pl_cell_unbound(o)) { ball = rt_pl_ball_instantiation(); break; }
              for (lst = pl_deref(o); lst && (int)lst->v == DT_PLREF && pl_arity(lst) == 2; ) {
                  pl_cell_t *aa = (pl_cell_t *)pl_compound_heap(lst); pl_cell_t *e = pl_deref(&aa[0]);
                  if (e && (int)e->v == DT_PLREF && pl_arity(e) == 1) {
                      const char *on = prolog_atom_name(plc_fid_name(e->slen)); pl_cell_t *ov = pl_deref(&((pl_cell_t *)pl_compound_heap(e))[0]);
                      int on_true = plc_fmt_is_atom(ov) && !strcmp(plc_atom_text(ov), "true");
                      if (on && !strcmp(on, "quoted")) quoted = on_true;
                      else if (on && !strcmp(on, "ignore_ops")) iops = on_true;
                  }
                  lst = pl_deref(&aa[1]);
              }
              plc_fb_term(&f, h, 3, quoted, iops);
          }
        }
    }
    if (!ball && args && !pl_cell_unbound(args) && !plc_is_nil(args)) ball = rt_pl_ball_kind2("domain_error", "format_arguments", all);
    if (!ball) { fd = plc_out(); if (f.n) fwrite(f.b, 1, f.n, fd); }
    ct_drop(f.b);
    plc_ws_end(cx, mark);
    return ball;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_format_cell(const char *fmt, void *list_cell) { (void)rt_pl_format_run(fmt, list_cell, (pl_tr_ctx_t *)0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_char_type_cell(void *char_cell, void *type_cell, void *val_cell, pl_tr_ctx_t *cx)
{
    pl_cell_t *tc = char_cell ? pl_deref((pl_cell_t *)char_cell) : (pl_cell_t *)0;
    if (!tc || !type_cell) return 0;
    char b0[256]; const char *cs = plc_atom_op_text(tc, b0, sizeof b0);
    if (!cs || !cs[0]) return 0;
    unsigned char ch = (unsigned char)cs[0];
    pl_cell_t *td = pl_deref((pl_cell_t *)type_cell);
    if ((int)td->v == DT_PLREF && pl_arity(td) >= 1) {
        const char *ty = prolog_atom_name(plc_functor(td));
        pl_cell_t *inner = val_cell ? (pl_cell_t *)val_cell : &((pl_cell_t *)pl_compound_heap(td))[0];
        pl_cell_t out;
        if (!ty) { return 0; }
        if (!strcmp(ty, "digit"))    { if (!isdigit(ch)) { return 0; } out = pl_make_int((int64_t)(ch - '0')); }
        else if (!strcmp(ty, "to_lower")) { char c2[2] = { (char)tolower(ch), 0 }; out = plc_make_atom_cell(c2); }
        else if (!strcmp(ty, "to_upper")) { char c2[2] = { (char)toupper(ch), 0 }; out = plc_make_atom_cell(c2); }
        else if (!strcmp(ty, "upper")) { if (!isupper(ch)) { return 0; } char c2[2] = { (char)tolower(ch), 0 }; out = plc_make_atom_cell(c2); }
        else if (!strcmp(ty, "lower")) { if (!islower(ch)) { return 0; } char c2[2] = { (char)toupper(ch), 0 }; out = plc_make_atom_cell(c2); }
        else if (!strcmp(ty, "code"))  { out = pl_make_int((int64_t)ch); }
        else { return 0; }
        if (!plc_unify_into_cell_cx(inner, out, cx)) { return 0; }
        return 1;
    }
    if ((int)td->v != DT_PLATOM && (int)td->v != DT_S) { return 0; }
    const char *ty = ((int)td->v == DT_S) ? (td->s ? td->s : "") : prolog_atom_name(pl_atom_id(td));
    if (!ty) { return 0; }
    int ok = 0;
    if      (!strcmp(ty, "alpha"))        ok = isalpha(ch);
    else if (!strcmp(ty, "alnum"))        ok = isalnum(ch);
    else if (!strcmp(ty, "digit"))        ok = isdigit(ch);
    else if (!strcmp(ty, "space") || !strcmp(ty, "white")) ok = isspace(ch);
    else if (!strcmp(ty, "upper"))        ok = isupper(ch);
    else if (!strcmp(ty, "lower"))        ok = islower(ch);
    else if (!strcmp(ty, "punct"))        ok = ispunct(ch);
    else if (!strcmp(ty, "graph"))        ok = isgraph(ch);
    else if (!strcmp(ty, "csym"))         ok = (isalnum(ch) || ch == '_');
    else if (!strcmp(ty, "csymf"))        ok = (isalpha(ch) || ch == '_');
    else if (!strcmp(ty, "end_of_line"))  ok = (ch == '\n' || ch == '\r');
    else if (!strcmp(ty, "newline"))      ok = (ch == '\n');
    else { return 0; }
    if (!ok) { return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_lower_upper_cell(void *lower_cell, void *upper_cell, pl_tr_ctx_t *cx)
{
    if (!lower_cell || !upper_cell) return 0;
    pl_cell_t *lo = pl_deref((pl_cell_t *)lower_cell);
    pl_cell_t *up = pl_deref((pl_cell_t *)upper_cell);
    int lo_bound = !pl_cell_unbound(lo), up_bound = !pl_cell_unbound(up);
    if (!lo_bound && !up_bound) { extern void rt_pl_iso_throw_instantiation(void); rt_pl_iso_throw_instantiation(); return 0; }
    char b0[256];
    if (lo_bound) {
        const char *cs = plc_atom_op_text(lo, b0, sizeof b0);
        if (!cs || !cs[0] || cs[1]) return 0;
        char c2[2] = { (char)toupper((unsigned char)cs[0]), 0 };
        if (!plc_unify_into_cell_cx(up, plc_make_atom_cell(c2), cx)) { return 0; }
        return 1;
    }
    const char *cs = plc_atom_op_text(up, b0, sizeof b0);
    if (!cs || !cs[0] || cs[1]) return 0;
    char c2[2] = { (char)tolower((unsigned char)cs[0]), 0 };
    if (!plc_unify_into_cell_cx(lo, plc_make_atom_cell(c2), cx)) { return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_text_eq(const DESCR_t *A, const DESCR_t *B) {
    if ((int)A->v == DT_PLATOM && (int)B->v == DT_PLATOM) return A->i == B->i;
    if ((int)A->v != DT_S || (int)B->v != DT_S) return 0;
    return A->slen == B->slen && (!A->slen || !memcmp(A->s, B->s, A->slen));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_pl_cell_class(pl_cell_t *d) {
    int t = (int)d->v;
    if (pl_cell_unbound(d)) return 0;
    if (t == DT_I || t == DT_R || t == DT_BIG) return 1;
    if (t == DT_S) return 2;
    if (t == DT_PLATOM) return 3;
    if (t == DT_PLREF) return 4;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *rt_pl_cell_name(pl_cell_t *d) {
    if ((int)d->v == DT_S) return d->s ? d->s : "";
    const char *n = prolog_atom_name((int)d->i);
    return n ? n : "";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_pl_cell_compare_leaf(pl_cell_t *a, pl_cell_t *b, void *ctx) {
    if ((int)a->v == DT_PLREF && (int)b->v == DT_PLREF && a->slen == b->slen) return PLR_DESCEND;
    if ((int)a->v == DT_I && (int)b->v == DT_I) return a->i < b->i ? -1 : (a->i > b->i ? 1 : 0);
    int cla = rt_pl_cell_class(a), clb = rt_pl_cell_class(b);
    if (cla != clb) return cla < clb ? -1 : 1;
    if (cla == 0) return a == b ? 0 : ((uintptr_t)a < (uintptr_t)b ? -1 : 1);
    if (cla == 1 && ((int)a->v == DT_R) != ((int)b->v == DT_R)) return ((int)a->v == DT_R) ? -1 : 1;
    if (cla == 1 && ((int)a->v == DT_BIG || (int)b->v == DT_BIG)) { extern int rt_big_cmp(DESCR_t, DESCR_t); int rc = rt_big_cmp(*a, *b); return rc < 0 ? -1 : (rc > 0 ? 1 : 0); }
    if (cla == 1) { double x = ((int)a->v == DT_I) ? (double)a->i : a->r, y = ((int)b->v == DT_I) ? (double)b->i : b->r; if (x < y) return -1; if (x > y) return 1;
        if ((int)a->v == DT_R && (int)b->v == DT_R && signbit(x) != signbit(y)) return signbit(x) ? -1 : 1; if ((int)a->v == (int)b->v) return 0; return ((int)a->v == DT_R) ? -1 : 1; }
    if (cla == 2) { uint32_t n = a->slen < b->slen ? a->slen : b->slen; int c = n ? memcmp(a->s, b->s, n) : 0; if (!c) c = a->slen < b->slen ? -1 : a->slen > b->slen ? 1 : 0; return c < 0 ? -1 : (c > 0 ? 1 : 0); }
    if (cla == 3) { if (a->i == b->i) return 0; int c = strcmp(rt_pl_cell_name(a), rt_pl_cell_name(b)); return c < 0 ? -1 : (c > 0 ? 1 : 0); }
    int ara = plc_fid_arity(a->slen), arb = plc_fid_arity(b->slen);
    if (ara != arb) return ara < arb ? -1 : 1;
    if ((plc_fid_name(a->slen)) != (plc_fid_name(b->slen))) { const char *na = prolog_atom_name(plc_fid_name(a->slen)), *nb = prolog_atom_name(plc_fid_name(b->slen));
      int c = strcmp(na ? na : "", nb ? nb : ""); if (c) return c < 0 ? -1 : 1; }
    (void)ctx; return PLR_DESCEND;
}
PLR_WALK2(plc_compare_walk, rt_pl_cell_compare_leaf, pl_deref)
static int rt_pl_cell_compare(pl_cell_t *ca, pl_cell_t *cb) { return plc_compare_walk(ca, cb, (void *)0, 0); }
static int plc_unify_leaf(pl_cell_t *A, pl_cell_t *B, void *ctx)
{
    int av = pl_cell_unbound(A), bv = pl_cell_unbound(B);
    (void)ctx;
    if (av && bv) { pl_cell_t *j = (pl_cell_t *)rt_ws_alloc_descr(1); pl_cell_t r; j->v = (DTYPE_t)DT_PLVAR; j->slen = 0; j->p = (void *)j; r.v = (DTYPE_t)DT_PLVAR; r.slen = 0; r.p = (void *)j;
        pl_bind(A, r); pl_bind(B, r); return 1; }
    if (av) { pl_bind(A, *B); return 1; }
    if (bv) { pl_bind(B, *A); return 1; }
    if ((int)A->v == DT_PLATOM && (int)B->v == DT_PLATOM) return A->i == B->i;
    if (((int)A->v == DT_S || (int)A->v == DT_PLATOM) && ((int)B->v == DT_S || (int)B->v == DT_PLATOM)) return rt_pl_text_eq(A, B);
    if (A->v != B->v) return 0;
    if ((int)A->v == DT_I) return A->i == B->i;
    if ((int)A->v == DT_R) return A->r == B->r;
    if ((int)A->v == DT_PLREF) return A->slen == B->slen ? PLR_DESCEND : 0;
    return 0;
}
PLR_WALK2(plc_unify_walk, plc_unify_leaf, pl_deref)
int rt_pl_unify_cyc_plain(pl_cell_t *a, pl_cell_t *b) { return plc_unify_walk(a, b, (void *)0, 1); }
int rt_pl_atop_cell(int op, void *a_cell, void *b_cell)
{
    int c = rt_pl_cell_compare((pl_cell_t *)a_cell, (pl_cell_t *)b_cell);
    if (op == 0) return c < 0;
    if (op == 1) return c <= 0;
    if (op == 2) return c > 0;
    if (op == 3) return c >= 0;
    if (op == 4) return c == 0;
    return c != 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_compare_cell(void *order_cell, void *a_cell, void *b_cell, pl_tr_ctx_t *cx)
{
    pl_cell_t *o = pl_deref((pl_cell_t *)order_cell);
    if (!pl_cell_unbound(o)) { extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t); const char *on = plc_is_atomlike(o) ? rt_pl_cell_name(o) : (const char *)0;
        if (!on) { if (cx && !cx->ball) cx->ball = rt_pl_ball_kind2("type_error", "atom", *o); return 0; }
        if (strcmp(on, "<") && strcmp(on, "=") && strcmp(on, ">")) { if (cx && !cx->ball) cx->ball = rt_pl_ball_kind2("domain_error", "order", *o); return 0; } }
    int c = rt_pl_cell_compare((pl_cell_t *)a_cell, (pl_cell_t *)b_cell);
    const char *nm = (c < 0) ? "<" : (c > 0) ? ">" : "=";
    const char *an = prolog_atom_name(prolog_atom_intern(nm));
    pl_cell_t ord = pl_make_atom(prolog_atom_intern(an ? an : nm));
    if (!plc_unify_cells_cx((pl_cell_t *)order_cell, &ord, cx)) { return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { pl_cell_t **a; pl_cell_t **b; int n; int cap; } pl_vmap_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_vmap_ok(pl_vmap_t *m, pl_cell_t *x, pl_cell_t *y) {
    for (int i = 0; i < m->n; i++) { if (m->a[i] == x) return m->b[i] == y; if (m->b[i] == y) return 0; }
    if (m->n >= m->cap) return 0;
    m->a[m->n] = x; m->b[m->n] = y; m->n++;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_variant_rec(pl_cell_t *ca, pl_cell_t *cb, pl_vmap_t *m) {
    pl_cell_t *a = pl_deref(ca), *b = pl_deref(cb);
    int ua = pl_cell_unbound(a), ub = pl_cell_unbound(b);
    if (ua || ub) { if (ua != ub) return 0; return pl_vmap_ok(m, a, b); }
    if (((int)a->v == (int)DT_PLREF) != ((int)b->v == (int)DT_PLREF)) return 0;
    if ((int)a->v == (int)DT_PLREF) {
        int ara = plc_fid_arity(a->slen), arb = plc_fid_arity(b->slen);
        if (ara != arb || plc_fid_name(a->slen) != plc_fid_name(b->slen)) return 0;
        { pl_cell_t *aa = (pl_cell_t *)a->p, *bb = (pl_cell_t *)b->p;
          for (int i = 0; i < ara; i++) if (!pl_variant_rec(&aa[i], &bb[i], m)) return 0;
          return 1; }
    }
    return rt_pl_atop_cell(4, (void *)a, (void *)b);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_var_occ(pl_cell_t *c);
static void rt_pl_tv_walk(pl_cell_t *c, pl_cell_t **pool, int *pool_n);
static int pl_acyclic_walk(pl_cell_t *c);
static pl_cell_t plc_copy(pl_cell_t *c);
int rt_pl_variant_cells(void *a_cell, void *b_cell) {
    pl_cell_t *a = (pl_cell_t *)a_cell, *b = (pl_cell_t *)b_cell;
    if (pl_acyclic_walk(a) && pl_acyclic_walk(b)) {
        int cap = plc_var_occ(a) + 1; pl_cell_t *ma[cap], *mb[cap]; pl_vmap_t m;
        m.a = ma; m.b = mb; m.n = 0; m.cap = cap;
        return pl_variant_rec(a, b, &m);
    }
    { pl_cell_t ca = plc_copy(a), cb = plc_copy(b); int na = plc_var_occ(&ca) + 1, nb = plc_var_occ(&cb) + 1, ka = 0, kb = 0; pl_cell_t *va[na], *vb[nb];
      rt_pl_tv_walk(&ca, va, &ka); rt_pl_tv_walk(&cb, vb, &kb);
      if (ka != kb) return 0;
      for (int i = 0; i < ka; i++) { va[i]->v = (DTYPE_t)DT_PLVAR; va[i]->slen = 0; va[i]->p = (void *)vb[i]; }
      return rt_pl_cell_compare(&ca, &cb) == 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_var_occ_visit(pl_cell_t *d, void *ctx) { if (pl_cell_unbound(d)) ++*(int *)ctx; return PLR_V_GO; }
PLR_WALK1(plc_var_occ_walk, plc_var_occ_visit, pl_deref)
static int plc_var_occ(pl_cell_t *c) { int k = 0; if (c) (void)plc_var_occ_walk(c, (void *)&k); return k; }
int rt_pl_var_occ(void *cell) { return plc_var_occ((pl_cell_t *)cell); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long plc_list_cells(void *list_cell, int dot_id)
{
    pl_cell_t *cur = list_cell ? pl_deref((pl_cell_t *)list_cell) : (pl_cell_t *)0; void *ck = (void *)0; long n = 0, st = 0, pw = 1;
    while (cur && (int)cur->v == DT_PLREF && plc_fid_name(cur->slen) == dot_id && plc_fid_arity(cur->slen) == 2 && cur->p) {
        if (cur->p == ck) return -1;
        if (++st == pw) { ck = cur->p; pw <<= 1; st = 0; }
        n++; cur = pl_deref(&((pl_cell_t *)cur->p)[1]);
    }
    return n;
}
static int plc_list_len(void *list_cell, int dot_id) { long n = plc_list_cells(list_cell, dot_id); return n > 0 ? (int)n : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { pl_cell_t **pool; int *n; int *counts; } plc_tv_t;
static int plc_tv_visit(pl_cell_t *d, void *ctx)
{
    plc_tv_t *t = (plc_tv_t *)ctx;
    if (!pl_cell_unbound(d)) return PLR_V_GO;
    for (int i = 0; i < *t->n; i++) if (t->pool[i] == d) { if (t->counts) t->counts[i]++; return PLR_V_GO; }
    if (t->counts) t->counts[*t->n] = 1;
    t->pool[(*t->n)++] = d;
    return PLR_V_GO;
}
PLR_WALK1(plc_tv_walk, plc_tv_visit, pl_deref)
static void rt_pl_tv_walk(pl_cell_t *c, pl_cell_t **pool, int *pool_n) { plc_tv_t t; t.pool = pool; t.n = pool_n; t.counts = (int *)0; if (c) (void)plc_tv_walk(c, (void *)&t); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_term_variables_cell(void *term_cell, void *vars_cell, void *tail_cell, pl_tr_ctx_t *cx)
{
    pl_cell_t *pool[plc_var_occ((pl_cell_t *)term_cell) + 1]; int pn = 0;
    rt_pl_tv_walk((pl_cell_t *)term_cell, pool, &pn);
    int dot_id = prolog_atom_intern(".");
    pl_cell_t nil = pl_make_atom(ATOM_NIL);
    pl_cell_t result = tail_cell ? *(pl_cell_t *)tail_cell : nil;
    for (int i = pn - 1; i >= 0; i--) {
        pl_cell_t *blk = (pl_cell_t *)PL_CELL_ALLOC(2 * sizeof(pl_cell_t));
        blk[0] = pl_make_ref(pool[i], (int)pool[i]->slen); blk[1] = result;
        result = pl_make_compound(dot_id, 2, blk);
    }
    if (!plc_unify_cells_cx((pl_cell_t *)vars_cell, &result, cx)) { return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_subsumes_cell(void *gen_cell, void *spec_cell)
{
    pl_cell_t *g = (pl_cell_t *)gen_cell, *s = (pl_cell_t *)spec_cell;
    if (!g || !s) return 0;
    pl_cell_t *svars[plc_var_occ(s) + 1]; int sn = 0;
    rt_pl_tv_walk(s, svars, &sn);
    int unified = pl_unify(g, s);
    int bound_spec = 0;
    if (unified) for (int i = 0; i < sn; i++) if (!pl_cell_unbound(pl_deref(svars[i]))) { bound_spec = 1; break; }
    return unified && !bound_spec;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_sort_cell(int do_msort, void *list_cell, void *result_cell, pl_tr_ctx_t *cx)
{
    int dot_id = prolog_atom_intern("."); long nl = plc_list_cells(list_cell, dot_id);
    if (nl < 0) return 0;
    pl_cell_t *elems[nl + 1]; int n = 0;
    pl_cell_t *cur = pl_deref((pl_cell_t *)list_cell);
    while (n < nl && cur && (int)cur->v == DT_PLREF && plc_fid_name(cur->slen) == dot_id && plc_fid_arity(cur->slen) == 2) {
        pl_cell_t *aa = (pl_cell_t *)cur->p;
        elems[n++] = &aa[0];
        cur = pl_deref(&aa[1]);
    }
    for (int i = 1; i < n; i++) {
        pl_cell_t *key = elems[i]; int j = i - 1;
        while (j >= 0 && rt_pl_cell_compare(elems[j], key) > 0) { elems[j + 1] = elems[j]; j--; }
        elems[j + 1] = key;
    }
    int m = 0; int out_idx[n + 1];
    for (int i = 0; i < n; i++) {
        if (!do_msort && m > 0 && rt_pl_cell_compare(elems[out_idx[m - 1]], elems[i]) == 0) continue;
        out_idx[m++] = i;
    }
    pl_cell_t nil = pl_make_atom(ATOM_NIL);
    pl_cell_t result = nil;
    for (int i = m - 1; i >= 0; i--) {
        pl_cell_t *blk = (pl_cell_t *)PL_CELL_ALLOC(2 * sizeof(pl_cell_t));
        blk[0] = pl_make_ref(elems[out_idx[i]], (int)elems[out_idx[i]]->slen); blk[1] = result;
        result = pl_make_compound(dot_id, 2, blk);
    }
    if (!plc_unify_cells_cx((pl_cell_t *)result_cell, &result, cx)) { return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_pairs_extract(void *list_cell, pl_cell_t **elems, int dot_id, int dash_id) {
    pl_cell_t *cur = list_cell ? pl_deref((pl_cell_t *)list_cell) : (pl_cell_t *)0;
    int n = 0;
    if (plc_list_cells(list_cell, dot_id) < 0) return -1;
    while (cur && (int)cur->v == DT_PLREF && plc_fid_name(cur->slen) == dot_id && plc_fid_arity(cur->slen) == 2) {
        pl_cell_t *aa = (pl_cell_t *)cur->p;
        pl_cell_t *pr = pl_deref(&aa[0]);
        if (!(pr && (int)pr->v == DT_PLREF && plc_fid_name(pr->slen) == dash_id && plc_fid_arity(pr->slen) == 2)) return -1;
        elems[n++] = pr;
        cur = pl_deref(&aa[1]);
    }
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_plain_list_extract(void *list_cell, pl_cell_t **elems, int dot_id) {
    pl_cell_t *cur = list_cell ? pl_deref((pl_cell_t *)list_cell) : (pl_cell_t *)0;
    int n = 0;
    if (plc_list_cells(list_cell, dot_id) < 0) return -1;
    while (cur && (int)cur->v == DT_PLREF && plc_fid_name(cur->slen) == dot_id && plc_fid_arity(cur->slen) == 2) {
        pl_cell_t *aa = (pl_cell_t *)cur->p;
        elems[n++] = pl_deref(&aa[0]);
        cur = pl_deref(&aa[1]);
    }
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static pl_cell_t plc_nil_cell(void) { return pl_make_atom(ATOM_NIL); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static pl_cell_t plc_cons(int fid, pl_cell_t head, pl_cell_t tail) { pl_cell_t *blk = (pl_cell_t *)PL_CELL_ALLOC(2 * sizeof(pl_cell_t)); blk[0] = head; blk[1] = tail; return pl_make_compound(fid, 2, blk); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_bag_prep_cell(int is_setof, void *list_cell, void *result_cell)
{
    int dot_id = prolog_atom_intern("."); int dash_id = prolog_atom_intern("-");
    pl_cell_t *elems[plc_list_len(list_cell, dot_id) + 1];
    int n = plc_pairs_extract(list_cell, elems, dot_id, dash_id);
    if (n < 0) return 0;
    for (int i = 1; i < n; i++) {
        pl_cell_t *key = elems[i]; int j = i - 1;
        if (is_setof) { while (j >= 0 && rt_pl_cell_compare(elems[j], key) > 0) { elems[j + 1] = elems[j]; j--; } }
        else { while (j >= 0 && rt_pl_cell_compare((pl_cell_t *)elems[j]->p, (pl_cell_t *)key->p) > 0) { elems[j + 1] = elems[j]; j--; } }
        elems[j + 1] = key;
    }
    int m = 0; int out_idx[n + 1];
    for (int i = 0; i < n; i++) {
        if (is_setof && m > 0 && rt_pl_cell_compare(elems[out_idx[m - 1]], elems[i]) == 0) continue;
        out_idx[m++] = i;
    }
    pl_cell_t nil = pl_make_atom(ATOM_NIL);
    pl_cell_t result = nil;
    for (int i = m - 1; i >= 0; i--) {
        pl_cell_t *blk = (pl_cell_t *)PL_CELL_ALLOC(2 * sizeof(pl_cell_t));
        blk[0] = pl_make_ref(elems[out_idx[i]], (int)elems[out_idx[i]]->slen); blk[1] = result;
        result = pl_make_compound(dot_id, 2, blk);
    }
    if (!pl_unify((pl_cell_t *)result_cell, &result)) { return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_keysort_cell(void *list_cell, void *result_cell, pl_tr_ctx_t *cx)
{
    int dot_id = prolog_atom_intern("."); int dash_id = prolog_atom_intern("-");
    pl_cell_t *elems[plc_list_len(list_cell, dot_id) + 1];
    int n = plc_pairs_extract(list_cell, elems, dot_id, dash_id);
    if (n < 0) return 0;
    for (int i = 1; i < n; i++) {
        pl_cell_t *key = elems[i]; pl_cell_t *kkey = (pl_cell_t *)key->p; int j = i - 1;
        while (j >= 0 && rt_pl_cell_compare((pl_cell_t *)elems[j]->p, kkey) > 0) { elems[j + 1] = elems[j]; j--; }
        elems[j + 1] = key;
    }
    pl_cell_t nil = pl_make_atom(ATOM_NIL);
    pl_cell_t result = nil;
    for (int i = n - 1; i >= 0; i--) {
        pl_cell_t *blk = (pl_cell_t *)PL_CELL_ALLOC(2 * sizeof(pl_cell_t));
        blk[0] = pl_make_ref(elems[i], (int)elems[i]->slen); blk[1] = result;
        result = pl_make_compound(dot_id, 2, blk);
    }
    if (!plc_unify_cells_cx((pl_cell_t *)result_cell, &result, cx)) { return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_inplace_cmp(int mode, pl_cell_t *vals, int a, int b) {
    return mode == 2 ? rt_pl_cell_compare((pl_cell_t *)vals[a].p, (pl_cell_t *)vals[b].p) : rt_pl_cell_compare(&vals[a], &vals[b]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_sort_in_place_cell(int mode, void *list_cell)
{
    int dot_id = prolog_atom_intern("."); int m = 0; long nl = plc_list_cells(list_cell, dot_id); int n = nl > 0 ? (int)nl : 0;
    pl_cell_t *cur;
    if (nl < 0) return 0;
    if (n == 0) return 1;
    pl_cell_t *kids[n]; pl_cell_t vals[n]; int ordb[n], tmpb[n]; int *ord = ordb, *tmp = tmpb;
    cur = pl_deref((pl_cell_t *)list_cell);
    for (int i = 0; i < n; i++) {
        pl_cell_t *d;
        kids[i] = (pl_cell_t *)cur->p; d = pl_deref(&kids[i][0]);
        vals[i] = pl_cell_unbound(d) ? pl_make_ref(d, (int)d->slen) : *d;
        ord[i] = i; cur = pl_deref(&kids[i][1]);
    }
    for (int w = 1; w < n; w *= 2) {
        for (int lo = 0; lo < n; lo += 2 * w) {
            int mid = lo + w < n ? lo + w : n, hi = lo + 2 * w < n ? lo + 2 * w : n, a = lo, b = mid, k = lo;
            while (a < mid && b < hi) tmp[k++] = plc_inplace_cmp(mode, vals, ord[a], ord[b]) <= 0 ? ord[a++] : ord[b++];
            while (a < mid) tmp[k++] = ord[a++];
            while (b < hi) tmp[k++] = ord[b++];
        }
        { int *t = ord; ord = tmp; tmp = t; }
    }
    for (int i = 0; i < n; i++) {
        if (mode == 0 && m > 0 && rt_pl_cell_compare(&vals[ord[m - 1]], &vals[ord[i]]) == 0) continue;
        ord[m++] = ord[i];
    }
    for (int i = 0; i < m; i++) kids[i][0] = vals[ord[i]];
    if (m < n) kids[m - 1][1] = plc_nil_cell();
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_group_pairs_by_key_cell(void *list_cell, void *result_cell)
{
    int dot_id = prolog_atom_intern("."); int dash_id = prolog_atom_intern("-");
    pl_cell_t *elems[plc_list_len(list_cell, dot_id) + 1];
    int n = plc_pairs_extract(list_cell, elems, dot_id, dash_id);
    if (n < 0) return 0;
    pl_cell_t groups[n + 1]; int ng = 0;
    pl_cell_t nil = plc_nil_cell();
    int i = 0;
    while (i < n) {
        pl_cell_t *key = pl_deref(&((pl_cell_t *)elems[i]->p)[0]);
        pl_cell_t *vlist[n - i]; int nv = 0; int j = i;
        while (j < n && rt_pl_cell_compare(pl_deref(&((pl_cell_t *)elems[j]->p)[0]), key) == 0) { vlist[nv++] = &((pl_cell_t *)elems[j]->p)[1]; j++; }
        pl_cell_t vals = nil;
        for (int k = nv - 1; k >= 0; k--) vals = plc_cons(dot_id, pl_make_ref(vlist[k], (int)vlist[k]->slen), vals);
        groups[ng++] = plc_cons(dash_id, pl_make_ref(key, (int)key->slen), vals);
        i = j;
    }
    pl_cell_t result = nil;
    for (int k = ng - 1; k >= 0; k--) result = plc_cons(dot_id, groups[k], result);
    if (!pl_unify((pl_cell_t *)result_cell, &result)) { return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_pairs_keys_values_cell(void *pairs_cell, void *keys_cell, void *values_cell)
{
    int dot_id = prolog_atom_intern("."); int dash_id = prolog_atom_intern("-");
    pl_cell_t *pd = pairs_cell ? pl_deref((pl_cell_t *)pairs_cell) : (pl_cell_t *)0;
    if (pd && !pl_cell_unbound(pd)) {
        pl_cell_t *elems[plc_list_len(pairs_cell, dot_id) + 1]; int n = plc_pairs_extract(pairs_cell, elems, dot_id, dash_id);
        if (n < 0) return 0;
        pl_cell_t keys = plc_nil_cell(), vals = plc_nil_cell();
        for (int i = n - 1; i >= 0; i--) {
            pl_cell_t *aa = (pl_cell_t *)elems[i]->p;
            keys = plc_cons(dot_id, pl_make_ref(&aa[0], (int)aa[0].slen), keys);
            vals = plc_cons(dot_id, pl_make_ref(&aa[1], (int)aa[1].slen), vals);
        }
        if (!pl_unify((pl_cell_t *)keys_cell, &keys) || !pl_unify((pl_cell_t *)values_cell, &vals)) { return 0; }
        return 1;
    }
    pl_cell_t *kelems[plc_list_len(keys_cell, dot_id) + 1]; int nk = plc_plain_list_extract(keys_cell, kelems, dot_id);
    pl_cell_t *velems[plc_list_len(values_cell, dot_id) + 1]; int nv = plc_plain_list_extract(values_cell, velems, dot_id);
    if (nk < 0 || nv < 0 || nk != nv) return 0;
    pl_cell_t result = plc_nil_cell();
    for (int i = nk - 1; i >= 0; i--) {
        pl_cell_t pair = plc_cons(dash_id, pl_make_ref(kelems[i], (int)kelems[i]->slen), pl_make_ref(velems[i], (int)velems[i]->slen));
        result = plc_cons(dot_id, pair, result);
    }
    if (!pl_unify((pl_cell_t *)pairs_cell, &result)) { return 0; }
    return 1;
}
typedef struct { pl_cell_t *rest; int mark; } pl_baggrp_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { long counter; int var_id; pl_tr_ctx_t *cx; } plc_nv_t;
static int plc_nv_visit(pl_cell_t *d, void *ctx)
{
    plc_nv_t *k = (plc_nv_t *)ctx;
    if (pl_cell_unbound(d)) { pl_cell_t *a = (pl_cell_t *)rt_ws_alloc_descr(1); *a = pl_make_int(k->counter++); plc_bind_cx(d, pl_make_compound(k->var_id, 1, a), k->cx); }
    return PLR_V_GO;
}
PLR_WALK1(plc_nv_walk, plc_nv_visit, pl_deref)
static long pl_numbervars_walk(pl_cell_t *c, long counter, int var_id, pl_tr_ctx_t *cx)
{
    plc_nv_t k; k.counter = counter; k.var_id = var_id; k.cx = cx;
    (void)plc_nv_walk(c, (void *)&k);
    return k.counter;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_numbervars_cell(void *term_cell, void *start_cell, void *end_cell, pl_tr_ctx_t *cx) {
    pl_cell_t *st = pl_deref((pl_cell_t *)start_cell);
    if (!st || (int)st->v != DT_I) return 0;
    long counter = (long)st->i;
    int var_id = prolog_atom_intern("$VAR");
    counter = pl_numbervars_walk((pl_cell_t *)term_cell, counter, var_id, cx);
    pl_cell_t endv = pl_make_int((int64_t)counter);
    if (!plc_unify_cells_cx((pl_cell_t *)end_cell, &endv, cx)) { return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_numbervars1_cell(void *term_cell, pl_tr_ctx_t *cx) {
    int var_id = prolog_atom_intern("$VAR");
    pl_numbervars_walk((pl_cell_t *)term_cell, 0, var_id, cx);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_get_print_stream_cell(void *stream_cell) {
    extern int fh_current_output(void);
    int stream_id = prolog_atom_intern("$stream");
    pl_cell_t *arg = (pl_cell_t *)rt_ws_alloc_descr(1); *arg = pl_make_int(fh_current_output());
    pl_cell_t sc = pl_make_compound(stream_id, 1, arg);
    if (!pl_unify((pl_cell_t *)stream_cell, &sc)) { return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_distinct_vars_walk(pl_cell_t *c, pl_cell_t **pool, int *counts, int *pool_n) { plc_tv_t t; t.pool = pool; t.n = pool_n; t.counts = counts; if (c) (void)plc_tv_walk(c, (void *)&t); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *pl_atom_text(pl_cell_t *d) {
    if ((int)d->v == DT_S || d->v == DT_SNUL) return d->s ? d->s : "";
    if ((int)d->v == DT_PLATOM) { const char *nm = prolog_atom_name((int)d->i); return nm ? nm : ""; }
    return (const char *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_is_nil(pl_cell_t *d) { const char *s = pl_atom_text(d); return s && !strcmp(s, "[]"); }
static pl_cell_t pl_nil_cell(void) { return pl_make_atom(ATOM_NIL); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_name_singleton_vars_cell(void *term_cell) {
    int occ = plc_var_occ((pl_cell_t *)term_cell) + 1; pl_cell_t *pool[occ]; int counts[occ]; int pn = 0;
    pl_distinct_vars_walk((pl_cell_t *)term_cell, pool, counts, &pn);
    int varname_id = prolog_atom_intern("$VARNAME"); int underscore_id = prolog_atom_intern("_");
    for (int i = 0; i < pn; i++) {
        if (counts[i] != 1) continue;
        pl_cell_t *arg = (pl_cell_t *)rt_ws_alloc_descr(1); *arg = pl_make_atom(underscore_id);
        pl_bind(pool[i], pl_make_compound(varname_id, 1, arg));
    }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_name_query_vars_cell(void *list_cell, void *rest_cell) {
    extern void rt_pl_iso_throw_instantiation(void); extern void rt_pl_iso_throw_type(const char *, DESCR_t);
    int dot_id = prolog_atom_intern("."); int varname_id = prolog_atom_intern("$VARNAME");
    if (plc_list_cells(list_cell, dot_id) < 0) { rt_pl_iso_throw_type("list", *pl_deref((pl_cell_t *)list_cell)); return 0; }
    pl_cell_t *rest_pairs[plc_list_len(list_cell, dot_id) + 1]; int nr = 0;
    pl_cell_t *cur = (pl_cell_t *)list_cell;
    for (;;) {
        pl_cell_t *d = pl_deref(cur);
        if (pl_cell_unbound(d)) { rt_pl_iso_throw_instantiation(); return 0; }
        if (pl_is_nil(d)) break;
        if ((int)d->v != DT_PLREF || pl_arity(d) != 2 || plc_functor(d) != dot_id) { rt_pl_iso_throw_type("list", *d); return 0; }
        pl_cell_t *kids = (pl_cell_t *)pl_compound_heap(d);
        pl_cell_t *pair = pl_deref(&kids[0]);
        int bound_ok = 0;
        if ((int)pair->v == DT_PLREF && pl_arity(pair) == 2) {
            const char *fn = prolog_atom_name(plc_functor(pair));
            if (fn && !strcmp(fn, "=")) {
                pl_cell_t *pk = (pl_cell_t *)pl_compound_heap(pair);
                pl_cell_t *nm = pl_deref(&pk[0]); pl_cell_t *vr = pl_deref(&pk[1]);
                const char *nm_txt = pl_atom_text(nm);
                if (nm_txt && pl_cell_unbound(vr)) {
                    pl_cell_t *arg = (pl_cell_t *)rt_ws_alloc_descr(1); *arg = *nm;
                    pl_bind(vr, pl_make_compound(varname_id, 1, arg));
                    bound_ok = 1;
                }
            }
        }
        if (!bound_ok) rest_pairs[nr++] = &kids[0];
        cur = &kids[1];
    }
    pl_cell_t result = pl_nil_cell();
    for (int i = nr - 1; i >= 0; i--) {
        pl_cell_t *blk = (pl_cell_t *)rt_ws_alloc_descr(2);
        blk[0] = *rest_pairs[i]; blk[1] = result;
        result = pl_make_compound(dot_id, 2, blk);
    }
    if (!pl_unify((pl_cell_t *)rest_cell, &result)) { return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_namevars_letters(long n, char *buf, size_t cap) {
    long letter = n % 26, block = n / 26;
    if (block == 0) snprintf(buf, cap, "%c", (int)('A' + letter));
    else snprintf(buf, cap, "%c%ld", (int)('A' + letter), block);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_bind_variables_cell(void *term_cell, void *options_cell) {
    extern void rt_pl_iso_throw_instantiation(void);
    extern void rt_pl_iso_throw_type(const char *, DESCR_t); extern void rt_pl_iso_throw_domain(const char *, DESCR_t);
    int dot_id = prolog_atom_intern("."); int var_id = prolog_atom_intern("$VAR"); int varname_id = prolog_atom_intern("$VARNAME");
    int use_namevars = 0; long from_n = 0; pl_cell_t *next_var = (pl_cell_t *)0; pl_cell_t *exclude_list = (pl_cell_t *)0;
    pl_cell_t *cur = (pl_cell_t *)options_cell;
    if (plc_list_cells(options_cell, dot_id) < 0) { rt_pl_iso_throw_type("list", *pl_deref((pl_cell_t *)options_cell)); return 0; }
    for (;;) {
        pl_cell_t *d = pl_deref(cur);
        if (pl_cell_unbound(d)) { rt_pl_iso_throw_instantiation(); return 0; }
        if (pl_is_nil(d)) break;
        if ((int)d->v != DT_PLREF || pl_arity(d) != 2 || plc_functor(d) != dot_id) { rt_pl_iso_throw_type("list", *d); return 0; }
        pl_cell_t *kids = (pl_cell_t *)pl_compound_heap(d);
        pl_cell_t *opt = pl_deref(&kids[0]);
        if (pl_cell_unbound(opt)) { rt_pl_iso_throw_instantiation(); return 0; }
        const char *opt_txt = pl_atom_text(opt);
        if (opt_txt) {
            if (!strcmp(opt_txt, "numbervars")) use_namevars = 0;
            else if (!strcmp(opt_txt, "namevars")) use_namevars = 1;
            else { rt_pl_iso_throw_domain("var_binding_option", *opt); return 0; }
        } else if ((int)opt->v == DT_PLREF && pl_arity(opt) == 1) {
            const char *f = prolog_atom_name(plc_functor(opt)); pl_cell_t *oa = (pl_cell_t *)pl_compound_heap(opt); pl_cell_t *av = pl_deref(&oa[0]);
            if (f && !strcmp(f, "from")) { if (pl_cell_unbound(av)) { rt_pl_iso_throw_instantiation(); return 0; } if ((int)av->v != DT_I) { rt_pl_iso_throw_domain("var_binding_option", *opt); return 0; } from_n = (long)av->i; }
            else if (f && !strcmp(f, "next")) { if (!pl_cell_unbound(av) && (int)av->v != DT_I) { rt_pl_iso_throw_domain("var_binding_option", *opt); return 0; } next_var = &oa[0]; }
            else if (f && !strcmp(f, "exclude")) { exclude_list = &oa[0]; }
            else { rt_pl_iso_throw_domain("var_binding_option", *opt); return 0; }
        } else { rt_pl_iso_throw_domain("var_binding_option", *opt); return 0; }
        cur = &kids[1];
    }
    if (exclude_list && plc_list_cells(exclude_list, dot_id) < 0) { rt_pl_iso_throw_type("list", *pl_deref(exclude_list)); return 0; }
    long used[exclude_list ? plc_list_len(exclude_list, dot_id) + 1 : 1]; int nused = 0;
    if (exclude_list) {
        pl_cell_t *ecur = exclude_list;
        for (;;) {
            pl_cell_t *ed = pl_deref(ecur);
            if (pl_is_nil(ed)) break;
            if ((int)ed->v != DT_PLREF || pl_arity(ed) != 2 || plc_functor(ed) != dot_id) break;
            pl_cell_t *ekids = (pl_cell_t *)pl_compound_heap(ed);
            pl_cell_t *el = pl_deref(&ekids[0]);
            if ((int)el->v == DT_PLREF && pl_arity(el) == 1 && plc_functor(el) == var_id) {
                pl_cell_t *ea = (pl_cell_t *)pl_compound_heap(el); pl_cell_t *ev = pl_deref(&ea[0]);
                if ((int)ev->v == DT_I) used[nused++] = (long)ev->i;
            }
            ecur = &ekids[1];
        }
    }
    int occ = plc_var_occ((pl_cell_t *)term_cell) + 1; pl_cell_t *pool[occ]; int dummy_counts[occ]; int pn = 0;
    pl_distinct_vars_walk((pl_cell_t *)term_cell, pool, dummy_counts, &pn);
    long n = from_n;
    for (int i = 0; i < pn; i++) {
        for (;;) { int clash = 0; for (int k = 0; k < nused; k++) if (used[k] == n) { clash = 1; break; } if (!clash) break; n++; }
        pl_cell_t *arg = (pl_cell_t *)rt_ws_alloc_descr(1);
        if (use_namevars) { char nb[32]; pl_namevars_letters(n, nb, sizeof nb); *arg = pl_make_atom(prolog_atom_intern(nb)); pl_bind(pool[i], pl_make_compound(varname_id, 1, arg)); }
        else { *arg = pl_make_int(n); pl_bind(pool[i], pl_make_compound(var_id, 1, arg)); }
        n++;
    }
    if (next_var) {
        pl_cell_t nv = pl_make_int(n);
        if (!pl_unify(next_var, &nv)) { return 0; }
    }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define PL_ACYC_ON_PATH ((DTYPE_t)(DT_PLREF + 1))
static int pl_acyclic_walk(pl_cell_t *c)
{
    pl_cell_t *first = (pl_cell_t *)0; long nmarked = 0; int ret = 1;
    for (;;) {
        pl_cell_t *d = pl_deref(c);
        if (d->v == PL_ACYC_ON_PATH) { ret = 0; break; }
        if ((int)d->v != DT_PLREF) break;
        int ar = pl_arity(d); pl_cell_t *aa = (pl_cell_t *)pl_compound_heap(d);
        if (ar <= 0) break;
        d->v = PL_ACYC_ON_PATH; if (!nmarked++) first = d;
        int bad = 0; for (int i = 0; i < ar - 1; i++) if (!pl_acyclic_walk(&aa[i])) { bad = 1; break; }
        if (bad) { ret = 0; break; }
        c = &aa[ar - 1];
    }
    for (pl_cell_t *d = first; nmarked > 0; nmarked--) { d->v = (DTYPE_t)DT_PLREF; if (nmarked > 1) d = pl_deref(&((pl_cell_t *)pl_compound_heap(d))[pl_arity(d) - 1]); }
    return ret;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_acyclic_cell(void *term_cell)
{
    return term_cell ? pl_acyclic_walk((pl_cell_t *)term_cell) : 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_term_string_cell(void *term_cell, void *str_cell, int as_str, pl_tr_ctx_t *cx)
{
    extern FILE *fh_memsink_open(char **, size_t *); char *buf = (char *)0; size_t len = 0;
    FILE *ms = fh_memsink_open(&buf, &len);
    if (!ms) { return 0; }
    plc_vmap m; m.n = 0; m.vnc = 0; m.fp = ms; m.portray = (int (*)(pl_cell_t *, plc_vmap *))0; m.pthrown = (void *)0; m.cx = (pl_tr_ctx_t *)0; m.pheld = 0;
    plc_writeq((pl_cell_t *)term_cell, &m);
    if (fclose(ms) != 0) { return 0; }
    return plc_text_out(str_cell, buf ? buf : "", as_str, cx);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { plr_stk_t ag, lg; pl_cell_t *s; pl_cell_t *d; } plc_cp_t;
static int plc_copy_run(void *wv)
{
    plc_cp_t *w = (plc_cp_t *)wv;
    for (;;) {
        pl_cell_t *d = plr_resolve(pl_deref(w->s));
        if (d->v == PLR_COPY) *w->d = pl_make_compound(plc_fid_name(d->slen), plc_fid_arity(d->slen), d->p);
        else if (d->v == PLR_VCOPY) { pl_cell_t *f = (pl_cell_t *)d->p; *w->d = pl_make_ref(f, (int)f->slen); }
        else if (pl_cell_unbound(d)) {
            pl_cell_t *fresh; uint64_t w0;
            if (!plr_room(&w->lg)) return plr_more(&w->ag, &w->lg, plc_copy_run, w);
            fresh = (pl_cell_t *)rt_ws_alloc_descr(1);
            if (!fresh) *w->d = *d;
            else { pl_init_var(fresh, -1); memcpy(&w0, d, 8); plr_push(&w->lg, d, d->p, w0); d->v = PLR_VCOPY; d->p = (void *)fresh; *w->d = pl_make_ref(fresh, (int)fresh->slen); }
        } else if ((int)d->v == DT_PLREF) {
            int fn = plc_fid_name(d->slen), ar = plc_fid_arity(d->slen); pl_cell_t *aa = (pl_cell_t *)d->p, *na; uint64_t w0;
            if (!plr_room(&w->lg) || (ar > 0 && !plr_room(&w->ag))) return plr_more(&w->ag, &w->lg, plc_copy_run, w);
            na = (pl_cell_t *)rt_ws_alloc_descr((size_t)(ar > 0 ? ar : 1));
            if (!na) *w->d = *d;
            else {
                memcpy(&w0, d, 8); *w->d = pl_make_compound(fn, ar, na); plr_push(&w->lg, d, (void *)aa, w0); d->v = PLR_COPY; d->p = (void *)na;
                if (ar > 0) plr_push(&w->ag, aa, na, (uint64_t)ar);
            }
        } else *w->d = *d;
        if (plr_empty(&w->ag)) break;
        { plr_ent_t *t = plr_peek(&w->ag); w->s = (pl_cell_t *)t->x; w->d = (pl_cell_t *)t->y; t->x = (void *)(w->s + 1); t->y = (void *)(w->d + 1); if (!--t->n) plr_drop(&w->ag); }
    }
    for (plr_seg_t *g = w->lg.top; g; g = g->prev) for (long i = g->n; i-- > 0; ) { pl_cell_t *c = (pl_cell_t *)g->e[i].x; uint64_t w0 = g->e[i].n; memcpy(c, &w0, 8); c->p = g->e[i].y; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static pl_cell_t plc_copy(pl_cell_t *c)
{
    pl_cell_t out; plc_cp_t w;
    memset(&out, 0, sizeof out); plr_stk_init(&w.ag); plr_stk_init(&w.lg); w.s = c; w.d = &out;
    (void)plc_copy_run(&w);
    return out;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
pl_cell_t rt_pl_cell_snapshot(void *cell)
{
    return plc_copy((pl_cell_t *)cell);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_ball_make(DESCR_t *a, int n)
{
    extern void *rt_pl_ball_instantiation(void);
    if (!a || n < 1) return (void *)0;
    if (pl_cell_unbound(pl_deref((pl_cell_t *)&a[0]))) return rt_pl_ball_instantiation();
    pl_cell_t *b = (pl_cell_t *)PL_CELL_ALLOC(sizeof(pl_cell_t));
    if (!b) return (void *)0;
    *b = rt_pl_cell_snapshot(&a[0]);
    return (void *)b;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *pl_ball_key_str(DESCR_t d)
{
    if ((int)d.v == DT_PLATOM) return prolog_atom_name((int)d.i);
    if ((int)d.v == DT_S || (int)d.v == DT_N) return d.s;
    return (const char *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_ball_existence(DESCR_t *a, int n)
{
    extern void *rt_pl_ball_existence_key(const char *key);
    return rt_pl_ball_existence_key((a && n > 0) ? pl_ball_key_str(a[0]) : (const char *)0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_ball_existence_key(const char *key)
{
    extern int rt_pl_unknown_suppress(const char *);
    int ar = 0;
    if (rt_pl_unknown_suppress(key)) return (void *)0;
    const char *sl = key ? strrchr(key, '/') : (const char *)0;
    char nm[key ? strlen(key) + 1 : 2];
    if (sl) { size_t kl = (size_t)(sl - key); memcpy(nm, key, kl); nm[kl] = 0; ar = atoi(sl + 1); }
    else { snprintf(nm, sizeof nm, "%s", key ? key : "?"); }
    pl_cell_t pi[2]; pi[0] = pl_make_atom(prolog_atom_intern(nm)); pi[1] = pl_make_int(ar);
    pl_cell_t *pic = (pl_cell_t *)rt_pl_compound_cell("/", 2, (void *)pi);
    if (!pic) return (void *)0;
    pl_cell_t ee[2]; ee[0] = pl_make_atom(prolog_atom_intern("procedure")); ee[1] = *pic;
    pl_cell_t *eec = (pl_cell_t *)rt_pl_compound_cell("existence_error", 2, (void *)ee);
    if (!eec) return (void *)0;
    pl_cell_t er[2]; er[0] = *eec; er[1] = *pic;
    return rt_pl_compound_cell("error", 2, (void *)er);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_ball_evaluable(const char *name, int arity)
{
    extern void *rt_pl_ball_type_pi(const char *kind, const char *what, const char *nm, int ar);
    return rt_pl_ball_type_pi("type_error", "evaluable", name, arity);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_ball_type_pi(const char *kind, const char *what, const char *nm, int ar)
{
    extern DESCR_t rt_pl_fresh_var_ref(void);
    pl_cell_t pi[2]; pl_cell_t *pic; pl_cell_t te[2]; pl_cell_t *tec; pl_cell_t er[2];
    pi[0] = pl_make_atom(prolog_atom_intern(nm ? nm : "?")); pi[1] = pl_make_int(ar);
    pic = (pl_cell_t *)rt_pl_compound_cell("/", 2, (void *)pi);
    if (!pic) return (void *)0;
    te[0] = pl_make_atom(prolog_atom_intern(what ? what : "evaluable")); te[1] = *pic;
    tec = (pl_cell_t *)rt_pl_compound_cell(kind ? kind : "type_error", 2, (void *)te);
    if (!tec) return (void *)0;
    er[0] = *tec; er[1] = rt_pl_fresh_var_ref();
    return rt_pl_compound_cell("error", 2, (void *)er);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_ball_eval_error(const char *kind, const char *op, int arity)
{
    extern DESCR_t rt_pl_fresh_var_ref(void);
    pl_cell_t ee[1]; pl_cell_t *eec; pl_cell_t pi[2]; pl_cell_t *pic; pl_cell_t ct[2]; pl_cell_t *ctc; pl_cell_t er[2];
    ee[0] = pl_make_atom(prolog_atom_intern(kind ? kind : "zero_divisor"));
    eec = (pl_cell_t *)rt_pl_compound_cell("evaluation_error", 1, (void *)ee);
    if (!eec) return (void *)0;
    pi[0] = pl_make_atom(prolog_atom_intern(op ? op : "is")); pi[1] = pl_make_int(arity);
    pic = (pl_cell_t *)rt_pl_compound_cell("/", 2, (void *)pi);
    if (!pic) return (void *)0;
    ct[0] = *pic; ct[1] = rt_pl_fresh_var_ref();
    ctc = (pl_cell_t *)rt_pl_compound_cell("context", 2, (void *)ct);
    if (!ctc) return (void *)0;
    er[0] = *eec; er[1] = *ctc;
    return rt_pl_compound_cell("error", 2, (void *)er);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_ball_kind2(const char *kind, const char *arg0_atom, DESCR_t culprit)
{
    extern DESCR_t rt_pl_fresh_var_ref(void);
    pl_cell_t fe[2]; pl_cell_t *fec; pl_cell_t er[2];
    fe[0] = pl_make_atom(prolog_atom_intern(arg0_atom ? arg0_atom : "term"));
    fe[1] = rt_pl_cell_snapshot(&culprit);
    fec = (pl_cell_t *)rt_pl_compound_cell(kind, 2, (void *)fe);
    if (!fec) return (void *)0;
    er[0] = *fec; er[1] = rt_pl_fresh_var_ref();
    return rt_pl_compound_cell("error", 2, (void *)er);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_ball_occurs(DESCR_t *var, DESCR_t *term)
{
    extern DESCR_t rt_pl_fresh_var_ref(void);
    pl_cell_t oc[2]; pl_cell_t *occ; pl_cell_t er[2];
    oc[0].v = (DTYPE_t)DT_PLVAR; oc[0].slen = 0; oc[0].p = (void *)var; oc[1] = *term;
    occ = (pl_cell_t *)rt_pl_compound_cell("occurs_check", 2, (void *)oc);
    if (!occ) return (void *)0;
    er[0] = rt_pl_cell_snapshot(occ); er[1] = rt_pl_fresh_var_ref();
    return rt_pl_compound_cell("error", 2, (void *)er);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_ball_kind2_pi(const char *kind, const char *arg0_atom, DESCR_t culprit, const char *nm, int ar)
{
    pl_cell_t fe[2]; pl_cell_t *fec; pl_cell_t pi[2]; pl_cell_t *pic; pl_cell_t er[2];
    fe[0] = pl_make_atom(prolog_atom_intern(arg0_atom ? arg0_atom : "term"));
    fe[1] = rt_pl_cell_snapshot(&culprit);
    fec = (pl_cell_t *)rt_pl_compound_cell(kind, 2, (void *)fe);
    if (!fec) return (void *)0;
    pi[0] = pl_make_atom(prolog_atom_intern(nm ? nm : "?")); pi[1] = pl_make_int(ar);
    pic = (pl_cell_t *)rt_pl_compound_cell("/", 2, (void *)pi);
    if (!pic) return (void *)0;
    er[0] = *fec; er[1] = *pic;
    return rt_pl_compound_cell("error", 2, (void *)er);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_ball_set_pi(void *ball, const char *nm, int ar)
{
    pl_cell_t pi[2]; pl_cell_t *pic; pl_cell_t *b = (pl_cell_t *)ball;
    if (!b || (int)b->v != DT_PLREF || !b->p) return ball;
    pi[0] = pl_make_atom(prolog_atom_intern(nm ? nm : "?")); pi[1] = pl_make_int(ar);
    pic = (pl_cell_t *)rt_pl_compound_cell("/", 2, (void *)pi);
    if (pic) ((pl_cell_t *)b->p)[1] = *pic;
    return ball;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_ball_culprit1(const char *kind, DESCR_t culprit)
{
    extern DESCR_t rt_pl_fresh_var_ref(void);
    pl_cell_t fe[1]; pl_cell_t *fec; pl_cell_t er[2];
    fe[0] = rt_pl_cell_snapshot(&culprit);
    fec = (pl_cell_t *)rt_pl_compound_cell(kind, 1, (void *)fe);
    if (!fec) return (void *)0;
    er[0] = *fec; er[1] = rt_pl_fresh_var_ref();
    return rt_pl_compound_cell("error", 2, (void *)er);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_ball_kind1(const char *kind, const char *arg0_atom)
{
    extern DESCR_t rt_pl_fresh_var_ref(void);
    pl_cell_t fe[1]; pl_cell_t *fec; pl_cell_t er[2];
    fe[0] = pl_make_atom(prolog_atom_intern(arg0_atom ? arg0_atom : "term"));
    fec = (pl_cell_t *)rt_pl_compound_cell(kind, 1, (void *)fe);
    if (!fec) return (void *)0;
    er[0] = *fec; er[1] = rt_pl_fresh_var_ref();
    return rt_pl_compound_cell("error", 2, (void *)er);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_ball_instantiation(void)
{
    extern DESCR_t rt_pl_fresh_var_ref(void);
    pl_cell_t er[2];
    er[0] = pl_make_atom(prolog_atom_intern("instantiation_error"));
    er[1] = rt_pl_fresh_var_ref();
    return rt_pl_compound_cell("error", 2, (void *)er);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_ball_permission_pi(const char *op, const char *type, const char *nm, int ar)
{
    extern DESCR_t rt_pl_fresh_var_ref(void);
    pl_cell_t pi[2]; pl_cell_t *pic; pl_cell_t pe[3]; pl_cell_t *pec; pl_cell_t er[2];
    pi[0] = pl_make_atom(prolog_atom_intern(nm ? nm : "?")); pi[1] = pl_make_int(ar);
    pic = (pl_cell_t *)rt_pl_compound_cell("/", 2, (void *)pi);
    if (!pic) return (void *)0;
    pe[0] = pl_make_atom(prolog_atom_intern(op ? op : "modify"));
    pe[1] = pl_make_atom(prolog_atom_intern(type ? type : "static_procedure"));
    pe[2] = *pic;
    pec = (pl_cell_t *)rt_pl_compound_cell("permission_error", 3, (void *)pe);
    if (!pec) return (void *)0;
    er[0] = *pec; er[1] = rt_pl_fresh_var_ref();
    return rt_pl_compound_cell("error", 2, (void *)er);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_ball_permission3(const char *op, const char *type, DESCR_t culprit)
{
    extern DESCR_t rt_pl_fresh_var_ref(void);
    pl_cell_t pe[3]; pl_cell_t *pec; pl_cell_t er[2];
    pe[0] = pl_make_atom(prolog_atom_intern(op ? op : "access"));
    pe[1] = pl_make_atom(prolog_atom_intern(type ? type : "stream"));
    pe[2] = rt_pl_cell_snapshot(&culprit);
    pec = (pl_cell_t *)rt_pl_compound_cell("permission_error", 3, (void *)pe);
    if (!pec) return (void *)0;
    er[0] = *pec; er[1] = rt_pl_fresh_var_ref();
    return rt_pl_compound_cell("error", 2, (void *)er);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_root_omega(void)
{
    extern void *rt_pl_ball_take(void);
    void *ball = rt_pl_ball_take();
    if (!ball) exit(0);
    rt_pl_ball_report(ball);
    exit(2);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_ball_report(void *ball)
{
    extern void rt_pl_write_cell_fp(void *, FILE *);
    fflush(stdout);
    fprintf(stderr, "Warning: goal raised exception: ");
    if (ball) rt_pl_write_cell_fp(ball, stderr); else fprintf(stderr, "unknown");
    fprintf(stderr, "\n");
    fflush(stderr);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_cell_atom_id(pl_cell_t *c)
{
    pl_cell_t *d = pl_deref(c);
    if ((int)d->v == DT_PLATOM) return (int)d->i;
    if ((int)d->v == DT_S) { extern int prolog_atom_intern(const char *); return prolog_atom_intern(d->s ? d->s : ""); }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_copy_term_cell(void *term_cell, void *copy_cell, pl_tr_ctx_t *cx)
{
    pl_cell_t copy = plc_copy((pl_cell_t *)term_cell);
    if (!plc_unify_cells_cx((pl_cell_t *)copy_cell, &copy, cx)) { return 0; }
    return 1;
}
static inline pl_cell_t *fa_items(DESCR_t *a) { return (pl_cell_t *)a[1].ptr; }
static inline int fa_n(DESCR_t *a) { return (int)a[0].i; }
static inline int fa_cap(DESCR_t *a) { return (int)a[0].slen; }
static inline void fa_set(DESCR_t *a, int n, int cap) { a[0].v = (DTYPE_t)DT_I; a[0].slen = (uint32_t)cap; a[0].i = (int64_t)n; }
static inline void fa_items_set(DESCR_t *a, pl_cell_t *p) { a[1].v = (DTYPE_t)DT_DATA; a[1].slen = DATA_ELEMS_SLEN; a[1].ptr = (void *)p; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void * rt_pl_findall_begin(void)
{
    DESCR_t *a = (DESCR_t *)rt_ws_alloc_descr(2);
    if (!a) return (void *)0;
    { pl_cell_t *it = (pl_cell_t *)rt_ws_alloc_descr(16); if (!it) return (void *)0; fa_set(a, 0, 16); fa_items_set(a, it); }
    return (void *)a;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_findall_collect(void *acc_v, void *tmpl_term)
{
    DESCR_t *a = (DESCR_t *)acc_v;
    if (!a || !fa_items(a)) return;
    if (fa_n(a) >= fa_cap(a)) {
        int nc = fa_cap(a) * 2; pl_cell_t *ni = (pl_cell_t *)rt_ws_alloc_descr((size_t)nc); if (!ni) return;
        for (int i = 0; i < fa_n(a); i++) ni[i] = fa_items(a)[i]; fa_items_set(a, ni); fa_set(a, fa_n(a), nc);
    }
    { int n = fa_n(a); pl_cell_t item = plc_copy((pl_cell_t *)tmpl_term); fa_items(a)[n] = item; fa_set(a, n + 1, fa_cap(a)); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_findall_count(void *acc_v)
{
    DESCR_t *a = (DESCR_t *)acc_v;
    return a ? fa_n(a) : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_findall_item(void *acc_v, int i, void *out)
{
    DESCR_t *a = (DESCR_t *)acc_v;
    if (!a || !out || i < 0 || i >= fa_n(a)) return;
    *(pl_cell_t *)out = fa_items(a)[i];
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_findall_finish(void *acc_v, void *result_term)
{
    DESCR_t *a = (DESCR_t *)acc_v;
    int dot = prolog_atom_intern(".");
    pl_cell_t lst; { extern const char *prolog_atom_name(int); const char *nm = prolog_atom_name(prolog_atom_intern("[]"));
        lst = pl_make_atom(prolog_atom_intern(nm ? nm : "")); }
    int n = a ? fa_n(a) : 0;
    for (int i = n - 1; i >= 0; i--) {
        pl_cell_t *c = (pl_cell_t *)rt_ws_alloc_descr(2); if (!c) return 0;
        c[0] = fa_items(a)[i]; c[1] = lst;
        lst = pl_make_compound(dot, 2, c);
    }
    if (!pl_unify((pl_cell_t *)result_term, &lst)) { return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_agg_count_finish(void *acc_v, void *result_term)
{
    DESCR_t *a = (DESCR_t *)acc_v;
    pl_cell_t cnt = pl_make_int((int64_t)(a ? fa_n(a) : 0));
    if (!pl_unify((pl_cell_t *)result_term, &cnt)) { return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int agg_num(pl_cell_t *c, long *iv, double *dv, int *isf)
{
    pl_cell_t *d = c ? pl_deref(c) : (pl_cell_t *)0;
    if (!d) return 0;
    if ((int)d->v == DT_I) { *iv = (long)d->i; *isf = 0; return 1; }
    if ((int)d->v == DT_R) { *dv = d->r;       *isf = 1; return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_agg_sum_finish(void *acc_v, void *result_term)
{
    DESCR_t *a = (DESCR_t *)acc_v;
    int n = a ? fa_n(a) : 0;
    long si = 0; double sd = 0.0; int isf = 0;
    for (int i = 0; i < n; i++) {
        long iv = 0; double dv = 0.0; int ef = 0;
        if (!agg_num(&fa_items(a)[i], &iv, &dv, &ef)) return 0;
        if (ef) { sd += dv; isf = 1; } else { si += iv; sd += (double)iv; }
    }
    pl_cell_t r = isf ? pl_make_float(sd) : pl_make_int((int64_t)si);
    if (!pl_unify((pl_cell_t *)result_term, &r)) { return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_pl_agg_minmax_finish(void *acc_v, void *result_term, int want_max)
{
    DESCR_t *a = (DESCR_t *)acc_v;
    int n = a ? fa_n(a) : 0;
    if (n <= 0) return 0;
    long bi = 0; double bd = 0.0; int isf = 0, have = 0;
    for (int i = 0; i < n; i++) {
        long iv = 0; double dv = 0.0; int ef = 0;
        if (!agg_num(&fa_items(a)[i], &iv, &dv, &ef)) return 0;
        double cur = ef ? dv : (double)iv;
        double best = isf ? bd : (double)bi;
        if (!have || (want_max ? (cur > best) : (cur < best))) { if (ef) { bd = dv; isf = 1; } else { bi = iv; bd = (double)iv; isf = 0; } have = 1; }
    }
    pl_cell_t r = isf ? pl_make_float(bd) : pl_make_int((int64_t)bi);
    if (!pl_unify((pl_cell_t *)result_term, &r)) { return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_agg_max_finish(void *acc_v, void *result_term) { return rt_pl_agg_minmax_finish(acc_v, result_term, 1); }
int rt_pl_agg_min_finish(void *acc_v, void *result_term) { return rt_pl_agg_minmax_finish(acc_v, result_term, 0); }
typedef struct dyn_clause { pl_cell_t *head; pl_cell_t *body; struct dyn_clause *next; } dyn_clause_t;
typedef struct { dyn_clause_t *next; } dyn_cursor_t;
typedef struct { dyn_clause_t *cur; int mark; } pl_dyn_it_t;
typedef struct { dyn_clause_t *cur; int mark; } pl_clause_it_t;
typedef struct { const char *name; long arity; } pl_pi_cand_t;
typedef struct { pl_pi_cand_t *v; int n; int i; int mark; } pl_curpred_it_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static pl_cell_t pl_atom_cell(const char *a) { return pl_make_atom(prolog_atom_intern(a)); }
typedef struct { const char *name; int prec; const char *type; } pl_op_cand_t;
typedef struct { pl_op_cand_t *v; int n; int i; int mark; } pl_curop_it_t;
typedef struct { int i; int mark; } pl_flagit_t;
typedef struct { int si; int pi; int mark; } pl_spropit_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define PL_DB_CELL0   24
typedef struct { pl_cell_t cl; int erased; int ref; int ridx; int chain_off; int next_idx; int pad; } pl_db_slot_t;
typedef struct { pl_db_slot_t *s; int n; int cap; int killed; int next_ref; int head_idx; int tail_idx; int cell_k; int pad; } pl_db_t;
_Static_assert(sizeof(pl_db_slot_t) == 40 && __builtin_offsetof(pl_db_slot_t, erased) == 16 && __builtin_offsetof(pl_db_slot_t, ref) == 20 && __builtin_offsetof(pl_db_slot_t, ridx) == 24 && __builtin_offsetof(pl_db_slot_t, chain_off) == 28 && __builtin_offsetof(pl_db_slot_t, next_idx) == 32, "xa_flat.cpp's chain-omega and bb_to.cpp's db walk bake the slot layout (ARCH-PROLOG-C-OUT-OF-THE-BOX 5.2 A/B)");
_Static_assert(__builtin_offsetof(pl_db_t, s) == 0 && __builtin_offsetof(pl_db_t, n) == 8 && __builtin_offsetof(pl_db_t, next_ref) == 20 && __builtin_offsetof(pl_db_t, head_idx) == 24, "xa_flat.cpp's packet entry and bb_to.cpp's db walk bake the store layout (ARCH-PROLOG-C-OUT-OF-THE-BOX 5.2 A/B)");
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void ** pl_db_cell_addr(void *root, int64_t k, int create);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void * rt_pl_db_get(void *root, int64_t k)
{
    if (!root || k < 0) return (void *)0;
    { pl_db_t **cell = (pl_db_t **)pl_db_cell_addr(root, k, 1);
      if (!cell) return (void *)0;
      if (!*cell) {
          pl_db_t *d = (pl_db_t *)rt_pl_struct_alloc(HB_PLDB, sizeof *d);
          if (!d) return (void *)0;
          d->cap = 8; d->n = 0; d->killed = 0; d->next_ref = 1; d->head_idx = -1; d->tail_idx = -1; d->cell_k = (int)k; d->pad = 0; d->s = (pl_db_slot_t *)rt_pl_struct_alloc(HB_PLDBS, (size_t)d->cap * sizeof(pl_db_slot_t));
          if (!d->s) { d->cap = 0; }
          *cell = d;
      }
      return (void *)*cell; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_nb_set(void *root, int64_t k, void *val)
{
    extern void *rt_ws_alloc_descr(size_t);
    if (!root || k < 0 || !val) return 0;
    { pl_cell_t **cell = (pl_cell_t **)pl_db_cell_addr(root, k, 1);
      if (!cell) return 0;
      pl_cell_t *t = pl_deref((pl_cell_t *)val);
      pl_cell_t stored = plc_copy(t);
      pl_cell_t *box = (pl_cell_t *)rt_ws_alloc_descr(1);
      if (!box) return 0;
      *box = stored; *cell = box; return 1; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_b_set(void *root, int64_t k, void *val, pl_tr_ctx_t *cx)
{
    extern void *rt_ws_alloc_descr(size_t);
    if (!root || k < 0 || !val || !cx) return 0;
    { pl_cell_t **cell = (pl_cell_t **)pl_db_cell_addr(root, k, 1);
      if (!cell) return 0;
      pl_cell_t *t = pl_deref((pl_cell_t *)val);
      pl_cell_t stored = plc_copy(t);
      if (*cell) {
          char probe; char *floor_ = &probe;
          if (pl_tr_needs_log(cx, *cell, floor_)) pl_tr_push(cx, *cell);
          **cell = stored;
      } else {
          pl_cell_t *box = (pl_cell_t *)rt_ws_alloc_descr(1);
          if (!box) return 0;
          *box = stored; *cell = box;
      }
      return 1; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_nb_get_cell(void *root, int64_t k, void *out_cell, pl_tr_ctx_t *cx)
{
    if (!root || k < 0 || !out_cell) return 0;
    { pl_cell_t **cell = (pl_cell_t **)pl_db_cell_addr(root, k, 0); pl_cell_t *box = cell ? *cell : (pl_cell_t *)0;
      if (!box) return 0;
      return plc_unify_cells_cx((pl_cell_t *)out_cell, box, cx) ? 1 : 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_nb_is_set(void *root, int64_t k)
{
    if (!root || k < 0) return 0;
    { pl_cell_t **cell = (pl_cell_t **)pl_db_cell_addr(root, k, 0); return cell && *cell != (pl_cell_t *)0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define PL_DB_REGISTRY_CELL 0
#define PL_DB_KEY_VLA(buf, nm, ar) char buf[fmt_len("%s/%d", (nm), (int)(ar))]; snprintf(buf, sizeof buf, "%s/%d", (nm), (int)(ar))
typedef struct { char *key; int k; pl_db_t *db; int stat; int decl; } pl_db_key_t;
typedef struct { pl_db_key_t *e; int n; int cap; int next_cell; int ovf_cap; void **ovf; } pl_db_reg_t;
_Static_assert(__builtin_offsetof(pl_db_reg_t, ovf) == 24, "xa_flat.cpp's packet road loads a root cell numbered PL_DB_FRAME_CELLS or more through [registry + 24], the overflow vector -- the cells past the root frame live there, in an HB_PVEC that grows by doubling");
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void pl_db_gc_visit(uint16_t type, void *p, size_t bytes)
{
    extern void rt_gc_visit_raw(const char **); extern void rt_gc_visit_descr(DESCR_t *);
    if (type == HB_PLDB) { pl_db_t *d = (pl_db_t *)p; if (d->s) rt_gc_visit_raw((const char **)&d->s); return; }
    if (type == HB_PLDBS) { pl_db_slot_t *s = (pl_db_slot_t *)p; size_t n = bytes / sizeof(pl_db_slot_t); for (size_t i = 0; i < n; i++) rt_gc_visit_descr(&s[i].cl); return; }
    if (type == HB_PLDBR) { pl_db_reg_t *r = (pl_db_reg_t *)p; if (r->e) rt_gc_visit_raw((const char **)&r->e); if (r->ovf) rt_gc_visit_raw((const char **)&r->ovf); return; }
    if (type == HB_PLDBK) { pl_db_key_t *k = (pl_db_key_t *)p; size_t n = bytes / sizeof(pl_db_key_t);
        for (size_t i = 0; i < n; i++) { if (k[i].db) rt_gc_visit_raw((const char **)&k[i].db); if (k[i].key) rt_gc_visit_raw((const char **)&k[i].key); } return; }
    abort();
}
static pl_db_reg_t * pl_db_registry(void *root, int create)
{
    if (!root) return (pl_db_reg_t *)0;
    { pl_db_reg_t **cell = (pl_db_reg_t **)((char *)root - PL_DB_CELL0 - 8 * (size_t)PL_DB_REGISTRY_CELL);
      if (!*cell && create) {
          pl_db_reg_t *r = (pl_db_reg_t *)rt_pl_struct_alloc(HB_PLDBR, sizeof *r);
          if (!r) return (pl_db_reg_t *)0;
          r->cap = 32; r->n = 0; r->next_cell = 1; r->ovf_cap = 0; r->ovf = (void **)0; r->e = (pl_db_key_t *)rt_pl_struct_alloc(HB_PLDBK, (size_t)r->cap * sizeof(pl_db_key_t));
          if (!r->e) { r->cap = 0; }
          *cell = r;
      }
      return *cell; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void ** pl_db_cell_addr(void *root, int64_t k, int create)
{
    extern void *rt_pvec_realloc(void *, size_t);
    if (!root || k < 0) return (void **)0;
    if (k < PL_DB_FRAME_CELLS) return (void **)((char *)root - PL_DB_CELL0 - 8 * (size_t)k);
    { pl_db_reg_t *r = pl_db_registry(root, create); int64_t i = k - PL_DB_FRAME_CELLS;
      if (!r) return (void **)0;
      if (i >= r->ovf_cap) {
          int64_t nc = r->ovf_cap > 0 ? r->ovf_cap : 16; void **v;
          if (!create) return (void **)0;
          while (nc <= i) nc *= 2;
          v = (void **)rt_pvec_realloc(r->ovf, (size_t)nc);
          if (!v) return (void **)0;
          r->ovf = v; r->ovf_cap = (int)nc;
      }
      return &r->ovf[i]; }
}
static pl_db_key_t * pl_db_reg_find(pl_db_reg_t *r, const char *key)
{
    if (!r || !key) return (pl_db_key_t *)0;
    for (int i = 0; i < r->n; i++) if (!strcmp(r->e[i].key, key)) return &r->e[i];
    return (pl_db_key_t *)0;
}
static pl_db_key_t * pl_db_reg_add(pl_db_reg_t *r, const char *key)
{
    if (!r || !key) return (pl_db_key_t *)0;
    if (r->n >= r->cap) {
        int nc = r->cap > 0 ? r->cap * 2 : 32;
        pl_db_key_t *ne = (pl_db_key_t *)rt_pl_struct_alloc(HB_PLDBK, (size_t)nc * sizeof(pl_db_key_t));
        if (!ne) return (pl_db_key_t *)0;
        for (int i = 0; i < r->n; i++) ne[i] = r->e[i];
        r->e = ne; r->cap = nc;
    }
    { char *ks = rt_str_dup(key); if (!ks) return (pl_db_key_t *)0;
      pl_db_key_t *e = &r->e[r->n++]; e->key = ks; e->k = -1; e->db = (pl_db_t *)0; e->stat = 0; e->decl = 0; return e; }
}
int rt_pl_db_bind(void *root, int64_t k, const char *name, int64_t arity)
{
    if (!root || !name || k <= PL_DB_REGISTRY_CELL) return 0;
    PL_DB_KEY_VLA(key, name, arity);
    { pl_db_reg_t *r = pl_db_registry(root, 1); pl_db_key_t *e = pl_db_reg_find(r, key);
      if (!e) e = pl_db_reg_add(r, key);
      if (!e || !pl_db_cell_addr(root, k, 1)) return 0;
      e->k = (int)k; if ((int)k + 1 > r->next_cell) r->next_cell = (int)k + 1; return 1; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_key_cell(void *root, const char *key)
{
    pl_db_key_t *e = pl_db_reg_find(pl_db_registry(root, 0), key);
    return (e && e->k >= 0) ? e->k : -1;
}
int rt_pl_db_key_is_dynamic(void *root, const char *key);
int rt_pl_db_cell_for(void *root, const char *name, int arity)
{
    PL_DB_KEY_VLA(key, name, arity);
    { pl_db_reg_t *r = pl_db_registry(root, 1); pl_db_key_t *e = r ? pl_db_reg_find(r, key) : (pl_db_key_t *)0;
      if (e && e->k >= 0) return e->k;
      if (!r) return -1;
      if (r->next_cell <= PL_DB_REGISTRY_CELL) r->next_cell = PL_DB_REGISTRY_CELL + 1;
      if (!e) e = pl_db_reg_add(r, key);
      if (!e || !pl_db_cell_addr(root, r->next_cell, 1)) return -1;
      e->k = r->next_cell++; return e->k; }
}
int rt_pl_db_key_is_dynamic_na(void *root, const char *name, int arity) { PL_DB_KEY_VLA(key, name, arity); return rt_pl_db_key_is_dynamic(root, key); }
int rt_pl_db_key_cell_na(void *root, const char *name, int arity) { PL_DB_KEY_VLA(key, name, arity); return rt_pl_db_key_cell(root, key); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_term_cell(void *root, void *term_cell)
{
    extern const char *prolog_atom_name(int); extern int prolog_atom_intern(const char *);
    pl_cell_t *t = pl_deref((pl_cell_t *)term_cell); pl_cell_t *h = t; const char *nm; int ar = 0;
    if ((int)t->v == DT_PLREF && pl_arity(t) == 2 && plc_functor(t) == prolog_atom_intern(":-")) h = pl_deref(&((pl_cell_t *)t->p)[0]);
    if ((int)h->v == DT_PLREF) { nm = prolog_atom_name(plc_functor(h)); ar = pl_arity(h); }
    else if ((int)h->v == DT_PLATOM) nm = prolog_atom_name((int)h->i);
    else if ((int)h->v == DT_S) nm = h->s;
    else return -1;
    if (!nm) return -1;
    { PL_DB_KEY_VLA(key, nm, ar); return rt_pl_db_key_cell(root, key); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_frame_cells(void) { return PL_DB_FRAME_CELLS; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_decl(void *root, const char *name, int64_t arity, int64_t kind);
int rt_pl_db_cells_base(void *root, int64_t n);
void rt_pl_db_decls_install(const long *tab, void *root)
{
    extern const char *prolog_atom_name(int);
    long cells; long n; const long *e;
    if (!tab || !root) return;
    n = tab[0] / 4; cells = tab[1]; e = tab + 2;
    rt_pl_db_cells_base(root, cells);
    for (long i = 0; i < n; i++) { const char *nm = prolog_atom_name((int)e[4 * i]); if (nm && e[4 * i + 3] >= 0) rt_pl_db_bind(root, e[4 * i + 3], nm, e[4 * i + 1]); }
    for (long i = 0; i < n; i++) { const char *nm = prolog_atom_name((int)e[4 * i]); if (nm && e[4 * i + 2] > 0) rt_pl_db_decl(root, nm, e[4 * i + 1], e[4 * i + 2]); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_cells_base(void *root, int64_t n)
{
    pl_db_reg_t *r = pl_db_registry(root, 1);
    if (!r) return 0;
    if ((int)n > r->next_cell) r->next_cell = (int)n;
    return 1;
}
int rt_pl_db_key_is_dynamic(void *root, const char *key)
{
    pl_db_key_t *e = pl_db_reg_find(pl_db_registry(root, 0), key);
    return (e && !e->stat) ? 1 : 0;
}
int rt_pl_db_key_is_static(void *root, const char *key)
{
    pl_db_key_t *e = pl_db_reg_find(pl_db_registry(root, 0), key);
    return (e && e->stat) ? 1 : 0;
}
int rt_pl_db_key_is_declared(void *root, const char *key)
{
    pl_db_key_t *e = pl_db_reg_find(pl_db_registry(root, 0), key);
    return (e && e->decl) ? 1 : 0;
}
static int pl_db_key_current(void *root, pl_db_key_t *e);
int rt_pl_db_key_kind(void *root, const char *key)
{
    pl_db_key_t *e = pl_db_reg_find(pl_db_registry(root, 0), key);
    if (!e || !pl_db_key_current(root, e)) return 0;
    return e->stat ? 1 : 2;
}
int rt_pl_db_decl(void *root, const char *name, int64_t arity, int64_t kind)
{
    if (!root || !name) return 0;
    PL_DB_KEY_VLA(key, name, arity);
    { pl_db_reg_t *r = pl_db_registry(root, 1); pl_db_key_t *e = pl_db_reg_find(r, key);
      if (!e) e = pl_db_reg_add(r, key);
      if (!e) return 0;
      if (kind == 1) { if (e->k < 0 && !e->db) e->stat = 1; } else if (kind == 3) e->stat = 1; else e->decl = 1;
      return 1; }
}
static pl_db_t * pl_db_cell_peek(void *root, int k)
{
    if (!root || k < 0) return (pl_db_t *)0;
    { pl_db_t **cell = (pl_db_t **)pl_db_cell_addr(root, k, 0); return cell ? *cell : (pl_db_t *)0; }
}
static int pl_db_key_current(void *root, pl_db_key_t *e)
{
    pl_db_t *db;
    if (!strncmp(e->key, "$pl_", 4) || !strncmp(e->key, "$db_", 4)) return 0;
    if (e->stat) return 1;
    db = (e->k >= 0) ? pl_db_cell_peek(root, e->k) : e->db;
    if (db) return db->killed ? 0 : 1;
    return e->decl ? 1 : 0;
}
int rt_pl_db_cp_count(void *root)
{
    pl_db_reg_t *r = pl_db_registry(root, 0); int c = 0;
    if (!r) return 0;
    for (int i = 0; i < r->n; i++) if (pl_db_key_current(root, &r->e[i])) c++;
    return c;
}
const char * rt_pl_db_cp_nth(void *root, int64_t i, int *arity)
{
    pl_db_reg_t *r = pl_db_registry(root, 0); int c = 0;
    if (!r || i < 1) return (const char *)0;
    for (int j = 0; j < r->n; j++)
        if (pl_db_key_current(root, &r->e[j]) && ++c == (int)i) {
            const char *sl = strrchr(r->e[j].key, '/'); *arity = sl ? atoi(sl + 1) : 0; return r->e[j].key; }
    return (const char *)0;
}
void * rt_pl_db_get_by_key(void *root, const char *key, int create)
{
    pl_db_reg_t *r = pl_db_registry(root, create);
    pl_db_key_t *e = pl_db_reg_find(r, key);
    if (e && e->k >= 0) return rt_pl_db_get(root, e->k);
    if (!create) return (void *)0;
    if (!e) e = pl_db_reg_add(r, key);
    if (!e) return (void *)0;
    e->k = r->next_cell++;
    return rt_pl_db_get(root, e->k);
}
int rt_pl_db_term_key(void *term_cell, char *out, size_t n, int *ar)
{
    extern const char *prolog_atom_name(int);
    extern int prolog_atom_intern(const char *);
    pl_cell_t *t = pl_deref((pl_cell_t *)term_cell);
    pl_cell_t *h = t;
    if ((int)t->v == DT_PLREF && pl_arity(t) == 2 && plc_functor(t) == prolog_atom_intern(":-")) h = pl_deref(&((pl_cell_t *)t->p)[0]);
    { const char *nm;
      if ((int)h->v == DT_PLREF) { nm = prolog_atom_name(plc_functor(h)); *ar = pl_arity(h); }
      else if ((int)h->v == DT_PLATOM) { nm = prolog_atom_name((int)h->i); *ar = 0; }
      else if ((int)h->v == DT_S) { nm = h->s; *ar = 0; }
      else return 0;
      if (!nm) return 0;
      snprintf(out, n, "%s/%d", nm, *ar);
      return 1; }
}
static const char *pl_db_head_name(void *pair_cell, int *ar);
static int pl_db_fragment(void *root, pl_db_t *db, int i)
{
    extern void * pl_runtime_define_fragment(const char *, void *, int, int, int, int *, int *, void *);
    extern int pl_runtime_install_packet_entry(const char *, int, const char *);
    int ar = 0; int ridx = -1; int off = 0; const char *nm;
    if (!db || i < 0 || i >= db->n) return 0;
    nm = pl_db_head_name((void *)&db->s[i].cl, &ar);
    if (!nm) return 0;
    { PL_DB_KEY_VLA(key, nm, ar);
      { char fkey[fmt_len("%s@%d", key, i)]; snprintf(fkey, sizeof fkey, "%s@%d", key, i);
        if (!pl_runtime_define_fragment(fkey, (void *)&db->s[i].cl, ar, db->cell_k, i, &ridx, &off, root)) return 0; }
      db->s[i].ridx = ridx; db->s[i].chain_off = off;
      if (db->head_idx >= 0) { char hkey[fmt_len("%s@%d", key, db->head_idx)]; snprintf(hkey, sizeof hkey, "%s@%d", key, db->head_idx); pl_runtime_install_packet_entry(key, ar, hkey); } }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_copy(void *db_v, int i, void *out_v);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *pl_db_head_name(void *pair_cell, int *ar)
{
    extern const char *prolog_atom_name(int);
    pl_cell_t *p = pl_deref((pl_cell_t *)pair_cell);
    if ((int)p->v != DT_PLREF || pl_arity(p) != 2) return (const char *)0;
    { pl_cell_t *h = pl_deref(&((pl_cell_t *)p->p)[0]);
      if ((int)h->v == DT_PLREF) { *ar = pl_arity(h); return prolog_atom_name(plc_functor(h)); }
      if ((int)h->v == DT_PLATOM) { *ar = 0; return prolog_atom_name((int)h->i); }
      if ((int)h->v == DT_S) { *ar = 0; return h->s; }
      return (const char *)0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_head_key(void *pair_cell, char *out, size_t n, int *ar)
{
    const char *nm = pl_db_head_name(pair_cell, ar);
    if (!nm) return 0;
    snprintf(out, n, "%s/%d", nm, *ar);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_db_store(void *root, void *db_v, void *clause_term, int prepend, int recompile);
int rt_pl_db_assert(void *root, void *db_v, void *clause_term, int prepend) { return pl_db_store(root, db_v, clause_term, prepend, 1); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_assert_ref(void *root, void *db_v, void *clause_term, int prepend)
{
    pl_db_t *db = (pl_db_t *)db_v;
    if (!pl_db_store(root, db_v, clause_term, prepend, 1)) return 0;
    return db->s[db->n - 1].ref;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_slot_of_ref(void *db_v, int ref)
{
    pl_db_t *db = (pl_db_t *)db_v;
    if (!db || ref < 1) return -1;
    for (int i = 0; i < db->n; i++) if (!db->s[i].erased && db->s[i].ref == ref) return i;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_ref_at(void *db_v, int i)
{
    pl_db_t *db = (pl_db_t *)db_v;
    if (!db || i < 0 || i >= db->n || db->s[i].erased) return 0;
    return db->s[i].ref;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_seed(void *root, void *db_v, void *clause_term) { return pl_db_store(root, db_v, clause_term, 0, 1); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_db_store(void *root, void *db_v, void *clause_term, int prepend, int recompile)
{
    extern int prolog_atom_intern(const char *);
    pl_db_t *db = (pl_db_t *)db_v;
    if (!db || !clause_term) return 0;
    db->killed = 0;
    if (db->n >= db->cap) {
        int nc = db->cap > 0 ? db->cap * 2 : 8;
        pl_db_slot_t *ns = (pl_db_slot_t *)rt_pl_struct_alloc(HB_PLDBS, (size_t)nc * sizeof(pl_db_slot_t));
        if (!ns) return 0;
        for (int i = 0; i < db->n; i++) ns[i] = db->s[i];
        db->s = ns; db->cap = nc;
    }
    { pl_cell_t *t = pl_deref((pl_cell_t *)clause_term);
      pl_cell_t kids[2]; pl_cell_t pair;
      if ((int)t->v == DT_PLREF && pl_arity(t) == 2 && plc_functor(t) == prolog_atom_intern(":-")) { pl_cell_t *aa = (pl_cell_t *)t->p; kids[0] = aa[0]; kids[1] = aa[1]; }
      else { kids[0] = *t; kids[1] = pl_make_atom(prolog_atom_intern("true")); }
      pair = pl_make_compound(prolog_atom_intern(":-"), 2, (void *)kids);
      { pl_cell_t stored = plc_copy(&pair); int i = db->n;
        if (db->next_ref < 1) db->next_ref = 1;
        db->s[i].cl = stored; db->s[i].erased = 0; db->s[i].ref = db->next_ref++; db->s[i].ridx = -1; db->s[i].chain_off = 0; db->s[i].next_idx = -1; db->s[i].pad = 0;
        if (prepend) { db->s[i].next_idx = db->head_idx; db->head_idx = i; if (db->tail_idx < 0) db->tail_idx = i; }
        else { if (db->tail_idx >= 0) db->s[db->tail_idx].next_idx = i; else db->head_idx = i; db->tail_idx = i; }
        db->n++;
        if (recompile) pl_db_fragment(root, db, i); }
      return 1; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_erase(void *db_v, int i)
{
    pl_db_t *db = (pl_db_t *)db_v;
    if (!db || i < 0 || i >= db->n || db->s[i].erased) return 0;
    db->s[i].erased = db->next_ref++;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_count(void *db_v) { pl_db_t *db = (pl_db_t *)db_v; return db ? db->n : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_gen(void *db_v) { pl_db_t *db = (pl_db_t *)db_v; return db ? db->next_ref : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_db_slot_copy(pl_db_t *db, int i, void *out_v)
{
    *(pl_cell_t *)out_v = plc_copy(&db->s[i].cl);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_clause_as_of(void *db_v, int i, int gen, void *out_v)
{
    pl_db_t *db = (pl_db_t *)db_v;
    if (!db || !out_v || i < 0 || i >= db->n || db->s[i].ref >= gen || (db->s[i].erased && db->s[i].erased < gen)) return 0;
    return pl_db_slot_copy(db, i, out_v);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_live_count(void *db_v) { pl_db_t *db = (pl_db_t *)db_v; int c = 0; if (!db) return 0; for (int i = 0; i < db->n; i++) if (!db->s[i].erased) c++; return c; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_clause_at(void *db_v, int i, void *out_v)
{
    pl_db_t *db = (pl_db_t *)db_v;
    if (!db || !out_v || i < 0 || i >= db->n || db->s[i].erased) return 0;
    return pl_db_slot_copy(db, i, out_v);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_copy(void *db_v, int i, void *out_v)
{
    pl_db_t *db = (pl_db_t *)db_v;
    if (!db || !out_v || i < 0 || i >= db->n) return 0;
    return pl_db_slot_copy(db, i, out_v);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_db_define_absent(void *root, const char *key, int arity)
{
    extern void * rt_pl_clause_tree(void *);
    extern void * pl_runtime_clause_tree(void *);
    extern void * pl_runtime_define_pred(const char *, const void *, int, void *);
    extern void * rt_pl_choice_new(const char *);
    extern void rt_pl_choice_add(void *, void *);
    extern int prolog_atom_intern(const char *);
    const char *sl; pl_cell_t *hargs; pl_cell_t head; pl_cell_t pi[2]; pl_cell_t pic;
    pl_cell_t ee[2]; pl_cell_t eec; pl_cell_t er[2]; pl_cell_t erc; pl_cell_t th[1]; pl_cell_t body; pl_cell_t kids[2]; pl_cell_t pair;
    void *raw; void *cl; void *ch;
    if (!key) return 0;
    sl = strrchr(key, '/'); if (!sl) return 0;
    char nm[(size_t)(sl - key) + 1];
    { size_t kl = (size_t)(sl - key); memcpy(nm, key, kl); nm[kl] = 0; }
    if (arity > 0) { hargs = (pl_cell_t *)PL_CELL_ALLOC((size_t)arity * sizeof(pl_cell_t)); if (!hargs) return 0; for (int i = 0; i < arity; i++) pl_init_var(&hargs[i], -1); head = pl_make_compound(prolog_atom_intern(nm), arity, hargs); }
    else head = pl_make_atom(prolog_atom_intern(nm));
    pi[0] = pl_make_atom(prolog_atom_intern(nm)); pi[1] = pl_make_int(arity); pic = pl_make_compound(prolog_atom_intern("/"), 2, (void *)pi);
    ee[0] = pl_make_atom(prolog_atom_intern("procedure")); ee[1] = pic; eec = pl_make_compound(prolog_atom_intern("existence_error"), 2, (void *)ee);
    er[0] = eec; er[1] = pic; erc = pl_make_compound(prolog_atom_intern("error"), 2, (void *)er);
    th[0] = erc; body = pl_make_compound(prolog_atom_intern("throw"), 1, (void *)th);
    kids[0] = head; kids[1] = body; pair = pl_make_compound(prolog_atom_intern(":-"), 2, (void *)kids);
    raw = rt_pl_clause_tree((void *)&pair); cl = raw ? pl_runtime_clause_tree(raw) : (void *)0;
    if (!cl) return 0;
    ch = rt_pl_choice_new(key); rt_pl_choice_add(ch, cl);
    return pl_runtime_define_pred(key, ch, arity, root) ? 1 : 0;
}
int rt_pl_db_abolish(void *root, void *db_v)
{
    pl_db_t *db = (pl_db_t *)db_v;
    int ar = 0; int hi = -1;
    if (!db) return 0;
    for (int i = 0; i < db->n && hi < 0; i++) if (pl_db_head_name((void *)&db->s[i].cl, &ar)) hi = i;
    { int st = db->next_ref++; for (int i = 0; i < db->n; i++) if (!db->s[i].erased) db->s[i].erased = st; }
    db->killed = 1;
    if (hi >= 0) { const char *nm = pl_db_head_name((void *)&db->s[hi].cl, &ar); PL_DB_KEY_VLA(key, nm, ar); pl_db_define_absent(root, key, ar); }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_match_erase(void *db_v, void *goal_term)
{
    pl_db_t *db = (pl_db_t *)db_v;
    int hit = 0; int ar = 0; int key_i = -1;
    if (!db || !goal_term) return 0;
    for (int i = 0; i < db->n; i++) {
        if (db->s[i].erased) continue;
        { pl_cell_t pair = plc_copy(&db->s[i].cl);
          pl_cell_t g = plc_copy((pl_cell_t *)goal_term);
          pl_cell_t *h = (pl_cell_t *)pl_deref(&pair)->p;
          if (h && pl_unify(&h[0], &g)) {
              if (key_i < 0 && pl_db_head_name((void *)&db->s[i].cl, &ar)) key_i = i;
              db->s[i].erased = db->next_ref++; hit++; } }
    }
    (void)key_i; (void)ar;
    return hit;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_killed(void *db_v) { pl_db_t *db = (pl_db_t *)db_v; return db ? db->killed : 0; }
