#!/usr/bin/env bash
# util_sweep_fold_arm_refs.sh -- SWEEP EVERY `.ref` IN corpus/ FOR A PIN THAT CAN ONLY HAVE COME FROM THE
# CASE-FOLDING ORACLE ARM (row `pre-s189-ref-sweep`, hq_C s265 FINDING: a `.ref` minted against `sbl -b`
# instead of the mandated `sbl -bf` agrees with a case-folding bug FOREVER -- exactly what happened to
# `corpus/demo/json.ref`, pinned `root=JOBJ` for a `DATATYPE()` bug that upper-cased
# programmer-defined type names, invisible to every grading pass because the WRONG oracle arm minted the
# pin the RIGHT arm's own bug agreed with).
#
#   bash scripts/util_sweep_fold_arm_refs.sh                # sweep everything, report, write nothing
#   bash scripts/util_sweep_fold_arm_refs.sh --dir <d>       # restrict to one directory (repeatable)
#
# ⛔⛔ v1 OF THIS SCRIPT SHIPPED BLIND AND REPORTED CLEAN WHILE BLIND (hq_C, same session, negative-control
# catch). v1 captured the oracle with `2>&1`, merging stderr into the comparison stream. `json.sno` -- the
# ROW'S OWN NAMED WITNESS -- deliberately writes `match_ms=0` to stderr (TERMINAL) so stdout stays
# byte-comparable against `.ref`; v1's merge appended that line to every captured run, so `cmp` against the
# pin could never match ANY file that writes to stderr, and the subsequent upper-case compare also couldn't
# match (one side carries an extra line no case-fold could remove). hq_C proved this by restoring the
# historical folded pin (`root=JOBJ`) and re-running v1: it reported CASE-ONLY-DIVERGENCE: 0, identical to
# the clean-corpus run -- an instrument that reports the same answer whether or not the defect is present is
# not measuring the defect. v2 (this file) fixes the capture (stdout only feeds the comparison; stderr is
# kept in its own file for the liveness checks that need it) AND, because "the sweep looked clean" was
# exactly the false signal that let json.ref hide for months, v2 REFUSES TO REPORT A SWEEP RESULT AT ALL
# unless it first proves, on a live fixture derived from the corpus's own historical witness, that it CAN
# still detect a case-only-folded pin. An instrument that has never been shown to find anything has not been
# shown to work -- this rule applies to this script as much as to any `.ref`.
#
# METHOD (hq_C, verbatim in substance): "re-derive, then diff; do not read for plausibility." For every
# `.sno` with a sibling `.ref`, re-run it under `sbl -bf` (the ONE correctness arm, s189/lib_oracle_flags.sh)
# and compare STDOUT ONLY (never stderr -- a `.ref` pins stdout; a program that deliberately separates a
# non-deterministic or diagnostic line onto stderr, as `json.sno`/`calculator-1.sno`/`calculator-2.sno` all
# do, is not a defect in the program). A MISMATCH is triaged: does it disappear when BOTH sides are
# upper-cased? If yes, the two texts are identical except for letter case -- the exact, general signature of
# a folding-arm-minted pin. A mismatch that survives upper-casing is a DIFFERENT class of divergence and is
# reported individually, never folded into a dismissed "out of scope" bucket -- hq_C's second catch: that
# bucket held 14 of 25 files in one directory in v1, almost entirely the same stderr-merge artifact, and a
# bucket that large dropped by category rather than by name is exactly how a sweep reports clean.
#
# ⛔ `corpus/lon/` IS REFUSED BY CONSTRUCTION (RULES.md credential rule): never walked, never read.
# ⛔ THIS SWEEP DOES NOT BLANKET-OVERWRITE ANYTHING. It only reports. A `.ref` may legitimately record
# behaviour `sbl` itself fails on -- every file this script flags needs its own witness read before any
# change, never a bulk `--write`.
# ⛔ STDIN: `<prog>.input` -> `<prog>.in` -> (strip a trailing `-match`/`-match-fence` and retry both
# extensions, the FAMILY input -- `json-match` takes `json.input`, per `ref_stdin()` in
# `util_mint_bench_refs.sh`) -> /dev/null. Missing this (v1's bug #3) silently ran family variants on empty
# stdin, which cannot produce anything resembling their `.ref` for reasons that have nothing to do with
# folding, and buried them in the same dismissed bucket as bug #1.
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; SC="$(cd "$HERE/.." && pwd)"
S4E="${S4E_HOME:-$(cd "$SC/.." && pwd)}"; S4A="${S4E_ASSETS:-$([ -d "$S4E/x64" ] && echo "$S4E" || echo /home/resources)}"
. "$HERE/lib_oracle_flags.sh" 2>/dev/null || { echo "REFUSING: cannot load lib_oracle_flags.sh -- the ONE oracle-flag authority (s200)." >&2; exit 3; }
SBL="${SBL:-$S4A/x64/bin/sbl}"; CORPUS="${CORPUS:-$S4E/corpus}"; T="${SWEEP_TIMEOUT:-20}"
[ -x "$SBL" ] || { echo "⛔ ORACLE MISSING: $SBL" >&2; exit 3; }
sbl_died() { grep -qE ' : ERROR [0-9][0-9][0-9] -- ' "$1" && grep -qE '^in statement +[0-9]+$' "$1"; }  # scorecard_snobol4.sh's own liveness check, copied not re-derived
DIRS=""
while [ $# -gt 0 ]; do case "$1" in --dir) DIRS="$DIRS $2"; shift 2;; *) echo "usage: $0 [--dir <d>]..." >&2; exit 2;; esac; done
[ -n "$DIRS" ] || DIRS="$CORPUS"
# ---------------------------------------------------------------- stdin resolution -- COMBINES BOTH existing
# authorities rather than re-deriving a third: `util_mint_bench_refs.sh`'s ref_stdin() (strip a trailing
# -match/-match-fence and retry, for corpus/benchmarks/snobol4) AND `scorecard_snobol4.sh`'s stdin_for()
# (demo's numbered/lettered variants -- calculator-1, calculator-2, treebank-alloc -- all
# share ONE family .input keyed by name PREFIX, a different convention the suffix-strip alone does not
# cover: hq_C's catch #3, "without it those programs run on /dev/null" -- confirmed, this is exactly what
# put calculator-1/-2/treebank-alloc in v1's other-diff bucket).
DEMO="$CORPUS/demo"
ref_stdin() {
  local p="$1" d n f; d="$(dirname "$p")"; n="$(basename "${p%.sno}")"
  [ -f "$d/$n.input" ] && { echo "$d/$n.input"; return; }
  [ -f "$d/$n.in" ] && { echo "$d/$n.in"; return; }
  f="$(sed 's/-match\(-fence\)\?$//' <<<"$n")"
  [ -f "$d/$f.input" ] && { echo "$d/$f.input"; return; }
  [ -f "$d/$f.in" ] && { echo "$d/$f.in"; return; }
  [ -f "$d/$n.dat" ] && { echo "$d/$n.dat"; return; }
  if [ "$d" = "$DEMO" ]; then
    case "$n" in
      claws5*) echo "$DEMO/claws5.input"; return;; treebank*) echo "$DEMO/treebank.input"; return;;
      json*) echo "$DEMO/json.input"; return;; calculator*) echo "$DEMO/calculator.input"; return;;
      porter*) echo "$DEMO/porter.input"; return;;
    esac
  fi
  echo /dev/null
}
# ---------------------------------------------------------------- ONE check, used by both the self-test and the real sweep -- the self-test is worthless if it exercises different code
# echoes MATCH | CASE_ONLY | OTHER_DIFF | SKIP. $3 is a scratch dir the caller owns (not cleaned here).
check_one() {
  local sno="$1" ref="$2" w="$3" in pd rc
  in="$(ref_stdin "$sno")"; pd="$(dirname "$sno")"
  ( cd "$pd" && timeout "$T" "$SBL" $(sbl_lang_flags) -d512m -i64m "$(basename "$sno")" < "$in" > "$w/o.txt" 2> "$w/e.txt" ); rc=$?
  # norm=ms (scorecard_snobol4.sh's own convention, copied not re-derived): a probe/benchmark that prints its
  # own iters:/ns:/ms: timing lines pins only the deterministic check: line -- these lines are never part of
  # the .ref by construction, so comparing them raw always "differs" regardless of correctness. Strip both
  # sides identically; a no-op for any file that never had them.
  sed -i '/^iters: [0-9][0-9]*$/d; /^ns: [0-9][0-9]*$/d; /^ms: [0-9][0-9]*$/d' "$w/o.txt"
  sed '/^iters: [0-9][0-9]*$/d; /^ns: [0-9][0-9]*$/d; /^ms: [0-9][0-9]*$/d' "$ref" > "$w/ref.norm"; ref="$w/ref.norm"
  # trailing-newline-only is not a content divergence (8 crosscheck/patterns witnesses found this session are
  # byte-identical content, ref just saved without the final newline SNOBOL4's OUTPUT always emits) -- ADD a
  # newline to whichever side lacks one rather than stripping, so a genuine multi-blank-line difference
  # elsewhere in the file is never silently absorbed. Orthogonal to the case-fold signal (folding cannot
  # create or remove a trailing newline), so this cannot mask what this sweep exists to find.
  for _f in "$w/o.txt" "$ref"; do [ -s "$_f" ] && [ "$(tail -c1 "$_f")" != "" ] && printf '\n' >> "$_f"; done
  if cmp -s "$w/o.txt" "$ref"; then echo MATCH; return; fi
  if [ "$rc" != 0 ] || grep -qE '\.sno\([0-9]+\) : ERROR' "$w/o.txt" "$w/e.txt" 2>/dev/null || sbl_died "$w/o.txt" 2>/dev/null || sbl_died "$w/e.txt" 2>/dev/null; then
    echo SKIP; return
  fi
  tr 'a-z' 'A-Z' < "$w/o.txt" > "$w/o.up"; tr 'a-z' 'A-Z' < "$ref" > "$w/r.up"
  if cmp -s "$w/o.up" "$w/r.up"; then echo CASE_ONLY; else echo OTHER_DIFF; fi
}
# ---------------------------------------------------------------- POSITIVE CONTROL: prove this instrument can still detect the row's own historical defect before trusting anything it reports
selftest() {
  local st witness win rc
  st="$(mktemp -d)"; witness="$CORPUS/demo/json.sno"; win="$CORPUS/demo/json.input"
  if [ ! -f "$witness" ] || [ ! -f "$win" ]; then echo "⛔ SELF-TEST SKIPPED: witness $witness or its .input is missing" >&2; rm -rf "$st"; return 2; fi
  ( cd "$(dirname "$witness")" && timeout "$T" "$SBL" $(sbl_lang_flags) -d512m -i64m "$(basename "$witness")" < "$win" ) > "$st/true.ref" 2>/dev/null
  [ -s "$st/true.ref" ] || { echo "⛔ SELF-TEST SKIPPED: could not derive a live baseline for $witness" >&2; rm -rf "$st"; return 2; }
  sed 's/^root=jobj$/root=JOBJ/' "$st/true.ref" > "$st/contaminated.ref"
  if cmp -s "$st/true.ref" "$st/contaminated.ref"; then
    echo "⛔ SELF-TEST INCONCLUSIVE: the fixture mutation was a no-op -- witness's own output shape moved (no 'root=jobj' line found), update the fixture, do not trust an unproven sweep" >&2
    rm -rf "$st"; return 2
  fi
  local verdict; verdict="$(check_one "$witness" "$st/contaminated.ref" "$st")"
  rm -rf "$st"
  [ "$verdict" = CASE_ONLY ]
}
echo "positive-control self-test (json.sno with a fabricated root=JOBJ pin, the row's own historical defect shape) ..."
if ! selftest; then
  echo "⛔⛔⛔ SELF-TEST FAILED -- this script could NOT detect a known-contaminated fixture. REFUSING to report a sweep result: an unproven instrument reporting 'clean' is worse than no result. Fix check_one/ref_stdin before trusting this script's output for anything." >&2
  exit 3
fi
echo "self-test PASSED -- proceeding to the real sweep."
# ---------------------------------------------------------------- the real sweep
W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
tot=0; match=0; case_only=0; other_diff=0; skip=0
CASE_HITS=""; OTHER=""; SKIPPED=""
for d in $DIRS; do
  [ -e "$d" ] || continue
  for sno in $(find "$d" -name "*.sno" -not -path "$CORPUS/lon/*" | sort); do
    ref="${sno%.sno}.ref"; [ -f "$ref" ] || continue
    tot=$((tot+1))
    v="$(check_one "$sno" "$ref" "$W")"
    case "$v" in
      MATCH) match=$((match+1));;
      CASE_ONLY) case_only=$((case_only+1)); CASE_HITS="$CASE_HITS\n$sno";;
      OTHER_DIFF) other_diff=$((other_diff+1)); OTHER="$OTHER\n$sno";;
      SKIP) skip=$((skip+1)); SKIPPED="$SKIPPED\n$sno";;
    esac
  done
done
echo "programs-with-ref: $tot   oracle-match: $match   CASE-ONLY-DIVERGENCE: $case_only   other-diff (needs individual triage, NOT auto-dismissed): $other_diff   skip (oracle unrunnable/errored): $skip"
if [ -n "$CASE_HITS" ]; then printf '\n⛔⛔ CASE-ONLY DIVERGENCE (folding-arm signature -- investigate before touching):%b\n' "$CASE_HITS"; fi
if [ -n "$OTHER" ]; then printf '\n(other-diff -- each needs its own reason on record, not a category dismissal):%b\n' "$OTHER"; fi
if [ -n "$SKIPPED" ] && [ -n "${SWEEP_SHOW_SKIPPED:-}" ]; then printf '\n(skip -- oracle could not run cleanly):%b\n' "$SKIPPED"; fi
[ "$case_only" -eq 0 ]
