#!/usr/bin/env bash
# util_style200_oracle.sh — THE GOAL-STYLE-200COL ORACLE, automated (R4, 2026-08-20).
#
# Per GOAL-STYLE-200COL.md: a reformat is proven by OBJECT IDENTITY, never by a test suite.  For each file:
#   (1) the TRUE original comes from `git show HEAD:<path>` -- NEVER a manual cp backup (the goal records a miss that
#       went unnoticed for two turns because of exactly that);
#   (2) compile original standalone;  (3) compile the working-tree version the same way;
#   (4) diff `objdump -d` (instruction stream) AND `objdump -s -j .data -j .rodata -j .bss` (data) -- BOTH must be
#       byte-identical.
# ⛔ objdump echoes the object FILENAME in its header, and NOT always on line 1 -- normalize GLOBALLY by string, never
#    by line number (the goal records that a line-numbered sed missed it at least twice).
# ⛔ A HEADER IS NOT INDEPENDENTLY ORACLE-ABLE: `gcc -c` on a lone .h yields something objdump cannot read, and because
#    BOTH sides fail identically the empty-vs-empty diff reads as a false PASS.  Headers are reported SKIP(header) here
#    and must be verified through a consumer .c/.cpp instead (the goal's stash method).
# ⛔ A file that fails to compile BOTH ways is SKIP(no-compile-both-ways), not a pass and not a break -- the same class
#    the goal already catalogues (dead-IR-vocabulary files, files needing a build context standalone compile lacks).
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
SRC="$ROOT/src"
INC="-I$SRC -I$SRC/include -I$SRC/contracts -I$SRC/lower -I$SRC/machine -I$SRC/emitter -I$SRC/runtime/core -I$SRC/runtime/builtins -I$SRC/runtime -I$SRC/runtime/rt -I$SRC/parser/snobol4 -I$SRC/parser/raku -I$SRC/optimizer -I$SRC/templates -I$SRC/driver"   # RT_INCS from the Makefile (:336) VERBATIM plus templates/driver -- the first cut used only CBASE's shorter list and skipped 96 of 161 files as "no-compile-both-ways", which is not a gate, it is a gate-shaped hole.
DEF="-DDYN_ENGINE_LINKED -DIR_DEFINE_NAMES"
T="$(mktemp -d)"; trap 'rm -rf "$T"' EXIT
ok=0; bad=0; skiph=0; skipc=0; BADF=""
cd "$ROOT"
for f in "$@"; do
    rel="${f#$ROOT/}"
    case "$rel" in *.h|*.hpp) skiph=$((skiph+1)); continue;; esac
    case "$rel" in *.cpp) CC=g++; STD="-std=c++17 -finput-charset=UTF-8";; *) CC=gcc; STD="";; esac
    git show "HEAD:$rel" > "$T/before.src" 2>/dev/null || { skipc=$((skipc+1)); continue; }
    ext="${rel##*.}"; cp "$T/before.src" "$T/before.$ext"; cp "$rel" "$T/after.$ext"
    $CC -c -O0 -w $STD $INC $DEF "$T/before.$ext" -o "$T/before.o" 2>/dev/null; rb=$?
    $CC -c -O0 -w $STD $INC $DEF "$T/after.$ext"  -o "$T/after.o"  2>/dev/null; ra=$?
    if [ $rb -ne 0 ] || [ $ra -ne 0 ]; then
        [ $rb -ne 0 ] && [ $ra -ne 0 ] && { skipc=$((skipc+1)); continue; }
        echo "⛔ BREAK (compiles one way only, rb=$rb ra=$ra): $rel"; bad=$((bad+1)); BADF="$BADF $rel"; continue
    fi
    objdump -d "$T/before.o" | sed "s#$T/before\.o#OBJ#g; s#before\.o#OBJ#g" > "$T/b.d"
    objdump -d "$T/after.o"  | sed "s#$T/after\.o#OBJ#g;  s#after\.o#OBJ#g"  > "$T/a.d"
    objdump -s -j .data -j .rodata -j .bss "$T/before.o" 2>/dev/null | sed "s#$T/before\.o#OBJ#g; s#before\.o#OBJ#g" > "$T/b.s"
    objdump -s -j .data -j .rodata -j .bss "$T/after.o"  2>/dev/null | sed "s#$T/after\.o#OBJ#g;  s#after\.o#OBJ#g"  > "$T/a.s"
    if cmp -s "$T/b.d" "$T/a.d" && cmp -s "$T/b.s" "$T/a.s"; then ok=$((ok+1))
    else echo "⛔ DIFF: $rel"; bad=$((bad+1)); BADF="$BADF $rel"; fi
done
echo "------------------------------------------------------------"
echo "ORACLE: $ok byte-identical · $bad BREAK/DIFF · $skiph skipped(header, verify via consumer) · $skipc skipped(no-compile-both-ways)"
[ $bad -ne 0 ] && { echo "⛔ FAIL — reformat touched semantics in:$BADF"; exit 1; }
echo "OK"; exit 0
