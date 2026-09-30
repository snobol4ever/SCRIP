#!/usr/bin/env bash
# test_gate_sno_arbno_literal_body_then_fence_terminates.sh -- ARBNO(P) WHOSE BODY BEGINS WITH A LITERAL, FOLLOWED BY A FENCE(Q)
# THAT IS BACKTRACKED INTO, TERMINATES AND ANSWERS AS SPITBOL DOES (ceo 2026-09-30, found rewriting bootstrap/parser_raku.sc:
# `'a' ARBNO(' b') FENCE(' ' | '') RPOS(0)` on 'a b' -- sbl -bf says yes, ./scrip never returns; the same with FENCE(' ') -- sbl no,
# ./scrip never returns; two fences -- sbl yes, ./scrip never returns. ARBNO(FENCE(...)) and ARBNO((' ' | '') 'x') agree on both
# engines, so the shape is the LITERAL-FIRST body. The witness and its oracle-cut ref: scripts/fixtures/pattern/
# arbno_literal_body_then_fence.{sno,ref}. rc 0 both modes print the ref within the limit; rc 1 a diff or a timeout (the hang);
# rc 2 no binary.
set -u
here=$(cd "$(dirname "$0")" && pwd); W=$(cd "$here/.." && pwd); G=sno_arbno_literal_body_then_fence_terminates
[ -x "$W/scrip" ] || { echo "GATE REFUSE(2) [$G]: no $W/scrip"; exit 2; }
F="$W/scripts/fixtures/pattern/arbno_literal_body_then_fence.sno"; R="${F%.sno}.ref"
[ -s "$F" ] && [ -s "$R" ] || { echo "GATE REFUSE(2) [$G]: fixture or ref missing"; exit 2; }
T=$(mktemp -d); trap 'rm -rf "$T"' EXIT
rc=0
timeout 20 "$W/scrip" "$F" < /dev/null > "$T/m3.out" 2>&1; r3=$?
if [ $r3 -eq 124 ]; then echo "  m3: HANG (20 s) -- the matcher does not terminate"; rc=1
elif ! cmp -s "$T/m3.out" "$R"; then echo "  m3: DIFF from the oracle's answer"; diff "$T/m3.out" "$R" | head -4; rc=1
else echo "  m3: agrees with sbl -bf"; fi
if "$W/scrip" --compile "$F" -o "$T/w.s" < /dev/null > /dev/null 2>&1 && gcc -m64 -no-pie -rdynamic "$T/w.s" -Wl,-rpath,"$W/out" -L"$W/out" -lscrip_rt -lm -lpthread -o "$T/w" 2>/dev/null; then
    timeout 20 "$T/w" < /dev/null > "$T/m4.out" 2>&1; r4=$?
    if [ $r4 -eq 124 ]; then echo "  m4: HANG (20 s)"; rc=1
    elif ! cmp -s "$T/m4.out" "$R"; then echo "  m4: DIFF from the oracle's answer"; rc=1
    else echo "  m4: agrees with sbl -bf"; fi
else echo "  m4: could not build"; rc=1; fi
[ $rc -eq 0 ] && { echo "GATE PASS(0) [$G]: ARBNO with a literal-first body then FENCE terminates and matches SPITBOL in both modes"; exit 0; }
echo "GATE FAIL(1) [$G]: the literal-first ARBNO body followed by a backtracked FENCE does not answer as SPITBOL does (see above)"; exit 1
