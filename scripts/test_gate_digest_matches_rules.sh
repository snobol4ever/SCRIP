#!/usr/bin/env bash
# test_gate_digest_matches_rules.sh -- GATE: no sibling-root CLAUDE.md may assert a FACT RULE's
# RETIRED text as if it were still current.
#
# WHY THIS EXISTS (row `claude-md-digest-drifts-from-rules`, minted by hq_P from seat11's find,
# FINDING-2026-08-23-hq_P-the-per-root-claude-md-digest-is-not-git-tracked-and-15-of-19-were-stale.md):
# measured s267, 15 of 19 roots carried the retired "-O2 is used ONLY for benchmark/demo runs" text
# with no mention of the s262 NO--O2 fact rule, and seat11 built at -O2 on it -- a real wasted build.
# Root cause is STRUCTURAL: each root's CLAUDE.md (D-17 PORTABLE-HOME, /home/claudeNN/CLAUDE.md etc)
# is a plain file, not tracked in any git repo, so it never updates when .github/RULES.md does, while
# being the first thing a seat reads and phrased as instruction. A memo fixes today and decays; this
# gate is the structural follow-up (.github/RULES.md's own corollary doctrine, ~line 168: "digests
# never restate law ... law that exists in one versioned file cannot go stale in nineteen unversioned
# ones" -- this gate is how that doctrine gets CHECKED instead of merely stated).
#
# METHOD, AND ITS HONEST LIMITATION: for each table entry below, grep every root's CLAUDE.md for a
# RETIRED-text pattern (a distinctive phrase from wording RULES.md itself has explicitly retired).
# A raw hit is not yet a violation -- a digest is allowed to MENTION retired text while explaining
# it is dead (RULES.md line ~149 does exactly this, on purpose, "quoted once, here, only so a reader
# who remembers it knows it was retired"). So each hit's +-2 line window is checked for a CORRECTIVE
# SIGNAL word list; only a hit with NO corrective signal nearby counts as a violation. This is a
# substring/proximity heuristic, not comprehension -- it can miss a differently-worded stale claim
# and could in principle misfire on an unlucky sentence. It is READ-ONLY and reports for a human (or
# the owning seat) to adjudicate; it does not decide anything by itself. Validated against the real
# fleet at mint (2026-08-24): the O2 rule below found 3 raw hits, all 3 correctly self-aware (0
# violations) -- proving the corrective-signal filter is load-bearing, not decorative, since a naive
# grep-only gate would have flagged all 3 as false positives. The SEGV-attribution rule found 16 of
# 19 roots carrying the retired claim UNCORRECTED (real, live, same-day as this gate's own mint).
#
# ⛔ READ-ONLY BY DEFAULT (task's own NEXT step 2) -- THIS SCRIPT NEVER WRITES TO ANY ROOT'S CLAUDE.md.
# hq_P attempted a bulk cross-seat edit at s267 and was correctly blocked by the permission classifier
# -- one bad regex would corrupt every seat's orientation file at once. This gate REPORTS; each seat
# fixes its own copy on sight, per the standing doctrine already in every current CLAUDE.md banner.
#
# EXTENDING THE TABLE: add one more `check_rule` call at the bottom -- id, retired-text grep -E
# pattern, corrective-signal grep -E pattern, and a citation into RULES.md. Not every FACT RULE has
# a quotable retired predecessor text (most don't retire anything, they state a fresh constraint), so
# this table is seeded with the rules that DO, not a mechanical one-per-bullet transcription.
#
# ROOTS: override via DIGEST_GATE_ROOTS (space-separated list of CLAUDE.md paths) for testing against
# scratch files instead of the real fleet -- never point the real invocation anywhere but the 19 real
# roots.
#
# Exit codes per lib_gate.sh: 0 CLEAN / 1 VIOLATION / 2 UNPROVEN (a root unreadable -- never silently
# skipped, per this row's own NEXT step 1).
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
# shellcheck source=lib_gate.sh
. "$HERE/lib_gate.sh"
gate_parse_args "$@"

if [ -n "${DIGEST_GATE_ROOTS:-}" ]; then
    read -r -a ROOTS <<< "$DIGEST_GATE_ROOTS"
else
    ROOTS=(/home/claude01/CLAUDE.md /home/claude02/CLAUDE.md /home/claude03/CLAUDE.md /home/claude04/CLAUDE.md \
           /home/claude05/CLAUDE.md /home/claude06/CLAUDE.md /home/claude07/CLAUDE.md /home/claude08/CLAUDE.md \
           /home/claude09/CLAUDE.md /home/claude10/CLAUDE.md /home/claude11/CLAUDE.md /home/claude12/CLAUDE.md \
           /home/claude13/CLAUDE.md /home/claude14/CLAUDE.md /home/claude15/CLAUDE.md /home/claude16/CLAUDE.md \
           /home/claude_C/CLAUDE.md /home/claude_P/CLAUDE.md /home/claude/CLAUDE.md)
fi

VIOLATIONS=0
EXAMINED=0

# check_rule <rule_id> <retired_text_grep_-E_pattern> <corrective_signal_grep_-E_pattern> <citation>
check_rule() {
    local rule_id="$1" retired_re="$2" signal_re="$3" citation="$4"
    local f lineno rest window
    for f in "${ROOTS[@]}"; do
        EXAMINED=$((EXAMINED + 1))
        if [ ! -r "$f" ]; then
            echo "GATE UNPROVEN(2) [$GATE_NAME]: required root digest is not readable at $f"
            echo "    Never counted as clean -- an unreadable root is not a compliant one."
            exit 2
        fi
        while IFS=: read -r lineno rest; do
            [ -n "${lineno:-}" ] || continue
            window="$(sed -n "$(( lineno > 2 ? lineno - 2 : 1 )),$(( lineno + 2 ))p" "$f")"
            if printf '%s\n' "$window" | grep -qiE "$signal_re"; then
                continue
            fi
            VIOLATIONS=$((VIOLATIONS + 1))
            echo "GATE HIT [$rule_id] $f:$lineno -- retired text with no corrective signal nearby"
            echo "    | $rest"
            echo "    cite: $citation"
        done < <(grep -niE "$retired_re" "$f")
    done
}

check_rule "NO-O2-EVER" \
    'used ONLY for benchmark' \
    'retire|supersed|\bdead\b|corrected|used to (carry|stand)|do not follow' \
    '.github/RULES.md FACT RULE "NO `-O2` BUILDS. EVER." (~line 148-149)'

check_rule "SEGV-HANDLER-ATTRIBUTION" \
    'CSN_NO_SEGV_HANDLER|SCRIP_NO_SEGV_HANDLER' \
    'not SCRIP|NEVER.{0,15}SCRIP|csnobol4|no getenv|correct(ed|ion)?|WRONG|never was|externally.clone' \
    '.github/RULES.md ASM-DIFF-FIRST correction, landed 2026-08-24 (~line 47)'

gate_floor "$EXAMINED" 2 "root-digest checks (roots × rules)"
gate_verdict "$VIOLATIONS" "root digest(s) asserting retired FACT RULE text uncorrected"
