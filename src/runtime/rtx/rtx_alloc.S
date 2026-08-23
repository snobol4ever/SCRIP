/* rtx_alloc.S — RTX family ALLOC (rung RTX-2, s163).
 *
 * READ .github/ARCH-SNOBOL4-RTX.md BEFORE EDITING. Contract macros: rtx_abi.inc.
 *
 * WHAT THIS FAMILY ACTUALLY IS (corrected s163 — see the rung's FINDING):
 *   ARCH section 5 names the ALLOC family "blk_alloc / blk_free". Those symbols are DEAD:
 *   zero live call sites, their C sat in archive/backend/ (deleted s267, in git history), and they appear only in the five
 *   legacy `scrip-cc -asm` artifacts that the inventory script sweeps alongside real output.
 *   The LIVE mint is rt_gcheap_alloc — the SIL bump allocator every string, aggregate and
 *   workspace block is carved from (manual p.216: bump until exhaustion, then regenerate).
 *
 * WHAT IS PORTED: the DETAX FAST PATH ONLY — armed, room at the frontier, and the carve
 * landing at or above the HP-2 virgin high-water mark so no payload zeroing is owed. That
 * is the hot case: a forward bump through untouched mmap-fresh pages.
 *
 * WHAT IS DELIBERATELY *NOT* PORTED (C keeps sole ownership — contract section 4):
 *   lazy heap init . GC trigger and storage regeneration . the fill-window second cursor .
 *   every path that owes a memset (recycled memory below virgin, or SCRIP_ZSKIP_OFF=1) .
 *   the alloc histogram . heap exhaustion. Each falls through to c_rt_gcheap_alloc with
 *   edi/rsi UNTOUCHED, so the tail jump needs no argument staging and is the byte-identity
 *   proof path. rt_gc_collect must never run under our feet: we jump to C before any
 *   condition that could provoke it.
 *
 * PRECEDENT: bb_call_fn.cpp's sink_carve48 already inlines this exact fast path for one
 * fixed shape (HB_PLJ/48B) via the PL-SINK-3 exported cell. This is the general form of
 * that same carve, so the two must agree — if you change one, read the other.
 *
 * ADDRESSING NOTE: g_hp_fr is EXPORTED (the emitter bakes its address into sink_carve48 and so
 * the driver must see it), therefore preemptible, therefore reached through the GOT rather than
 * a direct `lea [rip+sym]` — unlike the gate byte, which is .hidden precisely to avoid that. One
 * extra load per call, hoisted before every branch so it is never on a slow-path-only edge.
 *
 * g_hp_fr LAYOUT (baked, _Static_assert-anchored in gc_heap.c):
 *   +0 top (char*) . +8 end (char*) . +16 blocks (long) . +24 armed (int)
 *   +32 virgin (char*) . +40 zfull (int)         <- 32/40 added by RTX-2
 * rt_hblk_t TITLE (16B, gc_heap.h): +0 fwd (u64) . +8 size (u32) . +12 type (u16) . +14 flags (u16)
 *   type and flags are ONE aligned dword at +12, so the title costs three stores, not four.
 */
#include "rtx_abi.inc"

RTX_GATE_DEF(alloc)

#define HBF_TTL 0x0001
#define HB_AGGV 206                     /* gc_heap.h; anchored by _Static_assert in rtx_init.c */

/*-----------------------------------------------------------------------------
 * void *rt_gcheap_alloc(uint16_t type, uint64_t payload_bytes)
 *   C of record: src/runtime/rt/gc_heap.c c_rt_gcheap_alloc + c_rt_gcheap_carve
 *
 *   Args: edi = type, rsi = payload_bytes.   Returns: rax = payload pointer.
 *   Clobbers rax/rcx/r10/r11 only — edi and rsi survive for the tail jump.
 *
 *   C being reproduced, exactly:
 *     if (armed) { total = 16 + ((payload + 15) & ~15);
 *                  if (top + total <= end) { carve(top, total, type); top += total; return; } }
 *     carve: h->fwd = 0; h->size = total; h->type = type; h->flags = HBF_TTL;
 *            fresh = !zfull && at >= virgin;                    <- computed BEFORE virgin moves
 *            if (at >= virgin && at + total > virgin) virgin = at + total;
 *            if (fresh) {} else <memset paths>;  blocks += 1;  return h + 1;
 */
RTX_FUNC(rt_gcheap_alloc)
    RTX_GATE(alloc, c_rt_gcheap_alloc)
    mov     r10, [rip + g_hp_fr@GOTPCREL]  /* exported+preemptible => GOT, not lea */
    mov     eax, dword ptr [r10 + 24]   /* armed: mirrors detax==1 && hist off  */
    test    eax, eax
    je      .Lga_slow
.Lga_armed:                             /* rt_str_alloc enters here, r10 loaded */
    mov     eax, dword ptr [r10 + 40]   /* zfull: -1 unresolved, 1 force-full   */
    test    eax, eax
    jne     .Lga_slow                   /* either one -> C resolves / memsets   */
    lea     rcx, [rsi + 15]
    and     rcx, -16
    add     rcx, 16                     /* total = 16 + round16(payload)        */
    mov     r11, [r10 + 0]              /* at = top                             */
    mov     rax, r11
    add     rax, rcx                    /* newtop                               */
    jc      .Lga_slow                   /* address wrap on absurd size -> C     */
    cmp     rax, [r10 + 8]
    ja      .Lga_slow                   /* newtop > end -> C (may collect)      */
    cmp     r11, [r10 + 32]
    jb      .Lga_slow                   /* below virgin: recycled, owes memset  */
    mov     qword ptr [r11 + 0], 0      /* title.fwd  = 0                       */
    mov     dword ptr [r11 + 8], ecx    /* title.size = total                   */
    movzx   ecx, di
    or      ecx, HBF_TTL << 16
    mov     dword ptr [r11 + 12], ecx   /* title.type | title.flags, one store  */
    mov     [r10 + 32], rax             /* virgin = at + total (at >= virgin)   */
    mov     [r10 + 0], rax              /* top    = at + total                  */
    add     qword ptr [r10 + 16], 1     /* blocks += 1                          */
    lea     rax, [r11 + 16]             /* return h + 1                         */
    ret
.Lga_slow:
    jmp     c_rt_gcheap_alloc
RTX_ENDF(rt_gcheap_alloc)

/*-----------------------------------------------------------------------------
 * char *rt_str_alloc(long n)              C of record: gc_heap.c c_rt_str_alloc
 *
 *   if (rt_alloc_hist_on()) rt_alloc_hist_ra(...);
 *   want = (n < 0 ? 0 : n) + 1;                 n characters plus the NUL slot
 *   return rt_gcheap_alloc(DT_S, want);
 *
 *   THE FUSION: `armed` already encodes "histogram is off", so when it is set the
 *   histogram call provably cannot fire and is not merely skipped but ELIMINATED —
 *   this entry falls into the carve with no call at all. When armed is clear we hand
 *   the whole thing back to C, which still owns the histogram and the lazy init.
 */
RTX_FUNC(rt_str_alloc)
    RTX_GATE(alloc, c_rt_str_alloc)
    mov     r10, [rip + g_hp_fr@GOTPCREL]  /* exported+preemptible => GOT, not lea */
    mov     eax, dword ptr [r10 + 24]
    test    eax, eax
    je      c_rt_str_alloc              /* not armed -> C owns hist + init      */
    xor     eax, eax
    test    rdi, rdi
    mov     rsi, rdi
    cmovs   rsi, rax                    /* n < 0 -> 0                           */
    inc     rsi                         /* want = n + 1                         */
    mov     edi, DT_S
    jmp     .Lga_armed                  /* armed already proven, r10 loaded     */
RTX_ENDF(rt_str_alloc)

/*-----------------------------------------------------------------------------
 * void *rt_agg_alloc(int kind, size_t n)     C of record: gc_heap.c c_rt_agg_alloc
 *
 *   Args: edi = kind, rsi = n.   Returns: rax = payload pointer.
 *
 *   C being reproduced, exactly:
 *     ty = HB_AGGV + (kind < 0 ? 0 : (kind > 2 ? 2 : kind));
 *     if (g_alloc_detax != 1 && rt_alloc_hist_on()) rt_alloc_hist_ra(retaddr, ty, n);
 *     return rt_gcheap_alloc(ty, n ? n : 1);
 *
 *   THE SAME FUSION AS rt_str_alloc, AND IT IS A PROOF, NOT A SKIP: `armed` mirrors
 *   (g_alloc_detax == 1 && g_ah_on <= 0), so armed set => g_alloc_detax == 1 => the
 *   guard `g_alloc_detax != 1` is FALSE => the histogram call provably cannot fire.
 *   It is therefore ELIMINATED on this path, not merely branched around. When armed is
 *   clear we hand the whole call back to C, which still owns the histogram and lazy init.
 *
 *   WHY THIS SYMBOL IS WORTH A PORT AT ALL (measured s204, step 0(d), and stated so a
 *   null result is informative): rt_agg_alloc is a SIX-LINE WRAPPER whose own body is
 *   pure -O0 ceremony -- prologue, argument spills, a call to rt_alloc_hist_on(), and a
 *   second call frame for rt_gcheap_alloc, which is ITSELF already asm (RTX-2). The port
 *   deletes the frame and turns the inner call into a fallthrough. Density: table_churn
 *   8,001,201 calls (scales linearly: 16,001,201 at 2x passes) . table_access 7,506,501 .
 *   string_manip 0.  Gate on AGG/ALLOC programs, NEVER on string_manip or the rail.
 *
 *   CLAMP IS SIGNED (kind is int): cmovs for kind<0, cmovg for kind>2 -- branchless, and
 *   the two cmovs cost less than the single mispredict the C version's branches invite.
 */
RTX_FUNC(rt_agg_alloc)
    RTX_GATE(alloc, c_rt_agg_alloc)
    mov     r10, [rip + g_hp_fr@GOTPCREL]  /* exported+preemptible => GOT, not lea */
    mov     eax, dword ptr [r10 + 24]   /* armed: mirrors detax==1 && hist off  */
    test    eax, eax
    je      c_rt_agg_alloc              /* not armed -> C owns hist + init      */
    mov     eax, edi
    xor     ecx, ecx
    test    eax, eax
    cmovs   eax, ecx                    /* kind < 0 -> 0                        */
    mov     ecx, 2
    cmp     eax, 2
    cmovg   eax, ecx                    /* kind > 2 -> 2                        */
    add     eax, HB_AGGV
    mov     edi, eax                    /* type = HB_AGGV + clamp(kind,0,2)     */
    mov     rcx, 1
    test    rsi, rsi
    cmove   rsi, rcx                    /* n ? n : 1                            */
    jmp     .Lga_armed                  /* armed already proven, r10 loaded     */
RTX_ENDF(rt_agg_alloc)

/* Non-executable stack marker: without it ld marks the whole .so RWX-stack. */
.section .note.GNU-stack,"",@progbits
