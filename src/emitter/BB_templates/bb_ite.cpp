/* bb_ite.cpp — BB template for BB_ITE: Prolog (Cond -> Then ; Else).
   CAT-D-8 (2026-05-27, Opus 4.7). The driver flat_drive_pl_ite (emit_bb.c) is byte-free: it
   mints Then/Else region labels and recursively walks each sub-region. This template emits the
   wrapper's α prologue and the β-tombstone (β: jmp ω — ITE is non-resumable from caller's view;
   internal choice points inside Then/Else retry via their own β labels). All bytes originate
   here per the FACT RULE; the driver only mints labels and chains walk_bb_flat. x86 TEXT only. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_ite_str(BB_t * pBB, bb_bin_t & bin) {
    bin = {};
    (void)pBB;
    if (PLATFORM_X86) {
        return IF(MEDIUM_MACRO_DEF, s_comment("# no macro form — RESOLVE_ITE"))
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
             + IF(MEDIUM_TEXT,
                   s_comment("# END RESOLVE_ITE (β-tombstone via EP)")
                 + FOR(0, g_emit.xa_bb_emit_pair_n, [](int i) {
                       return (g_emit.xa_bb_emit_pair_define[i] ? emit_fmt("%s:\n", g_emit.xa_bb_emit_pair_define[i]->name) : std::string())
                            + (g_emit.xa_bb_emit_pair_jmp[i]    ? s_1asm(emit_fmt("jmp %s", g_emit.xa_bb_emit_pair_jmp[i]->name)) : std::string());
                   }));
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_ite(BB_t * pBB) {
    bb_bin_t bin;
    bb_emit_asm_result(bb_ite_str(pBB, bin), bin);
}
