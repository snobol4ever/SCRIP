#!/usr/bin/env bash
# test_gate_preflight_arms_stay_cheap.sh -- the guard on `make preflight` (hq_T's ask 2026-09-06; hq_R wired it).
# ⛔ WHAT IT PREVENTS, in hq_T's own words: "the target will quietly acquire a 45-second arm and stop being the
# thing that made it worth running." The value of a preflight target is that nobody weighs whether to run it, and
# that value is destroyed by a single slow arm -- silently, because a slow arm is still a CORRECT arm.
# ⛔ MEMBERSHIP IS PINNED BY MEASURED DURATION, NEVER BY A PATTERN OVER THE TEXT. A static grep cannot see one
# level of indirection: test_gate_pl_quad_regs.sh (~45s) and test_gate_capture_stdin_and_red_exit.sh (~15s) reach
# the compiler through lib_ helpers without naming it, and a comment-stripped grep clears both.
set -u
GATE_NAME=test_gate_preflight_arms_stay_cheap
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="${S4E_HOME:-$(cd "$HERE/.." && pwd)}"
. "$HERE/lib_gate.sh"
LIST="$HERE/preflight_arms.txt"
BUDGET_S=${PREFLIGHT_ARM_BUDGET_S:-5}
TOTAL_BUDGET_S=${PREFLIGHT_TOTAL_BUDGET_S:-40}
[ -f "$LIST" ] || { echo "⛔ REFUSED(2) [$GATE_NAME]: no $LIST -- the population is DECLARED, and an absent declaration is not an empty one"; exit 2; }
bad=0; n=0; total=0
while read -r arm; do
    case "$arm" in ""|\#*) continue ;; esac
    n=$((n + 1))
    script=${arm%% *}
    [ -e "$ROOT/$script" ] || { echo "  MISSING  $script"; bad=$((bad + 1)); continue; }
    s=$(date +%s.%N); ( cd "$ROOT" && timeout 30 bash -c "$arm" </dev/null >/dev/null 2>&1 ); rc=$?; e=$(date +%s.%N)
    case "$script" in *.py) ( cd "$ROOT" && timeout 30 python3 $arm </dev/null >/dev/null 2>&1 ); rc=$? ;; esac
    d=$(echo "$e - $s" | bc)
    over=$(echo "$d > $BUDGET_S" | bc)
    if [ "$over" = "1" ]; then printf '  SLOW     %6.2fs  %s  (budget %ss)\n' "$d" "$script" "$BUDGET_S"; bad=$((bad + 1)); continue; fi
    if [ "$rc" -ne 0 ]; then printf '  RED      rc=%-3s      %s\n' "$rc" "$script"; bad=$((bad + 1)); continue; fi
    total=$(echo "$total + $d" | bc)
    printf '  ok       %6.2fs  %s\n' "$d" "$script"
done < "$LIST"
[ "$n" -gt 0 ] || { echo "⛔ REFUSED(2) [$GATE_NAME]: graded ZERO arms -- a runner that cannot measure never prints the success shape"; exit 2; }
printf '  TOTAL    %6.2fs over %d arm(s) (per-arm ceiling %ss, total ceiling %ss)\n' "$total" "$n" "$BUDGET_S" "$TOTAL_BUDGET_S"
over_total=$(echo "$total > $TOTAL_BUDGET_S" | bc)
[ "$over_total" = "1" ] && { echo "  SLOW-TOTAL: the set as a whole exceeds its budget -- a preflight nobody runs is the defect this target cures"; bad=$((bad + 1)); }
GATE_EXAMINED="$n"; gate_verdict "$bad" "preflight arm(s) that are slow, red or missing (budget ${BUDGET_S}s each, examined $n)"
