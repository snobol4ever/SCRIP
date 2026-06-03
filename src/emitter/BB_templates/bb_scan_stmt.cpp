#include <string>
#include <cstdint>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "descr.h"
int rt_scan(void * pat_graph, void * subj_graph, int is_repl, const char * subj_name, void * repl_graph);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static inline const char * scan_subj_name()  { return _.op_sval ? _.op_sval : ""; }
static inline uint64_t     scan_subj_addr()  { return (uint64_t)(uintptr_t)scan_subj_name(); }
static inline const char * scan_subj_label() { return emit_intern_str(scan_subj_name()); }
static inline uint64_t     scan_pat_graph()  { return (uint64_t)(uintptr_t)(intptr_t) _.op_scan_pat; }
static inline uint64_t     scan_subj_graph() { return (uint64_t)(uintptr_t)(intptr_t) _.op_scan_subj; }
static inline uint64_t     scan_repl_graph() { return (uint64_t)(uintptr_t)(intptr_t) _.op_scan_repl; }
static inline long         scan_is_repl()    { return _.op_ival ? 1L : 0L; }
static inline uint64_t     fn_scan()         { int (*f)(void *, void *, int, const char *, void *) = rt_scan; return (uint64_t)(uintptr_t)(void *)f; }
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_scan_stmt_str() {
    if (PLATFORM_X86) {
        if (MEDIUM_TEXT) return x86_bomb("bb_scan: TEXT(mode-4) needs relocatable subgraph addrs (SNOBOL m4 pending LOWER four-port wiring)");
        return x86_load_ro("rdi", "??", scan_pat_graph())
             + x86_load_ro("rsi", "??", scan_subj_graph())
             + x86("mov",  "rdx", scan_is_repl())
             + x86("lea",  "rcx", "[rip + __]", scan_subj_addr(), scan_subj_label())
             + x86_load_ro("r8", "??", scan_repl_graph())
             + x86("call", "rt_scan", fn_scan())
             + x86("test", "eax", "eax")
             + x86("je",   PORT_OMEGA)
             + x86("jmp",  PORT_GAMMA)
             + x86("def",  PORT_BETA)
             + x86("jmp",  PORT_OMEGA);
    }
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_scan_stmt(IR_t * pBB) { (void)pBB; bb_emit_x86(bb_scan_stmt_str()); }
