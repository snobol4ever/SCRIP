#!/usr/bin/env bash
# FACT RULE "ONE MEDIUM, INVISIBLE": a BB template never hand-encodes an instruction as raw bytes nor
# branches on the medium for an instruction. Forbidden in BB_templates/*.cpp (comments stripped):
#   raw-byte producers: x86_Lrec, x86_Jrec, x86_Drec, x86_b1(, x86_b2(, x86_b3(, bytes(, u8(, u32le, u64le
#   instruction medium-branch: IF(MEDIUM_BINARY , IF(MEDIUM_MACRO_DEF
# ALLOWED: IF(MEDIUM_TEXT, ...) for a comment/label only. x86_asm.h is the encoder impl (exempt).
# Informational WIP baseline; --strict enforces zero.
cd "$(dirname "$0")/.." || exit 2
total=0; hits=""
for f in src/emitter/BB_templates/*.cpp; do
  code=$(perl -0777 -pe 's{/\*.*?\*/}{}gs; s{//[^\n]*}{}g' "$f")
  m=$(printf '%s' "$code" | grep -cE 'x86_Lrec|x86_Jrec|x86_Drec|x86_b1\(|x86_b2\(|x86_b3\(|\bbytes\(|\bu8\(|\bu32le|\bu64le|IF\(MEDIUM_BINARY|IF\(MEDIUM_MACRO_DEF')
  if [ "$m" -gt 0 ]; then total=$((total+m)); hits="$hits $(basename $f)($m)"; fi
done
echo "medium-branch / raw-byte producers in BB templates (code): $total  (target 0)"
if [ "$total" -ne 0 ]; then
  echo "REMAINING:$hits"
  echo "(informational WIP baseline — each owning GOAL-*-BB session drives its boxes to 0 as it converts.)"
  [ "$1" = "--strict" ] && { echo "GATE FAIL (--strict)."; exit 1; }
  exit 0
fi
echo "OK: every BB template is medium-invisible (pure x86() concat)."
