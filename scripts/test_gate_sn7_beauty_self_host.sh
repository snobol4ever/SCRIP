#!/usr/bin/env bash
# scripts/test_gate_sn7_beauty_self_host.sh — SN-7 gate:
# every *_driver.sno in corpus/programs/snobol4/beauty_suite/, under BOTH
# real modes (--run = m3 BINARY, --compile = m4 TEXT),
# diff=0 vs its pre-baked .ref file (SPITBOL ground truth where valid; some
# drivers have .ref files that reflect correct behavior SPITBOL itself fails
# on — see RULES.md on .ref authority).
#
# B-9 FIX (BOARD, this session): the loop previously read
# `for mode in --run --run --run` (before that, `--interp --interp --run`,
# mechanically flattened when --interp was eradicated project-wide) — it
# NEVER invoked --compile, so this gate has apparently never once measured
# mode 4 on beauty_suite. FAIL counts before this fix were exactly 3x the
# true --run-only fail count, all under the label "--run". Fixed to name
# both modes explicitly so a future flatten-style edit cannot silently
# collapse it again.
#
# Self-contained per RULES.md: paths derived from $0; no env deps.
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${SCRIP:-$HERE/../scrip}"
CORPUS="${CORPUS:-$S4E/corpus}"
BEAUTY="$CORPUS/programs/snobol4/beauty_suite"
TIMEOUT="${TIMEOUT:-30}"

if [ ! -x "$SCRIP" ]; then
    echo "SKIP scrip not built at $SCRIP"
    exit 0
fi
if [ ! -d "$BEAUTY" ]; then
    echo "SKIP corpus not populated at $CORPUS"
    exit 0
fi

PASS=0
FAIL=0
FAILS=""

for sno in "$BEAUTY"/*_driver.sno; do
    [ ! -f "$sno" ] && continue
    name=$(basename "$sno" .sno)
    ref="$BEAUTY/${name}.ref"
    [ ! -f "$ref" ] && continue
    for mode in --run --compile; do
        got=$(SNO_LIB="$BEAUTY" timeout "$TIMEOUT" "$SCRIP" $mode "$sno" < /dev/null 2>/dev/null || true)
        if diff <(printf '%s\n' "$got") "$ref" > /dev/null 2>&1; then
            PASS=$((PASS + 1))
        else
            FAIL=$((FAIL + 1))
            FAILS="$FAILS $name($mode)"
        fi
    done
done

echo "PASS=$PASS FAIL=$FAIL"
[ -n "$FAILS" ] && echo "FAILS:$FAILS"
[ "$FAIL" -eq 0 ]
