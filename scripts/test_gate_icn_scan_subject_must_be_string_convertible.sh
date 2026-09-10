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
printf '%s\n' 'procedure main();&error := -1;write(image([] ? []) | "none");write("done");end'                      > "$TD/conv_scan.icn"
printf '%s\n' 'procedure main();&error := -1;write(image([] ? []) | "none");write(image(=[]) | "none");write("done");end' > "$TD/conv_match.icn"
printf '%s\n' 'procedure main();&error := -1;"abc" ? write(image(move("a")) | "none");write("done");end'                 > "$TD/conv_move.icn"
printf '%s\n' 'procedure main();&error := -1;"abc" ? write(image(pos("a")) | "none");write("done");end'                  > "$TD/conv_pos.icn"
printf '%s\n' 'procedure main();&error := -1;"abc" ? write(image(tab("x")) | "none");write("done");end'                  > "$TD/conv_tab.icn"
printf '%s\n' 'procedure main();&error := -1;"abc" ? write(image(upto([])) | "none");write("done");end'                  > "$TD/conv_upto.icn"
printf '%s\n' 'procedure main();&error := -1;"abc" ? write(image(any([])) | "none");write("done");end'                   > "$TD/conv_any.icn"
printf '%s\n' 'procedure main();&error := -1;"abc" ? write(image(many([])) | "none");write("done");end'                  > "$TD/conv_many.icn"
printf '%s\n' 'procedure main();&error := -1;"abc" ? write(image(find([])) | "none");write("done");end'                  > "$TD/conv_find.icn"
printf '%s\n' 'procedure main();&error := -1;"abc" ? write(image(bal([])) | "none");write("done");end'                   > "$TD/conv_bal.icn"
printf '%s\n' 'procedure main();&error := -1;write(image(pos("a")) | "none");write("done");end'                          > "$TD/conv_pos_top.icn"
EXP_conv_scan='none
done'
EXP_conv_match='none
none
done'
EXP_conv_move=$EXP_conv_scan
EXP_conv_pos=$EXP_conv_scan
EXP_conv_tab=$EXP_conv_scan
EXP_conv_upto=$EXP_conv_scan
EXP_conv_any=$EXP_conv_scan
EXP_conv_many=$EXP_conv_scan
EXP_conv_find=$EXP_conv_scan
EXP_conv_bal=$EXP_conv_scan
EXP_conv_pos_top=$EXP_conv_scan
EXP_ok_str='a'
EXP_ok_int='12'
EXP_ok_real='2.5'
EXP_ok_cset='abc'
REJECTS="rej_list rej_var_list rej_null rej_table rej_set rej_proc rej_file rej_nested"
ACCEPTS="ok_str ok_int ok_real ok_cset conv_scan conv_match conv_move conv_pos conv_tab conv_upto conv_any conv_many conv_find conv_bal"
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
echo "graded=$GRADED FAIL=$FAIL (8 rejects x2 modes: a scan subject that is not string-convertible raises Icon 103, pinned from icont/iconx v9.5.25a; 4 accepts x2: string/integer/real/cset must still convert. The refusal is raised through core_icn_error, so it honours &error/&errorlimit like every other Icon error; and 10 CONVERSION accepts x2, added 2026-09-10 by hq_U when that class was CURED: under &error := -1 each of these raises 103/104/101 and the box must CONCEDE, so the alternative runs and the program reaches its next statement, exactly as icont/iconx v9.5.25a does -- every one of the 10 verdicts was re-cut from the oracle, not from us. ⛔ THIS PARENTHESIS USED TO EXCLUDE THEM, naming &error conversion a separate pre-existing class and citing FINDING-2026-09-09-hq_U-the-checked-icon-helpers-raise-but-their-boxes-have-no-failure-edge-so-ampersand-error-converts-nothing.md. The gap was load-bearing and not clerical: a guard that RAISES correctly and a guard whose answer nobody reads are indistinguishable from outside, so the 8 rejects above passed for days over a scan family that, one &error assignment away, SIGSEGVd in memcmp on a NULL subject (conv_match is that exact witness) and looped forever on move(\"a\") (conv_move). ⭐ The rejects grade the raise; only these 11 grade the EDGE the raise is supposed to take. ⛔ AND THEY DO NOT ALL DISCRIMINATE, which is stated here rather than left for a reader to assume, because a gate that does not say WHAT KIND of wrongness it can see prints a denominator describing its arms and not the defect space: MEASURED on the pre-cure binary, 5 of the 11 go RED (conv_scan wrong VALUE list_2(0), conv_match rc=139 SIGSEGV, conv_move and conv_tab wrong VALUE, conv_pos_top answers 1 where icont concedes) and 6 do NOT (conv_pos inside a scan, conv_upto conv_any conv_many conv_find conv_bal -- pre-cure rt_scan_needle coerced the rejected list to the empty string and the box then failed for its own ordinary reason, so the OUTCOME was right by accident while the mechanism was wrong). Those 6 are ORACLE-AGREEMENT arms, not detectors; they are kept because they pin agreement with icont and would catch a future cure that overshoots into refusing a legal case, and they must never be counted as evidence that this class is detectable. ⛔ conv_pos_top IS MINTED ABOVE AND DELIBERATELY NOT GRADED HERE, and the reason is scope and not convenience: pos("a") at TOP LEVEL has NO scanning subject at all, so it is outside this gate own question, and it does not reach any scan box -- --dump-ir shows it lowering to a plain CALL "pos" that lands in the by-name leaf at src/runtime/by_name_dispatch.c BID_pos, where to_int("a") yields 0 and the leaf answers 1 where icont concedes. That is THE SAME CLASS ON THE CALL ROUTE, it is still OPEN as of 2026-09-10, it is hq_U row icon-converted-error-does-not-concede-on-the-by-name-call-route-pos, and it is left RED-AND-NAMED rather than quietly dropped: the witness is here for whoever takes that row, one uncommented line away from being graded. It is not cured in the same landing because that leaf is SHARED by-name dispatch and a concede edge added there needs its own SNOBOL4 control arm. Cure and both faces: FINDING-2026-09-10-hq_U-a-converted-icon-error-did-not-concede-so-the-scan-family-ran-on-a-value-it-never-produced.md)"
if [ "$FAIL" -ne 0 ]; then echo "GATE FAIL(1) [$NAME]: FAIL=$FAIL of $GRADED"; exit 1; fi
echo "GATE PASS(0) [$NAME]: $GRADED/$GRADED"
exit 0
