#include "bb_builtin_common.h"
extern "C" {
#include "IR_interp_state.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int bb_op_floaty(const char *fn) {
    static const char *f[] = { "sqrt", "sin", "cos", "tan", "asin", "acos", "atan", "exp", "log",
                               "float", "float_integer_part", "float_fractional_part",
                               "truncate", "round", "ceiling", "floor", "integer", "/", NULL };
    for (int i = 0; f[i]; i++) if (!strcmp(fn, f[i])) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string emit_build_compound_term(const IR_t *nd);
static std::string emit_build_conj_chain(IR_t **goals, int i, int n) {
    if (i >= n - 1) return emit_build_compound_term(goals[n - 1]);
    char clbl[64]; strtab_label(clbl, sizeof clbl, ",");
    std::string out = s_2asm("sub rsp,", emit_fmt("%d", 16));
    out += emit_build_compound_term(goals[i]);
    out += s_2asm("mov", emit_fmt("qword ptr [rsp + %d], rax", 0));
    out += emit_build_conj_chain(goals, i + 1, n);
    out += s_2asm("mov", emit_fmt("qword ptr [rsp + %d], rax", 8));
    out += s_2asm("lea rdi,", emit_fmt("[rip + %s]", clbl))
         + s_2asm("mov esi,", emit_fmt("%d", 2))
         + s_2asm("mov", "rdx, rsp")
         + s_2asm("call", "rt_compound_build_n@PLT")
         + s_2asm("add rsp,", emit_fmt("%d", 16));
    return out;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string emit_build_compound_term(const IR_t *nd) {
    if (!nd) {
        return s_2asm("xor", "eax, eax")
             + s_2asm("xor", "edi, edi")
             + s_2asm("xor", "esi, esi")
             + s_2asm("xor", "edx, edx")
             + s_2asm("xor", "ecx, ecx")
             + s_2asm("call", "rt_node_to_term@PLT");
    }
    if (nd->t == IR_LIT_I || nd->t == IR_LIT_F || nd->t == IR_ATOM || nd->t == IR_LOGICVAR) {
        char slbl[64]; slbl[0] = 0;
        if (nd->t == IR_ATOM && nd->sval) strtab_label(slbl, sizeof slbl, nd->sval);
        return s_2asm("mov edi,",  emit_fmt("%d",  (int)nd->t))
             + s_2asm("mov rsi,",  emit_fmt("%ld", (long)nd->ival))
             + (slbl[0] ? s_2asm("lea rdx,", emit_fmt("[rip + %s]", slbl)) : s_2asm("xor", "edx, edx"))
             + s_2asm("xor", "ecx, ecx")
             + s_2asm("call", "rt_node_to_term@PLT");
    }
    if (nd->t == IR_STRUCT) {
        int arity = (int)nd->ival;
        if (arity <= 0 || !nd->α) {
            char fnlbl[64]; fnlbl[0] = 0;
            if (nd->sval) strtab_label(fnlbl, sizeof fnlbl, nd->sval);
            return s_2asm("mov edi,",  emit_fmt("%d", (int)IR_ATOM))
                 + s_2asm("xor", "rsi, rsi")
                 + (fnlbl[0] ? s_2asm("lea rdx,", emit_fmt("[rip + %s]", fnlbl)) : s_2asm("xor", "edx, edx"))
                 + s_2asm("xor", "ecx, ecx")
                 + s_2asm("call", "rt_node_to_term@PLT");
        }
        int slots_bytes = arity * 8;
        int frame = (slots_bytes + 15) & ~15;
        std::string out = s_2asm("sub rsp,", emit_fmt("%d", frame));
        const IR_t *child = nd->α;
        for (int i = 0; i < arity && child; i++) {
            out += emit_build_compound_term(child);
            out += s_2asm("mov", emit_fmt("qword ptr [rsp + %d], rax", i * 8));
            child = child->γ;
        }
        char fnlbl[64]; fnlbl[0] = 0;
        if (nd->sval) strtab_label(fnlbl, sizeof fnlbl, nd->sval);
        out += (fnlbl[0] ? s_2asm("lea rdi,", emit_fmt("[rip + %s]", fnlbl)) : s_2asm("xor", "edi, edi"))
             + s_2asm("mov esi,", emit_fmt("%d", arity))
             + s_2asm("mov", "rdx, rsp")
             + s_2asm("call", "rt_compound_build_n@PLT")
             + s_2asm("add rsp,", emit_fmt("%d", frame));
        return out;
    }
    if (nd->t == IR_GOAL) {
        bb_goal_state_t *zc = (bb_goal_state_t *)(intptr_t)nd->ival;
        const char *gfn = (zc && zc->callee) ? zc->callee : nd->sval;
        int arity = zc ? zc->arity : 0;
        char fnlbl[64]; fnlbl[0] = 0;
        if (gfn) strtab_label(fnlbl, sizeof fnlbl, gfn);
        if (arity <= 0 || !zc || !zc->args) {
            return s_2asm("mov edi,",  emit_fmt("%d", (int)IR_ATOM))
                 + s_2asm("xor", "rsi, rsi")
                 + (fnlbl[0] ? s_2asm("lea rdx,", emit_fmt("[rip + %s]", fnlbl)) : s_2asm("xor", "edx, edx"))
                 + s_2asm("xor", "ecx, ecx")
                 + s_2asm("call", "rt_node_to_term@PLT");
        }
        int navail = zc->nargs < arity ? zc->nargs : arity;
        int slots_bytes = arity * 8;
        int frame = (slots_bytes + 15) & ~15;
        std::string out = s_2asm("sub rsp,", emit_fmt("%d", frame));
        for (int i = 0; i < navail; i++) {
            out += emit_build_compound_term(zc->args[i]);
            out += s_2asm("mov", emit_fmt("qword ptr [rsp + %d], rax", i * 8));
        }
        out += (fnlbl[0] ? s_2asm("lea rdi,", emit_fmt("[rip + %s]", fnlbl)) : s_2asm("xor", "edi, edi"))
             + s_2asm("mov esi,", emit_fmt("%d", arity))
             + s_2asm("mov", "rdx, rsp")
             + s_2asm("call", "rt_compound_build_n@PLT")
             + s_2asm("add rsp,", emit_fmt("%d", frame));
        return out;
    }
    if (nd->t == IR_ARITH) {
        /* PLR-K-9 (2026-05-29): a IR_ARITH node in TERM position (not evaluated) is a compound term,  */
        /* e.g. term_to_atom(1+2, A). Mirror the MEDIUM_BINARY twin's IR_ARITH branch + resolve_node_to_term: */
        /* functor = sval, operands on α and β (NOT γ-chained like IR_STRUCT). arity 0 → atom; 1 →   */
        /* f(α); 2 → f(α,β). The TEXT walker previously had NO IR_ARITH branch → fell to the unhandled-  */
        /* kind comment → rax garbage → 1+2 rendered empty in mode-4 (the BINARY twin already had it).   */
        int arity = (int)nd->ival;
        if (arity <= 0 || !nd->α) {
            char fnlbl[64]; fnlbl[0] = 0;
            if (nd->sval) strtab_label(fnlbl, sizeof fnlbl, nd->sval);
            return s_2asm("mov edi,",  emit_fmt("%d", (int)IR_ATOM))
                 + s_2asm("xor", "rsi, rsi")
                 + (fnlbl[0] ? s_2asm("lea rdx,", emit_fmt("[rip + %s]", fnlbl)) : s_2asm("xor", "edx, edx"))
                 + s_2asm("xor", "ecx, ecx")
                 + s_2asm("call", "rt_node_to_term@PLT");
        }
        const IR_t *ops[2] = { nd->α, (arity >= 2) ? nd->β : NULL };
        int slots_bytes = arity * 8;
        int frame = (slots_bytes + 15) & ~15;
        std::string out = s_2asm("sub rsp,", emit_fmt("%d", frame));
        for (int i = 0; i < arity && ops[i]; i++) {
            out += emit_build_compound_term(ops[i]);
            out += s_2asm("mov", emit_fmt("qword ptr [rsp + %d], rax", i * 8));
        }
        char fnlbl[64]; fnlbl[0] = 0;
        if (nd->sval) strtab_label(fnlbl, sizeof fnlbl, nd->sval);
        out += (fnlbl[0] ? s_2asm("lea rdi,", emit_fmt("[rip + %s]", fnlbl)) : s_2asm("xor", "edi, edi"))
             + s_2asm("mov esi,", emit_fmt("%d", arity))
             + s_2asm("mov", "rdx, rsp")
             + s_2asm("call", "rt_compound_build_n@PLT")
             + s_2asm("add rsp,", emit_fmt("%d", frame));
        return out;
    }
    if (nd->t == IR_GCONJ) {
        bb_conj_state_t *zs = (bb_conj_state_t *)(intptr_t)nd->ival;
        if (zs && zs->goals && zs->ngoals >= 1) return emit_build_conj_chain(zs->goals, 0, zs->ngoals);
    }
    if (nd->t == IR_BUILTIN && nd->sval) {
        static const char *mset[] = { "is", "=:=", "=\\=", "<", ">", "=<", ">=", "=", "\\=", NULL };
        int hit = 0;
        for (int k = 0; mset[k]; k++) if (!strcmp(nd->sval, mset[k])) { hit = 1; break; }
        if (hit && nd->α && nd->β) {
            char fnlbl[64]; fnlbl[0] = 0;
            strtab_label(fnlbl, sizeof fnlbl, nd->sval);
            std::string out = s_2asm("sub rsp,", emit_fmt("%d", 16));
            out += emit_build_compound_term(nd->α);
            out += s_2asm("mov", emit_fmt("qword ptr [rsp + %d], rax", 0));
            out += emit_build_compound_term(nd->β);
            out += s_2asm("mov", emit_fmt("qword ptr [rsp + %d], rax", 8));
            out += s_2asm("lea rdi,", emit_fmt("[rip + %s]", fnlbl))
                 + s_2asm("mov esi,", emit_fmt("%d", 2))
                 + s_2asm("mov", "rdx, rsp")
                 + s_2asm("call", "rt_compound_build_n@PLT")
                 + s_2asm("add rsp,", emit_fmt("%d", 16));
            return out;
        }
    }
    return s_comment(emit_fmt("# build_compound_term: unhandled kind %d", (int)nd->t));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string emit_term_from_node_bin(const IR_t *nd) {
    std::string b;
    b += bytes(2, "\x48\xBF") + u64le((uint64_t)(uintptr_t)(const void *)nd);
    b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void *)rt_node_to_term_ptr) + bytes(2, "\xFF\xD0");
    return b;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_builtin_str(IR_t * pBB) {
    if (!PLATFORM_X86) return std::string();
    const char *fn = _.op_sval ? _.op_sval : "";
    if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — RESOLVE_BUILTIN");
    std::string hdr = s_1asm(emit_fmt("%s:", _.lbl_α))
                    + s_1asm(emit_fmt("# BOX RESOLVE_BUILTIN(%s/%d)", fn, _.op_ival));
    std::string r;
    r = bb_builtin_io_str(pBB, fn, hdr); if (!r.empty()) return r;
    r = bb_builtin_is_cmp_str(pBB, fn, hdr); if (!r.empty()) return r;
    r = bb_builtin_type_test_str(pBB, fn, hdr); if (!r.empty()) return r;
    r = bb_builtin_term_inspect_str(pBB, fn, hdr); if (!r.empty()) return r;
    r = bb_builtin_aggregate_nb_str(pBB, fn, hdr); if (!r.empty()) return r;
    r = bb_builtin_atom_string_str(pBB, fn, hdr); if (!r.empty()) return r;
    r = bb_builtin_term_io_str(pBB, fn, hdr); if (!r.empty()) return r;
    r = bb_builtin_findall_str(pBB, fn, hdr); if (!r.empty()) return r;
    r = bb_builtin_succ_plus_str(pBB, fn, hdr); if (!r.empty()) return r;
    r = bb_builtin_list_str(pBB, fn, hdr); if (!r.empty()) return r;
    r = bb_builtin_retract_throw_str(pBB, fn, hdr); if (!r.empty()) return r;
    if (MEDIUM_BINARY) return bytes(1, "\xE9") + u32le(0) + bytes(1, "\xE9") + u32le(0);
    if (MEDIUM_TEXT) {
    std::string succ_back = s_2asm("jmp", _.lbl_γ)
                          + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_γ);
    (void)succ_back;
        return hdr + s_1asm(emit_fmt("# RESOLVE_BUILTIN: unknown '%s' — stub", fn)) + succ_back;
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_builtin(IR_t * pBB) { bb_emit_x86(bb_builtin_str(pBB)); }
