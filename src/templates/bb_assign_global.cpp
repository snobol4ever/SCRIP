#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern int g_gva_active;
extern int g_monitor_bin;
DESCR_t NV_SET_fn(const char * name, DESCR_t val);
void comm_var(const char * name, DESCR_t val);
}
#include "x86_asm.h"
#include <cstdio>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int stf() { return _.flat_stmt_frame; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int mon_vars_on() { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_MON_VARS"); v = (e && *e == (char)48) ? 0 : 1; } return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_assign_global() {
    if (!(PLATFORM_X86 && (_.op_zres || (_.op_a_slot >= 0 && _.op_off >= 0))))
        return x86_alpha()
             + x86_bomb((std::string("bb_assign_global: unhandled (needs descr flat-chain + rhs slot + own slot) var=") + (_.op_sval ? _.op_sval : "?")).c_str());
    if (_.op_zres)
        return IF(g_gva_active && _.op_gva_k >= 0,
                  x86("comment", "IR_ASSIGN gva zd")
                + x86_alpha()
                + x86("note", ZOPN(0)) + x86("mov", "rax", ZOPQ(0, 0))
                + x86("note", ZOPN(0)) + x86("mov", "rdx", ZOPQ(0, 8))
                + x86("note", gva_name(_.op_gva_k)) + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(_.op_gva_k, 0) : ABSQ(RT_GVA_VA + _.op_gva_k * 16),     "rax")
                + x86("note", gva_name(_.op_gva_k)) + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(_.op_gva_k, 8) : ABSQ(RT_GVA_VA + _.op_gva_k * 16 + 8), "rdx")
                
                + IF(g_monitor_bin && mon_vars_on(),
                     x86("comment", "s196 MON-VARS (Lon in-chat): inline WRITE tap beside the GVA fast store -- comm_var is self-guarded (no-op unless a monitor/trace is live); shared codegen means --monitor bakes this into m4 binaries too")
                   + x86("push", "rax")
                   + x86("push", "rdx")
                   + x86("mov", "rsi", "rax")
                   + x86("mov", "rdi", ROQ(0))
                   + x86("call", "comm_var", (uint64_t)(uintptr_t)(void *)(void (*)(const char *, DESCR_t))comm_var)
                   + x86("pop", "rdx")
                   + x86("pop", "rax"))
                + x86_gamma()
                + x86_beta_trampoline()
                + IF(g_monitor_bin && mon_vars_on(),
                     x86("def",    L(0))
                   + x86(".quad",  LS(0), _.op_sval)
                   + x86("label",  LS(0))
                   + x86(".string", _.op_sval)))
             + IF(!(g_gva_active && _.op_gva_k >= 0),
                  x86("comment", "IR_ASSIGN global zd")
                + x86_alpha()
                + x86("note", ZOPN(0)) + x86("mov", "rsi", ZOPQ(0, 0))
                + x86("note", ZOPN(0)) + x86("mov", "rdx", ZOPQ(0, 8))
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
            + x86("mov", "rax", FRQ(_.op_a_slot)) + x86("mov", "rdx", FRQ(_.op_a_slot + 8))
            + x86("note", gva_name(_.op_gva_k)) + x86("mov",    (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(_.op_gva_k, 0) : ABSQ(RT_GVA_VA + _.op_gva_k * 16),     "rax")
            + x86("note", gva_name(_.op_gva_k)) + x86("mov",    (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(_.op_gva_k, 8) : ABSQ(RT_GVA_VA + _.op_gva_k * 16 + 8), "rdx")
            + IF(_.op_res_live && !stf(), x86("mov",    FRQ(_.op_off),     "rax")
                              + x86("mov",    FRQ(_.op_off + 8), "rdx"))
            
            + IF(g_monitor_bin && mon_vars_on(),
                 x86("comment", "s196 MON-VARS (Lon in-chat): inline WRITE tap beside the GVA fast store -- comm_var is self-guarded (no-op unless a monitor/trace is live); shared codegen means --monitor bakes this into m4 binaries too")
               + x86("push", "rax")
               + x86("push", "rdx")
               + x86("mov", "rsi", "rax")
               + x86("mov", "rdi", ROQ(0))
               + x86("call", "comm_var", (uint64_t)(uintptr_t)(void *)(void (*)(const char *, DESCR_t))comm_var)
               + x86("pop", "rdx")
               + x86("pop", "rax"))
            + x86_gamma()
            + x86_beta_trampoline()
            + IF(g_monitor_bin && mon_vars_on(),
                 x86("def",    L(0))
               + x86(".quad",  LS(0), _.op_sval)
               + x86("label",  LS(0))
               + x86(".string", _.op_sval)))
         + IF(!(g_gva_active && _.op_gva_k >= 0),
              x86("comment", "IR_ASSIGN global")
            + x86_alpha()
            + x86("mov", "rsi", FRQ(_.op_a_slot)) + x86("mov", "rdx", FRQ(_.op_a_slot + 8))
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
