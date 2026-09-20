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
#   ARM control  : shipped window, no forced collection  -- the baseline every other arm is diffed against
#   ARM arena    : SCRIP_HEAP_MB=1                       -- does a SMALL WINDOW change an answer
#   ARM stress:N : SCRIP_HEAP_MB=1 + SCRIP_GC_STRESS=N   -- does COLLECTING OFTEN change an answer, one arm per N
#
# ⛔⭐⭐ A STRESS BAND, NEVER A STRESS POINT -- ceo CEO-1024, 2026-09-20, and it retired MY OWN first number.
# hq_raku ran the raku master at SCRIP_HEAP_MB=1 SCRIP_GC_STRESS=16 on the same tree and same binary as two
# green arms: m3 853 -> 817, m4 853 -> 824. SIXTY-FIVE GRADINGS LOST, ZERO GAINED, THIRTY-SIX DISTINCT
# PROGRAMS, every one exit 0 with no diagnostic and a plausible wrong answer, every one MATCHING at stress 0.
# The fleet had been grading at 1, 3 and 5 and calling that clean. ⛔ THE RULING: if your lane is clean at 1, 3
# and 5 you have not measured your lane, YOU HAVE MEASURED THE BAND.
# ⭐ AND THE JUDGEMENT ERROR IS WORTH MORE THAN THE NUMBER, because it is the one a careful seat makes: this
# gate's first cut used STRESS_N=8, picked as the top of hq_snobol4's measured red band {1,2,4,8}. Choosing the
# plant from where the KNOWN defects were is choosing a band that ENDS WHERE THE DEFECTS START -- evidence-based
# in feel and circular in fact. A single point cannot answer "is there a divergence point above this one", and
# that is the only question worth asking. So the band ASCENDS and the gate reports WHERE the divergence point
# is, rather than whether one chosen plant happened to sit on it.
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
SHARDS="${SHARDS:-16}"; TINY_MB="${TINY_MB:-1}"
# ⛔ THE DEFAULT BAND GOES WELL ABOVE 5 AND ABOVE 16 ON PURPOSE (CEO-1024). 16 is where hq_raku found 36 silently
# wrong raku programs; a band that STOPS at the highest value anyone has yet found a defect at is the same
# circular choice this gate was just corrected for, so it carries a value ABOVE the known one.
STRESS_BAND="${STRESS_BAND:-8 16 32}"
STRESS_TIMEOUT="${STRESS_TIMEOUT:-600}"; SHARD_TIMEOUT="${SHARD_TIMEOUT:-5400}"
ARMS="control arena"; for _n in $STRESS_BAND; do ARMS="$ARMS stress:$_n"; done; ARMS="${ARMS_OVERRIDE:-$ARMS}"
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
  local fn="${tag/:/_}"
  : > "$W/$fn.db"; : > "$W/$fn.raw"
  for k in $(seq 1 "$SHARDS"); do
    ( case "$tag" in
        control)  unset SCRIP_HEAP_MB SCRIP_GC_STRESS ;;
        arena)    export SCRIP_HEAP_MB="$TINY_MB"; unset SCRIP_GC_STRESS ;;
        stress:*) export SCRIP_HEAP_MB="$TINY_MB" SCRIP_GC_STRESS="${tag#stress:}" TIMEOUT="$STRESS_TIMEOUT" ;;
        *) exit 9 ;;
      esac
      S4E_PROGRESS_DB="$W/$fn.db" timeout "$SHARD_TIMEOUT" python3 "$HERE/corpus_suite_harness.py" run \
        "$P/ALL.pl" "$P/ALL.ref" --lang prolog --modes m3,m4 --by-modes-column --shard "$k/$SHARDS" ) \
      >> "$W/$fn.raw" 2>&1
  done
  boards=$(grep -c '^SUITE_BOARD ' "$W/$fn.raw" || true)
  # ⛔ THE WHOLE HARNESS OUTPUT IS KEPT AND QUOTED ON A REFUSAL. Piping into `grep SUITE_BOARD` discards the ONE
  # RUNNER refusal, the likeliest reason a shard printed no board -- an honest refusal naming nothing actionable
  # (hq_B's lesson, carried on test_gate_pl_master_board_floor.sh).
  [ "$boards" -eq "$SHARDS" ] || { echo "---- $tag arm, last 40 lines ----"; tail -40 "$W/$fn.raw"; \
    refuse "$tag arm printed $boards of $SHARDS SUITE_BOARD lines -- no verdict is possible from a short count"; }
  [ -s "$W/$fn.db" ] || refuse "$tag arm recorded no progress rows -- nothing to compare per entry"
  # columns are util_progress_append.COLUMNS: 8=program 9=mode 10=outcome
  awk -F'\t' 'NR>1 && $8!="" {print $8"\x1f"$9"\t"$10}' "$W/$fn.db" | sort -u > "$W/$fn.tsv"
  echo "ARM $tag boards=$boards pairs=$(wc -l < "$W/$fn.tsv") $(grep -h '^ARENA ' "$W/$fn.raw" | head -1 | cut -c1-60)"
}
for a in $ARMS; do run_arm "$a"; done
case "$ARMS" in *control*) ;; *) refuse "the control arm is not in ARMS -- there is nothing to diff against";; esac
rc=0; inconclusive=0; total_div=0
for a in $ARMS; do
  [ "$a" = control ] && continue
  cut -f1 "$W/control.tsv" | sort -u > "$W/c.keys"; cut -f1 "$W/${a/:/_}.tsv" | sort -u > "$W/${a/:/_}.keys"
  if ! diff -q "$W/c.keys" "$W/${a/:/_}.keys" >/dev/null; then
    echo "---- graded in one arm only (control vs $a) ----"; diff "$W/c.keys" "$W/${a/:/_}.keys" | head -40
    refuse "arms control and $a graded DIFFERENT entry sets -- a diff across unequal populations is not a measurement"
  fi
  join -t$'\t' -j1 "$W/control.tsv" "$W/${a/:/_}.tsv" > "$W/j.${a/:/_}"
  awk -F'\t' -v arm="$a" '{split($1,k,"\x1f"); if ($2!=$3) printf "%s %s mode=%s control=%s %s=%s\n", ($3=="HANG" && $2!="HANG" ? "INCONCLUSIVE-SLOW" : "DIVERGENT"), k[1], k[2], $2, arm, $3}' "$W/j.${a/:/_}" | sort > "$W/d.${a/:/_}"
  nd=$(grep -c '^DIVERGENT ' "$W/d.${a/:/_}" || true); ni=$(grep -c '^INCONCLUSIVE-SLOW ' "$W/d.${a/:/_}" || true)
  np=$(wc -l < "$W/j.${a/:/_}")
  echo "---- AXIS $a vs control: $np pair(s) compared ----"
  [ "$nd" -eq 0 ] && [ "$ni" -eq 0 ] && echo "(no movement)" || cat "$W/d.${a/:/_}"
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
  echo "   Axes varied: window (${TINY_MB} MB) and poll set, as a BAND: SCRIP_GC_STRESS in {${STRESS_BAND}}. Prolog's master does not change its answer under collection."; }
exit $rc
