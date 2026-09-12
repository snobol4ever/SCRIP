#!/usr/bin/env bash
source "$(dirname "${BASH_SOURCE[0]}")/lib_one_runner.sh" && one_runner_guard "${0##*/}" "${S4E_CORPUS:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)/corpus}/packages/prolog/logtalk_iso" || exit 2
# test_prolog_logtalk_suite.sh — THE LOGTALK ISO/IEC 13211-1 CONFORMANCE BOARD
# (row prolog-logtalk-iso-3268-conformance-cases-have-no-runner, hq_R 2026-09-12; Lon 2026-09-11: "Let's
# get those Prolog programs graded.").
#
# WHAT IT GRADES. corpus/packages/prolog/logtalk_iso — 192 tests.lgt holding 3617 conformance cases across
# 97 built-in predicates, 54 arithmetic functions, 12 control constructs, 11 syntax groups, 7 directives,
# the logical update view, unbounded integers and Unicode. Roughly 7x the INRIA suite, Apache-2.0, and the
# suite SWI-Prolog, JIProlog and Tau Prolog self-check with. Before this runner: 0 of 3617 graded, which
# made it the largest single gap on the board.
#
# ⛔⭐ THE ROW NAME SAYS 3268 AND THE POPULATION IS 3617. The row was minted off a census of `test/2` heads
# alone; the suite also uses lgtunit's three legacy spellings — succeeds/1 162, throws/2 134, fails/1 53 —
# which are the same three expectations written differently (succeeds == true, fails == false,
# throws(Name,Ball) == the ball). ⭐ A PARSER THAT HANDLES ONLY test/2 GRADES 90% OF THIS SUITE AND REPORTS
# 100% OF IT, and that is the defect class this row exists to stop, so the legacy three are normalised into
# the same expectation vocabulary rather than given a second grading path.
#
# ⛔⭐ WHAT THE CRITERION IS, AND WHY IT IS NOT AN ORACLE DIFF. Every case carries its OWN expectation,
# lifted from ISO/IEC 13211-1 and citing the standard's section numbers in the comment above it. This
# runner therefore grades scrip against THE STANDARD AS THE STANDARD STATES IT, not against gprolog's or
# swipl's opinion of it — a stronger instrument than any oracle diff this repo owns, and the reason this
# row decides whether "100% of the industry-standard language" can be said about Prolog at all.
#
# ⛔ THE IDENTITY IS THE DELIVERABLE, NOT THE PERCENTAGE (ARCH-PROGRAM-LEDGER):
#     PASS + FAIL + OUTSIDE + UNGRADABLE + UNGRADED + DEFERRED == the population globbed
# and it is ASSERTED IN CODE — if it does not reconcile the run REFUSES rather than publishing numbers with
# an invisible remainder. A case this runner cannot set up, cannot express, or is told to skip by its own
# `condition(...)` option is UNGRADED **and named with its reason**: never dropped, never a pass. UNKNOWN
# is not ZERO.
#
# Usage: bash scripts/test_prolog_logtalk_suite.sh [--modes m3|m3,m4] [--name-reds] [--jobs N]
set -uo pipefail
GATE_NAME=test_prolog_logtalk_suite
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="${S4E_HOME:-$(cd "$HERE/../.." && pwd)}"
SUITE="${S4E_CORPUS:-$ROOT/corpus}/packages/prolog/logtalk_iso"
SCRIP="$HERE/../scrip"
MODES="${LOGTALK_MODES:-m3,m4}"
JOBS="${LOGTALK_JOBS:-12}"
NAME_REDS=""
refuse() { echo "⛔ REFUSED(2) [$GATE_NAME]: $*" >&2; exit 2; }
while [ $# -gt 0 ]; do
    case "$1" in
        --modes) MODES="$2"; shift 2 ;;
        --jobs) JOBS="$2"; shift 2 ;;
        --name-reds) NAME_REDS="--name-reds"; shift ;;
        *) refuse "unknown argument '$1' -- this runner has no silent fall-through" ;;
    esac
done

[ -d "$SUITE" ] || refuse "no vendored suite at $SUITE -- a suite that is absent is not a suite that is failing"
[ -x "$SCRIP" ] || refuse "no scrip binary at $SCRIP -- run make first; a missing binary prints a full, plausible, entirely false board"
# ⛔⭐ STALE-BINARY PREFLIGHT. The line above proves a binary EXISTS; this one proves it is the binary this
# tree describes. NO LOGIC HERE: util_require_fresh.sh sources gate_require_fresh from lib_gate.sh, the ONE
# authority — never a second copy of the staleness rule.
"$HERE/util_require_fresh.sh" --gate "$GATE_NAME" "$SCRIP" "${RT_DIR:-$HERE/../out}/libscrip_rt.so" || exit 2
. "$HERE/lib_gate.sh"
. "$HERE/lib_inventory.sh"

# ⛔⭐⭐ THE HARNESS PREFLIGHT, AND IT IS THE MOST IMPORTANT LINE IN THIS FILE. 1141 of the 3617 cases call
# an lgtunit framework predicate, so lib_logtalk_lgtunit.pl (the plain-Prolog shim that re-implements them)
# is prepended to EVERY generated case program. If the shim itself does not compile and run under this
# binary, every case comes back refused — and the board that prints is complete, plausible and entirely
# about the harness. ⭐ So the harness proves itself on a two-line witness BEFORE any case is graded, and
# says so out loud, because "the instrument was broken" must never be a thing the reader has to infer from
# a suspiciously round number.
_pf="$(mktemp -d "${TMPDIR:-/tmp}/lgt_preflight.XXXXXX")"
{ cat "$HERE/lib_logtalk_lgtunit.pl"
  echo "'\$p' :- lgt_h(lgt_set_text_output('')), write(ok), lgt_h(lgt_text_output_assertion(ok, A)), A."
  echo ":- ( catch('\$p', E, (write(user_output, '@PRE ball '), writeq(user_output, E))) -> write(user_output, '@PRE ok') ; write(user_output, '@PRE fail') ), nl(user_output)."
} > "$_pf/preflight.pl"
_pfout="$(cd "$_pf" && timeout 30 "$SCRIP" "$_pf/preflight.pl" </dev/null 2>&1)"; _pfrc=$?
rm -rf "$_pf"
case "$_pfout" in
    *"@PRE ok"*) echo "HARNESS PREFLIGHT ok: lib_logtalk_lgtunit.pl compiles and round-trips a captured write under this binary" ;;
    *) refuse "the lgtunit shim does not run under this binary (rc=$_pfrc): ${_pfout:-<no output>}
    Every case program embeds this shim, so grading now would publish a complete, plausible board that measures the harness and not the language." ;;
esac

_out="$(mktemp "${TMPDIR:-/tmp}/lgt_board.XXXXXX")"
python3 "$HERE/util_logtalk_grade.py" --suite "$SUITE" --scrip "$SCRIP" --modes "$MODES" --jobs "$JOBS" $NAME_REDS | tee "$_out"
# ⛔ PIPESTATUS[0], NEVER $? -- the pipeline ends in `tee`, so $? reports the pager and a python that died
# reads as a clean run (CLAUDE.md, measured live on this box).
_rc=${PIPESTATUS[0]}
[ "$_rc" -eq 0 ] || { rm -f "$_out"; exit "$_rc"; }

_shell="$(grep -m1 '^BOARD_FOR_SHELL ' "$_out" || true)"
[ -n "$_shell" ] || { echo "⚠ SCORE.md NOT UPDATED [$GATE_NAME]: the run printed no BOARD_FOR_SHELL line, so there is no measurement to record"; rm -f "$_out"; exit 1; }
set -- $_shell
_pop="$2"; _m3p="$3"; _m3f="$4"; _m4p="${5:-}"; _m4f="${6:-}"

# ⛔⭐ THE PACKAGE INVENTORY IS A FILE CENSUS AND THE BOARD ABOVE IS A CASE CENSUS -- two populations, and
# conflating them is how a package reports 100% of the wrong thing. shipped counts .lgt files (400: 192
# tests.lgt plus the vendor's 192 driver objects, 6 alternative entry objects and 10 encoded data
# fixtures, each named in UNGRADABLE.tsv); the board counts the 3617 CASES inside the 192. Both are
# printed, and the board line says which number is which.
INV_PACKAGE=logtalk_iso; INV_DIR="$SUITE"; INV_EXT=".lgt"
_iv="$(inventory_line 192 0)"; _ivrc=$?
[ "$_ivrc" -eq 0 ] && echo "$_iv"
[ "$_ivrc" -eq 0 ] || echo "⚠ the package inventory refused (rc=$_ivrc) -- the case board above stands on its own measurement; the FILE census does not"

_board="$(grep -m1 '^LOGTALK_ISO_BOARD ' "$_out" || true)"
_ident="$(grep -m1 ' identity ' "$_out" | sed 's/^ *//' || true)"
_txt="logtalk_iso (ISO/IEC 13211-1, the suite's own per-case expectations) $_m3p/$_pop m3"
[ -n "$_m4p" ] && _txt="$_txt · $_m4p/$_pop m4"
_txt="$_txt — every case graded against the standard as the standard states it, not an oracle diff; $_ident"
gate_score_row prolog vendor "$_txt${_iv:+ · $_iv (\`$GATE_NAME.sh\`)}" "$MODES" LOGTALK
rm -f "$_out"
exit 0
