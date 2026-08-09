/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ab_abi.h — LADDER AB: ACTIVATION BLOCK ABI contract (frozen at AB-1, 2026-08-09).                                                                                                                 */
/* ONE AUTHORITY: this header is the only place AB frame offsets, the <argreg> choice, and the Δ/Ω verdict are spelled.  All templates and runtime consumers include this file.                       */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef AB_ABI_H
#define AB_ABI_H
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ACTIVATION FRAME LAYOUT (RBP-relative; all offsets NEGATIVE — cells live below the saved-rbp pushed by push rbp).                                                                                  */
/* Established by α: push rbp; mov rbp,rsp; sub rsp,K.  Released by β: mov rsp,rbp; pop rbp (or LEAVE).                                                                                              */
/* The pushed rbp occupies [rbp-8] (below the saved caller rbp value = at [rbp+0]); all frame cells start at [rbp-0x10].                                                                              */
/*                                                                                                                                                                                                    */
/* Meta fields (6 × 8B = 48B; AB_META_BYTES):                                                                                                                                                        */
#define AB_OFF_GW       (-0x10)   /* γ wire  — caller's success continuation (lea rcx,γ at call site)    — filled at AB-2 */
#define AB_OFF_WW       (-0x18)   /* ω wire  — caller's failure continuation (lea rdx,ω at call site)    — filled at AB-2 */
#define AB_OFF_ERSP     (-0x20)   /* entry rsp at α (caller's RSP before α's sub-rsp)                    — filled at AB-2 */
#define AB_OFF_ANCHOR   (-0x28)   /* prev ACT-ANCHOR value (linked list of active frames)                 — filled at AB-2 */
#define AB_OFF_WN       (-0x30)   /* rt_g_want_name snapshot (caller wants NAME vs VALUE for NRETURN)     — filled at AB-2 */
#define AB_OFF_VTMARK   (-0x38)   /* value-trail mark (GC correctness: tidy dead window at β)             — filled at AB-2 */
/* Save-set: nsave × 16B DESCR cells, starting immediately below the meta block.                                                                                                                      */
/* Member order: [0]=fname, [1..nformals-1]=formals, [nformals..nsave-1]=locals.                                                                                                                      */
/* Offset of member k: AB_OFF_SAVE0 - k*16  (grows downward; member 0 is nearest to the meta block).                                                                                                 */
#define AB_OFF_SAVE0    (-0x40)   /* type-word of save-set member 0 (fname); value-word at AB_OFF_SAVE0+8 */
#define AB_META_BYTES   48        /* 6 meta fields × 8B */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* LEGACY WIRE QUAD OFFSETS (rt_flat_wires_t layout, for the AB-2 dual-arm that reads either protocol):                                                                                               */
/* rt_flat_wires_t: +0=γ +8=ω +16=rsp +24=rbp — matches AB_OFF_GW/WW/ERSP and one extra rbp field.                                                                                                  */
/* AB-2 NOTE: the RBP field in rt_flat_wires_t (+24) has no AB_OFF_ constant because the AB block                                                                                                    */
/* recovers rbp via LEAVE (mov rsp,rbp / pop rbp), not by reading a stored value.                                                                                                                    */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* FRAME SIZE CALCULATION:                                                                                                                                                                            */
/* total = AB_META_BYTES + nsave*16;  K = ((total + 8 + 15) & ~15) - 8;  (16-align ACROSS the pushed rbp)                                                                                            */
static inline long ab_frame_k(long nsave) { long t = AB_META_BYTES + nsave * 16L; return ((t + 8 + 15) & ~15L) - 8; }
/* RBP-relative offset of save-set member k type-word (value-word = +8): */
static inline int ab_save_off(long nsave, int k) { return AB_OFF_SAVE0 - (int)(16L * (long)k); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Δ/Ω VERDICT (AB-0, 2026-08-09): OMIT Δ/Ω from the ACTIVATION FRAME — safe-by-construction.                                                                                                       */
/* open_slim saves only Σ/Σlen (rt.c:1233); driver_call.c:387 saves all four.  The difference is safe                                                                                                 */
/* because: a pattern match within the function body runs its OWN MATCH_BEGIN which establishes fresh                                                                                                  */
/* Σ/Δ/Ω and MATCH_END restores them; a mid-match deferred *F() call (ab_defer_call witness) nests                                                                                                    */
/* under the MATCH FRAME's own save/restore, not the proc frame.  No Δ/Ω save field in the AB frame.                                                                                                 */
/* Frame therefore saves: Σ + Σlen (AB-2 fields, in the AB_OFF_ANCHOR slot pair — AB-2 decides exact                                                                                                  */
/* placement when the full field set is wired).                                                                                                                                                       */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* <ARGREG> CHOICE: r9                                                                                                                                                                                */
/* The call site (AB-3) does: lea r9,[rsp+argbase]; the block reads [r9+k*16].                                                                                                                        */
/* AVOIDANCE LIST: r12=CAS-top · r13=Σ · r14=δ · r15=Δ (scanner triad) · rcx=γ wire · rdx=ω wire                                                                                                    */
/*   · r8=RTCC RC-5 (&ANCHOR cache, RTCC_SLOT_R8) — all must remain live across α.                                                                                                                   */
/* r9 is not yet claimed by any RTCC rung (RC-6+ must coordinate here; this comment is the cross-ref).                                                                                                */
/* RTCC-COORD: if a future RTCC rung claims r9, AB-3's <argreg> must be updated IN THIS HEADER FIRST.                                                                                                 */
#define AB_ARGREG "r9"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* TYPE-CODE REGISTER for the β 3-way dispatch (RETURN / NRETURN / FRETURN):                                                                                                                          */
/* r15b (low byte of r15) — set by the floater before jumping through the ACT-ANCHOR.                                                                                                                 */
/* 0 = RETURN (γ exit, result in rax:rdx pre-restore).                                                                                                                                                */
/* 1 = NRETURN (γ exit, result = NAME descr in rax:rdx, deref/assign is CALLER-SIDE POST-RESTORE).                                                                                                    */
/* 2 = FRETURN (ω exit, no result).                                                                                                                                                                   */
/* r15 = Δ (scan-end cursor) in the match family; floaters save/restore it around the type-code set                                                                                                   */
/* — frozen at AB-2 when the floaters are wired.  Placeholder here for AB-1 bomb-stub.                                                                                                                */
#define AB_TYPECODE_REG "r15b"
#define AB_TC_RETURN    0
#define AB_TC_NRETURN   1
#define AB_TC_FRETURN   2
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* AB_ABI_H */
