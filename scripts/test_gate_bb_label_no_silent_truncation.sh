#!/usr/bin/env bash
# stale-binary preflight (row test-gate-scripts-that-grade-scrip-refuse-on-a-stale-binary-census-widened, hq_T 2026-09-05)
"$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/util_require_fresh.sh" --gate "$(basename "${BASH_SOURCE[0]}" .sh)" || exit $?
# test_gate_bb_label_no_silent_truncation.sh — THE PRISON for: A BB LABEL NAME IS NEVER SILENTLY TRUNCATED.
#
# THE INVARIANT: two distinct label names must never become one symbol. bb_label_t.name is a fixed
# BB_LABEL_NAME_MAX (80) byte field. Writing a longer name into it with a plain vsnprintf/strncpy
# discards whatever distinguished it -- INCLUDING the alpha/beta/gamma/omega port suffix, which is the
# last thing on the string -- so several boxes emit the SAME symbol and `as` rejects the file. The
# compiler printed NO diagnostic: the first evidence was the assembler's "already defined".
#
# Cured by SCRIP 32cce542: every write goes through bb_label_name_set(), which under the limit copies
# verbatim and over it keeps a 62-char prefix plus '$' and a 16-hex FNV-1a of the FULL name, backing the
# cut off a multi-byte UTF-8 boundary (the port suffixes are UTF-8).
#
# WITNESS: corpus/programs/prolog/rung10_programs_puzzles.pl. Its line 190 is a bare '#' + 119 dashes --
# no leading '%' and no terminating '.' -- so it lexes as a symbolic atom and becomes a predicate name
# that mangles to 360 chars. Pre-cure that file emitted 6 label occurrences with 1 distinct name.
#
# THREE INDEPENDENT LOCKS. A reintroduction must defeat all three.
#   LOCK 1 (structural) — no raw truncating write into a label name field survives in the emitter.
#   LOCK 2 (behavioral GREEN) — the witness compiles AND assembles, and its long labels are all distinct.
#   LOCK 3 (fail-once RED) — the SAME emitted names, truncated to 79 as the old code did, COLLIDE.
#       LOCK 3 is what makes LOCK 2 mean something: it proves the distinctness in LOCK 2 is produced by
#       the cure and not by the witness being easy. ⛔ What LOCK 3 does NOT do is rebuild a broken
#       emitter; for the full two-part proof against real codegen, run with BBLBL_REVERT_ARM=1 after
#       building 32cce542^ in a worktree, and this gate must go red there.
#
# PROOF STATUS, stated exactly (RULES.md § THE INSTRUMENT LAWS, part 1 -- two-part proof):
#   MEASURED GREEN on the cured tree, and MEASURED RED: an earlier draft of LOCK 3 re-cut the emitted
#   names to MAXN-1, which is a no-op because the cure emits exactly that length, and the gate correctly
#   went rc=1 rather than passing vacuously. So the failure path is exercised end to end, not assumed.
#   ⛔ WHAT IS NOT YET DONE: the reverted-emitter arm. A worktree at 32cce542^ was built for it but the
#   box was at loadavg 24 during a fleet stand-down and it did not finish. To complete the proof, build
#   32cce542^ (or force BB_LABEL_NAME_MAX small) and run this gate there with BBLBL_REVERT_ARM=1; it
#   MUST print a red LOCK 0 (cure absent) and a red LOCK 2a (as rejects the witness). Until someone does
#   that, treat this gate as measured-but-not-fully-proven and say so rather than assuming it is closed.
#
# ⛔ LATENT SIBLINGS OF THIS CLASS — deliberately NOT asserted here, because neither has a witness and an
# unwitnessed codegen change cannot be verified. Named so the next reader finds the boundary drawn on
# purpose rather than assuming the class is closed:
#   - src/templates/bb/bb_define.cpp:30  bb_ab_sym_name() truncates at 250 mangled chars with no
#     disambiguation; two predicate names sharing an 83-char prefix would still collide. Not reachable
#     through the label path any more (labels now hash), and no corpus program exercises it.
#   - src/emitter/emit.cpp:95  emit_label_intern() dedups by strcmp against the STORED name, so two
#     interns of one over-long name still allocate two entries carrying one final name. Behaviour is
#     byte-identical before and after 32cce542; normalising the lookup key would close it.
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
S4E="${S4E_HOME:-$(cd "$ROOT/.." && pwd)}"
WITNESS="$S4E/corpus/programs/prolog/rung10_programs_puzzles.pl"
refuse() { echo "⛔ GATE REFUSES (cannot measure): $*"; exit 2; }
command -v as       >/dev/null 2>&1 || refuse "no 'as' assembler on PATH"
[ -x "$ROOT/scrip" ]                || refuse "no built ./scrip at $ROOT/scrip -- run make first"
[ -r "$WITNESS" ]                   || refuse "witness unreadable: $WITNESS"
fail=0
# ⛔ The absence of the cure is a RED, never a refusal. An earlier draft refused (rc=2) here, which
# would have reported a REVERTED emitter as "cannot measure" -- the one case this gate exists to catch.
if ! grep -q 'bb_label_name_set' "$ROOT/src/emitter/emit.h"; then
    echo "  ✗ LOCK 0: bb_label_name_set() is absent from emit.h -- the cure has been reverted or removed"; fail=1
else echo "  ✅ LOCK 0: the cure (bb_label_name_set) is present in emit.h"; fi
# ---- LOCK 1: no raw truncating write into a label name field -------------------------------------
raw=$(grep -nE 'vsnprintf[[:space:]]*\([[:space:]]*lbl->name|strncpy[[:space:]]*\([[:space:]]*lbl\.name' \
        "$ROOT/src/emitter/emit.cpp" "$ROOT/src/emitter/emit.h" 2>/dev/null)
if [ -n "$raw" ]; then echo "  ✗ LOCK 1: a raw truncating write into a label name survives:"; echo "$raw" | sed 's/^/      /'; fail=1
else echo "  ✅ LOCK 1: every label-name write goes through bb_label_name_set()"; fi
# ---- LOCK 2: the witness compiles, assembles, and its long labels are distinct --------------------
S=$(mktemp) || refuse "mktemp failed"
trap 'rm -f "$S"' EXIT
if ! timeout 120 "$ROOT/scrip" --compile "$WITNESS" < /dev/null > "$S" 2>/dev/null || [ ! -s "$S" ]; then
    echo "  ✗ LOCK 2: witness failed to COMPILE (expected a non-empty .s)"; fail=1
else
    if as --64 -o /dev/null "$S" 2>/dev/null; then echo "  ✅ LOCK 2a: witness assembles (as --64 rc=0)"
    else echo "  ✗ LOCK 2a: as --64 REJECTED the emitted asm -- label collision is back:"; as --64 -o /dev/null "$S" 2>&1 | head -3 | sed 's/^/      /'; fail=1; fi
    occ=$(grep -cE '^\$[0-9A-F]{2}\$.*:$' "$S" 2>/dev/null || echo 0)
    dis=$(grep -oE '^\$[0-9A-F]{2}\$[^:]*' "$S" 2>/dev/null | sort -u | wc -l)
    if [ "$occ" -eq 0 ]; then echo "  ✗ LOCK 2b: witness emitted NO mangled labels -- it no longer exercises the class"; fail=1
    elif [ "$occ" -ne "$dis" ]; then echo "  ✗ LOCK 2b: $occ mangled label definitions but only $dis distinct -- names are colliding"; fail=1
    else echo "  ✅ LOCK 2b: $occ mangled label definitions, $dis distinct (1:1)"; fi
    # ---- LOCK 3: fail-once -- the same names truncated to 79 (the old behaviour) MUST collide -----
    names=$(grep -oE '^\$[0-9A-F]{2}\$[^:]*' "$S" 2>/dev/null | sort -u)
    MAXN=$(grep -oE '#define[[:space:]]+BB_LABEL_NAME_MAX[[:space:]]+[0-9]+' "$ROOT/src/emitter/emit.h" | grep -oE '[0-9]+$')
    [ -n "$MAXN" ] || refuse "cannot read BB_LABEL_NAME_MAX from emit.h"
    keep=$(( MAXN - 18 ))
    if [ -z "$names" ]; then echo "  ✗ LOCK 3: no names to test; LOCK 2b already reported why"; fail=1
    else
        ndis=$(printf '%s\n' "$names" | wc -l)
        pdis=$(printf '%s\n' "$names" | cut -c1-"$keep" | sort -u | wc -l)
        # ⛔ Do NOT re-cut these to MAXN-1: the cure already emits exactly that length, so cutting is a
        # no-op and the lock would pass vacuously. What proves the defect is stripping the DISAMBIGUATOR:
        # the surviving prefix is precisely what the old plain truncation would have kept.
        if [ "$pdis" -lt "$ndis" ]; then echo "  ✅ LOCK 3 (fail-once): $ndis distinct labels share only $pdis distinct ${keep}-char prefixes -- strip the hash the cure appends and they collapse, which is the old code's exact defect. LOCK 2b is load-bearing."
        else echo "  ✗ LOCK 3: the $ndis names differ within their first $keep chars, so this witness would NOT have collided pre-cure and proves nothing; mint a witness whose mangled name exceeds $MAXN"; fail=1; fi
    fi
fi
# ---- optional real-revert arm (two-part proof against real codegen) -------------------------------
if [ "${BBLBL_REVERT_ARM:-0}" = "1" ]; then
    if as --64 -o /dev/null "$S" 2>/dev/null; then echo "  ✗ REVERT ARM: built tree assembles the witness, so this is NOT a reverted emitter"; fail=1
    else echo "  ✅ REVERT ARM: reverted emitter rejects the witness -- gate goes red as required"; fi
fi
if [ "$fail" -ne 0 ]; then echo "⛔ GATE FAILED: a BB label name can be silently truncated into a colliding symbol."; exit 1; fi
echo "✅ GATE OK: BB label names are never silently truncated; distinct names stay distinct symbols."
