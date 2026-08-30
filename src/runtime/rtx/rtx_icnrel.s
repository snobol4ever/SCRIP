/* rtx_icnrel.s -- RTX-6b-ICN: rt_jct_relop, the Icon relational/junction dispatcher.
 *
 * CONTRACT: .github/ARCH-ICON-RTX.md. Ladder: .github/GOAL-ICON-RTX.md. Ledger: .github/RTX-CLAIMS.md.
 * Symbol checked out OUT:ICON-RTX:s212. Gate: SCRIP_RTX_ICNREL (ninth family gate -- a ledger event).
 * rt_jct_relop is an Icon-EXCLUSIVE row: zero SNOBOL4 and zero Prolog static call sites, so no
 * ownership arbitration is owed. rt_binop_overload, which the s211 rung text paired with it, is NOT
 * ported here: the ledger allocates it to SN4-RTX at 1.4x (197 SNO vs 141 ICON, past the 1.3x tie bar).
 *
 * WHICH ARMS ARE LIVE, AND HOW THAT WAS DECIDED (steps 0(d) + 0(g), measured s212):
 * An LD_PRELOAD interposer binned every call by operand tag pair and op across the whole 303-program
 * Icon corpus. 34 programs reach the symbol at all; 4308 calls total. Distribution:
 *   pairs: S/S 2892 (67.1%) . I/I 428 . R/I 358 . S/I 219 . R/R 192 . I/S 118 . rest < 30 each
 *   ops:   SEQ 2012 (46.7%) . EQV 805 . SNE 325 . GE 292 . NEQV 265 . GT 109 . EQ 100 . rest < 100
 * Mapping ops onto the C body's arm chain (by_name_dispatch.c rt_jct_relop_impl):
 *   - SEQ/SNE/SGT/SGE/SLT/SLE = 2577 calls (59.8%) fall past EVERY numeric arm to the TEXTUALLY LAST
 *     arm, the VARVAL_fn + strcmp tail.
 *   - EQV/NEQV = 1070 calls (24.8%) are caught by the TEXTUALLY FIRST block.
 *   - the numeric arms in between are ~15% combined.
 * So BOTH ends are live and the middle is cold. This is a THIRD regime, distinct from both prior rungs:
 * RTX-6-ICN's caller guard made the textually-first arms dead, and s211's recon predicted this rung
 * would be "the opposite regime, first arms live". Measurement says the dominant arm is the LAST one.
 *
 * ⛔ s211's RECON WAS FALSIFIED BEFORE IT WAS USED, AND IT IS RECORDED IN THE FINDING:
 * the s211 handoff states bb_binop_relop.cpp has NO inline tag guard and calls rt_jct_relop at FIVE
 * sites. Measured: it HAS DT_DATA and DT_I guards (lines 23-32, 107-116) and calls the symbol at FOUR
 * sites. The DT_I guard is why I/I is only 9.9% of arrivals despite integer comparison being Icon's
 * commonest relop -- int-int is inlined by the template and never reaches this symbol at all.
 *
 * WHAT THE PORT ACTUALLY REMOVES -- NOT -O0 FRAME CEREMONY (inbox gap #1, answered concretely):
 * the EXPORTED wrapper runs a setjmp on EVERY call (by_name_dispatch.c:4776-4785). On the S/S tail the
 * C additionally makes two junction_is calls, two VARVAL_fn calls, one libc strcmp, and walks ~24
 * compares of arm dispatch. The fast path below deletes the setjmp, all five calls and the whole
 * dispatch walk, replacing them with an inline byte compare. A deleted setjmp + deleted libc call is
 * removable work at any -O level.
 *
 * SOUNDNESS -- THE ONE PROPERTY THE WHOLE FILE RESTS ON:
 * the asm decides "this is definitely a simple case" or "let C do it". It NEVER decides a hard case.
 * Every bail is a bare `jmp c_rt_jct_relop` with rdi/rsi/rdx/rcx/r8 UNTOUCHED, so the C body reruns
 * from the original arguments, setjmp included. Nothing is written anywhere before the final `ret`,
 * so a bail can never leave partial state. Deliberate bails:
 *   - either operand tagged anything but DT_S on the string arm  -> bail
 *   - either .s pointer NULL                                     -> bail (C's VARVAL_fn substitutes "")
 *   - either string is a JUNCTION (first byte 0x03)              -> bail (junction_collapse stays in C)
 *   - either operand is a CSET (slen == 0xFFFFFFFF) on EQV       -> bail (cset_resolve stays in C)
 *   - any op outside the two measured-live families              -> bail
 * PORT != FIX: the string arm reproduces strcmp's NUL-terminated, slen-IGNORING semantics exactly,
 * because that is what the C tail does. Making it slen-aware would be a behaviour change wearing a
 * port's clothes, which is the defect class GOAL-ICON-RTX.md names on RTX-9.
 *
 * Registers: rdi = lhs (slen<<32)|v, rsi = lhs value, rdx = rhs (slen<<32)|v, rcx = rhs value,
 * r8d = op. Returns int in eax. All five argument registers are preserved on every bail path.
 * Working set per ARCH section 2: rax r9 r10 r11. Blob pins rbx ___ r12-r15 are never touched.
 */
#include "rtx_abi.inc"

RTX_GATE_DEF(icnrel)

/* BinopKind ordinals, src/runtime/builtins/gen.h. Kept as literals with the names beside them so a
 * renumbering of that enum is a visible diff here rather than a silent miscompare. */
#define OP_LE    6
#define OP_GE    8
#define OP_EQ    9
#define OP_NE   10
#define OP_SLT  12
#define OP_SLE  13
#define OP_SGT  14
#define OP_SGE  15
#define OP_SEQ  16
#define OP_SNE  17
#define OP_EQV  22
#define OP_NEQV 23

/* ⛔ TAG VALUES MUST MATCH src/contracts/descr.h (s229 renumber: DT_S descr.h:48 = 0x02, DT_I descr.h:49 = 0x03).
 * A _Static_assert cannot reach an asm #define (the rtx_icncall.s DT_E=11 lesson, ICN-ZFRAME s15) -- re-verify
 * against descr.h on every edit of this file. Stale tags here are SILENT: every fast arm simply never matches
 * (perf-only) EXCEPT rt_str_coerce, whose identity arm then returns csets UNCONVERTED (wrong answers). */
/* ⛔ NO TAG LITERALS HERE: DT_S/DT_I arrive from descr_tags.inc via rtx_abi.inc. Local defines agreed with descr.h at s237 but are the same shadowing class that made dat_field_get compare 100 against a live 0x70; removed so the gate can be exception-free. */

RTX_FUNC(rt_jct_relop)
    RTX_GATE(icnrel, c_rt_jct_relop)
/* Split on op family first: it is the cheapest discriminator and it is already in a register. */
    cmp     r8d, OP_EQV
    je      .Leqv
    cmp     r8d, OP_NEQV
    je      .Leqv
    cmp     r8d, OP_SLT
    jb      .Lbail
    cmp     r8d, OP_SNE
    ja      .Lbail
/* ---- STRING ARM: ops SLT SLE SGT SGE SEQ SNE, 59.8% of measured arrivals ----------------------
 * C equivalent, verified against the impl's final block:
 *     a = VARVAL_fn(lhs); b = VARVAL_fn(rhs); c = strcmp(a,b); switch (op) {...}
 * VARVAL_fn on DT_S with a non-NULL .s returns .s unchanged and allocates nothing (core.c:1840),
 * so for DT_S/DT_S this whole tail is exactly strcmp on the two stored pointers. */
    cmp     dil, DT_S
    jne     .Lbail
    cmp     dl, DT_S
    jne     .Lbail
/* CSET EXCLUSION (added with the s229 tag fix that ARMED this previously-dead arm): a cset is spelled
 * DT_S with slen 0xFFFFFFFF, and comparing its raw .s bypasses the sorted-character conversion order
 * that c_rt_str_coerce establishes. Same guard idiom as the EQV arm below. */
    mov     rax, rdi
    shr     rax, 32
    cmp     eax, -1
    je      .Lbail
    mov     rax, rdx
    shr     rax, 32
    cmp     eax, -1
    je      .Lbail
    test    rsi, rsi
    jz      .Lbail
    test    rcx, rcx
    jz      .Lbail
/* junction_is(v) = (IS_STR_fn(v) || v==DT_SNUL) && v.s && v.s[0]=='\x03'. Both are DT_S here, so the
 * junction test reduces to the first byte. A junction must reach junction_collapse in C. */
    cmp     byte ptr [rsi], 3
    je      .Lbail
    cmp     byte ptr [rcx], 3
    je      .Lbail
/* Inline strcmp: compare until a difference or a shared terminator, then set eax to the sign in the
 * same three-way sense strcmp reports. r9/r10 walk, r11d/eax hold the two bytes. */
    mov     r9, rsi
    mov     r10, rcx
.Lstrloop:
    movzx   r11d, byte ptr [r9]
    movzx   eax, byte ptr [r10]
    cmp     r11d, eax
    jne     .Lstrdiff
    test    r11d, r11d
    jz      .Lstreq
    inc     r9
    inc     r10
    jmp     .Lstrloop
.Lstreq:
/* Equal strings: c == 0. SEQ SLE SGE succeed, SNE SLT SGT fail. */
    xor     eax, eax
    cmp     r8d, OP_SEQ
    je      .Lret1
    cmp     r8d, OP_SLE
    je      .Lret1
    cmp     r8d, OP_SGE
    je      .Lret1
    ret
.Lstrdiff:
/* c != 0. strcmp's sign comes from the UNSIGNED byte comparison, which is what movzx + cmp gives.
 * r11d holds lhs byte, eax holds rhs byte; below == lhs < rhs. */
    cmp     r11d, eax
    jb      .Lless
/* lhs > rhs: SGT SGE SNE succeed. */
    xor     eax, eax
    cmp     r8d, OP_SGT
    je      .Lret1
    cmp     r8d, OP_SGE
    je      .Lret1
    cmp     r8d, OP_SNE
    je      .Lret1
    ret
.Lless:
/* lhs < rhs: SLT SLE SNE succeed. */
    xor     eax, eax
    cmp     r8d, OP_SLT
    je      .Lret1
    cmp     r8d, OP_SLE
    je      .Lret1
    cmp     r8d, OP_SNE
    je      .Lret1
    ret
/* ---- EQV / NEQV ARM: 24.8% of measured arrivals ----------------------------------------------
 * C equivalent, the impl's first block. Its opening test is
 *     if (lhs.v == rhs.v && lhs.i == rhs.i) eq = 1;
 * comparing the tag and the full 8-byte value union. That single test settles every case where the
 * two descriptors are bit-identical in both halves, which covers identical ints, identical reals,
 * and strings sharing a pointer. Anything else in that block needs cset_resolve, strlen or memcmp,
 * so it bails. DT_I/DT_I is additionally settled: with equal tags and unequal values the chain falls
 * to `else if (lhs.v==rhs.v && lhs.v==DT_I) eq = (lhs.i==rhs.i)`, i.e. eq = 0.
 * A CSET is DT_S with slen 0xFFFFFFFF and takes the cset_resolve branch, so csets bail. */
.Leqv:
    cmp     edi, edx
    jne     .Lbail
    cmp     rsi, rcx
    je      .Leqv_equal
/* Tags match, values differ. Settled ONLY for DT_I; every other tag may still compare equal by
 * content further down the C chain (string bytes, cset membership), so bail. */
    cmp     dil, DT_I
    jne     .Lbail
    xor     eax, eax
    cmp     r8d, OP_NEQV
    je      .Lret1
    ret
.Leqv_equal:
/* Bit-identical descriptors. Guard the cset spelling out of the fast path: for DT_S with
 * slen 0xFFFFFFFF the C reaches cset_resolve, and although an identical pointer would also compare
 * equal there, this port does not model cset_resolve's failure mode, so it refuses the case. */
    cmp     dil, DT_S
    jne     .Leqv_settled
    mov     rax, rdi
    shr     rax, 32
    cmp     eax, -1
    je      .Lbail
.Leqv_settled:
    xor     eax, eax
    cmp     r8d, OP_EQV
    je      .Lret1
    ret
.Lret1:
    mov     eax, 1
    ret
.Lbail:
/* Every argument register is still exactly as the caller left it. */
    jmp     c_rt_jct_relop
RTX_ENDF(rt_jct_relop)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* rt_str_coerce(d) -- ADDED s214-ICN. Icon-EXCLUSIVE, ledger row FREE, family COERCE, which is the
 * SAME family the icnrel gate already covers (rt_jct_relop is COERCE too), so this reuses the ninth
 * gate rather than minting a thirteenth. No new gate => NOT a ledger gate event.
 *
 * The C body (by_name_dispatch.c:4767) is:
 *     if (!IS_CSET_fn(d)) return d;            <= THE ARM PORTED HERE: pure identity, zero memory ops
 *     cset_resolve + rt_ws_alloc + memcpy + insertion sort + STRVAL   <= STAYS IN C
 * Only the guard arm is taken in asm. The cset arm allocates and sorts, and getting it wrong is a
 * silently mis-ordered cset, so it tail-jumps to C untouched.
 *
 * PREDICATE READ FROM SOURCE, NOT FROM PROSE (core.h:32):
 *     IS_CSET_fn(v) == (v.v == DT_S && v.slen == 0xFFFFFFFF)
 * s213 introduced AND fixed a cset bug on this exact predicate in rt_size_d, so it is checked against
 * core.h every time rather than recalled. A cset that is NOT recognised falls into the identity arm and
 * is returned unconverted -- silently wrong, never a crash.
 *
 * STEP 0 MEASURED s214-ICN (both of s209's independent questions, which a rung must pass BOTH of):
 *   (f) 146 `call rt_str_coerce@PLT` sites in a COMPILER sweep of 316 programs -- the stale ledger row
 *       says 38, i.e. UNDERstated 3.8x, the same direction s213 measured for rt_relop_overload.
 *   (g) WHICH ARM: interposer over all 303 corpus programs => 861 arrivals, identity arm 802 = 93.1%,
 *       cset arm 59 = 6.9%. The ported arm is DOMINANT. bb_unop/relop emit this call with NO inline tag
 *       guard, so unlike RTX-6 the guard does not steer arrivals away from the cheap arm -- a THIRD
 *       distinct regime, confirming s212's law that the regime never transfers between rungs.
 *   (d) SCALES: the triggering construct is LEXICAL COMPARISON (<<, <<=, ==, ~==, >>=, >>) on operands
 *       that may be csets -- found by ranking corpus programs by arrivals (rung36_jcon_lexcmp &c.), NOT
 *       guessed. A cset-CONCAT loop reaches the symbol ZERO times even though the program runs
 *       correctly, which is exactly why the construct was measured instead of assumed. On the real
 *       construct: 100 -> 400 arrivals at N -> 4N, EXACTLY 4x, both arms scaling.
 * ⛔ NO SPEED CLAIM. Not benchmarked; no isolation arm was run, so per s204 no ratio may be quoted for
 * this symbol or for the icnrel family after this edit. */
RTX_FUNC(rt_str_coerce)
    RTX_GATE(icnrel, c_rt_str_coerce)
    cmp     dil, DT_S
    jne     .Lsc_ident                  /* not DT_S => cannot be a cset => identity */
    mov     rax, rdi
    shr     rax, 32                     /* rax = slen */
    cmp     eax, -1                     /* slen == 0xFFFFFFFF => IS_CSET => C owns it */
    je      c_rt_str_coerce
.Lsc_ident:
    mov     rax, rdi
    mov     rdx, rsi
    ret
RTX_ENDF(rt_str_coerce)
.section .note.GNU-stack,"",@progbits
