#ifndef STAGE2_MODS_RESERVE_H
#define STAGE2_MODS_RESERVE_H
#include <stdint.h>
#include "ir/stage2_scripmodule.h"
#include "ir/stage2_scripmoduleregistry.h"
#include "ir/ct_arena.h"
static inline void stage2_mods_reserve(ScripModuleRegistry *r, int need) {
    if (need > r->cap) { int nc = r->cap ? r->cap * 2 : 8; while (nc < need) nc *= 2; r->mods = (ScripModule *)ct_grow(r->mods, (size_t)nc * sizeof(ScripModule)); r->cap = nc; }
}
#endif
