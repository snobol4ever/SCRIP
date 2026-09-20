#!/usr/bin/env bash
"$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/util_require_fresh.sh" --gate "$(basename "${BASH_SOURCE[0]}" .sh)" || exit $?
# test_gate_pl_master_is_clean_at_the_tiny_arena.sh -- PROLOG'S GC SHARE, HALF TWO (hq_prolog, row
# prolog-gc-the-prolog-share-of-the-unmapped-slot-population-censused-by-name-and-the-master-clean-at-one-megabyte;
# ceo CEO-1011 / TENET condition 1, hardened on Lon's challenge 2026-09-20).
#
# ⛔ WHAT THIS GATE EXISTS FOR, AND WHY NO EXISTING INSTRUMENT SEES IT. The ceo measured hq_snobol4's nine-line
# witness at SCRIP_HEAP_MB=1: four of seven stress points RETURN A WRONG ANSWER, SILENTLY, EXIT 0, NO DIAGNOSTIC.
# Every rc-shaped census reads GREEN over that class -- and so does a pass TOTAL, because a board that loses one
# entry and gains another reads identical (the swap, GOAL-TEST-SUITE-CONSISTENCY's per-entry-identity lesson).
# So this gate grades BY ORACLE DIFF, PER ENTRY, NEVER BY rc AND NEVER BY A TOTAL: it runs the Prolog master
# once per ARM on ONE binary and NAMES every (entry, mode) pair whose outcome MOVED against the control arm.
#
# ⛔⭐ TWO AXES, BECAUSE ONE AXIS READS GREEN OVER THE CLASS -- hq_snobol4 measured this and sent it to me
# 2026-09-20, and it was a hole in this gate's FIRST cut, which varied the arena alone. Their witness is
# byte-equal to its ref at stress 0 and wrong from stress>=1, IN BOTH MODES, AT BOTH ARENAS: the band moves with
# the POLL SET, not the window. Confirmed in the mechanism rather than taken on trust -- gc_heap.c
# c_rt_gcheap_alloc reads SCRIP_GC_STRESS once into a static and raises g_gc_pending every N allocations, so it
# is orthogonal to window size, and UNSET MEANS ZERO FORCED COLLECTIONS. A gate that pins the arena, leaves the
# poll set unset and prints "tiny arena" certifies half of what a reader will take it to mean.
#   ARM control : shipped window, no forced collection  -- the baseline every other arm is diffed against
#   ARM arena   : SCRIP_HEAP_MB=1                       -- does a SMALL WINDOW change an answer
#   ARM stress  : SCRIP_HEAP_MB=1 + SCRIP_GC_STRESS=1   -- does COLLECTING OFTEN change an answer
#
# ⛔ A TIMEOUT IS NOT A DIVERGENCE AND THIS GATE REFUSES RATHER THAN GUESSING. Under stress=1 a correct program
# can run two orders of magnitude slower; an rc=124 cannot distinguish "needs 8.1s" from "never finishes". So
# the stress arm raises the harness per-program TIMEOUT, and any pair whose ONLY movement is into HANG is
# reported as INCONCLUSIVE and REFUSES rc=2 -- never counted as a collector defect, and never quietly dropped
# to keep the gate green. A clean this gate could not measure is not a clean.
#
# ⛔ NOT IN `make test`, ON PURPOSE, AND DO NOT WIRE IT THERE. Three full master boards is ~3x a single board,
# and TENET condition 2 is that the blocking set is the shared resource that does not scale with seats (367
# serial arms, essentially serial, one 16-core box, ten seats queueing). This is a DELIBERATE gate: the owning
# row runs it, and a landing claiming Prolog's GC share is clean cites its output.
#
# ⛔ REFUSES rc=2 RATHER THAN REPORTING A CLEAN IT COULD NOT MEASURE: no binary, no suite, an arm that printed
# no SUITE_BOARD line, two arms whose entry sets differ, or an inconclusive HANG. A short count is never a green
# here -- the whole failure mode this gate is built against is a plausible wrong answer at exit 0.
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
S4E="${S4E_HOME:-$(cd "$ROOT/.." && pwd)}"
SCRIP="${SCRIP:-$ROOT/scrip}"; P="${PL_SUITE_DIR:-$S4E/corpus/tests/prolog}"
SHARDS="${SHARDS:-16}"; TINY_MB="${TINY_MB:-1}"; STRESS_N="${STRESS_N:-1}"
STRESS_TIMEOUT="${STRESS_TIMEOUT:-600}"; SHARD_TIMEOUT="${SHARD_TIMEOUT:-5400}"
ARMS="${ARMS:-control arena stress}"
refuse(){ echo "⛔ REFUSED-TO-GRADE(2): $*"; exit 2; }
[ -x "$SCRIP" ] || refuse "scrip not built at $SCRIP"
[ -f "$P/ALL.pl" ] && [ -f "$P/ALL.ref" ] || refuse "master suite not found at $P (want ALL.pl + ALL.ref)"
W="$(mktemp -d)" || refuse "mktemp failed"; trap 'rm -rf "$W"' EXIT
# ⭐ THE ARMS SHARE ONE PINNED BINARY FINGERPRINT ON PURPOSE. The harness refuses its own board if ./scrip or
# libscrip_rt.so moves mid-run; pinning S4E_BIN_AT_START across ALL arms extends that guarantee to the SET, so a
# rebuild between arms cannot pass itself off as an arena or stress effect. This is the REBASE-BASELINE
# COROLLARY in binary form: a before/after pair is a measurement only if both arms are the same tree.
S4E_BIN_AT_START="$(cd "$ROOT" && python3 -c 'import sys; sys.path.insert(0,"scripts"); import util_progress_append as p; print(p.bin_fingerprint().strip())' 2>/dev/null)"
[ -n "$S4E_BIN_AT_START" ] || refuse "could not fingerprint the binary -- no pinned baseline, so no comparable set"
export S4E_BIN_AT_START
echo "TREE SCRIP=$(git -C "$ROOT" rev-parse --short HEAD 2>/dev/null) corpus=$(git -C "$S4E/corpus" rev-parse --short HEAD 2>/dev/null) binary=$S4E_BIN_AT_START"
run_arm(){ # $1 = tag
  local tag="$1" k boards
  : > "$W/$tag.db"; : > "$W/$tag.raw"
  for k in $(seq 1 "$SHARDS"); do
    ( case "$tag" in
        control) unset SCRIP_HEAP_MB SCRIP_GC_STRESS ;;
        arena)   export SCRIP_HEAP_MB="$TINY_MB"; unset SCRIP_GC_STRESS ;;
        stress)  export SCRIP_HEAP_MB="$TINY_MB" SCRIP_GC_STRESS="$STRESS_N" TIMEOUT="$STRESS_TIMEOUT" ;;
        *) exit 9 ;;
      esac
      S4E_PROGRESS_DB="$W/$tag.db" timeout "$SHARD_TIMEOUT" python3 "$HERE/corpus_suite_harness.py" run \
        "$P/ALL.pl" "$P/ALL.ref" --lang prolog --modes m3,m4 --by-modes-column --shard "$k/$SHARDS" ) \
      >> "$W/$tag.raw" 2>&1
  done
  boards=$(grep -c '^SUITE_BOARD ' "$W/$tag.raw" || true)
  # ⛔ THE WHOLE HARNESS OUTPUT IS KEPT AND QUOTED ON A REFUSAL. Piping into `grep SUITE_BOARD` discards the ONE
  # RUNNER refusal, the likeliest reason a shard printed no board -- an honest refusal naming nothing actionable
  # (hq_B's lesson, carried on test_gate_pl_master_board_floor.sh).
  [ "$boards" -eq "$SHARDS" ] || { echo "---- $tag arm, last 40 lines ----"; tail -40 "$W/$tag.raw"; \
    refuse "$tag arm printed $boards of $SHARDS SUITE_BOARD lines -- no verdict is possible from a short count"; }
  [ -s "$W/$tag.db" ] || refuse "$tag arm recorded no progress rows -- nothing to compare per entry"
  # columns are util_progress_append.COLUMNS: 8=program 9=mode 10=outcome
  awk -F'\t' 'NR>1 && $8!="" {print $8"\x1f"$9"\t"$10}' "$W/$tag.db" | sort -u > "$W/$tag.tsv"
  echo "ARM $tag boards=$boards pairs=$(wc -l < "$W/$tag.tsv") $(grep -h '^ARENA ' "$W/$tag.raw" | head -1 | cut -c1-60)"
}
for a in $ARMS; do run_arm "$a"; done
case "$ARMS" in *control*) ;; *) refuse "the control arm is not in ARMS -- there is nothing to diff against";; esac
rc=0; inconclusive=0; total_div=0
for a in $ARMS; do
  [ "$a" = control ] && continue
  cut -f1 "$W/control.tsv" | sort -u > "$W/c.keys"; cut -f1 "$W/$a.tsv" | sort -u > "$W/$a.keys"
  if ! diff -q "$W/c.keys" "$W/$a.keys" >/dev/null; then
    echo "---- graded in one arm only (control vs $a) ----"; diff "$W/c.keys" "$W/$a.keys" | head -40
    refuse "arms control and $a graded DIFFERENT entry sets -- a diff across unequal populations is not a measurement"
  fi
  join -t$'\t' -j1 "$W/control.tsv" "$W/$a.tsv" > "$W/j.$a"
  awk -F'\t' -v arm="$a" '{split($1,k,"\x1f"); if ($2!=$3) printf "%s %s mode=%s control=%s %s=%s\n", ($3=="HANG" && $2!="HANG" ? "INCONCLUSIVE-SLOW" : "DIVERGENT"), k[1], k[2], $2, arm, $3}' "$W/j.$a" | sort > "$W/d.$a"
  nd=$(grep -c '^DIVERGENT ' "$W/d.$a" || true); ni=$(grep -c '^INCONCLUSIVE-SLOW ' "$W/d.$a" || true)
  np=$(wc -l < "$W/j.$a")
  echo "---- AXIS $a vs control: $np pair(s) compared ----"
  [ "$nd" -eq 0 ] && [ "$ni" -eq 0 ] && echo "(no movement)" || cat "$W/d.$a"
  echo "CENSUS pl-master-axis axis=$a pairs=$np divergent=$nd inconclusive=$ni want divergent=0 inconclusive=0"
  total_div=$((total_div+nd)); inconclusive=$((inconclusive+ni))
done
echo "---- PRE-EXISTING REDS (identical in EVERY arm -- completeness debt, NOT this gate's verdict) ----"
awk -F'\t' '$2!="PASS" && $2!="XFAIL" && $2!="SKIP" && $2!="OUTSIDE" {split($1,k,"\x1f"); printf "PRE-EXISTING-RED %s mode=%s outcome=%s\n", k[1], k[2], $2}' "$W/control.tsv" | sort
if [ "$total_div" -ne 0 ]; then
  echo "⛔ RESULT: FAIL -- $total_div (entry,mode) pair(s) CHANGED ANSWER against the control arm, each named above with its axis."
  echo "   Named, not counted: a pass total would have hidden every one of them, and a swap would have read identical."
  rc=1
fi
if [ "$inconclusive" -ne 0 ]; then
  echo "⛔ RESULT: REFUSE(2) -- $inconclusive pair(s) moved ONLY into HANG, which a timeout cannot tell from slowness."
  echo "   Raise STRESS_TIMEOUT (now ${STRESS_TIMEOUT}s) and re-run those entries, or record the duration. Never counted as a defect, never dropped."
  exit 2
fi
[ "$rc" -eq 0 ] && { echo "✅ RESULT: PASS -- every arm answers identically to control, per entry, BY ORACLE DIFF against ALL.ref."; \
  echo "   Axes varied: window (${TINY_MB} MB) and poll set (SCRIP_GC_STRESS=${STRESS_N}). Prolog's master does not change its answer under collection."; }
exit $rc
