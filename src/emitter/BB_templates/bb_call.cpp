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
void rt_write_str_nl(const char *s, uint32_t slen);
void rt_write_int_nl(int64_t v);
void rt_pop_write_int_nl(void);
void rt_pop_write_any_nl(void);
void rt_call_proc(const char *name, int nargs);
int  rt_proc_is_registered(const char *name);
void rt_call_builtin(const char *name, int nargs);
int  rt_builtin_is_known(const char *name);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_call_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (!PLATFORM_X86) return std::string();
    if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — IR_CALL");

    const char * fn   = pBB->sval ? pBB->sval : "";
    int64_t      narg = pBB->ival;
    IR_t       * a0   = pBB->α;

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
        const char *trailer_sym = arg_is_any ? "rt_pop_write_any_nl@PLT" : "rt_pop_write_int_nl@PLT";
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
