#!/usr/bin/env bash
# scorecard_snobol4.sh — THE SNOBOL4 SCORECARD + META SCORE (Lon directive 2026-08-15 s91, Fable seat).
#
#   bash scripts/scorecard_snobol4.sh run    [--suites a,b,..] [--jobs N] [--out DIR] [--force]   # measure (long)
#   bash scripts/scorecard_snobol4.sh report [DIR]                                       # aggregate a results dir
#   bash scripts/scorecard_snobol4.sh one <suite> <program> [N]                           # re-run ONE program N times, the way the board runs it
#   bash scripts/scorecard_snobol4.sh oracle <suite> <program> [outfile]                  # the ORACLE's own answer + a liveness status (for minting .ref)
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
S4A="${S4E_ASSETS:-$([ -d "$S4E/x64" ] && echo "$S4E" || echo /home/resources)}"   # D-17b: ASSET root -- oracles/vendor trees live at the HQ root on this machine (Lon: seats carry ONLY .github/SCRIP/corpus); a root owning its own x64 (HQ, or a full standalone clone-set) is self-contained.
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SC="$(cd "$HERE/.." && pwd)"
CORPUS="${CORPUS:-$S4E/corpus}"
SBL="${SBL:-$S4A/x64/bin/sbl}"
SCRIP="${SCRIP:-$SC/scrip}"
DEMO="$CORPUS/demo"
# ---------------------------------------------------------------- WEIGHTS (name  weight  root  find-args  lib  run-timeout  norm)
# norm=ms : measurement lines (^iters:/^ns:/^ms:) are DELETED from both sides before diff (timing is not correctness; the check: line is).
#           ⭐ s265: the `benchmarks` suite no longer needs it and no longer HAS it.  Those programs were wrappers that printed iters:/ns:/ms:;
#           they are standalone applications now (Lon: *"the *.sno file must look like and BE the REAL program application"*) and print only their
#           own answer, graded against a .ref minted from the oracle.  A norm filter with nothing to filter is a blindfold nobody is wearing --
#           removed rather than left in place, so the suite grades every byte the program emits.
#           ⛔ ns: WAS MISSING HERE AND IT COST THE WHOLE SUITE (s261): harness.inc grew a full-resolution `ns:` reading, the filter was
#           never extended, so the live side kept a nanosecond timing the pins never had and ALL 17 BENCHMARKS graded DIFF/DIFF = 0.0%.
#           A filter that lists measurement lines BY NAME must be extended whenever the harness prints a new one.
SUITES=$(cat <<'EOF'
beauty_self    20 SELF                                                -                          include 90 -
beauty_suite   15 snobol4/beauty_suite                       -maxdepth 1 -name *_driver.sno  SELFDIR      60 -
demos          15 demo                               -maxdepth 1 -name *.sno    SELFDIR      90 -
benchmarks     10 benchmarks/snobol4                                  -maxdepth 1 -name *.sno    SELFDIR      90 -
bb_probes      10 probe/bb                                            -name *.sno                SELFDIR      20 -
patterns       10 crosscheck/patterns                                 -maxdepth 1 -name *.sno    demo/inc     20 -
crosscheck     10 crosscheck                                          -name *.sno -not -path */patterns/*  demo/inc  20 -
feature_test    5 SCRIPTEST                                           -name *.sno                CORPUS       20 -
probes_misc     5 probe                                               -name *.sno -not -path */bb/*  SELFDIR   20 ms
csnobol4_suite  5 csnobol4-suite                             -maxdepth 1 -name *.sno    SELFDIR      20 -
gimpel          5 gimpel                                     -name *_driver.sno         SELFDIR:include   20 -
misc            3 MISC                                                -name *.sno                SELFDIR      20 -
EOF
)
# ⛔ LON IS EXCLUDED STRUCTURALLY, NOT SKIPPED AT RUN TIME (Lon in-chat 2026-08-20, via HQ; row `scorecard-drop-lon`, s189).  Lon ruled "We'll not run any lon
# programs" over ALL of corpus/lon/, and until s189 this script still executed that suite through run_one in BOTH engines and BOTH modes -- so RUNNING THE SNOBOL4
# SCORECARD WAS ITSELF AN INSTANCE OF THE VIOLATION (found by seat1 s185, .github 93aca5c8).  The `lon 5 lon ...` row is DELETED from the table above rather than
# filtered at run time, because a run-time skip is re-openable by anyone passing --suites lon; a deleted row is not.  The guard below closes the other door: no suite may
# name that tree by ROOT or by LIB, so re-adding the row -- or quietly pointing some other suite's include path at it -- fails the script instead of running the programs.
# ⛔ THE WEIGHTS ARE LON'S KNOB: lon's 5 points are NOT redistributed.  The declared total is 113, deliberately short of the old 118, and where those 5 go is Lon's call.
# ⛔ OFF LIMITS MEANS NOT RUN, NOT DESTROYED: corpus/lon/ stays exactly where it is (HQ-78: do not run, do not read into a transcript, do not scan, never delete).
case "$SUITES" in *programs/lon*) echo "⛔ scorecard_snobol4.sh: the suite table names lon -- Lon ruled that tree is not to be run. Remove the entry; do not skip it at run time." >&2; exit 2;; esac
MISC_DIRS="snobol4/feat snobol4/parser snobol4/smoke snobol4/jvm_j3 snobol4/linker snobol4/bench dotnet snobol4/aisnobol"
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
. "$HERE/lib_oracle_flags.sh" 2>/dev/null || { echo "REFUSING: cannot load lib_oracle_flags.sh -- the ONE oracle-flag authority (s200). A private fallback would time a DIFFERENT LANGUAGE (s189: -bf is the only correct arm). Fix the checkout; do not work around this." >&2; exit 3; }
sbl_flags() { echo "$(sbl_lang_flags) -d512m -i64m"; }   # ⭐ LANGUAGE ARM from the ONE authority (lib_oracle_flags.sh); ONLY the sizing is local
# ---------------------------------------------------------------- oracle LIVENESS -- `rc -eq 0` IS NOT A LIVENESS SIGNAL (s191, row `gimpel-suite-harness`)
# ⛔ `sbl` EXITS 0 AFTER A FATAL ERROR, so testing rc alone ADMITS A DEAD ORACLE and the board then grades SCRIP against SPITBOL's
# error dump instead of against program output.  SMALLEST REPRO, and its one-ingredient passing sibling: `INPUT(.INPUT,5,,"nosuch.in")`
# between two OUTPUT statements prints the first, dies with the report, NEVER REACHES THE SECOND -- and exits 0; move the filename to
# the third argument and the identical program runs clean.  The cause is a DIALECT split the manual names outright (v3.7 p.12, p.224):
# Catspaw SPITBOL takes the filename as INPUT's THIRD argument, SNOBOL4+ puts it FOURTH, so a SNOBOL4+ program hands Catspaw an empty
# file spec -- ERROR 116.  MEASURED s191 in corpus/gimpel: 8 library modules and 20 drivers exit 0 while printing the fatal
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
# ⛔ ONE AUTHORITY FOR THE ORACLE INVOCATION (s191, row `ref-the-ungraded-suites`).  Minting a `.ref` means recording what the
# oracle answers, and a .ref minted under a DIFFERENT cwd, lib path, stdin or flag set than the board grades with is a pin that
# can never match -- seat5's s191 conviction, twice in one session, was exactly this: "a census is a harness; copy run_one,
# never re-derive it".  So run_one and every probe verb now call ONE function, and the cwd/SETL4PATH/-bf/timeout facts are
# spelled once.  ⛔ NOTE the cwd rule it carries: the oracle runs in the PROGRAM's dir (so a relative -include resolves), except
# for a CORPUS-rooted lib where it runs at the corpus root.
sc_oracle_run() {  # $1 = program  $2 = RESOLVED lib path  $3 = stdin file  $4 = output file -> echoes the oracle rc
  local prog="$1" lib="$2" in="$3" out="$4" d ocwd rc
  d="$(dirname "$prog")"; ocwd="$d"; [ "$lib" = "$CORPUS" ] && ocwd="$CORPUS"
  (cd "$ocwd" && SETL4PATH=".:$lib" timeout 60 "$SBL" $(sbl_flags) "$prog" < "$in" > "$out" 2>/dev/null); rc=$?
  echo "$rc"
}
# The WEIGHTS table has a RAGGED tail (find-args are variable-length, with lib/rto/norm as the last three), so parsing a row is a
# fact worth spelling once.  ⛔ cmd_run's own loop is a THIRD spelling and is deliberately NOT converted here: it iterates every
# row and rewriting it would put the headline instrument at risk for a hygiene win inside someone else's row.  Flagged, not hidden.
sc_suite_fields() {  # $1 = suite name -> "root<TAB>fargs<TAB>lib<TAB>rto<TAB>norm"; rc 1 if no such suite
  local want="$1" name w root rest nf norm rto lib fargs
  while read -r name w root rest; do
    [ "$name" = "$want" ] || continue
    set -- $rest; nf=$#; norm=${!nf}; rto=${@:$((nf-1)):1}; lib=${@:$((nf-2)):1}; fargs="${*:1:$((nf-3))}"
    printf '%s\t%s\t%s\t%s\t%s\n' "$root" "$fargs" "$lib" "$rto" "$norm"; return 0
  done <<< "$SUITES"
  return 1
}
# ---------------------------------------------------------------- PROVENANCE + CONTENTION (row `scorecard-provenance`, s190)
# ⛔ THIS BOARD IS TIMING-GRADED AND COULD BE SILENTLY CORRUPTED BY A CO-TENANT WITH NOTHING IN ITS OUTPUT SAYING SO.  grade()
# turns rc 124 into TIMEOUT and nine of twelve suites run on a 20s budget, so a program near budget flips TIMEOUT<->PASS under
# CPU load WITH NO TREE CHANGE, and every META and every fail-set-by-name in GOAL-SNOBOL4-100.md rests on that number.  MEASURED,
# NOT HYPOTHETICAL (seat5, s189): two seats ran a full 12-suite board at --jobs 12 on the same 16-core box SIMULTANEOUSLY and
# neither knew until an unrelated ps; csnobol4-suite/nqueens.sno gave TIMEOUT/TIMEOUT, SIG11/TIMEOUT (x3) and SIG11/SIG11 across
# five runs of the SAME script on the SAME binary.  This is the HQ-27 class -- a stale build is indistinguishable from a current
# one by inspecting its output -- and it takes the same cure: MAKE THE PROVENANCE TRAVEL WITH THE NUMBER.
# ⛔ THE REGISTRY IS BOX-SCOPED, NOT TREE-SCOPED, AND THAT IS THE WHOLE POINT.  The colliding seats hold SEPARATE CHECKOUTS
# (/home/claude1/SCRIP, /home/claude2/SCRIP, ...) and therefore SEPARATE test-results/ dirs, so a lock file under $SC/test-results/
# is never seen by the seat it exists to announce -- VACUOUS for the only failure it was asked to catch.  MEASURED on this box:
# all eight seats run as ONE uid, share /tmp, /home and /proc, and see each other's processes -- so a /tmp registry is both shared
# and liveness-checkable (`[ -d /proc/$pid ]` is a real test here, not a guess).  S4E_BOARDS overrides the location.
# ⛔ TWO DETECTORS, DELIBERATELY, BECAUSE THEY CATCH DIFFERENT THINGS.  The REGISTRY names a cooperating board exactly (which pid,
# which tree, which out dir, since when) and can REFUSE to start; the LOAD SAMPLER catches EVERY contender including ones that
# will never register -- another runner, a make -j, a browser -- and is the only half that works against a non-participant.
# ⛔ CAPPING --jobs BY nproc DOES NOT HELP AND IS NOT DONE HERE: two seats each at a perfectly legal --jobs 12 still collide.
# The cure is not a smaller number, it is a number that says what it was measured under.
BOARDS="${S4E_BOARDS:-/tmp/s4e-boards.d}"
sc_load() {  # -> "<1-min average> <instantaneous runnable>"; field 4's numerator is far more responsive than the average on a 20s budget
  local l; l="$(cat /proc/loadavg 2>/dev/null)"; [ -n "$l" ] || l="0 0 0 0/0 0"; set -- $l; echo "$1 ${4%%/*}"
}
sc_peers() {  # -> one line per OTHER LIVE registered board: pid|root|out|jobs|start_iso   (stale entries are reaped, not reported)
  local f pid; [ -d "$BOARDS" ] || return 0
  for f in $(ls "$BOARDS" 2>/dev/null); do case "$f" in *.board) ;; *) continue;; esac
    pid="${f%.board}"; [ "$pid" = "$$" ] && continue
    [ -d "/proc/$pid" ] || { rm -f "$BOARDS/$f" 2>/dev/null; continue; }
    cat "$BOARDS/$f" 2>/dev/null; done
}
sc_board_register() { mkdir -p "$BOARDS" 2>/dev/null; printf '%s|%s|%s|%s|%s\n' "$$" "$S4E" "$1" "$2" "$(date +%Y-%m-%dT%H:%M:%S)" > "$BOARDS/$$.board"; }
sc_board_release() { rm -f "$BOARDS/$$.board" 2>/dev/null; }
sc_prov() { printf '%s\t%s\n' "$2" "$3" >> "$1"; }   # $1=file $2=key $3=value -- report reads these back and COMPUTES the verdict; nothing about contention is ever typed
sc_sampler() {  # $1 = out dir  $2 = owner pid -- appends `epoch load1 runnable npeers peerpids` every S4E_SAMPLE seconds; DIES WITH ITS OWNER (a container restart kills the board, and an orphaned sampler would loop forever writing into a dead board`s dir)
  local o="$1" own="$2" L n np pp
  while [ -d "/proc/$own" ]; do L="$(sc_load)"; n="${L##* }"; L="${L%% *}"
    pp="$(sc_peers | cut -d'|' -f1 | tr '\n' ',')"; pp="${pp%,}"; np=0; [ -n "$pp" ] && np="$(printf '%s\n' "$pp" | tr ',' '\n' | grep -c .)"
    printf '%s\t%s\t%s\t%s\t%s\n' "$(date +%s)" "$L" "$n" "$np" "$pp" >> "$o/load.tsv"; sleep "${S4E_SAMPLE:-5}"; done
}
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
  rc="$(sc_oracle_run "$prog" "$lib" "$in" "$W/live")"
  local ordead=""
  if [ $rc -eq 0 ]; then if sbl_died "$W/live"; then ordead=" fatal-report"; else have_live=1; fi; fi
  if [ $have_pin -eq 0 ] && [ $have_live -eq 0 ]; then
    echo -e "$suite\t${prog#$CORPUS/}\tORACLE_FAIL\tORACLE_FAIL\t0\t0\tsbl rc=$rc$ordead"; rm -rf "$W"; return; fi
  grade() {  # $1 = output file, $2 = rc  -> status
    local o="$1" r="$2"
    [ $r -eq 124 ] && { echo TIMEOUT; return; }
    [ $r -ge 128 ] && { echo "SIG$((r-128))"; return; }
    if [ "$norm" = ms ]; then sed -i '/^iters: [0-9][0-9]*$/d; /^ns: [0-9][0-9]*$/d; /^ms: [0-9][0-9]*$/d' "$o"; [ $have_pin = 1 ] && sed -i '/^iters: [0-9][0-9]*$/d; /^ns: [0-9][0-9]*$/d; /^ms: [0-9][0-9]*$/d' "$W/pin"; [ $have_live = 1 ] && sed -i '/^iters: [0-9][0-9]*$/d; /^ns: [0-9][0-9]*$/d; /^ms: [0-9][0-9]*$/d' "$W/live"; fi  # BM-ONE (s153): measurement lines are DELETED both sides (refs hold only the check: line -- the live oracle); rewrite-to-N was for the retired stamped family
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
export -f run_one stdin_for sc_libpath sbl_flags sbl_died sc_oracle_run; export CORPUS SBL SCRIP SC DEMO
# ---------------------------------------------------------------- run
cmd_run() {
  set -f
  local only="" jobs=1 out="" force=""
  while [ $# -gt 0 ]; do case "$1" in --suites) only="$2"; shift 2;; --jobs) jobs="$2"; shift 2;; --out) out="$2"; shift 2;; --force) force=1; shift;; *) shift;; esac; done
  # ⛔ AN UNKNOWN --suites NAME IS REFUSED, NOT IGNORED (s189).  The filter below is `grep -q ",$name,"` over the TABLE, so a name that is not in the table simply matches
  # nothing: `--suites lon` and `--suites crosschek` both used to run ZERO programs, truncate results.tsv, and then report a META over an empty or partial denominator that
  # LOOKS like a whole board -- the exact failure the s182 warning at the top of this file describes, reached by a typo instead of by two runs sharing one --out.
  local nm bad=""
  for nm in $(echo "$only" | tr ',' ' '); do
    echo "$SUITES" | awk 'NF{print $1}' | grep -qx -- "$nm" && continue
    case "$nm" in
      lon) echo "⛔ REFUSED --suites lon: Lon ruled corpus/lon is not to be run, and the suite was DELETED from this scorecard at s189. There is no flag that runs it." >&2;;
      *)   echo "⛔ REFUSED --suites $nm: no such suite. Known: $(echo "$SUITES" | awk 'NF{print $1}' | tr '\n' ' ')" >&2;;
    esac; bad=1
  done
  [ -n "$bad" ] && exit 2
  [ -z "$out" ] && out="$SC/test-results/scorecard-$(date +%Y%m%d-%H%M%S)"
  mkdir -p "$out"; : > "$out/results.tsv"; echo "$out"
  [ -x "$SCRIP" ] || { echo "SKIP scrip not built"; exit 1; }
  # ---- provenance + contention (row `scorecard-provenance`, s190).  REGISTER FIRST, THEN LOOK: the other order is a TOCTOU in
  # which two boards starting together each see an empty registry and both run -- the exact silent collision this cures.  Two
  # boards racing here therefore both refuse, which is FAIL-CLOSED BY CHOICE: a loud double refusal is recoverable in one command,
  # a quiet double run corrupts the headline instrument and says nothing.
  local prov="$out/provenance.tsv" samp="" peers="" npeers=0 st l0
  sc_board_register "$out" "$jobs"; peers="$(sc_peers)"; npeers="$(printf '%s' "$peers" | grep -c .)"
  if [ "$npeers" -gt 0 ] && [ -z "$force" ]; then
    echo "⛔ REFUSED: $npeers other SNOBOL4 board(s) already running on this box -- this board is TIMING-GRADED and would be measuring under contention:" >&2
    printf '%s\n' "$peers" | while IFS='|' read -r p r o j t; do echo "   holder pid=$p  root=$r  jobs=$j  since=$t  out=$o" >&2; done
    echo "   Wait for it, or re-run with --force (the report is then marked CONTENDED-BY-CONSTRUCTION and the number is born marked)." >&2
    sc_board_release; exit 3
  fi
  st="$(date +%s)"; l0="$(sc_load)"; : > "$prov"; : > "$out/load.tsv"
  sc_prov "$prov" host "$(hostname 2>/dev/null)"; sc_prov "$prov" pid "$$"; sc_prov "$prov" root "$S4E"; sc_prov "$prov" out "$out"
  sc_prov "$prov" suites "${only:-ALL}"; sc_prov "$prov" jobs "$jobs"; sc_prov "$prov" nproc "$(nproc 2>/dev/null || echo 0)"
  sc_prov "$prov" scrip_head "$(cd "$SC" && git rev-parse --short HEAD 2>/dev/null)"; sc_prov "$prov" corpus_head "$(cd "$CORPUS" && git rev-parse --short HEAD 2>/dev/null)"
  sc_prov "$prov" scrip_bin_md5 "$(md5sum "$SCRIP" 2>/dev/null | cut -c1-12)"; sc_prov "$prov" scrip_bin_mtime "$(date -r "$SCRIP" +%Y-%m-%dT%H:%M:%S 2>/dev/null)"
  # ⛔ HQ-27 IN ITS OWN RIGHT, AND THE BRIEF NAMED IT AS THIS ROW\'S PARENT CLASS: a stale build is indistinguishable from a current
  # one by inspecting its output.  The git HEAD in the report header is the TREE, not the BINARY -- a seat that pulls and does not
  # rebuild gets a header reading the new HEAD over numbers measured by the old compiler.  MEASURED, on this very session: the
  # header printed `scrip 21819132` beside a binary md5 and mtime that had not moved since an earlier HEAD.  So record what the
  # binary could have been built FROM (newest commit touching the compiler) and whether that source is dirty, and let `report`
  # compare -- the same cure as the load half, applied to the other axis of provenance.
  sc_prov "$prov" src_commit_epoch "$(cd "$SC" && git log -1 --format=%ct -- src Makefile 2>/dev/null)"
  sc_prov "$prov" src_commit_iso "$(cd "$SC" && git log -1 --format=%cd --date=format:%Y-%m-%dT%H:%M:%S -- src Makefile 2>/dev/null)"
  sc_prov "$prov" scrip_bin_epoch "$(date -r "$SCRIP" +%s 2>/dev/null)"
  sc_prov "$prov" src_dirty "$(cd "$SC" && git status --porcelain -- src Makefile 2>/dev/null | grep -c .)"
  sc_prov "$prov" rt_so_md5 "$(md5sum "$SC/out/libscrip_rt.so" 2>/dev/null | cut -c1-12)"; sc_prov "$prov" rt_so_mtime "$(date -r "$SC/out/libscrip_rt.so" +%Y-%m-%dT%H:%M:%S 2>/dev/null)"
  sc_prov "$prov" start_epoch "$st"; sc_prov "$prov" start_iso "$(date +%Y-%m-%dT%H:%M:%S)"; sc_prov "$prov" load_start "${l0%% *}"
  sc_prov "$prov" forced "${force:-0}"; sc_prov "$prov" peers_at_start "$(printf '%s' "$peers" | tr '\n' ';')"
  sc_sampler "$out" "$$" & samp=$!
  trap 'kill '"$samp"' 2>/dev/null; sc_board_release' EXIT INT TERM
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
  kill "$samp" 2>/dev/null; trap - EXIT INT TERM; sc_board_release
  local en agg; en="$(date +%s)"
  sc_prov "$prov" end_epoch "$en"; sc_prov "$prov" end_iso "$(date +%Y-%m-%dT%H:%M:%S)"; sc_prov "$prov" duration_s "$((en-st))"
  sc_prov "$prov" load_end "$(sc_load | cut -d' ' -f1)"
  agg="$(${AWK:-awk} -F'\t' 'BEGIN{mx=0;sum=0;n=0;rmx=0;pmx=0} {if($2+0>mx)mx=$2+0; sum+=$2; n++; if($3+0>rmx)rmx=$3+0; if($4+0>pmx)pmx=$4+0; if($5!=""){k=split($5,pp,","); for(i=1;i<=k;i++) if(pp[i]!="") SEEN[pp[i]]=1}}
      END{ l=""; for(x in SEEN) l=l (l==""?"":",") x; printf "%.2f\t%.2f\t%d\t%d\t%d\t%s", mx, (n?sum/n:0), rmx, pmx, n, (l==""?"-":l) }' "$out/load.tsv" 2>/dev/null)"
  set -- $agg
  sc_prov "$prov" load_peak "${1:-0}"; sc_prov "$prov" load_mean "${2:-0}"; sc_prov "$prov" runnable_peak "${3:-0}"
  sc_prov "$prov" peers_max "${4:-0}"; sc_prov "$prov" samples "${5:-0}"; sc_prov "$prov" peers_seen "${6:--}"
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
  # ---- PROVENANCE HEADER (row `scorecard-provenance`, s190).  ⛔ THE VERDICT IS COMPUTED FROM THE RECORDED FACTS, NEVER TYPED,
  # and its ABSENCE is reported as loudly as its presence: a board measured before s190 (or by any runner that does not record
  # provenance) cannot be told apart from a clean one by looking at it, which is the whole defect.  The threshold is not a guess
  # about "busy": the board's OWN expected load is ~jobs (sbl, scrip and gcc are single-threaded, one at a time per slot), so
  # everything above jobs is FOREIGN by construction and `excess` is the honest quantity to PRINT.  ⛔ BUT EXCESS IS NOT THE
  # ALARM, OVERSUBSCRIPTION IS, AND THE DIFFERENCE IS CALIBRATION, NOT PEDANTRY.  MEASURED on this box: the desktop baseline
  # alone (chrome and friends) holds ~4-5 runnable at idle, so "peak exceeds my jobs" fires on EVERY board ever run here and a
  # banner that always fires is a banner every seat learns to skip -- strictly worse than no banner.  What actually invalidates
  # a timing is the jobs of this board NOT EACH HOLDING A CORE, i.e. peak > nproc; seat5\'s measured collision (two boards at
  # --jobs 12 on 16 cores, peak ~24) trips that, a 6-job board beside an idle desktop does not.  Foreign load below that line is
  # reported as NOISY with its number so a later board can be compared against it, and never as "not comparable".
  # ⛔ THE ALARM RIDES THE SMOOTHED 1-MINUTE AVERAGE, NOT THE INSTANTANEOUS RUNNABLE COUNT, AND THE REASON WAS MEASURED HERE, NOT
  # REASONED: a 6-job board on an IDLE box sampled `runnable PEAK 13` against a 1-min average of 3.69, because each run_one slot
  # briefly holds two processes (timeout plus its child) and gcc bursts on top of that -- so a board can transiently out-run its
  # own job count all by itself.  Alarming on that would fire OVERSUBSCRIBED on every honest --jobs 12 board, which is the
  # cry-wolf failure again.  The runnable peak is kept and PRINTED because it is the responsive half and shows the burst shape;
  # the VERDICT uses the peak of the 1-minute average, which no single sample can spike.  Both of the failures this row exists to
  # catch still trip it: two --jobs 12 boards for an hour drive the AVERAGE past 16 cores, and a foreign burst inside a long board
  # is caught because load_peak is the max ACROSS samples, not the value at the end.
  if [ -f "$out/provenance.tsv" ]; then
    ${AWK:-awk} -F'\t' '{V[$1]=$2} END{
      j=V["jobs"]+0; np=V["nproc"]+0; pk=V["load_peak"]+0; rk=V["runnable_peak"]+0; pm=V["peers_max"]+0; f=V["forced"]+0; d=V["duration_s"]+0;
      hi=pk; ex=hi-j; sp=ENVIRON["S4E_SAMPLE"]; if(sp=="") sp="5";
      if(V["end_epoch"]=="") tail="⛔ RUN IN PROGRESS or KILLED -- no end recorded"; else tail=sprintf("end=%s  wall=%dm%02ds", V["end_iso"], d/60, d%60);
      pl=""; if(pm>0 && V["peers_seen"]!="-" && V["peers_seen"]!="") pl=sprintf("(pids %s)", V["peers_seen"]);
      printf "PROVENANCE  root=%s pid=%s  jobs=%d/%d cores  suites=%s  start=%s  %s\n", V["root"], V["pid"], j, np, V["suites"], V["start_iso"], tail;
      printf "            scrip %s (bin md5 %s, built %s) · corpus %s · rt.so md5 %s\n", V["scrip_head"], V["scrip_bin_md5"], V["scrip_bin_mtime"], V["corpus_head"], V["rt_so_md5"];
      printf "            load 1-min: start %s · mean %s · PEAK %s   runnable PEAK %d   samples %s@%ss   concurrent boards seen: %d %s\n", V["load_start"], V["load_mean"], V["load_peak"], rk, V["samples"], sp, pm, pl;
      be=V["scrip_bin_epoch"]+0; se=V["src_commit_epoch"]+0; dy=V["src_dirty"]+0;
      if(be>0 && se>0 && be<se) printf "⛔ STALE BINARY — scrip was built %s but the compiler source moved at %s, AFTER it. This board measured a tree that is no longer checked out. `make pristine` and re-measure before quoting any number from it (HQ-27).\n", V["scrip_bin_mtime"], V["src_commit_iso"];
      else if(dy>0) printf "⛔ UNCOMMITTED COMPILER SOURCE — %d modified file(s) under src/ or Makefile at measure time, so the binary md5 above may correspond to no commit at all and this board is not reproducible from its own HEAD.\n", dy;
      why="";
      if(f) why=why" started under --force while another board held the registry;";
      if(pm>0) why=why sprintf(" %d foreign board(s) registered DURING this run %s;", pm, pl);
      if(V["peers_at_start"]!="" && V["peers_at_start"]!="-") why=why" a board was already running when this one started;";
      if(np>0 && hi>np) why=why sprintf(" box OVERSUBSCRIBED -- peak %.2f runnable on %d cores, so the %d jobs of this board did NOT each hold a core;", hi, np, j);
      if(why!="") printf "⛔ CONTENDED — the timings of this board are NOT comparable to a quiet board:%s Nine of twelve suites grade on a 20s budget and rc 124 becomes TIMEOUT, so TIMEOUT and SIG rows here may be load artefacts rather than tree state.\n", why;
      else if(ex>2) printf "· NOISY (not contended) — about %.1f of sustained load beyond the %d jobs of this board, but peak %.2f still fits in %d cores, so every job held one. Timings are comparable; the estimate is printed so a later board can be compared against it.\n", ex, j, hi, np;
      else printf "⭐ CLEAN — sustained load never rose above the %d jobs of this board and no other board was seen; these timings are comparable to a quiet board.\n", j;
    }' "$out/provenance.tsv"
  else
    echo "⛔ NO PROVENANCE FOR THIS BOARD (measured before s190, or by a runner that does not record it): jobs, wall time, load and concurrent-board state are UNKNOWN, so a TIMEOUT row here CANNOT be distinguished from a load artefact. Re-measure before quoting these numbers."
  fi
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
# ---------------------------------------------------------------- one program, N times (row `scorecard-provenance`, s190)
# ⛔ THIS EXISTS BECAUSE THE CONTENTION EXPERIMENT NEEDS ONE PROGRAM, NOT NINETY.  seat5 convicted this instrument by running
# csnobol4-suite/nqueens.sno FIVE TIMES on the SAME binary and getting TIMEOUT/TIMEOUT, SIG11/TIMEOUT (x3) and SIG11/SIG11 --
# and until s190 the only way to re-run one program THE WAY THE BOARD RUNS IT was to run its whole suite, which perturbs the
# very load being measured.  It calls run_one -- the same authority, the same lib path, the same oracle flags, the same grade()
# -- and adds nothing but the repeat count and the load stamp, so a row measured here is comparable to a row on the board.
# It registers NO lock and refuses nothing: a single program is a probe, not a board, and must stay usable while one is running.
cmd_one() {  # <suite> <program-basename-or-path> [N]
  set -f
  local want="${1:-}" prog="${2:-}" reps="${3:-1}" i L
  [ -n "$want" ] && [ -n "$prog" ] || { echo "usage: $0 one <suite> <program> [N]   suites: $(echo "$SUITES" | ${AWK:-awk} 'NF{printf "%s ", $1}')" >&2; exit 2; }
  echo "$SUITES" | ${AWK:-awk} 'NF{print $1}' | grep -qx -- "$want" || { echo "⛔ REFUSED: no such suite `$want`. Known: $(echo "$SUITES" | ${AWK:-awk} 'NF{printf "%s ", $1}')" >&2; exit 2; }
  local name="$want" root fargs lib rto norm
  IFS=$'\t' read -r root fargs lib rto norm < <(sc_suite_fields "$want")
  {
    local full="$prog"
    if [ ! -f "$full" ]; then case "$root" in
      SELF)      full="$DEMO/beauty/beauty.sno";;
      SCRIPTEST) full="$(find "$SC/test" $fargs 2>/dev/null | grep -m1 "/$prog\(\.sno\)\?$")";;
      MISC)      local m; for m in $MISC_DIRS; do full="$(find "$CORPUS/$m" $fargs 2>/dev/null | grep -m1 "/$prog\(\.sno\)\?$")"; [ -n "$full" ] && break; done;;
      *)         full="$(find "$CORPUS/$root" $fargs 2>/dev/null | grep -m1 "/$prog\(\.sno\)\?$")";;
    esac; fi
    [ -n "$full" ] && [ -f "$full" ] || { echo "⛔ not found in suite $want: $prog" >&2; exit 2; }
    echo "# suite=$name lib=$lib rto=$rto norm=$norm prog=${full#$CORPUS/}  reps=$reps  (rep, load1, runnable, then the board TSV line)"
    for i in $(seq 1 "$reps"); do L="$(sc_load)"; printf '%s\t%s\t%s\t' "$i" "${L%% *}" "${L##* }"; run_one "$name" "$lib" "$full" "$norm" "$rto"; done
  }
}
# ---------------------------------------------------------------- the oracle's own answer (row `ref-the-ungraded-suites`, s191)
# ⛔ THIS EXISTS SO A .ref CAN BE MINTED THROUGH THE SAME DOOR THE BOARD GRADES THROUGH.  run_one runs the oracle and then THROWS
# THE OUTPUT AWAY (it only ever compares), so any tool that wants to RECORD the oracle answer had to re-derive the invocation --
# and a .ref minted under a different cwd, lib path, stdin or flag set than the board grades with is a pin that can never match.
# ⛔ AND rc IS NOT THE VERDICT: seat2 proved at s191 that sbl EXITS 0 WHILE PRINTING A FATAL ERROR DUMP, so `DEAD_REPORT` is a
# distinct status from `LIVE` and a caller that mints on rc alone pins an error report as ground truth.  EMPTY is also its own
# status and is NOT mintable: a 0-byte .ref grades "produced nothing" as correct, which is how 134 vacuous passes were nearly
# manufactured at s191.  The three non-LIVE statuses are exactly the brief's classes (ii) and (iii).
cmd_oracle() {  # <suite> <program> [outfile] -> "STATUS<TAB>rc<TAB>bytes<TAB>outfile"
  set -f
  local want="${1:-}" prog="${2:-}" out="${3:-}" root fargs lib rto norm full in rc st bytes
  [ -n "$want" ] && [ -n "$prog" ] || { echo "usage: $0 oracle <suite> <program> [outfile]" >&2; exit 2; }
  IFS=$'\t' read -r root fargs lib rto norm < <(sc_suite_fields "$want") || { echo "⛔ REFUSED: no such suite $want" >&2; exit 2; }
  [ -n "${root:-}" ] || { echo "⛔ REFUSED: no such suite $want" >&2; exit 2; }
  full="$prog"; [ -f "$full" ] || { echo "⛔ not found: $prog" >&2; exit 2; }
  [ -n "$out" ] || out="$(mktemp)"
  in="$(stdin_for "$full")"; [ "$want" = beauty_self ] && in="$full"
  lib="$(sc_libpath "$lib" "$(dirname "$full")")"
  rc="$(sc_oracle_run "$full" "$lib" "$in" "$out")"
  bytes="$(wc -c < "$out" 2>/dev/null || echo 0)"
  if [ "$rc" -eq 124 ]; then st=TIMEOUT
  elif [ "$rc" -ge 128 ]; then st="SIG$((rc-128))"
  elif [ "$rc" -ne 0 ]; then st="RC$rc"
  elif sbl_died "$out"; then st=DEAD_REPORT
  elif [ "$bytes" -eq 0 ]; then st=EMPTY
  else st=LIVE; fi
  printf '%s\t%s\t%s\t%s\n' "$st" "$rc" "$bytes" "$out"
}
case "${1:-report}" in run) shift; cmd_run "$@";; report) shift; cmd_report "$@";; one) shift; cmd_one "$@";; oracle) shift; cmd_oracle "$@";; *) echo "usage: $0 run|report|one|oracle"; exit 2;; esac
