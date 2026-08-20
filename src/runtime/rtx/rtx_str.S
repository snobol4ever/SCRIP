/* rtx_str.S — RTX family STR (rung RTX-3, s164).
 *
 * READ .github/ARCH-SNOBOL4-RTX.md BEFORE EDITING. Contract macros: rtx_abi.inc.
 *
 * WHAT THIS FAMILY ACTUALLY IS (corrected s164 — the RTX-2 lesson, repeated):
 *   ARCH section 5 / the RTX-3 rung name `rt_concat`, `rt_lcomp` and `rt_acomp`. All three are
 *   DEAD: they exist ONLY as declarations in rt/rt.h (lines 69/80/81), have NO definition
 *   anywhere in the tree, and have ZERO call sites. They are vestigial prototypes, the exact
 *   shape of the blk_alloc/blk_free phantom that cost RTX-2 a rung. Step 0 of the ARCH section 7
 *   protocol caught them in two minutes.
 *   The LIVE string surface, from the fixed inventory script over the 26 live artifacts, is:
 *     str_concat_d 152 (#2 overall, behind only rt_call_arr) . memcmp 106 (libc, stays) .
 *     rt_gvar_assign_str 44 + rt_gvar_assign_concat_parts 9 (NV family, rung RTX-7) .
 *     rt_coerce_str_d 2 static (indirect reach only — ARCH section 5 caveat (a)).
 *   So STR is str_concat_d, and this slice ports exactly that.
 *
 * WHAT IS PORTED: the TWO-PLAIN-STRINGS FAST PATH ONLY — both operands DT_S with an
 * authoritative nonzero length and a non-null payload, no collection pending, and the SXT
 * extend-in-place token NOT matching the left operand. That is the ordinary `A B` concat.
 * Under exactly those conditions the C body provably reduces to: carve, copy, copy, NUL,
 * re-arm the SXT token, return BSTRVAL — and every one of its ten helper calls
 * (rt_gc_point_arr, descr_to_str x2, VARVAL_fn x2, rt_sxt_match, memcpy x2, rt_sxt_note)
 * is eliminated rather than merely skipped.
 *
 * WHAT IS DELIBERATELY *NOT* PORTED (C keeps sole ownership — contract section 4):
 *   pattern concat (DT_P/DT_X -> pat_cat) . FAIL propagation . the NULL-operand identity .
 *   list concat (DT_DATA "list") . every coercion (DT_I/DT_R/DT_DATA operands) . CSET
 *   operands (slen == 0xFFFFFFFF) . unknown-length strings (slen == 0, which means "call
 *   strlen", NOT "empty") . the SXT extend-in-place path with its GVA alias scan . any
 *   pending collection. Each falls through to c_str_concat_d with rdi/rsi/rdx/rcx UNTOUCHED,
 *   so the tail jump needs no argument staging and is the byte-identity proof path.
 *
 * SPITBOL MANUAL SEMANTICS THIS SLICE MUST NOT BREAK (v3.7, Concatenation, p.21-22):
 *   (a) "The two strings remain unchanged and a third string emerges as the result." This is
 *       precisely why the SXT extend-in-place trick needs its ownership token, and why we hand
 *       any token match back to C rather than reimplementing the alias analysis here.
 *   (b) "if either operand is the null string, the other operand is returned unchanged. It is
 *       not coerced into the string data type" — the manual's own example is (20 - 17) ''
 *       yielding the INTEGER 3, not the string '3'. Type-preserving identity. Our guard
 *       requires slen != 0 on BOTH sides, so every null-operand case reaches C untouched.
 *
 * THE DESCR_t SLEN TRAP (core.h descr_slen, and the reason the guard is what it is):
 *   slen == 0          => length UNKNOWN, C calls strlen. NOT an empty string.
 *   slen == 0xFFFFFFFF => the operand is a CSET, not an ordinary string.
 *   Only a nonzero, non-sentinel slen is an authoritative byte count. Both are excluded.
 *
 * GC SAFETY (the one thing that would make this unsound if gotten wrong):
 *   rt_gc_point_arr is `if (!g_gc_pending) return;` — nothing more. So the C body's shielded
 *   collection point costs one compare here, and a pending collection routes to C. The
 *   allocator itself never collects: gc_heap.c only SETS g_gc_pending (lines 212/217) and
 *   collection happens exclusively at explicit gc_points, so no descriptor can move under us
 *   between the guard and the return. This is what makes holding raw a.s/b.s across the carve
 *   safe — the same property the C body already relies on.
 *
 * ARGS (SysV, two 16-byte all-INTEGER structs — contract section 2):
 *   a -> rdi = (a.slen << 32) | a.v , rsi = a.s
 *   b -> rdx = (b.slen << 32) | b.v , rcx = b.s
 * RETURN: rax = (len << 32) | DT_S , rdx = buf
 */
#include "rtx_abi.inc"

RTX_GATE_DEF(str)

#define HBF_TTL 0x0001

/*-----------------------------------------------------------------------------
 * RTX_MEMCPY — copy rcx bytes from rsi to rdi. EXACT BOUNDS, never one byte past
 * the requested length, so it is safe against any carve rounding assumption.
 * Clobbers rax, r11, rcx, rsi, rdi, xmm0, xmm1. Callers guarantee rcx >= 1.
 *
 * Shape is the standard overlapping-load memcpy: for every size below the rep
 * threshold, two loads and two stores that together cover [0,len) with a middle
 * overlap, which is branch-light and avoids rep movsb's ~30-cycle startup on the
 * short strings that dominate SNOBOL4 concatenation (tokens and single chars).
 */
.macro RTX_MEMCPY
    cmp     rcx, 16
    jb      201f
    cmp     rcx, 32
    ja      200f
    movdqu  xmm0, [rsi]                 /* 16..32: two overlapping 16B          */
    movdqu  xmm1, [rsi + rcx - 16]
    movdqu  [rdi], xmm0
    movdqu  [rdi + rcx - 16], xmm1
    jmp     209f
200:
    rep movsb                           /* >32: let the hardware string engine  */
    jmp     209f
201:
    cmp     rcx, 8
    jb      202f
    mov     rax, [rsi]                  /* 8..15: two overlapping 8B            */
    mov     r11, [rsi + rcx - 8]
    mov     [rdi], rax
    mov     [rdi + rcx - 8], r11
    jmp     209f
202:
    cmp     rcx, 4
    jb      203f
    mov     eax, [rsi]                  /* 4..7: two overlapping 4B             */
    mov     r11d, [rsi + rcx - 4]
    mov     [rdi], eax
    mov     [rdi + rcx - 4], r11d
    jmp     209f
203:
    mov     al, [rsi]                   /* 1..3: ends, then the middle of 3     */
    mov     r11b, [rsi + rcx - 1]
    mov     [rdi], al
    mov     [rdi + rcx - 1], r11b
    cmp     rcx, 3
    jb      209f
    mov     al, [rsi + 1]
    mov     [rdi + 1], al
209:
.endm

/*-----------------------------------------------------------------------------
 * DESCR_t str_concat_d(DESCR_t a, DESCR_t b)
 *   C of record: src/runtime/string_ops.c c_str_concat_d
 *
 *   The C this reproduces, once the guard below has excluded everything else:
 *     al = a.slen; bl = b.slen;
 *     buf = rt_str_alloc(al + bl);            <- carves al+bl+1, NUL slot included
 *     memcpy(buf, a.s, al); memcpy(buf + al, b.s, bl); buf[al + bl] = 0;
 *     rt_sxt_note(buf, al + bl);
 *     return BSTRVAL(buf, al + bl);
 */
RTX_FUNC(str_concat_d)
    RTX_GATE(str, c_str_concat_d)
    cmp     edi, DT_S                   /* a.v == DT_S (tag aliases edi)        */
    jne     .Lsc_null                   /* not two plain strings -> null arm    */
    cmp     edx, DT_S                   /* b.v == DT_S                          */
    jne     .Lsc_null
    test    rsi, rsi                    /* a.s non-null (else VARVAL_fn strdups)*/
    je      .Lsc_slow
    test    rcx, rcx                    /* b.s non-null                         */
    je      .Lsc_slow
    mov     r8, rdi
    shr     r8, 32                      /* r8 = a.slen, zero-extended           */
    je      .Lsc_slow                   /* slen 0 => unknown length, C strlens  */
    cmp     r8d, -1
    je      .Lsc_slow                   /* slen ~0 => CSET, not a string        */
    mov     r9, rdx
    shr     r9, 32                      /* r9 = b.slen                          */
    je      .Lsc_slow
    cmp     r9d, -1
    je      .Lsc_slow
    mov     rax, [rip + g_gc_pending@GOTPCREL]
    cmp     dword ptr [rax], 0
    jne     .Lsc_slow                   /* collection pending => C's gc point   */
    lea     rax, [rip + g_sxt_fr]       /* hidden => direct lea, no GOT hop     */
    mov     r10d, dword ptr [rax + 20]  /* off: -1 unresolved, 1 SXT disabled   */
    test    r10d, r10d
    js      .Lsc_slow                   /* unresolved => C resolves the getenv  */
    jnz     .Lsc_nosxt                  /* disabled => token cannot match       */
    cmp     rsi, [rax + 0]              /* a.s == g_sxt_owner ?                 */
    je      .Lsc_slow                   /* token hit => C owns extend-in-place  */
.Lsc_nosxt:
    push    rsi                         /* a.s  — live across the carve         */
    push    rcx                         /* b.s                                  */
    push    r8                          /* al                                   */
    push    r9                          /* bl                                   */
    sub     rsp, 8                      /* entry rsp==8 (mod 16); 8-40 => 0     */
    lea     rdi, [r8 + r9]              /* n = al + bl (rt_str_alloc adds NUL)  */
    call    rt_str_alloc
    add     rsp, 8
    pop     r9
    pop     r8
    pop     rcx
    pop     rsi
    mov     r10, rax                    /* r10 = buf, preserved through copies  */
    mov     rdx, rcx                    /* park b.s: rcx is the rep counter     */
    mov     rdi, rax
    mov     rcx, r8                     /* rsi = a.s already                    */
    RTX_MEMCPY                          /* buf[0 .. al)                         */
    lea     rdi, [r10 + r8]
    mov     rsi, rdx
    mov     rcx, r9
    RTX_MEMCPY                          /* buf[al .. al+bl)                     */
    lea     r11, [r8 + r9]              /* len = al + bl                        */
    mov     byte ptr [r10 + r11], 0
/* rt_sxt_note, inline and FAITHFUL — it clears the token unconditionally and re-arms only
 * when the block is a DT_S title ending exactly at the heap frontier. That condition is NOT
 * automatic: c_rt_str_alloc can satisfy a request from the fill window or a recycled block,
 * in which case C refuses to arm and so must we. The arena lower-bound test in the C is
 * subsumed — a block whose end equals g_hp_top and whose title validates is in-arena. */
    lea     rax, [rip + g_sxt_fr]
    mov     qword ptr [rax + 0], 0      /* g_sxt_owner = 0, unconditionally     */
    mov     rdx, [rip + g_hp_fr@GOTPCREL]
    mov     rdx, [rdx + 0]              /* g_hp_top                             */
    lea     rsi, [r10 - 16]             /* h = title of buf                     */
    test    word ptr [rsi + 14], HBF_TTL
    je      .Lsc_ret
    cmp     word ptr [rsi + 12], DT_S
    jne     .Lsc_ret
    mov     ecx, dword ptr [rsi + 8]    /* h->size                              */
    add     rcx, rsi
    cmp     rcx, rdx                    /* h + h->size == g_hp_top ?            */
    jne     .Lsc_ret
    mov     [rax + 0], r10              /* owner = buf                          */
    mov     [rax + 8], r11              /* len   = al + bl                      */
.Lsc_ret:
    mov     rax, r11
    shl     rax, 32
    or      rax, DT_S                   /* rax = (len << 32) | DT_S             */
    mov     rdx, r10                    /* rdx = buf                            */
    ret
/*
 * .Lsc_null -- THE TYPE-PRESERVING NULL IDENTITY (SPITBOL manual v3.7, p.22).
 *
 *   "It is always possible for concatenation to automatically convert a number to a
 *    string. But there is one important exception ... if either operand is the null
 *    string, the other operand is returned unchanged. It is not coerced into the
 *    string data type."          ?= (20 - 17) ''   ==>   3        (the INTEGER 3)
 *
 * Reached when the two-plain-strings test above failed on EITHER tag -- which is the
 * shape of every measured hot call: SNUL+I (the LT(...) loop-guard idiom), I+SNUL and
 * S+SNUL. c_str_concat_d services those with a full -O0 prologue, an rt_gc_point_arr
 * call with a 32-byte stack shadow array, and six tag compares, in order to return one
 * of its own arguments unchanged.
 *
 * GUARD ORDER IS DERIVED FROM c_str_concat_d AND MUST STAY EQUIVALENT TO IT:
 *   1. !g_gc_pending     - else C's gc point relocates a/b and does real work.
 *   2. neither DT_P/DT_X - pattern concat (pat_cat) takes precedence.
 *   3. neither DT_FAIL   - FAILDESCR takes precedence, and a DT_FAIL operand is NOT
 *                          necessarily bit-identical to FAILDESCR{.v=DT_FAIL,.i=0}.
 *   4. THEN a.v==DT_SNUL => return b ; b.v==DT_SNUL => return a.
 *
 * The SNUL test is hoisted above guards 2/3 in the a-arm ONLY because a.v==DT_SNUL(0)
 * already proves a is not DT_P(3), DT_X(15) or DT_FAIL(99) -- so only the OTHER operand
 * still needs guarding. Identical condition set to the C, half the compares.
 *
 * DT_SNUL ARM ONLY. IS_NULL_fn's second arm (DT_S && slen==0 && empty) is the RTX-3
 * LENGTH-UNKNOWN trap: slen==0 on a DT_S means "length not computed yet", not "empty".
 * .Lsc_nb rejects that shape outright instead of dereferencing a.s here.
 *
 * REGISTERS (RTX-1, verified empirically):  a = rdi:(slen<<32)|v , rsi:value
 *                                           b = rdx:(slen<<32)|v , rcx:value
 *                                      return = rax:(slen<<32)|v , rdx:value
 * Falls through to .Lsc_slow with rdi/rsi/rdx/rcx untouched (rax/r8 are scratch).
 */
.Lsc_null:
    mov     rax, [rip + g_gc_pending@GOTPCREL]  /* exported => preemptible => GOT */
    cmp     dword ptr [rax], 0
    jne     .Lsc_slow                   /* collection pending => C's gc point   */
    test    edi, edi                    /* a.v == DT_SNUL (0)?                  */
    jne     .Lsc_nb
    cmp     edx, DT_P                   /* a is SNUL, so guard b alone          */
    je      .Lsc_slow
    cmp     edx, DT_X
    je      .Lsc_slow
    cmp     edx, DT_FAIL
    je      .Lsc_slow
    mov     rax, rdx                    /* return b -- read rdx BEFORE overwrite*/
    mov     rdx, rcx
    ret
.Lsc_nb:
    test    edx, edx                    /* b.v == DT_SNUL (0)?                  */
    jne     .Lsc_slow
    mov     r8, rdi
    shr     r8, 32                      /* r8 = a.slen; ZF set when slen == 0   */
    jne     .Lsc_nb_ok
    cmp     edi, DT_S                   /* DT_S with slen==0 => IS_NULL_fn(a)   */
    je      .Lsc_slow                   /*   ambiguous here; let C decide       */
.Lsc_nb_ok:
    cmp     edi, DT_P                   /* b is SNUL, so guard a alone          */
    je      .Lsc_slow
    cmp     edi, DT_X
    je      .Lsc_slow
    cmp     edi, DT_FAIL
    je      .Lsc_slow
    mov     rax, rdi                    /* return a                             */
    mov     rdx, rsi
    ret
.Lsc_slow:
    jmp     c_str_concat_d
RTX_ENDF(str_concat_d)

/*-----------------------------------------------------------------------------
 * char *VARVAL_fn(DESCR_t v)          C of record: src/runtime/core/core.c:1837
 *
 *   Arg pair: edi = v.v (tag), rsi = value (.s when the tag is DT_S).
 *   Result: char * in rax.
 *
 * THE C IS A FOURTEEN-ARM SWITCH AND EXACTLY ONE ARM ALLOCATES NOTHING:
 *     case DT_S: return v.s ? v.s : rt_ws_strdup_c("");
 * with a non-NULL .s, which returns the caller's own pointer unchanged. Every
 * other arm mints a fresh string through rt_ws_strdup_c -- SNUL/P/E/C and the
 * default return constants, I and R FORMAT (SPITBOL Ch.3 conversion rules: an
 * integer loses leading zeros and keeps its sign, a real takes the standard
 * representation with a mandatory decimal point), A and T snprintf a prototype,
 * DATA reads the type name -- and DT_N and DT_K additionally RECURSE, through
 * rt_deref and NV_GET_fn respectively. Allocation and recursion both stay in C:
 * never replicate an allocation in asm (the rule rtx_match.S:471 already states).
 *
 * SO THE PORTED ARM IS A THREE-TEST STRAIGHT LINE WITH NO CALL AND NO ALLOCATION,
 * WHICH IS THE ARCH 0(f-pre) SHAPE WHOSE FALSIFIABILITY IS KNOWABLE IN ADVANCE:
 * entries are commits by construction, because the arm has no bail edge once its
 * two guards pass.
 *
 * 0(f) MEASURED BEFORE A LINE OF THIS WAS WRITTEN, AND IT FALSIFIED THE READING
 * THAT MOTIVATED THE RUNG. Reading the C suggested the surviving entries would be
 * biased toward the ALLOCATING arms, because prior rungs (rtx_match.S:822,
 * rtx_icnrel.S:92, rtx_icnagg.S:27) had already inlined this exact DT_S arm into
 * their own hot paths and would therefore no longer reach it. An LD_PRELOAD tag
 * census says otherwise: DT_S with a non-NULL .s is 100.00% of entries on
 * string_manip (20,000,006), pattern_bt_deep and table_access -- and the count
 * scales EXACTLY 2.00x with the loop bound (20,000,006 -> 10,000,006 at half).
 * The delegating arms are real but cold on this corpus; do not let that read as
 * permission to port them, and do not quote the census as coverage for them.
 */
RTX_FUNC(VARVAL_fn)
    RTX_GATE(str, c_VARVAL_fn)
    cmp     edi, DT_S
    jne     c_VARVAL_fn                 /* every other tag allocates -> C      */
    test    rsi, rsi
    jz      c_VARVAL_fn                 /* DT_S with NULL .s strdups "" -> C   */
    mov     rax, rsi                    /* return v.s unchanged, no allocation */
    ret
RTX_ENDF(VARVAL_fn)

/* Non-executable stack marker: without it ld marks the whole .so RWX-stack. */
.section .note.GNU-stack,"",@progbits
