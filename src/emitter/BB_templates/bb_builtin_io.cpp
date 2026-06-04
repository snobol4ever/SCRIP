#include "bb_builtin_common.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_builtin_io_str(IR_t *pBB, const char *fn, const std::string &hdr) {
    (void)pBB; (void)fn; (void)hdr;
    if (MEDIUM_BINARY) {
            if (strcmp(fn, "nl") == 0) {
                std::string b = bytes(1, "\xBF") + u32le(10)
                              + bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)putchar) + bytes(2, "\xFF\xD0");
                return x86_lit_bytes(b) + x86("jmp", PORT_GAMMA) + x86("def", PORT_BETA) + x86("jmp", PORT_GAMMA);
            }
            if (strcmp(fn, "halt") == 0) {
                std::string b = bytes(2, "\x31\xFF")
                              + bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)exit) + bytes(2, "\xFF\xD0");
                return x86_lit_bytes(b) + x86("def", PORT_BETA) + x86("jmp", PORT_GAMMA);
            }
            if (strcmp(fn, "write") == 0 || strcmp(fn, "writeln") == 0 || strcmp(fn, "print") == 0) {
                std::string b;
                if (_.op_ival >= 1 && pBB->α) {
                    IR_t *arg = pBB->α;
                    if (arg->t == IR_ATOM) {
                        const char *atom = arg->sval ? arg->sval : "";
                        /* mov rdi, imm64(atom)   48 BF [8] ; movabs rax,&rt_write_atom ; call rax */
                        b += bytes(2, "\x48\xBF") + u64le((uint64_t)(uintptr_t)atom)
                           + bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_write_atom)
                           + bytes(2, "\xFF\xD0");
                    } else if (arg->t == IR_LOGICVAR) {
                        /* mov edi, slot          BF [4] ; movabs rax,&rt_write_var ; call rax */
                        b += bytes(1, "\xBF") + u32le((uint32_t)(int)arg->ival)
                           + bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_write_var)
                           + bytes(2, "\xFF\xD0");
                    } else {
                        /* PLR-K-3 (2026-05-29): compound / int / float arg. Build the Term* via       */
                        /* emit_build_compound_term_bin (→ rax) then rt_write_term_ptr(rdi=rax).     */
                        /* sub rsp,8 keeps 16-alignment across the build's and helper's internal calls. */
                        b += bytes(4, "\x48\x83\xEC\x08");
                        b += emit_term_from_node_bin(arg);
                        b += bytes(3, "\x48\x89\xC7");   /* mov rdi, rax */
                        b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_write_term_ptr)
                           + bytes(2, "\xFF\xD0");
                        b += bytes(4, "\x48\x83\xC4\x08");   /* add rsp, 8 */
                    }
                }
                if (strcmp(fn, "writeln") == 0) {
                    b += bytes(1, "\xBF") + u32le(10)
                       + bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)putchar) + bytes(2, "\xFF\xD0");
                }
                return x86_lit_bytes(b) + x86("jmp", PORT_GAMMA) + x86("def", PORT_BETA) + x86("jmp", PORT_GAMMA);
            }
            if ((strcmp(fn, "writeq") == 0 || strcmp(fn, "write_canonical") == 0) && pBB->α) {
                IR_t *arg = pBB->α;
                void *writer = (strcmp(fn, "writeq") == 0)
                    ? (void *)rt_writeq_term_ptr
                    : (void *)rt_write_canonical_term_ptr;
                std::string b;
                b += bytes(4, "\x48\x83\xEC\x08");          /* sub rsp, 8 */
                b += emit_term_from_node_bin(arg);
                b += bytes(3, "\x48\x89\xC7");              /* mov rdi, rax */
                b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)writer) + bytes(2, "\xFF\xD0");
                b += bytes(4, "\x48\x83\xC4\x08");          /* add rsp, 8 */
                return x86_lit_bytes(b) + x86("jmp", PORT_GAMMA) + x86("def", PORT_BETA) + x86("jmp", PORT_GAMMA);
            }
    }
    if (MEDIUM_TEXT) {
    std::string succ_back = s_2asm("jmp", _.lbl_γ)
                          + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_γ);
    (void)succ_back;
        if (strcmp(fn, "nl") == 0) {
            return hdr + s_2asm("mov", "edi, 10") + s_2asm("call", "putchar@PLT") + succ_back;
        }
        if (strcmp(fn, "halt") == 0) {
            return hdr + s_2asm("xor", "edi, edi") + s_2asm("call", "exit@PLT")
                 + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_γ);
        }
        if (strcmp(fn, "write") == 0 || strcmp(fn, "writeln") == 0 || strcmp(fn, "print") == 0) {
            std::string write_body;
            if (_.op_ival >= 1 && pBB->α) {
                IR_t *arg = pBB->α;
                if (arg->t == IR_ATOM) {
                    write_body = _.bb_ls
                        ? s_2asm("lea rcx,", emit_fmt("[rip + %s]", _.bb_ls)) + s_2asm("mov", "rdi, rcx")
                        : s_2asm("xor", "edi, edi");
                    write_body += s_2asm("call", "rt_write_atom@PLT");
                } else if (arg->t == IR_LOGICVAR) {
                    write_body = s_2asm("mov edi,", emit_fmt("%d", (int)arg->ival))
                               + s_2asm("call", "rt_write_var@PLT");
                } else if (arg->t == IR_LIT_I) {
                    /* PLG-9j: bare int is a flat-tier arg (PLG-9b gate) — 16-aligned box entry, so a direct */
                    /* rt_write_int with no rsp adjust is correct (a sub rsp,8 here would mis-align the    */
                    /* call and fault). Restores the pre-PLG-9j leaf path; only compounds need pl_write.      */
                    write_body = s_2asm("mov rdi,", emit_fmt("%ld", (long)arg->ival))
                               + s_2asm("call", "rt_write_int@PLT");
                } else if (arg->t == IR_LIT_F) {
                    /* PLG-9j: bare float — likewise a flat-tier leaf (16-aligned entry). Load the literal's  */
                    /* bits into xmm0 (no .rodata constant) and call rt_write_float directly, no rsp       */
                    /* adjust. emit_build_compound_term hard-codes xmm0=0 for leaves, so the build+pl_write   */
                    /* path below would render 0.0 (and a sub rsp,8 would mis-align the leaf's call → fault).  */
                    uint64_t fb = 0; double dv = arg->dval; memcpy(&fb, &dv, sizeof fb);
                    write_body = s_2asm("mov rax,", emit_fmt("%llu", (unsigned long long)fb))
                               + s_2asm("movq", "xmm0, rax")
                               + s_2asm("call", "rt_write_float@PLT");
                } else {
                    /* PLG-9j (2026-06-01, Opus 4.8): compound / op-term arg (IR_STRUCT / IR_ARITH — rich     */
                    /* tier, 8-misaligned box entry) → build the Term* via emit_build_compound_term then      */
                    /* rt_write_term_ptr@PLT (which calls pl_write — the mode-2 oracle's writer, so it     */
                    /* sugars cons-cells against ATOM_DOT/ATOM_NIL: [a,b,c] not .(a,.(b,.(c,[])))). Replaces  */
                    /* the old emit_write_term inline walker, which rendered an IR_STRUCT '.'/2 generically   */
                    /* as functor notation (the m4 write-list gap — rung20 list). Byte-twin of the writeq     */
                    /* TEXT arm. sub rsp,8 realigns to 16 across the build's and writer's internal calls.     */
                    write_body = s_2asm("sub", "rsp, 8")
                               + emit_build_compound_term(arg)
                               + s_2asm("mov", "rdi, rax")
                               + s_2asm("call", "rt_write_term_ptr@PLT")
                               + s_2asm("add", "rsp, 8");
                }
            } else {
                write_body = s_comment("# RESOLVE_BUILTIN write: no arg");
            }
            std::string nl_suffix = (strcmp(fn, "writeln") == 0)
                ? s_2asm("mov", "edi, 10") + s_2asm("call", "putchar@PLT")
                : std::string();
            return hdr + write_body + nl_suffix + succ_back;
        }
        if ((strcmp(fn, "writeq") == 0 || strcmp(fn, "write_canonical") == 0) && pBB->α) {
            const char *writer = (strcmp(fn, "writeq") == 0)
                ? "rt_writeq_term_ptr@PLT"
                : "rt_write_canonical_term_ptr@PLT";
            return hdr
                 + s_2asm("sub", "rsp, 8")
                 + emit_build_compound_term(pBB->α)
                 + s_2asm("mov", "rdi, rax")
                 + s_2asm("call", writer)
                 + s_2asm("add", "rsp, 8")
                 + succ_back;
        }
    }
    return std::string();
}
