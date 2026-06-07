#include <string>
#include <cstdint>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "descr.h"
int rt_scan(void * pat_graph, void * subj_graph, int is_repl, const char * subj_name, void * repl_graph);
int rt_scan_lit(const char * subj_name, const char * subj_lit, const char * pat_lit, int is_repl, const char * repl_lit);
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
static inline const char * scan_pat_lit()    { return _.op_scan_pat_lit; }
static inline const char * scan_subj_lit()   { return _.op_scan_subj_lit; }
static inline const char * scan_replace_lit() { return _.op_scan_replace_lit; }
static inline int          scan_has_name()   { return _.op_sval && _.op_sval[0]; }
static inline uint64_t     fn_scan_lit()     { int (*f)(const char *, const char *, const char *, int, const char *) = rt_scan_lit; return (uint64_t)(uintptr_t)(void *)f; }
static inline const char * scan_lbl(const char * s) { s = s ? s : ""; const char * l = emit_intern_str(s); if (l) return l; static char b[24]; strtab_label(b, sizeof b, s); return b; }
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_scan_stmt_str() {
    if (!PLATFORM_X86) return std::string();
    return IF(MEDIUM_TEXT && !scan_pat_lit(),
              x86_bomb("bb_scan: TEXT(mode-4) non-literal pattern needs native PB-RB graph (pending)"))
         + IF(MEDIUM_TEXT && scan_pat_lit() && !scan_has_name() && !scan_subj_lit(),
              x86_bomb("bb_scan: TEXT(mode-4) non-literal subject needs native PB-RB graph (pending)"))
         + IF(MEDIUM_TEXT && scan_pat_lit() && (scan_has_name() || scan_subj_lit()) && scan_is_repl() && !scan_replace_lit(),
              x86_bomb("bb_scan: TEXT(mode-4) non-literal replacement needs native PB-RB graph (pending)"))
         + IF(MEDIUM_TEXT && scan_pat_lit() && (scan_has_name() || scan_subj_lit()) && (!scan_is_repl() || scan_replace_lit()),
               x86("label", _.lbl_α)
             + x86("comment", "BOX IR_SCAN literal-pattern [rt_scan_lit, RO ptrs @PLT]")
             + FOR(0, 5, [&](int i) {
                   return IF(i == 0 && scan_has_name(),  x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)scan_subj_name(), scan_lbl(scan_subj_name())))
                        + IF(i == 0 && !scan_has_name(), x86("mov", "rdi", (long)0))
                        + IF(i == 1 && (scan_subj_lit() != 0),  x86("lea", "rsi", "[rip + __]", (uint64_t)(uintptr_t)scan_subj_lit(), scan_lbl(scan_subj_lit())))
                        + IF(i == 1 && !scan_subj_lit(), x86("mov", "rsi", (long)0))
                        + IF(i == 2, x86("lea", "rdx", "[rip + __]", (uint64_t)(uintptr_t)scan_pat_lit(), scan_lbl(scan_pat_lit())))
                        + IF(i == 3, x86("mov", "rcx", scan_is_repl()))
                        + IF(i == 4 && (scan_replace_lit() != 0),  x86("lea", "r8", "[rip + __]", (uint64_t)(uintptr_t)scan_replace_lit(), scan_lbl(scan_replace_lit())))
                        + IF(i == 4 && !scan_replace_lit(), x86("mov", "r8", (long)0)); })
             + x86("call", "rt_scan_lit", fn_scan_lit())
             + x86("test", "eax", "eax")
             + x86("je",   "ω")
             + x86("jmp",  "γ")
             + x86("def",  "β")
             + x86("jmp",  "ω"))
         + IF(!MEDIUM_TEXT,
               x86_load_ro("rdi", "??", scan_pat_graph())
             + x86_load_ro("rsi", "??", scan_subj_graph())
             + x86("mov",  "rdx", scan_is_repl())
             + x86("lea",  "rcx", "[rip + __]", scan_subj_addr(), scan_subj_label())
             + x86_load_ro("r8", "??", scan_repl_graph())
             + x86("call", "rt_scan", fn_scan())
             + x86("test", "eax", "eax")
             + x86("je",   "ω")
             + x86("jmp",  "γ")
             + x86("def",  "β")
             + x86("jmp",  "ω"));
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_scan_stmt(IR_t * pBB) { (void)pBB; bb_emit_x86(bb_scan_stmt_str()); }
