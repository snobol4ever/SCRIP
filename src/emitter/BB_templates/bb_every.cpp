/* bb_every.cpp — BB template for IR_EVERY (generic generator pump driver).
   LANGUAGE-IGNORANT: IR_EVERY pumps its body generator to exhaustion.
   IBB ground-zero (Opus 4.7, 2026-05-28). MEDIUM_BINARY arm → x86() self-encoding (template-revamp,
   Icon GZ-11+, 2026-06-02).

   TWO ARMS, discriminated on MEDIUM (NOT g_icn_flat_chain — that flag is already 0 by the time the
   EMIT_PAIR_FILL → walk_bb_node → bb_every call lands, since flat_drive_every runs the FILL after the
   chain emitter has cleared it). The original split was always medium-based and stays that way:

   • MEDIUM_BINARY (mode-3 in-pool BLOB) — flat_drive_every (emit_bb.c) does ALL the body-walk + label-
     wiring BEFORE invoking this template; every code path that FILLs the every-box itself first runs
     EMIT_PAIR_RESET(); EMIT_PAIR_DEF_JMP(lbl_β, lbl_ω). So the box's only remaining emission is its own
     β port: define β, jmp ω (every never backtracks itself — generator exhaustion is the body's success
     exit; the loop-back lives in the already-emitted body sub-graph). This is now x86()-encoded
     (def PORT_BETA + jmp PORT_OMEGA), BYTE-IDENTICAL to the old pair-replay, with NO bb_bin_t.

   • MEDIUM_TEXT (mode-2 interp AND mode-4 --compile .s) — the recursive body-walk arm. This arm reads
     pBB->α (the body sub-graph) and recurses via walk_bb_node_str_c; it is NOT pBB-free and is left
     verbatim (an every box's mode-2/mode-4 form fundamentally needs the body graph, so it cannot reduce
     to a pure operand-scalar x86() concat the way a value leaf does).                                   */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
}
extern "C" char * walk_bb_node_str_c(IR_t *);
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* MEDIUM_BINARY (mode-3) β-port emission: define β, jmp ω. pBB-FREE: reads only g_emit ports. The IF()    */
/* label/comment is TEXT-only (no-op in BINARY); under BINARY this is exactly two records — D(β), J(ω).   */
static std::string bb_every_flat_str() {
    return IF(MEDIUM_TEXT, s_1asm(std::string(_.lbl_β) + ":")
                         + s_comment("# BOX IR_EVERY β [x86() self-encoding — every never backtracks: β -> ω]"))
         + x86("def", PORT_BETA)
         + x86("jmp", PORT_OMEGA);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_every_str(IR_t * pBB) {
    if (!PLATFORM_X86) return std::string();
    if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — IR_EVERY");
    if (MEDIUM_TEXT) {
        IR_t * body = pBB->α;
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
            + s_comment(emit_fmt("# BOX IR_EVERY id=%d — pump body to exhaustion", id));

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
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_every(IR_t * pBB) {
    if (MEDIUM_BINARY) { bb_emit_x86(bb_every_flat_str()); return; }
    std::string s = bb_every_str(pBB);            /* TEXT arm: GAS text, no patch sites (bb_bin_t ABOLISHED) */
    if (!s.empty()) emit_text_n(s.data(), s.size());
}
