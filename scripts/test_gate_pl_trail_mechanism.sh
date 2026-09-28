#!/usr/bin/env bash
# test_gate_pl_trail_mechanism.sh -- THE TRAIL (ARCH-PROLOG-BB-REWRITE.md section 5; rung 1 hq_C 2026-09-02, re-cut at R2 hq_prolog
# 2026-09-28): r12 is TR, the conditional-binding log in Prolog's own rt_slab_region arena; every binder reachable from a Prolog graph
# enters through an rtx gate (rtx_plunify.s) that hands {r12, r13} to the C body as a pl_tr_ctx_t. This gate exercises the mechanism
# DIRECTLY through the exported C bodies, in a C harness compiled here against out/libscrip_rt.so. SINCE R2 every Prolog variable
# cell lives in the collector heap (rt_pl_fresh_var_ref), a frame slot holds a value and is never bound, and a binding's entry is ONE
# WORD -- the heap cell's address -- because an unbound heap cell's old value is always reconstructible (a self-reference); the one
# backtrackable overwrite of a BOUND value, b_setval/2, pushes the three-word VALUE entry {old.lo, old.hi, addr|1} with the tagged word
# on top, and the pop and the collector's visitor read the tag bit of the top word. The frame-hi rule of rung 3 is gone with the frame
# cells it graded: until R4's HB compare, every binding made while a choice is open is logged.
#   (1) B = 0 (no choice): a binding is made and NOTHING is logged.
#   (2) B set: exactly one 8-byte entry naming the heap cell is pushed; the arena's top word (its header, read by the GC root-range
#       scan) tracks the live top.
#   (3) rt_pl_tr_unwind_to restores the cell to an unbound SELF-REFERENCE and pops the entry.
#   (4) a value entry (pl_tr_push_value) restores the bound old value on unwind, and a mixed run (word, value, word) unwinds in order.
#   (5) f(X,1) = f(2,3): the partial binding of X is undone by the failing unify itself (atomicity), tr back to its entry mark.
#   (6) the arena limit REFUSES rc=2 with a named limit -- never an abort, never a wrap into the next arena.
# EXIT: 0 every check passes · 1 a check fails · 2 UNPROVEN (no runtime library / no compiler / harness will not build).
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"; cd "$ROOT"
. "$HERE/lib_gate.sh"
gate_parse_args "$@"
RT="${RT_DIR:-$ROOT/out}"
gate_require "$RT/libscrip_rt.so" "runtime library"
gate_require "$ROOT/src/runtime/rt/rt_pl_trail.h" "trail header"
command -v gcc >/dev/null 2>&1 || { echo "GATE UNPROVEN(2) [$GATE_NAME]: no gcc to build the harness"; gate_stamp; exit 2; }
W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
cat > "$W/h.c" <<'CEOF'
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "descr.h"
#include "rt/rt_pl_trail.h"
extern DESCR_t rt_pl_dop_unify_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx);
extern DESCR_t rt_pl_dop_mkc_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx);
extern DESCR_t rt_pl_deref_val(DESCR_t v);
extern DESCR_t rt_pl_fresh_var_ref(void);
extern void core_lib_init(void);
static DESCR_t ival(long long i) { DESCR_t d; memset(&d, 0, sizeof d); d.v = DT_I; d.i = i; return d; }
static DESCR_t sval(const char *s) { DESCR_t d; memset(&d, 0, sizeof d); d.v = DT_S; d.slen = (uint32_t)strlen(s); d.s = (char *)s; return d; }
static DESCR_t *cell_of(DESCR_t ref) { return (DESCR_t *)ref.p; }
static int unbound_self(const DESCR_t *c) { return c->v == (DTYPE_t)DT_PLVAR && c->p == (const void *)c; }
static char hdr[64];
static int fails = 0;
#define CHECK(c, msg) do { if (c) printf("  ok   %s\n", msg); else { printf("  FAIL %s\n", msg); fails++; } } while (0)
int main(void) {
    core_lib_init();
    char *tr0 = (char *)rt_pl_tr_init();
    pl_tr_ctx_t cx; cx.tr = tr0; cx.b = 0;
    printf("arena base=%p tr0=%p (offset %lu) entry=%d value-entry=%d\n", (void *)pl_tr_base_of(tr0), (void *)tr0, (unsigned long)(tr0 - pl_tr_base_of(tr0)), (int)PL_TR_ENTRY_BYTES, (int)PL_TR_VALUE_ENTRY_BYTES);
    { DESCR_t x = rt_pl_fresh_var_ref(); DESCR_t a[2]; a[0] = x; a[1] = ival(5); DESCR_t r = rt_pl_dop_unify_c(a, 2, &cx);
      CHECK(r.v == DT_I && r.i == 5 && cell_of(x)->v == DT_I && cell_of(x)->i == 5, "B=0: unify binds the heap cell");
      CHECK(cx.tr == tr0, "B=0: nothing is logged (no choice)"); }
    memset(hdr, 0, sizeof hdr); cx.b = hdr;
    { DESCR_t x = rt_pl_fresh_var_ref(); DESCR_t a[2]; a[0] = x; a[1] = sval("bound"); char *before = cx.tr; DESCR_t r = rt_pl_dop_unify_c(a, 2, &cx);
      CHECK(r.v == DT_S && cell_of(x)->v == DT_S, "B set: unify binds the heap cell");
      CHECK(cx.tr == before + PL_TR_ENTRY_BYTES, "B set: exactly one 8-byte entry logged");
      CHECK(*(DESCR_t **)before == cell_of(x) && ((*(uintptr_t *)before) & 1u) == 0, "the entry is the cell's address, bit 0 clear");
      CHECK(*(char **)pl_tr_base_of(cx.tr) == cx.tr, "the arena top word tracks the live top for the GC");
      cx.tr = rt_pl_tr_unwind_to(cx.tr, before);
      CHECK(cx.tr == before && unbound_self(cell_of(x)), "unwind restores the cell to an unbound self-reference and pops the entry"); }
    { DESCR_t x = rt_pl_fresh_var_ref(); DESCR_t y = rt_pl_fresh_var_ref(); *cell_of(y) = ival(41); char *before = cx.tr;
      { DESCR_t a[2]; a[0] = x; a[1] = ival(1); rt_pl_dop_unify_c(a, 2, &cx); }
      pl_tr_push_value(&cx, cell_of(y)); *cell_of(y) = ival(42);
      { DESCR_t z = rt_pl_fresh_var_ref(); DESCR_t a[2]; a[0] = z; a[1] = ival(3); rt_pl_dop_unify_c(a, 2, &cx);
        CHECK(cx.tr == before + 2 * PL_TR_ENTRY_BYTES + PL_TR_VALUE_ENTRY_BYTES, "word, value, word: three entries of 8, 24, 8 bytes");
        CHECK((*(uintptr_t *)(cx.tr - PL_TR_ENTRY_BYTES - PL_TR_VALUE_ENTRY_BYTES + 16) & 1u) == 1u, "the value entry's top word carries the tag bit");
        cx.tr = rt_pl_tr_unwind_to(cx.tr, before);
        CHECK(cx.tr == before && unbound_self(cell_of(x)) && unbound_self(cell_of(z)) && cell_of(y)->v == DT_I && cell_of(y)->i == 41, "unwind in order: x and z unbound again, y back to its old bound value 41"); } }
    { DESCR_t x = rt_pl_fresh_var_ref(); DESCR_t m1[3]; m1[0] = sval("f"); m1[1] = x; m1[2] = ival(1); DESCR_t t1 = rt_pl_dop_mkc_c(m1, 3, &cx);
      DESCR_t m2[3]; m2[0] = sval("f"); m2[1] = ival(2); m2[2] = ival(3); DESCR_t t2 = rt_pl_dop_mkc_c(m2, 3, &cx);
      char *before = cx.tr; DESCR_t a[2]; a[0] = t1; a[1] = t2; DESCR_t r = rt_pl_dop_unify_c(a, 2, &cx);
      CHECK(r.v == DT_FAIL, "f(X,1) = f(2,3) fails");
      CHECK(cx.tr == before, "the failing unify unwound its own suffix (atomicity)");
      { DESCR_t xv = rt_pl_deref_val(x); CHECK(xv.v != DT_I, "X is unbound again after the partial binding was undone"); } }
    fflush(stdout); fflush(stderr);
    { pid_t pid = fork(); if (pid == 0) { pl_tr_ctx_t c2 = cx; c2.tr = pl_tr_base_of(cx.tr) + PL_TR_ARENA_BYTES - PL_TR_VALUE_ENTRY_BYTES - PL_TR_ENTRY_BYTES; DESCR_t junk; memset(&junk, 0, sizeof junk); pl_tr_push(&c2, &junk); fprintf(stderr, "first push below the limit: ok\n"); pl_tr_push(&c2, &junk); _exit(99); }
      int st = 0; waitpid(pid, &st, 0); CHECK(WIFEXITED(st) && WEXITSTATUS(st) == 2, "the arena limit REFUSES with rc=2 (never aborts, never wraps)"); }
    printf("%s (%d failures)\n", fails ? "TRAIL HARNESS FAIL" : "TRAIL HARNESS PASS", fails);
    return fails ? 1 : 0;
}
CEOF
gcc -O0 -g -I "$ROOT/src/ir" -I "$ROOT/src/runtime" -o "$W/h" "$W/h.c" "$RT/libscrip_rt.so" -lm -lstdc++ -Wl,-rpath,"$RT" >"$W/build.log" 2>&1 || { echo "GATE UNPROVEN(2) [$GATE_NAME]: harness does not build"; sed 's/^/    /' "$W/build.log" | head -20; gate_stamp; exit 2; }
"$W/h" > "$W/out.txt" 2>"$W/err.txt"; rc=$?
grep -E '^  (ok|FAIL) ' "$W/out.txt" | sort -u
n_ok=$(grep -c '^  ok ' "$W/out.txt"); n_fail=$(grep -c '^  FAIL ' "$W/out.txt")
GATE_EXAMINED="$((n_ok + n_fail)) checks (harness rc=$rc)"
gate_verdict "$n_fail" "trail-mechanism checks failing"
