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
# ââ­â­ ARM D ENFORCES A RULING, NOT A PREFERENCE: A DECLARATION IS A NUMBER ABOVE THE COMPILED CAP OR IT IS NOT A DECLARATION (ceo CEO-1171, 2026-09-23, ruling on the coo's ask).
# The refusal was asked for rather than assumed -- criterion-writing is not the instruments seat's verb --
# and the answer was that warn-and-pass would mint the false label CEO-1101 exists against. So arm D may
# not be relaxed to a warning by anyone reading this file later; it is law, cited, with its measurement.
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
# FAIL_ONCE=1 restores the pre-column world -- all five shell readers (declared_arena_kb, declared_arena_kb_beside,
# declared_stack_kb, declared_stack_kb_beside, declared_memory_table) return nothing for everybody AND the travelling sidecar is not
# written -- and MEASURED 2026-09-23 21:5x (coo, the table arms T and the runner census R added on CEO-1229) it reds exactly fourteen
# arms: B, C3, D1, D1b, E2, H1, H2, S1, S3, S6, T1, T2, T3 and T5. The other sixteen are green BY CONSTRUCTION under the mutant and
# are named so nobody reads their green as coverage: A1..A4, D2 and S4 read the runtime, F reads the shipped tree, E1 names a path,
# C1/C2/H3/S2 describe the reader-off world itself, G and S5 drive the Python harness, which the mutant leaves alone, T4 refuses a
# run that has no table (no reader involved), and R is a census of the runners' source -- its own fail-once is a scratch tree
# holding origin's unwired PAT runner and logtalk grader, where R reds naming exactly those two files and nothing else.
# An arm that has never been seen to fail is an arm that reads "there was never a bug here" (this gate's own law,
# COO-152), and a fail-once that reds for a STAGING reason while claiming to have proved the property is the shape
# COO-153 found three of inside the mechanism built to disprove exactly that -- so the ten are listed, not counted.
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
  declared_arena_kb_beside() { return 0; }
  declared_stack_kb() { return 0; }
  declared_stack_kb_beside() { return 0; }
  declared_memory_table() { return 0; }
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

# ── arm H: a STANDALONE program (a benchmark) declares through the <stem>.heap sidecar beside it ───────────────
# ⭐ (coo 2026-09-23, row instruments-benchmarks-enter-the-suite-grid-...): a benchmark tree has no ALL.csv, so its runners read
# the harness's own sidecar through declared_arena_kb_beside, which CALLS heap_declarations(). H1 requires the verdict to DIFFER
# across the declaration, as B does; H2 requires a bare number to be REFUSED, because the harness reads `16384` with no name as
# an absent declaration and a runner would then grade the program at the shipped default while the file claimed otherwise.
mkdir -p "$T/solo" "$T/solo2" && cp "$T/pkg/hungry.icn" "$T/solo/hungry.icn" && cp "$T/pkg/hungry.icn" "$T/solo2/hungry.icn"
printf 'hungry\t%s\n' "$DECL_KB" > "$T/solo/hungry.heap"
h_kb=$(declared_arena_kb_beside "$T/solo/hungry.icn" 2>/dev/null); h_rc=0
( cd "$T/solo" && if [ -n "$h_kb" ]; then export SCRIP_HEAP_KB="$h_kb"; fi
  env -u SCRIP_HEAP_MB -u SCRIP_HEAP_CAP_KB -u SCRIP_HEAP_MAX_MB timeout 60s "$ROOT/scrip" --run hungry.icn >/dev/null 2>&1 ) || h_rc=$?
[ "$h_rc" != 124 ] || refuse "arm H1 TIMED OUT (rc=124) -- the ceiling fired, so this run cannot say whether the sidecar was honoured"
[ "$h_rc" = 0 ] && ck ok "H1 a standalone program's hungry.heap (hungry TAB $DECL_KB) is read beside it and the run COMPLETES (rc=0) where the shipped default reads rc=$base_rc" \
  || ck bad "H1 the standalone program still fails (rc=$h_rc, read '$h_kb') -- its .heap sidecar was not honoured"
printf '%s\n' "$DECL_KB" > "$T/solo2/hungry.heap"
declared_arena_kb_beside "$T/solo2/hungry.icn" >/dev/null 2>"$T/bare.err"; h2=$?
[ "$h2" = 2 ] && grep -q 'declares no heap for hungry.icn' "$T/bare.err" && ck ok "H2 a bare-number .heap sidecar is REFUSED rc=2 by name -- never read as 'no declaration'" \
  || ck bad "H2 a bare-number .heap sidecar returned rc=$h2 -- the declaration would vanish silently while the file claimed it"
h3=$(declared_arena_kb_beside "$T/pkg/frugal.icn"); h3_rc=$?
[ "$h3_rc" = 0 ] && [ -z "$h3" ] && ck ok "H3 a program with no sidecar reads nothing (rc=0): the shipped default, not an error" \
  || ck bad "H3 a program with no sidecar read '$h3' rc=$h3_rc"

# ── arm S: THE STACK, THE HEAP'S TWIN (CEO-1225, the coo 2026-09-23) ─────────────────────────────────────────────────────────
# The runtime raises RLIMIT_STACK at init to SCRIP_STACK when set, else to its 64 MB floor (core.c), in BOTH modes. The witness:
# a DEPTH-deep Icon recursion dies with ERROR 246 at the default floor and completes at STK_KB; its neighbour (half as deep)
# completes at the default and dies at 16384k -- the measurement behind the floor rule: SCRIP_STACK REPLACES the floor, so a cell
# below it SHRINKS the stack. A timeout is a refusal here too, never a verdict.
core="$ROOT/src/runtime/core/core.c"; [ -f "$core" ] || refuse "no $core to read the stack floor back out of"
c_flr=$(sed -n 's/^[[:space:]]*long floor = \([0-9]*\)L \* 1024 \* 1024;.*/\1/p' "$core" | head -1)
[ -n "$c_flr" ] || refuse "could not read the stack floor (long floor = NL * 1024 * 1024) out of $core -- the drift alarm cannot be armed"
c_flr_kb=$((c_flr * 1024))
[ "$DECLARED_STACK_FLOOR_KB" = "$c_flr_kb" ] && grep -q "^GC_STACK_FLOOR_KB = 64 \* 1024 " "$HERE/corpus_suite_harness.py" && [ "$c_flr_kb" = 65536 ] \
  && ck ok "A4 the stack floor copied into lib_declared_arena.sh ($DECLARED_STACK_FLOOR_KB) and corpus_suite_harness.py matches core.c's ($c_flr_kb KB)" \
  || ck bad "A4 stack floor drift: lib $DECLARED_STACK_FLOOR_KB, core.c ${c_flr_kb} KB -- a copied runtime constant moved"
STK_KB=262144; DEPTH=200000
mkdir -p "$T/stk"
printf 'procedure f(n)\n   if n = 0 then return 0\n   return 1 + f(n - 1)\nend\nprocedure main()\n   write(f(%d))\nend\n' "$DEPTH" > "$T/stk/deep.icn"
printf 'procedure f(n)\n   if n = 0 then return 0\n   return 1 + f(n - 1)\nend\nprocedure main()\n   write(f(%d))\nend\n' $((DEPTH / 2)) > "$T/stk/shallow.icn"
printf 'rank,entry,origin,package,n_lines,stdin,want_rc,modes,heap_kb,stack_kb\n1,deep,p__deep,p,7,0,0,m3,,%s\n2,shallow,p__shallow,p,7,0,0,m3,,\n' "$STK_KB" > "$T/stk/ALL.csv"
runstk() { ( cd "$T/stk" && env -u SCRIP_STACK -u SCRIP_HEAP_KB -u SCRIP_HEAP_MB "$@" timeout 60s "$ROOT/scrip" --run "$STK_PROG" </dev/null >/dev/null 2>"$T/stk/err" ); }
STK_PROG=deep.icn; runstk; s_base=$?; grep -q 'ERROR 246' "$T/stk/err"; s_base246=$?
STK_PROG=deep.icn; runstk SCRIP_STACK="${STK_KB}k"; s_decl=$?
[ "$s_base" != 124 ] && [ "$s_decl" != 124 ] || refuse "the stack witness TIMED OUT (rc=124) -- a ceiling is not a stack verdict (load: $(uptime | sed 's/.*load average: //'))"
[ "$s_base246" = 0 ] || refuse "the stack witness at the default did not die with ERROR 246 (rc=$s_base) -- it no longer overflows the floor, so it witnesses nothing; deepen it"
[ "$s_decl" = 0 ] || refuse "the stack witness does not complete even at ${STK_KB} KB (rc=$s_decl) -- the declaration cannot be shown to buy anything"
echo "  STACK PREMISE MEASURED: deep.icn (depth $DEPTH) rc=$s_base with ERROR 246 at the runtime's floor, rc=$s_decl at SCRIP_STACK=${STK_KB}k"
s1=0; ( cd "$T/stk" && run_at_declared_arena "$T/stk/ALL.csv" deep -- env -u SCRIP_HEAP_KB timeout 60s "$ROOT/scrip" --run deep.icn </dev/null >/dev/null 2>&1 ) || s1=$?
[ "$s1" != 124 ] || refuse "arm S1 TIMED OUT (rc=124)"
[ "$s1" = 0 ] && ck ok "S1 the stack_kb=${STK_KB} entry COMPLETES through the runner's reader (rc=0) where it dies at the floor (rc=$s_base, ERROR 246)" \
  || ck bad "S1 the stack_kb entry still fails through the runner's reader (rc=$s1) -- the stack declaration was not exported"
s2=$(run_at_declared_arena "$T/stk/ALL.csv" shallow -- bash -c 'echo "[${SCRIP_STACK:-UNSET}]"')
[ "$s2" = "[UNSET]" ] && ck ok "S2 the undeclared neighbour sees SCRIP_STACK UNSET -- the stack export is per program" \
  || ck bad "S2 the undeclared neighbour saw SCRIP_STACK=$s2 -- a stack declaration leaked over the loop"
printf 'rank,entry,origin,package,n_lines,stdin,want_rc,modes,heap_kb,stack_kb\n1,deep,p__deep,p,7,0,0,m3,,16384\n' > "$T/stk/BAD.csv"
declared_stack_kb "$T/stk/BAD.csv" deep >/dev/null 2>"$T/stk/bad.err"; s3=$?
[ "$s3" = 2 ] && grep -q 'SHRINKS the stack' "$T/stk/bad.err" && ck ok "S3 stack_kb=16384 (below the ${c_flr_kb} KB floor) is REFUSED rc=2, naming that it would SHRINK the stack" \
  || ck bad "S3 stack_kb=16384 returned rc=$s3 -- a cell that shrinks the stack was accepted"
STK_PROG=shallow.icn; runstk; s4a=$?; STK_PROG=shallow.icn; runstk SCRIP_STACK=16384k; s4b=$?; grep -q 'ERROR 246' "$T/stk/err"; s4b246=$?
[ "$s4a" != 124 ] && [ "$s4b" != 124 ] || refuse "arm S4 TIMED OUT (rc=124)"
[ "$s4a" = 0 ] && [ "$s4b" != 0 ] && [ "$s4b246" = 0 ] && ck ok "S4 the runtime STILL replaces its floor: the half-depth witness completes at the default and dies with ERROR 246 at SCRIP_STACK=16384k -- the measurement S3 enforces is true of this tree" \
  || ck bad "S4 the floor rule's premise moved (half-depth: default rc=$s4a, 16384k rc=$s4b) -- re-derive the stack floor rule, do not leave S3 standing on it"
s5=$(python3 - "$T/stk" "$STK_KB" "$HERE" "$DEPTH" 2>/dev/null <<'PY'
import importlib.util, sys, os
T, kb, HERE, want = sys.argv[1], int(sys.argv[2]), sys.argv[3], sys.argv[4]
spec = importlib.util.spec_from_file_location("h", os.path.join(HERE, "corpus_suite_harness.py"))
m = importlib.util.module_from_spec(spec)
try: spec.loader.exec_module(m)
except SystemExit: pass
paths = m.resolve_paths()
a = m.run_m3(paths, os.path.join(T, "deep.icn"), want, stack_kb=None).kind
b = m.run_m3(paths, os.path.join(T, "deep.icn"), want, stack_kb=kb).kind
print("%s|%s" % (a, b))
PY
)
case "$s5" in
  PASS\|*) ck bad "S5 the harness read $s5 -- the witness PASSES at the floor under run_m3, so this arm proves nothing" ;;
  *\|PASS) ck ok  "S5 corpus_suite_harness.py run_m3 verdicts DIFFER across stack_kb ($s5) -- the stack reaches the process, not just the parser" ;;
  "")      ck bad "S5 the harness could not be exercised (no output)" ;;
  *)       ck bad "S5 harness verdicts did not change across stack_kb ($s5)" ;;
esac
mkdir -p "$T/stk2" && cp "$T/stk/deep.icn" "$T/stk2/deep.icn" && printf 'deep\t%s\n' "$STK_KB" > "$T/stk2/deep.stack"
s6=$(declared_stack_kb_beside "$T/stk2/deep.icn" 2>/dev/null); printf '%s\n' "$STK_KB" > "$T/stk2/deep.stack"; declared_stack_kb_beside "$T/stk2/deep.icn" >/dev/null 2>&1; s6b=$?
[ "$s6" = "$STK_KB" ] && [ "$s6b" = 2 ] && ck ok "S6 a standalone program's deep.stack (deep TAB $STK_KB) is read beside it, and a bare-number .stack is REFUSED rc=2" \
  || ck bad "S6 the .stack sidecar read '$s6' (want $STK_KB) and a bare number returned rc=$s6b (want 2)"

# ── arm T: THE ONE-READ TABLE the package runners grade through (CEO-1229, the coo 2026-09-23) ─────────────────────────────
# declared_memory_table / declared_memory_begin / run_at_declared_table (lib_declared_arena.sh): the harness's own validators, one
# python start per suite and one awk per run, because run_at_declared_arena's two python starts per run are over a minute of a
# 427-program board in two modes. T2 drives a REAL run through the table. T5 is the defect the table's first cut shipped to itself
# and its own refusal arm caught: handed any file, heap_declarations() read the ALL.csv BESIDE it.
mkdir -p "$T/tbl"
printf 'rank,entry,origin,package,n_lines,stdin,want_rc,modes,heap_kb,stack_kb\n1,a,p__a,p,1,0,0,m3,%s,\n2,b,p__b,p,1,0,0,m3,,262144\n3,c,p__c,p,1,0,0,m3,,\n' "$DECL_KB" > "$T/tbl/ALL.csv"
declared_memory_begin "$T/tbl/ALL.csv" "$T/tbl/t.tsv" >/dev/null 2>&1; t1rc=$?
t1=""; for e in a b c; do t1="$t1$(run_at_declared_table "$T/tbl/t.tsv" "$e" -- bash -c 'printf "%s=[%s|%s] " "$0" "${SCRIP_HEAP_KB:-}" "${SCRIP_STACK:-}"' "$e" 2>/dev/null)"; done
[ "$t1rc" = 0 ] && [ "$t1" = "a=[$DECL_KB|] b=[|262144k] c=[|] " ] && [ -z "${SCRIP_HEAP_KB:-}${SCRIP_STACK:-}" ] \
  && ck ok "T1 the table exports each entry's own heap and stack and nothing else (a heap only, b stack only, c neither), and nothing leaks into the caller" \
  || ck bad "T1 the table exported '$t1' (begin rc=$t1rc) -- want 'a=[$DECL_KB|] b=[|262144k] c=[|] '"
t2=0; { declared_memory_begin "$T/pkg/ALL.csv" "$T/tbl/pkg.tsv" >/dev/null 2>&1 && ( cd "$T/pkg" && run_at_declared_table "$T/tbl/pkg.tsv" hungry -- env -u SCRIP_HEAP_MB -u SCRIP_HEAP_CAP_KB -u SCRIP_HEAP_MAX_MB timeout 60s "$ROOT/scrip" --run hungry.icn >/dev/null 2>&1 ); } || t2=$?
[ "$t2" != 124 ] || refuse "arm T2 TIMED OUT (rc=124) -- a ceiling is not a verdict"
[ "$t2" = 0 ] && ck ok "T2 the declared entry COMPLETES through the table (rc=0) where it aborts rc=$base_rc at the shipped default -- the runners' path carries the declaration to a real run" \
  || ck bad "T2 the declared entry still fails through the table (rc=$t2) -- heap_kb=${DECL_KB} did not reach the run"
t3=""; n3=0
for bad in "1,a,p__a,p,1,0,0,m3,2048," "1,a,p__a,p,1,0,0,m3,,16384" "1,a,p__a,p,1,0,0,m3,$DECL_KB,
2,a,p__a,p,1,0,0,m3,$DECL_KB,"; do
  n3=$((n3+1)); printf 'rank,entry,origin,package,n_lines,stdin,want_rc,modes,heap_kb,stack_kb\n%s\n' "$bad" > "$T/tbl/bad.csv"; echo stale > "$T/tbl/bad.tsv"
  declared_memory_begin "$T/tbl/bad.csv" "$T/tbl/bad.tsv" >/dev/null 2>&1; r=$?
  { [ "$r" = 2 ] && [ ! -f "$T/tbl/bad.tsv" ]; } || t3="$t3 case$n3(rc=$r)"
done
[ -z "$t3" ] && ck ok "T3 a heap cell at the cap, a stack cell below the floor and a declaring entry named twice each REFUSE rc=2 and leave no table for a run to use" \
  || ck bad "T3 a refused cell was accepted:$t3 (1 heap 2048, 2 stack 16384, 3 a duplicate entry)"
run_at_declared_table "$T/tbl/no_such_table.tsv" a -- true 2>/dev/null; t4=$?
[ "$t4" = 2 ] && ck ok "T4 a run with no table REFUSES rc=2 -- a runner that never built its table cannot grade at the default under a receipt naming declarations" \
  || ck bad "T4 run_at_declared_table without a table returned rc=$t4, want 2"
printf 'rank,entry,origin,package,n_lines,stdin,want_rc,modes,heap_kb,stack_kb\n1,a,p__a,p,1,0,0,m3,,\n' > "$T/tbl/ALL.csv"
printf 'rank,entry,origin,package,n_lines,stdin,want_rc,modes,heap_kb,stack_kb\n1,a,p__a,p,1,0,0,m3,%s,\n' "$DECL_KB" > "$T/tbl/OTHER.csv"
t5="$(declared_memory_table "$T/tbl/OTHER.csv" 2>/dev/null)"
[ "$t5" = "$(printf 'a\t%s\t' "$DECL_KB")" ] && ck ok "T5 the table reads the FILE IT IS HANDED -- OTHER.csv's heap_kb=$DECL_KB, not the undeclared ALL.csv beside it" \
  || ck bad "T5 the table of OTHER.csv read '$t5' -- it read something other than the file it was handed"

# ── arm R: EVERY PACKAGE RUNNER READS THE DECLARATION (CEO-1229: "wire the ten remaining package runners yourself on the SWI pattern,
# each proven on a fixture") ────────────────────────────────────────────────────────────────────────────────────────────
# STRUCTURAL, AND NAMED AS SUCH: it holds each runner's wiring against a later edit that drops it -- every one was PROVEN on a
# fixture by the coo 2026-09-23 (a declared entry passes, the same entry emptied fails, a refused cell refuses; the receipts are in
# the row's baton) -- and it cannot show that the wiring works; arms B, S and T do that.
r_miss=""; r_n=0
need() { local f="$HERE/$1" c; r_n=$((r_n+1)); [ -f "$f" ] || { r_miss="$r_miss $1(missing)"; return; }; c=$(grep -c -- "$3" "$f"); [ "$c" -ge "$4" ] || r_miss="$r_miss $1($2: $c of $4)"; }
for r in test_snobol4_dotnet_suite.sh test_snobol4_spitbol_testpgms_suite.sh test_snobol4_csnobol4_suite.sh test_snoflake_suite.sh test_snobol4_spitbol_x64_suite.sh test_prolog_gnu_suite.sh test_pascal_fpc_suite.sh test_pascal_pat_suite.sh; do
  need "$r" begin 'declared_memory_begin "\$[A-Z_]*/ALL.csv"' 1; need "$r" runs 'run_at_declared_table "\$DECL"' 2
done
need scorecard_snobol4.sh table 'sc_decl_build' 3; need scorecard_snobol4.sh runs 'run_at_declared_table "\$SC_DECL"' 2
need test_snobol4_gimpel_suite.sh through-the-scorecard 'scorecard_snobol4.sh" run --suites gimpel' 1
need test_snobol4_aisnobol_suite.sh through-the-harness 'corpus_suite_harness.py run "\$SUITE/ALL.sno"' 1
need test_prolog_inria_suite.sh begin 'declared_memory_begin "\$SUITE/ALL.csv"' 1; need test_prolog_inria_suite.sh runs 'env=decl_env(_tidx, fam)' 4
need test_prolog_logtalk_suite.sh begin 'declared_memory_begin "\$SUITE/ALL.csv"' 1; need test_prolog_logtalk_suite.sh hands-it-over '--decl "\$_decl"' 1
need util_logtalk_grade.py per-case 'env=decl_env(decl' 2; need util_logtalk_grade.py runs 'cwd=d, env=env)' 2
need test_prolog_swi_suite.sh runs 'run_at_declared_arena "\$SWIT/ALL.csv"' 2
for r in test_icon_arizona_suite.sh test_icon_ipl_suite.sh test_icon_jcon_suite.sh; do need "$r" heap declared_arena_kb 1; need "$r" stack declared_stack_kb 1; done
[ -z "$r_miss" ] && ck ok "R  every package runner reads the declared heap and stack ($r_n wiring points across 17 runners and graders: the eight SNOBOL4, Prolog and Pascal loops, the scorecard behind gimpel, the harness behind aisnobol, inria, logtalk and its grader, SWI, and the three Icon runners)" \
  || ck bad "R  a runner lost its wiring:$r_miss"

# ── arm F: every shipped attribute file carries the column ─────────────────────────────────────────────────────
CORPUS="${S4E_HOME:-$(cd "$ROOT/.." && pwd)}/corpus"
miss=0; tot=0
for f in "$CORPUS"/tests/*/ALL.csv "$CORPUS"/packages/*/*/ALL.csv; do
  [ -f "$f" ] || continue
  tot=$((tot+1)); h1="$(head -1 "$f")"; case "$h1" in *,heap_kb,stack_kb*) ;; *) miss=$((miss+1)); echo "     NO heap_kb,stack_kb: $f";; esac
done
[ "$tot" -gt 0 ] || refuse "found no attribute files under $CORPUS -- cannot measure the population"
[ "$miss" = 0 ] && ck ok "F  all $tot shipped attribute file(s) carry the heap_kb and stack_kb columns, side by side (each converted in ONE landing, so no runner can read a file that lacks one)" \
  || ck bad "F  $miss of $tot attribute file(s) lack the heap_kb,stack_kb columns"

echo "GATE $G: $checks check(s), $fails failure(s) -- population: $tot attribute file(s); heap: 1 planted declared entry, 1 planted neighbour, arena ${DECL_KB} KB vs shipped default (cap ${rt_cap} KB, floor ${rt_flr} KB); stack: a depth-$DEPTH witness at ${STK_KB} KB vs the runtime's ${c_flr_kb} KB floor"
[ "$fails" = 0 ] || exit 1
exit 0
