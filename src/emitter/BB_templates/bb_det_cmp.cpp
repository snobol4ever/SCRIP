#include <string>
#include <cstring>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
}
#include "x86_asm.h"
extern "C" int rt_pl_arith_cmp_cell_val(const char *op, void *lhs_cell, long lhs_ival, void *rhs_cell, long rhs_ival);
/*--------------------------------------------------------------------------------------------------------------------*/
static int dcm_is_arith(const char *op) {
    return op && (strcmp(op,"<")==0||strcmp(op,">")==0||strcmp(op,">=")==0||strcmp(op,"=<")==0||strcmp(op,"=:=")==0||strcmp(op,"=\\=")==0);
}
static int gz_cmp_fold(const char *op, long a, long b) {
    if (strcmp(op,"<"  )==0) return a< b;
    if (strcmp(op,">"  )==0) return a> b;
    if (strcmp(op,">=" )==0) return a>=b;
    if (strcmp(op,"=<" )==0) return a<=b;
    if (strcmp(op,"=:=")==0) return a==b;
    if (strcmp(op,"=\\=")==0) return a!=b;
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static const char *dcm_op()   { return _.op_sval ? _.op_sval : "<"; }
static const IR_t *dcm_la()   { return (const IR_t *)_.bb_ln; }
static const IR_t *dcm_ra()   { return (const IR_t *)_.bb_rn; }
static int  dcm_l_var()       { return dcm_la()->t == IR_LOGICVAR; }
static int  dcm_r_var()       { return dcm_ra()->t == IR_LOGICVAR; }
static int  dcm_lslot()       { return dcm_l_var() ? (int)dcm_la()->ival : -1; }
static int  dcm_rslot()       { return dcm_r_var() ? (int)dcm_ra()->ival : -1; }
static long dcm_l_ival()      { return (!dcm_l_var() && dcm_la()->t == IR_LIT_I) ? (long)dcm_la()->ival : 0L; }
static long dcm_r_ival()      { return (!dcm_r_var() && dcm_ra()->t == IR_LIT_I) ? (long)dcm_ra()->ival : 0L; }
static int  dcm_both_lit_i()  { return dcm_la()->t == IR_LIT_I && dcm_ra()->t == IR_LIT_I; }
static int  dcm_fold_result() { return gz_cmp_fold(dcm_op(), (long)dcm_la()->ival, (long)dcm_ra()->ival); }
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string dcm_const_fold() {
    return IF(MEDIUM_TEXT,
               x86("label", _.lbl_α)
             + x86("comment", std::string("BOX DET_CMP(") + dcm_op() + ")  [PL-GZ-8: emit-time const fold -> " + (dcm_fold_result()?"γ":"ω") + "]"))
         + x86("jmp", dcm_fold_result() ? "γ" : "ω")
         + x86("def", "β")
         + x86("jmp", "ω");
}
static std::string dcm_rt_call() {
    return IF(MEDIUM_TEXT,
               x86("label", _.lbl_α)
             + x86("comment", std::string("BOX DET_CMP(") + dcm_op() + ")  [PL-GZ-8: rt_pl_arith_cmp_cell_val]"))
         + x86_ro_load_q("rdi", 0)
         + (dcm_l_var() ? x86("mov", "rsi", FRQ(GZ_CELL_OFF(dcm_lslot()))) : x86("xor", "esi", "esi"))
         + x86("mov", "rdx", dcm_l_ival())
         + (dcm_r_var() ? x86("mov", "rcx", FRQ(GZ_CELL_OFF(dcm_rslot()))) : x86("xor", "ecx", "ecx"))
         + x86("mov", "r8", dcm_r_ival())
         + x86("call", "rt_pl_arith_cmp_cell_val", (uint64_t)(uintptr_t)(void *)rt_pl_arith_cmp_cell_val)
         + x86("test", "eax", "eax")
         + x86("je", "ω")
         + x86("jmp", "γ")
         + x86("def", "β")
         + x86("jmp", "ω")
         + x86_ro_seal_str(0, dcm_op());
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_det_cmp_str() {
    if (!PLATFORM_X86) return std::string();
    x86_begin();
    if (!dcm_is_arith(dcm_op())) return x86_bomb("bb_det_cmp: term-order cmp not yet supported in GZ");
    if (!dcm_la() || !dcm_ra()) return x86_bomb("bb_det_cmp: null operand");
    if (dcm_both_lit_i()) return dcm_const_fold();
    return dcm_rt_call();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_det_cmp(void) { bb_emit_x86(bb_det_cmp_str()); }
