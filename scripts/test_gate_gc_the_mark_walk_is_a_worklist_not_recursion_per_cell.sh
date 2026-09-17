#!/usr/bin/env bash
# test_gate_gc_the_mark_walk_is_a_worklist_not_recursion_per_cell.sh -- A COLLECTOR MUST WALK A LONG DATA CHAIN ON A
# BOUNDED C STACK (row gc-descriptor-walk-recurses-once-per-list-cell-and-overflows-the-c-stack-on-a-long-data-chain;
# ceo ruling CEO-822, which put this row AHEAD of the F6 step-3 reporter because a divergence census that cannot run at
# all on the class that stresses the collector hardest would hand back a zero meaning NEVER MEASURED, and DARK is worse
# than RED).
#
# THE DEFECT, MEASURED ON ORIGIN b12714737: rt_gc_visit_descr recursed once per DATA instance along a CDR chain -- one C
# frame per cell -- so a collection with a 600000-cell CONS list live died with ERROR 246 (stack overflow) and
# COLLECTIONS=0 in BOTH modes, while a 60000-cell control printed 60000 at rc=0. The process died inside the mark walk
# before one collection completed. Cure: the aggregate arms push onto an explicit heap worklist (gc_wl_push) which
# rt_gc_visit_descr drains, so the C depth is bounded (gc_visit_one -> gc_visit_vcell -> gc_visit_tbblk -> push) no
# matter how long the chain is. The row's GOAL said to measure what SCRIP_GC_WORKLIST already covers before adding a
# switch: IT COVERS NOTHING -- that name is in no file under src/, and git log -S finds it only in f2270e807, the
# landing that DELETED all eight env-gated alternate collector paths. There is deliberately no new switch here either;
# the collector has one behaviour.
#
# ⭐ ARM 3 IS THE ONE THAT CANNOT BE FAKED. A cure that simply stopped walking the chain would pass arms 1 and 2 (rc=0,
# right answer) while silently collecting live data. Arm 3 reads wl_depth_max out of the [ZGC] telemetry and requires it
# to SCALE WITH THE CHAIN: a 1.5M-cell chain must put more than a million entries on the heap worklist. That number is
# the walk itself, so the arm is vacuous only if the collector is doing the work.
#
# FAIL-ONCE IS MEASURED, NOT ASSERTED: on origin b12714737 (this cure stashed, runtime rebuilt, this gate left in place)
# arms 1, 2 and 3 all read red -- rc=1, ERROR 246, empty output, collections=0.
"$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/util_require_fresh.sh" --gate "$(basename "${BASH_SOURCE[0]}" .sh)" || exit $?
set -uo pipefail
G="$(basename "${BASH_SOURCE[0]}" .sh)"
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP_BIN:-$ROOT/scrip}"; [ -x "$SCRIP" ] || { echo "⛔ GATE REFUSE(2) [$G]: no scrip at $SCRIP"; exit 2; }
T=$(mktemp -d) || exit 2; trap 'rm -rf "$T"' EXIT
RC=0; examined=0
chain() { printf " DATA('CONS(CAR,CDR)')\n L = \n I = 0\nLP I = LT(I,%s) I + 1 :F(DN)\n L = CONS(I, L) :(LP)\nDN X = COLLECT()\n J = 0\nCT J = LT(J,3000) J + 1 :F(OUT)\n S = DUPL('x', 20000) :(CT)\nOUT K = 0\nCN L = DIFFER(L) CDR(L) :F(FIN)\n K = K + 1 :(CN)\nFIN OUTPUT = K\nEND\n" "$1" > "$2"; }
chain 600000 "$T/w.sno"; chain 1500000 "$T/big.sno"
examined=$((examined+1))
SCRIP_ZETA_TELEM=1 timeout 180 "$SCRIP" "$T/w.sno" </dev/null >"$T/o3" 2>"$T/e3"; r3=$?; c3=$(grep -c regeneration "$T/e3")
if [ "$r3" = 0 ] && [ "$(cat "$T/o3")" = 600000 ] && [ "$c3" -ge 1 ]; then echo "  m3 PASS (600000-cell CONS chain live across a COLLECT(): rc=0, walked $(cat "$T/o3") cells back, $c3 collection(s))"
else echo "  m3 FAIL (rc=$r3 out=[$(cat "$T/o3")] collections=$c3 -- $(head -1 "$T/e3" | cut -c1-90))"; RC=1; fi
examined=$((examined+1))
( cd "$T" && timeout 180 "$SCRIP" --compile -o w.s w.sno </dev/null >/dev/null 2>&1 && gcc -no-pie w.s -o w.x -L"$ROOT/out" -lscrip_rt -Wl,-rpath,"$ROOT/out" -lm -lpthread >/dev/null 2>&1 ) || { echo "⛔ GATE REFUSE(2) [$G]: the m4 witness did not build"; exit 2; }
SCRIP_ZETA_TELEM=1 timeout 180 "$T/w.x" </dev/null >"$T/o4" 2>"$T/e4"; r4=$?; c4=$(grep -c regeneration "$T/e4")
if [ "$r4" = 0 ] && [ "$(cat "$T/o4")" = 600000 ] && [ "$c4" -ge 1 ]; then echo "  m4 PASS (same chain in mode 4: rc=0, walked $(cat "$T/o4") cells back, $c4 collection(s))"
else echo "  m4 FAIL (rc=$r4 out=[$(cat "$T/o4")] collections=$c4 -- $(head -1 "$T/e4" | cut -c1-90))"; RC=1; fi
examined=$((examined+1))
SCRIP_ZETA_TELEM=1 timeout 300 "$SCRIP" "$T/big.sno" </dev/null >"$T/ob" 2>"$T/eb"; rb=$?; cb=$(grep -c regeneration "$T/eb")
d=$(grep -oE 'wl_depth_max=[0-9]+' "$T/eb" | cut -d= -f2 | sort -n | tail -1)
if [ "$rb" = 0 ] && [ "$(cat "$T/ob")" = 1500000 ] && [ "$cb" -ge 1 ] && [ -n "$d" ] && [ "$d" -gt 1000000 ]; then echo "  scaling PASS (1500000-cell chain: rc=0, $cb collection(s), wl_depth_max=$d entries on the HEAP worklist -- the walk is not on the C stack, and it really walked)"
else echo "  scaling FAIL (rc=$rb out=[$(cat "$T/ob")] collections=$cb wl_depth_max=[${d:-absent}] -- either the chain did not survive, or the depth instrument is gone, or the collector stopped walking the chain and only looks cured)"; RC=1; fi
if [ "$RC" = 0 ]; then echo "GATE PASS(0) [$G]: the mark walk is an explicit worklist; a 1.5M-cell DATA chain is collected and walked on a bounded C stack in both modes (examined $examined arms)"
else echo "GATE FAIL(1) [$G]: the descriptor walk recurses per cell again, or the chain no longer survives a collection (examined $examined arms)"; fi
echo "    tree: SCRIP=$(git -C "$ROOT" rev-parse --short HEAD 2>/dev/null)$(git -C "$ROOT" diff --quiet 2>/dev/null || echo -DIRTY)  measured $(date -u +%Y-%m-%dT%H:%MZ)"
exit $RC
