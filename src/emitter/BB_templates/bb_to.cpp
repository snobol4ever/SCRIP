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
int  bb_slot_alloc(IR_t * nd);
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

        /* GZ-4 (GROUND ZERO 3, this session): stackless SINGLE-SHOT `lo to hi` over two READ-ONLY integer */
        /* literal bounds — the generator analog of GZ-3's RO-int binop. After the mode-3 ring→tree adapter */
        /* (icn_rt_arity now gives IR_TO arity 2) the bounds arrive as α/β IR_LIT_I children. Per the RO-IP- */
        /* relative + ONE-REGISTER FRAME FACT RULES: lo/hi are sealed RO data INSIDE this box's own blob and */
        /* read `[rip+disp]` (disp emit-time const); the first value (= lo, when lo<=hi) is the box's RW     */
        /* state → stored at [r12+off] (ζ=r12), off via bb_slot_alloc; the consumer (write) reads it via    */
        /* bb_slot_get. This is the α-port ONLY: `α: rax=lo; if lo>hi → ω; [r12+off]=rax; → γ`. The β-resume */
        /* PUMP (`I++; recheck`) that `every` needs is fork-blocked (the adapter NULLs iterate/branch), so   */
        /* β → ω here (single-shot: a bare `write(1 to 3)` consumes only the first value). Grounded in       */
        /* test_icon.c `to1` (the α arm: `to1_I = x1_V; if (to1_I > x2_V) goto …fail; else to1_V = to1_I`).  */
        if (MEDIUM_BINARY && pBB->α && pBB->β && pBB->α->t == IR_LIT_I && pBB->β->t == IR_LIT_I) {
            int      off  = bb_slot_alloc(pBB);
            int64_t  lov  = pBB->α->ival;
            int64_t  hiv  = pBB->β->ival;
            /*  STACKLESS PUMPING TO — grounded in test_icon.c to1 (Proebsting §4.4).               */
            /*  α: cur=lo; if lo>hi → ω; [r12+off]=cur; jmp γ                                       */
            /*  β: cur=[r12+off]; cur++; [r12+off]=cur; if cur>hi → ω; jmp γ                        */
            /*  lo/hi are SEALED RO data adjacent to the blob (IP-relative, no abs immediate).       */
            /*  cur lives in [r12+off] (ζ=r12 ONE-REGISTER FRAME). Zero rt_push/rt_pop.             */
            /*  Byte layout (86 total):                                                              */
            /*   α arm (offset 0..32):                                                               */
            /*    0  48 8B 05 XX XX XX XX  mov rax,[rip+d_lo]   rip-base=7; lo@70; d_lo=63           */
            /*    7  48 3B 05 XX XX XX XX  cmp rax,[rip+d_hi]   rip-base=14; hi@78; d_hi=64          */
            /*   14  0F 8F XX XX XX XX     jg ω                  ← ω-patch at 16                     */
            /*   20  49 89 84 24 XX XX XX XX  mov [r12+off],rax  store cur=lo                         */
            /*   28  E9 XX XX XX XX         jmp γ                ← γ-patch at 29                      */
            /*   β arm (offset 33..69):                                                               */
            /*   33  49 8B 84 24 XX XX XX XX  mov rax,[r12+off]  load cur  ← β-def at 33             */
            /*   41  48 FF C0                 inc rax             cur++                               */
            /*   44  49 89 84 24 XX XX XX XX  mov [r12+off],rax  store cur                            */
            /*   52  48 3B 05 XX XX XX XX  cmp rax,[rip+d_hi2]   rip-base=59; hi@78; d_hi2=19         */
            /*   59  0F 8F XX XX XX XX     jg ω                  ← ω-patch at 61                     */
            /*   65  E9 XX XX XX XX         jmp γ                ← γ-patch at 66                      */
            /*  data (offset 70..85):                                                                 */
            /*   70  <u64 lo>                sealed RO lo                                             */
            /*   78  <u64 hi>                sealed RO hi                                             */
            /*   86  end                                                                              */
            bin = { {16, 29, 33, 61, 66}, {_.lbl_ω_p, _.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p, _.lbl_γ_p}, {false, false, true, false, false} };
            return bytes(3, "\x48\x8B\x05") + u32le(63u)
                 + bytes(3, "\x48\x3B\x05") + u32le(64u)
                 + bytes(2, "\x0F\x8F")     + u32le(0)
                 + bytes(4, "\x49\x89\x84\x24") + u32le((uint32_t)off)
                 + bytes(1, "\xE9")         + u32le(0)
                 + bytes(4, "\x49\x8B\x84\x24") + u32le((uint32_t)off)
                 + bytes(3, "\x48\xFF\xC0")
                 + bytes(4, "\x49\x89\x84\x24") + u32le((uint32_t)off)
                 + bytes(3, "\x48\x3B\x05") + u32le(19u)
                 + bytes(2, "\x0F\x8F")     + u32le(0)
                 + bytes(1, "\xE9")         + u32le(0)
                 + u64le((uint64_t)lov)
                 + u64le((uint64_t)hiv);
        }

        if (MEDIUM_TEXT && pBB->α && pBB->β && pBB->α->t == IR_LIT_I && pBB->β->t == IR_LIT_I) {
            /* GZ-4 (GROUND ZERO 3), mode-4 TEXT twin of the BINARY pump above: lo/hi are READ-ONLY ints  */
            /* in .rodata (IP-relative), the moving cursor lives in the ζ frame slot [r12+off] (off via    */
            /* bb_slot_alloc so the consumer's bb_slot_get hits). α seeds cur=lo (guard lo>hi→ω) and       */
            /* yields via γ; β increments and re-yields until cur>hi→ω — the full stackless pump `every`    */
            /* re-drives. Grounded in test_icon.c `to1` (Proebsting §4.4). Zero rt_push/rt_pop.            */
            int      off = bb_slot_alloc(pBB);
            int      nid = bb_node_id(pBB);
            std::string lo = emit_fmt(".Lto%d_lo", nid);
            std::string hi = emit_fmt(".Lto%d_hi", nid);
            return s_1asm(emit_fmt("%s:", _.lbl_α))
                 + s_comment(emit_fmt("# BOX TO(lo=%lld hi=%lld) [GZ-4 stackless ζ-slot pump]", (long long)pBB->α->ival, (long long)pBB->β->ival))
                 + s_directive(".section .rodata")
                 + s_directive(lo + emit_fmt(": .quad %lld", (long long)pBB->α->ival))
                 + s_directive(hi + emit_fmt(": .quad %lld", (long long)pBB->β->ival))
                 + s_directive(".section .text")
                 + s_directive(".intel_syntax noprefix")
                 + s_2asm("mov rax,", "[rip + " + lo + "]")
                 + s_2asm("cmp rax,", "[rip + " + hi + "]")
                 + s_2asm("jg",       _.lbl_ω)
                 + s_2asm("mov",      emit_fmt("[r12 + %d], rax", off))
                 + s_2asm("jmp",      _.lbl_γ)
                 + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
                 + s_2asm("mov rax,", emit_fmt("[r12 + %d]", off))
                 + s_1asm("inc rax")
                 + s_2asm("mov",      emit_fmt("[r12 + %d], rax", off))
                 + s_2asm("cmp rax,", "[rip + " + hi + "]")
                 + s_2asm("jg",       _.lbl_ω)
                 + s_2asm("jmp",      _.lbl_γ);
        }

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
