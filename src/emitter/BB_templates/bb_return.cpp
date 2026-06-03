#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "descr.h"
extern int g_descr_flat_chain;
int bb_slot_get(IR_t * nd);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_return_str() {
    if (!PLATFORM_X86) return std::string();
    if (!g_descr_flat_chain) return std::string();
    int src = (_.node && _.node->α) ? bb_slot_get(_.node->α) : -1;
    std::string head = IF(MEDIUM_TEXT, s_1asm(std::string(_.lbl_α) + ":")
                          + s_comment(emit_fmt("# BOX IR_RETURN [GN x86() stackless: ret-value slot %d -> proc frame result [r12+0]; jmp omega]", src)));
    if (src >= 0)
        return head
             + x86_frame_load64("rax", src)
             + x86_frame_load64("rdx", src + 8)
             + x86_frame_store64(0, "rax")
             + x86_frame_store64(8, "rdx")
             + x86("jmp", PORT_OMEGA);
    return head
         + x86_frame_mov_imm64(0, (long)DT_SNUL)
         + x86_frame_mov_imm64(8, 0L)
         + x86("jmp", PORT_OMEGA);
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_return(IR_t * pBB) {
    (void)pBB; x86_begin();
    std::string s = bb_return_str();
    if (s.empty()) { bb_emit_x86(x86_bomb("bb_return: unhandled (needs descr flat-chain)")); return; }
    bb_emit_x86(s);
}
