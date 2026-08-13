#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
#include "../runtime/builtins/gen.h"
DESCR_t rt_num_arith(DESCR_t a, DESCR_t b, int op);
int     rt_jct_relop(DESCR_t lhs, DESCR_t rhs, int op);
int64_t to_int(DESCR_t v);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_to() {
    x86_begin();
    if (PLATFORM_X86)
        if (_.op_zres)
            /* ZK-2 ZD ARM DESIGN: counter and limit stored in FRQ (___-relative, depth-immune) at op_off+16 and op_off+24 -- the SAME slots the flat arm uses.  FRQ is depth-immune under a pinned ___ (flat_lcl_proc procs have sub rsp,frame + mov ___,rsp at entry; x86_fb_pinned()=1 for all graphs that reach this arm via icn_cells_graph && x86_fb_pinned()).  This means counter/limit are correctly addressed at BOTH α (first-pass, before downstream carves) and β (resume, after downstream carves moved RSP further).  Operands lo/hi are read via ZOPQ (staged depth-difference from zd_plan), correct at α where RSP has only TO's own K=32 added.  ZRES(0/8) holds the current yield value as usual.  op_off must be valid (the same precondition the flat arm checks); the ZD arm only fires when op_zres=1 which requires the graph to be icn_cells_graph AND x86_fb_pinned() (the zd_wl_kind IR_VAR guard), guaranteeing op_off is a valid frame slot (lower_icon.c assigns it for every IR_TO in a flat_lcl_proc graph). */
            return x86("comment", "IR_TO zd")
                 + x86_alpha()
                 + x86("note",  ZOPN(0)) + x86("mov", "rdi", ZOPQ(0, 0))
                 + x86("note",  ZOPN(0)) + x86("mov", "rsi", ZOPQ(0, 8))
                 + x86("call",  "to_int", (uint64_t)(uintptr_t)(void*)to_int)
                 + x86("mov",   FRQ(_.op_off + 16), "rax")   /* counter init: FRQ=[___+op_off+16], depth-immune at α and β */
                 + x86("note",  ZOPN(1)) + x86("mov", "rdi", ZOPQ(1, 0))
                 + x86("note",  ZOPN(1)) + x86("mov", "rsi", ZOPQ(1, 8))
                 + x86("call",  "to_int", (uint64_t)(uintptr_t)(void*)to_int)
                 + x86("mov",   FRQ(_.op_off + 24), "rax")   /* limit init: FRQ=[___+op_off+24], depth-immune at α and β; mirrors flat arm layout */
                 + x86("def",   L(0))
                 + x86("mov",   "rax",   FRQ(_.op_off + 16)) /* read counter: depth-immune */
                 + x86("mov",   "rcx",   FRQ(_.op_off + 24)) /* read limit:   depth-immune */
                 + x86("cmp",   "rax",   "rcx")
                 + x86_omega(  "jg")
                 + x86("note",  ZRESN()) + x86("mov", ZRES(0),  (long)DT_I)
                 + x86("note",  ZRESN()) + x86("mov", ZRES(8),  "rax")
                 + x86_gamma()
                 + x86_beta()
                 + x86("inc",   FRQ(_.op_off + 16))          /* increment counter in-place: depth-immune */
                 + x86("jmp",   L(0));
        return !(_.op_off >= 0 && _.op_sa >= 0 && _.op_sb >= 0) ? x86_alpha() + x86_bomb("bb_to: unhandled (needs static operands, descr flat-chain)") :
               _.op_num_real ?
               x86("comment", "IR_TO")
             + x86_alpha()
             + x86("mov",     "rax", FRQ(_.op_sa))
             + x86("mov",     FRQ(_.op_off + 16), "rax")
             + x86("mov",     "rax", FRQ(_.op_sa + 8))
             + x86("mov",     FRQ(_.op_off + 24), "rax")
             + x86("def",     L(10))
             + x86("mov",     "rdi", FRQ(_.op_off + 16))
             + x86("mov",     "rsi", FRQ(_.op_off + 24))
             + x86("mov",     "rdx", FRQ(_.op_sb))
             + x86("mov",     "rcx", FRQ(_.op_sb + 8))
             + x86("mov",     "r8d", (long)BINOP_LE)
             + x86("call",    "rt_jct_relop", (uint64_t)(uintptr_t)(void*)rt_jct_relop)
             + x86("test",    "eax", "eax")
             + x86_omega("jz")
             + x86("mov",     "rax", FRQ(_.op_off + 16))
             + x86("mov",     FRQ(_.op_off),     "rax")
             + x86("mov",     "rax", FRQ(_.op_off + 24))
             + x86("mov",     FRQ(_.op_off + 8), "rax")
             + x86_gamma()
             + x86_beta()
             + x86("mov",     "rdi", FRQ(_.op_off + 16))
             + x86("mov",     "rsi", FRQ(_.op_off + 24))
             + x86("mov",     "rdx", ROQ(0))
             + x86("mov",     "rcx", ROQ(1))
             + x86("mov",     "r8d", (long)BINOP_ADD)
             + x86("call",    "rt_num_arith", (uint64_t)(uintptr_t)(void*)rt_num_arith)
             + x86("mov",     FRQ(_.op_off + 16), "rax")
             + x86("mov",     FRQ(_.op_off + 24), "rdx")
             + x86("jmp",     L(10))
             + x86("def",     L(0))
             + x86(".quad",   (uint64_t)(int64_t)DT_R)
             + x86("def",     L(1))
             + x86(".quad",   (uint64_t)(int64_t)1) :
               x86("comment", "IR_TO")
             + x86_alpha()
             + x86("mov",     "rdi", FRQ(_.op_sa))
             + x86("mov",     "rsi", FRQ(_.op_sa + 8))
             + x86("call",    "to_int", (uint64_t)(uintptr_t)(void*)to_int)
             + x86("mov",     FRQ(_.op_sa),     (long)DT_I)
             + x86("mov",     FRQ(_.op_sa + 8), "rax")
             + x86("mov",     "rdi", FRQ(_.op_sb))
             + x86("mov",     "rsi", FRQ(_.op_sb + 8))
             + x86("call",    "to_int", (uint64_t)(uintptr_t)(void*)to_int)
             + x86("mov",     FRQ(_.op_sb),     (long)DT_I)
             + x86("mov",     FRQ(_.op_sb + 8), "rax")
             + x86("mov",     "rax", FRQ(_.op_sa + 8))
             + x86("mov",     FRQ(_.op_off + 16), "rax")
             + x86("def",     L(0))
             + x86("mov",     "rax", FRQ(_.op_off + 16))
             + x86("mov",     "rcx", FRQ(_.op_sb + 8))
             + x86("cmp",     "rax", "rcx")
             + x86_omega("jg")
             + x86("mov",     FRQ(_.op_off),     (long)DT_I)
             + x86("mov",     FRQ(_.op_off + 8), "rax")
             + x86_gamma()
             + x86_beta()
             + x86("inc",     FRQ(_.op_off + 16))
             + x86("jmp",     L(0));
    return std::string();
}
