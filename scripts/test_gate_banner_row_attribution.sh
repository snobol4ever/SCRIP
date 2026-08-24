#!/usr/bin/env bash
# test_gate_banner_row_attribution.sh — regression gate for banner-attributes-wrong-row-on-unclaim
# (instrument-repair-bundle PART 1). `s4e_msg.sh banner`'s row1/rowst selector used to fall back, on
# a bare invocation with zero OPEN claims (exactly what `unclaim` leaves behind), to a rescan of EVERY
# claim ever held by this identity INCLUDING DONE ones — so a session that released its row via
# `unclaim` got a banner naming an ancient, unrelated DONE claim instead of the row it actually worked,
# and (via the commit/FINDING --grep="$row1" fallback) sometimes lost its own commit count too.
# Measured 4 independent times: seat08, seat06, seat02 (LEDGER of tasks/banner-attributes-wrong-row-
# on-unclaim.task.md), and this row's own repro while designing the fix.
#
# Runs entirely in a SANDBOXED postoffice/home (S4E_POST/S4E_HOME/S4E_SEAT overrides) — never touches
# the real /home/resources/postoffice or any seat root. Two negative tests, both directions named in
# the task's own DONE-WHEN: (A) a stale prior DONE claim must NOT surface for a session that released
# a DIFFERENT, real row via unclaim; (B) a session with a real, attributable commit must never report
# zero commits / "nothing landed" just because it has no OPEN claim left.
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"
set -u
SELF_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SELF="$SELF_DIR/s4e_msg.sh"
. "$SELF_DIR/lib_gate.sh"
gate_require "$SELF" "s4e_msg.sh"

fail=0
TMPHOME="$(mktemp -d)"; TMPPOST="$(mktemp -d)"
cleanup() { rm -rf "$TMPHOME" "$TMPPOST"; }
trap cleanup EXIT

run() { S4E_HOME="$TMPHOME" S4E_POST="$TMPPOST" S4E_SEAT="testseat" S4E_NO_BANNER=1 bash "$SELF" "$@"; }

mkdir -p "$TMPPOST/testseat/inbox" "$TMPPOST/claims" "$TMPPOST/tasks"
: > "$TMPPOST/QUEUE.tsv"
: > "$TMPPOST/BOARD.md"

# ============================================================================================
# SCENARIO A — a stale prior DONE claim must not surface for a session that released a real row.
# ============================================================================================
printf 'testseat\nRUNNING\nDONE\n' > "$TMPPOST/claims/ancient-stale-topic.claim"
printf 'testseat\nRUNNING\n' > "$TMPPOST/claims/real-topic-a.claim"
run unclaim real-topic-a "gate test release" >/dev/null 2>&1
out_a="$(run banner 2>&1)"
if printf '%s\n' "$out_a" | grep -q "ancient-stale-topic"; then
  echo "VIOLATION A: stale DONE claim 'ancient-stale-topic' surfaced in banner after unclaim of the UNRELATED 'real-topic-a'"
  printf '%s\n' "$out_a"
  fail=$((fail+1))
fi
if ! printf '%s\n' "$out_a" | grep -q "real-topic-a"; then
  echo "VIOLATION A2: the row actually released this session, 'real-topic-a', did not surface in banner at all"
  printf '%s\n' "$out_a"
  fail=$((fail+1))
fi

# ============================================================================================
# SCENARIO B — a real session with an attributable commit must never report zero / nothing-landed,
# even with no OPEN claim remaining (unclaim already ran once above, so the only claim left standing
# before this scenario is the DONE one from scenario A — exactly the shape that used to defeat cmts).
# ============================================================================================
REPO="$TMPHOME/fakerepo"; mkdir -p "$REPO"
git -C "$REPO" init -q
printf 'testseat\nRUNNING\n' > "$TMPPOST/claims/real-topic-b.claim"
run unclaim real-topic-b "gate test release" >/dev/null 2>&1
# commit lands AFTER the marker write above, and its message names ONLY the topic (not the seat) —
# deliberately exercising the row1-must-be-correct-for-the-grep-to-hit path, not the seat-name path.
git -C "$REPO" -c user.name=gatetest -c user.email=gatetest@test.invalid commit -q --allow-empty \
    -m "real-topic-b: witness commit for the banner attribution gate"
out_b="$(run banner 2>&1)"
if printf '%s\n' "$out_b" | grep -qiE 'NOTHING LANDED|NOTHING ATTRIBUTABLE LANDED|0 commit\(s\)'; then
  echo "VIOLATION B: a session with a real, attributable commit reported zero / nothing landed"
  printf '%s\n' "$out_b"
  fail=$((fail+1))
fi
if ! printf '%s\n' "$out_b" | grep -q "real-topic-b"; then
  echo "VIOLATION B2: the row actually released this session, 'real-topic-b', did not surface in banner at all"
  printf '%s\n' "$out_b"
  fail=$((fail+1))
fi

GATE_EXAMINED=2   # two negative-test scenarios, both directions named in the task's DONE-WHEN
gate_verdict "$fail" "banner row-attribution violation(s)"
