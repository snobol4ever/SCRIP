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

/*=============================================================================
 * RTX-8 SLICE 2 (s215) -- THE DEFER OPEN/CLOSE PAIR.
 *
 * MEASURED TARGET, NOT AN INHERITED ONE. 0(d) re-run this session with
 * scripts/util_rtx_defer_arm_census.c (arm-level, because the s214 census counts
 * CALLS and s212's WRONG-TEMPLATE-ARM lesson says a call count never identifies
 * an ARM). json.sno + twitter.json, mode 3:
 *     open: plain=402121  star=0  fail=0  null=0  ival_nonzero=0
 *     close=402121 (exactly balanced with open)   step=0
 *     close arms: fail=363155 (90.3%)  advance-by-1=38966  adv0/adv2-8/adv>8=0
 * THREE CONSEQUENCES, ALL LOAD-BEARING:
 *  (1) The hot arm is the PLAIN NV_GET arm. The '*' star arm is DEAD here, so a
 *      port of the star path would grade a guaranteed 1.000x -- exactly the batch
 *      s213 lost. The star arm is left in C and tail-jumped to.
 *  (2) ival_flag is provably 0 on every call (bb_match_defer emits xor esi,esi at
 *      both call sites). Nonzero is still handled -- by bailing to C.
 *  (3) close's hot work is a ONE-BYTE literal compare that C reaches through a
 *      strncmp@PLT call inside an -O0 frame carrying a 40-byte char nb[40]
 *      scratch buffer it does not use on this arm. That call is the win.
 * rt_defer_step is NOT ported: 432 static sites tree-wide, 82 in json.s, and
 * ZERO dynamic calls -- confirmed by re-running the census rather than citing
 * s214's numbers (the s210 STALE CENSUS rule).
 *
 * NO SPEED NUMBER IS QUOTED FOR THIS SLICE AND THE REASON IS THE INSTRUMENT:
 * json.sno on twitter.json self-times its match window at 176 ms, and the
 * harness's own hard gate suppresses any ratio from a window under MIN_MS=800.
 * A longer-window defer benchmark is owed BEFORE this slice may claim a ratio;
 * see the FINDING. Correctness and code-size are what this slice may be graded
 * on today. Quoting 176 ms as a board would be the RTX-0b/0c/0d mistake again.
 *
 * BAIL-BEFORE-MUTATE IS THE STRUCTURAL RULE HERE. Both C bodies mutate the LIFO
 * top (open pushes, close pops) and the C fallback would repeat that mutation,
 * so every tail-jump to c_* MUST happen while g_dfx_top is still untouched.
 * open therefore tests all of its bail conditions before the push, and close
 * PEEKS at g_dfx[top-1] and only decrements once it has committed to an arm it
 * handles end to end. Getting this backwards double-pushes or double-pops a
 * recursion stack, which the manual's own recursion section (p.122-123) says is
 * reachable by design: a deferred value may itself be a pattern whose match runs
 * another deferred reference, so these frames nest.
 */

/*-----------------------------------------------------------------------------
 * long rt_defer_open(const char *varname, int ival_flag)
 *   C of record: pattern_match.c c_rt_defer_open. rdi = varname, esi = ival_flag.
 *   Returns fbytes when a call is owed, 0 when resolved or failed.
 *
 * ASM ARM: non-null varname, varname[0] != '*', name != "FAIL", ival_flag == 0,
 * dfx already carved and not full. Everything else -> c_rt_defer_open untouched.
 * Manual p.86: the deferred operator fetches the THEN-CURRENT value at match
 * time, which is why the NV_GET cannot be hoisted or cached here.
 */
RTX_FUNC(rt_defer_open)
    RTX_GATE(match, c_rt_defer_open)
    test    esi, esi
    jne     c_rt_defer_open             /* ival_flag != 0: NAMEVAL/NAMEPTR deref */
    test    rdi, rdi
    jz      c_rt_defer_open             /* NULL name                            */
    mov     al, byte ptr [rdi]
    cmp     al, 0x2A                    /* '*' -> star transfer, C owns it       */
    je      c_rt_defer_open
    cmp     al, 0x46                    /* 'F' -- only then can it be "FAIL"     */
    jne     .Ldo_push
    cmp     dword ptr [rdi], 0x4C494146 /* 'F''A''I''L' little-endian            */
    jne     .Ldo_push
    cmp     byte ptr [rdi + 4], 0
    je      c_rt_defer_open             /* exactly "FAIL" -> C sets failed       */
.Ldo_push:
    mov     r10, qword ptr [rip + g_dfx]
    test    r10, r10
    jz      c_rt_defer_open             /* not yet carved: C does the rt_cas     */
    mov     eax, dword ptr [rip + g_dfx_top]
    cmp     eax, dword ptr [rip + g_dfx_cap]
    jge     c_rt_defer_open             /* overflow: C reports and aborts        */
    lea     r11, [rax + rax*2]          /* top*3 ... *8 below == stride 24       */
    lea     r11, [r10 + r11*8]          /* r11 = &g_dfx[top]                     */
    add     eax, 1
    mov     dword ptr [rip + g_dfx_top], eax
    /* s->val = NULVCL: v=DT_SNUL(0), slen=0, s=&"" ; then failed=0, dtx_used=0.
     * The empty-string pointer is a distinct address from the C's "" literal and
     * that is observationally identical: close reads (val.s ? val.s : "") and
     * derives llen from strlen when slen==0, so both spell length 0. */
    xor     eax, eax
    mov     qword ptr [r11], rax
    lea     rdx, [rip + .Lrtx_dfx_nul]
    mov     qword ptr [r11 + 8], rdx
    mov     dword ptr [r11 + 16], eax
    mov     dword ptr [r11 + 20], eax
    RTX_CALL_ALIGN
    push    r11
    sub     rsp, 8
    call    NV_GET_fn                   /* rdi still = varname; -> rdx:rax       */
    add     rsp, 8
    pop     r11
    RTX_CALL_UNALIGN
    cmp     eax, DT_X
    je      .Ldo_dtx                    /* unevaluated expression: owed call     */
    mov     qword ptr [r11], rax        /* s->val = val (v|slen then value)      */
    mov     qword ptr [r11 + 8], rdx
    xor     eax, eax
    ret
.Ldo_dtx:
    /* s->dtx_used = 1; fb = rt_proc_call_open(val.s ? val.s : "", 0);
     * if (!fb) s->failed = 1; return fb;   -- val.s is in rdx from NV_GET_fn. */
    mov     dword ptr [r11 + 20], 1
    test    rdx, rdx
    jnz     .Ldo_dtx_arg
    lea     rdx, [rip + .Lrtx_dfx_nul]
.Ldo_dtx_arg:
    RTX_CALL_ALIGN
    push    r11
    sub     rsp, 8
    mov     rdi, rdx
    xor     esi, esi
    call    rt_proc_call_open
    add     rsp, 8
    pop     r11
    RTX_CALL_UNALIGN
    test    rax, rax
    jnz     .Ldo_dtx_ret
    mov     dword ptr [r11 + 16], 1     /* s->failed = 1                        */
.Ldo_dtx_ret:
    ret
RTX_ENDF(rt_defer_open)

/*-----------------------------------------------------------------------------
 * int rt_defer_close(int cur_delta)
 *   C of record: pattern_match.c c_rt_defer_close. edi = cur_delta.
 *   Returns the new cursor, or -1 to fail the match.
 *
 * ASM ARM: top > 0 and the popped frame is failed, DT_FAIL, or a DT_S/DT_SNUL
 * whose slen is AUTHORITATIVE (nonzero, high bit clear). Bails to C for DT_I and
 * DT_R (which stringify through snprintf), for DT_P/DT_X/anything else, and for
 * the two slen encodings RTX-3 had to learn the hard way: slen==0 means LENGTH
 * UNKNOWN (strlen is owed), and slen==0xFFFFFFFF means CSET -- and a CSET still
 * carries tag DT_S, so the tag alone cannot be trusted to mean "plain string".
 * The peek happens before any decrement so those bails leave the LIFO untouched.
 */
RTX_FUNC(rt_defer_close)
    RTX_GATE(match, c_rt_defer_close)
    mov     eax, dword ptr [rip + g_dfx_top]
    test    eax, eax
    jle     .Ldc_empty                  /* top <= 0 -> -1, nothing to pop        */
    mov     r10, qword ptr [rip + g_dfx]
    test    r10, r10
    jz      c_rt_defer_close
    lea     r11d, [rax - 1]             /* peek index = top-1                    */
    lea     r11, [r11 + r11*2]
    lea     r11, [r10 + r11*8]          /* r11 = &g_dfx[top-1], NOT popped yet   */
    cmp     dword ptr [r11 + 16], 0     /* s.failed                             */
    jne     .Ldc_pop_fail
    mov     ecx, dword ptr [r11]        /* val.v                                */
    cmp     ecx, DT_FAIL
    je      .Ldc_pop_fail
    cmp     ecx, DT_S
    je      .Ldc_str
    cmp     ecx, DT_SNUL
    jne     c_rt_defer_close            /* DT_I/DT_R/DT_P/DT_X/other -> C        */
.Ldc_str:
    mov     ecx, dword ptr [r11 + 4]    /* val.slen                             */
    test    ecx, ecx
    jz      c_rt_defer_close            /* 0 == LENGTH UNKNOWN, strlen owed      */
    test    ecx, 0x80000000
    jnz     c_rt_defer_close            /* CSET / absurd length -> C verbatim    */
    mov     rdx, qword ptr [r11 + 8]    /* val.s                                */
    test    rdx, rdx
    jz      c_rt_defer_close            /* NULL payload with nonzero slen -> C   */
    /* Committed to this arm: pop, then bounds-check and compare. */
    sub     dword ptr [rip + g_dfx_top], 1
    mov     r10, qword ptr [rip + Σlen@GOTPCREL]
    mov     r10d, dword ptr [r10]       /* Σlen                                */
    mov     eax, edi                    /* cur_delta                            */
    add     eax, ecx                    /* cur_delta + llen                     */
    cmp     eax, r10d
    jg      .Ldc_fail                   /* runs off the end of the subject       */
    mov     r10, qword ptr [rip + Σ@GOTPCREL]
    mov     r10, qword ptr [r10]        /* Σ base pointer                   */
    mov     esi, edi
    add     r10, rsi                    /* Σ + cur_delta                    */
    cmp     ecx, 1
    jne     .Ldc_cmpn
    mov     sil, byte ptr [r10]         /* the 90.3%-of-traffic single byte      */
    cmp     sil, byte ptr [rdx]
    jne     .Ldc_fail
    ret                                 /* eax already == cur_delta + 1          */
.Ldc_cmpn:
    mov     r8, rdi                     /* preserve nothing else is live         */
    mov     rsi, r10
    mov     rdi, rdx
    push    rax
    mov     ecx, ecx                    /* zero-extend llen into rcx             */
    cld
    repe    cmpsb
    pop     rax
    mov     rdi, r8
    jne     .Ldc_fail
    ret
.Ldc_pop_fail:
    sub     dword ptr [rip + g_dfx_top], 1
.Ldc_fail:
.Ldc_empty:
    mov     eax, -1
    ret
RTX_ENDF(rt_defer_close)

.section .rodata
.align 1
.Lrtx_dfx_nul:
    .byte 0

/*=====================================================================================================================
 * SLICE 4 — rt_match_ctx_restore: THE PATCTX 3-WORD RESTORE.
 *
 * C OF RECORD: src/runtime/builtins/gen_runtime.c:145, renamed c_rt_match_ctx_restore in this same commit.
 *
 * SCOPE, STATED HONESTLY AND UP FRONT: this is an ERADICATION slice (RTX-12), NOT a speed rung, and no speed number
 * may be quoted off it.  The C body is three stores; even a perfect port saves ~10 -O0 instructions against a 205 ms
 * window on pattern_bt.sno, i.e. well under the +-3% null floor the ladder has already established twice (RTX-7's
 * 0.58% rdtsc bound, RTX-5b's falsified 1.03-1.10x board).  What this slice buys is C deleted from the SNOBOL4-
 * reachable runtime.  Grade it on correctness and on kill-switch byte-identity, never on the clock.
 *
 * WHY IT IS THE RIGHT NEXT TARGET ANYWAY (ARCH section 7 step 0, all six checks, re-measured s217 not cited):
 *   0(a) live definition + TWO real template call sites (bb_match_head.cpp:89, bb_match_release.cpp:77)
 *   0(b) name round-trips byte-identical -- plain ASCII, no Greek codepoints, no truncation
 *   0(c) run ON THE OBJECTS: Σ/Σlen are `B` in stmt_exec.o, g_cap_gen is `D` in pattern_match.o -- all three GLOBAL;
 *        and all three appear in the .so DYNAMIC TABLE with DEFAULT visibility (g_cap_gen_next, hidden, correctly
 *        absent -- which is what proves the instrument discriminates).  => @GOTPCREL IS MANDATORY ON ALL THREE.
 *        A direct [rip+sym] would bind a DIFFERENT VARIABLE via copy relocation from a -no-pie executable, with no
 *        diagnostic at any stage (s214).  The idiom is copied verbatim from .Ldc_str above.
 *   0(d) 500,001 entries on pattern_bt.sno; 250,001 at half the loop literal => EXACTLY 2.000x, scaling proven.
 *   0(e) zero .S hits before this commit => not already ported.
 *   0(f) THE ARM CHECK, AND THIS IS THE POINT: the C body is STRAIGHT-LINE -- three unconditional stores, ZERO arms,
 *        ZERO calls, no early return, no predicate of any kind.  There is NO cold arm to bail to and therefore NO
 *        bail edge at all: ENTRIES ARE COMMITS BY CONSTRUCTION.  This is exactly the property rt_cap_push lacked at
 *        s216 (57,578 entries, 57,578 bails, 0 commits, reverted).  A straight-line body is the one shape where
 *        0(f) can be discharged before the asm exists, because the tool that measures it needs a c_* edge to count.
 *
 * SEMANTICS, FROM THE MANUAL (ARCH section 7 step 1, Lon standing directive):
 *   Ch.7 p.86 -- the deferred evaluation operator `*` builds the pattern ONCE but fetches the THEN-CURRENT value at
 *     every match-time reference, and is legal on a pattern's alternate clause, subsequent clause, or the whole
 *     pattern.  So a match can reach arbitrary user expressions mid-scan.
 *   Ch.9 p.123 -- SPITBOL applies NO quickscan heuristics: matching is exhaustive, "deferred expressions are not
 *     assumed to match at least one character, and recursive patterns always work properly."
 *   => RE-ENTRANT MATCHING IS A GUARANTEED LANGUAGE PROPERTY, NOT AN EDGE CASE.  A deferred expression evaluated
 *   mid-match may itself run a pattern match, which installs its own subject and draws its own capture generation.
 *   That is why the outer match's Σ/Σlen mirror and g_cap_gen id must be saved at alpha and restored LIFO at BOTH
 *   exits, and why this function must be an UNCONDITIONAL restore of three saved words with ZERO interpretation:
 *   it is handed the outer values by the template (head.sigma_save +48, head.capgen_save +72) and its whole job is
 *   to put them back.  Any cleverness here -- a skip-if-equal, a decrement instead of a store -- would break the
 *   nest1 invariant (an inner match's stamp must not invalidate the outer match's open capture brackets).
 *
 * WHAT IT DOES NOT TOUCH: the Σ PIN.  r13 is the subject base pointer and the template restores it directly; this
 * function syncs only the C-side global mirror that pattern_match.c / runtime_eval.c read.  Two different stores of
 * the same logical value, and this one is the C half.  Every blob pin (rbx r13 r14 r15 rbp rsp) is preserved here by
 * construction: the body touches rax-class scratch only (r10), never a pin, never the stack.
 *
 * SysV in: rdi = sig (Σ, 8 bytes) . rsi = len (Σlen, int, 4 bytes) . rdx = capgen (g_cap_gen, uint32_t, 4 bytes)
 * The two narrow stores use esi/edx deliberately: the C body casts to (int) and (uint32_t), so a 4-byte store is
 * the FAITHFUL transliteration -- an 8-byte store would clobber the adjacent global (Σlen sits at +0x14 in
 * stmt_exec.o, 4 bytes wide) and that is precisely the class of bug that links fine and passes short tests.
 *===================================================================================================================*/
RTX_FUNC(rt_match_ctx_restore)
    RTX_GATE(match, c_rt_match_ctx_restore)
    mov     r10, qword ptr [rip + Σ@GOTPCREL]       /* &Σ   -- exported, GOT-indirect per 0(c)   */
    mov     qword ptr [r10], rdi                        /* Σ = (const char *)sig                        */
    mov     r10, qword ptr [rip + Σlen@GOTPCREL]     /* &Σlen                                     */
    mov     dword ptr [r10], esi                        /* Σlen = (int)len          -- 4-byte store   */
    mov     r10, qword ptr [rip + g_cap_gen@GOTPCREL]   /* &g_cap_gen                                    */
    mov     dword ptr [r10], edx                        /* g_cap_gen = (uint32_t)capgen -- 4-byte store  */
    ret
RTX_ENDF(rt_match_ctx_restore)

/* rt_patstk_lazy_init — RTX-8 slice 5. THE LAZY-INIT SHAPE: one real init, N-1 no-op tests.
 *
 * C OF RECORD: pattern_match.c:620, renamed c_rt_patstk_lazy_init in this same commit.
 *
 * ARM CENSUS (0(f-pre), read from the C body, then measured):
 *   ARM 1  g_patstk_sp == 0  -> COLD. Reserves the 8 MB pend island via rt_slab_region,
 *                              aborts on failure, computes and 16-aligns the descending sp.
 *                              Runs EXACTLY ONCE per process. Delegated whole to C.
 *   ARM 2  g_patstk_sp != 0  -> HOT. Nothing to do; return. 4,000,000 of 4,000,001 calls
 *                              on pattern_bt.sno at LT(N,4000000).
 * Measured 0(d): 1,000,001 entries at K=1M and 2,000,001 at K=2M -- EXACTLY 2.000x.
 *
 * BAIL-BEFORE-MUTATE IS FREE HERE, AND THAT IS THE WHOLE REASON THIS TARGET IS SAFE:
 * the asm MUTATES NOTHING. It performs one load and one compare, then either returns or
 * tail-delegates to a C body that re-does the test itself. So the s215 double-push/double-pop
 * hazard cannot arise, and the cold path is bit-identical to pristine C by construction.
 *
 * g_patstk_sp IS EXPORTED AND MUST GO THROUGH THE GOT (ARCH §7 step 0(c), verified on
 * out/rt_pic/pattern_match.o -- capital `B`, and present in the .so dynamic table because
 * bb_match_head/bb_match_fence1/bb_match_release all read it from emitted code). A direct
 * [rip+g_patstk_sp] here would bind a DIFFERENT variable via copy relocation when a -no-pie
 * executable owns the storage -- silent island divergence, no diagnostic at any stage (s214).
 * g_patstk_base stays `static` (lowercase `b` in the object) and is UNREFERENCEABLE from a .S;
 * this port never touches it, only the C cold path does.
 *
 * NO SPEED NUMBER IS CLAIMED. Measured: the whole MATCH family gate moves pattern_bt by zero
 * (ON median 903 ms vs OFF 890 ms at LT(N,4000000)), and this container throws intermittent
 * +50% fliers, so the per-call saving here (~5 instructions against ~650 cycles per match)
 * is far below the floor. Graded on correctness and eradication, per the RTX-7/s217 precedent. */
RTX_FUNC(rt_patstk_lazy_init)
    RTX_GATE(match, c_rt_patstk_lazy_init)
    mov     r10, qword ptr [rip + g_patstk_sp@GOTPCREL]  /* &g_patstk_sp -- exported, GOT-indirect per 0(c) */
    cmp     qword ptr [r10], 0                           /* the island already reserved?                     */
    je      c_rt_patstk_lazy_init                        /* ARM 1 COLD: first match -- whole body to C       */
    ret                                                  /* ARM 2 HOT: nothing to do                         */
RTX_ENDF(rt_patstk_lazy_init)

/*===================================================================================================================
 * rt_match_enter — RTX-8 SLICE 6: THE HOT-PATH COLLAPSE.  The first MATCH rung that removes CALLS, not instructions.
 *
 * C OF RECORD: src/runtime/builtins/gen_runtime.c:127, renamed c_rt_match_enter in this same commit.
 *
 * WHY THIS RUNG EXISTS: s219 measured that the fixed per-match chain is SIX calls, one per match, and that porting
 * the LEAVES moves nothing -- toggling the whole MATCH family gate on the graded workload gave medians 903 vs 890 ms,
 * i.e. ZERO.  Three of those six links were already asm and the board had not moved.  So the lever is not another
 * leaf: it is collapsing the callees INTO their one caller, which is this function.  Per match the C performs
 *   rt_cap_match_begin() . rt_dcap_lazy_init() . rt_patstk_lazy_init() . VARVAL_fn() . strlen()
 * and FOUR of those five are inlined here.  Only strlen survives as a call, because the C's own semantics demand a
 * fresh length every match (see THE SLEN FINDING below) and glibc's SSE strlen beats any hand byte-loop on the
 * 50-byte subjects this workload uses.
 *
 * ⛔⛔ THE SLEN FINDING, AND IT INVERTED THIS RUNG'S FIRST DESIGN -- READ IT BEFORE EDITING ANY ARM.
 * The obvious fast arm is "DT_S with a cached slen => return slen, no strlen."  MEASURED on pattern_bt at
 * LT(N,2000000): that arm fires 0 times out of 2,000,001, and the slen==0 arm fires 2,000,001 -- ONE HUNDRED
 * PERCENT.  Cause: STRVAL (core.h) mints DT_S with .slen = 0, and only BSTRVAL populates it, so a plain SNOBOL4
 * string assignment never carries its length.  A port built on the cached-slen arm would assemble, gate green, be
 * byte-identical over the whole suite, and move NOTHING -- the s217 slice-3 vacuous-rung mode exactly.
 * ⭐ THIS IS NOW A CROSS-LANGUAGE FACT WITH TWO INDEPENDENT MEASUREMENTS: the Icon side reached the identical
 * conclusion at s217-ICN (FINDING ...ICN-RTX-9-SUBSCRIPT-GET2-FAST-ARM-WOULD-HAVE-BEEN-VACUOUS-AND-SLEN-IS-NEVER-
 * POPULATED: arr.slen == 0 on 100% of arrivals, "slen usable = 0" on both a synthetic and a real corpus).
 * ⇒ NO RTX FAST ARM IN EITHER LANGUAGE MAY BE KEYED ON slen != 0 until a LOWER-side rung populates it.  Populating
 * slen at assignment would delete 2,000,001 strlens per run here -- a LOWER/descriptor rung, NOT an RTX rung, and
 * worth more than this collapse.  Recorded rather than acted on, same discipline as RTX-5's "an asm port would
 * faithfully reproduce a poor algorithm at high speed."
 *
 * ARMS, AS MEASURED (interposer classifying rdi/rsi per call, 2,000,001 arrivals, pattern_bt at K=2M):
 *   ARM-FAST-B  DT_S . s != NULL . slen == 0 . *s != 0   -> 2,000,001 (100%).  Inlined here; L = strlen(s).
 *   ARM-FAST-A  DT_S . s != NULL . slen != 0             ->         0.  Inlined anyway (one branch): L = slen,
 *                                                           which is what the C computes because VARVAL_fn's DT_S
 *                                                           arm returns v.s, making the C's `s == sv.s` conjunct
 *                                                           trivially true.  A BSTRVAL subject lands here.
 *   DELEGATED   v != DT_S (INT/REAL need descr_to_str, and every other tag needs VARVAL_fn's switch)
 *               s == NULL (VARVAL_fn ALLOCATES via rt_ws_strdup_c -- never replicate an allocation in asm)
 *               slen == 0 && *s == 0 (IS_NULL_fn true => C returns the "" LITERAL, a different pointer than v.s;
 *                               Σ must receive that literal, so this arm is C's by right, not by convenience)
 *
 * ⛔ BAIL-BEFORE-MUTATE IS LOAD-BEARING HERE AND IS NOT FREE THE WAY IT WAS IN SLICE 5.  rt_cap_match_begin is NOT
 * idempotent -- it bumps the monotonic generation well on every call.  So a delegation that happened AFTER the
 * inlined bump would make c_rt_match_enter bump a SECOND time, issuing two generations for one match and retiring
 * capture brackets the outer match still owns: the s215 double-push/double-pop hazard, silent, no diagnostic.
 * THEREFORE all three delegation edges are placed BEFORE .Lme_mutate and nothing above that label writes memory.
 * The two COLD edges below .Lme_mutate call the INDIVIDUAL helpers, never the whole body, for the same reason.
 *
 * 0(c) LINKAGE, READ ON out/rt_pic/*.o AND NEVER ON THE .so (ARCH §7):
 *   Σ  B  + in dyn table  -> @GOTPCREL      Σlen  B @0x14 + in dyn table -> @GOTPCREL
 *   g_cap_gen  D + in dyn table -> @GOTPCREL          g_patstk_sp  B + in dyn table -> @GOTPCREL
 *   g_cap_gen_next  D in the object but ABSENT from the dyn table (hidden) -> direct [rip+sym]
 * ⛔ g_cap_gen @0x0 and g_cap_gen_next @0x4 are ADJACENT and BOTH 4 bytes, and NOTHING in the tree defends that
 * adjacency (s218).  Every store to either is therefore `dword ptr`, and each is addressed BY ITS OWN NAME rather
 * than as [r10+4] off its sibling, so this port carries no dependency on the layout it cannot enforce.  Σlen is
 * likewise `int`, so its store is dword: an 8-byte store there clobbers the neighbour, links fine, passes short
 * tests -- the s217 store-width class, now gated by scripts/test_gate_rtx_store_width.sh.
 *
 * g_dcap_top is NOT a symbol: it is #define g_dcap_top (*(const char **)RT_CAS_TOP), a BUILD-CONSTANT pinned VA
 * (pin_va.h, RT_PIN_BASE 0x70000000).  It is reached by absolute disp32 -- the same 48 8b 04 25 / 48 83 3c 25 form
 * ABSQ() bakes in the templates, no GOT and no RIP-relative -- and rtx_init.c carries a _Static_assert tying
 * RTX_CAS_TOP_VA below to RT_CAS_TOP, so moving the pin breaks the BUILD rather than the runtime.
 *
 * PINS: touches rax rdx rdi rsi r8 r10 and a balanced rsp only.  rbx r13 r14 r15 rbp are untouched by construction,
 * and the cold edges deliberately DO NOT use RTX_CALL_ALIGN because that macro borrows rbp, which is a per-graph pin
 * (FLATDISP-8); they align by push count instead -- entry rsp is 8 (mod 16), so push+push+sub 8 reaches 0, and in
 * the strlen edge the single `push rsi` both aligns AND preserves the subject pointer.
 *
 * NO SPEED NUMBER IS CLAIMED IN THIS COMMENT.  The rung's grading is in the FINDING, min-of-N on pattern_bt, and
 * the s219 refusal precedent stands: if the measurement does not clear the null floor, it is reported as a null.
 *===================================================================================================================*/
#define RTX_CAS_TOP_VA 0x70000000
RTX_FUNC(rt_match_enter)
    RTX_GATE(match, c_rt_match_enter)
    cmp     edi, DT_S                                    /* edi aliases the tag word; v == DT_S?              */
    jne     c_rt_match_enter                             /* DELEGATE: INT/REAL/every other tag                */
    test    rsi, rsi                                     /* sv.s == NULL?                                     */
    jz      c_rt_match_enter                             /* DELEGATE: VARVAL_fn allocates on that arm          */
    mov     r8, rdi
    shr     r8, 32                                       /* r8 = slen, zero-extended; ZF set from the result  */
    jnz     .Lme_mutate                                  /* ARM-FAST-A: cached length, no strlen owed         */
    cmp     byte ptr [rsi], 0                            /* slen == 0: is the string empty?                   */
    je      c_rt_match_enter                             /* DELEGATE: IS_NULL_fn true -> C's "" literal        */
.Lme_mutate:
    mov     eax, dword ptr [rip + g_cap_gen_next]        /* --- inlined rt_cap_match_begin ---                */
    add     eax, 1
    jne     .Lme_gen_ok                                  /* wrapped to 0? generation 0 is the fresh sentinel  */
    mov     eax, 1                                       /* reset BOTH to 1 so they stay in agreement          */
.Lme_gen_ok:
    mov     dword ptr [rip + g_cap_gen_next], eax        /* hidden -> direct; DWORD, adjacency-safe            */
    mov     r10, qword ptr [rip + g_cap_gen@GOTPCREL]
    mov     dword ptr [r10], eax                         /* exported -> GOT; DWORD, adjacency-safe             */
    cmp     qword ptr [RTX_CAS_TOP_VA], 0                /* --- inlined rt_dcap_lazy_init test (pinned VA) --- */
    je      .Lme_dcap_cold
.Lme_dcap_done:
    mov     r10, qword ptr [rip + g_patstk_sp@GOTPCREL]  /* --- inlined rt_patstk_lazy_init test ---           */
    cmp     qword ptr [r10], 0
    je      .Lme_patstk_cold
.Lme_patstk_done:
    test    r8d, r8d                                     /* ARM-FAST-A already holds L                        */
    jnz     .Lme_store
    mov     rdi, rsi                                     /* ARM-FAST-B: L = strlen(s), the one surviving call  */
    push    rsi                                          /* aligns rsp 8->0 (mod 16) AND preserves the subject */
    call    strlen@PLT
    pop     rsi
    mov     r8, rax                                      /* L is uint64_t in the C -- keep all 64 bits         */
.Lme_store:
    mov     r10, qword ptr [rip + Σ@GOTPCREL]
    mov     qword ptr [r10], rsi                         /* Σ = s                                             */
    mov     r10, qword ptr [rip + Σlen@GOTPCREL]
    mov     dword ptr [r10], r8d                         /* Σlen = (int)L -- DWORD, never qword                */
    mov     rax, rsi                                     /* r.ptr = s                                          */
    mov     rdx, r8                                      /* r.len = L                                          */
    ret
.Lme_dcap_cold:
    push    rsi
    push    r8
    sub     rsp, 8                                       /* 8 -> 0 (mod 16) for the SysV call                  */
    call    rt_dcap_lazy_init                            /* the INDIVIDUAL helper, never the whole body        */
    add     rsp, 8
    pop     r8
    pop     rsi
    jmp     .Lme_dcap_done
.Lme_patstk_cold:
    push    rsi
    push    r8
    sub     rsp, 8
    call    c_rt_patstk_lazy_init                        /* c_* directly: idempotent, re-tests g_patstk_sp     */
    add     rsp, 8
    pop     r8
    pop     rsi
    jmp     .Lme_patstk_done
RTX_ENDF(rt_match_enter)

/* Non-executable stack marker: without it ld marks the whole .so RWX-stack. */
.section .note.GNU-stack,"",@progbits
