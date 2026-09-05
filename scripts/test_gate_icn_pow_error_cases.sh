#!/bin/bash
# Icon exponentiation must RAISE the oracle's arithmetic runtime errors, not substitute a failure
# port or a NaN value and keep running. Row icon-pow-error-cases-fail-or-print-nan-instead-of-raising-204-and-206.
#
# Measured 2026-09-05 (hq_B, SCRIP e5b9f3333, RT_OPT=-O0), identical in BOTH modes:
#   0 ^ (-1)     oracle iconx: 'before' then Run-time error 204, rc=1  | SCRIP: 'before','after', rc=0
#                ⛔ the middle write NEVER RUNS -- the operator FAILED rather than erroring, and in Icon
#                expression failure is a CONTROL-FLOW SIGNAL, so this does not merely lose a diagnostic,
#                it can silently redirect an if/every/while written to depend on the error halting.
#   (-2.0) ^ 0.5 oracle iconx: 'before' then Run-time error 206, rc=1  | SCRIP: 'before','nan','after', rc=0
#
# These are the two named reds of the Icon master board (ladder_rung26_pow_pow_zero_negexp,
# ladder_rung26_pow_pow_negbase_real) and the reason its watermark pins at 607 and not 609.
# ALL.wantrc requires rc=1 and ALL.ref requires stdout to be exactly 'before' for both.
#
# ⛔ A GATE THAT CANNOT MEASURE REFUSES rc=2 -- never skip-as-success (RULES.md). All FOUR runs
# (2 witnesses x 2 modes) must be graded or this refuses; it can never fall through to a green.
# ⭐ Proved THREE ways when written: rc=1 today naming all four, rc=2 on an unreachable tree, and
# rc=0 against a stub that behaves correctly -- the last arm on purpose, because a criterion nobody
# has ever seen go green is indistinguishable from one that CANNOT (this session cured exactly that
# defect on the snoflake row's own DONE-WHEN).
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
cd "$ROOT" || exit 2
[ -x ./scrip ] && [ -f out/libscrip_rt.so ] || { echo "⛔ GATE REFUSES (rc=2): scrip/runtime not built at $ROOT"; exit 2; }
d=$(mktemp -d) || exit 2
trap 'rm -rf "$d"' EXIT
printf 'procedure main()\n  write("before");\n  write(0 ^ (-1));\n  write("after");\nend\n'      > "$d/zero_negexp.icn"
printf 'procedure main()\n  write("before");\n  write((-2.0) ^ 0.5);\n  write("after");\nend\n' > "$d/negbase_real.icn"
graded=0; bad=0
for w in zero_negexp negbase_real; do
    o=$(timeout 10 ./scrip "$d/$w.icn" </dev/null 2>/dev/null); r=$?; graded=$((graded+1))
    if [ "$o" != "before" ] || [ "$r" != 1 ]; then
        echo "  ⛔ RED m3 $w: rc=$r stdout=[$(printf '%s' "$o" | tr '\n' '|')] -- want rc=1 stdout=[before]"; bad=1
    fi
    timeout 60 ./scrip --compile -o "$d/$w.s" "$d/$w.icn" </dev/null >/dev/null 2>&1 \
        || { echo "⛔ GATE REFUSES (rc=2): mode-4 compile failed for $w"; exit 2; }
    gcc -m64 -no-pie "$d/$w.s" -o "$d/$w.m4" -L./out -lscrip_rt -Wl,-rpath,"$ROOT/out" -lm -lpthread 2>/dev/null \
        || { echo "⛔ GATE REFUSES (rc=2): mode-4 link failed for $w"; exit 2; }
    o=$(timeout 10 "$d/$w.m4" </dev/null 2>/dev/null); r=$?; graded=$((graded+1))
    if [ "$o" != "before" ] || [ "$r" != 1 ]; then
        echo "  ⛔ RED m4 $w: rc=$r stdout=[$(printf '%s' "$o" | tr '\n' '|')] -- want rc=1 stdout=[before]"; bad=1
    fi
done
[ "$graded" = 4 ] || { echo "⛔ GATE REFUSES (rc=2): graded $graded of the 4 required runs"; exit 2; }
[ "$bad" = 0 ] || { echo "⛔ GATE RED [icn_pow_error_cases]: Icon '^' does not raise on the oracle's error cases"; exit 1; }
echo "✅ GATE OK [icn_pow_error_cases]: 0^(-1) and (-2.0)^0.5 both raise (rc=1, stdout=before) in BOTH modes"
