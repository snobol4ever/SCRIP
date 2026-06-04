/* bb_clause.c — grouped BB template for the Prolog BB family.
   Per HQ Invariant #10 (grouped templates): the ten Prolog BB ops
   (BB_ARITH / ATOM / BUILTIN / CALL / CHOICE / UNIFY / CUT / VAR / ALT / SEQ)
   share an identical emit shape — an honest no-op stub across all five backends.
   No frontend lowers a Prolog BB graph to native today; Prolog execution is
   handled at runtime by bb_exec_node in src/lower/bb_exec.c. The dispatch switch
   in emit_core.c already discriminates the opcode; every Prolog arm routes here
   and the emitted text is identical (empty), so no per-op branch is needed.
   Phase B will fill these arms when frontends start emitting Prolog BB graphs as
   native code. This template slot keeps the BB layer total over BB_op_t. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_clause_str(BB_t * pBB) { (void)pBB; return std::string(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_clause(BB_t * pBB) { bb_clause_str(pBB); }
