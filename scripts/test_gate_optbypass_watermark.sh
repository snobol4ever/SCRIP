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

python3 "$HERE/util_census_optimizer_bypass.py" --gate \
  --pinned-population 1494 --pinned-opt0-max 176 --pinned-zd0-max 291
rc=$?

gate_stamp
exit $rc
