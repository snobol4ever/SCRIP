#ifndef G_RUNTIME_PC_H
#define G_RUNTIME_PC_H
#include <stdint.h>
typedef struct { uint64_t **pc_chunk; long pc_nchunk; char **pc_name; long pc_nname; long pc_nslot; } g_pc_t;
#ifdef __cplusplus
extern "C" {
#endif
extern g_pc_t g_rt_pc;
#ifdef __cplusplus
}
#endif
#endif
