#!/usr/bin/env bash
# test_gate_gc_scan_subject_saves_are_rooted.sh -- A SAVED SCAN SUBJECT IS A ROOT, NOT A POINTER THAT HAPPENS TO ALIAS A LIVE ONE.
# ⛔ THE MEASURED DEFECT (cfo 2026-09-18): gen_gc_roots visited the LIVE scan_subj and nothing else, so every entry of scan_saved[]
# (pushed by rt_scan_leave, read back by rt_scan_reenter) and every suspended coexpression's ScanState (rt_scan_state_capture, held
# on ctx->scan_state) carried a collected-heap subject pointer that NO root walk visited -- unmarked and, worse, unregistered for
# forwarding. It is the g_name_save class one stack over: the walk existed and looked complete, so every census that asks "is there
# a root walk for scanning" read it as rooted. Measured on the pre-cure tree: 40 of 41 collections of a plain Icon scanning
# generator carry exactly one such slot. It did not crash there because an EMITTED rooted holder aliased the same block -- an alibi,
# not a root, and it vanishes when F6 step 3 replaces the ranges with compile-time maps or when the ScanState outlives its stack.
# THE ARMS. (1) BEHAVIOURAL, and it is the discriminating one: SCRIP_GC_AUDIT_SLOTS asks the COLLECTOR'S OWN registered-slot set
# whether each heap-pointing saved subject was registered this collection; unrooted must be 0 at every collection, in BOTH media.
# Measured fail-once by deleting only the two cure lines and rebuilding: unrooted=1 at 40 collections, m3 and m4 (2026-09-18).
# (2) ⛔ THE FACT-RULE ARM, dynamic and not a grep: heap must be >= 1, so the gate REFUSES rc=2 rather than passing when the
# population is empty. That is what forbids the OTHER cure -- a later seat cannot green this gate by moving scan subjects off the
# collected heap (RULES.md: the cure for an unrooted holder is a ROOT, never a different allocator), because doing so drops heap to
# 0 and the gate refuses. (3) The witness's answer must stay the oracle's (icont/iconx: n=179240), so a cure that breaks scanning reds.
# A stale binary REFUSES rc=2 and a refusal is not a red.
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"; cd "$ROOT" || exit 2
bash scripts/util_require_fresh.sh >/dev/null 2>&1 || { echo "REFUSES rc=2: stale or missing ./scrip -- run make"; exit 2; }
W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
EXPECT="n=179240"
cat > "$W/w.icn" <<'ICN'
procedure main();
   local i, t, n;
   n := 0;
   every i := (repl("hello world ", 40) ? find("o")) do {
      t := repl("x", 2000);
      n := n + i + *t;
      };
   write("n=", n);
end
ICN
bad=0; refuse=0
audit_arm() {
    lbl="$1"; shift
    "$@" > "$W/out.txt" 2> "$W/err.txt"; r=$?
    hi=$(awk '/^\[GC-AUDIT\] scan-save/{for(i=1;i<=NF;i++){if($i~/^heap=/){split($i,a,"=");if(a[2]+0>m)m=a[2]+0}}} END{print m+0}' "$W/err.txt")
    un=$(awk '/^\[GC-AUDIT\] scan-save/{for(i=1;i<=NF;i++){if($i~/^unrooted=/){split($i,a,"=");if(a[2]+0>m)m=a[2]+0}}} END{print m+0}' "$W/err.txt")
    c=$(grep -c '^\[GC-AUDIT\] scan-save' "$W/err.txt")
    if [ "$r" -ne 0 ] || [ "$(tail -1 "$W/out.txt")" != "$EXPECT" ]; then echo "  $lbl RED: rc=$r answer=$(tail -1 "$W/out.txt") (oracle $EXPECT)"; bad=1; return; fi
    if [ "$c" -lt 1 ]; then echo "  $lbl REFUSES rc=2: no collection observed -- the witness no longer reaches the audit"; refuse=1; return; fi
    if [ "$hi" -lt 1 ]; then echo "  $lbl REFUSES rc=2: $c collection(s) but heap=0 everywhere -- scan subjects are no longer on the COLLECTED heap, so this gate cannot measure rooting (FACT RULE: the cure is a root, not a different allocator)"; refuse=1; return; fi
    if [ "$un" -ne 0 ]; then echo "  $lbl RED: $c collection(s), heap=$hi, unrooted=$un -- a saved scan subject is unregistered for forwarding"; bad=1; return; fi
    echo "  $lbl PASS: $c collection(s), heap=$hi, unrooted=0"
}
audit_arm "m3 " env SCRIP_GC_AUDIT_SLOTS=1 SCRIP_GC_STRESS=2 timeout 60 "$ROOT/scrip" "$W/w.icn"
if "$ROOT/scrip" --compile -o "$W/w.s" "$W/w.icn" < /dev/null 2>"$W/c.txt" && gcc "$W/w.s" -L "$ROOT/out" -lscrip_rt -lm -Wl,-rpath,"$ROOT/out" -o "$W/w.m4" 2>>"$W/c.txt"; then
    audit_arm "m4 " env SCRIP_GC_AUDIT_SLOTS=1 SCRIP_GC_STRESS=2 timeout 60 "$W/w.m4"
else
    echo "  m4  REFUSES rc=2: the witness did not compile or link -- $(tail -1 "$W/c.txt")"; refuse=1
fi
if [ "$bad" -ne 0 ]; then echo "GATE FAIL(1) [gc_scan_subject_saves_are_rooted]: a saved scan subject has no root"; exit 1; fi
if [ "$refuse" -ne 0 ]; then echo "GATE REFUSE(2) [gc_scan_subject_saves_are_rooted]: the gate could not measure what it asserts"; exit 2; fi
echo "GATE PASS(0) [gc_scan_subject_saves_are_rooted]: every heap-pointing saved scan subject is registered for forwarding in both media (2 arms, 0 red)"
