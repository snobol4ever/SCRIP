#ifndef STAGE2_SCRIPMODULEREGISTRY_H
#define STAGE2_SCRIPMODULEREGISTRY_H
#include <stdint.h>
#include "ir/stage2_scripmodule.h"
typedef struct ScripModuleRegistry { ScripModule *mods; int cap; int nmod; int main_mod; } ScripModuleRegistry;
#endif
