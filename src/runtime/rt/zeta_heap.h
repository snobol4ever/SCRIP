#ifndef ZETA_HEAP_H
#define ZETA_HEAP_H
/* ZH — the BB-MARKED SLIDING ZETA HEAP (Lon directive 2026-07-10): long-lived BB activation storage whose lifetime is marked SOLELY by the boxes — alpha BUMPS (rt_zh_alloc), omega MARKS
 * garbage (rt_zh_mark_dead / rt_zh_kill_since at bounded leaves), and the collector SLIDES (compaction inside rt_zh_alloc on exhaustion).  No tracer, no root scan of zeta: frames hold only
 * slide-invariant content (scalars + DESCRs into the value heap + code addresses) — NEVER a raw address into this heap; cross-activation references are HANDLES (small ints) through one
 * table the slide rewrites.  A frame is PINNED while its blob executes (the only raw-pointer window: r12 + trampoline locals), so a slide triggered by a nested alpha moves only suspended,
 * unpinned frames.  Selected at runtime via scrip --zeta=zh (ZC_ZETA_ZH, zeta_choices.h); default modes are byte-identical-untouched.  Slab is GC-scanned (one root range — retires the
 * per-frame root-set class) and never collected; value-heap payloads referenced from LIVE frames stay alive through it. */
unsigned rt_zh_alloc(long bytes, void **ubout);
void    *rt_zh_deref(unsigned h);
void     rt_zh_pin(unsigned h);
void     rt_zh_unpin(unsigned h);
void     rt_zh_mark_dead(unsigned h);
unsigned rt_zh_birthmark(void);
void     rt_zh_kill_since(unsigned mark);
long     rt_zh_live_count(void);
#endif
