#!/usr/bin/env bash
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="$ROOT/scrip"
NAME=icn_scan_subject_must_be_string_convertible
refuse() { echo "GATE REFUSE(2) [$NAME]: $*"; exit 2; }
[ -x "$SCRIP" ] || refuse "no scrip binary at $SCRIP -- cannot measure"
. "$HERE/lib_gate.sh"
gate_require_fresh "$ROOT" src "$ROOT/scrip" "$ROOT/out/libscrip_rt.so"
TD="$(mktemp -d)"; trap 'rm -rf "$TD"' EXIT
printf '%s\n' 'procedure main();[] ? write("in");end'                                            > "$TD/rej_list.icn"
printf '%s\n' 'procedure main();local L;L := [1,2];L ? write("in");end'                          > "$TD/rej_var_list.icn"
printf '%s\n' 'procedure main();&null ? write("in");end'                                         > "$TD/rej_null.icn"
printf '%s\n' 'procedure main();table() ? write("in");end'                                       > "$TD/rej_table.icn"
printf '%s\n' 'procedure main();set() ? write("in");end'                                         > "$TD/rej_set.icn"
printf '%s\n' 'procedure main();main ? write("in");end'                                          > "$TD/rej_proc.icn"
printf '%s\n' 'procedure main();&output ? write("in");end'                                       > "$TD/rej_file.icn"
printf '%s\n' 'procedure main();"ab" ? { [] ? write("inner") };write("after");end'               > "$TD/rej_nested.icn"
printf '%s\n' 'procedure main();"abc" ? write(tab(2));end'                                       > "$TD/ok_str.icn"
printf '%s\n' 'procedure main();12345 ? write(tab(3));end'                                       > "$TD/ok_int.icn"
printf '%s\n' 'procedure main();2.5 ? write(&subject);end'                                       > "$TD/ok_real.icn"
printf '%s\n' "procedure main();('abc') ? write(&subject);end"                                   > "$TD/ok_cset.icn"
EXP_ok_str='a'
EXP_ok_int='12'
EXP_ok_real='2.5'
EXP_ok_cset='abc'
REJECTS="rej_list rej_var_list rej_null rej_table rej_set rej_proc rej_file rej_nested"
ACCEPTS="ok_str ok_int ok_real ok_cset"
GRADED=0; FAIL=0
build_m4() { "$SCRIP" --compile -o "$TD/$1.s" "$TD/$1.icn" </dev/null >/dev/null 2>&1 || refuse "$1: mode-4 compile failed -- cannot measure"
  gcc -no-pie "$TD/$1.s" -o "$TD/$1.bin" -L "$ROOT/out" -lscrip_rt -lm -lpthread >/dev/null 2>&1 || refuse "$1: mode-4 link failed -- cannot measure"; }
for w in $REJECTS $ACCEPTS; do
  for m in 3 4; do
    if [ "$m" = 3 ]; then out="$(timeout 20s "$SCRIP" "$TD/$w.icn" </dev/null 2>&1)"; rc=$?
    else build_m4 "$w"; out="$(LD_LIBRARY_PATH="$ROOT/out" timeout 20s "$TD/$w.bin" </dev/null 2>&1)"; rc=$?; fi
    GRADED=$((GRADED+1))
    [ "$rc" = 124 ] && { echo "  FAIL m$m $w: timed out"; FAIL=$((FAIL+1)); continue; }
    case " $REJECTS " in
      *" $w "*)
        if [ "$rc" -eq 0 ] || ! printf '%s' "$out" | grep -q 'Run-time error 103'; then
          echo "  FAIL m$m $w: want rc!=0 and 'Run-time error 103', got rc=$rc [$(printf '%s' "$out" | tr '\n' '/')]"
          FAIL=$((FAIL+1)); continue
        fi ;;
      *)
        eval "want=\$EXP_$w"
        if [ "$rc" -ne 0 ] || [ "$out" != "$want" ]; then
          echo "  FAIL m$m $w: rc=$rc got [$(printf '%s' "$out" | tr '\n' '/')] want [$(printf '%s' "$want" | tr '\n' '/')]"
          FAIL=$((FAIL+1)); continue
        fi ;;
    esac
    echo "  PASS m$m $w"
  done
done
[ "$GRADED" -eq 0 ] && refuse "graded zero witnesses"
echo "graded=$GRADED FAIL=$FAIL (8 rejects x2 modes: a scan subject that is not string-convertible raises Icon 103, pinned from icont/iconx v9.5.25a; 4 accepts x2: string/integer/real/cset must still convert. The refusal is raised through core_icn_error, so it honours &error/&errorlimit like every other Icon error; it is NOT graded here because &error CONVERSION is a separate pre-existing class -- tab("x") and "a" to 5 under &error := 1 already produce neither the error nor the failure, measured on a build without this cure -- see FINDING-2026-09-09-hq_U-the-checked-icon-helpers-raise-but-their-boxes-have-no-failure-edge-so-ampersand-error-converts-nothing.md)"
if [ "$FAIL" -ne 0 ]; then echo "GATE FAIL(1) [$NAME]: FAIL=$FAIL of $GRADED"; exit 1; fi
echo "GATE PASS(0) [$NAME]: $GRADED/$GRADED"
exit 0
