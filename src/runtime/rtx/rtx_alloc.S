/* rtx_alloc.S — RTX family ALLOC (rung RTX-2, s163).
 *
 * READ .github/ARCH-SNOBOL4-RTX.md BEFORE EDITING. Contract macros: rtx_abi.inc.
 *
 * WHAT THIS FAMILY ACTUALLY IS (corrected s163 — see the rung's FINDING):
 *   ARCH section 5 names the ALLOC family "blk_alloc / blk_free". Those symbols are DEAD:
 *   zero live call sites, their C sits in archive/backend/, and they appear only in the five
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

/* Non-executable stack marker: without it ld marks the whole .so RWX-stack. */
.section .note.GNU-stack,"",@progbits
