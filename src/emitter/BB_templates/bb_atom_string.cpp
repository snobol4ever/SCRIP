#include "bb_common.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * bas_fn() { return _.op_sval ? _.op_sval : ""; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * bas_cn() { const char *f = bas_fn();
    return (!strcmp(f,"atom_length") || !strcmp(f,"string_length")) ? "rt_atom_length"
         : (!strcmp(f,"upcase_atom") || !strcmp(f,"string_upper"))  ? "rt_upcase_atom"
         : (!strcmp(f,"downcase_atom") || !strcmp(f,"string_lower"))? "rt_downcase_atom"
         : (!strcmp(f,"copy_term"))                                 ? "rt_copy_term"
         :                                                            "rt_atom_string_pair"; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void * bas_cp() { const char *f = bas_fn();
    return (!strcmp(f,"atom_length") || !strcmp(f,"string_length")) ? (void*)rt_atom_length
         : (!strcmp(f,"upcase_atom") || !strcmp(f,"string_upper"))  ? (void*)rt_upcase_atom
         : (!strcmp(f,"downcase_atom") || !strcmp(f,"string_lower"))? (void*)rt_downcase_atom
         : (!strcmp(f,"copy_term"))                                 ? (void*)rt_copy_term
         :                                                            (void*)rt_atom_string_pair; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bas_s(const char *r64, const char *r32, int j, int atomgate) { char lb[64];
    if ((!atomgate || _.op_parts_tag[j] == (int)IR_ATOM) && _.op_parts_str[j]) { strtab_label(lb, sizeof lb, _.op_parts_str[j]);
        return x86("lea", r64, "[rip + __]", (unsigned long long)(uintptr_t)_.op_parts_str[j], lb); }
    return x86("xor", r32, r32); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bas_term(int slot) { const IR_t *nd = (const IR_t *)(intptr_t)_.op_parts_ival[slot];
    if (MEDIUM_BINARY) return x86_lit_bytes(emit_term_from_node_bin(nd));
    return emit_build_compound_term(nd); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_atom_string_str(const std::string &hdr) {
    if (PLATFORM_X86) return
          IF(!strcmp(bas_fn(),"copy_term") && _.op_parts_n >= 2 && (_.op_parts_tag[0] == (int)IR_STRUCT || _.op_parts_tag[0] == (int)IR_ARITH),
              hdr
            + x86("sub", "rsp", 16L)
            + IF(_.op_parts_tag[1] == (int)IR_STRUCT || _.op_parts_tag[1] == (int)IR_ARITH,
                  bas_term(8)
                + x86("mov", RSP(0), "rax")
                + bas_term(9)
                + x86("mov", "rsi", "rax")
                + x86("mov", "rdi", RSP(0))
                + x86("call", "rt_copy_term_terms", (unsigned long long)(uintptr_t)(void*)rt_copy_term_terms))
            + IF(!(_.op_parts_tag[1] == (int)IR_STRUCT || _.op_parts_tag[1] == (int)IR_ARITH),
                  bas_term(8)
                + x86("mov", "rdi", "rax")
                + x86("mov32", "esi", (long)_.op_parts_tag[1])
                + x86("movabs", "rdx", (unsigned long long)(uint64_t)_.op_parts_ival[1])
                + bas_s("rcx", "ecx", 1, 1)
                + x86("call", "rt_copy_term_term", (unsigned long long)(uintptr_t)(void*)rt_copy_term_term))
            + x86("add", "rsp", 16L)
            + x86("test", "eax", "eax")
            + x86("je", "ω")
            + x86("jmp", "γ")
            + x86("label", _.lbl_β)
            + x86("jmp", "ω"))
        + IF((!strcmp(bas_fn(),"atom_length")   || !strcmp(bas_fn(),"upcase_atom")    || !strcmp(bas_fn(),"downcase_atom")
           || !strcmp(bas_fn(),"string_length") || !strcmp(bas_fn(),"string_upper")   || !strcmp(bas_fn(),"string_lower")
           || !strcmp(bas_fn(),"atom_string")   || !strcmp(bas_fn(),"string_to_atom") || !strcmp(bas_fn(),"copy_term"))
           && _.op_parts_n >= 2 && !(!strcmp(bas_fn(),"copy_term") && (_.op_parts_tag[0] == (int)IR_STRUCT || _.op_parts_tag[0] == (int)IR_ARITH)),
              hdr
            + x86("mov32", "edi", (long)_.op_parts_tag[0])
            + x86("movabs", "rsi", (unsigned long long)(uint64_t)_.op_parts_ival[0])
            + bas_s("rdx", "edx", 0, 1)
            + x86("mov32", "ecx", (long)_.op_parts_tag[1])
            + x86("movabs", "r8", (unsigned long long)(uint64_t)_.op_parts_ival[1])
            + bas_s("r9", "r9d", 1, 1)
            + x86("call", bas_cn(), (unsigned long long)(uintptr_t)bas_cp())
            + x86("test", "eax", "eax")
            + x86("je", "ω")
            + x86("jmp", "γ")
            + x86("label", _.lbl_β)
            + x86("jmp", "ω"))
        + IF((!strcmp(bas_fn(),"number_string") || !strcmp(bas_fn(),"atom_number")) && _.op_parts_n >= 2,
              hdr
            + x86("sub", "rsp", 16L)
            + x86("mov32", "edi", (long)(!strcmp(bas_fn(),"number_string") ? 1 : 0))
            + x86("mov32", "esi", (long)_.op_parts_tag[0])
            + x86("movabs", "rdx", (unsigned long long)(uint64_t)_.op_parts_ival[0])
            + bas_s("rcx", "ecx", 0, 1)
            + x86("mov32", "r8d", (long)_.op_parts_tag[1])
            + x86("movabs", "r9", (unsigned long long)(uint64_t)_.op_parts_ival[1])
            + bas_s("rax", "eax", 1, 1)
            + x86("mov", RSP(0), "rax")
            + x86("call", "rt_number_string_pair", (unsigned long long)(uintptr_t)(void*)rt_number_string_pair)
            + x86("add", "rsp", 16L)
            + x86("test", "eax", "eax")
            + x86("je", "ω")
            + x86("jmp", "γ")
            + x86("label", _.lbl_β)
            + x86("jmp", "ω"))
        + IF((!strcmp(bas_fn(),"atom_concat") || !strcmp(bas_fn(),"string_concat")) && _.op_parts_n >= 3,
              hdr
            + x86("sub", "rsp", 32L)
            + x86("mov32", "edi", (long)_.op_parts_tag[0])
            + x86("movabs", "rsi", (unsigned long long)(uint64_t)_.op_parts_ival[0])
            + bas_s("rdx", "edx", 0, 1)
            + x86("mov32", "ecx", (long)_.op_parts_tag[1])
            + x86("movabs", "r8", (unsigned long long)(uint64_t)_.op_parts_ival[1])
            + bas_s("r9", "r9d", 1, 1)
            + x86("stk32", 0L, (long)_.op_parts_tag[2])
            + x86("movabs", "rax", (unsigned long long)(uint64_t)_.op_parts_ival[2])
            + x86("mov", RSP(8), "rax")
            + bas_s("rax", "eax", 2, 1)
            + x86("mov", RSP(16), "rax")
            + x86("call", "rt_atom_concat", (unsigned long long)(uintptr_t)(void*)rt_atom_concat)
            + x86("add", "rsp", 32L)
            + x86("test", "eax", "eax")
            + x86("je", "ω")
            + x86("jmp", "γ")
            + x86("label", _.lbl_β)
            + x86("jmp", "ω"))
        + IF((!strcmp(bas_fn(),"atom_chars") || !strcmp(bas_fn(),"atom_codes") || !strcmp(bas_fn(),"string_chars") || !strcmp(bas_fn(),"string_codes")) && _.op_parts_n >= 2,
              hdr
            + x86("sub", "rsp", 16L)
            + IF(_.op_parts_tag[1] == (int)IR_STRUCT,
                  bas_term(9)
                + x86("mov", "r8", "rax")
                + x86("mov32", "edi", (long)(!strcmp(bas_fn(),"atom_codes") || !strcmp(bas_fn(),"string_codes") ? 1 : 0))
                + x86("mov32", "esi", (long)_.op_parts_tag[0])
                + x86("movabs", "rdx", (unsigned long long)(uint64_t)_.op_parts_ival[0])
                + bas_s("rcx", "ecx", 0, 1)
                + x86("call", "rt_atom_chars_codes_term", (unsigned long long)(uintptr_t)(void*)rt_atom_chars_codes_term))
            + IF(_.op_parts_tag[1] != (int)IR_STRUCT,
                  x86("mov32", "edi", (long)(!strcmp(bas_fn(),"atom_codes") || !strcmp(bas_fn(),"string_codes") ? 1 : 0))
                + x86("mov32", "esi", (long)_.op_parts_tag[0])
                + x86("movabs", "rdx", (unsigned long long)(uint64_t)_.op_parts_ival[0])
                + bas_s("rcx", "ecx", 0, 1)
                + x86("mov32", "r8d", (long)_.op_parts_tag[1])
                + x86("movabs", "r9", (unsigned long long)(uint64_t)_.op_parts_ival[1])
                + bas_s("rax", "eax", 1, 1)
                + x86("mov", RSP(0), "rax")
                + x86("call", "rt_atom_chars_codes", (unsigned long long)(uintptr_t)(void*)rt_atom_chars_codes))
            + x86("add", "rsp", 16L)
            + x86("test", "eax", "eax")
            + x86("je", "ω")
            + x86("jmp", "γ")
            + x86("label", _.lbl_β)
            + x86("jmp", "ω"))
        + IF(!strcmp(bas_fn(),"char_type") && _.op_ival == 2 && _.op_parts_n >= 2,
              hdr
            + x86("sub", "rsp", 16L)
            + x86("mov32", "edi", (long)_.op_parts_tag[0])
            + x86("movabs", "rsi", (unsigned long long)(uint64_t)_.op_parts_ival[0])
            + bas_s("rdx", "edx", 0, 1)
            + bas_s("rcx", "ecx", 1, 0)
            + x86("mov32", "r8d", (long)(((_.op_parts_tag[1] == (int)IR_STRUCT || _.op_parts_tag[1] == (int)IR_ARITH) && _.op_parts_str[1]) ? 1 : 0))
            + x86("mov32", "r9d", (long)_.op_parts_tag[3])
            + x86("movabs", "rax", (unsigned long long)(uint64_t)_.op_parts_ival[3])
            + x86("mov", RSP(0), "rax")
            + bas_s("rax", "eax", 3, 1)
            + x86("mov", RSP(8), "rax")
            + x86("call", "rt_char_type", (unsigned long long)(uintptr_t)(void*)rt_char_type)
            + x86("add", "rsp", 16L)
            + x86("test", "eax", "eax")
            + x86("je", "ω")
            + x86("jmp", "γ")
            + x86("label", _.lbl_β)
            + x86("jmp", "ω"));
    return std::string();
}
