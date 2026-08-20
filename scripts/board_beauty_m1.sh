#!/usr/bin/env bash
# board_beauty_m1.sh — ⭐ THE M1 PROGRESS BOARD (s183, HQ Fable).
#
#   bash scripts/board_beauty_m1.sh [--modes m3|m4|both] [--rungs "1 2 5 ..."] [--bisect]
#
# ⛔ WHY THIS EXISTS.  MILESTONE 1 was a BINARY fact — beauty either self-hosts or it does not — and a binary
# fact cannot be delegated: there is exactly one rung and one seat can hold it.  This board makes M1 a LADDER
# by feeding beauty INCREASING PREFIXES OF ITS OWN SOURCE, so "how much of itself can beauty handle" becomes a
# number that moves.  Every red rung it names is an independently dispatchable row, which is what lets the
# fleet work M1 in parallel instead of queueing behind one seat.
#
# THE ORACLE IS THE JUDGE, NOT THE INPUT FILE.  For a PREFIX of beauty.sno the beautified form is NOT
# necessarily the prefix itself (a prefix is usually not a well-formed program), so each rung compares SCRIP
# against the LIVE ORACLE on the same input.  Only the FULL-FILE rung is additionally checked for the M1 FIXED
# POINT (output byte-identical to the INPUT), which is the actual milestone.  ⛔ NO md5 IS EVER PINNED
# (GOAL-SNOBOL4-100 DOD item 2) — the oracle is re-run every time.
#
# ⛔ beauty.sno needs `sbl -bf`; plain `-b` SIGSEGVs after 34 lines (CLAUDE.md).  m4 links out/libscrip_rt.so.
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="$S4E/SCRIP/scrip"
SBL="$S4E/x64/bin/sbl"
BDIR="$S4E/corpus/programs/snobol4/demo/beauty"
SRC="$BDIR/beauty.sno"
MODES=both; RUNGS=""; BISECT=0
while [ $# -gt 0 ]; do case "$1" in
    --modes) MODES="$2"; shift 2;; --rungs) RUNGS="$2"; shift 2;; --bisect) BISECT=1; shift;;
    *) echo "usage: board_beauty_m1.sh [--modes m3|m4|both] [--rungs \"1 2 5\"] [--bisect]"; exit 2;; esac; done
[ -x "$SCRIP" ] || { echo "no scrip binary at $SCRIP — run make"; exit 2; }
[ -x "$SBL" ]   || { echo "⛔ ORACLE ABSENT ($SBL). Every verdict below would be a plausible FALSE all-FAIL table (CLAUDE.md). Clone x64 first."; exit 2; }
TOT=$(wc -l < "$SRC")
[ -n "$RUNGS" ] || RUNGS="1 2 5 10 20 40 80 160 320 $TOT"
WORK="$(mktemp -d)"; trap 'rm -rf "$WORK"' EXIT
# ---------------------------------------------------------------- one rung: N leading lines of beauty.sno as the input
rung() {
    local n="$1" mode="$2" tag rc out exp
    head -n "$n" "$SRC" > "$WORK/in.$n"
    timeout 60 "$SBL" -bf "$SRC" < "$WORK/in.$n" > "$WORK/oracle.$n" 2>/dev/null; local orc=$?
    if [ "$mode" = m3 ]; then
        timeout 60 "$SCRIP" "$SRC" < "$WORK/in.$n" > "$WORK/got.$n" 2>/dev/null; rc=$?
    else
        ( cd "$BDIR" && timeout 120 "$SCRIP" --compile -o "$WORK/b.s" "$SRC" ) >/dev/null 2>&1 || { echo "COMPILE-FAIL"; return; }
        gcc -no-pie "$WORK/b.s" -L"$S4E/SCRIP/out" -lscrip_rt -Wl,-rpath,"$S4E/SCRIP/out" -lm -o "$WORK/b.bin" >/dev/null 2>&1 \
            || { echo "LINK-FAIL"; return; }
        timeout 60 "$WORK/b.bin" < "$WORK/in.$n" > "$WORK/got.$n" 2>/dev/null; rc=$?
    fi
    if   [ "$orc" -ne 0 ]; then tag="ORACLE-rc$orc"           # the oracle itself refused: NOT a SCRIP verdict
    elif [ "$rc" -eq 139 ]; then tag="SEGV"
    elif [ "$rc" -eq 124 ]; then tag="HANG"
    elif [ "$rc" -ne 0 ];   then tag="rc$rc"
    elif cmp -s "$WORK/got.$n" "$WORK/oracle.$n"; then tag="PASS"
    else tag="DIFF"; fi
    # THE MILESTONE ITSELF: the full-file rung must ALSO be the identity (beauty.sno is already beautiful).
    if [ "$n" = "$TOT" ] && [ "$tag" = PASS ]; then
        cmp -s "$WORK/got.$n" "$WORK/in.$n" && tag="⭐M1-FIXED-POINT" || tag="PASS-but-NOT-fixed-point"
    fi
    echo "$tag"
}
echo "=== M1 PROGRESS BOARD — beauty.sno fed INCREASING PREFIXES OF ITSELF ($TOT lines total) ==="
echo "    judge = live oracle (sbl -bf) on the same input; full-file rung also checked for the FIXED POINT"
printf "%8s  %-24s %-24s\n" "lines" "m3 (--run)" "m4 (--compile)"
first_red_m3=""; first_red_m4=""; pass3=0; pass4=0; tried=0
for n in $RUNGS; do
    [ "$n" -gt "$TOT" ] && continue
    tried=$((tried+1)); r3="-"; r4="-"
    case "$MODES" in both|m3) r3="$(rung "$n" m3)";; esac
    case "$MODES" in both|m4) r4="$(rung "$n" m4)";; esac
    case "$r3" in PASS|⭐*) pass3=$((pass3+1));; -) ;; ORACLE-*) ;; *) [ -z "$first_red_m3" ] && first_red_m3="$n";; esac
    case "$r4" in PASS|⭐*) pass4=$((pass4+1));; -) ;; ORACLE-*) ;; *) [ -z "$first_red_m4" ] && first_red_m4="$n";; esac
    printf "%8s  %-24s %-24s\n" "$n" "$r3" "$r4"
done
echo "------------------------------------------------------------"
echo "M3 rungs green: $pass3/$tried   first red at: ${first_red_m3:-none}"
echo "M4 rungs green: $pass4/$tried   first red at: ${first_red_m4:-none}"
# ---------------------------------------------------------------- optional: bisect to the EXACT first failing line
if [ "$BISECT" = 1 ] && [ -n "$first_red_m3" ]; then
    lo=1; hi="$first_red_m3"
    for r in $RUNGS; do [ "$r" -lt "$hi" ] && lo="$r"; done
    echo "--- bisecting m3 between $lo (green) and $hi (red) for the EXACT first failing line ---"
    while [ $((hi-lo)) -gt 1 ]; do
        mid=$(( (lo+hi)/2 )); v="$(rung "$mid" m3)"
        echo "    $mid -> $v"
        case "$v" in PASS|⭐*) lo=$mid;; *) hi=$mid;; esac
    done
    echo "⭐ M3 FIRST FAILING LINE = $hi"
    echo "    the line itself:"; sed -n "${hi}p" "$SRC" | head -c 200; echo
fi
echo "⛔ A red rung is never denied (law 0d).  Each distinct red CLASS above is an independently dispatchable row."
