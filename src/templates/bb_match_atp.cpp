#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
extern "C" void rt_at_cursor(const char *varname, int cur_delta);
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_atp() {
    static char b[24];
    return !(_.op_sval ? _.op_sval : "")[0] ? std::string()
         : (strtab_label(b, sizeof b, _.op_sval ? _.op_sval : ""),
              x86("comment", "IR_MATCH_ATP")
            + x86_alpha()
            + x86("mov",  "esi", "r14d")
            + x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), b)
            + x86("call", "rt_at_cursor", (uint64_t)(uintptr_t)(void *)(void (*)(const char *, int))rt_at_cursor)
            + x86_gamma()
            + x86_beta_trampoline());
}
