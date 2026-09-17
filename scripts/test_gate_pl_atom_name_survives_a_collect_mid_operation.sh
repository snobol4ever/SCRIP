#!/usr/bin/env bash
# stale-binary preflight -- util_require_fresh.sh is the ONE authority; no second copy of the staleness rule here.
"$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/util_require_fresh.sh" --gate "$(basename "${BASH_SOURCE[0]}" .sh)" || exit $?
# test_gate_pl_atom_name_survives_a_collect_mid_operation.sh -- AN ATOM'S NAME, READ INTO A RAW C POINTER, MUST
# STILL BE THAT NAME AFTER A COLLECTION LANDS MID-OPERATION (hq_prolog 2026-09-16, row
# prolog-atom-names-live-in-71-c-locals-that-no-root-scan-sees-so-deleting-pinning-dangles-every-one; cto ruling
# 2026-09-14 on the cure and on what this gate grades; census by hq_V).
#
# ⛔⭐ THE TWO CLAUSES GRADE DIFFERENT THINGS, AND CLAUSE 2 IS RED TODAY FOR A REASON THAT IS NOT THIS ROW'S.
# Clause 1 grades the PRECONDITION the cto ruled: atom-name storage is OUTSIDE the collected heap. It is this
# row's fail-once lever and it went red -> green on the cure. The cto ruled option one (storage leaves the heap)
# over option two (re-read through prolog_atom_name after every point that can collect) because option two is a
# property at 56 call sites that nothing but the next author's memory can hold: a claim spanning two sites can be
# held by a check, a claim spanning fifty-six cannot be held at all.
# ⛔⛔ CLAUSE 2 IS A REGRESSION GUARD AND IT CANNOT PASS UNTIL ANOTHER ROW LANDS, WHICH IS WHY THIS GATE IS NOT
# WIRED. Grading 20 runs per mode under SCRIP_GC_STRESS=1 trips a SIGSEGV that has nothing to do with atom names:
# the collection point walks g_call_args (rt.c:2050) and hands rt_gc_visit_descr a word that is not a DESCR
# (gc_heap.c:419). Row prolog-twelve-master-entries-sigsegv-under-gc-stress-in-the-call-args-root-walk owns it,
# with corpus/benchmarks/prolog/bench/queens.pl as a 20/20 deterministic witness. ⭐ THAT ATTRIBUTION WAS MEASURED,
# NOT INFERRED FROM THE SHAPE: the atom-name cure left the rate unchanged (queens 20/20 before and after, this
# witness 4/30 before and 3/30 after), which is exactly how two defects that look alike at the surface were told
# apart. ⛔ WIRE THIS GATE BLOCKING IN THE LANDING THAT CURES THAT ROW -- not before, because red-on-arrival is not
# a gate, and not later, because a gate that is still `ls`-able reads as a gate that still runs.
#
# ⛔⭐ WHY CLAUSE 2 CANNOT BE THE LEVER, AND THIS IS THE PART WORTH READING. The collector is a SLIDING compactor:
# gc_heap.c memmoves live blocks down and DOES NOT overwrite what it vacates. So a pointer left behind by a move
# keeps reading the OLD BYTES, which are still the correct string until something else is allocated over them. A
# dangling atom-name read therefore returns a PLAUSIBLE answer, and an output comparison -- any number of runs of
# it -- cannot tell that class from a correct one. SCRIP_GC_POISON exists for exactly this: it fills the vacated
# tail with 0xDB after the slide, so a stale pointer into freed space reads garbage instead of the truth.
#
# ⛔ THE POSITIVE CONTROL IS MANDATORY AND A DEAD SWITCH REFUSES rc=2. This row's own history is the reason: its
# first witness was self-identical in every collector configuration with moved=0B everywhere -- a switch that was
# not on, reading exactly like a clean result. Before grading anything this gate asserts, from [ZGC-WALK] and
# [ZGC-POISON] under SCRIP_ZETA_TELEM=1, that blocks really moved AND that vacated space really was filled.
#
# ⛔ THE REF IS CUT FROM swipl AT RUN TIME, NEVER FROM OUR OWN OUTPUT (CEO-753). No swipl, no grade: rc=2.
#
# ⛔ WHAT THE POISON DOES NOT COVER, so the next seat does not over-trust a green: it fills the vacated TAIL only.
# A block that slid DOWN leaves its old address inside the still-live region, where other slid data now sits, so a
# stale pointer there still reads plausible bytes. Poison catches the tail class; it does not catch that one.
# EXIT: 0 both clauses pass · 1 the precondition is red or a run diverged from the oracle · 2 REFUSED to grade.
# ⛔ NOT IN make test / test-sequential, BY DECISION: see the clause-2 note above. Running it by hand today gives
# 1 of 3 checks green (the precondition) and clause 2 red on the other row's crash.
set -uo pipefail
GATE_NAME=test_gate_pl_atom_name_survives_a_collect_mid_operation
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP:-$ROOT/scrip}"; RT_DIR="${RT_DIR:-$ROOT/out}"; SWIPL="${SWIPL:-/usr/bin/swipl}"
ATOMC="$ROOT/src/parsers/prolog/prolog_atom.c"
ITERS="${ITERS:-20}"; STRESS="${SCRIP_GC_STRESS_VALUE:-1}"; CHURN="${WITNESS_CHURN:-300}"; MODES="${MODES:-m3,m4}"
refuse(){ echo "⛔ REFUSED(2) [$GATE_NAME]: $*" >&2; exit 2; }
[ -x "$SCRIP" ] || refuse "no scrip at $SCRIP -- a missing binary prints a full, plausible, entirely false board"
[ -x "$SWIPL" ] || refuse "no swipl oracle at $SWIPL -- the ref is cut from the oracle at run time or not at all"
[ -f "$ATOMC" ] || refuse "no $ATOMC -- the precondition clause has nothing to read"
fails=0; checks=0
ck(){ checks=$((checks+1)); if [ "$1" = ok ]; then printf '  ok    %s\n' "$2"; else printf '  FAIL  %s\n' "$2"; fails=$((fails+1)); fi; }
echo "CLAUSE 1 -- THE PRECONDITION: atom-name storage is outside the collected heap"
NAMEALLOC="$(grep -nE 'rt_heap_strdup|rt_heap_alloc|rt_pinned_(alloc|realloc|strdup)' "$ATOMC" || true)"
if [ -n "$NAMEALLOC" ]; then
    ck no "prolog_atom.c still takes atom-name storage from the collected heap: $(printf '%s' "$NAMEALLOC" | tr '\n' ' ')"
else
    ck ok "prolog_atom.c holds no collected-heap allocator -- a name pointer cannot be invalidated by a slide"
fi
W="$(mktemp -d "${TMPDIR:-/tmp}/pl_atom_name.XXXXXX")" || refuse "mktemp failed"
trap 'rm -rf "$W"' EXIT
cat > "$W/w.pl" <<PL
n_atom(N, A) :- number_codes(N, C), atom_codes(A0, C), atom_concat(nm_, A0, A).
churn(0) :- !.
churn(N) :- n_atom(N, A), atom_length(A, L), L > 0, N1 is N - 1, churn(N1).
:- dynamic held/2.
main :-
    churn($CHURN),
    n_atom(11, A), n_atom(22, B),
    functor(T, A, 3), churn($CHURN), functor(T, F, N), write(F/N), nl,
    atom_length(A, LA), churn($CHURN), write(LA), nl,
    atom_chars(B, Cs), churn($CHURN), write(Cs), nl,
    assertz(held(A, B)), churn($CHURN), held(X, Y), write(X-Y), nl,
    findall(Z, member(Z, [A,B,A]), Zs), churn($CHURN), write(Zs), nl,
    sort([B,A,B], Ss), churn($CHURN), write(Ss), nl,
    atom_concat(A, B, AB), churn($CHURN), write(AB), nl,
    T =.. [Hd|_], churn($CHURN), write(Hd), nl,
    ( A @< B -> write(lt) ; write(ge) ), nl,
    atom_codes(A, ACs), churn($CHURN), atom_codes(A2, ACs), write(A2), nl,
    write(A), nl, write(B), nl.
:- initialization(main).
PL
REF="$W/ref.txt"
if ! timeout 60 "$SWIPL" -q -g true -t halt "$W/w.pl" </dev/null > "$REF" 2>"$W/ref.err"; then
    refuse "the swipl oracle did not run the witness: $(head -3 "$W/ref.err" | tr '\n' ' ')"
fi
[ -s "$REF" ] || refuse "the oracle produced an EMPTY ref -- an empty expectation makes every run trivially pass"
grep -qx 'nm_22' "$REF" || refuse "the oracle's own answer is not the expected shape; witness or oracle changed"
echo "ORACLE REF (swipl, cut at run time): $(tr '\n' ' ' < "$REF")"
echo "POSITIVE CONTROL -- the collector must really move blocks AND really poison what it vacates"
TEL="$(SCRIP_GC_STRESS="$STRESS" SCRIP_GC_POISON=1 SCRIP_ZETA_TELEM=1 timeout 60 "$SCRIP" "$W/w.pl" </dev/null 2>&1 >/dev/null || true)"
MOVED="$(printf '%s' "$TEL" | grep -oE 'moved=[0-9]+B' | sed 's/[^0-9]//g' | awk '{s+=$1} END{print s+0}')"
VACD="$(printf '%s' "$TEL" | grep -oE 'vacated=[0-9]+B' | sed 's/[^0-9]//g' | awk '{s+=$1} END{print s+0}')"
[ "$MOVED" -gt 0 ] || refuse "moved=${MOVED}B under SCRIP_GC_STRESS=$STRESS -- nothing relocated, so a green below would prove nothing (this row's first witness died exactly here)"
[ "$VACD"  -gt 0 ] || refuse "vacated=${VACD}B under SCRIP_GC_POISON=1 -- the poison never fired, so a stale read would still find its old bytes intact"
echo "  control: moved=${MOVED}B relocated, vacated=${VACD}B filled with 0xDB"
echo "CLAUSE 2 -- THE REGRESSION GUARD: $ITERS runs per mode, byte-equal to the oracle, under stress AND poison"
M4DIR="$W/m4"; mkdir -p "$M4DIR"
m4_build(){
    rm -f "$M4DIR/w.s" "$M4DIR/w.bin"
    timeout 120 "$SCRIP" --compile -o "$M4DIR/w.s" "$W/w.pl" </dev/null >"$M4DIR/compile.log" 2>&1 || return 1
    [ -s "$M4DIR/w.s" ] || return 1
    timeout 120 gcc -no-pie "$M4DIR/w.s" -o "$M4DIR/w.bin" -L"$RT_DIR" -lscrip_rt -Wl,-rpath,"$RT_DIR" -lm >>"$M4DIR/compile.log" 2>&1 || return 1
    [ -x "$M4DIR/w.bin" ] || return 1
    return 0
}
run_mode_m3(){ SCRIP_GC_STRESS="$STRESS" SCRIP_GC_POISON=1 timeout 60 "$SCRIP" "$W/w.pl" </dev/null 2>&1; }
run_mode_m4(){ SCRIP_GC_STRESS="$STRESS" SCRIP_GC_POISON=1 timeout 60 "$M4DIR/w.bin" </dev/null 2>&1; }
for m in ${MODES//,/ }; do
    if [ "$m" = m4 ]; then
        m4_build || refuse "mode 4 could not build the witness -- refusing rather than grading a stale or absent artifact: $(tail -3 "$M4DIR/compile.log" 2>/dev/null | tr '\n' ' ')"
    fi
    bad=0; first=""
    for i in $(seq 1 "$ITERS"); do
        out="$(run_mode_$m)"
        if [ "$out" != "$(cat "$REF")" ]; then bad=$((bad+1)); [ -n "$first" ] || first="iter $i: $(printf '%s' "$out" | head -2 | tr '\n' ' ')"; fi
    done
    if [ "$bad" -eq 0 ]; then ck ok "$m: $ITERS/$ITERS runs byte-equal to the swipl ref under SCRIP_GC_STRESS=$STRESS SCRIP_GC_POISON=1"
    else ck no "$m: $bad of $ITERS runs DIVERGED from the swipl ref -- $first"; fi
done
echo "------------------------------------------------------------"
if [ "$fails" -eq 0 ]; then echo "✅ GATE PASS: atom-name storage is outside the collected heap and every raw-pointer site survives a mid-operation collection ($checks check(s))"; exit 0; fi
echo "⛔ GATE FAIL: $fails of $checks check(s) failed"; exit 1
