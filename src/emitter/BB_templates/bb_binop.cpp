/* bb_binop.cpp — IR_BINOP router. x86() self-encoding (template-revamp, 2026-06-02). The IR_BINOP
   kind has several distinct box shapes; each lives in its own one-box-one-file template, all pBB-free
   (they read g_emit scalars the driver deposits). This router calls each converted box's _str() in
   priority order and emits the first that produces bytes (empty = "does not apply, try next"). Boxes
   not yet converted are LOUD x86_bomb() — the router falls to the bomb when no converted arm fires. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_binop_relop_str();
std::string bb_binop_arith_str();
std::string bb_binop_concat_slot_str();
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_binop(IR_t * pBB) {
    (void)pBB;
    std::string s;
    s = bb_binop_relop_str();       if (!s.empty()) { bb_emit_x86(s); return; }
    s = bb_binop_arith_str();       if (!s.empty()) { bb_emit_x86(s); return; }
    s = bb_binop_concat_slot_str(); if (!s.empty()) { bb_emit_x86(s); return; }
    bb_emit_x86(x86_bomb("bb_binop: shape not yet converted (jct_relop/concat_lit/agpure are stubs)"));
}
