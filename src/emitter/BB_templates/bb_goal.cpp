/* bb_goal.cpp — BB template for BB_GOAL: Prolog predicate call (WAM-CP-5).
   WAM-CP-5 (2026-05-28, Sonnet 4.6): BB_GOAL owns NO CP record.  The CP is owned by the
   CALLEE's BB_CHOICE, which handles trail unwind between clauses in pre[i>0].  BB_CALL's beta
   calls _redo WITHOUT pre-unwinding (arg-alias bindings survive; CHOICE pre[i] handles it).
   caller_env saved into g_resolve_bfr->saved_args via rt_pl_cp_save_caller_env() on first success.
   BB_STRUCT compound args built via emit_build_compound_term (post-order BB walker).
   x86 TEXT only per RULES "X86 ONLY". */
#include <string>
#include <vector>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
#include "emit_core.h"
#include "BB.h"
}
extern std::string emit_build_compound_term(const IR_t *nd);
extern std::string emit_build_compound_term_bin(const IR_t *nd);
extern "C" {
void  *rt_pl_node_to_term(int kind, long ival, const char *sval, double dval);
void **resolve_bb_env_save_push(int nslots);
void   resolve_bb_bind_arg(int slot, void *caller_term);
int    rt_last_ok(void);
void **resolve_bb_env_install(void **env);
void   rt_pl_cp_save_caller_env(void *caller_env);
void   resolve_bb_env_pop(void **saved);
void  *resolve_cp_current(void);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* PLR-J-4a (2026-05-29): MEDIUM_BINARY twin of the TEXT build_arg.  Leaves a Term* in rax.            */
/* BB_STRUCT routes to emit_build_compound_term_bin (PLR-J-3); scalars use movabs+call to            */
/* rt_pl_node_to_term (SysV edi=kind rsi=ival rdx=sval xmm0=dval; dval=0 → xorps).                      */
static std::string build_arg_bin(IR_t *a) {
    if (!a) return bytes(2, "\x31\xC0");                  /* xor eax,eax → NULL Term* */
    if (a->t == BB_STRUCT) return emit_build_compound_term_bin(a);
    int  kind = (int)a->t;
    long ival = (long)a->ival;
    const char *sval = (a->sval && *a->sval) ? a->sval : NULL;
    std::string b;
    b += bytes(1, "\xBF") + u32le((uint32_t)kind);                            /* mov edi, kind */
    b += bytes(2, "\x48\xBE") + u64le((uint64_t)(long)ival);                  /* movabs rsi, ival */
    if (sval) b += bytes(2, "\x48\xBA") + u64le((uint64_t)(uintptr_t)sval);   /* movabs rdx, sval */
    else      b += bytes(2, "\x31\xD2");                                      /* xor edx,edx */
    b += bytes(3, "\x0F\x57\xC0");                                            /* xorps xmm0,xmm0 */
    b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_pl_node_to_term) + bytes(2, "\xFF\xD0");
    return b;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string build_arg(IR_t *a) {
    if (!a) return s_2asm("xor", "eax, eax");
    if (a->t == BB_STRUCT) return emit_build_compound_term(a);
    int kind = (int)a->t;
    long ival = (long)a->ival;
    const char *sval = a->sval;
    char slbl[64]; slbl[0] = 0;
    if (sval && *sval) strtab_label(slbl, sizeof slbl, sval);
    return s_2asm("mov", emit_fmt("edi, %d", kind))
         + s_2asm("mov", emit_fmt("rsi, %ld", ival))
         + (slbl[0] ? s_2asm("lea", emit_fmt("rdx, [rip + %s]", slbl))
                    : s_2asm("xor", "edx, edx"))
         + s_2asm("xorps", "xmm0, xmm0")
         + s_2asm("call", "rt_pl_node_to_term@PLT");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_goal_str(IR_t * pBB) {
    (void)pBB;
    if (PLATFORM_X86) {
        if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — RESOLVE_CALL");
        if (MEDIUM_BINARY) {
            /* PLR-J-4a (2026-05-29): full MEDIUM_BINARY call protocol — byte twin of the MEDIUM_TEXT  */
            /* arm below.  Was a double-jump stub (jmp ω; jmp ω).  Emits: build each caller-side arg    */
            /* Term (build_arg_bin) and push; resolve_bb_env_save_push (push caller_env); align; bind callee  */
            /* slots from the stashed Terms; `call .Lplpred_<name>_<arity>` (cross-block forward ref,    */
            /* resolved against the callee block the SM_BB_PRED_INVOKE sweep defines via the same interned */
            /* bb_label_t*); test rt_last_ok → γ/ω with env install + CP caller-env save; β redo path.   */
            /* Internal labels (_fail5, β, _nosol) are pre-patched by hand; γ/ω + the two cross-block    */
            /* calls (block + _redo) go through the bb_bin_t descriptor / emit_label_intern.             */
            const char *callee = pBB && pBB->sval ? pBB->sval : "";
            int arity = 0;
            bb_goal_state_t *zc = NULL;
            if (pBB && pBB->ival) { zc = (bb_goal_state_t *)(intptr_t)pBB->ival; arity = zc->arity; }
            int n_args = (zc && zc->args) ? zc->nargs : 0;
            if (n_args > arity) n_args = arity;
            char blbl[160]; resolve_call_block_label(blbl, sizeof blbl, callee, arity);
            char redo_lbl[200]; snprintf(redo_lbl, sizeof redo_lbl, "%s_redo", blbl);
            bb_label_t *blk_lbl  = emit_label_intern(blbl);
            bb_label_t *redo_lab = emit_label_intern(redo_lbl);
            int nslots = arity + 16;
            int push_count = n_args + 1;
            int need_pad   = (push_count & 1) ? 1 : 0;
            std::string b;
            std::vector<int>          sites;
            std::vector<bb_label_t *> labels;
            std::vector<bool>         is_def;
            /* α: define this node's fresh-entry label (predecessor γ targets it). */
            sites.push_back((int)b.size()); labels.push_back(_.lbl_α_p); is_def.push_back(true);
            /* Phase 1: build each arg Term (caller env), push rax. */
            for (int i = 0; i < n_args; i++) {
                IR_t *a = (zc && zc->args) ? zc->args[i] : NULL;
                b += build_arg_bin(a);
                b += bytes(1, "\x50");                                   /* push rax */
            }
            /* Phase 2: resolve_bb_env_save_push(nslots); push caller_env (rax). */
            b += bytes(1, "\xBF") + u32le((uint32_t)nslots);            /* mov edi, nslots */
            b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)resolve_bb_env_save_push) + bytes(2, "\xFF\xD0");
            b += bytes(1, "\x50");                                       /* push rax (caller_env) */
            /* Alignment pad. */
            if (need_pad) b += bytes(4, "\x48\x83\xEC\x08");            /* sub rsp, 8 */
            /* Phase 3: bind callee slots from stashed Term*s. */
            for (int i = n_args - 1; i >= 0; i--) {
                int off = 8 * (need_pad + 1 + (n_args - 1 - i));
                /* mov rsi, [rsp + off] */
                if (off == 0)        b += bytes(4, "\x48\x8B\x34\x24");
                else if (off < 128)  { char d = (char)(uint8_t)off; b += bytes(4, "\x48\x8B\x74\x24") + std::string(&d, 1); }
                else                 b += bytes(4, "\x48\x8B\xB4\x24") + u32le((uint32_t)off);
                b += bytes(1, "\xBF") + u32le((uint32_t)i);             /* mov edi, i */
                b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)resolve_bb_bind_arg) + bytes(2, "\xFF\xD0");
            }
            /* Phase 4: fresh call → callee block (cross-block forward ref). */
            b += bytes(1, "\xE8");                                       /* call rel32 */
            sites.push_back((int)b.size()); labels.push_back(blk_lbl); is_def.push_back(false);
            b += u32le(0);
            /* Phase 5: restore stack. */
            if (need_pad) b += bytes(4, "\x48\x83\xC4\x08");            /* add rsp, 8 */
            b += bytes(1, "\x5F");                                       /* pop rdi (caller_env) */
            if (n_args > 0) b += bytes(3, "\x48\x83\xC4") + std::string(1, (char)(uint8_t)(8 * n_args)); /* add rsp, 8*n_args (n_args<16) */
            /* test rt_last_ok → je _fail5. */
            b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_last_ok) + bytes(2, "\xFF\xD0");
            b += bytes(2, "\x85\xC0");                                   /* test eax, eax */
            b += bytes(2, "\x0F\x84") + u32le(0);                        /* je rel32 → _fail5 (internal, hand-patched) */
            int off_je_fail5 = (int)b.size() - 4;
            /* SUCCESS: install caller_env; save into CP for β; jmp γ. */
            b += bytes(1, "\x57");                                       /* push rdi (caller_env survives the call) */
            b += bytes(4, "\x48\x83\xEC\x08");                          /* sub rsp, 8 (align) */
            b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)resolve_bb_env_install) + bytes(2, "\xFF\xD0"); /* rdi already=caller_env */
            b += bytes(4, "\x48\x83\xC4\x08");                          /* add rsp, 8 */
            b += bytes(1, "\x5F");                                       /* pop rdi (caller_env) */
            b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_pl_cp_save_caller_env) + bytes(2, "\xFF\xD0");
            b += bytes(1, "\xE9") + u32le(0);                            /* jmp γ */
            sites.push_back((int)b.size() - 4); labels.push_back(_.lbl_γ_p); is_def.push_back(false);
            /* _fail5: resolve_bb_env_pop(caller_env in rdi was popped above into rdi); jmp ω.               */
            /* NOTE: at _fail5 the caller_env is in rdi (popped at Phase-5 `pop rdi`).                    */
            int off_fail5 = (int)b.size();
            b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)resolve_bb_env_pop) + bytes(2, "\xFF\xD0");
            b += bytes(1, "\xE9") + u32le(0);                            /* jmp ω */
            sites.push_back((int)b.size() - 4); labels.push_back(_.lbl_ω_p); is_def.push_back(false);
            /* β: redo entry. */
            sites.push_back((int)b.size()); labels.push_back(_.lbl_β_p); is_def.push_back(true);
            b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)resolve_cp_current) + bytes(2, "\xFF\xD0");
            b += bytes(3, "\x48\x85\xC0");                               /* test rax, rax */
            b += bytes(2, "\x0F\x84") + u32le(0);                        /* je → _nosol (hand-patched) */
            int off_je_nosol1 = (int)b.size() - 4;
            b += bytes(4, "\x48\x8B\x78\x18");                          /* mov rdi, [rax + 24] (cp->env) */
            b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)resolve_bb_env_install) + bytes(2, "\xFF\xD0");
            b += bytes(1, "\xE8");                                       /* call redo_lbl (cross-block) */
            sites.push_back((int)b.size()); labels.push_back(redo_lab); is_def.push_back(false);
            b += u32le(0);
            b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_last_ok) + bytes(2, "\xFF\xD0");
            b += bytes(2, "\x85\xC0");                                   /* test eax, eax */
            b += bytes(2, "\x0F\x84") + u32le(0);                        /* je → _nosol (hand-patched) */
            int off_je_nosol2 = (int)b.size() - 4;
            b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)resolve_cp_current) + bytes(2, "\xFF\xD0");
            b += bytes(4, "\x48\x8B\x78\x28");                          /* mov rdi, [rax + 40] (cp->saved_args) */
            b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)resolve_bb_env_install) + bytes(2, "\xFF\xD0");
            b += bytes(1, "\xE9") + u32le(0);                            /* jmp γ */
            sites.push_back((int)b.size() - 4); labels.push_back(_.lbl_γ_p); is_def.push_back(false);
            /* _nosol: jmp ω. */
            int off_nosol = (int)b.size();
            b += bytes(1, "\xE9") + u32le(0);                            /* jmp ω */
            sites.push_back((int)b.size() - 4); labels.push_back(_.lbl_ω_p); is_def.push_back(false);
            /* Hand-patch the three internal rel32s (targets are intra-blob, known now). */
            { int32_t r = off_fail5  - (off_je_fail5  + 4); memcpy(const_cast<char*>(b.data()) + off_je_fail5,  &r, 4); }
            { int32_t r = off_nosol  - (off_je_nosol1 + 4); memcpy(const_cast<char*>(b.data()) + off_je_nosol1, &r, 4); }
            { int32_t r = off_nosol  - (off_je_nosol2 + 4); memcpy(const_cast<char*>(b.data()) + off_je_nosol2, &r, 4); }
            bb_bin_t bin; bin.sites = sites; bin.labels = labels; bin.is_def = is_def; bin.bytes = b;
            bb_emit_asm_result(b, bin);
            return std::string();
        }
        if (MEDIUM_TEXT) {
            const char *callee = pBB && pBB->sval ? pBB->sval : "";
            int arity = 0;
            bb_goal_state_t *zc = NULL;
            if (pBB && pBB->ival) {
                zc = (bb_goal_state_t *)(intptr_t)pBB->ival;
                arity = zc->arity;
            }
            int n_args = (zc && zc->args) ? zc->nargs : 0;
            if (n_args > arity) n_args = arity;
            char blbl[160]; resolve_call_block_label(blbl, sizeof blbl, callee, arity);
            char redo_lbl[200]; snprintf(redo_lbl, sizeof redo_lbl, "%s_redo", blbl);
            std::string out = s_1asm(emit_fmt("%s:", _.lbl_α))
                            + s_comment(emit_fmt("# BOX RESOLVE_CALL %s/%d (WAM-CP-5, n_args=%d)", callee, arity, n_args));
            /* Phase 1: build caller-side arg Terms via build_arg (handles BB_STRUCT compound),    */
            /* push each on stack.  emit_build_compound_term may sub/add rsp internally but restores  */
            /* it; rax = Term* on exit from each build_arg call.                                       */
            for (int i = 0; i < n_args; i++) {
                IR_t *a = (zc && zc->args) ? zc->args[i] : NULL;
                out += build_arg(a)
                     + s_2asm("push", "rax");
            }
            /* Phase 2: push callee env.  resolve_bb_env_save_push allocates callee env, sets g_resolve_env,    */
            /* returns caller_env in rax.  Push caller_env on stack.                                   */
            int nslots = arity + 16;
            out += s_2asm("mov", emit_fmt("edi, %d", nslots))
                 + s_2asm("call", "resolve_bb_env_save_push@PLT")
                 + s_2asm("push", "rax");
            /* Alignment: (n_args+1) qwords pushed since frame entry.  Pad if odd.                   */
            int push_count = n_args + 1;
            int need_pad   = (push_count & 1) ? 1 : 0;
            if (need_pad) out += s_2asm("sub", "rsp, 8");
            /* Phase 3: bind callee slots.  Arg-alias bindings made here survive redo because CHOICE  */
            /* pre[i>0] only unwinds back to CHOICE's own trail mark (taken after these bindings).     */
            for (int i = n_args - 1; i >= 0; i--) {
                int off = 8 * (need_pad + 1 + (n_args - 1 - i));
                out += s_2asm("mov", emit_fmt("rsi, [rsp + %d]", off))
                     + s_2asm("mov", emit_fmt("edi, %d", i))
                     + s_2asm("call", "resolve_bb_bind_arg@PLT");
            }
            /* Phase 4: fresh call. */
            out += s_2asm("call", blbl);
            /* Phase 5: restore stack, test last_ok. */
            if (need_pad) out += s_2asm("add", "rsp, 8");
            out += s_2asm("pop",  "rdi");
            if (n_args > 0) out += s_2asm("add", emit_fmt("rsp, %d", 8 * n_args));
            out += s_2asm("call", "rt_last_ok@PLT")
                 + s_2asm("test", "eax, eax")
                 + s_2asm("je",   emit_fmt("%s_fail5", _.lbl_α));
            /* SUCCESS: restore caller_env; save into CHOICE's CP for beta. */
            out += s_2asm("call", "resolve_bb_env_install@PLT")
                 + s_2asm("mov",  "rdi, rax")
                 + s_2asm("call", "rt_pl_cp_save_caller_env@PLT")
                 + s_2asm("jmp",  _.lbl_γ);
            /* fail5: free callee env; CHOICE already popped its CP on exhausted path. */
            out += s_L1asm(emit_fmt("%s_fail5:", _.lbl_α), "")
                 + s_2asm("call", "resolve_bb_env_pop@PLT")
                 + s_2asm("jmp",  _.lbl_ω);
            /* beta: redo.  g_resolve_bfr = CHOICE's CP.  Reinstall callee_env (cp->env=24).              */
            /* Call _redo — no trail_unwind: CHOICE pre[i>0] handles it.                              */
            out += s_1asm(emit_fmt("%s:", _.lbl_β))
                 + s_2asm("call", "resolve_cp_current@PLT")
                 + s_2asm("test", "rax, rax")
                 + s_2asm("je",   emit_fmt("%s_nosol", _.lbl_α))
                 + s_2asm("mov",  "rdi, [rax + 24]")
                 + s_2asm("call", "resolve_bb_env_install@PLT")
                 + s_2asm("call", redo_lbl)
                 + s_2asm("call", "rt_last_ok@PLT")
                 + s_2asm("test", "eax, eax")
                 + s_2asm("je",   emit_fmt("%s_nosol", _.lbl_α))
                 + s_2asm("call", "resolve_cp_current@PLT")
                 + s_2asm("mov",  "rdi, [rax + 40]")
                 + s_2asm("call", "resolve_bb_env_install@PLT")
                 + s_2asm("jmp",  _.lbl_γ);
            out += s_L2asm(emit_fmt("%s_nosol:", _.lbl_α), "jmp", _.lbl_ω);
            return out;
        }
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_goal(IR_t * pBB) {
    std::string out = bb_goal_str(pBB);
    if (!out.empty()) emit_text_n(out.data(), out.size());
}
