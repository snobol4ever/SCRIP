#!/bin/bash
# test_gate_argnote_sweep.sh -- OBJ-NOTE ON-3 ARG-NOTE gate.  Compiles the SNOBOL4 corpus in mode-4 and asserts
# the three annotation invariants the ladder names: (1) ZERO stray '#@' markers survive into a .s -- an unfolded
# marker means the fold missed a chunk boundary; (2) ZERO notes on jump lines -- the GOTO column belongs to the
# jumps, Lon's explicit directive; (3) emit failures held at the pre-existing baseline, so an annotation pass is
# never the thing that broke a program.  Reports the annotation volume so a regression to zero is visible too.
set -u
SCRIP=${SCRIP:-/home/claude/SCRIP/scrip}
CORPUS=${CORPUS:-/home/claude/corpus}
LIMIT=${LIMIT:-0}
TMP=$(mktemp -d)
trap 'rm -rf "$TMP"' EXIT
stray=0; jumpnote=0; emitfail=0; total=0; notes=0; failed=""
JRE='^[[:space:]]+(jmp|je|jne|jg|jl|jge|jle|ja|jb|jae|jbe|js|jns|jz|jnz|jc|jnc|jo|jno|jp|jnp|jecxz|jrcxz)[[:space:]].*#'
for f in $(find "$CORPUS" -name '*.sno' | sort); do
    [ "$LIMIT" -gt 0 ] && [ "$total" -ge "$LIMIT" ] && break
    total=$((total + 1))
    out="$TMP/x.s"
    # ⛔ CWD MATTERS: a program using -INCLUDE names its includes RELATIVE to its own directory, so a sweep
    # that compiles from elsewhere manufactures ~120 phantom "cannot open include" failures and reports them
    # as regressions.  Measured s23d: the naive sweep read 266 fails, the correct one far fewer.
    if ! ( cd "$(dirname "$f")" && timeout 25 "$SCRIP" --compile "$(basename "$f")" ) > "$out" 2>"$TMP/err"; then
        emitfail=$((emitfail + 1)); failed="$failed $f"; continue
    fi
    s=$(grep -c '^[[:space:]]*#@' "$out" 2>/dev/null); s=${s:-0}
    j=$(grep -cE "$JRE" "$out" 2>/dev/null); j=${j:-0}
    n=$(grep -c '#' "$out" 2>/dev/null); n=${n:-0}
    stray=$((stray + s)); jumpnote=$((jumpnote + j)); notes=$((notes + n))
    if [ "$s" -ne 0 ] || [ "$j" -ne 0 ]; then echo "VIOLATION $f stray=$s jumpnote=$j"; fi
done
echo "programs=$total  emit-decline=$emitfail  stray-markers=$stray  notes-on-jumps=$jumpnote  notes=$notes"
echo "NOTE: emit-decline is NOT the watermark and must never be quoted as one -- this sweep walks EVERY .sno in the"
echo "      tree, including CRLF files, include fragments, and programs outside the graded set.  The watermark is"
echo "      xc.sh over corpus/crosscheck (.ref-anchored, both modes).  This number is a TRIPWIRE for the two"
echo "      annotation invariants above, nothing more."
[ -n "$failed" ] && { echo "EMIT-FAIL SET:"; for x in $failed; do echo "  $x"; done; }
if [ "$stray" -eq 0 ] && [ "$jumpnote" -eq 0 ]; then echo "GATE GREEN"; exit 0; fi
echo "GATE RED"; exit 1
