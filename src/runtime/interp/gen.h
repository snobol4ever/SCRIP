#ifndef GEN_H
#define GEN_H
#include <stdlib.h>
#include <string.h>
#include "bb_broker.h"
#include "core.h"
static DESCR_t fail_box(void *zeta, int entry) { (void)zeta; (void)entry; return FAILDESCR; }
static const bb_node_t FAIL_GEN_NODE = { fail_box, NULL, 0 };
static inline void *gen_gen_enter(void **pp, size_t size) {
    void *p = *pp;
    if (size) {
        if (p) memset(p, 0, size);
        else   p = *pp = calloc(1, size);
    }
    return p;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define GEN_ENTER(ref, T)  ((T *)gen_gen_enter((void **)(ref), sizeof(T)))
typedef struct { long lo; long hi; long cur; }                                        to_state_t;
#define TO_NESTED_MAX 256
typedef struct {
    long lo_vals[TO_NESTED_MAX];
    long hi_vals[TO_NESTED_MAX];
    int  nlo, nhi;
    int  li, hi2;
    long cur;
} to_nested_state_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { long lo; long hi; long step; long cur; }                             to_by_state_t;
typedef struct { double lo; double hi; double step; double cur; }                    to_by_real_state_t;
typedef struct { const char *str; long len; long pos; char ch[2]; }                  iterate_state_t;
typedef struct { TBBLK_t *tbl; int bucket; TBPAIR_t *entry; }                        tbl_iterate_state_t;
typedef struct { TBBLK_t *tbl; int bucket; TBPAIR_t *entry; }                        tbl_key_iterate_state_t;
typedef struct { DESCR_t list_obj; int pos; }                                         list_iterate_state_t;
typedef struct { DESCR_t inst; int pos; }                                             record_iterate_state_t;
typedef struct { const char *needle; const char *hay; int nlen; const char *next; }  find_state_t;
typedef struct {
    bb_node_t   subj_gen;
    const char *needle;
    int         nlen;
    const char *hay;
    const char *next;
    int         subj_entry;
} find_gen_subj_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct {
    bb_node_t   subj_gen;
    const char *cset;
    const char *hay;
    int         slen;
    int         pos;
    int         subj_entry;
} upto_gen_subj_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { const char *s; const char *c1; const char *c2; const char *c3; int slen; int pos; int endp; } bal_state_t;
typedef enum {
    BINOP_ADD, BINOP_SUB, BINOP_MUL, BINOP_DIV, BINOP_MOD,
    BINOP_LT, BINOP_LE, BINOP_GT, BINOP_GE, BINOP_EQ, BINOP_NE,
    BINOP_CONCAT,
    BINOP_SLT, BINOP_SLE, BINOP_SGT, BINOP_SGE, BINOP_SEQ, BINOP_SNE,
    BINOP_POW,
} BinopKind;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct {
    bb_node_t    left;
    bb_node_t    right;
    BinopKind op;
    int          is_relop;
    DESCR_t      left_val;
    DESCR_t      right_val;
    int          phase;
} binop_gen_state_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct {
    bb_node_t gen[2];
    int       which;
} alternate_state_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef EXPR_T_DEFINED
#define EXPR_T_DEFINED
typedef struct tree_t tree_t;
#endif
typedef struct {
    bb_node_t gen;
    long      max;
    long      count;
} limit_state_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct {
    bb_node_t  subj_gen;
    tree_t    *body;
    int        started;
    bb_node_t  body_gen;
    int        body_live;
    const char *body_subj;
    int         body_pos;
} scan_gen_state_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct {
    bb_node_t  gen;
    tree_t    *gen_ast;
    tree_t    *body;
    int        started;
} every_state_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct {
    bb_node_t  gen_a;
    bb_node_t  gen_b;
    tree_t    *ast_b;
    int        b_started;
    int        a_started;
} mutual_state_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct {
    tree_t   *proc_expr;
    bb_node_t arg_box;
    DESCR_t   cur_arg;
} bang_binary_state_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct {
    tree_t   **children;
    int        n;
    bb_node_t  last_box;
    int        started;
} seq_state_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { bb_node_t gen; struct tree_t *cat_expr; struct tree_t *leaf; } cat_gen_state_t;
extern void gather_trampoline(void);
typedef struct { tree_t *expr; int frame_popped; } not_state_t;
typedef struct { tree_t *expr; int started; int ever_succeeded; bb_node_t inner; } repalt_state_t;
typedef struct { tree_t *expr; tree_t *body; } while_state_t;
typedef struct { tree_t *expr; tree_t *body; } until_state_t;
typedef struct { tree_t *body; } repeat_state_t;
typedef struct { bb_node_t obj_gen; const char *field; } field_gen_state_t;
typedef struct { const char *kw; int fired; DESCR_t val; } kw_gen_state_t;
typedef enum { SEC_RANGE, SEC_PLUS, SEC_MINUS } sec_kind_t;
#define CASE_MAX     32
#define COMPOUND_MAX 32
#define LISTCON_MAX  64
typedef struct {
    DESCR_t disc; tree_t *clause_exprs[CASE_MAX]; tree_t *clause_bodies[CASE_MAX];
    int n_clauses; tree_t *dflt; int cur_clause; bb_node_t body_box; int body_started;
} case_state_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { tree_t *children[COMPOUND_MAX]; int n; bb_node_t last_box; int started; } compound_state_t;
typedef struct {
    bb_node_t val_gen; bb_node_t left_gen; bb_node_t right_gen;
    tree_t *val_expr; tree_t *left_expr; tree_t *right_expr;
    sec_kind_t kind; DESCR_t cur_val; DESCR_t cur_left;
    int val_started; int left_started; int right_started;
} section_gen_state_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { tree_t *children[LISTCON_MAX]; int n; int fired; } listcon_state_t;
typedef struct {
    tree_t *proc; int body_start; int nbody; int stmt_idx;
    bb_node_t expr_box; int in_suspend; tree_t *suspend_body; int frame_popped;
} proc_state_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { proc_state_t base; DESCR_t args[16]; int nargs; } proc_call_state_t;
typedef struct { int dummy; }                              noop_state_t;
typedef struct { long long val; }                          intlit_state_t;
typedef struct { double val; }                             reallit_state_t;
typedef struct { const char *s; }                          strlit_state_t;
typedef struct { const char *s; }                          csetlit_state_t;
typedef struct { int dummy; }                              global_state_t;
typedef struct { tree_t *cond; tree_t *then_e; tree_t *else_e; } if_state_t;
typedef struct { tree_t *body; }                           initial_state_t;
typedef struct { int dummy; }                              invocable_state_t;
typedef struct { int dummy; }                              link_state_t;
typedef struct { const char *name; int nfields; }          record_state_t;
typedef struct { tree_t *expr; }                           return_state_t;
typedef struct { int dummy; }                              fail_state_t;
typedef struct { const char *op; tree_t *operand; }        unop_state_t;
typedef struct { int dummy; }                              next_state_t;
typedef struct { tree_t *expr; }                           break_state_t;
typedef struct { tree_t *expr; }                           create_state_t;
typedef struct { int dummy; }                              coexplist_state_t;
typedef struct { int dummy; }                              arglist_state_t;
typedef struct { tree_t *proc; }                           procdecl_state_t;
typedef struct { tree_t *body; }                           procbody_state_t;
typedef struct { tree_t *init; tree_t *body; }             proccode_state_t;
#endif
