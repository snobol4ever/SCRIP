#include <string>
#include <cstdint>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "descr.h"
void rt_gvar_assign_str(const char * name, const char * str);
void rt_gvar_assign_var(const char * dst, const char * src);
void rt_gvar_assign_int(const char * name, int64_t val);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static inline const char * dst_name()  { return _.op_sval   ? _.op_sval   : ""; }
static inline uint64_t     dst_addr()  { return (uint64_t)(uintptr_t)dst_name(); }
static inline const char * dst_label() { return emit_intern_str(dst_name()); }
static inline const char * rhs_sval()  { return _.op_a_sval ? _.op_a_sval : ""; }
static inline uint64_t     rhs_addr()  { return (uint64_t)(uintptr_t)rhs_sval(); }
static inline const char * rhs_label() { return emit_intern_str(rhs_sval()); }
static inline uint64_t     fn_lit_s()  { void (*f)(const char *, const char *) = rt_gvar_assign_str; return (uint64_t)(uintptr_t)(void *)f; }
static inline uint64_t     fn_var()    { void (*f)(const char *, const char *) = rt_gvar_assign_var;    return (uint64_t)(uintptr_t)(void *)f; }
static inline uint64_t     fn_int()    { void (*f)(const char *, int64_t)      = rt_gvar_assign_int;    return (uint64_t)(uintptr_t)(void *)f; }
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_gvar_assign_str() {
    if (PLATFORM_X86) {
        if (_.op_a_node_kind == (int)IR_LIT_S)
            return IF(MEDIUM_TEXT, s_1asm(std::string(_.lbl_α) + ":")
                                 + s_comment("# BOX SNO IR_ASSIGN(lit_s) store = literal [RO ptrs, @PLT]"))
                 + x86("lea",  "rdi", "[rip + __]", dst_addr(), dst_label())
                 + x86("lea",  "rsi", "[rip + __]", rhs_addr(), rhs_label())
                 + x86("call", "rt_gvar_assign_str", fn_lit_s())
                 + x86("jmp",  PORT_GAMMA)
                 + x86("def",  PORT_BETA)
                 + x86("jmp",  PORT_OMEGA);
        if (_.op_a_node_kind == (int)IR_VAR)
            return IF(MEDIUM_TEXT, s_1asm(std::string(_.lbl_α) + ":")
                                 + s_comment("# BOX SNO IR_ASSIGN(var) store = read(src) [RO ptrs, @PLT]"))
                 + x86("lea",  "rdi", "[rip + __]", dst_addr(), dst_label())
                 + x86("lea",  "rsi", "[rip + __]", rhs_addr(), rhs_label())
                 + x86("call", "rt_gvar_assign_var", fn_var())
                 + x86("jmp",  PORT_GAMMA)
                 + x86("def",  PORT_BETA)
                 + x86("jmp",  PORT_OMEGA);
        if (_.op_a_node_kind == (int)IR_BINOP) {
            int slot = _.op_a_slot;
            if (slot < 0) return x86_bomb("bb_gvar_assign int-binop: op_a_slot==-1 (binop slot not promoted)");
            return IF(MEDIUM_TEXT, s_1asm(std::string(_.lbl_α) + ":")
                                 + s_comment("# BOX SNO IR_ASSIGN(int-binop) store = binop-int64-slot [RO dst ptr, FRQ slot, @PLT]"))
                 + x86("lea",  "rdi", "[rip + __]", dst_addr(), dst_label())
                 + x86("mov",  "rsi", FRQ(slot))
                 + x86("call", "rt_gvar_assign_int", fn_int())
                 + x86("jmp",  PORT_GAMMA)
                 + x86("def",  PORT_BETA)
                 + x86("jmp",  PORT_OMEGA);
        }
        return x86_bomb("bb_gvar_assign concat/other: relocatable form is STITCH_SEQ (PB-RB-4), not a box rewrite");
    }
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_gvar_assign(IR_t * pBB) { (void)pBB; bb_emit_x86(bb_gvar_assign_str()); }
