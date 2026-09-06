#!/usr/bin/env bash
# util_cut_icon_ipl_refs.sh -- STEP 1 of task icon-ipl-851-run-graded-against-iconx-refs-and-cured-by-class:
# cut a RUN ref (NAME.std) from the real Icon oracle (icon_bin, lib_oracle_flags.sh -- the ONE authority,
# never re-derived here) for every corpus/packages/icon/ipl/progs/*.icn with a deterministic, input-free
# `procedure main`. Same discipline as util_ref_mint.sh's SNOBOL4 LIVE/DEAD_REPORT/EMPTY classes, adapted:
# CENSUS ONLY by default (classifies, writes nothing); --apply mints a .std beside every LIVE program.
#
#   bash scripts/util_cut_icon_ipl_refs.sh [--apply] [-v]
#
# ⛔⛔ EVERY RUN IS SANDBOXED IN A DISPOSABLE SCRATCH COPY, NEVER THE TRACKED TREE (found this session,
# the hard way): IPL ships several programs that MUTATE THEIR OWN CWD as their normal, documented
# behavior -- progs/upper.icn and progs/lower.icn rename EVERY file in cwd to upper/lower case, and at
# least one other writes a fixed-name foo/XXXXXX scratch file. A first attempt that ran the oracle with
# $PKG/progs as cwd (matching test_icon_arizona_suite.sh's cwd-fidelity pattern) renamed all 275 vendored
# .icn sources to UPPERCASE in place -- `git status` showed 275 D + 279 ?? before it was caught and
# reverted (`git checkout --` + a scoped `git clean -f` on exactly that directory, verified clean after).
# Arizona's own snapshot-diff litter cleanup (test_icon_arizona_suite.sh) only catches ADDITIVE litter
# (new files); it does not protect EXISTING tracked files from being renamed or overwritten in place, so
# it is not sufficient here. This script instead builds ONE pristine template copy of
# progs+procs+gprocs+incl+gincl (~4.7M, cheap) and gives EACH oracle invocation its OWN throwaway copy of
# it, discarded immediately after -- so a self-mutating program can only ever damage a copy already bound
# for /tmp cleanup, never the corpus tree, regardless of what it does. THE SAME HAZARD APPLIES TO GRADING
# (STEP 3, running SCRIP against these programs) -- test_icon_ipl_suite.sh's own RUN tier must use the
# identical isolation, not cd into $PKG/progs directly, or every future grading run re-corrupts the tree.
#
# ⛔ A .std MINTED FROM A RUN THAT ISN'T A GENUINE CLEAN EXECUTION PINS A LIE -- there are SIX ways to
# pin one here, not one, so every progs/ file gets exactly one NAMED outcome, never a silent skip:
#   EMPTY          -- rc=0, zero bytes of stdout. A 0-byte .std would grade "produced nothing" as correct.
#   ORACLE_FAIL    -- rc!=0 under /dev/null stdin, and none of the named classes below explains it. The
#                      row carries THE ORACLE'S OWN FIRST NON-BLANK LINE as its reason, never a sentence
#                      this script composed -- see the arm's own note on the 212 rows that shared one
#                      untested compound reason until 2026-09-06.
#   TIMEOUT        -- exceeded $TIMEOUT (interactive read, or a genuinely long-running demo).
#   UNDECLARED_IDENTIFIER -- an identifier never resolved at link time. ⛔ Checked at ANY rc, not only
#                      rc=0: this oracle is built WITHOUT graphics (`&features` lists no graphics facility
#                      and `open(name,"g")` fails), so gprogs/ programs reference WOpen/WAttrib/DrawLine/Fg
#                      and friends unresolved -- some print the warning and stop (rc=0), 72 more call
#                      through the missing name and die (error 106, offending value &null). Same cause,
#                      two consequences, one class, told apart by the RC column.
#   OVERSIZED      -- output exceeds $MAX_BYTES. Checked via `wc -c` on the FILE, before ever slurping
#                      it into a bash variable (second incident this session: a timing-out program that
#                      is NOT quiet while it waits can still print gigabytes before `timeout` kills it --
#                      see the MAX_BYTES comment below for the full incident). A program whose output
#                      doesn't fit in a small pinned .std is not a good ref candidate regardless.
#   SUSPECT_USAGE  -- rc=0, non-empty, but the first line reads like a usage/error banner printed on a
#                      clean exit (the exact DEAD_REPORT trap util_ref_mint.sh names for SNOBOL4's sbl,
#                      adapted for Icon: a program that prints "usage: prog file" and exits 0 would
#                      otherwise pin its own complaint as ground truth). Named for a human/HQ to eyeball,
#                      never silently absorbed into LIVE.
#   NONDETERMINISTIC -- run twice, independently (fresh scratch copy each time); the two stdouts disagree
#                      (time/date/rand/table-order etc.). Pinning either run would fail the OTHER forever.
#   LIVE           -- rc=0 both runs, non-empty, byte-identical across the two runs, no usage-banner
#                      shape. Minted as progs/NAME.std with --apply; otherwise "would mint".
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
S4E="${S4E_HOME:-$(cd "$HERE/../.." && pwd)}"
. "$HERE/lib_oracle_flags.sh" 2>/dev/null || { echo "⛔ GATE REFUSES: lib_oracle_flags.sh unloadable" >&2; exit 2; }
PKG="$S4E/corpus/packages/icon/ipl"
SUBDIR="progs"
MAINS_ONLY=0
TIMEOUT="${TIMEOUT:-8}"
APPLY=""; VERBOSE=0
while [ $# -gt 0 ]; do case "$1" in
  --apply) APPLY=1;;
  -v) VERBOSE=1;;
  --mains-only) MAINS_ONLY=1;;
  --dir) shift; [ $# -gt 0 ] || { echo "⛔ --dir needs a value" >&2; exit 2; }; SUBDIR="$1";;
  --dir=*) SUBDIR="${1#--dir=}";;
  *) echo "usage: $0 [--apply] [-v] [--dir <subdir>] [--mains-only]" >&2; exit 2;;
esac; shift; done
case "$SUBDIR" in */*|""|.|..) echo "⛔ GATE REFUSES: --dir takes ONE package-relative subdirectory name, got '\''$SUBDIR'\''" >&2; exit 2;; esac
PROGS="$PKG/$SUBDIR"

[ -d "$PROGS" ] || { echo "⛔ GATE REFUSES: corpus subtree missing: $PROGS" >&2; exit 2; }
ICON="$(icon_bin)" || exit 2

# ── ONE pristine template, copied ONCE; every invocation below gets its OWN disposable copy of it.
TEMPLATE="$(mktemp -d "${TMPDIR:-/tmp}/ipl_ref_template.XXXXXX")" || { echo "⛔ mktemp failed" >&2; exit 2; }
for sub in progs gprogs procs gprocs incl gincl; do [ -d "$PKG/$sub" ] && cp -r "$PKG/$sub" "$TEMPLATE/$sub"; done
cleanup_template() { rm -rf "$TEMPLATE"; }

# run_isolated <file.icn> <outfile> -> writes captured combined output to <outfile>, returns rc via
# the function's own exit status (NOT a global -- a global set inside a command-substitution call would
# be lost with the subshell, exactly the bug this replaced: `RC=$?` inside a `$(run_isolated ...)` call
# never reaches the caller). Fresh scratch copy every call, destroyed before returning -- see the header
# note on why cwd=$PROGS directly is unsafe here.
# ⭐ NAME.dat STDIN SIDECAR (hq_I 2026-09-05): mirrors the convention test_icon_arizona_suite.sh and
# test_icon_jcon_suite.sh already use for their own fixtures -- a program that needs input to run
# deterministically gets a NAME.dat beside NAME.icn, fed as stdin. Absent a .dat, behavior is unchanged
# (/dev/null, as before). test_icon_ipl_suite.sh's RUN tier looks up the SAME file -- see FACT RULE in
# that script's header: a script and its own DONE-WHEN (and its sibling ref-cutter) must not disagree.
run_isolated() {
  local f="$1" outfile="$2" work rc stdin_src
  work="$(mktemp -d "${TMPDIR:-/tmp}/ipl_ref_run.XXXXXX")" || return 127
  cp -r "$TEMPLATE"/. "$work"/
  stdin_src=/dev/null
  [ -f "$PROGS/${f%.icn}.dat" ] && stdin_src="$PROGS/${f%.icn}.dat"
  ( cd "$work/$SUBDIR" && timeout "$TIMEOUT" env ICONPATH="$work/progs:$work/gprogs:$work/procs:$work/gprocs:$work/incl:$work/gincl" "$ICON" "$f" < "$stdin_src" > "$outfile" 2>&1 )
  rc=$?
  rm -rf "$work"
  return "$rc"
}

# first_diag <output> -> the first NON-BLANK line, truncated. ⛔ NOT `head -1`: icont's own output opens
# with a blank line before "Run-time error N", so head -1 reports an EMPTY reason for every runtime error
# -- the narrower-question trap this package has now paid for three times (head -1 vs the refusal in
# DISPLAY_REFUSED; `.icn` vs the container in the population count; command -v vs the oracle path).
first_diag() { printf '%s' "$1" | grep -m1 -v '^[[:space:]]*$' | cut -c1-200; }

if [ "$MAINS_ONLY" -eq 1 ]; then
  # ⛔⭐⭐ A PROGRAM ALREADY RULED UNGRADABLE IS NEVER A MINT CANDIDATE (hq_I 2026-09-06, caught the hard
# way twice in one sitting). This script walks the directory and knows nothing about the package's own
# rulings, so a program ruled UNGRADABLE in the sidecar can be re-minted by the very next --apply --
# SILENTLY UN-RULING IT, with no diff to show the ruling was overturned and no human deciding to.
# MEASURED: progs/gcomp.icn is ruled UNGRADABLE because its output IS ITS OWN DIRECTORY LISTING
# (gcomp.icn:41 reads `echo * .*` through a pipe), so a pinned ref encodes the file list of progs/ --
# which OUR OWN ref-cutting mutates. It minted anyway, and the fresh ref contained the six fixture files
# I had added minutes earlier. The next fixture anyone authors would have broken it.
# ⭐ THE GENERAL FORM: a census walks the TREE, while a ruling lives in a FILE BESIDE the tree, so the
# instrument cannot see the decision unless it is made to read it. Determinism, content and stability
# checks all pass on a program whose problem is that a ref is the wrong INSTRUMENT for it.
UNGRADABLE_TSV="$PKG/UNGRADABLE.tsv"
is_ruled_ungradable() {
  [ -f "$UNGRADABLE_TSV" ] || return 1
  awk -F'\t' -v want="$SUBDIR/$1" '$1==want{found=1} END{exit !found}' "$UNGRADABLE_TSV"
}
  # ⛔⭐ LEADING WHITESPACE IS LEGAL BEFORE `procedure` AND THE ANCHORED FORM SILENTLY DROPPED A PROGRAM
  # (hq_I 2026-09-06, found because one ipl UNGRADED row had no census row AT ALL). progs/literat.icn
  # declares its entry point at line 1054 as `    procedure main()`, indented; `^procedure` misses it, so
  # this script -- whose own header promises "every progs/ file gets exactly one NAMED outcome, never a
  # silent skip" -- emitted no row for a shipped main program, and the omission is invisible because the
  # TOTALS line counts the population it built, not the one on disk. ⭐ A census predicate cannot report
  # its own false negatives: everything downstream, including `total=`, is computed from the set it
  # already decided on. Measured over all six subdirectories: exactly one file differs today, which is
  # the point -- a one-file gap is precisely the kind that survives every eyeball on a 461-file count.
mapfile -t FILES < <(cd "$PROGS" && grep -lE '^[[:space:]]*procedure[[:space:]]+main[[:space:]]*\(' *.icn 2>/dev/null | sort)
else
  mapfile -t FILES < <(cd "$PROGS" && ls -1 *.icn | sort)
fi
TOTAL=${#FILES[@]}
[ "$TOTAL" -gt 0 ] || { echo "⛔ GATE REFUSES: zero .icn files found under $PROGS" >&2; exit 2; }

n_live=0; n_mint=0; n_empty=0; n_fail=0; n_display=0; n_diagnostic=0; n_ruled=0; n_timeout=0; n_suspect=0; n_undeclared=0; n_argv=0; n_nondet=0; n_havestd=0; n_oversized=0
OUT1="$(mktemp "${TMPDIR:-/tmp}/ipl_ref_out1.XXXXXX")"; OUT2="$(mktemp "${TMPDIR:-/tmp}/ipl_ref_out2.XXXXXX")"
HOLD="$(mktemp -d "${TMPDIR:-/tmp}/ipl_ref_hold.XXXXXX")"
trap 'cleanup_template; rm -f "$OUT1" "$OUT2"; rm -rf "$HOLD"' EXIT
# ⛔⛔ MAX_BYTES CAPS EVERY FULL-CONTENT READ, CHECKED ON THE FILE BEFORE EVER SLURPING IT INTO A BASH
# VARIABLE (found this session, the hard way -- second incident, same script): under /dev/null stdin a
# timing-out program is not necessarily QUIET while it waits -- one candidate (almost certainly
# noise.icn, matching its name and its TIMEOUT classification in the census) apparently printed as fast
# as it could for the full 8s before `timeout` killed it, and this script's PREVIOUS form did
# `out1="$(cat "$OUT1")"` UNCONDITIONALLY, before ever checking rc1 for a timeout -- slurping a
# many-GB file into a bash string. The process reached 8.7GB RSS with zero children and zero forward
# progress (confirmed via /proc: no fd open on the outfile, no subprocess, TIME frozen for 35+s) and had
# to be killed from outside -- on a shared 30G box with other seats actively running SCRIP concurrently
# at the time. Fix: `wc -c` the FILE first (cheap, no full read) and refuse to slurp past MAX_BYTES,
# same discipline as the EMPTY/SUSPECT_USAGE classes -- a program whose output doesn't fit in a small
# pinned .std is not a good ref candidate anyway, oversized or not.
MAX_BYTES=1048576
printf 'STATUS\tPROGRAM\tRC\tBYTES\tACTION\n'
for f in "${FILES[@]}"; do
  base="${f%.icn}"
  std="$PROGS/$base.std"
  if [ -f "$std" ]; then
    n_havestd=$((n_havestd+1)); printf 'HAVE_STD\t%s\t-\t-\tkept (pre-existing pin, never overwritten)\n' "$f"
    continue
  fi
  run_isolated "$f" "$OUT1"; rc1=$?
  if [ "$rc1" -eq 124 ]; then
    n_timeout=$((n_timeout+1)); printf 'TIMEOUT\t%s\t124\t-\tNOT MINTED -- exceeded %ss under /dev/null stdin\n' "$f" "$TIMEOUT"
    continue
  fi
  by1=$(wc -c < "$OUT1")
  if [ "$by1" -gt "$MAX_BYTES" ]; then
    n_oversized=$((n_oversized+1)); printf 'OVERSIZED\t%s\t%s\t%s\tNOT MINTED -- exceeds %s-byte cap, never slurped into memory\n' "$f" "$rc1" "$by1" "$MAX_BYTES"
    continue
  fi
  out1="$(cat "$OUT1")"
  # ⛔⭐ DISPLAY_REFUSED IS SPLIT OUT OF ORACLE_FAIL ON PURPOSE, AND THE DISTINCTION IS THE WHOLE POINT
  # (hq_I 2026-09-06, CEO-316, measured over gprogs/): the oracle's own binary prints "<prog>: can't open
  # display" and exits 1 on a headless box. Folding that into ORACLE_FAIL is not a cosmetic mislabel --
  # ORACLE_FAIL lands in UNGRADED.tsv, the file of work SOMEBODY IS OWED (supply argv, author a fixture),
  # and 136 of gprogs' 177 mains are display-bound. Filing them as owed manufactures 136 rows NO LANE CAN
  # EVER CLOSE, and buries the genuinely-curable argv rows among them. This is a ruling in the oracle's own
  # words -- UNGRADABLE / ORACLE_REFUSES, nobody owes work -- which is exactly THE DENOMINATOR RULE's test:
  # the program leaves the gradable set because THE ORACLE CANNOT ANSWER IT, not because we chose to skip it.
  # ⛔ SCAN THE WHOLE OUTPUT, NEVER head -1 (hq_I 2026-09-06, caught by measuring instead of assuming):
  # the one-step `icon` driver is a symlink to icont, so it prints icont's benign link-time
  # "undeclared identifier" warnings for the graphics procs FIRST and the real refusal LAST. A head -1
  # probe reads a warning, misses the refusal, and files all 136 display-bound gprogs as ORACLE_FAIL --
  # which is the narrower-question trap: head -1 answers "how does the output OPEN", never "what did the
  # oracle DECIDE". The first draft of this very class did exactly that and reported DISPLAY_REFUSED 0.
  if [ "$rc1" -ne 0 ] && printf '%s' "$out1" | grep -q "can't open display"; then
    # ⛔ QUOTE THE LINE THAT ACTUALLY MATCHED, never head -1. The class is decided by scanning the whole
    # output (above), so quoting line 1 in the EVIDENCE puts a benign "undeclared identifier" warning
    # where the reader expects the refusal -- a correct ruling wearing a false justification, which is
    # worse than a wrong ruling because it survives review. Measured on gprogs/fontpick.icn, whose
    # evidence read '"Font": undeclared identifier' for a display ruling.
    n_display=$((n_display+1)); printf 'DISPLAY_REFUSED\t%s\t%s\t-\tNOT MINTED -- oracle refuses headless: %s\n' "$f" "$rc1" "$(printf '%s' "$out1" | grep -m1 "can't open display")"
    continue
  fi
  # ⛔⭐ UNDECLARED_IDENTIFIER (seat07, STEP-1B verification, 2026-09-06): the SAME icont-symlink shape
  # DISPLAY_REFUSED's own comment above names -- "the one-step `icon` driver ... prints icont's benign
  # link-time 'undeclared identifier' warnings" -- but for a DIFFERENT failure point. DISPLAY_REFUSED
  # catches a RUNTIME refusal ("can't open display") that happens to have link warnings printed before
  # it; this catches programs where NO runtime refusal ever fires because nothing ever reaches the
  # missing symbol at all -- the warning IS the entire output, rc=0, and it is exactly as reproducible
  # across runs as real behavior would be, so every existing LIVE gate (rc, nonempty, determinism,
  # usage-banner) passes on it. MEASURED against gprogs/ specifically: of the 27 files this script
  # currently reports LIVE for that population, 26 (every one except rows2blp) are this shape, not real
  # program output -- confirmed individually via ipl_isolation_run, not sampled. blp2grid.icn's entire
  # "output" is 8 lines naming WriteImage/WAttrib/Pattern/Bg/Fg/DrawLine/FillRectangle as undeclared,
  # zero bytes of anything else -- this oracle build has no graphics facility, so nearly every gprogs/
  # program references an unresolved graphics builtin somewhere in its link closure even when the
  # program's own logic never calls it. Same root cause as DISPLAY_REFUSED (no graphics facility), a
  # different failure point (link-time unresolved symbol, never reaches a runtime WOpen call at all) --
  # kept as its own class rather than folded into DISPLAY_REFUSED so the evidence stays honest about
  # which one actually happened. Checked over the WHOLE output, same discipline as DISPLAY_REFUSED above,
  # not just line 1 (SUSPECT_USAGE's narrower scope) -- icont can print several before or after any real
  # output the program does produce.
  # ⛔⭐⭐ MOVED AHEAD OF THE rc!=0 ARM, AND THAT ORDERING WAS THE DEFECT (hq_I 2026-09-06, CEO-326,
  # measured over all 212 ORACLE_FAIL rows). This arm sat BELOW `if [ "$rc1" -ne 0 ]`, so it could only
  # ever see a run that had ALREADY SUCCEEDED -- the rc=0 half of one piece of evidence. The other half,
  # 72 programs carrying the SAME unresolved-graphics-builtin diagnostic and then DYING on it (error 106,
  # `&null()`, offending value &null -- a call through an identifier that resolved to nothing), fell
  # straight through to ORACLE_FAIL and was filed as "needs argv/stdin", i.e. as WORK OWED. No argument
  # can cure an unresolved builtin. ⭐ THE GENERAL FORM, and it is not about this script: a classifier
  # gated on rc reads its evidence only in the rc-branch you happened to put it in -- the evidence here
  # (a link-time diagnostic) is INDEPENDENT of rc, so gating it on rc silently halves its population.
  # The two failure points stay distinguished by the RC COLUMN and by the action text, which is the
  # honest split: same cause, two consequences.
  if printf '%s' "$out1" | grep -qE ': "[^"]+": undeclared identifier, procedure '; then
    undecl_line="$(printf '%s' "$out1" | grep -m1 ': "[^"]*": undeclared identifier, procedure ')"
    n_undeclared=$((n_undeclared+1))
    if [ "$rc1" -ne 0 ]; then
      printf 'UNDECLARED_IDENTIFIER\t%s\t%s\t%s\tNOT MINTED -- an identifier never resolved at link time and the run then FAILED on it (rc=%s); no argument or fixture can supply a missing builtin: %s | first run diagnostic: %s\n' "$f" "$rc1" "$by1" "$rc1" "$undecl_line" "$(first_diag "$out1")"
    else
      printf 'UNDECLARED_IDENTIFIER\t%s\t0\t%s\tNOT MINTED -- output contains an icont link-time unresolved-identifier diagnostic, not real program behavior: %s\n' "$f" "$by1" "$undecl_line"
    fi
    continue
  fi
  # ⛔⭐ NEEDS_ARGV_FIXTURE -- the ONE class the compound reason was right about, and it is 34 of 212.
  # A program that exits nonzero and prints its OWN usage banner has told us, in its own words, that it
  # refused the arguments it was given. That is a measured cause and a task a lane can pick up (supply a
  # NAME.args sidecar and cut the ref), so it belongs in UNGRADED's vocabulary -- lib_inventory.sh already
  # carries the word. ⛔ The predicate is DELIBERATELY WIDER than SUSPECT_USAGE's `^(usage|error)[: ]`
  # below: that one guards a CLEAN exit, where a banner at column 1 is the only safe signal, while here
  # the program has already failed, so `patchu: usage: patchu source diffs` and `diff: usage: diffu f1 f2`
  # -- which the anchored form misses -- are exactly as much of a refusal as an unprefixed one.
  # ⛔⭐ `grep -a`, AND THAT IS NOT A STYLE CHOICE. Measured while sizing this class: several ipl programs
  # open with ANSI escapes (progs/hr, progs/literat, progs/hebcalen), so a file of their diagnostics is
  # "ISO-8859 text, with escape sequences" and plain `grep -c` over it printed NOTHING AT ALL -- not zero,
  # nothing -- for a pattern with 34 matches. Same family as `command -v` and `head -1`: the instrument
  # quietly answered a narrower question than it was asked, and the empty answer read as an absence.
  if [ "$rc1" -ne 0 ] && printf '%s' "$(first_diag "$out1")" | grep -aqi 'usage'; then
    n_argv=$((n_argv+1)); printf 'NEEDS_ARGV_FIXTURE\t%s\t%s\t-\tNOT MINTED -- rc=%s and the program printed its own usage banner, so it refused the arguments it was given: %s\n' "$f" "$rc1" "$rc1" "$(first_diag "$out1")"
    continue
  fi
  # ⛔⭐⭐ THE EVIDENCE IS THE ORACLE'S OWN FIRST WORDS, NEVER A SENTENCE WE COMPOSED (hq_I 2026-09-06,
  # CEO-326). This arm used to write ONE reason onto every row it produced -- "needs argv/stdin this
  # driver does not supply, or genuine rejection" -- a COMPOUND claim, both halves untested, and it
  # reached 212 rows of ipl/UNGRADED.tsv. Measured, those 212 are: 88 that cannot run on this oracle at
  # all (no graphics facility), 6 that want a tty, 34 that print their own usage banner, and 84 that are
  # none of those. One sentence stood in for four different answers, and the two it names outright were
  # right for 34 of 212. ⭐ A compound reason cannot be falsified by any single observation, so it never
  # gets corrected -- it is the untestable form of "I did not look". Every row now carries the oracle's
  # own first non-blank line, so the next reader can sort 212 rows into classes without re-running
  # anything, and a wrong reason is visibly wrong.
  if [ "$rc1" -ne 0 ]; then
    n_fail=$((n_fail+1)); printf 'ORACLE_FAIL\t%s\t%s\t-\tNOT MINTED -- rc=%s under this driver; the oracle said: %s\n' "$f" "$rc1" "$rc1" "$(first_diag "$out1")"
    [ "$VERBOSE" -eq 1 ] && printf '   %s\n' "$(first_diag "$out1")"
    continue
  fi
  if [ "$by1" -eq 0 ]; then
    n_empty=$((n_empty+1)); printf 'EMPTY\t%s\t0\t0\tNOT MINTED -- rc=0, zero bytes; a 0-byte .std pins "produced nothing" as correct\n' "$f"
    continue
  fi
  # ⛔⭐⭐ THE CONTENT ASSERTION -- the FIFTH property, and the one that would have caught the 27
  # (hq_I 2026-09-06, ruled into the shared body by hq_T). Every other arm here asks whether the output
  # AGREES WITH ITSELF: same rc, same bytes, stable across runs, stable across a minute. NONE of them
  # ever asks what the output IS. That is satisfiable by being consistently wrong -- I minted 27 refs
  # whose entire body was an icont diagnostic, and they cleared rc, non-empty, four-run determinism AND
  # the minute-boundary arm, because a diagnostic is perfectly reproducible. STABILITY IS NOT CORRECTNESS.
  #
  # seat07's UNDECLARED_IDENTIFIER arm above is the specific cure for the specific pattern that bit us.
  # THIS is the general form: if EVERY non-blank line of the candidate looks like the ORACLE TALKING
  # ABOUT the program rather than the program talking, it is not a ref no matter how stable it is.
  # The oracle's diagnostics are file-scoped and name a source position or a procedure -- "foo.icn: ...",
  # "File foo.icn; Line 12 # ...", "Run-time error N". Real program output is not universally shaped
  # that way, and the test is deliberately ALL-lines: a program that legitimately prints one such line
  # among real output still mints.
  #
  # ⛔ IT IS A REFUSAL TO MINT, NOT A RULING ABOUT THE PROGRAM. The row says "a human decides", because
  # the honest reading is "this candidate's body is indistinguishable from the oracle complaining" --
  # which is a reason not to pin it, never a reason to declare the program ungradable.
  if [ "$(printf '%s' "$out1" | grep -cvE '^[[:space:]]*$')" -gt 0 ] \
     && [ "$(printf '%s' "$out1" | grep -vE '^[[:space:]]*$' | grep -cvE '^([^ :]+\.icn:|File [^;]+; Line [0-9]+ #|Run-time error [0-9]+)')" -eq 0 ]; then
    n_diagnostic=$((n_diagnostic+1)); printf 'ALL_ORACLE_DIAGNOSTIC\t%s\t0\t%s\tNOT MINTED -- every non-blank line is the ORACLE talking about the program, not the program: %s\n' "$f" "$by1" "$(printf '%s' "$out1" | grep -vE '^[[:space:]]*$' | head -1)"
    continue
  fi
  if printf '%s' "$out1" | head -1 | grep -qiE '^(usage|error)[: ]'; then
    n_suspect=$((n_suspect+1)); printf 'SUSPECT_USAGE\t%s\t0\t%s\tNOT MINTED -- first line reads like a usage/error banner on a clean exit\n' "$f" "$by1"
    [ "$VERBOSE" -eq 1 ] && printf '   %s\n' "$(printf '%s' "$out1" | head -1)"
    continue
  fi
  # ── determinism check: THREE MORE independent runs (four total), fresh scratch copy each, ALL must
  # agree. ⛔⛔ TWO TOTAL RUNS IS NOT ENOUGH -- MEASURED (found this session): a follow-up 5-sample check
  # against this script's first 66-file output caught 6 programs (filexref/gcomp/puzz/qt/shar/solit)
  # that had passed a 2-run check by pure coincidence and then disagreed on 4/4 further independent runs
  # -- confirmed genuinely time/RNG-seeded, not a fluke (shar/filexref print Icon's &dateline verbatim;
  # solit explicitly seeds &random from &clock). A 2-sample match on a program whose nondeterminism rate
  # is low but nonzero is a real, non-rare false accept, not a theoretical one -- 6 of 66 is ~9%. Four
  # total agreeing runs does not make a false accept impossible, only proportionately rarer; there is no
  # bound past which it's guaranteed zero, same as util_ref_mint.sh accepts for SNOBOL4's LIVE class.
  # ⛔⛔ RECURRENCE, same session (hq_I 2026-09-05): a plain re-run of THIS SAME SCRIPT re-minted
  # filexref/gcomp/qt/shar/solit as LIVE a second time -- exactly the seat01 mechanism ("no
  # same-invocation repeat-run check can see" it, FINDING-2026-09-05-seat01-icon-ipl-same-invocation-
  # determinism-check-has-a-blind-spot.md): four runs fired back-to-back in well under a second all read
  # the SAME &dateline/&clock second-granularity value and agree by coincidence, not by determinism.
  # More agreeing runs alone never closes this -- they just repeat the same coincidence faster. The
  # `sleep 1` below forces each of the 4 runs to land in a DIFFERENT wall-clock second, which a
  # second-granularity clock dependency cannot survive; it costs at most 3s per candidate that reaches
  # this loop (a small minority of the population), never per file overall.
  for _confirm in 1 2 3; do
    sleep 1
    run_isolated "$f" "$OUT2"; rc2=$?
    if [ "$rc2" -eq 124 ]; then
      n_nondet=$((n_nondet+1)); printf 'NONDETERMINISTIC\t%s\t%s/124\t-\tNOT MINTED -- a confirmation run timed out where the first did not\n' "$f" "$rc1"
      continue 2
    fi
    by2=$(wc -c < "$OUT2")
    if [ "$by2" -gt "$MAX_BYTES" ]; then
      n_oversized=$((n_oversized+1)); printf 'OVERSIZED\t%s\t%s\t%s\tNOT MINTED -- a confirmation run exceeds %s-byte cap, never slurped into memory\n' "$f" "$rc2" "$by2" "$MAX_BYTES"
      continue 2
    fi
    out2="$(cat "$OUT2")"
    if [ "$rc2" -ne "$rc1" ] || [ "$out1" != "$out2" ]; then
      n_nondet=$((n_nondet+1)); printf 'NONDETERMINISTIC\t%s\t%s/%s\t-\tNOT MINTED -- disagreed on confirmation run %s/3\n' "$f" "$rc1" "$rc2" "$_confirm"
      continue 2
    fi
  done
  # ⛔⛔ HELD, NEVER MINTED HERE -- the sleep-1 confirmation loop above is STRUCTURALLY BLIND to a
  # MINUTE-granularity clock read, and Icon's &dateline is exactly that ("Sunday, September 6, 2026
  # 9:38 am"). MEASURED (hq_I 2026-09-06, CEO-316): filexref, gcomp, qt and shar are all STABLE across
  # four runs 1.2s apart and ALL FOUR DIFFER across a single minute boundary. The previous fix reasoned
  # "a second-granularity dependency cannot survive a 1s gap" and was right about seconds and silent
  # about minutes -- more agreeing sub-minute runs never close it, they repeat the same coincidence
  # faster, which is the SAME sentence the back-to-back fix was written against, one unit up.
  # So every LIVE candidate is parked and re-run in a second pass AFTER the wall clock has entered a
  # different minute. The wait is paid ONCE for the whole population, not per candidate.
  # ⛔ STORED RAW (cp), NEVER via printf '%s' "$out1": out1 came from a command substitution, which
  # STRIPS TRAILING NEWLINES, while the second pass's $OUT2 is the raw file WITH its trailing newline.
  # Comparing the two with cmp can then NEVER match, and the arm rejects 100% of candidates while
  # looking exactly like a working determinism check. Measured: the first draft of this pass reported
  # "minute-rejected 27" out of 27 -- and its validation against four known-bad programs PASSED, because
  # a reject-everything bug rejects the known-bad too. Compare like for like; mint the stripped form.
  if is_ruled_ungradable "$f"; then
    n_ruled=$((n_ruled+1)); printf 'RULED_UNGRADABLE\t%s\t0\t%s\tNOT MINTED -- already ruled UNGRADABLE in the package sidecar; a mint here would silently overturn that ruling: %s\n' "$f" "$by1" "$(awk -F'\t' -v w="$SUBDIR/$f" '$1==w{print substr($3,1,120)}' "$UNGRADABLE_TSV")"
    continue
  fi
  n_live=$((n_live+1))
  cp "$OUT1" "$HOLD/$base.cand"
  printf 'LIVE_HELD\t%s\t0\t%s\theld for the minute-crossing second pass\n' "$f" "$by1"
done
# ═══ SECOND PASS: the minute-crossing confirmation. ═══════════════════════════════════════════════
# Every LIVE candidate held above is re-run ONCE, after the wall clock has entered a DIFFERENT minute
# from the one its first run landed in. A candidate that disagrees is NONDETERMINISTIC and is never
# minted; only a candidate that agrees across a minute boundary is minted. The wait is amortized: one
# sleep for the whole population, not one per candidate.
n_minute_reject=0
shopt -s nullglob
CANDS=("$HOLD"/*.cand)
shopt -u nullglob
if [ "${#CANDS[@]}" -gt 0 ]; then
  _start_min="$(date +%M)"
  # Sleep to just past the next minute boundary, +2s of margin. Never a bare `sleep 60`: that keeps the
  # SAME offset within the minute and, if the first pass straddled a boundary, can land in a minute the
  # candidate already saw. Bounded by construction -- at most 62s, and 0s is not accepted as "crossed".
  _wait=$(( 62 - 10#$(date +%S) ))
  [ "$_wait" -lt 2 ] && _wait=2
  echo "-- minute-crossing second pass: ${#CANDS[@]} candidate(s), waiting ${_wait}s for the clock to leave minute $_start_min --"
  sleep "$_wait"
  if [ "$(date +%M)" = "$_start_min" ]; then
    echo "⛔ GATE REFUSES: the clock did not leave minute $_start_min; the minute-crossing check did not actually happen" >&2
    exit 2
  fi
  for cand in "${CANDS[@]}"; do
    cb="$(basename "$cand" .cand)"
    run_isolated "$cb.icn" "$OUT2"; rc2=$?
    if [ "$rc2" -ne 0 ] || ! cmp -s "$cand" "$OUT2"; then
      n_live=$((n_live-1)); n_nondet=$((n_nondet+1)); n_minute_reject=$((n_minute_reject+1))
      printf 'NONDETERMINISTIC\t%s.icn\t%s\t-\tNOT MINTED -- agreed across four sub-second runs and DIFFERED across a minute boundary (clock-granularity dependence, e.g. &dateline)\n' "$cb" "$rc2"
      continue
    fi
    # ⛔⭐ MINT BY FILE COPY, NEVER `printf '%s' "$(cat ...)"` (hq_I, found reviewing seat07's
    # UNDECLARED_IDENTIFIER fix, 2026-09-06): a command substitution STRIPS ALL TRAILING NEWLINES from
    # what it captures. rows2blp.icn's real, deterministic, 1-byte oracle output IS a single newline --
    # by1=1 correctly cleared the EMPTY guard (rc=0, non-empty), and the program was correctly ruled
    # LIVE, but `$(cat "$cand")` then silently collapsed that byte to an empty string before printf ever
    # ran, minting a 0-byte .std that pins "produced nothing" for a program that provably did not. The
    # guard tested the INPUT to a transformation that came after it, not what the transformation actually
    # produced. `cp` moves the exact bytes verified above with no shell string handling in between.
    if [ -n "$APPLY" ]; then
      cp "$cand" "$PROGS/$cb.std"; n_mint=$((n_mint+1)); act="MINTED"
    else
      act="would mint"
    fi
    printf 'LIVE\t%s.icn\t0\t%s\t%s (confirmed across a minute boundary)\n' "$cb" "$(wc -c < "$cand")" "$act"
  done
fi
echo "----"
printf 'TOTALS[%s]: LIVE %d (minted %d, minute-rejected %d) · HAVE_STD %d · EMPTY %d · SUSPECT_USAGE %d · UNDECLARED_IDENTIFIER %d · NONDETERMINISTIC %d · NEEDS_ARGV_FIXTURE %d · ORACLE_FAIL %d · DISPLAY_REFUSED %d · ALL_ORACLE_DIAGNOSTIC %d · RULED_UNGRADABLE %d · TIMEOUT %d · OVERSIZED %d · total=%d\n' \
  "$SUBDIR" "$n_live" "$n_mint" "$n_minute_reject" "$n_havestd" "$n_empty" "$n_suspect" "$n_undeclared" "$n_nondet" "$n_argv" "$n_fail" "$n_display" "$n_diagnostic" "$n_ruled" "$n_timeout" "$n_oversized" "$TOTAL"
[ -n "$APPLY" ] || echo "(census only -- nothing written; re-run with --apply to mint)"
# ── belt-and-suspenders: prove the tracked tree is still exactly what HEAD says, every run, census or not.
if git -C "$S4E/corpus" diff --quiet -- packages/icon/ipl/progs packages/icon/ipl/gprogs packages/icon/ipl/procs packages/icon/ipl/gprocs packages/icon/ipl/incl packages/icon/ipl/gincl 2>/dev/null \
   && [ -z "$(git -C "$S4E/corpus" status --porcelain -- packages/icon/ipl/progs packages/icon/ipl/gprogs packages/icon/ipl/procs packages/icon/ipl/gprocs packages/icon/ipl/incl packages/icon/ipl/gincl 2>/dev/null | grep -v '\.std$')" ]; then
  :
else
  echo "⛔⛔⛔ THE TRACKED IPL TREE CHANGED DURING THIS RUN (excluding new .std mints) -- isolation was breached, investigate before trusting anything above ⛔⛔⛔" >&2
fi
