#include <string>
#include <cstring>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
}
#include "x86_asm.h"
extern "C" int rt_unify_terms(void *l, void *r);
extern "C" int rt_pl_unify_cell_const(void *cell_term, int kind, long ival, const char *sval);
extern "C" int rt_pl_unify_cell_float(void *cell_term, double dval);
extern "C" int rt_pl_unify_struct_gz(void *frame, const void *lnd, const void *rnd);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bcu_sh() { return (int)_.op_parts_ival[0]; }
static double bcu_fv() {
    double d; memcpy(&d, &_.op_parts_ival[2], 8); return d;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_cell_unify_str() {
    x86_begin();
    if (PLATFORM_X86) return x86("label", _.lbl_α)
         + x86("comment", "IR_CELL_UNIFY")
         + IF(bcu_sh() == 0,
           x86("mov", "rdi", "r12")
         + x86("mov", "rsi", ROQ(0))
         + x86("mov", "rdx", ROQ(1))
         + x86("call", "rt_pl_unify_struct_gz", (uint64_t)(uintptr_t)(void *)rt_pl_unify_struct_gz)
         + x86("test", "eax", "eax")
         + x86("je", "ω")
         + x86("jmp", "γ")
         + x86("def", "β")
         + x86("jmp", "ω")
         + x86("def", L(0))
         + x86(".quad", (long)_.op_parts_ival[1])
         + x86("def", L(1))
         + x86(".quad", (long)_.op_parts_ival[2]))
         + IF(bcu_sh() == 1 || bcu_sh() == 5,
           x86("jmp", "γ")
         + x86("def", "β")
         + x86("jmp", "ω"))
         + IF(bcu_sh() == 2,
           x86("mov", "rdi", FRQ(GZ_CELL_OFF((int)_.op_parts_ival[1])))
         + x86("mov", "rsi", FRQ(GZ_CELL_OFF((int)_.op_parts_ival[2])))
         + x86("call", "rt_unify_terms", (uint64_t)(uintptr_t)(void *)rt_unify_terms)
         + x86("test", "eax", "eax")
         + x86("je", "ω")
         + x86("jmp", "γ")
         + x86("def", "β")
         + x86("jmp", "ω"))
         + IF(bcu_sh() == 3,
           x86("mov", "rdi", FRQ(GZ_CELL_OFF((int)_.op_parts_ival[1])))
         + x86("movsd", "xmm0", F64(bcu_fv()))
         + x86("call", "rt_pl_unify_cell_float", (uint64_t)(uintptr_t)(void *)rt_pl_unify_cell_float)
         + x86("test", "eax", "eax")
         + x86("je", "ω")
         + x86("jmp", "γ")
         + x86("def", "β")
         + x86("jmp", "ω"))
         + IF(bcu_sh() == 4,
           x86("mov", "rdi", FRQ(GZ_CELL_OFF((int)_.op_parts_ival[1])))
         + x86("mov", "esi", (long)_.op_parts_ival[2])
         + x86("mov", "rdx", (long)_.op_parts_ival[3])
         + IF(_.op_parts_str[0] != 0, x86("mov", "rcx", ROQ(0)))
         + IF(_.op_parts_str[0] == 0, x86("mov", "ecx", (long)0))
         + x86("call", "rt_pl_unify_cell_const", (uint64_t)(uintptr_t)(void *)rt_pl_unify_cell_const)
         + x86("test", "eax", "eax")
         + x86("je", "ω")
         + x86("jmp", "γ")
         + x86("def", "β")
         + x86("jmp", "ω")
         + IF(_.op_parts_str[0] != 0,
           x86("def", L(0))
         + x86(".quad", LS(0), _.op_parts_str[0])
         + x86("label", LS(0))
         + x86(".string", _.op_parts_str[0])))
         + IF(bcu_sh() == 6,
           x86("jmp", "ω")
         + x86("def", "β")
         + x86("jmp", "ω"))
         + IF(bcu_sh() < 0, x86_bomb("bb_cell_unify: unadmitted operand shape reached the emitter"));
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_cell_unify(void) { bb_emit_x86(bb_cell_unify_str()); }
