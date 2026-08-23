#!/usr/bin/env bash
# lib_oracle_flags.sh -- ⭐ THE ONE AUTHORITY for the SPITBOL oracle's LANGUAGE ARM.
#
# WHY THIS FILE EXISTS (s200, row `bench-timed-oracle-flag`).  The s189 ruling established that `-bf` is the
# ONLY correct arm for grading or timing SCRIP against SPITBOL: SPITBOL CASE-FOLDS names by default (manual
# v3.7 p.23 names, p.28 labels) and `-f` turns folding OFF (p.162), which is the arm SCRIP -- declared
# CASE-SENSITIVE by RULES.md -- is MEASURED to agree with in three independent constructs (names/labels, the
# special name `output` vs `OUTPUT` p.192, and indirect reference `$('ABC')` p.182).  `scorecard_snobol4.sh`
# adopted it; the TIMING harnesses did not, and kept `-b`.  So the correctness board and the performance board
# were grading two DIFFERENT LANGUAGES, and `NOISE-FLOOR.tsv` was baked under the wrong one.
#
# ⛔ THE SPLIT THIS FILE ENCODES, AND WHY IT IS NOT ONE FUNCTION.  The LANGUAGE arm (`-bf`) is universal and is
# never a per-suite choice -- that was the whole s189 finding.  The SIZING knobs are NOT universal and are not a
# language question: a correctness run wants `-d512m -i64m` (heap/stack for the big corpus programs), a timed run
# wants `-s16m` (the json deserializer's recursive descent overflows the default stack).  Sizing a stack is not a
# throughput knob and not a semantics knob.  Callers therefore take `sbl_lang_flags` VERBATIM and append their own
# sizing -- they may never substitute for it.
#
# ⛔ CALLERS MUST REFUSE, NOT FALL BACK.  A harness that cannot load this file must exit non-zero and say so.  A
# silent private default is exactly the defect this file retires: it looks like a measurement and is a different
# language.  (Same shape as row `suite-table-one-authority`, where the refusal is the load-bearing half.)
sbl_lang_flags() { echo "-bf"; }

# ⭐ THE ONE AUTHORITY for the SPITBOL oracle's BINARY (s row `clean-oracle-build`).  Lon's FACT RULE
# (2026-08-22, in-chat): "For benchmarking we should use only the official SPITBOL from the x64 repo at
# the SPITBOL GitHub."  The repo's checked-in `x64/bin/sbl` is INSTRUMENTED -- it carries a monitor IPC
# bridge (sysmc/sysml/sysmv/sysmr/sysmw/pmcll/pmext/pmred/pmfal fire-points, ~130 lines over upstream)
# that is NOT part of official SPITBOL.  MEASURED (callgrind Ir, matched `-bf` flags, cross-validated via
# an independent same-toolchain rebuild of the full fork tree landing within 7 instructions of the
# checked-in binary): the monitor fire-points cost ~2.2-2.3x the instruction count on statement/store-
# dense code even with MONITOR_READY_PIPE/MONITOR_GO_PIPE unset -- documented as "negligible overhead"
# but empirically NOT, most likely an uncached per-fire env-var check.  This binary is a from-source
# build of official upstream (github.com/spitbol/spitbol, HEAD 4fe74db) plus exactly two ALLOW-LISTED
# compatibility patches (Lon's amendment, same session): the uppercase-keyword/datatype-name table
# conversion + matching flc/flstg fold-direction reversal (so `-bf` accepts SCRIP-convention uppercase
# keywords), and the NS-TIME clock fix to osint/systm.c (s249, Lon in-chat, CLOCK_MONOTONIC not
# CLOCK_PROCESS_CPUTIME_ID).  NO monitor instrumentation, NO LOAD/UNLOAD ABI rework (that subsystem was
# classified UNKNOWN/deferred, not needed for the 15 top-level benchmark kernels, and is NOT minor).
# Proven: nm/strings show zero monitor symbols; patched-vs-pure-upstream Ir delta is 0.0067%, fully
# one-time (does not scale with iteration count); all 15 corpus/benchmarks/snobol4 kernels' deterministic
# `check:` output matches x64/bin/sbl exactly.  Full measurement in
# FINDING-2026-08-22-seat2-clean-oracle-monitor-overhead.md.
#
# ⛔ CALLERS MUST REFUSE, NOT FALL BACK -- same law as sbl_lang_flags above.  This path is OUTSIDE every
# tracked repo (built alongside its upstream source clone, both living outside the seat-root workspace so
# neither is ever discovered as a repo by tooling that walks S4E_HOME); a caller that cannot find it must
# say so loudly, never silently substitute x64/bin/sbl.
# ⛔⛔ THE -f CAPABILITY ASSERTION (hq_P s259, row `oracle-bin-sbl-trap`).  MEASURED: three SPITBOL binaries on
# this box -- `spitbol-clean/bin/sbl` (before it was repointed), `spitbol-fork-rebuilt/bin/sbl` and
# `spitbol-upstream/bin/sbl`, all byte-identical -- DO NOT SUPPORT `-f`.  Handed the MANDATORY `-bf` they answer
# "No END statement found in source file(s)." to EVERY program, including a two-line one that plainly has an END.
# A harness pointed at one of them prints a full, plausible, ENTIRELY FALSE all-FAIL table -- the exact class
# CLAUDE.md records this project hitting in three separate sessions.
# ⭐ WHY IT IS STRUCTURAL AND WILL COME BACK: a SPITBOL source tree SHIPS a prebuilt `bin/sbl` as BASEBOL, the
# BOOTSTRAP compiler; `make spitbol` consumes it and writes the real product to the TOP LEVEL as `./sbl`.  In our
# `x64/` fork `bin/sbl` IS the product.  The two layouts disagree, so the natural generalisation "the oracle is
# <root>/bin/sbl" lands EXACTLY on the trap.  Documentation cannot fix that; a capability check can.
# ⛔ THE RULE THIS ENCODES: an oracle is not a PATH, it is a BINARY THAT ACCEPTS `-bf`.  Never assemble a path.
sbl_bf_capable() {   # $1 = binary.  0 = accepts -bf, 1 = does not, 2 = could not be tested.  Memoised per shell.
    [ -n "${1:-}" ] && [ -x "$1" ] || return 1
    local key; key="_sbl_bf_memo_$(printf '%s' "$1" | tr -c 'A-Za-z0-9' '_')"
    local memo="${!key:-}"; [ -n "$memo" ] && return "$memo"
    local t; t="$(mktemp 2>/dev/null)" || { printf '%s' "$1" >/dev/null; return 2; }
    printf "        OUTPUT = 'sblbfok'\nEND\n" > "$t"
    local out; out="$(timeout 30 "$1" -bf "$t" < /dev/null 2>&1)"; rm -f "$t"
    case "$out" in *sblbfok*) eval "$key=0"; return 0 ;; esac
    eval "$key=1"; return 1
}
# ⛔ REFUSE, NEVER FALL BACK -- the same law as the two functions above, now applied to CAPABILITY rather than
# to mere existence.  A missing binary was already loud; a PRESENT binary that silently grades nothing was not.
sbl_assert_bf() {    # $1 = binary, $2 = human label.  Prints the path on success; refuses loudly and returns 3.
    if sbl_bf_capable "$1"; then printf '%s\n' "$1"; return 0; fi
    printf '⛔ REFUSING: %s at %s does not accept the MANDATORY -bf (s189).\n' "${2:-oracle}" "$1" >&2
    printf '   It answers "No END statement found" to EVERY program, so a run against it is a FALSE all-FAIL table,\n' >&2
    printf '   not a measurement. A SPITBOL tree ships bin/sbl as the BASEBOL BOOTSTRAP; the built product is ./sbl\n' >&2
    printf '   at the TOP LEVEL. Do not assemble oracle paths -- use sbl_clean_bin / sbl_correctness_bin.\n' >&2
    return 3
}
# ⛔⛔ NAMING, CORRECTED BY LON s259 ("What is spitbol-upstream, is the pristine version? ... What is upstream mean?").
# "UPSTREAM" IS A GIT WORD, NOT A QUALITY WORD: it names the project you forked FROM (github.com/spitbol/spitbol),
# as opposed to `origin`, your own fork (snobol4ever/x64).  It says PROVENANCE and says NOTHING about whether the
# binary in that tree can serve as an oracle -- and the one shipped there CANNOT (no `-f`).
# ⭐ MEASURED, and it inverts the old names: `spitbol-upstream` had **0 modified files** -- it was the genuinely
# pristine one -- while `spitbol-clean` has **4** (the two allow-listed patches).  THE NAME "clean" WAS THE LIE.
# Renamed by ROLE s259, compatibility symlinks left at both old names so nothing in flight breaks:
#   spitbol-pristine/      <- was spitbol-upstream       untouched reference SOURCE. ⛔ NOT an oracle: its shipped
#                                                        bin/sbl is the BASEBOL bootstrap and rejects `-bf`.
#   spitbol-bench-oracle/  <- was spitbol-clean          the minimal-patch BUILD we TIME against. ⭐ the oracle.
#   x64/                   (shared, symlinked into all 19 roots since Lon s259) the CORRECTNESS oracle + IPC monitor.
sbl_clean_bin() { sbl_assert_bf "/home/resources/spitbol-bench-oracle/sbl" "the CLEAN benchmark oracle"; }
# ⭐ THE CORRECTNESS FACE, through the authority for the first time (hq_P s259).  Callers previously assembled
# "$X64/bin/sbl" by hand, which is how a path becomes a trap.  Since Lon's s259 ruling ("everyone should be
# using the shared /home/resources") every seat root's x64/ is a SYMLINK to /home/resources/x64, so the shared
# path is canonical and the per-root one still resolves.
# ⛔⭐⭐ SHARED ONLY -- NO PER-ROOT x64, NO SYMLINK (Lon, 2026-08-23 s261: "Ensure that no root have x64. Everyone must share." / "Do not use symlinks.")
# This used to fall back to "${S4E_HOME}/x64/bin/sbl" when the shared oracle was missing.  That fallback is DELETED, and deleting it is the point: a fallback does not just TOLERATE a per-root clone, it
# SILENTLY REWARDS one -- a seat that cloned x64 into its root would grade green against its own private copy and never learn it had diverged from everyone else.  A missing shared oracle must be LOUD.
# A per-root clone is also a permanent handoff_status.sh blocker the moment it takes a local commit, since that script adopts every directory in the root that is a git repo with an origin remote.
# VERIFIED s261 across all 19 roots: no x64 entry of any kind, symlink or directory.  The shared tree is /home/resources/x64 (clean, 0 dirty, 0 unpushed).
sbl_correctness_bin() {
    local c="/home/resources/x64/bin/sbl"
    if [ ! -x "$c" ]; then
        printf "⛔ THE SHARED CORRECTNESS ORACLE IS MISSING: %s\n" "$c" >&2
        printf "   ⛔ DO NOT clone x64 into your seat root and DO NOT symlink it -- Lon s261, everyone shares /home/resources/x64.\n" >&2
        printf "   Restore the SHARED tree, then re-run.  Grading with it absent prints a full, plausible, entirely false all-FAIL table.\n" >&2
        return 1
    fi
    sbl_assert_bf "$c" "the CORRECTNESS oracle (x64, instrumented)"
}

# ⭐ A LIVE EDGE, PRESERVED NOT FIXED (row oracle-two-face-adoption): the clean binary above is a from-
# source build of official upstream with exactly the two ALLOW-LISTED patches described in the block
# above -- it carries NO LOAD/UNLOAD ABI rework, so its external-fn loader is stock upstream and its
# parity with x64/bin/sbl on a LOAD()-calling program is UNVERIFIED (classified UNKNOWN/deferred, not
# needed for the 15 top-level benchmark kernels -- none of which call LOAD).  Fixing that subsystem is
# out of scope here.  What IS in scope: a benchmark timing a LOAD()-calling program against this binary
# must REFUSE LOUDLY, never silently print a number of unknown validity -- same "CALLERS MUST REFUSE,
# NOT FALL BACK" law as the two functions above, applied to program CONTENT instead of binary ABSENCE.
# Usage: sbl_clean_refuse_if_load "$path/to/prog.sno" || exit 3   (grep, not a parse -- LOAD as a bare
# word covers the real corpus shape; a string literal merely containing the four letters is not a
# realistic false-positive in named SNOBOL4 source and a parser is not worth building for this edge).
sbl_clean_refuse_if_load() {
  grep -qE '(^|[^A-Za-z0-9_])LOAD\s*\(' "$1" 2>/dev/null || return 0
  echo "⛔ REFUSING: $1 calls LOAD() -- the clean benchmark oracle's LOAD/external-fn support is stock upstream and UNVERIFIED against x64/bin/sbl (see sbl_clean_bin() above). Benchmarking this program against it would silently report a number of unknown validity, not a loud refusal. This is a preserved gap, not a bug for this row to fix. Time it against x64/bin/sbl directly (correctness oracle, sbl_lang_flags) if you must measure a LOAD()-calling program." >&2
  return 1
}
