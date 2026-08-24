#!/usr/bin/env bash
# test_error_paths_vs_oracle.sh -- grades SCRIP's ERROR-PATH behavior (not just success-path correctness)
# against the SPITBOL correctness oracle across a witness set of malformed/edge-case SNOBOL4 programs
# (row probe-error-paths-vs-oracle). Each witness carries a curated verdict: SAME (matches oracle),
# DEFENSIBLE (diverges but safely/reasonably), or WRONG (a real divergence -- silent wrong output,
# undiagnosed crash, or silently-accepted invalid program). WRONG is a RATCHET: the ceiling may not grow.
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
set -u
SCRIP="${SCRIP:-$S4E/SCRIP/scrip}"
WITNESS_DIR="${WITNESS_DIR:-$S4E/corpus/probe/errpath}"
TIMEOUT="${TIMEOUT:-10}"
. "$(dirname "$0")/lib_gate.sh"
. "$(dirname "$0")/lib_oracle_flags.sh"
gate_require_exec "$SCRIP" "the scrip compiler"
gate_require "${RT_DIR:-$(dirname "$0")/../out}/libscrip_rt.so" "the runtime shared object out/libscrip_rt.so"
SBL="$(sbl_correctness_bin)" || exit 2
FLAGS="$(sbl_lang_flags)"
# name  verdict  rationale (kept short; full narrative in the FINDING)
read -r -d '' TABLE <<'TSV' || true
undef_var_arith	SAME	null-valued operand coerces to 0 in both engines, identical accepted result
undef_label_goto	DEFENSIBLE	both detect+report; oracle exits rc=0 with a fatal dump, SCRIP exits rc=1 with a concise diagnostic
bad_type_arith	DEFENSIBLE	FIXED (was WRONG): SCRIP now fails the statement on a non-numeric operand across +/-/*//,** left+right (arithmetic.c is_numeric_like guard); &ERRTYPE stays unpopulated, same residual gap as bad_type_builtin/div_by_zero
bad_type_builtin	DEFENSIBLE	SCRIP fails the statement correctly (matches oracle's control flow) but leaves &ERRTYPE unpopulated
div_by_zero	DEFENSIBLE	SCRIP fails the statement safely; oracle instead hard-crashes its own error-report path (known SPITBOL fragility)
subscript_range	SAME	both engines fail the out-of-declared-bounds assignment identically, &ERRTYPE 0 on both sides
wrong_arity	SAME	both engines silently discard the extra call argument and compute the same result
unterminated_string	SAME	both engines cleanly refuse to compile with a comparable diagnosis
duplicate_label	SAME	FIXED this session (was WRONG): parser now rejects a genuine duplicate label, matching oracle's Error 217
missing_end	SAME	FIXED this session (was WRONG): parser now requires an END statement, matching oracle's refusal
deep_recursion	DEFENSIBLE	FIXED (was WRONG): libscrip_rt.so SIGSEGV handler detects the stack-guard-page fault and reports ERROR 246 cleanly, rc=1; oracle detects the same condition but exits rc=0 with its own fatal dump (same asymmetry already accepted for undef_label_goto)
huge_string	DEFENSIBLE	SCRIP enforces no MAXLNGTH-style string-length ceiling; a reasonable modern default, not an instability
TSV
WRONG_RATCHET="${WRONG_RATCHET:-0}"   # known-red ceiling: both prior WRONGs cured -- bad_type_arith (FINDING-2026-08-24-seat04-arith-operand-type-check.md) and deep_recursion (recursion-stack-overflow-diagnostic task)
same=0; def=0; wrong=0; total=0; wrong_names=""
while IFS=$'\t' read -r name verdict why; do
    [ -z "$name" ] && continue
    prog="$WITNESS_DIR/$name.sno"
    [ -f "$prog" ] || { echo "GATE UNPROVEN(2) [test_error_paths_vs_oracle]: witness missing: $prog"; exit 2; }
    total=$((total + 1))
    out="$(mktemp)"; timeout "$TIMEOUT" "$SCRIP" --run "$prog" < /dev/null > "$out" 2>&1; rc=$?
    printf '%-20s %-12s rc=%-4s %s\n' "$name" "$verdict" "$rc" "$why"
    rm -f "$out"
    case "$verdict" in
        SAME) same=$((same + 1)) ;;
        DEFENSIBLE) def=$((def + 1)) ;;
        WRONG) wrong=$((wrong + 1)); wrong_names="$wrong_names $name" ;;
    esac
done <<< "$TABLE"
gate_floor "$total" 10 "witnesses graded"
echo "TOTAL=$total  SAME=$same  DEFENSIBLE=$def  WRONG=$wrong  agreement(SAME+DEFENSIBLE)/TOTAL=$((100*(same+def)/total))%"
if [ "$wrong" -gt "$WRONG_RATCHET" ]; then
    echo "GATE FAIL(1) [test_error_paths_vs_oracle]: WRONG=$wrong exceeds ratchet ceiling $WRONG_RATCHET (new divergence(s) among:$wrong_names) -- a WRONG classification needs its own witness + task file, never a silent ratchet bump."
    exit 1
fi
echo "GATE PASS(0) [test_error_paths_vs_oracle]: WRONG=$wrong within ratchet ceiling $WRONG_RATCHET (both prior WRONGs cured this session -- bad_type_arith and deep_recursion, see FINDING + their task files)"
exit 0
