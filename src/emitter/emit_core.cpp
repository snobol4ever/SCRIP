/* emit_core.cpp — C++ wrapper for emit_core.c
 * 
 * This file exists only to allow the build system to compile with g++ (C++)
 * while keeping emit_core.c as pure C. This wrapper includes emit_core.c
 * within extern "C" scope, ensuring all symbols have C linkage (no mangling).
 * 
 * CPP-3 strategy: during the C→C++ migration, keep helpers at C linkage so
 * C template files (SM_templates/*.c, XA_templates/*.c) can call them unchanged.
 * Once templates are migrated to .cpp, this wrapper becomes unnecessary. */
/* g++ 13 fix (TOOLCHAIN-MIGRATION): pre-include every header that transitively pulls
 * the C++ standard library (<string>) at C++ linkage, BEFORE the extern "C" block.
 * Their include guards then neutralize the nested includes reached from emit_core.c,
 * so no C++ stdlib template is ever parsed under C linkage. emit_core.c itself uses
 * zero std:: — it only needs these headers' C-linkage declarations, which the guards
 * still expose on the second (no-op) include. */
#ifdef __cplusplus
#include "emit_io.h"
#include "emit_str.h"
#include "emit_str_builders.h"
#endif
#ifdef __cplusplus
extern "C" {
#endif
#include "driver/scrip_sm.h"
#include "emit_core.c"
#ifdef __cplusplus
}
#endif
