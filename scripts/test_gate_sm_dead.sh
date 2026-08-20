#!/usr/bin/env bash
# test_gate_sm_dead.sh — PHASE 0 (SM EXCISION) death ratchet.
#
# Counts LIVE references to the Stack-Machine EXECUTION surface — the global
# machinery being demolished: the SM interpreter (sm_interp_run), the native SM
# emitter (sm_run_native), and the SM value stack (g_vstack). Comments and
# extern forward-decls are excluded; the count is the live call/def/access sites.
#
# This gate does NOT match per-box backtrack arenas (e.g. `_1[64]`, enter()/calloc
# frames, ARBNO_i). Those are box-local data on the SURVIVOR side of the line and
# are intentionally invisible here — a future session must not "fix" them.
#
# It also does NOT match the SM_sequence_t *container* type. That struct currently
# carries the Icon BB table (bb_table/bb_count/bb_cap) which the survivor needs;
# the container is slimmed (SM fields removed), not deleted, in SMX-5.
#
# Ratchet: baseline 13. Lower as each language crosses off the SM and the
# interpreter/native/value-stack surface loses callers. Reaches 0 at SM-FINAL-DELETE.
set -u
cd "$(dirname "$0")/.." || exit 2

MAX="${SM_DEAD_MAX:-1}"

count=$(grep -rnE 'sm_interp_run *\(|sm_run_native *\(|g_vstack *\[' \
            src/ --include='*.c' --include='*.h' 2>/dev/null \
        | grep -vE ':[0-9]+:[[:space:]]*(/\*|\*|//)' \
        | grep -vE 'extern (int|void|DESCR_t)' \
        | wc -l)

echo "SM-execution-surface live references: $count  (MAX $MAX)"
if [ "$count" -gt "$MAX" ]; then
    echo "FAIL: SM execution surface GREW ($count > $MAX). The refuse only removes; it never adds."
    grep -rnE 'sm_interp_run *\(|sm_run_native *\(|g_vstack *\[' \
        src/ --include='*.c' --include='*.h' 2>/dev/null \
        | grep -vE ':[0-9]+:[[:space:]]*(/\*|\*|//)' \
        | grep -vE 'extern (int|void|DESCR_t)'
    exit 1
fi
echo "OK: SM execution surface <= MAX (drive to 0 as languages cross off the SM)."
exit 0
