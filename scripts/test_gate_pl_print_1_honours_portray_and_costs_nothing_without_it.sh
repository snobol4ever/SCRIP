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
# fake by measuring it after the fact, so the no-portray .s and stdout are captured BEFORE any
# portray-defining program is compiled in this script, and the .s is compared to a PINNED md5 recorded from
# the PRE-CURE compiler, measured 2026-09-13 by stashing the cure, rebuilding, and emitting the same
# program. ⛔ THE md5 IS OF THE PATH-NORMALISED TEXT, not of the file: the emitted .s embeds the absolute
# source path, so a raw md5 pinned from one directory reds in every other one -- which is what the first
# cut of this gate did, and it is a gate that fails for a reason that has nothing to do with what it
# grades. Arm 1b states a narrower claim that cannot drift with the emitter: a program with no portray/1
# mentions portray NOWHERE in its emission. ⛔ It deliberately does NOT grep for "print/1": that string is
# a startup predicate-name the compiler emitted long before this cure, so greping it convicts the
# pre-cure compiler too -- an arm that reds on a clean tree is measuring the wrong thing, and the
# path-normalised md5 above already pins the whole emission byte for byte.
#
# rc 0 green · 1 red · 2 could not measure.  Self-contained; run from anywhere.
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"; cd "$ROOT"
SCRIP="${SCRIP_BIN:-$ROOT/scrip}"
[ -x "$SCRIP" ] || { echo "⛔ REFUSE(2): no scrip at $SCRIP"; exit 2; }
D=$(mktemp -d) || { echo "⛔ REFUSE(2): no tmpdir"; exit 2; }
trap 'rm -rf "$D"' EXIT
PASS=0; FAIL=0; N=0
NOPORTRAY_S_MD5=5bc05f271884818139d94dfcce22ecc2   # path-normalised; pre-cure value, see banner
cat > "$D/noportray.pl" <<'PLEOF'
:- initialization(main).
main :- print(42), nl, print(foo), nl, print(a(b)), nl, print([1,2]), nl.
PLEOF
echo "=== ARM 1 (FIRST, and the one that can be faked by running it last): no portray/1 costs nothing ==="
N=$((N+1))
if ! timeout 60 "$SCRIP" --compile -o "$D/np.s" "$D/noportray.pl" >/dev/null 2>&1; then
  echo "  RED  no-portray program failed to compile"; FAIL=$((FAIL+1))
else
  got=$(sed "s|$D|@|g" "$D/np.s" | md5sum | cut -d' ' -f1)
  if [ "$got" = "$NOPORTRAY_S_MD5" ]; then PASS=$((PASS+1))
  else echo "  RED  the minted wrapper changed the EMISSION of a program with no portray/1: md5 $got want $NOPORTRAY_S_MD5"; FAIL=$((FAIL+1)); fi
fi
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
