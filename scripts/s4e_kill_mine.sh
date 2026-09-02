#!/usr/bin/env bash
# s4e_kill_mine.sh -- scoped kill: signals ONLY pids under the caller's own root.
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
set -u

S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 sibling root
pattern="${1:-}"

if [ -z "$pattern" ]; then
  echo "usage: $0 <exact-process-name-or-argv-token>" >&2
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

# --- steps 2+3: root-scope, then name/argv match ---
targets=()
for pidpath in /proc/[0-9]*/; do
  pid=${pidpath#/proc/}
  pid=${pid%/}
  [ -n "${ancestors[$pid]:-}" ] && continue

  cwd=$(readlink -f "/proc/$pid/cwd" 2>/dev/null) || continue
  case "$cwd" in
    "$S4E"|"$S4E"/*) ;;
    *) continue ;;
  esac

  comm=$(cat "/proc/$pid/comm" 2>/dev/null) || comm=""
  cmdline=$(tr '\0' ' ' < "/proc/$pid/cmdline" 2>/dev/null) || cmdline=""

  match=""
  [ "$comm" = "$pattern" ] && match=1
  case "$cmdline" in *"$pattern"*) match=1 ;; esac
  [ -n "$match" ] && targets+=("$pid")
done

if [ "${#targets[@]}" -eq 0 ]; then
  echo "refuse: nothing under $S4E matches '$pattern' (root-scoped, ancestor chain excluded) -- rc=2" >&2
  exit 2
fi

echo "s4e_kill_mine: signalling ${#targets[@]} pid(s) under $S4E matching '$pattern':"
for pid in "${targets[@]}"; do
  cwd=$(readlink -f "/proc/$pid/cwd" 2>/dev/null)
  cmdline=$(tr '\0' ' ' < "/proc/$pid/cmdline" 2>/dev/null)
  printf '  pid=%s cwd=%s cmd=%s\n' "$pid" "$cwd" "$cmdline"
done

for pid in "${targets[@]}"; do
  kill -TERM "$pid" 2>/dev/null
done
exit 0
