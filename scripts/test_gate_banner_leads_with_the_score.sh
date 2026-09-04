#!/usr/bin/env bash
# test_gate_banner_leads_with_the_score.sh -- the seat banner shows the leaderboard score at the TOP of every turn
# (Lon 2026-09-04: "Ensure the banner is showing the score by each seat at every turn."). Two arms, each proven once:
# PASS arm: `s4e_msg.sh banner` prints a `PROGRESS 09-10 |` line within its first 8 lines (before the slow work).
# FAIL-ONCE arm: with S4E_PROGRESS_PROBE_BROKEN=1 the banner cannot read a score and must print `PROGRESS: UNREADABLE`
# within the same window -- a refusal that is VISIBLE, never silent, never the verdict.
# EXIT 0 both arms hold · 1 either arm fails · 2 REFUSED (no postoffice or no banner to run).
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
[ -d /home/resources/postoffice ] || { echo "⛔ GATE REFUSE(2) [banner-leads-with-the-score]: no postoffice at /home/resources/postoffice"; exit 2; }
[ -x "$HERE/s4e_msg.sh" ] || { echo "⛔ GATE REFUSE(2) [banner-leads-with-the-score]: no s4e_msg.sh"; exit 2; }
head8="$(timeout 100 bash "$HERE/s4e_msg.sh" banner 2>&1 | head -8)"
[ -n "$head8" ] || { echo "⛔ GATE REFUSE(2) [banner-leads-with-the-score]: the banner printed nothing"; exit 2; }
red=0
printf '%s\n' "$head8" | grep -q '^PROGRESS 09-10 |' || { echo "RED: no 'PROGRESS 09-10 |' line in the banner's first 8 lines:"; printf '%s\n' "$head8" | sed 's/^/    /'; red=1; }
broken="$(S4E_PROGRESS_PROBE_BROKEN=1 timeout 100 bash "$HERE/s4e_msg.sh" banner 2>&1 | head -8)"
printf '%s\n' "$broken" | grep -q '^PROGRESS: UNREADABLE' || { echo "RED: with the probe broken the banner did not print a visible 'PROGRESS: UNREADABLE' line in its first 8 lines"; red=1; }
printf '%s\n' "$broken" | grep -q '^PROGRESS 09-10 |' && { echo "RED: the probe env did not break the score read (control arm)"; red=1; }
[ $red -eq 0 ] && echo "✅ GATE PASS(0) [banner-leads-with-the-score]: the score line is at the head of the banner; a refusal prints visibly"
exit $red
