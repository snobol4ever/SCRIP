#!/bin/bash
# util_swi_cut_refs.sh -- re-cut corpus/packages/prolog/swi_tests/*.ref verdicts from REAL swipl running
# its OWN library(plunit), never our corpus/tests/prolog/plunit.pl shim (which real swipl cannot even
# load -- it refuses to redefine its system set_prolog_flag/2 and current_prolog_flag/2). ceo ruling
# 2026-09-06, task prolog-swi-tests-refs-were-cut-through-a-shim-the-oracle-cannot-load-so-most-expected-verdicts-are-no-tests-ran:
# "the refs are RE-CUT from real swipl running its own plunit -- the oracle is swipl, never our shim."
#
# Per-unit verdict is read from plunit's OWN test_summary/2 bookkeeping (passed/failed/failed_assertions/
# blocked/sto counts) with the identical no_tests / all_passed / else branching plunit's own report/0
# uses (see /usr/lib/swi-prolog/library/plunit.pl ~line 1329) -- not scraped from printed dot positions,
# which are NOT unit-labelled under -q and cannot be safely correlated back to a unit name (a position-
# keyed extraction is exactly the class RULES.md's INSTRUMENT LAWS forbid).
#
# Each (file, unit) pair runs in its OWN swipl process: one file (test_string.pl, unit "string") triggers
# a genuine SWI-Prolog 9.0.4 C-level assertion failure in modify_case_atom (string_upper/2 on a codepoint
# above the Latin-1 range) that SIGABRTs the whole process -- isolating per-unit means that crash cannot
# swallow a sibling unit's verdict. A crash (or timeout, or a named unit absent from source) prints no
# RESULT line and is recorded as EMPTY plus a loud warning -- never guessed as PASS or FAIL, and never
# silently folded into a legitimate "no tests ran under a conforming loader" EMPTY with no distinction.
#
# Usage: util_swi_cut_refs.sh [--write] [base ...]     (default base set: every *.ref already in the dir)
#   --write   overwrite each base's .ref in place (default: print a diff, touch nothing)
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$HERE/lib_oracle_flags.sh"
SWIPL="$(swipl_bin)" || exit 2
S4E="${S4E_HOME:-$(cd "$HERE/../.." && pwd)}"
D="$S4E/corpus/packages/prolog/swi_tests"
[ -d "$D" ] || { echo "⛔ REFUSE(2): $D missing -- the corpus moved; re-pin rather than reinterpret" >&2; exit 2; }

WRITE=0
if [ "${1:-}" = "--write" ]; then WRITE=1; shift; fi

BASES=("$@")
if [ "${#BASES[@]}" -eq 0 ]; then
    while IFS= read -r r; do BASES+=("$(basename "$r" .ref)"); done < <(find "$D" -maxdepth 1 -name "*.ref" | sort)
fi

DRIVER=$(mktemp /tmp/swi_verdict_driver_XXXXXX.pl)
trap 'rm -f "$DRIVER"' EXIT
cat > "$DRIVER" <<'PLEOF'
main :-
    current_prolog_flag(argv, [File, Unit]),
    catch(consult(File), CErr,
          (print_message(error, CErr),
           format("RESULT unit=~w passed=0 failed=0 failed_assertions=0 blocked=0 sto=0~n", [Unit]),
           halt(0))),
    ( catch(run_tests([Unit]), TErr, (print_message(error, TErr), true)) -> true ; true ),
    ( catch(plunit:test_summary(Unit, S), _, fail)
    -> true
    ;  S = plunit{passed:0, failed:0, failed_assertions:0, blocked:0, sto:0}
    ),
    format("RESULT unit=~w passed=~w failed=~w failed_assertions=~w blocked=~w sto=~w~n",
           [Unit, S.passed, S.failed, S.failed_assertions, S.blocked, S.sto]),
    halt(0).
PLEOF

CRASHES=()
MISMATCHES=()

for base in "${BASES[@]}"; do
    f="$D/$base.pl"
    ref="$D/$base.ref"
    [ -f "$f" ] || { echo "⛔ REFUSE: $f missing -- skipping $base" >&2; continue; }
    src_units=$(grep -oP '^\s*:-\s*begin_tests\(\K[a-zA-Z0-9_]+' "$f")
    if [ -f "$ref" ]; then
        ref_units=$(awk '{print $2}' "$ref")
        only_ref=$(comm -13 <(echo "$src_units" | sort -u) <(echo "$ref_units" | sort -u))
        only_src=$(comm -23 <(echo "$src_units" | sort -u) <(echo "$ref_units" | sort -u))
        [ -z "$only_ref" ] || MISMATCHES+=("$base: .ref names not found as begin_tests() in source: $(echo "$only_ref" | tr '\n' ' ')")
        [ -z "$only_src" ] || MISMATCHES+=("$base: source begin_tests() missing from .ref (now added): $(echo "$only_src" | tr '\n' ' ')")
    fi
    OUT=$(mktemp)
    while IFS= read -r unit; do
        [ -n "$unit" ] || continue
        RAW=$(cd "$D" && timeout 20 "$SWIPL" -q -g main -t "halt(9)" "$DRIVER" -- "$f" "$unit" 2>&1)
        LINE=$(printf '%s\n' "$RAW" | grep -m1 '^RESULT ')
        if [ -z "$LINE" ]; then
            CRASHES+=("$base/$unit -- oracle process produced no verdict (crash, SIGABRT, or timeout); recorded EMPTY, NOT a legitimate no-tests-ran signal")
            echo "EMPTY $unit" >> "$OUT"
            continue
        fi
        P=$(printf '%s\n' "$LINE" | grep -oP 'passed=\K[0-9]+')
        F=$(printf '%s\n' "$LINE" | grep -oP 'failed=\K[0-9]+')
        FA=$(printf '%s\n' "$LINE" | grep -oP 'failed_assertions=\K[0-9]+')
        BL=$(printf '%s\n' "$LINE" | grep -oP 'blocked=\K[0-9]+')
        ST=$(printf '%s\n' "$LINE" | grep -oP 'sto=\K[0-9]+')
        if [ $((P + F + FA + BL + ST)) -eq 0 ]; then
            echo "EMPTY $unit" >> "$OUT"
        elif [ $((F + FA + BL + ST)) -eq 0 ]; then
            echo "PASS $unit" >> "$OUT"
        else
            echo "FAIL $unit" >> "$OUT"
        fi
    done <<< "$src_units"
    if [ "$WRITE" -eq 1 ]; then
        mv "$OUT" "$ref"
        echo "WROTE $ref"
    else
        echo "=== $base (dry run -- old vs new, nothing written) ==="
        diff "$ref" "$OUT" && echo "(no change)"
        rm -f "$OUT"
    fi
done

if [ "${#MISMATCHES[@]}" -gt 0 ]; then
    printf '\n⚠ NAME MISMATCHES (source begin_tests vs .ref names):\n'
    printf '  %s\n' "${MISMATCHES[@]}"
fi
if [ "${#CRASHES[@]}" -gt 0 ]; then
    printf '\n⚠ ORACLE DID NOT RETURN A VERDICT for these -- recorded EMPTY, needs a human/FINDING look:\n'
    printf '  %s\n' "${CRASHES[@]}"
fi
