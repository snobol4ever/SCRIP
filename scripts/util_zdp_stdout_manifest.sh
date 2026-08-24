#!/usr/bin/env bash
# util_zdp_stdout_manifest.sh — byte-identity manifest that hashes COMPILE STDOUT.  ⛔ NEVER hash the .s beside the .sno: those are COMMITTED ARTIFACTS and hashing them is what made s133 first gate report a fake zero.  Prove the null against itself before any zero is worth anything.
# Byte-identity manifest.  Hashes COMPILE STDOUT, never the committed .s beside the .sno (s133's fake gate).
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
OUT=$1
: > "$OUT"
for f in $(find $S4E/corpus/tests/snobol4 $S4E/corpus/probe -name '*.sno' | sort); do
    h=$(timeout 15 ./scrip --compile "$f" < /dev/null 2>/dev/null | md5sum | cut -d' ' -f1)
    echo "$h  $f" >> "$OUT"
done
wc -l < "$OUT"
