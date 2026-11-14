#ifndef STAGE2_DRIVER_PROCS_API_H
#define STAGE2_DRIVER_PROCS_API_H
#include <stdint.h>
#include "ir/stage2.h"
#include "ir/stage2_emit_install_hooks.h"
#ifdef __cplusplus
extern "C" {
#endif
void driver_register_proc(stage2_t *s2, int pi);
void driver_proc_props(stage2_t *s2, int pi);
void *driver_install_proc(stage2_t *s2, int pi, const emit_install_hooks_t *hooks);
#ifdef __cplusplus
}
#endif
#endif
