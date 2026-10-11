#!/usr/bin/env bash
# util_witness_sno_unknown_keyword_is_error_251.sh -- THE WITNESS of the unknown-keyword voice (the ceo, CEO-1651): SPITBOL reads or writes an unknown keyword
# (&FOO, &ANCHOR., &ANCHOR_1, &anchor under -f) as its run-time ERROR 251, on the read and on the write alike; SCRIP reads one as error 342 (an &constant read
# before its one-time assignment) and accepts the write as a definition with no error. GREEN (rc 0) when both modes of SCRIP raise 251 where sbl -bf raises 251,
# on every shape below; RED (rc 1) while any shape differs; REFUSE (rc 2) when the oracle or the binary is missing.
set -u
HERE="$(cd "$(dirname "$0")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
. "$HERE/lib_oracle_flags.sh" 2>/dev/null; SBL="${SBL:-$(sbl_correctness_bin 2>/dev/null || echo /home/resources/x64/bin/sbl)}"
[ -x "$ROOT/scrip" ] || { echo "⛔ REFUSE(2): no $ROOT/scrip -- make first"; exit 2; }
[ -x "$SBL" ] || { echo "⛔ REFUSE(2): no SPITBOL oracle at $SBL"; exit 2; }
T=$(mktemp -d) || exit 2; trap 'rm -rf "$T"' EXIT
n=0; bad=0
while IFS= read -r stmt; do
    n=$((n + 1)); printf '%b\nEND\n' "$stmt" > "$T/w.sno"
    o=$("$SBL" -bf "$T/w.sno" </dev/null 2>&1 | grep -aoE 'ERROR [0-9]+' | head -1 | tr -d 'ERRO ')
    m3=$(timeout 8 "$ROOT/scrip" "$T/w.sno" </dev/null 2>&1 | grep -aoE 'error [0-9]+' | head -1 | tr -d 'eror ')
    m4=""
    if timeout 8 "$ROOT/scrip" --compile -o "$T/w.s" "$T/w.sno" </dev/null >/dev/null 2>&1 && gcc -m64 "$T/w.s" -L"$ROOT/out" -lscrip_rt -lm -Wl,-rpath,"$ROOT/out" -o "$T/w" 2>/dev/null; then
        m4=$(timeout 8 "$T/w" </dev/null 2>&1 | grep -aoE 'error [0-9]+' | head -1 | tr -d 'eror ')
    fi
    if [ "$o" = "$m3" ] && [ "$o" = "$m4" ]; then v="agree"; else v="DIFFER"; bad=$((bad + 1)); fi
    printf '  %-6s %-28s sbl=%-4s m3=%-4s m4=%-4s\n' "$v" "$(printf '%b' "$stmt" | tr '\t' ' ')" "${o:--}" "${m3:--}" "${m4:--}"
done <<'SHAPES'
\tOUTPUT = &FOO
\t&FOO = 1
\tOUTPUT = &ANCHOR.
\t&ANCHOR. = 1
\tOUTPUT = &ANCHOR_1
\tOUTPUT = &anchor
\t&FOO = 1\n\tOUTPUT = &FOO
SHAPES
echo "WITNESS sno-unknown-keyword-is-error-251: shapes=$n differ=$bad"
[ $bad -eq 0 ] && { echo "✅ GREEN: an unknown keyword is SPITBOL's error 251 in both modes on every shape"; exit 0; }
echo "⛔ RED: $bad of $n shapes read a different error number from sbl -bf"; exit 1
