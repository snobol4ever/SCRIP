#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "descr.h"
extern int g_descr_flat_chain;
DESCR_t NV_SET_fn(const char * name, DESCR_t val);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_gvar_assign_icn_str() {
    if (!PLATFORM_X86) return std::string();
    if (!(g_descr_flat_chain && _.op_sa >= 0 && _.op_off >= 0)) return std::string();
    int rhs = _.op_sa;
    int off = _.op_off;
    const char * nm = _.op_sval ? _.op_sval : "";
    uint64_t fptr; { DESCR_t (*fp)(const char *, DESCR_t) = NV_SET_fn; fptr = (uint64_t)(uintptr_t)(void *)fp; }
    return IF(MEDIUM_TEXT, s_1asm(std::string(_.lbl_α) + ":")
                          + s_comment(emit_fmt("# BOX IR_ASSIGN global write(\"%s\") [GN-4 nv x86() stackless: NV_SET_fn(name, rhs slot %d) -> own slot %d; name sealed RO [rip+disp]]", nm, rhs, off)))
         + x86_frame_load64("rsi", rhs)
         + x86_frame_load64("rdx", rhs + 8)
         + x86_ro_load_q("rdi", 0)
         + x86("call", "NV_SET_fn", fptr)
         + x86_frame_store64(off, "rax")
         + x86_frame_store64(off + 8, "rdx")
         + x86("jmp", PORT_GAMMA)
         + x86("def", PORT_BETA)
         + x86("jmp", PORT_OMEGA)
         + x86_ro_seal_str(0, nm);
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_gvar_assign_icn(IR_t * pBB) {
    (void)pBB; x86_begin();
    std::string s = bb_gvar_assign_icn_str();
    if (s.empty()) { bb_emit_x86(x86_bomb("bb_gvar_assign_icn: unhandled (needs descr flat-chain + rhs slot + own slot)")); return; }
    bb_emit_x86(s);
}
