#!/usr/bin/env bash
# test_gate_icon_board_honours_modes_column.sh -- THE BOARD GRADES EACH ENTRY THE WAY ITS modes COLUMN SAYS.
# Row board-icon-master-runs-the-ast-graded-parser-fixtures-and-counts-their-inevitable-reds (ceo mint 2026-09-03).
#
# THE DEFECT: 153 of the icon master's 534 entries are parser-ladder fixtures whose .ref is a `--dump-ast` DUMP,
# and the runner graded them by RUNNING them. Their reds were inevitable and meant nothing. The printed 398/534
# ⛔ WAS NOT A COUNT OF ANYTHING -- it mixed two populations graded against two different kinds of expected
# output, so no reading of it was available: neither "how much Icon works" nor "how much of the parser ladder
# parses". A denominator spanning two grading regimes cannot be repaired by moving the numerator.
#
# WHAT IT PROVES, on a HERMETIC two-entry suite built under mktemp (never the real corpus):
#   (a) an ast-graded entry whose --dump-ast output MATCHES its ref reads PASS -- *while its RUN output differs
#       from that ref*. This is the row's fail-once shape and the heart of the cure: under the old runner this
#       same entry was a FAIL, and it was a FAIL for a reason that said nothing about the entry.
#   (b) an ast-graded entry whose dump DIFFERS reads FAIL. Without this, (a) would pass against a runner that
#       had simply stopped grading the ast population at all -- green by not looking.
#   (c) the two populations are printed as SEPARATE boards with their OWN denominators, and never summed.
#   (d) REFUSES (non-zero) when the sibling ALL.csv is missing or does not cover every entry: a column that
#       cannot be read is not a column that can be honoured, and defaulting silently is the original defect.
# ⛔ The fixtures are minted here rather than borrowed from corpus/ so the gate cannot go green because the real
# tree happened to change -- and so arm (a) can GUARANTEE run-output != ast-output, which no real entry promises.
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP:-$ROOT/scrip}"; SUT="${SUT:-$HERE/corpus_suite_harness.py}"
refuse(){ echo "⛔ REFUSED-TO-GRADE: $*"; exit 2; }
[ -x "$SCRIP" ] || refuse "scrip not built at $SCRIP"
[ -f "$SUT" ]   || refuse "harness under test not found: $SUT"
W="$(mktemp -d "${TMPDIR:-/tmp}/gate_modescol.XXXXXX")" || refuse "mktemp failed"
trap 'rm -rf "$W"' EXIT
S="$W/ALL.icn"; R="$W/ALL.ref"; C="$W/ALL.csv"
cat > "$W/probe.icn" <<'ICN'
procedure main()
  write("RUNTIME_OUTPUT")
end
ICN
AST="$(timeout 60 "$SCRIP" --dump-ast "$W/probe.icn" 2>/dev/null)" || refuse "scrip --dump-ast failed on the probe"
[ -n "$AST" ] || refuse "scrip --dump-ast produced nothing -- cannot mint an ast-graded fixture"
grep -q 'RUNTIME_OUTPUT' <<<"$AST" && ! grep -qx 'RUNTIME_OUTPUT' <<<"$AST" || true
{ echo "#---------------------------------------------------------------- 1 ast_match"
  cat "$W/probe.icn"
  echo "#---------------------------------------------------------------- 2 ast_differ"
  cat "$W/probe.icn"; } > "$S"
{ echo "#---------------------------------------------------------------- 1 ast_match"
  printf '%s\n' "$AST"
  echo "#---------------------------------------------------------------- 2 ast_differ"
  echo "THIS_IS_NOT_THE_DUMP"; } > "$R"
{ echo "rank,entry,origin,family,kind,xfail,n_lines,modes"
  echo "1,ast_match,g__ast_match,g,block,0,3,ast"
  echo "2,ast_differ,g__ast_differ,g,block,0,3,ast"; } > "$C"
fails=0; checks=0
ck(){ checks=$((checks+1)); if [ "$1" = ok ]; then printf '  ok    %s\n' "$2"; else printf '  FAIL  %s\n' "$2"; fails=$((fails+1)); fi; }
echo "=== gate: the board honours the modes column ==="
out="$(SUITE_LIST_ALL=1 timeout 300 python3 "$SUT" run "$S" "$R" --lang icon --modes m3,m4 --by-modes-column 2>&1)"
echo "--- (a)/(b) ast population graded by --dump-ast ---"
grep -q 'SUITE_BOARD_AST' <<<"$out" && ck ok "an ast board is printed" || ck no "no SUITE_BOARD_AST line -- the ast population was not graded separately"
grep -qE 'SUITE_BOARD_AST[^\n]*total=2'      <<<"$out" && ck ok "(c) ast board carries its OWN denominator (total=2)" || ck no "(c) ast board must carry the ast denominator"
grep -qE 'SUITE_BOARD_AST[^\n]*ast_pass=1'   <<<"$out" && ck ok "(a) the entry whose DUMP matches reads PASS (its RUN output differs from that ref)" || ck no "(a) dump-matching entry must PASS -- under the old runner it was a meaningless FAIL"
grep -qE 'SUITE_BOARD_AST[^\n]*ast_fail=1'   <<<"$out" && ck ok "(b) the entry whose DUMP differs reads FAIL" || ck no "(b) dump-differing entry must FAIL -- else the cure is green-by-not-looking"
grep -qE '^MODES_COLUMN .*ast_graded=2/2'    <<<"$out" && ck ok "(c) MODES_COLUMN states the split and the suite size" || ck no "(c) MODES_COLUMN must state ast_graded/total"
grep -qE '^SUITE_BOARD .*total=0'            <<<"$out" && ck ok "(c) run board denominator is the RUN population (0 here), not the suite" || ck no "(c) run board must count only run-graded entries"
echo "--- (d) an unreadable column REFUSES ---"
mv "$C" "$C.hidden"
out2="$(timeout 300 python3 "$SUT" run "$S" "$R" --lang icon --modes m3,m4 --by-modes-column 2>&1)"; rc2=$?
[ "$rc2" -ne 0 ] && ck ok "missing ALL.csv -> non-zero (got $rc2)" || ck no "missing ALL.csv must not be graded silently, got rc=0"
grep -qi 'REFUS' <<<"$out2" && ck ok "missing ALL.csv says REFUSING" || ck no "missing ALL.csv must REFUSE in words"
printf 'rank,entry,origin,family,kind,xfail,n_lines,modes\n1,ast_match,g__ast_match,g,block,0,3,ast\n' > "$C"
out3="$(timeout 300 python3 "$SUT" run "$S" "$R" --lang icon --modes m3,m4 --by-modes-column 2>&1)"; rc3=$?
[ "$rc3" -ne 0 ] && ck ok "partial ALL.csv (1 of 2 entries) -> non-zero (got $rc3)" || ck no "a column covering only some entries must REFUSE, got rc=0"
grep -qi 'REFUS' <<<"$out3" && ck ok "partial coverage says REFUSING" || ck no "partial coverage must REFUSE in words"
echo "------------------------------------------------------------"
[ "$fails" -ne 0 ] && { echo "⛔ GATE FAIL: $fails of $checks check(s) failed"; exit 1; }
echo "✅ GATE PASS: $checks/$checks checks"; exit 0
