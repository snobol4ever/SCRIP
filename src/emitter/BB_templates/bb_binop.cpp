#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_binop_relop_str();
std::string bb_binop_arith_str();
std::string bb_binop_gvar_arith_str();
std::string bb_binop_concat_slot_str();
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_binop(IR_t * pBB) {
    (void)pBB;
    std::string s;
    s = bb_binop_relop_str();       if (!s.empty()) { bb_emit_x86(s); return; }
    s = bb_binop_arith_str();       if (!s.empty()) { bb_emit_x86(s); return; }
    s = bb_binop_gvar_arith_str();  if (!s.empty()) { bb_emit_x86(s); return; }
    s = bb_binop_concat_slot_str(); if (!s.empty()) { bb_emit_x86(s); return; }
    bb_emit_x86(x86_bomb("bb_binop: shape not yet converted (jct_relop/concat_lit/agpure are stubs)"));
}
