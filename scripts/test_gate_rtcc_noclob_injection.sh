#!/usr/bin/env bash
# test_gate_rtcc_noclob_injection.sh -- INJECTION TEST for the rtcc-veneer-strip-pure-asm mechanism
# (GOAL-RTCC.md, Lon s200 in-chat).  x86_rtcc_clob()'s LEAF[] table (src/templates/x86_asm.h) is the
# ONE authority licensing a callee to skip the RTCC veneer; scripts/test_gate_rtcc_callee_class.sh is
# what makes that authority trustworthy -- it re-derives the claim from the actual assembly and fails
# the moment a claim stops matching the source.  This script proves the verifier itself has teeth: it
# builds a throwaway scratch tree, shaped exactly like the real one, and shows
#   (1) POSITIVE -- a symbol truthfully classified (mask covers every r8/r9/r10/r11 write) PASSES.
#   (2) NEGATIVE -- a deliberately clobbering stub, FALSELY claimed as non-clobbering (mask 0), is
#       CAUGHT and FAILS.  This is the "negative-tested by injection" leg of the rung's DoD: an
#       undeclared or falsely-declared callee must keep -- or in this case regain -- its veneer, and
#       the mechanism that decides that must be shown catching a lie, not just accepting the truth.
# Nothing here touches the real tree.  Usage: bash scripts/test_gate_rtcc_noclob_injection.sh (exit 0 = both controls fired correctly)
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TMP="$(mktemp -d)"
trap 'rm -rf "$TMP"' EXIT
mkdir -p "$TMP/src/templates" "$TMP/src/runtime/rtx"
cat > "$TMP/src/runtime/rtx/rtx_inject_stub.S" <<'ASM'
RTX_FUNC(rt_test_true_leaf)
    mov     eax, edi
    add     eax, esi
    ret
RTX_ENDF(rt_test_true_leaf)
RTX_FUNC(rt_test_false_leaf)
    mov     eax, edi
    mov     r9d, esi
    add     eax, r9d
    ret
RTX_ENDF(rt_test_false_leaf)
ASM
cat > "$TMP/src/templates/x86_asm.h" <<'HDR'
static inline unsigned x86_rtcc_clob(const char * sym) {
    static const struct { const char * n; unsigned m; } LEAF[] = {
        { "rt_test_true_leaf", 0 },
        { "rt_test_false_leaf", 0 },
    };
    for (size_t i = 0; i < sizeof(LEAF) / sizeof(LEAF[0]); i++) if (strcmp(sym, LEAF[i].n) == 0) return LEAF[i].m;
    return 15u;
}
HDR
out="$(ROOT="$TMP" bash "$HERE/test_gate_rtcc_callee_class.sh" 2>&1)"; rc=$?
echo "=== RTCC NOCLOB INJECTION TEST ==="
pos_ok=0; neg_ok=0
if [ "$rc" -ne 0 ] && printf '%s\n' "$out" | grep -q 'rt_test_false_leaf.*writes r9 but mask says <none>'; then neg_ok=1; fi
if ! printf '%s\n' "$out" | grep -q 'rt_test_true_leaf'; then pos_ok=1; fi
if [ "$pos_ok" -eq 1 ]; then echo "  PASS  positive control: truthfully-classified rt_test_true_leaf raised no FAIL"
else echo "  FAIL  positive control: rt_test_true_leaf should not have been flagged"; printf '%s\n' "$out"; fi
if [ "$neg_ok" -eq 1 ]; then echo "  PASS  negative control: falsely-classified rt_test_false_leaf WAS CAUGHT (gate exit=$rc)"
else echo "  FAIL  negative control: the injected false claim was NOT caught -- gate exit=$rc"; printf '%s\n' "$out"; fi
[ "$pos_ok" -eq 1 ] && [ "$neg_ok" -eq 1 ]
