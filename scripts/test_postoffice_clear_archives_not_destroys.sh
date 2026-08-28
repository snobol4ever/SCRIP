#!/usr/bin/env bash
# test_postoffice_clear_archives_not_destroys.sh -- THE MESSAGE-DESTRUCTION invariant (hq_P 2026-08-28, ceo-granted).
#
# THE DEFECT THIS PINS, and it is measured, not hypothetical -- it destroyed a message from hq_P to seat06 and one of
# ceo's the same morning.  `check` writes every basename into .last-check from its ITERATION LOOP, unconditionally, so
# .last-check records what check ITERATED and never what the reader actually SAW.  `clear` then acted on that list with
# `rm -f`, and there was no archive anywhere.  So ANY truncation between check's `cat` and the reader -- a pipe through
# head/tail, a pager, a context clip -- destroyed the body permanently while the seat believed it had read its mail.
#
# ⭐ THE TEST REPRODUCES THE TRUNCATION RATHER THAN DESCRIBING IT: it pipes `check` through `head -1`, which is exactly
# the shape that loses the body, then clears, then demands the body back.  A test that called check normally would pass
# against the BROKEN code too -- it would prove nothing, which is the vacuous-test class this project is sweeping for.
#
# THREE STATES:  rc=0 the body survived  |  rc=1 it was destroyed or mangled  |  rc=2 could not measure
set -u
here=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
msg="$here/s4e_msg.sh"
[ -f "$msg" ] || { echo "⛔ REFUSE: no s4e_msg.sh at $msg -- cannot measure"; exit 2; }
tmp=$(mktemp -d) || exit 2; trap 'rm -rf "$tmp"' EXIT
export S4E_POST="$tmp/po" S4E_SEAT=seat01
mkdir -p "$S4E_POST/seat01/inbox" || { echo "⛔ REFUSE: could not build a sandbox postoffice"; exit 2; }
CANARY='CANARY-BODY-b7f3e91a-this-line-must-survive-a-truncated-read'
printf 'FROM ceo TO seat01 RE canary\nheader line that head -1 will show\n%s\n' "$CANARY" > "$S4E_POST/seat01/inbox/1787900000000000000-ceo-canary.msg"
[ -s "$S4E_POST/seat01/inbox/1787900000000000000-ceo-canary.msg" ] || { echo "⛔ REFUSE: could not seed a message"; exit 2; }
# ⛔ THE NEGATIVE ARM: read it TRUNCATED.  head -1 closes the pipe after one line -- the reader never sees the body,
# but .last-check is already fully written, which is precisely the hazard.
bash "$msg" check 2>/dev/null | head -1 >/dev/null
[ -f "$S4E_POST/seat01/.last-check" ] || { echo "⛔ REFUSE: check wrote no .last-check -- the hazard's precondition did not reproduce, so this run proves nothing"; exit 2; }
grep -qxF '1787900000000000000-ceo-canary.msg' "$S4E_POST/seat01/.last-check" || { echo "⛔ REFUSE: the truncated read did not list the message -- precondition absent, nothing measured"; exit 2; }
bash "$msg" clear >/dev/null 2>&1
if [ -f "$S4E_POST/seat01/inbox/1787900000000000000-ceo-canary.msg" ]; then echo "⛔ REFUSE: clear did not clear the inbox -- this test measures survival AFTER a clear, and no clear happened"; exit 2; fi
found=$(grep -rlF "$CANARY" "$S4E_POST/seat01/archive" 2>/dev/null | head -1)
if [ -z "$found" ]; then
    echo "⛔ FAIL: the message body was DESTROYED by a truncated read + clear -- nothing in seat01/archive/ carries the canary."
    echo "        This is the seat06 defect: .last-check records what check ITERATED, not what the reader SAW."
    exit 1
fi
grep -qF 'header line that head -1 will show' "$found" || { echo "⛔ FAIL: archived copy is mangled -- header missing from $found"; exit 1; }
echo "=== POSTOFFICE MESSAGE-DESTRUCTION GATE: body survived a truncated read + clear, recovered from ${found#$S4E_POST/} ==="
echo "✅ PASS: clear ARCHIVES; a truncated read costs a lookup, not a message"
exit 0
