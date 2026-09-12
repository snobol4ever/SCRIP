#!/usr/bin/env bash
source "$(dirname "${BASH_SOURCE[0]}")/lib_one_runner.sh" && one_runner_guard "${0##*/}" || exit 2
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
. "$HERE/lib_inventory.sh" 2>/dev/null || { echo "⛔ GATE REFUSES: lib_inventory.sh unloadable" >&2; exit 2; }
. "$HERE/lib_progress.sh" 2>/dev/null || { echo "⛔ GATE REFUSES: lib_progress.sh unloadable -- a run that records nothing is a defect of that run (CEO-331)" >&2; exit 2; }
# ⛔ COUNTED, NOT SWALLOWED, AND NOT FATAL. See lib_progress.sh's header: it never hides the writer's rc and
# the caller decides. Aborting mid-loop would let one bookkeeping failure destroy a real measurement of 90
# programs (the reason gate_score_row is non-fatal); `|| true` would turn "never written" into silence.
PROGRESS_FAILED=0
arizona_progress() { progress_append package arizona icon "$1" "$2" "$3" || PROGRESS_FAILED=$((PROGRESS_FAILED+1)); }
SCRIP="${SCRIP:-$HERE/../scrip}"
RT_SO="$HERE/../out/libscrip_rt.so"
CORPUS="$S4E/corpus"
PKG="$CORPUS/packages/icon/arizona_tests"
SUITE_SUBDIRS="general special"   # ⛔ every subdirectory the package ships; add a new one here, not to a private list elsewhere
# ⛔⭐⭐ THE ONE-ORACLE RULE, APPLIED TO THIS PACKAGE (ceo CEO-527, landed CEO-578 by hq_T 2026-09-11). A program
# whose ground truth is not Arizona ICON but the Arizona DISTRIBUTION has no baseline answer here, so it is OUT of
# the graded denominator and NAMED -- never hidden and never counted either way. The sister runner has read its own
# OUTSIDE_ARIZONA_BASELINE.tsv since CEO-470; this one did not, so two rows the ceo ruled outside weeks ago went on
# printing four red cells (cfuncs m3+m4, extlvals m3+m4) because THE RULING HAD NO READER. ⭐ A ruling landed in a
# file no instrument opens is indistinguishable from a ruling nobody made.
# ⛔ THE KEY IS THE PACKAGE-RELATIVE PATH WITH EXTENSION ("general/cfuncs.icn"), not the bare basename the jcon copy
# uses: this package ships TWO subdirectories, and UNGRADABLE.tsv already keys that way for the same reason -- a
# bare name would let general/io and special/io buy each other's exclusion.
OUTSIDE="$PKG/OUTSIDE_ARIZONA_BASELINE.tsv"
outside_rows() { [ -f "$OUTSIDE" ] && grep -v '^[[:space:]]*#' "$OUTSIDE" | grep -v '^[[:space:]]*$'; }
is_outside_baseline() { [ -f "$OUTSIDE" ] && awk -F"\t" -v n="$1" '$1==n {f=1} END {exit f?0:1}' "$OUTSIDE"; }
outside_reason() { [ -f "$OUTSIDE" ] && awk -F"\t" -v n="$1" '$1==n {print $2; exit}' "$OUTSIDE"; }
OUTSIDE_LIST=""
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
# ⛔⭐ THE BINARY MOVED UNDER THIS BOARD (ceo CEO-524 (1), wired by the coo 2026-09-10 as THE ONE RUNNER).
# The line above proves the binary is CURRENT AT THE START. It cannot see a swap that happens while the board is
# still grading, and the dirty guard downstream reads the tree at WRITE time -- so hq_R's board straddled a binary
# swap and published m3 635/759 into SCORE.md with a CLEAN stamp while the correct 756/759 was refused as dirty.
# ⛔ gate_bin_watch/gate_bin_unmoved ALREADY EXISTED for exactly this (lib_gate.sh, hoisted by hq_S 2026-09-06 out
# of test_corpus_snobol4.sh) AND NO BOARD CALLED THEM -- the instrument was built, hoisted, documented and never
# wired, which is the failure class it was written to catch, one level up. Both artifacts are watched because
# ./scrip is a ~40 KB driver and the emitter lives in the .so: a scrip-only fingerprint is vacuous exactly when an
# emitter change is the thing that moved.
. "$HERE/lib_gate.sh" 2>/dev/null || { echo "⛔ REFUSED TO GRADE rc=2: lib_gate.sh unloadable -- this board cannot tell whether its binary moves under it" >&2; exit 2; }
GATE_NAME=test_icon_arizona_suite gate_bin_watch "$SCRIP" "${RT_DIR:-$HERE/../out}/libscrip_rt.so"
# ⛔⭐ THE TREE MOVED UNDER THIS BOARD, and the stamp that names the tree this row actually graded
# (coo 2026-09-10, COO-54, on the coo's own witness: a pass recorded one corpus hash and a background job
# moved corpus eight seconds later; gate_bin_watch stayed silent and was right to, because the BINARY had
# not moved). One call: gate_tree_watch exports S4E_TREE_AT_START in util_score_row's own vocabulary AND
# takes the baseline, so this board stops stamping HEAD-at-WRITE-time and starts refusing a split reading.
GATE_NAME=test_icon_arizona_suite gate_tree_watch "$(cd "$HERE/../.." && pwd)"
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
    # ⛔⭐ OUR OWN GENERATED CONTAINER IS NOT A SHIPPED PROGRAM (hq_I 2026-09-06, pre-empted rather than
    # walked into). lib_inventory.sh already skips `ALL.*` when it counts shipped, and the ipl package paid
    # for the disagreement: a `find -name '*.icn'` counted our generated ALL.icn as a vendored program, the
    # runner said 852 where the container's own accounting said 851, and a VENDOR score carried a file we
    # wrote. CEO-331 asks for an ALL.csv on this package next, which creates exactly that container here --
    # so the census excludes it BEFORE the file exists, not after the number moves.
    case "$(basename "$icn")" in ALL.*) continue ;; esac
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
MODEREF_NAMES=""
GRADED_NAMES=""
M3_PASS=0; M3_REJECT=0; M3_FAIL=0; M3_CRASH=0; M3_HANG=0
M4_PASS=0; M4_REJECT=0; M4_FAIL=0; M4_CRASH=0; M4_HANG=0
M3_REJECT_NAMES=""; M3_FAIL_NAMES=""; M3_CRASH_NAMES=""; M3_HANG_NAMES=""
M4_REJECT_NAMES=""; M4_FAIL_NAMES=""; M4_CRASH_NAMES=""; M4_HANG_NAMES=""
m3rc=0; m4rc=0

for sub in $SUITE_SUBDIRS; do
SUITE="$PKG/$sub"
[ -d "$SUITE" ] || continue
for std in "$SUITE"/*.std; do
  [ -f "$std" ] || continue
  name=$(basename "$std" .std)
  icn="$SUITE/$name.icn"
  [ -f "$icn" ] || continue
  # ⛔ OUT OF THE DENOMINATOR BEFORE IT IS COUNTED, NEVER SUBTRACTED AFTER: a program excluded after TOTAL++ would
  # ride in the fraction the board publishes and be named as excluded in the same breath, which is two answers.
  # It stays in SHIPPED (POPULATION LAW) and so falls into the GAP, where the UNGRADED/UNGRADABLE split accounts
  # for it -- the mirror check below refuses to let it be excluded here and invisible there.
  if is_outside_baseline "$sub/$name.icn"; then OUTSIDE_LIST="$OUTSIDE_LIST $sub/$name"; continue; fi
  TOTAL=$((TOTAL+1))
  GRADED_NAMES="$GRADED_NAMES $sub/$name"
  exp=$(cat "$std")
  # ⛔⭐ THE PER-MODE REF (ceo CEO-581): a line whose value the INVOCATION determines gets one ref per mode, declared in
  # NAME.moderef beside NAME.std with its oracle receipt, rendered THROUGH THE SHARED SHIM (util_apply_moderef.py imports
  # the harness's own reader) so this runner, the jcon runner and the master cannot disagree about what one means.
  # kwds prints &progname: the shipped .std was cut under "icont kwds.icn; ./kwds" and answers ./kwds, which is what m4
  # reproduces; m3 is handed the SOURCE (scrip --run kwds.icn) and &progname IS argv[0] verbatim, so it answers kwds.icn
  # -- exactly what icont answers for the one-step "icon kwds.icn" (hq_V's 09-11 receipt on jcon's own kwds). Nothing is
  # hidden: the line is graded in full against the answer for THAT invocation, and a sidecar that cannot be rendered
  # REFUSES rc=2 rather than grading a cell against the other mode's string.
  exp3="$exp"; exp4="$exp"
  if [ -f "$SUITE/$name.moderef" ]; then
    for _mm in m3 m4; do
      _want="$RUNDIR/$name.$_mm.want"
      if ! python3 "$HERE/util_apply_moderef.py" "$std" "$name" "$_mm" "$RUNDIR/$name.$_mm.nsub" > "$_want"; then
        echo "⛔ REFUSED TO GRADE rc=2: $name.moderef could not be rendered for $name/$_mm -- a declaration that cannot be applied is not a ref, and grading this cell against the OTHER mode's string would manufacture a red"; exit 2
      fi
      _nsub=$(cat "$RUNDIR/$name.$_mm.nsub" 2>/dev/null || echo 0)
      [ "${_nsub:-0}" -gt 0 ] && MODEREF_NAMES="$MODEREF_NAMES $name:$_mm:$_nsub line(s)"
      if [ "$_mm" = m3 ]; then exp3=$(cat "$_want"); else exp4=$(cat "$_want"); fi
    done
  fi
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
  # ⛔ THE BARE NAME, NOT $icn: &file (and so the &trace showline column) is the source path AS GIVEN, in iconx and in
  # SCRIP alike (measured 2026-09-08 against icont 9.5.25a: "large.icn    :", "sub/large.icn:", "ral/large.icn:" for the
  # bare, subdir and absolute forms). The .std files were cut with the bare name from inside $SUITE; an absolute $icn
  # here read `large` red on a compiler that matched the oracle byte for byte.
  # ⛔⭐ THE EXIT STATUS IS CAPTURED, AND UNTIL 2026-09-09 IT WAS THROWN AWAY (hq_T, CEO-445 item 2). This
  # runner counted PASS / REJECT / FAIL only, so a SIGSEGV and a TIMEOUT both landed in FAIL, indistinguishable
  # from a wrong answer -- while BOTH sibling Icon runners (jcon, ipl) already split CRASH and HANG out. ⭐ The
  # three route to different cures: a wrong answer is a semantics bug to diff against the oracle, a crash is a
  # box or runtime fault to open under gdb, and a hang is neither. Collapsing them costs the reader the first
  # and cheapest classification of the defect, and the board that hides it looks tidier for doing so.
  m3out=$(cd "$SUITE" && timeout "$TIMEOUT" "$SCRIP" --run "$name.icn" < "$stdin_file" 2>&1); m3rc=$?
  # ⛔⭐ ONE ERROR VOICE (CEO-625): SCRIP's error shape is rendered through the Icon equivalence list before the compare.
  m3out=$(printf '%s\n' "$m3out" | python3 "$HERE/util_render_error_voice.py" icon)
  if printf '%s' "$m3out" | grep -q 'parse error'; then
    M3_REJECT=$((M3_REJECT+1)); M3_REJECT_NAMES="$M3_REJECT_NAMES $name"; arizona_progress "$name" m3 REJECT
    [ "$VERBOSE" = 1 ] && echo "  [m3 REJECT] $name"
  elif [ "$m3out" = "$exp3" ]; then
    M3_PASS=$((M3_PASS+1)); arizona_progress "$name" m3 PASS
  elif [ "$m3rc" -eq 124 ]; then
    M3_HANG=$((M3_HANG+1)); M3_HANG_NAMES="$M3_HANG_NAMES $name"; arizona_progress "$name" m3 HANG
    [ "$VERBOSE" = 1 ] && echo "  [m3 HANG] $name"
  elif [ "$m3rc" -ge 128 ]; then
    M3_CRASH=$((M3_CRASH+1)); M3_CRASH_NAMES="$M3_CRASH_NAMES $name"; arizona_progress "$name" m3 CRASH
    [ "$VERBOSE" = 1 ] && echo "  [m3 CRASH rc=$m3rc] $name"
  else
    M3_FAIL=$((M3_FAIL+1)); M3_FAIL_NAMES="$M3_FAIL_NAMES $name"; arizona_progress "$name" m3 FAIL
    [ "$VERBOSE" = 1 ] && echo "  [m3 FAIL] $name"
  fi

  # ── mode 4: --compile (asm to stdout) -> assemble+link libscrip_rt.so -> run ─────────────────────
  # ⛔⭐ THE m4 INVOCATION IS PINNED TO ./NAME, AND THAT IS THIS RUNNER'S OWN GROUND TRUTH (ceo ruling
  # 2026-09-11, "neither ref is wrong, the runner is"; hq_B FINDING-2026-09-11-...-progname). &progname IS
  # argv[0], so a program printing it has an output that is A FUNCTION OF HOW IT WAS INVOKED. This built to
  # `mktemp /tmp/ariz_XXXXXX.bin` and ran it by that absolute path -- a name that matches NO ref cut from any
  # real invocation and CANNOT be made to by choosing a better ref. Upstream's Test-icon, cited as GROUND
  # TRUTH at the head of this file, compiles NAME.icn to NAME and runs ./NAME; that is what the .std files
  # were cut under, so that is what we reproduce. The binary is litter like any other and is removed by name
  # below, both explicitly and by the pre-run snapshot sweep.
  s4=$(mktemp /tmp/ariz_XXXXXX.s); bin4="$SUITE/$name"
  # ⛔ REFUSE rather than overwrite: if a shipped file already owns that name, building over it would destroy
  # tracked corpus content, and the snapshot sweep would NOT remove it (it is not new) -- so the damage would
  # be silent and permanent. No arizona name collides today; this is the guard for the day one does.
  if [ -e "$bin4" ]; then echo "REFUSE(2): $sub/$name -- cannot pin the m4 binary to $bin4, a shipped file already owns that name" >&2; rm -f "$s4"; exit 2; fi
  m4diag=$(cd "$SUITE" && timeout "$TIMEOUT" "$SCRIP" --compile "$name.icn" 2>&1 >"$s4" </dev/null)
  m4out=""
  if [ -s "$s4" ] && [ -f "$RT_SO" ]; then
    if gcc -no-pie "$s4" -L"$HERE/../out" -lscrip_rt -Wl,-rpath,"$HERE/../out" -o "$bin4" 2>/dev/null; then
      m4out=$(cd "$SUITE" && PATH="$SUITE:$PATH" timeout "$TIMEOUT" "$name" < "$stdin_file" 2>&1); m4rc=$?
      m4out=$(printf '%s\n' "$m4out" | python3 "$HERE/util_render_error_voice.py" icon)
    fi
  fi
  if printf '%s\n%s\n%s' "$m4diag" "$m4out" "$(cat "$s4" 2>/dev/null)" | grep -q 'parse error'; then
    M4_REJECT=$((M4_REJECT+1)); M4_REJECT_NAMES="$M4_REJECT_NAMES $name"; arizona_progress "$name" m4 REJECT
    [ "$VERBOSE" = 1 ] && echo "  [m4 REJECT] $name"
  elif [ "$m4out" = "$exp4" ]; then
    M4_PASS=$((M4_PASS+1)); arizona_progress "$name" m4 PASS
  elif [ "$m4rc" -eq 124 ]; then
    M4_HANG=$((M4_HANG+1)); M4_HANG_NAMES="$M4_HANG_NAMES $name"; arizona_progress "$name" m4 HANG
    [ "$VERBOSE" = 1 ] && echo "  [m4 HANG] $name"
  elif [ "$m4rc" -ge 128 ]; then
    M4_CRASH=$((M4_CRASH+1)); M4_CRASH_NAMES="$M4_CRASH_NAMES $name"; arizona_progress "$name" m4 CRASH
    [ "$VERBOSE" = 1 ] && echo "  [m4 CRASH rc=$m4rc] $name"
  else
    M4_FAIL=$((M4_FAIL+1)); M4_FAIL_NAMES="$M4_FAIL_NAMES $name"; arizona_progress "$name" m4 FAIL
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
    # ⛔ THE GAP IS NOT ONE THING AND THIS LIST MUST NOT READ AS IF IT WERE: an outside-baseline name is a RULING
    # with a measurement behind it, and an unannotated name beside it reads as a ref nobody has cut yet.
    *) if is_outside_baseline "$n.icn"; then UNGRADED_NAMES="$UNGRADED_NAMES $n(outside the Arizona baseline: $(outside_reason "$n.icn"))"
       else UNGRADED_NAMES="$UNGRADED_NAMES $n"; fi ;;
  esac
done

echo "=== Arizona Icon suite (upstream 9.5, ${SUITE_SUBDIRS// //}/) — shipped=$SHIPPED graded=$TOTAL gap=$GAP ==="
# ⛔ FAIL NOW MEANS WRONG ANSWER ONLY. Crashes and hangs used to be counted inside it, so m3_fail/m4_fail read
# LOWER than before on the same tree -- that is a re-classification of a population, never a cure, and the PASS
# counts do not move. The CRASH/HANG keys are ADDITIVE on the board line so no existing reader shifts.
echo "mode-3 (--run):      PASS=$M3_PASS REJECT=$M3_REJECT FAIL=$M3_FAIL CRASH=$M3_CRASH HANG=$M3_HANG  / $TOTAL graded"
echo "mode-4 (--compile):  PASS=$M4_PASS REJECT=$M4_REJECT FAIL=$M4_FAIL CRASH=$M4_CRASH HANG=$M4_HANG  / $TOTAL graded"
[ -n "$M3_CRASH_NAMES" ] && echo "m3 CRASH:$M3_CRASH_NAMES"
[ -n "$M3_HANG_NAMES" ] && echo "m3 HANG:$M3_HANG_NAMES"
[ -n "$M4_CRASH_NAMES" ] && echo "m4 CRASH:$M4_CRASH_NAMES"
[ -n "$M4_HANG_NAMES" ] && echo "m4 HANG:$M4_HANG_NAMES"
[ -n "$M3_FAIL_NAMES" ] && echo "m3 FAIL:$M3_FAIL_NAMES"
[ -n "$M4_FAIL_NAMES" ] && echo "m4 FAIL:$M4_FAIL_NAMES"
echo "m3 REJECT ($M3_REJECT):$M3_REJECT_NAMES"
echo "m4 REJECT ($M4_REJECT):$M4_REJECT_NAMES"
# ⛔ UNGRADED IS NOT A FAIL AND NOT A PASS -- it is "never run against the oracle", a third state this
# board must never fold into either count (RULES.md: "measured and clean" vs "never ran" may not share
# an output). Counted as ZERO of the population per Lon's ruling until each is individually resolved.
echo "NOT GRADED ($GAP, of $SHIPPED shipped, zero of population until graded -- the PACKAGE_INVENTORY line below splits these into ungraded=owed vs ungradable=ruled):$UNGRADED_NAMES"
# ⛔ PRINTED WHETHER OR NOT THE LIST IS EMPTY (CEO-409 guardrail 3 on masks): a line that appears only when
# something is excluded tells the reader nothing on the day one is added and everything on the day one is removed.
echo "OUTSIDE_ARIZONA_BASELINE ($(printf '%s' "$OUTSIDE_LIST" | wc -w), out of the graded denominator, named in $OUTSIDE):${OUTSIDE_LIST:- none}"
OUT_UNSHIPPED=""; OUT_UNMIRRORED=""; OUT_STALE=""; OUT_RECHECKED=0; OUT_UNCHECKED=""
while IFS=$'\t' read -r _on _oc _orest; do
    [ -n "$_on" ] || continue
    # A row naming no shipped program is a rename or a leftover, and it withdraws an exclusion SILENTLY.
    [ -f "$PKG/$_on" ] || { OUT_UNSHIPPED="$OUT_UNSHIPPED $_on"; continue; }
    # ⛔ THE MIRROR IS NOT DECORATION: the GAP is split by UNGRADED.tsv/UNGRADABLE.tsv and NOT by this file, so an
    # OUTSIDE row absent from both is excluded by one instrument and invisible to the other -- the population stops
    # summing while every individual file still reads honest.
    if ! grep -q "^$_on"$'\t' "$PKG/UNGRADED.tsv" 2>/dev/null && ! grep -q "^$_on"$'\t' "$PKG/UNGRADABLE.tsv" 2>/dev/null; then
        OUT_UNMIRRORED="$OUT_UNMIRRORED $_on"
    fi
    # ⛔⭐ THE STALENESS ARM, AND ITS SHAPE IS DECIDED BY THE CLASS, NOT COPIED FROM THE SISTER RUNNER. jcon
    # re-measures ORACLE_REFUSES rows by asking icont to compile them again, because there the oracle's refusal IS
    # the premise. Here the oracle ANSWERS both rows; the premise is that WE CANNOT, for a reason outside the
    # language (a library of the distribution's own bin). So the honest re-measure is to put the program to OUR
    # compiler and see whether the premise still holds: if it now reproduces the oracle's answer, the exclusion is
    # a number that can only ever be lowered, which is not a correction either.
    case "$_oc" in
        NEEDS_VENDORED_SOURCE|ORACLE_CONTRACT_NOT_IMPLEMENTED)
            _osub="${_on%%/*}"; _ob="$(basename "$_on" .icn)"
            if [ -f "$PKG/$_osub/$_ob.std" ]; then
                _odat="$PKG/$_osub/$_ob.dat"; _ostdin="/dev/null"; [ -f "$_odat" ] && _ostdin="$_odat"
                _oout=$(cd "$PKG/$_osub" && timeout "$TIMEOUT" "$SCRIP" --run "$_ob.icn" < "$_ostdin" 2>&1)
                OUT_RECHECKED=$((OUT_RECHECKED+1))
                [ "$_oout" = "$(cat "$PKG/$_osub/$_ob.std")" ] && OUT_STALE="$OUT_STALE $_on(m3 matches .std NOW)"
            else OUT_UNCHECKED="$OUT_UNCHECKED $_on(no .std)"; fi ;;
        *) OUT_UNCHECKED="$OUT_UNCHECKED $_on($_oc)" ;;
    esac
done <<EOF
$(outside_rows)
EOF
[ -n "$OUT_UNSHIPPED" ]  && echo "⚠ OUTSIDE_ARIZONA_BASELINE.tsv NAMES NOTHING SHIPPED:$OUT_UNSHIPPED -- a rename or a leftover; the row withdraws an exclusion silently, and this file is not covered by lib_inventory's declared-but-not-shipped refusal except through the mirror"
[ -n "$OUT_UNMIRRORED" ] && echo "⚠ OUTSIDE_ARIZONA_BASELINE.tsv DOES NOT MIRROR the lockdown buckets:$OUT_UNMIRRORED -- named outside the denominator here and in neither UNGRADED.tsv nor UNGRADABLE.tsv, so the gap split below cannot see them; work owed is UNGRADED, a ruling nobody owes work on is UNGRADABLE"
[ -n "$OUT_STALE" ]      && echo "⚠ OUTSIDE_ARIZONA_BASELINE.tsv STALE:$OUT_STALE -- recorded as outside our reach, and we produced the oracle's answer THIS RUN. Move it back into the graded denominator; an exclusion list that can only ever lower the number is not a correction either"
# ⛔ THE AGREEMENT LINE MAY NOT CLAIM MORE THAN WAS MEASURED (hq_T's own failure test on the jcon copy: with the
# oracle unreachable it re-measured zero rows and still printed "agrees with the oracle"). It names what it put to
# the compiler and what it did not.
if [ -z "$OUT_UNSHIPPED$OUT_UNMIRRORED$OUT_STALE" ]; then
    if [ "$OUT_RECHECKED" -gt 0 ]; then
        echo "OUTSIDE_ARIZONA_BASELINE.tsv agrees with the lockdown buckets and with this compiler (re-measured $OUT_RECHECKED row(s) in m3 against their own .std; NOT re-measured:${OUT_UNCHECKED:- none})"
    else
        echo "OUTSIDE_ARIZONA_BASELINE.tsv agrees with the lockdown buckets; NO ROW WAS RE-MEASURED, so every row stands on its recorded reason alone:${OUT_UNCHECKED:- none}"
    fi
fi
# ⛔ BEFORE THE BOARD LINE AND BEFORE ANY SCORE.md WRITE, NEVER AFTER (ceo CEO-524 (1)): a refusal that fires
# after the row is published is an annotation, not a refusal -- the same lesson test_gate_progress_rows_carry_
# the_start_fingerprint.sh was written for. gate_bin_unmoved exits 2 itself when the fingerprint moved.
GATE_NAME=test_icon_arizona_suite gate_bin_unmoved
# ⛔ BEFORE THE FIRST PUBLISHED NUMBER, beside the binary check and for the same reason: an annotation that
# the tree moved is not a refusal -- util_score_row would happily write "graded; HEAD moved during the run"
# onto a number that describes no single tree.
GATE_NAME=test_icon_arizona_suite gate_tree_unmoved
echo "ARIZONA_SUITE_BOARD shipped=$SHIPPED graded=$TOTAL gap=$GAP m3_pass=$M3_PASS m3_reject=$M3_REJECT m3_fail=$M3_FAIL m3_crash=$M3_CRASH m3_hang=$M3_HANG m4_pass=$M4_PASS m4_reject=$M4_REJECT m4_fail=$M4_FAIL m4_crash=$M4_CRASH m4_hang=$M4_HANG"
[ -n "$MODEREF_NAMES" ] && echo "    PER-MODE REF (CEO-581: an invocation-determined line, graded in full against the oracle's answer for THIS invocation; receipt in the .moderef row):$MODEREF_NAMES"
# ⭐ THE PACKAGE LOCKDOWN inventory line, via the shared body (lib_inventory.sh) -- never a second copy
# of the arithmetic. UNGRADABLE.tsv/UNGRADED.tsv beside $PKG (hq_I, corpus a284bcdbb) already split the
# GAP printed above; graded_narrow=0, this suite compares full output, never by error-number-only.
INV_PACKAGE=arizona; INV_DIR="$PKG"; INV_EXT=".icn"
INV_LINE="$(inventory_line "$TOTAL" 0)"
if [ -n "$INV_LINE" ]; then echo "$INV_LINE"; else echo "⚠ inventory refused (above) -- the board line still stands; the inventory does not" >&2; fi
if SPLIT_LINE="$(inventory_split_line)"; then [ -n "$SPLIT_LINE" ] && echo "$SPLIT_LINE"
else echo "⛔ PACKAGE INVENTORY SPLIT REFUSED (rc=2, reason above) -- the classes do not sum to their own buckets, so neither reading is published" >&2; fi
[ "${PROGRESS_FAILED:-0}" -eq 0 ] || echo "⛔ PROGRESS DB: $PROGRESS_FAILED per-program appends FAILED -- this run is not fully recorded (CEO-331); the board lines stand, the table does not" >&2
# ⛔ ONE LEADERBOARD (RULES.md FACT RULE, Lon 2026-09-03 ~16:05: "any run of a test suite by any
# session will update the ONE LEADERBOARD"). This records the board line printed just above into
# .github/SCORE.md -- it RUNS NOTHING, it only writes down what this script already measured.
# ⛔ NON-FATAL BY DESIGN: a bookkeeping failure must never turn a real measurement into a red board,
# because a gate that goes red for a reason unrelated to the code is a gate people route around. It
# warns and names the unrecorded row instead; it has no silent path.
# ⭐ THE INVENTORY CLAUSE RIDES IN THE CELL TOO (util_score_row.py's inventory_clauses(), CEO-316):
# appended verbatim when non-empty, with the runner name immediately after it so `by=`/backtick
# attribution finds it within the reader's 200-char window; absent when inventory_line refused, so
# a refusal never gets transcribed as if it were a measurement.
# ⛔⭐ TWO THINGS THIS --text GOT WRONG UNTIL 2026-09-06 (hq_I), both of which the cell then asserted on
# EVERY run, so neither could be cured by editing SCORE.md:
#   1. DENOMINATOR IS $TOTAL (graded), NOT $SHIPPED. Lon ruled 2026-09-05 "Show measured numbers from
#      running test suites not FLOORS", and the icon cell's own note records that it "read 46/124 ...
#      until now -- a real numerator over a population a third of which had never been run". This line
#      wrote $M3_PASS/$SHIPPED and so REINTRODUCED 47/124, undoing a correction already made by hand.
#   2. $GAP IS NOT "ungraded". Under hq_T's ruled vocabulary UNGRADED means OWED WORK and UNGRADABLE
#      means a RULING; $GAP is just shipped-minus-graded and today is 34 ungradable + 0 ungraded. Saying
#      "34 ungraded" put a flat contradiction beside this cell's own "ungraded=0 ungradable=34".
#      The neutral phrase is the honest one: the inventory clause riding alongside does the splitting.
# ⛔⭐ THE ROW IS THE AND PER PROGRAM WHEN THE MODES DISAGREE (ceo CEO-545, ruled 2026-09-10 on the coo's
# handling; it supersedes the m3-where-they-differ convention retired at ceo-372). THE DEFECT THIS REPLACES,
# measured on the 2026-09-10 19:00 pass: this line passed --suite-pass "$M3_PASS" -- m3's 84 -- into a cell
# whose m4 read 83, so the published row asserted a fraction NO MODE MEASURED and did it silently, every pass,
# whenever the modes differed. The coo corrected it by hand and the runner would have done it again next hour.
# ⭐ THE SIBLING RUNNER FAILED THE OTHER WAY ON THE SAME PASS: jcon handed over two fractions and util_score_row
# REFUSED the write, correctly. One silently wrong, one honestly unwritten, ONE ROOT -- neither could COMPUTE
# the number the rule asks for. gate_and_per_program is that computation, spelled once in lib_gate.sh.
# ⛔ REDS BY NAME, NEVER total-minus-two-pass-counts: a program red in BOTH modes would be subtracted twice.
read -r AND_PASS AND_RED AND_NAMES <<<"$(gate_and_per_program "$TOTAL" "$M3_REJECT_NAMES $M3_FAIL_NAMES $M3_CRASH_NAMES $M3_HANG_NAMES" "$M4_REJECT_NAMES $M4_FAIL_NAMES $M4_CRASH_NAMES $M4_HANG_NAMES")"
if [ -z "${AND_PASS:-}" ]; then
    echo "⛔ SCORE ROW REFUSES (rc=2): gate_and_per_program could not compute the AND per program over TOTAL='$TOTAL' -- the row is that number and this run will not guess one" >&2
    AND_PASS="$M3_PASS"; AND_RED=""; AND_NAMES=""
fi
echo "ARIZONA_AND_PER_PROGRAM and_pass=$AND_PASS of $TOTAL (m3 $M3_PASS · m4 $M4_PASS · union of reds $AND_RED:$AND_NAMES)"
python3 "$HERE/util_score_row.py" write --lang icon --column vendor --suite Arizona --modes m3,m4 --suite-pass "$AND_PASS" --suite-total "$TOTAL" \
    --measurer "${S4E_SEAT:-}" --text "AND per program $AND_PASS/$TOTAL (ceo CEO-545: a program is green only if BOTH modes are; union of reds $AND_RED:$AND_NAMES) · m3 $M3_PASS/$TOTAL · m4 $M4_PASS/$TOTAL graded (of $SHIPPED shipped, $TOTAL graded, $GAP not graded -- the inventory clause splits ungraded=owed from ungradable=ruled, m3_fail=$M3_FAIL m4_fail=$M4_FAIL, reject $M3_REJECT/$M4_REJECT)${INV_LINE:+ · $INV_LINE (\`test_icon_arizona_suite.sh\`)}" \
    || echo "⚠ SCORE.md NOT UPDATED -- record this row by hand (the REFUSED line above says why)"

