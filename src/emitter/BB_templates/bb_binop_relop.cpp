#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "SM.h"
#include "ast.h"
#include "descr.h"
#include "../../runtime/builtins/gen.h"
extern int g_descr_flat_chain;
int rt_jct_relop(DESCR_t lhs, DESCR_t rhs, int op);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static int rel_is_numrel(int64_t op) { return op >= BINOP_LT && op <= BINOP_NE; }
static int rel_is_strrel(int64_t op) { return op >= BINOP_SLT && op <= BINOP_SNE; }
static const char * rel_fail_mnem(int64_t op) {
    switch (op) {
    case BINOP_LT: return "jge";
    case BINOP_LE: return "jg";
    case BINOP_GT: return "jle";
    case BINOP_GE: return "jl";
    case BINOP_EQ: return "jne";
    case BINOP_NE: return "je";
    default:       return "jmp";
    }
}
static inline int brr_num_ok() { return g_descr_flat_chain && _.op_off >= 0 && rel_is_numrel(_.op_ival) && _.op_sa >= 0 && _.op_sb >= 0; }
static inline int brr_str_ok() { return g_descr_flat_chain && _.op_off >= 0 && rel_is_strrel(_.op_ival) && _.op_sa >= 0 && _.op_sb >= 0; }
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_binop_relop() {
    if (PLATFORM_X86) {
        if (brr_num_ok()) return IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                                               + x86("comment", "IR_BINOP_RELOP numeric"))
                               + x86("mov", "rax", FRQ(_.op_sa + 8))
                               + x86("mov", "rcx", FRQ(_.op_sb + 8))
                               + x86("cmp", "rax", "rcx")
                               + x86(rel_fail_mnem(_.op_ival), "ω")
                               + x86("mov", "rcx", FRQ(_.op_sb))
                               + x86("mov", FRQ(_.op_off), "rcx")
                               + x86("mov", "rcx", FRQ(_.op_sb + 8))
                               + x86("mov", FRQ(_.op_off + 8), "rcx")
                               + x86("jmp", "γ")
                               + x86("def", "β")
                               + x86("jmp", "ω");
        if (brr_str_ok()) { uint64_t fptr; { int (*fp)(DESCR_t,DESCR_t,int) = rt_jct_relop; fptr = (uint64_t)(uintptr_t)(void*)fp; }
            return IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                                 + x86("comment", "IR_BINOP_RELOP string"))
                 + x86("mov", "rdi", FRQ(_.op_sa))
                 + x86("mov", "rsi", FRQ(_.op_sa + 8))
                 + x86("mov", "rdx", FRQ(_.op_sb))
                 + x86("mov", "rcx", FRQ(_.op_sb + 8))
                 + x86("mov", "r8d", (long)_.op_ival)
                 + x86("call", "rt_jct_relop", fptr)
                 + x86("test", "eax", "eax")
                 + x86("jz", "ω")
                 + x86("mov", "rax", FRQ(_.op_sb))
                 + x86("mov", FRQ(_.op_off), "rax")
                 + x86("mov", "rax", FRQ(_.op_sb + 8))
                 + x86("mov", FRQ(_.op_off + 8), "rax")
                 + x86("jmp", "γ")
                 + x86("def", "β")
                 + x86("jmp", "ω");
        }
        return x86_bomb("bb_binop_relop: shape mismatch");
    }
    return std::string();
}
