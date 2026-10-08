#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_match_len_body() {
    return x86("comment", "IR_MATCH_LEN")
         + x86_alpha()
         + IF(_.op_sval != NULL,
               x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval + 1 : ""), x86_strtab_lbl((_.op_sval ? _.op_sval + 1 : "")).c_str())
             + bb_glue_prim_int(50)
             + x86("test", "rax", "rax")
             + x86_omega("js")
             + x86("mov", "ecx", "eax"))
         + IF(_.op_sval == NULL && _.op_sa >= 0 && _.op_zres, x86("note", ZOPN(0))
                                                              + x86("mov", "rcx", ZOPQ(0, 8)))
         + IF(_.op_sval == NULL && _.op_sa >= 0 && !_.op_zres, x86("mov", "rcx", XSAQ(8)))
         + x86("mov", "eax", "r14d")
         + IF(_.op_sval != NULL || _.op_sa >= 0, x86("add", "eax", "ecx"))
         + IF(_.op_sval == NULL && _.op_sa < 0, x86("add", "eax", (long)(int)_.op_ival))
         + x86("cmp", "eax", "r15d")
         + x86_omega("jg")
         + IF(_.op_sval != NULL || _.op_sa >= 0, x86("add", "r14d", "ecx"))
         + IF(_.op_sval == NULL && _.op_sa < 0, x86("add", "r14d", (long)(int)_.op_ival))
         + x86_gamma()
         + x86_beta()
         + IF(_.op_sval != NULL,
               x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval + 1 : ""), x86_strtab_lbl((_.op_sval ? _.op_sval + 1 : "")).c_str())
             + bb_glue_prim_int(60)
             + x86("test", "rax", "rax")
             + x86_omega("js")
             + x86("mov", "ecx", "eax"))
         + IF(_.op_sval == NULL && _.op_sa >= 0 && _.op_zres, x86("note", ZOPN(0))
                                                              + x86("mov", "rcx", ZOPQ(0, 8)))
         + IF(_.op_sval == NULL && _.op_sa >= 0 && !_.op_zres, x86("mov", "rcx", XSAQ(8)))
         + IF(_.op_sval != NULL || _.op_sa >= 0, x86("sub", "r14d", "ecx"))
         + IF(_.op_sval == NULL && _.op_sa < 0, x86("sub", "r14d", (long)(int)_.op_ival))
         + x86_omega();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_len() {
    return IF(_.op_zres,
               bb_match_len_body())
         + IF(!(_.op_zres),
               bb_match_len_body());
}
