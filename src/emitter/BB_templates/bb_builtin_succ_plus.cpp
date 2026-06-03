#include "bb_builtin_common.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_builtin_succ_plus_str(IR_t *pBB, const char *fn, const std::string &hdr) {
    (void)pBB; (void)fn; (void)hdr;
    if (MEDIUM_BINARY) {
            if (strcmp(fn, "succ") == 0 && pBB->ival == 2 && pBB->α && pBB->β) {
                IR_t *a0 = pBB->α, *a1 = pBB->β;
                int  k0 = (int)a0->t,    k1 = (int)a1->t;
                long i0 = (long)a0->ival, i1 = (long)a1->ival;
                const char *s0 = (k0 == IR_ATOM) ? a0->sval : NULL;
                const char *s1 = (k1 == IR_ATOM) ? a1->sval : NULL;
                std::string b;
                b += bytes(1, "\xBF") + u32le((uint32_t)k0);
                b += bytes(2, "\x48\xBE") + u64le((uint64_t)i0);
                if (s0) b += bytes(2, "\x48\xBA") + u64le((uint64_t)(uintptr_t)s0);
                else    b += bytes(2, "\x31\xD2");
                b += bytes(1, "\xB9") + u32le((uint32_t)k1);
                b += bytes(2, "\x49\xB8") + u64le((uint64_t)i1);
                if (s1) b += bytes(2, "\x49\xB9") + u64le((uint64_t)(uintptr_t)s1);
                else    b += bytes(3, "\x45\x31\xC9");
                b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_succ) + bytes(2, "\xFF\xD0");
                b += bytes(2, "\x85\xC0");
                return x86_lit_bytes(b) + x86("je", PORT_OMEGA) + x86("jmp", PORT_GAMMA) + x86("jmp", PORT_OMEGA);
            }
            if (strcmp(fn, "plus") == 0 && pBB->ival == 3 && pBB->α && pBB->α->γ && pBB->α->γ->γ) {
                IR_t *a0 = pBB->α, *a1 = a0->γ, *a2 = a1->γ;
                int  k0 = (int)a0->t,    k1 = (int)a1->t,    k2 = (int)a2->t;
                long i0 = (long)a0->ival, i1 = (long)a1->ival, i2 = (long)a2->ival;
                const char *s0 = (k0 == IR_ATOM) ? a0->sval : NULL;
                const char *s1 = (k1 == IR_ATOM) ? a1->sval : NULL;
                const char *s2 = (k2 == IR_ATOM) ? a2->sval : NULL;
                std::string b;
                b += bytes(4, "\x48\x83\xEC\x20");
                b += bytes(1, "\xBF") + u32le((uint32_t)k0);
                b += bytes(2, "\x48\xBE") + u64le((uint64_t)i0);
                if (s0) b += bytes(2, "\x48\xBA") + u64le((uint64_t)(uintptr_t)s0);
                else    b += bytes(2, "\x31\xD2");
                b += bytes(1, "\xB9") + u32le((uint32_t)k1);
                b += bytes(2, "\x49\xB8") + u64le((uint64_t)i1);
                if (s1) b += bytes(2, "\x49\xB9") + u64le((uint64_t)(uintptr_t)s1);
                else    b += bytes(3, "\x45\x31\xC9");
                b += bytes(3, "\xC7\x04\x24") + u32le((uint32_t)k2);
                b += bytes(2, "\x48\xB8") + u64le((uint64_t)i2);
                b += bytes(5, "\x48\x89\x44\x24\x08");
                if (s2) b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)s2);
                else    b += bytes(2, "\x31\xC0");
                b += bytes(5, "\x48\x89\x44\x24\x10");
                b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_plus) + bytes(2, "\xFF\xD0");
                b += bytes(4, "\x48\x83\xC4\x20");
                b += bytes(2, "\x85\xC0");
                return x86_lit_bytes(b) + x86("je", PORT_OMEGA) + x86("jmp", PORT_GAMMA) + x86("jmp", PORT_OMEGA);
            }
    }
    if (MEDIUM_TEXT) {
    std::string succ_back = s_2asm("jmp", _.lbl_γ)
                          + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_γ);
    (void)succ_back;
        if (strcmp(fn, "succ") == 0 && pBB->ival == 2 && pBB->α && pBB->β) {
            IR_t *a0 = pBB->α, *a1 = pBB->β;
            int  k0 = (int)a0->t,    k1 = (int)a1->t;
            long i0 = (long)a0->ival, i1 = (long)a1->ival;
            char s0lbl[64]; s0lbl[0] = 0;
            char s1lbl[64]; s1lbl[0] = 0;
            if (k0 == IR_ATOM && a0->sval) strtab_label(s0lbl, sizeof s0lbl, a0->sval);
            if (k1 == IR_ATOM && a1->sval) strtab_label(s1lbl, sizeof s1lbl, a1->sval);
            return hdr
                 + s_2asm("mov edi,",  emit_fmt("%d",  k0))
                 + s_2asm("mov rsi,",  emit_fmt("%ld", i0))
                 + (s0lbl[0] ? s_2asm("lea rdx,", emit_fmt("[rip + %s]", s0lbl)) : s_2asm("xor", "edx, edx"))
                 + s_2asm("mov ecx,",  emit_fmt("%d",  k1))
                 + s_2asm("mov r8,",   emit_fmt("%ld", i1))
                 + (s1lbl[0] ? s_2asm("lea r9,", emit_fmt("[rip + %s]", s1lbl)) : s_2asm("xor", "r9d, r9d"))
                 + s_2asm("call", "rt_succ@PLT")
                 + s_2asm("test", "eax, eax")
                 + s_2asm("je",   _.lbl_ω)
                 + s_2asm("jmp",  _.lbl_γ)
                 + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω);
        }
        if (strcmp(fn, "plus") == 0 && pBB->ival == 3 && pBB->α && pBB->α->γ && pBB->α->γ->γ) {
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
                 + s_2asm("sub", "rsp, 32")
                 + s_2asm("mov edi,",  emit_fmt("%d",  k0))
                 + s_2asm("mov rsi,",  emit_fmt("%ld", i0))
                 + (s0lbl[0] ? s_2asm("lea rdx,", emit_fmt("[rip + %s]", s0lbl)) : s_2asm("xor", "edx, edx"))
                 + s_2asm("mov ecx,",  emit_fmt("%d",  k1))
                 + s_2asm("mov r8,",   emit_fmt("%ld", i1))
                 + (s1lbl[0] ? s_2asm("lea r9,", emit_fmt("[rip + %s]", s1lbl)) : s_2asm("xor", "r9d, r9d"))
                 + s_2asm("mov dword ptr [rsp + 0],", emit_fmt("%d",  k2))
                 + s_2asm("mov rax,",  emit_fmt("%ld", i2))
                 + s_2asm("mov", "[rsp + 8], rax")
                 + (s2lbl[0]
                       ? (s_2asm("lea rax,", emit_fmt("[rip + %s]", s2lbl)) + s_2asm("mov", "[rsp + 16], rax"))
                       : s_2asm("mov", "qword ptr [rsp + 16], 0"))
                 + s_2asm("call", "rt_plus@PLT")
                 + s_2asm("add", "rsp, 32")
                 + s_2asm("test", "eax, eax")
                 + s_2asm("je",   _.lbl_ω)
                 + s_2asm("jmp",  _.lbl_γ)
                 + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω);
        }
    }
    return std::string();
}
