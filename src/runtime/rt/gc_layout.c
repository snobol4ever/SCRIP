#include <stddef.h>
#include "gc_heap.h"
#include "../core/core.h"
#include "gc_layout.h"
_Static_assert(sizeof(DESCR_t) == 16, "HB_DVEC is an array of DESCR_t cells, stride 16");
_Static_assert(sizeof(ARBLK_t) == 56 && offsetof(ARBLK_t, proto) == 24 && offsetof(ARBLK_t, data) == 32,
    "HB_ARR holds one ARBLK_t: the pointer members are proto and data; a new member means this row is re-read");
_Static_assert(sizeof(VCELL_t) == 72 && offsetof(VCELL_t, key_d) == 24 && offsetof(VCELL_t, sv) == 40,
    "HB_AGGV holds one VCELL_t: cellp, tbl and key raw words, key_d and sv DESCR_t cells; a new member means this row is re-read");
static const gc_layout_t gl_dvec = { HB_DVEC, sizeof(DESCR_t), 1, 0, { { 0, GL_DESCR } }, "DESCR_t[]" };
static const gc_layout_t gl_arr = { HB_ARR, 0, 2, 0, { { offsetof(ARBLK_t, proto), GL_RAW }, { offsetof(ARBLK_t, data), GL_RAW } }, "ARBLK_t" };
static const gc_layout_t gl_aggv = { HB_AGGV, 0, 5, 0, { { offsetof(VCELL_t, cellp), GL_RAW }, { offsetof(VCELL_t, tbl), GL_RAW }, { offsetof(VCELL_t, key), GL_RAW }, { offsetof(VCELL_t, key_d),
    GL_DESCR }, { offsetof(VCELL_t, sv), GL_DESCR } }, "VCELL_t" };
static const gc_layout_t *const gc_layouts[] = { &gl_dvec, &gl_arr, &gl_aggv };
const gc_layout_t *gc_layout_of(uint16_t type) {
    for (size_t i = 0; i < sizeof gc_layouts / sizeof gc_layouts[0]; i++) if (gc_layouts[i]->type == type) return gc_layouts[i];
    return (const gc_layout_t *)0;
}
