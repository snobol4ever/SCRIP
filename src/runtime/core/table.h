#ifndef TABLE_H
#define TABLE_H
#include <stdint.h>
#include "ir/descr.h"
#include "ir/descr_tags_dt_platom.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct _TBBLK_tEntry { DESCR_t key_descr; DESCR_t val; unsigned long long hkey; } TBPAIR_t;
typedef struct _TBBUCK_t { unsigned len, cap; TBPAIR_t ent[]; } TBBUCK_t;
#define TABLE_BUCKETS 256
typedef struct _TBBLK_t {
    TBBUCK_t **buckets;
    unsigned nbuck;
    int size;
    int init, inc;
    int is_set;
    DESCR_t dflt;
    long id;
    DESCR_t *ord;
    unsigned ord_len, ord_cap;
    unsigned long icn_mask;
    long dumpno;
    int64_t gen_idx, gen_pos;
    unsigned ord_dead;
    DESCR_t *icn_ord;
    unsigned icn_ord_len, icn_ord_cap, icn_stamp_len, icn_stamp_dead;
    unsigned long icn_stamp_mask, icn_stamp_size;
    unsigned long gen_mask, gen_lseg, gen_lslot, gen_lhn;
    unsigned gen_lseq;
    int gen_have;
    int null_one;
} TBBLK_t;
typedef struct _VCELL_t { DESCR_t *cellp; struct _TBBLK_t *tbl; const char *key; DESCR_t key_d; DESCR_t sv; long pos; long len; } VCELL_t;
#define TBL_FOREACH(t_, e_) for (unsigned _tb = 0; _tb < (t_)->nbuck; _tb++) if ((t_)->buckets[_tb]) for (unsigned _ts = 0; _ts < (t_)->buckets[_tb]->len && ((e_) = &(t_)->buckets[_tb]->ent[_ts]) != \
    (TBPAIR_t *)0; _ts++)
TBBLK_t *table_new(void);
TBBLK_t *set_new(void);
TBBLK_t *table_new_args(int init, int inc);
DESCR_t agg_prototype(DESCR_t v);
int tbl_key_equal(DESCR_t a, DESCR_t b);
long tbl_key_serial(DESCR_t k);
TBPAIR_t *table_find_pair_d(TBBLK_t *tbl, DESCR_t k);
int table_icn_nth(TBBLK_t *tbl, int64_t idx, TBPAIR_t **out);
TBPAIR_t *c_table_find_pair_d(TBBLK_t *tbl, DESCR_t k);
DESCR_t table_get_d(TBBLK_t *tbl, DESCR_t k);
DESCR_t table_get_found_d(TBBLK_t *tbl, DESCR_t k, int *found);
int table_has_d(TBBLK_t *tbl, DESCR_t k);
int table_delete_d(TBBLK_t *tbl, DESCR_t k);
void table_set_descr_d(TBBLK_t *tbl, DESCR_t k, DESCR_t val);
void table_copy_in_order(TBBLK_t *dst, TBBLK_t *src);
TBBLK_t *set_union(TBBLK_t *x, TBBLK_t *y);
TBBLK_t *set_diff(TBBLK_t *x, TBBLK_t *y);
TBBLK_t *set_inter(TBBLK_t *x, TBBLK_t *y);
#define TABLE_VAL(tbl_) ((DESCR_t){ .v = DT_T, .tbl = (tbl_) })
#ifdef __cplusplus
}
#endif
#endif
