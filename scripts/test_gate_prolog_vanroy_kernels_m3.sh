#!/usr/bin/env bash
# The van Roy kernels must RUN in mode 3, in their COUNTED form. hq_P's I26 reported them as one "m3 crash
# class"; measured, they are FOUR distinct classes (trail-arena refusal, timeout, stack overflow, and an
# unbound variable printed as the answer). Refuses rc=2 when it cannot measure -- never skip-as-success.
#
# ⛔⭐ THE KERNELS ARE GENERATED, NOT READ OUT OF GIT (hq_P 2026-09-13, CEO-567). This gate used to enumerate
#   corpus/benchmarks/prolog/vanroy/*.pl -- 21 checked-in files, each bench/<k>.pl with `main :- l__(N).`
#   frozen in, i.e. the iteration count living inside the artifact under measurement. That directory is
#   RETIRED. The same 21 kernels at the same 21 counts are now built on demand by gen_counted_set() from
#   the pristine bench/ sources plus corpus/benchmarks/prolog/fixed-iter-n.tsv, which is where those N
#   values were lifted to, verbatim by execution, so the retirement re-calibrated nothing.
#   THE NAME STAYS: "van Roy" is the kernels' PROVENANCE (van Roy's Prolog benchmark set), never the name
#   of the directory that used to hold a generated copy of them.
set -u
here=$(cd "$(dirname "$0")/.." && pwd)
root=$(cd "$here/.." && pwd)
[ -x "$here/scrip" ] || { echo "REFUSE: no ./scrip in $here -- run make first"; exit 2; }
. "$here/scripts/lib_prolog_bench.sh" 2>/dev/null || { echo "REFUSE: cannot source lib_prolog_bench.sh -- the one authority for materialising the counted set"; exit 2; }
d=$(mktemp -d "${TMPDIR:-/tmp}/vanroy_m3.XXXXXX") || { echo "REFUSE: cannot create a work dir"; exit 2; }
trap 'rm -rf "$d"' EXIT
n=$(gen_counted_set "$d" scrip) || exit 2
[ "$n" -gt 0 ] || { echo "REFUSE: zero .pl kernels generated -- a census that cannot see its population must not print 0"; exit 2; }
bad=0
for f in "$d"/*.pl; do
  b=$(basename "$f")
  out=$(cd "$d" && timeout 30 "$here/scrip" "$b" < /dev/null 2>&1); rc=$?
  if [ "$rc" != 0 ]; then bad=$((bad+1)); printf 'FAIL %-16s rc=%-4s %s\n' "$b" "$rc" "$(printf '%s' "$out" | head -1 | cut -c1-72)"; fi
done
echo "van Roy m3 census (counted form, generated): kernels=$n non-zero-exit=$bad"
[ "$bad" -eq 0 ] || { echo "GATE RED: $bad of $n van Roy kernels do not run in mode 3"; exit 1; }
command -v swipl > /dev/null || { echo "REFUSE: no swipl -- cannot grade the answer arm"; exit 2; }
for b in queens.pl queens_8.pl; do
  [ -f "$d/$b" ] || continue
  got=$(cd "$d" && timeout 30 "$here/scrip" "$b" < /dev/null 2>/dev/null | head -1)
  want=$(cd "$d" && timeout 30 swipl -q -g halt "$b" < /dev/null 2>/dev/null | head -1)
  [ -n "$want" ] || { echo "REFUSE: oracle produced nothing for $b"; exit 2; }
  [ "$got" = "$want" ] || { echo "GATE RED: $b first answer [$got] != oracle [$want]"; exit 1; }
done
echo "van Roy GREEN: $n generated counted kernels run in mode 3 and the queens answers match the oracle"
