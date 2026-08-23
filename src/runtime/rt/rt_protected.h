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
static inline int is_protected_pat_lead(char c) { return c == 'A' || c == 'B' || c == 'F' || c == 'R' || c == 'S'; }
int is_protected_pat_name(const char *name);
int protected_pat_name_to_sm_op(const char *name);
#ifdef __cplusplus
}
#endif
#endif
