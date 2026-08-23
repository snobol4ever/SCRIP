#!/usr/bin/env bash
# icon_correctness.sh — REAL output diff, not banner diff.
# post.icn suppresses program output unless OUTPUT is set (write := 1 idiom).
# With OUTPUT=1 the true program output lies strictly between the marker line
# "*** Benchmarking with output ***" and the "<name> elapsed time =" line.
# We extract exactly that window from both engines and diff it.
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
S4A="${S4E_ASSETS:-$([ -d "$S4E/x64" ] && echo "$S4E" || echo /home/resources)}"   # D-17b: ASSET root -- oracles/vendor trees live at the HQ root on this machine (Lon: seats carry ONLY .github/SCRIP/corpus); a root owning its own x64 (HQ, or a full standalone clone-set) is self-contained.
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
. "$HERE/lib_oracle_flags.sh" 2>/dev/null || { echo "REFUSING: cannot load lib_oracle_flags.sh -- the ONE oracle-flag authority (s200/s255), Icon-aware since row icon-oracle-accessors-shared." >&2; exit 3; }
SCRIPDIR="${SCRIPDIR:-$S4E/SCRIP}"
SCRIP="$SCRIPDIR/scrip"; RTDIR="$SCRIPDIR/out"
ICONT="${ICONT:-$(icont_bin)}" || exit 2
ICONX="${ICONX:-$(iconx_bin)}" || exit 2
CORPUS_SRC="${CORPUS_SRC:-$S4E/corpus/benchmarks/icon}"
[ -d "$CORPUS_SRC" ] || { echo "FATAL: no benchmark corpus at $CORPUS_SRC. Set CORPUS_SRC=." >&2; exit 2; }
# ⛔ SCRATCH LIVES UNDER THE SEAT ROOT ON /home, NEVER BARE /tmp (row icon-sweep-scratch-hardening, after the 2026-08-23 s267 outage).
# /tmp is on the 125G ROOT partition; /home is a separate 503G partition. This script used to `mktemp -d /tmp/icn_corr_XXXXXX`
# with no cleanup trap and no output cap, and one diverging program (deal.icn, 6.79GB against the oracle's 285KB) filled the
# root filesystem to 100% and stopped nineteen sessions. The asymmetry between the two partitions is invisible in the word "/tmp".
SCRATCH_ROOT="${S4E_SCRATCH:-$S4E/.scratch}"
mkdir -p "$SCRATCH_ROOT" 2>/dev/null || { echo "REFUSING: cannot create scratch root $SCRATCH_ROOT -- set S4E_SCRATCH to a writable dir ON /home. Refusing rather than falling back to /tmp, which is the root partition." >&2; exit 2; }
WORK="$(mktemp -d "$SCRATCH_ROOT/icn_corr_XXXXXX")"
# ⛔ CLEANUP MUST SURVIVE THE FAILURE PATH. Cleanup that only runs on success is not cleanup: the failure path is exactly when
# the scratch is biggest, and the caller wraps this script in `timeout 900`, so a kill is the EXPECTED exit, not the rare one.
# ICN_KEEP_WORK=1 preserves the dir for debugging (and then says so, instead of printing a path that no longer exists).
icn_corr_cleanup() { [ "${ICN_KEEP_WORK:-0}" = "1" ] && return 0; [ -n "${WORK:-}" ] && [ -d "$WORK" ] && rm -rf "$WORK"; return 0; }
trap icn_corr_cleanup EXIT INT TERM
# ⛔ PER-PROGRAM OUTPUT CAP. A diverging program writes GBs inside the 120s per-program timeout. head -c bounds the write AND
# SIGPIPEs the producer, so a runaway dies instead of racing the disk. Hitting the cap is a REPORTABLE RESULT, never swallowed.
CAP="${ICN_OUT_CAP:-67108864}"
export PATH="$(dirname "$ICONT"):$PATH"
mkdir -p "$WORK/corpus"
cp "$CORPUS_SRC"/*.icn "$CORPUS_SRC"/*.dat "$WORK/corpus/" 2>/dev/null
CORPUS="$WORK/corpus"
( cd "$CORPUS" && "$ICONT" -c options.icn post.icn shuffle.icn >/dev/null 2>&1 )
IPXIN="$WORK/ipxref_input.icn"; cat "$CORPUS"/*.icn > "$IPXIN"
( cd "$CORPUS" && "$ICONT" -s -o micro.icx micro.icn >/dev/null 2>&1 && \
  "$ICONX" micro.icx 0.02 > "$WORK/micsum_input.txt" 2>/dev/null )

declare -A STDIN ARGS LINKDEPS
STDIN[concord]="$CORPUS/concord.dat"; ARGS[concord]=""
STDIN[deal]="";                       ARGS[deal]="-h 1000"
STDIN[geddump]="$CORPUS/geddump.dat"; ARGS[geddump]=""
STDIN[ipxref]="$IPXIN";               ARGS[ipxref]=""
STDIN[micsum]="$WORK/micsum_input.txt"; ARGS[micsum]=""
STDIN[queens]="";                     ARGS[queens]="-n10"
STDIN[rsg]="$CORPUS/rsg.dat";         ARGS[rsg]=""
STDIN[tgrlink]="";                    ARGS[tgrlink]="$CORPUS/tgrlink.dat"
STDIN[version]="";                    ARGS[version]=""
LINKDEPS[concord]="options.icn post.icn"
LINKDEPS[deal]="options.icn post.icn shuffle.icn"
LINKDEPS[ipxref]="options.icn post.icn"
LINKDEPS[queens]="options.icn post.icn"
LINKDEPS[rsg]="options.icn post.icn"
LINKDEPS[tgrlink]="options.icn"

# extract the true program-output window.
# Programs linked with post.icn bracket their real output between the marker line and the
# elapsed-time line. Programs WITHOUT post.icn (geddump, micsum, tgrlink — see LINKDEPS above)
# never emit the marker, so the awk window came back EMPTY for them; two empty windows then
# compared equal and printed IDENTICAL. That false green reported geddump — a KNOWN live defect
# diverging 12568L vs 13645L — as a pass. When the marker is absent the whole file IS the
# program output, so fall back to it rather than silently yielding nothing.
window() {
  if grep -q '\*\*\* Benchmarking with output \*\*\*' "$1" 2>/dev/null; then
    awk '/\*\*\* Benchmarking with output \*\*\*/{f=1;next} / elapsed time = /{f=0} f' "$1"
  else
    cat "$1"
  fi
}

progs="${*:-concord deal geddump ipxref micsum queens rsg tgrlink}"
printf "%-9s | %-9s | %-9s | %-9s | %s\n" "PROGRAM" "ORACLE L" "SCRIP L" "VERDICT" "FIRST DIFF"
printf -- "----------+-----------+-----------+-----------+---------------------------\n"
for name in $progs; do
  sin="${STDIN[$name]:-}"; [ -z "$sin" ] && sin=/dev/null
  read -ra a <<< "${ARGS[$name]}"
  read -ra ld <<< "${LINKDEPS[$name]:-}"
  ( cd "$CORPUS" && "$ICONT" -s -o "$WORK/$name.icx" "$name.icn" ) >/dev/null 2>&1
  OUTPUT=1 timeout 120 "$ICONX" "$WORK/$name.icx" "${a[@]}" 2>/dev/null <"$sin" | head -c "$CAP" >"$WORK/$name.orc"
  orc_rc=${PIPESTATUS[0]}
  ( cd "$CORPUS" && timeout 120 "$SCRIP" --compile --target=x86 "$name.icn" "${ld[@]}" \
      >"$WORK/$name.s" 2>"$WORK/$name.cerr" )
  if [ -s "$WORK/$name.s" ] && gcc -no-pie "$WORK/$name.s" -L"$RTDIR" -lscrip_rt \
        -Wl,-rpath,"$RTDIR" -o "$WORK/$name.bin" 2>"$WORK/$name.lerr"; then
    OUTPUT=1 timeout 120 "$WORK/$name.bin" "${a[@]}" 2>/dev/null <"$sin" | head -c "$CAP" >"$WORK/$name.scr"
    scr_rc=${PIPESTATUS[0]}
  else
    printf "%-9s | %-9s | %-9s | %-9s | %s\n" "$name" "-" "-" "EMITFAIL" "$(grep -m1 -oE 'FATAL.{0,30}' "$WORK/$name.cerr")"
    continue
  fi
  window "$WORK/$name.orc" > "$WORK/$name.orc.w"
  window "$WORK/$name.scr" > "$WORK/$name.scr.w"
  ol=$(wc -l <"$WORK/$name.orc.w"); sl=$(wc -l <"$WORK/$name.scr.w")
  # ⛔ A CAP HIT IS A RESULT, NOT A DETAIL TO SWALLOW. Reporting the truncation and NOT the runaway is how one defect hid as
  # three (s267: the same deal.icn runaway filled the disk, caused the 900s timeout, AND scored 0/1 at weight 15). If SCRIP
  # hit the cap and the oracle did not, that IS the finding -- announce it instead of letting a truncated prefix compare.
  cap_o=0; cap_s=0
  [ "$(stat -c%s "$WORK/$name.orc" 2>/dev/null || echo 0)" -ge "$CAP" ] && cap_o=1
  [ "$(stat -c%s "$WORK/$name.scr" 2>/dev/null || echo 0)" -ge "$CAP" ] && cap_s=1
  if [ "$cap_s" -eq 1 ] && [ "$cap_o" -eq 0 ]; then v="RUNAWAY"; d="SCRIP output hit the ${CAP}-byte cap, oracle did not - unbounded output, NOT a diff"
  elif [ "$cap_o" -eq 1 ] || [ "$cap_s" -eq 1 ]; then v="CAPPED"; d="output truncated at ${CAP} bytes (oracle=$cap_o scrip=$cap_s) - comparison is UNPROVEN, not a pass"
  elif [ "$ol" -eq 0 ] && [ "$sl" -eq 0 ]; then v="NO-OUTPUT"; d="both empty - NOT a pass, harness or program produced nothing"
  elif cmp -s "$WORK/$name.orc.w" "$WORK/$name.scr.w"; then v="IDENTICAL"; d="-"
  else v="DIVERGE"; d="$(diff "$WORK/$name.orc.w" "$WORK/$name.scr.w" | head -2 | tr '\n' ' ' | cut -c1-40)"; fi
  printf "%-9s | %-9s | %-9s | %-9s | %s\n" "$name" "$ol" "$sl" "$v" "$d"
done
if [ "${ICN_KEEP_WORK:-0}" = "1" ]; then echo "workdir: $WORK (KEPT -- ICN_KEEP_WORK=1; delete it yourself)"; else echo "workdir: $WORK (removed on exit; ICN_KEEP_WORK=1 to keep it)"; fi
