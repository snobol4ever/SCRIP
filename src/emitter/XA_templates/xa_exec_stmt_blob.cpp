/* xa_exec_stmt_blob.cpp — XA template for exec-stmt pattern blob.
 * SMX-4 (2026-05-30): the only reader of this blob was the SM-driven codegen_program /
 * xa_dispatch mode-4 path, deleted with the Stack Machine. The body referenced an SM_t
 * instruction (g_emit.instr); that field is gone. Neutered to emit nothing until x86
 * emission is rebuilt directly on the BB graph. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "xa_template_common.h"
#include "emit_bb.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void xa_exec_stmt_blob(void) { }
