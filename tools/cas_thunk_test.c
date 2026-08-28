/* cas_thunk_test.c -- unit witness for the C.A.S. EXPRESSION-THUNK entry kind (row lang-lambda-pattern-primitives).
   ⭐ UNIT AND NOT LANGUAGE-LEVEL ON PURPOSE: the lambda SURFACE is on hold while Lon redesigns it (ceo 2026-08-28),
   so this drives the machinery through the runtime's own exported entry points instead of through a syntax that
   may still change. It proves the three properties Lon named -- push at cursor-pass, discard on backtrack, run at
   success -- for the new entry kind, and it keeps proving them whatever the surface turns out to be. */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
typedef struct { const char *varname; uint64_t saved_delta; uint64_t len; } rt_dcap_e;
#define RT_DCAP_E_THUNK  (~(uint64_t)0)
extern void rt_dcap_lazy_init(void);
extern const char *g_dcap_base;
extern long c_rt_dcap_end_ok_open(const char *mark, const char *top, const char *subj);
extern void rt_dcap_end_ok_close(void);
static int g_ran, g_order[8], g_norder;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void thunk_a(void) { g_ran++; if (g_norder < 8) g_order[g_norder++] = 1; }
static void thunk_b(void) { g_ran++; if (g_norder < 8) g_order[g_norder++] = 2; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static rt_dcap_e *slot(int i) { return (rt_dcap_e *)(void *)((char *)(void *)(uintptr_t)g_dcap_base + (size_t)i * sizeof(rt_dcap_e)); }
static void push_thunk(int i, void (*fn)(void), const char *nm) { rt_dcap_e *e = slot(i); e->varname = nm; e->saved_delta = (uint64_t)(uintptr_t)fn; e->len = RT_DCAP_E_THUNK; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int fails;
static void check(const char *what, int got, int want) { if (got != want) { fails++; printf("  FAIL %-44s got=%d want=%d\n", what, got, want); } else printf("  ok   %-44s %d\n", what, got); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int main(void) {
    rt_dcap_lazy_init();
    if (!g_dcap_base) { printf("REFUSING: dcap island did not initialise -- cannot measure\n"); return 2; }
    const char *base = (const char *)(uintptr_t)g_dcap_base;
    /* 1. PUMP AT SUCCESS: one queued thunk runs exactly once. */
    g_ran = 0; g_norder = 0;
    push_thunk(0, thunk_a, "t_a");
    c_rt_dcap_end_ok_open(base, base + sizeof(rt_dcap_e), "");
    rt_dcap_end_ok_close();
    check("one queued thunk runs at success", g_ran, 1);
    /* 2. DISCARD ON BACKTRACK: an entry whose push was undone (top rewound to mark, the `sub r12,24` the
          capture template emits) is never seen by the pump.
       ⛔ THIS CASE CANNOT DISCRIMINATE ON ITS OWN AND MUST NEVER BE THE ONLY ONE LEFT.  Measured by
       deliberately breaking the pump so no thunk runs at all: cases 1 and 3 went red, and THIS ONE STAYED
       GREEN -- "nothing ran" is trivially true when nothing can run.  Cases 1 and 3 carry the whole
       discriminating power; delete them and this file becomes a gate that passes on a dead pump. */
    g_ran = 0;
    push_thunk(0, thunk_a, "t_a");
    c_rt_dcap_end_ok_open(base, base, "");          /* top == mark: the entry was popped */
    rt_dcap_end_ok_close();
    check("backtracked thunk does NOT run", g_ran, 0);
    /* 3. ORDER: queued thunks run in queue order, not reverse. */
    g_ran = 0; g_norder = 0;
    push_thunk(0, thunk_a, "t_a");
    push_thunk(1, thunk_b, "t_b");
    c_rt_dcap_end_ok_open(base, base + 2 * sizeof(rt_dcap_e), "");
    rt_dcap_end_ok_close();
    check("two thunks both run", g_ran, 2);
    check("first queued ran first", g_norder >= 1 ? g_order[0] : -1, 1);
    check("second queued ran second", g_norder >= 2 ? g_order[1] : -1, 2);
    /* 4. THE GUARD-ORDER PROPERTY. A thunk's saved_delta is a CODE ADDRESS -- a number vastly larger than any
       subject length -- so if the capture-span guard were tested BEFORE the thunk arm it would refuse every
       thunk with a corruption diagnostic on stderr naming an overrun that never happened. Cases 1-3 passing at
       all IS that proof: the entries carry real function addresses and were executed rather than refused. */
    printf("%s: %d failure(s)\n", fails ? "CAS-THUNK FAIL" : "CAS-THUNK OK", fails);
    return fails ? 1 : 0;
}
