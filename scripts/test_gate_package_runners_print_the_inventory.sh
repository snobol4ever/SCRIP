#!/usr/bin/env bash
# test_gate_package_runners_print_the_inventory.sh — the invariant gate for THE PACKAGE LOCKDOWN's
# instrument (row every-package-runner-prints-shipped-graded-ungraded-and-ungradable-and-the-leaderboard-
# carries-the-inventory, hq_T 2026-09-06, rank 0 on the ceo's order; Lon: "Fix the never graded business.
# Let's lock down our package testing and make it complete.").
#
# WHAT IT PROTECTS. Every lockdown row's DONE-WHEN is `ungraded=0`, so the number that criterion reads is
# load-bearing for the whole fleet. ⛔ THE FAILURE IT GUARDS AGAINST IS NOT A WRONG COUNT — it is a
# program that lands in NO bucket at all and therefore never appears in any number. Measured on the first
# run against a real package: icon/ipl ships 852 .icn files, 211 are declared in UNGRADED.tsv, and 641
# were in no bucket whatsoever. Not red, not skipped, not owed — simply absent, which is exactly the
# "never graded business" the order exists to end.
#
# ⭐⭐ SO THE LOAD-BEARING PROPERTY IS THE SUM, NOT ANY BUCKET. graded + ungraded + ungradable must equal
# shipped. Without that, the line is four independent opinions, each individually plausible, and the
# absent 641 are invisible in all four. A gate that only checked "does the runner print a line" would go
# green on an inventory that omits three quarters of the package.
#
# ⭐ AND THE SUM IS WHY `gap=` HAD TO SPLIT. Four runners printed `shipped/graded/gap`; `gap` held both
# "not graded YET" (work owed) and "CANNOT be graded" (a ruling). While an ungradable program sits inside
# gap, `ungraded=0` is UNREACHABLE BY BEING CORRECT — a lane does every piece of real work available and
# still never reaches its own criterion.
#
# Usage: bash scripts/test_gate_package_runners_print_the_inventory.sh
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
cd "$ROOT"
. "$HERE/lib_gate.sh"
gate_parse_args "$@"
gate_require "$HERE/lib_inventory.sh" "the one package-inventory body lib_inventory.sh"
. "$HERE/lib_inventory.sh"

violations=0; examined=0
TD="$(mktemp -d "${TMPDIR:-/tmp}/pkg_inv.XXXXXX")"; trap 'rm -rf "$TD"' EXIT

mkpkg() {  # mkpkg <n-files>; fresh scratch package, no sidecars
    rm -rf "$TD/p"; mkdir -p "$TD/p"
    local i; for i in $(seq 1 "$1"); do echo 'procedure main(); end' > "$TD/p/f$i.icn"; done
    INV_PACKAGE=scratch; INV_DIR="$TD/p"; INV_EXT=".icn"
}
run_inv() { inventory_line "$1" "$2" 2>&1; }

# ARM 1 — the happy path: buckets that sum produce exactly one line, in the one shape, rc=0.
mkpkg 5
printf 'f4.icn\tEMPTY\tstdin-driven filter, no .in authored yet\n' > "$TD/p/UNGRADED.tsv"
printf 'f5.icn\tNO-ORACLE\tthe oracle refuses this program: unsupported extension\n' > "$TD/p/UNGRADABLE.tsv"
examined=$((examined+1))
out="$(run_inv 3 0)"; rc=$?
want='PACKAGE_INVENTORY package=scratch shipped=5 graded=3 ungraded=1 ungradable=1 graded_stream=3 graded_narrow=0'
if [ "$rc" -ne 0 ] || [ "$out" != "$want" ]; then
    echo "GATE FAIL: the summing case did not print the one shape (rc=$rc)"; echo "    want: $want"; echo "    got : $out"
    violations=$((violations+1))
fi

# ARM 2 — ⭐⭐ THE LOAD-BEARING ARM. A program in NO bucket must REFUSE, and the refusal must name the
# delta. This is the 641-of-852 case, and a gate without it would pass an inventory that loses a package.
examined=$((examined+1))
out="$(run_inv 2 0)"; rc=$?
if [ "$rc" -ne 2 ] || ! printf '%s' "$out" | grep -q 'delta 1'; then
    echo "GATE FAIL: a shipped program in NO bucket was not refused with its delta named (rc=$rc): $out"
    violations=$((violations+1))
fi

# ARM 3 — over-counting refuses too: the sum is an identity, not a floor.
examined=$((examined+1))
out="$(run_inv 4 0)"; rc=$?
[ "$rc" -eq 2 ] || { echo "GATE FAIL: graded overshooting shipped was not refused (rc=$rc): $out"; violations=$((violations+1)); }

# ARM 4 — a declaration naming nothing shipped is STALE and refuses.
mkpkg 3; printf 'ghost.icn\tEMPTY\tnames no shipped file\n' > "$TD/p/UNGRADED.tsv"
examined=$((examined+1))
out="$(run_inv 2 0)"; rc=$?
if [ "$rc" -ne 2 ] || ! printf '%s' "$out" | grep -q 'declared but not shipped'; then
    echo "GATE FAIL: a stale declaration was not refused (rc=$rc): $out"; violations=$((violations+1))
fi

# ARM 5 — a program declared BOTH owed and impossible refuses: contradictory claims about one program.
mkpkg 3
printf 'f1.icn\tEMPTY\towed\n' > "$TD/p/UNGRADED.tsv"; printf 'f1.icn\tNO-ORACLE\timpossible\n' > "$TD/p/UNGRADABLE.tsv"
examined=$((examined+1))
out="$(run_inv 2 0)"; rc=$?
if [ "$rc" -ne 2 ] || ! printf '%s' "$out" | grep -q 'BOTH'; then
    echo "GATE FAIL: a name in both sidecars was not refused (rc=$rc): $out"; violations=$((violations+1))
fi

# ARM 6 — a declaration with NO REASON refuses. An inventory whose rows carry no reason is a number
# nobody can act on: the same failure one size down.
mkpkg 3; printf 'f1.icn\tEMPTY\t\n' > "$TD/p/UNGRADED.tsv"; rm -f "$TD/p/UNGRADABLE.tsv"
examined=$((examined+1))
out="$(run_inv 2 0)"; rc=$?
[ "$rc" -eq 2 ] || { echo "GATE FAIL: an unexplained declaration was not refused (rc=$rc): $out"; violations=$((violations+1)); }

# ARM 7 — ⛔ AN UNEXPLAINED NARROW BUCKET IS JUST A SMALLER LIE (hq_P). Claiming graded_narrow with no
# NARROW.tsv, or with a NARROW.tsv of the wrong size, must refuse: the count and the explanation are one
# population or the bucket is unfalsifiable.
mkpkg 3; rm -f "$TD/p"/*.tsv
examined=$((examined+1))
out="$(run_inv 2 1)"; rc=$?
if [ "$rc" -ne 2 ] || ! printf '%s' "$out" | grep -q 'NARROW.tsv'; then
    echo "GATE FAIL: graded_narrow claimed with no NARROW.tsv was not refused (rc=$rc): $out"; violations=$((violations+1))
fi
examined=$((examined+1))
printf 'f1.icn\tERRNO\terror compared by number only\nf2.icn\tERRNO\tsame\n' > "$TD/p/NARROW.tsv"
out="$(run_inv 2 1)"; rc=$?
if [ "$rc" -ne 2 ] || ! printf '%s' "$out" | grep -q 'same population'; then
    echo "GATE FAIL: graded_narrow disagreeing with NARROW.tsv's size was not refused (rc=$rc): $out"; violations=$((violations+1))
fi
examined=$((examined+1))
printf 'f1.icn\tERRNO\terror compared by number only\n' > "$TD/p/NARROW.tsv"
out="$(run_inv 2 1)"; rc=$?
if [ "$rc" -ne 0 ] || ! printf '%s' "$out" | grep -q 'graded_narrow=1'; then
    echo "GATE FAIL: a correctly explained narrow bucket did not pass (rc=$rc): $out"; violations=$((violations+1))
fi

# ARM 8 — an empty shipped population REFUSES rather than printing a clean zero. A percent over zero is
# not a score, and "0 ungraded" over 0 shipped would satisfy every lockdown criterion instantly.
rm -rf "$TD/p"; mkdir -p "$TD/p"; INV_DIR="$TD/p"
examined=$((examined+1))
out="$(run_inv 0 0)"; rc=$?
[ "$rc" -eq 2 ] || { echo "GATE FAIL: an empty package was not refused (rc=$rc): $out"; violations=$((violations+1)); }

# ARM 9 — a moved package REFUSES rather than reinterpreting.
INV_DIR="$TD/does-not-exist"
examined=$((examined+1))
out="$(run_inv 0 0)"; rc=$?
[ "$rc" -eq 2 ] || { echo "GATE FAIL: a missing package dir was not refused (rc=$rc): $out"; violations=$((violations+1)); }

# ARM 10 — THE CENSUS, WITH ITS DENOMINATOR PRINTED. Every package runner must reach the shared body.
# ⚠ REPORTED, NOT COUNTED, WHILE THE ROW RAMPS: the instrument landed before its callers, so naming them
# is the work list, not a verdict. ⭐ It prints the denominator rather than a boolean for the reason the
# stale-binary gate's census arm does: that arm caught THIS author shipping a gate with no freshness
# guard on the day it landed (gates=97 wired=96 uncovered=1), which a boolean could not have done.
runners=0; wired=0; missing=""
for r in "$HERE"/test_*_suite.sh "$HERE"/raku_roast_scoreboard.sh "$HERE"/board_packages.sh; do
    [ -f "$r" ] || continue
    runners=$((runners+1))
    if grep -q 'lib_inventory.sh' "$r"; then wired=$((wired+1)); else missing="$missing $(basename "$r")"; fi
done
echo "    package runners censused=$runners wired=$wired unwired=$((runners-wired))"
[ -z "$missing" ] || { echo "    NOT YET WIRED (the row's work list, not a verdict):"; printf '      %s\n' $missing; }

GATE_EXAMINED="$examined arms"
gate_verdict "$violations" "package-inventory violations"
