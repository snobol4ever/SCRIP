/* bb_unify.cpp — BB template for IR_UNIFY: Prolog unification (X = Y).
   PA-1 (GOAL-PROLOG-BB.md): the four-port (γ/ω) decision is emitted INLINE here,
   not delegated to a C port-logic helper. The three former rt_pl_unify_var_atom/
   _var_var/_generic functions (which encapsulated the γ/ω choice in C — INVARIANT 9)
   are DELETED. The template now builds each operand term via the pure CONVERSION
   helper rt_pl_node_to_term, unifies via the EFFECT helper rt_pl_unify_terms
   (trail_mark+unify+unwind — returns plain 1/0, makes NO jump), and the emitted x86
   itself takes the γ (success) or ω (fail) port. Both helpers are on the KEEP side
   of PJ-RT-PURGE: a conversion and a side-effect, neither a four-port dispatcher.
   x86 only per Invariant #14. */
#include <string>
#include <cstring>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
}
extern "C" void *rt_pl_node_to_term(int kind, long ival, const char *sval, double dval);
extern "C" int   rt_pl_unify_terms(void *l, void *r);
extern "C" int   rt_pl_unify_const(int slot, int kind, long ival, const char *sval, double dval);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Port tail: test the unify result in eax and branch γ/ω. Identical for text and
   binary; β re-entry falls straight to ω (a leaf has no retry). */
static std::string resolve_unify_tail_binary(int base, bb_bin_t & bin) {
    /* base = byte offset of the `test eax,eax` (2 bytes) within the box. */
    int je = base + 2;                 /* je rel32 site starts after test       */
    bin = { {je + 2, je + 6 + 1, je + 6 + 5, je + 6 + 5 + 1},
            {_.lbl_ω_p, _.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p},
            {false, false, true, false} };
    return bytes(2, "\x85\xC0")                 /* test eax, eax                  */
         + bytes(2, "\x0F\x84") + u32le(0)      /* je   ω                         */
         + bytes(1, "\xE9")     + u32le(0)      /* jmp  γ                         */
         + bytes(1, "\xE9")     + u32le(0);     /* β:  jmp ω                      */
}
static std::string resolve_unify_tail_text(void) {
    return s_2asm("test", "eax, eax")
         + s_2asm("je", _.lbl_ω)
         + s_2asm("jmp", _.lbl_γ)
         + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω);
}
/* CAT-B (2026-05-27, Opus 4.7): defined in bb_builtin.cpp (BB_templates/). Same TU set (Makefile      */
/* `scrip` target links all BB_templates/*.cpp into one binary); de-staticized so we can reuse the    */
/* recursive Term* walker for IR_STRUCT operands instead of the broken default-arm-of-             */
/* rt_pl_node_to_term path. Forward declaration matches the post-static signature in bb_builtin.cpp. */
extern std::string emit_build_compound_term(const IR_t *nd);
extern std::string emit_build_compound_term_bin(const IR_t *nd);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Build one operand term into rax via rt_pl_node_to_term(kind, ival, sval, dval).
   SysV: edi=kind, rsi=ival, rdx=sval-ptr (0 if none), xmm0=dval (0 if none).
   `lbl` is the interned .S label for the operand's sval (NULL → pass 0). */
static std::string build_term_text(const IR_t *nd, const char *lbl) {
    std::string load_rdx = lbl
        ? s_2asm("lea", emit_fmt("rdx, [rip + %s]", lbl))
        : s_2asm("xor", "edx, edx");
    return s_2asm("mov edi,", emit_fmt("%d", (int)nd->t))
         + s_2asm("mov rsi,", emit_fmt("%ld", (long)nd->ival))
         + load_rdx
         + s_2asm("xorps", "xmm0, xmm0")
         + s_2asm("call", "rt_pl_node_to_term@PLT");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* CAT-B (2026-05-27, Opus 4.7): build one operand term into rax. For scalar leaves (IR_LOGICVAR, IR_ATOM,
   IR_LIT_I, IR_LIT_F) → build_term_text → rt_pl_node_to_term, same as before. For IR_STRUCT →
   emit_build_compound_term (declared in bb_builtin.cpp), which post-order materializes the full
   Term* tree (functor name + arity + recursively built args via rt_pl_compound_build_n). Closes
   CAT-B: f(X,a) = f(b,Y) no longer falls through rt_pl_node_to_term's default arm to
   term_new_int(arity) — the actual compound is constructed, so unify(L, R) binds X=b, Y=a. */
static std::string build_operand_term(const IR_t *nd, const char *lbl) {
    if (nd && nd->t == IR_STRUCT) return emit_build_compound_term(nd);
    return build_term_text(nd, lbl);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_unify_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (PLATFORM_X86) {
        if (MEDIUM_MACRO_DEF) {
            return s_comment("# no macro form — RESOLVE_UNIFY");
        }
        /* Missing operand(s): vacuous success (matches bb_exec.c F-6d fallthrough). */
        if (!pBB->α || !pBB->β) {
            if (MEDIUM_BINARY) {
                bin = { {1, 5, 6}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
                return bytes(1, "\xE9") + u32le(0)
                     + bytes(1, "\xE9") + u32le(0);
            }
            return s_comment("# BOX RESOLVE_UNIFY")
                 + s_comment("# RESOLVE_UNIFY: missing children — vacuous success")
                 + s_2asm("jmp", _.lbl_γ)
                 + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω);
        }
        IR_t *lhs = pBB->α, *rhs = pBB->β;
        const char *ls = _.bb_ls;   /* interned .S label for lhs->sval (or NULL) */
        const char *rs = _.bb_rs;   /* interned .S label for rhs->sval (or NULL) */
        /* WAM-CP-7 first-occurrence / self-unify elision (gprolog get_variable; swipl H_FIRSTVAR): a head variable at its own positional slot lowers to IR_UNIFY(LOGICVAR(i), LOGICVAR(i)); unify(env[i], env[i]) hits unify()'s */
        /* t1==t2 short-circuit (always true, no binding, no trail). The sole side-effect of the general path here is resolve_node_to_term's lazy vivification of env[i], which every reader performs idempotently (storing back */
        /* the same term_new_var(i)), so a NULL slot and a vivified-unbound-var slot are observationally identical and eliding to a bare success jump is equivalent. Reuses the missing-operand vacuous-success emission (γ; β→ω). */
        if (lhs->t == IR_LOGICVAR && rhs->t == IR_LOGICVAR && lhs->ival == rhs->ival) {
            if (MEDIUM_BINARY) {
                bin = { {1, 5, 6}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
                return bytes(1, "\xE9") + u32le(0)
                     + bytes(1, "\xE9") + u32le(0);
            }
            return s_1asm(emit_fmt("%s:", _.lbl_α))
                 + s_comment("# BOX RESOLVE_UNIFY (WAM-CP-7 self-unify x=x — vacuous success)")
                 + s_2asm("jmp", _.lbl_γ)
                 + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω);
        }
        /* WAM-CP-7 var-vs-const head-match specialization (gprolog get_atom/get_integer; swipl H_ATOM/H_SMALLINT): one operand a logic-var slot, the other a scalar constant → a single rt_pl_unify_const call (reads/derefs */
        /* the slot; an unbound var binds+trails via the same unify path, a bound var scalar-compares) in place of node_to_term×2 + unify_terms, skipping the const Term alloc in the bound case. rt_pl_unify_const is built */
        /* from the identical unify() leaves, so it is provably equal to the general arm below and m4 output stays byte-identical to the m2/m3 oracle. Any other shape (var-var, var-compound, const-const, …) falls through. */
        {
            const IR_t *vnode = NULL, *cnode = NULL; const char *clbl = NULL;
            if (lhs->t == IR_LOGICVAR && (rhs->t == IR_ATOM || rhs->t == IR_LIT_I || rhs->t == IR_LIT_F)) { vnode = lhs; cnode = rhs; clbl = rs; }
            else if (rhs->t == IR_LOGICVAR && (lhs->t == IR_ATOM || lhs->t == IR_LIT_I || lhs->t == IR_LIT_F)) { vnode = rhs; cnode = lhs; clbl = ls; }
            if (vnode && cnode) {
                int slot = (int)vnode->ival;
                if (MEDIUM_TEXT) {
                    std::string fload;
                    if (cnode->t == IR_LIT_F) { uint64_t fb = 0; double dv = cnode->dval; memcpy(&fb, &dv, sizeof fb); fload = s_2asm("mov rax,", emit_fmt("%llu", (unsigned long long)fb)) + s_2asm("movq", "xmm0, rax"); }
                    else                      { fload = s_2asm("xorps", "xmm0, xmm0"); }
                    std::string body =
                          s_1asm(emit_fmt("%s:", _.lbl_α))
                        + s_comment("# BOX RESOLVE_UNIFY (WAM-CP-7 var-const)")
                        + s_2asm("mov edi,", emit_fmt("%d", slot))                 /* arg0 slot                     */
                        + s_2asm("mov esi,", emit_fmt("%d", (int)cnode->t))        /* arg1 kind (IR enum ordinal)   */
                        + s_2asm("mov rdx,", emit_fmt("%ld", (long)cnode->ival))   /* arg2 ival                     */
                        + (clbl ? s_2asm("lea", emit_fmt("rcx, [rip + %s]", clbl)) : s_2asm("xor", "ecx, ecx"))  /* arg3 sval */
                        + fload                                                    /* arg4 dval (xmm0)              */
                        + s_2asm("call", "rt_pl_unify_const@PLT");
                    return body + resolve_unify_tail_text();
                }
                if (MEDIUM_BINARY) {
                    std::string b;
                    b += bytes(1, "\xBF") + u32le((uint32_t)slot);                  /* mov edi, imm32 (slot)         */
                    b += bytes(1, "\xBE") + u32le((uint32_t)(int)cnode->t);         /* mov esi, imm32 (kind)         */
                    b += bytes(2, "\x48\xBA") + u64le((uint64_t)(long)cnode->ival); /* movabs rdx, imm64 (ival)      */
                    if (clbl) b += bytes(2, "\x48\xB9") + u64le((uint64_t)(uintptr_t)cnode->sval); /* movabs rcx, sval-ptr */
                    else      b += bytes(2, "\x31\xC9");                            /* xor ecx, ecx                  */
                    if (cnode->t == IR_LIT_F) { uint64_t fb = 0; double dv = cnode->dval; memcpy(&fb, &dv, sizeof fb); b += bytes(2, "\x48\xB8") + u64le(fb) + bytes(5, "\x66\x48\x0F\x6E\xC0"); } /* movabs rax,bits; movq xmm0,rax */
                    else                      { b += bytes(3, "\x0F\x57\xC0"); }    /* xorps xmm0, xmm0              */
                    b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_pl_unify_const) + bytes(2, "\xFF\xD0"); /* movabs rax,&rt_pl_unify_const; call rax */
                    int base = 5 + 5 + 10;                                          /* edi(5) + esi(5) + rdx(10)     */
                    base += clbl ? 10 : 2;                                          /* arg3: movabs rcx(10)|xor(2)   */
                    base += (cnode->t == IR_LIT_F) ? 15 : 3;                        /* arg4: rax+movq(15)|xorps(3)   */
                    base += 10 + 2;                                                 /* movabs rax(10) + call rax(2)  */
                    return b + resolve_unify_tail_binary(base, bin);
                }
            }
        }

        if (MEDIUM_TEXT) {
            /* CAT-B (2026-05-27, Opus 4.7): use a 16-aligned scratch frame instead of push/pop so that
               emit_build_compound_term — which assumes 16-aligned rsp because its recursive rt_pl_*
               calls require SysV alignment — can be invoked for either operand without re-alignment
               hazards. Box entry rsp is 8B-mis-aligned (return-addr pushed by the call into the box);
               `sub rsp, 16` realigns to 16 and reserves one 8B scratch slot (matches the precedent in
               bb_builtin.cpp's functor_term / arg_term / univ_term arms at lines 562, 614, 689, 705). */
            std::string body =
                  s_1asm(emit_fmt("%s:", _.lbl_α))
                + s_comment("# BOX RESOLVE_UNIFY")
                + s_2asm("sub", "rsp, 16")
                + build_operand_term(lhs, ls)                  /* L term -> rax                 */
                + s_2asm("mov", "qword ptr [rsp + 0], rax")    /* stash L in scratch slot       */
                + build_operand_term(rhs, rs)                  /* R term -> rax                 */
                + s_2asm("mov", "rsi, rax")                    /* rsi = R                       */
                + s_2asm("mov", "rdi, qword ptr [rsp + 0]")    /* rdi = L                       */
                + s_2asm("add", "rsp, 16")                     /* restore to box-entry rsp      */
                + s_2asm("call", "rt_pl_unify_terms@PLT");
            return body + resolve_unify_tail_text();
        }
        if (MEDIUM_BINARY) {
            /* PLR-J-5 (2026-05-29): COMPOUND operands now wired.  Was honest-abort-guarded (the TEXT
               arm routes IR_STRUCT through emit_build_compound_term; the binary twin
               emit_build_compound_term_bin landed in PLR-J-3 and leaves a Term* in rax, balancing its
               own rsp frame).  build_bin routes IR_STRUCT to it; scalars keep the rt_pl_node_to_term
               immediate path.  Uses the TEXT arm's 16-byte scratch-slot discipline (sub rsp,16 / mov
               [rsp],L / build R / add rsp,16) instead of `push rax`, so rsp stays 16-aligned across a
               compound R build's internal `call rt_pl_compound_build_n` (SysV).                        */
            auto build_bin = [](const IR_t *nd, const char *lbl) -> std::string {
                if (nd && nd->t == IR_STRUCT) return emit_build_compound_term_bin(nd);
                std::string b;
                b += bytes(1, "\xBF") + u32le((uint32_t)(int)nd->t);          /* mov edi, imm32        */
                b += bytes(2, "\x48\xBE") + u64le((uint64_t)(long)nd->ival);  /* movabs rsi, imm64     */
                if (lbl) b += bytes(2, "\x48\xBA") + u64le((uint64_t)(uintptr_t)nd->sval); /* movabs rdx,imm64 */
                else     b += bytes(2, "\x31\xD2");                            /* xor edx, edx          */
                b += bytes(3, "\x0F\x57\xC0");                                 /* xorps xmm0, xmm0      */
                b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_pl_node_to_term) + bytes(2, "\xFF\xD0"); /* movabs rax,&rt_pl_node_to_term; call rax */
                return b;
            };
            std::string b;
            b += bytes(4, "\x48\x83\xEC\x10");                                /* sub rsp, 16           */
            b += build_bin(lhs, ls);                                          /* L term -> rax         */
            b += bytes(4, "\x48\x89\x04\x24");                                /* mov [rsp + 0], rax    */
            b += build_bin(rhs, rs);                                          /* R term -> rax         */
            b += bytes(3, "\x48\x89\xC6");                                    /* mov rsi, rax          */
            b += bytes(4, "\x48\x8B\x3C\x24");                                /* mov rdi, [rsp + 0]    */
            b += bytes(4, "\x48\x83\xC4\x10");                                /* add rsp, 16           */
            b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_pl_unify_terms) + bytes(2, "\xFF\xD0"); /* movabs rax,&rt_pl_unify_terms; call rax */
            return b + resolve_unify_tail_binary((int)b.size(), bin);
        }
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_unify(IR_t * pBB) {
    bb_bin_t bin;
    bb_emit_asm_result(bb_unify_str(pBB, bin), bin);
}
