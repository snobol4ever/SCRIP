#ifndef GEN_RUNTIME_H
#define GEN_RUNTIME_H
#include "ast.h"
#include "../../parser/snobol4/scrip_cc.h"
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
#define GLOBAL_MAX      64
struct GeneratorState;
typedef struct { tree_t *node; long cur; const char *sval; } ScopeEntry;
typedef struct {
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
} GenFrame;
/*--------------------------------------------------------------------------------------------------------------------*/
static inline IR_graph_t *bb_graph_of_proc(const ProcEntry *e)
{
    if (!e) return NULL;
    if (e->bb_idx >= 0 && e->bb_idx < g_stage2.bbp.count)
        return g_stage2.bbp.table[e->bb_idx];
    return NULL;
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern int          g_lang;
extern tree_t      *g_root;
extern GenFrame     frame_stack[FRAME_STACK_MAX];
extern int          frame_depth;
#define FRAME (frame_stack[frame_depth - 1])
extern const char  *scan_subj;
extern int          scan_pos;
typedef struct { const char *subj; int pos; } ScanEntry;
extern ScanEntry scan_stack[SCAN_STACK_MAX];
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
DESCR_t sm_call_proc(int entry_pc, int nparams, DESCR_t *args, int nargs);
DESCR_t proc_table_call(int pi, DESCR_t *args, int nargs);
bb_node_t gen_bb_pump_proc_by_name(const char *name, DESCR_t *args, int nargs);
int       is_suspendable(tree_t *e);
void      init_save_frame(void);
const char *real_str(double r, char *buf, int bufsz);
int descr_identical(DESCR_t a, DESCR_t b);
const char *cset_complement(const char *cs);
const char *cset_union(const char *a, const char *b);
const char *cset_diff(const char *a, const char *b);
const char *cset_inter(const char *a, const char *b);
const char *cset_canonical(const char *cs);
int kw_assign(const char *kw, DESCR_t val);
int kw_can_assign(const char *kw, DESCR_t val);
DESCR_t kw_read(const char *kw);
const char *kw_cset_name(const char *ptr);
int kw_cset_len(const char *ptr);
extern int g_sm_dispatch_active;
extern int g_ast_pump_active;
#define NO_AST_WALK_GUARD(fn_name) \
    do { if (g_sm_dispatch_active && !g_ast_pump_active && g_lang == LANG_ICN) { \
        fprintf(stderr, "FATAL: " fn_name " reached from SM dispatch (Icon BB incomplete)\n"); \
        abort(); \
    } } while (0)
#endif
