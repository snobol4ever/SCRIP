#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_galt() {
    x86_begin();
    if (!PLATFORM_X86 || _.op_off < 0 || !_.lbl_t0 || !_.lbl_t1 || !_.lbl_t0_p || !_.lbl_t1_p)
        return x86_alpha() + x86_bomb("bb_galt: IR_GALT missing dslot or arm labels");
    std::string dptr = "dword ptr [rsp + " + std::to_string(_.op_off) + "]";
    return x86("comment", "IR_GALT (grammar alternation): save delta to [rsp+dslot] at alpha; restore+jmp-arm2 at beta")
         + x86_alpha()
         + x86("mov", dptr, "r14d")
         + x86_jmp_lblptr(_.lbl_t0_p, _.lbl_t0)
         + x86_beta()
         + x86("mov", "r14d", dptr)
         + x86_jmp_lblptr(_.lbl_t1_p, _.lbl_t1)
         + x86_gamma()
         + x86_omega();
}
