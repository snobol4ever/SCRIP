#ifndef STAGE2_SCRIPMODULE_H
#define STAGE2_SCRIPMODULE_H
#include <stdint.h>
#include "ir/ast.h"
typedef struct ScripModule { const char *name; const tree_t *first; const tree_t *last; int nstmts; int core_label_start; int core_label_count; int proc_start; int nprocs; } ScripModule;
#endif
