#include <string>
#include <cstring>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
#include "x86_asm.h"
extern "C" long rt_sg_scan_member(void);
extern "C" long rt_sg_scan_nonmember(void);
extern "C" long rt_sg_member(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define CSK() ((long) strlen(_.op_sval ? _.op_sval : ""))
static char sp_nlb[24];
static long sp_gu() { return _.op_sa < 0 && (ZC_LIT_GUTS == ZC_LIT_GUTS_UNROLL || ZC_LIT_GUTS == ZC_LIT_GUTS_RANGE); }
static long sp_rangep() { return _.op_sa < 0 && ZC_LIT_GUTS == ZC_LIT_GUTS_RANGE; }
static unsigned char sp_rlo[128], sp_rhi[128]; static long sp_rn;
static void sp_ranges() { unsigned char in[256]; memset(in, 0, 256); for (const unsigned char * p = (const unsigned char *)(_.op_sval ? _.op_sval : ""); *p; p++) in[*p] = 1; sp_rn = 0; for (int b = 0; b < 256; b++) if (in[b]) { if (sp_rn && sp_rhi[sp_rn - 1] == b - 1) sp_rhi[sp_rn - 1] = (unsigned char)b; else { sp_rlo[sp_rn] = sp_rhi[sp_rn] = (unsigned char)b; sp_rn++; } } }
static std::string sp_rtest(long u, long i) { return sp_rlo[i] == sp_rhi[i] ? x86("cmp", "esi", (long)sp_rlo[i]) + x86("je", L(10 + u)) : x86("mov", "eax", "esi") + x86("sub", "eax", (long)sp_rlo[i]) + x86("cmp", "eax", (long)(sp_rhi[i] - sp_rlo[i])) + x86("jbe", L(10 + u)); }
static std::string sp_rmemb(long u, long i) { return i >= sp_rn ? x86("jmp", L(1)) + x86("def", L(10 + u)) : sp_rtest(u, i) + sp_rmemb(u, i + 1); }
static long sp_gi() { return _.op_sa >= 0 ? ZC_SPAN_GUTS == ZC_SPAN_GUTS_INLINE : ZC_LIT_GUTS == ZC_LIT_GUTS_INLINE; }
static long sp_gc() { return _.op_sa >= 0 ? ZC_SPAN_GUTS == ZC_SPAN_GUTS_CALL   : ZC_LIT_GUTS == ZC_LIT_GUTS_CALL; }
static std::string sp_ndl_r8()  { return _.op_sa >= 0 ? x86("mov", "r8",  FRQ(_.op_sa + 8)) + x86("mov", "r9d", FR(_.op_sa + 4)) : x86("lea", "r8",  "[rip + __]", (uint64_t)(uintptr_t)(_.op_sval ? _.op_sval : ""), sp_nlb) + x86("mov32", "r9d", CSK()); }
static std::string sp_ndl_rsi() { return _.op_sa >= 0 ? x86("mov", "rsi", FRQ(_.op_sa + 8)) + x86("mov", "edx", FR(_.op_sa + 4)) : x86("lea", "rsi", "[rip + __]", (uint64_t)(uintptr_t)(_.op_sval ? _.op_sval : ""), sp_nlb) + x86("mov32", "edx", CSK()); }
static long sp_chainp() { return sp_gu() && !sp_rangep() && CSK() >= 1 && CSK() <= ZC_CSET_CHAIN_MAX; }
static long sp_tablep() { return sp_gu() && !sp_rangep() && !sp_chainp(); }
static std::string sp_memb(long u, long i) { return i >= CSK() ? x86("jmp", L(1)) + x86("def", L(10 + u)) : x86("cmp", "esi", (long)(unsigned char)_.op_sval[i]) + x86("je", L(10 + u)) + sp_memb(u, i + 1); }
static std::string sp_char(long u) { return x86("cmp", "ecx", "r15d") + x86("jge", L(1)) + x86("movzx", "esi", "[r13+rcx]") + (sp_rangep() ? sp_rmemb(u, 0) : sp_chainp() ? sp_memb(u, 0) : x86("cmpb0", "[rdi+rsi]", "0") + x86("je", L(1))) + x86("add", "ecx", (long)1); }
std::string bb_match_span() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    static char c[24];
    const void * ct = sp_tablep() ? csettab_label(c, sizeof c, _.op_sval ? _.op_sval : "") : (const void *)0;
    if (sp_rangep()) sp_ranges();
    if (_.op_sa < 0 && ZC_LIT_GUTS != ZC_LIT_GUTS_UNROLL) strtab_label(sp_nlb, sizeof sp_nlb, _.op_sval ? _.op_sval : "");
    if (_.op_zres && _.op_sa >= 0)
        /* MATCH-SPAN-ZD-FIX (this session, 2026-08-12): eax carries the LOOP POSITION into the call, but rt_sg_member returns its boolean result in
         * eax too (call ABI), and the RTCC veneer's reload after a call() covers only the scratch tier
         * {r8,r9,r10,r11} -- eax/rsi/rdx are explicitly NOT restored (RC-4 "arg tier reload deferred", by
         * design, so a caller reading the CALL'S OWN return value in rax still sees it).  The old code read
         * eax as the position, then let the call overwrite it with 0/1 and used THAT for "add eax,1; jmp L(0)"
         * -- position tracking was destroyed after the first successful match.  rsi/edx (needle ptr/len) are
         * loaded ONCE before the loop and likewise have no veneer protection across the call.  FIX: keep the
         * position in FR(_.x86_scratch_off) (memory, call-safe) and reload rsi/edx from the zeta-cell every
         * iteration instead of holding them live in registers across the call. */
             return x86("comment", "IR_MATCH_SPAN zd")
             + x86_alpha()
             + x86("mov",    FR(_.x86_scratch_off), "r14d")
             + x86("def",    L(0))
             + x86("mov",    "eax", FR(_.x86_scratch_off))
             + x86("cmp",    "eax", "r15d")
             + x86("jge",    L(1))   /* end-of-subject with every char matched so far IS a valid span end, not a failure */
             + x86("movsxd", "rcx", "eax")
             + x86("movzx",  "edi", "[r13+rcx]")
             + x86("note",   ZOPN(0)) + x86("mov", "rsi", ZOPQ(0, 8))
             + x86("note",   ZOPN(0)) + x86("mov", "edx", ZOPD(0, 4))
             + x86("call",   "rt_sg_member", (uint64_t)(uintptr_t)(void *)rt_sg_member)
             + x86("test",   "eax", "eax")
             + x86("je",     L(1))
             + x86("mov",    "eax", FR(_.x86_scratch_off))
             + x86("add",    "eax", (long)1)
             + x86("mov",    FR(_.x86_scratch_off), "eax")
             + x86("jmp",    L(0))
             + x86("def",    L(1))
             + x86("mov",    "eax", FR(_.x86_scratch_off))
             + x86("cmp",    "eax", "r14d")
             + x86_omega("je")
             + x86("mov",    FR(_.x86_scratch_off), "r14d")
             + x86("mov",    "r14d", "eax")
             + x86_gamma()
             + x86_beta()
             + x86("mov",    "r14d", FR(_.x86_scratch_off))
             + x86_omega();
    return x86("comment", "IR_MATCH_SPAN")
         + x86_alpha()
         + IF(sp_gi(),
              x86("mov",    FR(_.x86_scratch_off), (long)0)
            + sp_ndl_r8()
            + x86("def",    L(0))
            + x86("mov",    "eax", "r14d")
            + x86("add",    "eax", FR(_.x86_scratch_off))
            + x86("cmp",    "eax", "r15d")
            + x86("jge",    L(1))
            + x86("movsxd", "rcx", "eax")
            + x86("movzx",  "esi", "[r13+rcx]")
            + x86("mov",    "edx", (long)0)
            + x86("def",    L(2))
            + x86("cmp",    "edx", "r9d")
            + x86("jge",    L(1))
            + x86("movzx",  "edi", "[r8+rdx]")
            + x86("cmp",    "esi", "edi")
            + x86("je",     L(3))
            + x86("add",    "edx", (long)1)
            + x86("jmp",    L(2))
            + x86("def",    L(3))
            + x86("add",    FR(_.x86_scratch_off), (long)1)
            + x86("jmp",    L(0))
            + x86("def",    L(1))
            + x86("mov",    "eax", FR(_.x86_scratch_off))
            + x86("test",   "eax", "eax")
            + x86_omega("jle")
            + x86("mov",    "edx", "r14d")
            + x86("mov",    FR(_.x86_scratch_off + 4), "edx")
            + x86("add",    "edx", "eax")
            + x86("mov",    "r14d", "edx"))
         + IF(sp_gc(),
              x86("mov",    "edi", "r14d")
            + sp_ndl_rsi()
            + x86("call",   "rt_sg_scan_nonmember", (uint64_t)(uintptr_t)(void *)rt_sg_scan_nonmember)
            + x86("cmp",    "eax", "r14d")
            + x86_omega("jle")
            + x86("mov",    FR(_.x86_scratch_off + 4), "r14d")
            + x86("mov",    "r14d", "eax"))
         + IF(sp_gu(),
              IF(sp_tablep(), x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)ct, c))
            + x86("movsxd", "rcx", "r14d")
            + x86("def",    L(0))
            + sp_char(0)
            + x86("jmp",    L(0))
            + x86("def",    L(1))
            + x86("cmp",    "ecx", "r14d")
            + x86_omega("jle")
            + x86("mov",    FR(_.x86_scratch_off + 4), "r14d")
            + x86("mov",    "r14d", "ecx"))
         + x86_gamma()
         + x86_beta()
         + x86("mov",    "r14d", FR(_.x86_scratch_off + 4))
         + x86_omega();
}
