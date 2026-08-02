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
/* ZTOS-1 (Lon s21x-o "Do not put RSP references directly into the templates"): private raw-rsp helper RETIRED -- call sites now speak the sanctioned spine accessor ZTOS/ZTOSD (x86_asm.h), which adds op_zdepth so a box's own carve and its own TOS reads compose instead of colliding.  Byte-identical while this kind is unarmed (op_zdepth==0); correct once it is armed, which is what lets the _spine exclusion list retire. */

static inline int stf() { return _.flat_stmt_frame; }   /* SUBJ-ARM-3 (the environ-smash root, this session): under the armed regime the graph carve is 8B, so the FRQ(op_off) flat result store lands 150-390B ABOVE the statement bracket -- argv/environ.  Regime protocol: the RESULT IS THE CELL -- the popped source cell is overwritten in place ([rsp+0/8], NO pop, net-zero rsp: the unop precedent the registry comments bless), reclaimed by the bracket leave for free; the flat store DIES under stf.  The !vfc-under-stf arm (chained-assign flat READ) is a separate pre-existing latent, noted not chased. */
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
                + x86("note", gva_name(_.op_gva_k)) + x86("mov", ABSQ(RT_GVA_VA + _.op_gva_k * 16),     "rax")
                + x86("note", gva_name(_.op_gva_k)) + x86("mov", ABSQ(RT_GVA_VA + _.op_gva_k * 16 + 8), "rdx")
                + x86_gamma()
                + x86_beta_trampoline())
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
                + x86(".string", _.op_sval));   /* ZD-1 (Lon s21x-v): the assign is the statement's SINK -- K=0 (no result cell of its own; chained assigns decline at the planner), the value is the producer's suspended cell read at the staged difference (mode 3), and its gamma is where op_zgpop returns rsp to statement entry (the hook's statement-terminal release).  The flat op_res_live store dies here: a ZD statement HAS no flat readers by verdict. */
    return IF(g_gva_active && _.op_gva_k >= 0,
              x86("comment", "IR_ASSIGN gva")
            + x86_alpha()
            + x86("mov", "rax", FRQ(_.op_a_slot)) + x86("mov", "rdx", FRQ(_.op_a_slot + 8))
            + x86("note", gva_name(_.op_gva_k)) + x86("mov",    ABSQ(RT_GVA_VA + _.op_gva_k * 16),     "rax")
            + x86("note", gva_name(_.op_gva_k)) + x86("mov",    ABSQ(RT_GVA_VA + _.op_gva_k * 16 + 8), "rdx")
            + IF(_.op_res_live && !stf(), x86("mov",    FRQ(_.op_off),     "rax")
                              + x86("mov",    FRQ(_.op_off + 8), "rdx"))
            + x86_gamma()
            + x86_beta_trampoline())
         + IF(!(g_gva_active && _.op_gva_k >= 0),
              x86("comment", "IR_ASSIGN global")
            + x86_alpha()
            + x86("mov", "rsi", FRQ(_.op_a_slot)) + x86("mov", "rdx", FRQ(_.op_a_slot + 8))
            + x86("mov",    "rdi", ROQ(0))
            + x86("call",   "NV_SET_fn", (uint64_t)(uintptr_t)(void *)(DESCR_t (*)(const char *, DESCR_t))NV_SET_fn)
            + IF(_.op_res_live && !stf(), x86("mov",    FRQ(_.op_off),     "rax")
                              + x86("mov",    FRQ(_.op_off + 8), "rdx"))
            + x86_gamma()
            + x86_beta_trampoline()
            + x86("def",    L(0))
            + x86(".quad",  LS(0), _.op_sval)
            + x86("label",  LS(0))
            + x86(".string", _.op_sval));
}
