/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef ZETA_CHOICES_H
#define ZETA_CHOICES_H
#ifndef ZC_ZBLOCK_KB
#define ZC_ZBLOCK_KB 64
#endif
#define ZC_COL_MALLOC 0
#define ZC_COL_ARENA  1
#define ZC_COL_GC     2
#ifndef ZC_COLLECTION
#define ZC_COLLECTION ZC_COL_MALLOC
#endif
#define ZC_SELFLOAD_OFF        0
#define ZC_SELFLOAD_PLANE_CELL 1
#define ZC_SELFLOAD_ASSERT     2
#define ZC_SELFLOAD_STATIC     3
#define ZC_SELFLOAD_ALLOC      4
#ifndef ZC_SELFLOAD
#define ZC_SELFLOAD ZC_SELFLOAD_OFF
#endif
#define ZC_PORT_PLAIN        0
#define ZC_PORT_INSTRUMENTED 1
#define ZC_PORT_ALLOC        2
#define ZC_PORT_INLINE       3
#define ZC_PORT_CSTACK       4
#define ZC_PORT_OWNED        5
#define ZC_PORT_FORTH        6
#define ZC_PORT_HEAP         7
#ifndef ZC_ZH_IN_GCHEAP
#define ZC_ZH_IN_GCHEAP 1
#endif
#ifndef ZC_COEXPR_STACK_GCHEAP
#define ZC_COEXPR_STACK_GCHEAP 1
#endif
#ifndef ZC_PORT
#define ZC_PORT ZC_PORT_FORTH
#endif
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
#define ZC_INIT_ZERO  0
#define ZC_INIT_NONE  1
#define ZC_INIT_CLONE 2
#ifndef ZC_INIT
#define ZC_INIT ZC_INIT_ZERO
#endif
#define ZC_POISON_OFF  0
#define ZC_POISON_FILL 1
#ifndef ZC_POISON
#define ZC_POISON ZC_POISON_FILL
#endif
#define ZC_TELEM_OFF 0
#define ZC_TELEM_ON  1
#ifndef ZC_TELEMETRY
#define ZC_TELEMETRY ZC_TELEM_ON
#endif
#define ZC_OVF_BOMB  0
#define ZC_OVF_GUARD 1
#ifndef ZC_OVERFLOW
#define ZC_OVERFLOW ZC_OVF_BOMB
#endif
#ifndef ZC_ARENA_MB
#define ZC_ARENA_MB 1024
#endif
#ifndef ZC_ZLS2_MB
#define ZC_ZLS2_MB 512
#endif
#define ZC_FRAME_ISLE 0
#define ZC_FRAME_DEAD5 1
#define ZC_FRAME_RSP 2
#ifndef ZC_FRAME
#define ZC_FRAME ZC_FRAME_RSP
#endif
#define ZC_ZETA_ZLS  0
#define ZC_ZETA_ZLS2 1
#define ZC_ZETA_ZH   2
#ifndef ZC_ZETA
#define ZC_ZETA ZC_ZETA_ZLS2
#endif
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
#define ZC_PROMOTE_GATE 0
#define ZC_PROMOTE_ON   1
#ifndef ZC_PROMOTE
#define ZC_PROMOTE ZC_PROMOTE_GATE
#endif
#define ZLS2_BUMP    1
#define ZLS2_RESTORE 2
#define ZLS2_RELEASE 4
#ifndef ZC_CSET_CHAIN_MAX
#define ZC_CSET_CHAIN_MAX 3
#endif
#define ZC_SPAN_GUTS_INLINE 0
#define ZC_SPAN_GUTS_CALL   1
#ifndef ZC_SPAN_GUTS
#define ZC_SPAN_GUTS ZC_SPAN_GUTS_INLINE
#endif
#define ZC_LIT_GUTS_UNROLL 0
#define ZC_LIT_GUTS_INLINE 1
#define ZC_LIT_GUTS_CALL   2
#define ZC_LIT_GUTS_RANGE  3
#ifndef ZC_LIT_GUTS
#define ZC_LIT_GUTS ZC_LIT_GUTS_UNROLL
#endif
#if ZC_COLLECTION == ZC_COL_GC
#error "ZC_COL_GC is a stub until GC-4 lands (ARCH-ZETA-LOCAL-STORAGE.md section 6e)"
#endif
#if ZC_FRAME == ZC_FRAME_DEAD5
#error "ZC_FRAME_DEAD5 DOES NOT RUN. Measured s202 by matched-pair A/B (same 20-program batch across SNOBOL4/Icon/Prolog, same invocation, compiler AND runtime rebuilt for each arm): RSP ok=15 crash=5, FB5 ok=6 crash=14. Every one of the RSP control's 5 crashes is PRE-EXISTING AND DOCUMENTED -- 3 are the SNOBOL4 ARBNO-family bench crashers GOAL-SNOBOL4-BB.md records as 'all 5 bench CRASHers' (pattern_bt, string_pattern, roman), the other 2 are Icon jcon_args FZ-E1 and the jcon_btrees xfail -- so the basis flip alone contributes 9 NET NEW crashes. ROOT CAUSE, structural: the 17 'ZC_FRAME != ZC_FRAME_RSP' arms across bb_match_end/capture/head, bb_call_proc_staged, xa_flat and zeta_storage.c were written for the R12 basis (bb_call_proc_staged.cpp:281 says so verbatim: 'configs where r12 IS the zeta frame'), and ZR-RSPFB5-1 deleted ZC_FRAME_R12's LABEL while leaving that CODE, so the arms silently re-pointed at FB5 without ever being designed for it. Under FB5 x86_zr() and x86_fb() are BOTH ___, so bb_match_end.cpp:32's push x86_zr() / mov x86_zr(),rsp push and repoint the frame base itself; and x86_align_save(), the helper whose whole purpose was making the C-call dance frame-safe under an ___ frame, has ZERO definitions in the tree while x86_asm.h:1435 still describes it as live. THE ZETA BASIS IS REACHABLE AT RSP ONLY. This guard is NOT the fix -- it converts a silent 9-crash trap into a loud one. The fix is Lon's call: re-establish the FB5 arms against the current register contract, or delete all 17 and retire ZC_FRAME, leaving the PER-GRAPH x86_fb_pinned() rsp/___ selection (FLATDISP-8) as the whole zeta RSP/FB5 story -- the duality that actually runs and is already complete (FLATDISP-9 census, unseeded=0). Do NOT half-land the deletion: several arms sit inside suspend/resume protocols. See FINDING-2026-07-28b."
#endif
#if ZC_PROMOTE == ZC_PROMOTE_ON
#error "ZC_PROMOTE_ON is a stub until the heap-promotion rung lands (ARCH-ZETA-LOCAL-STORAGE.md section 7)"
#endif
#endif
