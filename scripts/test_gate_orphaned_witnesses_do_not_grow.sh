#!/usr/bin/env bash
# scripts/test_gate_orphaned_witnesses_do_not_grow.sh — ceo CEO-414 (minted hq_P 2026-09-08).
#
# WHAT IT GATES.  An ORPHANED WITNESS is a committed `<name><ext>` + `<name>.ref` pair under
# corpus/tests/<lang>/ whose name appears in NO `ALL.csv`, `ALL.xfail` or `ALL.excluded.txt`.
# It is not passing, not expected-failing and not excluded: NO DENOMINATOR CONTAINS IT, so no
# board has ever run it.  Of the 39 SNOBOL4 orphans measured at mint, FOUR ARE RED and one of
# those SEGFAULTS in both modes (nested_arbno_rpos, rc=139) -- while the SNOBOL4 master read
# 1894/1894 FAIL=0 on the same tree.  This count may FALL, never RISE.
#
# ⭐ WHY A RATCHET IS LEGITIMATE HERE — the question test_gate_term_wordref_ratchet.sh's header
# forces on anyone copying its shape is CAN THIS COUNT REACH ZERO BY DESIGN?  Here it can and is
# meant to: every witness pair we commit is supposed to end up in its language's master, so a
# remaining orphan is debt and 0 is the intended terminal value.  It never fires on legitimate
# work, because ABSORBING an orphan lowers the count and writing a NEW witness straight into the
# master never raises it.
#
# ⛔ THE CURE IS TO ABSORB, NEVER TO DELETE.  Deleting an orphan also makes this gate green and is
# the one move that would satisfy the ratchet while destroying the evidence.  Say so here because
# a ratchet cannot tell the two apart.
#
# ⛔ MEMBERSHIP IS SUBSTRING, NOT TOKEN — AND THAT IS A CORRECTION, NOT A PREFERENCE.  Absorbed
# files are RENAMED into the master and keep their original filename only inside a longer token:
# icon's `alt_arith` lives in ALL.csv as `parser_alt_arith__alt_arith`.  The first census of this
# defect used token matching, could not see that, and MANUFACTURED ~153 FALSE ICON ORPHANS (it
# reported icon 172 / total 213; the truth is icon 19 / total 59).  ⭐ It was control-armed in both
# directions -- ON SNOBOL4, whose naming happens not to use that form, so the control arm passed
# AND COULD NOT HAVE FAILED.  A control arm proves the instrument on the population you ran it on.
# Substring matching is deliberately GENEROUS: it UNDER-reports rather than inventing orphans,
# which is the only safe direction for a ratchet.
set -u
cd "$(dirname "$0")/.." || exit 2
CORPUS="${CORPUS:-$(cd .. && pwd)/corpus}"
CENSUS="$CORPUS/tests/ORPHANED-WITNESSES.tsv"
[ -d "$CORPUS/tests" ] || { echo "⛔ GATE REFUSES(2): no corpus tests tree at $CORPUS/tests"; exit 2; }
[ -f scripts/util_orphaned_witness_census.py ] || { echo "⛔ GATE REFUSES(2): census generator missing"; exit 2; }
# ---- the floor, per language.  Lower a number ONLY in the commit that absorbs the witnesses.
# ⭐ FLOOR_icon 19 -> 20 (hq_V 2026-09-10, ceo CEO-516 landing). NET +1 FROM TWO MOVES IN OPPOSITE DIRECTIONS, named
# apart so neither hides the other: the cfo's EIGHTH pair (cset_membership_is_a_bit_test_not_a_name_lookup) was
# ABSORBED into the Icon master and its loose pair deleted, which REMOVED an orphan; and the cfo's SEVENTH pair
# (trace_call_line_prints_every_parameter_and_images_a_list) became a PERMANENT loose keeper, which ADDED one.
# ⛔ THE KEEPER IS NOT A DEBT AND THIS RAISE IS NOT A RATCHET LEAK: it prints its own file name in all 43 of its
# trace lines, so the master builder's rename would make it red by construction (86 differing lines, measured,
# with a control green under its own name) -- tests/icon/KEEP.md carries the measurement. It can never be absorbed,
# so its orphan is permanent and the floor is where that fact belongs.
# ⭐⛔ FLOOR_icon 20 -> 5 (hq_V 2026-09-10, ceo CEO-541 landing), AND FIFTEEN OF THE EIGHTEEN WERE ALREADY PAID FOR --
# THE CENSUS FILE JUST NEVER SAID SO. This landing absorbs eleven rung36_jcon keepers, which removes 0 orphans (they
# pair a .icn with a .expected, and the census only counts a .ref sibling). The fall is HQV-26's: that landing
# absorbed 18 long-loose witnesses and DID NOT REGENERATE corpus/tests/ORPHANED-WITNESSES.tsv, so the checked-in
# census kept naming absorbed files and this gate read STALE -- red on origin, for anyone who ran it, in a way that
# reads like someone else's debt. Regenerated here in the same commit. ⛔ THE DIRECTION IS THE ONLY MERCY: a stale
# census OVER-reports orphans, so the ratchet failed safe rather than hiding real debt. An absorb landing owes this
# file its regeneration, exactly as it owes ALL.csv one -- the count is not the artifact, the census is.
# ⭐ FLOOR_icon 5 -> 4 (hq_V 2026-09-10, same session, one landing later). Three more loose witnesses absorbed into the
# Icon master -- display_prints_the_frames_locals_and_the_globals and icon_display_builtin_unimplemented (handed green in
# BOTH modes by the cto on SCRIP 6c453e4d0, the mode-4 procedure-name-table cure) and
# a_section_of_the_null_value_raises_string_or_list_expected (handed by the cfo) -- each re-measured three ways here at
# absorb time rather than taken on the handing seat's word.
# ⚠ THE TWO PAIRS THAT AROSE IN MY EARLIER REBASE WINDOW ARE STILL ORPHANS AND ARE STILL THEIRS: corpus cfc826b0f
# (stop_writes_to_a_leading_file_argument_and_ends_its_line) and 2e8b94fe5
# (list_and_reads_check_their_size_argument_and_real_exponentiation_reports_no_value). The count fell past them because
# THREE absorbs outweighed TWO additions, not because either was resolved -- ORPHANED-WITNESSES.tsv still names both, by
# name, which is where a debt belongs. A floor is a ceiling on the count, never a receipt for what is under it.
# ⭐ AND ONE OF THE FIVE WAS NEVER A DEBT AT ALL -- IT WAS BOOKKEEPING LAG, WHICH THIS CENSUS CANNOT TELL APART FROM
# DEBT. corpus bc1b1f900 landed an_ordinary_call_chain_prints_every_frame_in_the_traceback, which hq_S DECLARED in
# KEEP.md in that same commit as a permanent name-echoing keeper -- correctly: its ref carries its own file name on 4
# lines, so the master builder's rename would manufacture a red. It counted as an orphan only because ALL.excluded.txt
# is written by the BUILDER and no build had run since. Verified the echo here before writing anything, then added the
# one line the builder emits verbatim. ⛔ A DECLARED KEEPER READS AS AN ORPHAN UNTIL SOMEBODY RUNS A BUILD: the census
# reads three files, and one of the three lags the declaration that governs it.
FLOOR_icon=4 FLOOR_pascal=0 FLOOR_prolog=0 FLOOR_raku=0 FLOOR_rebus=0 FLOOR_snobol4=39 FLOOR_snocone=1
out="$(python3 scripts/util_orphaned_witness_census.py "$CORPUS" --counts 2>&1)" || { echo "⛔ GATE REFUSES(2): census generator failed:"; echo "$out"; exit 2; }
printf %s "$out" | grep -q . || { echo "⛔ GATE REFUSES(2): census produced NO output -- an empty census is not a green board"; exit 2; }
rc=0 tot=0 seen=0
while IFS=$'\t' read -r lang n; do
  [ -n "${lang:-}" ] || continue
  seen=$((seen+1)); tot=$((tot+n))
  eval "floor=\${FLOOR_$lang-}"
  if [ -z "$floor" ]; then echo "  ⛔ $lang: $n orphan(s) — NO FLOOR PINNED for this language; add FLOOR_$lang to this gate"; rc=1; continue; fi
  if [ "$n" -gt "$floor" ]; then echo "  ⛔ $lang: $n orphan(s), floor $floor — GREW by $((n-floor))"; rc=1
  elif [ "$n" -lt "$floor" ]; then echo "  ⭐ $lang: $n orphan(s), floor $floor — FELL by $((floor-n)); lower FLOOR_$lang to $n in this commit"; rc=1
  else echo "  ok    $lang: $n orphan(s) at the floor"; fi
done <<< "$out"
[ "$seen" -gt 0 ] || { echo "⛔ GATE REFUSES(2): census listed no languages at all"; exit 2; }
if [ ! -f "$CENSUS" ]; then echo "  ⛔ the census file $CENSUS is MISSING — regenerate it"; rc=1
else
  fresh="$(python3 scripts/util_orphaned_witness_census.py "$CORPUS")"
  if ! printf '%s\n' "$fresh" | diff -q - "$CENSUS" >/dev/null 2>&1; then
    echo "  ⛔ $CENSUS is STALE — it does not match the tree; regenerate:"
    echo "       python3 SCRIP/scripts/util_orphaned_witness_census.py corpus > corpus/tests/ORPHANED-WITNESSES.tsv"; rc=1; fi
fi
echo "------------------------------------------------------------"
[ $rc -eq 0 ] && echo "✅ GATE PASS [orphaned_witnesses_do_not_grow]: $tot orphaned witness pair(s), none above its floor" \
              || echo "⛔ GATE FAIL [orphaned_witnesses_do_not_grow]: $tot orphaned witness pair(s) — see above"
exit $rc
