#!/usr/bin/env bash
# test_gate_scrip_polyglot_demo_segfaults.sh -- DONE-WHEN for scrip-polyglot-demo-3-segfaults.
# The three named polyglot demos under corpus/scrip/demoNN/ must not crash the compiler.
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME
set -u
SCRIP="${SCRIP:-$S4E/SCRIP/scrip}"
CORPUS="${CORPUS:-$S4E/corpus}"

fail() { echo "⛔ NOT DONE: $*" >&2; exit 1; }
[ -x "$SCRIP" ] || fail "scrip not built at $SCRIP -- run 'make pristine' in SCRIP/ first"

for f in demo02/wordcount demo04/palindrome demo06/sieve; do
    src="$CORPUS/scrip/$f.scrip"
    [ -f "$src" ] || fail "witness not found: $src"
    timeout 8 "$SCRIP" --run "$src" < /dev/null > /dev/null 2>/tmp/_scrip_demo_segv_err
    rc=$?
    [ "$rc" -lt 128 ] || fail "$f still crashes (rc=$rc): $(cat /tmp/_scrip_demo_segv_err)"
done
rm -f /tmp/_scrip_demo_segv_err

echo "✅ scrip-polyglot-demo-3-segfaults DONE-WHEN: none of the three named demos crash anymore"
exit 0
