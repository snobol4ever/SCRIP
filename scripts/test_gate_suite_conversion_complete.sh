#!/bin/bash
# test_gate_suite_conversion_complete.sh <lang> -- ONE DONE-WHEN authority for the six tests-consolidate-* rows.
#
# ⭐ WHY THIS EXISTS (hq_C ruling 2026-08-27, on seat02's question, corroborated by seat14's measurement).
# The fan-out rows shipped with two independent defects in their DONE-WHENs, and each is trap #1 -- a criterion
# that can never say YES:
#   (1) FOUR of the six (pascal/prolog/rebus/snocone) test `find tests/<lang> -name '*.<ext>' | wc -l -eq 0`.
#       But the SETTLED architecture puts converted suite output INSIDE tests/<lang>/ WITH THE REAL EXTENSION --
#       live proof: corpus/tests/snobol4/crosscheck/patterns.sno, 44KB, pushed. So a successful conversion makes
#       that count go UP, and the criterion is unsatisfiable BY DOING THE WORK CORRECTLY. Measured at ruling
#       time: `find tests/snobol4 -name '*.sno'` = 187, against a criterion demanding 0.
#   (2) The one row that DID exclude the suite subdir hardcoded a keeper allowance (`-le 12`). seat14 then
#       measured the real number: 34 -- all of beauty_suite is STANDALONE-KEEP. A guessed keeper count is a
#       second unsatisfiable criterion wearing a fix's clothing; the keeper set is DISCOVERED by doing the work,
#       so it cannot be a constant known when the row is minted.
#
# THE CRITERION THIS IMPLEMENTS, which needs no guess: every loose program file is EITHER converted (gone from
# the loose tree) OR explicitly declared a keeper in a KEEP.md. Same three-bucket shape as the corpus coverage
# manifest -- converted / declared-kept / nothing-falls-through -- and it is the declaration, not a number, that
# carries the judgment. Do NOT "fix" a red by raising an allowance; declare the file and say why.
#
# REFUSES rc=2 when it cannot measure -- never skip-as-success.
set -u
LANG_DIR="${1:-}"
[ -n "$LANG_DIR" ] || { echo "REFUSES rc=2: usage: $0 <lang>   (pascal|prolog|rebus|snocone|snobol4|raku|icon)"; exit 2; }
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"
TREE="$S4E/corpus/tests/$LANG_DIR"
[ -d "$TREE" ] || { echo "REFUSES rc=2: no such tree: $TREE"; exit 2; }
case "$LANG_DIR" in
  snobol4) EXT='*.sno';;  icon) EXT='*.icn';;   prolog) EXT='*.pl';;
  snocone) EXT='*.sc';;   rebus) EXT='*.reb';;  raku) EXT='*.raku';;  pascal) EXT='*.pas';;
  *) echo "REFUSES rc=2: unknown language '$LANG_DIR' -- add its extension deliberately rather than defaulting"; echo "   to a glob that might match nothing and read as success."; exit 2;;
esac
# ⭐ A converted suite file is EXEMPT WHEREVER IT LIVES, not only under crosscheck/. Two destination conventions
# are in live use -- tests/<lang>/crosscheck/<family>.<ext> (the SNOBOL4 shape) and tests/<lang>/<family>.<ext>
# (the parser-family shape, e.g. tests/raku/parser.raku, tests/rebus/parser.reb) -- and the first draft of this
# gate only knew the first. It therefore counted FINISHED SUITE OUTPUT as unconverted work: raku reported 104
# undeclared on an ALREADY-CONVERTED row. Reported by seat01 2026-08-27; a false red, and the same class of
# defect this gate exists to catch.
# ⛔ The exemption is CONTENT-BASED, deliberately, not a second hand-maintained list of family names or paths:
# a converted file opens with a numbered banner block (comment leader + rule + index + entry name), which the
# harness emits for every language -- "#--- 1 arr_get", "*--- 1 038_pat_literal". Validated at write time to
# match all three live conventions and none of the ordinary loose programs it was tested against.
is_suite_output() {
    head -1 "$1" 2>/dev/null | grep -qE '^[^A-Za-z0-9]*-{10,}[[:space:]]+[0-9]+[[:space:]]+[A-Za-z0-9_]'
}
mapfile -t ALLF < <(find "$TREE" -type f -name "$EXT" -not -path '*/crosscheck/*' 2>/dev/null | sort)
LOOSE=(); SUITEOUT=0
for _f in "${ALLF[@]}"; do
    if is_suite_output "$_f"; then SUITEOUT=$((SUITEOUT+1)); else LOOSE+=("$_f"); fi
done
TOTAL_ANY=$(find "$TREE" -type f -name "$EXT" 2>/dev/null | wc -l)
if [ "$TOTAL_ANY" -eq 0 ]; then
    echo "REFUSES rc=2: zero '$EXT' files anywhere under $TREE -- the extension or the tree moved, so this gate"
    echo "   has gone vacuous. A gate that matches nothing is not a green gate."
    exit 2
fi
echo "suite conversion completeness -- $LANG_DIR -- $(date -u +%Y-%m-%dT%H:%M:%SZ)"
echo "tree: $TREE   pattern: $EXT   total: $TOTAL_ANY   converted suite output: $SUITEOUT   loose: ${#LOOSE[@]}"
[ "${#LOOSE[@]}" -eq 0 ] && { echo "GATE OK -- nothing loose; every $EXT file lives in the governed suite tree."; exit 0; }
mapfile -t KEEPFILES < <(find "$TREE" -type f -name 'KEEP.md' 2>/dev/null)
DECLARED=""
[ "${#KEEPFILES[@]}" -gt 0 ] && DECLARED=$(cat "${KEEPFILES[@]}" 2>/dev/null)
UND=0; UNDLIST=""
for f in "${LOOSE[@]}"; do
    b=$(basename "$f")
    case "$DECLARED" in *"$b"*) : ;; *) UND=$((UND+1)); UNDLIST="$UNDLIST\n     $b";; esac
done
echo "KEEP.md file(s) found: ${#KEEPFILES[@]}   loose-but-undeclared: $UND"
if [ "$UND" -ne 0 ]; then
    echo "GATE FAILED -- $UND loose file(s) neither converted nor declared as keepers:"
    # ⛔⭐ THE LIST IS CAPPED AND THE CAP MUST ANNOUNCE ITSELF (hq_B 2026-08-29). This printed 19 names under
    # a "44 loose file(s)" headline with nothing saying the list was partial, so a seat working from the
    # printed list fixes what it can see, re-runs, and meets 25 files it was never shown. Same family as
    # `ls | head -5` read as absence, and as `command -v` answering a narrower question than was asked:
    # an instrument that truncates silently reports a subset in the shape of a whole.
    if [ -n "${SUITE_GATE_LIST_ALL:-}${GATE_LIST_ALL:-}" ]; then  # GATE_LIST_ALL is the fleet-wide spelling; SUITE_GATE_LIST_ALL kept as an alias
        printf "$UNDLIST\n" | grep -v '^[[:space:]]*$'
    else
        printf "$UNDLIST\n" | grep -v '^[[:space:]]*$' | head -20
        [ "$UND" -gt 20 ] && {
            echo "     ... and $((UND - 20)) MORE NOT SHOWN -- this list is capped at 20; $UND above is the true count."
            echo "     Full list: SUITE_GATE_LIST_ALL=1 bash scripts/test_gate_suite_conversion_complete.sh $LANG_DIR"
        }
    fi
    echo "     -> convert them, or name each in a KEEP.md WITH ITS REASON. Do not raise an allowance."
    exit 1
fi
echo "GATE OK -- ${#LOOSE[@]} loose file(s), all declared STANDALONE-KEEP. Conversion complete for $LANG_DIR."
exit 0
