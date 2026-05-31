/* bb_alt.cpp — BB_ALT: Icon n-ary alternation (A|B|C...).
   GOAL-ICON-BB. x86 only. RULES: no emission outside templates.
   Mode-2 path: bb_exec.c:1720 walks the ω-chain from pBB->α and uses pBB->counter as the active
   arm index across α/β re-entry.
   Mode-3 (this template, MEDIUM_BINARY): counter-state dispatch slab. The driver
   flat_drive_alt_icn (emit_bb.c) lays down per-arm bodies at arm_α[i] labels (after this slab
   emits), and queues those labels via xa_bb_emit_pair_jmp[i] (one EMIT_PAIR_JMP per arm).
   This template emits the dispatcher BEFORE the arm bodies (mirrors flat_drive_pl_choice's
   template-first-then-bodies pattern):
     lbl_α:      movabs rcx,&pBB->counter ; mov qword[rcx],0 ; jmp short dispatch
     lbl_β:      movabs rcx,&pBB->counter ; add qword[rcx],1 ; (fall through to dispatch)
     dispatch:   movabs rcx,&pBB->counter ; mov rcx,[rcx]
                 ; per arm i = 0..n-1:
                 cmp rcx, i ; je arm_α[i]
                 jmp lbl_ω                ; counter == n exhausted
   pBB->counter is a per-node int64 slot (same one mode-2 uses); valid runtime address at emit
   time. The cmp imm uses the 4-byte form (48 81 F9 + u32le) so arm counts > 127 work too.
   Per-arm γ/ω wiring (arm.γ→outer γ, arm.ω→outer ω) is set by the driver via walk_bb_flat. */
#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_alt_str(BB_t *pBB, bb_bin_t &bin) {
    bin = {};
    if (!PLATFORM_X86) return std::string();
    if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — BB_ALT");
    if (MEDIUM_TEXT) {
        return s_1asm(emit_fmt("%s:", _.lbl_α))
             + s_comment("# BOX BB_ALT [TEXT path port-wired; BINARY path is the real dispatcher]")
             + s_2asm("jmp",  _.lbl_γ)
             + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
             + s_2asm("jmp",  _.lbl_ω);
    }
    if (MEDIUM_BINARY) {
        int n = g_emit.xa_bb_emit_pair_n;
        if (n <= 0 || !pBB) {
            /* Degenerate: no arms — α→γ (won't happen for canonical-5; defensive only).               */
            bin = { {1, 5, 6}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
            return bytes(1,"\xE9") + u32le(0) + bytes(1,"\xE9") + u32le(0);
        }
        uint64_t cnt = (uint64_t)(uintptr_t)&pBB->counter;
        std::string b;
        /* α: movabs rcx, &counter ; mov qword[rcx], 0 ; jmp short dispatch */
        b += bytes(2, "\x48\xB9") + u64le(cnt);
        b += bytes(3, "\x48\xC7\x01") + u32le(0u);
        int alpha_jmp = (int)b.size();
        b += bytes(2, "\xEB\x00");                              /* jmp short (patched below) */
        /* β: movabs rcx, &counter ; add qword[rcx], 1 ; (fall through to dispatch) */
        int beta_off = (int)b.size();
        b += bytes(2, "\x48\xB9") + u64le(cnt);
        b += bytes(3, "\x48\x81\x01") + u32le(1u);
        /* dispatch: */
        int dispatch_off = (int)b.size();
        ((char*)b.data())[alpha_jmp+1] = (char)(dispatch_off - (alpha_jmp+2));
        b += bytes(2, "\x48\xB9") + u64le(cnt);
        b += bytes(3, "\x48\x8B\x09");                          /* mov rcx, [rcx] */
        /* Per-arm dispatch: cmp rcx, i ; je arm_α[i]                                                  */
        /* cmp rcx, imm32 = 48 81 F9 + u32le  (7 bytes)                                                */
        /* je rel32       = 0F 84 + u32le      (6 bytes)                                                */
        /* The je's u32le starts at je_opcode_offset + 2.                                              */
        bin.sites.push_back(beta_off);
        bin.labels.push_back(_.lbl_β_p);
        bin.is_def.push_back(true);
        for (int i = 0; i < n; i++) {
            b += bytes(3, "\x48\x81\xF9") + u32le((uint32_t)i);
            b += bytes(2, "\x0F\x84");
            bin.sites.push_back((int)b.size());
            bin.labels.push_back(g_emit.xa_bb_emit_pair_jmp[i]);
            bin.is_def.push_back(false);
            b += u32le(0);
        }
        /* Exhausted: jmp lbl_ω */
        b += bytes(1, "\xE9");
        bin.sites.push_back((int)b.size());
        bin.labels.push_back(_.lbl_ω_p);
        bin.is_def.push_back(false);
        b += u32le(0);
        return b;
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_alt(BB_t *pBB) { bb_bin_t bin; bb_emit_asm_result(bb_alt_str(pBB,bin),bin); }
