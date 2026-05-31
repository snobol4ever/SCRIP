#!/usr/bin/env bash
# test_prolog_mode4_rung.sh — PL-G-1 (GOAL-PROLOG-BB)
# Mode-4 Prolog correctness gate. For each minimal program, compile via
# `scrip --compile --target=x86`, assemble, link, run, and diff stdout against
# `scrip --interp`. Reports PASS=N FAIL=M.
#
# Gate threshold (GOAL-PROLOG-BB PL-G-1): mode-4 PASS >= 1 before any AGW-9 rung
# is marked complete. Zero passing is acceptable while the bb_pl_* templates are
# still empty stubs — the script just needs to exist and measure the gap.
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ONE4ALL="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP:-$ONE4ALL/scrip}"
LIBRT="${LIBRT:-$ONE4ALL/out/libscrip_rt.so}"
RUNNER="$HERE/run_prolog_via_x86_backend.sh"
[ -x "$SCRIP" ] || { echo "FAIL scrip not built: $SCRIP"; exit 1; }
[ -f "$LIBRT" ] || { echo "FAIL libscrip_rt.so not built: $LIBRT (run 'make libscrip_rt')"; exit 1; }
WORK="$(mktemp -d /tmp/pl_m4_rung_XXXXXX)"
trap 'rm -rf "$WORK"' EXIT

# Minimal mode-4 rung programs. Each climbs one structural construct.
# m4-seq    : BB_PL_SEQ   (conjunction)
# m4-call   : BB_PL_CALL  (single predicate call)
# m4-choice : BB_CHOICE   (multi-clause predicate, first clause succeeds)
# m4-alt    : BB_ALT      (`;` disjunction)
declare -a NAMES=(m4-seq m4-call m4-choice m4-alt)
write_prog() {
    case "$1" in
        m4-seq)    printf 'main :- X is 1+2, write(X), nl.\n' ;;
        m4-call)   printf 'greet :- write(hi), nl.\nmain :- greet.\n' ;;
        m4-choice) printf 'p(a).\np(b).\nmain :- p(X), write(X), nl.\n' ;;
        m4-alt)    printf 'main :- ( true ; true ), write(ok), nl.\n' ;;
    esac
}

PASS=0; FAIL=0
for nm in "${NAMES[@]}"; do
    PL="$WORK/$nm.pl"
    write_prog "$nm" > "$PL"
    WANT="$(timeout 8 "$SCRIP" --interp "$PL" < /dev/null 2>/dev/null)"
    GOT="$(timeout 12 bash "$RUNNER" "$PL" 2>/dev/null)"
    if [ "$WANT" = "$GOT" ] && [ -n "$WANT" ]; then
        echo "  PASS $nm"
        PASS=$((PASS+1))
    else
        echo "  FAIL $nm  want:[$WANT] got:[$GOT]"
        FAIL=$((FAIL+1))
    fi
done
echo "--- Prolog (mode-4 x86): PASS=$PASS FAIL=$FAIL TOTAL=$((PASS+FAIL)) ---"
[ "$PASS" -ge 1 ] && exit 0 || exit 1
