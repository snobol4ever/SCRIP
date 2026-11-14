#ifndef STAGE2_SCOPE_RESERVE_H
#define STAGE2_SCOPE_RESERVE_H
#include <stdint.h>
#include "ir/stage2_scopeent.h"
#include "ir/stage2_scope.h"
static inline void stage2_scope_reserve(Scope *sc, int need) {
    if (need > sc->cap) { int nc = sc->cap ? sc->cap * 2 : 8; while (nc < need) nc *= 2; sc->e = (ScopeEnt *)ct_grow(sc->e, (size_t)nc * sizeof(ScopeEnt)); sc->cap = nc; }
}
#endif
