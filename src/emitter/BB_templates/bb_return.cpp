#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern int g_descr_flat_chain;
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_return() {
    x86_begin();
    if (!PLATFORM_X86 || !g_descr_flat_chain) return x86_bomb("bb_return: unhandled (needs descr flat-chain)");
    return IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                         + x86("comment", "BOX IR_RETURN [GN x86() stackless: ret-value slot " + std::to_string(_.op_sa) + " -> proc frame result [r12+0]; jmp omega]"))
         + IF(_.op_sa >= 0,
               x86_frame_load64("rax", _.op_sa)
             + x86_frame_load64("rdx", _.op_sa + 8)
             + x86_frame_store64(0, "rax")
             + x86_frame_store64(8, "rdx"))
         + IF(_.op_sa < 0,
               x86_frame_mov_imm64(0, (long)DT_SNUL)
             + x86_frame_mov_imm64(8, 0L))
         + x86("jmp", "ω");
}