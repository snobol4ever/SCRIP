#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ZB-FC-3a (ARCH-ZETA S13 Tier C, LINEAR-ARM v1): when LOWER registered exact per-arm footprints AND the
 * FORTH flavor is live (afc), A's own delta/dcap/alt_i quad rides an rsp cell and the S10d pad-to-max law is
 * emitted: each arm's sigma edge lands its OWN stub (PAIR 2N+2+j) which subs (FPMAX - fp_j) so every arm
 * yields at the uniform padded depth; A.beta then reads alt_i at [rsp + FPMAX + 8] (the live arm's padded
 * cells sit BELOW the read site; a C call at beta pushes below them, safe) and each resume-dispatch branch
 * adds its arm's pad back so the arm's own beta re-enters at ITS true frontier -- the SEQ transit cascade
 * then pops pure LIFO.  na_f runs at the frontier (arm fully popped before phi), so its accesses auto-rebase
 * through the ordinary fc window; the single unconditional omega carries the one hook pop.  !afc = the
 * pre-rung template verbatim. */
static inline int  afc()      { return x86_fc_on() && _.op_fc_fpmax >= 0; }
static inline int  fpad(int j){ return (int)(_.op_fc_fpmax - (long)_.op_fc_arm_fp[j]); }
static inline const char * rspd(int off) { static char b[8][40]; static int i; i = (i + 1) & 7; snprintf(b[i], 40, "dword ptr [rsp + %d]", off); return b[i]; }
static std::string alt_dispatch_chain(long N, int base, int lo) { std::string r; for (long i = lo; i < N; i++) r += x86("cmp", "eax", (int)i) + x86("je", PAIR((int)(base + i))); return r; }
static std::string alt_resume_chain_fc(long N) {
    std::string r;
    for (long i = 0; i < N - 1; i++)
        r += x86("cmp", "eax", (int)i)
           + (fpad((int)i) > 0 ? (x86("jne", L((int)(100 + i))) + x86("add", "rsp", (long)fpad((int)i)) + x86("jmp", PAIR((int)(N + i))) + x86("def", L((int)(100 + i))))
                               : x86("je", PAIR((int)(N + i))));
    r += (fpad((int)(N - 1)) > 0 ? x86("add", "rsp", (long)fpad((int)(N - 1))) : std::string())
       + x86("jmp", PAIR((int)(N + N - 1)));
    return r;
}
static std::string alt_pad_stubs(long N) {
    std::string r;
    for (long j = 0; j < N; j++)
        r += x86("def", PAIR((int)(2 * N + 2 + j)))
           + (fpad((int)j) > 0 ? x86("sub", "rsp", (long)fpad((int)j)) : std::string())
           + x86("jmp", PAIR((int)(2 * N)));
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_alternate() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    return _.op_off < 0
             ? x86_alpha() + x86_bomb("IR_MATCH_ALTERNATE: cursor slot not granted (zls)")
             : x86("comment", "IR_MATCH_ALT_NARY")
             + x86_alpha()
             /* rbp-dcap (s46): ALTERNATE TOUCHES THE PEND STACK NOWHERE.  The old rt_dcap_height (α) /
              * rt_dcap_restore_to (switch) C-call windows are DELETED OUTRIGHT — not ported to movs.
              * WHY (Lon's theorem, snobol4python/_backend_pure.py): Π.γ is `for P in self.AP: yield from
              * P.γ()` — it has NO cstack handling at all, and neither does Σ or ARBNO; the entire assignment
              * stack is isolated to the capture leaves (Δ/θ/λ), each a balanced append→yield→pop inside its
              * OWN generator.  Generator scoping IS the LIFO discipline: an alternative cannot fail until
              * every interior generator has been resumed to exhaustion, and each interior capture runs its
              * own pop on that resume path BEFORE its exhaustion is visible here.  So rbp is ALREADY at
              * entry height whenever a switch is taken — there is no height to save because there is no
              * moment at which it could be wrong.  MEASURED, not assumed (the SZ-2c transit-gap worry was
              * the reason to doubt it): with both instructions deleted, crosscheck is watermark-exact in
              * BOTH flavors (default CSTACK and SCRIP_ZETA_PORT=6), smokes 7/7, and the two tests named for
              * exactly this hazard — 156_pat_cap_alt_abandon_pop and 160_pat_alt_inner_gen_resume — pass.
              * Quad is now: +0 δ, +4 alt_i, +8 dead pad. */
             + x86("mov", FR(_.op_off), "r14d")
             + x86("mov", FR(_.op_off + 4), 0)
             + x86("jmp", PAIR(0))
             + IF(afc(), alt_pad_stubs(_.op_ival))
             + x86("def", PAIR((int)(2 * _.op_ival)))
             + x86_gamma()
             + x86_beta()
             + IF(afc(),  x86("mov", "eax", rspd((int)_.op_fc_fpmax + 4)))
             + IF(!afc(), x86("mov", "eax", FR(_.op_off + 4)))
             + IF(afc(),  alt_resume_chain_fc(_.op_ival))
             + IF(!afc(), alt_dispatch_chain(_.op_ival - 1, (int)_.op_ival, 0) + x86("jmp", PAIR((int)(_.op_ival + _.op_ival - 1))))
             + x86("def", PAIR((int)(2 * _.op_ival + 1)))
             + x86("add", FR(_.op_off + 4), 1)
             + x86("mov", "r14d", FR(_.op_off))
             + x86("mov", "eax", FR(_.op_off + 4))
             + alt_dispatch_chain(_.op_ival, 0, 1)
             + x86_omega();
}
