#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
extern DESCR_t rt_num_arith_strict(DESCR_t, DESCR_t, int);
#include "bb_template_common.h"
#include "descr.h"
#include "../runtime/builtins/gen.h"
DESCR_t rt_num_arith(DESCR_t a, DESCR_t b, int op);
int     rt_jct_relop(DESCR_t lhs, DESCR_t rhs, int op);
int64_t to_int(DESCR_t v);
int64_t core_icn_to_int_check(uint64_t lo, uint64_t hi);
int     core_icn_int_operand_ok(uint64_t lo, uint64_t hi);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string xa_to_trail_mark();
std::string xa_to_trail_unwind();
std::string xa_to_int_operand_guard(int slot);
std::string xa_to_db_load_store();
std::string xa_to_db_slot_addr();
#define TO_BOMB1() (x86_fb_pinned() && (_.op_zres || _.op_num_real))
static std::string bb_to_db() {
    return x86("comment", "IR_TO db (ARCH-PROLOG-C-OUT-OF-THE-BOX 5.2 B): the packet's slots in packet order, each visible at the generation G this enumeration took at alpha; "
                          "the store is reloaded from its root cell at every step (a collected object moves)")
         + x86_alpha()
         + x86("mov", "rax", FRQ(_.op_sa + 8))
         + x86_shift_imm("shl", 4, "rax", 3)
         + x86("mov", "rcx", "r14")
         + x86("sub", "rcx", "rax")
         + x86("mov", "rax", RDQ("rcx", -24))
         + x86("test", "rax", "rax")
         + x86_omega("je")
         + x86("mov", "r9", RDQ("rax", 0))
         + x86("mov", "r8d", RDD("rax", 20))
         + x86("mov", FRQ(_.op_off + 32), "r8")
         + x86("mov", "esi", RDD("rax", 24))
         + x86("movsxd", "rsi", "esi")
         + x86("mov", FRQ(_.op_off + 16), "rsi")
         + xa_to_trail_mark()
         + x86("def", L(0))
         + x86("mov", "rsi", FRQ(_.op_off + 16))
         + x86("cmp", "esi", 0L)
         + x86_omega("jl")
         + xa_to_db_load_store() + xa_to_db_slot_addr() + x86("mov", "r8", FRQ(_.op_off + 32))
         + x86("mov", "r11d", RDD("r10", 20))
         + x86("cmp", "r11d", "r8d")
         + x86("jge", L(3))
         + x86("mov", "r11d", RDD("r10", 16))
         + x86("test", "r11d", "r11d")
         + x86("je", L(1))
         + x86("cmp", "r11d", "r8d")
         + x86("jl", L(3))
         + x86("def", L(1))
         + x86("mov", FRQ(_.op_off), (long)DT_I)
         + x86("mov", FRQ(_.op_off + 8), "rsi")
         + x86_gamma()
         + x86_beta()
         + xa_to_trail_unwind()
         + x86("mov", "rsi", FRQ(_.op_off + 16)) + xa_to_db_load_store() + xa_to_db_slot_addr()
         + x86("def", L(3))
         + x86("mov", "esi", RDD("r10", 32))
         + x86("movsxd", "rsi", "esi")
         + x86("mov", FRQ(_.op_off + 16), "rsi")
         + x86("jmp", L(0));
}
std::string bb_to() {
    x86_begin();
    return IF(_.op_db_walk,
               (x86_fb_pinned() && _.op_off >= 0 && _.op_sa >= 0)
                   ? bb_to_db()
                   : x86_alpha()
                   + x86_bomb("IR_TO db: a packet enumeration outside a pinned Prolog frame or without static operands"))
         + IF(!_.op_db_walk && TO_BOMB1(),
               x86_alpha()
                   + x86_bomb("IR_TO: a Prolog generator reached the zd/real arm, where op_off+24 is to.limit and the rung-7 trail mark "
                       "would alias the loop bound -- grant IR_TO a fourth word before enabling this path"))
         + IF(!_.op_db_walk && !TO_BOMB1() && _.op_zres,
               x86("comment", "IR_TO zd")
                 + x86_alpha()
                 + x86("note",  ZOPN(0))
                 + x86("mov", "rdi", ZOPQ(0, 0))
                 + x86("note",  ZOPN(0))
                 + x86("mov", "rsi", ZOPQ(0, 8))
                 + x86("call",  _.op_range_int_operands ? "core_icn_to_int_check" : "to_int",
                     _.op_range_int_operands ? (uint64_t)(uintptr_t)(void*)core_icn_to_int_check : (uint64_t)(uintptr_t)(void*)to_int)
                 + x86("mov",   FRQ(_.op_off + 16), "rax")
                 + x86_rt_gc_poll()
                 + x86("note",  ZOPN(1))
                 + x86("mov", "rdi", ZOPQ(1, 0))
                 + x86("note",  ZOPN(1))
                 + x86("mov", "rsi", ZOPQ(1, 8))
                 + x86("call",  _.op_range_int_operands ? "core_icn_to_int_check" : "to_int",
                     _.op_range_int_operands ? (uint64_t)(uintptr_t)(void*)core_icn_to_int_check : (uint64_t)(uintptr_t)(void*)to_int)
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
                 + x86_rt_gc_poll()
                 + x86_gamma()
                 + x86_beta()
                 + x86("inc",   FRQ(_.op_off + 16))
                 + x86_omega(  "jo")
                 + x86("jmp",   L(0)))
         + IF(!_.op_db_walk && !TO_BOMB1() && !_.op_zres,
               !(_.op_off >= 0 && _.op_sa >= 0 && _.op_sb >= 0) ? x86_alpha()
               + x86_bomb("bb_to: unhandled (needs static operands, descr flat-chain)") :
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
             + x86_rt_gc_poll()
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
             + x86("call",    (_.op_strict ? "rt_num_arith_strict" : "rt_num_arith"), (uint64_t)(uintptr_t)(void *)(_.op_strict ? rt_num_arith_strict : rt_num_arith))
             + x86("mov",     FRQ(_.op_off + 16), "rax")
             + x86("mov",     FRQ(_.op_off + 24), "rdx")
             + x86_rt_gc_poll()
             + x86("jmp",     L(10))
             + x86("def",     L(0))
             + x86(".quad",   (uint64_t)(int64_t)DT_R)
             + x86("def",     L(1))
             + x86(".quad",   (uint64_t)(int64_t)1) :
               x86("comment", "IR_TO")
             + x86_alpha()
             + xa_to_int_operand_guard(_.op_sa)
             + x86("mov",     "rdi", FRQ(_.op_sa))
             + x86("mov",     "rsi", FRQ(_.op_sa + 8))
             + x86("call", _.op_range_int_operands ? "core_icn_to_int_check" : "to_int",
                 _.op_range_int_operands ? (uint64_t)(uintptr_t)(void*)core_icn_to_int_check : (uint64_t)(uintptr_t)(void*)to_int)
             + x86("mov",     FRQ(_.op_sa),     (long)DT_I)
             + x86("mov",     FRQ(_.op_sa + 8), "rax")
             + x86_rt_gc_poll()
             + xa_to_int_operand_guard(_.op_sb)
             + x86("mov",     "rdi", FRQ(_.op_sb))
             + x86("mov",     "rsi", FRQ(_.op_sb + 8))
             + x86("call", _.op_range_int_operands ? "core_icn_to_int_check" : "to_int",
                 _.op_range_int_operands ? (uint64_t)(uintptr_t)(void*)core_icn_to_int_check : (uint64_t)(uintptr_t)(void*)to_int)
             + x86("mov",     FRQ(_.op_sb),     (long)DT_I)
             + x86("mov",     FRQ(_.op_sb + 8), "rax")
             + x86_rt_gc_poll()
             + x86("mov",     "rax", FRQ(_.op_sa + 8))
             + x86("mov",     FRQ(_.op_off + 16), "rax")
             + xa_to_trail_mark()
             + x86("def",     L(0))
             + x86("mov",     "rax", FRQ(_.op_off + 16))
             + x86("mov",     "rcx", FRQ(_.op_sb + 8))
             + x86("cmp",     "rax", "rcx")
             + x86_omega("jg")
             + x86("mov",     FRQ(_.op_off),     (long)DT_I)
             + x86("mov",     FRQ(_.op_off + 8), "rax")
             + x86_gamma()
             + x86_beta()
             + xa_to_trail_unwind()
             + x86("inc",     FRQ(_.op_off + 16))
             + x86_omega("jo")
             + x86("jmp",     L(0)));
}
