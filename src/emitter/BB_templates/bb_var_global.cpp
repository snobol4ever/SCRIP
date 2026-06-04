#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "descr.h"
extern int g_descr_flat_chain;
DESCR_t NV_GET_fn(const char * name);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_var_global_str() {
    if (!PLATFORM_X86) return std::string();
    if (!(g_descr_flat_chain && _.op_off >= 0)) return std::string();
    int off = _.op_off;
    const char * nm = _.op_sval ? _.op_sval : "";
    uint64_t fptr; { DESCR_t (*fp)(const char *) = NV_GET_fn; fptr = (uint64_t)(uintptr_t)(void *)fp; }
    return IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                          + x86("comment", emit_fmt("BOX IR_VAR global read(\"%s\") [GN-3 x86() stackless: NV_GET_fn -> own slot %d; name sealed RO [rip+disp]]", nm, off)))
         + x86_ro_load_q("rdi", 0)
         + x86("call", "NV_GET_fn", fptr)
         + x86_frame_store64(off, "rax")
         + x86_frame_store64(off + 8, "rdx")
         + x86("jmp", PORT_GAMMA)
         + x86("def", PORT_BETA)
         + x86("jmp", PORT_OMEGA)
         + x86_ro_seal_str(0, nm);
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_var_global(IR_t * pBB) {
    (void)pBB; x86_begin();
    std::string s = bb_var_global_str();
    if (s.empty()) { bb_emit_x86(x86_bomb("bb_var_global: unhandled (needs descr flat-chain + own slot)")); return; }
    bb_emit_x86(s);
}
