#!/bin/bash
# scripts/lib_raku_roast_bucket.sh -- THE ONE BUCKET AUTHORITY FOR THE RAKU ROAST TREE (hq_raku 2026-09-16).
# Extracted VERBATIM from raku_roast_scoreboard.sh's --inventory arm on the lib_ladder.sh / lib_port_trace.sh
# precedent: one body, sourced, never a second copy. It exists because the ABLATION RANKER must decide
# "did this file move into a GRADED bucket" using the SAME rule the inventory used to say it was dark --
# and a ranker carrying its own private copy of that rule would be free to disagree with the census it is
# ranking, silently and in the flattering direction.
# ⛔ WHY AN EXTRACTION AND NOT A REIMPLEMENTATION: the measurement this feeds is "files moved into a graded
# bucket", which is a DIFFERENCE between two classifications. A difference taken across two copies of a
# classifier measures the copies as much as the cure -- so the two arms must be the same bytes, not merely
# the same intent (CLAUDE.md § Testing: shared authorities are sourced rather than reimplemented).
# Contract: classify <stdout> <stderr> <rc> -> PASS|FAIL|PARSE-FAIL|CRASH|NO-TAP  (the TAP verdict)
#           roast_bucket <stdout> <stderr> <rc> -> GRADED-PASS|GRADED-FAIL|UNGRADED-PARSE|UNGRADED-EMITTER|
#                                                  UNGRADED-NO-TAP|UNGRADED-OTHER|UNGRADABLE-TIMEOUT
classify() {
  # $1=stdout file  $2=stderr file  $3=rc  -> echoes one of PASS/FAIL/PARSE-FAIL/CRASH/NO-TAP
  local so="$1" se="$2" rc="$3"
  if grep -q "parse error" "$se" 2>/dev/null; then echo "PARSE-FAIL"; return; fi
  if [ "$rc" -ge 124 ]; then echo "CRASH"; return; fi
  local plan ok notok
  plan=$(grep -cE '^1\.\.[0-9]+' "$so" 2>/dev/null)
  ok=$(grep -cE '^ok [0-9]+' "$so" 2>/dev/null)
  notok=$(grep -cE '^not ok [0-9]+' "$so" 2>/dev/null)
  if [ "$plan" -eq 0 ] && [ "$ok" -eq 0 ] && [ "$notok" -eq 0 ]; then echo "NO-TAP"; return; fi
  if [ "$notok" -gt 0 ]; then echo "FAIL"; return; fi
  if [ "$rc" -ne 0 ]; then echo "FAIL"; return; fi
  echo "PASS"
}
# THE BUCKET. GRADED means our compiler ran the file and TAP came out; everything else is a reason it did not.
roast_bucket() {
  local so="$1" se="$2" rc="$3" err1
  err1=$(head -1 "$se" 2>/dev/null)
  if printf '%s' "$err1" | grep -q 'parse error'; then echo UNGRADED-PARSE; return; fi
  if printf '%s' "$err1" | grep -q 'does not yet cover'; then echo UNGRADED-EMITTER; return; fi
  # ⛔⭐⭐ A CRASH IS NOT A TIMEOUT, AND THIS LINE CALLED EVERY SIGNAL DEATH A TIMEOUT (hq_raku
  # 2026-09-16, found by doing what the baton said: TIME the three UNGRADABLE-TIMEOUT files before
  # classifying them). `rc -ge 124` is true for 124 (timeout fired) AND for 134 (SIGABRT) AND 139 (SIGSEGV),
  # so every crash landed in a bucket named for the clock. THE MEASUREMENT: all three "timeout" files finish
  # in ~1.4s -- rc=134, rc=139, rc=139. None of them was ever slow. Our front end crashes on them.
  # ⛔ THE CONSEQUENCE IS THE WHOLE POINT, NOT THE LABEL: UNGRADABLE- reads as "not our problem, out of
  # the numerator honestly" (the oracle refuses it, it needs input we do not ship). A compiler that SIGSEGVs
  # is our defect and belongs in the DARK count where it is owed, not in the excused one. Three files were
  # sitting in the excused column with a SIGSEGV behind them.
  # ⭐ AND THE INFORMATION WAS ALREADY IN THE BUILDING: classify() below returns "CRASH" for exactly this
  # condition. The bucket layer re-tested the same rc and overwrote a correct name with a worse one -- which
  # also made classify()'s CRASH arm dead code on the roast path. When two layers classify the same fact,
  # the outer one silently wins, so they must not both decide.
  if [ "$rc" -eq 124 ]; then echo UNGRADABLE-TIMEOUT; return; fi
  if [ "$rc" -ge 128 ]; then echo UNGRADED-CRASH; return; fi
  if [ "$rc" -ge 125 ] && [ "$rc" -le 127 ]; then echo UNGRADED-OTHER; return; fi
  case "$(classify "$so" "$se" "$rc")" in
    PASS)   echo GRADED-PASS ;;
    FAIL)   echo GRADED-FAIL ;;
    NO-TAP) echo UNGRADED-NO-TAP ;;
    *)      echo UNGRADED-OTHER ;;
  esac
}
# THE CONSTRUCT KEY: the source line the parse died on, literals and numbers folded, so a histogram and a
# ranker name CONSTRUCTS and not line numbers -- and so both name them THE SAME WAY.
roast_fold_construct() { sed "s/'[^']*'/'STR'/g; s/\"[^\"]*\"/\"STR\"/g; s/[0-9][0-9]*/N/g"; }
roast_err_line() { printf '%s' "$1" | sed -n 's/.*line \([0-9]\+\).*/\1/p'; }
# ⛔⭐ THE POPULATION RESOLVER, SHARED FOR THE SAME REASON THE BUCKET RULE IS (hq_raku 2026-09-16):
# an instrument that RANKS a census must grade the same files the census graded. Resolution order is
# per-root refs/ FIRST (a root still carrying its own gitignored copy keeps grading that copy, so this
# moves no existing measurement), then the shared read-only tree under /home/resources, then an explicit
# env override. ⛔ REFUSES naming every candidate tried, rather than grading a population nobody chose.
resolve_pop() {  # $1=env override  $2..=candidates, first that exists wins
  # ⛔ AN EXPLICIT OVERRIDE IS CHECKED LIKE ANY OTHER CANDIDATE (hq_raku 2026-09-16, caught by this lib's
  # own fail-once arm). It used to be returned unvalidated, so RAKU_ROAST_TREE=/nonexistent resolved happily
  # and the caller refused several steps later with "censused ZERO files" -- a true refusal naming the wrong
  # cause, which is the shape that sends the next seat to look at the census loop instead of at their env.
  local ov="$1"; shift
  if [ -n "$ov" ]; then
    [ -e "$ov" ] || { printf '%s\n' ""; return 1; }
    printf '%s\n' "$ov"; return 0
  fi
  local c; for c in "$@"; do [ -e "$c" ] && { printf '%s\n' "$c"; return 0; }; done
  printf '%s\n' ""; return 1
}
roast_resolve_population() {  # $1=ROOT -> sets ROAST and MANIFEST, or refuses rc=2
  local root="$1"
  MANIFEST=$(resolve_pop "${RAKU_ROAST_MANIFEST:-}" "$root/refs/rakudo-main/t/spectest.data.6.c" /home/resources/rakudo-main/t/spectest.data.6.c) || {
    echo "⛔ REFUSE(2) [roast]: no spectest manifest -- tried \$RAKU_ROAST_MANIFEST, $root/refs/rakudo-main/t/spectest.data.6.c, /home/resources/rakudo-main/t/spectest.data.6.c" >&2; return 2; }
  ROAST=$(resolve_pop "${RAKU_ROAST_TREE:-}" "$root/refs/roast" /home/resources/roast-master) || {
    echo "⛔ REFUSE(2) [roast]: no roast tree -- tried \$RAKU_ROAST_TREE, $root/refs/roast, /home/resources/roast-master" >&2; return 2; }
  return 0
}
