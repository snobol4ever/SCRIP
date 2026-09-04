#!/usr/bin/env bash
# test_snobol4_gimpel_suite.sh -- dedicated gate for corpus/packages/snobol4/gimpel (144 *_driver.sno
# programs; 145 further NAME.sno library modules exist alongside them but are structurally unscoreable
# by design -- see gimpel/README.md), minted for row
# snobol4-gimpel-aisnobol-dotnet-measured-with-dedicated-gates (hq_T 2026-09-03).
#
# THIN WRAPPER, not a reimplementation: scorecard_snobol4.sh already carries a dedicated, working
# `gimpel` suite row (weight 5, `-name *_driver.sno`, SELFDIR:include lib, 20s timeout) with mature
# oracle-liveness handling (sbl_died: an oracle that exits 0 while printing a fatal-report banner is
# UNSCR, never a silent PASS or a manufactured FAIL) -- reusing it is lower-risk than a parallel
# implementation of the exact same dialect traps scorecard_snobol4.sh's own history already paid for.
# This script only runs it scoped to `gimpel`, reads its results.tsv, and reprints the board in this
# suite's own dedicated-gate line format so the DONE-WHEN of the minting row can find it by name.
#
# Exit: 0 iff m3_fail=0 and m4_fail=0 over the SCORED (non-ORACLE_FAIL) denominator.
set -u
HERE="$(cd "$(dirname "$0")" && pwd)"; SD="$HERE/.."; ROOT="$(cd "$SD/.." && pwd)"
SCRIP="$SD/scrip"
[ -x "$SCRIP" ] || { echo "⛔ REFUSE(rc=2): no scrip binary at $SCRIP -- build first (make)"; exit 2; }
# ⛔⭐ STALE-BINARY PREFLIGHT (row harness-and-ladder-runner-refuse-on-a-stale-binary-like-the-artifact-regen-
# does, ceo -> hq_T 2026-09-04). The line above proves a binary EXISTS; this one proves it is the binary this
# tree describes. ceo's witness, twice on 2026-09-04: a 10:57 binary graded at 14:03 read RED, then GREEN after
# an incremental make -- a vendor board is exactly where that is least visible, because a plausible all-FAIL
# table is this class's normal output. NO LOGIC HERE: util_require_fresh.sh sources gate_require_fresh from
# lib_gate.sh, the ONE authority (hq_B 4c7253e99) -- never a second copy of the staleness rule.
"$HERE/util_require_fresh.sh" --gate test_snobol4_gimpel_suite "$SCRIP" "${RT_DIR:-$HERE/../out}/libscrip_rt.so" || exit 2
W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
bash "$HERE/scorecard_snobol4.sh" run --suites gimpel --out "$W" > "$W/run.log" 2>&1
rc=$?
TSV="$W/results.tsv"
[ -f "$TSV" ] || { echo "⛔ REFUSE(rc=2): scorecard_snobol4.sh produced no results.tsv (rc=$rc) -- run.log:"; cat "$W/run.log"; exit 2; }
TOTAL=$(awk -F'\t' 'END{print NR}' "$TSV")
UNSCR=$(awk -F'\t' '$3=="ORACLE_FAIL"' "$TSV" | wc -l)
M3P=$(awk -F'\t' '$3!="ORACLE_FAIL" && $3=="PASS"' "$TSV" | wc -l)
M3F=$(awk -F'\t' '$3!="ORACLE_FAIL" && $3!="PASS"' "$TSV" | wc -l)
M4P=$(awk -F'\t' '$3!="ORACLE_FAIL" && $4=="PASS"' "$TSV" | wc -l)
M4F=$(awk -F'\t' '$3!="ORACLE_FAIL" && $4!="PASS"' "$TSV" | wc -l)
SCRIP_HASH="$(git -C "$SD" rev-parse --short HEAD 2>/dev/null || echo '?')"
CORP_HASH="$(git -C "$ROOT/corpus" rev-parse --short HEAD 2>/dev/null || echo '?')"
SCORED=$((TOTAL-UNSCR))
echo "GIMPEL_BOARD total=$TOTAL scored=$SCORED unscr=$UNSCR m3_pass=$M3P m3_fail=$M3F m4_pass=$M4P m4_fail=$M4F -- SCRIP $SCRIP_HASH corpus $CORP_HASH RT_OPT=-O0 oracle=sbl-bf (via scorecard_snobol4.sh --suites gimpel)"
awk -F'\t' '$3=="ORACLE_FAIL"{printf "  UNSCR  %s  %s\n", $2, $7}' "$TSV"
awk -F'\t' '$3!="ORACLE_FAIL" && ($3!="PASS" || $4!="PASS"){printf "  RED    %s  m3=%s m4=%s%s\n", $2, $3, $4, ($7!="" ? "  "$7 : "")}' "$TSV"
[ "$M3F" = 0 ] && [ "$M4F" = 0 ]
