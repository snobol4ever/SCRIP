#include <string>
#include <cstdio>
#include "emit.h"
#include "x86_asm.h"
extern "C" {
#include "xa_template_common.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_bb_ptr_slot_text(void) {
    return x86("directive", ".section .data")
         + x86("comment", std::string(g_emit.bb_ptr_slot_lbl) + ": .quad 0")
         + x86("directive", ".section .text")
         + x86("directive", ".intel_syntax noprefix");
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void xa_bb_ptr_slot(void) {
    snprintf(g_emit.bb_ptr_slot_lbl, sizeof g_emit.bb_ptr_slot_lbl, ".Lrtc%d_z", g_flat_node_id++);
    bb_emit_x86(xa_bb_ptr_slot_text());
}
