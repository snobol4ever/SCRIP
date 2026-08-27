#!/bin/bash
# test_gate_no_weak_abort_stub.sh -- pins the two halves of the sm_eval_subexpr landmine so a refactor cannot re-arm it.
#
# ⭐ WHAT THE LANDMINE ACTUALLY WAS (measured 2026-08-27, row sm-eval-subexpr-weak-abort-landmine):
# `sm_eval_subexpr` was declared __attribute__((weak)) with NO strong definition anywhere in the tree, and its body
# called abort(). A weak symbol that aborts is the worst available failure mode: the call TYPE-CHECKS, it LINKS, and
# it kills the process at run time. Two live call arms reached it, and both were unreachable only BY ACCIDENT:
#   (1) runtime_eval.c's `slen == 1` arm -- reachable only from proc_as_value(), which sets pv.slen to the PROC-TABLE
#       INDEX while the consumer reads slen as a TYPE TAG. It never fired because proc_as_value() has zero callers.
#       Had anything called it for the proc at index 1, EXPVAL on that value would have aborted the process.
#   (2) invocation.c's proc_table_call -> sm_call_proc, guarded by `entry_pc >= 0`. It never fired because all live
#       stage2_proc_grow() sites assign entry_pc = -1 BY HAND -- over a memset-to-0 default in which 0 is >= 0.
# ⛔ NEITHER GUARD WAS STRUCTURAL. Both were one edit away from arming: a caller for proc_as_value, or one new
# allocation site that forgot the -1. The path is deleted now; this gate keeps the shape from coming back.
#
# REFUSES rc=2 when it cannot measure -- never skip-as-success.
set -u
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"
SRC="${WEAKGATE_SRC:-$S4E/SCRIP/src}"
[ -d "$SRC" ] || SRC="$S4E/src"
[ -d "$SRC" ] || { echo "⛔ REFUSES rc=2: src tree not found (tried \$S4E/SCRIP/src and \$S4E/src)"; exit 2; }
command -v grep >/dev/null || { echo "⛔ REFUSES rc=2: no grep"; exit 2; }

FAIL=0
echo "weak-abort-stub gate -- recomputed live, $(date -u +%Y-%m-%dT%H:%M:%SZ)"
echo "src: $SRC"

# ── HALF 1: no weak definition may reappear. ──────────────────────────────────────────────────────────────────────
WEAK=$(grep -rn '__attribute__((weak))' "$SRC" --include=*.c --include=*.h 2>/dev/null || true)
WN=$(printf '%s' "$WEAK" | grep -c . || true)
if [ "$WN" -ne 0 ]; then
    echo "⛔ WEAK DEFINITION(S) PRESENT -- each is a call that type-checks, links, and may kill the process:"
    printf '%s\n' "$WEAK" | sed 's|^|     |'
    echo "     -> give it a strong definition, or delete the path. If one must stay, name it here deliberately."
    FAIL=1
else
    echo "  ✅ half 1: zero __attribute__((weak)) definitions in src/"
fi

# ── HALF 2: the entry_pc invariant. Every proc allocation must set entry_pc = -1. ─────────────────────────────────
# The default from stage2_proc_grow() is memset-0, and 0 satisfies every `entry_pc >= 0` test in the tree. So a site
# that allocates without assigning -1 does not merely leave a field unset -- it opts that proc INTO the >= 0 branches.
SITES=0; BAD=0; BADLIST=""
while IFS=: read -r f l _; do
    [ -n "${f:-}" ] || continue
    case "$f" in *"gz5-parked"*) continue;; esac      # not in the Makefile's explicit source list; not built
    SITES=$((SITES+1))
    # the assignment sits within a few lines of the allocation at every live site
    if ! sed -n "${l},$((l+6))p" "$f" 2>/dev/null | grep -q 'entry_pc *= *-1'; then
        BAD=$((BAD+1)); BADLIST="$BADLIST\n     $f:$l"
    fi
done <<EOF
$(grep -rnE '=[[:space:]]*stage2_proc_grow\(' "$SRC" --include=*.c 2>/dev/null)
EOF

# ⭐ ANTI-VACUITY BY CROSS-CHECK, NOT BY A >0 FLOOR. A floor of zero is not a real check: this gate's first draft
# matched exactly ONE site out of seventeen (a stray \? in a BRE) and reported a confident green, because 1 > 0.
# An instrument that half-matches reports success on the fraction it can see. So count independently and disagree loudly.
XPECT=$(grep -rlE '=[[:space:]]*stage2_proc_grow\(' "$SRC" --include=*.c 2>/dev/null | grep -v 'gz5-parked' \
        | xargs grep -cHE '=[[:space:]]*stage2_proc_grow\(' 2>/dev/null | awk -F: '{n+=$NF}END{print n+0}')
REFUSE=0
if [ "$SITES" -eq 0 ] || [ "$SITES" -ne "$XPECT" ]; then
    echo "⛔ CANNOT MEASURE half 2: enumerated $SITES call site(s) but an independent count says $XPECT."
    echo "   The pattern and the tree disagree, so a half-match would report green on the fraction it can see."
    echo "   Fix the pattern; do not lower the expectation."
    REFUSE=1
fi
if [ "$REFUSE" -eq 0 ]; then
if [ "$BAD" -ne 0 ]; then
    echo "⛔ $BAD of $SITES stage2_proc_grow site(s) do NOT set entry_pc = -1 within 6 lines:"
    printf "$BADLIST\n"
    echo "     -> the memset-0 default is 0, and 0 is >= 0. An unset entry_pc opts that proc into every >= 0 branch."
    FAIL=1
else
    echo "  ✅ half 2: all $SITES live stage2_proc_grow site(s) set entry_pc = -1"
fi
fi

# A definite defect outranks an unmeasurable half -- but never silently: the refusal is printed either way.
[ "$FAIL" -ne 0 ] && { echo "⛔ GATE FAILED"; exit 1; }
[ "$REFUSE" -ne 0 ] && { echo "⛔ REFUSES rc=2 -- half 2 could not measure; half 1 alone is not a green gate."; exit 2; }
echo "✅ GATE OK"; exit 0
