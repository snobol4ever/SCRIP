#ifndef CALL_FORMS_H
#define CALL_FORMS_H
#include <stdint.h>
#include "IR.h"
#ifdef __cplusplus
extern "C" {
#endif
enum {
    CALL_ROUTE_FATAL = 0, CALL_ROUTE_BYNAME = 1, CALL_ROUTE_DVAL2_BOMB = 3, CALL_ROUTE_PROC_STAGED = 5, CALL_ROUTE_RK_BOOL_SLOT = 6, CALL_ROUTE_RK_BOOL_RELOP = 13, CALL_ROUTE_FN = 11,
        CALL_ROUTE_BYNAME_GEN = 12, CALL_ROUTE_RK_BOOL = 14
};
enum { CALL_OPEN_NONE = 0, CALL_OPEN_METHOD = 1, CALL_OPEN_REDISP = 2, CALL_OPEN_APPLY = 3, CALL_OPEN_EVAL = 4 };
enum { CALL_REFUSE_NONE = 0, CALL_REFUSE_NO_SLOT = 1, CALL_REFUSE_ARGC = 2, CALL_REFUSE_DET_LEAF = 3 };
enum {
    MARG_NONE = 0, MARG_DIRECT = 1, MARG_SLOT = 2, MARG_LIT_I = 3, MARG_LIT_F = 4, MARG_LIT_ATOM = 5, MARG_LIT_S = 6, MARG_DSTAR = 7, MARG_THUNK = 8, MARG_BOMB_TRAMPOLINE = 9, MARG_BOMB_GLOBAL = 10,
        MARG_BOMB_VARSLOT = 11
};
enum { PLAX_OTHER = 0, PLAX_ADD = 1, PLAX_SUB = 2, PLAX_MUL = 3, PLAX_IDIV = 4, PLAX_MOD = 5, PLAX_REM = 6 };
enum { PLTYPE_TAGS = 0, PLTYPE_VAR = 1, PLTYPE_NONVAR = 2 };
enum { PLATOP_OTHER = 0, PLATOP_EQ = 1, PLATOP_NE = 2 };
typedef struct { int kind; int src; uint64_t imm; const char * str; } marg_t;
struct stage2_t;
int lower_knob_unless_zero(const char * name);
long bid_bake_of(const char * fn);
int lower_call_is_scan_builtin(const char * fn);
int lower_call_is_curmov(const char * fn);
int lower_call_node_is_curmov(const IR_t * nd);
int sn4_byname_kind(const char * fn, int strict);
int bb_callee_baked_kind(const char * fn, int strict);
int lower_call_pas_bi(const char * fn);
int lower_call_tail_try(const char * fn, int strict);
int lower_call_open(const char * fn, int narg, int strict);
int lower_call_route(const IR_t * nd);
IR_e lower_call_road_kind(const IR_t * nd, int route);
void lower_call_forms_graph(IR_graph_t * g);
void lower_call_forms_all(struct stage2_t * s2);
int pl_ax_sub(const char * op);
int pl_type_sub(const char * kind);
const char * pl_type_tags(const char * kind);
int pl_type_rat(const char * kind);
int pl_atop_sub(const char * op);
const char * pl_cmp_fail_jcc(const char * op);
#ifdef __cplusplus
}
#endif
#endif
