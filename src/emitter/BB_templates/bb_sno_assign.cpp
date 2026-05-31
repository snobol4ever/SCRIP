/* bb_sno_assign.cpp — STACKLESS BB template for SNOBOL4 IR_ASSIGN of a literal-string rhs.
   SBL-M3-STACKLESS (2026-05-31, Opus 4.8). Lon directive: NO VALUE STACK — forbidden. So this box
   passes both operands in REGISTERS (SysV rdi/rsi) to one runtime call; nothing touches g_vstack /
   rt_push_* / rt_pop_*. The rhs string and the lhs name are READ-ONLY constants (their pointers are
   compile-time immediates for the in-process mode-3 LIVE box, mirroring how every BINARY template bakes
   RO data + fn pointers via movabs). The four-port shape is the proven 32-byte bb_assign trailer plus one
   extra movabs (rsi = str): no β-retry (an assign is bounded/single-shot), β = jmp ω.

   Reached via emit_core's IR_ASSIGN case, lang-guarded (IR_LANG_SNO + α==IR_LIT_S); the Icon arm
   (bb_assign, α==IR_VAR) is untouched. Driver = flat_drive_sno_assign (emit_bb.c), which sets pBB->α to
   the rhs literal (postfix-folded by sno_ring_to_tree) and keeps the target name in pBB->sval.

   Bytes (42 total):
       0  : 48 BF + u64le name_ptr      movabs rdi, name_ptr
       10 : 48 BE + u64le str_ptr       movabs rsi, str_ptr
       20 : 48 B8 + u64le fn_ptr        movabs rax, &rt_sno_assign_lit_s
       30 : FF D0                       call rax
       32 : E9 + u32le γ_rel32          jmp γ              (rel32 patch at 33 -> lbl_γ)
       37 : E9 + u32le ω_rel32          β: jmp ω           (β-def at 37, rel32 patch at 38 -> lbl_ω)
       42 : end
   mode-4 TEXT arm: bomb (—compile/x86 emission is excised this era; honest stub, RULES-OK).
*/
#include <string>
#include <string.h>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
void rt_sno_assign_lit_s(const char *name, const char *str);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_sno_assign_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (!PLATFORM_X86) return std::string();
    if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — SNO IR_ASSIGN(lit_s)");
    if (MEDIUM_TEXT) {
        /* SBL-M4-STACKLESS (2026-05-31, Opus 4.8): mode-4 GAS arm — same four-port stackless shape as the
           BINARY arm, but the name/str are RO `.L` labels reached via `lea [rip+label]` (PIC-safe) and the
           store is `call rt_sno_assign_lit_s@PLT`. NO value stack. The two .asciz strings live in a
           self-contained `.section .rodata` block emitted just before the box's α-body, then `.section .text`
           restores the code section so the next box lands in .text. Unique labels keyed on flat node id. */
        IR_t *rhs = pBB ? pBB->α : NULL;
        if (!pBB || !pBB->sval || !rhs || rhs->t != IR_LIT_S || !rhs->sval) {
            return bomb_text(emit_fmt("SNO IR_ASSIGN(lit_s): need sval(name)+alpha IR_LIT_S sval(str) (got name=%s alpha=%d)",
                                      (pBB && pBB->sval) ? pBB->sval : "(null)", rhs ? (int)rhs->t : -1).c_str());
        }
        const char *name = pBB->sval;
        const char *str  = rhs->sval;
        int id = g_flat_node_id++;
        std::string nlbl = emit_fmt(".Lsno_name_%d", id);
        std::string slbl = emit_fmt(".Lsno_str_%d",  id);
        return s_directive(".section .rodata")
             + s_L1asm(nlbl + ":", emit_fmt(".asciz \"%s\"", name))
             + s_L1asm(slbl + ":", emit_fmt(".asciz \"%s\"", str))
             + s_directive(".section .text")
             + s_1asm(emit_fmt("%s:", _.lbl_α))
             + s_comment(emit_fmt("# BOX SNO IR_ASSIGN(lit_s) store(\"%s\") = '%s' [stackless, @PLT]", name, str))
             + s_2asm("lea",  emit_fmt("rdi, [rip + %s]", nlbl.c_str()))
             + s_2asm("lea",  emit_fmt("rsi, [rip + %s]", slbl.c_str()))
             + s_2asm("call", "rt_sno_assign_lit_s@PLT")
             + s_2asm("jmp",  _.lbl_γ)
             + s_1asm(std::string(_.lbl_β) + ":")
             + s_2asm("jmp",  _.lbl_ω);
    }
    if (MEDIUM_BINARY) {
        IR_t *rhs = pBB ? pBB->α : NULL;
        if (!pBB || !pBB->sval || !rhs || rhs->t != IR_LIT_S || !rhs->sval) {
            fprintf(stderr, "[SBB] FATAL bb_sno_assign: need IR_ASSIGN sval(name) + alpha IR_LIT_S sval(str) "
                            "(got name=%s alpha=%d)\n",
                    (pBB && pBB->sval) ? pBB->sval : "(null)", rhs ? (int)rhs->t : -1);
            abort();
        }
        const char *name = pBB->sval;
        const char *str  = rhs->sval;
        uint64_t nptr = (uint64_t)(uintptr_t)name;
        uint64_t sptr = (uint64_t)(uintptr_t)str;
        uint64_t fptr; { void (*fp)(const char *, const char *) = rt_sno_assign_lit_s; fptr = (uint64_t)(uintptr_t)(void*)fp; }
        bin = { {33, 37, 38}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
        return bytes(2, "\x48\xBF") + u64le(nptr)
             + bytes(2, "\x48\xBE") + u64le(sptr)
             + bytes(2, "\x48\xB8") + u64le(fptr)
             + bytes(2, "\xFF\xD0")
             + bytes(1, "\xE9")     + u32le(0)
             + bytes(1, "\xE9")     + u32le(0);
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_sno_assign(IR_t * pBB) { bb_bin_t bin; bb_emit_asm_result(bb_sno_assign_str(pBB, bin), bin); }
