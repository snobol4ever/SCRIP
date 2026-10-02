#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_setexit_take(int land_id) {
    return x86("mov", "rcx", "[rip@got + __]", (uint64_t)(uintptr_t)(const void *)&rtccb[0], "rtccb")
         + x86("mov", "rax", RDQ("rcx", 200))
         + x86("test", "rax", "rax")
         + x86_jcc_id("jz", land_id)
         + x86("mov", RDQ("rcx", 200), 0L)
         + x86_lea_id("rdx", land_id)
         + x86("mov", RDQ("rcx", 208), "rdx")
         + x86("mov", RDQ("rcx", 216), "rsp")
         + x86("mov", RDQ("rcx", 224), "rbp")
         + x86("mov", RDQ("rcx", 232), "r12")
         + x86("jmp", "rax");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_setexit_test() {
    return x86("comment", "IR_SETEXIT_TEST: a SETEXIT trap core_runtime_error left pending in rtccb[25] enters its handler on this statement's failure path; rtccb[26..29] keep this path, rsp, rbp and r12 for CONTINUE")
         + x86_alpha()
         + bb_setexit_take(1)
         + x86_deflabel_id(1)
         + x86_gamma()
         + x86_beta_trampoline();
}
