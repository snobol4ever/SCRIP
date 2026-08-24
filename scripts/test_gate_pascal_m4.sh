#!/bin/bash
# test_gate_pascal_m4.sh — Pascal M4 (--compile -> gcc -> link -> run) gate
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
SCRIP="${SCRIP:-$S4E/SCRIP/scrip}"
CORPUS="${CORPUS:-$S4E/corpus/tests/pascal}"
RESULTS="${RESULTS:-/tmp/m4_results.tsv}"
RT="${RT:-$S4E/SCRIP/out/libscrip_rt.so}"
TMP=$(mktemp -d)
trap "rm -rf $TMP" EXIT
PASS=0; FAIL=0; NOREF=0; XFAIL=0
echo -e "probe\tclass\tnotes" > "$RESULTS"
for pas in "$CORPUS"/*.pas; do
    name=$(basename "$pas" .pas)
    ref="$CORPUS/$name.ref"
    [[ "$name" == "pcom" || "$name" == "pint" ]] && continue
    if [ ! -f "$ref" ]; then
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
echo "M4: PASS=$PASS FAIL=$FAIL NOREF=$NOREF XFAIL=$XFAIL"
[ $FAIL -eq 0 ]
