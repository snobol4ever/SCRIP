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
# who remembers it knows it was retired"). So each hit's OWN LINE is checked for a CORRECTIVE SIGNAL
# word list; only a hit with NO corrective signal on that same line counts as a violation. This is a
# substring heuristic, not comprehension -- it can miss a differently-worded stale claim and could in
# principle misfire on an unlucky sentence. It is READ-ONLY and reports for a human (or the owning
# seat) to adjudicate; it does not decide anything by itself.
#
# ⛔ CORRECTED 2026-08-24, SAME DAY AS MINT (seat15 found it, hq_C root-caused it, this seat fixed it
# -- kept here as an addendum, not a silent edit, per this project's own transcription-provenance
# rule). The ORIGINAL version checked a +-2 LINE WINDOW around each hit, and two of the signal
# alternatives were unanchored catch-alls: `correct(ed|ion)?` (the optional group matches the bare
# substring "correct", so it fires on "correctness" -- a word saturating a project whose HQ is named
# HQ-CORRECTNESS) and `csnobol4` alone (fires on any unrelated mention of the oracle tree, which every
# digest's workspace-map section contains). PROVEN BY CONSTRUCTION: a scratch file carrying the exact
# retired SEGV-handler line reported GATE PASS(0) with "correctness" one line above it, and PASS(0)
# again with "csnobol4" one line above it -- a real violation, wrongly exempted by unrelated text in a
# DIFFERENT sentence. Current state was genuinely clean when this fired (verified independently of the
# gate, by direct per-root reading), so it was not masking anything THAT day -- but the instrument
# could not have told a true clean state from a false one, which is the actual defect: a gate needs a
# test for every way it can say NOTHING, not only for every way it can say something, and the exemption
# path never had one. Fix: (1) anchor the signal check to the MATCHED LINE ONLY, never a window -- every
# real corrected example measured (both rules, all fleet roots) puts the correction in the SAME
# sentence/line as the retired text, so this loses no real recall; (2) drop both catch-alls --
# `corrected|correction` (no optional group) and `csnobol4[ -]?oracle` (adjacency required). Re-verified
# against real data after the fix: O2 rule's 3 self-aware hits still correctly exempted (0 violations,
# unchanged); SEGV rule's live violations still correctly caught. Negative-tested the exemption path
# itself, not just the three exit-code arms: hq_C's two proof-by-construction false negatives (word on
# the line ABOVE a real hit) now both correctly report VIOLATION; a same-line "correctness" (no
# "corrected"/"correction") on an otherwise-real hit now also correctly reports VIOLATION, isolating the
# regex-tightening fix from the windowing fix. Full receipts in the task file's own LEDGER and a FINDING.
#
# ⛔ SECOND CORRECTION 2026-08-26 (hq_C, found while rewriting its own root's CLAUDE.md). The 2026-08-24
# addendum above cured an EXEMPTION miss -- the gate SAW a hit and wrongly excused it. This is the
# other half, and it is a RECALL miss: the gate NEVER SAW the hit at all. NO-O2-EVER's retired-text
# pattern was the single literal phrase `used ONLY for benchmark`, so a PARAPHRASE walked straight
# past it. PROVEN BY CONSTRUCTION: hq_C's own root carried "**`-O2` is reserved for benchmark and demo
# runs**, passed explicitly: RT_OPT="-O2 ..." make" -- a live, uncorrected violation that also PRINTS
# THE FORBIDDEN COMMAND -- and the gate reported PASS(0) on it, both alone and inside the full 38-check
# run that was quoted as proof the fleet was clean. ⭐ THE TWO MISSES ARE NOT THE SAME BUG AND A TEST
# FOR ONE DOES NOT COVER THE OTHER: an exemption miss is a wrong ANSWER on a real hit; a recall miss is
# NO QUESTION ASKED. Both present identically to the reader -- as PASS(0). The 2026-08-24 note's own
# line ("a gate needs a test for every way it can say NOTHING") named the class correctly and the fix
# only covered the exemption path; the retired-text pattern was left as a single literal string, which
# is the OTHER way this gate can say nothing. Fix: (1) retired_re gains `reserved for benchmark`,
# `-O2 is (reserved|used only)`, and `RT_OPT=.?-O2` -- the last matters most because a digest printing
# the forbidden BUILD COMMAND is instructing a violation no matter how the prose is worded; (2) the
# signal list gains `never (pass|build|use|quote)` and `NO .?-O2. BUILDS` so a digest that names the
# command in order to FORBID it is still correctly exempt. Negative-tested, five arms: paraphrase alone
# -> VIOLATION; paraphrase + signal on the SAME line -> exempt; paraphrase + signal on a NEIGHBOURING
# line -> VIOLATION (hq_P's requested assertion, the 2026-08-24 anchoring preserved); bare forbidden
# RT_OPT command -> VIOLATION; hq_C's real repaired digest -> clean. Full 19-root run PASS(0) both
# before and after, so this is pure recall gain: no root is newly flagged and no verdict moved.
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
# scratch files instead of the real fleet -- never point the real invocation anywhere but the 21 real
# roots. claude_B and claude_T were ABSENT until 2026-09-04 (hq_B, ceo ruling ruling-digest-gate-roots):
# both roots were opened after this list was written, so the two HQs whose lanes are HYGIENE and TESTS
# were the two whose digests no gate read, and the gate was green about the other nineteen the whole
# time -- the same narrow-instrument shape those digests document for `command -v` and for `$?` after a
# pipe. A hard-coded population does not announce what it left out.
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
           /home/claude_C/CLAUDE.md /home/claude_P/CLAUDE.md /home/claude_B/CLAUDE.md /home/claude_T/CLAUDE.md \
           /home/claude/CLAUDE.md)
fi

VIOLATIONS=0
EXAMINED=0

# check_rule <rule_id> <retired_text_grep_-E_pattern> <corrective_signal_grep_-E_pattern> <citation> <canary_line>
#
# ⭐⭐ THE CANARY (5th arg) IS NOT OPTIONAL AND IT IS THE POINT OF THIS FUNCTION'S THIRD REVISION.
# hq_P, 2026-08-26: "You cannot fix a recall problem by improving recall, because the failure is
# silent BY CONSTRUCTION -- you have made the next miss less likely without making it any louder."
# Correct, and it is the argument against the 2026-08-26 fix on its own terms: broadening retired_re
# to catch a paraphrase is still A PATTERN, and every pattern shares the property that killed the
# original one -- WHEN IT STOPS MATCHING, NOTHING HAPPENS. So each rule now carries a known-violating
# line that MUST be caught. If the canary goes quiet -- someone "tidies" the regex, a grep flag
# changes, the alternation is broken by an unescaped char -- the gate REFUSES (rc=2) instead of
# printing the PASS(0) it would otherwise print, unchanged, forever. That converts recall from a
# hope into a tested property. ⛔ The canary must ALSO be checked against signal_re: a canary that
# the exemption path would swallow tests nothing, and would itself go quiet silently.
check_rule() {
    local rule_id="$1" retired_re="$2" signal_re="$3" citation="$4" canary="$5"
    local f lineno rest
    if [ -z "$canary" ]; then
        echo "GATE UNPROVEN(2) [$GATE_NAME]: rule $rule_id has no canary -- recall is untested and a silent miss is indistinguishable from a clean root"
        exit 2
    fi
    if ! printf '%s\n' "$canary" | grep -qiE "$retired_re"; then
        echo "GATE UNPROVEN(2) [$GATE_NAME]: CANARY DEAD for rule $rule_id -- its known-violating line is no longer matched by retired_re"
        echo "    canary: $canary"
        echo "    This gate can no longer detect the thing it exists to detect. Fix retired_re; never quiet the canary."
        exit 2
    fi
    if printf '%s\n' "$canary" | grep -qiE "$signal_re"; then
        echo "GATE UNPROVEN(2) [$GATE_NAME]: CANARY SELF-EXEMPTS for rule $rule_id -- signal_re matches the canary, so it would be excused, not caught"
        echo "    canary: $canary"
        exit 2
    fi
    for f in "${ROOTS[@]}"; do
        EXAMINED=$((EXAMINED + 1))
        if [ ! -r "$f" ]; then
            echo "GATE UNPROVEN(2) [$GATE_NAME]: required root digest is not readable at $f"
            echo "    Never counted as clean -- an unreadable root is not a compliant one."
            exit 2
        fi
        while IFS=: read -r lineno rest; do
            [ -n "${lineno:-}" ] || continue
            # SAME LINE ONLY -- see the 2026-08-24 CORRECTED addendum above. A wider window let an
            # unrelated word in a different sentence exempt a real violation.
            if printf '%s\n' "$rest" | grep -qiE "$signal_re"; then
                continue
            fi
            VIOLATIONS=$((VIOLATIONS + 1))
            echo "GATE HIT [$rule_id] $f:$lineno -- retired text with no corrective signal on the same line"
            echo "    | $rest"
            echo "    cite: $citation"
        done < <(grep -niE "$retired_re" "$f")
    done
}

check_rule "NO-O2-EVER" \
    'used ONLY for benchmark|reserved for benchmark|-O2 is (reserved|used only)|RT_OPT=.?-O2' \
    'retire|supersed|\bdead\b|corrected|used to (carry|stand)|do not follow|never (pass|build|use|quote)|NO .?-O2. BUILDS' \
    '.github/RULES.md FACT RULE "NO `-O2` BUILDS. EVER." (~line 148-149)' \
    '**`-O2` is reserved for benchmark and demo runs**, passed explicitly: RT_OPT="-O2 -g" make'

check_rule "SEGV-HANDLER-ATTRIBUTION" \
    'CSN_NO_SEGV_HANDLER|SCRIP_NO_SEGV_HANDLER' \
    'not SCRIP|NEVER.{0,15}SCRIP|csnobol4[ -]?oracle|no getenv|corrected|correction|WRONG|never was|externally.clone' \
    '.github/RULES.md ASM-DIFF-FIRST correction, landed 2026-08-24 (~line 47)' \
    'run gdb with CSN_NO_SEGV_HANDLER=1 to get a clean backtrace'

gate_floor "$EXAMINED" 2 "root-digest checks (roots × rules)"
gate_verdict "$VIOLATIONS" "root digest(s) asserting retired FACT RULE text uncorrected"
