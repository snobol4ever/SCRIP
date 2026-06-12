#include <string>
#include <cstdint>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "emit.h"
void rt_gvar_assign_str(const char * name, const char * str);
int  rt_gvar_assign_concat(const char * name, void * left_graph, void * right_graph);
void rt_gvar_assign_concat_parts(const char * dst, void * parts, int n);
int  bb_slot_claim(int bytes);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static inline const char * dst_name()    { return _.op_sval   ? _.op_sval   : ""; }
static inline uint64_t     dst_addr()    { return (uint64_t)(uintptr_t)dst_name(); }
static inline const char * dst_label()   { const char * l = emit_intern_str(dst_name()); if (l) return l; static char b[64]; strtab_label(b, sizeof b, dst_name()); return b; }
static inline uint64_t     lhs_graph()   { return (uint64_t)(uintptr_t)(intptr_t) _.op_a_counter; }
static inline uint64_t     rhs_graph()   { return (uint64_t)(uintptr_t)(intptr_t) _.op_a_ival_sg; }
static inline uint64_t     fn_str()      { void (*f)(const char *, const char *) = rt_gvar_assign_str; return (uint64_t)(uintptr_t)(void *)f; }
static inline uint64_t     fn_concat()        { int  (*f)(const char *, void *, void *) = rt_gvar_assign_concat;       return (uint64_t)(uintptr_t)(void *)f; }
static inline uint64_t     fn_concat_parts()  { void (*f)(const char *, void *, int)    = rt_gvar_assign_concat_parts; return (uint64_t)(uintptr_t)(void *)f; }
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_gvar_assign_concat() {
    if (PLATFORM_X86) {
        if (_.op_a_node_kind == (int)IR_LIT_S) {
            const char * sv = _.op_a_sval ? _.op_a_sval : "";
            uint64_t sa = (uint64_t)(uintptr_t)sv;
            const char * sl = emit_intern_str(sv); static char sb[64]; if (!sl) { strtab_label(sb, sizeof sb, sv); sl = sb; }
            return IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                                 + x86("comment", "BOX IR_ASSIGN_CONCAT(folded->lit_s) store = literal [RO ptrs, @PLT]"))
                 + x86("lea",  "rdi", "[rip + __]", dst_addr(), dst_label())
                 + x86("lea",  "rsi", "[rip + __]", sa, sl)
                 + x86("call", "rt_gvar_assign_str", fn_str())
                 + x86("jmp",  "\xCE\xB3")
                 + x86("def",  "\xCE\xB2")
                 + x86("jmp",  "\xCF\x89");
        }
        if (MEDIUM_TEXT) {
            int n = _.op_parts_n;
            if (n <= 0) return x86_bomb("bb_gvar_assign_concat: TEXT(mode-4) value shape not flattenable to lit/var parts (pending)");
            int off = bb_slot_claim(16 * n);
            std::string s = x86("label", _.lbl_α)
                          + x86("comment", std::string("BOX IR_ASSIGN_CONCAT ") + std::to_string(n) + " staged parts -> rt_gvar_assign_concat_parts [zeta region, @PLT]");
            for (int i = 0; i < n; i++) {
                const char *ps = _.op_parts_str[i] ? _.op_parts_str[i] : "";
                const char *pl = emit_intern_str(ps); static char b[64]; if (!pl) { strtab_label(b, sizeof b, ps); pl = b; }
                s += x86("mov", FR(off + 16 * i), (long)_.op_parts_tag[i])
                   + x86("lea", "rax", "[rip + __]", (uint64_t)(uintptr_t)ps, pl)
                   + x86("mov", FRQ(off + 16 * i + 8), "rax");
            }
            s += x86("lea",  "rdi", "[rip + __]", dst_addr(), dst_label())
               + x86("lea",  "rsi", FR(off))
               + x86("mov",  "edx", (long)n)
               + x86("push", "r10")
               + x86("push", "rbx")
               + x86("mov",  "rbx", "rsp")
               + x86("and",  "rsp", -16L)
               + x86("call", "rt_gvar_assign_concat_parts", fn_concat_parts())
               + x86("mov",  "rsp", "rbx")
               + x86("pop",  "rbx")
               + x86("pop",  "r10")
               + x86("jmp",  "\xCE\xB3")
               + x86("def",  "\xCE\xB2")
               + x86("jmp",  "\xCF\x89");
            return s;
        }
        return x86("lea",  "rdi", "[rip + __]", dst_addr(), dst_label())
             + x86_load_ro("rsi", "??", lhs_graph())
             + x86_load_ro("rdx", "??", rhs_graph())
             + x86("call", "rt_gvar_assign_concat", fn_concat())
             + x86("jmp",  "\xCE\xB3")
             + x86("def",  "\xCE\xB2")
             + x86("jmp",  "\xCF\x89");
    }
    return std::string();
}