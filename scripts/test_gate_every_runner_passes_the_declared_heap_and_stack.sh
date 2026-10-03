#!/usr/bin/env bash
export S4E_ONE_RUNNER_FIXTURE="gate arm ${0##*/}: a runner invoked as an instrument fixture over a scratch population, not a board (CEO-547)"
# test_gate_every_runner_passes_the_declared_heap_and_stack.sh -- EVERY RUNNER PASSES EACH UNIT'S DECLARED HEAP AND STACK IN BOTH MODES,
# NEVER AS THE WINDOW (Lon 2026-09-28 15:4x CDT, in-chat to the ceo, verbatim: "Ensure that every program in every language as it necessary
# stack size and heap size values stored in the per-program attribute file, and ensure that those command-line switches and environment
# variable values are being used by the harness shell scripts which run all of them."; ceo CEO-1353; half (b) of the coo's row
# instruments-every-runner-passes-each-units-declared-heap-and-stack-in-both-modes-never-as-the-window-names-a-missing-one).
# THE VERDICT-PAIR METHOD of test_gate_package_runners_read_each_units_declared_compile_args.sh, extended to SIZE. Each runner family
# grades FOUR scratch units of two witnesses whose outcome depends on the declared size, in both modes:
#   deepdecl    a 50000-deep recursion declaring stack_kb 262144      -> PASS  (it needs the declared stack)
#   deepnodecl  the same program at the shipped 4096 KB stack          -> FAIL  (ERROR 246 at the default)
#   livedecl    a 30000-entry live set declaring heap_kb 2048          -> FAIL  (HARD CAP at the declared 2 MB)
#   livenodecl  the same program at the shipped 131072 KB cap          -> PASS
# A runner that reads no declaration reads deepdecl FAIL; a runner that hands the declared heap to SCRIP_HEAP_KB (the collector's initial
# WINDOW, cap = max(128 MB, window)) reads livedecl PASS -- both are named. The premise arm W re-measures the four against scrip directly
# (and the window mistake), so the day a witness stops depending on its size this gate says so.
# FAMILIES: H the harness (the rung suites' and package tables' one reader), D test_demos_suite.sh (a standalone unit's .heap/.stack
# sidecars), P1 test_snobol4_dotnet_suite.sh, P2 test_snobol4_csnobol4_suite.sh (the SNOBOL4 package runners through
# run_at_declared_table), L lib_ladder.sh -- the one body of all seven test_<lang>_ladder.sh, driven through test_snobol4_ladder.sh
# over a scratch S4E_HOME whose rungs table carries the four units as ladder__rung00_* origins (the coo, 2026-09-30). PBS PBM PB4 the Prolog bench
# family's row runner and its two correctness scripts over a scratch kernel dir of four Prolog units (the coo, 2026-10-01; FAIL_ONCE reds
# PBM and PB4 through the stubbed declared_switches_beside; PBS runs from scripts/ itself, since it finds its binary beside its own script and
# reads the sidecars through declared_arena_kb_beside, which the plant leaves alone, so it stays green). EVERY OTHER RUNNER FAMILY IS NAMED BELOW AS NOT YET FIXTURED AND COUNTED RED -- the DONE-WHEN cannot pass while
# a family is unproven; each is added here as its lane cures its runner (the HQ asks of 2026-09-28). THE EIGHT BOOTSTRAP PARSER TOOLS
# grade no test unit -- each runs one fixed program per language, the chain -- so they are not a family here: their chain declares its
# sizes beside bootstrap/parser_<lang>.sc and test_gate_oracle_args_sidecar_is_declared_and_reaches_the_oracle.sh holds every tool to it
# (arms C and W, the coo 2026-10-03).
# FAIL_ONCE=1: the harness family runs from a scratch copy of scripts/ whose _size_switches returns nothing (the reader removed) -- H reds;
# the ladder family runs from a scratch copy whose lib_declared_arena.sh declared_switches_beside prints nothing -- L reds.
# EXIT 0 every family's pair holds and none is pending; 1 a pair failed or a family is pending; 2 could not measure.
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
. "$HERE/lib_gate.sh" || { echo "REFUSING(2): cannot load lib_gate.sh"; exit 2; }
GATE_NAME=every_runner_passes_the_declared_heap_and_stack
gate_parse_args "$@"
SCRIP="${SCRIP:-$ROOT/scrip}"; RT="${RT_DIR:-$ROOT/out}"
gate_require_exec "$SCRIP" "scrip binary" || exit 2
[ -n "${SCRIP_BIN:-}" ] || "$HERE/util_require_fresh.sh" --gate "$GATE_NAME" "$SCRIP" "$RT/libscrip_rt.so" || exit 2
. "$HERE/lib_oracle_flags.sh" 2>/dev/null || { echo "GATE UNPROVEN(2) [$GATE_NAME]: lib_oracle_flags.sh unloadable"; exit 2; }
SBL="$(sbl_correctness_bin 2>/dev/null)"; SBLF="$(sbl_lang_flags 2>/dev/null)"
[ -x "$SBL" ] || { echo "GATE UNPROVEN(2) [$GATE_NAME]: no SNOBOL4 oracle"; exit 2; }
SCRATCH="${S4E_SCRATCH:-$(cd "$ROOT/.." && pwd)/.scratch}"; mkdir -p "$SCRATCH" || exit 2
W=$(mktemp -d "$SCRATCH/gate_sizes_XXXXXX") || exit 2
trap 'rm -rf "$W"' EXIT INT TERM
PASS=0; FAIL=0
ok()  { PASS=$((PASS+1)); echo "  ok   $1: $2"; }
red() { FAIL=$((FAIL+1)); echo "  FAIL $1: $2"; }

# the witnesses
printf "        DEFINE('R(N)')                    :(REND)\nR       R = EQ(N,0) 0                     :S(RETURN)\n        R = R(N - 1) + 1                  :(RETURN)\nREND    OUTPUT = 'depth=' R(50000)\nEND\n" > "$W/deep.sno"
printf "        T = TABLE()\n        I = 0\nL       I = LT(I,30000) I + 1         :F(D)\n        T<I> = DUPL('x',100)          :(L)\nD       OUTPUT = 'built ' I\nEND\n" > "$W/live.sno"
(cd "$W" && timeout 60 "$SBL" $SBLF deep.sno < /dev/null > deep.ref 2>&1; timeout 60 "$SBL" $SBLF live.sno < /dev/null > live.ref 2>&1)
[ "$(cat "$W/deep.ref")" = "depth=50000" ] && [ "$(cat "$W/live.ref")" = "built 30000" ] || { echo "GATE UNPROVEN(2) [$GATE_NAME]: the oracle's refs moved: deep '$(head -c 60 "$W/deep.ref")' live '$(head -c 60 "$W/live.ref")'"; gate_stamp; exit 2; }
declare -A HEAP=([deepdecl]=131072 [deepnodecl]=131072 [livedecl]=2048 [livenodecl]=131072)
declare -A STACK=([deepdecl]=262144 [deepnodecl]=4096 [livedecl]=4096 [livenodecl]=4096)
declare -A WANT=([deepdecl]=PASS [deepnodecl]=FAIL [livedecl]=FAIL [livenodecl]=PASS)
UNITS="deepdecl deepnodecl livedecl livenodecl"
src_of() { case "$1" in deep*) echo "$W/deep.sno";; *) echo "$W/live.sno";; esac; }
ref_of() { case "$1" in deep*) echo "$W/deep.ref";; *) echo "$W/live.ref";; esac; }

echo "--- W PREMISE: the four units against scrip directly, both modes, and the window mistake ---"
m4bin() { (cd "$W" && timeout 120 "$SCRIP" --compile "$1" < /dev/null > "$2.s" 2>/dev/null && gcc -c "$2.s" -o "$2.o" && gcc "$2.o" -L"$RT" -lscrip_rt -lm -Wl,-rpath,"$RT" -o "$2.bin") >/dev/null 2>&1; }
m4bin "$W/deep.sno" "$W/deepbin"; m4bin "$W/live.sno" "$W/livebin"
prem_ok=1
for u in $UNITS; do
  s=$(src_of "$u"); r=$(ref_of "$u"); b=$W/$([ "${u#deep}" != "$u" ] && echo deepbin || echo livebin).bin
  o3=$(cd "$W" && timeout 60 "$SCRIP" --run -d${HEAP[$u]}k -s${STACK[$u]}k "$s" < /dev/null 2>/dev/null); o4=$(timeout 60 "$b" -d${HEAP[$u]}k -s${STACK[$u]}k < /dev/null 2>/dev/null)
  v3=FAIL; [ "$o3" = "$(cat "$r")" ] && v3=PASS; v4=FAIL; [ "$o4" = "$(cat "$r")" ] && v4=PASS
  [ "$v3" = "${WANT[$u]}" ] && [ "$v4" = "${WANT[$u]}" ] || { prem_ok=0; echo "      $u m3=$v3 m4=$v4 want ${WANT[$u]}"; }
done
ow=$(cd "$W" && SCRIP_HEAP_KB=2048 timeout 60 "$SCRIP" --run "$W/live.sno" < /dev/null 2>/dev/null)
[ "$prem_ok" = 1 ] || { echo "GATE UNPROVEN(2) [$GATE_NAME]: a witness no longer depends on its declared size -- the pairs below would measure nothing"; gate_stamp; exit 2; }
ok W "deepdecl PASS / deepnodecl FAIL / livedecl FAIL / livenodecl PASS against scrip -d -s directly, m3 and m4; the declared 2 MB handed to SCRIP_HEAP_KB (the window) reads '$(head -c 20 <<<"$ow")' -- PASS, the mistake a pair catches"

# outcome <db> <unit> <mode> -- the program's last outcome in a scratch progress table (a runner names it bare, or with its extension)
outcome() { awk -F'\t' -v p="$2" -v m="$3" 'NR>1 && ($8==p || $8==p".sno" || $8~("/"p"(\\.sno)?$")) && $9==m {v=$10} END {print v}' "$1" 2>/dev/null; }
quad() {  # <arm> <db> <label> -- the four units' outcomes in both modes against WANT
  local a="$1" db="$2" lbl="$3" bad="" u m v
  for u in $UNITS; do for m in m3 m4; do v=$(outcome "$db" "$u" "$m"); [ -n "$v" ] || v=MISSING
    { [ "${WANT[$u]}" = PASS ] && [ "$v" = PASS ]; } || { [ "${WANT[$u]}" = FAIL ] && [ "$v" != PASS ] && [ "$v" != MISSING ]; } || bad="$bad $u:$m=$v"; done; done
  if [ -z "$bad" ]; then ok "$a" "$lbl -- deepdecl PASS, deepnodecl not, livedecl not, livenodecl PASS, both modes"
  else red "$a" "$lbl --$bad (want deepdecl PASS, deepnodecl FAIL, livedecl FAIL, livenodecl PASS)"; fi; }
HDR="rank,entry,origin,package,n_lines,stdin,want_rc,heap_kb,stack_kb,compile_args,run_args"
csv4() { printf '%s\n' "$HDR"; local i=1 u; for u in $UNITS; do printf '%s,%s,%s,%s,5,0,0,%s,%s,,\n' "$i" "$u" "$u" "$1" "${HEAP[$u]}" "${STACK[$u]}"; i=$((i+1)); done; }

echo "--- H: the harness (the rung suites' and package tables' one reader) ---"
mkdir -p "$W/h"; python3 - "$HERE" "$W" <<'PY'
import sys; sys.path.insert(0, sys.argv[1]); import corpus_suite_harness as h
W = sys.argv[2]; src = []; ref = []
for i, u in enumerate(["deepdecl", "deepnodecl", "livedecl", "livenodecl"], 1):
    b = h.make_banner(i, u); w = "deep" if u.startswith("deep") else "live"
    src.append(b + "\n" + open(f"{W}/{w}.sno").read()); ref.append(b + "\n" + open(f"{W}/{w}.ref").read())
open(f"{W}/h/ALL.sno", "w").write("".join(src)); open(f"{W}/h/ALL.ref", "w").write("".join(ref))
PY
csv4 hfam > "$W/h/ALL.csv"
HSCRIPTS="$HERE"
if [ "${FAIL_ONCE:-0}" = 1 ]; then
  mkdir -p "$W/fo"; cp -rs "$HERE" "$W/fo/scripts" 2>/dev/null; rm -f "$W/fo/scripts/corpus_suite_harness.py"
  sed 's/^def _size_switches(heap_kb, stack_kb):/def _size_switches(heap_kb, stack_kb):\n    return []  # FAIL_ONCE: the reader removed/' "$HERE/corpus_suite_harness.py" > "$W/fo/scripts/corpus_suite_harness.py"
  grep -q 'FAIL_ONCE: the reader removed' "$W/fo/scripts/corpus_suite_harness.py" || { echo "GATE UNPROVEN(2) [$GATE_NAME]: FAIL_ONCE found no _size_switches anchor"; exit 2; }
  HSCRIPTS="$W/fo/scripts"; echo "FAIL_ONCE=1: the harness family runs with _size_switches removed"
fi
oh=$(SCRIP="$SCRIP" RT_DIR="$RT" S4E_PROGRESS_DB="$W/h.tsv" timeout 600 python3 "$HSCRIPTS/corpus_suite_harness.py" run "$W/h/ALL.sno" "$W/h/ALL.ref" --modes m3,m4 2>&1); rh=$?
[ "$rh" = 2 ] && { echo "      $(grep -m1 -E 'REFUS' <<<"$oh" | cut -c1-200)"; }
# a suite outside the corpus appends no progress rows, so the harness's own output is read: every non-PASS entry prints
# "  <KIND> <mode> <name>: ..." and an entry absent from those lines in a graded run (SUITE_BOARD printed) passed
if grep -q '^SUITE_BOARD ' <<<"$oh"; then
  { printf 'ts\tscrip\tcorpus\tmeasurer\tclass\tsuite\tlang\tprogram\tmode\toutcome\n'
    for u in $UNITS; do for m in m3 m4; do k=$(grep -m1 -E "^  [A-Z]+ $m $u: " <<<"$oh" | awk '{print $1}'); printf 'x\tx\tx\tx\tx\th\tsnobol4\t%s\t%s\t%s\n' "$u" "$m" "${k:-PASS}"; done; done; } > "$W/h.tsv"
fi
quad H "$W/h.tsv" "corpus_suite_harness.py run over a scratch SNOBOL4 table (rc $rh)"

echo "--- D: test_demos_suite.sh (a standalone unit's .heap/.stack sidecars) ---"
mkdir -p "$W/d"
for u in $UNITS; do cp "$(src_of "$u")" "$W/d/$u.sno"; cp "$(ref_of "$u")" "$W/d/$u.ref"; printf '%s\t%s\n' "$u" "${HEAP[$u]}" > "$W/d/$u.heap"; printf '%s\t%s\n' "$u" "${STACK[$u]}" > "$W/d/$u.stack"; done
od=$(DEMOS_DIR="$W/d" S4E_PROGRESS_DB="$W/d.tsv" timeout 600 bash "$HERE/test_demos_suite.sh" snobol4 2>&1); rd=$?
[ "$rd" = 2 ] && echo "      $(grep -m1 -E 'REFUS' <<<"$od" | cut -c1-200)"
quad D "$W/d.tsv" "test_demos_suite.sh snobol4 over a scratch demos dir (rc $rd)"

echo "--- P1: test_snobol4_dotnet_suite.sh (live oracle diff, the table's heap_kb/stack_kb through run_at_declared_table) ---"
mkdir -p "$W/p1"; for u in $UNITS; do cp "$(src_of "$u")" "$W/p1/$u.sno"; done; csv4 dotnet > "$W/p1/ALL.csv"
o1=$(DOTNET_SUITE="$W/p1" S4E_PROGRESS_DB="$W/p1.tsv" timeout 600 bash "$HERE/test_snobol4_dotnet_suite.sh" 2>&1); r1=$?
[ "$r1" = 2 ] && echo "      $(grep -m1 -E 'REFUS' <<<"$o1" | cut -c1-200)"
quad P1 "$W/p1.tsv" "test_snobol4_dotnet_suite.sh over a scratch package (rc $r1)"

echo "--- P2: test_snobol4_csnobol4_suite.sh (the Budne runner, ref-graded, run_at_declared_table) ---"
mkdir -p "$W/p2"; for u in $UNITS; do cp "$(src_of "$u")" "$W/p2/$u.sno"; cp "$(ref_of "$u")" "$W/p2/$u.ref"; done; csv4 csnobol4_suite > "$W/p2/ALL.csv"; : > "$W/p2/ALL.ref"
o2=$(CSNOBOL4_SUITE="$W/p2" S4E_PROGRESS_DB="$W/p2.tsv" timeout 600 bash "$HERE/test_snobol4_csnobol4_suite.sh" 2>&1); r2=$?
[ "$r2" = 2 ] && echo "      $(grep -m1 -E 'REFUS' <<<"$o2" | cut -c1-200)"
quad P2 "$W/p2.tsv" "test_snobol4_csnobol4_suite.sh over a scratch package (rc $r2)"

echo "--- L: lib_ladder.sh (the ONE body of all seven ladders) through test_snobol4_ladder.sh over a scratch rungs table ---"
mkdir -p "$W/l/corpus/tests/snobol4"; python3 - "$HERE" "$W" <<'PY'
import sys; sys.path.insert(0, sys.argv[1]); import corpus_suite_harness as h
W = sys.argv[2]; src = []; ref = []
for i, u in enumerate(["deepdecl", "deepnodecl", "livedecl", "livenodecl"], 1):
    b = h.make_banner(i, u); w = "deep" if u.startswith("deep") else "live"
    src.append(b + "\n" + open(f"{W}/{w}.sno").read()); ref.append(b + "\n" + open(f"{W}/{w}.ref").read())
open(f"{W}/l/corpus/tests/snobol4/ALL.sno", "w").write("".join(src)); open(f"{W}/l/corpus/tests/snobol4/ALL.ref", "w").write("".join(ref))
PY
{ printf '%s\n' "$HDR"; i=1; for u in $UNITS; do printf '%s,%s,ladder__rung00_%s,ladder,5,0,0,%s,%s,,\n' "$i" "$u" "$u" "${HEAP[$u]}" "${STACK[$u]}"; i=$((i+1)); done; } > "$W/l/corpus/tests/snobol4/ALL.csv"
LSCRIPTS="$HERE"
if [ "${FAIL_ONCE:-0}" = 1 ]; then
  mkdir -p "$W/fl"; cp -rs "$HERE" "$W/fl/scripts" 2>/dev/null; rm -f "$W/fl/scripts/lib_declared_arena.sh"
  { cat "$HERE/lib_declared_arena.sh"; printf '\ndeclared_switches_beside() { return 0; }  # FAIL_ONCE: the reader removed\n'; } > "$W/fl/scripts/lib_declared_arena.sh"
  LSCRIPTS="$W/fl/scripts"; echo "FAIL_ONCE=1: the ladder family runs with declared_switches_beside removed"
fi
ol=$(S4E_HOME="$W/l" SCRIP="$SCRIP" RT_DIR="$RT" timeout 600 bash "$LSCRIPTS/test_snobol4_ladder.sh" 2>&1); rl=$?
[ "$rl" = 2 ] && echo "      $(grep -m1 -E 'REFUS' <<<"$ol" | cut -c1-200)"
# the ladder appends no progress rows; it prints one line per witness, "rung NN  ladder__rung00_<unit>  m3=<v> m4=<v> (...)"
{ printf 'ts\tscrip\tcorpus\tmeasurer\tclass\tsuite\tlang\tprogram\tmode\toutcome\n'
  for u in $UNITS; do ln=$(grep -m1 -E "^rung +0 +ladder__rung00_$u " <<<"$ol"); for m in m3 m4; do
    v=$(sed -n "s/.* $m=\([A-Z]*\).*/\1/p" <<<"$ln"); printf 'x\tx\tx\tx\tx\tl\tsnobol4\t%s\t%s\t%s\n' "$u" "$m" "$v"; done; done; } > "$W/l.tsv"
quad L "$W/l.tsv" "test_snobol4_ladder.sh (lib_ladder.sh) over a scratch rungs table (rc $rl)"

echo "--- PB: the Prolog bench family over a scratch kernel dir -- test_prolog_bench_suite.sh (the row runner), test_bench_prolog_modes.sh, test_bench_prolog_4way.sh ---"
# Prolog units, since the SNOBOL4 witnesses cannot run here. MEASURED 2026-10-01 at c1c899df3, m3 and m4: d/2 300000 deep overflows the 4 MB
# default and passes at -s262144k; a findall of 30000 x/2 terms reaches the HARD CAP at -d2048k and passes at the default cap. Both live units
# declare -s65536k because findall and length over 30000 elements also need more than 4 MB of stack, so the live pair differs in heap alone.
declare -A PHEAP=([deepdecl]=131072 [deepnodecl]=131072 [livedecl]=2048 [livenodecl]=131072)
declare -A PSTACK=([deepdecl]=262144 [deepnodecl]=4096 [livedecl]=65536 [livenodecl]=65536)
mkdir -p "$W/pb"
for u in $UNITS; do
  case "$u" in deep*) body=$'bench_work(R) :- d(300000, R).\nd(0, 0) :- !.\nd(N, R) :- M is N - 1, d(M, R0), R is R0 + 1.'; r=300000 ;;
               *) body='bench_work(N) :- findall(x(I, abcdefgh), between(1, 30000, I), L), length(L, N).'; r=30000 ;; esac
  printf '%% *BENCH kernel=%s\n:- initialization(main).\n%s\nmain :- bench_work(Res), write(Res), nl.\n' "$u" "$body" > "$W/pb/$u.pl"
  echo "$r" > "$W/pb/$u.ref"; printf '%s\t%s\n' "$u" "${PHEAP[$u]}" > "$W/pb/$u.heap"; printf '%s\t%s\n' "$u" "${PSTACK[$u]}" > "$W/pb/$u.stack"
done
pbtab() {  # <runner output> <awk program printing the verdict of unit u in mode m> -- the quad table from a runner's printed rows
  local u m; printf 'ts\tscrip\tcorpus\tmeasurer\tclass\tsuite\tlang\tprogram\tmode\toutcome\n'
  for u in $UNITS; do for m in m3 m4; do printf 'x\tx\tx\tx\tx\tpb\tprolog\t%s\t%s\t%s\n' "$u" "$m" "$(awk -v u="$u" -v m="$m" "$2" <<<"$1")"; done; done; }
os=$(BENCH_PROLOG_DIR="$W/pb" S4E_PROGRESS_DB="$W/pbs.db" SCRIP="$SCRIP" RT_DIR="$RT" timeout 900 bash "$HERE/test_prolog_bench_suite.sh" 2>&1); rs=$?
[ "$rs" = 2 ] && echo "      $(grep -m1 -E 'REFUS' <<<"$os" | cut -c1-200)"
pbtab "$os" '$1==u && $2==m {print $6; exit}' > "$W/pbs.tsv"
quad PBS "$W/pbs.tsv" "test_prolog_bench_suite.sh over a scratch kernel dir, SCRIP_HEAP_CAP_KB and SCRIP_STACK (rc $rs)"
om=$(BENCH_DIR="$W/pb" SCRIP="$SCRIP" RT_DIR="$RT" timeout 900 bash "$LSCRIPTS/test_bench_prolog_modes.sh" 2>&1); rmo=$?
pbtab "$om" '$1==u {print (m=="m3" ? $2 : $3); exit}' > "$W/pbm.tsv"
quad PBM "$W/pbm.tsv" "test_bench_prolog_modes.sh over a scratch kernel dir, -d/-s switches (rc $rmo)"
o4=$(BENCH_DIR="$W/pb" SCRIP="$SCRIP" RT_DIR="$RT" timeout 900 bash "$LSCRIPTS/test_bench_prolog_4way.sh" 2>&1); r4=$?
pbtab "$o4" '$1==u {print (m=="m3" ? $4 : $5); exit}' > "$W/pb4.tsv"
quad PB4 "$W/pb4.tsv" "test_bench_prolog_4way.sh over a scratch kernel dir, -d/-s switches (rc $r4)"

echo "--- PENDING: runner families not yet fixtured here -- RED by declaration until each is added as its lane cures its runner ---"
PENDING="snoflake, spitbol_x64, spitbol_x32, testpgms, aisnobol, gimpel/scorecard (hq_snobol4) | arizona, jcon, ipl, the icon bench suite and triangulator, the icon rung suites (hq_icon) | inria, swi, gnu, logtalk, the prolog bench timing angles (bench_prolog_fixed_iter, test_bench_prolog_timed, bench_prolog_perf, bench_prolog_vanroy --two-number), the prolog rung suite (hq_prolog) | fpc, pat, the pascal benches (hq_pascal) | roast, the raku benches (hq_raku) | the snocone and rebus benches (hq_snocone) | board_icon_rungs.sh, the smokes, monitor_run.sh, lib_port_trace.sh (the coo)"
n_pend=$(tr '|' '\n' <<<"$PENDING" | grep -c .)
echo "  PENDING ($n_pend groups): $PENDING"
FAIL=$((FAIL+n_pend))

echo "------------------------------------------------------------"
echo "population: $((PASS+FAIL)) verdict(s): the premise, 8 families (H D P1 P2 L PBS PBM PB4) x 4 units x 2 modes, and $n_pend pending family group(s) counted red"
if [ "$FAIL" -eq 0 ]; then echo "GATE PASS [$GATE_NAME]: every runner family passes each unit's declared heap and stack in both modes"; gate_stamp; exit 0; fi
echo "⛔ GATE FAIL [$GATE_NAME]: $FAIL red (pairs that failed, and $n_pend family group(s) not yet fixtured)"; gate_stamp; exit 1
