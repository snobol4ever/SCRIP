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
extern "C" int rt_pl_is_cell(void *lhs_cell, void *rhs_node);
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
static std::string bb_det_is_str() {
    if (!PLATFORM_X86) return std::string();
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
             + x86("test32", "eax", "eax")
             + x86("je", PORT_OMEGA)
             + x86("jmp", PORT_GAMMA)
             + x86("def", PORT_BETA)
             + x86("jmp", PORT_OMEGA);
    }
    return IF(MEDIUM_TEXT,
               x86("label", _.lbl_α)
             + x86("comment", "BOX DET_IS(X is Expr)  [PL-GZ-8: m3-only node-ptr path; m4 declines non-const rhs]"))
         + IF(MEDIUM_BINARY,
               x86("mov", "rdi", FRQ(GZ_CELL_OFF(slot)))
             + x86("mov", "rsi", (uint64_t)(uintptr_t)(void *)rhs)
             + x86("call", "rt_pl_is_cell", (uint64_t)(uintptr_t)(void *)rt_pl_is_cell)
             + x86("test32", "eax", "eax")
             + x86("je", PORT_OMEGA)
             + x86("jmp", PORT_GAMMA)
             + x86("def", PORT_BETA)
             + x86("jmp", PORT_OMEGA))
         + IF(MEDIUM_TEXT, x86_bomb("bb_det_is: non-const rhs not yet supported in m4 TEXT"));
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_det_is(void) { bb_emit_x86(bb_det_is_str()); }
