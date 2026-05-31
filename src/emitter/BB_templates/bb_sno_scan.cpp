/* bb_sno_scan.cpp — STACKLESS BB template for a SNOBOL4 pattern-match statement (IR_SCAN).
   SBL-M3-SCAN (2026-05-31, Opus 4.8). Lon directive: NO VALUE STACK. The box passes all operands in
   REGISTERS (SysV rdi/rsi/rdx/rcx/r8) to ONE runtime call rt_sno_exec_scan(subj_name, subj_lit, has_repl,
   repl_str, pat_graph) which drives the pattern sub-graph through the proven 19-arm IR_PAT_* oracle
   (bb_exec_once) with anchored start-iteration + deferred-capture flush + the replacement splice — the
   SAME semantics as the mode-2 IR_SCAN arm. Returns 1 on match → box jmps γ; 0 on failure → box jmps ω.

   Operands (folded onto the IR_SCAN node by sno_ring_to_tree):
     pBB->sval    = subject VARIABLE name (replacement form; SPITBOL ch.6 requires a variable for `=`).
     pBB->ival    = has_repl (1 = `SUBJECT PATTERN = REPLACEMENT`, 0 = plain match).
     pBB->α       = repl form: the replacement value-node (IR_LIT_S → repl string baked RO);
                    plain form: the subject value-node (IR_LIT_S literal → subj_lit, or IR_VAR → subj_name).
     pBB->counter = the pattern sub-graph (IR_graph_t*) — a PROCESS-VALID pointer in mode-3, baked as imm64.

   mode-3 BINARY arm: full bytes (below). mode-4 TEXT arm: the pat-graph pointer is from the COMPILER
   process and is meaningless in a separately-linked binary, so the TEXT arm bombs honestly (the brokered
   pattern-blob path is the future mode-4 route). RULES-OK: an honest bomb_text, never a silent wrong emit.

   Bytes (75 total, mode-3 BINARY):
       0  : 48 BF + u64le subj_name   movabs rdi, subj_name_ptr   (0 if plain-literal subject)
       10 : 48 BE + u64le subj_lit    movabs rsi, subj_lit_ptr    (0 if variable subject)
       20 : BA    + u32le has_repl    mov    edx, has_repl
       25 : 48 B9 + u64le repl_ptr    movabs rcx, repl_str_ptr    (0 if no replacement)
       35 : 49 B8 + u64le pat_ptr     movabs r8,  pat_graph_ptr
       45 : 48 B8 + u64le fn_ptr      movabs rax, &rt_sno_exec_scan
       55 : FF D0                     call rax
       57 : 85 C0                     test eax, eax
       59 : 0F 84 + u32le ω_rel32     jz ω           (rel32 patch at 61 → lbl_ω)
       65 : E9    + u32le γ_rel32     jmp γ          (rel32 patch at 66 → lbl_γ)
       70 : E9    + u32le ω_rel32     β: jmp ω       (β-def at 70, rel32 patch at 71 → lbl_ω)
       75 : end
*/
#include <string>
#include <string.h>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
int rt_sno_exec_scan(const char *subj_name, const char *subj_lit, int has_repl, const char *repl_str, void *pat_graph);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_sno_scan_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (!PLATFORM_X86) return std::string();
    if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — SNO IR_SCAN");
    if (!pBB) {
        if (MEDIUM_BINARY) { fprintf(stderr, "[SBB] FATAL bb_sno_scan: null node\n"); abort(); }
        return bomb_text("SNO IR_SCAN: null node");
    }
    int has_repl = (pBB->ival != 0) ? 1 : 0;
    const char *subj_name = NULL;
    const char *subj_lit  = NULL;
    const char *repl_str  = NULL;
    void       *pat_graph = (void *)(intptr_t)pBB->counter;
    IR_t *operand = pBB->α;
    if (has_repl) {
        subj_name = pBB->sval;
        repl_str  = (operand && operand->sval) ? operand->sval : "";
        if (!subj_name) {
            if (MEDIUM_BINARY) { fprintf(stderr, "[SBB] FATAL bb_sno_scan(repl): missing subject variable name (sval)\n"); abort(); }
            return bomb_text("SNO IR_SCAN(repl): missing subject variable name");
        }
        if (operand && operand->t != IR_LIT_S) {
            if (MEDIUM_BINARY) { fprintf(stderr, "[SBB] FATAL bb_sno_scan(repl): non-literal replacement (kind=%d) not yet wired\n", (int)operand->t); abort(); }
            return bomb_text("SNO IR_SCAN(repl): non-literal replacement not yet wired");
        }
    } else {
        if (operand && operand->t == IR_LIT_S)      subj_lit  = operand->sval ? operand->sval : "";
        else if (operand && operand->t == IR_VAR)   subj_name = operand->sval;
        else {
            if (MEDIUM_BINARY) { fprintf(stderr, "[SBB] FATAL bb_sno_scan(plain): subject kind=%d not yet wired (literal/var only)\n", operand ? (int)operand->t : -1); abort(); }
            return bomb_text("SNO IR_SCAN(plain): subject must be literal or variable");
        }
    }
    if (!pat_graph) {
        if (MEDIUM_BINARY) { fprintf(stderr, "[SBB] FATAL bb_sno_scan: missing pattern sub-graph (counter)\n"); abort(); }
        return bomb_text("SNO IR_SCAN: missing pattern sub-graph");
    }
    if (MEDIUM_TEXT) {
        /* mode-4: the pattern sub-graph pointer is not valid in a separately-linked binary. Honest bomb;
           the brokered pattern-blob emission is the future route (SBL-M4-SCAN). */
        return bomb_text("SNO IR_SCAN: mode-4 pattern emission pending (brokered pattern blob); use --run for mode-3");
    }
    if (MEDIUM_BINARY) {
        uint64_t snptr = (uint64_t)(uintptr_t)subj_name;
        uint64_t slptr = (uint64_t)(uintptr_t)subj_lit;
        uint64_t rptr  = (uint64_t)(uintptr_t)repl_str;
        uint64_t pptr  = (uint64_t)(uintptr_t)pat_graph;
        uint64_t fptr; { int (*fp)(const char *, const char *, int, const char *, void *) = rt_sno_exec_scan; fptr = (uint64_t)(uintptr_t)(void*)fp; }
        bin = { {61, 66, 70, 71}, {_.lbl_ω_p, _.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, false, true, false} };
        return bytes(2, "\x48\xBF") + u64le(snptr)
             + bytes(2, "\x48\xBE") + u64le(slptr)
             + bytes(1, "\xBA")     + u32le((uint32_t)has_repl)
             + bytes(2, "\x48\xB9") + u64le(rptr)
             + bytes(2, "\x49\xB8") + u64le(pptr)
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
extern "C" void bb_sno_scan(IR_t * pBB) { bb_bin_t bin; bb_emit_asm_result(bb_sno_scan_str(pBB, bin), bin); }
