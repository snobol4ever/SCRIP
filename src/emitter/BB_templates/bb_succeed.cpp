/* bb_succeed.cpp — BB template for IR_SUCCEED (the Prolog `true` / `otherwise` leaf).
   AGW-9B-3 (2026-05-27). Walk_bb_flat's IR_SUCCEED case populates g_emit.xa_bb_emit_pair_* with a
   `jmp γ_in` glue entry and the β-fail entry; this template replays them, mirroring bb_conj.
   Pure leaf — no per-node state, no effect helpers, just two label-defs/jmps. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_succeed_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    (void)pBB;
    if (PLATFORM_X86) {
        if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — SUCCEED");
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
        if (MEDIUM_TEXT) {
            std::string head = s_1asm(emit_fmt("%s:", _.lbl_α))
                             + s_comment("# BOX SUCCEED()");
            return head + FOR(0, g_emit.xa_bb_emit_pair_n, [](int i) {
                return (g_emit.xa_bb_emit_pair_define[i] ? emit_fmt("%s:\n", g_emit.xa_bb_emit_pair_define[i]->name) : std::string())
                     + (g_emit.xa_bb_emit_pair_jmp[i]    ? s_1asm(emit_fmt("jmp %s", g_emit.xa_bb_emit_pair_jmp[i]->name)) : std::string());
            });
        }
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_succeed(IR_t * pBB) {
    bb_bin_t bin;
    bb_emit_asm_result(bb_succeed_str(pBB, bin), bin);
}
