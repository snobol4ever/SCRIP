#!/usr/bin/env bash
# test_gate_package_runners_print_the_inventory.sh — the invariant gate for THE PACKAGE LOCKDOWN's
# instrument (row every-package-runner-prints-shipped-graded-ungraded-and-ungradable-and-the-leaderboard-
# carries-the-inventory, hq_T 2026-09-06, rank 0 on the ceo's order; Lon: "Fix the never graded business.
# Let's lock down our package testing and make it complete.").
#
# WHAT IT PROTECTS. Every lockdown row's DONE-WHEN is `ungraded=0`, so the number that criterion reads is
# load-bearing for the whole fleet. ⛔ THE FAILURE IT GUARDS AGAINST IS NOT A WRONG COUNT — it is a
# program that lands in NO bucket at all and therefore never appears in any number. Measured on the first
# run against a real package: icon/ipl ships 852 .icn files, 211 are declared in UNGRADED.tsv, and 641
# were in no bucket whatsoever. Not red, not skipped, not owed — simply absent, which is exactly the
# "never graded business" the order exists to end.
#
# ⭐⭐ SO THE LOAD-BEARING PROPERTY IS THE SUM, NOT ANY BUCKET. graded + ungraded + ungradable must equal
# shipped. Without that, the line is four independent opinions, each individually plausible, and the
# absent 641 are invisible in all four. A gate that only checked "does the runner print a line" would go
# green on an inventory that omits three quarters of the package.
#
# ⭐ AND THE SUM IS WHY `gap=` HAD TO SPLIT. Four runners printed `shipped/graded/gap`; `gap` held both
# "not graded YET" (work owed) and "CANNOT be graded" (a ruling). While an ungradable program sits inside
# gap, `ungraded=0` is UNREACHABLE BY BEING CORRECT — a lane does every piece of real work available and
# still never reaches its own criterion.
#
# Usage: bash scripts/test_gate_package_runners_print_the_inventory.sh
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
cd "$ROOT"
. "$HERE/lib_gate.sh"
gate_parse_args "$@"
gate_require "$HERE/lib_inventory.sh" "the one package-inventory body lib_inventory.sh"
. "$HERE/lib_inventory.sh"

violations=0; examined=0
TD="$(mktemp -d "${TMPDIR:-/tmp}/pkg_inv.XXXXXX")"; trap 'rm -rf "$TD"' EXIT

mkpkg() {  # mkpkg <n-files>; fresh scratch package, no sidecars
    rm -rf "$TD/p"; mkdir -p "$TD/p"
    local i; for i in $(seq 1 "$1"); do echo 'procedure main(); end' > "$TD/p/f$i.icn"; done
    INV_PACKAGE=scratch; INV_DIR="$TD/p"; INV_EXT=".icn"
}
run_inv() { inventory_line "$1" "$2" 2>&1; }

# ARM 1 — the happy path: buckets that sum produce exactly one line, in the one shape, rc=0.
mkpkg 5
printf 'f4.icn\tNEEDS_STDIN_FIXTURE\tstdin-driven filter, no .in authored yet\n' > "$TD/p/UNGRADED.tsv"
printf 'f5.icn\tORACLE_REFUSES\tthe oracle refuses this program: unsupported extension\n' > "$TD/p/UNGRADABLE.tsv"
examined=$((examined+1))
out="$(run_inv 3 0)"; rc=$?
want='PACKAGE_INVENTORY package=scratch shipped=5 graded=3 ungraded=1 ungradable=1 graded_stream=3 graded_narrow=0'
if [ "$rc" -ne 0 ] || [ "$out" != "$want" ]; then
    echo "GATE FAIL: the summing case did not print the one shape (rc=$rc)"; echo "    want: $want"; echo "    got : $out"
    violations=$((violations+1))
fi


# ⭐ ARM 1b — THE CLASS SPLIT, AND THE INVARIANT IS WHY THE ARM EXISTS (hq_T ruling 2026-09-06, on hq_I's
# ipl construction: the split moves into the shared body as its OWN line, and "a split that can disagree
# with its own total is worse than no split, because it looks like detail"). Two properties, both checked
# here: PACKAGE_INVENTORY stays EXACTLY ONE LINE and byte-identical (ARM 1 above already pins that, and
# this arm would have caught an implementation that printed the split from inventory_line instead), and
# the per-class counts sum to the very buckets inventory_line published.
examined=$((examined+1))
split_out="$(inventory_split_line 2>&1)"; split_rc=$?
inv_only="$(run_inv 3 0)"
if [ "$split_rc" -ne 0 ]; then
    echo "GATE FAIL: inventory_split_line refused a package whose inventory sums (rc=$split_rc): $split_out"; violations=$((violations+1))
elif [ "$inv_only" != "$want" ]; then
    echo "GATE FAIL: PACKAGE_INVENTORY is no longer exactly one byte-identical line once the split exists"; violations=$((violations+1))
else
    _u="$(printf '%s' "$split_out" | sed -n 's/.*ungraded_by_class=\([^ ]*\).*/\1/p')"
    _d="$(printf '%s' "$split_out" | sed -n 's/.*ungradable_by_class=\([^ ]*\).*/\1/p')"
    _su=$(printf '%s' "$_u" | tr ',' '\n' | awk -F: '{n+=$2} END{print n+0}')
    _sd=$(printf '%s' "$_d" | tr ',' '\n' | awk -F: '{n+=$2} END{print n+0}')
    # the scratch package above declares exactly one UNGRADED row and one UNGRADABLE row
    if [ "$_su" -ne 1 ] || [ "$_sd" -ne 1 ]; then
        echo "GATE FAIL: the split's per-class counts do not sum to the buckets inventory_line published (ungraded parts=$_su want 1, ungradable parts=$_sd want 1); split line: $split_out"; violations=$((violations+1))
    fi
fi
# ⚠ NOT PROVEN, and said so rather than claimed: _inv_class_hist's own rc=2 branch (the two readings of one
# population disagreeing) cannot be reached from a well-formed sidecar, because _inv_names refuses a
# malformed row before the histogram is ever counted. It is a defensive refusal against a future code
# change, not a behaviour this gate demonstrates.

# ARM 2 — ⭐⭐ THE LOAD-BEARING ARM. A program in NO bucket must REFUSE, and the refusal must name the
# delta. This is the 641-of-852 case, and a gate without it would pass an inventory that loses a package.
examined=$((examined+1))
out="$(run_inv 2 0)"; rc=$?
if [ "$rc" -ne 2 ] || ! printf '%s' "$out" | grep -q 'delta 1'; then
    echo "GATE FAIL: a shipped program in NO bucket was not refused with its delta named (rc=$rc): $out"
    violations=$((violations+1))
fi

# ARM 3 — over-counting refuses too: the sum is an identity, not a floor.
examined=$((examined+1))
out="$(run_inv 4 0)"; rc=$?
[ "$rc" -eq 2 ] || { echo "GATE FAIL: graded overshooting shipped was not refused (rc=$rc): $out"; violations=$((violations+1)); }

# ARM 4 — a declaration naming nothing shipped is STALE and refuses.
mkpkg 3; printf 'ghost.icn\tNEEDS_STDIN_FIXTURE\tnames no shipped file\n' > "$TD/p/UNGRADED.tsv"
examined=$((examined+1))
out="$(run_inv 2 0)"; rc=$?
if [ "$rc" -ne 2 ] || ! printf '%s' "$out" | grep -q 'declared but not shipped'; then
    echo "GATE FAIL: a stale declaration was not refused (rc=$rc): $out"; violations=$((violations+1))
fi

# ARM 5 — a program declared BOTH owed and impossible refuses: contradictory claims about one program.
mkpkg 3
printf 'f1.icn\tNEEDS_STDIN_FIXTURE\towed\n' > "$TD/p/UNGRADED.tsv"; printf 'f1.icn\tORACLE_REFUSES\timpossible\n' > "$TD/p/UNGRADABLE.tsv"
examined=$((examined+1))
out="$(run_inv 2 0)"; rc=$?
if [ "$rc" -ne 2 ] || ! printf '%s' "$out" | grep -q 'BOTH'; then
    echo "GATE FAIL: a name in both sidecars was not refused (rc=$rc): $out"; violations=$((violations+1))
fi

# ARM 6 — a declaration with NO REASON refuses. An inventory whose rows carry no reason is a number
# nobody can act on: the same failure one size down.
mkpkg 3; printf 'f1.icn\tNEEDS_STDIN_FIXTURE\t\n' > "$TD/p/UNGRADED.tsv"; rm -f "$TD/p/UNGRADABLE.tsv"
examined=$((examined+1))
out="$(run_inv 2 0)"; rc=$?
[ "$rc" -eq 2 ] || { echo "GATE FAIL: an unexplained declaration was not refused (rc=$rc): $out"; violations=$((violations+1)); }

# ARM 7 — ⛔ AN UNEXPLAINED NARROW BUCKET IS JUST A SMALLER LIE (hq_P). Claiming graded_narrow with no
# NARROW.tsv, or with a NARROW.tsv of the wrong size, must refuse: the count and the explanation are one
# population or the bucket is unfalsifiable.
mkpkg 3; rm -f "$TD/p"/*.tsv
examined=$((examined+1))
out="$(run_inv 2 1)"; rc=$?
if [ "$rc" -ne 2 ] || ! printf '%s' "$out" | grep -q 'NARROW.tsv'; then
    echo "GATE FAIL: graded_narrow claimed with no NARROW.tsv was not refused (rc=$rc): $out"; violations=$((violations+1))
fi
examined=$((examined+1))
printf 'f1.icn\tERRNO\terror compared by number only\nf2.icn\tERRNO\tsame\n' > "$TD/p/NARROW.tsv"
out="$(run_inv 2 1)"; rc=$?
if [ "$rc" -ne 2 ] || ! printf '%s' "$out" | grep -q 'same population'; then
    echo "GATE FAIL: graded_narrow disagreeing with NARROW.tsv's size was not refused (rc=$rc): $out"; violations=$((violations+1))
fi
examined=$((examined+1))
printf 'f1.icn\tERRNO\terror compared by number only\n' > "$TD/p/NARROW.tsv"
out="$(run_inv 2 1)"; rc=$?
if [ "$rc" -ne 0 ] || ! printf '%s' "$out" | grep -q 'graded_narrow=1'; then
    echo "GATE FAIL: a correctly explained narrow bucket did not pass (rc=$rc): $out"; violations=$((violations+1))
fi

# ARM 8 — ⛔⭐⭐ UNGRADABLE MUST NAME THE ORACLE'S REASON, NEVER OURS. hq_C measured the whole failure on
# prolog/swi_tests: EXCLUDED.md names 240 programs and 240 OF 240 give a SCRIP-side reason, zero give an
# oracle-side one. A program excluded because our own compiler fails it is A RED MOVED OUT OF THE
# DENOMINATOR -- the score cannot fall when we fail, because failing is what removes the entry, and the
# lockdown criterion becomes satisfiable BY failing. ⭐ This is an arm rather than a review note because
# every one of those 240 entries is individually honest and precisely documented: 240 true notes compose
# into a denominator that cannot fall, so entry-level diligence is exactly what hides it.
mkpkg 3; rm -f "$TD/p"/*.tsv
printf 'f3.icn\tORACLE_REFUSES\tscrip produces zero PASS/FAIL lines for this file today\n' > "$TD/p/UNGRADABLE.tsv"
examined=$((examined+1))
out="$(run_inv 2 0)"; rc=$?
if [ "$rc" -ne 2 ] || ! printf '%s' "$out" | grep -q 'OUR OWN COMPILER'; then
    echo "GATE FAIL: a scrip-side UNGRADABLE reason was not refused (rc=$rc): $out"; violations=$((violations+1))
fi
# and the control: an ORACLE-side reason for the same program is accepted, so the arm grades the REASON
# rather than merely rejecting the bucket.
examined=$((examined+1))
printf 'f3.icn\tNO_ORACLE_SHIPPED\tupstream ships no expected output for this program\n' > "$TD/p/UNGRADABLE.tsv"
out="$(run_inv 2 0)"; rc=$?
if [ "$rc" -ne 0 ]; then
    echo "GATE FAIL: an oracle-side UNGRADABLE reason was rejected (rc=$rc): $out"; violations=$((violations+1))
fi

# ARM 9 — an empty shipped population REFUSES rather than printing a clean zero. A percent over zero is
# not a score, and "0 ungraded" over 0 shipped would satisfy every lockdown criterion instantly.
rm -rf "$TD/p"; mkdir -p "$TD/p"; INV_DIR="$TD/p"
examined=$((examined+1))
out="$(run_inv 0 0)"; rc=$?
[ "$rc" -eq 2 ] || { echo "GATE FAIL: an empty package was not refused (rc=$rc): $out"; violations=$((violations+1)); }

# ARM 10 — a moved package REFUSES rather than reinterpreting.
INV_DIR="$TD/does-not-exist"
examined=$((examined+1))
out="$(run_inv 0 0)"; rc=$?
[ "$rc" -eq 2 ] || { echo "GATE FAIL: a missing package dir was not refused (rc=$rc): $out"; violations=$((violations+1)); }

# ARM 12 — ⛔⭐⭐ THE DECLARATION MATCHER MUST BE DETERMINISTIC, and it was not. Measured 2026-09-06 on
# icon/ipl: five runs of identical code over an identical tree reported 127, 137, 139, 125 and 138
# "declared but not shipped" programs, EVERY ONE OF THEM FALSE. The matcher was
# `printf '%s\n' "${found[@]}" | grep -qxF "$nm"`, and every package runner sets `pipefail`; `grep -q`
# exits at the first match, SIGPIPEs the printf, and the pipeline reports 141. Whether printf finishes
# first is a RACE, so the answer depended on scheduling.
# ⭐⭐ WHY THIS OUTRANKS THE WRONG COUNT: this is the arm that catches a STALE declaration — a ruling
# that no longer names a file. Under the race it accused 211 live files at once, so a real stale entry
# was indistinguishable from the noise, and the honest response to the refusal was to disbelieve it.
# ⛔ AND IT COULD NOT BE SEEN AT FIXTURE SCALE: with three files printf always wins, so every arm above
# passed on the day it landed. hq_I's rule, from the arizona determinism case the same morning: A CHECK
# THAT CANNOT FAIL FOR THE REASON YOU ARE ASKING ABOUT IS NOT A WEAK CHECK. ⭐ This arm was itself written
# wrong once and caught by that rule: at 600 short names it passed against the KNOWN-BAD body, because the
# defect needs the pipe to actually fill. Five INDEPENDENT runs are compared against each other — never a
# back-to-back pair inside one process, hq_I's other measured warning from the same morning.
# ⛔⭐ THE WITNESS HAS TWO INGREDIENTS AND NEITHER IS OPTIONAL: the name list must EXCEED THE 64K PIPE
# BUFFER (800 long names = 72000 bytes) and the declared name must match EARLY, so `grep -q` exits while
# `printf` is still writing. With three short names printf's whole output fits in one buffer, it never
# blocks, EPIPE never happens, and the arm silently grades nothing — which is why eleven arms passed over
# the defect on the day it landed. Measured 2026-09-06: 5/5 false refusals pre-cure, 5/5 clean post-cure.
rm -rf "$TD/big"; mkdir -p "$TD/big/sub"
_n='a_program_name_long_enough_that_eight_hundred_of_them_exceed_the_64k_pipe_buffer'
for i in $(seq -w 1 600); do echo 'procedure main(); end' > "$TD/big/${_n}_$i.icn"; done
for i in $(seq -w 1 200); do echo 'procedure main(); end' > "$TD/big/sub/${_n}_s$i.icn"; done
_first="$(basename "$(find "$TD/big" -type f -name '*.icn' | head -1)")"
printf '%s\tNEEDS_STDIN_FIXTURE\tthe name grep -q matches first, so printf is still writing when the pipe closes\n' "$_first" > "$TD/big/UNGRADED.tsv"
printf 'sub/%s_s001.icn\tCONTAINER_OR_LIBRARY\tdeclared by package-relative path, in a subdirectory\n' "$_n" > "$TD/big/UNGRADABLE.tsv"
INV_PACKAGE=big; INV_DIR="$TD/big"; INV_EXT=".icn"
# ⛔⭐⭐ AND THE ARM ASSERTS AGREEMENT **AND** CORRECTNESS, because stability alone is satisfiable by
# being consistently wrong — which is exactly what the known-bad body does on this fixture: 5/5 IDENTICAL
# false refusals, so an agreement-only check goes green over the defect it was written for. (It is
# unstable at ipl's scale and stable here; both are the same bug, and only the correctness half sees both.)
# ⭐ This arm was written agreement-only first and caught by running it against the pre-cure body, which is
# the whole reason a new gate arm must be proven to fail once before it lands.
_want='PACKAGE_INVENTORY package=big shipped=800 graded=798 ungraded=1 ungradable=1 graded_stream=798 graded_narrow=0'
examined=$((examined+1))
first=""; unstable=0; wrong=0
for i in 1 2 3 4 5; do
    out="$(run_inv 798 0)"; rc=$?
    [ -n "$first" ] || first="$rc|$out"
    [ "$rc|$out" = "$first" ] || unstable=1
    { [ "$rc" -eq 0 ] && [ "$out" = "$_want" ]; } || wrong=1
done
[ "$unstable" -eq 0 ] || { echo "GATE FAIL: the declaration matcher is NONDETERMINISTIC over 800 files -- five runs did not agree"; violations=$((violations+1)); }
[ "$wrong" -eq 0 ]    || { echo "GATE FAIL: the declaration matcher is stably WRONG over 800 files -- $first"; violations=$((violations+1)); }

# ARM 13 — and the stable answer must be the RIGHT one. Two ways it was wrong at once: the race accused
# live files, and the census indexed BASENAMES, so ipl's 390 `procs/…`/`gprocs/…` declarations could
# never match anything and were reported missing wholesale even when printf won.
examined=$((examined+1))
out="$(run_inv 798 0)"; rc=$?
want='PACKAGE_INVENTORY package=big shipped=800 graded=798 ungraded=1 ungradable=1 graded_stream=798 graded_narrow=0'
if [ "$rc" -ne 0 ] || [ "$out" != "$want" ]; then
    echo "GATE FAIL: a package-relative declaration did not resolve (rc=$rc): $out"; violations=$((violations+1))
fi

# ARM 14 — ⛔ A BARE NAME IS ONLY A NAME WHILE IT IS UNIQUE. ipl ships four collisions across procs/ and
# progs/ (gener, morse, repeats, spokes); a bare `gener.icn` ruling would land on whichever the census
# reached first, which is the ambiguity the race above made invisible. Refuse and ask for the path.
rm -rf "$TD/dup"; mkdir -p "$TD/dup/a" "$TD/dup/b"
echo x > "$TD/dup/a/same.icn"; echo x > "$TD/dup/b/same.icn"; echo x > "$TD/dup/only.icn"
printf 'same.icn\tCONTAINER_OR_LIBRARY\ta bare name that matches two shipped files\n' > "$TD/dup/UNGRADABLE.tsv"
INV_PACKAGE=dup; INV_DIR="$TD/dup"; INV_EXT=".icn"
examined=$((examined+1))
out="$(run_inv 2 0)"; rc=$?
if [ "$rc" -ne 2 ] || ! printf '%s' "$out" | grep -q 'BARE NAME'; then
    echo "GATE FAIL: an ambiguous bare declaration was not refused (rc=$rc): $out"; violations=$((violations+1))
fi

# ARM 15 — ⛔⭐ OUR OWN GENERATED MASTER IS NOT A SHIPPED PROGRAM. Four packages carry an `ALL.<ext>`
# beside the vendor's files. Counting it inflates `shipped` by one and makes the SUM unreachable by one
# forever. ⭐ It was also the entire 851-vs-852 disagreement between hq_I's Icon runner and this body —
# two correct instruments answering different questions, neither able to say which.
rm -rf "$TD/gen"; mkdir -p "$TD/gen"
echo x > "$TD/gen/v1.icn"; echo x > "$TD/gen/v2.icn"; echo x > "$TD/gen/ALL.icn"
INV_PACKAGE=gen; INV_DIR="$TD/gen"; INV_EXT=".icn"
examined=$((examined+1))
out="$(run_inv 2 0)"; rc=$?
if [ "$rc" -ne 0 ] || ! printf '%s' "$out" | grep -q 'shipped=2 '; then
    echo "GATE FAIL: the generated ALL.icn master was counted as a shipped program (rc=$rc): $out"; violations=$((violations+1))
fi

# ARM 16 — THE VOCABULARY IS CLOSED (hq_T ruling 2026-09-06). An unknown class refuses; a legacy spelling
# is accepted with a warning so no lane's board breaks on the ruling and the debt stays countable.
mkpkg 3; rm -f "$TD/p"/*.tsv
printf 'f3.icn\tHANDWAVE\ta class nobody ruled\n' > "$TD/p/UNGRADED.tsv"
examined=$((examined+1))
out="$(run_inv 2 0)"; rc=$?
if [ "$rc" -ne 2 ] || ! printf '%s' "$out" | grep -q 'vocabulary is CLOSED'; then
    echo "GATE FAIL: an unruled reason code was accepted (rc=$rc): $out"; violations=$((violations+1))
fi
examined=$((examined+1))
printf 'f3.icn\tEMPTY\tthe pre-ruling spelling, still in 106 ipl rows\n' > "$TD/p/UNGRADED.tsv"
out="$(run_inv 2 0)"; rc=$?
if [ "$rc" -ne 0 ] || ! printf '%s' "$out" | grep -q 'LEGACY CLASS'; then
    echo "GATE FAIL: a legacy class did not warn-and-pass (rc=$rc): $out"; violations=$((violations+1))
fi
# ⛔ and the two vocabularies do not share members: UNGRADABLE names what the ORACLE did, UNGRADED names
# what is OWED, so a class may not drift between the files.
mkpkg 3; rm -f "$TD/p"/*.tsv
printf 'f3.icn\tNEEDS_STDIN_FIXTURE\ta task, filed as a ruling\n' > "$TD/p/UNGRADABLE.tsv"
examined=$((examined+1))
out="$(run_inv 2 0)"; rc=$?
if [ "$rc" -ne 2 ]; then
    echo "GATE FAIL: an UNGRADED class was accepted in UNGRADABLE (rc=$rc): $out"; violations=$((violations+1))
fi

# ARM 11 — THE CENSUS, WITH ITS DENOMINATOR PRINTED. Every package runner must reach the shared body.
# ⚠ REPORTED, NOT COUNTED, WHILE THE ROW RAMPS: the instrument landed before its callers, so naming them
# is the work list, not a verdict. ⭐ It prints the denominator rather than a boolean for the reason the
# stale-binary gate's census arm does: that arm caught THIS author shipping a gate with no freshness
# guard on the day it landed (gates=97 wired=96 uncovered=1), which a boolean could not have done.
# ⛔⭐⭐ AND IT COUNTS WHAT IT CAN PROVE, IN THREE TIERS, BECAUSE "WIRED" WAS A CLAIM THIS GATE COULD NOT
# SUPPORT (hq_I, 2026-09-06, and they were right). It greps for the stanza, so it counted runners that
# COULD emit a line, not runners that DO -- and those differed for EVERY runner wired so far. jcon is the
# worked case: it sourced the body and called it, the census said wired=1, and it could not emit a line at
# all, because its UNGRADED.tsv was 2-column and its call SWALLOWED the refusal into a warning. A number
# that is wrong for every member of its population is not a census, and the ceo has since made this report
# load-bearing under Lon's order, which is the worst moment to be carrying an unprovable claim.
# ⭐ STATIC ANALYSIS CANNOT PROVE EMISSION -- only running the runner does, and that is board_packages.sh's
# job, not a 2-second mktemp-only gate's. So the honest move is not a cleverer grep: it is to stop printing
# one number, print the three that ARE decidable from the source, and name the gap the gate cannot close.
runners=0; sources=0; calls=0; swallows=0; missing=""; partial=""; swallowed=""; notpkg=""
for r in "$HERE"/test_*_suite.sh "$HERE"/raku_roast_scoreboard.sh "$HERE"/board_packages.sh; do
    [ -f "$r" ] || continue
    b="$(basename "$r")"
    # ⛔⭐ AN AGGREGATOR IS NOT AN EMITTER, and this census called one broken within minutes of landing.
    # board_packages.sh READS other runners' lines; it sets no stanza and emits none, correctly. Judged by
    # the emitter's checklist it read as "sources the body with an INCOMPLETE STANZA -- wired to REFUSE",
    # which is a false accusation against the one caller doing exactly what it should. ⭐ Same shape as the
    # defect this arm was rewritten to cure one commit earlier: a census that counts the wrong population
    # is wrong for every member of it, and it is loudest about the members that are most correct.
    case "$b" in board_*.sh) continue ;; esac
    # ⛔⭐ A CONSTRUCT-LADDER/STRESS RUNNER OVER OUR OWN CORPUS IS NOT A VENDORED-PACKAGE RUNNER EITHER
    # (seat12, 2026-09-06). This row's population is "every program a VENDORED PACKAGE SHIPS" (the task's
    # own GOAL text, row every-package-runner-prints-...) -- a ladder/stress suite over corpus/crosscheck
    # or a hand-built rung corpus has no vendor and nothing "shipped" to name shipped=/ungradable= against;
    # forcing one to grow a package-inventory line is the same category error CLAUDE.md names for running
    # an ALL.<ext> master whole ("both are artifacts of the wrong invocation, never findings"). Verified
    # structurally, not asserted: EVERY runner wired onto lib_inventory.sh (12/12 measured this sitting)
    # references corpus/packages/ literally; every runner that does not was individually read (not just
    # grepped) and confirmed to be a construct-ladder/stress suite: test_gc_stress_suite.sh (corpus/
    # crosscheck/gc/ GC torture matrix), test_icon_rung_suite.sh / test_prolog_rung_suite.sh (rung ladders,
    # GOAL-ICON-BB/GOAL-PROLOG-BB), test_snobol4_pat_rung_suite.sh (SNOBOL4 pattern rung suite). NAMED, not
    # silently shrunk -- see the report line below; a new runner earns inclusion by shipping a vendored
    # package, not by matching a glob.
    # ⛔ THE PATTERN MATCHES THE PATH SHAPE, NOT THE LITERAL STRING "corpus/packages" -- caught live:
    # test_csnobol4_budne_suite.sh builds its path as `$CORPUS/packages/snobol4/csnobol4_suite`, so the
    # literal substring "corpus/packages" never appears in the file and a naive grep misclassified a REAL
    # vendored-package runner (deliberately unwired for its own, different, already-ruled reason -- a
    # stale mode-2 (--run) duplicate of test_snobol4_csnobol4_suite.sh, modes 1/2 DELETED per CLAUDE.md,
    # see board_packages.sh's own comment) as if no vendor shipped it at all. /packages/<name>/ matches
    # both the literal and the variable-built form and was re-verified to still exclude all nine genuine
    # construct-ladder/stress runners (seat12, 2026-09-06).
    grep -qE '/packages/[a-z0-9_]+/' "$r" || { notpkg="$notpkg $b"; continue; }
    runners=$((runners+1))
    grep -q 'lib_inventory.sh' "$r" || { missing="$missing $b"; continue; }
    sources=$((sources+1))
    # a COMPLETE stanza: all three tokens plus the call. A missing INV_EXT refuses at run time, so a
    # runner that sources the body and sets two of three is wired to refuse, not wired to report.
    if grep -q 'inventory_line' "$r" && grep -q 'INV_PACKAGE=' "$r" && grep -q 'INV_DIR=' "$r" && grep -q 'INV_EXT=' "$r"; then
        calls=$((calls+1))
    else
        partial="$partial $b"; continue
    fi
    # ⛔ AND A SWALLOWED REFUSAL IS THE jcon CASE EXACTLY: `inventory_line ... || echo "warn"` turns rc=2
    # into a line nobody reads, so a package can refuse on every run while the board stays quiet.
    grep -qE 'inventory_line[^|&]*(\|\||&&|2>/dev/null)' "$r" && { swallows=$((swallows+1)); swallowed="$swallowed $b"; }
done
echo "    package runners censused=$runners  sources=$sources  complete-stanza=$calls  unwired=$((runners-sources))  [aggregators excluded: they read lines, never emit one]"
echo "    ⚠ NOT DECIDABLE HERE: whether a runner actually EMITS a summing line. Only running it proves that (board_packages.sh)."
[ -z "$notpkg" ]    || { echo "    NOT A VENDORED-PACKAGE RUNNER (excluded from this population -- no /packages/<name>/ path, nothing a vendor ships, never wire):"; printf '      %s\n' $notpkg; }
[ -z "$missing" ]   || { echo "    NOT YET WIRED (the row's work list, not a verdict):"; printf '      %s\n' $missing; }
[ -z "$partial" ]   || { echo "    ⛔ SOURCES THE BODY WITH AN INCOMPLETE STANZA -- wired to REFUSE, not to report:"; printf '      %s\n' $partial; }
[ -z "$swallowed" ] || { echo "    ⛔ SWALLOWS THE REFUSAL (rc=2 becomes a warning nobody reads -- the jcon case):"; printf '      %s\n' $swallowed; }

GATE_EXAMINED="$examined arms"
gate_verdict "$violations" "package-inventory violations"
