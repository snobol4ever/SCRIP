#include <string>
#include <cstdint>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
extern int g_gvar_flat_chain;
#include "emit.h"
#include "descr.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string fr_load64(const char * dst, const char * base, int disp) {
    int g = x86_rnum(dst), b = x86_rnum(base);
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = 0x48; if (g >= 8) rex |= 0x04; if (b >= 8) rex |= 0x01; c += (char)rex; c += (char)0x8B; c += (char)(0x80 | ((g & 7) << 3) | (b & 7)); c += u32le((uint32_t)disp);
        return x86_Lrec(c);
    }
    return std::string(" mov ") + dst + ", qword ptr [" + base + " + " + std::to_string(disp) + "]\n";
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_var_frame_ref_str(IR_t * pBB) {
    if (!PLATFORM_X86) return std::string();
    if (!(g_gvar_flat_chain && _.op_off >= 0)) return x86_bomb("bb_var_frame_ref: needs gvar flat-chain + own slot");
    int hops = (int) pBB->dval;
    int voff = 16 + (int) pBB->ival * 16;
    std::string s = IF(MEDIUM_TEXT, s_1asm(std::string(_.lbl_α) + ":")
                                  + s_comment(emit_fmt("# BOX IR_VAR_FRAME_REF \"%s\" slot=%d hops=%d deref -> [r12+%d]", _.op_sval ? _.op_sval : "", (int) pBB->ival, hops, _.op_off)));
    s += x86_frame_lea("rax", 0);
    for (int h = 0; h < hops; h++) s += fr_load64("rax", "rax", 0);
    s += fr_load64("rax", "rax", voff + 8);
    s += fr_load64("rcx", "rax", 0) + x86_frame_store64(_.op_off, "rcx");
    s += fr_load64("rcx", "rax", 8) + x86_frame_store64(_.op_off + 8, "rcx");
    s += x86("jmp", PORT_GAMMA)
       + x86("def", PORT_BETA)
       + x86("jmp", PORT_OMEGA);
    return s;
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_var_frame_ref(IR_t * pBB) { bb_emit_x86(bb_var_frame_ref_str(pBB)); }
