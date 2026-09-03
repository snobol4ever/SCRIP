#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
#include "../runtime/builtins/gen.h"
DESCR_t rt_num_arith(DESCR_t a, DESCR_t b, int op);
void    rt_pl_tr_unwind(void *);
void    rt_pl_disj_open(void *, void *);
int     rt_jct_relop(DESCR_t lhs, DESCR_t rhs, int op);
int64_t to_int(DESCR_t v);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string to_trail_mark() {
    if (!x86_fb_pinned()) return std::string();
    int kt = g_emit.flat_frame_bytes;
    uint64_t fp; { void (*f)(void *, void *) = rt_pl_disj_open; fp = (uint64_t)(uintptr_t)(void *)f; }
    return x86("comment", "PL GENERATOR CHOICE (rung 7, ARCH sec B.13 (i) as ruled): bank the trail top as this generator's mark AND open the choice through the same rtx helper the rung-3 disjunction uses, so the values this box hands out are loggable and its redo has something to undo.")
         + x86("mov", FRQ(_.op_off + 24), "r12")
         + x86("lea", "rdi", RDQ(x86_fb(), kt - 64))
         + x86("mov", "rsi", x86_fb())
         + x86("call_bare", "rt_pl_disj_open", fp);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string to_trail_unwind() {
    if (!x86_fb_pinned()) return std::string();
    uint64_t fp; { void (*f)(void *) = rt_pl_tr_unwind; fp = (uint64_t)(uintptr_t)(void *)f; }
    return x86("comment", "PL GENERATOR STEP (rung 7, ARCH sec B.13): a solution was refused, so undo everything it bound back to this box's mark before the cursor advances -- the same rtx helper the rung-2 clause step and the rung-3 disjunction step use.")
         + x86("mov", "rdi", FRQ(_.op_off + 24))
         + x86("call_bare", "rt_pl_tr_unwind", fp);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_to() {
    x86_begin();
    if (x86_fb_pinned() && (_.op_zres || _.op_num_real))
        return x86_alpha() + x86_bomb("IR_TO: a Prolog generator reached the zd/real arm, where op_off+24 is to.limit and the rung-7 trail mark would alias the loop bound -- grant IR_TO a fourth word before enabling this path");
        if (_.op_zres)
            return x86("comment", "IR_TO zd")
                 + x86_alpha()
                 + x86("note",  ZOPN(0))
                 + x86("mov", "rdi", ZOPQ(0, 0))
                 + x86("note",  ZOPN(0))
                 + x86("mov", "rsi", ZOPQ(0, 8))
                 + x86("call",  "to_int", (uint64_t)(uintptr_t)(void*)to_int)
                 + x86("mov",   FRQ(_.op_off + 16), "rax")
                 + x86("note",  ZOPN(1))
                 + x86("mov", "rdi", ZOPQ(1, 0))
                 + x86("note",  ZOPN(1))
                 + x86("mov", "rsi", ZOPQ(1, 8))
                 + x86("call",  "to_int", (uint64_t)(uintptr_t)(void*)to_int)
                 + x86("mov",   FRQ(_.op_off + 24), "rax")
                 + x86("def",   L(0))
                 + x86("mov",   "rax",   FRQ(_.op_off + 16))
                 + x86("mov",   "rcx",   FRQ(_.op_off + 24))
                 + x86("cmp",   "rax",   "rcx")
                 + x86_omega(  "jg")
                 + x86("note",  ZRESN())
                 + x86("mov", ZRES(0),  (long)DT_I)
                 + x86("note",  ZRESN())
                 + x86("mov", ZRES(8),  "rax")
                 + x86_gamma()
                 + x86_beta()
                 + x86("inc",   FRQ(_.op_off + 16))
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
             + to_trail_mark()
             + x86("def",     L(0))
             + x86("mov",     "rax", FRQ(_.op_off + 16))
             + x86("mov",     "rcx", FRQ(_.op_sb + 8))
             + x86("cmp",     "rax", "rcx")
             + x86_omega("jg")
             + x86("mov",     FRQ(_.op_off),     (long)DT_I)
             + x86("mov",     FRQ(_.op_off + 8), "rax")
             + x86_gamma()
             + x86_beta()
             + to_trail_unwind()
             + x86("inc",     FRQ(_.op_off + 16))
             + x86("jmp",     L(0));
}
