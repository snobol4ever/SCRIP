#!/usr/bin/env bash
# test_gate_score_unreadable_package_is_marked.sh — A READING FAILURE MUST NOT PUBLISH AS A MEASUREMENT.
# Row score-a-vendor-cell-reporting-counts-not-fractions-publishes-a-reading-failure-as-a-measured-zero
# (minted by hq_I 2026-09-05 off their ipl fold; cured by hq_T the same day).
#
# THE DEFECT THIS PINS: counted_fractions() already reported an unreadable package loudly -- it named it and
# quoted the cell back. But `got` is a dict and never None, so language_progress()'s `if got is None` raised the
# language-level flag ONLY when the WHOLE cell failed to parse. A per-package reading failure flowed into P/T as
# an ordinary zero and the percent published with NO marking: icon's ipl read 0/851 while its suite had
# run-graded 34/60, and had done for as long as that cell reported counts.
#
# ⛔⭐ WHAT THIS GATE DELIBERATELY DOES **NOT** REQUIRE, because the obvious test is backwards: it does NOT
# require the unreadable package to stop counting zero. Dropping it from both sides was the original cure's own
# first draft and it was measured moving icon 8% -> 48% in a single edit, purely because a cell had become
# unreadable. Excluding the unknown is the OPTIMISTIC reading; a floor is the conservative one. So the zero stays
# and the gate PINS it -- what changes is that the number is now MARKED as a floor instead of presented as a
# measurement. An unreadable cell must never be able to raise a score.
set -u
GATE_NAME=test_gate_score_unreadable_package_is_marked
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
refuse() { echo "⛔ REFUSED(2) [$GATE_NAME]: $*" >&2; exit 2; }
[ -f "$HERE/util_score_row.py" ] || refuse "no util_score_row.py beside this gate"
cd "$HERE" || refuse "cannot cd to $HERE"
# ⛔ THE HEREDOC DELIMITER IS QUOTED and the path crosses by ENVIRONMENT, never by interpolation -- this repo has
# measured the alternative three times in one day (an unquoted heredoc hands the shell the whole program).
export GATE_NAME
python3 - <<'PY'
import importlib.util, os, sys
spec = importlib.util.spec_from_file_location("usr", "util_score_row.py")
m = importlib.util.module_from_spec(spec); spec.loader.exec_module(m)
G = os.environ["GATE_NAME"]
def red(msg):  print("⛔ GATE RED [%s]: %s" % (G, msg)); sys.exit(1)
def refuse(msg): print("⛔ REFUSED(2) [%s]: %s" % (G, msg), file=sys.stderr); sys.exit(2)
# hq_I's own witness, verbatim in shape: a package clause reporting COUNTS where a fraction is required.
cell = ("arizona 46/124 · jcon_tests 45/91 · ipl compile_pass=544 compile_fail=307 "
        "run m3 34/60 m4 34/60")
got, work = m.counted_fractions("icon", cell)
if not [w for w in work if "UNREADABLE" in str(w)]:
    refuse("the fixture no longer reproduces an UNREADABLE package -- this gate can no longer measure its own "
           "subject, which is not the same as the defect being cured")
# ARM 1 -- the signal reaches the caller at all. This is the whole defect: it was computed and then dropped.
if not getattr(got, "unreadable", ()):
    red("counted_fractions reported UNREADABLE in its workings but the caller cannot see it -- a per-package "
        "reading failure still flows into the percent as an ordinary zero, with no marking")
# ARM 2 -- and the conservative zero SURVIVES the cure. Pinned because the tempting fix breaks it.
if got.get(851) != 0:
    red("the unreadable package no longer counts zero over its population (got %r). An unreadable cell must "
        "never be able to RAISE a score -- measured at icon 8%% -> 48%% when this was tried" % (got.get(851),))
# ARM 3 -- a READABLE cell must not be marked. A gate that only proves the alarm fires cannot tell an alarm
# from a stuck bell.
clean = "arizona 46/124 · jcon_tests 45/91 · ipl 34/851"
cgot, _cw = m.counted_fractions("icon", clean)
if getattr(cgot, "unreadable", ()):
    red("a fully readable cell was reported as unreadable (%r) -- the marker is stuck on" % (cgot.unreadable,))
# ARM 4 -- the published line explains the glyph it prints. A marker nobody can decode is not a marking.
src = open("util_score_row.py", encoding="utf-8").read()
if "! = a package in that cell could NOT BE READ" not in src:
    red("the progress legend does not explain the `!` marker -- a glyph with no legend is not a marking")
print("✅ GATE PASS [%s]: an unreadable package marks its language as a FLOOR (%s), still counts zero, a clean "
      "cell stays unmarked, and the legend explains the glyph" % (G, ", ".join(got.unreadable)))
PY
