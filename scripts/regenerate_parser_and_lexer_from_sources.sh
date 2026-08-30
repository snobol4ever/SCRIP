#!/usr/bin/env bash
# build_regenerate.sh — regenerate committed parser/lexer C files from their .y/.l sources
#
# When to run: after editing any .y or .l source file.
# The generated .tab.c / .lex.c files are committed so the build never requires
# bison/flex at normal build time — but when sources change, regenerate here
# and commit both the .y/.l and the updated .tab.c/.lex.c together.
#
# Requires: bison >= 3.0, flex >= 2.6  (installed by build_packages.sh)
# Idempotent. Safe to run multiple times.
# Usage: bash build/build_regenerate.sh
set -euo pipefail

SCRIP="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
SNO="$SCRIP/src/parsers/snobol4"
REBUS="$SCRIP/src/parsers/rebus"

# ── prerequisite check ───────────────────────────────────────────────────────
for tool in bison flex; do
    command -v "$tool" >/dev/null 2>&1 || {
        echo "FAIL $tool not found — run bash build/build_packages.sh first"
        exit 1
    }
done
echo "OK  bison $(bison --version | head -1 | awk '{print $NF}'), flex $(flex --version | awk '{print $NF}')"

# ── bison must WORK, not merely be on PATH ──────────────────────────────────
# `command -v bison` answers "is it on PATH", never "can it generate". Measured
# 2026-08-30 (hq_C): bison 3.8.2 on PATH, --print-datadir naming a directory that
# did not exist, so every generate died with
#   bison: /usr/share/bison/m4sugar/m4sugar.m4: cannot open: No such file or directory
# AFTER printing the grammar's conflict summary — so it looked like it had run.
# Resolve a usable skeleton dir, then prove it on a throwaway grammar.
if [ ! -f "$(bison --print-datadir 2>/dev/null)/m4sugar/m4sugar.m4" ]; then
    for d in "$HOME/.local/share/bison" /tmp/flexbison/root/usr/share/bison /usr/local/share/bison; do
        [ -f "$d/m4sugar/m4sugar.m4" ] && { export BISON_PKGDATADIR="$d"; break; }
    done
fi
_bt=$(mktemp -d)
printf '%%%%\ns: ;\n' > "$_bt/t.y"
if bison -o "$_bt/t.c" "$_bt/t.y" 2>"$_bt/err" && [ -s "$_bt/t.c" ]; then
    echo "OK  bison generates (datadir $(bison --print-datadir 2>/dev/null)${BISON_PKGDATADIR:+ -> $BISON_PKGDATADIR})"
else
    echo "FAIL bison is on PATH but cannot generate — $(head -1 "$_bt/err")"
    echo "     no skeleton dir found; install bison's data files or set BISON_PKGDATADIR"
    rm -rf "$_bt"; exit 1
fi
rm -rf "$_bt"

# ── snobol4 parser: snobol4.y → snobol4.tab.c + snobol4.tab.h ───────────────
echo "GEN snobol4.tab.c from snobol4.y"
cd "$SNO"
bison -d -o snobol4.tab.c snobol4.y
echo "OK  snobol4.tab.c snobol4.tab.h"

# ── snobol4 lexer: snobol4.l → snobol4.lex.c ────────────────────────────────
echo "GEN snobol4.lex.c from snobol4.l"
flex --noline -o snobol4.lex.c snobol4.l
echo "OK  snobol4.lex.c"

# ── snocone parser: snocone_parse.y → snocone_parse.tab.c + snocone_parse.tab.h
# (LS-4.a — atoms + arithmetic + parens + assignment; LS-4.b–LS-4.i extend.)
# The Snocone lexer is a hand-written threaded-code FSM (snocone_lex.c), not
# a Flex source — so this stanza generates only the parser, not a lex file.
# LS-4.cn (session-#7) renamed snocone.y → snocone_parse.y for symmetry
# with snocone_lex.{c,h}; generated artifacts also picked up the _parse
# infix.
SNOCONE="$SCRIP/src/parsers/snocone"
echo "GEN snocone_parse.tab.c from snocone_parse.y"
cd "$SNOCONE"
bison -d -o snocone_parse.tab.c snocone_parse.y
echo "OK  snocone_parse.tab.c snocone_parse.tab.h"

# ── rebus parser: rebus.y → rebus.tab.c + rebus.tab.h ───────────────────────
echo "GEN rebus.tab.c from rebus.y"
cd "$REBUS"
bison -d -o rebus.tab.c rebus.y
echo "OK  rebus.tab.c rebus.tab.h"

# ── rebus lexer: rebus.l → lex.rebus.c ──────────────────────────────────────
echo "GEN lex.rebus.c from rebus.l"
flex --noline -o lex.rebus.c rebus.l
echo "OK  lex.rebus.c"

# ── raku parser: raku.y → raku.tab.c + raku.tab.h ───────────────────────────
RAKU="$SCRIP/src/parsers/raku"
echo "GEN raku.tab.c from raku.y"
cd "$RAKU"
bison -d --warnings=none -Wno-yacc -o raku.tab.c raku.y
echo "OK  raku.tab.c raku.tab.h"

# ── raku lexer: raku.l → raku.lex.c ─────────────────────────────────────────
echo "GEN raku.lex.c from raku.l"
flex --noline --prefix=raku_yy -o raku.lex.c raku.l
echo "OK  raku.lex.c"

echo "DONE — commit .y/.l sources and generated .tab.c/.tab.h/.lex.c together"

# ── pascal parser: pascal.y → pascal.tab.c + pascal.tab.h ───────────────────
# (PB-0..PB-3, session 2026-06-02 — 7th frontend. Grammar adapted from the
# MIT-licensed pascalp.y syntactic reference. One expected dangling-else
# shift/reduce conflict, same as pascalp.y. Case-insensitive per ISO 7185 and
# the fpc -Miso oracle (pascal.l: %option caseless + IDENT lowercased at the
# lexer boundary, row pascal-identifiers-must-be-case-insensitive-writeln-only-lowercase
# 2026-08-30 -- was lowercase-only before that, a real defect, not a design choice).
PASCAL="$SCRIP/src/parsers/pascal"
echo "GEN pascal.tab.c from pascal.y"
cd "$PASCAL"
bison -d -o pascal.tab.c pascal.y
echo "OK  pascal.tab.c pascal.tab.h"
echo "GEN pascal.lex.c from pascal.l"
flex --noline -o pascal.lex.c pascal.l
echo "OK  pascal.lex.c"
