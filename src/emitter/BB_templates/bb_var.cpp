#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "descr.h"
DESCR_t NV_GET_fn(const char * name);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_var_str() {
    if (!PLATFORM_X86) return std::string();
    if (g_gvar_flat_chain && _.op_off >= 0 && _.op_sval && _.op_sval[0] != '&') {
        uint64_t fptr; { DESCR_t (*fp)(const char *) = NV_GET_fn; fptr = (uint64_t)(uintptr_t)(void *)fp; }
        return IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                             + x86("comment", emit_fmt("BOX IR_VAR \"%s\" [gvar flat-chain: NV_GET -> slot %d]", _.op_sval, _.op_off)))
             + x86_ro_load_q("rdi", 0)
             + x86("call", "NV_GET_fn", fptr)
             + x86_frame_store64(_.op_off, "rax")
             + x86_frame_store64(_.op_off + 8, "rdx")
             + x86("jmp",  PORT_GAMMA)
             + x86("def",  PORT_BETA)
             + x86("jmp",  PORT_OMEGA)
             + x86_ro_seal_str(0, _.op_sval);
    }
    if (g_gvar_flat_chain)
        return IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                             + x86("comment", emit_fmt("BOX IR_VAR \"%s\" [gvar flat-chain: pass-through]", _.op_sval ? _.op_sval : "")))
             + x86("jmp",  PORT_GAMMA)
             + x86("def",  PORT_BETA)
             + x86("jmp",  PORT_OMEGA);
    if (g_descr_flat_chain && _.op_off >= 0 && _.op_sa >= 0)
        return IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                             + x86("comment", emit_fmt("BOX IR_VAR read(\"%s\") [descr flat-chain: var slot %d -> own slot %d]", _.op_sval ? _.op_sval : "", _.op_sa, _.op_off)))
             + x86_frame_load64("rax", _.op_sa)
             + x86_frame_store64(_.op_off, "rax")
             + x86_frame_load64("rax", _.op_sa + 8)
             + x86_frame_store64(_.op_off + 8, "rax")
             + x86("jmp",  PORT_GAMMA)
             + x86("def",  PORT_BETA)
             + x86("jmp",  PORT_OMEGA);
    return x86_bomb("bb_var: unhandled arm (no flat-chain mode or missing slot)");
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_var(IR_t * pBB) { (void)pBB; x86_begin(); bb_emit_x86(bb_var_str()); }
