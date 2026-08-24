/* rtx_icnvar.S — RTX family ICNVAR.  Icon variable assignment (RTX-1-ICN).
 *
 * READ .github/ARCH-ICON-RTX.md BEFORE EDITING.  Contract macros: rtx_abi.inc.
 *
 * WHY THIS SYMBOL, AND NOT THE ONE THE STATIC TABLE NAMES:
 *   rt_assign_var is static rank 20 (147 sites) but the DYNAMIC #1 unported C
 *   symbol on Icon's board — 15,871 calls at queens N=8, 8.7x rt_call_arr's
 *   1,822, scaling 14.2x against a ~14x workload (FINDING-2026-07-29-CLAUDE-
 *   ICN-RTX-0D...).  Step 0(d) is DONE and is the only reason this rung exists;
 *   nothing in the static inventory predicted it.
 *
 * WHERE THE WIN COMES FROM — it is NOT -O0 frame ceremony:
 *   The C of record (src/runtime/pattern_match.c) opens every call with an
 *   UNCONDITIONAL GC safepoint: build a 2-slot DESCR_t shadow array on stack,
 *   lea it, call rt_gc_point_arr@PLT, then reload var and val back out of it.
 *   But that callee's entire body is
 *        int pv = g_gc_pending; if (!pv) return;
 *   So the common case pays ~6 memory ops plus a PLT call to read one int.
 *   This port tests g_gc_pending inline and reaches C only when a collect is
 *   genuinely pending — which is also exactly when the shadow array MATTERS,
 *   because a real collect may RELOCATE var and val.  The fast path is
 *   therefore only ever taken in the window where the array is provably dead.
 *
 * SAFE FALLTHROUGH — why re-entering C after partial work is not a bug:
 *   Every .Lav_c exit re-runs the two leading hooks inside c_rt_assign_var.
 *   Both are idempotent at that point: rt_gc_point_arr sees g_gc_pending == 0
 *   and returns immediately, and rt_sxt_break (if (s && s == g_sxt_owner)
 *   g_sxt_owner = 0) has already cleared the owner.  Order is preserved too:
 *   a pending collect jumps to C BEFORE the sxt break, exactly as the C
 *   sequences them.
 *
 * SCOPE — the cold half is deliberately NOT ported:
 *   NAMETRAP/VCELL, table stores and the tvsubs string-splice arm (which
 *   allocates, memcpy's three times and recurses) stay in C.  Porting them
 *   would be volume, not speed.
 *
 * ARG/RETURN PAIRS (rtx_abi.inc, verified s162):
 *   var: rdi = (var.slen << 32) | var.v   rsi = var.value
 *   val: rdx = (val.slen << 32) | val.v   rcx = val.value
 *   ret: rax = (slen << 32) | v           rdx = value
 *
 * STEP 0(c) — LINKAGE:
 *   g_gc_pending is `int g_gc_pending;` in gc_heap.c and appears in nm -D as
 *   B, i.e. EXPORTED and therefore PREEMPTIBLE => it must be reached through
 *   @GOTPCREL, never a direct [rip+sym].  This is the same rule that broke the
 *   step-0(d) interposer's first link (R_X86_64_PC32 against a shared object).
 *   g_sxt_owner is NOT a symbol at all — it is #define g_sxt_owner
 *   (g_sxt_fr.owner), so inlining it would hardcode a struct offset that no
 *   _Static_assert guards.  The call is kept; the C guards it on DT_S anyway.
 */
#include "rtx_abi.inc"

RTX_GATE_DEF(icnvar)

#define VCELL_TBL        8      /* VCELL_t.tbl     (TBBLK_t *)    */
#define VCELL_KEY_D      24     /* VCELL_t.key_d   (DESCR_t)      */

/*-----------------------------------------------------------------------------
 * DESCR_t rt_assign_var(DESCR_t var, DESCR_t val)
 *   C of record: src/runtime/pattern_match.c:1196 (renamed c_rt_assign_var)
 *
 *     { DESCR_t sh[2]; sh[0]=var; sh[1]=val; rt_gc_point_arr(sh,2,0);
 *       var=sh[0]; val=sh[1]; }
 *     if (val.v == DT_S) rt_sxt_break(val.s);
 *     if (var.v==DT_N && var.slen==0 && var.s && *var.s) { NV_SET_fn(var.s,val); return val; }
 *     if (var.v==DT_N && var.slen==1 && var.ptr)         { *(DESCR_t*)var.ptr = val; return val; }
 *     ... NAMETRAP / bomb / VCELL / tvsubs ...            -> stays in C
 */
RTX_FUNC(rt_assign_var)
    RTX_GATE(icnvar, c_rt_assign_var)

    /* (1) GC safepoint, inlined to a single load+compare. */
    mov     r10, [rip + g_gc_pending@GOTPCREL]   /* exported => GOT, not lea   */
    cmp     dword ptr [r10], 0
    jne     .Lav_c                      /* pending: C does the shield dance    */

    /* (2) sxt break — same DT_S guard the C uses. */
    cmp     dl, DT_S
    je      .Lav_sxt
.Lav_sxt_done:

    /* (3) var must be a name descriptor; dispatch on slen. */
    cmp     dil, DT_N
    jne     .Lav_c
    mov     r11, rdi
    shr     r11, 32                     /* r11d = var.slen                     */
    cmp     r11d, 1
    je      .Lav_cell                   /* 1 = direct cell pointer             */
    test    r11d, r11d
    je      .Lav_named                  /* 0 = named global                    */
    cmp     r11d, 2
    je      .Lav_nametrap               /* 2 = NAMETRAP -> VCELL               */
    jmp     .Lav_c                      /* anything else                       */

/*--- FAST PATH B: frame slot.  *(DESCR_t *)var.ptr = val; return val; -------
 *   Icon locals and (under the OLD frame-slot backend) globals land here.  It
 *   is the arm the queens/deal board actually drives.  ARCH-ICON.md keeps the
 *   frame-slot and NV-dictionary backends side by side by Lon directive, so
 *   BOTH arms are live and neither may be collapsed into the other. */
.Lav_cell:
    test    rsi, rsi
    je      .Lav_c
    mov     [rsi], rdx                  /* {v, slen}                           */
    mov     [rsi + 8], rcx              /* value union                         */
    mov     rax, rdx                    /* return val                          */
    mov     rdx, rcx
    ret

/*--- FAST PATH C: NAMETRAP -> VCELL cellp store  (RTX-1b-ICN, s209c) --------
 *   ⭐ THIS IS THE ARM ICON ACTUALLY TAKES.  s209b measured that all 147 of
 *   Icon's static rt_assign_var sites are SUBSCRIPTED assignment (L[i] := v);
 *   local/global/augmented/swap assignment are emitted inline by their own BB
 *   templates and never reach this symbol.  A subscript lvalue arrives as a
 *   NAMETRAP (slen==2) over a VCELL_t, so it takes NEITHER fast path A nor B —
 *   RTX-1-ICN ported two arms its own caller never enters, and excluded this
 *   one as "cold".  It is the only live arm.
 *
 *   C of record:
 *       VCELL_t *vc = (VCELL_t *)var.p; if (!vc) return FAILDESCR;
 *       if (vc->cellp) { *vc->cellp = val; return val; }
 *       ... vc->tbl, tvsubs splice ...            <- stay in C
 *
 *   cellp is the FIRST member of VCELL_t => offset 0.  Guarded by a
 *   _Static_assert in rtx_init.c: a silent layout drift here would store a
 *   descriptor through whatever pointer happened to land first, which links
 *   fine and corrupts quietly. */
.Lav_nametrap:
    test    rsi, rsi
    je      .Lav_c                      /* !vc -> C returns FAILDESCR          */
    mov     r10, [rsi]                  /* vc->cellp (offset 0)                */
    test    r10, r10
    jne     .Lav_cellp_store            /* nonzero: direct-cell store, below    */
    /* RTX-NEW-ICNVAR (seat01, 2026-08-24): cellp==0 -- check vc->tbl before falling
     * to C.  This file's own header above calls table stores "volume, not speed" and
     * deliberately leaves them cold; that call was right for Icon's queens board (the
     * workload this port was measured against) and is WRONG for what changed it: the
     * sibling fix in rtx_icnsub.S (RTX-31) now mints a (tbl,key_d) trap -- cellp==0,
     * tbl!=0 -- for EVERY SNOBOL4 table subscript-write, and table_access.sno alone
     * sends 10,500 of them here, ALL of which fell to c_rt_assign_var: the single
     * largest named C-side cost in the post-RTX-31 profile (8.93%). This is still not
     * "porting the insert" -- table_set_descr_d (aggregates.c) is called UNCHANGED,
     * same hash + sorted-bucket-insert it already does; this arm only skips the
     * redundant GC-safepoint/sxt/IS_NAMETRAP re-checks rt_assign_var's OWN asm entry
     * already did above, and calls that one function directly. See
     * perf-table-subscript-fastpath task LEDGER. */
    mov     r11, [rsi + VCELL_TBL]
    test    r11, r11
    je      .Lav_c                      /* tvsubs (sv-shaped) trap: still C's job */
    jmp     .Lav_table_store
.Lav_cellp_store:
    mov     [r10], rdx                  /* {v, slen}                           */
    mov     [r10 + 8], rcx              /* value union                         */
    mov     rax, rdx                    /* return val                          */
    mov     rdx, rcx
    ret

/*--- FAST PATH E: table store  (RTX-NEW-ICNVAR, 2026-08-24) -----------------
 * C of record, c_rt_assign_var_body's vc->tbl arm (pattern_match.c):
 *     table_set_descr_d(vc->tbl, vc->key_d, val); return val;
 * Calls the SAME C insert function, unchanged; only the two calling layers
 * around it (c_rt_assign_var -> c_rt_assign_var_body) and their redundant
 * GC-safepoint/sxt-break/IS_NAMETRAP re-checks are skipped -- this asm entry
 * already ran the one check that matters (the GC safepoint, top of function)
 * before ever reaching .Lav_nametrap.
 *
 * table_set_descr_d(TBBLK_t *tbl, DESCR_t k, DESCR_t val) -- five integer/
 * pointer args (tbl, k.v, k.value, val.v, val.value), all in registers, no
 * stack args needed (SysV allows six). val is PARKED across the call (two
 * pushes, 16 bytes, keeps RTX_CALL_ALIGN's exact 0-mod-16 alignment -- same
 * idiom as .Lav_sxt's four pushes above) because the call clobbers rdx/rcx
 * and the return value is val itself, not table_set_descr_d's (void).
 */
.Lav_table_store:
    mov     r9,  [rsi + VCELL_KEY_D]        /* key_d tag word                   */
    mov     r10, [rsi + VCELL_KEY_D + 8]    /* key_d payload                    */
    RTX_CALL_ALIGN
    push    rdx                             /* park val.tag (for the return)    */
    push    rcx                             /* park val.value; two pushes = 16B */
    mov     rdi, r11                        /* arg1 = tbl                       */
    mov     rsi, r9                         /* arg2 = key_d.tag                 */
    mov     rdx, r10                        /* arg3 = key_d.value               */
    mov     rcx, [rsp + 8]                  /* arg4 = val.tag                   */
    mov     r8,  [rsp]                      /* arg5 = val.value                 */
    call    table_set_descr_d@PLT
    pop     rcx                             /* val.value                        */
    pop     rdx                             /* val.tag                          */
    RTX_CALL_UNALIGN
    mov     rax, rdx                        /* return lo = val.tag              */
    mov     rdx, rcx                        /* return hi = val.value            */
    ret

/*--- FAST PATH A: named global.  NV_SET_fn(var.s, val); return val; ---------
 *   Note the C DISCARDS NV_SET_fn's return and yields val, so this cannot be a
 *   tail call; val is parked across the boundary instead. */
.Lav_named:
    test    rsi, rsi
    je      .Lav_c
    cmp     byte ptr [rsi], 0           /* *var.s — empty name falls to C,     */
    je      .Lav_c                      /* which bombs, exactly as before      */
    RTX_CALL_ALIGN
    push    rdx                         /* park val.tag                        */
    push    rcx                         /* park val.value; the pair re-aligns  */
    mov     rdi, rsi                    /* arg1   = var.s                      */
    mov     rsi, rdx                    /* arg2lo = val.tag                    */
    mov     rdx, rcx                    /* arg2hi = val.value                  */
    call    NV_SET_fn@PLT
    pop     rdx                         /* val.value -> return hi              */
    pop     rax                         /* val.tag   -> return lo              */
    RTX_CALL_UNALIGN
    ret

/*--- out-of-line: rt_sxt_break(val.s), all four arg registers preserved ----*/
.Lav_sxt:
    RTX_CALL_ALIGN
    push    rdi
    push    rsi
    push    rdx
    push    rcx                         /* four pushes = 32B, stays 16-aligned */
    mov     rdi, rcx
    call    rt_sxt_break@PLT
    pop     rcx
    pop     rdx
    pop     rsi
    pop     rdi
    RTX_CALL_UNALIGN
    jmp     .Lav_sxt_done

/*--- cold: hand the untouched argument registers to the C body -------------*/
.Lav_c:
    jmp     c_rt_assign_var
RTX_ENDF(rt_assign_var)

.section .note.GNU-stack,"",@progbits
