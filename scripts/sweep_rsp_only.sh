#!/usr/bin/env bash
# sweep_rsp_only.sh — RSP-ONLY empirical census: PASS = rc==0 AND stdout==.ref, per mode.
# NO PIPE on the measured command (the census script's pipe-trap law).
# Usage: bash scripts/sweep_rsp_only.sh OUT.tsv DIR [DIR...]
set -u
SC="${SC:-/home/claude/SCRIP}"
OUT="$1"; shift
W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
: > "$OUT"
G3=0; G4=0; GT=0; DYN=0
for D in "$@"; do
  t=0; p3=0; p4=0
  for f in "$D"/*.sno; do
    [ -e "$f" ] || continue
    ref="${f%.sno}.ref"; [ -e "$ref" ] || continue
    n="$(basename "$f" .sno)"; t=$((t+1))
    o3="$(cd "$SC" && timeout 8 ./scrip --run "$f" < /dev/null 2>/dev/null)"; r3=$?
    s3=FAIL; [ $r3 -eq 0 ] && [ "$o3" = "$(cat "$ref")" ] && s3=PASS
    c3=$r3
    dyn=0
    if (cd "$SC" && timeout 25 ./scrip --compile "$f" > "$W/$n.s" 2>/dev/null) \
       && gcc -no-pie "$W/$n.s" -L"$SC/out" -lscrip_rt -lm -Wl,-rpath,"$SC/out" -o "$W/$n.prog" 2>/dev/null; then
        grep -q 'rsp + -1\]' "$W/$n.s" && dyn=1
        o4="$(timeout 8 "$W/$n.prog" < /dev/null 2>/dev/null)"; r4=$?
        s4=FAIL; [ $r4 -eq 0 ] && [ "$o4" = "$(cat "$ref")" ] && s4=PASS
        c4=$r4
    else s4=BUILDFAIL; c4=-1; fi
    [ "$s3" = PASS ] && p3=$((p3+1)); [ "$s4" = PASS ] && p4=$((p4+1))
    [ $dyn -eq 1 ] && DYN=$((DYN+1))
    printf '%s\t%s\t%s\t%s\t%d\t%d\t%d\n' "$D" "$n" "$s3" "$s4" "$c3" "$c4" "$dyn" >> "$OUT"
  done
  [ $t -gt 0 ] && printf '%-46s n=%-4d m3=%-4d m4=%d\n' "$(basename "$D")" "$t" "$p3" "$p4"
  G3=$((G3+p3)); G4=$((G4+p4)); GT=$((GT+t))
done
echo "TOTAL n=$GT  m3 PASS=$G3  m4 PASS=$G4  dyn-offset(.s carries [rsp + -1])=$DYN"
