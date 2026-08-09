/* rtx_icncall.S -- ICON-RTX: the Icon ICNCALL family. Procedure-value minting.
 *
 * CONTRACT: .github/ARCH-ICON-RTX.md. Ladder: .github/GOAL-ICON-RTX.md. Ledger: .github/RTX-CLAIMS.md.
 * Gate: SCRIP_RTX_ICNCALL (twelfth family gate -- a ledger event, recorded in RTX-CLAIMS.md).
 *
 * WHY THIS ONE: Lon's directive is COMPLETENESS, small body first. rt_proc_value is a 3-line body and
 * the simplest possible shape in the whole runtime -- it touches NO memory, NO global, and makes NO
 * call. It assembles a two-eightbyte constant and returns it:
 *     DESCR_t rt_proc_value(const char *name)
 *     { DESCR_t d; d.v = DT_E; d.slen = 0xFFFFFFFEu; d.s = (char *)name; return d; }
 * All 16 bytes are written by those three stores (v@0, slen@4, s@8), so there is no uninitialised
 * tail to reproduce and the asm sets both eightbytes outright.
 *
 * STEP 0, MEASURED s214-ICN (re-run, never inherited -- s212's law):
 *   (a) live definition: by_name_dispatch.c:874.
 *   (b) ASCII spelling, no Greek codepoint hazard.
 *   (c) NO globals touched at all => step 0(c) is vacuous here, which is the whole appeal.
 *   (d) ⚠ ARRIVES 4 TIMES AND IS **FLAT**: 4 at N=50 and 4 at N=200 over a procedure-value call loop.
 *       This is the SETUP-ONLY SIGNATURE THAT TRAPPED rt_call_arr (s188: 8 calls flat N=1 -> N=64), and
 *       under the old speed-ranked ladder it would be a REJECT. It lands here ONLY because the standing
 *       directive is completeness. ⛔ NO SPEED CLAIM IS MADE. 4 arrivals is, however, non-zero, so the
 *       symbol IS falsifiable, which is the property that actually gates a port.
 *   (e) not already asm (UTF-8-aware sweep of every .S).
 *   (f) 285 `call rt_proc_value@PLT` sites in a COMPILER sweep of 316 Icon programs (RULES.md: sweep
 *       the compiler, never the frozen corpus artifacts -- s213).
 *
 * CANONICAL SEMANTICS (ARCH-ICON-RTX §8 step 1 -- read BEFORE porting):
 *   refs/icon-master/src/h/rmacros.h:326   D_Proc = (T_Proc | D_Typecode | F_Ptr)
 *   refs/icon-master/src/h/rstructs.h:88   struct b_proc { word title (== T_Proc); ... }
 *   refs/jcon-master/jcon/vProc.java:14    vProc { vString img; int args; } Type() == "procedure"
 * So a procedure value is canonically a TYPED POINTER DESCRIPTOR: a tag word plus a pointer to the
 * block that carries the name and arity. SCRIP's DT_E descriptor is that same (tag, pointer) shape;
 * what differs is that SCRIP carries the NAME and defers resolution, and the slen field is the
 * discriminator that says so.
 *
 * ⛔⛔ THE SENTINEL IS IDENTITY, NOT PADDING. slen == 0xFFFFFFFE is what every consumer tests to decide
 * "this DT_E carries a procedure name in .s": by_name_dispatch.c:880 (procval_name), :5173, :5457
 * (_is_self_default), :5595, :5657. A wrong sentinel does not crash -- it silently changes procedure
 * IDENTITY, which is the same failure class as the cset bug s213 introduced and fixed in rt_size_d.
 * It is spelled here as an expression, not a magic literal, so it reads the same as the C.
 */
#include "rtx_abi.inc"

#define DT_E            0x38        /* src/contracts/descr.h DT_E = 0x38 (s229 renumber); old layout had 11 -- stale copies caught this session */
#define PROCVAL_SLEN    0xFFFFFFFE

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RTX_GATE_DEF(icncall)

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* rt_proc_value(name) -- mint the deferred procedure value. rdi = name; return pair rax:rdx where
 * rax = the {v, slen} eightbyte and rdx = the value union (the name pointer, unchanged). */
RTX_FUNC(rt_proc_value)
    RTX_GATE(icncall, c_rt_proc_value)
    mov     rax, (PROCVAL_SLEN << 32) | DT_E
    mov     rdx, rdi
    ret
RTX_ENDF(rt_proc_value)

.section .note.GNU-stack,"",@progbits
