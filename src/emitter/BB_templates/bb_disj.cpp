/* bb_disj.cpp — BB template for IR_DISJ: Prolog `;` disjunction (`( A ; B )`).
   AGW-9B-3 (2026-05-27). Same dispatcher structure as bb_choice but n is always 2 (the two
   branches stored as nd->α / nd->β by lower_pl). The branches were ALREADY wired by lower_pl
   (a.γ=γ_in, a.ω=b.α; b.γ=γ_in, b.ω=ω_in), so the bodies self-chain through their own node-pointer
   ports — the dispatcher only needs to land control at branch 0's entry with a trail mark, and
   the alt's β redo path stays as fail-through for now (full resumable disjunction is a later rung).
   x86 TEXT only. */
#include <string>
#include <vector>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
#include "IR.h"
}
/* PLR-J-5 (2026-05-29): runtime helpers the MEDIUM_BINARY alt arm calls via movabs+call rax.        */
extern "C" {
void rt_pl_trail_mark_push(void);
void rt_pl_trail_unwind_top(void);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_disj_str(IR_t * pBB) {
    (void)pBB;
    if (PLATFORM_X86) {
        if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — RESOLVE_ALT");
        if (MEDIUM_BINARY) {
            /* PLR-J-5 (2026-05-29): full MEDIUM_BINARY arm — byte twin of the TEXT arm below.  Was a   */
            /* double-jump stub.  α → pre[0] (trail_mark_push) → body[0]; pre[i>0] (trail_unwind_top) →  */
            /* body[i]; β → ω.  Branch self-chaining (branch_i.ω → pre[i+1]) is wired by the driver.     */
            /* pre[i]/body[i] are shared with flat_drive_pl_alt via emit_label_intern (pointer identity).*/
            int id = _.resolve_choice_id;
            int n  = _.resolve_choice_n;
            std::string b;
            std::vector<int> st; std::vector<bb_label_t *> lb; std::vector<bool> df;
            auto def = [&](bb_label_t *L){ st.push_back((int)b.size()); lb.push_back(L); df.push_back(true); };
            auto ref = [&](bb_label_t *L){ st.push_back((int)b.size() - 4); lb.push_back(L); df.push_back(false); };
            auto jmp32 = [&](bb_label_t *L){ b += bytes(1, "\xE9") + u32le(0); ref(L); };
            auto callabs = [&](void *fn){ b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)fn) + bytes(2, "\xFF\xD0"); };
            if (n <= 0) {
                def(_.lbl_α_p); jmp32(_.lbl_ω_p);
                def(_.lbl_β_p); jmp32(_.lbl_ω_p);
                bb_bin_t bin; bin.sites = st; bin.labels = lb; bin.is_def = df; bin.bytes = b;
                bb_emit_asm_result(b, bin);
                return std::string();
            }
            char buf[200];
            std::vector<bb_label_t *> L_pre(n), L_body(n);
            for (int i = 0; i < n; i++) {
                resolve_choice_clause_label(buf, sizeof buf, id, i, "pre");  L_pre[i]  = emit_label_intern(buf);
                resolve_choice_clause_label(buf, sizeof buf, id, i, "body"); L_body[i] = emit_label_intern(buf);
            }
            /* α: jmp pre[0]. */
            def(_.lbl_α_p);
            jmp32(L_pre[0]);
            /* pre[0]: trail_mark_push; jmp body[0]. */
            def(L_pre[0]);
            callabs((void*)rt_pl_trail_mark_push);
            jmp32(L_body[0]);
            /* pre[i>0]: trail_unwind_top; jmp body[i]. */
            for (int i = 1; i < n; i++) {
                def(L_pre[i]);
                callabs((void*)rt_pl_trail_unwind_top);
                jmp32(L_body[i]);
            }
            /* β: jmp ω. */
            def(_.lbl_β_p);
            jmp32(_.lbl_ω_p);
            bb_bin_t bin; bin.sites = st; bin.labels = lb; bin.is_def = df; bin.bytes = b;
            bb_emit_asm_result(b, bin);
            return std::string();
        }
        if (MEDIUM_TEXT) {
            int id = _.resolve_choice_id;
            int n  = _.resolve_choice_n;
            if (n <= 0) {
                return s_1asm(emit_fmt("%s:", _.lbl_α))
                     + s_2asm("jmp", _.lbl_ω)
                     + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω);
            }
            std::string out = s_1asm(emit_fmt("%s:", _.lbl_α))
                            + s_comment(emit_fmt("# BOX RESOLVE_ALT n=%d (mode-4 first-solution)", n));
            char pre0[160]; resolve_choice_clause_label(pre0, sizeof pre0, id, 0, "pre");
            out += s_2asm("jmp", pre0);
            char body0[160]; resolve_choice_clause_label(body0, sizeof body0, id, 0, "body");
            out += s_1asm(emit_fmt("%s:", pre0))
                 + s_2asm("call", "rt_pl_trail_mark_push@PLT")
                 + s_2asm("jmp", body0);
            for (int i = 1; i < n; i++) {
                char prei[160], bodyi[160];
                resolve_choice_clause_label(prei,  sizeof prei,  id, i, "pre");
                resolve_choice_clause_label(bodyi, sizeof bodyi, id, i, "body");
                out += s_1asm(emit_fmt("%s:", prei))
                     + s_2asm("call", "rt_pl_trail_unwind_top@PLT")
                     + s_2asm("jmp", bodyi);
            }
            out += s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω);
            return out;
        }
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_disj(IR_t * pBB) {
    std::string out = bb_disj_str(pBB);
    if (!out.empty()) emit_text_n(out.data(), out.size());
}
