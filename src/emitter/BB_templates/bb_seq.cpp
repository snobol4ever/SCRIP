/* bb_seq.cpp — IR_SEQ router. The IR_SEQ kind (Icon/Snocone compound-stmt sequence, Raku gather
   multi-yield body) has THREE distinct box shapes; each lives in its own one-box-one-file template
   per the BB-HYGIENE FACT RULE (DUP FORM 4 de-cram, RK-HY-1, 2026-06-02). This router tries the
   raw-binary gather emitter first (it emits in-place and reports handled), then calls each string-
   returning box's _str() in priority order and emits the first that produces bytes ("" = "not my
   shape, try the next"). The per-box files:
     bb_seq_gather.cpp        Raku gather multi-yield driver — SUSPEND children (TEXT _str + the
                              raw-binary in-place bb_seq_gather_binary). Legacy SM-era gather path;
                              the primary Raku gather is now IR_GATHER -> bb_rk_gather.cpp.
     bb_seq_flat.cpp          flat in-order statement sequence (TEXT, n>0, no SUSPEND) — Icon
                              compound semantics: stmt γ falls to next stmt α, last stmt γ to outer γ.
     bb_seq_passthrough.cpp   goto-chain passthrough / glue (CATCH-ALL): MEDIUM_MACRO_DEF comment,
                              MEDIUM_TEXT n==0 empty-seq glue, MEDIUM_BINARY any-n flat-chain glue.
   Routing order matches the original in-file fall-through: gather-binary (raw) → gather-text → flat
   → passthrough. FACT RULE clean; PEERS: no IR_t fields added. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int         bb_seq_gather_binary(IR_t * pBB);
std::string bb_seq_gather_str     (IR_t * pBB, bb_bin_t & bin);
std::string bb_seq_flat_str       (IR_t * pBB, bb_bin_t & bin);
std::string bb_seq_passthrough_str(IR_t * pBB, bb_bin_t & bin);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_seq(IR_t * pBB) {
    if (bb_seq_gather_binary(pBB)) return;
    bb_bin_t bin;
    std::string s;
    s = bb_seq_gather_str(pBB, bin);      if (!s.empty()) { bb_emit_asm_result(s, bin); return; }
    s = bb_seq_flat_str(pBB, bin);        if (!s.empty()) { bb_emit_asm_result(s, bin); return; }
    s = bb_seq_passthrough_str(pBB, bin); bb_emit_asm_result(s, bin);
}
