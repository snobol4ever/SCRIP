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
    if (nd->t == IR_LIT_I) { *out = (long)nd->ival; return 1; }
    if (nd->t != IR_ARITH || !nd->α) return 0;
    const char *op = nd->sval ? nd->sval : "+";
    if (!nd->β) {
        long a = 0;
        if (!gz_arith_const_eval(nd->α, &a)) return 0;
        if (strcmp(op,"-")==0) { *out = -a; return 1; }
        if (strcmp(op,"+")==0) { *out =  a; return 1; }
        if (strcmp(op,"abs")==0) { *out = (a<0)?-a:a; return 1; }
        return 0;
    }
    long a = 0, b = 0;
    if (!gz_arith_const_eval(nd->α, &a) || !gz_arith_const_eval(nd->β, &b)) return 0;
    if (strcmp(op,"+")==0) { *out = a+b; return 1; }
    if (strcmp(op,"-")==0) { *out = a-b; return 1; }
    if (strcmp(op,"*")==0) { *out = a*b; return 1; }
    if (strcmp(op,"/")==0) { if (!b) return 0; *out = a/b; return 1; }
    if (strcmp(op,"mod")==0||strcmp(op,"rem")==0) { if (!b) return 0; *out = a%b; return 1; }
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int gz_arith_var_plus_const(const IR_t *nd, int *var_slot, const char **op_out, long *c_out) {
    if (!nd) return 0;
    if (nd->t == IR_LOGICVAR) { *var_slot = (int)nd->ival; *op_out = NULL; *c_out = 0; return 1; }
    if (nd->t != IR_ARITH || !nd->sval || !nd->α || !nd->β) return 0;
    const char *op = nd->sval;
    if (strcmp(op,"+")==0||strcmp(op,"-")==0||strcmp(op,"*")==0||strcmp(op,"mod")==0||strcmp(op,"rem")==0) {
        if (nd->α->t == IR_LOGICVAR && nd->β->t == IR_LIT_I) {
            *var_slot = (int)nd->α->ival; *op_out = op; *c_out = (long)nd->β->ival; return 1;
        }
    }
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int gz_arith_var_bivar(const IR_t *nd, int *slot1, int *slot2, const char **op_out) {
    if (!nd || nd->t != IR_ARITH || !nd->sval || !nd->α || !nd->β) return 0;
    const char *op = nd->sval;
    if (strcmp(op,"+")==0||strcmp(op,"-")==0||strcmp(op,"*")==0||strcmp(op,"mod")==0||strcmp(op,"rem")==0) {
        if (nd->α->t == IR_LOGICVAR && nd->β->t == IR_LOGICVAR) {
            *slot1 = (int)nd->α->ival; *slot2 = (int)nd->β->ival; *op_out = op; return 1;
        }
    }
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_det_is_str() {
    if (!PLATFORM_X86) return std::string();
    x86_begin();
    const IR_t *lhs = (const IR_t *)_.bb_ln;
    const IR_t *rhs = (const IR_t *)_.bb_rn;
    if (!lhs || lhs->t != IR_LOGICVAR) return x86_bomb("bb_det_is: lhs not LOGICVAR");
    int slot = (int)lhs->ival;
    long cval = 0;
    if (gz_arith_const_eval(rhs, &cval)) {
        return IF(MEDIUM_TEXT,
                   x86("label", _.lbl_α)
                 + x86("comment", "BOX DET_IS(X is const)  [PL-GZ-8: emit-time eval -> rt_pl_is_cell_int]"))
             + x86("mov", "rdi", FRQ(GZ_CELL_OFF(slot)))
             + x86("mov", "rsi", cval)
             + x86("call", "rt_pl_is_cell_int", (uint64_t)(uintptr_t)(void *)rt_pl_is_cell_int)
             + x86("test", "eax", "eax")
             + x86("je", PORT_OMEGA)
             + x86("jmp", PORT_GAMMA)
             + x86("def", PORT_BETA)
             + x86("jmp", PORT_OMEGA);
    }
    int rslot = -1; const char *rop = NULL; long rc = 0;
    if (gz_arith_var_plus_const(rhs, &rslot, &rop, &rc)) {
        return IF(MEDIUM_TEXT,
                   x86("label", _.lbl_α)
                 + x86("comment", "BOX DET_IS(X is Y op C)  [PL-GZ-8: rt_pl_is_cell_arith]"))
             + x86("mov", "rdi", FRQ(GZ_CELL_OFF(slot)))
             + x86("mov", "rsi", FRQ(GZ_CELL_OFF(rslot)))
             + x86_ro_load_q("rdx", 0)
             + x86("mov", "rcx", rc)
             + x86("call", "rt_pl_is_cell_arith", (uint64_t)(uintptr_t)(void *)rt_pl_is_cell_arith)
             + x86("test", "eax", "eax")
             + x86("je", PORT_OMEGA)
             + x86("jmp", PORT_GAMMA)
             + x86("def", PORT_BETA)
             + x86("jmp", PORT_OMEGA)
             + x86_ro_seal_str(0, rop);
    }
    int bslot1 = -1, bslot2 = -1; const char *bop = NULL;
    if (gz_arith_var_bivar(rhs, &bslot1, &bslot2, &bop)) {
        return IF(MEDIUM_TEXT,
                   x86("label", _.lbl_α)
                 + x86("comment", "BOX DET_IS(X is Y op Z)  [PL-GZ-9b: rt_pl_is_cell_bivar]"))
             + x86("mov", "rdi", FRQ(GZ_CELL_OFF(slot)))
             + x86("mov", "rsi", FRQ(GZ_CELL_OFF(bslot1)))
             + x86("mov", "rdx", FRQ(GZ_CELL_OFF(bslot2)))
             + x86_ro_load_q("rcx", 0)
             + x86("call", "rt_pl_is_cell_bivar", (uint64_t)(uintptr_t)(void *)rt_pl_is_cell_bivar)
             + x86("test", "eax", "eax")
             + x86("je", PORT_OMEGA)
             + x86("jmp", PORT_GAMMA)
             + x86("def", PORT_BETA)
             + x86("jmp", PORT_OMEGA)
             + x86_ro_seal_str(0, bop);
    }
    return x86_bomb("bb_det_is: unsupported rhs shape in GZ");
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_det_is(void) { bb_emit_x86(bb_det_is_str()); }
