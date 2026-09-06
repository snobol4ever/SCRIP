#include "rt/rt_arena.h"
#include "rt/rt.h"
#include "core.h"
#include "bb_pool.h"
#include "../parsers/prolog/prolog_atom.h"
#include "../ir/IR.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "../parsers/prolog/pl_cell.h"
#include "rt/rt_pl_trail.h"
#define PL_CELL_ALLOC(n) (rt_ws_alloc(n))
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
void * rt_enter(void **slot, int nslots)
{
    extern void *rt_ws_alloc(size_t);
    if (!*slot) *slot = rt_ws_alloc((size_t)(8 + 16 * (nslots > 0 ? nslots : 1)));
    return *slot;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_unify_cell_float(void *cell, double dval)
{
    pl_cell_t *c = (pl_cell_t *)cell; if (!c) return 0;
    pl_cell_t w = pl_make_float(dval);
    if (!pl_unify(c, &w)) { return 0; }
    return 1;
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
int rt_pl_unify_struct(void *dst, const char *functor_name, int arity, void *arg_words)
{
    pl_cell_t *src = (pl_cell_t *)arg_words; if (!dst) return 0;
    pl_cell_t *D = pl_deref((pl_cell_t *)dst);
    int fid = prolog_atom_intern(functor_name ? functor_name : "[]");
    if (pl_cell_unbound(D)) {
        pl_cell_t *blk = (pl_cell_t *)PL_CELL_ALLOC((size_t)(arity > 0 ? arity : 1) * sizeof(pl_cell_t));
        for (int i = 0; i < arity; i++) blk[i] = src[i];
        pl_bind(D, pl_make_compound(fid, arity, blk)); return 1;
    }
    if ((int)D->v == (int)DT_PLREF) {
        uint32_t want = (((uint32_t)fid) << 16) | ((uint32_t)arity & 0xFFFFu); if (D->slen != want) return 0;
        pl_cell_t *blk = (pl_cell_t *)D->p;        for (int i = 0; i < arity; i++) if (!pl_unify(&blk[i], &src[i])) { return 0; }
        return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static FILE *plc_out(void) { extern FILE *fh_cur_out_fp(void); FILE *f = fh_cur_out_fp(); return f ? f : stdout; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { pl_cell_t *seen[1024]; int n; FILE *fp; } plc_vmap;
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
static int plc_op_prec(const char *name, int arity)
{
    struct { const char *n; int a; int p; } tbl[] = {
        {":-",2,1200},{";",2,1100},{"->",2,1050},{",",2,1000},
        {"=",2,700},{"\\=",2,700},{"is",2,700},{"=:=",2,700},{"=\\=",2,700},
        {"<",2,700},{">",2,700},{"=<",2,700},{">=",2,700},
        {"==",2,700},{"\\==",2,700},{"@<",2,700},{"@>",2,700},
        {"@=<",2,700},{"@>=",2,700},{"=..",2,700},
        {"+",2,500},{"-",2,500},
        {"*",2,400},{"/",2,400},{"//",2,400},{"mod",2,400},
        {"rem",2,400},{"<<",2,400},{">>",2,400},
        {"**",2,200},{"^",2,200},
        {"-",1,200},{"\\+",1,900},{"not",1,900},
        {NULL,0,0}
    };
    for (int i = 0; tbl[i].n; i++) if (tbl[i].a == arity && strcmp(tbl[i].n, name) == 0) return tbl[i].p;
    { int p, r; if (plc_user_op(name, arity, &p, &r)) return p; }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_operand_prec(pl_cell_t *a)
{
    pl_cell_t *d = pl_deref(a);
    if ((int)d->v != DT_PLREF) return -1;
    const char *n = prolog_atom_name((int)(d->slen >> 16));
    return n ? plc_op_prec(n, (int)(d->slen & 0xFFFFu)) : -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_is_nil(pl_cell_t *d)
{
    extern int ATOM_NIL;
    if ((int)d->v == DT_A) return (int)d->i == ATOM_NIL;
    if ((int)d->v == DT_S) return d->s && strcmp(d->s, "[]") == 0;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_write(pl_cell_t *c, plc_vmap *m)
{
    extern FILE *fh_cur_out_fp(void);
    extern int ATOM_DOT;
    FILE *fp = m->fp;
    if (!c) { fprintf(fp, "[]"); return; }
    pl_cell_t *d = pl_deref(c);
    int tg = (int)d->v;
    if (pl_cell_unbound(d)) { fprintf(fp, "_G%d", plc_vindex(m, d)); return; }
    if (tg == DT_I) { fprintf(fp, "%ld", (long)d->i); return; }
    if (tg == DT_A || tg == DT_S) { fprintf(fp, "%s", plc_atom_text(d)); return; }
    if (tg == DT_R) {
        double fv = d->r; char fb[64];
        for (int prec = 15; prec <= 17; prec++) { snprintf(fb, sizeof fb, "%.*g", prec, fv); if (strtod(fb, NULL) == fv) break; }
        if (!strpbrk(fb, ".eEnN")) { size_t n = strlen(fb); if (n+2 < sizeof fb) { fb[n]='.'; fb[n+1]='0'; fb[n+2]='\0'; } }
        fputs(fb, fp); return;
    }
    if (tg != DT_PLREF) { fprintf(fp, "_G%d", plc_vindex(m, d)); return; }
    int fnid = (int)(d->slen >> 16), ar = (int)(d->slen & 0xFFFFu);
    pl_cell_t *aa = (pl_cell_t *)d->p;
    const char *fn = prolog_atom_name(fnid);
    if (!fn) fn = "?";
    if (strcmp(fn, "$VAR") == 0 && ar == 1) {
        pl_cell_t *n = pl_deref(&aa[0]);
        if ((int)n->v == DT_I) {
            long num = (long)n->i; int letter = (int)(num % 26); long suffix = num / 26;
            if (suffix == 0) fprintf(fp, "%c", 'A' + letter); else fprintf(fp, "%c%ld", 'A' + letter, suffix);
            return;
        }
    }
    if (strcmp(fn, "$VARNAME") == 0 && ar == 1) {
        pl_cell_t *n = pl_deref(&aa[0]);
        if ((int)n->v == DT_A || (int)n->v == DT_S) { const char *vn = plc_atom_text(n); fprintf(fp, "%s", vn ? vn : "_"); return; }
    }
    if (fnid == ATOM_DOT && ar == 2) {
        fprintf(fp, "[");
        plc_write(&aa[0], m);
        pl_cell_t *tail = pl_deref(&aa[1]);
        while ((int)tail->v == DT_PLREF && (int)(tail->slen >> 16) == ATOM_DOT && (int)(tail->slen & 0xFFFFu) == 2) {
            pl_cell_t *ta = (pl_cell_t *)tail->p;
            fprintf(fp, ","); plc_write(&ta[0], m); tail = pl_deref(&ta[1]);
        }
        if (plc_is_nil(tail)) { } else { fprintf(fp, "|"); plc_write(tail, m); }
        fprintf(fp, "]"); return;
    }
    if (ar == 1 && strcmp(fn, "{}") == 0) { fprintf(fp, "{"); plc_write(&aa[0], m); fprintf(fp, "}"); return; }
    struct { const char *name; int arity; int prec; int right_assoc; } ops[] = {
        {":-",2,1200,1}, {";",2,1100,1}, {"->",2,1050,1},
        {",",2,1000,1},
        {"=",2,700,0},{"\\=",2,700,0},{"is",2,700,0},
        {"=:=",2,700,0},{"=\\=",2,700,0},
        {"<",2,700,0},{">",2,700,0},{"=<",2,700,0},{">=",2,700,0},
        {"==",2,700,0},{"\\==",2,700,0},
        {"@<",2,700,0},{"@>",2,700,0},{"@=<",2,700,0},{"@>=",2,700,0},
        {"=..",2,700,0},
        {"+",2,500,0},{"-",2,500,0},
        {"*",2,400,0},{"/",2,400,0},{"//",2,400,0},{"mod",2,400,0},
        {"rem",2,400,0},{"<<",2,400,0},{">>",2,400,0},
        {"**",2,200,1},{"^",2,200,1},
        {"-",1,200,0},{"\\+",1,900,0},{"not",1,900,0},
        {NULL,0,0,0}
    };
    int op_prec = -1, op_ra = 0;
    for (int i = 0; ops[i].name; i++) if (strcmp(fn, ops[i].name) == 0 && ar == ops[i].arity) { op_prec = ops[i].prec; op_ra = ops[i].right_assoc; break; }
    if (op_prec < 0) plc_user_op(fn, ar, &op_prec, &op_ra);
    if (op_prec >= 0) {
            if (ar == 2) {
                int lp = plc_operand_prec(&aa[0]), rp = plc_operand_prec(&aa[1]), my_prec = op_prec;
                int lneed = (lp > my_prec) || (lp == my_prec && op_ra);
                int rneed = (rp > my_prec) || (rp == my_prec && !op_ra);
                if (lneed) fprintf(fp, "(");
                plc_write(&aa[0], m);
                if (lneed) fprintf(fp, ")");
                if (isalpha((unsigned char)fn[0])) fprintf(fp, " %s ", fn); else fprintf(fp, "%s", fn);
                if (rneed) fprintf(fp, "(");
                plc_write(&aa[1], m);
                if (rneed) fprintf(fp, ")");
            } else {
                int ap = plc_operand_prec(&aa[0]);
                int aneed = (ap >= op_prec);
                if (isalpha((unsigned char)fn[0])) fprintf(fp, "%s ", fn); else fprintf(fp, "%s", fn);
                if (aneed) fprintf(fp, "(");
                plc_write(&aa[0], m);
                if (aneed) fprintf(fp, ")");
            }
            return;
            }
    fprintf(fp, "%s(", fn);
    for (int i = 0; i < ar; i++) { if (i) fprintf(fp, ","); plc_write(&aa[i], m); }
    fprintf(fp, ")");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_atom_needs_quoting(const char *name)
{
    if (!name || !name[0]) return 1;
    if (name[0] == '[' && strcmp(name, "[]") == 0) return 0;
    if (name[0] == '{' && strcmp(name, "{}") == 0) return 0;
    if (isupper((unsigned char)name[0]) || name[0] == '_') return 1;
    int all_graphic = 1;
    static const char *const graphic = "#&*+-./:<=>?@\\^~";
    for (const char *q = name; *q; q++) if (!strchr(graphic, *q)) { all_graphic = 0; break; }
    if (all_graphic) return 0;
    if (islower((unsigned char)name[0])) { for (const char *q = name+1; *q; q++) if (!isalnum((unsigned char)*q) && *q != '_') return 1; return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_wt_atom(FILE *fp, const char *name, int quoted)
{
    if (!name) name = "?";
    if (quoted && plc_atom_needs_quoting(name)) { fputc('\'', fp); for (const char *q = name; *q; q++) { if (*q == '\'') fputc('\'', fp); fputc(*q, fp); } fputc('\'', fp); }
    else fprintf(fp, "%s", name);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_wt(pl_cell_t *c, int quoted, int ignore_ops, int numbervars, long max_depth, long depth, plc_vmap *m)
{
    extern int ATOM_DOT;
    FILE *fp = m->fp;
    if (!c) { plc_wt_atom(fp, "[]", quoted); return; }
    pl_cell_t *d = pl_deref(c);
    if (max_depth > 0 && depth >= max_depth) { fprintf(fp, "..."); return; }
    int tg = (int)d->v;
    if (pl_cell_unbound(d)) { fprintf(fp, "_G%d", plc_vindex(m, d)); return; }
    if (tg == DT_A || tg == DT_S) { plc_wt_atom(fp, plc_atom_text(d), quoted); return; }
    if (tg == DT_I) { fprintf(fp, "%ld", (long)d->i); return; }
    if (tg == DT_R) {
        double fv = d->r; char fb[64];
        for (int pr = 15; pr <= 17; pr++) { snprintf(fb, sizeof fb, "%.*g", pr, fv); if (strtod(fb, NULL) == fv) break; }
        if (!strpbrk(fb, ".eEnN")) { size_t n = strlen(fb); if (n+2 < sizeof fb) { fb[n]='.'; fb[n+1]='0'; fb[n+2]='\0'; } }
        fputs(fb, fp); return;
    }
    if (tg != DT_PLREF) { fprintf(fp, "_G%d", plc_vindex(m, d)); return; }
    int fnid = (int)(d->slen >> 16), ar = (int)(d->slen & 0xFFFFu);
    pl_cell_t *aa = (pl_cell_t *)d->p;
    const char *fn = prolog_atom_name(fnid);
    if (!fn) fn = "?";
    if (numbervars && strcmp(fn, "$VAR") == 0 && ar == 1) {
        pl_cell_t *n = pl_deref(&aa[0]);
        if ((int)n->v == DT_I) { long num = (long)n->i; int letter = (int)(num % 26); long suf = num / 26;
            if (suf == 0) fprintf(fp, "%c", 'A' + letter); else fprintf(fp, "%c%ld", 'A' + letter, suf); return; }
    }
    if (!ignore_ops && fnid == ATOM_DOT && ar == 2) {
        fprintf(fp, "["); plc_wt(&aa[0], quoted, ignore_ops, numbervars, max_depth, depth+1, m);
        pl_cell_t *tail = pl_deref(&aa[1]); long dd = depth + 1; int open = 1;
        while ((int)tail->v == DT_PLREF && (int)(tail->slen >> 16) == ATOM_DOT && (int)(tail->slen & 0xFFFFu) == 2) {
            if (max_depth > 0 && dd >= max_depth) { fprintf(fp, "|..."); tail = (pl_cell_t *)0; open = 0; break; }
            pl_cell_t *ta = (pl_cell_t *)tail->p;
            fprintf(fp, ","); plc_wt(&ta[0], quoted, ignore_ops, numbervars, max_depth, dd+1, m);
            tail = pl_deref(&ta[1]); dd++;
        }
        if (open && tail && !plc_is_nil(tail)) { fprintf(fp, "|"); plc_wt(tail, quoted, ignore_ops, numbervars, max_depth, dd, m); }
        fprintf(fp, "]"); return;
    }
    struct { const char *name; int arity; int prec; int right_assoc; } ops[] = {
        {":-",2,1200,1},{";",2,1100,1},{"->",2,1050,1},{",",2,1000,1},
        {"=",2,700,0},{"\\=",2,700,0},{"is",2,700,0},{"=:=",2,700,0},{"=\\=",2,700,0},
        {"<",2,700,0},{">",2,700,0},{"=<",2,700,0},{">=",2,700,0},
        {"==",2,700,0},{"\\==",2,700,0},{"@<",2,700,0},{"@>",2,700,0},{"@=<",2,700,0},{"@>=",2,700,0},
        {"=..",2,700,0},{"+",2,500,0},{"-",2,500,0},
        {"*",2,400,0},{"/",2,400,0},{"//",2,400,0},{"mod",2,400,0},{"rem",2,400,0},{"<<",2,400,0},{">>",2,400,0},
        {"**",2,200,1},{"^",2,200,1},{"-",1,200,0},{"\\+",1,900,0},{"not",1,900,0},{NULL,0,0,0}
    };
    int op_prec = -1, op_ra = 0;
    if (!ignore_ops) { for (int i = 0; ops[i].name; i++) if (strcmp(fn, ops[i].name) == 0 && ar == ops[i].arity) { op_prec = ops[i].prec; op_ra = ops[i].right_assoc; break; }
        if (op_prec < 0) plc_user_op(fn, ar, &op_prec, &op_ra); }
    if (op_prec >= 0) {
        if (ar == 2) {
            int lp = plc_operand_prec(&aa[0]), rp = plc_operand_prec(&aa[1]), my = op_prec;
            if ((lp > my) || (lp == my && op_ra)) { fprintf(fp, "("); plc_wt(&aa[0], quoted, ignore_ops, numbervars, max_depth, depth+1, m); fprintf(fp, ")"); }
            else plc_wt(&aa[0], quoted, ignore_ops, numbervars, max_depth, depth+1, m);
            if (isalpha((unsigned char)fn[0])) fprintf(fp, " %s ", fn); else fprintf(fp, "%s", fn);
            if ((rp > my) || (rp == my && !op_ra)) { fprintf(fp, "("); plc_wt(&aa[1], quoted, ignore_ops, numbervars, max_depth, depth+1, m); fprintf(fp, ")"); }
            else plc_wt(&aa[1], quoted, ignore_ops, numbervars, max_depth, depth+1, m);
        } else {
            int ap = plc_operand_prec(&aa[0]);
            if (isalpha((unsigned char)fn[0])) fprintf(fp, "%s ", fn); else fprintf(fp, "%s", fn);
            if (ap >= op_prec) { fprintf(fp, "("); plc_wt(&aa[0], quoted, ignore_ops, numbervars, max_depth, depth+1, m); fprintf(fp, ")"); }
            else plc_wt(&aa[0], quoted, ignore_ops, numbervars, max_depth, depth+1, m);
        }
        return;
        }
    if (fn[0] == '.' && fn[1] == 0) fprintf(fp, "'.'"); else plc_wt_atom(fp, fn, quoted);
    fprintf(fp, "(");
    for (int i = 0; i < ar; i++) { if (i) fprintf(fp, ","); plc_wt(&aa[i], quoted, ignore_ops, numbervars, max_depth, depth+1, m); }
    fprintf(fp, ")");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_writeq(pl_cell_t *c, plc_vmap *m)
{
    extern int ATOM_DOT;
    FILE *fp = m->fp;
    if (!c) { fprintf(fp, "'[]'"); return; }
    pl_cell_t *d = pl_deref(c);
    int tg = (int)d->v;
    if (pl_cell_unbound(d)) { fprintf(fp, "_G%d", plc_vindex(m, d)); return; }
    if (tg == DT_A || tg == DT_S) { plc_wt_atom(fp, plc_atom_text(d), 1); return; }
    if (tg == DT_I) { fprintf(fp, "%ld", (long)d->i); return; }
    if (tg == DT_R) {
        double fv = d->r;
        if (fv == (long)fv && fv >= -1e15 && fv <= 1e15) fprintf(fp, "%.1f", fv); else fprintf(fp, "%g", fv);
        return;
    }
    if (tg != DT_PLREF) { fprintf(fp, "_G%d", plc_vindex(m, d)); return; }
    int fnid = (int)(d->slen >> 16), ar = (int)(d->slen & 0xFFFFu);
    pl_cell_t *aa = (pl_cell_t *)d->p;
    const char *fn = prolog_atom_name(fnid);
    if (!fn) fn = "?";
    if (strcmp(fn, "$VAR") == 0 && ar == 1) {
        pl_cell_t *n = pl_deref(&aa[0]);
        if ((int)n->v == DT_I) {
            long num = (long)n->i; int letter = (int)(num % 26); long suffix = num / 26;
            if (suffix == 0) fprintf(fp, "%c", 'A' + letter); else fprintf(fp, "%c%ld", 'A' + letter, suffix);
            return;
        }
    }
    if (strcmp(fn, "$VARNAME") == 0 && ar == 1) {
        pl_cell_t *n = pl_deref(&aa[0]);
        if ((int)n->v == DT_A || (int)n->v == DT_S) { const char *vn = plc_atom_text(n); fprintf(fp, "%s", vn ? vn : "_"); return; }
    }
    if (fnid == ATOM_DOT && ar == 2) {
        fprintf(fp, "[");
        plc_writeq(&aa[0], m);
        pl_cell_t *tail = pl_deref(&aa[1]);
        while ((int)tail->v == DT_PLREF && (int)(tail->slen >> 16) == ATOM_DOT && (int)(tail->slen & 0xFFFFu) == 2) {
            pl_cell_t *ta = (pl_cell_t *)tail->p;
            fprintf(fp, ","); plc_writeq(&ta[0], m); tail = pl_deref(&ta[1]);
        }
        if (!plc_is_nil(tail)) { fprintf(fp, "|"); plc_writeq(tail, m); }
        fprintf(fp, "]"); return;
    }
    if (ar == 1 && strcmp(fn, "{}") == 0) { fprintf(fp, "{"); plc_writeq(&aa[0], m); fprintf(fp, "}"); return; }
    struct { const char *name; int arity; int prec; int right_assoc; } ops[] = {
        {":-",2,1200,1}, {";",2,1100,1}, {"->",2,1050,1}, {",",2,1000,1},
        {"=",2,700,0},{"\\=",2,700,0},{"is",2,700,0},{"=:=",2,700,0},{"=\\=",2,700,0},
        {"<",2,700,0},{">",2,700,0},{"=<",2,700,0},{">=",2,700,0},
        {"==",2,700,0},{"\\==",2,700,0},{"@<",2,700,0},{"@>",2,700,0},{"@=<",2,700,0},{"@>=",2,700,0},
        {"=..",2,700,0},{"+",2,500,0},{"-",2,500,0},
        {"*",2,400,0},{"/",2,400,0},{"//",2,400,0},{"mod",2,400,0},{"rem",2,400,0},{"<<",2,400,0},{">>",2,400,0},
        {"**",2,200,1},{"^",2,200,1},{"-",1,200,0},{"\\+",1,900,0},{"not",1,900,0},
        {NULL,0,0,0}
    };
    int op_prec = -1, op_ra = 0;
    for (int i = 0; ops[i].name; i++) if (strcmp(fn, ops[i].name) == 0 && ar == ops[i].arity) { op_prec = ops[i].prec; op_ra = ops[i].right_assoc; break; }
    if (op_prec < 0) plc_user_op(fn, ar, &op_prec, &op_ra);
    if (op_prec >= 0) {
            if (ar == 2) {
                int lp = plc_operand_prec(&aa[0]), rp = plc_operand_prec(&aa[1]), my_prec = op_prec;
                int lneed = (lp > my_prec) || (lp == my_prec && op_ra);
                int rneed = (rp > my_prec) || (rp == my_prec && !op_ra);
                if (lneed) fprintf(fp, "(");
                plc_writeq(&aa[0], m);
                if (lneed) fprintf(fp, ")");
                if (isalpha((unsigned char)fn[0])) fprintf(fp, " %s ", fn); else fprintf(fp, "%s", fn);
                if (rneed) fprintf(fp, "(");
                plc_writeq(&aa[1], m);
                if (rneed) fprintf(fp, ")");
            } else {
                int ap = plc_operand_prec(&aa[0]);
                int aneed = (ap >= op_prec);
                if (isalpha((unsigned char)fn[0])) fprintf(fp, "%s ", fn); else fprintf(fp, "%s", fn);
                if (aneed) fprintf(fp, "(");
                plc_writeq(&aa[0], m);
                if (aneed) fprintf(fp, ")");
            }
            return;
            }
    plc_wt_atom(fp, fn, 1);
    fprintf(fp, "(");
    for (int i = 0; i < ar; i++) { if (i) fprintf(fp, ","); plc_writeq(&aa[i], m); }
    fprintf(fp, ")");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_write_canonical(pl_cell_t *c, plc_vmap *m)
{
    extern int ATOM_DOT;
    FILE *fp = m->fp;
    if (!c) { fprintf(fp, "'[]'"); return; }
    pl_cell_t *d = pl_deref(c);
    int tg = (int)d->v;
    if (pl_cell_unbound(d)) { fprintf(fp, "_G%d", plc_vindex(m, d)); return; }
    if (tg == DT_A || tg == DT_S) { plc_wt_atom(fp, plc_atom_text(d), 1); return; }
    if (tg == DT_I) { fprintf(fp, "%ld", (long)d->i); return; }
    if (tg == DT_R) {
        double fv = d->r;
        if (fv == (long)fv && fv >= -1e15 && fv <= 1e15) fprintf(fp, "%.1f", fv); else fprintf(fp, "%g", fv);
        return;
    }
    if (tg != DT_PLREF) { fprintf(fp, "_G%d", plc_vindex(m, d)); return; }
    int fnid = (int)(d->slen >> 16), ar = (int)(d->slen & 0xFFFFu);
    pl_cell_t *aa = (pl_cell_t *)d->p;
    const char *fn = prolog_atom_name(fnid);
    if (!fn) fn = "?";
    if (fnid == ATOM_DOT && ar == 2) {
        fprintf(fp, "[");
        plc_write_canonical(&aa[0], m);
        pl_cell_t *tail = pl_deref(&aa[1]);
        while ((int)tail->v == DT_PLREF && (int)(tail->slen >> 16) == ATOM_DOT && (int)(tail->slen & 0xFFFFu) == 2) {
            pl_cell_t *ta = (pl_cell_t *)tail->p;
            fprintf(fp, ","); plc_write_canonical(&ta[0], m); tail = pl_deref(&ta[1]);
        }
        if (!plc_is_nil(tail)) { fprintf(fp, "|"); plc_write_canonical(tail, m); }
        fprintf(fp, "]"); return;
    }
    plc_wt_atom(fp, fn, 1);
    fprintf(fp, "(");
    for (int i = 0; i < ar; i++) { if (i) fprintf(fp, ","); plc_write_canonical(&aa[i], m); }
    fprintf(fp, ")");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_atoms_ready(void) { extern int ATOM_DOT; extern void prolog_atom_init(void); if (ATOM_DOT <= 0) prolog_atom_init(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_write_cell(void *cell)
{
    plc_atoms_ready();
    plc_vmap m; m.n = 0; m.fp = plc_out();
    plc_write((pl_cell_t *)cell, &m);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_write_cell_fp(void *cell, FILE *fp)
{
    plc_atoms_ready();
    plc_vmap m; m.n = 0; m.fp = fp ? fp : plc_out();
    plc_write((pl_cell_t *)cell, &m);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_writeq_cell(void *cell)
{
    plc_atoms_ready();
    plc_vmap m; m.n = 0; m.fp = plc_out();
    plc_writeq((pl_cell_t *)cell, &m);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_write_canonical_cell(void *cell)
{
    plc_atoms_ready();
    plc_vmap m; m.n = 0; m.fp = plc_out();
    plc_write_canonical((pl_cell_t *)cell, &m);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_display_cell(void *cell)
{
    plc_atoms_ready();
    plc_vmap m; m.n = 0; m.fp = plc_out();
    plc_wt((pl_cell_t *)cell, 0, 1, 0, 0, 0, &m);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_opt_is_true(pl_cell_t *o)
{
    if (!o) return 0;
    o = pl_deref(o);
    if ((int)o->v == DT_PLREF && (int)(o->slen & 0xFFFFu) == 1) {
        pl_cell_t *a = pl_deref((pl_cell_t *)o->p);
        return ((int)a->v == DT_A || (int)a->v == DT_S) && strcmp(plc_atom_text(a), "true") == 0;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_write_term_cell(void *term_cell, void *opts_cell)
{
    plc_atoms_ready();
    plc_vmap m; m.n = 0; m.fp = plc_out();
    int quoted = 0, ignore_ops = 0, numbervars = 0; long max_depth = 0;
    pl_cell_t *lst = opts_cell ? pl_deref((pl_cell_t *)opts_cell) : (pl_cell_t *)0;
    while (lst && (int)lst->v == DT_PLREF && (int)(lst->slen & 0xFFFFu) == 2) {
        pl_cell_t *pair = (pl_cell_t *)lst->p;
        pl_cell_t *o = pl_deref(&pair[0]);
        if ((int)o->v == DT_PLREF && (int)(o->slen & 0xFFFFu) == 1) {
            const char *on = prolog_atom_name((int)(o->slen >> 16));
            pl_cell_t *oa = (pl_cell_t *)o->p;
            if (on && !strcmp(on, "quoted")) quoted = plc_opt_is_true(o);
            else if (on && !strcmp(on, "ignore_ops")) ignore_ops = plc_opt_is_true(o);
            else if (on && !strcmp(on, "numbervars")) numbervars = plc_opt_is_true(o);
            else if (on && !strcmp(on, "max_depth")) { pl_cell_t *a = pl_deref(&oa[0]); if ((int)a->v == DT_I) max_depth = (long)a->i; }
        }
        lst = pl_deref(&pair[1]);
    }
    plc_wt((pl_cell_t *)term_cell, quoted, ignore_ops, numbervars, max_depth, 0, &m);
}
extern const char *prolog_atom_name(int id);
extern int    rt_last_ok(void);
extern long   rt_arith(int lk, long li, const char *ls, int rk, long ri, const char *rs, const char *op);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_atom_id_of(pl_cell_t *d)
{
    if ((int)d->v == DT_A) return (int)d->i;
    if ((int)d->v == DT_S) { extern int prolog_atom_intern(const char *); return prolog_atom_intern(d->s ? d->s : ""); }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_is_atomlike(pl_cell_t *d) { return (int)d->v == DT_A || (int)d->v == DT_S; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static pl_cell_t plc_atom_id_cell(int id) { const char *n = prolog_atom_name(id); pl_cell_t c; c.v = DT_S; c.slen = (uint32_t)(n ? strlen(n) : 0); c.s = n ? n : ""; return c; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_cell_is_ground(pl_cell_t *c)
{
    pl_cell_t *d = c ? pl_deref(c) : (pl_cell_t *)0;
    if (!d || pl_cell_unbound(d)) return 0;
    if ((int)d->v == DT_PLREF) {
        int ar = (int)(d->slen & 0xFFFFu); pl_cell_t *aa = (pl_cell_t *)d->p;
        if (ar > 0 && !aa) return 0;
        for (int i = 0; i < ar; i++) if (!plc_cell_is_ground(&aa[i])) return 0;
    }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_cell_is_proper_list(pl_cell_t *c)
{
    extern int ATOM_DOT, ATOM_NIL;
    pl_cell_t *d = c ? pl_deref(c) : (pl_cell_t *)0;
    while (d) {
        if (plc_is_atomlike(d) && plc_atom_id_of(d) == ATOM_NIL) return 1;
        if ((int)d->v == DT_PLREF && (int)(d->slen >> 16) == ATOM_DOT && (int)(d->slen & 0xFFFFu) == 2 && d->p) { d = pl_deref(&((pl_cell_t *)d->p)[1]); continue; }
        return 0;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_type_test_cell(void *cell_term, const char *fn)
{
    pl_cell_t *d = cell_term ? pl_deref((pl_cell_t *)cell_term) : (pl_cell_t *)0;
    int isvar = (!d || pl_cell_unbound(d));
    int isatom = (d && !isvar && plc_is_atomlike(d));
    int isint = (d && !isvar && (int)d->v == DT_I);
    int isfloat = (d && !isvar && (int)d->v == DT_R);
    int iscomp = (d && !isvar && (int)d->v == DT_PLREF);
    if (!fn) return 0;
    if (strcmp(fn, "var")      == 0) return  isvar ? 1 : 0;
    if (strcmp(fn, "nonvar")   == 0) return !isvar ? 1 : 0;
    if (strcmp(fn, "atom")     == 0) return isatom ? 1 : 0;
    if (strcmp(fn, "integer")  == 0) return isint ? 1 : 0;
    if (strcmp(fn, "float")    == 0) return isfloat ? 1 : 0;
    if (strcmp(fn, "number")   == 0) return (isint || isfloat) ? 1 : 0;
    if (strcmp(fn, "atomic")   == 0) return (isatom || isint || isfloat) ? 1 : 0;
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
        if ((int)d0->v == DT_PLREF)   { nameV = plc_atom_id_cell((int)(d0->slen >> 16)); arityV = pl_make_int((int64_t)(d0->slen & 0xFFFFu)); }
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
    if (n < 1 || n > (long)(dT->slen & 0xFFFFu)) { return 0; }
    if (!plc_unify_cells_cx((pl_cell_t *)arg_cell, &((pl_cell_t *)dT->p)[n - 1], cx)) { return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_univ_cell(void *t0_cell, void *list_cell, pl_tr_ctx_t *cx)
{
    extern int ATOM_DOT;
    pl_cell_t *d0 = t0_cell ? pl_deref((pl_cell_t *)t0_cell) : (pl_cell_t *)0;
    if (d0 && !pl_cell_unbound(d0)) {
        pl_cell_t lst = plc_atom_id_cell(prolog_atom_intern("[]"));
        if ((int)d0->v == DT_PLREF) {
            int ar = (int)(d0->slen & 0xFFFFu); pl_cell_t *aa = (pl_cell_t *)d0->p;
            for (int i = ar - 1; i >= 0; i--) {
                pl_cell_t *c = (pl_cell_t *)PL_CELL_ALLOC(2 * sizeof(pl_cell_t));
                c[0] = aa[i]; c[1] = lst;
                lst = pl_make_compound(ATOM_DOT, 2, c);
            }
            pl_cell_t *c = (pl_cell_t *)PL_CELL_ALLOC(2 * sizeof(pl_cell_t));
            c[0] = plc_atom_id_cell((int)(d0->slen >> 16)); c[1] = lst;
            lst = pl_make_compound(ATOM_DOT, 2, c);
        } else {
            pl_cell_t *c = (pl_cell_t *)PL_CELL_ALLOC(2 * sizeof(pl_cell_t));
            c[0] = *d0; c[1] = lst;
            lst = pl_make_compound(ATOM_DOT, 2, c);
        }
        if (!plc_unify_cells_cx((pl_cell_t *)list_cell, &lst, cx)) { return 0; }
        return 1;
    }
    pl_cell_t *cur = list_cell ? pl_deref((pl_cell_t *)list_cell) : (pl_cell_t *)0;
    pl_cell_t *elems[64]; int ne = 0;
    while (cur && (int)cur->v == DT_PLREF && (int)(cur->slen >> 16) == ATOM_DOT && (int)(cur->slen & 0xFFFFu) == 2 && cur->p) {
        if (ne >= 64) break;
        elems[ne++] = pl_deref(&((pl_cell_t *)cur->p)[0]);
        cur = pl_deref(&((pl_cell_t *)cur->p)[1]);
    }
    if (ne == 0) { return 0; }
    pl_cell_t built;
    if (ne == 1) { built = *elems[0]; }
    else {
        pl_cell_t *h = elems[0];
        if (!h || !plc_is_atomlike(h)) { return 0; }
        pl_cell_t *args = (pl_cell_t *)PL_CELL_ALLOC((size_t)(ne - 1) * sizeof(pl_cell_t));
        for (int i = 1; i < ne; i++) args[i - 1] = *elems[i];
        built = pl_make_compound(plc_atom_id_of(h), ne - 1, args);
    }
    if (!plc_unify_cells_cx((pl_cell_t *)t0_cell, &built, cx)) { return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_succ_plus_cell(long arity, void *a_cell, void *b_cell, void *c_cell, pl_tr_ctx_t *cx)
{
    pl_cell_t *da = a_cell ? pl_deref((pl_cell_t *)a_cell) : (pl_cell_t *)0;
    pl_cell_t *db = b_cell ? pl_deref((pl_cell_t *)b_cell) : (pl_cell_t *)0;
    pl_cell_t *dc = c_cell ? pl_deref((pl_cell_t *)c_cell) : (pl_cell_t *)0;
    if (arity == 2) {
        if (da && (int)da->v == DT_I) {
            if (da->i < 0) return 0;
            pl_cell_t v = pl_make_int(da->i + 1);
            if (!plc_unify_cells_cx((pl_cell_t *)b_cell, &v, cx)) { return 0; }
            return 1;
        }
        if (db && (int)db->v == DT_I) {
            if (db->i <= 0) return 0;
            pl_cell_t v = pl_make_int(db->i - 1);
            if (!plc_unify_cells_cx((pl_cell_t *)a_cell, &v, cx)) { return 0; }
            return 1;
        }
        return 0;
    }
    if (arity == 3) {
        int va = (da && (int)da->v == DT_I), vb = (db && (int)db->v == DT_I), vc = (dc && (int)dc->v == DT_I);
        int ok = 0; pl_cell_t v;
        if (va && vb)      { v = pl_make_int(da->i + db->i); ok = plc_unify_cells_cx((pl_cell_t *)c_cell, &v, cx); }
        else if (va && vc) { v = pl_make_int(dc->i - da->i); ok = plc_unify_cells_cx((pl_cell_t *)b_cell, &v, cx); }
        else if (vb && vc) { v = pl_make_int(dc->i - db->i); ok = plc_unify_cells_cx((pl_cell_t *)a_cell, &v, cx); }
        if (!ok) { return 0; }
        return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *plc_atom_op_text(pl_cell_t *t, char *buf, size_t bufsz)
{
    if (!t) return (const char *)0;
    pl_cell_t *d = pl_deref(t);
    if ((int)d->v == DT_A || (int)d->v == DT_S) return plc_atom_text(d);
    if ((int)d->v == DT_I) { snprintf(buf, bufsz, "%ld", (long)d->i); return buf; }
    if ((int)d->v == DT_R) { snprintf(buf, bufsz, "%g", d->r); return buf; }
    return (const char *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static pl_cell_t plc_make_atom_cell(const char *name)
{
    int id = prolog_atom_intern(name ? name : "");
    const char *nm = prolog_atom_name(id);
    pl_cell_t c; c.v = DT_S; c.slen = (uint32_t)(nm ? strlen(nm) : 0); c.s = nm ? nm : ""; return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_unify_into_cell(pl_cell_t *dst, pl_cell_t val)
{
    pl_cell_t tmp = val;
    return pl_unify(dst, &tmp);
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
        if (!plc_unify_into_cell_cx((pl_cell_t *)a1_cell, pl_make_int((int64_t)strlen(s)), cx)) { return 0; }
        return 1;
    }
    if (!strcmp(fn, "atom_concat")) {
        const char *s0 = plc_atom_op_text(t0, buf0, sizeof buf0);
        const char *s1 = plc_atom_op_text(t1, buf1, sizeof buf1);
        if (!s0 || !s1) { return 0; }
        size_t l0 = strlen(s0), l1 = strlen(s1);
        char *cat = (char *)rt_ws_alloc(l0 + l1 + 1); memcpy(cat, s0, l0); memcpy(cat + l0, s1, l1); cat[l0 + l1] = '\0';
        if (!plc_unify_into_cell_cx((pl_cell_t *)a2_cell, plc_make_atom_cell(cat), cx)) { return 0; }
        return 1;
    }
    if (!strcmp(fn, "upcase_atom") || !strcmp(fn, "downcase_atom")) {
        const char *s = plc_atom_op_text(t0, buf0, sizeof buf0);
        if (!s) { return 0; }
        size_t n = strlen(s); char *out = (char *)rt_ws_alloc(n + 1);
        int up = (!strcmp(fn, "upcase_atom"));
        for (size_t i = 0; i < n; i++) out[i] = up ? (char)toupper((unsigned char)s[i]) : (char)tolower((unsigned char)s[i]);
        out[n] = '\0';
        if (!plc_unify_into_cell_cx((pl_cell_t *)a1_cell, plc_make_atom_cell(out), cx)) { return 0; }
        return 1;
    }
    int as_codes = (!strcmp(fn, "atom_codes"));
    if (!strcmp(fn, "atom_chars") || as_codes) {
        if (t0 && !pl_cell_unbound(t0)) {
            const char *s = plc_atom_op_text(t0, buf0, sizeof buf0);
            if (!s) { return 0; }
            size_t n = strlen(s);
            pl_cell_t lst = plc_make_atom_cell("[]");
            for (size_t i = n; i > 0; i--) {
                unsigned char ch = (unsigned char)s[i - 1];
                pl_cell_t el;
                if (as_codes) el = pl_make_int((int64_t)ch);
                else { char cs[2] = { (char)ch, '\0' }; el = plc_make_atom_cell(cs); }
                pl_cell_t *pair = (pl_cell_t *)PL_CELL_ALLOC(2 * sizeof(pl_cell_t));
                pair[0] = el; pair[1] = lst;
                lst = pl_make_compound(ATOM_DOT, 2, pair);
            }
            if (!plc_unify_into_cell_cx((pl_cell_t *)a1_cell, lst, cx)) { return 0; }
            return 1;
        }
        pl_cell_t *cur = t1;
        char out[512]; size_t oi = 0;
        while (cur && (int)cur->v == DT_PLREF && (int)(cur->slen >> 16) == ATOM_DOT && (int)(cur->slen & 0xFFFFu) == 2) {
            pl_cell_t *pr = (pl_cell_t *)cur->p;
            pl_cell_t *el = pl_deref(&pr[0]);
            if (oi >= sizeof(out) - 1) break;
            if (as_codes) { if ((int)el->v != DT_I) { return 0; } out[oi++] = (char)el->i; }
            else { if ((int)el->v != DT_A && (int)el->v != DT_S) { return 0; } const char *cn = plc_atom_text(el); out[oi++] = cn ? cn[0] : '?'; }
            cur = pl_deref(&pr[1]);
        }
        out[oi] = '\0';
        if (!plc_unify_into_cell_cx((pl_cell_t *)a0_cell, plc_make_atom_cell(out), cx)) { return 0; }
        return 1;
    }
    if (!strcmp(fn, "string_length")) {
        const char *s = plc_atom_op_text(t0, buf0, sizeof buf0);
        if (!s) { return 0; }
        if (!plc_unify_into_cell_cx((pl_cell_t *)a1_cell, pl_make_int((int64_t)strlen(s)), cx)) { return 0; }
        return 1;
    }
    if (!strcmp(fn, "string_upper") || !strcmp(fn, "string_lower")) {
        const char *s = plc_atom_op_text(t0, buf0, sizeof buf0);
        if (!s) { return 0; }
        size_t n = strlen(s); char *out = (char *)rt_ws_alloc(n + 1);
        int up = (!strcmp(fn, "string_upper"));
        for (size_t i = 0; i < n; i++) out[i] = up ? (char)toupper((unsigned char)s[i]) : (char)tolower((unsigned char)s[i]);
        out[n] = '\0';
        if (!plc_unify_into_cell_cx((pl_cell_t *)a1_cell, plc_make_atom_cell(out), cx)) { return 0; }
        return 1;
    }
    if (!strcmp(fn, "atom_string") || !strcmp(fn, "string_to_atom")) {
        if (t0 && !pl_cell_unbound(t0)) {
            const char *s = plc_atom_op_text(t0, buf0, sizeof buf0);
            if (!s) { return 0; }
            if (!plc_unify_into_cell_cx((pl_cell_t *)a1_cell, plc_make_atom_cell(s), cx)) { return 0; }
            return 1;
        }
        const char *s = plc_atom_op_text(t1, buf1, sizeof buf1);
        if (!s) { return 0; }
        if (!plc_unify_into_cell_cx((pl_cell_t *)a0_cell, plc_make_atom_cell(s), cx)) { return 0; }
        return 1;
    }
    if (!strcmp(fn, "number_string")) {
        if (t0 && !pl_cell_unbound(t0)) {
            const char *s = plc_atom_op_text(t0, buf0, sizeof buf0);
            if (!s) { return 0; }
            if (!plc_unify_into_cell_cx((pl_cell_t *)a1_cell, plc_make_atom_cell(s), cx)) { return 0; }
            return 1;
        }
        const char *s = plc_atom_op_text(t1, buf1, sizeof buf1);
        if (!s) { return 0; }
        char *end; long iv = strtol(s, &end, 10);
        if (*end == '\0') { if (!plc_unify_into_cell_cx((pl_cell_t *)a0_cell, pl_make_int(iv), cx)) { return 0; } return 1; }
        double dv = strtod(s, &end);
        if (*end == '\0') { if (!plc_unify_into_cell_cx((pl_cell_t *)a0_cell, pl_make_float(dv), cx)) { return 0; } return 1; }
        return 0;
    }
    if (!strcmp(fn, "atom_number")) {
        if (t0 && !pl_cell_unbound(t0)) {
            const char *s = plc_atom_op_text(t0, buf0, sizeof buf0);
            if (!s) { return 0; }
            char *end; long iv = strtol(s, &end, 10);
            if (*end == '\0') { if (!plc_unify_into_cell_cx((pl_cell_t *)a1_cell, pl_make_int(iv), cx)) { return 0; } return 1; }
            double dv = strtod(s, &end);
            if (*end == '\0') { if (!plc_unify_into_cell_cx((pl_cell_t *)a1_cell, pl_make_float(dv), cx)) { return 0; } return 1; }
            return 0;
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
        char *cat = (char *)rt_ws_alloc(l0 + l1 + 1); memcpy(cat, s0, l0); memcpy(cat + l0, s1, l1); cat[l0 + l1] = '\0';
        if (!plc_unify_into_cell_cx((pl_cell_t *)a2_cell, plc_make_atom_cell(cat), cx)) { return 0; }
        return 1;
    }
    if (!strcmp(fn, "atomic_list_concat") || !strcmp(fn, "concat_atom")) {
        const char *sep = (t1 && ((int)t1->v == DT_A || (int)t1->v == DT_S)) ? plc_atom_text(t1) : "";
        void *result_cell = t2 ? a2_cell : a1_cell;
        pl_cell_t *lst = t0;
        char *out = (char *)rt_ws_alloc(4096); size_t oi = 0;
        int first = 1;
        while (lst && (int)lst->v == DT_PLREF && (int)(lst->slen & 0xFFFFu) == 2) {
            pl_cell_t *pr = (pl_cell_t *)lst->p;
            pl_cell_t *el = pl_deref(&pr[0]);
            if (!first && sep && sep[0]) { size_t sl = strlen(sep); if (oi + sl < 4095) { memcpy(out + oi, sep, sl); oi += sl; } }
            first = 0;
            const char *es = plc_atom_op_text(el, buf0, sizeof buf0);
            if (!es) { return 0; }
            size_t el_len = strlen(es); if (oi + el_len < 4095) { memcpy(out + oi, es, el_len); oi += el_len; }
            lst = pl_deref(&pr[1]);
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
void rt_pl_format_cell(const char *fmt, void *list_cell)
{
    plc_atoms_ready();
    if (!fmt) return;
    plc_vmap vm; vm.n = 0; vm.fp = plc_out();
    FILE *fd = vm.fp;
    pl_cell_t *args = (pl_cell_t *)list_cell;
    for (const char *p = fmt; *p; p++) {
        if (*p != '~') { fputc(*p, fd); continue; }
        p++;
        int have_n = 0; long nval = 0;
        if (*p == '*') { pl_cell_t *h = plc_fmt_next_arg(&args); if (h && pl_is_int(h)) { nval = (long)pl_int_val(h); have_n = 1; } p++; }
        else { while (*p >= '0' && *p <= '9') { nval = nval * 10 + (*p - '0'); have_n = 1; p++; } }
        if (*p == 'w' || *p == 'a' || *p == 'p') { pl_cell_t *h = plc_fmt_next_arg(&args); if (h) plc_write(h, &vm); }
        else if (*p == 'q') { pl_cell_t *h = plc_fmt_next_arg(&args); if (h) plc_writeq(h, &vm); }
        else if (*p == 'd') { pl_cell_t *h = plc_fmt_next_arg(&args); if (h && pl_is_int(h)) fprintf(fd, "%ld", (long)pl_int_val(h)); }
        else if (*p == 'e') { pl_cell_t *h = plc_fmt_next_arg(&args); if (h) { double d = pl_is_int(h) ? (double)pl_int_val(h) : pl_float_val(h); fprintf(fd, "%e", d); } }
        else if (*p == 'g') { pl_cell_t *h = plc_fmt_next_arg(&args); if (h) { double d = pl_is_int(h) ? (double)pl_int_val(h) : pl_float_val(h); fprintf(fd, "%g", d); } }
        else if (*p == 'f') { pl_cell_t *h = plc_fmt_next_arg(&args); if (h) { double d = pl_is_int(h) ? (double)pl_int_val(h) : pl_float_val(h); fprintf(fd, "%.*f", have_n ? (int)nval : 6, d); } }
        else if (*p == 'r' || *p == 'R') { pl_cell_t *h = plc_fmt_next_arg(&args); int base = have_n ? (int)nval : 8; if (h && pl_is_int(h) && base >= 2 && base <= 36) { long iv = (long)pl_int_val(h); char buf[72]; int bi = 0; unsigned long u = (iv < 0) ? (unsigned long)(-iv) : (unsigned long)iv; const char *dig = (*p == 'R') ? "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ" : "0123456789abcdefghijklmnopqrstuvwxyz"; if (u == 0) buf[bi++] = '0'; while (u) { buf[bi++] = dig[u % (unsigned long)base]; u /= (unsigned long)base; } if (iv < 0) fputc('-', fd); while (bi) fputc(buf[--bi], fd); } }
        else if (*p == 'c') { pl_cell_t *h = plc_fmt_next_arg(&args); if (h && pl_is_int(h)) { int rep = have_n ? (int)nval : 1; for (int i = 0; i < rep; i++) fputc((int)pl_int_val(h), fd); } }
        else if (*p == 's') { pl_cell_t *h = plc_fmt_next_arg(&args); pl_cell_t *lst = h ? pl_deref(h) : (pl_cell_t *)0; while (lst && (int)lst->v == DT_PLREF && pl_arity(lst) == 2) { pl_cell_t *aa = (pl_cell_t *)pl_compound_heap(lst); pl_cell_t *e = pl_deref(&aa[0]); if (e && pl_is_int(e)) fputc((int)pl_int_val(e), fd); lst = pl_deref(&aa[1]); } }
        else if (*p == 'i') { plc_fmt_next_arg(&args); }
        else if (*p == 'n' || *p == 'N') { int rep = have_n ? (int)nval : 1; for (int i = 0; i < rep; i++) fputc('\n', fd); }
        else if (*p == '~') { fputc('~', fd); }
    }
}
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
    if ((int)td->v != DT_A && (int)td->v != DT_S) { return 0; }
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
typedef struct { pl_cell_t *seen[256]; int idx[256]; int n; int next; } pl_vord_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_pl_vord_of(pl_vord_t *m, pl_cell_t *d) {
    for (int i = 0; i < m->n; i++) if (m->seen[i] == d) return m->idx[i];
    int k = m->next++;
    if (m->n < 256) { m->seen[m->n] = d; m->idx[m->n] = k; m->n++; }
    return k;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rt_pl_vord_walk(pl_cell_t *c, pl_vord_t *m) {
    pl_cell_t *d = pl_deref(c);
    int t = (int)d->v;
    if (pl_cell_unbound(d)) { rt_pl_vord_of(m, d); return; }
    if (t == DT_PLREF) { int ar = (int)(d->slen & 0xFFFFu); pl_cell_t *aa = (pl_cell_t *)d->p; for (int i = 0; i < ar; i++) rt_pl_vord_walk(&aa[i], m); return; }
    if (t != DT_I && t != DT_A && t != DT_S && t != DT_R) m->next++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_pl_cell_class(pl_cell_t *d) {
    int t = (int)d->v;
    if (pl_cell_unbound(d)) return 0;
    if (t == DT_I || t == DT_R) return 1;
    if (t == DT_A || t == DT_S) return 2;
    if (t == DT_PLREF) return 3;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *rt_pl_cell_name(pl_cell_t *d) {
    if ((int)d->v == DT_S) return d->s ? d->s : "";
    const char *n = prolog_atom_name((int)d->i);
    return n ? n : "";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_pl_cell_compare(pl_cell_t *ca, pl_cell_t *cb, pl_vord_t *m) {
    pl_cell_t *a = pl_deref(ca), *b = pl_deref(cb);
    int cla = rt_pl_cell_class(a), clb = rt_pl_cell_class(b);
    if (cla != clb) return cla < clb ? -1 : 1;
    if (cla == 0) { int ia = rt_pl_vord_of(m, a), ib = rt_pl_vord_of(m, b); return ia == ib ? 0 : (ia < ib ? -1 : 1); }
    if (cla == 1) { double x = ((int)a->v == DT_I) ? (double)a->i : a->r, y = ((int)b->v == DT_I) ? (double)b->i : b->r; return x < y ? -1 : (x > y ? 1 : 0); }
    if (cla == 2) { int c = strcmp(rt_pl_cell_name(a), rt_pl_cell_name(b)); return c < 0 ? -1 : (c > 0 ? 1 : 0); }
    int ara = (int)(a->slen & 0xFFFFu), arb = (int)(b->slen & 0xFFFFu);
    if (ara != arb) return ara < arb ? -1 : 1;
    const char *na = prolog_atom_name((int)(a->slen >> 16)), *nb = prolog_atom_name((int)(b->slen >> 16));
    int c = strcmp(na ? na : "", nb ? nb : ""); if (c) return c < 0 ? -1 : 1;
    pl_cell_t *aa = (pl_cell_t *)a->p, *bb = (pl_cell_t *)b->p;
    for (int i = 0; i < ara; i++) { int r = rt_pl_cell_compare(&aa[i], &bb[i], m); if (r) return r; }
    return 0;
}
int rt_pl_atop_cell(int op, void *a_cell, void *b_cell)
{
    pl_vord_t m; m.n = 0; m.next = 0;
    rt_pl_vord_walk((pl_cell_t *)a_cell, &m);
    rt_pl_vord_walk((pl_cell_t *)b_cell, &m);
    int c = rt_pl_cell_compare((pl_cell_t *)a_cell, (pl_cell_t *)b_cell, &m);
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
    pl_vord_t m; m.n = 0; m.next = 0;
    rt_pl_vord_walk((pl_cell_t *)a_cell, &m);
    rt_pl_vord_walk((pl_cell_t *)b_cell, &m);
    int c = rt_pl_cell_compare((pl_cell_t *)a_cell, (pl_cell_t *)b_cell, &m);
    const char *nm = (c < 0) ? "<" : (c > 0) ? ">" : "=";
    const char *an = prolog_atom_name(prolog_atom_intern(nm));
    pl_cell_t ord; ord.v = DT_S; ord.slen = (uint32_t)(an ? strlen(an) : 0); ord.s = an ? an : nm;
    if (!plc_unify_cells_cx((pl_cell_t *)order_cell, &ord, cx)) { return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rt_pl_tv_walk(pl_cell_t *c, pl_cell_t **pool, int *pool_n, int cap) {
    if (!c) return;
    pl_cell_t *d = pl_deref(c);
    if (pl_cell_unbound(d)) { for (int i = 0; i < *pool_n; i++) if (pool[i] == d) return; if (*pool_n < cap) pool[(*pool_n)++] = d; return; }
    if ((int)d->v == (int)DT_PLREF) { int ar = (int)(d->slen & 0xFFFFu); pl_cell_t *aa = (pl_cell_t *)d->p; for (int i = 0; i < ar; i++) rt_pl_tv_walk(&aa[i], pool, pool_n, cap); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_term_variables_cell(void *term_cell, void *vars_cell, void *tail_cell, pl_tr_ctx_t *cx)
{
    pl_cell_t *pool[8192]; int pn = 0;
    rt_pl_tv_walk((pl_cell_t *)term_cell, pool, &pn, 8192);
    int dot_id = prolog_atom_intern(".");
    pl_cell_t nil; { const char *nm = prolog_atom_name(prolog_atom_intern("[]")); nil.v = DT_S; nil.slen = (uint32_t)(nm ? strlen(nm) : 0); nil.s = nm ? nm : "[]"; }
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
    pl_cell_t *svars[8192]; int sn = 0;
    rt_pl_tv_walk(s, svars, &sn, 8192);
    int unified = pl_unify(g, s);
    int bound_spec = 0;
    if (unified) for (int i = 0; i < sn; i++) if (!pl_cell_unbound(pl_deref(svars[i]))) { bound_spec = 1; break; }
    return unified && !bound_spec;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_sort_cell(int do_msort, void *list_cell, void *result_cell, pl_tr_ctx_t *cx)
{
    pl_cell_t *elems[4096]; int n = 0;
    int dot_id = prolog_atom_intern(".");
    pl_cell_t *cur = pl_deref((pl_cell_t *)list_cell);
    while (cur && (int)cur->v == DT_PLREF && (int)(cur->slen >> 16) == dot_id && (int)(cur->slen & 0xFFFFu) == 2 && n < 4096) {
        pl_cell_t *aa = (pl_cell_t *)cur->p;
        elems[n++] = &aa[0];
        cur = pl_deref(&aa[1]);
    }
    pl_vord_t vm; vm.n = 0; vm.next = 0;
    for (int i = 0; i < n; i++) rt_pl_vord_walk(elems[i], &vm);
    for (int i = 1; i < n; i++) {
        pl_cell_t *key = elems[i]; int j = i - 1;
        while (j >= 0 && rt_pl_cell_compare(elems[j], key, &vm) > 0) { elems[j + 1] = elems[j]; j--; }
        elems[j + 1] = key;
    }
    int m = 0; int out_idx[4096];
    for (int i = 0; i < n; i++) {
        if (!do_msort && m > 0 && rt_pl_cell_compare(elems[out_idx[m - 1]], elems[i], &vm) == 0) continue;
        out_idx[m++] = i;
    }
    pl_cell_t nil; { const char *nm = prolog_atom_name(prolog_atom_intern("[]")); nil.v = DT_S; nil.slen = (uint32_t)(nm ? strlen(nm) : 0); nil.s = nm ? nm : "[]"; }
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
static int plc_pairs_extract(void *list_cell, pl_cell_t **elems, int cap, int dot_id, int dash_id) {
    pl_cell_t *cur = list_cell ? pl_deref((pl_cell_t *)list_cell) : (pl_cell_t *)0;
    int n = 0;
    while (cur && (int)cur->v == DT_PLREF && (int)(cur->slen >> 16) == dot_id && (int)(cur->slen & 0xFFFFu) == 2 && n < cap) {
        pl_cell_t *aa = (pl_cell_t *)cur->p;
        pl_cell_t *pr = pl_deref(&aa[0]);
        if (!(pr && (int)pr->v == DT_PLREF && (int)(pr->slen >> 16) == dash_id && (int)(pr->slen & 0xFFFFu) == 2)) return -1;
        elems[n++] = pr;
        cur = pl_deref(&aa[1]);
    }
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_plain_list_extract(void *list_cell, pl_cell_t **elems, int cap, int dot_id) {
    pl_cell_t *cur = list_cell ? pl_deref((pl_cell_t *)list_cell) : (pl_cell_t *)0;
    int n = 0;
    while (cur && (int)cur->v == DT_PLREF && (int)(cur->slen >> 16) == dot_id && (int)(cur->slen & 0xFFFFu) == 2 && n < cap) {
        pl_cell_t *aa = (pl_cell_t *)cur->p;
        elems[n++] = pl_deref(&aa[0]);
        cur = pl_deref(&aa[1]);
    }
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static pl_cell_t plc_nil_cell(void) { const char *nm = prolog_atom_name(prolog_atom_intern("[]")); pl_cell_t c; c.v = DT_S; c.slen = (uint32_t)(nm ? strlen(nm) : 0); c.s = nm ? nm : "[]"; return c; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static pl_cell_t plc_cons(int fid, pl_cell_t head, pl_cell_t tail) { pl_cell_t *blk = (pl_cell_t *)PL_CELL_ALLOC(2 * sizeof(pl_cell_t)); blk[0] = head; blk[1] = tail; return pl_make_compound(fid, 2, blk); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_bag_prep_cell(int is_setof, void *list_cell, void *result_cell)
{
    int dot_id = prolog_atom_intern("."); int dash_id = prolog_atom_intern("-");
    pl_cell_t *elems[4096];
    int n = plc_pairs_extract(list_cell, elems, 4096, dot_id, dash_id);
    if (n < 0) return 0;
    pl_vord_t vm; vm.n = 0; vm.next = 0;
    for (int i = 0; i < n; i++) rt_pl_vord_walk((pl_cell_t *)elems[i]->p, &vm);
    for (int i = 1; i < n; i++) {
        pl_cell_t *key = elems[i]; int j = i - 1;
        if (is_setof) { while (j >= 0 && rt_pl_cell_compare(elems[j], key, &vm) > 0) { elems[j + 1] = elems[j]; j--; } }
        else { while (j >= 0 && rt_pl_cell_compare((pl_cell_t *)elems[j]->p, (pl_cell_t *)key->p, &vm) > 0) { elems[j + 1] = elems[j]; j--; } }
        elems[j + 1] = key;
    }
    int m = 0; int out_idx[4096];
    for (int i = 0; i < n; i++) {
        if (is_setof && m > 0 && rt_pl_cell_compare(elems[out_idx[m - 1]], elems[i], &vm) == 0) continue;
        out_idx[m++] = i;
    }
    pl_cell_t nil; { const char *nm = prolog_atom_name(prolog_atom_intern("[]")); nil.v = DT_S; nil.slen = (uint32_t)(nm ? strlen(nm) : 0); nil.s = nm ? nm : "[]"; }
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
    pl_cell_t *elems[4096];
    int n = plc_pairs_extract(list_cell, elems, 4096, dot_id, dash_id);
    if (n < 0) return 0;
    pl_vord_t vm; vm.n = 0; vm.next = 0;
    for (int i = 0; i < n; i++) rt_pl_vord_walk((pl_cell_t *)elems[i]->p, &vm);
    for (int i = 1; i < n; i++) {
        pl_cell_t *key = elems[i]; pl_cell_t *kkey = (pl_cell_t *)key->p; int j = i - 1;
        while (j >= 0 && rt_pl_cell_compare((pl_cell_t *)elems[j]->p, kkey, &vm) > 0) { elems[j + 1] = elems[j]; j--; }
        elems[j + 1] = key;
    }
    pl_cell_t nil; { const char *nm = prolog_atom_name(prolog_atom_intern("[]")); nil.v = DT_S; nil.slen = (uint32_t)(nm ? strlen(nm) : 0); nil.s = nm ? nm : "[]"; }
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
int rt_pl_group_pairs_by_key_cell(void *list_cell, void *result_cell)
{
    int dot_id = prolog_atom_intern("."); int dash_id = prolog_atom_intern("-");
    pl_cell_t *elems[4096];
    int n = plc_pairs_extract(list_cell, elems, 4096, dot_id, dash_id);
    if (n < 0) return 0;
    pl_vord_t vm; vm.n = 0; vm.next = 0;
    for (int i = 0; i < n; i++) rt_pl_vord_walk((pl_cell_t *)elems[i]->p, &vm);
    pl_cell_t groups[4096]; int ng = 0;
    pl_cell_t nil = plc_nil_cell();
    int i = 0;
    while (i < n) {
        pl_cell_t *key = pl_deref(&((pl_cell_t *)elems[i]->p)[0]);
        pl_cell_t *vlist[4096]; int nv = 0; int j = i;
        while (j < n && rt_pl_cell_compare(pl_deref(&((pl_cell_t *)elems[j]->p)[0]), key, &vm) == 0 && nv < 4096) { vlist[nv++] = &((pl_cell_t *)elems[j]->p)[1]; j++; }
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
        pl_cell_t *elems[4096]; int n = plc_pairs_extract(pairs_cell, elems, 4096, dot_id, dash_id);
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
    pl_cell_t *kelems[4096]; int nk = plc_plain_list_extract(keys_cell, kelems, 4096, dot_id);
    pl_cell_t *velems[4096]; int nv = plc_plain_list_extract(values_cell, velems, 4096, dot_id);
    if (nk != nv) return 0;
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
static long pl_numbervars_walk(pl_cell_t *c, long counter, int var_id, pl_tr_ctx_t *cx)
{
    pl_cell_t *d = pl_deref(c);
    if (pl_cell_unbound(d)) {
        pl_cell_t *a = (pl_cell_t *)rt_ws_alloc(sizeof(pl_cell_t));
        *a = pl_make_int(counter++);
        plc_bind_cx(d, pl_make_compound(var_id, 1, a), cx);
        return counter;
    }
    if ((int)d->v == DT_PLREF) {
        int ar = pl_arity(d); pl_cell_t *aa = (pl_cell_t *)pl_compound_heap(d);
        for (int i = 0; i < ar; i++) counter = pl_numbervars_walk(&aa[i], counter, var_id, cx);
    }
    return counter;
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
    pl_cell_t *arg = (pl_cell_t *)rt_ws_alloc(sizeof(pl_cell_t)); *arg = pl_make_int(fh_current_output());
    pl_cell_t sc = pl_make_compound(stream_id, 1, arg);
    if (!pl_unify((pl_cell_t *)stream_cell, &sc)) { return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_distinct_vars_walk(pl_cell_t *c, pl_cell_t **pool, int *counts, int *pool_n, int cap) {
    pl_cell_t *d = pl_deref(c);
    if (pl_cell_unbound(d)) {
        for (int i = 0; i < *pool_n; i++) if (pool[i] == d) { counts[i]++; return; }
        if (*pool_n < cap) { pool[*pool_n] = d; counts[*pool_n] = 1; (*pool_n)++; }
        return;
    }
    if ((int)d->v == DT_PLREF) { int ar = pl_arity(d); pl_cell_t *aa = (pl_cell_t *)pl_compound_heap(d); for (int i = 0; i < ar; i++) pl_distinct_vars_walk(&aa[i], pool, counts, pool_n, cap); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *pl_atom_text(pl_cell_t *d) {
    if ((int)d->v == DT_S || d->v == DT_SNUL) return d->s ? d->s : "";
    if ((int)d->v == DT_A) { const char *nm = prolog_atom_name((int)d->i); return nm ? nm : ""; }
    return (const char *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_is_nil(pl_cell_t *d) { const char *s = pl_atom_text(d); return s && !strcmp(s, "[]"); }
static pl_cell_t pl_nil_cell(void) { const char *nm = prolog_atom_name(prolog_atom_intern("[]")); pl_cell_t c; c.v = DT_S; c.slen = (uint32_t)(nm ? strlen(nm) : 0); c.s = nm ? nm : "[]"; return c; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_name_singleton_vars_cell(void *term_cell) {
    pl_cell_t *pool[8192]; int counts[8192]; int pn = 0;
    pl_distinct_vars_walk((pl_cell_t *)term_cell, pool, counts, &pn, 8192);
    int varname_id = prolog_atom_intern("$VARNAME"); int underscore_id = prolog_atom_intern("_");
    for (int i = 0; i < pn; i++) {
        if (counts[i] != 1) continue;
        pl_cell_t *arg = (pl_cell_t *)rt_ws_alloc(sizeof(pl_cell_t)); *arg = pl_make_atom(underscore_id);
        pl_bind(pool[i], pl_make_compound(varname_id, 1, arg));
    }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_name_query_vars_cell(void *list_cell, void *rest_cell) {
    extern void rt_pl_iso_throw_instantiation(void); extern void rt_pl_iso_throw_type(const char *, DESCR_t);
    int dot_id = prolog_atom_intern("."); int varname_id = prolog_atom_intern("$VARNAME");
    pl_cell_t *rest_pairs[8192]; int nr = 0;
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
                    pl_cell_t *arg = (pl_cell_t *)rt_ws_alloc(sizeof(pl_cell_t)); *arg = *nm;
                    pl_bind(vr, pl_make_compound(varname_id, 1, arg));
                    bound_ok = 1;
                }
            }
        }
        if (!bound_ok && nr < 8192) rest_pairs[nr++] = &kids[0];
        cur = &kids[1];
    }
    pl_cell_t result = pl_nil_cell();
    for (int i = nr - 1; i >= 0; i--) {
        pl_cell_t *blk = (pl_cell_t *)rt_ws_alloc(2 * sizeof(pl_cell_t));
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
    long used[8192]; int nused = 0;
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
                if ((int)ev->v == DT_I && nused < 8192) used[nused++] = (long)ev->i;
            }
            ecur = &ekids[1];
        }
    }
    pl_cell_t *pool[8192]; int dummy_counts[8192]; int pn = 0;
    pl_distinct_vars_walk((pl_cell_t *)term_cell, pool, dummy_counts, &pn, 8192);
    long n = from_n;
    for (int i = 0; i < pn; i++) {
        for (;;) { int clash = 0; for (int k = 0; k < nused; k++) if (used[k] == n) { clash = 1; break; } if (!clash) break; n++; }
        pl_cell_t *arg = (pl_cell_t *)rt_ws_alloc(sizeof(pl_cell_t));
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
static int pl_acyclic_walk(pl_cell_t *c, pl_cell_t ***ppath, int *pdepth, int *pcap)
{
    int base = *pdepth, ret = 1;
    for (;;) {
        pl_cell_t *d = pl_deref(c);
        if ((int)d->v != DT_PLREF) { ret = 1; break; }
        int cyc = 0; for (int i = 0; i < *pdepth; i++) if ((*ppath)[i] == d) { cyc = 1; break; }
        if (cyc) { ret = 0; break; }
        if (*pdepth >= *pcap) { int nc = *pcap * 2; pl_cell_t **np = (pl_cell_t **)realloc(*ppath, (size_t)nc * sizeof(pl_cell_t *)); if (!np) { ret = 0; break; } *ppath = np; *pcap = nc; }
        (*ppath)[(*pdepth)++] = d;
        int ar = pl_arity(d); pl_cell_t *aa = (pl_cell_t *)pl_compound_heap(d);
        if (ar <= 0) { ret = 1; break; }
        int bad = 0; for (int i = 0; i < ar - 1; i++) if (!pl_acyclic_walk(&aa[i], ppath, pdepth, pcap)) { bad = 1; break; }
        if (bad) { ret = 0; break; }
        c = &aa[ar - 1];
    }
    *pdepth = base; return ret;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_acyclic_cell(void *term_cell)
{
    if (!term_cell) return 1;
    int cap = 256, depth = 0; pl_cell_t **path = (pl_cell_t **)malloc((size_t)cap * sizeof(pl_cell_t *));
    if (!path) return 1;
    int r = pl_acyclic_walk((pl_cell_t *)term_cell, &path, &depth, &cap);
    free(path); return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_term_string_cell(void *term_cell, void *str_cell, pl_tr_ctx_t *cx)
{
    char *buf = (char *)0; size_t len = 0;
    FILE *ms = open_memstream(&buf, &len);
    if (!ms) { return 0; }
    plc_vmap m; m.n = 0; m.fp = ms;
    plc_writeq((pl_cell_t *)term_cell, &m);
    if (fclose(ms) != 0) { free(buf); return 0; }
    int ok = plc_unify_into_cell_cx((pl_cell_t *)str_cell, plc_make_atom_cell(buf ? buf : ""), cx);
    free(buf);
    if (!ok) { return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static pl_cell_t pl_cell_copy_cells(pl_cell_t *c, pl_cell_t **vaddr, pl_cell_t **vnew, int *vn, int cap)
{
    pl_cell_t *d = pl_deref(c);
    if (pl_cell_unbound(d)) {
        for (int i = 0; i < *vn; i++) if (vaddr[i] == d) return pl_make_ref(vnew[i], (int)vnew[i]->slen);
        pl_cell_t *fresh = (pl_cell_t *)rt_ws_alloc(sizeof(pl_cell_t));
        if (!fresh) return *d;
        pl_init_var(fresh, -1);
        if (*vn < cap) { vaddr[*vn] = d; vnew[*vn] = fresh; (*vn)++; }
        return pl_make_ref(fresh, (int)fresh->slen);
    }
    if ((int)d->v == DT_PLREF) {
        int fn = (int)(d->slen >> 16), ar = (int)(d->slen & 0xFFFFu);
        pl_cell_t *aa = (pl_cell_t *)d->p;
        pl_cell_t *na = (pl_cell_t *)rt_ws_alloc((size_t)(ar > 0 ? ar : 1) * sizeof(pl_cell_t));
        if (!na) return *d;
        for (int i = 0; i < ar; i++) na[i] = pl_cell_copy_cells(&aa[i], vaddr, vnew, vn, cap);
        return pl_make_compound(fn, ar, na);
    }
    return *d;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
pl_cell_t rt_pl_cell_snapshot(void *cell)
{
    pl_cell_t *vaddr[256]; pl_cell_t *vnew[256]; int vn = 0;
    return pl_cell_copy_cells((pl_cell_t *)cell, vaddr, vnew, &vn, 256);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_ball_make(DESCR_t *a, int n)
{
    if (!a || n < 1) return (void *)0;
    pl_cell_t *b = (pl_cell_t *)PL_CELL_ALLOC(sizeof(pl_cell_t));
    if (!b) return (void *)0;
    *b = rt_pl_cell_snapshot(&a[0]);
    return (void *)b;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *pl_ball_key_str(DESCR_t d)
{
    if ((int)d.v == DT_A) return prolog_atom_name((int)d.i);
    if ((int)d.v == DT_S || (int)d.v == DT_N) return d.s;
    return (const char *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_ball_existence(DESCR_t *a, int n)
{
    const char *key = (a && n > 0) ? pl_ball_key_str(a[0]) : (const char *)0;
    char nm[200]; int ar = 0;
    const char *sl = key ? strrchr(key, '/') : (const char *)0;
    if (sl) { size_t kl = (size_t)(sl - key); if (kl > sizeof nm - 1) kl = sizeof nm - 1; memcpy(nm, key, kl); nm[kl] = 0; ar = atoi(sl + 1); }
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
    er[0].v = DT_S; er[0].slen = (uint32_t)strlen("instantiation_error"); er[0].s = "instantiation_error";
    er[1] = rt_pl_fresh_var_ref();
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
    if (!ball) exit(1);
    rt_pl_ball_report(ball);
    exit(0);
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
    if ((int)d->v == DT_A) return (int)d->i;
    if ((int)d->v == DT_S) { extern int prolog_atom_intern(const char *); return prolog_atom_intern(d->s ? d->s : ""); }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_copy_term_cell(void *term_cell, void *copy_cell, pl_tr_ctx_t *cx)
{
    pl_cell_t *vaddr[256]; pl_cell_t *vnew[256]; int vn = 0;
    pl_cell_t copy = pl_cell_copy_cells((pl_cell_t *)term_cell, vaddr, vnew, &vn, 256);
    if (!plc_unify_cells_cx((pl_cell_t *)copy_cell, &copy, cx)) { return 0; }
    return 1;
}
typedef struct { pl_cell_t *items; int n; int cap; } pl_findall_acc;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void * rt_pl_findall_begin(void)
{
    pl_findall_acc *a = (pl_findall_acc *)rt_ws_alloc(sizeof *a);
    if (!a) return (void *)0;
    a->cap = 16; a->n = 0; a->items = (pl_cell_t *)rt_ws_alloc((size_t)a->cap * sizeof(pl_cell_t));
    return (void *)a;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_findall_collect(void *acc_v, void *tmpl_term)
{
    pl_findall_acc *a = (pl_findall_acc *)acc_v;
    if (!a || !a->items) return;
    if (a->n >= a->cap) {
        int nc = a->cap * 2; pl_cell_t *ni = (pl_cell_t *)rt_ws_alloc((size_t)nc * sizeof(pl_cell_t)); if (!ni) return;
        for (int i = 0; i < a->n; i++) ni[i] = a->items[i]; a->items = ni; a->cap = nc;
    }
    pl_cell_t *vaddr[256]; pl_cell_t *vnew[256]; int vn = 0;
    a->items[a->n++] = pl_cell_copy_cells((pl_cell_t *)tmpl_term, vaddr, vnew, &vn, 256);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_findall_count(void *acc_v)
{
    pl_findall_acc *a = (pl_findall_acc *)acc_v;
    return a ? a->n : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_findall_item(void *acc_v, int i, void *out)
{
    pl_findall_acc *a = (pl_findall_acc *)acc_v;
    if (!a || !out || i < 0 || i >= a->n) return;
    *(pl_cell_t *)out = a->items[i];
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_findall_finish(void *acc_v, void *result_term)
{
    pl_findall_acc *a = (pl_findall_acc *)acc_v;
    int dot = prolog_atom_intern(".");
    pl_cell_t lst; { extern const char *prolog_atom_name(int); const char *nm = prolog_atom_name(prolog_atom_intern("[]"));
        lst.v = DT_S; lst.slen = (uint32_t)(nm ? strlen(nm) : 0); lst.s = nm ? nm : ""; }
    int n = a ? a->n : 0;
    for (int i = n - 1; i >= 0; i--) {
        pl_cell_t *c = (pl_cell_t *)rt_ws_alloc(2 * sizeof(pl_cell_t)); if (!c) return 0;
        c[0] = a->items[i]; c[1] = lst;
        lst = pl_make_compound(dot, 2, c);
    }
    if (!pl_unify((pl_cell_t *)result_term, &lst)) { return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_agg_count_finish(void *acc_v, void *result_term)
{
    pl_findall_acc *a = (pl_findall_acc *)acc_v;
    pl_cell_t cnt = pl_make_int((int64_t)(a ? a->n : 0));
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
    pl_findall_acc *a = (pl_findall_acc *)acc_v;
    int n = a ? a->n : 0;
    long si = 0; double sd = 0.0; int isf = 0;
    for (int i = 0; i < n; i++) {
        long iv = 0; double dv = 0.0; int ef = 0;
        if (!agg_num(&a->items[i], &iv, &dv, &ef)) return 0;
        if (ef) { sd += dv; isf = 1; } else { si += iv; sd += (double)iv; }
    }
    pl_cell_t r = isf ? pl_make_float(sd) : pl_make_int((int64_t)si);
    if (!pl_unify((pl_cell_t *)result_term, &r)) { return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_pl_agg_minmax_finish(void *acc_v, void *result_term, int want_max)
{
    pl_findall_acc *a = (pl_findall_acc *)acc_v;
    int n = a ? a->n : 0;
    if (n <= 0) return 0;
    long bi = 0; double bd = 0.0; int isf = 0, have = 0;
    for (int i = 0; i < n; i++) {
        long iv = 0; double dv = 0.0; int ef = 0;
        if (!agg_num(&a->items[i], &iv, &dv, &ef)) return 0;
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
static pl_cell_t pl_atom_cell(const char *a) { const char *n = prolog_atom_name(prolog_atom_intern(a)); if (!n) n = ""; pl_cell_t c; c.v = DT_S; c.slen = (uint32_t)strlen(n); c.s = n; return c; }
typedef struct { const char *props[4]; int n; int i; int mark; } pl_predprop_it_t;
typedef struct { const char *name; int prec; const char *type; } pl_op_cand_t;
typedef struct { pl_op_cand_t *v; int n; int i; int mark; } pl_curop_it_t;
typedef struct { int i; int mark; } pl_flagit_t;
typedef struct { int si; int pi; int mark; } pl_spropit_t;
static pl_cell_t pl_cell_copy_persist(pl_cell_t *c, pl_cell_t **vaddr, pl_cell_t **vnew, int *vn, int cap)
{
    extern void *rt_plj_alloc(size_t);
    pl_cell_t *d = pl_deref(c);
    if (pl_cell_unbound(d)) {
        for (int i = 0; i < *vn; i++) if (vaddr[i] == d) return pl_make_ref(vnew[i], (int)vnew[i]->slen);
        pl_cell_t *fresh = (pl_cell_t *)rt_plj_alloc(sizeof(pl_cell_t));
        if (!fresh) return *d;
        pl_init_var(fresh, -1);
        if (*vn < cap) { vaddr[*vn] = d; vnew[*vn] = fresh; (*vn)++; }
        return pl_make_ref(fresh, (int)fresh->slen);
    }
    if ((int)d->v == DT_A) {
        extern const char *prolog_atom_name(int); const char *nm = prolog_atom_name((int)d->i);
        pl_cell_t c2; c2.v = DT_S; c2.slen = (uint32_t)(nm ? strlen(nm) : 0); c2.s = nm ? nm : ""; return c2;
    }
    if ((int)d->v == DT_PLREF) {
        int fn = (int)(d->slen >> 16), ar = (int)(d->slen & 0xFFFFu);
        pl_cell_t *aa = (pl_cell_t *)d->p;
        pl_cell_t *na = (pl_cell_t *)rt_plj_alloc((size_t)(ar > 0 ? ar : 1) * sizeof(pl_cell_t));
        if (!na) return *d;
        for (int i = 0; i < ar; i++) na[i] = pl_cell_copy_persist(&aa[i], vaddr, vnew, vn, cap);
        return pl_make_compound(fn, ar, na);
    }
    return *d;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define PL_DB_CELL0   24
#define PL_DB_CELLS_MAX 64
typedef struct { pl_cell_t cl; int erased; } pl_db_slot_t;
typedef struct { pl_db_slot_t *s; int n; int cap; int killed; } pl_db_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void * rt_pl_db_get(void *root, int64_t k)
{
    extern void *rt_plj_alloc(size_t);
    if (!root || k < 0 || k >= PL_DB_CELLS_MAX) return (void *)0;
    { pl_db_t **cell = (pl_db_t **)((char *)root - PL_DB_CELL0 - 8 * (size_t)k);
      if (!*cell) {
          pl_db_t *d = (pl_db_t *)rt_plj_alloc(sizeof *d);
          if (!d) return (void *)0;
          d->cap = 8; d->n = 0; d->killed = 0; d->s = (pl_db_slot_t *)rt_plj_alloc((size_t)d->cap * sizeof(pl_db_slot_t));
          if (!d->s) { d->cap = 0; }
          *cell = d;
      }
      return (void *)*cell; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_nb_set(void *root, int64_t k, void *val)
{
    extern void *rt_plj_alloc(size_t);
    if (!root || k < 0 || k >= PL_DB_CELLS_MAX || !val) return 0;
    { pl_cell_t **cell = (pl_cell_t **)((char *)root - PL_DB_CELL0 - 8 * (size_t)k);
      pl_cell_t *t = pl_deref((pl_cell_t *)val);
      pl_cell_t *va[256]; pl_cell_t *vn2[256]; int vn = 0;
      pl_cell_t stored = pl_cell_copy_persist(t, va, vn2, &vn, 256);
      pl_cell_t *box = (pl_cell_t *)rt_plj_alloc(sizeof *box);
      if (!box) return 0;
      *box = stored; *cell = box; return 1; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_nb_get_cell(void *root, int64_t k, void *out_cell, pl_tr_ctx_t *cx)
{
    if (!root || k < 0 || k >= PL_DB_CELLS_MAX || !out_cell) return 0;
    { pl_cell_t *box = *(pl_cell_t **)((char *)root - PL_DB_CELL0 - 8 * (size_t)k);
      if (!box) return 0;
      return plc_unify_cells_cx((pl_cell_t *)out_cell, box, cx) ? 1 : 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_nb_is_set(void *root, int64_t k)
{
    if (!root || k < 0 || k >= PL_DB_CELLS_MAX) return 0;
    return *(pl_cell_t **)((char *)root - PL_DB_CELL0 - 8 * (size_t)k) != (pl_cell_t *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_recompile(void *db_v, const char *key, int arity)
{
    extern void * rt_pl_clause_tree(void *);
    extern void * pl_runtime_clause_tree(void *);
    extern void * pl_runtime_define_pred(const char *, const void *, int);
    extern void * rt_pl_choice_new(const char *);
    extern void rt_pl_choice_add(void *, void *);
    extern int rt_pl_choice_n(void *);
    pl_db_t *db = (pl_db_t *)db_v;
    void *ch;
    if (!db || !key) return 0;
    ch = rt_pl_choice_new(key);
    for (int i = 0; i < db->n; i++) {
        if (db->s[i].erased) continue;
        { void *raw = rt_pl_clause_tree((void *)&db->s[i].cl); void *cl = raw ? pl_runtime_clause_tree(raw) : (void *)0;
          if (cl) rt_pl_choice_add(ch, cl); }
    }
    if (rt_pl_choice_n(ch) < 1) return 1;
    return pl_runtime_define_pred(key, ch, arity) ? 1 : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_head_key(void *pair_cell, char *out, size_t n, int *ar)
{
    extern const char *prolog_atom_name(int);
    pl_cell_t *p = pl_deref((pl_cell_t *)pair_cell);
    if ((int)p->v != DT_PLREF || pl_arity(p) != 2) return 0;
    { pl_cell_t *h = pl_deref(&((pl_cell_t *)p->p)[0]); const char *nm;
      if ((int)h->v == DT_PLREF) { nm = prolog_atom_name(plc_functor(h)); *ar = pl_arity(h); }
      else if ((int)h->v == DT_A) { nm = prolog_atom_name((int)h->i); *ar = 0; }
      else if ((int)h->v == DT_S) { nm = h->s; *ar = 0; }
      else return 0;
      if (!nm) return 0;
      snprintf(out, n, "%s/%d", nm, *ar);
      return 1; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_assert(void *db_v, void *clause_term, int prepend)
{
    extern void *rt_plj_alloc(size_t);
    extern int prolog_atom_intern(const char *);
    pl_db_t *db = (pl_db_t *)db_v;
    if (!db || !clause_term) return 0;
    db->killed = 0;
    if (db->n >= db->cap) {
        int nc = db->cap > 0 ? db->cap * 2 : 8;
        pl_db_slot_t *ns = (pl_db_slot_t *)rt_plj_alloc((size_t)nc * sizeof(pl_db_slot_t));
        if (!ns) return 0;
        for (int i = 0; i < db->n; i++) ns[i] = db->s[i];
        db->s = ns; db->cap = nc;
    }
    { pl_cell_t *t = pl_deref((pl_cell_t *)clause_term);
      pl_cell_t kids[2]; pl_cell_t pair; pl_cell_t *va[256]; pl_cell_t *vn2[256]; int vn = 0;
      if ((int)t->v == DT_PLREF && pl_arity(t) == 2 && plc_functor(t) == prolog_atom_intern(":-")) { pl_cell_t *aa = (pl_cell_t *)t->p; kids[0] = aa[0]; kids[1] = aa[1]; }
      else { kids[0] = *t; kids[1] = pl_make_atom(prolog_atom_intern("true")); }
      pair = pl_make_compound(prolog_atom_intern(":-"), 2, (void *)kids);
      { pl_cell_t stored = pl_cell_copy_persist(&pair, va, vn2, &vn, 256);
        if (prepend) { for (int i = db->n; i > 0; i--) db->s[i] = db->s[i - 1]; db->s[0].cl = stored; db->s[0].erased = 0; }
        else { db->s[db->n].cl = stored; db->s[db->n].erased = 0; }
        db->n++; }
      { char key[264]; int ar = 0;
        if (rt_pl_db_head_key((void *)&db->s[prepend ? 0 : db->n - 1].cl, key, sizeof key, &ar)) rt_pl_db_recompile(db_v, key, ar); }
      return 1; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_erase(void *db_v, int i)
{
    pl_db_t *db = (pl_db_t *)db_v;
    if (!db || i < 0 || i >= db->n || db->s[i].erased) return 0;
    db->s[i].erased = 1;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_abolish(void *db_v)
{
    pl_db_t *db = (pl_db_t *)db_v;
    if (!db) return 0;
    for (int i = 0; i < db->n; i++) db->s[i].erased = 1;
    db->killed = 1;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_match_erase(void *db_v, void *goal_term)
{
    pl_db_t *db = (pl_db_t *)db_v;
    int hit = 0;
    if (!db || !goal_term) return 0;
    for (int i = 0; i < db->n; i++) {
        if (db->s[i].erased) continue;
        { pl_cell_t *va[256]; pl_cell_t *vn2[256]; int vn = 0; pl_cell_t pair = pl_cell_copy_cells(&db->s[i].cl, va, vn2, &vn, 256);
          int vn3 = 0; pl_cell_t *va3[256]; pl_cell_t *vn4[256]; pl_cell_t g = pl_cell_copy_cells((pl_cell_t *)goal_term, va3, vn4, &vn3, 256);
          pl_cell_t *h = (pl_cell_t *)pl_deref(&pair)->p;
          if (h && pl_unify(&h[0], &g)) { db->s[i].erased = 1; hit++; } }
    }
    return hit;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_killed(void *db_v) { pl_db_t *db = (pl_db_t *)db_v; return db ? db->killed : 0; }
