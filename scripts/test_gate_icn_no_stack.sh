#!/usr/bin/env bash
# test_gate_icn_no_stack.sh — GROUND ZERO 3 no-value-stack gate for the Icon BB path.
# Counts rt_push_*/rt_pop_* value-stack emissions in the Icon emission path (BB_templates + emit_bb.c),
# excluding the Prolog (_pl_) family. Target is 0 for every Icon box family as it is rebuilt stackless.
# Ratchet: fails if the count EXCEEDS $MAX (default = recorded baseline) so no new value-stack traffic
# is introduced; lower $MAX (and the BASELINE below) as each GZ rung rebuilds a family to 0.
# AUTHORS: Lon Jones Cherryholmes · Jeffrey Cooper M.D. · Claude Sonnet  DATE: 2026-05-30
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SRC="${HERE}/../src"
BASELINE=127
MAX="${MAX:-$BASELINE}"

COUNT=$(grep -rnoE 'rt_(push|pop)_[a-z_]+' "${SRC}/emitter/BB_templates/" "${SRC}/emitter/emit_bb.c" \
        | grep -v _pl_ | wc -l | tr -d ' ')

echo "=== GROUND ZERO 3 — Icon no-value-stack gate ==="
echo "rt_push_*/rt_pop_* value-stack emissions on the Icon path: ${COUNT}  (baseline ${BASELINE}, MAX ${MAX})"
echo "--- per box-family breakdown ---"
grep -rnoE 'rt_(push|pop)_[a-z_]+' "${SRC}/emitter/BB_templates/" "${SRC}/emitter/emit_bb.c" \
    | grep -v _pl_ | sed -E 's,.*/([^/:]+):[0-9]+:,\1  ,' | sort | uniq -c | sort -rn

if [ "${COUNT}" -gt "${MAX}" ]; then
    echo "FAIL: count ${COUNT} exceeds MAX ${MAX} — a value-stack emission was reintroduced."
    exit 1
fi
echo "OK: count ${COUNT} <= MAX ${MAX}  (drive to 0 as families are rebuilt)"
exit 0
