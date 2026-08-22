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
sbl_clean_bin() { echo "/home/resources/spitbol-clean/sbl"; }
