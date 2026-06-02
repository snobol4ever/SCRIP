#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_cset_str(IR_t * pBB) { (void)pBB; return std::string(); }
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_cset(IR_t * pBB) { bb_cset_str(pBB); }
