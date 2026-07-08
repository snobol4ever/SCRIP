#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
typedef struct { uint64_t ptr; uint64_t len; } ScanSubjRegs;
ScanSubjRegs rt_scan_enter(uint64_t lo, uint64_t hi, uint64_t sigma, uint64_t delta, uint64_t Delta);
void rt_scan_leave(uint64_t *out3);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_gen_scan() {
    x86_begin();
    if (!PLATFORM_X86) return x86("def", "α") + x86_bomb("bb_gen_scan: leave glue without regs out-area (op_off < 0)");
    return x86("comment", "IR_GEN_SCAN")
         + x86("def",     "α")
         + IF(_.op_sb == 1,
               x86("mov", "rdi", FRQ(_.op_sa))
             + x86("mov", "rsi", FRQ(_.op_sa + 8))
             + x86("mov", "rdx", "r13")
             + x86("mov", "rcx", "r14")
             + x86("mov", "r8",  "r15")
             + x86("call", "rt_scan_enter", (uint64_t)(uintptr_t)(void *)rt_scan_enter)
             + x86("mov", "r13", "rax")
             + x86("mov", "r15", "rdx")
             + x86("mov", "r14", (long)0)
             + x86("jmp", "γ")
             + x86("def", "β")
             + x86("jmp", "ω"))
         + IF(_.op_sb != 1 && _.op_off >= 0,
               IF(_.op_sa >= 0 && _.op_ival >= 0,
                   x86("mov", "rax", FRQ(_.op_sa))
                 + x86("mov", FRQ(_.op_ival), "rax")
                 + x86("mov", "rax", FRQ(_.op_sa + 8))
                 + x86("mov", FRQ(_.op_ival + 8), "rax"))
             + x86("lea", "rdi", FRQ(_.op_off))
             + x86("call", "rt_scan_leave", (uint64_t)(uintptr_t)(void *)rt_scan_leave)
             + x86("mov", "r13", FRQ(_.op_off))
             + x86("mov", "r14", FRQ(_.op_off + 8))
             + x86("mov", "r15", FRQ(_.op_off + 16))
             + x86("jmp", "γ")
             + x86("def", "β")
             + x86("jmp", "ω"))
         + IF(_.op_sb != 1 && _.op_off < 0, x86_bomb("bb_gen_scan: leave glue without regs out-area (op_off < 0)"));
}
