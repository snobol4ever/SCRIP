#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/resource.h>
#include "descr.h"
#include "core.h"
#include "ast.h"
#include "pin_va.h"
#include "rt_sc_module.h"
enum { SC_GVA_RESERVE = 741 };
typedef struct {
    const sc_module_desc_t *d;
    DESCR_t *base;
    int inited;
    int busy;
    int no_reserve;
    char lang[16];
    int i_src;
    int i_path;
    int i_lang;
    int i_linked;
    int i_tree;
    int i_inited;
    int i_errpos;
    int i_incl;
} sc_module_t;
static sc_module_t g_sc_mod;
#include "../rtx/rtcc.h"
extern int64_t g_kw_anchor, g_kw_trim, g_kw_fullscan, g_kw_maxlngth, g_kw_stlimit, g_kw_stcount, g_kw_trace, g_kw_ftrace;
extern void core_lib_init(void);
extern DESCR_t *gva_register(const char **names, DESCR_t *cells, int n);
extern DESCR_t *rt_gva_island(int n);
extern void rt_ab_cell_bind_table(void **tab);
extern void rt_gc_frame_maps_install_counted(const void *tab);
extern void rt_gc_frame_sites_install_counted(const void *tab);
extern void rt_proc_table_fill(int64_t *tab, const char **names, int n);
extern char *rt_gc_program_top_get(void);
extern void rt_gc_program_top_set(char *top);
extern void rt_sc_outer_call(void *fn);
extern char *rt_heap_strdup_c(const char *s);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t _MATCHAT_(DESCR_t *args, int nargs) { (void)args; (void)nargs; return (DESCR_t){ .v = DT_I, .slen = 0, .i = (int64_t)g_rtccb[RTCC_SLOT_MATCHAT] }; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long rt_sc_gva_reserve(void) { return g_sc_mod.no_reserve ? 0 : SC_GVA_RESERVE; }
void rt_sc_module_compiling(void) { g_sc_mod.no_reserve = 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static sc_module_t *sc_module_get(const char *lang);
int rt_sc_module_linked(const char *lang) { return sc_module_get(lang) ? 1 : 0; }
int rt_sc_module_busy(void) { return g_sc_mod.busy; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_sc_modules_invalidate(void) { g_sc_mod.inited = 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sc_slot_of(const sc_module_desc_t *d, const char *nm) { for (long k = 0; k < d->n_gva; k++) if (d->gva_names[k] && !strcmp(d->gva_names[k], nm)) return (int)k; return -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static sc_module_t *sc_module_get(const char *lang) {
    sc_module_t *m = &g_sc_mod;
    if (!lang || !*lang || strlen(lang) >= sizeof m->lang) return (sc_module_t *)0;
    if (m->d) {
        if (strcmp(m->lang, lang) == 0) return m;
        fprintf(stderr, "rt_sc_module: the %s parser is loaded and a %s parse asks for its own -- one parser module per process until each has its own GVA island\n", m->lang, lang);
        return (sc_module_t *)0;
    }
    {
        Dl_info di;
        const char *so = dladdr((void *)rt_sc_module_busy, &di) && di.dli_fname ? di.dli_fname : "";
        const char *sl = strrchr(so, '/');
        size_t dl = sl ? (size_t)(sl - so) + 1 : 0;
        char path[dl + fmt_len("libscrip_parser_%s.so", lang)];
        char sym[fmt_len("parser_%s_sc_module", lang)];
        memcpy(path, so, dl);
        snprintf(path + dl, sizeof path - dl, "libscrip_parser_%s.so", lang);
        snprintf(sym, sizeof sym, "parser_%s_sc_module", lang);
        void *h = dlopen(path, RTLD_NOW | RTLD_LOCAL);
        const sc_module_desc_t *d = h ? (const sc_module_desc_t *)dlsym(h, sym) : (const sc_module_desc_t *)0;
        if (!d || !d->main_alpha) return (sc_module_t *)0;
        if (d->n_gva > SC_GVA_RESERVE) {
            fprintf(stderr, "rt_sc_module: the parser module has %ld GVA cells and the runtime reserves %d for it -- raise SC_GVA_RESERVE in rt_sc_module.c\n", d->n_gva, (int)SC_GVA_RESERVE);
            abort();
        }
        m->d = d;
        memcpy(m->lang, lang, strlen(lang) + 1);
    }
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
    int64_t s_anchor = g_kw_anchor, s_trim = g_kw_trim, s_full = g_kw_fullscan, s_max = g_kw_maxlngth, s_stl = g_kw_stlimit, s_stc = g_kw_stcount, s_tr = g_kw_trace, s_ftr = g_kw_ftrace;
    char *save_top = rt_gc_program_top_get();
    if (save_top) { fprintf(stderr, "rt_sc_module: the parser module was entered while a program is live -- a run-time parse takes the C parser (CEO-1749)\n"); abort(); }
    g_kw_anchor = 0;
    g_kw_trim = 0;
    g_kw_fullscan = 1;
    g_kw_maxlngth = 16777216;
    g_kw_stlimit = -1;
    g_kw_trace = 0;
    g_kw_ftrace = 0;
    rt_gc_program_top_set((char *)0);
    rt_sc_outer_call(m->d->main_alpha);
    rt_gc_program_top_set(save_top);
    g_kw_anchor = s_anchor;
    g_kw_trim = s_trim;
    g_kw_fullscan = s_full;
    g_kw_maxlngth = s_max;
    g_kw_stlimit = s_stl;
    g_kw_stcount = s_stc;
    g_kw_trace = s_tr;
    g_kw_ftrace = s_ftr;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sc_module_init(sc_module_t *m) {
    const sc_module_desc_t *d = m->d;
    {
        extern int g_core_stack_floor_raised;
        struct rlimit rl;
        if (getrlimit(RLIMIT_STACK, &rl) == 0 && rl.rlim_cur != RLIM_INFINITY && rl.rlim_cur < (rlim_t)(64L << 20)) {
            rl.rlim_cur = (rlim_t)(64L << 20);
            if (rl.rlim_max != RLIM_INFINITY && rl.rlim_cur > rl.rlim_max) rl.rlim_cur = rl.rlim_max;
            (void)setrlimit(RLIMIT_STACK, &rl);
        }
        g_core_stack_floor_raised = 1;
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
DESCR_t rt_sc_module_parse_begin(const char *lang, const char *src, size_t len, const char *path, const char *incl, size_t incl_len, long *errpos_out) {
    sc_module_t *m = sc_module_get(lang);
    DESCR_t none = { .v = DT_SNUL, .slen = 0, .s = "" };
    if (errpos_out) *errpos_out = -1;
    if (!m) return none;
    if (m->busy) { fprintf(stderr, "rt_sc_module: the parser module is re-entered while it is parsing\n"); return none; }
    m->busy = 1;
    if (!m->inited) sc_module_init(m);
    {
        char *hs = (char *)rt_heap_strdup_c(src ? src : "");
        m->base[m->i_src] = (DESCR_t){ .v = DT_S, .slen = (uint32_t)len, .s = hs };
        m->base[m->i_path] = path && *path ? STRVAL(rt_heap_strdup_c(path)) : NULVCL;
        m->base[m->i_lang] = STRVAL(rt_heap_strdup_c(lang));
        if (m->i_incl >= 0) m->base[m->i_incl] = incl && incl_len ? (DESCR_t){ .v = DT_S, .slen = (uint32_t)incl_len, .s = (char *)incl } : NULVCL;
        m->base[m->i_tree] = none;
        if (m->i_errpos >= 0) m->base[m->i_errpos] = (DESCR_t){ .v = DT_I, .slen = 0, .i = -1 };
        sc_module_run(m);
    }
    {
        DESCR_t t = m->base[m->i_tree];
        if (!(IS_DATA_INST_fn(t) && t.u) && m->i_errpos >= 0 && errpos_out) { DESCR_t ep = m->base[m->i_errpos]; *errpos_out = ep.v == DT_I ? (long)ep.i : -1; }
        return t;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_sc_module_parse_end(void) {
    sc_module_t *m = &g_sc_mod;
    if (!m->d) return;
    m->base[m->i_src] = (DESCR_t){ .v = DT_SNUL, .slen = 0, .s = "" };
    m->base[m->i_tree] = (DESCR_t){ .v = DT_SNUL, .slen = 0, .s = "" };
    m->busy = 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
__asm__(".globl rt_sc_outer_call\n.type rt_sc_outer_call, @function\n" "rt_sc_outer_call:\n" "  push %rbp\n  mov %rsp, %rbp\n  push %rbx\n  push %r12\n  push %r13\n  push %r14\n  push %r15\n"
    "  sub $8, %rsp\n  sub $1048576, %rsp\n  and $-16, %rsp\n  mov %rdi, %rax\n  push %rax\n  push %rax\n  mov %rsp, %rdi\n  call rt_program_top_adopt@PLT\n  pop %rax\n  pop %rax\n"
    "  mov 0x70000000, %r12\n  movq g_rtccb@GOTPCREL(%rip), %r10\n  movq 48(%r10), %r9\n  leaq 1f(%rip), %rcx\n  push %rcx\n  push %rcx\n  xor %esi, %esi\n  xor %r13d, %r13d\n  xor %r14d, %r14d\n"
    "  xor %r15d, %r15d\n  jmp *%rax\n" "1:\n" "  leaq 1b(%rip), %rcx\n  mov (%rsp), %rax\n  cmp %rax, %rcx\n  jne 2f\n  add $8, %rsp\n" "2:\n"
    "  lea -40(%rbp), %rsp\n  pop %r15\n  pop %r14\n  pop %r13\n  pop %r12\n  pop %rbx\n  pop %rbp\n  ret\n" ".size rt_sc_outer_call, .-rt_sc_outer_call\n");
