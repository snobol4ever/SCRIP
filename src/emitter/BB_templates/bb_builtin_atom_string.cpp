#include "bb_builtin_common.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_builtin_atom_string_str(IR_t *pBB, const char *fn, const std::string &hdr) {
    (void)pBB; (void)fn; (void)hdr;
    if (MEDIUM_BINARY) {
            if (strcmp(fn, "copy_term") == 0 && pBB->α && pBB->α->γ
                && (pBB->α->t == IR_STRUCT || pBB->α->t == IR_ARITH)) {
                IR_t *a0 = pBB->α, *a1 = a0->γ;
                int  a1_compound = (a1->t == IR_STRUCT || a1->t == IR_ARITH);
                std::string b;
                b += bytes(4, "\x48\x83\xEC\x10");                /* sub rsp, 16 */
                if (a1_compound) {
                    b += emit_term_from_node_bin(a0);        /* build arg0 → rax */
                    b += bytes(4, "\x48\x89\x04\x24");            /* mov [rsp+0], rax (hold across build) */
                    b += emit_term_from_node_bin(a1);        /* build arg1 → rax */
                    b += bytes(3, "\x48\x89\xC6");                /* mov rsi, rax (t1) */
                    b += bytes(4, "\x48\x8B\x3C\x24");            /* mov rdi, [rsp+0] (t0) */
                    b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_copy_term_terms) + bytes(2, "\xFF\xD0");
                } else {
                    int  k1 = (int)a1->t;
                    long i1 = (long)a1->ival;
                    const char *s1 = (k1 == IR_ATOM) ? a1->sval : NULL;
                    b += emit_term_from_node_bin(a0);        /* build arg0 → rax */
                    b += bytes(3, "\x48\x89\xC7");                /* mov rdi, rax */
                    b += bytes(1, "\xBE") + u32le((uint32_t)k1);  /* mov esi, k1 */
                    b += bytes(2, "\x48\xBA") + u64le((uint64_t)i1);  /* mov rdx, i1 */
                    if (s1) b += bytes(2, "\x48\xB9") + u64le((uint64_t)(uintptr_t)s1);  /* mov rcx, s1 */
                    else    b += bytes(2, "\x31\xC9");            /* xor ecx, ecx */
                    b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_copy_term_term) + bytes(2, "\xFF\xD0");
                }
                b += bytes(4, "\x48\x83\xC4\x10");                /* add rsp, 16 */
                b += bytes(2, "\x85\xC0");                        /* test eax, eax */
                return x86_lit_bytes(b) + x86("je", PORT_OMEGA) + x86("jmp", PORT_GAMMA) + x86("jmp", PORT_OMEGA);
            }
            if ((strcmp(fn,"atom_length")==0   || strcmp(fn,"upcase_atom")==0   || strcmp(fn,"downcase_atom")==0
              || strcmp(fn,"string_length")==0 || strcmp(fn,"string_upper")==0  || strcmp(fn,"string_lower")==0
              || strcmp(fn,"atom_string")==0   || strcmp(fn,"string_to_atom")==0 || strcmp(fn,"copy_term")==0)
                && pBB->α && pBB->α->γ) {
                IR_t *a0 = pBB->α, *a1 = a0->γ;
                int  k0 = (int)a0->t,    k1 = (int)a1->t;
                long i0 = (long)a0->ival, i1 = (long)a1->ival;
                const char *s0 = (k0 == IR_ATOM) ? a0->sval : NULL;
                const char *s1 = (k1 == IR_ATOM) ? a1->sval : NULL;
                void *callee =
                    (strcmp(fn,"atom_length")==0   || strcmp(fn,"string_length")==0) ? (void*)rt_atom_length   :
                    (strcmp(fn,"upcase_atom")==0   || strcmp(fn,"string_upper")==0)  ? (void*)rt_upcase_atom   :
                    (strcmp(fn,"downcase_atom")==0 || strcmp(fn,"string_lower")==0)  ? (void*)rt_downcase_atom :
                    (strcmp(fn,"copy_term")==0)                                      ? (void*)rt_copy_term     :
                                                                                       (void*)rt_atom_string_pair;
                std::string b;
                /* mov edi, k0    BF [4]                                                                 */
                b += bytes(1, "\xBF") + u32le((uint32_t)k0);
                /* mov rsi, i0    48 BE [8]                                                              */
                b += bytes(2, "\x48\xBE") + u64le((uint64_t)i0);
                /* mov rdx, s0    48 BA [8]   (or xor edx,edx  31 D2)                                    */
                if (s0) b += bytes(2, "\x48\xBA") + u64le((uint64_t)(uintptr_t)s0);
                else    b += bytes(2, "\x31\xD2");
                /* mov ecx, k1    B9 [4]                                                                 */
                b += bytes(1, "\xB9") + u32le((uint32_t)k1);
                /* mov r8, i1     49 B8 [8]                                                              */
                b += bytes(2, "\x49\xB8") + u64le((uint64_t)i1);
                /* mov r9, s1     49 B9 [8]   (or xor r9d,r9d  45 31 C9)                                 */
                if (s1) b += bytes(2, "\x49\xB9") + u64le((uint64_t)(uintptr_t)s1);
                else    b += bytes(3, "\x45\x31\xC9");
                /* movabs rax, &callee; call rax   48 B8 [8] FF D0                                       */
                b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)callee) + bytes(2, "\xFF\xD0");
                /* test eax, eax   85 C0                                                                 */
                b += bytes(2, "\x85\xC0");
                return x86_lit_bytes(b) + x86("je", PORT_OMEGA) + x86("jmp", PORT_GAMMA) + x86("jmp", PORT_OMEGA);
            }
            if ((strcmp(fn,"number_string")==0 || strcmp(fn,"atom_number")==0)
                && pBB->α && pBB->α->γ) {
                IR_t *a0 = pBB->α, *a1 = a0->γ;
                int  num_first = (strcmp(fn,"number_string")==0) ? 1 : 0;
                int  k0 = (int)a0->t,    k1 = (int)a1->t;
                long i0 = (long)a0->ival, i1 = (long)a1->ival;
                const char *s0 = (k0 == IR_ATOM) ? a0->sval : NULL;
                const char *s1 = (k1 == IR_ATOM) ? a1->sval : NULL;
                std::string b;
                /* sub rsp, 16     48 83 EC 10                                                           */
                b += bytes(4, "\x48\x83\xEC\x10");
                /* mov edi, num_first    BF [4]                                                          */
                b += bytes(1, "\xBF") + u32le((uint32_t)num_first);
                /* mov esi, k0    BE [4]                                                                 */
                b += bytes(1, "\xBE") + u32le((uint32_t)k0);
                /* mov rdx, i0    48 BA [8]                                                              */
                b += bytes(2, "\x48\xBA") + u64le((uint64_t)i0);
                /* mov rcx, s0    48 B9 [8]   (or xor ecx,ecx  31 C9)                                    */
                if (s0) b += bytes(2, "\x48\xB9") + u64le((uint64_t)(uintptr_t)s0);
                else    b += bytes(2, "\x31\xC9");
                /* mov r8d, k1    41 B8 [4]                                                              */
                b += bytes(2, "\x41\xB8") + u32le((uint32_t)k1);
                /* mov r9, i1     49 B9 [8]                                                              */
                b += bytes(2, "\x49\xB9") + u64le((uint64_t)i1);
                /* mov rax, s1 (or xor eax,eax) ; mov [rsp+0], rax    48 B8[8]/31 C0 ; 48 89 04 24       */
                if (s1) b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)s1);
                else    b += bytes(2, "\x31\xC0");
                b += bytes(4, "\x48\x89\x04\x24");
                /* movabs rax, &rt_number_string_pair; call rax                                       */
                b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_number_string_pair) + bytes(2, "\xFF\xD0");
                /* add rsp, 16     48 83 C4 10                                                           */
                b += bytes(4, "\x48\x83\xC4\x10");
                /* test eax, eax   85 C0                                                                 */
                b += bytes(2, "\x85\xC0");
                return x86_lit_bytes(b) + x86("je", PORT_OMEGA) + x86("jmp", PORT_GAMMA) + x86("jmp", PORT_OMEGA);
            }
            if ((strcmp(fn,"atom_concat")==0 || strcmp(fn,"string_concat")==0)
                && pBB->α && pBB->α->γ && pBB->α->γ->γ) {
                IR_t *a0 = pBB->α, *a1 = a0->γ, *a2 = a1->γ;
                int  k0 = (int)a0->t,    k1 = (int)a1->t,    k2 = (int)a2->t;
                long i0 = (long)a0->ival, i1 = (long)a1->ival, i2 = (long)a2->ival;
                const char *s0 = (k0 == IR_ATOM) ? a0->sval : NULL;
                const char *s1 = (k1 == IR_ATOM) ? a1->sval : NULL;
                const char *s2 = (k2 == IR_ATOM) ? a2->sval : NULL;
                std::string b;
                /* sub rsp, 32     48 83 EC 20                                                           */
                b += bytes(4, "\x48\x83\xEC\x20");
                /* mov edi, k0 ; mov rsi, i0 ; mov rdx, s0                                               */
                b += bytes(1, "\xBF") + u32le((uint32_t)k0);
                b += bytes(2, "\x48\xBE") + u64le((uint64_t)i0);
                if (s0) b += bytes(2, "\x48\xBA") + u64le((uint64_t)(uintptr_t)s0);
                else    b += bytes(2, "\x31\xD2");
                /* mov ecx, k1 ; mov r8, i1 ; mov r9, s1                                                 */
                b += bytes(1, "\xB9") + u32le((uint32_t)k1);
                b += bytes(2, "\x49\xB8") + u64le((uint64_t)i1);
                if (s1) b += bytes(2, "\x49\xB9") + u64le((uint64_t)(uintptr_t)s1);
                else    b += bytes(3, "\x45\x31\xC9");
                /* mov dword ptr [rsp+0], k2    C7 04 24 [4]                                             */
                b += bytes(3, "\xC7\x04\x24") + u32le((uint32_t)k2);
                /* mov rax, i2 ; mov [rsp+8], rax    48 B8 [8] ; 48 89 44 24 08                          */
                b += bytes(2, "\x48\xB8") + u64le((uint64_t)i2);
                b += bytes(5, "\x48\x89\x44\x24\x08");
                /* mov rax, s2 (or xor eax,eax) ; mov [rsp+16], rax    48 B8[8]/31 C0 ; 48 89 44 24 10   */
                if (s2) b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)s2);
                else    b += bytes(2, "\x31\xC0");
                b += bytes(5, "\x48\x89\x44\x24\x10");
                /* movabs rax, &rt_atom_concat; call rax                                              */
                b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_atom_concat) + bytes(2, "\xFF\xD0");
                /* add rsp, 32     48 83 C4 20                                                           */
                b += bytes(4, "\x48\x83\xC4\x20");
                /* test eax, eax   85 C0                                                                 */
                b += bytes(2, "\x85\xC0");
                return x86_lit_bytes(b) + x86("je", PORT_OMEGA) + x86("jmp", PORT_GAMMA) + x86("jmp", PORT_OMEGA);
            }
            if ((strcmp(fn,"atom_chars")==0 || strcmp(fn,"atom_codes")==0
              || strcmp(fn,"string_chars")==0 || strcmp(fn,"string_codes")==0)
                && pBB->α && pBB->α->γ) {
                IR_t *a0 = pBB->α, *a1 = a0->γ;
                int  k0 = (int)a0->t;
                long i0 = (long)a0->ival;
                const char *s0 = (k0 == IR_ATOM) ? a0->sval : NULL;
                int  as_codes = (strcmp(fn,"atom_codes")==0 || strcmp(fn,"string_codes")==0) ? 1 : 0;
                std::string b;
                if (a1->t == IR_STRUCT) {
                    /* Path B: build the list Term* then call the _term helper.                          */
                    /* sub rsp, 8     48 83 EC 08   (align: one odd push to balance the build's call)     */
                    b += bytes(4, "\x48\x83\xEC\x08");
                    b += emit_term_from_node_bin(a1);
                    /* mov r8, rax    49 89 C0                                                            */
                    b += bytes(3, "\x49\x89\xC0");
                    /* mov edi, as_codes ; mov esi, k0 ; mov rdx, i0 ; mov rcx, s0                        */
                    b += bytes(1, "\xBF") + u32le((uint32_t)as_codes);
                    b += bytes(1, "\xBE") + u32le((uint32_t)k0);
                    b += bytes(2, "\x48\xBA") + u64le((uint64_t)i0);
                    if (s0) b += bytes(2, "\x48\xB9") + u64le((uint64_t)(uintptr_t)s0);
                    else    b += bytes(2, "\x31\xC9");
                    b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_atom_chars_codes_term) + bytes(2, "\xFF\xD0");
                    /* add rsp, 8     48 83 C4 08                                                         */
                    b += bytes(4, "\x48\x83\xC4\x08");
                } else {
                    /* Path A: scalar a1 (VAR or ATOM). 7th arg s1 on the stack.                         */
                    int  k1 = (int)a1->t;
                    long i1 = (long)a1->ival;
                    const char *s1 = (k1 == IR_ATOM) ? a1->sval : NULL;
                    /* sub rsp, 16    48 83 EC 10                                                         */
                    b += bytes(4, "\x48\x83\xEC\x10");
                    /* mov edi, as_codes ; mov esi, k0 ; mov rdx, i0 ; mov rcx, s0                        */
                    b += bytes(1, "\xBF") + u32le((uint32_t)as_codes);
                    b += bytes(1, "\xBE") + u32le((uint32_t)k0);
                    b += bytes(2, "\x48\xBA") + u64le((uint64_t)i0);
                    if (s0) b += bytes(2, "\x48\xB9") + u64le((uint64_t)(uintptr_t)s0);
                    else    b += bytes(2, "\x31\xC9");
                    /* mov r8d, k1 ; mov r9, i1                                                           */
                    b += bytes(2, "\x41\xB8") + u32le((uint32_t)k1);
                    b += bytes(2, "\x49\xB9") + u64le((uint64_t)i1);
                    /* mov rax, s1 (or xor eax,eax) ; mov [rsp+0], rax                                    */
                    if (s1) b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)s1);
                    else    b += bytes(2, "\x31\xC0");
                    b += bytes(4, "\x48\x89\x04\x24");
                    b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_atom_chars_codes) + bytes(2, "\xFF\xD0");
                    /* add rsp, 16    48 83 C4 10                                                         */
                    b += bytes(4, "\x48\x83\xC4\x10");
                }
                b += bytes(2, "\x85\xC0");
                return x86_lit_bytes(b) + x86("je", PORT_OMEGA) + x86("jmp", PORT_GAMMA) + x86("jmp", PORT_OMEGA);
            }
            if (strcmp(fn,"char_type")==0 && _.op_ival==2 && pBB->α && pBB->α->γ) {
                IR_t *a0 = pBB->α, *a1 = a0->γ;
                int  k0 = (int)a0->t;
                long i0 = (long)a0->ival;
                const char *s0 = (k0 == IR_ATOM) ? a0->sval : NULL;
                int  is_compound = ((a1->t == IR_STRUCT || a1->t == IR_ARITH) && a1->sval) ? 1 : 0;
                const char *ty = a1->sval;
                IR_t *inner = is_compound ? a1->α : NULL;
                int  ki = inner ? (int)inner->t : 0;
                long ii = inner ? (long)inner->ival : 0;
                const char *si = (inner && inner->t == IR_ATOM) ? inner->sval : NULL;
                std::string b;
                /* sub rsp, 16    48 83 EC 10                                                          */
                b += bytes(4, "\x48\x83\xEC\x10");
                /* mov edi, k0 ; mov rsi, i0 ; mov rdx, s0 (or xor edx,edx)                            */
                b += bytes(1, "\xBF") + u32le((uint32_t)k0);
                b += bytes(2, "\x48\xBE") + u64le((uint64_t)i0);
                if (s0) b += bytes(2, "\x48\xBA") + u64le((uint64_t)(uintptr_t)s0);
                else    b += bytes(2, "\x31\xD2");
                /* mov rcx, ty (or xor ecx,ecx)    48 B9 [8] / 31 C9                                   */
                if (ty) b += bytes(2, "\x48\xB9") + u64le((uint64_t)(uintptr_t)ty);
                else    b += bytes(2, "\x31\xC9");
                /* mov r8d, is_compound    41 B8 [4]                                                   */
                b += bytes(2, "\x41\xB8") + u32le((uint32_t)is_compound);
                /* mov r9d, ki             41 B9 [4]                                                   */
                b += bytes(2, "\x41\xB9") + u32le((uint32_t)ki);
                /* mov rax, ii ; mov [rsp+0], rax    48 B8 [8] ; 48 89 04 24                           */
                b += bytes(2, "\x48\xB8") + u64le((uint64_t)ii);
                b += bytes(4, "\x48\x89\x04\x24");
                /* mov rax, si (or xor eax,eax) ; mov [rsp+8], rax                                     */
                if (si) b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)si);
                else    b += bytes(2, "\x31\xC0");
                b += bytes(5, "\x48\x89\x44\x24\x08");
                /* movabs rax, &rt_char_type; call rax                                              */
                b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_char_type) + bytes(2, "\xFF\xD0");
                /* add rsp, 16    48 83 C4 10                                                          */
                b += bytes(4, "\x48\x83\xC4\x10");
                /* test eax, eax   85 C0                                                               */
                b += bytes(2, "\x85\xC0");
                return x86_lit_bytes(b) + x86("je", PORT_OMEGA) + x86("jmp", PORT_GAMMA) + x86("jmp", PORT_OMEGA);
            }
    }
    if (MEDIUM_TEXT) {
    std::string succ_back = x86("ins2", "jmp", _.lbl_γ)
                          + x86("Lins2", emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_γ);
    (void)succ_back;
        if ((strcmp(fn,"atom_length")==0   || strcmp(fn,"upcase_atom")==0   || strcmp(fn,"downcase_atom")==0
          || strcmp(fn,"string_length")==0 || strcmp(fn,"string_upper")==0  || strcmp(fn,"string_lower")==0)
            && pBB->α && pBB->α->γ) {
            IR_t *a0 = pBB->α, *a1 = a0->γ;
            int  k0 = (int)a0->t,    k1 = (int)a1->t;
            long i0 = (long)a0->ival, i1 = (long)a1->ival;
            /* CAT-D-1 (2026-05-27): strtab_label only for nodes that genuinely carry an atom string.   */
            /* IR_LOGICVAR's sval is unionized with ival in the AST (see lower_pl.c:65 nd->sval =          */
            /* e->v.sval where the same union slot holds the variable slot index as ival), so for a     */
            /* variable arg, sval is garbage (typically the slot index reinterpreted as a pointer). The */
            /* helper does not need sval for VAR args anyway — it materializes them via the IR_LOGICVAR   */
            /* arm of rt_node_to_term, which reads only ival (the slot). Restrict strtab_label to    */
            /* atom/literal-typed args.                                                                 */
            char s0lbl[64]; s0lbl[0] = 0;
            char s1lbl[64]; s1lbl[0] = 0;
            /* Note: do NOT require *sval to be non-empty here — empty-atom '' (sval = "") is a   */
            /* distinct atom from [] (sval = NULL → defaults to "[]" in rt_node_to_term). For   */
            /* atom_length('', Z) the helper must see sval = "" so strlen returns 0. The empty    */
            /* string IS in the strtab (resolve_pre_intern_pred_names walks every IR_ATOM->sval).      */
            if (k0 == IR_ATOM && a0->sval) strtab_label(s0lbl, sizeof s0lbl, a0->sval);
            if (k1 == IR_ATOM && a1->sval) strtab_label(s1lbl, sizeof s1lbl, a1->sval);
            const char *helper =
                (strcmp(fn,"atom_length")==0   || strcmp(fn,"string_length")==0) ? "rt_atom_length@PLT"   :
                (strcmp(fn,"upcase_atom")==0   || strcmp(fn,"string_upper")==0)  ? "rt_upcase_atom@PLT"   :
                                                                                   "rt_downcase_atom@PLT";
            return hdr
                 + x86("ins2", "mov edi,",  emit_fmt("%d",  k0))
                 + x86("ins2", "mov rsi,",  emit_fmt("%ld", i0))
                 + (s0lbl[0] ? x86("ins2", "lea rdx,", emit_fmt("[rip + %s]", s0lbl)) : x86("ins2", "xor", "edx, edx"))
                 + x86("ins2", "mov ecx,",  emit_fmt("%d",  k1))
                 + x86("ins2", "mov r8,",   emit_fmt("%ld", i1))
                 + (s1lbl[0] ? x86("ins2", "lea r9,", emit_fmt("[rip + %s]", s1lbl)) : x86("ins2", "xor", "r9d, r9d"))
                 + x86("ins2", "call", helper)
                 + x86("ins2", "test", "eax, eax")
                 + x86("ins2", "je",   _.lbl_ω)
                 + x86("ins2", "jmp",  _.lbl_γ)
                 + x86("Lins2", emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω);
        }
        if (strcmp(fn, "copy_term") == 0 && pBB->α && pBB->α->γ
            && (pBB->α->t == IR_STRUCT || pBB->α->t == IR_ARITH)) {
            IR_t *a0 = pBB->α, *a1 = a0->γ;
            int   a1_compound = (a1->t == IR_STRUCT || a1->t == IR_ARITH);
            std::string b = hdr + x86("ins2", "sub", "rsp, 16");
            if (a1_compound) {
                b += emit_build_compound_term(a0)
                   + x86("ins2", "mov", "qword ptr [rsp + 0], rax")
                   + emit_build_compound_term(a1)
                   + x86("ins2", "mov", "rsi, rax")
                   + x86("ins2", "mov", "rdi, qword ptr [rsp + 0]")
                   + x86("ins2", "call", "rt_copy_term_terms@PLT");
            } else {
                int  k1 = (int)a1->t;
                long i1 = (long)a1->ival;
                char s1lbl[64]; s1lbl[0] = 0;
                if (k1 == IR_ATOM && a1->sval) strtab_label(s1lbl, sizeof s1lbl, a1->sval);
                b += emit_build_compound_term(a0)
                   + x86("ins2", "mov", "rdi, rax")
                   + x86("ins2", "mov esi,", emit_fmt("%d", k1))
                   + x86("ins2", "mov rdx,", emit_fmt("%ld", i1))
                   + (s1lbl[0] ? x86("ins2", "lea rcx,", emit_fmt("[rip + %s]", s1lbl)) : x86("ins2", "xor", "ecx, ecx"))
                   + x86("ins2", "call", "rt_copy_term_term@PLT");
            }
            return b + x86("ins2", "add", "rsp, 16")
                     + x86("ins2", "test", "eax, eax")
                     + x86("ins2", "je",   _.lbl_ω)
                     + x86("ins2", "jmp",  _.lbl_γ)
                     + x86("Lins2", emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω);
        }
        if ((strcmp(fn,"atom_string")==0 || strcmp(fn,"string_to_atom")==0 || strcmp(fn,"copy_term")==0)
            && pBB->α && pBB->α->γ) {
            IR_t *a0 = pBB->α, *a1 = a0->γ;
            int  k0 = (int)a0->t,    k1 = (int)a1->t;
            long i0 = (long)a0->ival, i1 = (long)a1->ival;
            char s0lbl[64]; s0lbl[0] = 0;
            char s1lbl[64]; s1lbl[0] = 0;
            if (k0 == IR_ATOM && a0->sval) strtab_label(s0lbl, sizeof s0lbl, a0->sval);
            if (k1 == IR_ATOM && a1->sval) strtab_label(s1lbl, sizeof s1lbl, a1->sval);
            const char *helper2 =
                (strcmp(fn,"copy_term")==0) ? "rt_copy_term@PLT"
                                            : "rt_atom_string_pair@PLT";
            return hdr
                 + x86("ins2", "mov edi,",  emit_fmt("%d",  k0))
                 + x86("ins2", "mov rsi,",  emit_fmt("%ld", i0))
                 + (s0lbl[0] ? x86("ins2", "lea rdx,", emit_fmt("[rip + %s]", s0lbl)) : x86("ins2", "xor", "edx, edx"))
                 + x86("ins2", "mov ecx,",  emit_fmt("%d",  k1))
                 + x86("ins2", "mov r8,",   emit_fmt("%ld", i1))
                 + (s1lbl[0] ? x86("ins2", "lea r9,", emit_fmt("[rip + %s]", s1lbl)) : x86("ins2", "xor", "r9d, r9d"))
                 + x86("ins2", "call", helper2)
                 + x86("ins2", "test", "eax, eax")
                 + x86("ins2", "je",   _.lbl_ω)
                 + x86("ins2", "jmp",  _.lbl_γ)
                 + x86("Lins2", emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω);
        }
        if ((strcmp(fn,"number_string")==0 || strcmp(fn,"atom_number")==0)
            && pBB->α && pBB->α->γ) {
            IR_t *a0 = pBB->α, *a1 = a0->γ;
            int  num_first = (strcmp(fn,"number_string")==0) ? 1 : 0;
            int  k0 = (int)a0->t,    k1 = (int)a1->t;
            long i0 = (long)a0->ival, i1 = (long)a1->ival;
            char s0lbl[64]; s0lbl[0] = 0;
            char s1lbl[64]; s1lbl[0] = 0;
            if (k0 == IR_ATOM && a0->sval) strtab_label(s0lbl, sizeof s0lbl, a0->sval);
            if (k1 == IR_ATOM && a1->sval) strtab_label(s1lbl, sizeof s1lbl, a1->sval);
            return hdr
                 + x86("ins2", "sub", "rsp, 16")
                 + x86("ins2", "mov edi,",  emit_fmt("%d",  num_first))
                 + x86("ins2", "mov esi,",  emit_fmt("%d",  k0))
                 + x86("ins2", "mov rdx,",  emit_fmt("%ld", i0))
                 + (s0lbl[0] ? x86("ins2", "lea rcx,", emit_fmt("[rip + %s]", s0lbl)) : x86("ins2", "xor", "ecx, ecx"))
                 + x86("ins2", "mov r8d,",  emit_fmt("%d",  k1))
                 + x86("ins2", "mov r9,",   emit_fmt("%ld", i1))
                 + (s1lbl[0] ? x86("ins2", "lea rax,", emit_fmt("[rip + %s]", s1lbl)) : x86("ins2", "xor", "eax, eax"))
                 + x86("ins2", "mov", "qword ptr [rsp + 0], rax")
                 + x86("ins2", "call", "rt_number_string_pair@PLT")
                 + x86("ins2", "add", "rsp, 16")
                 + x86("ins2", "test", "eax, eax")
                 + x86("ins2", "je",   _.lbl_ω)
                 + x86("ins2", "jmp",  _.lbl_γ)
                 + x86("Lins2", emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω);
        }
        if ((strcmp(fn,"atom_concat")==0 || strcmp(fn,"string_concat")==0)
            && pBB->α && pBB->α->γ && pBB->α->γ->γ) {
            IR_t *a0 = pBB->α, *a1 = a0->γ, *a2 = a1->γ;
            int  k0 = (int)a0->t,    k1 = (int)a1->t,    k2 = (int)a2->t;
            long i0 = (long)a0->ival, i1 = (long)a1->ival, i2 = (long)a2->ival;
            char s0lbl[64]; s0lbl[0] = 0;
            char s1lbl[64]; s1lbl[0] = 0;
            char s2lbl[64]; s2lbl[0] = 0;
            if (k0 == IR_ATOM && a0->sval) strtab_label(s0lbl, sizeof s0lbl, a0->sval);
            if (k1 == IR_ATOM && a1->sval) strtab_label(s1lbl, sizeof s1lbl, a1->sval);
            if (k2 == IR_ATOM && a2->sval) strtab_label(s2lbl, sizeof s2lbl, a2->sval);
            return hdr
                 + x86("ins2", "sub", "rsp, 32")
                 + x86("ins2", "mov edi,",  emit_fmt("%d",  k0))
                 + x86("ins2", "mov rsi,",  emit_fmt("%ld", i0))
                 + (s0lbl[0] ? x86("ins2", "lea rdx,", emit_fmt("[rip + %s]", s0lbl)) : x86("ins2", "xor", "edx, edx"))
                 + x86("ins2", "mov ecx,",  emit_fmt("%d",  k1))
                 + x86("ins2", "mov r8,",   emit_fmt("%ld", i1))
                 + (s1lbl[0] ? x86("ins2", "lea r9,", emit_fmt("[rip + %s]", s1lbl)) : x86("ins2", "xor", "r9d, r9d"))
                 + x86("ins2", "mov dword ptr [rsp + 0],", emit_fmt("%d",  k2))
                 + x86("ins2", "mov rax,",  emit_fmt("%ld", i2))
                 + x86("ins2", "mov", "qword ptr [rsp + 8], rax")
                 + (s2lbl[0] ? x86("ins2", "lea rax,", emit_fmt("[rip + %s]", s2lbl)) : x86("ins2", "xor", "eax, eax"))
                 + x86("ins2", "mov", "qword ptr [rsp + 16], rax")
                 + x86("ins2", "call", "rt_atom_concat@PLT")
                 + x86("ins2", "add", "rsp, 32")
                 + x86("ins2", "test", "eax, eax")
                 + x86("ins2", "je",   _.lbl_ω)
                 + x86("ins2", "jmp",  _.lbl_γ)
                 + x86("Lins2", emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω);
        }
        if ((strcmp(fn,"atom_chars")==0 || strcmp(fn,"atom_codes")==0
          || strcmp(fn,"string_chars")==0 || strcmp(fn,"string_codes")==0)
            && pBB->α && pBB->α->γ) {
            IR_t *a0 = pBB->α, *a1 = a0->γ;
            int  k0 = (int)a0->t;
            long i0 = (long)a0->ival;
            int  as_codes = (strcmp(fn,"atom_codes")==0 || strcmp(fn,"string_codes")==0) ? 1 : 0;
            char s0lbl[64]; s0lbl[0] = 0;
            if (k0 == IR_ATOM && a0->sval) strtab_label(s0lbl, sizeof s0lbl, a0->sval);
            if (a1->t == IR_STRUCT) {
                /* Path B: literal list in a1 — build Term* from cons-cell tree, pass as pointer. */
                return hdr
                     + x86("ins2", "sub", "rsp, 16")
                     + emit_build_compound_term(a1)
                     + x86("ins2", "mov", "r8, rax")
                     + x86("ins2", "mov edi,",  emit_fmt("%d",  as_codes))
                     + x86("ins2", "mov esi,",  emit_fmt("%d",  k0))
                     + x86("ins2", "mov rdx,",  emit_fmt("%ld", i0))
                     + (s0lbl[0] ? x86("ins2", "lea rcx,", emit_fmt("[rip + %s]", s0lbl)) : x86("ins2", "xor", "ecx, ecx"))
                     + x86("ins2", "call", "rt_atom_chars_codes_term@PLT")
                     + x86("ins2", "add", "rsp, 16")
                     + x86("ins2", "test", "eax, eax")
                     + x86("ins2", "je",   _.lbl_ω)
                     + x86("ins2", "jmp",  _.lbl_γ)
                     + x86("Lins2", emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω);
            }
            /* Path A: scalar a1 (VAR or ATOM). */
            int  k1 = (int)a1->t;
            long i1 = (long)a1->ival;
            char s1lbl[64]; s1lbl[0] = 0;
            if (k1 == IR_ATOM && a1->sval) strtab_label(s1lbl, sizeof s1lbl, a1->sval);
            return hdr
                 + x86("ins2", "sub", "rsp, 16")
                 + x86("ins2", "mov edi,",  emit_fmt("%d",  as_codes))
                 + x86("ins2", "mov esi,",  emit_fmt("%d",  k0))
                 + x86("ins2", "mov rdx,",  emit_fmt("%ld", i0))
                 + (s0lbl[0] ? x86("ins2", "lea rcx,", emit_fmt("[rip + %s]", s0lbl)) : x86("ins2", "xor", "ecx, ecx"))
                 + x86("ins2", "mov r8d,",  emit_fmt("%d",  k1))
                 + x86("ins2", "mov r9,",   emit_fmt("%ld", i1))
                 + (s1lbl[0] ? x86("ins2", "lea rax,", emit_fmt("[rip + %s]", s1lbl)) : x86("ins2", "xor", "eax, eax"))
                 + x86("ins2", "mov", "qword ptr [rsp + 0], rax")
                 + x86("ins2", "call", "rt_atom_chars_codes@PLT")
                 + x86("ins2", "add", "rsp, 16")
                 + x86("ins2", "test", "eax, eax")
                 + x86("ins2", "je",   _.lbl_ω)
                 + x86("ins2", "jmp",  _.lbl_γ)
                 + x86("Lins2", emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω);
        }
        if (strcmp(fn,"char_type")==0 && _.op_ival==2 && pBB->α && pBB->α->γ) {
            IR_t *a0 = pBB->α, *a1 = a0->γ;
            int  k0 = (int)a0->t;
            long i0 = (long)a0->ival;
            char s0lbl[64]; s0lbl[0] = 0;
            if (k0 == IR_ATOM && a0->sval) strtab_label(s0lbl, sizeof s0lbl, a0->sval);
            int  is_compound = ((a1->t == IR_STRUCT || a1->t == IR_ARITH) && a1->sval) ? 1 : 0;
            char tylbl[64]; tylbl[0] = 0;
            if (a1->sval) strtab_label(tylbl, sizeof tylbl, a1->sval);
            IR_t *inner = is_compound ? a1->α : NULL;
            int  ki = inner ? (int)inner->t : 0;
            long ii = inner ? (long)inner->ival : 0;
            char silbl[64]; silbl[0] = 0;
            if (inner && inner->t == IR_ATOM && inner->sval) strtab_label(silbl, sizeof silbl, inner->sval);
            return hdr
                 + x86("ins2", "sub", "rsp, 16")
                 + x86("ins2", "mov edi,",  emit_fmt("%d",  k0))
                 + x86("ins2", "mov rsi,",  emit_fmt("%ld", i0))
                 + (s0lbl[0] ? x86("ins2", "lea rdx,", emit_fmt("[rip + %s]", s0lbl)) : x86("ins2", "xor", "edx, edx"))
                 + (tylbl[0] ? x86("ins2", "lea rcx,", emit_fmt("[rip + %s]", tylbl)) : x86("ins2", "xor", "ecx, ecx"))
                 + x86("ins2", "mov r8d,",  emit_fmt("%d",  is_compound))
                 + x86("ins2", "mov r9d,",  emit_fmt("%d",  ki))
                 + x86("ins2", "mov rax,",  emit_fmt("%ld", ii))
                 + x86("ins2", "mov", "qword ptr [rsp + 0], rax")
                 + (silbl[0] ? x86("ins2", "lea rax,", emit_fmt("[rip + %s]", silbl)) : x86("ins2", "xor", "eax, eax"))
                 + x86("ins2", "mov", "qword ptr [rsp + 8], rax")
                 + x86("ins2", "call", "rt_char_type@PLT")
                 + x86("ins2", "add", "rsp, 16")
                 + x86("ins2", "test", "eax, eax")
                 + x86("ins2", "je",   _.lbl_ω)
                 + x86("ins2", "jmp",  _.lbl_γ)
                 + x86("Lins2", emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω);
        }
    }
    return std::string();
}
