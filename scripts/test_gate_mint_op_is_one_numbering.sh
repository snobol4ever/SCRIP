#!/usr/bin/env bash
# test_gate_mint_op_is_one_numbering.sh -- THE DESCRIPTOR'S PROVENANCE BYTE IS ONE NUMBERING, IT IS CALLED
# mint_op, AND THE ONLY THING THAT CAN OVERFLOW IT IS ASSERTED AT COMPILE TIME
# (row mint-op-is-one-numbering-from-1-the-123-rt-ids-go-and-the-field-is-renamed, hq_R 2026-09-13,
# CEO-719 as twice corrected on Lon's word; the cto is HQ).
#
# WHAT WENT WRONG, BECAUSE EVERY ARM HERE IS ONE CLAUSE OF IT. `mod_op` is a uint8_t recording WHICH BB MINTED
# a descriptor. Its name reads as MODIFIED, so a numbering that answers WHO MINTED looked like it needed a
# second population: 123 `MOD_OP_RT_*` ids, one per hand-written rtx/*.s minting routine, based at 130 with a
# comment saying that was "one past IR_OP_COUNT (129)". ⛔ THREE THINGS WERE WRONG AT ONCE. (1) IR_OP_COUNT was
# 136, not 129 -- measured by COMPILING it, not by reading the comment -- so seven IR ops stamped straight onto
# RT_FAILDESCR..RT_PROC_VALUE and a stamp reader could not tell a compiler mint from a runtime one, which is
# the exact property the 130 base existed to provide. (2) 1 + 136 + 123 = 260 IN A 256-WIDE FIELD: the space
# was not running out, IT WAS ALREADY OVER. (3) The rt ids were never a numbering of this at all -- they were a
# SECOND, ORTHOGONAL DIMENSION (which rt function a BB called, "Like BB_CALL, which call?"), and 118 of the 123
# have no IR-op counterpart by name because they name runtime HELPERS a BB calls, not BBs.
#
# ⭐ SO THE CURE IS A DELETION, A RENAME AND ONE ASSERT, AND THIS GATE HOLDS ALL THREE. The dimension is gone
# and its loss is DELIBERATE, TEMPORARY AND NAMED IN descr_tags.inc (Lon: "We can do without that granularity
# for now until we figure something else") -- ⛔ and the one thing a future reader must not do is put it back
# HERE: N values for "which rt" in this byte re-creates the overflow. ARM 1 is what stops it coming back one
# `#define` at a time.
#
# ⛔ ARM 3b IS THE ARM THAT MAKES ARM 3a MEAN ANYTHING: a static assert is invisible when it holds, so this
# gate also compiles a VIOLATING one through the same macro and REQUIRES the compiler to reject it. Without
# that, a DESCR_SASSERT accidentally defined empty would pass arm 3a forever while guarding nothing -- the
# green-when-measuring-nothing shape this tree refuses everywhere else.
# rc 0 green · 1 red · 2 could not measure.
set -uo pipefail
GATE=test_gate_mint_op_is_one_numbering
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"; cd "$ROOT"
PASS=0; FAIL=0; N=0
D=$(mktemp -d) || { echo "⛔ REFUSE(2) [$GATE]: no tmpdir"; exit 2; }
trap 'rm -rf "$D"' EXIT
for f in src/ir/descr.h src/ir/descr_tags.inc src/ir/IR.h src/templates/bb/bb_lit_scalar.cpp; do
    [ -f "$f" ] || { echo "⛔ REFUSE(2) [$GATE]: $f is missing -- this gate cannot measure a tree it cannot read"; exit 2; }
done

# ---- ARM 1 — ONE NUMBERING: no second population of ids, under either name, anywhere under src/.
N=$((N+1))
second="$(grep -rnE '#[[:space:]]*define[[:space:]]+(MOD_OP|MINT_OP)_RT_' src/ 2>/dev/null | head -5 || true)"
if [ -z "$second" ]; then PASS=$((PASS+1))
else echo "  RED  ARM 1: a SECOND population of provenance ids is back in src/ -- mint_op is ONE numbering (0 unstamped, 1..IR_OP_COUNT-1 the minting op) and a per-routine id belongs in its own storage, never in this byte, because N of them here re-creates the 260-in-256 overflow this row cured:"; printf '%s\n' "$second"; FAIL=$((FAIL+1)); fi

# ---- ARM 2 — THE NAME: `mod_op` is gone. It reads as MODIFIED and the field records WHO MINTED; that
# misnaming is what made a second population look necessary, twice, to two different readers.
N=$((N+1))
oldname="$(grep -rn '\bmod_op\b' src/ 2>/dev/null | head -5 || true)"
if [ -z "$oldname" ]; then PASS=$((PASS+1))
else echo "  RED  ARM 2: the field is named mod_op again somewhere under src/ -- it is mint_op:"; printf '%s\n' "$oldname"; FAIL=$((FAIL+1)); fi

# ---- ARM 3a — THE ASSERT IS PRESENT, and it is tied to IR_OP_COUNT rather than to a typed literal.
N=$((N+1))
if grep -qE 'DESCR_SASSERT\(IR_OP_COUNT <= 255' src/ir/IR.h; then PASS=$((PASS+1))
else echo "  RED  ARM 3a: IR.h carries no DESCR_SASSERT(IR_OP_COUNT <= 255, ...). The literal it replaced was true when it was typed and stopped being true seven times, because nothing read IR.h and descr_tags.inc together. The assert IS the guard."; FAIL=$((FAIL+1)); fi

# ---- ARM 3b — AND IT BITES. The same macro with a violating value must REFUSE to compile.
N=$((N+1))
cat > "$D/bite.c" <<'CEOF'
#include "descr.h"
enum { FAKE_OP_COUNT = 300 };
DESCR_SASSERT(FAKE_OP_COUNT <= 255, "a violating assert must not compile");
int main(void) { return 0; }
CEOF
if gcc -std=c11 -Isrc/ir -Isrc -c "$D/bite.c" -o "$D/bite.o" >"$D/bite.log" 2>&1; then
    echo "  RED  ARM 3b: a DELIBERATELY VIOLATING DESCR_SASSERT COMPILED CLEAN, so the macro is guarding nothing and ARM 3a proves nothing -- that is a green over an empty measurement"; FAIL=$((FAIL+1))
else PASS=$((PASS+1)); fi

# ---- ARM 4 — mint_op == opcode, WITH NO ARITHMETIC TO GET WRONG: the enum starts at 1 and the stamp adds no 1.
N=$((N+1))
if grep -qE '^\s*IR_ACTIVATE = 1,' src/ir/IR.h; then PASS=$((PASS+1))
else echo "  RED  ARM 4: the IR enum's first enumerator is not '= 1'. mint_op 0 MEANS UNSTAMPED, so an op numbered 0 would be indistinguishable from 'nothing touched this'."; FAIL=$((FAIL+1)); fi
N=$((N+1))
if grep -qE 'node->op \+ 1' src/templates/bb/bb_lit_scalar.cpp; then
    echo "  RED  ARM 4b: bb_lit_scalar.cpp still stamps node->op + 1. The +1 existed only to clear a zero that now means UNSTAMPED by construction, and an offset between the op and its stamp is one more thing that can drift."; FAIL=$((FAIL+1))
else PASS=$((PASS+1)); fi

# ---- ARM 5 — THE HAND-WRITTEN ASM MINTS ARE UNSTAMPED, ASSERTED IN THE SOURCE OF THE MINT ITSELF. 49 sites
# folded a private id into their existing immediate; they now fold nothing, which keeps the one-instruction
# mint the design was bought with.
N=$((N+1))
stamps="$(grep -rnE '<<[[:space:]]*8\)' src/runtime/rtx/*.s src/runtime/rtx/*.inc 2>/dev/null | grep -E 'MOD_OP|MINT_OP' | head -5 || true)"
if [ -z "$stamps" ]; then PASS=$((PASS+1))
else echo "  RED  ARM 5: an rtx asm mint folds a provenance id into its tag immediate again:"; printf '%s\n' "$stamps"; FAIL=$((FAIL+1)); fi

# ---- ARM 6 — AND IT IS TRUE OF THE BINARY, NOT ONLY OF THE SOURCE: the RTX differential battery compares the
# asm family against its C body bit for bit, INCLUDING this byte, so an asm mint that still stamped something
# would mismatch its golden. This is the only arm here that runs code; it needs out/libscrip_rt.so.
N=$((N+1))
if [ -f "${RT_DIR:-$ROOT/out}/libscrip_rt.so" ]; then
    out="$(timeout 180 bash "$HERE/test_rtx_unit.sh" 2>&1 || true)"
    case "$out" in
        *"RTX UNIT: ALL PASS"*) PASS=$((PASS+1)) ;;
        *) echo "  RED  ARM 6: the RTX differential battery does not pass, so this byte is not proven in the binary:"; printf '%s\n' "$out" | tail -6; FAIL=$((FAIL+1)) ;;
    esac
else
    echo "⛔ REFUSE(2) [$GATE]: no out/libscrip_rt.so -- ARM 6 is the only arm that reads the BINARY and there is no substitute for it here; run make first"; exit 2
fi
[ "$N" -gt 0 ] || { echo "⛔ REFUSE(2) [$GATE]: graded nothing"; exit 2; }
echo "[$GATE] arms=$N pass=$PASS fail=$FAIL"
[ "$FAIL" -eq 0 ] && { echo "GATE PASS [$GATE]: mint_op is one numbering (0 unstamped, 1..IR_OP_COUNT-1 the minting BB/IR op), the old name is gone, the width is asserted against IR_OP_COUNT by an assert proven to bite, the stamp needs no arithmetic, and the hand-written asm mints leave it unstamped in source AND in the binary"; exit 0; }
echo "verdict=RED"; exit 1
