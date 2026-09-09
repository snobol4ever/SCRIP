#!/usr/bin/env bash
# scripts/test_gate_master_sidecars_cover_stdin_and_argv.sh -- EVERY MASTER ENTRY THAT READS STDIN OR TAKES
# ARGUMENTS DECLARES THEM IN A SIDECAR, AND THE NUMBER THAT DO NOT ONLY EVER GOES DOWN.
# AUTHORS: Lon Jones Cherryholmes · Claude Opus 5   DATE: 2026-09-08
#
# Lon 2026-09-08, in-chat to hq_T, verbatim: "Ensure all has sister (side cars) files for the ONE-LINERS and
# MULTI-LINERS such that they have command-line args and an input file for each test along with the ref and
# source files."
#
# ⛔⭐⭐ THE DEFECT THIS CLOSES DOES NOT ANNOUNCE ITSELF, AND WE HAVE A MEASURED SPECIMEN. An entry that reads
# stdin but is graded against /dev/null does not crash and does not error: it takes EOF, prints whatever a
# starved run prints, and the mint records THAT as its .ref. From then on it passes forever, in both modes,
# while executing none of the behaviour it was written to test. corpus 6c94504c0 is one such entry cured --
# icon's 831 (rung36_jcon_recogn), whose input sat in tests/icon/config/ where loose_stdin_companion() did
# not look until SCRIP bc9812abe. Fed and re-cut from iconx, the Icon master went from a false 704/704 to an
# honest 703/704, and the red it exposed is a real oracle-confirmed engine defect that three sittings had
# already misattributed to a gate. ONE starved entry hid ONE engine bug for three sittings; this gate exists
# because the census then found 60 more of the same shape.
# ⭐ THE ARGV HALF IS THE SAME DEFECT WEARING A QUIETER FACE: `n := integer(args[1]) | 6` runs GREEN down the
# `| 6` default, so the board reports a pass for a program whose argument handling never ran. The harness has
# had complete argv-sidecar support since 2026-09-06 and NOT ONE master has ever carried an ALL.argv, so all
# 34 such entries are graded along their no-arguments path.
#
# ⛔⭐ A RATCHET, NOT A FAIL=0 BAR, AND THE CHOICE IS THE POINT. 95 entries owe a declaration today (61 stdin,
# 34 argv) and each one needs an AUTHORED input plus a ref re-cut from that language's oracle -- that is a
# campaign, not a landing. A FAIL=0 gate here would be red for every seat until the campaign finished, and an
# ignored gate is worse than no gate (hq_P's standing ruling on the retired optbypass watermark). So the bar
# is: THE OWED COUNT MAY NOT GROW. That is green on arrival, which is why it can be BLOCKING from the start,
# and it buys the thing actually worth having tonight -- nobody can add a 96th stdin-reading or argv-taking
# entry to any master without also declaring its input, which is precisely how the first 95 accumulated.
# ⭐ RE-PIN DOWNWARD, NEVER UPWARD. The gate itself prints the new floor when the count falls, and a commit
# that lowers it re-pins in the same push. Raising a floor to make this gate pass is the one edit that voids
# it -- if a change genuinely needs a new undeclared entry, that is a ruling, not a number to bump.
#
# ⛔ THE CENSUS IS A WORK LIST, NEVER A PER-PROGRAM VERDICT. util_master_sidecar_census.py finds these by
# SOURCE CONSTRUCT, which is a heuristic: it can name an entry that says INPUT in a comment, and it cannot
# see a program reaching stdin by a route its table does not list. So this gate grades the COUNT, and the
# verdict on any individual entry belongs to the oracle once that entry is fed. It writes no sidecar and
# never invents input -- inventing plausible input is how you manufacture the self-pinned ref this exists
# to find.
#
# EXIT: 0 the owed count is at or below the pinned floor · 1 it GREW (or a floor needs lowering and did not)
#       · 2 REFUSED -- cannot measure (no corpus, no master, census returned an empty denominator).
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"; cd "$ROOT"
. "$HERE/lib_gate.sh"
GATE_NAME="test_gate_master_sidecars_cover_stdin_and_argv"
gate_parse_args "$@"

# ⛔ THE PINNED FLOORS -- measured 2026-09-08 by hq_T at corpus 6c94504c0 over 5086 entries in 7 masters.
# Lower them in the commit that earns it; never raise them.
# 61 -> 53 at corpus 687132c48: eight snobol4 entries fed their recovered real input (crosscheck word1-4,
# cross, triplet, fileinfo, expr_eval), refs re-cut from sbl -bf. Zero changed verdict -- SCRIP was already
# right on all eight -- but each had been passing an EMPTY ref produced by a starved run.
PIN_STDIN="${PIN_STDIN:-53}"
PIN_ARGV="${PIN_ARGV:-34}"

CENSUS="$HERE/util_master_sidecar_census.py"
[ -f "$CENSUS" ] || { echo "GATE UNPROVEN(2) [$GATE_NAME]: no census tool at $CENSUS"; gate_stamp; exit 2; }

# ⭐ TESTS_DIR EXISTS SO THIS GATE CAN BE PROVEN RED WITHOUT REDDENING THE REAL CORPUS -- the fail-once arm
# points it at a scratch copy carrying one deliberately undeclared entry. Unset, it censuses the real tree.
CENSUS_ARGS=()
[ -n "${TESTS_DIR:-}" ] && CENSUS_ARGS+=(--tests-dir "$TESTS_DIR")
# ⛔ CAPTURE FIRST, THEN TEST -- never `cmd | head; echo $?`, which reports the pager's status and not the
# census's (CLAUDE.md's own measured trap; it printed rc=0 for a command that exits 1).
out=$(python3 "$CENSUS" ${CENSUS_ARGS[@]+"${CENSUS_ARGS[@]}"} --json 2>&1); rc=$?
if [ "$rc" -ne 0 ]; then
  echo "GATE UNPROVEN(2) [$GATE_NAME]: the census could not measure -- ${out}"; gate_stamp; exit 2
fi

read -r examined n_stdin n_argv <<<"$(printf '%s' "$out" | python3 -c '
import json,sys
d=json.load(sys.stdin)
r=d["rows"]
print(d["examined"], sum(1 for x in r if x["owes_stdin"]), sum(1 for x in r if x["owes_argv"]))
')" || { echo "GATE UNPROVEN(2) [$GATE_NAME]: could not parse the census JSON"; gate_stamp; exit 2; }

case "$examined" in ''|*[!0-9]*) echo "GATE UNPROVEN(2) [$GATE_NAME]: census printed no examined count"; gate_stamp; exit 2;; esac
[ "$examined" -gt 0 ] || { echo "GATE UNPROVEN(2) [$GATE_NAME]: censused ZERO entries -- an empty denominator is not a clean census"; gate_stamp; exit 2; }
GATE_EXAMINED="$examined master entries"

echo "SIDECAR DEBT: stdin=$n_stdin (floor $PIN_STDIN) · argv=$n_argv (floor $PIN_ARGV) · examined $examined entries in 7 masters"
bad=0
if [ "$n_stdin" -gt "$PIN_STDIN" ]; then
  echo "⛔ GREW: $n_stdin entries read stdin with no ALL.in block, above the pinned $PIN_STDIN."
  echo "   A new entry that reads stdin must ship its input in the same commit -- unfed, its ref records the"
  echo "   STARVED run and it passes forever while testing nothing (corpus 6c94504c0 is that bug, cured)."
  bad=$((bad+1))
fi
if [ "$n_argv" -gt "$PIN_ARGV" ]; then
  echo "⛔ GREW: $n_argv entries take program arguments with no ALL.argv declaration, above the pinned $PIN_ARGV."
  echo "   Declare them: one 'name<TAB>arg<TAB>arg' line in the master's ALL.argv (read_argv_sidecar)."
  bad=$((bad+1))
fi
if [ "$n_stdin" -lt "$PIN_STDIN" ] || [ "$n_argv" -lt "$PIN_ARGV" ]; then
  echo "⭐ DEBT FELL — re-pin in this commit: PIN_STDIN=$n_stdin PIN_ARGV=$n_argv"
  echo "   (a floor is only ever lowered; the names are in: python3 scripts/util_master_sidecar_census.py)"
fi
gate_verdict "$bad" "sidecar debt counts above their pinned floor"
