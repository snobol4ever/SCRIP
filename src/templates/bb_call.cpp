#include <string>
#include <string.h>
#include <stdint.h>
#include <cstdio>
#include <cstdlib>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "ast.h"
#include "../runtime/builtins/gen.h"
extern DESCR_t POWER_fn(DESCR_t, DESCR_t);
extern DESCR_t rt_num_arith(DESCR_t, DESCR_t, int);
extern DESCR_t rt_call_arr_gen(const char *, DESCR_t *, int, int64_t *);
void rt_write_any_nl(DESCR_t d);
int  rt_proc_is_registered(const char *name);
int  rt_builtin_is_known(const char *name);
int  rt_builtin_is_generator(const char *name);
int  bb_slot_get(IR_t * nd);
int  bb_node_id(IR_t * nd);
int  bb_varslot_peek(const char * name);
int  is_global(const char * name);
DESCR_t rt_call_arr(const char * fn, DESCR_t * args, int nargs);
int64_t rt_gvar_get_int(const char * name);
extern int g_gva_active;
int gva_index_of(const char * name);
DESCR_t rt_gvar_get_descr(const char * name);
DESCR_t rt_call_named_proc(const char * name, DESCR_t * args, int nargs);
DESCR_t rt_call_named_proc_sl(const char * name, DESCR_t * args, int nargs, void * sl);
DESCR_t rt_call_proc_direct(long idx, DESCR_t * args, int nargs);
int  rt_proc_index_of(const char * name);
int  rt_proc_frame_nslots(const char * name);
int  rt_proc_decl_level(const char * name);
uint64_t rt_proc_byref_mask(const char * name);
DESCR_t * rt_gvar_cell(const char * name);
extern int g_emit_frame_caller_dl;
DESCR_t NV_GET_fn(const char * name);
int  rt_is_truthy(DESCR_t v);
int  rt_jct_relop(DESCR_t lhs, DESCR_t rhs, int op);
DESCR_t rt_concat_parts_d(void * parts, int n);
}
#include "x86_asm.h"
static inline int zoff(const IR_t * nd) { return nd ? zls_off(nd) : -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pas_sl_setup(const char * fn) {
    int callee_dl = rt_proc_decl_level(fn);
    if (g_emit_frame_caller_dl < 0 || callee_dl < 1) return x86("mov32", "ecx", (long)0);
    int h = (g_emit_frame_caller_dl + 1) - callee_dl;
    if (h < 0) h = 0;
    std::string s = x86_frame_lea("rcx", 0);
    for (int i = 0; i < h; i++) s += x86_reg_disp32_load64("rcx", "rcx", 0);
    return s;
}
extern std::string bb_call_proc_staged_str(IR_t *);
extern std::string bb_call_write_slot_str(IR_t *);
extern std::string bb_call_write_binop_str(IR_t *);
extern std::string bb_call_write_legacy_str(IR_t *, int);
extern std::string bb_call_fn_str(IR_t *);
extern std::string bb_call_bool_str(IR_t *);
std::string marshal_call_arg(IR_t * lf, IR_graph_t * sg, int aoff, IR_t * owner, int idx);
static std::string marshal_single_call(IR_t * lf, int aoff, int lblid);
static std::string marshal_arith_rax(IR_graph_t * sg, IR_t * nd);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void arith_operands(IR_graph_t * sg, IR_t * nd, IR_t ** a, IR_t ** b) {
    *a = ir_pair_arg(nd, 0); *b = ir_pair_arg(nd, 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int arith_is_arith_binop(IR_t * nd) {
    return nd && nd->op == IR_BINOP && (IR_LIT(nd).ival == BINOP_ADD || IR_LIT(nd).ival == BINOP_SUB || IR_LIT(nd).ival == BINOP_MUL || IR_LIT(nd).ival == BINOP_DIV || IR_LIT(nd).ival == BINOP_MOD);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int lits_int_val(IR_t * nd, long long * out) {
    if (!nd || nd->op != IR_LIT_STRING || !IR_LIT(nd).sval) return 0;
    const char * p = IR_LIT(nd).sval; while (*p == ' ' || *p == '\t') p++;
    const char * e = p + strlen(p); while (e > p && (e[-1] == ' ' || e[-1] == '\t')) e--;
    if (p == e) { *out = 0; return 1; }
    const char * q = p; if (q < e && (*q == '+' || *q == '-')) q++;
    if (q == e) return 0;
    for (const char * r = q; r < e; r++) if (*r < '0' || *r > '9') return 0;
    *out = strtoll(p, NULL, 10); return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int arith_kind_ok(IR_t * nd) {
    if (!nd) return 0;
    if (nd->op == IR_LIT_STRING) { long long _v; return lits_int_val(nd, &_v); }
    if (nd->op == IR_LIT_INTEGER || nd->op == IR_OP_COUNT || nd->op == IR_OP_COUNT) return 1;
    if (nd->op == IR_VAR && IR_LIT(nd).sval) return 1;
    if (nd->op == IR_CALL || nd->op == IR_OP_COUNT || ir_is_call_kind(nd->op)) return 1;
    if (arith_is_arith_binop(nd)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int arith_is_relop(IR_t * nd) {
    return nd && nd->op == IR_BINOP && IR_LIT(nd).ival >= BINOP_LT && IR_LIT(nd).ival <= BINOP_NE;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * relop_fail_mnem(IR_t * nd) {
    return IR_LIT(nd).ival == BINOP_LT ? "jge" : IR_LIT(nd).ival == BINOP_LE ? "jg" : IR_LIT(nd).ival == BINOP_GT ? "jle"
         : IR_LIT(nd).ival == BINOP_GE ? "jl"  : IR_LIT(nd).ival == BINOP_EQ ? "jne" : "je";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string arith_opnd_a(IR_graph_t * sg, IR_t * a, int gk_lb = -1) {
    std::string s;
    if (a->op == IR_VAR && IR_LIT(a).sval) {
        char b1[80]; strtab_label(b1, sizeof b1, IR_LIT(a).sval);
        std::string slow = x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)IR_LIT(a).sval, b1) + x86("call", "rt_gvar_get_int", (uint64_t)(uintptr_t)(void *)rt_gvar_get_int);
        int k = (gk_lb >= 0 && g_gva_active) ? gva_index_of(IR_LIT(a).sval) : -1;
        if (k >= 0)
            s += x86("mov", "rdx", RDQ("rbx", k * 16)) + x86("cmp", "edx", (long)DT_I) + x86("jne", L(gk_lb)) + x86("mov", "rax", RDQ("rbx", k * 16 + 8)) + x86("jmp", L(gk_lb + 1))
               + x86("def", L(gk_lb)) + slow + x86("def", L(gk_lb + 1));
        else s += slow;
    } else if (a->op == IR_OP_COUNT) {
        s += x86_frame_lea("rax", 0);
        for (int h = 0; h < (int) IR_LIT(a).dval; h++) s += x86_reg_disp32_load64("rax", "rax", 0);
        s += x86_reg_disp32_load64("rax", "rax", 16 + (int) IR_LIT(a).ival * 16 + 8);
    } else if (a->op == IR_OP_COUNT) {
        s += x86_frame_lea("rax", 0);
        for (int h = 0; h < (int) IR_LIT(a).dval; h++) s += x86_reg_disp32_load64("rax", "rax", 0);
        s += x86_reg_disp32_load64("rax", "rax", 16 + (int) IR_LIT(a).ival * 16 + 8);
        s += x86_reg_disp32_load64("rax", "rax", 8);
    } else if (a->op == IR_LIT_INTEGER) {
        s += x86_movabs_r64("rax", (uint64_t)IR_LIT(a).ival);
    } else if (a->op == IR_LIT_STRING) {
        long long av = 0; if (!lits_int_val(a, &av)) return x86_bomb("marshal inline-arith: non-numeric string left operand");
        s += x86_movabs_r64("rax", (uint64_t)av);
    } else if (a->op == IR_CALL || a->op == IR_OP_COUNT || ir_is_call_kind(a->op)) {
        int sc = zoff(a);
        if (sc < 0) return x86_bomb("marshal inline-arith: nested call has no LOWER slot grant (TMP-ERADICATE)");
        s += marshal_single_call(a, sc, bb_node_id(a));
        s += x86_frame_load64("rax", sc + 8);
    } else if (arith_is_arith_binop(a)) {
        s += marshal_arith_rax(sg, a);
    } else return x86_bomb("marshal inline-arith: unhandled left operand shape");
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string arith_opnd_b(IR_graph_t * sg, IR_t * b, int gk_lb = -1) {
    std::string s;
    if (b->op == IR_VAR && IR_LIT(b).sval) {
        char b2[80]; strtab_label(b2, sizeof b2, IR_LIT(b).sval);
        std::string slow = x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)IR_LIT(b).sval, b2) + x86("call", "rt_gvar_get_int", (uint64_t)(uintptr_t)(void *)rt_gvar_get_int);
        slow += x86("mov", "rcx", "rax");
        int k = (gk_lb >= 0 && g_gva_active) ? gva_index_of(IR_LIT(b).sval) : -1;
        if (k >= 0)
            s += x86("mov", "rdx", RDQ("rbx", k * 16)) + x86("cmp", "edx", (long)DT_I) + x86("jne", L(gk_lb)) + x86("mov", "rcx", RDQ("rbx", k * 16 + 8)) + x86("jmp", L(gk_lb + 1))
               + x86("def", L(gk_lb)) + slow + x86("def", L(gk_lb + 1));
        else s += slow;
    } else if (b->op == IR_OP_COUNT) {
        s += x86_frame_lea("rax", 0);
        for (int h = 0; h < (int) IR_LIT(b).dval; h++) s += x86_reg_disp32_load64("rax", "rax", 0);
        s += x86_reg_disp32_load64("rax", "rax", 16 + (int) IR_LIT(b).ival * 16 + 8);
        s += x86("mov", "rcx", "rax");
    } else if (b->op == IR_OP_COUNT) {
        s += x86_frame_lea("rax", 0);
        for (int h = 0; h < (int) IR_LIT(b).dval; h++) s += x86_reg_disp32_load64("rax", "rax", 0);
        s += x86_reg_disp32_load64("rax", "rax", 16 + (int) IR_LIT(b).ival * 16 + 8);
        s += x86_reg_disp32_load64("rax", "rax", 8);
        s += x86("mov", "rcx", "rax");
    } else if (b->op == IR_LIT_INTEGER) {
        s += x86("mov", "rcx", (long)IR_LIT(b).ival);
    } else if (b->op == IR_LIT_STRING) {
        long long bv = 0; if (!lits_int_val(b, &bv)) return x86_bomb("marshal inline-arith: non-numeric string right operand");
        s += x86_movabs_r64("rcx", (uint64_t)bv);
    } else if (b->op == IR_CALL || b->op == IR_OP_COUNT || ir_is_call_kind(b->op)) {
        int sc = zoff(b);
        if (sc < 0) return x86_bomb("marshal inline-arith: nested call has no LOWER slot grant (TMP-ERADICATE)");
        s += marshal_single_call(b, sc, bb_node_id(b));
        s += x86_frame_load64("rcx", sc + 8);
    } else if (arith_is_arith_binop(b)) {
        s += marshal_arith_rax(sg, b);
        s += x86("mov", "rcx", "rax");
    } else return x86_bomb("marshal inline-arith: unhandled right operand shape");
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string marshal_arith_rax(IR_graph_t * sg, IR_t * nd) {
    IR_t * a = NULL, * b = NULL;
    arith_operands(sg, nd, &a, &b);
    if (!a || !b) return x86_bomb("marshal inline-arith: binop operands unresolved");
    int scratch = zoff(nd);
    if (scratch < 0) return x86_bomb("marshal inline-arith: binop has no LOWER slot grant (TMP-ERADICATE)");
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string marshal_single_call(IR_t * lf, int aoff, int lblid) {
    const char * nfn = IR_LIT(lf).sval ? IR_LIT(lf).sval : "";
    int nn = (int) IR_LIT(lf).ival;
    int avbase = zoff(lf);
    if (avbase < 0 || nn > (lf ? lf->n_operands : 0)) return x86_bomb("marshal_single_call: no/short LOWER slot grant (TMP-ERADICATE)");
    avbase += 16;
    int isreg = (nfn[0] && rt_proc_is_registered(nfn));
    int nmig  = isreg && rt_proc_frame_nslots(nfn) >= 0;
    const char * rsym = isreg ? (nmig ? "rt_call_named_proc_sl" : "rt_call_named_proc") : "rt_call_arr";
    std::string s;
    if (nn > 0) return x86_bomb("marshal_single_call: arg marshal gouged, no sub-graph source (TMP-ERADICATE)");
    uint64_t fptr;
    if (nmig)       { DESCR_t (*fp)(const char *, DESCR_t *, int, void *) = rt_call_named_proc_sl; fptr = (uint64_t)(uintptr_t)(void*)fp; }
    else if (isreg) { DESCR_t (*fp)(const char *, DESCR_t *, int) = rt_call_named_proc; fptr = (uint64_t)(uintptr_t)(void*)fp; }
    else            { DESCR_t (*fp)(const char *, DESCR_t *, int) = rt_call_arr;        fptr = (uint64_t)(uintptr_t)(void*)fp; }
    std::string fl = std::string(".Lcallfn") + std::to_string(lblid);
    s += x86("directive", ".section .rodata");
    s += x86("directive", (fl + ": .string \"" + nfn + "\"").c_str());
    s += x86("directive", ".section .text");
    s += x86("directive", ".intel_syntax noprefix");
    s += x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)nfn, fl.c_str());
    s += x86("lea", "rsi", FRQ(avbase));
    s += x86("mov32", "edx", (long)nn);
    s += IF(nmig, pas_sl_setup(nfn));
    s += x86("call", rsym, fptr);
    s += x86("mov", FRQ(aoff), "rax");
    s += x86("mov", FRQ(aoff + 8), "rdx");
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string marshal_call_arg(IR_t * lf, IR_graph_t * sg, int aoff, IR_t * owner, int idx) {
    if (owner && owner == _.node && idx >= 0 && idx < _.op_arg_slot_n && _.op_arg_slot[idx] >= 0) {
        int ps = _.op_arg_slot[idx];
        std::string s = x86("comment", emit_fmt("marshal arg%d = producer-box slot [zr+%d] -> [zr+%d]", idx, ps, aoff));
        s += x86_frame_load64("rax", ps)     + x86_frame_store64(aoff, "rax");
        s += x86_frame_load64("rax", ps + 8) + x86_frame_store64(aoff + 8, "rax");
        return s;
    }
    if (!lf) return std::string();
    if (lf->op == IR_LIT_INTEGER) {
        std::string s;
        s += x86("comment", emit_fmt("marshal arg%d = LIT_I -> [zr+%d]", idx, aoff));
        s += x86("mov", FRQ(aoff), (long)6);
        s += x86_movabs_r64("rax", (uint64_t)IR_LIT(lf).ival);
        s += x86_frame_store64(aoff + 8, "rax");
        return s;
    }
    if (lf->op == IR_LIT_REAL) {
        uint64_t bits; double d = IR_LIT(lf).dval; memcpy(&bits, &d, 8);
        std::string s;
        s += x86("comment", emit_fmt("marshal arg%d = LIT_F -> [zr+%d]", idx, aoff));
        s += x86("mov", FRQ(aoff), (long)7);
        s += x86_movabs_r64("rax", bits);
        s += x86_frame_store64(aoff + 8, "rax");
        return s;
    }
    if (lf->op == IR_OP_COUNT) {
        std::string s;
        s += x86("comment", emit_fmt("marshal arg%d = LIT_NUL -> [zr+%d]", idx, aoff));
        s += x86("mov", FRQ(aoff), (long)0);
        s += x86("mov", FRQ(aoff + 8), (long)0);
        return s;
    }
    if (lf->op == IR_LIT_STRING) {
        int nseal = idx * 2, nskip = idx * 2 + 1;
        std::string s;
        s += x86("comment", emit_fmt("marshal arg%d = LIT_S (string REG-RO sealed in-band) -> [zr+%d]", idx, aoff));
        s += x86("mov", FRQ(aoff), (long)1);
        s += x86_ro_load_q("rax", nseal);
        s += x86_frame_store64(aoff + 8, "rax");
        s += x86_jmp_id(nskip);
        s += x86_ro_seal_str(nseal, IR_LIT(lf).sval ? IR_LIT(lf).sval : "");
        s += x86_deflabel_id(nskip);
        return s;
    }
    if ((lf->op == IR_CALL && (IR_LIT(lf).dval == 2.0 || IR_LIT(lf).dval == 3.0)) || lf->op == IR_OP_COUNT || ir_is_call_kind(lf->op)) {
        int staged = (lf->op == IR_CALL_PROC_STAGED || lf->op == IR_PROC_GEN);
        if (owner && owner == _.node && staged && bb_slot_get(lf) >= 0) {
            int ps = bb_slot_get(lf); std::string s = x86("comment", emit_fmt("marshal arg%d = spine call-result slot [zr+%d] -> [zr+%d]", idx, ps, aoff));
            s += x86_frame_load64("rax", ps) + x86_frame_store64(aoff, "rax"); s += x86_frame_load64("rax", ps + 8) + x86_frame_store64(aoff + 8, "rax"); return s;
        } return marshal_single_call(lf, aoff, bb_node_id(lf));
    }
    if (lf->op == IR_VAR && IR_LIT(lf).sval && IR_LIT(lf).sval[0] != '&' && is_global(IR_LIT(lf).sval)) {
        char b1[80]; strtab_label(b1, sizeof b1, IR_LIT(lf).sval);
        std::string s;
        s += x86("comment", emit_fmt("marshal arg%d = global VAR NV_GET -> [zr+%d]", idx, aoff));
        s += x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)IR_LIT(lf).sval, b1);
        s += x86("call", "NV_GET_fn", (uint64_t)(uintptr_t)(void *)NV_GET_fn);
        s += x86_frame_store64(aoff, "rax");
        s += x86_frame_store64(aoff + 8, "rdx");
        return s;
    }
    {
        int is_local_var = (lf->op == IR_VAR && IR_LIT(lf).sval && IR_LIT(lf).sval[0] != '&' && !is_global(IR_LIT(lf).sval));
        int ps = is_local_var ? -1 : bb_slot_get(lf);
        if (ps < 0 && !is_local_var) ps = zoff(lf);
        if (ps >= 0) {
            std::string s = x86("comment", emit_fmt("marshal arg%d = nested producer-box slot [zr+%d] -> [zr+%d]", idx, ps, aoff));
            s += x86_frame_load64("rax", ps)     + x86_frame_store64(aoff, "rax");
            s += x86_frame_load64("rax", ps + 8) + x86_frame_store64(aoff + 8, "rax");
            return s;
        }
    }
    {
        int voff = bb_varslot_peek(IR_LIT(lf).sval ? IR_LIT(lf).sval : "");
        if (voff < 0) return x86_bomb("bb_call marshal: IR_VAR arg names a local with no LOWER-granted varslot (TE-4: grant in ir_drive_slot_assign)");
        std::string s;
        s += x86("comment", emit_fmt("marshal arg%d = varslot [zr+%d] -> [zr+%d]", idx, voff, aoff));
        s += x86_frame_load64("rax", voff)     + x86_frame_store64(aoff, "rax");
        s += x86_frame_load64("rax", voff + 8) + x86_frame_store64(aoff + 8, "rax");
        return s;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int relop_call_fail_idx(const char * fn) {
    if (!fn) return -1;
    if (!strcmp(fn, "LT")) return BINOP_LT;
    if (!strcmp(fn, "LE")) return BINOP_LE;
    if (!strcmp(fn, "GT")) return BINOP_GT;
    if (!strcmp(fn, "GE")) return BINOP_GE;
    if (!strcmp(fn, "EQ")) return BINOP_EQ;
    if (!strcmp(fn, "NE")) return BINOP_NE;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * relop_idx_fail_mnem(int op) {
    return op == BINOP_LT ? "jge" : op == BINOP_LE ? "jg" : op == BINOP_GT ? "jle" : op == BINOP_GE ? "jl" : op == BINOP_EQ ? "jne" : "je";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * relop_arg_simple_operand(IR_graph_t * sg) {
    if (!sg || !sg->entry) return NULL;
    IR_t * e = sg->entry;
    if (e->γ.node && e->γ.node->op != IR_SUCCEED && e->γ.node->op != IR_FAIL) return NULL;
    return arith_kind_ok(e) ? e : NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_call_relop_inline_str(IR_t * pBB, const char * fn, IR_graph_t ** subs, int relop) {
    x86_begin();
    int resoff = zoff(_.node);
    if (resoff < 0) return x86_alpha() + x86_bomb("bb_call_relop_inline: no LOWER slot grant (TMP-ERADICATE)");
    IR_t * a = relop_arg_simple_operand(subs[0]);
    IR_t * b = relop_arg_simple_operand(subs[1]);
    int scratch = resoff + 16;
    std::string s = x86_alpha()
        + x86("comment", emit_fmt("BOX CALL %s(...) inline integer relop [four-port, FAIL->ω]", fn));
    s += arith_opnd_a(NULL, a, 0);
    s += x86_frame_store64(scratch, "rax");
    s += arith_opnd_b(NULL, b, 2);
    s += x86("mov", FRQ(resoff), (long)DT_SNUL);
    s += x86("mov", FRQ(resoff + 8), (long)0);
    s += x86_frame_load64("rax", scratch);
    s += x86("cmp", "rax", "rcx");
    s += x86_omega(relop_idx_fail_mnem(relop));
    s += x86_gamma();
    s += x86_beta();
    s += x86_omega();
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_call_byname_str(IR_t * pBB) {
    if (!PLATFORM_X86) return std::string();
    const char * fn   = _.op_sval ? _.op_sval : "";
    int64_t      narg = _.op_ival;
    IR_graph_t ** subs = (IR_graph_t **)(intptr_t) _.op_counter;
    int resoff  = zoff(_.node);
    if (resoff < 0) return x86_alpha() + x86_bomb("bb_call_byname: no LOWER slot grant (TMP-ERADICATE)");
    if (_.node && (int)narg > _.node->n_operands) return x86_alpha() + x86_bomb("bb_call_byname: arg count exceeds LOWER grant (TMP-ERADICATE)");
    int argbase = resoff + 16;
    std::string fl = std::string(".Lbynamefn") + std::to_string((long long)_.nid);
    uint64_t fptr; { DESCR_t (*fp)(const char *, DESCR_t *, int) = rt_call_arr; fptr = (uint64_t)(uintptr_t)(void*)fp; }
    std::string s = x86_alpha()
        + x86("comment", emit_fmt("BOX CALL %s(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]", fn));
    for (int i = 0; i < (int)narg; i++)
        s += marshal_call_arg(subs && subs[i] ? subs[i]->entry : NULL, subs && subs[i] ? subs[i] : NULL, argbase + i * 16, _.node, i);
    s += x86("directive", ".section .rodata")
       + x86("directive", (fl + ": .string \"" + fn + "\"").c_str())
       + x86("directive", ".section .text") + x86("directive", ".intel_syntax noprefix");
    s += x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)fn, fl.c_str());
    s += x86("lea", "rsi", FRQ(argbase));
    s += x86("mov32", "edx", (long)narg);
    s += x86("call", "rt_call_arr", fptr);
    s += x86("mov", FRQ(resoff), "rax");
    s += x86("mov", FRQ(resoff + 8), "rdx");
    s += x86("cmp", "eax", (long)99);
    s += x86_omega("je");
    s += x86_gamma();
    s += x86_beta();
    s += x86_omega();
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_call_byname_gen_str(IR_t * pBB) {
    if (!PLATFORM_X86) return std::string();
    const char * fn   = _.op_sval ? _.op_sval : "";
    int64_t      narg = _.op_ival;
    IR_graph_t ** subs = (IR_graph_t **)(intptr_t) _.op_counter;
    int resoff  = zoff(_.node);
    if (resoff < 0) return x86_alpha() + x86_bomb("bb_call_byname_gen: no LOWER slot grant (TMP-ERADICATE)");
    if (_.node && (int)narg > _.node->n_operands) return x86_alpha() + x86_bomb("bb_call_byname_gen: arg count exceeds LOWER grant (TMP-ERADICATE)");
    int argbase = resoff + 16;
    int genoff  = resoff + 16 * (1 + (int)narg);
    std::string fl = std::string(".Lbynamegenfn") + std::to_string((long long)_.nid);
    uint64_t fptr; { DESCR_t (*fp)(const char *, DESCR_t *, int, int64_t *) = rt_call_arr_gen; fptr = (uint64_t)(uintptr_t)(void*)fp; }
    std::string s = x86_alpha()
        + x86("comment", emit_fmt("BOX CALL_GEN %s(...) -> rt_call_arr_gen by-name [four-port generator; alpha zeroes resume cell, beta re-pumps invoke with persisted cell]", fn));
    for (int i = 0; i < (int)narg; i++)
        s += marshal_call_arg(subs && subs[i] ? subs[i]->entry : NULL, subs && subs[i] ? subs[i] : NULL, argbase + i * 16, _.node, i);
    s += x86("mov", FRQ(genoff), (long)0);
    s += x86("def", L(60));
    s += x86("directive", ".section .rodata")
       + x86("directive", (fl + ": .string \"" + fn + "\"").c_str())
       + x86("directive", ".section .text") + x86("directive", ".intel_syntax noprefix");
    s += x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)fn, fl.c_str());
    s += x86("lea", "rsi", FRQ(argbase));
    s += x86("mov32", "edx", (long)narg);
    s += x86("lea", "rcx", FRQ(genoff));
    s += x86("call", "rt_call_arr_gen", fptr);
    s += x86("mov", FRQ(resoff), "rax");
    s += x86("mov", FRQ(resoff + 8), "rdx");
    s += x86("cmp", "eax", (long)99);
    s += x86_omega("je");
    s += x86_gamma();
    s += x86_beta();
    s += x86("jmp", L(60));
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * rkbool_cond_relop(IR_graph_t * cond) {
    if (!cond) return NULL;
    IR_t * p = cond->entry; int g = 0;
    while (p && g++ < 256) { if (arith_is_relop(p)) return p; if (!p->γ.node) break; p = p->γ.node; }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_jct_call(IR_t * r) {
    return r && (r->op == IR_CALL || ir_is_call_kind(r->op)) && IR_LIT(r).sval && !strncmp(IR_LIT(r).sval, "__rk_jct_", 9);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_call_bool_truthy_cond_str(IR_t * pBB) {
    if (!PLATFORM_X86) return std::string();
    IR_graph_t ** blks = (IR_graph_t **)(intptr_t) _.op_counter;
    IR_graph_t * cond = blks ? blks[0] : NULL;
    IR_t * e = cond ? cond->entry : NULL;
    if (!e) return x86_alpha() + x86_bomb("bb_call_bool_truthy: empty cond sub-graph");
    std::string s = x86_alpha()
                  + x86("comment", "BOX __rk_bool [dval=2 truthy condition -> rt_is_truthy -> branch true=γ / false=ω]");
    if (e->op == IR_LIT_INTEGER) {
        s += x86("mov32", "edi", (long)6) + x86_movabs_r64("rsi", (uint64_t)IR_LIT(e).ival);
    } else if (e->op == IR_LIT_STRING) {
        s += x86("mov32", "edi", (long)1) + x86_ro_load_q("rsi", 0) + x86_jmp_id(1) + x86_ro_seal_str(0, IR_LIT(e).sval ? IR_LIT(e).sval : "") + x86_deflabel_id(1);
    } else if (e->op == IR_VAR && IR_LIT(e).sval) {
        int voff = bb_varslot_peek(IR_LIT(e).sval);
        if (voff < 0) return x86_alpha() + x86_bomb("bb_call_bool_truthy: IR_VAR cond names a local with no LOWER-granted varslot (TE-4: grant in ir_drive_slot_assign)");
        s += x86_frame_load64("rdi", voff) + x86_frame_load64("rsi", voff + 8);
    } else {
        return x86_alpha() + x86_bomb("bb_call_bool_truthy: unhandled cond entry kind");
    }
    return s + x86("call", "rt_is_truthy", (uint64_t)(uintptr_t)(void *)rt_is_truthy)
             + x86("test", "eax", "eax") + x86_omega("je") + x86_gamma() + x86_beta() + x86_omega();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_call_bool_jct_cond_str(IR_t * pBB) {
    if (!PLATFORM_X86) return std::string();
    IR_graph_t ** blks = (IR_graph_t **)(intptr_t) _.op_counter;
    IR_graph_t * cond = blks ? blks[0] : NULL;
    IR_t * relnd = rkbool_cond_relop(cond);
    if (!relnd) return x86_alpha() + x86_bomb("bb_call_bool_jct: no relop in cond sub-graph");
    IR_t * ra = NULL, * rb = NULL; arith_operands(cond, relnd, &ra, &rb);
    if (!ra || !rb) return x86_alpha() + x86_bomb("bb_call_bool_jct: relop operands unresolved");
    int lhs_slot = (zoff(_.node) >= 0) ? zoff(_.node) + 16 : -1;
    if (lhs_slot < 0) return x86_alpha() + x86_bomb("bb_call_bool_jct: no LOWER slot grant (TMP-ERADICATE)");
    if (_.node->n_operands < 2) return x86_alpha() + x86_bomb("bb_call_bool_jct: grant narrower than 2 slots (TMP-ERADICATE)");
    int rhs_slot = lhs_slot + 16;
    std::string s = x86_alpha()
                  + x86("comment", "BOX __rk_bool [dval=2 junction relop -> rt_jct_relop -> branch true=γ / false=ω]");
    s += marshal_call_arg(ra, cond, lhs_slot, NULL, 0);
    s += marshal_call_arg(rb, cond, rhs_slot, NULL, 1);
    s += x86_frame_load64("rdi", lhs_slot) + x86_frame_load64("rsi", lhs_slot + 8);
    s += x86_frame_load64("rdx", rhs_slot) + x86_frame_load64("rcx", rhs_slot + 8);
    s += x86("mov32", "r8d", (long)IR_LIT(relnd).ival);
    return s + x86("call", "rt_jct_relop", (uint64_t)(uintptr_t)(void *)rt_jct_relop)
             + x86("test", "eax", "eax") + x86_omega("je") + x86_gamma() + x86_beta() + x86_omega();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_call_bool_cond_str(IR_t * pBB) {
    if (!PLATFORM_X86) return std::string();
    IR_graph_t ** blks = (IR_graph_t **)(intptr_t) _.op_counter;
    IR_graph_t * cond = blks ? blks[0] : NULL;
    IR_t * relnd = rkbool_cond_relop(cond);
    if (!relnd) return bb_call_bool_truthy_cond_str(pBB);
    IR_t * ra = NULL, * rb = NULL; arith_operands(cond, relnd, &ra, &rb);
    if (!ra || !rb) return x86_alpha() + x86_bomb("bb_call_bool_cond: relop operands unresolved");
    if (is_jct_call(ra) || is_jct_call(rb)) return bb_call_bool_jct_cond_str(pBB);
    if (!arith_kind_ok(ra) || !arith_kind_ok(rb)) return x86_alpha() + x86_bomb("bb_call_bool_cond: relop operands unhandled");
    int scratch = zoff(relnd);
    if (scratch < 0) return x86_alpha() + x86_bomb("bb_call_bool_cond: relop has no LOWER slot grant (TMP-ERADICATE)");
    return x86_alpha()
         + x86("comment", "BOX __rk_bool [dval=2 relop condition -> branch true=γ / false=ω]")
         + arith_opnd_a(cond, ra) + x86_frame_store64(scratch, "rax")
         + arith_opnd_b(cond, rb) + x86_frame_load64("rax", scratch)
         + x86("cmp", "rax", "rcx")
         + x86_omega(relop_fail_mnem(relnd))
         + x86_gamma()
         + x86_beta()
         + x86_omega();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_call(IR_t * pBB) {
    if (!PLATFORM_X86) return std::string();
    switch (_.op_call_route) {
        case CALL_ROUTE_BYNAME:        return bb_call_byname_str(pBB);
        case CALL_ROUTE_BYNAME_GEN:    return bb_call_byname_gen_str(pBB);
        case CALL_ROUTE_RK_BOOL_COND:  return bb_call_bool_cond_str(pBB);
        case CALL_ROUTE_DVAL2_BOMB:    return x86_alpha() + x86_bomb("CALL dval=2 descr-chain arm aborted per LANGUAGE-BLIND rule");
        case CALL_ROUTE_PROC_STAGED:   return bb_call_proc_staged_str(pBB);
        case CALL_ROUTE_RK_BOOL_SLOT:  return bb_call_bool_str(pBB);
        case CALL_ROUTE_WRITE_SLOT:    return bb_call_write_slot_str(pBB);
        case CALL_ROUTE_WRITE_BINOP:   return bb_call_write_binop_str(pBB);
        case CALL_ROUTE_WRITE_LEGACY:  return bb_call_write_legacy_str(pBB, 1);
        case CALL_ROUTE_WRITE_EMPTY:   return std::string();
        case CALL_ROUTE_FN:            return bb_call_fn_str(pBB);
        default: break;
    }
    fprintf(stderr, "[IBB] FATAL bb_call: unsupported call shape fn='%s'\n", _.op_sval ? _.op_sval : "");
    abort();
    return std::string();
}
