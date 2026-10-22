#ifndef G_RUNTIME_VEC_H
#define G_RUNTIME_VEC_H
#include <stdint.h>
typedef struct gv_s { void *p; uint32_t len, cap, esz; uint16_t kind; uint16_t pad; } gv_t;
#endif
