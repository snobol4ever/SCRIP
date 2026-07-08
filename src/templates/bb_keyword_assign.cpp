#include <string>
#include <stdint.h>
#include <string.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern int g_scan_regs_live;
int64_t rt_cvpos_pos(struct DESCR_t v, int64_t len);
struct DESCR_t rt_keyword_pos_set(struct DESCR_t v);
struct DESCR_t rt_keyword_random_set(struct DESCR_t v);
struct DESCR_t rt_keyword_error_set(struct DESCR_t v);
struct DESCR_t rt_keyword_trace_set(struct DESCR_t v);
struct DESCR_t rt_keyword_dump_set(struct DESCR_t v);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_keyword_assign() {
    if (!PLATFORM_X86) return std::string();
    if (!(_.op_off >= 0)) return x86_alpha() + x86_bomb("bb_keyword_assign: no result slot");
    if (_.op_a_slot < 0) return x86_alpha() + x86_bomb("bb_keyword_assign: rhs operand slot unresolved");
    const char *kw = !_.op_sval ? "" : (_.op_sval[0] == '&' ? _.op_sval + 1 : _.op_sval);
    if (!strcmp(kw, "pos")) {
        if (g_scan_regs_live)
            return x86("comment", "BOX ICN IR_KEYWORD_ASSIGN pos reg [oasgn.r kywdpos: cvpos(v,Delta) fail->omega; delta=pos-1; result {DT_I,pos}]")
                 + x86_alpha()
                 + x86("mov",  "rdi", FRQ(_.op_a_slot))
                 + x86("mov",  "rsi", FRQ(_.op_a_slot + 8))
                 + x86("mov",  "rdx", "r15")
                 + x86("call", "rt_cvpos_pos", (uint64_t)(uintptr_t)(void *)rt_cvpos_pos)
                 + x86("cmp",  "rax", (long)0)
                 + x86_omega("je")
                 + x86("mov",  "r14", "rax")
                 + x86("sub",  "r14", (long)1)
                 + x86("mov",  FRQ(_.op_off),     (long)DT_I)
                 + x86("mov",  FRQ(_.op_off + 8), "rax")
                 + x86_gamma()
                 + x86_beta()
                 + x86_omega();
        return x86("comment", "BOX ICN IR_KEYWORD_ASSIGN pos call [non-scan: rt_keyword_pos_set cvpos vs strlen(scan_subj), set scan_pos, fail->omega]")
             + x86_alpha()
             + x86("mov",  "rdi", FRQ(_.op_a_slot))
             + x86("mov",  "rsi", FRQ(_.op_a_slot + 8))
             + x86("call", "rt_keyword_pos_set", (uint64_t)(uintptr_t)(void *)rt_keyword_pos_set)
             + x86("cmp",  "eax", (long)DT_FAIL)
             + x86_omega("je")
             + x86("mov",  FRQ(_.op_off),     "rax")
             + x86("mov",  FRQ(_.op_off + 8), "rdx")
             + x86_gamma()
             + x86_beta()
             + x86_omega();
    }
    if (!strcmp(kw, "random")) {
        return x86("comment", "BOX ICN IR_KEYWORD_ASSIGN random [data.r kywdint: coerce int, store g_random seed, result {DT_I,n}; non-numeric->omega]")
             + x86_alpha()
             + x86("mov",  "rdi", FRQ(_.op_a_slot))
             + x86("mov",  "rsi", FRQ(_.op_a_slot + 8))
             + x86("call", "rt_keyword_random_set", (uint64_t)(uintptr_t)(void *)rt_keyword_random_set)
             + x86("cmp",  "eax", (long)DT_FAIL)
             + x86_omega("je")
             + x86("mov",  FRQ(_.op_off),     "rax")
             + x86("mov",  FRQ(_.op_off + 8), "rdx")
             + x86_gamma()
             + x86_beta()
             + x86_omega();
    }
    if (!strcmp(kw, "error")) {
        return x86("comment", "BOX ICN IR_KEYWORD_ASSIGN error [coerce int, store g_error; non-numeric->omega]")
             + x86_alpha()
             + x86("mov",  "rdi", FRQ(_.op_a_slot))
             + x86("mov",  "rsi", FRQ(_.op_a_slot + 8))
             + x86("call", "rt_keyword_error_set", (uint64_t)(uintptr_t)(void *)rt_keyword_error_set)
             + x86("cmp",  "eax", (long)DT_FAIL)
             + x86_omega("je")
             + x86("mov",  FRQ(_.op_off),     "rax")
             + x86("mov",  FRQ(_.op_off + 8), "rdx")
             + x86_gamma()
             + x86_beta()
             + x86_omega();
    }
    if (!strcmp(kw, "trace")) {
        return x86("comment", "BOX ICN IR_KEYWORD_ASSIGN trace [coerce int, store g_trace; non-numeric->omega]")
             + x86_alpha()
             + x86("mov",  "rdi", FRQ(_.op_a_slot))
             + x86("mov",  "rsi", FRQ(_.op_a_slot + 8))
             + x86("call", "rt_keyword_trace_set", (uint64_t)(uintptr_t)(void *)rt_keyword_trace_set)
             + x86("cmp",  "eax", (long)DT_FAIL)
             + x86_omega("je")
             + x86("mov",  FRQ(_.op_off),     "rax")
             + x86("mov",  FRQ(_.op_off + 8), "rdx")
             + x86_gamma()
             + x86_beta()
             + x86_omega();
    }
    if (!strcmp(kw, "dump")) {
        return x86("comment", "BOX ICN IR_KEYWORD_ASSIGN dump [coerce int, store g_dump; non-numeric->omega]")
             + x86_alpha()
             + x86("mov",  "rdi", FRQ(_.op_a_slot))
             + x86("mov",  "rsi", FRQ(_.op_a_slot + 8))
             + x86("call", "rt_keyword_dump_set", (uint64_t)(uintptr_t)(void *)rt_keyword_dump_set)
             + x86("cmp",  "eax", (long)DT_FAIL)
             + x86_omega("je")
             + x86("mov",  FRQ(_.op_off),     "rax")
             + x86("mov",  FRQ(_.op_off + 8), "rdx")
             + x86_gamma()
             + x86_beta()
             + x86_omega();
    }
        return x86_alpha() + x86_bomb("bb_keyword_assign: only &pos/&random assignment implemented (KEYWORD-LVALUE rung; &subject/:=:/<-> are follow-ons)");
}
