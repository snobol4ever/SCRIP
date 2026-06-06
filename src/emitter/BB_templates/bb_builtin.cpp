#include "bb_builtin_common.h"
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
static int bfrm(int n) { return (n * 8 + 15) & ~15; }
static const IR_t * bnth(const IR_t *c, int i) { while (i-- > 0 && c) c = c->γ; return c; }
static std::string briplbl(const char *s) { char b[64]; b[0] = 0; if (s) strtab_label(b, sizeof b, s); return std::string("[rip + ") + b + "]"; }
static std::string bslot(int i) { return std::string("qword ptr [rsp + ") + std::to_string(i) + "], rax"; }
static int bmset(const char *s) { static const char *m[] = { "is", "=:=", "=\\=", "<", ">", "=<", ">=", "=", "\\=", NULL }; for (int k = 0; m[k]; k++) if (!strcmp(s, m[k])) return 1; return 0; }
/*--------------------------------------------------------------------------------------------------------------------*/
std::string emit_build_compound_term(const IR_t *nd);
static std::string emit_build_conj_chain(IR_t **goals, int i, int n) {
    return IF(i >= n - 1, emit_build_compound_term(goals[n - 1]))
         + IF(i <  n - 1, x86("ins2", "sub rsp,", "16")
                        + emit_build_compound_term(goals[i])
                        + x86("ins2", "mov", bslot(0))
                        + emit_build_conj_chain(goals, i + 1, n)
                        + x86("ins2", "mov", bslot(8))
                        + x86("ins2", "lea rdi,", briplbl(","))
                        + x86("ins2", "mov esi,", "2")
                        + x86("ins2", "mov", "rdx, rsp")
                        + x86("ins2", "call", "rt_compound_build_n@PLT")
                        + x86("ins2", "add rsp,", "16"));
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bterm_atomform(int kind, const char *sval) {
    return x86("ins2", "mov edi,",  std::to_string(kind))
         + x86("ins2", "xor", "rsi, rsi")
         + IF(sval != NULL, x86("ins2", "lea rdx,", briplbl(sval))) + IF(sval == NULL, x86("ins2", "xor", "edx, edx"))
         + x86("ins2", "xor", "ecx, ecx")
         + x86("ins2", "call", "rt_node_to_term@PLT");
}
static std::string bterm_goal(const IR_t *nd) { bb_goal_state_t *zc = (bb_goal_state_t *)(intptr_t)nd->ival; const char *gfn = (zc && zc->callee) ? zc->callee : nd->sval; int arity = zc ? zc->arity : 0;
    return IF(arity <= 0 || !zc || !zc->args, bterm_atomform((int)IR_ATOM, gfn))
         + IF(arity > 0 && zc && zc->args,
              x86("ins2", "sub rsp,", std::to_string(bfrm(arity)))
            + FOR(0, (zc && zc->args && zc->nargs < arity ? zc->nargs : arity), [&](int i) { return emit_build_compound_term(zc->args[i]) + x86("ins2", "mov", bslot(i * 8)); })
            + IF(gfn != NULL, x86("ins2", "lea rdi,", briplbl(gfn))) + IF(gfn == NULL, x86("ins2", "xor", "edi, edi"))
            + x86("ins2", "mov esi,", std::to_string(arity))
            + x86("ins2", "mov", "rdx, rsp")
            + x86("ins2", "call", "rt_compound_build_n@PLT")
            + x86("ins2", "add rsp,", std::to_string(bfrm(arity))));
}
static std::string bterm_arith(const IR_t *nd) { int arity = (int)nd->ival; const IR_t *o0 = nd->α; const IR_t *o1 = (arity >= 2) ? nd->β : NULL;
    return x86("ins2", "sub rsp,", std::to_string(bfrm(arity)))
         + IF(o0 != NULL, emit_build_compound_term(o0) + x86("ins2", "mov", bslot(0)))
         + IF(o0 != NULL && arity >= 2 && o1 != NULL, emit_build_compound_term(o1) + x86("ins2", "mov", bslot(8)))
         + IF(nd->sval != NULL, x86("ins2", "lea rdi,", briplbl(nd->sval))) + IF(nd->sval == NULL, x86("ins2", "xor", "edi, edi"))
         + x86("ins2", "mov esi,", std::to_string(arity))
         + x86("ins2", "mov", "rdx, rsp")
         + x86("ins2", "call", "rt_compound_build_n@PLT")
         + x86("ins2", "add rsp,", std::to_string(bfrm(arity)));
}
static std::string bterm_mset(const IR_t *nd) {
    return x86("ins2", "sub rsp,", "16")
         + emit_build_compound_term(nd->α)
         + x86("ins2", "mov", bslot(0))
         + emit_build_compound_term(nd->β)
         + x86("ins2", "mov", bslot(8))
         + x86("ins2", "lea rdi,", briplbl(nd->sval))
         + x86("ins2", "mov esi,", "2")
         + x86("ins2", "mov", "rdx, rsp")
         + x86("ins2", "call", "rt_compound_build_n@PLT")
         + x86("ins2", "add rsp,", "16");
}
/*--------------------------------------------------------------------------------------------------------------------*/
std::string emit_build_compound_term(const IR_t *nd) {
    if (!nd) {
        return x86("ins2", "xor", "eax, eax")
             + x86("ins2", "xor", "edi, edi")
             + x86("ins2", "xor", "esi, esi")
             + x86("ins2", "xor", "edx, edx")
             + x86("ins2", "xor", "ecx, ecx")
             + x86("ins2", "call", "rt_node_to_term@PLT");
    }
    if (nd->t == IR_LIT_I || nd->t == IR_LIT_F || nd->t == IR_ATOM || nd->t == IR_LOGICVAR) {
        return x86("ins2", "mov edi,",  std::to_string((int)nd->t))
             + x86("ins2", "mov rsi,",  std::to_string((long)nd->ival))
             + IF(nd->t == IR_ATOM && nd->sval, x86("ins2", "lea rdx,", briplbl(nd->sval))) + IF(!(nd->t == IR_ATOM && nd->sval), x86("ins2", "xor", "edx, edx"))
             + x86("ins2", "xor", "ecx, ecx")
             + x86("ins2", "call", "rt_node_to_term@PLT");
    }
    if (nd->t == IR_STRUCT) {
        if ((int)nd->ival <= 0 || !nd->α) return bterm_atomform((int)IR_ATOM, nd->sval);
        return x86("ins2", "sub rsp,", std::to_string(bfrm((int)nd->ival)))
             + FOR(0, (int)nd->ival, [&](int i) { return IF(bnth(nd->α, i) != NULL, emit_build_compound_term(bnth(nd->α, i)) + x86("ins2", "mov", bslot(i * 8))); })
             + IF(nd->sval != NULL, x86("ins2", "lea rdi,", briplbl(nd->sval))) + IF(nd->sval == NULL, x86("ins2", "xor", "edi, edi"))
             + x86("ins2", "mov esi,", std::to_string((int)nd->ival))
             + x86("ins2", "mov", "rdx, rsp")
             + x86("ins2", "call", "rt_compound_build_n@PLT")
             + x86("ins2", "add rsp,", std::to_string(bfrm((int)nd->ival)));
    }
    if (nd->t == IR_GOAL) return bterm_goal(nd);
    if (nd->t == IR_ARITH) {
        if ((int)nd->ival <= 0 || !nd->α) return bterm_atomform((int)IR_ATOM, nd->sval);
        return bterm_arith(nd);
    }
    if (nd->t == IR_GCONJ) {
        bb_conj_state_t *zs = (bb_conj_state_t *)(intptr_t)nd->ival;
        if (zs && zs->goals && zs->ngoals >= 1) return emit_build_conj_chain(zs->goals, 0, zs->ngoals);
    }
    if (nd->t == IR_BUILTIN && nd->sval && bmset(nd->sval) && nd->α && nd->β) return bterm_mset(nd);
    return x86("comment", std::string("build_compound_term: unhandled kind ") + std::to_string((int)nd->t));
}
/*--------------------------------------------------------------------------------------------------------------------*/
std::string emit_term_from_node_bin(const IR_t *nd) {
    return bytes(2, "\x48\xBF") + u64le((uint64_t)(uintptr_t)(const void *)nd) + bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void *)rt_node_to_term_ptr) + bytes(2, "\xFF\xD0");
}
/*--------------------------------------------------------------------------------------------------------------------*/
static const char * bfn() { return _.op_sval ? _.op_sval : ""; }
static std::string bhdr() { return x86("label", _.lbl_α) + x86("ins1", std::string("# BOX RESOLVE_BUILTIN(") + bfn() + "/" + std::to_string((int)_.op_ival) + ")"); }
static std::string bunknown(const std::string &hdr) {
    if (MEDIUM_BINARY) return bytes(1, "\xE9") + u32le(0) + bytes(1, "\xE9") + u32le(0);
    return IF(MEDIUM_TEXT, hdr + x86("ins1", std::string("# RESOLVE_BUILTIN: unknown '") + bfn() + "' — stub") + x86("ins2", "jmp", _.lbl_γ) + x86("Lins2", std::string(_.lbl_β) + ":", "jmp", _.lbl_γ));
}
static std::string bdisp(IR_t *pBB) { const char *fn = bfn(); std::string hdr = bhdr(); std::string r;
    if (!(r = bb_builtin_io_str(pBB, fn, hdr)).empty()) return r;
    if (!(r = bb_builtin_is_cmp_str(pBB, fn, hdr)).empty()) return r;
    if (!(r = bb_builtin_type_test_str(pBB, fn, hdr)).empty()) return r;
    if (!(r = bb_builtin_term_inspect_str(pBB, fn, hdr)).empty()) return r;
    if (!(r = bb_builtin_aggregate_nb_str(pBB, fn, hdr)).empty()) return r;
    if (!(r = bb_builtin_atom_string_str(pBB, fn, hdr)).empty()) return r;
    if (!(r = bb_builtin_term_io_str(pBB, fn, hdr)).empty()) return r;
    if (!(r = bb_builtin_findall_str(pBB, fn, hdr)).empty()) return r;
    if (!(r = bb_builtin_succ_plus_str(pBB, fn, hdr)).empty()) return r;
    if (!(r = bb_builtin_list_str(pBB, fn, hdr)).empty()) return r;
    if (!(r = bb_builtin_retract_throw_str(pBB, fn, hdr)).empty()) return r;
    return bunknown(hdr);
}
static std::string bb_builtin_str(IR_t * pBB) {
    return IF(PLATFORM_X86 && MEDIUM_MACRO_DEF, x86("comment", "no macro form — RESOLVE_BUILTIN"))
         + IF(PLATFORM_X86 && !MEDIUM_MACRO_DEF, bdisp(pBB));
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_builtin(IR_t * pBB) { bb_emit_x86(bb_builtin_str(pBB)); }
