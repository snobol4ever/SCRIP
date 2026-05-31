/* bb_to.cpp — BB template for IR_TO (`lo to hi`, implicit step +1).
   IBB-3 / JA-2b, GOAL-ICON-BB. One file per BB kind per RULES.md.
   x86 only — IS_JVM/JS/NET/WASM arms stub (RULES.md: x86 only for now).

   IR_TO is the language-ignorant counted-integer generator. Today the only frontend
   producing it is Icon (`lower_icn.c` TT_TO) but the opcode and template carry no
   language identity (per Lon directive 2026-05-28: BB_* / SM_* opcodes and templates
   are language-independent operations).

   Lowering (lower_icn.c TT_TO): two shapes.
     LITERAL  : both bounds TT_ILIT → α/β NULL; lo in ival, hi bit-cast in dval.
     DYNAMIC  : non-literal bound(s) → α=lo operand box, β=hi operand box.
   The four-port generator mirrors bb_exec.c IR_TO (mode-2 reference) with step +1:
     α (fresh): cur = lo;            then fall into check
     β (retry): cur += 1;            then fall into check
     check    : cur > hi → ω (exhausted); else yield DT_I(cur) via γ.

   LITERAL FAST-PATH (JA-2b): lo/hi are compile-time constants read off the node
   (ival / dval bit-cast) — fully self-contained inline x86, no operand-box value
   read, no C Byrd box. Mirrors bb_to_by / bb_iterate / bb_upto counter idiom.
   State (cur) lives in a per-node .data quad (TEXT) / &pBB->counter (BINARY).
   Dynamic operands remain the documented passthrough TODO (H-3 value-field read;
   blocked behind Phase J flat-BB reachability — same gate as bb_to_by dynamic). */
#include <string>
#include <string.h>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
void rt_push_int(int64_t v);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_to_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (PLATFORM_X86) {
        int id = bb_node_id(pBB);
        /* Literal fast-path: both bounds folded into ival/dval, α/β NULL. */
        int lit_bounds = (pBB->α == NULL && pBB->β == NULL);
        int64_t lo_i = pBB->ival;
        int64_t hi_i = 0;
        memcpy(&hi_i, &pBB->dval, 8);

        if (MEDIUM_MACRO_DEF)
            return s_comment("# no macro form — TO");

        if (!lit_bounds) {
            /* DYNAMIC operands — not yet emitted inline (H-3 value-field read; blocked
               behind Phase J flat-BB reachability). Wire ports passthrough (α→γ, β→ω)
               so the graph stays connected; generator yields nothing. TEXT + BINARY
               mirror each other (identical to bb_to_by's dynamic arm). */
            if (MEDIUM_TEXT) {
                return s_1asm(emit_fmt("%s:", _.lbl_α))
                     + s_comment("# BOX TO [dynamic operands — inline-x86 TODO JA-2b]")
                     + s_2asm("jmp", _.lbl_γ)
                     + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
                     + s_2asm("jmp", _.lbl_ω);
            }
            if (MEDIUM_BINARY) {
                /* Mirror TEXT dynamic passthrough: α→γ, lbl_β define, β→ω.  Matches TEXT shape. */
                bin = { {1, 6, 11},
                        {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p},
                        {false, true, false} };
                return bytes(1,"\xE9")+u32le(0)+bytes(1,"\xE9")+u32le(0)+bytes(1,"\xE9")+u32le(0);
            }
        }

        /* ── LITERAL INTEGER GENERATOR (step +1) ── */
        std::string cur = emit_fmt(".Lto%d_cur", id);
        std::string chk = emit_fmt(".Lto%d_chk", id);

        if (MEDIUM_TEXT) {
            return s_1asm(emit_fmt("%s:", _.lbl_α))
                 + s_comment(emit_fmt("# BOX TO(lo=%ld hi=%ld)", (long)lo_i, (long)hi_i))
                 + s_directive(".section .data")
                 + s_directive(cur + ": .quad 0")
                 + s_directive(".section .text")
                 + s_directive(".intel_syntax noprefix")
                 /* α: cur = lo */
                 + s_2asm("lea rax,", "[rip + " + cur + "]")
                 + s_2asm("mov qword ptr [rax],", emit_fmt("%ld", (long)lo_i))
                 + s_1asm("jmp " + chk)
                 /* β: cur += 1 */
                 + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
                 + s_2asm("lea rax,", "[rip + " + cur + "]")
                 + s_2asm("add qword ptr [rax],", "1")
                 /* check: cur > hi → ω, then yield */
                 + s_L1asm(chk + ":", "")
                 + s_2asm("lea rax,", "[rip + " + cur + "]")
                 + s_2asm("mov rcx,", "[rax]")
                 + s_2asm("cmp rcx,", emit_fmt("%ld", (long)hi_i))
                 + s_2asm("jg", _.lbl_ω)
                 /* yield DT_I(cur) via the SM value-stack ABI (rt_push_int), matching bb_upto's    */
                 /* TEXT convention. cur is in rcx; rt_push_int takes the value in rdi. */
                 + s_2asm("mov", "rdi, rcx")
                 + s_2asm("call", "rt_push_int@PLT")
                 + s_2asm("jmp", _.lbl_γ);
        }

        if (MEDIUM_BINARY) {
            /* State slot = &pBB->counter (emitter-process address; valid at emit time;
               mode-4 standalone substitutes its own relocated .data — see bb_to_by). */
            uint64_t cnt = (uint64_t)(uintptr_t)&pBB->counter;
            std::string b;
            /* α: movabs rcx,&cur ; mov qword[rcx],lo ; jmp chk */
            b += bytes(2,"\x48\xB9") + u64le(cnt);
            b += bytes(3,"\x48\xC7\x01") + u32le((uint32_t)(int32_t)lo_i);
            int alpha_jmp = (int)b.size();
            b += bytes(2,"\xEB\x00");                               /* jmp chk (patch) */
            /* β: movabs rcx,&cur ; add qword[rcx],1 */
            int back_off = (int)b.size();
            b += bytes(2,"\x48\xB9") + u64le(cnt);
            b += bytes(3,"\x48\x81\x01") + u32le(1u);               /* add qword[rcx],1 */
            /* chk: */
            int chk_off = (int)b.size();
            ((char*)b.data())[alpha_jmp+1] = (char)(chk_off - (alpha_jmp+2));
            b += bytes(2,"\x48\xB9") + u64le(cnt);
            b += bytes(3,"\x48\x8B\x09");                           /* mov rcx,[rcx] */
            b += bytes(2,"\x48\xB8") + u64le((uint64_t)hi_i);       /* movabs rax,hi */
            b += bytes(3,"\x48\x39\xC1");                           /* cmp rcx,rax */
            int fail_off = (int)b.size();
            b += bytes(2, "\x0F\x8F") + u32le(0);                   /* jg lbl_ω RELOC */
            /* IBB-4 (Opus 4.7, 2026-05-28): yield DT_I(cur) via rt_push_int (Icon mode-3 vstack          */
            /* convention chosen in IBB-3 e612d519). r12 was rejected because XA_FLAT_PROLOGUE does not   */
            /* initialize r12. cur is in rcx; SysV ABI puts arg0 in rdi.                                  */
            /*   mov rdi, rcx              (3 bytes: 48 89 CF)                                            */
            /*   movabs rax, &rt_push_int  (10 bytes: 48 B8 + u64le)                                      */
            /*   call rax                  ( 2 bytes: FF D0)                                              */
            uint64_t push_int_fptr; { void (*fp)(int64_t) = rt_push_int; push_int_fptr = (uint64_t)(uintptr_t)(void*)fp; }
            b += bytes(3, "\x48\x89\xCF");
            b += bytes(2, "\x48\xB8") + u64le(push_int_fptr);
            b += bytes(2, "\xFF\xD0");
            int succ_off = (int)b.size();
            b += bytes(5,"\xE9\x00\x00\x00\x00");                   /* jmp lbl_γ RELOC */
            bin = { {back_off, fail_off+2, succ_off+1},
                    {_.lbl_β_p, _.lbl_ω_p, _.lbl_γ_p},
                    {true, false, false} };
            return b;
        }
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_to(IR_t * pBB) {
    bb_bin_t bin;
    bb_emit_asm_result(bb_to_str(pBB, bin), bin);
}
