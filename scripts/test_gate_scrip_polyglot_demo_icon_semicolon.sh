#!/usr/bin/env bash
# test_gate_scrip_polyglot_demo_icon_semicolon.sh -- DONE-WHEN for scrip-polyglot-demo-icon-semicolon-5-files.
# The five named polyglot demos, broken by the Icon-section's newline-terminated statements
# (SCRIP's Icon frontend requires semicolons -- same root cause class as icon-ipl-corpus-
# newline-incompatibility, but this is FIRST-PARTY demo content, not a third-party archive,
# so "author a new semicolon-correct file" collapses to "just fix these five files"), must
# produce their own .expected output once fixed.
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME
set -u
SCRIP="${SCRIP:-$S4E/SCRIP/scrip}"
CORPUS="${CORPUS:-$S4E/corpus}"

fail() { echo "⛔ NOT DONE: $*" >&2; exit 1; }
[ -x "$SCRIP" ] || fail "scrip not built at $SCRIP -- run 'make pristine' in SCRIP/ first"

for d in demo05 demo07 demo08 demo09 demo10; do
    src="$(ls "$CORPUS/demos/scrip/$d"/*.scrip 2>/dev/null | head -1)"
    exp="$(ls "$CORPUS/demos/scrip/$d"/*.expected 2>/dev/null | head -1)"
    [ -f "$src" ] && [ -f "$exp" ] || fail "$d: witness or .expected not found under $CORPUS/demos/scrip/$d"
    actual="$(timeout 8 "$SCRIP" --run "$src" < /dev/null 2>/tmp/_scrip_demo_semi_err)"
    expected="$(cat "$exp")"
    [ "$actual" = "$expected" ] || fail "$d: output mismatch -- expected=[$expected] actual=[$actual] stderr=$(cat /tmp/_scrip_demo_semi_err)"
done
rm -f /tmp/_scrip_demo_semi_err

echo "✅ scrip-polyglot-demo-icon-semicolon-5-files DONE-WHEN: all five named demos now match their .expected"
exit 0
