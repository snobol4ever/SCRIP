/* rtx_plunify.s — RTX family PLUNIFY.  RTX-2-PL: the Prolog general unification leaf.
 *
 * READ .github/ARCH-PROLOG-RTX.md BEFORE EDITING, AND ITS PARENT .github/ARCH-SNOBOL4-RTX.md §7
 * (the shared step-0 checklist; the Prolog contract deliberately does not restate it).
 * Contract macros: rtx_abi.inc.
 *
 * WHAT THIS PORTS
 *   rt_pl_dop_unify — the runtime sink for the lowered $unify(A,B) node.  In ISO terms this is
 *   =/2 on two already-marshalled arguments.  GNU Prolog's Pl_Unify (EnginePl/unify.c) and SWI's
 *   PL_unify()/do_unify() are the same operation; the recursive structural walk itself lives in
 *   plw_unify_cells and is NOT reimplemented here — see SCOPE below, which is the whole point of
 *   this rung.
 *
 * ⭐ WHY THIS SYMBOL, MEASURED s226 AND NOT INHERITED
 *   Board = corpus/benchmarks/prolog/bench/, 22 files, all rc=0, md5s recorded in the FINDING.
 *   rt_pl_dop_unify = 1,108,786 arrivals at REACH 22/22 — the BROADEST reach of any Prolog symbol,
 *   broader than RTX-1-PL's rt_proc_call_open_det (19/22).  On the heavier off-board program
 *   rung10_programs_puzzle_19.pl it takes 1,775,371 arrivals, reproducing s224's figure to the digit.
 *   Ledger state FREE, Prolog-exclusive (no arbitration with SN4-RTX or ICON-RTX).
 *
 * ⛔⛔ SCOPE — THIS IS A WRAPPER PORT AND IT SAYS SO IN ITS OWN SOURCE
 *   PL-SINK-1 (landed s142) already inlines the FAST unify arm in the EMITTER, so the arrivals that
 *   reach this leaf are the residue SINK refused.  Re-implementing a fast arm in asm here would
 *   therefore be VACUOUS BY CONSTRUCTION — the exact class RTX-8 SLICE3 fell into (passed every
 *   step-0 check, was vacuous).  What is portable WITHOUT that risk is the CEREMONY the C pays on
 *   every arrival regardless of shape:
 *     - the -O0 prologue/epilogue and its stack shuffling (~20 instructions of pure -O0 tax);
 *     - the nargs guard;
 *     - the unwind-floor save/set/restore (2 GOT loads + 2 stores);
 *     - rt_gc_point_arr, which is a REAL @plt CALL whose body is `if (!g_gc_pending) return;`
 *       — absorbed here as an inline flag test with a cold out-of-line arm.
 *   plw_unify_vals and rt_pl_deref_val REMAIN CALLS.  One @plt crossing is deleted per arrival, not
 *   the unifier.  ⛔ DO NOT let a later rung read this file as "unify is ported" — it is not.
 *
 * ⚠ THE FLOOR IS SEMANTIC, NOT BOOKKEEPING, AND ITS VALUE IS EXACT
 *   g_plw_unwind_floor feeds pl_dead_cstack (parsers/prolog/pl_cell.h, renamed off its old name --
 *   row prolog-call-n-compiles-through-eval-and-the-plc-runtime-solver-is-deleted): pl_trail_unwind SKIPS the
 *   restore for any trail entry whose target address is < floor+16, i.e. lives in a dead C frame.
 *   Writing a DIFFERENT floor than the C would silently change which bindings survive backtracking.
 *   The C stores __builtin_frame_address(0), which at -O0 is ___ AFTER `push ___; mov ___,rsp`,
 *   i.e. EXACTLY rsp_at_entry - 8.  This port builds the same frame, so ___ holds the identical
 *   value bit for bit.  VERIFIED against the disassembly of the C body (`mov %___,%rdx; mov %rdx,(%rax)`).
 *
 * ⭐ THE nargs GUARD RETURNS WITHOUT TOUCHING THE FLOOR OR THE SAFEPOINT
 *   Read from the C's disassembly, not from its source shape: the nargs!=2 arm jumps straight to the
 *   epilogue, so it never saves, sets, or restores the floor and never reaches the GC point.  A refuse
 *   is side-effect-free.  Reproduced exactly — the guard precedes the frame work.
 *
 * LINKAGE SPLIT — CHECKED WITH nm ON THE OBJECT FILES, NEVER ON THE .so (shared §7, s209 lesson)
 *   out/rt_pic/by_name_dispatch.o : g_plw_unwind_floor `B` (exported) => @GOTPCREL
 *                                   rt_pl_deref_val    `T` (exported) => plain call
 *                                   plw_unify_vals     `t` (STATIC)   => de-static'd to
 *                                                                        visibility("hidden") in the
 *                                                                        same commit (s187 rt_nret_fix
 *                                                                        precedent; no contract
 *                                                                        amendment owed).  s214 axis 2
 *                                                                        checked FIRST: zero template
 *                                                                        references, so it is never
 *                                                                        named in emitted mode-4 TEXT
 *                                                                        and hidden is safe.
 *   out/rt_pic/gc_heap.o          : g_gc_pending       `B` (exported) => @GOTPCREL
 *   Getting that backwards is a link error on the hidden side and a SILENT DIFFERENT VARIABLE on the
 *   exported side under -no-pie copy relocation, which is why every exported one goes through the GOT.
 *
 * LAYOUT BAKED HERE
 *   DESCR_t : v@0 (4) . slen@4 (4) . payload@8 (8) . sizeof 16   — locked by the _Static_assert at
 *   by_name_dispatch.c:105/107, which already names DESCR v@0 / payload@8 / sizeof 16 for PL-SINK.
 *   DESCR_t is returned in rax:rdx and passed in two INTEGER registers (union of pointer/int64/double
 *   classifies INTEGER).  CONFIRMED from the C's own disassembly, not from ABI reasoning.
 *   FAILDESCR = { .v = DT_FAIL(99), .i = 0 } => rax = 99, rdx = 0.
 *
 * REGISTER SAFETY
 *   Uses rax rcx rdx rsi rdi r8 r9 — all inside the ARCH §2 RTX working set.  NO blob pin
 *   (rbx r13 r14 r15) is read or written, and r12 is not used at all.  ___ is the standard frame
 *   pointer and is required here: it IS the floor value.
 */
#include "rtx_abi.inc"

RTX_GATE_DEF(plunify)

#define D_LO               0          /* DESCR_t {v,slen} eightbyte                                */
#define D_HI               8          /* DESCR_t payload eightbyte                                 */
#define D_SIZE            16

#define FR_ARGS           16          /* saved DESCR_t *args (rsp-relative after sub rsp,24)       */
#define FR_FLOOR           8          /* saved previous g_plw_unwind_floor (rsp-relative)          */

/*-----------------------------------------------------------------------------------------------------
 * DESCR_t rt_pl_dop_unify(DESCR_t *args, int nargs)       C of record: by_name_dispatch.c:1508
 *
 *   if (nargs != 2) return FAILDESCR;
 *   char *fl = g_plw_unwind_floor;
 *   g_plw_unwind_floor = (char *)__builtin_frame_address(0);
 *   rt_gc_point_arr(args, 2, 0);
 *   out = plw_unify_vals(args[0], args[1]) ? rt_pl_deref_val(args[0]) : FAILDESCR;
 *   g_plw_unwind_floor = fl;
 *   return out;
 *
 *   rdi = args, esi = nargs.  Returns DESCR_t in rax:rdx.
 */
RTX_FUNC(rt_pl_dop_unify)
    cmp     esi, 2
    jne     .Lpu_fail_bare                          /* refuse BEFORE any side effect              */
    sub     rsp, 24                                 /* RSP-ONLY: entry%16==8, -24 => aligned calls; slots [rsp+FR_ARGS],[rsp+FR_FLOOR]; exit adds 24 */
    mov     qword ptr [rsp + FR_ARGS], rdi
    mov     rax, qword ptr [rip + g_plw_unwind_floor@GOTPCREL]
    mov     rcx, qword ptr [rax]
    mov     qword ptr [rsp + FR_FLOOR], rcx         /* fl = g_plw_unwind_floor                      */
    lea     rcx, [rsp + 16]                         /* == rsp_at_entry-8, the exact old frame_address(0) bit value */
    mov     qword ptr [rax], rcx                    /* g_plw_unwind_floor = that landmark           */
/* --- absorbed rt_gc_point_arr(args, 2, 0): its body is `if (!g_gc_pending) return;`, so the common
 * case is a flag test and the @plt crossing is deleted.  Cold arm calls the real thing unchanged. -- */
    mov     rax, qword ptr [rip + g_gc_pending@GOTPCREL]
    cmp     dword ptr [rax], 0
    jne     .Lpu_gc
.Lpu_gc_done:
    mov     rax, qword ptr [rsp + FR_ARGS]
    mov     rdi, qword ptr [rax + D_LO]             /* args[0] lo                                   */
    mov     rsi, qword ptr [rax + D_HI]             /* args[0] hi                                   */
    mov     rdx, qword ptr [rax + D_SIZE + D_LO]    /* args[1] lo                                   */
    mov     rcx, qword ptr [rax + D_SIZE + D_HI]    /* args[1] hi (loaded last: rcx is the base's    */
    call    plw_unify_vals                          /*   scratch in the C too)                      */
    test    eax, eax
    je      .Lpu_fail
    mov     rax, qword ptr [rsp + FR_ARGS]
    mov     rdi, qword ptr [rax + D_LO]
    mov     rsi, qword ptr [rax + D_HI]
    call    rt_pl_deref_val                         /* exported `T`: plain call, returns rax:rdx     */
    jmp     .Lpu_restore
.Lpu_fail:
    mov     eax, DT_FAIL | (MOD_OP_RT_PL_DOP_UNIFY << 8)   /* FAILDESCR, stamped (row descr-stamp-asm-mints) */
    xor     edx, edx
.Lpu_restore:
    mov     rcx, qword ptr [rip + g_plw_unwind_floor@GOTPCREL]
    mov     r8,  qword ptr [rsp + FR_FLOOR]
    mov     qword ptr [rcx], r8                     /* restore floor WITHOUT touching rax:rdx        */
    add     rsp, 24
    ret
/* COLD: a collection is pending.  Call the real safepoint with the C's exact arguments so the shield
 * registration is byte-identical; args is reloaded from the frame afterwards by the common path. */
.Lpu_gc:
    mov     rdi, qword ptr [rsp + FR_ARGS]
    mov     esi, 2
    xor     edx, edx
    call    rt_gc_point_arr
    jmp     .Lpu_gc_done
/* The nargs guard's return: no frame was built, no global was touched. */
.Lpu_fail_bare:
    mov     eax, DT_FAIL | (MOD_OP_RT_PL_DOP_UNIFY << 8)   /* stamped (row descr-stamp-asm-mints) */
    xor     edx, edx
    ret
RTX_ENDF(rt_pl_dop_unify)

/* Non-executable stack marker: without it ld marks the whole .so RWX-stack. */
.section .note.GNU-stack,"",@progbits
