#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
extern long g_stno;
extern long g_line;
extern long g_lastno;
extern long g_lastline;
extern const char *g_file;
extern long g_stcount;
}
#include "x86_asm.h"
#include "bb_templates.h"
#define RO_SEAL_STR(n, s) \
    (x86("def", L(n)) \
   + x86(".quad", LS(n), (s)) \
   + x86("label", LS(n)) \
   + x86(".string", (s)))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_stmt_mark(long stno, long line) {
    x86_begin();
    return x86_alpha()
         + IF(_.op_seal == IR_SEAL_STMT_HOOK,
               x86("comment", "IR_STMT_MARK stlimit hook: the leaf counts the statement and fills a pending KEYWORD trace; the box runs the callback (CEO-1635)")
             + x86_align_enter()
             + x86("sub", "rsp", 112L)
             + x86("mov", RDQ("rsp", 0), 0L)
             + x86("mov", "rdi", (long)stno)
             + x86("mov", "rsi", (long)line)
             + x86("mov", "rdx", "rsp")
             + x86("call", "rt_stmt_enter_p", (uint64_t)(uintptr_t)(void *)rt_stmt_enter_p)
             + x86_rt_gc_poll()
             + bb_glue_trace_pend_run(100, std::string(), 0)
             + x86("add", "rsp", 112L)
             + x86_align_leave())
         + IF(_.op_seal == IR_SEAL_LABEL_HOOK,
               x86("comment", "IR_STMT_MARK label hook: a transfer reached a traced label; the leaf fills a pending LABEL trace and the box runs the callback (CEO-1644)")
             + x86_align_enter()
             + x86("sub", "rsp", 112L)
             + x86("mov", RDQ("rsp", 0), 0L)
             + x86("mov", RDQ("rsp", 8), 0L)
             + bb_glue_name_or_rec_lea(_.op_sval ? _.op_sval : "")
             + x86("mov", "rsi", "rsp")
             + x86("call", "rt_trace_label_hook_p", (uint64_t)(uintptr_t)(void *)rt_trace_label_hook_p)
             + x86_rt_gc_poll()
             + bb_glue_trace_pend_run(100, std::string(), 0)
             + x86("add", "rsp", 112L)
             + x86_align_leave())
         + IF(_.op_seal != IR_SEAL_STMT_HOOK && _.op_seal != IR_SEAL_LABEL_HOOK,
               x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_stno, "g_stno")
         + x86("mov", "rcx", RDQ("rax", 0))
         + x86("mov", RDQ("rax", 0), (long)stno)
         + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_lastno, "g_lastno")
         + x86("mov", RDQ("rax", 0), "rcx")
         + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_line, "g_line")
         + x86("mov", "rcx", RDQ("rax", 0))
         + x86("mov", RDQ("rax", 0), (long)line)
         + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_lastline, "g_lastline")
         + x86("mov", RDQ("rax", 0), "rcx")
         + IF(x86_trace_hooks_on(), x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_stcount, "g_stcount")
         + x86("inc", RDQ("rax", 0))))
         + x86_gamma()
         + x86_beta_trampoline();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_line_mark(long line, const char * file) {
    x86_begin();
    return x86_alpha()
         + IF(line >= 0, x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_line, "g_line")
         + x86("mov", RDQ("rax", 0), (long)line))
         + IF(file != (const char *)0,
             x86("comment", "IR_LINE_MARK with the source file: g_file <- the read-only string, the ICN$LINE dispatcher call this replaces set exactly g_line and g_file")
           + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_file, "g_file")
           + x86("mov", "rcx", ROQ(0))
           + x86("mov", RDQ("rax", 0), "rcx"))
         + x86_gamma()
         + x86_beta_trampoline()
         + IF(file != (const char *)0, RO_SEAL_STR(0, file));
}
