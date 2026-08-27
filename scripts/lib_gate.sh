#!/bin/bash
# lib_gate.sh -- V2-5 GATE HONESTY primitives.  Sourced, never executed.
#
# THE DEFECT THIS EXISTS TO KILL (seat16's audit, FINDING-2026-08-22-seat16-rung-gate-false-green-audit.md):
# 31 of 105 gates COULD NOT SAY NO.  Four distinct classes, one root cause -- a gate that examined NOTHING
# reported the same thing as a gate that examined everything and found it clean.  At 16 seats a gate that
# cannot fail closes rows on false green at scale, and ARCH-FLEET-CEO.md LAW 1 ("done is computed, never
# declared") is only as good as the gates a DONE-WHEN cites.
#
# THE THREE EXIT CODES.  This is the whole idea: "I checked and it is clean" and "I could not check" must
# never be the same exit code again.
#   0  CLEAN     -- work was actually examined, and it was good.
#   1  VIOLATION -- work was examined, and it was bad.
#   2  UNPROVEN  -- the gate could not examine the work (missing binary, empty corpus, absent path).
#                   ⛔ NOT a pass.  A fresh unbuilt seat gets 2, which is what SKIP-as-success was hiding.
#
# STRICTNESS IS NOW THE DEFAULT.  The old shape was "informational, exit 0, pass --strict to enforce" -- and
# `grep -rn -- --strict scripts/` proved NOTHING in the repo ever passed it.  A flag no caller passes is not
# an option, it is a disabled gate.  Callers wanting the old behaviour pass --informational explicitly, and
# it announces itself.
GATE_NAME="${GATE_NAME:-$(basename "${BASH_SOURCE[1]:-gate}" .sh)}"
GATE_STRICT=1
GATE_EXAMINED=0
gate_parse_args() {
    for a in "$@"; do
        case "$a" in
            --strict)        GATE_STRICT=1 ;;
            --informational) GATE_STRICT=0 ;;
        esac
    done
    [ "$GATE_STRICT" = "0" ] && echo "⛔ $GATE_NAME: --informational -- verdict is NOT enforced, exit code is not a gate result."
    return 0
}
# gate_require <path> <what-it-is> -- a prerequisite that must exist.  Absent => UNPROVEN(2), never SKIP-0.
gate_require() {
    if [ ! -e "$1" ]; then
        echo "GATE UNPROVEN(2) [$GATE_NAME]: required $2 is absent at $1"
        echo "    This is NOT a pass.  The gate could not examine anything, so it cannot certify anything."
        exit 2
    fi
}
# gate_require_exec <path> <what-it-is> -- same, but must also be executable (the unbuilt-scrip class).
gate_require_exec() {
    if [ ! -x "$1" ]; then
        echo "GATE UNPROVEN(2) [$GATE_NAME]: required $2 is not built/executable at $1"
        echo "    This is NOT a pass.  Build it (make) and re-run; a fresh seat must not read as green."
        exit 2
    fi
}
# gate_floor <examined-count> <minimum> <what-was-counted> -- the empty-glob / empty-dir / zero-files class.
gate_floor() {
    GATE_EXAMINED="$1"
    if [ "$1" -lt "$2" ] 2>/dev/null; then
        echo "GATE UNPROVEN(2) [$GATE_NAME]: examined $1 $3, floor is $2"
        echo "    Zero-work-examined is indistinguishable from all-clean, so it is refused rather than passed."
        exit 2
    fi
}
# gate_oracle_stdout_match <oracle-stdout-file> <actual-stdout-file> <actual-stderr-file> <actual-rc>
#
# ERROR-IDENTITY NORMALIZER (row conform-unload-noop, hq_C ruling 2026-08-27). x64/bin/sbl prints a full
# abnormal-termination trailer to STDOUT on every uncaught runtime error -- file/line/statement/stmts-
# executed/"memory used (bytes)"/"memory left (bytes)" -- and the memory figures are SPITBOL-internal
# accounting that differs per program (measured: 11880/1036688 on f15_unload vs a different pair on
# f12_load), so no reimplementation can ever reproduce them byte-for-byte. The oracle's own exit code is
# ALSO 0 on this whole class of error (SCRIP's core_runtime_error always exit(1)), and SCRIP reports its
# error on STDERR where the oracle reports on STDOUT. A byte-exact-stdout-plus-exact-rc instrument can
# therefore never pass for a witness the oracle halts on -- a DONE-WHEN shape that cannot mechanically
# succeed, the same defect family as a gate that can never say FAIL, wearing the opposite mask.
#
# RULING: a witness the oracle completed NORMALLY on (no trailer) is graded exactly as before -- byte-
# exact stdout, matched rc -- so this changes nothing for the hundreds of already-passing witnesses. A
# witness the oracle HALTED on is graded on ERROR IDENTITY (the numeric `ERROR nnn` code, wherever each
# side reports it) plus termination status (actual rc != 0), with the genuine pre-error program output
# compared after the oracle's trailer -- and the blank-line padding SPITBOL prints immediately before it
# -- is stripped; command substitution's trailing-newline trim absorbs that padding on both sides.
#
# Returns 0 (match) or 1 (mismatch); prints nothing, same silent contract as the `cmp -s` it replaces.
gate_oracle_stdout_match() {
    local ora="$1" act_out="$2" act_err="$3" act_rc="$4"
    local ora_code
    ora_code="$(sed -n 's/^.*: ERROR \([0-9][0-9]*\) -- .*$/\1/p' "$ora" | head -1)"
    if [ -z "$ora_code" ]; then
        [ "$act_rc" -eq 0 ] && cmp -s "$ora" "$act_out"
        return $?
    fi
    local ora_pre act_pre act_code
    ora_pre="$(sed -n '/: ERROR [0-9][0-9]* -- /q; p' "$ora")"
    act_pre="$(cat "$act_out")"
    [ "$ora_pre" = "$act_pre" ] || return 1
    [ "$act_rc" -ne 0 ] || return 1
    act_code="$(sed -n 's/^\*\* Error \([0-9][0-9]*\) in statement.*$/\1/p' "$act_err" | head -1)"
    [ -n "$act_code" ] || return 1
    [ "$((10#$ora_code))" -eq "$((10#$act_code))" ]
}
# gate_verdict <violation-count> <what-was-violated> -- the computed verdict.  Never declared.
gate_verdict() {
    if [ "${1:-0}" -ne 0 ] 2>/dev/null; then
        echo "GATE FAIL(1) [$GATE_NAME]: $1 $2 (examined ${GATE_EXAMINED:-?})"
        [ "$GATE_STRICT" = "1" ] && exit 1
        echo "    (--informational: exiting 0 despite the failure above)"
        exit 0
    fi
    echo "GATE PASS(0) [$GATE_NAME]: 0 $2 (examined ${GATE_EXAMINED:-?})"
    exit 0
}
