#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
}
#include "x86_asm.h"
extern "C" int rt_unify_terms(void *l, void *r);
extern "C" int rt_pl_unify_cell_const(void *cell_term, int kind, long ival, const char *sval);
extern "C" int rt_pl_unify_cell_float(void *cell_term, double dval);
extern "C" int rt_pl_unify_struct_gz(void *frame, const void *lnd, const void *rnd);
/*--------------------------------------------------------------------------------------------------------------------*/
static int    bcu_lk()  { return _.bb_lk; }
static int    bcu_rk()  { return _.bb_rk; }
static long   bcu_li()  { return (long)_.bb_li; }
static long   bcu_ri()  { return (long)_.bb_ri; }
static const IR_t *bcu_ln() { return (const IR_t *)_.bb_ln; }
static const IR_t *bcu_rn() { return (const IR_t *)_.bb_rn; }
static const char *bcu_ls() { return bcu_ln() ? IR_LIT(bcu_ln()).sval : (const char *)0; }
static const char *bcu_rs() { return bcu_rn() ? IR_LIT(bcu_rn()).sval : (const char *)0; }
static std::string bcu_tail() { return x86("test", "eax", "eax") + x86("je", "ω") + x86("jmp", "γ") + x86("def", "β") + x86("jmp", "ω"); }
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bcu_struct() {
    return IF(MEDIUM_TEXT, x86("label", _.lbl_α) + x86("comment", "BOX CELL_UNIFY struct-unify  [PL-GZ-9a: rdi=r12 GZ frame, rsi/rdx=sealed IR_t* trees [rip+disp]; rt_pl_unify_struct_gz builds Term*s from frame cells then unifies]"))
         + x86("mov", "rdi", "r12")
         + x86_ro_load_q("rsi", 0)
         + x86_ro_load_q("rdx", 1)
         + x86("call", "rt_pl_unify_struct_gz", (uint64_t)(uintptr_t)(void *)rt_pl_unify_struct_gz)
         + bcu_tail()
         + x86_ro_seal_q(0, (uint64_t)(uintptr_t)(const void *)bcu_ln())
         + x86_ro_seal_q(1, (uint64_t)(uintptr_t)(const void *)bcu_rn());
}
static std::string bcu_self() {
    return IF(MEDIUM_TEXT, x86("label", _.lbl_α) + x86("comment", "BOX CELL_UNIFY cell=cell self — vacuous success  [PL-GZ-3 frame-cell unify, x86() self-encoding]"))
         + x86("jmp", "γ") + x86("def", "β") + x86("jmp", "ω");
}
static std::string bcu_cell_cell() {
    return IF(MEDIUM_TEXT, x86("label", _.lbl_α) + x86("comment", "BOX CELL_UNIFY cell↔cell  [PL-GZ-3: Term* from [ζ+off], ONE rt VALUE call, binding trailed in rt]"))
         + x86("mov", "rdi", FRQ(GZ_CELL_OFF((int)bcu_li())))
         + x86("mov", "rsi", FRQ(GZ_CELL_OFF((int)bcu_ri())))
         + x86("call", "rt_unify_terms", (uint64_t)(uintptr_t)(void *)rt_unify_terms)
         + bcu_tail();
}
static int bcu_fslot() {
    if (bcu_lk() == IR_LOGICVAR && bcu_rk() == IR_LIT_F && bcu_rn()) return (int)bcu_li();
    if (bcu_rk() == IR_LOGICVAR && bcu_lk() == IR_LIT_F && bcu_ln()) return (int)bcu_ri();
    return -1;
}
static double bcu_fval() {
    if (bcu_lk() == IR_LOGICVAR && bcu_rk() == IR_LIT_F && bcu_rn()) return IR_LIT(bcu_rn()).dval;
    return bcu_ln() ? IR_LIT(bcu_ln()).dval : 0.0;
}
static std::string bcu_cell_float() {
    return IF(MEDIUM_TEXT, x86("label", _.lbl_α) + x86("comment", "BOX CELL_UNIFY cell↔float  [PL-GZ: float sealed RO; rt_pl_unify_cell_float(cell,dval)]"))
         + x86("mov", "rdi", FRQ(GZ_CELL_OFF(bcu_fslot())))
         + x86("movsd", "xmm0", F64(bcu_fval()))
         + x86("call", "rt_pl_unify_cell_float", (uint64_t)(uintptr_t)(void *)rt_pl_unify_cell_float)
         + bcu_tail();
}
static int bcu_cslot() {
    if (bcu_lk() == IR_LOGICVAR && (bcu_rk() == IR_ATOM || bcu_rk() == IR_LIT_I)) return (int)bcu_li();
    if (bcu_rk() == IR_LOGICVAR && (bcu_lk() == IR_ATOM || bcu_lk() == IR_LIT_I)) return (int)bcu_ri();
    return -1;
}
static int bcu_ck() { return (bcu_lk() == IR_LOGICVAR) ? bcu_rk() : bcu_lk(); }
static long bcu_ci() { return (bcu_lk() == IR_LOGICVAR) ? bcu_ri() : bcu_li(); }
static const char *bcu_cs() { return (bcu_lk() == IR_LOGICVAR) ? bcu_rs() : bcu_ls(); }
static std::string bcu_cell_const() {
    return IF(MEDIUM_TEXT, x86("label", _.lbl_α) + x86("comment", "BOX CELL_UNIFY cell↔const  [PL-GZ-3: const sealed RO in-box [rip+disp], binding trailed in rt]"))
         + x86("mov", "rdi", FRQ(GZ_CELL_OFF(bcu_cslot())))
         + x86("mov", "esi", (long)bcu_ck())
         + x86("mov", "rdx", bcu_ci())
         + (bcu_cs() ? x86_ro_load_q("rcx", 0) : x86("mov", "ecx", (long)0))
         + x86("call", "rt_pl_unify_cell_const", (uint64_t)(uintptr_t)(void *)rt_pl_unify_cell_const)
         + bcu_tail()
         + (bcu_cs() ? x86_ro_seal_str(0, bcu_cs()) : std::string());
}
static int bcu_const_eq() {
    if (bcu_lk() != bcu_rk()) return 0;
    if (bcu_lk() == IR_LIT_F) return bcu_ln() && bcu_rn() && IR_LIT(bcu_ln()).dval == IR_LIT(bcu_rn()).dval;
    if (bcu_lk() == IR_LIT_I) return bcu_li() == bcu_ri();
    return bcu_ls() && bcu_rs() && strcmp(bcu_ls(), bcu_rs()) == 0;
}
static std::string bcu_const_const_eq() {
    return IF(MEDIUM_TEXT, x86("label", _.lbl_α) + x86("comment", "BOX CELL_UNIFY const=const — folded vacuous success at emit time  [PL-GZ-3b fact head-unify]"))
         + x86("jmp", "γ") + x86("def", "β") + x86("jmp", "ω");
}
static std::string bcu_const_const_ne() {
    return IF(MEDIUM_TEXT, x86("label", _.lbl_α) + x86("comment", "BOX CELL_UNIFY const≠const — folded fail at emit time  [PL-GZ-3b fact head-unify]"))
         + x86("jmp", "ω") + x86("def", "β") + x86("jmp", "ω");
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_cell_unify_str() {
    if (!PLATFORM_X86) return std::string();
    x86_begin();
    if (bcu_lk() == IR_STRUCT || bcu_rk() == IR_STRUCT) return bcu_struct();
    if (bcu_lk() == IR_LOGICVAR && bcu_rk() == IR_LOGICVAR && bcu_li() == bcu_ri()) return bcu_self();
    if (bcu_lk() == IR_LOGICVAR && bcu_rk() == IR_LOGICVAR) return bcu_cell_cell();
    if (bcu_fslot() >= 0) return bcu_cell_float();
    if (bcu_cslot() >= 0) return bcu_cell_const();
    if ((bcu_lk() == IR_ATOM || bcu_lk() == IR_LIT_I || bcu_lk() == IR_LIT_F) &&
        (bcu_rk() == IR_ATOM || bcu_rk() == IR_LIT_I || bcu_rk() == IR_LIT_F))
        return bcu_const_eq() ? bcu_const_const_eq() : bcu_const_const_ne();
    return x86_bomb("bb_cell_unify: unadmitted operand shape reached the emitter");
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_cell_unify(void) { bb_emit_x86(bb_cell_unify_str()); }
