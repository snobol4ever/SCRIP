#include <string>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern int g_gva_active;
DESCR_t NV_GET_fn(const char * name);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline const char * vg_res(int w) { return _.op_zres ? ZRES(w) : FRQ(_.op_off + w); }
static inline int vg_patv_slot(const char * sv) {
    const char * d = sv ? strstr(sv, "$V") : 0;
    if (!d || strncmp(sv, "PAT$", 4)) return -1;
    char * e = 0;
    long k = strtol(d + 2, &e, 10);
    return (e && e != d + 2 && !*e) ? (int) k : -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_var_global() {
        const int vs = vg_patv_slot(_.op_sval);
        return !_.op_zres && _.op_off < 0 ? x86_alpha() + x86_bomb("bb_var_global: unhandled (needs descr flat-chain + own slot)")
             : vs >= 0 ?
               x86("comment", "IR_VAR patv-slot: the snapshot the pattern's constructor passed, read from its own header")
             + x86_alpha()
             + (_.op_head_spine && _.op_head_rsp < 0 ? x86_bomb("bb_var_global: a frameless thunk reads its head from a box with no spine depth") :
               x86("mov", "rdi", _.op_head_spine ? RDQ("rsp", _.op_head_rsp) : RDQ("rbp", -24)))
             + x86("test",   "rdi", "rdi")
             + x86("je",     L(1))
             + x86("mov",    "rsi", RDQ("rdi", 32))
             + x86("test",   "rsi", "rsi")
             + x86("je",     L(1))
             + x86("cmp",    RDQ("rdi", 40), (long) vs + 1)
             + x86("jl",     L(1))
             + x86("mov",    "rax", RDQ("rsi", vs * 16))
             + x86("mov",    "rdx", RDQ("rsi", vs * 16 + 8))
             + x86("jmp",    L(2))
             + x86("def",    L(1))
             + x86("xor",    "eax", "eax")
             + x86("xor",    "edx", "edx")
             + x86("def",    L(2))
             + x86("note", "result")
             + x86("mov",    vg_res(0), "rax")
             + x86("note", "result")
             + x86("mov",    vg_res(8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline()
             : g_gva_active && _.op_gva_k >= 0 ?
               x86("comment", "IR_VAR")
             + x86_alpha()
             + x86("note", gva_name(_.op_gva_k))
             + x86("mov",    "rax", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(_.op_gva_k, 0) : ABSQ(RT_GVA_VA + _.op_gva_k * 16))
             + x86("note", gva_name(_.op_gva_k))
             + x86("mov",    "rdx", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(_.op_gva_k, 8) : ABSQ(RT_GVA_VA + _.op_gva_k * 16 + 8))
             + x86("note", "result")
             + x86("mov",    vg_res(0), "rax")
             + x86("note", "result")
             + x86("mov",    vg_res(8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline()
             : x86("comment", "IR_VAR")
             + x86_alpha()
             + x86("mov",    "rdi", ROQ(0))
             + x86("call",   "NV_GET_fn", (uint64_t)(uintptr_t)(void *)NV_GET_fn)
             + x86("cmp",    "al", (long)DT_FAIL)
             + x86_omega("je")
             + x86("note", ZRESN())
             + x86("mov",    vg_res(0), "rax")
             + x86("note", ZRESN())
             + x86("mov",    vg_res(8), "rdx")
             + x86_rt_gc_poll()
             + x86_gamma()
             + x86_beta_trampoline()
             + x86("def",    L(0))
             + x86(".quad",  LS(0), _.op_sval)
             + x86("label",  LS(0))
             + x86(".string", _.op_sval);
}
