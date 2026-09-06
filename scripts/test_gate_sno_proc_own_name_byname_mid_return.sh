#!/usr/bin/env bash
# test_gate_sno_proc_own_name_byname_mid_return.sh -- a DEFINE'd proc's own name reads BLANK through an
# external by-name ($NAME) lookup while that proc is in the middle of returning.
#
# Row: defined-proc-own-name-blank-via-byname-lookup-mid-return (hq_U). Witness minted and oracle-verified by
# seat11 2026-09-05; this gate is the runnable criterion for it (the row carried the placeholder DONE-WHEN and
# was therefore UNPICKABLE -- ceo census 19:02).
#
# ⛔ WHAT THIS IS NOT (seat11 ruled these out; do not re-derive them):
#   - NOT the retval threaded through rt_trace_return_hook: rt_trace_event's registered-callback branch never
#     reads its own value parameter, so that plumbing is correct but INERT here.
#   - NOT an RTCC_GLOBAL_R9_GVA vs absolute-addressing split: an ordinary global's own by-name read through the
#     identical NAMEVAL path, in the identical callback, resolves correctly.
#   F differs only in being the name of the procedure CURRENTLY ACTIVE/returning at the instant of the lookup.
#
# TRACE is the INSTRUMENT here, not the subject: it is currently the only way to run code at the exact instant
# of a return before it completes. A cure is a symbol-resolution fix, not a TRACE fix.
#
# Graded in BOTH modes against /home/resources/x64/bin/sbl -bf. rc=0 both match, rc=1 either diverges,
# rc=2 could not measure (no oracle, no binary, build/link failure, zero programs graded).
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
GATE_NAME="$(basename "${BASH_SOURCE[0]}")"
. "$HERE/lib_oracle_flags.sh" || { echo "GATE UNPROVEN(2) [$GATE_NAME]: lib_oracle_flags.sh unavailable"; exit 2; }
. "$HERE/lib_gate.sh"          || { echo "GATE UNPROVEN(2) [$GATE_NAME]: lib_gate.sh unavailable"; exit 2; }
gate_parse_args "$@"
O="$(sbl_correctness_bin)" || exit 2
gate_require_exec "$ROOT/scrip" "the scrip binary"
T="$(mktemp -d)" || { echo "GATE UNPROVEN(2) [$GATE_NAME]: mktemp failed"; exit 2; }
trap 'rm -rf "$T"' EXIT
W="$T/ownname.sno"
{ printf '        &TRACE = 10\n'
  printf '        DEFINE("TF(NAME,TAG)")\n'
  printf '        DEFINE("F(X)")                          :(START)\n'
  printf 'TF      OUTPUT = TAG " " NAME " = " $NAME       :(RETURN)\n'
  printf 'F       F = X                                   :(RETURN)\n'
  printf 'START   TRACE("F","RETURN","tagR",.TF)\n'
  printf '        N = F(2)\n'
  printf 'END\n'; } > "$W"
ORA="$( cd "$T" && timeout 30s "$O" -bf ownname.sno </dev/null 2>/dev/null )"; orc=$?
case "$ORA" in *tagR*) : ;; *) echo "GATE UNPROVEN(2) [$GATE_NAME]: oracle produced no tagR line (rc=$orc)"; exit 2;; esac
M3="$( cd "$T" && timeout 30s "$ROOT/scrip" ownname.sno </dev/null 2>/dev/null )"
( cd "$T" && timeout 30s "$ROOT/scrip" --compile -o ownname.s ownname.sno </dev/null >/dev/null 2>&1 ) \
  || { echo "GATE UNPROVEN(2) [$GATE_NAME]: mode-4 compile failed"; exit 2; }
( cd "$T" && gcc -no-pie ownname.s -o ownname.bin -L"$ROOT/out" -lscrip_rt -lm -Wl,-rpath,"$ROOT/out" >/dev/null 2>&1 ) \
  || { echo "GATE UNPROVEN(2) [$GATE_NAME]: mode-4 link failed"; exit 2; }
M4="$( cd "$T" && timeout 30s ./ownname.bin </dev/null 2>/dev/null )"
red=0; examined=0
for tag in m3 m4; do
    eval "got=\$$(echo "$tag" | tr 'a-z' 'A-Z')"
    examined=$((examined+1))
    if [ "$got" = "$ORA" ]; then echo "PASS $tag: [$got]"
    else echo "RED  $tag: got [$got] oracle [$ORA]"; red=$((red+1)); fi
done
gate_floor "$examined" 2 "modes graded"
gate_verdict "$red" "mode(s) where a returning proc's own name resolves blank through a by-name lookup"
