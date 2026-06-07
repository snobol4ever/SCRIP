#include <string>
#include <vector>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
#include "emit_core.h"
#include "IR.h"
#include "IR_interp_state.h"
}
#include "x86_asm.h"
extern std::string emit_build_compound_term(const IR_t *nd);
extern "C" {
void  *rt_node_to_term(int kind, long ival, const char *sval, double dval);
void **resolve_bb_env_save_push(int nslots);
void   resolve_bb_bind_arg(int slot, void *caller_term);
int    rt_last_ok(void);
void **resolve_bb_env_install(void **env);
void   rt_cp_save_caller_env(void *caller_env);
void   resolve_bb_env_pop(void **saved);
void  *resolve_cp_current(void);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string build_arg(IR_t *a) {
    if (!a) return x86("ins2", "xor", "eax, eax");
    if (a->t == IR_STRUCT) return emit_build_compound_term(a);
    int kind = (int)a->t;
    long ival = (long)IR_LIT(a).ival;
    const char *sval = IR_LIT(a).sval;
    char slbl[64]; slbl[0] = 0;
    if (sval && *sval) strtab_label(slbl, sizeof slbl, sval);
    return x86("ins2", "mov", emit_fmt("edi, %d", kind))
         + x86("ins2", "mov", emit_fmt("rsi, %ld", ival))
         + (slbl[0] ? x86("ins2", "lea", emit_fmt("rdx, [rip + %s]", slbl))
                    : x86("ins2", "xor", "edx, edx"))
         + x86("ins2", "xorps", "xmm0, xmm0")
         + x86("ins2", "call", "rt_node_to_term@PLT");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_goal_str(IR_t * pBB) {
    (void)pBB;
    if (PLATFORM_X86) {
        if (MEDIUM_MACRO_DEF) return x86("comment", "no macro form — RESOLVE_CALL");
        if (MEDIUM_TEXT) {
            const char *callee = pBB && _.op_sval ? _.op_sval : "";
            int arity = 0;
            bb_goal_state_t *zc = NULL;
            if (pBB && _.op_ival) {
                zc = (bb_goal_state_t *)(intptr_t)_.op_ival;
                arity = zc->arity;
            }
            int n_args = (zc && zc->args) ? zc->nargs : 0;
            if (n_args > arity) n_args = arity;
            char blbl[160]; resolve_call_block_label(blbl, sizeof blbl, callee, arity);
            char redo_lbl[200]; snprintf(redo_lbl, sizeof redo_lbl, "%s_redo", blbl);
            std::string out = x86("label", _.lbl_α)
                            + x86("comment", emit_fmt("BOX RESOLVE_CALL %s/%d (n_args=%d)", callee, arity, n_args));
            for (int i = 0; i < n_args; i++) {
                IR_t *a = (zc && zc->args) ? zc->args[i] : NULL;
                out += build_arg(a)
                     + x86("ins2", "push", "rax");
            }
            int nslots = arity + 16;
            out += x86("ins2", "mov", emit_fmt("edi, %d", nslots))
                 + x86("ins2", "call", "resolve_bb_env_save_push@PLT")
                 + x86("ins2", "push", "rax");
            int push_count = n_args + 1;
            int need_pad   = (push_count & 1) ? 1 : 0;
            if (need_pad) out += x86("ins2", "sub", "rsp, 8");
            for (int i = n_args - 1; i >= 0; i--) {
                int off = 8 * (need_pad + 1 + (n_args - 1 - i));
                out += x86("ins2", "mov", emit_fmt("rsi, [rsp + %d]", off))
                     + x86("ins2", "mov", emit_fmt("edi, %d", i))
                     + x86("ins2", "call", "resolve_bb_bind_arg@PLT");
            }
            out += x86("ins2", "call", blbl);
            if (need_pad) out += x86("ins2", "add", "rsp, 8");
            out += x86("ins2", "pop",  "rdi");
            if (n_args > 0) out += x86("ins2", "add", emit_fmt("rsp, %d", 8 * n_args));
            out += x86("ins2", "call", "rt_last_ok@PLT")
                 + x86("ins2", "test", "eax, eax")
                 + x86("ins2", "je",   emit_fmt("%s_fail5", _.lbl_α));
            out += x86("ins2", "call", "resolve_bb_env_install@PLT")
                 + x86("ins2", "mov",  "rdi, rax")
                 + x86("ins2", "call", "rt_cp_save_caller_env@PLT")
                 + x86("ins2", "jmp",  _.lbl_γ);
            out += x86("Lins1", emit_fmt("%s_fail5:", _.lbl_α), "")
                 + x86("ins2", "call", "resolve_bb_env_pop@PLT")
                 + x86("ins2", "jmp",  _.lbl_ω);
            out += x86("label", _.lbl_β)
                 + x86("ins2", "call", "resolve_cp_current@PLT")
                 + x86("ins2", "test", "rax, rax")
                 + x86("ins2", "je",   emit_fmt("%s_nosol", _.lbl_α))
                 + x86("ins2", "mov",  "rdi, [rax + 24]")
                 + x86("ins2", "call", "resolve_bb_env_install@PLT")
                 + x86("ins2", "call", redo_lbl)
                 + x86("ins2", "call", "rt_last_ok@PLT")
                 + x86("ins2", "test", "eax, eax")
                 + x86("ins2", "je",   emit_fmt("%s_nosol", _.lbl_α))
                 + x86("ins2", "call", "resolve_cp_current@PLT")
                 + x86("ins2", "mov",  "rdi, [rax + 40]")
                 + x86("ins2", "call", "resolve_bb_env_install@PLT")
                 + x86("ins2", "jmp",  _.lbl_γ);
            out += x86("Lins2", emit_fmt("%s_nosol:", _.lbl_α), "jmp", _.lbl_ω);
            return out;
        }
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_goal(IR_t * pBB) { bb_emit_x86(bb_goal_str(pBB)); }
