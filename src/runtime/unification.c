#include <math.h>
#include <errno.h>
#include "rt/rt_arena.h"
#include "ct_arena.h"
#include "rt/rt.h"
#include "rt/gc_heap.h"
#include "pl_rat.h"
#include "core.h"
#include "core/utf8.h"
#include "bb_pool.h"
#include "rt/prolog_atom.h"
#include "../ir/IR.h"
#include "../ir/gc_frame_map.h"
#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <locale.h>
#include <wctype.h>
#include <sys/mman.h>
#include "../parsers/prolog/pl_cell.h"
#include "rt/rt_pl_trail.h"
#include "rt/pl_rational.h"
void rt_pl_ball_report(void *ball);
#define PL_CELL_ALLOC(n) (rt_ws_alloc_descr((size_t)(n) / sizeof(pl_cell_t)))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_unify_into_cell(pl_cell_t *dst, pl_cell_t val);
extern int plw_unify_cell_val(DESCR_t *, DESCR_t, pl_tr_ctx_t *);
extern int plw_unify_cells_x(DESCR_t *, DESCR_t *, pl_tr_ctx_t *);
extern int rt_pl_eng_inst_is(DESCR_t);
extern int64_t rt_pl_eng_inst_id(DESCR_t);
extern void plw_bind_x(DESCR_t *, DESCR_t, pl_tr_ctx_t *);
static int plc_unify_into_cell_cx(pl_cell_t *dst, pl_cell_t val, pl_tr_ctx_t *cx) { return cx ? plw_unify_cell_val(dst, val, cx) : plc_unify_into_cell(dst, val); }
static int plc_unify_cells_cx(pl_cell_t *a, pl_cell_t *b, pl_tr_ctx_t *cx) { return cx ? plw_unify_cells_x(a, b, cx) : pl_unify(a, b); }
static void plc_bind_cx(pl_cell_t *cell, pl_cell_t word, pl_tr_ctx_t *cx) { if (cx) plw_bind_x(pl_deref(cell), word, cx); else pl_bind(cell, word); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_unify_terms(void *l, void *r) { pl_cell_t *a = (pl_cell_t *)l, *b = (pl_cell_t *)r; if (!a || !b) return 0; if (!pl_unify(a, b)) { return 0; } return 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_unify_const(int slot, int kind, long ival, const char *sval, double dval) { (void)slot; (void)kind; (void)ival; (void)sval; (void)dval; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_unify_var_var(int lslot, int rslot) { (void)lslot; (void)rslot; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_cells_init(void **cells, int n) { char *base = (char *)cells; for (int i = 0; i < n; i++) pl_init_var((pl_cell_t *)(base + (size_t)16 * (size_t)i), i); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_gz_init(void *frame, int nslots) { prolog_atom_init(); char *base = (char *)frame; for (int i = 0; i < nslots; i++) pl_init_var((pl_cell_t *)(base + 8 + (size_t)16 * (size_t)i), i); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_compound_cell(const char *functor_name, int arity, void *arg_words) {
    pl_cell_t *src = (pl_cell_t *)arg_words;
    pl_cell_t *blk = (pl_cell_t *)PL_CELL_ALLOC((size_t)(arity > 0 ? arity : 1) * sizeof(pl_cell_t));
    for (int i = 0; i < arity; i++) blk[i] = src[i];
    if (arity == 0) blk[0] = pl_make_int(0);
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
#define PLC_WQ_XESC 8
struct plc_vmap {
    pl_cell_t *seen[1024];
    int n;
    FILE *fp;
    pl_cell_t *vnl;
    int vnc;
    int (*portray)(pl_cell_t *, plc_vmap *);
    pl_tr_ctx_t *cx;
    void *pthrown;
    long pheld;
    int space_args;
    int next_arg;
    pl_cell_t *pgoal;
    pl_cell_t *popts;
};
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *plc_atom_text(pl_cell_t *d);
static const char *plc_vname(plc_vmap *m, pl_cell_t *d) {
    const char *r = (const char *)0;
    if (m->vnc <= 0 || !m->vnl) return r;
    for (pl_cell_t *lst = pl_deref(m->vnl); (int)lst->v == DT_PLREF && plc_fid_arity(lst->slen) == 2 && lst->p; lst = pl_deref(&((pl_cell_t *)lst->p)[1])) {
        pl_cell_t *o = pl_deref(&((pl_cell_t *)lst->p)[0]);
        const char *on = (int)o->v == DT_PLREF && plc_fid_arity(o->slen) == 1 && o->p ? prolog_atom_name(plc_fid_name(o->slen)) : (const char *)0;
        if (!on || strcmp(on, "variable_names")) continue;
        for (pl_cell_t *vl = pl_deref(&((pl_cell_t *)o->p)[0]); (int)vl->v == DT_PLREF && plc_fid_arity(vl->slen) == 2 && vl->p; vl = pl_deref(&((pl_cell_t *)vl->p)[1])) {
            pl_cell_t *e = pl_deref(&((pl_cell_t *)vl->p)[0]);
            if (!((int)e->v == DT_PLREF && plc_fid_arity(e->slen) == 2 && e->p)) continue;
            pl_cell_t *nm = pl_deref(&((pl_cell_t *)e->p)[0]), *vr = pl_deref(&((pl_cell_t *)e->p)[1]);
            if (vr == d && pl_cell_unbound(vr) && ((int)nm->v == DT_PLATOM || (int)nm->v == DT_S)) { r = plc_atom_text(nm); break; }
        }
    }
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_vindex(plc_vmap *m, pl_cell_t *d) { for (int i = 0; i < m->n; i++) if (m->seen[i] == d) return i; if (m->n < 1024) { m->seen[m->n] = d; return m->n++; } return m->n - 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *plc_atom_text(pl_cell_t *d) { if ((int)d->v == DT_S) return d->s ? d->s : ""; const char *n = prolog_atom_name((int)d->i); return n ? n : "?"; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static size_t plc_atom_len(pl_cell_t *d) { if ((int)d->v == DT_S) return d->s ? d->slen : 0; return prolog_atom_name((int)d->i) ? prolog_atom_len((int)d->i) : 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_user_op(const char *fn, int ar, int *prec, int *ra) {
    extern int prolog_op_user_count(void);
    extern int prolog_op_user_get(int, const char **, int *, const char **);
    int n = prolog_op_user_count();
    for (int i = 0; i < n; i++) {
        const char *nm = 0;
        const char *ty = 0;
        int pr = 0;
        if (!prolog_op_user_get(i, &nm, &pr, &ty) || !nm || !ty || strcmp(nm, fn)) continue;
        if (ar == 2 && (!strcmp(ty, "xfx") || !strcmp(ty, "xfy") || !strcmp(ty, "yfx"))) { *prec = pr; *ra = !strcmp(ty, "xfy"); return 1; }
        if (ar == 1 && (!strcmp(ty, "fy") || !strcmp(ty, "fx"))) { *prec = pr; *ra = 0; return 1; }
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define PLC_WC_IGNORE_OPS 1
static int plc_op_info_id(int nid, int arity, int *prec, int *lmax, int *rmax) {
    extern int prolog_op_col_get(int, int, int *, int *);
    extern int prolog_op_col_user(int, int);
    int p = 0, t = 0;
    if (nid < 0) return 0;
    if (arity == 2) {
        if (!prolog_op_col_get(nid, 1, &p, &t)) return 0;
    } else if (arity == 1) {
        if (prolog_op_col_get(nid, 0, &p, &t) && prolog_op_col_user(nid, 0)) {
        } else if (prolog_op_col_get(nid, 2, &p, &t) && prolog_op_col_user(nid, 2)) {
        } else if (!prolog_op_col_get(nid, 0, &p, &t)) return 0;
    } else return 0;
    *prec = p;
    if (t == 1) {
        *lmax = p - 1;
        *rmax = p - 1;
    } else if (t == 2) {
        *lmax = p - 1;
        *rmax = p;
    } else if (t == 3) {
        *lmax = p;
        *rmax = p - 1;
    } else if (t == 4) {
        *lmax = 0;
        *rmax = p;
    } else if (t == 5) {
        *lmax = 0;
        *rmax = p - 1;
    } else if (t == 6) {
        *lmax = p;
        *rmax = 0;
    } else {
        *lmax = p - 1;
        *rmax = 0;
    }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_op_info(const char *name, int arity, int *prec, int *lmax, int *rmax) { return name ? plc_op_info_id(prolog_atom_intern(name), arity, prec, lmax, rmax) : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_op_is_postfix_id(int nid) {
    extern int prolog_op_col_get(int, int, int *, int *);
    extern int prolog_op_col_user(int, int);
    int p = 0, t = 0;
    if (nid < 0 || !prolog_op_col_get(nid, 2, &p, &t) || !prolog_op_col_user(nid, 2)) return 0;
    { int pp = 0, lm = 0, rm = 0; return !(plc_op_info_id(nid, 1, &pp, &lm, &rm) && rm > 0); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_op_is_postfix(const char *name) { return name ? plc_op_is_postfix_id(prolog_atom_intern(name)) : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_is_nil(pl_cell_t *d) { if ((int)d->v == DT_PLATOM) return (int)d->i == g_runtime.atoms.atom_nil; if ((int)d->v == DT_S) return d->s && strcmp(d->s, "[]") == 0; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_u8_get(const char *s, int *adv);
static int plc_atom_needs_quoting(const char *name) {
    static const char *const graphic = "#$&*+-./:<=>?@\\^~";
    extern int rt_pl_dialect_xsb(void);
    if (!name || !name[0]) return 1;
    if (rt_pl_dialect_xsb() && (!strcmp(name, "{}") || !strcmp(name, ";"))) return 1;
    if (!strcmp(name, "[]") || !strcmp(name, "{}") || !strcmp(name, "!") || !strcmp(name, ";")) return 0;
    if (isupper((unsigned char)name[0]) || name[0] == '_') return 1;
    {
        extern int prolog_u_letter(const char *, int *);
        int adv, k = ((unsigned char)name[0] >= 0x80) ? prolog_u_letter(name, &adv) : 0;
        if (islower((unsigned char)name[0]) || k == 2) {
            for (const char *q = name + (k ? adv : 1); *q; ) {
                if (isalnum((unsigned char)*q) || *q == '_') { q++; continue; }
                if ((unsigned char)*q >= 0x80 && prolog_u_letter(q, &adv)) { q += adv; continue; }
                return 1;
            }
            return 0;
        }
        if (k == 1) return 1;
    }
    {
        extern int prolog_u_letter(const char *, int *);
        extern int rt_pl_dialect_swi(void);
        int all_graphic = 1;
        for (const char *q = name; *q; ) {
            int adv = 1, cp;
            if ((unsigned char)*q < 0x80) { if (!strchr(graphic, *q)) { all_graphic = 0; break; } q++; continue; }
            if (!rt_pl_dialect_swi() || prolog_u_letter(q, &adv) ||
                (cp = rt_pl_u8_get(q, &adv), cp < 0xA0 || cp == 0xA0 || cp == 0x1680 || (cp >= 0x2000 && cp <= 0x200A) || cp == 0x2028 || cp == 0x2029 || cp == 0x202F || cp == 0x205F || cp == 0x3000))
                {
                all_graphic = 0;
                break;
            }
            q += adv;
        }
        if (all_graphic) { if (!strcmp(name, ".")) return 1; if (name[0] == '/' && name[1] == '*') return 1; return 0; }
    }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_utf8_run(const char *q) {
    int n, k;
    if ((unsigned char)*q < 0x80) return 1;
    n = utf8_seqlen((unsigned char)*q);
    if (n < 2) return 1;
    for (k = 1; k < n; k++) if (((unsigned char)q[k] & 0xC0) != 0x80) return 1;
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_has_non_ascii(const char *s) { for (; *s; s++) if ((unsigned char)*s >= 0x80) return 1; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern int rt_pl_dialect_swi(void);
extern int rt_pl_dialect_xsb(void);
extern int rt_pl_dialect_gnu(void);
int rt_pl_u8_get(const char *s, int *adv);
extern int rt_pl_flag_true(const char *nm);
static void plc_wt_atom_n(FILE *fp, const char *name, size_t len, int quoted) {
    extern int rt_pl_fp_max_code(FILE *);
    int noesc = quoted & PLC_WQ_NOESC, qna = quoted & PLC_WQ_NONASCII, xesc = quoted & PLC_WQ_XESC, maxc = -1;
    quoted &= 1;
    if (!name) { name = "?"; len = 1; }
    if (quoted && plc_has_non_ascii(name)) { int adv; maxc = rt_pl_fp_max_code(fp); for (const char *q = name; *q && maxc < 0x10FFFF; q += adv) if (rt_pl_u8_get(q, &adv) > maxc) { qna = 1; break; } }
    if (quoted && (plc_atom_needs_quoting(name) || memchr(name, 0, len) || (qna && plc_has_non_ascii(name)))) {
        fputc('\'', fp);
        for (const char *q = name; q < name + len; q++) {
            if (noesc && *q != '\'') { fputc(*q, fp); continue; }
            switch (*q) {
                case '\'':
                fputs(!noesc && rt_pl_dialect_swi() ? "\\'" : "''", fp);
                break;
                case '\\':
                fputs("\\\\", fp);
                break;
                case '\n':
                fputs("\\n", fp);
                break;
                case '\t':
                fputs("\\t", fp);
                break;
                case '\r':
                fputs("\\r", fp);
                break;
                case '\a':
                fputs("\\a", fp);
                break;
                case '\b':
                fputs("\\b", fp);
                break;
                case '\f':
                fputs("\\f", fp);
                break;
                case '\v':
                fputs("\\v", fp);
                break;
                case '\0':
                fputs("\\x0\\", fp);
                break;
                default:
                {
                    int _sl = plc_utf8_run(q), _adv, _cp = _sl > 1 ? rt_pl_u8_get(q, &_adv) : 0;
                    if (_sl > 1 && maxc < 0) maxc = rt_pl_fp_max_code(fp);
                    if (_sl > 1 && _cp > maxc) {
                        if (xesc) fprintf(fp, "\\x%X\\", (unsigned)_cp);
                        else fprintf(fp, _cp > 0xFFFF ? "\\U%08X" : "\\u%04X", (unsigned)_cp);
                        q += _sl - 1;
                    } else if (_sl > 1) {
                        for (int _k = 0; _k < _sl; _k++) fputc(q[_k], fp);
                        q += _sl - 1;
                    } else if (!isprint((unsigned char)*q)) fprintf(fp, "\\x%x\\", (unsigned)(unsigned char)*q);
                    else fputc(*q, fp);
                    break;
                }
            }
        }
        fputc('\'', fp);
    } else fwrite(name, 1, len, fp);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_wt_atom(FILE *fp, const char *name, int quoted) { plc_wt_atom_n(fp, name, name ? strlen(name) : 1, quoted); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_wt_string(FILE *fp, pl_cell_t *d, int quoted) {
    const char *b = d->s ? d->s : "";
    uint32_t n = d->s ? d->slen : 0;
    if (!(quoted & 1)) { fwrite(b, 1, n, fp); return; }
    fputc('"', fp);
    for (uint32_t i = 0; i < n; i++) {
        unsigned char c = (unsigned char)b[i];
        if ((quoted & PLC_WQ_NOESC) && c != '"') { fputc(c, fp); continue; }
        switch (c) {
            case '"':
            fputs("\\\"", fp);
            break;
            case '\\':
            fputs("\\\\", fp);
            break;
            case '\n':
            fputs("\\n", fp);
            break;
            case '\t':
            fputs("\\t", fp);
            break;
            case '\r':
            fputs("\\r", fp);
            break;
            case '\a':
            fputs("\\a", fp);
            break;
            case '\b':
            fputs("\\b", fp);
            break;
            case '\f':
            fputs("\\f", fp);
            break;
            case '\v':
            fputs("\\v", fp);
            break;
            case '\0':
            fputs("\\0\\", fp);
            break;
            default:
            if (c < 0x20 || c == 0x7f) fprintf(fp, "\\x%x\\", (unsigned)c);
            else fputc(c, fp);
        }
    }
    fputc('"', fp);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_wt_num(FILE *fp, pl_cell_t *d) { extern const char *pl_real_iso_str(double, char *, int); char fb[64]; fputs(pl_real_iso_str(d->r, fb, (int)sizeof fb), fp); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_term_prio(pl_cell_t *c, int ignore_ops) {
    pl_cell_t *d = c ? pl_deref(c) : (pl_cell_t *)0;
    int pr = 0, lm = 0, rm = 0, ar;
    if (!d || ignore_ops || (int)d->v != DT_PLREF) return 0;
    ar = plc_fid_arity(d->slen);
    if (plc_fid_name(d->slen) == g_runtime.atoms.atom_dot && ar == 2) return 0;
    if (ar != 1 && ar != 2) return 0;
    { int nid = plc_fid_name(d->slen); const char *fn = prolog_atom_name(nid); if (fn && !strcmp(fn, "{}") && ar == 1) return 0; if (fn && plc_op_info_id(nid, ar, &pr, &lm, &rm)) return pr; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_is_graphic_char(int ch) { static const char *const g = "#$&*+-./:<=>?@\\^~"; return ch && strchr(g, ch) != (const char *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_is_op_atom(pl_cell_t *d) {
    int p = 0, l = 0, r = 0;
    int nid = (int)d->v == DT_PLATOM ? (int)d->i : -1;
    if (nid >= 0 && nid == g_runtime.atoms.atom_dot && rt_pl_dialect_swi()) return 1;
    return nid >= 0 && (plc_op_info_id(nid, 2, &p, &l, &r) || plc_op_info_id(nid, 1, &p, &l, &r));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_first_char_d(pl_cell_t *c, int quoted, int ignore_ops, int numbervars, long dl) {
    pl_cell_t *d = c ? pl_deref(c) : (pl_cell_t *)0;
    int pr = 0, lm = 0, rm = 0;
    if (!d) return '[';
    if (dl <= 0) return '.';
    if (pl_cell_unbound(d)) return '_';
    if ((int)d->v == DT_I) return d->i < 0 ? '-' : '0';
    if ((int)d->v == DT_R) return signbit(d->r) && !isnan(d->r) ? '-' : '0';
    if ((int)d->v == DT_BIG) { extern int rt_big_sign(DESCR_t); return rt_big_sign(*d) < 0 ? '-' : '0'; }
    if (pl_rat_is(*d)) return pl_rat_cmp(*d, pl_make_int(0)) < 0 ? '-' : '0';
    if ((int)d->v == DT_S) return '"';
    if ((int)d->v == DT_PLATOM || (int)d->v == DT_S) {
        const char *n = plc_atom_text(d);
        if (!n || !n[0]) return quoted ? '\'' : ' ';
        return (quoted && plc_atom_needs_quoting(n)) ? '\'' : (unsigned char)n[0];
    }
    if ((int)d->v != DT_PLREF) return '_';
    {
        int ar = plc_fid_arity(d->slen);
        int nid = plc_fid_name(d->slen);
        const char *fn = prolog_atom_name(nid);
        pl_cell_t *aa = (pl_cell_t *)d->p;
        if (!fn) return 'a';
        if (numbervars && ar == 1 && !strcmp(fn, "$VAR")) {
            pl_cell_t *n = pl_deref(&aa[0]);
            if ((int)n->v == DT_I) return 'A';
            if ((int)n->v == DT_PLATOM || (int)n->v == DT_S) { const char *t = plc_atom_text(n); return t ? (unsigned char)*t : 0; }
        }
        if ((!ignore_ops || (ignore_ops & 2)) && plc_fid_name(d->slen) == g_runtime.atoms.atom_dot && ar == 2) return '[';
        if ((!ignore_ops || (ignore_ops & 4)) && ar == 1 && !strcmp(fn, "{}")) return '{';
        if (!ignore_ops && (ar == 1 || ar == 2) && plc_op_info_id(nid, ar, &pr, &lm, &rm)) {
            if (pr > 1200) return (unsigned char)fn[0];
            if (ar == 2 && dl > 1 && plc_is_op_atom(pl_deref(&aa[0]))) return '(';
            if (ar == 2 || plc_op_is_postfix_id(nid)) return plc_first_char_d(&aa[0], quoted, ignore_ops, numbervars, dl - 1);
            return (quoted && plc_atom_needs_quoting(fn)) ? '\'' : (unsigned char)fn[0];
        }
        return (quoted && plc_atom_needs_quoting(fn)) ? '\'' : (unsigned char)fn[0];
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_first_char(pl_cell_t *c, int quoted, int ignore_ops, int numbervars) { return plc_first_char_d(c, quoted, ignore_ops, numbervars, LONG_MAX); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_alpha_glue(int ch) { return isalnum((unsigned char)ch) || ch == '_' || ch >= 0x80; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_last_char_d(pl_cell_t *c, int quoted, int ignore_ops, int numbervars, long dl) {
    pl_cell_t *d = c ? pl_deref(c) : (pl_cell_t *)0;
    int pr = 0, lm = 0, rm = 0, ar, nid;
    const char *fn, *n;
    pl_cell_t *aa;
    if (!d) return ']';
    if (dl <= 0) return '.';
    if (pl_cell_unbound(d) || (int)d->v == DT_I || (int)d->v == DT_R || (int)d->v == DT_BIG || pl_rat_is(*d)) return '0';
    if ((int)d->v == DT_S) { n = plc_atom_text(d); return quoted ? '"' : (n[0] ? (unsigned char)n[strlen(n) - 1] : ' '); }
    if ((int)d->v == DT_PLATOM) { n = plc_atom_text(d); if (!n[0]) return quoted ? '\'' : ' '; return (quoted && plc_atom_needs_quoting(n)) ? '\'' : (unsigned char)n[strlen(n) - 1]; }
    if ((int)d->v != DT_PLREF) return '0';
    ar = plc_fid_arity(d->slen);
    nid = plc_fid_name(d->slen);
    fn = prolog_atom_name(nid);
    aa = (pl_cell_t *)d->p;
    if (!fn || !fn[0]) return ')';
    if (numbervars && ar == 1 && !strcmp(fn, "$VAR")) return 'a';
    if ((!ignore_ops || (ignore_ops & 2)) && nid == g_runtime.atoms.atom_dot && ar == 2) return ']';
    if ((!ignore_ops || (ignore_ops & 4)) && ar == 1 && !strcmp(fn, "{}")) return '}';
    if (ignore_ops || (ar != 1 && ar != 2) || !plc_op_info_id(nid, ar, &pr, &lm, &rm)) return ')';
    if (ar == 1 && plc_op_is_postfix_id(nid)) return (unsigned char)fn[strlen(fn) - 1];
    aa = ar == 2 ? &aa[1] : &aa[0];
    if (dl <= 1) return '.';
    return (plc_is_op_atom(pl_deref(aa)) || plc_term_prio(aa, ignore_ops) > rm) ? ')' : plc_last_char_d(aa, quoted, ignore_ops, numbervars, dl - 1);
}
static int plc_last_char(pl_cell_t *c, int quoted, int ignore_ops, int numbervars) { return plc_last_char_d(c, quoted, ignore_ops, numbervars, LONG_MAX); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { long k; pl_cell_t *p; } plc_wsref;
static DESCR_t *plc_ws_vec(const pl_tr_ctx_t *cx) { return (cx && cx->tr) ? *(DESCR_t **)pl_tr_wslot_slot(pl_tr_base_of(cx->tr)) : (DESCR_t *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long plc_ws_mark(const pl_tr_ctx_t *cx) { DESCR_t *v; if (!cx || !cx->tr) return -1; v = plc_ws_vec(cx); return v ? (long)v[0].i : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_ws_end(const pl_tr_ctx_t *cx, long mark) {
    DESCR_t *v = plc_ws_vec(cx);
    long n;
    if (mark < 0 || !v) return;
    n = (long)v[0].i;
    for (long i = mark + 1; i <= n; i++) memset(&v[i], 0, sizeof v[i]);
    v[0].i = mark;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long plc_ws_push(const pl_tr_ctx_t *cx, DESCR_t x) {
    void **w = pl_tr_wslot_slot(pl_tr_base_of(cx->tr));
    DESCR_t *v = (DESCR_t *)*w;
    long n = v ? (long)v[0].i : 0, cap = v ? (long)v[0].slen : 0;
    if (n + 1 >= cap) {
        long nc = cap ? 2 * cap : 32;
        DESCR_t *nv = (DESCR_t *)rt_ws_alloc_descr((size_t)nc);
        if (v) memcpy(nv, v, sizeof(DESCR_t) * (size_t)(n + 1));
        nv[0].v = (DTYPE_t)DT_I;
        nv[0].slen = (uint32_t)nc;
        nv[0].i = n;
        v = nv;
        *w = (void *)v;
    }
    v[n + 1] = x;
    v[0].i = n + 1;
    return n + 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_wsref_set(const pl_tr_ctx_t *cx, plc_wsref *r, pl_cell_t *c) {
    extern int rt_gc_in_arena(const char *);
    DESCR_t x;
    r->p = c;
    if (!cx || !cx->tr) { r->k = -1; return; }
    memset(&x, 0, sizeof x);
    if (c && rt_gc_in_arena((const char *)c)) { x.v = (DTYPE_t)DT_PLVAR; x.p = (void *)c; }
    if (r->k < 0) r->k = plc_ws_push(cx, x);
    else plc_ws_vec(cx)[r->k] = x;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static pl_cell_t *plc_wsref_get(const pl_tr_ctx_t *cx, plc_wsref *r) {
    if (r->k >= 0) { DESCR_t *v = plc_ws_vec(cx); if (v && (int)v[r->k].v == DT_PLVAR) r->p = (pl_cell_t *)v[r->k].p; }
    return r->p;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static pl_cell_t plc_atom_id_cell(int id);
static int plc_portray_hit(pl_cell_t *d, plc_vmap *m) {
    extern DESCR_t rt_pl_goal_gen_h_c(DESCR_t goal, DESCR_t *argv, int n, void **hslot, void **ball, pl_tr_ctx_t *cx);
    extern int fh_alloc(FILE *);
    extern int fh_find_fp(FILE *);
    extern void fh_free(int);
    extern int fh_current_output(void);
    extern void fh_set_output(int);
    if (m->pthrown || !d) return 0;
    {
        pl_cell_t g = m->pgoal ? *m->pgoal : plc_atom_id_cell(prolog_atom_intern("portray"));
        DESCR_t a[2] = { *d, m->popts ? *m->popts : *d };
        void *h = (void *)0;
        void *b = (void *)0;
        int sv = fh_current_output();
        int slot = fh_find_fp(m->fp);
        int own = slot < 0;
        DESCR_t r;
        if (own) slot = fh_alloc(m->fp);
        if (slot < 0) return 0;
        if (own) { extern const char *fh_alias_name(int); extern void fh_set_alias(int, const char *); fh_set_alias(slot, fh_alias_name(sv)); }
        fflush(m->fp);
        fh_set_output(slot);
        r = rt_pl_goal_gen_h_c(g, a, m->pgoal ? 2 : 1, &h, &b, m->cx);
        if (!b && IS_FAIL_fn(r)) { extern void *rt_pl_ball_take(void); b = rt_pl_ball_take(); }
        fflush(m->fp);
        fh_set_output(sv);
        if (own) fh_free(slot);
        if (h) { extern void rt_proc_drop_frame_h(void **hslot); m->pheld++; rt_proc_drop_frame_h(&h); }
        if (b) { m->pthrown = b; return 0; }
        return !IS_FAIL_fn(r);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_wt(pl_cell_t *c, int quoted, int ignore_ops, int numbervars, long max_depth, long depth, int maxp, plc_vmap *m);
#define PLC_KID(cl, dp, mp) do { if (wx) plc_wt((cl), quoted, ignore_ops, numbervars, max_depth, (dp), (mp), m); else plc_wt_lv((cl), quoted, ignore_ops, numbervars, max_depth, (dp), (mp), m); } \
    while (0)
static void plc_wt_lv(pl_cell_t *c, int quoted, int ignore_ops, int numbervars, long max_depth, long depth, int maxp, plc_vmap *m) {
    FILE *fp = m->fp;
    const pl_tr_ctx_t *wx = m->portray ? m->cx : (const pl_tr_ctx_t *)0;
    plc_wsref rd = { -1, (pl_cell_t *)0 };
    if (!c) { plc_wt_atom(fp, "[]", quoted); return; }
    pl_cell_t *d = pl_deref(c);
    if (wx) plc_wsref_set(wx, &rd, d);
    if (m->pthrown || (m->portray && (m->portray(d, m) || m->pthrown))) return;
    if (wx) d = plc_wsref_get(wx, &rd);
    if (max_depth > 0 && depth >= max_depth) { fprintf(fp, "..."); return; }
    int tg = (int)d->v;
    if (pl_cell_unbound(d)) { { const char *vn = plc_vname(m, d); if (vn) fputs(vn, fp); else fprintf(fp, "_G%d", plc_vindex(m, d)); } return; }
    if (tg == DT_S) { plc_wt_string(fp, d, quoted); return; }
    if (tg == DT_PLATOM) { plc_wt_atom_n(fp, plc_atom_text(d), plc_atom_len(d), quoted); return; }
    if (tg == DT_I) { fprintf(fp, "%ld", (long)d->i); return; }
    if (tg == DT_R) { plc_wt_num(fp, d); return; }
    if (tg == DT_BIG) { extern char *rt_big_str(DESCR_t); char *bs = rt_big_str(*d); fputs(bs ? bs : "0", fp); return; }
    if (pl_rat_is(*d)) { pl_rat_fput(*d, fp); return; }
    if (rt_pl_eng_inst_is(*d)) { fprintf(fp, "<engine>(%ld,0x%lx)", (long)rt_pl_eng_inst_id(*d), (unsigned long)(uintptr_t)d->u); return; }
    if (tg != DT_PLREF) { { const char *vn = plc_vname(m, d); if (vn) fputs(vn, fp); else fprintf(fp, "_G%d", plc_vindex(m, d)); } return; }
    int fnid = plc_fid_name(d->slen), ar = plc_fid_arity(d->slen);
    pl_cell_t *aa = (pl_cell_t *)d->p;
    const char *fn = prolog_atom_name(fnid);
    if (!fn) fn = "?";
    if (numbervars && strcmp(fn, "$VAR") == 0 && ar == 1) {
        pl_cell_t *n = pl_deref(&aa[0]);
        if ((int)n->v == DT_I) {
            long num = (long)n->i;
            int letter = (int)(num % 26);
            long suf = num / 26;
            if (suf == 0) fprintf(fp, "%c", 'A' + letter);
            else fprintf(fp, "%c%ld", 'A' + letter, suf);
            return;
        }
        if ((int)n->v == DT_PLATOM || (int)n->v == DT_S) { const char *vn = plc_atom_text(n); fputs(vn ? vn : "", fp); return; }
    }
    if (numbervars && strcmp(fn, "$VARNAME") == 0 && ar == 1) {
        pl_cell_t *n = pl_deref(&aa[0]);
        if ((int)n->v == DT_PLATOM || (int)n->v == DT_S) { const char *vn = plc_atom_text(n); fprintf(fp, "%s", vn ? vn : "_"); return; }
    }
    if ((!ignore_ops || (ignore_ops & 2)) && fnid == g_runtime.atoms.atom_dot && ar == 2) {
        if (max_depth > 0 && !wx && rt_pl_dialect_gnu()) {
            long rem = max_depth - depth;
            int first = 1;
            pl_cell_t *gc = d;
            fputc('[', fp);
            for (;;) {
                pl_cell_t *ca = (pl_cell_t *)gc->p;
                pl_cell_t *hd = pl_deref(&ca[0]);
                int hv = pl_cell_unbound(hd);
                int hc = ((int)hd->v == DT_PLREF);
                int emit;
                pl_cell_t *tl;
                if (rem > 0 && (!first || hc || hv)) rem--;
                emit = (rem != 0 || first || hv);
                if (emit) { if (!first) fputs(m->space_args || m->next_arg ? ", " : ",", fp); PLC_KID(&ca[0], (hv && rem == 0) ? max_depth - 1 : max_depth - rem, 999); }
                tl = pl_deref(&ca[1]);
                if (rem == 0) { if (!emit || !plc_is_nil(tl)) fputs("|...", fp); break; }
                first = 0;
                if ((int)tl->v == DT_PLREF && plc_fid_name(tl->slen) == g_runtime.atoms.atom_dot && plc_fid_arity(tl->slen) == 2 && tl->p) { gc = tl; continue; }
                if (!plc_is_nil(tl)) { fputc('|', fp); PLC_KID(tl, max_depth - rem, 999); }
                break;
            }
            fputc(']', fp);
            return;
        }
        extern void *rt_pl_ball_kind1(const char *, const char *);
        pl_cell_t *cur = d;
        long n = 0;
        pl_cell_t *tort = d;
        plc_wsref rc = { -1, (pl_cell_t *)0 }, rtt = { -1, (pl_cell_t *)0 };
        if (wx) { plc_wsref_set(wx, &rc, cur); plc_wsref_set(wx, &rtt, tort); }
        fputc('[', fp);
        for (;;) {
            pl_cell_t *ca = (pl_cell_t *)cur->p;
            pl_cell_t *tl;
            if (max_depth > 0 && n > 0 && depth + 1 + n >= max_depth) { fprintf(fp, "|..."); break; }
            if (n) fputs(m->space_args || m->next_arg ? ", " : ",", fp);
            PLC_KID(&ca[0], depth + 1 + n, 999);
            n++;
            if (wx) { cur = plc_wsref_get(wx, &rc); tort = plc_wsref_get(wx, &rtt); ca = (pl_cell_t *)cur->p; }
            tl = pl_deref(&ca[1]);
            if ((int)tl->v == DT_PLREF && plc_fid_name(tl->slen) == g_runtime.atoms.atom_dot && plc_fid_arity(tl->slen) == 2 && tl->p) {
                cur = tl;
                if (cur == tort) { fprintf(fp, "|..."); if (!m->pthrown) m->pthrown = rt_pl_ball_kind1("resource_error", "cyclic_term"); break; }
                if ((n & (n - 1)) == 0) tort = cur;
                if (wx) { plc_wsref_set(wx, &rc, cur); plc_wsref_set(wx, &rtt, tort); }
                continue;
            }
            if (!plc_is_nil(tl)) { if (max_depth > 0 && depth + 1 + n >= max_depth) fputs("|...", fp); else { fputc('|', fp); PLC_KID(tl, depth + 1 + n, 999); } }
            break;
        }
        fputc(']', fp);
        return;
    }
    if ((ar & 1) && strcmp(fn, "$dict") == 0) {
        PLC_KID(&aa[0], depth+1, 0);
        if (wx) { d = plc_wsref_get(wx, &rd); aa = (pl_cell_t *)d->p; }
        fputc('{', fp);
        for (int i = 1; i < ar; i += 2) {
            if (i > 1) fputs(m->next_arg ? ", " : ",", fp);
            PLC_KID(&aa[i + 1], depth+1, 0);
            if (wx) { d = plc_wsref_get(wx, &rd); aa = (pl_cell_t *)d->p; }
            fputc(':', fp);
            if (plc_is_graphic_char(plc_first_char(&aa[i], quoted, ignore_ops, numbervars))) fputc(' ', fp);
            PLC_KID(&aa[i], depth+1, 999);
            if (wx) { d = plc_wsref_get(wx, &rd); aa = (pl_cell_t *)d->p; }
        }
        fputc('}', fp);
        return;
    }
    if ((!ignore_ops || (ignore_ops & 4)) && ar == 1 && strcmp(fn, "{}") == 0) { fputs(m->space_args ? "{ " : "{", fp); PLC_KID(&aa[0], depth+1, 1200); fputs(m->space_args ? " }" : "}", fp); return; }
    if (!ignore_ops && ar == 2 && strcmp(fn, "{}") == 0 && plc_op_is_postfix(fn)) {
        pl_cell_t *b0 = pl_deref(&aa[0]);
        int pp = 0, lm = 0, rm = 0;
        if ((int)b0->v == DT_PLREF && plc_fid_arity(b0->slen) == 1 && !strcmp(prolog_atom_name(plc_fid_name(b0->slen)), "{}") && plc_op_info(fn, 1, &pp, &lm, &rm)) {
            if (pp > maxp) fputc('(', fp);
            PLC_KID(&aa[1], depth+1, lm);
            if (wx) { d = plc_wsref_get(wx, &rd); aa = (pl_cell_t *)d->p; }
            fputc(' ', fp);
            PLC_KID(&aa[0], depth+1, 0);
            if (pp > maxp) fputc(')', fp);
            return;
        }
    }
    {
        int op_prec = 0, lmax = 0, rmax = 0;
        if (!ignore_ops && (ar == 1 || ar == 2) && plc_op_info(fn, ar, &op_prec, &lmax, &rmax)) {
            long kdl = max_depth > 0 ? max_depth - (depth + 1) : LONG_MAX;
            int kel = kdl <= 0;
            int wrap = op_prec > maxp;
            if (wrap) fputc('(', fp);
            if (ar == 2) {
                int xsbp = rt_pl_dialect_xsb() && !quoted;
                int lop = !kel && !xsbp && plc_is_op_atom(pl_deref(&aa[0])), rop = !kel && !xsbp && plc_is_op_atom(pl_deref(&aa[1]));
                if (lop) fputc('(', fp);
                PLC_KID(&aa[0], depth+1, lop ? 1200 : lmax);
                if (wx) { d = plc_wsref_get(wx, &rd); aa = (pl_cell_t *)d->p; }
                if (lop) fputc(')', fp);
                if (isalnum((unsigned char)fn[0]) || fn[0] == '_') {
                    if (rt_pl_dialect_swi() && !m->space_args) {
                        int lead = plc_alpha_glue((lop || (!kel && plc_term_prio(&aa[0], ignore_ops) > lmax)) ? ')' : plc_last_char_d(&aa[0], quoted, ignore_ops, numbervars, kdl));
                        int rf = (rop || (!kel && plc_term_prio(&aa[1], ignore_ops) > rmax)) ? '(' : plc_first_char_d(&aa[1], quoted, ignore_ops, numbervars, kdl);
                        fprintf(fp, "%s%s%s", lead ? " " : "", fn, (lead || plc_alpha_glue(rf)) ? " " : "");
                    } else fprintf(fp, " %s ", fn);
                } else if (!strcmp(fn, ",") || !strcmp(fn, "|")) {
                    fputc(fn[0], fp);
                    if (m->space_args || m->next_arg) fputc(' ', fp);
                } else if (m->space_args || (rt_pl_dialect_xsb() && strcmp(fn, ";"))) {
                    fputc(' ', fp);
                    plc_wt_atom(fp, fn, quoted);
                    fputc(' ', fp);
                } else if (!fn[0] && !quoted) {
                    fputc(' ', fp);
                } else {
                    int lglue = !lop && plc_is_graphic_char((unsigned char)fn[0]) && (kel || plc_term_prio(&aa[0], ignore_ops) <= lmax) &&
                        plc_is_graphic_char(plc_last_char_d(&aa[0], quoted, ignore_ops, numbervars, kdl));
                    if (lglue) fputc(' ', fp);
                    plc_wt_atom(fp, fn, quoted && strcmp(fn, ";"));
                    if (lglue || (!rop && (kel || plc_term_prio(&aa[1], ignore_ops) <= rmax) && plc_is_graphic_char(plc_first_char_d(&aa[1], quoted, ignore_ops, numbervars, kdl)))) fputc(' ', fp);
                }
                if (rop) fputc('(', fp);
                PLC_KID(&aa[1], depth+1, rop ? 1200 : rmax);
                if (rop) fputc(')', fp);
            } else if (plc_op_is_postfix(fn)) {
                int lglue = plc_is_graphic_char((unsigned char)fn[0]) && (kel || plc_term_prio(&aa[0], ignore_ops) <= lmax) &&
                    plc_is_graphic_char(plc_last_char_d(&aa[0], quoted, ignore_ops, numbervars, kdl));
                PLC_KID(&aa[0], depth+1, lmax);
                if (isalnum((unsigned char)fn[0]) || fn[0] == '_') fprintf(fp, " %s", fn);
                else { if (lglue) fputc(' ', fp); plc_wt_atom(fp, fn, quoted); }
            } else {
                {
                    pl_cell_t *a0 = pl_deref(&aa[0]);
                    int ap = kel ? 0 : plc_term_prio(a0, ignore_ops);
                    int alnum_op = isalnum((unsigned char)fn[0]) || fn[0] == '_';
                    extern int rt_big_sign(DESCR_t);
                    int a_num = !kel && (((int)a0->v == DT_I && a0->i >= 0) || ((int)a0->v == DT_R && a0->r >= 0) || ((int)a0->v == DT_BIG && rt_big_sign(*a0) >= 0));
                    int a_opatom = !kel && !(rt_pl_dialect_xsb() && !quoted) && plc_is_op_atom(a0);
                    int a_digit = !kel && !strcmp(fn, "-") && isdigit(plc_first_char(a0, quoted, ignore_ops, numbervars));
                    int swi_numop = rt_pl_dialect_swi() && !kel && !strcmp(fn, "-") && (a_num || a_digit);
                    int xsbw = rt_pl_dialect_xsb() && !quoted;
                    int needp = (ap > rmax) || a_opatom || (!swi_numop && !xsbw && ((a_num && !strcmp(fn, "-")) || (!rt_pl_dialect_xsb() && a_digit)));
                    int sep = needp || swi_numop || rt_pl_dialect_xsb() || (!alnum_op && plc_is_graphic_char(plc_first_char_d(&aa[0], quoted, ignore_ops, numbervars, kdl)));
                    if (alnum_op) fprintf(fp, "%s ", fn);
                    else plc_wt_atom(fp, fn, quoted);
                    if ((sep || m->space_args) && !alnum_op) fputc(' ', fp);
                    if (needp) fputc('(', fp);
                    PLC_KID(&aa[0], depth+1, needp ? 1200 : rmax);
                    if (needp) fputc(')', fp);
                }
            }
            if (wrap) fputc(')', fp);
            return;
        }
    }
    plc_wt_atom(fp, fn, quoted || (fn[0] == '.' && fn[1] == 0));
    fprintf(fp, "(");
    int nb = ar;
    if (max_depth > 0 && !wx && rt_pl_dialect_gnu() && max_depth - depth < ar) nb = (int)(max_depth - depth);
    for (int i = 0; i < nb; i++) {
        int opq = quoted && rt_pl_dialect_xsb() && !ignore_ops && plc_is_op_atom(pl_deref(&aa[i])) && strcmp(plc_atom_text(pl_deref(&aa[i])), "|");
        if (i) fputs(m->space_args || m->next_arg ? ", " : ",", fp);
        if (opq) fputc('(', fp);
        PLC_KID(&aa[i], depth+1, 999);
        if (opq) fputc(')', fp);
        if (wx) { d = plc_wsref_get(wx, &rd); aa = (pl_cell_t *)d->p; }
    }
    if (nb < ar) fputs(m->space_args || m->next_arg ? ", ..." : ",...", fp);
    fprintf(fp, ")");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#undef PLC_KID
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { void *blk; pl_cell_t *nb; int done; } plc_fz_ent_t;
typedef struct { plr_stk_t ag, lg; pl_cell_t *s; pl_cell_t *d; long next_k; int named; int dag; pl_cell_t knodes; plc_fz_ent_t *tab; long mask; } plc_fz_t;
static pl_cell_t plc_fz_var(plc_fz_t *z, long k) {
    if (z->named) {
        char nb[32];
        pl_cell_t *a = (pl_cell_t *)rt_ws_alloc_descr(1);
        snprintf(nb, sizeof nb, "S_%ld", k);
        *a = pl_make_atom(prolog_atom_intern(nb));
        return pl_make_compound(prolog_atom_intern("$VAR"), 1, a);
    }
    { pl_cell_t *v = (pl_cell_t *)rt_ws_alloc_descr(1); pl_init_var(v, -1); return pl_make_ref(v, (int)v->slen); }
}
static plc_fz_ent_t *plc_fz_slot(plc_fz_t *z, void *blk) {
    long i = (long)(((uintptr_t)blk >> 4) * 0x9E3779B97F4A7C15ull >> 20) & z->mask;
    while (z->tab[i].blk && z->tab[i].blk != blk) i = (i + 1) & z->mask;
    return &z->tab[i];
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_fz_run(void *wv) {
    plc_fz_t *w = (plc_fz_t *)wv;
    for (;;) {
        pl_cell_t *d = plr_resolve(pl_deref(w->s));
        int ar = (int)d->v == DT_PLREF ? plc_fid_arity(d->slen) : 0;
        plc_fz_ent_t *e = ar > 0 ? plc_fz_slot(w, d->p) : (plc_fz_ent_t *)0;
        if (e && e->blk) {
            pl_cell_t *nb = e->nb;
            if ((!e->done || w->dag) && nb[ar].v == DT_SNUL) {
                pl_cell_t *kc = (pl_cell_t *)rt_ws_alloc_descr(2);
                nb[ar] = plc_fz_var(w, ++w->next_k);
                kc[0] = pl_make_compound(plc_fid_name(d->slen), ar, nb);
                kc[1] = w->knodes;
                w->knodes = pl_make_compound(g_runtime.atoms.atom_dot, 2, kc);
            }
            *w->d = nb[ar].v != DT_SNUL ? nb[ar] : pl_make_compound(plc_fid_name(d->slen), ar, nb);
        } else if (e) {
            pl_cell_t *nb;
            if (!plr_room(&w->ag)) return plr_more(&w->ag, &w->lg, plc_fz_run, w);
            nb = (pl_cell_t *)rt_ws_alloc_descr((size_t)ar + 3);
            memset(&nb[ar], 0, sizeof nb[ar]);
            nb[ar + 1] = pl_make_int((int64_t)(intptr_t)w->d);
            nb[ar + 2] = pl_make_int((int64_t)(e - w->tab));
            e->blk = d->p;
            e->nb = nb;
            e->done = 0;
            *w->d = pl_make_compound(plc_fid_name(d->slen), ar, nb);
            plr_push(&w->ag, d->p, nb, (uint64_t)ar);
        } else *w->d = pl_cell_unbound(d) ? pl_make_ref(d, (int)d->slen) : *d;
        for (;;) {
            plr_ent_t *t;
            if (plr_empty(&w->ag)) return 0;
            t = plr_peek(&w->ag);
            if (!t->n) { w->tab[((pl_cell_t *)t->y)[2].i].done = 1; plr_drop(&w->ag); continue; }
            w->s = (pl_cell_t *)t->x;
            w->d = (pl_cell_t *)t->y;
            t->x = (void *)(w->s + 1);
            t->y = (void *)(w->d + 1);
            t->n--;
            break;
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_fz_count_visit(pl_cell_t *d, void *ctx) { if ((int)d->v == DT_PLREF) ++*(long *)ctx; return PLR_V_GO; }
PLR_WALK1(plc_fz_count_walk, plc_fz_count_visit, pl_deref) static pl_cell_t plc_factorize_x(pl_cell_t *c, int named, int dag) {
    long n = 0, cap = 64;
    pl_cell_t out, subs = pl_make_atom(g_runtime.atoms.atom_nil), *args = (pl_cell_t *)rt_ws_alloc_descr(2);
    plc_fz_t w;
    (void)plc_fz_count_walk(c, (void *)&n);
    while (cap < 2 * n + 2) cap <<= 1;
    {
        plc_fz_ent_t tab[cap];
        memset(tab, 0, sizeof tab);
        memset(&out, 0, sizeof out);
        plr_stk_init(&w.ag);
        plr_stk_init(&w.lg);
        w.s = c;
        w.d = &out;
        w.next_k = 0;
        w.named = named;
        w.dag = dag;
        w.knodes = pl_make_atom(g_runtime.atoms.atom_nil);
        w.tab = tab;
        w.mask = cap - 1;
        (void)plc_fz_run(&w);
    }
    for (pl_cell_t kl = w.knodes; (int)kl.v == DT_PLREF; kl = ((pl_cell_t *)kl.p)[1]) {
        pl_cell_t node = ((pl_cell_t *)kl.p)[0], *nb = (pl_cell_t *)node.p, *eq = (pl_cell_t *)rt_ws_alloc_descr(2), *cons = (pl_cell_t *)rt_ws_alloc_descr(2);
        int ar = plc_fid_arity(node.slen);
        *(pl_cell_t *)(intptr_t)nb[ar + 1].i = nb[ar];
        eq[0] = nb[ar];
        eq[1] = node;
        cons[0] = pl_make_compound(prolog_atom_intern("="), 2, eq);
        cons[1] = subs;
        subs = pl_make_compound(g_runtime.atoms.atom_dot, 2, cons);
    }
    args[0] = out;
    args[1] = subs;
    return pl_make_compound(prolog_atom_intern("@"), 2, args);
}
static pl_cell_t plc_factorize(pl_cell_t *c, int named) { return plc_factorize_x(c, named, 0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_acyclic_walk(pl_cell_t *c);
static void plc_wt(pl_cell_t *c, int quoted, int ignore_ops, int numbervars, long max_depth, long depth, int maxp, plc_vmap *m) {
    const pl_tr_ctx_t *wx = m->portray ? m->cx : (const pl_tr_ctx_t *)0;
    long mark = plc_ws_mark(wx);
    if (!depth && max_depth <= 0 && c && !pl_acyclic_walk(c)) {
        pl_cell_t *f = (pl_cell_t *)rt_ws_alloc_descr(1);
        plc_wsref rf = { -1, (pl_cell_t *)0 };
        *f = plc_factorize(c, numbervars);
        if (wx) plc_wsref_set(wx, &rf, f);
        fputs("@(", m->fp);
        plc_wt_lv(&((pl_cell_t *)f->p)[0], quoted, ignore_ops, numbervars, 0, 1, 999, m);
        if (wx) f = plc_wsref_get(wx, &rf);
        fputc(',', m->fp);
        plc_wt_lv(&((pl_cell_t *)f->p)[1], quoted, ignore_ops, numbervars, 0, 1, 999, m);
        fputc(')', m->fp);
        plc_ws_end(wx, mark);
        return;
    }
    plc_wt_lv(c, quoted, ignore_ops, numbervars, max_depth, depth, maxp, m);
    plc_ws_end(wx, mark);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_write(pl_cell_t *c, plc_vmap *m) { plc_wt(c, 0, 0, 1, 0, 0, rt_pl_dialect_xsb() ? 1000 : 1200, m); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_writeq(pl_cell_t *c, plc_vmap *m) { plc_wt(c, 1, 0, 1, 0, 0, rt_pl_dialect_xsb() ? 1000 : 1200, m); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_write_canonical(pl_cell_t *c, plc_vmap *m) { plc_wt(c, 1, (rt_pl_dialect_swi() || rt_pl_dialect_xsb()) ? 3 : PLC_WC_IGNORE_OPS, 0, 0, 0, 1200, m); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_atoms_ready(void) { extern void prolog_atom_init(void); if (g_runtime.atoms.atom_dot < 0) prolog_atom_init(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_write_cell(void *cell) {
    plc_atoms_ready();
    plc_vmap m;
    m.n = 0;
    m.vnc = 0;
    m.fp = plc_out();
    m.portray = (int (*)(pl_cell_t *, plc_vmap *))0;
    m.pthrown = (void *)0;
    m.cx = (pl_tr_ctx_t *)0;
    m.pheld = 0;
    m.pgoal = (pl_cell_t *)0;
    m.popts = (pl_cell_t *)0;
    m.space_args = 0;
    m.next_arg = 0;
    plc_write((pl_cell_t *)cell, &m);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_write_cell_fp(void *cell, FILE *fp) {
    plc_atoms_ready();
    plc_vmap m;
    m.n = 0;
    m.vnc = 0;
    m.fp = fp ? fp : plc_out();
    m.portray = (int (*)(pl_cell_t *, plc_vmap *))0;
    m.pthrown = (void *)0;
    m.cx = (pl_tr_ctx_t *)0;
    m.pheld = 0;
    m.pgoal = (pl_cell_t *)0;
    m.popts = (pl_cell_t *)0;
    m.space_args = 0;
    m.next_arg = 0;
    plc_write((pl_cell_t *)cell, &m);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_write_cell_fp_p(void *cell, FILE *fp, int maxp) {
    plc_atoms_ready();
    plc_vmap m;
    m.n = 0;
    m.vnc = 0;
    m.fp = fp ? fp : plc_out();
    m.portray = (int (*)(pl_cell_t *, plc_vmap *))0;
    m.pthrown = (void *)0;
    m.cx = (pl_tr_ctx_t *)0;
    m.pheld = 0;
    m.pgoal = (pl_cell_t *)0;
    m.popts = (pl_cell_t *)0;
    m.space_args = 0;
    m.next_arg = 0;
    plc_wt((pl_cell_t *)cell, 0, 0, 1, 0, 0, maxp, &m);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_writeq_cell(void *cell) {
    plc_atoms_ready();
    plc_vmap m;
    m.n = 0;
    m.vnc = 0;
    m.fp = plc_out();
    m.portray = (int (*)(pl_cell_t *, plc_vmap *))0;
    m.pthrown = (void *)0;
    m.cx = (pl_tr_ctx_t *)0;
    m.pheld = 0;
    m.pgoal = (pl_cell_t *)0;
    m.popts = (pl_cell_t *)0;
    m.space_args = 0;
    m.next_arg = 0;
    plc_writeq((pl_cell_t *)cell, &m);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_write_canonical_cell(void *cell) {
    plc_atoms_ready();
    plc_vmap m;
    m.n = 0;
    m.vnc = 0;
    m.fp = plc_out();
    m.portray = (int (*)(pl_cell_t *, plc_vmap *))0;
    m.pthrown = (void *)0;
    m.cx = (pl_tr_ctx_t *)0;
    m.pheld = 0;
    m.pgoal = (pl_cell_t *)0;
    m.popts = (pl_cell_t *)0;
    m.space_args = 0;
    m.next_arg = 0;
    plc_write_canonical((pl_cell_t *)cell, &m);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_display_cell_ball(void *cell) {
    plc_atoms_ready();
    plc_vmap m;
    m.n = 0;
    m.vnc = 0;
    m.fp = plc_out();
    m.portray = (int (*)(pl_cell_t *, plc_vmap *))0;
    m.pthrown = (void *)0;
    m.cx = (pl_tr_ctx_t *)0;
    m.pheld = 0;
    m.pgoal = (pl_cell_t *)0;
    m.popts = (pl_cell_t *)0;
    m.space_args = 0;
    m.next_arg = 0;
    plc_wt((pl_cell_t *)cell, 0, 1, 0, 0, 0, 1200, &m);
    return m.pthrown;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_display_cell(void *cell) { rt_pl_display_cell_ball(cell); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_opt_is_true(pl_cell_t *o) {
    if (!o) return 0;
    o = pl_deref(o);
    if ((int)o->v == DT_PLREF && plc_fid_arity(o->slen) == 1) {
        pl_cell_t *a = pl_deref((pl_cell_t *)o->p);
        return ((int)a->v == DT_PLATOM || (int)a->v == DT_S) && strcmp(plc_atom_text(a), "true") == 0;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_write_term_cell(void *term_cell, void *opts_cell, void *cxp) {
    plc_atoms_ready();
    plc_vmap m;
    m.n = 0;
    m.vnc = 0;
    m.fp = plc_out();
    m.portray = (int (*)(pl_cell_t *, plc_vmap *))0;
    m.pthrown = (void *)0;
    m.cx = (pl_tr_ctx_t *)0;
    m.pheld = 0;
    m.pgoal = (pl_cell_t *)0;
    m.popts = (pl_cell_t *)0;
    m.space_args = 0;
    m.next_arg = 0;
    int quoted = 0, ignore_ops = 0, numbervars = 0, prio = rt_pl_dialect_xsb() ? 1000 : 1200, noesc = 0, qna = 0, partial = 0, xesc = !rt_pl_flag_true("character_escapes_unicode"), nolists = 0,
        nobraces = 0;
    long max_depth = 0;
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
            else if (on && !strcmp(on, "character_escapes_unicode")) xesc = !plc_opt_is_true(o);
            else if (on && !strcmp(on, "partial")) partial = plc_opt_is_true(o);
            else if (on && !strcmp(on, "priority")) {
                pl_cell_t *a = pl_deref(&oa[0]);
                if ((int)a->v == DT_I && a->i >= 0 && a->i <= 1200) prio = (int)a->i;
            } else if (on && !strcmp(on, "ignore_ops")) ignore_ops = plc_opt_is_true(o);
            else if (on && (!strcmp(on, "no_lists") || !strcmp(on, "dotlists"))) nolists = plc_opt_is_true(o);
            else if (on && !strcmp(on, "brace_terms")) nobraces = !plc_opt_is_true(o);
            else if (on && !strcmp(on, "numbervars")) numbervars = plc_opt_is_true(o);
            else if (on && !strcmp(on, "space_args")) m.space_args = plc_opt_is_true(o);
            else if (on && !strcmp(on, "spacing")) {
                pl_cell_t *a = pl_deref(&oa[0]);
                m.next_arg = (int)a->v == DT_PLATOM && !strcmp(plc_atom_text(a), "next_argument");
            } else if (on && !strcmp(on, "portray_goal") && cxp) {
                m.pgoal = pl_deref(&oa[0]);
                m.popts = pl_deref((pl_cell_t *)opts_cell);
                m.cx = (pl_tr_ctx_t *)cxp;
                m.portray = plc_portray_hit;
            } else if (on && !strcmp(on, "max_depth")) {
                pl_cell_t *a = pl_deref(&oa[0]);
                if ((int)a->v == DT_I) max_depth = (long)a->i;
            } else if (on && !strcmp(on, "variable_names")) {
                m.vnc++;
                m.vnl = pl_deref((pl_cell_t *)opts_cell);
            }
        } else if ((int)o->v == DT_PLATOM && (rt_pl_dialect_swi() || rt_pl_dialect_xsb())) {
            const char *bn = plc_atom_text(o);
            if (bn && !strcmp(bn, "quoted")) quoted = 1;
            else if (bn && !strcmp(bn, "ignore_ops")) ignore_ops = 1;
            else if (bn && !strcmp(bn, "numbervars")) numbervars = 1;
            else if (bn && !strcmp(bn, "quote_non_ascii")) qna = 1;
            else if (bn && !strcmp(bn, "character_escapes_unicode")) xesc = 0;
            else if (bn && !strcmp(bn, "partial")) partial = 1;
            else if (bn && !strcmp(bn, "space_args")) m.space_args = 1;
        }
        lst = pl_deref(&pair[1]);
    }
    if (ignore_ops && rt_pl_dialect_swi()) ignore_ops = 1 | (nolists ? 0 : 2) | (nobraces ? 0 : 4);
    pl_cell_t *tt = pl_deref((pl_cell_t *)term_cell);
    int br = 0, p1 = 0, p2 = 0, lm, rm;
    if (partial && (int)tt->v == DT_PLATOM) { const char *an = plc_atom_text(tt); br = (plc_op_info(an, 1, &p1, &lm, &rm) && p1 > prio) || (plc_op_info(an, 2, &p2, &lm, &rm) && p2 > prio); }
    if (br) fputc('(', m.fp);
    plc_wt((pl_cell_t *)term_cell, quoted ? (1 | (noesc ? PLC_WQ_NOESC : 0) | (qna ? PLC_WQ_NONASCII : 0) | (xesc ? PLC_WQ_XESC : 0)) : 0, ignore_ops, numbervars, max_depth, 0, prio, &m);
    if (br) fputc(')', m.fp);
    return m.pthrown;
}
extern const char *prolog_atom_name(int id);
extern int rt_last_ok(void);
extern long rt_arith(int lk, long li, const char *ls, int rk, long ri, const char *rs, const char *op);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_atom_id_of(pl_cell_t *d) {
    if ((int)d->v == DT_PLATOM) return (int)d->i;
    if ((int)d->v == DT_S) { extern int prolog_atom_intern(const char *); return prolog_atom_intern(d->s ? d->s : ""); }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_is_atomlike(pl_cell_t *d) { return (int)d->v == DT_PLATOM || (int)d->v == DT_S; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static pl_cell_t plc_atom_id_cell(int id) { return pl_make_atom(id); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_ground_visit(pl_cell_t *d, void *ctx) {
    (void)ctx;
    if (pl_cell_unbound(d)) return PLR_V_STOP;
    return ((int)d->v == DT_PLREF && plc_fid_arity(d->slen) > 0 && !d->p) ? PLR_V_STOP : PLR_V_GO;
}
PLR_WALK1(plc_ground_walk, plc_ground_visit, pl_deref) static int plc_cell_is_ground(pl_cell_t *c) { return c && !plc_ground_walk(c, (void *)0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_cell_is_proper_list(pl_cell_t *c) {
    pl_cell_t *d = c ? pl_deref(c) : (pl_cell_t *)0;
    pl_cell_t *tort = d;
    unsigned long n = 0;
    while (d) {
        if (plc_is_atomlike(d) && plc_atom_id_of(d) == g_runtime.atoms.atom_nil) return 1;
        if ((int)d->v == DT_PLREF && plc_fid_name(d->slen) == g_runtime.atoms.atom_dot && plc_fid_arity(d->slen) == 2 && d->p) {
            d = pl_deref(&((pl_cell_t *)d->p)[1]);
            n++;
            if (d == tort) return 0;
            if ((n & (n - 1)) == 0) tort = d;
            continue;
        }
        return 0;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_type_test_cell(void *cell_term, const char *fn) {
    pl_cell_t *d = cell_term ? pl_deref((pl_cell_t *)cell_term) : (pl_cell_t *)0;
    int isvar = (!d || pl_cell_unbound(d));
    int isatom = (d && !isvar && (int)d->v == DT_PLATOM);
    int isstr = (d && !isvar && (int)d->v == DT_S);
    int isint = (d && !isvar && ((int)d->v == DT_I || (int)d->v == DT_BIG));
    int isfloat = (d && !isvar && (int)d->v == DT_R);
    int iscomp = (d && !isvar && (int)d->v == DT_PLREF);
    int israt = (d && !isvar && pl_rat_is(*d));
    int isblob = (d && !isvar && rt_pl_eng_inst_is(*d));
    if (!fn) return 0;
    if (strcmp(fn, "var") == 0) return isvar ? 1 : 0;
    if (strcmp(fn, "nonvar") == 0) return !isvar ? 1 : 0;
    if (strcmp(fn, "atom") == 0) return isatom ? 1 : 0;
    if (strcmp(fn, "integer") == 0) return isint ? 1 : 0;
    if (strcmp(fn, "float") == 0) return isfloat ? 1 : 0;
    if (strcmp(fn, "number") == 0) return (isint || isfloat || israt) ? 1 : 0;
    if (strcmp(fn, "rational") == 0) return (isint || israt) ? 1 : 0;
    if (strcmp(fn, "atomic") == 0) return (isatom || isstr || isint || isfloat || israt || isblob) ? 1 : 0;
    if (strcmp(fn, "string") == 0) return isstr ? 1 : 0;
    if (strcmp(fn, "compound") == 0) return iscomp ? 1 : 0;
    if (strcmp(fn, "callable") == 0) return (isatom || iscomp) ? 1 : 0;
    if (strcmp(fn, "ground") == 0) return plc_cell_is_ground((pl_cell_t *)cell_term) ? 1 : 0;
    if (strcmp(fn, "is_list") == 0) return plc_cell_is_proper_list((pl_cell_t *)cell_term) ? 1 : 0;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int plc_swi_list_name(int name, int arity) { extern int rt_pl_dialect_swi(void); return (arity == 2 && name == g_runtime.atoms.atom_dot && rt_pl_dialect_swi()) ? prolog_atom_intern("[|]") : name; }
int plc_swi_list_build(int name, int arity) { extern int rt_pl_dialect_swi(void); return (arity == 2 && rt_pl_dialect_swi() && name == prolog_atom_intern("[|]")) ? g_runtime.atoms.atom_dot : name; }
int rt_pl_functor_cell(void *t0_cell, void *name_cell, void *arity_cell, pl_tr_ctx_t *cx) {
    pl_cell_t *d0 = t0_cell ? pl_deref((pl_cell_t *)t0_cell) : (pl_cell_t *)0;
    pl_cell_t *d1 = name_cell ? pl_deref((pl_cell_t *)name_cell) : (pl_cell_t *)0;
    if (d0 && !pl_cell_unbound(d0)) {
        pl_cell_t nameV, arityV;
        if ((int)d0->v == DT_PLREF && plc_fid_arity(d0->slen) == 0) {
            extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
            if (cx) cx->ball = rt_pl_ball_kind2("domain_error", "compound_non_zero_arity", *d0);
            return 0;
        }
        if ((int)d0->v == DT_PLREF) {
            nameV = plc_atom_id_cell(plc_swi_list_name(plc_fid_name(d0->slen), plc_fid_arity(d0->slen)));
            arityV = pl_make_int((int64_t)plc_fid_arity(d0->slen));
        } else if ((int)d0->v == DT_S) {
            nameV = *d0;
            arityV = pl_make_int(0);
        } else if (plc_is_atomlike(d0)) {
            nameV = plc_atom_id_cell(plc_atom_id_of(d0));
            arityV = pl_make_int(0);
        } else if ((int)d0->v == DT_I) {
            nameV = pl_make_int(d0->i);
            arityV = pl_make_int(0);
        } else if ((int)d0->v == DT_R) {
            nameV = pl_make_float(d0->r);
            arityV = pl_make_int(0);
        } else if ((int)d0->v == DT_BIG || pl_rat_is(*d0)) {
            nameV = *d0;
            arityV = pl_make_int(0);
        } else {
            return 0;
        }
        if (!plc_unify_cells_cx((pl_cell_t *)name_cell, &nameV, cx) || !plc_unify_cells_cx((pl_cell_t *)arity_cell, &arityV, cx)) { return 0; }
        return 1;
    }
    pl_cell_t *d2 = arity_cell ? pl_deref((pl_cell_t *)arity_cell) : (pl_cell_t *)0;
    if (!d2 || (int)d2->v != DT_I) { return 0; }
    long ar = (long)d2->i;
    pl_cell_t built;
    if (ar == 0) {
        built = d1 ? *d1 : plc_atom_id_cell(prolog_atom_intern("[]"));
    } else {
        if (!d1 || !plc_is_atomlike(d1)) { return 0; }
        pl_cell_t *args = (pl_cell_t *)PL_CELL_ALLOC((size_t)ar * sizeof(pl_cell_t));
        for (long i = 0; i < ar; i++) pl_init_var(&args[i], -1);
        built = pl_make_compound(plc_swi_list_build(plc_atom_id_of(d1), (int)ar), (int)ar, args);
    }
    if (!plc_unify_cells_cx((pl_cell_t *)t0_cell, &built, cx)) { return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_arg_cell(void *n_cell, void *t_cell, void *arg_cell, pl_tr_ctx_t *cx) {
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
int rt_pl_univ_cell(void *t0_cell, void *list_cell, pl_tr_ctx_t *cx) {
    pl_cell_t *d0 = t0_cell ? pl_deref((pl_cell_t *)t0_cell) : (pl_cell_t *)0;
    if (d0 && !pl_cell_unbound(d0)) {
        pl_cell_t lst = plc_atom_id_cell(prolog_atom_intern("[]"));
        if ((int)d0->v == DT_PLREF && plc_fid_arity(d0->slen) == 0) {
            extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
            if (cx) cx->ball = rt_pl_ball_kind2("domain_error", "compound_non_zero_arity", *d0);
            return 0;
        }
        if ((int)d0->v == DT_PLREF) {
            int ar = plc_fid_arity(d0->slen);
            pl_cell_t *aa = (pl_cell_t *)d0->p;
            for (int i = ar - 1; i >= 0; i--) { pl_cell_t *c = (pl_cell_t *)PL_CELL_ALLOC(2 * sizeof(pl_cell_t)); c[0] = aa[i]; c[1] = lst; lst = pl_make_compound(g_runtime.atoms.atom_dot, 2, c); }
            pl_cell_t *c = (pl_cell_t *)PL_CELL_ALLOC(2 * sizeof(pl_cell_t));
            c[0] = plc_atom_id_cell(plc_swi_list_name(plc_fid_name(d0->slen), plc_fid_arity(d0->slen)));
            c[1] = lst;
            lst = pl_make_compound(g_runtime.atoms.atom_dot, 2, c);
        } else {
            pl_cell_t *c = (pl_cell_t *)PL_CELL_ALLOC(2 * sizeof(pl_cell_t));
            c[0] = *d0;
            c[1] = lst;
            lst = pl_make_compound(g_runtime.atoms.atom_dot, 2, c);
        }
        if (!plc_unify_cells_cx((pl_cell_t *)list_cell, &lst, cx)) { return 0; }
        return 1;
    }
    extern void *rt_pl_ball_instantiation(void);
    extern void *rt_pl_ball_kind1(const char *, const char *);
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    pl_cell_t *l0 = list_cell ? pl_deref((pl_cell_t *)list_cell) : (pl_cell_t *)0, *cur = l0;
    void *b = (void *)0;
    int ne = 0;
    long nl = plc_list_cells(list_cell, g_runtime.atoms.atom_dot);
    while (nl >= 0 && cur && (int)cur->v == DT_PLREF && plc_fid_name(cur->slen) == g_runtime.atoms.atom_dot && plc_fid_arity(cur->slen) == 2 && cur->p) {
        ne++;
        cur = pl_deref(&((pl_cell_t *)cur->p)[1]);
    }
    if (nl < 0) b = rt_pl_ball_kind2("type_error", "list", *l0);
    else if (!cur || pl_cell_unbound(cur)) b = rt_pl_ball_instantiation();
    else if (!(plc_is_atomlike(cur) && plc_atom_id_of(cur) == g_runtime.atoms.atom_nil)) b = rt_pl_ball_kind2("type_error", "list", *l0);
    else if (ne == 0) b = rt_pl_ball_kind2("domain_error", "non_empty_list", *l0);
    if (b) { if (cx && !cx->ball) cx->ball = b; return 0; }
    pl_cell_t *h = pl_deref(&((pl_cell_t *)l0->p)[0]);
    if (pl_cell_unbound(h)) b = rt_pl_ball_instantiation();
    else if (ne == 1 && (int)h->v == DT_PLREF) b = rt_pl_ball_kind2("type_error", "atomic", *h);
    else if (ne > 1 && !plc_is_atomlike(h)) b = rt_pl_ball_kind2("type_error", "atom", *h);
    else if ((long)ne - 1 > PROLOG_MAX_ARITY) b = rt_pl_ball_kind1("representation_error", "max_arity");
    if (b) { if (cx && !cx->ball) cx->ball = b; return 0; }
    pl_cell_t built;
    if (ne == 1) {
        built = *h;
    } else {
        pl_cell_t *args = (pl_cell_t *)PL_CELL_ALLOC((size_t)(ne - 1) * sizeof(pl_cell_t));
        int i = 0;
        for (cur = pl_deref(&((pl_cell_t *)l0->p)[1]); i < ne - 1; cur = pl_deref(&((pl_cell_t *)cur->p)[1])) args[i++] = *pl_deref(&((pl_cell_t *)cur->p)[0]);
        built = pl_make_compound(plc_swi_list_build(plc_atom_id_of(h), ne - 1), ne - 1, args);
    }
    if (!plc_unify_cells_cx((pl_cell_t *)t0_cell, &built, cx)) { return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_sp_ball(pl_tr_ctx_t *cx, void *b) { if (cx && !cx->ball) cx->ball = b; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_sp_int_arg(pl_cell_t *d, pl_tr_ctx_t *cx, int *bound) {
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    *bound = 0;
    if (!d || pl_cell_unbound(d)) return 1;
    if ((int)d->v != DT_I && (int)d->v != DT_BIG) { plc_sp_ball(cx, rt_pl_ball_kind2("type_error", "integer", *d)); return 0; }
    *bound = 1;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_sp_not_negative(pl_cell_t *d, pl_tr_ctx_t *cx) {
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    extern int rt_big_sign(DESCR_t);
    if (d && ((int)d->v == DT_I || (int)d->v == DT_BIG) && rt_big_sign(*d) < 0) { plc_sp_ball(cx, rt_pl_ball_kind2("domain_error", "not_less_than_zero", *d)); return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static pl_cell_t plc_sp_addsub(pl_cell_t a, pl_cell_t b, int sub) {
    extern DESCR_t rt_big_add(DESCR_t, DESCR_t);
    extern DESCR_t rt_big_sub(DESCR_t, DESCR_t);
    long long r;
    if ((int)a.v == DT_I && (int)b.v == DT_I && !(sub ? __builtin_sub_overflow(a.i, b.i, &r) : __builtin_add_overflow(a.i, b.i, &r))) return pl_make_int(r);
    return sub ? rt_big_sub(a, b) : rt_big_add(a, b);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_wall_clock_cell(int ms, void *a_cell, pl_tr_ctx_t *cx) {
    struct timespec _ts;
    clock_gettime(ms == 2 ? CLOCK_PROCESS_CPUTIME_ID : CLOCK_MONOTONIC, &_ts);
    long long us = (long long)_ts.tv_sec * 1000000LL + (long long)_ts.tv_nsec / 1000LL;
    pl_cell_t v = pl_make_int(ms ? us / 1000LL : us);
    return plc_unify_cells_cx((pl_cell_t *)a_cell, &v, cx) ? 1 : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_succ_plus_cell(long arity, void *a_cell, void *b_cell, void *c_cell, pl_tr_ctx_t *cx) {
    extern void *rt_pl_ball_instantiation(void);
    pl_cell_t *da = a_cell ? pl_deref((pl_cell_t *)a_cell) : (pl_cell_t *)0;
    pl_cell_t *db = b_cell ? pl_deref((pl_cell_t *)b_cell) : (pl_cell_t *)0;
    pl_cell_t *dc = c_cell ? pl_deref((pl_cell_t *)c_cell) : (pl_cell_t *)0;
    if (arity == 2) {
        int va, vb;
        if (!plc_sp_int_arg(da, cx, &va) || !plc_sp_int_arg(db, cx, &vb)) return 0;
        if (!va && !vb) { plc_sp_ball(cx, rt_pl_ball_instantiation()); return 0; }
        if (!plc_sp_not_negative(da, cx) || !plc_sp_not_negative(db, cx)) return 0;
        if (va) { pl_cell_t v = plc_sp_addsub(*da, pl_make_int(1), 0); return plc_unify_cells_cx((pl_cell_t *)b_cell, &v, cx) ? 1 : 0; }
        if ((int)db->v == DT_I && db->i == 0) return 0;
        { pl_cell_t v = plc_sp_addsub(*db, pl_make_int(1), 1); return plc_unify_cells_cx((pl_cell_t *)a_cell, &v, cx) ? 1 : 0; }
    }
    if (arity == 3) {
        int va, vb, vc;
        if (!plc_sp_int_arg(da, cx, &va) || !plc_sp_int_arg(db, cx, &vb) || !plc_sp_int_arg(dc, cx, &vc)) return 0;
        if (va + vb + vc < 2) { plc_sp_ball(cx, rt_pl_ball_instantiation()); return 0; }
        if (va && vb) { pl_cell_t v = plc_sp_addsub(*da, *db, 0); return plc_unify_cells_cx((pl_cell_t *)c_cell, &v, cx) ? 1 : 0; }
        if (va && vc) { pl_cell_t v = plc_sp_addsub(*dc, *da, 1); return plc_unify_cells_cx((pl_cell_t *)b_cell, &v, cx) ? 1 : 0; }
        { pl_cell_t v = plc_sp_addsub(*dc, *db, 1); return plc_unify_cells_cx((pl_cell_t *)a_cell, &v, cx) ? 1 : 0; }
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *plc_atom_op_list_text_heap(pl_cell_t *d, size_t *len);
static const char *plc_atom_op_text_n(pl_cell_t *t, char *buf, size_t bufsz, size_t *len);
static const char *plc_text_nil_empty(pl_cell_t *t, char *buf, size_t bufsz, size_t *len) {
    if (t && rt_pl_dialect_swi() && plc_is_nil(pl_deref(t))) { *len = 0; return ""; }
    return plc_atom_op_text_n(t, buf, bufsz, len);
}
static const char *plc_atom_op_text_n(pl_cell_t *t, char *buf, size_t bufsz, size_t *len) {
    const char *r = (const char *)0;
    *len = 0;
    if (!t) return r;
    pl_cell_t *d = pl_deref(t);
    if ((int)d->v == DT_PLATOM || (int)d->v == DT_S) { *len = plc_atom_len(d); return plc_atom_text(d); }
    if ((int)d->v == DT_PLREF) return plc_atom_op_list_text_heap(d, len);
    if ((int)d->v == DT_I) {
        snprintf(buf, bufsz, "%ld", (long)d->i);
        r = buf;
    } else if ((int)d->v == DT_BIG) {
        extern char *rt_big_str(DESCR_t);
        r = rt_big_str(*d);
    } else if (pl_rat_is(*d)) {
        size_t n = pl_rat_text(*d, (char *)0, 0) + 1;
        char *o = n <= bufsz ? buf : (char *)rt_wsb_alloc(n);
        (void)pl_rat_text(*d, o, n);
        r = o;
    } else if ((int)d->v == DT_R) {
        extern const char *pl_real_iso_str(double, char *, int);
        r = pl_real_iso_str(d->r, buf, (int)bufsz);
    }
    if (r) *len = strlen(r);
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *plc_atom_op_text(pl_cell_t *t, char *buf, size_t bufsz) { size_t n; return plc_atom_op_text_n(t, buf, bufsz, &n); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_num_of_text(const char *s, pl_cell_t *out) {
    extern int rt_pl_parse_number(const char *, DESCR_t *);
    extern int rt_pl_flag_is(const char *, const char *);
    if (!s || !rt_pl_parse_number(s, out)) return 0;
    return !((int)out->v == DT_R && isinf(out->r) && !strstr(s, "Inf") && !strstr(s, "inf") && !rt_pl_flag_is("float_overflow", "infinity"));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static pl_cell_t plc_make_atom_cell(const char *name) { int id = prolog_atom_intern(name ? name : ""); const char *nm = prolog_atom_name(id); (void)nm; return pl_make_atom(id); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static pl_cell_t plc_make_atom_cell_n(const char *s, size_t n) { return pl_make_atom(prolog_atom_intern_n(s ? s : "", s ? n : 0)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_unify_into_cell(pl_cell_t *dst, pl_cell_t val) { pl_cell_t tmp = val; return pl_unify(dst, &tmp); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
pl_cell_t rt_pl_make_string_cell_n(const char *s, size_t n) {
    pl_cell_t c = {0};
    c.v = (DTYPE_t)DT_S;
    c.s = (char *)rt_heap_alloc_c(n + 1);
    if (n) memcpy(c.s, s, n);
    c.s[n] = 0;
    c.slen = (uint32_t)n;
    return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
pl_cell_t rt_pl_make_string_cell(const char *s) { if (!s) s = ""; return rt_pl_make_string_cell_n(s, strlen(s)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_text_out_n(void *cell, const char *s, size_t n, int as_str, pl_tr_ctx_t *cx) {
    pl_cell_t *d = cell ? pl_deref((pl_cell_t *)cell) : (pl_cell_t *)0;
    if (d && !pl_cell_unbound(d) && ((int)d->v == DT_S || (int)d->v == DT_PLATOM)) return plc_atom_len(d) == n && (!n || !memcmp(plc_atom_text(d), s, n));
    return plc_unify_into_cell_cx((pl_cell_t *)cell, as_str ? rt_pl_make_string_cell_n(s, n) : plc_make_atom_cell_n(s, n), cx);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_text_out(void *cell, const char *s, int as_str, pl_tr_ctx_t *cx) { return plc_text_out_n(cell, s, s ? strlen(s) : 0, as_str, cx); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *plc_text_carve(size_t n, int as_str, pl_tr_ctx_t *cx) {
    extern int rt_pl_cap_refuses(unsigned long long, unsigned long long);
    extern void *rt_pl_cap_error(void);
    char *b = (char *)0;
    if (!as_str) b = prolog_atom_carve(n);
    else if (!rt_pl_cap_refuses((unsigned long long)n + 1ULL, 1ULL)) b = (char *)rt_heap_alloc_c(n + 1);
    if (!b && cx && !cx->ball) cx->ball = rt_pl_cap_error();
    return b;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static pl_cell_t plc_atom_built(char *b, size_t n, size_t k) { return pl_make_atom(prolog_atom_carved(b, n, k)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_text_built(void *cell, char *b, size_t n, size_t k, int as_str, pl_tr_ctx_t *cx) {
    pl_cell_t *d = cell ? pl_deref((pl_cell_t *)cell) : (pl_cell_t *)0;
    if (d && !pl_cell_unbound(d) && ((int)d->v == DT_S || (int)d->v == DT_PLATOM)) {
        int eq = plc_atom_len(d) == k && (!k || !memcmp(plc_atom_text(d), b, k));
        if (!as_str) prolog_atom_uncarve(b, n);
        return eq;
    }
    if (as_str) { pl_cell_t c = {0}; b[k] = 0; c.v = (DTYPE_t)DT_S; c.s = b; c.slen = (uint32_t)k; return plc_unify_into_cell_cx((pl_cell_t *)cell, c, cx); }
    return plc_unify_into_cell_cx((pl_cell_t *)cell, plc_atom_built(b, n, k), cx);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_u8_get(const char *s, int *adv) {
    const unsigned char *u = (const unsigned char *)s;
    unsigned c = u[0];
    if (c < 0x80) { *adv = 1; return (int)c; }
    if ((c & 0xE0) == 0xC0 && (u[1] & 0xC0) == 0x80) { *adv = 2; return (int)(((c & 0x1Fu) << 6) | (u[1] & 0x3Fu)); }
    if ((c & 0xF0) == 0xE0 && (u[1] & 0xC0) == 0x80 && (u[2] & 0xC0) == 0x80) { *adv = 3; return (int)(((c & 0x0Fu) << 12) | ((u[1] & 0x3Fu) << 6) | (u[2] & 0x3Fu)); }
    if ((c & 0xF8) == 0xF0 && (u[1] & 0xC0) == 0x80 && (u[2] & 0xC0) == 0x80 && (u[3] & 0xC0) == 0x80) {
        *adv = 4;
        return (int)(((c & 0x07u) << 18) | ((u[1] & 0x3Fu) << 12) | ((u[2] & 0x3Fu) << 6) | (u[3] & 0x3Fu));
    }
    *adv = 1;
    return (int)c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static size_t pl_u8_count_n(const char *s, size_t len) { size_t n = 0; int a; for (const char *e = s + len; s < e; s += a) { (void)rt_pl_u8_get(s, &a); n++; } return n; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_u8_put(char *o, int cp) {
    if (cp < 0) return 0;
    if (cp < 0x80) { o[0] = (char)cp; return 1; }
    if (cp < 0x800) { o[0] = (char)(0xC0 | (cp >> 6)); o[1] = (char)(0x80 | (cp & 0x3F)); return 2; }
    if (cp < 0x10000) { o[0] = (char)(0xE0 | (cp >> 12)); o[1] = (char)(0x80 | ((cp >> 6) & 0x3F)); o[2] = (char)(0x80 | (cp & 0x3F)); return 3; }
    o[0] = (char)(0xF0 | (cp >> 18));
    o[1] = (char)(0x80 | ((cp >> 12) & 0x3F));
    o[2] = (char)(0x80 | ((cp >> 6) & 0x3F));
    o[3] = (char)(0x80 | (cp & 0x3F));
    return 4;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *plc_atom_op_list_text_heap(pl_cell_t *d, size_t *len) {
    extern void *rt_wsb_alloc(size_t);
    extern void *rt_wsb_realloc(void *, size_t);
    size_t oi = 0, cap = 64;
    char *buf = (char *)rt_wsb_alloc(cap);
    while (d && (int)d->v == DT_PLREF && plc_fid_name(d->slen) == g_runtime.atoms.atom_dot && plc_fid_arity(d->slen) == 2 && d->p) {
        pl_cell_t *pr = (pl_cell_t *)d->p;
        pl_cell_t *el = pl_deref(&pr[0]);
        if ((int)el->v == DT_I) {
            if (oi + 8 >= cap) { cap *= 2; buf = (char *)rt_wsb_realloc(buf, cap); }
            oi += (size_t)rt_pl_u8_put(buf + oi, (int)el->i);
        } else if (plc_is_atomlike(el)) {
            const char *cn = plc_atom_text(el);
            size_t cl = cn ? plc_atom_len(el) : 0;
            if (!cn || !cl) return (const char *)0;
            while (oi + cl + 1 > cap) cap *= 2;
            buf = (char *)rt_wsb_realloc(buf, cap);
            memcpy(buf + oi, cn, cl);
            oi += cl;
        } else return (const char *)0;
        d = pl_deref(&pr[1]);
    }
    if (!(d && plc_is_atomlike(d) && plc_atom_id_of(d) == g_runtime.atoms.atom_nil)) return (const char *)0;
    buf[oi] = '\0';
    *len = oi;
    return buf;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_atom_op_cell(const char *fn, void *a0_cell, void *a1_cell, void *a2_cell, pl_tr_ctx_t *cx) {
    pl_cell_t *t0 = a0_cell ? pl_deref((pl_cell_t *)a0_cell) : (pl_cell_t *)0;
    pl_cell_t *t1 = a1_cell ? pl_deref((pl_cell_t *)a1_cell) : (pl_cell_t *)0;
    pl_cell_t *t2 = a2_cell ? pl_deref((pl_cell_t *)a2_cell) : (pl_cell_t *)0;
    char buf0[512], buf1[512];
    if (!strcmp(fn, "atom_length")) {
        size_t sn;
        const char *s = plc_text_nil_empty(t0, buf0, sizeof buf0, &sn);
        if (!s) { return 0; }
        if (!plc_unify_into_cell_cx((pl_cell_t *)a1_cell, pl_make_int((int64_t)pl_u8_count_n(s, sn)), cx)) { return 0; }
        return 1;
    }
    if (!strcmp(fn, "atomic_concat")) {
        size_t l0, l1;
        const char *s0 = plc_atom_op_text_n(t0, buf0, sizeof buf0, &l0);
        const char *s1 = plc_atom_op_text_n(t1, buf1, sizeof buf1, &l1);
        if (!s0 || !s1) { return 0; }
        {
            char *cat = plc_text_carve(l0 + l1, 0, cx);
            if (!cat) { return 0; }
            memcpy(cat, s0, l0);
            memcpy(cat + l0, s1, l1);
            return plc_unify_into_cell_cx((pl_cell_t *)a2_cell, plc_atom_built(cat, l0 + l1, l0 + l1), cx) ? 1 : 0;
        }
    }
    if (!strcmp(fn, "atom_concat")) {
        size_t l0, l1;
        const char *s0 = plc_atom_op_text_n(t0, buf0, sizeof buf0, &l0);
        const char *s1 = plc_atom_op_text_n(t1, buf1, sizeof buf1, &l1);
        if (!s0 || !s1) {
            char bufz[512];
            size_t lz;
            const char *sz = plc_atom_op_text_n(t2, bufz, sizeof bufz, &lz);
            if (!sz) { return 0; }
            {
                if (s0 && !s1) { if (l0 > lz || memcmp(sz, s0, l0)) { return 0; } return plc_unify_into_cell_cx((pl_cell_t *)a1_cell, plc_make_atom_cell_n(sz + l0, lz - l0), cx) ? 1 : 0; }
                if (!s0 && s1) { if (l1 > lz || memcmp(sz + (lz - l1), s1, l1)) { return 0; } return plc_unify_into_cell_cx((pl_cell_t *)a0_cell, plc_make_atom_cell_n(sz, lz - l1), cx) ? 1 : 0; }
            }
            return 0;
        }
        char *cat = plc_text_carve(l0 + l1, 0, cx);
        if (!cat) { return 0; }
        memcpy(cat, s0, l0);
        memcpy(cat + l0, s1, l1);
        if (!plc_unify_into_cell_cx((pl_cell_t *)a2_cell, plc_atom_built(cat, l0 + l1, l0 + l1), cx)) { return 0; }
        return 1;
    }
    if (!strcmp(fn, "upcase_atom") || !strcmp(fn, "downcase_atom")) {
        size_t n;
        const char *s = plc_atom_op_text_n(t0, buf0, sizeof buf0, &n);
        if (!s) { return 0; }
        char *out = plc_text_carve(n, 0, cx);
        int up = (!strcmp(fn, "upcase_atom"));
        if (!out) { return 0; }
        for (size_t i = 0; i < n; i++) out[i] = up ? (char)toupper((unsigned char)s[i]) : (char)tolower((unsigned char)s[i]);
        if (!plc_unify_into_cell_cx((pl_cell_t *)a1_cell, plc_atom_built(out, n, n), cx)) { return 0; }
        return 1;
    }
    int as_codes = (!strcmp(fn, "atom_codes") || !strcmp(fn, "string_codes"));
    if (!strcmp(fn, "atom_chars") || !strcmp(fn, "string_chars") || as_codes) {
        int swi = !strncmp(fn, "string_", 7);
        extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
        extern void *rt_pl_ball_instantiation(void);
        if (t0 && !pl_cell_unbound(t0)) {
            size_t sn;
            const char *s = swi ? plc_text_nil_empty(t0, buf0, sizeof buf0, &sn) : plc_atom_op_text_n(t0, buf0, sizeof buf0, &sn);
            if (!s) { if (swi && cx && !cx->ball) cx->ball = rt_pl_ball_kind2("type_error", "text", *t0); return 0; }
            if (t1 && (int)t1->v == DT_S) return plc_atom_len(t1) == sn && (!sn || !memcmp(plc_atom_text(t1), s, sn));
            if (rt_pl_dialect_swi() && t1 && (int)t1->v == DT_PLREF && plc_fid_name(t1->slen) == g_runtime.atoms.atom_dot && plc_fid_arity(t1->slen) == 2 && t1->p) {
                pl_cell_t *c = t1, *e0 = pl_deref(&((pl_cell_t *)t1->p)[0]);
                while (c && (int)c->v == DT_PLREF && plc_fid_name(c->slen) == g_runtime.atoms.atom_dot && plc_fid_arity(c->slen) == 2 && c->p) {
                    if (pl_cell_unbound(pl_deref(&((pl_cell_t *)c->p)[0]))) break;
                    c = pl_deref(&((pl_cell_t *)c->p)[1]);
                }
                if (c && plc_is_atomlike(c) && plc_atom_id_of(c) == g_runtime.atoms.atom_nil) as_codes = (int)e0->v == DT_I;
            }
            int *cps = (int *)rt_wsb_alloc((sn + 1) * sizeof(int));
            int cn = 0, adv;
            for (const char *q = s; q < s + sn; q += adv) { cps[cn] = rt_pl_u8_get(q, &adv); cn++; }
            pl_cell_t lst = plc_make_atom_cell("[]");
            for (int i = cn; i > 0; i--) {
                pl_cell_t el;
                if (as_codes) el = pl_make_int((int64_t)cps[i - 1]);
                else { char cs[8]; int bl = rt_pl_u8_put(cs, cps[i - 1]); cs[bl] = '\0'; el = plc_make_atom_cell_n(cs, (size_t)bl); }
                pl_cell_t *pair = (pl_cell_t *)PL_CELL_ALLOC(2 * sizeof(pl_cell_t));
                pair[0] = el;
                pair[1] = lst;
                lst = pl_make_compound(g_runtime.atoms.atom_dot, 2, pair);
            }
            if (!plc_unify_into_cell_cx((pl_cell_t *)a1_cell, lst, cx)) { return 0; }
            return 1;
        }
        if (t1 && (int)t1->v == DT_S) return plc_text_out_n(a0_cell, plc_atom_text(t1), plc_atom_len(t1), fn[0] == 's', cx);
        if (swi || rt_pl_dialect_swi()) {
            int mode = -1;
            size_t cap = 64, oi = 0;
            char *o = (char *)rt_wsb_alloc(cap);
            void *b = (void *)0;
            for (pl_cell_t *c = t1; !b; c = pl_deref(&((pl_cell_t *)c->p)[1])) {
                if (!c || pl_cell_unbound(c)) { b = rt_pl_ball_instantiation(); break; }
                if (plc_is_atomlike(c) && plc_atom_id_of(c) == g_runtime.atoms.atom_nil) break;
                if (!((int)c->v == DT_PLREF && plc_fid_name(c->slen) == g_runtime.atoms.atom_dot && plc_fid_arity(c->slen) == 2 && c->p)) { b = rt_pl_ball_kind2("type_error", "list", *t1); break; }
                pl_cell_t *el = pl_deref(&((pl_cell_t *)c->p)[0]);
                int cp = 0, adv = 0;
                const char *cn = ((int)el->v == DT_PLATOM || (int)el->v == DT_S) ? plc_atom_text(el) : (const char *)0;
                size_t cnl = cn ? plc_atom_len(el) : 0;
                if (pl_cell_unbound(el)) { b = rt_pl_ball_instantiation(); break; }
                if (mode < 0) mode = (int)el->v == DT_I ? 0 : 1;
                if (mode == 0 && (int)el->v != DT_I) { b = rt_pl_ball_kind2("type_error", "character_code", *el); break; }
                if (mode == 1 && (!cn || !cnl || ((cp = rt_pl_u8_get(cn, &adv)), (size_t)adv != cnl))) { b = rt_pl_ball_kind2("type_error", "character", *el); break; }
                if (mode == 0) cp = (int)el->i;
                if (oi + 8 >= cap) { cap *= 2; o = (char *)rt_wsb_realloc(o, cap); }
                oi += (size_t)rt_pl_u8_put(o + oi, cp);
            }
            if (b) { if (cx && !cx->ball) cx->ball = b; return 0; }
            o[oi] = '\0';
            return plc_text_out_n(a0_cell, o, oi, swi, cx);
        }
        pl_cell_t *cur = t1;
        size_t ocap = 256;
        char *out = (char *)rt_wsb_alloc(ocap);
        size_t oi = 0;
        while (cur && (int)cur->v == DT_PLREF && plc_fid_name(cur->slen) == g_runtime.atoms.atom_dot && plc_fid_arity(cur->slen) == 2) {
            pl_cell_t *pr = (pl_cell_t *)cur->p;
            pl_cell_t *el = pl_deref(&pr[0]);
            if (as_codes) {
                if ((int)el->v != DT_I) { return 0; }
                if (oi + 8 >= ocap) { ocap *= 2; out = (char *)rt_wsb_realloc(out, ocap); }
                oi += (size_t)rt_pl_u8_put(out + oi, (int)el->i);
            } else {
                if ((int)el->v != DT_PLATOM && (int)el->v != DT_S) { return 0; }
                const char *cn = plc_atom_text(el);
                if (!cn) {
                    if (oi + 2 > ocap) { ocap *= 2; out = (char *)rt_wsb_realloc(out, ocap); }
                    out[oi++] = '?';
                } else {
                    size_t cl = plc_atom_len(el);
                    while (oi + cl + 1 > ocap) ocap *= 2;
                    out = (char *)rt_wsb_realloc(out, ocap);
                    memcpy(out + oi, cn, cl);
                    oi += cl;
                }
            }
            cur = pl_deref(&pr[1]);
        }
        out[oi] = '\0';
        return plc_text_out_n(a0_cell, out, oi, fn[0] == 's', cx);
    }
    if (!strcmp(fn, "string_length")) {
        size_t sn;
        const char *s = plc_text_nil_empty(t0, buf0, sizeof buf0, &sn);
        if (!s) { return 0; }
        if (!plc_unify_into_cell_cx((pl_cell_t *)a1_cell, pl_make_int((int64_t)pl_u8_count_n(s, sn)), cx)) { return 0; }
        return 1;
    }
    if (!strcmp(fn, "string_upper") || !strcmp(fn, "string_lower")) {
        const char *s = plc_atom_op_text(t0, buf0, sizeof buf0);
        if (!s) { return 0; }
        size_t n = strlen(s);
        char *out = plc_text_carve(n, 1, cx);
        int up = (!strcmp(fn, "string_upper"));
        if (!out) { return 0; }
        for (size_t i = 0; i < n; i++) out[i] = up ? (char)toupper((unsigned char)s[i]) : (char)tolower((unsigned char)s[i]);
        return plc_text_built(a1_cell, out, n, n, 1, cx);
    }
    if (!strcmp(fn, "atom_string") || !strcmp(fn, "string_to_atom")) {
        int a0_str = !strcmp(fn, "string_to_atom");
        size_t sn;
        if (t0 && !pl_cell_unbound(t0)) { const char *s = plc_text_nil_empty(t0, buf0, sizeof buf0, &sn); if (!s) { return 0; } return plc_text_out_n(a1_cell, s, sn, !a0_str, cx); }
        const char *s = plc_atom_op_text_n(t1, buf1, sizeof buf1, &sn);
        if (!s) { return 0; }
        return plc_text_out_n(a0_cell, s, sn, a0_str, cx);
    }
    if (!strcmp(fn, "number_string")) {
        if (t0 && !pl_cell_unbound(t0) && (!t1 || pl_cell_unbound(t1))) { const char *s = plc_atom_op_text(t0, buf0, sizeof buf0); if (!s) { return 0; } return plc_text_out(a1_cell, s, 1, cx); }
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
        int u0 = !t0 || pl_cell_unbound(t0), u1 = !t1 || pl_cell_unbound(t1);
        if (u0 != u1 && t2 && !pl_cell_unbound(t2)) {
            char buf2[512];
            const char *s2 = plc_atom_op_text(t2, buf2, sizeof buf2);
            if (!s2 || (u0 ? !s1 : !s0)) { return 0; }
            size_t l2 = strlen(s2);
            if (!u0) { size_t l0 = strlen(s0); return (l0 <= l2 && !memcmp(s2, s0, l0)) ? plc_text_out(a1_cell, s2 + l0, 1, cx) : 0; }
            {
                size_t l1 = strlen(s1);
                char *pre;
                if (l1 > l2 || memcmp(s2 + l2 - l1, s1, l1)) { return 0; }
                pre = plc_text_carve(l2 - l1, 1, cx);
                if (!pre) { return 0; }
                memcpy(pre, s2, l2 - l1);
                return plc_text_built(a0_cell, pre, l2 - l1, l2 - l1, 1, cx);
            }
        }
        if (!s0 || !s1) { return 0; }
        size_t l0 = strlen(s0), l1 = strlen(s1);
        char *cat = plc_text_carve(l0 + l1, 1, cx);
        if (!cat) { return 0; }
        memcpy(cat, s0, l0);
        memcpy(cat + l0, s1, l1);
        return plc_text_built(a2_cell, cat, l0 + l1, l0 + l1, 1, cx);
    }
    if (!strcmp(fn, "atomic_list_concat") || !strcmp(fn, "concat_atom")) {
        const char *sep = (t2 && t1 && ((int)t1->v == DT_PLATOM || (int)t1->v == DT_S)) ? plc_atom_text(t1) : "";
        void *result_cell = t2 ? a2_cell : a1_cell;
        size_t sl = (sep && sep[0]) ? strlen(sep) : 0, need = 1;
        long ne = 0;
        for (pl_cell_t *lst = t0; lst && (int)lst->v == DT_PLREF && plc_fid_arity(lst->slen) == 2; lst = pl_deref(&((pl_cell_t *)lst->p)[1])) {
            const char *es = plc_atom_op_text(pl_deref(&((pl_cell_t *)lst->p)[0]), buf0, sizeof buf0);
            if (!es) { return 0; }
            need += strlen(es) + (ne++ ? sl : 0);
        }
        char *out = plc_text_carve(need - 1, 0, cx);
        size_t oi = 0;
        if (!out) { return 0; }
        ne = 0;
        for (pl_cell_t *lst = t0; lst && (int)lst->v == DT_PLREF && plc_fid_arity(lst->slen) == 2; lst = pl_deref(&((pl_cell_t *)lst->p)[1])) {
            const char *es = plc_atom_op_text(pl_deref(&((pl_cell_t *)lst->p)[0]), buf0, sizeof buf0);
            size_t el_len = strlen(es);
            if (ne++ && sl) { memcpy(out + oi, sep, sl); oi += sl; }
            memcpy(out + oi, es, el_len);
            oi += el_len;
        }
        if (!plc_unify_into_cell_cx((pl_cell_t *)result_cell, plc_atom_built(out, need - 1, oi), cx)) { return 0; }
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
typedef struct { char *b; size_t n; size_t cap; size_t seg; size_t fpos[256]; int fchr[256]; int nf; pl_tr_ctx_t *cx; long cadj; long segadj; } plc_fb;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_fb_raw(plc_fb *f, const char *s, size_t n) {
    if (!n) return;
    if (f->n + n + 1 > f->cap) { size_t c = f->cap ? f->cap : 512; while (c < f->n + n + 1) c *= 2; char *nb = (char *)ct_grow(f->b, c); if (!nb) return; f->b = nb; f->cap = c; }
    memcpy(f->b + f->n, s, n);
    f->n += n;
    f->b[f->n] = '\0';
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_fb_add(plc_fb *f, const char *s, size_t n) {
    size_t at = f->n;
    plc_fb_raw(f, s, n);
    for (size_t k = at; k < f->n; k++) if (f->b[k] == '\n') { f->seg = k + 1; f->nf = 0; f->cadj = 0; f->segadj = 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static size_t plc_fb_colat(plc_fb *f, size_t at) {
    size_t k = at, col = 0;
    while (k > 0 && f->b[k - 1] != '\n') k--;
    for (; k < at; k++) if (((unsigned char)f->b[k] & 0xC0u) != 0x80u) col++;
    return (size_t)((long)col + f->cadj);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_fb_u8(plc_fb *f, unsigned cp) {
    char u[4];
    if (cp < 0x80u) { u[0] = (char)cp; plc_fb_raw(f, u, 1); return; }
    if (cp < 0x800u) { u[0] = (char)(0xC0u | (cp >> 6)); u[1] = (char)(0x80u | (cp & 0x3Fu)); plc_fb_raw(f, u, 2); return; }
    if (cp < 0x10000u) { u[0] = (char)(0xE0u | (cp >> 12)); u[1] = (char)(0x80u | ((cp >> 6) & 0x3Fu)); u[2] = (char)(0x80u | (cp & 0x3Fu)); plc_fb_raw(f, u, 3); return; }
    u[0] = (char)(0xF0u | (cp >> 18));
    u[1] = (char)(0x80u | ((cp >> 12) & 0x3Fu));
    u[2] = (char)(0x80u | ((cp >> 6) & 0x3Fu));
    u[3] = (char)(0x80u | (cp & 0x3Fu));
    plc_fb_raw(f, u, 4);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_fb_tab(plc_fb *f, int fill) { if (f->nf < 256) { f->fpos[f->nf] = f->n; f->fchr[f->nf] = fill; f->nf++; } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_fb_stop(plc_fb *f, long target) {
    long cur = (long)plc_fb_colat(f, f->n), pad = target - cur;
    if (pad < 0) f->cadj += pad;
    if (pad > 0 && f->nf == 0) {
        for (long i = 0; i < pad; i++) plc_fb_raw(f, " ", 1);
    } else if (pad > 0) {
        size_t base = f->seg, segn = f->n - f->seg;
        int nf = f->nf, fi = 0;
        char seg[segn + 1];
        if (segn) memcpy(seg, f->b + base, segn);
        f->n = base;
        if (f->b) f->b[f->n] = '\0';
        long w[nf > 0 ? nf : 1];
        for (int i = 0; i < nf; i++) w[i] = pad / nf;
        for (long r = 0; r < pad % nf; r++) w[(nf / 2 + r) % nf]++;
        for (size_t k = 0; k <= segn; k++) {
            while (fi < nf && f->fpos[fi] - base == k) { for (long j = 0; j < w[fi]; j++) plc_fb_u8(f, (unsigned)f->fchr[fi]); fi++; }
            if (k < segn) plc_fb_raw(f, seg + k, 1);
        }
    }
    f->seg = f->n;
    f->nf = 0;
    f->segadj = 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void *plc_fb_term(plc_fb *f, pl_cell_t *t, int kind, int quoted, int ignore_ops) {
    extern FILE *fh_memsink_open(char **, size_t *);
    char *bp = (char *)0;
    size_t bn = 0;
    FILE *ms = fh_memsink_open(&bp, &bn);
    plc_vmap m;
    if (!ms) return (void *)0;
    m.n = 0;
    m.vnc = 0;
    m.fp = ms;
    m.portray = (int (*)(pl_cell_t *, plc_vmap *))0;
    m.pthrown = (void *)0;
    m.cx = (pl_tr_ctx_t *)0;
    m.pheld = 0;
    m.pgoal = (pl_cell_t *)0;
    m.popts = (pl_cell_t *)0;
    m.space_args = 0;
    m.next_arg = 0;
    if (kind == 4) {
        extern int rt_proc_is_registered(const char *);
        m.portray = rt_proc_is_registered("portray/1") ? plc_portray_hit : (int (*)(pl_cell_t *, plc_vmap *))0;
        m.cx = f->cx;
        plc_write(t, &m);
    } else if (kind == 0) plc_write(t, &m);
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
static int plc_fmt_eval(pl_cell_t *h, DESCR_t *out, void **ball) {
    extern int rt_pl_ax_eval_val(DESCR_t, DESCR_t *, void **);
    if (!h) return 0;
    if (pl_cell_unbound(h)) { extern void *rt_pl_ball_instantiation(void); if (!*ball) *ball = rt_pl_ball_instantiation(); return 0; }
    return rt_pl_ax_eval_val(*h, out, ball);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_fmt_int(pl_cell_t *h, long *out, void **ball) {
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    DESCR_t v;
    if (!plc_fmt_eval(h, &v, ball)) return 0;
    if (v.v != DT_I) { if (!*ball) *ball = rt_pl_ball_kind2("type_error", "integer", v); return 0; }
    *out = (long)v.i;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_fmt_dbl(pl_cell_t *h, double *out, void **ball) { DESCR_t v; if (!plc_fmt_eval(h, &v, ball)) return 0; *out = (v.v == DT_I) ? (double)v.i : v.r; return 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_fmt_text(pl_cell_t *h, char **out, size_t *len, void **ball) {
    extern void *rt_pl_ball_instantiation(void);
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    size_t cap = 64, n = 0;
    char *b;
    pl_cell_t *lst;
    if (!h) return 0;
    if (pl_cell_unbound(h)) { if (!*ball) *ball = rt_pl_ball_instantiation(); return 0; }
    if (plc_fmt_is_atom(h)) {
        const char *s = plc_atom_text(h);
        if (!strcmp(s, "[]")) s = "";
        *len = strlen(s);
        *out = (char *)ct_alloc(*len + 1);
        if (!*out) return 0;
        memcpy(*out, s, *len + 1);
        return 1;
    }
    b = (char *)ct_alloc(cap);
    if (!b) return 0;
    lst = pl_deref(h);
    while (lst && (int)lst->v == DT_PLREF && pl_arity(lst) == 2) {
        pl_cell_t *aa = (pl_cell_t *)pl_compound_heap(lst);
        pl_cell_t *e = pl_deref(&aa[0]);
        long ch = -1;
        const char *cs = (const char *)0;
        size_t cl = 0;
        if (pl_cell_unbound(e)) { ct_drop(b); if (!*ball) *ball = rt_pl_ball_instantiation(); return 0; }
        if (pl_is_int(e)) ch = (long)pl_int_val(e);
        else if (plc_fmt_is_atom(e)) { cs = plc_atom_text(e); cl = cs ? strlen(cs) : 0; } else { ct_drop(b); if (!*ball) *ball = rt_pl_ball_kind2("type_error", "text", *e); return 0; }
        if (n + cl + 6 > cap) { while (n + cl + 6 > cap) cap *= 2; char *nb = (char *)ct_grow(b, cap); if (!nb) { ct_drop(b); return 0; } b = nb; }
        if (cs) { memcpy(b + n, cs, cl); n += cl; } else if (ch < 0x80) b[n++] = (char)ch;
        else if (ch < 0x800) {
            b[n++] = (char)(0xC0 | (ch >> 6));
            b[n++] = (char)(0x80 | (ch & 0x3F));
        } else if (ch < 0x10000) {
            b[n++] = (char)(0xE0 | (ch >> 12));
            b[n++] = (char)(0x80 | ((ch >> 6) & 0x3F));
            b[n++] = (char)(0x80 | (ch & 0x3F));
        } else {
            b[n++] = (char)(0xF0 | (ch >> 18));
            b[n++] = (char)(0x80 | ((ch >> 12) & 0x3F));
            b[n++] = (char)(0x80 | ((ch >> 6) & 0x3F));
            b[n++] = (char)(0x80 | (ch & 0x3F));
        }
        lst = pl_deref(&aa[1]);
    }
    if (lst && pl_cell_unbound(lst)) { ct_drop(b); if (!*ball) *ball = rt_pl_ball_instantiation(); return 0; }
    if (!lst || !plc_is_nil(lst)) { ct_drop(b); if (!*ball) *ball = rt_pl_ball_kind2("type_error", "text", lst ? *lst : *h); return 0; }
    b[n] = '\0';
    *out = b;
    *len = n;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_fmt_digits(plc_fb *f, int neg, const char *d, long nd, long places, int group) {
    extern const char *rt_pl_flag_text(const char *);
    long tot = nd > places ? nd : places + 1, ip = tot - places, i;
    const char *dp = ".", *ts = ",", *gs = "r3", *x;
    char mark[ip + 1];
    memset(mark, 0, (size_t)ip + 1);
    if (group) {
        if ((x = rt_pl_flag_text("$loc_dp")) && x[0]) dp = x;
        if ((x = rt_pl_flag_text("$loc_ts"))) ts = x;
        if ((x = rt_pl_flag_text("$loc_grp")) && x[0]) gs = x;
        for (long pos = ip; pos > 0 && *gs; ) {
            char *e;
            long g;
            if (*gs == 'r') { g = strtol(gs + 1, &e, 10); while (g > 0 && (pos -= g) > 0) mark[pos] = 1; break; }
            g = strtol(gs, &e, 10);
            if (g <= 0 || e == gs) break;
            if ((pos -= g) > 0) mark[pos] = 1;
            gs = *e == ',' ? e + 1 : e;
        }
    }
    if (neg) plc_fb_add(f, "-", 1);
    for (i = 0; i < tot; i++) {
        char c = i < tot - nd ? '0' : d[i - (tot - nd)];
        if (i == ip && places > 0) plc_fb_add(f, group ? dp : ".", strlen(group ? dp : "."));
        if (group && i && i < ip && mark[i]) plc_fb_add(f, ts, strlen(ts));
        plc_fb_add(f, &c, 1);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_fmt_dec(plc_fb *f, long long iv, long places, int group) {
    char dig[24];
    int nd = 0, neg = iv < 0;
    unsigned long long u = neg ? (unsigned long long)(-(iv + 1)) + 1ull : (unsigned long long)iv;
    if (!u) dig[nd++] = '0';
    while (u) { dig[nd++] = (char)('0' + (int)(u % 10ull)); u /= 10ull; }
    for (int i = 0; i < nd / 2; i++) { char t = dig[i]; dig[i] = dig[nd - 1 - i]; dig[nd - 1 - i] = t; }
    plc_fmt_digits(f, neg, dig, nd, places, group);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_fmt_radix(plc_fb *f, long long iv, int base, int upper) {
    const char *d = upper ? "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ" : "0123456789abcdefghijklmnopqrstuvwxyz";
    char buf[80];
    int bi = 0;
    unsigned long long u = iv < 0 ? (unsigned long long)(-(iv + 1)) + 1ull : (unsigned long long)iv, ub = (unsigned long long)base;
    if (!u) buf[bi++] = '0';
    while (u) { buf[bi++] = d[(int)(u % ub)]; u /= ub; }
    if (iv < 0) plc_fb_add(f, "-", 1);
    while (bi) plc_fb_add(f, &buf[--bi], 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void *plc_fmt_exhausted(pl_cell_t all) {
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    return !rt_pl_dialect_gnu() ? rt_pl_ball_kind2("domain_error", "format_arguments", all) : rt_pl_ball_kind2("domain_error", "non_empty_list", pl_make_atom(g_runtime.atoms.atom_nil));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_fmt_has_stop(const char *p) {
    for (; *p; p++) {
        if (*p != '~') continue;
        p++;
        if (*p == '`' && p[1]) p += 2;
        else if (*p == '*') p++;
        else while (*p >= '0' && *p <= '9') p++;
        if (*p == ':') p++;
        if (*p == '|' || *p == '+' || *p == 'N') return 1;
        if (!*p) return 0;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_format_run(const char *fmt, void *list_cell, pl_tr_ctx_t *cx) {
    extern void *rt_pl_ball_instantiation(void);
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    plc_fb f;
    void *ball = (void *)0;
    pl_cell_t *args = list_cell ? pl_deref((pl_cell_t *)list_cell) : (pl_cell_t *)0;
    pl_cell_t all = args ? *args : pl_make_int(0);
    const char *p;
    FILE *fd;
    plc_wsref ra = { -1, (pl_cell_t *)0 }, rall = { -1, (pl_cell_t *)0 };
    long mark = -1;
    pl_cell_t *args0 = args;
    plc_atoms_ready();
    memset(&f, 0, sizeof f);
    f.cx = cx;
    if (!fmt) return (void *)0;
    size_t fl = strlen(fmt);
    char fmtc[fl + 1];
    memcpy(fmtc, fmt, fl + 1);
    fmt = fmtc;
    if (plc_fmt_has_stop(fmt)) {
        extern int fh_query_position(int, long *, long *, long *);
        extern int fh_current_output(void);
        long c = 0, l = 0, lp = 0;
        if (fh_query_position(fh_current_output(), &c, &l, &lp) == 1) f.cadj = f.segadj = lp;
    }
    for (p = fmt; *p && !ball; p++) {
        long nval = 0;
        int have_n = 0, fill = ' ', colon = 0;
        if (*p != '~') { plc_fb_add(&f, p, 1); continue; }
        p++;
        if (*p == '`' && p[1]) {
            fill = (unsigned char)p[1];
            have_n = 1;
            nval = fill;
            p += 2;
        } else if (*p == '*') {
            pl_cell_t *h = plc_fmt_next_arg(&args);
            p++;
            if (!h) { ball = plc_fmt_exhausted(all); break; }
            if (!plc_fmt_int(h, &nval, &ball)) break;
            have_n = 1;
            fill = (int)nval;
        } else {
            while (*p >= '0' && *p <= '9') { nval = nval * 10 + (*p - '0'); have_n = 1; p++; }
            if (have_n) fill = (int)nval;
        }
        if (*p == ':') { colon = 1; p++; }
        if (!*p) { if (rt_pl_dialect_gnu()) ball = rt_pl_ball_kind2("domain_error", "format_control_sequence", plc_make_atom_cell("")); break; }
        if (*p == '~') { plc_fb_add(&f, "~", 1); continue; }
        if (*p == 'n') { long r = have_n ? nval : 1; for (long i = 0; i < r; i++) plc_fb_add(&f, "\n", 1); continue; }
        if (*p == 'N') { if (plc_fb_colat(&f, f.n)) plc_fb_add(&f, "\n", 1); continue; }
        if (*p == 't') { plc_fb_tab(&f, have_n ? fill : ' '); continue; }
        if (*p == '|') { plc_fb_stop(&f, have_n ? nval : (long)plc_fb_colat(&f, f.n)); continue; }
        if (*p == '+') { plc_fb_stop(&f, (long)plc_fb_colat(&f, f.seg) - f.segadj + (have_n ? nval : 8)); continue; }
        if (rt_pl_dialect_gnu() && !strchr("acdDeEfgGikpqrRswW", *p)) { char cb[2] = { *p, 0 }; ball = rt_pl_ball_kind2("domain_error", "format_control_sequence", plc_make_atom_cell(cb)); break; }
        {
            pl_cell_t *h = plc_fmt_next_arg(&args);
            if (!h) { ball = plc_fmt_exhausted(all); break; }
            if (*p == 'w') plc_fb_term(&f, h, 0, 0, 0);
            else if (*p == 'p') {
                void *tb;
                if (mark < 0 && cx && cx->tr) { mark = plc_ws_mark(cx); plc_wsref_set(cx, &rall, args0); }
                if (mark >= 0) plc_wsref_set(cx, &ra, args);
                tb = plc_fb_term(&f, h, 4, 0, 0);
                if (mark >= 0) { args = plc_wsref_get(cx, &ra); args0 = plc_wsref_get(cx, &rall); if (args0) all = *args0; }
                if (tb) { ball = tb; break; }
            } else if (*p == 'q') plc_fb_term(&f, h, 1, 0, 0);
            else if (*p == 'k') plc_fb_term(&f, h, 2, 0, 0);
            else if (*p == 'i') {
            } else if (*p == 'a') {
                char ab[96];
                const char *as = (const char *)0;
                if (pl_cell_unbound(h)) { ball = rt_pl_ball_instantiation(); break; }
                if (plc_fmt_is_atom(h)) as = plc_atom_text(h);
                else if (rt_pl_dialect_swi() && (int)h->v != DT_PLREF) as = plc_atom_op_text(h, ab, sizeof ab);
                if (!as) { ball = rt_pl_ball_kind2("type_error", "atom", *h); break; }
                plc_fb_add(&f, as, strlen(as));
            } else if (*p == 'c') {
                long cv;
                if (!plc_fmt_int(h, &cv, &ball)) break;
                if (rt_pl_dialect_gnu() && (cv < 0 || cv > 0x10FFFF)) {
                    extern void *rt_pl_ball_kind1(const char *, const char *);
                    ball = rt_pl_ball_kind1("representation_error", "character_code");
                    break;
                }
                { long r = have_n ? nval : 1; char c = (char)cv; for (long i = 0; i < r; i++) plc_fb_add(&f, &c, 1); }
            } else if (*p == 'd' || *p == 'D') {
                long iv;
                if (have_n && nval < 0) { ball = rt_pl_ball_kind2("domain_error", "format_argument", pl_make_int(nval)); break; }
                {
                    DESCR_t bv;
                    if (plc_fmt_eval(h, &bv, &ball) && bv.v == DT_BIG) {
                        extern char *rt_big_str(DESCR_t);
                        char *bs = rt_big_str(bv);
                        int ng = bs && bs[0] == '-';
                        plc_fmt_digits(&f, ng, bs ? bs + ng : "0", bs ? (long)strlen(bs + ng) : 1L, have_n ? nval : 0, *p == 'D' || colon);
                        continue;
                    }
                }
                if (!plc_fmt_int(h, &iv, &ball)) break;
                plc_fmt_dec(&f, (long long)iv, have_n ? nval : 0, *p == 'D' || colon);
            } else if (*p == 'r' || *p == 'R') {
                long iv;
                long base = have_n ? nval : 8;
                if (base < 2 || base > 36) { ball = rt_pl_ball_kind2("domain_error", "radix", pl_make_int(base)); break; }
                if (!plc_fmt_int(h, &iv, &ball)) break;
                plc_fmt_radix(&f, (long long)iv, (int)base, *p == 'R');
            } else if (*p == 'f' || *p == 'e' || *p == 'E' || *p == 'g' || *p == 'G') {
                double dv;
                char spec[8];
                DESCR_t nv;
                int prec = have_n ? (int)nval : 6;
                if (!plc_fmt_eval(h, &nv, &ball)) break;
                if (*p == 'f' && (nv.v == DT_I || nv.v == DT_BIG)) {
                    extern char *rt_big_str(DESCR_t);
                    char ib[24];
                    const char *is = nv.v == DT_BIG ? rt_big_str(nv) : (snprintf(ib, sizeof ib, "%lld", (long long)nv.i), ib);
                    int ng = is && is[0] == '-';
                    size_t il = is ? strlen(is + ng) : 0;
                    char dg[il + (size_t)(prec > 0 ? prec : 0) + 2];
                    memcpy(dg, is ? is + ng : "0", il);
                    for (int k = 0; k < prec; k++) dg[il + (size_t)k] = '0';
                    plc_fmt_digits(&f, ng, dg, (long)il + (prec > 0 ? prec : 0), prec > 0 ? prec : 0, colon);
                    continue;
                }
                if (*p == 'f' && pl_rat_is(nv)) {
                    extern char *rt_big_str(DESCR_t);
                    int ng = 0;
                    long pp = prec > 0 ? prec : 0;
                    DESCR_t qv = pl_rat_scaled(nv, pp, &ng);
                    char ib[24];
                    const char *is = qv.v == DT_BIG ? rt_big_str(qv) : (snprintf(ib, sizeof ib, "%lld", (long long)qv.i), ib);
                    size_t il = strlen(is), z = il <= (size_t)pp ? (size_t)pp + 1 - il : 0;
                    char dg[il + z + 1];
                    memset(dg, '0', z);
                    memcpy(dg + z, is, il);
                    plc_fmt_digits(&f, ng, dg, (long)(il + z), pp, colon);
                    continue;
                }
                if (pl_rat_is(nv)) dv = pl_rat_real(nv);
                else if (nv.v == DT_BIG) { extern char *rt_big_str(DESCR_t); dv = strtod(rt_big_str(nv), (char **)0); } else dv = nv.v == DT_I ? (double)nv.i : nv.r;
                spec[0] = '%';
                spec[1] = '.';
                spec[2] = '*';
                spec[3] = (char)*p;
                spec[4] = '\0';
                {
                    char nb[fmt_len(spec, prec, dv)];
                    snprintf(nb, sizeof nb, spec, prec, dv);
                    if (colon && *p == 'f' && isfinite(dv)) {
                        int ng = nb[0] == '-';
                        size_t nl = strlen(nb), k = 0;
                        char dg[nl + 1];
                        for (size_t i = (size_t)ng; i < nl; i++) if (nb[i] != '.') dg[k++] = nb[i];
                        plc_fmt_digits(&f, ng, dg, (long)k, prec, 1);
                    } else plc_fb_add(&f, nb, strlen(nb));
                }
            } else if (*p == 's') {
                char *tx = (char *)0;
                size_t tn = 0;
                if (!plc_fmt_text(h, &tx, &tn, &ball)) break;
                if (have_n && (size_t)nval < tn) tn = (size_t)(nval < 0 ? 0 : nval);
                plc_fb_add(&f, tx, tn);
                if (have_n && (size_t)nval > tn && !rt_pl_dialect_swi()) for (size_t i = tn; i < (size_t)nval; i++) plc_fb_add(&f, " ", 1);
                ct_drop(tx);
            } else if (*p == 'W') {
                pl_cell_t *o = plc_fmt_next_arg(&args);
                int quoted = 0, iops = 0;
                pl_cell_t *lst;
                if (!o) { ball = plc_fmt_exhausted(all); break; }
                if (pl_cell_unbound(o)) { ball = rt_pl_ball_instantiation(); break; }
                for (lst = pl_deref(o); lst && (int)lst->v == DT_PLREF && pl_arity(lst) == 2; ) {
                    pl_cell_t *aa = (pl_cell_t *)pl_compound_heap(lst);
                    pl_cell_t *e = pl_deref(&aa[0]);
                    if (e && (int)e->v == DT_PLREF && pl_arity(e) == 1) {
                        const char *on = prolog_atom_name(plc_fid_name(e->slen));
                        pl_cell_t *ov = pl_deref(&((pl_cell_t *)pl_compound_heap(e))[0]);
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
    { fd = plc_out(); if (f.n) fwrite(f.b, 1, f.n, fd); }
    ct_drop(f.b);
    plc_ws_end(cx, mark);
    return ball;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_format_cell(const char *fmt, void *list_cell) { (void)rt_pl_format_run(fmt, list_cell, (pl_tr_ctx_t *)0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_ct_print(const char *cs) {
    int adv, cp = rt_pl_u8_get(cs, &adv), ok;
    locale_t l;
    if (cp < 0x80) return cp >= 32 && cp != 127;
    l = newlocale(LC_CTYPE_MASK, "C.UTF-8", (locale_t)0);
    if (!l) return cp >= 0xA0;
    ok = iswprint_l((wint_t)cp, l) != 0;
    freelocale(l);
    return ok;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_char_type_cell(void *char_cell, void *type_cell, void *val_cell, pl_tr_ctx_t *cx) {
    pl_cell_t *tc = char_cell ? pl_deref((pl_cell_t *)char_cell) : (pl_cell_t *)0;
    if (!tc || !type_cell) return 0;
    char b0[256];
    int cn = ((int)tc->v == DT_I && tc->i >= 0 && tc->i <= 0x10FFFF) ? rt_pl_u8_put(b0, (int)tc->i) : 0;
    if ((int)tc->v == DT_I) b0[cn] = 0;
    const char *cs = (int)tc->v == DT_I ? b0 : plc_atom_op_text(tc, b0, sizeof b0);
    if (!cs || !cs[0]) return 0;
    unsigned char ch = (unsigned char)cs[0];
    pl_cell_t *td = pl_deref((pl_cell_t *)type_cell);
    if ((int)td->v == DT_PLREF && pl_arity(td) >= 1) {
        const char *ty = prolog_atom_name(plc_functor(td));
        pl_cell_t *inner = val_cell ? (pl_cell_t *)val_cell : &((pl_cell_t *)pl_compound_heap(td))[0];
        pl_cell_t out;
        if (!ty) { return 0; }
        if (!strcmp(ty, "digit")) {
            if (!isdigit(ch)) { return 0; }
            out = pl_make_int((int64_t)(ch - '0'));
        } else if (!strcmp(ty, "xdigit")) {
            if (!isxdigit(ch)) { return 0; }
            out = pl_make_int((int64_t)(isdigit(ch) ? ch - '0' : tolower(ch) - 'a' + 10));
        } else if (!strcmp(ty, "to_lower")) {
            char c2[2] = { (char)toupper(ch), 0 };
            out = plc_make_atom_cell(c2);
        } else if (!strcmp(ty, "to_upper")) {
            char c2[2] = { (char)tolower(ch), 0 };
            out = plc_make_atom_cell(c2);
        } else if (!strcmp(ty, "upper")) {
            if (!isupper(ch)) { return 0; }
            char c2[2] = { (char)tolower(ch), 0 };
            out = plc_make_atom_cell(c2);
        } else if (!strcmp(ty, "lower")) {
            if (!islower(ch)) { return 0; }
            char c2[2] = { (char)toupper(ch), 0 };
            out = plc_make_atom_cell(c2);
        } else if (!strcmp(ty, "code")) {
            out = pl_make_int((int64_t)ch);
        } else {
            return 0;
        }
        if (ty[0] == 't' && !pl_cell_unbound(pl_deref(inner))) {
            char b1[8];
            const char *vs = plc_atom_op_text(pl_deref(inner), b1, sizeof b1);
            if (vs && (unsigned char)vs[0] == ch && !vs[1]) return 1;
        }
        if (!plc_unify_into_cell_cx(inner, out, cx)) { return 0; }
        return 1;
    }
    if ((int)td->v != DT_PLATOM && (int)td->v != DT_S) { return 0; }
    const char *ty = ((int)td->v == DT_S) ? (td->s ? td->s : "") : prolog_atom_name(pl_atom_id(td));
    if (!ty) { return 0; }
    int ok = 0;
    if (!strcmp(ty, "alpha")) ok = isalpha(ch);
    else if (!strcmp(ty, "alnum")) ok = isalnum(ch);
    else if (!strcmp(ty, "digit")) ok = isdigit(ch);
    else if (!strcmp(ty, "space") || !strcmp(ty, "white")) ok = isspace(ch);
    else if (!strcmp(ty, "upper")) ok = isupper(ch);
    else if (!strcmp(ty, "lower")) ok = islower(ch);
    else if (!strcmp(ty, "punct")) ok = ispunct(ch);
    else if (!strcmp(ty, "graph")) ok = isgraph(ch);
    else if (!strcmp(ty, "print")) ok = pl_ct_print(cs);
    else if (!strcmp(ty, "csym")) ok = (isalnum(ch) || ch == '_');
    else if (!strcmp(ty, "csymf")) ok = (isalpha(ch) || ch == '_');
    else if (!strcmp(ty, "end_of_line")) ok = (ch == '\n' || ch == '\r');
    else if (!strcmp(ty, "newline")) ok = (ch == '\n');
    else if (!strcmp(ty, "ascii")) ok = (ch < 128);
    else { return 0; }
    if (!ok) { return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_lower_upper_cell(void *lower_cell, void *upper_cell, pl_tr_ctx_t *cx) {
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
    if (t == DT_I || t == DT_R || t == DT_BIG || pl_rat_is(*d)) return 1;
    if (t == DT_S) return 2;
    if (t == DT_PLATOM) return 3;
    if (t == DT_PLREF) return 4;
    if (rt_pl_eng_inst_is(*d)) return 3;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *rt_pl_cell_name(pl_cell_t *d) { if ((int)d->v == DT_S) return d->s ? d->s : ""; const char *n = prolog_atom_name((int)d->i); return n ? n : ""; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static double plc_big_real(pl_cell_t *d) { extern char *rt_big_str(DESCR_t); char *t = rt_big_str(*d); return t ? strtod(t, (char **)0) : 0.0; }
static int rt_pl_cell_compare_leaf(pl_cell_t *a, pl_cell_t *b, void *ctx) {
    if ((int)a->v == DT_PLREF && (int)b->v == DT_PLREF && a->slen == b->slen) return PLR_DESCEND;
    if ((int)a->v == DT_I && (int)b->v == DT_I) return a->i < b->i ? -1 : (a->i > b->i ? 1 : 0);
    int cla = rt_pl_cell_class(a), clb = rt_pl_cell_class(b);
    if (cla != clb) return cla < clb ? -1 : 1;
    if (cla == 0) return a == b ? 0 : ((uintptr_t)a < (uintptr_t)b ? -1 : 1);
    if (cla == 1 && (pl_rat_is(*a) || pl_rat_is(*b))) {
        if ((int)a->v == DT_R || (int)b->v == DT_R) {
            double x = (int)a->v == DT_R ? a->r : pl_rat_real(*a), y = (int)b->v == DT_R ? b->r : pl_rat_real(*b);
            if (x != y) return x < y ? -1 : 1;
            return (int)a->v == DT_R ? -1 : 1;
        }
        { int c = pl_rat_cmp(*a, *b); return c < 0 ? -1 : (c > 0 ? 1 : 0); }
    }
    if (cla == 1 && ((int)a->v == DT_R) != ((int)b->v == DT_R) && !rt_pl_dialect_swi()) return ((int)a->v == DT_R) ? -1 : 1;
    if (cla == 1 && ((int)a->v == DT_BIG || (int)b->v == DT_BIG) && (int)a->v != DT_R && (int)b->v != DT_R) {
        extern int rt_big_cmp(DESCR_t, DESCR_t);
        int rc = rt_big_cmp(*a, *b);
        return rc < 0 ? -1 : (rc > 0 ? 1 : 0);
    }
    if (cla == 1) {
        double x = ((int)a->v == DT_I) ? (double)a->i : (int)a->v == DT_BIG ? plc_big_real(a) : a->r, y = ((int)b->v == DT_I) ? (double)b->i : (int)b->v == DT_BIG ? plc_big_real(b) : b->r;
        if (isnan(x) || isnan(y)) return isnan(x) && isnan(y) ? 0 : isnan(x) ? -1 : 1;
        if (x < y) return -1;
        if (x > y) return 1;
        if ((int)a->v == DT_R && (int)b->v == DT_R && signbit(x) != signbit(y)) return signbit(x) ? -1 : 1;
        if ((int)a->v == (int)b->v) return 0;
        return ((int)a->v == DT_R) ? -1 : 1;
    }
    if (cla == 2) {
        uint32_t n = a->slen < b->slen ? a->slen : b->slen;
        int c = n ? memcmp(a->s, b->s, n) : 0;
        if (!c) c = a->slen < b->slen ? -1 : a->slen > b->slen ? 1 : 0;
        return c < 0 ? -1 : (c > 0 ? 1 : 0);
    }
    if (cla == 3 && ((int)a->v == DT_DATA || (int)b->v == DT_DATA)) {
        if ((int)a->v == DT_DATA && (int)b->v == DT_DATA) { int64_t x = rt_pl_eng_inst_id(*a), y = rt_pl_eng_inst_id(*b); return x < y ? -1 : x > y ? 1 : 0; }
        return (int)a->v == DT_DATA ? 1 : -1;
    }
    if (cla == 3) {
        if (a->i == b->i) return 0;
        size_t la = plc_atom_len(a), lb = plc_atom_len(b);
        int c = memcmp(rt_pl_cell_name(a), rt_pl_cell_name(b), la < lb ? la : lb);
        if (!c) c = la < lb ? -1 : la > lb ? 1 : 0;
        return c < 0 ? -1 : (c > 0 ? 1 : 0);
    }
    int ara = plc_fid_arity(a->slen), arb = plc_fid_arity(b->slen);
    if (ara != arb) return ara < arb ? -1 : 1;
    if ((plc_fid_name(a->slen)) != (plc_fid_name(b->slen))) {
        const char *na = prolog_atom_name(plc_fid_name(a->slen)), *nb = prolog_atom_name(plc_fid_name(b->slen));
        size_t la = na ? prolog_atom_len(plc_fid_name(a->slen)) : 0, lb = nb ? prolog_atom_len(plc_fid_name(b->slen)) : 0;
        int c = memcmp(na ? na : "", nb ? nb : "", la < lb ? la : lb);
        if (!c) c = la < lb ? -1 : la > lb ? 1 : 0;
        if (c) return c < 0 ? -1 : 1;
    }
    (void)ctx;
    return PLR_DESCEND;
}
PLR_WALK2(plc_compare_walk, rt_pl_cell_compare_leaf, pl_deref)
#define PLC_CMP_DEPTH 1024
#define PLC_CMP_MAP 16384
static const void *plc_cmp_find(const void **mk, const void **mv, const void *p) {
    for (int g = 0; g < PLC_CMP_MAP; g++) {
        size_t h = ((uintptr_t)p >> 4) * 0x9E3779B97F4A7C15ull >> 50;
        while (mk[h] && mk[h] != p) h = (h + 1) & (PLC_CMP_MAP - 1);
        if (!mk[h]) return p;
        p = mv[h];
    }
    return p;
}
static int plc_cmp_bounded(pl_cell_t *ca, pl_cell_t *cb, int use_map, int *ok) {
    pl_cell_t *fa[PLC_CMP_DEPTH], *fb[PLC_CMP_DEPTH];
    int fi[PLC_CMP_DEPTH], fn[PLC_CMP_DEPTH], top = 0, used = 0;
    long steps = 0;
    const void **mk = (const void **)0, **mv = (const void **)0;
    if (use_map) { mk = (const void **)alloca(sizeof(void *) * PLC_CMP_MAP); mv = (const void **)alloca(sizeof(void *) * PLC_CMP_MAP); memset(mk, 0, sizeof(void *) * PLC_CMP_MAP); }
    *ok = 1;
    for (;;) {
        pl_cell_t *A = pl_deref(ca), *B = pl_deref(cb);
        if (A != B) {
            int r = rt_pl_cell_compare_leaf(A, B, (void *)0);
            if (r == PLR_DESCEND) {
                int ar = plc_fid_arity(A->slen);
                int same = 0;
                if (use_map) {
                    const void *ra = plc_cmp_find(mk, mv, A->p), *rb = plc_cmp_find(mk, mv, B->p);
                    if (ra == rb) same = 1;
                    else {
                        if (used * 4 >= PLC_CMP_MAP * 3) { *ok = 0; return 0; }
                        size_t h = ((uintptr_t)ra >> 4) * 0x9E3779B97F4A7C15ull >> 50;
                        while (mk[h]) h = (h + 1) & (PLC_CMP_MAP - 1);
                        mk[h] = ra;
                        mv[h] = rb;
                        used++;
                    }
                } else if (A->p == B->p) same = 1;
                if (!same && ar > 0) {
                    while (top > 0 && fi[top - 1] >= fn[top - 1]) top--;
                    if (top >= PLC_CMP_DEPTH || (!use_map && ++steps > (1L << 16))) { *ok = 0; return 0; }
                    fa[top] = (pl_cell_t *)A->p;
                    fb[top] = (pl_cell_t *)B->p;
                    fi[top] = 0;
                    fn[top] = ar;
                    top++;
                }
            } else if (r) return r;
        }
        while (top > 0 && fi[top - 1] >= fn[top - 1]) top--;
        if (top == 0) return 0;
        ca = &fa[top - 1][fi[top - 1]];
        cb = &fb[top - 1][fi[top - 1]];
        fi[top - 1]++;
    }
}
static int rt_pl_cell_compare(pl_cell_t *ca, pl_cell_t *cb) {
    int ok, r = plc_cmp_bounded(ca, cb, 0, &ok);
    if (ok) return r;
    r = plc_cmp_bounded(ca, cb, 1, &ok);
    return ok ? r : plc_compare_walk(ca, cb, (void *)0, 0);
}
__attribute__((noinline)) int rt_pl_bind_vv(pl_cell_t *A, pl_cell_t *B) {
    char probe;
    int as = (char *)A > &probe, bs = (char *)B > &probe;
    pl_cell_t *from = as && bs ? (A < B ? A : B) : as ? A : bs ? B : (A > B ? A : B);
    pl_cell_t *to = from == A ? B : A;
    pl_cell_t r;
    r.v = (DTYPE_t)DT_PLVAR;
    r.slen = 0;
    r.p = (void *)to;
    if ((int)to->v != DT_PLVAR || to->p != (void *)to) *to = r;
    *from = r;
    return 1;
}
static int plc_unify_leaf(pl_cell_t *A, pl_cell_t *B, void *ctx) {
    int av = pl_cell_unbound(A), bv = pl_cell_unbound(B);
    (void)ctx;
    if (av && bv && pl_attv_is(A) != pl_attv_is(B)) { pl_cell_t r; r.v = (DTYPE_t)DT_PLVAR; r.slen = 0; r.p = (void *)(pl_attv_is(A) ? A : B); pl_bind(pl_attv_is(A) ? B : A, r); return 1; }
    if (av && bv) return rt_pl_bind_vv(A, B);
    if (av) { pl_bind(A, *B); return 1; }
    if (bv) { pl_bind(B, *A); return 1; }
    if ((int)A->v == DT_PLATOM && (int)B->v == DT_PLATOM) return A->i == B->i;
    if (((int)A->v == DT_S || (int)A->v == DT_PLATOM) && ((int)B->v == DT_S || (int)B->v == DT_PLATOM)) return rt_pl_text_eq(A, B);
    if (A->v != B->v) return 0;
    if ((int)A->v == DT_I) return A->i == B->i;
    if ((int)A->v == DT_R) return (isnan(A->r) && isnan(B->r)) || !memcmp(&A->r, &B->r, sizeof A->r);
    if ((int)A->v == DT_PLREF) return A->slen == B->slen ? PLR_DESCEND : 0;
    return 0;
}
PLR_WALK2(plc_unify_walk, plc_unify_leaf, pl_deref) int rt_pl_unify_cyc_plain(pl_cell_t *a, pl_cell_t *b) { return plc_unify_walk(a, b, (void *)0, 1); }
int rt_pl_atop_cell(int op, void *a_cell, void *b_cell) {
    int c = rt_pl_cell_compare((pl_cell_t *)a_cell, (pl_cell_t *)b_cell);
    if (op == 0) return c < 0;
    if (op == 1) return c <= 0;
    if (op == 2) return c > 0;
    if (op == 3) return c >= 0;
    if (op == 4) return c == 0;
    return c != 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_compare_cell(void *order_cell, void *a_cell, void *b_cell, pl_tr_ctx_t *cx) {
    pl_cell_t *o = pl_deref((pl_cell_t *)order_cell);
    if (!pl_cell_unbound(o)) {
        extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
        const char *on = plc_is_atomlike(o) ? rt_pl_cell_name(o) : (const char *)0;
        if (!on) { if (cx && !cx->ball) cx->ball = rt_pl_ball_kind2("type_error", "atom", *o); return 0; }
        if (strcmp(on, "<") && strcmp(on, "=") && strcmp(on, ">")) { if (cx && !cx->ball) cx->ball = rt_pl_ball_kind2("domain_error", "order", *o); return 0; }
    }
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
    m->a[m->n] = x;
    m->b[m->n] = y;
    m->n++;
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
        { pl_cell_t *aa = (pl_cell_t *)a->p, *bb = (pl_cell_t *)b->p; for (int i = 0; i < ara; i++) if (!pl_variant_rec(&aa[i], &bb[i], m)) return 0; return 1; }
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
        int cap = plc_var_occ(a) + 1;
        pl_cell_t *ma[cap], *mb[cap];
        pl_vmap_t m;
        m.a = ma;
        m.b = mb;
        m.n = 0;
        m.cap = cap;
        return pl_variant_rec(a, b, &m);
    }
    {
        pl_cell_t ca = plc_copy(a), cb = plc_copy(b);
        int na = plc_var_occ(&ca) + 1, nb = plc_var_occ(&cb) + 1, ka = 0, kb = 0;
        pl_cell_t *va[na], *vb[nb];
        rt_pl_tv_walk(&ca, va, &ka);
        rt_pl_tv_walk(&cb, vb, &kb);
        if (ka != kb) return 0;
        for (int i = 0; i < ka; i++) { va[i]->v = (DTYPE_t)DT_PLVAR; va[i]->slen = 0; va[i]->p = (void *)vb[i]; }
        return rt_pl_cell_compare(&ca, &cb) == 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_var_occ_visit(pl_cell_t *d, void *ctx) { if (pl_cell_unbound(d)) ++*(int *)ctx; return PLR_V_GO; }
PLR_WALK1(plc_var_occ_walk, plc_var_occ_visit, pl_deref) static int plc_var_occ(pl_cell_t *c) { int k = 0; if (c) (void)plc_var_occ_walk(c, (void *)&k); return k; }
int rt_pl_var_occ(void *cell) { return plc_var_occ((pl_cell_t *)cell); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long plc_list_cells(void *list_cell, int dot_id) {
    pl_cell_t *cur = list_cell ? pl_deref((pl_cell_t *)list_cell) : (pl_cell_t *)0;
    void *ck = (void *)0;
    long n = 0, st = 0, pw = 1;
    while (cur && (int)cur->v == DT_PLREF && plc_fid_name(cur->slen) == dot_id && plc_fid_arity(cur->slen) == 2 && cur->p) {
        if (cur->p == ck) return -1;
        if (++st == pw) { ck = cur->p; pw <<= 1; st = 0; }
        n++;
        cur = pl_deref(&((pl_cell_t *)cur->p)[1]);
    }
    return n;
}
static int plc_list_len(void *list_cell, int dot_id) { long n = plc_list_cells(list_cell, dot_id); return n > 0 ? (int)n : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { pl_cell_t **pool; int *n; int *counts; } plc_tv_t;
static int plc_tv_visit(pl_cell_t *d, void *ctx) {
    plc_tv_t *t = (plc_tv_t *)ctx;
    if (!pl_cell_unbound(d)) return PLR_V_GO;
    for (int i = 0; i < *t->n; i++) if (t->pool[i] == d) { if (t->counts) t->counts[i]++; return PLR_V_GO; }
    if (t->counts) t->counts[*t->n] = 1;
    t->pool[(*t->n)++] = d;
    return PLR_V_GO;
}
PLR_WALK1(plc_tv_walk, plc_tv_visit, pl_deref) static void rt_pl_tv_walk(pl_cell_t *c, pl_cell_t **pool, int *pool_n) {
    plc_tv_t t;
    t.pool = pool;
    t.n = pool_n;
    t.counts = (int *)0;
    if (c) (void)plc_tv_walk(c, (void *)&t);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { pl_cell_t *head; int other; } plc_tv1_t;
typedef struct { pl_cell_t *head; long st; } plc_tv1a_t;
plc_tv1a_t rt_pl_tv1_walk_a(pl_cell_t *c, const void *functors, long nfunctors);
const void *rt_pl_functor_entries(void);
int prolog_functor_count(void);
_Static_assert(PLR_LINK == (DTYPE_t)(DT_PLREF + 2) && PLR_SEEN == (DTYPE_t)(DT_PLREF + 3) && PLR_VCOPY == (DTYPE_t)(DT_PLREF + 5) && __builtin_offsetof(DESCR_t, v) == 0 &&
    __builtin_offsetof(DESCR_t, slen) == 4 && __builtin_offsetof(DESCR_t, p) == 8 && __builtin_offsetof(VCELL_t, cellp) == 0,
    "rt_pl_tv1_walk_a (rtx_plunify.s) is PLR_WALK1 over plc_tv1_visit in asm: it reads the tag byte at 0, the functor id at 4 and the pointer at 8, follows a DT_N name to VCELL_t.cellp at 0, marks P"
    "LR_SEEN and PLR_VCOPY at DT_PLREF + 3 and + 5, resolves PLR_LINK at + 2, keeps its agenda (128 entries) in its frame and answers 2 when it fills, so the C walk takes the term");
static int plc_tv1_visit(pl_cell_t *d, void *ctx) {
    plc_tv1_t *t = (plc_tv1_t *)ctx;
    if (d->v == PLR_VCOPY) return PLR_V_GO;
    if ((int)d->v == DT_PLVAR) { d->v = PLR_VCOPY; d->p = (void *)t->head; t->head = d; return PLR_V_GO; }
    if (d->v == DT_SNUL || d->v == DT_FAIL) { t->other = 1; return PLR_V_STOP; }
    return PLR_V_GO;
}
PLR_WALK1(plc_tv1_walk, plc_tv1_visit, pl_deref)
    _Static_assert(PLR_VCOPY != (DTYPE_t)DT_PLVAR,
    "TERM_VARIABLES IN ONE WALK (hq_runtime 2026-10-17, CEO-1682; hq_prolog's go): a variable is marked PLR_VCOPY the first time the walk meets it and its pointer word threads the variables found so"
    " far"
    ", so the walk needs no pool, no counting walk to size one and no linear search to de-duplicate (det.pl spent 1.8 G instructions in those); copy_term's PLR_VCOPY marks never meet these, since ea"
    "ch runtime " "call restores its marks before it returns, and an unbound DT_SNUL or DT_FAIL cell, which carries no self pointer to restore, sends the call back to the pool road");
int rt_pl_term_variables_cell(void *term_cell, void *vars_cell, void *tail_cell, pl_tr_ctx_t *cx) {
    int dot_id = prolog_atom_intern(".");
    pl_cell_t nil = pl_make_atom(g_runtime.atoms.atom_nil);
    pl_cell_t result = tail_cell ? *(pl_cell_t *)tail_cell : nil;
    plc_tv1_t t;
    t.head = (pl_cell_t *)0;
    t.other = 0;
    if (term_cell) {
        plc_tv1a_t a = rt_pl_tv1_walk_a((pl_cell_t *)term_cell, rt_pl_functor_entries(), (long)prolog_functor_count());
        t.head = a.head;
        t.other = a.st == 1;
        if (a.st == 2) {
            for (pl_cell_t *v = t.head, *nx; v; v = nx) { nx = (pl_cell_t *)v->p; v->v = (DTYPE_t)DT_PLVAR; v->p = (void *)v; }
            t.head = (pl_cell_t *)0;
            (void)plc_tv1_walk((pl_cell_t *)term_cell, (void *)&t);
        }
    }
    if (t.other) {
        for (pl_cell_t *v = t.head, *nx; v; v = nx) { nx = (pl_cell_t *)v->p; v->v = (DTYPE_t)DT_PLVAR; v->p = (void *)v; }
        pl_cell_t *pool[plc_var_occ((pl_cell_t *)term_cell) + 1];
        int pn = 0;
        rt_pl_tv_walk((pl_cell_t *)term_cell, pool, &pn);
        for (int i = pn - 1; i >= 0; i--) {
            pl_cell_t *blk = (pl_cell_t *)PL_CELL_ALLOC(2 * sizeof(pl_cell_t));
            blk[0] = pl_make_ref(pool[i], (int)pool[i]->slen);
            blk[1] = result;
            result = pl_make_compound(dot_id, 2, blk);
        }
    } else for (pl_cell_t *v = t.head, *nx; v; v = nx) {
        pl_cell_t *blk = (pl_cell_t *)PL_CELL_ALLOC(2 * sizeof(pl_cell_t));
        nx = (pl_cell_t *)v->p;
        v->v = (DTYPE_t)DT_PLVAR;
        v->p = (void *)v;
        blk[0] = pl_make_ref(v, (int)v->slen);
        blk[1] = result;
        result = pl_make_compound(dot_id, 2, blk);
    }
    if (!plc_unify_cells_cx((pl_cell_t *)vars_cell, &result, cx)) { return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_subsumes_cell(void *gen_cell, void *spec_cell) {
    pl_cell_t *g = (pl_cell_t *)gen_cell, *s = (pl_cell_t *)spec_cell;
    if (!g || !s) return 0;
    pl_cell_t *svars[plc_var_occ(s) + 1];
    int sn = 0;
    rt_pl_tv_walk(s, svars, &sn);
    int unified = pl_unify(g, s);
    int bound_spec = 0;
    if (unified) for (int i = 0; i < sn; i++) if (!pl_cell_unbound(pl_deref(svars[i]))) { bound_spec = 1; break; }
    return unified && !bound_spec;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_sort_cell(int do_msort, void *list_cell, void *result_cell, pl_tr_ctx_t *cx) {
    int dot_id = prolog_atom_intern(".");
    long nl = plc_list_cells(list_cell, dot_id);
    if (nl < 0) return 0;
    pl_cell_t *elems[nl + 1];
    int n = 0;
    pl_cell_t *cur = pl_deref((pl_cell_t *)list_cell);
    while (n < nl && cur && (int)cur->v == DT_PLREF && plc_fid_name(cur->slen) == dot_id && plc_fid_arity(cur->slen) == 2) {
        pl_cell_t *aa = (pl_cell_t *)cur->p;
        elems[n++] = &aa[0];
        cur = pl_deref(&aa[1]);
    }
    for (int i = 1; i < n; i++) { pl_cell_t *key = elems[i]; int j = i - 1; while (j >= 0 && rt_pl_cell_compare(elems[j], key) > 0) { elems[j + 1] = elems[j]; j--; } elems[j + 1] = key; }
    int m = 0;
    int out_idx[n + 1];
    for (int i = 0; i < n; i++) { if (!do_msort && m > 0 && rt_pl_cell_compare(elems[out_idx[m - 1]], elems[i]) == 0) continue; out_idx[m++] = i; }
    pl_cell_t nil = pl_make_atom(g_runtime.atoms.atom_nil);
    pl_cell_t result = nil;
    for (int i = m - 1; i >= 0; i--) {
        pl_cell_t *blk = (pl_cell_t *)PL_CELL_ALLOC(2 * sizeof(pl_cell_t));
        blk[0] = pl_make_ref(elems[out_idx[i]], (int)elems[out_idx[i]]->slen);
        blk[1] = result;
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
static pl_cell_t plc_nil_cell(void) { return pl_make_atom(g_runtime.atoms.atom_nil); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static pl_cell_t plc_cons(int fid, pl_cell_t head, pl_cell_t tail) {
    pl_cell_t *blk = (pl_cell_t *)PL_CELL_ALLOC(2 * sizeof(pl_cell_t));
    blk[0] = head;
    blk[1] = tail;
    return pl_make_compound(fid, 2, blk);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_bag_prep_cell(int is_setof, void *list_cell, void *result_cell) {
    int dot_id = prolog_atom_intern(".");
    int dash_id = prolog_atom_intern("-");
    pl_cell_t *elems[plc_list_len(list_cell, dot_id) + 1];
    int n = plc_pairs_extract(list_cell, elems, dot_id, dash_id);
    if (n < 0) return 0;
    for (int i = 1; i < n; i++) {
        pl_cell_t *key = elems[i];
        int j = i - 1;
        if (is_setof) {
            while (j >= 0 && rt_pl_cell_compare(elems[j], key) > 0) { elems[j + 1] = elems[j]; j--; }
        } else {
            while (j >= 0 && rt_pl_cell_compare((pl_cell_t *)elems[j]->p, (pl_cell_t *)key->p) > 0) { elems[j + 1] = elems[j]; j--; }
        }
        elems[j + 1] = key;
    }
    int m = 0;
    int out_idx[n + 1];
    for (int i = 0; i < n; i++) { if (is_setof && m > 0 && rt_pl_cell_compare(elems[out_idx[m - 1]], elems[i]) == 0) continue; out_idx[m++] = i; }
    pl_cell_t nil = pl_make_atom(g_runtime.atoms.atom_nil);
    pl_cell_t result = nil;
    for (int i = m - 1; i >= 0; i--) {
        pl_cell_t *blk = (pl_cell_t *)PL_CELL_ALLOC(2 * sizeof(pl_cell_t));
        blk[0] = pl_make_ref(elems[out_idx[i]], (int)elems[out_idx[i]]->slen);
        blk[1] = result;
        result = pl_make_compound(dot_id, 2, blk);
    }
    if (!pl_unify((pl_cell_t *)result_cell, &result)) { return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_keysort_cell(void *list_cell, void *result_cell, pl_tr_ctx_t *cx) {
    int dot_id = prolog_atom_intern(".");
    int dash_id = prolog_atom_intern("-");
    pl_cell_t *elems[plc_list_len(list_cell, dot_id) + 1];
    int n = plc_pairs_extract(list_cell, elems, dot_id, dash_id);
    if (n < 0) return 0;
    for (int i = 1; i < n; i++) {
        pl_cell_t *key = elems[i];
        pl_cell_t *kkey = (pl_cell_t *)key->p;
        int j = i - 1;
        while (j >= 0 && rt_pl_cell_compare((pl_cell_t *)elems[j]->p, kkey) > 0) { elems[j + 1] = elems[j]; j--; }
        elems[j + 1] = key;
    }
    pl_cell_t nil = pl_make_atom(g_runtime.atoms.atom_nil);
    pl_cell_t result = nil;
    for (int i = n - 1; i >= 0; i--) {
        pl_cell_t *blk = (pl_cell_t *)PL_CELL_ALLOC(2 * sizeof(pl_cell_t));
        blk[0] = pl_make_ref(elems[i], (int)elems[i]->slen);
        blk[1] = result;
        result = pl_make_compound(dot_id, 2, blk);
    }
    if (!plc_unify_cells_cx((pl_cell_t *)result_cell, &result, cx)) { return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_inplace_cmp(int mode, pl_cell_t *vals, int a, int b) { return mode == 2 ? rt_pl_cell_compare((pl_cell_t *)vals[a].p, (pl_cell_t *)vals[b].p) : rt_pl_cell_compare(&vals[a], &vals[b]); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_sort_in_place_cell(int mode, void *list_cell) {
    int dot_id = prolog_atom_intern(".");
    int m = 0;
    long nl = plc_list_cells(list_cell, dot_id);
    int n = nl > 0 ? (int)nl : 0;
    pl_cell_t *cur;
    if (nl < 0) return 0;
    if (n == 0) return 1;
    pl_cell_t *kids[n];
    pl_cell_t vals[n];
    int ordb[n], tmpb[n];
    int *ord = ordb, *tmp = tmpb;
    cur = pl_deref((pl_cell_t *)list_cell);
    for (int i = 0; i < n; i++) {
        pl_cell_t *d;
        kids[i] = (pl_cell_t *)cur->p;
        d = pl_deref(&kids[i][0]);
        vals[i] = pl_cell_unbound(d) ? pl_make_ref(d, (int)d->slen) : *d;
        ord[i] = i;
        cur = pl_deref(&kids[i][1]);
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
    for (int i = 0; i < n; i++) { if (mode == 0 && m > 0 && rt_pl_cell_compare(&vals[ord[m - 1]], &vals[ord[i]]) == 0) continue; ord[m++] = ord[i]; }
    for (int i = 0; i < m; i++) kids[i][0] = vals[ord[i]];
    if (m < n) kids[m - 1][1] = plc_nil_cell();
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_group_pairs_by_key_cell(void *list_cell, void *result_cell) {
    int dot_id = prolog_atom_intern(".");
    int dash_id = prolog_atom_intern("-");
    pl_cell_t *elems[plc_list_len(list_cell, dot_id) + 1];
    int n = plc_pairs_extract(list_cell, elems, dot_id, dash_id);
    if (n < 0) return 0;
    pl_cell_t groups[n + 1];
    int ng = 0;
    pl_cell_t nil = plc_nil_cell();
    int i = 0;
    while (i < n) {
        pl_cell_t *key = pl_deref(&((pl_cell_t *)elems[i]->p)[0]);
        pl_cell_t *vlist[n - i];
        int nv = 0;
        int j = i;
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
int rt_pl_pairs_keys_values_cell(void *pairs_cell, void *keys_cell, void *values_cell) {
    int dot_id = prolog_atom_intern(".");
    int dash_id = prolog_atom_intern("-");
    pl_cell_t *pd = pairs_cell ? pl_deref((pl_cell_t *)pairs_cell) : (pl_cell_t *)0;
    if (pd && !pl_cell_unbound(pd)) {
        pl_cell_t *elems[plc_list_len(pairs_cell, dot_id) + 1];
        int n = plc_pairs_extract(pairs_cell, elems, dot_id, dash_id);
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
    pl_cell_t *kelems[plc_list_len(keys_cell, dot_id) + 1];
    int nk = plc_plain_list_extract(keys_cell, kelems, dot_id);
    pl_cell_t *velems[plc_list_len(values_cell, dot_id) + 1];
    int nv = plc_plain_list_extract(values_cell, velems, dot_id);
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
static int plc_nv_visit(pl_cell_t *d, void *ctx) {
    plc_nv_t *k = (plc_nv_t *)ctx;
    if (pl_cell_unbound(d)) { pl_cell_t *a = (pl_cell_t *)rt_ws_alloc_descr(1); *a = pl_make_int(k->counter++); plc_bind_cx(d, pl_make_compound(k->var_id, 1, a), k->cx); }
    return PLR_V_GO;
}
PLR_WALK1(plc_nv_walk, plc_nv_visit, pl_deref) static long pl_numbervars_walk(pl_cell_t *c, long counter, int var_id, pl_tr_ctx_t *cx) {
    plc_nv_t k;
    k.counter = counter;
    k.var_id = var_id;
    k.cx = cx;
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
int rt_pl_numbervars1_cell(void *term_cell, pl_tr_ctx_t *cx) { int var_id = prolog_atom_intern("$VAR"); pl_numbervars_walk((pl_cell_t *)term_cell, 0, var_id, cx); return 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_get_print_stream_cell(void *stream_cell) {
    extern int fh_current_output(void);
    int stream_id = prolog_atom_intern("$stream");
    pl_cell_t *arg = (pl_cell_t *)rt_ws_alloc_descr(1);
    *arg = pl_make_int(fh_current_output());
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
static pl_cell_t pl_nil_cell(void) { return pl_make_atom(g_runtime.atoms.atom_nil); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_name_singleton_vars_cell(void *term_cell) {
    int occ = plc_var_occ((pl_cell_t *)term_cell) + 1;
    pl_cell_t *pool[occ];
    int counts[occ];
    int pn = 0;
    pl_distinct_vars_walk((pl_cell_t *)term_cell, pool, counts, &pn);
    int varname_id = prolog_atom_intern("$VARNAME");
    int underscore_id = prolog_atom_intern("_");
    for (int i = 0; i < pn; i++) {
        if (counts[i] != 1) continue;
        pl_cell_t *arg = (pl_cell_t *)rt_ws_alloc_descr(1);
        *arg = pl_make_atom(underscore_id);
        pl_bind(pool[i], pl_make_compound(varname_id, 1, arg));
    }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_name_query_vars_cell(void *list_cell, void *rest_cell) {
    extern void rt_pl_iso_throw_instantiation(void);
    extern void rt_pl_iso_throw_type(const char *, DESCR_t);
    int dot_id = prolog_atom_intern(".");
    int varname_id = prolog_atom_intern("$VARNAME");
    if (plc_list_cells(list_cell, dot_id) < 0) { rt_pl_iso_throw_type("list", *pl_deref((pl_cell_t *)list_cell)); return 0; }
    pl_cell_t *rest_pairs[plc_list_len(list_cell, dot_id) + 1];
    int nr = 0;
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
                pl_cell_t *nm = pl_deref(&pk[0]);
                pl_cell_t *vr = pl_deref(&pk[1]);
                const char *nm_txt = pl_atom_text(nm);
                if (nm_txt && pl_cell_unbound(vr)) { pl_cell_t *arg = (pl_cell_t *)rt_ws_alloc_descr(1); *arg = *nm; pl_bind(vr, pl_make_compound(varname_id, 1, arg)); bound_ok = 1; }
            }
        }
        if (!bound_ok) rest_pairs[nr++] = &kids[0];
        cur = &kids[1];
    }
    pl_cell_t result = pl_nil_cell();
    for (int i = nr - 1; i >= 0; i--) { pl_cell_t *blk = (pl_cell_t *)rt_ws_alloc_descr(2); blk[0] = *rest_pairs[i]; blk[1] = result; result = pl_make_compound(dot_id, 2, blk); }
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
    extern void rt_pl_iso_throw_type(const char *, DESCR_t);
    extern void rt_pl_iso_throw_domain(const char *, DESCR_t);
    int dot_id = prolog_atom_intern(".");
    int var_id = prolog_atom_intern("$VAR");
    int varname_id = prolog_atom_intern("$VARNAME");
    int use_namevars = 0;
    long from_n = 0;
    pl_cell_t *next_var = (pl_cell_t *)0;
    pl_cell_t *exclude_list = (pl_cell_t *)0;
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
            const char *f = prolog_atom_name(plc_functor(opt));
            pl_cell_t *oa = (pl_cell_t *)pl_compound_heap(opt);
            pl_cell_t *av = pl_deref(&oa[0]);
            if (f && !strcmp(f, "from")) {
                if (pl_cell_unbound(av)) { rt_pl_iso_throw_instantiation(); return 0; }
                if ((int)av->v != DT_I) { rt_pl_iso_throw_domain("var_binding_option", *opt); return 0; }
                from_n = (long)av->i;
            } else if (f && !strcmp(f, "next")) {
                if (!pl_cell_unbound(av) && (int)av->v != DT_I) { rt_pl_iso_throw_domain("var_binding_option", *opt); return 0; }
                next_var = &oa[0];
            } else if (f && !strcmp(f, "exclude")) {
                exclude_list = &oa[0];
            } else {
                rt_pl_iso_throw_domain("var_binding_option", *opt);
                return 0;
            }
        } else {
            rt_pl_iso_throw_domain("var_binding_option", *opt);
            return 0;
        }
        cur = &kids[1];
    }
    if (exclude_list && plc_list_cells(exclude_list, dot_id) < 0) { rt_pl_iso_throw_type("list", *pl_deref(exclude_list)); return 0; }
    long used[exclude_list ? plc_list_len(exclude_list, dot_id) + 1 : 1];
    int nused = 0;
    if (exclude_list) {
        pl_cell_t *ecur = exclude_list;
        for (;;) {
            pl_cell_t *ed = pl_deref(ecur);
            if (pl_is_nil(ed)) break;
            if ((int)ed->v != DT_PLREF || pl_arity(ed) != 2 || plc_functor(ed) != dot_id) break;
            pl_cell_t *ekids = (pl_cell_t *)pl_compound_heap(ed);
            pl_cell_t *el = pl_deref(&ekids[0]);
            if ((int)el->v == DT_PLREF && pl_arity(el) == 1 && plc_functor(el) == var_id) {
                pl_cell_t *ea = (pl_cell_t *)pl_compound_heap(el);
                pl_cell_t *ev = pl_deref(&ea[0]);
                if ((int)ev->v == DT_I) used[nused++] = (long)ev->i;
            }
            ecur = &ekids[1];
        }
    }
    int occ = plc_var_occ((pl_cell_t *)term_cell) + 1;
    pl_cell_t *pool[occ];
    int dummy_counts[occ];
    int pn = 0;
    pl_distinct_vars_walk((pl_cell_t *)term_cell, pool, dummy_counts, &pn);
    long n = from_n;
    for (int i = 0; i < pn; i++) {
        for (;;) { int clash = 0; for (int k = 0; k < nused; k++) if (used[k] == n) { clash = 1; break; } if (!clash) break; n++; }
        pl_cell_t *arg = (pl_cell_t *)rt_ws_alloc_descr(1);
        if (use_namevars) {
            char nb[32];
            pl_namevars_letters(n, nb, sizeof nb);
            *arg = pl_make_atom(prolog_atom_intern(nb));
            pl_bind(pool[i], pl_make_compound(varname_id, 1, arg));
        } else {
            *arg = pl_make_int(n);
            pl_bind(pool[i], pl_make_compound(var_id, 1, arg));
        }
        n++;
    }
    if (next_var) { pl_cell_t nv = pl_make_int(n); if (!pl_unify(next_var, &nv)) { return 0; } }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define PL_ACYC_ON_PATH ((DTYPE_t)(DT_PLREF + 1))
static int pl_acyclic_walk(pl_cell_t *c) {
    pl_cell_t *first = (pl_cell_t *)0;
    long nmarked = 0;
    int ret = 1;
    for (;;) {
        pl_cell_t *d = pl_deref(c);
        if (d->v == PL_ACYC_ON_PATH) { ret = 0; break; }
        if ((int)d->v != DT_PLREF) break;
        int ar = pl_arity(d);
        pl_cell_t *aa = (pl_cell_t *)pl_compound_heap(d);
        if (ar <= 0) break;
        d->v = PL_ACYC_ON_PATH;
        if (!nmarked++) first = d;
        int bad = 0;
        for (int i = 0; i < ar - 1; i++) if (!pl_acyclic_walk(&aa[i])) { bad = 1; break; }
        if (bad) { ret = 0; break; }
        c = &aa[ar - 1];
    }
    for (pl_cell_t *d = first; nmarked > 0; nmarked--) { d->v = (DTYPE_t)DT_PLREF; if (nmarked > 1) d = pl_deref(&((pl_cell_t *)pl_compound_heap(d))[pl_arity(d) - 1]); }
    return ret;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_acyclic_cell(void *term_cell) { return term_cell ? pl_acyclic_walk((pl_cell_t *)term_cell) : 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_term_string_cell(void *term_cell, void *str_cell, int as_str, pl_tr_ctx_t *cx) {
    extern FILE *fh_memsink_open(char **, size_t *);
    char *buf = (char *)0;
    size_t len = 0;
    FILE *ms = fh_memsink_open(&buf, &len);
    if (!ms) { return 0; }
    plc_vmap m;
    m.n = 0;
    m.vnc = 0;
    m.fp = ms;
    m.portray = (int (*)(pl_cell_t *, plc_vmap *))0;
    m.pthrown = (void *)0;
    m.cx = (pl_tr_ctx_t *)0;
    m.pheld = 0;
    m.pgoal = (pl_cell_t *)0;
    m.popts = (pl_cell_t *)0;
    m.space_args = 0;
    m.next_arg = 0;
    plc_writeq((pl_cell_t *)term_cell, &m);
    if (fclose(ms) != 0) { return 0; }
    return plc_text_out(str_cell, buf ? buf : "", as_str, cx);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { plr_stk_t ag, lg; pl_cell_t *s; pl_cell_t *d; int atts; pl_tr_ctx_t *cx; long nfr; } plc_cp_t;
static int plc_copy_run(void *wv) {
    plc_cp_t *w = (plc_cp_t *)wv;
    for (;;) {
        pl_cell_t *d = plr_resolve(pl_deref(w->s));
        if (d->v == PLR_COPY) *w->d = pl_make_compound(plc_fid_name(d->slen), plc_fid_arity(d->slen), d->p);
        else if (d->v == PLR_VCOPY) {
            pl_cell_t *f = (pl_cell_t *)d->p;
            *w->d = pl_make_ref(f, (int)f->slen);
        } else if (pl_cell_unbound(d)) {
            pl_cell_t *fresh;
            uint64_t w0;
            int at = w->atts && pl_attv_is(d);
            if (!plr_room(&w->lg) || (at && !plr_room(&w->ag))) return plr_more(&w->ag, &w->lg, plc_copy_run, w);
            fresh = (pl_cell_t *)rt_ws_alloc_descr(at ? 2 : 1);
            if (!fresh) *w->d = *d;
            else {
                if (at) { fresh[0].v = (DTYPE_t)DT_PLVAR; fresh[0].slen = PL_ATTV_SLEN; fresh[0].p = (void *)fresh; } else pl_init_var(fresh, -1);
                if (at && pl_tr_residue_on(w->cx)) { memset(&fresh[1], 0, sizeof fresh[1]); pl_tr_push_residue(w->cx, fresh + 1); }
                memcpy(&w0, d, 8);
                plr_push(&w->lg, d, d->p, w0);
                d->v = PLR_VCOPY;
                d->p = (void *)fresh;
                *w->d = pl_make_ref(fresh, (int)fresh->slen);
                if (at) plr_push(&w->ag, d + 1, fresh + 1, 1);
            }
        } else if ((int)d->v == DT_PLREF) {
            int fn = plc_fid_name(d->slen), ar = plc_fid_arity(d->slen);
            pl_cell_t *aa = (pl_cell_t *)d->p, *na;
            uint64_t w0;
            if (!plr_room(&w->lg) || (ar > 0 && !plr_room(&w->ag))) return plr_more(&w->ag, &w->lg, plc_copy_run, w);
            na = (pl_cell_t *)rt_ws_alloc_descr((size_t)(ar > 0 ? ar : 1));
            if (!na) *w->d = *d;
            else {
                memcpy(&w0, d, 8);
                *w->d = pl_make_compound(fn, ar, na);
                plr_push(&w->lg, d, (void *)aa, w0);
                d->v = PLR_COPY;
                d->p = (void *)na;
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
static pl_cell_t plc_copy_cx(pl_cell_t *c, int atts, pl_tr_ctx_t *cx) {
    pl_cell_t out;
    plc_cp_t w;
    memset(&out, 0, sizeof out);
    plr_stk_init(&w.ag);
    plr_stk_init(&w.lg);
    w.s = c;
    w.d = &out;
    w.atts = atts;
    w.cx = cx;
    w.nfr = 0;
    (void)plc_copy_run(&w);
    return out;
}
static pl_cell_t plc_copy_x(pl_cell_t *c, int atts) { return plc_copy_cx(c, atts, (pl_tr_ctx_t *)0); }
static pl_cell_t plc_copy(pl_cell_t *c) { return plc_copy_x(c, 0); }
#define PLC_SH_FRAME (1ull << 63)
#define PLC_SH_CHG (1ull << 62)
#define PLC_SH_ATT (1ull << 61)
#define PLC_SH_CNT 0xFFFFFFFFull
#define PLC_SH_BUDGET 1024
_Static_assert(PLC_SH_BUDGET > 0,
    "copy_term shares a subterm its copy would leave unchanged (SWI shares ground subterms), deciding at the subterm's end, so a compound still unchanged when its last argument comes "
    "up keeps its frame; past this many kept frames a compound is copied instead of shared, so a long ground spine costs the copier no more stack than the plain copy's own marks");
static int plc_sh_room2(const plr_stk_t *s) { return s->top && (s->top->cap - s->top->n >= 2 || s->top->next); }
static void plc_sh_changed(plc_cp_t *w) { if (!plr_empty(&w->ag)) plr_peek(&w->ag)->n |= PLC_SH_CHG; }
static int plc_copy_share_run(void *wv);
static int plc_sh_more(plc_cp_t *w) {
    long na = plc_sh_room2(&w->ag) ? 0 : w->ag.want, nb = plr_room(&w->lg) ? 0 : w->lg.want;
    plr_ent_t ea[na > 0 ? na : 1], eb[nb > 0 ? nb : 1];
    plr_seg_t sa, sb;
    if (na) plr_link(&w->ag, &sa, ea, na);
    if (nb) plr_link(&w->lg, &sb, eb, nb);
    return plc_copy_share_run(w);
}
static int plc_copy_share_run(void *wv) {
    plc_cp_t *w = (plc_cp_t *)wv;
    for (;;) {
        pl_cell_t *d = plr_resolve(pl_deref(w->s));
        if (d->v == PLR_COPY) {
            *w->d = pl_make_compound(plc_fid_name(d->slen), plc_fid_arity(d->slen), d->p);
            plc_sh_changed(w);
        } else if (d->v == PLR_VCOPY) {
            pl_cell_t *f = (pl_cell_t *)d->p;
            *w->d = pl_make_ref(f, (int)f->slen);
            if (f != d) plc_sh_changed(w);
        } else if (pl_cell_unbound(d)) {
            pl_cell_t *fresh;
            uint64_t w0;
            int at = w->atts && pl_attv_is(d);
            if (!plr_room(&w->lg) || (at && !plr_room(&w->ag))) return plc_sh_more(w);
            fresh = (pl_cell_t *)rt_ws_alloc_descr(at ? 2 : 1);
            if (at) { fresh[0].v = (DTYPE_t)DT_PLVAR; fresh[0].slen = PL_ATTV_SLEN; fresh[0].p = (void *)fresh; } else pl_init_var(fresh, -1);
            if (at && pl_tr_residue_on(w->cx)) { memset(&fresh[1], 0, sizeof fresh[1]); pl_tr_push_residue(w->cx, fresh + 1); }
            memcpy(&w0, d, 8);
            plr_push(&w->lg, d, d->p, w0);
            d->v = PLR_VCOPY;
            d->p = (void *)fresh;
            *w->d = pl_make_ref(fresh, (int)fresh->slen);
            plc_sh_changed(w);
            if (at) plr_push(&w->ag, d + 1, fresh + 1, 1 | PLC_SH_ATT);
        } else if ((int)d->v == DT_PLREF) {
            int ar = plc_fid_arity(d->slen);
            pl_cell_t *aa = (pl_cell_t *)d->p, *na;
            uint64_t w0;
            if (ar <= 0) *w->d = *d;
            else {
                if (!plr_room(&w->lg) || !plc_sh_room2(&w->ag)) return plc_sh_more(w);
                na = (pl_cell_t *)rt_ws_alloc_descr((size_t)ar);
                memcpy(&w0, d, 8);
                *w->d = pl_make_compound(plc_fid_name(d->slen), ar, na);
                plr_push(&w->lg, d, (void *)aa, w0);
                plr_push(&w->ag, (void *)w->d, (void *)aa, PLC_SH_FRAME | (uint64_t)d->slen);
                w->nfr++;
                plr_push(&w->ag, (void *)aa, (void *)na, (uint64_t)ar);
                d->v = PLR_COPY;
                d->p = (void *)na;
            }
        } else *w->d = *d;
        for (;;) {
            plr_ent_t *t;
            if (plr_empty(&w->ag)) goto done;
            t = plr_peek(&w->ag);
            if (t->n & PLC_SH_CNT) {
                w->s = (pl_cell_t *)t->x;
                w->d = (pl_cell_t *)t->y;
                t->x = (void *)(w->s + 1);
                t->y = (void *)(w->d + 1);
                if (--t->n & PLC_SH_CNT || (!(t->n & (PLC_SH_CHG | PLC_SH_ATT)) && w->nfr <= PLC_SH_BUDGET)) break;
                { uint64_t f = t->n; plr_drop(&w->ag); if (!(f & PLC_SH_ATT)) { plr_drop(&w->ag); w->nfr--; plc_sh_changed(w); } }
                break;
            }
            {
                uint64_t f = t->n;
                plr_drop(&w->ag);
                if (f & PLC_SH_ATT) continue;
                t = plr_peek(&w->ag);
                {
                    pl_cell_t *dst = (pl_cell_t *)t->x, r;
                    memset(&r, 0, sizeof r);
                    r.v = (DTYPE_t)DT_PLREF;
                    r.slen = (uint32_t)(t->n & PLC_SH_CNT);
                    r.p = t->y;
                    plr_drop(&w->ag);
                    w->nfr--;
                    if (!(f & PLC_SH_CHG)) *dst = r;
                    else plc_sh_changed(w);
                }
            }
        }
    }
    done:
    for (plr_seg_t *g = w->lg.top; g; g = g->prev) for (long i = g->n; i-- > 0; ) { pl_cell_t *c = (pl_cell_t *)g->e[i].x; uint64_t w0 = g->e[i].n; memcpy(c, &w0, 8); c->p = g->e[i].y; }
    return 0;
}
static pl_cell_t plc_copy_shared(pl_cell_t *c, int atts, pl_tr_ctx_t *cx) {
    pl_cell_t out;
    plc_cp_t w;
    memset(&out, 0, sizeof out);
    plr_stk_init(&w.ag);
    plr_stk_init(&w.lg);
    w.s = c;
    w.d = &out;
    w.atts = atts;
    w.cx = cx;
    w.nfr = 0;
    (void)plc_copy_share_run(&w);
    return out;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
pl_cell_t rt_pl_cell_snapshot(void *cell) { return plc_copy((pl_cell_t *)cell); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_ball_make(DESCR_t *a, int n) {
    extern void *rt_pl_ball_instantiation(void);
    if (!a || n < 1) return (void *)0;
    if (pl_cell_unbound(pl_deref((pl_cell_t *)&a[0]))) return rt_pl_ball_instantiation();
    pl_cell_t *b = (pl_cell_t *)PL_CELL_ALLOC(sizeof(pl_cell_t));
    if (!b) return (void *)0;
    *b = plc_copy_x((pl_cell_t *)&a[0], 1);
    return (void *)b;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *pl_ball_key_str(DESCR_t d) { if ((int)d.v == DT_PLATOM) return prolog_atom_name((int)d.i); if ((int)d.v == DT_S || (int)d.v == DT_N) return d.s; return (const char *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_ball_existence(DESCR_t *a, int n) { extern void *rt_pl_ball_existence_key(const char *key); return rt_pl_ball_existence_key((a && n > 0) ? pl_ball_key_str(a[0]) : (const char *)0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_ball_existence_key(const char *key) {
    extern int rt_pl_unknown_suppress(const char *);
    int ar = 0;
    if (rt_pl_unknown_suppress(key)) return (void *)0;
    const char *sl = key ? strrchr(key, '/') : (const char *)0;
    int nid = sl ? prolog_atom_intern_n(key, (size_t)(sl - key)) : prolog_atom_intern(key ? key : "?");
    if (sl) ar = atoi(sl + 1);
    pl_cell_t pi[2];
    pi[0] = pl_make_atom(nid);
    pi[1] = pl_make_int(ar);
    pl_cell_t *pic = (pl_cell_t *)rt_pl_compound_cell("/", 2, (void *)pi);
    if (!pic) return (void *)0;
    pl_cell_t ee[2];
    ee[0] = pl_make_atom(prolog_atom_intern("procedure"));
    ee[1] = *pic;
    pl_cell_t *eec = (pl_cell_t *)rt_pl_compound_cell("existence_error", 2, (void *)ee);
    if (!eec) return (void *)0;
    pl_cell_t er[2];
    er[0] = *eec;
    er[1] = *pic;
    return rt_pl_compound_cell("error", 2, (void *)er);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_ball_evaluable(const char *name, int arity) {
    extern void *rt_pl_ball_type_pi(const char *kind, const char *what, const char *nm, int ar);
    return rt_pl_ball_type_pi("type_error", "evaluable", name, arity);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_ball_type_pi(const char *kind, const char *what, const char *nm, int ar) {
    extern DESCR_t rt_pl_fresh_var_ref(void);
    pl_cell_t pi[2];
    pl_cell_t *pic;
    pl_cell_t te[2];
    pl_cell_t *tec;
    pl_cell_t er[2];
    pi[0] = pl_make_atom(prolog_atom_intern(nm ? nm : "?"));
    pi[1] = pl_make_int(ar);
    pic = (pl_cell_t *)rt_pl_compound_cell("/", 2, (void *)pi);
    if (!pic) return (void *)0;
    te[0] = pl_make_atom(prolog_atom_intern(what ? what : "evaluable"));
    te[1] = *pic;
    tec = (pl_cell_t *)rt_pl_compound_cell(kind ? kind : "type_error", 2, (void *)te);
    if (!tec) return (void *)0;
    er[0] = *tec;
    er[1] = rt_pl_fresh_var_ref();
    return rt_pl_compound_cell("error", 2, (void *)er);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_ball_eval_error(const char *kind, const char *op, int arity) {
    extern DESCR_t rt_pl_fresh_var_ref(void);
    pl_cell_t ee[1];
    pl_cell_t *eec;
    pl_cell_t pi[2];
    pl_cell_t *pic;
    pl_cell_t ct[2];
    pl_cell_t *ctc;
    pl_cell_t er[2];
    ee[0] = pl_make_atom(prolog_atom_intern(kind ? kind : "zero_divisor"));
    eec = (pl_cell_t *)rt_pl_compound_cell("evaluation_error", 1, (void *)ee);
    if (!eec) return (void *)0;
    pi[0] = pl_make_atom(prolog_atom_intern(op ? op : "is"));
    pi[1] = pl_make_int(arity);
    pic = (pl_cell_t *)rt_pl_compound_cell("/", 2, (void *)pi);
    if (!pic) return (void *)0;
    ct[0] = *pic;
    ct[1] = rt_pl_fresh_var_ref();
    ctc = (pl_cell_t *)rt_pl_compound_cell("context", 2, (void *)ct);
    if (!ctc) return (void *)0;
    er[0] = *eec;
    er[1] = *ctc;
    return rt_pl_compound_cell("error", 2, (void *)er);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_ball_kind1_system_ctx(const char *kind, const char *arg0_atom, const char *nm, int ar, const char *msg) {
    extern DESCR_t rt_pl_fresh_var_ref(void);
    pl_cell_t fe[1];
    pl_cell_t *fec;
    pl_cell_t pi[2];
    pl_cell_t *pic;
    pl_cell_t sy[2];
    pl_cell_t *syc;
    pl_cell_t ct[2];
    pl_cell_t *ctc;
    pl_cell_t er[2];
    fe[0] = pl_make_atom(prolog_atom_intern(arg0_atom ? arg0_atom : "term"));
    fec = (pl_cell_t *)rt_pl_compound_cell(kind, 1, (void *)fe);
    if (!fec) return (void *)0;
    pi[0] = pl_make_atom(prolog_atom_intern(nm ? nm : "?"));
    pi[1] = pl_make_int(ar);
    pic = (pl_cell_t *)rt_pl_compound_cell("/", 2, (void *)pi);
    if (!pic) return (void *)0;
    sy[0] = pl_make_atom(prolog_atom_intern("system"));
    sy[1] = *pic;
    syc = (pl_cell_t *)rt_pl_compound_cell(":", 2, (void *)sy);
    if (!syc) return (void *)0;
    ct[0] = *syc;
    ct[1] = msg ? pl_make_atom(prolog_atom_intern(msg)) : rt_pl_fresh_var_ref();
    ctc = (pl_cell_t *)rt_pl_compound_cell("context", 2, (void *)ct);
    if (!ctc) return (void *)0;
    er[0] = *fec;
    er[1] = *ctc;
    return rt_pl_compound_cell("error", 2, (void *)er);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_ball_kind2(const char *kind, const char *arg0_atom, DESCR_t culprit) {
    extern DESCR_t rt_pl_fresh_var_ref(void);
    pl_cell_t fe[2];
    pl_cell_t *fec;
    pl_cell_t er[2];
    fe[0] = pl_make_atom(prolog_atom_intern(arg0_atom ? arg0_atom : "term"));
    fe[1] = rt_pl_cell_snapshot(&culprit);
    fec = (pl_cell_t *)rt_pl_compound_cell(kind, 2, (void *)fe);
    if (!fec) return (void *)0;
    er[0] = *fec;
    er[1] = rt_pl_fresh_var_ref();
    return rt_pl_compound_cell("error", 2, (void *)er);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_ball_occurs(DESCR_t *var, DESCR_t *term) {
    extern DESCR_t rt_pl_fresh_var_ref(void);
    pl_cell_t oc[2];
    pl_cell_t *occ;
    pl_cell_t er[2];
    oc[0].v = (DTYPE_t)DT_PLVAR;
    oc[0].slen = 0;
    oc[0].p = (void *)var;
    oc[1] = *term;
    occ = (pl_cell_t *)rt_pl_compound_cell("occurs_check", 2, (void *)oc);
    if (!occ) return (void *)0;
    er[0] = rt_pl_cell_snapshot(occ);
    er[1] = rt_pl_fresh_var_ref();
    return rt_pl_compound_cell("error", 2, (void *)er);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_ball_kind2_pi(const char *kind, const char *arg0_atom, DESCR_t culprit, const char *nm, int ar) {
    pl_cell_t fe[2];
    pl_cell_t *fec;
    pl_cell_t pi[2];
    pl_cell_t *pic;
    pl_cell_t er[2];
    fe[0] = pl_make_atom(prolog_atom_intern(arg0_atom ? arg0_atom : "term"));
    fe[1] = rt_pl_cell_snapshot(&culprit);
    fec = (pl_cell_t *)rt_pl_compound_cell(kind, 2, (void *)fe);
    if (!fec) return (void *)0;
    pi[0] = pl_make_atom(prolog_atom_intern(nm ? nm : "?"));
    pi[1] = pl_make_int(ar);
    pic = (pl_cell_t *)rt_pl_compound_cell("/", 2, (void *)pi);
    if (!pic) return (void *)0;
    er[0] = *fec;
    er[1] = *pic;
    return rt_pl_compound_cell("error", 2, (void *)er);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_ball_set_pi(void *ball, const char *nm, int ar) {
    pl_cell_t pi[2];
    pl_cell_t *pic;
    pl_cell_t *b = (pl_cell_t *)ball;
    if (!b || (int)b->v != DT_PLREF || !b->p) return ball;
    pi[0] = pl_make_atom(prolog_atom_intern(nm ? nm : "?"));
    pi[1] = pl_make_int(ar);
    pic = (pl_cell_t *)rt_pl_compound_cell("/", 2, (void *)pi);
    if (pic) ((pl_cell_t *)b->p)[1] = *pic;
    return ball;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_ball_culprit1(const char *kind, DESCR_t culprit) {
    extern DESCR_t rt_pl_fresh_var_ref(void);
    pl_cell_t fe[1];
    pl_cell_t *fec;
    pl_cell_t er[2];
    fe[0] = rt_pl_cell_snapshot(&culprit);
    fec = (pl_cell_t *)rt_pl_compound_cell(kind, 1, (void *)fe);
    if (!fec) return (void *)0;
    er[0] = *fec;
    er[1] = rt_pl_fresh_var_ref();
    return rt_pl_compound_cell("error", 2, (void *)er);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_ball_kind1(const char *kind, const char *arg0_atom) {
    extern DESCR_t rt_pl_fresh_var_ref(void);
    pl_cell_t fe[1];
    pl_cell_t *fec;
    pl_cell_t er[2];
    const char *sep = arg0_atom ? strchr(arg0_atom, 1) : (const char *)0;
    if (sep) {
        char nm[64];
        size_t k = (size_t)(sep - arg0_atom) < sizeof nm - 1 ? (size_t)(sep - arg0_atom) : sizeof nm - 1;
        pl_cell_t ia[1];
        pl_cell_t *ic;
        memcpy(nm, arg0_atom, k);
        nm[k] = 0;
        ia[0] = pl_make_atom(prolog_atom_intern(sep + 1));
        ic = (pl_cell_t *)rt_pl_compound_cell(nm, 1, (void *)ia);
        if (!ic) return (void *)0;
        fe[0] = *ic;
    } else fe[0] = pl_make_atom(prolog_atom_intern(arg0_atom ? arg0_atom : "term"));
    fec = (pl_cell_t *)rt_pl_compound_cell(kind, 1, (void *)fe);
    if (!fec) return (void *)0;
    er[0] = *fec;
    er[1] = rt_pl_fresh_var_ref();
    return rt_pl_compound_cell("error", 2, (void *)er);
}
void *rt_pl_cap_error(void) { return rt_pl_ball_kind1("resource_error", "stack"); }
void *rt_pl_cap_ball(void) {
    extern void rt_gcheap_cap_raised(void);
    void *b = rt_pl_cap_error();
    if (!b) { fprintf(stderr, "[ZHP] REFUSED: the resource error at the hard cap could not be built from the error reserve\n"); exit(2); }
    rt_gcheap_cap_raised();
    return b;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_ball_instantiation(void) {
    extern DESCR_t rt_pl_fresh_var_ref(void);
    pl_cell_t er[2];
    er[0] = pl_make_atom(prolog_atom_intern("instantiation_error"));
    er[1] = rt_pl_fresh_var_ref();
    return rt_pl_compound_cell("error", 2, (void *)er);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_ball_permission_pi(const char *op, const char *type, const char *nm, int ar) {
    extern DESCR_t rt_pl_fresh_var_ref(void);
    pl_cell_t pi[2];
    pl_cell_t *pic;
    pl_cell_t pe[3];
    pl_cell_t *pec;
    pl_cell_t er[2];
    pi[0] = pl_make_atom(prolog_atom_intern(nm ? nm : "?"));
    pi[1] = pl_make_int(ar);
    pic = (pl_cell_t *)rt_pl_compound_cell("/", 2, (void *)pi);
    if (!pic) return (void *)0;
    pe[0] = pl_make_atom(prolog_atom_intern(op ? op : "modify"));
    pe[1] = pl_make_atom(prolog_atom_intern(type ? type : "static_procedure"));
    pe[2] = *pic;
    pec = (pl_cell_t *)rt_pl_compound_cell("permission_error", 3, (void *)pe);
    if (!pec) return (void *)0;
    er[0] = *pec;
    er[1] = rt_pl_fresh_var_ref();
    return rt_pl_compound_cell("error", 2, (void *)er);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_ball_permission3(const char *op, const char *type, DESCR_t culprit) {
    extern DESCR_t rt_pl_fresh_var_ref(void);
    pl_cell_t pe[3];
    pl_cell_t *pec;
    pl_cell_t er[2];
    pe[0] = pl_make_atom(prolog_atom_intern(op ? op : "access"));
    pe[1] = pl_make_atom(prolog_atom_intern(type ? type : "stream"));
    pe[2] = rt_pl_cell_snapshot(&culprit);
    pec = (pl_cell_t *)rt_pl_compound_cell("permission_error", 3, (void *)pe);
    if (!pec) return (void *)0;
    er[0] = *pec;
    er[1] = rt_pl_fresh_var_ref();
    return rt_pl_compound_cell("error", 2, (void *)er);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_root_omega(void) { extern void *rt_pl_ball_take(void); void *ball = rt_pl_ball_take(); if (!ball) exit(0); rt_pl_ball_report(ball); exit(2); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_ball_report(void *ball) {
    extern void rt_pl_write_cell_fp(void *, FILE *);
    fflush(stdout);
    fprintf(stderr, "Warning: goal raised exception: ");
    if (ball) rt_pl_write_cell_fp(ball, stderr);
    else fprintf(stderr, "unknown");
    fprintf(stderr, "\n");
    fflush(stderr);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_cell_atom_id(pl_cell_t *c) {
    pl_cell_t *d = pl_deref(c);
    if ((int)d->v == DT_PLATOM) return (int)d->i;
    if ((int)d->v == DT_S) { extern int prolog_atom_intern(const char *); return prolog_atom_intern(d->s ? d->s : ""); }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_copy_term_cell(void *term_cell, void *copy_cell, pl_tr_ctx_t *cx) {
    pl_cell_t copy = plc_copy_cx((pl_cell_t *)term_cell, 1, cx);
    if (!plc_unify_cells_cx((pl_cell_t *)copy_cell, &copy, cx)) { return 0; }
    return 1;
}
void rt_pl_residue_recopy(DESCR_t *el, int n, pl_tr_ctx_t *cx) { if (pl_tr_residue_on(cx)) for (int i = 0; i < n; i++) el[i] = plc_copy_cx((pl_cell_t *)&el[i], 1, cx); }
int rt_pl_copy_term_share_cell(void *term_cell, void *copy_cell, pl_tr_ctx_t *cx) {
    if (!rt_pl_dialect_swi()) return rt_pl_copy_term_cell(term_cell, copy_cell, cx);
    pl_cell_t copy = plc_copy_shared((pl_cell_t *)term_cell, 1, cx);
    return plc_unify_cells_cx((pl_cell_t *)copy_cell, &copy, cx) ? 1 : 0;
}
int rt_pl_copy_keep_cell(void *keep_cell, void *src_cell, void *dst_cell, void *atts_cell, pl_tr_ctx_t *cx) {
    pl_cell_t *a = pl_deref((pl_cell_t *)atts_cell), copy;
    if ((int)a->v != DT_I) return 0;
    for (pl_cell_t *l = pl_deref((pl_cell_t *)keep_cell); (int)l->v == DT_PLREF && plc_fid_arity(l->slen) == 2; l = pl_deref(&((pl_cell_t *)l->p)[1])) {
        pl_cell_t *v = pl_deref(&((pl_cell_t *)l->p)[0]);
        if ((int)v->v == DT_PLVAR && v->p == (void *)v) v->v = PLR_VCOPY;
    }
    copy = plc_copy_shared((pl_cell_t *)src_cell, a->i != 0, cx);
    for (pl_cell_t *l = pl_deref((pl_cell_t *)keep_cell); (int)l->v == DT_PLREF && plc_fid_arity(l->slen) == 2; l = pl_deref(&((pl_cell_t *)l->p)[1])) {
        pl_cell_t *v = pl_deref(&((pl_cell_t *)l->p)[0]);
        if (v->v == PLR_VCOPY && v->p == (void *)v) v->v = (DTYPE_t)DT_PLVAR;
    }
    return plc_unify_cells_cx((pl_cell_t *)dst_cell, &copy, cx) ? 1 : 0;
}
int rt_pl_factorize_term_cell(void *term_cell, void *skel_cell, void *subst_cell, pl_tr_ctx_t *cx) {
    pl_cell_t f = plc_factorize_x((pl_cell_t *)term_cell, 0, 1);
    pl_cell_t *fa = (pl_cell_t *)f.p;
    return plc_unify_cells_cx((pl_cell_t *)skel_cell, &fa[0], cx) && plc_unify_cells_cx((pl_cell_t *)subst_cell, &fa[1], cx) ? 1 : 0;
}
int rt_pl_copy_term_nat_cell(void *term_cell, void *copy_cell, pl_tr_ctx_t *cx) {
    pl_cell_t copy = plc_copy_x((pl_cell_t *)term_cell, 0);
    if (!plc_unify_cells_cx((pl_cell_t *)copy_cell, &copy, cx)) { return 0; }
    return 1;
}
static inline pl_cell_t *fa_items(DESCR_t *a) { return (pl_cell_t *)a[1].ptr; }
static inline int fa_n(DESCR_t *a) { return (int)a[0].i; }
static inline int fa_cap(DESCR_t *a) { return (int)a[0].slen; }
static inline void fa_set(DESCR_t *a, int n, int cap) { a[0].v = (DTYPE_t)DT_I; a[0].slen = (uint32_t)cap; a[0].i = (int64_t)n; }
static inline void fa_items_set(DESCR_t *a, pl_cell_t *p) { a[1].v = (DTYPE_t)DT_DATA; a[1].slen = DATA_ELEMS_SLEN; a[1].ptr = (void *)p; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void * rt_pl_findall_begin(void) {
    DESCR_t *a = (DESCR_t *)rt_ws_alloc_descr(2);
    if (!a) return (void *)0;
    { pl_cell_t *it = (pl_cell_t *)rt_ws_alloc_descr(16); if (!it) return (void *)0; fa_set(a, 0, 16); fa_items_set(a, it); }
    return (void *)a;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_findall_collect(void *acc_v, void *tmpl_term) {
    DESCR_t *a = (DESCR_t *)acc_v;
    if (!a || !fa_items(a)) return;
    if (fa_n(a) >= fa_cap(a)) {
        extern int rt_pl_cap_refuses(unsigned long long, unsigned long long);
        extern int rt_gcheap_cap_refused(void);
        int nc = fa_cap(a) * 2;
        if (rt_pl_cap_refuses((unsigned long long)nc + (unsigned long long)fa_cap(a), sizeof(DESCR_t)) && rt_gcheap_cap_refused()) return;
        pl_cell_t *ni = (pl_cell_t *)rt_ws_alloc_descr((size_t)nc);
        if (!ni) return;
        for (int i = 0; i < fa_n(a); i++) ni[i] = fa_items(a)[i];
        fa_items_set(a, ni);
        fa_set(a, fa_n(a), nc);
    }
    { int n = fa_n(a); pl_cell_t item = plc_copy_x((pl_cell_t *)tmpl_term, 1); fa_items(a)[n] = item; fa_set(a, n + 1, fa_cap(a)); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_findall_count(void *acc_v) { DESCR_t *a = (DESCR_t *)acc_v; return a ? fa_n(a) : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_findall_item(void *acc_v, int i, void *out) { DESCR_t *a = (DESCR_t *)acc_v; if (!a || !out || i < 0 || i >= fa_n(a)) return; *(pl_cell_t *)out = fa_items(a)[i]; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_findall_finish(void *acc_v, void *result_term) {
    DESCR_t *a = (DESCR_t *)acc_v;
    int dot = prolog_atom_intern(".");
    pl_cell_t lst;
    { extern const char *prolog_atom_name(int); const char *nm = prolog_atom_name(prolog_atom_intern("[]")); lst = pl_make_atom(prolog_atom_intern(nm ? nm : "")); }
    int n = a ? fa_n(a) : 0;
    for (int i = n - 1; i >= 0; i--) { pl_cell_t *c = (pl_cell_t *)rt_ws_alloc_descr(2); if (!c) return 0; c[0] = fa_items(a)[i]; c[1] = lst; lst = pl_make_compound(dot, 2, c); }
    if (!pl_unify((pl_cell_t *)result_term, &lst)) { return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_agg_count_finish(void *acc_v, void *result_term) {
    DESCR_t *a = (DESCR_t *)acc_v;
    pl_cell_t cnt = pl_make_int((int64_t)(a ? fa_n(a) : 0));
    if (!pl_unify((pl_cell_t *)result_term, &cnt)) { return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int agg_num(pl_cell_t *c, long *iv, double *dv, int *isf) {
    pl_cell_t *d = c ? pl_deref(c) : (pl_cell_t *)0;
    if (!d) return 0;
    if ((int)d->v == DT_I) { *iv = (long)d->i; *isf = 0; return 1; }
    if ((int)d->v == DT_R) { *dv = d->r; *isf = 1; return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_agg_sum_finish(void *acc_v, void *result_term) {
    DESCR_t *a = (DESCR_t *)acc_v;
    int n = a ? fa_n(a) : 0;
    long si = 0;
    double sd = 0.0;
    int isf = 0;
    for (int i = 0; i < n; i++) {
        long iv = 0;
        double dv = 0.0;
        int ef = 0;
        if (!agg_num(&fa_items(a)[i], &iv, &dv, &ef)) return 0;
        if (ef) { sd += dv; isf = 1; } else { si += iv; sd += (double)iv; }
    }
    pl_cell_t r = isf ? pl_make_float(sd) : pl_make_int((int64_t)si);
    if (!pl_unify((pl_cell_t *)result_term, &r)) { return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_pl_agg_minmax_finish(void *acc_v, void *result_term, int want_max) {
    DESCR_t *a = (DESCR_t *)acc_v;
    int n = a ? fa_n(a) : 0;
    if (n <= 0) return 0;
    long bi = 0;
    double bd = 0.0;
    int isf = 0, have = 0;
    for (int i = 0; i < n; i++) {
        long iv = 0;
        double dv = 0.0;
        int ef = 0;
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
#define PL_DB_CELL0 24
typedef struct { pl_cell_t cl; int erased; int ref; int ridx; int chain_off; int next_idx; int frag; } pl_db_slot_t;
typedef struct {
    pl_db_slot_t *s;
    int n;
    int cap;
    int killed;
    int next_ref;
    int head_idx;
    int tail_idx;
    int cell_k;
    int erased_n;
    int recl_n;
    int recl_lo;
    int recl_gc;
    int recl_since;
    int frag_top;
    int tx_pins;
} pl_db_t;
#define PL_DB_FRAG_PAGES(f) (((f) >> 20) & 0x3FF)
#define PL_DB_FRAG_HELD 0x40000000
#define PL_DB_FRAG_GONE ((int)0x80000000u)
#define PL_DB_FRAG_REC(f) (((f) & 0x3FFFFFFF) != 0)
_Static_assert(__builtin_offsetof(pl_db_slot_t, frag) == 36,
    "frag at +36 is the C side's alone (no emitted code reads it): the fragment's first code-pool page in the low 20 bits, its page count in the next 10, HELD (a reclaim pass saw a holder) in bit 30"
    " and " "GONE (its pages reclaimed) in bit 31, 0 when unrecorded; a range of 1024 pages or more is never recorded, and 1024 pages is FLAT_BUF_MAX's floor, so a run-time compile nested inside the "
    "fragment's own emission (which leaves the emission's whole reservation below it) can never be taken for the fragment");
_Static_assert(sizeof(pl_db_slot_t) == 40 && __builtin_offsetof(pl_db_slot_t, erased) == 16 && __builtin_offsetof(pl_db_slot_t, ref) == 20 && __builtin_offsetof(pl_db_slot_t, ridx) == 24 &&
    __builtin_offsetof(pl_db_slot_t, chain_off) == 28 && __builtin_offsetof(pl_db_slot_t, next_idx) == 32,
    "xa_flat.cpp's chain-omega and bb_to.cpp's db walk bake the slot layout (ARCH-PROLOG-C-OUT-OF-THE-BOX 5.2 A/B)");
_Static_assert(__builtin_offsetof(pl_db_t, s) == 0 && __builtin_offsetof(pl_db_t, n) == 8 && __builtin_offsetof(pl_db_t, next_ref) == 20 && __builtin_offsetof(pl_db_t, head_idx) == 24,
    "xa_flat.cpp's packet entry and bb_to.cpp's db walk bake the store layout (ARCH-PROLOG-C-OUT-OF-THE-BOX 5.2 A/B)");
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void ** pl_db_cell_addr(void *root, int64_t k, int create);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void * rt_pl_db_get(void *root, int64_t k) {
    if (!root || k < 0) return (void *)0;
    {
        pl_db_t **cell = (pl_db_t **)pl_db_cell_addr(root, k, 1);
        if (!cell) return (void *)0;
        if (!*cell) {
            pl_db_t *d = (pl_db_t *)rt_pl_struct_alloc(HB_PLDB, sizeof *d);
            if (!d) return (void *)0;
            d->cap = 8;
            d->n = 0;
            d->killed = 0;
            d->next_ref = 1;
            d->head_idx = -1;
            d->tail_idx = -1;
            d->cell_k = (int)k;
            d->erased_n = 0;
            d->recl_n = 0;
            d->recl_lo = 0;
            d->recl_gc = -1;
            d->recl_since = 0;
            d->frag_top = -1;
            d->tx_pins = 0;
            d->s = (pl_db_slot_t *)rt_pl_struct_alloc(HB_PLDBS, (size_t)d->cap * sizeof(pl_db_slot_t));
            if (!d->s) { d->cap = 0; }
            *cell = d;
        }
        return (void *)*cell;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define PLC_GVAR_ABSENT 0x47564152u
static int plc_gvar_absent(const pl_cell_t *b) { return b && (int)b->v == DT_I && b->slen == PLC_GVAR_ABSENT; }
int rt_pl_nb_set(void *root, int64_t k, void *val) {
    extern void *rt_ws_alloc_descr(size_t);
    if (!root || k < 0 || !val) return 0;
    {
        pl_cell_t **cell = (pl_cell_t **)pl_db_cell_addr(root, k, 1);
        if (!cell) return 0;
        pl_cell_t *t = pl_deref((pl_cell_t *)val);
        pl_cell_t stored = plc_copy_x(t, 1);
        pl_cell_t *box = (pl_cell_t *)rt_ws_alloc_descr(1);
        if (!box) return 0;
        *box = stored;
        *cell = box;
        if ((int)stored.v == DT_PLREF || (int)stored.v == DT_PLVAR) rt_pl_tr_freeze();
        return 1;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_b_set(void *root, int64_t k, void *val, pl_tr_ctx_t *cx) {
    extern void *rt_ws_alloc_descr(size_t);
    if (!root || k < 0 || !val || !cx) return 0;
    {
        pl_cell_t **cell = (pl_cell_t **)pl_db_cell_addr(root, k, 1);
        if (!cell) return 0;
        pl_cell_t *j = (pl_cell_t *)rt_ws_alloc_descr(1);
        if (!j) return 0;
        j->v = (DTYPE_t)DT_PLVAR;
        j->slen = 0;
        j->p = (void *)j;
        if (!plc_unify_cells_cx(j, (pl_cell_t *)val, cx)) return 0;
        pl_cell_t stored;
        stored.v = (DTYPE_t)DT_PLVAR;
        stored.slen = 0;
        stored.p = (void *)j;
        if (*cell) {
            char probe;
            char *floor_ = &probe;
            if (pl_tr_needs_log(cx, *cell, floor_) && !pl_tr_kept_since_choice(cx, *cell)) pl_tr_push_keep(cx, *cell);
            **cell = stored;
        } else {
            pl_cell_t *box = (pl_cell_t *)rt_ws_alloc_descr(1);
            if (!box) return 0;
            *box = pl_make_int(0);
            box->slen = PLC_GVAR_ABSENT;
            *cell = box;
            if (cx->b) pl_tr_push_keep(cx, box);
            *box = stored;
        }
        return 1;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_nb_get_cell(void *root, int64_t k, void *out_cell, pl_tr_ctx_t *cx) {
    if (!root || k < 0 || !out_cell) return 0;
    {
        pl_cell_t **cell = (pl_cell_t **)pl_db_cell_addr(root, k, 0);
        pl_cell_t *box = cell ? *cell : (pl_cell_t *)0;
        if (!box || plc_gvar_absent(box)) return 0;
        return plc_unify_cells_cx((pl_cell_t *)out_cell, box, cx) ? 1 : 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_nb_is_set(void *root, int64_t k) {
    if (!root || k < 0) return 0;
    { pl_cell_t **cell = (pl_cell_t **)pl_db_cell_addr(root, k, 0); return cell && *cell != (pl_cell_t *)0 && !plc_gvar_absent(*cell); }
}
void rt_pl_nb_unset(void *root, int64_t k) { pl_cell_t **cell = root && k >= 0 ? (pl_cell_t **)pl_db_cell_addr(root, k, 0) : (pl_cell_t **)0; if (cell) *cell = (pl_cell_t *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define PL_DB_REGISTRY_CELL 0
#define PL_DB_KEY_VLA(buf, nm, ar) char buf[fmt_len("%s/%d", (nm), (int)(ar))]; snprintf(buf, sizeof buf, "%s/%d", (nm), (int)(ar))
typedef struct { char *key; int k; int meta; pl_db_t *db; int stat; int decl; int file; int line; int mf; int lib; } pl_db_key_t;
typedef struct {
    pl_db_key_t *e;
    int n;
    int cap;
    int next_cell;
    int ovf_cap;
    void **ovf;
    void **tries;
    int ntries;
    int tcap;
    int tbl_vt;
    int *hix;
    int hcap;
    long *mtok;
    int mtn;
    int mtcap;
} pl_db_reg_t;
_Static_assert(__builtin_offsetof(pl_db_reg_t, hix) == 56, "rt_gc_mark_run (rtx_gc.s) visits the registry's key index at [registry + 56], an unscanned block of entry numbers");
_Static_assert(__builtin_offsetof(pl_db_reg_t, ovf) == 24,
    "xa_flat.cpp's packet road loads a root cell numbered PL_DB_FRAME_CELLS or more through [registry + 24], the overflow vector -- the cells past the root frame live there, in an HB_PVEC that grows"
    " by doubling");
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
_Static_assert(__builtin_offsetof(pl_db_t, s) == 0 && __builtin_offsetof(pl_db_slot_t, cl) == 0 && sizeof(pl_db_slot_t) == 40 && __builtin_offsetof(pl_db_reg_t, e) == 0 &&
    __builtin_offsetof(pl_db_reg_t, ovf) == 24 && __builtin_offsetof(pl_db_reg_t, tries) == 32 && __builtin_offsetof(pl_db_key_t, key) == 0 && __builtin_offsetof(pl_db_key_t, db) == 16 &&
    sizeof(pl_db_key_t) == 48 && __builtin_offsetof(pl_db_reg_t, hcap) == 64,
    "rt_gc_mark_run and rt_pl_db_reg_find_na (rtx_gc.s, rtx_plunify.s) read the Prolog db store, slots, registry and keys at these offsets");
static pl_db_reg_t * pl_db_registry(void *root, int create) {
    if (!root) return (pl_db_reg_t *)0;
    {
        pl_db_reg_t **cell = (pl_db_reg_t **)((char *)root - PL_DB_CELL0 - 8 * (size_t)PL_DB_REGISTRY_CELL);
        if (!*cell && create) {
            pl_db_reg_t *r = (pl_db_reg_t *)rt_pl_struct_alloc(HB_PLDBR, sizeof *r);
            if (!r) return (pl_db_reg_t *)0;
            r->cap = 32;
            r->n = 0;
            r->next_cell = 1;
            r->ovf_cap = 0;
            r->ovf = (void **)0;
            r->tries = (void **)0;
            r->ntries = 0;
            r->tcap = 0;
            r->tbl_vt = -1;
            r->hix = (int *)0;
            r->hcap = 0;
            r->mtok = (long *)0;
            r->mtn = 0;
            r->mtcap = 0;
            r->e = (pl_db_key_t *)rt_pl_struct_alloc(HB_PLDBK, (size_t)r->cap * sizeof(pl_db_key_t));
            if (!r->e) { r->cap = 0; }
            *cell = r;
        }
        return *cell;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void ** pl_db_cell_addr(void *root, int64_t k, int create) {
    extern void *rt_pvec_realloc(void *, size_t);
    if (!root || k < 0) return (void **)0;
    if (k < PL_DB_FRAME_CELLS) return (void **)((char *)root - PL_DB_CELL0 - 8 * (size_t)k);
    {
        pl_db_reg_t *r = pl_db_registry(root, create);
        int64_t i = k - PL_DB_FRAME_CELLS;
        if (!r) return (void **)0;
        if (i >= r->ovf_cap) {
            int64_t nc = r->ovf_cap > 0 ? r->ovf_cap : 16;
            void **v;
            if (!create) return (void **)0;
            while (nc <= i) nc *= 2;
            v = (void **)rt_pvec_realloc(r->ovf, (size_t)nc);
            if (!v) return (void **)0;
            r->ovf = v;
            r->ovf_cap = (int)nc;
        }
        return &r->ovf[i];
    }
}
static uint64_t pl_db_hash_bytes(uint64_t h, const char *s, size_t n) { for (size_t i = 0; i < n; i++) h = (h ^ (unsigned char)s[i]) * 0x100000001B3ull; return h; }
static void pl_db_hix_put(pl_db_reg_t *r, int idx) {
    const char *k = r->e[idx].key;
    int m = r->hcap - 1, i = (int)(pl_db_hash_bytes(0xCBF29CE484222325ull, k, strlen(k)) & (uint64_t)m);
    while (r->hix[i]) i = (i + 1) & m;
    r->hix[i] = idx + 1;
}
static void pl_db_hix_index(pl_db_reg_t *r) {
    if (r->hix && 2 * r->n <= r->hcap) { pl_db_hix_put(r, r->n - 1); return; }
    {
        int nc = r->hcap > 0 ? 2 * r->hcap : 64;
        int *nh = (int *)rt_str_alloc((long)nc * (long)sizeof(int));
        r->hix = nh;
        r->hcap = nh ? nc : 0;
        if (!nh) return;
        memset(nh, 0, (size_t)nc * sizeof(int));
        for (int i = 0; i < r->n; i++) pl_db_hix_put(r, i);
    }
}
extern pl_db_key_t * rt_pl_db_reg_find_na(pl_db_reg_t *r, const char *nm, int arity);
static pl_db_key_t * pl_db_reg_find(pl_db_reg_t *r, const char *key) {
    if (!r || !key) return (pl_db_key_t *)0;
    if (r->hix) {
        int m = r->hcap - 1;
        for (int i = (int)(pl_db_hash_bytes(0xCBF29CE484222325ull, key, strlen(key)) & (uint64_t)m), x; (x = r->hix[i]) != 0; i = (i + 1) & m) if (!strcmp(r->e[x - 1].key, key)) return &r->e[x - 1];
        return (pl_db_key_t *)0;
    }
    for (int i = 0; i < r->n; i++) if (!strcmp(r->e[i].key, key)) return &r->e[i];
    return (pl_db_key_t *)0;
}
static pl_db_key_t * pl_db_reg_add(pl_db_reg_t *r, const char *key) {
    if (!r || !key) return (pl_db_key_t *)0;
    if (r->n >= r->cap) {
        int nc = r->cap > 0 ? r->cap * 2 : 32;
        pl_db_key_t *ne = (pl_db_key_t *)rt_pl_struct_alloc(HB_PLDBK, (size_t)nc * sizeof(pl_db_key_t));
        if (!ne) return (pl_db_key_t *)0;
        for (int i = 0; i < r->n; i++) ne[i] = r->e[i];
        r->e = ne;
        r->cap = nc;
    }
    {
        char *ks = rt_str_dup(key);
        if (!ks) return (pl_db_key_t *)0;
        pl_db_key_t *e = &r->e[r->n++];
        e->key = ks;
        e->k = -1;
        e->meta = 0;
        e->db = (pl_db_t *)0;
        e->stat = 0;
        e->decl = 0;
        e->file = -1;
        e->line = 0;
        e->mf = 0;
        e->lib = 0;
        pl_db_hix_index(r);
        return &r->e[r->n - 1];
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct pl_trie_node_s { DESCR_t tok; DESCR_t val; struct pl_trie_node_s *parent; void **kids; int nkids; int kcap; int kind; int has_val; int ent; int depth; } pl_trie_node_t;
typedef struct { pl_trie_node_t *root; void **ents; int nents; int ecap; int nvals; int alive; DESCR_t status; DESCR_t data; } pl_trie_t;
typedef struct { plr_stk_t ag, lg; pl_cell_t *s; pl_trie_node_t *cur; int create; int nvar; int miss; int attv; } pl_trie_w_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void pl_trie_gc_visit(uint16_t type, void *p) {
    extern void rt_gc_visit_raw(const char **);
    extern void rt_gc_visit_descr(DESCR_t *);
    if (type == HB_TRIE) {
        pl_trie_t *t = (pl_trie_t *)p;
        if (t->root) rt_gc_visit_raw((const char **)&t->root);
        if (t->ents) rt_gc_visit_raw((const char **)&t->ents);
        rt_gc_visit_descr(&t->status);
        rt_gc_visit_descr(&t->data);
        return;
    }
    {
        pl_trie_node_t *n = (pl_trie_node_t *)p;
        rt_gc_visit_descr(&n->tok);
        rt_gc_visit_descr(&n->val);
        if (n->parent) rt_gc_visit_raw((const char **)&n->parent);
        if (n->kids) rt_gc_visit_raw((const char **)&n->kids);
    }
}
static pl_trie_node_t * pl_trie_node_new(pl_trie_node_t *parent, int kind, DESCR_t tok) {
    pl_trie_node_t *n = (pl_trie_node_t *)rt_pl_struct_alloc(HB_TRIEN, sizeof *n);
    if (!n) return (pl_trie_node_t *)0;
    memset(n, 0, sizeof *n);
    n->tok = tok;
    n->kind = kind;
    n->parent = parent;
    n->ent = -1;
    n->depth = parent ? parent->depth + 1 : 0;
    return n;
}
static pl_trie_t * pl_trie_at(void *root, int64_t id) {
    pl_db_reg_t *r = pl_db_registry(root, 0);
    if (!r || !r->tries || id < 0 || id >= r->ntries) return (pl_trie_t *)0;
    { pl_trie_t *t = (pl_trie_t *)r->tries[id]; return (t && t->alive) ? t : (pl_trie_t *)0; }
}
int rt_pl_trie_new(void *root) {
    pl_db_reg_t *r = pl_db_registry(root, 1);
    if (!r) return -1;
    if (r->ntries >= r->tcap) { int nc = r->tcap > 0 ? 2 * r->tcap : 8; void **nv = (void **)rt_pvec_realloc(r->tries, (size_t)nc); if (!nv) return -1; r->tries = nv; r->tcap = nc; }
    {
        pl_trie_t *t = (pl_trie_t *)rt_pl_struct_alloc(HB_TRIE, sizeof *t);
        DESCR_t none;
        if (!t) return -1;
        memset(t, 0, sizeof *t);
        memset(&none, 0, sizeof none);
        t->root = pl_trie_node_new((pl_trie_node_t *)0, 0, none);
        if (!t->root) return -1;
        t->alive = 1;
        r->tries[r->ntries] = t;
        return r->ntries++;
    }
}
int rt_pl_trie_alive(void *root, int64_t id) { return pl_trie_at(root, id) ? 1 : 0; }
int rt_pl_trie_destroy(void *root, int64_t id) {
    pl_db_reg_t *r = pl_db_registry(root, 0);
    pl_trie_t *t = pl_trie_at(root, id);
    if (!t) return 0;
    t->alive = 0;
    r->tries[id] = (void *)0;
    if (r->tbl_vt == id) r->tbl_vt = -1;
    return 1;
}
static int pl_trie_tok_eq(int ka, const DESCR_t *a, int kb, const DESCR_t *b) {
    if (ka != kb) return 0;
    if (ka != 1) return a->i == b->i;
    if (a->v != b->v) return 0;
    if ((int)a->v == DT_PLATOM || (int)a->v == DT_I) return a->i == b->i;
    if ((int)a->v == DT_R) return !memcmp(&a->r, &b->r, sizeof a->r);
    return rt_pl_cell_compare((pl_cell_t *)a, (pl_cell_t *)b) == 0;
}
#define PL_TRIE_LINEAR_KIDS 8
static uint64_t pl_trie_tok_hash(int kind, const DESCR_t *t) {
    uint64_t h = (uint64_t)kind * 0x9E3779B97F4A7C15ull;
    if (kind != 1 || (int)t->v == DT_PLATOM || (int)t->v == DT_I) h ^= (uint64_t)t->i;
    else if ((int)t->v == DT_R) { uint64_t b; memcpy(&b, &t->r, sizeof b); h ^= b; }
    if (kind == 1) h ^= (uint64_t)(uint32_t)t->v << 40;
    h *= 0xFF51AFD7ED558CCDull;
    return h ^ (h >> 29);
}
static int pl_trie_kid_slot(void **kids, int cap, int kind, const DESCR_t *tok) {
    int m = cap - 1, i = (int)(pl_trie_tok_hash(kind, tok) & (uint64_t)m);
    for (pl_trie_node_t *c; (c = (pl_trie_node_t *)kids[i]) != 0; i = (i + 1) & m) if (pl_trie_tok_eq(c->kind, &c->tok, kind, tok)) return i;
    return i;
}
static int pl_trie_kids_rehash(pl_trie_node_t *n, int nc) {
    void **nv = (void **)rt_pvec_alloc((size_t)nc);
    if (!nv) return 0;
    if (n->kcap > PL_TRIE_LINEAR_KIDS) {
        for (int i = 0; i < n->kcap; i++) if (n->kids[i]) { pl_trie_node_t *c = (pl_trie_node_t *)n->kids[i]; nv[pl_trie_kid_slot(nv, nc, c->kind, &c->tok)] = c; }
    } else {
        for (int i = 0; i < n->nkids; i++) { pl_trie_node_t *c = (pl_trie_node_t *)n->kids[i]; nv[pl_trie_kid_slot(nv, nc, c->kind, &c->tok)] = c; }
    }
    n->kids = nv;
    n->kcap = nc;
    return 1;
}
static pl_trie_node_t * pl_trie_child(pl_trie_node_t *n, int kind, const DESCR_t *tok, int create) {
    if (n->kcap > PL_TRIE_LINEAR_KIDS) {
        int s = pl_trie_kid_slot(n->kids, n->kcap, kind, tok);
        pl_trie_node_t *c = (pl_trie_node_t *)n->kids[s];
        if (c || !create) return c;
        if (2 * (n->nkids + 1) > n->kcap) { if (!pl_trie_kids_rehash(n, 2 * n->kcap)) return (pl_trie_node_t *)0; s = pl_trie_kid_slot(n->kids, n->kcap, kind, tok); }
        if (!(c = pl_trie_node_new(n, kind, *tok))) return (pl_trie_node_t *)0;
        n->kids[s] = (void *)c;
        n->nkids++;
        return c;
    }
    for (int i = 0; i < n->nkids; i++) { pl_trie_node_t *c = (pl_trie_node_t *)n->kids[i]; if (pl_trie_tok_eq(c->kind, &c->tok, kind, tok)) return c; }
    if (!create) return (pl_trie_node_t *)0;
    if (n->nkids >= PL_TRIE_LINEAR_KIDS) return pl_trie_kids_rehash(n, 4 * PL_TRIE_LINEAR_KIDS) ? pl_trie_child(n, kind, tok, create) : (pl_trie_node_t *)0;
    if (n->nkids >= n->kcap) { int nc = n->kcap > 0 ? 2 * n->kcap : 4; void **nv = (void **)rt_pvec_realloc(n->kids, (size_t)nc); if (!nv) return (pl_trie_node_t *)0; n->kids = nv; n->kcap = nc; }
    { pl_trie_node_t *c = pl_trie_node_new(n, kind, *tok); if (!c) return (pl_trie_node_t *)0; n->kids[n->nkids++] = (void *)c; return c; }
}
static int pl_trie_walk_run(void *wv) {
    pl_trie_w_t *w = (pl_trie_w_t *)wv;
    for (;;) {
        pl_cell_t *d = pl_deref(w->s);
        DESCR_t tok;
        int kind;
        if (d->v == PLR_VCOPY) {
            kind = 3;
            tok = pl_make_int((int64_t)(intptr_t)d->p);
        } else if (pl_cell_unbound(d)) {
            uint64_t w0;
            if (pl_attv_is(d)) { w->attv = 1; w->miss = 1; }
            if (!plr_room(&w->lg)) return plr_more(&w->ag, &w->lg, pl_trie_walk_run, w);
            memcpy(&w0, d, 8);
            plr_push(&w->lg, d, d->p, w0);
            kind = 3;
            tok = pl_make_int(w->nvar);
            d->v = PLR_VCOPY;
            d->p = (void *)(intptr_t)w->nvar;
            w->nvar++;
        } else if ((int)d->v == DT_PLREF) {
            int ar = plc_fid_arity(d->slen);
            if (ar > 0 && !plr_room(&w->ag)) return plr_more(&w->ag, &w->lg, pl_trie_walk_run, w);
            kind = 2;
            tok = pl_make_int((int64_t)d->slen);
            if (ar > 0) plr_push(&w->ag, d->p, (void *)0, (uint64_t)ar);
        } else {
            kind = 1;
            tok = *d;
        }
        if (!w->miss) { pl_trie_node_t *c = pl_trie_child(w->cur, kind, &tok, w->create); if (c) w->cur = c; else w->miss = 1; }
        if (plr_empty(&w->ag)) break;
        { plr_ent_t *t = plr_peek(&w->ag); w->s = (pl_cell_t *)t->x; t->x = (void *)(w->s + 1); if (!--t->n) plr_drop(&w->ag); }
    }
    for (plr_seg_t *g = w->lg.top; g; g = g->prev) for (long i = g->n; i-- > 0; ) { pl_cell_t *c = (pl_cell_t *)g->e[i].x; uint64_t w0 = g->e[i].n; memcpy(c, &w0, 8); c->p = g->e[i].y; }
    return 0;
}
static pl_trie_node_t * pl_trie_descend(pl_trie_t *t, pl_cell_t *key, int create, int *attv) {
    pl_trie_w_t w;
    plr_stk_init(&w.ag);
    plr_stk_init(&w.lg);
    w.s = key;
    w.cur = t->root;
    w.create = create;
    w.nvar = 0;
    w.miss = 0;
    w.attv = 0;
    (void)pl_trie_walk_run(&w);
    if (attv) *attv = w.attv;
    return w.miss ? (pl_trie_node_t *)0 : w.cur;
}
static pl_cell_t pl_trie_key_term(pl_trie_node_t *n) {
    int d = n->depth, nv = 0, sp = 0;
    pl_trie_node_t *path[d > 0 ? d : 1];
    pl_cell_t *slots[d > 0 ? d : 1];
    pl_cell_t out;
    memset(&out, 0, sizeof out);
    for (pl_trie_node_t *c = n; c && c->kind; c = c->parent) { path[c->depth - 1] = c; if (c->kind == 3 && (int)c->tok.i + 1 > nv) nv = (int)c->tok.i + 1; }
    {
        pl_cell_t *vars[nv > 0 ? nv : 1];
        for (int i = 0; i < nv; i++) vars[i] = (pl_cell_t *)0;
        slots[sp++] = &out;
        for (int i = 0; i < d && sp > 0; i++) {
            pl_trie_node_t *c = path[i];
            pl_cell_t *dst = slots[--sp];
            if (c->kind == 2) {
                int ar = plc_fid_arity((uint32_t)c->tok.i);
                pl_cell_t *aa = ar > 0 ? (pl_cell_t *)rt_ws_alloc_descr((size_t)ar) : (pl_cell_t *)0;
                dst->v = (DTYPE_t)DT_PLREF;
                dst->slen = (uint32_t)c->tok.i;
                dst->p = (void *)aa;
                for (int k = ar - 1; k >= 0; k--) slots[sp++] = &aa[k];
            } else if (c->kind == 3) {
                int k = (int)c->tok.i;
                if (!vars[k]) { vars[k] = (pl_cell_t *)rt_ws_alloc_descr(1); pl_init_var(vars[k], -1); }
                *dst = pl_make_ref(vars[k], (int)vars[k]->slen);
            } else *dst = c->tok;
        }
    }
    return out;
}
int rt_pl_trie_insert(void *root, int64_t id, void *key, void *val, int update) {
    pl_trie_t *t = pl_trie_at(root, id);
    pl_trie_node_t *n;
    if (!t) return -1;
    { int av = 0; n = pl_trie_descend(t, (pl_cell_t *)key, 1, &av); if (av) return -4; }
    if (!n) return -1;
    if (n->has_val && !update) return rt_pl_cell_compare(pl_deref((pl_cell_t *)val), &n->val) == 0 ? -2 : -3;
    if (n->ent < 0) {
        if (t->nents >= t->ecap) { int nc = t->ecap > 0 ? 2 * t->ecap : 8; void **nv = (void **)rt_pvec_realloc(t->ents, (size_t)nc); if (!nv) return -1; t->ents = nv; t->ecap = nc; }
        n->ent = t->nents;
        t->ents[t->nents++] = (void *)n;
    }
    if (!n->has_val) t->nvals++;
    n->val = plc_copy(pl_deref((pl_cell_t *)val));
    n->has_val = 1;
    return n->ent;
}
static int pl_trie_give(pl_cell_t src, void *out_cell, pl_tr_ctx_t *cx) {
    pl_cell_t *box = (pl_cell_t *)rt_ws_alloc_descr(1);
    if (!box) return 0;
    *box = src;
    return plc_unify_cells_cx((pl_cell_t *)out_cell, box, cx) ? 1 : 0;
}
int rt_pl_trie_lookup(void *root, int64_t id, void *key, void *val_cell, pl_tr_ctx_t *cx) {
    pl_trie_t *t = pl_trie_at(root, id);
    pl_trie_node_t *n = t ? pl_trie_descend(t, (pl_cell_t *)key, 0, (int *)0) : (pl_trie_node_t *)0;
    if (!n || !n->has_val) return 0;
    return pl_trie_give(plc_copy(&n->val), val_cell, cx);
}
int rt_pl_trie_delete(void *root, int64_t id, void *key, void *val_cell, pl_tr_ctx_t *cx) {
    pl_trie_t *t = pl_trie_at(root, id);
    pl_trie_node_t *n = t ? pl_trie_descend(t, (pl_cell_t *)key, 0, (int *)0) : (pl_trie_node_t *)0;
    pl_cell_t v;
    if (!n || !n->has_val) return 0;
    v = n->val;
    n->has_val = 0;
    memset(&n->val, 0, sizeof n->val);
    if (n->ent >= 0) { t->ents[n->ent] = (void *)0; n->ent = -1; }
    t->nvals--;
    return pl_trie_give(v, val_cell, cx);
}
static int pl_trie_ents_list(pl_trie_t *t, int64_t from, int64_t hi, pl_cell_t *out) {
    pl_cell_t acc = plc_nil_cell();
    int dot_id = prolog_atom_intern("."), e_id = prolog_atom_intern("e");
    for (int64_t i = hi - 1; i >= from; i--) {
        pl_trie_node_t *n = (pl_trie_node_t *)t->ents[i];
        pl_cell_t *blk;
        if (!n || !n->has_val) continue;
        blk = (pl_cell_t *)rt_ws_alloc_descr(3);
        if (!blk) return 0;
        blk[0] = pl_make_int(i);
        blk[1] = pl_trie_key_term(n);
        blk[2] = plc_copy(&n->val);
        acc = plc_cons(dot_id, pl_make_compound(e_id, 3, blk), acc);
    }
    *out = acc;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_trie_entries(void *root, int64_t id, void *out_cell, pl_tr_ctx_t *cx) {
    pl_trie_t *t = pl_trie_at(root, id);
    pl_cell_t acc;
    if (!t || !pl_trie_ents_list(t, 0, t->nents, &acc)) return 0;
    return pl_trie_give(acc, out_cell, cx);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_trie_slice(void *root, int64_t id, int64_t from, int64_t to, void *out_cell, pl_tr_ctx_t *cx) {
    pl_trie_t *t = pl_trie_at(root, id);
    pl_cell_t acc, *pr;
    if (!t || !pl_trie_ents_list(t, from < 0 ? 0 : from, (to < 0 || to > t->nents) ? t->nents : to, &acc)) return 0;
    pr = (pl_cell_t *)rt_ws_alloc_descr(2);
    if (!pr) return 0;
    pr[0] = pl_make_int(t->nents);
    pr[1] = acc;
    return pl_trie_give(pl_make_compound(prolog_atom_intern("-"), 2, pr), out_cell, cx);
}
int rt_pl_trie_entry_key(void *root, int64_t id, int64_t ent, void *out_cell, pl_tr_ctx_t *cx) {
    pl_trie_t *t = pl_trie_at(root, id);
    pl_trie_node_t *n;
    if (!t || ent < 0 || ent >= t->nents) return 0;
    n = (pl_trie_node_t *)t->ents[ent];
    if (!n || !n->has_val) return 0;
    return pl_trie_give(pl_trie_key_term(n), out_cell, cx);
}
int rt_pl_trie_count(void *root, int64_t id) { pl_trie_t *t = pl_trie_at(root, id); return t ? t->nvals : -1; }
int rt_pl_trie_meta_set(void *root, int64_t id, int which, void *cell) {
    pl_trie_t *t = pl_trie_at(root, id);
    if (!t) return 0;
    if (which) t->data = plc_copy(pl_deref((pl_cell_t *)cell));
    else t->status = plc_copy(pl_deref((pl_cell_t *)cell));
    return 1;
}
int rt_pl_trie_meta_get(void *root, int64_t id, int which, void *out_cell, pl_tr_ctx_t *cx) {
    pl_trie_t *t = pl_trie_at(root, id);
    pl_cell_t *src;
    if (!t) return 0;
    src = which ? &t->data : &t->status;
    if (src->v == DT_SNUL) return 0;
    return pl_trie_give(plc_copy(src), out_cell, cx);
}
int rt_pl_trie_tbl_vt(void *root) {
    pl_db_reg_t *r = pl_db_registry(root, 1);
    if (!r) return -1;
    if (r->tbl_vt < 0 || !pl_trie_at(root, r->tbl_vt)) r->tbl_vt = rt_pl_trie_new(root);
    return r->tbl_vt;
}
int rt_pl_db_bind(void *root, int64_t k, const char *name, int64_t arity) {
    if (!root || !name || k <= PL_DB_REGISTRY_CELL) return 0;
    PL_DB_KEY_VLA(key, name, arity);
    {
        pl_db_reg_t *r = pl_db_registry(root, 1);
        pl_db_key_t *e = pl_db_reg_find(r, key);
        if (!e) e = pl_db_reg_add(r, key);
        if (!e || !pl_db_cell_addr(root, k, 1)) return 0;
        e->k = (int)k;
        if ((int)k + 1 > r->next_cell) r->next_cell = (int)k + 1;
        return 1;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_key_cell(void *root, const char *key) { pl_db_key_t *e = pl_db_reg_find(pl_db_registry(root, 0), key); return (e && e->k >= 0) ? e->k : -1; }
int rt_pl_db_key_is_dynamic(void *root, const char *key);
int rt_pl_db_cell_for(void *root, const char *name, int arity) {
    { pl_db_reg_t *r = name ? pl_db_registry(root, 0) : (pl_db_reg_t *)0; pl_db_key_t *e = (r && r->hix) ? rt_pl_db_reg_find_na(r, name, arity) : (pl_db_key_t *)0; if (e && e->k >= 0) return e->k; }
    PL_DB_KEY_VLA(key, name, arity);
    {
        pl_db_reg_t *r = pl_db_registry(root, 1);
        pl_db_key_t *e = r ? pl_db_reg_find(r, key) : (pl_db_key_t *)0;
        if (e && e->k >= 0) return e->k;
        if (!r) return -1;
        if (r->next_cell <= PL_DB_REGISTRY_CELL) r->next_cell = PL_DB_REGISTRY_CELL + 1;
        if (!e) e = pl_db_reg_add(r, key);
        if (!e || !pl_db_cell_addr(root, r->next_cell, 1)) return -1;
        e->k = r->next_cell++;
        return e->k;
    }
}
int rt_pl_db_key_is_dynamic_na(void *root, const char *name, int arity) { PL_DB_KEY_VLA(key, name, arity); return rt_pl_db_key_is_dynamic(root, key); }
int rt_pl_db_key_cell_na(void *root, const char *name, int arity) { PL_DB_KEY_VLA(key, name, arity); return rt_pl_db_key_cell(root, key); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t * pl_tbh_cur(void *root) {
    int64_t k = rt_pl_db_cell_for(root, "$tbl_cur", -1);
    pl_cell_t **cell = k >= 0 ? (pl_cell_t **)pl_db_cell_addr(root, k, 0) : (pl_cell_t **)0;
    pl_cell_t *v = (cell && *cell && !plc_gvar_absent(*cell)) ? pl_deref(*cell) : (pl_cell_t *)0;
    return (v && (int)v->v == DT_I && v->i) ? (DESCR_t *)(uintptr_t)v->i : (DESCR_t *)0;
}
static DESCR_t * pl_tbh_fwd(DESCR_t *h, int f) { return h ? ((int)h[f].v == DT_RAW ? (DESCR_t *)(uintptr_t)h[f].i : &h[f]) : (DESCR_t *)0; }
static DESCR_t * pl_tbh_owner(void *root, int f) { return pl_tbh_fwd(pl_tbh_cur(root), f); }
int rt_pl_tbh_open(void *root, void *arg, pl_tr_ctx_t *cx) {
    DESCR_t *a = (DESCR_t *)arg;
    if (!root || !a || (int)a->v != DT_I || !a->i) return 0;
    {
        DESCR_t *h = (DESCR_t *)(uintptr_t)a->i;
        DESCR_t *prev = pl_tbh_cur(root);
        int64_t k = rt_pl_db_cell_for(root, "$tbl_cur", -1);
        h[0] = (DESCR_t){ .v = DT_I, .i = (int64_t)(uintptr_t)prev };
        for (int f = 1; f < PL_TBH_CELLS; f++) h[f] = (DESCR_t){ .v = DT_RAW, .i = (int64_t)(uintptr_t)pl_tbh_fwd(prev, f) };
        h[PL_TBH_CELLS + 1] = (DESCR_t){ .v = DT_I, .i = (int64_t)(uintptr_t)cx->tr };
        return k >= 0 && rt_pl_b_set(root, k, arg, cx);
    }
}
int rt_pl_tbh_close(void *root, pl_tr_ctx_t *cx) {
    DESCR_t *h = pl_tbh_cur(root);
    int64_t k = rt_pl_db_cell_for(root, "$tbl_cur", -1);
    if (!h || k < 0) return 0;
    { DESCR_t prev = (DESCR_t){ .v = DT_I, .i = h[0].i }; return rt_pl_b_set(root, k, (void *)&prev, cx); }
}
int rt_pl_tbh_leading(void *root) { return pl_tbh_cur(root) != (DESCR_t *)0; }
DESCR_t * rt_pl_tbh_current(void *root) { return pl_tbh_cur(root); }
int rt_pl_tbh_get(void *root, int64_t f, void *out, pl_tr_ctx_t *cx) {
    DESCR_t *c = (f >= 1 && f < PL_TBH_CELLS) ? pl_tbh_owner(root, (int)f) : (DESCR_t *)0;
    return c && plc_unify_cells_cx((pl_cell_t *)out, (pl_cell_t *)c, cx) ? 1 : 0;
}
int rt_pl_tbl_dl(void *root, int set, void *val, pl_tr_ctx_t *cx) {
    int64_t k = rt_pl_db_cell_for(root, "$tbl_dl", -1);
    return k < 0 ? 0 : set ? rt_pl_b_set(root, k, val, cx) : rt_pl_nb_get_cell(root, k, val, cx);
}
int rt_pl_tbh_put(void *root, int64_t f, int mode, void *val, pl_tr_ctx_t *cx) {
    DESCR_t *h = pl_tbh_cur(root);
    if (f < 1 || f >= PL_TBH_CELLS) return 0;
    if (!h) return mode != 0;
    {
        DESCR_t *c = mode == 0 ? &h[f] : pl_tbh_owner(root, (int)f);
        if (!c) return 1;
        { pl_cell_t v = plc_copy(pl_deref((pl_cell_t *)val)); char probe; if (mode == 2 && pl_tr_needs_log(cx, c, &probe)) pl_tr_push(cx, c); *(pl_cell_t *)c = v; return 1; }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_term_cell(void *root, void *term_cell) {
    extern const char *prolog_atom_name(int);
    extern int prolog_atom_intern(const char *);
    pl_cell_t *t = pl_deref((pl_cell_t *)term_cell);
    pl_cell_t *h = t;
    const char *nm;
    int ar = 0;
    if ((int)t->v == DT_PLREF && pl_arity(t) == 2 && plc_functor(t) == prolog_atom_intern(":-")) h = pl_deref(&((pl_cell_t *)t->p)[0]);
    if ((int)h->v == DT_PLREF) { nm = prolog_atom_name(plc_functor(h)); ar = pl_arity(h); } else if ((int)h->v == DT_PLATOM) nm = prolog_atom_name((int)h->i);
    else if ((int)h->v == DT_S) nm = h->s;
    else return -1;
    if (!nm) return -1;
    { PL_DB_KEY_VLA(key, nm, ar); return rt_pl_db_key_cell(root, key); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_frame_cells(void) { return PL_DB_FRAME_CELLS; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_decl(void *root, const char *name, int64_t arity, int64_t kind);
static void pl_db_key_set_file(void *root, const char *name, int64_t arity, int64_t file) {
    PL_DB_KEY_VLA(key, name, arity);
    pl_db_key_t *e = pl_db_reg_find(pl_db_registry(root, 0), key);
    if (e) e->file = (int)file;
}
static void pl_db_key_set_meta(void *root, const char *name, int64_t arity, int64_t kind, int64_t v) {
    PL_DB_KEY_VLA(key, name, arity);
    pl_db_key_t *e = pl_db_reg_find(pl_db_registry(root, 0), key);
    if (e && kind == 5) e->line = (int)v;
    if (e && kind == 6) e->mf = 1;
}
static void pl_db_key_put_meta(void *root, const char *key, const long *ids, int n) {
    extern void *ct_grow(void *, size_t);
    pl_db_reg_t *r = pl_db_registry(root, 0);
    pl_db_key_t *e = pl_db_reg_find(r, key);
    if (!e || n <= 0) return;
    if (r->mtn + n > r->mtcap) { int nc = r->mtcap ? r->mtcap : 64; while (nc < r->mtn + n) nc *= 2; r->mtok = (long *)ct_grow((void *)r->mtok, (size_t)nc * sizeof(long)); r->mtcap = nc; }
    e = pl_db_reg_find(r, key);
    for (int i = 0; i < n; i++) r->mtok[r->mtn + i] = ids[i];
    e->meta = r->mtn + 1;
    r->mtn += n;
}
int rt_pl_db_key_meta(void *root, const char *key, const long **ids) {
    pl_db_reg_t *r = pl_db_registry(root, 0);
    pl_db_key_t *e = pl_db_reg_find(r, key);
    const char *sl = strrchr(key, '/');
    if (!e || e->meta <= 0 || !sl) return 0;
    *ids = r->mtok + e->meta - 1;
    return (int)strtol(sl + 1, (char **)0, 10);
}
void rt_pl_db_meta_decl(void *root, const char *name, int64_t arity, const char *const *toks) {
    extern int prolog_atom_intern(const char *);
    long ids[arity > 0 ? arity : 1];
    PL_DB_KEY_VLA(key, name, arity);
    if (!root || !name || arity <= 0) return;
    if (!pl_db_reg_find(pl_db_registry(root, 1), key)) (void)rt_pl_db_decl(root, name, arity, 7);
    for (int64_t i = 0; i < arity; i++) ids[i] = prolog_atom_intern(toks[i]);
    pl_db_key_put_meta(root, key, ids, (int)arity);
}
int rt_pl_db_key_line(void *root, const char *key) { pl_db_key_t *e = pl_db_reg_find(pl_db_registry(root, 0), key); return e ? e->line : 0; }
int rt_pl_db_key_mf(void *root, const char *key) { pl_db_key_t *e = pl_db_reg_find(pl_db_registry(root, 0), key); return e ? e->mf : 0; }
int rt_pl_db_cells_base(void *root, int64_t n);
void rt_pl_db_decls_install(const long *tab, void *root) {
    extern const char *prolog_atom_name(int);
    long cells;
    long n;
    const long *e;
    if (!tab || !root) return;
    n = tab[0] / 4;
    cells = tab[1];
    e = tab + 2;
    rt_pl_db_cells_base(root, cells);
    for (long i = 0; i < n; i++) {
        const char *nm = prolog_atom_name((int)e[4 * i]);
        if (nm && e[4 * i + 2] != 4 && e[4 * i + 2] < 5 && e[4 * i + 3] >= 0) rt_pl_db_bind(root, e[4 * i + 3], nm, e[4 * i + 1]);
    }
    for (long i = 0; i < n; i++) {
        const char *nm = prolog_atom_name((int)e[4 * i]);
        if (nm && e[4 * i + 2] == 4) pl_db_key_set_file(root, nm, e[4 * i + 1], e[4 * i + 3]);
        else if (nm && e[4 * i + 2] == 8) continue;
        else if (nm && e[4 * i + 2] >= 5 && e[4 * i + 2] != 7) pl_db_key_set_meta(root, nm, e[4 * i + 1], e[4 * i + 2], e[4 * i + 3]);
        else if (nm && e[4 * i + 2] > 0) rt_pl_db_decl(root, nm, e[4 * i + 1], e[4 * i + 2]);
    }
    for (long i = 0; i < n; i++) {
        const char *nm = prolog_atom_name((int)e[4 * i]);
        long ar = e[4 * i + 1], j = 0;
        if (!nm || e[4 * i + 2] != 8 || ar <= 0 || i + ar > n) continue;
        {
            long ids[ar];
            PL_DB_KEY_VLA(key, nm, ar);
            for (j = 0; j < ar && e[4 * (i + j) + 2] == 8 && e[4 * (i + j)] == e[4 * i] && e[4 * (i + j) + 1] == ar; j++) ids[j] = e[4 * (i + j) + 3];
            if (j == ar) { if (!pl_db_reg_find(pl_db_registry(root, 1), key)) (void)rt_pl_db_decl(root, nm, ar, 7); pl_db_key_put_meta(root, key, ids, (int)ar); }
            i += j - 1;
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_cells_base(void *root, int64_t n) { pl_db_reg_t *r = pl_db_registry(root, 1); if (!r) return 0; if ((int)n > r->next_cell) r->next_cell = (int)n; return 1; }
int rt_pl_db_key_is_dynamic(void *root, const char *key) { pl_db_key_t *e = pl_db_reg_find(pl_db_registry(root, 0), key); return (e && !e->stat && !e->lib) ? 1 : 0; }
int rt_pl_db_key_is_static(void *root, const char *key) { pl_db_key_t *e = pl_db_reg_find(pl_db_registry(root, 0), key); return (e && e->stat) ? 1 : 0; }
int rt_pl_db_key_lib_take(void *root, const char *key) { pl_db_key_t *e = pl_db_reg_find(pl_db_registry(root, 0), key); if (!e || !e->lib) return 0; e->lib = 0; return 1; }
int rt_pl_db_key_is_declared(void *root, const char *key) { pl_db_key_t *e = pl_db_reg_find(pl_db_registry(root, 0), key); return (e && e->decl) ? 1 : 0; }
static int pl_db_key_current(void *root, pl_db_key_t *e);
const char *rt_pl_db_key_file(void *root, const char *key) {
    extern const char *prolog_atom_name(int);
    pl_db_key_t *e = pl_db_reg_find(pl_db_registry(root, 0), key);
    return (e && e->stat && e->file >= 0 && pl_db_key_current(root, e)) ? prolog_atom_name(e->file) : (const char *)0;
}
int rt_pl_db_key_kind(void *root, const char *key) { pl_db_key_t *e = pl_db_reg_find(pl_db_registry(root, 0), key); if (!e || !pl_db_key_current(root, e)) return 0; return e->stat ? 1 : 2; }
int rt_pl_db_decl(void *root, const char *name, int64_t arity, int64_t kind) {
    if (!root || !name) return 0;
    PL_DB_KEY_VLA(key, name, arity);
    {
        pl_db_reg_t *r = pl_db_registry(root, 1);
        pl_db_key_t *e = pl_db_reg_find(r, key);
        if (!e) e = pl_db_reg_add(r, key);
        if (!e) return 0;
        if (kind == 1) { if (e->k < 0 && !e->db) e->stat = 1; } else if (kind == 7) { if (e->k < 0 && !e->db) e->lib = 1; } else if (kind == 3) e->stat = 1;
        else e->decl = 1;
        return 1;
    }
}
static pl_db_t * pl_db_cell_peek(void *root, int k) { if (!root || k < 0) return (pl_db_t *)0; { pl_db_t **cell = (pl_db_t **)pl_db_cell_addr(root, k, 0); return cell ? *cell : (pl_db_t *)0; } }
static int pl_db_key_current(void *root, pl_db_key_t *e) {
    pl_db_t *db;
    if (!strncmp(e->key, "$pl_", 4) || !strncmp(e->key, "$db_", 4)) return 0;
    if (e->lib) return 0;
    if (e->stat) return 1;
    db = (e->k >= 0) ? pl_db_cell_peek(root, e->k) : e->db;
    if (db) return db->killed ? 0 : 1;
    return e->decl ? 1 : 0;
}
int rt_pl_db_cp_count(void *root) { pl_db_reg_t *r = pl_db_registry(root, 0); int c = 0; if (!r) return 0; for (int i = 0; i < r->n; i++) if (pl_db_key_current(root, &r->e[i])) c++; return c; }
const char * rt_pl_db_cp_nth(void *root, int64_t i, int *arity) {
    pl_db_reg_t *r = pl_db_registry(root, 0);
    int c = 0;
    if (!r || i < 1) return (const char *)0;
    for (int j = 0; j < r->n; j++) if (pl_db_key_current(root, &r->e[j]) && ++c == (int)i) { const char *sl = strrchr(r->e[j].key, '/'); *arity = sl ? atoi(sl + 1) : 0; return r->e[j].key; }
    return (const char *)0;
}
void * rt_pl_db_get_by_key(void *root, const char *key, int create) {
    pl_db_reg_t *r = pl_db_registry(root, create);
    pl_db_key_t *e = pl_db_reg_find(r, key);
    if (e && e->k >= 0) return rt_pl_db_get(root, e->k);
    if (!create) return (void *)0;
    if (!e) e = pl_db_reg_add(r, key);
    if (!e) return (void *)0;
    e->k = r->next_cell++;
    return rt_pl_db_get(root, e->k);
}
int rt_pl_db_term_key(void *term_cell, char *out, size_t n, int *ar) {
    extern const char *prolog_atom_name(int);
    extern int prolog_atom_intern(const char *);
    pl_cell_t *t = pl_deref((pl_cell_t *)term_cell);
    pl_cell_t *h = t;
    if ((int)t->v == DT_PLREF && pl_arity(t) == 2 && plc_functor(t) == prolog_atom_intern(":-")) h = pl_deref(&((pl_cell_t *)t->p)[0]);
    {
        const char *nm;
        if ((int)h->v == DT_PLREF) {
            nm = prolog_atom_name(plc_functor(h));
            *ar = pl_arity(h);
        } else if ((int)h->v == DT_PLATOM) {
            nm = prolog_atom_name((int)h->i);
            *ar = 0;
        } else if ((int)h->v == DT_S) {
            nm = h->s;
            *ar = 0;
        } else return 0;
        if (!nm) return 0;
        snprintf(out, n, "%s/%d", nm, *ar);
        return 1;
    }
}
static const char *pl_db_head_name(void *pair_cell, int *ar);
static int pl_db_frag_pack(size_t m0, size_t m1) {
    long ps = bb_pool_page();
    size_t lo, hi;
    if (ps <= 0) return 0;
    lo = (m0 + (size_t)ps - 1) / (size_t)ps;
    hi = (m1 + (size_t)ps - 1) / (size_t)ps;
    if (hi <= lo || lo >= ((size_t)1 << 20) || hi - lo >= ((size_t)1 << 10)) return 0;
    return (int)(lo | ((hi - lo) << 20));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *pl_db_frag_lo(int f) { const uint8_t *b = bb_pool_base(); return (PL_DB_FRAG_REC(f) && b) ? (const char *)b + (size_t)(f & 0xFFFFF) * (size_t)bb_pool_page() : (const char *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *pl_db_frag_hi(int f) { const char *lo = pl_db_frag_lo(f); return lo ? lo + (size_t)PL_DB_FRAG_PAGES(f) * (size_t)bb_pool_page() : (const char *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_db_frag_record(pl_db_t *db, int i, const void *fn, size_t m0) {
    const uint8_t *b = bb_pool_base();
    size_t at = (b && fn) ? (size_t)((const uint8_t *)fn - b) : 0;
    int f = (b && fn && (const uint8_t *)fn >= b && at >= m0) ? pl_db_frag_pack(at, bb_pool_mark()) : 0;
    db->s[i].frag = f;
    if (!f) { db->frag_top = -2; return; }
    if (db->frag_top == -2) return;
    db->frag_top = ((f & 0xFFFFF) > db->frag_top) ? (f & 0xFFFFF) : -2;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_db_fragment(void *root, pl_db_t *db, int i) {
    extern void * pl_runtime_define_fragment(const char *, void *, int, int, int, int *, int *, void *);
    extern int pl_runtime_install_packet_entry(const char *, int, const char *);
    int ar = 0;
    int ridx = -1;
    int off = 0;
    const char *nm;
    if (!db || i < 0 || i >= db->n) return 0;
    nm = pl_db_head_name((void *)&db->s[i].cl, &ar);
    if (!nm) return 0;
    {
        PL_DB_KEY_VLA(key, nm, ar);
        {
            char fkey[fmt_len("%s@%d", key, i)];
            size_t m0 = bb_pool_mark();
            void *fn;
            snprintf(fkey, sizeof fkey, "%s@%d", key, i);
            fn = pl_runtime_define_fragment(fkey, (void *)&db->s[i].cl, ar, db->cell_k, i, &ridx, &off, root);
            if (!fn) return 0;
            pl_db_frag_record(db, i, fn, m0);
        }
        db->s[i].ridx = ridx;
        db->s[i].chain_off = off;
        if (db->head_idx >= 0) { char hkey[fmt_len("%s@%d", key, db->head_idx)]; snprintf(hkey, sizeof hkey, "%s@%d", key, db->head_idx); pl_runtime_install_packet_entry(key, ar, hkey); }
    }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_copy(void *db_v, int i, void *out_v);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *pl_db_head_name(void *pair_cell, int *ar) {
    extern const char *prolog_atom_name(int);
    pl_cell_t *p = pl_deref((pl_cell_t *)pair_cell);
    if ((int)p->v != DT_PLREF || pl_arity(p) != 2) return (const char *)0;
    {
        pl_cell_t *h = pl_deref(&((pl_cell_t *)p->p)[0]);
        if ((int)h->v == DT_PLREF) { *ar = pl_arity(h); return prolog_atom_name(plc_functor(h)); }
        if ((int)h->v == DT_PLATOM) { *ar = 0; return prolog_atom_name((int)h->i); }
        if ((int)h->v == DT_S) { *ar = 0; return h->s; }
        return (const char *)0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_head_key(void *pair_cell, char *out, size_t n, int *ar) { const char *nm = pl_db_head_name(pair_cell, ar); if (!nm) return 0; snprintf(out, n, "%s/%d", nm, *ar); return 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_db_store(void *root, void *db_v, void *clause_term, int prepend, int recompile, int tx);
static void pl_db_reclaim(pl_db_t *db);
static void pl_db_tx_note(void *root, pl_db_t *db, int kind, int slot);
int rt_pl_db_assert(void *root, void *db_v, void *clause_term, int prepend) { return pl_db_store(root, db_v, clause_term, prepend, 1, 1); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_assert_ref(void *root, void *db_v, void *clause_term, int prepend) {
    pl_db_t *db = (pl_db_t *)db_v;
    if (!pl_db_store(root, db_v, clause_term, prepend, 1, 1)) return 0;
    return db->s[db->n - 1].ref;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_slot_of_ref(void *db_v, int ref) {
    pl_db_t *db = (pl_db_t *)db_v;
    if (!db || ref < 1) return -1;
    for (int i = 0; i < db->n; i++) if (!db->s[i].erased && db->s[i].ref == ref) return i;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_ref_at(void *db_v, int i) { pl_db_t *db = (pl_db_t *)db_v; if (!db || i < 0 || i >= db->n || db->s[i].erased) return 0; return db->s[i].ref; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_seed(void *root, void *db_v, void *clause_term) { return pl_db_store(root, db_v, clause_term, 0, 1, 0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_db_store(void *root, void *db_v, void *clause_term, int prepend, int recompile, int tx) {
    extern int prolog_atom_intern(const char *);
    pl_db_t *db = (pl_db_t *)db_v;
    if (!db || !clause_term) return 0;
    pl_db_reclaim(db);
    db->killed = 0;
    if (db->n >= db->cap) {
        int nc = db->cap > 0 ? db->cap * 2 : 8;
        pl_db_slot_t *ns = (pl_db_slot_t *)rt_pl_struct_alloc(HB_PLDBS, (size_t)nc * sizeof(pl_db_slot_t));
        if (!ns) return 0;
        for (int i = 0; i < db->n; i++) ns[i] = db->s[i];
        db->s = ns;
        db->cap = nc;
    }
    {
        pl_cell_t *t = pl_deref((pl_cell_t *)clause_term);
        pl_cell_t kids[2];
        pl_cell_t pair;
        if ((int)t->v == DT_PLREF && pl_arity(t) == 2 && plc_functor(t) == prolog_atom_intern(":-")) {
            pl_cell_t *aa = (pl_cell_t *)t->p;
            kids[0] = aa[0];
            kids[1] = aa[1];
        } else {
            kids[0] = *t;
            kids[1] = pl_make_atom(prolog_atom_intern("true"));
        }
        pair = pl_make_compound(prolog_atom_intern(":-"), 2, (void *)kids);
        {
            pl_cell_t stored = plc_copy(&pair);
            int i = db->n;
            if (db->next_ref < 1) db->next_ref = 1;
            db->s[i].cl = stored;
            db->s[i].erased = 0;
            db->s[i].ref = db->next_ref++;
            db->s[i].ridx = -1;
            db->s[i].chain_off = 0;
            db->s[i].next_idx = -1;
            db->s[i].frag = 0;
            if (prepend) {
                db->s[i].next_idx = db->head_idx;
                if (db->head_idx >= 0 && db->head_idx < db->recl_lo) db->recl_lo = db->head_idx;
                db->head_idx = i;
                if (db->tail_idx < 0) db->tail_idx = i;
            } else {
                if (db->tail_idx >= 0) db->s[db->tail_idx].next_idx = i;
                else db->head_idx = i;
                db->tail_idx = i;
            }
            db->n++;
            if (tx) pl_db_tx_note(root, db, 0, i);
            if (recompile) pl_db_fragment(root, db, i);
        }
        return 1;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { pl_db_t *db; unsigned char *mk; long frames; } pl_db_rr_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_db_rr_mark(pl_db_rr_t *r, const char *p, unsigned char bit) {
    const char *b = (const char *)bb_pool_base();
    if (!p || !b || p < b || p >= b + bb_pool_used()) return;
    for (int i = 0; i < r->db->n; i++) { int f = r->db->s[i].frag; if (f && p >= pl_db_frag_lo(f) && p < pl_db_frag_hi(f)) { r->mk[i] |= bit; return; } }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_db_rr_frame(const void *map, const char *base, void *a) {
    pl_db_rr_t *r = (pl_db_rr_t *)a;
    const gc_frame_map_t *m = (const gc_frame_map_t *)map;
    r->frames++;
    pl_db_rr_mark(r, (const char *)map, 1);
    if (!m || !(m->flags & (GC_FRAME_MAP_BLOB | GC_FRAME_MAP_LAYOUT))) return;
    {
        const uint64_t *t = (const uint64_t *)(m + 1);
        for (long i = 0; i < (long)t[0]; i++) {
            uint64_t q = t[1 + i];
            if (GC_LAY_KIND(q) != GC_LAY_PTR_CODE) continue;
            for (int o = 0; o + 8 <= GC_LAY_SIZE(q); o += 8) pl_db_rr_mark(r, *(const char * const *)(base + GC_LAY_OFF(q) + o), 1);
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_db_reclaim_report(pl_db_t *db) {
    extern char *rt_gc_program_stack_top(void);
    const char *e = getenv("SCRIP_PL_RECLAIM_REPORT");
    long every = e ? strtol(e, (char **)0, 10) : 0;
    if (every <= 0 || !db || db->n <= 0 || db->erased_n % every) return;
    {
        unsigned char mk[db->n];
        pl_db_rr_t r = { db, mk, 0 };
        int ar = 0, live = 0, recount = 0, unrec = 0, vis = 0, raw = 0, only = 0, ok;
        long pages = 0, gone = 0;
        const char *nm = (const char *)0, *top = rt_gc_program_stack_top();
        char probe = 0;
        memset(mk, 0, sizeof mk);
        ok = rt_gc_frames_visit(pl_db_rr_frame, (void *)&r);
        for (const char *w = (const char *)(((uintptr_t)&probe + 7) & ~(uintptr_t)7); top && w + 8 <= top; w += 8) pl_db_rr_mark(&r, *(const char * const *)w, 2);
        for (int i = 0; i < db->n; i++) {
            if (!nm) nm = pl_db_head_name((void *)&db->s[i].cl, &ar);
            if (!db->s[i].erased) { live++; continue; }
            recount++;
            if (!PL_DB_FRAG_REC(db->s[i].frag)) unrec++;
            else if (db->s[i].frag & PL_DB_FRAG_GONE) gone += PL_DB_FRAG_PAGES(db->s[i].frag);
            else pages += PL_DB_FRAG_PAGES(db->s[i].frag);
            if (mk[i] & 1) vis++;
            if (mk[i] & 2) raw++;
            if ((mk[i] & 2) && !(mk[i] & 1)) only++;
        }
        fprintf(stderr,
            "[PL-RECLAIM] key=%s/%d slots=%d live=%d erased=%d recount=%d erased_pages=%ld unrecorded=%d visitor=%s frames=%ld visitor_live=%d raw_live=%d raw_only=%d pool_used=%zu slot_bytes=%ld"
            " reclaimed=%d reclaimed_pages=%ld\n", nm ? nm : "?", ar, db->n, live, db->erased_n, recount, pages, unrec, ok ? "whole" : "refused", r.frames, vis, raw, only, bb_pool_used(),
            (long)db->cap * (long)sizeof(pl_db_slot_t), db->recl_n, gone);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_db_frag_find(const pl_db_t *db, const char *p) {
    int lo = 0, hi = db->n - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2, m = mid;
        while (m <= hi && !PL_DB_FRAG_REC(db->s[m].frag)) m++;
        if (m > hi) { hi = mid - 1; continue; }
        if (p < pl_db_frag_lo(db->s[m].frag)) hi = mid - 1;
        else if (p >= pl_db_frag_hi(db->s[m].frag)) lo = m + 1;
        else return m;
    }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { pl_db_t *db; const char *lo; const char *hi; int min_g; int c_lo; int c_hi; } pl_db_rc_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_db_rc_hold(pl_db_rc_t *r, const char *p) {
    int j;
    if (!p || p < r->lo || p >= r->hi || (j = pl_db_frag_find(r->db, p)) < 0) return;
    if (r->db->s[j].frag & PL_DB_FRAG_GONE)
        rt_bomb("the assertz reclaim: a frame holds a reclaimed run-time Prolog fragment -- a pass reclaimed code a live activation still reaches (unification.c pl_db_rc_hold)");
    if (r->db->s[j].ref + 1 < r->min_g) r->min_g = r->db->s[j].ref + 1;
    if (j >= r->c_lo && j <= r->c_hi) r->db->s[j].frag |= PL_DB_FRAG_HELD;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_db_rc_frame(const void *map, const char *base, void *a) {
    pl_db_rc_t *r = (pl_db_rc_t *)a;
    const gc_frame_map_t *m = (const gc_frame_map_t *)map;
    pl_db_rc_hold(r, (const char *)map);
    if (!m || !(m->flags & (GC_FRAME_MAP_BLOB | GC_FRAME_MAP_LAYOUT))) return;
    {
        const uint64_t *t = (const uint64_t *)(m + 1);
        for (long i = 0; i < (long)t[0]; i++) {
            uint64_t q = t[1 + i];
            if (GC_LAY_KIND(q) != GC_LAY_PTR_CODE) continue;
            for (int o = 0; o + 8 <= GC_LAY_SIZE(q); o += 8) pl_db_rc_hold(r, *(const char * const *)(base + GC_LAY_OFF(q) + o));
        }
    }
}
static void pl_db_rc_seg(const char *lo, const char *hi, void *a) {
    pl_db_rc_t *r = (pl_db_rc_t *)a;
    for (const char *w = (const char *)(((uintptr_t)lo + 7) & ~(uintptr_t)7); w + 8 <= hi; w += 8) {
        uint64_t x = *(const uint64_t *)w;
        const char *p = (const char *)(uintptr_t)(x >> 8);
        int j;
        if ((x & 255u) != (uint64_t)DT_RAW || p < r->lo || p >= r->hi || (j = pl_db_frag_find(r->db, p)) < 0 || (r->db->s[j].frag & PL_DB_FRAG_GONE)) continue;
        pl_db_rc_hold(r, p);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern void rt_pl_db_reclaimed_entry(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_db_reclaim_one(pl_db_t *db, int i) {
    extern void rt_gc_frame_maps_drop_range(const void *, const void *);
    extern void rt_gc_frame_sites_drop_range(const void *, const void *);
    extern void rt_proc_set_fn_at(int, bb_box_fn);
    extern void rt_proc_set_dcfn(const char *, void *);
    extern void bb_ab_seal_alpha(const char *, void *);
    int f = db->s[i].frag, ar = 0;
    char *lo = (char *)pl_db_frag_lo(f), *hi = (char *)pl_db_frag_hi(f);
    const char *nm = pl_db_head_name((void *)&db->s[i].cl, &ar);
    if (!lo || !nm || db->s[i].ridx < 0 || mprotect(lo, (size_t)(hi - lo), PROT_NONE) != 0) return 0;
    madvise(lo, (size_t)(hi - lo), MADV_DONTNEED);
    rt_gc_frame_maps_drop_range(lo, hi);
    rt_gc_frame_sites_drop_range(lo, hi);
    {
        PL_DB_KEY_VLA(key, nm, ar);
        char fkey[fmt_len("%s@%d", key, i)];
        snprintf(fkey, sizeof fkey, "%s@%d", key, i);
        rt_proc_set_fn_at(db->s[i].ridx, (bb_box_fn)rt_pl_db_reclaimed_entry);
        rt_proc_set_dcfn(fkey, (void *)rt_pl_db_reclaimed_entry);
        bb_ab_seal_alpha(fkey, (void *)rt_pl_db_reclaimed_entry);
    }
    db->s[i].chain_off = 0;
    db->s[i].frag = (f & ~PL_DB_FRAG_HELD) | PL_DB_FRAG_GONE;
    db->recl_n++;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_db_reclaim_cand(const pl_db_t *db, int i) { int f = db->s[i].frag; return db->s[i].erased && PL_DB_FRAG_REC(f) && !(f & PL_DB_FRAG_GONE) && i != db->head_idx; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_db_reclaim(pl_db_t *db) {
    extern long rt_gc_runs_count(void);
    const char *b = (const char *)bb_pool_base();
    int runs, lo = -1, hi = -1, next_lo;
    if (!db || db->frag_top < 0 || !b || db->recl_since <= 0 || db->tx_pins > 0) return;
    runs = (int)rt_gc_runs_count();
    if (runs == db->recl_gc && db->recl_since < 1024) return;
    db->recl_gc = runs;
    db->recl_since = 0;
    for (int i = db->recl_lo; i < db->n; i++) if (pl_db_reclaim_cand(db, i)) { if (lo < 0) lo = i; hi = i; }
    if (lo < 0) { db->recl_lo = db->n; return; }
    {
        pl_db_rc_t r = { db, b, b + bb_pool_used(), INT_MAX, lo, hi };
        int ok = rt_gc_frames_visit(pl_db_rc_frame, (void *)&r);
        if (ok) rt_gc_segments_visit(pl_db_rc_seg, (void *)&r);
        next_lo = db->n;
        for (int i = lo; i <= hi; i++) {
            int held = db->s[i].frag & PL_DB_FRAG_HELD;
            db->s[i].frag &= ~PL_DB_FRAG_HELD;
            if (!pl_db_reclaim_cand(db, i)) continue;
            if (!ok || held || db->s[i].erased >= r.min_g || !pl_db_reclaim_one(db, i)) { if (i < next_lo) next_lo = i; }
        }
        db->recl_lo = next_lo;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_db_note_erase(pl_db_t *db, int i, int gen) { db->s[i].erased = gen; db->erased_n++; db->recl_since++; if (i < db->recl_lo) db->recl_lo = i; }
static pl_cell_t * pl_db_tx_log(void *root) {
    extern int prolog_atom_intern(const char *);
    pl_db_reg_t *r = pl_db_registry(root, 0);
    pl_db_key_t *e = (r && r->hix) ? rt_pl_db_reg_find_na(r, "$tx_cur", -1) : (pl_db_key_t *)0;
    pl_cell_t **cell, *box, *t;
    if (!e || e->k < 0) return (pl_cell_t *)0;
    cell = (pl_cell_t **)pl_db_cell_addr(root, e->k, 0);
    box = cell ? *cell : (pl_cell_t *)0;
    if (!box || plc_gvar_absent(box)) return (pl_cell_t *)0;
    t = pl_deref(box);
    if ((int)t->v != DT_PLREF || !t->p || plc_fid_arity(t->slen) != 1 || plc_fid_name(t->slen) != prolog_atom_intern("tx")) return (pl_cell_t *)0;
    return &((pl_cell_t *)t->p)[0];
}
static void pl_db_tx_note(void *root, pl_db_t *db, int kind, int slot) {
    extern int prolog_atom_intern(const char *);
    pl_cell_t *lst = pl_db_tx_log(root);
    const char *nm;
    int ar = 0;
    if (!lst || !db || slot < 0 || slot >= db->n) return;
    nm = pl_db_head_name((void *)&db->s[slot].cl, &ar);
    if (!nm || nm[0] == '$') return;
    {
        pl_cell_t *da = (pl_cell_t *)rt_ws_alloc_descr(3), *cons = (pl_cell_t *)rt_ws_alloc_descr(2);
        da[0] = pl_make_int(kind);
        da[1] = pl_make_int(db->cell_k);
        da[2] = pl_make_int(slot);
        cons[0] = pl_make_compound(prolog_atom_intern("$txdb"), 3, (void *)da);
        cons[1] = *lst;
        *lst = pl_make_compound(g_runtime.atoms.atom_dot, 2, (void *)cons);
        if (kind == 1) db->tx_pins++;
    }
}
static void pl_db_erase_slot(void *root, pl_db_t *db, int i, int gen) { pl_db_note_erase(db, i, gen); pl_db_tx_note(root, db, 1, i); }
int rt_pl_mod_ext(void *root, void *mc, void *gc, void *ec, void *out, pl_tr_ctx_t *cx) {
    extern const char *prolog_atom_name(int);
    extern int prolog_atom_intern(const char *);
    extern int rt_proc_is_registered(const char *);
    pl_cell_t *m = pl_deref((pl_cell_t *)mc), *g = pl_deref((pl_cell_t *)gc), *e = pl_deref((pl_cell_t *)ec), r = *g;
    if ((int)m->v == DT_PLATOM && (int)e->v == DT_I && ((int)g->v == DT_PLATOM || ((int)g->v == DT_PLREF && g->p))) {
        const char *mn = prolog_atom_name((int)m->i);
        int ar = (int)g->v == DT_PLREF ? plc_fid_arity(g->slen) : 0;
        const char *gn = (int)g->v == DT_PLREF ? prolog_atom_name(plc_fid_name(g->slen)) : prolog_atom_name((int)g->i);
        if (mn && gn && strcmp(mn, "user")) {
            char key[strlen(mn) + strlen(gn) + 24];
            snprintf(key, sizeof key, "%s:%s/%d", mn, gn, ar + (int)e->i);
            if (rt_proc_is_registered(key) || rt_pl_db_key_cell(root, key) >= 0) {
                char qn[strlen(mn) + strlen(gn) + 2];
                int qid;
                snprintf(qn, sizeof qn, "%s:%s", mn, gn);
                qid = prolog_atom_intern(qn);
                r = (int)g->v == DT_PLREF ? pl_make_compound(qid, ar, g->p) : pl_make_atom(qid);
            }
        }
    }
    return plc_unify_cells_cx((pl_cell_t *)out, &r, cx) ? 1 : 0;
}
int rt_pl_tx_db(void *root, int op, void *recs) {
    extern int prolog_atom_intern(const char *);
    int txdb = prolog_atom_intern("$txdb");
    pl_cell_t *l = pl_deref((pl_cell_t *)recs);
    while ((int)l->v == DT_PLREF && l->p && plc_fid_arity(l->slen) == 2 && plc_fid_name(l->slen) == g_runtime.atoms.atom_dot) {
        pl_cell_t *cons = (pl_cell_t *)l->p, *d = pl_deref(&cons[0]);
        if ((int)d->v == DT_PLREF && d->p && plc_fid_arity(d->slen) == 3 && plc_fid_name(d->slen) == txdb) {
            pl_cell_t *a = (pl_cell_t *)d->p, *k = pl_deref(&a[0]), *c = pl_deref(&a[1]), *s = pl_deref(&a[2]);
            pl_db_t *db = ((int)k->v == DT_I && (int)c->v == DT_I && (int)s->v == DT_I) ? (pl_db_t *)rt_pl_db_get(root, c->i) : (pl_db_t *)0;
            int i = (int)s->i;
            if (db && i >= 0 && i < db->n) {
                if (k->i == 1) {
                    if (db->tx_pins > 0) db->tx_pins--;
                    if (op == 0 && db->s[i].erased) { db->s[i].erased = 0; if (db->erased_n > 0) db->erased_n--; }
                } else if (op == 0 && !db->s[i].erased) pl_db_note_erase(db, i, db->next_ref++);
            }
        }
        l = pl_deref(&cons[1]);
    }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_erase(void *root, void *db_v, int i) {
    pl_db_t *db = (pl_db_t *)db_v;
    if (!db || i < 0 || i >= db->n || db->s[i].erased) return 0;
    pl_db_erase_slot(root, db, i, db->next_ref++);
    pl_db_reclaim_report(db);
    pl_db_reclaim(db);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_count(void *db_v) { pl_db_t *db = (pl_db_t *)db_v; return db ? db->n : 0; }
int rt_pl_db_slot_erased(void *db_v, int i) { pl_db_t *db = (pl_db_t *)db_v; return db && i >= 0 && i < db->n && db->s[i].erased; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_gen(void *db_v) { pl_db_t *db = (pl_db_t *)db_v; return db ? db->next_ref : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_db_slot_copy(pl_db_t *db, int i, void *out_v) { *(pl_cell_t *)out_v = plc_copy(&db->s[i].cl); return 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_clause_as_of(void *db_v, int i, int gen, void *out_v) {
    pl_db_t *db = (pl_db_t *)db_v;
    if (!db || !out_v || i < 0 || i >= db->n || db->s[i].ref >= gen || (db->s[i].erased && db->s[i].erased < gen)) return 0;
    return pl_db_slot_copy(db, i, out_v);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_live_count(void *db_v) { pl_db_t *db = (pl_db_t *)db_v; return db ? db->n - db->erased_n : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_clause_at(void *db_v, int i, void *out_v) { pl_db_t *db = (pl_db_t *)db_v; if (!db || !out_v || i < 0 || i >= db->n || db->s[i].erased) return 0; return pl_db_slot_copy(db, i, out_v); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_copy(void *db_v, int i, void *out_v) { pl_db_t *db = (pl_db_t *)db_v; if (!db || !out_v || i < 0 || i >= db->n) return 0; return pl_db_slot_copy(db, i, out_v); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_db_define_absent(void *root, const char *key, int arity) {
    extern void * rt_pl_clause_tree(void *);
    extern void * pl_runtime_clause_tree(void *);
    extern void * pl_runtime_define_pred(const char *, const void *, int, void *);
    extern void * rt_pl_choice_new(const char *);
    extern void rt_pl_choice_add(void *, void *);
    extern int prolog_atom_intern(const char *);
    extern int rt_proc_is_registered(const char *);
    extern int pl_runtime_install_packet_entry(const char *, int, const char *);
    const char *sl;
    pl_cell_t *hargs;
    pl_cell_t head;
    pl_cell_t pi[2];
    pl_cell_t pic;
    pl_cell_t ee[2];
    pl_cell_t eec;
    pl_cell_t er[2];
    pl_cell_t erc;
    pl_cell_t th[1];
    pl_cell_t body;
    pl_cell_t kids[2];
    pl_cell_t pair;
    void *raw;
    void *cl;
    void *ch;
    if (!key) return 0;
    sl = strrchr(key, '/');
    if (!sl) return 0;
    char skey[fmt_len("%s@absent", key)];
    snprintf(skey, sizeof skey, "%s@absent", key);
    if (rt_proc_is_registered(skey)) return pl_runtime_install_packet_entry(key, arity, skey);
    int nid = prolog_atom_intern_n(key, (size_t)(sl - key));
    if (arity > 0) {
        hargs = (pl_cell_t *)PL_CELL_ALLOC((size_t)arity * sizeof(pl_cell_t));
        if (!hargs) return 0;
        for (int i = 0; i < arity; i++) pl_init_var(&hargs[i], -1);
        head = pl_make_compound(nid, arity, hargs);
    } else head = pl_make_atom(nid);
    pi[0] = pl_make_atom(nid);
    pi[1] = pl_make_int(arity);
    pic = pl_make_compound(prolog_atom_intern("/"), 2, (void *)pi);
    ee[0] = pl_make_atom(prolog_atom_intern("procedure"));
    ee[1] = pic;
    eec = pl_make_compound(prolog_atom_intern("existence_error"), 2, (void *)ee);
    er[0] = eec;
    er[1] = pic;
    erc = pl_make_compound(prolog_atom_intern("error"), 2, (void *)er);
    th[0] = erc;
    body = pl_make_compound(prolog_atom_intern("throw"), 1, (void *)th);
    kids[0] = head;
    kids[1] = body;
    pair = pl_make_compound(prolog_atom_intern(":-"), 2, (void *)kids);
    raw = rt_pl_clause_tree((void *)&pair);
    cl = raw ? pl_runtime_clause_tree(raw) : (void *)0;
    if (!cl) return 0;
    ch = rt_pl_choice_new(skey);
    rt_pl_choice_add(ch, cl);
    if (!pl_runtime_define_pred(skey, ch, arity, root)) return 0;
    return pl_runtime_install_packet_entry(key, arity, skey);
}
int rt_pl_db_abolish(void *root, void *db_v) {
    pl_db_t *db = (pl_db_t *)db_v;
    int ar = 0;
    int hi = -1;
    if (!db) return 0;
    for (int i = 0; i < db->n && hi < 0; i++) if (pl_db_head_name((void *)&db->s[i].cl, &ar)) hi = i;
    { int st = db->next_ref++; for (int i = 0; i < db->n; i++) if (!db->s[i].erased) pl_db_erase_slot(root, db, i, st); }
    pl_db_reclaim_report(db);
    db->killed = 1;
    if (hi >= 0) { const char *nm = pl_db_head_name((void *)&db->s[hi].cl, &ar); PL_DB_KEY_VLA(key, nm, ar); pl_db_define_absent(root, key, ar); }
    pl_db_reclaim(db);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_match_erase(void *root, void *db_v, void *goal_term) {
    pl_db_t *db = (pl_db_t *)db_v;
    int hit = 0;
    int ar = 0;
    int key_i = -1;
    if (!db || !goal_term) return 0;
    for (int i = 0; i < db->n; i++) {
        if (db->s[i].erased) continue;
        {
            pl_cell_t pair = plc_copy(&db->s[i].cl);
            pl_cell_t g = plc_copy((pl_cell_t *)goal_term);
            pl_cell_t *h = (pl_cell_t *)pl_deref(&pair)->p;
            if (h && pl_unify(&h[0], &g)) { if (key_i < 0 && pl_db_head_name((void *)&db->s[i].cl, &ar)) key_i = i; pl_db_erase_slot(root, db, i, db->next_ref++); hit++; }
        }
    }
    (void)key_i;
    (void)ar;
    if (hit) pl_db_reclaim_report(db);
    if (hit) pl_db_reclaim(db);
    return hit;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_killed(void *db_v) { pl_db_t *db = (pl_db_t *)db_v; return db ? db->killed : 0; }
