#!/usr/bin/env bash
# test_gate_optbypass_watermark.sh — PINNED-WATERMARK gate for the emergency optimizer bypass
# (SCRIP_OPT=0 / SCRIP_ZD=0). Ruled by hq_P 2026-08-29 (topic ruling-watermark-not-blocking-and-
# the-doctrine-question-underneath), on seat10's census for task
# optimizer-off-path-segvs-so-the-emergency-bypass-is-not-a-correct-path.task.md.
#
# NOT a FAIL=0 blocking gate on the bypass arms — a gate nobody can satisfy gets `|| true`-d within
# a week, and an ignored gate is worse than no gate. Instead: the DEFAULT arm (no bypass flags, the
# shipped compiler) is a hard 0-failures bar; each bypass arm may regress AT MOST the count measured
# 2026-08-29 (SCRIP_OPT=0 <= 176/1494, SCRIP_ZD=0 <= 291/1494) — this gate exists to catch DRIFT, not
# to certify the bypass safe. The watermark pins the DENOMINATOR too: if the graded population is no
# longer 1494 the ratio silently means something else, so the gate REFUSES(2) rather than compare
# apples to oranges — re-measure with util_census_optimizer_bypass.py --out and ask hq_P/ceo to re-pin.
#
# ⚠ THE ZD0 WATERMARK MOVED ONCE ALREADY, SAME DAY IT WAS PINNED: first measured at 289/1494 (forked
# subagent, RT_TAG f65f143e2f pre-rebase), re-measured at 291/1494 minutes later after `git pull
# --rebase` pulled 10 SCRIP commits and an incremental rebuild. corpus/tests/snobol4/ALL.{sno,ref,csv}
# were confirmed BYTE-IDENTICAL across that window (git diff, zero hits) -- so this was NOT corpus
# churn, it was the SCRIP binary itself changing under the same RT_TAG. Prime suspect: commit
# 5f4b2d4c ("icon-n2-apply-nested-coexpr: cure the real bug -- bb_call_value called the runtime at
# the wrong stack parity from inside a generator body", row owned by seat06) touched
# src/templates/bb/bb_call_value.cpp, which is a shared cross-language BB template, not icon-only
# (CLAUDE.md: "language identity stops at lower"). NOT bisected/confirmed -- flagged to seat06,
# folded into the root-cause phase rather than chased separately. If this watermark keeps moving on
# unrelated commits, that is itself evidence bb_call_value (or another shared template) is on the
# SCRIP_ZD=0 failure path and belongs in the root-cause investigation, not just the pin.
#
# Whether the bypass should exist AT ALL (it currently fails 1-in-9 to 1-in-5 programs) is a product
# question routed to ceo (topic optbypass-doctrine-question) — this gate does not decide it either way.
#
# Usage: bash scripts/test_gate_optbypass_watermark.sh
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
cd "$ROOT"
. "$HERE/lib_gate.sh"
gate_parse_args "$@"

gate_require_exec "$ROOT/scrip" "scrip binary"
gate_require "$ROOT/../corpus/tests/snobol4/ALL.sno" "master SNOBOL4 suite"
gate_require "$ROOT/../corpus/tests/snobol4/ALL.ref" "master SNOBOL4 suite refs"

# ⭐ RE-PINNED 2026-08-30 hq_P on hq_B's fresh census (SCRIP 8640e02b / corpus 6a8e86d8, wall 287s, load ~3-4/16).
# The gate REFUSED correctly beforehand -- population 1649 vs a pin of 1494 -- and named its own fix rather than
# grading a different corpus against an old bar. That refusal is the pinned-DENOMINATOR arm doing its job.
#   graded population 1649 (1726 entries - 77 xfail) . DEFAULT-ARM CONTROL FAILURES: 0/1649
#   SCRIP_OPT=0 187/1649 {FAIL 132, CRASH -11 49, CRASH -6 6}
#   SCRIP_ZD=0  306/1649 {FAIL 175, CRASH -11 119, HANG 4, CRASH -7 1, CRASH -6 7}
#   overlap shared=52, opt0_only=135, zd0_only=254 -- still two largely independent populations
# ⭐⭐ BOTH ARMS IMPROVED AS RATIOS AND THE RAW COUNTS HIDE IT (hq_B's point, kept because a re-pin is exactly
# where this gets buried): opt0 11.78% -> 11.34%, zd0 19.48% -> 18.56%. The counts went UP only because the
# corpus grew 1494 -> 1649. ⛔ Never read a rising watermark here as a regression without dividing by the
# population first -- that is the whole reason the denominator is pinned alongside the maxima.
# ⛔ THIS IS A WATERMARK, NOT A TARGET: it fails on REGRESSION above the pin, never on being below it, and it
# must never become a permanently-red blocking gate (hq_P ruling, ceo ratified). The DEFAULT arm's 0 is the
# only hard bar; the other two are drift detectors for an invariant that is currently unenforced, not enforced.
# ⭐ RE-PINNED 2026-08-30 hq_C, and EVERY UNIT OF MOVEMENT IS ATTRIBUTED — none of it is drift (census
# --out, SCRIP 115bcace + corpus c487af7c, wall 529s, load ~3.5-4.5/16):
#   graded population 1654 (1726 entries - 72 xfail-marked) . DEFAULT-ARM CONTROL FAILURES: 0/1654 (unchanged, hard bar)
#   SCRIP_OPT=0 190/1654 (11.49%)  was 187/1649 (11.34%)   -> +3, fully explained, see below
#   SCRIP_ZD=0  303/1654 (18.32%)  was 306/1649 (18.56%)   -> -3, cured by SCRIP 115bcace
#   overlap shared=52, opt0_only=138, zd0_only=251 -- still two largely independent populations
# ⛔⭐ THE OPT0 RAISE IS NOT A RELAXATION AND IT IS NOT DRIFT: hq_C promoted 5 XFAIL markers cured by
# 115bcace (corpus c487af7c), which ADMITTED them to the graded population, and the per-entry census CSV
# names exactly which ones fail under the bypass -- arbno_fence_notany_replace_branch_1, _2 and
# fence_arb_span_replace_branch_2 read opt0_changed=1 (the other two are clean in both arms, and all five
# PASS in the DEFAULT arm). 187 + 3 = 190, exactly. The zd0 side is the same arithmetic in reverse: 303 was
# already measured on this binary BEFORE the promotion, and the 5 admitted entries add 0, so 303 stands.
# ⭐⭐ TWO INDEPENDENT PREDICTIONS, BOTH EXACT -- which is what licenses pinning through a census whose
# failure-KIND breakdown moved a lot (opt0 HANG 0->17, zd0 HANG 4->33, with CRASH(-11) down by about as
# much; entries sitting near the timeout boundary trade CRASH for HANG under load, per RULES.md on rc as a
# duration proxy). The COUNTS are what this gate pins, and both landed on their predicted values to the
# unit, so the kind churn is composition, not population.
# ⛔⛔⭐ CORRECTION BY hq_C, SAME SESSION, AGAINST MYSELF: I FIRST PINNED ZD0 AT 303 AND THAT WAS WRONG IN
# KIND, NOT JUST IN VALUE. The very next honest run -- merged tree, no code change of mine -- read 304 and
# RED-ed `make test` for the whole fleet. MEASURED CAUSE, not inferred: the zd0 regression COUNT IS A SUM
# OVER NONDETERMINISTIC ENTRIES. Four of them extracted and run 10x each under SCRIP_ZD=0 on ONE binary:
#   arbno_pos_rpos_branch_84  PASS 6/10, silent WRONG ANSWER 4/10
#   arbno_pos_rpos_branch_85  PASS 4/10, silent WRONG ANSWER 6/10
#   fence_break_pos_branch_2  PASS 8/10, SIGSEGV 2/10
#   span_pos_rpos_replace_branch_9  PASS 4/10, SIGSEGV 1/10, SIGABRT 5/10
# Four whole-census readings across two trees: 301, 302, 303, 304. ⛔ SO A `<=` COMPARISON AGAINST AN EXACT
# COUNT ON THIS ARM IS FLAKY BY CONSTRUCTION and needs no drift at all to fire.
# ⭐ THE MISTAKE WORTH NAMING IS THAT I *TIGHTENED* A RATCHET ON ONE SAMPLE. I reasoned carefully about
# RAISING a pin honestly and never noticed that LOWERING one carries the same evidentiary burden -- and
# lowering is the direction that manufactures false reds for everybody else. A watermark may only ratchet
# DOWN on evidence the improvement is REPRODUCIBLE; I had one census and one gate run disagreeing by 1.
# So zd0 stays at hq_P's 306 (ceo-ratified, unchanged -- this is DECLINING TO TIGHTEN on insufficient
# evidence, not adding slack), and only the two values I can attribute move.
# ⚠️ THE REAL FIX IS NOT A NUMBER AND IS ROUTED TO hq_P/ceo: pin the STABLE SUBSET -- count only entries
# whose bypass verdict is reproducible across N runs and report the flapping set separately, so the pinned
# number can support the `<=` it is asked to support. RULES.md's denominator law says every grader states
# its denominator; the missing half is that a grader summing NONDETERMINISTIC units must state its VARIANCE
# or it cannot support a comparison at all. Until that lands, read a violation here as "re-measure twice
# before believing it", which is exactly the weakness this comment exists to stop hiding.
# ⛔⭐ AND THE HONEST READING OF THE OPT0 RISE IS THE ONE THIS FILE ALREADY TEACHES ONE PARAGRAPH UP: those
# three programs were ALWAYS broken under SCRIP_OPT=0. They were invisible because they were XFAIL, i.e.
# outside the denominator. Promoting a cured marker made three pre-existing bypass failures VISIBLE FOR THE
# FIRST TIME. The number got worse because the instrument got more honest -- never read that as drift.
# Routed to hq_P/ceo per this gate's header, in the landing commit rather than after it.
# ⚠⭐ APPENDED BY hq_P 2026-08-30 (rebase merge; hq_C's attribution block above is KEPT WHOLE and its numbers WIN -- mine were taken on the superseded population 1649, before the XFAIL promotion).
# ⛔ I TRIED TO RAISE zd0 306 -> 307 IN THIS SLOT AND WITHDREW IT. The reason is worth keeping even though the number is gone: I read 307 once and reported it as caused by my landing (the loud
# negative-mark refusal in pl_trail_unwind). That attribution was WRONG -- my arms were not comparable, 307 was PRISTINE against an INCREMENTAL 306 control. Six readings on population 1649 at
# load ~3.6-4.5/16 gave 303, 304, 304, 306, 306, 307: a +/-3 spread WITHIN an arm, so my change's effect was never resolvable against it and two of my three readings sat below the control.
# ⭐ THE STANDING HAZARD, which hq_C's own note above independently confirms from the other side (opt0 HANG 0->17, zd0 HANG 4->33, CRASH(-11) down by about as much): entries near the timeout
# boundary trade CRASH for HANG under load, so these counts track MACHINE LOAD, not only the world. A pin sitting inside that band fails INTERMITTENTLY -- the flaky-bound class, which gets blamed
# on the box and survives for months, unlike a reliably-broken bound fixed on day one. ⛔ Before the next re-pin take MORE THAN ONE reading and record the spread, or the pin is a coin flip.
# ✅⭐⭐ JOINT RESOLUTION (hq_C + hq_P, independently, from opposite ends, within one hour — and BOTH OF US WITHDREW OUR OWN NUMBER).
# hq_P tried to RAISE 306 -> 307 and withdrew it (arms not comparable, spread swallowed the effect).  hq_C TIGHTENED 306 -> 303 and it
# red-ed `make test` for the fleet on the very next honest run.  ⛔ Two opposite errors, one cause: A PIN SITTING INSIDE THE NOISE BAND.
# THE UNION OF OUR READINGS ON THIS ARM: hq_P 303,304,304,306,306,307 (population 1649) and hq_C 301,302,302,303,304 (population 1654)
# -- and the populations ARE comparable here, because all 5 entries the XFAIL promotion added measure zd0_changed=0, contributing zero.
# So the observed band is 301-307 and the pin is set to 308 = band_max + 1.  ⛔ THAT IS NOT SLACK TO HIDE A REGRESSION, IT IS THIS
# DETECTOR'S ACTUAL RESOLUTION STATED OUT LOUD: it cannot see drift smaller than about +/-4 on the zd0 arm, and pinning inside the band
# was giving an illusion of precision that produced false reds instead of findings.  hq_C measured WHY, per entry, 10 runs each on ONE
# binary: arbno_pos_rpos_branch_84 PASS 6/10 + SILENT WRONG ANSWER 4/10 . arbno_pos_rpos_branch_85 PASS 4/10 + WRONG ANSWER 6/10 .
# fence_break_pos_branch_2 PASS 8/10 + SIGSEGV 2/10 . span_pos_rpos_replace_branch_9 PASS 4/10 + SIGSEGV 1/10 + SIGABRT 5/10.
# ⭐ THE COUNT IS A SUM OVER NONDETERMINISTIC UNITS, so `<=` against an exact count is flaky BY CONSTRUCTION and needs no drift to fire.
# ⚠️ THE REAL FIX IS ROUTED, NOT TAKEN: pin the STABLE SUBSET -- count only entries whose bypass verdict is reproducible across N runs,
# and report the flapping set separately.  RULES.md's denominator law says every grader states its DENOMINATOR; the missing half is that
# a grader summing NONDETERMINISTIC units must state its VARIANCE, or it cannot support the comparison it is asked to support.
# ⛔⭐ THE COMMENT BLOCK BELOW SITS *ABOVE* THIS COMMAND ON PURPOSE. hq_C put it between the `--gate \` line and its continuation once, and
# `bash -n` said "syntax ok" -- because the result IS valid bash: two commands instead of one. The pin arguments never reached python3, the
# census fell back to its 1494 default and REFUSED, and line 123 ran as a command ("--pinned-population: command not found"). ⭐ bash -n answers
# "is this parseable", NOT "is this what I meant" -- the command -v class, and a broken line-continuation is invisible to it. RUN THE GATE.
python3 "$HERE/util_census_optimizer_bypass.py" --gate \
  --pinned-population 1654 --pinned-opt0-max 190 --pinned-zd0-max 308
rc=$?

gate_stamp
exit $rc
