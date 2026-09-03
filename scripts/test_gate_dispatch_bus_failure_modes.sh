#!/usr/bin/env bash
# test_gate_dispatch_bus_failure_modes.sh -- DONE-WHEN gate for fix-dispatch-bus-two-failure-modes (HQ s256).
#
# THE TWO FAILURE MODES FROM THE ORIGINAL BRIEF are each already fixed and each has its own dedicated
# negative-injection suite; this gate is the single canonical entry point that PROVES BOTH ARE STILL FIXED
# by composing those suites, then closes the one gap neither of them covers.
#   (1) identity depends on clone freshness, silent phantom-mailbox invention  -> test_gate_postoffice_identity.sh
#   (2) dispatch-by-inbox-alone (three sources of truth that could disagree)   -> test_gate_s4e_picker_v2.sh
#       (run here with --self-check too, so mode 2's own negative-injection proof runs as part of THIS gate)
#
# THE GAP THIS FILE ACTUALLY CLOSES: seat07's live report, q-s4e-msg-banner-attribution-undercount --
# next() and banner both resolved MULTIPLE of a seat's own claims by glob (alphabetical) file order, so a
# seat holding two open/assigned claims could be served, resumed, or attributed to the WRONG one. Cured by
# rank-sorting Pass 1/Pass 2 of next() the same way V2-1 already rank-sorts Pass 3 (free rows), and by
# having banner actually USE the topic done(topic) was already passing it (the call was `banner "$topic"
# ...` all along; banner just never read $2 for anything but the "-v" flag).
#
# ⛔ NEGATIVE-TESTED: --self-check re-runs every assertion below with the pre-fix (glob-order) answer
# injected in place of the real one -- same technique test_gate_s4e_picker_v2.sh already uses -- and
# requires itself to FAIL. A gate that cannot say NO is not a gate (seat16 measured 31/105 that cannot).
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; MSG="${S4E_MSG_BIN:-$HERE/s4e_msg.sh}"   # override = the real negative test
SELF=0; [ "${1:-}" = "--self-check" ] && SELF=1
# ⛔ FOUND BY HAND (fix-dispatch-bus-two-failure-modes, s266): Pass 3 self-invokes as `"$0" claim <topic>`,
# a direct exec, not `bash "$0"` -- a script copy that lost its +x bit (e.g. `git show HEAD:path > /tmp/x`)
# fails that exec silently inside the `if ... >/dev/null 2>&1` guard and every row reads as unclaimable,
# which looks exactly like "next() is broken" and is not. Refuse up front instead of misreporting it as one.
[ -x "$MSG" ] || { echo "⛔ $MSG is not executable -- Pass 3's \"\$0 claim ...\" self-invokes it directly, not via bash (set S4E_MSG_BIN to an executable copy)" >&2; exit 2; }
printf '  subject: %s%s\n' "$MSG" "$([ "$SELF" = 1 ] && echo '  [--self-check: injecting pre-fix answers]')"
pass=0; fail=0
ok(){ pass=$((pass+1)); printf '  ok   %s\n' "$1"; }
no(){ fail=$((fail+1)); printf '  FAIL %s\n     expected: %s\n     actual  : %s\n' "$1" "$2" "$3"; }
run_component() { # $1 = label, $2 = script, rest = its args. Folds a whole sub-gate's rc into ONE check here.
  local label="$1" script="$2" out rc; shift 2
  out="$(S4E_MSG_BIN="$MSG" bash "$script" "$@" 2>&1)"; rc=$?
  if [ "$rc" -eq 0 ]; then ok "$label"; else no "$label" "exit 0" "exit $rc -- tail:$(printf '%s' "$out" | tail -3 | tr '\n' '|')"; fi
}

echo "== component: FAILURE MODE 1 -- identity depends on clone freshness =="
# No self-check knob of its own (negative-injection was proved by hand against the pre-patch script, per
# its own header); always run normally here -- this component answers "is mode 1 still fixed," period.
run_component "postoffice identity suite (V2-3/V2-4, 18 negative-injection checks)" "$HERE/test_gate_postoffice_identity.sh"

echo "== component: FAILURE MODE 2 -- dispatch-by-inbox-alone is only a next()-overridable suggestion =="
if [ "$SELF" = 1 ]; then run_component "picker v2 suite, --self-check (proves IT can say NO)" "$HERE/test_gate_s4e_picker_v2.sh" --self-check
else run_component "picker v2 suite (rank-sort Pass 3 + assign-is-the-lock, 19 checks)" "$HERE/test_gate_s4e_picker_v2.sh"; fi

echo "== NEW: resume ordering -- next()/banner must not resolve a seat's OWN multiple claims by glob order =="
# Topics are named so ALPHABETICAL order and RANK order actively DISAGREE: aa-high-rank sorts first but
# carries the WORSE (higher/less urgent) rank; zz-low-rank sorts last but is the one the fleet actually
# wants served first. A glob-order implementation gets every check below wrong, not just wrong sometimes.
T=$(mktemp -d); trap 'rm -rf "$T"' EXIT
mkdir -p "$T/root/SCRIP/scripts"
printf '%s\n' '#!/bin/sh' 'echo HANDOFF COMPLETE' 'exit 0' > "$T/root/SCRIP/scripts/handoff_status.sh"; chmod +x "$T/root/SCRIP/scripts/handoff_status.sh"
run() { local seat="$1"; shift; S4E_POST="$T/po" S4E_HOME="$T/root" S4E_SEAT="$seat" S4E_NO_BANNER=1 bash "$MSG" "$@" 2>&1; }
newpo() { rm -rf "$T/po"; mkdir -p "$T/po"/{claims,tasks,seatAA/inbox,hq_C/inbox}
  printf '9\taa-high-rank\tb\tFREE\n1\tzz-low-rank\tb\tFREE\n' > "$T/po/QUEUE.tsv"; }

echo "-- A: next() Pass 1, two ASSIGNED-BY-but-not-yet-RUNNING claims --"
newpo
run hq_C assign seatAA aa-high-rank >/dev/null; run hq_C assign seatAA zz-low-rank >/dev/null; run seatAA clear >/dev/null
out="$(run seatAA next)"
[ "$SELF" = 1 ] && out="ASSIGNED->RUNNING aa-high-rank (dispatched by hq_C)"   # inject the pre-fix glob-first answer
case "$out" in *"ASSIGNED->RUNNING zz-low-rank"*) ok "Pass 1 serves the lower-rank assignment first, not the alphabetically-first one";;
  *) no "Pass 1 rank order among MY OWN claims" "ASSIGNED->RUNNING zz-low-rank" "$(echo "$out" | head -1)";; esac

echo "-- B: next() Pass 2, two already-open unfinished claims --"
newpo
printf 'seatAA\nRUNNING\n' > "$T/po/claims/aa-high-rank.claim"; printf 'seatAA\nRUNNING\n' > "$T/po/claims/zz-low-rank.claim"
out="$(run seatAA next)"
[ "$SELF" = 1 ] && out="RESUME aa-high-rank (yours, unfinished — s4e_msg.sh done aa-high-rank when the handoff clause is met)"
case "$out" in *"RESUME zz-low-rank"*) ok "Pass 2 resumes the lower-rank open claim first, not the alphabetically-first one";;
  *) no "Pass 2 rank order among MY OWN claims" "RESUME zz-low-rank" "$(echo "$out" | head -1)";; esac

echo "-- C: bare banner falls back to the same rank-sort, never glob order, among still-open claims --"
# same $T/po as B: aa-high-rank and zz-low-rank are both still open
out="$(run seatAA banner)"
[ "$SELF" = 1 ] && out="· row aa-high-rank"
case "$out" in *"row zz-low-rank"*) ok "bare banner attributes to the lower-rank open claim, not the alphabetically-first one";;
  *) no "banner rank-sort fallback" "row zz-low-rank" "$(echo "$out" | tail -3 | tr '\n' ' ')";; esac

echo "-- D: done(topic) already passes ITS topic to banner -- banner must USE it over the rank fallback --"
cat > "$T/po/tasks/aa-high-rank.task.md" <<EOF
# TASK aa-high-rank · owner: hq_C · state: FREE
GOAL: g
DONE-WHEN: test -f $T/flag
## NEXT
n
EOF
touch "$T/flag"
run seatAA done aa-high-rank >/dev/null   # S4E_NO_BANNER=1 in run(): verifies + marks DONE, does not fire banner yet
out="$(run seatAA banner aa-high-rank)"   # exactly what done's own internal "$0 banner $topic" call does
[ "$SELF" = 1 ] && out="· row zz-low-rank"   # inject the pre-fix "banner ignores its own \$2" answer
case "$out" in *"row aa-high-rank"*) ok "banner given an explicit topic (as done passes) reports THAT topic over the rank fallback";;
  *) no "banner pref-topic override" "row aa-high-rank" "$(echo "$out" | tail -3 | tr '\n' ' ')";; esac

echo "-- E: end-to-end -- done's OWN auto-fired banner (not a manual simulation) attributes correctly --"
# ⛔ FOUND STALE (postoffice-gates-red-on-origin, 2026-09-03): this step used to close zz-low-rank with NO
# baton on disk. That worked when the fixture was written and has been REFUSED rc=2 since the HOLE-A cure of
# 2026-08-28 -- "a row with no baton has no computable DONE-WHEN, so its completion cannot be verified, so it
# cannot be closed". The refusal is the tool being RIGHT; only D minted a baton, and E inherited none.
# ⭐ Note what the stale fixture would have cost if cured the other way: relaxing `done` to close a batonless
# row to make this gate green would have re-opened LAW 1 across all sixteen seats to spare one fixture.
# The banner-attribution property E actually measures is untouched by minting the baton E always needed.
cat > "$T/po/tasks/zz-low-rank.task.md" <<EOF
# TASK zz-low-rank · owner: hq_C · state: FREE
GOAL: g
DONE-WHEN: test -f $T/flag-zz
## NEXT
n
EOF
touch "$T/flag-zz"
out="$(S4E_POST="$T/po" S4E_HOME="$T/root" S4E_SEAT=seatAA bash "$MSG" done zz-low-rank 2>&1)"   # no S4E_NO_BANNER: let done fire its real banner call
[ "$SELF" = 1 ] && out="· row aa-high-rank"
case "$out" in *"row zz-low-rank"*) ok "done's own auto-fired banner (no manual passthrough) attributes to the row it just closed";;
  *) no "end-to-end done->banner passthrough" "row zz-low-rank" "$(echo "$out" | tail -3 | tr '\n' ' ')";; esac

printf '\n  %s: %d passed, %d failed\n' "$([ "$fail" -eq 0 ] && echo PASS || echo '⛔ FAIL')" "$pass" "$fail"
if [ "$SELF" = 1 ]; then
  if [ "$fail" -gt 0 ]; then printf '  ✅ SELF-CHECK PASSED: with pre-fix (glob-order) behaviour injected this gate said NO (%d failures). It can fail.\n' "$fail"; exit 0
  else printf '  ⛔ SELF-CHECK FAILED: pre-fix behaviour was injected and the gate still said PASS. This gate is BLIND — do not cite it.\n'; exit 1; fi; fi
[ "$fail" -eq 0 ]
