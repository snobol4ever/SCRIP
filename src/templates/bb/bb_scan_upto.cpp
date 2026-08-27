#include <string>
#include <cstring>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_scan_upto() {
    x86_begin();
        return (_.op_off >= 0 && !_.op_name1 && _.op_sa >= 0) ?
               x86("comment", "IR_SCAN_UPTO (var cset) [fstranl.r upto: generate positions with s[i] in cset-descr@slot; cursor at off+16; beta resumes]")
             + x86_alpha()
             + x86("mov",     FRQ(_.op_off + 16), "r14")
             + x86("def",     L(0))
             + x86("mov",     "rax", FRQ(_.op_off + 16))
             + x86("cmp",     "rax", "r15")
             + x86_omega("jge")
             + x86("mov",     "rcx", "rax")
             + x86("movzx",   "esi", "[r13+rcx]")
             + x86("mov",     "rdi", FRQ(_.op_sa + 8))
             + x86("push",    "rax")
             + x86("sub",     "rsp", (long)8)
             + x86("call",    "strchr", (uint64_t)(uintptr_t)(void *)(const char *(*)(const char *, int))strchr)
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
             + x86("call",    "strchr", (uint64_t)(uintptr_t)(void *)(const char *(*)(const char *, int))strchr)
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
    return std::string();
}
