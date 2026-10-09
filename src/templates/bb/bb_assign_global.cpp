#include <string>
#include <cstdint>
#include "emit.h"
#include "bb_templates.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern int g_gva_active;
extern int g_monitor_bin;
DESCR_t NV_SET_fn(const char * name, DESCR_t val);
const char * stmt_src_get_file(void);
}
#include "x86_asm.h"
#include <cstdio>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define stf() (_.flat_stmt_frame)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int mon_vars_on() { return x86_trace_hooks_on(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define MVT_B 110
#define MVT_LAND() ( \
      x86("mov", "rax", RDQ("rsp", 80)) \
    + x86("mov", RDQ("rsp", 184), "rax") \
    + x86("mov", "rax", RDQ("rsp", 88)) \
    + x86("mov", RDQ("rsp", 152), "rax"))
static inline std::string mon_var_trace_tap() {
    return x86("push", "rax")
         + x86("push", "rax")
         + x86("push", "rdi")
         + x86("push", "rsi")
         + x86("push", "rdx")
         + x86("push", "rcx")
         + x86("push", "r8")
         + x86("push", "r9")
         + x86("push", "r10")
         + x86("push", "r11")
         + x86("sub", "rsp", 112L)
         + x86("mov", RDQ("rsp", 0), 0L)
         + x86("mov", "rsi", "rax")
         + x86("mov", "rdi", ROQ(0))
         + x86("mov", "rcx", (long)_.op_stno)
         + x86("mov", "r8", "rsp")
         + x86("call", "comm_var_open", (uint64_t)(uintptr_t)(void *)comm_var_open)
         + bb_glue_trace_pend_run(MVT_B, MVT_LAND())
         + x86("add", "rsp", 112L)
         + x86("pop", "r11")
         + x86("pop", "r10")
         + x86("pop", "r9")
         + x86("pop", "r8")
         + x86("pop", "rcx")
         + x86("pop", "rdx")
         + x86("pop", "rsi")
         + x86("pop", "rdi")
         + x86("pop", "rax")
         + x86("pop", "rax") + x86_rt_gc_poll_res();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define AG_HAS_SLOT() (_.op_zres || (_.op_a_slot >= 0 && _.op_off >= 0))
std::string bb_assign_global() {
    return IF(!AG_HAS_SLOT(),
               x86_alpha()
             + x86_bomb((std::string("bb_assign_global: unhandled (needs descr flat-chain + rhs slot + own slot) var=") + (_.op_sval ? _.op_sval : "?")).c_str()))
         + IF(AG_HAS_SLOT() && _.op_zres,
               IF(g_gva_active && _.op_gva_k >= 0,
                  x86("comment", "IR_ASSIGN gva zd")
                + x86_alpha()
                + x86("note", ZOPN(0))
                + x86("mov", "rax", ZOPQ(0, 0))
                + x86("note", ZOPN(0))
                + x86("mov", "rdx", ZOPQ(0, 8))
                + x86("note", gva_note(_.op_gva_k))
                + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(_.op_gva_k, 0) : ABSQ(RT_GVA_VA + _.op_gva_k * 16), "rax")
                + x86("note", gva_note(_.op_gva_k))
                + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(_.op_gva_k, 8) : ABSQ(RT_GVA_VA + _.op_gva_k * 16 + 8), "rdx")
                + IF(mon_vars_on(),
                      mon_var_trace_tap())
                + x86_gamma()
                + x86_beta_trampoline()
                + IF(mon_vars_on(),
                     x86("def", L(0))
                   + x86(".quad", LS(0), _.op_sval)
                   + x86("label", LS(0))
                   + x86(".string", _.op_sval)))
             + IF(!(g_gva_active && _.op_gva_k >= 0),
                  x86("comment", "IR_ASSIGN global zd")
                + x86_alpha()
                + x86("note", ZOPN(0))
                + x86("mov", "rax", ZOPQ(0, 0))
                + x86("note", ZOPN(0))
                + x86("mov", "rdx", ZOPQ(0, 8))
                + IF(mon_vars_on(),
                      mon_var_trace_tap())
                + x86("mov", "rsi", "rax")
                + x86("mov", "rdi", ROQ(0))
                + x86("call", "NV_SET_fn", (uint64_t)(uintptr_t)(void *)(DESCR_t (*)(const char *, DESCR_t))NV_SET_fn)
                + x86_rt_gc_poll_rec_res()
                + x86_gamma()
                + x86_beta_trampoline()
                + x86("def", L(0))
                + x86(".quad", LS(0), _.op_sval)
                + x86("label", LS(0))
                + x86(".string", _.op_sval)))
         + IF(AG_HAS_SLOT() && !_.op_zres,
               IF(g_gva_active && _.op_gva_k >= 0,
              x86("comment", "IR_ASSIGN gva")
            + x86_alpha()
            + x86("mov", "rax", FRQ(_.op_a_slot))
            + x86("mov", "rdx", FRQ(_.op_a_slot + 8))
            + x86("note", gva_note(_.op_gva_k))
            + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(_.op_gva_k, 0) : ABSQ(RT_GVA_VA + _.op_gva_k * 16), "rax")
            + x86("note", gva_note(_.op_gva_k))
            + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(_.op_gva_k, 8) : ABSQ(RT_GVA_VA + _.op_gva_k * 16 + 8), "rdx")
            + IF(_.op_res_live && !stf(), x86("mov", FRQ(_.op_off), "rax")
                              + x86("mov", FRQ(_.op_off + 8), "rdx"))
            + IF(mon_vars_on(),
                  mon_var_trace_tap())
            + x86_gamma()
            + x86_beta_trampoline()
            + IF(mon_vars_on(),
                 x86("def", L(0))
               + x86(".quad", LS(0), _.op_sval)
               + x86("label", LS(0))
               + x86(".string", _.op_sval)))
         + IF(!(g_gva_active && _.op_gva_k >= 0),
              x86("comment", "IR_ASSIGN global")
            + x86_alpha()
            + x86("mov", "rax", FRQ(_.op_a_slot))
            + x86("mov", "rdx", FRQ(_.op_a_slot + 8))
            + IF(mon_vars_on(),
                  mon_var_trace_tap())
            + x86("mov", "rsi", "rax")
            + x86("mov", "rdi", ROQ(0))
            + x86("rtcc_wb")
            + x86("call_bare", "NV_SET_fn", (uint64_t)(uintptr_t)(void *)(DESCR_t (*)(const char *, DESCR_t))NV_SET_fn)
            + IF(_.op_res_live && !stf(), x86("mov", FRQ(_.op_off), "rax")
                              + x86("mov", FRQ(_.op_off + 8), "rdx"))
                              + x86_rt_gc_poll()
            + x86("rtcc_rl")
            + x86_gamma()
            + x86_beta_trampoline()
            + x86("def", L(0))
            + x86(".quad", LS(0), _.op_sval)
            + x86("label", LS(0))
            + x86(".string", _.op_sval)));
}
