#include "bb_builtin_common.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_builtin_term_io_str(IR_t *pBB, const char *fn, const std::string &hdr) {
    (void)pBB; (void)fn; (void)hdr;
    if (MEDIUM_BINARY) {
            if (strcmp(fn, "numbervars") == 0 && _.op_ival == 3 && pBB->α && pBB->α->γ && pBB->α->γ->γ) {
                IR_t *a0 = pBB->α, *a1 = a0->γ, *a2 = a1->γ;
                long start = (long)a1->ival;
                int  k2 = (int)a2->t;
                long i2 = (long)a2->ival;
                const char *s2 = (k2 == IR_ATOM) ? a2->sval : NULL;
                std::string b;
                /* sub rsp, 8 (keep 16-alignment across the build's internal calls)   48 83 EC 08      */
                b += bytes(4, "\x48\x83\xEC\x08");
                /* build a0's term → rax                                                               */
                b += emit_term_from_node_bin(a0);
                /* mov rdi, rax    48 89 C7                                                             */
                b += bytes(3, "\x48\x89\xC7");
                /* mov rsi, start    48 BE [8]                                                          */
                b += bytes(2, "\x48\xBE") + u64le((uint64_t)start);
                /* mov edx, k2    BA [4]                                                                */
                b += bytes(1, "\xBA") + u32le((uint32_t)k2);
                /* mov rcx, i2    48 B9 [8]                                                             */
                b += bytes(2, "\x48\xB9") + u64le((uint64_t)i2);
                /* mov r8, s2 (or xor r8d,r8d)    49 B8 [8] / 45 31 C0                                  */
                if (s2) b += bytes(2, "\x49\xB8") + u64le((uint64_t)(uintptr_t)s2);
                else    b += bytes(3, "\x45\x31\xC0");
                /* movabs rax, &rt_numbervars_term; call rax                                         */
                b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_numbervars_term) + bytes(2, "\xFF\xD0");
                /* add rsp, 8    48 83 C4 08                                                            */
                b += bytes(4, "\x48\x83\xC4\x08");
                /* test eax, eax    85 C0                                                               */
                b += bytes(2, "\x85\xC0");
                return x86_lit_bytes(b) + x86("je", PORT_OMEGA) + x86("jmp", PORT_GAMMA) + x86("jmp", PORT_OMEGA);
            }
            if ((strcmp(fn, "term_to_atom") == 0 || strcmp(fn, "term_string") == 0)
                && pBB->α && pBB->α->γ) {
                IR_t *a0 = pBB->α, *a1 = a0->γ;
                int   k1 = (int)a1->t;
                long  i1 = (long)a1->ival;
                const char *s1 = (k1 == IR_ATOM) ? a1->sval : NULL;
                std::string b;
                b += bytes(4, "\x48\x83\xEC\x10");                /* sub rsp, 16 */
                b += emit_term_from_node_bin(a0);            /* build a0's term → rax */
                b += bytes(3, "\x48\x89\xC7");                    /* mov rdi, rax */
                b += bytes(1, "\xBE") + u32le((uint32_t)k1);      /* mov esi, k1 */
                b += bytes(2, "\x48\xBA") + u64le((uint64_t)i1);  /* mov rdx, i1 */
                if (s1) b += bytes(2, "\x48\xB9") + u64le((uint64_t)(uintptr_t)s1);  /* mov rcx, s1 */
                else    b += bytes(2, "\x31\xC9");                /* xor ecx, ecx */
                b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_term_to_atom_term) + bytes(2, "\xFF\xD0");
                b += bytes(4, "\x48\x83\xC4\x10");                /* add rsp, 16 */
                b += bytes(2, "\x85\xC0");                        /* test eax, eax */
                return x86_lit_bytes(b) + x86("je", PORT_OMEGA) + x86("jmp", PORT_GAMMA) + x86("jmp", PORT_OMEGA);
            }
            if (strcmp(fn, "format") == 0 && pBB->α && (_.op_ival == 1 || _.op_ival == 2)) {
                int   arity = (int)_.op_ival;
                IR_t *a0 = pBB->α;
                IR_t *a1 = (arity == 2) ? a0->γ : NULL;
                int   k0 = (int)a0->t;
                long  i0 = (long)a0->ival;
                const char *s0 = (k0 == IR_ATOM) ? a0->sval : NULL;
                int   compound1 = (a1 && a1->t == IR_STRUCT);
                std::string b;
                b += bytes(4, "\x48\x83\xEC\x10");                /* sub rsp, 16 */
                if (compound1) {
                    /* Path B: build args-list Term* (→ rax), move to r8, then scalar fmt args. */
                    b += emit_term_from_node_bin(a1);
                    b += bytes(3, "\x49\x89\xC0");                /* mov r8, rax */
                    b += bytes(1, "\xBF") + u32le((uint32_t)arity);          /* mov edi, arity */
                    b += bytes(1, "\xBE") + u32le((uint32_t)k0);             /* mov esi, k0 */
                    b += bytes(2, "\x48\xBA") + u64le((uint64_t)i0);         /* mov rdx, i0 */
                    if (s0) b += bytes(2, "\x48\xB9") + u64le((uint64_t)(uintptr_t)s0);  /* mov rcx, s0 */
                    else    b += bytes(2, "\x31\xC9");                       /* xor ecx, ecx */
                    b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_format_term) + bytes(2, "\xFF\xD0");
                } else {
                    /* Path A: scalar args1 (variable, atom, or absent for arity 1). */
                    int   k1 = a1 ? (int)a1->t : 0;
                    long  i1 = a1 ? (long)a1->ival : 0;
                    const char *s1 = (a1 && k1 == IR_ATOM) ? a1->sval : NULL;
                    b += bytes(1, "\xBF") + u32le((uint32_t)arity);          /* mov edi, arity */
                    b += bytes(1, "\xBE") + u32le((uint32_t)k0);             /* mov esi, k0 */
                    b += bytes(2, "\x48\xBA") + u64le((uint64_t)i0);         /* mov rdx, i0 */
                    if (s0) b += bytes(2, "\x48\xB9") + u64le((uint64_t)(uintptr_t)s0);  /* mov rcx, s0 */
                    else    b += bytes(2, "\x31\xC9");                       /* xor ecx, ecx */
                    b += bytes(2, "\x41\xB8") + u32le((uint32_t)k1);         /* mov r8d, k1 */
                    b += bytes(2, "\x49\xB9") + u64le((uint64_t)i1);         /* mov r9, i1 */
                    if (s1) b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)s1);  /* mov rax, s1 */
                    else    b += bytes(2, "\x31\xC0");                       /* xor eax, eax */
                    b += bytes(4, "\x48\x89\x04\x24");                       /* mov [rsp+0], rax */
                    b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_format) + bytes(2, "\xFF\xD0");
                }
                b += bytes(4, "\x48\x83\xC4\x10");                /* add rsp, 16 */
                b += bytes(2, "\x85\xC0");                        /* test eax, eax */
                return x86_lit_bytes(b) + x86("je", PORT_OMEGA) + x86("jmp", PORT_GAMMA) + x86("jmp", PORT_OMEGA);
            }
    }
    if (MEDIUM_TEXT) {
    std::string succ_back = x86("ins2", "jmp", _.lbl_γ)
                          + x86("Lins2", emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_γ);
    (void)succ_back;
        if (strcmp(fn, "numbervars") == 0 && _.op_ival == 3 && pBB->α && pBB->α->γ && pBB->α->γ->γ) {
            IR_t *a0 = pBB->α, *a1 = a0->γ, *a2 = a1->γ;
            long start = (long)a1->ival;
            int  k2 = (int)a2->t;
            long i2 = (long)a2->ival;
            char s2lbl[64]; s2lbl[0] = 0;
            if (k2 == IR_ATOM && a2->sval) strtab_label(s2lbl, sizeof s2lbl, a2->sval);
            return hdr
                 + x86("ins2", "sub", "rsp, 8")
                 + emit_build_compound_term(a0)
                 + x86("ins2", "mov", "rdi, rax")
                 + x86("ins2", "mov rsi,", emit_fmt("%ld", start))
                 + x86("ins2", "mov edx,", emit_fmt("%d", k2))
                 + x86("ins2", "mov rcx,", emit_fmt("%ld", i2))
                 + (s2lbl[0] ? x86("ins2", "lea r8,", emit_fmt("[rip + %s]", s2lbl)) : x86("ins2", "xor", "r8d, r8d"))
                 + x86("ins2", "call", "rt_numbervars_term@PLT")
                 + x86("ins2", "add", "rsp, 8")
                 + x86("ins2", "test", "eax, eax")
                 + x86("ins2", "je",   _.lbl_ω)
                 + x86("ins2", "jmp",  _.lbl_γ)
                 + x86("Lins2", emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω);
        }
        if ((strcmp(fn, "term_to_atom") == 0 || strcmp(fn, "term_string") == 0)
            && pBB->α && pBB->α->γ) {
            IR_t *a0 = pBB->α, *a1 = a0->γ;
            int   k1 = (int)a1->t;
            long  i1 = (long)a1->ival;
            char s1lbl[64]; s1lbl[0] = 0;
            if (k1 == IR_ATOM && a1->sval) strtab_label(s1lbl, sizeof s1lbl, a1->sval);
            return hdr
                 + x86("ins2", "sub", "rsp, 16")
                 + emit_build_compound_term(a0)
                 + x86("ins2", "mov", "rdi, rax")
                 + x86("ins2", "mov esi,",  emit_fmt("%d",  k1))
                 + x86("ins2", "mov rdx,",  emit_fmt("%ld", i1))
                 + (s1lbl[0] ? x86("ins2", "lea rcx,", emit_fmt("[rip + %s]", s1lbl)) : x86("ins2", "xor", "ecx, ecx"))
                 + x86("ins2", "call", "rt_term_to_atom_term@PLT")
                 + x86("ins2", "add", "rsp, 16")
                 + x86("ins2", "test", "eax, eax")
                 + x86("ins2", "je",   _.lbl_ω)
                 + x86("ins2", "jmp",  _.lbl_γ)
                 + x86("Lins2", emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω);
        }
        if (strcmp(fn, "format") == 0 && pBB->α && (_.op_ival == 1 || _.op_ival == 2)) {
            int arity = (int)_.op_ival;
            IR_t *a0 = pBB->α;
            IR_t *a1 = (arity == 2) ? a0->γ : NULL;
            int  k0 = (int)a0->t;
            long i0 = (long)a0->ival;
            char s0lbl[64]; s0lbl[0] = 0;
            if (k0 == IR_ATOM && a0->sval) strtab_label(s0lbl, sizeof s0lbl, a0->sval);
            int compound1 = (a1 && a1->t == IR_STRUCT);
            if (compound1) {
                /* Path B: build args-list Term* in rax, then call rt_format_term(arity, k0,i0,s0, args). */
                return hdr
                     + x86("ins2", "sub", "rsp, 16")          /* 16B alignment + scratch */
                     + emit_build_compound_term(a1)
                     + x86("ins2", "mov", "r8, rax")          /* args-list Term* → r8 */
                     + x86("ins2", "mov edi,",  emit_fmt("%d", arity))
                     + x86("ins2", "mov esi,",  emit_fmt("%d", k0))
                     + x86("ins2", "mov rdx,",  emit_fmt("%ld", i0))
                     + (s0lbl[0] ? x86("ins2", "lea rcx,", emit_fmt("[rip + %s]", s0lbl)) : x86("ins2", "xor", "ecx, ecx"))
                     + x86("ins2", "call", "rt_format_term@PLT")
                     + x86("ins2", "add", "rsp, 16")
                     + x86("ins2", "test", "eax, eax")
                     + x86("ins2", "je",   _.lbl_ω)
                     + x86("ins2", "jmp",  _.lbl_γ)
                     + x86("Lins2", emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω);
            }
            /* Path A: scalar args1 (variable, atom, or absent for arity 1). */
            int  k1 = a1 ? (int)a1->t : 0;
            long i1 = a1 ? (long)a1->ival : 0;
            char s1lbl[64]; s1lbl[0] = 0;
            if (a1 && k1 == IR_ATOM && a1->sval) strtab_label(s1lbl, sizeof s1lbl, a1->sval);
            return hdr
                 + x86("ins2", "sub", "rsp, 16")
                 + x86("ins2", "mov edi,",  emit_fmt("%d", arity))
                 + x86("ins2", "mov esi,",  emit_fmt("%d", k0))
                 + x86("ins2", "mov rdx,",  emit_fmt("%ld", i0))
                 + (s0lbl[0] ? x86("ins2", "lea rcx,", emit_fmt("[rip + %s]", s0lbl)) : x86("ins2", "xor", "ecx, ecx"))
                 + x86("ins2", "mov r8d,",  emit_fmt("%d", k1))
                 + x86("ins2", "mov r9,",   emit_fmt("%ld", i1))
                 /* s1 (7th arg) lives at [rsp+0]; if absent, NULL */
                 + (s1lbl[0]
                       ? (x86("ins2", "lea rax,", emit_fmt("[rip + %s]", s1lbl)) + x86("ins2", "mov", "[rsp + 0], rax"))
                       : x86("ins2", "mov", "qword ptr [rsp + 0], 0"))
                 + x86("ins2", "call", "rt_format@PLT")
                 + x86("ins2", "add", "rsp, 16")
                 + x86("ins2", "test", "eax, eax")
                 + x86("ins2", "je",   _.lbl_ω)
                 + x86("ins2", "jmp",  _.lbl_γ)
                 + x86("Lins2", emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω);
        }
    }
    return std::string();
}
