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
/*--------------------------------------------------------------------------------------------------------------------*/
static int gz_lit_i_eval(const IR_t *nd, long *out) {
    if (!nd || nd->t != IR_LIT_I) return 0;
    *out = (long)nd->ival;
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
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
static std::string bb_det_cmp_str() {
    if (!PLATFORM_X86) return std::string();
    const char *op = _.op_sval ? _.op_sval : "<";
    if (!dcm_is_arith(op)) return x86_bomb("bb_det_cmp: term-order cmp not yet supported in GZ");
    const IR_t *la = (const IR_t *)_.bb_ln;
    const IR_t *ra = (const IR_t *)_.bb_rn;
    if (!la || !ra) return x86_bomb("bb_det_cmp: null operand");
    long av = 0, bv = 0;
    if (gz_lit_i_eval(la, &av) && gz_lit_i_eval(ra, &bv)) {
        int ok = gz_cmp_fold(op, av, bv);
        return IF(MEDIUM_TEXT,
                   x86("label", _.lbl_α)
                 + x86("comment", std::string("BOX DET_CMP(") + op + ")  [PL-GZ-8: emit-time const fold -> " + (ok?"γ":"ω") + "]"))
             + x86("jmp", ok ? PORT_GAMMA : PORT_OMEGA)
             + x86("def", PORT_BETA)
             + x86("jmp", PORT_OMEGA);
    }
    bool l_var = (la->t == IR_LOGICVAR), r_var = (ra->t == IR_LOGICVAR);
    int lslot = l_var ? (int)la->ival : -1;
    int rslot = r_var ? (int)ra->ival : -1;
    long l_ival = (!l_var && la->t == IR_LIT_I) ? (long)la->ival : 0L;
    long r_ival = (!r_var && ra->t == IR_LIT_I) ? (long)ra->ival : 0L;
    return IF(MEDIUM_TEXT,
               x86("label", _.lbl_α)
             + x86("comment", std::string("BOX DET_CMP(") + op + ")  [PL-GZ-8: rt_pl_arith_cmp_cell_val]"))
         + x86_ro_load_q("rdi", 0)
         + (l_var ? x86("mov", "rsi", FRQ(GZ_CELL_OFF(lslot))) : x86("xor", "esi", "esi"))
         + x86("mov", "rdx", l_ival)
         + (r_var ? x86("mov", "rcx", FRQ(GZ_CELL_OFF(rslot))) : x86("xor", "ecx", "ecx"))
         + x86("mov", "r8", r_ival)
         + x86("call", "rt_pl_arith_cmp_cell_val", (uint64_t)(uintptr_t)(void *)rt_pl_arith_cmp_cell_val)
         + x86("test", "eax", "eax")
         + x86("je", PORT_OMEGA)
         + x86("jmp", PORT_GAMMA)
         + x86("def", PORT_BETA)
         + x86("jmp", PORT_OMEGA)
         + x86_ro_seal_str(0, op);
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_det_cmp(void) { bb_emit_x86(bb_det_cmp_str()); }
