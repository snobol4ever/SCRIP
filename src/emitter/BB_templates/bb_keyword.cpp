#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern int g_descr_flat_chain;
struct DESCR_t rt_icn_keyword_subject(void);
struct DESCR_t rt_icn_keyword_pos(void);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_keyword_str() {
    if (!PLATFORM_X86) return std::string();
    int off = _.op_off;
    if (!(g_descr_flat_chain && off >= 0)) return x86_bomb("bb_keyword: no slot");
    const char * kw = _.op_sval ? _.op_sval : "";
    if (kw[0] == '&') kw++;
    if (!strcmp(kw, "subject")) {
        struct DESCR_t (*fp)(void) = rt_icn_keyword_subject; uint64_t fptr = (uint64_t)(uintptr_t)(void *)fp;
        return IF(MEDIUM_TEXT, s_1asm(std::string(_.lbl_α) + ":")
                             + s_comment("# BOX ICN IR_KEYWORD &subject [x86() stackless: rt_icn_keyword_subject->slot->γ]"))
             + x86("call", "rt_icn_keyword_subject", fptr)
             + x86("mov", FRQ(off),     "rax")
             + x86("mov", FRQ(off + 8), "rdx")
             + x86("jmp", PORT_GAMMA)
             + x86("def", PORT_BETA)
             + x86("jmp", PORT_OMEGA);
    }
    if (!strcmp(kw, "pos")) {
        struct DESCR_t (*fp)(void) = rt_icn_keyword_pos; uint64_t fptr = (uint64_t)(uintptr_t)(void *)fp;
        return IF(MEDIUM_TEXT, s_1asm(std::string(_.lbl_α) + ":")
                             + s_comment("# BOX ICN IR_KEYWORD &pos [x86() stackless: rt_icn_keyword_pos->slot->γ]"))
             + x86("call", "rt_icn_keyword_pos", fptr)
             + x86("mov", FRQ(off),     "rax")
             + x86("mov", FRQ(off + 8), "rdx")
             + x86("jmp", PORT_GAMMA)
             + x86("def", PORT_BETA)
             + x86("jmp", PORT_OMEGA);
    }
    if (!strcmp(kw, "null")) {
        return IF(MEDIUM_TEXT, s_1asm(std::string(_.lbl_α) + ":")
                             + s_comment("# BOX ICN IR_KEYWORD &null [x86() stackless: {DT_SNUL,0}->slot->γ]"))
             + x86("mov", FRQ(off),     (long)DT_SNUL)
             + x86("mov", FRQ(off + 8), (long)0)
             + x86("jmp", PORT_GAMMA)
             + x86("def", PORT_BETA)
             + x86("jmp", PORT_OMEGA);
    }
    if (!strcmp(kw, "fail")) {
        return IF(MEDIUM_TEXT, s_1asm(std::string(_.lbl_α) + ":")
                             + s_comment("# BOX ICN IR_KEYWORD &fail [x86() stackless: ->ω]"))
             + x86("jmp", PORT_OMEGA)
             + x86("def", PORT_BETA)
             + x86("jmp", PORT_OMEGA);
    }
    return x86_bomb("bb_keyword: unsupported keyword");
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_keyword(IR_t * pBB) { (void)pBB; x86_begin(); bb_emit_x86(bb_keyword_str()); }
