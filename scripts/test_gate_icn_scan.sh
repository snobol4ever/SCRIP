#!/usr/bin/env bash
# scripts/test_gate_icn_scan.sh — ICN-SCAN-FENCE gate (GOAL-ICON-BB.md, ICN-SCAN ladder close-out).
# Four sections per the FENCE spec:
#   (a) all nine IR_SCAN_* kinds OFF icn_kind_native_stub (and present in src/contracts);
#   (b) the ladder probe sweep — every ICN-SCAN-0..13a probe, three modes, policy per documented flags:
#         STRICT  m2==m3==m4==expected AND no [SMX] (an decline must not masquerade as a fail-probe pass)
#         M34     m3==m4==expected, m2 recorded informationally (the SCAN-3 pre-existing oracle pos gap)
#         PIN     m2==m3==m4==ONE-SHOT expected (ORACLE SCAN-FN GENERATIVITY flag: goal probes specify
#                 multi-result; the m2 oracle is one-shot for every scan builtin; native matches the oracle
#                 bit-for-bit and is pump-ready. Making m2 generative SHIFTS THE 129 BASELINE — Lon's call.
#                 Until that re-baseline rung, this gate pins three-mode AGREEMENT on the one-shot value.)
#         X34     m2==expected AND m3 AND m4 each print [SMX] with rc=0 (LOUD DECLINE — the =s var-operand
#                 shape, the wave-1 dynamic-arg declines; SCAN-13b deferral PROMOTED to STRICT at ICN-VAR-3)
#   (c) the corpus scan bucket — every corpus .icn whose --dump-bb graph carries IR_GEN_SCAN, all three
#       modes vs .expected, with ratchet floors (DECLINED->PASS deltas land here as future boxes light up);
#   (d) standing structural gates: no_bb_bin_t . no_handencoded --strict . icn_no_stack .
#       icn_one_reg_frame (HARD); no_vstack informational; medium-invisible scoped to the
#       scan-family templates (the global --strict RED is the documented Prolog-lane bb_* WIP).
# Exit 0 iff (a) + (b) + (c floors) + (d) all hold.
# Authors: LCherryholmes . Jeffrey Cooper M.D. . Claude Opus 4.8   DATE: 2026-06-04
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP:-$ROOT/scrip}"
RT_SO="${RT_SO:-$ROOT/out/libscrip_rt.so}"
CORPUS="${CORPUS:-$S4E/corpus/programs/icon}"
SCAN_M2_MIN="${SCAN_M2_MIN:-31}"
SCAN_M3_MIN="${SCAN_M3_MIN:-11}"
SCAN_M4_MIN="${SCAN_M4_MIN:-11}"
BAD=0
[ -x "$SCRIP" ] || { echo "SKIP scrip not built at $SCRIP — run scripts/build_scrip.sh"; exit 0; }

A2=""; A3=""; A4=""; SMX3=0; SMX4=0; RC3=0
run3() {
    local f="$1" TO="${2:-8}"
    A2=$(timeout "$TO" "$SCRIP" --run "$f" 2>/dev/null </dev/null)
    local e3; e3=$(mktemp)
    A3=$(timeout "$TO" "$SCRIP" --run "$f" 2>"$e3" </dev/null); RC3=$?
    SMX3=0; grep -q '\[SMX\]' "$e3" && SMX3=1; rm -f "$e3"
    A4=""; SMX4=0
    local s4 b4 e4; s4=$(mktemp --suffix=.s); b4=$(mktemp); rm -f "$b4"; e4=$(mktemp)
    timeout "$TO" "$SCRIP" --compile "$f" >"$s4" 2>"$e4" </dev/null || true
    grep -q '\[SMX\]' "$e4" && SMX4=1
    if [ "$SMX4" = 0 ] && [ -s "$s4" ] && [ -f "$RT_SO" ]; then
        if gcc -no-pie "$s4" -L"$ROOT/out" -lscrip_rt -Wl,-rpath,"$ROOT/out" -o "$b4" 2>/dev/null; then
            A4=$(timeout "$TO" "$b4" 2>/dev/null </dev/null)
        fi
    fi
    rm -f "$s4" "$b4" "$e4"
}

PP=0; PF=0
probe() {
    local name="$1" policy="$2" exp="$3"
    local f; f=$(mktemp /tmp/icnscan_XXXXXX.icn); cat > "$f"
    run3 "$f"; rm -f "$f"
    local ok=1 note=""
    case "$policy" in
        STRICT) { [ "$A2" = "$exp" ] && [ "$A3" = "$exp" ] && [ "$A4" = "$exp" ] && [ "$SMX3" = 0 ] && [ "$SMX4" = 0 ]; } || ok=0 ;;
        M34)    { [ "$A3" = "$exp" ] && [ "$A4" = "$exp" ] && [ "$SMX3" = 0 ] && [ "$SMX4" = 0 ]; } || ok=0
                note="(m2='$A2' — SCAN-3 documented oracle pos gap)" ;;
        PIN)    { [ "$A2" = "$exp" ] && [ "$A3" = "$exp" ] && [ "$A4" = "$exp" ] && [ "$SMX3" = 0 ] && [ "$SMX4" = 0 ]; } || ok=0
                note="(one-shot pin — ORACLE GENERATIVITY flag)" ;;
        X34)    { [ "$A2" = "$exp" ] && [ "$SMX3" = 1 ] && [ "$SMX4" = 1 ] && [ "$RC3" = 0 ]; } || ok=0
                note="(m3/m4 LOUD DECLINED by design)" ;;
    esac
    if [ "$ok" = 1 ]; then PP=$((PP+1)); printf "  OK   %-22s %s\n" "$name" "$note"
    else PF=$((PF+1)); BAD=1; printf "  FAIL %-22s m2='%s' m3='%s'(smx=%s) m4='%s'(smx=%s) exp='%s' %s\n" "$name" "$A2" "$A3" "$SMX3" "$A4" "$SMX4" "$exp" "$note"; fi
}

echo "=== ICN-SCAN-FENCE gate ==="
echo "--- (a) icn_kind_native_stub carries ZERO IR_SCAN_* kinds ---"
stub_hits=$(awk '/static int icn_kind_native_stub/{f=1} f{print} f&&/^\}/{exit}' "$ROOT/src/driver/scrip.c" | grep -c 'IR_SCAN_' || true)
if [ "$stub_hits" = 0 ]; then echo "  OK   stub-list IR_SCAN_* count = 0"; else echo "  FAIL stub-list IR_SCAN_* count = $stub_hits (must be 0)"; BAD=1; fi
for k in POS ANY MATCH MANY TAB MOVE UPTO FIND BAL; do
    grep -rq "IR_SCAN_$k" "$ROOT/src/contracts/" || { echo "  FAIL IR_SCAN_$k missing from src/contracts/"; BAD=1; }
done
echo "  OK   all nine IR_SCAN_* kinds present in src/contracts/"

echo "--- (b) ladder probe sweep (three modes per probe) ---"
probe scan_subject STRICT "hello" << 'EOF'
procedure main()
  "hello" ? write(&subject);
end
EOF
probe scan_concat_subject STRICT "foobar" << 'EOF'
procedure main()
  ("foo" || "bar") ? write(&subject);
end
EOF
probe scan_pos_kw STRICT "1" << 'EOF'
procedure main()
  "hello" ? write(&pos);
end
EOF
probe scan_nested STRICT "inner" << 'EOF'
procedure main()
  "outer" ? ("inner" ? write(&subject));
end
EOF
probe pos_1 M34 "1" << 'EOF'
procedure main()
  "abc" ? write(pos(1));
end
EOF
probe pos_fail STRICT "" << 'EOF'
procedure main()
  "hello" ? write(pos(2));
end
EOF
probe any_h STRICT "2" << 'EOF'
procedure main()
  "hello" ? write(any('h'));
end
EOF
probe any_multi STRICT "2" << 'EOF'
procedure main()
  "hello" ? write(any('xeh'));
end
EOF
probe any_fail STRICT "" << 'EOF'
procedure main()
  "hello" ? write(any('x'));
end
EOF
probe any_empty_subject STRICT "" << 'EOF'
procedure main()
  "" ? write(any('h'));
end
EOF
probe any_dynamic_arg X34 "2" << 'EOF'
procedure main()
  c := 'h';
  "hello" ? write(any(c));
end
EOF
probe match_he STRICT "3" << 'EOF'
procedure main()
  "hello" ? write(match("he"));
end
EOF
probe match_fail STRICT "" << 'EOF'
procedure main()
  "hello" ? write(match("xx"));
end
EOF
probe match_subj_short STRICT "" << 'EOF'
procedure main()
  "he" ? write(match("hello"));
end
EOF
probe match_empty STRICT "1" << 'EOF'
procedure main()
  "hello" ? write(match(""));
end
EOF
probe many_spaces STRICT "3" << 'EOF'
procedure main()
  "  x" ? write(many(' '));
end
EOF
probe many_fail STRICT "" << 'EOF'
procedure main()
  "hello" ? write(many(' '));
end
EOF
probe tab_3 STRICT "he" << 'EOF'
procedure main()
  "hello" ? write(tab(3));
end
EOF
probe tab_upto STRICT "he" << 'EOF'
procedure main()
  "hello" ? write(tab(upto('l')));
end
EOF
probe move_2 STRICT "he" << 'EOF'
procedure main()
  "hello" ? write(move(2));
end
EOF
probe upto_oneshot PIN "3" << 'EOF'
procedure main()
  "hello" ? every write(upto('l'));
end
EOF
probe find_oneshot PIN "2" << 'EOF'
procedure main()
  "banana" ? every write(find("an"));
end
EOF
probe bal_b STRICT "4" << 'EOF'
procedure main()
  "(a)b" ? every write(bal('b'));
end
EOF
probe eq_match_he STRICT "he" << 'EOF'
procedure main()
  "hello" ? write(="he");
end
EOF
probe eq_match_fail STRICT "" << 'EOF'
procedure main()
  "hello" ? write(="x");
end
EOF
probe eq_match_var X34 "he" << 'EOF'
procedure main()
  s := "he";
  "hello" ? write(=s);
end
EOF
probe augop_scan STRICT "he" << 'EOF'
procedure main()
  s := "hello";
  s ?:= tab(3);
  write(s);
end
EOF
probe augop_scan_fail STRICT "hello" << 'EOF'
procedure main()
  s := "hello";
  s ?:= tab(99);
  write(s);
end
EOF
echo "  probes: PASS=$PP FAIL=$PF"

echo "--- (c) corpus IR_GEN_SCAN bucket (ratchet floors m2>=$SCAN_M2_MIN m3>=$SCAN_M3_MIN m4>=$SCAN_M4_MIN) ---"
C2P=0; C2F=0; C3P=0; C3F=0; C3E=0; C4P=0; C4F=0; C4E=0; CN=0
while IFS= read -r f; do
    dump=$(timeout 30 "$SCRIP" --dump-bb "$f" 2>/dev/null </dev/null) || true
    case "$dump" in *GEN_SCAN*) ;; *) continue ;; esac
    CN=$((CN+1))
    exp=$(cat "${f%.icn}.expected" 2>/dev/null || true)
    run3 "$f" 30
    if [ "$A2" = "$exp" ]; then r2=PASS; C2P=$((C2P+1)); else r2=FAIL; C2F=$((C2F+1)); fi
    if [ "$SMX3" = 1 ]; then r3=DECLINED; C3E=$((C3E+1)); elif [ "$A3" = "$exp" ]; then r3=PASS; C3P=$((C3P+1)); else r3=FAIL; C3F=$((C3F+1)); fi
    if [ "$SMX4" = 1 ]; then r4=DECLINED; C4E=$((C4E+1)); elif [ "$A4" = "$exp" ]; then r4=PASS; C4P=$((C4P+1)); else r4=FAIL; C4F=$((C4F+1)); fi
    printf "  %-44s m2=%-4s m3=%-7s m4=%s\n" "$(basename "$f" .icn)" "$r2" "$r3" "$r4"
done < <(find "$CORPUS" -name '*.icn' | sort)
echo "  bucket: N=$CN | m2 PASS=$C2P FAIL=$C2F | m3 PASS=$C3P FAIL=$C3F DECLINED=$C3E | m4 PASS=$C4P FAIL=$C4F DECLINED=$C4E"
[ "$C2P" -ge "$SCAN_M2_MIN" ] || { echo "  FLOOR FAIL m2 $C2P < $SCAN_M2_MIN"; BAD=1; }
[ "$C3P" -ge "$SCAN_M3_MIN" ] || { echo "  FLOOR FAIL m3 $C3P < $SCAN_M3_MIN"; BAD=1; }
[ "$C4P" -ge "$SCAN_M4_MIN" ] || { echo "  FLOOR FAIL m4 $C4P < $SCAN_M4_MIN"; BAD=1; }

echo "--- (d) standing structural gates ---"
gate_hard() {
    local label="$1"; shift
    if "$@" >/dev/null 2>&1; then echo "  OK   $label"
    else echo "  FAIL $label"; BAD=1; fi
}
gate_hard "no_bb_bin_t"            bash "$HERE/test_gate_no_bb_bin_t.sh"
gate_hard "no_handencoded --strict" bash "$HERE/test_gate_no_handencoded_bytes.sh" --strict
gate_hard "icn_no_stack"           bash "$HERE/test_gate_icn_no_stack.sh"
gate_hard "icn_one_reg_frame"      bash "$HERE/test_gate_icn_one_reg_frame.sh"
bash "$HERE/test_gate_no_vstack.sh" 2>/dev/null | tail -1 | sed 's/^/  INFO /'
mi_line=$(bash "$HERE/test_gate_template_medium_invisible.sh" 2>/dev/null | grep '^REMAINING:' || true)
if echo "$mi_line" | grep -qE 'bb_scan_|bb_gen_scan|bb_keyword'; then
    echo "  FAIL medium-invisible: a scan-family template carries raw-byte/medium-branch sites"; BAD=1
else
    echo "  OK   medium-invisible (scan-family templates clean; global --strict RED = documented Prolog-lane bb_* WIP)"
fi

echo "==="
if [ "$BAD" = 0 ]; then echo "ICN-SCAN-FENCE GATE: PASS"; exit 0
else echo "ICN-SCAN-FENCE GATE: FAIL"; exit 1; fi
