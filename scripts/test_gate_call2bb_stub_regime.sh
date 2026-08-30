#!/usr/bin/env bash
# test_gate_call2bb_stub_regime.sh — CALL2BB slice 3a executable witness (s21x-h, SCRIP 126645fc).
# Proves: under SCRIP_STMT_FRAME=1 + SCRIP_CALL2BB=1 a DEFINE stub blob rides the statement regime
# (proc_DOUBLE_α carves exactly 48 = 32B wire header + slot0, no whole-graph carve), the default path
# keeps the legacy floored carve, and BOTH gate states are output-exact on the two seed witnesses
# (armed body blob + honestly-declining recursive FIB).  Exits nonzero on any drift.
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; SCRIP="${SCRIP:-$HERE/../scrip}"; OUT="$HERE/../out"
# ⛔ test_sno_call2bb_1/2 moved into suite format 2026-08-28 (probe-consolidate-bb, LON-20260828 total
# conversion): library/probe_reference/bb no longer holds loose .sno files, so the two witnesses this gate hardcodes by
# exact path are materialized here via the harness's `extract` (ONE AUTHORITY for the suite grammar,
# same as every other suite consumer) into a scratch dir, then used exactly as the old standalone files were.
. "$(dirname "${BASH_SOURCE[0]}")/lib_master_extract.sh"   # re-pointed to THE MASTER by origin (zero-subfolders cutover, ceo s283h)
if [ ! -x "$SCRIP" ]; then echo "SKIP scrip not built"; exit 0; fi
W=$(mktemp -d); trap 'rm -rf "$W"' EXIT; fail=0
S1="$W/test_sno_call2bb_1.sno"; S2="$W/test_sno_call2bb_2.sno"
master_extract_origin probe_bb__test_sno_call2bb_1 "$S1" || { echo "SETUP: extract test_sno_call2bb_1 failed"; exit 2; }
master_extract_origin probe_bb__test_sno_call2bb_2 "$S2" || { echo "SETUP: extract test_sno_call2bb_2 failed"; exit 2; }
ck() { [ "$2" = "$3" ] && echo "PASS $1" || { echo "FAIL $1: got [$2] want [$3]"; fail=1; }; }
ck "m3 default  dbl" "$(timeout 8 "$SCRIP" --run "$S1" </dev/null 2>&1 | tr '\n' ' ')" "10 42 "
ck "m3 gated    dbl" "$(SCRIP_STMT_FRAME=1 SCRIP_CALL2BB=1 timeout 8 "$SCRIP" --run "$S1" </dev/null 2>&1 | tr '\n' ' ')" "10 42 "
ck "m3 gated    fib" "$(SCRIP_STMT_FRAME=1 SCRIP_CALL2BB=1 timeout 8 "$SCRIP" --run "$S2" </dev/null 2>&1 | tr '\n' ' ')" "55 1 "
SCRIP_STMT_FRAME=1 SCRIP_CALL2BB=1 timeout 15 "$SCRIP" --compile "$S1" > "$W/g.s" 2>/dev/null
kt_g=$(awk '/^proc_DOUBLE_α:/{f=1} f&&/sub +rsp/{print $3; exit}' "$W/g.s")
ck "m4 gated stub kt" "$kt_g" "48"
timeout 15 "$SCRIP" --compile "$S1" > "$W/d.s" 2>/dev/null
kt_d=$(awk '/^proc_DOUBLE_α:/{f=1} f&&/sub +rsp/{print $3; exit}' "$W/d.s")
[ "$kt_d" != "48" ] && [ -n "$kt_d" ] && echo "PASS m4 default stub kt=$kt_d (legacy, != 48)" || { echo "FAIL m4 default stub kt=[$kt_d] must be legacy != 48"; fail=1; }
if gcc -no-pie "$W/g.s" -L"$OUT" -lscrip_rt -Wl,-rpath,"$OUT" -o "$W/g" 2>/dev/null; then
  ck "m4 gated    dbl" "$(timeout 8 "$W/g" </dev/null 2>&1 | tr '\n' ' ')" "10 42 "
else echo "FAIL m4 gated link"; fail=1; fi
[ $fail -eq 0 ] && echo "GATE GREEN: call2bb stub regime" || echo "GATE RED"
exit $fail
