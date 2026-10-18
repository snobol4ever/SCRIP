#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "descr.h"
#include "core.h"
#define BB_DEFINE_NAMES 1
#include "ast.h"
#include "ct_arena.h"
extern DESCR_t array_get(ARBLK_t *a, int i);
extern int rt_sc_module_linked(const char *lang);
extern char *rt_gc_program_top_get(void);
extern DESCR_t rt_sc_module_parse_begin(const char *lang, const char *src, size_t len, const char *path, const char *incl, size_t incl_len, long *errpos_out);
extern void rt_sc_module_parse_end(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sc_field_index(const DATINST_t *u, const char *nm) {
    const DATBLK_t *t = u ? u->type : (DATBLK_t *)0;
    for (int i = 0; t && i < t->nfields; i++) if (t->fields[i] && !strcmp(t->fields[i], nm)) return i;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { const char *src; size_t len; const int *line_starts; int nlines; int ft, fv, fn, fc, fp; long nodes; const int *bucket; int nbucket; } sc_export_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static unsigned sc_kind_hash(const char *nm) { unsigned h = 2166136261u; for (const char *p = nm; *p; p++) { h ^= (unsigned char)*p; h *= 16777619u; } return h; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sc_kinds_build(int *bucket, int nb) {
    for (int i = 0; i < nb; i++) bucket[i] = -1;
    for (int k = 0; k < TT_KIND_COUNT; k++) {
        if (!tt_e_name[k][0]) continue;
        { unsigned h = sc_kind_hash(tt_e_name[k]) & (unsigned)(nb - 1); while (bucket[h] >= 0) h = (h + 1) & (unsigned)(nb - 1); bucket[h] = k; }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sc_kind_of(const sc_export_t *x, const char *nm) {
    unsigned h = sc_kind_hash(nm) & (unsigned)(x->nbucket - 1);
    while (x->bucket[h] >= 0) { if (!strcmp(tt_e_name[x->bucket[h]], nm)) return x->bucket[h]; h = (h + 1) & (unsigned)(x->nbucket - 1); }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *sc_text_of(DESCR_t d, size_t *len_out) { if (d.v == DT_S) { *len_out = d.slen; return d.s ? d.s : ""; } *len_out = 0; return ""; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sc_pos_of(const sc_export_t *x, long off, int *line, int *col) {
    int lo = 0, hi = x->nlines - 1;
    if (off < 0) off = 0;
    while (lo < hi) { int mid = (lo + hi + 1) >> 1; if ((long)x->line_starts[mid] <= off) lo = mid; else hi = mid - 1; }
    *line = lo + 1;
    *col = (int)(off - x->line_starts[lo]) + 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *sc_export(sc_export_t *x, DESCR_t d, long *first_off, int root) {
    tree_t *e;
    long my_off = -1, kid_off;
    if (!IS_DATA_INST_fn(d) || !d.u || !d.u->fields) return (tree_t *)0;
    {
        DESCR_t *f = d.u->fields;
        size_t tl, vl;
        const char *tn = sc_text_of(f[x->ft], &tl);
        int k = root ? (int)TT_PROGRAM : sc_kind_of(x, tn);
        if (k < 0) { fprintf(stderr, "rt_sc_module: the tree names an unknown kind '%s'\n", tn); return (tree_t *)0; }
        e = ast_node_new((tree_e)k);
        x->nodes++;
        if (root) {
        } else if (k == TT_ILIT) {
            DESCR_t v = f[x->fv];
            e->v.ival = v.v == DT_I ? v.i : (long long)strtoll(sc_text_of(v, &vl), (char **)0, 10);
        } else if (k == TT_FLIT) {
            DESCR_t v = f[x->fv];
            e->v.dval = v.v == DT_R ? v.r : v.v == DT_I ? (double)v.i : strtod(sc_text_of(v, &vl), (char **)0);
        } else if (k == TT_CHRLIT) {
            DESCR_t v = f[x->fv];
            e->v.ival = v.v == DT_I ? v.i : (long long)strtoll(sc_text_of(v, &vl), (char **)0, 10);
        } else {
            DESCR_t v = f[x->fv];
            const char *vt = sc_text_of(v, &vl);
            if (vl > 0 || k == TT_QLIT || k == TT_CSET) { char *s = (char *)ct_alloc(vl + 1); memcpy(s, vt, vl); s[vl] = 0; e->v.sval = s; e->slen = (int)vl; }
        }
        if (x->fp >= 0) { DESCR_t p = f[x->fp]; if (p.v == DT_I) my_off = (long)p.i; else if (p.v == DT_S && p.slen > 0) my_off = strtol(sc_text_of(p, &vl), (char **)0, 10); }
        {
            DESCR_t cd = f[x->fc];
            DESCR_t nd = f[x->fn];
            long n = nd.v == DT_I ? (long)nd.i : nd.v == DT_S ? strtol(sc_text_of(nd, &vl), (char **)0, 10) : 0;
            if (cd.v == DT_A && cd.arr && n > 0) {
                ARBLK_t *a = cd.arr;
                for (long i = 0; i < n && a->lo + i <= a->hi; i++) {
                    DESCR_t kd = array_get(a, (int)(a->lo + i));
                    tree_t *c;
                    if (root && IS_DATA_INST_fn(kd) && kd.u && kd.u->fields) { size_t kl; if (sc_kind_of(x, sc_text_of(kd.u->fields[x->ft], &kl)) < 0) continue; }
                    c = sc_export(x, kd, &kid_off, 0);
                    if (!c) return (tree_t *)0;
                    if (kid_off >= 0 && (my_off < 0 || kid_off < my_off)) my_off = kid_off;
                    ast_push(e, c);
                }
            }
        }
    }
    *first_off = my_off;
    if (my_off >= 0 && x->nlines > 0) { int ln, cl; sc_pos_of(x, my_off, &ln, &cl); e->line = ln; }
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *sc_export_tree(DESCR_t t, const char *src, size_t len) {
    sc_export_t x;
    long nl = 0;
    for (size_t i = 0; i < len; i++) if (src[i] == '\n') nl++;
    int ls[nl + 2];
    long off0 = -1;
    int nb = 1;
    while (nb < 2 * (int)TT_KIND_COUNT) nb <<= 1;
    int bucket[nb];
    sc_kinds_build(bucket, nb);
    x.bucket = bucket;
    x.nbucket = nb;
    x.src = src;
    x.len = len;
    x.nlines = 0;
    x.nodes = 0;
    ls[x.nlines++] = 0;
    for (size_t i = 0; i < len; i++) if (src[i] == '\n') ls[x.nlines++] = (int)(i + 1);
    x.line_starts = ls;
    x.ft = sc_field_index(t.u, "sc_t");
    x.fv = sc_field_index(t.u, "sc_v");
    x.fn = sc_field_index(t.u, "sc_n");
    x.fc = sc_field_index(t.u, "sc_c");
    x.fp = sc_field_index(t.u, "sc_p");
    if (x.ft < 0 || x.fv < 0 || x.fn < 0 || x.fc < 0) { fprintf(stderr, "rt_sc_module: the snobol4 parser returned a record that is not a tree\n"); return (tree_t *)0; }
    return sc_export(&x, t, &off0, 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern int sno_nerrors;
extern int sno_end_lineno;
extern tree_t *sno_parse_ast_buf(char *buf, size_t len, const char *fname, void *code_out, int expand_includes);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sc_stage_wants_module(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_PARSE_STAGE"); v = (e && !strcmp(e, "sc")) ? 1 : 0; } return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *sc_attr(const char *tag, const char *text) {
    tree_t *a = ast_node_new(TT_ATTR);
    a->v.sval = ct_strdup(tag);
    if (text) { tree_t *q = ast_node_new(TT_QLIT); q->v.sval = ct_strdup(text); q->slen = (int)strlen(text); ast_push(a, q); }
    return a;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *sc_attr_int(const char *tag, long v) { char b[32]; snprintf(b, sizeof b, "%ld", v); return sc_attr(tag, b); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t *sc_stmt_attr(const tree_t *st, const char *tag) {
    for (int i = 0; i < st->n; i++) if (st->c[i] && st->c[i]->t == TT_ATTR && st->c[i]->v.sval && !strcmp(st->c[i]->v.sval, tag)) return st->c[i];
    return (const tree_t *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sc_stmt_insert(tree_t *st, int at, tree_t *a) { ast_push(st, a); for (int i = st->n - 1; i > at; i--) st->c[i] = st->c[i - 1]; st->c[at] = a; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { const char **lines; int *lens; int n; } sc_lines_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sc_lines_build(const char *src, size_t len, sc_lines_t *L, const char **lines, int *lens) {
    L->lines = lines;
    L->lens = lens;
    L->n = 0;
    {
        size_t a = 0;
        for (size_t i = 0; i <= len; i++) {
            if (i == len || src[i] == '\n') {
                size_t e = i;
                while (e > a && (src[e - 1] == ' ' || src[e - 1] == '\t' || src[e - 1] == '\r')) e--;
                L->lines[L->n] = src + a;
                L->lens[L->n] = (int)(e - a);
                L->n++;
                a = i + 1;
            }
        }
    }
    if (len > 0 && src[len - 1] == '\n' && L->n > 1) L->n--;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *sc_src_slice(const sc_lines_t *L, int n1, int n2, const char *label) {
    if (n1 < 1 || n1 > L->n) return (char *)0;
    if (n2 <= n1) n2 = n1 + 1;
    if (n2 > L->n + 1) n2 = L->n + 1;
    while (n2 - 1 > n1) { const char *t = L->lines[n2 - 2]; int tl = L->lens[n2 - 2]; if (tl == 0 || t[0] == '*' || t[0] == '-') n2--; else break; }
    {
        const char *first = L->lines[n1 - 1];
        int fl = L->lens[n1 - 1];
        if (fl == 0 || first[0] == '*' || first[0] == '-') return (char *)0;
        if (label && label[0]) { size_t ll = strlen(label); if ((size_t)fl < ll || strncmp(first, label, ll) != 0) return (char *)0; } else if (first[0] != ' ' && first[0] != '\t') return (char *)0;
    }
    {
        size_t tot = 0;
        for (int i = n1; i < n2; i++) tot += (size_t)L->lens[i - 1] + 1;
        {
            char *out = (char *)ct_alloc(tot + 1);
            size_t o = 0;
            for (int i = n1; i < n2; i++) { memcpy(out + o, L->lines[i - 1], (size_t)L->lens[i - 1]); o += (size_t)L->lens[i - 1]; out[o++] = '\n'; }
            if (o) o--;
            out[o] = 0;
            return out;
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sc_line_is_blank_or_semi(const sc_lines_t *L, int ln) {
    if (ln < 1 || ln > L->n) return 1;
    { const char *t = L->lines[ln - 1]; int tl = L->lens[ln - 1]; for (int i = 0; i < tl; i++) { if (t[i] == ';') return 1; if (t[i] != ' ' && t[i] != '\t') return 0; } return 1; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *sc_sno_bookkeep(tree_t *root, const char *src, size_t len, const char *path) {
    sc_lines_t L;
    tree_t *prog = ast_node_new(TT_PROGRAM);
    int ordinal = 0, prev_line = 0;
    (void)path;
    long nl = 1;
    for (size_t i = 0; i < len; i++) if (src[i] == '\n') nl++;
    const char *lines[nl];
    int lens[nl];
    sc_lines_build(src, len, &L, lines, lens);
    for (int i = 0; i < root->n; i++) { tree_t *st = root->c[i]; if (!st || (st->t != TT_STMT && st->t != TT_END)) continue; ast_push(prog, st); }
    for (int i = 0; i < prog->n; i++) {
        tree_t *st = prog->c[i];
        const tree_t *lbl = sc_stmt_attr(st, ":lbl");
        const char *label = lbl && lbl->n > 0 && lbl->c[0] && lbl->c[0]->v.sval ? lbl->c[0]->v.sval : (const char *)0;
        int at = lbl ? 1 : 0;
        int line = st->line;
        int is_null = st->t == TT_STMT && !sc_stmt_attr(st, ":subj") && !sc_stmt_attr(st, ":pat") && !sc_stmt_attr(st, ":eq") && !label && st->n == 0;
        ordinal++;
        if (is_null) { if (!sc_line_is_blank_or_semi(&L, line)) line = prev_line > 0 ? prev_line : 1; st->line = line; }
        if (st->t == TT_END) { const tree_t *ent = sc_stmt_attr(st, ":entry"); at = (lbl ? 1 : 0) + (ent ? 1 : 0); }
        sc_stmt_insert(st, at++, sc_attr_int(":line", line));
        sc_stmt_insert(st, at++, sc_attr_int(":lline", line));
        sc_stmt_insert(st, at++, sc_attr_int(":stno", ordinal));
        if (st->t == TT_STMT) {
            char *ssrc = is_null ? ct_strdup("") : sc_src_slice(&L, line, line + 1, label);
            if (!ssrc) {
                char b[fmt_len("%s%s<stmt %d, line %d: source not resolvable>", label ? label : "", label ? "  " : "        ", ordinal, line)];
                snprintf(b, sizeof b, "%s%s<stmt %d, line %d: source not resolvable>", label ? label : "", label ? "  " : "        ", ordinal, line);
                ssrc = ct_strdup(b);
            }
            sc_stmt_insert(st, at++, sc_attr(":src", ssrc));
        } else sno_end_lineno = line;
        prev_line = line;
    }
    return prog;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *sno_parse_stage_buf(const char *src, size_t len, const char *path, const char *incl, size_t incl_len) {
    if (sc_stage_wants_module() && rt_sc_module_linked("snobol4") && !rt_gc_program_top_get()) {
        long errpos = -1;
        DESCR_t t = rt_sc_module_parse_begin("snobol4", src, len, path, incl, incl_len, &errpos);
        tree_t *root = IS_DATA_INST_fn(t) && t.u ? sc_export_tree(t, src, len) : (tree_t *)0;
        rt_sc_module_parse_end();
        if (!root) {
            sno_nerrors = 1;
            fprintf(stderr, "scrip: parse error in '%s'%s\n", path ? path : "<stdin>", errpos >= 0 ? " (the linked SNOBOL4 parser refused a statement)" : "");
            return (tree_t *)0;
        }
        sno_nerrors = 0;
        return sc_sno_bookkeep(root, src, len, path);
    }
    { char *copy = (char *)ct_alloc(len + 2); memcpy(copy, src, len); copy[len] = 0; copy[len + 1] = 0; return sno_parse_ast_buf(copy, len, path, (void *)0, 1); }
}
