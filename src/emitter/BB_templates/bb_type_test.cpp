#include "bb_common.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_type_test_str(IR_t *pBB, const char *fn, const std::string &hdr) {
    (void)pBB; (void)fn; (void)hdr;
    if (MEDIUM_BINARY) {
            if (_.op_ival == 1 && ir_call_arg(pBB,0) &&
                (strcmp(fn,"var")==0      || strcmp(fn,"nonvar")==0   || strcmp(fn,"atom")==0
              || strcmp(fn,"atomic")==0   || strcmp(fn,"number")==0   || strcmp(fn,"integer")==0
              || strcmp(fn,"float")==0    || strcmp(fn,"compound")==0 || strcmp(fn,"callable")==0
              || strcmp(fn,"is_list")==0  || strcmp(fn,"ground")==0)) {
                IR_t *a0 = ir_call_arg(pBB,0);
                if (a0->op == IR_STRUCT || a0->op == IR_ARITH) {
                    /* PLR-K-5 (Opus 4.8, 2026-05-29): compound-literal arg, e.g. is_list([1,2,3]) /   */
                    /* compound(f(a)) / ground(g(X,Y)). Build the Term* via emit_build_compound_term_bin */
                    /* (→ rax), pass as t0=rsi, fn ptr=rdi, call rt_type_test_term. sub rsp,8 keeps   */
                    /* 16-alignment across the build's internal calls. Std test/je-ω/jmp-γ/β→ω tail.      */
                    std::string b;
                    b += bytes(4, "\x48\x83\xEC\x08");          /* sub rsp, 8 */
                    b += emit_term_from_node_bin(a0);
                    b += bytes(3, "\x48\x89\xC6");              /* mov rsi, rax */
                    b += bytes(2, "\x48\xBF") + u64le((uint64_t)(uintptr_t)fn);   /* mov rdi, fn */
                    b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_type_test_term) + bytes(2, "\xFF\xD0");
                    b += bytes(4, "\x48\x83\xC4\x08");          /* add rsp, 8 */
                    b += bytes(2, "\x85\xC0");                  /* test eax, eax */
                    return x86_lit_bytes(b) + x86("je", PORT_OMEGA) + x86("jmp", PORT_GAMMA) + x86("jmp", PORT_OMEGA);
                }
                int  k0 = (int)a0->op;
                long i0 = (long)IR_LIT(a0).ival;
                const char *s0 = (k0 == IR_ATOM) ? IR_LIT(a0).sval : NULL;
                std::string b;
                /* mov rdi, fn (ptr)     48 BF [8]                                                    */
                b += bytes(2, "\x48\xBF") + u64le((uint64_t)(uintptr_t)fn);
                /* mov esi, k0           BE [4]                                                        */
                b += bytes(1, "\xBE") + u32le((uint32_t)k0);
                /* mov rdx, i0           48 BA [8]                                                     */
                b += bytes(2, "\x48\xBA") + u64le((uint64_t)i0);
                /* mov rcx, s0 (or xor ecx,ecx)   48 B9 [8] / 31 C9                                   */
                if (s0) b += bytes(2, "\x48\xB9") + u64le((uint64_t)(uintptr_t)s0);
                else    b += bytes(2, "\x31\xC9");
                /* movabs rax, &rt_type_test; call rax   48 B8 [8] FF D0                            */
                b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_type_test) + bytes(2, "\xFF\xD0");
                /* test eax, eax    85 C0                                                              */
                b += bytes(2, "\x85\xC0");
                return x86_lit_bytes(b) + x86("je", PORT_OMEGA) + x86("jmp", PORT_GAMMA) + x86("jmp", PORT_OMEGA);
            }
    }
    if (MEDIUM_TEXT) {
    std::string succ_back = x86("ins2", "jmp", _.lbl_γ)
                          + x86("Lins2", emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_γ);
    (void)succ_back;
        if (_.op_ival == 1 && ir_call_arg(pBB,0) &&
            (strcmp(fn,"var")==0      || strcmp(fn,"nonvar")==0   || strcmp(fn,"atom")==0
          || strcmp(fn,"atomic")==0   || strcmp(fn,"number")==0   || strcmp(fn,"integer")==0
          || strcmp(fn,"float")==0    || strcmp(fn,"compound")==0 || strcmp(fn,"callable")==0
          || strcmp(fn,"is_list")==0  || strcmp(fn,"ground")==0)) {
            char op_lbl[64]; strtab_label(op_lbl, sizeof op_lbl, fn);
            IR_t *a0 = ir_call_arg(pBB,0);
            if (a0->op == IR_STRUCT) {
                return hdr
                     + x86("ins2", "sub", "rsp, 16")
                     + emit_build_compound_term(a0)
                     + x86("ins2", "mov", "rsi, rax")
                     + x86("ins2", "lea rdi,", emit_fmt("[rip + %s]", op_lbl))
                     + x86("ins2", "call", "rt_type_test_term@PLT")
                     + x86("ins2", "add", "rsp, 16")
                     + x86("ins2", "test", "eax, eax")
                     + x86("ins2", "je",   _.lbl_ω)
                     + x86("ins2", "jmp",  _.lbl_γ)
                     + x86("Lins2", emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω);
            }
            int  k0 = (int)a0->op;
            long i0 = (long)IR_LIT(a0).ival;
            char s0lbl[64]; s0lbl[0] = 0;
            if (k0 == IR_ATOM && IR_LIT(a0).sval) strtab_label(s0lbl, sizeof s0lbl, IR_LIT(a0).sval);
            return hdr
                 + x86("ins2", "lea rdi,", emit_fmt("[rip + %s]", op_lbl))
                 + x86("ins2", "mov esi,",  emit_fmt("%d",  k0))
                 + x86("ins2", "mov rdx,",  emit_fmt("%ld", i0))
                 + (s0lbl[0] ? x86("ins2", "lea rcx,", emit_fmt("[rip + %s]", s0lbl)) : x86("ins2", "xor", "ecx, ecx"))
                 + x86("ins2", "call", "rt_type_test@PLT")
                 + x86("ins2", "test", "eax, eax")
                 + x86("ins2", "je",   _.lbl_ω)
                 + x86("ins2", "jmp",  _.lbl_γ)
                 + x86("Lins2", emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω);
        }
    }
    return std::string();
}
