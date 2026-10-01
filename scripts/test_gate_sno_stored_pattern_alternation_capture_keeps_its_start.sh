#!/usr/bin/env bash
# test_gate_sno_stored_pattern_alternation_capture_keeps_its_start.sh -- the coo's four-line witness of 2026-10-01 11:4x
# (round 2 of the testing officer's loop at 934c76f9f, bisected to 093e95d68, the stored-pattern leaf cells on the RSP
# spine): a BREAK, SPAN or REM leaf inside an alternation arm of a stored pattern captured with . lost the capture's start
# cursor on the spine road -- 'ABS x' ? PAT read [BS] for sbl's [ABS], and the SPAN arm refused as a CORRUPT CAPTURE
# ENTRY. It took AIS 8/8 to 6/8 and snoflake's wang-theorem-prover red; the masters did not see it. Ruled CEO-1389: the
# shipped default is the leaf-FRAME road again until the spine road passes this witness; SCRIP_LEAF_FRAME=0 selects the
# spine road and is the cto's control arm on the row. ARMS 1-2 grade the DEFAULT in both modes against sbl -bf and BLOCK;
# ARM 3 runs the spine road and REPORTS its answer, never failing, so the row's progress is visible without anchoring a
# gate on the defect (CEO-554).
set -u
HERE="$(cd "$(dirname "$0")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"; SCRIP="$ROOT/scrip"; RT="$ROOT/out"
. "$HERE/lib_oracle_flags.sh" 2>/dev/null || true
SBL="${SBL:-$(sbl_correctness_bin 2>/dev/null || echo /home/resources/x64/bin/sbl)}"
[ -x "$SCRIP" ] && [ -f "$RT/libscrip_rt.so" ] || { echo "⛔ REFUSE(2): no ./scrip or out/libscrip_rt.so -- build first"; exit 2; }
[ -x "$SBL" ] || { echo "⛔ REFUSE(2): no SPITBOL oracle at $SBL"; exit 2; }
W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT; trap 'rm -rf "$W"; exit 143' TERM; trap 'rm -rf "$W"; exit 130' INT
cat > "$W/cap.sno" <<'SNO'
        &ANCHOR = 1
        PAT = (ANY('ABCDEFGHIJKLMNOPQRSTUVWXYZ') (BREAK(' ') | REM)) . LABEL
        'ABS x' ? PAT
        OUTPUT = '[' LABEL ']'
        'ACOS' ? PAT
        OUTPUT = '[' LABEL ']'
        P2 = (ANY('AB') (SPAN('C') | REM)) . L2
        'ACCD' ? P2
        OUTPUT = '[' L2 ']'
END
SNO
fail=0; ck() { if [ "$1" = ok ]; then echo "  ✅ $2"; else echo "  ⛔ $2"; fail=$((fail+1)); fi; }
ref="$("$SBL" -bf "$W/cap.sno" </dev/null 2>/dev/null)"
[ "$ref" = "$(printf '[ABS]\n[ACOS]\n[ACC]')" ] || { echo "⛔ REFUSE(2): the oracle did not answer [ABS] [ACOS] [ACC] -- got: $ref"; exit 2; }
m3="$(cd "$W" && env -u SCRIP_LEAF_FRAME timeout 10 "$SCRIP" cap.sno </dev/null 2>&1)"
ck "$([ "$m3" = "$ref" ] && echo ok || echo no)" "mode 3, shipped default: the three captures match sbl (got: $(printf '%s' "$m3" | tr '\n' ' ' | cut -c1-80))"
(cd "$W" && env -u SCRIP_LEAF_FRAME timeout 10 "$SCRIP" --compile -o cap.s cap.sno </dev/null >/dev/null 2>&1 && gcc -no-pie -o cap cap.s "$RT/libscrip_rt.so" -lm -lstdc++ -Wl,-rpath,"$RT" 2>/dev/null)
m4="$(cd "$W" && timeout 10 ./cap </dev/null 2>&1)"
ck "$([ "$m4" = "$ref" ] && echo ok || echo no)" "mode 4, shipped default: the same (got: $(printf '%s' "$m4" | tr '\n' ' ' | cut -c1-80))"
sp="$(cd "$W" && SCRIP_LEAF_FRAME=0 timeout 10 "$SCRIP" cap.sno </dev/null 2>&1 | tr '\n' ' ' | cut -c1-100)"
if [ "$(printf '%s' "$sp" | tr -d ' ')" = "$(printf '%s' "$ref" | tr -d '\n')" ]; then echo "  ✅ REPORTED: the spine road (SCRIP_LEAF_FRAME=0) matches sbl too -- the row may flip the default back"; else echo "  ⚠ REPORTED: the spine road (SCRIP_LEAF_FRAME=0) still reads: $sp"; fi
[ "$fail" = 0 ] && { echo "✅ GATE OK: a stored pattern's alternation-arm leaf keeps the capture's start cursor on the shipped road, both modes"; exit 0; }
echo "⛔ GATE RED: $fail arm(s) failed"; exit 1
