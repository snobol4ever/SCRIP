#!/usr/bin/env bash
# test_gate_port_exit_value_contract.sh — THE PORT-EXIT VALUE CONTRACT gate.
# Row: port-exit-value-contract-untagged-rax-forges-dt-fail (hq_P-owned; class triaged by hq_C from
# seat09's Pascal root-cause).  Built by seat02 2026-09-01 as that row's CENSUS+GATE rung.
#
# THE CONTRACT: a procedure-level success exit (`<proc>_γ:` opening `mov rdi, rax` / `mov rsi, rdx`)
# forwards rax:rdx as the procedure's DESCR_t return value, and the caller detects a failed nested
# call with `cmp al, 104` (DT_FAIL = 0x68; DESCR_t.v is uint8_t, so the tag IS rax's low byte).
# EVERY transfer reaching such an exit must therefore arrive with a real DESCR_t in rax:rdx.
# A transfer arriving with an untagged value forges DT_FAIL whenever that value's low byte is 104 —
# a silent, data-dependent (or, for a `lea`'d code address, LINK-LAYOUT-dependent) wrong answer that
# cascades a spurious ω one frame at a time up the entire live call chain.
#
# ⭐ IT IS A GENERATION-TIME CHECK, DELIBERATELY, per the row's DONE-WHEN: the defect is
# data-dependent, so a runtime assert fires only on the inputs that already broke.  This reads the
# emitted mode-4 TEXT and answers the question for every path, including unexercised ones.
# ⛔ MODE-SCOPED (MODES MAY DIVERGE, Lon 2026-08-28): this reads mode-4 TEXT and binds mode 4 only.
#
# Usage: bash scripts/test_gate_port_exit_value_contract.sh [--informational] [-- <program>...]
#        SCRIP_PEVC_PROGRAMS="a.pas b.pl"  bash scripts/test_gate_port_exit_value_contract.sh
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
cd "$ROOT"
GATE_NAME="port-exit-value-contract"
. "$HERE/lib_gate.sh"
gate_parse_args "$@"

SCAN="$HERE/port_exit_value_contract_scan.py"
gate_require_exec "$ROOT/scrip" "the scrip compiler (make -j4 scrip)"
gate_require "$SCAN" "the scan half of this gate"

CORP="$(cd "$ROOT/../corpus" 2>/dev/null && pwd || true)"
if [ -n "${SCRIP_PEVC_PROGRAMS:-}" ]; then read -r -a PROGS <<< "$SCRIP_PEVC_PROGRAMS"
else
    PROGS=()
    for p in benchmarks/pascal/quick.pas benchmarks/pascal/queens.pas benchmarks/pascal/perm.pas \
             benchmarks/pascal/fbench.pas demos/prolog/family.pl benchmarks/icon/tgrlink.icn; do
        [ -n "$CORP" ] && [ -f "$CORP/$p" ] && PROGS+=("$CORP/$p")
    done
fi
[ "${#PROGS[@]}" -eq 0 ] && { echo "GATE UNPROVEN(2) [$GATE_NAME]: no witness programs found (corpus at '${CORP:-?}')";
                              echo "    This is NOT a pass. The gate compiled nothing, so it certifies nothing."; gate_stamp; exit 2; }

WORK="$(mktemp -d "${TMPDIR:-/tmp}/pevc.XXXXXX")"; trap 'rm -rf "$WORK"' EXIT
ASM=(); BUILT=0
for f in "${PROGS[@]}"; do
    b="$(basename "$f" | tr './' '__')"
    if timeout 60s "$ROOT/scrip" --compile "$f" < /dev/null > "$WORK/$b.s" 2>/dev/null && [ -s "$WORK/$b.s" ]; then
        ASM+=("$WORK/$b.s"); BUILT=$((BUILT+1))
    else
        echo "   ⚠ could not compile, excluded from the verdict: $f"
    fi
done
[ "$BUILT" -eq 0 ] && { echo "GATE UNPROVEN(2) [$GATE_NAME]: every witness failed to compile";
                        echo "    This is NOT a pass. 'measured and clean' and 'never ran' must not share an output."; gate_stamp; exit 2; }

JSON="$WORK/scan.json"
# ⛔ THE CALL ARM'S ALLOW-LIST, DERIVED FROM SOURCE EVERY RUN (seat03 2026-09-01).  The scanner used to treat
# every `call` as DESCR-returning; measured, 9 of 10 such transfers in this witness set call int-returning
# runtime functions, so the reported violation count was half the real one.  Derived here rather than
# hand-maintained, because a hand list decays and this gate's whole purpose is to not depend on a census
# staying complete.  Fail-closed: the scanner REFUSES if this file is missing or empty.
DFNS="$WORK/descr_fns.txt"
grep -rhoE '\bDESCR_t[[:space:]]+\**[[:space:]]*(rt_|c_rt_|scrip_)[A-Za-z0-9_]+[[:space:]]*\(' \
     "$ROOT/src/runtime/" "$ROOT/src/ir/" 2>/dev/null \
  | sed -E 's/^DESCR_t[[:space:]]*\**[[:space:]]*//; s/[[:space:]]*\($//' | sort -u > "$DFNS"
if [ ! -s "$DFNS" ]; then
    echo "GATE UNPROVEN(2) [$GATE_NAME]: could not derive the DESCR-returning symbol set from src/"
    echo "    This is NOT a pass. With an empty allow-list every call reads as a violation."; gate_stamp; exit 2
fi
echo "   DESCR-returning runtime symbols in the call allow-list: $(wc -l < "$DFNS")"
python3 "$SCAN" "--descr-fns=$DFNS" "${ASM[@]}" > "$JSON" 2>"$WORK/scan.err" || {
    echo "GATE UNPROVEN(2) [$GATE_NAME]: the scanner itself failed"; sed -n '1,10p' "$WORK/scan.err"; gate_stamp; exit 2; }

read -r EXITS XFERS VIOL <<< "$(python3 - "$JSON" <<'PY'
import json,sys
r=json.load(open(sys.argv[1]))
e=sum(len(f['exits']) for f in r); x=sum(len(f['findings']) for f in r)
v=sum(1 for f in r for y in f['findings'] if y['verdict']=='RAW')
print(e,x,v)
PY
)"
python3 - "$JSON" <<'PY'
import json,sys,collections
r=json.load(open(sys.argv[1]))
c=collections.Counter(y['verdict'] for f in r for y in f['findings'])
print("   programs %d | rax-forwarding procedure exits %d | transfers into them %d"
      % (len(r), sum(len(f['exits']) for f in r), sum(len(f['findings']) for f in r)))
print("   " + "  ".join("%s=%d"%(k,v) for k,v in sorted(c.items())))
for f in r:
    bad=[y for y in f['findings'] if y['verdict']=='RAW']
    if not bad: continue
    print("   ⛔ %s" % f['file'].split('/')[-1])
    for y in bad[:6]:
        print("        line %-7d %-5s -> %-22s %s" % (y['line'],y['mnem'],y['target'],y['detail'][:88]))
    if len(bad)>6: print("        ... and %d more" % (len(bad)-6))
PY

# ⛔ the cannot-measure floor: zero exits examined is NOT a clean tree, it is a broken instrument.
gate_floor "$EXITS" 1 "rax-forwarding procedure exits"
gate_verdict "$VIOL" "port exits reached with an unnormalized value in rax (forgeable DT_FAIL)"
