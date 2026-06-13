#include <string>
#include <cstring>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
#include "emit_core.h"
#include "IR.h"
}
#include "x86_asm.h"
extern "C" int rt_unify_terms(void *l, void *r);
extern "C" int rt_pl_unify_cell_const(void *cell_term, int kind, long ival, const char *sval);
extern "C" int rt_pl_unify_cell_float(void *cell_term, double dval);
extern "C" void *rt_node_to_term(int kind, long ival, const char *sval, double dval);
extern "C" void *rt_compound_build_n(const char *functor_name, int arity, void *args_ptr);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string gzu_atom_lea(const char *dst, const char *s) { char b[64]; b[0] = 0; if (s) strtab_label(b, sizeof b, s); return x86("lea", dst, "[rip + __]", (uint64_t)(uintptr_t)(s ? s : ""), b); }
static std::string gzu_node_atom(const char *sval) { return x86("mov32", "edi", (long)IR_ATOM) + x86("xor", "rsi", "rsi") + gzu_atom_lea("rdx", sval ? sval : "[]") + x86("xorps", "xmm0", "xmm0") + x86("call", "rt_node_to_term", (uint64_t)(uintptr_t)(void *)rt_node_to_term); }
static std::string gzu_build(const IR_t *nd) {
    if (!nd) return x86("xor", "eax", "eax");
    if (nd->op == IR_LOGICVAR) { int slot = (int)IR_LIT(nd).ival; return (slot < 0) ? x86("xor", "eax", "eax") : x86("mov", "rax", FRQ(GZ_CELL_OFF(slot))); }
    if (nd->op == IR_ATOM)  return gzu_node_atom(IR_LIT(nd).sval);
    if (nd->op == IR_LIT_I) return x86("mov32", "edi", (long)IR_LIT_I) + x86("mov", "rsi", (long)IR_LIT(nd).ival) + x86("xor", "edx", "edx") + x86("xorps", "xmm0", "xmm0") + x86("call", "rt_node_to_term", (uint64_t)(uintptr_t)(void *)rt_node_to_term);
    if (nd->op == IR_LIT_F) return x86("mov32", "edi", (long)IR_LIT_F) + x86("xor", "rsi", "rsi") + x86("xor", "edx", "edx") + x86("movsd", "xmm0", F64(IR_LIT(nd).dval)) + x86("call", "rt_node_to_term", (uint64_t)(uintptr_t)(void *)rt_node_to_term);
    if (nd->op == IR_STRUCT) {
        int arity = (int)IR_LIT(nd).ival;
        if (arity <= 0 || !ir_call_arg(nd, 0)) return gzu_node_atom(IR_LIT(nd).sval);
        int frm = (arity * 8 + 15) & ~15;
        return x86("sub", "rsp", (long)frm)
             + FOR(0, arity, [&](int i) { return IF(ir_call_arg(nd, i) != NULL, gzu_build(ir_call_arg(nd, i)) + x86("mov", RSP(i * 8), "rax")); })
             + gzu_atom_lea("rdi", IR_LIT(nd).sval ? IR_LIT(nd).sval : "[]")
             + x86("mov32", "esi", (long)arity)
             + x86("mov", "rdx", "rsp")
             + x86("call", "rt_compound_build_n", (uint64_t)(uintptr_t)(void *)rt_compound_build_n)
             + x86("add", "rsp", (long)frm);
    }
    return x86("xor", "eax", "eax");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bcu_sh() { return (int)_.op_parts_ival[0]; }
static double bcu_fv() {
    double d; memcpy(&d, &_.op_parts_ival[2], 8); return d;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_cell_unify() {
    x86_begin();
    if (PLATFORM_X86) return x86("label", _.lbl_α)
         + x86("comment", "IR_CELL_UNIFY")
         + IF(bcu_sh() == 0,
           x86("sub", "rsp", 16L)
         + gzu_build((const IR_t *)(intptr_t)_.op_parts_ival[1])
         + x86("mov", RSP(0), "rax")
         + gzu_build((const IR_t *)(intptr_t)_.op_parts_ival[2])
         + x86("mov", RSP(8), "rax")
         + x86("mov", "rdi", RSP(0))
         + x86("mov", "rsi", RSP(8))
         + x86("add", "rsp", 16L)
         + x86("call", "rt_unify_terms", (uint64_t)(uintptr_t)(void *)rt_unify_terms)
         + x86("test", "eax", "eax")
         + x86("je", "ω")
         + x86("jmp", "γ")
         + x86("def", "β")
         + x86("jmp", "ω"))
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