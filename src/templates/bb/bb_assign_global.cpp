#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern int g_gva_active;
extern int g_monitor_bin;
DESCR_t NV_SET_fn(const char * name, DESCR_t val);
void comm_var(const char * name, DESCR_t val, const char * file, long line, long long stno);
const char * stmt_src_get_file(void);
}
#include "x86_asm.h"
#include <cstdio>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int stf() { return _.flat_stmt_frame; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int mon_vars_on() { static int v = -1; if (v < 0)
{ const char * e = getenv("SCRIP_MON_VARS"); v = (e && *e == (char)48) ? 0 : 1; } return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline std::string mon_var_trace_tap() {
    const char * srcfile = stmt_src_get_file();
    if (!srcfile) srcfile = "";
    std::string fl = LS(1);
    return x86("push", "rax") + x86("push", "rax") + x86("push", "rdi") + x86("push", "rsi")
         + x86("push", "rdx") + x86("push", "rcx") + x86("push", "r8") + x86("push", "r9")
         + x86("push", "r10") + x86("push", "r11")
         + x86("mov", "rsi", "rax")
         + x86("mov", "rdi", ROQ(0))
         + x86("directive", ".section .rodata")
         + x86("directive", (fl + ": .string \"" + srcfile + "\"").c_str())
         + x86("directive", ".section .text")
         + x86("directive", ".intel_syntax noprefix")
         + x86("lea", "rcx", "[rip + __]", (uint64_t)(uintptr_t)srcfile, fl.c_str())
         + x86("mov", "r8", (long)_.op_line)
         + x86("mov", "r9", (long)_.op_stno)
         + x86("call", "comm_var", (uint64_t)(uintptr_t)(void *)(void (*)(const char *, DESCR_t, const char *, long, long long))comm_var)
         + x86("pop", "r11") + x86("pop", "r10") + x86("pop", "r9") + x86("pop", "r8")
         + x86("pop", "rcx") + x86("pop", "rdx") + x86("pop", "rsi") + x86("pop", "rdi")
         + x86("pop", "rax") + x86("pop", "rax");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_assign_global() {
    if (!((_.op_zres || (_.op_a_slot >= 0 && _.op_off >= 0))))
        return x86_alpha()
             + x86_bomb((std::string("bb_assign_global: unhandled (needs descr flat-chain + rhs slot + own slot) var=") + (_.op_sval ? _.op_sval : "?")).c_str());
    if (_.op_zres)
        return IF(g_gva_active && _.op_gva_k >= 0,
                  x86("comment", "IR_ASSIGN gva zd")
                + x86_alpha()
                + x86("note", ZOPN(0))
                + x86("mov", "rax", ZOPQ(0, 0))
                + x86("note", ZOPN(0))
                + x86("mov", "rdx", ZOPQ(0, 8))
                + x86("note", gva_name(_.op_gva_k))
                + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(_.op_gva_k, 0) : ABSQ(RT_GVA_VA + _.op_gva_k * 16),     "rax")
                + x86("note", gva_name(_.op_gva_k))
                + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(_.op_gva_k, 8) : ABSQ(RT_GVA_VA + _.op_gva_k * 16 + 8), "rdx")
                + IF(mon_vars_on(),
                     x86("comment", "s196 MON-VARS, widened row snobol4-csnobol4-trace-builtin-and-ftrace-produce-zero-output: inline WRITE tap beside the GVA fast store -- now fires unconditionally (not only under --monitor) so TRACE()/&TRACE work in ordinary runs; comm_var itself stays self-guarded")
                   + mon_var_trace_tap())
                + x86_gamma()
                + x86_beta_trampoline()
                + IF(mon_vars_on(),
                     x86("def",    L(0))
                   + x86(".quad",  LS(0), _.op_sval)
                   + x86("label",  LS(0))
                   + x86(".string", _.op_sval)))
             + IF(!(g_gva_active && _.op_gva_k >= 0),
                  x86("comment", "IR_ASSIGN global zd")
                + x86_alpha()
                + x86("note", ZOPN(0))
                + x86("mov", "rax", ZOPQ(0, 0))
                + x86("note", ZOPN(0))
                + x86("mov", "rdx", ZOPQ(0, 8))
                + IF(mon_vars_on(),
                     x86("comment", "row snobol4-csnobol4-trace-builtin-and-ftrace-produce-zero-output: this non-GVA path had NO trace tap at all before (not even under --monitor); tap BEFORE the call so NV_SET_fn's return value in rax cannot be mistaken for it")
                   + mon_var_trace_tap())
                + x86("mov", "rsi", "rax")
                + x86("mov", "rdi", ROQ(0))
                + x86("call", "NV_SET_fn", (uint64_t)(uintptr_t)(void *)(DESCR_t (*)(const char *, DESCR_t))NV_SET_fn)
                + x86_gamma()
                + x86_beta_trampoline()
                + x86("def",    L(0))
                + x86(".quad",  LS(0), _.op_sval)
                + x86("label",  LS(0))
                + x86(".string", _.op_sval));
    return IF(g_gva_active && _.op_gva_k >= 0,
              x86("comment", "IR_ASSIGN gva")
            + x86_alpha()
            + x86("mov", "rax", FRQ(_.op_a_slot))
            + x86("mov", "rdx", FRQ(_.op_a_slot + 8))
            + x86("note", gva_name(_.op_gva_k))
            + x86("mov",    (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(_.op_gva_k, 0) : ABSQ(RT_GVA_VA + _.op_gva_k * 16),     "rax")
            + x86("note", gva_name(_.op_gva_k))
            + x86("mov",    (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(_.op_gva_k, 8) : ABSQ(RT_GVA_VA + _.op_gva_k * 16 + 8), "rdx")
            + IF(_.op_res_live && !stf(), x86("mov",    FRQ(_.op_off),     "rax")
                              + x86("mov",    FRQ(_.op_off + 8), "rdx"))
            + IF(mon_vars_on(),
                 x86("comment", "s196 MON-VARS, widened row snobol4-csnobol4-trace-builtin-and-ftrace-produce-zero-output: inline WRITE tap beside the GVA fast store -- now fires unconditionally (not only under --monitor) so TRACE()/&TRACE work in ordinary runs; comm_var itself stays self-guarded")
               + mon_var_trace_tap())
            + x86_gamma()
            + x86_beta_trampoline()
            + IF(mon_vars_on(),
                 x86("def",    L(0))
               + x86(".quad",  LS(0), _.op_sval)
               + x86("label",  LS(0))
               + x86(".string", _.op_sval)))
         + IF(!(g_gva_active && _.op_gva_k >= 0),
              x86("comment", "IR_ASSIGN global")
            + x86_alpha()
            + x86("mov", "rax", FRQ(_.op_a_slot))
            + x86("mov", "rdx", FRQ(_.op_a_slot + 8))
            + IF(mon_vars_on(),
                 x86("comment", "row snobol4-csnobol4-trace-builtin-and-ftrace-produce-zero-output: this non-GVA path had NO trace tap at all before (not even under --monitor); tap BEFORE the call so NV_SET_fn's return value in rax cannot be mistaken for it")
               + mon_var_trace_tap())
            + x86("mov", "rsi", "rax")
            + x86("mov",    "rdi", ROQ(0))
            + x86("rtcc_wb")
            + x86("call_bare",   "NV_SET_fn", (uint64_t)(uintptr_t)(void *)(DESCR_t (*)(const char *, DESCR_t))NV_SET_fn)
            + IF(_.op_res_live && !stf(), x86("mov",    FRQ(_.op_off),     "rax")
                              + x86("mov",    FRQ(_.op_off + 8), "rdx"))
            + x86("rtcc_rl")
            + x86_gamma()
            + x86_beta_trampoline()
            + x86("def",    L(0))
            + x86(".quad",  LS(0), _.op_sval)
            + x86("label",  LS(0))
            + x86(".string", _.op_sval));
}
