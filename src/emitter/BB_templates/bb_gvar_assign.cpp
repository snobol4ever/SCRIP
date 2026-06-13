#include <string>
#include <cstdint>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
void rt_gvar_assign_str(const char * name, const char * str);
void rt_gvar_assign_var(const char * dst, const char * src);
void rt_gvar_assign_int(const char * name, int64_t val);
void rt_gvar_assign_descr(const char * name, int64_t lo, int64_t hi);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_gvar_assign() {
    union { double d; uint64_t q; } _uf; _uf.d = _.op_a_dval; uint64_t _bits_f = _uf.q;
    if (PLATFORM_X86)
        return x86("label", _.lbl_α)
             + x86("comment", "IR_ASSIGN")
             + (_.op_a_node_kind == (int)IR_LIT_S ?
                    x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t)(_.op_sval ? _.op_sval : ""), _.bb_ls)
                  + x86("lea",  "rsi", "[rip + __]", (uint64_t)(uintptr_t)(_.op_a_sval ? _.op_a_sval : ""), _.bb_rs)
                  + x86("call", "rt_gvar_assign_str", (uint64_t)(uintptr_t)(void *)(void (*)(const char *, const char *))rt_gvar_assign_str)
                  + x86("jmp",  "γ")
                  + x86("def",  "β")
                  + x86("jmp",  "ω")
             : _.op_a_node_kind == (int)IR_LIT_I ?
                    x86("lea",    "rdi", "[rip + __]", (uint64_t)(uintptr_t)(_.op_sval ? _.op_sval : ""), _.bb_ls)
                  + x86("movabs", "rsi", (uint64_t)_.op_a_ival_sg)
                  + x86("call",   "rt_gvar_assign_int", (uint64_t)(uintptr_t)(void *)(void (*)(const char *, int64_t))rt_gvar_assign_int)
                  + x86("jmp",    "γ")
                  + x86("def",    "β")
                  + x86("jmp",    "ω")
             : _.op_a_node_kind == (int)IR_LIT_F ?
                    x86("lea",    "rdi", "[rip + __]", (uint64_t)(uintptr_t)(_.op_sval ? _.op_sval : ""), _.bb_ls)
                  + x86("movabs", "rsi", (uint64_t)7)
                  + x86("movabs", "rdx", _bits_f)
                  + x86("call",   "rt_gvar_assign_descr", (uint64_t)(uintptr_t)(void *)(void (*)(const char *, int64_t, int64_t))rt_gvar_assign_descr)
                  + x86("jmp",    "γ")
                  + x86("def",    "β")
                  + x86("jmp",    "ω")
             : _.op_a_node_kind == (int)IR_VAR ?
                    x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t)(_.op_sval ? _.op_sval : ""), _.bb_ls)
                  + x86("lea",  "rsi", "[rip + __]", (uint64_t)(uintptr_t)(_.op_a_sval ? _.op_a_sval : ""), _.bb_rs)
                  + x86("call", "rt_gvar_assign_var", (uint64_t)(uintptr_t)(void *)(void (*)(const char *, const char *))rt_gvar_assign_var)
                  + x86("jmp",  "γ")
                  + x86("def",  "β")
                  + x86("jmp",  "ω")
             : _.op_a_node_kind == (int)IR_BINOP ?
                    (_.op_a_slot < 0 ? x86_bomb("bb_gvar_assign int-binop: op_a_slot==-1 (binop slot not promoted)")
                   : x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t)(_.op_sval ? _.op_sval : ""), _.bb_ls)
                   + x86("mov",  "rsi", FRQ(_.op_a_slot))
                   + x86("call", "rt_gvar_assign_int", (uint64_t)(uintptr_t)(void *)(void (*)(const char *, int64_t))rt_gvar_assign_int)
                   + x86("jmp",  "γ")
                   + x86("def",  "β")
                   + x86("jmp",  "ω"))
             : _.op_a_node_kind == (int)IR_CALL ?
                    (_.op_a_slot < 0 ? x86_bomb("bb_gvar_assign call-result: op_a_slot==-1 (call result slot not promoted)")
                   : x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)(_.op_sval ? _.op_sval : ""), _.bb_ls)
                   + x86("mov", "rsi", FRQ(_.op_a_slot))
                   + x86("mov", "rdx", FRQ(_.op_a_slot + 8))
                   + x86("call","rt_gvar_assign_descr", (uint64_t)(uintptr_t)(void *)(void (*)(const char *, int64_t, int64_t))rt_gvar_assign_descr)
                   + x86("jmp", "γ")
                   + x86("def", "β")
                   + x86("jmp", "ω"))
             : x86_bomb("bb_gvar_assign other: unhandled rhs shape"));
    return std::string();
}
