#!/usr/bin/env bash
# test_gate_pl_a_program_with_more_than_64_dynamic_declarations_keeps_every_one.sh -- THE DYNAMIC TABLES GROW; THE ROOT CELLS ARE 256 AND THE 257TH REFUSES LOUDLY.
# hq_prolog 2026-10-04, the cfo's finding handed to the dynamic-database row: three tables capped at 64 (stage2.h pl_dyn_name/arity with pl_dyn_mark's silent
# return, lower_prolog.c g_pl_decl_dyn_name, and PL_DB_CELLS_MAX in the runtime) dropped the 64th user declaration silently (slot 0 is the registry's), so
# a program with 64 or more ":- dynamic" declarations printed nothing in both modes where swipl answers. Now the compile-time tables grow (ct_grow), every
# Prolog root frame carries PL_DB_CELLS_MAX = 256 cells, and a program needing more refuses rc=2 naming the limit instead of dropping a predicate.
# Witnesses: N = 70 declarations asserted and read back (swipl's text, both modes); N = 300 refuses rc=2 with the message in both compiles.
# RED BEFORE on origin 689af8179: N = 70 prints nothing (rc 0) in both modes.
set -u
GATE_NAME=test_gate_pl_a_program_with_more_than_64_dynamic_declarations_keeps_every_one
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${SCRIP_BIN:-$HERE/../scrip}"
RT="${RT_DIR:-$(dirname "$SCRIP")/out}"
refuse() { echo "⛔ REFUSED(2) [$GATE_NAME]: $*" >&2; exit 2; }
[ -x "$SCRIP" ] || refuse "no scrip binary at $SCRIP -- run make first"
[ -n "${SCRIP_BIN:-}" ] || "$HERE/util_require_fresh.sh" --gate "$GATE_NAME" "$SCRIP" "$RT/libscrip_rt.so" || exit 2
TMPD="$(mktemp -d)"; trap 'rm -rf "$TMPD"' EXIT
gen() { local n=$1 f=$2; : > "$f"; for ((k = 0; k < n; k++)); do echo ":- dynamic(d$k/1)." >> "$f"; done
    printf 'main :- ' >> "$f"; for ((k = 0; k < n; k++)); do printf 'assertz(d%d(%d)), ' $k $k >> "$f"; done
    printf 'findall(X, d0(X), L0), write(L0), nl, findall(Y, d%d(Y), Ln), write(Ln), nl, d%d(Z), write(Z), nl.\n:- initialization(main).\n' $((n-1)) $((n/2)) >> "$f"; }
gen 70 "$TMPD/w70.pl"; gen 300 "$TMPD/w300.pl"
want="$(printf '[0]\n[69]\n35\n')"
red=0
got="$(cd "$TMPD" && timeout 30 "$SCRIP" w70.pl </dev/null 2>"$TMPD/err")"; rc=$?
if [ "$rc" = 0 ] && [ "$got" = "$want" ]; then echo "  ok  70 declarations m3"; else echo "  RED 70 declarations m3: rc=$rc out=[$(printf '%s' "$got" | tr '\n' '|' | cut -c1-80)] err=[$(head -c 120 "$TMPD/err" | tr '\n' '|')]"; red=$((red+1)); fi
if timeout 120 "$SCRIP" --compile -o "$TMPD/w70.s" "$TMPD/w70.pl" </dev/null 2>"$TMPD/err" && gcc -m64 -no-pie "$TMPD/w70.s" -o "$TMPD/w70.bin" -L"$RT" -lscrip_rt -Wl,-rpath,"$RT" -lm 2>"$TMPD/err"; then
    got="$(cd "$TMPD" && timeout 30 ./w70.bin </dev/null 2>"$TMPD/err")"; rc=$?
    if [ "$rc" = 0 ] && [ "$got" = "$want" ]; then echo "  ok  70 declarations m4"; else echo "  RED 70 declarations m4: rc=$rc out=[$(printf '%s' "$got" | tr '\n' '|' | cut -c1-80)] err=[$(head -c 120 "$TMPD/err" | tr '\n' '|')]"; red=$((red+1)); fi
else echo "  RED 70 declarations m4: the compile or link refused: $(head -c 160 "$TMPD/err" | tr '\n' '|')"; red=$((red+1)); fi
for mode in m3 m4; do
    if [ "$mode" = m3 ]; then (cd "$TMPD" && timeout 30 "$SCRIP" w300.pl </dev/null >"$TMPD/out" 2>"$TMPD/err"); rc=$?
    else timeout 120 "$SCRIP" --compile -o "$TMPD/w300.s" "$TMPD/w300.pl" </dev/null >"$TMPD/out" 2>"$TMPD/err"; rc=$?; fi
    if [ "$rc" = 2 ] && grep -q 'PL_DB_CELLS_MAX' "$TMPD/err"; then echo "  ok  300 declarations $mode refuse rc=2 naming the cell count"
    else echo "  RED 300 declarations $mode: rc=$rc (want 2 with the PL_DB_CELLS_MAX message) err=[$(head -c 160 "$TMPD/err" | tr '\n' '|')]"; red=$((red+1)); fi
done
[ "$red" = 0 ] || { echo "GATE FAIL [$GATE_NAME]: $red arm(s) red"; exit 1; }
echo "GATE PASS [$GATE_NAME]: 70 dynamic declarations are all kept in both modes, and 300 refuse rc=2 naming the 256-cell limit"
exit 0
