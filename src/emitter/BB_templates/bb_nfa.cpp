/* bb_nfa.cpp — RK-NFA-4 mode-4 templates for the ISOLATED BB_NFA_* family (GOAL-RAKU-BB). */
/* NOT shared with SNOBOL4's pattern opcodes — a Raku regex bug must never touch SNOBOL4's hot path. */
/* Opcode names derive 1:1 from Nfa_kind. This file currently lands the TRIVIAL passthrough nodes      */
/* (EPS / CAP_OPEN / CAP_CLOSE) that are pure `jmp γ` with no runtime helper and no backtracking — the */
/* exact bb_eps shape. The consuming/branching nodes (CHAR/ANY/CLASS/SPLIT/BOL/EOL/ACCEPT) carry the   */
/* register model + char/cset tests + the SPLIT γ/β backtrack and land in a follow-up step (they need  */
/* the pos/subject/slen register convention + capture block + cset rodata — see GOAL-RAKU-BB RK-NFA-4  */
/* DESIGN). Nothing invokes a BB_NFA_* graph yet (the `~~` path stays on the proven C matcher across    */
/* all three modes), so these are reached only once RK-NFA-4 rewires `~~`→SM_BB_INVOKE behind a flag.   */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Shared passthrough body: γ on out1, β→ω (no retry). Byte-identical to bb_eps — the EPS/CAP_* nodes  */
/* are pure epsilon joins in nfa_bt (tail to s->out1; captures record pos as a side effect handled by  */
/* the capture-block writer added in the follow-up step, not by control flow). */
static std::string bb_nfa_passthrough_str(BB_t * pBB, bb_bin_t & bin) {
    bin = {};
    (void)pBB;
    if (PLATFORM_X86) {
        bin = { {1, 5, 6}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
        return IF(MEDIUM_MACRO_DEF,
               s_comment("# no macro form — NFA passthrough"))
             + IF(MEDIUM_BINARY,
               bytes(1, "\xE9") + u32le(0)
                 + bytes(1, "\xE9") + u32le(0))
             + IF(MEDIUM_TEXT,
               s_1asm(emit_fmt("%s:", _.lbl_α))
               + s_comment("# BOX NFA passthrough (EPS/CAP)")
                 + s_2asm("jmp", _.lbl_γ)
                 + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω));
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_nfa_eps(BB_t * pBB) {
    bb_bin_t bin;
    bb_emit_asm_result(bb_nfa_passthrough_str(pBB, bin), bin);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_nfa_cap_open(BB_t * pBB) {
    bb_bin_t bin;
    bb_emit_asm_result(bb_nfa_passthrough_str(pBB, bin), bin);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_nfa_cap_close(BB_t * pBB) {
    bb_bin_t bin;
    bb_emit_asm_result(bb_nfa_passthrough_str(pBB, bin), bin);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* RK-NFA-4 G1-1 S3 (mode-4 TEXT): consuming + terminal leaves against the walker's register model.    */
/* The NFA walker (sm_bb_invoke.cpp, gated on gen->t in BB_NFA_*) owns the subject preamble + leftmost  */
/* sweep + reg save/restore and holds r13=pos, r14=subject base ptr, r15d=slen across the node chain    */
/* (all callee-saved, saved by the walker). These leaves emit only their own four-port body; γ/ω/β come */
/* from g_emit.lbl_* set per-node by the walker. MEDIUM_BINARY (mode-3 native) is deferred to RK-NFA-5  */
/* — mode-3 ~~ still runs via the proven C matcher + byname dispatch, so the BINARY arm is a comment.   */
/* BB_NFA_CHAR: match one literal char (pBB->ival) at pos; advance + γ on hit, ω on miss/end-of-input.  */
static std::string bb_nfa_char_str(BB_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (PLATFORM_X86) {
        return IF(MEDIUM_MACRO_DEF, s_comment("# no macro form — NFA CHAR"))
             + IF(MEDIUM_BINARY,    s_comment("# NFA CHAR mode-3 BINARY deferred to RK-NFA-5"))
             + IF(MEDIUM_TEXT,
               s_1asm(emit_fmt("%s:", _.lbl_α))
               + s_comment(emit_fmt("# BOX NFA_CHAR '%c' (%d)", (pBB->ival >= 32 && pBB->ival < 127) ? (int)pBB->ival : '?', (int)pBB->ival))
               + s_2asm("cmp", "r13d, r15d")
               + s_2asm("jae", _.lbl_ω)
               + s_2asm("movzx", "eax, byte ptr [r14 + r13]")
               + s_2asm("cmp", emit_fmt("eax, %d", (int)(pBB->ival & 0xff)))
               + s_2asm("jne", _.lbl_ω)
               + s_2asm("inc", "r13")
               + s_2asm("jmp", _.lbl_γ)
               + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω));
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_nfa_char(BB_t * pBB) {
    bb_bin_t bin;
    bb_emit_asm_result(bb_nfa_char_str(pBB, bin), bin);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* BB_NFA_ACCEPT: terminal success — γ is the walker's matched-epilogue label (which restores the saved */
/* regs, pushes the verdict, sets last_ok). Pure jmp γ; β→ω like the passthrough leaves.                */
static std::string bb_nfa_accept_str(BB_t * pBB, bb_bin_t & bin) {
    bin = {};
    (void)pBB;
    if (PLATFORM_X86) {
        return IF(MEDIUM_MACRO_DEF, s_comment("# no macro form — NFA ACCEPT"))
             + IF(MEDIUM_BINARY,    s_comment("# NFA ACCEPT mode-3 BINARY deferred to RK-NFA-5"))
             + IF(MEDIUM_TEXT,
               s_1asm(emit_fmt("%s:", _.lbl_α))
               + s_comment("# BOX NFA_ACCEPT")
               + s_2asm("jmp", _.lbl_γ)
               + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω));
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_nfa_accept(BB_t * pBB) {
    bb_bin_t bin;
    bb_emit_asm_result(bb_nfa_accept_str(pBB, bin), bin);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* BB_NFA_ANY: Raku `.` — match any char except '\n' at pos; advance + γ on hit, ω on end/newline.     */
static std::string bb_nfa_any_str(BB_t * pBB, bb_bin_t & bin) {
    bin = {};
    (void)pBB;
    if (PLATFORM_X86) {
        return IF(MEDIUM_MACRO_DEF, s_comment("# no macro form — NFA ANY"))
             + IF(MEDIUM_BINARY,    s_comment("# NFA ANY mode-3 BINARY deferred to RK-NFA-5"))
             + IF(MEDIUM_TEXT,
               s_1asm(emit_fmt("%s:", _.lbl_α))
               + s_comment("# BOX NFA_ANY '.'")
               + s_2asm("cmp", "r13d, r15d")
               + s_2asm("jae", _.lbl_ω)
               + s_2asm("movzx", "eax, byte ptr [r14 + r13]")
               + s_2asm("cmp", "eax, 10")               /* '\n' */
               + s_2asm("je", _.lbl_ω)
               + s_2asm("inc", "r13")
               + s_2asm("jmp", _.lbl_γ)
               + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω));
    }
    return std::string();
}
extern "C" void bb_nfa_any(BB_t * pBB) {
    bb_bin_t bin;
    bb_emit_asm_result(bb_nfa_any_str(pBB, bin), bin);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* BB_NFA_BOL: Raku `^` — zero-width; γ iff pos==0, else ω. No advance.                                */
static std::string bb_nfa_bol_str(BB_t * pBB, bb_bin_t & bin) {
    bin = {};
    (void)pBB;
    if (PLATFORM_X86) {
        return IF(MEDIUM_MACRO_DEF, s_comment("# no macro form — NFA BOL"))
             + IF(MEDIUM_BINARY,    s_comment("# NFA BOL mode-3 BINARY deferred to RK-NFA-5"))
             + IF(MEDIUM_TEXT,
               s_1asm(emit_fmt("%s:", _.lbl_α))
               + s_comment("# BOX NFA_BOL '^'")
               + s_2asm("test", "r13d, r13d")
               + s_2asm("jnz", _.lbl_ω)
               + s_2asm("jmp", _.lbl_γ)
               + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω));
    }
    return std::string();
}
extern "C" void bb_nfa_bol(BB_t * pBB) {
    bb_bin_t bin;
    bb_emit_asm_result(bb_nfa_bol_str(pBB, bin), bin);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* BB_NFA_EOL: Raku `$` — zero-width; γ iff pos==slen, else ω. No advance.                             */
static std::string bb_nfa_eol_str(BB_t * pBB, bb_bin_t & bin) {
    bin = {};
    (void)pBB;
    if (PLATFORM_X86) {
        return IF(MEDIUM_MACRO_DEF, s_comment("# no macro form — NFA EOL"))
             + IF(MEDIUM_BINARY,    s_comment("# NFA EOL mode-3 BINARY deferred to RK-NFA-5"))
             + IF(MEDIUM_TEXT,
               s_1asm(emit_fmt("%s:", _.lbl_α))
               + s_comment("# BOX NFA_EOL '$'")
               + s_2asm("cmp", "r13d, r15d")
               + s_2asm("jne", _.lbl_ω)
               + s_2asm("jmp", _.lbl_γ)
               + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω));
    }
    return std::string();
}
extern "C" void bb_nfa_eol(BB_t * pBB) {
    bb_bin_t bin;
    bb_emit_asm_result(bb_nfa_eol_str(pBB, bin), bin);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* BB_NFA_CLASS: match one char in a 32-byte cset bitset (pBB->sval, blob from raku_nfa_to_bb). γ on    */
/* hit + advance, ω on miss/end. The bitset is emitted INLINE as 32 .byte rodata (NOT a movabs of      */
/* pBB->sval — that pointer is valid only inside the compiler process; mode-4 TEXT assembles a SEPARATE */
/* native binary, so the cset must travel as emitted bytes). Membership test mirrors raku_cc_test:     */
/* byte = bits[c>>3], bit = c&7 → `bt edx, eax`. Scratch regs eax/ecx/edx only (caller-saved; the       */
/* walker's r12-r15 and callee-saved rbx untouched).                                                   */
static std::string bb_nfa_class_str(BB_t * pBB, bb_bin_t & bin) {
    bin = {};
    int id = bb_node_id(pBB);
    if (PLATFORM_X86) {
        std::string cs = emit_fmt(".Lnfacc%d", id);
        std::string blob;
        const unsigned char * bits = (const unsigned char *)pBB->sval;
        for (int i = 0; i < 32; i++)
            blob += s_directive(emit_fmt(".byte 0x%02x", bits ? (int)bits[i] : 0));
        return IF(MEDIUM_MACRO_DEF, s_comment("# no macro form — NFA CLASS"))
             + IF(MEDIUM_BINARY,    s_comment("# NFA CLASS mode-3 BINARY deferred to RK-NFA-5"))
             + IF(MEDIUM_TEXT,
               s_1asm(emit_fmt("%s:", _.lbl_α))
               + s_comment("# BOX NFA_CLASS [cset]")
               + s_directive(".section .rodata")
               + s_directive(cs + ":")
               + blob
               + s_directive(".section .text")
               + s_directive(".intel_syntax noprefix")
               + s_2asm("cmp", "r13d, r15d")
               + s_2asm("jae", _.lbl_ω)
               + s_2asm("movzx", "eax, byte ptr [r14 + r13]")
               + s_2asm("mov", "ecx, eax")
               + s_2asm("shr", "ecx, 3")
               + s_2asm("lea", emit_fmt("rdx, [rip + %s]", cs.c_str()))
               + s_2asm("movzx", "edx, byte ptr [rdx + rcx]")
               + s_2asm("and", "eax, 7")
               + s_2asm("bt", "edx, eax")
               + s_2asm("jnc", _.lbl_ω)
               + s_2asm("inc", "r13")
               + s_2asm("jmp", _.lbl_γ)
               + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω));
    }
    return std::string();
}
extern "C" void bb_nfa_class(BB_t * pBB) {
    bb_bin_t bin;
    bb_emit_asm_result(bb_nfa_class_str(pBB, bin), bin);
}
