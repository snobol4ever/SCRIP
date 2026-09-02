#include "gva_collect.h"
#include "IR.h"
#include <string.h>
#include <stdlib.h>
static const char **g_gva_names = NULL;
static int g_gva_n = 0;
static int g_gva_max = 0;
static const char **g_gva_io_refused = NULL;
static int g_gva_io_refused_n = 0;
static int g_gva_io_refused_max = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int gva_io_refused(const char *name) {
    for (int i = 0; i < g_gva_io_refused_n; i++) if (g_gva_io_refused[i] && strcmp(g_gva_io_refused[i], name) == 0) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void gva_io_refuse_name(const char *name) {
    if (!name || !name[0] || gva_io_refused(name)) return;
    if (g_gva_io_refused_n >= g_gva_io_refused_max) {
        int nm = g_gva_io_refused_max ? g_gva_io_refused_max * 2 : 32; const char **g = (const char **)realloc(g_gva_io_refused, (size_t)nm * sizeof(const char *));
        if (!g) return; g_gva_io_refused = g; g_gva_io_refused_max = nm;
    }
    g_gva_io_refused[g_gva_io_refused_n++] = name;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void gva_io_refuse_scan_graph(struct IR_graph_t *g) {
    if (!g) return;
    for (int i = 0; i < g->n; i++) {
        IR_t *nd = g->all[i]; if (!nd) continue;
        if (nd->op != IR_CALL) continue;
        { const char *fn = IR_LIT(nd).sval;
          if (!fn || (strcmp(fn, "INPUT") != 0 && strcmp(fn, "OUTPUT") != 0)) continue; }
        if (nd->n_operands < 1 || !nd->operands[0]) continue;
        { IR_t *arg = nd->operands[0];
          if ((arg->op == IR_LIT_NAME || arg->op == IR_LIT_STRING) && IR_LIT(arg).sval) gva_io_refuse_name(IR_LIT(arg).sval); }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern int is_protected_pat_name(const char *name);
int gva_name_eligible(const char *name) {
    if (!name || !name[0]) return 0;
    if (name[0] == '&') return 0;
    static const char *excl[] = { "INPUT","OUTPUT","PUNCH","TERMINAL","PUNCHAR","STLIMIT","STCOUNT","STNO","ANCHOR","TRIM","FULLSCAN","CASE","MAXLNGTH",
                                   "FTRACE","TRACE","ERRLIMIT","CODE","FNCLEVEL","RTNTYPE","ALPHABET","ABEND","DUMP","STEXEC","ERRTYPE","ERRTEXT","GTRACE",
                                   "FATALLIMIT","PARM","PI", (const char *)0 };
    for (int i = 0; excl[i]; i++) if (strcmp(name, excl[i]) == 0) return 0;
    if (is_protected_pat_name(name)) return 0;
    if (gva_io_refused(name)) return 0;
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
