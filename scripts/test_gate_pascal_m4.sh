#!/bin/bash
# test_gate_pascal_m4.sh — Pascal M4 (--compile -> gcc -> link -> run) gate
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
SCRIP="${SCRIP:-$S4E/SCRIP/scrip}"
CORPUS="${CORPUS:-$S4E/corpus/tests/pascal}"
HARNESS="${HARNESS:-$(dirname "${BASH_SOURCE[0]}")/corpus_suite_harness.py}"
MASTER_SRC="${MASTER_SRC:-$CORPUS/ALL.pas}"
MASTER_REF="${MASTER_REF:-$CORPUS/ALL.ref}"
# ⭐ REPOINTED (seat04, 2026-08-30, row pascal-master-flatten-and-scrip-test-pas) -- see test_gate_pascal_m3.sh
# for the full rationale (kept byte-identical in substance between the two files, same reason as before).
# ⛔ REVERTED BACK TO THE FLAT PATH (seat04, 2026-08-30) after a stale-checkout mixup briefly pointed this at
# the already-deleted `master/` subdir -- see test_gate_pascal_m3.sh for the full timeline (kept
# byte-identical in substance between the two files, same reason as before).
# ⛔ HAND-MAINTAINED -- keep byte-identical to test_gate_pascal_m3.sh's STDIN_FAMILIES list.
STDIN_FAMILIES="read1 read2 read3 read4 pb35"
RESULTS="${RESULTS:-/tmp/m4_results.tsv}"
RT="${RT:-$S4E/SCRIP/out/libscrip_rt.so}"
TMP=$(mktemp -d)
trap "rm -rf $TMP" EXIT
PASS=0; FAIL=0; NOREF=0; XFAIL=0; EXAMINED=0
echo -e "probe\tclass\tnotes" > "$RESULTS"

for name in $STDIN_FAMILIES; do
    pas="$CORPUS/$name.pas"; ref="$CORPUS/$name.ref"
    EXAMINED=$((EXAMINED+1))
    if [ ! -f "$pas" ] || [ ! -f "$ref" ]; then
        echo -e "$name\tNOREF\t" >> "$RESULTS"
        NOREF=$((NOREF+1)); continue
    fi
    asm="$TMP/$name.s"
    obj="$TMP/$name.o"
    exe="$TMP/$name"
    timeout 8s "$SCRIP" --compile "$pas" < /dev/null > "$asm" 2>/dev/null
    if [ $? -ne 0 ] || [ ! -s "$asm" ]; then
        echo -e "$name\tCOMPILE_FAIL\t" >> "$RESULTS"
        FAIL=$((FAIL+1)); continue
    fi
    gcc -c -o "$obj" "$asm" 2>/dev/null && \
    gcc -o "$exe" "$obj" "$RT" -no-pie -Wl,-rpath,"$(dirname $RT)" 2>/dev/null
    if [ $? -ne 0 ] || [ ! -x "$exe" ]; then
        echo -e "$name\tLINK_FAIL\t" >> "$RESULTS"
        FAIL=$((FAIL+1)); continue
    fi
    inp="$CORPUS/$name.in"; [ -f "$inp" ] || inp=/dev/null
    out=$(timeout 8s "$exe" < "$inp" 2>/dev/null)
    rc=$?
    exp=$(cat "$ref")
    if [ $rc -eq 124 ]; then
        echo -e "$name\tTIMEOUT\t" >> "$RESULTS"
        FAIL=$((FAIL+1))
    elif [ -z "$out" ] && [ $rc -ne 0 ]; then
        echo -e "$name\tEMPTY_rc$rc\t" >> "$RESULTS"
        FAIL=$((FAIL+1))
    elif [ "$out" = "$exp" ]; then
        echo -e "$name\tPASS\t" >> "$RESULTS"
        PASS=$((PASS+1))
    else
        echo -e "$name\tWRONG_rc$rc\t" >> "$RESULTS"
        FAIL=$((FAIL+1))
    fi
done

MASTER_PASS=0; MASTER_FAIL=0; MASTER_EXAMINED=0
if [ -f "$MASTER_SRC" ] && [ -f "$MASTER_REF" ]; then
    # ⭐ --by-modes-column (seat11, row pascal-master-eleven-reds-cured, 2026-09-04): ALL.csv marks the 5
    # `parser__*` entries `modes=ast` (graded by --dump-ast diff, never executed) — invoking plain `--modes m4`
    # silently ran them as m4 anyway and misreported all 5 as output-mismatch FAILs. Same bug class already hit
    # Raku once (SCORE.md's raku row: a modes-blind invocation collapsed run-graded entries into the ast bucket).
    board=$(timeout 180s python3 "$HARNESS" run "$MASTER_SRC" "$MASTER_REF" --lang pascal --by-modes-column --modes m4 2>/dev/null)
    p=$(grep -oP '(?<=m4_pass=)\d+' <<<"$board"); f=$(grep -oP '(?<=m4_fail=)\d+' <<<"$board")
    crash=$(grep -oP '(?<=m4_crash=)\d+' <<<"$board"); hang=$(grep -oP '(?<=m4_hang=)\d+' <<<"$board")
    unproven=$(grep -oP '(?<=m4_unproven=)\d+' <<<"$board")
    total=$(grep '^SUITE_BOARD ' <<<"$board" | grep -oP '(?<=total=)\d+')
    ast_p=$(grep -oP '(?<=ast_pass=)\d+' <<<"$board"); ast_f=$(grep -oP '(?<=ast_fail=)\d+' <<<"$board")
    ast_crash=$(grep -oP '(?<=ast_crash=)\d+' <<<"$board"); ast_hang=$(grep -oP '(?<=ast_hang=)\d+' <<<"$board")
    ast_unproven=$(grep -oP '(?<=ast_unproven=)\d+' <<<"$board")
    ast_total=$(grep '^SUITE_BOARD_AST ' <<<"$board" | grep -oP '(?<=total=)\d+')
    if [ -z "$p" ]; then
        echo -e "master:ALL\tHARNESS_UNPROVEN\t" >> "$RESULTS"
        FAIL=$((FAIL+1))
    else
        ast_bad=$(( ${ast_f:-0} + ${ast_crash:-0} + ${ast_hang:-0} + ${ast_unproven:-0} ))
        MASTER_EXAMINED=$(( ${total:-0} + ${ast_total:-0} ))
        bad=$((f + crash + hang + unproven + ast_bad))
        echo -e "master:ALL\tPASS=$p FAIL=$f CRASH=$crash HANG=$hang UNPROVEN=$unproven AST_PASS=${ast_p:-0} AST_FAIL=${ast_f:-0}\t" >> "$RESULTS"
        PASS=$((PASS+p+${ast_p:-0})); FAIL=$((FAIL+bad))
        MASTER_PASS=$((p+${ast_p:-0})); MASTER_FAIL=$bad
    fi
else
    echo -e "master:ALL\tMISSING\t" >> "$RESULTS"
    FAIL=$((FAIL+1))
fi

echo "M4: PASS=$PASS FAIL=$FAIL NOREF=$NOREF XFAIL=$XFAIL (master: $MASTER_EXAMINED entries, $MASTER_PASS pass / $MASTER_FAIL fail; stdin-loose: $EXAMINED examined)"
# ⛔ MASTER_EXAMINED refuses ON ITS OWN (seat11's finding, see test_gate_pascal_m3.sh) -- the stdin-loose loop
# always examines >0, so a master-path defect used to hide completely behind it.
if [ $MASTER_EXAMINED -eq 0 ]; then
    echo "⛔ UNPROVEN: 0 master entries examined under $MASTER_SRC / $MASTER_REF -- path defect or unpopulated master, not a clean pass (this arm cannot hide behind the other)" >&2
    exit 2
fi
if [ $EXAMINED -eq 0 ]; then
    echo "⛔ UNPROVEN: 0 stdin-loose files examined under $CORPUS -- corpus-path typo or unpopulated clone, not a clean pass" >&2
    exit 2
fi
[ $FAIL -eq 0 ]
