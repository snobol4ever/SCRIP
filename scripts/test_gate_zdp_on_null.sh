#!/usr/bin/env bash
# test_gate_zdp_on_null.sh — ⭐⭐⭐⭐⭐ THE ON-NULL GATE (s137, mandated by FINDING-2026-08-17-s136).
#
# THE LAW THIS ENFORCES: **A DEFAULT-INERT PASS IS NOT AN INERT PASS.**  A pass that is off by default can still
# perturb the program it measures the moment it is switched on -- and then every number it reports is a verdict on a
# DIFFERENT program than the one that ships.  That is the s132 MONITOR lesson, and s136 proved the instrument built to
# replace the monitor had inherited it verbatim: `SCRIP_ZDP=1 ./scrip --compile` moved 81 of 656 programs.
#
# The pre-existing manifest (util_zdp_stdout_manifest.sh) only ever compared instrument-OFF against a prior build.  It
# proved the pass contributes nothing WHEN DISABLED -- which is not the property that matters for a pass whose entire
# purpose is to run.  This gate proves the property that does matter:
#
#     compile STDOUT with the pass ON  ==  compile STDOUT with the pass OFF,  for every program.
#
# ⛔ HASHES COMPILE STDOUT, NEVER the committed .s beside the .sno.  Hashing the committed artifact is what made the
# s133 gate report a fake zero: the artifacts are regenerated on a different schedule than the compiler that emits them.
#
# Usage:  bash scripts/test_gate_zdp_on_null.sh [ENV_ASSIGNMENT ...]
#         bash scripts/test_gate_zdp_on_null.sh                     # defaults to SCRIP_ZDP=1
#         bash scripts/test_gate_zdp_on_null.sh SCRIP_ZONE=1
#         bash scripts/test_gate_zdp_on_null.sh SCRIP_ZDP=1 SCRIP_ZONE=1
# Exit 0 = 0 movers (the pass is genuinely inert).  Exit 1 = movers listed (the pass perturbs; its numbers are void).
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
set -u
cd "$(dirname "$0")/.." || exit 1
[ -x ./scrip ] || { echo "FAIL  ./scrip not built"; exit 1; }
ENVS=("$@"); [ ${#ENVS[@]} -eq 0 ] && ENVS=("SCRIP_ZDP=1")
CORPUS_DIRS="${CORPUS_DIRS:-$S4E/corpus/tests/snobol4 $S4E/corpus/probe}"
LIST=$(find $CORPUS_DIRS -name '*.sno' 2>/dev/null | sort)
TOTAL=0; MOVED=0; FLAKY=0; MOVERS=""; FLAKIES=""
for f in $LIST; do
    TOTAL=$((TOTAL + 1))
    off=$(timeout 15 ./scrip --compile "$f" < /dev/null 2>/dev/null | md5sum | cut -d' ' -f1)
    on=$(env "${ENVS[@]}" timeout 15 ./scrip --compile "$f" < /dev/null 2>/dev/null | md5sum | cut -d' ' -f1)
    [ "$off" = "$on" ] && continue
    # ⭐ SELF-NULL CLASSIFICATION (s137).  A program that emits different bytes on two IDENTICAL runs is nondeterministic
    # ON ITS OWN and cannot testify about the pass either way.  Counting it as a "mover" is how a real perturbation and a
    # pre-existing flake get averaged into one meaningless number -- and it is why the s136 OFF null read 1 instead of 0.
    off2=$(timeout 15 ./scrip --compile "$f" < /dev/null 2>/dev/null | md5sum | cut -d' ' -f1)
    if [ "$off" != "$off2" ]; then FLAKY=$((FLAKY + 1)); FLAKIES="$FLAKIES$f"$'\n'; continue; fi
    MOVED=$((MOVED + 1)); MOVERS="$MOVERS$f"$'\n'
done
echo "ON-NULL GATE  env='${ENVS[*]}'  total=$TOTAL  movers=$MOVED  self-nondeterministic=$FLAKY"
if [ "$FLAKY" -ne 0 ]; then
    echo "-- SELF-NONDETERMINISTIC (a separate defect; these cannot testify about the pass) --"
    echo "$FLAKIES" | head -20
fi
if [ "$MOVED" -ne 0 ]; then
    echo "$MOVERS" | head -40
    echo "FAIL  the pass PERTURBS the program it measures -- every number it reports is void until this is 0."
    exit 1
fi
# NON-VACUITY: a gate that hashes nothing passes trivially.  s133 shipped exactly that and called it a zero.
if [ "$TOTAL" -lt 100 ]; then echo "FAIL  vacuous: only $TOTAL programs hashed"; exit 1; fi
echo "PASS  the pass is inert with respect to emitted code."
exit 0
