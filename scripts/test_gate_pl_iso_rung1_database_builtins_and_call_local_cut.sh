#!/usr/bin/env bash
# test_gate_pl_iso_rung1_database_builtins_and_call_local_cut.sh -- PROLOG ISO LADDER, RUNG 1 (cto, CEO-617, 2026-09-12).
# THE RUNG: the database builtins raise the ISO error terms for literal arguments (type_error(callable, T) for a
# number in head or body, permission_error(modify, static_procedure, PI) for a control construct or builtin head,
# permission_error(access, private_procedure, PI) for clause/2 on one, type_error(predicate_indicator, S) and
# abolish's atom/integer/not_less_than_zero/max_arity terms for a bad spec), a directive assert marks its predicate
# dynamic the way a clause-body assert always did, a number in goal position raises type_error(callable) BEFORE
# anything runs (call((write(3), 1)) prints nothing), a cut inside call/1 is local to the call (ISO 7.8.3.3) while
# a cut inside a branch of a disjunction in the SAME scope still prunes it, max_arity is 1024 and functor/3 raises
# representation_error(max_arity) above it, and standard order puts 1.0 before 1 (ISO 7.2.1).
# WITNESS: the Logtalk ISO conformance groups this rung moves, graded by the suite's own grader ONE GROUP AT A
# TIME (--group is the grader's own development aid, never a board) in BOTH modes, against the pass floors
# measured at landing. RED BEFORE (coo board 2026-09-12T15:59:54 at 55aaa01ad, m3): asserta_1 4/24, assertz_1
# 4/24, abolish_1 5/16, retract_1 7/18, retractall_1 6/15, clause_2 8/16, current_predicate_1 4/23, cut_0 13/21,
# call_1 15/20. The floors below are what this rung LANDED at; a later rung may only raise them.
set -u
GATE_NAME=test_gate_pl_iso_rung1_database_builtins_and_call_local_cut
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="${S4E_HOME:-$(cd "$HERE/../.." && pwd)}"
SUITE="${S4E_CORPUS:-$ROOT/corpus}/packages/prolog/logtalk_iso"
SCRIP="$HERE/../scrip"
refuse() { echo "⛔ REFUSED(2) [$GATE_NAME]: $*" >&2; exit 2; }
[ -d "$SUITE" ] || refuse "no vendored suite at $SUITE"
[ -x "$SCRIP" ] || refuse "no scrip binary at $SCRIP -- run make first"
"$HERE/util_require_fresh.sh" --gate "$GATE_NAME" "$SCRIP" "${RT_DIR:-$HERE/../out}/libscrip_rt.so" || exit 2
MODES="${PL_RUNG_MODES:-m3,m4}"
red=0; total=0
floor() {
    local group="$1" want="$2" line pass
    line="$(python3 "$HERE/util_logtalk_grade.py" --suite "$SUITE" --scrip "$SCRIP" --modes "$MODES" --jobs "${PL_RUNG_JOBS:-8}" --group "$group" 2>/dev/null | grep -m1 '^LOGTALK_ISO_BOARD ')"
    [ -n "$line" ] || { echo "  ⛔ $group: the grader printed no LOGTALK_ISO_BOARD line"; red=$((red+1)); total=$((total+1)); return; }
    pass="$(printf '%s\n' "$line" | sed -n 's/.*m3_pass=\([0-9]*\).*/\1/p')"
    total=$((total+1))
    if [ "${pass:-0}" -ge "$want" ]; then echo "  ok  $group m3_pass=$pass (floor $want)  $line"
    else echo "  RED $group m3_pass=$pass < floor $want  $line"; red=$((red+1)); fi
    case "$MODES" in *m4*)
        local p4; p4="$(printf '%s\n' "$line" | sed -n 's/.*m4_pass=\([0-9]*\).*/\1/p')"
        if [ -n "$p4" ] && [ "$p4" -lt "$want" ]; then echo "  RED $group m4_pass=$p4 < floor $want (modes may diverge as an optimization, never here)"; red=$((red+1)); fi ;;
    esac
}
floor asserta_1 16
floor assertz_1 16
floor abolish_1 12
floor retract_1 13
floor retractall_1 10
floor clause_2 11
floor current_predicate_1 7
floor cut_0 16
floor call_1 16
floor if_then_else_3 13
floor disjunction_2 6
floor term_comparison 36
floor compare_3 23
floor functor_3 21
echo "$GATE_NAME: groups=$total red=$red modes=$MODES"
[ "$red" -eq 0 ] || { echo "⛔ $GATE_NAME RED"; exit 1; }
echo "✅ $GATE_NAME GREEN"
exit 0
