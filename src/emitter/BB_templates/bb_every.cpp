#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
}
extern "C" char * walk_bb_node_str_c(IR_t *);
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_every_flat_str() {
    return IF(MEDIUM_TEXT, s_1asm(std::string(_.lbl_β) + ":")
                         + s_comment("# BOX IR_EVERY β [x86() self-encoding — every never backtracks: β -> ω]"))
         + x86("def", PORT_BETA)
         + x86("jmp", PORT_OMEGA);
}
/*--------------------------------------------------------------------------------------------------------------------*/
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
        static bb_label_t Lba, Lbg, Lbo, Lbb;
        emit_label_initf(&Lba, ".Levery%d_body_α", id);
        emit_label_initf(&Lbg, ".Levery%d_body_γ", id);
        emit_label_initf(&Lbo, ".Levery%d_body_ω", id);
        emit_label_initf(&Lbb, ".Levery%d_body_β", id);
        g_emit.lbl_α = Lba.name; g_emit.lbl_α_p = &Lba;
        g_emit.lbl_γ = Lbg.name; g_emit.lbl_γ_p = &Lbg;
        g_emit.lbl_ω = Lbo.name; g_emit.lbl_ω_p = &Lbo;
        g_emit.lbl_β = Lbb.name; g_emit.lbl_β_p = &Lbb;
        char * c = walk_bb_node_str_c(body);
        std::string body_text = c ? c : "";
        if (c) free(c);
        g_emit.lbl_α = outer_α;
        g_emit.lbl_γ = outer_γ; g_emit.lbl_γ_p = outer_γ_p;
        g_emit.lbl_ω = outer_ω; g_emit.lbl_ω_p = outer_ω_p;
        g_emit.lbl_β = outer_β;
        std::string head =
              s_directive(".intel_syntax noprefix")
            + s_1asm(std::string(outer_α) + ":")
            + s_comment(emit_fmt("# BOX IR_EVERY id=%d — pump body to exhaustion", id));
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
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_every(IR_t * pBB) {
    if (MEDIUM_BINARY) { bb_emit_x86(bb_every_flat_str()); return; }
    std::string s = bb_every_str(pBB);
    if (!s.empty()) emit_text_n(s.data(), s.size());
}
