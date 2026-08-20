#!/usr/bin/env bash
# scorecard_snobol4.sh — THE SNOBOL4 SCORECARD + META SCORE (Lon directive 2026-08-15 s91, Fable seat).
#
#   bash scripts/scorecard_snobol4.sh run    [--suites a,b,..] [--jobs N] [--out DIR]   # measure (long)
#   bash scripts/scorecard_snobol4.sh report [DIR]                                       # aggregate a results dir
#
# EVERY SNOBOL4 test source found in corpus/ + SCRIP/test is a member of exactly ONE suite below.  Every
# program is run in BOTH real modes (m3 = --run BINARY in-process, m4 = --compile -> gcc -no-pie -> run) and
# graded against ground truth = pinned sibling .ref if present AND the live x64 sbl oracle (same stdin, same
# cwd); PASS iff output equals EITHER (a pinned .ref may reflect behaviour sbl itself fails on; a live oracle
# guards against a stale pin).  A program whose oracle fails and has no pin is ORACLE_FAIL = unscoreable
# (excluded from the denominator, reported).
#
# STATUS classes (per mode): PASS DIFF TIMEOUT SIG<n> RC<n> COMPILE_FAIL ASM_FAIL(=link) ORACLE_FAIL
# SUITE SCORE = mean(m3 pass rate, m4 pass rate) over scoreable programs (both modes are law: 1:1).
# META SCORE  = weight-averaged suite score.  Weights encode COMPLEXITY (Lon: "how close to 100% complete");
# they are printed with every report so they can be tuned in one place — the WEIGHTS table below.
#
# results: <out>/results.tsv  suite<TAB>program<TAB>m3<TAB>m4<TAB>t3<TAB>t4<TAB>note   (appended as it goes;
# `report` works on a partial file, so a running sweep can be reported at any time).
# ⛔ EVERY `run` TRUNCATES <out>/results.tsv FIRST.  Two --suites runs sharing one --out is NOT a union: the
# second wipes the first, and `report` then scores a partial denominator that LOOKS like a whole board.  A
# META number comes from ONE run naming every suite; a --suites run gets its OWN --out dir (s182).
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
S4A="${S4E_ASSETS:-$([ -d "$S4E/x64" ] && echo "$S4E" || echo /home/claude)}"   # D-17b: ASSET root -- oracles/vendor trees live at the HQ root on this machine (Lon: seats carry ONLY .github/SCRIP/corpus); a root owning its own x64 (HQ, or a full standalone clone-set) is self-contained.
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SC="$(cd "$HERE/.." && pwd)"
CORPUS="${CORPUS:-$S4E/corpus}"
SBL="${SBL:-$S4A/x64/bin/sbl}"
SCRIP="${SCRIP:-$SC/scrip}"
DEMO="$CORPUS/programs/snobol4/demo"
# ---------------------------------------------------------------- WEIGHTS (name  weight  root  find-args  lib  run-timeout  norm)
# norm=ms : measurement lines (^iters:/^ms:) are DELETED from both sides before diff (timing is not correctness; the check: line is)
SUITES=$(cat <<'EOF'
beauty_self    20 SELF                                                -                          demo/beauty  90 -
beauty_suite   15 programs/snobol4/beauty_suite                       -maxdepth 1 -name *_driver.sno  SELFDIR      60 -
demos          15 programs/snobol4/demo                               -maxdepth 1 -name *.sno    SELFDIR      90 -
benchmarks     10 benchmarks/snobol4                                  -maxdepth 1 -name *.sno    SELFDIR      90 ms
bb_probes      10 probe/bb                                            -name *.sno                SELFDIR      20 -
patterns       10 crosscheck/patterns                                 -maxdepth 1 -name *.sno    demo/inc     20 -
crosscheck     10 crosscheck                                          -name *.sno -not -path */patterns/*  demo/inc  20 -
feature_test    5 SCRIPTEST                                           -name *.sno                CORPUS       20 -
probes_misc     5 probe                                               -name *.sno -not -path */bb/*  SELFDIR   20 -
csnobol4_suite  5 programs/csnobol4-suite                             -maxdepth 1 -name *.sno    SELFDIR      20 -
gimpel          5 programs/gimpel                                     -name *.sno                SELFDIR      20 -
lon             5 programs/lon                                        -name *.sno                SELFDIR      20 -
misc            3 MISC                                                -name *.sno                SELFDIR      20 -
EOF
)
MISC_DIRS="programs/snobol4/feat programs/snobol4/parser programs/snobol4/smoke programs/snobol4/jvm_j3 programs/snobol4/linker programs/snobol4/bench programs/dotnet programs/aisnobol"
# ---------------------------------------------------------------- stdin mapping (family conventions from the board scripts)
stdin_for() {  # $1 = program path -> input file or /dev/null
  local p="$1" b d n; b="${p%.sno}"; d="$(dirname "$p")"; n="$(basename "$b")"
  if [ -f "$b.input" ]; then echo "$b.input"; return; fi
  if [ -f "$b.in" ]; then echo "$b.in"; return; fi
  case "$d" in
    "$DEMO") case "$n" in claws5*) echo "$DEMO/claws5.input";; treebank*) echo "$DEMO/treebank.input";; json*) echo "$DEMO/json.input";;
                          calculator*) echo "$DEMO/calculator.input";; porter*) echo "$DEMO/porter.input";; *) echo /dev/null;; esac; return;;
  esac
  echo /dev/null
}
# ---------------------------------------------------------------- one program, one line
run_one() {  # suite lib prog norm run_to
  local suite="$1" lib="$2" prog="$3" norm="$4" rto="$5"
  local d n in ref_pin ref_live have_pin=0 have_live=0 W st3 st4 t0 t3 t4 rc out note=""
  d="$(dirname "$prog")"; n="$(basename "$prog" .sno)"; in="$(stdin_for "$prog")"
  case "$lib" in SELFDIR) lib="$d";; CORPUS) lib="$CORPUS";; *) lib="$CORPUS/$lib";; esac
  W="$(mktemp -d)"; ulimit -s unlimited 2>/dev/null
  if [ "$suite" = beauty_self ]; then in="$prog"; fi
  # ---- ground truth
  [ -f "$d/$n.ref" ] && { cp "$d/$n.ref" "$W/pin"; have_pin=1; }
  local sblflags="-b -d512m -i64m"; [ "$suite" = beauty_self ] && sblflags="-bf -d512m -i64m"
  local ocwd="$d"; [ "$lib" = "$CORPUS" ] && ocwd="$CORPUS"
  (cd "$ocwd" && SETL4PATH=. timeout 60 "$SBL" $sblflags "$prog" < "$in" > "$W/live" 2>/dev/null); rc=$?
  [ $rc -eq 0 ] && have_live=1
  if [ $have_pin -eq 0 ] && [ $have_live -eq 0 ]; then
    echo -e "$suite\t${prog#$CORPUS/}\tORACLE_FAIL\tORACLE_FAIL\t0\t0\tsbl rc=$rc"; rm -rf "$W"; return; fi
  grade() {  # $1 = output file, $2 = rc  -> status
    local o="$1" r="$2"
    [ $r -eq 124 ] && { echo TIMEOUT; return; }
    [ $r -ge 128 ] && { echo "SIG$((r-128))"; return; }
    if [ "$norm" = ms ]; then sed -i '/^iters: [0-9][0-9]*$/d; /^ms: [0-9][0-9]*$/d' "$o"; [ $have_pin = 1 ] && sed -i '/^iters: [0-9][0-9]*$/d; /^ms: [0-9][0-9]*$/d' "$W/pin"; [ $have_live = 1 ] && sed -i '/^iters: [0-9][0-9]*$/d; /^ms: [0-9][0-9]*$/d' "$W/live"; fi  # BM-ONE (s153): measurement lines are DELETED both sides (refs hold only the check: line -- the live oracle); rewrite-to-N was for the retired stamped family
    { [ $have_pin = 1 ] && cmp -s "$o" "$W/pin"; } && { echo PASS; return; }
    { [ $have_live = 1 ] && cmp -s "$o" "$W/live"; } && { echo PASS; return; }
    [ $r -ne 0 ] && { echo "RC$r"; return; }
    echo DIFF
  }
  # ---- m3
  t0=$SECONDS
  (cd "$d" && SNO_LIB="$lib" timeout "$rto" "$SCRIP" --run "$prog" < "$in" > "$W/m3" 2>"$W/m3e"); rc=$?
  if [ ! -s "$W/m3" ] && [ $rc -ne 0 ] && grep -q 'emit_chain.*FAILED\|unresolved forward\|bb_emit_end\|[Pp]arse error\|syntax error\|COMPILE' "$W/m3e" 2>/dev/null; then st3=COMPILE_FAIL; else st3="$(grade "$W/m3" $rc)"; fi
  t3=$((SECONDS-t0))
  # ---- m4
  t0=$SECONDS
  if ! (cd "$d" && SNO_LIB="$lib" timeout 60 "$SCRIP" --compile "$prog" </dev/null > "$W/p.s" 2>/dev/null) || [ ! -s "$W/p.s" ]; then st4=COMPILE_FAIL
  elif ! gcc -no-pie "$W/p.s" -L"$SC/out" -lscrip_rt -lm -Wl,-rpath,"$SC/out" -o "$W/p.bin" 2>/dev/null; then st4=ASM_FAIL
  else
    (cd "$d" && SNO_LIB="$lib" timeout "$rto" "$W/p.bin" < "$in" > "$W/m4" 2>/dev/null); rc=$?
    st4="$(grade "$W/m4" $rc)"
  fi
  t4=$((SECONDS-t0))
  [ $have_pin = 1 ] && [ $have_live = 1 ] && ! cmp -s "$W/pin" "$W/live" && note="pin!=live"
  [ $have_live = 0 ] && note="pin-only"
  echo -e "$suite\t${prog#$CORPUS/}\t$st3\t$st4\t$t3\t$t4\t$note"
  rm -rf "$W"
}
export -f run_one stdin_for; export CORPUS SBL SCRIP SC DEMO
# ---------------------------------------------------------------- run
cmd_run() {
  set -f
  local only="" jobs=1 out=""
  while [ $# -gt 0 ]; do case "$1" in --suites) only="$2"; shift 2;; --jobs) jobs="$2"; shift 2;; --out) out="$2"; shift 2;; *) shift;; esac; done
  [ -z "$out" ] && out="$SC/test-results/scorecard-$(date +%Y%m%d-%H%M%S)"
  mkdir -p "$out"; : > "$out/results.tsv"; echo "$out"
  [ -x "$SCRIP" ] || { echo "SKIP scrip not built"; exit 1; }
  echo "$SUITES" | while read -r name w root fargs_lib_rest; do
    [ -z "$name" ] && continue
    [ -n "$only" ] && ! echo ",$only," | grep -q ",$name," && continue
    # split trailing fields: lib rto norm are the last three tokens; find-args in between
    set -- $fargs_lib_rest; local nf=$#; local norm=${!nf}; local rto=${@:$((nf-1)):1}; local lib=${@:$((nf-2)):1}
    local fargs="${*:1:$((nf-3))}"
    local list="$out/list.$name"
    case "$root" in
      SELF)      echo "$DEMO/beauty/beauty.sno" > "$list";;
      SCRIPTEST) find "$SC/test" $fargs 2>/dev/null | sort > "$list";;
      MISC)      : > "$list"; for m in $MISC_DIRS; do find "$CORPUS/$m" $fargs 2>/dev/null; done | sort >> "$list";;
      *)         find "$CORPUS/$root" $fargs 2>/dev/null | sort > "$list";;
    esac
    [ "$fargs" = "-" ] && true
    echo "== $name: $(wc -l < "$list") programs (w=$w lib=$lib rto=$rto norm=$norm)  $(date +%H:%M:%S)"
    xargs -a "$list" -P "$jobs" -I{} bash -c 'run_one "$0" "$1" "$2" "$3" "$4"' "$name" "$lib" {} "$norm" "$rto" >> "$out/results.tsv" 2>>"$out/noise.log"
  done
  echo "DONE $(date +%H:%M:%S)"
  cmd_report "$out"
}
# ---------------------------------------------------------------- report
cmd_report() {
  local out="${1:-$(ls -d "$SC"/test-results/scorecard-* 2>/dev/null | tail -1)}"
  [ -f "$out/results.tsv" ] || { echo "no results at $out"; exit 1; }
  echo "SNOBOL4 SCORECARD — $(basename "$out")  (SCRIP $(cd "$SC" && git rev-parse --short HEAD 2>/dev/null), corpus $(cd "$CORPUS" && git rev-parse --short HEAD 2>/dev/null))  rows=$(wc -l < "$out/results.tsv")"
  printf '%-15s %3s %5s %5s %6s %6s %6s %6s %6s  %s\n' SUITE W N M3ok M4ok m3% m4% SCORE UNSCR "top failure classes"
  ${AWK:-awk} -F'\t' -v S="$SUITES" 'BEGIN{ n=split(S,L,"\n"); for(i=1;i<=n;i++){ split(L[i],f," "); W[f[1]]=f[2]; ORD[i]=f[1] } }
    { s=$1; if($3=="ORACLE_FAIL"){U[s]++; next} N[s]++; if($3=="PASS")P3[s]++; if($4=="PASS")P4[s]++; C[s","$3"/"$4]++;
      c3[s","$3]++; c4[s","$4]++ }
    END{ tw=0; ts=0;
      for(i=1;i<=n;i++){ s=ORD[i]; if(!(s in N) && !(s in U)) continue; nn=N[s]+0; p3=P3[s]+0; p4=P4[s]+0;
        r3=(nn?100*p3/nn:0); r4=(nn?100*p4/nn:0); sc=(r3+r4)/2; if(nn){tw+=W[s]; ts+=W[s]*sc}
        # top failure classes for this suite (m3/m4 pair keys)
        top=""; for(key in C){ split(key,kk,","); if(kk[1]!=s) continue; if(kk[2]=="PASS/PASS") continue; A[kk[2]]=C[key] }
        for(cnt=0;cnt<3;cnt++){ best=""; bv=-1; for(a in A){ if(A[a]>bv){ bv=A[a]; best=a } } if(best=="") break; top=top" "best"="bv; delete A[best] } delete A
        printf "%-15s %3d %5d %5d %5d %6.1f %6.1f %6.1f %6d %s\n", s, W[s], nn, p3, p4, r3, r4, sc, U[s]+0, top }
      printf "%-15s %3d %5s %5s %5s %6s %6s %6.1f\n", "META SCORE", tw, "", "", "", "", "", (tw?ts/tw:0) }' "$out/results.tsv"
  echo "(SCORE = mean of m3%,m4% per suite; META = weight-averaged; UNSCR = oracle failed & no pin; classes are m3/m4 status pairs)"
}
case "${1:-report}" in run) shift; cmd_run "$@";; report) shift; cmd_report "$@";; *) echo "usage: $0 run|report"; exit 2;; esac
