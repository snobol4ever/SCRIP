#include "gen_runtime.h"
#include "gen_value.h"
#include "ast.h"
#include "../../parser/snobol4/scrip_cc.h"
#include "gen.h"
#include "coerce.h"
#include "../by_name_dispatch.h"
#include "../../lower/lower.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <gc/gc.h>
extern bb_node_t gen_bb_make_proc_box(tree_t *proc, DESCR_t *args, int nargs);
#define NO_AST_WALK_GUARD(fn_name) \
    do { if (g_sm_dispatch_active && !g_ast_pump_active && g_lang == LANG_ICN) { \
        fprintf(stderr, "FATAL: " fn_name " reached from SM dispatch (Icon BB incomplete)\n"); \
        abort(); \
    } } while (0)
extern DESCR_t NV_SET_fn(const char *name, DESCR_t val);
int          g_lang         = 0;
tree_t      *g_root     = NULL;
int g_sm_dispatch_active = 0;
int g_ast_pump_active = 0;
unsigned long bb_rnd_seed = 12345UL;
GenFrame frame_stack[FRAME_STACK_MAX];
int      frame_depth = 0;
tree_t  *drive_node = NULL;
DESCR_t  drive_val;
/*--------------------------------------------------------------------------------------------------------------------*/
const char *scan_subj  = "";
int         scan_pos   = 1;
ScanEntry scan_stack[SCAN_STACK_MAX];
int         scan_depth = 0;
/*--------------------------------------------------------------------------------------------------------------------*/
ScanSubjRegs rt_scan_enter(uint64_t lo, uint64_t hi, uint64_t sigma, uint64_t delta, uint64_t Delta) {
    uint64_t w[2]; w[0] = lo; w[1] = hi; DESCR_t sv; memcpy(&sv, w, sizeof sv);
    if (IS_INT_fn(sv) || IS_REAL_fn(sv)) sv = descr_to_str(sv);
    const char *s = IS_NULL_fn(sv) ? "" : VARVAL_fn(sv);
    if (!s) s = "";
    if (scan_depth < SCAN_STACK_MAX) {
        scan_stack[scan_depth].subj  = scan_subj;
        scan_stack[scan_depth].pos   = scan_pos;
        scan_stack[scan_depth].sigma = sigma;
        scan_stack[scan_depth].delta = delta;
        scan_stack[scan_depth].Delta = Delta;
        scan_depth++;
    }
    scan_subj = s;
    scan_pos  = 1;
    ScanSubjRegs r; r.ptr = (uint64_t)(uintptr_t)s; r.len = (uint64_t)strlen(s);
    return r;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_scan_leave(uint64_t *out3) {
    if (scan_depth > 0) {
        scan_depth--;
        scan_subj = scan_stack[scan_depth].subj;
        scan_pos  = scan_stack[scan_depth].pos;
        if (out3) { out3[0] = scan_stack[scan_depth].sigma; out3[1] = scan_stack[scan_depth].delta; out3[2] = scan_stack[scan_depth].Delta; }
    } else if (out3) { out3[0] = 0; out3[1] = 0; out3[2] = 0; }
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_substr(const char *sigma, int64_t a, int64_t b) {
    if (!sigma) sigma = "";
    int64_t lo = a < b ? a : b;
    int64_t hi = a < b ? b : a;
    int64_t len = hi - lo;
    char *buf = (char *)GC_malloc((size_t)len + 1);
    if (len > 0) memcpy(buf, sigma + lo, (size_t)len);
    buf[len] = '\0';
    return STRVAL(buf);
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_keyword_subject(void) { return scan_subj ? STRVAL(scan_subj) : NULVCL; }
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_keyword_pos(void) { return INTVAL((int64_t)scan_pos); }
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
#include "../../driver/interp_private.h"
#include <time.h>
