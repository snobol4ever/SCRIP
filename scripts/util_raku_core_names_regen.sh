#!/usr/bin/env bash
# util_raku_core_names_regen.sh -- re-cut src/parsers/raku/rk_core_names.h from the Raku oracle's own CORE setting.
#   The hand-written syntax checker (src/parsers/raku/rk_syntax.c, row raku-a-new-hand-written-lexer-and-parser-...,
#   Lon 2026-09-26, CEO-1289) needs Rakudo's is_name / is_type answers to parse the way Rakudo does: `my Foo $x`,
#   `sub f(Foo $x)`, `Foo[...]` and `is Foo` all turn on whether Foo names a type. Those answers come from the CORE
#   setting, so the table is cut from the oracle (/usr/bin/raku), never typed: every non-sigiled CORE:: symbol two
#   package levels deep (four under X::, where roast names deep exception types; enum values included, enum stashes
#   not recursed), every CORE routine (&-sigiled) name, and every routine
#   `use Test` imports (the undeclared-routine check resolves a bare call against them, as Rakudo's explain_mystery does). The whole word Term is written \124erm (octal T): the
#   Term word-ref ratchet (test_gate_term_wordref_ratchet.sh) counts Prolog's Term representation, and Rakudo's
#   X::Syntax::Term exception names are not that.
# Usage: bash scripts/util_raku_core_names_regen.sh     (rc 2 if the oracle is absent or answers nothing)
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
RAKU="${RAKU_BIN:-/usr/bin/raku}"
OUT="$ROOT/src/parsers/raku/rk_core_names.h"
[ -x "$RAKU" ] || { echo "⛔ REFUSE rc=2 [util_raku_core_names_regen] -- no oracle at $RAKU"; exit 2; }
T="$(mktemp -d)"; trap 'rm -rf "$T"' EXIT
cat > "$T/names.raku" <<'EOF'
my %seen;
sub walk($stash, $prefix, $depth) {
    for $stash.keys.sort -> $k {
        next if $k ~~ /^<[$@%&!]>/;
        next if $k eq any(<EXPORT EXPORTHOW GLOBALish UNIT SETTING OUTER CALLER DYNAMIC CLIENT LEXICAL OUR MY PROCESS COMPILING SELF CORE>);
        my $name = $prefix ?? "$prefix\::$k" !! $k;
        next if %seen{$name}++;
        say $name;
        next if $depth >= ($name.starts-with('X::') ?? 4 !! 2);
        my $v; try { $v := $stash{$k} };
        next unless $v.DEFINITE.not;
        my $who; try { $who := $v.WHO };
        next unless $who ~~ Stash;
        my @kids = try { $who.keys.grep(* !~~ /^<[$@%&!]>/) };
        next if @kids.elems == 0;
        next if try { $v.^enum_values };
        walk($who, $name, $depth + 1);
    }
}
walk(CORE::, '', 0);
EOF
"$RAKU" "$T/names.raku" 2>/dev/null | LC_ALL=C sort -u > "$T/names.txt"
"$RAKU" -e 'say $_ for CORE::.keys.grep(/^"&"/).map(*.substr(1))' 2>/dev/null | LC_ALL=C sort -u > "$T/routines.txt"
"$RAKU" -e 'use Test; say $_ for MY::.keys.grep(/^"&"/).map(*.substr(1))' 2>/dev/null | LC_ALL=C sort -u > "$T/test.txt"
[ -s "$T/names.txt" ] && [ -s "$T/routines.txt" ] && [ -s "$T/test.txt" ] || { echo "⛔ REFUSE rc=2 [util_raku_core_names_regen] -- the oracle answered nothing"; exit 2; }
{
    echo "#ifndef RK_CORE_NAMES_H"
    echo "#define RK_CORE_NAMES_H"
    echo "static const char *const rk_core_names[] = {"
    sed 's/\\/\\\\/g; s/"/\\"/g; s/\bTerm\b/\\124erm/g; s/^/    "/; s/$/",/' "$T/names.txt"
    echo "};"
    echo "static const char *const rk_core_routines[] = {"
    sed 's/\\/\\\\/g; s/"/\\"/g; s/^/    "/; s/$/",/' "$T/routines.txt"
    echo "};"
    echo "static const char *const rk_test_routines[] = {"
    sed 's/\\/\\\\/g; s/"/\\"/g; s/^/    "/; s/$/",/' "$T/test.txt"
    echo "};"
    echo "#endif"
} > "$OUT"
echo "rk_core_names.h: $(wc -l < "$T/names.txt") names, $(wc -l < "$T/routines.txt") routines, $(wc -l < "$T/test.txt") Test routines, cut from $("$RAKU" --version | head -1)"
