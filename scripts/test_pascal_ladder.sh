#!/usr/bin/env bash
# test_pascal_ladder.sh -- THE CONSTRUCT-LADDER RUNNER for Pascal (GOAL-TEST-SUITE-CONSISTENCY.md, row
# pascal-smoke-floor-gate-and-construct-ladder-from-rung-0; shape copied from test_prolog_ladder.sh via
# test_raku_ladder.sh, minted by hq_B 2026-09-02).
#   --to N     grade rungs 0..N CUMULATIVELY (default: every rung the master carries)
#   --only N   grade rung N ALONE -- the rung under construction, while the rungs below it are still red.
#              Mutually exclusive with --to: a request naming both REFUSES rc=2 rather than guessing.
#   --list     print the witnesses per rung and exit 0 without grading
# POPULATION: every origin `ladder__rungNN_<slug>` of corpus/tests/pascal/ALL.csv with NN <= N, materialized
# OUT of the master by origin through lib_master_extract.sh (MASTER_DIR=corpus/tests/pascal, MASTER_EXT=.pas)
# -- keyed on the CSV `origin` column, never on the entry name or a filename glob.
# EACH WITNESS RUNS ALONE IN BOTH MODES:  m3 = scrip --run   ·   m4 = scrip --compile -o x.s + as --64 + gcc -no-pie + run
# Verdict per witness per mode: stdout byte-equal to the .ref AND rc equal to the declared rc (ALL.wantrc,
# keyed on the entry NAME; default 0, and the file need not exist -- every rung 0-9 witness here exits 0).
# A timeout is a FAIL(rc=124), never a hang.
# ⭐ REFS ARE ORACLE-CUT, NOT HAND-AUTHORED: every .ref line for a ladder witness is the literal captured
# stdout of the real `fpc -Miso` oracle (FPC 3.2.2, ISO 7185 mode -- GOAL-CEO CEO-33) on that exact source.
# rung09_strings uses `packed array[1..N] of char` (ISO 7185's actual string mechanism), never Delphi's
# `string` type, which -Miso itself rejects -- confirmed directly (`Error: Identifier not found "string"`)
# before writing the witness, not assumed. This runner does not re-invoke fpc at grading time (same reason
# test_prolog_ladder.sh does not re-invoke swipl/gprolog per run): the .ref is the recorded oracle answer,
# cut once, and the runner's only job is diffing SCRIP against it. Re-cut a rung's .ref only when its
# witness source changes.
# ⛔ XFAIL MARKERS ARE IGNORED HERE ON PURPOSE, same as the Prolog/Raku runners: this is the gate for the
# rung that OWNS the witness, and a rung is landed only when its witnesses PASS.
# PRINTS ITS DENOMINATOR. REFUSES rc=2 when it graded ZERO witnesses (no master, no ladder origins at or
# below --to N, or an extraction failure) -- a runner that cannot measure never prints the success shape.
# FAILED ONCE, PASSED ONCE (INSTRUMENT LAWS): rungs 0-9 (10 witnesses, one per rung) built and verified
# against `fpc -Miso` 2026-09-03 by seat10. `--to 5` PASS 12/12 witness x mode. rung09_strings is a KNOWN
# RED rung (`--only 9` FAILs both modes) -- `a = 'foo'` (packed-array-of-char equality) hits SCRIP's
# "Run-time error 102 / numeric expected" (same class as the already-minted row
# pascal-fpc-class-runtime-102-numeric-expected; isolated directly: assignment+writeln alone passes,
# only the `=` comparison breaks) -- left red deliberately, not routed around, per this ladder's own
# purpose: a construct SCRIP does not yet support is exactly what a rung is for.
# EXIT: 0 every graded witness PASS in both modes · 1 any FAIL · 2 REFUSED (cannot measure).
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP:-$ROOT/scrip}"; RT="${RT_DIR:-$ROOT/out}"; T="${TIMEOUT:-20}"
MASTER_DIR="$S4E/corpus/tests/pascal"; MASTER_EXT=.pas; export MASTER_DIR MASTER_EXT
TO=""; ONLY=""; LIST=0
while [ $# -gt 0 ]; do
  case "$1" in
    --to) shift; TO="${1:-}";;
    --to=*) TO="${1#--to=}";;
    --only) shift; ONLY="${1:-}";;
    --only=*) ONLY="${1#--only=}";;
    --list) LIST=1;;
    -h|--help) sed -n '2,20p' "$0"; exit 0;;
    *) echo "REFUSE (rc=2): unknown argument '$1' (usage: $0 [--to N | --only N] [--list])"; exit 2;;
  esac; shift
done
case "$TO" in ""|*[!0-9]*) [ -z "$TO" ] || { echo "REFUSE (rc=2): --to wants a rung number, got '$TO'"; exit 2; };; esac
case "$ONLY" in ""|*[!0-9]*) [ -z "$ONLY" ] || { echo "REFUSE (rc=2): --only wants a rung number, got '$ONLY'"; exit 2; };; esac
[ -z "$TO" ] || [ -z "$ONLY" ] || { echo "REFUSE (rc=2): --to $TO and --only $ONLY name two different populations (cumulative 0..N vs rung N alone) -- pass one, never both"; exit 2; }
if [ -n "$ONLY" ]; then SEL="--only $ONLY"; else SEL="--to ${TO:-max}"; fi
refuse() { echo "REFUSE (rc=2): $*"; exit 2; }
[ -x "$SCRIP" ] || refuse "scrip binary not built at $SCRIP"
[ -f "$RT/libscrip_rt.so" ] || refuse "runtime library missing at $RT/libscrip_rt.so"
[ -f "$MASTER_DIR/ALL.pas" ] && [ -f "$MASTER_DIR/ALL.ref" ] && [ -f "$MASTER_DIR/ALL.csv" ] || refuse "Pascal master suite missing under $MASTER_DIR (ALL.pas / ALL.ref / ALL.csv)"
. "$HERE/lib_master_extract.sh" || refuse "cannot source lib_master_extract.sh"
all_origins=$(master_origins_of_family ladder 2>/dev/null) || all_origins=""
[ -n "$all_origins" ] || refuse "no \`ladder\` origins in $MASTER_DIR/ALL.csv -- the witnesses moved or were never absorbed; re-point, never skip"
declare -a origins=()
for o in $(printf '%s\n' $all_origins | sort); do
  r=$(printf '%s' "$o" | sed -nE 's/^ladder__rung([0-9]+)_.*$/\1/p'); [ -n "$r" ] || continue
  r=$((10#$r)); if [ -n "$ONLY" ]; then [ "$r" -eq "$ONLY" ] || continue; else [ -z "$TO" ] || [ "$r" -le "$TO" ] || continue; fi
  origins+=("$r $o")
done
[ "${#origins[@]}" -gt 0 ] || refuse "zero ladder__rungNN_* origins selected by $SEL in $MASTER_DIR/ALL.csv (family present, rung filter empty) -- a rung with no witness is UNMEASURED, never a pass"
if [ "$LIST" = 1 ]; then printf '%s\n' "${origins[@]}" | sort -n | awk '{printf "rung %2d  %s\n", $1, $2}'; echo "witnesses=${#origins[@]} ($SEL)"; exit 0; fi
wantrc() { local n="$1"; [ -f "$MASTER_DIR/ALL.wantrc" ] || { echo 0; return; }; local v; v=$(awk -F'\t' -v n="$n" '$1==n{print $2; exit}' "$MASTER_DIR/ALL.wantrc"); printf '%s\n' "${v:-0}"; }
W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
n=0; pass=0; fail=0; declare -A rp rf
for pair in $(printf '%s\n' "${origins[@]}" | sort -n | tr ' ' ':'); do
  r=${pair%%:*}; o=${pair#*:}; src="$W/$o.pas"; ref="$W/$o.ref"
  master_extract_origin "$o" "$src" "$ref" >/dev/null 2>&1 || refuse "cannot extract $o from the master suite (lib_master_extract.sh)"
  name=$(master_entry_for_origin "$o") || refuse "no entry name for origin $o"
  want=$(wantrc "$name"); n=$((n+1))
  r3=$( ( timeout "$T" "$SCRIP" --run "$src" </dev/null >"$W/$o.m3.out" 2>/dev/null; echo $? ) 2>/dev/null )
  if [ "$r3" = "$want" ] && cmp -s "$W/$o.m3.out" "$ref"; then v3=PASS; pass=$((pass+1)); rp[$r]=$(( ${rp[$r]:-0} + 1 )); else v3="FAIL(rc=$r3)"; fail=$((fail+1)); rf[$r]=$(( ${rf[$r]:-0} + 1 )); fi
  v4=NOBUILD
  if (cd "$W" && timeout "$T" "$SCRIP" --compile -o "$o.s" "$src" </dev/null >/dev/null 2>&1) && [ -s "$W/$o.s" ] \
     && as --64 -o "$W/$o.o" "$W/$o.s" 2>/dev/null && gcc -no-pie -o "$W/$o.bin" "$W/$o.o" "$RT/libscrip_rt.so" -lm -lstdc++ -Wl,-rpath,"$RT" 2>/dev/null; then
    r4=$( ( timeout "$T" "$W/$o.bin" </dev/null >"$W/$o.m4.out" 2>/dev/null; echo $? ) 2>/dev/null )
    if [ "$r4" = "$want" ] && cmp -s "$W/$o.m4.out" "$ref"; then v4=PASS; else v4="FAIL(rc=$r4)"; fi
  fi
  if [ "$v4" = PASS ]; then pass=$((pass+1)); rp[$r]=$(( ${rp[$r]:-0} + 1 )); else fail=$((fail+1)); rf[$r]=$(( ${rf[$r]:-0} + 1 )); fi
  printf 'rung %2d  %-44s m3=%-12s m4=%-12s (%s, want rc=%s)\n' "$r" "$o" "$v3" "$v4" "$name" "$want"
done
for r in $(printf '%s\n' "${!rp[@]}" "${!rf[@]}" | sort -nu); do printf 'rung %2d summary: PASS=%d FAIL=%d (witness x mode)\n' "$r" "${rp[$r]:-0}" "${rf[$r]:-0}"; done
echo "LADDER $SEL: witnesses=$n modes=2 (m3 --run, m4 --compile+as+gcc) graded=$((n*2)) PASS=$pass FAIL=$fail  tree: SCRIP=$(git -C "$ROOT" rev-parse --short HEAD 2>/dev/null || echo ?)$(git -C "$ROOT" status --short 2>/dev/null | grep -q . && echo -DIRTY) corpus=$(git -C "$S4E/corpus" rev-parse --short HEAD 2>/dev/null || echo ?)$(git -C "$S4E/corpus" status --short 2>/dev/null | grep -q . && echo -DIRTY)"
[ "$n" -gt 0 ] || refuse "graded ZERO witnesses -- cannot measure, not a pass"
[ "$fail" -eq 0 ] && { if [ -n "$ONLY" ]; then echo "✅ LADDER OK: rung $ONLY alone PASS $pass/$((n*2))"; else echo "✅ LADDER OK: rungs 0..${TO:-max} PASS $pass/$((n*2))"; fi; exit 0; }
echo "⛔ LADDER RED: $fail of $((n*2)) witness x mode gradings FAIL"; exit 1
