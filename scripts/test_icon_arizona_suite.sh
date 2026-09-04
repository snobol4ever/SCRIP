#!/usr/bin/env bash
# test_icon_arizona_suite.sh — grade SCRIP m3 (--run) + m4 (--compile+link) against the vendored
# official Arizona Icon test suite (corpus/packages/icon/arizona_tests, upstream 9.5).
#
# GROUND TRUTH FOR THE CONTRACT: upstream's own general/Test-icon — compile NAME.icn, feed NAME.dat
# as stdin if present else /dev/null, capture stdout+stderr combined, diff against NAME.std. This
# script reproduces that exactly for SCRIP's two native execution modes instead of icont/iconx.
#
# ⛔ POPULATION LAW (Lon, ruled 2026-09-04, routed via hq_B): the counted population is every .icn
# this package SHIPS across ALL its subdirectories, not merely the subset that happens to carry a
# .std oracle reference today. A shipped program with no .std is UNGRADED (counts as ZERO of the
# population, never PASS) until it is brought into the graded set -- it may NEVER be silently
# excluded from the denominator (RULES.md THE INSTRUMENT LAWS: "names beside every count"; the
# earlier form of this script hardcoded "of 99 vendored" in its own banner -- itself wrong and a
# textbook instance of the defect this law now forbids). SHIPPED/GRADED/GAP are computed fresh every
# run, never hand-maintained, and the GAP is named, not merely counted.
#
# THREE OUTCOMES OVER THE GRADED SET, NEVER CONFLATED (SCRIP Icon requires an explicit ';' between
# bare statements and does zero newline processing — RULES.md FACT RULE,
# test_gate_icn_semicolon_required.sh — so these unmodified upstream standard-Icon-dialect programs
# are expected to hit this in large numbers):
#   PASS   — parses, runs, output byte-identical to .std.
#   REJECT — fails to *parse* at all ("parse error" from SCRIP, always on stderr). Named per-file,
#            counted separately, never silently folded into FAIL.
#   FAIL   — parses and runs, output does not match .std. The genuine correctness signal.
#
# Usage: bash scripts/test_icon_arizona_suite.sh [-v]   (-v prints every REJECT/FAIL name inline)
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 sibling root
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
. "$HERE/lib_flag_gate.sh" 2>/dev/null || { echo "⛔ GATE REFUSES: lib_flag_gate.sh unloadable" >&2; exit 2; }
SCRIP="${SCRIP:-$HERE/../scrip}"
RT_SO="$HERE/../out/libscrip_rt.so"
CORPUS="$S4E/corpus"
PKG="$CORPUS/packages/icon/arizona_tests"
SUITE_SUBDIRS="general special"   # ⛔ every subdirectory the package ships; add a new one here, not to a private list elsewhere
TIMEOUT="${TIMEOUT:-8}"
VERBOSE=0
if [ $# -gt 0 ]; then
  case "$1" in
    -v) VERBOSE=1 ;;
    *) flaggate_reject "$1" "-v" ;;
  esac
fi

# ⛔ A MISSING PREREQUISITE IS A REFUSAL (rc=2), NEVER A GREEN EXIT (RULES.md FACT RULE).
if [ ! -d "$PKG" ]; then
  echo "⛔ GATE REFUSES: corpus subtree missing: $PKG" >&2
  echo "   vendor /home/resources/icon-master/tests/{general,special} into it first." >&2
  exit 2
fi
if [ ! -x "$SCRIP" ]; then
  echo "⛔ GATE REFUSES: scrip not built at $SCRIP" >&2
  exit 2
fi
# ⛔⭐ STALE-BINARY PREFLIGHT (row harness-and-ladder-runner-refuse-on-a-stale-binary-like-the-artifact-regen-
# does, ceo -> hq_T 2026-09-04). The line above proves a binary EXISTS; this one proves it is the binary this
# tree describes. ceo's witness, twice on 2026-09-04: a 10:57 binary graded at 14:03 read RED, then GREEN after
# an incremental make -- a vendor board is exactly where that is least visible, because a plausible all-FAIL
# table is this class's normal output. NO LOGIC HERE: util_require_fresh.sh sources gate_require_fresh from
# lib_gate.sh, the ONE authority (hq_B 4c7253e99) -- never a second copy of the staleness rule.
"$HERE/util_require_fresh.sh" --gate test_icon_arizona_suite "$SCRIP" "${RT_DIR:-$HERE/../out}/libscrip_rt.so" || exit 2
# ⛔ EVERY PROGRAM RUNS IN A SCRATCH CWD, NEVER THE CALLER'S (seat02 -> hq_T 2026-09-04): general/fncs1.icn and
# general/checkc.icn both `open("foo.baz","w")`, so any run whose cwd was the package dir left an untracked foo.baz in
# corpus/packages/icon/arizona_tests/general/ -- and that litter blocked a seat's SCORE.md landing on util_score_row's
# dirty-tree refusal. A grader that writes into the tree it grades is the same defect as a gate that edits the artifact
# it measures. Everything a program READS is reached by absolute path ($SUITE, $stdin_file, $SCRIP are all absolute), so
# nothing it reads moves; only what it WRITES lands here, and dies with the trap.
RUNDIR="$(mktemp -d "${TMPDIR:-/tmp}/ariz_run.XXXXXX")" || { echo "⛔ GATE REFUSES: mktemp failed" >&2; exit 2; }
trap 'rm -rf "$RUNDIR"' EXIT

# ── SHIPPED: every .icn under every subdirectory this package ships, computed fresh, never hand-pinned.
SHIPPED=0
SHIPPED_NAMES=""
for sub in $SUITE_SUBDIRS; do
  d="$PKG/$sub"
  [ -d "$d" ] || continue
  for icn in "$d"/*.icn; do
    [ -f "$icn" ] || continue
    SHIPPED=$((SHIPPED+1))
    SHIPPED_NAMES="$SHIPPED_NAMES $sub/$(basename "$icn" .icn)"
  done
done

# ── PRE-RUN SNAPSHOT (for litter cleanup below): CWD fidelity means a graded program's relative file
# writes (fncs1.icn's "foo.baz", io.icn's "./tmp1"/"./tmp2", etc.) now land for real in $SUITE instead of
# silently failing elsewhere. Recorded per-subdir BEFORE grading so anything new after is unambiguously
# this run's litter, not pre-existing content -- removed by plain `rm`, never `git clean` (no destructive
# git verb needed or wanted for a known, self-caused, named set of new files).
PRESNAP_FILE=$(mktemp)
for sub in $SUITE_SUBDIRS; do
  d="$PKG/$sub"
  [ -d "$d" ] || continue
  ( cd "$d" && ls -A ) | sed "s|^|$sub/|" >> "$PRESNAP_FILE"
done

TOTAL=0
GRADED_NAMES=""
M3_PASS=0; M3_REJECT=0; M3_FAIL=0
M4_PASS=0; M4_REJECT=0; M4_FAIL=0
M3_REJECT_NAMES=""; M3_FAIL_NAMES=""
M4_REJECT_NAMES=""; M4_FAIL_NAMES=""

for sub in $SUITE_SUBDIRS; do
SUITE="$PKG/$sub"
[ -d "$SUITE" ] || continue
for std in "$SUITE"/*.std; do
  [ -f "$std" ] || continue
  name=$(basename "$std" .std)
  icn="$SUITE/$name.icn"
  [ -f "$icn" ] || continue
  TOTAL=$((TOTAL+1))
  GRADED_NAMES="$GRADED_NAMES $sub/$name"
  exp=$(cat "$std")
  dat="$SUITE/$name.dat"
  stdin_file="/dev/null"
  [ -f "$dat" ] && stdin_file="$dat"

  # ── mode 3: --run ──────────────────────────────────────────────────────────────────────────────
  # ⛔ CWD FIDELITY (RULES.md THE INSTRUMENT LAWS): upstream's own Test-icon runs every program with the
  # test directory AS its CWD (that's how relative opens like open("gc1.icn") and io.icn's "./tmp1"/
  # "./tmp2" resolve in the real suite). A harness that runs from elsewhere silently fails every relative-
  # path program with a compiler-looking error that is actually the HARNESS diverging from upstream
  # fidelity -- not a SCRIP defect. cd into $SUITE (never $RUNDIR here: RUNDIR is an empty scratch dir,
  # so a relative read like fncs1.icn's open("gc1.icn") would silently fail to find its sibling file --
  # confirmed directly, see FINDING-2026-09-04-seat02-icon-arizona-population-law-cwd-fidelity-and-fresh-census.md).
  # Litter this creates in $SUITE is swept by the PRE-RUN SNAPSHOT diff below, so fidelity costs nothing.
  m3out=$(cd "$SUITE" && timeout "$TIMEOUT" "$SCRIP" --run "$icn" < "$stdin_file" 2>&1)
  if printf '%s' "$m3out" | grep -q 'parse error'; then
    M3_REJECT=$((M3_REJECT+1)); M3_REJECT_NAMES="$M3_REJECT_NAMES $name"
    [ "$VERBOSE" = 1 ] && echo "  [m3 REJECT] $name"
  elif [ "$m3out" = "$exp" ]; then
    M3_PASS=$((M3_PASS+1))
  else
    M3_FAIL=$((M3_FAIL+1)); M3_FAIL_NAMES="$M3_FAIL_NAMES $name"
    [ "$VERBOSE" = 1 ] && echo "  [m3 FAIL] $name"
  fi

  # ── mode 4: --compile (asm to stdout) -> assemble+link libscrip_rt.so -> run ─────────────────────
  s4=$(mktemp /tmp/ariz_XXXXXX.s); bin4=$(mktemp /tmp/ariz_XXXXXX.bin); rm -f "$bin4"
  m4diag=$(cd "$RUNDIR" && timeout "$TIMEOUT" "$SCRIP" --compile "$icn" 2>&1 >"$s4" </dev/null)
  m4out=""
  if [ -s "$s4" ] && [ -f "$RT_SO" ]; then
    if gcc -no-pie "$s4" -L"$HERE/../out" -lscrip_rt -Wl,-rpath,"$HERE/../out" -o "$bin4" 2>/dev/null; then
      m4out=$(cd "$SUITE" && timeout "$TIMEOUT" "$bin4" < "$stdin_file" 2>&1)
    fi
  fi
  if printf '%s\n%s\n%s' "$m4diag" "$m4out" "$(cat "$s4" 2>/dev/null)" | grep -q 'parse error'; then
    M4_REJECT=$((M4_REJECT+1)); M4_REJECT_NAMES="$M4_REJECT_NAMES $name"
    [ "$VERBOSE" = 1 ] && echo "  [m4 REJECT] $name"
  elif [ "$m4out" = "$exp" ]; then
    M4_PASS=$((M4_PASS+1))
  else
    M4_FAIL=$((M4_FAIL+1)); M4_FAIL_NAMES="$M4_FAIL_NAMES $name"
    [ "$VERBOSE" = 1 ] && echo "  [m4 FAIL] $name"
  fi
  rm -f "$s4" "$bin4"
done
done

# ⛔ CWD FIDELITY'S OWN SIDE EFFECT (found this session): see the PRE-RUN SNAPSHOT note above -- this
# leaves real litter in a tracked corpus tree on every run, which would also trip the score-writer's own
# dirty-tree refusal below (CEO-174) forever, not just this once. Removed by NAME against the snapshot
# taken before grading started -- never `git clean` (that verb is repo-wide-destructive by nature; this
# harness knows exactly which files it just caused to exist and removes only those).
for sub in $SUITE_SUBDIRS; do
  d="$PKG/$sub"
  [ -d "$d" ] || continue
  ( cd "$d" && ls -A ) | sed "s|^|$sub/|" > "${PRESNAP_FILE}.post"
  comm -13 <(sort "$PRESNAP_FILE") <(sort "${PRESNAP_FILE}.post") | while IFS= read -r new; do
    [ -n "$new" ] && rm -f -- "$PKG/$new"
  done
done
rm -f "$PRESNAP_FILE" "${PRESNAP_FILE}.post"

# ── GAP: shipped minus graded, by set difference on the "$sub/$name" identity -- named, never a bare count.
GAP=$((SHIPPED-TOTAL))
UNGRADED_NAMES=""
for n in $SHIPPED_NAMES; do
  case " $GRADED_NAMES " in
    *" $n "*) ;;
    *) UNGRADED_NAMES="$UNGRADED_NAMES $n" ;;
  esac
done

echo "=== Arizona Icon suite (upstream 9.5, ${SUITE_SUBDIRS// //}/) — shipped=$SHIPPED graded=$TOTAL gap=$GAP ==="
echo "mode-3 (--run):      PASS=$M3_PASS REJECT=$M3_REJECT FAIL=$M3_FAIL  / $TOTAL graded"
echo "mode-4 (--compile):  PASS=$M4_PASS REJECT=$M4_REJECT FAIL=$M4_FAIL  / $TOTAL graded"
[ -n "$M3_FAIL_NAMES" ] && echo "m3 FAIL:$M3_FAIL_NAMES"
[ -n "$M4_FAIL_NAMES" ] && echo "m4 FAIL:$M4_FAIL_NAMES"
echo "m3 REJECT ($M3_REJECT):$M3_REJECT_NAMES"
echo "m4 REJECT ($M4_REJECT):$M4_REJECT_NAMES"
# ⛔ UNGRADED IS NOT A FAIL AND NOT A PASS -- it is "never run against the oracle", a third state this
# board must never fold into either count (RULES.md: "measured and clean" vs "never ran" may not share
# an output). Counted as ZERO of the population per Lon's ruling until each is individually resolved.
echo "UNGRADED ($GAP, of $SHIPPED shipped, zero of population until graded):$UNGRADED_NAMES"
echo "ARIZONA_SUITE_BOARD shipped=$SHIPPED graded=$TOTAL gap=$GAP m3_pass=$M3_PASS m3_reject=$M3_REJECT m3_fail=$M3_FAIL m4_pass=$M4_PASS m4_reject=$M4_REJECT m4_fail=$M4_FAIL"
# ⛔ ONE LEADERBOARD (RULES.md FACT RULE, Lon 2026-09-03 ~16:05: "any run of a test suite by any
# session will update the ONE LEADERBOARD"). This records the board line printed just above into
# .github/SCORE.md -- it RUNS NOTHING, it only writes down what this script already measured.
# ⛔ NON-FATAL BY DESIGN: a bookkeeping failure must never turn a real measurement into a red board,
# because a gate that goes red for a reason unrelated to the code is a gate people route around. It
# warns and names the unrecorded row instead; it has no silent path.
python3 "$HERE/util_score_row.py" write --lang icon --column vendor --suite Arizona --modes m3,m4 \
    --measurer "${S4E_SEAT:-}" --text "m3 $M3_PASS/$SHIPPED · m4 $M4_PASS/$SHIPPED (of $SHIPPED shipped, $TOTAL graded, $GAP ungraded, m3_fail=$M3_FAIL m4_fail=$M4_FAIL, reject $M3_REJECT/$M4_REJECT, \`test_icon_arizona_suite.sh\`)" \
    || echo "⚠ SCORE.md NOT UPDATED -- record this row by hand (the REFUSED line above says why)"

