#ifndef GC_LAYOUT_H
#define GC_LAYOUT_H
#include <stdint.h>
#define GL_RAW 1
#define GL_DESCR 2
#define GL_MAXSLOT 8
typedef struct gc_lslot_t { uint16_t off; uint16_t kind; } gc_lslot_t;
typedef struct gc_layout_t { uint16_t type; uint16_t stride; uint16_t nslot; uint16_t pad; gc_lslot_t slot[GL_MAXSLOT]; const char *from; } gc_layout_t;
const gc_layout_t *gc_layout_of(uint16_t type);
#endif
