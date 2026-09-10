#!/usr/bin/env bash
# test_gate_boards_refuse_when_the_binary_moves.sh -- A BOARD THAT STRADDLES A BINARY SWAP MUST REFUSE rc=2
# BEFORE IT PUBLISHES A NUMBER (ceo CEO-524 (1), to the coo as THE ONE RUNNER, 2026-09-10).
#
# THE WITNESS: hq_R's board straddled a binary swap and wrote m3 635/759 into SCORE.md WITH A CLEAN STAMP,
# while the correct 756/759 from the same sitting was refused as dirty. The dirty guard reads the tree at
# WRITE time; it has nothing to say about the window the board spent grading. Half a population on one build
# and half on another prints A COMPLETE, PLAUSIBLE TABLE -- this house's own recurring shape, an instrument
# that cannot fail printing exactly like one that passed.
#
# ⛔⭐ AND THE CURE ALREADY EXISTED. gate_bin_watch/gate_bin_unmoved have been in lib_gate.sh since
# 2026-09-06, hoisted by hq_S out of test_corpus_snobol4.sh after hq_S rebuilt under a live 1854-entry board.
# Census the day this gate was written: FIVE boards, ZERO callers of gate_bin_watch. Diagnosed, cured,
# hoisted, documented -- and wired to nothing. ⭐ THAT IS WHAT THIS GATE IS ACTUALLY DEFENDING: not the
# existence of the check, which nobody was ever going to delete, but the WIRING, which nobody had ever added.
#
#   ARM 1  the four boards MODE line 2 gives the one runner, plus the donor, each carry BOTH calls
#   ARM 2  ORDERING: the first gate_bin_unmoved precedes the first line that publishes a count. A refusal
#          that fires after the row is published is an annotation, not a refusal (the same lesson
#          test_gate_progress_rows_carry_the_start_fingerprint.sh was written for -- seat10's runner refused
#          at 14:23 a board whose 3736 rows the harness had already appended).
#   ARM 3  the authority REFUSES rc=2 when a watched artifact's bytes change, and names both fingerprints
#   ARM 4  CONTROL: it passes when nothing moves -- without this, arm 3 is satisfied by a check that always refuses
#   ARM 5  it refuses when a watched artifact is DELETED mid-run (the moved case in its loudest form)
#   ARM 6  it refuses AT WATCH TIME when the artifacts cannot be fingerprinted at all, so a board with no
#          baseline discovers that at the start rather than after paying for the whole run
#   ARM 7  PUBLICATION IS PREVENTED, NOT ANNOTATED: a stub board wired the sanctioned way exits 2 and its
#          board line NEVER REACHES STDOUT. This is the arm that grades the defect class rather than the call.
#   ARM 8  the RATCHET, with a printed denominator: over every board-shaped script in scripts/, the number
#          carrying the guard may never fall below the floor pinned here. A new board without it lands red
#          here rather than in the next false row.
#   ARM 9  ONE COPY: no board re-spells the fingerprint rule privately (the donor's own copy was retired
#          onto the library in the landing this gate accompanies).
#
# HERMETIC: arms 3-7 run against scratch files under mktemp and never touch this tree's scrip, out/ or
# SCORE.md. Arms 1-2 and 8-9 are static reads. NO BUILD REQUIRED -- this gate grades wiring and a library
# function, not codegen, so it must not refuse on a stale binary it never executes.
# EXIT: 0 all arms · 1 an arm failed · 2 REFUSED (could not measure).
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
GATE_NAME="test_gate_boards_refuse_when_the_binary_moves"; export GATE_NAME
. "$HERE/lib_gate.sh" 2>/dev/null || { echo "⛔ REFUSED-TO-GRADE rc=2 [$GATE_NAME]: lib_gate.sh unloadable"; exit 2; }
for _fn in gate_bin_watch gate_bin_unmoved gate_file_has_bin_watch_guard gate_verdict gate_floor; do
    command -v "$_fn" >/dev/null 2>&1 || { echo "⛔ REFUSED-TO-GRADE rc=2 [$GATE_NAME]: lib_gate.sh carries no $_fn -- the rule under test is unreachable"; exit 2; }
done

# ⛔ THE ONE RUNNER'S BOARDS ARE NAMED, NOT GLOBBED. MODE line 2 (Lon 2026-09-10 16:3x, "one runner and one
# board") gives the coo exactly these four, and the fifth is the donor the library function came out of. A
# glob here would silently change what this gate asserts every time somebody adds a board_*.sh; the growing
# population is ARM 8's job, and it is a ratchet precisely so the two questions stay separate.
BOARDS=(test_icon_jcon_suite.sh test_icon_arizona_suite.sh test_icon_ipl_suite.sh board_icon_master.sh test_corpus_snobol4.sh)

fails=0; examined=0
red() { echo "  ⛔ $*"; fails=$((fails+1)); }
ok()  { echo "  ok    $*"; }

echo "── ARM 1-2: the one runner's boards carry the guard, and it precedes publication ──"
for b in "${BOARDS[@]}"; do
    f="$HERE/$b"
    [ -f "$f" ] || { echo "⛔ REFUSED-TO-GRADE rc=2 [$GATE_NAME]: named board missing: $b -- a renamed board must be renamed HERE too, never dropped"; exit 2; }
    examined=$((examined+1))
    gate_file_has_bin_watch_guard "$f"; rc=$?
    case "$rc" in
        0) ok "$b carries gate_bin_watch and gate_bin_unmoved, and the check precedes the first published count" ;;
        1) red "$b: no start/end binary fingerprint, or gate_bin_unmoved comes AFTER the first line that publishes a count -- this board can print a table graded on two builds" ;;
        *) echo "⛔ REFUSED-TO-GRADE rc=2 [$GATE_NAME]: could not read $b"; exit 2 ;;
    esac
done

W="$(mktemp -d "${TMPDIR:-/tmp}/gate_bin_moved.XXXXXX")" || { echo "⛔ REFUSED-TO-GRADE rc=2 [$GATE_NAME]: mktemp failed"; exit 2; }
trap 'rm -rf "$W"' EXIT
printf 'AAAA' > "$W/a.bin"; printf 'BBBB' > "$W/b.so"

# ⛔ EACH ARM RUNS IN ITS OWN SUBSHELL because gate_bin_watch/gate_bin_unmoved `exit` on refusal -- that is
# their contract (a board must not be able to continue past one) and it is exactly why they cannot be probed
# in-process. The subshell's rc IS the measurement.
echo "── ARM 3-6: the authority itself ──"
out3="$( ( . "$HERE/lib_gate.sh"; GATE_NAME=probe gate_bin_watch "$W/a.bin" "$W/b.so"; printf 'CCCC' > "$W/a.bin"; GATE_NAME=probe gate_bin_unmoved; echo "PUBLISHED" ) 2>&1 )"; rc3=$?
if [ "$rc3" -eq 2 ] && grep -q 'THE BINARY MOVED UNDER THIS BOARD' <<<"$out3" && ! grep -q 'PUBLISHED' <<<"$out3"; then
    ok "ARM 3 a changed artifact refuses rc=2 and names both fingerprints: $(grep -o 'start \[[^]]*\] end \[[^]]*\]' <<<"$out3" | head -1)"
else red "ARM 3 a changed artifact did not refuse: rc=$rc3 out=$(head -2 <<<"$out3")"; fi
examined=$((examined+1))

printf 'AAAA' > "$W/a.bin"
out4="$( ( . "$HERE/lib_gate.sh"; GATE_NAME=probe gate_bin_watch "$W/a.bin" "$W/b.so"; GATE_NAME=probe gate_bin_unmoved; echo "PUBLISHED" ) 2>&1 )"; rc4=$?
if [ "$rc4" -eq 0 ] && grep -q 'PUBLISHED' <<<"$out4"; then ok "ARM 4 CONTROL: an unmoved pair passes and the board publishes"
else red "ARM 4 CONTROL failed -- a guard that always refuses would satisfy ARM 3: rc=$rc4 out=$(head -2 <<<"$out4")"; fi
examined=$((examined+1))

out5="$( ( . "$HERE/lib_gate.sh"; GATE_NAME=probe gate_bin_watch "$W/a.bin" "$W/b.so"; rm -f "$W/b.so"; GATE_NAME=probe gate_bin_unmoved; echo "PUBLISHED" ) 2>&1 )"; rc5=$?
if [ "$rc5" -eq 2 ] && ! grep -q 'PUBLISHED' <<<"$out5"; then ok "ARM 5 a DELETED artifact refuses rc=2 (the moved case in its loudest form)"
else red "ARM 5 a deleted artifact did not refuse: rc=$rc5 out=$(head -2 <<<"$out5")"; fi
examined=$((examined+1))
printf 'BBBB' > "$W/b.so"

out6="$( ( . "$HERE/lib_gate.sh"; GATE_NAME=probe gate_bin_watch "$W/nothing-here" ; echo "GRADED-ANYWAY" ) 2>&1 )"; rc6=$?
if [ "$rc6" -eq 2 ] && ! grep -q 'GRADED-ANYWAY' <<<"$out6"; then ok "ARM 6 an unfingerprintable artifact refuses AT WATCH TIME, before the run is paid for"
else red "ARM 6 watch accepted an artifact it could not fingerprint: rc=$rc6 out=$(head -2 <<<"$out6")"; fi
examined=$((examined+1))

echo "── ARM 7: publication is PREVENTED, not annotated ──"
# ⭐ THIS IS THE ARM THAT GRADES THE DEFECT CLASS. Arms 3-6 prove the check answers correctly; this one proves
# a board wired the sanctioned way cannot reach its own board line, which is the property hq_R's run lacked.
cat > "$W/stub_board.sh" <<STUB
#!/usr/bin/env bash
set -uo pipefail
. "$HERE/lib_gate.sh"
GATE_NAME=stub_board gate_bin_watch "$W/a.bin" "$W/b.so"
printf 'MOVED' > "$W/a.bin"          # somebody rebuilds while this board is grading
GATE_NAME=stub_board gate_bin_unmoved
echo "STUB_SUITE_BOARD total=759 m3_pass=635"
python3 -c "print('and here it would have written SCORE.md')"
STUB
chmod +x "$W/stub_board.sh"
out7="$(bash "$W/stub_board.sh" 2>&1)"; rc7=$?
if [ "$rc7" -eq 2 ] && ! grep -q 'STUB_SUITE_BOARD' <<<"$out7" && ! grep -q 'SCORE.md' <<<"$out7"; then
    ok "ARM 7 the stub board exited 2 and NEITHER its board line NOR its score write was reached"
else red "ARM 7 a board that straddled a swap still published: rc=$rc7 out=$(head -3 <<<"$out7")"; fi
examined=$((examined+1))

echo "── ARM 8: the RATCHET over every board-shaped script ──"
# ⛔ THE POPULATION IS SPELLED HERE AND ITS DENOMINATOR IS PRINTED. Board-shaped = a SUITE RUNNER or BOARD
# (test_*_suite.sh, test_corpus_*.sh, board_*.sh, raku_roast_scoreboard.sh) that publishes a count -- it echoes
# a *_BOARD line or writes a SCORE.md row. ⛔ NAME-SHAPE FIRST, THEN BEHAVIOUR, and the first draft of this arm
# had only the second half: it swept in handoff_status.sh, util_census_fpc_tests.sh, a seat-identity gate and
# six bench_* scripts, none of which grade a population, and then reported a coverage fraction over them. A
# denominator that includes things nobody will ever wire makes the ratchet unreadable in the direction that
# matters -- it can only ever go down as unrelated scripts are added.
# THE FLOOR is what was wired the day this gate landed; it may rise and may never fall. ⛔ NOT A FAIL=0 BAR:
# parked boards predate the guard, and a gate nobody can satisfy is worse than no gate (hq_P's standing ruling
# on the retired optbypass watermark). ⭐ The five the one runner actually runs are ARM 1's job and are already
# red-if-missing there; this arm exists so the SIXTH board to be wired can never quietly become unwired.
COVER_FLOOR=5
pop=0; covered=0; uncovered=""
while IFS= read -r f; do
    base="$(basename "$f")"
    case "$base" in
        test_*_suite.sh|test_corpus_*.sh|board_*.sh|raku_roast_scoreboard.sh) : ;;
        *) continue ;;
    esac
    body="$(grep -vE '^[[:space:]]*#' "$f" 2>/dev/null)" || continue
    grep -qE '_BOARD |util_score_row\.py' <<<"$body" || continue
    pop=$((pop+1))
    gate_file_has_bin_watch_guard "$f"; if [ $? -eq 0 ]; then covered=$((covered+1)); else uncovered="$uncovered $base"; fi
done < <(find "$HERE" -maxdepth 1 -name '*.sh' -type f | sort)
echo "    board-shaped scripts=$pop  carrying the guard=$covered  floor=$COVER_FLOOR"
[ "$pop" -ge 5 ] || { echo "⛔ REFUSED-TO-GRADE rc=2 [$GATE_NAME]: found only $pop board-shaped scripts -- zero-work-examined is indistinguishable from all-clean"; exit 2; }
if [ "$covered" -ge "$COVER_FLOOR" ]; then ok "ARM 8 ratchet holds: $covered of $pop board-shaped scripts carry the start/end fingerprint (floor $COVER_FLOOR)"
else red "ARM 8 RATCHET FELL: $covered of $pop carry the guard, floor is $COVER_FLOOR -- a board lost its start/end fingerprint"; fi
[ -n "$uncovered" ] && echo "    not yet wired (named, not counted as red -- raise COVER_FLOOR as these land):$uncovered"
examined=$((examined+1))

echo "── ARM 9: ONE COPY of the fingerprint rule ──"
# ⛔ THE DONOR KEPT ITS OWN COPY THROUGH THE 2026-09-06 HOIST and the fleet carried both for four days. A cure
# applied to one copy strengthens everyone's belief the class is dead while the other keeps the bug.
copies="$(grep -lE '_bin_fp\(\)|md5sum "\$SCRIP" "\$RT_DIR/libscrip_rt\.so"' "$HERE"/*.sh 2>/dev/null | grep -v '/lib_gate\.sh$' | xargs -r -n1 basename | tr '\n' ' ')"
if [ -z "$copies" ]; then ok "ARM 9 the fingerprint rule is spelled only in lib_gate.sh"
else red "ARM 9 a private copy of the fingerprint rule survives: $copies -- retire it onto gate_bin_watch/gate_bin_unmoved"; fi
examined=$((examined+1))

GATE_EXAMINED="$examined"
gate_verdict "$fails" "arm(s) failed (boards named ${#BOARDS[@]}, board-shaped population $pop, covered $covered)"
