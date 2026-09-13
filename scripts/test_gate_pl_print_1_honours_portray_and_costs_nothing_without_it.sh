#!/usr/bin/env bash
# test_gate_pl_print_1_honours_portray_and_costs_nothing_without_it.sh
# Row prolog-logtalk-write-term-print-and-write-canonical-family (hq_R, CEO-650); design approved by the
# cto 2026-09-13: the LOWERER MINTS A WRAPPER CLAUSE and the existing goal dispatch finds it -- no new goal
# machinery, no runtime-to-Prolog callback.
#
# print/1 is ( portray(X) -> true ; writeq(X) ), MINTED ONLY WHEN THE PROGRAM DEFINES portray/1. Otherwise
# print/1 stays the $writeq det leaf it has always been.
#
# ⛔⭐ ARM 1 IS THE POINT AND IT RUNS FIRST, BY ORDER, NOT BY CONVENTION. The cure's whole claim is that a
# program WITHOUT portray/1 pays nothing -- no proc, no dispatch, no emitted byte. That claim is easy to
# fake by measuring it after the fact, so the no-portray emission and stdout are captured BEFORE any
# portray-defining program is compiled in this script.
#
# ⛔⭐ ARM 1 WAS A PINNED WHOLE-EMISSION md5 AND THAT WAS A DEFECT OF MINE, NOW DELETED (hq_R 2026-09-13, on
# hq_U's report). RULES.md says .s byte-identity must never be wired into a gate, and this is exactly why:
# hq_U landed CEO-684 slice 1, which shrinks every activation frame by one 16-byte slot per call node, so
# every offset in the file moved and the pin necessarily reddened -- on a CORRECT landing, for a reason
# having nothing to do with portray. hq_U told me rather than bumping the constant, which is the right call:
# re-pinning another seat's constant is how a pin stops meaning anything. A second such landing (the
# cto's Lon-ordered IR_MOVE_LABEL delete) was already queued behind it.
#
# ⭐ THE CURE IS TO STATE THE CLAIM DIFFERENTIALLY, IN ONE BUILD, SO NO CONSTANT EXISTS TO GO STALE. The
# claim is not "this program emits these bytes"; it is "with no portray/1, print/1 IS the writeq det leaf".
# So emit the SAME program twice in the SAME build -- once written with print/1, once with writeq/1 -- and
# require the two path-normalised emissions to be IDENTICAL TO EACH OTHER. That is immune to every future
# emitter change by construction, because both sides move together. ARM 1c is its control and it is what
# gives the comparison teeth: with portray/1 DEFINED the same two programs must DIFFER, so a comparison
# that trivially passes (say both emissions empty, or the normaliser eating the difference) is caught.
# Arm 1b keeps the narrower claim: a program with no portray/1 mentions portray NOWHERE in its emission.
# ⛔ It deliberately does NOT grep for "print/1": that string is a startup predicate-name the compiler
# emitted long before this cure, so grepping it convicts the pre-cure compiler too -- an arm that reds on a
# clean tree is measuring the wrong thing.
#
# rc 0 green · 1 red · 2 could not measure.  Self-contained; run from anywhere.
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"; cd "$ROOT"
SCRIP="${SCRIP_BIN:-$ROOT/scrip}"
"$HERE/util_require_fresh.sh" --gate "$(basename "${BASH_SOURCE[0]}" .sh)" || exit $?
[ -x "$SCRIP" ] || { echo "⛔ REFUSE(2): no scrip at $SCRIP"; exit 2; }
D=$(mktemp -d) || { echo "⛔ REFUSE(2): no tmpdir"; exit 2; }
trap 'rm -rf "$D"' EXIT
PASS=0; FAIL=0; N=0
cat > "$D/noportray.pl" <<'PLEOF'
:- initialization(main).
main :- print(42), nl, print(foo), nl, print(a(b)), nl, print([1,2]), nl.
PLEOF
echo "=== ARM 1 (FIRST, and the one that can be faked by running it last): no portray/1 costs nothing ==="
sed 's/print(/writeq(/g' "$D/noportray.pl" > "$D/nowriteq.pl"
gate_emit_md5() {   # $1 = .pl to compile, $2 = .s to write -- path- AND basename-normalised, so only CODE differs
  timeout 60 "$SCRIP" --compile -o "$2" "$1" >/dev/null 2>&1 || return 1
  sed "s|$D|@|g; s|noportray|@PROG@|g; s|nowriteq|@PROG@|g; s|yesportray|@PROG@|g; s|yeswriteq|@PROG@|g" "$2" | md5sum | cut -d' ' -f1
}
N=$((N+1))
mp=$(gate_emit_md5 "$D/noportray.pl" "$D/np.s"); rcp=$?
mq=$(gate_emit_md5 "$D/nowriteq.pl"  "$D/nq.s"); rcq=$?
if [ $rcp -ne 0 ] || [ $rcq -ne 0 ]; then
  echo "  RED  a no-portray program failed to compile (print rc=$rcp writeq rc=$rcq)"; FAIL=$((FAIL+1))
elif [ -z "$mp" ] || [ -z "$mq" ]; then
  echo "⛔ REFUSE(2): an emission md5 came back EMPTY, so arm 1 graded nothing -- a comparison of two blanks is not a pass"; exit 2
elif [ "$mp" = "$mq" ]; then PASS=$((PASS+1))
else
  echo "  RED  with NO portray/1 defined, print/1 did not emit identically to writeq/1, so the cure is not free:"
  echo "       print md5 $mp  vs  writeq md5 $mq"
  diff <(sed "s|$D|@|g; s|noportray|@PROG@|g" "$D/np.s") <(sed "s|$D|@|g; s|nowriteq|@PROG@|g" "$D/nq.s") | head -12 | sed 's/^/       /'
  FAIL=$((FAIL+1))
fi
N=$((N+1))
cat > "$D/yesportray.pl" <<'PLEOF'
portray(A) :- atom(A), write(A), write(A).
:- initialization(main).
main :- print(42), nl, print(foo), nl, print(a(b)), nl, print([1,2]), nl.
PLEOF
sed 's/print(/writeq(/g' "$D/yesportray.pl" > "$D/yeswriteq.pl"
yp=$(gate_emit_md5 "$D/yesportray.pl" "$D/yp.s"); yq=$(gate_emit_md5 "$D/yeswriteq.pl" "$D/yq.s")
if [ -n "$yp" ] && [ -n "$yq" ] && [ "$yp" != "$yq" ]; then PASS=$((PASS+1))
else echo "  RED  ARM 1c CONTROL: with portray/1 DEFINED, print/1 emitted the SAME as writeq/1 ($yp vs $yq) -- so arm 1's comparison has no teeth and its pass means nothing"; FAIL=$((FAIL+1)); fi
N=$((N+1))
if grep -qE '(^|[^A-Za-z0-9_])portray([^A-Za-z0-9_]|$)' "$D/np.s" 2>/dev/null; then
  echo "  RED  a program with no portray/1 emitted a reference to portray"; FAIL=$((FAIL+1))
else PASS=$((PASS+1)); fi
N=$((N+1))
got=$(timeout 20 "$SCRIP" "$D/noportray.pl" </dev/null 2>&1)
want=$'42\nfoo\na(b)\n[1,2]'
if [ "$got" = "$want" ]; then PASS=$((PASS+1)); else printf '  RED  no-portray stdout changed: got [%s] want [%s]\n' "$got" "$want"; FAIL=$((FAIL+1)); fi
echo "=== ARM 2: with portray/1 defined, print/1 consults it -- both modes ==="
cat > "$D/portray.pl" <<'PLEOF'
portray(A) :- atom(A), write(A), write(A).
portray(F) :- float(F), I is truncate(F), write(I).
:- initialization(main).
main :- print(42), nl, print(3.14), nl, print(foo), nl, halt.
PLEOF
want2=$'42\n3\nfoofoo'
N=$((N+1))
got=$(timeout 20 "$SCRIP" "$D/portray.pl" </dev/null 2>/dev/null)
if [ "$got" = "$want2" ]; then PASS=$((PASS+1)); else printf '  m3 RED  portray goal: got [%s] want [%s]\n' "$got" "$want2"; FAIL=$((FAIL+1)); fi
N=$((N+1))
if timeout 60 "$SCRIP" --compile -o "$D/p.s" "$D/portray.pl" >/dev/null 2>&1 \
   && gcc -m64 -no-pie "$D/p.s" -o "$D/p.bin" -L"$ROOT/out" -lscrip_rt -Wl,-rpath,"$ROOT/out" -lm >/dev/null 2>&1; then
  got=$(timeout 20 "$D/p.bin" 2>/dev/null)
  if [ "$got" = "$want2" ]; then PASS=$((PASS+1)); else printf '  m4 RED  portray goal: got [%s] want [%s]\n' "$got" "$want2"; FAIL=$((FAIL+1)); fi
else echo "  m4 RED  portray program failed to compile or link"; FAIL=$((FAIL+1)); fi
echo "=== ARM 3: a program that defines its OWN print/1 keeps it -- the wrapper never shadows the user ==="
cat > "$D/ownprint.pl" <<'PLEOF'
portray(A) :- atom(A), write(A), write(A).
print(X) :- write(mine(X)).
:- initialization(main).
main :- print(foo), nl, halt.
PLEOF
N=$((N+1))
got=$(timeout 20 "$SCRIP" "$D/ownprint.pl" </dev/null 2>/dev/null)
if [ "$got" = "mine(foo)" ]; then PASS=$((PASS+1)); else printf '  RED  user print/1 was shadowed: got [%s] want [mine(foo)]\n' "$got"; FAIL=$((FAIL+1)); fi
[ "$N" -gt 0 ] || { echo "⛔ REFUSE(2): graded nothing"; exit 2; }
echo "PL PRINT/PORTRAY: PASS=$PASS FAIL=$FAIL / $N arms  verdict=$([ $FAIL = 0 ] && echo GREEN || echo RED)"
[ "$FAIL" = 0 ] || exit 1
exit 0
