/* bb_rk_gather.cpp — BB template for IR_GATHER (Raku `gather { take E0; take E1; ... }`).
   RK-EMIT-GATHER (Opus 4.8, 2026-06-01). One file per BB kind per RULES.md TEMPLATE-ONLY EMISSION.
   x86 only — IS_JVM/JS/NET/WASM arms stub (RULES.md: x86 only for now).

   IR_GATHER is the Raku resumable Seq PRODUCER, realising the FLAT-take model
   (docs.raku.org/syntax/gather%20take: a take yields one value per pull; the keystone spec
   APPENDIX-A RK-M2-GATHER = counter-as-resume-cursor). Layout set by lower.c v_raku_gather:
       .ival    = take COUNT (n)
       .counter = IR_graph_t* array of take-payload value sub-graphs (FLAT model: each entry is IR_LIT_I)
       .state   = takes already yielded (the resume cursor; calloc-zeroed at IR_node_alloc)
   ports: γ = the yielded element delivered to the consumer (the for-loop bind); ω = Seq drained.

   ⛔ NO RESET ON α — the flat-chain wiring (codegen_flat_chain_body) routes BOTH the initial loop
   entry AND the for-loop body's re-pump back-edge to this box's α label (the BFS resolves a γ-edge to
   the target node's α, never its β). So α must NOT reset the cursor to 0 — it must YIELD-THEN-ADVANCE
   from the current cursor exactly as the resume does. First-entry correctness rides on .state being
   zero-initialised (IR_node_alloc calloc + explicit .state=0). β is a safety landing that re-enters the
   same check. This matches the mode-2 oracle (bb_exec.c IR_GATHER): yield subs[state] then state++, and
   walking past the last take (state>=n) FAILs to ω. SCOPE: single evaluation (the smoke + corpus shape);
   a gather re-evaluated fresh from an OUTER loop would want a true α-reset (the oracle's bb_reset) — a
   later refinement on this same node, exactly like the mode-2 "FLAT-take, dynamic-scope-takes-later" note.

   ⛔ NO VALUE STACK — the yielded element is written as a 16-byte DESCR into this box's OWN ζ result slot
   [r12+resoff] (bb_slot_alloc16, ζ=r12), which the consumer (bb_assign GZ-7) reads via bb_slot_get. The
   take literals are READ-ONLY constants extracted at EMIT time (no runtime bb-graph walking — the oracle's
   bb_exec is NOT called) and sealed: in-blob RO data read [rip+disp] (BINARY), or .rodata (TEXT). The
   resume cursor is the per-box RW slot &pBB->state (BINARY, in-process) / a .data quad (TEXT). DESCR int
   tag = DT_I (6), [slot+0]=tag, [slot+8]=value — the proven bb_call.cpp marshalling layout.                */
#include <string>
#include <string.h>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
int  bb_slot_alloc16(IR_t * nd);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_rk_gather_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (!PLATFORM_X86) return std::string();
    if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — IR_GATHER");

    int            n    = (int) pBB->ival;
    IR_graph_t **  subs = (IR_graph_t **)(intptr_t) pBB->counter;
    /* FLAT-take model: extract each take payload's literal int at emit time. A non-literal take payload    */
    /* (take($x), take(expr)) is outside this rung — FALL LOUD per the missing-arm FACT RULE.               */
    int64_t vals[256];
    /* FLAT-take model: a take count out of range, or ANY non-literal-int take payload (take($x),           */
    /* take(expr)), is outside this rung — a SINGLE fall-loud guard per the missing-arm FACT RULE (one      */
    /* sanctioned side-effect per template, matching every peer bb_*.cpp).                                  */
    int ok = (n >= 0 && n <= 256 && (n == 0 || subs != NULL));
    for (int i = 0; ok && i < n; i++) {
        IR_t * lf = subs[i] ? subs[i]->entry : NULL;
        if (!lf || lf->t != IR_LIT_I) { ok = 0; break; }
        vals[i] = lf->ival;
    }
    if (!ok) { fprintf(stderr, "[RK] FATAL bb_rk_gather: gather requires 0..256 literal-int take payloads (FLAT-take model); n=%d\n", n); abort(); }
    int     resoff = bb_slot_alloc16(pBB);
    int     id     = bb_node_id(pBB);

    if (MEDIUM_TEXT) {
        std::string st  = emit_fmt(".Lrkg%d_state", id);
        std::string vl  = emit_fmt(".Lrkg%d_vals",  id);
        std::string chk = emit_fmt(".Lrkg%d_chk",   id);
        std::string s   =
              s_directive(".section .data")
            + s_directive(st + ": .quad 0")
            + s_directive(".section .rodata");
        std::string quads = vl + ":";
        if (n > 0) {
            quads += " .quad ";
            for (int i = 0; i < n; i++) quads += emit_fmt("%s%lld", (i ? ", " : ""), (long long)vals[i]);
        } else {
            quads += " .quad 0";
        }
        s += s_directive(quads)
           + s_directive(".section .text")
           + s_directive(".intel_syntax noprefix")
           + s_comment(emit_fmt("# BOX IR_GATHER n=%d [RK-EMIT-GATHER yield-then-advance ζ-slot]", n))
           + s_L1asm(chk + ":", "")
           + s_2asm("lea rax,", "[rip + " + st + "]")
           + s_2asm("mov rcx,", "[rax]")
           + s_2asm("cmp rcx,", emit_fmt("%d", n))
           + s_2asm("jge", _.lbl_ω)
           + s_2asm("lea rdx,", "[rip + " + vl + "]")
           + s_2asm("mov rsi,", "[rdx + rcx*8]")
           + s_2asm("mov qword ptr", emit_fmt("[r12 + %d], 6", resoff))
           + s_2asm("mov", emit_fmt("[r12 + %d], rsi", resoff + 8))
           + s_1asm("inc rcx")
           + s_2asm("mov", "[rax], rcx")
           + s_2asm("jmp", _.lbl_γ)
           + s_L1asm(std::string(_.lbl_β) + ":", "")
           + s_2asm("jmp", chk);
        return s;
    }

    if (MEDIUM_BINARY) {
        /* In-process pointers (mode-3 emitter == runtime): cursor slot &pBB->state; the take values are    */
        /* sealed RO data at the END of this blob, read IP-relative by [rip+disp] + index rcx*8. β re-enters */
        /* the check at offset 0 (self-patched rel32). External patches via bin.sites: ω (jge), γ (jmp),     */
        /* β-define. Byte layout (code is a FIXED 73 bytes; data follows so disps don't shift with n):       */
        /*   0  : 48 B8 <u64 &state>     movabs rax, &state                                                  */
        /*   10 : 48 8B 08               mov rcx, [rax]            (rcx = cursor)                            */
        /*   13 : 48 81 F9 <u32 n>       cmp rcx, n                                                          */
        /*   20 : 0F 8D <rel32 ω>        jge ω      (drained)      ← ω-patch at 22                           */
        /*   26 : 48 8D 15 <u32 40>      lea rdx, [rip+vals]       (rip-base=33; vals@73; disp=40)           */
        /*   33 : 48 8B 34 CA            mov rsi, [rdx + rcx*8]    (rsi = vals[cursor])                      */
        /*   37 : 49 C7 84 24 <u32 resoff> <u32 6>   mov qword[r12+resoff], 6   (DT_I tag)                   */
        /*   49 : 49 89 B4 24 <u32 resoff+8>          mov [r12+resoff+8], rsi   (value)                      */
        /*   57 : 48 FF C1               inc rcx                                                             */
        /*   60 : 48 89 08               mov [rax], rcx           (store cursor)                            */
        /*   63 : E9 <rel32 γ>           jmp γ                     ← γ-patch at 64                           */
        /*   68 : E9 <rel32 chk(0)>      β: jmp check             ← β-def 68, self-patch rel32 at 69         */
        /*   73 : <u64 vals[0..n-1]>     sealed RO take literals                                             */
        uint64_t st_addr = (uint64_t)(uintptr_t) &pBB->state;
        std::string b;
        b += bytes(2, "\x48\xB8") + u64le(st_addr);
        b += bytes(3, "\x48\x8B\x08");
        b += bytes(3, "\x48\x81\xF9") + u32le((uint32_t)n);
        b += bytes(2, "\x0F\x8D")     + u32le(0);
        b += bytes(3, "\x48\x8D\x15") + u32le(40u);
        b += bytes(4, "\x48\x8B\x34\xCA");
        b += bytes(4, "\x49\xC7\x84\x24") + u32le((uint32_t)resoff) + u32le(6u);
        b += bytes(4, "\x49\x89\xB4\x24") + u32le((uint32_t)(resoff + 8));
        b += bytes(3, "\x48\xFF\xC1");
        b += bytes(3, "\x48\x89\x08");
        b += bytes(1, "\xE9") + u32le(0);
        b += bytes(1, "\xE9") + u32le((uint32_t)(int32_t)(0 - (68 + 5)));
        for (int i = 0; i < n; i++) b += u64le((uint64_t)vals[i]);
        if (n == 0) b += u64le(0);
        bin = { {22, 64, 68}, {_.lbl_ω_p, _.lbl_γ_p, _.lbl_β_p}, {false, false, true} };
        return b;
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_rk_gather(IR_t * pBB) { bb_bin_t bin; bb_emit_asm_result(bb_rk_gather_str(pBB, bin), bin); }
