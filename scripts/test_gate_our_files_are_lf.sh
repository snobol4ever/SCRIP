#!/usr/bin/env bash
# test_gate_our_files_are_lf.sh -- OUR FILES ARE LF (Lon 2026-09-03, in-chat to hq_P, verbatim: "Do not use CRLF, use LF.";
# 2026-09-04, in-chat to ceo, verbatim: "Fix the CRLF to LF problem. Why am I still hearing about that?").
#
# POPULATION: every git-tracked file in the sibling repos SCRIP, corpus and .github, MINUS corpus/packages/ (third-party
# vendor fixtures: their CRLF is the oracle's own input format -- FINDING-2026-08-20-s183 measured that curing gimpel's
# CRLF LOWERS its score; they convert only on Lon's word, in a commit of their own) and MINUS generated flex/bison outputs.
# WHAT IT COUNTS: a CR at END OF LINE (\r$). A CR byte inside a string literal (benchmarks/icon/geddump.s carries
# "\t\n\r " as DATA in a .string directive) is not a line ending and is not counted.
# WHY IT EXISTS: Python's csv.writer defaults lineterminator to "\r\n", so util_build_master_suite.py re-minted every
# ALL.csv as CRLF on each rebuild; tests/prolog/ALL.csv was converted by hand (corpus 1feca4aa4) and the other five were
# not, and a seat then RESTORED snobol4's CRLF (corpus 40441ed53) applying the binary-read rule with no law to read
# against -- RULES.md carried no LF rule until 2026-09-04. A rule with no instrument is a memo; this is the instrument.
# rc=0 clean (population printed) / rc=1 red (every offender named) / rc=2 cannot measure (never a silent zero).
# FAIL-ONCE PROOF: LF_GATE_REPOS="<dir> ..." overrides the repo list; point it at a scratch git repo holding one
# CRLF file and the gate must print RED rc=1, then LF the file and it must print PASS rc=0.
set -u
GATE_NAME=test_gate_our_files_are_lf
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="${S4E_HOME:-$(cd "$HERE/../.." && pwd)}"
REPOS="${LF_GATE_REPOS:-$ROOT/SCRIP $ROOT/corpus $ROOT/.github}"
refuse() { echo "⛔ REFUSED(2) [$GATE_NAME]: $*" >&2; exit 2; }
command -v git >/dev/null 2>&1 || refuse "git not on PATH"
total=0; bad=0; badlist=""
for r in $REPOS; do
    [ -d "$r/.git" ] || refuse "$r is not a git repo -- a census that cannot see its population is not a zero"
    list="$(git -C "$r" ls-files | grep -v '^packages/' | grep -vE '\.tab\.[ch]$|lex\.yy\.c$|\.yy\.c$')" || true
    n="$(printf '%s\n' "$list" | grep -c .)"
    [ "$n" -gt 0 ] || refuse "$r: git ls-files listed nothing after the exclusions"
    hits="$(cd "$r" && printf '%s\n' "$list" | xargs -d '\n' grep -lI $'\r$' -- 2>/dev/null)" || true
    if [ -n "$hits" ]; then
        while IFS= read -r h; do [ -n "$h" ] || continue; bad=$((bad+1)); badlist="$badlist
    $r/$h"; done <<< "$hits"
    fi
    total=$((total+n))
done
echo "LF_CENSUS repos=$(printf '%s\n' $REPOS | wc -l) tracked_text_files=$total crlf_files=$bad (corpus/packages/ and generated flex/bison outputs excluded)"
if [ "$bad" -gt 0 ]; then
    echo "⛔ GATE RED [$GATE_NAME]: $bad tracked file(s) carry CR at end of line -- OUR FILES ARE LF (RULES.md FACT RULE, Lon 2026-09-03/04). Convert each in a commit of its own (prove: HEAD's bytes with CRs stripped == new bytes):$badlist"
    exit 1
fi
echo "GATE PASS(0) [$GATE_NAME]: $total tracked files, 0 with CRLF"
exit 0
