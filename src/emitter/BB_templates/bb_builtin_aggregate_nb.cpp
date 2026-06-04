#include "bb_builtin_common.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string agg_build_term(IR_t *a) {
    if (!a) return s_2asm("xor", "eax, eax");
    if (a->t == IR_STRUCT) return emit_build_compound_term(a);
    int kind = (int)a->t;
    long ival = (long)a->ival;
    const char *sval = a->sval;
    char slbl[64]; slbl[0] = 0;
    if (sval && *sval) strtab_label(slbl, sizeof slbl, sval);
    return s_2asm("mov", emit_fmt("edi, %d", kind))
         + s_2asm("mov", emit_fmt("rsi, %ld", ival))
         + (slbl[0] ? s_2asm("lea", emit_fmt("rdx, [rip + %s]", slbl))
                    : s_2asm("xor", "edx, edx"))
         + s_2asm("xorps", "xmm0, xmm0")
         + s_2asm("call", "rt_node_to_term@PLT");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_builtin_aggregate_nb_str(IR_t *pBB, const char *fn, const std::string &hdr) {
    (void)pBB; (void)fn; (void)hdr;
    if (MEDIUM_BINARY) {
            if (strcmp(fn, "aggregate_all") == 0 && pBB->α && pBB->α->γ
                && pBB->α->γ->γ && _.op_ival == 3) {
                IR_t *a0 = pBB->α, *a1 = a0->γ, *a2 = a1->γ;
                int  kres = (int)a2->t;
                long ires = (long)a2->ival;
                const char *sres = (kres == IR_ATOM) ? a2->sval : NULL;
                std::string b;
                b += bytes(4, "\x48\x83\xEC\x10");                /* sub rsp, 16 */
                b += emit_term_from_node_bin(a0);            /* build template → rax */
                b += bytes(4, "\x48\x89\x04\x24");                /* mov [rsp+0], rax (hold tmpl) */
                b += emit_term_from_node_bin(a1);            /* build goal → rax */
                b += bytes(3, "\x48\x89\xC6");                    /* mov rsi, rax (goal) */
                b += bytes(4, "\x48\x8B\x3C\x24");                /* mov rdi, [rsp+0] (tmpl) */
                b += bytes(1, "\xBA") + u32le((uint32_t)kres);    /* mov edx, kres */
                b += bytes(2, "\x48\xB9") + u64le((uint64_t)ires);/* mov rcx, ires */
                if (sres) b += bytes(2, "\x49\xB8") + u64le((uint64_t)(uintptr_t)sres);  /* mov r8, sres */
                else      b += bytes(3, "\x45\x31\xC0");          /* xor r8d, r8d */
                b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_aggregate_all_term) + bytes(2, "\xFF\xD0");
                b += bytes(4, "\x48\x83\xC4\x10");                /* add rsp, 16 */
                b += bytes(2, "\x85\xC0");                        /* test eax, eax */
                return x86_lit_bytes(b) + x86("je", PORT_OMEGA) + x86("jmp", PORT_GAMMA) + x86("jmp", PORT_OMEGA);
            }
            if ((strcmp(fn, "nb_setval") == 0 || strcmp(fn, "nb_getval") == 0)
                && pBB->α && pBB->α->γ && _.op_ival == 2) {
                int   is_set = (strcmp(fn, "nb_setval") == 0);
                IR_t *a0 = pBB->α, *a1 = a0->γ;
                std::string b;
                b += bytes(4, "\x48\x83\xEC\x10");                /* sub rsp, 16 */
                if (is_set) {
                    b += emit_term_from_node_bin(a0);        /* build key → rax */
                    b += bytes(4, "\x48\x89\x04\x24");            /* mov [rsp+0], rax (hold key) */
                    b += emit_term_from_node_bin(a1);        /* build value → rax */
                    b += bytes(3, "\x48\x89\xC6");                /* mov rsi, rax (val) */
                    b += bytes(4, "\x48\x8B\x3C\x24");            /* mov rdi, [rsp+0] (key) */
                    b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_nb_setval_term) + bytes(2, "\xFF\xD0");
                } else {
                    int  kres = (int)a1->t;
                    long ires = (long)a1->ival;
                    const char *sres = (kres == IR_ATOM) ? a1->sval : NULL;
                    b += emit_term_from_node_bin(a0);        /* build key → rax */
                    b += bytes(3, "\x48\x89\xC7");                /* mov rdi, rax (key) */
                    b += bytes(1, "\xBE") + u32le((uint32_t)kres);/* mov esi, kres */
                    b += bytes(2, "\x48\xBA") + u64le((uint64_t)ires);  /* mov rdx, ires */
                    if (sres) b += bytes(2, "\x48\xB9") + u64le((uint64_t)(uintptr_t)sres);  /* mov rcx, sres */
                    else      b += bytes(2, "\x31\xC9");          /* xor ecx, ecx */
                    b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_nb_getval_term) + bytes(2, "\xFF\xD0");
                }
                b += bytes(4, "\x48\x83\xC4\x10");                /* add rsp, 16 */
                b += bytes(2, "\x85\xC0");                        /* test eax, eax */
                return x86_lit_bytes(b) + x86("je", PORT_OMEGA) + x86("jmp", PORT_GAMMA) + x86("jmp", PORT_OMEGA);
            }
    }
    if (MEDIUM_TEXT) {
        if (strcmp(fn, "aggregate_all") == 0 && pBB->α && pBB->α->γ && pBB->α->γ->γ && _.op_ival == 3) {
            IR_t *a0 = pBB->α, *a1 = a0->γ, *a2 = a1->γ;
            return hdr
                 + agg_build_term(a0)                       + s_2asm("push", "rax")
                 + agg_build_term(a1)                       + s_2asm("push", "rax")
                 + agg_build_term(a2)                       + s_2asm("push", "rax")
                 + s_2asm("sub", "rsp, 8")
                 + s_2asm("mov", "rdx, [rsp + 8]")
                 + s_2asm("mov", "rsi, [rsp + 16]")
                 + s_2asm("mov", "rdi, [rsp + 24]")
                 + s_2asm("call", "rt_aggregate_all_meta@PLT")
                 + s_2asm("add", "rsp, 32")
                 + s_2asm("test", "eax, eax")
                 + s_2asm("je",   _.lbl_ω)
                 + s_2asm("jmp",  _.lbl_γ)
                 + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω);
        }
        if ((strcmp(fn, "nb_setval") == 0 || strcmp(fn, "nb_getval") == 0)
            && pBB->α && pBB->α->γ && _.op_ival == 2) {
            int   is_set = (strcmp(fn, "nb_setval") == 0);
            IR_t *a0 = pBB->α, *a1 = a0->γ;
            if (is_set) {
                return hdr
                     + agg_build_term(a0)                   + s_2asm("push", "rax")
                     + s_2asm("sub", "rsp, 8")
                     + agg_build_term(a1)
                     + s_2asm("mov", "rsi, rax")
                     + s_2asm("mov", "rdi, [rsp + 8]")
                     + s_2asm("call", "rt_nb_setval_term@PLT")
                     + s_2asm("add", "rsp, 16")
                     + s_2asm("test", "eax, eax")
                     + s_2asm("je",   _.lbl_ω)
                     + s_2asm("jmp",  _.lbl_γ)
                     + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω);
            }
            int  kres = (int)a1->t;
            long ires = (long)a1->ival;
            const char *sres = (kres == IR_ATOM) ? a1->sval : NULL;
            char slbl[64]; slbl[0] = 0;
            if (sres && *sres) strtab_label(slbl, sizeof slbl, sres);
            return hdr
                 + agg_build_term(a0)
                 + s_2asm("mov", "rdi, rax")
                 + s_2asm("mov", emit_fmt("esi, %d", kres))
                 + s_2asm("mov", emit_fmt("rdx, %ld", ires))
                 + (slbl[0] ? s_2asm("lea", emit_fmt("rcx, [rip + %s]", slbl))
                            : s_2asm("xor", "ecx, ecx"))
                 + s_2asm("call", "rt_nb_getval_term@PLT")
                 + s_2asm("test", "eax, eax")
                 + s_2asm("je",   _.lbl_ω)
                 + s_2asm("jmp",  _.lbl_γ)
                 + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω);
        }
    }
    return std::string();
}
