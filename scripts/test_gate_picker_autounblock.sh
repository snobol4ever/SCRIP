#!/usr/bin/env bash
# test_gate_picker_autounblock.sh -- DONE-WHEN gate for picker-skips-blocked-rows (ceo, 2026-08-28).
#
# BEFORE this row: a row encoded as BLOCKED-ON:<topic>/PARKED-AWAITING:<topic> in QUEUE.tsv's state column
# skipped next()'s PASS 3 FOREVER, even the session after <topic> landed DONE -- nothing ever re-asked. The
# live symptom: bench-rivals-raku-pascal sat encoded as plain "FREE" (the OTHER half of the same defect --
# nobody had even written the block down) and was re-discovered blocked by 9+ seats in one day.
#
# THIS GATE PROVES, against a SANDBOX postoffice (never the live one):
#   U1  a BLOCKED-ON row stays unpickable while its named blocker is still open (the row itself, in isolation
#       -- not just "some OTHER free row wins," which would also be true before this fix)
#   U2  the moment the blocker's claim carries DONE, a fresh `next` serves the previously-blocked row
#   U3  serving it self-heals the QUEUE.tsv state column back to FREE (so a human never has to remember to)
#   U4  PARKED-AWAITING:<topic> (the pre-existing spelling several real rows already use) gets the same cure
#   U5  a blocker resolved only via QUEUE.done.tsv (no live claim file at all) still unblocks -- the DONE-WHEN's
#       own "claims/ or QUEUE.done.tsv" wording, taken literally
#
# ⛔ NEGATIVE-TESTED: --self-check re-runs U2/U4/U5 with the pre-fix (permanently-stuck) answer injected in
# place of the real one -- same technique test_gate_s4e_picker_v2.sh already uses -- and requires itself to
# FAIL. A gate that cannot say NO is not a gate (seat16 measured 31/105 that cannot).
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; MSG="${S4E_MSG_BIN:-$HERE/s4e_msg.sh}"
SELF=0; [ "${1:-}" = "--self-check" ] && SELF=1
[ -x "$MSG" ] || { echo "⛔ $MSG is not executable -- PASS 3 self-invokes it directly, not via bash (set S4E_MSG_BIN to an executable copy)" >&2; exit 2; }
printf '  subject: %s%s\n' "$MSG" "$([ "$SELF" = 1 ] && echo '  [--self-check: injecting pre-fix (permanently-stuck) answers]')"
pass=0; fail=0
ok(){ pass=$((pass+1)); printf '  ok   %s\n' "$1"; }
no(){ fail=$((fail+1)); printf '  FAIL %s\n     expected: %s\n     actual  : %s\n' "$1" "$2" "$3"; }

T="$(mktemp -d)"; trap 'rm -rf "$T"' EXIT
run() { local seat="$1"; shift; S4E_POST="$T/po" S4E_HOME="$T/root" S4E_SEAT="$seat" S4E_NO_BANNER=1 bash "$MSG" "$@" 2>&1; }
newpo() { rm -rf "$T/po"; mkdir -p "$T/po"/{claims,tasks,seatAA/inbox,seatBB/inbox}; : > "$T/po/QUEUE.done.tsv"; }
mkroot() { rm -rf "$T/root"; mkdir -p "$T/root/SCRIP/scripts"
  printf '%s\n' '#!/bin/sh' 'echo HANDOFF COMPLETE' 'exit 0' > "$T/root/SCRIP/scripts/handoff_status.sh"
  chmod +x "$T/root/SCRIP/scripts/handoff_status.sh"; }
mkroot
task() { cat > "$T/po/tasks/$1.task.md" <<EOF
# TASK $1 · owner: unassigned · state: FREE
GOAL: gate fixture -- never a real row.
DONE-WHEN: test -e $T/flag-$1
## NEXT
n
EOF
}

echo "== U1  a BLOCKED-ON row stays unpickable, in isolation, while its blocker is open =="
newpo
printf '0\tblocker-x\tb\tFREE\n1\tblocked-x\tb\tBLOCKED-ON:blocker-x\n' > "$T/po/QUEUE.tsv"
task blocker-x; task blocked-x
out1="$(run seatAA next)"                      # claims blocker-x (rank 0, the only FREE row)
case "$out1" in *"LOCKED blocker-x"*) ok "first next() takes the only actually-free row (blocker-x)";;
  *) no "setup: blocker-x gets locked" "LOCKED blocker-x" "$(echo "$out1" | grep -m1 LOCKED)";; esac
out2="$(run seatBB next)"                      # a SECOND, unrelated seat: blocked-x must not be servable to anyone
case "$out2" in *"blocked-x"*) no "U1: blocked-x must not be served to any seat while blocker-x is open" "QUEUE EMPTY" "$(echo "$out2" | head -1)";;
  *) ok "U1: blocked-x stays unpickable fleet-wide while blocker-x is still open (QUEUE EMPTY)";; esac

echo "== U2  the moment the blocker lands DONE, a fresh next() serves the previously-blocked row =="
touch "$T/flag-blocker-x"; run seatAA done blocker-x >/dev/null
out3="$(run seatAA next)"
if [ "$SELF" = 1 ]; then out3="QUEUE EMPTY — every row claimed. Ask hq: s4e_msg.sh ask work 'queue empty'"; fi   # inject the pre-fix permanently-stuck answer
case "$out3" in *"LOCKED blocked-x"*) ok "U2: resolved blocker unsticks the row on the very next dispatch";;
  *) no "U2: blocked-x becomes pickable once blocker-x is DONE" "LOCKED blocked-x (rank 1)" "$(echo "$out3" | head -1)";; esac

echo "== U3  serving it self-heals the QUEUE.tsv state column back to FREE =="
st="$(awk -F'\t' '$2=="blocked-x"{print $4}' "$T/po/QUEUE.tsv")"
if [ "$SELF" = 1 ]; then st="BLOCKED-ON:blocker-x"; fi   # inject the pre-fix answer: state column never rewritten
[ "$st" = "FREE" ] && ok "U3: QUEUE.tsv row for blocked-x now reads FREE, not a stale BLOCKED-ON" \
  || no "U3: state column self-heals" "FREE" "$st"

echo "== U4  PARKED-AWAITING:<topic> (the pre-existing spelling) gets the identical cure =="
newpo
printf '0\tblocker-y\tb\tFREE\n1\tblocked-y\tb\tPARKED-AWAITING:blocker-y\n' > "$T/po/QUEUE.tsv"
task blocker-y; task blocked-y
run seatAA next >/dev/null                     # claims blocker-y
touch "$T/flag-blocker-y"; run seatAA done blocker-y >/dev/null
out4="$(run seatBB next)"
if [ "$SELF" = 1 ]; then out4="QUEUE EMPTY — every row claimed. Ask hq: s4e_msg.sh ask work 'queue empty'"; fi
case "$out4" in *"LOCKED blocked-y"*) ok "U4: PARKED-AWAITING unsticks exactly like BLOCKED-ON";;
  *) no "U4: PARKED-AWAITING:<done-topic> becomes pickable" "LOCKED blocked-y (rank 1)" "$(echo "$out4" | head -1)";; esac

echo "== U5  a blocker resolved ONLY via QUEUE.done.tsv (no live claim file) still unblocks =="
newpo
printf '1\tblocked-z\tb\tBLOCKED-ON:blocker-z\n' > "$T/po/QUEUE.tsv"     # blocker-z has NO row here at all -- fully swept
printf '0\tblocker-z\tb\tFREE\n' > "$T/po/QUEUE.done.tsv"                # ...but IS recorded landed in the memory file
task blocked-z
out5="$(run seatAA next)"
if [ "$SELF" = 1 ]; then out5="QUEUE EMPTY — every row claimed. Ask hq: s4e_msg.sh ask work 'queue empty'"; fi
case "$out5" in *"LOCKED blocked-z"*) ok "U5: QUEUE.done.tsv alone (no claims/ evidence) is enough to unblock";;
  *) no "U5: blocker resolved only in QUEUE.done.tsv still unblocks" "LOCKED blocked-z (rank 1)" "$(echo "$out5" | head -1)";; esac

printf '\n  %s: %d passed, %d failed\n' "$([ "$fail" -eq 0 ] && echo PASS || echo '⛔ FAIL')" "$pass" "$fail"
if [ "$SELF" = 1 ]; then
  if [ "$fail" -gt 0 ]; then printf '  ✅ SELF-CHECK PASSED: with the pre-fix (permanently-stuck) answer injected this gate said NO (%d failures). It can fail.\n' "$fail"; exit 0
  else printf '  ⛔ SELF-CHECK FAILED: pre-fix behaviour was injected and the gate still said PASS. This gate is BLIND -- do not cite it.\n'; exit 1; fi; fi
[ "$fail" -eq 0 ]
