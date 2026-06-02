/* bb_nv_assign.cpp — x86() self-encoding (TEMPLATE-REVAMP, 2026-06-02). SNOBOL4 IR_ASSIGN.
   pBB-free: the dst name is _.op_sval (the assign node's own sval); the rhs operand's sval + kind are
   _.op_a_sval / _.op_a_node_kind, PROMOTED at the single dispatch point from nd->α (the no-neighbor FACT
   RULE — the dispatcher, which sees the graph, marshals operands onto _; the box reads only _, never a
   neighbor pBB->α). Two arms converted clean (two RO pointer loads + one runtime call, via the keystone
   x86_load_ro / x86_call_ro encoders):
       lit_s : `DST = "literal"`  → rt_nv_assign_str(dst, str)
       var   : `DST = SRC`         → rt_nv_assign_var(dst, src)
   Two arms still LOUD-bomb (each needs more than a box rewrite; documented):
       int-binop : reads the binop neighbor's ζ-slot via bb_slot_get(α) — a fusion the no-neighbor rule
                   forbids; the clean form promotes the rhs slot OFFSET onto _ (op_a_slot) at dispatch, a
                   small follow-on plumbing step (parallel to this op_a_sval promotion).
       concat    : the old arm baked process-local sub-graph pointers (mode-3-only, NOT relocatable; its TEXT
                   arm already bombed). The relocatable form is the STITCH_SEQ pattern-graph build (PB-RB-4),
                   not a box rewrite.
   Single-shot statement box (an assign does not re-offer): β = jmp ω. No r10 guard / no rsp-align dance —
   matches the original (rsp is 16-aligned at α entry, so one direct call is SysV-correct). */
#include <string>
#include <cstdint>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "descr.h"
void rt_nv_assign_str(const char * name, const char * str);
void rt_nv_assign_var(const char * dst, const char * src);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline const char * dst_name()  { return _.op_sval   ? _.op_sval   : ""; }
static inline uint64_t     dst_addr()  { return (uint64_t)(uintptr_t)dst_name(); }
static inline const char * dst_label() { return emit_intern_str(dst_name()); }
static inline const char * rhs_sval()  { return _.op_a_sval ? _.op_a_sval : ""; }
static inline uint64_t     rhs_addr()  { return (uint64_t)(uintptr_t)rhs_sval(); }
static inline const char * rhs_label() { return emit_intern_str(rhs_sval()); }
static inline uint64_t     fn_lit_s()  { void (*f)(const char *, const char *) = rt_nv_assign_str; return (uint64_t)(uintptr_t)(void *)f; }
static inline uint64_t     fn_var()    { void (*f)(const char *, const char *) = rt_nv_assign_var;    return (uint64_t)(uintptr_t)(void *)f; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_nv_assign_str() {
    if (PLATFORM_X86) {
        if (_.op_a_node_kind == (int)IR_LIT_S)
            return IF(MEDIUM_TEXT, s_1asm(std::string(_.lbl_α) + ":")
                                 + s_comment("# BOX SNO IR_ASSIGN(lit_s) store = literal [RO ptrs, @PLT]"))
                 + x86("lea",  "rdi", "[rip + __]", dst_addr(), dst_label())
                 + x86("lea",  "rsi", "[rip + __]", rhs_addr(), rhs_label())
                 + x86("call", "rt_nv_assign_str", fn_lit_s())
                 + x86("jmp",  PORT_GAMMA)
                 + x86("def",  PORT_BETA)
                 + x86("jmp",  PORT_OMEGA);
        if (_.op_a_node_kind == (int)IR_VAR)
            return IF(MEDIUM_TEXT, s_1asm(std::string(_.lbl_α) + ":")
                                 + s_comment("# BOX SNO IR_ASSIGN(var) store = read(src) [RO ptrs, @PLT]"))
                 + x86("lea",  "rdi", "[rip + __]", dst_addr(), dst_label())
                 + x86("lea",  "rsi", "[rip + __]", rhs_addr(), rhs_label())
                 + x86("call", "rt_nv_assign_var", fn_var())
                 + x86("jmp",  PORT_GAMMA)
                 + x86("def",  PORT_BETA)
                 + x86("jmp",  PORT_OMEGA);
        if (_.op_a_node_kind == (int)IR_BINOP)
            return x86_bomb("bb_nv_assign int-binop: needs the binop's ζ-slot offset promoted onto _ (op_a_slot)");
        return x86_bomb("bb_nv_assign concat/other: relocatable form is STITCH_SEQ (PB-RB-4), not a box rewrite");
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_nv_assign(IR_t * pBB) { (void)pBB; bb_emit_x86(bb_nv_assign_str()); }
