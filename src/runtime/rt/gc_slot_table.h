#ifndef GC_SLOT_TABLE_H
#define GC_SLOT_TABLE_H
#include <stdint.h>
#define GS_RAW 1
#define GS_DESCR 2
#define GR_TYPED 0
#define GR_UNTYPED 1
#define GR_REFUSE 2
typedef struct gc_sfield_t { uint16_t off; uint16_t kind; } gc_sfield_t;
typedef struct gc_run_t { uint16_t off; uint16_t stride; uint16_t count_off; uint16_t count_w; uint16_t nslot; const gc_sfield_t *slot; } gc_run_t;
typedef struct gc_slot_row_t { uint16_t lo; uint16_t hi; uint16_t cls; uint16_t nslot; const gc_sfield_t *slot; const gc_run_t *run; const char *from; } gc_slot_row_t;
const gc_slot_row_t *gc_slot_row_of(uint16_t type);
#endif
