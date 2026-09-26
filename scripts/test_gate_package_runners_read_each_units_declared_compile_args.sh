#!/usr/bin/env bash
export S4E_ONE_RUNNER_FIXTURE="gate arm ${0##*/}: a runner invoked as an instrument fixture, not a board (CEO-547)"
# test_gate_package_runners_read_each_units_declared_compile_args.sh -- A TEST UNIT'S COMPILE SWITCHES ARE ITS OWN DECLARATION, AND A
# RUNNER TYPES NONE OF ITS OWN. RULES.md hard-cap rule clause 8 (f); ceo CEO-1281; Lon 2026-09-26 11:1x-11:2x CDT, in-chat to the ceo,
# verbatim: "Basically the command-line arguments for compile time and run time (if needed) should be stored per-test unit."
#
# ⛔⭐ WHY EVERY ARM READS A VERDICT PAIR. Landing 3 of the compile_args row deleted the typing: corpus_suite_harness.py no longer puts
# --stlimit on every snobol4, snocone and rebus compile, and ten package runners no longer export SCRIP_SNO_STMTKW=1 over their whole
# loop; each reads the unit's compile_args instead. On the shipped tables every row declares --stlimit, so a runner that still types
# the switch for everybody and a runner that reads the declaration grade every real board identically. So each arm plants TWO copies
# of one witness whose output depends on the switch -- decl, which declares --stlimit, and nodecl, which declares nothing -- and
# requires the verdicts to DIFFER: decl PASS in both modes, nodecl FAIL in both. A runner that types the switch reads nodecl PASS; a
# runner that reads no declaration reads decl FAIL. The SNOBOL4 witness prints &STCOUNT (SPITBOL always counts, 3; SCRIP 3 with the
# switch and 0 without); the Icon witness sets &trace (the Icon oracle prints three trace lines; SCRIP the same three with the switch,
# none without). The premise arm re-measures both before any arm reads them, so the day the switch stops mattering this gate says so.
#
#   W   PREMISE: both witnesses still depend on the switch, in both modes, and the oracles agree with the switched run
#   L1  declared_memory_table: a declaring row carries its compile_args as a 4th column; a row declaring none prints its three
#       columns byte-identical to a table with no compile_args column; declared_compile_args_from_table returns exactly those words
#   L2  refusals, rc 2 each: a compile_args word outside COMPILE_ARGS_ADMITTED; a run_args cell (no bash reader honours one yet);
#       a lookup with no table
#   H   corpus_suite_harness.py run on a scratch one-line SNOBOL4 family -- the masters' path
#   P1..P10  the ten package runners, each on its own scratch suite and scratch progress table: dotnet, snoflake, spitbol_x64,
#       spitbol_testpgms, csnobol4, aisnobol (through the harness), jcon, arizona, ipl, and spitbol_x32 (hq_snobol4's X32T, 1cddec296)
#
# NOT COVERED, AND NAMED: test_corpus_snobol4.sh and scorecard_snobol4.sh (and test_snobol4_gimpel_suite.sh, which grades through the
# scorecard) keep their export until landing 4 gives their standalone programs STEM.args sidecars; the ladder, port-trace and gate
# graders keep theirs, because a grader's instrumentation is not a unit attribute.
# FAIL-ONCE, MEASURED 2026-09-26 17:4x CDT (coo) on SCRIP 0dd682acd plus this landing, both directions: (1) FAIL_ONCE=1 exports
# SCRIP_SNO_STMTKW=1 around every harness and runner invocation -- the typed switch this row retires -- and nodecl reads PASS/PASS
# everywhere: H and P1..P10 red, 11 of 14; W, L1 and L2 read no runner and stay green by construction. (2) The same gate over origin's
# twelve pre-landing files (the harness, lib_declared_arena.sh and ten runners, restored for the run and put back byte-identical): 13 of
# 14 red -- L1 and L2 (the old table has no 4th column, no refusal and no lookup) and H, P1..P10 (nodecl PASS/PASS); only W is green.
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
. "$HERE/lib_gate.sh"
gate_parse_args "$@"
G=package_runners_read_each_units_declared_compile_args
unproven() { echo "GATE UNPROVEN(2) [$G]: $*"; exit 2; }
"$HERE/util_require_fresh.sh" --gate "test_gate_$G" "$ROOT/scrip" "$ROOT/out/libscrip_rt.so" >/dev/null 2>&1 || unproven "this tree's binary is stale or unbuilt -- run make"
. "$HERE/lib_oracle_flags.sh" 2>/dev/null || unproven "lib_oracle_flags.sh unloadable"
. "$HERE/lib_declared_arena.sh" 2>/dev/null || unproven "lib_declared_arena.sh unloadable"
SBL="$(sbl_correctness_bin)"; ICON="$(icon_bin)"
[ -x "$SBL" ] || unproven "the sbl -bf oracle is absent"
[ -x "$ICON" ] || unproven "the Icon oracle is absent"
SCRIP="$ROOT/scrip"; RT="$ROOT/out"
W="$(mktemp -d "${TMPDIR:-/tmp}/gate_cargs.XXXXXX")" || unproven "mktemp failed"
trap 'rm -rf "$W"' EXIT
unset SCRIP_SNO_STMTKW   # the caller's environment never reaches an arm; FAIL_ONCE=1 alone puts it back, around the runners
PASS=0; FAIL=0; UNM=""
ok()  { PASS=$((PASS+1)); echo "  ✅ $1: $2"; }
red() { FAIL=$((FAIL+1)); echo "  ⛔ $1 RED: $2"; }
typed=(); [ "${FAIL_ONCE:-0}" = 1 ] && typed=(SCRIP_SNO_STMTKW=1) && echo "FAIL_ONCE=1: SCRIP_SNO_STMTKW=1 is exported around every harness and runner invocation"

# ── W: the witnesses, and the premise that they depend on the switch ─────────────────────────────────────────────────────────────
printf "        X = 1\n        Y = 2\n        OUTPUT = 'count=' &STCOUNT\nEND\n" > "$W/cnt.sno"
printf 'procedure p(x);\n  return x + 1;\nend\nprocedure main();\n  &trace := -1;\n  write(p(1));\nend\n' > "$W/tr.icn"
m3_of() { ( cd "$W" && timeout 20 "$SCRIP" --run $2 "$1" < /dev/null 2>&1 ); }
m4_of() { ( cd "$W" && "$SCRIP" --compile $2 "$1" > "$W/w.s" 2>/dev/null < /dev/null && gcc -no-pie "$W/w.s" -L"$RT" -lscrip_rt -lm -Wl,-rpath,"$RT" -o "$W/w.bin" 2>/dev/null && timeout 20 "$W/w.bin" < /dev/null 2>&1 ); }
(cd "$W" && timeout 20 "$SBL" -bf cnt.sno < /dev/null > sbl.out 2>&1)
(cd "$W" && timeout 20 "$ICON" tr.icn < /dev/null > icon.out 2>&1)
s3y="$(m3_of cnt.sno --stlimit)"; s3n="$(m3_of cnt.sno '')"; s4y="$(m4_of cnt.sno --stlimit)"; s4n="$(m4_of cnt.sno '')"
i3y="$(m3_of tr.icn --stlimit)"; i3n="$(m3_of tr.icn '')"; i4y="$(m4_of tr.icn --stlimit)"; i4n="$(m4_of tr.icn '')"
sbl_o="$(cat "$W/sbl.out")"; icon_o="$(cat "$W/icon.out")"
if [ "$s3y" = "$sbl_o" ] && [ "$s4y" = "$sbl_o" ] && [ "$s3n" != "$sbl_o" ] && [ "$s4n" != "$sbl_o" ] \
   && [ "$i3y" = "$icon_o" ] && [ "$i4y" = "$icon_o" ] && [ "$i3n" != "$icon_o" ] && [ "$i4n" != "$icon_o" ]; then
    ok W "both witnesses depend on the switch: SNOBOL4 m3/m4 with it '$s3y'/'$s4y' = sbl '$sbl_o', without '$s3n'/'$s4n'; Icon m3/m4 with it = the oracle's $(printf '%s\n' "$icon_o" | wc -l) lines, without $(printf '%s\n' "$i3n" | wc -l)/$(printf '%s\n' "$i4n" | wc -l)"
else
    unproven "a witness no longer depends on the switch, so no arm below can tell a declaration from a default -- SNOBOL4 sbl='$sbl_o' m3 with='$s3y' without='$s3n' m4 with='$s4y' without='$s4n'; Icon m3 with/without equal to the oracle: $([ "$i3y" = "$icon_o" ] && echo y || echo n)/$([ "$i3n" = "$icon_o" ] && echo y || echo n), m4 $([ "$i4y" = "$icon_o" ] && echo y || echo n)/$([ "$i4n" = "$icon_o" ] && echo y || echo n) -- re-derive the witness"
fi

# ── L1, L2: the reader ───────────────────────────────────────────────────────────────────────────────────────────────────────────
HDR='entry,heap_kb,stack_kb,compile_args,run_args'
printf '%s\ndecl,131072,4096,--stlimit,\nnodecl,131072,4096,,\n' "$HDR" > "$W/l.csv"
printf 'entry,heap_kb,stack_kb\ndecl,131072,4096\nnodecl,131072,4096\n' > "$W/l_old.csv"
declared_memory_table "$W/l.csv" > "$W/l.tsv" 2>"$W/l.err"; l_rc=$?
declared_memory_table "$W/l_old.csv" > "$W/l_old.tsv" 2>/dev/null
want_decl="$(printf 'decl\t131072\t4096\t--stlimit')"; want_nodecl="$(printf 'nodecl\t131072\t4096')"
got_decl="$(grep '^decl	' "$W/l.tsv")"; got_nodecl="$(grep '^nodecl	' "$W/l.tsv")"; old_nodecl="$(grep '^nodecl	' "$W/l_old.tsv")"
ca_d="$(declared_compile_args_from_table "$W/l.tsv" decl)"; ca_n="$(declared_compile_args_from_table "$W/l.tsv" nodecl)"
if [ "$l_rc" = 0 ] && [ "$got_decl" = "$want_decl" ] && [ "$got_nodecl" = "$want_nodecl" ] && [ "$got_nodecl" = "$old_nodecl" ] && [ "$ca_d" = "--stlimit" ] && [ -z "$ca_n" ]; then
    ok L1 "the table carries decl's --stlimit as a 4th column, nodecl's line is byte-identical to a table with no compile_args column, and the lookup returns '--stlimit' and ''"
else
    red L1 "rc=$l_rc decl=[$(printf '%s' "$got_decl" | cat -A)] nodecl=[$(printf '%s' "$got_nodecl" | cat -A)] old=[$(printf '%s' "$old_nodecl" | cat -A)] lookup=[$ca_d]/[$ca_n] $(head -1 "$W/l.err")"
fi
printf '%s\nbad,131072,4096,--nosuchswitch,\n' "$HDR" > "$W/bad.csv"; declared_memory_table "$W/bad.csv" >/dev/null 2>&1; r1=$?
printf '%s\nra,131072,4096,,x\n' "$HDR" > "$W/ra.csv"; declared_memory_table "$W/ra.csv" >/dev/null 2>&1; r2=$?
declared_compile_args_from_table "$W/no_such_table.tsv" decl >/dev/null 2>&1; r3=$?
[ "$r1" = 2 ] && [ "$r2" = 2 ] && [ "$r3" = 2 ] && ok L2 "a word outside the admitted list, a run_args cell and a lookup with no table each refuse rc 2" \
    || red L2 "rc for --nosuchswitch=$r1, run_args=$r2, no table=$r3 (want 2, 2, 2)"

# ── the verdict pair ─────────────────────────────────────────────────────────────────────────────────────────────────────────────
outcome() { awk -F'\t' -v p="$2" -v m="$3" 'NR>1 && $8==p && $9==m {print $10}' "$1" 2>/dev/null | tail -1; }
pair() {  # <arm> <runner rc> <decl m3> <decl m4> <nodecl m3> <nodecl m4>
    local a="$1" rc="$2"
    if [ "$rc" = 2 ] || [ "$rc" = 124 ]; then red "$a" "the runner could not measure its fixture (rc=$rc)"; UNM="$UNM $a"; return 1; fi
    if [ "$3" = PASS ] && [ "$4" = PASS ] && [ -n "$5" ] && [ "$5" != PASS ] && [ -n "$6" ] && [ "$6" != PASS ]; then
        ok "$a" "decl PASS/PASS, nodecl $5/$6 -- the declaration reached the compile in both modes and the neighbour got no switch"
    else
        red "$a" "decl ${3:-none}/${4:-none}, nodecl ${5:-none}/${6:-none} (want PASS/PASS and FAIL/FAIL)"; return 1
    fi
}
show_on_red() { printf '%s\n' "$1" | grep -E 'REFUSE|⛔' | head -3 | sed 's/^/        /'; }
csv_pair() { printf '%s\n%s,131072,4096,--stlimit,\n%s,131072,4096,,\n' "$HDR" "$1" "$2"; }

# ── H: the harness, on a one-line SNOBOL4 family (the masters' path); the same family feeds P6, the aisnobol runner ────────────────
mkdir -p "$W/fam"
printf " X = 1; Y = 2; OUTPUT = 'count=' &STCOUNT;END;* decl\n X = 1; Y = 2; OUTPUT = 'count=' &STCOUNT;END;* nodecl\n" > "$W/fam/ALL.sno"
printf 'count=3\ncount=3\n' > "$W/fam/ALL.ref"; for n in decl nodecl; do cp "$W/cnt.sno" "$W/fam/$n.sno"; done   # the loose programs a package ships beside its suite
printf 'rank,entry,origin,family,kind,xfail,n_lines,heap_kb,stack_kb,compile_args,run_args\n1,decl,fx__decl,fx,line,0,1,131072,4096,--stlimit,\n2,nodecl,fx__nodecl,fx,line,0,1,131072,4096,,\n' > "$W/fam/ALL.csv"
board_pair() {  # <arm> <rc> <output> -- a harness board prints FAIL lines and a SUITE_BOARD or <PKG>_BOARD count line
    local b3 b4 d3=FAIL d4=FAIL n3=PASS n4=PASS
    b3="$(printf '%s\n' "$3" | grep -oE 'm3_(pass|PASS)=[0-9]+' | head -1 | cut -d= -f2)"; b4="$(printf '%s\n' "$3" | grep -oE 'm4_(pass|PASS)=[0-9]+' | head -1 | cut -d= -f2)"
    printf '%s\n' "$3" | grep -qE 'FAIL m3 nodecl:' && n3=FAIL; printf '%s\n' "$3" | grep -qE 'FAIL m4 nodecl:' && n4=FAIL
    # a PASS prints nothing, so decl passed when it has no FAIL line and the board's pass count holds it beside nodecl's
    local k3=1 k4=1; [ "$n3" = PASS ] && k3=2; [ "$n4" = PASS ] && k4=2
    printf '%s\n' "$3" | grep -qE 'FAIL m3 decl:' || { [ "${b3:-x}" = "$k3" ] && d3=PASS; }
    printf '%s\n' "$3" | grep -qE 'FAIL m4 decl:' || { [ "${b4:-x}" = "$k4" ] && d4=PASS; }
    pair "$1" "$2" "$d3" "$d4" "$n3" "$n4" || show_on_red "$3"
}
out="$(cd "$ROOT" && env "${typed[@]}" timeout 300 python3 "$HERE/corpus_suite_harness.py" run "$W/fam/ALL.sno" "$W/fam/ALL.ref" --modes m3,m4 2>&1)"; rc=$?
board_pair H "$rc" "$out"

# ── P1..P10: the package runners ──────────────────────────────────────────────────────────────────────────────────────────────────
run_db() {  # <arm> <db> <env...> -- <runner and args>
    local a="$1" db="$2" o r; shift 2; local -a e=(); while [ "$1" != -- ]; do e+=("$1"); shift; done; shift
    o="$(cd "$ROOT" && env "${typed[@]}" S4E_PROGRESS_DB="$db" "${e[@]}" timeout 300 bash "$@" 2>&1)"; r=$?
    pair "$a" "$r" "$(outcome "$db" decl m3)" "$(outcome "$db" decl m4)" "$(outcome "$db" nodecl m3)" "$(outcome "$db" nodecl m4)" || show_on_red "$o"
}
mk_sno() {  # <dir> <ext>
    mkdir -p "$1"; for n in decl nodecl; do cp "$W/cnt.sno" "$1/$n.$2"; done; csv_pair decl nodecl > "$1/ALL.csv"; }
mk_icn() {  # <dir> <entry prefix>
    mkdir -p "$1"; for n in decl nodecl; do cp "$W/tr.icn" "$1/$n.icn"; ( cd "$1" && timeout 20 "$ICON" "$n.icn" < /dev/null > "$n.ref" 2>&1 ); done; }

mk_sno "$W/dot" sno
run_db P1 "$W/p1.tsv" DOTNET_SUITE="$W/dot" -- "$HERE/test_snobol4_dotnet_suite.sh"
mk_sno "$W/flake" sno
run_db P2 "$W/p2.tsv" SNOFLAKE_SUITE="$W/flake" ARM_CSN=0 -- "$HERE/test_snoflake_suite.sh"
mk_sno "$W/x64" sbl
run_db P3 "$W/p3.tsv" SPITBOL_X64_SUITE="$W/x64" SPITBOL_X64_SHIPPED=2 -- "$HERE/test_snobol4_spitbol_x64_suite.sh"
# P4 spitbol_testpgms records no progress row for a scratch suite, so its RED lines and board counts are read
mkdir -p "$W/tp"; for n in testdecl testnodecl; do cp "$W/cnt.sno" "$W/tp/$n.spt"; done; : > "$W/tp/testpgms.in"; csv_pair testdecl testnodecl > "$W/tp/ALL.csv"
out="$(cd "$ROOT" && env "${typed[@]}" SPITBOL_TESTPGMS_SUITE="$W/tp" timeout 300 bash "$HERE/test_snobol4_spitbol_testpgms_suite.sh" 2>&1)"; rc=$?
b="$(printf '%s\n' "$out" | grep -E '^SPITBOL_TESTPGMS_BOARD ')"
d3=FAIL; d4=FAIL; n3=PASS; n4=PASS
printf '%s\n' "$out" | grep -qE '^  RED  testnodecl m3 ' && n3=FAIL; printf '%s\n' "$out" | grep -qE '^  RED  testnodecl m4 ' && n4=FAIL
k3=1; k4=1; [ "$n3" = PASS ] && k3=2; [ "$n4" = PASS ] && k4=2
printf '%s\n' "$out" | grep -qE '^  RED  testdecl m3 ' || { printf '%s' "$b" | grep -q " m3_pass=$k3 " && d3=PASS; }
printf '%s\n' "$out" | grep -qE '^  RED  testdecl m4 ' || { printf '%s' "$b" | grep -q " m4_pass=$k4 " && d4=PASS; }
pair P4 "$rc" "$d3" "$d4" "$n3" "$n4" || show_on_red "$out"
mk_sno "$W/csn" sno; for n in decl nodecl; do printf '%s\n' "$sbl_o" > "$W/csn/$n.ref"; done
run_db P5 "$W/p5.tsv" CSNOBOL4_SUITE="$W/csn" -- "$HERE/test_snobol4_csnobol4_suite.sh"
out="$(cd "$ROOT" && env "${typed[@]}" AISNOBOL_SUITE="$W/fam" timeout 300 bash "$HERE/test_snobol4_aisnobol_suite.sh" 2>&1)"; rc=$?
board_pair P6 "$rc" "$out"
mk_icn "$W/jcon"; csv_pair decl nodecl > "$W/jcon/ALL.csv"
run_db P7 "$W/p7.tsv" -- "$HERE/test_icon_jcon_suite.sh" --corpus "$W/jcon"
A="$W/az/corpus/packages/icon/arizona_tests"; mk_icn "$A/general"; mkdir -p "$A/special"; csv_pair general/decl general/nodecl > "$A/ALL.csv"
run_db P8 "$W/p8.tsv" S4E_HOME="$W/az" -- "$HERE/test_icon_arizona_suite.sh"
P="$W/ipl/corpus/packages/icon/ipl"; mk_icn "$P/progs"; for d in gprogs procs gprocs incl gincl; do mkdir -p "$P/$d"; done; csv_pair progs/decl progs/nodecl > "$P/ALL.csv"
run_db P9 "$W/p9.tsv" S4E_HOME="$W/ipl" -- "$HERE/test_icon_ipl_suite.sh"
mk_sno "$W/x32" spt
run_db P10 "$W/p10.tsv" SPITBOL_X32_SUITE="$W/x32" SPITBOL_X32_SHIPPED=2 -- "$HERE/test_snobol4_spitbol_x32_suite.sh"

echo "------------------------------------------------------------"
N=$((PASS + FAIL))
if [ "$FAIL" = 0 ]; then echo "GATE PASS(0) [$G]: $PASS of $N arms green (denominator: $N arms -- the premise, two reader arms, the harness and ten package runners, each on a decl/nodecl pair in both modes)"; exit 0; fi
if [ -n "$UNM" ] && [ "$FAIL" = "$(printf '%s' "$UNM" | wc -w)" ]; then echo "GATE UNPROVEN(2) [$G]: $FAIL of $N arms could not measure their fixtures:$UNM"; exit 2; fi
echo "GATE FAIL(1) [$G]: $FAIL of $N arms red${UNM:+ (could not measure:$UNM)}"; exit 1
