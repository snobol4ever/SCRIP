/* bb_sno_pat_build_lit.cpp — STACKLESS BB template for the SNOBOL4 pattern-match PATTERN-BUILDER phase,
   literal first (IR_PAT_BUILD_LIT). PB-1 (GOAL-SNOBOL4-BB SESSION RUNG #0 SBL-PAT-BB, 2026-05-31, Claude
   Opus 4.8). Phase 2 of the five-phase `SUBJ ? PAT [= REPL]` native model (ARCH-SNOBOL4.md "Native pattern
   architecture — modes 3 & 4"): a SNOBOL4 pattern is a runtime byrd-box GRAPH, and "build pattern" lowers to
   BUILDER BBs that build OTHER BBs dynamically. This box is the literal builder — the construction-protocol
   model the rest of PB-3 (CAT/ALT/LEN/SPAN/...) reuses.

   WHAT IT BUILDS: a runtime pattern node (PATND_t) via the value-stack-FREE rt_sno_pat_build_lit (which wraps
   the proven pat_lit constructor in core/pattern.c). It is NOT the matcher-leaf IR_PAT_LIT the mode-2 oracle
   consumes (that stays intact → zero regression); it is the BUILDER whose runtime EFFECT constructs the node
   the PB-2 BB_MATCH box will scan. SPITBOL Manual ch.18: a literal (`'BLUE'`) is a known component; the built
   node carries its bytes, and the matcher advances the cursor δ past them on a successful match.

   STACKLESS + ζ-FRAME (RULES one-register-frame + PER-BOX LOCAL STORAGE FACT RULE): the literal bytes are the
   box's READ-ONLY local (baked adjacent to the BLOB, reached `[rip+disp]` in TEXT / movabs immediate in
   BINARY — RO data, never on a stack). The built pattern-graph HEAD pointer is the box's READ-WRITE local: it
   lands in ONE 8-byte ζ-frame slot `[ζ=r12+off]` (allocated at emit time via bb_slot_alloc; r12 is preserved
   by the flat prologue). NO value stack (NOT the deleted rt_pat_lit vstack wrapper), NO ring, NO name-table
   round-trip — exactly the bb_sno_subject register-passing shape (load operand → ONE runtime call → store
   result to [ζ+off]). The PB-2 BB_MATCH box recovers the head by this box's frame offset (bb_slot_get).

   Operand: pBB->sval = the pattern literal string (baked RO).
   rt_sno_pat_build_lit(const char *s) returns the built PATND_t* head in rax.

   mode-3 BINARY arm: full bytes (below). mode-4 TEXT arm: lea/call@PLT into libscrip_rt — SAME work, GAS
   text. Bounded single-shot (building the literal once is enough; on backtrack the builder fails): β = jmp ω.

   Bytes (40 total, mode-3 BINARY):
       0  : 48 BF + u64le lit_ptr    movabs rdi, lit_ptr   (&pattern literal, RO)
       10 : 48 B8 + u64le fn_ptr     movabs rax, &rt_sno_pat_build_lit
       20 : FF D0                    call rax
       22 : 49 89 84 24 + u32le off  mov [r12+off], rax     (built PATND_t* head -> ζ slot)
       30 : E9 + u32le γ_rel32       jmp γ              (rel32 patch at 31 -> lbl_γ)
       35 : E9 + u32le ω_rel32       β: jmp ω           (β-def at 35, rel32 patch at 36 -> lbl_ω)
       40 : end
*/
#include <string>
#include <string.h>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
void *rt_sno_pat_build_lit(const char *s);
int   bb_slot_alloc(IR_t * nd);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_sno_pat_build_lit_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (!PLATFORM_X86) return std::string();
    if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — SNO IR_PAT_BUILD_LIT");
    const char *lit = pBB ? pBB->sval : NULL;
    if (!pBB || !lit) {
        if (MEDIUM_BINARY) {
            fprintf(stderr, "[SBB] FATAL bb_sno_pat_build_lit: need IR_PAT_BUILD_LIT with sval (pattern literal)\n");
            abort();
        }
        return bomb_text("SNO IR_PAT_BUILD_LIT: need sval (pattern literal)");
    }
    int off = bb_slot_alloc(pBB);
    if (MEDIUM_TEXT) {
        int id = g_flat_node_id++;
        std::string llbl = emit_fmt(".Lsno_patlit_%d", id);
        std::string rodata = s_directive(".section .rodata")
                           + s_L1asm(llbl + ":", emit_fmt(".asciz \"%s\"", lit));
        return rodata
             + s_directive(".section .text")
             + s_1asm(emit_fmt("%s:", _.lbl_α))
             + s_comment(emit_fmt("# BOX SNO IR_PAT_BUILD_LIT build LIT -> head=[r12+%d] [stackless ζ-frame, @PLT]", off))
             + s_2asm("lea",  emit_fmt("rdi, [rip + %s]", llbl.c_str()))
             + s_2asm("call", "rt_sno_pat_build_lit@PLT")
             + s_2asm("mov",  emit_fmt("[r12 + %d], rax", off))
             + s_2asm("jmp",  _.lbl_γ)
             + s_1asm(std::string(_.lbl_β) + ":")
             + s_2asm("jmp",  _.lbl_ω);
    }
    if (MEDIUM_BINARY) {
        uint64_t lptr = (uint64_t)(uintptr_t)lit;
        uint64_t fptr; { void *(*fp)(const char *) = rt_sno_pat_build_lit; fptr = (uint64_t)(uintptr_t)(void*)fp; }
        bin = { {31, 35, 36}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
        return bytes(2, "\x48\xBF")         + u64le(lptr)
             + bytes(2, "\x48\xB8")         + u64le(fptr)
             + bytes(2, "\xFF\xD0")
             + bytes(4, "\x49\x89\x84\x24") + u32le((uint32_t)off)
             + bytes(1, "\xE9")             + u32le(0)
             + bytes(1, "\xE9")             + u32le(0);
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_sno_pat_build_lit(IR_t * pBB) { bb_bin_t bin; bb_emit_asm_result(bb_sno_pat_build_lit_str(pBB, bin), bin); }
