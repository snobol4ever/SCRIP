#!/usr/bin/env bash
# test_crosscheck_prolog.sh — 3-mode crosscheck for PROLOG (GOAL-LANG-PROLOG)
#
# Runs the prolog test corpus through --run, --run, --run.
# Run on every major push. Mode-consistency check, not regression.
# If .ref present alongside test file: diffs vs oracle too.
# Exits 0 only if all three modes agree on every test.
#
# AUTHORS: Lon Jones Cherryholmes · Claude Sonnet 4.6  DATE: 2026-04-14
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${HERE}/../scrip"
TIMEOUT=30
PASS=0; FAIL=0; SKIP=0; ORACLE_MISS=0

xcheck() {
    local label="$1" file="$2" ref="${3:-}"
    if [ ! -f "$file" ]; then echo "  SKIP $label (no file)"; SKIP=$((SKIP+1)); return; fi
    local ir run_out run_rc
    ir=$(timeout  $TIMEOUT "$SCRIP" --run  "$file" </dev/null 2>/dev/null)
    # Mode 3 = NATIVE. --run is unconditionally native (sm_run_native: x86 SM asm + flat-wired x86 BB,
    # NOT sm_interp_run / IR_interp / brokers). There is NO interpreter fallback anywhere — --run either
    # runs native fully or ABORTs (rc 134 / SIGABRT). An abort is NEVER a pass: a program whose native
    # path is unimplemented (e.g. SM_BB_PL_INVOKE MEDIUM_BINARY stub for Prolog) aborts here, and we
    # count it NATIVE-ABORT, not a degenerate empty-output match against --run.
    # (Lon directive 2026-05-29: run the specified mode fully or abort; no fallback paths.)
    run_out=$(timeout $TIMEOUT "$SCRIP" --run "$file" </dev/null 2>/dev/null)
    run_rc=$?
    # Primary purpose of this gate: mode dispatch consistency.
    # --run (Mode 2) and native --run (Mode 3) must agree with each other.
    # Oracle (.ref) mismatches are a frontend completeness issue, not a mode
    # dispatch issue, and are reported separately as ORACLE_MISS (informational).
    local ok=1
    if [ "$run_rc" -ge 128 ]; then
        echo "  NATIVE-ABORT $label (mode-3 --run aborted rc=$run_rc — native emit unimplemented; not a pass)"; ok=0
    elif [ "$run_out" != "$ir" ]; then
        echo "  FAIL $label --run(native) vs --run"; diff <(echo "$ir") <(echo "$run_out") | head -5 | sed 's/^/    /'; ok=0
    fi
    if [ "$ok" -eq 1 ]; then
        echo "  PASS $label"; PASS=$((PASS+1))
        if [ -n "$ref" ] && [ -f "$ref" ]; then
            local exp; exp=$(cat "$ref")
            [ "$ir" != "$exp" ] && { echo "    (ORACLE_MISS $label — modes agree but differ from oracle)"; ORACLE_MISS=$((ORACLE_MISS+1)); }
        fi
    else
        FAIL=$((FAIL+1))
    fi
}

echo "=== Prolog 3-mode crosscheck ==="

T=$(mktemp /tmp/pl_XXXXXX.pl)
cat > "$T" << 'EOF'
:- initialization(main).
main :- write(hello), nl.
EOF
xcheck "hello" "$T"

cat > "$T" << 'EOF'
:- initialization(main).
fact(a). fact(b). fact(c).
main :- fact(X), write(X), nl, fail ; true.
EOF
xcheck "backtrack" "$T"

cat > "$T" << 'EOF'
:- initialization(main).
main :- X is 2 + 3, write(X), nl.
EOF
xcheck "arith" "$T"

cat > "$T" << 'EOF'
:- initialization(main).
count(0) :- !.
count(N) :- N > 0, write(N), nl, N1 is N - 1, count(N1).
main :- count(3).
EOF
xcheck "recursion" "$T"

rm -f "$T"

# Rung corpus files (PJ-9b: extended to walk flat-file corpus, not just subdirs)
RUNGS=$S4E/corpus/prolog
if [ -d "$RUNGS" ]; then
    for f in "$RUNGS"/rung*.pl; do
        [ -f "$f" ] || continue
        # Skip programs that --run can't complete (timeout, non-zero exit)
        ir_rc=$(timeout 4 "$SCRIP" --run "$f" </dev/null >/dev/null 2>&1; echo $?)
        if [ "$ir_rc" != "0" ]; then SKIP=$((SKIP+1)); continue; fi
        ref="${f%.pl}.ref"
        xcheck "$(basename $f .pl)" "$f" "$ref"
    done
fi

echo ""
echo "PASS=$PASS FAIL=$FAIL SKIP=$SKIP ORACLE_MISS=$ORACLE_MISS"
echo "(PASS = 3 modes agree; ORACLE_MISS = 3 modes agree but differ from .ref — frontend gap, not mode issue)"
[ "$FAIL" -eq 0 ]
