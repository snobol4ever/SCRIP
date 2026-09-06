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
#   ipl_isolation_verify_clean "$CORPUS_ROOT"
#       belt-and-suspenders: confirms the tracked ipl progs/procs/gprocs/incl/gincl subtree is still
#       exactly what HEAD says (new *.std files excepted). Prints a loud ⛔ to stderr and returns 1 if
#       not -- call this at the end of any script that uses ipl_isolation_run, so a breach is caught by
#       the harness itself rather than by the next `git status` a human happens to run.
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
ipl_isolation_run() {
  local outfile="$1" to="$2" stdin_src="$3" work rc sub; shift 3
  sub="${IPL_ISO_SUBDIR:-progs}"
  work="$(mktemp -d "${TMPDIR:-/tmp}/ipl_iso_run.XXXXXX")" || return 127
  cp -r "$IPL_ISO_TEMPLATE"/. "$work"/
  [ -d "$work/$sub" ] || { echo "⛔ ipl_isolation_run: no such package subdirectory: $sub" >&2; rm -rf "$work"; return 127; }
  ( cd "$work/$sub" && timeout "$to" env ICONPATH="$work/progs:$work/gprogs:$work/procs:$work/gprocs:$work/incl:$work/gincl" "$@" < "$stdin_src" > "$outfile" 2>&1 )
  rc=$?
  rm -rf "$work"
  return "$rc"
}

ipl_isolation_verify_clean() {
  local corpus="$1"
  if git -C "$corpus" diff --quiet -- packages/icon/ipl/progs packages/icon/ipl/gprogs packages/icon/ipl/procs packages/icon/ipl/gprocs packages/icon/ipl/incl packages/icon/ipl/gincl 2>/dev/null \
     && [ -z "$(git -C "$corpus" status --porcelain -- packages/icon/ipl/progs packages/icon/ipl/gprogs packages/icon/ipl/procs packages/icon/ipl/gprocs packages/icon/ipl/incl packages/icon/ipl/gincl 2>/dev/null | grep -v '\.std$')" ]; then
    return 0
  fi
  echo "⛔⛔⛔ THE TRACKED IPL TREE CHANGED DURING THIS RUN (excluding new .std mints) -- isolation was breached, investigate before trusting anything above ⛔⛔⛔" >&2
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
