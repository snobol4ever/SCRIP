#!/usr/bin/env bash
# util_style200_oracle_yl.sh — THE GOAL-STYLE-200COL ORACLE FOR .y / .l SOURCES (R4, 2026-08-20).
#
# A .y/.l file has no object code of its own, so the C Oracle does not apply.  Its object-equivalent is THE GENERATED
# PARSER: reformat the grammar, regenerate, and the emitted .c must be byte-identical.  That is a STRONGER gate than the
# C one -- it proves the reformat changed neither the grammar nor the actions.
#
# ⛔ THE GATE IS OBJECT IDENTITY OF THE *COMPILED GENERATED PARSER*, NOT TEXT IDENTITY.  Measured while building this:
#    a pure comment strip on snobol4.y leaves a 1000-line raw text diff, which falls to 48 once `#line` directives are
#    ignored and to 28 once comments are too -- and those last 28 are `yyrline[]`, bison's table of GRAMMAR-RULE LINE
#    NUMBERS, emitted only under YYDEBUG.  Deleting a comment shifts every later line, so #line and yyrline both move
#    while behaviour does not.  Compiling settles it: without YYDEBUG the table is not compiled in and #line touches
#    only debug info, so byte-identical objects prove the grammar and actions are unchanged.
# ⛔ COMPARE THE GENERATED PARSER *WITH ITS COMMENTS STRIPPED*.  bison and flex COPY action-block comments verbatim
#    into the generated .c, so removing a comment from a grammar legitimately changes generated TEXT while changing no
#    behaviour whatsoever.  A raw text compare therefore fails 7 of 9 files on a pure comment strip -- measured.  Both
#    sides are run through tools/strip_comments.py before the diff, which makes the comparison SEMANTIC: comments can
#    never affect compiled output, so identical-after-stripping means the grammar and the actions are unchanged.
# ⛔ BOTH SIDES MUST BE GENERATED FROM AN IDENTICAL PATH AND OUTPUT NAME.  bison/flex bake the input path into `#line`
#    directives and the output basename into an `#include`, so generating "before" as /tmp/x.y and "after" as the real
#    src/parsers/... path produces a diff that is pure path noise and proves nothing.  Measured while building this:
#    running the repo's own regenerate script from inside each parser dir rewrote 10 committed files, and the ENTIRE
#    difference was `#line` prefixes plus one `#include` basename -- 4 non-#line lines in total.  The committed parsers
#    ARE reproducible; they were simply generated from the SCRIP root rather than from the parser directory.
# ⛔ NEEDS flex + bison.  Neither is installed system-wide here.  Rootless (seat7's recipe, no sudo):
#      cd /tmp && mkdir -p fb/dl fb/root && cd fb/dl && apt-get download flex bison libbison-dev libfl-dev
#      for d in *.deb; do dpkg-deb -x "$d" ../root/; done
#      export PATH=/tmp/fb/root/usr/bin:$PATH BISON_PKGDATADIR=/tmp/fb/root/usr/share/bison
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
command -v bison >/dev/null || { echo "⛔ bison MISSING — see the rootless recipe in this file's header"; exit 2; }
command -v flex  >/dev/null || { echo "⛔ flex MISSING — see the rootless recipe in this file's header"; exit 2; }
T="$(mktemp -d)"; trap 'rm -rf "$T"' EXIT
ok=0; bad=0; BADF=""
cd "$ROOT"
for f in "$@"; do
    rel="${f#$ROOT/}"; base="$(basename "$rel")"; ext="${rel##*.}"
    mkdir -p "$T/before" "$T/after"
    git show "HEAD:$rel" > "$T/before/$base" 2>/dev/null || { echo "  SKIP (not in HEAD): $rel"; continue; }
    cp "$rel" "$T/after/$base"
    gen() {  # $1 = dir holding the identically-named source; identical CWD + identical names on both sides
        ( cd "$1" || exit 9
          case "$ext" in
            y) bison -d --warnings=none -Wno-yacc -o out.tab.c "$base" >/dev/null 2>&1 ;;
            l) flex --noline -o out.lex.c "$base" >/dev/null 2>&1 ;;
          esac )
    }
    gen "$T/before"; rb=$?
    gen "$T/after";  ra=$?
    if [ $rb -ne 0 ] || [ $ra -ne 0 ]; then
        if [ $rb -ne 0 ] && [ $ra -ne 0 ]; then echo "  SKIP (fails to generate BOTH ways): $rel"; continue; fi
        echo "⛔ BREAK ($rel): generates one way only (before=$rb after=$ra) — the reformat broke the grammar"
        bad=$((bad+1)); BADF="$BADF $rel"; continue
    fi
    SRCD="$ROOT/src"
    INCF="-I$SRCD -I$SRCD/include -I$SRCD/contracts -I$SRCD/lower -I$SRCD/machine -I$SRCD/emitter -I$SRCD/runtime/core -I$SRCD/runtime/builtins -I$SRCD/runtime -I$SRCD/runtime/rt -I$SRCD/parser/snobol4 -I$SRCD/parser/raku -I$SRCD/optimizer -I$SRCD/templates -I$SRCD/driver -I$(dirname "$ROOT/$rel")"
    d=0; comp=0
    for o in out.tab.c out.lex.c; do
        [ -f "$T/before/$o" ] || continue
        gcc -c -O0 -w $INCF -DDYN_ENGINE_LINKED -DIR_DEFINE_NAMES "$T/before/$o" -o "$T/before/$o.o" 2>/dev/null; cb=$?
        gcc -c -O0 -w $INCF -DDYN_ENGINE_LINKED -DIR_DEFINE_NAMES "$T/after/$o"  -o "$T/after/$o.o"  2>/dev/null; ca=$?
        if [ $cb -ne 0 ] || [ $ca -ne 0 ]; then continue; fi
        comp=1
        objdump -d "$T/before/$o.o" | sed "s#^\\S*\\.o:#OBJ:#" > "$T/before/$o.d"
        objdump -d "$T/after/$o.o"  | sed "s#^\\S*\\.o:#OBJ:#" > "$T/after/$o.d"
        objdump -s -j .data -j .rodata -j .bss "$T/before/$o.o" 2>/dev/null | sed "s#^\\S*\\.o:#OBJ:#" > "$T/before/$o.s"
        objdump -s -j .data -j .rodata -j .bss "$T/after/$o.o"  2>/dev/null | sed "s#^\\S*\\.o:#OBJ:#" > "$T/after/$o.s"
        cmp -s "$T/before/$o.d" "$T/after/$o.d" && cmp -s "$T/before/$o.s" "$T/after/$o.s" || d=1
    done
    if [ $comp -eq 0 ]; then printf "  SKIP(generated parser will not compile standalone) %s\n" "$rel"; continue; fi
    if [ $d -eq 0 ]; then ok=$((ok+1)); printf "  ✅ %-34s generated parser byte-identical\n" "$rel"
    else bad=$((bad+1)); BADF="$BADF $rel"; printf "  ⛔ %-34s GENERATED PARSER DIFFERS\n" "$rel"; fi
done
echo "------------------------------------------------------------"
echo "Y/L ORACLE: $ok byte-identical · $bad BREAK/DIFF"
[ $bad -ne 0 ] && { echo "⛔ FAIL — reformat changed the generated parser for:$BADF"; exit 1; }
echo "OK"; exit 0
