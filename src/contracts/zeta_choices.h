#ifndef ZETA_CHOICES_H
#define ZETA_CHOICES_H
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ZC_ALLOC AXIS RETIRED (RK-ZETA-4 close, 2026-07-17): the BUMP_INFINITE/BUMP_LIFO/MALLOC/GC allocator choice is superseded and had ZERO code consumers left
 * (sole reference was the zls_dump diagnostic label; the RK-ZETA-3 MALLOC-vs-BUMP_LIFO A/B was byte-identical BECAUSE the knob was disconnected, not because two
 * live allocators agreed).  The law it once selected (Lon ruling 2026-07-08 s6) stands, implemented by the layered end state: TWO zeta lifetime classes, two
 * providers, zero globals.  Control-flow-lifetime zeta rides the machine stack (ZC_PORT_FORTH fixed cells on rsp + the rbp value-slot frame under ZC_FRAME_RSP).
 * LONG-LIVED zeta — persists its data state, freed independent of control flow (suspended generator frames, coexpr stacks, COLLECTION captures) — lives on the
 * HEAP: rt_zls_alloc/release IS that provider, issuing fixed ZC_ZBLOCK_KB blocks through rt_arena_zblock_get/put (ZA-FLIP s67; GC-heap-carved when ZC_ZH_IN_GCHEAP). */
/* ZA-FLIP s67 (GC-U-2): the fixed reusable ζ-block class rt_zls_alloc issues through rt_arena_zblock_get/put — requests over one block route to the grow-only workspace (ZBF_WS). */
#ifndef ZC_ZBLOCK_KB
#define ZC_ZBLOCK_KB 64
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
/* ZC_PORT_CSTACK (C-STACK rung, Lon directive 2026-07-09: "implement all ZETA storage for SNOBOL4 on the C
 * stack"): the SAME grant-keyed BUMP/RELEASE protocol as INLINE with the cursor CELL RETIRED — the cursor IS
 * rsp, so the activation block lives on the machine C stack.  Everything below rsp is dead by the platform's
 * own contract, so C calls, signals, and libgc's conservative C-stack scan all compose for free (the mmap
 * arena carried none of those guarantees and needed explicit GC rooting).  Bumps round up to 16 so the
 * ambient call-site alignment mod 16 is preserved for the bare-call idiom; the statement backstop
 * (x86_zls2_mark_save / x86_zls2_release_to_call) collapses to ONE mov each (mark: FRQ(off)=rsp; release:
 * rsp=FRQ(off)), which is also what keeps rsp net-balanced at graph exit — every match statement brackets.
 * INHERITED STRUCTURAL LIMIT, stated honestly: a block that must SURVIVE a C return (suspended generator
 * activations, the s6 long-lived class) cannot ride the C stack; rt_proc_call_gen and the COLLECTION
 * provider are deliberately untouched.  Select with SCRIP_ZETA_PORT=4. */
#define ZC_PORT_CSTACK       4
/* DEFAULT RULING (Lon directive 2026-07-09: "implement all ZETA storage for SNOBOL4 on the C stack. Then
 * make it the default."): CSTACK is the compiled default — proven watermark-EXACT both modes (m3 263/17,
 * m4 262/6/12, DIVERGE=1(1017), fail lists byte-identical to the INLINE baseline), with the emitted-side
 * cursor = rsp (hook arms + statement backstop), C-side exact-bracket frames = alloca (4 proc sites +
 * the EVAL/CODE fragment frame, whose 64KB-per-crossing leak thereby closes), the graph entry/exit anchor
 * (xa_flat), and the rbx=GVA self-load at graph entry (the flow-through-C assumption was codegen luck; see
 * xa_flat.cpp).  The MAIN frame moved to the driver/main stack, RUNG ZS-1 s57 (was the memoized rt_frame arena block): allocated
 * once, never freed, shared across driver re-entries — it participates in no alloc/free discipline; moving
 * it is a mapped follow-on (sites scrip.c :1010/:1180 emitted, :1290-1302/:1307-1379 driver).  Suspended
 * generator activations (s6 long-lived class) stay on the heap provider by construction.  SCRIP_ZETA_PORT=3
 * remains the INLINE arena flavor; =2 ALLOC (poison/telemetry proving); =0 PLAIN; =1 INSTRUMENTED. */
/* ZC_PORT_OWNED (RUNG ZB-OWN-0, Lon directive 2026-07-11 "switch over to ZETA local storage BB-owned"):
 * INLINE base (cell cursor; arena; C-side arena proc frames) + the UNIVERSAL per-box discipline as a
 * balance-proving SHADOW: at every alpha AND beta define of a node holding a zls entry, the g_zls2_cur cell
 * is positioned ABSOLUTELY to (statement mark - C_i), C_i = zls_off(nd)+zls_node_bytes(nd) -- everything
 * left of the live box allocated, everything right dead.  DEFINE-sites only => flags-contract-clean, zero
 * omega-side code (conditional-omega exits need no free; the next landing repositions), beta idempotent (no
 * re-alloc hazard).  CELL not rsp at rung 0 BY NECESSITY: absolute rsp positioning would hoist over -- and
 * the next C call would smash -- any live CSTACK dynamic activation block (ARB/ARBNO) pushed below the mark;
 * rsp becomes reachable at ZB-OWN-1+ once region coalescing resolves static/dynamic coexistence.  Granted
 * dynamic blocks (ARB/ARBNO) keep the INLINE cell protocol under OWNED.  Select SCRIP_ZETA_PORT=5 /
 * --zeta-port 5; NOT the compiled default.  See GOAL-SNOBOL4-BB.md RUNG ZB-OWN. */
#define ZC_PORT_OWNED        5
/* ZC_PORT_FORTH (RUNG ZB-FC-0, Lon directive 2026-07-12 "Switch over to the FORTH zeta cell stack
 * implementation ... main spine on the RSP for all languages"; design of record ARCH-ZETA-LOCAL-STORAGE.md
 * S10 + this session's AMENDED TWO-FLAVOR LAW): a CSTACK SUPERSET -- everything CSTACK does (statement
 * bracket, dynamic ARB/ARBNO activation blocks on rsp, alloca proc frames) PLUS the per-BB fixed FORTH cell:
 * every fc_geom-granted box pushes its own FIXED-SIZE 16-multiple cell at alpha (sub rsp,K), addresses its
 * own locals rsp-relative through the FR/FRQ in-range translation (offset - op_fc_base), and pops at EVERY
 * omega (add rsp,K at the jmp-omega hook; conditional omegas are routed through the x86_jcc invert+pop+jmp
 * synth so the single hook arm serves all exit paths -- S10b's G3 made visible).  Cells are fixed size BY
 * LAW (Lon 2026-07-12): no allocation on the stack other than the one cell per box; variable COLLECTION
 * data (e.g. ARBNO's per-iteration array) belongs to the HEAP flavor with gamma/omega overloads doing the
 * alloc/free -- the escapable-BB story, ZB-ACT-3 territory.  gamma SUSPENDS the cell (S10c: live through all
 * gamma/beta cycling); beta emits nothing (LIFO put rsp back at the frontier).  v1 FENCE: fc_geom grants
 * IR_MATCH_SPAN only (the keystone looping box; private z/zo locals, no cross-box readers, dirty-cell-safe
 * because alpha writes before any read); widening is per-kind, verified against the S10c port invariant.
 * Select SCRIP_ZETA_PORT=6 / --zeta-port 6; NOT the compiled default. */
#define ZC_PORT_FORTH        6
/* ZC_PORT_HEAP (REG-4 slice A, GOAL-SNOBOL4-BB RUNG REG-MAP; = GC-U-2b HZ-1's register half, s77): the
 * HEAP-ZETA port flavor of the C2 two-flavor law — α allocates the box's ζ from the pinned bump frontier
 * [RT_WS_TOP] guarded by [RT_WS_LIMIT] (ja → rt_zh_bump_slow refill; lazy init falls out of the guard since
 * the REG-0 page is zero-filled), ω emits NOTHING (C3: escapee reclamation = the region's own LIVE/DEAD
 * lifecycle, never collected, never slid).  SLICE A IS THE ALLOCATION HALF ONLY: the box receives its ζ base
 * in rax, but FR/FRQ frame plumbing under heap residence (x86_zr for escapee classes) is HZ-1's census slice,
 * and the frontier's promotion from the cell into REGISTER rbx (the s73 map's GC-TOP tenant) is the named
 * REG-4b follow-up — the exact park→promote sequence proven on pend (REG-2 → REG-6).  Select
 * SCRIP_ZETA_PORT=7; NOT the compiled default (default FORTH ⇒ this arm is dormant ⇒ byte-identity). */
#define ZC_PORT_HEAP         7
/* ZC_ZH_IN_GCHEAP (GC-U-6 slice 2, s88): rt_zh_bump_slow carves its refill blocks FROM the collected span via rt_gcheap_alloc(HB_ZBLK), not private rt_slab_region — the rbx-guarded [RT_WS_TOP]
 * window is thereby a window INTO the ONE workspace and gc_collect_ex is the one collector over it (HB_ZBLK pinned-immortal v1 like slice 1's HB_WS: the v0 tail-residue leak becomes bounded pinned
 * blocks inside the walkable span; reclamation of dead windows = GC-U-7's root registration).  0 = the intact private-slab arm for A/B (the ZC_HEAP_STRINGS precedent). */
#ifndef ZC_ZH_IN_GCHEAP
#define ZC_ZH_IN_GCHEAP 1
#endif
/* ZC_COEXPR_STACK_GCHEAP (GC-U-6 slice 3, s89 Lon rulings: coexprs OWN their stack; gen procs = ordinary BBs, LIFO on the calling spine — escapee class is coexpr ONLY): each coexpr pthread stack is
 * CARVED from the collected span (HB_ZBLK window, page-aligned interior, low guard page PROT_NONE since setstack stacks get no libc guard), pinned-while-live (thread = liveness), and at
 * scrip_coexpr_destroy the guard is restored RW and the title flips HB_FILL — gc SLIDE reclaims dead coexpr stacks.  0 = the anonymous pthread_attr_setstacksize arm intact for A/B. */
#ifndef ZC_COEXPR_STACK_GCHEAP
#define ZC_COEXPR_STACK_GCHEAP 1
#endif
#ifndef ZC_PORT
#define ZC_PORT ZC_PORT_FORTH
#endif
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ZC_STORAGE — THE FOUR-CONFIG ζ-STORAGE SELECTOR (Lon directive 2026-07-28, GOAL-ZETA-FOUR.md rung Z4-4 slice 1).  ONE knob replaces the ZC_PORT ladder + the ZC_FRAME axis + ZC_ZETA + SCRIP_ZMODE.
 * FOUR values, because four techniques were built and each worked with limitations: FRAME_R12 = whole-graph ζ frame (all boxes' results+locals, 1 entry / 1 exit) on an r12-based island stack, the
 * original; FRAME_RSP = that same whole-graph frame moved onto the C stack, shared with C; CELL_STACK = per-BB cell, fixed rsp carve, with rbp pinned PER GRAPH for the dynamic-sized housekeeping
 * (ARBNO/FENCE/suspending generators) and rsp for the static-sized; CELL_HEAP = per-BB cell with LOCALS on the rbx-topped GC island and the RESULT on its own island (the VSP value stack).
 * ONE ENUM, NOT FOUR BOOLEANS, BY DESIGN: a graph mixing two regimes is the s188 failure shape, so the mixed states must be UNREPRESENTABLE rather than merely discouraged; this also collapses the
 * SCRIP_CELLS × SCRIP_ZMODE interaction matrix collapsed into four named points.  MEASURED at Z4-0/1/2 (m3, -O0, best-of-3, corpus/probe): FRAME_R12 5/5 correct
 * (the only 5/5 config, hence the CORRECTNESS ORACLE) · FRAME_RSP 5/5 · CELL_STACK 4/5 (capture defect, a regression bisectable to d79a427a..cca948c5) · CELL_HEAP incomplete (RUNG ZHEAP).
 * PERF, stated because it is the whole motive: CELL beats FRAME decisively on pattern work (span 3.3x, arbno 1.5x) and LOSES on calls -- z4_fib degraded MONOTONICALLY 71 -> 83 -> 98 ms across the
 * three generations, so activation cost is the open perf question and z4_fib is its instrument.  SLICE 1 IS THE ENUM ONLY: no consumer reads ZC_STORAGE yet, the legacy axes remain authoritative, and
 * the default maps to today's compiled behaviour (CELL_STACK == ZC_PORT_FORTH + ZC_FRAME_RSP) so output is byte-identical BY CONSTRUCTION.  Slice 2 adds --zeta-storage= / SCRIP_ZETA_STORAGE + the
 * runtime setter (clamped over the FULL range -- the s206 lesson, where a stale clamp silently reset port 7 to the default and made every m3-vs-m4 A/B on it invalid) + the mode-4 bake beside the
 * existing twins at scrip.c:1216/1407, so emitted code and runtime mode can never disagree.  Slice 3+ flips the seams per config; Z4-9 deletes the legacy axes.  ⛔ WHATEVER SURVIVES THE CUT MUST BE
 * COVERED BY Z4-10's GATE: Z4-0 measured FIVE OF SEVEN ports SEGV-ing at HEAD purely because nothing gated them -- an unguarded config rots by exactly that mechanism. */
#define ZC_STORAGE_FRAME_R12  0
#define ZC_STORAGE_FRAME_RSP  1
#define ZC_STORAGE_CELL_STACK 2
#define ZC_STORAGE_CELL_HEAP  3
#ifndef ZC_STORAGE
#define ZC_STORAGE ZC_STORAGE_CELL_STACK
#endif
#if ZC_STORAGE < ZC_STORAGE_FRAME_R12 || ZC_STORAGE > ZC_STORAGE_CELL_HEAP
#error "ZC_STORAGE must be one of ZC_STORAGE_{FRAME_R12,FRAME_RSP,CELL_STACK,CELL_HEAP} -- the closed four-config set (GOAL-ZETA-FOUR.md)"
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
/* ZC_FRAME_R12 (value 0) DELETED (ZR-RSPRBP-1, Lon directive 2026-07-27 "finish new ZETA storage based from RSP and RBP"): the r12-as-ζ basis was
 * retired in practice at R12-ERAD s65 when RSP became the default, and ARCH-ICON.md has recorded "R12 is FREE" ever since — but the ENUM arm survived
 * as compile-time-selectable history, so the tree still claimed three ζ bases while only two were reachable.  It had ZERO `#if ZC_FRAME == ZC_FRAME_R12`
 * consumers at deletion (measured, not assumed) and its only residue was the unreachable third ternary arm in x86_zr/x86_zr_num.  ζ is now based from
 * RSP and RBP, and that is the whole closed set.  RBP/RSP keep their values 1/2 — NOT renumbered, because nothing passes ZC_FRAME numerically (measured
 * across Makefile + scripts/) and renumbering would be churn that buys nothing.  The PER-GRAPH selection between the two lives in x86_fb_pinned(). */
#define ZC_FRAME_ISLE 0 /* Z4-7 slice 1 (GOAL-ZETA-FOUR.md): the LIVE-frame value rt_zc_frame_live() returns when the runtime selector says ZC_STORAGE_FRAME_R12.  This is the deleted ZC_FRAME_R12's old value 0 wearing a new name on purpose: the 17 `!= ZC_FRAME_RSP` arms were written FOR that basis (EXTRACT-Z4-R12.md §1 — R12 never named itself, the negative predicate WAS its selector), so making the live comparison resolve to a non-RSP value re-connects them without one positive `== R12` test appearing anywhere, exactly as the extraction doc prescribes.  NOT a ZC_FRAME build value — the build axis stays RSP-only until Z4-9 deletes it; every former compile-time `ZC_FRAME ==/!= ZC_FRAME_RSP` comparison now routes through rt_zc_frame_live()/x86_zc_frame(), which fold to the identical truth table under every non-island selection. */
#define ZC_FRAME_RBP 1
#define ZC_FRAME_RSP 2 /* rsp-as-ζ (Lon directive 2026-07-09): EMIT-side switch only today — runnable only after the
 * proc trampoline retires (no C frame above a live BB frame, zeta_alloc.c ZLS2 note) and escaping
 * activations (suspends/coexprs) live off-spine.  Selectable at BUILD TIME ONLY — set ZC_FRAME here or -DZC_FRAME=…; the env/runtime
 * switch is deleted (Lon 2026-07-09: never flipped at runtime). */
#ifndef ZC_FRAME
#define ZC_FRAME ZC_FRAME_RSP  /* R12-ERAD s65: RSP is the default (replaced ZC_FRAME_R12, which was here through s64 and is DELETED outright at ZR-RSPRBP-1) */
#endif
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ZETA SUBSYSTEM SELECTOR (Lon directive 2026-07-09: "we will want a command line switch to select the ZETA
 * mode") — WHICH zeta subsystem backs BB activation storage: ZLS (header-chained v1) vs ZLS2 (bare-bump down
 * arena), later the C-stack mode.  Runtime-selectable BY DESIGN via scrip --zeta=zls|zls2 (contrast ZC_FRAME
 * above, a build constant by the same day's directive); ZC_ZETA is only the DEFAULT when the flag is absent.
 * RUNG-0 (this change) = the selector spine only — flag → rt_zeta_set_mode → x86_zeta_mode(), zero seams
 * flipped, default output byte-identical.  RUNG-1 flips the dual-ready seams (ALLOC port flavor, generator
 * activations, statement backstop) on x86_zeta_mode(); the v1-bound pattern frames + graph-scope release are
 * NOT selectable until v1/v2 convergence — today the subsystems are co-resident, not alternatives. */
#define ZC_ZETA_ZLS  0
#define ZC_ZETA_ZLS2 1
/* ZH (Lon directive 2026-07-10): the BB-marked SLIDING heap for long-lived activations — alpha bumps, omega marks, the collector slides; handles not pointers across activations; frames
 * pinned while executing.  Rung 0 flips ONE seam: generator-procedure activations (rt_proc_call_gen_h / rt_proc_resume_frame_h, rt.c) route through zeta_heap.c when --zeta=zh; the
 * bounded-leave mass-kill primitive (rt_zh_kill_since) is built + unit-proven but its emit-side hook placement is the NEXT rung (the Icon twin of the SNOBOL4 statement backstop). */
#define ZC_ZETA_ZH   2
/* DEFAULT RULING (Lon directive 2026-07-10 s16: "Use the dual ZETA storage, stack for main spine and heap for
 * co-expressions and generator procedures"): ZH is the compiled default.  The dual storage is therefore the
 * default execution model — control-flow-lifetime zeta rides the C stack (ZC_PORT_CSTACK above), long-lived
 * activations (suspended generator frames; coexpr-side allocations, whose threads the ZH mutex already
 * serializes) ride the BB-marked sliding heap.  --zeta=zls2 / =zls remain selectable for A/B. */
#ifndef ZC_ZETA
#define ZC_ZETA ZC_ZETA_ZLS2 /* ZH is opt-in (--zeta=zh) until the det/nondet split rung lands (det-success accumulation: fib/tak/queens never cross a fail edge, so --zeta=zh never reclaims them; 22/22 bench only holds under ZLS2 default) */
#endif
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define ZC_HEAP_SCRIP 1
#ifndef ZC_HEAP_STRINGS
#define ZC_HEAP_STRINGS ZC_HEAP_SCRIP
#endif
#ifndef ZC_HEAP_MB
#define ZC_HEAP_MB 512
#endif
#ifndef ZC_WSI_MB
#define ZC_WSI_MB 1024
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
#define ZLS2_RESTORE 2 /* RETIRED (Lon ruling 2026-07-08 s7): only α and ω participate — the ω landing on a β already adjusted the cursor via its own RELEASE; never granted (zls2_geom), no hook site keys on it; bit kept so the mask meaning stays recorded */
#define ZLS2_RELEASE 4
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* SG SPAN-GUTS switches (GOAL-SNOBOL4-BB.md SG ladder, Lon directive s126 + s127 "unroll DEFAULT"): the bb_match_{span,break,breakx,any,notany,span_var} family.
 * ZC_SPAN_LIT_UNROLL   — level-1 LITERAL-needle loop-unroll on/off (s125 machinery); DEFAULT ON (Lon s127).  OFF = same body at factor 1 (one char-step per iteration).
 * ZC_UNROLL_FACTOR     — chars per unrolled literal-loop iteration (s125 hardcoded 4; lifted here, s125's owed switch).
 * ZC_CSET_CHAIN_MAX    — literal needle length at or below which the compare-chain is emitted instead of the 256B membership table (s127 A/B RESOLVED on treebank-lit x100: MAX=8 256ms / MAX=3 239ms / MAX=2 249ms — 4-char chains lose to the table, 1-3-char chains win; pattern_bt/claws5 wash).
 * ZC_SPAN_GUTS         — VARIABLE-needle (op_sa >= 0 / span_var) membership mechanism: INLINE = emitted inner loop over the needle bytes, slen-bounded; CALL = hand-written x86 leaf
 *                        (src/runtime/rt/rt_sg_scan.S, R13/R15-aware lean convention, no C ABI dance).  Both are NUL-SAFE by explicit length — the strchr-per-char arm is DELETED
 *                        (Lon s126: strchr truncates a NUL-bearing needle AND strchr(s,0) matches the terminator, falsely accepting a NUL subject byte as a member). */
#ifndef ZC_SPAN_LIT_UNROLL
#define ZC_SPAN_LIT_UNROLL 1
#endif
#ifndef ZC_UNROLL_FACTOR
#define ZC_UNROLL_FACTOR 4
#endif
#ifndef ZC_CSET_CHAIN_MAX
#define ZC_CSET_CHAIN_MAX 3
#endif
#define ZC_SPAN_GUTS_INLINE 0
#define ZC_SPAN_GUTS_CALL   1
#ifndef ZC_SPAN_GUTS
#define ZC_SPAN_GUTS ZC_SPAN_GUTS_INLINE
#endif
/* ZC_LIT_GUTS — LITERAL-needle (op_sa < 0) guts mechanism, the A/B/C flavor switch (Lon directive s128: "100% loop un-rolling versus INLINE versus CALL" on the match-only pair).  UNROLL = the s125
 * unrolled chain/table machinery (DEFAULT — today's bytes, gated byte-identical); INLINE = the SAME emitted inner needle-loop as the ZC_SPAN_GUTS INLINE arm, needle bytes reached RO ([rip+.Sn] TEXT /
 * movabs sval BINARY — sval lives on the pattern header's rcp, blob-lifetime-stable), length an imm32; CALL = the rt_sg_scan.S leafs with the RO needle pointer + imm32 length.  All three respect the
 * R13=Σ/R14=δ/R15=Δ contract.  bb_match_breakx keeps UNROLL-only this rung (β-extension arm doubles the surface; neither match-only program uses BREAKX). */
#define ZC_LIT_GUTS_UNROLL 0
#define ZC_LIT_GUTS_INLINE 1
#define ZC_LIT_GUTS_CALL   2
#define ZC_LIT_GUTS_RANGE  3
/* RANGE (Lon s128, "4th variant"): same unrolled subject-step body as UNROLL, but the membership test is a chain of RANGES — the literal cset's bytes coalesced into maximal contiguous runs at emit
 * time.  A single-byte run emits `cmp esi,b; je HIT` (2 inst); a multi-byte run emits the unsigned-sub trick `mov eax,esi; sub eax,lo; cmp eax,hi-lo; jbe HIT` (4 inst, eax dead at every use site).
 * SPAN(&LCASE &UCASE '0123456789') = 3 range tests instead of a 62-compare chain or a 256B table load. */
#ifndef ZC_LIT_GUTS
#define ZC_LIT_GUTS ZC_LIT_GUTS_UNROLL
#endif
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#if ZC_COLLECTION == ZC_COL_GC
#error "ZC_COL_GC is a stub until GC-4 lands (ARCH-ZETA-LOCAL-STORAGE.md section 6e)"
#endif
#if ZC_FRAME == ZC_FRAME_RBP
#error "ZC_FRAME_RBP DOES NOT RUN. Measured s202 by matched-pair A/B (same 20-program batch across SNOBOL4/Icon/Prolog, same invocation, compiler AND runtime rebuilt for each arm): RSP ok=15 crash=5, RBP ok=6 crash=14. Every one of the RSP control's 5 crashes is PRE-EXISTING AND DOCUMENTED -- 3 are the SNOBOL4 ARBNO-family bench crashers GOAL-SNOBOL4-BB.md records as 'all 5 bench CRASHers' (pattern_bt, string_pattern, roman), the other 2 are Icon jcon_args FZ-E1 and the jcon_btrees xfail -- so the basis flip alone contributes 9 NET NEW crashes. ROOT CAUSE, structural: the 17 'ZC_FRAME != ZC_FRAME_RSP' arms across bb_match_end/capture/head, bb_call_proc_staged, xa_flat and zeta_storage.c were written for the R12 basis (bb_call_proc_staged.cpp:281 says so verbatim: 'configs where r12 IS the zeta frame'), and ZR-RSPRBP-1 deleted ZC_FRAME_R12's LABEL while leaving that CODE, so the arms silently re-pointed at RBP without ever being designed for it. Under RBP x86_zr() and x86_fb() are BOTH rbp, so bb_match_end.cpp:32's push x86_zr() / mov x86_zr(),rsp push and repoint the frame base itself; and x86_align_save(), the helper whose whole purpose was making the C-call dance frame-safe under an rbp frame, has ZERO definitions in the tree while x86_asm.h:1435 still describes it as live. THE ZETA BASIS IS REACHABLE AT RSP ONLY. This guard is NOT the fix -- it converts a silent 9-crash trap into a loud one. The fix is Lon's call: re-establish the RBP arms against the current register contract, or delete all 17 and retire ZC_FRAME, leaving the PER-GRAPH x86_fb_pinned() rsp/rbp selection (FLATDISP-8) as the whole zeta RSP/RBP story -- the duality that actually runs and is already complete (FLATDISP-9 census, unseeded=0). Do NOT half-land the deletion: several arms sit inside suspend/resume protocols. See FINDING-2026-07-28b."
#endif
#if ZC_PROMOTE == ZC_PROMOTE_ON
#error "ZC_PROMOTE_ON is a stub until the heap-promotion rung lands (ARCH-ZETA-LOCAL-STORAGE.md section 7)"
#endif
#endif
