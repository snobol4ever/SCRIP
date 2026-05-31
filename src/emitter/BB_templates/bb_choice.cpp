/* bb_choice.cpp — BB template for BB_CHOICE: Prolog multi-clause predicate.
   WAM-CP-5 (2026-05-28, Sonnet 4.6): migrated from 16-byte rsp stack frame to heap resolve_choice
   record via resolve_cp_push/pop.  The cursor (next clause to try) lives in cp->cursor (offset 48);
   the trail mark lives in cp->trail_mark (offset 16, int).  This makes the cursor survive across
   the α→γ→β cycle: the rsp frame was torn down by exit_γ, but the heap record lives until
   resolve_cp_pop.  g_resolve_bfr points to this CHOICE's CP for the duration of its life.
   The driver flat_drive_pl_choice (emit_bb.c) populates g_emit.resolve_choice_id + resolve_choice_n.
   resolve_choice field offsets (64-bit): trail_mark=16(int), cursor=48(int).
   WAM-CP-9 (2026-05-28, Opus 4.7): cut-scope nested in the CP record.  Mode-4 cannot use a C-local
   saved_cut/saved_barrier pair the way mode-2 does (the α→γ→caller→β round-trip discards stack),
   so the outer cut state lives in cp->saved_cut_flag (+56) and cp->saved_cut_barrier (+64).
   rt_pl_choice_cut_enter saves and clears at α / β entry; rt_pl_choice_cut_exit restores at the
   normal γ / exhausted exit; rt_pl_choice_cut_unwind restores AND truncates the CP chain when the
   body fired `!` (g_resolve_cut_flag observed at dispatch top or at exit_γ).  BB_CUT itself only sets
   the flag (no truncate); the CHOICE owns the truncate so cp stays alive long enough to read its
   saved slots on the cut path.
   Control flow:
     α  → resolve_cp_push(RESOLVE_CP_CLAUSE, trail_mark, callee_env, NULL, 0) → cp; cut_enter(cp); jmp dispatch
     β  → resolve_cp_current; if NULL jmp ω_in; cut_enter(cp); jmp dispatch
     dispatch: if g_resolve_cut_flag → cut_unwind + jmp ω_in; else cursor dispatch
     pre[0]   → cp->cursor++, jmp body[0]
     pre[i>0] → trail_unwind(cp->trail_mark), cp->cursor++, jmp body[i]
     exit_γ   → if g_resolve_cut_flag → cut_unwind + jmp γ_in; else cut_exit + jmp γ_in
     body[i].γ → jmp exit_γ
     body[i].ω → jmp dispatch
     exhausted → cut_exit + trail_unwind + resolve_cp_pop + jmp ω_in */
#include <string>
#include <vector>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
#include "BB.h"
}
/* PLR-J-5 (2026-05-29): runtime helpers the MEDIUM_BINARY choice arm calls via movabs+call rax     */
/* (in-process absolute addresses).  Byte twin of the MEDIUM_TEXT arm's @PLT calls below.            */
extern "C" {
void  *rt_pl_env_current(void);
int    rt_pl_trail_mark(void);
void   rt_pl_trail_unwind(int mark);
void  *resolve_cp_push(int type, int trail_mark, void *env, void *resume, int cursor);
void  *resolve_cp_current(void);
void   resolve_cp_pop(void);
void   rt_pl_choice_cut_enter(void *cp);
void   rt_pl_choice_cut_exit(void *cp);
void   rt_pl_choice_cut_unwind(void *cp);
int    rt_pl_get_cut_flag(void);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_choice_str(IR_t * pBB) {
    (void)pBB;
    if (PLATFORM_X86) {
        if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — RESOLVE_CHOICE");
        if (MEDIUM_BINARY) {
            /* PLR-J-5 (2026-05-29): full MEDIUM_BINARY choice arm — byte twin of the MEDIUM_TEXT arm */
            /* below.  Was a double-jump stub (jmp ω; jmp ω) that the SM_BB_PRED_INVOKE multi-clause     */
            /* guard refused to walk.  Every @PLT call becomes movabs rax,&fn; call rax (in-process    */
            /* absolute, valid for mode-3 native).  Internal labels (dispatch/exit_γ/exhausted/cut_γ/  */
            /* cut_ω/β_nosol and per-clause pre[i]) and the cross-shared labels (α/β/γ/ω + body[i] +   */
            /* exit_γ, which flat_drive_pl_choice references) all go through the bb_bin_t descriptor    */
            /* via emit_label_intern — same name → same bb_label_t*, so bb_label_define / patch_rel32   */
            /* resolve by pointer identity across the template and the driver-emitted clause bodies.    */
            int id = _.resolve_choice_id;
            int n  = _.resolve_choice_n;
            if (n <= 0) {
                /* α: jmp ω ; β: jmp ω */
                std::vector<int> st; std::vector<bb_label_t *> lb; std::vector<bool> df;
                std::string b;
                st.push_back((int)b.size()); lb.push_back(_.lbl_α_p); df.push_back(true);
                b += bytes(1, "\xE9"); st.push_back((int)b.size()); lb.push_back(_.lbl_ω_p); df.push_back(false); b += u32le(0);
                st.push_back((int)b.size()); lb.push_back(_.lbl_β_p); df.push_back(true);
                b += bytes(1, "\xE9"); st.push_back((int)b.size()); lb.push_back(_.lbl_ω_p); df.push_back(false); b += u32le(0);
                bb_bin_t bin; bin.sites = st; bin.labels = lb; bin.is_def = df; bin.bytes = b;
                bb_emit_asm_result(b, bin);
                return std::string();
            }
            /* Intern the template-internal + cross-shared labels (names mirror the TEXT arm exactly). */
            char buf[200];
            snprintf(buf, sizeof buf, ".Lplch%d_dispatch",  id); bb_label_t *L_disp  = emit_label_intern(buf);
            snprintf(buf, sizeof buf, ".Lplch%d_exit_γ",    id); bb_label_t *L_exitγ = emit_label_intern(buf);
            snprintf(buf, sizeof buf, ".Lplch%d_exhausted", id); bb_label_t *L_exh   = emit_label_intern(buf);
            snprintf(buf, sizeof buf, ".Lplch%d_cut_γ",     id); bb_label_t *L_cutγ  = emit_label_intern(buf);
            snprintf(buf, sizeof buf, ".Lplch%d_cut_ω",     id); bb_label_t *L_cutω  = emit_label_intern(buf);
            snprintf(buf, sizeof buf, ".Lplch%d_β_nosol",   id); bb_label_t *L_nosol = emit_label_intern(buf);
            std::vector<bb_label_t *> L_pre(n), L_body(n);
            for (int i = 0; i < n; i++) {
                resolve_choice_clause_label(buf, sizeof buf, id, i, "pre");  L_pre[i]  = emit_label_intern(buf);
                resolve_choice_clause_label(buf, sizeof buf, id, i, "body"); L_body[i] = emit_label_intern(buf);
            }
            std::string b;
            std::vector<int> st; std::vector<bb_label_t *> lb; std::vector<bool> df;
            /* def(lbl): record a label-definition site (0 bytes consumed). */
            auto def = [&](bb_label_t *L){ st.push_back((int)b.size()); lb.push_back(L); df.push_back(true); };
            /* ref32(): the preceding opcode left a rel32 slot at b.end()-4; register it as a ref. */
            auto ref = [&](bb_label_t *L){ st.push_back((int)b.size() - 4); lb.push_back(L); df.push_back(false); };
            /* callabs(fn): movabs rax, &fn ; call rax. */
            auto callabs = [&](void *fn){ b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)fn) + bytes(2, "\xFF\xD0"); };
            /* jmp32(L): E9 rel32 → L. */
            auto jmp32 = [&](bb_label_t *L){ b += bytes(1, "\xE9") + u32le(0); ref(L); };
            /* je32(L): 0F 84 rel32 → L. */
            auto je32  = [&](bb_label_t *L){ b += bytes(2, "\x0F\x84") + u32le(0); ref(L); };
            auto jge32 = [&](bb_label_t *L){ b += bytes(2, "\x0F\x8D") + u32le(0); ref(L); };
            auto jnz32 = [&](bb_label_t *L){ b += bytes(2, "\x0F\x85") + u32le(0); ref(L); };
            /* === α: push CP record, cut_enter. === */
            def(_.lbl_α_p);
            callabs((void*)rt_pl_env_current);               /* rax = g_resolve_env             */
            b += bytes(3, "\x48\x89\xC2");                   /* mov rdx, rax (callee_env)  */
            callabs((void*)rt_pl_trail_mark);                /* eax = trail mark           */
            b += bytes(3, "\x48\x89\xC6");                   /* mov rsi, rax (trail_mark)  */
            b += bytes(2, "\x31\xFF");                        /* xor edi, edi (type=0)      */
            b += bytes(2, "\x31\xC9");                        /* xor ecx, ecx (resume=NULL) */
            b += bytes(3, "\x45\x31\xC0");                    /* xor r8d, r8d (cursor=0)    */
            callabs((void*)resolve_cp_push);                       /* rax = cp                   */
            b += bytes(3, "\x48\x89\xC7");                    /* mov rdi, rax               */
            callabs((void*)rt_pl_choice_cut_enter);
            /* === dispatch: cursor → next clause pre[]. === */
            def(L_disp);
            callabs((void*)resolve_cp_current);                    /* rax = cp                   */
            b += bytes(3, "\x8B\x78\x30");                    /* mov edi, [rax + 48]        */
            b += bytes(2, "\x81\xFF") + u32le((uint32_t)n);   /* cmp edi, n                 */
            jge32(L_exh);
            for (int i = 0; i < n; i++) {
                b += bytes(2, "\x81\xFF") + u32le((uint32_t)i); /* cmp edi, i               */
                je32(L_pre[i]);
            }
            jmp32(L_exh);
            /* === pre[0]: no unwind, cursor++, jmp body[0]. === */
            def(L_pre[0]);
            callabs((void*)resolve_cp_current);
            b += bytes(3, "\xFF\x40\x30");                    /* inc dword ptr [rax + 48]   */
            jmp32(L_body[0]);
            /* === pre[i>0]: unwind trail to cp->trail_mark, cursor++, jmp body[i]. === */
            for (int i = 1; i < n; i++) {
                def(L_pre[i]);
                callabs((void*)resolve_cp_current);
                b += bytes(3, "\x8B\x78\x10");                /* mov edi, [rax + 16]        */
                callabs((void*)rt_pl_trail_unwind);
                callabs((void*)resolve_cp_current);
                b += bytes(3, "\xFF\x40\x30");                /* inc dword ptr [rax + 48]   */
                jmp32(L_body[i]);
            }
            /* === exit_γ: body succeeded.  cut-check → cut_γ, else cut_exit, jmp γ. === */
            def(L_exitγ);
            callabs((void*)rt_pl_get_cut_flag);
            b += bytes(2, "\x85\xC0");                        /* test eax, eax              */
            jnz32(L_cutγ);
            callabs((void*)resolve_cp_current);
            b += bytes(3, "\x48\x89\xC7");                    /* mov rdi, rax               */
            callabs((void*)rt_pl_choice_cut_exit);
            jmp32(_.lbl_γ_p);
            /* === cut_γ: cut fired + body succeeded → unwind + truncate → jmp γ. === */
            def(L_cutγ);
            callabs((void*)resolve_cp_current);
            b += bytes(3, "\x48\x89\xC7");
            callabs((void*)rt_pl_choice_cut_unwind);
            jmp32(_.lbl_γ_p);
            /* === cut_ω: cut fired then body failed → unwind + truncate → jmp ω. === */
            def(L_cutω);
            callabs((void*)resolve_cp_current);
            b += bytes(3, "\x48\x89\xC7");
            callabs((void*)rt_pl_choice_cut_unwind);
            jmp32(_.lbl_ω_p);
            /* === exhausted: all clauses failed → cut_exit, unwind, pop, jmp ω. === */
            def(L_exh);
            callabs((void*)resolve_cp_current);
            b += bytes(3, "\x48\x89\xC7");
            callabs((void*)rt_pl_choice_cut_exit);
            callabs((void*)resolve_cp_current);
            b += bytes(3, "\x8B\x78\x10");                    /* mov edi, [rax + 16]        */
            callabs((void*)rt_pl_trail_unwind);
            callabs((void*)resolve_cp_pop);
            jmp32(_.lbl_ω_p);
            /* === β: redo entry. cut-check → cut_ω; else if cp NULL → β_nosol; else cut_enter, disp. */
            def(_.lbl_β_p);
            callabs((void*)rt_pl_get_cut_flag);
            b += bytes(2, "\x85\xC0");                        /* test eax, eax              */
            jnz32(L_cutω);
            callabs((void*)resolve_cp_current);
            b += bytes(3, "\x48\x85\xC0");                    /* test rax, rax              */
            je32(L_nosol);
            b += bytes(3, "\x48\x89\xC7");                    /* mov rdi, rax               */
            callabs((void*)rt_pl_choice_cut_enter);
            jmp32(L_disp);
            /* === β_nosol: jmp ω. === */
            def(L_nosol);
            jmp32(_.lbl_ω_p);
            /* exit_γ is referenced by the driver-emitted clause bodies (walk_bb_flat γ target) via the */
            /* interned .Lplch%d_exit_γ pointer; we DEFINED it above, so those refs resolve by identity. */
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
            char disp[160];      snprintf(disp,      sizeof disp,      ".Lplch%d_dispatch",  id);
            char exit_γ[160];    snprintf(exit_γ,    sizeof exit_γ,    ".Lplch%d_exit_γ",    id);
            char exhausted[160]; snprintf(exhausted,  sizeof exhausted, ".Lplch%d_exhausted", id);
            char cut_unwind_ω[160]; snprintf(cut_unwind_ω, sizeof cut_unwind_ω, ".Lplch%d_cut_ω", id);
            char cut_unwind_γ[160]; snprintf(cut_unwind_γ, sizeof cut_unwind_γ, ".Lplch%d_cut_γ", id);
            char β_nosol[160];      snprintf(β_nosol,      sizeof β_nosol,      ".Lplch%d_β_nosol", id);
            /* α: push CP record. resolve_cp_push(RESOLVE_CP_CLAUSE=0, trail_mark, callee_env, NULL, 0). Get      */
            /* env first, then trail_mark so callee_env (g_resolve_env) is captured after args are bound by  */
            /* caller. SysV order: rdi=type(0), rsi=trail_mark, rdx=callee_env, rcx=NULL, r8=cursor(0). */
            /* WAM-CP-9: immediately after push (rax=cp), call rt_pl_choice_cut_enter(cp) to save the   */
            /* outer cut state into cp->saved_cut_{flag,barrier} and set g_resolve_cut_barrier = cp->parent. */
            std::string out = s_1asm(emit_fmt("%s:", _.lbl_α))
                            + s_comment(emit_fmt("# BOX RESOLVE_CHOICE n=%d (WAM-CP-5 heap cursor, WAM-CP-9 cut)", n))
                            + s_2asm("call", "rt_pl_env_current@PLT")
                            + s_2asm("mov",  "rdx, rax")
                            + s_2asm("call", "rt_pl_trail_mark@PLT")
                            + s_2asm("mov",  "rsi, rax")
                            + s_2asm("xor",  "edi, edi")
                            + s_2asm("xor",  "ecx, ecx")
                            + s_2asm("xor",  "r8d, r8d")
                            + s_2asm("call", "resolve_cp_push@PLT")
                            + s_2asm("mov",  "rdi, rax")
                            + s_2asm("call", "rt_pl_choice_cut_enter@PLT");
            /* dispatch: cursor → next clause's pre[]. Flag check moved to β/exit_γ entries (the flag */
            /* is only meaningful BEFORE we re-enter cut scope; here we've just cleared it via _enter). */
            out += s_1asm(emit_fmt("%s:", disp))
                 + s_2asm("call", "resolve_cp_current@PLT")
                 + s_2asm("mov",  "edi, [rax + 48]")
                 + s_2asm("cmp",  emit_fmt("edi, %d", n))
                 + s_2asm("jge",  exhausted);
            for (int i = 0; i < n; i++) {
                char prei[160]; resolve_choice_clause_label(prei, sizeof prei, id, i, "pre");
                out += s_2asm("cmp", emit_fmt("edi, %d", i))
                     + s_2asm("je",  prei);
            }
            out += s_2asm("jmp", exhausted);
            /* pre[0]: no unwind (already at entry mark), cursor++, jmp body[0] */
            char body0[160]; resolve_choice_clause_label(body0, sizeof body0, id, 0, "body");
            char pre0[160];  resolve_choice_clause_label(pre0,  sizeof pre0,  id, 0, "pre");
            out += s_1asm(emit_fmt("%s:", pre0))
                 + s_2asm("call", "resolve_cp_current@PLT")
                 + s_2asm("inc",  "dword ptr [rax + 48]")
                 + s_2asm("jmp",  body0);
            /* pre[i>0]: unwind trail to cp->trail_mark, cursor++, jmp body[i] */
            for (int i = 1; i < n; i++) {
                char prei[160], bodyi[160];
                resolve_choice_clause_label(prei,  sizeof prei,  id, i, "pre");
                resolve_choice_clause_label(bodyi, sizeof bodyi, id, i, "body");
                out += s_1asm(emit_fmt("%s:", prei))
                     + s_2asm("call", "resolve_cp_current@PLT")
                     + s_2asm("mov",  "edi, [rax + 16]")
                     + s_2asm("call", "rt_pl_trail_unwind@PLT")
                     + s_2asm("call", "resolve_cp_current@PLT")
                     + s_2asm("inc",  "dword ptr [rax + 48]")
                     + s_2asm("jmp",  bodyi);
            }
            /* exit_γ: clause body succeeded. Cut-check: if `!` fired during body, restore outer cut    */
            /* state and truncate cp (no further clauses available). Else restore outer cut state and  */
            /* keep cp alive for caller's β redo. Either way, jmp γ_in.                                 */
            out += s_1asm(emit_fmt("%s:", exit_γ))
                 + s_2asm("call", "rt_pl_get_cut_flag@PLT")
                 + s_2asm("test", "eax, eax")
                 + s_2asm("jnz",  cut_unwind_γ)
                 + s_2asm("call", "resolve_cp_current@PLT")
                 + s_2asm("mov",  "rdi, rax")
                 + s_2asm("call", "rt_pl_choice_cut_exit@PLT")
                 + s_2asm("jmp",  _.lbl_γ);
            /* cut_unwind_γ: cut fired, body succeeded → restore + truncate to cp->parent → jmp γ_in.  */
            out += s_1asm(emit_fmt("%s:", cut_unwind_γ))
                 + s_2asm("call", "resolve_cp_current@PLT")
                 + s_2asm("mov",  "rdi, rax")
                 + s_2asm("call", "rt_pl_choice_cut_unwind@PLT")
                 + s_2asm("jmp",  _.lbl_γ);
            /* cut_unwind_ω: cut fired then body failed → restore + truncate to cp->parent → jmp ω_in. */
            out += s_1asm(emit_fmt("%s:", cut_unwind_ω))
                 + s_2asm("call", "resolve_cp_current@PLT")
                 + s_2asm("mov",  "rdi, rax")
                 + s_2asm("call", "rt_pl_choice_cut_unwind@PLT")
                 + s_2asm("jmp",  _.lbl_ω);
            /* exhausted: all clauses tried, none succeeded. Restore outer cut state, unwind trail to  */
            /* cp's mark, pop cp, jmp ω_in. cut state SHOULD already be 0 here (would have been caught */
            /* at dispatch otherwise), but restoring unconditionally keeps the invariant clean.        */
            out += s_1asm(emit_fmt("%s:", exhausted))
                 + s_2asm("call", "resolve_cp_current@PLT")
                 + s_2asm("mov",  "rdi, rax")
                 + s_2asm("call", "rt_pl_choice_cut_exit@PLT")
                 + s_2asm("call", "resolve_cp_current@PLT")
                 + s_2asm("mov",  "edi, [rax + 16]")
                 + s_2asm("call", "rt_pl_trail_unwind@PLT")
                 + s_2asm("call", "resolve_cp_pop@PLT")
                 + s_2asm("jmp",  _.lbl_ω);
            /* β: re-entry. Body[i].ω chains here on failure AND caller's BB_GOAL β chains here on  */
            /* redo. Distinguishing them is the cut flag's job: if the body just executed fired `!`, the */
            /* flag is set (cut_set was called inside; we haven't called _enter since), and we unwind to */
            /* ω_in. Otherwise (flag==0): if cp is NULL the outer cut truncated us → ω_in; else re-save */
            /* the (current outer) cut state into our cp and clear the flag for the next clause attempt. */
            out += s_1asm(emit_fmt("%s:", _.lbl_β))
                 + s_2asm("call", "rt_pl_get_cut_flag@PLT")
                 + s_2asm("test", "eax, eax")
                 + s_2asm("jnz",  cut_unwind_ω)
                 + s_2asm("call", "resolve_cp_current@PLT")
                 + s_2asm("test", "rax, rax")
                 + s_2asm("je",   β_nosol)
                 + s_2asm("mov",  "rdi, rax")
                 + s_2asm("call", "rt_pl_choice_cut_enter@PLT")
                 + s_2asm("jmp",  disp);
            out += s_L2asm(emit_fmt("%s:", β_nosol), "jmp", _.lbl_ω);
            return out;
        }
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_choice(IR_t * pBB) {
    std::string out = bb_choice_str(pBB);
    if (!out.empty()) emit_text_n(out.data(), out.size());
}
