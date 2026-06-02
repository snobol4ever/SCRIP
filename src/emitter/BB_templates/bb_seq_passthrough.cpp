/* bb_seq_passthrough.cpp — BB box: IR_SEQ goto-chain passthrough / glue shape.
   RK-HY-1 de-cram (2026-06-02, Raku). Split out of bb_seq.cpp (one-box-one-file per the BB-HYGIENE
   FACT RULE DUP FORM 4). This box emits the driver-collected glue pairs in g_emit.xa_bb_emit_pair_*[]
   (the bb_conj.cpp idiom): label-defs + jmps. It owns three of the original bb_seq returns —
   (a) MEDIUM_MACRO_DEF -> the no-macro comment (any n); (b) MEDIUM_TEXT with n==0 -> the empty-seq
   glue (α→γ succeed-through, β→ω no-backtrack); (c) MEDIUM_BINARY with any n -> the flat-chain glue
   (in BINARY the children were already emitted by flat_drive_seq; this leaf emits only the collected
   β/ω pairs). It is the router's CATCH-ALL (last in priority): the TEXT n>0 flat + gather shapes are
   tried first and only the leftover combinations reach here. Returns "" for MEDIUM_TEXT with n>0 (that
   is the flat/gather shape's job — defer). FACT RULE: every byte via s_* and bytes() — no seg_byte/SL_B/
   sl_emit_one/emit_standard_blob. PEERS: no IR_t fields added. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Count the γ-chain length rooted at α. */
static int seq_chain_len_pt(IR_t * head) {
    int n = 0;
    for (IR_t * c = head; c; c = c->γ) n++;
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_seq_passthrough_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (!PLATFORM_X86) return std::string();
    if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — IR_SEQ");
    int n = seq_chain_len_pt(pBB->α);
    if (MEDIUM_TEXT && n != 0) return std::string();
    if (MEDIUM_TEXT) {
        std::string s;
        for (int i = 0; i < g_emit.xa_bb_emit_pair_n; i++) {
            if (g_emit.xa_bb_emit_pair_define[i]) s += emit_fmt("%s:\n", g_emit.xa_bb_emit_pair_define[i]->name);
            if (g_emit.xa_bb_emit_pair_jmp[i])    s += s_1asm(emit_fmt("jmp %s", g_emit.xa_bb_emit_pair_jmp[i]->name));
        }
        return s;
    }
    if (MEDIUM_BINARY) {
        std::string b;
        for (int i = 0; i < g_emit.xa_bb_emit_pair_n; i++) {
            if (g_emit.xa_bb_emit_pair_define[i]) {
                bin.sites.push_back((int)b.size());
                bin.labels.push_back(g_emit.xa_bb_emit_pair_define[i]);
                bin.is_def.push_back(true);
            }
            if (g_emit.xa_bb_emit_pair_jmp[i]) {
                b += bytes(1, "\xE9");
                bin.sites.push_back((int)b.size());
                bin.labels.push_back(g_emit.xa_bb_emit_pair_jmp[i]);
                bin.is_def.push_back(false);
                b += u32le(0);
            }
        }
        return b;
    }
    return std::string();
}
