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
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void xa_bb_ptr_slot(void) {
    const char * kind = x86_boxkind(); int id = g_flat_node_id++;
    char lbl[fmt_len(".L%s_rtc%d_z", kind, id)]; snprintf(lbl, sizeof lbl, ".L%s_rtc%d_z", kind, id);
    g_emit.bb_ptr_slot_lbl = lbl;
    bb_emit_x86(xa_bb_ptr_slot_text());
    g_emit.bb_ptr_slot_lbl = (const char *)0;
}
