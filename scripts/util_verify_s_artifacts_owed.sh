#!/usr/bin/env bash
# util_verify_s_artifacts_owed.sh — DRY drift check for the .s artifact regen chain
# named in RULES.md handoff step 4. Turns "I remembered to regen" into "the tree says
# so": runs every REQUIRED regen script against a disposable local clone of corpus
# (never the real tree), diffs, reports, discards the clone. NEVER writes to the real
# corpus or SCRIP checkouts and NEVER commits anywhere real — a "verifier" that
# silently commits regenerated artifacts is a different, more dangerous tool.
# (row six-owed-verifier, minted hq_C 2026-08-24 s272, approved CEO-21)
#
# SCOPE, DELIBERATE (RULES.md handoff step 4, CURRENT text, post-s269 ARTIFACT POLICY
# ruling by Lon in-chat to CEO, verbatim in substance: "We will abandon the idea of
# having artifacts for tests ... But we want artifacts for benchmarks and demos."):
#   CHECKED:      util_regen_benchmark_s_artifacts.sh    (SNOBOL4 benchmarks)
#                 util_regen_demo_s_artifacts.sh          (demo corpus)
#                 util_regen_prolog_bench_s_artifacts.sh  (Prolog benchmarks)
#                 update_icon_bench_asm.sh CHECK=1        (Icon benchmarks; native dry-run)
#   OUT OF SCOPE, NAMED NOT OMITTED: util_regen_feature_s_artifacts.sh,
#                 util_regen_crosscheck_s_artifacts.sh, util_regen_programs_s_artifacts.sh
#                 — RULES.md s269 retired all three from the required chain ("test-tree
#                 artifacts ... leave the chain ... the s169/s192 silent-drift lessons
#                 those steps encoded are moot once no test artifact exists to drift").
#                 The row that minted this script said "six" — that described RULES.md's
#                 PRE-s269 text; corrected here to match RULES.md as it reads today (see
#                 task six-owed-verifier's QA and its FINDING for the full reasoning,
#                 including that the three retired trees are NOT actually deleted yet).
#
# WHY A SCRATCH CLONE, NOT THE REAL TREE ("CHECK, DON'T WRITE"): three of the four
# checked scripts `git commit` directly inside $CORPUS when they find drift — that is
# not a dirty working tree a stash could isolate, it is a REAL COMMIT. The only way to
# see what they WOULD do without mutating the real repo's history is to point them at a
# disposable local clone via the CORPUS/BENCH_DIR env vars they already respect, then
# discard the clone. update_icon_bench_asm.sh needs no clone: CHECK=1 is its own native,
# non-mutating dry-run (writes nothing, reports what would change, own exit code).
#
# WHY NOT TRUST EACH SCRIPT'S OWN EXIT CODE / "No changes" ALONE: only the benchmark
# script's exit code reflects a per-file compile/assemble failure ($fail); demo and
# prolog_bench always exit 0 (their last command is an echo), so a REJECTED-BY-AS /
# TIMEOUT / assembler-rejected line can sail past a caller that only checks $?. And a
# script that finds ZERO of its expected inputs reports "No changes" — indistinguishable
# from genuinely current unless something also checks that inputs were found at all
# (empirically true right now for demo: corpus/demos was reorganized into per-family
# subfolders at s272 — commit db20f3cf, "demo/ grouped into per-family subfolders" — and
# util_regen_demo_s_artifacts.sh still assumes flat corpus/demos/<name>.sno, so every
# entry SKIPs and it reports "No changes" having looked at nothing). This script greps
# each captured log for its own trouble vocabulary rather than trusting $? alone, and
# treats "found nothing to check" as a DISTINCT, always-reported condition — never
# silently folded into "clean". That is the whole point of this row: exit 0 on a tree
# that HAS drift (or cannot even be checked) is the make-test false-green trap.
#
# EXIT CODES: 0 = nothing owed, every check actually ran. 1 = something owed, OR a
# checked script hit trouble (compile/assemble failure, missing sources) — never a
# verdict of clean when a check could not actually complete. 2 = environment error
# (corpus repo missing, clone failed, pristine build failed) — no verdict was possible.
#
# Usage: util_verify_s_artifacts_owed.sh [--pristine] [--skip-pristine]
#   --pristine        wipe out/ and rebuild from scratch first. ⛔ DESTRUCTIVE TO THIS ROOT: ./scrip is
#                     ABSENT for ~10-20 minutes, so any OTHER measurement running in this root during that
#                     window dies with rc=127 and reads as a catastrophic regression in whatever it graded.
#                     ⭐ NO LONGER THE DEFAULT (hq_T 2026-09-06, on hq_U's report): it cited HQ-27, which Lon
#                     VOIDED as a per-landing requirement on 2026-09-03 ("It's time to loosen this pristine
#                     build ... causes 20 minute wait times"), and a .s-currency check needs a CURRENT
#                     compiler, not a from-scratch one. Use it for a ceo audit or a release point.
#   --skip-pristine   reuse the already-built ./scrip as-is (fast iteration ONLY —
#                      NEVER use for a real handoff/gate verdict; HQ-27 requires pristine).
#   SCRIP=<path>      override the scrip binary (default $ROOT/scrip)
#   CORPUS=<path>     override the real corpus checkout to clone from (default $S4E/corpus)
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
SCRIP_BIN="${SCRIP:-$ROOT/scrip}"
SCRIP_RT="${SCRIP_RT:-$ROOT/out/libscrip_rt.so}"   # overridable so the gate can drive the .so arm on a fixture, never the live build
REAL_CORPUS="${CORPUS:-$S4E/corpus}"
SKIP_PRISTINE=0
WANT_PRISTINE=0
for _a in "$@"; do
  case "$_a" in
    --skip-pristine) SKIP_PRISTINE=1 ;;
    --pristine)      WANT_PRISTINE=1 ;;
  esac
done

# ⛔⭐⭐ STALE-BINARY PREFLIGHT — REFUSE rc=2, NEVER PRINT AN OWED COUNT FROM A BINARY THAT PREDATES src/.
# This script dry-run-regenerates with THE BINARY IN THE TREE. Under --skip-pristine (which is how
# handoff_status.sh always calls it, deliberately -- row `stop-hook-pristine`: a pristine build here was
# wiping live ./scrip out from under seats) the only previous check was `-x`, so a binary that merely
# EXISTS was trusted. A stale one manufactures phantom debt that reads exactly like a lying gate.
# ⭐ MEASURED, hq_B 2026-09-02: `OWED -- 26 item(s)` at rung-6 HEAD with a ./scrip built 17:32, 17 minutes
# before rung 6 landed at 17:49; 0 owed after `make pristine`. The .s.REFUSED marker text embeds
# "rung N lands it", so a pre-rung-6 binary genuinely owes different markers -- the count was TRUE about
# that binary and FALSE about origin, and nothing in the output said which question it had answered.
# ⛔ The header comment in handoff_status.sh reasoned "can degrade to a REFUSAL, never to a false CLEAN".
# That is right about CLEAN and was never the exposure: it degrades to a false OWED, which BLOCKS a handoff.
# ⛔⭐ THE REFERENCE IS THE NEWEST FILE MTIME UNDER src/, AND DELIBERATELY *NOT* THE NEWEST COMMIT TOUCHING src/
# -- which is what the row specified and what this function shipped with for exactly one commit (92d300f0).
# A commit's %ct is when it was AUTHORED, and COMMITTING SOURCE YOU HAVE ALREADY BUILT MOVES IT PAST YOUR BINARY
# WITHOUT CHANGING A BYTE OF SOURCE. That is the ordinary order of work -- build, test, commit -- so the commit
# half declared a CORRECT binary stale for everybody who follows it. MEASURED here 2026-09-02, minutes after it
# landed: scrip built 20:06:35, newest src FILE 19:58:46 (binary genuinely current), newest src COMMIT 20:19:47,
# verdict REFUSED. ⭐ A false REFUSAL is far cheaper than the false OWED this function exists to stop -- it warns
# instead of blocking -- but it still costs a pointless rebuild and, worse, teaches seats to shrug at the check.
# ⭐ The file half loses nothing the commit half had: `git checkout` stamps NOW onto every file it updates, so a
# pull-after-build -- the case the commit half was reaching for -- moves the FILE mtimes too. HQ-27 is the law;
# this states it without crying wolf on the commit that follows a good build.
# ⛔ Caught by test_gate_s_artifacts_verifier_stale_binary_refuses.sh, which asserts the refusal PRINTS the src/
# timestamp it compared against: the gate went red the moment the printed number stopped being the file mtime.
# That is the whole reason the refusal quotes its own reference instead of just naming a verdict.
# ⛔ THE LOCAL COPY OF assert_binary_current IS GONE -- it now lives in lib_build_currency.sh beside
# assert_so_current, because SEVEN nm-grading scripts need the same question answered about the RUNTIME and a
# second copy is how one caller keeps the old rule after the rule changes (CLAUDE.md: shared authorities are
# sourced, never reimplemented). The lib RETURNS 2 where this file used to `exit 2`, so the exit stays here,
# at the call site, where a reader can see it.
. "$HERE/lib_build_currency.sh"
if [ "$SKIP_PRISTINE" -eq 1 ]; then
  assert_binary_current "$SCRIP_BIN" "$ROOT" || exit 2
  # ⛔ AND THE RUNTIME, which this preflight never checked (row stale-binary-preflight-also-covers-out-libscrip-rt-so):
  # out/libscrip_rt.so lags src/ exactly as ./scrip can, and a 13:37 .so against a 14:16 src read `exec_stmt` as
  # STILL EXPORTED after its deletion. The .so is what every `nm -D` verdict in the tree actually reads.
  assert_so_current "$SCRIP_RT" "$ROOT" || exit 2
fi

[ -d "$REAL_CORPUS/.git" ] || { echo "FATAL: corpus repo not found at $REAL_CORPUS"; exit 2; }

WORK="$(mktemp -d "${TMPDIR:-/tmp}/verify_s_owed.XXXXXX")" || { echo "FATAL: mktemp failed"; exit 2; }
trap 'rm -rf "$WORK"' EXIT

echo "=== util_verify_s_artifacts_owed: dry-run drift check ==="
echo "    corpus and SCRIP SOURCES are never written (the .s comparison runs in a scratch clone)."
echo "    THIS ROOT'S BUILD IS: $([ "$WANT_PRISTINE" -eq 1 ] && echo 'WIPED AND REBUILT (make pristine) -- ./scrip is ABSENT for minutes' || echo 'brought current with an incremental make')"

if [ "$SKIP_PRISTINE" -eq 1 ]; then
  echo "--skip-pristine given: reusing $SCRIP_BIN as-is."
  [ -x "$SCRIP_BIN" ] || { echo "FATAL: scrip not built: $SCRIP_BIN"; exit 2; }
elif [ "$WANT_PRISTINE" -eq 1 ]; then
  echo "⛔⛔⛔ DESTRUCTIVE STEP STARTING IN $ROOT -- 'make pristine' WIPES out/ AND DELETES ./scrip ⛔⛔⛔"
  echo "⛔ For the next ~10-20 minutes this root HAS NO COMPILER. Any other measurement running here will see"
  echo "⛔ rc=127 (command not found) and will read as a catastrophic regression in whatever it was grading."
  echo "⛔ If you are that other measurement: your run is VOID, nothing regressed, re-run it when this finishes."
  echo "⛔ pid=$$ started $(date -u +%Y-%m-%dT%H:%M:%SZ)"
  if ! ( cd "$ROOT" && make pristine ) > "$WORK/pristine_build.log" 2>&1; then
    echo "FATAL: 'make pristine' failed — last 40 lines of $WORK/pristine_build.log:"
    tail -40 "$WORK/pristine_build.log"
    exit 2
  fi
  [ -x "$SCRIP_BIN" ] || { echo "FATAL: pristine build finished but $SCRIP_BIN is not executable"; exit 2; }
  echo "  pristine build OK: $SCRIP_BIN -- this root has a compiler again"
else
  echo "Bringing $ROOT current with an incremental make (the pristine is NOT required for a drift verdict --"
  echo "  Lon 2026-09-03 loosened HQ-27; a .s-currency check needs a CURRENT compiler, not a from-scratch one)..."
  if ! ( cd "$ROOT" && make ) > "$WORK/build.log" 2>&1; then
    echo "FATAL: 'make' failed — last 40 lines of $WORK/build.log:"
    tail -40 "$WORK/build.log"
    exit 2
  fi
  [ -x "$SCRIP_BIN" ] || { echo "FATAL: build finished but $SCRIP_BIN is not executable"; exit 2; }
  echo "  incremental build OK: $SCRIP_BIN"
fi

SCRATCH_CORPUS="$WORK/corpus"
echo "Cloning corpus (local, disposable, real repo untouched) -> $SCRATCH_CORPUS ..."
if ! git clone --quiet --local --no-hardlinks "$REAL_CORPUS" "$SCRATCH_CORPUS" 2>"$WORK/clone.err"; then
  echo "FATAL: local clone of $REAL_CORPUS failed:"; cat "$WORK/clone.err"; exit 2
fi

owed_total=0
trouble_total=0
report=()

echo "------------------------------------------------------------"
echo "--- benchmark (util_regen_benchmark_s_artifacts.sh) ---"
out="$WORK/benchmark.out"
pre="$(git -C "$SCRATCH_CORPUS" rev-parse HEAD)"
CORPUS="$SCRATCH_CORPUS" SCRIP="$SCRIP_BIN" \
  bash "$ROOT/scripts/util_regen_benchmark_s_artifacts.sh" "verify-dry-run-discard" > "$out" 2>&1
bench_rc=$?
post="$(git -C "$SCRATCH_CORPUS" rev-parse HEAD)"
sed 's/^/    /' "$out"
bench_owed=0
if [ "$pre" != "$post" ]; then
  bench_files="$(git -C "$SCRATCH_CORPUS" diff --name-only "$pre" "$post")"
  bench_owed="$(printf '%s\n' "$bench_files" | grep -c .)"
  owed_total=$((owed_total + bench_owed))
  report+=("benchmark: $bench_owed .s owed -> $(printf '%s' "$bench_files" | tr '\n' ' ')")
fi
bench_trouble="$(grep -cE 'AS-FAIL|EMIT-FAIL' "$out" || true)"
if [ "$bench_trouble" -gt 0 ] || [ "$bench_rc" -ne 0 ]; then
  trouble_total=$((trouble_total + (bench_trouble > 0 ? bench_trouble : 1)))
  report+=("benchmark: TROUBLE (rc=$bench_rc, $bench_trouble AS-FAIL/EMIT-FAIL line(s)) — see log above, current compiler cannot cleanly produce one or more benchmark .s")
fi
echo "    [benchmark] rc=$bench_rc owed=$bench_owed trouble=$bench_trouble"

echo "------------------------------------------------------------"
echo "--- demo (util_regen_demo_s_artifacts.sh) ---"
out="$WORK/demo.out"
pre="$(git -C "$SCRATCH_CORPUS" rev-parse HEAD)"
CORPUS="$SCRATCH_CORPUS" SCRIP="$SCRIP_BIN" \
  bash "$ROOT/scripts/util_regen_demo_s_artifacts.sh" "verify-dry-run-discard" > "$out" 2>&1
demo_rc=$?
post="$(git -C "$SCRATCH_CORPUS" rev-parse HEAD)"
sed 's/^/    /' "$out"
demo_owed=0
if [ "$pre" != "$post" ]; then
  demo_files="$(git -C "$SCRATCH_CORPUS" diff --name-only "$pre" "$post")"
  demo_owed="$(printf '%s\n' "$demo_files" | grep -c .)"
  owed_total=$((owed_total + demo_owed))
  report+=("demo: $demo_owed .s owed -> $(printf '%s' "$demo_files" | tr '\n' ' ')")
fi
demo_missing="$(grep -c 'no \.sno$' "$out" || true)"
if [ "$demo_missing" -gt 0 ]; then
  trouble_total=$((trouble_total + demo_missing))
  report+=("demo: $demo_missing source(s) NOT FOUND at the expected flat path — corpus/demos was reorganized into per-family subfolders (commit db20f3cf) and this script was not updated to match; its 'No changes' verdict is NOT trustworthy, it never found the sources to compare. This is a script-repair item, not something a regen run can fix — flagged, not fixed, by this row.")
fi
demo_trouble="$(grep -cE 'compile failed|empty emit|assembler-rejected' "$out" || true)"
if [ "$demo_trouble" -gt 0 ]; then
  trouble_total=$((trouble_total + demo_trouble))
  report+=("demo: $demo_trouble compile/assemble failure(s) on existing sources — see log above")
fi
echo "    [demo] rc=$demo_rc owed=$demo_owed missing-sources=$demo_missing trouble=$demo_trouble"

echo "------------------------------------------------------------"
echo "--- prolog_bench (util_regen_prolog_bench_s_artifacts.sh) ---"
out="$WORK/prolog_bench.out"
pre="$(git -C "$SCRATCH_CORPUS" rev-parse HEAD)"
CORPUS="$SCRATCH_CORPUS" SCRIP="$SCRIP_BIN" BENCH_DIR="$SCRATCH_CORPUS/benchmarks/prolog/bench" \
  bash "$ROOT/scripts/util_regen_prolog_bench_s_artifacts.sh" "verify-dry-run-discard" > "$out" 2>&1
pb_rc=$?
post="$(git -C "$SCRATCH_CORPUS" rev-parse HEAD)"
sed 's/^/    /' "$out"
pb_owed=0
if [ "$pre" != "$post" ]; then
  pb_files="$(git -C "$SCRATCH_CORPUS" diff --name-only "$pre" "$post")"
  pb_owed="$(printf '%s\n' "$pb_files" | grep -c .)"
  owed_total=$((owed_total + pb_owed))
  report+=("prolog_bench: $pb_owed .s/.FENCED owed -> $(printf '%s' "$pb_files" | tr '\n' ' ')")
fi
pb_trouble="$(grep -cE 'REJECTED-BY-AS|TIMEOUT|^\s*ERROR ' "$out" || true)"
if [ "$pb_trouble" -gt 0 ]; then
  trouble_total=$((trouble_total + pb_trouble))
  report+=("prolog_bench: $pb_trouble REJECTED-BY-AS/TIMEOUT/ERROR line(s) — see log above (this script's own exit code is always 0, so only a text scan catches these)")
fi
echo "    [prolog_bench] rc=$pb_rc owed=$pb_owed trouble=$pb_trouble"

echo "------------------------------------------------------------"
echo "--- icon_bench (update_icon_bench_asm.sh CHECK=1 — native dry-run, no clone needed) ---"
out="$WORK/icon_bench.out"
CHECK=1 ICON_CORPUS="$REAL_CORPUS/benchmarks/icon" bash "$ROOT/scripts/update_icon_bench_asm.sh" > "$out" 2>&1
icon_rc=$?
sed 's/^/    /' "$out"
icon_owed="$(grep -c '^WOULD-' "$out" || true)"
if [ "$icon_owed" -gt 0 ]; then
  owed_total=$((owed_total + icon_owed))
  icon_names="$(grep '^WOULD-' "$out" | awk '{print $2}' | tr '\n' ' ')"
  report+=("icon_bench: $icon_owed .s owed -> $icon_names")
fi
icon_trouble="$(grep -c '^CERR' "$out" || true)"
if [ "$icon_trouble" -gt 0 ]; then
  trouble_total=$((trouble_total + icon_trouble))
  report+=("icon_bench: $icon_trouble CERR (genuine compile error) line(s) — see log above")
fi
if [ "$icon_rc" -ne 0 ] && [ "$icon_owed" -eq 0 ] && [ "$icon_trouble" -eq 0 ]; then
  # Caught empirically (row six-owed-verifier, 2026-08-24): the script's own refusal guard
  # `case "$CORPUS" in */icon|*/icon/*)` matches ANY path ending in /icon — including its
  # OWN correct default corpus/benchmarks/icon, not just the corpus/tests/icon rung-test tree it
  # means to exclude. Confirmed with ZERO env override (its true documented default): same
  # REFUSED. A rc!=0 with no WOULD-/CERR lines means it did not check anything — never
  # silently pass that as clean. Script bug, unrelated to this row; flagged, not fixed, here.
  icon_trouble=1
  trouble_total=$((trouble_total + 1))
  report+=("icon_bench: COULD NOT RUN AT ALL (rc=$icon_rc, no WOULD-/CERR lines — see log above). update_icon_bench_asm.sh's own */icon refusal guard self-matches its own default path; pre-existing script bug, not caused by this row.")
fi
echo "    [icon_bench] rc=$icon_rc owed=$icon_owed trouble=$icon_trouble"

echo "============================================================"
echo "S-ARTIFACTS-OWED-TOTAL: $owed_total"
echo "S-ARTIFACTS-TROUBLE-TOTAL: $trouble_total"
if [ "$owed_total" -eq 0 ] && [ "$trouble_total" -eq 0 ]; then
  echo "VERDICT: CLEAN — nothing owed across benchmark/demo/prolog_bench/icon_bench, all four checks actually ran."
  exit 0
else
  echo "VERDICT: OWED — $((owed_total + trouble_total)) item(s) need attention before this counts as current:"
  printf '  - %s\n' "${report[@]}"
  echo ""
  echo "  Regen (real tree, not this scratch check): bash scripts/util_regen_benchmark_s_artifacts.sh \"<rung>\" && bash scripts/util_regen_demo_s_artifacts.sh \"<rung>\" && bash scripts/util_regen_prolog_bench_s_artifacts.sh \"<rung>\""
  echo "  Icon emitter/lowerer touched this session? also: bash scripts/update_icon_bench_asm.sh"
  exit 1
fi
