#!/usr/bin/env bash
# test_gate_ast_shaped_refs_are_declared_ast.sh — THE CONVERSE DIRECTION: A REF THAT *IS* A DUMP MUST BE
# DECLARED `modes=ast`. AUTHORS: Lon Jones Cherryholmes · Claude Opus 5   DATE: 2026-09-05  (hq_T, row
# snobol4-modes-gate-population-derives-from-the-column-it-protects.)
#
# ⛔⭐⭐ THE DEFECT THIS EXISTS TO KILL IS A GATE GOING QUIET, NOT A SUITE GOING RED.
# test_gate_modes_declaration_travels.sh picks its population as "the smallest family whose entries are ALL
# modes=ast", read off each master's own ALL.csv. So when the master builder wiped the `modes` column, snobol4
# went to ZERO all-ast families and that gate simply had nothing to look at: ARM B never ran, ARM C printed
# "no language declares an all-ast family -- ARM C has nothing to grade (not a failure)", and the gate PASSED
# in exactly the language where the field it protects had just broken. Measured on corpus tests/snobol4 at the
# time: 460 families, ZERO all-ast, modes totals UNKNOWN=1797 vs m3,m4=62, and the master board printed
# `master-ast total=0 pass=0 FAIL=0` -- an empty population wearing the success shape.
#
# ⭐⭐ THE GENERAL FORM, which is why this is a separate gate and not a patch to that one:
# A GATE WHOSE POPULATION IS DERIVED FROM THE FIELD IT PROTECTS GOES SILENT PRECISELY WHEN THAT FIELD BREAKS.
# Zero rows to check reads as "nothing to check", never as "the column is gone". Same family as an empty
# denominator printing the success shape, and as a criterion that grades rc alone with its witness output sent
# to /dev/null. The cure is an arm keyed on something the corruption CANNOT MOVE -- here, the REF SHAPE, which
# lives in ALL.ref and is untouched by any edit to ALL.csv.
#
# ⛔ WHY A THIRD GATE RATHER THAN AN ARM ON EITHER SIBLING: the filename is the claim, and these are three
# genuinely different claims about one field.
#   test_gate_modes_declaration_travels          -- the field TRAVELS with an extraction        (plumbing)
#   test_gate_ast_declared_refs_are_ast_dumps    -- declared ast  => the ref really is a dump   (direction A)
#   this gate                                    -- the ref is a dump => it is declared ast     (direction B)
# Direction A is the one that goes vacuous when the column is wiped: zero declared entries, nothing to check,
# green. Direction B cannot, because its population comes from the refs. Folding B into A's file would put a
# claim under a name that denies it.
#
# ⛔ THE SHAPE TEST HERE IS DELIBERATELY STRICTER THAN THE SIBLING'S, AND THE ASYMMETRY IS THE POINT.
# Direction A asks "is this declared dump shaped like one?" and a loose test (first non-blank line opens with
# `(`) is right: it is looking for refs that are obviously NOT dumps. Direction B asks the accusing question
# -- "this ref IS a dump, so the column is wrong" -- where a false positive REDS A CORRECT TREE over a program
# that legitimately prints a line starting with `(`. So a ref counts as a dump here only if it opens with `(`
# AND carries a structural marker no ordinary program output does: `(STMT` / `(PROGRAM` at the head, or a
# `:subj` / `:lhs` attribute anywhere. Tightening the accuser and loosening the auditor is not an
# inconsistency to be unified away; the two gates ask opposite questions and pay opposite costs for a mistake.
#
# WHAT IT ASSERTS, over every master carrying an ALL.csv, in every language:
#   (1) every entry whose .ref is dump-shaped is declared modes=ast -- named individually when not;
#   (2) ⭐ THE ANTI-SILENCE ARM: a language that HAS dump-shaped refs must declare at least one ast entry, so
#       a wiped column reds HERE instead of quietly emptying every population downstream.
# It prints its denominator per language and in total, and REFUSES rc=2 rather than printing a zero it cannot
# stand behind: an unreadable master, a missing harness, or zero masters examined.
#
# rc=0 clean · rc=1 a dump-shaped ref is not declared ast, or a language with dumps declares none · rc=2 REFUSAL.
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
cd "$ROOT"
. "$HERE/lib_gate.sh" 2>/dev/null || { echo "GATE UNPROVEN(2): cannot load lib_gate.sh"; exit 2; }
gate_parse_args "$@"

S4E="${S4E_HOME:-$(cd "$ROOT/.." && pwd)}"
HARNESS="$HERE/corpus_suite_harness.py"
TESTS="$S4E/corpus/tests"
gate_require "$HARNESS" "corpus_suite_harness.py, the ONE grading authority"
gate_require "$TESTS" "the per-language master suites"

echo "--- every dump-shaped ref is declared modes=ast (population = the REFS, never the column) ---"
python3 - "$HERE" "$TESTS" <<'PY'
import os, sys
sys.path.insert(0, sys.argv[1])
import corpus_suite_harness as m
tests = sys.argv[2]
LANGS = dict({l: c["ext"] for l, c in m.LANG_CONFIGS.items()}, snobol4=".sno")

def is_dump(ref):
    """⛔ THE STRICT (ACCUSING) PREDICATE -- see the header. Opens with '(' AND carries a marker ordinary
    program output does not, so a program that merely prints a parenthesised first line is never accused."""
    txt = ref if isinstance(ref, str) else "\n".join(ref)
    txt = txt.strip()
    if not txt.startswith("("):
        return False
    return txt.startswith("(STMT") or txt.startswith("(PROGRAM") or ":subj" in txt or ":lhs" in txt

graded = 0
total_entries = 0
total_dumps = 0
violations = 0
for lang in sorted(LANGS):
    d = os.path.join(tests, lang)
    sp = os.path.join(d, "ALL" + LANGS[lang])
    rp = os.path.join(d, "ALL.ref")
    if not (os.path.isfile(sp) and os.path.isfile(rp)):
        continue
    try:
        if lang == "snobol4":
            ents = m.read_suite(sp, rp)
        else:
            cfg = m.LANG_CONFIGS[lang]
            ents = m.read_block_suite(sp, rp, m.banner_re_for(cfg["comment_open"], cfg["comment_close"]))
    except Exception as e:
        # ⛔ AN UNREADABLE MASTER IS A REFUSAL, NEVER A CLEAN LANGUAGE -- it is exactly the state in which a
        # wrong declaration is most likely, so reading nothing must never be allowed to print zero.
        sys.stderr.write("REFUSED(2): %s: cannot read its master pair (%s)\n" % (lang, str(e)[:140]))
        raise SystemExit(2)
    decl, _csv = m.modes_declarations(sp)
    if decl is None:
        decl = {}
    graded += 1
    total_entries += len(ents)
    dumps = [e for e in ents if is_dump(e.ref)]
    declared = [e for e in ents if (decl.get(e.name) or "").strip() == "ast"]
    total_dumps += len(dumps)
    undeclared = [e.name for e in dumps if (decl.get(e.name) or "").strip() != "ast"]
    flag = "ok"
    if undeclared:
        flag = "⛔"
    elif dumps and not declared:
        flag = "⛔"
    print("   %-8s %5d entries : %4d dump-shaped ref(s), %4d declared ast, %3d undeclared  %s"
          % (lang, len(ents), len(dumps), len(declared), len(undeclared), flag))
    for n in undeclared[:8]:
        print("        dump-shaped ref NOT declared modes=ast: %s" % n)
    if len(undeclared) > 8:
        print("        ... and %d more" % (len(undeclared) - 8))
    if undeclared:
        violations += len(undeclared)
    # ⭐ THE ANTI-SILENCE ARM. Reached only when the column is so empty that nothing above could accuse.
    if dumps and not declared:
        print("        ⛔ %s has %d dump-shaped ref(s) and declares ZERO modes=ast entries -- the column is"
              % (lang, len(dumps)))
        print("           gone, and every population derived from it is now silently empty, not clean")
        violations += 1

if graded == 0:
    sys.stderr.write("REFUSED(2): no master suite with a readable ALL pair under %s -- nothing measured\n" % tests)
    raise SystemExit(2)
print("   ---")
print("   DENOMINATOR: %d language master(s), %d entries, %d dump-shaped ref(s) examined"
      % (graded, total_entries, total_dumps))
if violations:
    print("⛔ GATE FAIL [ast_shaped_refs_are_declared_ast]: %d violation(s) over %d master(s)" % (violations, graded))
    raise SystemExit(1)
print("✅ GATE PASS [ast_shaped_refs_are_declared_ast]: every dump-shaped ref is declared modes=ast (%d master(s), %d dumps)"
      % (graded, total_dumps))
PY
exit $?
