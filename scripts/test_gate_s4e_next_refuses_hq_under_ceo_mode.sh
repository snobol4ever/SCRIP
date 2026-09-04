#!/usr/bin/env bash
# test_gate_s4e_next_refuses_hq_under_ceo_mode.sh -- `s4e_msg.sh next` REFUSES rc=2 to dispatch to an identity that
# is not standing in the current MODE, instead of reading the MODE file for DISPLAY and dispatching anyway.
#
# Row next-refuses-to-dispatch-to-an-hq-seat-when-mode-line-1-is-ceo (hq_C measured 2026-09-03 18:51, routed by ceo
# CEO-175): next printed "MODE: CEO" and in the SAME PRINTOUT served and LOCKED a Prolog rung to hq_C. ⛔ The damage
# is the LOCK, not the wasted turn -- a claimed row leaves the picker, so a stood-down identity holding a claim hides
# that row from the whole fleet, by a route nobody watches (`fleet` reports a seat that STALLS, never one that was
# never meant to run).
#
# SIX ARMS against a THROWAWAY postoffice under mktemp -- never the live one. Both REFUSALS and, just as important,
# the three ALLOWS: a guard proven only by what it blocks is indistinguishable from one that blocks everything.
#   (A) hq_B + MODE CEO      -> rc=2, names the mode AND the MODE file, and the row is NOT claimed.
#   (B) seat01 + MODE QUARTET-> rc=2 (no fleet in QUARTET), same two names, row NOT claimed.
#   (C) ceo + MODE CEO       -> ALLOWED. The ceo is exactly who works under CEO; s4e_is_hq() counts ceo as an HQ
#                              because it is an AUTHORITY test, so a guard written on that predicate would refuse
#                              the one identity that must not be refused. This arm pins that distinction.
#   (D) hq_B + MODE FLEET-16 -> ALLOWED (the live mode today).
#   (E) hq_B + NO MODE FILE  -> ALLOWED. Every existing s4e gate fixture omits it; refusing here would break them,
#                              and s4e_mode_line already reports absence loudly. Pinned so nobody "tightens" it.
#   (F) FAIL-ONCE: a mutant copy with the guard's `exit 2` neutered must make arm (A) dispatch again. A gate that
#                  cannot go red proves nothing (INSTRUMENT LAWS, fifth batch).
# EXIT 0 all six hold; 1 any arm wrong; 2 REFUSED (fixture cannot be built).
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; MSG="$HERE/s4e_msg.sh"
ROOT="$(cd "$HERE/../.." && pwd)"
[ -f "$MSG" ] || { echo "⛔ REFUSED: $MSG missing -- nothing was checked"; exit 2; }
W="$(mktemp -d)" || { echo "⛔ REFUSED: mktemp failed -- nothing was checked"; exit 2; }; trap 'rm -rf "$W"' EXIT
PO="$W/po"; ROW=gate-fixture-row
mk_po() {  # $1 = mode text, or the literal NONE to omit the file entirely
  rm -rf "$PO"; mkdir -p "$PO/tasks" "$PO/claims" || return 2
  for m in ceo hq_B seat01; do mkdir -p "$PO/$m/inbox" || return 2; done
  : > "$PO/BOARD.md"; : > "$PO/QUEUE.done.tsv"
  [ "$1" = NONE ] || printf '%s\n' "$1" > "$PO/MODE"
  { printf '# gate fixture queue\n'; printf '0\t%s\tunassigned\tFREE\n' "$ROW"; } > "$PO/QUEUE.tsv"
  printf '# TASK %s\nGOAL: gate fixture.\nDONE-WHEN: test -f /no-such-file-next-mode-gate\nLINKS: none\n## NEXT\n(none)\n## LEDGER\n' "$ROW" > "$PO/tasks/$ROW.task.md"
  return 0; }
run_next() { S4E_POST="$PO" S4E_SEAT="$1" S4E_HOME="$ROOT" S4E_NO_BANNER=1 bash "${2:-$MSG}" next 2>&1; }
claimed() { [ -f "$PO/claims/$ROW.claim" ]; }
fail=0
arm_refuse() { # $1 seat  $2 mode  $3 label
  mk_po "$2" || { echo "⛔ REFUSED: fixture build failed"; exit 2; }
  out="$(run_next "$1")"; rc=$?
  if [ "$rc" -ne 2 ]; then echo "⛔ $3: expected rc=2, got rc=$rc"; echo "$out" | sed 's/^/      /'; fail=1; return; fi
  case "$out" in *"$2"*) :;; *) echo "⛔ $3: refusal does not NAME the mode ($2)"; fail=1;; esac
  case "$out" in *"$PO/MODE"*) :;; *) echo "⛔ $3: refusal does not NAME the MODE file"; fail=1;; esac
  if claimed; then echo "⛔ $3: REFUSED but the row was CLAIMED anyway -- the lock is the whole harm"; fail=1; return; fi
  echo "ok  $3: rc=2, names the mode and the file, row NOT claimed"; }
arm_allow() { # $1 seat  $2 mode  $3 label
  mk_po "$2" || { echo "⛔ REFUSED: fixture build failed"; exit 2; }
  out="$(run_next "$1")"; rc=$?
  if [ "$rc" -eq 2 ] && printf '%s' "$out" | grep -q "REFUSING TO DISPATCH"; then
      echo "⛔ $3: expected dispatch, got the mode refusal"; echo "$out" | sed 's/^/      /'; fail=1; return; fi
  if ! claimed; then echo "⛔ $3: expected the row to be claimed, it was not (rc=$rc)"; echo "$out" | sed 's/^/      /'; fail=1; return; fi
  echo "ok  $3: dispatched and claimed (rc=$rc)"; }
arm_refuse hq_B   CEO     "(A) hq_B under CEO"
arm_refuse seat01 QUARTET "(B) seat01 under QUARTET"
arm_allow  ceo    CEO     "(C) ceo under CEO is NEVER refused"
arm_allow  hq_B   FLEET-16 "(D) hq_B under FLEET-16"
arm_allow  hq_B   NONE     "(E) hq_B with NO MODE file"
# ⛔ THE MUTANT IS FULLY HERMETIC, AND GETTING THERE TOOK TWO TRIES. First cut put it under mktemp alone and it
# failed to source lib_release_guard.sh (s4e_msg.sh resolves siblings from its OWN directory), so arm (F) went red
# for a reason that had nothing to do with the guard -- a fixture defect wearing a real finding's clothes. The
# obvious repair, writing it into scripts/ under a dot-name, WORKS AND IS STILL WRONG HERE: this gate runs inside
# make test on a tree sixteen seats are committing from, and a stray file in the working tree is one `git add -A`
# away from being committed by somebody else. So the mutant and the ONE sibling it sources are copied into the
# scratch dir together, and nothing is ever written into the repo.
MUT="$W/s4e_mutant.sh"
cp "$HERE/lib_release_guard.sh" "$W/" 2>/dev/null || { echo "⛔ REFUSED: cannot copy lib_release_guard.sh -- the mutant could not source it"; exit 2; }
n=$(grep -c '^             exit 2; }$' "$MSG")
[ "$n" = 1 ] || { echo "⛔ REFUSED: expected exactly 1 guard exit line to neuter, found $n -- the guard's shape moved and this gate is blind"; exit 2; }
sed 's/^             exit 2; }$/             return 0; }/' "$MSG" > "$MUT"; chmod +x "$MUT"
if cmp -s "$MSG" "$MUT"; then echo "⛔ REFUSED: mutation changed nothing -- this gate is blind"; exit 2; fi
mk_po CEO || { echo "⛔ REFUSED: fixture build failed"; exit 2; }
out="$(run_next hq_B "$MUT")"; rc=$?
if claimed; then echo "ok  (F) FAIL-ONCE: with the guard neutered, hq_B under CEO dispatches again"
else echo "⛔ (F) FAIL-ONCE: mutant did NOT dispatch (rc=$rc) -- this gate cannot go red, so it proves nothing"
     echo "$out" | sed 's/^/      /'; fail=1; fi
[ "$fail" -eq 0 ] && { echo "✅ GATE OK: next refuses a stood-down identity (rc=2, naming mode + file, no lock taken), never the ceo, and the guard is proven removable."; exit 0; }
echo "⛔ GATE FAIL"; exit 1
