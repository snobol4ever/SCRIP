#!/usr/bin/env bash
# JSON-FENCE-JSTRBODY-CAS GATE -- row fence-jstrbody-cas item 1 (FENCE after jkey/jstring's closing quote,
# corpus/demos/snobol4/json/{json.sno,json-match-fence.sno}), graded on its own real DONE-WHEN target:
# the full 1.7MB corpus/demos/snobol4/json/citm_catalog.json, both files, both modes, byte-identical to the
# correctness oracle (sbl_correctness_bin, -bf per lib_oracle_flags.sh -- the s189 authority, never -b alone).
# Refs minted this session from a clean oracle run (rc=0, ~0.2s wall, match_ms=240 -- FENCE keeps this cheap;
# maxdepth=8 matches hq_P's independent citm measurement in FINDING-2026-08-23-hq_P-fence0-blob-floor-...).
# Item 2 (relocating jobject/jarray's FENCE from definition-site to use-site) is NOT exercised here and never
# will be by this gate: seat04 (FINDING-2026-08-22-seat04-json-alternate-af-spin-root-cause-flat-choice-record-
# rsp-drift.md §7b) proved it trades the af-spin hang for a SIGSEGV on recursive boxes via blob_choice_rbp_scan
# eligibility -- a receipted refusal, not an oversight; see FINDING-2026-08-29-seat10-fence-jstrbody-cas-citm-
# measured-item2-stays-refused.md.
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME
set -u
SCRIP="${SCRIP:-$S4E/SCRIP/scrip}"
RT="${RT:-$S4E/SCRIP/out}"
DEMO="${DEMO:-$S4E/corpus/demos/snobol4/json}"
PROBE="${PROBE:-$S4E/corpus/probe/json_fence_jstrbody_cas}"
pass=0; fail=0
chk() { if [ "$1" = 0 ]; then pass=$((pass+1)); else fail=$((fail+1)); echo "  FAIL: $2"; fi; }

if [ ! -x "$SCRIP" ]; then
  echo "  REFUSED TO GRADE: no scrip binary at $SCRIP -- build first (make pristine)"
  echo "JSON-FENCE-JSTRBODY-CAS GATE: PASS=0 FAIL=0 REFUSED"
  exit 2
fi
if [ ! -f "$DEMO/citm_catalog.json" ] || [ ! -f "$PROBE/citm_catalog_json.ref" ] || [ ! -f "$PROBE/citm_catalog_match_fence.ref" ]; then
  echo "  REFUSED TO GRADE: missing input or ref under $DEMO / $PROBE"
  echo "JSON-FENCE-JSTRBODY-CAS GATE: PASS=0 FAIL=0 REFUSED"
  exit 2
fi

"$SCRIP" --compile "$DEMO/json.sno" -o /tmp/gate_jfjc_json.s < /dev/null > /dev/null 2>&1
gcc -no-pie /tmp/gate_jfjc_json.s -L"$RT" -lscrip_rt -Wl,-rpath,"$RT" -lm -lpthread -o /tmp/gate_jfjc_json.bin 2>/dev/null
"$SCRIP" --compile "$DEMO/json-match-fence.sno" -o /tmp/gate_jfjc_jmf.s < /dev/null > /dev/null 2>&1
gcc -no-pie /tmp/gate_jfjc_jmf.s -L"$RT" -lscrip_rt -Wl,-rpath,"$RT" -lm -lpthread -o /tmp/gate_jfjc_jmf.bin 2>/dev/null

timeout 60 "$SCRIP" --run "$DEMO/json.sno" < "$DEMO/citm_catalog.json" > /tmp/gate_jfjc_m3_json.txt 2>/dev/null
diff -q /tmp/gate_jfjc_m3_json.txt "$PROBE/citm_catalog_json.ref" > /dev/null 2>&1; chk $? "m3 json.sno matches oracle on citm_catalog.json"

timeout 60 /tmp/gate_jfjc_json.bin < "$DEMO/citm_catalog.json" > /tmp/gate_jfjc_m4_json.txt 2>/dev/null
diff -q /tmp/gate_jfjc_m4_json.txt "$PROBE/citm_catalog_json.ref" > /dev/null 2>&1; chk $? "m4 json.sno matches oracle on citm_catalog.json"

timeout 60 "$SCRIP" --run "$DEMO/json-match-fence.sno" < "$DEMO/citm_catalog.json" > /tmp/gate_jfjc_m3_jmf.txt 2>/dev/null
diff -q /tmp/gate_jfjc_m3_jmf.txt "$PROBE/citm_catalog_match_fence.ref" > /dev/null 2>&1; chk $? "m3 json-match-fence.sno matches oracle on citm_catalog.json"

timeout 60 /tmp/gate_jfjc_jmf.bin < "$DEMO/citm_catalog.json" > /tmp/gate_jfjc_m4_jmf.txt 2>/dev/null
diff -q /tmp/gate_jfjc_m4_jmf.txt "$PROBE/citm_catalog_match_fence.ref" > /dev/null 2>&1; chk $? "m4 json-match-fence.sno matches oracle on citm_catalog.json"

# ⭐ REGRESSION LOCK, NOT JUST A CORRECTNESS CHECK: jstrbody's FENCE must still be textually present at both
# sites -- a future edit that silently drops it would still pass the byte-identical checks above (small/no
# backtrack either way on this input's happy path) while reintroducing the CAS-retention this row cured.
grep -qE "jkey[[:space:]]*=.*dq FENCE \(epsilon \. \*ekey\(\)\)" "$DEMO/json.sno"; chk $? "json.sno jkey still carries the trailing FENCE"
grep -qE "jstring[[:space:]]*=.*dq FENCE \(epsilon \. \*estr\(\)\)" "$DEMO/json.sno"; chk $? "json.sno jstring still carries the trailing FENCE"
grep -qE "jstring[[:space:]]*=.*ARBNO\(jescape jchunk\) '\"' FENCE" "$DEMO/json-match-fence.sno"; chk $? "json-match-fence.sno jstring still carries the trailing FENCE"

echo "JSON-FENCE-JSTRBODY-CAS GATE: PASS=$pass FAIL=$fail"
[ "$fail" = 0 ] || exit 1
