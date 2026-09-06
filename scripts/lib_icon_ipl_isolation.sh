# lib_icon_ipl_isolation.sh -- THE ONE AUTHORITY for executing a corpus/packages/icon/ipl/progs/*.icn
# program (or a SCRIP binary compiled from one), never re-derived per caller.
#
# ⛔⛔ WHY THIS EXISTS: IPL ships several programs that MUTATE THEIR OWN CWD as documented, normal
# behavior -- progs/upper.icn and progs/lower.icn rename EVERY file in cwd to upper/lower case; at least
# one other writes a fixed-name scratch file. A first attempt at util_cut_icon_ipl_refs.sh ran the Icon
# oracle with $PKG/progs as cwd directly (matching test_icon_arizona_suite.sh's cwd-fidelity pattern,
# which is safe for Arizona because its fixtures only ever ADD litter, never rename existing files) and
# it renamed all 275 vendored .icn sources to UPPERCASE in place -- `git status` showed 275 D + 279 ??
# before it was caught and reverted. Arizona's snapshot-diff cleanup does not protect EXISTING tracked
# files from being renamed or overwritten, so it is not sufficient for IPL. ANY caller that executes an
# IPL progs/ program (as source, via the oracle, or as a SCRIP-compiled binary) with progs/ as its cwd
# MUST go through ipl_isolation_run below, never `cd "$PKG/progs" && ...` directly -- this is the same
# "loud refusal, never a private re-derivation" law lib_oracle_flags.sh already applies to oracle paths.
#
# USAGE:
#   . lib_icon_ipl_isolation.sh
#   ipl_isolation_init "$PKG"          # once; builds the pristine template, registers its own cleanup
#   ipl_isolation_run "$OUTFILE" "$TIMEOUT" "$STDIN" cmd arg...
#       runs `cmd arg...` with a FRESH scratch copy's progs/ as cwd, ICONPATH set to the scratch
#       siblings (progs:procs:gprocs:incl:gincl), stdin from $STDIN (pass /dev/null for none -- callers
#       decide, this lib never guesses), under `timeout $TIMEOUT`. Combined stdout+stderr go to
#       $OUTFILE. Returns the command's own exit status (124 = timeout). The scratch copy is destroyed
#       before returning, pass or fail -- a self-mutating program can only ever damage a copy already
#       bound for deletion, never the tracked tree.
#       ⭐ NAME.dat STDIN SIDECAR (hq_I 2026-09-05): callers grading a progs/NAME.icn against a
#       NAME.std should pass "$PKG/progs/NAME.dat" (falling back to /dev/null if absent) -- the same
#       convention test_icon_arizona_suite.sh/test_icon_jcon_suite.sh already use, and the one
#       util_cut_icon_ipl_refs.sh's own run_isolated() now mints refs under; see its header.
#   ipl_isolation_baseline "$CORPUS_ROOT"    # BEFORE the first program runs
#   ipl_isolation_verify_clean "$CORPUS_ROOT"  # after the last one
#       belt-and-suspenders: confirms nothing in the tracked ipl progs/gprogs/procs/gprocs/incl/gincl
#       subtree CHANGED between the two calls (a newly minted untracked *.std excepted). Prints a loud ⛔
#       to stderr, names what moved, and returns 1 if it did -- call it at the end of any script that
#       uses ipl_isolation_run, so a breach is caught by the harness itself rather than by the next
#       `git status` a human happens to run. Skipping the baseline call still gives a guard, comparing
#       against HEAD, and the refusal text says that is what it did.
# A caller's own EXIT trap must call `ipl_isolation_cleanup` (or `rm -rf "$IPL_ISO_TEMPLATE"` directly) --
# this file does not set a trap itself, so it never silently overrides one a caller already set.

ipl_isolation_init() {
  local pkg="$1"
  IPL_ISO_TEMPLATE="$(mktemp -d "${TMPDIR:-/tmp}/ipl_iso_template.XXXXXX")" || return 1
  local sub
  for sub in progs gprogs procs gprocs incl gincl; do
    [ -d "$pkg/$sub" ] && cp -r "$pkg/$sub" "$IPL_ISO_TEMPLATE/$sub"
  done
}

ipl_isolation_cleanup() { [ -n "${IPL_ISO_TEMPLATE:-}" ] && rm -rf "$IPL_ISO_TEMPLATE"; }

# ⛔⭐ THE CWD SUBDIRECTORY IS A PARAMETER, defaulting to progs/ (hq_I 2026-09-06, CEO-316). ipl refs
# are no longer progs-only: gprogs/ now carries .std files too, and this helper hardcoded BOTH the cwd
# and an ICONPATH with no gprogs entry. Left alone it would have run every gprogs entry from the wrong
# directory, with its own package subdirectory missing from the link path -- and, because the caller
# passes the .icn by its TRACKED absolute path, the self-mutation hazard this whole file exists to
# prevent would have been live again for exactly the programs it was not covering. Set IPL_ISO_SUBDIR
# to the entry's own subdirectory before calling; unset means progs/, which is the historical behavior
# byte for byte.
#       ⭐ NAME.fixtures/ FIXTURE-FILE SIDECAR (seat07 2026-09-06): set IPL_ISO_FIXTURES to a program's
#       .icn path before calling and any files under NAME.fixtures/ are staged into the run directory
#       before exec, same env-var-toggle shape as IPL_ISO_SUBDIR. Unset means no staging (unchanged
#       historical behavior). A malformed sidecar (see ipl_fixtures_stage below) returns 125, distinct
#       from every other sentinel this function already returns (124 timeout, 127 setup failure).
ipl_isolation_run() {
  local outfile="$1" to="$2" stdin_src="$3" work rc sub; shift 3
  sub="${IPL_ISO_SUBDIR:-progs}"
  work="$(mktemp -d "${TMPDIR:-/tmp}/ipl_iso_run.XXXXXX")" || return 127
  cp -r "$IPL_ISO_TEMPLATE"/. "$work"/
  [ -d "$work/$sub" ] || { echo "⛔ ipl_isolation_run: no such package subdirectory: $sub" >&2; rm -rf "$work"; return 127; }
  if [ -n "${IPL_ISO_FIXTURES:-}" ]; then
    ipl_fixtures_stage "$IPL_ISO_FIXTURES" "$work/$sub"; [ $? -eq 2 ] && { rm -rf "$work"; return 125; }
  fi
  ( cd "$work/$sub" && timeout "$to" env ICONPATH="$work/progs:$work/gprogs:$work/procs:$work/gprocs:$work/incl:$work/gincl" "$@" < "$stdin_src" > "$outfile" 2>&1 )
  rc=$?
  rm -rf "$work"
  return "$rc"
}

# ⛔⭐⭐ THIS ANSWERS "DID THE TREE CHANGE DURING THIS RUN", AND IT USED TO ANSWER "DOES THE TREE DIFFER
# FROM HEAD" -- two different questions, and the second one cries BREACH at work the package asked for
# (hq_I 2026-09-06, measured while authoring the CEO-328 argv fixtures). Ten new NAME.argv/NAME.in files
# sat untracked in progs/ before the cutter ever started; every run then ended with the triple-⛔ banner
# saying isolation was breached, over a run in which nothing was breached at all. ⭐ THE COST IS NOT THE
# FALSE LINE, IT IS WHAT A FALSE LINE TEACHES: this banner exists because a first cut of the ref-cutter
# renamed all 275 vendored sources to UPPERCASE in place, and a banner that fires on every fixture-
# authoring sitting is one nobody reads on the sitting it is finally right. So: SNAPSHOT FIRST, COMPARE
# AFTER. ipl_isolation_baseline records the subtree's porcelain state before any program runs; verify
# reports only what is NEW since then, whatever its extension.
# ⛔ WITH NO BASELINE the old HEAD comparison is kept -- a caller that never snapshotted still gets a
# guard -- but it SAYS SO in the refusal, because "you have untracked fixtures" and "a program rewrote
# the tree" must not print the same sentence.
# ⭐ The exemption narrowed while it moved: it was `grep -v '\.std$'`, which exempted a .std at ANY
# status -- including ` M` on a TRACKED ref, i.e. a pinned ref overwritten mid-run, the single most
# damaging thing that could happen here. Only an UNTRACKED NEW .std (`?? …`, what --apply legitimately
# mints) is exempt now; the old form leaned on the separate `git diff --quiet` arm to catch that case.
_ipl_iso_state() {
  git -C "$1" status --porcelain -- packages/icon/ipl/progs packages/icon/ipl/gprogs packages/icon/ipl/procs packages/icon/ipl/gprocs packages/icon/ipl/incl packages/icon/ipl/gincl 2>/dev/null | LC_ALL=C sort
}
ipl_isolation_baseline() {
  IPL_ISO_BASELINE="$(mktemp "${TMPDIR:-/tmp}/ipl_iso_baseline.XXXXXX")" || return 1
  _ipl_iso_state "$1" > "$IPL_ISO_BASELINE"
}
ipl_isolation_verify_clean() {
  local corpus="$1" now changed scope
  now="$(mktemp "${TMPDIR:-/tmp}/ipl_iso_now.XXXXXX")" || return 1
  _ipl_iso_state "$corpus" > "$now"
  if [ -n "${IPL_ISO_BASELINE:-}" ] && [ -f "$IPL_ISO_BASELINE" ]; then
    changed="$(LC_ALL=C comm -13 "$IPL_ISO_BASELINE" "$now" | grep -v '^?? .*\.std$')"
    scope="since this run's baseline"
  else
    changed="$(grep -v '^?? .*\.std$' "$now")"
    scope="against HEAD -- NO BASELINE WAS CAPTURED, so a fixture that was already untracked before this run reads as a breach here; call ipl_isolation_baseline before the first program runs to get the real answer"
  fi
  rm -f "$now"
  [ -z "$changed" ] && return 0
  echo "⛔⛔⛔ THE TRACKED IPL TREE CHANGED DURING THIS RUN ($scope) -- isolation was breached, investigate before trusting anything above ⛔⛔⛔" >&2
  printf '%s\n' "$changed" | head -20 >&2
  return 1
}

# ⛔⭐⭐ THE ARGV SIDECAR, AND IT LIVES HERE FOR THE SAME REASON THE CWD RULE DOES (hq_I 2026-09-06,
# CEO-328 ruling: "one path-aware cutter that reads both sidecars ... never a fork of the cutter or the
# grader"). ipl already honours a NAME.dat stdin sidecar, looked up INDEPENDENTLY by the ref-cutter and
# by the suite -- two copies of one convention, which is exactly how test_prolog_ladder.sh and its Raku
# twin silently diverged until they were diffed. This reader is the single authority for the second
# sidecar, so a cutter that mints a ref and a grader that checks it cannot disagree about what argv the
# program was given. ⛔ PATH-AWARE, NEVER BASENAME: ipl ships gener/morse/repeats/spokes in two
# directories apiece, so the sidecar is resolved beside the .icn and nowhere else.
#
# FORMAT -- NAME.argv beside NAME.icn, ONE line: NAME<TAB>arg<TAB>arg...
#   The leading NAME column is REDUNDANT here (the filename already carries it) and is required anyway,
#   deliberately: it is hq_T's format for the suite-level <family>.argv byte for byte (SCRIP 44f9e17ce),
#   so nobody moving between the two has to remember which shape they are in -- and a copied sidecar
#   whose name column no longer matches its file is caught instead of silently arming the wrong program.
#   ⭐ NO QUOTING LANGUAGE AT ALL, exactly as hq_T argued: an argument containing spaces arrives byte for
#   byte because TAB is the only separator and there is no word-splitting layer to be misread through.
#
# ipl_argv_read <icn-path> <array-name>
#   0 -> a sidecar was read; the named array holds the declared argv (never empty on 0)
#   1 -> no sidecar beside that .icn; the array is emptied. NOT an error: most programs take no argv.
#   2 -> the sidecar exists and is MALFORMED. ⛔ Refuses loudly rather than running the program with a
#        guessed argv: a fixture that arms the wrong arguments prints plausible output and pins it.
ipl_argv_read() {
  local icn="$1" arr="$2" side base line n
  eval "$arr=()"
  side="${icn%.icn}.argv"
  base="$(basename "$icn" .icn)"
  [ -f "$side" ] || return 1
  n=$(grep -cv '^[[:space:]]*\(#.*\)\?$' "$side")
  if [ "$n" -ne 1 ]; then
    echo "⛔ ARGV SIDECAR REFUSES(2): $side holds $n declaration lines, expected exactly 1 -- one program, one argv" >&2; return 2
  fi
  line="$(grep -v '^[[:space:]]*\(#.*\)\?$' "$side" | head -1)"
  local IFS=$'\t'; local -a f=($line); unset IFS
  if [ "${f[0]}" != "$base" ]; then
    echo "⛔ ARGV SIDECAR REFUSES(2): $side declares '${f[0]}' but sits beside $base.icn -- a copied sidecar arming the wrong program" >&2; return 2
  fi
  if [ "${#f[@]}" -lt 2 ]; then
    echo "⛔ ARGV SIDECAR REFUSES(2): $side names $base and declares no arguments -- an empty argv is what you get with no sidecar at all, so the file states nothing" >&2; return 2
  fi
  eval "$arr=(\"\${f[@]:1}\")"
  return 0
}

# ⭐ NAME.fixtures/ FIXTURE-FILE SIDECAR (seat07 2026-09-06). NAME.dat covers stdin content and NAME.argv
# covers argument VALUES, but several ipl programs take an argument that NAMES A REAL FILE ON DISK --
# gediff.icn diffs two file arguments, huffstuf.icn/filecnvt.icn/iiencode.icn each read a named input file
# directly. Neither existing sidecar can place a file into the isolated run directory, so a NAME.argv
# naming "fixture.txt" is useless without something to put fixture.txt there. Same shape as ipl_argv_read:
# one shared reader, called by both the cutter's own run_isolated() and the shared ipl_isolation_run()
# above (via IPL_ISO_FIXTURES), so a program's minted ref and its later grading can never disagree about
# which files were present when it ran.
#
# FORMAT -- NAME.fixtures/ directory beside NAME.icn, holding one regular file per fixture, copied into
# the run directory verbatim BY ITS OWN FILENAME (never renamed); NAME.argv then names that same filename.
# No manifest: the directory's own contents ARE the declaration, same as NAME.dat's bytes ARE the stdin
# declaration with no format wrapped around them.
#
# ipl_fixtures_stage <icn-path> <dest-dir>
#   0 -> zero or more fixture files staged into dest-dir (0 is normal: most programs need none)
#   2 -> NAME.fixtures exists but is not a plain directory of regular files -- refuses loudly rather than
#        stage a symlink or subdirectory silently, which would either escape the scratch tree or leave the
#        program that needed it failing under the wrong reported class.
ipl_fixtures_stage() {
  local icn="$1" dest="$2" dir f
  dir="${icn%.icn}.fixtures"
  [ -e "$dir" ] || return 0
  if [ ! -d "$dir" ] || [ -L "$dir" ]; then
    echo "⛔ FIXTURE SIDECAR REFUSES(2): $dir exists but is not a plain directory" >&2; return 2
  fi
  for f in "$dir"/*; do
    [ -e "$f" ] || continue
    if [ ! -f "$f" ] || [ -L "$f" ]; then
      echo "⛔ FIXTURE SIDECAR REFUSES(2): $f is not a plain regular file" >&2; return 2
    fi
    cp "$f" "$dest/$(basename "$f")" || return 2
  done
  return 0
}
