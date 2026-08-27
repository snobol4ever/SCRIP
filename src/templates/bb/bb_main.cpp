#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
#include "bb_templates.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_main_entry_bridge() {
    return x86("comment", "bb_main ENTRY BRIDGE: binary chain entry IS the buffer base (emit_chain returns (bb_box_fn)buf) — the hoisted ports must never become the entry point")
         + x86_jmp_ext(_.flat_main_body_p);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_main_β() {
    return x86_jmp_ext(_.flat_main_ω_p);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_main_floater(int kind) {
    return (kind == 3) ? bb_nreturn_mark() : bb_define();
}
