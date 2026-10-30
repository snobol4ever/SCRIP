#include <stddef.h>
#include "gc_heap.h"
#include "../core/core.h"
#include "gc_slot_table.h"
_Static_assert(sizeof(DESCR_t) == 16, "HB_DVEC is an array of DESCR_t cells, stride 16");
_Static_assert(sizeof(ARBLK_t) == 56 && offsetof(ARBLK_t, proto) == 24 && offsetof(ARBLK_t, data) == 32,
    "HB_ARR holds one ARBLK_t: the pointer members are proto and data; a new member means this row is re-read");
_Static_assert(sizeof(VCELL_t) == 72 && offsetof(VCELL_t, key_d) == 24 && offsetof(VCELL_t, sv) == 40,
    "HB_AGGV holds one VCELL_t: cellp, tbl and key raw words, key_d and sv DESCR_t cells; a new member means this row is re-read");
static const gc_sfield_t ss_dvec[] = { { 0, GS_DESCR } };
static const gc_sfield_t ss_arr[] = { { offsetof(ARBLK_t, proto), GS_RAW }, { offsetof(ARBLK_t, data), GS_RAW } };
static const gc_sfield_t ss_aggv[] = { { offsetof(VCELL_t, cellp), GS_RAW }, { offsetof(VCELL_t, tbl), GS_RAW }, { offsetof(VCELL_t, key), GS_RAW }, { offsetof(VCELL_t, key_d), GS_DESCR },
    { offsetof(VCELL_t, sv), GS_DESCR } };
static const gc_slot_row_t sr_str = { 0, HB_ZCOL, 0, 0, (const gc_sfield_t *)0, "string block: bytes, no heap pointer" };
static const gc_slot_row_t sr_fill = { HB_FILL, HB_FILL, 0, 0, (const gc_sfield_t *)0, "FILL: a gap header over dead ground, no payload" };
static const gc_slot_row_t sr_dvec = { HB_DVEC, HB_DVEC, sizeof(DESCR_t), sizeof ss_dvec / sizeof ss_dvec[0], ss_dvec, "DESCR_t[]" };
static const gc_slot_row_t sr_arr = { HB_ARR, HB_ARR, 0, sizeof ss_arr / sizeof ss_arr[0], ss_arr, "ARBLK_t" };
static const gc_slot_row_t sr_aggv = { HB_AGGV, HB_AGGV, 0, sizeof ss_aggv / sizeof ss_aggv[0], ss_aggv, "VCELL_t" };
static const gc_slot_row_t *const gc_slot_rows[] = { &sr_str, &sr_fill, &sr_dvec, &sr_arr, &sr_aggv };
const gc_slot_row_t *gc_slot_row_of(uint16_t type) {
    for (size_t i = 0; i < sizeof gc_slot_rows / sizeof gc_slot_rows[0]; i++) if (type >= gc_slot_rows[i]->lo && type <= gc_slot_rows[i]->hi) return gc_slot_rows[i];
    return (const gc_slot_row_t *)0;
}
