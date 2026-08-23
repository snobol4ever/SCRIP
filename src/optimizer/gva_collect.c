#include "gva_collect.h"
#include <string.h>
#include <stdlib.h>
static const char **g_gva_names = NULL;
static int g_gva_n = 0;
static int g_gva_max = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern int is_protected_pat_name(const char *name);
/* ⛔ PROTECTED PATTERN NAMES ARE REFUSED ADMISSION TO THE GVA ISLAND (hq_C s261, lead from hq_P s262).  A GVA-eligible name is stored with a direct cell write (mov [r9+N], rax) that NEVER CALLS NV_SET_fn, so EVERY name-based guard living inside NV_SET_fn is bypassed for it -- core.c:2308's protected-variable refusal (ERROR 042) among them, which let `ARB = 1` land silently and the program run on.  Refusing here rather than at the three gva_collect_var call sites is deliberate: this predicate is the single admission funnel, and a per-call-site filter is the shape RULES.md forbids.  It sits beside the existing exclusion list for the same reason those names are there -- their stores must stay on the guarded path.  ⛔ NOT a compile-time refusal, and it must not become one: measured against x64/bin/sbl -bf, SPITBOL prints the program's earlier output and THEN raises 042 at RUN time (in statement 2, stmts executed 2), so the store must still be reached and refused during execution -- which is exactly what routing it back through NV_SET_fn does, at zero fast-path cost. */
int gva_name_eligible(const char *name) {
    if (!name || !name[0]) return 0;
    if (name[0] == '&') return 0;
    static const char *excl[] = { "INPUT","OUTPUT","PUNCH","TERMINAL","PUNCHAR","STLIMIT","STCOUNT","STNO","ANCHOR","TRIM","FULLSCAN","CASE","MAXLNGTH",
                                   "FTRACE","TRACE","ERRLIMIT","CODE","FNCLEVEL","RTNTYPE","ALPHABET","ABEND","DUMP","STEXEC","ERRTYPE","ERRTEXT","GTRACE",
                                   "FATALLIMIT","PARM","PI", (const char *)0 };
    for (int i = 0; excl[i]; i++) if (strcmp(name, excl[i]) == 0) return 0;
    if (is_protected_pat_name(name)) return 0;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void gva_collect_reset(void) { g_gva_n = 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int gva_index_of(const char *name) {
    if (!name) return -1;
    for (int i = 0; i < g_gva_n; i++) if (g_gva_names[i] && strcmp(g_gva_names[i], name) == 0) return i;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int gva_collect_var(const char *name) {
    if (!gva_name_eligible(name)) return -1;
    int k = gva_index_of(name); if (k >= 0) return k;
    if (g_gva_n >= g_gva_max) {
        int nm = g_gva_max ? g_gva_max * 2 : 256; const char **g = (const char **)realloc(g_gva_names, (size_t)nm * sizeof(const char *));
        if (!g) return -1; g_gva_names = g; g_gva_max = nm;
    }
    g_gva_names[g_gva_n] = name; return g_gva_n++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int gva_count(void) { return g_gva_n; }
const char *gva_name(int k) { return (k >= 0 && k < g_gva_n) ? g_gva_names[k] : (const char *)0; }
