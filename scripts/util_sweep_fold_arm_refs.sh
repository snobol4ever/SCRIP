#!/usr/bin/env bash
# util_sweep_fold_arm_refs.sh -- SWEEP EVERY `.ref` IN corpus/ FOR A PIN THAT CAN ONLY HAVE COME FROM THE
# CASE-FOLDING ORACLE ARM (row `pre-s189-ref-sweep`, hq_C s265 FINDING: a `.ref` minted against `sbl -b`
# instead of the mandated `sbl -bf` agrees with a case-folding bug FOREVER -- exactly what happened to
# `corpus/programs/snobol4/demo/json.ref`, pinned `root=JOBJ` for a `DATATYPE()` bug that upper-cased
# programmer-defined type names, invisible to every grading pass because the WRONG oracle arm minted the
# pin the RIGHT arm's own bug agreed with).
#
#   bash scripts/util_sweep_fold_arm_refs.sh                # sweep everything, report, write nothing
#   bash scripts/util_sweep_fold_arm_refs.sh --dir <d>       # restrict to one directory (repeatable)
#
# METHOD (hq_C, verbatim in substance): "re-derive, then diff; do not read for plausibility." For every
# `.sno` with a sibling `.ref`, re-run it under `sbl -bf` (the ONE correctness arm, s189/lib_oracle_flags.sh)
# and compare. A byte-identical match needs no further look. A MISMATCH is triaged into exactly the bucket
# that matters here: does the mismatch disappear when BOTH sides are upper-cased? If yes, the two texts are
# identical except for letter case -- the exact, general signature of a folding-arm-minted pin (SPITBOL's
# folding arm upper-cases names/labels and the special `output`/`input` keywords; a quoted string LITERAL is
# never folded, so this signature cannot arise from any other known mechanism). If a mismatch survives
# upper-casing, it is a DIFFERENT class of divergence (a real SCRIP defect already tracked elsewhere, a
# pre-existing oracle/harness incompatibility, or a deliberately SCRIP-pinned `.ref` self-documented as such)
# and is out of scope for this sweep -- reported separately, never silently folded into the case-only bucket.
#
# ⛔ `corpus/programs/lon/` IS REFUSED BY CONSTRUCTION (RULES.md credential rule): never walked, never read.
# ⛔ THIS SWEEP DOES NOT BLANKET-OVERWRITE ANYTHING. It only reports. A `.ref` may legitimately record
# behaviour `sbl` itself fails on (a SCRIP-only extension, a stale include-path a historical test assumed) --
# every file this script flags needs its own witness read before any change, never a bulk `--write`.
# ⛔ STDIN: `<prog>.input` -> `<prog>.in` -> /dev/null, mirroring `scorecard_snobol4.sh`'s `stdin_for()`. A
# handful of corpus programs need argv, a library search path beyond their own directory, or multi-file
# concatenation this sweep does not attempt (confirmed against `scorecard_snobol4.sh oracle` for the specific
# csnobol4-suite witnesses this sweep's own FINDING names: identical RC/byte-count through both paths) --
# those land in the SKIP bucket by their failure signature, not silently misclassified as a match.
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; SC="$(cd "$HERE/.." && pwd)"
S4E="${S4E_HOME:-$(cd "$SC/.." && pwd)}"; S4A="${S4E_ASSETS:-$([ -d "$S4E/x64" ] && echo "$S4E" || echo /home/resources)}"
. "$HERE/lib_oracle_flags.sh" 2>/dev/null || { echo "REFUSING: cannot load lib_oracle_flags.sh -- the ONE oracle-flag authority (s200)." >&2; exit 3; }
SBL="${SBL:-$S4A/x64/bin/sbl}"; CORPUS="${CORPUS:-$S4E/corpus}"
[ -x "$SBL" ] || { echo "⛔ ORACLE MISSING: $SBL" >&2; exit 3; }
DIRS=""
while [ $# -gt 0 ]; do case "$1" in --dir) DIRS="$DIRS $2"; shift 2;; *) echo "usage: $0 [--dir <d>]..." >&2; exit 2;; esac; done
[ -n "$DIRS" ] || DIRS="$CORPUS"
W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
tot=0; match=0; case_only=0; other_diff=0; skip=0
CASE_HITS=""; OTHER=""; SKIPPED=""
for d in $DIRS; do
  [ -e "$d" ] || continue
  for sno in $(find "$d" -name "*.sno" -not -path "$CORPUS/programs/lon/*" | sort); do
    ref="${sno%.sno}.ref"; [ -f "$ref" ] || continue
    tot=$((tot+1))
    in="${sno%.sno}.input"; [ -f "$in" ] || in="${sno%.sno}.in"; [ -f "$in" ] || in=/dev/null
    pd="$(dirname "$sno")"
    ( cd "$pd" && timeout 5 "$SBL" $(sbl_lang_flags) -d512m -i64m "$(basename "$sno")" < "$in" ) > "$W/o.txt" 2>&1
    rc=$?
    if cmp -s "$W/o.txt" "$ref"; then match=$((match+1)); continue; fi
    if [ "$rc" != 0 ] || grep -qE '\.sno\([0-9]+\) : ERROR' "$W/o.txt" || sbl_died "$W/o.txt" 2>/dev/null; then
      skip=$((skip+1)); SKIPPED="$SKIPPED\n$sno (rc=$rc)"; continue
    fi
    tr 'a-z' 'A-Z' < "$W/o.txt" > "$W/o.up"; tr 'a-z' 'A-Z' < "$ref" > "$W/r.up"
    if cmp -s "$W/o.up" "$W/r.up"; then
      case_only=$((case_only+1)); CASE_HITS="$CASE_HITS\n$sno"
    else
      other_diff=$((other_diff+1)); OTHER="$OTHER\n$sno"
    fi
  done
done
echo "programs-with-ref: $tot   oracle-match: $match   CASE-ONLY-DIVERGENCE: $case_only   other-diff (out of scope): $other_diff   skip (oracle unrunnable/errored): $skip"
if [ -n "$CASE_HITS" ]; then printf '\n⛔⛔ CASE-ONLY DIVERGENCE (folding-arm signature -- investigate before touching):%b\n' "$CASE_HITS"; fi
if [ -n "$OTHER" ]; then printf '\n(other-diff, NOT this sweep''s bug class, not further triaged here):%b\n' "$OTHER"; fi
[ "$case_only" -eq 0 ]
