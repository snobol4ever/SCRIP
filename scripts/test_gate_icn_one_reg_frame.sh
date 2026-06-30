#!/usr/bin/env bash
# test_gate_icn_one_reg_frame.sh — ICON STACKLESS ONE-REGISTER FRAME (FACT RULE) ratchet.
# Counts absolute-address slot emissions &pBB->(value|counter|state) / &a0->(...) in the Icon
# templates. The rule mandates register-relative [reg+off] addressing into ONE per-sequence frame,
# so absolute (uintptr_t)&pBB->slot immediates are FORBIDDEN (mode-3-only, not relocatable, not
# one-register-indexed). Ratchet: fails if count EXCEEDS $MAX (default baseline); lower the BASELINE
# below and $MAX as each box family migrates to [reg+off]. Target 0.
# AUTHORS: Lon Jones Cherryholmes · Jeffrey Cooper M.D. · Claude Sonnet  DATE: 2026-05-30
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SRC="${HERE}/../src"
BASELINE=21
MAX="${MAX:-$BASELINE}"

COUNT=$(grep -rnoE '\(uintptr_t\)[[:space:]]*&(pBB|a0)->(value|counter|state)' \
        "${SRC}/templates/" "${SRC}/emitter/emit_bb.c" \
        | grep -v _pl_ | wc -l | tr -d ' ')

echo "=== ICON STACKLESS ONE-REGISTER FRAME gate ==="
echo "absolute &pBB/a0->slot emissions on the Icon path: ${COUNT}  (baseline ${BASELINE}, MAX ${MAX})"
echo "--- per box-family breakdown ---"
grep -rnoE '\(uintptr_t\)[[:space:]]*&(pBB|a0)->(value|counter|state)' \
    "${SRC}/templates/" "${SRC}/emitter/emit_bb.c" \
    | grep -v _pl_ | sed -E 's,.*/([^/:]+):[0-9]+:.*,\1,' | sort | uniq -c | sort -rn

if [ "${COUNT}" -gt "${MAX}" ]; then
    echo "FAIL: count ${COUNT} exceeds MAX ${MAX} — an absolute slot address was introduced; use [reg+off]."
    exit 1
fi
echo "OK: count ${COUNT} <= MAX ${MAX}  (drive to 0 as families migrate to the one-register frame)"
exit 0
