#!/usr/bin/env bash
# s4e_kill_mine.sh -- scoped kill: signals ONLY pids under the caller's own root,
#                     and ANSWERS THE SAME QUESTION WITHOUT KILLING ANYTHING.
#
# Why this exists: every seat runs as the SAME unix user on ONE box, so a bare
# `pkill -f <pattern>` (or `pgrep -f`) matches and kills every OTHER seat's
# processes too -- hq_P's 18:28 box-wide kill took 19 other seats' harness
# workers; seat07 typed the same shape minutes after reading that relay and
# was saved only because the pattern matched its own shell first. `-f` is
# also a self-match trap: the pattern argument is itself part of THIS
# script's own command line, so a naive `pgrep -f "$pattern"` run from inside
# this script matches this script (and its whole ancestor chain, since a
# parent's cmdline can carry the same text -- e.g. a wrapper script that
# embeds the pattern literally). See RULES.md pgrep -x history.
#
# Scope, in order:
#   1. Walk this process's OWN ancestor chain ($$  up through every PPid) and
#      exclude it completely -- the caller's shell/pid tree is never a target,
#      no matter what it matches.
#   2. Of what remains, keep only pids whose /proc/<pid>/cwd resolves under
#      the caller's own root ($S4E_HOME, or derived from $0 per the D-17
#      sibling-root convention -- same derivation s4e_msg.sh uses).
#   3. Of what remains, keep pids matching the given pattern by exact
#      executable name (comm) OR as a substring of their argv/cmdline.
# There is no flag that skips step 2 -- there is no box-wide form.
#
# ⛔⭐⭐ --explain-only EXISTS BECAUSE A REFUSAL YOU CAN ONLY REACH BY KILLING IS NOT
# A REACHABLE REFUSAL (CEO-560; row a-seats-kill-must-name-its-own-root-and-the-
# refusal-is-reachable-without-killing-anything, CEO-976, coo 2026-09-20).
# The incident that rowed it: at about 01:4x CDT 2026-09-20 a seat signalled ELEVEN
# pids over a pattern list including `corpus_suite_harness` and `test-arena` --
# patterns EVERY seat shares -- and SIX were pids it had not verified were its own.
# A background waiter belonging to a THIRD seat died with exit 144 in the same
# window. The scoping below would have prevented all of it and was not used, and
# the reason it was not used is the whole lesson: UNTIL NOW THE ONLY WAY TO LEARN
# WHAT THIS SCRIPT WOULD DO WAS TO LET IT DO IT. An instrument whose safe behaviour
# can only be observed by taking the unsafe action gets replaced, under pressure,
# by the unsafe action -- so the dry run is not a convenience, it is the feature
# that makes the scoping adoptable. `--explain-only` signals NOTHING, ever, and
# names every pid it WOULD DECLINE together with the reason class it was declined
# under, so the decision is auditable before it is irreversible.
#
# ⛔⭐ AND THE ROOT IS NAMED, NEVER MERELY INHERITED. Arm one of that row: a kill
# from a seat NAMES ITS OWN ROOT OR REFUSES. `--root <dir>` states it explicitly and
# is CHECKED against the root this script derives independently -- a named root that
# is not the caller's own is REFUSED rather than honoured, which is what makes
# `--root /` (the box-wide form this script's header promises does not exist) a
# refusal rather than a wildcard. When no --root is given the derived root is still
# PRINTED on every path, including the kill path: a signal whose scope was never
# stated in its own output cannot be audited afterwards, and the eleven-pid incident
# is exactly the case where afterwards is the only time anyone looks.
set -u

S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 sibling root

usage() {
  echo "usage: $0 [--root <dir>] [--explain-only] [--] <exact-process-name-or-argv-token>" >&2
  echo "       --explain-only   answer the question and SIGNAL NOTHING; names every pid it would decline and why" >&2
  echo "       --root <dir>     name the root this kill is scoped to; a root that is not the caller's own REFUSES" >&2
}

explain_only=""
named_root=""
pattern=""
while [ $# -gt 0 ]; do
  case "$1" in
    --explain-only) explain_only=1; shift ;;
    --root) named_root="${2:-}"; shift 2 || { echo "refuse: --root needs a directory" >&2; exit 2; } ;;
    --root=*) named_root="${1#--root=}"; shift ;;
    -h|--help) usage; exit 2 ;;
    --) shift; pattern="${1:-}"; break ;;
    -*) usage; echo "refuse: unknown flag '$1' -- refusing rather than guessing what it scopes to" >&2; exit 2 ;;
    *) pattern="$1"; shift ;;
  esac
done

# --- ARM ONE, AND IT IS CHECKED BEFORE ANYTHING IS ENUMERATED: a named root that is
# not the caller's own is a refusal.  Order matters -- validating the root only after
# building a target list would mean the wrong-root case had already read every other
# seat's /proc entry to decide what it was "about" to do.
if [ -n "$named_root" ]; then
  nr=$(readlink -f "$named_root" 2>/dev/null || printf '%s' "$named_root")
  if [ "$nr" != "$S4E" ]; then
    echo "refuse: --root '$named_root' (resolves to '$nr') is NOT this caller's own root '$S4E'." >&2
    echo "        A kill names its own root or it refuses; there is no box-wide form, so '/' is a refusal and not a wildcard." >&2
    echo "        rc=2 -- nothing was enumerated and nothing was signalled." >&2
    exit 2
  fi
fi

if [ -z "$pattern" ] && [ -z "$explain_only" ]; then
  usage
  echo "refuse: no pattern given -- there is no box-wide form" >&2
  exit 2
fi

# --- step 1: the caller's own shell/pid tree, always excluded ---
declare -A ancestors
p=$$
while [[ $p =~ ^[0-9]+$ ]]; do
  ancestors[$p]=1
  [ "$p" -le 1 ] && break
  p=$(awk '/^PPid:/{print $2}' "/proc/$p/status" 2>/dev/null)
done

# --- steps 2+3: root-scope, then name/argv match.  In --explain-only every pid that
# falls out is RECORDED WITH THE REASON CLASS IT FELL OUT UNDER, because "I would not
# touch this" is only checkable if it says which rule spared it.
targets=()
declined_ancestor=() declined_outside=() declined_nomatch=()
scanned=0
for pidpath in /proc/[0-9]*/; do
  pid=${pidpath#/proc/}
  pid=${pid%/}
  scanned=$((scanned + 1))
  if [ -n "${ancestors[$pid]:-}" ]; then
    [ -n "$explain_only" ] && declined_ancestor+=("$pid")
    continue
  fi

  cwd=$(readlink -f "/proc/$pid/cwd" 2>/dev/null) || continue
  case "$cwd" in
    "$S4E"|"$S4E"/*) ;;
    *) [ -n "$explain_only" ] && declined_outside+=("$pid"); continue ;;
  esac

  comm=$(cat "/proc/$pid/comm" 2>/dev/null) || comm=""
  cmdline=$(tr '\0' ' ' < "/proc/$pid/cmdline" 2>/dev/null) || cmdline=""

  match=""
  if [ -n "$pattern" ]; then
    [ "$comm" = "$pattern" ] && match=1
    case "$cmdline" in *"$pattern"*) match=1 ;; esac
  fi
  if [ -n "$match" ]; then
    targets+=("$pid")
  else
    [ -n "$explain_only" ] && declined_nomatch+=("$pid")
  fi
done

show() { # pid -> one evidence line
  local pid="$1" c m
  c=$(readlink -f "/proc/$pid/cwd" 2>/dev/null)
  m=$(tr '\0' ' ' < "/proc/$pid/cmdline" 2>/dev/null)
  printf '      pid=%s cwd=%s cmd=%s\n' "$pid" "${c:-<gone>}" "${m:0:120}"
}

if [ -n "$explain_only" ]; then
  echo "s4e_kill_mine --explain-only: SIGNALLING NOTHING. This is the answer to the question, not the act."
  # ⛔ `${v:+a}${v:-b}` IS NOT AN IF/ELSE and this line proved it in testing: when v is set,
  # ${v:-b} expands to v's VALUE, so the "verified" branch printed the root a second time
  # ("...and verified/home/claude_coo"). Two expansions of the same variable are not two
  # branches of one choice. Written as a plain if, which is what it always meant.
  if [ -n "$named_root" ]; then
    echo "  root scoped to: $S4E   (named as '$named_root' and VERIFIED against this caller's own derived root)"
  else
    echo "  root scoped to: $S4E   (derived, D-17 sibling root -- name it with --root to have it verified)"
  fi
  echo "  pattern: ${pattern:-<none given> -- with no pattern NOTHING can match, so every pid below is declined and the decline reasons are the point}"
  echo "  /proc entries scanned: $scanned"
  echo "  WOULD SIGNAL: ${#targets[@]} pid(s)"
  for pid in "${targets[@]:-}"; do [ -n "$pid" ] && show "$pid"; done
  echo "  WOULD DECLINE: $(( ${#declined_ancestor[@]} + ${#declined_outside[@]} + ${#declined_nomatch[@]} )) pid(s), by reason class:"
  echo "    would decline ${#declined_ancestor[@]} as ANCESTOR_CHAIN -- the caller's own shell/pid tree, never a target whatever it matches (step 1)"
  for pid in "${declined_ancestor[@]:-}"; do [ -n "$pid" ] && show "$pid"; done
  echo "    would decline ${#declined_outside[@]} as NOT_UNDER_ROOT -- cwd resolves outside $S4E; THIS IS THE CLASS THE ELEVEN-PID INCIDENT SIGNALLED (step 2)"
  echo "    would decline ${#declined_nomatch[@]} as $([ -n "$pattern" ] && echo NO_PATTERN_MATCH || echo NO_PATTERN_GIVEN) -- under this root, but no comm/argv match (step 3)"
  echo "  ⛔ NOTHING WAS SIGNALLED BY THIS RUN. Re-run without --explain-only to act, and the root above is the scope you get."
  exit 0
fi

if [ "${#targets[@]}" -eq 0 ]; then
  echo "refuse: nothing under $S4E matches '$pattern' (root-scoped, ancestor chain excluded) -- rc=2" >&2
  exit 2
fi

echo "s4e_kill_mine: root=$S4E ${named_root:+(named and verified)} -- signalling ${#targets[@]} pid(s) matching '$pattern':"
for pid in "${targets[@]}"; do
  cwd=$(readlink -f "/proc/$pid/cwd" 2>/dev/null)
  cmdline=$(tr '\0' ' ' < "/proc/$pid/cmdline" 2>/dev/null)
  printf '  pid=%s cwd=%s cmd=%s\n' "$pid" "$cwd" "$cmdline"
done

for pid in "${targets[@]}"; do
  kill -TERM "$pid" 2>/dev/null
done
exit 0
