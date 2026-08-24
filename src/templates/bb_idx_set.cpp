#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
int subscript_set(DESCR_t arr, DESCR_t idx, DESCR_t val);
extern int g_monitor_bin;
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_idx_set() {
    if (!(_.op_a_slot >= 0 && _.op_sb >= 0 && _.op_sc >= 0))
        return x86_alpha() + x86_bomb("bb_idx_set: needs base/key/value operand slots ([ζ+off] producers)");
    x86_begin();
    return x86_alpha()
         + x86("comment", "IR_IDX_SET: base/key/value from [ζ+off]; inline DT_A+int fast path, else subscript_set")
         + IF(g_monitor_bin, x86("comment", "s112 MON-CAP: under MONITOR_BIN the inline DT_A+int fast path is SKIPPED so every subscripted store routes through subscript_set, which owns the <lval> VALUE tap. Emit-time gate — with the monitor dark the fast path emits verbatim, so default bytes are unchanged. Tables never took the fast path; arrays did, and would otherwise stay blind to the monitor exactly where beauty.sno's UTF/table class was."))
         + IF(g_monitor_bin, x86("jmp", L(0)))
         + x86("mov", "rax", FRQ(_.op_a_slot))
         + x86("cmp", "al", (long)DT_A)
         + x86("jne", L(0))
         + x86("mov", "rax", FRQ(_.op_sb))
         + x86("cmp", "al", (long)DT_I)
         + x86("jne", L(0))
         + x86("mov", "rsi", FRQ(_.op_a_slot + 8))
         + x86("mov", "rcx", FRQ(_.op_sb + 8))
         + x86("mov", "r8",  "[rsi]")
         + x86("sub", "ecx", "r8d")
         + x86("js",  L(0))
         + x86("mov", "r9",  "[rsi + 4]")
         + x86("sub", "r9d", "r8d")
         + x86("cmp", "ecx", "r9d")
         + x86("jg",  L(0))
         + x86("mov", "rdi", "[rsi + 24]")
         + x86("movsxd", "rcx", "ecx")
         + x86("add", "rcx", "rcx")
         + x86("add", "rcx", "rcx")
         + x86("add", "rcx", "rcx")
         + x86("add", "rcx", "rcx")
         + x86("add", "rdi", "rcx")
         + IF(_.op_num_real, x86("mov", "rax", FRQ(_.op_sc)))
         + IF(_.op_num_real, x86("mov", "rdx", FRQ(_.op_sc + 8)))
         + IF(!_.op_num_real, x86("mov", "rax", (long)DT_I))
         + IF(!_.op_num_real, x86("mov", "rdx", FRQ(_.op_sc)))
         + x86("mov", "[rdi + 0]", "rax")
         + x86("mov", "[rdi + 8]", "rdx")
         + x86_gamma()
         + x86("def", L(0))
         + x86("mov",  "rdi", FRQ(_.op_a_slot))
         + x86("mov",  "rsi", FRQ(_.op_a_slot + 8))
         + x86("mov",  "rdx", FRQ(_.op_sb))
         + x86("mov",  "rcx", FRQ(_.op_sb + 8))
         + IF(_.op_num_real, x86("mov", "r8",  FRQ(_.op_sc)))
         + IF(_.op_num_real, x86("mov", "r9",  FRQ(_.op_sc + 8)))
         + IF(!_.op_num_real, x86("mov", "r8", (long)DT_I))
         + IF(!_.op_num_real, x86("mov", "r9", FRQ(_.op_sc)))
         + x86("call", "subscript_set", (uint64_t)(uintptr_t)(void *) subscript_set)
         + x86("cmp",  "eax", (long)0)
         + x86_omega("je")
         + x86_gamma()
         + x86_beta_trampoline();
}
