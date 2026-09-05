#!/usr/bin/env bash
# test_gate_s4e_donewhen_runs_the_whole_criterion.sh -- `done` RUNS THE WHOLE DONE-WHEN, NOT ITS FIRST PHYSICAL LINE.
# AUTHORS: Lon Jones Cherryholmes · Claude Opus 5   DATE: 2026-09-05
# Row s4e-msg-donewhen-truncation-false-closes-multiline-heredoc-batons (seat04 reproduced it end to end; ceo ranked
# the cure 0 in hq_T's lane 2026-09-05 14:38).
#
# ⛔⭐⭐ THE DEFECT, AND ITS DIRECTION IS WHAT MAKES IT LETHAL. The ONE extraction point was
# `sed -n 's/^DONE-WHEN:...//p' "$b" | head -1` -- one physical line. For the self-contained heredoc witness this
# project encourages, the captured text is `cat > /tmp/w.sno <<'EOF'` with no closing delimiter: bash WARNS, treats
# the body as EMPTY, writes a zero-byte file, and that cat -- the only command reached -- exits 0. The criterion
# exits 0 HAVING RUN NOTHING, so a broken tree and a fixed one are byte-identical in verdict. ⭐ MEASURED CLOSURE:
# snobol4-pattern-primitive-as-function-argument-always-fails-in-callee closed DONE 2026-09-05T15:33Z through
# exactly this path while its full text exits 1 on the same tree. 185 of 1128 live batons carry a multi-line
# DONE-WHEN, so this is a population, not an anecdote.
#
# FOUR ARMS against a THROWAWAY postoffice under mktemp -- never the live one:
#   (A) TWO-LINE CRITERION, SECOND LINE RED: line 1 passes, line 2 exits 1 -> rc=1 NOT DONE, row NOT closed.
#       This is the ceo's named fail-once shape and the minimal statement of the whole defect.
#   (B) HEREDOC WITNESS: writes a file, then greps it, then exits 1 -> rc=1, AND the file is non-empty, which is
#       what proves the BODY ran rather than the opener alone. ⛔ Arm A alone cannot prove that: a fix that read
#       two lines and stopped would pass A and still truncate every heredoc.
#   (C) UNTERMINATED HEREDOC (an author's mistake, or a future re-truncation) -> REFUSED rc=2, row NOT closed.
#       Never rc=0 and never rc=1: bash cannot finish reading it, so nobody measured anything.
#   (D) MULTI-LINE AND GENUINELY GREEN -> rc=0 and the row CLOSES. Without this the gate would pass on a `done`
#       that had simply stopped closing anything, which is the cheap way to satisfy A/B/C and cure nothing.
# EXIT 0 all four hold on the live script AND each mutant goes red; 1 otherwise; 2 REFUSED (fixture cannot be built).
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; MSG="$HERE/s4e_msg.sh"
ROOT="$(cd "$HERE/../.." && pwd)"
[ -f "$MSG" ] || { echo "⛔ REFUSED: $MSG missing"; exit 2; }
[ -f "$ROOT/SCRIP/scripts/s4e_msg.sh" ] || { echo "⛔ REFUSED: sibling root not resolvable from $HERE (want $ROOT/SCRIP)"; exit 2; }
W="$(mktemp -d)" || { echo "⛔ REFUSED: mktemp failed"; exit 2; }; trap 'rm -rf "$W"' EXIT
PO="$W/po"; ME=hq_T; HD="$W/hd.txt"
# ⛔ EVERY criterion names a path or a `$` ON PURPOSE: done's vacuity probe skips a criterion containing `/` or `$`,
# and without that these fixtures would be refused as decorated no-ops -- a green gate proving something else.
mk_po() {
  rm -rf "$PO" "$HD"; mkdir -p "$PO/tasks" "$PO/claims" "$PO/released" "$PO/$ME/inbox" || return 2
  : > "$PO/BOARD.md"; : > "$PO/QUEUE.done.tsv"; printf "TRIO\n" > "$PO/MODE"
  { printf '# gate fixture queue\n'; for t in t-two t-heredoc t-unterm t-green; do printf '2\t%s\tunassigned\tFREE\n' "$t"; done; } > "$PO/QUEUE.tsv"
  for t in t-two t-heredoc t-unterm t-green; do
    { printf '# TASK %s\nGOAL: gate fixture.\n' "$t"
      case "$t" in
        t-two)     printf 'DONE-WHEN: test -f "$S4E_HOME/SCRIP/scripts/s4e_msg.sh"\n'
                   printf 'test -f /no-such-file-donewhen-whole-criterion-gate || exit 1\n';;
        t-heredoc) printf "DONE-WHEN: cat > %s <<'HEOF'\n" "$HD"
                   printf 'hello-from-the-heredoc-body\nHEOF\n'
                   printf 'grep -q hello-from-the-heredoc-body %s || exit 3\n' "$HD"
                   printf 'test -f /no-such-file-donewhen-whole-criterion-gate || exit 1\n';;
        t-unterm)  printf "DONE-WHEN: cat > %s <<'HEOF'\n" "$W/unterm.txt"
                   printf 'a body whose delimiter never arrives\n';;
        t-green)   printf 'DONE-WHEN: test -f "$S4E_HOME/SCRIP/scripts/s4e_msg.sh"\n'
                   printf 'test -d "$S4E_HOME/SCRIP/scripts"\n';;
      esac
      printf 'LINKS: none\n## NEXT\n(none)\n## LEDGER\n'; } > "$PO/tasks/$t.task.md"
    S4E_POST="$PO" S4E_SEAT="$ME" S4E_HOME="$ROOT" S4E_NO_BANNER=1 bash "$MSG" claim "$t" >/dev/null 2>&1 || return 2
    [ "$(head -1 "$PO/claims/$t.claim")" = "$ME" ] || return 2
  done
}
RC=0
run() { S4E_POST="$PO" S4E_SEAT="$ME" S4E_HOME="$ROOT" S4E_NO_BANNER=1 bash "$1" done "$2" >"$W/out" 2>&1; RC=$?; }
say() { grep -m3 -E '⛔|NOT DONE|REFUSED|✅' "$W/out" | sed 's/^/      | /' || tail -3 "$W/out" | sed 's/^/      | /'; }
closed() { grep -q '^DONE$' "$PO/claims/$1.claim" 2>/dev/null; }
arm() {   # arm <label> <script> -> 0 iff all four contracts hold; 2 iff the fixture broke
  local lbl="$1" s="$2" ok=1
  mk_po || return 2
  run "$s" t-two
  [ "$RC" = 1 ] || { echo "  [$lbl] (A) two-line criterion whose SECOND line exits 1 returned $RC (want 1 -- measured red)"; say; ok=0; }
  closed t-two && { echo "  [$lbl] (A) ⛔ the row CLOSED -- the second line never ran"; ok=0; }
  run "$s" t-heredoc
  [ "$RC" = 1 ] || { echo "  [$lbl] (B) heredoc criterion returned $RC (want 1 -- measured red after the body ran)"; say; ok=0; }
  [ -s "$HD" ] || { echo "  [$lbl] (B) ⛔ the heredoc body did not reach the file -- the opener ran alone"; ok=0; }
  closed t-heredoc && { echo "  [$lbl] (B) ⛔ the row CLOSED on a criterion whose real check exits 1"; ok=0; }
  run "$s" t-unterm
  [ "$RC" = 2 ] || { echo "  [$lbl] (C) unterminated heredoc returned $RC (want 2 -- could not measure)"; say; ok=0; }
  closed t-unterm && { echo "  [$lbl] (C) ⛔ the row CLOSED on a criterion bash could not finish reading"; ok=0; }
  run "$s" t-green
  [ "$RC" = 0 ] || { echo "  [$lbl] (D) a genuinely green multi-line criterion returned $RC (want 0)"; say; ok=0; }
  closed t-green || { echo "  [$lbl] (D) ⛔ a green criterion did NOT close its row -- the cure stopped closing anything"; ok=0; }
  [ "$ok" = 1 ]
}
echo "s4e done: the WHOLE DONE-WHEN runs, a heredoc body reaches its file, an unterminated one REFUSES (scratch postoffice under $W)"
if arm PASS "$MSG"; then echo "  [PASS] (A) second line runs; (B) heredoc body runs; (C) unterminated -> rc=2, row open; (D) green multi-line still closes"; pass=1
else pass=$?; [ "$pass" = 2 ] && { echo "⛔ REFUSED: fixture could not be built (rc=2)"; exit 2; }; pass=0; fi
# FAIL-ONCE, one mutant per half of the cure. M1 restores the head -1 truncation (A and B must red -- the rows close
# on criteria that never ran). M2 removes the unterminated-heredoc refusal (C must red -- rc=0 on an unreadable one).
sed 's|dw="$(s4e_donewhen_text "$tf")"|dw="$(sed -n '"'"'s/^DONE-WHEN:[[:space:]]*//p'"'"' "$tf" \| head -1)"|' "$MSG" > "$W/m1.sh"
sed 's/here-document\.\*delimited by end-of-file/a-warning-string-that-never-appears-90124/' "$MSG" > "$W/m2.sh"
grep -q 'head -1' "$W/m1.sh" || { echo "⛔ REFUSED: could not build mutant M1 (the done-site extraction moved?)"; exit 2; }
grep -q 'never-appears-90124' "$W/m2.sh" || { echo "⛔ REFUSED: could not build mutant M2 (the heredoc guard moved?)"; exit 2; }
red=1
for m in m1 m2; do
  if arm "FAIL-ONCE:$m" "$W/$m.sh" >"$W/fo.$m" 2>&1; then echo "  [FAIL-ONCE:$m] ⛔ STAYED GREEN with that half of the cure removed -- it cannot detect the defect it exists for"; red=0
  else echo "  [FAIL-ONCE:$m] red as required: $(grep -m1 '  \[' "$W/fo.$m" | sed 's/^ *//' | cut -c1-110)"; fi
done
if [ "$pass" = 1 ] && [ "$red" = 1 ]; then echo "✅ GATE OK: done runs the whole criterion, heredoc bodies included, and refuses one it cannot finish reading."; exit 0; fi
echo "⛔ GATE FAILED (pass=$pass fail-once-red=$red)"; exit 1
