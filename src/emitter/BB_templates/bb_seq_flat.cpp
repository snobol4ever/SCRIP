/* bb_seq_flat.cpp — BB box: IR_SEQ flat in-order statement sequence (no SUSPEND children).
   RK-HY-1 de-cram (2026-06-02, Raku). Split out of bb_seq.cpp (one-box-one-file per the BB-HYGIENE
   FACT RULE DUP FORM 4). This is the Icon compound-statement / Snocone proc-body sequence shape:
   each statement's γ falls through to the NEXT statement's α (success advances); only the LAST
   statement's γ goes to the outer γ. Statement failure (ω) advances to the outer ω (no backtracking
   across statements — Icon compound semantics, IBB GROUND-ZERO RESET 2026-05-28). The outer β just
   jmps the outer ω (a flat sequence has no resume). MEDIUM_TEXT only and n>0 only; returns "" for
   any other combination so the router falls through to the gather shape (SUSPEND children) or the
   passthrough/glue shape (n==0, or MEDIUM_BINARY). FACT RULE: every byte via s_* and bytes() — no
   seg_byte/SL_B/sl_emit_one/emit_standard_blob. PEERS: no IR_t fields added. */
#include <string>
#include <vector>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Count the γ-chain length rooted at α. */
static int seq_chain_len_fl(IR_t * head) {
    int n = 0;
    for (IR_t * c = head; c; c = c->γ) n++;
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Emit one child's four-port x86 inline with caller-chosen labels; returns the box's emitted TEXT. */
static std::string emit_child_box_fl(IR_t * child,
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
std::string bb_seq_flat_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (!PLATFORM_X86) return std::string();
    if (!MEDIUM_TEXT)  return std::string();
    int id = bb_node_id(pBB);
    int n  = seq_chain_len_fl(pBB->α);
    if (n == 0) return std::string();
    for (IR_t * c = pBB->α; c; c = c->γ) if (c->t == IR_SUSPEND) return std::string();

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

    std::string head =
          s_directive(".intel_syntax noprefix")
        + s_1asm(std::string(outer_α) + ":")
        + s_comment(emit_fmt("# BOX IR_SEQ(n=%d, flat in-order sequence — no SUSPEND children)", n))
        + s_1asm(emit_fmt("jmp .Lseq%d_s0_α", id));

    std::string body;
    IR_t * cur = pBB->α;
    for (int k = 0; k < n && cur; k++, cur = cur->γ) {
        bb_label_t * child_γ_target = (k + 1 < n) ? &Lα[k + 1] : outer_γ_p;
        body += emit_child_box_fl(cur, &Lα[k], child_γ_target, outer_ω_p, &Lβ[k]);
    }

    std::string tail =
          s_L1asm(std::string(outer_β) + ":", "")
        + s_comment("# flat seq: no backtracking — β -> outer ω")
        + s_1asm(std::string("jmp ") + outer_ω);

    g_emit.lbl_α = outer_α;
    g_emit.lbl_γ = outer_γ; g_emit.lbl_γ_p = outer_γ_p;
    g_emit.lbl_ω = outer_ω; g_emit.lbl_ω_p = outer_ω_p;
    g_emit.lbl_β = outer_β;

    return head + body + tail;
}
