#include "bb_common.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_atom_string_str(IR_t *pBB, const char *fn, const std::string &hdr) {
    (void)pBB; (void)fn; (void)hdr;
    if (MEDIUM_BINARY) {
            if (strcmp(fn, "copy_term") == 0 && pBB->α && pBB->α->γ
                && (pBB->α->t == IR_STRUCT || pBB->α->t == IR_ARITH)) {
                IR_t *a0 = pBB->α, *a1 = a0->γ;
                int  a1_compound = (a1->t == IR_STRUCT || a1->t == IR_ARITH);
                std::string b;
                b += x86("sub", "rsp", 16L);
                if (a1_compound) {
                    b += x86_lit_bytes(emit_term_from_node_bin(a0));
                    b += x86("mov", RSP(0), "rax");
                    b += x86_lit_bytes(emit_term_from_node_bin(a1));
                    b += x86("mov", "rsi", "rax");
                    b += x86("mov", "rdi", RSP(0));
                    b += x86("call", "rt_copy_term_terms", (unsigned long long)(uintptr_t)(void*)rt_copy_term_terms);
                } else {
                    int  k1 = (int)a1->t;
                    long i1 = (long)IR_LIT(a1).ival;
                    const char *s1 = (k1 == IR_ATOM) ? IR_LIT(a1).sval : NULL;
                    b += x86_lit_bytes(emit_term_from_node_bin(a0));
                    b += x86("mov", "rdi", "rax");
                    b += x86("mov32", "esi", (long)k1);
                    b += x86("movabs", "rdx", (unsigned long long)(uint64_t)i1);
                    if (s1) b += x86("movabs", "rcx", (unsigned long long)(uintptr_t)s1);
                    else    b += x86("xor", "ecx", "ecx");
                    b += x86("call", "rt_copy_term_term", (unsigned long long)(uintptr_t)(void*)rt_copy_term_term);
                }
                b += x86("add", "rsp", 16L);
                b += x86("test", "eax", "eax");
                return b + x86("je", "ω") + x86("jmp", "γ") + x86("jmp", "ω");
            }
            if ((strcmp(fn,"atom_length")==0   || strcmp(fn,"upcase_atom")==0   || strcmp(fn,"downcase_atom")==0
              || strcmp(fn,"string_length")==0 || strcmp(fn,"string_upper")==0  || strcmp(fn,"string_lower")==0
              || strcmp(fn,"atom_string")==0   || strcmp(fn,"string_to_atom")==0 || strcmp(fn,"copy_term")==0)
                && pBB->α && pBB->α->γ) {
                IR_t *a0 = pBB->α, *a1 = a0->γ;
                int  k0 = (int)a0->t,    k1 = (int)a1->t;
                long i0 = (long)IR_LIT(a0).ival, i1 = (long)IR_LIT(a1).ival;
                const char *s0 = (k0 == IR_ATOM) ? IR_LIT(a0).sval : NULL;
                const char *s1 = (k1 == IR_ATOM) ? IR_LIT(a1).sval : NULL;
                void *callee =
                    (strcmp(fn,"atom_length")==0   || strcmp(fn,"string_length")==0) ? (void*)rt_atom_length   :
                    (strcmp(fn,"upcase_atom")==0   || strcmp(fn,"string_upper")==0)  ? (void*)rt_upcase_atom   :
                    (strcmp(fn,"downcase_atom")==0 || strcmp(fn,"string_lower")==0)  ? (void*)rt_downcase_atom :
                    (strcmp(fn,"copy_term")==0)                                      ? (void*)rt_copy_term     :
                                                                                       (void*)rt_atom_string_pair;
                std::string b;
                b += x86("mov32", "edi", (long)k0);
                b += x86("movabs", "rsi", (unsigned long long)(uint64_t)i0);
                if (s0) b += x86("movabs", "rdx", (unsigned long long)(uintptr_t)s0);
                else    b += x86("xor", "edx", "edx");
                b += x86("mov32", "ecx", (long)k1);
                b += x86("movabs", "r8", (unsigned long long)(uint64_t)i1);
                if (s1) b += x86("movabs", "r9", (unsigned long long)(uintptr_t)s1);
                else    b += x86("xor", "r9d", "r9d");
                b += x86("call", "rt_atom_string_family", (unsigned long long)(uintptr_t)callee);
                b += x86("test", "eax", "eax");
                return b + x86("je", "ω") + x86("jmp", "γ") + x86("jmp", "ω");
            }
            if ((strcmp(fn,"number_string")==0 || strcmp(fn,"atom_number")==0)
                && pBB->α && pBB->α->γ) {
                IR_t *a0 = pBB->α, *a1 = a0->γ;
                int  num_first = (strcmp(fn,"number_string")==0) ? 1 : 0;
                int  k0 = (int)a0->t,    k1 = (int)a1->t;
                long i0 = (long)IR_LIT(a0).ival, i1 = (long)IR_LIT(a1).ival;
                const char *s0 = (k0 == IR_ATOM) ? IR_LIT(a0).sval : NULL;
                const char *s1 = (k1 == IR_ATOM) ? IR_LIT(a1).sval : NULL;
                std::string b;
                b += x86("sub", "rsp", 16L);
                b += x86("mov32", "edi", (long)num_first);
                b += x86("mov32", "esi", (long)k0);
                b += x86("movabs", "rdx", (unsigned long long)(uint64_t)i0);
                if (s0) b += x86("movabs", "rcx", (unsigned long long)(uintptr_t)s0);
                else    b += x86("xor", "ecx", "ecx");
                b += x86("mov32", "r8d", (long)k1);
                b += x86("movabs", "r9", (unsigned long long)(uint64_t)i1);
                if (s1) b += x86("movabs", "rax", (unsigned long long)(uintptr_t)s1);
                else    b += x86("xor", "eax", "eax");
                b += x86("mov", RSP(0), "rax");
                b += x86("call", "rt_number_string_pair", (unsigned long long)(uintptr_t)(void*)rt_number_string_pair);
                b += x86("add", "rsp", 16L);
                b += x86("test", "eax", "eax");
                return b + x86("je", "ω") + x86("jmp", "γ") + x86("jmp", "ω");
            }
            if ((strcmp(fn,"atom_concat")==0 || strcmp(fn,"string_concat")==0)
                && pBB->α && pBB->α->γ && pBB->α->γ->γ) {
                IR_t *a0 = pBB->α, *a1 = a0->γ, *a2 = a1->γ;
                int  k0 = (int)a0->t,    k1 = (int)a1->t,    k2 = (int)a2->t;
                long i0 = (long)IR_LIT(a0).ival, i1 = (long)IR_LIT(a1).ival, i2 = (long)IR_LIT(a2).ival;
                const char *s0 = (k0 == IR_ATOM) ? IR_LIT(a0).sval : NULL;
                const char *s1 = (k1 == IR_ATOM) ? IR_LIT(a1).sval : NULL;
                const char *s2 = (k2 == IR_ATOM) ? IR_LIT(a2).sval : NULL;
                std::string b;
                b += x86("sub", "rsp", 32L);
                b += x86("mov32", "edi", (long)k0);
                b += x86("movabs", "rsi", (unsigned long long)(uint64_t)i0);
                if (s0) b += x86("movabs", "rdx", (unsigned long long)(uintptr_t)s0);
                else    b += x86("xor", "edx", "edx");
                b += x86("mov32", "ecx", (long)k1);
                b += x86("movabs", "r8", (unsigned long long)(uint64_t)i1);
                if (s1) b += x86("movabs", "r9", (unsigned long long)(uintptr_t)s1);
                else    b += x86("xor", "r9d", "r9d");
                b += x86("stk32", 0L, (long)k2);
                b += x86("movabs", "rax", (unsigned long long)(uint64_t)i2);
                b += x86("mov", RSP(8), "rax");
                if (s2) b += x86("movabs", "rax", (unsigned long long)(uintptr_t)s2);
                else    b += x86("xor", "eax", "eax");
                b += x86("mov", RSP(16), "rax");
                b += x86("call", "rt_atom_concat", (unsigned long long)(uintptr_t)(void*)rt_atom_concat);
                b += x86("add", "rsp", 32L);
                b += x86("test", "eax", "eax");
                return b + x86("je", "ω") + x86("jmp", "γ") + x86("jmp", "ω");
            }
            if ((strcmp(fn,"atom_chars")==0 || strcmp(fn,"atom_codes")==0
              || strcmp(fn,"string_chars")==0 || strcmp(fn,"string_codes")==0)
                && pBB->α && pBB->α->γ) {
                IR_t *a0 = pBB->α, *a1 = a0->γ;
                int  k0 = (int)a0->t;
                long i0 = (long)IR_LIT(a0).ival;
                const char *s0 = (k0 == IR_ATOM) ? IR_LIT(a0).sval : NULL;
                int  as_codes = (strcmp(fn,"atom_codes")==0 || strcmp(fn,"string_codes")==0) ? 1 : 0;
                std::string b;
                if (a1->t == IR_STRUCT) {
                    b += x86("sub", "rsp", 8L);
                    b += x86_lit_bytes(emit_term_from_node_bin(a1));
                    b += x86("mov", "r8", "rax");
                    b += x86("mov32", "edi", (long)as_codes);
                    b += x86("mov32", "esi", (long)k0);
                    b += x86("movabs", "rdx", (unsigned long long)(uint64_t)i0);
                    if (s0) b += x86("movabs", "rcx", (unsigned long long)(uintptr_t)s0);
                    else    b += x86("xor", "ecx", "ecx");
                    b += x86("call", "rt_atom_chars_codes_term", (unsigned long long)(uintptr_t)(void*)rt_atom_chars_codes_term);
                    b += x86("add", "rsp", 8L);
                } else {
                    int  k1 = (int)a1->t;
                    long i1 = (long)IR_LIT(a1).ival;
                    const char *s1 = (k1 == IR_ATOM) ? IR_LIT(a1).sval : NULL;
                    b += x86("sub", "rsp", 16L);
                    b += x86("mov32", "edi", (long)as_codes);
                    b += x86("mov32", "esi", (long)k0);
                    b += x86("movabs", "rdx", (unsigned long long)(uint64_t)i0);
                    if (s0) b += x86("movabs", "rcx", (unsigned long long)(uintptr_t)s0);
                    else    b += x86("xor", "ecx", "ecx");
                    b += x86("mov32", "r8d", (long)k1);
                    b += x86("movabs", "r9", (unsigned long long)(uint64_t)i1);
                    if (s1) b += x86("movabs", "rax", (unsigned long long)(uintptr_t)s1);
                    else    b += x86("xor", "eax", "eax");
                    b += x86("mov", RSP(0), "rax");
                    b += x86("call", "rt_atom_chars_codes", (unsigned long long)(uintptr_t)(void*)rt_atom_chars_codes);
                    b += x86("add", "rsp", 16L);
                }
                b += x86("test", "eax", "eax");
                return b + x86("je", "ω") + x86("jmp", "γ") + x86("jmp", "ω");
            }
            if (strcmp(fn,"char_type")==0 && _.op_ival==2 && pBB->α && pBB->α->γ) {
                IR_t *a0 = pBB->α, *a1 = a0->γ;
                int  k0 = (int)a0->t;
                long i0 = (long)IR_LIT(a0).ival;
                const char *s0 = (k0 == IR_ATOM) ? IR_LIT(a0).sval : NULL;
                int  is_compound = ((a1->t == IR_STRUCT || a1->t == IR_ARITH) && IR_LIT(a1).sval) ? 1 : 0;
                const char *ty = IR_LIT(a1).sval;
                IR_t *inner = is_compound ? a1->α : NULL;
                int  ki = inner ? (int)inner->t : 0;
                long ii = inner ? (long)IR_LIT(inner).ival : 0;
                const char *si = (inner && inner->t == IR_ATOM) ? IR_LIT(inner).sval : NULL;
                std::string b;
                b += x86("sub", "rsp", 16L);
                b += x86("mov32", "edi", (long)k0);
                b += x86("movabs", "rsi", (unsigned long long)(uint64_t)i0);
                if (s0) b += x86("movabs", "rdx", (unsigned long long)(uintptr_t)s0);
                else    b += x86("xor", "edx", "edx");
                if (ty) b += x86("movabs", "rcx", (unsigned long long)(uintptr_t)ty);
                else    b += x86("xor", "ecx", "ecx");
                b += x86("mov32", "r8d", (long)is_compound);
                b += x86("mov32", "r9d", (long)ki);
                b += x86("movabs", "rax", (unsigned long long)(uint64_t)ii);
                b += x86("mov", RSP(0), "rax");
                if (si) b += x86("movabs", "rax", (unsigned long long)(uintptr_t)si);
                else    b += x86("xor", "eax", "eax");
                b += x86("mov", RSP(8), "rax");
                b += x86("call", "rt_char_type", (unsigned long long)(uintptr_t)(void*)rt_char_type);
                b += x86("add", "rsp", 16L);
                b += x86("test", "eax", "eax");
                return b + x86("je", "ω") + x86("jmp", "γ") + x86("jmp", "ω");
            }
    }
    if (MEDIUM_TEXT) {
        if ((strcmp(fn,"atom_length")==0   || strcmp(fn,"upcase_atom")==0   || strcmp(fn,"downcase_atom")==0
          || strcmp(fn,"string_length")==0 || strcmp(fn,"string_upper")==0  || strcmp(fn,"string_lower")==0)
            && pBB->α && pBB->α->γ) {
            IR_t *a0 = pBB->α, *a1 = a0->γ;
            int  k0 = (int)a0->t,    k1 = (int)a1->t;
            long i0 = (long)IR_LIT(a0).ival, i1 = (long)IR_LIT(a1).ival;
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
            if (k0 == IR_ATOM && IR_LIT(a0).sval) strtab_label(s0lbl, sizeof s0lbl, IR_LIT(a0).sval);
            if (k1 == IR_ATOM && IR_LIT(a1).sval) strtab_label(s1lbl, sizeof s1lbl, IR_LIT(a1).sval);
            const char *helper =
                (strcmp(fn,"atom_length")==0   || strcmp(fn,"string_length")==0) ? "rt_atom_length@PLT"   :
                (strcmp(fn,"upcase_atom")==0   || strcmp(fn,"string_upper")==0)  ? "rt_upcase_atom@PLT"   :
                                                                                   "rt_downcase_atom@PLT";
            return hdr
                 + x86("ins2", "mov edi,",  std::to_string(k0))
                 + x86("ins2", "mov rsi,",  std::to_string(i0))
                 + (s0lbl[0] ? x86("ins2", "lea rdx,", std::string("[rip + ") + s0lbl + "]") : x86("ins2", "xor", "edx, edx"))
                 + x86("ins2", "mov ecx,",  std::to_string(k1))
                 + x86("ins2", "mov r8,",   std::to_string(i1))
                 + (s1lbl[0] ? x86("ins2", "lea r9,", std::string("[rip + ") + s1lbl + "]") : x86("ins2", "xor", "r9d, r9d"))
                 + x86("ins2", "call", helper)
                 + x86("ins2", "test", "eax, eax")
                 + x86("ins2", "je",   _.lbl_ω)
                 + x86("ins2", "jmp",  _.lbl_γ)
                 + x86("Lins2", std::string(_.lbl_β) + ":", "jmp", _.lbl_ω);
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
                long i1 = (long)IR_LIT(a1).ival;
                char s1lbl[64]; s1lbl[0] = 0;
                if (k1 == IR_ATOM && IR_LIT(a1).sval) strtab_label(s1lbl, sizeof s1lbl, IR_LIT(a1).sval);
                b += emit_build_compound_term(a0)
                   + x86("ins2", "mov", "rdi, rax")
                   + x86("ins2", "mov esi,", std::to_string(k1))
                   + x86("ins2", "mov rdx,", std::to_string(i1))
                   + (s1lbl[0] ? x86("ins2", "lea rcx,", std::string("[rip + ") + s1lbl + "]") : x86("ins2", "xor", "ecx, ecx"))
                   + x86("ins2", "call", "rt_copy_term_term@PLT");
            }
            return b + x86("ins2", "add", "rsp, 16")
                     + x86("ins2", "test", "eax, eax")
                     + x86("ins2", "je",   _.lbl_ω)
                     + x86("ins2", "jmp",  _.lbl_γ)
                     + x86("Lins2", std::string(_.lbl_β) + ":", "jmp", _.lbl_ω);
        }
        if ((strcmp(fn,"atom_string")==0 || strcmp(fn,"string_to_atom")==0 || strcmp(fn,"copy_term")==0)
            && pBB->α && pBB->α->γ) {
            IR_t *a0 = pBB->α, *a1 = a0->γ;
            int  k0 = (int)a0->t,    k1 = (int)a1->t;
            long i0 = (long)IR_LIT(a0).ival, i1 = (long)IR_LIT(a1).ival;
            char s0lbl[64]; s0lbl[0] = 0;
            char s1lbl[64]; s1lbl[0] = 0;
            if (k0 == IR_ATOM && IR_LIT(a0).sval) strtab_label(s0lbl, sizeof s0lbl, IR_LIT(a0).sval);
            if (k1 == IR_ATOM && IR_LIT(a1).sval) strtab_label(s1lbl, sizeof s1lbl, IR_LIT(a1).sval);
            const char *helper2 =
                (strcmp(fn,"copy_term")==0) ? "rt_copy_term@PLT"
                                            : "rt_atom_string_pair@PLT";
            return hdr
                 + x86("ins2", "mov edi,",  std::to_string(k0))
                 + x86("ins2", "mov rsi,",  std::to_string(i0))
                 + (s0lbl[0] ? x86("ins2", "lea rdx,", std::string("[rip + ") + s0lbl + "]") : x86("ins2", "xor", "edx, edx"))
                 + x86("ins2", "mov ecx,",  std::to_string(k1))
                 + x86("ins2", "mov r8,",   std::to_string(i1))
                 + (s1lbl[0] ? x86("ins2", "lea r9,", std::string("[rip + ") + s1lbl + "]") : x86("ins2", "xor", "r9d, r9d"))
                 + x86("ins2", "call", helper2)
                 + x86("ins2", "test", "eax, eax")
                 + x86("ins2", "je",   _.lbl_ω)
                 + x86("ins2", "jmp",  _.lbl_γ)
                 + x86("Lins2", std::string(_.lbl_β) + ":", "jmp", _.lbl_ω);
        }
        if ((strcmp(fn,"number_string")==0 || strcmp(fn,"atom_number")==0)
            && pBB->α && pBB->α->γ) {
            IR_t *a0 = pBB->α, *a1 = a0->γ;
            int  num_first = (strcmp(fn,"number_string")==0) ? 1 : 0;
            int  k0 = (int)a0->t,    k1 = (int)a1->t;
            long i0 = (long)IR_LIT(a0).ival, i1 = (long)IR_LIT(a1).ival;
            char s0lbl[64]; s0lbl[0] = 0;
            char s1lbl[64]; s1lbl[0] = 0;
            if (k0 == IR_ATOM && IR_LIT(a0).sval) strtab_label(s0lbl, sizeof s0lbl, IR_LIT(a0).sval);
            if (k1 == IR_ATOM && IR_LIT(a1).sval) strtab_label(s1lbl, sizeof s1lbl, IR_LIT(a1).sval);
            return hdr
                 + x86("ins2", "sub", "rsp, 16")
                 + x86("ins2", "mov edi,",  std::to_string(num_first))
                 + x86("ins2", "mov esi,",  std::to_string(k0))
                 + x86("ins2", "mov rdx,",  std::to_string(i0))
                 + (s0lbl[0] ? x86("ins2", "lea rcx,", std::string("[rip + ") + s0lbl + "]") : x86("ins2", "xor", "ecx, ecx"))
                 + x86("ins2", "mov r8d,",  std::to_string(k1))
                 + x86("ins2", "mov r9,",   std::to_string(i1))
                 + (s1lbl[0] ? x86("ins2", "lea rax,", std::string("[rip + ") + s1lbl + "]") : x86("ins2", "xor", "eax, eax"))
                 + x86("ins2", "mov", "qword ptr [rsp + 0], rax")
                 + x86("ins2", "call", "rt_number_string_pair@PLT")
                 + x86("ins2", "add", "rsp, 16")
                 + x86("ins2", "test", "eax, eax")
                 + x86("ins2", "je",   _.lbl_ω)
                 + x86("ins2", "jmp",  _.lbl_γ)
                 + x86("Lins2", std::string(_.lbl_β) + ":", "jmp", _.lbl_ω);
        }
        if ((strcmp(fn,"atom_concat")==0 || strcmp(fn,"string_concat")==0)
            && pBB->α && pBB->α->γ && pBB->α->γ->γ) {
            IR_t *a0 = pBB->α, *a1 = a0->γ, *a2 = a1->γ;
            int  k0 = (int)a0->t,    k1 = (int)a1->t,    k2 = (int)a2->t;
            long i0 = (long)IR_LIT(a0).ival, i1 = (long)IR_LIT(a1).ival, i2 = (long)IR_LIT(a2).ival;
            char s0lbl[64]; s0lbl[0] = 0;
            char s1lbl[64]; s1lbl[0] = 0;
            char s2lbl[64]; s2lbl[0] = 0;
            if (k0 == IR_ATOM && IR_LIT(a0).sval) strtab_label(s0lbl, sizeof s0lbl, IR_LIT(a0).sval);
            if (k1 == IR_ATOM && IR_LIT(a1).sval) strtab_label(s1lbl, sizeof s1lbl, IR_LIT(a1).sval);
            if (k2 == IR_ATOM && IR_LIT(a2).sval) strtab_label(s2lbl, sizeof s2lbl, IR_LIT(a2).sval);
            return hdr
                 + x86("ins2", "sub", "rsp, 32")
                 + x86("ins2", "mov edi,",  std::to_string(k0))
                 + x86("ins2", "mov rsi,",  std::to_string(i0))
                 + (s0lbl[0] ? x86("ins2", "lea rdx,", std::string("[rip + ") + s0lbl + "]") : x86("ins2", "xor", "edx, edx"))
                 + x86("ins2", "mov ecx,",  std::to_string(k1))
                 + x86("ins2", "mov r8,",   std::to_string(i1))
                 + (s1lbl[0] ? x86("ins2", "lea r9,", std::string("[rip + ") + s1lbl + "]") : x86("ins2", "xor", "r9d, r9d"))
                 + x86("ins2", "mov dword ptr [rsp + 0],", std::to_string(k2))
                 + x86("ins2", "mov rax,",  std::to_string(i2))
                 + x86("ins2", "mov", "qword ptr [rsp + 8], rax")
                 + (s2lbl[0] ? x86("ins2", "lea rax,", std::string("[rip + ") + s2lbl + "]") : x86("ins2", "xor", "eax, eax"))
                 + x86("ins2", "mov", "qword ptr [rsp + 16], rax")
                 + x86("ins2", "call", "rt_atom_concat@PLT")
                 + x86("ins2", "add", "rsp, 32")
                 + x86("ins2", "test", "eax, eax")
                 + x86("ins2", "je",   _.lbl_ω)
                 + x86("ins2", "jmp",  _.lbl_γ)
                 + x86("Lins2", std::string(_.lbl_β) + ":", "jmp", _.lbl_ω);
        }
        if ((strcmp(fn,"atom_chars")==0 || strcmp(fn,"atom_codes")==0
          || strcmp(fn,"string_chars")==0 || strcmp(fn,"string_codes")==0)
            && pBB->α && pBB->α->γ) {
            IR_t *a0 = pBB->α, *a1 = a0->γ;
            int  k0 = (int)a0->t;
            long i0 = (long)IR_LIT(a0).ival;
            int  as_codes = (strcmp(fn,"atom_codes")==0 || strcmp(fn,"string_codes")==0) ? 1 : 0;
            char s0lbl[64]; s0lbl[0] = 0;
            if (k0 == IR_ATOM && IR_LIT(a0).sval) strtab_label(s0lbl, sizeof s0lbl, IR_LIT(a0).sval);
            if (a1->t == IR_STRUCT) {
                /* Path B: literal list in a1 — build Term* from cons-cell tree, pass as pointer. */
                return hdr
                     + x86("ins2", "sub", "rsp, 16")
                     + emit_build_compound_term(a1)
                     + x86("ins2", "mov", "r8, rax")
                     + x86("ins2", "mov edi,",  std::to_string(as_codes))
                     + x86("ins2", "mov esi,",  std::to_string(k0))
                     + x86("ins2", "mov rdx,",  std::to_string(i0))
                     + (s0lbl[0] ? x86("ins2", "lea rcx,", std::string("[rip + ") + s0lbl + "]") : x86("ins2", "xor", "ecx, ecx"))
                     + x86("ins2", "call", "rt_atom_chars_codes_term@PLT")
                     + x86("ins2", "add", "rsp, 16")
                     + x86("ins2", "test", "eax, eax")
                     + x86("ins2", "je",   _.lbl_ω)
                     + x86("ins2", "jmp",  _.lbl_γ)
                     + x86("Lins2", std::string(_.lbl_β) + ":", "jmp", _.lbl_ω);
            }
            /* Path A: scalar a1 (VAR or ATOM). */
            int  k1 = (int)a1->t;
            long i1 = (long)IR_LIT(a1).ival;
            char s1lbl[64]; s1lbl[0] = 0;
            if (k1 == IR_ATOM && IR_LIT(a1).sval) strtab_label(s1lbl, sizeof s1lbl, IR_LIT(a1).sval);
            return hdr
                 + x86("ins2", "sub", "rsp, 16")
                 + x86("ins2", "mov edi,",  std::to_string(as_codes))
                 + x86("ins2", "mov esi,",  std::to_string(k0))
                 + x86("ins2", "mov rdx,",  std::to_string(i0))
                 + (s0lbl[0] ? x86("ins2", "lea rcx,", std::string("[rip + ") + s0lbl + "]") : x86("ins2", "xor", "ecx, ecx"))
                 + x86("ins2", "mov r8d,",  std::to_string(k1))
                 + x86("ins2", "mov r9,",   std::to_string(i1))
                 + (s1lbl[0] ? x86("ins2", "lea rax,", std::string("[rip + ") + s1lbl + "]") : x86("ins2", "xor", "eax, eax"))
                 + x86("ins2", "mov", "qword ptr [rsp + 0], rax")
                 + x86("ins2", "call", "rt_atom_chars_codes@PLT")
                 + x86("ins2", "add", "rsp, 16")
                 + x86("ins2", "test", "eax, eax")
                 + x86("ins2", "je",   _.lbl_ω)
                 + x86("ins2", "jmp",  _.lbl_γ)
                 + x86("Lins2", std::string(_.lbl_β) + ":", "jmp", _.lbl_ω);
        }
        if (strcmp(fn,"char_type")==0 && _.op_ival==2 && pBB->α && pBB->α->γ) {
            IR_t *a0 = pBB->α, *a1 = a0->γ;
            int  k0 = (int)a0->t;
            long i0 = (long)IR_LIT(a0).ival;
            char s0lbl[64]; s0lbl[0] = 0;
            if (k0 == IR_ATOM && IR_LIT(a0).sval) strtab_label(s0lbl, sizeof s0lbl, IR_LIT(a0).sval);
            int  is_compound = ((a1->t == IR_STRUCT || a1->t == IR_ARITH) && IR_LIT(a1).sval) ? 1 : 0;
            char tylbl[64]; tylbl[0] = 0;
            if (IR_LIT(a1).sval) strtab_label(tylbl, sizeof tylbl, IR_LIT(a1).sval);
            IR_t *inner = is_compound ? a1->α : NULL;
            int  ki = inner ? (int)inner->t : 0;
            long ii = inner ? (long)IR_LIT(inner).ival : 0;
            char silbl[64]; silbl[0] = 0;
            if (inner && inner->t == IR_ATOM && IR_LIT(inner).sval) strtab_label(silbl, sizeof silbl, IR_LIT(inner).sval);
            return hdr
                 + x86("ins2", "sub", "rsp, 16")
                 + x86("ins2", "mov edi,",  std::to_string(k0))
                 + x86("ins2", "mov rsi,",  std::to_string(i0))
                 + (s0lbl[0] ? x86("ins2", "lea rdx,", std::string("[rip + ") + s0lbl + "]") : x86("ins2", "xor", "edx, edx"))
                 + (tylbl[0] ? x86("ins2", "lea rcx,", std::string("[rip + ") + tylbl + "]") : x86("ins2", "xor", "ecx, ecx"))
                 + x86("ins2", "mov r8d,",  std::to_string(is_compound))
                 + x86("ins2", "mov r9d,",  std::to_string(ki))
                 + x86("ins2", "mov rax,",  std::to_string(ii))
                 + x86("ins2", "mov", "qword ptr [rsp + 0], rax")
                 + (silbl[0] ? x86("ins2", "lea rax,", std::string("[rip + ") + silbl + "]") : x86("ins2", "xor", "eax, eax"))
                 + x86("ins2", "mov", "qword ptr [rsp + 8], rax")
                 + x86("ins2", "call", "rt_char_type@PLT")
                 + x86("ins2", "add", "rsp, 16")
                 + x86("ins2", "test", "eax, eax")
                 + x86("ins2", "je",   _.lbl_ω)
                 + x86("ins2", "jmp",  _.lbl_γ)
                 + x86("Lins2", std::string(_.lbl_β) + ":", "jmp", _.lbl_ω);
        }
    }
    return std::string();
}
