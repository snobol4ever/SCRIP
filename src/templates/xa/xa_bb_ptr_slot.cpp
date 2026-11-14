#include <string>
#include <cstdio>
#include "emitter/emit_state_em_03_sh_api.h"
#include "emitter/emit_walk.h"
#include "templates/x86/x86_ops.h"
#include "ir/ct_arena.h"
extern "C" {
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_bb_ptr_slot_text(void) {
    return x86("directive", ".section .data")
         + x86("comment", std::string(g_emit.bb_ptr_slot_lbl) + ": .quad 0")
         + x86("directive", ".section .text")
         + x86("directive", ".intel_syntax noprefix");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void xa_bb_ptr_slot(void) {
    g_emit.bb_ptr_slot_lbl = ct_fmt(".L%s_rtc%d_z", x86_boxkind(), g_flat_node_id++);
    bb_emit_x86(xa_bb_ptr_slot_text());
}
