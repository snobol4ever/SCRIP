#!/usr/bin/env bash
# ⛔⭐ EVERY BANNER PRODUCER FITS 80 DISPLAY COLUMNS (Lon 2026-09-13, in-chat to cto, routed by cto to all
# seats, verbatim: "That banner printed is nu-formatted and un-readble with wrapping text. Do not show that
# again. Show as a grid." and "Do not show the test suite banner ever again. It is un-readable. Do show a
# grid next time and all other times.").
# ⭐ WHAT THIS GATE IS ACTUALLY FOR, because the obvious reading is the wrong one. The suite banner was
# ALREADY a grid when Lon called it unreadable, and its own docstring said "TWO COLUMNS SO 25 SUITES FIT
# WITHOUT WRAPPING". It measured 83 display columns -- over by exactly THREE -- so a three-character tail
# wrapped under all twenty of its rows and shredded a correctly-formed grid into a block. Gridding harder
# would not have fixed it. A grid is only a grid if it FITS; width is the property, and this gate is the
# only thing that holds it. Measured on the tree the day the rule was cut: 26 of the banner's 42 lines
# exceeded 80 columns, rendering ~70 wrapped rows.
# ⛔⛔ DISPLAY COLUMNS, NEVER BYTES, AND THIS IS THE TRAP THAT WOULD HAVE MADE THIS GATE FICTION. The
# separator and progress-bar lines are multibyte: `awk length()` and python `len()` read 240 for a rule
# that is EXACTLY 80 columns wide. A byte-width gate flags the compliant lines as the worst offenders and
# misses every real one -- it would be confidently, precisely wrong in both directions at once. There is
# ⛔⭐ DISPLAY COLUMNS OR THE GATE IS FICTION (cto 2026-09-13, asking for this sentence in this header
# because the next person to touch this check will reach for length() by reflex): 240 vs 80 is the whole
# argument -- the separator line is 240 BYTES and exactly 80 COLUMNS. There is
# one authority for the answer, `dw()` in .github/scripts/util_suite_banner.py, which already handles the
# three ways len() lies here (wide chars, variation selectors, regional-indicator pairs). It is IMPORTED,
# never reimplemented: a second width function beside it is the same defect as a second roster.
# ⛔ REFUSES rc=2 WHEN IT CANNOT MEASURE, never skip-as-success: a width gate that cannot find a producer
# must not report that the producer is narrow.
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/../.." && pwd)"
GH="$ROOT/.github/scripts"
LIMIT="${BANNER_COL_LIMIT:-80}"
SB="$GH/util_suite_banner.py"
# ⛔ DERIVED FROM THIS SCRIPT'S OWN LOCATION, NEVER FROM "$ROOT/SCRIP" (CEO-663, and
# test_gate_no_worktree_blind_subject.sh caught this gate the first time it ran in preflight -- ratchet
# 69 > 68). These two files are this script's SIBLINGS, so $HERE already names them; spelling the repo
# out as $ROOT/SCRIP makes the gate grade the checkout it was NAMED after instead of the checkout it is
# RUNNING IN, so in a worktree it silently measures the wrong tree and still prints a confident verdict.
SR="$HERE/util_score_row.py"
MSG="$HERE/s4e_msg.sh"
for f in "$SB" "$SR" "$MSG"; do
  [ -f "$f" ] || { echo "⛔ REFUSE rc=2: cannot measure -- producer absent: $f"; exit 2; }
done
TMP="$(mktemp -d)"; trap 'rm -rf "$TMP"' EXIT
pass=0; fail=0
# ⭐ THE ONE CHECKER. Reads a file, prints every line wider than LIMIT with its measured width, and exits
# 1 if any exists. `--expect-wide` inverts it, which is what makes the detector proofs below possible:
# an arm that can only ever report "narrow" proves nothing about a producer that printed nothing at all.
cat > "$TMP/width.py" <<'PYEOF'
import sys, os, io, contextlib, importlib.util
# ⛔ THE BANNER SCRIPT HAS NO __main__ GUARD, so importing it PRINTS A WHOLE BANNER. Unredirected, that
# banner lands in this gate's own output and in the file some arms grade -- a checker that contaminates the
# thing it measures. Swallow it: we want dw(), not a banner.
def _load(path):
    spec = importlib.util.spec_from_file_location("_sb", path)
    m = importlib.util.module_from_spec(spec)
    buf = io.StringIO()
    with contextlib.redirect_stdout(buf), contextlib.redirect_stderr(buf):
        try: spec.loader.exec_module(m)
        except SystemExit: pass
    return m
dw = _load(os.environ["SB_PATH"]).dw
limit = int(sys.argv[2]); expect_wide = (len(sys.argv) > 3 and sys.argv[3] == "--expect-wide")
lines = open(sys.argv[1], encoding="utf-8").read().split("\n")
if lines and lines[-1] == "": lines.pop()
over = [(i + 1, dw(l), l) for i, l in enumerate(lines) if dw(l) > limit]
if not lines:
    print("  REFUSE: producer emitted ZERO lines -- a width check over nothing is not a measurement"); sys.exit(2)
# ⭐ DENOMINATOR CELL, ALWAYS (hq_I via cto 2026-09-13): the line COUNT is printed even when nothing is
# over, because a grid of green rows is exactly as misleading as a green banner if the rows that could not
# run are simply absent. "0 over" means nothing until you know 0 of how many.
print("  measured %d line(s), %d over, widest %d col(s), limit %d"
      % (len(lines), len(over), max(dw(l) for l in lines), limit))
# ⛔ TRUNCATE CELLS, NEVER ROWS (the cfo's rule, via cto 2026-09-13, paid for at 26 minutes: it piped a
# board through `tail -30`, got a truthful verdict over a list it could not see, and had to re-run to learn
# whether the 12 reds were its own). A verdict with no subjects is the same defect as a wrapped block. So a
# long line is CLIPPED, and the row COUNT is never silently cut: if this listing elides anything it says how
# many it elided, and every offender is named when the gate is red.
SHOW = len(over) if os.environ.get("BANNER_GATE_SHOW_ALL") else min(len(over), 12)
for n, w, l in over[:SHOW]:
    print("    line %-3d %4d cols  %s" % (n, w, l[:58] + ("\u2026" if len(l) > 58 else "")))
if len(over) > SHOW:
    print("    ... and %d more over-wide line(s) NOT listed (BANNER_GATE_SHOW_ALL=1 lists every one)"
          % (len(over) - SHOW))
if expect_wide: sys.exit(0 if over else 1)
sys.exit(1 if over else 0)
PYEOF
export SB_PATH="$SB"
arm() { # arm <name> <file> [--expect-wide]
  printf '%-58s' "$1"
  out="$(python3 "$TMP/width.py" "$2" "$LIMIT" ${3:-} 2>&1)"; rc=$?
  if [ $rc -eq 2 ]; then echo "REFUSE"; echo "$out"; exit 2; fi
  if [ $rc -eq 0 ]; then echo "ok"; pass=$((pass+1)); else echo "FAIL"; fail=$((fail+1)); fi
  echo "$out"
}
echo "=== test_gate_banner_fits_eighty_columns.sh  (limit ${LIMIT} DISPLAY columns)"
# ⭐ ARM 5 FIRST, AND IT IS THE LOAD-BEARING ONE: prove the INSTRUMENT before trusting any verdict it gives.
# A gate measuring bytes would pass arms 1-3 on a producer that wraps and fail them on one that does not.
printf '%-58s' "arm5 dw() measures columns not bytes"
python3 - <<'PYEOF' || { echo "FAIL"; exit 2; }
import os, sys, io, contextlib, importlib.util
spec = importlib.util.spec_from_file_location("_sb", os.environ["SB_PATH"])
m = importlib.util.module_from_spec(spec)
with contextlib.redirect_stdout(io.StringIO()), contextlib.redirect_stderr(io.StringIO()):
    try: spec.loader.exec_module(m)
    except SystemExit: pass
rule = "═" * 80                      # 80 box-drawing chars = 240 BYTES, 80 columns
assert len(rule.encode()) == 240, len(rule.encode())
assert m.dw(rule) == 80, "box rule read %d, want 80" % m.dw(rule)
assert m.dw("x" * 83) == 83
assert m.dw("✅") == 2, "wide char must read 2"
assert m.dw("❄️") == 2, "variation selector must read 2, not 3"
PYEOF
echo "ok  (80 box chars = 240 bytes read as 80 cols; a byte gate would read 240)"; pass=$((pass+1))
# --- scratch fixtures -------------------------------------------------------
mkdir -p "$TMP/home/.github"
cp "$ROOT/.github/SUITES.tsv" "$TMP/home/.github/SUITES.tsv" 2>/dev/null || true
cp "$ROOT/.github/DEFERRED.tsv" "$TMP/home/.github/DEFERRED.tsv" 2>/dev/null || true
cp "$ROOT/.github/SCORE.md" "$TMP/home/.github/SCORE.md" 2>/dev/null || true
[ -s "$TMP/home/.github/SUITES.tsv" ] || { echo "⛔ REFUSE rc=2: no SUITES.tsv to grade a banner over"; exit 2; }
# ARM 1 -- the suite banner
S4E_SUITES_TSV="$TMP/home/.github/SUITES.tsv" S4E_DEFERRED_TSV="$TMP/home/.github/DEFERRED.tsv" \
  python3 "$SB" --plain > "$TMP/a1.txt" 2>&1
arm "arm1 util_suite_banner.py grid" "$TMP/a1.txt"
# ARM 2 -- the progress block
S4E_HOME="$TMP/home" python3 "$SR" progress > "$TMP/a2.txt" 2>&1
arm "arm2 util_score_row.py progress" "$TMP/a2.txt"
# ⭐ ARM 3 GRADES THE REAL ARTIFACT, NOT A PROBE OF IT. An earlier draft of this gate added a
# `banner-widthprobe` subcommand so the check would be fast; that would have graded a second code path that
# nobody reads, and left the banner Lon actually sees ungraded -- the exact "instrument answers a narrower
# question than you think you asked" shape. The real banner costs ~15s and is hermetic here: S4E_POST
# redirects the postoffice and S4E_BANNER_NO_BOARD=1 stops it writing BOARD.md, so it touches no shared state.
grep -q 's4e_fit' "$MSG" || {
  echo "⛔ REFUSE rc=2: s4e_msg.sh has no width fitter (s4e_fit) -- the verdict grid is gone, and a width"
  echo "   check over a banner nothing fits would report narrow lines as a property rather than an accident"; exit 2; }
# ⛔ THE MAILBOX MUST BE A REAL MAILBOX, inbox AND ALL. The first draft of this fixture created
# $TMP/po/<seat>/ without inbox/, so s4e_assert_box refused at line 3 and this arm graded a TEN-line refusal
# instead of the banner -- and PASSED, because ten short lines are all under 80. The gate was green about
# nothing. That is the empty-denominator shape this same gate warns about two arms down, met while building it.
# ⛔ THE SEAT IS DERIVED FROM THE ROOT PATH inside s4e_msg.sh and there is no subcommand that prints it,
# so this fixture does NOT guess: it mirrors the mailbox NAMES the live postoffice already has. Hard-coding
# "hq_B" here would make the gate pass on this root and refuse on every other seat's -- a population that
# never announces what it left out, which is the defect this tree files most often.
mkdir -p "$TMP/po/claims" "$TMP/po/tasks"
_made=0
for _d in /home/resources/postoffice/*/; do
  [ -d "$_d/inbox" ] || continue; mkdir -p "$TMP/po/$(basename "$_d")/inbox"; _made=$((_made+1))
done
[ "$_made" -gt 0 ] || { echo "⛔ REFUSE rc=2: no mailbox names to mirror from the live postoffice"; exit 2; }
head -2 /home/resources/postoffice/MODE > "$TMP/po/MODE" 2>/dev/null || echo NONET > "$TMP/po/MODE"
: > "$TMP/po/QUEUE.tsv"; : > "$TMP/po/BOARD.md"
S4E_POST="$TMP/po" S4E_BANNER_NO_BOARD=1 timeout 300 bash "$MSG" banner > "$TMP/a3.txt" 2>&1
[ -s "$TMP/a3.txt" ] || { echo "⛔ REFUSE rc=2: the banner printed nothing -- cannot measure its width"; exit 2; }
# ⛔ AND IT MUST BE A WHOLE BANNER, not a refusal that happens to be narrow. A width gate cannot tell a
# compliant banner from an early exit -- both are "no line over 80" -- so the population is asserted BY NAME
# before the verdict is trusted: the suite grid, the PROGRESS line and the verdict rule must all be present.
for _need in 'SUITES,' 'PROGRESS ' '════'; do
  grep -q -- "$_need" "$TMP/a3.txt" || { echo "⛔ REFUSE rc=2: the captured banner is missing '$_need' -- it is a"
    echo "   short-circuit, not a banner, and grading its width would be green about nothing:"
    sed -n '1,6p' "$TMP/a3.txt" | sed 's/^/     /'; exit 2; }
done
_n3=$(wc -l < "$TMP/a3.txt"); [ "$_n3" -ge 20 ] || { echo "⛔ REFUSE rc=2: banner is only $_n3 lines; expected >=20"; exit 2; }
arm "arm3 s4e_msg.sh banner, whole output" "$TMP/a3.txt"
# ⭐ ARM 4 -- THE DETECTOR PROOF, and it is not decoration. Arms 1-3 are satisfied by a producer that
# prints nothing, or by a checker that always says "narrow". This fixture MUST be caught, or the three
# arms above are passing over nothing -- the empty-denominator shape.
{ echo "short line"; printf 'x%.0s' $(seq 1 $((LIMIT + 1))); echo; } > "$TMP/a4.txt"
arm "arm4 detector: an over-wide line IS caught" "$TMP/a4.txt" --expect-wide
echo "--- arms pass=$pass fail=$fail"
[ "$fail" -eq 0 ] || { echo "⛔ GATE RED: a banner producer emits lines wider than $LIMIT display columns."; exit 1; }
echo "✅ GATE GREEN: every banner producer fits $LIMIT display columns."
