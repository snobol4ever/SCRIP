#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
extern long g_stno;
extern long g_line;
extern long g_stcount;
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_stmt_mark(long stno, long line) {
    x86_begin();
    return x86("comment", "STATEMENT MARK: the statement context the SPITBOL termination report reads, stored inline. A program that mentions no statement keyword carries no SNO$STMT dispatch, so before this box g_stno/g_line/g_stcount were still at their initialisers when core_runtime_error() terminated and the report read zero in five of eight fields")
         + x86_alpha()
         + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_stno, "g_stno")
         + x86("mov", RDQ("rax", 0), (long)stno)
         + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_line, "g_line")
         + x86("mov", RDQ("rax", 0), (long)line)
         + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_stcount, "g_stcount")
         + x86("inc", RDQ("rax", 0))
         + x86_gamma()
         + x86_beta_trampoline();
}
