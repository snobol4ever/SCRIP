#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/resource.h>
#include "descr.h"
#include "core.h"
#define BB_DEFINE_NAMES 1
#include "ast.h"
#include "pin_va.h"
typedef struct {
    void *module_init;
    void *main_alpha;
    const char **gva_names;
    long n_gva;
    void **alpha_tab;
    const void *gc_maps;
    const void *gc_sites;
    int64_t *proc_tab;
    const char **proc_names;
    long n_proc;
    const char *name;
} sc_module_desc_t;
typedef struct { const sc_module_desc_t *d; DESCR_t *base; int inited; int busy; int i_src; int i_path; int i_lang; int i_linked; int i_tree; int i_inited; int i_errpos; int i_incl; } sc_module_t;
static sc_module_t g_sc_mod;
extern const sc_module_desc_t scparsers_sc_module __attribute__((weak));
#include "../rtx/rtcc.h"
extern int64_t kw_anchor, kw_trim, kw_fullscan, kw_maxlngth, kw_stlimit, kw_stcount, kw_trace, kw_ftrace;
extern void core_lib_init(void);
extern DESCR_t *gva_register(const char **names, DESCR_t *cells, int n);
extern DESCR_t *rt_gva_island(int n);
extern void rt_ab_cell_bind_table(void **tab);
extern void rt_gc_frame_maps_install_counted(const void *tab);
extern void rt_gc_frame_sites_install_counted(const void *tab);
extern void rt_proc_table_fill(int64_t *tab, const char **names, int n);
extern char *rt_gc_program_top_get(void);
extern void rt_gc_program_top_set(char *top);
extern void rt_gc_inhibit_push(void);
extern void rt_gc_inhibit_pop(void);
extern void rt_sc_outer_call(void *fn);
extern DESCR_t array_get(ARBLK_t *a, int i);
extern char *rt_heap_strdup_c(const char *s);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *g_sc_inc_dirs = (char *)0;
void rt_sc_include_dir_add(const char *d) {
    size_t o = g_sc_inc_dirs ? strlen(g_sc_inc_dirs) : 0, n = d ? strlen(d) : 0;
    char *j;
    if (!n) return;
    j = (char *)malloc(o + n + 2);
    if (o) { memcpy(j, g_sc_inc_dirs, o); j[o++] = ':'; }
    memcpy(j + o, d, n);
    j[o + n] = 0;
    free(g_sc_inc_dirs);
    g_sc_inc_dirs = j;
}
const char *rt_sc_include_dirs_joined(void) { return g_sc_inc_dirs ? g_sc_inc_dirs : ""; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t _MATCHAT_(DESCR_t *args, int nargs) { (void)args; (void)nargs; return (DESCR_t){ .v = DT_I, .slen = 0, .i = (int64_t)rtccb[RTCC_SLOT_MATCHAT] }; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long rt_sc_gva_reserve(void) { return &scparsers_sc_module ? scparsers_sc_module.n_gva : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_sc_module_linked(void) { return &scparsers_sc_module ? 1 : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_sc_modules_invalidate(void) { g_sc_mod.inited = 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sc_slot_of(const sc_module_desc_t *d, const char *nm) { for (long k = 0; k < d->n_gva; k++) if (d->gva_names[k] && !strcmp(d->gva_names[k], nm)) return (int)k; return -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static sc_module_t *sc_module_get(void) {
    sc_module_t *m = &g_sc_mod;
    if (m->d) return m;
    if (!&scparsers_sc_module) return (sc_module_t *)0;
    m->d = &scparsers_sc_module;
    m->base = (DESCR_t *)RT_GVA_VA;
    m->i_src = sc_slot_of(m->d, "ModSrc");
    m->i_path = sc_slot_of(m->d, "ModPath");
    m->i_lang = sc_slot_of(m->d, "ModLang");
    m->i_linked = sc_slot_of(m->d, "ModLinked");
    m->i_tree = sc_slot_of(m->d, "ModTree");
    m->i_inited = sc_slot_of(m->d, "ModInited");
    m->i_errpos = sc_slot_of(m->d, "ModErrPos");
    m->i_incl = sc_slot_of(m->d, "ModIncl");
    if (m->i_src < 0 || m->i_path < 0 || m->i_lang < 0 || m->i_linked < 0 || m->i_tree < 0 || m->i_inited < 0) {
        fprintf(stderr, "rt_sc_module: the linked parser module lacks the mod_head/mod_tail cells\n");
        abort();
    }
    return m;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sc_module_run(sc_module_t *m) {
    int64_t s_anchor = kw_anchor, s_trim = kw_trim, s_full = kw_fullscan, s_max = kw_maxlngth, s_stl = kw_stlimit, s_stc = kw_stcount, s_tr = kw_trace, s_ftr = kw_ftrace;
    char *save_top = rt_gc_program_top_get();
    int nested = save_top != (char *)0;
    kw_anchor = 0;
    kw_trim = 0;
    kw_fullscan = 1;
    kw_maxlngth = 16777216;
    kw_stlimit = -1;
    kw_trace = 0;
    kw_ftrace = 0;
    if (nested) rt_gc_inhibit_push();
    else rt_gc_program_top_set((char *)0);
    rt_sc_outer_call(m->d->main_alpha);
    if (nested) rt_gc_inhibit_pop();
    else rt_gc_program_top_set(save_top);
    kw_anchor = s_anchor;
    kw_trim = s_trim;
    kw_fullscan = s_full;
    kw_maxlngth = s_max;
    kw_stlimit = s_stl;
    kw_stcount = s_stc;
    kw_trace = s_tr;
    kw_ftrace = s_ftr;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sc_module_init(sc_module_t *m) {
    const sc_module_desc_t *d = m->d;
    {
        extern int core_stack_floor_raised;
        struct rlimit rl;
        if (getrlimit(RLIMIT_STACK, &rl) == 0 && rl.rlim_cur != RLIM_INFINITY && rl.rlim_cur < (rlim_t)(64L << 20)) {
            rl.rlim_cur = (rlim_t)(64L << 20);
            if (rl.rlim_max != RLIM_INFINITY && rl.rlim_cur > rl.rlim_max) rl.rlim_cur = rl.rlim_max;
            (void)setrlimit(RLIMIT_STACK, &rl);
        }
        core_stack_floor_raised = 1;
    }
    core_lib_init();
    { extern int rt_sxt_gva_count_get(void); if (rt_sxt_gva_count_get() < (int)d->n_gva) rt_gva_island((int)d->n_gva); }
    if (d->module_init) ((void (*)(void))d->module_init)();
    if (d->proc_tab && d->proc_names && d->n_proc > 0) rt_proc_table_fill(d->proc_tab, d->proc_names, (int)d->n_proc);
    gva_register(d->gva_names, m->base, (int)d->n_gva);
    if (d->alpha_tab) rt_ab_cell_bind_table(d->alpha_tab);
    if (!m->inited && d->gc_maps) rt_gc_frame_maps_install_counted(d->gc_maps);
    if (!m->inited && d->gc_sites) rt_gc_frame_sites_install_counted(d->gc_sites);
    m->base[m->i_linked] = (DESCR_t){ .v = DT_I, .slen = 0, .i = 1 };
    m->base[m->i_inited] = (DESCR_t){ .v = DT_SNUL, .slen = 0, .s = "" };
    sc_module_run(m);
    m->inited = 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sc_field_index(const DATINST_t *u, const char *nm) {
    const DATBLK_t *t = u ? u->type : (DATBLK_t *)0;
    for (int i = 0; t && i < t->nfields; i++) if (t->fields[i] && !strcmp(t->fields[i], nm)) return i;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sc_kind_of(const char *nm) {
    static int built = 0;
    static int bucket[1024];
    if (!built) {
        for (int i = 0; i < 1024; i++) bucket[i] = -1;
        for (int k = 0; k < TT_KIND_COUNT; k++) {
            if (!tt_e_name[k]) continue;
            { unsigned h = 2166136261u; for (const char *p = tt_e_name[k]; *p; p++) { h ^= (unsigned char)*p; h *= 16777619u; } h &= 1023u; while (bucket[h] >= 0) h = (h + 1) & 1023u; bucket[h] = k; }
        }
        built = 1;
    }
    {
        unsigned h = 2166136261u;
        for (const char *p = nm; *p; p++) { h ^= (unsigned char)*p; h *= 16777619u; }
        h &= 1023u;
        while (bucket[h] >= 0) { if (!strcmp(tt_e_name[bucket[h]], nm)) return bucket[h]; h = (h + 1) & 1023u; }
    }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *sc_text_of(DESCR_t d, size_t *len_out) {
    if (d.v == DT_S) { *len_out = d.slen; return d.s ? d.s : ""; }
    if (d.v == DT_SNUL) { *len_out = 0; return ""; }
    if (d.v == DT_I) { static char b[32]; int n = snprintf(b, sizeof b, "%lld", (long long)d.i); *len_out = (size_t)n; return b; }
    if (d.v == DT_R) { static char b[64]; int n = snprintf(b, sizeof b, "%.17g", d.r); *len_out = (size_t)n; return b; }
    *len_out = 0;
    return "";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { const char *src; size_t len; const int *line_starts; int nlines; int ft, fv, fn, fc, fp; long nodes; } sc_export_t;
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
        int k = root ? (int)TT_PROGRAM : sc_kind_of(tn);
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
                    if (root && IS_DATA_INST_fn(kd) && kd.u && kd.u->fields) { size_t kl; if (sc_kind_of(sc_text_of(kd.u->fields[x->ft], &kl)) < 0) continue; }
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
tree_t *rt_sc_module_parse(const char *lang, const char *src, size_t len, const char *path, const char *incl, long *nodes_out, long *errpos_out) {
    sc_module_t *m = sc_module_get();
    tree_t *root = (tree_t *)0;
    if (errpos_out) *errpos_out = -1;
    if (nodes_out) *nodes_out = 0;
    if (!m) return (tree_t *)0;
    if (m->busy) { fprintf(stderr, "rt_sc_module: the parser module is re-entered while it is parsing\n"); return (tree_t *)0; }
    m->busy = 1;
    if (!m->inited) sc_module_init(m);
    {
        char *hs = (char *)rt_heap_strdup_c(src ? src : "");
        m->base[m->i_src] = (DESCR_t){ .v = DT_S, .slen = (uint32_t)len, .s = hs };
        m->base[m->i_path] = path && *path ? STRVAL(rt_heap_strdup_c(path)) : NULVCL;
        m->base[m->i_lang] = STRVAL(rt_heap_strdup_c(lang));
        if (m->i_incl >= 0) m->base[m->i_incl] = incl && *incl ? STRVAL(rt_heap_strdup_c(incl)) : NULVCL;
        m->base[m->i_tree] = (DESCR_t){ .v = DT_SNUL, .slen = 0, .s = "" };
        if (m->i_errpos >= 0) m->base[m->i_errpos] = (DESCR_t){ .v = DT_I, .slen = 0, .i = -1 };
        sc_module_run(m);
    }
    {
        DESCR_t t = m->base[m->i_tree];
        if (IS_DATA_INST_fn(t) && t.u) {
            sc_export_t x;
            int *ls = (int *)malloc(sizeof(int) * (len + 2));
            long off0 = -1;
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
            if (x.ft < 0 || x.fv < 0 || x.fn < 0 || x.fc < 0) {
                fprintf(stderr, "rt_sc_module: the %s parser returned a record that is not a tree\n", lang);
                free(ls);
                m->busy = 0;
                return (tree_t *)0;
            }
            root = sc_export(&x, t, &off0, 1);
            if (nodes_out) *nodes_out = x.nodes;
            free(ls);
        } else if (m->i_errpos >= 0 && errpos_out) {
            DESCR_t ep = m->base[m->i_errpos];
            *errpos_out = ep.v == DT_I ? (long)ep.i : -1;
        }
    }
    m->base[m->i_src] = (DESCR_t){ .v = DT_SNUL, .slen = 0, .s = "" };
    m->base[m->i_tree] = (DESCR_t){ .v = DT_SNUL, .slen = 0, .s = "" };
    m->busy = 0;
    return root;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
__asm__(".globl rt_sc_outer_call\n.type rt_sc_outer_call, @function\n" "rt_sc_outer_call:\n" "  push %rbp\n  mov %rsp, %rbp\n  push %rbx\n  push %r12\n  push %r13\n  push %r14\n  push %r15\n"
    "  sub $8, %rsp\n  sub $1048576, %rsp\n  and $-16, %rsp\n  mov %rdi, %rax\n  push %rax\n  push %rax\n  lea 16(%rsp), %rdi\n  call rt_program_top_adopt@PLT\n  pop %rax\n  pop %rax\n"
    "  mov 0x70000000, %r12\n  movq rtccb@GOTPCREL(%rip), %r10\n  movq 48(%r10), %r9\n  leaq 1f(%rip), %rcx\n  push %rcx\n  push %rcx\n  xor %esi, %esi\n  xor %r13d, %r13d\n  xor %r14d, %r14d\n"
    "  xor %r15d, %r15d\n  jmp *%rax\n" "1:\n" "  leaq 1b(%rip), %rcx\n  mov (%rsp), %rax\n  cmp %rax, %rcx\n  jne 2f\n  add $8, %rsp\n" "2:\n"
    "  lea -40(%rbp), %rsp\n  pop %r15\n  pop %r14\n  pop %r13\n  pop %r12\n  pop %rbx\n  pop %rbp\n  ret\n" ".size rt_sc_outer_call, .-rt_sc_outer_call\n");
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
static void sc_lines_build(const char *src, size_t len, sc_lines_t *L) {
    int n = 1;
    for (size_t i = 0; i < len; i++) if (src[i] == '\n') n++;
    L->lines = (const char **)malloc(sizeof(const char *) * (size_t)n);
    L->lens = (int *)malloc(sizeof(int) * (size_t)n);
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
    sc_lines_build(src, len, &L);
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
            int next_line = i + 1 < prog->n && prog->c[i + 1]->line > line ? prog->c[i + 1]->line : line + 1;
            char *ssrc = is_null ? ct_strdup("") : sc_src_slice(&L, line, next_line, label);
            if (!ssrc) {
                char b[fmt_len("%s%s<stmt %d, line %d: source not resolvable>", label ? label : "", label ? "  " : "        ", ordinal, line)];
                snprintf(b, sizeof b, "%s%s<stmt %d, line %d: source not resolvable>", label ? label : "", label ? "  " : "        ", ordinal, line);
                ssrc = ct_strdup(b);
            }
            sc_stmt_insert(st, at++, sc_attr(":src", ssrc));
        } else sno_end_lineno = line;
        prev_line = line;
    }
    free((void *)L.lines);
    free(L.lens);
    return prog;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *sno_parse_stage_buf(const char *src, size_t len, const char *path) {
    if (rt_sc_module_linked() && sc_stage_wants_module()) {
        long nodes = 0, errpos = -1;
        tree_t *root = rt_sc_module_parse("snobol4", src, len, path, rt_sc_include_dirs_joined(), &nodes, &errpos);
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *sno_parse_stage_file(FILE *f, const char *path) {
    size_t cap = 65536, len = 0;
    char *buf = (char *)malloc(cap);
    tree_t *t;
    if (!f || !buf) return (tree_t *)0;
    for (;;) { size_t got = fread(buf + len, 1, cap - len, f); len += got; if (len < cap) break; cap *= 2; buf = (char *)realloc(buf, cap); if (!buf) return (tree_t *)0; }
    t = sno_parse_stage_buf(buf, len, path);
    free(buf);
    return t;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *sno_parse_stage_string(const char *src) {
    if (!(rt_sc_module_linked() && sc_stage_wants_module())) { extern tree_t *sno_parse_string_ast(const char *, void *); return sno_parse_string_ast(src, (void *)0); }
    {
        size_t n = src ? strlen(src) : 0;
        char *buf = (char *)ct_alloc(n + 6);
        tree_t *prog;
        if (n) memcpy(buf, src, n);
        memcpy(buf + n, "\nEND\n", 6);
        prog = sno_parse_stage_buf(buf, n + 5, "<code>");
        if (prog && prog->n > 0 && prog->c[prog->n - 1] && prog->c[prog->n - 1]->t == TT_END) prog->n--;
        return prog && prog->n > 0 ? prog : (tree_t *)0;
    }
}
