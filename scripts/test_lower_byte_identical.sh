#!/usr/bin/env bash
# test_lower_byte_identical.sh — SM bytecode byte-identical gate (SR-1)
#
# Compiles a corpus of small programs across all six frontends,
# captures --dump-sm output, and compares md5 hashes against a baked
# baseline.  Any drift from baseline = FAIL.
#
# Usage:
#   bash scripts/test_lower_byte_identical.sh            # compare
#   bash scripts/test_lower_byte_identical.sh --bake     # bake baseline
#
# AUTHORS: Lon Jones Cherryholmes · Claude Sonnet 4.6  DATE: 2026-05-11
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${HERE}/../scrip"
CORPUS="$S4E/corpus"
BASELINE="${HERE}/lower_baseline.txt"

PASS=0; FAIL=0; SKIP=0
BAKE=0
[ "${1:-}" = "--bake" ] && BAKE=1

# ⛔⭐⭐ THIS GATE MEASURES NOTHING AND HAS SAID "PASS" WHILE DOING SO. Measured 2026-08-30 (hq_B):
#   · `--dump-sm` IS NOT A FLAG. `grep -c dump-sm src/driver/scrip.c` = 0. Every unrecognised argument
#     falls through to being treated as a FILENAME, so every case here runs
#     `scrip --dump-sm <path>` -> "scrip: cannot open '--dump-sm'" on stderr, rc=1, EMPTY stdout.
#   · ALL 30 BAKED BASELINE HASHES ARE d41d8cd98f00b204e9800998ecf8427e, WHICH IS md5 OF THE EMPTY
#     STRING. Not some of them. All thirty.
#   · So the compare loop hashes empty output and finds it equal to a baked empty hash, forever, for
#     every frontend, whatever the compiler does. Its "PASS=5 FAIL=0" was five empties matching five empties.
# ⭐⭐ AND THE BAKE STEP IS WHY IT SURVIVED: --bake runs the SAME broken invocation and writes what
# it gets. A baseline generated through the instrument it is meant to check cannot disagree with it -- the
# defect is laundered into the expected values and the gate becomes perfectly self-consistent and perfectly
# empty. Any bake-and-compare gate has this hole; the cure is that the bake step must assert its output is
# non-trivial before writing it, which is the same "agreement on nothing is not agreement" law the corpus
# harness already carries three copies of.
# ⚠ A second defect hid the first: 25 of the 30 paths went stale in the corpus re-grid and the loop
# SKIPPED them, so only 5 cases ran. Had all 30 resolved it would have printed "PASS=30 FAIL=0" and read as
# a comprehensive six-frontend gate. The six icon/prolog paths are repointed below (demo/ -> demos/); the
# other 19 are absorbed into their language masters and every one is recoverable by ORIGIN through
# lib_master_extract.sh -- e.g. feat__f01_core_labels_goto, parser__arr_get, corpus__sc1_literals.
# ⛔ REFUSING rather than being deleted, because the QUESTION is legitimate and someone has to rule on
# it: SR-1 wants a post-lowering byte-identity check, and the SM dump it was written against in 2026-05 is
# gone. The successor is `--dump-ir` (or `--dump-bb`), but pinning IR hashes as goldens is a DESIGN call
# with a live precedent against it (RULES: .s artifacts must never be wired into a gate), so it is not one
# to make silently inside a repair. Row: sr1-lower-gate-instrument-is-gone-rebase-or-retire.
echo "⛔ GATE REFUSES: this gate's instrument does not exist." >&2
echo "   --dump-sm is not a scrip flag (0 occurrences in src/driver/scrip.c); it is parsed as a FILENAME," >&2
echo "   so every case produced empty stdout, and all 30 baked baseline hashes are md5(\"\")." >&2
echo "   A green run here has never been evidence of anything. See the banner in this file." >&2
echo "   FIX: rebase SR-1 onto --dump-ir/--dump-bb and re-bake with a non-trivial-output assertion, or" >&2
echo "        retire SR-1 and drop demos/{icon,prolog} from corpus_coverage_manifest.tsv's GATED bucket." >&2
exit 2

if [ ! -x "$SCRIP" ]; then
    echo "SKIP scrip not built at $SCRIP"
    exit 0
fi

if [ ! -d "$CORPUS" ]; then
    echo "SKIP corpus not found at $CORPUS"
    exit 0
fi

# ------------------------------------------------------------------
# Program list: ~10 per frontend × 6 = ~60 total
# Format: "label  flag  path"
# ------------------------------------------------------------------
declare -a PROGRAMS=(
    # SNOBOL4 (21 feat programs — all parse cleanly)
    "sno_f01  --dump-sm  $CORPUS/tests/snobol4/feat/f01_core_labels_goto.sno"
    "sno_f02  --dump-sm  $CORPUS/tests/snobol4/feat/f02_string_ops.sno"
    "sno_f03  --dump-sm  $CORPUS/tests/snobol4/feat/f03_numeric.sno"
    "sno_f04  --dump-sm  $CORPUS/tests/snobol4/feat/f04_pattern_primitives.sno"
    "sno_f05  --dump-sm  $CORPUS/tests/snobol4/feat/f05_capture.sno"
    "sno_f06  --dump-sm  $CORPUS/tests/snobol4/feat/f06_builtins_predicates.sno"
    "sno_f07  --dump-sm  $CORPUS/tests/snobol4/feat/f07_keywords.sno"
    "sno_f08  --dump-sm  $CORPUS/tests/snobol4/feat/f08_data_array_table.sno"
    "sno_f09  --dump-sm  $CORPUS/tests/snobol4/feat/f09_functions.sno"
    "sno_f10  --dump-sm  $CORPUS/tests/snobol4/feat/f10_io_basic.sno"

    # Icon
    "icn_family    --dump-sm  $CORPUS/demos/icon/demo/family_icon.icn"
    "icn_parser    --dump-sm  $CORPUS/demos/icon/demo/icon_parser.icn"
    "icn_recog     --dump-sm  $CORPUS/demos/icon/demo/icon_recognizer.icn"

    # Prolog
    "pl_family     --dump-sm  $CORPUS/demos/prolog/family_prolog.pl"
    "pl_parser     --dump-sm  $CORPUS/demos/prolog/prolog_parser.pl"
    "pl_recog      --dump-sm  $CORPUS/demos/prolog/prolog_recognizer.pl"

    # Raku
    "rk_arith_add  --dump-sm  $CORPUS/tests/raku/parser/arith_add.raku"
    "rk_arith_chain --dump-sm $CORPUS/tests/raku/parser/arith_chain.raku"
    "rk_arith_mul  --dump-sm  $CORPUS/tests/raku/parser/arith_mul.raku"
    "rk_arith_prec --dump-sm  $CORPUS/tests/raku/parser/arith_prec.raku"
    "rk_arr_get    --dump-sm  $CORPUS/tests/raku/parser/arr_get.raku"
    "rk_for_range  --dump-sm  $CORPUS/tests/raku/parser/for_range.raku"
    "rk_str_chars  --dump-sm  $CORPUS/tests/raku/parser/str_chars.raku"
    "rk_logic_or   --dump-sm  $CORPUS/tests/raku/parser/logic_or.raku"

    # Snocone
    "sc_literals   --dump-sm  $CORPUS/snocone/corpus/sc1_literals.sc"
    "sc_assign     --dump-sm  $CORPUS/snocone/corpus/sc2_assign.sc"
    "sc_control    --dump-sm  $CORPUS/snocone/corpus/sc4_control.sc"
    "sc_strings    --dump-sm  $CORPUS/snocone/corpus/sc8_strings.sc"
    "sc_wordcount  --dump-sm  $CORPUS/snocone/corpus/sc10_wordcount.sc"

    # Rebus
    "reb_btrees    --dump-sm  $CORPUS/tests/rebus/binary_trees.reb"
)

# ------------------------------------------------------------------
# Bake mode: generate baseline
# ------------------------------------------------------------------
if [ "$BAKE" = "1" ]; then
    echo "=== Baking SM lower baseline ==="
    rm -f "$BASELINE"
    for entry in "${PROGRAMS[@]}"; do
        read -r label flag path <<< "$entry"
        if [ ! -f "$path" ]; then
            echo "  SKIP $label (file missing: $path)"
            continue
        fi
        hash=$(timeout 15 "$SCRIP" $flag "$path" 2>/dev/null | md5sum | awk '{print $1}')
        printf '%s  %s\n' "$hash" "$label" >> "$BASELINE"
        echo "  BAKED $label  $hash"
    done
    echo "Baseline written: $BASELINE"
    exit 0
fi

# ------------------------------------------------------------------
# Compare mode
# ------------------------------------------------------------------
if [ ! -f "$BASELINE" ]; then
    echo "SKIP baseline not baked — run: bash $0 --bake"
    exit 0
fi

echo "=== SM lower byte-identical gate ==="

declare -A EXPECTED
while IFS='  ' read -r hash label; do
    EXPECTED["$label"]="$hash"
done < "$BASELINE"

for entry in "${PROGRAMS[@]}"; do
    read -r label flag path <<< "$entry"
    if [ ! -f "$path" ]; then
        echo "  SKIP $label (file missing)"
        SKIP=$((SKIP+1))
        continue
    fi
    expected="${EXPECTED[$label]:-}"
    if [ -z "$expected" ]; then
        echo "  SKIP $label (not in baseline)"
        SKIP=$((SKIP+1))
        continue
    fi
    actual=$(timeout 15 "$SCRIP" $flag "$path" 2>/dev/null | md5sum | awk '{print $1}')
    if [ "$actual" = "$expected" ]; then
        echo "  PASS $label"
        PASS=$((PASS+1))
    else
        echo "  FAIL $label  expected=$expected  got=$actual"
        FAIL=$((FAIL+1))
    fi
done

echo ""
echo "PASS=$PASS FAIL=$FAIL SKIP=$SKIP"
[ "$FAIL" -eq 0 ]
