/* bb_conj.cpp — BB template for BB_GCONJ: Prolog conjunction (a, b, c).
   AGW-9A (2026-05-27). Mirrors bb_pat_cat.cpp: the driver flat_drive_pl_seq in emit_bb.c
   owns all recursion + label minting (byte-free) and populates g_emit.xa_bb_emit_pair_* with the
   conjunction glue (the β-chain jmp into the last goal's β). This template is the leaf that
   emits those collected label-defs + jmps. No XA_PL_SEQ_DRIVE opcode (that pattern does not
   exist in the tree — see GOAL-PROLOG-BB PL-2 correction). x86 TEXT only for now. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_conj_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (PLATFORM_X86) {
        return IF(MEDIUM_MACRO_DEF, s_comment("# no macro form — RESOLVE_SEQ"))
             + IF(MEDIUM_BINARY, [&]() {
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
               }())
             + IF(MEDIUM_TEXT, FOR(0, g_emit.xa_bb_emit_pair_n, [](int i) {
                   return (g_emit.xa_bb_emit_pair_define[i] ? emit_fmt("%s:\n", g_emit.xa_bb_emit_pair_define[i]->name) : std::string())
                        + (g_emit.xa_bb_emit_pair_jmp[i]    ? s_1asm(emit_fmt("jmp %s", g_emit.xa_bb_emit_pair_jmp[i]->name)) : std::string());
               }));
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_conj(IR_t * pBB) {
    bb_bin_t bin;
    bb_emit_asm_result(bb_conj_str(pBB, bin), bin);
}
