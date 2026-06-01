/* bb_sno_assign.cpp — STACKLESS BB template for SNOBOL4 IR_ASSIGN of a literal-string rhs.
   SBL-M3-STACKLESS (2026-05-31, Opus 4.8). Lon directive: NO VALUE STACK — forbidden. So this box
   passes both operands in REGISTERS (SysV rdi/rsi) to one runtime call; nothing touches g_vstack /
   rt_push_* / rt_pop_*. The rhs string and the lhs name are READ-ONLY constants (their pointers are
   compile-time immediates for the in-process mode-3 LIVE box, mirroring how every BINARY template bakes
   RO data + fn pointers via movabs). The four-port shape is the proven 32-byte bb_assign trailer plus one
   extra movabs (rsi = str): no β-retry (an assign is bounded/single-shot), β = jmp ω.

   Reached via emit_core's IR_ASSIGN case, lang-guarded (IR_LANG_SNO + α==IR_LIT_S); the Icon arm
   (bb_assign, α==IR_VAR) is untouched. Driver = flat_drive_sno_assign (emit_bb.c), which sets pBB->α to
   the rhs literal (postfix-folded by sno_ring_to_tree) and keeps the target name in pBB->sval.

   Bytes (42 total):
       0  : 48 BF + u64le name_ptr      movabs rdi, name_ptr
       10 : 48 BE + u64le str_ptr       movabs rsi, str_ptr
       20 : 48 B8 + u64le fn_ptr        movabs rax, &rt_sno_assign_lit_s
       30 : FF D0                       call rax
       32 : E9 + u32le γ_rel32          jmp γ              (rel32 patch at 33 -> lbl_γ)
       37 : E9 + u32le ω_rel32          β: jmp ω           (β-def at 37, rel32 patch at 38 -> lbl_ω)
       42 : end
   mode-4 TEXT arm: bomb (—compile/x86 emission is excised this era; honest stub, RULES-OK).
*/
#include <string>
#include <string.h>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
void rt_sno_assign_lit_s(const char *name, const char *str);
void rt_sno_assign_int(const char *name, int64_t val);
void rt_sno_assign_var(const char *dst, const char *src);
int  rt_sno_assign_concat(const char *name, void *left_graph, void *right_graph);
int  bb_slot_get(IR_t * nd);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* SBL-M3-ARITH (2026-05-31): SNOBOL4 `name = <int binop>` assign. The rhs is the GZ-3 stackless RO-int binop box, which has already stored its raw int64 result into a ζ-frame slot [r12+off]; recover off via bb_slot_get(rhs).  */
/* This box reads the int64 by value into rsi and the target name (RO immediate / .L label) into rdi, then calls rt_sno_assign_int (builds a DT_I DESCR + NV_SET_fn — OUTPUT prints via the same hook as lit_s). NO value stack.   */
static std::string bb_sno_assign_int_str(IR_t * pBB, IR_t * rhs, bb_bin_t & bin) {
    int off = bb_slot_get(rhs);
    if (off < 0) {
        if (MEDIUM_BINARY) {
            fprintf(stderr, "[SBB] FATAL bb_sno_assign(binop): result slot not allocated (bb_slot_get miss) for '%s'\n", pBB->sval ? pBB->sval : "(null)");
            abort();
        }
        return bomb_text(emit_fmt("SNO IR_ASSIGN(binop): result slot not allocated for '%s'", pBB->sval ? pBB->sval : "(null)").c_str());
    }
    const char *name = pBB->sval;
    if (MEDIUM_TEXT) {
        int id = g_flat_node_id++;
        std::string nlbl = emit_fmt(".Lsno_iname_%d", id);
        return s_directive(".section .rodata")
             + s_L1asm(nlbl + ":", emit_fmt(".asciz \"%s\"", name))
             + s_directive(".section .text")
             + s_1asm(emit_fmt("%s:", _.lbl_α))
             + s_comment(emit_fmt("# BOX SNO IR_ASSIGN(int binop) store(\"%s\") = [r12+%d] [stackless, @PLT]", name, off))
             + s_2asm("mov",  emit_fmt("rsi, [r12 + %d]", off))
             + s_2asm("lea",  emit_fmt("rdi, [rip + %s]", nlbl.c_str()))
             + s_2asm("call", "rt_sno_assign_int@PLT")
             + s_2asm("jmp",  _.lbl_γ)
             + s_1asm(std::string(_.lbl_β) + ":")
             + s_2asm("jmp",  _.lbl_ω);
    }
    if (MEDIUM_BINARY) {
        uint64_t nptr = (uint64_t)(uintptr_t)name;
        uint64_t fptr; { void (*fp)(const char *, int64_t) = rt_sno_assign_int; fptr = (uint64_t)(uintptr_t)(void*)fp; }
        /*   0  : 49 8B B4 24 <u32 off>   mov rsi, [r12+off]   (read the binop's int64 result from the ζ slot)*/
        /*   8  : 48 BF + u64le name_ptr  movabs rdi, name_ptr                                                */
        /*  18  : 48 B8 + u64le fn_ptr    movabs rax, &rt_sno_assign_int                                      */
        /*  28  : FF D0                   call rax                                                            */
        /*  30  : E9 + u32le γ_rel32      jmp γ               ← γ patch at 31                                 */
        /*  35  : E9 + u32le ω_rel32      β: jmp ω            ← β-def 35, ω patch 36                          */
        /*  40  : end                                                                                         */
        bin = { {31, 35, 36}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
        return bytes(4, "\x49\x8B\xB4\x24") + u32le((uint32_t)off)
             + bytes(2, "\x48\xBF")         + u64le(nptr)
             + bytes(2, "\x48\xB8")         + u64le(fptr)
             + bytes(2, "\xFF\xD0")
             + bytes(1, "\xE9")             + u32le(0)
             + bytes(1, "\xE9")             + u32le(0);
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* SBL-M3-VAR (2026-05-31): SNOBOL4 `dst = src` where the rhs is a bare variable read (`OUTPUT = S`). Stackless: dst name in rdi, src name in rsi, both RO immediates (BINARY) / `.L` labels via lea (TEXT), one call to       */
/* rt_sno_assign_var (NV_SET(dst, NV_GET(src))). NO value stack. Same four-port 42-byte shape as the lit_s arm (two name pointers instead of name+str). Bounded single-shot: β = jmp ω.                                        */
static std::string bb_sno_assign_var_str(IR_t * pBB, bb_bin_t & bin) {
    IR_t *rhs = pBB ? pBB->α : NULL;
    if (!pBB || !pBB->sval || !rhs || rhs->t != IR_VAR || !rhs->sval) {
        if (MEDIUM_BINARY) {
            fprintf(stderr, "[SBB] FATAL bb_sno_assign(var): need IR_ASSIGN sval(dst) + alpha IR_VAR sval(src) (got dst=%s alpha=%d)\n",
                    (pBB && pBB->sval) ? pBB->sval : "(null)", rhs ? (int)rhs->t : -1);
            abort();
        }
        return bomb_text(emit_fmt("SNO IR_ASSIGN(var): need sval(dst)+alpha IR_VAR sval(src) (got dst=%s alpha=%d)",
                                  (pBB && pBB->sval) ? pBB->sval : "(null)", rhs ? (int)rhs->t : -1).c_str());
    }
    const char *dst = pBB->sval;
    const char *src = rhs->sval;
    if (MEDIUM_TEXT) {
        int id = g_flat_node_id++;
        std::string dlbl = emit_fmt(".Lsno_dst_%d", id);
        std::string slbl = emit_fmt(".Lsno_src_%d", id);
        return s_directive(".section .rodata")
             + s_L1asm(dlbl + ":", emit_fmt(".asciz \"%s\"", dst))
             + s_L1asm(slbl + ":", emit_fmt(".asciz \"%s\"", src))
             + s_directive(".section .text")
             + s_1asm(emit_fmt("%s:", _.lbl_α))
             + s_comment(emit_fmt("# BOX SNO IR_ASSIGN(var) store(\"%s\") = read(\"%s\") [stackless, @PLT]", dst, src))
             + s_2asm("lea",  emit_fmt("rdi, [rip + %s]", dlbl.c_str()))
             + s_2asm("lea",  emit_fmt("rsi, [rip + %s]", slbl.c_str()))
             + s_2asm("call", "rt_sno_assign_var@PLT")
             + s_2asm("jmp",  _.lbl_γ)
             + s_1asm(std::string(_.lbl_β) + ":")
             + s_2asm("jmp",  _.lbl_ω);
    }
    if (MEDIUM_BINARY) {
        uint64_t dptr = (uint64_t)(uintptr_t)dst;
        uint64_t sptr = (uint64_t)(uintptr_t)src;
        uint64_t fptr; { void (*fp)(const char *, const char *) = rt_sno_assign_var; fptr = (uint64_t)(uintptr_t)(void*)fp; }
        bin = { {33, 37, 38}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
        return bytes(2, "\x48\xBF") + u64le(dptr)
             + bytes(2, "\x48\xBE") + u64le(sptr)
             + bytes(2, "\x48\xB8") + u64le(fptr)
             + bytes(2, "\xFF\xD0")
             + bytes(1, "\xE9")     + u32le(0)
             + bytes(1, "\xE9")     + u32le(0);
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* SBL-M3-CONCAT (2026-05-31): SNOBOL4 `dst = A B` (whitespace concatenation). The rhs IR_SEQ concat node (dval=1.0) carries its two operands as isolated IR_graph_t sub-graphs (left on counter, right on ival). The box  */
/* bakes dst name + both sub-graph pointers (process-valid in mode-3) and calls rt_sno_assign_concat, which runs each sub-graph via bb_exec_once and stores the concatenation. Tests the result: 1 -> γ, 0 -> ω. NO vstack. */
static std::string bb_sno_assign_concat_str(IR_t * pBB, bb_bin_t & bin) {
    IR_t *seq = pBB ? pBB->α : NULL;
    if (!pBB || !pBB->sval || !seq || !(seq->t == IR_SEQ || seq->t == IR_SEQ_EXPR) || seq->dval != 1.0 || !seq->counter || !seq->ival) {
        if (MEDIUM_BINARY) {
            fprintf(stderr, "[SBB] FATAL bb_sno_assign(concat): need sval(dst)+alpha SNO-concat IR_SEQ with both operand sub-graphs (dst=%s alpha=%d)\n",
                    (pBB && pBB->sval) ? pBB->sval : "(null)", seq ? (int)seq->t : -1);
            abort();
        }
        return bomb_text("SNO IR_ASSIGN(concat): need alpha SNO-concat IR_SEQ with both operand sub-graphs");
    }
    const char *name = pBB->sval;
    if (MEDIUM_TEXT) {
        return bomb_text("SNO IR_ASSIGN(concat): mode-4 emission pending (operand sub-graphs are process-local); use --run for mode-3");
    }
    if (MEDIUM_BINARY) {
        uint64_t nptr = (uint64_t)(uintptr_t)name;
        uint64_t lptr = (uint64_t)(uintptr_t)(intptr_t)seq->counter;
        uint64_t rptr = (uint64_t)(uintptr_t)(intptr_t)seq->ival;
        uint64_t fptr; { int (*fp)(const char *, void *, void *) = rt_sno_assign_concat; fptr = (uint64_t)(uintptr_t)(void*)fp; }
        bin = { {46, 51, 55, 56}, {_.lbl_ω_p, _.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, false, true, false} };
        return bytes(2, "\x48\xBF") + u64le(nptr)
             + bytes(2, "\x48\xBE") + u64le(lptr)
             + bytes(2, "\x48\xBA") + u64le(rptr)
             + bytes(2, "\x48\xB8") + u64le(fptr)
             + bytes(2, "\xFF\xD0")
             + bytes(2, "\x85\xC0")
             + bytes(2, "\x0F\x84") + u32le(0)
             + bytes(1, "\xE9")     + u32le(0)
             + bytes(1, "\xE9")     + u32le(0);
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_sno_assign_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (!PLATFORM_X86) return std::string();
    if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — SNO IR_ASSIGN(lit_s)");
    if (pBB && pBB->sval && pBB->α && pBB->α->t == IR_BINOP) return bb_sno_assign_int_str(pBB, pBB->α, bin);
    if (pBB && pBB->sval && pBB->α && pBB->α->t == IR_VAR)   return bb_sno_assign_var_str(pBB, bin);
    if (pBB && pBB->sval && pBB->α && (pBB->α->t == IR_SEQ || pBB->α->t == IR_SEQ_EXPR)) return bb_sno_assign_concat_str(pBB, bin);
    if (MEDIUM_TEXT) {
        /* SBL-M4-STACKLESS (2026-05-31, Opus 4.8): mode-4 GAS arm — same four-port stackless shape as the
           BINARY arm, but the name/str are RO `.L` labels reached via `lea [rip+label]` (PIC-safe) and the
           store is `call rt_sno_assign_lit_s@PLT`. NO value stack. The two .asciz strings live in a
           self-contained `.section .rodata` block emitted just before the box's α-body, then `.section .text`
           restores the code section so the next box lands in .text. Unique labels keyed on flat node id. */
        IR_t *rhs = pBB ? pBB->α : NULL;
        if (!pBB || !pBB->sval || !rhs || rhs->t != IR_LIT_S || !rhs->sval) {
            return bomb_text(emit_fmt("SNO IR_ASSIGN(lit_s): need sval(name)+alpha IR_LIT_S sval(str) (got name=%s alpha=%d)",
                                      (pBB && pBB->sval) ? pBB->sval : "(null)", rhs ? (int)rhs->t : -1).c_str());
        }
        const char *name = pBB->sval;
        const char *str  = rhs->sval;
        int id = g_flat_node_id++;
        std::string nlbl = emit_fmt(".Lsno_name_%d", id);
        std::string slbl = emit_fmt(".Lsno_str_%d",  id);
        return s_directive(".section .rodata")
             + s_L1asm(nlbl + ":", emit_fmt(".asciz \"%s\"", name))
             + s_L1asm(slbl + ":", emit_fmt(".asciz \"%s\"", str))
             + s_directive(".section .text")
             + s_1asm(emit_fmt("%s:", _.lbl_α))
             + s_comment(emit_fmt("# BOX SNO IR_ASSIGN(lit_s) store(\"%s\") = '%s' [stackless, @PLT]", name, str))
             + s_2asm("lea",  emit_fmt("rdi, [rip + %s]", nlbl.c_str()))
             + s_2asm("lea",  emit_fmt("rsi, [rip + %s]", slbl.c_str()))
             + s_2asm("call", "rt_sno_assign_lit_s@PLT")
             + s_2asm("jmp",  _.lbl_γ)
             + s_1asm(std::string(_.lbl_β) + ":")
             + s_2asm("jmp",  _.lbl_ω);
    }
    if (MEDIUM_BINARY) {
        IR_t *rhs = pBB ? pBB->α : NULL;
        if (!pBB || !pBB->sval || !rhs || rhs->t != IR_LIT_S || !rhs->sval) {
            fprintf(stderr, "[SBB] FATAL bb_sno_assign: need IR_ASSIGN sval(name) + alpha IR_LIT_S sval(str) "
                            "(got name=%s alpha=%d)\n",
                    (pBB && pBB->sval) ? pBB->sval : "(null)", rhs ? (int)rhs->t : -1);
            abort();
        }
        const char *name = pBB->sval;
        const char *str  = rhs->sval;
        uint64_t nptr = (uint64_t)(uintptr_t)name;
        uint64_t sptr = (uint64_t)(uintptr_t)str;
        uint64_t fptr; { void (*fp)(const char *, const char *) = rt_sno_assign_lit_s; fptr = (uint64_t)(uintptr_t)(void*)fp; }
        bin = { {33, 37, 38}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
        return bytes(2, "\x48\xBF") + u64le(nptr)
             + bytes(2, "\x48\xBE") + u64le(sptr)
             + bytes(2, "\x48\xB8") + u64le(fptr)
             + bytes(2, "\xFF\xD0")
             + bytes(1, "\xE9")     + u32le(0)
             + bytes(1, "\xE9")     + u32le(0);
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_sno_assign(IR_t * pBB) { bb_bin_t bin; bb_emit_asm_result(bb_sno_assign_str(pBB, bin), bin); }
