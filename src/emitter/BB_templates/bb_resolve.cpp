#include "bb_common.h"
extern "C" {
#include "IR_interp_state.h"
}
/*--------------------------------------------------------------------------------------------------------------------*/
int bb_op_floaty(const char *fn) {
    static const char *f[] = { "sqrt", "sin", "cos", "tan", "asin", "acos", "atan", "exp", "log",
                               "float", "float_integer_part", "float_fractional_part",
                               "truncate", "round", "ceiling", "floor", "integer", "/", NULL };
    for (int i = 0; f[i]; i++) if (!strcmp(fn, f[i])) return 1;
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string briplbl_expr(const char *s) {
    char b[64]; b[0] = 0; if (s) strtab_label(b, sizeof b, s);
    return std::string("[rip + ") + b + "]";
}
static int bmset(const char *s) {
    static const char *m[] = { "is", "=:=", "=\\=", "<", ">", "=<", ">=", "=", "\\=", NULL };
    for (int k = 0; m[k]; k++) if (!strcmp(s, m[k])) return 1;
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
std::string emit_build_compound_term(const IR_t *nd);
static std::string emit_build_conj_chain(IR_t **goals, int i, int n) {
    int frm = 16;
    return IF(i >= n - 1, emit_build_compound_term(goals[n - 1]))
         + IF(i <  n - 1, x86("sub",  "rsp", (long)frm)
                        + emit_build_compound_term(goals[i])
                        + x86("mov",  RSP(0), "rax")
                        + emit_build_conj_chain(goals, i + 1, n)
                        + x86("mov",  RSP(8), "rax")
                        + x86("lea",  "rdi", briplbl_expr(","))
                        + x86("mov32","esi", (long)2)
                        + x86("mov",  "rdx", "rsp")
                        + x86("call", "rt_compound_build_n", (uint64_t)(uintptr_t)(void*)rt_compound_build_n)
                        + x86("add",  "rsp", (long)frm));
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bterm_atomform(int kind, const char *sval) {
    return x86("mov32", "edi",  (long)kind)
         + x86("xor",   "rsi",  "rsi")
         + IF(sval != NULL, x86("lea", "rdx", briplbl_expr(sval)))
         + IF(sval == NULL, x86("xor", "edx", "edx"))
         + x86("xor",   "ecx",  "ecx")
         + x86("call",  "rt_node_to_term", (uint64_t)(uintptr_t)(void*)rt_node_to_term);
}
static std::string bterm_goal(const IR_t *nd) {
    bb_goal_state_t *zc = (bb_goal_state_t *)(intptr_t)IR_LIT(nd).ival;
    const char *gfn = (zc && zc->callee) ? zc->callee : IR_LIT(nd).sval;
    int arity = zc ? zc->arity : 0;
    int frm = (arity * 8 + 15) & ~15;
    return IF(arity <= 0 || !zc || !zc->args, bterm_atomform((int)IR_ATOM, gfn))
         + IF(arity > 0 && zc && zc->args,
              x86("sub", "rsp", (long)frm)
            + FOR(0, (zc && zc->args && zc->nargs < arity ? zc->nargs : arity), [&](int i) {
                  return emit_build_compound_term(zc->args[i]) + x86("mov", RSP(i * 8), "rax"); })
            + IF(gfn != NULL, x86("lea", "rdi", briplbl_expr(gfn)))
            + IF(gfn == NULL, x86("xor", "edi", "edi"))
            + x86("mov32","esi", (long)arity)
            + x86("mov",  "rdx", "rsp")
            + x86("call", "rt_compound_build_n", (uint64_t)(uintptr_t)(void*)rt_compound_build_n)
            + x86("add",  "rsp", (long)frm));
}
static std::string bterm_arith(const IR_t *nd) {
    int arity = (int)IR_LIT(nd).ival;
    int frm = (arity * 8 + 15) & ~15;
    const IR_t *o0 = ir_pair_arg(nd, 0);
    const IR_t *o1 = (arity >= 2) ? ir_pair_arg(nd, 1) : NULL;
    return x86("sub", "rsp", (long)frm)
         + IF(o0 != NULL, emit_build_compound_term(o0) + x86("mov", RSP(0), "rax"))
         + IF(o0 != NULL && arity >= 2 && o1 != NULL, emit_build_compound_term(o1) + x86("mov", RSP(8), "rax"))
         + IF(IR_LIT(nd).sval != NULL, x86("lea", "rdi", briplbl_expr(IR_LIT(nd).sval)))
         + IF(IR_LIT(nd).sval == NULL, x86("xor", "edi", "edi"))
         + x86("mov32","esi", (long)arity)
         + x86("mov",  "rdx", "rsp")
         + x86("call", "rt_compound_build_n", (uint64_t)(uintptr_t)(void*)rt_compound_build_n)
         + x86("add",  "rsp", (long)frm);
}
static std::string bterm_mset(const IR_t *nd) {
    return x86("sub",  "rsp", (long)16)
         + emit_build_compound_term(ir_pair_arg(nd, 0))
         + x86("mov",  RSP(0), "rax")
         + emit_build_compound_term(ir_pair_arg(nd, 1))
         + x86("mov",  RSP(8), "rax")
         + x86("lea",  "rdi", briplbl_expr(IR_LIT(nd).sval))
         + x86("mov32","esi", (long)2)
         + x86("mov",  "rdx", "rsp")
         + x86("call", "rt_compound_build_n", (uint64_t)(uintptr_t)(void*)rt_compound_build_n)
         + x86("add",  "rsp", (long)16);
}
/*--------------------------------------------------------------------------------------------------------------------*/
std::string emit_build_compound_term(const IR_t *nd) {
    if (!nd)
        return x86("xor", "eax", "eax")
             + x86("xor", "edi", "edi")
             + x86("xor", "esi", "esi")
             + x86("xor", "edx", "edx")
             + x86("xor", "ecx", "ecx")
             + x86("call","rt_node_to_term", (uint64_t)(uintptr_t)(void*)rt_node_to_term);
    if (nd->op == IR_LIT_I || nd->op == IR_LIT_F || nd->op == IR_ATOM || nd->op == IR_LOGICVAR)
        return x86("mov32","edi", (long)nd->op)
             + x86("mov",  "rsi", (long)IR_LIT(nd).ival)
             + IF(nd->op == IR_ATOM && IR_LIT(nd).sval, x86("lea", "rdx", briplbl_expr(IR_LIT(nd).sval)))
             + IF(!(nd->op == IR_ATOM && IR_LIT(nd).sval), x86("xor", "edx", "edx"))
             + x86("xor",  "ecx", "ecx")
             + x86("call", "rt_node_to_term", (uint64_t)(uintptr_t)(void*)rt_node_to_term);
    if (nd->op == IR_STRUCT) {
        if ((int)IR_LIT(nd).ival <= 0 || !ir_call_arg(nd, 0)) return bterm_atomform((int)IR_ATOM, IR_LIT(nd).sval);
        int arity = (int)IR_LIT(nd).ival;
        int frm = (arity * 8 + 15) & ~15;
        return x86("sub", "rsp", (long)frm)
             + FOR(0, arity, [&](int i) { return IF(ir_call_arg(nd, i) != NULL, emit_build_compound_term(ir_call_arg(nd, i)) + x86("mov", RSP(i * 8), "rax")); })
             + IF(IR_LIT(nd).sval != NULL, x86("lea", "rdi", briplbl_expr(IR_LIT(nd).sval)))
             + IF(IR_LIT(nd).sval == NULL, x86("xor", "edi", "edi"))
             + x86("mov32","esi", (long)arity)
             + x86("mov",  "rdx", "rsp")
             + x86("call", "rt_compound_build_n", (uint64_t)(uintptr_t)(void*)rt_compound_build_n)
             + x86("add",  "rsp", (long)frm);
    }
    if (nd->op == IR_GOAL) return bterm_goal(nd);
    if (nd->op == IR_ARITH) {
        if ((int)IR_LIT(nd).ival <= 0 || !ir_pair_arg(nd, 0)) return bterm_atomform((int)IR_ATOM, IR_LIT(nd).sval);
        return bterm_arith(nd);
    }
    if (nd->op == IR_GCONJ) {
        bb_conj_state_t *zs = (bb_conj_state_t *)(intptr_t)IR_LIT(nd).ival;
        if (zs && zs->goals && zs->ngoals >= 1) return emit_build_conj_chain(zs->goals, 0, zs->ngoals);
    }
    if (nd->op == IR_BUILTIN && IR_LIT(nd).sval && bmset(IR_LIT(nd).sval) && ir_pair_arg(nd, 0) && ir_pair_arg(nd, 1))
        return bterm_mset(nd);
    return x86("comment", std::string("build_compound_term: unhandled kind ") + std::to_string((int)nd->op));
}
/*--------------------------------------------------------------------------------------------------------------------*/
std::string emit_term_from_node_bin(const IR_t *nd) {
    return bytes(2, "\x48\xBF") + u64le((uint64_t)(uintptr_t)(const void *)nd)
         + bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void *)rt_node_to_term_ptr)
         + bytes(2, "\xFF\xD0");
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bdisp(IR_t *pBB) {
    const char *fn = _.op_sval ? _.op_sval : "";
    std::string hdr = x86("label", _.lbl_α);
    std::string r;
    if (!(r = bb_io()).empty()) return r;
    if (!(r = bb_is_cmp_str(pBB, fn, hdr)).empty()) return r;
    if (!(r = bb_type_test_str(pBB, fn, hdr)).empty()) return r;
    if (!(r = bb_term_inspect_str(pBB, fn, hdr)).empty()) return r;
    if (!(r = bb_aggregate_nb_str(pBB, fn, hdr)).empty()) return r;
    if (!(r = bb_atom_string_str(hdr)).empty()) return r;
    if (!(r = bb_term_io_str(pBB, fn, hdr)).empty()) return r;
    if (!(r = bb_findall_str(pBB, fn, hdr)).empty()) return r;
    if (!(r = bb_succ_plus_str(pBB, fn, hdr)).empty()) return r;
    if (!(r = bb_list_str(pBB, fn, hdr)).empty()) return r;
    if (!(r = bb_retract_throw_str(pBB, fn, hdr)).empty()) return r;
    if (MEDIUM_BINARY) return bytes(1, "\xE9") + u32le(0) + bytes(1, "\xE9") + u32le(0);
    return x86("comment", std::string("RESOLVE: unknown '") + fn + "' stub")
         + x86("jmp", "γ")
         + x86("def", "β")
         + x86("jmp", "γ");
}
std::string bb_resolve(IR_t * pBB) {
    return IF(PLATFORM_X86 && MEDIUM_MACRO_DEF, x86("comment", "no macro form — RESOLVE"))
         + IF(PLATFORM_X86 && !MEDIUM_MACRO_DEF, bdisp(pBB));
}
