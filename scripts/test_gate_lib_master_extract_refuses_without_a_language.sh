#!/usr/bin/env bash
# test_gate_lib_master_extract_refuses_without_a_language.sh -- lib_master_extract.sh HAS NO DEFAULT MASTER: unset MASTER_DIR and
# no MASTER_LANG is a REFUSAL rc=2, never a quiet snobol4 (coo 2026-09-16; hq_snocone's witness, hq_pascal's precedent lib_ladder.sh
# line 49; row lib-master-extract-defaults-master-dir-to-the-snobol4-master-when-unset-instead-of-refusing).
#
# THE DEFECT: line 72 read MASTER_DIR from the environment OR ELSE defaulted to corpus/tests/snobol4, so a consumer that forgot to
# set it graded the SNOBOL4 master and said nothing; entry names collide across masters, so the wrong-language answer was
# well-formed and plausible (hq_snocone's named-entry arm: three Snocone entries read as SNOBOL4 entries of the same name).
#
# ARMS: (a) MASTER_DIR and MASTER_LANG both unset: sourcing refuses rc=2 naming both variables; (b) MASTER_LANG=icon derives
# corpus/tests/icon and ALL.icn; (c) MASTER_DIR set explicitly is honoured as given; (d) an unknown MASTER_LANG refuses rc=2;
# (e) CENSUS: every script that sources the library sets MASTER_DIR or MASTER_LANG (inline or before), printed as a count.
# FAIL_ONCE=1 plants one consumer without either variable into the census farm, to prove arm (e) trips.
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; LIB="$HERE/lib_master_extract.sh"
[ -f "$LIB" ] || { echo "⛔ REFUSED-TO-GRADE: no $LIB"; exit 2; }
fails=0; checks=0; ck(){ checks=$((checks+1)); if [ "$1" = ok ]; then printf '  ok    %s\n' "$2"; else printf '  FAIL  %s\n' "$2"; fails=$((fails+1)); fi; }
echo "=== gate: lib_master_extract has no default master ==="
out="$(env -u MASTER_DIR -u MASTER_LANG bash -c ". '$LIB'" 2>&1)"; rc=$?
[ "$rc" = 2 ] && grep -q 'MASTER_DIR' <<<"$out" && grep -q 'MASTER_LANG' <<<"$out" && ck ok "(a) both unset: REFUSED rc=2 naming MASTER_DIR and MASTER_LANG" || ck no "(a) rc=$rc -- got: $(head -1 <<<"$out" | cut -c1-140)"
out="$(env -u MASTER_DIR MASTER_LANG=icon bash -c ". '$LIB' && printf '%s\n%s\n' \"\$MASTER_DIR\" \"\$MASTER_SNO\"" 2>&1)"; rc=$?
[ "$rc" = 0 ] && grep -q 'corpus/tests/icon$' <<<"$out" && grep -q 'ALL\.icn$' <<<"$out" && ck ok "(b) MASTER_LANG=icon derives corpus/tests/icon and ALL.icn" || ck no "(b) rc=$rc -- got: $out"
out="$(env -u MASTER_LANG MASTER_DIR=/tmp/explicit-master bash -c ". '$LIB' && printf '%s\n' \"\$MASTER_SNO\"" 2>&1)"; rc=$?
[ "$rc" = 0 ] && [ "$(tail -1 <<<"$out")" = "/tmp/explicit-master/ALL.sno" ] && ck ok "(c) an explicit MASTER_DIR is honoured as given" || ck no "(c) rc=$rc -- got: $out"
out="$(env -u MASTER_DIR MASTER_LANG=cobol bash -c ". '$LIB'" 2>&1)"; rc=$?
[ "$rc" = 2 ] && grep -q "cobol" <<<"$out" && ck ok "(d) an unknown MASTER_LANG refuses rc=2 naming it" || ck no "(d) rc=$rc -- got: $(head -1 <<<"$out" | cut -c1-120)"
W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT; cp "$HERE"/*.sh "$W"/ 2>/dev/null
[ -n "${FAIL_ONCE:-}" ] && printf '#!/usr/bin/env bash\n. "$(dirname "$0")/lib_master_extract.sh"\n' > "$W/zz_planted_consumer.sh"
missing=""; n=0
for f in "$W"/*.sh; do
  b="$(basename "$f")"; [ "$b" = lib_master_extract.sh ] && continue
  grep -v '^\s*#' "$f" | grep -q 'lib_master_extract' || continue
  n=$((n+1))
  grep -qE 'MASTER_DIR=|MASTER_LANG=' "$f" || missing="$missing $b"
done
[ -z "$missing" ] && ck ok "(e) CENSUS: all $n consumers set MASTER_DIR or MASTER_LANG before sourcing" || ck no "(e) CENSUS: $n consumers, these rely on a default that no longer exists:$missing"
echo "population: $checks arm(s) graded, $fails FAIL; consumers: $n"
[ "$fails" = 0 ] && { echo "GATE PASS [lib_master_extract_refuses_without_a_language]: $checks of $checks arms hold"; exit 0; }
echo "⛔ GATE RED [lib_master_extract_refuses_without_a_language]: $fails of $checks arms FAIL"; exit 1
