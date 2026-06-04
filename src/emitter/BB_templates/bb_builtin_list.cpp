#include "bb_builtin_common.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_builtin_list_str(IR_t *pBB, const char *fn, const std::string &hdr) {
    (void)pBB; (void)fn; (void)hdr;
    if (MEDIUM_BINARY) {
            if ((strcmp(fn, "atomic_list_concat") == 0 || strcmp(fn, "concat_atom") == 0)
                && pBB->α && (_.op_ival == 2 || _.op_ival == 3)) {
                int   arity = (int)_.op_ival;
                IR_t *a0 = pBB->α, *a1 = a0->γ, *a2 = a1 ? a1->γ : NULL;
                IR_t *sepN = (arity == 3) ? a1 : NULL;
                IR_t *resN = (arity == 3) ? a2 : a1;
                int   ksep = sepN ? (int)sepN->t : 0;
                long  isep = sepN ? (long)sepN->ival : 0;
                const char *ssep = (sepN && ksep == IR_ATOM) ? sepN->sval : NULL;
                int   kres = resN ? (int)resN->t : 0;
                long  ires = resN ? (long)resN->ival : 0;
                const char *sres = (resN && kres == IR_ATOM) ? resN->sval : NULL;
                std::string b;
                b += bytes(4, "\x48\x83\xEC\x10");                /* sub rsp, 16 */
                b += emit_term_from_node_bin(a0);            /* build arg0 list → rax */
                b += bytes(3, "\x48\x89\xC7");                    /* mov rdi, rax */
                b += bytes(1, "\xBE") + u32le((uint32_t)arity);   /* mov esi, arity */
                b += bytes(1, "\xBA") + u32le((uint32_t)ksep);    /* mov edx, ksep */
                b += bytes(2, "\x48\xB9") + u64le((uint64_t)isep);/* mov rcx, isep */
                if (ssep) b += bytes(2, "\x49\xB8") + u64le((uint64_t)(uintptr_t)ssep);  /* mov r8, ssep */
                else      b += bytes(3, "\x45\x31\xC0");          /* xor r8d, r8d */
                b += bytes(2, "\x41\xB9") + u32le((uint32_t)kres);/* mov r9d, kres */
                /* ires → [rsp+0] */
                b += bytes(2, "\x48\xB8") + u64le((uint64_t)ires);/* mov rax, ires */
                b += bytes(4, "\x48\x89\x04\x24");                /* mov [rsp+0], rax */
                /* sres → [rsp+8] */
                if (sres) b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)sres);  /* mov rax, sres */
                else      b += bytes(2, "\x31\xC0");              /* xor eax, eax */
                b += bytes(5, "\x48\x89\x44\x24\x08");            /* mov [rsp+8], rax */
                b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_atomic_list_concat_term) + bytes(2, "\xFF\xD0");
                b += bytes(4, "\x48\x83\xC4\x10");                /* add rsp, 16 */
                b += bytes(2, "\x85\xC0");                        /* test eax, eax */
                return x86_lit_bytes(b) + x86("je", PORT_OMEGA) + x86("jmp", PORT_GAMMA) + x86("jmp", PORT_OMEGA);
            }
            if ((strcmp(fn, "sort") == 0 || strcmp(fn, "msort") == 0) && pBB->α && pBB->α->γ) {
                IR_t *a0 = pBB->α, *a1 = a0->γ;
                int   do_msort = (strcmp(fn, "msort") == 0) ? 1 : 0;
                int   k1 = (int)a1->t;
                long  i1 = (long)a1->ival;
                const char *s1 = (k1 == IR_ATOM) ? a1->sval : NULL;
                std::string b;
                b += bytes(4, "\x48\x83\xEC\x10");                /* sub rsp, 16 */
                if (a0->t == IR_STRUCT) {
                    /* Path B: build a0's term → rax → rsi, then scalar args in rdi/edx/rcx/r8. */
                    b += emit_term_from_node_bin(a0);
                    b += bytes(3, "\x48\x89\xC6");                /* mov rsi, rax */
                    b += bytes(1, "\xBF") + u32le((uint32_t)do_msort);       /* mov edi, do_msort */
                    b += bytes(1, "\xBA") + u32le((uint32_t)k1);             /* mov edx, k1 */
                    b += bytes(2, "\x48\xB9") + u64le((uint64_t)i1);         /* mov rcx, i1 */
                    if (s1) b += bytes(2, "\x49\xB8") + u64le((uint64_t)(uintptr_t)s1);  /* mov r8, s1 */
                    else    b += bytes(3, "\x45\x31\xC0");                   /* xor r8d, r8d */
                    b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_sort_msort_term) + bytes(2, "\xFF\xD0");
                } else {
                    /* Path A: 7 scalars — do_msort + 2×(k,i,s); s1 on stack [rsp+0]. */
                    int   k0 = (int)a0->t;
                    long  i0 = (long)a0->ival;
                    const char *s0 = (k0 == IR_ATOM) ? a0->sval : NULL;
                    b += bytes(1, "\xBF") + u32le((uint32_t)do_msort);       /* mov edi, do_msort */
                    b += bytes(1, "\xBE") + u32le((uint32_t)k0);             /* mov esi, k0 */
                    b += bytes(2, "\x48\xBA") + u64le((uint64_t)i0);         /* mov rdx, i0 */
                    if (s0) b += bytes(2, "\x48\xB9") + u64le((uint64_t)(uintptr_t)s0);  /* mov rcx, s0 */
                    else    b += bytes(2, "\x31\xC9");                       /* xor ecx, ecx */
                    b += bytes(2, "\x41\xB8") + u32le((uint32_t)k1);         /* mov r8d, k1 */
                    b += bytes(2, "\x49\xB9") + u64le((uint64_t)i1);         /* mov r9, i1 */
                    if (s1) b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)s1);  /* mov rax, s1 */
                    else    b += bytes(2, "\x31\xC0");                       /* xor eax, eax */
                    b += bytes(4, "\x48\x89\x04\x24");            /* mov [rsp+0], rax */
                    b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_sort_msort) + bytes(2, "\xFF\xD0");
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
        if ((strcmp(fn, "atomic_list_concat") == 0 || strcmp(fn, "concat_atom") == 0)
            && pBB->α && (_.op_ival == 2 || _.op_ival == 3)) {
            int   arity = (int)_.op_ival;
            IR_t *a0 = pBB->α, *a1 = a0->γ, *a2 = a1 ? a1->γ : NULL;
            IR_t *sepN = (arity == 3) ? a1 : NULL;
            IR_t *resN = (arity == 3) ? a2 : a1;
            int   ksep = sepN ? (int)sepN->t : 0;
            long  isep = sepN ? (long)sepN->ival : 0;
            char  ssepl[64]; ssepl[0] = 0;
            if (sepN && ksep == IR_ATOM && sepN->sval) strtab_label(ssepl, sizeof ssepl, sepN->sval);
            int   kres = resN ? (int)resN->t : 0;
            long  ires = resN ? (long)resN->ival : 0;
            char  sresl[64]; sresl[0] = 0;
            if (resN && kres == IR_ATOM && resN->sval) strtab_label(sresl, sizeof sresl, resN->sval);
            return hdr
                 + x86("ins2", "sub", "rsp, 16")
                 + emit_build_compound_term(a0)
                 + x86("ins2", "mov", "rdi, rax")
                 + x86("ins2", "mov esi,", emit_fmt("%d", arity))
                 + x86("ins2", "mov edx,", emit_fmt("%d", ksep))
                 + x86("ins2", "mov rcx,", emit_fmt("%ld", isep))
                 + (ssepl[0] ? x86("ins2", "lea r8,", emit_fmt("[rip + %s]", ssepl)) : x86("ins2", "xor", "r8d, r8d"))
                 + x86("ins2", "mov r9d,", emit_fmt("%d", kres))
                 + x86("ins2", "mov rax,", emit_fmt("%ld", ires))
                 + x86("ins2", "mov", "[rsp + 0], rax")
                 + (sresl[0]
                       ? (x86("ins2", "lea rax,", emit_fmt("[rip + %s]", sresl)) + x86("ins2", "mov", "[rsp + 8], rax"))
                       : x86("ins2", "mov", "qword ptr [rsp + 8], 0"))
                 + x86("ins2", "call", "rt_atomic_list_concat_term@PLT")
                 + x86("ins2", "add", "rsp, 16")
                 + x86("ins2", "test", "eax, eax")
                 + x86("ins2", "je",   _.lbl_ω)
                 + x86("ins2", "jmp",  _.lbl_γ)
                 + x86("Lins2", emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω);
        }
        if ((strcmp(fn,"sort")==0 || strcmp(fn,"msort")==0) && pBB->α && pBB->α->γ) {
            IR_t *a0 = pBB->α, *a1 = a0->γ;
            int  do_msort = (strcmp(fn,"msort")==0) ? 1 : 0;
            int  k1 = (int)a1->t;
            long i1 = (long)a1->ival;
            char s1lbl[64]; s1lbl[0] = 0;
            if (k1 == IR_ATOM && a1->sval) strtab_label(s1lbl, sizeof s1lbl, a1->sval);
            if (a0->t == IR_STRUCT) {
                return hdr
                     + x86("ins2", "sub", "rsp, 16")
                     + emit_build_compound_term(a0)
                     + x86("ins2", "mov", "rsi, rax")
                     + x86("ins2", "mov edi,",  emit_fmt("%d",  do_msort))
                     + x86("ins2", "mov edx,",  emit_fmt("%d",  k1))
                     + x86("ins2", "mov rcx,",  emit_fmt("%ld", i1))
                     + (s1lbl[0] ? x86("ins2", "lea r8,", emit_fmt("[rip + %s]", s1lbl)) : x86("ins2", "xor", "r8d, r8d"))
                     + x86("ins2", "call", "rt_sort_msort_term@PLT")
                     + x86("ins2", "add", "rsp, 16")
                     + x86("ins2", "test", "eax, eax")
                     + x86("ins2", "je",   _.lbl_ω)
                     + x86("ins2", "jmp",  _.lbl_γ)
                     + x86("Lins2", emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω);
            }
            int  k0 = (int)a0->t;
            long i0 = (long)a0->ival;
            char s0lbl[64]; s0lbl[0] = 0;
            if (k0 == IR_ATOM && a0->sval) strtab_label(s0lbl, sizeof s0lbl, a0->sval);
            return hdr
                 + x86("ins2", "sub", "rsp, 16")
                 + x86("ins2", "mov edi,",  emit_fmt("%d",  do_msort))
                 + x86("ins2", "mov esi,",  emit_fmt("%d",  k0))
                 + x86("ins2", "mov rdx,",  emit_fmt("%ld", i0))
                 + (s0lbl[0] ? x86("ins2", "lea rcx,", emit_fmt("[rip + %s]", s0lbl)) : x86("ins2", "xor", "ecx, ecx"))
                 + x86("ins2", "mov r8d,",  emit_fmt("%d",  k1))
                 + x86("ins2", "mov r9,",   emit_fmt("%ld", i1))
                 + (s1lbl[0] ? x86("ins2", "lea rax,", emit_fmt("[rip + %s]", s1lbl)) : x86("ins2", "xor", "eax, eax"))
                 + x86("ins2", "mov", "qword ptr [rsp + 0], rax")
                 + x86("ins2", "call", "rt_sort_msort@PLT")
                 + x86("ins2", "add", "rsp, 16")
                 + x86("ins2", "test", "eax, eax")
                 + x86("ins2", "je",   _.lbl_ω)
                 + x86("ins2", "jmp",  _.lbl_γ)
                 + x86("Lins2", emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω);
        }
    }
    return std::string();
}
