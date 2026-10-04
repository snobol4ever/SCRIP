#!/usr/bin/env bash
# test_gate_pl_a_static_call_jumps_to_its_callee_directly_and_a_last_call_stages_from_the_slot.sh -- A STATIC CALL NEVER GOES THROUGH THE REGISTRY RECORD.
# hq_prolog 2026-10-04, row prolog-bb-a-static-call-jumps-to-its-callee-directly-and-stages-its-last-call-block-by-compile-time-cell-classes-not-a-four-hop-loop-586-registry-loads
# (ARCH-PROLOG-C-OUT-OF-THE-BOX.md section 9). ARM 1: the 23 kernels' mode-4 text loads g_rt_gen_procs ZERO times -- a static callee (file-defined,
# a registered compile-time graph or the predicate's own key; never declared dynamic, never a packet) is entered by lea rax, [rip + FN__callee];
# jmp rax in the text and through its sealed alpha cell in the slab (x86_jmp_via_cell, both media), not by the record's three dependent loads;
# a dynamic or run-time-defined callee keeps the registry jump. ARMS 2-3: two swipl-cut witnesses in m3 AND m4, the behaviour the cure keeps:
# (2) a 300,000-deep last-call recursion with variable arguments in the default stack (the tail arm fires: the block staged from each
# variable's own slot, one hop fewer than through its name cell), a last call passing a head variable down two frames, a last call passing a
# local bound by the previous goal, two locals aliased by =/2 then passed (the decline road); (3) a static clause whose last call is a
# DYNAMIC predicate, extended by assertz and shrunk by retract between calls (the registry must be followed), and an asserted clause whose
# last call is a static predicate.
# RED BEFORE on origin ec631d327: arm 1 counts 586 loads (every call box, two arms each); the witnesses pass there.
set -u
GATE_NAME=test_gate_pl_a_static_call_jumps_to_its_callee_directly_and_a_last_call_stages_from_the_slot
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="${S4E_HOME:-$(cd "$HERE/../.." && pwd)}"
SCRIP="${SCRIP_BIN:-$HERE/../scrip}"
RT="${RT_DIR:-$(dirname "$SCRIP")/out}"
refuse() { echo "⛔ REFUSED(2) [$GATE_NAME]: $*" >&2; exit 2; }
[ -x "$SCRIP" ] || refuse "no scrip binary at $SCRIP -- run make first"
[ -x "$HERE/bench_prolog_text_census.sh" ] || refuse "no bench_prolog_text_census.sh beside this gate"
[ -n "${SCRIP_BIN:-}" ] || "$HERE/util_require_fresh.sh" --gate "$GATE_NAME" "$SCRIP" "$RT/libscrip_rt.so" || exit 2
TMPD="$(mktemp -d)"; trap 'rm -rf "$TMPD"' EXIT
red=0
cen="$(S4E_HOME="$ROOT" SCRIP_BIN="$SCRIP" bash "$HERE/bench_prolog_text_census.sh" 'g_rt_gen_procs' 2>&1)"; crc=$?
case "$crc" in
    0) echo "  ok  arm 1: $cen" ;;
    1) echo "  RED arm 1: $(printf '%s' "$cen" | cut -c1-200)"; red=$((red+1)) ;;
    *) refuse "the text census could not measure: $(printf '%s' "$cen" | cut -c1-200)" ;;
esac
mkw() { cat > "$TMPD/$1.pl"; }
mkw w_lco <<'EOP'
cnt(0, A, A) :- !.
cnt(N, A, R) :- N1 is N - 1, A1 is A + N, cnt(N1, A1, R).
t(X) :- u(X).
u(Y) :- Y = done.
v(L) :- L = [a, b, c].
r(L, R) :- length(L, R).
w(R) :- v(L), r(L, R).
a(R) :- X = Y, b(X, Y, R).
b(P, Q, R) :- P = 1, R = Q.
main :- cnt(300000, 0, S), write(S), nl, t(V), write(V), nl, w(N), write(N), nl, a(Q), write(Q), nl.
:- initialization(main).
EOP
mkw w_dyn <<'EOP'
:- dynamic(d/1).
d(1).
s(X) :- d(X).
cnt(0, A, A) :- !.
cnt(N, A, R) :- N1 is N - 1, A1 is A + N, cnt(N1, A1, R).
main :- findall(X, s(X), L0), write(L0), nl, assertz(d(2)), findall(Y, s(Y), L1), write(L1), nl, assertz((e(Z) :- cnt(10, 0, Z))), e(V), write(V), nl, retract(d(1)), findall(W, s(W), L2), write(L2), nl.
:- initialization(main).
EOP
want_w_lco='45000150000
done
3
1'
want_w_dyn='[1]
[1,2]
55
[2]'
for w in w_lco w_dyn; do
    eval "want=\$want_$w"
    for mode in m3 m4; do
        if [ "$mode" = m3 ]; then got="$(cd "$TMPD" && timeout 60 "$SCRIP" "$w.pl" </dev/null 2>"$TMPD/err")"; rc=$?
        else
            timeout 120 "$SCRIP" --compile -o "$TMPD/$w.s" "$TMPD/$w.pl" </dev/null 2>"$TMPD/err" || { echo "  RED $w $mode: the compile refused: $(head -c 160 "$TMPD/err")"; red=$((red+1)); continue; }
            gcc -m64 -no-pie "$TMPD/$w.s" -o "$TMPD/$w.bin" -L"$RT" -lscrip_rt -Wl,-rpath,"$RT" -lm 2>"$TMPD/err" || { echo "  RED $w $mode: the assembler or linker refused: $(grep -m1 -E 'Error|error' "$TMPD/err" | head -c 200)"; red=$((red+1)); continue; }
            got="$(cd "$TMPD" && timeout 60 "./$w.bin" </dev/null 2>"$TMPD/err")"; rc=$?
            rm -f "$TMPD/$w.s" "$TMPD/$w.bin"
        fi
        if [ "$rc" = 0 ] && [ "$got" = "$want" ]; then echo "  ok  $w $mode"
        else echo "  RED $w $mode: rc=$rc out=[$(printf '%s' "$got" | tr '\n' '|' | cut -c1-200)] err=[$(head -c 160 "$TMPD/err" | tr '\n' '|')] want [$(printf '%s' "$want" | tr '\n' '|' | cut -c1-120)]"; red=$((red+1)); fi
    done
done
[ "$red" = 0 ] || { echo "GATE FAIL [$GATE_NAME]: $red arm(s) red"; exit 1; }
echo "GATE PASS [$GATE_NAME]: no static call of the 23 kernels goes through the registry record, and the last-call witnesses answer swipl's text in both modes"
exit 0
