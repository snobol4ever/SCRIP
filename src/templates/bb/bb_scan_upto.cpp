#include <string>
#include <cstring>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
int rt_icn_cset_member(const char *needle, int ch);
typedef struct { uint64_t ptr; uint64_t len; } ScanSubjRegs_needle_t;
ScanSubjRegs_needle_t rt_scan_needle(uint64_t lo, uint64_t hi);
void core_icn_argtype_check(uint64_t lo, uint64_t hi, uint64_t code);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_scan_upto() {
    x86_begin();
        return (_.op_off >= 0 && !_.op_name1 && _.op_sa >= 0) ?
               x86("comment", "IR_SCAN_UPTO (var cset) [fstranl.r upto: generate positions with s[i] in cset-descr@slot; cursor at off+16; beta resumes; rt_scan_needle coerces (int/real->string), mirroring bb_scan_match.cpp -- FINDING-2026-09-03-seat02-icon-jcon-suite-census-and-level-cure.md class fix]")
             + x86_alpha()
             + x86("mov",     "rdi", FRQ(_.op_sa))
             + x86("mov",     "rsi", FRQ(_.op_sa + 8))
             + x86("mov",     "edx", (long)104)
             + x86("sub",     "rsp", (long)8)
             + x86("call",    "core_icn_argtype_check", (uint64_t)(uintptr_t)(void*)core_icn_argtype_check)
             + x86("add",     "rsp", (long)8)
             + x86("mov",     FRQ(_.op_off + 16), "r14")
             + x86("def",     L(0))
             + x86("mov",     "rax", FRQ(_.op_off + 16))
             + x86("cmp",     "rax", "r15")
             + x86_omega("jge")
             + x86("mov",     "rdi", FRQ(_.op_sa))
             + x86("mov",     "rsi", FRQ(_.op_sa + 8))
             + x86("sub",     "rsp", (long)8)
             + x86("call",    "rt_scan_needle", (uint64_t)(uintptr_t)(void*)rt_scan_needle)
             + x86("add",     "rsp", (long)8)
             + x86("mov",     "rbx", "rax")
             + x86("mov",     "rcx", FRQ(_.op_off + 16))
             + x86("movzx",   "esi", "[r13+rcx]")
             + x86("mov",     "rdi", "rbx")
             + x86("sub",     "rsp", (long)8)
             + x86("call",    "rt_icn_cset_member", (uint64_t)(uintptr_t)(void *)(int (*)(const char *, int))rt_icn_cset_member)
             + x86("add",     "rsp", (long)8)
             + x86("test",    "rax", "rax")
             + x86("mov",     "rax", FRQ(_.op_off + 16))
             + x86("je",      L(1))
             + x86("mov",     FRQ(_.op_off), (long)DT_I)
             + x86("add",     "rax", (long)1)
             + x86("mov",     FRQ(_.op_off + 8), "rax")
             + x86_gamma()
             + x86("def",     L(1))
             + x86("inc",     FRQ(_.op_off + 16))
             + x86("jmp",     L(0))
             + x86_beta()
             + x86("inc",     FRQ(_.op_off + 16))
             + x86("jmp",     L(0)) :
               (_.op_off >= 0 && _.op_name1) ?
               x86("comment", "IR_SCAN_UPTO")
             + x86_alpha()
             + x86("mov",     FRQ(_.op_off + 16), "r14")
             + x86("def",     L(0))
             + x86("mov",     "rax", FRQ(_.op_off + 16))
             + x86("cmp",     "rax", "r15")
             + x86_omega("jge")
             + x86("mov",     "rcx", "rax")
             + x86("movzx",   "esi", "[r13+rcx]")
             + x86("mov",     "rdi", ROQ(2))
             + x86("push",    "rax")
             + x86("sub",     "rsp", (long)8)
             + x86("call",    "rt_icn_cset_member", (uint64_t)(uintptr_t)(void *)(int (*)(const char *, int))rt_icn_cset_member)
             + x86("add",     "rsp", (long)8)
             + x86("test",    "rax", "rax")
             + x86("pop",     "rax")
             + x86("je",      L(1))
             + x86("mov",     FRQ(_.op_off), (long)DT_I)
             + x86("add",     "rax", (long)1)
             + x86("mov",     FRQ(_.op_off + 8), "rax")
             + x86_gamma()
             + x86("def",     L(1))
             + x86("inc",     FRQ(_.op_off + 16))
             + x86("jmp",     L(0))
             + x86_beta()
             + x86("inc",     FRQ(_.op_off + 16))
             + x86("jmp",     L(0))
             + x86("def",     L(2))
             + x86(".quad",   LS(2), _.op_name1)
             + x86("label",   LS(2))
             + x86(".string", _.op_name1) :
               x86_bomb("bb_scan_upto: unhandled (needs literal cset arg + descr flat-chain slot)");
}
