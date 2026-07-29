/* rtx_match.S — RTX family MATCH, slice 1: the CAP accessor leaves.
 *
 * READ .github/ARCH-SNOBOL4-RTX.md BEFORE EDITING. Contract macros: rtx_abi.inc.
 *
 * SCOPE, STATED HONESTLY: this is an ERADICATION slice (RTX-12), not a speed
 * rung. The three functions here are 1-line C accessors over a per-box capture
 * stack; the C they replace is already minimal and NO speed claim is made or
 * should be quoted off them. What this slice buys is C deleted from the
 * SNOBOL4-reachable runtime, plus the MATCH family gate and build wiring that
 * RTX-8's larger sinks (cap/dcap/defer, match_enter/variant/value/replace) will
 * land behind. Grade it on correctness, not on the clock.
 *
 * C OF RECORD: src/runtime/pattern_match.c:743/768/770 (bodies renamed c_* in
 * the same commit). The capture stack is SN4-PAT-CAPTURE-STACK (Lon directive
 * 2026-07-05): the SAVE box's alpha pushes, its beta pops, and the COND at each
 * yield reads top-of-stack, so the beta-resume chain survives a generator
 * re-entry between capture-open and capture-close.
 *
 * THE SLOT: rt_cap_stk_t = { uint32_t *buf; uint32_t gen; uint32_t sp; }
 *   +0  buf   rt_ws_alloc'd u32[]: buf[0] = capacity, frames from buf[1]
 *   +8  gen   per-match generation stamped by rt_match_enter
 *   +12 sp    frame count
 * All four offsets are _Static_assert-anchored beside the struct in
 * pattern_match.c, so a struct change breaks the BUILD, not the runtime --
 * stronger than any probe (the RTX-4 slice-3 stride lesson).
 *
 * WHY THE GENERATION COMPARE IS LOAD-BEARING AND IS REPRODUCED EXACTLY:
 * a stale-gen slot lazily resets sp=0, so success-exited frames -- which are
 * never beta-popped, the gamma-exit-live case -- die at the next match instead
 * of leaking across statement executions. It also validates ZC_INIT_ZERO-fresh
 * zeta frames, since gen 0 matches no live generation. An asm port that
 * "simplified" this to a bare sp test would leak captures across statements,
 * link fine, and pass every short test. It is not simplified here.
 *
 * g_cap_gen was `static` in the C and is UNREFERENCEABLE from a .S in that
 * form. It is now a non-static hidden global (un-static'd by PATCTX-2,
 * 2026-07-29: IR_MATCH_HEAD's alpha reads it via [rip+g_cap_gen] to save the
 * current match id before rt_match_enter issues a fresh one). g_cap_gen_next
 * is the monotonic well; it stays static in the C but is promoted to hidden
 * here to make the asm side readable. Do NOT read either through the GOT.
 */
#include "rtx_abi.inc"

RTX_GATE_DEF(match)

/*-----------------------------------------------------------------------------
 * void rt_cap_match_begin(void)    C of record: pattern_match.c (c_rt_cap_match_begin)
 *   g_cap_gen = ++g_cap_gen_next; if (!g_cap_gen) g_cap_gen = g_cap_gen_next = 1;
 *
 * PATCTX-2 (2026-07-29): the two-variable discipline. g_cap_gen_next is the
 * monotonic well; g_cap_gen holds the CURRENT match's id and is readable by
 * IR_MATCH_HEAD's alpha (which saves it to the frame before rt_match_enter
 * issues a fresh stamp). Exits restore g_cap_gen to the saved id -- never the
 * counter itself -- so a restored-then-re-bumped counter cannot re-issue a
 * retired stamp and zombie success-exited frames.
 *
 * Wrap guard: generation 0 is the ZC_INIT_ZERO-fresh sentinel; a wrap to 0
 * on either variable must reset BOTH to 1 so they stay in agreement and the
 * sentinel invariant holds.
 */
RTX_FUNC(rt_cap_match_begin)
    RTX_GATE(match, c_rt_cap_match_begin)
    mov     eax, dword ptr [rip + g_cap_gen_next]
    add     eax, 1
    je      .Lcmb_wrap                  /* ZF: incremented to 0 -> wrap both   */
    mov     dword ptr [rip + g_cap_gen_next], eax
    mov     r10, qword ptr [rip + g_cap_gen@GOTPCREL]
    mov     dword ptr [r10], eax
    ret
.Lcmb_wrap:
    mov     eax, 1
    mov     dword ptr [rip + g_cap_gen_next], eax
    mov     r10, qword ptr [rip + g_cap_gen@GOTPCREL]
    mov     dword ptr [r10], eax
    ret
RTX_ENDF(rt_cap_match_begin)

/*-----------------------------------------------------------------------------
 * void rt_cap_pop(void *slot)            C of record: pattern_match.c:768
 *   if (s->gen == g_cap_gen && s->sp) s->sp--;
 *
 * Arg: rdi = slot. No return value. Both guards are short-circuit in the C and
 * fall through to a bare ret here, so a stale or empty slot writes NOTHING --
 * matching the C, which must not stamp gen on a pop.
 */
RTX_FUNC(rt_cap_pop)
    RTX_GATE(match, c_rt_cap_pop)
    mov     r10, qword ptr [rip + g_cap_gen@GOTPCREL]
    mov     eax, dword ptr [r10]
    cmp     eax, dword ptr [rdi + 8]    /* s->gen                              */
    jne     .Lcp_ret                    /* stale slot -> no write              */
    mov     eax, dword ptr [rdi + 12]   /* s->sp                               */
    test    eax, eax
    je      .Lcp_ret                    /* empty -> no write                   */
    sub     eax, 1
    mov     dword ptr [rdi + 12], eax
.Lcp_ret:
    ret
RTX_ENDF(rt_cap_pop)

/*-----------------------------------------------------------------------------
 * int rt_cap_top(void *slot)             C of record: pattern_match.c:770
 *   return (s->gen == g_cap_gen && s->sp) ? (int)s->buf[s->sp] : 0;
 *
 * Arg: rdi = slot. Result: eax.
 * NOTE THE INDEX IS s->sp, NOT s->sp - 1, and that is correct rather than an
 * off-by-one: rt_cap_push stores at buf[1 + sp++], so after a push the live
 * frame sits at buf[sp] with buf[0] holding the capacity. Reproduced verbatim.
 * The 32-bit `mov ecx` zero-extends into rcx, so scaling an unsigned sp by 4 in
 * the memory operand needs no explicit widening.
 */
RTX_FUNC(rt_cap_top)
    RTX_GATE(match, c_rt_cap_top)
    xor     eax, eax                    /* default 0 for both failing guards   */
    mov     r10, qword ptr [rip + g_cap_gen@GOTPCREL]
    mov     ecx, dword ptr [r10]
    cmp     ecx, dword ptr [rdi + 8]    /* s->gen                              */
    jne     .Lct_ret
    mov     ecx, dword ptr [rdi + 12]   /* s->sp (zero-extended into rcx)      */
    test    ecx, ecx
    je      .Lct_ret
    mov     rdx, qword ptr [rdi]        /* s->buf                              */
    mov     eax, dword ptr [rdx + rcx*4]/* buf[sp]                             */
.Lct_ret:
    ret
RTX_ENDF(rt_cap_top)

/* Non-executable stack marker: without it ld marks the whole .so RWX-stack. */
.section .note.GNU-stack,"",@progbits
