#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
extern "C" void * rt_zcol_push(void ** ptr_cell, int * cap_cell, int i, long elem_sz);
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_arbno() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    return _.op_off < 0
             ? x86_alpha() + x86_bomb("IR_MATCH_ARBNO: cursor slot not granted (zls)")
         : (int)_.op_phase == 0
             ? x86("comment", "IR_MATCH_ARBNO gen")
             + x86_alpha()
             + x86("mov", FR(_.op_off), "r14d")
             + x86("mov", FR(_.op_off + 4), "r14d")
             + x86_gamma()
             + x86_beta()
             + x86("mov", "r14d", FR(_.op_off + 4))
             + x86("mov", FR(_.op_off + 8), "r14d")
             + x86_omega()
         : (int)_.op_phase == 1
             ? x86("comment", "IR_MATCH_ARBNO ok")
             + x86_alpha()
             + x86("mov", "eax", FR(_.op_off + 8))
             + x86("cmp", "r14d", "eax")
             + x86("je",  L(0))
             + x86("mov", FR(_.op_off + 4), "r14d")
             + x86_gamma()
             + x86("def", L(0))
             + x86_omega()
         : (int)_.op_phase == 2
             ? x86("comment", "IR_MATCH_ARBNO exhaust")
             + x86_alpha()
             + x86_beta()
             + x86("mov", "r14d", FR(_.op_off))
             + x86("def", L(9))
             + x86_omega()
         : (_.op_sa < 0 || _.op_sb <= 0)
             ? x86_alpha() + x86_bomb("IR_MATCH_ARBNO v2: COLLECTION geometry not staged (zls_arbno_geom)")
         : (int)_.op_phase == 3
             ? x86("comment", "IR_MATCH_ARBNO2 gen")
             + x86_alpha()
             + x86("mov", FR(_.op_off), "r14d")
             + x86("mov", FR(_.op_off + 4), "r14d")
             + x86("mov", FR(_.op_off + 8), 0L)
             + x86_gamma()
             + x86_beta()
             + x86("mov", "r14d", FR(_.op_off + 4))
             + x86("lea", "rdi", FR(_.op_off + 16))
             + x86("lea", "rsi", FR(_.op_off + 12))
             + x86("mov", "edx", FR(_.op_off + 8))
             + x86("mov", "rcx", (long)_.op_sb)
             + x86_align_enter()
             + x86("call", "rt_zcol_push", (uint64_t)(uintptr_t)(void *)(void * (*)(void **, int *, int, long))rt_zcol_push)
             + x86_align_leave()
             + x86("mov", RDQ("rax", 0), x86_zr())
             + x86("mov", "ecx", "r14d")
             + x86("mov", RDQ("rax", 8), "rcx")
             + x86("lea", x86_zr(), RDQ("rax", 16 - _.op_sa))
             + x86_omega()
         : (int)_.op_phase == 4
             ? x86("comment", "IR_MATCH_ARBNO2 ok")
             + x86_alpha()
             + x86("mov", "eax", FR(_.op_sa - 8))
             + x86("mov", "rdx", FRQ(_.op_sa - 16))
             + x86("cmp", "r14d", "eax")
             + x86("je",  L(0))
             + x86("mov", x86_zr(), "rdx")
             + x86("mov", "eax", FR(_.op_off + 8))
             + x86("add", "eax", 1L)
             + x86("mov", FR(_.op_off + 8), "eax")
             + x86("mov", FR(_.op_off + 4), "r14d")
             + x86_gamma()
             + x86("def", L(0))
             + x86("mov", x86_zr(), "rdx")
             + x86_omega()
         : x86("comment", "IR_MATCH_ARBNO2 pop/exhaust")
             + x86_alpha()
             + x86("jmp", L(1))
             + x86_beta()
             + x86("mov", "rdx", FRQ(_.op_sa - 16))
             + x86("mov", x86_zr(), "rdx")
             + x86("mov", "eax", FR(_.op_off + 8))
             + x86("test", "eax", "eax")
             + x86("jz",  L(2))
             + x86("sub", "eax", 1L)
             + x86("mov", FR(_.op_off + 8), "eax")
             + x86("def", L(1))
             + x86("mov", "rax", FRQ(_.op_off + 16))
             + x86("mov", "ecx", FR(_.op_off + 8))
             + x86("mov", "rdx", (long)_.op_sb)
             + x86("imul", "rcx", "rdx")
             + x86("add", "rax", "rcx")
             + x86("lea", x86_zr(), RDQ("rax", 16 - _.op_sa))
             + x86_gamma()
             + x86("def", L(2))
             + x86("mov", "r14d", FR(_.op_off))
             + x86_omega();
}
