#!/bin/bash
"$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/util_require_fresh.sh" --gate "$(basename "${BASH_SOURCE[0]}" .sh)" || exit $?
# ⭐ WITNESS GATE for row snobol4-executable-pool-exhaustion-is-silent-and-fragments-are-never-freed (cfo 2026-09-12,
# on Lon's word: "check that EVAL and CODE items get freed at the appropriate times").
# MEASURED 2026-09-12: on a 64 MB executable pool a loop of DISTINCT EVALs failed at 15,359 with &ERRTYPE 0 and an
# empty &ERRTEXT -- bb_alloc returned NULL and nothing said so.  A CODE-replaced labeled block leaks 13.1 KB and a
# retained EVAL 5.5 KB, forever.  This gate pins the FIRST half: exhaustion must be a NUMBERED error under the one
# error voice, never a silent failure.  It sizes the pool small through SCRIP_BB_POOL_MB and REFUSES rc=2 until the
# runtime honours that variable (the cure lands it), so it can never pass vacuously on a pool too large to fill.
set -u
HERE="$(cd "$(dirname "$0")" && pwd)"; ROOT="${S4E_HOME:-$(cd "$HERE/.." && pwd)}"; [ -x "$ROOT/scrip" ] || ROOT="$(cd "$HERE/../.." && pwd)/SCRIP"
SCRIP_BIN="${SCRIP_BIN:-$ROOT/scrip}"; [ -x "$SCRIP_BIN" ] || { echo "⛔ REFUSE(2): no scrip binary"; exit 2; }
grep -q 'SCRIP_BB_POOL_MB' "$ROOT/src/ir/bb_pool.c" 2>/dev/null || { echo "⛔ REFUSE(2): the executable pool cannot be sized from the environment yet (no SCRIP_BB_POOL_MB in src/ir/bb_pool.c) -- this gate cannot fill it and refuses rather than pass on a pool it never exhausted"; exit 2; }
T=$(mktemp -d) || exit 2; trap 'rm -rf "$T"' EXIT
printf '\tI = 0\n\t&ERRLIMIT = 100000\nL\tX = EVAL(I '"'"' + 2.0'"'"')\t:F(BAD)\n\tI = I + 1\n\tLT(I, 40000)\t:S(L)\n\tOUTPUT = '"'"'all '"'"' I '"'"' ok'"'"'\t:(END)\nBAD\tOUTPUT = '"'"'failed at '"'"' I '"'"' errtype='"'"' &ERRTYPE\nEND\n' > "$T/ev.sno"
out="$(cd "$T" && SCRIP_BB_POOL_MB=16 timeout 120 "$SCRIP_BIN" ev.sno </dev/null 2>&1)"
printf '%s\n' "$out" | tail -2
if printf '%s\n' "$out" | grep -q 'errtype=0$'; then echo "⛔ RED: the pool ran out and the program saw errtype 0 with no text -- exhaustion is SILENT"; exit 1; fi
if printf '%s\n' "$out" | grep -qE 'errtype=[1-9][0-9]*$|scrip: error [0-9]+'; then echo "GATE OK: exhausting a 16 MB executable pool is a numbered error"; exit 0; fi
if printf '%s\n' "$out" | grep -q 'all 40000 ok'; then echo "⛔ REFUSE(2): 40,000 distinct EVALs did not fill a 16 MB pool -- either fragments are now released (then lower the loop's expectation and pin THAT) or the variable was ignored"; exit 2; fi
echo "⛔ RED: unexpected shape"; exit 1
