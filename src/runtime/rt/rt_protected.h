/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef RT_PROTECTED_H
#define RT_PROTECTED_H
#ifdef __cplusplus
extern "C" {
#endif
/* ⭐ THE LEAD-CHARACTER SET, DEFINED ONCE (hq_P s262).  is_protected_pat_name is a cross-TU call that costs ~29 Ir
   (PLT hop + prologue + switch) to answer "no", and NV_SET_fn asks it on EVERY store -- 26,400 times in roman.sno,
   775,000 Ir = 1.87% of the program, for a variable named `T`.  The function's own switch already rejects on name[0],
   so a caller may pre-filter on the same character and cannot change the answer -- the s260 FAIL-strcmp cure's shape.
   ⛔ THE SET LIVES HERE, NOT IN THE CALLER, AND is_protected_pat_name ITSELF USES IT.  A duplicated letter list in a
   hot caller is exactly the per-op filter RULES.md forbids: add a protected name starting with 'P' and the guard
   would silently refuse it.  With one definition consulted by both, such a name is rejected by the FUNCTION too --
   the failure is loud and local instead of silent and remote, and the gate below pins the two together. */
/* ⛔⛔ always_inline IS LOad-BEARING AT -O0, NOT DECORATION.  Under the s262 NO-`-O2` fact rule -O0 is the number of record, and at -O0 gcc honours neither `inline` nor its own cost model: a plain `static inline` helper is a REAL CALL with a real prologue.  MEASURED on roman.sno the day this guard landed -- is_protected_pat_lead showed up in callgrind as `is_protected_pat_lead'NV_SET_fn (49,600x)` costing 843,200 Ir = 0.80% of the whole program, i.e. the pre-filter that was supposed to REMOVE a call had merely swapped a PLT call for a local one.  always_inline is honoured at -O0 and is the only way to get inlining back without the optimiser we are no longer allowed to build.  ⭐ THE GENERAL RULE THIS STANDS FOR: in a hot -O0 runtime leaf, `static inline` is a promise the compiler does not keep -- say always_inline or write the field reads out by hand. */
static inline __attribute__((always_inline)) int is_protected_pat_lead(char c) { return c == 'A' || c == 'B' || c == 'F' || c == 'R' || c == 'S'; }
int is_protected_pat_name(const char *name);
int protected_pat_name_to_sm_op(const char *name);
#ifdef __cplusplus
}
#endif
#endif
