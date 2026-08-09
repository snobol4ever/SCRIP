#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
long rt_pat_prim_int(const char *varname);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_match_len_body() {
    /* D08 FIX (s22 CLIMB): deferred-integer arm for LEN(*var).  op_sval="*varname" (set by lower when TT_DEFER arg).
     * Calls rt_pat_prim_int(varname) at MATCH TIME (both α and β) to fetch the current variable value and coerce
     * to integer.  r14/r15/r13 are callee-saved across the C call (System V ABI); rax = return value = count or -1.
     * Negative return = coerce failed or var not numeric → omega (fail). Manual p.86: *V fetches at match time. */
    static char b[24];
    const char * _vn1 = _.op_sval ? _.op_sval + 1 : "";   /* strip leading '*' to get bare variable name */
    return x86("comment", "IR_MATCH_LEN")   /* ZD arm (A-7/ZD-5b, s25/O-7a): K=0 — scanner-register only; static arm uses op_ival (no stack read); dynamic arm reads ZOPQ(0,8) (COERCE_INTEGER predecessor cell). */
         + x86_alpha()
         + IF(_.op_sval != NULL,
               x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)_vn1, (strtab_label(b, sizeof b, _vn1), b))
             + x86("call", "rt_pat_prim_int", (uint64_t)(uintptr_t)(void *)(long (*)(const char *))rt_pat_prim_int)
             + x86("test", "rax", "rax")
             + x86_omega("js")
             + x86("mov", "ecx", "eax"))
         + IF(_.op_sval == NULL && _.op_sa >= 0 &&  _.op_zres, x86("note", ZOPN(0)) + x86("mov", "rcx", ZOPQ(0, 8)))
         + IF(_.op_sval == NULL && _.op_sa >= 0 && !_.op_zres, x86("mov", "rcx", FRQ(_.op_sa + 8)))
         + x86("mov", "eax", "r14d")
         + IF(_.op_sval != NULL || _.op_sa >= 0, x86("add", "eax", "ecx"))
         + IF(_.op_sval == NULL && _.op_sa <  0, x86("add", "eax", (long)(int)_.op_ival))
         + x86("cmp", "eax", "r15d")
         + x86_omega("jg")
         + IF(_.op_sval != NULL || _.op_sa >= 0, x86("add", "r14d", "ecx"))
         + IF(_.op_sval == NULL && _.op_sa <  0, x86("add", "r14d", (long)(int)_.op_ival))
         + x86_gamma()
         + x86_beta()
         + IF(_.op_sval != NULL,
               x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)_vn1, (strtab_label(b, sizeof b, _vn1), b))
             + x86("call", "rt_pat_prim_int", (uint64_t)(uintptr_t)(void *)(long (*)(const char *))rt_pat_prim_int)
             + x86("test", "rax", "rax")
             + x86_omega("js")
             + x86("mov", "ecx", "eax"))
         + IF(_.op_sval == NULL && _.op_sa >= 0 &&  _.op_zres, x86("note", ZOPN(0)) + x86("mov", "rcx", ZOPQ(0, 8)))
         + IF(_.op_sval == NULL && _.op_sa >= 0 && !_.op_zres, x86("mov", "rcx", FRQ(_.op_sa + 8)))
         + IF(_.op_sval != NULL || _.op_sa >= 0, x86("sub", "r14d", "ecx"))
         + IF(_.op_sval == NULL && _.op_sa <  0, x86("sub", "r14d", (long)(int)_.op_ival))
         + x86_omega();
}
std::string bb_match_len() {
    if (!PLATFORM_X86) return std::string();
    if (_.op_zres) return bb_match_len_body();
    return bb_match_len_body();
}
