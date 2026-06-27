/*--------------------------------------------------------------------------------------------------------------------*/
#include "gva_collect.h"
#include <string.h>
#include <stdlib.h>
static const char **g_gva_names = NULL;
static int g_gva_n = 0;
static int g_gva_max = 0;
int gva_name_eligible(const char *name) {
    if (!name || !name[0]) return 0;
    if (name[0] == '&') return 0;
    static const char *excl[] = { "INPUT","OUTPUT","PUNCH","TERMINAL","PUNCHAR","STLIMIT","STCOUNT","STNO","ANCHOR","TRIM","FULLSCAN","CASE","MAXLNGTH","FTRACE","TRACE","ERRLIMIT","CODE","FNCLEVEL","RTNTYPE","ALPHABET","ABEND","DUMP","STEXEC","ERRTYPE","ERRTEXT","GTRACE","FATALLIMIT","PARM","PI", (const char *)0 };
    for (int i = 0; excl[i]; i++) if (strcmp(name, excl[i]) == 0) return 0;
    return 1;
}
void gva_collect_reset(void) { g_gva_n = 0; }
int gva_index_of(const char *name) {
    if (!name) return -1;
    for (int i = 0; i < g_gva_n; i++) if (g_gva_names[i] && strcmp(g_gva_names[i], name) == 0) return i;
    return -1;
}
int gva_collect_var(const char *name) {
    if (!gva_name_eligible(name)) return -1;
    int k = gva_index_of(name); if (k >= 0) return k;
    if (g_gva_n >= g_gva_max) { int nm = g_gva_max ? g_gva_max * 2 : 256; const char **g = (const char **)realloc(g_gva_names, (size_t)nm * sizeof(const char *)); if (!g) return -1; g_gva_names = g; g_gva_max = nm; }
    g_gva_names[g_gva_n] = name; return g_gva_n++;
}
int gva_count(void) { return g_gva_n; }
const char *gva_name(int k) { return (k >= 0 && k < g_gva_n) ? g_gva_names[k] : (const char *)0; }
