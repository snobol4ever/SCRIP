/* bb_call.cpp — BB template for IR_CALL (generic call/builtin BB, language-ignorant).
   IBB GROUND-ZERO RESET (Opus 4.7, 2026-05-28, Lon directive).

   Supported shape only: write(string_literal). Everything else ABORTs.

   MEDIUM_TEXT (mode 4): emit asm directives + lea rdi,[rip+str]; call rt_write_str_nl@PLT.
   MEDIUM_BINARY (mode 3): emit raw x86 in the bb_pool slab. String address and runtime
   function address are both in-process — we use 64-bit absolute loads (movabs).

   Bytes (32 total, including β tail):
       offset 0:  48 BF <8-byte str_addr>          ; movabs rdi, <str_addr>
       offset 10: BE <4-byte len>                  ; mov esi, <len>
       offset 15: 48 B8 <8-byte fn_addr>           ; movabs rax, &rt_write_str_nl
       offset 25: FF D0                            ; call rax
       offset 27: E9 <4-byte rel32>                ; jmp lbl_γ
       offset 32: (lbl_β defined here)             ; β-entry
       offset 32: E9 <4-byte rel32>                ; jmp lbl_ω

   Patch list (bin):
       offset 0    : lbl_α DEFINED (label at start of slab)
       offset 28   : rel32 to lbl_γ
       offset 32   : lbl_β DEFINED
       offset 33   : rel32 to lbl_ω
*/
#include <string>
#include <string.h>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
#include "../../runtime/interp/gen.h"
void rt_write_str_nl(const char *s, uint32_t slen);
void rt_write_int_nl(int64_t v);
void rt_write_any_nl(DESCR_t d);
void rt_write_strz_nl(const char *s);
void rt_pop_write_int_nl(void);
void rt_pop_write_any_nl(void);
void rt_call_proc(const char *name, int nargs);
DESCR_t rt_icn_call_proc_descr(const char *name, int nargs);
int  rt_proc_is_registered(const char *name);
void rt_call_builtin(const char *name, int nargs);
int  rt_builtin_is_known(const char *name);
int  bb_slot_get(IR_t * nd);
int  bb_slot_alloc16(IR_t * nd);
int  bb_varslot(const char * name);
DESCR_t rt_rk_call_arr(const char * fn, DESCR_t * args, int nargs);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_call_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (!PLATFORM_X86) return std::string();
    if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — IR_CALL");

    const char * fn   = pBB->sval ? pBB->sval : "";
    int64_t      narg = pBB->ival;
    IR_t       * a0   = pBB->α;

    /* RK-EMIT-2 (2026-05-31): general deterministic builtin call (dval==2.0). v_raku_det_call lowers       */
    /* `fn(arg0,arg1,...)` (Raku __rk_arr / elems / sort / __rk_jct_* / hash_get / arr_get / …) to ONE       */
    /* IR_CALL whose nargs ARGUMENTS are isolated value sub-graphs on `counter` (NOT on the γ-chain). The    */
    /* mode-2 oracle runs each sub-graph via bb_exec_once; modes 3/4 cannot walk Byrd boxes at run time      */
    /* (FACT RULE), so this box MATERIALISES each argument's 16-byte DESCR into a per-call vector in its OWN  */
    /* ζ frame region (the ARBNO-style per-activation array the no-value-stack FACT RULE permits — never a   */
    /* global value stack, never a name-table round-trip) and calls the by-array dispatcher rt_rk_call_arr,  */
    /* which is the SAME try_call_builtin_by_name the oracle uses ⇒ m2==m3==m4. The result DESCR (rax:rdx)   */
    /* is stored into this node's own slot [r12+resoff] so the consumer (say/assign) reads it via            */
    /* bb_slot_get(this). Each arg sub-graph's entry must be a SIMPLE leaf (literal / variable); a nested or  */
    /* generator arg sub-graph is NOT handled here (this arm declines so the loud abort below fires — never   */
    /* a silent wrong emit). Variable args are read from their ζ varslot (the Icon flat model home), NOT the */
    /* name table, so the value matches what IR_ASSIGN stored.                                                */
    if (g_icn_flat_chain && pBB->dval == 2.0) {
        IR_graph_t ** subs = (IR_graph_t **)(intptr_t) pBB->counter;
        int leaves_ok = 1;
        for (int i = 0; i < (int)narg; i++) {
            IR_t * lf = (subs && subs[i]) ? subs[i]->entry : NULL;
            if (!lf) { leaves_ok = 0; break; }
            if (lf->t != IR_LIT_I && lf->t != IR_LIT_S && lf->t != IR_LIT_F
                && lf->t != IR_LIT_NUL && lf->t != IR_VAR) { leaves_ok = 0; break; }
        }
        if (leaves_ok) {
            int resoff  = bb_slot_alloc16(pBB);
            int argbase = (narg > 0) ? bb_slot_alloc16(subs[0]->entry) : resoff;
            for (int i = 1; i < (int)narg; i++) bb_slot_alloc16(subs[i]->entry);
            if (MEDIUM_TEXT) {
                std::string s = s_1asm(emit_fmt("%s:", _.lbl_α))
                    + s_comment(emit_fmt("# BOX IR_CALL %s(...) [RK-EMIT-2 dval=2 marshalled args -> rt_rk_call_arr]", fn));
                for (int i = 0; i < (int)narg; i++) {
                    IR_t * lf = subs[i]->entry; int aoff = argbase + i * 16;
                    if (lf->t == IR_LIT_I) {
                        s += s_2asm("mov", emit_fmt("qword ptr [r12+%d], 6", aoff));
                        s += s_2asm("movabs", emit_fmt("rax, %lld", (long long)lf->ival));
                        s += s_2asm("mov", emit_fmt("[r12+%d], rax", aoff + 8));
                    } else if (lf->t == IR_LIT_F) {
                        uint64_t bits; double d = lf->dval; memcpy(&bits, &d, 8);
                        s += s_2asm("mov", emit_fmt("qword ptr [r12+%d], 7", aoff));
                        s += s_2asm("movabs", emit_fmt("rax, %llu", (unsigned long long)bits));
                        s += s_2asm("mov", emit_fmt("[r12+%d], rax", aoff + 8));
                    } else if (lf->t == IR_LIT_NUL) {
                        s += s_2asm("mov", emit_fmt("qword ptr [r12+%d], 0", aoff));
                        s += s_2asm("mov", emit_fmt("qword ptr [r12+%d], 0", aoff + 8));
                    } else if (lf->t == IR_LIT_S) {
                        std::string sl = emit_fmt(".Lrkarg%d_%d", bb_node_id(pBB), i);
                        s += s_directive(".section .rodata")
                           + s_directive(sl + ": .string \"" + (lf->sval ? lf->sval : "") + "\"")
                           + s_directive(".section .text") + s_directive(".intel_syntax noprefix");
                        s += s_2asm("mov", emit_fmt("qword ptr [r12+%d], 1", aoff));
                        s += s_2asm("lea", emit_fmt("rax, [rip+%s]", sl.c_str()));
                        s += s_2asm("mov", emit_fmt("[r12+%d], rax", aoff + 8));
                    } else { /* IR_VAR — read its ζ varslot */
                        int voff = bb_varslot(lf->sval ? lf->sval : "");
                        s += s_2asm("mov", emit_fmt("rax, [r12+%d]", voff));
                        s += s_2asm("mov", emit_fmt("[r12+%d], rax", aoff));
                        s += s_2asm("mov", emit_fmt("rax, [r12+%d]", voff + 8));
                        s += s_2asm("mov", emit_fmt("[r12+%d], rax", aoff + 8));
                    }
                }
                std::string fl = emit_fmt(".Lrkfn%d", bb_node_id(pBB));
                s += s_directive(".section .rodata")
                   + s_directive(fl + ": .string \"" + fn + "\"")
                   + s_directive(".section .text") + s_directive(".intel_syntax noprefix");
                s += s_2asm("lea", emit_fmt("rdi, [rip+%s]", fl.c_str()));
                s += s_2asm("lea", emit_fmt("rsi, [r12+%d]", argbase));
                s += s_2asm("mov", emit_fmt("edx, %lld", (long long)narg));
                s += s_2asm("call", "rt_rk_call_arr@PLT");
                s += s_2asm("mov", emit_fmt("[r12+%d], rax", resoff));
                s += s_2asm("mov", emit_fmt("[r12+%d], rdx", resoff + 8));
                s += s_2asm("jmp", _.lbl_γ);
                s += s_L1asm(emit_fmt("%s:", _.lbl_β), "");
                s += s_2asm("jmp", _.lbl_ω);
                return s;
            }
            if (MEDIUM_BINARY) {
                std::string s;
                for (int i = 0; i < (int)narg; i++) {
                    IR_t * lf = subs[i]->entry; uint32_t aoff = (uint32_t)(argbase + i * 16);
                    if (lf->t == IR_LIT_I || lf->t == IR_LIT_F || lf->t == IR_LIT_NUL || lf->t == IR_LIT_S) {
                        uint64_t tag = (lf->t == IR_LIT_I) ? 6 : (lf->t == IR_LIT_F) ? 7 : (lf->t == IR_LIT_S) ? 1 : 0;
                        s += bytes(4, "\x49\xC7\x84\x24") + u32le(aoff) + u32le((uint32_t)tag);   /* mov qword [r12+aoff], tag */
                        if (lf->t == IR_LIT_NUL) {
                            s += bytes(4, "\x49\xC7\x84\x24") + u32le(aoff + 8) + u32le(0);        /* mov qword [r12+aoff+8], 0 */
                        } else {
                            uint64_t v;
                            if (lf->t == IR_LIT_I)      v = (uint64_t)lf->ival;
                            else if (lf->t == IR_LIT_F) { double d = lf->dval; memcpy(&v, &d, 8); }
                            else                        v = (uint64_t)(uintptr_t)(lf->sval ? lf->sval : "");
                            s += bytes(2, "\x48\xB8") + u64le(v);                                   /* movabs rax, v */
                            s += bytes(4, "\x49\x89\x84\x24") + u32le(aoff + 8);                    /* mov [r12+aoff+8], rax */
                        }
                    } else { /* IR_VAR */
                        uint32_t voff = (uint32_t) bb_varslot(lf->sval ? lf->sval : "");
                        s += bytes(4, "\x49\x8B\x84\x24") + u32le(voff)     + bytes(4, "\x49\x89\x84\x24") + u32le(aoff);
                        s += bytes(4, "\x49\x8B\x84\x24") + u32le(voff + 8) + bytes(4, "\x49\x89\x84\x24") + u32le(aoff + 8);
                    }
                }
                uint64_t nptr = (uint64_t)(uintptr_t) fn;
                uint64_t fptr; { DESCR_t (*fp)(const char *, DESCR_t *, int) = rt_rk_call_arr; fptr = (uint64_t)(uintptr_t)(void*)fp; }
                s += bytes(2, "\x48\xBF") + u64le(nptr);                          /* movabs rdi, fn        */
                s += bytes(4, "\x49\x8D\xB4\x24") + u32le((uint32_t)argbase);     /* lea rsi, [r12+argbase] */
                s += bytes(1, "\xBA") + u32le((uint32_t)narg);                    /* mov edx, narg         */
                s += bytes(2, "\x48\xB8") + u64le(fptr);                          /* movabs rax, &fn       */
                s += bytes(2, "\xFF\xD0");                                        /* call rax              */
                s += bytes(4, "\x49\x89\x84\x24") + u32le((uint32_t)resoff);      /* mov [r12+resoff], rax */
                s += bytes(4, "\x49\x89\x94\x24") + u32le((uint32_t)(resoff + 8));/* mov [r12+resoff+8],rdx*/
                size_t base = s.size();
                bin = { { (int)(base + 1), (int)(base + 5), (int)(base + 6) },
                        { _.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p }, { false, true, false } };
                s += bytes(1, "\xE9") + u32le(0);                                 /* jmp γ   (patch base+1) */
                s += bytes(1, "\xE9") + u32le(0);                                 /* β: jmp ω (def base+5, patch base+6) */
                return s;
            }
        }
    }

    /* GZ-10 (modes 3/4, GROUND ZERO 3) flat-chain user-procedure call: `f(...)` where f is a registered  */
    /* Icon procedure (dval==3.0). The stackless (ζζ,entry) model (test_sno_3.c): the call box calls the   */
    /* C helper rt_icn_call_proc_descr, which allocates a FRESH per-activation frame from the depth-indexed */
    /* arena (recursion-safe — NOT a value stack), binds the staged args into the callee's param slots,    */
    /* runs the callee's flat slab (built with the return-slot-at-[0] / params-at-[16(i+1)] convention),   */
    /* and returns the callee's RETURN-slot DESCR in rax:rdx. This box stores that DESCR into its OWN frame */
    /* result slot [r12+off] so a consumer (write/binop) reads it by slot — NO value stack, NO ring. Args  */
    /* (when narg>0) are staged into g_icn_call_args by reading each arg producer's slot just before the    */
    /* call; the 0-arg case (this rung) needs no staging. β re-pump target recovered from the EMIT_PAIR.   */
    if (g_icn_flat_chain && fn && rt_proc_is_registered(fn) && pBB->dval == 3.0) {
        int off = bb_slot_alloc16(pBB);
        bb_label_t *beta_tgt = _.lbl_ω_p;
        for (int i = 0; i < g_emit.xa_bb_emit_pair_n; i++)
            if (g_emit.xa_bb_emit_pair_define[i] == _.lbl_β_p && g_emit.xa_bb_emit_pair_jmp[i]) { beta_tgt = g_emit.xa_bb_emit_pair_jmp[i]; break; }
        if (MEDIUM_BINARY) {
            /*   0  : 48 BF <u64 name>      movabs rdi, name                                                */
            /*  10  : BE <u32 nargs>        mov esi, nargs                                                  */
            /*  15  : 48 B8 <u64 fptr>      movabs rax, &rt_icn_call_proc_descr                             */
            /*  25  : FF D0                 call rax                  (returns DESCR in rax:rdx)            */
            /*  27  : 49 89 84 24 <u32 off> mov [r12+off], rax        (result eightbyte0)                   */
            /*  35  : 49 89 94 24 <u32 o+8> mov [r12+off+8], rdx      (result eightbyte1)                   */
            /*  43  : E9 <γ_rel32>          jmp γ                     ← γ patch at 44                       */
            /*  48  : E9 <β_rel32>          β: jmp β-target           ← β-def 48, tgt patch at 49           */
            /*  53  : end                                                                                   */
            uint64_t nptr = (uint64_t)(uintptr_t)fn;
            uint64_t fptr; { DESCR_t (*fp)(const char *, int) = rt_icn_call_proc_descr; fptr = (uint64_t)(uintptr_t)(void*)fp; }
            bin = { {44, 48, 49}, {_.lbl_γ_p, _.lbl_β_p, beta_tgt}, {false, true, false} };
            return bytes(2, "\x48\xBF") + u64le(nptr)
                 + bytes(1, "\xBE")     + u32le((uint32_t)narg)
                 + bytes(2, "\x48\xB8") + u64le(fptr)
                 + bytes(2, "\xFF\xD0")
                 + bytes(4, "\x49\x89\x84\x24") + u32le((uint32_t)off)
                 + bytes(4, "\x49\x89\x94\x24") + u32le((uint32_t)(off + 8))
                 + bytes(1, "\xE9")     + u32le(0)
                 + bytes(1, "\xE9")     + u32le(0);
        }
        if (MEDIUM_TEXT) {
            /* mode-4 standalone: the proc slab + its name-keyed registration are NOT yet emitted into the  */
            /* compiled binary (the name is an in-scrip AST pointer here), so a user-proc call cannot work  */
            /* in --compile yet. Emit a loud abort so mode-4 FAILS visibly (tracked; floor 0) rather than   */
            /* calling through a stale pointer. GZ-10 mode-4 (proc-slab emission + startup registration) is */
            /* the next rung.                                                                                */
            return s_1asm(emit_fmt("%s:", _.lbl_α))
                 + s_comment(emit_fmt("# BOX IR_CALL %s(...) [GZ-10 user-proc — mode-4 not yet wired]", fn))
                 + s_2asm("call", "abort@PLT")
                 + s_2asm("jmp",  _.lbl_γ)
                 + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
                 + s_2asm("jmp",  beta_tgt ? beta_tgt->name : _.lbl_ω);
        }
    }

    /* GZ-7 (GROUND ZERO 3) flat-chain slot model: write(E) where E is any single producer box already   */
    /* emitted earlier in the chain (it wrote its 16-byte DESCR result into [r12+off_E]). This box reads  */
    /* that slot by value into rdi:rsi (DESCR eightbyte0 + eightbyte1) and calls the by-value             */
    /* rt_write_any_nl, which dispatches on the DESCR type tag (int/real/string). NO value stack, NO ring */
    /* — the operand IS its slot (test_sno_1.c `write_str(out, seq)` reading seq's slot). The β port      */
    /* re-pump target is recovered from the queued EMIT_PAIR (the arg generator's resume) as elsewhere.   */
    if (g_icn_flat_chain && fn && (!strcmp(fn, "write")) && narg == 1 && a0) {
        int off = bb_slot_get(a0);
        if (off >= 0) {
            bb_label_t *beta_tgt = _.lbl_ω_p;
            for (int i = 0; i < g_emit.xa_bb_emit_pair_n; i++)
                if (g_emit.xa_bb_emit_pair_define[i] == _.lbl_β_p && g_emit.xa_bb_emit_pair_jmp[i]) { beta_tgt = g_emit.xa_bb_emit_pair_jmp[i]; break; }
            if (MEDIUM_BINARY) {
                uint64_t fptr; { void (*fp)(DESCR_t) = rt_write_any_nl; fptr = (uint64_t)(uintptr_t)(void*)fp; }
                /*   0   49 8B BC 24 <u32 off>     mov rdi,[r12+off]    (DESCR eightbyte0: v|slen)            */
                /*   8   49 8B B4 24 <u32 off+8>   mov rsi,[r12+off+8]  (DESCR eightbyte1: int/ptr)           */
                /*  16   48 B8 <u64 fptr>          movabs rax,&rt_write_any_nl                                */
                /*  26   FF D0                     call rax                                                   */
                /*  28   E9 <rel32 → γ>            jmp γ                ← γ patch at 29                       */
                /*  33   E9 <rel32 → β-tgt>        β: jmp β-tgt         ← β-def 33, tgt patch at 34           */
                /*  38   end                                                                                  */
                bin = { {29, 33, 34}, {_.lbl_γ_p, _.lbl_β_p, beta_tgt}, {false, true, false} };
                return bytes(4, "\x49\x8B\xBC\x24") + u32le((uint32_t)off)
                     + bytes(4, "\x49\x8B\xB4\x24") + u32le((uint32_t)(off + 8))
                     + bytes(2, "\x48\xB8")         + u64le(fptr)
                     + bytes(2, "\xFF\xD0")
                     + bytes(1, "\xE9")             + u32le(0)
                     + bytes(1, "\xE9")             + u32le(0);
            }
            if (MEDIUM_TEXT) {
                return s_1asm(emit_fmt("%s:", _.lbl_α))
                     + s_comment("# BOX IR_CALL write(operand) [GZ-7 flat-chain operand slot → rt_write_any_nl]")
                     + s_2asm("mov", emit_fmt("rdi, [r12+%d]", off))
                     + s_2asm("mov", emit_fmt("rsi, [r12+%d]", off + 8))
                     + s_2asm("call", "rt_write_any_nl@PLT")
                     + s_2asm("jmp", _.lbl_γ)
                     + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
                     + s_2asm("jmp", beta_tgt ? beta_tgt->name : _.lbl_ω);
            }
        }
    }

    int is_write_strlit  = (fn && !strcmp(fn, "write") && narg == 1 && a0 && a0->t == IR_LIT_S && a0->sval);
    /* IBB-3 (2026-05-28): write(int_expr) shape — arg0 was already evaluated by                    */
    /* flat_drive_call_intexpr (which walked it before defining this template's α). The expected     */
    /* runtime state: an int DESCR_t is on top of the vstack. Trailer: rt_pop_write_int_nl + jmp γ.   */
    /* IBB-4 (2026-05-28): extended to generator-leaf arg0 kinds IR_TO and IR_ALT, which          */
    /* push int values onto the vstack the same way under their MEDIUM_BINARY arms (rt_push_int).    */
    /* IBB-6 (2026-05-28): extended to IR_BINOP_GEN (cross-product generator; driver walks both     */
    /* operand sub-trees, applies rt_arith/rt_acomp, pushes result onto vstack before trailer).     */
    /* IBB-7 (2026-05-29): extended to IR_VAR (variable read; bb_var.cpp pushes value via         */
    /* rt_nv_get). Variables can hold strings/reals, so the trailer below switches to             */
    /* rt_pop_write_any_nl when arg0 is IR_VAR.                                                     */
    /* IBB-9-UNOP (2026-05-29, Opus 4.8): value-producing unary ops as write arg0 — write(-x),        */
    /* write(\x), write(/x), write(not E).  flat_drive_call_intexpr walks the unop (which walks its    */
    /* own operand, applies rt_unop_*, leaves the result on the vstack) before this trailer.  These    */
    /* yield any DESCR kind (int/real/string/null), so they route through rt_pop_write_any_nl below.   */
    int is_write_intexpr = (fn && !strcmp(fn, "write") && narg == 1 && a0 &&
                            (a0->t == IR_BINOP || a0->t == IR_LIT_I || a0->t == IR_TO || a0->t == IR_TO_BY || a0->t == IR_ALT || a0->t == IR_BINOP_GEN || a0->t == IR_VAR ||
                             a0->t == IR_NEG || a0->t == IR_POS || a0->t == IR_NONNULL || a0->t == IR_NULL_TEST || a0->t == IR_NOT || a0->t == IR_SIZE || a0->t == IR_CALL || a0->t == IR_CASE || a0->t == IR_FIELD_GET || a0->t == IR_LIST_BANG || a0->t == IR_LIMIT || a0->t == IR_IDX));
    /* IBB-8 (2026-05-29): IR_BINOP and IR_BINOP_GEN can yield DT_R (e.g. Icon `^` always reals,    */
    /* mixed int/real arith). The int-write trailer prints raw IEEE bits for a double, so route them */
    /* through rt_pop_write_any_nl which inspects the DESCR_t kind at runtime (DT_I via %lld exactly */
    /* as before, DT_R via canonical real_str, DT_S via %.*s). Pure-int producers are unaffected     */
    /* except for one extra runtime type-tag compare. IR_TO/IR_ALT/IR_LIT_I stay int-only.           */
    /* IBB (2026-05-29, Opus 4.8): IR_ALT can yield non-int values (alt of string/real literals, e.g.   */
    /* `write("a"|"b"|"c")`).  The int-write trailer prints a string descriptor's raw pointer bits as   */
    /* an integer (garbage); route alts through rt_pop_write_any_nl, which dispatches on the DESCR kind  */
    /* at runtime (DT_I → %lld exactly as before, so int-alts are unaffected).                            */
    int arg_is_any = (a0 && (a0->t == IR_VAR || a0->t == IR_BINOP || a0->t == IR_BINOP_GEN || a0->t == IR_ALT ||
                             a0->t == IR_NEG || a0->t == IR_POS || a0->t == IR_NONNULL || a0->t == IR_NULL_TEST || a0->t == IR_NOT || a0->t == IR_SIZE ||
                             a0->t == IR_CALL || a0->t == IR_CASE || a0->t == IR_FIELD_GET || a0->t == IR_LIST_BANG || a0->t == IR_LIMIT || a0->t == IR_IDX ||
                             (a0->t == IR_TO_BY && a0->sval && a0->sval[0] == 'r')));

    /* IBB-9-6 (Opus 4.8, 2026-05-29): user-procedure call. fn names a procedure registered by the   */
    /* mode-3 driver (rt_proc_register). The flat driver (flat_drive_call_userproc) walked the    */
    /* arg γ-chain, leaving nargs values on the vstack (arg0 deepest). Trailer: load name + nargs into */
    /* the SysV arg registers and call rt_call_proc, which binds params, invokes the callee slab,  */
    /* and leaves the return value on the vstack. Then jmp γ. This is the JCON ir_a_Call invoke step.  */
    /* Checked BEFORE the write-shape arms only matters for a bare `proc(...)` statement; write(proc())*/
    /* routes here too because the inner IR_CALL is a0 (handled via the userproc arm of arg_is_any +    */
    /* the write trailer reading the pushed return value).                                             */
    int is_userproc = (fn && rt_proc_is_registered(fn) && !is_write_strlit && !is_write_intexpr);
    if (is_userproc) {
        if (MEDIUM_TEXT) {
            return s_1asm(emit_fmt("%s:", _.lbl_α))
                 + s_comment(emit_fmt("# BOX IR_CALL %s(...) [IBB-9-6 user-proc dispatch]", fn))
                 + s_2asm("call", "rt_call_proc@PLT")
                 + s_2asm("jmp",  _.lbl_γ)
                 + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
                 + s_2asm("jmp",  _.lbl_ω);
        }
        if (MEDIUM_BINARY) {
            /*   0 : 48 BF + u64le name_ptr    movabs rdi, name                                        */
            /*  10 : BE   + u32le nargs        mov esi, nargs                                          */
            /*  15 : 48 B8 + u64le fn_ptr      movabs rax, &rt_call_proc                           */
            /*  25 : FF D0                     call rax                                                */
            /*  27 : E9 + u32le γ_rel32        jmp γ            ← γ patch at 28                        */
            /*  32 : E9 + u32le ω_rel32        β: jmp ω         ← β-def at 32, ω patch at 33           */
            /*  37 : end                                                                               */
            uint64_t nptr = (uint64_t)(uintptr_t)fn;
            uint64_t fptr; { void (*fp)(const char *, int) = rt_call_proc; fptr = (uint64_t)(uintptr_t)(void*)fp; }
            bin = { {28, 32, 33}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
            return bytes(2, "\x48\xBF") + u64le(nptr)
                 + bytes(1, "\xBE")     + u32le((uint32_t)narg)
                 + bytes(2, "\x48\xB8") + u64le(fptr)
                 + bytes(2, "\xFF\xD0")
                 + bytes(1, "\xE9")     + u32le(0)
                 + bytes(1, "\xE9")     + u32le(0);
        }
        return std::string();
    }

    /* IBB-10 (GOAL-ICON-BB, builtin dispatch): Icon builtin call (non-user-proc). fn names a builtin   */
    /* the mode-2 oracle table handles (type/image/integer/string/trim/reverse/repl/map/table/list/set/  */
    /* etc.). The flat driver (flat_drive_call_builtin) walked the arg γ-chain, leaving nargs single-shot */
    /* values on the vstack (arg0 deepest). Trailer: load name + nargs into the SysV arg registers and    */
    /* call rt_call_builtin, which pops the args, dispatches through try_call_builtin_by_name (so */
    /* m2==m3 by construction), and leaves the result on the vstack. Then jmp γ. Checked AFTER the        */
    /* specialized write(strlit)/write(int_expr) arms (which keep their proven byte trailers for the      */
    /* single-simple-arg case) and AFTER the user-proc arm. Gated by rt_builtin_is_known so a name    */
    /* the table cannot serve still ABORTs loudly (no silent empty output). Byte layout identical to the  */
    /* user-proc arm above (only the called fn pointer differs).                                          */
    int is_builtin = (fn && rt_builtin_is_known(fn) && !is_write_strlit && !is_write_intexpr);
    if (is_builtin) {
        if (MEDIUM_TEXT) {
            return s_1asm(emit_fmt("%s:", _.lbl_α))
                 + s_comment(emit_fmt("# BOX IR_CALL %s(...) [IBB-10 builtin dispatch]", fn))
                 + s_2asm("call", "rt_call_builtin@PLT")
                 + s_2asm("jmp",  _.lbl_γ)
                 + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
                 + s_2asm("jmp",  _.lbl_ω);
        }
        if (MEDIUM_BINARY) {
            /*   0 : 48 BF + u64le name_ptr    movabs rdi, name                                        */
            /*  10 : BE   + u32le nargs        mov esi, nargs                                          */
            /*  15 : 48 B8 + u64le fn_ptr      movabs rax, &rt_call_builtin                        */
            /*  25 : FF D0                     call rax                                                */
            /*  27 : E9 + u32le γ_rel32        jmp γ            ← γ patch at 28                        */
            /*  32 : E9 + u32le ω_rel32        β: jmp ω         ← β-def at 32, ω patch at 33           */
            /*  37 : end                                                                               */
            uint64_t nptr = (uint64_t)(uintptr_t)fn;
            uint64_t fptr; { void (*fp)(const char *, int) = rt_call_builtin; fptr = (uint64_t)(uintptr_t)(void*)fp; }
            bin = { {28, 32, 33}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
            return bytes(2, "\x48\xBF") + u64le(nptr)
                 + bytes(1, "\xBE")     + u32le((uint32_t)narg)
                 + bytes(2, "\x48\xB8") + u64le(fptr)
                 + bytes(2, "\xFF\xD0")
                 + bytes(1, "\xE9")     + u32le(0)
                 + bytes(1, "\xE9")     + u32le(0);
        }
        return std::string();
    }

    if (!is_write_strlit && !is_write_intexpr) {
        fprintf(stderr, "[IBB] FATAL bb_call: unsupported call shape — fn='%s' narg=%lld arg0_kind=%d\n",
                fn, (long long)narg, a0 ? (int)a0->t : -1);
        fprintf(stderr, "[IBB] ground zero supports ONLY write(string_literal) and write(int_expr). Abort.\n");
        abort();
    }

    if (is_write_intexpr) {
        /* GZ-2 (ICON READ-ONLY LOCALS ARE IP-RELATIVE, 2026-05-30): arg0 is a literal int — a READ-ONLY  */
        /* constant. Per the RO-IP-relative FACT RULE the constant is emitted as 8 bytes of sealed data    */
        /* INSIDE this box's own blob (after the jmps, never executed) and read `[rip+disp]` with disp an   */
        /* emit-time constant (data and access share the blob — NO patch, NO absolute address, NO stack).   */
        /* Then call the by-value rt_write_int_nl. Other arg0 kinds still route through rt_pop_write_* below */
        /* (they ABORT until their own rung). Mirrors the write(strlit) one-blob shape.                      */
        int arg_is_ro_int = (a0 && a0->t == IR_LIT_I);
        if (arg_is_ro_int && MEDIUM_BINARY) {
            bb_label_t *beta_jmp_target = _.lbl_ω_p;
            for (int i = 0; i < g_emit.xa_bb_emit_pair_n; i++) {
                if (g_emit.xa_bb_emit_pair_define[i] == _.lbl_β_p && g_emit.xa_bb_emit_pair_jmp[i]) {
                    beta_jmp_target = g_emit.xa_bb_emit_pair_jmp[i];
                    break;
                }
            }
            /*   0  : 48 8B 3D + u32le 22     mov rdi, [rip+22]   (rip-base=7; data@29; disp=29-7=22)     */
            /*   7  : 48 B8 + u64le fptr      movabs rax, &rt_write_int_nl                                 */
            /*  17  : FF D0                   call rax                                                     */
            /*  19  : E9 + u32le γ_rel32      jmp γ               <- γ patch at 20                         */
            /*  24  : E9 + u32le β_tgt_rel32  β: jmp β-target     <- β-def at 24, tgt patch at 25          */
            /*  29  : <u64le val>             sealed RO literal (never executed; reached only by [rip+22]) */
            /*  37  : end                                                                                  */
            uint64_t fptr; { void (*fp)(int64_t) = rt_write_int_nl; fptr = (uint64_t)(uintptr_t)(void*)fp; }
            int64_t  val = a0->ival;
            bin = { {20, 24, 25}, {_.lbl_γ_p, _.lbl_β_p, beta_jmp_target}, {false, true, false} };
            return bytes(3, "\x48\x8B\x3D") + u32le(22u)
                 + bytes(2, "\x48\xB8")     + u64le(fptr)
                 + bytes(2, "\xFF\xD0")
                 + bytes(1, "\xE9")         + u32le(0)
                 + bytes(1, "\xE9")         + u32le(0)
                 + u64le((uint64_t)val);
        }
        /* GZ-3 (GROUND ZERO 3, 2026-05-31): arg0 is the stackless RO-int binop. Its result was stored    */
        /* into a per-sequence frame slot [r12+off] (ζ=r12) by bb_binop.cpp; recover off via bb_slot_get   */
        /* on the binop node and read the value directly into rdi, then call the by-value rt_write_int_nl.  */
        /* No value stack. β-target taken from the driver's queued lbl_β pair (else ω), like GZ-2.          */
        int arg_is_ro_binop = (a0 && (a0->t == IR_BINOP || a0->t == IR_TO || a0->t == IR_TO_BY));
        /* GZ-2 (ICON RO-LOCALS IP-RELATIVE), mode-4 TEXT: the string analog's int twin — the literal     */
        /* int64 is a READ-ONLY constant emitted to .rodata with a unique label, read `mov rdi,[rip+lbl]` */
        /* (IP-relative, no value stack, no movabs-immediate), then the by-value rt_write_int_nl. Mirrors  */
        /* the MEDIUM_BINARY arm above (which seals the int in-blob and reads [rip+22]).                    */
        if (arg_is_ro_int && MEDIUM_TEXT) {
            std::string sl = emit_fmt(".Lwrite_int%d", bb_node_id(pBB));
            return s_1asm(emit_fmt("%s:", _.lbl_α))
                 + s_comment(emit_fmt("# BOX IR_CALL write(%lld) [GZ-2 RO-int IP-relative]", (long long)a0->ival))
                 + s_directive(".section .rodata")
                 + s_directive(sl + emit_fmt(": .quad %lld", (long long)a0->ival))
                 + s_directive(".section .text")
                 + s_directive(".intel_syntax noprefix")
                 + s_2asm("mov rdi,", "[rip + " + sl + "]")
                 + s_2asm("call",     "rt_write_int_nl@PLT")
                 + s_2asm("jmp",      _.lbl_γ);
        }
        if (arg_is_ro_binop && MEDIUM_BINARY) {
            int off = bb_slot_get(a0);
            if (off < 0) {
                fprintf(stderr, "[GZ-3] FATAL bb_call: write(binop) — binop result slot not allocated (bb_slot_get miss)\n");
                abort();
            }
            bb_label_t *beta_jmp_target = _.lbl_ω_p;
            for (int i = 0; i < g_emit.xa_bb_emit_pair_n; i++) {
                if (g_emit.xa_bb_emit_pair_define[i] == _.lbl_β_p && g_emit.xa_bb_emit_pair_jmp[i]) {
                    beta_jmp_target = g_emit.xa_bb_emit_pair_jmp[i];
                    break;
                }
            }
            /* GZ-4-string (this session): arg0 is the stackless RO-string CONCAT binop. Its 16-byte result */
            /* DESCR was stored at [r12+off] (eightbyte0=v+slen) / [r12+off+8] (eightbyte1=payload ptr) by  */
            /* bb_binop.cpp's str_concat_d arm. The joined buffer is NUL-terminated, so write reads the     */
            /* payload ptr at [r12+off+8] into rdi and calls the by-value rt_write_strz_nl (fwrite+strlen). */
            /* No value stack. Same {γ,β,ω} patch layout as the int arm, one byte longer at the read.       */
            if (a0->t == IR_BINOP && a0->ival == BINOP_CONCAT) {
                uint64_t fptr; { void (*fp)(const char *) = rt_write_strz_nl; fptr = (uint64_t)(uintptr_t)(void*)fp; }
                /*   0  : 49 8B BC 24 <u32 off+8> mov rdi, [r12+off+8]  (payload ptr of the concat DESCR)     */
                /*   8  : 48 B8 + u64le fptr      movabs rax, &rt_write_strz_nl                               */
                /*  18  : FF D0                   call rax                                                    */
                /*  20  : E9 + u32le γ_rel32      jmp γ               ← γ patch at 21                         */
                /*  25  : E9 + u32le β_tgt_rel32  β: jmp β-target     ← β-def 25, tgt patch at 26             */
                /*  30  : end                                                                                */
                bin = { {21, 25, 26}, {_.lbl_γ_p, _.lbl_β_p, beta_jmp_target}, {false, true, false} };
                return bytes(4, "\x49\x8B\xBC\x24") + u32le((uint32_t)(off + 8))
                     + bytes(2, "\x48\xB8")         + u64le(fptr)
                     + bytes(2, "\xFF\xD0")
                     + bytes(1, "\xE9")             + u32le(0)
                     + bytes(1, "\xE9")             + u32le(0);
            }
            /* GZ-3: arg0 is the stackless RO-int binop — result int64 at [r12+off]; read into rdi, call    */
            /* the by-value rt_write_int_nl. No value stack.                                                 */
            /*   0  : 49 8B BC 24 <u32 off>   mov rdi, [r12+off]   (read binop result from the ζ frame slot)*/
            /*   8  : 48 B8 + u64le fptr      movabs rax, &rt_write_int_nl                                  */
            /*  18  : FF D0                   call rax                                                      */
            /*  20  : E9 + u32le γ_rel32      jmp γ               ← γ patch at 21                           */
            /*  25  : E9 + u32le β_tgt_rel32  β: jmp β-target     ← β-def 25, tgt patch at 26               */
            /*  30  : end                                                                                   */
            uint64_t fptr; { void (*fp)(int64_t) = rt_write_int_nl; fptr = (uint64_t)(uintptr_t)(void*)fp; }
            bin = { {21, 25, 26}, {_.lbl_γ_p, _.lbl_β_p, beta_jmp_target}, {false, true, false} };
            return bytes(4, "\x49\x8B\xBC\x24") + u32le((uint32_t)off)
                 + bytes(2, "\x48\xB8")         + u64le(fptr)
                 + bytes(2, "\xFF\xD0")
                 + bytes(1, "\xE9")             + u32le(0)
                 + bytes(1, "\xE9")             + u32le(0);
        }
        const char *trailer_sym = arg_is_any ? "rt_pop_write_any_nl@PLT" : "rt_pop_write_int_nl@PLT";
        /* GZ-3 (GROUND ZERO 3), mode-4 TEXT: arg0 is the stackless binop/to whose result the binop/to    */
        /* template stored at [r12+off] (ζ=r12, off via bb_slot_get). Read it directly into rdi and call   */
        /* the by-value writer — no value stack. The CONCAT sub-arm reads the 16-byte DESCR payload ptr at */
        /* [r12+off+8] (NUL-terminated buffer) and calls rt_write_strz_nl. Mirrors the MEDIUM_BINARY arms. */
        if (arg_is_ro_binop && MEDIUM_TEXT) {
            int off = bb_slot_get(a0);
            if (off < 0) {
                fprintf(stderr, "[GZ-3] FATAL bb_call(text): write(binop) — result slot not allocated (bb_slot_get miss)\n");
                abort();
            }
            /* β re-pump target: mirror the BINARY arm — scan the queued EMIT_PAIR for lbl_β's jmp target */
            /* (the arg generator's β resume when EVERY drives a re-pump; else ω). Define β inline + jmp.  */
            bb_label_t *beta_tgt = _.lbl_ω_p;
            for (int i = 0; i < g_emit.xa_bb_emit_pair_n; i++) {
                if (g_emit.xa_bb_emit_pair_define[i] == _.lbl_β_p && g_emit.xa_bb_emit_pair_jmp[i]) { beta_tgt = g_emit.xa_bb_emit_pair_jmp[i]; break; }
            }
            std::string tail = s_L1asm(emit_fmt("%s:", _.lbl_β), "")
                             + s_2asm("jmp", beta_tgt && beta_tgt->name[0] ? beta_tgt->name : _.lbl_ω);
            if (a0->t == IR_BINOP && a0->ival == BINOP_CONCAT) {
                return s_1asm(emit_fmt("%s:", _.lbl_α))
                     + s_comment("# BOX IR_CALL write(concat) [GZ-4 stackless ζ-slot payload]")
                     + s_2asm("mov rdi,", emit_fmt("[r12 + %d]", off + 8))
                     + s_2asm("call",     "rt_write_strz_nl@PLT")
                     + s_2asm("jmp",      _.lbl_γ)
                     + tail;
            }
            return s_1asm(emit_fmt("%s:", _.lbl_α))
                 + s_comment("# BOX IR_CALL write(int-binop/to) [GZ-3 stackless ζ-slot]")
                 + s_2asm("mov rdi,", emit_fmt("[r12 + %d]", off))
                 + s_2asm("call",     "rt_write_int_nl@PLT")
                 + s_2asm("jmp",      _.lbl_γ)
                 + tail;
        }
        if (MEDIUM_TEXT) {
            return s_1asm(emit_fmt("%s:", _.lbl_α))
                 + s_comment(arg_is_any ? "# BOX IR_CALL write(IR_VAR) [IBB-7 any-write trailer]"
                                         : "# BOX IR_CALL write(int_expr) [IBB-3 vstack pop+write trailer]")
                 + s_2asm("call", trailer_sym)
                 + s_2asm("jmp",  _.lbl_γ)
                 + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
                 + s_2asm("jmp",  _.lbl_ω);
        }
        if (MEDIUM_BINARY) {
            /* movabs rax, &rt_pop_write_int_nl   (10 bytes: 48 B8 + u64le)                          */
            /* call rax                            ( 2 bytes: FF D0)                                  */
            /* jmp γ                               ( 5 bytes: E9 + u32le)                             */
            /* β: jmp <β-target>                   ( 5 bytes: E9 + u32le)                             */
            /* Total 22. Patch: 17 → γ, 22 → β-def, 23 → β-target.                                   */
            /*                                                                                        */
            /* IBB-4 (2026-05-28): β-stub destination is the driver's queued lbl_β jmp target (set    */
            /* by flat_drive_call_intexpr via EMIT_PAIR_DEF_JMP(lbl_β, arg_β)). For the prior strlit  */
            /* path / IBB-3 add.icn shape, the driver queues (lbl_β, lbl_ω) so behavior is unchanged. */
            /* This lets IR_EVERY drive a re-pump: every wires call.γ→call.β, call.β chains to arg.β. */
            bb_label_t *beta_jmp_target = _.lbl_ω_p;
            for (int i = 0; i < g_emit.xa_bb_emit_pair_n; i++) {
                if (g_emit.xa_bb_emit_pair_define[i] == _.lbl_β_p && g_emit.xa_bb_emit_pair_jmp[i]) {
                    beta_jmp_target = g_emit.xa_bb_emit_pair_jmp[i];
                    break;
                }
            }
            uint64_t fptr;
            if (arg_is_any) { void (*fp)(void) = rt_pop_write_any_nl; fptr = (uint64_t)(uintptr_t)(void*)fp; }
            else            { void (*fp)(void) = rt_pop_write_int_nl; fptr = (uint64_t)(uintptr_t)(void*)fp; }
            /* Layout:                                                                                  */
            /*   0 : 48 B8 + u64le fptr       (10)  movabs rax, &rt_pop_write_int_nl                    */
            /*  10 : FF D0                    ( 2)  call rax                                            */
            /*  12 : E9 + u32le γ_rel32       ( 5)  jmp γ            ← γ patch at offset 13             */
            /*  17 : E9 + u32le β_tgt_rel32   ( 5)  β: jmp β-target  ← β-def at 17, β-tgt patch at 18   */
            /*  22 : end                                                                                 */
            bin = { {13, 17, 18}, {_.lbl_γ_p, _.lbl_β_p, beta_jmp_target}, {false, true, false} };
            return bytes(2, "\x48\xB8") + u64le(fptr)
                 + bytes(2, "\xFF\xD0")
                 + bytes(1, "\xE9") + u32le(0)
                 + bytes(1, "\xE9") + u32le(0);
        }
        return std::string();
    }

    int id = bb_node_id(pBB);
    if (MEDIUM_TEXT) {
        const char * lit = a0->sval;
        uint32_t     len = (uint32_t)strlen(lit);
        std::string  sl  = emit_fmt(".Lcall%d_str", id);
        return s_1asm(emit_fmt("%s:", _.lbl_α))
             + s_comment(emit_fmt("# BOX IR_CALL write(\"%s\") [GROUND-ZERO bb_call literal-string shape]", lit))
             + s_directive(".section .rodata")
             + s_directive(sl + ": .ascii \"" + lit + "\"")
             + s_directive(".section .text")
             + s_directive(".intel_syntax noprefix")
             + s_2asm("lea rdi,", "[rip + " + sl + "]")
             + s_2asm("mov esi,", emit_fmt("%u", len))
             + s_2asm("call",     "rt_write_str_nl@PLT")
             + s_2asm("jmp",      _.lbl_γ);
    }
    if (MEDIUM_BINARY) {
        /* R-HW-2 (GROUND ZERO 3, 2026-05-30): ICON READ-ONLY LOCALS ARE IP-RELATIVE (FACT RULE). The   */
        /* string literal is a READ-ONLY constant: its bytes are sealed INSIDE this box's own blob (after */
        /* the jmps, never executed) and the address is taken `lea rdi,[rip+disp]` with disp an emit-time */
        /* constant (data and access share the blob — NO patch, NO absolute address, NO value stack). The */
        /* string analog of GZ-2's `write(42)` int box. Replaces the prior `movabs rdi,&"…"` form, which  */
        /* threaded an in-process AST-pool pointer (broke mode-4 relocatability, was not IP-relative). The */
        /* length is a compile-time constant immediate (mov esi,slen) — not an address, so no relocation. */
        /* β-target taken from the driver's queued lbl_β pair (EVERY re-pump), else ω, like GZ-2.         */
        /*                                                                                                 */
        /*   off  bytes                       asm                                                          */
        /*   0    48 8D 3D <u32 disp>         lea rdi,[rip+disp]   (rip-base=7; data@34; disp=34-7=27)     */
        /*   7    BE <u32 slen>               mov esi, slen                                                */
        /*   12   48 B8 <u64 fn_addr>         movabs rax, &rt_write_str_nl                                 */
        /*   22   FF D0                       call rax                                                     */
        /*   24   E9 <rel32 → γ>              jmp lbl_γ            ← γ patch at 25                         */
        /*   29   (lbl_β defined here)        β: jmp β-target      ← β-def at 29, β-tgt patch at 30        */
        /*   34   <slen RO string bytes>      sealed RO literal (reached only by [rip+27], never executed) */
        /*   34+slen end (slab continues)                                                                 */
        const char *lit  = a0->sval;
        uint32_t    slen = (uint32_t)strlen(lit);
        uint64_t    fptr; { void (*fp)(const char *, uint32_t) = rt_write_str_nl; fptr = (uint64_t)(uintptr_t)(void*)fp; }
        bb_label_t *beta_jmp_target = _.lbl_ω_p;
        for (int i = 0; i < g_emit.xa_bb_emit_pair_n; i++) {
            if (g_emit.xa_bb_emit_pair_define[i] == _.lbl_β_p && g_emit.xa_bb_emit_pair_jmp[i]) {
                beta_jmp_target = g_emit.xa_bb_emit_pair_jmp[i];
                break;
            }
        }
        bin = { {25, 29, 30}, {_.lbl_γ_p, _.lbl_β_p, beta_jmp_target}, {false, true, false} };
        return bytes(3, "\x48\x8D\x3D") + u32le(27u)
             + bytes(1, "\xBE")         + u32le(slen)
             + bytes(2, "\x48\xB8")     + u64le(fptr)
             + bytes(2, "\xFF\xD0")
             + bytes(1, "\xE9")         + u32le(0)
             + bytes(1, "\xE9")         + u32le(0)
             + std::string(lit, slen);
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_call(IR_t * pBB) { bb_bin_t bin; bb_emit_asm_result(bb_call_str(pBB, bin), bin); }
