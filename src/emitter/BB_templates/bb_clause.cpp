#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_clause_str(IR_t * pBB) { (void)pBB; return std::string(); }
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_clause(IR_t * pBB) { bb_clause_str(pBB); }
