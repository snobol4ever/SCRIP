#ifndef LOWER_LOWER_SNOBOL4_LSN_HEAD_H
#define LOWER_LOWER_SNOBOL4_LSN_HEAD_H
#include "ir/ir_node.h"
#include "runtime/builtins/gen_runtime_lower_common_api.h"
#include "ir/ast.h"
extern void global_register(const char * name);
typedef struct { const tree_t * arg; IR_t * prim; int str; long codes; const char * snapg; int ppos; IR_t * prim2; } sprearg_t;
typedef struct sno_kw_link { const char * nm; const struct sno_kw_link * up; } sno_kw_link_t;
struct sno_tvec_s;
typedef struct {
    IR_graph_t * g;
    const sno_kw_link_t * kw_chain;
    IR_t * loop_exit;
    IR_t * loop_next;
    const char * result_name;
    IR_t * pat_fail;
    IR_t * pat_seal;
    sprearg_t pre[64];
    int npre;
    int snap;
    int ppos;
    long prog_nstmt;
    long prog_endline;
    long stno_base;
    IR_t * seq_rlast;
    IR_t * seq_rlast_for;
    IR_t * seq_rtail;
    const tree_t * wn_tree;
    struct sno_tvec_s * capro;
    struct sno_tvec_s * capro_cand;
} scx_t;
typedef struct { const char * fname; const char * entry; const char * result_name; const char ** names; int nnames; int nformals; } sno_def_t;
typedef struct { const char * name; const tree_t * expr; int salt; int want_name; int nm; const char * svar; } sno_expr_ent_t;
typedef struct { const char * name; const tree_t * pat; int salt; } sno_pat_ent_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { int code; const char * msg; } sno_pe_t;
#define SNO_PE_212 "syntax error: value used where name is required"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern int ir_is_generator_kind(IR_e t);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct sno_tvec_s { const tree_t ** v; char * rc; int n, cap; } sno_tvec_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { const char * name; const tree_t * pat; const tree_t * val; } sno_seal_ent_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { const char * op; const char * tgt; int arity; int poisoned; } sno_t4_t;
typedef struct { const IR_t * nd; const IR_t * save; int nd_idx; int save_idx; int i_end; int fp_inner; int promo; } sno_scd_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { char *buf; size_t len; size_t cap; } sno_list_sb_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define SNO_LIST_NP 60
#define SNO_LIST_PRLEN 120
#define SNO_LIST_PGOFS 112
#define SNO_LIST_STNPD 8
#define SNO_LIST_VERSION "4.0f"
typedef struct { sno_list_sb_t sb; int lstlc; int lstpg; int headp; int cnttl; int cswls; int cswpr; int cswdb; const char * ttl; const char * stl; } sno_list_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif
