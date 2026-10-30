#include <stddef.h>
#include "gc_heap.h"
#include "../core/core.h"
#include "gc_slot_table.h"
_Static_assert(sizeof(DESCR_t) == 16, "HB_DVEC is an array of DESCR_t cells, stride 16");
_Static_assert(sizeof(ARBLK_t) == 56 && offsetof(ARBLK_t, proto) == 24 && offsetof(ARBLK_t, data) == 32,
    "HB_ARR holds one ARBLK_t: the pointer members are proto and data; a new member means this row is re-read");
_Static_assert(sizeof(VCELL_t) == 72 && offsetof(VCELL_t, key_d) == 24 && offsetof(VCELL_t, sv) == 40,
    "HB_AGGV holds one VCELL_t: cellp, tbl and key raw words, key_d and sv DESCR_t cells; a new member means this row is re-read");
_Static_assert(sizeof(TBBLK_t) == 200 && offsetof(TBBLK_t, buckets) == 0 && offsetof(TBBLK_t, dflt) == 32 && offsetof(TBBLK_t, ord) == 56 && offsetof(TBBLK_t, icn_ord) == 112,
    "HB_AGGT holds one TBBLK_t: the pointer members are buckets, dflt (a DESCR_t), ord and icn_ord; a new member means this row is re-read");
_Static_assert(sizeof(TBBUCK_t) == 8 && offsetof(TBBUCK_t, len) == 0 && offsetof(TBBUCK_t, ent) == 8 && sizeof(((TBBUCK_t *)0)->len) == 4 && sizeof(TBPAIR_t) == 40 &&
    offsetof(TBPAIR_t, key_descr) == 0 && offsetof(TBPAIR_t, val) == 16, "HB_AGGB holds one TBBUCK_t: len, cap, then len TBPAIR_t entries; key_descr and val are DESCR_t cells");
_Static_assert(sizeof(DATINST_t) == 32 && offsetof(DATINST_t, type) == 0 && offsetof(DATINST_t, fields) == 8,
    "HB_DINST holds one DATINST_t: the pointer members are type and fields, then the inline DESCR_t field cells when fields points past the header");
static const gc_sfield_t ss_descr[] = { { 0, GS_DESCR } };
static const gc_sfield_t ss_raw[] = { { 0, GS_RAW } };
static const gc_run_t run_descr = { 0, sizeof(DESCR_t), 0, 0, sizeof ss_descr / sizeof ss_descr[0], ss_descr };
static const gc_run_t run_raw = { 0, sizeof(char *), 0, 0, sizeof ss_raw / sizeof ss_raw[0], ss_raw };
static const gc_sfield_t ss_arr[] = { { offsetof(ARBLK_t, proto), GS_RAW }, { offsetof(ARBLK_t, data), GS_RAW } };
static const gc_sfield_t ss_aggv[] = { { offsetof(VCELL_t, cellp), GS_RAW }, { offsetof(VCELL_t, tbl), GS_RAW }, { offsetof(VCELL_t, key), GS_RAW }, { offsetof(VCELL_t, key_d), GS_DESCR },
    { offsetof(VCELL_t, sv), GS_DESCR } };
static const gc_sfield_t ss_aggt[] = { { offsetof(TBBLK_t, dflt), GS_DESCR }, { offsetof(TBBLK_t, ord), GS_RAW }, { offsetof(TBBLK_t, icn_ord), GS_RAW }, { offsetof(TBBLK_t, buckets), GS_RAW } };
static const gc_sfield_t ss_aggb_e[] = { { offsetof(TBPAIR_t, key_descr), GS_DESCR }, { offsetof(TBPAIR_t, val), GS_DESCR } };
static const gc_run_t run_aggb = { offsetof(TBBUCK_t, ent), sizeof(TBPAIR_t), offsetof(TBBUCK_t, len), sizeof(((TBBUCK_t *)0)->len), sizeof ss_aggb_e / sizeof ss_aggb_e[0], ss_aggb_e };
static const gc_sfield_t ss_dinst[] = { { offsetof(DATINST_t, type), GS_RAW }, { offsetof(DATINST_t, fields), GS_RAW } };
static const gc_run_t run_dinst = { sizeof(DATINST_t), sizeof(DESCR_t), 0, 0, sizeof ss_descr / sizeof ss_descr[0], ss_descr };
static const gc_slot_row_t sr_str = { 0, HB_ZCOL, GR_TYPED, 0, (const gc_sfield_t *)0, (const gc_run_t *)0, "string block: bytes, no heap pointer" };
static const gc_slot_row_t sr_fill = { HB_FILL, HB_FILL, GR_TYPED, 0, (const gc_sfield_t *)0, (const gc_run_t *)0, "FILL: a gap header over dead ground, no payload" };
static const gc_slot_row_t sr_zblk = { HB_ZBLK, HB_ZBLK, GR_REFUSE, 0, (const gc_sfield_t *)0, (const gc_run_t *)0, "ZBLK: no producer and no struct; a block of this type seen is a finding" };
static const gc_slot_row_t sr_wsc = { HB_WSC, HB_WSC, GR_UNTYPED, 0, (const gc_sfield_t *)0, (const gc_run_t *)0, "WSC: untyped char workspace (35 producers, bytes and some pointer arrays)" };
static const gc_slot_row_t sr_aggv = { HB_AGGV, HB_AGGV, GR_TYPED, sizeof ss_aggv / sizeof ss_aggv[0], ss_aggv, (const gc_run_t *)0, "VCELL_t" };
static const gc_slot_row_t sr_aggp = { HB_AGGP, HB_AGGP, GR_TYPED, 0, (const gc_sfield_t *)0, &run_raw, "TBBUCK_t *[]: a table's bucket pointer vector" };
static const gc_slot_row_t sr_aggt = { HB_AGGT, HB_AGGT, GR_TYPED, sizeof ss_aggt / sizeof ss_aggt[0], ss_aggt, (const gc_run_t *)0, "TBBLK_t" };
static const gc_slot_row_t sr_aggb = { HB_AGGB, HB_AGGB, GR_TYPED, 0, (const gc_sfield_t *)0, &run_aggb, "TBBUCK_t: len, cap, TBPAIR_t ent[len]" };
static const gc_slot_row_t sr_arr = { HB_ARR, HB_ARR, GR_TYPED, sizeof ss_arr / sizeof ss_arr[0], ss_arr, (const gc_run_t *)0, "ARBLK_t" };
static const gc_slot_row_t sr_dinst = { HB_DINST, HB_DINST, GR_TYPED, sizeof ss_dinst / sizeof ss_dinst[0], ss_dinst, &run_dinst, "DATINST_t, then its inline DESCR_t fields" };
static const gc_slot_row_t sr_dvec = { HB_DVEC, HB_DVEC, GR_TYPED, 0, (const gc_sfield_t *)0, &run_descr, "DESCR_t[]" };
static const gc_slot_row_t sr_wsb = { HB_WSB, HB_WSB, GR_UNTYPED, 0, (const gc_sfield_t *)0, (const gc_run_t *)0,
    "WSB: untyped workspace (321 producers; its pointers are described by the owner's root visitor)" };
static const gc_slot_row_t sr_pvec = { HB_PVEC, HB_PVEC, GR_TYPED, 0, (const gc_sfield_t *)0, &run_raw, "void *[]: a vector of raw words" };
static const gc_slot_row_t *const gc_slot_rows[] = { &sr_str, &sr_fill, &sr_zblk, &sr_wsc, &sr_aggv, &sr_aggp, &sr_aggt, &sr_aggb, &sr_arr, &sr_dinst, &sr_dvec, &sr_wsb, &sr_pvec };
const gc_slot_row_t *gc_slot_row_of(uint16_t type) {
    for (size_t i = 0; i < sizeof gc_slot_rows / sizeof gc_slot_rows[0]; i++) if (type >= gc_slot_rows[i]->lo && type <= gc_slot_rows[i]->hi) return gc_slot_rows[i];
    return (const gc_slot_row_t *)0;
}
