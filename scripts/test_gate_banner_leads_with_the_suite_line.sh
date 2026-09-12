#!/usr/bin/env bash
# test_gate_banner_leads_with_the_suite_line.sh -- proves `s4e_msg.sh banner` leads with THE SUITE BANNER
# (row banner-leads-with-the-suite-banner-line-from-util-suite-banner, rank 0, Lon 2026-09-06 11:20 via ceo,
# THE FLEET-12 PLAN: "Banner every turn: python3 .github/scripts/util_suite_banner.py").
#
# THREE PROPERTIES, and the third is the one that needs a gate rather than a look:
#   1. the suite line PRINTS at all;
#   2. it prints BEFORE the per-language PROGRESS line -- ordering is the whole point of the row, because
#      whatever prints last is what a Stop-hook timeout or a truncated display loses (measured on seat07:
#      four banners, two score lines, the score printing after ~9 s of slower work);
#   3. ⛔ FAIL-ONCE: with the script absent the banner SAYS SO VISIBLY and the verdict is untouched.
#
# ⛔ WHY (3) IS THE LOAD-BEARING ARM. A banner line that silently vanishes when its data is missing is
# INDISTINGUISHABLE FROM A FLEET WITH NO SUITES -- the not-found/not-there collision this tree has now
# collected half a dozen instances of. An arm that only asserted the happy path would go green against a
# wiring that prints nothing at all on a fresh clone, which is exactly the state a new seat starts in.
# ⭐ And it asserts the verdict SURVIVES the failure, not merely that a message appeared: a refusal that
# takes the banner's computed ✅/⛔ down with it would trade a missing line for a missing verdict.
#
# Runs the REAL banner against the REAL postoffice read-only (banner is idempotent and writes nothing) and
# drives the absent case through S4E_SUITE_BANNER_PROBE_BROKEN, which points at a nonexistent SCRIPT.
# ⛔ THAT PROBE IS DELIBERATELY NOT THE ONE BESIDE IT IN s4e_msg.sh: the PROGRESS line proves its refusal by
# pointing S4E_HOME at a nonexistent home, but util_suite_banner.py resolves SUITES.tsv from its own
# __file__ and never reads S4E_HOME -- so that probe would prove nothing here while looking identical.
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
S4E_MSG="$HERE/s4e_msg.sh"
GATE="banner_leads_with_the_suite_line"
viol=0
arms=0
fail() { echo "  ⛔ $1"; viol=$((viol+1)); }

[ -f "$S4E_MSG" ] || { echo "GATE UNPROVEN(2) [$GATE]: $S4E_MSG not on disk -- cannot measure"; exit 2; }

# ⛔⭐⭐ THE DATA SOURCE IS PREFLIGHTED, BECAUSE ITS ABSENCE IS NOT THIS GATE'S VERDICT TO GIVE (hq_T
# 2026-09-10, on the cfo's report via CEO-489; row banner-gate-arm-4-reads-live-session-state-so-it-grades-
# the-seat-not-the-tool, which this is the first measured instance of). ARMS 1, 2 and 6 all read the LIVE
# banner's suite line, and that line is produced from $S4E/.github/scripts/util_suite_banner.py. In a root
# whose .github is unpulled or thinner than this one's, the script is simply not there, the line cannot
# print, and this gate RED THREE OF SIX ARMS -- reproduced exactly, rc=1 "3 of 6 arms broken", by pointing
# S4E_HOME at a root without it. ⛔ THAT IS A FALSE RED: the banner was correct and the tool was correct;
# only the DATA was absent, and "I could not measure" is not "you are broken" (RULES.md: a test that cannot
# measure REFUSES rc=2, never skip-as-success and never a conviction either). It cost the cfo a red on
# origin and a telegram, and it convicted the one seat whose root was fine.
# ⭐ WHY THIS DOES NOT MAKE THE GATE VACUOUS, which is the obvious objection: ARM 3 drives the absent case
# DELIBERATELY through S4E_SUITE_BANNER_PROBE_BROKEN and still asserts the banner SAYS SO out loud. So the
# missing-script behaviour is still fully graded -- by the arm built to grade it, on a root where the
# script exists. What is removed is only this gate's ability to convict a root for what its .github lacks.
_S4E="${S4E_HOME:-$(cd "$HERE/../.." && pwd)}"
_SB="$_S4E/.github/scripts/util_suite_banner.py"
[ -f "$_SB" ] || { echo "GATE UNPROVEN(2) [$GATE]: the suite banner's own source is not on disk at $_SB, so the live banner CANNOT carry a suite line and arms 1/2/6 would convict this root for what its .github lacks -- cannot measure. Pull .github (git -C $_S4E/.github merge --ff-only origin/main) and re-run."; exit 2; }

OUT="$(bash "$S4E_MSG" banner 2>&1)" || true
[ -n "$OUT" ] || { echo "GATE UNPROVEN(2) [$GATE]: banner printed nothing at all -- cannot measure"; exit 2; }

# --- ARM 1: the suite line prints ---
arms=$((arms+1))
suite_ln="$(printf '%s\n' "$OUT" | grep -n 'SUITES 100/100' | head -1 | cut -d: -f1)"
[ -n "$suite_ln" ] || fail "ARM 1: no suite line in the banner (expected the util_suite_banner.py headline)"

# --- ARM 2: it prints BEFORE the per-language PROGRESS line ---
arms=$((arms+1))
prog_ln="$(printf '%s\n' "$OUT" | grep -n '^PROGRESS [0-9][0-9]-[0-9][0-9] |' | head -1 | cut -d: -f1)"
if [ -n "$suite_ln" ] && [ -n "$prog_ln" ]; then
    [ "$suite_ln" -lt "$prog_ln" ] || fail "ARM 2: the suite line is at line $suite_ln, BELOW the PROGRESS line at $prog_ln -- the row is about which one leads"
elif [ -z "$prog_ln" ]; then
    fail "ARM 2: no PROGRESS line to order against -- the language line is demoted by this row, never removed"
fi

# --- ARM 3 (FAIL-ONCE): script absent -> visible refusal, verdict untouched ---
arms=$((arms+1))
BROKE="$(S4E_SUITE_BANNER_PROBE_BROKEN=1 bash "$S4E_MSG" banner 2>&1)" || true
printf '%s\n' "$BROKE" | grep -q 'SUITE BANNER: UNREADABLE' \
    || fail "ARM 3: with the script absent the banner did NOT print 'SUITE BANNER: UNREADABLE' -- a silently missing suite line reads as a fleet with no suites"

# --- ARM 4: the computed verdict survives the absent case ---
# ⛔⭐ ALL THREE VERDICT CLASSES COUNT, NOT TWO (cfo 2026-09-08, ceo RULED 17:39; FINDING-2026-09-08-cfo-
# make-test-dies-at-arm-2-...). This arm matched only ✅ and ⛔ while the banner has a THIRD class, "⚠ NOTHING
# LANDED", printed when a session produced no commit and no FINDING -- a correctly COMPUTED verdict that this
# arm read as NO verdict. Because the arm greps the LIVE session's banner, that made `make test` RED AT ARM 2
# OF ~65 for any seat that had not yet committed, and GREEN for the same tree the moment it did: measured both
# ways on one tree, stash-controlled, and it was concealing a real Prolog red at arm ~57 that the pre-landing
# run could never reach. ⛔ THE DEEPER DEFECT IS THAT THIS ARM GRADES THE SEAT RATHER THAN THE TOOL and so
# breaks test-postoffice's own hermetic membership rule; the structural cure -- drive the banner from a session
# state this gate CONTROLS -- is its own row (banner-gate-arm-4-reads-live-session-state-so-it-grades-the-seat-
# not-the-tool, cfo). Until that lands, this class list must track s4e_msg.sh's verdict classes or the arm reds
# again for the next class anyone adds.
arms=$((arms+1))
printf '%s\n' "$BROKE" | grep -qE '(✅|⛔|⚠) [A-Z]' \
    || fail "ARM 4: the banner printed no computed verdict while the suite script was absent -- the refusal must not take the verdict down with it"

# --- ARM 5: the happy path does not print the refusal (or ARM 3 proves nothing) ---
arms=$((arms+1))
printf '%s\n' "$OUT" | grep -q 'SUITE BANNER: UNREADABLE' \
    && fail "ARM 5: the NORMAL banner printed the UNREADABLE refusal -- ARM 3 would then pass against a wiring that always refuses"

# --- ARM 6: the GRID LINES UP VERTICALLY, measured in DISPLAY COLUMNS not characters ---
# Lon 2026-09-06: "get the suites banner to line up vertically; most likely your length counts are off due to
# unicode." He was exactly right about the cause. Three independent ways len() lied in one 40-column cell:
#   - a WIDE glyph is 1 char / 2 columns (U+2705 done, U+26D4 stuck, U+1F195 new, most emoji);
#   - a VARIATION SELECTOR is 1 char / 0 columns AND makes its narrow base render wide (❄️ 🏛️ = base + U+FE0F);
#   - a REGIONAL INDICATOR PAIR is 2 chars / one 2-column glyph (🇫🇷 = U+1F1EB U+1F1F7).
# ⛔ THE ARROW IS WHY IT LOOKED ALMOST RIGHT: U+2192 is east_asian_width 'A' and renders NARROW, so the ETA
# cells were the only ones whose character count matched their column count. A defect that is correct on the
# majority of rows reads as a rendering quirk rather than a bug.
# ⭐ THIS ARM MEASURES SEPARATOR POSITIONS, NOT CELL LENGTHS. Equal cell widths are what the buggy version
# already believed it had; what a reader actually sees is where the │ lands.
# ⛔ A SHORT FINAL ROW IS NOT A MISALIGNMENT (ceo 2026-09-08, CEO-394): the arm compared the whole tuple of
# separator positions and required every row to have the SAME NUMBER of them, so it went red the moment the
# suite count stopped being a multiple of the column count -- the coo's 22nd suite row (TPgm) turned a 7x3
# grid ragged and reddened the blocking set for every seat, with nothing misaligned on screen. Each row's
# separators must now match the PREFIX of the widest row, which is the property a reader can actually see;
# a real misalignment (a mis-measured glyph moving a │) still reds it, and that fail-once was re-proven.
arms=$((arms+1))
python3 - "$HERE/../../.github/scripts/util_suite_banner.py" <<'PYEOF' || fail "ARM 6: the suite grid does not line up vertically in display columns"
import sys, subprocess, importlib.util
spec = importlib.util.spec_from_file_location("b", sys.argv[1])
m = importlib.util.module_from_spec(spec); spec.loader.exec_module(m)
out = subprocess.run([sys.executable, sys.argv[1], "--plain"], capture_output=True, text=True).stdout
lines = [l for l in out.split("\n") if " \u2502 " in l]
if not lines: print("REFUSE: no grid rows to grade"); sys.exit(2)
pos = []
for l in lines:
    seps = []; start = 0
    while True:
        i = l.find(" \u2502 ", start)
        if i < 0: break
        seps.append(m.dw(l[:i])); start = i + 3
    pos.append(tuple(seps))
widest = max(pos, key=len)
bad = [p for p in pos if p != widest[:len(p)]]
if bad:
    print("separator display-columns differ across rows: %s against the full row %s" % (sorted(set(bad)), (widest,))); sys.exit(1)
short = [p for p in pos if len(p) != len(widest)]
print("grid aligned: %d rows, separators at %s display columns%s"
      % (len(lines), widest, (" (%d final row(s) short a cell: the suite count is not a multiple of the column count)" % len(short)) if short else ""))
PYEOF

if [ "$viol" -ne 0 ]; then
    echo "⛔ GATE FAIL [$GATE]: $viol of $arms arms broken"
    exit 1
fi
echo "GATE PASS(0) [$GATE]: $arms arms -- suite line prints, leads the PROGRESS line, refuses VISIBLY when absent, and the verdict survives"
exit 0
