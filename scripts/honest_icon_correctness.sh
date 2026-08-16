#!/usr/bin/env bash
# icon_correctness.sh — REAL output diff, not banner diff.
# post.icn suppresses program output unless OUTPUT is set (write := 1 idiom).
# With OUTPUT=1 the true program output lies strictly between the marker line
# "*** Benchmarking with output ***" and the "<name> elapsed time =" line.
# We extract exactly that window from both engines and diff it.
set -u
SCRIPDIR="${SCRIPDIR:-/home/claude/SCRIP}"
SCRIP="$SCRIPDIR/scrip"; RTDIR="$SCRIPDIR/out"
ICONM="${ICONM:-${ORACLE_BIN:+${ORACLE_BIN%/bin}}}"; ICONM="${ICONM:-/home/claude/icon-build}"
ICONT="$ICONM/bin/icont"; ICONX="$ICONM/bin/iconx"
[ -x "$ICONT" ] && [ -x "$ICONX" ] || { echo "FATAL: no Icon oracle at $ICONM/bin (icont/iconx). Set ICONM= or ORACLE_BIN=." >&2; exit 2; }
CORPUS_SRC="${CORPUS_SRC:-/home/claude/corpus/benchmarks/icon}"
[ -d "$CORPUS_SRC" ] || { echo "FATAL: no benchmark corpus at $CORPUS_SRC. Set CORPUS_SRC=." >&2; exit 2; }
WORK="$(mktemp -d /tmp/icn_corr_XXXXXX)"
export PATH="$ICONM/bin:$PATH"
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
  OUTPUT=1 timeout 120 "$ICONX" "$WORK/$name.icx" "${a[@]}" >"$WORK/$name.orc" 2>/dev/null <"$sin"
  orc_rc=$?
  ( cd "$CORPUS" && timeout 120 "$SCRIP" --compile --target=x86 "$name.icn" "${ld[@]}" \
      >"$WORK/$name.s" 2>"$WORK/$name.cerr" )
  if [ -s "$WORK/$name.s" ] && gcc -no-pie "$WORK/$name.s" -L"$RTDIR" -lscrip_rt \
        -Wl,-rpath,"$RTDIR" -o "$WORK/$name.bin" 2>"$WORK/$name.lerr"; then
    OUTPUT=1 timeout 120 "$WORK/$name.bin" "${a[@]}" >"$WORK/$name.scr" 2>/dev/null <"$sin"
    scr_rc=$?
  else
    printf "%-9s | %-9s | %-9s | %-9s | %s\n" "$name" "-" "-" "EMITFAIL" "$(grep -m1 -oE 'FATAL.{0,30}' "$WORK/$name.cerr")"
    continue
  fi
  window "$WORK/$name.orc" > "$WORK/$name.orc.w"
  window "$WORK/$name.scr" > "$WORK/$name.scr.w"
  ol=$(wc -l <"$WORK/$name.orc.w"); sl=$(wc -l <"$WORK/$name.scr.w")
  if [ "$ol" -eq 0 ] && [ "$sl" -eq 0 ]; then v="NO-OUTPUT"; d="both empty - NOT a pass, harness or program produced nothing"
  elif cmp -s "$WORK/$name.orc.w" "$WORK/$name.scr.w"; then v="IDENTICAL"; d="-"
  else v="DIVERGE"; d="$(diff "$WORK/$name.orc.w" "$WORK/$name.scr.w" | head -2 | tr '\n' ' ' | cut -c1-40)"; fi
  printf "%-9s | %-9s | %-9s | %-9s | %s\n" "$name" "$ol" "$sl" "$v" "$d"
done
echo "workdir: $WORK"
