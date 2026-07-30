#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "SM.h"
#include "ast.h"
#include "descr.h"
#include "../runtime/builtins/gen.h"
DESCR_t str_concat_d(DESCR_t a, DESCR_t b);
}
#include "x86_asm.h"
#include <cstdio>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ZTOS-1 (Lon s21x-o "Do not put RSP references directly into the templates"): private raw-rsp helper RETIRED -- call sites now speak the sanctioned spine accessor ZTOS/ZTOSD (x86_asm.h), which adds op_zdepth so a box's own carve and its own TOS reads compose instead of colliding.  Byte-identical while this kind is unarmed (op_zdepth==0); correct once it is armed, which is what lets the _spine exclusion list retire. */
static inline int bcs_ok() { return _.op_off >= 0 && _.op_ival == BINOP_CONCAT && _.op_sa >= 0 && _.op_sb >= 0; }
static inline int vfcc() { return x86_port_mode() == ZC_PORT_FORTH && _.op_fc_disp >= 0 && _.op_ival == BINOP_CONCAT; }   /* ZB-VAL-6a: registered value-spine concat -- operands are the TOP TWO cells (a=[rsp+16..31], b=[rsp+0..15]).  str_concat_d is TYPE-BLIND (it takes whatever DESCR pair it is handed and owns the null-string identity + the to-string coercion, SPITBOL manual p.21-22), so unlike the arith arm this needs NO fast/overload/generic ladder and NO omega edge -- the whole box is ONE call plus the net */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_binop_concat_slot() {
    return IF(PLATFORM_X86 && vfcc(),
           x86_alpha()
         + x86("comment", "IR_BINOP_CONCAT fc")
         + x86("mov", "rdi", ZTOS(16))
         + x86("mov", "rsi", ZTOS(24))
         + x86("mov", "rdx", ZTOS(0))
         + x86("mov", "rcx", ZTOS(8))
         + x86("call", "str_concat_d", (uint64_t)(uintptr_t)(void*)str_concat_d)
         + x86_zrelease(16)
         + x86("mov", ZTOS(0), "rax")
         + x86("mov", ZTOS(8), "rdx")
         + x86_gamma()
         + x86_beta_trampoline())
         + IF(PLATFORM_X86 && !vfcc() && bcs_ok(),
           x86_alpha()
         + x86("comment", "IR_BINOP_CONCAT")
         + x86("mov", "rdi", FRQ(_.op_sa))
         + x86("mov", "rsi", FRQ(_.op_sa + 8))
         + x86("mov", "rdx", FRQ(_.op_sb))
         + x86("mov", "rcx", FRQ(_.op_sb + 8))
         + x86("call", "str_concat_d", (uint64_t)(uintptr_t)(void*)str_concat_d)
         + x86("mov", FRQ(_.op_off),     "rax")
         + x86("mov", FRQ(_.op_off + 8), "rdx")
         + x86_gamma()
         + x86_beta_trampoline());
}
