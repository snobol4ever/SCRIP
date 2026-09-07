#!/usr/bin/env bash
# test_pascal_fpc_suite.sh -- Phase 3 of row `fpc-tests-vendor-script-run`: grades the vendored
# subset of Free Pascal's own test suite (corpus/packages/pascal/fpc_tests/, 181 .pas/.ref pairs)
# both SCRIP modes against refs captured from the ruled fpc -Miso oracle (util_census_fpc_tests.sh
# found 298 files SCRIP's front end compiles at all under the full 7618-file FPCSource/tests/ tree;
# of those, 55 are from FPC's own tbf/webtbf "expected to fail" categories -- SCRIP wrongly accepted
# them, a front-end leniency finding reported separately, never vendored as normal suite entries; of
# the remaining 243, 181 vendored clean under -Miso and 62 could not (about a third of THOSE are a
# known, named mode limitation: -Miso is pure ISO 7185 and rejects the `string` identifier, a
# Borland/Delphi-lineage extension SCRIP itself supports -- flagged as a real, open mode question,
# not silently worked around by picking a different mode unilaterally; see this row's own NEXT/LEDGER).
#
# ⚠️ test_gate_pascal_m4.sh's own row (pascal-m4-intermittent-segv-layout-sensitive) measured GENUINE
# run-to-run non-determinism on Pascal m4 output (5 consecutive runs, same tree, same binary, 5
# different pass counts) -- inherited risk, not introduced here. Do not read one run's m4 number as
# exact; the board line names its own run so a caller can re-run and compare.
set -u
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
. "$HERE/lib_flag_gate.sh" 2>/dev/null || { echo "⛔ REFUSED-TO-GRADE: lib_flag_gate.sh unloadable"; exit 2; }
. "$HERE/lib_inventory.sh" 2>/dev/null || { echo "⛔ REFUSED-TO-GRADE: lib_inventory.sh unloadable"; exit 2; }
. "$HERE/lib_progress.sh" 2>/dev/null || { echo "⛔ REFUSED-TO-GRADE: lib_progress.sh unloadable"; exit 2; }
[ $# -eq 0 ] || flaggate_reject "$1" "(none -- set FPC_SUITE_RUN_TIMEOUT / FPC_SUITE_VERBOSE via environment instead)"
SCRIP="${HERE}/../scrip"
RT_SO="${HERE}/../out/libscrip_rt.so"
SUITE="$S4E/corpus/packages/pascal/fpc_tests"
RUN_TIMEOUT="${FPC_SUITE_RUN_TIMEOUT:-10}"
VERBOSE="${FPC_SUITE_VERBOSE:-0}"

[ -d "$SUITE" ]  || { echo "⛔ REFUSED-TO-GRADE: $SUITE missing"; exit 2; }
[ -x "$SCRIP" ]  || { echo "⛔ REFUSED-TO-GRADE: scrip not built"; exit 2; }
# ⛔⭐ STALE-BINARY PREFLIGHT (row harness-and-ladder-runner-refuse-on-a-stale-binary-like-the-artifact-regen-
# does, ceo -> hq_T 2026-09-04). The line above proves a binary EXISTS; this one proves it is the binary this
# tree describes. ceo's witness, twice on 2026-09-04: a 10:57 binary graded at 14:03 read RED, then GREEN after
# an incremental make -- a vendor board is exactly where that is least visible, because a plausible all-FAIL
# table is this class's normal output. NO LOGIC HERE: util_require_fresh.sh sources gate_require_fresh from
# lib_gate.sh, the ONE authority (hq_B 4c7253e99) -- never a second copy of the staleness rule.
"$HERE/util_require_fresh.sh" --gate test_pascal_fpc_suite "$SCRIP" "${RT_DIR:-$HERE/../out}/libscrip_rt.so" || exit 2
[ -f "$RT_SO" ]  || { echo "⛔ REFUSED-TO-GRADE: $RT_SO missing (m4 link needs it)"; exit 2; }

TMP="$(mktemp -d /tmp/fpc_suite_XXXXXX)"
trap 'rm -rf "$TMP"' EXIT
cd "$TMP"   # master harness convention (test_gate_em_beauty_subsystems_mode4.sh): graded programs run
            # against a scratch cwd, never the invoker's -- some vendored fpc tests (tisobuf1/tisoread)
            # write scratch files relative to cwd and were leaking them into the caller's directory

mapfile -t PAIRS < <(cd "$SUITE" && ls *.pas 2>/dev/null | sed 's/\.pas$//' | sort)
TOTAL=${#PAIRS[@]}
[ "$TOTAL" -gt 0 ] || { echo "⛔ REFUSED-TO-GRADE: zero .pas/.ref pairs in $SUITE"; exit 2; }

M3_PASS=0; M3_FAIL=0; M4_PASS=0; M4_FAIL=0; REJECT=0
# ⛔⭐ THE AND PER PROGRAM (ceo-372, 2026-09-06): this suite's leaderboard row states the programs green in
# EVERY graded mode -- never m4 alone (what it published until now), never m3 alone, and never
# min(M3_PASS,M4_PASS), which is not a count of any set of programs: one program red only in m3 and another
# red only in m4 leave min() a program too high. Accumulated per program below, where both verdicts are in hand.
BOTH_PASS=0
M3_FAIL_NAMES=(); M4_FAIL_NAMES=(); REJECT_NAMES=()
PROG_ROWS="$TMP/progress.tsv"; : >"$PROG_ROWS"

echo "=== FPC vendored-suite grade ($TOTAL pairs, $SUITE) ==="

for name in "${PAIRS[@]}"; do
    pas="$SUITE/$name.pas"; ref="$SUITE/$name.ref"
    inp="$SUITE/$name.in"; [ -f "$inp" ] || inp=/dev/null
    if [ ! -f "$ref" ]; then
        REJECT=$((REJECT+1)); REJECT_NAMES+=("$name (no .ref)")
        # ⭐ RECORDED, NOT DROPPED: a shipped .pas with no .ref is owed work (REF_NOT_CUT), and THE PACKAGE
        # LOCKDOWN is precisely the rule that such a program must appear somewhere as ungraded rather than
        # silently vanish from the table the census reads.
        for m in m3 m4; do printf 'package\tfpc\tpascal\t%s\t%s\tUNGRADED\t0\tno-ref-cut\n' "$name" "$m" >>"$PROG_ROWS"; done
        continue
    fi
    exp="$(cat "$ref")"

    m3ok=0
    m3out=$(cd "$TMP" && timeout "$RUN_TIMEOUT" "$SCRIP" --run "$pas" < "$inp" 2>/dev/null)
    if [ "$m3out" = "$exp" ]; then
        M3_PASS=$((M3_PASS+1)); m3ok=1
        printf 'package\tfpc\tpascal\t%s\tm3\tPASS\t0\t\n' "$name" >>"$PROG_ROWS"
        [ "$VERBOSE" -eq 1 ] && echo "  m3 PASS $name"
    else
        M3_FAIL=$((M3_FAIL+1)); M3_FAIL_NAMES+=("$name")
        printf 'package\tfpc\tpascal\t%s\tm3\tFAIL\t0\toutput-differs-from-ref\n' "$name" >>"$PROG_ROWS"
        [ "$VERBOSE" -eq 1 ] && echo "  m3 FAIL $name"
    fi

    m4bin="$TMP/${name}.bin"; m4s="$TMP/${name}.s"
    if timeout "$RUN_TIMEOUT" "$SCRIP" --compile "$pas" -o "$m4s" < /dev/null 2>/dev/null \
        && gcc -no-pie "$m4s" -L "${HERE}/../out" -lscrip_rt -Wl,-rpath,"${HERE}/../out" -o "$m4bin" 2>/dev/null; then
        m4out=$(cd "$TMP" && timeout "$RUN_TIMEOUT" "$m4bin" < "$inp" 2>/dev/null)
        if [ "$m4out" = "$exp" ]; then
            M4_PASS=$((M4_PASS+1)); [ "$m3ok" -eq 1 ] && BOTH_PASS=$((BOTH_PASS+1))
            printf 'package\tfpc\tpascal\t%s\tm4\tPASS\t0\t\n' "$name" >>"$PROG_ROWS"
            [ "$VERBOSE" -eq 1 ] && echo "  m4 PASS $name"
        else
            M4_FAIL=$((M4_FAIL+1)); M4_FAIL_NAMES+=("$name")
            printf 'package\tfpc\tpascal\t%s\tm4\tFAIL\t0\toutput-differs-from-ref\n' "$name" >>"$PROG_ROWS"
            [ "$VERBOSE" -eq 1 ] && echo "  m4 FAIL $name"
        fi
    else
        M4_FAIL=$((M4_FAIL+1)); M4_FAIL_NAMES+=("$name (build/link failed)")
        printf 'package\tfpc\tpascal\t%s\tm4\tFAIL\t0\tbuild-or-link-failed\n' "$name" >>"$PROG_ROWS"
        [ "$VERBOSE" -eq 1 ] && echo "  m4 FAIL $name (build/link failed)"
    fi
done

echo ""
if [ "$REJECT" -gt 0 ]; then
    echo "-- pairs missing a .ref (instrument issue, not a grading result): $REJECT --"
    for n in "${REJECT_NAMES[@]}"; do echo "   $n"; done
fi
if [ "$VERBOSE" -ne 1 ] && [ "$M3_FAIL" -gt 0 ]; then
    echo "-- m3 FAIL ($M3_FAIL): ${M3_FAIL_NAMES[*]:0:10}$([ "$M3_FAIL" -gt 10 ] && echo ' ...')"
fi
if [ "$VERBOSE" -ne 1 ] && [ "$M4_FAIL" -gt 0 ]; then
    echo "-- m4 FAIL ($M4_FAIL): ${M4_FAIL_NAMES[*]:0:10}$([ "$M4_FAIL" -gt 10 ] && echo ' ...')"
fi

echo ""
echo "FPC_SUITE_BOARD total=$TOTAL both_pass=$BOTH_PASS m3_pass=$M3_PASS m3_fail=$M3_FAIL m4_pass=$M4_PASS m4_fail=$M4_FAIL reject=$REJECT"
# ⭐ THE PACKAGE LOCKDOWN inventory line, via the shared body (lib_inventory.sh) -- never a second copy
# of the arithmetic. REJECT here is "shipped .pas with no .ref yet" -- real owed work (REF_NOT_CUT), not
# an oracle ruling -- so a nonzero REJECT with no UNGRADED.tsv beside $SUITE correctly REFUSES below
# rather than being silently folded into ungradable the way the old ad hoc line did.
FPC_GRADED=$((TOTAL - REJECT))
INV_PACKAGE=fpc; INV_DIR="$SUITE"; INV_EXT=".pas"
INV_LINE="$(inventory_line "$FPC_GRADED" 0)"
if [ -n "$INV_LINE" ]; then echo "$INV_LINE"; else echo "⚠ inventory refused (above) -- the board line still stands; the inventory does not" >&2; fi
# ⛔ ONE LEADERBOARD (RULES.md FACT RULE, Lon 2026-09-03 ~16:05: "any run of a test suite by any
# session will update the ONE LEADERBOARD"). This records the board line printed just above into
# .github/SCORE.md -- it RUNS NOTHING, it only writes down what this script already measured.
# ⛔ NON-FATAL BY DESIGN: a bookkeeping failure must never turn a real measurement into a red board,
# because a gate that goes red for a reason unrelated to the code is a gate people route around. It
# warns and names the unrecorded row instead; it has no silent path.
# ⛔⭐ THE SUITE ROW'S PAIR IS DECLARED, NEVER PARSED (hq_T 2026-09-06, ceo CEO-363; the coo had to set
# this row BY HAND after a clean run because util_score_row correctly REFUSED to guess). The --text below
# carries THREE fractions over the same denominator (the AND, m3 and m4), and there is no fact of the matter
# about which one a text parse should take -- so the writer refuses rather than picking, and the runner
# declares its headline pair.
# ⛔⭐ THE RULING CAME, AND IT IS NEITHER ARM: ceo-372, 2026-09-06, on hq_T's ask. This row was wired to m4
# (because SUITES.tsv carried m4), then to m3 by hq_V at 20:5x when the coo hand-set the published row to the
# m3 numbers -- each rewiring correctly avoiding the phantom regression the other would have published, and
# hq_V closed with "The ceo is asked to rule; either way it stays one line." This is that line. The pair is
# THE AND PER PROGRAM: "a program red in m3 and another red in m4 both count against the row; never m3 alone,
# never m4 alone, never the min of two counts (which hides a program red in each)."
# ⭐ hq_V's SECOND, STANDING REASON IS ANSWERED RATHER THAN OVERRULED. m4 here really is run-to-run
# non-deterministic (this file's header: five runs, one tree, five pass counts), and a row wired to a number
# that moves without the code moving really does manufacture phantom movement in the table Lon reads. The
# ceo's ruling puts that where it belongs: "a mode whose count varies run to run is a DEFECT ROW in that lane
# -- a nondeterministic compile is the xfail shape with a runner's excuse in front of it -- never a reason to
# publish the steadier mode." The wobble is now the Pascal lane's row (pascal-m4-intermittent-segv-layout-
# sensitive), not a thing the leaderboard steers around by choosing an arm.
# ⛔ THE NUMBER DROPS FROM $M3_PASS (m3, as published) TO $BOTH_PASS -- 130 to 116 of 181, measured this
# sitting -- AND THAT IS A CRITERION CHANGE, NOT A REGRESSION. The commit landing it says so in those words.
python3 "$HERE/util_score_row.py" write --lang pascal --column vendor --suite fpc --modes m3,m4 \
    --suite-pass "$BOTH_PASS" --suite-total "$TOTAL" \
    --measurer "${S4E_SEAT:-}" --text "both-modes $BOTH_PASS/$TOTAL · m3 $M3_PASS/$TOTAL · m4 $M4_PASS/$TOTAL (m3_fail=$M3_FAIL m4_fail=$M4_FAIL reject=$REJECT${INV_LINE:+ · $INV_LINE (\`test_pascal_fpc_suite.sh\`)})" \
    || echo "⚠ SCORE.md NOT UPDATED -- record this row by hand (the REFUSED line above says why)"


# ⛔⭐ THE FACT RULE'S OTHER HALF (CEO-319, /home/resources/progress/README.md): every suite run APPENDS its
# per-program rows in the same sitting it rewrites its cell. MEASURED by hq_V at its opening, 2026-09-06:
# of 497 pascal rows in that table every one was pascal-master -- ZERO from fpc or pat, so a Pascal PACKAGE
# flip was invisible to the measure OCTET is run on. One bulk call, not 362. Non-fatal, never silent.
if [ -s "$PROG_ROWS" ]; then
    if ! progress_append_rows_tsv "$PROG_ROWS"; then
        echo "⚠ PROGRESS DB NOT UPDATED -- the board above stands, its per-program rows do not (reason above)" >&2
    fi
fi

# ⛔⭐ POPULATION FLOOR (row every-board-wrapper-refuses-on-a-zero-population-instead-of-passing-
# vacuously, hq_T 2026-09-04): M3_FAIL/M4_FAIL/REJECT all read 0 over TOTAL=0 too (empty discovery) --
# refuse before the vacuous-clean verdict below can be reached.
"$HERE/util_require_population.sh" --gate test_pascal_fpc_suite "$TOTAL" 1 "pascal witnesses" || exit 2
[ "$M3_FAIL" -eq 0 ] && [ "$M4_FAIL" -eq 0 ] && [ "$REJECT" -eq 0 ]
