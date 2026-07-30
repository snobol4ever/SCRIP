#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern int g_gva_active;
DESCR_t NV_SET_fn(const char * name, DESCR_t val);
}
#include "x86_asm.h"
#include <cstdio>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline const char * rspq(int off) { static char b[8][40]; static int i; i = (i + 1) & 7; snprintf(b[i], 40, "qword ptr [rsp + %d]", off); return b[i]; }   /* ZB-VAL-0: the bb_match_capture rspd precedent, qword */
static inline int vfc() { return x86_port_mode() == ZC_PORT_FORTH && _.op_fc_disp >= 0; }   /* ZB-VAL-0: granted-lit operand arrives in an rsp cell */
static inline int stf() { return _.flat_stmt_frame; }   /* SUBJ-ARM-3 (the environ-smash root, this session): under the armed regime the graph carve is 8B, so the FRQ(op_off) flat result store lands 150-390B ABOVE the statement bracket -- argv/environ.  Regime protocol: the RESULT IS THE CELL -- the popped source cell is overwritten in place ([rsp+0/8], NO pop, net-zero rsp: the unop precedent the registry comments bless), reclaimed by the bracket leave for free; the flat store DIES under stf.  The !vfc-under-stf arm (chained-assign flat READ) is a separate pre-existing latent, noted not chased. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_assign_global() {
    if (!(PLATFORM_X86 && _.op_a_slot >= 0 && _.op_off >= 0))
        return x86_alpha()
             + x86_bomb((std::string("bb_assign_global: unhandled (needs descr flat-chain + rhs slot + own slot) var=") + (_.op_sval ? _.op_sval : "?")).c_str());
    return IF(g_gva_active && _.op_gva_k >= 0,
              x86("comment", "IR_ASSIGN gva")
            + x86_alpha()
            + IF(!vfc(), x86("mov", "rax", FRQ(_.op_a_slot)) + x86("mov", "rdx", FRQ(_.op_a_slot + 8)))
            + IF(vfc(),  x86("mov", "rax", rspq(_.op_fc_disp)) + x86("mov", "rdx", rspq(_.op_fc_disp + 8)) + IF(!stf(), x86("add", "rsp", (long)16)))
            + x86("mov",    ABSQ(RT_GVA_VA + _.op_gva_k * 16),     "rax")
            + x86("mov",    ABSQ(RT_GVA_VA + _.op_gva_k * 16 + 8), "rdx")
            + IF(vfc() && stf(), x86("mov", rspq(0), "rax") + x86("mov", rspq(8), "rdx"))
            + IF(_.op_res_live && !stf(), x86("mov",    FRQ(_.op_off),     "rax")
                              + x86("mov",    FRQ(_.op_off + 8), "rdx"))
            + x86_gamma()
            + x86_beta_trampoline())
         + IF(!(g_gva_active && _.op_gva_k >= 0),
              x86("comment", "IR_ASSIGN global")
            + x86_alpha()
            + IF(!vfc(), x86("mov", "rsi", FRQ(_.op_a_slot)) + x86("mov", "rdx", FRQ(_.op_a_slot + 8)))
            + IF(vfc(),  x86("mov", "rsi", rspq(_.op_fc_disp)) + x86("mov", "rdx", rspq(_.op_fc_disp + 8)) + IF(!stf(), x86("add", "rsp", (long)16)))
            + x86("mov",    "rdi", ROQ(0))
            + x86("call",   "NV_SET_fn", (uint64_t)(uintptr_t)(void *)(DESCR_t (*)(const char *, DESCR_t))NV_SET_fn)
            + IF(vfc() && stf(), x86("mov", rspq(0), "rax") + x86("mov", rspq(8), "rdx"))
            + IF(_.op_res_live && !stf(), x86("mov",    FRQ(_.op_off),     "rax")
                              + x86("mov",    FRQ(_.op_off + 8), "rdx"))
            + x86_gamma()
            + x86_beta_trampoline()
            + x86("def",    L(0))
            + x86(".quad",  LS(0), _.op_sval)
            + x86("label",  LS(0))
            + x86(".string", _.op_sval));
}
