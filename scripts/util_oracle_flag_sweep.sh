#!/usr/bin/env bash
# util_oracle_flag_sweep.sh -- measure what the SPITBOL ORACLE FLAG is worth, per program, both arms.
#
# WHY THIS EXISTS.  scorecard_snobol4.sh runs the live oracle as `sbl -b` for 13 of its 14 suites and
# `sbl -bf` for beauty_self alone.  `-b` only suppresses the sign-on banner (manual l.6944); `-f` is the
# CASE-FOLDING switch (l.6945), and folding is SPITBOL's DEFAULT (l.1205, l.7049).  Per the manual's own
# words (l.11074-11077) `-f` is what you use "if compatibility with standard SNOBOL4 is desired", and
# RULES.md declares SCRIP IS CASE-SENSITIVE.  So the flag is a LANGUAGE-SEMANTICS SWITCH, not a cosmetic
# one, and which arm is "ground truth" decides what a whole class of programs is graded against.
#
#   sweep      <outdir> [jobs]   3 arms per program: -b, -b again (CONTROL), -bf.  Tags each row
#                                same | MOVER | FLAKY.  FLAKY = the two SAME-FLAG control runs disagree,
#                                so that program cannot testify about the flag at all -- without this arm
#                                timing-bearing and runaway programs read as false MOVERs (measured: the
#                                2-arm form reported 103 movers where 3-arm reports 77 + 26 flaky).
#   transition <outdir>          for every MOVER, does flipping the LIVE arm change the scorecard VERDICT?
#                                Replays grade(): PASS iff output == pinned .ref OR == live arm (live counts
#                                only when the oracle exited 0).  Reports m3 and m4 before -> after.
#
# ⛔ corpus/programs/lon/ IS OFF LIMITS (RULES.md ABSOLUTE RULES; Lon s186).  TWO INDEPENDENT LOCKS: the
#    `lon` suite is not enumerated at all, AND every program path is re-checked before it is run.  This
#    script is therefore usable where scorecard_snobol4.sh currently is not.
# ⛔ ^iters:/^ms: lines are deleted from every arm before hashing (the scorecard's own norm=ms rule).
set -u
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"
S4A="${S4E_ASSETS:-$([ -d "$S4E/x64" ] && echo "$S4E" || echo /home/claude)}"
SC="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
CORPUS="${CORPUS:-$S4E/corpus}"; SBL="${SBL:-$S4A/x64/bin/sbl}"; SCRIP="${SCRIP:-$SC/scrip}"
DEMO="$CORPUS/programs/snobol4/demo"; NRM='/^iters: [0-9][0-9]*$/d; /^ms: [0-9][0-9]*$/d'
[ -x "$SBL" ] || { echo "FATAL: no oracle at $SBL -- a board without it prints a plausible all-FAIL table (PLAN.md 1b)"; exit 1; }
SUITES=$(cat <<'EOF'
beauty_self    SELF                                                -                          demo/beauty
beauty_suite   programs/snobol4/beauty_suite                       -maxdepth 1 -name *_driver.sno  SELFDIR
demos          programs/snobol4/demo                               -maxdepth 1 -name *.sno    SELFDIR
benchmarks     benchmarks/snobol4                                  -maxdepth 1 -name *.sno    SELFDIR
bb_probes      probe/bb                                            -name *.sno                SELFDIR
patterns       crosscheck/patterns                                 -maxdepth 1 -name *.sno    demo/inc
crosscheck     crosscheck                                          -name *.sno -not -path */patterns/*  demo/inc
feature_test   SCRIPTEST                                           -name *.sno                CORPUS
probes_misc    probe                                               -name *.sno -not -path */bb/*  SELFDIR
csnobol4_suite programs/csnobol4-suite                             -maxdepth 1 -name *.sno    SELFDIR
gimpel         programs/gimpel                                     -name *.sno                SELFDIR:programs/include
misc           MISC                                                -name *.sno                SELFDIR
EOF
)
MISC_DIRS="programs/snobol4/feat programs/snobol4/parser programs/snobol4/smoke programs/snobol4/jvm_j3 programs/snobol4/linker programs/snobol4/bench programs/dotnet programs/aisnobol"
stdin_for() { local p="$1" b d n; b="${p%.sno}"; d="$(dirname "$p")"; n="$(basename "$b")"
  [ -f "$b.input" ] && { echo "$b.input"; return; }; [ -f "$b.in" ] && { echo "$b.in"; return; }
  case "$d" in "$DEMO") case "$n" in claws5*) echo "$DEMO/claws5.input";; treebank*) echo "$DEMO/treebank.input";; json*) echo "$DEMO/json.input";;
    calculator*) echo "$DEMO/calculator.input";; porter*) echo "$DEMO/porter.input";; *) echo /dev/null;; esac; return;; esac; echo /dev/null; }
sc_libpath() { local spec="$1" pd="$2" e out="" oi="$IFS"; IFS=':'; for e in $spec; do case "$e" in SELFDIR) e="$pd";; CORPUS) e="$CORPUS";; *) e="$CORPUS/$e";; esac; out="${out:+$out:}$e"; done; IFS="$oi"; echo "$out"; }
libspec_for() { case "$1" in beauty_self) echo demo/beauty;; patterns|crosscheck) echo demo/inc;; feature_test) echo CORPUS;; gimpel) echo SELFDIR:programs/include;; *) echo SELFDIR;; esac; }
lon_guard() { case "$1" in */programs/lon/*) echo "REFUSED off-limits lon path: $1" >&2; return 1;; esac; return 0; }
one_prog() {
  local name="$1" lib="$2" prog="$3" d in L W r1 r2 rf m1 m2 mf tag
  lon_guard "$prog" || return
  d="$(dirname "$prog")"; in="$(stdin_for "$prog")"; L="$(sc_libpath "$lib" "$d")"
  [ "$name" = beauty_self ] && in="$prog"
  W="$(mktemp -d)"
  (cd "$d" && SETL4PATH=".:$L" timeout 60 "$SBL" -b  -d512m -i64m "$prog" < "$in" > "$W/b1" 2>/dev/null); r1=$?
  (cd "$d" && SETL4PATH=".:$L" timeout 60 "$SBL" -b  -d512m -i64m "$prog" < "$in" > "$W/b2" 2>/dev/null); r2=$?
  (cd "$d" && SETL4PATH=".:$L" timeout 60 "$SBL" -bf -d512m -i64m "$prog" < "$in" > "$W/f"  2>/dev/null); rf=$?
  sed -i "$NRM" "$W/b1" "$W/b2" "$W/f"
  m1=$(md5sum < "$W/b1" | cut -c1-12); m2=$(md5sum < "$W/b2" | cut -c1-12); mf=$(md5sum < "$W/f" | cut -c1-12)
  if [ "$m1" != "$m2" ] || [ "$r1" != "$r2" ]; then tag=FLAKY
  elif [ "$m1" != "$mf" ] || [ "$r1" != "$rf" ]; then tag=MOVER; else tag=same; fi
  printf '%s\t%s\t%s\t%s\t%s\t%s\t%s\n' "$name" "${prog#$CORPUS/}" "$r1" "$rf" "$m1" "$mf" "$tag"
  rm -rf "$W"
}
export -f one_prog stdin_for sc_libpath lon_guard; export CORPUS SBL DEMO NRM
cmd_sweep() {
  local out="${1:?usage: sweep <outdir> [jobs]}" jobs="${2:-8}" name root rest nf lib fargs list m
  mkdir -p "$out"; : > "$out/arms.tsv"; set -f
  echo "$SUITES" | while read -r name root rest; do
    [ -z "$name" ] && continue
    set -- $rest; nf=$#; lib=${!nf}; fargs="${*:1:$((nf-1))}"; list="$out/list.$name"
    case "$root" in
      SELF)      echo "$DEMO/beauty/beauty.sno" > "$list";;
      SCRIPTEST) find "$SC/test" $fargs 2>/dev/null | sort > "$list";;
      MISC)      : > "$list"; for m in $MISC_DIRS; do find "$CORPUS/$m" $fargs 2>/dev/null; done | sort >> "$list";;
      *)         find "$CORPUS/$root" $fargs 2>/dev/null | sort > "$list";;
    esac
    grep -v '/programs/lon/' "$list" > "$list.safe" && mv "$list.safe" "$list"
    echo "== $name: $(wc -l < "$list") programs  $(date +%H:%M:%S)" >&2
    xargs -a "$list" -P "$jobs" -I{} bash -c 'one_prog "$0" "$1" "$2"' "$name" "$lib" {} >> "$out/arms.tsv" 2>>"$out/noise.log"
  done
  echo "DONE $(date +%H:%M:%S)  rows=$(wc -l < "$out/arms.tsv")" >&2
  awk -F'\t' '{t[$7]++} END{printf "same=%d MOVER=%d FLAKY=%d\n", t["same"], t["MOVER"], t["FLAKY"]}' "$out/arms.tsv"
}
cmd_transition() {
  local out="${1:?usage: transition <outdir>}" suite prog rb rf mb mf tag full d in L W rcb rcf hp ref b3 a3 b4 a4 m4
  while IFS=$'\t' read -r suite prog rb rf mb mf tag; do
    [ "$tag" = MOVER ] || continue
    case "$prog" in /*) full="$prog";; *) full="$CORPUS/$prog";; esac
    [ -f "$full" ] || continue; lon_guard "$full" || continue
    d="$(dirname "$full")"; in="$(stdin_for "$full")"; L="$(sc_libpath "$(libspec_for "$suite")" "$d")"
    [ "$suite" = beauty_self ] && in="$full"
    W="$(mktemp -d)"
    (cd "$d" && SETL4PATH=".:$L" timeout 60 "$SBL" -b  -d512m -i64m "$full" < "$in" > "$W/b" 2>/dev/null); rcb=$?
    (cd "$d" && SETL4PATH=".:$L" timeout 60 "$SBL" -bf -d512m -i64m "$full" < "$in" > "$W/f" 2>/dev/null); rcf=$?
    ref="${full%.sno}.ref"; hp=0; [ -f "$ref" ] && { sed "$NRM" "$ref" > "$W/pin"; hp=1; }
    (cd "$d" && SNO_LIB="$L" timeout 30 "$SCRIP" --run "$full" < "$in" > "$W/m3" 2>/dev/null)
    m4=SKIP
    if (cd "$d" && SNO_LIB="$L" timeout 60 "$SCRIP" --compile "$full" </dev/null > "$W/p.s" 2>/dev/null) && [ -s "$W/p.s" ] \
       && gcc -no-pie "$W/p.s" -L"$SC/out" -lscrip_rt -lm -Wl,-rpath,"$SC/out" -o "$W/p.bin" 2>/dev/null; then
         (cd "$d" && SNO_LIB="$L" timeout 30 "$W/p.bin" < "$in" > "$W/m4o" 2>/dev/null); m4=RAN; fi
    sed -i "$NRM" "$W/b" "$W/f" "$W/m3"; [ "$m4" = RAN ] && sed -i "$NRM" "$W/m4o"
    verdict() { local o="$1" lv="$2" lrc="$3"
      { [ $hp = 1 ] && cmp -s "$o" "$W/pin"; } && { echo PASS; return; }
      { [ "$lrc" = 0 ] && cmp -s "$o" "$lv"; } && { echo PASS; return; }; echo FAIL; }
    b3=$(verdict "$W/m3" "$W/b" $rcb); a3=$(verdict "$W/m3" "$W/f" $rcf)
    if [ "$m4" = RAN ]; then b4=$(verdict "$W/m4o" "$W/b" $rcb); a4=$(verdict "$W/m4o" "$W/f" $rcf); else b4=NOBUILD; a4=NOBUILD; fi
    printf '%s\t%s\t%s\t%s\t%s\t%s\n' "$suite" "$prog" "$b3" "$a3" "$b4" "$a4"
    rm -rf "$W"
  done < "$out/arms.tsv" | tee "$out/transition.tsv"
  echo "-- m3 --"; awk -F'\t' '{print $3" -> "$4}' "$out/transition.tsv" | sort | uniq -c | sort -rn
  echo "-- m4 --"; awk -F'\t' '{print $5" -> "$6}' "$out/transition.tsv" | sort | uniq -c | sort -rn
}
case "${1:-}" in
  sweep)      shift; cmd_sweep "$@";;
  transition) shift; cmd_transition "$@";;
  *) echo "usage: $0 sweep <outdir> [jobs] | $0 transition <outdir>"; exit 2;;
esac
