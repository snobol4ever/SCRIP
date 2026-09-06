#!/usr/bin/env bash
# lib_master_extract.sh -- THE ONE way a script materializes a witness out of a language's MASTER flat suite
# (Lon 2026-08-29 one-flat-suite + zero-subfolders rulings: per-family suite files and the tests/snobol4
# subdirs are GONE; every absorbed test lives in tests/snobol4/ALL.{sno,ref,in,csv} side by side).
# A consumer that used to run tests/snobol4/probe/<fam>.sno or a loose pair now asks for entries BY ORIGIN
# (the CSV's provenance column, "<family>__<old-name>") or by CURRENT name, and gets standalone files back.
#   master_entry_for_origin <origin>                 -> prints the entry's CURRENT (descriptive) name
#   master_extract_origin  <origin> <out_src> [ref]  -> materializes one entry (+.ref) by origin
#   master_extract_name    <name>   <out_src> [ref]  -> same, by current name
#   master_origins_of_family <family>                -> every origin whose family column matches
# All lookups go through ALL.csv (never a second parser of the suite grammar); extraction goes through
# corpus_suite_harness.py extract (the ONE authority). Missing entry/master -> rc=2 REFUSAL, never silence.
# ⛔⭐ MULTI-LANGUAGE (seat03 2026-08-30, HALF-GENERALIZED-CONTRACT CHECK per corpus-suites-
# consolidation.task.md): the extension was hardcoded to .sno, so every function here silently
# assumed SNOBOL4 -- grepped this whole file for the old discriminator before adding MASTER_EXT,
# per that law. corpus_suite_harness.py's own extract/extract-family commands already detect the
# reader (line vs block) from the file SUFFIX, so the shell side only needed a suffix knob, not a
# second code path. Default stays ".sno" so all 21 existing SNOBOL4 callers are byte-identical;
# icon callers set MASTER_EXT=.icn (and MASTER_DIR=$S4E/corpus/tests/icon) before sourcing/calling.
# ⛔⛔⭐ INTERIM PROMOTION PROTOCOL -- READ THIS BEFORE YOU PROMOTE OR RETIRE AN XFAIL MARKER
# (ceo ruling 2026-09-01, routed here by hq_C; the row it was INTERIM until, `optbypass-pin-stable-subset`, is RETIRED.)
#
# ⛔⭐ A MARKER LIVES IN **FOUR** PLACES, NOT THREE -- THIS PARAGRAPH SAID THREE UNTIL 2026-09-06 AND THAT
# OMISSION HAS NOW COST TWO SEATS AND ONE BLOCKING RED ON ORIGIN:
#   (1) the banner in ALL.sno
#   (2) the SAME banner in ALL.ref
#   (3) the reason line in ALL.xfail
#   (4) the `xfail` COLUMN (field 6) of ALL.csv        <-- THE ONE THIS FILE USED TO OMIT
# read_suite compares the .sno and .ref banners and RAISES on a mismatch, so a half-applied promotion of
# (1)+(2) does not degrade -- it makes the master suite UNREADABLE. Measured 2026-09-01: corpus 2d75933ec
# dropped ' XFAIL' from ALL.sno and ALL.xfail but not ALL.ref, and every SNOBOL4 board and census on the
# box refused for ~40 minutes until 5eb68cb8 restored the pair.
# ⭐ WHY (4) IS THE DANGEROUS ONE, and why following this file exactly was not enough: (1)-(3) fail LOUDLY
# and immediately -- read_suite raises and names the torn seq. (4) fails SILENTLY in the promoting seat's
# root and reddens `test_gate_xfail_marker_and_index_agree` (BLOCKING) for everyone else. Measured
# 2026-09-06: hq_T promoted two markers in corpus f0262a113, ran read_suite on the result exactly as the
# rule below demands, got rc=0 over 1882 entries, and pushed -- leaving ALL.csv at xfail=1 and the gate
# red on origin at master=32 index=34 until hq_P landed 324c5926d. **THE PROOF THE RULE ASKED FOR WAS
# PERFORMED AND PASSED, AND THE PROMOTION WAS STILL INCOMPLETE**, because read_suite does not read ALL.csv.
# ⛔ A verification step that cannot see one of the places it is verifying will certify a half-done job.
#
#   THE RULE: a marker promotion is proven by running read_suite (or the board) ON THE RESULT **and** by
#   re-running test_gate_xfail_marker_and_index_agree.sh, in the SAME COMMIT -- never by the extract alone,
#   and never by read_suite alone, which is blind to place (4).
#

#
# ⭐ THE CHECK COSTS 0.046 s, SO THERE IS NO BUDGET EXCUSE FOR SKIPPING IT (hq_C, measured; the 400-650 s
# board is what people were actually skipping). `list` runs the same read_suite the board does:
#
#     python3 "$S4E_HOME/SCRIP/scripts/corpus_suite_harness.py" list \
#             "$S4E_HOME/corpus/tests/snobol4/ALL.sno" "$S4E_HOME/corpus/tests/snobol4/ALL.ref"
#
# PROVEN BOTH DIRECTIONS on the real tear (hq_C 2026-09-01, not asserted -- run):
#   torn pair  (at 2d75933ec) -> rc=1, "ValueError: family.ref banner mismatch at seq 1678:
#                                       sno='... 1678 user_function_indirect_replace_2'
#                                       ref='... 1678 user_function_indirect_replace_2 XFAIL'"
#   fixed pair (at 5eb68cb8)  -> rc=0, 1726 entries listed
# ⛔ Read the rc, not the tail: a wrong PATH to the harness also exits nonzero and looks identical at a
# glance ("can't open file ..."). That mistake was made while proving this very line.
#
# ✅⛔ THE BYPASS-ARM SECOND CHECK IS RETIRED (hq_T 2026-09-05, ruling R4 lane). It required every promotion to also run
# util_census_optimizer_bypass.py --only <entry> so a promoted entry could not silently push the optbypass watermark up.
# BOTH the tool and test_gate_optbypass_watermark.sh are DELETED: their subject, SCRIP_OPT=0 / SCRIP_ZD=0, was removed by
# Lon's ruling on 2026-09-03 (SCRIP ce199b05e, "the emergency optimizer bypass is gone, not merely retired"), so the arms
# measured the default arm under a different name. A promotion owes the read_suite/board proof above and NOTHING ELSE.
# ⭐ THE ONE LESSON WORTH CARRYING OUT OF IT, now in .github/GOAL-TEST-SUITE-CONSISTENCY.md: a CARRIED MAX watermark is an
# upper bound only under SHRINKAGE. Every entry promoted INTO a graded set brings its own verdict with it, so under GROWTH
# -- the direction this corpus actually moves -- a carried max is not conservative, it is wrong.
_ME_HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
_ME_S4E="${S4E_HOME:-$(cd "$_ME_HERE/../.." && pwd)}"
MASTER_DIR="${MASTER_DIR:-$_ME_S4E/corpus/tests/snobol4}"
MASTER_EXT="${MASTER_EXT:-.sno}"
MASTER_SNO="$MASTER_DIR/ALL$MASTER_EXT"
MASTER_REF="$MASTER_DIR/ALL.ref"
MASTER_CSV="$MASTER_DIR/ALL.csv"
_me_refuse() { echo "⛔ REFUSE(lib_master_extract): $*" >&2; return 2; }
# ⛔ DERIVE THE EXTENSION FROM MASTER_SNO, NEVER READ $MASTER_EXT DIRECTLY OUTSIDE THIS FILE'S OWN
# TOP LEVEL (seat03 2026-08-30). A caller's `MASTER_DIR=... MASTER_EXT=... source lib_master_extract.sh`
# only exports MASTER_EXT for the DURATION of that one `source` command -- MASTER_SNO/MASTER_DIR get
# baked in correctly (plain assignments made WHILE sourcing, so they persist as ordinary shell vars
# afterward), but a bare $MASTER_EXT read later, inside a function called on a SEPARATE line, sees it
# unset again and (under a caller's `set -u`) aborts. MASTER_SNO is always reliable; extract from it.
_me_ext() { local s="$MASTER_SNO"; printf '%s\n' ".${s##*.}"; }
master_entry_for_origin() {
    [ -f "$MASTER_CSV" ] || { _me_refuse "no master CSV at $MASTER_CSV"; return 2; }
    local n; n=$(awk -F, -v o="$1" '$3==o{print $2; exit}' "$MASTER_CSV")
    [ -n "$n" ] || { _me_refuse "origin '$1' not in $MASTER_CSV"; return 2; }
    printf '%s\n' "$n"
}
master_origins_of_family() {
    [ -f "$MASTER_CSV" ] || { _me_refuse "no master CSV at $MASTER_CSV"; return 2; }
    awk -F, -v f="$1" 'NR>1 && $4==f{print $3}' "$MASTER_CSV"
}
master_extract_name() {
    # ⛔⭐ ALWAYS REQUEST --out-in (row suite-harness-xfail-extract-round-trip, seat15 2026-08-30): the
    # harness now REFUSES (rc=3) to extract a stdin-bearing entry without it -- correctly, since a
    # stdin-starved SNOBOL4 program routinely still exits rc=0, so a caller that dropped the field got a
    # clean-looking run of a DIFFERENT witness than the one actually graded. Passing this unconditionally
    # costs nothing for a non-stdin entry (the harness only creates the file when e.stdin is not None) and
    # means every one of this function's 21 existing callers keeps working, transparently, with the correct
    # input from now on instead of a silent wrong answer.
    local name="$1" out="$2" ref="${3:-}"
    [ -f "$MASTER_SNO" ] && [ -f "$MASTER_REF" ] || { _me_refuse "master pair missing at $MASTER_SNO"; return 2; }
    local args=("$MASTER_SNO" "$MASTER_REF" "$name" "$out" --out-in "${out%.*}.in")
    [ -n "$ref" ] && args+=(--out-ref "$ref")
    # ⛔ THE OLD >/dev/null 2>&1 SWALLOWED THE HARNESS'S OWN REFUSAL REASON, leaving only a generic "failed"
    # -- exactly the message a caller would need to see to learn THIS entry needs --out-in in the first
    # place. Capture stderr, fold it into the refusal instead of discarding it.
    local err
    err=$(python3 "$_ME_HERE/corpus_suite_harness.py" extract "${args[@]}" 2>&1 >/dev/null) || { _me_refuse "extract '$name' failed: $err"; return 2; }
}
master_extract_origin() {
    local o="$1" out="$2" ref="${3:-}" n
    n=$(master_entry_for_origin "$o") || return 2
    master_extract_name "$n" "$out" "$ref"
}
master_origins_by_prefix() {
    # ⭐ seat03 2026-08-30: added for absorptions where each source file got its OWN family column
    # (one-file-per-family, e.g. a numbered audit battery) rather than one shared family across many
    # files -- master_origins_of_family can't group these, since there is no single family value to
    # match. Matches the CSV's origin column by a literal string PREFIX instead.
    [ -f "$MASTER_CSV" ] || { _me_refuse "no master CSV at $MASTER_CSV"; return 2; }
    awk -F, -v p="$1" 'NR>1 && index($3,p)==1{print $3}' "$MASTER_CSV"
}
master_extract_origin_prefix() {
    # materialize EVERY entry whose ORIGIN starts with a prefix as loose <old-name>$MASTER_EXT/.ref
    # pairs into a dir -- the prefix-matching sibling of master_extract_family, for the one-family-
    # per-file absorption shape (see master_origins_by_prefix above).
    local prefix="$1" dir="$2" o old rc=0
    mkdir -p "$dir" || return 2
    while IFS= read -r o; do
        [ -n "$o" ] || continue
        old="${o#*__}"
        master_extract_origin "$o" "$dir/$old$(_me_ext)" "$dir/$old.ref" || rc=2
    done < <(master_origins_by_prefix "$prefix")
    return $rc
}
master_extract_family() {
    # materialize EVERY entry of one origin-family as loose <old-name>.sno/.ref pairs into a dir --
    # the bridge for gates whose logic iterates per-file the way the old per-family trees did.
    local fam="$1" dir="$2" o n old rc=0
    mkdir -p "$dir" || return 2
    while IFS= read -r o; do
        [ -n "$o" ] || continue
        old="${o#${fam}__}"
        master_extract_origin "$o" "$dir/$old$(_me_ext)" "$dir/$old.ref" || rc=2
    done < <(master_origins_of_family "$fam")
    return $rc
}
