#!/bin/bash
# test_gate_suite_conversion_complete.sh <lang> -- ONE DONE-WHEN authority for the six tests-consolidate-* rows.
#
# ⭐ WHY THIS EXISTS (hq_C ruling 2026-08-27, on seat02's question, corroborated by seat14's measurement).
# The fan-out rows shipped with two independent defects in their DONE-WHENs, and each is trap #1 -- a criterion
# that can never say YES:
#   (1) FOUR of the six (pascal/prolog/rebus/snocone) test `find tests/<lang> -name '*.<ext>' | wc -l -eq 0`.
#       But the SETTLED architecture puts converted suite output INSIDE tests/<lang>/ WITH THE REAL EXTENSION --
#       live proof: corpus/tests/snobol4/crosscheck/patterns.sno, 44KB, pushed. So a successful conversion makes
#       that count go UP, and the criterion is unsatisfiable BY DOING THE WORK CORRECTLY. Measured at ruling
#       time: `find tests/snobol4 -name '*.sno'` = 187, against a criterion demanding 0.
#   (2) The one row that DID exclude the suite subdir hardcoded a keeper allowance (`-le 12`). seat14 then
#       measured the real number: 34 -- all of beauty_suite is STANDALONE-KEEP. A guessed keeper count is a
#       second unsatisfiable criterion wearing a fix's clothing; the keeper set is DISCOVERED by doing the work,
#       so it cannot be a constant known when the row is minted.
#
# THE CRITERION THIS IMPLEMENTS, which needs no guess: every loose program file is EITHER converted (gone from
# the loose tree) OR explicitly declared a keeper in a KEEP.md. Same three-bucket shape as the corpus coverage
# manifest -- converted / declared-kept / nothing-falls-through -- and it is the declaration, not a number, that
# carries the judgment. Do NOT "fix" a red by raising an allowance; declare the file and say why.
#
# ⭐⭐ THE FOURTH BUCKET: DEFERRED-TO-ANOTHER-ROW (PENDING.md) -- hq_B 2026-08-29, on hq_P's catch.
# A ROW-scoped split against a TREE-scoped gate cannot ever go green. tests-consolidate-prolog was split and its
# genuinely-blocked third carved out into tests-consolidate-prolog-pz4-blocked-33; those files are STILL PHYSICALLY
# IN THE TREE, so this gate counts them and the parent's DONE-WHEN is unreachable no matter how much of the
# parent's OWN work is finished. The split bought an honest FREE parent with workable items, not a reachable gate.
# ⛔ KEEP.md IS THE WRONG INSTRUMENT FOR THEM AND USING IT WOULD BE A LIE (hq_P's point, and it is the crux):
# these files are PENDING, not keepers. A keeper declaration says "this stays loose forever, on purpose"; these
# are "this converts later, when a named row unblocks". Filing the second as the first is how a deferral becomes
# permanent silently -- an allowance wearing a declaration's clothes, which is the exact defect above.
# ⛔ SO THE DEFERRAL MUST EXPIRE BY ITSELF, and that is the whole design: a PENDING.md names the OWNING ROW, and
# this gate RESOLVES that row against the live queue every run. Row missing => FAIL (dangling). Row already DONE
# => FAIL (the deferral outlived its reason; convert them now). Only a LIVE row defers. Modelled directly on
# s4e_msg.sh's BLOCKED-ON:/PARKED-AWAITING: self-clearing and its dangling-blocker refusal (2eaca909) -- a block
# recorded against a row that no longer exists is not a block, it is a hiding place.
# ⛔ AND A DEFERRED FILE NEVER READS AS CONVERTED. The verdict line says the tree is NOT fully consolidated and
# names the blocking row(s); this gate certifies THIS ROW'S SCOPE, never the tree's completeness. If the queue
# cannot be read at all, deferrals cannot be verified and the gate REFUSES rc=2 rather than trusting them.
#
# REFUSES rc=2 when it cannot measure -- never skip-as-success.
set -u
LANG_DIR="${1:-}"
[ -n "$LANG_DIR" ] || { echo "REFUSES rc=2: usage: $0 <lang>   (pascal|prolog|rebus|snocone|snobol4|raku|icon)"; exit 2; }
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"
TREE="$S4E/corpus/tests/$LANG_DIR"
[ -d "$TREE" ] || { echo "REFUSES rc=2: no such tree: $TREE"; exit 2; }
case "$LANG_DIR" in
  snobol4) EXT='*.sno';;  icon) EXT='*.icn';;   prolog) EXT='*.pl';;
  snocone) EXT='*.sc';;   rebus) EXT='*.reb';;  raku) EXT='*.raku';;  pascal) EXT='*.pas';;
  *) echo "REFUSES rc=2: unknown language '$LANG_DIR' -- add its extension deliberately rather than defaulting"; echo "   to a glob that might match nothing and read as success."; exit 2;;
esac
# ⭐ A converted suite file is EXEMPT WHEREVER IT LIVES, not only under crosscheck/. Two destination conventions
# are in live use -- tests/<lang>/crosscheck/<family>.<ext> (the SNOBOL4 shape) and tests/<lang>/<family>.<ext>
# (the parser-family shape, e.g. tests/raku/parser.raku, tests/rebus/parser.reb) -- and the first draft of this
# gate only knew the first. It therefore counted FINISHED SUITE OUTPUT as unconverted work: raku reported 104
# undeclared on an ALREADY-CONVERTED row. Reported by seat01 2026-08-27; a false red, and the same class of
# defect this gate exists to catch.
# ⛔ The exemption is CONTENT-BASED, deliberately, not a second hand-maintained list of family names or paths:
# a converted file opens with a numbered banner block (comment leader + rule + index + entry name), which the
# harness emits for every language -- "#--- 1 arr_get", "*--- 1 038_pat_literal". Validated at write time to
# match all three live conventions and none of the ordinary loose programs it was tested against.
is_suite_output() {
    head -1 "$1" 2>/dev/null | grep -qE '^[^A-Za-z0-9]*-{10,}[[:space:]]+[0-9]+[[:space:]]+[A-Za-z0-9_]'
}
mapfile -t ALLF < <(find "$TREE" -type f -name "$EXT" -not -path '*/crosscheck/*' 2>/dev/null | sort)
LOOSE=(); SUITEOUT=0
for _f in "${ALLF[@]}"; do
    if is_suite_output "$_f"; then SUITEOUT=$((SUITEOUT+1)); else LOOSE+=("$_f"); fi
done
TOTAL_ANY=$(find "$TREE" -type f -name "$EXT" 2>/dev/null | wc -l)
if [ "$TOTAL_ANY" -eq 0 ]; then
    echo "REFUSES rc=2: zero '$EXT' files anywhere under $TREE -- the extension or the tree moved, so this gate"
    echo "   has gone vacuous. A gate that matches nothing is not a green gate."
    exit 2
fi
echo "suite conversion completeness -- $LANG_DIR -- $(date -u +%Y-%m-%dT%H:%M:%SZ)"
echo "tree: $TREE   pattern: $EXT   total: $TOTAL_ANY   converted suite output: $SUITEOUT   loose: ${#LOOSE[@]}"
[ "${#LOOSE[@]}" -eq 0 ] && { echo "GATE OK -- nothing loose; every $EXT file lives in the governed suite tree."; exit 0; }
# --- the deferral bucket -----------------------------------------------------------------------------------
PO="${S4E_POST:-/home/resources/postoffice}"
# Resolve a row name against the fleet queue. Echoes LIVE | EXPIRED | DANGLING | UNVERIFIABLE.
# ⛔ UNVERIFIABLE is NOT a pass -- the caller turns it into rc=2, per this gate's own no-skip-as-success rule.
pending_row_state() {
    local _row="$1" _q="$PO/QUEUE.tsv" _d="$PO/QUEUE.done.tsv" _c="$PO/claims/$1.claim"
    [ -r "$_q" ] || { echo UNVERIFIABLE; return; }
    if [ -f "$_c" ] && grep -q '^DONE$' "$_c" 2>/dev/null; then echo EXPIRED; return; fi
    if [ -r "$_d" ] && grep -qP "^[0-9]+\t\Q$_row\E\t" "$_d" 2>/dev/null; then echo EXPIRED; return; fi
    if grep -qP "^[0-9]+\t\Q$_row\E\t" "$_q" 2>/dev/null; then echo LIVE; return; fi
    echo DANGLING
}
# The ROW a PENDING.md speaks for: the first `ROW: <topic>` line. One row per PENDING.md, deliberately --
# a file that needs two owners has not been split yet, and inventing a per-entry syntax here would re-create
# the prose-parsing surface the KEEP.md matcher was just cured of.
pending_row_of() { sed -n 's/^[[:space:]]*ROW:[[:space:]]*\([A-Za-z0-9_.-]\{1,\}\).*$/\1/p' "$1" 2>/dev/null | head -1; }
mapfile -t KEEPFILES < <(find "$TREE" -type f -name 'KEEP.md' 2>/dev/null)
# ⛔⭐⭐ THE DECLARATION CHECK WAS A RAW SUBSTRING TEST OVER EVERY KEEP.md CONCATENATED, AND IT UNDER-COUNTED
# (seat14 2026-08-29, measured; cured here by hq_P). The old form was:
#     DECLARED=$(cat "${KEEPFILES[@]}"); case "$DECLARED" in *"$b"*) : ;; ... esac
# — two independent defects on one line, BOTH of which make the gate report FEWER undeclared files than exist,
# i.e. they push it toward FALSE GREEN:
#   (1) SCOPE WAS DESTROYED. Every KEEP.md in the tree was concatenated, so a KEEP.md in one subdirectory
#       silently declared same-named files anywhere else. MEASURED: writing tests/prolog/KEEP.md to rule on
#       ONE file (plunit.pl) dropped loose-but-undeclared by THREE — frontend/plunit.pl was declared by
#       accident purely through a basename collision.
#   (2) THE MATCH WAS AN UNDELIMITED SUBSTRING, so `foo.pl` matches inside `myfoo.pl`, and any KEEP.md that
#       merely MENTIONS another file's name — to contrast it, exclude it, or cross-reference it, which is a
#       normal and good thing to write in prose — silently declared that file too.
# ⭐ THE CURE KEEPS THE PROSE FORMAT (every real KEEP.md is prose with backticked names; a structured-list
# requirement would invalidate all of them). Instead: a KEEP.md may only declare files in ITS OWN DIRECTORY
# or below it, and the basename must appear DELIMITED — bounded by a non-filename character on each side.
# ⛔ Prose mention WITHIN the owning directory still counts as a declaration; that surface is deliberately
# left, because narrowing it further needs a format change and would break every existing KEEP.md. The
# declaring file is now PRINTED for each kept file, so an accidental declaration is visible instead of silent.
# ⭐ AMBIGUITY IS THE ACTUAL TEST, not path depth. Two rules were tried and BOTH were wrong:
#   bare basename anywhere in any KEEP.md  -> seat14's measured false positive (a root KEEP.md naming
#                                             `plunit.pl` silently declared frontend/plunit.pl too);
#   path relative to the KEEP.md, always   -> a FALSE RED on 66 real files: tests/snocone/ladder/KEEP.md
#                                             legitimately declares ladder/prog/*.sc by bare basename, and
#                                             there is nothing ambiguous about it.
# ⛔ What made seat14's case wrong was not the bare name, it was that the bare name was AMBIGUOUS — two loose
# files shared it and only one was meant. So: a bare basename declares a file only when that basename is
# UNIQUE among the loose files; when two or more share it, the declaration must be written as the path
# relative to the KEEP.md. Unambiguous trees keep working untouched; ambiguous ones must say which they mean.
declare -A _BNC=()
for f in "${LOOSE[@]}"; do _b=$(basename "$f"); _BNC["$_b"]=$(( ${_BNC["$_b"]:-0} + 1 )); done
UND=0; UNDLIST=""; DECLBY=""; AMBIG=0
DEF=0; DEFLIST=""; DEFROWS=""; PBAD=0; PBADLIST=""; PUNV=0; PUNVLIST=""
for f in "${LOOSE[@]}"; do
    b=$(basename "$f"); d=$(dirname "$f"); found=""; via=""
    probe="$d"
    while : ; do
        k="$probe/KEEP.md"
        if [ -f "$k" ]; then
            rel=${f#$probe/}; rre=${rel//./[.]}
            if grep -qE "(^|[^A-Za-z0-9_./-])$rre([^A-Za-z0-9_-]|$)" "$k" 2>/dev/null; then found="$k"; via="path"; break; fi
            if [ "${_BNC[$b]}" -eq 1 ]; then
                bre=${b//./[.]}
                if grep -qE "(^|[^A-Za-z0-9_./-])$bre([^A-Za-z0-9_-]|$)" "$k" 2>/dev/null; then found="$k"; via="name"; break; fi
            fi
        fi
        [ "$probe" = "$TREE" ] && break
        parent=$(dirname "$probe"); [ "$parent" = "$probe" ] && break; probe="$parent"
    done
    if [ -n "$found" ]; then DECLBY="$DECLBY\n     ${f#$TREE/}  <-  ${found#$TREE/} (by $via)"
    else
        # Not a keeper. Before calling it undeclared, ask whether a PENDING.md defers it to a live row --
        # SAME directory-scoped walk and SAME delimited/unambiguous matching rules as KEEP.md above, because
        # a second, laxer matcher here would reintroduce exactly the two defects hq_P cured in that one.
        pfound=""; pvia=""; prow=""
        pprobe="$d"
        while : ; do
            pk="$pprobe/PENDING.md"
            if [ -f "$pk" ]; then
                prel=${f#$pprobe/}; prre=${prel//./[.]}
                if grep -qE "(^|[^A-Za-z0-9_./-])$prre([^A-Za-z0-9_-]|$)" "$pk" 2>/dev/null; then pfound="$pk"; pvia="path"; fi
                if [ -z "$pfound" ] && [ "${_BNC[$b]}" -eq 1 ]; then
                    pbre=${b//./[.]}
                    grep -qE "(^|[^A-Za-z0-9_./-])$pbre([^A-Za-z0-9_-]|$)" "$pk" 2>/dev/null && { pfound="$pk"; pvia="name"; }
                fi
                [ -n "$pfound" ] && { prow="$(pending_row_of "$pk")"; break; }
            fi
            [ "$pprobe" = "$TREE" ] && break
            pparent=$(dirname "$pprobe"); [ "$pparent" = "$pprobe" ] && break; pprobe="$pparent"
        done
        if [ -n "$pfound" ] && [ -z "$prow" ]; then
            PBAD=$((PBAD+1)); PBADLIST="$PBADLIST\n     ${f#$TREE/}  <-  ${pfound#$TREE/}  (NO 'ROW:' LINE -- a deferral must name the row it waits on)"
        elif [ -n "$pfound" ]; then
            st="$(pending_row_state "$prow")"
            case "$st" in
                LIVE)         DEF=$((DEF+1)); DEFLIST="$DEFLIST\n     ${f#$TREE/}  ->  $prow  (live; by $pvia)"; DEFROWS="$DEFROWS $prow";;
                UNVERIFIABLE) PUNV=$((PUNV+1)); PUNVLIST="$PUNVLIST\n     ${f#$TREE/}  ->  $prow  (queue unreadable at $PO)";;
                *)            PBAD=$((PBAD+1)); PBADLIST="$PBADLIST\n     ${f#$TREE/}  ->  $prow  ($st -- deferral is stale, convert these now)";;
            esac
        else
            UND=$((UND+1)); UNDLIST="$UNDLIST\n     ${f#$TREE/}"
            [ "${_BNC[$b]}" -gt 1 ] && AMBIG=$((AMBIG+1))
        fi
    fi
done
[ "$AMBIG" -gt 0 ] && echo "note: $AMBIG undeclared file(s) share a basename with another loose file — declare those by path relative to the KEEP.md, not by bare name"
mapfile -t PENDFILES < <(find "$TREE" -type f -name 'PENDING.md' 2>/dev/null)
DEFROWS_U="$(printf '%s\n' $DEFROWS | grep -v '^$' | sort -u | tr '\n' ' ')"
echo "KEEP.md file(s) found: ${#KEEPFILES[@]}   PENDING.md file(s) found: ${#PENDFILES[@]}"
echo "loose-but-undeclared: $UND   deferred-to-live-row: $DEF   stale-deferral: $PBAD   unverifiable-deferral: $PUNV"
[ "$DEF"  -gt 0 ] && { echo "deferred to another row (NOT converted, NOT keepers -- this gate certifies THIS row's scope, not the tree's completeness):"; printf "$DEFLIST\n" | grep -v '^[[:space:]]*$'; }
# ⛔ A STALE DEFERRAL IS A FAILURE, NOT A NOTE. The row it waits on is gone or already DONE, so nothing will ever
# convert these -- which is precisely the permanent-allowance state PENDING.md exists to make impossible.
if [ "$PBAD" -ne 0 ]; then
    echo "GATE FAILED -- $PBAD file(s) deferred to a row that is DONE, missing, or unnamed:"
    printf "$PBADLIST\n" | grep -v '^[[:space:]]*$'
    echo "     -> a deferral whose row has landed has outlived its reason: CONVERT these now, or if they are"
    echo "        genuinely permanent, move them to a KEEP.md with a reason. Do not repoint PENDING.md at a"
    echo "        fresh row to keep the gate quiet -- that is the allowance this bucket was built to refuse."
    exit 1
fi
if [ "$UND" -eq 0 ] && [ "$PUNV" -ne 0 ]; then
    echo "REFUSES rc=2: $PUNV deferral(s) name a row but the queue is unreadable at $PO, so they cannot be"
    echo "   verified. An unverified deferral is not a keeper and must never be counted as one."
    printf "$PUNVLIST\n" | grep -v '^[[:space:]]*$'
    exit 2
fi
if [ "$UND" -ne 0 ]; then
    echo "GATE FAILED -- $UND loose file(s) neither converted nor declared as keepers:"
    # ⛔⭐ THE LIST IS CAPPED AND THE CAP MUST ANNOUNCE ITSELF (hq_B 2026-08-29). This printed 19 names under
    # a "44 loose file(s)" headline with nothing saying the list was partial, so a seat working from the
    # printed list fixes what it can see, re-runs, and meets 25 files it was never shown. Same family as
    # `ls | head -5` read as absence, and as `command -v` answering a narrower question than was asked:
    # an instrument that truncates silently reports a subset in the shape of a whole.
    if [ -n "${SUITE_GATE_LIST_ALL:-}${GATE_LIST_ALL:-}" ]; then  # GATE_LIST_ALL is the fleet-wide spelling; SUITE_GATE_LIST_ALL kept as an alias
        printf "$UNDLIST\n" | grep -v '^[[:space:]]*$'
    else
        printf "$UNDLIST\n" | grep -v '^[[:space:]]*$' | head -20
        [ "$UND" -gt 20 ] && {
            echo "     ... and $((UND - 20)) MORE NOT SHOWN -- this list is capped at 20; $UND above is the true count."
            echo "     Full list: SUITE_GATE_LIST_ALL=1 bash scripts/test_gate_suite_conversion_complete.sh $LANG_DIR"
        }
    fi
    echo "     -> convert them, or name each in a KEEP.md WITH ITS REASON. Do not raise an allowance."
    exit 1
fi
if [ "$DEF" -gt 0 ]; then
    echo "GATE OK for the tests-consolidate-$LANG_DIR ROW -- $((${#LOOSE[@]} - DEF)) loose file(s) declared STANDALONE-KEEP."
    echo "⛔ THE TREE IS NOT FULLY CONSOLIDATED: $DEF file(s) remain, deferred to live row(s): $DEFROWS_U"
    echo "   This verdict certifies THIS row's scope only. The tree is complete when those rows land and this"
    echo "   gate is re-run with their PENDING.md removed -- at which point a stale deferral would FAIL it."
    exit 0
fi
echo "GATE OK -- ${#LOOSE[@]} loose file(s), all declared STANDALONE-KEEP. Conversion complete for $LANG_DIR."
exit 0
