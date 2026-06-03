#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern int g_descr_flat_chain;
void rt_icn_scan_enter(uint64_t lo, uint64_t hi);
void rt_icn_scan_leave(void);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_gen_scan_str() {
    if (!PLATFORM_X86) return std::string();
    if (_.op_sb == 1) {
        int sa = _.op_sa;
        void (*fp)(uint64_t, uint64_t) = rt_icn_scan_enter; uint64_t fptr = (uint64_t)(uintptr_t)(void *)fp;
        return IF(MEDIUM_TEXT, s_comment("# BOX ICN IR_GEN_SCAN enter [x86() stackless: subject DESCR slot->rt_icn_scan_enter->body]"))
             + x86("mov", "rdi", FRQ(sa))
             + x86("mov", "rsi", FRQ(sa + 8))
             + x86("call", "rt_icn_scan_enter", fptr)
             + x86("jmp", PORT_GAMMA)
             + x86("def", PORT_BETA)
             + x86("jmp", PORT_OMEGA);
    }
    void (*fp)(void) = rt_icn_scan_leave; uint64_t fptr = (uint64_t)(uintptr_t)(void *)fp;
    return IF(MEDIUM_TEXT, s_comment("# BOX ICN IR_GEN_SCAN leave [x86() stackless: rt_icn_scan_leave->restore &subject/&pos]"))
         + x86("call", "rt_icn_scan_leave", fptr)
         + x86("jmp", PORT_GAMMA)
         + x86("def", PORT_BETA)
         + x86("jmp", PORT_OMEGA);
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_gen_scan(IR_t * pBB) { (void)pBB; x86_begin(); bb_emit_x86(bb_gen_scan_str()); }
