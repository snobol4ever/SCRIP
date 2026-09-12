#!/usr/bin/env bash
# test_gate_gc_pacing_bounds_a_churning_program.sh -- storage regeneration is PACED, not exhaustion-only (GC-7, ceo CEO-639).
#
# MEASURED 2026-09-12 on f8421f4bd: `every 1 to 3000000 do image("ab\tcd")` reached 564 MB RSS in both modes -- the first
# regeneration fired only when the 512 MB arena was exhausted (the seam-side line sat at half the arena and the assembly
# fast path carved inline against the arena END, never seeing any line), so a program with a few KB of live data touched
# every page of the arena and CEO-605's micro.icn aborted on the pinned-allocation ceiling. Cure: the collection line sits
# min(128 MB, half of what remains) past the top (SCRIP_GC_LINE_MB, default 128 -- 64 cost ~60% wall on the churniest Icon
# benchmark, 128 reads at parity with the old policy; 0 = the old half-of-arena seam policy), the
# fast region carries that line at offset 48 and rtx_alloc.s carves inline only up to it, and c_rt_gcheap_alloc collects
# at the line through the same conservative path exhaustion already used. Arms: (1) the churning witness stays under
# 256 MB RSS (the row's own bound) in mode 3 and mode 4 and prints ok; (2) CONTROL: SCRIP_GC_LINE_MB=0 reproduces the old behaviour, RSS above
# 400 MB -- the control arm proves the pacing is what bounds the RSS and that the seam still selects the old policy.
# FAIL_ONCE=1 plants the control arm's RSS into arm 1 to prove the bound trips.
"$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/util_require_fresh.sh" --gate "$(basename "${BASH_SOURCE[0]}" .sh)" || exit $?
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP_BIN:-$ROOT/scrip}"; [ -x "$SCRIP" ] || { echo "⛔ REFUSE(2): no scrip at $SCRIP"; exit 2; }
command -v /usr/bin/time >/dev/null || { echo "⛔ REFUSE(2): /usr/bin/time is the RSS instrument and it is absent"; exit 2; }
T=$(mktemp -d) || exit 2; trap 'rm -rf "$T"' EXIT
printf 'procedure main()\n   every 1 to 3000000 do image("ab\\tcd");\n   write("ok");\nend\n' > "$T/churn.icn"
rss_of() { grep -o 'RSS=[0-9]*' "$1" | tail -1 | cut -d= -f2; }
RC=0; examined=0
( cd "$T" && /usr/bin/time -f 'RSS=%M' timeout 120 "$SCRIP" churn.icn ) >"$T/m3.out" 2>"$T/m3.err"; r3=$(rss_of "$T/m3.err")
( cd "$T" && timeout 30 "$SCRIP" --compile -o churn.s churn.icn </dev/null && gcc churn.s -o churn.bin -L"$ROOT/out" -lscrip_rt -lm -lpthread ) >/dev/null 2>&1 || { echo "⛔ REFUSE(2): mode-4 witness did not build"; exit 2; }
( cd "$T" && LD_LIBRARY_PATH="$ROOT/out" /usr/bin/time -f 'RSS=%M' timeout 120 ./churn.bin ) >"$T/m4.out" 2>"$T/m4.err"; r4=$(rss_of "$T/m4.err")
( cd "$T" && SCRIP_GC_LINE_MB=0 /usr/bin/time -f 'RSS=%M' timeout 120 "$SCRIP" churn.icn ) >"$T/c.out" 2>"$T/c.err"; rc_=$(rss_of "$T/c.err")
[ -n "${FAIL_ONCE:-}" ] && r3="$rc_"
for M in m3 m4; do
  examined=$((examined+1)); v=$([ "$M" = m3 ] && echo "$r3" || echo "$r4"); out=$(head -1 "$T/$M.out")
  if [ "$out" = ok ] && [ -n "$v" ] && [ "$v" -lt 262144 ]; then echo "  $M PASS (ok, RSS ${v} KB under 256 MB with 3M dead image results)"
  else echo "  $M FAIL (out=[$out] RSS=[${v:-?}] KB, bound 262144)"; RC=1; fi
done
examined=$((examined+1))
if [ "$(head -1 "$T/c.out")" = ok ] && [ -n "$rc_" ] && [ "$rc_" -gt 409600 ]; then echo "  control PASS (SCRIP_GC_LINE_MB=0 reproduces the exhaustion-only policy: RSS ${rc_} KB)"
else echo "  control FAIL (SCRIP_GC_LINE_MB=0 read RSS=[${rc_:-?}] KB, expected above 400 MB -- the seam no longer selects the old policy, so the paced arms prove nothing)"; RC=1; fi
if [ "$RC" = 0 ]; then echo "GATE PASS [$(basename "${BASH_SOURCE[0]}" .sh)]: regeneration is paced at the 128 MB line in both modes and the seam still selects the old policy (examined $examined arms)"
else echo "GATE FAIL(1) [$(basename "${BASH_SOURCE[0]}" .sh)]: pacing does not bound a churning program, or the control arm no longer discriminates (examined $examined arms)"; fi
echo "    tree: SCRIP=$(git -C "$ROOT" rev-parse --short HEAD 2>/dev/null)$(git -C "$ROOT" diff --quiet 2>/dev/null || echo -DIRTY)  measured $(date -u +%Y-%m-%dT%H:%MZ)"
exit $RC
