#define BB_DEFINE_NAMES
#include "scrip_cc.h"
#include <stdint.h>
#define AST_PRINT_MAX_DEPTH 64
static int ast_print_width = 140;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void print_escaped(const char * s, FILE * f) {
    if (!s) { fputs("(null)", f); return; }
    fputc('"', f);
    for (const char * p = s; *p; p++) {
        switch (*p) {
        case '"':  fputs("\\\"", f); break;
        case '\\': fputs("\\\\", f); break;
        case '\n': fputs("\\n",  f); break;
        case '\r': fputs("\\r",  f); break;
        case '\t': fputs("\\t",  f); break;
        default:
            if ((unsigned char)*p < 0x20) fprintf(f, "\\x%02x", (unsigned char)*p);
            else fputc(*p, f);
        }
    }
    fputc('"', f);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void print_indent(int depth, FILE * f) {
    for (int i = 0; i < depth * 2; i++) fputc(' ', f);
}
#define FLAT_TOO_LONG 99999
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int flat_length(const tree_t *e, int budget) {
    if (!e || budget <= 0) return FLAT_TOO_LONG;
    const char *kname = (e->t >= 0 && e->t < TT_KIND_COUNT) ? tt_e_name[e->t] : "E_???";
    int klen = (int)strlen(kname);
    int total;
    switch (e->t) {
    case TT_QLIT: case TT_CSET: {
        const char *s = e->v.sval ? e->v.sval : "";
        int elen = 2;
        for (const char *p = s; *p; p++) {
            switch (*p) {
            case '"': case '\\': case '\n': case '\r': case '\t': elen += 2; break;
            default: elen += ((unsigned char)*p < 0x20) ? 4 : 1; break;
            }
        }
        return 1 + klen + 1 + elen + 1;
    }
    case TT_ILIT: { char buf[32]; return 1 + klen + 1 + snprintf(buf,sizeof buf,"%lld",(long long)e->v.ival) + 1; }
    case TT_FLIT: { char buf[32]; return 1 + klen + 1 + snprintf(buf,sizeof buf,"%g",e->v.dval) + 1; }
    case TT_NUL:  return 1 + klen + 1;
    default: break;
    }
    total = 1 + klen;
    if (e->v.sval && (uintptr_t)e->v.sval >= 4096 && e->t != TT_QLIT && e->t != TT_CSET && e->t != TT_CLAUSE && e->t != TT_SUB_DECL && e->t != TT_PROC_DECL && e->t != TT_REGEX_DECL && e->t != TT_AUGOP)
        total += 1 + (int)strlen(e->v.sval);
    if (e->n == 0) return total + 1;
    for (int i = 0; i < e->n; i++) {
        total += 1 + flat_length(e->c[i], budget - total);
        if (total > budget) return FLAT_TOO_LONG;
    }
    return total + 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void print_node(const tree_t * e, FILE * f, int depth) {
    const char * kname;
    int i;
    if (!e)                          { fputs("(null)", f); return; }
    if (depth > AST_PRINT_MAX_DEPTH) { fputs("(...)",  f); return; }
    kname = (e->t >= 0 && e->t < TT_KIND_COUNT) ? tt_e_name[e->t] : "E_???";
    int budget    = ast_print_width - depth * 2;
    int inline_ok = (flat_length(e, budget) <= budget);
    if (inline_ok) {
        switch (e->t) {
        case TT_QLIT: case TT_CSET:
            fputc('(', f); fputs(kname, f); fputc(' ', f); print_escaped(e->v.sval, f); fputc(')', f);
            return;
        case TT_ILIT: fprintf(f, "(%s %lld)", kname, (long long)e->v.ival); return;
        case TT_FLIT: fprintf(f, "(%s %g)",   kname, e->v.dval);             return;
        case TT_NUL:  fprintf(f, "(%s)",      kname);                         return;
        default: break;
        }
        fputc('(', f); fputs(kname, f);
        if (e->v.sval && (uintptr_t)e->v.sval >= 4096 && e->t != TT_QLIT && e->t != TT_CSET && e->t != TT_CLAUSE && e->t != TT_SUB_DECL && e->t != TT_PROC_DECL && e->t != TT_REGEX_DECL && e->t != TT_AUGOP) { fputc(' ', f); fputs(e->v.sval, f); }
        else if (e->t == TT_VAR && (uintptr_t)e->v.sval < 4096) fprintf(f, " #%d", (int)e->v.ival);
        for (i = 0; i < e->n; i++) { fputc(' ', f); print_node(e->c[i], f, depth + 1); }
        fputc(')', f);
        return;
    }
    fputc('(', f); fputs(kname, f);
    if (e->v.sval && (uintptr_t)e->v.sval >= 4096 && e->t != TT_QLIT && e->t != TT_CSET && e->t != TT_CLAUSE && e->t != TT_SUB_DECL && e->t != TT_PROC_DECL && e->t != TT_REGEX_DECL && e->t != TT_AUGOP) { fputc(' ', f); fputs(e->v.sval, f); }
    if (e->n == 0) { fputc(')', f); return; }
    for (i = 0; i < e->n; i++) {
        fputc('\n', f); print_indent(depth + 1, f); print_node(e->c[i], f, depth + 1);
    }
    fputc('\n', f); print_indent(depth, f); fputc(')', f);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void ir_print_node(const tree_t * e, FILE * f)    { print_node(e, f, 0); }
