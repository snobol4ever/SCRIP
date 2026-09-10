#!/usr/bin/env bash
# ⭐⭐ GATE: `next` REFUSES A ROW OF A LANGUAGE THE ORDER OF WORK HAS PARKED -- AND SAYS SO.
# Row `snobol4-icon-postoffice-next-refuses-a-row-of-a-language-parked-by-the-order-of-work-on-mode-line-2`
# (CEO-489, hq_P). ⛔ THE MEASURED DEFECT, 2026-09-10: MODE line 1 read NONET and line 2 carried
# `THE ORDER OF WORK IS ICON ONLY`, and `s4e_msg.sh next` served hq_P a rank-0 SNOBOL4 row through the
# owner-cell pass. The picker DID hold a language-freeze check; its extractor matched exactly one spelling,
# `ON <LANG> ONLY`, and the ceo had written `IS ICON ONLY`. The extractor returned EMPTY, empty is
# documented in the picker as NO FREEZE (never "undetermined"), and so every parked-language row in the
# queue became servable -- silently, with no refusal to read. The only thing between an idle seat and hours
# of parked-language work was the seat remembering to refuse.
# ⛔ WHY THE EXISTING GATE WAS GREEN THROUGH ALL OF IT: test_gate_next_honours_the_lane_cut.sh has a
# freeze arm, and its fixture writes `ON SNOBOL4 ONLY` -- the one spelling the parser knew. The gate and the
# code under test shared the assumption, so the canary died with the guard. ⭐ THIS GATE'S FIRST ARM IS
# THEREFORE THE LIVE PROSE VERBATIM, not a spelling chosen to suit the parser: an arm that grades a fixture
# written to match the implementation grades nothing.
# ✅ THE CURE UNDER TEST: THE ORDER OF WORK is DECLARED in a machine line the bus reads --
#     ORDER-OF-WORK: icon
# -- the whitespace/comma-separated list of LIVE languages, matched BY MARKER anywhere in MODE (never by
# line number: the ceo prepends each dated entry, so a positional line would be shoved down by the next
# announcement and start answering for a superseded order of work). Absent marker falls back to the prose
# parse, now widened to the `IS` form as well, so the cure never reads as a loosening.
# ARMS: (a) the live `IS` spelling parks a rank-0 SNOBOL4 row and the icon row is served instead
#       (b) the skip is PRINTED, naming the parked topic and the order of work -- never a silent skip
#       (c) the marker line alone parks it, with no ONLY clause in prose anywhere
#       (d) the marker is AUTHORITATIVE OVER PROSE in the opening direction (marker snobol4 vs prose ICON)
#       (e) THE ORDER OF WORK IS A SET, not one language: `icon snobol4` serves the SNOBOL4 row
#       (f) `ORDER-OF-WORK: all` restricts nothing
#       (g) POSITIVE CONTROL -- no marker and no ONLY clause: the SNOBOL4 row is served (no misfire)
#       (h) the marker is found deep in the file, and the FIRST of several wins (prepend-newest)
#       (i) a language-neutral topic is never parked by any order of work
#       (j) an EXPLICIT ASSIGNMENT outranks the order of work -- the ceo dispatching a parked-language row
#           by name is a deliberate direction, the same precedent `claim` and the rank cap already set
# HERMETIC: builds its own scratch postoffice under mktemp; reads and writes nothing under /home/resources.
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SUT="${SUT:-$HERE/s4e_msg.sh}"
refuse(){ echo "⛔ REFUSED-TO-GRADE: $*"; exit 2; }
[ -f "$SUT" ] || refuse "picker under test not found: $SUT"
W="$(mktemp -d "${TMPDIR:-/tmp}/gate_order_of_work.XXXXXX")" || refuse "mktemp failed"
trap 'rm -rf "$W"' EXIT
mkdir -p "$W/tasks" "$W/claims" "$W/released"
for s in ceo hq_C hq_B hq_P hq_T seat07; do mkdir -p "$W/$s/inbox" "$W/$s/archive"; done
mk(){ printf '%s\t%s\t%s\t%s\n' "$1" "$2" "$3" "$4" >> "$W/QUEUE.tsv"; printf '# TASK %s\nGOAL: fixture\nDONE-WHEN: true\n## NEXT\nfixture\n## QA\n## LEDGER\n' "$2" > "$W/tasks/$2.task.md"; }
run_next(){ S4E_POST="$W" S4E_SEAT="$1" S4E_RELEASE_COOLDOWN=0 bash "$SUT" next 2>&1; }
fails=0; checks=0
ck(){ checks=$((checks+1)); if [ "$1" = ok ]; then printf '  ok    %s\n' "$2"; else printf '  FAIL  %s\n' "$2"; fails=$((fails+1)); fi; }
reset_q(){ : > "$W/QUEUE.tsv"; rm -f "$W/claims/"*.claim 2>/dev/null; }
# The MODE fixture: line 1 the bare value, then dated prose entries newest-first, exactly as the live file
# is shaped. Extra lines are passed through verbatim so an arm can place a marker at any depth.
set_mode(){ : > "$W/MODE"; local l; for l in "$@"; do printf '%s\n' "$l" >> "$W/MODE"; done; }
# ⭐ THE LIVE PROSE, COPIED FROM /home/resources/postoffice/MODE line 2 as it stood 2026-09-10 -- the
# spelling that actually defeated the picker. Never rewrite this to suit the parser; that is the defect.
LIVE_L2='# 2026-09-09 09:0x CDT ceo: MODE stays NONET; THE ORDER OF WORK IS ICON ONLY, on Lon'"'"'s word (2026-09-09 09:0x CDT, in-chat to ceo, verbatim: "Switch to Icon completely.").'
OTHER_L2='# 2026-09-08 19:09 CDT ceo: MODE EXECUTIVE -> QUARTET on Lon'"'"'s word; the four HQs work rows and the executives rule.'

echo "=== gate: next refuses a row of a language parked by THE ORDER OF WORK ==="

# --- (a)+(b): THE REGRESSION ITSELF, in the live spelling ------------------------------------------------
# The rank-0 row is SNOBOL4 and carries hq_P's OWN owner cell -- the owner-cell pass, which is exactly how
# the live picker served it. Nothing but the order of work can explain a refusal here: same seat, same lane,
# better rank, and an owner cell that names the seat by name.
set_mode 'NONET' "$LIVE_L2"
reset_q
mk 0 snobol4-master-red-fixture  hq_P FREE
mk 5 icon-jcon-std-recut-fixture hq_P FREE
out="$(run_next hq_P)"
grep -qE '^LOCKED.*icon-jcon-std-recut-fixture' <<<"$out" && ! grep -qE '^LOCKED.*snobol4-master-red-fixture' <<<"$out" \
  && ck ok "(a) THE ORDER OF WORK IS ICON ONLY parks the rank-0 SNOBOL4 row; the rank-5 icon row is served instead" \
  || ck no "(a) a parked-language row must never be served automatically, owner cell or not -- got: $(grep -E '^LOCKED|QUEUE EMPTY' <<<"$out")"
grep -q 'snobol4-master-red-fixture' <<<"$out" && grep -qE 'SKIP|PARKED|ORDER OF WORK' <<<"$out" \
  && ck ok "(b) the skip is printed and names the parked topic -- a refusal a reader can act on, not silence" \
  || ck no "(b) a parked-language skip must be VISIBLE and NAMED, never silent -- got: $out"

# --- (c): THE MACHINE LINE ALONE, no ONLY clause in prose at all -----------------------------------------
# This is the deliverable the row asked for: the bus reads a declaration, not prose. The prose here names a
# different mode entirely and carries no freeze wording of any spelling.
set_mode 'NONET' "$OTHER_L2" 'ORDER-OF-WORK: icon'
reset_q
mk 0 snobol4-master-red-fixture  hq_P FREE
mk 5 icon-jcon-std-recut-fixture hq_P FREE
out="$(run_next hq_P)"
grep -qE '^LOCKED.*icon-jcon-std-recut-fixture' <<<"$out" \
  && ck ok "(c) the ORDER-OF-WORK marker alone parks the SNOBOL4 row, with no ONLY clause in prose anywhere" \
  || ck no "(c) the machine line must be sufficient on its own -- got: $(grep -E '^LOCKED|QUEUE EMPTY' <<<"$out")"

# --- (d): THE MARKER OUTRANKS PROSE, IN THE OPENING DIRECTION --------------------------------------------
# ⭐ Tested in the direction that can only be explained by precedence: prose says ICON ONLY, the marker says
# snobol4, and the SNOBOL4 row is served. Had the marker merely been ANDed with prose, this row would stay
# parked and the arm would fail. Precedence proven, not assumed.
set_mode 'NONET' "$LIVE_L2" 'ORDER-OF-WORK: snobol4'
reset_q
mk 0 snobol4-master-red-fixture  hq_P FREE
mk 5 icon-jcon-std-recut-fixture hq_P FREE
out="$(run_next hq_P)"
grep -qE '^LOCKED.*snobol4-master-red-fixture' <<<"$out" \
  && ck ok "(d) the marker line is AUTHORITATIVE over prose -- prose ICON ONLY, marker snobol4, SNOBOL4 served" \
  || ck no "(d) the machine line must outrank the prose it replaces -- got: $(grep -E '^LOCKED|QUEUE EMPTY' <<<"$out")"

# --- (e): THE ORDER OF WORK IS A SET ---------------------------------------------------------------------
# ⛔ Equality was wrong the moment the order of work named two languages -- and it stood at two on
# 2026-09-08 ("SNOBOL4 AND ICON TO 100%"). An equality test fails CLOSED here, parking a LIVE language.
set_mode 'NONET' "$OTHER_L2" 'ORDER-OF-WORK: icon snobol4'
reset_q
mk 0 snobol4-master-red-fixture  hq_P FREE
mk 5 icon-jcon-std-recut-fixture hq_P FREE
out="$(run_next hq_P)"
grep -qE '^LOCKED.*snobol4-master-red-fixture' <<<"$out" \
  && ck ok "(e) two live languages: a SNOBOL4 row is served when the order of work names icon AND snobol4" \
  || ck no "(e) the order of work is a SET, not one language -- a live language must never be parked: $(grep -E '^LOCKED|QUEUE EMPTY' <<<"$out")"

# --- (f): `all` RESTRICTS NOTHING ------------------------------------------------------------------------
set_mode 'NONET' "$LIVE_L2" 'ORDER-OF-WORK: all'
reset_q
mk 0 prolog-inria-red-fixture hq_P FREE
out="$(run_next hq_P)"
grep -qE '^LOCKED.*prolog-inria-red-fixture' <<<"$out" \
  && ck ok "(f) ORDER-OF-WORK: all restricts nothing -- the explicit way to lift a freeze without deleting the line" \
  || ck no "(f) `all` must open every language -- got: $(grep -E '^LOCKED|QUEUE EMPTY' <<<"$out")"

# --- (g): POSITIVE CONTROL, THE ARM THAT PROVES THE CHECK CAN BE SILENT ----------------------------------
# ⛔ A checker that always refuses and one that never refuses BOTH pass every arm above except this one.
set_mode 'NONET' "$OTHER_L2"
reset_q
mk 0 snobol4-master-red-fixture hq_P FREE
out="$(run_next hq_P)"
grep -qE '^LOCKED.*snobol4-master-red-fixture' <<<"$out" \
  && ck ok "(g) POSITIVE CONTROL: with no marker and no ONLY clause, the SNOBOL4 row IS served -- no misfire" \
  || ck no "(g) an inactive order of work must restrict nothing -- got: $(grep -E '^LOCKED|QUEUE EMPTY' <<<"$out")"

# --- (h): FOUND BY MARKER, NOT BY POSITION; FIRST WINS ---------------------------------------------------
# ⭐ The ceo PREPENDS each dated entry, so a positional read (line 3) would be shoved down by the very next
# announcement and start answering for a superseded order of work. Marker at depth 6 here, with a
# SUPERSEDED marker below it: the first must win, which under prepend-newest is the newest.
set_mode 'NONET' "$LIVE_L2" "$OTHER_L2" '# 2026-09-07 08:15 CDT ceo: CEO-382 announcement shape.' \
         '# 2026-09-06 15:41 CDT ceo: MODE OCTET.' 'ORDER-OF-WORK: icon' \
         '# older entries below' 'ORDER-OF-WORK: prolog'
reset_q
mk 0 prolog-inria-red-fixture    hq_P FREE
mk 5 icon-jcon-std-recut-fixture hq_P FREE
out="$(run_next hq_P)"
grep -qE '^LOCKED.*icon-jcon-std-recut-fixture' <<<"$out" && ! grep -qE '^LOCKED.*prolog-inria-red-fixture' <<<"$out" \
  && ck ok "(h) the marker is matched by MARKER at any depth and the FIRST wins -- a superseded marker below it is ignored" \
  || ck no "(h) a positional read would break on the next prepended entry -- got: $(grep -E '^LOCKED|QUEUE EMPTY' <<<"$out")"

# --- (i): A LANGUAGE-NEUTRAL TOPIC IS NEVER PARKED -------------------------------------------------------
# Tooling/meta rows name no language and must survive every order of work -- otherwise the instrument that
# repairs the bus is itself unreachable the moment the bus is frozen.
set_mode 'NONET' "$LIVE_L2" 'ORDER-OF-WORK: icon'
reset_q
mk 0 postoffice-tooling-fixture hq_P FREE
out="$(run_next hq_P)"
grep -qE '^LOCKED.*postoffice-tooling-fixture' <<<"$out" \
  && ck ok "(i) a topic naming no language is language-neutral and is served under any order of work" \
  || ck no "(i) a language-neutral topic must never be parked -- got: $(grep -E '^LOCKED|QUEUE EMPTY' <<<"$out")"

# --- (j): AN EXPLICIT ASSIGNMENT OUTRANKS THE ORDER OF WORK ----------------------------------------------
# ⭐ PINNED DELIBERATELY, so nobody later "tightens" the freeze into blocking the ceo's own dispatch. The
# order of work is the DEFAULT a picker applies when choosing for itself; a row an authority pointed at this
# seat BY NAME is a decision already taken -- the same precedent the rank cap states outright ("an explicit
# direction outranks the cap") and that `claim` has always been. This row itself reached hq_P that way: its
# topic begins `snobol4-` and the order of work was ICON ONLY.
set_mode 'NONET' "$LIVE_L2" 'ORDER-OF-WORK: icon'
reset_q
mk 0 snobol4-assigned-tooling-fixture hq_P ASSIGNED:hq_P
# ⛔ THE ASSIGNMENT LIVES IN THE CLAIM FILE, NOT THE QUEUE COLUMN -- PASS 1 reads `claims/<topic>.claim`
# (first line the assignee, an `ASSIGNED-BY` line, neither DONE nor RUNNING), which is precisely what
# `dispatch` writes. A fixture that set only the ASSIGNED:<seat> column would prove nothing about PASS 1.
{ echo 'hq_P'; echo "ASSIGNED-BY ceo $(date -u +%FT%TZ)"; } > "$W/claims/snobol4-assigned-tooling-fixture.claim"
out="$(run_next hq_P)"
grep -qE 'snobol4-assigned-tooling-fixture' <<<"$out" && ! grep -qE 'QUEUE EMPTY' <<<"$out" \
  && ck ok "(j) a row ASSIGNED to this seat by name is still served -- an explicit direction outranks the default" \
  || ck no "(j) the order of work must not block a deliberate assignment -- got: $out"

echo "---"
if [ "$fails" -eq 0 ]; then printf '✅ PASS: %d/%d arms — next refuses a parked language under THE ORDER OF WORK, hermetically\n' "$checks" "$checks"; exit 0
else printf '⛔ FAIL: %d of %d arms red\n' "$fails" "$checks"; exit 1; fi
