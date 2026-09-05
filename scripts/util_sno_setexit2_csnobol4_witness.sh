#!/usr/bin/env bash
# util_sno_setexit2_csnobol4_witness.sh -- grade csnobol4_suite/setexit2.sno against its .ref in both
# modes under the CSNOBOL4 dialect switch (row snobol4-csnobol4-setexit2-errtype-errtext-default-
# instead-of-fail-on-clean-exit). setexit2 exercises a SETEXIT trap firing on a clean END, a CSNOBOL4-
# only extension (ceo R1 dialect ruling): --compat=csnobol4 sets SCRIP_SETEXIT_END=1 for the CURRENT
# process only, so m3 (--run) picks it up as a CLI flag but the m4 (--compile) path does not -- the
# compiled binary is a separate process invocation, so the env var must be set again when RUNNING it,
# never at compile time. Getting this wrong silently reproduces the SPITBOL-default (trap never fires,
# empty output) rather than testing the CSNOBOL4 path this row is actually about.
# rc 0 = both modes byte-match .ref · rc 1 = a mode differs or fails to build · rc 2 = cannot measure.
set -u
here=$(cd "$(dirname "$0")" && pwd); cd "$here/.." || exit 2
SUITE="$here/../../corpus/packages/snobol4/csnobol4_suite"
sno="$SUITE/setexit2.sno"; ref="$SUITE/setexit2.ref"
[ -f "$sno" ] && [ -f "$ref" ] || { echo "REFUSED rc=2: missing $sno or $ref"; exit 2; }
[ -x ./scrip ] && [ -f out/libscrip_rt.so ] || { echo "REFUSED rc=2: build first (make) -- no ./scrip or out/libscrip_rt.so"; exit 2; }
RT_DIR="$PWD/out"; SCRIP_BIN="$PWD/scrip"    # the binary the freshness check above actually verified -- never another root's
T=$(mktemp -d); trap 'rm -rf "$T"' EXIT
rc=0
got3=$(cd "$SUITE" && timeout 8s "$SCRIP_BIN" --compat=csnobol4 setexit2.sno </dev/null 2>&1)
if [ "$got3" = "$(cat "$ref")" ]; then echo "PASS m3"; else echo "FAIL m3 (< ref, > scrip):"; diff <(cat "$ref") <(printf '%s\n' "$got3") | head -12; rc=1; fi
if (cd "$SUITE" && "$SCRIP_BIN" --compile -o "$T/w.s" setexit2.sno </dev/null >/dev/null 2>"$T/cerr") \
   && as -o "$T/w.o" "$T/w.s" 2>"$T/aerr" \
   && gcc "$T/w.o" -L"$RT_DIR" -lscrip_rt -lm -Wl,-rpath,"$RT_DIR" -o "$T/w.bin" 2>"$T/lerr"; then
  got4=$(cd "$SUITE" && SCRIP_SETEXIT_END=1 timeout 8s "$T/w.bin" </dev/null 2>&1)
  if [ "$got4" = "$(cat "$ref")" ]; then echo "PASS m4"; else echo "FAIL m4 (< ref, > scrip):"; diff <(cat "$ref") <(printf '%s\n' "$got4") | head -12; rc=1; fi
else
  echo "FAIL m4: compile/assemble/link failed"; sed 's/^/  /' "$T"/*err 2>/dev/null; rc=1
fi
echo "tree=$(git rev-parse --short HEAD 2>/dev/null) witness=$sno rc=$rc"
exit $rc
