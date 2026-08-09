/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ab_abi.h — LADDER AB: ACTIVATION BLOCK ABI contract (frozen at AB-1/AB-2, 2026-08-09).                                                                                                            */
/* ONE AUTHORITY: this header is the only place AB frame offsets, the <argreg> choice, and the Δ/Ω verdict are spelled.  All templates and runtime consumers include this file.                       */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef AB_ABI_H
#define AB_ABI_H
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ACTIVATION FRAME LAYOUT (RBP-relative; all offsets NEGATIVE — cells live below the saved-rbp pushed by push rbp).                                                                                  */
/* Established by α: push rbp; mov rbp,rsp; sub rsp,K.  Released by β: mov rsp,rbp; pop rbp (or LEAVE).                                                                                              */
/* The pushed rbp occupies [rbp-8]; frame cells start at [rbp-0x10].                                                                                                                                  */
/*                                                                                                                                                                                                    */
/* Meta fields (12 × 8B = 96B; AB_META_BYTES) — AB-2 fills every field:                                                                                                                              */
#define AB_OFF_GW       (-0x10)   /* γ wire  — caller's success continuation                                                                                                                          */
#define AB_OFF_WW       (-0x18)   /* ω wire  — caller's failure continuation                                                                                                                          */
#define AB_OFF_ERSP     (-0x20)   /* entry rsp at α (caller's RSP before α's own sub-rsp)                                                                                                            */
#define AB_OFF_ANCHOR   (-0x28)   /* prev ACT-ANCHOR value (linked list of active frames)                                                                                                             */
#define AB_OFF_WN       (-0x30)   /* rt_g_want_name snapshot (caller wants NAME vs VALUE for NRETURN)                                                                                                 */
#define AB_OFF_VTMARK   (-0x38)   /* value-trail mark (GC correctness: tidy dead window at β)                                                                                                        */
#define AB_OFF_BADDR    (-0x40)   /* β address of THIS block (shared floater reads it via anchor chain)                                                                                               */
#define AB_OFF_SIGMA    (-0x48)   /* Σ (subject base pointer) — mid-match call must not lose enclosing subject                                                                                        */
#define AB_OFF_SIGMALEN (-0x50)   /* Σlen (subject length)                                                                                                                                           */
#define AB_OFF_RES0     (-0x58)   /* result stash low-word (type .v + .slen) — saved across monitor tap in β                                                                                         */
#define AB_OFF_RES1     (-0x60)   /* result stash high-word (.i / .p / .s) — saved across monitor tap in β                                                                                           */
#define AB_OFF_PAD      (-0x68)   /* padding to 16-align save-set base (8B reserved)                                                                                                                  */
/* Save-set: nsave × 16B DESCR cells, starting immediately below the meta block.                                                                                                                      */
/* Member order: [0]=fname, [1..nformals-1]=formals, [nformals..nsave-1]=locals.                                                                                                                      */
/* Offset of member k: AB_OFF_SAVE0 - k*16  (grows downward; member 0 is nearest to the meta block).                                                                                                 */
#define AB_OFF_SAVE0    (-0x70)   /* type-word of save-set member 0 (fname); value-word at AB_OFF_SAVE0+8 */
#define AB_META_BYTES   96        /* 12 meta fields × 8B (AB_OFF_PAD fills the 12th; SAVE0 at -0x70) */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* LEGACY WIRE QUAD OFFSETS (rt_flat_wires_t layout, for the AB-2 dual-arm that reads either protocol).                                                                                               */
/* rt_flat_wires_t: +0=γ +8=ω +16=rsp +24=rbp — same as AB_OFF_GW/WW/ERSP + an extra rbp field.                                                                                                     */
/* The AB block recovers rbp via LEAVE (mov rsp,rbp / pop rbp), not by reading a stored value.                                                                                                        */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* FRAME SIZE CALCULATION:                                                                                                                                                                             */
/* total = AB_META_BYTES + nsave*16;  K = ((total + 8 + 15) & ~15) - 8;  (16-align ACROSS the pushed rbp)                                                                                            */
static inline long ab_frame_k(long nsave) { long t = AB_META_BYTES + nsave * 16L; return ((t + 8 + 15) & ~15L) - 8; }
/* RBP-relative offset of save-set member k type-word (value-word = +8): */
static inline int ab_save_off(long nsave, int k) { (void)nsave; return AB_OFF_SAVE0 - (int)(16L * (long)k); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Δ/Ω VERDICT (AB-0, 2026-08-09): OMIT Δ/Ω from the ACTIVATION FRAME — safe-by-construction.                                                                                                       */
/* open_slim saves only Σ/Σlen; driver_call.c:387 saves all four.  The difference is safe:                                                                                                            */
/* a pattern match within the function body runs its OWN MATCH_BEGIN which establishes fresh                                                                                                           */
/* Σ/Δ/Ω and MATCH_END restores them; a mid-match deferred *F() call nests under the MATCH FRAME.                                                                                                     */
/* Σ/Σlen ARE saved (AB_OFF_SIGMA/SIGMALEN) since a mid-match call to a DEFINE'd function must                                                                                                        */
/* preserve the enclosing match's subject for when it returns.                                                                                                                                        */
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
/* cl (low byte of rcx) — set by the floater before jumping through the ACT-ANCHOR.                                                                                                                   */
/* rcx is dead at floater time (γ-wire was consumed at call-site lea; cl is scratch at the floater label).                                                                                            */
/* 0 = RETURN (γ exit, result in rax:rdx pre-restore).                                                                                                                                                */
/* 1 = NRETURN (γ exit, result = NAME descr in rax:rdx; deref/assign CALLER-SIDE POST-RESTORE).                                                                                                       */
/* 2 = FRETURN (ω exit, no result).                                                                                                                                                                   */
/* AB-1 placeholder was r15b — CHANGED at AB-2: r15=Δ is live on the mid-match deferred *F() path                                                                                                    */
/* and must stay intact through β (the nret deref is caller-side; Δ may still be live above us).                                                                                                      */
#define AB_TYPECODE_REG "cl"
#define AB_TC_RETURN    0
#define AB_TC_NRETURN   1
#define AB_TC_FRETURN   2
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ACT-ANCHOR: one process-global 8B cell on the pin page, holds rbp of the innermost active frame                                                                                                    */
/* (or NULL when no DEFINE'd function is active). Linked list: each α links prev anchor into the                                                                                                      */
/* frame at AB_OFF_ANCHOR; β unlinks. Address: RT_AB_ANCHOR (defined in pin_va.h).                                                                                                                    */
/* NRET DISCRIMINATOR CELL: RT_AB_NRET (8B, pin page) — written by SNO$NRET / NRETURN-body before                                                                                                    */
/* jumping to the RETURN floater; read and cleared by role-1 floater on both arms.                                                                                                                     */
/* Both cells sit in .bss-equivalent mmap: zero-initialised, RW, reachable by disp32 from anywhere. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* AB_ABI_H */
