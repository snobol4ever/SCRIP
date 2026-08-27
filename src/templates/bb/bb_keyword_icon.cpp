#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern int g_scan_regs_live;
struct DESCR_t rt_keyword_subject(void);
struct DESCR_t rt_keyword_pos(void);
DESCR_t rt_keyword_read(const char *sval);
DESCR_t rt_keyword_gen(const char *sval, long idx);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_keyword_icon() {
    if (!(_.op_off >= 0) && !_.op_zres) return x86_alpha() + x86_bomb("bb_keyword: no slot");
    const char *kw = !_.op_sval ? "" : (_.op_sval[0] == '&' ? _.op_sval + 1 : _.op_sval);
    if (_.op_zres) {
        if (!strcmp(kw, "subject")) {
            if (g_scan_regs_live)
                return x86("comment", "KEYWORD_subject_reg->ZRES (ZK-2)")
                     + x86_alpha()
                     + x86("mov", ZRES(0), (long)DT_S)
                     + x86("note", ZRESN()) + x86("mov", ZRES(8), "r13")
                     + x86_gamma() + x86_beta_trampoline();
            return x86("comment", "KEYWORD_subject_call->ZRES (ZK-2)")
                 + x86_alpha()
                 + x86("call", "rt_keyword_subject", (uint64_t)(uintptr_t)(void *)rt_keyword_subject)
                 + x86("note", ZRESN()) + x86("mov", ZRES(0), "rax")
                 + x86("note", ZRESN()) + x86("mov", ZRES(8), "rdx")
                 + x86_gamma() + x86_beta_trampoline();
        }
        if (!strcmp(kw, "pos"))
            return x86("comment", "KEYWORD_pos->ZRES (ZK-2) [r14+1: scan-reg truth]")
                 + x86_alpha()
                 + x86("mov", ZRES(0), (long)DT_I)
                 + x86("mov", "rax", "r14")
                 + x86("add", "rax", (long)1)
                 + x86("note", ZRESN()) + x86("mov", ZRES(8), "rax")
                 + x86_gamma() + x86_beta_trampoline();
        if (!strcmp(kw, "null"))
            return x86("comment", "KEYWORD_null->ZRES (ZK-2)")
                 + x86_alpha()
                 + x86("mov", ZRES(0), (long)DT_SNUL)
                 + x86("mov", ZRES(8), (long)0)
                 + x86_gamma() + x86_beta_trampoline();
        return x86_alpha() + x86_bomb("bb_keyword_icon: unhandled keyword in ZD arm");
    }
    std::string tail = x86_gamma() + x86_beta() + x86_omega();
    if (!strcmp(kw, "subject")) {
        if (g_scan_regs_live)
            return x86("comment", "KEYWORD_subject_reg")
                 + x86_alpha()
                 + x86("mov", FRQ(_.op_off),     (long)DT_S)
                 + x86("mov", FRQ(_.op_off + 8), "r13")
                 + tail;
        return x86("comment", "KEYWORD_subject_call")
             + x86_alpha()
             + x86("call", "rt_keyword_subject", (uint64_t)(uintptr_t)(void *)rt_keyword_subject)
             + x86("mov", FRQ(_.op_off),     "rax")
             + x86("mov", FRQ(_.op_off + 8), "rdx")
             + tail;
    }
    if (!strcmp(kw, "pos")) {
        return x86("comment", "KEYWORD_pos_reg [always r14+1: register cursor is the source of truth, incl. ?-less scanning callees]")
             + x86_alpha()
             + x86("mov", FRQ(_.op_off), (long)DT_I)
             + x86("mov", "rax", "r14")
             + x86("add", "rax", (long)1)
             + x86("mov", FRQ(_.op_off + 8), "rax")
             + tail;
    }
    if (!strcmp(kw, "null"))
        return x86("comment", "KEYWORD_null")
             + x86_alpha()
             + x86("mov", FRQ(_.op_off),     (long)DT_SNUL)
             + x86("mov", FRQ(_.op_off + 8), (long)0)
             + tail;
    if (!strcmp(kw, "fail"))
        return x86("comment", "KEYWORD_fail")
             + x86_alpha()
             + x86_omega()
             + x86_beta()
             + x86_omega();
    if (!strcmp(kw, "features") || !strcmp(kw, "regions") || !strcmp(kw, "storage") || !strcmp(kw, "collections") || !strcmp(kw, "allocated"))
        return x86("comment", "KEYWORD_gen")
             + x86_alpha()
             + x86("mov",     FRQ(_.op_off + 16), (long)0)
             + x86("def",     L(1))
             + x86("mov",     "rdi", ROQ(0))
             + x86("mov",     "rsi", FRQ(_.op_off + 16))
             + x86("call",    "rt_keyword_gen", (uint64_t)(uintptr_t)(void *)rt_keyword_gen)
             + x86("cmp",     "al", (long)DT_FAIL)
             + x86_omega("je")
             + x86("mov",     FRQ(_.op_off),     "rax")
             + x86("mov",     FRQ(_.op_off + 8), "rdx")
             + x86("mov",     "rax", FRQ(_.op_off + 16))
             + x86("add",     "rax", (long)1)
             + x86("mov",     FRQ(_.op_off + 16), "rax")
             + x86_gamma()
             + x86_beta()
             + x86("jmp",     L(1))
             + x86("def",     L(0))
             + x86(".quad",   LS(0), _.op_sval)
             + x86("label",   LS(0))
             + x86(".string", _.op_sval);
    return x86("comment", "KEYWORD_read")
         + x86_alpha()
         + x86("mov",     "rdi", ROQ(0))
         + x86("call",    "rt_keyword_read", (uint64_t)(uintptr_t)(void *)rt_keyword_read)
         + x86("cmp",     "al", (long)DT_FAIL)
         + x86_omega("je")
         + x86("mov",     FRQ(_.op_off),     "rax")
         + x86("mov",     FRQ(_.op_off + 8), "rdx")
         + tail
         + x86("def",     L(0))
         + x86(".quad",   LS(0), _.op_sval)
         + x86("label",   LS(0))
         + x86(".string", _.op_sval);
}
