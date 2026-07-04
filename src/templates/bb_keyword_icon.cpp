#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern int g_gvar_flat_chain;
extern int g_scan_regs_live;
struct DESCR_t rt_keyword_subject(void);
struct DESCR_t rt_keyword_pos(void);
DESCR_t rt_keyword_read(const char *sval);
DESCR_t rt_keyword_gen(const char *sval, long idx);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_keyword_icon() {
    if (!PLATFORM_X86) return std::string();
    if (!(_.op_off >= 0)) return x86_bomb("bb_keyword: no slot");
    const char *kw = !_.op_sval ? "" : (_.op_sval[0] == '&' ? _.op_sval + 1 : _.op_sval);
    std::string tail = x86("jmp", "γ") + x86("def", "β") + x86("jmp", "ω");
    if (!strcmp(kw, "subject")) {
        if (g_scan_regs_live)
            return x86("comment", "IR_KEYWORD_subject_reg")
                 + x86("label",   _.lbl_α)
                 + x86("mov", FRQ(_.op_off),     (long)DT_S)
                 + x86("mov", FRQ(_.op_off + 8), "r13")
                 + tail;
        return x86("comment", "IR_KEYWORD_subject_call")
             + x86("label",   _.lbl_α)
             + x86("call", "rt_keyword_subject", (uint64_t)(uintptr_t)(void *)rt_keyword_subject)
             + x86("mov", FRQ(_.op_off),     "rax")
             + x86("mov", FRQ(_.op_off + 8), "rdx")
             + tail;
    }
    if (!strcmp(kw, "pos")) {
        if (g_scan_regs_live)
            return x86("comment", "IR_KEYWORD_pos_reg")
                 + x86("label",   _.lbl_α)
                 + x86("mov", FRQ(_.op_off), (long)DT_I)
                 + x86("mov", "rax", "r14")
                 + x86("add", "rax", (long)1)
                 + x86("mov", FRQ(_.op_off + 8), "rax")
                 + tail;
        return x86("comment", "IR_KEYWORD_pos_call")
             + x86("label",   _.lbl_α)
             + x86("call", "rt_keyword_pos", (uint64_t)(uintptr_t)(void *)rt_keyword_pos)
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
    if (!strcmp(kw, "features") || !strcmp(kw, "regions") || !strcmp(kw, "storage") || !strcmp(kw, "collections"))
        return x86("comment", "IR_KEYWORD_gen")
             + x86("label",   _.lbl_α)
             + x86("mov",     FRQ(_.op_off + 16), (long)0)
             + x86("def",     L(1))
             + x86("mov",     "rdi", ROQ(0))
             + x86("mov",     "rsi", FRQ(_.op_off + 16))
             + x86("call",    "rt_keyword_gen", (uint64_t)(uintptr_t)(void *)rt_keyword_gen)
             + x86("cmp",     "eax", (long)DT_FAIL)
             + x86("je",      "ω")
             + x86("mov",     FRQ(_.op_off),     "rax")
             + x86("mov",     FRQ(_.op_off + 8), "rdx")
             + x86("mov",     "rax", FRQ(_.op_off + 16))
             + x86("add",     "rax", (long)1)
             + x86("mov",     FRQ(_.op_off + 16), "rax")
             + x86("jmp",     "γ")
             + x86("def",     "β")
             + x86("jmp",     L(1))
             + x86("def",     L(0))
             + x86(".quad",   LS(0), _.op_sval)
             + x86("label",   LS(0))
             + x86(".string", _.op_sval);
    return x86("comment", "IR_KEYWORD_read")
         + x86("label",   _.lbl_α)
         + x86("mov",     "rdi", ROQ(0))
         + x86("call",    "rt_keyword_read", (uint64_t)(uintptr_t)(void *)rt_keyword_read)
         + x86("mov",     FRQ(_.op_off),     "rax")
         + x86("mov",     FRQ(_.op_off + 8), "rdx")
         + tail
         + x86("def",     L(0))
         + x86(".quad",   LS(0), _.op_sval)
         + x86("label",   LS(0))
         + x86(".string", _.op_sval);
}
