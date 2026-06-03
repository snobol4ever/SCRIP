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
int  rt_gvar_assign_concat(const char * name, void * left_graph, void * right_graph);
void rt_gvar_assign_descr(const char * name, int64_t lo, int64_t hi);
int  bb_slot_get(IR_t * nd);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static inline const char * dst_name()  { return _.op_sval   ? _.op_sval   : ""; }
static inline uint64_t     dst_addr()  { return (uint64_t)(uintptr_t)dst_name(); }
static inline const char * dst_label() { const char * l = emit_intern_str(dst_name()); if (l) return l; static char b[64]; strtab_label(b, sizeof b, dst_name()); return b; }
static inline const char * rhs_sval()  { return _.op_a_sval ? _.op_a_sval : ""; }
static inline uint64_t     rhs_addr()  { return (uint64_t)(uintptr_t)rhs_sval(); }
static inline const char * rhs_label() { const char * l = emit_intern_str(rhs_sval()); if (l) return l; static char b[64]; strtab_label(b, sizeof b, rhs_sval()); return b; }
static inline uint64_t     fn_lit_s()  { void (*f)(const char *, const char *) = rt_gvar_assign_str; return (uint64_t)(uintptr_t)(void *)f; }
static inline uint64_t     fn_var()    { void (*f)(const char *, const char *) = rt_gvar_assign_var;    return (uint64_t)(uintptr_t)(void *)f; }
static inline uint64_t     fn_int()    { void (*f)(const char *, int64_t)      = rt_gvar_assign_int;    return (uint64_t)(uintptr_t)(void *)f; }
static inline uint64_t     lhs_graph()  { return (uint64_t)(uintptr_t)(intptr_t) _.op_a_counter; }
static inline uint64_t     rhs_graph()  { return (uint64_t)(uintptr_t)(intptr_t) _.op_a_ival_sg; }
static inline uint64_t     fn_concat()  { int (*f)(const char *, void *, void *) = rt_gvar_assign_concat; return (uint64_t)(uintptr_t)(void *)f; }
static inline uint64_t     fn_descr()   { void (*f)(const char *, int64_t, int64_t) = rt_gvar_assign_descr; return (uint64_t)(uintptr_t)(void *)f; }
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
        if (_.op_a_node_kind == (int)IR_SEQ) {
            if (MEDIUM_TEXT) return x86_bomb("bb_gvar_assign concat: TEXT(mode-4) needs relocatable subgraph addrs (SNOBOL m4 pending LOWER four-port wiring)");
            return x86("lea",  "rdi", "[rip + __]", dst_addr(), dst_label())
                 + x86_load_ro("rsi", "??", lhs_graph())
                 + x86_load_ro("rdx", "??", rhs_graph())
                 + x86("call", "rt_gvar_assign_concat", fn_concat())
                 + x86("jmp",  PORT_GAMMA)
                 + x86("def",  PORT_BETA)
                 + x86("jmp",  PORT_OMEGA);
        }
        if (_.op_a_node_kind == (int)IR_CALL) {
            int slot = _.op_a_slot;
            if (slot < 0) return x86_bomb("bb_gvar_assign call-result: op_a_slot==-1 (call result slot not promoted)");
            if (MEDIUM_TEXT)
                return s_1asm(std::string(_.lbl_α) + ":")
                     + s_comment("# BOX SNO IR_ASSIGN(call-result) store = DESCR from call zeta-slot [RO dst ptr, @PLT]")
                     + x86("lea",  "rdi", "[rip + __]", dst_addr(), dst_label())
                     + x86_frame_load64("rsi", slot)
                     + x86_frame_load64("rdx", slot + 8)
                     + x86("call", "rt_gvar_assign_descr", fn_descr())
                     + x86("jmp",  PORT_GAMMA)
                     + x86("def",  PORT_BETA)
                     + x86("jmp",  PORT_OMEGA);
            return x86("lea",  "rdi", "[rip + __]", dst_addr(), dst_label())
                 + x86_frame_load64("rsi", slot)
                 + x86_frame_load64("rdx", slot + 8)
                 + x86("call", "rt_gvar_assign_descr", fn_descr())
                 + x86("jmp",  PORT_GAMMA)
                 + x86("def",  PORT_BETA)
                 + x86("jmp",  PORT_OMEGA);
        }
        return x86_bomb("bb_gvar_assign other: unhandled rhs shape");
    }
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_gvar_assign(IR_t * pBB) { (void)pBB; bb_emit_x86(bb_gvar_assign_str()); }
