/* bb_stub.c — catch-all grouped BB template for unfilled generic BB ops.
   Per HQ Invariant #10 (grouped templates): the 42 generic BB ops that share an
   identical emit shape — an honest no-op stub across all five backends — are
   handled by this single template fn.
   Ops (42): BB_VAR / ASSIGN / AUGOP / BINOP / UNOP / CALL / SEQ / SUCCEED /
     GOTO / RETURN / IF / ALTERNATE / TO_BY / EVERY / WHILE / UNTIL / REPEAT /
     ALT / SIZE / CASE / LIMIT / SUSPEND / PROC / SCAN / NONNULL / INTERROGATE /
     NOT / PAT_CALLOUT / BREAK / NEXT / IDENTICAL / NULL_TEST / RANDOM / NEG /
     POS / BINOP_GEN / SWAP / SEQ_EXPR / INITIAL / IF / SCAN.
   These kinds are handled today by the AST/runtime path in src/lower/bb_exec.c
   and src/lower/lower_*.c; this template slot keeps the BB layer total over
   BB_op_t so future native-codegen work has a place to land. Phase B splits
   individual ops back out when their arms get real bodies. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_stub_str(IR_t * pBB) { (void)pBB; return std::string(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_stub(IR_t * pBB) { bb_stub_str(pBB); }
