#ifndef LOWER_LOWER_ICON_LIC_HEAD_H
#define LOWER_LOWER_ICON_LIC_HEAD_H
#include "lower/g_lower.h"
typedef struct icn_loop_fr { IR_t * exit; IR_t * next; IR_t * fail; struct icn_loop_fr * up; } icn_loop_fr;
typedef struct icn_scan_fr { IR_t * enter; struct icn_scan_fr * up; } icn_scan_fr;
typedef struct {
    IR_graph_t * g;
    IR_t * psucc;
    IR_t * pfail;
    const char ** pn;
    int npn;
    const char ** ln;
    int nln;
    const char ** gn;
    int ngn;
    const char * pname;
    IR_t * last_gen;
    IR_t * loop_exit;
    IR_t * loop_break_beta;
    IR_t * loop_next;
    IR_t * beta;
    IR_t * conj_resumable;
    icn_loop_fr * loop_top;
    IR_t * loop_fail;
    int loop_sp;
    icn_scan_fr * scan_top;
    int scan_sp;
    int loop_next_ssp;
    int want_lines;
    int has_trap;
    int end_line;
    int after_line;
    int key_after_line;
    const tree_t * op_last;
    int op_line;
    const tree_t * discard;
    int ret_lv;
    int kw_snap;
    int var_road;
    const char * file;
    int links;
    const tree_t * prog;
    const tree_t * varcase_arg;
    const tree_t * body;
} icx_t;
#define ICN_IF_FROM_INITIAL 0x1417
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct rx_s { char k; const char * s; int n; struct rx_s * a, * b; } rx_t;
typedef struct { const char * p, * e; int line; } rxs_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif
