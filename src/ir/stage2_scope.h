#ifndef STAGE2_SCOPE_H
#define STAGE2_SCOPE_H
#include <stdint.h>
#include "ir/stage2_scopeent.h"
typedef struct Scope { ScopeEnt *e; int n; int cap; } Scope;
#endif
