#!/usr/bin/env bash
# test_gate_no_finding_files.sh -- NO FINDING-*.md FILE IS TRACKED IN .github OR SCRIP (Lon 2026-09-16, CEO-760: every FINDING file removed;
# ceo CEO-796; coo, row instruments-a-finding-file-in-github-is-refused-by-gate-and-the-digest-gate-gains-the-no-finding-rule). Six came
# back the same afternoon because seven root digests still said 'FINDINGs are named FINDING-<date>-<seat>-...'. The record is the LIVE
# CURSOR and the batons; a finding's evidence goes into the row's baton, and git history keeps every removed file citable.
# Cheap, no build: `git ls-files` in .github and SCRIP; the population (tracked files examined) is printed; rc=1 names each offender;
# rc=2 when a repo cannot be listed. FAIL_ONCE=1 plants a FINDING file in a scratch clone of .github and grades that clone instead.
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/../.." && pwd)"
GH="$ROOT/.github"; SC="$ROOT/SCRIP"
if [ -n "${FAIL_ONCE:-}" ]; then
  W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
  git clone -q "$GH" "$W/gh" 2>/dev/null || { echo "⛔ REFUSED-TO-GRADE: could not clone $GH for the planted arm"; exit 2; }
  printf '# planted\n' > "$W/gh/FINDING-2099-01-01-fixture-planted.md"; git -C "$W/gh" add FINDING-2099-01-01-fixture-planted.md
  GH="$W/gh"
fi
bad=""; n=0
for repo in "$GH" "$SC"; do
  [ -d "$repo/.git" ] || [ -f "$repo/.git" ] || { echo "⛔ REFUSED-TO-GRADE: $repo is not a git checkout"; exit 2; }
  files="$(git -C "$repo" ls-files 2>/dev/null)" || { echo "⛔ REFUSED-TO-GRADE: git ls-files failed in $repo"; exit 2; }
  n=$((n + $(printf '%s\n' "$files" | grep -c .)))
  hits="$(printf '%s\n' "$files" | grep -E '(^|/)FINDING-[^/]*\.md$' || true)"
  [ -n "$hits" ] && bad="$bad$(printf '%s\n' "$hits" | sed "s#^#  $(basename "$repo")/#")\n"
done
echo "population: $n tracked file(s) examined across .github and SCRIP"
if [ -n "$bad" ]; then
  printf '⛔ GATE RED [no_finding_files]: FINDING-*.md file(s) are tracked -- every FINDING file was removed on Lon'"'"'s word (CEO-760) and the record is the LIVE CURSOR and the batons:\n%b' "$bad"
  echo "  cure: fold the evidence into the row's baton (git history keeps the file citable), git rm the file, and fix the digest that told you to write it (test_gate_digest_matches_rules.sh rule NO-FINDING-FILES)"
  exit 1
fi
echo "GATE PASS [no_finding_files]: no FINDING-*.md is tracked in .github or SCRIP ($n files examined)"
