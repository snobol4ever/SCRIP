#!/usr/bin/env bash
# test_one_witness.sh — ONE program, m3 + m4 end-to-end vs the LIVE x64 sbl oracle.
# Usage: SCRIP_ARBNO_RBP=<arm> bash scripts/test_one_witness.sh <file.sno> [tag]
# Keeps the built .s/.bin in $W so a following gdb run uses the SAME artifact.
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
S4A="${S4E_ASSETS:-$([ -d "$S4E/x64" ] && echo "$S4E" || echo /home/claude)}"   # D-17b: ASSET root -- oracles/vendor trees live at the HQ root on this machine (Lon: seats carry ONLY .github/SCRIP/corpus); a root owning its own x64 (HQ, or a full standalone clone-set) is self-contained.
set -u
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
SBL="${SBL:-$S4A/x64/bin/sbl}"
F="$1"; TAG="${2:-run}"
b=$(basename "$F" .sno)
W=/tmp/onew.$TAG; mkdir -p "$W"
"$SBL" -b "$F" < /dev/null > "$W/$b.ora" 2>/dev/null; orc=$?
timeout 15 "$ROOT/scrip" --run "$F" < /dev/null > "$W/$b.m3" 2>/dev/null; m3rc=$?
if [ $m3rc -eq $orc ] && cmp -s "$W/$b.ora" "$W/$b.m3"; then m3="PASS"; else m3="rc=$m3rc/$orc"; cmp -s "$W/$b.ora" "$W/$b.m3" || m3="$m3,DIFF"; fi
s=$(timeout 15 "$ROOT/scrip" --compile "$F" < /dev/null 2>/dev/null)
if [ -z "$s" ]; then m4="EMITFAIL"; else
    printf '%s' "$s" > "$W/$b.s"
    if as -o "$W/$b.o" "$W/$b.s" 2>"$W/$b.aserr" && gcc -no-pie -o "$W/$b.bin" "$W/$b.o" "$ROOT/out/libscrip_rt.so" -Wl,-rpath,"$ROOT/out" 2>"$W/$b.lderr"; then
        timeout 15 "$W/$b.bin" < /dev/null > "$W/$b.m4" 2>/dev/null; m4rc=$?
        if [ $m4rc -eq $orc ] && cmp -s "$W/$b.ora" "$W/$b.m4"; then m4="PASS"; else m4="rc=$m4rc/$orc"; cmp -s "$W/$b.ora" "$W/$b.m4" || m4="$m4,DIFF"; fi
    else m4="ASM/LINK"; fi
fi
printf '%-28s arm=%-3s m3=%-12s m4=%-12s (oracle rc=%s, artifacts %s)\n' "$b" "${SCRIP_ARBNO_RBP:-default}" "$m3" "$m4" "$orc" "$W"
