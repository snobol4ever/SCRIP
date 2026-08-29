#ifndef GEN_RUNTIME_H
#define GEN_RUNTIME_H
#include "ast.h"
#include "../../frontend/snobol4/scrip_cc.h"
#include "bb_box.h"
#include "gen.h"
#include "IR.h"
#include "SM.h"
#include "stage2.h"
#define FRAME_SLOT_MAX        STAGE2_FRAME_SLOT_MAX
#define CORO_STACK_SZ         (1024 * 1024)
#define PROC_TABLE_MAX        STAGE2_PROC_TABLE_MAX
#define FRAME_DEPTH_MAX         16
#define FRAME_STACK_MAX      256
#define EVERY_GEN_SLOT_MAX    16
#define SCAN_STACK_MAX  16
#define GLOBAL_MAX      4096
struct GeneratorState;
typedef struct { tree_t *node; long cur; const char *sval; } ScopeEntry;
typedef struct GenFrame GenFrame;
typedef struct { unsigned char is_ref; GenFrame *frame; int slot; const char *name; } SlotRef;
struct GenFrame {
    DESCR_t       env[FRAME_SLOT_MAX];
    int           env_n;
    int           returning;
    DESCR_t       return_val;
    ScopeEntry gen[FRAME_DEPTH_MAX];
    int           gen_depth;
    int           loop_break;
    int           loop_next;
    tree_t       *body_root;
    Scope      sc;
    int           suspending;
    DESCR_t       suspend_val;
    tree_t       *suspend_do;
    struct GeneratorState *every_gen[EVERY_GEN_SLOT_MAX];
    SlotRef       slotref[FRAME_SLOT_MAX];
    GenFrame     *static_link;
    int           level;
};
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline IR_graph_t *bb_graph_of_proc(const ProcEntry *e)
{
    if (!e) return NULL;
    if (e->bb_idx >= 0 && e->bb_idx < g_stage2.bbp.count)
        return g_stage2.bbp.table[e->bb_idx];
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline IR_t *bb_proc_entry(const ProcEntry *e)
{
    IR_graph_t *g = bb_graph_of_proc(e);
    if (!g) return NULL;
    if (e->proc_entry_node) return e->proc_entry_node;
    return g->entry;
}
extern tree_t      *g_root;
extern GenFrame     frame_stack[FRAME_STACK_MAX];
extern int          frame_depth;
#define FRAME (frame_stack[frame_depth - 1])
extern const char  *scan_subj;
extern int          scan_pos;
typedef struct { const char *subj; int pos; } ScanEntry;
typedef struct { uint64_t ptr; uint64_t len; } ScanSubjRegs;
typedef struct { const char *subj; int pos; int depth; int saved_depth; ScanEntry saved[SCAN_STACK_MAX]; } ScanState;
void  *rt_scan_state_capture(void *prev);
void   rt_scan_state_apply(void *saved);
void   rt_scan_state_reset(void);
unsigned long rt_scan_state_size(void);
ScanSubjRegs rt_scan_enter(uint64_t lo, uint64_t hi);
ScanSubjRegs rt_scan_needle(uint64_t lo, uint64_t hi);
ScanSubjRegs rt_keyword_subject_set(uint64_t lo, uint64_t hi);
ScanSubjRegs rt_match_enter(uint64_t lo, uint64_t hi);
void rt_match_ctx_restore(uint64_t sig, uint64_t len, uint64_t capgen);
DESCR_t rt_match_capture(uint64_t sigma, int64_t start, int64_t end, const char *var);
void rt_scan_leave(uint64_t outer_sigma, uint64_t outer_delta);
ScanSubjRegs rt_scan_reenter(void);
void rt_scan_sync_out(uint64_t delta);
uint64_t rt_scan_sync_in(void);
DESCR_t rt_substr(const char *sigma, int64_t a, int64_t b);
extern int          scan_depth;
extern const char  *global_names[GLOBAL_MAX];
extern int          global_count;
int     frame_lookup(tree_t *n, long *out);
int     frame_lookup_sv(tree_t *n, long *out, const char **sv);
int     is_global(const char *name);
void    global_register(const char *name);
extern DESCR_t  drive_val;
int     scope_add(Scope *sc, const char *name);
int     scope_get(Scope *sc, const char *name);
void    scope_patch(Scope *sc, tree_t *e);
int     static_get(tree_t *proc, const char *name, DESCR_t *out);
void    static_set(tree_t *proc, const char *name, DESCR_t val);
DESCR_t proc_table_call(int pi, DESCR_t *args, int nargs);
int       is_suspendable(tree_t *e);
void      init_save_frame(void);
const char *real_str(double r, char *buf, int bufsz);
int descr_identical(DESCR_t a, DESCR_t b);
const char *cset_complement(const char *cs);
const char *cset_union(const char *a, int alen, const char *b, int blen);
const char *cset_diff(const char *a, int alen, const char *b, int blen);
const char *cset_inter(const char *a, int alen, const char *b, int blen);
const char *cset_canonical(const char *cs);
#include "../keywords.h"
#endif
