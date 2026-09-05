#!/usr/bin/env bash
# test_gate_next_honours_the_lane_cut.sh -- THE DEPENDENCY-PROMOTION PATH IN `next` MUST HONOUR THE SAME
# LANE CUT AND MODE LANGUAGE FREEZE THE ORDINARY FREE-ROW PATH ALREADY APPLIES, BEFORE IT EVER CLAIMS A ROW.
# Row next-dependency-promotion-walks-around-the-mode-lane-filter (seat01 measured the class; hq_B minted).
#
# THE MEASURED DEFECT: with MODE reading FLEET-8 ON SNOBOL4 ONLY and seat01's lane being snoflake/aisnobol/
# dotnet, `next` served and CLAIMED icon-ladder-every-feature-in-isolation-with-variations (rank 1, Icon) --
# promoted as the blocker of another rank-1 Icon row. THE ENABLING DEFECT: s4e_servable_blocker (CURE 1,
# picker-dependency-and-boomerang-blindness) only asks "is the blocker in a servable STATE" and, on yes,
# the caller claims and serves it immediately -- neither the lane filter nor any language-freeze check
# (which did not exist at all) ever ran on the PROMOTED row, only on the row it displaced.
#
# WHAT IT PROVES, against a THROWAWAY postoffice (S4E_POST) so the live queue is never read or written:
#   (a) OWN-LANE PASS REFUSES A CROSS-LANE PROMOTION: a blocked own-lane row whose blocker is a DIFFERENT
#       lane's FREE row does NOT get that blocker promoted+served in the own-lane pass -- a genuine
#       own-lane row at a worse rank is served instead, exactly as the ordinary FREE-row path already
#       prefers own-lane over rank.
#   (b) THE REFUSAL IS PRINTED, NEVER SILENT: naming the blocked row, the refused blocker, and the lane
#       mismatch -- per the row's own explicit requirement.
#   (c) CROSS-LANE FALLBACK STILL WORKS FOR PROMOTION: when the seat's own lane truly has nothing else
#       servable, the blocker IS promoted and served cross-lane -- dependency inversion is not removed,
#       only gated, exactly as the row's GOAL insists ("do NOT fix this by removing dependency promotion").
#   (d) THE MODE LANGUAGE FREEZE REFUSES A PROMOTION EVEN WHEN THE LANE MATCHES: a same-lane blocker whose
#       LANGUAGE differs from an active "ON <LANG> ONLY" freeze is refused -- this is the actual shape of
#       the reported incident (owner cell was already correct; nothing but a language-phase rule should
#       have stopped it, and until this fix nothing did).
#   (e) THE FREEZE REFUSAL IS ALSO PRINTED, NEVER SILENT.
#   (f) POSITIVE CONTROL -- NO FREEZE ACTIVE: the identical same-lane, wrong-"language" blocker from (d) IS
#       promotable when MODE carries no "ON ... ONLY" clause -- proves the freeze check does not misfire
#       when it should be a no-op, not just that it fires when it should.
#   (g) POSITIVE CONTROL -- LANGUAGE-NEUTRAL TOPIC: a promotion candidate with no recognized language
#       prefix (tooling/meta, this row's own kind of topic) is unaffected by an active freeze.
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SUT="${SUT:-$HERE/s4e_msg.sh}"
refuse(){ echo "⛔ REFUSED-TO-GRADE: $*"; exit 2; }
[ -f "$SUT" ] || refuse "picker under test not found: $SUT"
W="$(mktemp -d "${TMPDIR:-/tmp}/gate_promo_lane.XXXXXX")" || refuse "mktemp failed"
trap 'rm -rf "$W"' EXIT
mkdir -p "$W/tasks" "$W/claims" "$W/released"
for s in ceo hq_C hq_B hq_P hq_T seat07; do mkdir -p "$W/$s/inbox" "$W/$s/archive"; done
printf 'hq_B\n' > "$W/seat07/HQ"     # seat07's lane is hq_B for this fixture (icon)
mk(){ printf '%s\t%s\t%s\t%s\n' "$1" "$2" "$3" "$4" >> "$W/QUEUE.tsv"; printf '# TASK %s\nGOAL: fixture\nDONE-WHEN: true\n## NEXT\nfixture\n## QA\n## LEDGER\n' "$2" > "$W/tasks/$2.task.md"; }
run_next(){ S4E_POST="$W" S4E_SEAT="$1" S4E_RELEASE_COOLDOWN=0 bash "$SUT" next 2>&1; }
fails=0; checks=0
ck(){ checks=$((checks+1)); if [ "$1" = ok ]; then printf '  ok    %s\n' "$2"; else printf '  FAIL  %s\n' "$2"; fails=$((fails+1)); fi; }
reset_q(){ : > "$W/QUEUE.tsv"; rm -f "$W/claims/"*.claim 2>/dev/null; }
set_mode(){ printf '%s\n' "$1" > "$W/MODE"; }

echo "=== gate: next's dependency-promotion path honours the lane cut and the MODE language freeze ==="

# --- (a)+(b)+(c): THE LANE CUT ---------------------------------------------------------------------------
set_mode 'FLEET-16'   # no language freeze active for this block -- isolates the LANE mechanism alone

# (a)+(b): own-lane pass must refuse to promote a cross-lane blocker, and say so; a genuine own-lane
# row at a worse rank is served instead.
reset_q
mk 0 icon-blocked-row      hq_B BLOCKED-ON:snobol4-blocker-row
mk 5 snobol4-blocker-row   unassigned FREE
mk 1 icon-fallback-row     unassigned FREE
out="$(run_next seat07)"
grep -qE '^LOCKED.*icon-fallback-row' <<<"$out" && ! grep -qE '^LOCKED.*snobol4-blocker-row' <<<"$out" \
  && ck ok "(a) own-lane pass serves the genuine own-lane fallback row, not the cross-lane promoted blocker" \
  || ck no "(a) a cross-lane blocker must not be promoted while an own-lane row is servable -- got: $(grep -E '^LOCKED' <<<"$out")"
grep -qi 'REFUSED PROMOTION' <<<"$out" && grep -q 'icon-blocked-row' <<<"$out" && grep -q 'snobol4-blocker-row' <<<"$out" \
  && ck ok "(b) the refusal is printed, naming both the blocked row and the refused blocker" \
  || ck no "(b) a refused promotion must be a visible REFUSAL naming both topics, never a silent skip -- got: $out"

# (c): with NO own-lane fallback available at all, the cross-lane blocker IS still promoted+served --
# dependency inversion keeps working, only gated by lane, not removed.
reset_q
mk 0 icon-blocked-row2     hq_B BLOCKED-ON:snobol4-blocker-row2
mk 5 snobol4-blocker-row2  unassigned FREE
out="$(run_next seat07)"
grep -qE '^LOCKED.*snobol4-blocker-row2' <<<"$out" \
  && ck ok "(c) cross-lane fallback still promotes the blocker when the seat's own lane has nothing else" \
  || ck no "(c) dependency promotion must still work cross-lane when nothing own-lane is servable -- got: $(grep -E '^LOCKED|QUEUE EMPTY' <<<"$out")"

# --- (d)+(e)+(f): THE MODE LANGUAGE FREEZE ---------------------------------------------------------------
# Same lane on both rows this time (deliberately) so a refusal here can ONLY be explained by the freeze,
# never by the lane cut above -- isolates the item-(3) mechanism from item-(2)'s.
set_mode 'FLEET-8
# 2026-09-04 18:23 CDT ceo: MODE FLEET-16 -> FLEET-8 ON SNOBOL4 ONLY, on Lon'"'"'s word'

# (d)+(e): an active SNOBOL4-only freeze refuses a same-lane Icon blocker, and says so; a same-lane
# SNOBOL4-language row at a worse rank is served instead.
reset_q
mk 0 icon-blocked-row3       hq_B BLOCKED-ON:icon-frozen-blocker
mk 5 icon-frozen-blocker     hq_B FREE
mk 1 snobol4-fallback-row    unassigned FREE
out="$(run_next seat07)"
grep -qE '^LOCKED.*snobol4-fallback-row' <<<"$out" && ! grep -qE '^LOCKED.*icon-frozen-blocker' <<<"$out" \
  && ck ok "(d) an active SNOBOL4-only freeze refuses a same-lane Icon blocker; the SNOBOL4 fallback row is served instead" \
  || ck no "(d) the freeze must refuse promotion by LANGUAGE even when the lane matches -- got: $(grep -E '^LOCKED' <<<"$out")"
grep -qi 'REFUSED PROMOTION' <<<"$out" && grep -qi 'SNOBOL4' <<<"$out" \
  && ck ok "(e) the freeze refusal is printed and names the frozen language" \
  || ck no "(e) a freeze-refused promotion must be a visible, named REFUSAL, never a silent skip -- got: $out"

# (f) POSITIVE CONTROL: identical fixture, no freeze active -- the Icon blocker IS promotable. Proves the
# freeze check is not simply always-refusing (a checker that never fires or always fires both pass silently
# without this control -- the standing lesson to test both arms, not just the reported direction).
set_mode 'FLEET-16'
reset_q
mk 0 icon-blocked-row3       hq_B BLOCKED-ON:icon-frozen-blocker
mk 5 icon-frozen-blocker     hq_B FREE
mk 1 snobol4-fallback-row    unassigned FREE
out="$(run_next seat07)"
grep -qE '^LOCKED.*icon-frozen-blocker' <<<"$out" \
  && ck ok "(f) CONTROL: the same Icon blocker IS promoted when no MODE freeze is active" \
  || ck no "(f) CONTROL FAILED: with no freeze active, promotion must work exactly as before this row -- got: $(grep -E '^LOCKED' <<<"$out")"

# (g) POSITIVE CONTROL: a language-neutral promotion candidate (no recognized prefix) is never frozen out.
set_mode 'FLEET-8
# 2026-09-04 18:23 CDT ceo: MODE FLEET-16 -> FLEET-8 ON SNOBOL4 ONLY, on Lon'"'"'s word'
reset_q
mk 0 icon-blocked-row4        hq_B BLOCKED-ON:postoffice-tooling-blocker
mk 5 postoffice-tooling-blocker unassigned FREE
out="$(run_next seat07)"
grep -qE '^LOCKED.*postoffice-tooling-blocker' <<<"$out" \
  && ck ok "(g) CONTROL: a language-neutral blocker is promoted even under an active freeze" \
  || ck no "(g) CONTROL FAILED: a topic with no recognized language prefix must never be frozen out -- got: $(grep -E '^LOCKED' <<<"$out")"

echo "------------------------------------------------------------"
[ "$fails" -ne 0 ] && { echo "⛔ GATE FAIL: $fails of $checks check(s) failed"; exit 1; }
echo "✅ GATE PASS: $checks/$checks checks"; exit 0
