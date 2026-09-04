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
M3_FAIL_NAMES=(); M4_FAIL_NAMES=(); REJECT_NAMES=()

echo "=== FPC vendored-suite grade ($TOTAL pairs, $SUITE) ==="

for name in "${PAIRS[@]}"; do
    pas="$SUITE/$name.pas"; ref="$SUITE/$name.ref"
    inp="$SUITE/$name.in"; [ -f "$inp" ] || inp=/dev/null
    if [ ! -f "$ref" ]; then
        REJECT=$((REJECT+1)); REJECT_NAMES+=("$name (no .ref)")
        continue
    fi
    exp="$(cat "$ref")"

    m3out=$(cd "$TMP" && timeout "$RUN_TIMEOUT" "$SCRIP" --run "$pas" < "$inp" 2>/dev/null)
    if [ "$m3out" = "$exp" ]; then
        M3_PASS=$((M3_PASS+1))
    else
        M3_FAIL=$((M3_FAIL+1)); M3_FAIL_NAMES+=("$name")
        [ "$VERBOSE" -eq 1 ] && echo "  m3 FAIL $name"
    fi

    m4bin="$TMP/${name}.bin"; m4s="$TMP/${name}.s"
    if timeout "$RUN_TIMEOUT" "$SCRIP" --compile "$pas" -o "$m4s" < /dev/null 2>/dev/null \
        && gcc -no-pie "$m4s" -L "${HERE}/../out" -lscrip_rt -Wl,-rpath,"${HERE}/../out" -o "$m4bin" 2>/dev/null; then
        m4out=$(cd "$TMP" && timeout "$RUN_TIMEOUT" "$m4bin" < "$inp" 2>/dev/null)
        if [ "$m4out" = "$exp" ]; then
            M4_PASS=$((M4_PASS+1))
        else
            M4_FAIL=$((M4_FAIL+1)); M4_FAIL_NAMES+=("$name")
            [ "$VERBOSE" -eq 1 ] && echo "  m4 FAIL $name"
        fi
    else
        M4_FAIL=$((M4_FAIL+1)); M4_FAIL_NAMES+=("$name (build/link failed)")
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
echo "FPC_SUITE_BOARD total=$TOTAL m3_pass=$M3_PASS m3_fail=$M3_FAIL m4_pass=$M4_PASS m4_fail=$M4_FAIL reject=$REJECT"
# ⛔ ONE LEADERBOARD (RULES.md FACT RULE, Lon 2026-09-03 ~16:05: "any run of a test suite by any
# session will update the ONE LEADERBOARD"). This records the board line printed just above into
# .github/SCORE.md -- it RUNS NOTHING, it only writes down what this script already measured.
# ⛔ NON-FATAL BY DESIGN: a bookkeeping failure must never turn a real measurement into a red board,
# because a gate that goes red for a reason unrelated to the code is a gate people route around. It
# warns and names the unrecorded row instead; it has no silent path.
python3 "$HERE/util_score_row.py" write --lang pascal --column vendor --suite fpc --modes m3,m4 \
    --measurer "${S4E_SEAT:-}" --text "m3 $M3_PASS/$TOTAL · m4 $M4_PASS/$TOTAL (m3_fail=$M3_FAIL m4_fail=$M4_FAIL reject=$REJECT, \`test_pascal_fpc_suite.sh\`)" \
    || echo "⚠ SCORE.md NOT UPDATED -- record this row by hand (the REFUSED line above says why)"


[ "$M3_FAIL" -eq 0 ] && [ "$M4_FAIL" -eq 0 ] && [ "$REJECT" -eq 0 ]
