#!/usr/bin/env bash
# LADDER C rung 10b: an asserted clause must be callable with a GROUND and with an
# UNBOUND argument, in BOTH modes. Mode 4 was cured by bb_pool_init(); mode 3 still
# self-binds the unbound argument (infinite c_VARVAL_fn deref). Refuses rc=2 when it
# cannot measure -- never skip-as-success.
set -u
here=$(cd "$(dirname "$0")/.." && pwd)
cd "$here" || exit 2
[ -x ./scrip ] || { echo "REFUSE: no ./scrip in $here -- run make first"; exit 2; }
[ -f out/libscrip_rt.so ] || { echo "REFUSE: no out/libscrip_rt.so -- run make first"; exit 2; }
command -v gcc > /dev/null || { echo "REFUSE: no gcc -- cannot build the mode-4 arm"; exit 2; }
d=$(mktemp -d) || { echo "REFUSE: mktemp failed"; exit 2; }
trap 'rm -rf "$d"' EXIT
{ echo ":- dynamic p/1."; echo ":- initialization(main)."; echo "main :- assertz(p(1)), p(X), write(X), nl."; } > "$d/unbound.pl"
{ echo ":- dynamic q/1."; echo ":- initialization(main)."; echo "main :- assertz(q(1)), q(1), write(yes), nl."; } > "$d/ground.pl"
rc=0
for w in unbound ground; do
  case $w in unbound) want=1 ;; ground) want=yes ;; esac
  g3=$(timeout 20 ./scrip "$d/$w.pl" < /dev/null 2>&1)
  if [ "$g3" = "$want" ]; then echo "PASS m3 $w"; else echo "FAIL m3 $w: want [$want] got [$g3]"; rc=1; fi
  if ! timeout 60 ./scrip --compile -o "$d/$w.s" "$d/$w.pl" < /dev/null > /dev/null 2>&1; then echo "FAIL m4 $w: compile refused"; rc=1; continue; fi
  if ! gcc -no-pie "$d/$w.s" -L out -lscrip_rt -Wl,-rpath,"$here/out" -o "$d/$w.exe" > /dev/null 2>&1; then echo "FAIL m4 $w: link"; rc=1; continue; fi
  g4=$(timeout 20 "$d/$w.exe" < /dev/null 2>&1)
  if [ "$g4" = "$want" ]; then echo "PASS m4 $w"; else echo "FAIL m4 $w: want [$want] got [$g4]"; rc=1; fi
done
[ "$rc" = 0 ] || { echo "rung 10b RED: the dynamic database is not correct in both modes"; exit 1; }
c=$(timeout 900 bash scripts/test_prolog_ladder.sh --to 9 2>&1); crc=$?
[ "$crc" = 0 ] || { echo "FAIL: control arm rungs 0-9 red (rc=$crc)"; printf "%s\n" "$c" | tail -3; exit 1; }
printf "%s\n" "$c" | tail -1
echo "rung 10b GREEN: ground and unbound both correct in BOTH modes, rungs 0-9 green"
