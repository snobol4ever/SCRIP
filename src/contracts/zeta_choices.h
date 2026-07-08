#ifndef ZETA_CHOICES_H
#define ZETA_CHOICES_H
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define ZC_ALLOC_BUMP_INFINITE 0
#define ZC_ALLOC_BUMP_LIFO     1
#define ZC_ALLOC_MALLOC        2
#define ZC_ALLOC_GC            3
/* Lon ruling 2026-07-08 s6: TWO zeta lifetime classes, two providers, zero globals. Control-flow-lifetime
 * zeta rides the ZLS2 down-arena (the stack; r12-destined). LONG-LIVED zeta — persists its data state, freed
 * independent of control flow (suspended generator frames, COLLECTION captures) — lives on the HEAP: the v1
 * rt_zls_alloc/release pair IS that heap provider (malloc + GC roots, per-block free). The v1 up-arena is
 * therefore retired from the default build (never mmapped under MALLOC). */
#ifndef ZC_ALLOC
#define ZC_ALLOC ZC_ALLOC_MALLOC
#endif
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define ZC_COL_MALLOC 0
#define ZC_COL_ARENA  1
#define ZC_COL_GC     2
#ifndef ZC_COLLECTION
#define ZC_COLLECTION ZC_COL_MALLOC
#endif
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define ZC_SELFLOAD_OFF        0
#define ZC_SELFLOAD_PLANE_CELL 1
#define ZC_SELFLOAD_ASSERT     2
#define ZC_SELFLOAD_STATIC     3
#define ZC_SELFLOAD_ALLOC      4
#ifndef ZC_SELFLOAD
#define ZC_SELFLOAD ZC_SELFLOAD_OFF
#endif
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define ZC_PORT_PLAIN        0
#define ZC_PORT_INSTRUMENTED 1
#define ZC_PORT_ALLOC        2
/* ZC_PORT_INLINE (ZETA-INLINE rung, GOAL-SNOBOL4-BB.md, first slice 2026-07-08 s7): the SAME BUMP/RESTORE/
 * RELEASE frame protocol as ALLOC, emitted as raw cursor arithmetic on the exported g_zls2_cur cell — zero C
 * calls, zero align dances.  The cursor stays a memory cell (not r12: repointing r12 breaks every sibling
 * [r12+off] reference, the documented wall; the register-cursor end state waits on the activation-local-
 * offsets contract).  Dropped vs ALLOC by design: poison, telemetry, trace, LIFO aborts, the exhaustion bomb
 * (overflow protection = the unmapped address space below the MAP_NORESERVE reserve — a touch below g_zls2_lo
 * faults; the ZC_OVF_GUARD idea for free).  ALLOC remains the proving flavor; INLINE is the perf flavor.
 * Select with SCRIP_ZETA_PORT=3. */
#define ZC_PORT_INLINE       3
/* DEFAULT RULING (Lon session directive 2026-07-08 s7: "Finish optimized BB memory allocation where it's all
 * routed through x86_*() port hooks"): INLINE is the compiled default — proven watermark-EXACT both modes,
 * Icon/Prolog crosschecks clean, zero gate regressions, adversarial ARBNO micro 2150ms→419ms (5.2×, parity
 * with PLAIN), pattern_bt outputs identical.  SCRIP_ZETA_PORT=2 remains the ALLOC proving flavor (poison,
 * telemetry, LIFO aborts); =0 PLAIN; =1 INSTRUMENTED. */
#ifndef ZC_PORT
#define ZC_PORT ZC_PORT_INLINE
#endif
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define ZC_INIT_ZERO  0
#define ZC_INIT_NONE  1
#define ZC_INIT_CLONE 2
#ifndef ZC_INIT
#define ZC_INIT ZC_INIT_ZERO
#endif
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define ZC_POISON_OFF  0
#define ZC_POISON_FILL 1
#ifndef ZC_POISON
#define ZC_POISON ZC_POISON_FILL
#endif
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define ZC_TELEM_OFF 0
#define ZC_TELEM_ON  1
#ifndef ZC_TELEMETRY
#define ZC_TELEMETRY ZC_TELEM_ON
#endif
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define ZC_OVF_BOMB  0
#define ZC_OVF_GUARD 1
#ifndef ZC_OVERFLOW
#define ZC_OVERFLOW ZC_OVF_BOMB
#endif
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef ZC_ARENA_MB
#define ZC_ARENA_MB 1024
#endif
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef ZC_ZLS2_MB
#define ZC_ZLS2_MB 512
#endif
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define ZC_FRAME_R12 0
#define ZC_FRAME_RBP 1
#ifndef ZC_FRAME
#define ZC_FRAME ZC_FRAME_R12
#endif
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define ZC_HEAP_LIBGC 0
#define ZC_HEAP_SCRIP 1
#ifndef ZC_HEAP_STRINGS
#define ZC_HEAP_STRINGS ZC_HEAP_SCRIP
#endif
#ifndef ZC_HEAP_MB
#define ZC_HEAP_MB 512
#endif
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define ZC_PROMOTE_GATE 0
#define ZC_PROMOTE_ON   1
#ifndef ZC_PROMOTE
#define ZC_PROMOTE ZC_PROMOTE_GATE
#endif
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ZLS2 port-hook injection ops (Lon directive 2026-07-08: frame protocol injected at the α/β/ω port seam;
 * templates stay untouched).  Per-node bitmask computed by zls2_geom (zeta_storage.c, the save-slot layout
 * authority), promoted into g_emit.op_zls2_ops at the emit dispatch point, read by x86_port_hook (x86_asm.h)
 * under ZC_PORT_ALLOC:
 *   BUMP    fires at the α define — rt_zls2_push(K), prev-chain via the save slot, store block to slot.
 *   RESTORE fires at the β define — rt_zls2_release_to(slot block): backtrack arrival resets the arena
 *           cursor to this frame, wholesale-freeing every failed successor's frame (the fail-direction
 *           release).  Idempotent; the runtime hard-aborts on LIFO violation.
 *   RELEASE fires before a jmp-ω — unchain + rt_zls2_release_to(block+K).  Granted ONLY to roles whose
 *           ω-jumps are statically all activation-death (the grant is the classifier; op_omega_is_death is
 *           recorded broken and never consulted). */
#define ZLS2_BUMP    1
#define ZLS2_RESTORE 2
#define ZLS2_RELEASE 4
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#if ZC_ALLOC == ZC_ALLOC_GC
#error "ZC_ALLOC_GC is a stub until GC-3 lands (ARCH-ZETA-LOCAL-STORAGE.md section 6e)"
#endif
#if ZC_COLLECTION == ZC_COL_GC
#error "ZC_COL_GC is a stub until GC-4 lands (ARCH-ZETA-LOCAL-STORAGE.md section 6e)"
#endif
#if ZC_COLLECTION == ZC_COL_ARENA && ZC_ALLOC == ZC_ALLOC_MALLOC
#error "ZC_COL_ARENA (grow-in-place-when-top) requires a bump ZC_ALLOC arena"
#endif
#if ZC_PROMOTE == ZC_PROMOTE_ON
#error "ZC_PROMOTE_ON is a stub until the heap-promotion rung lands (ARCH-ZETA-LOCAL-STORAGE.md section 7)"
#endif
#endif
