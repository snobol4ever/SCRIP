#!/bin/bash
# scripts/util_raku_roast_ablation_ranker.sh -- RANK A CURE BY ABLATION, NEVER BY FREQUENCY (hq_raku 2026-09-16,
# baton raku-every-roast-file-run-graded-against-rakudo-or-named-ungradable, TAKE NEXT item 1).
#
# ⛔⭐ THE INSTRUMENT THIS ROW LACKED, AND WHY THE ROW STALLED WITHOUT IT. raku_roast_scoreboard.sh --inventory
# prints ROAST_TOP_BLOCKING_CONSTRUCTS: a frequency histogram of the source line each file's parse died on.
# That list is A QUEUE, NOT A BOTTLENECK -- it ranks what each file hits FIRST, which is not what is BLOCKING
# it, and curing the head of the list merely advances those files to their next error. THE MEASUREMENT that
# settled it (hq_T 2026-09-14): the top entry headed 59 files; deleting that line from all 59 left 57 still
# parse-failing on ~40 different next constructs and moved EXACTLY ZERO into a graded bucket. The cure was
# fully defensible from the histogram and would have bought nothing.
# ⭐ SO THE ONLY HONEST UNLOCK METRIC IS ABLATION: remove the construct, re-run, and count FILES THAT MOVED.
# Frequency is where the queue is LONG; value is where the queue is SHORT BEHIND IT, and those are different
# lists. This script computes the second one. Frequency is used ONLY to nominate candidates -- never to rank
# them -- because you must start the ablation somewhere and the histogram is a free ordering of where files are.
#
# ⛔ IT WRITES NOTHING: no SCORE row, no RAKU-COVERAGE.md, no board. ONE RUNNER ONE BOARD is untouched
# (CEO-744) -- this publishes nothing and grades no published population, so it is admitted to any seat on
# the CEO-547 rule that a board is what it GRADES AND PUBLISHES.
# ⛔ IT REFUSES rc=2 RATHER THAN PRINTING THE SUCCESS SHAPE over a population it could not measure, and it
# PRINTS ITS DENOMINATOR on every line (CLAUDE.md § Testing).
#
# Usage: bash scripts/util_raku_roast_ablation_ranker.sh [--limit N] [--candidates K] [--section S03]
#   --limit N       census only the first N shipped .t files (bound the cost; default all)
#   --candidates K  ablate the K most frequent first-error constructs (default 8)
#   --section S     only files whose path starts with this prefix
set -u
HERE="$(cd "$(dirname "$0")" && pwd)"
cd "$HERE/.." || exit 2
ROOT=$(pwd)
SCRIP="$ROOT/scrip"
. "$HERE/lib_raku_roast_bucket.sh" || { echo "⛔ REFUSE(2): lib_raku_roast_bucket.sh unavailable" >&2; exit 2; }
command -v roast_bucket >/dev/null 2>&1 || { echo "⛔ REFUSE(2): lib_raku_roast_bucket.sh missing roast_bucket -- the ranker must use the SAME bucket rule as the census it ranks" >&2; exit 2; }
# ⛔ A STALE BINARY MAKES EVERY ABLATION NUMBER FICTION, and unlike a board this instrument has no tree stamp
# to betray it (FINDING-2026-08-30-hq_C: a board grades whatever scrip exists and labels it with git HEAD).
if ! . "$HERE/lib_gate.sh" 2>/dev/null || ! command -v gate_require_fresh >/dev/null 2>&1; then
  echo "⛔ REFUSE(2): lib_gate.sh unavailable or missing gate_require_fresh -- cannot verify binary freshness" >&2; exit 2
fi
gate_require_fresh "$ROOT" src "$SCRIP" "$ROOT/out/libscrip_rt.so"
[ -x "$SCRIP" ] || { echo "⛔ REFUSE(2): no scrip binary (run: make)" >&2; exit 2; }
roast_resolve_population "$ROOT" || exit 2

LIMIT=0; CANDS=8; SECTION=""
while [ $# -gt 0 ]; do
  case "$1" in
    --limit) LIMIT="$2"; shift 2 ;;
    --candidates) CANDS="$2"; shift 2 ;;
    --section) SECTION="$2"; shift 2 ;;
    *) echo "⛔ REFUSE(2): unknown argument '$1' -- this instrument names what it does not understand rather than treating it as a filename" >&2; exit 2 ;;
  esac
done
TMP=$(mktemp -d); trap 'rm -rf "$TMP"' EXIT
CSV="$TMP/census.tsv"; : > "$CSV"

# \u26d4 CALLERS INVOKE THIS AS `run_one <f> 2>/dev/null`: a witness that dies on SIGSEGV/SIGABRT makes
# bash report "Segmentation fault" to the RANKER's own stderr as it reaps the child, interleaving compiler
# crashes into the instrument's report. The crash is still CAPTURED in "$se" and classified either way (a
# rc>=124 is UNGRADABLE-TIMEOUT, a signal death lands in the TAP classifier), so the notice carries no
# information the buckets do not already hold. \u26d4 Redirecting INSIDE the command cannot suppress it --
# the shell prints it at reap time, in the caller.
run_one() {  # $1=file to run -> sets B_BUCKET B_LINE B_KEY
  local f="$1" so="$TMP/o" se="$TMP/e" rc err1
  cp "$f" "$TMP/case.raku"
  # ⛔ THE SUBSHELL IS LOAD-BEARING: a witness that dies on SIGABRT makes bash print "Aborted" to the
  # RANKER's own stderr, interleaving compiler crashes into the instrument's report. Its stderr is captured
  # in "$se" and classified either way, so silencing the job-control notice loses no information.
  timeout 10 "$SCRIP" --run "$TMP/case.raku" > "$so" 2> "$se" < /dev/null; rc=$?
  B_BUCKET=$(roast_bucket "$so" "$se" "$rc")
  err1=$(head -1 "$se" 2>/dev/null)
  B_LINE=$(roast_err_line "$err1")
  B_KEY="."
  [ -n "$B_LINE" ] && B_KEY=$(sed -n "${B_LINE}p" "$f" 2>/dev/null | sed 's/^[ \t]*//' | cut -c1-90 | roast_fold_construct)
}

echo "RANKER PASS 1 -- census (the same bucket rule raku_roast_scoreboard.sh --inventory uses, sourced not copied)" >&2
n=0
while IFS= read -r -d '' src; do
  rel="${src#$ROAST/}"
  [ -n "$SECTION" ] && case "$rel" in "$SECTION"*) : ;; *) continue ;; esac
  n=$((n+1)); [ "$LIMIT" -gt 0 ] && [ "$n" -gt "$LIMIT" ] && { n=$((n-1)); break; }
  run_one "$src" 2>/dev/null
  printf '%s\t%s\t%s\t%s\n' "$rel" "$B_BUCKET" "${B_LINE:-.}" "${B_KEY:-.}" >> "$CSV"
  [ $((n % 200)) -eq 0 ] && printf '  ...%d censused\n' "$n" >&2
done < <(find -L "$ROAST" -name '*.t' -print0 | sort -z)
[ "$n" -gt 0 ] || { echo "⛔ REFUSE(2): censused ZERO files -- a ranker with no population is not a ranker" >&2; exit 2; }

parse_n=$(awk -F'\t' '$2=="UNGRADED-PARSE"' "$CSV" | wc -l)
graded_n=$(awk -F'\t' '$2 ~ /^GRADED-/' "$CSV" | wc -l)
[ "$parse_n" -gt 0 ] || { echo "⛔ REFUSE(2): no UNGRADED-PARSE files in a census of $n -- nothing to ablate, and a ranker that prints an empty ranking reads like a solved problem" >&2; exit 2; }

# Frequency NOMINATES the candidates; ablation RANKS them. These are different jobs and the output says so.
awk -F'\t' '$2=="UNGRADED-PARSE" && $4!="."{print $4}' "$CSV" | sort | uniq -c | sort -rn | head -"$CANDS" > "$TMP/cands"
cand_n=$(wc -l < "$TMP/cands")
[ "$cand_n" -gt 0 ] || { echo "⛔ REFUSE(2): no candidate constructs with a recoverable error line" >&2; exit 2; }

echo "RANKER PASS 2 -- ablating $cand_n candidate construct(s)" >&2
RANK="$TMP/rank"; : > "$RANK"
ci=0
while IFS= read -r line; do
  ci=$((ci+1))
  key=$(printf '%s' "$line" | sed 's/^ *[0-9]* //')
  headed=0; moved=0; past=0; advanced=0; same=0; earlier=0
  while IFS=$'\t' read -r rel bucket eline ekey; do
    [ "$bucket" = UNGRADED-PARSE ] || continue
    [ "$ekey" = "$key" ] || continue
    [ "$eline" = "." ] && continue
    headed=$((headed+1))
    # THE ABLATION OPERATOR IS DELETION OF THE DYING LINE -- the same operator hq_T's 59-file measurement used.
    sed "${eline}d" "$ROAST/$rel" > "$TMP/ablated.t" 2>/dev/null || continue
    run_one "$TMP/ablated.t" 2>/dev/null
    case "$B_BUCKET" in
      GRADED-*) moved=$((moved+1)) ;;
      UNGRADED-PARSE)
        if [ "$B_KEY" = "$key" ]; then same=$((same+1)); else advanced=$((advanced+1)); fi
        if [ -n "$B_LINE" ] && [ "$B_LINE" -lt "$eline" ] 2>/dev/null; then earlier=$((earlier+1)); fi ;;
      *) past=$((past+1)) ;;
    esac
  done < "$CSV"
  printf '%d\t%d\t%d\t%d\t%d\t%d\t%s\n' "$moved" "$past" "$advanced" "$same" "$headed" "$earlier" "$key" >> "$RANK"
  printf '  ...candidate %d/%d headed=%d moved=%d\n' "$ci" "$cand_n" "$headed" "$moved" >&2
done < "$TMP/cands"

echo "ROAST_ABLATION_RANK census=$n parse_dark=$parse_n graded=$graded_n candidates=$cand_n roast=$ROAST"
echo "  ⭐ RANKED BY moved_to_graded -- the ONLY column that is an unlock. headed= is the frequency histogram's"
echo "     number and is shown only so the two can be compared; a large headed= with moved=0 is the exact shape"
echo "     that makes a frequency list look like a work plan when it is not."
echo "  columns: moved=became GRADED · past_parse=left the parse bucket but not graded · advanced=still parse, NEW construct · same=ablation did not take"
sort -rn "$RANK" | awk -F'\t' '{printf "    moved=%-4s past_parse=%-4s advanced=%-4s same=%-4s headed=%-4s  %s\n", $1, $2, $3, $4, $5, $7}'
tot_moved=$(awk -F'\t' '{s+=$1} END{print s+0}' "$RANK")
tot_head=$(awk -F'\t' '{s+=$5} END{print s+0}' "$RANK")
tot_earlier=$(awk -F'\t' '{s+=$6} END{print s+0}' "$RANK")
echo "ROAST_ABLATION_TOTAL candidates=$cand_n files_headed=$tot_head files_moved_to_graded=$tot_moved"
[ "$tot_earlier" -gt 0 ] && echo "  ⚠️ $tot_earlier ablation(s) made the parse die EARLIER than the deleted line -- deletion damaged structure there, so those files' 'did not move' is UNPROVEN, not evidence the construct is cheap."
if [ "$tot_moved" = 0 ]; then
  echo "⛔ ZERO FILES MOVED across $tot_head headed by the $cand_n most frequent constructs. Read this as the baton reads it:"
  echo "   roast is a BROAD FLAT PARSER GAP with no block-unlock in the head of the histogram. Disbelieve any plan"
  echo "   promising a large roast jump from a handful of cures unless an ablation like this one backs it."
fi
echo "ROAST_ABLATION_DONE ⛔ NOTHING WRITTEN: no SCORE row, no RAKU-COVERAGE.md, no board."
exit 0
