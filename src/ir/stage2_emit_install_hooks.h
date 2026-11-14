#ifndef STAGE2_EMIT_INSTALL_HOOKS_H
#define STAGE2_EMIT_INSTALL_HOOKS_H
#include <stdint.h>
typedef struct { void (*before)(void *ctx, int pi, int bb_idx); void (*after)(void *ctx, int pi, int bb_idx, void *fn); void *ctx; } emit_install_hooks_t;
#endif
