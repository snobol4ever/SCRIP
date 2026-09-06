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
  for sub in progs procs gprocs incl gincl; do
    [ -d "$pkg/$sub" ] && cp -r "$pkg/$sub" "$IPL_ISO_TEMPLATE/$sub"
  done
}

ipl_isolation_cleanup() { [ -n "${IPL_ISO_TEMPLATE:-}" ] && rm -rf "$IPL_ISO_TEMPLATE"; }

ipl_isolation_run() {
  local outfile="$1" to="$2" stdin_src="$3" work rc; shift 3
  work="$(mktemp -d "${TMPDIR:-/tmp}/ipl_iso_run.XXXXXX")" || return 127
  cp -r "$IPL_ISO_TEMPLATE"/. "$work"/
  ( cd "$work/progs" && timeout "$to" env ICONPATH="$work/progs:$work/procs:$work/gprocs:$work/incl:$work/gincl" "$@" < "$stdin_src" > "$outfile" 2>&1 )
  rc=$?
  rm -rf "$work"
  return "$rc"
}

ipl_isolation_verify_clean() {
  local corpus="$1"
  if git -C "$corpus" diff --quiet -- packages/icon/ipl/progs packages/icon/ipl/procs packages/icon/ipl/gprocs packages/icon/ipl/incl packages/icon/ipl/gincl 2>/dev/null \
     && [ -z "$(git -C "$corpus" status --porcelain -- packages/icon/ipl/progs packages/icon/ipl/procs packages/icon/ipl/gprocs packages/icon/ipl/incl packages/icon/ipl/gincl 2>/dev/null | grep -v '\.std$')" ]; then
    return 0
  fi
  echo "⛔⛔⛔ THE TRACKED IPL TREE CHANGED DURING THIS RUN (excluding new .std mints) -- isolation was breached, investigate before trusting anything above ⛔⛔⛔" >&2
  return 1
}
