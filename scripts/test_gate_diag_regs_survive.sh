#!/usr/bin/env bash
# test_gate_diag_regs_survive.sh — DIAG-REGS SURVIVAL GATE (row diag-regs-telemetry-can-lie).
#
# r10/r11 carry write-only crash telemetry (r10 = SNOBOL4 statement number, r11 = BB node id,
# c951f257) meant to answer "what was running" on ABORT/CRASH/ERROR.  The stamp alone proves
# nothing: unless it SURVIVES to the fault, a reader gets a confidently wrong answer, which is
# worse than no answer at all (ARCH-SNOBOL4-RTX.md #2's honesty clause). 53819b4a widened the
# RTCC veneer to write-back/reload r10/r11 around every protected runtime call, closing the
# specific clobber that clause measured (rt_match_enter) -- but nothing PROVED it stayed closed.
# THIS GATE IS THAT PROOF, and it is NEGATIVE-TESTED: SCRIP_RTCC_VENEER=3 rebuilds the SAME
# witness with r8/r9 protection intact (so the run does not crash for an unrelated reason -- r9
# is the live GVA base register by default, ARCH-SNOBOL4-RTX.md #2) and r10/r11 protection
# SPECIFICALLY withheld, forces the identical fault, and asserts the reading comes back WRONG --
# reproducing the exact defect the honesty clause originally measured. If that arm ever reads
# correct too, this gate is measuring nothing and must fail loudly, not pass quietly.
#
# Method: compile tests/snobol4/probe/diag_regs_witness.{sno,ref} (mode-4), link it, set a breakpoint at
# n*_match_break's alpha (reached only after rt_match_enter's protected call has returned), and
# FORCE a real SIGSEGV there via `set $rip = 0` -- a standard fault-injection idiom that corrupts
# only RIP, leaving every GPR exactly as the box left it, so r10 is read as a genuine crash would
# show it. ptrace stops the inferior before any handler runs (RULES.md ASM-DIFF-FIRST), so gdb's
# post-fault register read is the same value a real core dump would carry. The box name is
# matched by SUFFIX, not by its numeric node-id prefix, so an unrelated lowering change that
# shifts node ids does not silently blind this gate.
#
# KNOWN, NAMED LIMITATION (not this gate's to close, recorded so nobody re-discovers it as new):
# the veneer only guarantees the register AFTER a protected call returns. A fault that lands
# WHILE such a call is still executing (including inside PLT lazy-binding, on that symbol's first
# invocation in the process) reads the CALLEE's transient scratch, not the diagnostic -- the
# correct value is still recoverable from the RTCC block itself (rtccb+56 for r10, +64 for r11)
# for the single in-flight call, but nothing makes a live-register read safe at that instant.
# free-r10/free-r11 (separate, open rows) are the only way to close that window for good.
# See ARCH-SNOBOL4-RTX.md #2.
#
# Usage: bash scripts/test_gate_diag_regs_survive.sh    (exit 0 = green, 1 = measured broken, 2 = could not measure)
set -u
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"
ROOT="$S4E/SCRIP"
CORPUS="$S4E/corpus"
# corpus-crosscheck-probe-total-conversion (2026-08-29, seat12): the loose witness moved into
# tests/snobol4/probe/diag_regs_witness.{sno,ref} (suite format, single entry) -- extract() below
# materializes a standalone .sno into $WORK, same idiom test_arbno_witnesses.sh already uses.
WITNESS_SNO="$CORPUS/tests/snobol4/probe/diag_regs_witness.sno"
WITNESS_REF="$CORPUS/tests/snobol4/probe/diag_regs_witness.ref"

if [ ! -x "$ROOT/scrip" ]; then echo "⛔ FAIL: scrip is not built."; exit 1; fi
if [ ! -f "$WITNESS_SNO" ] || [ ! -f "$WITNESS_REF" ]; then echo "⛔ REFUSED: suite missing: $WITNESS_SNO"; exit 2; fi
if ! command -v gdb >/dev/null 2>&1; then echo "⛔ REFUSED: gdb not available -- cannot grade a live-register crash reading without it."; exit 2; fi

WORK="$(mktemp -d)"
trap 'rm -rf "$WORK"' EXIT

WITNESS="$WORK/diag_regs_witness.sno"
python3 "$ROOT/scripts/corpus_suite_harness.py" extract "$WITNESS_SNO" "$WITNESS_REF" diag_regs_witness "$WITNESS" >/dev/null 2>&1
if [ ! -f "$WITNESS" ]; then echo "⛔ REFUSED: could not extract diag_regs_witness from suite"; exit 2; fi

compile_and_link() {   # $1 = output basename; caller sets/unsets SCRIP_RTCC_VENEER first
    local name="$1"
    ( cd "$WORK" && "$ROOT/scrip" --compile -o "$name.s" "$WITNESS" < /dev/null ) || return 1
    gcc -no-pie -g -o "$WORK/$name" "$WORK/$name.s" -L"$ROOT/out" -lscrip_rt -Wl,-rpath,"$ROOT/out" \
        2>"$WORK/$name.gcc.log" || { cat "$WORK/$name.gcc.log" >&2; return 1; }
}

read_r10_at_crash() {   # $1 = binary path -> prints r10 decimal on stdout, empty on any mishap
    local bin="$1" addr
    addr="$(nm "$bin" 2>/dev/null | awk '$3 ~ /_match_break_α$/ {print $1; exit}')"
    [ -n "$addr" ] || return 1
    timeout 20 gdb -batch -nx \
        -ex "break *0x$addr" \
        -ex "run" \
        -ex 'set $rip = 0' \
        -ex "continue" \
        -ex 'printf "R10=%ld\n", $r10' \
        -ex "quit" \
        "$bin" 2>/dev/null | sed -n 's/^R10=//p'
}

echo "=== positive arm: default build (today's tree, veneer live) ==="
unset SCRIP_RTCC_VENEER
if ! compile_and_link good; then echo "⛔ FAIL: could not build the positive-arm witness"; exit 1; fi
got_good="$(read_r10_at_crash "$WORK/good")"
echo "  r10 at forced crash = ${got_good:-<none>}  (expect 2)"

echo "=== negative arm: r8/r9 protected, r10/r11 protection WITHHELD (SCRIP_RTCC_VENEER=3) ==="
export SCRIP_RTCC_VENEER=3
if ! compile_and_link bad; then echo "⛔ FAIL: could not build the negative-arm witness"; exit 1; fi
unset SCRIP_RTCC_VENEER
got_bad="$(read_r10_at_crash "$WORK/bad")"
echo "  r10 at forced crash = ${got_bad:-<none>}  (expect NOT 2 -- proves this gate can fail)"

pass=1
if [ "$got_good" != "2" ]; then
    echo "⛔ FAIL: positive arm read r10=${got_good:-<none>}, wanted 2 -- today's tree no longer survives the crash."
    pass=0
fi
if [ "$got_bad" = "2" ]; then
    echo "⛔ FAIL: negative arm ALSO read r10=2 -- this gate is not measuring anything (SCRIP_RTCC_VENEER=3 no longer isolates the defect)."
    pass=0
fi

if [ "$pass" = "1" ]; then
    echo "✅ PASS: r10 survives to a forced crash after a protected runtime call returns, and the negative arm proves the check is real."
    exit 0
fi
exit 1
