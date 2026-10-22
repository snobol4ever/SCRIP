#ifndef G_RUNTIME_ABI_H
#define G_RUNTIME_ABI_H
#include <stdint.h>
#include <stddef.h>
#include "g_runtime_vec.h"
#define RT_DCAP_NVCACHE_N 16
struct DESCR_t;
struct rt_hblk_t;
struct gc_slot_t;
typedef struct rt_hp_fr_t {
    char *top;
    char *end;
    long blocks;
    int armed;
    int _pad;
    char *virgin;
    int zfull;
    int _pad2;
    char *line;
    long alloc_total;
    long alloc_str;
    uint64_t *live, *start_abs, *vis, *start, *ssum, *prefix;
    long ngran;
} rt_hp_fr_t;
typedef struct { char *owner; long len; int gva_n; int off; } rt_sxt_fr_t;
struct gc_mk_t {
    struct DESCR_t **wl;
    long wln, wlcap;
    struct gc_slot_t *slots;
    long nslot, scap;
    const char *arena, *top;
    const uint64_t *ssum, *start;
    uint64_t *live, *vis;
    struct rt_hblk_t *mhead;
    struct DESCR_t *rng_p;
    long rng_n, wlmax, interior, dvec_elems, dvec_nondvec, nmk, mxs, walked, pv_end, draining;
    const char *fc_anchor;
    const void *fc_map;
    const char *fc_lo, *fc_hi;
    long fc_i;
    const char *fc_c, *fc_e;
    long fc_kind;
    const char *sw_p, *sw_hi;
    void **hs;
    long hcap, hn, tb_h, code_unsorted, census, maps, diag, trap, plant, verify, telem, chain;
};
struct rt_proc_s;
extern const char g_scan_empty[1];
typedef struct {
    rt_hp_fr_t hp_fr;
    rt_sxt_fr_t sxt_fr;
    struct gc_mk_t gc_mk;
    long error;
    long trace;
    long dump;
    long random;
    long stno;
    long stcount;
    long lastno;
    long line;
    long lastline;
    const char *file;
    int64_t kw_ftrace;
    int64_t kw_errlimit;
    int64_t kw_fnclevel;
    char kw_rtntype[16];
    long trace_budget;
    int call_fastpath_off;
    int monitor_fd;
    int monitor_bin;
    int mon_max_stno;
    int trace_set_n;
    int comm_dbg;
    int sno_etrace_n;
    gv_t var_buckets;
    unsigned var_nbuckets;
    int var_init_done;
    unsigned long nv_memo_gen;
    int protected_pat_vars_armed;
    int dcap_trace;
    uint32_t cap_gen;
    uint32_t cap_gen_next;
    const char *dcap_nv_key[RT_DCAP_NVCACHE_N];
    struct DESCR_t *dcap_nv_cell[RT_DCAP_NVCACHE_N];
    unsigned long dcap_nv_seen[RT_DCAP_NVCACHE_N];
    uint64_t sno_defer_cells[2048];
    const char *scan_subj;
    long scan_subj_len;
    const char *scan_subj_ptr;
    int scan_pos;
    int scan_depth;
    int repl_trace;
    int rt_k_level;
    unsigned long zdp_anchor_rsp;
    unsigned long zdp_anchor_rbp;
    int gc_in;
    long gc_polls;
    const char *gc_poll_site;
    char *hp_gcline;
    long anchor;
    long maxlngth;
    unsigned char rtcc_on;
    struct rt_proc_s *rt_gen_procs;
} g_abi_t;
#endif
