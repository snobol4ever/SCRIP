/* bb_assign.cpp — BB template for IR_ASSIGN (Icon variable assign).
   IBB-7 (2026-05-29). Composite: α = lhs (IR_VAR — sval is the slot name), β = rhs subgraph.
   The flat-driver walks β first (pushing the value onto vstack), then defines lbl_α at this
   template's location; the template emits the trailer `rt_pop_nv_set(name); jmp γ; β: jmp ω`.

   Mirrors bb_call.cpp write(int_expr) trailer (22-byte movabs/call/jmp/β-jmp), but loads the
   name pointer into rdi (SysV arg0) before the call.

   Bytes (32 total):
       0  : 48 BF + u64le name_ptr      movabs rdi, name_ptr
       10 : 48 B8 + u64le fn_ptr        movabs rax, &rt_pop_nv_set
       20 : FF D0                       call rax
       22 : E9 + u32le γ_rel32          jmp γ              (patch at 23 → lbl_γ)
       27 : E9 + u32le ω_rel32          β: jmp ω           (β-def at 27, ω patch at 28)
       32 : end

   The driver (flat_drive_assign in emit_bb.c) is responsible for walking β before the IR_ASSIGN
   template label is defined. The template assumes the value is on the vstack at entry.
*/
#include <string>
#include <string.h>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
void rt_pop_nv_set(const char *name);
int  bb_slot_get(IR_t * nd);
int  bb_varslot(const char * name);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_assign_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (!PLATFORM_X86) return std::string();
    if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — IR_ASSIGN");
    /* GZ-7 (GROUND ZERO 3) flat-chain slot model: an Icon `x := E` assignment. The unified lowerer puts */
    /* the target NAME in pBB->sval and threads the RHS via the γ-chain; icn_chain_operand_refs records   */
    /* the RHS producer box on pBB->α (an OPERAND REFERENCE, not for re-walk). This box reads the RHS     */
    /* producer's slot [r12+off_rhs] (bb_slot_get) and writes the named variable slot [r12+off_x]         */
    /* (bb_varslot keyed by name, shared with IR_VAR readers). 16-byte DESCR copy. The variable IS its    */
    /* slot — the test_sno_1.c named-slot model. NO rt_pop_nv_set, NO value stack, NO ring.              */
    /* GZ-9 (2026-05-31): the RHS may also be an IR_BINOP (e.g. `i := i + 1` of a while/until loop). The   */
    /* binop is its own producer box (bb_binop GZ-9 arith arm) that wrote a DESCR into its slot; the copy  */
    /* below is generic over the producer kind, so accepting IR_BINOP needs only the type-guard widened.   */
    /* RK-EMIT-2 (2026-05-31): IR_CALL is likewise a producer box — the dval==2.0 general builtin call      */
    /* (bb_call RK-EMIT-2 arm) stores its result DESCR into its own slot; `@a = elems(...)` / `@a = (..)`   */
    /* (list ctor) / `$x = sort(@a)` all reach here with pBB->α an IR_CALL. The 16-byte copy is identical.  */
    /* RK-EMIT-GATHER (2026-06-01): IR_GATHER is likewise a producer box — the resumable Seq producer       */
    /* (bb_rk_gather) writes its yielded element DESCR into its own ζ slot; `for gather{..} -> $v` reaches   */
    /* here with pBB->α the IR_GATHER node (set by icn_chain_operand_refs as the bind's operand). The        */
    /* 16-byte copy from the producer slot to the var slot is identical to the IR_BINOP/IR_CALL cases.       */
    if (g_icn_flat_chain && pBB && pBB->sval && pBB->α
        && (pBB->α->t == IR_LIT_I || pBB->α->t == IR_LIT_S || pBB->α->t == IR_VAR || pBB->α->t == IR_BINOP || pBB->α->t == IR_CALL || pBB->α->t == IR_GATHER)) {
        int rhs_off = bb_slot_get(pBB->α);
        if (rhs_off >= 0) {
            int voff = bb_varslot(pBB->sval);
            if (MEDIUM_BINARY) {
                /*   0    49 8B 84 24 <u32 rhs_off>     mov rax,[r12+rhs_off]   (read RHS producer slot lo)    */
                /*   8    49 89 84 24 <u32 voff>        mov [r12+voff],rax      (write var slot lo)            */
                /*  16    49 8B 84 24 <u32 rhs_off+8>   mov rax,[r12+rhs_off+8] (read RHS producer slot hi)    */
                /*  24    49 89 84 24 <u32 voff+8>      mov [r12+voff+8],rax    (write var slot hi)            */
                /*  32    E9 <rel32 → γ>                jmp γ                   ← γ patch at 33               */
                /*  37    E9 <rel32 → ω>                β: jmp ω                ← β-def 37, ω patch 38         */
                /*  42    end                                                                                 */
                bin = { {33, 37, 38}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
                return bytes(4, "\x49\x8B\x84\x24") + u32le((uint32_t)rhs_off)
                     + bytes(4, "\x49\x89\x84\x24") + u32le((uint32_t)voff)
                     + bytes(4, "\x49\x8B\x84\x24") + u32le((uint32_t)(rhs_off + 8))
                     + bytes(4, "\x49\x89\x84\x24") + u32le((uint32_t)(voff + 8))
                     + bytes(1, "\xE9") + u32le(0)
                     + bytes(1, "\xE9") + u32le(0);
            }
            if (MEDIUM_TEXT) {
                return s_1asm(emit_fmt("%s:", _.lbl_α))
                     + s_comment(emit_fmt("# BOX IR_ASSIGN store(\"%s\") [GZ-7 flat-chain RHS slot → var slot]", pBB->sval))
                     + s_2asm("mov", emit_fmt("rax, [r12+%d]", rhs_off))
                     + s_2asm("mov", emit_fmt("[r12+%d], rax", voff))
                     + s_2asm("mov", emit_fmt("rax, [r12+%d]", rhs_off + 8))
                     + s_2asm("mov", emit_fmt("[r12+%d], rax", voff + 8))
                     + s_2asm("jmp", _.lbl_γ)
                     + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
                     + s_2asm("jmp", _.lbl_ω);
            }
        }
    }
    IR_t *lhs = pBB ? pBB->α : NULL;
    if (!lhs || lhs->t != IR_VAR || !lhs->sval) {
        fprintf(stderr, "[IBB] FATAL bb_assign: lhs (pBB->α) must be IR_VAR with sval (got kind=%d sval=%s)\n",
                lhs ? (int)lhs->t : -1, (lhs && lhs->sval) ? lhs->sval : "(null)");
        abort();
    }
    const char *name = lhs->sval;
    if (MEDIUM_TEXT) {
        return s_1asm(emit_fmt("%s:", _.lbl_α))
             + s_comment(emit_fmt("# BOX IR_ASSIGN store(\"%s\") [IBB-7 rt_pop_nv_set]", name))
             + s_2asm("call",     "rt_pop_nv_set@PLT")
             + s_2asm("jmp",      _.lbl_γ)
             + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
             + s_2asm("jmp",      _.lbl_ω);
    }
    if (MEDIUM_BINARY) {
        uint64_t nptr = (uint64_t)(uintptr_t)name;
        uint64_t fptr; { void (*fp)(const char *) = rt_pop_nv_set; fptr = (uint64_t)(uintptr_t)(void*)fp; }
        bin = { {23, 27, 28}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
        return bytes(2, "\x48\xBF") + u64le(nptr)
             + bytes(2, "\x48\xB8") + u64le(fptr)
             + bytes(2, "\xFF\xD0")
             + bytes(1, "\xE9")     + u32le(0)
             + bytes(1, "\xE9")     + u32le(0);
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_assign(IR_t * pBB) { bb_bin_t bin; bb_emit_asm_result(bb_assign_str(pBB, bin), bin); }
