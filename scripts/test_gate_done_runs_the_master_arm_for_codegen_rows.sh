#!/usr/bin/env bash
# test_gate_done_runs_the_master_arm_for_codegen_rows.sh -- `s4e_msg.sh done` carries the master as a CONTROL ARM on
# any landing whose DIFF touches codegen, and it CITES a board of that exact tree rather than running one.
#
# THE INCIDENT (ceo, CEO-342 rule 4, row done-runs-the-snobol4-master-arm-itself-...): seat11's DEFINE landing
# d067ceae4 turned 299 SNOBOL4 master entries red AND ITS DONE-WHEN PASSED, because "a codegen landing carries the
# master as its control arm" lived in prose and `done` could not see it. Three hours of eight HQs went to the
# bisect, the revert and the re-measure. A law the tooling does not enforce is a hope.
#
# ⛔⭐ CITE, NOT RUN, AND THAT IS A RULING (ceo CEO-697 2026-09-13; MODE line 2 under NONET, "ONE RUNNER, ONE
# BOARD"). `done` runs in EVERY seat, so a `done` that auto-ran a master board would MECHANISE the very contention
# CEO-697 was issued to stop -- thirteen seats firing a board per codegen landing. The row's own GOAL offered both
# branches ("test_corpus_snobol4.sh, OR the one-board-per-tree CITED board of the same tree") and CEO-697 picks one.
#
# EIGHT ARMS against a THROWAWAY postoffice, a THROWAWAY git tree and a THROWAWAY progress database under mktemp --
# never the live ones. Hermetic and seconds-cheap: it builds nothing, runs no suite, and reads no live board.
#   (1) NON-CODEGEN landing            -> no control arm at all, row closes
#   (2) CODEGEN + cited CLEAN board    -> closes, and the receipt says the board was CITED and names the tree
#   (3) CODEGEN + cited board RED      -> REFUSES rc=1 and NAMES the failing entries, row NOT closed
#   (4) same red board, reds == INHERITED -> closes (the inherited set is honoured)
#   (5) CODEGEN + NO board of this tree-> closes, but says COULD NOT MEASURE and RECORDS the landing
#   (6) S4E_DONE_SKIP_MASTER_ARM=why   -> closes, why echoed AND written to the baton ledger
#   (7) ANTI-ASSERT: the touch set is COMPUTED FROM THE DIFF -- a baton whose PROSE says codegen but whose diff
#       touches nothing gets no arm, and a diff that touches codegen gets the arm with nothing declaring it
#   (8) INHERITED is READ LIVE from the baton -- editing that one line flips the verdict with no code change
# FAIL-ONCE: every arm is re-run against MUTANTS of s4e_msg.sh with one half of the cure removed, and each mutant
# must go RED. An arm that cannot fail is not counted (cto's THE UNEXPLAINED GREEN, corollary 1).
# EXIT 0 all arms hold and every mutant reds; 1 an arm is red; 2 REFUSED (the fixture could not be built).
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; MSG="$HERE/s4e_msg.sh"
ROOT="$(cd "$HERE/../.." && pwd)"
[ -f "$MSG" ] || { echo "⛔ REFUSED(2): $MSG missing"; exit 2; }
command -v git >/dev/null 2>&1 || { echo "⛔ REFUSED(2): no git -- the touch set IS a git property"; exit 2; }
W="$(mktemp -d)" || { echo "⛔ REFUSED(2): mktemp failed"; exit 2; }; trap 'rm -rf "$W"' EXIT
ME=hq_B; PO="$W/po"; HOME_T="$W/home"; DB="$W/results.tsv"
SCRIP_SHA=""; CORPUS_SHA=""
# ---- the scratch sibling root: a real git tree, because the cure reads a real diff ------------------------------
mk_tree() {
  rm -rf "$HOME_T"; mkdir -p "$HOME_T/SCRIP/src/ir" "$HOME_T/SCRIP/src/templates" "$HOME_T/SCRIP/scripts" "$HOME_T/corpus/tests" || return 2
  local r
  for r in SCRIP corpus; do
    ( cd "$HOME_T/$r" && git init -q . && git config user.email g@g && git config user.name g \
      && echo seed > seed.txt && git add -A && git commit -qm seed ) >/dev/null 2>&1 || return 2
  done
  echo 'int x;' > "$HOME_T/SCRIP/src/ir/IR.h"
  ( cd "$HOME_T/SCRIP" && git add -A && git commit -qm base ) >/dev/null 2>&1 || return 2
  SCRIP_SHA="$(git -C "$HOME_T/SCRIP" rev-parse --short HEAD)"; CORPUS_SHA="$(git -C "$HOME_T/corpus" rev-parse --short HEAD)"
  [ -n "$SCRIP_SHA" ] && [ -n "$CORPUS_SHA" ] || return 2
}
# dirty_codegen / dirty_other -- make the WORKING TREE the thing that decides, which is the point of arm 7
dirty_codegen() { echo '/* touched */' >> "$HOME_T/SCRIP/src/ir/IR.h"; }
# ⛔⭐ dirty_other MUST LEAVE THE SCRIP TREE GENUINELY DIRTY, and it asserts that rather than assuming it. The
# first version of this gate wrote into SCRIP/scripts/ AFTER clean_tree's `git clean -fd` had removed that
# untracked directory: the write failed to stderr, the tree stayed CLEAN, and arms 1 and 7 passed because a clean
# tree trivially touches no codegen. They were green about NOTHING -- the exact "fixture that passes for a reason
# the gate does not name" this suite exists to catch, produced while writing the suite. The assert is the cure:
# a fixture that cannot establish its precondition must REFUSE, never quietly hand an arm a free pass.
dirty_other()   { mkdir -p "$HOME_T/SCRIP/scripts" || return 2
  echo 'note' >> "$HOME_T/corpus/tests/notes.txt"; echo 'doc' >> "$HOME_T/SCRIP/scripts/readme.md" || return 2
  git -C "$HOME_T/SCRIP" add -A >/dev/null 2>&1
  [ -n "$(git -C "$HOME_T/SCRIP" diff --name-only HEAD 2>/dev/null)" ] || return 2; }
# dirty_codegen asserts its own precondition for the same reason, in the other direction.
assert_codegen_dirty() { git -C "$HOME_T/SCRIP" diff --name-only HEAD 2>/dev/null | grep -q '^src/' || return 2; }
clean_tree()    { ( cd "$HOME_T/SCRIP" && git checkout -q -- . && git clean -qfd ) >/dev/null 2>&1; }
# ---- the scratch progress database ------------------------------------------------------------------------------
# One row per program, in the live schema: ts scrip corpus measurer class suite lang program mode outcome secs note
db_row() { printf '%s\t%s\t%s\t%s\tmaster\t%s\tsnobol4\t%s\tm4\t%s\t0\t\n' \
            "2026-09-13T10:00:00" "$2" "$3" "coo" "$4" "$5" "$6" >> "$1"; }
mk_db() {   # mk_db <outcome-for-two-entries>  -- "" means NO board of this tree at all
  : > "$DB"; printf 'ts_utc\tscrip\tcorpus\tmeasurer\tclass\tsuite\tlang\tprogram\tmode\toutcome\tsecs\tnote\n' > "$DB"
  [ -z "${1:-}" ] && return 0
  db_row "$DB" "$SCRIP_SHA" "$CORPUS_SHA" snobol4-master alpha_1 PASS
  db_row "$DB" "$SCRIP_SHA" "$CORPUS_SHA" snobol4-master beta_2  "$1"
  db_row "$DB" "$SCRIP_SHA" "$CORPUS_SHA" snobol4-master gamma_3 "$1"
}
# ---- the scratch postoffice -------------------------------------------------------------------------------------
DW='test -f "$S4E_HOME/SCRIP/src/ir/IR.h"'
mk_po() {   # mk_po <topic> [INHERITED-line]
  rm -rf "$PO"; mkdir -p "$PO/tasks" "$PO/claims" "$PO/$ME/inbox" || return 2
  : > "$PO/BOARD.md"; : > "$PO/QUEUE.done.tsv"; printf 'NONET\n# fixture lane line\n' > "$PO/MODE"
  { printf '# gate fixture queue\n'; printf '2\t%s\thq_B\tFREE\n' "$1"; } > "$PO/QUEUE.tsv"
  { printf '# TASK %s\nGOAL: gate fixture.\n' "$1"; [ -n "${2:-}" ] && printf 'INHERITED: %s\n' "$2"
    printf 'DONE-WHEN: %s\nLINKS: none\n## NEXT\n(none)\n## LEDGER\n' "$DW"; } > "$PO/tasks/$1.task.md"
  S4E_POST="$PO" S4E_SEAT="$ME" S4E_HOME="$HOME_T" S4E_NO_BANNER=1 bash "$MSG" claim "$1" >/dev/null 2>&1 || return 2
  [ "$(head -1 "$PO/claims/$1.claim")" = "$ME" ] || return 2
}
RC=0
run() {   # run <script> <topic> [skip-reason]
  if [ -n "${3:-}" ]; then S4E_DONE_SKIP_MASTER_ARM="$3" S4E_PROGRESS_DB="$DB" S4E_POST="$PO" S4E_SEAT="$ME" \
       S4E_HOME="$HOME_T" S4E_NO_BANNER=1 bash "$1" done "$2" >"$W/out" 2>&1
  else S4E_PROGRESS_DB="$DB" S4E_POST="$PO" S4E_SEAT="$ME" S4E_HOME="$HOME_T" S4E_NO_BANNER=1 \
       bash "$1" done "$2" >"$W/out" 2>&1; fi
  RC=$?; }
say() { sed 's/^/      | /' "$W/out" | grep -m4 -E '⛔|⚠|✅|CODEGEN|CITED|COULD NOT' || tail -4 "$W/out" | sed 's/^/      | /'; }
closed() { grep -q '^DONE$' "$PO/claims/$1.claim" 2>/dev/null; }
arm() {   # arm <label> <script> -> 0 iff every contract holds on <script>
  local lbl="$1" s="$2" ok=1
  # (1) NON-CODEGEN -> no arm at all
  mk_tree || return 2; mk_db PASS || return 2; mk_po t1 || return 2; clean_tree; dirty_other || return 2
  git -C "$HOME_T/SCRIP" diff --name-only HEAD | grep -q '^src/' && { echo "  [$lbl] (1) FIXTURE BROKEN: the non-codegen diff touches src/"; return 2; }
  run "$s" t1
  [ "$RC" = 0 ] || { echo "  [$lbl] (1) non-codegen landing exited $RC (want 0)"; say; ok=0; }
  grep -q 'CODEGEN CONTROL ARM' "$W/out" && { echo "  [$lbl] (1) the control arm fired on a diff touching NO codegen"; say; ok=0; }
  closed t1 || { echo "  [$lbl] (1) row not closed"; ok=0; }
  # (2) CODEGEN + clean cited board -> closes, receipt says CITED and names the tree
  mk_tree || return 2; mk_db PASS || return 2; mk_po t2 || return 2; clean_tree; dirty_codegen; assert_codegen_dirty || return 2
  run "$s" t2
  [ "$RC" = 0 ] || { echo "  [$lbl] (2) codegen + clean cited board exited $RC (want 0)"; say; ok=0; }
  grep -q 'CODEGEN CONTROL ARM' "$W/out" || { echo "  [$lbl] (2) the control arm did NOT fire on a codegen diff"; say; ok=0; }
  grep -q 'src/ir/IR.h' "$W/out" || { echo "  [$lbl] (2) the receipt does not NAME the touched codegen path"; say; ok=0; }
  grep -q "scrip=$SCRIP_SHA" "$W/out" || { echo "  [$lbl] (2) the receipt does not name the TREE the board was cited for"; say; ok=0; }
  grep -qE 'CITED board of this tree is clean' "$W/out" || { echo "  [$lbl] (2) no CITED-clean verdict in the receipt"; say; ok=0; }
  closed t2 || { echo "  [$lbl] (2) row not closed"; ok=0; }
  # (3) CODEGEN + red cited board, nothing inherited -> REFUSE, name the entries, do NOT close
  mk_tree || return 2; mk_db FAIL || return 2; mk_po t3 || return 2; clean_tree; dirty_codegen; assert_codegen_dirty || return 2
  run "$s" t3
  [ "$RC" = 1 ] || { echo "  [$lbl] (3) a red cited board exited $RC (want 1 -- measured and not done)"; say; ok=0; }
  grep -q 'beta_2' "$W/out" && grep -q 'gamma_3' "$W/out" || { echo "  [$lbl] (3) the refusal does not NAME the failing entries"; say; ok=0; }
  closed t3 && { echo "  [$lbl] (3) the row was CLOSED despite a red control arm"; ok=0; }
  # (4) same red board, but the reds are exactly INHERITED -> closes
  mk_tree || return 2; mk_db FAIL || return 2; mk_po t4 "beta_2 gamma_3" || return 2; clean_tree; dirty_codegen; assert_codegen_dirty || return 2
  run "$s" t4
  [ "$RC" = 0 ] || { echo "  [$lbl] (4) reds exactly == INHERITED exited $RC (want 0)"; say; ok=0; }
  closed t4 || { echo "  [$lbl] (4) row not closed although every red was inherited"; ok=0; }
  # (8) INHERITED IS READ LIVE: same tree, same board, one entry inherited -> still refuses, naming ONLY the other
  mk_tree || return 2; mk_db FAIL || return 2; mk_po t8 "beta_2" || return 2; clean_tree; dirty_codegen; assert_codegen_dirty || return 2
  run "$s" t8
  [ "$RC" = 1 ] || { echo "  [$lbl] (8) a partially-inherited red board exited $RC (want 1)"; say; ok=0; }
  grep -q 'gamma_3' "$W/out" || { echo "  [$lbl] (8) the refusal does not name the NON-inherited entry"; say; ok=0; }
  awk '/Entries failing/,0' "$W/out" | grep -q 'beta_2' && { echo "  [$lbl] (8) it named an entry the baton DOES inherit"; say; ok=0; }
  # (5) CODEGEN + NO board of this tree -> closes, but COULD NOT MEASURE and RECORDED
  mk_tree || return 2; mk_db "" || return 2; mk_po t5 || return 2; clean_tree; dirty_codegen; assert_codegen_dirty || return 2
  run "$s" t5
  [ "$RC" = 0 ] || { echo "  [$lbl] (5) no citable board exited $RC (want 0 -- cannot-measure must not block the fleet)"; say; ok=0; }
  grep -q 'COULD NOT MEASURE' "$W/out" || { echo "  [$lbl] (5) an unmeasurable arm did not SAY so"; say; ok=0; }
  grep -q 't5' "$PO/codegen-landings.tsv" 2>/dev/null || { echo "  [$lbl] (5) the landing was not RECORDED for the coo stage pass"; say; ok=0; }
  grep -q 'NO CITABLE BOARD' "$PO/tasks/t5.task.md" || { echo "  [$lbl] (5) the baton ledger does not record the unmeasured arm"; ok=0; }
  closed t5 || { echo "  [$lbl] (5) row not closed"; ok=0; }
  # (6) SKIP FLAG -- loud AND recorded
  mk_tree || return 2; mk_db FAIL || return 2; mk_po t6 || return 2; clean_tree; dirty_codegen; assert_codegen_dirty || return 2
  run "$s" t6 "hq_B: fixture reason"
  [ "$RC" = 0 ] || { echo "  [$lbl] (6) the skip flag did not close the row (exited $RC)"; say; ok=0; }
  grep -q 'CONTROL ARM SKIPPED' "$W/out" || { echo "  [$lbl] (6) the skip was SILENT"; say; ok=0; }
  grep -q 'fixture reason' "$W/out" || { echo "  [$lbl] (6) the stated reason was not echoed"; say; ok=0; }
  grep -q 'fixture reason' "$PO/tasks/t6.task.md" || { echo "  [$lbl] (6) the reason was not written to the baton ledger"; ok=0; }
  # (7) ANTI-ASSERT: prose claiming codegen with a clean diff must NOT fire the arm
  mk_tree || return 2; mk_db FAIL || return 2; mk_po t7 || return 2; clean_tree
  printf 'GOAL PROSE: this row rewrites src/templates/ and src/emitter/ and all of codegen.\n' >> "$PO/tasks/t7.task.md"
  dirty_other || return 2
  git -C "$HOME_T/SCRIP" diff --name-only HEAD | grep -q '^src/' && { echo "  [$lbl] (7) FIXTURE BROKEN: the non-codegen diff touches src/"; return 2; }
  run "$s" t7
  [ "$RC" = 0 ] || { echo "  [$lbl] (7) a row whose PROSE names codegen but whose DIFF does not exited $RC (want 0)"; say; ok=0; }
  grep -q 'CODEGEN CONTROL ARM' "$W/out" && { echo "  [$lbl] (7) the arm fired on PROSE -- the touch set is not computed from the diff"; say; ok=0; }
  [ "$ok" = 1 ]
}
echo "== test_gate_done_runs_the_master_arm_for_codegen_rows -- 8 arms, hermetic (scratch po + scratch git tree + scratch db)"
arm LIVE "$MSG"; live=$?
[ "$live" = 2 ] && { echo "⛔ REFUSED(2): the fixture could not be built -- nothing was measured"; exit 2; }
[ "$live" = 0 ] || { echo "⛔ GATE RED: the live s4e_msg.sh does not hold the contract above"; exit 1; }
echo "   ✅ all 8 arms hold on the live s4e_msg.sh"
# ---- FAIL-ONCE: every arm must be capable of going red -----------------------------------------------------------
# ⛔ AN ARM THAT CANNOT FAIL IS NOT COUNTED. Each mutant removes ONE half of the cure; each must red.
mut() {   # mut <name> <sed-program>
  local n="$1" p="$2" m="$W/mut_$1.sh"
  sed "$p" "$MSG" > "$m" || return 2; bash -n "$m" 2>/dev/null || { echo "   [$n] mutant is not valid bash -- not a proof"; return 2; }
  if arm "MUT:$n" "$m" >/dev/null 2>&1; then echo "   ⛔ mutant $n STILL PASSES -- the arms do not discriminate"; return 1
  else echo "   ✅ mutant $n goes red, as it must"; return 0; fi
}
fails=0
mut no-arm        's/^              _cg_repo=.*$/              _cg_touch=""/'                                  || fails=1
mut blind-touch   's/^    printf .%s\\n. "\$files" | sed .\/\^\$\/d. | sort -u | grep -E.*$/    return 1/'      || fails=1
mut deaf-inherit  's/^    sed -n .s\/\^INHERITED:\[\[:space:\]\]\*\/\/p. "\$1".*$/    return 0/'                || fails=1
[ "$fails" = 0 ] || { echo "⛔ GATE RED: at least one arm is unfalsifiable"; exit 1; }
echo "✅ GATE OK -- 8 arms green on the live script, 3 mutants red. Denominator printed; nothing live was touched."
exit 0
