#!/usr/bin/env bash
# board_icon_master.sh — THE SET-LEVEL BOARD OVER corpus/tests/icon/ALL.icn (row icon-all-master-board).
# Authors: LCherryholmes · Claude Opus 5   DATE: 2026-08-30
#
# ⛔⭐⭐ WHY THIS EXISTS: UNTIL TODAY, ICON'S COVERAGE SAFETY RESTED ON A NAMING CONVENTION.
# `test_icon_all_rungs.sh` and its kin grade the master suite FAMILY BY FAMILY, by name (`SUITE
# rung30_builtins_misc: pass=5`, ...). That covers whatever families those scripts happen to name —
# and NOTHING checks that the named families still add up to the whole set. An entry whose family is
# renamed, or a family nobody re-points a grader at, simply stops being graded, and every board stays
# green because no board ever knew the denominator. MEASURED at mint: ALL.csv carries **534** entries;
# the aggregate rung grader names a strict subset. That gap is the same absorbed-population class that
# hid NINE gradings in the Prolog rungs and 143 more behind rc=2 refusals — this board closes the door
# on the Icon side by grading the SET and printing its own denominator.
#
# ⛔ THIS IS A WATERMARK CENSUS BOARD, NOT A FAIL=0 GATE, AND THAT IS DELIBERATE.
# MEASURED at mint (pristine -O0, both modes): total=534 · m3 pass=393 fail=122 crash=16 hang=2 ·
# m4 pass=393 fail=122 crash=1 hang=2 skip=15.  RE-PINNED the same day to m3/m4 pass=398 (fail=120,
# m3 crash=13, m4 skip=12) after Icon cures landed from other seats -- and the re-pin happened because
# THIS BOARD'S OWN "WATERMARK MOVED UP" ARM SAID SO on the first re-run against a moved tree, which is
# the arm earning its keep on day one. Icon is nowhere near green on the whole set, so a
# FAIL=0 bar here would be a gate nobody can satisfy — and hq_P's standing ruling on exactly that
# shape (the RETIRED optbypass watermark gate; lesson kept in .github/GOAL-TEST-SUITE-CONSISTENCY.md
# ignored gate is worse than no gate. So the verdict is a RATCHET: red only if the graded population
# SHRINKS or the pass counts REGRESS below the pinned watermark. Curing Icon is not this board's job;
# noticing that it moved is.
#
# ⛔ NOT WIRED INTO THE BLOCKING SET. `make test`'s membership is a ruling, not a script's to take —
# and a board that is red-by-construction must never be made blocking. Routed to ceo at mint.
#
# ⭐ FLOOR, NOT A PINNED TOTAL (RULES.md § the denominator law): growth needs no re-pin; only an
# ATTRIBUTED retirement may lower these, in the same commit that shrinks the master.
#
# ⛔⭐ THE AST-SHAPE COUNT IS INFORMATIONAL ONLY, NEVER A GATE (ast-dump-refs-are-self-pins-not-
# oracles, 2026-09-05): the parser-ladder fixtures' .ref is SCRIP's own past self-dumped AST, pinned
# against no external oracle -- no oracle emits SCRIP's AST shape, so that count can only ever answer
# "did the shape change since it was last decided", never "is it right". This board used to let that
# count set RED and feed a pass/fail floor on this SCORED board, which manufactures phantom defects
# out of drift the moment someone re-decides the shape. It is still measured and printed every run
# (Icon's parser-ladder population is NOT left ungated) but it no longer participates in this board's
# verdict, its floor, or the leaderboard's pass/fail framing -- a mismatch here means RE-DECIDE THE
# SHAPE AND REGENERATE the ref, never "N programs FAIL".
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME
set -euo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
# ⛔⭐ RECORD THE TREE THIS RUN GRADES, AT ITS START (hq_T 2026-09-08). A board takes ten to forty minutes and its
# SCORE.md write happens at the END; a seat who commits and pushes mid-run -- which the CEO-174 dirty-tree guard
# actively pushes you toward, since a dirty tree skips the write entirely -- moves HEAD under a measurement that
# already happened, and the row then names a tree carrying commits it never ran. Measured three times in one
# sitting, twice while doing the disciplined thing. util_score_row.py stamps this instead of HEAD, and says so
# when the two differ; unset, it behaves exactly as it always did.
export S4E_TREE_AT_START="SCRIP=$(git -C "$HERE/.." rev-parse --short HEAD 2>/dev/null),corpus=$(git -C "$HERE/../../corpus" rev-parse --short HEAD 2>/dev/null)"
SCRIP="${SCRIP:-$HERE/../scrip}"
CORPUS="${CORPUS:-$S4E/corpus/tests/icon}"
HARNESS="$HERE/corpus_suite_harness.py"
MASTER_ICN="$CORPUS/ALL.icn"
MASTER_REF="$CORPUS/ALL.ref"
MASTER_CSV="$CORPUS/ALL.csv"
ENTRY_FLOOR="${ICON_MASTER_ENTRY_FLOOR:-534}"
# ⛔⭐ THE PASS FLOORS ARE OVER THE RUN-GRADED POPULATION (381), NOT THE SUITE (534), and the old 398 is VOID.
# Until 2026-09-03 this board graded all 534 entries by RUNNING them, including the 153 parser-ladder fixtures
# whose .ref is a self-pinned AST-shape dump (SCRIP's own past output, see the AST-SHAPE note above) -- so its
# number mixed two populations graded against two different kinds
# of expected output and could not be read as anything. Re-pinned from the honest split measured on
# SCRIP 4f847224 / corpus 53477317: run-graded m3/m4 PASS=377 of 381, ast-graded PASS=153 of 153.
# ⭐ Note which way the old instrument moved when the corpus got BETTER: the ceo's re-cut of 30 stale AST pins
# (corpus 53477317) gave those fixtures correct dumps, which match RUN output even less, so the old board fell
# 398 -> 377. An instrument that gets worse as its subject improves is not miscalibrated, it is measuring
# something else.
# RE-PINNED 377 -> 379 (row icon-level-keyword-not-tracked, this same commit): &level's entry-side increment
# was landing but its &level READ (keywords.c) still returned the raw un-adjusted counter (kw_fnclevel's own
# "-1" convention never applied to Icon's "level" keyword string), and *&subject's word0 was a bare `mov`
# of the DT_S tag that clobbered slen along with it whenever built from the in-scan r13/r15 fast path
# (bb_keyword_icon.cpp) -- both cured this commit, procedure_alt_fail_replace_1 and procedure_scan_write_1
# move FAIL -> PASS in both modes; procedure_every_alt_replace_4 still shows &progname mismatch, which is
# the consolidation rename itself (correct compiler behavior), not a defect -- see the FINDING.
# ⭐ 380/381 (seat03, icon-master-six-run-graded-reds-cured, re-verified 2026-09-03): the master's last
# open "red" among the original six -- procedure_record_limit_replace_1 and procedure_record_every_replace_2
# -- were never compiler defects: both need companion fixture files (prepro.dat / fncs1.dat, staged from
# corpus/tests/icon/config/ by _copy_companions) that a bare `extract`+direct-invoke repro never supplies,
# so they read FAIL only when tested outside run_suite_entry's own methodology; under it (this board) both
# already PASS. The sixth, procedure_scan_while_1, WAS the master's one XFAIL (probe_witness__witness_icn_options_
# dash_branch): main(args) needed real argv, and corpus_suite_harness.py's suite format carries stdin/want-rc
# sidecars and NO argv sidecar. ⛔ RETIRED 2026-09-04 (hq_B, corpus 1520d35d1) under THERE IS NO XFAIL: the test
# was faulty FOR THIS HARNESS, so the test was fixed -- opt2(["-x"]) over a literal list, ref re-cut from icont,
# marker dropped, ALL.xfail deleted -- and the master reads XFAIL=0 with 596/596 both modes. A real argv-through-
# the-harness witness is still owed once the argv sidecar lands (hq_T's harness row); this comment is not it.
# ⭐ RE-PINNED 596 -> 607 (hq_B 2026-09-05 ~16:0x, MODE OCTET, on the ceo's order to re-pin the watermark).
# ⛔ THE ORDER SAID 601 AND 601 IS ALREADY STALE — pinned to the MEASURED number, not the relayed one.
# hq_P measured 601/601 both modes on SCRIP `7e190f16a`; corpus `bb5ee2b69` then minted the 8 declared-missing
# isolation-phase witnesses, so the run-graded population is 609, not 601. Measured here on SCRIP `b812fb6d1`
# corpus `8972babeb` RT_OPT=-O0, incremental make: entries=762 · m3 PASS=607 · m4 PASS=607 of 609.
# Pinning 601 would have sat SIX passes BELOW the tree and silently stopped protecting them — a watermark under
# the water is not a ratchet, and it fails in the direction nobody looks, because the board still prints OK.
# ⛔ THE TWO REDS ARE NAMED, NOT XFAILED (RULES.md § THERE IS NO XFAIL) and are NOT a regression: both are new
# entries (ALL.csv rows 761-762) minted by `bb5ee2b69` that have never passed —
# ladder_rung26_pow_pow_negbase_real and ladder_rung26_pow_pow_zero_negexp. They are open Icon defects to cure,
# which is why 607 and not 609: a floor is what today already holds, never what tomorrow owes.
M3_PASS_FLOOR="${ICON_MASTER_M3_PASS_FLOOR:-607}"
M4_PASS_FLOOR="${ICON_MASTER_M4_PASS_FLOOR:-607}"
# ⛔ NO AST_PASS_FLOOR: a self-pin has no floor to regress below, only a CURRENT-run comparison of
# ap (matched) vs at (total) -- see the AST-SHAPE note above. Removed under ast-dump-refs-are-self-
# pins-not-oracles rather than kept-but-unused, so a reader cannot mistake its presence for gating.

# ⛔ A BOARD THAT CANNOT MEASURE REFUSES rc=2 — never skip-as-success (RULES.md). Each arm below names
# what is missing, because "cannot enumerate" and "enumerated zero" are different facts and a single
# exit code for both is how a vanished corpus reads as a clean run.
if [ ! -x "$SCRIP" ];       then echo "⛔ BOARD REFUSES (rc=2): scrip not built at $SCRIP"; exit 2; fi
if [ ! -f "$HARNESS" ];     then echo "⛔ BOARD REFUSES (rc=2): corpus_suite_harness.py missing at $HARNESS"; exit 2; fi
if [ ! -f "$MASTER_ICN" ] || [ ! -f "$MASTER_REF" ]; then
    echo "⛔ BOARD REFUSES (rc=2): the Icon master suite is missing at $MASTER_ICN"
    echo "   Do NOT read a family-by-family rung grader's smaller total as this board — that substitution"
    echo "   is the exact failure this board was minted to make impossible. Pull corpus."
    exit 2
fi
if [ ! -f "$MASTER_CSV" ]; then echo "⛔ BOARD REFUSES (rc=2): ALL.csv missing at $MASTER_CSV — no per-entry provenance to attribute a shrink to"; exit 2; fi

CSV_ENTRIES=$(( $(wc -l < "$MASTER_CSV") - 1 ))
echo "=== Icon MASTER board — corpus/tests/icon/ALL.icn ==="
# ⛔ `|| true` IS LOAD-BEARING, NOT SLOPPINESS: the harness exits NON-ZERO whenever any entry fails, and
# this board is a census over a suite that is legitimately red today. Under `set -euo pipefail` the bare
# pipeline aborted the script after printing only its header — a board that dies silently on the very
# condition it exists to report. The refusal arms below are what distinguish "could not measure" from
# "measured a red suite"; the exit status of the harness never does.
# ⛔⭐⭐ STDERR IS CAPTURED, NOT DISCARDED -- IT IS WHERE THE FAILURE NAMES LIVE. This line read
# `2>/dev/null` and therefore threw away the only per-entry information the run produces: the harness
# DOES print `FAIL <mode> <entry>: <why>` rows in --by-modes-column mode, on stderr, and this board was
# deleting them before anyone could read them. hq_C measured the consequence from the other end
# (2026-09-03): the icon row accumulated THREE readings of one board in one day -- 377/381, 378/381, and
# a pinned floor of 379, on three different trees -- and none of them could be reconciled against the
# others, because a board that says "two of these regressed" without saying WHICH two produces numbers
# that can only ever be compared, never diffed. Their diagnosis was that the harness lacks a verbose
# flag in this mode; measured here, the harness was printing the rows all along and the BOARD was
# dropping them, which is a cheaper fix and a different file.
# ⛔ Kept in a SEPARATE stream, deliberately: folding stderr into $_raw would put arbitrary diagnostic
# text through the `grep '^SUITE_BOARD '` parses below, and a board that mis-parses its own verdict to
# gain a fail list has traded the number for the names rather than getting both.
_errf=$(mktemp); trap 'rm -f "$_errf"' EXIT
# ⛔⭐ THE OUTSIDE LIST RIDES ONLY IF IT EXISTS, and its absence is not an error: a suite with nothing outside
# its baseline has no file, exactly as a package without one does. When it IS there the harness REFUSES on a
# name it cannot find in the suite, so a stale list cannot quietly shrink the denominator (ceo ruling
# 2026-09-08: a master entry the oracle cannot run is outside the master's baseline, recorded with the
# oracle's own words and never masked per line).
OUTSIDE_TSV="$CORPUS/ALL.outside.tsv"
_outside_arg=""; [ -f "$OUTSIDE_TSV" ] && _outside_arg="--outside $OUTSIDE_TSV"
_raw=$(timeout 1800 python3 "$HARNESS" run "$MASTER_ICN" "$MASTER_REF" --lang icon --modes m3,m4 --by-modes-column $_outside_arg 2>"$_errf" || true)
# ⛔ THE OUTSIDE SET IS ECHOED, NEVER SWALLOWED. This board captures the harness into $_raw and prints only
# what it greps, so without this the entries dropped from the denominator would be INVISIBLE on the very
# board whose number they changed -- which is precisely the masking the ruling forbids. Printed before the
# counts, so a reader sees what left the population before reading the population.
printf '%s\n' "$_raw" | grep '^OUTSIDE_BASELINE' || true
board=$(printf '%s\n' "$_raw" | grep '^SUITE_BOARD ' | tail -1 || true)
astboard=$(printf '%s\n' "$_raw" | grep '^SUITE_BOARD_AST ' | tail -1 || true)
split=$(printf '%s\n' "$_raw" | grep '^MODES_COLUMN ' | tail -1 || true)
if [ -z "$astboard" ] || [ -z "$split" ]; then
    echo "⛔ BOARD REFUSES (rc=2): the harness printed no ast board / no MODES_COLUMN split."
    echo "   This board grades BY THE MASTER'S modes COLUMN; without that split the only number available"
    echo "   is the old one that ran the 153 self-pinned AST-shape fixtures and counted their inevitable reds."
    exit 2
fi
if [ -z "$board" ]; then
    echo "⛔ BOARD REFUSES (rc=2): harness produced no SUITE_BOARD line for the Icon master suite"
    echo "   Measured nothing. That is NOT a pass — see RULES.md: a test that cannot measure refuses."
    exit 2
fi
field() { echo "$board" | grep -oE "$1=[0-9]+" | cut -d= -f2; }
mt=$(field total)
m3p=$(field m3_pass); m3f=$(field m3_fail); m3c=$(field m3_crash); m3h=$(field m3_hang); m3u=$(field m3_unproven); m3x=$(field m3_xfail); m3xp=$(field m3_xpass)
m4p=$(field m4_pass); m4f=$(field m4_fail); m4c=$(field m4_crash); m4h=$(field m4_hang); m4u=$(field m4_unproven); m4s=$(field m4_skip); m4x=$(field m4_xfail); m4xp=$(field m4_xpass)
# ⛔⭐ THE AND PER PROGRAM (ceo-372, 2026-09-06): the number this board's leaderboard row states is the count of
# entries green in EVERY mode they were graded in. It is read from the harness, never re-derived here -- m3p and
# m4p cannot yield it, because the AND is a fact about each ENTRY and the board line has already thrown the
# entries away. That is precisely why this row published one mode until now.
# ⛔ REFUSE RATHER THAN READ A ZERO. An older harness prints no all_pass=, `field` returns empty, and a row
# written over an empty string is a plausible false number -- this board's whole failure class.
mall=$(field all_pass)
# ⛔⭐ THE XFAIL OUTCOME SPLIT (ceo CEO-432 item 2): xfail says EXPECTED RED and nothing about WHICH red, so a
# crash, a hang, a wrong answer and a compile refusal are four different repairs wearing one count -- and an entry
# that crashes in one mode and merely answers wrong in the other cannot show at all without a re-run.
xfsplit() { # $1=mode prefix -> "wrong=A crash=B hang=C unproven=D skip=E"; an absent bucket is not a zero
    local m="$1" k v out=""
    for k in wrong crash hang unproven skip; do
        v="$(field "${m}_xfail_${k}")"
        [ -n "$v" ] || { echo "⛔ BOARD REFUSES: SUITE_BOARD carries no ${m}_xfail_${k}= field -- the outcome split cannot be assembled (CEO-432 item 2)." >&2; exit 2; }
        out="${out}${out:+ }${k}=${v}"
    done
    printf '%s' "$out"
}
# ⛔ THE `|| exit 2` IS LOAD-BEARING: xfsplit runs inside a command substitution, so its own `exit 2`
# leaves only the SUBSHELL -- the refusal would print to stderr and the run would carry on with an
# empty split, publishing "xfail=25 ()" as though that were a reading. The assignment's status IS the
# substitution's status, so this is where the refusal actually crosses back out.
m3xs="$(xfsplit m3)" || exit 2
m4xs="$(xfsplit m4)" || exit 2
[ -n "$mall" ] || { echo "⛔ BOARD REFUSES: SUITE_BOARD carries no all_pass= field -- this row is the AND per program (ceo-372) and cannot be assembled without it; corpus_suite_harness.py beside this script emits it."; exit 2; }
# ⛔⭐ AND THE SAME REFUSAL FOR THE xfail/xpass FIELDS, because this row now PUBLISHES them (coo 2026-09-08): a
# headline that holds N constant while xfail and xpass trade underneath it is blind to movement inside its own
# known-red set -- a cured bug and a stale marker are both invisible in it, in opposite directions. `field` returns
# the EMPTY STRING for a field the harness never emitted, and "xpass=" in a cell is not a zero; it is a reading that
# was never taken, wearing the shape of one that was, and it would read as "no stale markers" to every consumer.
for _f in m3_xfail:m3x m3_xpass:m3xp m4_xfail:m4x m4_xpass:m4xp; do
    _fld="${_f%%:*}"; _var="${_f##*:}"
    [ -n "${!_var}" ] || { echo "⛔ BOARD REFUSES: SUITE_BOARD carries no ${_fld}= field -- this row publishes the xpass count beside its fraction and cannot assemble it; corpus_suite_harness.py beside this script emits it."; exit 2; }
done
if [ -z "$mt" ] || [ "$mt" -eq 0 ]; then
    echo "⛔ BOARD REFUSES (rc=2): the harness graded ZERO entries over a master file that exists"; exit 2
fi

astfield() { echo "$astboard" | grep -oE "$1=[0-9]+" | cut -d= -f2; }
at=$(astfield total); ap=$(astfield ast_pass); af=$(astfield ast_fail); ac=$(astfield ast_crash); ah=$(astfield ast_hang); axp=$(astfield ast_xpass)
if [ -z "$at" ] || [ "$at" -eq 0 ]; then
    echo "⛔ BOARD REFUSES (rc=2): the modes column declares an ast population but zero were graded"; exit 2
fi
graded=$(( mt + at ))
echo "entries=$graded  (run-graded $mt + ast-graded $at; ALL.csv rows=$CSV_ENTRIES, floor=$ENTRY_FLOOR)"
echo "$split"
echo "AST-shape drift check (self-pinned dump, parser-ladder fixtures, INFORMATIONAL -- never part of this board's verdict): $ap/$at match  DRIFTED=$af CRASH=$ac HANG=$ah XPASS=$axp"
echo "run-graded population: $mt entries (the ast fixtures are NOT in these two lines and are never summed into them)"
echo "mode-3 (--run):     PASS=$m3p FAIL=$m3f CRASH=$m3c HANG=$m3h UNPROVEN=$m3u XFAIL=$m3x ($m3xs) XPASS=$m3xp   / $mt"
echo "mode-4 (--compile): PASS=$m4p FAIL=$m4f CRASH=$m4c HANG=$m4h UNPROVEN=$m4u SKIP=$m4s XFAIL=$m4x ($m4xs) XPASS=$m4xp   / $mt"
echo "rerun a single mode: python3 $HARNESS run $MASTER_ICN $MASTER_REF --lang icon --modes m3   (per-entry attributes: ALL.csv)"
# ⭐ THE NAMES, so two runs of this board can be DIFFED and not merely compared. Capped, because the
# point is to make a regression identifiable, not to paste a census into a terminal -- and the cap says
# so out loud rather than truncating silently, which would be a smaller version of the same defect.
# ⛔ `grep -c` PRINTS "0" *AND* EXITS 1 on no match, so `|| echo 0` printed a SECOND zero and every green board printed
# "[: 0\n0: integer expression expected" twice and carried on (measured 2026-09-04, hq_B; rc stayed 0, so nothing caught it).
# ⛔ AND THE FIRST CURE (08c96e2b9, drop the fallback) KILLED EVERY GREEN BOARD: this script runs under `set -e`, and
# `grep -c` exits 1 on zero matches, so the bare assignment aborted the board silently right after the "rerun a
# single mode" line -- measured by the strip row's `done` 20 minutes later. `|| true` keeps grep's own "0" and its
# failing status out of set -e; the empty-case default stays for a missing file.
_nfail=$(grep -cE '^[[:space:]]*(FAIL|CRASH|HANG|XPASS|UNPROVEN) ' "$_errf" 2>/dev/null || true); _nfail=${_nfail:-0}
if [ "${_nfail:-0}" -gt 0 ]; then
    echo ""
    echo "--- the $_nfail non-PASS entries by name (showing up to 40; stderr of the run above) ---"
    grep -E '^[[:space:]]*(FAIL|CRASH|HANG|XPASS|UNPROVEN) ' "$_errf" | head -40
    [ "$_nfail" -gt 40 ] && echo "    ... and $((_nfail - 40)) more (rerun and keep stderr to see them all)"
fi

RED=0
# ⛔ THE DENOMINATOR CHECK IS THE POINT OF THIS BOARD — a shrink is the silent-orphan class itself.
if [ "$graded" -lt "$ENTRY_FLOOR" ]; then
    echo "⛔ RED: graded population $graded is UNDER the floor $ENTRY_FLOOR — $((ENTRY_FLOOR-mt)) entries vanished from the master,"
    echo "        or this checkout is behind origin. An entry that stops being graded is exactly what this board exists to catch."
    RED=1
fi
if [ "$graded" -ne "$CSV_ENTRIES" ]; then
    echo "⚠️  NOTE: harness graded $graded entries but ALL.csv carries $CSV_ENTRIES rows — the suite file and its"
    echo "        provenance index disagree. Neither number is wrong on its face; they must not drift apart."
fi
# ⭐ PASS WATERMARKS: red on regression only. These are NOT a claim that the remainder is acceptable —
# 122 m3 failures are real and belong to hq_C's lane; this board's job is to notice movement.
if [ "$m3p" -lt "$M3_PASS_FLOOR" ]; then echo "⛔ RED: m3 PASS $m3p regressed below watermark $M3_PASS_FLOOR"; RED=1; fi
if [ "$m4p" -lt "$M4_PASS_FLOOR" ]; then echo "⛔ RED: m4 PASS $m4p regressed below watermark $M4_PASS_FLOOR"; RED=1; fi
# ⛔ NEVER RED: a self-pin has nothing to regress below (see the AST-SHAPE note above) -- reported so
# drift is visible, but it cannot fail this board. A mismatch means RE-DECIDE THE SHAPE AND REGENERATE.
if [ "$ap" -lt "$at" ]; then echo "⚠️  AST-shape drift: $((at-ap)) parser-ladder fixture(s) no longer match their pinned dump — re-decide the shape and regenerate (ast-dump-refs-are-self-pins-not-oracles), NOT a correctness regression"; fi
if [ "$((m3p+m4p))" -gt "$((M3_PASS_FLOOR+M4_PASS_FLOOR))" ]; then
    echo "⭐ WATERMARK MOVED UP (m3 $m3p vs $M3_PASS_FLOOR, m4 $m4p vs $M4_PASS_FLOOR) — re-pin the floors in the commit that earned it."
fi
[ "$((m3xp+m4xp))" -gt 0 ] && echo "⭐ XPASS>0: a bug got FIXED and its XFAIL marker was never promoted — as actionable as a failure, in the opposite direction."

# ⛔ ONE LEADERBOARD (RULES.md FACT RULE, Lon 2026-09-03 ~16:05). Records what this script just
# measured into .github/SCORE.md; runs nothing itself. Non-fatal: a bookkeeping failure must never
# turn a real measurement into a red board.
# ⭐ THE CELL CARRIES THE NAMES, not just the count. A leaderboard cell reading "2 below the floor" makes
# the next reader re-run a 20-minute board to find out WHICH two; the names make the row diffable against
# the next run, which is the whole complaint that row icon-master-board-is-two-below-watermark-and-the-
# board-never-names-the-failures was raised about. Capped at six so a genuinely broken board does not
# paste a census into a markdown table, and the overflow is stated rather than silently dropped.
_named=""
if [ "${_nfail:-0}" -gt 0 ]; then
    _names=$(grep -E '^[[:space:]]*(FAIL|CRASH|HANG|XPASS|UNPROVEN) ' "$_errf" \
             | sed -E 's/^[[:space:]]*([A-Z]+) [a-z0-9]+ ([^:]+):.*/\1 \2/' | sort -u | head -6 | paste -sd'; ' -)
    _named=" — reds by name: $_names"
    _uniq=$(grep -E '^[[:space:]]*(FAIL|CRASH|HANG|XPASS|UNPROVEN) ' "$_errf" \
            | sed -E 's/^[[:space:]]*[A-Z]+ [a-z0-9]+ ([^:]+):.*/\1/' | sort -u | wc -l)
    [ "${_uniq:-0}" -gt 6 ] && _named="$_named (and $((_uniq - 6)) more entries)"
fi
# ⛔ PLACED ABOVE THE RED EXIT ON PURPOSE. A red board is still a MEASUREMENT, and the FACT RULE says
# ANY run -- recording only green boards would make the leaderboard a trophy cabinet, showing each
# suite's best remembered day rather than its state, which is the exact opposite of what it is for.
# ⛔⭐ DECLARE THE PAIR RATHER THAN RELY ON THE TEXT HAPPENING TO CARRY ONE FRACTION (hq_T 2026-09-06,
# ceo CEO-363). This row auto-resolved until now only because its board line prints m3 and m4 with the SAME
# numbers, so the two fractions collapsed to one reading; the day the modes diverge -- which is exactly the
# day the number matters -- it would have started refusing instead. A mechanism that works because two
# values happen to be equal is not wired, it is lucky.
# ⛔⭐ AND THE DECLARED PAIR IS THE AND PER PROGRAM (ceo-372): the entries green in EVERY graded mode, never
# m4 alone (what this row published until now), never m3 alone, never min(m3p,m4p). ⭐ NOTE THIS BOARD'S OWN
# HISTORY IS THE ARGUMENT FOR IT: m3p and m4p have been EQUAL here, which reads as "both modes agree" and is
# not the same claim at all -- equal COUNTS are consistent with two disjoint sets of reds. Only the AND can
# tell those apart, and where the two numbers really did describe the same programs it changes nothing.
python3 "$HERE/util_score_row.py" write --lang icon --column board --modes m3,m4 \
    --suite-pass "$mall" --suite-total "$mt" \
    --measurer "${S4E_SEAT:-}" \
    --text "$([ "$RED" -ne 0 ] && echo "⛔ RED — ")run-graded both-modes $mall/$mt · m3 $m3p/$mt xfail=$m3x ($m3xs) xpass=$m3xp · m4 $m4p/$mt xfail=$m4x ($m4xs) xpass=$m4xp (entries=$graded, floors m3 $M3_PASS_FLOOR / m4 $M4_PASS_FLOOR, \`board_icon_master.sh\`) · ast-shape check $ap/$at xpass=$axp (informational, not scored)$_named" \
    || echo "⚠ SCORE.md NOT UPDATED -- record this row by hand (the REFUSED line above says why)"
# ⭐ THE PROGRESS LINE, after the rewrite.  This runner writes its row DIRECTLY rather than through
# lib_gate.sh's gate_score_row, so it needs the call the shared path already carries -- same one line,
# not a second implementation (both ends run `util_score_row.py progress`, which reads SCORE.md and
# runs no suite).  Non-fatal by construction: it must not be able to change this board's verdict.
python3 "$HERE/util_score_row.py" progress 2>/dev/null || true
if [ "$RED" -ne 0 ]; then echo "⛔ ICON MASTER BOARD RED"; exit 1; fi
echo "✅ ICON MASTER BOARD OK: entries=$graded at/above floor $ENTRY_FLOOR · run-graded both-modes PASS=$mall/$mt (the AND, what the leaderboard row states) · m3 PASS=$m3p m4 PASS=$m4p / $mt (watermarks held) · ast-shape check $ap/$at (informational)"
