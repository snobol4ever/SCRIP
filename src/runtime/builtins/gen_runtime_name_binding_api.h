#ifndef GEN_RUNTIME_NAME_BINDING_API_H
#define GEN_RUNTIME_NAME_BINDING_API_H
#include "ir/stage2_scope.h"
#include "ir/stage2_procentry.h"
#ifdef __cplusplus
extern "C" {
#endif
int scope_add(Scope *sc, const char *name);
int scope_get(Scope *sc, const char *name);
#ifdef __cplusplus
}
#endif
#endif
