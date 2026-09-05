#!/usr/bin/env bash
# test_gate_modes_declaration_travels.sh — THE `modes` DECLARATION MUST TRAVEL WITH THE SUITE IT DESCRIBES.
# (hq_T 2026-09-05, row test-suite-consistency-seven-languages-one-standard; the cross-language census hq_B
# asked for after util_census_optimizer_bypass.py was found grading 28 modes=ast entries by EXECUTION.)
#
# WHAT IT PROTECTS. Every master declares, per entry, HOW it is graded: `ast` entries have a --dump-ast dump
# for a .ref and must be graded by --dump-ast; the rest are executed. Grade an `ast` entry by execution and
# you get a red that means nothing; the harness therefore REFUSES a `--modes m3,m4` run over a suite that
# declares any. ⛔ THAT GUARD READ ONE FILE -- a sibling ALL.csv -- SO ITS ACTIVATION DEPENDED ON WHERE THE
# CALLER PUT THE SUITE, not on what the suite declares. Every runner that grades an EXTRACTED family in a
# tempdir (`extract-family`, the documented bridge, and test_snocone_corpus_suite.sh's own shape) had no
# ALL.csv beside it, so the guard saw nothing and said nothing, which is indistinguishable from a pass.
#
# MEASURED BOTH WAYS ON ONE PAIR OF COMMANDS (pascal's 5 modes=ast parser entries, 2026-09-05):
#   graded IN PLACE      -> rc=2 REFUSING, the guard's evidence is beside it
#   extracted, same run  -> rc=1 and a full plausible board: total=5 m3_fail=5 m4_fail=5
# ⭐ AND THE FIVE WERE ALL PASSING. Graded by the instrument they declare, the same five entries are
# ast_pass=5 ast_fail=0. The false board was not merely unreliable, it was wrong in every cell it printed.
#
# ⛔ THE OTHER HALF, and the reason this is a defect rather than a missing feature: --by-modes-column
# REFUSED on an extracted pair for want of that sibling csv. On an extraction the CORRECT call was
# impossible and the incorrect call was silent -- which is not a choice a caller can be blamed for making.
# The cure is that extract-family carries a `.modes` sidecar the way it already carries `.in` and `.xfail`,
# under the law its own docstring states: A CHECK THAT DOES NOT CARRY EVERY FIELD THE GRADER READS IS NOT A
# CHECK (hq_C). `modes` was the one field it did not carry.
#
# ⭐ WHY A GATE AND NOT A FIXED CALL SITE: the census found the class live in a SECOND tool after it had
# been cured in the first (hq_B's master board runner, then util_census_optimizer_bypass.py), and a third
# and fourth are one `extract-family` away. A defect that reproduces across independent lanes is in the
# thing they share -- so the property is asserted on the shared thing, in every language that has one.
#
# Usage: bash scripts/test_gate_modes_declaration_travels.sh
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
cd "$ROOT"
. "$HERE/lib_gate.sh"
gate_parse_args "$@"

S4E="${S4E_HOME:-$(cd "$ROOT/.." && pwd)}"
HARNESS="$HERE/corpus_suite_harness.py"
TESTS="$S4E/corpus/tests"
gate_require "$HARNESS" "corpus_suite_harness.py, the ONE grading authority"
gate_require "$TESTS" "the per-language master suites"

violations=0
examined=0
W="$(mktemp -d "${TMPDIR:-/tmp}/modes_travels.XXXXXX")"
trap 'rm -rf "$W"' EXIT

# The smallest family whose entries are ALL `ast`, and the smallest carrying NO `ast`, per language --
# read off each master's own ALL.csv. ⛔ CHOSEN BY MEASUREMENT, NEVER PINNED: a hardcoded family name is a
# pinned population, which is the anti-pattern this lane spent 2026-09-05 removing from another gate.
PLAN="$W/plan.tsv"
python3 - "$TESTS" "$PLAN" <<'PY'
import csv, collections, os, sys
tests, out = sys.argv[1], sys.argv[2]
rows_out = []
for lang in sorted(os.listdir(tests)):
    csvp = os.path.join(tests, lang, "ALL.csv")
    if not os.path.isfile(csvp):
        continue
    ext = None
    for cand in (".sno", ".icn", ".pl", ".sc", ".reb", ".raku", ".pas"):
        if os.path.isfile(os.path.join(tests, lang, "ALL" + cand)):
            ext = cand
            break
    if ext is None:
        continue
    fam = collections.defaultdict(collections.Counter)
    with open(csvp, newline="") as f:
        for r in csv.DictReader(f):
            fam[r.get("family", "?")][(r.get("modes") or "").strip()] += 1
    ast = sorted((sum(v.values()), k) for k, v in fam.items() if set(v) == {"ast"})
    run = sorted((sum(v.values()), k) for k, v in fam.items() if "ast" not in v)
    rows_out.append("\t".join([lang, ext,
                               ast[0][1] if ast else "-", str(ast[0][0]) if ast else "0",
                               run[0][1] if run else "-", str(run[0][0]) if run else "0"]))
open(out, "w").write("\n".join(rows_out) + "\n")
PY
if [ ! -s "$PLAN" ]; then
    echo "GATE REFUSES (rc=2): no master suite with an ALL.csv under $TESTS -- nothing to measure"
    exit 2
fi

langs_with_ast=0
while IFS=$'\t' read -r lang ext astfam astn runfam runn; do
    [ -n "${lang:-}" ] || continue
    M="$TESTS/$lang/ALL$ext"; R="$TESTS/$lang/ALL.ref"; C="$TESTS/$lang/ALL.csv"
    [ -f "$M" ] && [ -f "$R" ] || continue

    # ARM A — the declaration travels, for EVERY language, ast or not. A language with no ast entries at
    # all (prolog today) still proves the carry: the sidecar must exist and repeat what the csv declares.
    fam="$astfam"; [ "$fam" = "-" ] && fam="$runfam"
    [ "$fam" = "-" ] && continue
    examined=$((examined + 1))
    d="$W/$lang"; mkdir -p "$d"
    if ! python3 "$HARNESS" extract-family "$M" "$R" "$C" "$fam" "$d/f$ext" "$d/f.ref" >/dev/null 2>&1; then
        echo "GATE FAIL [$lang]: extract-family '$fam' failed -- cannot measure whether its declaration travels"
        violations=$((violations + 1)); continue
    fi
    if [ ! -f "$d/f.modes" ]; then
        echo "GATE FAIL [$lang]: extract-family wrote no .modes sidecar beside $fam -- the field the grader"
        echo "                   reads did not travel, so every guard downstream is blind on this pair"
        violations=$((violations + 1)); continue
    fi
    want="$(python3 - "$C" "$fam" <<'PY'
import csv, sys
c, fam = sys.argv[1], sys.argv[2]
with open(c, newline="") as f:
    print("\n".join("%s\t%s" % (r["entry"], (r.get("modes") or "").strip())
                    for r in csv.DictReader(f) if r.get("family") == fam))
PY
)"
    got="$(grep -v '^#' "$d/f.modes" | sed '/^$/d')"
    if [ "$want" != "$got" ]; then
        echo "GATE FAIL [$lang]: the .modes sidecar does not match the master's own csv for family $fam"
        diff <(printf '%s\n' "$want") <(printf '%s\n' "$got") | head -6 | sed 's/^/    /'
        violations=$((violations + 1))
    fi

    # ARM B — and on an ast-declaring extraction, a run asked for m3,m4 WITHOUT --by-modes-column must
    # REFUSE rc=2. This is the arm that was rc=1-with-a-board before the cure. It costs nothing: the
    # refusal fires before a single entry is compiled.
    [ "$astfam" = "-" ] && continue
    langs_with_ast=$((langs_with_ast + 1))
    examined=$((examined + 1))
    out="$(timeout 120 python3 "$HARNESS" run "$d/f$ext" "$d/f.ref" --lang "$lang" --modes m3,m4 2>&1)"; rc=$?
    if [ "$rc" -ne 2 ]; then
        echo "GATE FAIL [$lang]: an extracted family declaring $astn modes=ast entr(y/ies) was graded by"
        echo "                   EXECUTION without refusing (rc=$rc) -- manufactured reds that mean nothing"
        printf '%s\n' "$out" | grep -E '^SUITE_BOARD' | head -1 | sed 's/^/    /'
        violations=$((violations + 1))
    elif ! printf '%s' "$out" | grep -q 'modes=ast'; then
        echo "GATE FAIL [$lang]: refused (rc=2) but did not name the ast declaration as the reason"
        violations=$((violations + 1))
    fi
done < "$PLAN"

# ARM C — THE CORRECT CALL MUST BE POSSIBLE ON AN EXTRACTION, which is the half that makes ARM B fair.
# Before the cure --by-modes-column refused on an extracted pair for want of a sibling ALL.csv, so a
# runner had no way to do the right thing. Graded on the cheapest all-ast family on the plan.
examined=$((examined + 1))
read -r clang cext cfam cn _ < <(sort -t$'\t' -k4,4n "$PLAN" | awk -F'\t' '$3!="-"{print $1"\t"$2"\t"$3"\t"$4; exit}')
if [ -z "${cfam:-}" ]; then
    echo "GATE NOTE: no language declares an all-ast family -- ARM C has nothing to grade (not a failure)"
else
    out="$(timeout 300 python3 "$HARNESS" run "$W/$clang/f$cext" "$W/$clang/f.ref" --lang "$clang" \
             --modes m3,m4 --by-modes-column 2>&1)"; rc=$?
    board="$(printf '%s\n' "$out" | grep -m1 '^SUITE_BOARD_AST')"
    if [ "$rc" -eq 2 ] || [ -z "$board" ]; then
        echo "GATE FAIL: --by-modes-column could not honour the declaration on an EXTRACTED pair ($clang/$cfam, rc=$rc)"
        printf '%s\n' "$out" | tail -2 | sed 's/^/    /'
        violations=$((violations + 1))
    elif ! printf '%s' "$board" | grep -q "total=$cn"; then
        echo "GATE FAIL: --by-modes-column graded the wrong population on $clang/$cfam (wanted total=$cn)"
        printf '%s\n' "$board" | sed 's/^/    /'
        violations=$((violations + 1))
    fi
fi

# ARM D — SCOPED, AND THE SCOPE IS PROVEN, NOT ASSERTED. A gate broader than its rule gets switched off by
# the first person it blocks for a good reason, so: a family that declares NO ast entries must still grade
# by execution with no refusal. Cheapest non-ast family on the plan.
examined=$((examined + 1))
read -r nlang next nfam nn _ < <(sort -t$'\t' -k6,6n "$PLAN" | awk -F'\t' '$5!="-"{print $1"\t"$2"\t"$5"\t"$6; exit}')
if [ -z "${nfam:-}" ]; then
    echo "GATE NOTE: every family on the plan declares ast -- ARM D has nothing to grade (not a failure)"
else
    nd="$W/scope_$nlang"; mkdir -p "$nd"
    if ! python3 "$HARNESS" extract-family "$TESTS/$nlang/ALL$next" "$TESTS/$nlang/ALL.ref" \
            "$TESTS/$nlang/ALL.csv" "$nfam" "$nd/f$next" "$nd/f.ref" >/dev/null 2>&1; then
        echo "GATE FAIL: could not extract the non-ast control family $nlang/$nfam"
        violations=$((violations + 1))
    else
        out="$(timeout 300 python3 "$HARNESS" run "$nd/f$next" "$nd/f.ref" --lang "$nlang" --modes m3,m4 2>&1)"; rc=$?
        if [ "$rc" -eq 2 ] && printf '%s' "$out" | grep -q 'modes=ast'; then
            echo "GATE FAIL: the ast guard fired on $nlang/$nfam, which declares no ast entry at all --"
            echo "           over-broad, and it would red every honest execution board on the box"
            violations=$((violations + 1))
        fi
    fi
fi

# ARM E — ⛔⭐⭐ A DECLARATION THAT MATCHES NOTHING IS NOT A DECLARATION. ARMS A-D prove the declaration
# TRAVELS with its suite; none of them proves any key in it MATCHES an entry, so a MODES.tsv can be 100%
# orphaned and travel perfectly. Measured 2026-09-05 (hq_B's find, routed to this lane, and this arm is the
# guard they asked for): snobol4 families were renamed X__Y -> X and MODES.tsv was never followed, so 71 of
# its 72 keys matched NOTHING -- only `ladder` resolved -- and 459 of 460 real families fell to the UNKNOWN
# default, which is how 28 --dump-ast fixtures came to be graded BY EXECUTION and printed a full, plausible,
# entirely false board.
# ⭐ THE SHAPE, which is why this is a permanent arm and not a one-time repair: `modes_decl.get(fam, DEFAULT)`
# reports DEFAULT with identical confidence for "this family is declared DEFAULT" and "our two key sets have
# nothing in common". A lookup's default is an invariance proof over exactly the keys that happened to match,
# and it never says which case it is in. So the arm prints the DENOMINATOR -- declared, orphaned, and the
# undeclared entry rate -- because a count with no denominator cannot distinguish those two either.
# ⛔ RATCHET, NOT A BAR, AND DELIBERATELY SO. Pinning the bar at zero today would red make test on three
# languages at once over debt this lane did not create and cannot honestly restamp -- which is how a gate
# earns a `|| true` within a week. The watermarks below are a DEBT LEDGER measured 2026-09-05 on corpus
# e2f9c2f2c: they may only ever go DOWN, and lowering one when you cure a language is part of curing it.
# snobol4 71 is hq_B's, whose key repair was measured but is NOT on origin as of e2f9c2f2c (checked against a
# fresh fetch) -- when it lands this drops to 0. pascal 5 and icon 3 are named in hq_B's FINDING and unowned.
examined=$((examined + 1))
orph_out="$(S4E_HOME="$S4E" python3 - <<'EOF'
import csv, importlib.util, os, sys
spec = importlib.util.spec_from_file_location("b", os.path.join("scripts", "util_build_master_suite.py"))
b = importlib.util.module_from_spec(spec); spec.loader.exec_module(b)
# The DEBT LEDGER. Lower a number when you cure its language; never raise one.
WATERMARK = {"snobol4": 71, "pascal": 5, "icon": 3, "prolog": 0, "raku": 0, "snocone": 0, "rebus": 0}
tests = os.path.join(os.environ.get("S4E_HOME", ".."), "corpus", "tests")
graded, bad = 0, 0
print("  %-9s %8s %8s %8s %s" % ("lang", "declared", "orphaned", "cap", "undeclared entries (the UNKNOWN default's real reach)"))
for lang in sorted(WATERMARK):
    root = os.path.join(tests, lang)
    csv_path = os.path.join(root, "ALL.csv")
    if not os.path.isfile(csv_path):
        continue
    decl = b.read_modes_decl(root)
    fams = {}
    with open(csv_path, encoding="utf-8") as fh:
        for row in csv.DictReader(fh):
            f = row.get("family") or ""
            fams[f] = fams.get(f, 0) + 1
    if not fams:
        continue
    graded += 1
    orphaned = sorted(set(decl) - set(fams))
    undecl_fams = sorted(set(fams) - set(decl))
    undecl_entries = sum(fams[f] for f in undecl_fams)
    total = sum(fams.values())
    cap = WATERMARK[lang]
    flag = "" if len(orphaned) <= cap else "   <-- ABOVE ITS WATERMARK"
    print("  %-9s %8d %8d %8d %d/%d entries in %d undeclared famil(y/ies)%s" % (
        lang, len(decl), len(orphaned), cap, undecl_entries, total, len(undecl_fams), flag))
    if len(orphaned) > cap:
        bad += 1
        for f in orphaned[:5]:
            print("      orphaned key: %s" % f)
        if len(orphaned) > 5:
            print("      ... and %d more" % (len(orphaned) - 5))
if not graded:
    sys.stderr.write("REFUSED: ARM E graded ZERO languages -- no ALL.csv found under %s\n" % tests)
    raise SystemExit(2)
print("GRADED=%d BAD=%d" % (graded, bad))
EOF
)"; orc=$?
printf '%s\n' "$orph_out"
if [ "$orc" -eq 2 ] || ! printf '%s' "$orph_out" | grep -q '^GRADED='; then
    echo "GATE REFUSES: ARM E could not measure the declaration/entry key agreement (rc=$orc)"
    exit 2
elif [ "$(printf '%s' "$orph_out" | sed -n 's/^GRADED=[0-9]* BAD=//p')" != "0" ]; then
    echo "GATE FAIL: a language's ORPHANED declaration count rose above its watermark -- a declared family"
    echo "           that matches no entry silently hands every entry the UNKNOWN default instead"
    violations=$((violations + 1))
fi

GATE_EXAMINED="$examined arms across $(wc -l < "$PLAN") language(s), $langs_with_ast declaring ast"
gate_verdict "$violations" "suites whose modes declaration does not travel with them"
