#include "gva_collect.h"
#include "ct_arena.h"
#include "IR.h"
#include "g_lower.h"
#include <string.h>
#include <stdlib.h>
#include <strings.h>
static uint32_t gva_hash(const char * s) { uint32_t h = 2166136261u; for (const unsigned char * p = (const unsigned char *)s; *p; p++) { h ^= *p; h *= 16777619u; } return h; }
static void gva_hx_put(const char * name, int k) {
    uint32_t m = g_lower.opt.gva_hx_cap - 1, h = gva_hash(name) & m;
    while (g_lower.opt.gva_hx[h].k1) h = (h + 1) & m;
    g_lower.opt.gva_hx[h].name = name;
    g_lower.opt.gva_hx[h].k1 = k + 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int gva_io_refused(const char *name) {
    for (int i = 0; i < g_lower.opt.gva_io_refused_n; i++) if (g_lower.opt.gva_io_refused[i] && strcmp(g_lower.opt.gva_io_refused[i], name) == 0) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void gva_io_refuse_name(const char *name) {
    if (!name || !name[0] || gva_io_refused(name)) return;
    if (g_lower.opt.gva_io_refused_n >= g_lower.opt.gva_io_refused_max) {
        int nm = g_lower.opt.gva_io_refused_max ? g_lower.opt.gva_io_refused_max * 2 : 32;
        const char **g = (const char **)ct_grow(g_lower.opt.gva_io_refused, (size_t)nm * sizeof(const char *));
        if (!g) return;
        g_lower.opt.gva_io_refused = g;
        g_lower.opt.gva_io_refused_max = nm;
    }
    g_lower.opt.gva_io_refused[g_lower.opt.gva_io_refused_n++] = name;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void gva_io_refuse_scan_graph(struct IR_graph_t *g) {
    if (!g) return;
    for (int i = 0; i < g->n; i++) {
        IR_t *nd = g->all[i];
        if (!nd) continue;
        if (nd->op != IR_CALL) continue;
        { const char *fn = IR_LIT(nd).sval; if (!fn || (strcmp(fn, "INPUT") != 0 && strcmp(fn, "OUTPUT") != 0)) continue; }
        if (nd->n_operands < 1 || !nd->operands[0]) continue;
        { IR_t *arg = nd->operands[0]; if ((arg->op == IR_LIT_NAME || arg->op == IR_LIT_STRING) && IR_LIT(arg).sval) gva_io_refuse_name(IR_LIT(arg).sval); }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int gva_keyword_refused(const char *name) {
    for (int i = 0; i < g_lower.opt.gva_kw_refused_n; i++) if (g_lower.opt.gva_kw_refused[i] && strcmp(g_lower.opt.gva_kw_refused[i], name) == 0) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void gva_keyword_refuse_name(const char *name) {
    if (!name || !name[0] || gva_keyword_refused(name)) return;
    if (g_lower.opt.gva_kw_refused_n >= g_lower.opt.gva_kw_refused_max) {
        int nm = g_lower.opt.gva_kw_refused_max ? g_lower.opt.gva_kw_refused_max * 2 : 32;
        const char **g = (const char **)ct_grow(g_lower.opt.gva_kw_refused, (size_t)nm * sizeof(const char *));
        if (!g) return;
        g_lower.opt.gva_kw_refused = g;
        g_lower.opt.gva_kw_refused_max = nm;
    }
    g_lower.opt.gva_kw_refused[g_lower.opt.gva_kw_refused_n++] = name;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void gva_keyword_refuse_reset(void) { g_lower.opt.gva_kw_refused_n = 0; g_lower.opt.gva_kw_seeded = 1; g_lower.opt.gva_sn4_family = 0; g_lower.opt.gva_trace_demote = 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void gva_keyword_refuse_seed_snobol4(void) {
    g_lower.opt.gva_kw_seeded = 1;
    g_lower.opt.gva_sn4_family = 1;
    static const char * const kw[] = { "INPUT","OUTPUT","PUNCH","TERMINAL","PUNCHAR","STLIMIT","STCOUNT","STNO","ANCHOR","TRIM","FULLSCAN","CASE","MAXLNGTH", "FTRACE","TRACE","ERRLIMIT","CODE",
        "FNCLEVEL", "RTNTYPE","ALPHABET","ABEND","DUMP","STEXEC","ERRTYPE","ERRTEXT","GTRACE", "FATALLIMIT","PARM","PI", (const char *)0 };
    for (int i = 0; kw[i]; i++) gva_keyword_refuse_name(kw[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int gva_trace_demoted(void) { return g_lower.opt.gva_trace_demote; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int gva_trace_type_provably_non_access(IR_t *arg) {
    if (!arg) return 0;
    if (arg->op != IR_LIT_NAME && arg->op != IR_LIT_STRING) return 0;
    { const char *t = IR_LIT(arg).sval; if (!t) return 0; if (strcasecmp(t, "A") == 0 || strcasecmp(t, "ACCESS") == 0) return 0; return 1; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void gva_trace_demote_scan_graph(struct IR_graph_t *g) {
    if (!g || !g_lower.opt.gva_sn4_family || g_lower.opt.gva_trace_demote) return;
    for (int i = 0; i < g->n; i++) {
        IR_t *nd = g->all[i];
        if (!nd) continue;
        if (nd->op != IR_CALL) continue;
        { const char *fn = IR_LIT(nd).sval; if (!fn || strcmp(fn, "TRACE") != 0) continue; }
        if (nd->n_operands < 2) continue;
        if (gva_trace_type_provably_non_access(nd->operands[1])) continue;
        g_lower.opt.gva_trace_demote = 1;
        return;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern int is_protected_pat_name(const char *name);
int gva_name_eligible(const char *name) {
    if (!name || !name[0]) return 0;
    if (name[0] == '&') return 0;
    if (gva_keyword_refused(name)) return 0;
    if (is_protected_pat_name(name)) return 0;
    if (gva_io_refused(name)) return 0;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void gva_collect_reset(void) { g_lower.opt.gva_n = 0; if (g_lower.opt.gva_hx_cap) memset(g_lower.opt.gva_hx, 0, (size_t)g_lower.opt.gva_hx_cap * sizeof(gva_hx_t)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int gva_index_of(const char *name) {
    if (!name) return -1;
    if (!g_lower.opt.gva_n || !g_lower.opt.gva_hx_cap) return -1;
    {
        uint32_t m = g_lower.opt.gva_hx_cap - 1, h = gva_hash(name) & m;
        for (; g_lower.opt.gva_hx[h].k1; h = (h + 1) & m) if (strcmp(g_lower.opt.gva_hx[h].name, name) == 0) return g_lower.opt.gva_hx[h].k1 - 1;
    }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int gva_collect_var(const char *name) {
    if (!gva_name_eligible(name)) return -1;
    int k = gva_index_of(name);
    if (k >= 0) return k;
    if (g_lower.opt.gva_n >= g_lower.opt.gva_max) {
        int nm = g_lower.opt.gva_max ? g_lower.opt.gva_max * 2 : 256;
        const char **g = (const char **)ct_grow(g_lower.opt.gva_names, (size_t)nm * sizeof(const char *));
        if (!g) return -1;
        g_lower.opt.gva_names = g;
        g_lower.opt.gva_max = nm;
    }
    g_lower.opt.gva_names[g_lower.opt.gva_n] = name;
    g_lower.opt.gva_n++;
    if ((uint64_t)g_lower.opt.gva_n * 2 > (uint64_t)g_lower.opt.gva_hx_cap) {
        uint32_t c = g_lower.opt.gva_hx_cap ? g_lower.opt.gva_hx_cap * 2 : 1024;
        while ((uint64_t)c < (uint64_t)g_lower.opt.gva_n * 2 + 2) c *= 2;
        g_lower.opt.gva_hx = (gva_hx_t *)ct_zalloc(c, sizeof(gva_hx_t));
        g_lower.opt.gva_hx_cap = c;
        for (int i = 0; i < g_lower.opt.gva_n; i++) if (g_lower.opt.gva_names[i]) gva_hx_put(g_lower.opt.gva_names[i], i);
    } else gva_hx_put(name, g_lower.opt.gva_n - 1);
    return g_lower.opt.gva_n - 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int gva_collect_reserve(void) {
    if (g_lower.opt.gva_n >= g_lower.opt.gva_max) {
        int nm = g_lower.opt.gva_max ? g_lower.opt.gva_max * 2 : 256;
        const char **g = (const char **)ct_grow(g_lower.opt.gva_names, (size_t)nm * sizeof(const char *));
        if (!g) return -1;
        g_lower.opt.gva_names = g;
        g_lower.opt.gva_max = nm;
    }
    g_lower.opt.gva_names[g_lower.opt.gva_n] = (const char *)0;
    g_lower.opt.gva_n++;
    return g_lower.opt.gva_n - 1;
}
int gva_collect_var_copy(const char *name) {
    if (!gva_name_eligible(name)) return -1;
    { int k = gva_index_of(name); if (k >= 0) return k; }
    { const char *own = ct_strdup(name); return own ? gva_collect_var(own) : -1; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int gva_name_hidden(const char *name) { return name && (!strncmp(name, "SCV$", 4) || !strncmp(name, "SNO$VL$", 7) || !strncmp(name, "PATV$", 5) || !strncmp(name, "PAT$", 4)); }
int gva_count(void) { return g_lower.opt.gva_n; }
const char *gva_name(int k) { return (k >= 0 && k < g_lower.opt.gva_n) ? g_lower.opt.gva_names[k] : (const char *)0; }
const char *gva_note(int k) { extern const char *bb_ab_thunk_stem_or(const char *); const char *n = gva_name(k); return (!n || gva_name_hidden(n) || bb_ab_thunk_stem_or(n) != n) ? "" : n; }
