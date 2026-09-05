#!/usr/bin/env bash
# test_gate_ast_declared_refs_are_ast_dumps.sh — A `modes=ast` DECLARATION IS NOT EVIDENCE. THE REF MUST BE A DUMP.
# AUTHORS: Lon Jones Cherryholmes · Claude Opus 5   DATE: 2026-09-05  (hq_T, row test-suite-consistency-seven-
# languages-one-standard; answers seat12's question of why test_gate_modes_declaration_travels.sh did not flag this.)
#
# ⛔⭐⭐ THE MEASURED DEFECT. The prolog master declares its 134-entry `parser` family `modes=ast`, so the harness grades
# every one of them with `scrip --dump-ast`. 133 of their refs were EMPTY and the 134th held `starting` -- the program's
# RUNTIME output, captured by running an entry its own family says is never run. The dumps are real and deterministic
# (134/134 rc=0, byte-identical over two runs, every one starting `(STMT`), so all 134 failed BY CONSTRUCTION: a dump
# diffed against nothing. That is 20% of the prolog master, ast_fail=134/134, on every board since the absorption.
# ⭐ HOW THE REFS WENT MISSING: the fixtures used to live as their own suite pair, corpus/tests/prolog/parser.{pl,ref},
# and the absorption into the master carried the SOURCES and left the REFS behind. The dedicated gate that owned them,
# test_prolog_parser_fixtures.sh, then printed "SKIP suite not found" and exited 0 -- so the population went dark and
# its own instrument reported success, for over a week.
#
# ⛔⭐ WHY THE NEIGHBOURING GATE COULD NOT SEE IT, and the distinction worth keeping: test_gate_modes_declaration_travels
# asserts that the `modes` field TRAVELS with the suite -- a plumbing property. These 134 entries carried their
# declaration perfectly. Nothing asserted the declaration was TRUE OF THE DATA. A field that is present, correct in
# form, and describes something that is not there passes every check written about the field.
#
# WHAT THIS ASSERTS, over every master that declares any ast entry:
#   (1) an ast-declared entry's ref is NON-EMPTY -- nothing legitimately dumps to nothing, and the two known bare-term
#       cases (`42.`, `X.`) were deliberately left OUT of a suite for exactly this reason (their KEEP.md says so);
#   (2) it is SHAPED like a dump -- first non-blank line opens with `(` -- so a ref captured by RUNNING the entry
#       (`starting`) is caught as the same defect wearing different clothes.
# ⛔ It does NOT re-derive the dumps: that is the per-language fixture gate's job, needs a build, and a check that
# duplicates another's work disagrees with it eventually. This one is a pure file parse, no build, no program run.
#
# rc=0 clean · rc=1 an ast-declared ref is empty or is not a dump · rc=2 REFUSAL (cannot measure).
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

out="$(python3 - "$HERE" "$TESTS" <<'PY'
import os, sys
sys.path.insert(0, sys.argv[1])
import corpus_suite_harness as m
tests = sys.argv[2]
LANGS = dict({l: c["ext"] for l, c in m.LANG_CONFIGS.items()}, snobol4=".sno")
graded = bad = 0
langs_with_ast = 0
for lang in sorted(LANGS):
    d = os.path.join(tests, lang)
    sp = os.path.join(d, "ALL" + LANGS[lang])
    rp = os.path.join(d, "ALL.ref")
    if not (os.path.isfile(sp) and os.path.isfile(rp)):
        continue
    decl, _csv = m.modes_declarations(sp)
    if not decl:
        print("   %-8s no modes declaration -- not measured here (that is the travels gate's arm)" % lang)
        continue
    try:
        if lang == "snobol4":
            ents = m.read_suite(sp, rp)
        else:
            cfg = m.LANG_CONFIGS[lang]
            ents = m.read_block_suite(sp, rp, m.banner_re_for(cfg["comment_open"], cfg["comment_close"]))
    except Exception as e:
        # ⛔ A MASTER THIS CANNOT READ IS A REFUSAL, NEVER A CLEAN LANGUAGE. An unreadable pair is exactly the state
        # in which a wrong ref is most likely, so reading nothing must not be allowed to print zero.
        sys.stderr.write("REFUSED: %s: cannot read its master pair (%s)\n" % (lang, str(e)[:120]))
        raise SystemExit(2)
    ast = [e for e in ents if decl.get(e.name) == "ast"]
    graded += 1
    if not ast:
        print("   %-8s %5d entries, 0 declared ast" % (lang, len(ents)))
        continue
    langs_with_ast += 1
    empty = [e.name for e in ast if not "".join(e.ref).strip()]
    shape = [e.name for e in ast
             if "".join(e.ref).strip()
             and not next(l for l in ("\n".join(e.ref)).split("\n") if l.strip()).lstrip().startswith("(")]
    flag = "ok" if not (empty or shape) else "⛔"
    print("   %-8s %5d entries, %4d declared ast : %3d EMPTY ref, %3d not dump-shaped  %s"
          % (lang, len(ents), len(ast), len(empty), len(shape), flag))
    for n in empty[:5]:
        print("        EMPTY ref, graded by --dump-ast against nothing: %s" % n)
    if len(empty) > 5:
        print("        ... and %d more with an empty ref" % (len(empty) - 5))
    for n in shape[:5]:
        print("        ref is not a dump (captured by RUNNING it?): %s" % n)
    if len(shape) > 5:
        print("        ... and %d more not dump-shaped" % (len(shape) - 5))
    bad += len(empty) + len(shape)
if not graded:
    sys.stderr.write("REFUSED: graded ZERO languages -- no master with a modes declaration under %s\n" % tests)
    raise SystemExit(2)
print("GRADED=%d LANGS_WITH_AST=%d BAD=%d" % (graded, langs_with_ast, bad))
PY
)"; rc=$?
printf '%s\n' "$out"
if [ "$rc" -eq 2 ] || ! printf '%s' "$out" | grep -q '^GRADED='; then
    echo "⛔ GATE REFUSES: could not measure the ast-declared refs (rc=$rc)"
    echo "   REFUSAL cause=ast-ref-census-rc-$rc signal=none"
    exit 2
fi
violations=0
BADN="$(printf '%s' "$out" | sed -n 's/^GRADED=[0-9]* LANGS_WITH_AST=[0-9]* BAD=//p')"
if [ "$BADN" != "0" ]; then
    echo "⛔ GATE FAIL: $BADN ast-declared entr(y/ies) whose .ref is not a --dump-ast dump."
    echo "   Each one is graded by --dump-ast against a ref it can never match, so it is a red that means nothing"
    echo "   and it survives every check written about the modes FIELD -- the field is correct; the data is not."
    echo "   CURE: cut the refs from --dump-ast (they are self-pins; no oracle emits SCRIP's AST), or correct the"
    echo "   family's declaration in tests/<lang>/config/MODES.tsv if those entries are meant to be RUN."
    violations=1
fi
GATE_EXAMINED="$(printf '%s' "$out" | sed -n 's/^GRADED=\([0-9]*\).*/\1/p') master(s), $(printf '%s' "$out" | sed -n 's/.*LANGS_WITH_AST=\([0-9]*\).*/\1/p') declaring ast entries"
gate_verdict "$violations" "masters whose ast-declared refs are not dumps"
