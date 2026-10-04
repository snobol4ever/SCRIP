#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
void rt_pl_tr_refuse(const char *);
int rt_pl_unify_const_cold(const DESCR_t *, int64_t, int);
}
#include "rt/rt_pl_trail.h"
#include "x86_asm.h"
static_assert(X86_PL_TR_ENTRY_BYTES == PL_TR_ENTRY_BYTES && X86_PL_TR_ARENA_MASK == -(long)PL_TR_ARENA_BYTES, "the trail push spells the entry size and the arena mask as literals");
static_assert(PL_TR_FRAME_HI_OFF == 32 && offsetof(pl_tr_entry_t, cell) == 0 && offsetof(pl_tr_entry_t, pad) == 8 && offsetof(pl_tr_entry_t, old) == 16,
              "the trail push reads [B + 32] and writes {cell, pad, old}");
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "bb_pl_cell.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_unify_const() {
    x86_begin();
    return IF(_.op_zres,
               x86_alpha()
             + x86_bomb("bb_unify_const: a Prolog head is a flat-frame graph; no ZD arm exists")
             + x86_beta_trampoline())
         + IF(!_.op_zres && _.op_u_why != 0,
               x86_alpha()
             + x86_bomb("bb_unify_const: operands are a cell source (a named IR_VAR_REF with a frame slot, or a driven parent IR_UNIFY_STRUCT), a child index and a literal")
             + x86_beta_trampoline())
         + IF(!_.op_zres && _.op_u_why == 0,
               x86("comment", "IR_UNIFY_CONST: deref the cell, compare tag and value, or bind the constant (trail test and push inline)")
             + x86_alpha()
             + PL_SRC_RDI()
             + PL_DEREF(10, 11, 12, 13)
             + x86("cmp", "al", _.op_u_ktag)
             + x86("jne", L(20))
             + IF(!_.op_u_atom,
                   x86("mov", "ecx", RDD("rdi", 4))
                 + x86("test", "ecx", "ecx")
                 + x86("jne", L(40)))
             + x86_movabs_r64("rax", _.op_u_kval)
             + x86("mov", "rsi", RDQ("rdi", 8))
             + x86("cmp", "rsi", "rax")
             + x86_omega("jne")
             + x86_gamma()
             + x86("def", L(20))
             + x86("note", "unbound: the zero DESCR, DT_FAIL, a self PLVAR (the deref stopped on it) or a self name")
             + x86("cmp", "al", 0L)
             + x86("je", L(30))
             + x86("cmp", "al", (long)DT_FAIL)
             + x86("je", L(30))
             + x86("cmp", "al", (long)DT_PLVAR)
             + x86("je", L(30))
             + x86("cmp", "al", (long)DT_N)
             + x86("jne", L(25))
             + x86("mov", "rsi", RDQ("rdi", 8))
             + x86("cmp", "rsi", "rdi")
             + x86("je", L(30))
             + x86_omega()
             + x86("def", L(25))
             + IF(_.op_u_atom,
                   x86("cmp", "al", (long)DT_S)
                 + x86("je", L(40))
                 + x86_omega())
             + IF(!_.op_u_atom,
                   x86("cmp", "al", (long)DT_PLREF)
                 + x86_omega("je")
                 + x86("cmp", "al", (long)DT_PLATOM)
                 + x86_omega("je")
                 + x86("cmp", "al", (long)DT_S)
                 + x86_omega("je")
                 + x86("jmp", L(40)))
             + x86("def", L(30))
             + PL_TRAIL(31, 39)
             + x86("mov", RDQ("rdi", 0), _.op_u_ktag)
             + x86_movabs_r64("rax", _.op_u_kval)
             + x86("mov", RDQ("rdi", 8), "rax")
             + x86_gamma()
             + x86("def", L(40))
             + x86("note", "cold: a legacy DT_S atom text, or a number of another kind, compared strictly by class by the value service; it never binds and never allocates, so no poll follows (ARCH-PROLOG-C-OUT-OF-THE-BOX 10)")
             + x86_movabs_r64("rsi", _.op_u_kval)
             + x86("mov", "edx", (long)_.op_u_atom)
             + x86("call", "rt_pl_unify_const_cold", (uint64_t)(uintptr_t)(void *)rt_pl_unify_const_cold)
             + x86("test", "eax", "eax")
             + x86_omega("je")
             + x86_gamma()
             + x86("def", L(39))
             + x86("mov", "rdi", "r12")
             + x86("call", "rt_pl_tr_refuse", (uint64_t)(uintptr_t)(void *)rt_pl_tr_refuse)
             + x86_beta_trampoline());
}
