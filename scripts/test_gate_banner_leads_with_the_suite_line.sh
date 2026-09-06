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

OUT="$(bash "$S4E_MSG" banner 2>&1)" || true
[ -n "$OUT" ] || { echo "GATE UNPROVEN(2) [$GATE]: banner printed nothing at all -- cannot measure"; exit 2; }

# --- ARM 1: the suite line prints ---
arms=$((arms+1))
suite_ln="$(printf '%s\n' "$OUT" | grep -n 'SUITES 100/100' | head -1 | cut -d: -f1)"
[ -n "$suite_ln" ] || fail "ARM 1: no suite line in the banner (expected the util_suite_banner.py headline)"

# --- ARM 2: it prints BEFORE the per-language PROGRESS line ---
arms=$((arms+1))
prog_ln="$(printf '%s\n' "$OUT" | grep -n '^PROGRESS 09-10 |' | head -1 | cut -d: -f1)"
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
arms=$((arms+1))
printf '%s\n' "$BROKE" | grep -qE '(✅|⛔) [A-Z]' \
    || fail "ARM 4: the banner printed no computed verdict while the suite script was absent -- the refusal must not take the verdict down with it"

# --- ARM 5: the happy path does not print the refusal (or ARM 3 proves nothing) ---
arms=$((arms+1))
printf '%s\n' "$OUT" | grep -q 'SUITE BANNER: UNREADABLE' \
    && fail "ARM 5: the NORMAL banner printed the UNREADABLE refusal -- ARM 3 would then pass against a wiring that always refuses"

if [ "$viol" -ne 0 ]; then
    echo "⛔ GATE FAIL [$GATE]: $viol of $arms arms broken"
    exit 1
fi
echo "GATE PASS(0) [$GATE]: $arms arms -- suite line prints, leads the PROGRESS line, refuses VISIBLY when absent, and the verdict survives"
exit 0
