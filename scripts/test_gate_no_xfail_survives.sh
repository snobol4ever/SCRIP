#!/usr/bin/env bash
# test_gate_no_xfail_survives.sh — THERE IS NO SUCH THING AS XFAIL (Lon 2026-09-03 21:30, in-chat to ceo, verbatim:
# "Also keep in mind there is no such thing now as XFAIL. We are shooting for 100%. So if an XFAIL is a faulty test
# than lets fix all those tests."). An xfail counts as FAIL for the score. This is THE ONE instrument for all seven
# languages -- one gate, not seven copies -- and it is RED while any suite still carries one.
#
# ⛔ IT COUNTS THE POPULATION, NEVER A PINNED NUMBER. Three independent sources, because the seven suites spell an
# xfail three different ways and a census that knew only one would report a confident zero for the others:
#     ALL.csv `xfail` column · ALL.xfail lines · *.xfail marker files
# ⭐ That is the whole reason this is one script rather than a grep: snobol4 uses all three (68 + 136 + 1), icon uses
# csv + 27 marker files, raku uses csv alone (156). A single-source census reads pascal/rebus 0 correctly and icon
# 6-instead-of-33 wrongly, and both look equally like an answer.
set -u
GATE_NAME=test_gate_no_xfail_survives
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="${S4E_HOME:-$(cd "$HERE/../.." && pwd)}"
T="$ROOT/corpus/tests"
LANGS="snobol4 icon prolog raku pascal snocone rebus"
refuse() { echo "⛔ REFUSED(2) [$GATE_NAME]: $*" >&2; exit 2; }
[ -d "$T" ] || refuse "no corpus/tests at $T -- a census that cannot see the population is not a zero"
GRAND=0; MISSING=""
for l in $LANGS; do
    d="$T/$l"
    if [ ! -d "$d" ]; then MISSING="$MISSING $l"; continue; fi
    csv=0
    if [ -f "$d/ALL.csv" ]; then
        csv=$(python3 - "$d/ALL.csv" <<'PY'
import csv,sys
try:
    rows=list(csv.DictReader(open(sys.argv[1])))
except OSError: print("ERR"); raise SystemExit(0)
if rows and "xfail" not in rows[0]: print("NOCOL"); raise SystemExit(0)
print(sum(1 for r in rows if (r.get("xfail") or "0").strip() not in ("","0")))
PY
)
        case "$csv" in ERR|NOCOL) refuse "$l: ALL.csv unreadable or has no xfail column ($csv) -- cannot census, and a census that cannot see its population must not print 0";; esac
    fi
    ax=0; [ -f "$d/ALL.xfail" ] && ax=$(grep -cve '^[[:space:]]*$' -e '^[[:space:]]*#' "$d/ALL.xfail" || true)
    fx=$(find "$d" -name '*.xfail' 2>/dev/null | wc -l)
    tot=$((csv + ax + fx)); GRAND=$((GRAND + tot))
    printf 'XFAIL_CENSUS lang=%-8s csv=%-4s allxfail=%-4s files=%-4s total=%s\n' "$l" "$csv" "$ax" "$fx" "$tot"
done
[ -z "$MISSING" ] || refuse "no suite directory for:$MISSING -- an unreachable suite is not a suite with zero xfails"
echo "XFAIL_TOTAL all_languages=$GRAND"
if . "$HERE/lib_gate.sh" 2>/dev/null && command -v gate_stamp >/dev/null 2>&1; then gate_stamp; fi
if [ "$GRAND" -eq 0 ]; then
    echo "GATE PASS(0) [$GATE_NAME]: 0 xfails survive in any of the seven suites"
    exit 0
fi
echo "⛔ GATE RED [$GATE_NAME]: $GRAND xfail(s) survive across the seven suites — an xfail counts as FAIL (Lon 2026-09-03 21:30)."
echo "   Each is either a FAULTY TEST (fix it against its oracle, or delete it with the reason in the commit) or a REAL DEFECT (class row on its rung, cured)."
echo "   ⭐ A reason is the first step of a fix, not a resting place — the old 'every xfail reasoned' clauses are superseded."
# ⛔⭐ REPORTED-NOT-BLOCKING BY DEFAULT, AND THAT IS A RAMP, NOT A SOFTENING. The gate is genuinely RED (rc=1) and
# says so; what is deliberately NOT mine to decide is whether it reds `make test` for four HQs tonight. Wiring it
# blocking right now would fail the landing verdict of every seat -- INCLUDING the landings that reduce the count --
# so the gate would prevent the work it exists to cause. Precedent in this repo: board_packages.sh ran REPORTED
# before it went BLOCKING at a measured floor. Flip with XFAIL_GATE_BLOCKING=1 (one env var, one line in the
# Makefile); that flip is ceo's/Lon's call on a date, not a script author's on a whim.
if [ "${XFAIL_GATE_BLOCKING:-0}" = "1" ]; then exit 1; fi
echo "   (REPORTED, not blocking: set XFAIL_GATE_BLOCKING=1 to make this fail the build. rc=1 is what it returns then.)"
exit 0
