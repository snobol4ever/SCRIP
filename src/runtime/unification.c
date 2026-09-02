#include "rt/rt_arena.h"
#include "rt/rt.h"
#include "core.h"
#include "builtins/resolution.h"
#include "bb_pool.h"
#include "../parsers/prolog/prolog_atom.h"
#include "../ir/IR.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "../parsers/prolog/pl_cell.h"
#define PL_CELL_ALLOC(n) (rt_pl_cellws_on() ? rt_pl_cellws_alloc(n) : rt_ws_alloc(n))
#include "../parsers/prolog/pl_cell_conv.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_unify_terms(void *l, void *r)
{
    extern pl_trail_t g_pl_trail;
    pl_cell_t *a = (pl_cell_t *)l, *b = (pl_cell_t *)r;
    if (!a || !b) return 0;
    int mark = pl_trail_mark(&g_pl_trail);
    if (!pl_unify(a, b, &g_pl_trail)) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
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
    extern pl_trail_t g_pl_trail;
    pl_cell_t *c = (pl_cell_t *)cell; if (!c) return 0;
    pl_cell_t w = pl_make_float(dval);
    int mark = pl_trail_mark(&g_pl_trail);
    if (!pl_unify(c, &w, &g_pl_trail)) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
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
    extern pl_trail_t g_pl_trail;
    pl_cell_t *src = (pl_cell_t *)arg_words; if (!dst) return 0;
    pl_cell_t *D = pl_deref((pl_cell_t *)dst);
    int fid = prolog_atom_intern(functor_name ? functor_name : "[]");
    if (pl_cell_unbound(D)) {
        pl_cell_t *blk = (pl_cell_t *)PL_CELL_ALLOC((size_t)(arity > 0 ? arity : 1) * sizeof(pl_cell_t));
        for (int i = 0; i < arity; i++) blk[i] = src[i];
        pl_bind(D, pl_make_compound(fid, arity, blk), &g_pl_trail); return 1;
    }
    if ((int)D->v == (int)DT_PLREF) {
        uint32_t want = (((uint32_t)fid) << 16) | ((uint32_t)arity & 0xFFFFu); if (D->slen != want) return 0;
        pl_cell_t *blk = (pl_cell_t *)D->p; int mark = pl_trail_mark(&g_pl_trail);
        for (int i = 0; i < arity; i++) if (!pl_unify(&blk[i], &src[i], &g_pl_trail)) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
        return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static FILE *plc_out(void) { extern FILE *fh_cur_out_fp(void); FILE *f = fh_cur_out_fp(); return f ? f : stdout; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* plc_* : the CELL-NATIVE Prolog printer (slice prolog-term-descr-s1). Mirrors prolog_builtin.c's pl_write ARM FOR ARM on DESCR cells, so the heap round-trip through the parser's structure type is deleted, not relocated. Byte-identity is the bar: the var
   numbering below reproduces pl_cell_to_term_named's pl_v2t_index EXACTLY -- unbound cells print _G<n> by FIRST-ENCOUNTER order with shared vars reusing an index, and the map is per top-level call, NOT the cell's own slot.
   Using pl_disc() here instead would compile, run, and silently renumber every variable in every printed term. The map is a stack local threaded through the walk; no global is added (NO-NEW-GLOBALS). */
typedef struct { pl_cell_t *seen[1024]; int n; } plc_vmap;
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
/* nil in EITHER cell spelling. An atom that round-trips through pl_term_to_cell_word arrives as DT_S carrying the name, not DT_A carrying the id (see pl_cell_conv.h), and the converter this printer replaces interned that
   string back to an id before comparing -- so a DT_S "[]" tail closed the bracket. Checking only DT_A here would print "|[]" for exactly those lists: a divergence the probe does not reach, because probe-built lists carry DT_A tails. */
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
    FILE *fp = plc_out();
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
    for (int i = 0; ops[i].name; i++) {
        if (strcmp(fn, ops[i].name) == 0 && ar == ops[i].arity) {
            if (ops[i].arity == 2) {
                int lp = plc_operand_prec(&aa[0]), rp = plc_operand_prec(&aa[1]), my_prec = ops[i].prec;
                int lneed = (lp > my_prec) || (lp == my_prec && ops[i].right_assoc);
                int rneed = (rp > my_prec) || (rp == my_prec && !ops[i].right_assoc);
                if (lneed) fprintf(fp, "(");
                plc_write(&aa[0], m);
                if (lneed) fprintf(fp, ")");
                if (isalpha((unsigned char)fn[0])) fprintf(fp, " %s ", fn); else fprintf(fp, "%s", fn);
                if (rneed) fprintf(fp, "(");
                plc_write(&aa[1], m);
                if (rneed) fprintf(fp, ")");
            } else {
                int ap = plc_operand_prec(&aa[0]);
                int aneed = (ap >= ops[i].prec);
                if (isalpha((unsigned char)fn[0])) fprintf(fp, "%s ", fn); else fprintf(fp, "%s", fn);
                if (aneed) fprintf(fp, "(");
                plc_write(&aa[0], m);
                if (aneed) fprintf(fp, ")");
            }
            return;
        }
    }
    fprintf(fp, "%s(", fn);
    for (int i = 0; i < ar; i++) { if (i) fprintf(fp, ","); plc_write(&aa[i], m); }
    fprintf(fp, ")");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* plc_wt : the options-bearing cell-native printer, mirroring prolog_builtin.c's pl_wt arm for arm. NOT a flag on plc_write -- pl_wt is a genuinely
   different algorithm (quoting, ignore_ops gating BOTH list and operator syntax,
   max_depth ellipsis, no $VARNAME arm, no {} arm, '.' self-quoted in the canonical fallback), so collapsing the two would change output in both
   directions. Written once here with the full option set: display is (0,1,0,0) and
   write_term parses its own. The var numbering is plc_vindex again, because the caller it replaces converted through pl_cell_to_term_named FIRST
   and pl_wt then printed that already-renumbered slot. */
static int plc_atom_needs_quoting(const char *name)
{
    if (!name || !name[0]) return 1;
    if (name[0] == '[' && strcmp(name, "[]") == 0) return 0;
    if (name[0] == '{' && strcmp(name, "{}") == 0) return 0;
    if (isupper((unsigned char)name[0]) || name[0] == '_') return 1;
    int all_graphic = 1;
    static const char *graphic = "#&*+-./:<=>?@\\^~";
    for (const char *q = name; *q; q++) if (!strchr(graphic, *q)) { all_graphic = 0; break; }
    if (all_graphic) return 0;
    if (islower((unsigned char)name[0])) { for (const char *q = name+1; *q; q++) if (!isalnum((unsigned char)*q) && *q != '_') return 1; return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_wt_atom(const char *name, int quoted)
{
    FILE *fp = plc_out();
    if (!name) name = "?";
    if (quoted && plc_atom_needs_quoting(name)) { fputc('\'', fp); for (const char *q = name; *q; q++) { if (*q == '\'') fputc('\'', fp); fputc(*q, fp); } fputc('\'', fp); }
    else fprintf(fp, "%s", name);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_wt(pl_cell_t *c, int quoted, int ignore_ops, int numbervars, long max_depth, long depth, plc_vmap *m)
{
    extern int ATOM_DOT;
    FILE *fp = plc_out();
    if (!c) { plc_wt_atom("[]", quoted); return; }
    pl_cell_t *d = pl_deref(c);
    if (max_depth > 0 && depth >= max_depth) { fprintf(fp, "..."); return; }
    int tg = (int)d->v;
    if (pl_cell_unbound(d)) { fprintf(fp, "_G%d", plc_vindex(m, d)); return; }
    if (tg == DT_A || tg == DT_S) { plc_wt_atom(plc_atom_text(d), quoted); return; }
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
    if (!ignore_ops) for (int i = 0; ops[i].name; i++) if (strcmp(fn, ops[i].name) == 0 && ar == ops[i].arity) {
        if (ops[i].arity == 2) {
            int lp = plc_operand_prec(&aa[0]), rp = plc_operand_prec(&aa[1]), my = ops[i].prec;
            if ((lp > my) || (lp == my && ops[i].right_assoc)) { fprintf(fp, "("); plc_wt(&aa[0], quoted, ignore_ops, numbervars, max_depth, depth+1, m); fprintf(fp, ")"); }
            else plc_wt(&aa[0], quoted, ignore_ops, numbervars, max_depth, depth+1, m);
            if (isalpha((unsigned char)fn[0])) fprintf(fp, " %s ", fn); else fprintf(fp, "%s", fn);
            if ((rp > my) || (rp == my && !ops[i].right_assoc)) { fprintf(fp, "("); plc_wt(&aa[1], quoted, ignore_ops, numbervars, max_depth, depth+1, m); fprintf(fp, ")"); }
            else plc_wt(&aa[1], quoted, ignore_ops, numbervars, max_depth, depth+1, m);
        } else {
            int ap = plc_operand_prec(&aa[0]);
            if (isalpha((unsigned char)fn[0])) fprintf(fp, "%s ", fn); else fprintf(fp, "%s", fn);
            if (ap >= ops[i].prec) { fprintf(fp, "("); plc_wt(&aa[0], quoted, ignore_ops, numbervars, max_depth, depth+1, m); fprintf(fp, ")"); }
            else plc_wt(&aa[0], quoted, ignore_ops, numbervars, max_depth, depth+1, m);
        }
        return;
    }
    if (fn[0] == '.' && fn[1] == 0) fprintf(fp, "'.'"); else plc_wt_atom(fn, quoted);
    fprintf(fp, "(");
    for (int i = 0; i < ar; i++) { if (i) fprintf(fp, ","); plc_wt(&aa[i], quoted, ignore_ops, numbervars, max_depth, depth+1, m); }
    fprintf(fp, ")");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* plc_writeq : mirrors prolog_builtin.c's pl_writeq_term arm for arm. NOT plc_wt with quoted=1 -- writeq always translates $VAR AND $VARNAME (no numbervars gate), always applies {} and operator/list syntax (no ignore_ops), and formats floats with the SIMPLE %.1f/%g rule, not plc_wt's round-trip-safe %.*g. Collapsing them would silently change float output. */
static void plc_writeq(pl_cell_t *c, plc_vmap *m)
{
    extern int ATOM_DOT;
    FILE *fp = plc_out();
    if (!c) { fprintf(fp, "'[]'"); return; }
    pl_cell_t *d = pl_deref(c);
    int tg = (int)d->v;
    if (pl_cell_unbound(d)) { fprintf(fp, "_G%d", plc_vindex(m, d)); return; }
    if (tg == DT_A || tg == DT_S) { plc_wt_atom(plc_atom_text(d), 1); return; }
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
    for (int i = 0; ops[i].name; i++) {
        if (strcmp(fn, ops[i].name) == 0 && ar == ops[i].arity) {
            if (ops[i].arity == 2) {
                int lp = plc_operand_prec(&aa[0]), rp = plc_operand_prec(&aa[1]), my_prec = ops[i].prec;
                int lneed = (lp > my_prec) || (lp == my_prec && ops[i].right_assoc);
                int rneed = (rp > my_prec) || (rp == my_prec && !ops[i].right_assoc);
                if (lneed) fprintf(fp, "(");
                plc_writeq(&aa[0], m);
                if (lneed) fprintf(fp, ")");
                if (isalpha((unsigned char)fn[0])) fprintf(fp, " %s ", fn); else fprintf(fp, "%s", fn);
                if (rneed) fprintf(fp, "(");
                plc_writeq(&aa[1], m);
                if (rneed) fprintf(fp, ")");
            } else {
                int ap = plc_operand_prec(&aa[0]);
                int aneed = (ap >= ops[i].prec);
                if (isalpha((unsigned char)fn[0])) fprintf(fp, "%s ", fn); else fprintf(fp, "%s", fn);
                if (aneed) fprintf(fp, "(");
                plc_writeq(&aa[0], m);
                if (aneed) fprintf(fp, ")");
            }
            return;
        }
    }
    plc_wt_atom(fn, 1);
    fprintf(fp, "(");
    for (int i = 0; i < ar; i++) { if (i) fprintf(fp, ","); plc_writeq(&aa[i], m); }
    fprintf(fp, ")");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* plc_write_canonical : mirrors prolog_builtin.c's pl_write_canonical_term arm for arm. Quoting always on, NO operator syntax ever (always functor(args)), NO $VAR/$VARNAME translation, dot-pairs ALWAYS print as [a,b] list notation -- MEASURED against the project's sole Prolog oracle (SWI): write_canonical/1 uses list notation for proper lists unconditionally, unlike display/1's raw '.'(H,T) form (a distinct, unaffected code path via plc_wt); matches plc_writeq's own unconditional list handling. SIMPLE %.1f/%g float format like plc_writeq -- a third, distinct algorithm from both plc_writeq and plc_wt. */
static void plc_write_canonical(pl_cell_t *c, plc_vmap *m)
{
    extern int ATOM_DOT;
    FILE *fp = plc_out();
    if (!c) { fprintf(fp, "'[]'"); return; }
    pl_cell_t *d = pl_deref(c);
    int tg = (int)d->v;
    if (pl_cell_unbound(d)) { fprintf(fp, "_G%d", plc_vindex(m, d)); return; }
    if (tg == DT_A || tg == DT_S) { plc_wt_atom(plc_atom_text(d), 1); return; }
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
    plc_wt_atom(fn, 1);
    fprintf(fp, "(");
    for (int i = 0; i < ar; i++) { if (i) fprintf(fp, ","); plc_write_canonical(&aa[i], m); }
    fprintf(fp, ")");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_write_cell(void *cell)
{
    plc_vmap m; m.n = 0;
    plc_write((pl_cell_t *)cell, &m);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_writeq_cell(void *cell)
{
    plc_vmap m; m.n = 0;
    plc_writeq((pl_cell_t *)cell, &m);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_write_canonical_cell(void *cell)
{
    plc_vmap m; m.n = 0;
    plc_write_canonical((pl_cell_t *)cell, &m);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_display_cell(void *cell)
{
    plc_vmap m; m.n = 0;
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
    plc_vmap m; m.n = 0;
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_trail_mark(void)
{
    extern pl_trail_t g_pl_trail;
    return pl_trail_mark(&g_pl_trail);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_trail_unwind(int mark)
{
    extern pl_trail_t g_pl_trail;
    pl_trail_unwind(&g_pl_trail, mark);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_e_bump(int nbytes)
{
    extern pl_area_t g_pl_env_area;
    return pl_env_bump(&g_pl_env_area, nbytes);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_e_mark(void)
{
    extern pl_area_t g_pl_env_area;
    return pl_env_mark(&g_pl_env_area);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_e_reset(int off)
{
    extern pl_area_t g_pl_env_area;
    pl_env_reset(&g_pl_env_area, off);
}
#define RT_MARK_STACK_MAX 32
static int g_resolve_mark_stack[RT_MARK_STACK_MAX];
static int g_resolve_mark_top = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_trail_mark_push(void)
{
    extern pl_trail_t g_pl_trail;
    int m = pl_trail_mark(&g_pl_trail);
    if (g_resolve_mark_top < RT_MARK_STACK_MAX) g_resolve_mark_stack[g_resolve_mark_top++] = m;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_trail_unwind_top(void)
{
    extern pl_trail_t g_pl_trail;
    if (g_resolve_mark_top <= 0) return;
    int m = g_resolve_mark_stack[g_resolve_mark_top - 1];
    pl_trail_unwind(&g_pl_trail, m);
}
typedef struct { const char *name; long arity; void *alpha; void *redo; } pl_pred_row_t;
static pl_pred_row_t *g_pl_pred_table = (pl_pred_row_t *)0;
static long           g_pl_pred_n     = 0;
extern const char *prolog_atom_name(int id);
extern int    rt_last_ok(void);
extern long   rt_arith(int lk, long li, const char *ls, int rk, long ri, const char *rs, const char *op);
enum { MK_TRUE, MK_FAIL, MK_PRED, MK_BUILTIN, MK_CONJ, MK_DISJ };
typedef struct meta_fr {
    int kind; Term *goal; const char *name; int arity; Term **args;
    void *alpha; void *redo; resolve_choice *mark;
    struct meta_fr **kids; int nkids; int cur;
    resolve_choice *br_cp; int br_trail;
} meta_fr;
typedef struct { meta_fr *fr; Term **E; } meta_root;
static const char *g_meta_builtins[] = { "is", "=:=", "=\\=", "<", ">", "=<", ">=", "=", "\\=", (const char *)0 };
static int meta_solve(meta_fr *f, Term **E);
static int meta_redo(meta_fr *f, Term **E);
static meta_root *g_meta_compat = (meta_root *)0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
/* Every atom this slice hands to a caller (functor/3 Name; univ's =.. functor element) must be DT_S -- pl_make_atom's DT_A type-checks and links fine but SIGSEGVs in write/1's out_write_descr,
   via c_VARVAL_fn, which never learned DT_A (FINDING-2026-09-01-seat10-prolog-atoms-ride-dt-s-not-dt-a). Matches pl_term_to_cell_word (pl_cell_conv.h:70); twin of pl_atom_cell/pl_nil_cell below. */
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
int g_pl_functor_slot_ctr = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_functor_cell(void *t0_cell, void *name_cell, void *arity_cell)
{
    extern pl_trail_t g_pl_trail;
    pl_cell_t *d0 = t0_cell ? pl_deref((pl_cell_t *)t0_cell) : (pl_cell_t *)0;
    pl_cell_t *d1 = name_cell ? pl_deref((pl_cell_t *)name_cell) : (pl_cell_t *)0;
    int mark = pl_trail_mark(&g_pl_trail);
    if (d0 && !pl_cell_unbound(d0)) {
        pl_cell_t nameV, arityV;
        if ((int)d0->v == DT_PLREF)   { nameV = plc_atom_id_cell((int)(d0->slen >> 16)); arityV = pl_make_int((int64_t)(d0->slen & 0xFFFFu)); }
        else if (plc_is_atomlike(d0)) { nameV = plc_atom_id_cell(plc_atom_id_of(d0)); arityV = pl_make_int(0); }
        else if ((int)d0->v == DT_I)  { nameV = pl_make_int(d0->i); arityV = pl_make_int(0); }
        else if ((int)d0->v == DT_R)  { nameV = pl_make_float(d0->r); arityV = pl_make_int(0); }
        else { pl_trail_unwind(&g_pl_trail, mark); return 0; }
        if (!pl_unify((pl_cell_t *)name_cell, &nameV, &g_pl_trail) ||
            !pl_unify((pl_cell_t *)arity_cell, &arityV, &g_pl_trail)) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
        return 1;
    }
    pl_cell_t *d2 = arity_cell ? pl_deref((pl_cell_t *)arity_cell) : (pl_cell_t *)0;
    if (!d2 || (int)d2->v != DT_I) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
    long ar = (long)d2->i;
    pl_cell_t built;
    if (ar == 0) { built = d1 ? *d1 : plc_atom_id_cell(prolog_atom_intern("[]")); }
    else {
        if (!d1 || !plc_is_atomlike(d1)) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
        extern int g_pl_functor_slot_ctr;
        pl_cell_t *args = (pl_cell_t *)PL_CELL_ALLOC((size_t)ar * sizeof(pl_cell_t));
        for (long i = 0; i < ar; i++) pl_init_var(&args[i], g_pl_functor_slot_ctr++);
        built = pl_make_compound(plc_atom_id_of(d1), (int)ar, args);
    }
    if (!pl_unify((pl_cell_t *)t0_cell, &built, &g_pl_trail)) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_arg_cell(void *n_cell, void *t_cell, void *arg_cell)
{
    extern pl_trail_t g_pl_trail;
    pl_cell_t *dN = n_cell ? pl_deref((pl_cell_t *)n_cell) : (pl_cell_t *)0;
    pl_cell_t *dT = t_cell ? pl_deref((pl_cell_t *)t_cell) : (pl_cell_t *)0;
    int mark = pl_trail_mark(&g_pl_trail);
    if (!dN || (int)dN->v != DT_I || !dT || (int)dT->v != DT_PLREF || !dT->p) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
    long n = (long)dN->i;
    if (n < 1 || n > (long)(dT->slen & 0xFFFFu)) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
    if (!pl_unify((pl_cell_t *)arg_cell, &((pl_cell_t *)dT->p)[n - 1], &g_pl_trail)) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_univ_cell(void *t0_cell, void *list_cell)
{
    extern int ATOM_DOT;
    extern pl_trail_t g_pl_trail;
    pl_cell_t *d0 = t0_cell ? pl_deref((pl_cell_t *)t0_cell) : (pl_cell_t *)0;
    int mark = pl_trail_mark(&g_pl_trail);
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
        if (!pl_unify((pl_cell_t *)list_cell, &lst, &g_pl_trail)) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
        return 1;
    }
    pl_cell_t *cur = list_cell ? pl_deref((pl_cell_t *)list_cell) : (pl_cell_t *)0;
    pl_cell_t *elems[64]; int ne = 0;
    while (cur && (int)cur->v == DT_PLREF && (int)(cur->slen >> 16) == ATOM_DOT && (int)(cur->slen & 0xFFFFu) == 2 && cur->p) {
        if (ne >= 64) break;
        elems[ne++] = pl_deref(&((pl_cell_t *)cur->p)[0]);
        cur = pl_deref(&((pl_cell_t *)cur->p)[1]);
    }
    if (ne == 0) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
    pl_cell_t built;
    if (ne == 1) { built = *elems[0]; }
    else {
        pl_cell_t *h = elems[0];
        if (!h || !plc_is_atomlike(h)) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
        pl_cell_t *args = (pl_cell_t *)PL_CELL_ALLOC((size_t)(ne - 1) * sizeof(pl_cell_t));
        for (int i = 1; i < ne; i++) args[i - 1] = *elems[i];
        built = pl_make_compound(plc_atom_id_of(h), ne - 1, args);
    }
    if (!pl_unify((pl_cell_t *)t0_cell, &built, &g_pl_trail)) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_succ_plus_cell(long arity, void *a_cell, void *b_cell, void *c_cell)
{
    extern pl_trail_t g_pl_trail;
    pl_cell_t *da = a_cell ? pl_deref((pl_cell_t *)a_cell) : (pl_cell_t *)0;
    pl_cell_t *db = b_cell ? pl_deref((pl_cell_t *)b_cell) : (pl_cell_t *)0;
    pl_cell_t *dc = c_cell ? pl_deref((pl_cell_t *)c_cell) : (pl_cell_t *)0;
    int mark = pl_trail_mark(&g_pl_trail);
    if (arity == 2) {
        if (da && (int)da->v == DT_I) {
            if (da->i < 0) return 0;
            pl_cell_t v = pl_make_int(da->i + 1);
            if (!pl_unify((pl_cell_t *)b_cell, &v, &g_pl_trail)) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
            return 1;
        }
        if (db && (int)db->v == DT_I) {
            if (db->i <= 0) return 0;
            pl_cell_t v = pl_make_int(db->i - 1);
            if (!pl_unify((pl_cell_t *)a_cell, &v, &g_pl_trail)) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
            return 1;
        }
        return 0;
    }
    if (arity == 3) {
        int va = (da && (int)da->v == DT_I), vb = (db && (int)db->v == DT_I), vc = (dc && (int)dc->v == DT_I);
        int ok = 0; pl_cell_t v;
        if (va && vb)      { v = pl_make_int(da->i + db->i); ok = pl_unify((pl_cell_t *)c_cell, &v, &g_pl_trail); }
        else if (va && vc) { v = pl_make_int(dc->i - da->i); ok = pl_unify((pl_cell_t *)b_cell, &v, &g_pl_trail); }
        else if (vb && vc) { v = pl_make_int(dc->i - db->i); ok = pl_unify((pl_cell_t *)a_cell, &v, &g_pl_trail); }
        if (!ok) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
        return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *atom_op_text(Term *t, char *buf, size_t bufsz)
{
    t = t ? term_deref(t) : (Term *)0;
    if (!t) return (const char *)0;
    if (t->tag == TERM_ATOM)  return prolog_atom_name(t->atom_id);
    if (t->tag == TERM_INT)   { snprintf(buf, bufsz, "%ld", t->ival);  return buf; }
    if (t->tag == TERM_FLOAT) { snprintf(buf, bufsz, "%g", t->fval);   return buf; }
    return (const char *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* plc_atom_op_text/plc_make_atom_cell/plc_unify_into_cell : the cell-native trio behind rt_pl_atom_op_cell and rt_pl_char_type_cell below. atom_op_text (Term-based, above) STAYS -- rt_pl_lower_upper_cell (out of this
   slice's scope) still calls it, so it cannot be deleted here; re-verify its callers before ever retiring it. plc_make_atom_cell builds DT_S, never DT_A -- pl_make_atom builds DT_A but Prolog atoms ride DT_S
   (hq_C's trap, this file's LEDGER); the exact field-setting mirrors pl_term_to_cell_word_m's TERM_ATOM arm in pl_cell_conv.h. plc_unify_into_cell mirrors pl_unify_term_into_cell's shape (build a value, unify it)
   without the Term round-trip. */
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
static int plc_unify_into_cell(pl_cell_t *dst, pl_cell_t val, pl_trail_t *trail)
{
    pl_cell_t tmp = val;
    return pl_unify(dst, &tmp, trail);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_atom_op_cell(const char *fn, void *a0_cell, void *a1_cell, void *a2_cell)
{
    extern pl_trail_t g_pl_trail;
    pl_cell_t *t0 = a0_cell ? pl_deref((pl_cell_t *)a0_cell) : (pl_cell_t *)0;
    pl_cell_t *t1 = a1_cell ? pl_deref((pl_cell_t *)a1_cell) : (pl_cell_t *)0;
    pl_cell_t *t2 = a2_cell ? pl_deref((pl_cell_t *)a2_cell) : (pl_cell_t *)0;
    int mark = pl_trail_mark(&g_pl_trail);
    char buf0[512], buf1[512];
    if (!strcmp(fn, "atom_length")) {
        const char *s = plc_atom_op_text(t0, buf0, sizeof buf0);
        if (!s) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
        if (!plc_unify_into_cell((pl_cell_t *)a1_cell, pl_make_int((int64_t)strlen(s)), &g_pl_trail)) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
        return 1;
    }
    if (!strcmp(fn, "atom_concat")) {
        const char *s0 = plc_atom_op_text(t0, buf0, sizeof buf0);
        const char *s1 = plc_atom_op_text(t1, buf1, sizeof buf1);
        if (!s0 || !s1) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
        size_t l0 = strlen(s0), l1 = strlen(s1);
        char *cat = (char *)rt_ws_alloc(l0 + l1 + 1); memcpy(cat, s0, l0); memcpy(cat + l0, s1, l1); cat[l0 + l1] = '\0';
        if (!plc_unify_into_cell((pl_cell_t *)a2_cell, plc_make_atom_cell(cat), &g_pl_trail)) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
        return 1;
    }
    if (!strcmp(fn, "upcase_atom") || !strcmp(fn, "downcase_atom")) {
        const char *s = plc_atom_op_text(t0, buf0, sizeof buf0);
        if (!s) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
        size_t n = strlen(s); char *out = (char *)rt_ws_alloc(n + 1);
        int up = (!strcmp(fn, "upcase_atom"));
        for (size_t i = 0; i < n; i++) out[i] = up ? (char)toupper((unsigned char)s[i]) : (char)tolower((unsigned char)s[i]);
        out[n] = '\0';
        if (!plc_unify_into_cell((pl_cell_t *)a1_cell, plc_make_atom_cell(out), &g_pl_trail)) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
        return 1;
    }
    int as_codes = (!strcmp(fn, "atom_codes"));
    if (!strcmp(fn, "atom_chars") || as_codes) {
        if (t0 && !pl_cell_unbound(t0)) {
            const char *s = plc_atom_op_text(t0, buf0, sizeof buf0);
            if (!s) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
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
            if (!plc_unify_into_cell((pl_cell_t *)a1_cell, lst, &g_pl_trail)) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
            return 1;
        }
        pl_cell_t *cur = t1;
        char out[512]; size_t oi = 0;
        while (cur && (int)cur->v == DT_PLREF && (int)(cur->slen >> 16) == ATOM_DOT && (int)(cur->slen & 0xFFFFu) == 2) {
            pl_cell_t *pr = (pl_cell_t *)cur->p;
            pl_cell_t *el = pl_deref(&pr[0]);
            if (oi >= sizeof(out) - 1) break;
            if (as_codes) { if ((int)el->v != DT_I) { pl_trail_unwind(&g_pl_trail, mark); return 0; } out[oi++] = (char)el->i; }
            else { if ((int)el->v != DT_A && (int)el->v != DT_S) { pl_trail_unwind(&g_pl_trail, mark); return 0; } const char *cn = plc_atom_text(el); out[oi++] = cn ? cn[0] : '?'; }
            cur = pl_deref(&pr[1]);
        }
        out[oi] = '\0';
        if (!plc_unify_into_cell((pl_cell_t *)a0_cell, plc_make_atom_cell(out), &g_pl_trail)) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
        return 1;
    }
    if (!strcmp(fn, "string_length")) {
        const char *s = plc_atom_op_text(t0, buf0, sizeof buf0);
        if (!s) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
        if (!plc_unify_into_cell((pl_cell_t *)a1_cell, pl_make_int((int64_t)strlen(s)), &g_pl_trail)) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
        return 1;
    }
    if (!strcmp(fn, "string_upper") || !strcmp(fn, "string_lower")) {
        const char *s = plc_atom_op_text(t0, buf0, sizeof buf0);
        if (!s) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
        size_t n = strlen(s); char *out = (char *)rt_ws_alloc(n + 1);
        int up = (!strcmp(fn, "string_upper"));
        for (size_t i = 0; i < n; i++) out[i] = up ? (char)toupper((unsigned char)s[i]) : (char)tolower((unsigned char)s[i]);
        out[n] = '\0';
        if (!plc_unify_into_cell((pl_cell_t *)a1_cell, plc_make_atom_cell(out), &g_pl_trail)) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
        return 1;
    }
    if (!strcmp(fn, "atom_string") || !strcmp(fn, "string_to_atom")) {
        if (t0 && !pl_cell_unbound(t0)) {
            const char *s = plc_atom_op_text(t0, buf0, sizeof buf0);
            if (!s) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
            if (!plc_unify_into_cell((pl_cell_t *)a1_cell, plc_make_atom_cell(s), &g_pl_trail)) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
            return 1;
        }
        const char *s = plc_atom_op_text(t1, buf1, sizeof buf1);
        if (!s) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
        if (!plc_unify_into_cell((pl_cell_t *)a0_cell, plc_make_atom_cell(s), &g_pl_trail)) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
        return 1;
    }
    if (!strcmp(fn, "number_string") || !strcmp(fn, "atom_number")) {
        if (t0 && !pl_cell_unbound(t0)) {
            const char *s = plc_atom_op_text(t0, buf0, sizeof buf0);
            if (!s) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
            if (!plc_unify_into_cell((pl_cell_t *)a1_cell, plc_make_atom_cell(s), &g_pl_trail)) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
            return 1;
        }
        const char *s = plc_atom_op_text(t1, buf1, sizeof buf1);
        if (!s) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
        char *end; long iv = strtol(s, &end, 10);
        if (*end == '\0') { if (!plc_unify_into_cell((pl_cell_t *)a0_cell, pl_make_int(iv), &g_pl_trail)) { pl_trail_unwind(&g_pl_trail, mark); return 0; } return 1; }
        double dv = strtod(s, &end);
        if (*end == '\0') { if (!plc_unify_into_cell((pl_cell_t *)a0_cell, pl_make_float(dv), &g_pl_trail)) { pl_trail_unwind(&g_pl_trail, mark); return 0; } return 1; }
        pl_trail_unwind(&g_pl_trail, mark); return 0;
    }
    if (!strcmp(fn, "string_concat")) {
        const char *s0 = plc_atom_op_text(t0, buf0, sizeof buf0);
        const char *s1 = plc_atom_op_text(t1, buf1, sizeof buf1);
        if (!s0 || !s1) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
        size_t l0 = strlen(s0), l1 = strlen(s1);
        char *cat = (char *)rt_ws_alloc(l0 + l1 + 1); memcpy(cat, s0, l0); memcpy(cat + l0, s1, l1); cat[l0 + l1] = '\0';
        if (!plc_unify_into_cell((pl_cell_t *)a2_cell, plc_make_atom_cell(cat), &g_pl_trail)) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
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
            if (!es) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
            size_t el_len = strlen(es); if (oi + el_len < 4095) { memcpy(out + oi, es, el_len); oi += el_len; }
            lst = pl_deref(&pr[1]);
        }
        out[oi] = '\0';
        if (!plc_unify_into_cell((pl_cell_t *)result_cell, plc_make_atom_cell(out), &g_pl_trail)) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
        return 1;
    }
    (void)t2;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static Term *pl_fmt_next_arg(Term **args) {
    if (*args && (*args)->tag == TERM_COMPOUND && (*args)->compound.arity == 2) { Term *h = term_deref((*args)->compound.args[0]); *args = term_deref((*args)->compound.args[1]); return h; }
    return (Term *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_format_cell(const char *fmt, void *list_cell)
{
    extern void pl_write(Term *); extern void pl_writeq(Term *); extern void pl_wr_set_fp(FILE *); extern FILE *fh_cur_out_fp(void);
    if (!fmt) return;
    FILE *fd = fh_cur_out_fp(); pl_wr_set_fp(fd);
    arena_mark_t cm = rt_pl_cterm_mark();
    Term *args = list_cell ? pl_cell_to_term_named((pl_cell_t *)list_cell) : (Term *)0;
    for (const char *p = fmt; *p; p++) {
        if (*p != '~') { fputc(*p, fd); continue; }
        p++;
        int have_n = 0; long nval = 0;
        if (*p == '*') { Term *h = pl_fmt_next_arg(&args); if (h && h->tag == TERM_INT) { nval = h->ival; have_n = 1; } p++; }
        else { while (*p >= '0' && *p <= '9') { nval = nval * 10 + (*p - '0'); have_n = 1; p++; } }
        if (*p == 'w' || *p == 'a' || *p == 'p') { Term *h = pl_fmt_next_arg(&args); if (h) pl_write(h); }
        else if (*p == 'q') { Term *h = pl_fmt_next_arg(&args); if (h) pl_writeq(h); }
        else if (*p == 'd') { Term *h = pl_fmt_next_arg(&args); if (h && h->tag == TERM_INT) fprintf(fd, "%ld", h->ival); }
        else if (*p == 'e') { Term *h = pl_fmt_next_arg(&args); if (h) { double d = (h->tag == TERM_INT) ? (double)h->ival : h->fval; fprintf(fd, "%e", d); } }
        else if (*p == 'g') { Term *h = pl_fmt_next_arg(&args); if (h) { double d = (h->tag == TERM_INT) ? (double)h->ival : h->fval; fprintf(fd, "%g", d); } }
        else if (*p == 'f') { Term *h = pl_fmt_next_arg(&args); if (h) { double d = (h->tag == TERM_INT) ? (double)h->ival : h->fval; fprintf(fd, "%.*f", have_n ? (int)nval : 6, d); } }
        else if (*p == 'r' || *p == 'R') { Term *h = pl_fmt_next_arg(&args); int base = have_n ? (int)nval : 8; if (h && h->tag == TERM_INT && base >= 2 && base <= 36) { char buf[72]; int bi = 0; unsigned long u = (h->ival < 0) ? (unsigned long)(-h->ival) : (unsigned long)h->ival; const char *dig = (*p == 'R') ? "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ" : "0123456789abcdefghijklmnopqrstuvwxyz"; if (u == 0) buf[bi++] = '0'; while (u) { buf[bi++] = dig[u % (unsigned long)base]; u /= (unsigned long)base; } if (h->ival < 0) fputc('-', fd); while (bi) fputc(buf[--bi], fd); } }
        else if (*p == 'c') { Term *h = pl_fmt_next_arg(&args); if (h && h->tag == TERM_INT) { int rep = have_n ? (int)nval : 1; for (int i = 0; i < rep; i++) fputc((int)h->ival, fd); } }
        else if (*p == 's') { Term *h = pl_fmt_next_arg(&args); Term *lst = h ? term_deref(h) : (Term *)0; while (lst && lst->tag == TERM_COMPOUND && lst->compound.arity == 2) { Term *e = term_deref(lst->compound.args[0]); if (e && e->tag == TERM_INT) fputc((int)e->ival, fd); lst = term_deref(lst->compound.args[1]); } }
        else if (*p == 'i') { pl_fmt_next_arg(&args); }
        else if (*p == 'n' || *p == 'N') { int rep = have_n ? (int)nval : 1; for (int i = 0; i < rep; i++) fputc('\n', fd); }
        else if (*p == '~') { fputc('~', fd); }
    }
    if (rt_pl_ctr_on()) rt_pl_cterm_release(cm);
    pl_wr_set_fp((FILE *)0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_char_type_cell(void *char_cell, void *type_cell, void *val_cell)
{
    extern pl_trail_t g_pl_trail;
    pl_cell_t *tc = char_cell ? pl_deref((pl_cell_t *)char_cell) : (pl_cell_t *)0;
    if (!tc || !type_cell) return 0;
    char b0[256]; const char *cs = plc_atom_op_text(tc, b0, sizeof b0);
    if (!cs || !cs[0]) return 0;
    unsigned char ch = (unsigned char)cs[0];
    pl_cell_t *td = pl_deref((pl_cell_t *)type_cell);
    int mark = pl_trail_mark(&g_pl_trail);
    if ((int)td->v == DT_PLREF && pl_arity(td) >= 1) {
        const char *ty = prolog_atom_name(plc_functor(td));
        pl_cell_t *inner = val_cell ? (pl_cell_t *)val_cell : &((pl_cell_t *)pl_compound_heap(td))[0];
        pl_cell_t out;
        if (!ty) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
        if (!strcmp(ty, "digit"))    { if (!isdigit(ch)) { pl_trail_unwind(&g_pl_trail, mark); return 0; } out = pl_make_int((int64_t)(ch - '0')); }
        else if (!strcmp(ty, "to_lower")) { char c2[2] = { (char)tolower(ch), 0 }; out = plc_make_atom_cell(c2); }
        else if (!strcmp(ty, "to_upper")) { char c2[2] = { (char)toupper(ch), 0 }; out = plc_make_atom_cell(c2); }
        else if (!strcmp(ty, "upper")) { if (!isupper(ch)) { pl_trail_unwind(&g_pl_trail, mark); return 0; } char c2[2] = { (char)tolower(ch), 0 }; out = plc_make_atom_cell(c2); }
        else if (!strcmp(ty, "lower")) { if (!islower(ch)) { pl_trail_unwind(&g_pl_trail, mark); return 0; } char c2[2] = { (char)toupper(ch), 0 }; out = plc_make_atom_cell(c2); }
        else if (!strcmp(ty, "code"))  { out = pl_make_int((int64_t)ch); }
        else { pl_trail_unwind(&g_pl_trail, mark); return 0; }
        if (!plc_unify_into_cell(inner, out, &g_pl_trail)) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
        return 1;
    }
    if ((int)td->v != DT_A && (int)td->v != DT_S) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
    const char *ty = ((int)td->v == DT_S) ? (td->s ? td->s : "") : prolog_atom_name(pl_atom_id(td));
    if (!ty) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
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
    else { pl_trail_unwind(&g_pl_trail, mark); return 0; }
    if (!ok) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_lower_upper_cell(void *lower_cell, void *upper_cell)
{
    extern pl_trail_t g_pl_trail;
    if (!lower_cell || !upper_cell) return 0;
    pl_cell_t *lo = pl_deref((pl_cell_t *)lower_cell);
    pl_cell_t *up = pl_deref((pl_cell_t *)upper_cell);
    int lo_bound = !pl_cell_unbound(lo), up_bound = !pl_cell_unbound(up);
    if (!lo_bound && !up_bound) { extern void rt_pl_iso_throw_instantiation(void); rt_pl_iso_throw_instantiation(); return 0; }
    char b0[256];
    if (lo_bound) {
        const char *cs = atom_op_text(pl_cell_to_term(lo), b0, sizeof b0);
        if (!cs || !cs[0] || cs[1]) return 0;
        char c2[2] = { (char)toupper((unsigned char)cs[0]), 0 };
        int mark = pl_trail_mark(&g_pl_trail);
        if (!pl_unify_term_into_cell(up, term_new_atom(prolog_atom_intern(c2)), &g_pl_trail)) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
        return 1;
    }
    const char *cs = atom_op_text(pl_cell_to_term(up), b0, sizeof b0);
    if (!cs || !cs[0] || cs[1]) return 0;
    char c2[2] = { (char)tolower((unsigned char)cs[0]), 0 };
    int mark = pl_trail_mark(&g_pl_trail);
    if (!pl_unify_term_into_cell(lo, term_new_atom(prolog_atom_intern(c2)), &g_pl_trail)) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_pl_term_class(Term *t) {
    switch (t->tag) {
    case TERM_VAR:      return 0;
    case TERM_FLOAT:    return 1;
    case TERM_INT:      return 1;
    case TERM_ATOM:     return 2;
    case TERM_COMPOUND: return 3;
    default:            return 4;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_pl_term_compare(Term *a, Term *b) {
    a = a ? term_deref(a) : NULL; b = b ? term_deref(b) : NULL;
    if (!a && !b) return 0; if (!a) return -1; if (!b) return 1;
    int ca = rt_pl_term_class(a), cb = rt_pl_term_class(b);
    if (ca != cb) return ca < cb ? -1 : 1;
    switch (a->tag) {
    case TERM_VAR: return (a == b) ? 0 : (a < b ? -1 : 1);
    case TERM_INT: { double x = (double)a->ival, y = (b->tag == TERM_INT) ? (double)b->ival : b->fval; return x < y ? -1 : (x > y ? 1 : 0); }
    case TERM_FLOAT: { double x = a->fval, y = (b->tag == TERM_INT) ? (double)b->ival : b->fval; return x < y ? -1 : (x > y ? 1 : 0); }
    case TERM_ATOM: { const char *na = prolog_atom_name(a->atom_id), *nb = prolog_atom_name(b->atom_id); int c = strcmp(na ? na : "", nb ? nb : ""); return c < 0 ? -1 : (c > 0 ? 1 : 0); }
    case TERM_COMPOUND: {
        if (a->compound.arity != b->compound.arity) return a->compound.arity < b->compound.arity ? -1 : 1;
        const char *na = prolog_atom_name(a->compound.functor), *nb = prolog_atom_name(b->compound.functor);
        int c = strcmp(na ? na : "", nb ? nb : ""); if (c) return c < 0 ? -1 : 1;
        for (int i = 0; i < a->compound.arity; i++) { int r = rt_pl_term_compare(a->compound.args[i], b->compound.args[i]); if (r) return r; }
        return 0;
    }
    default: return 0;
    }
}
static Term *pl_cell_copy_walk(pl_cell_t *c, pl_cell_t **vaddr, Term **vterm, int *vn, int cap);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Cell-native standard order of terms (slice s2). WHY AN INDEX MAP AND NOT A POINTER TEST: pl_cell_copy_walk minted one fresh heap node per distinct unbound cell from rt_pl_cterm_alloc, a strict bump
   arena, so rt_pl_term_compare's `a < b` on TERM_VAR ordered variables by FIRST-ENCOUNTER position over (all of a, then all of b) -- never by the address of the variable cell itself. pl_vord_t
   records that position, and that is what keeps this path byte-identical to the converter path it replaces. Cap 256 and the walk order below are inherited from pl_cell_copy_walk deliberately: they are
   the behaviour being preserved, not a limit to round up. */
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
    if (t != DT_I && t != DT_A && t != DT_S && t != DT_R) m->next++;   /* copy_walk fell through to term_new_var(-1): one fresh var per occurrence, so the counter advances, nothing memoised */
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_pl_cell_class(pl_cell_t *d) {
    int t = (int)d->v;
    if (pl_cell_unbound(d)) return 0;
    if (t == DT_I || t == DT_R) return 1;
    if (t == DT_A || t == DT_S) return 2;   /* copy_walk interned DT_S to an atom, so a string sorts AS its atom, not as a fifth class -- see QA in the s2 baton */
    if (t == DT_PLREF) return 3;
    return 0;                               /* unrecognised tag became term_new_var(-1), i.e. class 0, NOT the unreachable class 4 of rt_pl_term_class */
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
int rt_pl_compare_cell(void *order_cell, void *a_cell, void *b_cell)
{
    extern pl_trail_t g_pl_trail;
    pl_vord_t m; m.n = 0; m.next = 0;
    rt_pl_vord_walk((pl_cell_t *)a_cell, &m);
    rt_pl_vord_walk((pl_cell_t *)b_cell, &m);
    int c = rt_pl_cell_compare((pl_cell_t *)a_cell, (pl_cell_t *)b_cell, &m);
    const char *nm = (c < 0) ? "<" : (c > 0) ? ">" : "=";
    const char *an = prolog_atom_name(prolog_atom_intern(nm));   /* DT_S, not pl_make_atom's DT_A -- Prolog atoms ride DT_S (pl_cell_conv.h:70-71) */
    pl_cell_t ord; ord.v = DT_S; ord.slen = (uint32_t)(an ? strlen(an) : 0); ord.s = an ? an : nm;
    int mark = pl_trail_mark(&g_pl_trail);
    if (!pl_unify((pl_cell_t *)order_cell, &ord, &g_pl_trail)) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_can_compare_cell(void *a_cell, void *b_cell)
{
    extern pl_trail_t g_pl_trail;
    pl_cell_t *a = (pl_cell_t *)a_cell, *b = (pl_cell_t *)b_cell;
    if (!a || !b) return 0;
    int mark = pl_trail_mark(&g_pl_trail);
    int unified = pl_unify(a, b, &g_pl_trail);
    int moved = (pl_trail_mark(&g_pl_trail) != mark);
    pl_trail_unwind(&g_pl_trail, mark);
    return !unified || !moved;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rt_pl_tv_walk(pl_cell_t *c, pl_cell_t **pool, int *pool_n, int cap) {
    if (!c) return;
    pl_cell_t *d = pl_deref(c);
    if (pl_cell_unbound(d)) { for (int i = 0; i < *pool_n; i++) if (pool[i] == d) return; if (*pool_n < cap) pool[(*pool_n)++] = d; return; }
    if ((int)d->v == (int)DT_PLREF) { int ar = (int)(d->slen & 0xFFFFu); pl_cell_t *aa = (pl_cell_t *)d->p; for (int i = 0; i < ar; i++) rt_pl_tv_walk(&aa[i], pool, pool_n, cap); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_term_variables_cell(void *term_cell, void *vars_cell, void *tail_cell)
{
    extern pl_trail_t g_pl_trail;
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
    int mark = pl_trail_mark(&g_pl_trail);
    if (!pl_unify((pl_cell_t *)vars_cell, &result, &g_pl_trail)) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_subsumes_cell(void *gen_cell, void *spec_cell)
{
    extern pl_trail_t g_pl_trail;
    pl_cell_t *g = (pl_cell_t *)gen_cell, *s = (pl_cell_t *)spec_cell;
    if (!g || !s) return 0;
    pl_cell_t *svars[8192]; int sn = 0;
    rt_pl_tv_walk(s, svars, &sn, 8192);
    int mark = pl_trail_mark(&g_pl_trail);
    int unified = pl_unify(g, s, &g_pl_trail);
    int bound_spec = 0;
    if (unified) for (int i = 0; i < sn; i++) if (!pl_cell_unbound(pl_deref(svars[i]))) { bound_spec = 1; break; }
    pl_trail_unwind(&g_pl_trail, mark);
    return unified && !bound_spec;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_sort_cell(int do_msort, void *list_cell, void *result_cell)
{
    extern pl_trail_t g_pl_trail;
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
    int mark = pl_trail_mark(&g_pl_trail);
    if (!pl_unify((pl_cell_t *)result_cell, &result, &g_pl_trail)) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pb_canon_walk(Term *t, Term **pool, int *pool_n, int save, int *next, int cap) {
    t = term_deref(t);
    if (!t) return;
    if (t->tag == TERM_VAR) {
        for (int i = 0; i < *pool_n; i++) if (pool[i] == t) return;
        if (*next < save) { t->tag = TERM_REF; t->ref = pool[(*next)++]; return; }
        if (*pool_n < cap) pool[(*pool_n)++] = t;
        return;
    }
    if (t->tag == TERM_COMPOUND) for (int i = 0; i < t->compound.arity; i++) pb_canon_walk(t->compound.args[i], pool, pool_n, save, next, cap);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pb_pairs_extract(void *list_cell, Term **elems, int cap, int dot_id, int dash_id) {
    Term *lst = pl_cell_to_term((pl_cell_t *)list_cell);
    int n = 0;
    Term *cur = lst;
    while (cur && cur->tag == TERM_COMPOUND && cur->compound.functor == dot_id && cur->compound.arity == 2 && n < cap) {
        Term *p = term_deref(cur->compound.args[0]);
        if (!(p && p->tag == TERM_COMPOUND && p->compound.functor == dash_id && p->compound.arity == 2)) return -1;
        elems[n++] = p;
        cur = term_deref(cur->compound.args[1]);
    }
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_bag_prep_cell(int is_setof, void *list_cell, void *result_cell)
{
    extern pl_trail_t g_pl_trail;
    int dot_id = prolog_atom_intern("."); int dash_id = prolog_atom_intern("-");
    Term *elems[4096];
    int n = pb_pairs_extract(list_cell, elems, 4096, dot_id, dash_id);
    if (n < 0) return 0;
    Term *pool[1024]; int pool_n = 0;
    for (int i = 0; i < n; i++) { int save = pool_n; int next = 0; pb_canon_walk(elems[i]->compound.args[0], pool, &pool_n, save, &next, 1024); }
    for (int i = 1; i < n; i++) {
        Term *key = elems[i]; int j = i - 1;
        if (is_setof) { while (j >= 0 && rt_pl_term_compare(elems[j], key) > 0) { elems[j + 1] = elems[j]; j--; } }
        else { while (j >= 0 && rt_pl_term_compare(term_deref(elems[j]->compound.args[0]), term_deref(key->compound.args[0])) > 0) { elems[j + 1] = elems[j]; j--; } }
        elems[j + 1] = key;
    }
    int m = 0; int out_idx[4096];
    for (int i = 0; i < n; i++) {
        if (is_setof && m > 0 && rt_pl_term_compare(elems[out_idx[m - 1]], elems[i]) == 0) continue;
        out_idx[m++] = i;
    }
    Term *result = term_new_atom(prolog_atom_intern("[]"));
    for (int i = m - 1; i >= 0; i--) { Term *pair[2]; pair[0] = elems[out_idx[i]]; pair[1] = result; result = term_new_compound(dot_id, 2, pair); }
    int mark = pl_trail_mark(&g_pl_trail);
    if (!pl_unify_term_into_cell((pl_cell_t *)result_cell, result, &g_pl_trail)) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_keysort_cell(void *list_cell, void *result_cell)
{
    extern pl_trail_t g_pl_trail;
    int dot_id = prolog_atom_intern("."); int dash_id = prolog_atom_intern("-");
    pl_cell_t *elems[4096]; int n = 0;
    pl_cell_t *cur = pl_deref((pl_cell_t *)list_cell);
    while (cur && (int)cur->v == DT_PLREF && (int)(cur->slen >> 16) == dot_id && (int)(cur->slen & 0xFFFFu) == 2 && n < 4096) {
        pl_cell_t *aa = (pl_cell_t *)cur->p;
        pl_cell_t *p = pl_deref(&aa[0]);
        if (!((int)p->v == DT_PLREF && (int)(p->slen >> 16) == dash_id && (int)(p->slen & 0xFFFFu) == 2)) return 0;
        elems[n++] = p;
        cur = pl_deref(&aa[1]);
    }
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
    int mark = pl_trail_mark(&g_pl_trail);
    if (!pl_unify((pl_cell_t *)result_cell, &result, &g_pl_trail)) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_plain_list_extract(void *list_cell, Term **elems, int cap, int dot_id) {
    Term *cur = term_deref(pl_cell_to_term((pl_cell_t *)list_cell));
    int n = 0;
    while (cur && cur->tag == TERM_COMPOUND && cur->compound.functor == dot_id && cur->compound.arity == 2 && n < cap) { elems[n++] = cur->compound.args[0]; cur = term_deref(cur->compound.args[1]); }
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_group_pairs_by_key_cell(void *list_cell, void *result_cell)
{
    extern pl_trail_t g_pl_trail;
    int dot_id = prolog_atom_intern("."); int dash_id = prolog_atom_intern("-");
    Term *elems[4096];
    int n = pb_pairs_extract(list_cell, elems, 4096, dot_id, dash_id);
    if (n < 0) return 0;
    Term *groups[4096]; int ng = 0;
    int i = 0;
    while (i < n) {
        Term *key = term_deref(elems[i]->compound.args[0]);
        Term *vlist[4096]; int nv = 0; int j = i;
        while (j < n && rt_pl_term_compare(term_deref(elems[j]->compound.args[0]), key) == 0 && nv < 4096) { vlist[nv++] = elems[j]->compound.args[1]; j++; }
        Term *vals = term_new_atom(prolog_atom_intern("[]"));
        for (int k = nv - 1; k >= 0; k--) { Term *vp[2]; vp[0] = vlist[k]; vp[1] = vals; vals = term_new_compound(dot_id, 2, vp); }
        Term *grp[2]; grp[0] = key; grp[1] = vals; groups[ng++] = term_new_compound(dash_id, 2, grp);
        i = j;
    }
    Term *result = term_new_atom(prolog_atom_intern("[]"));
    for (int k = ng - 1; k >= 0; k--) { Term *gp[2]; gp[0] = groups[k]; gp[1] = result; result = term_new_compound(dot_id, 2, gp); }
    int mark = pl_trail_mark(&g_pl_trail);
    if (!pl_unify_term_into_cell((pl_cell_t *)result_cell, result, &g_pl_trail)) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_pairs_keys_values_cell(void *pairs_cell, void *keys_cell, void *values_cell)
{
    extern pl_trail_t g_pl_trail;
    int dot_id = prolog_atom_intern("."); int dash_id = prolog_atom_intern("-");
    Term *pairs_t = term_deref(pl_cell_to_term((pl_cell_t *)pairs_cell));
    int mark = pl_trail_mark(&g_pl_trail);
    if (pairs_t && pairs_t->tag != TERM_VAR) {
        Term *elems[4096]; int n = pb_pairs_extract(pairs_cell, elems, 4096, dot_id, dash_id);
        if (n < 0) return 0;
        Term *keys = term_new_atom(prolog_atom_intern("[]")); Term *vals = term_new_atom(prolog_atom_intern("[]"));
        for (int i = n - 1; i >= 0; i--) {
            Term *kp[2]; kp[0] = elems[i]->compound.args[0]; kp[1] = keys; keys = term_new_compound(dot_id, 2, kp);
            Term *vp[2]; vp[0] = elems[i]->compound.args[1]; vp[1] = vals; vals = term_new_compound(dot_id, 2, vp);
        }
        if (!pl_unify_term_into_cell((pl_cell_t *)keys_cell, keys, &g_pl_trail) || !pl_unify_term_into_cell((pl_cell_t *)values_cell, vals, &g_pl_trail)) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
        return 1;
    }
    Term *kelems[4096]; int nk = pl_plain_list_extract(keys_cell, kelems, 4096, dot_id);
    Term *velems[4096]; int nv = pl_plain_list_extract(values_cell, velems, 4096, dot_id);
    if (nk != nv) return 0;
    Term *result = term_new_atom(prolog_atom_intern("[]"));
    for (int i = nk - 1; i >= 0; i--) {
        Term *pr[2]; pr[0] = kelems[i]; pr[1] = velems[i]; Term *pair_term = term_new_compound(dash_id, 2, pr);
        Term *lp[2]; lp[0] = pair_term; lp[1] = result; result = term_new_compound(dot_id, 2, lp);
    }
    if (!pl_unify_term_into_cell((pl_cell_t *)pairs_cell, result, &g_pl_trail)) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
    return 1;
}
typedef struct { Term *rest; int mark; } pl_baggrp_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_bag_group_gen(DESCR_t *args, int nargs, int64_t *resume)
{
    extern pl_trail_t g_pl_trail;
    if (nargs < 3) return FAILDESCR;
    if (*resume == 0) {
        pl_baggrp_t *it = (pl_baggrp_t *)rt_ws_alloc(sizeof *it);
        DESCR_t t0 = args[0];
        it->rest = pl_cell_to_term((pl_cell_t *)&t0);
        it->mark = pl_trail_mark(&g_pl_trail);
        *resume = (int64_t)(intptr_t)it;
    }
    pl_baggrp_t *it = (pl_baggrp_t *)(intptr_t)*resume;
    int dot_id = prolog_atom_intern("."); int dash_id = prolog_atom_intern("-");
    pl_trail_unwind(&g_pl_trail, it->mark);
    Term *cur = term_deref(it->rest);
    if (!(cur && cur->tag == TERM_COMPOUND && cur->compound.functor == dot_id && cur->compound.arity == 2)) return FAILDESCR;
    Term *first = term_deref(cur->compound.args[0]);
    if (!(first && first->tag == TERM_COMPOUND && first->compound.functor == dash_id && first->compound.arity == 2)) return FAILDESCR;
    Term *key = term_deref(first->compound.args[0]);
    Term *vals[4096]; int nv = 0;
    vals[nv++] = first->compound.args[1];
    Term *rest = term_deref(cur->compound.args[1]);
    while (rest && rest->tag == TERM_COMPOUND && rest->compound.functor == dot_id && rest->compound.arity == 2 && nv < 4096) {
        Term *p = term_deref(rest->compound.args[0]);
        if (!(p && p->tag == TERM_COMPOUND && p->compound.functor == dash_id && p->compound.arity == 2)) break;
        if (rt_pl_term_compare(key, term_deref(p->compound.args[0])) != 0) break;
        vals[nv++] = p->compound.args[1];
        rest = term_deref(rest->compound.args[1]);
    }
    it->rest = rest;
    Term *vlist = term_new_atom(prolog_atom_intern("[]"));
    for (int i = nv - 1; i >= 0; i--) { Term *pr[2]; pr[0] = vals[i]; pr[1] = vlist; vlist = term_new_compound(dot_id, 2, pr); }
    Term *kp[2]; kp[0] = key; kp[1] = vlist;
    Term *pairT = term_new_compound(dash_id, 2, kp);
    pl_cell_t *kids = (pl_cell_t *)rt_ws_alloc(2 * sizeof(pl_cell_t));
    kids[0] = *(pl_cell_t *)&args[1]; kids[1] = *(pl_cell_t *)&args[2];
    pl_cell_t pc = pl_make_compound(dash_id, 2, kids);
    if (!pl_unify_term_into_cell(&pc, pairT, &g_pl_trail)) { pl_trail_unwind(&g_pl_trail, it->mark); return FAILDESCR; }
    { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; return r; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long pl_numbervars_walk(pl_cell_t *c, long counter, int var_id, pl_trail_t *trail)
{
    pl_cell_t *d = pl_deref(c);
    if (pl_cell_unbound(d)) {
        pl_cell_t *a = (pl_cell_t *)rt_ws_alloc(sizeof(pl_cell_t));
        *a = pl_make_int(counter++);
        pl_bind(d, pl_make_compound(var_id, 1, a), trail);
        return counter;
    }
    if ((int)d->v == DT_PLREF) {
        int ar = pl_arity(d); pl_cell_t *aa = (pl_cell_t *)pl_compound_heap(d);
        for (int i = 0; i < ar; i++) counter = pl_numbervars_walk(&aa[i], counter, var_id, trail);
    }
    return counter;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_numbervars_cell(void *term_cell, void *start_cell, void *end_cell) {
    extern pl_trail_t g_pl_trail;
    Term *st = pl_cell_to_term((pl_cell_t *)start_cell);
    if (!st || st->tag != TERM_INT) return 0;
    long counter = st->ival;
    int var_id = prolog_atom_intern("$VAR");
    int mark = pl_trail_mark(&g_pl_trail);
    counter = pl_numbervars_walk((pl_cell_t *)term_cell, counter, var_id, &g_pl_trail);
    if (!pl_unify_term_into_cell((pl_cell_t *)end_cell, term_new_int(counter), &g_pl_trail)) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_numbervars1_cell(void *term_cell) {
    extern pl_trail_t g_pl_trail;
    int var_id = prolog_atom_intern("$VAR");
    pl_numbervars_walk((pl_cell_t *)term_cell, 0, var_id, &g_pl_trail);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_get_print_stream_cell(void *stream_cell) {
    extern pl_trail_t g_pl_trail; extern int fh_current_output(void);
    int stream_id = prolog_atom_intern("$stream");
    pl_cell_t *arg = (pl_cell_t *)rt_ws_alloc(sizeof(pl_cell_t)); *arg = pl_make_int(fh_current_output());
    pl_cell_t sc = pl_make_compound(stream_id, 1, arg);
    int mark = pl_trail_mark(&g_pl_trail);
    if (!pl_unify((pl_cell_t *)stream_cell, &sc, &g_pl_trail)) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
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
    extern pl_trail_t g_pl_trail;
    pl_cell_t *pool[8192]; int counts[8192]; int pn = 0;
    pl_distinct_vars_walk((pl_cell_t *)term_cell, pool, counts, &pn, 8192);
    int varname_id = prolog_atom_intern("$VARNAME"); int underscore_id = prolog_atom_intern("_");
    for (int i = 0; i < pn; i++) {
        if (counts[i] != 1) continue;
        pl_cell_t *arg = (pl_cell_t *)rt_ws_alloc(sizeof(pl_cell_t)); *arg = pl_make_atom(underscore_id);
        pl_bind(pool[i], pl_make_compound(varname_id, 1, arg), &g_pl_trail);
    }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_name_query_vars_cell(void *list_cell, void *rest_cell) {
    extern pl_trail_t g_pl_trail; extern void rt_pl_iso_throw_instantiation(void); extern void rt_pl_iso_throw_type(const char *, DESCR_t);
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
                    pl_bind(vr, pl_make_compound(varname_id, 1, arg), &g_pl_trail);
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
    int mark = pl_trail_mark(&g_pl_trail);
    if (!pl_unify((pl_cell_t *)rest_cell, &result, &g_pl_trail)) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
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
    extern pl_trail_t g_pl_trail; extern void rt_pl_iso_throw_instantiation(void);
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
        if (use_namevars) { char nb[32]; pl_namevars_letters(n, nb, sizeof nb); *arg = pl_make_atom(prolog_atom_intern(nb)); pl_bind(pool[i], pl_make_compound(varname_id, 1, arg), &g_pl_trail); }
        else { *arg = pl_make_int(n); pl_bind(pool[i], pl_make_compound(var_id, 1, arg), &g_pl_trail); }
        n++;
    }
    if (next_var) {
        int mark = pl_trail_mark(&g_pl_trail); pl_cell_t nv = pl_make_int(n);
        if (!pl_unify(next_var, &nv, &g_pl_trail)) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
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
int rt_pl_term_string_cell(void *term_cell, void *str_cell)
{
    extern pl_trail_t g_pl_trail;
    extern void pl_writeq(Term *);
    int mark = pl_trail_mark(&g_pl_trail);
    Term *t = pl_cell_to_term((pl_cell_t *)term_cell);
    int pipefd[2];
    if (pipe(pipefd) != 0) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
    int saved_fd = dup(STDOUT_FILENO);
    if (saved_fd < 0) { close(pipefd[0]); close(pipefd[1]); pl_trail_unwind(&g_pl_trail, mark); return 0; }
    dup2(pipefd[1], STDOUT_FILENO); close(pipefd[1]);
    pl_writeq(t);
    fflush(stdout);
    dup2(saved_fd, STDOUT_FILENO); close(saved_fd);
    char buf[4096]; ssize_t n = read(pipefd[0], buf, sizeof buf - 1); close(pipefd[0]);
    if (n < 0) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
    buf[n] = '\0';
    int atom_id = prolog_atom_intern(buf);
    if (!pl_unify_term_into_cell((pl_cell_t *)str_cell, term_new_atom(atom_id), &g_pl_trail)) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static Term *copy_term_deep(Term *t, Term **var_map, int *var_cap, int *var_n)
{
    if (!t) return NULL;
    t = term_deref(t);
    if (!t) return NULL;
    if (t->tag == TERM_VAR) {
        for (int i = 0; i < *var_n; i += 2) if (var_map[i] == t) return var_map[i+1];
        if (*var_n + 2 > *var_cap) return NULL;
        Term *fresh = term_new_var(-1);
        var_map[(*var_n)++] = t; var_map[(*var_n)++] = fresh;
        return fresh;
    }
    if (t->tag == TERM_ATOM || t->tag == TERM_INT || t->tag == TERM_FLOAT) return t;
    if (t->tag == TERM_COMPOUND) {
        Term **args = (Term **)rt_ws_alloc(t->compound.arity * sizeof(Term *));
        if (!args) return NULL;
        for (int i = 0; i < t->compound.arity; i++) { args[i] = copy_term_deep(t->compound.args[i], var_map, var_cap, var_n); if (!args[i] && t->compound.args[i]) return NULL; }
        return term_new_compound(t->compound.functor, t->compound.arity, args);
    }
    return t;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static Term *pl_cell_copy_walk(pl_cell_t *c, pl_cell_t **vaddr, Term **vterm, int *vn, int cap)
{
    pl_cell_t *d = pl_deref(c);
    int t = (int)d->v;
    if (pl_cell_unbound(d)) {
        extern int g_pl_copy_slot_mode; extern int g_pl_copy_slot_ctr;
        for (int i = 0; i < *vn; i++) if (vaddr[i] == d) return vterm[i];
        Term *fresh = term_new_var(g_pl_copy_slot_mode ? g_pl_copy_slot_ctr++ : -1);
        if (*vn < cap) { vaddr[*vn] = d; vterm[*vn] = fresh; (*vn)++; }
        return fresh;
    }
    if (t == DT_I) return term_new_int((long)d->i);
    if (t == DT_A) return term_new_atom((int)d->i);
    if (t == DT_S) { extern int prolog_atom_intern(const char *); return term_new_atom(prolog_atom_intern(d->s ? d->s : "")); }
    if (t == DT_R) return term_new_float(d->r);
    if (t == DT_PLREF) {
        int fn = (int)(d->slen >> 16), ar = (int)(d->slen & 0xFFFFu);
        pl_cell_t *aa = (pl_cell_t *)d->p;
        Term **args = (Term **)rt_ws_alloc((size_t)(ar > 0 ? ar : 1) * sizeof(Term *));
        for (int i = 0; i < ar; i++) args[i] = pl_cell_copy_walk(&aa[i], vaddr, vterm, vn, cap);
        return term_new_compound(fn, ar, args);
    }
    return term_new_var(-1);
}
/* CELL-NATIVE deep copy -- the DESCR twin of copy_term_deep/pl_cell_copy_walk, which build heap Terms.
 * Structure never lives on the heap (GOAL-PROLOG-100 top block): a compound copy is a fresh cell array,
 * an atom/int/float/string copies BY VALUE, and only unbound vars need fresh storage. Slot numbering is
 * shared with pl_cell_copy_walk (same counter, same recursion order) so copy_term's printed variable
 * names are unchanged. ⛔ pl_cell_copy_walk is NOT deleted: 7 functions in OTHER slices still consume its
 * Term return (rt_pl_atop_cell, rt_pl_compare_cell, the rt_pl_dyn_* family, rt_pl_throw_set). */
static pl_cell_t pl_cell_copy_cells(pl_cell_t *c, pl_cell_t **vaddr, pl_cell_t **vnew, int *vn, int cap)
{
    extern int g_pl_copy_slot_mode; extern int g_pl_copy_slot_ctr;
    pl_cell_t *d = pl_deref(c);
    if (pl_cell_unbound(d)) {
        for (int i = 0; i < *vn; i++) if (vaddr[i] == d) return pl_make_ref(vnew[i], (int)vnew[i]->slen);
        pl_cell_t *fresh = (pl_cell_t *)rt_ws_alloc(sizeof(pl_cell_t));
        if (!fresh) return *d;
        pl_init_var(fresh, g_pl_copy_slot_mode ? g_pl_copy_slot_ctr++ : -1);
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
/* The findall/bagof SNAPSHOT, exported for by_name_dispatch's accumulator: capture a template's CURRENT
 * bindings as cells before backtracking undoes them. Same primitive the in-file services use, so both
 * accumulators agree on the element type -- they are the SAME allocation reached by two names. */
pl_cell_t rt_pl_cell_snapshot(void *cell)
{
    pl_cell_t *vaddr[256]; pl_cell_t *vnew[256]; int vn = 0;
    return pl_cell_copy_cells((pl_cell_t *)cell, vaddr, vnew, &vn, 256);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* An atom's identity as an id, read straight off the cell: DT_A carries it, DT_S carries the name. */
static int pl_cell_atom_id(pl_cell_t *c)
{
    pl_cell_t *d = pl_deref(c);
    if ((int)d->v == DT_A) return (int)d->i;
    if ((int)d->v == DT_S) { extern int prolog_atom_intern(const char *); return prolog_atom_intern(d->s ? d->s : ""); }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int g_pl_copy_slot_mode = 0;
int g_pl_copy_slot_ctr = 1048576;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_copy_term_cell(void *term_cell, void *copy_cell)
{
    extern pl_trail_t g_pl_trail;
    int mark = pl_trail_mark(&g_pl_trail);
    pl_cell_t *vaddr[256]; pl_cell_t *vnew[256]; int vn = 0;
    g_pl_copy_slot_mode = 1; g_pl_copy_slot_ctr = 1048576;
    pl_cell_t copy = pl_cell_copy_cells((pl_cell_t *)term_cell, vaddr, vnew, &vn, 256);
    g_pl_copy_slot_mode = 0;
    if (!pl_unify((pl_cell_t *)copy_cell, &copy, &g_pl_trail)) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
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
int rt_pl_findall_finish(void *acc_v, void *result_term)
{
    extern pl_trail_t g_pl_trail;
    pl_findall_acc *a = (pl_findall_acc *)acc_v;
    int dot = prolog_atom_intern(".");
    pl_cell_t lst; { extern const char *prolog_atom_name(int); const char *nm = prolog_atom_name(prolog_atom_intern("[]"));
        lst.v = DT_S; lst.slen = (uint32_t)(nm ? strlen(nm) : 0); lst.s = nm ? nm : ""; }  /* ⛔ NOT pl_make_atom: it builds DT_A, but Prolog atoms ride DT_S (pl_cell_conv.h:70) -- hq_C measured the DT_A form core-dumping predicate_property (baton ledger 2026-09-01, seat10 on s5). */
    int n = a ? a->n : 0;
    for (int i = n - 1; i >= 0; i--) {
        pl_cell_t *c = (pl_cell_t *)rt_ws_alloc(2 * sizeof(pl_cell_t)); if (!c) return 0;
        c[0] = a->items[i]; c[1] = lst;
        lst = pl_make_compound(dot, 2, c);
    }
    int mark = pl_trail_mark(&g_pl_trail);
    if (!pl_unify((pl_cell_t *)result_term, &lst, &g_pl_trail)) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_agg_count_finish(void *acc_v, void *result_term)
{
    extern pl_trail_t g_pl_trail;
    pl_findall_acc *a = (pl_findall_acc *)acc_v;
    int mark = pl_trail_mark(&g_pl_trail);
    if (!pl_unify_term_into_cell((pl_cell_t *)result_term, term_new_int(a ? a->n : 0), &g_pl_trail)) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
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
    extern pl_trail_t g_pl_trail;
    pl_findall_acc *a = (pl_findall_acc *)acc_v;
    int n = a ? a->n : 0;
    long si = 0; double sd = 0.0; int isf = 0;
    for (int i = 0; i < n; i++) {
        long iv = 0; double dv = 0.0; int ef = 0;
        if (!agg_num(&a->items[i], &iv, &dv, &ef)) return 0;
        if (ef) { sd += dv; isf = 1; } else { si += iv; sd += (double)iv; }
    }
    int mark = pl_trail_mark(&g_pl_trail);
    pl_cell_t r = isf ? pl_make_float(sd) : pl_make_int((int64_t)si);
    if (!pl_unify((pl_cell_t *)result_term, &r, &g_pl_trail)) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_pl_agg_minmax_finish(void *acc_v, void *result_term, int want_max)
{
    extern pl_trail_t g_pl_trail;
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
    int mark = pl_trail_mark(&g_pl_trail);
    pl_cell_t r = isf ? pl_make_float(bd) : pl_make_int((int64_t)bi);
    if (!pl_unify((pl_cell_t *)result_term, &r, &g_pl_trail)) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_agg_max_finish(void *acc_v, void *result_term) { return rt_pl_agg_minmax_finish(acc_v, result_term, 1); }
int rt_pl_agg_min_finish(void *acc_v, void *result_term) { return rt_pl_agg_minmax_finish(acc_v, result_term, 0); }
static pl_cell_t **g_rt_pl_nb = (pl_cell_t **)0;
static int g_rt_pl_nb_n = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_pl_nb_ensure(int id)
{
    if (id < 0) return 0;
    if (id >= g_rt_pl_nb_n) {
        extern void *rt_ws_alloc(size_t); extern void *rt_ws_realloc(void *, size_t);
        int nc = g_rt_pl_nb_n ? g_rt_pl_nb_n : 16; while (nc <= id) nc *= 2;
        pl_cell_t **nv = (pl_cell_t **)(g_rt_pl_nb ? rt_ws_realloc(g_rt_pl_nb, (size_t)nc * sizeof(pl_cell_t *)) : rt_ws_alloc((size_t)nc * sizeof(pl_cell_t *)));
        if (!nv) return 0;
        for (int i = g_rt_pl_nb_n; i < nc; i++) nv[i] = (pl_cell_t *)0;
        g_rt_pl_nb = nv; g_rt_pl_nb_n = nc;
    }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_nb_copy_persist(void *val_cell)
{
    pl_cell_t *vaddr[256]; pl_cell_t *vnew[256]; int vn = 0;
    pl_cell_t *out = (pl_cell_t *)rt_ws_alloc(sizeof(pl_cell_t));
    if (!out) return (void *)0;
    *out = pl_cell_copy_cells((pl_cell_t *)val_cell, vaddr, vnew, &vn, 256);
    return (void *)out;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_nb_getval_ptr(void *stored_cell, void *val_cell)
{
    extern pl_trail_t g_pl_trail;
    pl_cell_t *val = (pl_cell_t *)stored_cell;
    if (!val) return 0;
    int mark = pl_trail_mark(&g_pl_trail);
    pl_cell_t *vaddr[256]; pl_cell_t *vnew[256]; int vn = 0;
    pl_cell_t fresh = pl_cell_copy_cells(val, vaddr, vnew, &vn, 256);
    if (!pl_unify((pl_cell_t *)val_cell, &fresh, &g_pl_trail)) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_nb_setval_cell(void *key_cell, void *val_cell)
{
    int id = pl_cell_atom_id((pl_cell_t *)key_cell);
    if (id < 0) return 0;
    if (!rt_pl_nb_ensure(id)) return 0;
    g_rt_pl_nb[id] = (pl_cell_t *)rt_pl_nb_copy_persist(val_cell);
    return g_rt_pl_nb[id] ? 1 : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_nb_getval_cell(void *key_cell, void *val_cell)
{
    int id = pl_cell_atom_id((pl_cell_t *)key_cell);
    if (id < 0) return 0;
    pl_cell_t *val = (id < g_rt_pl_nb_n) ? g_rt_pl_nb[id] : (pl_cell_t *)0;
    return rt_pl_nb_getval_ptr((void *)val, val_cell);
}
typedef struct dyn_clause { Term *head; Term *body; struct dyn_clause *next; } dyn_clause_t;
typedef struct { const char *name; long arity; dyn_clause_t *head; dyn_clause_t *tail; int abolished; } dyn_pred_row_t;
static dyn_pred_row_t *g_pl_dyn_pred_table = (dyn_pred_row_t *)0;
static long            g_pl_dyn_pred_n     = 0;
static long            g_pl_dyn_pred_cap   = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static dyn_pred_row_t *dyn_pred_find(const char *name, long arity)
{
    for (long i = 0; i < g_pl_dyn_pred_n; i++) if (g_pl_dyn_pred_table[i].name && !strcmp(g_pl_dyn_pred_table[i].name, name) && g_pl_dyn_pred_table[i].arity == arity) return &g_pl_dyn_pred_table[i];
    return (dyn_pred_row_t *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void dyn_term_key(pl_cell_t *c, const char **name_out, long *arity_out)
{
    pl_cell_t *d = c ? pl_deref(c) : (pl_cell_t *)0;
    if (d && (int)d->v == DT_PLREF) { *name_out = prolog_atom_name((int)(d->slen >> 16)); *arity_out = (long)(d->slen & 0xFFFFu); return; }
    if (d && (int)d->v == DT_A) { *name_out = prolog_atom_name((int)d->i); *arity_out = 0; return; }
    if (d && d->v == DT_S) { *name_out = d->s ? d->s : ""; *arity_out = 0; return; }
    *name_out = (const char *)0; *arity_out = 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_dyn_abolish_cell(void *fn_cell, void *ar_cell)
{
    pl_cell_t *fnc = pl_deref((pl_cell_t *)fn_cell);
    pl_cell_t *arc = pl_deref((pl_cell_t *)ar_cell);
    if (((int)fnc->v != DT_A && fnc->v != DT_S) || (int)arc->v != DT_I) return 1;
    const char *name = ((int)fnc->v == DT_A) ? prolog_atom_name((int)fnc->i) : (fnc->s ? fnc->s : "");
    if (!name) return 1;
    dyn_pred_row_t *row = dyn_pred_find(name, (long)arc->i);
    // abolish/1 (ISO 8.4.4) reverts the predicate to NOT DEFINED, unlike retractall/1 which leaves it
    // defined-with-zero-clauses -- the `abolished` flag is what lets rt_pl_dyn_iter_gen tell the two
    // apart and throw existence_error only for the abolished case (FINDING-2026-08-29-seat06).
    if (row) { row->head = (dyn_clause_t *)0; row->tail = (dyn_clause_t *)0; row->abolished = 1; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_dyn_retract_cell(void *head_cell)
{
    extern pl_trail_t g_pl_trail;
    const char *name = (const char *)0; long arity = 0;
    dyn_term_key((pl_cell_t *)head_cell, &name, &arity);
    if (!name) return 0;
    dyn_pred_row_t *row = dyn_pred_find(name, arity);
    if (!row) return 0;
    dyn_clause_t *prev = (dyn_clause_t *)0;
    for (dyn_clause_t *c = row->head; c; prev = c, c = c->next) {
        int mark = pl_trail_mark(&g_pl_trail);
        Term *var_map[256]; int var_cap = 256, var_n = 0;
        Term *hcopy = copy_term_deep(c->head, var_map, &var_cap, &var_n);
        if (hcopy && pl_unify_term_into_cell((pl_cell_t *)head_cell, hcopy, &g_pl_trail)) {
            if (prev) prev->next = c->next; else row->head = c->next;
            if (row->tail == c) row->tail = prev;
            return 1;
        }
        pl_trail_unwind(&g_pl_trail, mark);
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static dyn_pred_row_t *dyn_pred_intern(const char *name, long arity)
{
    dyn_pred_row_t *r = dyn_pred_find(name, arity);
    if (r) return r;
    if (g_pl_dyn_pred_n >= g_pl_dyn_pred_cap) {
        g_pl_dyn_pred_cap = g_pl_dyn_pred_cap ? g_pl_dyn_pred_cap * 2 : 8;
        g_pl_dyn_pred_table = (dyn_pred_row_t *)realloc(g_pl_dyn_pred_table, (size_t)g_pl_dyn_pred_cap * sizeof(dyn_pred_row_t));
    }
    r = &g_pl_dyn_pred_table[g_pl_dyn_pred_n++];
    r->name = name; r->arity = arity; r->head = (dyn_clause_t *)0; r->tail = (dyn_clause_t *)0; r->abolished = 0;
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_dyn_assertz_cell(void *clause_cell, int prepend)
{
    Term *cl = pl_cell_to_term((pl_cell_t *)clause_cell);
    if (!cl) return 1;
    Term *h = cl, *b = (Term *)0;
    if (cl->tag == TERM_COMPOUND && cl->compound.arity == 2 && prolog_atom_name(cl->compound.functor) && !strcmp(prolog_atom_name(cl->compound.functor), ":-")) {
        h = term_deref(cl->compound.args[0]); b = term_deref(cl->compound.args[1]);
    }
    const char *name = (const char *)0; long arity = 0;
    pl_cell_t *clc = pl_deref((pl_cell_t *)clause_cell); pl_cell_t *hc = clc;
    const char *clf = ((int)clc->v == DT_PLREF) ? prolog_atom_name((int)(clc->slen >> 16)) : (const char *)0;
    if (clf && (int)(clc->slen & 0xFFFFu) == 2 && !strcmp(clf, ":-")) hc = pl_deref(&((pl_cell_t *)clc->p)[0]);
    dyn_term_key(hc, &name, &arity);
    if (!name) return 1;
    Term *var_map[256]; int var_cap = 256, var_n = 0;
    Term *hcopy = copy_term_deep(h, var_map, &var_cap, &var_n);
    Term *bcopy = b ? copy_term_deep(b, var_map, &var_cap, &var_n) : (Term *)0;
    dyn_pred_row_t *row = dyn_pred_intern(name, arity);
    row->abolished = 0;
    dyn_clause_t *node = (dyn_clause_t *)rt_ws_alloc(sizeof *node);
    node->head = hcopy ? hcopy : h; node->body = bcopy; node->next = (dyn_clause_t *)0;
    if (prepend) { node->next = row->head; row->head = node; if (!row->tail) row->tail = node; }
    else { if (row->tail) row->tail->next = node; else row->head = node; row->tail = node; }
    return 1;
}
typedef struct { dyn_clause_t *next; } dyn_cursor_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_dyn_iter_begin(const char *name, long arity)
{
    dyn_pred_row_t *row = name ? dyn_pred_find(name, arity) : (dyn_pred_row_t *)0;
    dyn_cursor_t *cur = (dyn_cursor_t *)rt_ws_alloc(sizeof *cur);
    cur->next = row ? row->head : (dyn_clause_t *)0;
    return cur;
}
typedef struct { dyn_clause_t *cur; int mark; } pl_dyn_it_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dyn_iter_gen(DESCR_t *args, int nargs, int64_t *resume)
{
    extern pl_trail_t g_pl_trail;
    extern const char *prolog_atom_name(int);
    long arity = nargs - 1;
    if (*resume == 0) {
        const char *nm = (args[0].v == DT_S) ? args[0].s : ((int)args[0].v == DT_A) ? prolog_atom_name((int)args[0].i) : (const char *)0;
        dyn_pred_row_t *row = nm ? dyn_pred_find(nm, arity) : (dyn_pred_row_t *)0;
        // A row surviving abolish/1 with `abolished` set is a genuinely undefined procedure (never
        // re-asserted since) -- ISO 8.4.4 says calling it is an existence_error, not a silent fail;
        // a row that is merely empty (declared dynamic, or retractall'd) is not, and keeps falling
        // through to the zero-clause loop below, unchanged.
        if (row && row->abolished) { extern void rt_pl_iso_throw_pi(const char *, const char *, const char *, int); rt_pl_iso_throw_pi("existence_error", "procedure", nm, (int)arity); return FAILDESCR; }
        pl_dyn_it_t *it = (pl_dyn_it_t *)rt_ws_alloc(sizeof *it);
        it->cur = row ? row->head : (dyn_clause_t *)0;
        it->mark = pl_trail_mark(&g_pl_trail);
        *resume = (int64_t)(intptr_t)it;
    }
    pl_dyn_it_t *it = (pl_dyn_it_t *)(intptr_t)*resume;
    while (it->cur) {
        pl_trail_unwind(&g_pl_trail, it->mark);
        dyn_clause_t *c = it->cur;
        it->cur = c->next;
        Term *var_map[256]; int var_cap = 256, var_n = 0;
        Term *h = copy_term_deep(c->head, var_map, &var_cap, &var_n);
        int ok = 1;
        if (arity == 0) ok = (h != (Term *)0);
        else if (h && h->tag == TERM_COMPOUND && h->compound.arity == arity) {
            for (long i = 0; ok && i < arity; i++)
                ok = pl_unify_term_into_cell((pl_cell_t *)&args[1 + i], h->compound.args[i], &g_pl_trail);
        } else ok = 0;
        if (ok) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; return r; }
    }
    pl_trail_unwind(&g_pl_trail, it->mark);
    return FAILDESCR;
}
typedef struct { dyn_clause_t *cur; int mark; } pl_clause_it_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_clause_gen(DESCR_t *args, int nargs, int64_t *resume) {
    extern pl_trail_t g_pl_trail;
    extern void rt_pl_iso_throw_instantiation(void);
    extern void rt_pl_iso_throw_pi(const char *, const char *, const char *, int);
    extern void rt_pl_iso_throw_permission(const char *, const char *, const char *, int);
    extern int rt_pl_proc_defined_static(const char *, long);
    if (nargs < 2 || !resume) return FAILDESCR;
    if (*resume == 0) {
        Term *hd = pl_cell_to_term((pl_cell_t *)&args[0]);
        hd = hd ? term_deref(hd) : (Term *)0;
        if (!hd || hd->tag == TERM_VAR) { rt_pl_iso_throw_instantiation(); return FAILDESCR; }
        if (hd->tag != TERM_ATOM && hd->tag != TERM_COMPOUND) { rt_pl_iso_throw_pi("type_error", "callable", (hd->tag == TERM_INT) ? "integer" : "?", 0); return FAILDESCR; }
        Term *bd = pl_cell_to_term((pl_cell_t *)&args[1]);
        bd = bd ? term_deref(bd) : (Term *)0;
        if (bd && bd->tag != TERM_VAR && bd->tag != TERM_ATOM && bd->tag != TERM_COMPOUND) { rt_pl_iso_throw_pi("type_error", "callable", "?", 0); return FAILDESCR; }
        const char *name = (const char *)0; long arity = 0; dyn_term_key((pl_cell_t *)&args[0], &name, &arity);
        if (!name) return FAILDESCR;
        dyn_pred_row_t *row = dyn_pred_find(name, arity);
        if (!row) { if (rt_pl_proc_defined_static(name, arity)) rt_pl_iso_throw_permission("access", "private_procedure", name, (int)arity); return FAILDESCR; }
        pl_clause_it_t *it = (pl_clause_it_t *)rt_ws_alloc(sizeof *it);
        it->cur = row->head; it->mark = pl_trail_mark(&g_pl_trail);
        *resume = (int64_t)(intptr_t)it;
    }
    pl_clause_it_t *it = (pl_clause_it_t *)(intptr_t)*resume;
    while (it->cur) {
        pl_trail_unwind(&g_pl_trail, it->mark);
        dyn_clause_t *c = it->cur; it->cur = c->next;
        Term *bt = c->body ? c->body : term_new_atom(ATOM_TRUE);
        Term *vk[256]; pl_cell_t *vv[256]; int vn = 0;
        pl_term_to_cell_word_m(c->head, vk, vv, &vn, 256); pl_term_to_cell_word_m(bt, vk, vv, &vn, 256);
        pl_cell_t hcell = pl_term_to_cell_word_m(c->head, vk, vv, &vn, 256);
        pl_cell_t bcell = pl_term_to_cell_word_m(bt, vk, vv, &vn, 256);
        if (pl_unify((pl_cell_t *)&args[0], &hcell, &g_pl_trail) && pl_unify((pl_cell_t *)&args[1], &bcell, &g_pl_trail)) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; return r; }
    }
    pl_trail_unwind(&g_pl_trail, it->mark);
    return FAILDESCR;
}
typedef struct { const char *name; long arity; } pl_pi_cand_t;
typedef struct { pl_pi_cand_t *v; int n; int i; int mark; } pl_curpred_it_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static pl_cell_t pl_atom_cell(const char *a) { const char *n = prolog_atom_name(prolog_atom_intern(a)); if (!n) n = ""; pl_cell_t c; c.v = DT_S; c.slen = (uint32_t)strlen(n); c.s = n; return c; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_current_predicate_gen(DESCR_t *args, int nargs, int64_t *resume) {
    extern pl_trail_t g_pl_trail;
    if (nargs < 1 || !resume) return FAILDESCR;
    if (*resume == 0) {
        pl_curpred_it_t *it = (pl_curpred_it_t *)rt_ws_alloc(sizeof *it);
        int cap = (int)g_pl_dyn_pred_n + 8; it->v = (pl_pi_cand_t *)rt_ws_alloc((size_t)cap * sizeof(pl_pi_cand_t)); it->n = 0;
        for (long r = 0; r < g_pl_dyn_pred_n; r++) if (g_pl_dyn_pred_table[r].name) { it->v[it->n].name = g_pl_dyn_pred_table[r].name; it->v[it->n].arity = g_pl_dyn_pred_table[r].arity; it->n++; }
        it->i = 0; it->mark = pl_trail_mark(&g_pl_trail);
        *resume = (int64_t)(intptr_t)it;
    }
    pl_curpred_it_t *it = (pl_curpred_it_t *)(intptr_t)*resume;
    while (it->i < it->n) {
        pl_trail_unwind(&g_pl_trail, it->mark);
        pl_pi_cand_t cand = it->v[it->i++];
        pl_cell_t *kids = (pl_cell_t *)rt_ws_alloc(2 * sizeof(pl_cell_t)); kids[0] = pl_atom_cell(cand.name); kids[1] = pl_make_int(cand.arity);
        pl_cell_t pi = pl_make_compound(prolog_atom_intern("/"), 2, kids);
        if (pl_unify((pl_cell_t *)&args[0], &pi, &g_pl_trail)) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; return r; }
    }
    pl_trail_unwind(&g_pl_trail, it->mark);
    return FAILDESCR;
}
typedef struct { const char *props[4]; int n; int i; int mark; } pl_predprop_it_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_predicate_property_gen(DESCR_t *args, int nargs, int64_t *resume) {
    extern pl_trail_t g_pl_trail;
    extern void rt_pl_iso_throw_instantiation(void);
    extern void rt_pl_iso_throw_pi(const char *, const char *, const char *, int);
    extern int rt_pl_proc_defined_static(const char *, long);
    if (nargs < 2 || !resume) return FAILDESCR;
    if (*resume == 0) {
        pl_cell_t *hdc = pl_deref((pl_cell_t *)&args[0]);
        if (pl_cell_unbound(hdc)) { rt_pl_iso_throw_instantiation(); return FAILDESCR; }
        if ((int)hdc->v != DT_A && hdc->v != DT_S && (int)hdc->v != DT_PLREF) { rt_pl_iso_throw_pi("type_error", "callable", "?", 0); return FAILDESCR; }
        const char *name = (const char *)0; long arity = 0; dyn_term_key(hdc, &name, &arity);
        if (!name) return FAILDESCR;
        int is_dyn = dyn_pred_find(name, arity) != (dyn_pred_row_t *)0;
        int is_stat = rt_pl_proc_defined_static(name, arity);
        if (!is_dyn && !is_stat) return FAILDESCR;
        pl_predprop_it_t *it = (pl_predprop_it_t *)rt_ws_alloc(sizeof *it); it->n = 0;
        if (is_dyn) it->props[it->n++] = "dynamic"; else it->props[it->n++] = "static";
        it->props[it->n++] = "defined";
        it->i = 0; it->mark = pl_trail_mark(&g_pl_trail);
        *resume = (int64_t)(intptr_t)it;
    }
    pl_predprop_it_t *it = (pl_predprop_it_t *)(intptr_t)*resume;
    while (it->i < it->n) {
        pl_trail_unwind(&g_pl_trail, it->mark);
        const char *p = it->props[it->i++];
        pl_cell_t pt = pl_atom_cell(p);
        if (pl_unify((pl_cell_t *)&args[1], &pt, &g_pl_trail)) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; return r; }
    }
    pl_trail_unwind(&g_pl_trail, it->mark);
    return FAILDESCR;
}
typedef struct { const char *name; int prec; const char *type; } pl_op_cand_t;
typedef struct { pl_op_cand_t *v; int n; int i; int mark; } pl_curop_it_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_current_op_gen(DESCR_t *args, int nargs, int64_t *resume) {
    extern pl_trail_t g_pl_trail;
    extern int prolog_op_table_count(void);
    extern int prolog_op_table_get(int, const char **, int *, const char **);
    extern void rt_pl_iso_throw_pi(const char *, const char *, const char *, int);
    if (nargs < 3 || !resume) return FAILDESCR;
    if (*resume == 0) {
        pl_cell_t *nmc = pl_deref((pl_cell_t *)&args[2]);
        if (!pl_cell_unbound(nmc) && (int)nmc->v != DT_A && nmc->v != DT_S) { rt_pl_iso_throw_pi("type_error", "atom", "?", 0); return FAILDESCR; }
        int cap = prolog_op_table_count(); if (cap < 0) cap = 0;
        pl_curop_it_t *it = (pl_curop_it_t *)rt_ws_alloc(sizeof *it);
        it->v = (pl_op_cand_t *)rt_ws_alloc((size_t)(cap + 1) * sizeof(pl_op_cand_t)); it->n = 0;
        for (int r = 0; r < cap; r++) { const char *onm = 0; int op = 0; const char *ot = 0; if (prolog_op_table_get(r, &onm, &op, &ot) && onm && ot) { it->v[it->n].name = onm; it->v[it->n].prec = op; it->v[it->n].type = ot; it->n++; } }
        it->i = 0; it->mark = pl_trail_mark(&g_pl_trail);
        *resume = (int64_t)(intptr_t)it;
    }
    pl_curop_it_t *it = (pl_curop_it_t *)(intptr_t)*resume;
    while (it->i < it->n) {
        pl_trail_unwind(&g_pl_trail, it->mark);
        pl_op_cand_t cand = it->v[it->i++];
        pl_cell_t precT = pl_make_int(cand.prec);
        pl_cell_t typeT = pl_atom_cell(cand.type);
        pl_cell_t nameT = pl_atom_cell(cand.name);
        if (pl_unify((pl_cell_t *)&args[0], &precT, &g_pl_trail) && pl_unify((pl_cell_t *)&args[1], &typeT, &g_pl_trail) && pl_unify((pl_cell_t *)&args[2], &nameT, &g_pl_trail)) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; return r; }
    }
    pl_trail_unwind(&g_pl_trail, it->mark);
    return FAILDESCR;
}
typedef struct { const char *name; int rw; char val[32]; int is_int; long long ival; } pl_flag_ent_t;
static pl_flag_ent_t g_pl_flags[] = { { "bounded", 0, "true", 0, 0 }, { "max_integer", 0, "", 1, 9223372036854775807LL }, { "min_integer", 0, "", 1, (-9223372036854775807LL - 1) }, { "double_quotes", 1, "atom", 0, 0 }, { "unknown", 1, "error", 0, 0 }, { "occurs_check", 1, "false", 0, 0 }, { "dialect", 1, "gnu", 0, 0 } };
static const int g_pl_flags_n = (int)(sizeof g_pl_flags / sizeof g_pl_flags[0]);
typedef struct { int i; int mark; } pl_flagit_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static pl_flag_ent_t *pl_flag_find(const char *nm) { if (!nm) return (pl_flag_ent_t *)0; for (int i = 0; i < g_pl_flags_n; i++) if (!strcmp(g_pl_flags[i].name, nm)) return &g_pl_flags[i]; return (pl_flag_ent_t *)0; }
int rt_pl_dialect_is_swi(void) { pl_flag_ent_t *e = pl_flag_find("dialect"); return e && !strcmp(e->val, "swi"); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_set_prolog_flag(DESCR_t fd, DESCR_t vd) {
    extern void rt_pl_iso_throw_instantiation(void);
    extern void rt_pl_iso_throw_pi(const char *, const char *, const char *, int);
    pl_cell_t *ftc = pl_deref((pl_cell_t *)&fd);
    if (pl_cell_unbound(ftc)) { rt_pl_iso_throw_instantiation(); return 0; }
    if ((int)ftc->v != DT_A && ftc->v != DT_S) { rt_pl_iso_throw_pi("type_error", "atom", "?", 0); return 0; }
    const char *fn = ((int)ftc->v == DT_A) ? prolog_atom_name((int)ftc->i) : (ftc->s ? ftc->s : ""); if (!fn) { rt_pl_iso_throw_pi("type_error", "atom", "?", 0); return 0; }
    pl_flag_ent_t *e = pl_flag_find(fn); if (!e) { rt_pl_iso_throw_pi("domain_error", "prolog_flag", fn, 0); return 0; }
    if (!e->rw) { extern void rt_pl_iso_throw_permission(const char *, const char *, const char *, int); rt_pl_iso_throw_permission("modify", "flag", fn, 0); return 0; }
    pl_cell_t *vtc = pl_deref((pl_cell_t *)&vd);
    if (pl_cell_unbound(vtc)) { rt_pl_iso_throw_instantiation(); return 0; }
    if ((int)vtc->v != DT_A && vtc->v != DT_S) { rt_pl_iso_throw_pi("type_error", "atom", "?", 0); return 0; }
    const char *vs = ((int)vtc->v == DT_A) ? prolog_atom_name((int)vtc->i) : (vtc->s ? vtc->s : ""); if (!vs) { rt_pl_iso_throw_pi("type_error", "atom", "?", 0); return 0; }
    snprintf(e->val, sizeof e->val, "%s", vs); e->is_int = 0; return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_current_prolog_flag_gen(DESCR_t *args, int nargs, int64_t *resume) {
    extern pl_trail_t g_pl_trail;
    extern void rt_pl_iso_throw_pi(const char *, const char *, const char *, int);
    if (nargs < 2 || !resume) return FAILDESCR;
    if (*resume == 0) {
        pl_cell_t *nmc = pl_deref((pl_cell_t *)&args[0]);
        if (!pl_cell_unbound(nmc) && (int)nmc->v != DT_A && nmc->v != DT_S) { rt_pl_iso_throw_pi("type_error", "atom", "?", 0); return FAILDESCR; }
        pl_flagit_t *it = (pl_flagit_t *)rt_ws_alloc(sizeof *it); it->i = 0; it->mark = pl_trail_mark(&g_pl_trail); *resume = (int64_t)(intptr_t)it;
    }
    pl_flagit_t *it = (pl_flagit_t *)(intptr_t)*resume;
    while (it->i < g_pl_flags_n) {
        pl_trail_unwind(&g_pl_trail, it->mark);
        pl_flag_ent_t *e = &g_pl_flags[it->i++];
        pl_cell_t nameT = pl_atom_cell(e->name);
        pl_cell_t valT = e->is_int ? pl_make_int((int64_t)e->ival) : pl_atom_cell(e->val);
        if (pl_unify((pl_cell_t *)&args[0], &nameT, &g_pl_trail) && pl_unify((pl_cell_t *)&args[1], &valT, &g_pl_trail)) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; return r; }
    }
    pl_trail_unwind(&g_pl_trail, it->mark);
    return FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_current_stream_gen(DESCR_t *args, int nargs, int64_t *resume) {
    extern pl_trail_t g_pl_trail; extern FILE *fh_get(int); extern int prolog_atom_intern(const char *);
    if (nargs < 1 || !resume) return FAILDESCR;
    if (*resume == 0) { pl_flagit_t *it = (pl_flagit_t *)rt_ws_alloc(sizeof *it); it->i = 0; it->mark = pl_trail_mark(&g_pl_trail); *resume = (int64_t)(intptr_t)it; }
    pl_flagit_t *it = (pl_flagit_t *)(intptr_t)*resume;
    while (it->i < 64) {
        int idx = it->i++;
        if (!fh_get(idx)) continue;
        pl_trail_unwind(&g_pl_trail, it->mark);
        pl_cell_t *sa = (pl_cell_t *)rt_ws_alloc(sizeof(pl_cell_t)); sa[0] = pl_make_int(idx);
        pl_cell_t st = pl_make_compound(prolog_atom_intern("$stream"), 1, sa);
        if (pl_unify((pl_cell_t *)&args[0], &st, &g_pl_trail)) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; return r; }
    }
    pl_trail_unwind(&g_pl_trail, it->mark);
    return FAILDESCR;
}
typedef struct { int si; int pi; int mark; } pl_spropit_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_stream_property_gen(DESCR_t *args, int nargs, int64_t *resume) {
    extern pl_trail_t g_pl_trail; extern FILE *fh_get(int); extern int prolog_atom_intern(const char *); extern void *rt_ws_alloc(size_t); extern char *fh_name[]; extern char fh_mode[]; extern char fh_type[];
    if (nargs < 2 || !resume) return FAILDESCR;
    if (*resume == 0) { pl_spropit_t *it0 = (pl_spropit_t *)rt_ws_alloc(sizeof *it0); it0->si = 0; it0->pi = 0; it0->mark = pl_trail_mark(&g_pl_trail); *resume = (int64_t)(intptr_t)it0; }
    pl_spropit_t *it = (pl_spropit_t *)(intptr_t)*resume;
    while (it->si < 64) {
        int idx = it->si;
        if (!fh_get(idx)) { it->si++; it->pi = 0; continue; }
        char m = fh_mode[idx]; char ty = fh_type[idx];
        while (it->pi < 8) {
            int pi = it->pi++;
            pl_cell_t *prop = (pl_cell_t *)0; pl_cell_t *pa = (pl_cell_t *)rt_ws_alloc(sizeof(pl_cell_t)); pl_cell_t *pv = (pl_cell_t *)rt_ws_alloc(sizeof(pl_cell_t));
            if (pi == 0) { if (idx >= 3 && fh_name[idx]) { pa[0] = pl_atom_cell(fh_name[idx]); *pv = pl_make_compound(prolog_atom_intern("file_name"), 1, pa); prop = pv; } }
            else if (pi == 1) { pa[0] = pl_atom_cell(m == 'r' ? "read" : (m == 'a' ? "append" : "write")); *pv = pl_make_compound(prolog_atom_intern("mode"), 1, pa); prop = pv; }
            else if (pi == 2) { *pv = pl_atom_cell(m == 'r' ? "input" : "output"); prop = pv; }
            else if (pi == 3) { pa[0] = pl_atom_cell(ty == 'b' ? "binary" : "text"); *pv = pl_make_compound(prolog_atom_intern("type"), 1, pa); prop = pv; }
            else if (pi == 4) { if (idx >= 3) { pa[0] = pl_atom_cell("true"); *pv = pl_make_compound(prolog_atom_intern("reposition"), 1, pa); prop = pv; } }
            else if (pi == 5) { if (idx >= 3) { pa[0] = pl_atom_cell("eof_code"); *pv = pl_make_compound(prolog_atom_intern("eof_action"), 1, pa); prop = pv; } }
            else if (pi == 6) { if (idx >= 3) { pa[0] = pl_atom_cell("block"); *pv = pl_make_compound(prolog_atom_intern("buffering"), 1, pa); prop = pv; } }
            else if (pi == 7) { if (idx >= 3) { FILE *fp = fh_get(idx); const char *eos = "not"; if (fp && m == 'r') { if (feof(fp)) eos = "past"; else { int cpk = getc(fp); if (cpk == EOF) eos = "at"; else ungetc(cpk, fp); } } pa[0] = pl_atom_cell(eos); *pv = pl_make_compound(prolog_atom_intern("end_of_stream"), 1, pa); prop = pv; } }
            if (!prop) continue;
            pl_trail_unwind(&g_pl_trail, it->mark);
            pl_cell_t *sa = (pl_cell_t *)rt_ws_alloc(sizeof(pl_cell_t)); sa[0] = pl_make_int(idx); pl_cell_t st = pl_make_compound(prolog_atom_intern("$stream"), 1, sa);
            if (pl_unify((pl_cell_t *)&args[0], &st, &g_pl_trail) && pl_unify((pl_cell_t *)&args[1], prop, &g_pl_trail)) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; return r; }
        }
        it->si++; it->pi = 0;
    }
    pl_trail_unwind(&g_pl_trail, it->mark);
    return FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_dyn_iter_step(void *cursor, void **arg_cell0, long arity){
    extern pl_trail_t g_pl_trail;
    dyn_cursor_t *cur = (dyn_cursor_t *)cursor;
    if (!cur) return 0;
    while (cur->next) {
        dyn_clause_t *c = cur->next; cur->next = c->next;
        int mark = pl_trail_mark(&g_pl_trail);
        Term *var_map[256]; int var_cap = 256, var_n = 0;
        Term *hcopy = copy_term_deep(c->head, var_map, &var_cap, &var_n);
        int ok = 1;
        if (arity == 0) { ok = (hcopy && hcopy->tag == TERM_ATOM); }
        else if (!hcopy || hcopy->tag != TERM_COMPOUND || hcopy->compound.arity != (int)arity) { ok = 0; }
        else {
            for (long i = 0; i < arity && ok; i++) {
                pl_cell_t *ac = (pl_cell_t *)((char *)arg_cell0 + (size_t)16 * (size_t)i);
                if (!pl_unify_term_into_cell(ac, hcopy->compound.args[i], &g_pl_trail)) ok = 0;
            }
        }
        if (ok) return 1;
        pl_trail_unwind(&g_pl_trail, mark);
    }
    return 0;
}
static Term *g_pl_throw_ball = (Term *)0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_throw_set(void *ball_cell)
{
    Term *var_map[256]; int var_cap = 256, var_n = 0;
    Term *b = copy_term_deep(pl_cell_to_term((pl_cell_t *)ball_cell), var_map, &var_cap, &var_n);
    g_pl_throw_ball = b ? b : pl_cell_to_term((pl_cell_t *)ball_cell);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_throw_pending(void) { return g_pl_throw_ball != (Term *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_throw_match(void *catcher_cell)
{
    extern pl_trail_t g_pl_trail;
    if (!g_pl_throw_ball) return 0;
    int mark = pl_trail_mark(&g_pl_trail);
    if (!pl_unify_term_into_cell((pl_cell_t *)catcher_cell, g_pl_throw_ball, &g_pl_trail)) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
    g_pl_throw_ball = (Term *)0;
    return 1;
}
