#!/usr/bin/env bash
# test_snobol4_aisnobol_suite.sh -- thin face over corpus_suite_harness.py for corpus/packages/snobol4/aisnobol
# (task every-vendored-package-absorbed-into-the-one-liner-or-multi-liner-python-harness-with-oracle-cut-refs,
# hq_T doorbell 2026-09-04, Lon verbatim: "You make the programs run, you measure the output, make a REF file,
# and place it into a ONE-LINER or a MULTI-LINER Python test harness."). SUPERSEDES the live-oracle-diff
# version of this script (git history holds it): the ALL.sno/ALL.ref container is built once by
# util_build_package_suite.py, with refs CUT FROM THE ORACLE (never hand-typed, never SCRIP's own output --
# see that script's header for the exclusion reasons), and this script becomes a thin face over the one
# suite grammar/grading authority (corpus_suite_harness.py) instead of a second, hand-rolled comparison
# loop duplicating oracle-invocation logic that already lives there.
#
# Regenerate the container (only needed if the shipped programs or the oracle changed):
#   python3 scripts/util_build_package_suite.py ../corpus/packages/snobol4/aisnobol
#
# Population: 8 shipped .sno programs. absorbed + excluded == shipped is asserted by the builder itself
# (refuses otherwise) -- not re-checked here, this script only grades what the container already holds.
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; SD="$HERE/.."; ROOT="$(cd "$SD/.." && pwd)"
SUITE="${AISNOBOL_SUITE:-$ROOT/corpus/packages/snobol4/aisnobol}"
SCRIP="$SD/scrip"; RT_DIR="$SD/out"
[ -x "$SCRIP" ] || { echo "⛔ REFUSE(rc=2): no scrip binary at $SCRIP -- build first (make)"; exit 2; }
# ⛔⭐ STALE-BINARY PREFLIGHT (row harness-and-ladder-runner-refuse-on-a-stale-binary-like-the-artifact-regen-
# does, ceo -> hq_T 2026-09-04). The line above proves a binary EXISTS; this one proves it is the binary this
# tree describes. ceo's witness, twice on 2026-09-04: a 10:57 binary graded at 14:03 read RED, then GREEN after
# an incremental make -- a vendor board is exactly where that is least visible, because a plausible all-FAIL
# table is this class's normal output. NO LOGIC HERE: util_require_fresh.sh sources gate_require_fresh from
# lib_gate.sh, the ONE authority (hq_B 4c7253e99) -- never a second copy of the staleness rule.
"$HERE/util_require_fresh.sh" --gate test_snobol4_aisnobol_suite "$SCRIP" "${RT_DIR:-$HERE/../out}/libscrip_rt.so" || exit 2
[ -f "$RT_DIR/libscrip_rt.so" ] || { echo "⛔ REFUSE(rc=2): no $RT_DIR/libscrip_rt.so"; exit 2; }
cd "$SD" || exit 2

if [ ! -f "$SUITE/ALL.sno" ] || [ ! -f "$SUITE/ALL.ref" ]; then
  echo "⛔ GATE REFUSES(rc=2): no container at $SUITE/ALL.{sno,ref} -- run:" >&2
  echo "   python3 scripts/util_build_package_suite.py ${SUITE#"$ROOT"/}" >&2
  exit 2
fi

OUT="$(python3 scripts/corpus_suite_harness.py run "$SUITE/ALL.sno" "$SUITE/ALL.ref" --modes m3,m4 2>&1)"
rc=$?
printf '%s\n' "$OUT"

board="$(printf '%s\n' "$OUT" | grep '^SUITE_BOARD ')"
if [ -z "$board" ]; then
  # ⛔ The harness printed no SUITE_BOARD line: it refused before measuring anything (see the REFUSING
  # line in $OUT above for why -- e.g. ALL.csv missing a `modes` column, corpus_suite_harness.py
  # c3948a321). Computing shipped/scored/PASS/FAIL from an empty $board silently manufactures a
  # blank-but-plausible AISNOBOL_BOARD line and, worse, persists it into SCORE.md as a real reading --
  # measured directly 2026-09-05 (seat02): every count field came back empty and still got written to
  # the shared leaderboard. An instrument that refused must say so, not print the pass/fail shape.
  echo "AISNOBOL_BOARD REFUSED -- harness printed no SUITE_BOARD line, see REFUSING output above"
  python3 "$HERE/util_score_row.py" write --lang snobol4 --column vendor --suite aisnobol --modes m3,m4 \
      --measurer "${S4E_SEAT:-}" \
      --text "aisnobol: REFUSED -- corpus_suite_harness.py produced no measurement this run, see \`test_snobol4_aisnobol_suite.sh\` REFUSING output (\`test_snobol4_aisnobol_suite.sh\`)" \
      || echo "WARNING SCORE.md NOT UPDATED -- record this row by hand (the REFUSING line above says why)"
  [ "$rc" -eq 0 ] && rc=2
  exit "$rc"
fi
field() { printf '%s\n' "$board" | grep -oE "$1=[0-9]+" | head -1 | cut -d= -f2; }
scored="$(field total)"
excl=0; [ -s "$SUITE/ALL.excluded.txt" ] && excl=$(grep -c . "$SUITE/ALL.excluded.txt")
shipped=$((scored + excl))
m3p="$(field m3_pass)"; m3f="$(field m3_fail)"; m3c="$(field m3_crash)"; m3h="$(field m3_hang)"
m4p="$(field m4_pass)"; m4f="$(field m4_fail)"; m4c="$(field m4_crash)"; m4h="$(field m4_hang)"

echo "AISNOBOL_BOARD shipped=$shipped scored=$scored excluded=$excl m3_PASS=$m3p m3_FAIL=$m3f m3_CRASH=$m3c m3_HANG=$m3h m4_PASS=$m4p m4_FAIL=$m4f m4_CRASH=$m4c m4_HANG=$m4h"
if [ -s "$SUITE/ALL.excluded.txt" ]; then
  echo "EXCLUDED (named, not counted as fail):"
  sed 's/^/  /' "$SUITE/ALL.excluded.txt"
fi

# ⭐ THE PACKAGE LOCKDOWN (Lon 2026-09-06, MASTER-PLAN sec THE PACKAGE LOCKDOWN): shipped is measured
# FRESH from the vendored dir every run, never derived from the container alone -- a file added after
# the container was last built must show up as ungraded, not silently vanish. ALL.sno is the container
# itself, never a shipped program.
real_shipped=$(find "$SUITE" -maxdepth 1 -name '*.sno' ! -name 'ALL.sno' | wc -l | tr -d ' ')
ungraded=$((real_shipped - scored - excl))
[ "$real_shipped" != "$shipped" ] && echo "⚠ CONTAINER STALE: $SUITE ships $real_shipped .sno file(s) now, container knows $shipped -- rebuild: python3 scripts/util_build_package_suite.py ${SUITE#"$ROOT"/}"
echo "PACKAGE_INVENTORY shipped=$real_shipped graded=$scored ungradable=$excl ungraded=$ungraded"

# ⛔ ONE LEADERBOARD (RULES.md FACT RULE, Lon 2026-09-03 ~16:05: "any run of a test suite by any session
# will update the ONE LEADERBOARD"). Records the board just printed into .github/SCORE.md -- runs nothing
# new. NON-FATAL BY DESIGN (matches test_icon_arizona_suite.sh's own convention): a bookkeeping failure
# must never turn a real measurement into a red gate for a reason unrelated to the code.
python3 "$HERE/util_score_row.py" write --lang snobol4 --column vendor --suite aisnobol --modes m3,m4 \
    --measurer "${S4E_SEAT:-}" --text "aisnobol $m3p/$scored m3 . $m4p/$scored m4 SCORED (of $shipped shipped, $excl excluded and named) . m3 FAIL=$m3f CRASH=$m3c HANG=$m3h . m4 FAIL=$m4f CRASH=$m4c HANG=$m4h (\`test_snobol4_aisnobol_suite.sh\`)" \
    || echo "WARNING SCORE.md NOT UPDATED -- record this row by hand (the REFUSED line above says why)"

exit $rc
