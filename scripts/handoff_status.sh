#!/usr/bin/env bash
# handoff_status.sh — THE ONLY SANCTIONED SOURCE OF A CHAT SESSION COMPLETION CLAIM.
#
# Per RULES.md FACT RULE (2026-06-24): the assistant MUST NOT compose "CHAT SESSION COMPLETE"
# as prose. It runs THIS script and presents the verbatim output.
#
# It does NOT hardcode a repo count. With no args it DISCOVERS every git repo that has an
# 'origin' remote directly under the workspace root (the parent of the SCRIP repo this
# script lives in) — so it can never miss a touched repo, and it prints how many it found.
# Pass explicit repo dirs to override. A handoff is COMPLETE only when EVERY discovered
# repo is tree-clean AND local HEAD == origin/<branch> with zero unpushed. Reading origin
# needs no credential; only the push that PRECEDES this check does. Exit 0 = complete, 1 = blocked.
set -uo pipefail
SELF_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"   # <ws>/SCRIP/scripts
WS="$(dirname "$(dirname "$SELF_DIR")")"                    # <ws> (parent of the SCRIP repo)
REPOS=("$@") ; src="given on command line"
if [ ${#REPOS[@]} -eq 0 ]; then
  src="auto-discovered under $WS"
  shopt -s nullglob dotglob
  for d in "$WS"/*/; do
    d="${d%/}"; base="$(basename "$d")"
    { [ "$base" = "." ] || [ "$base" = ".." ]; } && continue
    [ -d "$d/.git" ] || continue                              # is a git repo
    git -C "$d" remote get-url origin >/dev/null 2>&1 || continue   # has an origin to push to
    REPOS+=("$d")
  done
  IFS=$'\n' REPOS=($(printf '%s\n' "${REPOS[@]}" | sort)); unset IFS
fi
echo "=== CHAT SESSION STATUS — checking ${#REPOS[@]} repo(s) ($src); ground truth from git ==="
[ ${#REPOS[@]} -eq 0 ] && { echo "CHAT SESSION WAITING — no git repos with an 'origin' remote found under $WS"; exit 1; }
blocked=0; reasons=()
for r in "${REPOS[@]}"; do
  name=$(basename "$r")
  br=$(git -C "$r" rev-parse --abbrev-ref HEAD 2>/dev/null)
  dirty=$(git -C "$r" status --porcelain 2>/dev/null | wc -l)
  git -C "$r" fetch --quiet origin 2>/dev/null || reasons+=("$name: fetch failed (origin ref may be stale)")
  lh=$(git -C "$r" rev-parse HEAD 2>/dev/null)
  oh=$(git -C "$r" rev-parse "origin/$br" 2>/dev/null || echo MISSING)
  unpushed=$(git -C "$r" log --oneline "@{u}..HEAD" 2>/dev/null | wc -l)
  st="OK"
  if [ "$dirty" -ne 0 ]; then st="DIRTY"; reasons+=("$name: $dirty uncommitted change(s) — commit them"); blocked=1; fi
  if [ "$lh" != "$oh" ] || [ "$unpushed" -ne 0 ]; then st="UNPUSHED"; reasons+=("$name: local HEAD not on origin/$br ($unpushed unpushed) — git pull --rebase && git push"); blocked=1; fi
  printf "  %-22s %-10s local=%s origin=%s\n" "$name [$br]" "$st" "${lh:0:9}" "${oh:0:9}"
done
echo "------------------------------------------------------------"
if [ "$blocked" -eq 0 ] && [ ${#reasons[@]} -eq 0 ]; then echo "CHAT SESSION COMPLETE"; exit 0
else echo "CHAT SESSION WAITING — not done:"; printf '  - %s\n' "${reasons[@]}"; exit 1; fi
