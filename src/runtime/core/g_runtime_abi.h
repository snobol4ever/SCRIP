#ifndef G_RUNTIME_ABI_H
#define G_RUNTIME_ABI_H
#include <stdint.h>
#include <stddef.h>
#include "g_runtime_vec.h"
#define RT_DCAP_NVCACHE_N 16
struct DESCR_t;
extern const char g_scan_empty[1];
typedef struct {
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
} g_abi_t;
#endif
