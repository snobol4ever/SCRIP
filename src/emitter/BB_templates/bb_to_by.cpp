/* bb_to_by.cpp — BB template for BB_TO_BY (Icon `lo to hi by step`, int + real).
   ICN-T-2 / JA-2, GOAL-BB-TEMPLATE-LADDER. One file per BB kind per RULES.md.
   x86 only — IS_JVM/JS/NET/WASM arms stub (RULES.md: x86 only for now).

   Lowering (lower_icn.c TT_TO_BY): α=lo operand box, β=hi operand box, ival=step,
   sval="i"/"r". The four-port generator mirrors bb_exec.c BB_TO_BY (mode-2 reference):
     α (fresh): cur = lo;            then fall into check
     β (retry): cur += by;           then fall into check
     check    : (by>=0 ? cur>hi : cur<hi) → ω (exhausted); else yield cur via γ.

   LITERAL FAST-PATH (JA-2 step 1): when both bound operands are BB_LIT_I, lo/hi are
   compile-time constants read off the operand nodes (α->ival / β->ival) — fully
   self-contained inline x86, no operand-box value read, no C Byrd box. Mirrors
   bb_iterate / bb_upto counter idiom. State (cur) lives in a per-node .data quad
   (TEXT) / &pBB->counter (BINARY). Dynamic-operand + real paths remain the
   documented passthrough TODO (H-3 value-field read; JA-2 step 2). */
#include <string>
#include <string.h>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
#include "descr.h"
void rt_push_int(int64_t v);
int  rt_toby_real(DESCR_t *cur_slot, int64_t lo_bits, int64_t hi_bits, int64_t step_bits, int reset);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_to_by_str(BB_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (PLATFORM_X86) {
        int id = bb_node_id(pBB);
        int is_real = (pBB->sval != NULL && pBB->sval[0] == 'r');
        int64_t by_i = pBB->ival ? pBB->ival : 1;
        /* Literal fast-path detection: both bounds are BB_LIT_I operand boxes. */
        int lit_bounds = (!is_real && pBB->α && pBB->β &&
                          pBB->α->t == BB_LIT_I && pBB->β->t == BB_LIT_I);
        int64_t lo_i = lit_bounds ? pBB->α->ival : 0;
        int64_t hi_i = lit_bounds ? pBB->β->ival : 0;
        /* IBB-TOBY-REAL: literal-real bounds (`1.0 to 2.0 by 0.5`) — α/β are BB_LIT_F, step bit-cast  */
        /* in pBB->ival. Read lo/hi/step as doubles (bit-cast to u64 for the call) and emit a generator */
        /* driven by rt_toby_real (mirrors the mode-2 real arm → m2==m3 by construction).           */
        int lit_real = (is_real && pBB->α && pBB->β &&
                        pBB->α->t == BB_LIT_F && pBB->β->t == BB_LIT_F);

        if (MEDIUM_MACRO_DEF)
            return s_comment("# no macro form — TO_BY");

        if (lit_real) {
            uint64_t cur_slot = (uint64_t)(uintptr_t)&pBB->value;
            uint64_t lo_bits; { double d = pBB->α->dval; memcpy(&lo_bits, &d, 8); }
            uint64_t hi_bits; { double d = pBB->β->dval; memcpy(&hi_bits, &d, 8); }
            uint64_t step_bits = (uint64_t)pBB->ival;   /* already bit-cast double in lowering */
            uint64_t fptr; { int (*fp)(DESCR_t*, int64_t, int64_t, int64_t, int) = rt_toby_real; fptr = (uint64_t)(uintptr_t)(void*)fp; }
            if (MEDIUM_TEXT) {
                std::string s = s_1asm(emit_fmt("%s:", _.lbl_α))
                     + s_comment("# BOX TO_BY real-literal [rt_toby_real]");
                for (int half = 0; half < 2; half++) {
                    if (half == 1) s += s_L1asm(emit_fmt("%s:", _.lbl_β), "");
                    s += s_2asm("movabs rdi,", emit_fmt("%llu", (unsigned long long)cur_slot))
                       + s_2asm("movabs rsi,", emit_fmt("%llu", (unsigned long long)lo_bits))
                       + s_2asm("movabs rdx,", emit_fmt("%llu", (unsigned long long)hi_bits))
                       + s_2asm("movabs rcx,", emit_fmt("%llu", (unsigned long long)step_bits))
                       + s_2asm("mov r8d,", half == 0 ? "1" : "0")
                       + s_2asm("movabs rax,", emit_fmt("%llu", (unsigned long long)fptr))
                       + s_2asm("call", "rax")
                       + s_2asm("test", "rax, rax")
                       + s_2asm("jz", _.lbl_ω)
                       + s_2asm("jmp", _.lbl_γ);
                }
                return s;
            }
            if (MEDIUM_BINARY) {
                std::string b;
                for (int half = 0; half < 2; half++) {
                    b += bytes(2, "\x48\xBF") + u64le(cur_slot);    /* movabs rdi, cur_slot  */
                    b += bytes(2, "\x48\xBE") + u64le(lo_bits);     /* movabs rsi, lo_bits   */
                    b += bytes(2, "\x48\xBA") + u64le(hi_bits);     /* movabs rdx, hi_bits   */
                    b += bytes(2, "\x48\xB9") + u64le(step_bits);   /* movabs rcx, step_bits */
                    b += bytes(2, "\x41\xB8") + u32le(half == 0 ? 1u : 0u); /* mov r8d, reset */
                    b += bytes(2, "\x48\xB8") + u64le(fptr);        /* movabs rax, &fn       */
                    b += bytes(2, "\xFF\xD0");                      /* call rax              */
                    b += bytes(3, "\x48\x85\xC0");                 /* test rax, rax         */
                    b += bytes(2, "\x0F\x84") + u32le(0);           /* jz ω  (patch)         */
                    b += bytes(1, "\xE9")     + u32le(0);            /* jmp γ (patch)         */
                }
                /* Per half: jz-ω site at +63, jmp-γ at +68; half size 72. β-define at 72.            */
                bin = { {63, 68, 72, 135, 140},
                        {_.lbl_ω_p, _.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p, _.lbl_γ_p},
                        {false, false, true, false, false} };
                return b;
            }
        }

        if (!lit_bounds) {
            /* DYNAMIC / REAL operands — not yet emitted inline (H-3 value-field read,
               JA-2 step 2). Wire ports passthrough (α→γ, β→ω) so the graph stays
               connected; generator yields nothing. TEXT + BINARY mirror each other. */
            if (MEDIUM_TEXT) {
                return s_1asm(emit_fmt("%s:", _.lbl_α))
                     + s_comment(emit_fmt("# BOX TO_BY(by=%ld%s) [dynamic/real operands — inline-x86 TODO JA-2]",
                                          (long)by_i, is_real ? " real" : ""))
                     + s_2asm("jmp", _.lbl_γ)
                     + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
                     + s_2asm("jmp", _.lbl_ω);
            }
            if (MEDIUM_BINARY) {
                bin = { {1, 5, 6}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
                return bytes(1, "\xE9") + u32le(0) + bytes(1, "\xE9") + u32le(0);
            }
        }

        /* ── LITERAL INTEGER GENERATOR ── */
        std::string cur = emit_fmt(".Ltoby%d_cur", id);
        std::string chk = emit_fmt(".Ltoby%d_chk", id);

        if (MEDIUM_TEXT) {
            return s_1asm(emit_fmt("%s:", _.lbl_α))
                 + s_comment(emit_fmt("# BOX TO_BY(lo=%ld hi=%ld by=%ld)",
                                      (long)lo_i, (long)hi_i, (long)by_i))
                 + s_directive(".section .data")
                 + s_directive(cur + ": .quad 0")
                 + s_directive(".section .text")
                 + s_directive(".intel_syntax noprefix")
                 /* α: cur = lo */
                 + s_2asm("lea rax,", "[rip + " + cur + "]")
                 + s_2asm("mov qword ptr [rax],", emit_fmt("%ld", (long)lo_i))
                 + s_1asm("jmp " + chk)
                 /* β: cur += by */
                 + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
                 + s_2asm("lea rax,", "[rip + " + cur + "]")
                 + s_2asm("add qword ptr [rax],", emit_fmt("%ld", (long)by_i))
                 /* check: bound test (sign of step), then yield */
                 + s_L1asm(chk + ":", "")
                 + s_2asm("lea rax,", "[rip + " + cur + "]")
                 + s_2asm("mov rcx,", "[rax]")
                 + s_2asm("cmp rcx,", emit_fmt("%ld", (long)hi_i))
                 + s_2asm(by_i >= 0 ? "jg" : "jl", _.lbl_ω)
                 /* yield DT_I(cur) via the SM value-stack ABI (rt_push_int), matching bb_to /  */
                 /* bb_upto TEXT convention. cur is in rcx; rt_push_int takes the value in rdi. raw */
                 /* r12 here SEGFAULTS in mode-4 (--compile) — r12 is the value-stack only in the   */
                 /* MEDIUM_BINARY (brokered) path below; in TEXT it is not set up. (Goal trap #2.)   */
                 + s_2asm("mov", "rdi, rcx")
                 + s_2asm("call", "rt_push_int@PLT")
                 + s_2asm("jmp", _.lbl_γ);
        }

        if (MEDIUM_BINARY) {
            /* State slot = &pBB->counter (emitter-process address; valid in native codegen/mode-3
               since the BB node is read at emit time. Mode-4 standalone uses its own
               relocated .data — the binary emitter substitutes the slot; see bb_iterate). */
            uint64_t cnt = (uint64_t)(uintptr_t)&pBB->counter;
            std::string b;
            /* α: movabs rcx,&cur ; mov qword[rcx],lo ; jmp chk */
            b += bytes(2,"\x48\xB9") + u64le(cnt);                 /* movabs rcx,&cur */
            b += bytes(3,"\x48\xC7\x01") + u32le((uint32_t)(int32_t)lo_i); /* mov qword[rcx],lo (sign-ext imm32) */
            int alpha_jmp = (int)b.size();
            b += bytes(2,"\xEB\x00");                               /* jmp chk (patch) */
            /* β: movabs rcx,&cur ; add qword[rcx],by (imm32, sign-extended) */
            int back_off = (int)b.size();
            b += bytes(2,"\x48\xB9") + u64le(cnt);
            b += bytes(3,"\x48\x81\x01") + u32le((uint32_t)(int32_t)by_i); /* add qword[rcx],imm32 */
            /* chk: */
            int chk_off = (int)b.size();
            ((char*)b.data())[alpha_jmp+1] = (char)(chk_off - (alpha_jmp+2));
            b += bytes(2,"\x48\xB9") + u64le(cnt);
            b += bytes(3,"\x48\x8B\x09");                           /* mov rcx,[rcx] */
            b += bytes(2,"\x48\xB8") + u64le((uint64_t)hi_i);       /* movabs rax,hi */
            b += bytes(3,"\x48\x39\xC1");                           /* cmp rcx,rax */
            int fail_off = (int)b.size();
            /* jg lbl_ω (by>=0) else jl lbl_ω : 0F 8F / 0F 8C rel32 */
            b += bytes(2, by_i >= 0 ? "\x0F\x8F" : "\x0F\x8C") + u32le(0);   /* jcc lbl_ω RELOC */
            /* M3-RK-NOINTERP-1a (Sonnet 4.6, 2026-05-28): yield DT_I(cur) via rt_push_int — matches      */
            /* bb_to.cpp MEDIUM_BINARY (IBB-3 e612d519) and the MEDIUM_TEXT arm above (line ~89-90).      */
            /* The prior r12-write path is incompatible with sm_run_native (mode-3 native), which is     */
            /* the only consumer of BB_TO_BY MEDIUM_BINARY today: neither sm_run_native nor XA_FLAT_     */
            /* PROLOGUE initialises r12, so the four `r12`-relative writes that followed segfaulted or   */
            /* corrupted memory, then the SM rt-vstack hit underflow. Convention: cur is in rcx; SysV    */
            /* puts arg0 in rdi.                                                                          */
            /*   mov rdi, rcx              (3 bytes: 48 89 CF)                                            */
            /*   movabs rax, &rt_push_int  (10 bytes: 48 B8 + u64le)                                      */
            /*   call rax                  ( 2 bytes: FF D0)                                              */
            uint64_t push_int_fptr; { void (*fp)(int64_t) = rt_push_int; push_int_fptr = (uint64_t)(uintptr_t)(void*)fp; }
            b += bytes(3, "\x48\x89\xCF");
            b += bytes(2, "\x48\xB8") + u64le(push_int_fptr);
            b += bytes(2, "\xFF\xD0");
            int succ_off = (int)b.size();
            b += bytes(5,"\xE9\x00\x00\x00\x00");                         /* jmp lbl_γ RELOC */
            /* M3-RK-NOINTERP-1b (Opus 4.7, 2026-05-28): sites MUST be ascending per emit_str.cpp's       */
            /* bb_emit_asm_result patch loop (`for (; pos < bin.sites[i]; pos++) bb_emit_byte(...)` —     */
            /* pos strictly advances).  Parallel to the every_to.icn canonical-5 fix in bb_to.cpp:        */
            /* reordered from non-ascending {fail_off+2, succ_off+1, back_off} (back_off ~19 was being    */
            /* processed last past-end) to ascending {back_off=19, fail_off+2~52, succ_off+1~75}.         */
            bin = { {back_off, fail_off+2, succ_off+1},
                    {_.lbl_β_p, _.lbl_ω_p, _.lbl_γ_p},
                    {true, false, false} };
            return b;
        }
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_to_by(BB_t * pBB) {
    bb_bin_t bin;
    bb_emit_asm_result(bb_to_by_str(pBB, bin), bin);
}
