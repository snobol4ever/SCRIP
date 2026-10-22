#ifndef G_RUNTIME_CORE_H
#define G_RUNTIME_CORE_H
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <setjmp.h>
#include "g_runtime_vec.h"
#include "ct_vec.h"
#define DATA_MAX_TYPES 64
#define DATA_MAX_FIELDS 16
#define FIELD_ACCESSOR_MAX (DATA_MAX_TYPES * DATA_MAX_FIELDS)
#define NV_MEMO_N 2048
struct g_data_type { char *tname; int nfields; char *fields[DATA_MAX_FIELDS]; };
struct g_facc_slot { int tidx; int fidx; };
struct _VarEntry;
struct _DATINST_tType;
struct rt_slab;
typedef struct {
    int monitor_ack_fd;
    int monitor_ready;
    int trace_tap_off;
    gv_t g_trace_tab;
    int trace_access_n;
    int trace_recursion_depth;
    long trace_depth_override;
    int trace_stmt_seen;
    long trace_stmt_gen;
    gv_t trace_value_last;
    char **bin_names;
    int *bin_name_lens;
    int bin_n_names;
    int bin_names_cap;
    int64_t kw_fullscan;
    int64_t kw_maxlngth;
    int64_t kw_trim;
    int64_t kw_code;
    const char *host_u;
    long sw_stack_bytes;
    gv_t io_chan;
    FILE *input_fp;
    int input_fp_is_popen;
    int64_t start_ns;
    gv_t setexit_label;
    int setexit_resume;
    int data_ntypes;
    int facc_n;
    jmp_buf core_err_jmp;
    struct _DATINST_tType *udef_types;
    cv_t udef_nidx;
    unsigned long var_count;
    const char *nv_memo_key[NV_MEMO_N];
    struct _VarEntry *nv_memo_val[NV_MEMO_N];
    unsigned long nv_memo_seen[NV_MEMO_N];
    struct rt_slab *func_bslab;
    char *input_buf;
    size_t input_cap;
    long input_rlen;
    FILE *terminal_fp;
    int terminal_tried;
    int (*label_exists_hook)(const char *);
    struct g_data_type data_types[DATA_MAX_TYPES];
    struct g_facc_slot facc_slots[FIELD_ACCESSOR_MAX];
} g_core_t;
#endif
