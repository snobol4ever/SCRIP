#include "rt/rt.h"
#include "core.h"
#include "builtins/resolution.h"
#include "../parser/prolog/prolog_atom.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void output_val(DESCR_t v) {
    char *s = VARVAL_fn(v);
    printf("%s\n", s ? s : "");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void output_str(const char *s) {
    printf("%s\n", s ? s : "");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_write_int_nl(int64_t v)
{
    fprintf(stdout, "%lld\n", (long long)v);
}
static void rt_format_float(char *buf, size_t bufsz, double d);
void out_write_str(FILE *dest, const char *s);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_write_any_nl(DESCR_t d)
{
    extern const char *rk_obj_stringify(DESCR_t d, int use_gist);
    if (d.v == DT_I)       fprintf(stdout, "%lld\n", (long long)d.i);
    else if (d.v == DT_R)  { char b[64]; rt_format_float(b, sizeof b, d.r); fprintf(stdout, "%s\n", b); }
    else if (d.v == DT_FAIL) fputc('\n', stdout);
    else if (d.v == DT_DATA) { const char *s = rk_obj_stringify(d, 1); if (s) out_write_str(stdout, s); fputc('\n', stdout); }
    else {
        char *s = VARVAL_fn(d);
        if (s) out_write_str(stdout, s);
        fputc('\n', stdout);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_write_strz_nl(const char *s)
{
    if (s) fwrite(s, 1, strlen(s), stdout);
    fputc('\n', stdout);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_write_atom(const char *s)
{
    if (s) fputs(s, stdout);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_write_int(long v)
{
    printf("%ld", v);
}
