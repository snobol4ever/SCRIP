#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
DESCR_t rt_keyword_read_snobol4(const char *sval);
int rt_kw_index(const char *kw);
DESCR_t rt_kw_read_idx(int64_t idx);
const char *rt_kw_direct_sym(int idx, int *soff, const void **base);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_keyword_snobol4() {
    const int kwi = _.op_sval ? rt_kw_index(_.op_sval) : -1;
    int soff = 0; const void *cbase = (const void *)0;
    const char *csym = (kwi >= 0) ? rt_kw_direct_sym(kwi, &soff, &cbase) : (const char *)0;
    if (_.op_zres && csym)
        return x86("comment", "IR_KW_SNOBOL4_read zd [KW-D direct cell]")
             + x86_alpha()
             + x86_load_got("rcx", csym, (uint64_t)(uintptr_t)cbase)
             + (soff ? x86("add", "rcx", (long)soff) : std::string())
             + x86("mov", "rdx", "[rcx]")
             + x86("mov", "rax", (long)DT_I)
             + x86("note", ZRESN())
             + x86("mov",  ZRES(0), "rax")
             + x86("note", ZRESN())
             + x86("mov",  ZRES(8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline();
    if (_.op_zres && kwi >= 0)
        return x86("comment", "IR_KW_SNOBOL4_read zd [KW-3 static idx]")
             + x86_alpha()
             + x86_ro_load_q("rdi", 0)
             + x86("call",    "rt_kw_read_idx", (uint64_t)(uintptr_t)(void *)rt_kw_read_idx)
             + x86("note", ZRESN())
             + x86("mov",  ZRES(0), "rax")
             + x86("note", ZRESN())
             + x86("mov",  ZRES(8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline()
             + x86_ro_seal_q(0, (uint64_t)(int64_t)kwi);
    if (_.op_zres)
        return x86("comment", "IR_KW_SNOBOL4_read zd")
             + x86_alpha()
             + x86("mov",     "rdi", ROQ(0))
             + x86("call",    "rt_keyword_read_snobol4", (uint64_t)(uintptr_t)(void *)rt_keyword_read_snobol4)
             + x86("note", ZRESN())
             + x86("mov",  ZRES(0), "rax")
             + x86("note", ZRESN())
             + x86("mov",  ZRES(8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline()
             + x86("def",     L(0))
             + x86(".quad",   LS(0), _.op_sval)
             + x86("label",   LS(0))
             + x86(".string", _.op_sval);
    if (!(_.op_off >= 0)) return x86_alpha() + x86_bomb("bb_keyword_snobol4: no slot");
    if (csym)
        return x86("comment", "IR_KW_SNOBOL4_read [KW-D direct cell]")
             + x86_alpha()
             + x86_load_got("rcx", csym, (uint64_t)(uintptr_t)cbase)
             + (soff ? x86("add", "rcx", (long)soff) : std::string())
             + x86("mov", "rdx", "[rcx]")
             + x86("mov", "rax", (long)DT_I)
             + x86("mov", FRQ(_.op_off),     "rax")
             + x86("mov", FRQ(_.op_off + 8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline();
    if (kwi >= 0)
        return x86("comment", "IR_KW_SNOBOL4_read [KW-3 static idx]")
             + x86_alpha()
             + x86_ro_load_q("rdi", 0)
             + x86("call",    "rt_kw_read_idx", (uint64_t)(uintptr_t)(void *)rt_kw_read_idx)
             + x86("mov",     FRQ(_.op_off),     "rax")
             + x86("mov",     FRQ(_.op_off + 8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline()
             + x86_ro_seal_q(0, (uint64_t)(int64_t)kwi);
    return x86("comment", "IR_KW_SNOBOL4_read")
         + x86_alpha()
         + x86("mov",     "rdi", ROQ(0))
         + x86("call",    "rt_keyword_read_snobol4", (uint64_t)(uintptr_t)(void *)rt_keyword_read_snobol4)
         + x86("mov",     FRQ(_.op_off),     "rax")
         + x86("mov",     FRQ(_.op_off + 8), "rdx")
         + x86_gamma()
         + x86_beta_trampoline()
         + x86("def",     L(0))
         + x86(".quad",   LS(0), _.op_sval)
         + x86("label",   LS(0))
         + x86(".string", _.op_sval);
}
