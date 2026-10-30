#ifndef PROLOG_DB_BLOCKS_H
#define PROLOG_DB_BLOCKS_H
#include "../parsers/prolog/pl_cell.h"
#include "core/core.h"
#include "rt/rt_pl_trail.h"
typedef struct { pl_cell_t cl; int erased; int ref; int ridx; int chain_off; int next_idx; int frag; } pl_db_slot_t;
typedef struct {
    pl_db_slot_t *s;
    int n;
    int cap;
    int killed;
    int next_ref;
    int head_idx;
    int tail_idx;
    int cell_k;
    int erased_n;
    int recl_n;
    int recl_lo;
    int recl_gc;
    int recl_since;
    int frag_top;
    int tx_pins;
    void *ix;
    int ix_off;
    int data_on;
    int data_off;
    int entry_idx;
} pl_db_t;
typedef struct { char *key; int k; int meta; pl_db_t *db; int stat; int decl; int file; int line; int mf; int lib; } pl_db_key_t;
typedef struct {
    pl_db_key_t *e;
    int n;
    int cap;
    int next_cell;
    int ovf_cap;
    void **ovf;
    void **tries;
    int ntries;
    int tcap;
    int tbl_vt;
    int *hix;
    int hcap;
    long *mtok;
    int mtn;
    int mtcap;
} pl_db_reg_t;
typedef struct pl_trie_node_s { DESCR_t tok; DESCR_t val; struct pl_trie_node_s *parent; void **kids; int nkids; int kcap; int kind; int has_val; int ent; int depth; } pl_trie_node_t;
typedef struct { pl_trie_node_t *root; void **ents; int nents; int ecap; int nvals; int alive; DESCR_t status; DESCR_t data; pl_trie_sl_t sl; } pl_trie_t;
#endif
