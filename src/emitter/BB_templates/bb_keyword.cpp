#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern int g_descr_flat_chain;
extern int g_icn_scan_regs_live;
struct DESCR_t rt_icn_keyword_subject(void);
struct DESCR_t rt_icn_keyword_pos(void);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_keyword() {
    if (!PLATFORM_X86) return std::string();
    if (!(g_descr_flat_chain && _.op_off >= 0)) return x86_bomb("bb_keyword: no slot");
    const char *kw = !_.op_sval ? "" : (_.op_sval[0] == '&' ? _.op_sval + 1 : _.op_sval);
    std::string tail = x86("jmp", "γ") + x86("def", "β") + x86("jmp", "ω");
    if (!strcmp(kw, "subject")) {
        if (g_icn_scan_regs_live)
            return x86("comment", "IR_KEYWORD_subject_reg")
                 + x86("label",   _.lbl_α)
                 + x86("mov", FRQ(_.op_off),     (long)DT_S)
                 + x86("mov", FRQ(_.op_off + 8), "r13")
                 + tail;
        return x86("comment", "IR_KEYWORD_subject_call")
             + x86("label",   _.lbl_α)
             + x86("call", "rt_icn_keyword_subject", (uint64_t)(uintptr_t)(void *)rt_icn_keyword_subject)
             + x86("mov", FRQ(_.op_off),     "rax")
             + x86("mov", FRQ(_.op_off + 8), "rdx")
             + tail;
    }
    if (!strcmp(kw, "pos")) {
        if (g_icn_scan_regs_live)
            return x86("comment", "IR_KEYWORD_pos_reg")
                 + x86("label",   _.lbl_α)
                 + x86("mov", FRQ(_.op_off), (long)DT_I)
                 + x86("mov", "rax", "r14")
                 + x86("add", "rax", (long)1)
                 + x86("mov", FRQ(_.op_off + 8), "rax")
                 + tail;
        return x86("comment", "IR_KEYWORD_pos_call")
             + x86("label",   _.lbl_α)
             + x86("call", "rt_icn_keyword_pos", (uint64_t)(uintptr_t)(void *)rt_icn_keyword_pos)
             + x86("mov", FRQ(_.op_off),     "rax")
             + x86("mov", FRQ(_.op_off + 8), "rdx")
             + tail;
    }
    if (!strcmp(kw, "null"))
        return x86("comment", "IR_KEYWORD_null")
             + x86("label",   _.lbl_α)
             + x86("mov", FRQ(_.op_off),     (long)DT_SNUL)
             + x86("mov", FRQ(_.op_off + 8), (long)0)
             + tail;
    if (!strcmp(kw, "fail"))
        return x86("comment", "IR_KEYWORD_fail")
             + x86("label",   _.lbl_α)
             + x86("jmp", "ω")
             + x86("def", "β")
             + x86("jmp", "ω");
    return x86_bomb("bb_keyword: unsupported keyword");
}
