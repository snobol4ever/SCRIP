/* bb_seq_gather.cpp — BB box: IR_SEQ Raku gather multi-yield driver (SUSPEND children).
   RK-HY-1 de-cram (2026-06-02, Raku). Split out of bb_seq.cpp (one-box-one-file per the BB-HYGIENE
   FACT RULE DUP FORM 4). Reached when the IR_SEQ body is a chain of IR_SUSPEND nodes (one per take()
   in a Raku gather block); the generator PUMP wrapping this SEQ pumps it: α drives stmt0, β resumes
   whichever stmt last yielded, ω terminates when drained. A single β-entry has to know which child
   yielded last, so a resume_addr quad in .data (TEXT) / a malloc'd slot (BINARY) holds the address to
   jmp to on next β (init 0 = fresh → child 0 α); each child's γ-out is wrapped by a fixup that writes
   resume_addr := next_child_α (or the seq-done trampoline for the last yielder) BEFORE jumping to the
   outer γ; the outer β-entry loads resume_addr and indirect-jumps to it.

   NOTE (RK-EMIT-GATHER, 2026-06-01): the PRIMARY Raku gather path is now IR_GATHER -> bb_rk_gather.cpp
   (native m2/m3/m4). This IR_SEQ/IR_SUSPEND driver is the legacy SM-era gather mechanism, retained for
   any IR_SEQ that still carries IR_SUSPEND children; behavior is preserved byte-identical to the
   pre-de-cram bb_seq.cpp so nothing regresses.

   TWO emitters:
     bb_seq_gather_str(pBB,bin)   MEDIUM_TEXT driver (string-returning; "" if not n>0 + has_suspend).
     bb_seq_gather_binary(pBB)    MEDIUM_BINARY raw in-place emitter for the SM_BB_INVOKE -> walk_bb_node
                                  path where g_emit.xa_bb_emit_pair_*[] is UNPOPULATED; returns 1 if it
                                  handled emission, 0 to fall through. The TEXT .data resume quad becomes
                                  a malloc'd slot (the scratch page has no .data section); intermediate
                                  port labels are malloc'd so their pointers survive into the wrapper's
                                  bb_emit_end (which resolves all patches after this returns); the wrapper-
                                  owned outer ports γ/ω/β are referenced via _.lbl_*_p (stack-stable).

   FACT RULE: TEXT bytes via s_*; BINARY bytes via bb_emit_byte/bb_emit_u64/bb_emit_patch_rel32 — no
   seg_byte/SL_B/sl_emit_one/emit_standard_blob. PEERS: no IR_t fields added. */
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <vector>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Count the γ-chain length rooted at α. */
static int seq_chain_len_ga(IR_t * head) {
    int n = 0;
    for (IR_t * c = head; c; c = c->γ) n++;
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Emit one child's four-port x86 inline with caller-chosen labels; returns the box's emitted TEXT. */
static std::string emit_child_box_ga(IR_t * child,
                                     bb_label_t * la, bb_label_t * lg,
                                     bb_label_t * lo, bb_label_t * lb) {
    g_emit.lbl_α = la->name; g_emit.lbl_α_p = la;
    g_emit.lbl_γ = lg->name; g_emit.lbl_γ_p = lg;
    g_emit.lbl_ω = lo->name; g_emit.lbl_ω_p = lo;
    g_emit.lbl_β = lb->name; g_emit.lbl_β_p = lb;
    char * c = walk_bb_node_str_c(child);
    std::string s = c ? c : "";
    if (c) free(c);
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_seq_gather_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (!PLATFORM_X86) return std::string();
    if (!MEDIUM_TEXT)  return std::string();
    int id = bb_node_id(pBB);
    int n  = seq_chain_len_ga(pBB->α);
    if (n == 0) return std::string();
    int has_suspend = 0;
    for (IR_t * c = pBB->α; c; c = c->γ) if (c->t == IR_SUSPEND) { has_suspend = 1; break; }
    if (!has_suspend) return std::string();

    const char * outer_α = _.lbl_α;
    const char * outer_γ = _.lbl_γ;
    const char * outer_ω = _.lbl_ω;
    const char * outer_β = _.lbl_β;
    bb_label_t * outer_γ_p = _.lbl_γ_p;
    bb_label_t * outer_ω_p = _.lbl_ω_p;

    std::vector<bb_label_t> Lα(n), Lγ(n), Lω(n), Lβ(n);
    for (int k = 0; k < n; k++) {
        emit_label_initf(&Lα[k], ".Lseq%d_s%d_α", id, k);
        emit_label_initf(&Lγ[k], ".Lseq%d_s%d_γ", id, k);
        emit_label_initf(&Lω[k], ".Lseq%d_s%d_ω", id, k);
        emit_label_initf(&Lβ[k], ".Lseq%d_s%d_β", id, k);
    }

    std::string resume_slot = emit_fmt(".Lseq%d_resume", id);
    std::string seq_α       = std::string(outer_α);
    std::string seq_β       = std::string(outer_β);

    std::string head =
          s_directive(".section .data")
        + s_directive(resume_slot + ": .quad 0")
        + s_directive(".section .text")
        + s_directive(".intel_syntax noprefix")
        + s_1asm(seq_α + ":")
        + s_comment(emit_fmt("# BOX IR_SEQ(n=%d, gather multi-yield driver)", n))
        + s_1asm(emit_fmt("jmp .Lseq%d_s0_α", id))
        + s_L1asm(seq_β + ":", "")
        + s_comment("# β-resume: indirect-jmp to resume_addr (set by last yielder)")
        + s_2asm("lea rax,", "[rip + " + resume_slot + "]")
        + s_2asm("mov rax,", "[rax]")
        + s_2asm("jmp",      "rax");

    std::string body;
    IR_t * cur = pBB->α;
    for (int k = 0; k < n && cur; k++, cur = cur->γ) {
        bb_label_t * child_ω_target = (k + 1 < n) ? &Lα[k + 1] : outer_ω_p;
        body += emit_child_box_ga(cur, &Lα[k], &Lγ[k], child_ω_target, &Lβ[k]);
        std::string nxt_addr = (k + 1 < n) ? std::string(Lα[k + 1].name) : emit_fmt(".Lseq%d_done", id);
        body += s_L1asm(std::string(Lγ[k].name) + ":", "")
              + s_comment(emit_fmt("# fixup: set resume_addr = %s ; then γ-out", nxt_addr.c_str()))
              + s_2asm("lea rax,", "[rip + " + resume_slot + "]")
              + s_2asm("lea rcx,", "[rip + " + nxt_addr + "]")
              + s_2asm("mov [rax],", "rcx")
              + s_1asm(std::string("jmp ") + outer_γ);
    }

    std::string tail =
          s_L1asm(emit_fmt(".Lseq%d_done:", id), "")
        + s_comment("# SEQ drained — jmp outer ω")
        + s_1asm(std::string("jmp ") + outer_ω);

    g_emit.lbl_α = outer_α;
    g_emit.lbl_γ = outer_γ; g_emit.lbl_γ_p = outer_γ_p;
    g_emit.lbl_ω = outer_ω; g_emit.lbl_ω_p = outer_ω_p;
    g_emit.lbl_β = outer_β;

    return head + body + tail;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Raw-x86 gather multi-yield driver for the mode-3 SM_BB_INVOKE -> walk_bb_node path (g_emit.xa_bb_   */
/* emit_pair_*[] UNPOPULATED). Children (IR_SUSPEND/IR_FAIL) carry their own MEDIUM_BINARY arms; this  */
/* emits only the seq spine (α fan-out, β resume indirect-jump, per-child γ fixups, done trampoline)   */
/* directly into the shared bb_emit_buf, interleaving walk_bb_node(child, NULL) calls. Returns 1 if    */
/* handled, 0 to fall through to bb_seq_gather_str / passthrough.                                      */
int bb_seq_gather_binary(IR_t * pBB) {
    if (!(PLATFORM_X86 && MEDIUM_BINARY)) return 0;
    int n = seq_chain_len_ga(pBB->α);
    if (n == 0) return 0;
    int has_suspend = 0;
    for (IR_t * c = pBB->α; c; c = c->γ) if (c->t == IR_SUSPEND) { has_suspend = 1; break; }
    if (!has_suspend) return 0;
    int id = bb_node_id(pBB);
    uint64_t * resume_slot = (uint64_t *)calloc(1, sizeof(uint64_t));
    if (!resume_slot) return 0;
    bb_label_t * Lα = (bb_label_t *)calloc((size_t)n, sizeof(bb_label_t));
    bb_label_t * Lγ = (bb_label_t *)calloc((size_t)n, sizeof(bb_label_t));
    bb_label_t * Lω = (bb_label_t *)calloc((size_t)n, sizeof(bb_label_t));
    bb_label_t * Lβ = (bb_label_t *)calloc((size_t)n, sizeof(bb_label_t));
    bb_label_t * Ldone = (bb_label_t *)calloc(1, sizeof(bb_label_t));
    if (!Lα || !Lγ || !Lω || !Lβ || !Ldone) { return 0; }
    for (int k = 0; k < n; k++) {
        emit_label_initf(&Lα[k], ".Lseq%d_s%d_α", id, k);
        emit_label_initf(&Lγ[k], ".Lseq%d_s%d_γ", id, k);
        emit_label_initf(&Lω[k], ".Lseq%d_s%d_ω", id, k);
        emit_label_initf(&Lβ[k], ".Lseq%d_s%d_β", id, k);
    }
    emit_label_initf(Ldone, ".Lseq%d_done", id);
    bb_label_t * outer_γ_p = _.lbl_γ_p;
    bb_label_t * outer_ω_p = _.lbl_ω_p;
    bb_label_t * outer_β_p = _.lbl_β_p;
    bb_emit_byte(0xE9); bb_emit_patch_rel32(&Lα[0]);
    bb_label_define(outer_β_p);
    bb_emit_byte(0x48); bb_emit_byte(0xB8); bb_emit_u64((uint64_t)(uintptr_t)resume_slot);
    bb_emit_byte(0x48); bb_emit_byte(0x8B); bb_emit_byte(0x00);
    bb_emit_byte(0xFF); bb_emit_byte(0xE0);
    IR_t * cur = pBB->α;
    for (int k = 0; k < n && cur; k++, cur = cur->γ) {
        bb_label_t * child_ω = (k + 1 < n) ? &Lα[k + 1] : outer_ω_p;
        bb_label_define(&Lα[k]);
        _.lbl_α = Lα[k].name; _.lbl_α_p = &Lα[k];
        _.lbl_γ = Lγ[k].name; _.lbl_γ_p = &Lγ[k];
        _.lbl_ω = child_ω->name; _.lbl_ω_p = child_ω;
        _.lbl_β = Lβ[k].name; _.lbl_β_p = &Lβ[k];
        walk_bb_node(cur, NULL);
        bb_label_t * nxt = (k + 1 < n) ? &Lα[k + 1] : Ldone;
        bb_label_define(&Lγ[k]);
        bb_emit_byte(0x48); bb_emit_byte(0xB8); bb_emit_u64((uint64_t)(uintptr_t)resume_slot);
        bb_emit_byte(0x48); bb_emit_byte(0x8D); bb_emit_byte(0x0D); bb_emit_patch_rel32(nxt);
        bb_emit_byte(0x48); bb_emit_byte(0x89); bb_emit_byte(0x08);
        bb_emit_byte(0xE9); bb_emit_patch_rel32(outer_γ_p);
    }
    bb_label_define(Ldone);
    bb_emit_byte(0xE9); bb_emit_patch_rel32(outer_ω_p);
    _.lbl_γ_p = outer_γ_p; _.lbl_ω_p = outer_ω_p; _.lbl_β_p = outer_β_p;
    return 1;
}
