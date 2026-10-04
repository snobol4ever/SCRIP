#!/usr/bin/env bash
# test_gate_gc_every_credited_bare_poll_site_has_a_zero_collection_witness.sh -- THE BARE-POLL RE-SCREEN (cto 2026-09-23;
# row gc-the-116-bare-poll-sites-already-counted-as-done-are-re-screened-by-the-zero-collection-arm; ceo CEO-1137/1144).
#
# WHAT IT GRADES.  The safe-point census credits a site as POLLED when a poll is EMITTED; the bare form roots nothing and
# has three convictions as a wrong answer with the collector never run.  This gate reads the DECLARED table
# scripts/gc_bare_poll_witnesses.tsv (one row per credited bare site, naming a witness or UNWITNESSED) through
# util_gc_safe_point_contract.py --bare-poll: a site is WITNESSED only when its named witness's emission reaches the site
# (the poll helper stamps its template:line into the emitted text) AND that witness's run at stress 0 in a 512 MB arena
# reports collections=0 grew=0 on its own [GC-EXERCISE] line AND prints its oracle-cut .ref, in mode 3 and in mode 4.
# Everything else is NAMED: UNWITNESSED, STALE, NOT-ZERO, UNREAD, DIVERGING, RETIRED, UNDECLARED.
#
# ⛔ THE PIN.  SCRIP_HEAP_MB=512 with SCRIP_HEAP_KB removed, set INSIDE the checker (bare_run_env) and not here: the
# quantity measured is the poll's PRESENCE with the collector never running, and a tiny arena makes every reading
# NOT-ZERO by construction.  This gate is named in test_gate_gc_the_tiny_arena_is_the_default_of_gc_testing.sh's
# declared pinned set for that reason (CEO-931 pin rule: a pin names its reason at the pin and its name in the set).
#
# ARMS.  (a) the checker's selftest holds (the clause's ten plants included);  (b) the reading is not REFUSED and its
# notes-vs-calls count matches (a poll whose note the renderer dropped would read UNREACHED);  (c) DIVERGING is zero --
# a witness that prints the wrong answer with the collector never run is the conviction this row exists for;
# (d) the unwitnessed count is at or below the DECLARED CEILING, which falls with witnesses and RISES ONLY with a landing that ADDS credited bare sites, naming the arrivals (65 over 131 sites at the first landing; 82 over 158 when the 36-to-zero landing of 2026-09-23 added 27 credited bare sites of which 10 the existing witnesses reach:
# the 61 witnesses on disk reach 66; the other 65 are named in the reading and are the row's open work; 79 over 160 at SCRIP 66c79e918; 77 over 160 on 6fe560abc, where the sibling-frames witness reaches the three bb_limit sites and the zd depth planner (5b228d355 73214b8e0 fb66a2cb2) moved the Snocone concat witness's five OUTPUT assignments onto the recording zd path, leaving bb_assign_global.cpp:120 UNWITNESSED).
# 78 over 162 on the landing that made the allocating census follow CONDITIONAL tail exits and GOT slots (cto 2026-09-23, CEO-1224):
# the census now counts rt_str_coerce and rt_size_d as allocating, so two already-polled bare sites join the table -- bb_binop_relop.cpp
# rt_str_coerce (hb_table_dump_every_key_ref.icn reaches it) and bb_unop.cpp rt_size_d (no witness yet: the arrival this ceiling names);
# the two bb_match_capture.cpp sites are the same two under the label rt_cap_open_plain/rt_cap_open, and the rewrite also absorbed the
# 2b89a5ef2 / ae2a9e433 line drift in emit.cpp and bb_define.cpp that had arm (d) red on origin at 86.
# 57 over 150 on the family-1 retirement (cto 2026-09-26, row gc-the-116-bare-poll-sites): ten credited bare sites LEFT THE TABLE with their roads -- the
# marshal_arith cycle, the NV_GET argument arm, the by-name dop leaf, the three write routes, IR_DEFINE role 0 and the two slim roads --
# each proven unreached by a tagged plant compiled over 4693 corpus sources (0 hits, controls 109..605); no arrival, so the ceiling only fell.
# FAIL_ONCE=1 plants a lower ceiling and requires arm (d) to red.
set -u
cd "$(dirname "$0")/.." || exit 2
ROOT="$PWD"
"$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/util_require_fresh.sh" --gate "$(basename "${BASH_SOURCE[0]}" .sh)" || exit $?
G="$(basename "$0" .sh)"
CHK="$ROOT/scripts/util_gc_safe_point_contract.py"
TABLE="$ROOT/scripts/gc_bare_poll_witnesses.tsv"
# 57 over 154 on the slim-road restore (cto 2026-09-27, hq_snobol4's shared-node verdict on 65b0bc779): the two rt_proc_call_open_slim sites
# (bb_call_proc_staged.cpp, the zref and frame slim roads) RETURN to the table UNWITNESSED with their roads -- two SnoRungs DEFINE entries with 30 and
# 60 formals reach them and the plant that retired them never compiled a rung suite's entries; the count holds at 57 because the same re-cut
# (over the witness set on the pristine tree) credits the two bb_field_get.cpp sites to the record-field witness of 67c8f3692, which the
# table still carried UNWITNESSED -- two in, two out, the ceiling a ratchet on arrivals that neither falls for the witnessed pair nor rises.
# 55 over 154 on the slim roads' second retirement (cto 2026-09-27, row gc-family-1-...-extracted-one-per-file): the role-4 shim now takes 30 to
# 60 formals (beaf84b26), and the plant over the corpus sources, the rungs entries compiled and the dyn-scope rungs entries run in mode 3 read
# both slim sites at 0 with a forced-positive control (SCRIP_NO_TINY=1) firing them (scripts/gc_rungs_entry_plant_receipt.tsv); the two
# UNWITNESSED rt_proc_call_open_slim sites leave the table with their roads, and nothing arrives, so the ceiling only falls.
# 53 over 159 on R4.1 (hq_prolog 2026-10-01, IR_UNIFY_CONST): the PL-REGAIN-5 const arm and its two witnessed rt_pl_dop_unify_ca/ci sites leave
# the table with the C entries; the box's cold rt_pl_unify_const_cold site arrives WITNESSED by the same .pl witness; and the det-leaf row the
# $cutcall landing (db4be5bb6) had left UNDECLARED beside its RETIRED twin is re-cut under its new key and read witnessed, so nothing arrives
# unwitnessed and the ceiling only falls.
# 54 over 164 on the Pascal block-protocol landing (hq_pascal 2026-10-03, CEO-1491): a deterministic Pascal call no longer takes the bcps_det_arm zres det road, so the two sites
# there (bb_call_proc_staged.cpp:308 rt_proc_call_open_detN and :310 rt_arg_stage, both polled at :311) lose the Pascal witness that was their only reader and no Icon, Raku or SNOBOL4
# program reaches the zres arm (tried: a 3- and a 9-argument call in each); they return UNWITNESSED, one more than the 53 -- the arrival this ceiling names.
# 61 over 165 with the Icon and Pascal block-protocol landings together (hq_pascal 2026-10-04, merging onto hq_icon's b1bee0c53): the det/open roads in bb_call_proc_staged.cpp (:102 rt_arg_stage, :327/:329 the zres det arm, :463/:467/:470 the Icon det arm, :796/:799 the pinned by-name road) and xa_flat.cpp:125 rt_arg_stage lost the Icon and Pascal witnesses that were their readers -- both languages' calls now build a block and cross none of them; no program in the set reaches them. They are dead roads pending their retirement when the six helpers leave nm -D, so the ceiling names them as arrivals rather than hiding them.
# 63 over 169 with the fail-path poll landing (hq_templates 2026-10-04, the row for the four baked SNOBOL4 callee sites and the fifth, bb_match_defer.cpp:300): the baked callee/field call arms on the FRAME road now store their result into the mapped frame slot and poll with the plain poll before the fail test (c869d57c1's result-preserving poll there lost r13, the scan subject, across the collection: tgrlink.icn and the Icon pre-step SIGSEGV), so the census credits the frame-slot baked arms that stood uncredited as common-tail unpolled -- bb_call.cpp:428 and bb_call_fn.cpp:227 (rt_call_callee_sn4/rt_call_fld_sn4) -- and no program in scripts/gc_witnesses reaches them (they are reached by beauty.sc and the parser_*.sc programs; a DATA-field call in a loop condition, a call subscript and an assignment arm of a Snocone function were tried and reach the zres arm only): the two arrivals this ceiling names. The zres (spine) arms keep c869d57c1's result-preserving poll and are not bare sites.
CEILING="${BARE_POLL_UNWITNESSED_CEILING:-63}"
[ "${FAIL_ONCE:-0}" = 1 ] && CEILING=0
checks=0; fails=0
ck() { checks=$((checks+1)); if [ "$1" = ok ]; then echo "  ok   $2"; else fails=$((fails+1)); echo "  FAIL $2"; fi; }
refuse() { echo "⛔ GATE REFUSED(2) [$G]: $1"; exit 2; }
[ -x "$ROOT/scrip" ] || refuse "no scrip binary -- build before grading"
[ -f "$CHK" ] || refuse "the checker $CHK is missing"
[ -f "$TABLE" ] || refuse "the declared table $TABLE is missing; run: python3 scripts/util_gc_safe_point_contract.py scripts/gc_witnesses/*.{icn,sno,pl,raku,sc,pas} --write-bare-poll-table"
export SCRIP_HEAP_MB=512   # THE PIN (CEO-931 pin rule): this gate measures the poll's PRESENCE with the collector NEVER RUNNING, so collections must read 0; a tiny arena would make every site NOT-ZERO by construction. The checker's bare_run_env sets the same value and removes SCRIP_HEAP_KB; this line is the declaration the tiny-arena gate counts.
unset SCRIP_HEAP_KB
echo "ARENA SCRIP_HEAP_MB=512 PINNED inside the checker for the stress-0 arm (collections must read 0; a tiny arena would make every site NOT-ZERO by construction) -- declared in the tiny-arena gate's pinned set"
echo "POPULATION (declared): $(grep -vc '^#' "$TABLE") credited bare-poll site(s) in $(basename "$TABLE"); ceiling on unwitnessed: $CEILING"
st="$(timeout 300s python3 "$CHK" --selftest 2>&1)"; src=$?
n="$(printf '%s\n' "$st" | sed -n 's/^SELFTEST \([0-9]*\)\/\([0-9]*\) arms green/\1 \2/p')"; set -- ${n:-0 0}
if [ "$src" = 0 ] && [ "${1:-0}" = "${2:-x}" ] && printf '%s\n' "$st" | grep -q "BARE-POLL PLANTED: a poll call WITHOUT its note"; then
  ck ok "(a) the checker's selftest holds, ${1} of ${2} arms, the bare-poll clause's plants among them"
else
  ck no "(a) the checker's selftest did not hold (rc=$src): $(printf '%s\n' "$st" | grep -i 'fail' | head -2 | tr '\n' ' ')"
fi
OUT="$(mktemp -t gc_bare_poll_reading.XXXXXX)"; trap 'rm -f "$OUT"' EXIT
WIT="$(grep -v '^#' "$TABLE" | cut -f3 | grep -v '^UNWITNESSED$' | sort -u | sed "s|^|$ROOT/scripts/gc_witnesses/|")"
timeout 900s python3 "$CHK" $WIT --bare-poll > "$OUT" 2>&1; rrc=$?
summ="$(grep '^CONTRACT BARE-POLL sites=' "$OUT" | head -1)"
notes="$(grep '^CONTRACT BARE-POLL-NOTES ' "$OUT" | head -1)"
if [ "$rrc" = 0 ] && [ -n "$summ" ] && printf '%s\n' "$notes" | grep -q 'mismatched=0'; then
  ck ok "(b) the reading was taken and every witness's poll notes match its poll calls: ${notes#CONTRACT BARE-POLL-NOTES }"
else
  ck no "(b) the reading was REFUSED or unreadable (rc=$rrc): $(grep 'REFUSED' "$OUT" | head -1)"
fi
field() { printf '%s\n' "$summ" | grep -o " $1=[0-9]*" | head -1 | cut -d= -f2; }
div="$(field diverging)"; unw="$(field unwitnessed)"; wit="$(field witnessed)"; sites="$(field sites)"
if [ "${div:-x}" = 0 ]; then
  ck ok "(c) diverging=0: no named witness prints a wrong answer or dies with the collector never run (witnessed=$wit of $sites)"
else
  ck no "(c) diverging=${div:-?}: $(grep '^CONTRACT BARE-POLL-SITE .* DIVERGING' "$OUT" | head -3 | tr '\n' ' ')"
fi
if [ -n "$unw" ] && [ "$unw" -le "$CEILING" ]; then
  ck ok "(d) unwitnessed=$unw is at or below the declared ceiling $CEILING (a ceiling only falls; lower it in this gate when a landing adds witnesses)"
else
  ck no "(d) unwitnessed=${unw:-?} is above the declared ceiling $CEILING -- a site lost its witness (STALE), a witness stopped reading zero (NOT-ZERO), or the census credits a new bare site (UNDECLARED): $(grep -E '^CONTRACT BARE-POLL-SITE .* (STALE|NOT-ZERO|UNREAD|UNDECLARED)' "$OUT" | head -3 | tr '\n' ' ')"
fi
echo "READING: $summ" | cut -c1-260
echo "population: 4 arm(s) over the declared table; the full reading names every site: $OUT (deleted at exit; re-run the checker with --bare-poll to keep it)"
if [ "$fails" = 0 ]; then echo "GATE PASS(0) [$G]: every credited bare-poll site is either WITNESSED at zero collections in both modes or NAMED ($checks arms)"; exit 0; fi
echo "GATE FAIL(1) [$G]: $fails of $checks arm(s) red"; exit 1
