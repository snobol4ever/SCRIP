#!/usr/bin/env bash
# util_zdp_stdout_manifest.sh — byte-identity manifest that hashes COMPILE STDOUT.  ⛔ NEVER hash the .s beside the .sno: those are COMMITTED ARTIFACTS and hashing them is what made s133 first gate report a fake zero.  Prove the null against itself before any zero is worth anything.
# Byte-identity manifest.  Hashes COMPILE STDOUT, never the committed .s beside the .sno (s133's fake gate).
OUT=$1
: > "$OUT"
for f in $(find /home/claude/corpus/programs/snobol4 /home/claude/corpus/probe -name '*.sno' | sort); do
    h=$(timeout 15 ./scrip --compile "$f" < /dev/null 2>/dev/null | md5sum | cut -d' ' -f1)
    echo "$h  $f" >> "$OUT"
done
wc -l < "$OUT"
