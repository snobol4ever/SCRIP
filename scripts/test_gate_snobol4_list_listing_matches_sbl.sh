#!/usr/bin/env bash
"$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/util_require_fresh.sh" --gate "$(basename "${BASH_SOURCE[0]}" .sh)" || exit $?
# test_gate_snobol4_list_listing_matches_sbl.sh -- THE -LIST LISTING IS SPITBOL'S LISTING, BYTE FOR BYTE, IN BOTH MODES (row
# snobol4-the-list-control-line-prints-spitbols-listing-comments-pages-and-banner-included; Lon 2026-10-03 to the ceo, CEO-1459: "We need to
# implement the -LIST feature, since statement numbers are unknown. The DEBUG.sno program depends on a listing."; the cut at the END line is
# CEO-1739: the run-time error report after it is the one error voice's, not this gate's). Every witness is run by the correctness oracle
# (sbl -bf) and by SCRIP's mode 3 and mode 4; each output drops the 'x86-64 ' banner lines and stops at the END statement line of the listing.
# WITNESSES: SourceLines001 (dotnet), hide (csnobol4_suite), debug.sno with -LIST in place of its -NOLIST, and generated probes: -TITLE -STITL
# -SPACE -EJECT -DOUBLE -SINGLE, -PRINT/-NOPRINT/-NOLIST, a comma chain, a tab-indented line, and programs of 119 and 1199 statements that cross
# the 49-53 line page boundary and the 1200 line mark. KNOWN GAPS (not witnessed here, each its own landing): an -INCLUDEd file's images carry
# their depth in column 5; a blank statement with a whitespace-only continuation lists the continuation; -SPACE x is ERROR 247.
# FAIL_ONCE (recorded 2026-10-14, a worktree of 2bbadd1c9^ = 539cc4a3d, the commit before the cure): 16 of 16 listings RED (banner 1.0, comments and page
# breaks missing, debug_list 273 and 214 lines against sbl's 619); this tree reads 16 of 16 SAME.
# EXIT 0 every witness in both modes; 1 a red (named, with the first differing line); 2 REFUSED (oracle, build, or sbl printed no listing).
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"; S4E="${S4E_HOME:-$(cd "$ROOT/.." && pwd)}"
. "$HERE/lib_oracle_flags.sh"
SBL="$(sbl_correctness_bin)"; [ -x "$SBL" ] || { echo "REFUSED(2): correctness oracle missing ($SBL)"; exit 2; }
[ -x "$ROOT/scrip" ] && [ -f "$ROOT/out/libscrip_rt.so" ] || { echo "REFUSED(2): $ROOT/scrip or out/libscrip_rt.so not built"; exit 2; }
C="$S4E/corpus"
for f in packages/snobol4/dotnet/SourceLines001.sno packages/snobol4/csnobol4_suite/hide.sno programs/lon_cherryholmes/sno/debug.sno; do
    [ -f "$C/$f" ] || { echo "REFUSED(2): witness $C/$f missing"; exit 2; }
done
W="$(mktemp -d)" || exit 2; trap 'rm -rf "$W"' EXIT
cd "$W" || exit 2
cp "$C/packages/snobol4/dotnet/SourceLines001.sno" "$C/packages/snobol4/csnobol4_suite/hide.sno" . || exit 2
sed '1s/^-NOLIST/-LIST/' "$C/programs/lon_cherryholmes/sno/debug.sno" > debug_list.sno || exit 2
grep -q '^-LIST' debug_list.sno || { echo "REFUSED(2): debug.sno no longer opens with -NOLIST, the witness is not cut"; exit 2; }
cat > p_title.sno <<'SNO'
-LIST
-TITLE A title here
-STITL a sub title
 X = 1
-EJECT
 Y = 2
-SPACE 3
 Z = 3
-DOUBLE
 W = 4
-SINGLE
 OUTPUT = X + Y
END
SNO
cat > p_print.sno <<'SNO'
-LIST
 A = 1
-NOPRINT
 B = 2
-PRINT
 C = 3
-NOLIST
 D = 4
-LIST
 E = 5
* a comment
 OUTPUT = A + C + E
END
SNO
printf -- '-LIST,-EJECT\n A = 1\n-TITLE x,-LIST\n\tB = 2\n OUTPUT = A + B\nEND\n' > p_chain.sno
{ echo '-LIST'; for i in $(seq 1 119); do echo " V$i = $i"; done; printf ' OUTPUT = V1\nEND\n'; } > p_page50.sno
{ echo '-LIST'; for i in $(seq 1 1199); do echo " V = $i"; done; printf ' OUTPUT = V\nEND\n'; } > p_page1200.sno
cut_at_end() { grep -v '^x86-64 ' | awk '{print} /^[0-9]+ +END( |$)/{exit}'; }
rc=0; n=0
for src in SourceLines001 hide debug_list p_title p_print p_chain p_page50 p_page1200; do
    timeout 60 "$SBL" -bf $src.sno < /dev/null 2>/dev/null | cut_at_end > $src.o
    [ "$(grep -c 'spitbol version' $src.o)" -ge 1 ] || { echo "REFUSED(2): sbl printed no listing for $src"; exit 2; }
    timeout 60 "$ROOT/scrip" --stlimit $src.sno < /dev/null 2>/dev/null | cut_at_end > $src.m3
    timeout 60 "$ROOT/scrip" --compile -o $src.s $src.sno < /dev/null > /dev/null 2>&1 \
        && gcc -m64 -no-pie $src.s -Wl,-rpath,"$ROOT/out" -L"$ROOT/out" -lscrip_rt -lm -lpthread -o $src.bin > /dev/null 2>&1 \
        && timeout 60 ./$src.bin --stlimit < /dev/null 2>/dev/null | cut_at_end > $src.m4 || : > $src.m4
    for m in m3 m4; do
        n=$((n+1))
        if cmp -s $src.o $src.$m; then continue; fi
        echo "RED $src $m: $(wc -l < $src.o) sbl lines, $(wc -l < $src.$m) scrip lines, first difference: $(diff $src.o $src.$m | sed -n 2p | cut -c1-70)"
        rc=1
    done
done
[ $rc = 0 ] && echo "PASS: $n listings (8 witnesses x 2 modes) byte-identical to sbl through the END line"
exit $rc
