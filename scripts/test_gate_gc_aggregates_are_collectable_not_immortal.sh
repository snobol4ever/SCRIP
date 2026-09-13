#!/usr/bin/env bash
# test_gate_gc_aggregates_are_collectable_not_immortal.sh -- a dead list, table, set or record is RECLAIMED: the collector marks the four HB_WS/HB_WSS/HB_DINST/HB_ARR types from their roots like every other block instead of force-marking them live at every regeneration (ceo CEO-661, GC-5 rung 1, on Lon's "we do not allow pinning in our heap since we slide").
#
# MEASURED 2026-09-13 on 71b32337f: `every i := 1 to 4000000 do L := [i, i+1, i+2]` -- a program holding ONE list --
# died with SIGABRT at 997 MB, because gc_heap.c force-marked every block of those four types live at every collection
# (hb_pinned, the mark loop) since ac044419d. Cure: the force-mark is gone by default; the four types are reached
# through the roots. That exposed a real hole the exemption had been hiding: the name/value table's own entries are
# rt_pinned_alloc'd HB_WS blocks and core_gc_roots walked the buckets WITHOUT marking the entries, their names or their
# chain -- so with the exemption gone the table was collected out from under itself and the next collection read a
# freed entry's `cell` as a pointer (SIGSEGV in rt_gc_visit_descr). core_gc_roots now marks the table it walks, the
# way the Pascal heap table became a movable root.
# ARMS: (1) the 4M-list churn COMPLETES with the right answer and stays under 800 MB; (2) the control arm
# SCRIP_GC_PIN_AGGREGATES=1 restores the old policy and still dies -- if it ever stops dying this gate is measuring
# nothing; (3) correctness under heavy collection: 200000 lists into a table, byte-identical to icont.
# FAIL_ONCE=1 runs arm 1 under the control arm, which must fail it.
"$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/util_require_fresh.sh" --gate "$(basename "${BASH_SOURCE[0]}" .sh)" || exit $?
set -uo pipefail
G="$(basename "${BASH_SOURCE[0]}" .sh)"
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP_BIN:-$ROOT/scrip}"; [ -x "$SCRIP" ] || { echo "⛔ GATE REFUSE(2) [$G]: no scrip at $SCRIP"; exit 2; }
ICONT="${ICONT_BIN:-/home/resources/icon-master/bin/icont}"; [ -x "$ICONT" ] || { echo "⛔ GATE REFUSE(2) [$G]: no icont at $ICONT -- the correctness arm's ref is CUT FROM THE ORACLE"; exit 2; }
T=$(mktemp -d) || exit 2; trap 'rm -rf "$T"' EXIT
RC=0
printf 'procedure main()\n   local i, L;\n   every i := 1 to 4000000 do L := [i, i+1, i+2];\n   write("done ", *L);\nend\n' > "$T/churn.icn"
printf 'procedure main()\n   local i, L, T, R;\n   T := table(0);\n   every i := 1 to 200000 do { L := [i, i+1]; T[i %% 7] := L[1] + L[2] };\n   R := 0; every i := 0 to 6 do R +:= T[i];\n   write("sum ", R, " ", *T);\nend\n' > "$T/corr.icn"
run_rss() { /usr/bin/time -f '%M' -o "$T/rss" timeout 300 env "$@" >"$T/out" 2>"$T/err"; echo $?; }
PIN=""; [ -n "${FAIL_ONCE:-}" ] && PIN="SCRIP_GC_PIN_AGGREGATES=1"
rc=$(run_rss $PIN "$SCRIP" "$T/churn.icn"); rss=$(tail -1 "$T/rss" 2>/dev/null || echo 0); got=$(cat "$T/out")
if [ "$rc" = 0 ] && [ "$got" = "done 3" ] && [ "$rss" -lt 819200 ]; then echo "  churn PASS (4M dead lists: completed, answer \"$got\", RSS ${rss} KB under 800 MB)"
else echo "  churn FAIL (rc=$rc answer=\"$got\" RSS=${rss} KB -- a program holding one list did not survive four million dead ones)"; RC=1; fi
rc2=$(run_rss SCRIP_GC_PIN_AGGREGATES=1 "$SCRIP" "$T/churn.icn"); rss2=$(tail -1 "$T/rss" 2>/dev/null || echo 0)
if [ "$rc2" != 0 ]; then echo "  control PASS (SCRIP_GC_PIN_AGGREGATES=1 restores the immortal policy and still dies: rc=$rc2, RSS ${rss2} KB)"
else echo "  control FAIL (the old policy no longer dies on this witness -- rc=$rc2 RSS=${rss2} KB; this gate is measuring nothing until the witness is made bigger)"; RC=1; fi
( cd "$T" && "$ICONT" -s corr.icn -x ) >"$T/corr.ref" 2>&1
( cd "$T" && timeout 120 "$SCRIP" corr.icn </dev/null ) >"$T/corr.out" 2>&1
if diff -q "$T/corr.ref" "$T/corr.out" >/dev/null; then echo "  correctness PASS (200000 lists through a table under heavy collection, byte-identical to icont: $(cat "$T/corr.ref"))"
else echo "  correctness FAIL (collection changed the answer: got \"$(cat "$T/corr.out")\", icont \"$(cat "$T/corr.ref")\")"; RC=1; fi
if [ "$RC" = 0 ]; then echo "GATE PASS [$G]: dead aggregates are reclaimed, the old immortal policy still discriminates, and collection does not change an answer"
else echo "GATE FAIL(1) [$G]: aggregates are immortal again, or the control arm stopped discriminating (examined 3 arms)"; fi
echo "    tree: SCRIP=$(git -C "$ROOT" rev-parse --short HEAD 2>/dev/null)$(git -C "$ROOT" diff --quiet 2>/dev/null || echo -DIRTY)  measured $(date -u +%Y-%m-%dT%H:%MZ)"
exit $RC
