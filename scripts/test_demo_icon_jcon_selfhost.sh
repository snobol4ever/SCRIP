#!/usr/bin/env bash
# test_demo_icon_jcon_selfhost.sh -- THE JCON SELF-HOST GATE: SCRIP-built jtran compiles JCON's own sources, and every
# class file it writes is byte-identical to what an Arizona-icont-built jtran writes from the same sources.
#
# WHAT IS COMPARED. jtran (JCON's 17-module Icon-to-JVM translator, corpus/packages/icon/jcon-compiler) is built twice
# from the SAME sources on every run: once by Arizona icont (the oracle, Lon 2026-09-23: "Use Arizona Icon"), once by
# SCRIP in mode 4 from the demo's own link manifest (corpus/demos/icon/jcon/jtran.icn). Each is then run over EVERY module
# of the package through the pipeline JCON's own jcont script uses for a bytecode build:
#     preproc M.icn : yylex : parse : ast2ir : optim -O : bc_File -class:lM -dir:DIR/
# and the two output directories are compared file by file, byte for byte. ⛔ NO JVM IS INSTALLED OR RUN, EVER (s121,
# and Lon 2026-09-23: "Worry not about actually running Java Virtual Machine"): a class file is compared as DATA.
#
# ⭐ TWO HEAP ARMS, BECAUSE THEY ANSWER TWO DIFFERENT QUESTIONS (measured 2026-09-23, hq_icon):
#   --heap=window  (DEFAULT) SCRIP_HEAP_MB=$SELFHOST_WINDOW_MB (2048): a window so large that no collection runs over the
#                  whole package, so a difference here is an ICON-SEMANTICS defect and nothing else. 553 of 553 at SCRIP
#                  27650ec66.
#   --heap=default the shipped arena and cap: the same programs WITH the collector running. A difference that appears here
#                  and not in the window arm is a COLLECTOR defect -- measured: irgen.icn exhausts the 4 MB cap after 281
#                  collections, and at a 16 MB cap reads a stale table pointer in table_icn_nth resuming `every op := !!!t`
#                  inside bc_File's co-expression (the cto's lane, THE COLLECTOR).
# The oracle runs with jcont's own environment: COEXPSIZE=1000000 (every pipeline stage is a co-expression) and HEAPSIZE,
# BLOCKSIZE, MSTKSIZE, QLSIZE unset.
#
# EXIT: 0 every module's class files byte-identical and every exit status equal; 1 a difference (named per module);
#       2 REFUSED -- could not build or could not measure. A gate that cannot measure never reports green.
# USAGE: bash scripts/test_demo_icon_jcon_selfhost.sh [--heap=window|default] [module ...]   (default: every module)
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
S4E="${S4E_HOME:-$(cd "$ROOT/.." && pwd)}"
SCRIP="${SCRIP:-$ROOT/scrip}"; RT="$ROOT/out"
D="${JCON_DEMO_DIR:-$S4E/corpus/demos/icon/jcon}"
PKG="${JCON_PKG_DIR:-$S4E/corpus/packages/icon/jcon-compiler}"
TMO="${JCON_TIMEOUT:-900}"
HEAP=window; WANT=""
for a in "$@"; do case "$a" in --heap=window) HEAP=window;; --heap=default) HEAP=default;; --heap=*) echo "⛔ unknown heap arm $a"; exit 2;; *) WANT="$WANT $a";; esac; done
refuse() { echo "⛔ JCON SELF-HOST GATE UNPROVEN(2): $*"; echo "    This is NOT a pass -- the gate could not measure, so it certifies nothing."; exit 2; }
. "$HERE/lib_oracle_flags.sh" 2>/dev/null || refuse "cannot load lib_oracle_flags.sh -- the ONE oracle-path authority."
ICONT="$(icont_bin)" || refuse "the Arizona icont oracle is missing (lib_oracle_flags.sh names the path)."
[ -x "$SCRIP" ] || refuse "scrip is not built at $SCRIP -- run make."
[ -f "$D/jtran.icn" ] || refuse "the jtran link manifest is missing: $D/jtran.icn"
[ -d "$PKG" ] || refuse "JCON package missing: $PKG"
W="$(mktemp -d "${TMPDIR:-/tmp}/jconself.XXXXXX")" || refuse "cannot make a work dir."
trap 'rm -rf "$W"' EXIT
MODS=$(sed -n 's/^[[:space:]]*link[[:space:]]*"\([^"]*\)".*$/\1/p' "$D/jtran.icn" | sed 's|.*/||')
[ -n "$MODS" ] || refuse "jtran.icn names no modules."
mkdir -p "$W/src" && cp "$PKG"/*.icn "$W/src/" || refuse "cannot stage the package sources."
for m in $MODS; do [ -f "$W/src/$m.icn" ] || refuse "jtran links $m but $PKG/$m.icn does not exist."; done
( cd "$W/src" && "$ICONT" -s -o "$W/jtran_oracle" $(for m in $MODS; do printf '%s.icn ' "$m"; done) ) >"$W/icont.log" 2>&1 \
    || refuse "icont refused to build the oracle jtran: $(tail -2 "$W/icont.log")"
( cd "$D" && "$SCRIP" --compile -o "$W/jtran.s" jtran.icn </dev/null ) >"$W/scrip.log" 2>&1 \
    || refuse "SCRIP could not compile jtran (rc=$?): $(grep -v '^$' "$W/scrip.log" | tail -1 | cut -c1-200)"
as --64 -o "$W/jtran.o" "$W/jtran.s" 2>>"$W/scrip.log" && gcc -no-pie -o "$W/jtran_scrip" "$W/jtran.o" "$RT/libscrip_rt.so" -lm -lstdc++ -Wl,-rpath,"$RT" 2>>"$W/scrip.log" \
    || refuse "SCRIP-built jtran did not assemble or link: $(tail -2 "$W/scrip.log")"
HEAPENV=(); [ "$HEAP" = window ] && HEAPENV=(SCRIP_HEAP_MB="${SELFHOST_WINDOW_MB:-2048}")
printf '%-14s %-18s %-18s %s\n' MODULE ORACLE SCRIP VERDICT
printf '%s\n' "------------------------------------------------------------------------------"
TOT=0; SAME=0; BAD=0; ROWS=0
for f in "$W"/src/*.icn; do
    m="$(basename "$f" .icn)"
    [ -n "$WANT" ] && ! grep -qw "$m" <<<"$WANT" && continue
    ROWS=$((ROWS+1)); mkdir -p "$W/o/$m" "$W/s/$m"
    ( cd "$W/src" && env -u HEAPSIZE -u BLOCKSIZE -u MSTKSIZE -u QLSIZE COEXPSIZE=1000000 timeout "$TMO" "$W/jtran_oracle" \
        preproc "$m.icn" : yylex : parse : ast2ir : optim -O : bc_File -class:"l$m" -dir:"$W/o/$m/" ) >/dev/null 2>"$W/o/$m.err"; orc=$?
    ( cd "$W/src" && env ${HEAPENV[@]+"${HEAPENV[@]}"} timeout "$TMO" "$W/jtran_scrip" \
        preproc "$m.icn" : yylex : parse : ast2ir : optim -O : bc_File -class:"l$m" -dir:"$W/s/$m/" ) >/dev/null 2>"$W/s/$m.err"; src=$?
    no=$(ls "$W/o/$m" | wc -l); ns=$(ls "$W/s/$m" | wc -l); same=0
    for c in "$W/o/$m"/*; do [ -e "$c" ] || continue; TOT=$((TOT+1)); cmp -s "$c" "$W/s/$m/$(basename "$c")" && { same=$((same+1)); SAME=$((SAME+1)); }; done
    extra=$(comm -13 <(ls "$W/o/$m") <(ls "$W/s/$m") | wc -l)
    if [ "$no" -eq 0 ]; then V="VOID -- the oracle wrote no class file (rc=$orc: $(head -c 120 "$W/o/$m.err"))"; BAD=1
    elif [ "$orc" = "$src" ] && [ "$same" -eq "$no" ] && [ "$extra" -eq 0 ]; then V="IDENTICAL"
    else V="DIFF $same/$no identical, $extra extra, rc $orc vs $src$( [ "$src" -ge 128 ] 2>/dev/null && printf ' -- %s' "$(grep -m1 -o '\[Z[A-Z-]*\][^.]*' "$W/s/$m.err" | cut -c1-110)")"; BAD=1; fi
    printf '%-14s %-18s %-18s %s\n' "$m" "rc=$orc files=$no" "rc=$src files=$ns" "$V"
done
printf '%s\n' "------------------------------------------------------------------------------"
[ "$ROWS" -gt 0 ] || refuse "no module matched -- a gate that graded nothing is not a pass."
echo "JCON_SELFHOST heap=$HEAP modules=$ROWS class_files_identical=$SAME of $TOT oracle=$ICONT scrip=$SCRIP RT_OPT=-O0"
if [ "$BAD" = 0 ]; then echo "✅ JCON SELF-HOST PASS(0): SCRIP-built jtran wrote every class file byte-identical to the Arizona-built jtran."; exit 0; fi
echo "⛔ JCON SELF-HOST FAIL(1): at least one module differs (rows above)."; exit 1
