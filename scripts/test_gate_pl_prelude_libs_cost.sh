#!/usr/bin/env bash
# test_gate_pl_prelude_libs_cost.sh -- A PROGRAM NAMING NO VENDORED LIBRARY PAYS NOTHING FOR THEM; ONE NAMING A LIBRARY PAYS
# FOR THAT LIBRARY ALONE (cfo 2026-10-10, row prolog-swi-library-modules-rbtrees-assoc-ordsets-apply-error-pairs-aggregate-
# option-strings-516-swi-cases; CEO-1473's condition: "the compile-time cost measured on a program naming nothing and on one
# naming rbtrees, both gated fail-once pass-once").
#
# THE MECHANISM (src/parsers/prolog/prolog_parse.c pl_libs_append): a vendored library's source is parsed only when the
# program names one of its exports (or a selected library depends on it); its clauses then join the base prelude's pool
# and the existing closure keeps only what the program reaches.
#
# THE INSTRUMENT is callgrind (load-immune: instructions, not time) over `scrip --dump-ast`, which runs the parse and the
# prelude injection and skips emission (90% of a compile).  Two readings per program: the instructions inclusive of
# pl_libs_append, and the calls it makes to prolog_parse (one per library parsed).
#   ARM 1 (names nothing): 0 library parses, and pl_libs_append <= 100,000 instructions.  FAIL-ONCE, two defects: with
#     selection ablated (every vendored library parsed, the defect this arm exists for) it read 1 parse and 2,556,828
#     instructions, red; the first cut built the base prelude's key set before it knew whether any library was selected,
#     4,730,717 for hello world, red; the cure returns before any work (11,735).
#   ARM 2 (names foldl/4, library(apply)): exactly the libraries of apply's dependency closure are parsed (the generator's
#     own deps line: apply and error, whose must_be/2 apply's partition/5 calls), and pl_libs_append <= 3,000,000
#     instructions per library parsed (apply alone measured 2,561,359).  FAIL-ONCE: the first cut checked each library
#     clause against an allocated key set of the whole base prelude, 5,662,131 for apply alone, red here.
#   ARM 3 (names aggregate_all/3 only with templates SCRIP's lowerer answers itself -- count, sum, max, min, bag, set): 0
#     library parses.  library(aggregate) exports aggregate_all/3 for the templates SCRIP does not lower, and its closure
#     is ~80 predicates; pulling it for a count-only program cost 4,010,190,562 instructions to compile against 968,161,311
#     without it (hello world 935,320,040).  pl_aggregate_all_native_kind is the ONE test the lowerer and the injector
#     share.  FAIL-ONCE: with the injector's native-only suppression ablated it read 4 parses (aggregate and its deps).
#   rbtrees, the largest library CEO-1473 names, is not vendored yet (its SSU '=>' rules wait on SCRIP reading SSU); when
#   it lands this arm moves to it.
# A reading that does not find pl_libs_append in the profile refuses rc=2 (a reader that finds nothing reads 0 and passes).
# ~4 s.  rc=0 all arms · rc=1 a red · rc=2 refused (valgrind missing, a stale build, a reader that found nothing).
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
B="$HERE/.."
refuse() { echo "⛔ REFUSED(2) [test_gate_pl_prelude_libs_cost]: $*"; exit 2; }
command -v valgrind >/dev/null 2>&1 || refuse "valgrind missing -- the budget is an instruction count"
[ -x "$B/scrip" ] || refuse "$B/scrip is not built"
"$HERE/util_require_fresh.sh" --gate test_gate_pl_prelude_libs_cost || exit $?
D="$(mktemp -d)" || refuse "no scratch dir"
trap 'rm -rf "$D"' EXIT
cat > "$D/nothing.pl" <<'PL'
:- initialization(main).
main :- append([a], [b], L), member(X, L), write(X), nl, length(L, N), write(N), nl.
PL
cat > "$D/apply.pl" <<'PL'
:- initialization(main).
main :- foldl([X,A0,A]>>(A is A0+X), [1,2,3], 0, S), write(S), nl.
PL
cat > "$D/native.pl" <<'PL'
:- initialization(main).
main :- aggregate_all(count, member(_, [a, b]), C), aggregate_all(sum(X), member(X, [1, 2]), S), write(C-S), nl.
PL
deps="$(python3 "$HERE/util_gen_prolog_prelude_libs.py" --show apply | sed -n 's/^deps: //p')" || refuse "the generator could not show apply"
want=$((1 + $(echo $deps | wc -w)))
reading() {
    ( cd "$D" && valgrind --tool=callgrind --callgrind-out-file="$D/cg.$1" "$B/scrip" --dump-ast "$D/$1.pl" < /dev/null > /dev/null 2>&1 ) || refuse "callgrind over scrip --dump-ast $1.pl failed"
    python3 - "$D/cg.$1" <<'PY'
import sys, re
names, cur, callee, parses, found = {}, None, None, 0, False
for line in open(sys.argv[1]):
    m = re.match(r'^(c?fn)=\((\d+)\)(?: (.*))?', line)
    if m:
        kind, i, nm = m.groups()
        if nm: names[i] = nm.strip()
        if kind == 'fn': cur = names[i]; found = found or cur == 'pl_libs_append'
        else: callee = names[i]
        continue
    m = re.match(r'^calls=(\d+)', line)
    if m and cur == 'pl_libs_append' and callee == 'prolog_parse': parses += int(m.group(1))
print(parses, found)
PY
}
incl_of() { callgrind_annotate --inclusive=yes --auto=no --threshold=100 "$D/cg.$1" 2>/dev/null | grep -E 'prolog_parse\.c:pl_libs_append ' | head -1 | awk '{gsub(",", "", $1); print $1}'; }
red=0
read -r p1 f1 <<< "$(reading nothing)"; i1="$(incl_of nothing)"
read -r p2 f2 <<< "$(reading apply)"; i2="$(incl_of apply)"
read -r p3 f3 <<< "$(reading native)"
[ "$f3" = True ] || refuse "arm 3: pl_libs_append is not in the profile -- the reader found nothing"
[ "$f1" = True ] && [ -n "$i1" ] || refuse "arm 1: pl_libs_append is not in the profile -- the reader found nothing"
[ "$f2" = True ] && [ -n "$i2" ] || refuse "arm 2: pl_libs_append is not in the profile -- the reader found nothing"
if [ "$p1" = 0 ] && [ "$i1" -le 100000 ]; then echo "  ARM 1 GREEN: a program naming no library parsed $p1 libraries, pl_libs_append $i1 instructions (<= 100,000)"
else echo "  ARM 1 RED: a program naming no library parsed $p1 libraries, pl_libs_append $i1 instructions (want 0 and <= 100,000)"; red=1; fi
b2=$((3000000 * want))
if [ "$p2" = "$want" ] && [ "$i2" -le "$b2" ]; then echo "  ARM 2 GREEN: naming foldl/4 parsed $p2 libraries (apply's closure: $want), pl_libs_append $i2 instructions (<= $b2, 3,000,000 a library)"
else echo "  ARM 2 RED: naming foldl/4 parsed $p2 libraries (want apply's closure, $want), pl_libs_append $i2 instructions (want <= $b2, 3,000,000 a library)"; red=1; fi
if [ "$p3" = 0 ]; then echo "  ARM 3 GREEN: a program whose aggregate_all/3 calls SCRIP lowers itself parsed $p3 libraries"
else echo "  ARM 3 RED: a program whose aggregate_all/3 calls SCRIP lowers itself parsed $p3 libraries (want 0)"; red=1; fi
[ "$red" = 0 ] && echo "PASS test_gate_pl_prelude_libs_cost"
exit "$red"
