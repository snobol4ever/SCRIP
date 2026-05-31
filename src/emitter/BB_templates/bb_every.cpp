/* bb_every.cpp — BB template for BB_EVERY (generic generator pump driver).
   LANGUAGE-IGNORANT: BB_EVERY pumps its body generator to exhaustion.
   IBB ground-zero (Opus 4.7, 2026-05-28).

   Wire:
       outer_α: jmp body.α
       body.γ -> outer_α  (loop: pump body's β as if α — bb_to handles its own re-entry)
       body.ω -> outer_γ  (generator exhausted -> every succeeds)
       outer_β: jmp outer_ω  (every doesn't backtrack itself)

   The body sub-graph re-yields by jumping to its β label (which we wire = outer_α for the
   first iteration only; per the generator's own loop the body is re-entered via its β port
   on retry). For now, the simplest correct shape: body.γ jumps back to body.β if body has
   a β label, else to body.α. We use body.β here because every generator BB template
   (BB_TO etc.) emits a real β label that resumes the generator.                          */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
}
extern "C" char * walk_bb_node_str_c(BB_t *);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_every_str(BB_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (!PLATFORM_X86) return std::string();
    if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — BB_EVERY");
    if (MEDIUM_TEXT) {
        BB_t * body = pBB->α;
        if (!body) {
            fprintf(stderr, "[IBB] FATAL bb_every: body (pBB->α) is NULL\n");
            abort();
        }
        int id = bb_node_id(pBB);
        const char * outer_α = _.lbl_α;
        const char * outer_γ = _.lbl_γ;
        const char * outer_ω = _.lbl_ω;
        const char * outer_β = _.lbl_β;
        bb_label_t * outer_γ_p = _.lbl_γ_p;
        bb_label_t * outer_ω_p = _.lbl_ω_p;

        /* Body label set: body.α (entry), body.γ -> body.β (loop), body.ω -> outer.γ, body.β. */
        static bb_label_t Lba, Lbg, Lbo, Lbb;
        emit_label_initf(&Lba, ".Levery%d_body_α", id);
        emit_label_initf(&Lbg, ".Levery%d_body_γ", id);
        emit_label_initf(&Lbo, ".Levery%d_body_ω", id);
        emit_label_initf(&Lbb, ".Levery%d_body_β", id);

        /* Capture the outer-label state to restore after child emit overwrites g_emit.lbl_*. */
        g_emit.lbl_α = Lba.name; g_emit.lbl_α_p = &Lba;
        g_emit.lbl_γ = Lbg.name; g_emit.lbl_γ_p = &Lbg;
        g_emit.lbl_ω = Lbo.name; g_emit.lbl_ω_p = &Lbo;
        g_emit.lbl_β = Lbb.name; g_emit.lbl_β_p = &Lbb;
        char * c = walk_bb_node_str_c(body);
        std::string body_text = c ? c : "";
        if (c) free(c);
        /* Restore outer labels. */
        g_emit.lbl_α = outer_α;
        g_emit.lbl_γ = outer_γ; g_emit.lbl_γ_p = outer_γ_p;
        g_emit.lbl_ω = outer_ω; g_emit.lbl_ω_p = outer_ω_p;
        g_emit.lbl_β = outer_β;

        std::string head =
              s_directive(".intel_syntax noprefix")
            + s_1asm(std::string(outer_α) + ":")
            + s_comment(emit_fmt("# BOX BB_EVERY id=%d — pump body to exhaustion", id));

        /* After body's own text is laid down, body.γ landed here defines the loop-back point.   */
        /* body's text emits `jmp .Levery_body_γ` on yield. We define .Levery_body_γ as a label   */
        /* that jmps back into body.β (re-pump the generator). body.ω jmps outer_γ.               */
        std::string post_body =
              s_L1asm(std::string(Lbg.name) + ":", "")
            + s_comment("# every: body yielded a value — re-pump via body.β")
            + s_1asm(std::string("jmp ") + Lbb.name)
            + s_L1asm(std::string(Lbo.name) + ":", "")
            + s_comment("# every: body exhausted — every succeeds")
            + s_1asm(std::string("jmp ") + outer_γ)
            + s_L1asm(std::string(outer_β) + ":", "")
            + s_comment("# every: no backtracking — β -> outer ω")
            + s_1asm(std::string("jmp ") + outer_ω);

        return head
             + s_1asm(std::string("jmp ") + Lba.name)
             + body_text
             + post_body;
    }
    if (MEDIUM_BINARY) {
        /* IBB-4 (Opus 4.7, 2026-05-28): pair-driven emit. flat_drive_every (in emit_bb.c) walks  */
        /* bb->α (the body) with γ-and-β both pointing at body's mid-stream β-define label, and    */
        /* body.ω → outer γ, so the loop topology is set up BEFORE we get here. This template      */
        /* emits ONLY the driver's queued EMIT_PAIR_DEF_JMP entries (every's own outer-β port stub */
        /* that jmps to outer ω), mirroring bb_pat_alt.cpp's MEDIUM_BINARY arm.                     */
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_every(BB_t * pBB) { bb_bin_t bin; bb_emit_asm_result(bb_every_str(pBB, bin), bin); }
