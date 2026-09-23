#!/usr/bin/env bash
# test_gate_suite_runners_honour_the_tests_declared_memory.sh -- LON'S WORD OF 2026-09-23, MEASURED.
#
# ⛔⭐ THE ROW.  Lon, in-chat to the ceo, verbatim: "Well, each test should have the needed amount of memory in the
# attribute TSV/CSV files." (routed CEO-1167).  Before that word a program whose live set did not fit the shipped
# window was a capacity red and an argument for raising the default for EVERYBODY.  After it, the program DECLARES
# what it needs in its suite's ALL.csv heap_kb column, and the runner exports SCRIP_HEAP_KB FOR THAT PROGRAM ONLY.
#
# ⛔⭐⭐ WHAT THIS GATE REFUSES TO TAKE ON TRUST, AND IT IS THE WHOLE REASON IT EXISTS.  "The runner reads the
# column" is cheap to assert and cheap to believe.  The failure that matters is the SILENT one: a reader that
# returns the default for every entry still produces a green board, because an undeclared program and a program
# whose declaration was dropped run identically.  So arm B does not ask whether the reader ran -- it plants a
# program that CANNOT COMPLETE at the shipped default and CAN at its declared arena, and requires the verdicts to
# differ.  A reader that quietly does nothing fails that arm by construction.  Arm C is its negative control: the
# NEIGHBOUR entry in the same csv, same runner, same loop, must still see the shipped default -- because a reader
# that exports one program's arena over the whole run has not honoured a declaration, it has pinned an arena, which
# is precisely the thing a declaration replaces (util_arena_pin_census.py, CEO-1146).
#
# ⛔⭐ ARM A IS A DRIFT ALARM ON A COPIED CONSTANT.  lib_declared_arena.sh and corpus_suite_harness.py each carry a
# copy of gc_heap.c's GC_HEAP_CAP_KB / GC_HEAP_KB_FLOOR.  A copy of a runtime constant that nothing checks is the
# false-label class this column exists to close, so the #defines are re-read from source here and compared.
#
# ⛔⭐⭐ ARM D IS THE MEASUREMENT THAT MADE THIS COLUMN HONEST, AND IT IS NOT A STYLE CHECK.  gc_heap.c reads
#     cap_kb = (long)GC_HEAP_CAP_KB; if (cap_kb < kb) cap_kb = kb;
# so SCRIP_HEAP_KB moves the INITIAL WINDOW always and the HARD CAP only once it climbs past 4096.  Measured by the
# coo 2026-09-23 on one binary and one 60000-element Icon witness: 128, 2048 and 4096 all die rc=134 at a cap the
# runtime itself prints as 4096 KB; 4097 moves it to 4100; 8192 completes rc=0.  A cell reading 2048 therefore
# declares NOTHING ABOUT CAPACITY while reading to every consumer exactly like a capacity declaration -- and the
# baton's own guardrail (matched at that arena, NOT matched one step below) is UNPROVABLE for it, because one step
# below carries the same cap and both arms read the same.  Arm D requires that cell to be REFUSED rc=2, not
# silently accepted as a no-op.  Arm D2 requires the runtime to still behave that way, so the day the cap moves
# this gate says so instead of continuing to enforce a rule the runtime no longer has.
#
# ⛔ ARM E IS CEO-1127's EVIDENCE BAR.  Every one of the ten seats grades ENTRIES EXTRACTED STANDALONE.  An
# extracted pair has no sibling ALL.csv, so a declaration that did not travel would mean a program needing 16384 KB
# runs at the shipped 128 in EXACTLY the workflow the fleet cures in, while passing on the board it is published
# from -- two arms disagreeing for a reason neither names.
#
# FAIL_ONCE=1 restores the pre-column world -- the shell reader returns nothing for everybody AND the travelling
# sidecar is not written -- and MEASURED 2026-09-23 it reds exactly five arms: B, C3, D1, D1b, E2.  The other eight
# are green BY CONSTRUCTION under the mutant and are named here so nobody reads their green as coverage: A1..A3 and
# D2 read the runtime, F reads the shipped tree, E1 names a path, and C1/C2 describe the reader-off world itself.
# An arm that has never been seen to fail is an arm that reads "there was never a bug here" (this gate's own law,
# COO-152), and a fail-once that reds for a STAGING reason while claiming to have proved the property is the shape
# COO-153 found three of inside the mechanism built to disprove exactly that -- so the five are listed, not counted.
"$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/util_require_fresh.sh" --gate "$(basename "${BASH_SOURCE[0]}" .sh)" || exit $?
set -u
cd "$(dirname "$0")/.." || exit 2
ROOT="$PWD"; HERE="$ROOT/scripts"; G="suite_runners_honour_the_tests_declared_memory"
checks=0; fails=0
ck() { checks=$((checks+1)); if [ "$1" = ok ]; then echo "  ok   $2"; else fails=$((fails+1)); echo "  FAIL $2"; fi; }
refuse() { echo "⛔ GATE REFUSED(2) [$G]: $1"; exit 2; }
[ -x "$ROOT/scrip" ] || refuse "no ./scrip -- this gate runs real programs at two arenas and cannot measure without one"
[ -f "$ROOT/out/libscrip_rt.so" ] || refuse "no out/libscrip_rt.so"
[ -f "$HERE/lib_declared_arena.sh" ] || refuse "lib_declared_arena.sh is missing -- the runners' one reader is not on this tree, so there is nothing to hold to the contract"
command -v python3 >/dev/null 2>&1 || refuse "no python3 -- the harness reader cannot be exercised"
. "$HERE/lib_declared_arena.sh"
T="$(mktemp -d)"; trap 'rm -rf "$T"' EXIT
ulimit -c 0 2>/dev/null || true
DECL_KB=16384
echo "GATE $G -- declared arena ${DECL_KB} KB vs the shipped default; fixture in $T (planted, NOT a corpus suite: a gate's own mktemp fixture outside the corpus tree is not a board, CEO-547)"

# ── arm A: the copied constants still match gc_heap.c ──────────────────────────────────────────────────────────
SRC="$ROOT/src/runtime/rt/gc_heap.c"
[ -f "$SRC" ] || refuse "no $SRC to read the constants back out of"
rt_cap=$(sed -n 's/^#define[[:space:]]\+GC_HEAP_CAP_KB[[:space:]]\+\([0-9]\+\).*/\1/p' "$SRC" | head -1)
rt_flr=$(sed -n 's/^#define[[:space:]]\+GC_HEAP_KB_FLOOR[[:space:]]\+\([0-9]\+\).*/\1/p' "$SRC" | head -1)
[ -n "$rt_cap" ] && [ -n "$rt_flr" ] || refuse "could not read GC_HEAP_CAP_KB / GC_HEAP_KB_FLOOR out of $SRC -- the drift alarm cannot be armed, and an unarmed drift alarm is worse than none"
py_cap=$(python3 -c "import sys;sys.path.insert(0,'$HERE');exec(open('$HERE/corpus_suite_harness.py').read().split('def heap_sidecar_path')[0].split('# ====================================================== declared arena ===')[1]);print(GC_HEAP_CAP_KB)" 2>/dev/null || echo "")
[ "$DECLARED_ARENA_CAP_KB" = "$rt_cap" ] && ck ok "A1 lib_declared_arena.sh cap ($DECLARED_ARENA_CAP_KB) == gc_heap.c GC_HEAP_CAP_KB ($rt_cap)" \
  || ck bad "A1 lib_declared_arena.sh cap=$DECLARED_ARENA_CAP_KB but gc_heap.c says $rt_cap -- a copied runtime constant drifted, which is the false-label class this column exists to close"
grep -q "^GC_HEAP_CAP_KB = $rt_cap\b" "$HERE/corpus_suite_harness.py" && ck ok "A2 corpus_suite_harness.py GC_HEAP_CAP_KB == gc_heap.c ($rt_cap)" \
  || ck bad "A2 corpus_suite_harness.py GC_HEAP_CAP_KB disagrees with gc_heap.c ($rt_cap)"
grep -q "^GC_HEAP_KB_FLOOR = $rt_flr\b" "$HERE/corpus_suite_harness.py" && ck ok "A3 corpus_suite_harness.py GC_HEAP_KB_FLOOR == gc_heap.c ($rt_flr)" \
  || ck bad "A3 corpus_suite_harness.py GC_HEAP_KB_FLOOR disagrees with gc_heap.c ($rt_flr)"

# ── the fixture: a program that does NOT fit the shipped cap, and a neighbour that does ────────────────────────
mkdir -p "$T/pkg"
# ⛔⭐⭐ FEW BIG BLOCKS, NOT MANY SMALL ONES, AND THE FIRST CUT OF THIS GATE GOT IT WRONG IN A WAY THAT COST
# A `done` RUN. The first fixture was 60000 small strings. MEASURED: at the shipped default that does not
# abort quickly -- it THRASHES, because a 128 KB window growing toward the 4096 KB cap collects on almost
# every allocation, and the cost grows superlinearly with the count (8000 entries 1.4s, 15000 4.6s, 25000
# 17.3s, all rc=0; only at 60000 does it finally abort, after ~90s). Four such runs under load 28 blew the
# criterion's own `timeout 300` and the gate returned rc=124. ⛔ AND THE REAL DEFECT WAS NOT THE DURATION:
# the premise arm tested `rc != 0`, so rc=124 (THE TIMEOUT FIRING) read as "the fixture does not fit" --
# a loaded box would have been recorded as a capacity measurement. That is the timeout-and-crash-in-one-
# branch shape the coo cured in this same seat's stack gate one tick earlier (COO-152), reappearing in a
# file written the day after. 200 blocks of 32 KB carry the same 6400 KB live set with ~200 allocations
# instead of 60000: it aborts rc=134 in ~1.5s at the default and completes rc=0 in ~75ms at 16384 KB, so
# the two verdicts are far apart, unambiguous, and cheap enough to sit in the blocking set.
# ⛔ FIX_N IS THE SINGLE SOURCE FOR BOTH THE FIXTURE AND THE STRING ARM G GRADES AGAINST. They were two
# literals for one run of this gate and arm G promptly read FAIL when the fixture was resized -- a gate
# grading its own witness against the PREVIOUS witness's output, which is a red that says nothing about
# the subject. One name, expanded into both.
FIX_N=200
cat > "$T/pkg/hungry.icn" <<EOF
procedure main()
   L := list();
   every i := 1 to $FIX_N do put(L, repl("x", 32768));
   write(*L);
end
EOF
cat > "$T/pkg/frugal.icn" <<'EOF'
procedure main()
   write("frugal");
end
EOF
printf 'rank,entry,origin,package,n_lines,stdin,want_rc,modes,heap_kb\n1,hungry,p__hungry,p,5,0,0,m3,%s\n2,frugal,p__frugal,p,3,0,0,m3,\n' "$DECL_KB" > "$T/pkg/ALL.csv"

# The premise this whole gate rests on, measured rather than assumed: hungry must genuinely NOT fit the default
# and must genuinely fit the declaration. If either stops being true the fixture has stopped being a witness and
# every arm below would be grading a tautology -- so this is a REFUSAL, never a red.
( cd "$T/pkg" && env -u SCRIP_HEAP_KB -u SCRIP_HEAP_MB -u SCRIP_HEAP_CAP_KB -u SCRIP_HEAP_MAX_MB timeout 60s "$ROOT/scrip" --run hungry.icn >/dev/null 2>&1 )
base_rc=$?
( cd "$T/pkg" && env -u SCRIP_HEAP_MB -u SCRIP_HEAP_CAP_KB -u SCRIP_HEAP_MAX_MB SCRIP_HEAP_KB="$DECL_KB" timeout 60s "$ROOT/scrip" --run hungry.icn >/dev/null 2>&1 )
decl_rc=$?
# ⛔⭐ A TIMEOUT IS NOT A CAPACITY VERDICT AND IS REFUSED HERE RATHER THAN COUNTED. rc=124 says the ceiling
# fired; it cannot tell "this program does not fit" from "this box is at load 28". Testing rc != 0 would
# accept a loaded machine as evidence of a live set, which is the false-label direction. The abort we are
# claiming is rc=134 (SIGABRT from the hard cap), and nothing else will do.
[ "$base_rc" != 124 ] || refuse "the fixture TIMED OUT at the shipped default rather than aborting -- rc=124 is the ceiling firing and cannot be told from a loaded box, so this run measured nothing about capacity (load: $(uptime | sed 's/.*load average: //'))"
[ "$decl_rc" != 124 ] || refuse "the fixture TIMED OUT at ${DECL_KB} KB -- same reason: a ceiling is not a verdict (load: $(uptime | sed 's/.*load average: //'))"
[ "$base_rc" = 134 ] || refuse "the fixture exited rc=$base_rc at the shipped default, not the rc=134 hard-cap abort this gate claims -- it is no longer a witness for a declared arena and every arm below would grade a tautology. Grow it or re-measure the cap."
[ "$decl_rc" = 0 ] || refuse "the fixture does not complete even at ${DECL_KB} KB (rc=$decl_rc) -- the declaration cannot be shown to buy anything, so a green here would mean nothing"
echo "  PREMISE MEASURED: hungry.icn rc=$base_rc (hard-cap abort) at the shipped default, rc=$decl_rc at ${DECL_KB} KB -- the two verdicts differ and neither is a timeout, so the arms below can tell a honoured declaration from a dropped one"

# ── arm B: the runner's reader actually changes the verdict ────────────────────────────────────────────────────
if [ "${FAIL_ONCE:-0}" = 1 ]; then
  # Restore the pre-column world: the reader returns nothing for everybody.
  declared_arena_kb() { return 0; }
fi
b_rc=0
( cd "$T/pkg" && run_at_declared_arena "$T/pkg/ALL.csv" hungry -- env -u SCRIP_HEAP_MB -u SCRIP_HEAP_CAP_KB -u SCRIP_HEAP_MAX_MB timeout 60s "$ROOT/scrip" --run hungry.icn >/dev/null 2>&1 ) || b_rc=$?
[ "$b_rc" != 124 ] || refuse "arm B TIMED OUT (rc=124) -- the ceiling fired, so this run cannot say whether the declaration was honoured"
[ "$b_rc" = 0 ] && ck ok "B  the declared entry COMPLETES through the runner's reader (rc=0) where the same program rc=$base_rc at the shipped default -- the declaration is honoured, and a reader that silently did nothing could not pass this" \
  || ck bad "B  the declared entry still fails through the runner's reader (rc=$b_rc) -- heap_kb=${DECL_KB} was not exported for it"

# ── arm C: the NEIGHBOUR still runs at the shipped default (no leak, no pin) ───────────────────────────────────
seen=$(run_at_declared_arena "$T/pkg/ALL.csv" frugal -- bash -c 'echo "[${SCRIP_HEAP_KB:-UNSET}]"')
[ "$seen" = "[UNSET]" ] && ck ok "C1 the undeclared neighbour sees SCRIP_HEAP_KB UNSET -- the export is per program, not per run" \
  || ck bad "C1 the undeclared neighbour saw SCRIP_HEAP_KB=$seen -- one entry's declaration leaked over the loop, which is a runner PINNING an arena (the thing a declaration replaces)"
after="[${SCRIP_HEAP_KB:-UNSET}]"
[ "$after" = "[UNSET]" ] && ck ok "C2 nothing leaked into the gate's own environment after the declared run" \
  || ck bad "C2 SCRIP_HEAP_KB=$after leaked out of the declared run into the caller"
seen2=$(run_at_declared_arena "$T/pkg/ALL.csv" hungry -- bash -c 'echo "[${SCRIP_HEAP_KB:-UNSET}]"')
[ "$seen2" = "[$DECL_KB]" ] && ck ok "C3 the declared entry sees exactly SCRIP_HEAP_KB=$DECL_KB" \
  || ck bad "C3 the declared entry saw $seen2, expected [$DECL_KB]"

# ── arm D: a cell that grants no capacity is REFUSED, and the runtime still says why ───────────────────────────
printf 'rank,entry,origin,package,n_lines,stdin,want_rc,modes,heap_kb\n1,hungry,p__hungry,p,5,0,0,m3,2048\n' > "$T/pkg/BAD.csv"
declared_arena_kb "$T/pkg/BAD.csv" hungry >/dev/null 2>"$T/bad.err"; d_rc=$?
[ "$d_rc" = 2 ] && ck ok "D1 heap_kb=2048 (<= the ${rt_cap} KB shipped cap) is REFUSED rc=2 rather than accepted as a silent no-op" \
  || ck bad "D1 heap_kb=2048 returned rc=$d_rc -- a cell that grants no capacity was accepted, and it would read downstream as a capacity declaration"
grep -q "granting no capacity" "$T/bad.err" && ck ok "D1b the refusal names WHY, not just THAT" || ck bad "D1b the refusal does not say why"
( cd "$T/pkg" && env -u SCRIP_HEAP_MB -u SCRIP_HEAP_CAP_KB -u SCRIP_HEAP_MAX_MB SCRIP_HEAP_KB=2048 timeout 60s "$ROOT/scrip" --run hungry.icn >/dev/null 2>&1 )
d2=$?
[ "$d2" != 124 ] || refuse "arm D2 TIMED OUT (rc=124) -- a ceiling is not evidence about the cap rule"
[ "$d2" = 134 ] && ck ok "D2 the runtime STILL caps a 2048 KB window at ${rt_cap} KB (rc=$d2, the same hard-cap abort as the default) -- the measurement arm D enforces is still true of this tree" \
  || ck bad "D2 a 2048 KB window now COMPLETES (rc=0) -- gc_heap.c's cap rule changed, so arm D is enforcing a rule the runtime no longer has and the floor must be re-derived, not left standing"

# ── arm E: the declaration travels with an extracted family (CEO-1127's evidence bar) ──────────────────────────
cat > "$T/fam.icn" <<'EOF'
x
EOF
python3 - "$T" "$DECL_KB" <<'PY' >/dev/null 2>&1
import csv, sys
T, kb = sys.argv[1], sys.argv[2]
with open(T + "/famcsv.csv", "w", newline="") as f:
    w = csv.writer(f, lineterminator="\n")
    w.writerow(["rank","entry","origin","family","kind","xfail","n_lines","modes","heap_kb"])
    w.writerow([1,"e_one","fam__e_one","fam","block",0,3,"m3",kb])
    w.writerow([2,"e_two","fam__e_two","fam","block",0,3,"m3",""])
PY
e_have=$(python3 -c "
import sys; sys.path.insert(0,'$HERE')
import importlib.util
spec=importlib.util.spec_from_file_location('h','$HERE/corpus_suite_harness.py')
m=importlib.util.module_from_spec(spec)
try:
    spec.loader.exec_module(m)
except SystemExit:
    pass
print(m.heap_sidecar_path('$T/fam.icn'))
" 2>/dev/null)
[ "$e_have" = "$T/fam.heap" ] && ck ok "E1 the harness names a travelling sidecar for an extracted pair ($e_have)" \
  || ck bad "E1 heap_sidecar_path() did not name <stem>.heap beside the pair (got '$e_have')"
# ⛔ UNDER FAIL_ONCE THE SIDECAR IS NOT WRITTEN AT ALL -- that is exactly the pre-column world for an
# extracted pair (extract-family carried modes and not heap_kb), so E2 must red. Neutering only the SHELL
# reader above would leave this arm green on the Python path and the header's claim that "arms B and E go
# red" would be false about its own mutant, which is the fail-once-that-proves-nothing shape COO-153 found
# three of inside the very mechanism built to disprove it.
[ "${FAIL_ONCE:-0}" = 1 ] || printf '# planted\ne_one\t%s\n' "$DECL_KB" > "$T/fam.heap"
e_read=$(python3 -c "
import sys; sys.path.insert(0,'$HERE')
import importlib.util
spec=importlib.util.spec_from_file_location('h','$HERE/corpus_suite_harness.py')
m=importlib.util.module_from_spec(spec)
try:
    spec.loader.exec_module(m)
except SystemExit:
    pass
d,src=m.heap_declarations('$T/fam.icn')
print('%s|%s' % (d.get('e_one'), 'e_two' in d))
" 2>/dev/null)
[ "$e_read" = "$DECL_KB|False" ] && ck ok "E2 the harness reads the declaration back out of the travelling sidecar ($DECL_KB) and leaves the undeclared entry absent -- a cured seat's standalone run sees the same arena as the board" \
  || ck bad "E2 sidecar read back '$e_read', expected '$DECL_KB|False' -- a declaration would not survive extract-family, so every standalone cure run would silently use the shipped default"

# ── arm G: the PYTHON harness exports at a real run, not merely reads the column ───────────────────────────────
# ⛔⭐ ARMS B AND C PROVE THE SHELL READER; E PROVES THE PYTHON READER CAN READ.  Neither proves that
# corpus_suite_harness.py, which is what grades every master and three of the packages, actually puts the
# value into the environment of the process it runs.  A reader that parses perfectly and threads the value
# nowhere is the exact silent shape this gate was written against, and it would pass A..F.  So arm G runs
# the SAME fixture through run_m3() twice and requires the two verdicts to differ.
g_out=$(python3 - "$T" "$DECL_KB" "$HERE" "$FIX_N" 2>/dev/null <<'PY'
import importlib.util, sys, os
T, kb, HERE = sys.argv[1], int(sys.argv[2]), sys.argv[3]
spec = importlib.util.spec_from_file_location("h", os.path.join(HERE, "corpus_suite_harness.py"))
m = importlib.util.module_from_spec(spec)
try: spec.loader.exec_module(m)
except SystemExit: pass
paths = m.resolve_paths()
want = sys.argv[4]
a = m.run_m3(paths, os.path.join(T, "pkg", "hungry.icn"), want, heap_kb=None).kind
b = m.run_m3(paths, os.path.join(T, "pkg", "hungry.icn"), want, heap_kb=kb).kind
print("%s|%s" % (a, b))
PY
)
case "$g_out" in
  PASS\|*) ck bad "G  the harness read $g_out -- the fixture PASSES at the shipped default under run_m3, so this arm proves nothing about the export" ;;
  *\|PASS) ck ok  "G  corpus_suite_harness.py run_m3 verdicts DIFFER across the declaration ($g_out) -- the value reaches the process environment, not just the parser" ;;
  "")      ck bad "G  the harness could not be exercised at all (no output) -- the export is unmeasured, which is not the same as working" ;;
  *)       ck bad "G  harness verdicts did not change across the declaration ($g_out) -- heap_kb was parsed and threaded nowhere" ;;
esac

# ── arm F: every shipped attribute file carries the column ─────────────────────────────────────────────────────
CORPUS="${S4E_HOME:-$(cd "$ROOT/.." && pwd)}/corpus"
miss=0; tot=0
for f in "$CORPUS"/tests/*/ALL.csv "$CORPUS"/packages/*/*/ALL.csv; do
  [ -f "$f" ] || continue
  tot=$((tot+1)); head -1 "$f" | grep -q ',heap_kb' || { miss=$((miss+1)); echo "     NO heap_kb: $f"; }
done
[ "$tot" -gt 0 ] || refuse "found no attribute files under $CORPUS -- cannot measure the population"
[ "$miss" = 0 ] && ck ok "F  all $tot shipped attribute file(s) carry the heap_kb column (converted in ONE landing, so no runner can read a file that lacks it)" \
  || ck bad "F  $miss of $tot attribute file(s) lack the heap_kb column"

echo "GATE $G: $checks check(s), $fails failure(s) -- population: $tot attribute file(s), 1 planted declared entry, 1 planted neighbour, arena ${DECL_KB} KB vs shipped default (cap ${rt_cap} KB, floor ${rt_flr} KB)"
[ "$fails" = 0 ] || exit 1
exit 0
