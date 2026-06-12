#include <string>
#include <string.h>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "emit.h"
#include "emit_bb.h"
#include "../../runtime/builtins/gen.h"
void rt_write_any_nl(DESCR_t d);
int  rt_proc_is_registered(const char *name);
int  rt_builtin_is_known(const char *name);
int  bb_slot_get(IR_t * nd);
int  bb_slot_alloc16(IR_t * nd);
int  bb_node_id(IR_t * nd);
int  bb_varslot(const char * name);
DESCR_t rt_call_arr(const char * fn, DESCR_t * args, int nargs);
int64_t rt_gvar_get_int(const char * name);
DESCR_t rt_gvar_get_descr(const char * name);
DESCR_t rt_call_named_proc(const char * name, DESCR_t * args, int nargs);
DESCR_t rt_call_named_proc_sl(const char * name, DESCR_t * args, int nargs, void * sl);
int  rt_proc_frame_nslots(const char * name);
int  rt_proc_decl_level(const char * name);
uint64_t rt_proc_byref_mask(const char * name);
DESCR_t * rt_gvar_cell(const char * name);
extern int g_emit_frame_caller_dl;
DESCR_t rt_proc_define(const char * spec);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string pas_sl_setup(const char * fn) {
    int callee_dl = rt_proc_decl_level(fn);
    if (g_emit_frame_caller_dl < 0 || callee_dl < 1) return x86("mov32", "ecx", (long)0);
    int h = (g_emit_frame_caller_dl + 1) - callee_dl;
    if (h < 0) h = 0;
    std::string s = x86_frame_lea("rcx", 0);
    for (int i = 0; i < h; i++) s += x86_reg_disp32_load64("rcx", "rcx", 0);
    return s;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string marshal_varparam_addr(IR_t * lf, int aoff, int idx) {
    if (!lf) return x86_bomb("marshal_varparam_addr: null arg head");
    std::string s;
    if (lf->op == IR_VAR_FRAME) {
        int hops = (int) IR_LIT(lf).dval;
        int voff = 16 + (int) IR_LIT(lf).ival * 16;
        if (MEDIUM_TEXT) s += x86("comment", emit_fmt("marshal arg%d = VAR-PARAM cell addr of frame slot=%d hops=%d -> [r12+%d]", idx, (int) IR_LIT(lf).ival, hops, aoff));
        s += x86_frame_lea("rax", 0);
        for (int h = 0; h < hops; h++) s += x86_reg_disp32_load64("rax", "rax", 0);
        s += x86_reg_disp32_lea64("rax", "rax", voff);
    } else if (lf->op == IR_VAR_FRAME_REF) {
        int hops = (int) IR_LIT(lf).dval;
        int voff = 16 + (int) IR_LIT(lf).ival * 16;
        if (MEDIUM_TEXT) s += x86("comment", emit_fmt("marshal arg%d = VAR-PARAM forward cell addr from ref slot=%d hops=%d -> [r12+%d]", idx, (int) IR_LIT(lf).ival, hops, aoff));
        s += x86_frame_lea("rax", 0);
        for (int h = 0; h < hops; h++) s += x86_reg_disp32_load64("rax", "rax", 0);
        s += x86_reg_disp32_load64("rax", "rax", voff + 8);
    } else if (lf->op == IR_VAR && IR_LIT(lf).sval) {
        if (MEDIUM_TEXT) {
            char b1[80]; strtab_label(b1, sizeof b1, IR_LIT(lf).sval);
            s += x86("comment", emit_fmt("marshal arg%d = VAR-PARAM cell addr of gvar -> [r12+%d]", idx, aoff))
               + x86("ins2", "lea", emit_fmt("rdi, [rip + %s]", b1)) + x86("ins2", "call", "rt_gvar_cell@PLT");
        } else {
            uint64_t fptr; { DESCR_t * (*fp)(const char *) = rt_gvar_cell; fptr = (uint64_t)(uintptr_t)(void *) fp; }
            s += x86_load_ro("rdi", "??", (uint64_t)(uintptr_t)IR_LIT(lf).sval) + x86_call_ro("rt_gvar_cell", fptr);
        }
    } else {
        return x86_bomb("marshal_varparam_addr: var-param arg is not a variable");
    }
    if (MEDIUM_TEXT) {
        s += x86("ins2", "mov", emit_fmt("qword ptr [r12+%d], 0", aoff));
        s += x86("ins2", "mov", emit_fmt("[r12+%d], rax", aoff + 8));
    } else {
        s += x86("mov", FRQ(aoff), (long)0);
        s += x86_frame_store64(aoff + 8, "rax");
    }
    return s;
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern std::string bb_call_proc_staged_str(IR_t *);
extern std::string bb_call_write_slot_str(IR_t *);
extern std::string bb_call_write_binop_str(IR_t *);
extern std::string bb_call_write_legacy_str(IR_t *, int);
extern std::string bb_call_userproc_str(IR_t *);
extern std::string bb_call_fn_str(IR_t *);
/*--------------------------------------------------------------------------------------------------------------------*/
std::string marshal_call_arg(IR_t * lf, IR_graph_t * sg, int aoff, IR_t * owner, int idx);
static std::string marshal_single_call(IR_t * lf, int aoff, int lblid);
static std::string marshal_arith_rax(IR_graph_t * sg, IR_t * nd);
/*--------------------------------------------------------------------------------------------------------------------*/
static void arith_operands(IR_graph_t * sg, IR_t * nd, IR_t ** a, IR_t ** b) {
    *a = ir_pair_arg(nd, 0); *b = ir_pair_arg(nd, 1);
    if (nd && (!*a || !*b) && sg) { int n = 0; IR_t * const * aux = bb_operand_aux_get(sg, nd, &n); if (aux && n >= 2) { *a = aux[0]; *b = aux[1]; } }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int arith_is_arith_binop(IR_t * nd) {
    return nd && nd->op == IR_BINOP && (IR_LIT(nd).ival == BINOP_ADD || IR_LIT(nd).ival == BINOP_SUB || IR_LIT(nd).ival == BINOP_MUL || IR_LIT(nd).ival == BINOP_DIV || IR_LIT(nd).ival == BINOP_MOD);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int arith_kind_ok(IR_t * nd) {
    if (!nd) return 0;
    if (nd->op == IR_LIT_I || nd->op == IR_VAR_FRAME || nd->op == IR_VAR_FRAME_REF) return 1;
    if (nd->op == IR_VAR && IR_LIT(nd).sval) return 1;
    if (nd->op == IR_CALL && (IR_LIT(nd).dval == 2.0 || IR_LIT(nd).dval == 3.0 || IR_LIT(nd).dval == 5.0)) return 1;
    if (arith_is_arith_binop(nd)) return 1;
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int arith_is_relop(IR_t * nd) {
    return nd && nd->op == IR_BINOP && IR_LIT(nd).ival >= BINOP_LT && IR_LIT(nd).ival <= BINOP_NE;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static const char * relop_fail_mnem(IR_t * nd) {
    return IR_LIT(nd).ival == BINOP_LT ? "jge" : IR_LIT(nd).ival == BINOP_LE ? "jg" : IR_LIT(nd).ival == BINOP_GT ? "jle" : IR_LIT(nd).ival == BINOP_GE ? "jl" : IR_LIT(nd).ival == BINOP_EQ ? "jne" : "je";
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string arith_opnd_a(IR_graph_t * sg, IR_t * a) {
    std::string s;
    if (a->op == IR_VAR && IR_LIT(a).sval) {
        if (MEDIUM_TEXT) { char b1[80]; strtab_label(b1, sizeof b1, IR_LIT(a).sval); s += x86("ins2", "lea", emit_fmt("rdi, [rip + %s]", b1)) + x86("ins2", "call", "rt_gvar_get_int@PLT"); }
        else { s += x86_load_ro("rdi", "??", (uint64_t)(uintptr_t)IR_LIT(a).sval) + x86("call", "rt_gvar_get_int", (uint64_t)(uintptr_t)(void *)rt_gvar_get_int); }
    } else if (a->op == IR_VAR_FRAME) {
        s += x86_frame_lea("rax", 0);
        for (int h = 0; h < (int) IR_LIT(a).dval; h++) s += x86_reg_disp32_load64("rax", "rax", 0);
        s += x86_reg_disp32_load64("rax", "rax", 16 + (int) IR_LIT(a).ival * 16 + 8);
    } else if (a->op == IR_VAR_FRAME_REF) {
        s += x86_frame_lea("rax", 0);
        for (int h = 0; h < (int) IR_LIT(a).dval; h++) s += x86_reg_disp32_load64("rax", "rax", 0);
        s += x86_reg_disp32_load64("rax", "rax", 16 + (int) IR_LIT(a).ival * 16 + 8);
        s += x86_reg_disp32_load64("rax", "rax", 8);
    } else if (a->op == IR_LIT_I) {
        s += x86_movabs_r64("rax", (uint64_t)IR_LIT(a).ival);
    } else if (a->op == IR_CALL && (IR_LIT(a).dval == 2.0 || IR_LIT(a).dval == 3.0 || IR_LIT(a).dval == 5.0)) {
        int sc = bb_slot_alloc16(a);
        s += marshal_single_call(a, sc, bb_node_id(a));
        s += x86_frame_load64("rax", sc + 8);
    } else if (arith_is_arith_binop(a)) {
        s += marshal_arith_rax(sg, a);
    } else return x86_bomb("marshal inline-arith: unhandled left operand shape");
    return s;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string arith_opnd_b(IR_graph_t * sg, IR_t * b) {
    std::string s;
    if (b->op == IR_VAR && IR_LIT(b).sval) {
        if (MEDIUM_TEXT) { char b2[80]; strtab_label(b2, sizeof b2, IR_LIT(b).sval); s += x86("ins2", "lea", emit_fmt("rdi, [rip + %s]", b2)) + x86("ins2", "call", "rt_gvar_get_int@PLT"); }
        else { s += x86_load_ro("rdi", "??", (uint64_t)(uintptr_t)IR_LIT(b).sval) + x86("call", "rt_gvar_get_int", (uint64_t)(uintptr_t)(void *)rt_gvar_get_int); }
        s += x86("mov", "rcx", "rax");
    } else if (b->op == IR_VAR_FRAME) {
        s += x86_frame_lea("rax", 0);
        for (int h = 0; h < (int) IR_LIT(b).dval; h++) s += x86_reg_disp32_load64("rax", "rax", 0);
        s += x86_reg_disp32_load64("rax", "rax", 16 + (int) IR_LIT(b).ival * 16 + 8);
        s += x86("mov", "rcx", "rax");
    } else if (b->op == IR_VAR_FRAME_REF) {
        s += x86_frame_lea("rax", 0);
        for (int h = 0; h < (int) IR_LIT(b).dval; h++) s += x86_reg_disp32_load64("rax", "rax", 0);
        s += x86_reg_disp32_load64("rax", "rax", 16 + (int) IR_LIT(b).ival * 16 + 8);
        s += x86_reg_disp32_load64("rax", "rax", 8);
        s += x86("mov", "rcx", "rax");
    } else if (b->op == IR_LIT_I) {
        s += x86("mov", "rcx", (long)IR_LIT(b).ival);
    } else if (b->op == IR_CALL && (IR_LIT(b).dval == 2.0 || IR_LIT(b).dval == 3.0 || IR_LIT(b).dval == 5.0)) {
        int sc = bb_slot_alloc16(b);
        s += marshal_single_call(b, sc, bb_node_id(b));
        s += x86_frame_load64("rcx", sc + 8);
    } else if (arith_is_arith_binop(b)) {
        s += marshal_arith_rax(sg, b);
        s += x86("mov", "rcx", "rax");
    } else return x86_bomb("marshal inline-arith: unhandled right operand shape");
    return s;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string marshal_arith_rax(IR_graph_t * sg, IR_t * nd) {
    IR_t * a = NULL, * b = NULL;
    arith_operands(sg, nd, &a, &b);
    if (!a || !b) return x86_bomb("marshal inline-arith: binop operands unresolved");
    int scratch = bb_slot_alloc16(nd);
    std::string s = arith_opnd_a(sg, a);
    s += x86_frame_store64(scratch, "rax");
    s += arith_opnd_b(sg, b);
    s += x86_frame_load64("rax", scratch);
    switch ((int)IR_LIT(nd).ival) {
    case BINOP_ADD: s += x86("add",  "rax", "rcx"); break;
    case BINOP_SUB: s += x86("sub",  "rax", "rcx"); break;
    case BINOP_MUL: s += x86("imul", "rax", "rcx"); break;
    case BINOP_DIV: s += x86("cqo") + x86("idiv", "rcx"); break;
    case BINOP_MOD: s += x86("cqo") + x86("idiv", "rcx") + x86("mov", "rax", "rdx"); break;
    }
    return s;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string marshal_single_call(IR_t * lf, int aoff, int lblid) {
    const char * nfn = IR_LIT(lf).sval ? IR_LIT(lf).sval : "";
    int nn = (int) IR_LIT(lf).ival;
    IR_graph_t ** nsubs = (IR_graph_t **)(intptr_t) IR_EXEC(lf).counter;
    int avbase = (nn > 0) ? bb_slot_alloc16(nsubs[0]->entry) : bb_slot_alloc16(lf);
    for (int j = 1; j < nn; j++) bb_slot_alloc16(nsubs[j]->entry);
    int isreg = (nfn[0] && rt_proc_is_registered(nfn));
    int nmig  = isreg && rt_proc_frame_nslots(nfn) >= 0;
    uint64_t nbrm = isreg ? rt_proc_byref_mask(nfn) : 0;
    const char * rsym = isreg ? (nmig ? "rt_call_named_proc_sl" : "rt_call_named_proc") : "rt_call_arr";
    std::string s;
    for (int j = 0; j < nn; j++) s += ((nbrm >> j) & 1ull) ? marshal_varparam_addr(nsubs[j]->entry, avbase + j * 16, j) : marshal_call_arg(nsubs[j]->entry, nsubs[j], avbase + j * 16, lf, j);
    if (MEDIUM_TEXT) {
        std::string fl = emit_fmt(".Lcallfn%d", lblid);
        s += x86("directive", ".section .rodata")
           + x86("directive", (fl + ": .string \"" + nfn + "\"").c_str())
           + x86("directive", ".section .text") + x86("directive", ".intel_syntax noprefix");
        s += x86("ins2", "lea", emit_fmt("rdi, [rip+%s]", fl.c_str()));
        s += x86("ins2", "lea", emit_fmt("rsi, [r12+%d]", avbase));
        s += x86("ins2", "mov", emit_fmt("edx, %d", nn));
        if (nmig) s += pas_sl_setup(nfn);
        s += x86("ins2", "call", emit_fmt("%s@PLT", rsym));
        s += x86("ins2", "mov", emit_fmt("[r12+%d], rax", aoff));
        s += x86("ins2", "mov", emit_fmt("[r12+%d], rdx", aoff + 8));
    } else if (MEDIUM_BINARY) {
        uint64_t fptr;
        if (nmig)       { DESCR_t (*fp)(const char *, DESCR_t *, int, void *) = rt_call_named_proc_sl; fptr = (uint64_t)(uintptr_t)(void*)fp; }
        else if (isreg) { DESCR_t (*fp)(const char *, DESCR_t *, int) = rt_call_named_proc; fptr = (uint64_t)(uintptr_t)(void*)fp; }
        else            { DESCR_t (*fp)(const char *, DESCR_t *, int) = rt_call_arr;        fptr = (uint64_t)(uintptr_t)(void*)fp; }
        s += x86_load_ro("rdi", "??", (uint64_t)(uintptr_t)nfn);
        s += x86_frame_lea("rsi", avbase);
        s += x86("mov32", "edx", (long)nn);
        if (nmig) s += pas_sl_setup(nfn);
        s += x86_call_ro(rsym, fptr);
        s += x86_frame_store64(aoff, "rax");
        s += x86_frame_store64(aoff + 8, "rdx");
    }
    return s;
}
/*--------------------------------------------------------------------------------------------------------------------*/
std::string marshal_call_arg(IR_t * lf, IR_graph_t * sg, int aoff, IR_t * owner, int idx) {
    if (!lf) return std::string();
    if (owner && owner == _.node && idx >= 0 && idx < _.op_arg_slot_n && _.op_arg_slot[idx] >= 0) {
        int ps = _.op_arg_slot[idx];
        std::string s = IF(MEDIUM_TEXT, x86("comment", emit_fmt("marshal arg%d = producer-box slot [r12+%d] -> [r12+%d]", idx, ps, aoff)));
        s += x86_frame_load64("rax", ps)     + x86_frame_store64(aoff, "rax");
        s += x86_frame_load64("rax", ps + 8) + x86_frame_store64(aoff + 8, "rax");
        return s;
    }
    {
        int ps = bb_slot_get(lf);
        if (ps >= 0) {
            std::string s = IF(MEDIUM_TEXT, x86("comment", emit_fmt("marshal arg%d = nested producer-box slot [r12+%d] -> [r12+%d]", idx, ps, aoff)));
            s += x86_frame_load64("rax", ps)     + x86_frame_store64(aoff, "rax");
            s += x86_frame_load64("rax", ps + 8) + x86_frame_store64(aoff + 8, "rax");
            return s;
        }
    }
    if (g_gvar_flat_chain) {
        IR_t * fin = lf; int gg = 0;
        while (fin && fin->γ.node && fin->γ.node->op != IR_SUCCEED && fin->γ.node->op != IR_FAIL && gg++ < 256) fin = fin->γ.node;
        int fin_arith = arith_is_arith_binop(fin);
        IR_t * fa = NULL, * fb = NULL;
        if (fin_arith) arith_operands(sg, fin, &fa, &fb);
        if (fin && fin != lf && fin_arith && arith_kind_ok(fa) && arith_kind_ok(fb)) {
            std::string s;
            if (MEDIUM_TEXT) s += x86("comment", emit_fmt("marshal arg%d = inline gvar-arith subexpr -> [r12+%d]", idx, aoff));
            s += marshal_arith_rax(sg, fin);
            s += x86("mov", FRQ(aoff), (long)6);
            s += x86_frame_store64(aoff + 8, "rax");
            return s;
        }
        IR_t * relnd = NULL;
        { IR_t * rp = lf; int rg = 0; while (rp && rg++ < 256) { if (arith_is_relop(rp)) { relnd = rp; break; } if (!rp->γ.node || rp->γ.node->op == IR_SUCCEED || rp->γ.node->op == IR_FAIL) break; rp = rp->γ.node; } }
        if (relnd && fin && fin->op == IR_LIT_I && IR_LIT(fin).ival == 1 && relnd->ω.node && relnd->ω.node->op == IR_LIT_I && IR_LIT(relnd->ω.node).ival == 0) {
            IR_t * ra = NULL, * rb = NULL;
            arith_operands(sg, relnd, &ra, &rb);
            if (ra && rb && arith_kind_ok(ra) && arith_kind_ok(rb)) {
                if (idx * 2 + 1 >= X86_INTERNAL_MAX) return x86_bomb("marshal boolean-relop: arg index exceeds internal label capacity");
                std::string s;
                if (MEDIUM_TEXT) s += x86("comment", emit_fmt("marshal arg%d = boolean relop value INTVAL(0/1) -> [r12+%d]", idx, aoff));
                int scratch = bb_slot_alloc16(relnd);
                s += arith_opnd_a(sg, ra);
                s += x86_frame_store64(scratch, "rax");
                s += arith_opnd_b(sg, rb);
                s += x86("mov", FRQ(aoff), (long)6);
                s += x86_frame_load64("rax", scratch);
                s += x86("cmp", "rax", "rcx");
                if (MEDIUM_TEXT) {
                    std::string tl = emit_fmt(".Lbrel%d_f", bb_node_id(relnd));
                    std::string el = emit_fmt(".Lbrel%d_e", bb_node_id(relnd));
                    s += x86("ins2", relop_fail_mnem(relnd), tl.c_str());
                    s += x86("mov", FRQ(aoff + 8), (long)1);
                    s += x86("ins2", "jmp", el.c_str());
                    s += x86("Lins1", emit_fmt("%s:", tl.c_str()), "");
                    s += x86("mov", FRQ(aoff + 8), (long)0);
                    s += x86("Lins1", emit_fmt("%s:", el.c_str()), "");
                } else {
                    s += x86_jcc_id(relop_fail_mnem(relnd), idx * 2);
                    s += x86("mov", FRQ(aoff + 8), (long)1);
                    s += x86_jmp_id(idx * 2 + 1);
                    s += x86_deflabel_id(idx * 2);
                    s += x86("mov", FRQ(aoff + 8), (long)0);
                    s += x86_deflabel_id(idx * 2 + 1);
                }
                return s;
            }
        }
    }
    if (lf->op == IR_CALL && (IR_LIT(lf).dval == 2.0 || IR_LIT(lf).dval == 3.0 || IR_LIT(lf).dval == 5.0)) return marshal_single_call(lf, aoff, bb_node_id(lf));
    std::string s;
    if (lf->op == IR_LIT_I) {
        s += IF(MEDIUM_TEXT, x86("comment", emit_fmt("marshal arg%d = LIT_I -> [r12+%d]", idx, aoff)));
        s += x86("mov", FRQ(aoff), (long)6);
        s += x86_movabs_r64("rax", (uint64_t)IR_LIT(lf).ival);
        s += x86_frame_store64(aoff + 8, "rax");
    } else if (lf->op == IR_LIT_F) {
        uint64_t bits; double d = IR_LIT(lf).dval; memcpy(&bits, &d, 8);
        s += IF(MEDIUM_TEXT, x86("comment", emit_fmt("marshal arg%d = LIT_F -> [r12+%d]", idx, aoff)));
        s += x86("mov", FRQ(aoff), (long)7);
        s += x86_movabs_r64("rax", bits);
        s += x86_frame_store64(aoff + 8, "rax");
    } else if (lf->op == IR_LIT_NUL) {
        s += IF(MEDIUM_TEXT, x86("comment", emit_fmt("marshal arg%d = LIT_NUL -> [r12+%d]", idx, aoff)));
        s += x86("mov", FRQ(aoff), (long)0);
        s += x86("mov", FRQ(aoff + 8), (long)0);
    } else if (lf->op == IR_LIT_S) {
        int nseal = idx * 2, nskip = idx * 2 + 1;
        s += IF(MEDIUM_TEXT, x86("comment", emit_fmt("marshal arg%d = LIT_S (string REG-RO sealed in-band) -> [r12+%d]", idx, aoff)));
        s += x86("mov", FRQ(aoff), (long)1);
        s += x86_ro_load_q("rax", nseal);
        s += x86_frame_store64(aoff + 8, "rax");
        s += x86_jmp_id(nskip);
        s += x86_ro_seal_str(nseal, IR_LIT(lf).sval ? IR_LIT(lf).sval : "");
        s += x86_deflabel_id(nskip);
    } else {
        int voff = bb_varslot(IR_LIT(lf).sval ? IR_LIT(lf).sval : "");
        s += IF(MEDIUM_TEXT, x86("comment", emit_fmt("marshal arg%d = varslot [r12+%d] -> [r12+%d]", idx, voff, aoff)));
        s += x86_frame_load64("rax", voff)     + x86_frame_store64(aoff, "rax");
        s += x86_frame_load64("rax", voff + 8) + x86_frame_store64(aoff + 8, "rax");
    }
    return s;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_call_gvar_define_str(IR_t * pBB) {
    if (!PLATFORM_X86) return std::string();
    int64_t narg = _.op_ival;
    IR_graph_t ** subs = (IR_graph_t **)(intptr_t) _.op_counter;
    IR_t * spec = (narg > 0 && subs && subs[0]) ? subs[0]->entry : NULL;
    const char * specstr = (spec && spec->op == IR_LIT_S && IR_LIT(spec).sval) ? IR_LIT(spec).sval : "";
    if (MEDIUM_TEXT) {
        std::string fl = emit_fmt(".Ldefspec%d", g_flat_node_id++);
        std::string s = x86("label", _.lbl_α)
            + x86("comment", "BOX IR_CALL DEFINE(spec) -> rt_proc_define [single-shot success]")
            + x86("directive", ".section .rodata")
            + x86("directive", (fl + ": .string \"" + specstr + "\"").c_str())
            + x86("directive", ".section .text") + x86("directive", ".intel_syntax noprefix");
        s += x86("ins2", "lea", emit_fmt("rdi, [rip+%s]", fl.c_str()));
        s += x86("ins2", "call", "rt_proc_define@PLT");
        s += x86("ins2", "jmp", _.lbl_γ);
        s += x86("Lins1", emit_fmt("%s:", _.lbl_β), "");
        s += x86("ins2", "jmp", _.lbl_ω);
        return s;
    }
    if (MEDIUM_BINARY) {
        uint64_t fptr; { DESCR_t (*fp)(const char *) = rt_proc_define; fptr = (uint64_t)(uintptr_t)(void*)fp; }
        std::string s;
        s += x86_load_ro("rdi", "??", (uint64_t)(uintptr_t)specstr);
        s += x86_call_ro("rt_proc_define", fptr);
        s += x86("jmp", PORT_GAMMA);
        s += x86("def", PORT_BETA);
        s += x86("jmp", PORT_OMEGA);
        return s;
    }
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_call_gvar_userproc_str(IR_t * pBB) {
    if (!PLATFORM_X86) return std::string();
    const char * fn   = _.op_sval ? _.op_sval : "";
    int64_t      narg = _.op_ival;
    IR_graph_t ** subs = (IR_graph_t **)(intptr_t) _.op_counter;
    int resoff  = bb_slot_alloc16(_.node);
    int argbase = (narg > 0) ? bb_slot_alloc16(subs[0]->entry) : resoff;
    for (int i = 1; i < (int)narg; i++) bb_slot_alloc16(subs[i]->entry);
    if (MEDIUM_TEXT) {
        std::string s = x86("label", _.lbl_α)
            + x86("comment", emit_fmt("BOX IR_CALL %s(...) -> rt_call_named_proc [four-port, FAIL->ω.node]", fn));
        uint64_t brm = rt_proc_byref_mask(fn);
        for (int i = 0; i < (int)narg; i++)
            s += ((brm >> i) & 1ull) ? marshal_varparam_addr(subs[i]->entry, argbase + i * 16, i) : marshal_call_arg(subs[i]->entry, subs[i], argbase + i * 16, _.node, i);
        std::string fl = emit_fmt(".Lprocfn%d", g_flat_node_id++);
        s += x86("directive", ".section .rodata")
           + x86("directive", (fl + ": .string \"" + fn + "\"").c_str())
           + x86("directive", ".section .text") + x86("directive", ".intel_syntax noprefix");
        s += x86("ins2", "lea", emit_fmt("rdi, [rip+%s]", fl.c_str()));
        s += x86("ins2", "lea", emit_fmt("rsi, [r12+%d]", argbase));
        s += x86("ins2", "mov", emit_fmt("edx, %lld", (long long)narg));
        int mig = rt_proc_frame_nslots(fn) >= 0;
        if (mig) s += pas_sl_setup(fn);
        s += x86("ins2", "call", mig ? "rt_call_named_proc_sl@PLT" : "rt_call_named_proc@PLT");
        s += x86("ins2", "mov", emit_fmt("[r12+%d], rax", resoff));
        s += x86("ins2", "mov", emit_fmt("[r12+%d], rdx", resoff + 8));
        s += x86("ins2", "cmp", "eax, 99");
        s += x86("ins2", "je", _.lbl_ω);
        s += x86("ins2", "jmp", _.lbl_γ);
        s += x86("Lins1", emit_fmt("%s:", _.lbl_β), "");
        s += x86("ins2", "jmp", _.lbl_ω);
        return s;
    }
    if (MEDIUM_BINARY) {
        std::string s;
        uint64_t brm = rt_proc_byref_mask(fn);
        for (int i = 0; i < (int)narg; i++)
            s += ((brm >> i) & 1ull) ? marshal_varparam_addr(subs[i]->entry, argbase + i * 16, i) : marshal_call_arg(subs[i]->entry, subs[i], argbase + i * 16, _.node, i);
        int mig = rt_proc_frame_nslots(fn) >= 0;
        uint64_t fptr;
        if (mig) { DESCR_t (*fp)(const char *, DESCR_t *, int, void *) = rt_call_named_proc_sl; fptr = (uint64_t)(uintptr_t)(void*)fp; }
        else     { DESCR_t (*fp)(const char *, DESCR_t *, int) = rt_call_named_proc; fptr = (uint64_t)(uintptr_t)(void*)fp; }
        s += x86_load_ro("rdi", "??", (uint64_t)(uintptr_t)fn);
        s += x86_frame_lea("rsi", argbase);
        s += x86("mov32", "edx", (long)narg);
        if (mig) s += pas_sl_setup(fn);
        s += x86_call_ro(mig ? "rt_call_named_proc_sl" : "rt_call_named_proc", fptr);
        s += x86_frame_store64(resoff, "rax");
        s += x86_frame_store64(resoff + 8, "rdx");
        s += x86("cmp", "eax", (long)99);
        s += x86("je", PORT_OMEGA);
        s += x86("jmp", PORT_GAMMA);
        s += x86("def", PORT_BETA);
        s += x86("jmp", PORT_OMEGA);
        return s;
    }
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_call_byname_str(IR_t * pBB) {
    if (!PLATFORM_X86) return std::string();
    const char * fn   = _.op_sval ? _.op_sval : "";
    int64_t      narg = _.op_ival;
    IR_graph_t ** subs = (IR_graph_t **)(intptr_t) _.op_counter;
    int resoff  = bb_slot_alloc16(_.node);
    int argbase = (narg > 0 && subs && subs[0]) ? bb_slot_alloc16(subs[0]->entry) : resoff;
    for (int i = 1; i < (int)narg; i++) if (subs && subs[i]) bb_slot_alloc16(subs[i]->entry);
    if (MEDIUM_TEXT) {
        std::string s = x86("label", _.lbl_α)
            + x86("comment", emit_fmt("BOX IR_CALL %s(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]", fn));
        for (int i = 0; i < (int)narg; i++)
            s += marshal_call_arg(subs && subs[i] ? subs[i]->entry : NULL, subs && subs[i] ? subs[i] : NULL, argbase + i * 16, _.node, i);
        std::string fl = emit_fmt(".Lbynamefn%d", g_flat_node_id++);
        s += x86("directive", ".section .rodata")
           + x86("directive", (fl + ": .string \"" + fn + "\"").c_str())
           + x86("directive", ".section .text") + x86("directive", ".intel_syntax noprefix");
        s += x86("ins2", "lea", emit_fmt("rdi, [rip+%s]", fl.c_str()));
        s += x86("ins2", "lea", emit_fmt("rsi, [r12+%d]", argbase));
        s += x86("ins2", "mov", emit_fmt("edx, %lld", (long long)narg));
        s += x86("ins2", "call", "rt_call_arr@PLT");
        s += x86("ins2", "mov", emit_fmt("[r12+%d], rax", resoff));
        s += x86("ins2", "mov", emit_fmt("[r12+%d], rdx", resoff + 8));
        s += x86("ins2", "cmp", "eax, 99");
        s += x86("ins2", "je", _.lbl_ω);
        s += x86("ins2", "jmp", _.lbl_γ);
        s += x86("Lins1", emit_fmt("%s:", _.lbl_β), "");
        s += x86("ins2", "jmp", _.lbl_ω);
        return s;
    }
    if (MEDIUM_BINARY) {
        std::string s;
        for (int i = 0; i < (int)narg; i++)
            s += marshal_call_arg(subs && subs[i] ? subs[i]->entry : NULL, subs && subs[i] ? subs[i] : NULL, argbase + i * 16, _.node, i);
        uint64_t fptr; { DESCR_t (*fp)(const char *, DESCR_t *, int) = rt_call_arr; fptr = (uint64_t)(uintptr_t)(void*)fp; }
        s += x86_load_ro("rdi", "??", (uint64_t)(uintptr_t)fn);
        s += x86_frame_lea("rsi", argbase);
        s += x86("mov32", "edx", (long)narg);
        s += x86_call_ro("rt_call_arr", fptr);
        s += x86_frame_store64(resoff, "rax");
        s += x86_frame_store64(resoff + 8, "rdx");
        s += x86("cmp", "eax", (long)99);
        s += x86("je", PORT_OMEGA);
        s += x86("jmp", PORT_GAMMA);
        s += x86("def", PORT_BETA);
        s += x86("jmp", PORT_OMEGA);
        return s;
    }
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_call_str(IR_t * pBB) {
    if (!PLATFORM_X86) return std::string();
    const char * fn   = _.op_sval ? _.op_sval : "";
    int64_t      narg = _.op_ival;
    IR_t       * a0   = ir_call_arg(_.node, 0);
    if (g_descr_flat_chain && _.op_dval == 2.0) return x86_bomb("IR_CALL dval=2 descr-chain arm aborted per LANGUAGE-BLIND rule");
    if (g_gvar_flat_chain && _.op_dval == 5.0) return bb_call_gvar_define_str(pBB);
    if (g_gvar_flat_chain && (_.op_dval == 2.0 || _.op_dval == 3.0) && fn && rt_proc_is_registered(fn)) return bb_call_gvar_userproc_str(pBB);
    if (g_descr_flat_chain && fn && rt_proc_is_registered(fn) && _.op_dval == 3.0) return bb_call_proc_staged_str(pBB);
    if (g_gvar_flat_chain && _.op_dval == 3.0 && fn && fn[0] && !rt_proc_is_registered(fn)) return bb_call_byname_str(pBB);
    if (g_gvar_flat_chain && _.op_dval == 2.0 && fn && fn[0] && !rt_proc_is_registered(fn) && !rt_builtin_is_known(fn)) return bb_call_byname_str(pBB);
    if (g_descr_flat_chain && fn && (!strcmp(fn, "write")) && narg == 1 && a0) {
        int off = bb_slot_get(a0);
        if (off >= 0) return bb_call_write_slot_str(pBB);
    }
    int is_write_strlit  = (fn && !strcmp(fn, "write") && narg == 1 && a0 && a0->op == IR_LIT_S && IR_LIT(a0).sval);
    int is_write_intexpr = (fn && !strcmp(fn, "write") && narg == 1 && a0 &&
                            (a0->op == IR_BINOP || a0->op == IR_LIT_I || a0->op == IR_TO || a0->op == IR_TO_BY || a0->op == IR_ALT || a0->op == IR_BINOP_GEN || a0->op == IR_VAR ||
                             a0->op == IR_NEG || a0->op == IR_POS || a0->op == IR_NONNULL || a0->op == IR_NULL_TEST || a0->op == IR_NOT || a0->op == IR_SIZE
                                || a0->op == IR_CALL || a0->op == IR_CASE || a0->op == IR_FIELD_GET || a0->op == IR_LIST_BANG || a0->op == IR_LIMIT || a0->op == IR_IDX));
    int arg_is_any = (a0 && (a0->op == IR_VAR || a0->op == IR_BINOP || a0->op == IR_BINOP_GEN || a0->op == IR_ALT ||
                             a0->op == IR_NEG || a0->op == IR_POS || a0->op == IR_NONNULL || a0->op == IR_NULL_TEST || a0->op == IR_NOT || a0->op == IR_SIZE ||
                             a0->op == IR_CALL || a0->op == IR_CASE || a0->op == IR_FIELD_GET || a0->op == IR_LIST_BANG || a0->op == IR_LIMIT || a0->op == IR_IDX ||
                             (a0->op == IR_TO_BY && IR_LIT(a0).sval && IR_LIT(a0).sval[0] == 'r')));
    int is_userproc = (fn && rt_proc_is_registered(fn) && !is_write_strlit && !is_write_intexpr);
    if (is_userproc) return bb_call_userproc_str(pBB);
    int is_builtin  = (fn && rt_builtin_is_known(fn)       && !is_write_strlit && !is_write_intexpr);
    if (is_builtin)  return bb_call_fn_str(pBB);
    if (!is_write_strlit && !is_write_intexpr) {
        fprintf(stderr, "[IBB] FATAL bb_call: unsupported call shape fn='%s' narg=%lld a0=%d\n",
                fn, (long long)narg, a0 ? (int)a0->op : -1);
        abort();
    }
    if (is_write_intexpr) {
        int arg_is_ro_binop = (a0 && (a0->op == IR_BINOP || a0->op == IR_TO || a0->op == IR_TO_BY));
        if (arg_is_ro_binop) return bb_call_write_binop_str(pBB);
        return bb_call_write_legacy_str(pBB, arg_is_any);
    }
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_call(IR_t * pBB) { bb_emit_x86(bb_call_str(pBB)); }
