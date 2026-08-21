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
gimpel          5 programs/gimpel                                     -name *_driver.sno         SELFDIR:programs/include   20 -
misc            3 MISC                                                -name *.sno                SELFDIR      20 -
EOF
)
# ⛔ LON IS EXCLUDED STRUCTURALLY, NOT SKIPPED AT RUN TIME (Lon in-chat 2026-08-20, via HQ; row `scorecard-drop-lon`, s189).  Lon ruled "We'll not run any programs/lon
# programs" over ALL of corpus/programs/lon/, and until s189 this script still executed that suite through run_one in BOTH engines and BOTH modes -- so RUNNING THE SNOBOL4
# SCORECARD WAS ITSELF AN INSTANCE OF THE VIOLATION (found by seat1 s185, .github 93aca5c8).  The `lon 5 programs/lon ...` row is DELETED from the table above rather than
# filtered at run time, because a run-time skip is re-openable by anyone passing --suites lon; a deleted row is not.  The guard below closes the other door: no suite may
# name that tree by ROOT or by LIB, so re-adding the row -- or quietly pointing some other suite's include path at it -- fails the script instead of running the programs.
# ⛔ THE WEIGHTS ARE LON'S KNOB: lon's 5 points are NOT redistributed.  The declared total is 113, deliberately short of the old 118, and where those 5 go is Lon's call.
# ⛔ OFF LIMITS MEANS NOT RUN, NOT DESTROYED: corpus/programs/lon/ stays exactly where it is (HQ-78: do not run, do not read into a transcript, do not scan, never delete).
case "$SUITES" in *programs/lon*) echo "⛔ scorecard_snobol4.sh: the suite table names programs/lon -- Lon ruled that tree is not to be run. Remove the entry; do not skip it at run time." >&2; exit 2;; esac
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
# ---------------------------------------------------------------- library search path -- ONE view, handed to BOTH engines
# The lib column is a COLON LIST of SELFDIR (the program's own dir) | CORPUS | <corpus-relative dir>, resolved here and given
# to SCRIP as SNO_LIB and to sbl as SETL4PATH.  s185 MEASURED: BOTH accept a colon list (scrip.c:941 strsep(":"); sbl tested
# A:B / B:A / A/:B/ all resolve, /nonexistent -> ERROR 285), so neither engine needs a pooled include dir.  Program dir first.
sc_libpath() {  # $1 = lib spec  $2 = program dir -> colon list of real dirs
  local spec="$1" pd="$2" e out="" oi="$IFS"; IFS=':'; for e in $spec; do case "$e" in SELFDIR) e="$pd";; CORPUS) e="$CORPUS";; *) e="$CORPUS/$e";; esac; out="${out:+$out:}$e"; done; IFS="$oi"; echo "$out"
}
# ---------------------------------------------------------------- oracle invocation -- ONE AUTHORITY (s189)
# ⛔ `-f` IS A LANGUAGE-SEMANTICS SWITCH, NOT A COSMETIC FLAG.  SPITBOL CASE-FOLDS NAMES BY DEFAULT (manual v3.7
# p.23/182 -- `Buffer`, `buFFer`, `BUFFER` are all one name; p.28/176 -- labels likewise) and `-f` turns folding OFF
# (p.162).  The manual itself names `-f` as THE flag for standard-SNOBOL4 compatibility (p.266 note 10), and
# RULES.md declares SCRIP CASE-SENSITIVE.  MEASURED s189: SCRIP agrees with `-f` and NOT with the folding default
# in three independent constructs -- distinct-case labels/variables, the special name `output` vs `OUTPUT`
# (p.192: special names take any case ONLY under folding), and `$('ABC')` (p.182: folding treats the string as
# upper-case when making the name, `-f` does not).  Grading against the folding default scores SCRIP on behaviour
# it is REQUIRED not to reproduce.  ⛔ NO SUITE MAY CARRY ITS OWN ORACLE FLAGS: the per-suite `beauty_self`
# exception this replaces is exactly why the fact stayed invisible for every other suite, and it was read as a
# SIGSEGV workaround (CLAUDE.md) when the SIGSEGV is the ERROR-217 report path that folding's PHANTOM duplicate
# labels walk into -- a genuine duplicate label SIGSEGVs under `-bf` too (measured s189).
sbl_flags() { echo "-bf -d512m -i64m"; }
# ---------------------------------------------------------------- oracle LIVENESS -- `rc -eq 0` IS NOT A LIVENESS SIGNAL (s191, row `gimpel-suite-harness`)
# ⛔ `sbl` EXITS 0 AFTER A FATAL ERROR, so testing rc alone ADMITS A DEAD ORACLE and the board then grades SCRIP against SPITBOL's
# error dump instead of against program output.  SMALLEST REPRO, and its one-ingredient passing sibling: `INPUT(.INPUT,5,,"nosuch.in")`
# between two OUTPUT statements prints the first, dies with the report, NEVER REACHES THE SECOND -- and exits 0; move the filename to
# the third argument and the identical program runs clean.  The cause is a DIALECT split the manual names outright (v3.7 p.12, p.224):
# Catspaw SPITBOL takes the filename as INPUT's THIRD argument, SNOBOL4+ puts it FOURTH, so a SNOBOL4+ program hands Catspaw an empty
# file spec -- ERROR 116.  MEASURED s191 in corpus/programs/gimpel: 8 library modules and 20 drivers exit 0 while printing the fatal
# report (ERROR 042/116/156/160/199/248), and the 10 of those drivers carrying NO pin had that dump as their ONLY ground truth -- every
# one a false red BY CONSTRUCTION.  The report is recognised by its two invariant parts TOGETHER, never either alone: the
# `FILE(N) : ERROR nnn -- text` line AND the `in statement N` locator of the termination block (`stmts executed` is NOT invariant -- a
# compile-time death omits it; RSEASON.sno is the witness).  FALSE-POSITIVE FLOOR MEASURED, NOT ASSUMED: zero of the corpus's .ref pins
# contain EITHER pattern, and the guard is negative-tested against each half alone.
# ⛔ THE OPPOSITE DIRECTION IS NOT GUARDED AND MUST NOT BE.  It is tempting to also call a 0-byte rc!=0 run "live and empty", but
# rc!=0 with no output is a GENUINE failure here: `END` alone exits 0, while an empty file and a program with no END statement exit 1.
# Admitting those would turn 134 correctly-UNSCR gimpel library modules into rows graded against empty output -- manufacturing 134
# vacuous passes, which is the very defect the pin/live mutual-silence row exists to kill.  ONE DIRECTION IS A FIX, BOTH IS A LIE.
sbl_died() { grep -qE ' : ERROR [0-9][0-9][0-9] -- ' "$1" && grep -qE '^in statement +[0-9]+$' "$1"; }
# ---------------------------------------------------------------- one program, one line
run_one() {  # suite lib prog norm run_to
  local suite="$1" lib="$2" prog="$3" norm="$4" rto="$5"
  local d n in ref_pin ref_live have_pin=0 have_live=0 W st3 st4 t0 t3 t4 rc out note=""
  d="$(dirname "$prog")"; n="$(basename "$prog" .sno)"; in="$(stdin_for "$prog")"
  lib="$(sc_libpath "$lib" "$d")"
  W="$(mktemp -d)"; ulimit -s unlimited 2>/dev/null
  if [ "$suite" = beauty_self ]; then in="$prog"; fi
  # ---- ground truth
  [ -f "$d/$n.ref" ] && { cp "$d/$n.ref" "$W/pin"; have_pin=1; }
  local sblflags="$(sbl_flags)"
  local ocwd="$d"; [ "$lib" = "$CORPUS" ] && ocwd="$CORPUS"
  (cd "$ocwd" && SETL4PATH=".:$lib" timeout 60 "$SBL" $sblflags "$prog" < "$in" > "$W/live" 2>/dev/null); rc=$?
  local ordead=""
  if [ $rc -eq 0 ]; then if sbl_died "$W/live"; then ordead=" fatal-report"; else have_live=1; fi; fi
  if [ $have_pin -eq 0 ] && [ $have_live -eq 0 ]; then
    echo -e "$suite\t${prog#$CORPUS/}\tORACLE_FAIL\tORACLE_FAIL\t0\t0\tsbl rc=$rc$ordead"; rm -rf "$W"; return; fi
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
  [ $have_live = 0 ] && note="pin-only$ordead"
  echo -e "$suite\t${prog#$CORPUS/}\t$st3\t$st4\t$t3\t$t4\t$note"
  rm -rf "$W"
}
export -f run_one stdin_for sc_libpath sbl_flags sbl_died; export CORPUS SBL SCRIP SC DEMO
# ---------------------------------------------------------------- run
cmd_run() {
  set -f
  local only="" jobs=1 out=""
  while [ $# -gt 0 ]; do case "$1" in --suites) only="$2"; shift 2;; --jobs) jobs="$2"; shift 2;; --out) out="$2"; shift 2;; *) shift;; esac; done
  # ⛔ AN UNKNOWN --suites NAME IS REFUSED, NOT IGNORED (s189).  The filter below is `grep -q ",$name,"` over the TABLE, so a name that is not in the table simply matches
  # nothing: `--suites lon` and `--suites crosschek` both used to run ZERO programs, truncate results.tsv, and then report a META over an empty or partial denominator that
  # LOOKS like a whole board -- the exact failure the s182 warning at the top of this file describes, reached by a typo instead of by two runs sharing one --out.
  local nm bad=""
  for nm in $(echo "$only" | tr ',' ' '); do
    echo "$SUITES" | awk 'NF{print $1}' | grep -qx -- "$nm" && continue
    case "$nm" in
      lon) echo "⛔ REFUSED --suites lon: Lon ruled corpus/programs/lon is not to be run, and the suite was DELETED from this scorecard at s189. There is no flag that runs it." >&2;;
      *)   echo "⛔ REFUSED --suites $nm: no such suite. Known: $(echo "$SUITES" | awk 'NF{print $1}' | tr '\n' ' ')" >&2;;
    esac; bad=1
  done
  [ -n "$bad" ] && exit 2
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
  # ⛔ A WITHDRAWN SUITE'S ROWS ARE DROPPED SILENTLY BY THE awk BELOW (it builds W[] and ORD[] from $SUITES, so an unknown suite is never iterated and never reaches tw/ts).
  # That is the CORRECT scoring -- a results.tsv measured before s189 still holds `lon` rows and they must not enter META -- but silence is how a plausible wrong number ships.
  local orph; orph="$(cut -f1 "$out/results.tsv" | sort -u | grep -vxF -f <(echo "$SUITES" | awk 'NF{print $1}') | tr '\n' ' ')"
  [ -n "$orph" ] && echo "⛔ IGNORED (rows measured, suite not in the table -- excluded from every number below, including META): $orph"
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
  # ⛔ UNSCR IS NAMED, NEVER MERELY COUNTED (s191, row `gimpel-suite-harness`).  An UNSCR row leaves the denominator ENTIRELY, so a
  # suite's score is computed over a smaller set than its row count suggests and the difference is invisible in a single integer.
  # CONVICTION: `UNSCR 136` on gimpel read as a property of SCRIP and was in fact 135 LIBRARY MODULES -- a DEFINE and a label, no END,
  # no output, not programs at all -- plus ONE real row.  Nobody could have known that from the count; naming them is what made it
  # visible in one line.  A row named here is a claim that the ORACLE could not run it and nothing pinned it, so it is also the list
  # to audit when a suite's N looks wrong.  The note column says WHY, including the case where sbl exited 0 and printed a fatal report.
  local nu; nu="$(awk -F'\t' '$3=="ORACLE_FAIL"' "$out/results.tsv" | wc -l)"
  if [ "$nu" -gt 0 ]; then
    echo "UNSCR ROWS — oracle could not run it AND no pin; excluded from every number above; all $nu named:"
    awk -F'\t' '$3=="ORACLE_FAIL"{printf "  %-15s %-60s %s\n",$1,$2,$7}' "$out/results.tsv" | sort
  fi
  # A row whose oracle died but which HAS a pin does not become UNSCR -- it is still scored, against the pin alone.  That is correct,
  # and it is also a weaker footing than a two-source row, so the count is reported rather than left for someone to derive.
  local np; np="$(awk -F'\t' '$7 ~ /^pin-only/' "$out/results.tsv" | wc -l)"
  local nd; nd="$(awk -F'\t' '$7 ~ /^pin-only fatal-report/' "$out/results.tsv" | wc -l)"
  # ⛔ `if`, NOT `[ ... ] && echo`: this is cmd_report's LAST statement, so a bare test would make the whole script exit 1 on any
  # board that happens to have zero pin-only rows -- a green board reporting failure.  Caught by running it, not by reading it.
  if [ "$np" -gt 0 ]; then echo "PIN-ONLY ROWS — scored against the pin because the live oracle did not deliver: $np (of which oracle exited 0 on a FATAL REPORT: $nd)"; fi
}
case "${1:-report}" in run) shift; cmd_run "$@";; report) shift; cmd_report "$@";; *) echo "usage: $0 run|report"; exit 2;; esac
