#ifndef G_RUNTIME_EVAL_H
#define G_RUNTIME_EVAL_H
struct g_eval_cache_ent;
struct g_lbl_ent;
typedef struct {
    struct g_eval_cache_ent *eval_cache;
    int eval_cache_n;
    int eval_cache_cap;
    int rt_fragment_emit;
    int eval_ret_v;
    struct g_lbl_ent *lbl_tab;
    int lbl_n;
    int lbl_cap;
    long sno_stmt_compiled;
} g_eval_t;
#endif
