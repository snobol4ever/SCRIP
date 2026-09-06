#!/usr/bin/env bash
# lib_inventory.sh — THE ONE PACKAGE-INVENTORY BODY. Sourced by every package runner; never copied.
#
# ⭐ THE ORDER (Lon 2026-09-06, in-chat to ceo, verbatim: "Fix the never graded business. Let's lock
# down our package testing and make it complete."). Every program a vendored package SHIPS is either
# GRADED against the package's own oracle or NAMED UNGRADABLE with the oracle's reason. Never dropped,
# never counted as passed. The denominator is the SHIPPED population.
#
# ⛔⭐ WHAT THIS REPLACES, AND WHY A SINGLE `gap=` WAS THE DEFECT. Four runners already printed
# `shipped=N graded=N gap=N` (arizona, jcon, swi, aisnobol). `gap` is ONE bucket holding two things that
# must never be added together: "not graded YET" (work owed) and "CANNOT be graded" (a ruling, with the
# oracle's reason). ⛔ The lockdown's own criterion is `ungraded=0`, and while an ungradable program sits
# inside `gap` that criterion is UNREACHABLE BY BEING CORRECT — a lane can do every piece of real work
# available to it and still never reach the number. So `gap` splits, and the four buckets must SUM to
# shipped, which is what makes the line an inventory rather than four opinions.
#
# ⛔⭐⭐ THE FIFTH NUMBER, AND IT IS INVISIBLE PRECISELY BECAUSE IT LIVES INSIDE `graded` (hq_P 2026-09-06,
# from hq_B's measured case). The snoflake runner compares errors BY NUMBER — deliberately, documented,
# and correctly, since error TEXT cannot match byte-for-byte across implementations. Consequence:
# gimpel's topological-sort scored PASS FOR MONTHS while SCRIP printed its refusal at LINE 0 STATEMENT 0
# and printed it TWICE, against an oracle that prints it once at line 20 statement 5 and HALTS. Same
# integer, three defects. Such a program is NOT ungraded (it ran, it was compared, it matched) and NOT
# honestly graded either (the comparison was narrower than the claim the percent makes). Collapsed into
# `graded`, it lets a lane reach `ungraded=0` with a wrong program still running.
#
# ⭐ SO THE BUCKET IS NAMED FOR THE PROPERTY, NOT THE INSTANCE — hq_T's call, adopted by hq_P over their
# own ask, and the reason is on the record because it is the whole design: a bucket named
# `graded_errno_only` would silently grade every OTHER narrow instrument as an honest pass, and the next
# narrow comparison would land in `graded` and be rediscovered in three weeks with a different integer.
# `graded_narrow` = THE COMPARISON WAS NARROWER THAN THE CLAIM THE PERCENT MAKES. Error-number equality
# is member one, not the definition. Known siblings: grading rc alone with output to /dev/null (hq_I),
# and — hq_P's purest form — AN INSTRUMENT WHOSE SENSITIVITY IS BELOW THE EFFECT IT WAS BUILT TO DETECT
# IS NOT A WEAK TEST, IT IS A TEST THAT CANNOT FAIL, AND IT PRINTS THE SAME STRING AS ONE THAT PASSED
# (a leak check at 200,000 iterations: 1.6MB against an 8MB stack, passing forever).
#
# ⛔ AND EVERY NARROW GRADING NAMES ITSELF PER ENTRY, in NARROW.tsv. hq_P's sentence is the rule:
# AN UNEXPLAINED BUCKET IS JUST A SMALLER LIE. A count with no per-entry reason cannot be acted on and
# cannot be falsified, which is the same failure one size down.
#
# THE LINE, one shape, from every package runner:
#   PACKAGE_INVENTORY package=<name> shipped=N graded=N ungraded=N ungradable=N graded_stream=N graded_narrow=N
#
# THE STANZA a runner writes (four tokens, same shape as lib_port_trace.sh's):
#   INV_PACKAGE=gimpel ; INV_DIR="$CORPUS/packages/snobol4/gimpel" ; INV_EXT=".sno"
#   inventory_line "$STREAM_PASSES" "$NARROW_PASSES"
#
# THE SIDECARS, beside the package (or one directory below it, as ipl/progs/UNGRADED.tsv already sits):
#   UNGRADED.tsv    name<TAB>CLASS<TAB>reason   work owed: why it is not graded YET
#                   name is the PACKAGE-RELATIVE PATH (a bare name only while it is unique in the tree)
#   UNGRADABLE.tsv  name<TAB>CLASS<TAB>reason   a ruling: the ORACLE's reason it cannot be graded
#   NARROW.tsv      name<TAB>NARROWING<TAB>reason   graded, but by a comparison narrower than the claim
#
# ⛔ ALL THREE ARE DECLARATIONS, NEVER INFERENCES. A runner may not decide at run time that something is
# ungradable; that is a ruling somebody makes once, in a file, with a reason a later reader can dispute.

# ⛔⭐⭐ THE REASON-CODE VOCABULARY IS CLOSED (hq_T ruling 2026-09-06, asked for by hq_I: three lanes had
# invented three vocabularies in one morning -- CONTAINER_OR_LIBRARY (hq_I, seat03), NO-ORACLE-SHIPPED and
# ORACLE-IS-NOT-PROGRAM-OUTPUT (corpus a1623cf8f), EMPTY and ORACLE_FAIL (ipl/progs) -- in dashes and in
# underscores, for the same handful of situations).
#
# ⭐ THE RULING IS NOT MAINLY A SPELLING. It is that THE TWO FILES ANSWER TWO DIFFERENT QUESTIONS, so their
# class columns are different vocabularies and a class may not drift between them:
#   UNGRADABLE names WHAT THE ORACLE OR THE PACKAGE DID  -- a ruling, disputable, nobody owes work.
#   UNGRADED   names WHAT IS OWED                        -- a task, claimable, somebody can finish it.
# ⛔ So an UNGRADED class that names an OBSERVATION rather than a task is the defect one size down from
# ARM 8's. `EMPTY` is the measured case: 106 ipl rows say `EMPTY` in the class and *"needs a .in with
# representative input, not yet authored"* in the reason -- the file already knows what is owed, and the
# column a reader sorts by does not. A lane cannot pick up 106 rows of `EMPTY`; it can pick up 106 rows of
# NEEDS_STDIN_FIXTURE. Hence the respellings in INV_CLASS_LEGACY, which is DEBT, listed so it shrinks.
#
# ⭐ Spelling is UPPER_SNAKE because four of the five vocabularies already were; the dashed spellings are
# the minority, and a mixed convention means a grep for a class silently answers over a subset.
INV_CLASS_UNGRADABLE="CONTAINER_OR_LIBRARY NO_ORACLE_SHIPPED ORACLE_REFUSES ORACLE_CONTRACT_NOT_IMPLEMENTED NEEDS_INPUT NEEDS_INTERACTIVE_TTY NONDETERMINISTIC"
INV_CLASS_UNGRADED="NEEDS_STDIN_FIXTURE NEEDS_ARGV_FIXTURE NEEDS_RUNNER_WIRING REF_NOT_CUT ORACLE_FAIL TIMEOUT NONDETERMINISTIC LIVE SUSPECT_USAGE"
# LEGACY: accepted with a WARN so no lane's board breaks on the ruling, and named so the debt is one sed
# wide and visible. ⛔ Do not add to this list -- a new file uses the closed vocabulary above.
INV_CLASS_LEGACY="EMPTY:NEEDS_STDIN_FIXTURE GRADABLE:REF_NOT_CUT NO-ORACLE-SHIPPED:NO_ORACLE_SHIPPED NO-ORACLE:NO_ORACLE_SHIPPED ORACLE-IS-NOT-PROGRAM-OUTPUT:NO_ORACLE_SHIPPED NEEDS_MULTIFILE_LINK:NEEDS_RUNNER_WIRING"
# ⭐⭐ THE LEGACY MAP EARNED ITS KEEP WITHIN THE HOUR, AND ON A CASE THE RULING GOT WRONG, NOT AN OLD ONE.
# hq_I cured jcon concurrently with this ruling landing and wrote NEEDS_MULTIFILE_LINK -- a good, precise
# name for a real situation, invented in good faith minutes after the vocabulary closed. A bare "unruled
# classes refuse" would have red-lit a lane delivering exactly the work Lon had just ordered, and the lane
# would have been right to route around the ruling. ⭐ THE RULING STANDS AND THE FILE STILL PASSES: the
# CLASS is the bucket a lane sorts and picks up work by, so it stays coarse (NEEDS_RUNNER_WIRING covers
# "the runner cannot invoke it the way the package's own harness does" -- a two-file compile and a -L
# preload are the same pickup), and the REASON column carries the specificity, where it costs nothing. A
# vocabulary that grows a member per situation is the every-lane-invents-a-spelling problem wearing a
# tidier hat. ⛔ So the map is not only for pre-ruling debt: it is how a closed vocabulary absorbs a
# collision without stopping anyone, which is the only reason closing one is safe to do mid-flight.

inventory_refuse() { echo "⛔ INVENTORY REFUSES(2): $*" >&2; return 2; }

# _inv_class_check <kind> <file> <line-no> <class> -- rc 0 known, 1 legacy (warned), 2 unknown (refused).
_inv_class_check() {
    local kind="$1" f="$2" n="$3" cls="$4" known pair
    case "$kind" in
        UNGRADABLE) known="$INV_CLASS_UNGRADABLE" ;;
        UNGRADED)   known="$INV_CLASS_UNGRADED" ;;
        *)          return 0 ;;   # NARROW's narrowing vocabulary is hq_P's to close; format only here.
    esac
    case " $known " in *" $cls "*) return 0 ;; esac
    for pair in $INV_CLASS_LEGACY; do
        [ "${pair%%:*}" = "$cls" ] || continue
        echo "⚠ INVENTORY LEGACY CLASS: $f line $n uses '$cls'; the ruled $kind class is '${pair#*:}' (hq_T 2026-09-06). Accepted, not correct -- one sed from gone." >&2
        return 1
    done
    inventory_refuse "$f line $n: '$cls' is not a $kind class. The vocabulary is CLOSED (hq_T 2026-09-06) because three lanes invented three spellings for the same handful of situations in one morning:
    $kind = $known
    ⛔ Note which question this file answers: UNGRADABLE names what the ORACLE did (a ruling, nobody owes work); UNGRADED names what is OWED (a task somebody can pick up). A class naming an observation in UNGRADED cannot be worked, and a class naming our own work in UNGRADABLE is ARM 8's defect."
    return 2
}

# _inv_tsv <basename> -- echo the path of a sidecar beside INV_DIR or one level below it, or nothing.
_inv_tsv() {
    local b="$1" c
    for c in "$INV_DIR/$b" "$INV_DIR"/*/"$b"; do
        [ -f "$c" ] && { echo "$c"; return 0; }
    done
    return 0
}

# _inv_names <tsv> <kind> -- echo the name column, refusing a malformed, unexplained or misclassed row.
_inv_names() {
    local f="$1" kind="${2:-}" n=0
    [ -n "$f" ] || return 0
    while IFS= read -r line; do
        n=$((n + 1))
        case "$line" in ''|'#'*) continue ;; esac
        # ⛔ THREE COLUMNS, AND THE REASON MAY NOT BE EMPTY. A declaration with no reason is the shape
        # that turns an inventory back into a number nobody can act on.
        local name class reason
        name="$(printf '%s' "$line" | cut -f1)"
        class="$(printf '%s' "$line" | cut -f2)"
        reason="$(printf '%s' "$line" | cut -f3-)"
        if [ -z "$name" ] || [ -z "$class" ] || [ -z "${reason// /}" ]; then
            inventory_refuse "$f line $n: expected name<TAB>CLASS<TAB>reason, got: $line" || return 2
        fi
        _inv_class_check "$kind" "$f" "$n" "$class"; [ "$?" -eq 2 ] && return 2
        printf '%s\n' "$name"
    done < "$f"
}

inventory_line() {
    local graded_stream="${1:-0}" graded_narrow="${2:-0}"
    [ -n "${INV_PACKAGE:-}" ] || { inventory_refuse "INV_PACKAGE unset -- the stanza is incomplete"; return 2; }
    [ -d "${INV_DIR:-}" ]     || { inventory_refuse "INV_DIR '${INV_DIR:-}' is not a directory -- the package moved; re-pin rather than reinterpret"; return 2; }
    [ -n "${INV_EXT:-}" ]     || { inventory_refuse "INV_EXT unset -- cannot census a shipped population without knowing its extension"; return 2; }

    # ⛔⭐ SHIPPED IS COUNTED BY EXTENSION OVER THE WHOLE PACKAGE TREE, never by the one directory anyone
    # had in mind. This is CLAUDE.md's own most-reused lesson (census by extension, never by the one you
    # had in mind) applied to the population that the entire lockdown percentage divides by.
    local shipped=0 e f rel
    local -A _rel=() _base=()
    for e in $INV_EXT; do
        while IFS= read -r f; do
            rel="${f#"$INV_DIR"/}"
            # ⛔⭐ OUR OWN GENERATED MASTER IS NOT A SHIPPED PROGRAM. Four packages carry an `ALL.<ext>`
            # concatenated master beside the vendor's files (ipl, aisnobol, csnobol4_suite, gimpel,
            # measured 2026-09-06). Counting it inflates `shipped` by exactly one and makes the SUM
            # unreachable by one FOREVER -- a lane grades every vendor program and still refuses.
            # ⭐ It is also the whole of the 851-vs-852 disagreement between hq_I's Icon runner (which
            # excludes ALL.icn) and this body (which did not): two correct instruments answering two
            # different questions, and neither able to say so.
            case "${f##*/}" in ALL.*) continue ;; esac
            _rel["$rel"]=1
            _base["${f##*/}"]=$(( ${_base["${f##*/}"]:-0} + 1 ))
            shipped=$((shipped + 1))
        done < <(find "$INV_DIR" -type f -name "*$e" 2>/dev/null)
    done
    [ "$shipped" -gt 0 ] || { inventory_refuse "zero files matching '$INV_EXT' under $INV_DIR -- an empty shipped population is not an inventory, and a percent over zero is not a score"; return 2; }

    local ung_f ugd_f nar_f
    ung_f="$(_inv_tsv UNGRADED.tsv)";   ugd_f="$(_inv_tsv UNGRADABLE.tsv)"; nar_f="$(_inv_tsv NARROW.tsv)"
    local ung_n ugd_n
    ung_n="$(_inv_names "$ung_f" UNGRADED)" || return 2
    ugd_n="$(_inv_names "$ugd_f" UNGRADABLE)" || return 2
    local ungraded ungradable
    ungraded=$(printf '%s' "$ung_n"   | grep -c . || true)
    ungradable=$(printf '%s' "$ugd_n" | grep -c . || true)

    # ⛔⭐⭐ AN UNGRADABLE DECLARATION MUST GIVE THE ORACLE'S REASON, NEVER OURS -- and this is the arm
    # that stops the lockdown from being satisfiable by failing. hq_C measured it on prolog/swi_tests
    # 2026-09-06: EXCLUDED.md names 240 programs as not graded and 240 OF 240 GIVE A SCRIP-SIDE REASON
    # ("scrip produces zero PASS/FAIL/EMPTY lines for this file today"), ZERO give an oracle-side one.
    # ⛔ A PROGRAM EXCLUDED BECAUSE OUR OWN COMPILER FAILS IT IS A RED MOVED OUT OF THE DENOMINATOR: the
    # score cannot fall when we fail, because failing is what removes the entry. Against the real oracle
    # only FIVE of 170 were genuinely ungradable, all oracle-side (swipl aborts, SIGSEGVs, times out).
    # ⭐ AND NOTE WHY NOBODY CAUGHT IT, because it is the reason this must be a machine check rather than
    # a review habit: every one of those 240 entries is individually honest and well documented, naming
    # file, rung and exact error. 240 carefully-written TRUE notes compose into a denominator that cannot
    # fall. Diligence at the entry level is exactly what makes the aggregate invisible.
    if [ -n "$ugd_f" ]; then
        local badreason
        badreason="$(grep -inE '(^|[^a-z])(scrip|our compiler|m3|m4|mode-3|mode-4|rung [0-9])([^a-z]|$)' "$ugd_f" | head -3 || true)"
        if [ -n "$badreason" ]; then
            inventory_refuse "$ugd_f names OUR OWN COMPILER as the reason a program cannot be graded:
$badreason
    ⛔ UNGRADABLE is a statement about the ORACLE, never about us. A program excluded because SCRIP fails it is a RED MOVED OUT OF THE DENOMINATOR -- the score cannot fall when we fail, because failing is what removes the entry. If the oracle grades it and we do not, it is GRADED and RED. Move it, or give the oracle's own reason.
    ⭐ AND A ROW CITING **BOTH** IS STILL REFUSED, deliberately -- measured on arizona general/tpp.icn, whose reason gives \"TWO reasons this is ungradable here\": one oracle-side (tpp.ok was never vendored) and one ours (no preprocessor-only mode). The oracle-side half is sufficient on its own, so state it on its own; while our half is written beside it a reader cannot tell whether the ruling would survive without it, and that is the whole property this arm protects."
            return 2
        fi
    fi

    # ⛔ A NAME MAY NOT BE IN BOTH FILES. "Work owed" and "ruled impossible" are contradictory claims
    # about one program, and whichever the reader saw first would decide whether the lane owes work.
    local both
    both="$(printf '%s\n%s\n' "$ung_n" "$ugd_n" | grep -v '^$' | sort | uniq -d)"
    [ -z "$both" ] && : || { inventory_refuse "named in BOTH UNGRADED.tsv and UNGRADABLE.tsv: $(printf '%s' "$both" | tr '\n' ' ')-- a program cannot be both work owed and ruled impossible"; return 2; }

    # ⛔ A DECLARATION NAMING NOTHING SHIPPED IS STALE -- the same refusal the wantrc/xfail sidecars make.
    # A stale line silently shrinks `graded` and inflates the lane's apparent debt, or hides a real one.
    local nm miss="" ambig=""
    for nm in $ung_n $ugd_n; do
        if [ -n "${_rel["$nm"]:-}" ]; then continue; fi
        case "${_base["$nm"]:-0}" in
            0) miss="$miss $nm" ;;
            1) : ;;
            *) ambig="$ambig $nm(${_base["$nm"]})" ;;
        esac
    done
    [ -z "$ambig" ] || { inventory_refuse "declared by a BARE NAME that names more than one shipped file:$ambig -- write the package-relative path instead. A bare name is only a name while it is unique; ipl ships four such collisions (gener/morse/repeats/spokes under both procs/ and progs/), so the ruling would land on whichever file the census happened to reach first."; return 2; }
    [ -z "$miss" ] || { inventory_refuse "declared but not shipped:$miss -- a declaration matching no file is a rename or a leftover, and both withdraw a guarantee silently"; return 2; }

    # ⛔ AN UNEXPLAINED NARROW BUCKET IS JUST A SMALLER LIE (hq_P). A count with no per-entry narrowing
    # cannot be acted on and cannot be disputed.
    if [ "$graded_narrow" -gt 0 ]; then
        [ -n "$nar_f" ] || { inventory_refuse "graded_narrow=$graded_narrow but no NARROW.tsv beside $INV_DIR -- name which narrowing applies to which entry, or do not claim the bucket"; return 2; }
        local nar_n; nar_n="$(_inv_names "$nar_f" NARROW)" || return 2
        local nar_c; nar_c=$(printf '%s' "$nar_n" | grep -c . || true)
        [ "$nar_c" -eq "$graded_narrow" ] || { inventory_refuse "graded_narrow=$graded_narrow but NARROW.tsv names $nar_c entr(y/ies) -- the count and the explanation must be the same population"; return 2; }
    fi

    # ⛔⭐ THE FOUR BUCKETS MUST SUM TO SHIPPED. This is the whole load-bearing property: without it the
    # line is four independent opinions, each individually plausible, and a program can fall out of every
    # bucket -- which is the "never graded" defect the order exists to end, wearing an inventory's clothes.
    local graded=$((graded_stream + graded_narrow))
    local total=$((graded + ungraded + ungradable))
    if [ "$total" -ne "$shipped" ]; then
        inventory_refuse "buckets do not sum: graded($graded)=stream($graded_stream)+narrow($graded_narrow) + ungraded($ungraded) + ungradable($ungradable) = $total, but shipped=$shipped (delta $((shipped - total))). Every shipped program lands in exactly one bucket, or the inventory is four opinions rather than a census."
        return 2
    fi
    echo "PACKAGE_INVENTORY package=$INV_PACKAGE shipped=$shipped graded=$graded ungraded=$ungraded ungradable=$ungradable graded_stream=$graded_stream graded_narrow=$graded_narrow"
    return 0
}
