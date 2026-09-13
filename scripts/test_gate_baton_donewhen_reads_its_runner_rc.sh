#!/usr/bin/env bash
# test_gate_baton_donewhen_reads_its_runner_rc.sh -- hq_B 2026-09-13.
# GATE: no live baton DONE-WHEN may (A) capture a runner's output without being able to tell that runner's
# REFUSAL from its RED, or (B) hardcode a seat root as its ${S4E_HOME:-...} fallback.
# Both classes come from the ceo's own audit: FINDING-2026-09-11-ceo-a-done-when-that-captures-a-board-turns-
# could-not-measure-into-failed.md. Under ONE RUNNER, ONE BOARD a runner answers any seat but the coo with
# rc=2; a criterion that greps the captured TEXT and never looks at the rc turns that refusal into rc=1
# FAILED -- so precisely the rows that most need auditing cannot be audited by hand by anyone but the coo.
# ⛔ THE CENSUS IS NOT IN THIS FILE. It is util_donewhen_rc_census.py, the one authority, so that the gate and
# the cure can never drift apart -- the four-copies-of-one-extractor defect lib_donewhen.sh was written to end.
# --self-test PROVES THE GATE RED rather than asserting it, and proves the CLASS behaviourally: it stands up a
# stub runner that exits 2 the way a real runner refuses, runs a criterion of the bad shape and of the cured
# shape against it, and shows the bad one answering rc=1 FAILED where the cured one answers rc=2 REFUSE.
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
CENSUS="$HERE/util_donewhen_rc_census.py"
[ -f "$CENSUS" ] || { echo "⛔ REFUSE(2): $CENSUS missing -- cannot measure"; exit 2; }
if [ "${1:-}" = "--self-test" ]; then
    T="$(mktemp -d)" || exit 2; trap 'rm -rf "$T"' EXIT
    mkdir -p "$T/tasks" "$T/scripts"
    # a runner that REFUSES the way ONE RUNNER, ONE BOARD makes every runner refuse a non-coo seat
    printf '#!/usr/bin/env bash\necho "⛔ REFUSE(2): seat hq_B is not the coo -- ONE RUNNER, ONE BOARD"\nexit 2\n' \
        > "$T/scripts/board_stub.sh"; chmod +x "$T/scripts/board_stub.sh"
    BAD_PLAIN='out=$(bash '"$T"'/scripts/board_stub.sh 2>&1); printf "%s" "$out" | grep -q "^STUB_BOARD ok"'
    BAD_PIPED='out=$(bash '"$T"'/scripts/board_stub.sh 2>&1 | tail -1); printf "%s" "$out" | grep -q "^STUB_BOARD ok"'
    CURED_PLAIN='{ out=$(bash '"$T"'/scripts/board_stub.sh 2>&1); _dwrc=$?; if [ "$_dwrc" = 2 ]; then echo "REFUSE(2): the runner could not measure (rc=2) -- this is not a red"; exit 2; fi; [ "$_dwrc" = 0 ]; } && printf "%s" "$out" | grep -q "^STUB_BOARD ok"'
    HONEST='out=$(bash '"$T"'/scripts/board_stub.sh 2>&1); printf "%s" "$out" | grep -q "^STUB_BOARD " || { echo "REFUSE(2): runner printed no STUB_BOARD line"; exit 2; }; printf "%s" "$out" | grep -q "ok"'
    fail=0
    # ---- ARM 1: the behavioural witness. This is what makes the class real rather than asserted.
    bash -c "$BAD_PLAIN"  >/dev/null 2>&1; rb=$?
    bash -c "$BAD_PIPED"  >/dev/null 2>&1; rp=$?
    bash -c "$CURED_PLAIN">/dev/null 2>&1; rc_=$?
    bash -c "$HONEST"     >/dev/null 2>&1; rh=$?
    echo "SELF-TEST arm 1 (behaviour against a runner that exits 2):"
    echo "   bad  plain criterion -> rc=$rb   (expect 1: a REFUSAL read as a RED)"
    echo "   bad  piped criterion -> rc=$rp   (expect 1: same, and rc=\$? here would read the pipe)"
    echo "   cured      criterion -> rc=$rc_   (expect 2: the refusal propagates)"
    echo "   honest     criterion -> rc=$rh   (expect 2: marker absent, refuses without ever reading rc)"
    [ "$rb" = 1 ] && [ "$rp" = 1 ] && [ "$rc_" = 2 ] && [ "$rh" = 2 ] || { echo "⛔ arm 1 FAILED"; fail=1; }
    # ---- ARM 2: the census must SEE each shape, and must not call the honest one defective.
    mk() { printf '# TASK %s\nGOAL: self-test fixture\nDONE-WHEN: %s\n## NEXT\nx\n' "$1" "$2" > "$T/tasks/$1.task.md"; }
    mk selftest_bad_plain  "$BAD_PLAIN"
    mk selftest_bad_piped  "$BAD_PIPED"
    mk selftest_cured      "$CURED_PLAIN"
    mk selftest_honest     "$HONEST"
    mk selftest_classb     'R="${S4E_HOME:-/home/claude_cfo}"; [ -x "$R/SCRIP/scrip" ] || exit 2'
    out="$(S4E_TASKS="$T/tasks" python3 "$CENSUS" 2>&1)"; crc=$?
    echo "SELF-TEST arm 2 (the census over 5 crafted fixtures), rc=$crc:"; printf '%s\n' "$out" | sed 's/^/   /'
    printf '%s' "$out" | grep -q "defective (captures a runner, cannot tell a refusal from a red): 2" || { echo "⛔ arm 2: expected exactly 2 defective"; fail=1; }
    printf '%s' "$out" | grep -q "pipe: 1\]" || { echo "⛔ arm 2: expected exactly 1 piped"; fail=1; }
    printf '%s' "$out" | grep -q "absent): 1" || { echo "⛔ arm 2: expected exactly 1 honest (the cured one checks rc, so it is neither)"; fail=1; }
    printf '%s' "$out" | grep -q "S4E_HOME fallback):       1" || { echo "⛔ arm 2: expected exactly 1 class B"; fail=1; }
    [ "$crc" = 1 ] || { echo "⛔ arm 2: census must exit 1 when it finds defects (got $crc)"; fail=1; }
    # ---- ARM 3: the census must REFUSE, never print 0, when it cannot see its population.
    S4E_TASKS="$T/nowhere" python3 "$CENSUS" >/dev/null 2>&1
    [ $? = 2 ] || { echo "⛔ arm 3: census must exit 2 on an unreachable tasks dir"; fail=1; }
    echo "SELF-TEST arm 3 (unreachable population refuses rc=2): ok"
    [ "$fail" = 0 ] && { echo "✅ SELF-TEST GREEN: the gate proves red against a criterion of each bad shape."; exit 0; }
    echo "⛔ SELF-TEST FAILED"; exit 1
fi
out="$(python3 "$CENSUS" --list 2>&1)"; rc=$?
printf '%s\n' "$out"
case "$rc" in
  2) echo "⛔ REFUSE(2): the census could not measure its population -- not a red"; exit 2 ;;
  0) echo "✅ GATE GREEN: every baton DONE-WHEN can tell a runner's refusal from its red, and none hardcodes a seat root."; exit 0 ;;
  *) echo "⛔ GATE RED: the classes above are open. Cure with: python3 scripts/util_donewhen_rc_census.py --cure"; exit 1 ;;
esac
