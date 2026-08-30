#!/usr/bin/env bash
# test_raku_parser_fixtures.sh
#
# Grades the "parser*" families (ast mode) inside corpus/tests/raku/ALL.raku / .ref. Originally
# pointed at a standalone corpus/tests/raku/parser.raku / .ref pair; that pair was absorbed into
# the ONE-FLAT-SUITE master and deleted (seat06, 2026-08-30, ceo all-hands raku consolidation)
# once independently verified byte-equal against it. ALL.raku now
# mixes ast-mode parser fixtures with m3/m4-mode program tests (scrip_test absorption, same
# session), so this gate can no longer just hand the whole master to `run --lang raku --modes
# ast` -- that would grade the non-ast entries in the wrong mode and manufacture false FAILs.
# Extracts the ast-mode "parser*" family subset to a scratch pair (via ALL.csv's family column,
# never by name-pattern-guessing) and grades THAT, exactly reproducing this gate's original scope:
# 83 clean ast_pass entries + the 14 known-bad class/OOP + gather/take stems, now correctly
# carrying their own XFAIL markers (a pre-existing xfail=0 mis-declaration on those 14, inherited
# from the original absorption's plain-mode path never computing xfail, fixed the same session).
#
# Gate: FAIL=0 over the printed total (do not pin a specific total here -- the fixture count
# drifts; a probe asserts FAIL=0/SKIP=0 over its own denominator, never a copied number -- RULES.md).
#
# Commit identity: LCherryholmes / lcherryh@yahoo.com  (RULES.md)
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME

set -uo pipefail   # deliberately NOT -e: this script's own diagnostic must never mask a fail count (see test_snocone_parser_fixtures.sh fix, 2026-08-27)

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
MASTER_SNO="$S4E/corpus/tests/raku/ALL.raku"
MASTER_REF="$S4E/corpus/tests/raku/ALL.ref"
MASTER_CSV="$S4E/corpus/tests/raku/ALL.csv"

echo "=== Raku parser fixtures (ast-mode family subset of the master) ==="

if [ ! -f "$MASTER_SNO" ] || [ ! -f "$MASTER_REF" ] || [ ! -f "$MASTER_CSV" ]; then
    echo "REFUSED: master not found at $MASTER_SNO / $MASTER_REF / $MASTER_CSV -- cannot grade nothing as a pass"
    exit 2
fi

TMP="$(mktemp -d)"
trap 'rm -rf "$TMP"' EXIT

python3 - "$MASTER_SNO" "$MASTER_REF" "$MASTER_CSV" "$TMP" "$HERE" <<'PYEOF'
import sys, csv

master_sno, master_ref, master_csv, tmpdir, scripts_dir = sys.argv[1:6]
sys.path.insert(0, scripts_dir)
import corpus_suite_harness as h
family_by_name = {}
with open(master_csv, newline="") as f:
    for row in csv.DictReader(f):
        if row["family"] == "parser" or row["family"].startswith("parser_"):
            family_by_name[row["entry"]] = row["family"]

bre = h.banner_re_for("#", "")
entries = h.read_block_suite(master_sno, master_ref, bre)
subset = [e for e in entries if e.name in family_by_name]
if not subset:
    print("REFUSED: zero parser-family entries found in the master -- cannot grade nothing as a pass")
    sys.exit(2)
h.write_block_suite(subset, tmpdir + "/parser.raku", tmpdir + "/parser.ref", "#", "")
print("extracted %d parser-family entries for grading" % len(subset), file=sys.stderr)
PYEOF
extract_rc=$?
if [ "$extract_rc" -ne 0 ]; then
    exit "$extract_rc"
fi

out=$(python3 "$HERE/corpus_suite_harness.py" run "$TMP/parser.raku" "$TMP/parser.ref" --lang raku 2>&1)
rc=$?
echo "$out"
[ "$rc" -eq 0 ]
