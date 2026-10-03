#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_setexit_take(int land_id, long drop) {
    return x86("mov", "rcx", "[rip@got + __]", (uint64_t)(uintptr_t)(const void *)&rtccb[0], "rtccb")
         + x86("mov", "rax", RDQ("rcx", 200))
         + x86("test", "rax", "rax")
         + x86_jcc_id("jz", land_id)
         + IF(drop > 0, x86("add", "rsp", drop))
         + x86("mov", RDQ("rcx", 200), 0L)
         + x86_lea_id("rdx", land_id + 60)
         + x86("mov", RDQ("rcx", 208), "rdx")
         + x86("mov", RDQ("rcx", 216), "rsp")
         + x86("mov", RDQ("rcx", 224), "rbp")
         + x86("mov", RDQ("rcx", 232), "r12")
         + x86("jmp", "rax")
         + x86_deflabel_id(land_id + 60)
         + IF(drop > 0, x86("sub", "rsp", drop));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_setexit_test() {
    return x86("comment", "IR_SETEXIT_TEST: a SETEXIT trap core_runtime_error left pending in rtccb[25] enters its handler on this statement's failure path; "
                          "the inline take keeps this path (a resume label in this box), rsp, rbp and r12 in rtccb[26..29] for CONTINUE")
         + x86_alpha()
         + bb_setexit_take(1, _.op_trap_drop)
         + x86_deflabel_id(1)
         + x86_gamma()
         + x86_beta_trampoline();
}
