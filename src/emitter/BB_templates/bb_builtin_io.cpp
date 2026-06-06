#include "bb_builtin_common.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bio_succ(const char *lγ, const char *lβ) {
    return x86("ins2", "jmp", lγ) + x86("Lins2", std::string(lβ) + ":", "jmp", lγ);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bio_fbits_str(double d) {
    union { double dv; uint64_t u; } fb = {d};
    return std::to_string((unsigned long long)fb.u);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bio_bin_write_arg(IR_t *arg) {
    if (!arg) return std::string();
    if (arg->t == IR_ATOM) {
        const char *atom = arg->sval ? arg->sval : "";
        return bytes(2, "\x48\xBF") + u64le((uint64_t)(uintptr_t)atom)
             + bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_write_atom)
             + bytes(2, "\xFF\xD0");
    }
    if (arg->t == IR_LOGICVAR)
        return bytes(1, "\xBF") + u32le((uint32_t)(int)arg->ival)
             + bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_write_var)
             + bytes(2, "\xFF\xD0");
    return bytes(4, "\x48\x83\xEC\x08")
         + emit_term_from_node_bin(arg)
         + bytes(3, "\x48\x89\xC7")
         + bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_write_term_ptr)
         + bytes(2, "\xFF\xD0")
         + bytes(4, "\x48\x83\xC4\x08");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bio_write_body(const char *bb_ls, long op_ival, IR_t *α) {
    if (op_ival < 1 || !α)
        return x86("comment", "RESOLVE_BUILTIN write: no arg");
    if (α->t == IR_ATOM)
        return (bb_ls ? x86("ins2", "lea rcx,", std::string("[rip + ") + bb_ls + "]") + x86("ins2", "mov", "rdi, rcx")
                      : x86("ins2", "xor", "edi, edi"))
             + x86("ins2", "call", "rt_write_atom@PLT");
    if (α->t == IR_LOGICVAR)
        return x86("ins2", "mov edi,", std::to_string((int)α->ival)) + x86("ins2", "call", "rt_write_var@PLT");
    if (α->t == IR_LIT_I)
        return x86("ins2", "mov rdi,", std::to_string((long)α->ival)) + x86("ins2", "call", "rt_write_int@PLT");
    if (α->t == IR_LIT_F)
        return x86("ins2", "mov rax,", bio_fbits_str(α->dval))
             + x86("ins2", "movq", "xmm0, rax")
             + x86("ins2", "call", "rt_write_float@PLT");
    return x86("ins2", "sub", "rsp, 8")
         + emit_build_compound_term(α)
         + x86("ins2", "mov", "rdi, rax")
         + x86("ins2", "call", "rt_write_term_ptr@PLT")
         + x86("ins2", "add", "rsp, 8");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_builtin_io_str(IR_t *pBB, const char *fn, const std::string &hdr) {
    (void)pBB; (void)fn; (void)hdr;
    if (MEDIUM_BINARY) {
        if (strcmp(fn, "nl") == 0)
            return x86_lit_bytes(
                bytes(1, "\xBF") + u32le(10)
                + bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)putchar) + bytes(2, "\xFF\xD0")
            ) + x86("jmp", "γ") + x86("def", "β") + x86("jmp", "γ");
        if (strcmp(fn, "halt") == 0)
            return x86_lit_bytes(
                bytes(2, "\x31\xFF")
                + bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)exit) + bytes(2, "\xFF\xD0")
            ) + x86("def", "β") + x86("jmp", "γ");
        if (strcmp(fn, "write") == 0 || strcmp(fn, "writeln") == 0 || strcmp(fn, "print") == 0)
            return x86_lit_bytes(
                (_.op_ival >= 1 && pBB->α ? bio_bin_write_arg(pBB->α) : std::string())
                + (strcmp(fn, "writeln") == 0
                   ? bytes(1, "\xBF") + u32le(10) + bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)putchar) + bytes(2, "\xFF\xD0")
                   : std::string())
            ) + x86("jmp", "γ") + x86("def", "β") + x86("jmp", "γ");
        if ((strcmp(fn, "writeq") == 0 || strcmp(fn, "write_canonical") == 0) && pBB->α) {
            void *writer = (strcmp(fn, "writeq") == 0) ? (void*)rt_writeq_term_ptr : (void*)rt_write_canonical_term_ptr;
            return x86_lit_bytes(
                bytes(4, "\x48\x83\xEC\x08")
                + emit_term_from_node_bin(pBB->α)
                + bytes(3, "\x48\x89\xC7")
                + bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)writer) + bytes(2, "\xFF\xD0")
                + bytes(4, "\x48\x83\xC4\x08")
            ) + x86("jmp", "γ") + x86("def", "β") + x86("jmp", "γ");
        }
    }
    if (MEDIUM_TEXT) {
        if (strcmp(fn, "nl") == 0)
            return hdr + x86("ins2", "mov", "edi, 10") + x86("ins2", "call", "putchar@PLT") + bio_succ(_.lbl_γ, _.lbl_β);
        if (strcmp(fn, "halt") == 0)
            return hdr + x86("ins2", "xor", "edi, edi") + x86("ins2", "call", "exit@PLT")
                 + x86("Lins2", std::string(_.lbl_β) + ":", "jmp", _.lbl_γ);
        if (strcmp(fn, "write") == 0 || strcmp(fn, "writeln") == 0 || strcmp(fn, "print") == 0)
            return hdr
                 + bio_write_body(_.bb_ls, _.op_ival, pBB->α)
                 + ((strcmp(fn, "writeln") == 0) ? x86("ins2", "mov", "edi, 10") + x86("ins2", "call", "putchar@PLT") : std::string())
                 + bio_succ(_.lbl_γ, _.lbl_β);
        if ((strcmp(fn, "writeq") == 0 || strcmp(fn, "write_canonical") == 0) && pBB->α)
            return hdr
                 + x86("ins2", "sub", "rsp, 8")
                 + emit_build_compound_term(pBB->α)
                 + x86("ins2", "mov", "rdi, rax")
                 + x86("ins2", "call", (strcmp(fn, "writeq") == 0) ? "rt_writeq_term_ptr@PLT" : "rt_write_canonical_term_ptr@PLT")
                 + x86("ins2", "add", "rsp, 8")
                 + bio_succ(_.lbl_γ, _.lbl_β);
    }
    return std::string();
}
