#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "SM.h"
#include "ast.h"
#include "descr.h"
#include "IR.h"
#include "../runtime/builtins/gen.h"
extern int g_gvar_flat_chain;
DESCR_t str_concat_d(DESCR_t a, DESCR_t b);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static inline int bgc_bareint(int k) { return k == (int)IR_BINOP || k == (int)IR_LIT_INTEGER || k == (int)IR_OP_COUNT; }
static inline int bgc_ok() { return g_gvar_flat_chain && _.op_off >= 0 && _.op_ival == BINOP_CONCAT && _.op_sa >= 0 && _.op_sb >= 0; }
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_binop_gvar_concat() {
    if (!(PLATFORM_X86 && bgc_ok())) return std::string();
    std::string a = bgc_bareint(_.bb_lk)
                  ? x86("mov", "rdi", (long)DT_I) + x86("mov", "rsi", FRQ(_.op_sa + 8))
                  : x86("mov", "rdi", FRQ(_.op_sa)) + x86("mov", "rsi", FRQ(_.op_sa + 8));
    std::string b = bgc_bareint(_.bb_rk)
                  ? x86("mov", "rdx", (long)DT_I) + x86("mov", "rcx", FRQ(_.op_sb + 8))
                  : x86("mov", "rdx", FRQ(_.op_sb)) + x86("mov", "rcx", FRQ(_.op_sb + 8));
    return x86("label", _.lbl_α)
         + x86("comment", "IR_BINOP_GVAR_CONCAT")
         + a + b
         + x86("call", "str_concat_d", (uint64_t)(uintptr_t)(void*)str_concat_d)
         + x86("mov", FRQ(_.op_off),     "rax")
         + x86("mov", FRQ(_.op_off + 8), "rdx")
         + x86("jmp", "γ")
         + x86("def", "β")
         + x86("jmp", "ω");
}
