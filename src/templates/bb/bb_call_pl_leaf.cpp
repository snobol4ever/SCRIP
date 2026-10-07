#include <string>
#include <string.h>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "descr.h"
DESCR_t rt_pl_ax_cold(DESCR_t *, int, const char *);
DESCR_t rt_pl_cmp_cold(DESCR_t *, int, const char *);
DESCR_t rt_pl_is_cold(DESCR_t *, int);
DESCR_t rt_pl_zguard_cold(DESCR_t *, int);
DESCR_t rt_pl_anum_cold(DESCR_t *, int);
DESCR_t rt_pl_type_cold(DESCR_t *, int, const char *);
DESCR_t rt_pl_atop_cold(DESCR_t *, int, const char *);
int rtx_pl_unify(DESCR_t *, DESCR_t *);
void rt_pl_tr_refuse(const char *);
#include "rt/gc_heap.h"
}
#include "rt/rt_pl_trail.h"
#include "x86_asm.h"
#include "bb_pl_cell.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
enum { PLK_NONE = 0, PLK_AX, PLK_CMP, PLK_IS, PLK_TYPE, PLK_ATOP, PLK_ZGUARD, PLK_ANUM, PLK_MKC, PLK_DBDECLS, PLK_UNIFY };
enum { PLR_ANY = 0, PLR_TEXT, PLR_NUM, PLR_INT0, PLR_UNB, PLR_UNB_OR_INT0, PLR_UNB_OR_TEXT, PLR_COMP, PLR_NONVAR, PLR_TEXT_OR_NUM, PLR_INTCODE };
static const int PL_L_COLD = 190, PL_L_OK = 180, PL_L_FAIL = 195;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_ax_arity(const char * op) {
    static const char * const b2[] = { "add", "sub", "mul", "div", "idiv", "divf", "mod", "rem", "fpow", "pow", "min", "max", "gcd", "xor", "shr", "shl", "band", "bor", 0 };
    static const char * const b1[] = { "neg", "pos", "abs", "sign", "trunc", "intg", "flt", "floor", "ceil", "round", "sqrt", "msb", "bnot", "sin", "cos", "atan", "log", "exp", "fip", "ffp", 0 };
    static const char * const b0[] = { "pi", "e", 0 };
    for (int i = 0; b2[i]; i++) if (!strcmp(op, b2[i])) return 2;
    for (int i = 0; b1[i]; i++) if (!strcmp(op, b1[i])) return 1;
    for (int i = 0; b0[i]; i++) if (!strcmp(op, b0[i])) return 0;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_leaf_kind(const char * fn, int narg, const char ** op) {
    static const char * const cmps[] = { "lt", "gt", "le", "ge", "eq", "ne", 0 };
    static const char * const types[] = { "var", "nonvar", "atom", "number", "integer", "float", "atomic", "compound", "callable", "string", 0 };
    *op = 0;
    if (!fn || fn[0] != '$') return PLK_NONE;
    if (!strcmp(fn, "$mkc")) return narg >= 1 ? PLK_MKC : PLK_NONE;
    if (!strcmp(fn, "$db_decls")) return narg >= 1 ? PLK_DBDECLS : PLK_NONE;
    if (!strcmp(fn, "$unify")) return narg == 2 ? PLK_UNIFY : PLK_NONE;
    if (!strcmp(fn, "$ax_zguard")) return narg == 2 ? PLK_ZGUARD : PLK_NONE;
    if (!strcmp(fn, "$ax_eguard")) return PLK_NONE;
    if (!strncmp(fn, "$ax_", 4)) { *op = fn + 4; return pl_ax_arity(*op) == narg ? PLK_AX : PLK_NONE; }
    if (!strncmp(fn, "$cmp_", 5) && narg == 2) { for (int i = 0; cmps[i]; i++) if (!strcmp(fn + 5, cmps[i])) { *op = fn + 5; return PLK_CMP; } return PLK_NONE; }
    if (!strcmp(fn, "$is_v")) return narg == 2 ? PLK_IS : PLK_NONE;
    if (!strncmp(fn, "$atop_", 6) && narg == 2) { for (int i = 0; cmps[i]; i++) if (!strcmp(fn + 6, cmps[i])) { *op = fn + 6; return PLK_ATOP; } return PLK_NONE; }
    if (!strcmp(fn, "$pl_anum_guard2")) return narg == 3 ? PLK_ANUM : PLK_NONE;
    if (!strcmp(fn, "$pl_anum_guard3")) return narg == 4 ? PLK_ANUM : PLK_NONE;
    if (!strcmp(fn, "$pl_anum_guard5")) return narg == 6 ? PLK_ANUM : PLK_NONE;
    if (narg == 1) for (int i = 0; types[i]; i++) if (!strcmp(fn + 1, types[i])) { *op = fn + 1; return PLK_TYPE; }
    return PLK_NONE;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_leaf_inline_known(const char * fn, int narg) {
    const char * op = 0;
    return pl_leaf_kind(fn, narg, &op) != PLK_NONE;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pl_opstr(const char * reg, const char * str) {
    std::string fl = std::string(".L") + x86_boxkind() + "_plop" + std::to_string(g_flat_node_id++);
    return x86("directive", ".section .rodata")
         + x86("directive", (fl + ": .string \"" + str + "\"").c_str())
         + x86("directive", ".section .text")
         + x86("directive", ".intel_syntax noprefix")
         + x86("lea", reg, "[rip + __]", (uint64_t)(uintptr_t)str, fl.c_str());
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pl_deref(const char * r, int b, int l_unb, int l_cold, int uniform) {
    std::string s = x86("def", L(b))
                  + x86("mov", "eax", RDD(r, 0))
                  + x86("cmp", "al", (long)DT_PLVAR)
                  + x86("je", L(b + 1))
                  + x86("cmp", "al", (long)DT_N)
                  + x86("jne", L(b + 2))
                  + x86("mov", "ecx", RDD(r, 4))
                  + x86("cmp", "ecx", (long)1)
                  + x86("jne", L(l_cold))
                  + x86("def", L(b + 1))
                  + x86("mov", "rcx", RDQ(r, 8))
                  + x86("test", "rcx", "rcx")
                  + x86("jz", L(l_cold))
                  + x86("cmp", "rcx", r)
                  + x86("je", L(l_unb))
                  + x86("mov", r, "rcx")
                  + x86("jmp", L(b))
                  + x86("def", L(b + 2));
    if (uniform) s += x86("cmp", "al", (long)DT_SNUL)
                    + x86("je", L(l_unb))
                    + x86("cmp", "al", (long)DT_FAIL)
                    + x86("je", L(l_unb));
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pl_cold_call(const char * sym, void * fp, int narg, int argbase, int resoff, const char * opstr) {
    std::string s = x86("lea", "rdi", FRQ(argbase))
                  + x86("mov32", "esi", (long)narg);
    if (opstr) s += pl_opstr("rdx", opstr);
    s += x86("call", sym, (uint64_t)(uintptr_t)fp)
       + x86("mov", FRQ(resoff), "rax")
       + x86("mov", FRQ(resoff + 8), "rdx") + x86_rt_gc_poll()
       + x86("cmp", "al", (long)DT_FAIL) + x86_omega("je");
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pl_ok_store(int resoff) { return x86("def", L(PL_L_OK))
                                                  + x86("mov", FRQ(resoff), (long)DT_I)
                                                  + x86("mov", FRQ(resoff + 8), (long)1) + x86_gamma(); }
static std::string pl_fail_store(int resoff) { return x86("def", L(PL_L_FAIL))
                                                    + x86("mov", FRQ(resoff), (long)DT_FAIL)
                                                    + x86("mov", FRQ(resoff + 8), (long)0) + x86_omega(); }
static std::string pl_tail() { return x86_gamma() + x86_beta() + x86_omega(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pl_arm_ax(const char * op, int narg, int argbase, int resoff) {
    int add = !strcmp(op, "add"), sub = !strcmp(op, "sub"), mul = !strcmp(op, "mul"), idiv = !strcmp(op, "idiv"), mod = !strcmp(op, "mod"), rem = !strcmp(op, "rem");
    std::string s = x86("comment", (std::string("PL-R7 $ax_") + op + ": the small-integer case inline, rt_pl_ax_cold (a value service) behind it").c_str());
    if (narg == 2 && (add || sub || mul || idiv || mod || rem)) {
        s += x86("lea", "rdi", FRQ(argbase)) + pl_deref("rdi", 100, PL_L_COLD, PL_L_COLD, 0)
           + x86("cmp", "al", (long)DT_I)
           + x86("jne", L(PL_L_COLD))
           + x86("mov", "rdx", RDQ("rdi", 8))
           + x86("lea", "rsi", FRQ(argbase + 16)) + pl_deref("rsi", 110, PL_L_COLD, PL_L_COLD, 0)
           + x86("cmp", "al", (long)DT_I)
           + x86("jne", L(PL_L_COLD))
           + x86("mov", "rcx", RDQ("rsi", 8))
           + x86("mov", "rax", "rdx");
        if (add) s += x86("add", "rax", "rcx")
                    + x86("jo", L(PL_L_COLD));
        if (sub) s += x86("sub", "rax", "rcx")
                    + x86("jo", L(PL_L_COLD));
        if (mul) s += x86("imul", "rax", "rcx")
                    + x86("jo", L(PL_L_COLD));
        if (idiv || mod || rem) s += x86("test", "rcx", "rcx")
                                   + x86("jz", L(PL_L_COLD))
                                   + x86("cmp", "rcx", (long)-1)
                                   + x86("je", L(PL_L_COLD))
                                   + x86("cqo")
                                   + x86("idiv", "rcx");
        if (rem) s += x86("mov", "rax", "rdx");
        if (mod) s += x86("mov", "rax", "rdx")
                    + x86("test", "rax", "rax")
                    + x86("jz", L(120))
                    + x86("mov", "rdx", "rax")
                    + x86("xor", "rdx", "rcx")
                    + x86("jns", L(120))
                    + x86("add", "rax", "rcx")
                    + x86("def", L(120));
        s += x86("mov", FRQ(resoff), (long)DT_I)
           + x86("mov", FRQ(resoff + 8), "rax") + x86_gamma();
    }
    s += x86("def", L(PL_L_COLD)) + pl_cold_call("rt_pl_ax_cold", (void *)rt_pl_ax_cold, narg, argbase, resoff, op) + pl_tail();
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * pl_cmp_fail_jcc(const char * op) {
    return !strcmp(op, "lt") ? "jge" : !strcmp(op, "gt") ? "jle" : !strcmp(op, "le") ? "jg" : !strcmp(op, "ge") ? "jl" : !strcmp(op, "eq") ? "jne" : "je";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pl_arm_cmp(const char * op, int argbase, int resoff) {
    std::string s = x86("comment", (std::string("PL-R7 $cmp_") + op + ": two small integers compared inline, rt_pl_cmp_cold behind it").c_str());
    s += x86("lea", "rdi", FRQ(argbase)) + pl_deref("rdi", 100, PL_L_COLD, PL_L_COLD, 0)
       + x86("cmp", "al", (long)DT_I)
       + x86("jne", L(PL_L_COLD))
       + x86("mov", "rdx", RDQ("rdi", 8))
       + x86("lea", "rsi", FRQ(argbase + 16)) + pl_deref("rsi", 110, PL_L_COLD, PL_L_COLD, 0)
       + x86("cmp", "al", (long)DT_I)
       + x86("jne", L(PL_L_COLD))
       + x86("mov", "rcx", RDQ("rsi", 8))
       + x86("cmp", "rdx", "rcx")
       + x86(pl_cmp_fail_jcc(op), L(PL_L_FAIL))
       + x86("mov", FRQ(resoff), (long)DT_I)
       + x86("mov", FRQ(resoff + 8), "rdx") + x86_gamma()
       + pl_fail_store(resoff)
       + x86("def", L(PL_L_COLD)) + pl_cold_call("rt_pl_cmp_cold", (void *)rt_pl_cmp_cold, 2, argbase, resoff, op) + pl_tail();
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pl_arm_is(int argbase, int resoff) {
    std::string s = x86("comment", "PL-R7 $is_v: a numeric value skips the evaluator, rt_pl_is_cold evaluates the rest, the bind is the general-unify leaf's");
    s += x86("lea", "rdi", FRQ(argbase + 16)) + pl_deref("rdi", 100, PL_L_COLD, PL_L_COLD, 0)
       + x86("cmp", "al", (long)DT_I)
       + x86("je", L(120))
       + x86("cmp", "al", (long)DT_R)
       + x86("je", L(120))
       + x86("cmp", "al", (long)DT_BIG)
       + x86("je", L(120))
       + x86("jmp", L(PL_L_COLD))
       + x86("def", L(120))
       + x86("mov", "rax", RDQ("rdi", 0))
       + x86("mov", "rdx", RDQ("rdi", 8))
       + x86("mov", FRQ(resoff), "rax")
       + x86("mov", FRQ(resoff + 8), "rdx")
       + x86("lea", "rdi", FRQ(argbase))
       + x86("lea", "rsi", FRQ(argbase + 16))
       + x86("call", "rtx_pl_unify", (uint64_t)(uintptr_t)(void *)rtx_pl_unify)
       + x86("test", "eax", "eax") + x86_omega("jz") + x86_gamma()
       + x86("def", L(PL_L_COLD))
       + x86("lea", "rdi", FRQ(argbase))
       + x86("mov32", "esi", (long)2)
       + x86("call", "rt_pl_is_cold", (uint64_t)(uintptr_t)(void *)rt_pl_is_cold)
       + x86("mov", FRQ(argbase + 16), "rax")
       + x86("mov", FRQ(argbase + 24), "rdx")
       + x86("mov", FRQ(resoff), "rax")
       + x86("mov", FRQ(resoff + 8), "rdx") + x86_rt_gc_poll()
       + x86("cmp", "al", (long)DT_FAIL) + x86_omega("je")
       + x86("lea", "rdi", FRQ(argbase + 16))
       + x86("jmp", L(120)) + pl_tail();
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pl_tag_in(const char * tags, int l_yes) {
    std::string s;
    for (const char * p = tags; *p; p++) {
        long t = *p == 'I' ? (long)DT_I : *p == 'R' ? (long)DT_R : *p == 'B' ? (long)DT_BIG : *p == 'S' ? (long)DT_S : *p == 'A' ? (long)DT_PLATOM : (long)DT_PLREF;
        s += x86("cmp", "al", t)
           + x86("je", L(l_yes));
    }
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pl_arm_type(const char * kind, int argbase, int resoff) {
    int var = !strcmp(kind, "var"), nonvar = !strcmp(kind, "nonvar");
    const char * tags = !strcmp(kind, "atom") ? "A" : !strcmp(kind, "string") ? "S" : !strcmp(kind, "number") ? "IRB" : !strcmp(kind, "integer") ? "IB" : !strcmp(kind, "float") ? "R" : !strcmp(kind,
        "atomic") ? "SAIRB" : !strcmp(kind, "compound") ? "P" : !strcmp(kind, "callable") ? "AP" : "";
    std::string s = x86("comment", (std::string("PL-R7 $") + kind + ": deref and one tag compare inline, rt_pl_type_cold behind it").c_str());
    s += x86("lea", "rdi", FRQ(argbase)) + pl_deref("rdi", 100, 130, PL_L_COLD, 1);
    if (var) s += x86("jmp", L(PL_L_FAIL))
                + x86("def", L(130))
                + x86("jmp", L(PL_L_OK));
    else if (nonvar) s += x86("jmp", L(PL_L_OK))
                        + x86("def", L(130))
                        + x86("jmp", L(PL_L_FAIL));
    else s += pl_tag_in(tags, PL_L_OK) + x86("def", L(130))
                                       + x86("jmp", L(PL_L_FAIL));
    s += pl_ok_store(resoff) + pl_fail_store(resoff)
       + x86("def", L(PL_L_COLD)) + pl_cold_call("rt_pl_type_cold", (void *)rt_pl_type_cold, 1, argbase, resoff, kind) + pl_tail();
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pl_arm_atop(const char * op, int argbase, int resoff) {
    int eq = !strcmp(op, "eq"), ne = !strcmp(op, "ne");
    std::string s = x86("comment", (std::string("PL-R7 $atop_") + op + ": two small integers or two unbound cells decided inline, rt_pl_atop_cold behind it").c_str());
    s += x86("lea", "rdi", FRQ(argbase)) + pl_deref("rdi", 100, 130, PL_L_COLD, 1)
       + x86("cmp", "al", (long)DT_I)
       + x86("jne", L(PL_L_COLD))
       + x86("mov", "rdx", RDQ("rdi", 8))
       + x86("lea", "rsi", FRQ(argbase + 16)) + pl_deref("rsi", 110, PL_L_COLD, PL_L_COLD, 1)
       + x86("cmp", "al", (long)DT_I)
       + x86("jne", L(PL_L_COLD))
       + x86("mov", "rcx", RDQ("rsi", 8))
       + x86("cmp", "rdx", "rcx")
       + x86(pl_cmp_fail_jcc(op), L(PL_L_FAIL))
       + x86("jmp", L(PL_L_OK))
       + x86("def", L(130))
       + x86("lea", "rsi", FRQ(argbase + 16)) + pl_deref("rsi", 120, 135, PL_L_COLD, 1)
       + x86("jmp", L(PL_L_COLD))
       + x86("def", L(135));
    if (eq) s += x86("cmp", "rdi", "rsi")
               + x86("jne", L(PL_L_FAIL))
               + x86("jmp", L(PL_L_OK));
    else if (ne) s += x86("cmp", "rdi", "rsi")
                    + x86("je", L(PL_L_FAIL))
                    + x86("jmp", L(PL_L_OK));
    else s += x86("jmp", L(PL_L_COLD));
    s += pl_ok_store(resoff) + pl_fail_store(resoff)
       + x86("def", L(PL_L_COLD)) + pl_cold_call("rt_pl_atop_cold", (void *)rt_pl_atop_cold, 2, argbase, resoff, op) + pl_tail();
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pl_arm_zguard(int argbase, int resoff) {
    std::string s = x86("comment", "PL-R7 $ax_zguard: a non-zero small-integer divisor passes inline, rt_pl_zguard_cold raises the ISO ball");
    s += x86("lea", "rdi", FRQ(argbase)) + pl_deref("rdi", 100, PL_L_COLD, PL_L_COLD, 0)
       + x86("cmp", "al", (long)DT_I)
       + x86("jne", L(PL_L_COLD))
       + x86("mov", "rcx", RDQ("rdi", 8))
       + x86("test", "rcx", "rcx")
       + x86("jz", L(PL_L_COLD))
       + pl_ok_store(resoff)
       + x86("def", L(PL_L_COLD)) + pl_cold_call("rt_pl_zguard_cold", (void *)rt_pl_zguard_cold, 2, argbase, resoff, 0) + pl_tail();
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const int * pl_anum_rules(const char * name, int nargs) {
    static const int r_atom_length[] = { PLR_TEXT, PLR_UNB_OR_INT0 }, r_text_list[] = { PLR_TEXT, PLR_UNB }, r_num_list[] = { PLR_NUM, PLR_UNB }, r_number_string[] = { PLR_NUM, PLR_ANY };
    static const int r_char_code[] = { PLR_UNB, PLR_INTCODE }, r_atom_concat[] = { PLR_TEXT, PLR_TEXT, PLR_UNB_OR_TEXT }, r_atomic_concat[] = { PLR_TEXT_OR_NUM, PLR_TEXT_OR_NUM, PLR_UNB_OR_TEXT };
    static const int r_sub_atom[] = { PLR_TEXT, PLR_UNB_OR_INT0, PLR_UNB_OR_INT0, PLR_UNB_OR_INT0, PLR_UNB_OR_TEXT }, r_arg[] = { PLR_INT0, PLR_COMP, PLR_ANY }, r_functor[] = { PLR_NONVAR,
        PLR_ANY, PLR_ANY };
    if (!name) return 0;
    if (nargs == 2 && !strcmp(name, "atom_length")) return r_atom_length;
    if (nargs == 2 && (!strcmp(name, "atom_chars") || !strcmp(name, "atom_codes"))) return r_text_list;
    if (nargs == 2 && (!strcmp(name, "number_chars") || !strcmp(name, "number_codes"))) return r_num_list;
    if (nargs == 2 && !strcmp(name, "number_string")) return r_number_string;
    if (nargs == 2 && !strcmp(name, "char_code")) return r_char_code;
    if (nargs == 3 && !strcmp(name, "atom_concat")) return r_atom_concat;
    if (nargs == 3 && !strcmp(name, "atomic_concat")) return r_atomic_concat;
    if (nargs == 5 && !strcmp(name, "sub_atom")) return r_sub_atom;
    if (nargs == 3 && !strcmp(name, "arg")) return r_arg;
    if (nargs == 3 && !strcmp(name, "functor")) return r_functor;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pl_anum_arg(int rule, int aoff, int b) {
    int l_unb = b + 3, l_next = b + 4;
    std::string s = x86("lea", "rdi", FRQ(aoff)) + pl_deref("rdi", b, l_unb, PL_L_COLD, 1);
    switch (rule) {
        case PLR_TEXT: case PLR_UNB_OR_TEXT: s += pl_tag_in("SA", l_next) + x86("jmp", L(PL_L_COLD)); break;
        case PLR_NUM: s += pl_tag_in("IRB", l_next) + x86("jmp", L(PL_L_COLD)); break;
        case PLR_TEXT_OR_NUM: s += pl_tag_in("SAIRB", l_next) + x86("jmp", L(PL_L_COLD)); break;
        case PLR_INT0: case PLR_UNB_OR_INT0: s += x86("cmp", "al", (long)DT_I)
                                                + x86("jne", L(PL_L_COLD))
                                                + x86("mov", "rcx", RDQ("rdi", 8))
                                                + x86("test", "rcx", "rcx")
                                                + x86("js", L(PL_L_COLD))
                                                + x86("jmp", L(l_next)); break;
        case PLR_INTCODE: s += x86("cmp", "al", (long)DT_I)
                             + x86("jne", L(PL_L_COLD))
                             + x86("mov", "rcx", RDQ("rdi", 8))
                             + x86("cmp", "rcx", (long)0x10FFFF)
                             + x86("ja", L(PL_L_COLD))
                             + x86("jmp", L(l_next)); break;
        case PLR_COMP: s += x86("cmp", "al", (long)DT_PLREF)
                          + x86("jne", L(PL_L_COLD))
                          + x86("jmp", L(l_next)); break;
        case PLR_UNB: s += x86("jmp", L(PL_L_COLD)); break;
        default: s += x86("jmp", L(l_next)); break;
    }
    s += x86("def", L(l_unb));
    switch (rule) {
        case PLR_UNB: case PLR_UNB_OR_INT0: case PLR_UNB_OR_TEXT: s += x86("jmp", L(l_next)); break;
        default: s += x86("jmp", L(PL_L_COLD)); break;
    }
    return s + x86("def", L(l_next));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pl_arm_anum(int narg, int argbase, int resoff, IR_t * name_node) {
    const char * name = (name_node && name_node->op == IR_LIT_STRING) ? IR_LIT(name_node).sval : (const char *)0;
    const int * rules = pl_anum_rules(name, narg - 1);
    std::string s = x86("comment", (std::string("PL-R7 $pl_anum_guard: ") + (name ? name : "?")
        + (rules ? " -- the accepting argument shapes tested inline, rt_pl_anum_cold builds the ISO ball" : " -- no inline rule, rt_pl_anum_cold decides")).c_str());
    if (rules) {
        for (int i = 0; i < narg - 1; i++) if (rules[i] != PLR_ANY) s += pl_anum_arg(rules[i], argbase + 16 * (i + 1), 100 + 10 * i);
        s += x86("jmp", L(PL_L_OK)) + pl_ok_store(resoff);
    }
    s += x86("def", L(PL_L_COLD)) + pl_cold_call("rt_pl_anum_cold", (void *)rt_pl_anum_cold, narg, argbase, resoff, 0) + pl_tail();
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string pl_arm_mkc(int narg, int argbase, int resoff, IR_t * fnode) {
    return IF(!fnode || fnode->op != IR_LIT_INTEGER,
               x86_bomb("PL-MKC: the functor operand of a $mkc is the id the lowerer interned (an IR_LIT_INTEGER); the R1.3 re-intern fallback is deleted (the cto's check 4, 2026-10-03)"))
         + IF(fnode && fnode->op == IR_LIT_INTEGER,
               x86("comment",
                   "PL-MKC (ARCH-PROLOG-C-OUT-OF-THE-BOX 3.2): the box builds the compound -- one allocating call for the argument block (HB_DVEC; the allocator's own zero-fill is what types the "
                              "cells before the poll, both arms), the result cell {DT_PLREF, functor id, block} stored before the poll, then "
                                  "the kids by an inline deref/copy loop with no call in it; an "
                              "unbound kid gets a fresh self-reference in the block and its cell is bound to it under the trail test. No rt_pl_dop_mkc, no plw_mkc_kids.")
         + x86("mov32", "edi", (long)HB_DVEC)
         + x86("mov32", "esi", (long)(16 * (narg - 1)))
         + x86("call", "rt_gcheap_alloc", (uint64_t)(uintptr_t)(void *)rt_gcheap_alloc)
         + x86_movabs_r64("rcx", (uint64_t)DT_PLREF | ((uint64_t)(uint32_t)IR_LIT(fnode).ival << 32))
         + x86("note",
             "the result cell is parked in argv[0] (the functor literal's cell, dead once its id is baked above) across the "
                 "poll and the kid loop: the box's result slot may be one of the kids' argv cells")
         + x86("mov", FRQ(argbase), "rcx")
         + x86("mov", FRQ(argbase + 8), "rax")
         + x86_rt_gc_poll()
         + x86("mov", "r10", FRQ(argbase + 8))
         + x86("lea", "r9", FRQ(argbase + 16))
         + x86("mov", "r11", (long)(narg - 1))
         + x86("def", L(100))
         + x86("test", "r11", "r11")
         + x86("jz", L(102))
         + x86("mov", "rdi", "r9")
         + PL_DEREF(103, 104, 105, 106)
         + PL_UNBOUND(107, 108)
         + x86("mov", "rax", RDQ("rdi", 0))
         + x86("mov", "rdx", RDQ("rdi", 8))
         + x86("mov", RDQ("r10", 0), "rax")
         + x86("mov", RDQ("r10", 8), "rdx")
         + x86("jmp", L(101))
         + x86("def", L(107))
         + x86("note", "an unbound kid already on the heap (at or below rsp, the unifier's own test) is LINKED from the block, never rebound: binding the older heap variable to the newer "
                       "cell would move the variable on every term built over it and its standard order would follow the build order")
         + x86("cmp", "rdi", "rsp")
         + x86("ja", L(113))
         + x86("mov", RDQ("r10", 0), (long)DT_PLVAR)
         + x86("mov", RDQ("r10", 8), "rdi")
         + x86("jmp", L(101))
         + x86("def", L(113))
         + x86("mov", RDQ("r10", 0), (long)DT_PLVAR)
         + x86("mov", RDQ("r10", 8), "r10")
         + PL_TRAIL(109, 111)
         + x86("mov", RDQ("rdi", 0), (long)DT_PLVAR)
         + x86("mov", RDQ("rdi", 8), "r10")
         + x86("def", L(101))
         + x86("add", "r9", 16L)
         + x86("add", "r10", 16L)
         + x86("sub", "r11", 1L)
         + x86("jmp", L(100))
         + x86("def", L(102))
         + x86("mov", "rax", FRQ(argbase))
         + x86("mov", "rdx", FRQ(argbase + 8))
         + x86("mov", FRQ(resoff), "rax")
         + x86("mov", FRQ(resoff + 8), "rdx")
         + x86_gamma()
         + x86("def", L(111))
         + x86("mov", "rdi", "r12")
         + x86("call", "rt_pl_tr_refuse", (uint64_t)(uintptr_t)(void *)rt_pl_tr_refuse)
         + x86_beta_trampoline());
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void rt_pl_db_decls_install(const long *, void *);
extern "C" int pl_db_decls_words(long long ** out);
static std::string pl_arm_dbdecls(int narg, int resoff) {
    long long * w = (long long *) 0; int n = pl_db_decls_words(&w); std::string data; (void)narg;
    if (n < 2 || !w) return x86_bomb("$db_decls: the declaration table could not be read from the lowerer at emit time");
    for (int i = 1; i < n; i++) data += x86(".quad", (uint64_t)(int64_t)w[i]);
    return x86("comment",
        "the Prolog declaration table (ARCH-PROLOG-C-OUT-OF-THE-BOX 5.2 C): one compile-time table of {atom id, arity, "
            "kind, cell} entries carried in the box and installed by ONE call once the root frame exists")
         + x86_lea_id("rdi", 120) + x86("mov", "rsi", "r14")
         + x86("call", "rt_pl_db_decls_install", (uint64_t)(uintptr_t)(void *)rt_pl_db_decls_install)
         + x86_rt_gc_poll()
         + x86("mov32", "eax", (long)DT_I)
         + x86("mov32", "edx", 1L)
         + x86("mov", FRQ(resoff), "rax")
         + x86("mov", FRQ(resoff + 8), "rdx")
         + x86_gamma()
         + x86("def", L(120))
         + x86(".quad", (uint64_t)(int64_t)w[0]) + data
         + x86_beta_trampoline();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int rtx_pl_unify(DESCR_t *, DESCR_t *);
static std::string pl_arm_unify(int argbase, int resoff) {
    return x86("comment", "body =/2 (ARCH-PROLOG-C-OUT-OF-THE-BOX 6.2): the two argv cells to the general-unify leaf on the spine; the result is the leaf's verdict")
         + x86("lea", "rdi", FRQ(argbase))
         + x86("lea", "rsi", FRQ(argbase + 16))
         + x86("call", "rtx_pl_unify", (uint64_t)(uintptr_t)(void *)rtx_pl_unify)
         + x86("test", "eax", "eax")
         + x86("jz", L(PL_L_FAIL))
         + x86("mov", FRQ(resoff), (long)DT_I)
         + x86("mov", FRQ(resoff + 8), 1L) + x86_gamma()
         + pl_fail_store(resoff) + pl_tail();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string pl_leaf_inline_arm(const char * fn, int narg, int argbase, int resoff, IR_t * first_operand) {
    const char * op = 0;
    switch (pl_leaf_kind(fn, narg, &op)) {
        case PLK_MKC:    return pl_arm_mkc(narg, argbase, resoff, first_operand);
        case PLK_DBDECLS: return pl_arm_dbdecls(narg, resoff);
        case PLK_UNIFY:  return pl_arm_unify(argbase, resoff);
        case PLK_AX:     return pl_arm_ax(op, narg, argbase, resoff);
        case PLK_CMP:    return pl_arm_cmp(op, argbase, resoff);
        case PLK_IS:     return pl_arm_is(argbase, resoff);
        case PLK_TYPE:   return pl_arm_type(op, argbase, resoff);
        case PLK_ATOP:   return pl_arm_atop(op, argbase, resoff);
        case PLK_ZGUARD: return pl_arm_zguard(argbase, resoff);
        case PLK_ANUM:   return pl_arm_anum(narg, argbase, resoff, first_operand);
        default:         return std::string();
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string pl_leaf_zd_cold(const char * fn, int narg) {
    const char * op = 0; int k = pl_leaf_kind(fn, narg, &op);
    std::string s = x86("comment", (std::string("PL-R7 ") + fn + " under ZD: the cold value service alone (no Prolog graph takes this route today)").c_str());
    s += x86("lea", "rdi", RDQ("rsp", 0))
       + x86("mov32", "esi", (long)narg);
    switch (k) {
        case PLK_MKC:
            return x86_bomb("PL-MKC under ZD: a Prolog body term is a flat-frame box; no ZD arm exists");
        case PLK_AX:
            s += pl_opstr("rdx", op) + x86("call", "rt_pl_ax_cold", (uint64_t)(uintptr_t)(void *)rt_pl_ax_cold);
            return s + x86_rt_gc_poll_res();
        case PLK_CMP:
            s += pl_opstr("rdx", op) + x86("call", "rt_pl_cmp_cold", (uint64_t)(uintptr_t)(void *)rt_pl_cmp_cold);
            return s + x86_rt_gc_poll_res();
        case PLK_TYPE:
            s += pl_opstr("rdx", op) + x86("call", "rt_pl_type_cold", (uint64_t)(uintptr_t)(void *)rt_pl_type_cold);
            return s + x86_rt_gc_poll_res();
        case PLK_ATOP:
            s += pl_opstr("rdx", op) + x86("call", "rt_pl_atop_cold", (uint64_t)(uintptr_t)(void *)rt_pl_atop_cold);
            return s + x86_rt_gc_poll_res();
        case PLK_ZGUARD:
            s += x86("call", "rt_pl_zguard_cold", (uint64_t)(uintptr_t)(void *)rt_pl_zguard_cold);
            return s + x86_rt_gc_poll_res();
        case PLK_ANUM:
            s += x86("call", "rt_pl_anum_cold", (uint64_t)(uintptr_t)(void *)rt_pl_anum_cold);
            return s + x86_rt_gc_poll_res();
        case PLK_IS:
            s += x86("call", "rt_pl_is_cold", (uint64_t)(uintptr_t)(void *)rt_pl_is_cold);
            s += x86_rt_gc_poll_res();
            s += x86("cmp", "al", (long)DT_FAIL)
               + x86("je", L(150));
            s += x86("mov", RDQ("rsp", 16), "rax")
               + x86("mov", RDQ("rsp", 24), "rdx")
               + x86("lea", "rdi", RDQ("rsp", 0))
               + x86("lea", "rsi", RDQ("rsp", 16));
            s += x86("call", "rtx_pl_unify", (uint64_t)(uintptr_t)(void *)rtx_pl_unify);
            s += x86("test", "eax", "eax")
               + x86("jz", L(151))
               + x86("mov", "rax", RDQ("rsp", 16))
               + x86("mov", "rdx", RDQ("rsp", 24))
               + x86("jmp", L(150));
            s += x86("def", L(151))
               + x86("mov32", "eax", (long)DT_FAIL)
               + x86("xor", "edx", "edx");
            return s + x86("def", L(150));
        default:         return std::string();
    }
}
