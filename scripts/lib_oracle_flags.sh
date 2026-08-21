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
