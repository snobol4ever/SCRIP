#!/usr/bin/env bash
# stale-binary preflight (row test-gate-scripts-that-grade-scrip-refuse-on-a-stale-binary-census-widened, hq_T 2026-09-05)
"$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/util_require_fresh.sh" --gate "$(basename "${BASH_SOURCE[0]}" .sh)" || exit $?
# test_gate_diag_regs_survive.sh — DIAG-REGS SURVIVAL GATE (row diag-regs-telemetry-can-lie).
#
# r10/r11 carry write-only crash telemetry (r10 = SNOBOL4 statement number, r11 = BB node id,
# c951f257) meant to answer "what was running" on ABORT/CRASH/ERROR.  The stamp alone proves
# nothing: unless it SURVIVES to the fault, a reader gets a confidently wrong answer, which is
# worse than no answer at all (ARCH-SNOBOL4-RTX.md #2's honesty clause). 53819b4a widened the
# RTCC veneer to write-back/reload r10/r11 around every protected runtime call, closing the
# specific clobber that clause measured (rt_match_enter) -- but nothing PROVED it stayed closed.
# THIS GATE IS THAT PROOF, and it is NEGATIVE-TESTED. ⛔ RE-POINTED 2026-09-23 (cto, CEO-1224, row
# spine-no-rtccb-veneer-on-any-call-into-the-asm-runtime-the-rtx-abi-preserves-r8-to-r11): the protected
# call is rt_match_enter, an ASM-RUNTIME entry, and those are now called BARE -- the callee keeps r8-r11
# itself (rtx_abi.inc, proven by test_gate_rtx_entries_keep_the_rtcc_four.sh) -- so the old negative
# arm, SCRIP_RTCC_VENEER=3 withholding the call-site veneer's r10/r11 slots, withholds nothing and read
# correct. The negative arm is now SCRIP_RTX_PLANT_CLOBBER=1, which makes the emitter write -1 into r10
# and r11 right after every bare asm-runtime call: exactly what a callee that broke the contract would
# leave. It rebuilds the SAME witness, forces the identical fault, and asserts the reading comes back
# WRONG. If that arm ever reads correct too, this gate is measuring nothing and must fail loudly.
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
# ⭐ RE-POINTED 2026-08-30 (seat12, repo-wide dead-suite-path consumer sweep): the single-entry
# per-family suite pair this comment used to name (2026-08-29, seat12) was itself absorbed into THE
# ONE FLAT MASTER and deleted. lib_master_extract.sh now materializes the entry back into a
# standalone .sno by its ALL.csv origin ("probe_diag_regs_witness__diag_regs_witness").

if [ ! -x "$ROOT/scrip" ]; then echo "⛔ FAIL: scrip is not built."; exit 1; fi
if ! command -v gdb >/dev/null 2>&1; then echo "⛔ REFUSED: gdb not available -- cannot grade a live-register crash reading without it."; exit 2; fi

WORK="$(mktemp -d)"
trap 'rm -rf "$WORK"' EXIT

MASTER_LANG="${MASTER_LANG:-snobol4}" . "$ROOT/scripts/lib_master_extract.sh"
WITNESS="$WORK/diag_regs_witness.sno"
master_extract_origin probe_diag_regs_witness__diag_regs_witness "$WITNESS" >/dev/null 2>&1
if [ ! -f "$WITNESS" ]; then echo "⛔ REFUSED: could not extract diag_regs_witness from the master"; exit 2; fi

compile_and_link() {   # $1 = output basename; caller sets/unsets SCRIP_RTX_PLANT_CLOBBER first
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

echo "=== positive arm: default build (today's tree, the callee keeps r10/r11) ==="
unset SCRIP_RTX_PLANT_CLOBBER SCRIP_RTCC_VENEER
if ! compile_and_link good; then echo "⛔ FAIL: could not build the positive-arm witness"; exit 1; fi
got_good="$(read_r10_at_crash "$WORK/good")"
echo "  r10 at forced crash = ${got_good:-<none>}  (expect 2)"

echo "=== negative arm: a callee that breaks the contract (SCRIP_RTX_PLANT_CLOBBER=1) ==="
export SCRIP_RTX_PLANT_CLOBBER=1
if ! compile_and_link bad; then echo "⛔ FAIL: could not build the negative-arm witness"; exit 1; fi
unset SCRIP_RTX_PLANT_CLOBBER
got_bad="$(read_r10_at_crash "$WORK/bad")"
echo "  r10 at forced crash = ${got_bad:-<none>}  (expect NOT 2 -- proves this gate can fail)"

pass=1
if [ "$got_good" != "2" ]; then
    echo "⛔ FAIL: positive arm read r10=${got_good:-<none>}, wanted 2 -- today's tree no longer survives the crash."
    pass=0
fi
if [ "$got_bad" = "2" ]; then
    echo "⛔ FAIL: negative arm ALSO read r10=2 -- this gate is not measuring anything (SCRIP_RTX_PLANT_CLOBBER=1 no longer reaches the protected call)."
    pass=0
fi

if [ "$pass" = "1" ]; then
    echo "✅ PASS: r10 survives to a forced crash after the bare asm-runtime call returns, and the negative arm proves the check is real."
    exit 0
fi
exit 1
