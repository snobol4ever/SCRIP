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
_ME_HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
_ME_S4E="${S4E_HOME:-$(cd "$_ME_HERE/../.." && pwd)}"
MASTER_DIR="${MASTER_DIR:-$_ME_S4E/corpus/tests/snobol4}"
MASTER_SNO="$MASTER_DIR/ALL.sno"
MASTER_REF="$MASTER_DIR/ALL.ref"
MASTER_CSV="$MASTER_DIR/ALL.csv"
_me_refuse() { echo "⛔ REFUSE(lib_master_extract): $*" >&2; return 2; }
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
    local args=("$MASTER_SNO" "$MASTER_REF" "$name" "$out" --out-in "${out%.sno}.in")
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
master_extract_family() {
    # materialize EVERY entry of one origin-family as loose <old-name>.sno/.ref pairs into a dir --
    # the bridge for gates whose logic iterates per-file the way the old per-family trees did.
    local fam="$1" dir="$2" o n old rc=0
    mkdir -p "$dir" || return 2
    while IFS= read -r o; do
        [ -n "$o" ] || continue
        old="${o#${fam}__}"
        master_extract_origin "$o" "$dir/$old.sno" "$dir/$old.ref" || rc=2
    done < <(master_origins_of_family "$fam")
    return $rc
}
