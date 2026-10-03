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
#define RO_SEAL_Q(n, v) \
    (x86("def", L(n)) \
   + x86(".quad", (uint64_t)(v)))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define KS_IDX() (_.op_imm_a >= 0)
std::string bb_keyword_snobol4() {
    return IF(_.op_zres && _.op_name1,
               x86("comment", "IR_KW_SNOBOL4_read zd [KW-D direct cell]")
             + x86_alpha()
             + x86_load_got("rcx", _.op_name1, _.op_addr)
             + (_.op_imm_b ? x86("add", "rcx", (long)_.op_imm_b) : std::string())
             + x86("mov", "rdx", "[rcx]")
             + x86("mov", "rax", (long)DT_I)
             + x86("note", ZRESN())
             + x86("mov",  ZRES(0), "rax")
             + x86("note", ZRESN())
             + x86("mov",  ZRES(8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline())
         + IF(_.op_zres && !_.op_name1 && KS_IDX(),
               x86("comment", "IR_KW_SNOBOL4_read zd [KW-3 static idx]")
             + x86_alpha()
             + x86("mov", "rdi", ROQ(0))
             + x86("call",    "rt_kw_read_idx", (uint64_t)(uintptr_t)(void *)rt_kw_read_idx)
             + x86("note", ZRESN())
             + x86("mov",  ZRES(0), "rax")
             + x86("note", ZRESN())
             + x86("mov",  ZRES(8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline()
             + RO_SEAL_Q(0, (uint64_t)(int64_t)_.op_imm_a))
         + IF(_.op_zres && !_.op_name1 && !KS_IDX(),
               x86("comment", "IR_KW_SNOBOL4_read zd")
             + x86_alpha()
             + x86("mov",     "rdi", ROQ(0))
             + x86("call",    "rt_keyword_read_snobol4", (uint64_t)(uintptr_t)(void *)rt_keyword_read_snobol4)
             + x86("cmp",     "al", (long)DT_FAIL)
             + x86_omega("je")
             + x86("note", ZRESN())
             + x86("mov",  ZRES(0), "rax")
             + x86("note", ZRESN())
             + x86("mov",  ZRES(8), "rdx")
             + x86_rt_gc_poll()
             + x86_gamma()
             + x86_beta_trampoline()
             + x86("def",     L(0))
             + x86(".quad",   LS(0), _.op_sval)
             + x86("label",   LS(0))
             + x86(".string", _.op_sval))
         + IF(!_.op_zres && !(_.op_off >= 0), x86_alpha() + x86_bomb("bb_keyword_snobol4: no slot"))
         + IF(!_.op_zres && (_.op_off >= 0) && _.op_name1,
               x86("comment", "IR_KW_SNOBOL4_read [KW-D direct cell]")
             + x86_alpha()
             + x86_load_got("rcx", _.op_name1, _.op_addr)
             + (_.op_imm_b ? x86("add", "rcx", (long)_.op_imm_b) : std::string())
             + x86("mov", "rdx", "[rcx]")
             + x86("mov", "rax", (long)DT_I)
             + x86("mov", FRQ(_.op_off),     "rax")
             + x86("mov", FRQ(_.op_off + 8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline())
         + IF(!_.op_zres && (_.op_off >= 0) && !_.op_name1 && KS_IDX(),
               x86("comment", "IR_KW_SNOBOL4_read [KW-3 static idx]")
             + x86_alpha()
             + x86("mov", "rdi", ROQ(0))
             + x86("call",    "rt_kw_read_idx", (uint64_t)(uintptr_t)(void *)rt_kw_read_idx)
             + x86("mov",     FRQ(_.op_off),     "rax")
             + x86("mov",     FRQ(_.op_off + 8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline()
             + RO_SEAL_Q(0, (uint64_t)(int64_t)_.op_imm_a))
         + IF(!_.op_zres && (_.op_off >= 0) && !_.op_name1 && !KS_IDX(),
               x86("comment", "IR_KW_SNOBOL4_read")
             + x86_alpha()
             + x86("mov",     "rdi", ROQ(0))
             + x86("call",    "rt_keyword_read_snobol4", (uint64_t)(uintptr_t)(void *)rt_keyword_read_snobol4)
             + x86("cmp",     "al", (long)DT_FAIL)
             + x86_omega("je")
             + x86("mov",     FRQ(_.op_off),     "rax")
             + x86("mov",     FRQ(_.op_off + 8), "rdx")
             + x86_rt_gc_poll()
             + x86_gamma()
             + x86_beta_trampoline()
             + x86("def",     L(0))
             + x86(".quad",   LS(0), _.op_sval)
             + x86("label",   LS(0))
             + x86(".string", _.op_sval));
}
