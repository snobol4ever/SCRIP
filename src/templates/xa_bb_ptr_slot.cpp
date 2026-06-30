#include <string>
#include "emit.h"
#include "x86_asm.h"
extern "C" {
#include "xa_template_common.h"
#include "../emitter/emit.h"
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string xa_bb_ptr_slot_str(void) {
    if (PLATFORM_X86) {
        if (MEDIUM_MACRO_DEF) return x86("comment", "# no macro form — XA_BB_PTR_SLOT");
        if (MEDIUM_BINARY)    return std::string();
        if (MEDIUM_TEXT) {
            return x86("directive", ".section .data")
                 + x86("comment", std::string(g_emit.bb_ptr_slot_lbl) + ": .quad 0")
                 + x86("directive", ".section .text")
                 + x86("directive", ".intel_syntax noprefix");
        }
    }
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void xa_bb_ptr_slot(void) {
    if (MEDIUM_TEXT) {
        std::string _lbl = emit_fmt(".Lrtc%d_z", g_flat_node_id++);
        strncpy(g_emit.bb_ptr_slot_lbl, _lbl.c_str(), 87);
        g_emit.bb_ptr_slot_lbl[87] = '\0';
    }
    auto s = xa_bb_ptr_slot_str();
    if (!s.empty()) emit_text_n(s.data(), s.size());
}
