#!/usr/bin/env bash
# test_gate_icn_one_reg_frame.sh — ICON STACKLESS ONE-REGISTER FRAME (FACT RULE) — TWO LOCKS.
#
# THE RULE (RULES.md): "all per-box RW in ONE local frame [reg+off]"; read-only locals are IP-relative.
# The corollary the template rules state (GOAL-TEMPLATE-REVAMP ζ-FRAME SCRATCH): a box reaches its storage
# through the frame ACCESSORS — FR/FRQ/FRQB, ZRES/ZLOC/ZOPQ, XSAQ/XSAD, zone_ref — "never movabs a process
# addr, never rip-rel .data".  The accessors are what make BINARY == TEXT and what let a slot be RE-HOMED
# (PF-1a..1d move a seam-crossed cell into the activation frame by changing the accessor's answer, not the
# call sites).  A hand-built operand string is invisible to all of that.
#
# ⛔ LOCK 2 IS NEW, AND LOCK 1 WAS VACUOUS (s247, seat1, rung N-0).  The gate's only test counted
# `(uintptr_t)&(pBB|a0)->(value|counter|state)` — the mode-3 `bb_bin_t` box structs.  THOSE STRUCTS NO LONGER
# EXIST (bb_bin_t was abolished 2026-06-02); the count is 0 and CANNOT BE ANYTHING ELSE, so the gate has been
# certifying a property nothing could violate while `test_gate_icn_var.sh` re-ran it as a HARD lock.  A ratchet
# whose subject has been deleted reports success forever.  Lock 1 is KEPT as a cheap zero-assert (if the
# structs ever return, fail), and Lock 2 measures the LIVE form of the same property.
#
# LOCK 2 — HAND-BUILT MEMORY BASES.  Counts string literals `"[rsp…"` / `"[rbp…"` built inside a template
# instead of asking an accessor.  Measured 22 at s247 (bb_call 4, bb_call_fn 4, bb_make_list 4, xa_flat 7,
# bb_define 3).  ⭐ THIS IS NOT A STYLE COUNT.  The arg-marshalling trio (bb_call / bb_call_fn / bb_make_list)
# hand-builds `[rsp + op_zread[i] + N]` — a RAW FLAT SPINE COORDINATE — where ZOPQ(k,w) would answer with the
# operand's ACTIVATION-FRAME home whenever PF-1c has re-homed it (`op_zread_xf[k]`).  Those sites therefore
# cannot see a re-homed operand: the exact "flat ZLS coordinate vs re-homed cell" defect class the SN4
# campaign spent 2026-08 killing (s129/s130/s177/s184/s189).  The xa_flat sites are the ICN-FR-2 wire header
# and the CLASS-C chain prologue — the pre-wire-stack crossing that rung N-1 deletes.
# ⛔ RATCHET, LOWERED MANUALLY IN THE LANDING COMMIT, NEVER AUTO (parallel seats; RULES.md CONCURRENCY).
# Target 0: reached when N-1 retires the wire header and N-2/N-4 route marshalling through ZOPQ.
# Surface is src/templates/*.cpp with COMMENTS STRIPPED (the medium-invisible gate's idiom — spelling-
# independent).  x86_asm.h is EXEMPT BY CONSTRUCTION: it is the encoder, the sanctioned sole producer of
# these strings.  `strstr(t, "[rsp")` is a search NEEDLE, not an emission, and is excluded by name.
# AUTHORS: Lon Jones Cherryholmes · Jeffrey Cooper M.D. · Claude Sonnet · Claude Fable  DATE: 2026-05-30 (relaid 2026-08-21)
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SRC="${HERE}/../src"
HANDBUILT_RATCHET="${HANDBUILT_RATCHET:-22}"
rc=0

[ -d "${SRC}/templates" ] || { echo "GATE FAIL(2): census surface ${SRC}/templates does not exist."; exit 2; }

echo "=== ICON STACKLESS ONE-REGISTER FRAME gate ==="
LEGACY=$(grep -rnoE '\(uintptr_t\)[[:space:]]*&(pBB|a0)->(value|counter|state)' "${SRC}" 2>/dev/null | grep -v _pl_ | grep -c . || true)
echo "LOCK 1 (zero-assert) absolute &pBB/a0->slot immediates: ${LEGACY}  (must be 0; the structs are abolished)"
if [ "${LEGACY}" -gt 0 ]; then
    echo "FAIL: an absolute box-slot address returned — per-box RW must be register-relative through the frame."
    rc=1
fi

sites=0
for f in "${SRC}"/templates/*.cpp; do
    code=$(perl -0777 -pe 's{/\*.*?\*/}{}gs; s{//[^\n]*}{}g' "$f" | grep -v 'strstr(')
    n=$(printf '%s' "$code" | grep -oE '"\[r(sp|bp)[^"]*"' | grep -c . || true)
    [ "$n" -gt 0 ] && { sites=$((sites+n)); printf '    %-32s %d\n' "$(basename "$f")" "$n"; }
done
echo "LOCK 2 (ratchet) hand-built [rsp/[rbp operand strings in templates: ${sites}  (ceiling ${HANDBUILT_RATCHET}, target 0)"
if [ "${sites}" -gt "${HANDBUILT_RATCHET}" ]; then
    echo "FAIL: ${sites} > ceiling ${HANDBUILT_RATCHET} — a memory base was hand-built; ask an accessor (FR/FRQ/ZRES/ZLOC/ZOPQ/XSAQ) instead."
    rc=1
fi
[ "${sites}" -lt "${HANDBUILT_RATCHET}" ] && echo "RATCHET IMPROVED: ${sites} < ${HANDBUILT_RATCHET} — lower HANDBUILT_RATCHET in this script to lock the gain in."
[ "$rc" -eq 0 ] && echo "OK: both locks hold."
exit "$rc"
