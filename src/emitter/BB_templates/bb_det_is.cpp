#include <string>
#include <cmath>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
}
#include "x86_asm.h"
extern "C" int rt_pl_is_cell_int(void *lhs_cell, long val);
extern "C" int rt_pl_is_cell_arith(void *lhs_cell, void *rhs_cell, const char *op, long rhs_ival);
extern "C" int rt_pl_is_cell_bivar(void *lhs_cell, void *cell1, void *cell2, const char *op);
/*--------------------------------------------------------------------------------------------------------------------*/
static int gz_arith_const_eval(const IR_t *nd, long *out) {
    if (!nd) return 0;
    if (nd->op == IR_LIT_I) { *out = (long)IR_LIT(nd).ival; return 1; }
    const IR_t *a0 = ir_pair_arg(nd, 0), *a1 = ir_pair_arg(nd, 1);
    if (nd->op != IR_ARITH || !a0) return 0;
    const char *op = IR_LIT(nd).sval ? IR_LIT(nd).sval : "+";
    if (!a1) {
        long a = 0;
        if (!gz_arith_const_eval(a0, &a)) return 0;
        if (strcmp(op,"-")==0) { *out = -a; return 1; }
        if (strcmp(op,"+")==0) { *out =  a; return 1; }
        if (strcmp(op,"abs")==0) { *out = (a<0)?-a:a; return 1; }
        return 0;
    }
    long a = 0, b = 0;
    if (!gz_arith_const_eval(a0, &a) || !gz_arith_const_eval(a1, &b)) return 0;
    if (strcmp(op,"+")==0) { *out = a+b; return 1; }
    if (strcmp(op,"-")==0) { *out = a-b; return 1; }
    if (strcmp(op,"*")==0) { *out = a*b; return 1; }
    if (strcmp(op,"/")==0) { if (!b) return 0; *out = a/b; return 1; }
    if (strcmp(op,"mod")==0||strcmp(op,"rem")==0) { if (!b) return 0; *out = a%b; return 1; }
    return 0;
}
static int gz_arith_var_plus_const(const IR_t *nd, int *var_slot, const char **op_out, long *c_out) {
    if (!nd) return 0;
    if (nd->op == IR_LOGICVAR) { *var_slot = (int)IR_LIT(nd).ival; *op_out = NULL; *c_out = 0; return 1; }
    const IR_t *p0 = ir_pair_arg(nd, 0), *p1 = ir_pair_arg(nd, 1);
    if (nd->op != IR_ARITH || !IR_LIT(nd).sval || !p0 || !p1) return 0;
    const char *op = IR_LIT(nd).sval;
    if (strcmp(op,"+")==0||strcmp(op,"-")==0||strcmp(op,"*")==0||strcmp(op,"mod")==0||strcmp(op,"rem")==0) {
        if (p0->op == IR_LOGICVAR && p1->op == IR_LIT_I) {
            *var_slot = (int)IR_LIT(p0).ival; *op_out = op; *c_out = (long)IR_LIT(p1).ival; return 1;
        }
    }
    return 0;
}
static int gz_arith_var_bivar(const IR_t *nd, int *slot1, int *slot2, const char **op_out) {
    const IR_t *b0 = ir_pair_arg(nd, 0), *b1 = ir_pair_arg(nd, 1);
    if (!nd || nd->op != IR_ARITH || !IR_LIT(nd).sval || !b0 || !b1) return 0;
    const char *op = IR_LIT(nd).sval;
    if (strcmp(op,"+")==0||strcmp(op,"-")==0||strcmp(op,"*")==0||strcmp(op,"mod")==0||strcmp(op,"rem")==0) {
        if (b0->op == IR_LOGICVAR && b1->op == IR_LOGICVAR) {
            *slot1 = (int)IR_LIT(b0).ival; *slot2 = (int)IR_LIT(b1).ival; *op_out = op; return 1;
        }
    }
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static const IR_t *bdis_lhs() { return (const IR_t *)_.bb_ln; }
static const IR_t *bdis_rhs() { return (const IR_t *)_.bb_rn; }
static int bdis_slot()        { return (int)IR_LIT(bdis_lhs()).ival; }
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_det_is_str() {
    if (!PLATFORM_X86) return std::string();
    x86_begin();
    if (!bdis_lhs() || bdis_lhs()->op != IR_LOGICVAR) return x86_bomb("bb_det_is: lhs not LOGICVAR");
    long cval = 0;
    if (gz_arith_const_eval(bdis_rhs(), &cval))
        return x86("label", _.lbl_α)
             + x86("comment", "IR_DET_IS")
             + x86("mov", "rdi", FRQ(GZ_CELL_OFF(bdis_slot())))
             + x86("mov", "rsi", cval)
             + x86("call", "rt_pl_is_cell_int", (uint64_t)(uintptr_t)(void *)rt_pl_is_cell_int)
             + x86("test", "eax", "eax")
             + x86("je", "ω")
             + x86("jmp", "γ")
             + x86("def", "β")
             + x86("jmp", "ω");
    int rslot = -1; const char *rop = NULL; long rc = 0;
    if (gz_arith_var_plus_const(bdis_rhs(), &rslot, &rop, &rc))
        return x86("label", _.lbl_α)
             + x86("comment", "IR_DET_IS")
             + x86("mov", "rdi", FRQ(GZ_CELL_OFF(bdis_slot())))
             + x86("mov", "rsi", FRQ(GZ_CELL_OFF(rslot)))
             + x86("mov", "rdx", ROQ(0))
             + x86("mov", "rcx", rc)
             + x86("call", "rt_pl_is_cell_arith", (uint64_t)(uintptr_t)(void *)rt_pl_is_cell_arith)
             + x86("test", "eax", "eax")
             + x86("je", "ω")
             + x86("jmp", "γ")
             + x86("def", "β")
             + x86("jmp", "ω")
             + x86("ro_seal_str", 0, rop);
    int bslot1 = -1, bslot2 = -1; const char *bop = NULL;
    if (gz_arith_var_bivar(bdis_rhs(), &bslot1, &bslot2, &bop))
        return x86("label", _.lbl_α)
             + x86("comment", "IR_DET_IS")
             + x86("mov", "rdi", FRQ(GZ_CELL_OFF(bdis_slot())))
             + x86("mov", "rsi", FRQ(GZ_CELL_OFF(bslot1)))
             + x86("mov", "rdx", FRQ(GZ_CELL_OFF(bslot2)))
             + x86("mov", "rcx", ROQ(0))
             + x86("call", "rt_pl_is_cell_bivar", (uint64_t)(uintptr_t)(void *)rt_pl_is_cell_bivar)
             + x86("test", "eax", "eax")
             + x86("je", "ω")
             + x86("jmp", "γ")
             + x86("def", "β")
             + x86("jmp", "ω")
             + x86("ro_seal_str", 0, bop);
    return x86_bomb("bb_det_is: unsupported rhs shape in GZ");
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_det_is(void) { bb_emit_x86(bb_det_is_str()); }
