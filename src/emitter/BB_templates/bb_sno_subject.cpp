/* bb_sno_subject.cpp — STACKLESS BB template for the SNOBOL4 pattern-match SUBJECT phase (IR_SUBJECT).
   PB-0 (GOAL-SNOBOL4-BB SESSION RUNG #0 SBL-PAT-BB, 2026-05-31, Claude Opus). Phase 1 of the five-phase
   `SUBJ ? PAT [= REPL]` native model (ARCH-SNOBOL4.md "Native pattern architecture — modes 3 & 4"): the
   SUBJECT box evaluates the subject value-expr and establishes the SCANNED WHOLE — Σ (base ptr) and Δ
   (length). SPITBOL Manual ch.18 (the pattern-match algorithm): the cursor "is set to zero when a pattern
   match begins", so the cursor δ is OWNED BY THE MATCHER (PB-2 BB_MATCH), not by this box; SUBJECT only
   loads the fixed whole (Σ) and bound (Δ).

   STACKLESS + ζ-FRAME (RULES one-register-frame): the box does NOT clobber the callee-saved subject
   registers (r13/r14/r15) — that convention is adopted at PB-2 when BB_MATCH consumes Σ/δ/Δ inside ONE
   sealed sequence (no C return between SUBJECT and MATCH, so the flat-sequence prologue can preserve them).
   For PB-0 the box stands alone and returns to C, so it stores Σ/Δ into a 16-byte ζ-frame slot
   ([r12+off] = Σ base, [r12+off+8] = Δ length) — ABI-safe (r12 is pushed/popped by the flat prologue) and
   the RULES-preferred RW location. The subject value is fetched by ONE runtime call (rt_sno_subject_load),
   exactly the bb_sno_assign register-passing pattern: NO value stack, NO ring.

   Operand (pBB->α):
     IR_VAR   sval = subject variable name  → rdi = &name, rsi = 0
     IR_LIT_S sval = subject literal string → rdi = 0,      rsi = &lit
   rt_sno_subject_load returns {base,len} in rax:rdx (SysV 16-byte two-INTEGER struct).

   mode-3 BINARY arm: full bytes (below). mode-4 TEXT arm: lea/call@PLT into libscrip_rt — SAME work, GAS
   text. Bounded single-shot (an established subject does not re-offer): β = jmp ω.

   Bytes (58 total, mode-3 BINARY):
       0  : 48 BF + u64le name_ptr   movabs rdi, name_ptr   (0 if literal subject)
       10 : 48 BE + u64le lit_ptr    movabs rsi, lit_ptr    (0 if variable subject)
       20 : 48 B8 + u64le fn_ptr     movabs rax, &rt_sno_subject_load
       30 : FF D0                    call rax
       32 : 49 89 84 24 + u32le off      mov [r12+off],   rax   (Σ base)
       40 : 49 89 94 24 + u32le off+8    mov [r12+off+8], rdx   (Δ length)
       48 : E9 + u32le γ_rel32       jmp γ              (rel32 patch at 49 -> lbl_γ)
       53 : E9 + u32le ω_rel32       β: jmp ω           (β-def at 53, rel32 patch at 54 -> lbl_ω)
       58 : end
*/
#include <string>
#include <string.h>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
typedef struct { const char *base; long len; } rt_subj_t;
rt_subj_t rt_sno_subject_load(const char *name, const char *lit);
int       bb_slot_alloc16(IR_t * nd);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_sno_subject_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (!PLATFORM_X86) return std::string();
    if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — SNO IR_SUBJECT");
    IR_t *op = pBB ? pBB->α : NULL;
    int is_var = op && op->t == IR_VAR   && op->sval;
    int is_lit = op && op->t == IR_LIT_S && op->sval;
    if (!pBB || (!is_var && !is_lit)) {
        if (MEDIUM_BINARY) {
            fprintf(stderr, "[SBB] FATAL bb_sno_subject: need IR_SUBJECT with alpha IR_VAR|IR_LIT_S sval (got alpha=%d)\n", op ? (int)op->t : -1);
            abort();
        }
        return bomb_text(emit_fmt("SNO IR_SUBJECT: need alpha IR_VAR|IR_LIT_S sval (got alpha=%d)", op ? (int)op->t : -1).c_str());
    }
    const char *name = is_var ? op->sval : NULL;
    const char *lit  = is_lit ? op->sval : NULL;
    int off = bb_slot_alloc16(pBB);
    if (MEDIUM_TEXT) {
        int id = g_flat_node_id++;
        std::string nlbl = emit_fmt(".Lsno_subjname_%d", id);
        std::string llbl = emit_fmt(".Lsno_subjlit_%d",  id);
        std::string rodata = s_directive(".section .rodata");
        if (is_var) rodata += s_L1asm(nlbl + ":", emit_fmt(".asciz \"%s\"", name));
        if (is_lit) rodata += s_L1asm(llbl + ":", emit_fmt(".asciz \"%s\"", lit));
        return rodata
             + s_directive(".section .text")
             + s_1asm(emit_fmt("%s:", _.lbl_α))
             + s_comment(emit_fmt("# BOX SNO IR_SUBJECT load Σ=[r12+%d] Δ=[r12+%d] [stackless ζ-frame, @PLT]", off, off + 8))
             + (is_var ? s_2asm("lea", emit_fmt("rdi, [rip + %s]", nlbl.c_str())) : s_2asm("xor", "edi, edi"))
             + (is_lit ? s_2asm("lea", emit_fmt("rsi, [rip + %s]", llbl.c_str())) : s_2asm("xor", "esi, esi"))
             + s_2asm("call", "rt_sno_subject_load@PLT")
             + s_2asm("mov",  emit_fmt("[r12 + %d], rax", off))
             + s_2asm("mov",  emit_fmt("[r12 + %d], rdx", off + 8))
             + s_2asm("jmp",  _.lbl_γ)
             + s_1asm(std::string(_.lbl_β) + ":")
             + s_2asm("jmp",  _.lbl_ω);
    }
    if (MEDIUM_BINARY) {
        uint64_t nptr = is_var ? (uint64_t)(uintptr_t)name : 0;
        uint64_t lptr = is_lit ? (uint64_t)(uintptr_t)lit  : 0;
        uint64_t fptr; { rt_subj_t (*fp)(const char *, const char *) = rt_sno_subject_load; fptr = (uint64_t)(uintptr_t)(void*)fp; }
        bin = { {49, 53, 54}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
        return bytes(2, "\x48\xBF")         + u64le(nptr)
             + bytes(2, "\x48\xBE")         + u64le(lptr)
             + bytes(2, "\x48\xB8")         + u64le(fptr)
             + bytes(2, "\xFF\xD0")
             + bytes(4, "\x49\x89\x84\x24") + u32le((uint32_t)off)
             + bytes(4, "\x49\x89\x94\x24") + u32le((uint32_t)(off + 8))
             + bytes(1, "\xE9")             + u32le(0)
             + bytes(1, "\xE9")             + u32le(0);
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_sno_subject(IR_t * pBB) { bb_bin_t bin; bb_emit_asm_result(bb_sno_subject_str(pBB, bin), bin); }
