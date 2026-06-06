#include "rt/rt.h"
#include "core.h"
#include "builtins/resolution.h"
#include "../parser/prolog/prolog_atom.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*====================================================================================================================*/
void output_val(DESCR_t v) {
    char *s = VARVAL_fn(v);
    printf("%s\n", s ? s : "");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void output_str(const char *s) {
    printf("%s\n", s ? s : "");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_write_str_nl(const char *s, uint32_t slen)
{
    if (s && slen) fwrite(s, 1, slen, stdout);
    fputc('\n', stdout);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_write_int_nl(int64_t v)
{
    fprintf(stdout, "%lld\n", (long long)v);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void rt_format_float(char *buf, size_t bufsz, double d);
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_write_any_nl(DESCR_t d)
{
    if (d.v == DT_I)       fprintf(stdout, "%lld\n", (long long)d.i);
    else if (d.v == DT_R)  { char b[64]; rt_format_float(b, sizeof b, d.r); fprintf(stdout, "%s\n", b); }
    else if (d.v == DT_FAIL) fputc('\n', stdout);
    else {
        char *s = VARVAL_fn(d);
        if (s) fwrite(s, 1, strlen(s), stdout);
        fputc('\n', stdout);
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_write_strz_nl(const char *s)
{
    if (s) fwrite(s, 1, strlen(s), stdout);
    fputc('\n', stdout);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void rt_format_float(char *buf, size_t bufsz, double d)
{
    for (int prec = 15; prec <= 17; prec++) {
        snprintf(buf, bufsz, "%.*g", prec, d);
        if (strtod(buf, NULL) == d) break;
    }
    if (!strpbrk(buf, ".eEnN")) {
        size_t n = strlen(buf);
        if (n + 2 < bufsz) { buf[n] = '.'; buf[n+1] = '0'; buf[n+2] = '\0'; }
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_write_atom(const char *s)
{
    if (s) fputs(s, stdout);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_write_int(long v)
{
    printf("%ld", v);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_write_float(double v)
{
    char fb[64]; rt_format_float(fb, sizeof fb, v); fputs(fb, stdout);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_write_cstr(const char *s)
{
    if (s) fputs(s, stdout);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_write_var(int slot)
{
    extern Term **g_resolve_env;
    if (!g_resolve_env || slot < 0) { fputs("_", stdout); return; }
    Term *t = g_resolve_env[slot] ? term_deref(g_resolve_env[slot]) : NULL;
    if (!t) { fputs("_", stdout); return; }
    if (t->tag == TERM_INT)   { printf("%ld", (long)t->ival); return; }
    if (t->tag == TERM_FLOAT) { char fb[64]; rt_format_float(fb, sizeof fb, t->fval); fputs(fb, stdout); return; }
    if (t->tag == TERM_ATOM)  { const char *nm = prolog_atom_name(t->atom_id);
                                 if (nm) fputs(nm, stdout); return; }
    if (t->tag == TERM_COMPOUND) { extern void pl_write(Term *); pl_write(t); return; }
    fputs("_", stdout);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_write_term_ptr(void *t)
{
    extern void pl_write(Term *);
    Term *d = t ? term_deref((Term *)t) : NULL;
    if (!d) { fputs("_", stdout); return; }
    pl_write(d);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_writeq_term_ptr(void *t)
{
    extern void pl_writeq(Term *);
    Term *d = t ? term_deref((Term *)t) : NULL;
    if (!d) { fputs("_", stdout); return; }
    pl_writeq(d);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_write_canonical_term_ptr(void *t)
{
    extern void pl_write_canonical(Term *);
    Term *d = t ? term_deref((Term *)t) : NULL;
    if (!d) { fputs("_", stdout); return; }
    pl_write_canonical(d);
}
