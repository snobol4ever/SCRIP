#!/usr/bin/env bash
# test_gate_bb_block_label_prefix.sh — BB BLOCK LABEL PREFIX UNIFORMITY GATE
# (postoffice task bb-label-prefix-uniform, RE-OPENED AND WIDENED by Lon 2026-08-28: "ALL labels emitted
#  in the ALPHA and BETA block must change... EVERY variation of labels gets unified into ONE COMMON
#  PREFIX for all in each block" — the 08-27 slice (seat01, a01fe9f6) cured only the anonymous `.LxN`
#  internal-label serials; this gate is the mechanical proof that the widened ruling holds and stays held.
#
# WHAT IT WALKS: an emitted TEXT-mode (--compile) .s file, top to bottom, tracking a CURRENT BLOCK OWNER.
# A label shaped `n<digits>_<kind>_<greek>` (α/β/γ/ω — the box's OWN port, minted at emit.cpp's per-box
# label loop) is a REAL box boundary: it SETS the owner to <kind> (e.g. "n39_call_α" -> owner "call"). A
# label shaped `n<digits>_<anything-else>` (that SAME box's own β/bx/as/af/ry/rt/s<N> family) already
# carries its owning box's kind by construction and PRESERVES the current owner unchanged. A bareword
# label ending in a Greek suffix but NOT shaped `n<digits>_...` (main_α, PATTERN_BT_γ, or a DEFINE'd
# proc's own by-name entry point like "Push_α" — bb_define.cpp composes these directly from the proc
# name, mid-emission of whichever box is registering/marshalling that proc) is an ADDRESSABLE LANDING
# PAD, not a new box scope, so it ALSO preserves the current owner (measured: Push_α/_γ/_ω sit embedded
# inside a "define"-kind box's own output, and internal labels either side of them correctly say
# "define" — requiring them to say "Push" instead was the wrong bar and false-flagged real passes). Any
# OTHER bareword label (module_init, RETURN, a bare proc/label name with no box open on it, ...) is a
# genuine SCOPE RESET, clearing the owner to NONE until the next real anchor. Every remaining `.L`-
# prefixed label — internal code labels (the L(n) family), literal-pool / signature-blob / thunk-name
# data, whatever family a future template invents — is CONTENT: while an owner is established, its name
# MUST CONTAIN that owner as a substring, or it is a VIOLATION. A `.L`-label seen before the first real
# anchor (or after a scope reset — GVA name tables, the proc-registry startup block, the shared string/
# charset literal pool) carries no owner and is correctly out of scope: those are whole-program tables,
# not box-local state (measured against a real pattern_bt.s: .Lgvan*/.Lstartup_*/.Lseala* sit strictly
# before the chain's own first alpha or after module_init, never inside a bracket).
#
# TWO-PART PROOF (RULES.md): this gate FAILS on the pre-fix tree (`.Lmain_α_<uid>_<n>` — chain fam, not
# the owning box's own kind; ad hoc `.Lsig*`/`.Lrkfn*`/`.Lbynamefn*`/`.Lrtc*` mints with no box prefix at
# all) and PASSES on the fixed tree; both arms exercised by hand, on real corpus witnesses, before this
# file was minted — including the two false-positive traps found the same way (a shared mutable scratch
# buffer aliased into x86_uid_kind, and the Push/PATTERN_BT embedded-landing-pad shape above).
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root
set -u
SCRIP=${SCRIP:-$S4E/SCRIP/scrip}
DIR=${1:-$S4E/corpus/benchmarks/snobol4}
WORK=$(mktemp -d); trap 'rm -rf "$WORK"' EXIT
. "$(dirname "$0")/lib_gate.sh"
gate_require_exec "${SCRIP:-${SCRIP_BIN:-$(dirname "$0")/../scrip}}" "the scrip compiler"
gate_require "$DIR" "corpus dir $DIR"

CHECKER="$WORK/check.py"
cat > "$CHECKER" <<'PYEOF'
import re, sys
GREEK_SUFFIX = re.compile(r'_(α|β|γ|ω)$')   # _α _β _γ _ω
LABEL_DEF = re.compile(r'(?m)^[ \t]*(\.?[A-Za-z_]\w*):(?!\s*\.)')
BOXWORD = re.compile(r'^n\d+_')
NUM_PREFIX = re.compile(r'^n\d+_')   # box port labels are "n<uid>_<kind>_<greek>"; the KIND is the
                                      # attribution that matters (a human reads it in a diff/backtrace and
                                      # knows which BOX KIND crashed) -- the internal-label uid space is a
                                      # separate, deliberately-untouched counter (see x86_asm.h/emit.cpp:
                                      # x86_uid_kind), so requiring the numeral to also match would demand
                                      # threading a precomputed id through the whole per-box dispatch
                                      # pipeline for a purely cosmetic win. A block's required prefix is
                                      # therefore its owner's KIND with any leading "n<digits>_" stripped.

def classify(name):
    is_boxfam = bool(BOXWORD.match(name))          # starts "n<digits>_" -- this box's OWN family
    m = GREEK_SUFFIX.search(name)
    if is_boxfam and m:
        return 'anchor', name[:m.start()]           # n<uid>_<kind>_<greek> -- a REAL box boundary
    if is_boxfam:
        return 'preserve', None                     # n<uid>_<kind>_bx/_as/_af/_ry/_rt/_s<N> -- already
                                                      # self-conforming, doesn't change the current owner
    if not name.startswith('.L'):
        if m:
            # A bareword proc-name entry/exit point (Push_α, PATTERN_BT_γ, main_ω, ...) minted AS PART OF
            # the currently-dispatching box's own template (bb_define.cpp composes these directly from the
            # proc name, not through the n<uid>_<kind> port-label loop) -- it is an ADDRESSABLE LANDING PAD
            # for calling that proc BY NAME, not a new box/chain scope, so the owner it is embedded inside
            # (e.g. a "define" box registering/marshalling that very proc) must NOT change because of it.
            return 'preserve', None
        return 'reset', None                         # module_init, RETURN, a bare proc/label name, ... --
                                                       # genuinely a new top-level routine with no box owner
    return 'data', None

def required_prefix(owner):
    return NUM_PREFIX.sub('', owner, count=1)

def check(path):
    text = open(path, encoding='utf-8', errors='replace').read()
    owner = None
    violations = []
    for m in LABEL_DEF.finditer(text):
        name = m.group(1)
        kind, new_owner = classify(name)
        if kind == 'anchor':
            owner = new_owner
        elif kind == 'reset':
            owner = None
        elif kind == 'data':
            if owner and required_prefix(owner) not in name:
                line_no = text.count('\n', 0, m.start()) + 1
                violations.append((line_no, name, owner))
    return violations

total = 0
for path in sys.argv[1:]:
    vs = check(path)
    if vs:
        total += len(vs)
        print(f"VIOLATION {path}:")
        for line_no, name, owner in vs[:20]:
            print(f"  line {line_no}: {name!r} does not carry block owner {owner!r}")
        if len(vs) > 20:
            print(f"  ... and {len(vs) - 20} more in this file")
sys.exit(1 if total else 0)
PYEOF

examined=0
violated=0
failed_files=""
for f in "$DIR"/*.sno; do
    [ -e "$f" ] || continue
    b=$(basename "$f" .sno)
    timeout 30 "$SCRIP" --compile "$f" > "$WORK/$b.s" 2>"$WORK/$b.err" < /dev/null
    rc=$?
    # a program the emitter refuses (parse/emit FATAL) carries no label witness here -- test_corpus_snobol4.sh
    # is the correctness gate for THAT; a missing .s must never read as a clean label sweep (V2-5).
    [ $rc -ne 0 ] && continue
    [ -s "$WORK/$b.s" ] || continue
    examined=$((examined + 1))
    if ! python3 "$CHECKER" "$WORK/$b.s"; then
        violated=$((violated + 1))
        failed_files="$failed_files $b"
    fi
done

gate_floor "$examined" 1 "compiled witnesses"
[ -n "$failed_files" ] && echo "FILES WITH VIOLATIONS:$failed_files"
gate_verdict "$violated" "file(s) with a label inside an alpha/beta block not carrying the block's owning-box prefix"
