#!/bin/bash
# SNOBOL4 IMMEDIATE deferred capture with a target that can never yield a name -- P $ *(N = N + 1) --
# must FAIL THE MATCH, not abort. SPITBOL evaluates the expression and then fails the match; SCRIP
# reaches rt_assign_var with a value where a variable is required and dies:
#   [IDX] BOMB rt_assign_var: lvalue is not a variable (dtype=3)   rc=134, core dumped
# Row snobol4-immediate-deferred-capture-with-a-non-name-target-bombs-instead-of-failing-the-match.
#
# ⭐ THE CONDITIONAL TWIN IS ALREADY CORRECT AND IS GRADED HERE AS A CONTROL ARM: P . *(N = N + 1)
# agrees with the oracle today (pattern_match.c:734, `strict && !by_name` sets rc=1 so the match fails
# at END). It is both the model for the cure and the thing a cure must not break -- if the immediate
# form is fixed by making every non-name target abort earlier, this arm goes red and says so.
#
# The expectation is not hardcoded: the oracle is RUN, so this gate cannot drift from it. sbl needs
# -bf (case-sensitive, matching SCRIP) and a writable listing sink, or it dumps its listing to stdout.
# ⛔ REFUSES rc=2 unless all 4 runs (2 witnesses x 2 modes) are graded, and rc=2 if the oracle is absent
# or its own run fails -- a missing oracle prints a full, plausible, entirely false table.
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"; cd "$ROOT" || exit 2
. "$HERE/lib_oracle_flags.sh" 2>/dev/null || { echo "⛔ GATE REFUSES (rc=2): lib_oracle_flags.sh not sourceable"; exit 2; }
[ -x ./scrip ] && [ -f out/libscrip_rt.so ] || { echo "⛔ GATE REFUSES (rc=2): scrip/runtime not built at $ROOT"; exit 2; }
SBL="/home/resources/x64/bin/sbl"; [ -x "$SBL" ] || { echo "⛔ GATE REFUSES (rc=2): correctness oracle $SBL is missing -- a board without an oracle prints a false all-FAIL table"; exit 2; }
d=$(mktemp -d) || exit 2; trap 'rm -rf "$d"' EXIT
sink="$(sbl_listing_sink_flag "$d")" || { echo "⛔ GATE REFUSES (rc=2): no writable listing sink for sbl"; exit 2; }
mk() { printf '        N = 0\n        P = LEN(1) %s *(N = N + 1)\n        "ABC" P                             :S(YES)F(NO)\nYES     OUTPUT = "MATCH"                    :(SHOW)\nNO      OUTPUT = "NOMATCH"\nSHOW    OUTPUT = "N=" N\nEND\n' "$1" > "$2"; }
mk '$' "$d/immediate.sno"; mk '.' "$d/conditional.sno"
graded=0; bad=0
for w in immediate conditional; do
    want=$("$SBL" -bf "$sink" "$d/$w.sno" </dev/null 2>&1); orc=$?
    [ "$orc" = 0 ] && [ -n "$want" ] || { echo "⛔ GATE REFUSES (rc=2): oracle run for $w exited $orc -- cannot measure"; exit 2; }
    o=$(timeout 20 ./scrip "$d/$w.sno" </dev/null 2>&1); r=$?; graded=$((graded+1))
    if [ "$r" != 0 ] || [ "$o" != "$want" ]; then
        echo "  ⛔ RED m3 $w: rc=$r out=[$(printf '%s' "$o" | tr '\n' '|' | cut -c1-90)] -- oracle rc=0 out=[$(printf '%s' "$want" | tr '\n' '|')]"; bad=1
    fi
    if timeout 120 ./scrip --compile -o "$d/$w.s" "$d/$w.sno" </dev/null >/dev/null 2>&1 \
       && gcc -m64 -no-pie "$d/$w.s" -o "$d/$w.m4" -L./out -lscrip_rt -Wl,-rpath,"$ROOT/out" -lm -lpthread 2>/dev/null; then
        o=$(timeout 20 "$d/$w.m4" </dev/null 2>&1); r=$?; graded=$((graded+1))
        if [ "$r" != 0 ] || [ "$o" != "$want" ]; then
            echo "  ⛔ RED m4 $w: rc=$r out=[$(printf '%s' "$o" | tr '\n' '|' | cut -c1-90)] -- oracle rc=0 out=[$(printf '%s' "$want" | tr '\n' '|')]"; bad=1
        fi
    else
        graded=$((graded+1)); echo "  ⛔ RED m4 $w: does not compile/link"; bad=1
    fi
done
[ "$graded" = 4 ] || { echo "⛔ GATE REFUSES (rc=2): graded $graded of the 4 required runs"; exit 2; }
[ "$bad" = 0 ] || { echo "⛔ GATE RED [sno_immediate_capture_non_name_target]: the immediate form still does not fail the match cleanly"; exit 1; }
echo "✅ GATE OK [sno_immediate_capture_non_name_target]: both capture forms match sbl -bf by VALUE, BOTH modes"
