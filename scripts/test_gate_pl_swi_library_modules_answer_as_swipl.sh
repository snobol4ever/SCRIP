#!/usr/bin/env bash
# test_gate_pl_swi_library_modules_answer_as_swipl.sh -- THE VENDORED SWI LIBRARIES ANSWER AS SWIPL, IN BOTH MODES (cfo
# 2026-10-10, row prolog-swi-library-modules-rbtrees-assoc-ordsets-apply-error-pairs-aggregate-option-strings-516-swi-cases;
# CEO-1473).
#
# THE CURE: swipl 9.0.4's own library(apply), library(error), library(pairs), library(heaps), library(aggregate) and
# library(strings), vendored verbatim under src/parsers/prolog/prelude/ and embedded by util_gen_prolog_prelude_libs.py,
# reach a program that names one of their exports; the C-side helpers their closures call and SCRIP lacked are hand-typed
# in the base prelude ('$is_char_list'/2, '$is_code_list'/2, rational/1,3, is_dict/1,2, atomics_to_string/3); library
# clauses read their "..." as strings, as SWI reads them, whatever the program's double_quotes flag; aggregate_all/3 keeps
# SCRIP's own lowering for count, sum, max, min, bag and set and reaches the library's clauses for every other template.
#
# THE ARM: one witness, each line an export of a vendored library or one of its error terms, cut against swipl -q in this
# run and diffed in mode 3 and mode 4.  RED BEFORE (the parent): every line names existence_error for its predicate, or the
# run refuses aggregate_all at "rung 8".  aggregate_all's count, sum, max, bag and set are called DIRECTLY (agg_*/1), the
# shape SCRIP's lowerer answers; a META-CALLED aggregate_all(sum(X), ...) reaches SWI's clause, which needs nb_setarg/3,
# which SCRIP lacks (a row of its own; the parent refused every meta-called aggregate_all).  A lambda of five arguments is
# yall's >>/7, which this landing does not vendor; foldl/6 is graded with a plain predicate.  dedent_lines/3 calls option/3
# (library(option), written in SSU '=>' rules) and joins this witness when SCRIP reads SSU.
# rc=0 both modes agree with swipl · rc=1 a divergence · rc=2 REFUSAL (no swipl, no build).  ~4 s.
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
B="$HERE/.."
refuse() { echo "⛔ REFUSED(2) [test_gate_pl_swi_library_modules_answer_as_swipl]: $*"; exit 2; }
command -v swipl >/dev/null 2>&1 || refuse "no swipl on PATH -- the oracle"
[ -x "$B/scrip" ] || refuse "$B/scrip is not built"
[ -f "$B/out/libscrip_rt.so" ] || refuse "out/libscrip_rt.so is not built -- mode 4 cannot link"
"$HERE/util_require_fresh.sh" --gate test_gate_pl_swi_library_modules_answer_as_swipl || exit $?
D="$(mktemp -d)" || refuse "no scratch dir"
trap 'rm -rf "$D"' EXIT
cat > "$D/w.pl" <<'PL'
:- set_prolog_flag(double_quotes, string).
:- initialization(main).
t(G) :- catch((call(G) -> \+ \+ (numbervars(G, 0, _), writeq(G)) ; writeq(no)), error(E, _), \+ \+ (numbervars(E, 0, _), writeq(caught(E)))), nl.
p(X) :- X > 1.
c(X, Y) :- X > 1, Y is X * 10.
age(peter, 7). age(mary, 9). age(john, 7).
add3(A, B, C, S0, S) :- S is S0 + A + B + C.
agg_count(C) :- aggregate_all(count, member(_, [a,b,c]), C).
agg_sum(S) :- aggregate_all(sum(Q), member(Q, [1,2,3]), S).
agg_max(M) :- aggregate_all(max(R), member(R, [3,9,2]), M).
agg_bag(B) :- aggregate_all(bag(Z), member(Z, [c,a,c]), B).
agg_set(S) :- aggregate_all(set(Z), member(Z, [c,a,c]), S).
main :-
    t(partition(p, [1,2,3], _, _)), t(partition([X,O]>>compare(O,X,2), [1,2,3], _, _, _)), t(convlist(c, [1,2,3], _)),
    t(foldl([E,A0,A]>>(A is A0+E), [1,2,3], 0, _)), t(foldl([E1,E2,B0,B]>>(B is B0+E1*E2), [1,2], [3,4], 0, _)),
    t(foldl(add3, [1,4], [2,5], [3,6], 0, _)), t(scanl([S,S0,S1]>>(S1 is S0+S), [1,2,3], 0, _)),
    t(must_be(integer, a)), t(must_be(positive_integer, 0)), t(must_be(nonneg, 3)), t(must_be(list(integer), [1,a])),
    t(must_be(oneof([a,b]), c)), t(must_be(boolean, x)), t(must_be(between(1,3), 5)), t(must_be(chars, [a,b])),
    t(must_be(text, f(x))), t(must_be(rational, 1)), t(must_be(atom, _)), t(must_be(no_such_type, 1)),
    t(is_of_type(list(atom), [a,b])), t(type_error(integer, a)), t(domain_error(positive, -1)),
    t(pairs_keys_values(_, [a,b], [1,2])), t(pairs_keys([a-1,b-2], _)), t(pairs_values([a-1,b-2], _)),
    t(transpose_pairs([a-1,b-2], _)), t(map_list_to_pairs(atom_length, [ab,c], _)),
    t((list_to_heap([3-c,1-a,2-b], H), get_from_heap(H, _, _, H1), heap_size(H1, _), heap_to_list(H1, _))),
    t((empty_heap(E0), add_to_heap(E0, 5, x, E1), add_to_heap(E1, 2, y, E2), min_of_heap(E2, _, _))),
    t(aggregate(count, Y^age(Y, _), _)), t(aggregate(sum(G), N^age(N, G), _)), t(aggregate(bag(N2), age(N2, 7), _)),
    t(aggregate(max(G2, N3), age(N3, G2), _)), t(aggregate(count, N4, age(N4, _), _)),
    t(aggregate_all(term(count, bag(V)), between(1, 3, V), _)), t(aggregate_all(r(max(M)), member(M, [3,9,2]), _)),
    t(agg_count(_)), t(agg_sum(_)), t(agg_max(_)), t(agg_bag(_)), t(agg_set(_)), t(aggregate_all(count, member(_, [a,b]), _)),
    t(aggregate_all(count, K, member(K, [a,b,a]), _)),
    t(string_lines(_, ["a", "b"])), t(string_lines("x\ny\n", _)),
    t(indent_lines("> ", "a\nb", _)).
PL
( cd "$D" && timeout 30 swipl -q -t halt w.pl < /dev/null > want 2> /dev/null )
[ -s "$D/want" ] || refuse "swipl printed nothing for the witness"
( cd "$D" && timeout 30 "$B/scrip" --run w.pl < /dev/null > m3 2> m3.err )
( cd "$D" && timeout 60 "$B/scrip" --compile -o w.s w.pl < /dev/null 2> m4.err && gcc -no-pie w.s -L"$B/out" -lscrip_rt -lm -Wl,-rpath,"$B/out" -o w 2>> m4.err && timeout 30 ./w < /dev/null > m4 2>> m4.err )
n=$(wc -l < "$D/want"); red=0
for m in m3 m4; do
    if cmp -s "$D/want" "$D/$m"; then echo "  $m GREEN: $n of $n lines as swipl"
    else echo "  $m RED: $(diff "$D/want" "$D/$m" 2>/dev/null | grep -c '^<') of $n lines differ from swipl"; diff "$D/want" "$D/$m" 2>/dev/null | head -10 | sed 's/^/    /'; red=1; fi
done
[ "$red" = 0 ] && echo "PASS test_gate_pl_swi_library_modules_answer_as_swipl"
exit "$red"
