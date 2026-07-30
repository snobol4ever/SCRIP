/* rtx_plcall.S — RTX family PLCALL.  RTX-1-PL: the Prolog deterministic predicate-call open.
 *
 * READ .github/ARCH-PROLOG-RTX.md BEFORE EDITING, AND ITS PARENT .github/ARCH-SNOBOL4-RTX.md §7
 * (the shared step-0 checklist; the Prolog contract deliberately does not restate it).
 * Contract macros: rtx_abi.inc.
 *
 * WHAT THIS PORTS
 *   rt_proc_call_open_det — the open leaf every Prolog predicate call crosses.  In ISO terms this is
 *   the entry side of a call to a user-defined predicate named by its PREDICATE INDICATOR (Name/Arity;
 *   lower_prolog.c builds exactly that string via pl_pi_name).  GNU Prolog's PredInf keys <functor,arity>
 *   -> codep and SWI's lookupProcedure(functor_t, Module) -> Definition are the same dispatch; here the
 *   emitter has already resolved the indicator to a DENSE REGISTRY INDEX at emission time, so this leaf
 *   takes an integer index, not a name — no hash, no strcmp.  The choice-point discipline the WAM keeps
 *   in its B register appears here as the vtmark stamped into the pcall record (the epilogue landings
 *   tidy the value trail back to it), which is why the trail read below is NOT optional bookkeeping.
 *
 * WHY IT IS THE RIGHT FIRST PROLOG RUNG (measured s221, re-derived s222 from the tree)
 *   rank 2 by dynamic arrivals (2,060,043 corpus-wide) but rank 1 BY REACH — 19 of 22 van Roy programs,
 *   the broadest of any Prolog symbol; NO PL-SINK rung claims it, so it is the one hot Prolog symbol
 *   that does not collide with the concurrently-running SINK ladder; not a `dop`, so it sits structurally
 *   outside SINK's $op territory.  Its sibling rt_proc_call_open measures ZERO arrivals in 22/22 despite
 *   5,626 static sites — the _det variant takes the whole path.
 *
 * ⭐ THE EMITTING-TEMPLATE CHECK (the shared step 0's "read the template first") CHANGED THIS RUNG'S
 *   STORY AND MUST NOT BE RE-DERIVED FROM PROSE.  bb_call_proc_staged.cpp has TWO arms that call this
 *   symbol, not one:
 *     (1) the DET arm, whose chained ternary offers PL-DC -> fused rt_proc_call_open_detN -> generic;
 *     (2) bcps_spine_gen_arm, THE GENERATOR ARM, whose own comment states that "every nondet Prolog
 *         predicate (app/3, nrev/2, every multi-clause pred) is dispatched HERE".
 *   The generator arm has NO fused family and NO det_fuse test at all: its only choice is
 *   gi_idx >= 0 ? rt_proc_call_open_det : rt_proc_call_open.  So the hot Prolog path reaches THIS symbol
 *   directly and unconditionally-by-index, and it cannot be steered away from it by any fused-family
 *   eligibility fix in the det arm.  There is no guard here whose rejected arm needs porting instead.
 *
 * WHAT THE -O0 C SPENDS, AND WHERE THIS PORT GETS ITS WIN
 *   The C reaches four levels: open_det -> rt_proc_call_prologue_lex -> { rt_pcall_grow,
 *   rt_value_trail_mark }.  Three of those four are absorbed here, so the SUCCESS PATH CONTAINS NO CALL:
 *     - rt_proc_call_prologue_lex computes `fbytes` (a max, then a 16-align) and RETURNS it — and every
 *       caller on this path throws it away behind a (void) cast.  The computation is side-effect-free,
 *       so it is ELIDED ENTIRELY.  That is dead work the C pays 2,060,043 times.
 *     - rt_pcall_grow is `if (g_pcall_top < g_pcall_cap) return;` — a capacity guard whose common case
 *       returns immediately.  Inlined as a compare with an out-of-line cold arm.
 *     - rt_value_trail_mark is `{ return g_pl_trail.top; }` — one global load, inlined.
 *   Nothing semantic changes.  The record written is field-for-field what the C writes.
 *
 * LINKAGE SPLIT — CHECKED WITH readelf ON rt.o, NEVER ON THE .so (shared §7 0(c), the s209 lesson:
 * the link demotes hidden globals to lowercase in the .so, so the .so is structurally incapable of
 * separating `static` from visibility("hidden")).  MEASURED AT HEAD:
 *   HIDDEN  => rip-direct : g_rt_gen_procs g_rt_gen_proc_count g_pcall g_pcall_top g_pcall_cap
 *                           g_pcall_wires rt_k_level
 *   DEFAULT => @GOTPCREL  : rt_g_want_name  Σ  Σlen  g_pl_trail
 * Getting that backwards is a link error on the hidden side and a SILENT DIFFERENT VARIABLE on the
 * exported side under -no-pie copy relocation, which is why every exported one goes through the GOT.
 * rt_pcall_grow was `static` (nm: lowercase t) hence unreferenceable from .S; it is de-static'd to
 * visibility("hidden") in the same commit — the s187 rt_nret_fix precedent, no contract amendment owed.
 *
 * OFFSETS ARE LOCKED BY _Static_assert IN THE OWNING TU, NOT TRUSTED FROM THIS COMMENT
 *   rt_proc_t   : name 0 . fn 8 . frame_bytes 48 . is_generator 0x4c . dyn_scope 80 . sizeof 128
 *   rt_pcall_t  : p 0 . rname 8 . save_Σ 16 . save_Σlen 24 . save_base 28 . wn 32 . lex 36 .
 *                 nargs 40 . fb 48 . vtmark 56 . sizeof 64
 *   pl_trail_t  : area 0 (pl_area_t = 4 words) . top 32
 *   CALL_ARGS_MAX 64
 *   sizeof(rt_proc_t) == 128 is a power of two, so the index scales by `shl 7`; rt_pcall_t by `shl 6`;
 *   rt_flat_wires_t is 40, NOT a power of two, so index*40 == ((index*5)<<3) as lea+shl.
 *
 * REGISTER SAFETY
 *   Uses rax rcx rdx rsi rdi r8 r9 r10 r11 xmm0 — all inside the ARCH §2 RTX working set.  No blob pin
 *   (rbx r13 r14 r15) is read or written; rbp is touched only inside the cold grow arm, under the
 *   standard prologue, and r12 is not used at all.
 */
#include "rtx_abi.inc"

RTX_GATE_DEF(plcall)

#define PROC_NAME          0
#define PROC_FN            8
#define PROC_DYN_SCOPE     80
#define PROC_SHIFT         7          /* sizeof(rt_proc_t) == 128 */

#define PC_P               0
#define PC_RNAME           8
#define PC_SAVE_S          16
#define PC_SAVE_SLEN       24
#define PC_SAVE_BASE       28
#define PC_WN              32
#define PC_LEX             36
#define PC_NARGS           40
#define PC_FB              48
#define PC_VTMARK          56

#define PL_TRAIL_TOP       32
#define CALL_ARGS_MAX      64

/*-----------------------------------------------------------------------------------------------------
 * void *rt_proc_call_open_det(long idx, int nargs)          C of record: rt/rt.c:1425
 *
 *   if (idx < 0 || idx >= g_rt_gen_proc_count) return 0;
 *   rt_proc_t *p = &g_rt_gen_procs[idx];
 *   if (!p->fn || p->dyn_scope) return 0;
 *   int wn = rt_g_want_name; rt_g_want_name = 0;
 *   (void)rt_proc_call_prologue_lex(p, nargs, wn);
 *   return p->fn;
 *
 *   rdi = idx, esi = nargs.  Returns rax.  Strict leaf on the success path.
 *   EVERY GUARD PRECEDES EVERY SIDE EFFECT, exactly as the C does, so a decline is side-effect-free —
 *   that is the SCC arm's discipline and the emitted site's FAIL arm depends on it (`test rax,rax; je`).
 */
RTX_FUNC(rt_proc_call_open_det)
    RTX_GATE(plcall, c_rt_proc_call_open_det)
    test    rdi, rdi
    js      .Lpc_zero                               /* idx < 0                                       */
    movsxd  rax, dword ptr [rip + g_rt_gen_proc_count]
    cmp     rdi, rax
    jge     .Lpc_zero                               /* idx >= count                                  */
    mov     r8, qword ptr [rip + g_rt_gen_procs]
    mov     rax, rdi
    shl     rax, PROC_SHIFT                         /* * sizeof(rt_proc_t) == 128                    */
    add     r8, rax                                 /* r8 = p                                        */
    mov     r9, qword ptr [r8 + PROC_FN]            /* r9 = p->fn, also the return value             */
    test    r9, r9
    je      .Lpc_zero                               /* no body                                       */
    cmp     dword ptr [r8 + PROC_DYN_SCOPE], 0
    jne     .Lpc_zero                               /* dyn-scope callee is not this leaf's business   */
    mov     rcx, qword ptr [rip + rt_g_want_name@GOTPCREL]
    mov     r10d, dword ptr [rcx]                   /* wn = rt_g_want_name                           */
    mov     dword ptr [rcx], 0                      /* rt_g_want_name = 0                            */
/* --- absorbed rt_proc_call_prologue_lex(p, nargs, wn); its fbytes result is discarded by this
 * caller and its computation is side-effect-free, so the max + 16-align are elided ----------------- */
    cmp     esi, CALL_ARGS_MAX                      /* signed, as the C is                           */
    jle     .Lpc_nargs_ok
    mov     esi, CALL_ARGS_MAX
.Lpc_nargs_ok:
    mov     eax, dword ptr [rip + g_pcall_top]      /* the ONE load of top                           */
    cmp     eax, dword ptr [rip + g_pcall_cap]
    jge     .Lpc_grow                               /* absorbed rt_pcall_grow: cold arm only          */
.Lpc_fill:
    mov     rcx, qword ptr [rip + g_pcall]          /* loaded AFTER any grow — realloc may move it   */
    movsxd  rdx, eax
    shl     rdx, 6                                  /* * sizeof(rt_pcall_t) == 64                    */
    add     rcx, rdx                                /* rcx = c = &g_pcall[top]                       */
    mov     qword ptr [rcx + PC_P], r8
    mov     rdx, qword ptr [r8 + PROC_NAME]
    mov     qword ptr [rcx + PC_RNAME], rdx         /* c->rname = p->name (lex protocol, not result_name) */
    mov     rdx, qword ptr [rip + Σ@GOTPCREL]       /* exported => GOT, never lea                     */
    mov     rdx, qword ptr [rdx]
    mov     qword ptr [rcx + PC_SAVE_S], rdx
    mov     rdx, qword ptr [rip + Σlen@GOTPCREL]
    mov     edx, dword ptr [rdx]                    /* Σlen is int: 4-byte load and store             */
    mov     dword ptr [rcx + PC_SAVE_SLEN], edx
    mov     dword ptr [rcx + PC_SAVE_BASE], 0
    mov     dword ptr [rcx + PC_WN], r10d
    mov     dword ptr [rcx + PC_LEX], 1
    mov     dword ptr [rcx + PC_NARGS], esi
    mov     qword ptr [rcx + PC_FB], 0
    mov     rdx, qword ptr [rip + g_pl_trail@GOTPCREL]
    mov     edx, dword ptr [rdx + PL_TRAIL_TOP]     /* absorbed rt_value_trail_mark()                 */
    mov     dword ptr [rcx + PC_VTMARK], edx
    mov     rdx, qword ptr [rip + g_pcall_wires]
    test    rdx, rdx
    jz      .Lpc_bump
    movsxd  r11, eax
    lea     r11, [r11 + r11*4]                      /* index * 5 ...                                  */
    shl     r11, 3                                  /* ... * 8 == * 40 == sizeof(rt_flat_wires_t)     */
    add     rdx, r11
    pxor    xmm0, xmm0                              /* the C compound literal has 4 initializers for  */
    movdqu  xmmword ptr [rdx +  0], xmm0            /* 5 members, so C zeroes the tail too: all 40    */
    movdqu  xmmword ptr [rdx + 16], xmm0            /* bytes are cleared, r12 field included          */
    mov     qword ptr [rdx + 32], 0
.Lpc_bump:
    inc     dword ptr [rip + g_pcall_top]
    inc     dword ptr [rip + rt_k_level]
    mov     rax, r9                                 /* return p->fn                                   */
    ret
.Lpc_zero:
    xor     eax, eax
    ret
/* COLD: capacity exhausted.  rt_pcall_grow may realloc BOTH arrays or fail (realloc null), in which
 * case the C skips the record fill but STILL bumps top and rt_k_level — reproduced exactly.  This is
 * the only path that builds a frame; the volatiles that must survive the call are parked. */
.Lpc_grow:
    push    rbp                                     /* entry rsp%16==8; push => 0, sub 32 => 0        */
    mov     rbp, rsp
    sub     rsp, 32
    mov     qword ptr [rbp-8],  r8
    mov     qword ptr [rbp-16], r9
    mov     dword ptr [rbp-20], esi
    mov     dword ptr [rbp-24], r10d
    call    rt_pcall_grow                           /* de-static'd + hidden this commit               */
    mov     r8,   qword ptr [rbp-8]
    mov     r9,   qword ptr [rbp-16]
    mov     esi,  dword ptr [rbp-20]
    mov     r10d, dword ptr [rbp-24]
    leave
    mov     eax, dword ptr [rip + g_pcall_top]
    cmp     eax, dword ptr [rip + g_pcall_cap]
    jl      .Lpc_fill
    jmp     .Lpc_bump                               /* grow failed: no fill, but the bumps still run  */
RTX_ENDF(rt_proc_call_open_det)

/* Non-executable stack marker: without it ld marks the whole .so RWX-stack. */
.section .note.GNU-stack,"",@progbits
