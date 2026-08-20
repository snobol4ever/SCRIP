#!/usr/bin/env bash
# util_three_section_audit.sh — audit every compiled SM/BB/XA template's
# PLATFORM_X86 block for the three-section contract (TSX rung).
#
# Contract: every template with a PLATFORM_X86 block must contain, INSIDE
# that block, exactly three medium sections each with ONE and ONLY ONE return:
#     if (MEDIUM_MACRO_DEF) { ... return ...; }
#     if (MEDIUM_BINARY)    { ... return ...; }   <- the byte-emitting section
#     if (MEDIUM_TEXT)      { ... return ...; }
#
# This script reports presence of each section (not return-count — that is a
# manual structural review per file). Exit 1 if any compiled template with an
# x86 block is missing a section.
set -u
# PATH CORRECTED s169 (seat1, gates-dead-paths): this walked src/emitter/ and harvested filenames by
# grepping the Makefile for "(SM|BB|XA)_templates/x.cpp". After the src reorg the Makefile spells every
# template $(SRC)/templates/x.cpp, so the grep matched ZERO files, the loop body never ran, and the script
# printed "AUDIT GREEN: all x86 blocks have three sections" while auditing NOTHING.
cd "$(dirname "$0")/.." || exit 2
MK=Makefile
fail=0
files=$(grep -oE "templates/[a-z0-9_]+\.cpp" "$MK" | sort -u | sed 's#^#src/#')
if [ -z "$files" ]; then echo "VACUOUS: no template files harvested from $MK — audit enforced nothing." >&2; exit 2; fi
for f in $files; do
  bn=$(basename "$f")
  x86=$(grep -c 'PLATFORM_X86'    "$f")
  md=$(grep  -c 'MEDIUM_MACRO_DEF' "$f")
  bn=$(grep  -c 'MEDIUM_BINARY'    "$f")
  tx=$(grep  -c 'MEDIUM_TEXT'      "$f")
  if [ "$x86" -eq 0 ]; then
    printf "%-45s  (no x86 block — skip)\n" "$(basename "$f")"
    continue
  fi
  status="OK"
  [ "$md" -eq 0 ] && { status="MISSING-MACRO_DEF"; fail=1; }
  [ "$bn" -eq 0 ] && { status="MISSING-BINARY";    fail=1; }
  [ "$tx" -eq 0 ] && { status="${status};MISSING-TEXT-GUARD"; fail=1; }
  printf "%-45s x86=%s md=%s bin=%s txt=%s  -> %s\n" \
         "$(basename "$f")" "$x86" "$md" "$bn" "$tx" "$status"
done
echo "----"
[ "$fail" -eq 0 ] && echo "AUDIT GREEN: all x86 blocks have three sections." \
                  || echo "AUDIT RED: sections missing above."
exit $fail
