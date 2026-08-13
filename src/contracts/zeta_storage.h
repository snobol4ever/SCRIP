#ifndef ZETA_STORAGE_H
#define ZETA_STORAGE_H
#include <stdio.h>
#include "IR.h"
enum { ZK_DESCR = 0, ZK_RAW = 1, ZK_PTR_GC = 2, ZK_PTR_CODE = 3 };
enum { ZSC_FN = 0, ZSC_GROUP = 1, ZSC_ITER = 2, ZSC_PAT = 3, ZSC_COEXPR = 4 };
typedef struct { int off; int size; unsigned char kind; unsigned char audit; const char * what; } zls_field_t;
typedef struct { int id; int parent; int klass; const char * name; int first_field; int n_fields; int lo_off; int hi_off; } zls_scope_t;
void         zls_reset(void);
void         zls_graph_name(const IR_graph_t * g, const char * name);
void         zls_group_mark(const IR_graph_t * g, const char * name);
void         zls_group_mark_anchor(const IR_graph_t * g, const char * name, const IR_t * anchor);
int          zls_g_group_count(const IR_graph_t * g);
const IR_t * zls_g_group_anchor(const IR_graph_t * g, int k);
void         zls_build(IR_graph_t * g);
int          zls_off(const IR_t * nd);
int          zls_result_off(const IR_t * nd);
int          zls_result_live(const IR_t * nd);
int          zls_arbno_geom(const IR_t * nd, int * min_off, int * span);
int          zls2_geom(const IR_t * nd, int base_off, int * slot_off, long * k);
int          zls_scope_of(const IR_t * nd);
int          zls_g_nslots(const IR_graph_t * g);
int          zls_g_region(const IR_graph_t * g);
int          zls_g_resume(const IR_graph_t * g);
int          zls_g_resume_by_name(const char *name);   /* ICN-FR-4: emit-time callee-resume-slot lookup by proc name; avoids rt_proc_t/rt_pcall_t struct growth */
int          zls_g_zeta_mark(const IR_graph_t * g);
int          zls_g_locals(const IR_graph_t * g);
int          zls_node_bytes(const IR_t * nd);
int          zls_g_vslot_count(const IR_graph_t * g);
const char * zls_g_vslot_get(const IR_graph_t * g, int i, int * off);
void         zls_dump(FILE * fp);
#endif

void fc_alt_register(const IR_t * nd, int n, const int * fp, const int * ab, const int * ae);
int fc_alt_fpmax(const IR_t * nd);
int fc_alt_fp(const IR_t * nd, int j);
int fc_alt_n(const IR_t * nd);
int fc_alt_extent(const IR_t * nd, int * b, int * e);
int fc_alt_arm_range(const IR_t * nd, int j, int * b, int * e);
void fc_seq_register(const IR_t * nd);
int fc_seq_active(const IR_t * nd);
void fc_head_register(const IR_t * nd, int fp);
int fc_head_fp(const IR_t * nd);
void fc_tables_reset(void);
int  fc_frameless_fpr_rsp(const IR_t * nd);   /* OS-2·SLICE-ARBNO-FPR: 1 iff ZC_PORT_FORTH && fc_geom(nd)==0 (flat-allocated, rsp frontier, no ZLS slot); callers gate on g_zd_arm separately */
int  fc_cells_active(void);   /* Z4-6 PUBLIC ACCESSOR (RBX census): sanctioned external read of the private fc_cells_on() ONE-PORT-OPINION -- FORTH-or-HEAP. */
