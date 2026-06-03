#include "bb_builtin_common.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_builtin_term_inspect_str(IR_t *pBB, const char *fn, const std::string &hdr) {
    (void)pBB; (void)fn; (void)hdr;
    if (MEDIUM_BINARY) {
            if (strcmp(fn,"functor")==0 && pBB->ival==3 && pBB->α && pBB->α->γ && pBB->α->γ->γ
                && pBB->α->t == IR_STRUCT) {
                IR_t *a0 = pBB->α, *a1 = a0->γ, *a2 = a1->γ;
                int  k1 = (int)a1->t,  k2 = (int)a2->t;
                long i1 = (long)a1->ival, i2 = (long)a2->ival;
                const char *s1 = (k1 == IR_ATOM) ? a1->sval : NULL;
                const char *s2 = (k2 == IR_ATOM) ? a2->sval : NULL;
                std::string b;
                b += bytes(4, "\x48\x83\xEC\x10");
                b += emit_term_from_node_bin(a0);
                b += bytes(3, "\x48\x89\xC7");
                b += bytes(1, "\xBE") + u32le((uint32_t)k1);
                b += bytes(2, "\x48\xBA") + u64le((uint64_t)i1);
                if (s1) b += bytes(2, "\x48\xB9") + u64le((uint64_t)(uintptr_t)s1);
                else    b += bytes(2, "\x31\xC9");
                b += bytes(2, "\x41\xB8") + u32le((uint32_t)k2);
                b += bytes(2, "\x49\xB9") + u64le((uint64_t)i2);
                if (s2) b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)s2);
                else    b += bytes(2, "\x31\xC0");
                b += bytes(4, "\x48\x89\x04\x24");
                b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_functor_term) + bytes(2, "\xFF\xD0");
                b += bytes(4, "\x48\x83\xC4\x10");
                b += bytes(2, "\x85\xC0");
                return x86_lit_bytes(b) + x86("je", PORT_OMEGA) + x86("jmp", PORT_GAMMA) + x86("jmp", PORT_OMEGA);
            }
            if (strcmp(fn,"arg")==0 && pBB->ival==3 && pBB->α && pBB->α->γ && pBB->α->γ->γ
                && pBB->α->γ->t == IR_STRUCT) {
                IR_t *a0 = pBB->α, *a1 = a0->γ, *a2 = a1->γ;
                int  k0 = (int)a0->t,  k2 = (int)a2->t;
                long i0 = (long)a0->ival, i2 = (long)a2->ival;
                const char *s0 = (k0 == IR_ATOM) ? a0->sval : NULL;
                const char *s2 = (k2 == IR_ATOM) ? a2->sval : NULL;
                std::string b;
                b += bytes(4, "\x48\x83\xEC\x10");
                b += emit_term_from_node_bin(a1);
                b += bytes(3, "\x48\x89\xC1");
                b += bytes(1, "\xBF") + u32le((uint32_t)k0);
                b += bytes(2, "\x48\xBE") + u64le((uint64_t)i0);
                if (s0) b += bytes(2, "\x48\xBA") + u64le((uint64_t)(uintptr_t)s0);
                else    b += bytes(2, "\x31\xD2");
                b += bytes(2, "\x41\xB8") + u32le((uint32_t)k2);
                b += bytes(2, "\x49\xB9") + u64le((uint64_t)i2);
                if (s2) b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)s2);
                else    b += bytes(2, "\x31\xC0");
                b += bytes(4, "\x48\x89\x04\x24");
                b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_arg_term) + bytes(2, "\xFF\xD0");
                b += bytes(4, "\x48\x83\xC4\x10");
                b += bytes(2, "\x85\xC0");
                return x86_lit_bytes(b) + x86("je", PORT_OMEGA) + x86("jmp", PORT_GAMMA) + x86("jmp", PORT_OMEGA);
            }
            if (strcmp(fn,"=..")==0 && pBB->ival==2 && pBB->α && pBB->α->γ
                && (pBB->α->t == IR_STRUCT || pBB->α->γ->t == IR_STRUCT)) {
                IR_t *a0 = pBB->α, *a1 = a0->γ;
                int compound0 = (a0->t == IR_STRUCT);
                int compound1 = (a1->t == IR_STRUCT);
                int  k0 = (int)a0->t,  k1 = (int)a1->t;
                long i0 = (long)a0->ival, i1 = (long)a1->ival;
                const char *s0 = (!compound0 && k0 == IR_ATOM) ? a0->sval : NULL;
                const char *s1 = (!compound1 && k1 == IR_ATOM) ? a1->sval : NULL;
                std::string b;
                if (compound0 && compound1) {
                    b += bytes(4, "\x48\x83\xEC\x10");
                    b += emit_term_from_node_bin(a0);
                    b += bytes(4, "\x48\x89\x04\x24");
                    b += emit_term_from_node_bin(a1);
                    b += bytes(3, "\x48\x89\xC6");
                    b += bytes(4, "\x48\x8B\x3C\x24");
                    b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_univ_term_term) + bytes(2, "\xFF\xD0");
                    b += bytes(4, "\x48\x83\xC4\x10");
                } else if (compound0) {
                    b += bytes(4, "\x48\x83\xEC\x08");
                    b += emit_term_from_node_bin(a0);
                    b += bytes(3, "\x48\x89\xC7");
                    b += bytes(1, "\xBE") + u32le((uint32_t)k1);
                    b += bytes(2, "\x48\xBA") + u64le((uint64_t)i1);
                    if (s1) b += bytes(2, "\x48\xB9") + u64le((uint64_t)(uintptr_t)s1);
                    else    b += bytes(2, "\x31\xC9");
                    b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_univ_term) + bytes(2, "\xFF\xD0");
                    b += bytes(4, "\x48\x83\xC4\x08");
                } else {
                    b += bytes(4, "\x48\x83\xEC\x08");
                    b += emit_term_from_node_bin(a1);
                    b += bytes(3, "\x48\x89\xC1");
                    b += bytes(1, "\xBF") + u32le((uint32_t)k0);
                    b += bytes(2, "\x48\xBE") + u64le((uint64_t)i0);
                    if (s0) b += bytes(2, "\x48\xBA") + u64le((uint64_t)(uintptr_t)s0);
                    else    b += bytes(2, "\x31\xD2");
                    b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_univ_term_list) + bytes(2, "\xFF\xD0");
                    b += bytes(4, "\x48\x83\xC4\x08");
                }
                b += bytes(2, "\x85\xC0");
                return x86_lit_bytes(b) + x86("je", PORT_OMEGA) + x86("jmp", PORT_GAMMA) + x86("jmp", PORT_OMEGA);
            }
    }
    if (MEDIUM_TEXT) {
    std::string succ_back = s_2asm("jmp", _.lbl_γ)
                          + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_γ);
    (void)succ_back;
        if (strcmp(fn,"functor")==0 && pBB->ival==3 && pBB->α && pBB->α->γ && pBB->α->γ->γ) {
            IR_t *a0 = pBB->α, *a1 = a0->γ, *a2 = a1->γ;
            /* Name/Arity (a1/a2) are by Prolog semantics never compound literals (atom + int). Only a0    */
            /* may be a compound literal (e.g. `functor(foo(a,b), N, A)`). */
            int  k1 = (int)a1->t,  k2 = (int)a2->t;
            long i1 = (long)a1->ival, i2 = (long)a2->ival;
            char s1lbl[64]; s1lbl[0] = 0;
            char s2lbl[64]; s2lbl[0] = 0;
            if (k1 == IR_ATOM && a1->sval) strtab_label(s1lbl, sizeof s1lbl, a1->sval);
            if (k2 == IR_ATOM && a2->sval) strtab_label(s2lbl, sizeof s2lbl, a2->sval);
            if (a0->t == IR_STRUCT) {
                /* a0 compound-literal path: rt_functor_term(t0, k1,i1,s1, k2,i2,s2) */
                return hdr
                     + s_2asm("sub", "rsp, 16")
                     + emit_build_compound_term(a0)
                     + s_2asm("mov", "rdi, rax")
                     + s_2asm("mov esi,",  emit_fmt("%d",  k1))
                     + s_2asm("mov rdx,",  emit_fmt("%ld", i1))
                     + (s1lbl[0] ? s_2asm("lea rcx,", emit_fmt("[rip + %s]", s1lbl)) : s_2asm("xor", "ecx, ecx"))
                     + s_2asm("mov r8d,",  emit_fmt("%d",  k2))
                     + s_2asm("mov r9,",   emit_fmt("%ld", i2))
                     + (s2lbl[0] ? s_2asm("lea rax,", emit_fmt("[rip + %s]", s2lbl)) : s_2asm("xor", "eax, eax"))
                     + s_2asm("mov", "qword ptr [rsp + 0], rax")
                     + s_2asm("call", "rt_functor_term@PLT")
                     + s_2asm("add", "rsp, 16")
                     + s_2asm("test", "eax, eax")
                     + s_2asm("je",   _.lbl_ω)
                     + s_2asm("jmp",  _.lbl_γ)
                     + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω);
            }
            /* Scalar a0 path: rt_functor(k0,i0,s0, k1,i1,s1, k2,i2,s2). 3 stack args ⇒ sub rsp,32. */
            int  k0 = (int)a0->t;
            long i0 = (long)a0->ival;
            char s0lbl[64]; s0lbl[0] = 0;
            if (k0 == IR_ATOM && a0->sval) strtab_label(s0lbl, sizeof s0lbl, a0->sval);
            return hdr
                 + s_2asm("sub", "rsp, 32")
                 + s_2asm("mov edi,",  emit_fmt("%d",  k0))
                 + s_2asm("mov rsi,",  emit_fmt("%ld", i0))
                 + (s0lbl[0] ? s_2asm("lea rdx,", emit_fmt("[rip + %s]", s0lbl)) : s_2asm("xor", "edx, edx"))
                 + s_2asm("mov ecx,",  emit_fmt("%d",  k1))
                 + s_2asm("mov r8,",   emit_fmt("%ld", i1))
                 + (s1lbl[0] ? s_2asm("lea r9,",  emit_fmt("[rip + %s]", s1lbl)) : s_2asm("xor", "r9d, r9d"))
                 + s_2asm("mov dword ptr [rsp + 0],", emit_fmt("%d",  k2))
                 + s_2asm("mov rax,",  emit_fmt("%ld", i2))
                 + s_2asm("mov", "qword ptr [rsp + 8], rax")
                 + (s2lbl[0] ? s_2asm("lea rax,", emit_fmt("[rip + %s]", s2lbl)) : s_2asm("xor", "eax, eax"))
                 + s_2asm("mov", "qword ptr [rsp + 16], rax")
                 + s_2asm("call", "rt_functor@PLT")
                 + s_2asm("add", "rsp, 32")
                 + s_2asm("test", "eax, eax")
                 + s_2asm("je",   _.lbl_ω)
                 + s_2asm("jmp",  _.lbl_γ)
                 + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω);
        }
        if (strcmp(fn,"arg")==0 && pBB->ival==3 && pBB->α && pBB->α->γ && pBB->α->γ->γ) {
            IR_t *a0 = pBB->α, *a1 = a0->γ, *a2 = a1->γ;
            /* arg(N, Term, Arg): N is int, Term is the compound-literal candidate (a1), Arg is var.       */
            int  k0 = (int)a0->t,  k2 = (int)a2->t;
            long i0 = (long)a0->ival, i2 = (long)a2->ival;
            char s0lbl[64]; s0lbl[0] = 0;
            char s2lbl[64]; s2lbl[0] = 0;
            if (k0 == IR_ATOM && a0->sval) strtab_label(s0lbl, sizeof s0lbl, a0->sval);
            if (k2 == IR_ATOM && a2->sval) strtab_label(s2lbl, sizeof s2lbl, a2->sval);
            if (a1->t == IR_STRUCT) {
                /* a1 compound-literal path: rt_arg_term(k0,i0,s0, t1, k2,i2,s2). */
                return hdr
                     + s_2asm("sub", "rsp, 16")
                     + emit_build_compound_term(a1)
                     + s_2asm("mov", "rcx, rax")
                     + s_2asm("mov edi,",  emit_fmt("%d",  k0))
                     + s_2asm("mov rsi,",  emit_fmt("%ld", i0))
                     + (s0lbl[0] ? s_2asm("lea rdx,", emit_fmt("[rip + %s]", s0lbl)) : s_2asm("xor", "edx, edx"))
                     + s_2asm("mov r8d,",  emit_fmt("%d",  k2))
                     + s_2asm("mov r9,",   emit_fmt("%ld", i2))
                     + (s2lbl[0] ? s_2asm("lea rax,", emit_fmt("[rip + %s]", s2lbl)) : s_2asm("xor", "eax, eax"))
                     + s_2asm("mov", "qword ptr [rsp + 0], rax")
                     + s_2asm("call", "rt_arg_term@PLT")
                     + s_2asm("add", "rsp, 16")
                     + s_2asm("test", "eax, eax")
                     + s_2asm("je",   _.lbl_ω)
                     + s_2asm("jmp",  _.lbl_γ)
                     + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω);
            }
            /* Scalar a1 path: rt_arg(k0,i0,s0, k1,i1,s1, k2,i2,s2). */
            int  k1 = (int)a1->t;
            long i1 = (long)a1->ival;
            char s1lbl[64]; s1lbl[0] = 0;
            if (k1 == IR_ATOM && a1->sval) strtab_label(s1lbl, sizeof s1lbl, a1->sval);
            return hdr
                 + s_2asm("sub", "rsp, 32")
                 + s_2asm("mov edi,",  emit_fmt("%d",  k0))
                 + s_2asm("mov rsi,",  emit_fmt("%ld", i0))
                 + (s0lbl[0] ? s_2asm("lea rdx,", emit_fmt("[rip + %s]", s0lbl)) : s_2asm("xor", "edx, edx"))
                 + s_2asm("mov ecx,",  emit_fmt("%d",  k1))
                 + s_2asm("mov r8,",   emit_fmt("%ld", i1))
                 + (s1lbl[0] ? s_2asm("lea r9,",  emit_fmt("[rip + %s]", s1lbl)) : s_2asm("xor", "r9d, r9d"))
                 + s_2asm("mov dword ptr [rsp + 0],", emit_fmt("%d",  k2))
                 + s_2asm("mov rax,",  emit_fmt("%ld", i2))
                 + s_2asm("mov", "qword ptr [rsp + 8], rax")
                 + (s2lbl[0] ? s_2asm("lea rax,", emit_fmt("[rip + %s]", s2lbl)) : s_2asm("xor", "eax, eax"))
                 + s_2asm("mov", "qword ptr [rsp + 16], rax")
                 + s_2asm("call", "rt_arg@PLT")
                 + s_2asm("add", "rsp, 32")
                 + s_2asm("test", "eax, eax")
                 + s_2asm("je",   _.lbl_ω)
                 + s_2asm("jmp",  _.lbl_γ)
                 + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω);
        }
        if (strcmp(fn,"=..")==0 && pBB->ival==2 && pBB->α && pBB->α->γ) {
            IR_t *a0 = pBB->α, *a1 = a0->γ;
            int compound0 = (a0->t == IR_STRUCT);
            int compound1 = (a1->t == IR_STRUCT);
            char s0lbl[64]; s0lbl[0] = 0;
            char s1lbl[64]; s1lbl[0] = 0;
            int  k0 = (int)a0->t,  k1 = (int)a1->t;
            long i0 = (long)a0->ival, i1 = (long)a1->ival;
            if (!compound0 && k0 == IR_ATOM && a0->sval) strtab_label(s0lbl, sizeof s0lbl, a0->sval);
            if (!compound1 && k1 == IR_ATOM && a1->sval) strtab_label(s1lbl, sizeof s1lbl, a1->sval);
            if (compound0 && compound1) {
                /* Both compound literals: rt_univ_term_term(t0, t1). Save t0 across t1 build.           */
                return hdr
                     + s_2asm("sub", "rsp, 16")
                     + emit_build_compound_term(a0)
                     + s_2asm("mov", "qword ptr [rsp + 0], rax")
                     + emit_build_compound_term(a1)
                     + s_2asm("mov", "rsi, rax")
                     + s_2asm("mov", "rdi, qword ptr [rsp + 0]")
                     + s_2asm("call", "rt_univ_term_term@PLT")
                     + s_2asm("add", "rsp, 16")
                     + s_2asm("test", "eax, eax")
                     + s_2asm("je",   _.lbl_ω)
                     + s_2asm("jmp",  _.lbl_γ)
                     + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω);
            }
            if (compound0) {
                /* a0 compound literal: rt_univ_term(t0, k1,i1,s1) — t0=rdi, k1=esi, i1=rdx, s1=rcx. */
                return hdr
                     + s_2asm("sub", "rsp, 8")
                     + emit_build_compound_term(a0)
                     + s_2asm("mov", "rdi, rax")
                     + s_2asm("mov esi,",  emit_fmt("%d",  k1))
                     + s_2asm("mov rdx,",  emit_fmt("%ld", i1))
                     + (s1lbl[0] ? s_2asm("lea rcx,", emit_fmt("[rip + %s]", s1lbl)) : s_2asm("xor", "ecx, ecx"))
                     + s_2asm("call", "rt_univ_term@PLT")
                     + s_2asm("add", "rsp, 8")
                     + s_2asm("test", "eax, eax")
                     + s_2asm("je",   _.lbl_ω)
                     + s_2asm("jmp",  _.lbl_γ)
                     + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω);
            }
            if (compound1) {
                /* a1 compound literal (list literal): rt_univ_term_list(k0,i0,s0, t1).                  */
                return hdr
                     + s_2asm("sub", "rsp, 8")
                     + emit_build_compound_term(a1)
                     + s_2asm("mov", "rcx, rax")
                     + s_2asm("mov edi,",  emit_fmt("%d",  k0))
                     + s_2asm("mov rsi,",  emit_fmt("%ld", i0))
                     + (s0lbl[0] ? s_2asm("lea rdx,", emit_fmt("[rip + %s]", s0lbl)) : s_2asm("xor", "edx, edx"))
                     + s_2asm("call", "rt_univ_term_list@PLT")
                     + s_2asm("add", "rsp, 8")
                     + s_2asm("test", "eax, eax")
                     + s_2asm("je",   _.lbl_ω)
                     + s_2asm("jmp",  _.lbl_γ)
                     + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω);
            }
            /* Both scalars: rt_univ(k0,i0,s0, k1,i1,s1). All 6 args in registers. */
            return hdr
                 + s_2asm("sub", "rsp, 8")
                 + s_2asm("mov edi,",  emit_fmt("%d",  k0))
                 + s_2asm("mov rsi,",  emit_fmt("%ld", i0))
                 + (s0lbl[0] ? s_2asm("lea rdx,", emit_fmt("[rip + %s]", s0lbl)) : s_2asm("xor", "edx, edx"))
                 + s_2asm("mov ecx,",  emit_fmt("%d",  k1))
                 + s_2asm("mov r8,",   emit_fmt("%ld", i1))
                 + (s1lbl[0] ? s_2asm("lea r9,",  emit_fmt("[rip + %s]", s1lbl)) : s_2asm("xor", "r9d, r9d"))
                 + s_2asm("call", "rt_univ@PLT")
                 + s_2asm("add", "rsp, 8")
                 + s_2asm("test", "eax, eax")
                 + s_2asm("je",   _.lbl_ω)
                 + s_2asm("jmp",  _.lbl_γ)
                 + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω);
        }
    }
    return std::string();
}
