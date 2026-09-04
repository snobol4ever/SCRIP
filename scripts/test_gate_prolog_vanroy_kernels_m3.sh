#!/usr/bin/env bash
# The van Roy kernels must RUN in mode 3. hq_P's I26 reported them as one "m3 crash class";
# measured, they are FOUR distinct classes (trail-arena refusal, timeout, stack overflow, and an
# unbound variable printed as the answer). Refuses rc=2 when it cannot measure -- never skip-as-success.
set -u
here=$(cd "$(dirname "$0")/.." && pwd)
root=$(cd "$here/.." && pwd)
d="$root/corpus/benchmarks/prolog/vanroy"
[ -x "$here/scrip" ] || { echo "REFUSE: no ./scrip in $here -- run make first"; exit 2; }
[ -d "$d" ] || { echo "REFUSE: no van Roy corpus at $d"; exit 2; }
n=$(ls "$d"/*.pl 2>/dev/null | wc -l); [ "$n" -gt 0 ] || { echo "REFUSE: zero .pl kernels found -- a census that cannot see its population must not print 0"; exit 2; }
bad=0
for f in "$d"/*.pl; do
  b=$(basename "$f")
  out=$(cd "$d" && timeout 30 "$here/scrip" "$b" < /dev/null 2>&1); rc=$?
  if [ "$rc" != 0 ]; then bad=$((bad+1)); printf 'FAIL %-16s rc=%-4s %s\n' "$b" "$rc" "$(printf '%s' "$out" | head -1 | cut -c1-72)"; fi
done
echo "van Roy m3 census: kernels=$n non-zero-exit=$bad"
[ "$bad" -eq 0 ] || { echo "GATE RED: $bad of $n van Roy kernels do not run in mode 3"; exit 1; }
command -v swipl > /dev/null || { echo "REFUSE: no swipl -- cannot grade the answer arm"; exit 2; }
for b in queens.pl queens_8.pl; do
  [ -f "$d/$b" ] || continue
  got=$(cd "$d" && timeout 30 "$here/scrip" "$b" < /dev/null 2>/dev/null | head -1)
  want=$(cd "$d" && timeout 30 swipl -q -g halt "$b" < /dev/null 2>/dev/null | head -1)
  [ -n "$want" ] || { echo "REFUSE: oracle produced nothing for $b"; exit 2; }
  [ "$got" = "$want" ] || { echo "GATE RED: $b first answer [$got] != oracle [$want]"; exit 1; }
done
echo "van Roy GREEN: $n kernels run in mode 3 and the queens answers match the oracle"
