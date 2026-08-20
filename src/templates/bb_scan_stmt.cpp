#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
int rt_scan_lit(const char * subj_name, const char * subj_lit, const char * pat_lit, int is_repl, const char * repl_lit);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * scan_lbl(const char * s) { s = s ? s : ""; const char * l = emit_intern_str(s); if (l) return l; static char b[24]; strtab_label(b, sizeof b, s); return b; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_scan_stmt() {
    if (!PLATFORM_X86) return std::string();
    if (_.op_scan_pat_lit)
        return x86("comment", "IR_SCAN")
             + x86_alpha()
             + IF(_.op_sval && _.op_sval[0], x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)_.op_sval, scan_lbl(_.op_sval)))
             + IF(!(_.op_sval && _.op_sval[0]), x86("mov", "rdi", (long)0))
             + IF(_.op_scan_subj_lit != 0,  x86("lea", "rsi", "[rip + __]", (uint64_t)(uintptr_t)_.op_scan_subj_lit, scan_lbl(_.op_scan_subj_lit)))
             + IF(!_.op_scan_subj_lit, x86("mov", "rsi", (long)0))
             + x86("lea",     "rdx", "[rip + __]", (uint64_t)(uintptr_t)_.op_scan_pat_lit, scan_lbl(_.op_scan_pat_lit))
             + x86("mov",     "rcx", _.op_ival ? 1L : 0L)
             + IF(_.op_scan_replace_lit != 0,  x86("lea", "r8", "[rip + __]", (uint64_t)(uintptr_t)_.op_scan_replace_lit, scan_lbl(_.op_scan_replace_lit)))
             + IF(!_.op_scan_replace_lit, x86("mov", "r8", (long)0))
             + x86("call",    "rt_scan_lit", (uint64_t)(uintptr_t)(void *)(int (*)(const char *, const char *, const char *, int, const char *))rt_scan_lit)
             + x86("test",    "eax", "eax")
             + x86_omega("je")
             + x86_gamma()
             + x86_beta_trampoline();
    if (!(_.op_sval && _.op_sval[0]) && !_.op_scan_subj_lit)
        return x86_bomb("bb_scan: non-literal subject needs native PB-RB graph (pending)");
    if (_.op_ival && !_.op_scan_replace_lit)
        return x86_bomb("bb_scan: non-literal replacement needs native PB-RB graph (pending)");
    return x86_bomb("bb_scan: non-literal pattern needs native PB-RB graph (pending)");   /* medium-retire s170: the medium selected only the DIAGNOSTIC WORDING here -- TEXT split the refuse three ways by which operand was non-literal, BINARY lumped all three under "pattern".  The real discriminator is behavioural, not the medium, so it is spelled that way; m3 gains the subject/replacement precision it never had.  The second test no longer needs its subject conjunct: falling past the first test already proves a literal subject. */
}
