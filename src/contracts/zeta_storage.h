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
void         zls_build(IR_graph_t * g);
int          zls_off(const IR_t * nd);
int          zls_arbno_geom(const IR_t * nd, int * min_off, int * span);
int          zls2_geom(const IR_t * nd, int base_off, int * slot_off, long * k);
int          zls_scope_of(const IR_t * nd);
int          zls_g_nslots(const IR_graph_t * g);
int          zls_g_region(const IR_graph_t * g);
int          zls_g_resume(const IR_graph_t * g);
int          zls_g_zeta_mark(const IR_graph_t * g);
int          zls_node_bytes(const IR_t * nd);
int          zls_g_vslot_count(const IR_graph_t * g);
const char * zls_g_vslot_get(const IR_graph_t * g, int i, int * off);
void         zls_dump(FILE * fp);
#endif
