#!/usr/bin/env bash
# util_dyn_caps_witness.sh compile|runtime|census -- the criteria of ARCH-DYNAMIC-STORAGE.md (Lon 2026-09-23: no fixed limits).
# compile: the three compile-time witnesses (200 DEFINEs and a 608-entry blob layout vs sbl -bf, 600 predicates vs gprolog) plus zero compiler tables bound
#          by a compile-time population cap in the census; runtime: the 70-argument Icon call vs iconx plus zero runtime tables
#          bound by a runtime population cap; census: the ratchet -- file/static/field declarations may only fall from BASELINE, and the
#          locals a program fills (CEO-1231 stage 2) may only fall from BASELINE_FUNCTION_SCOPE.
# rc 0 GREEN, 1 RED, 2 REFUSE (an oracle or the binary missing). The population regexes are the page's § 6 list, spelled once here.
# The GNU Prolog oracle is gplc's native binary, never `gprolog --consult-file`: the top level prints a four-line banner and consult's two
# "compiling ..." lines to STDOUT before the program runs, so that arm read 606 lines against a correct 600 and could never go green (cfo 2026-09-23).
set -u
HERE=$(cd "$(dirname "$0")" && pwd); cd "$HERE/.." || exit 2
mode=${1:-}; [ -n "$mode" ] || { echo "REFUSE(2): usage: util_dyn_caps_witness.sh compile|runtime|census"; exit 2; }
T=$(mktemp -d); trap 'rm -rf "$T"' EXIT
python3 scripts/audit_fixed_caps_census.py --tsv "$T/c.tsv" --no-report > "$T/census.txt" 2>&1 || { echo "REFUSE(2): the census refused -- $(tail -2 "$T/census.txt" | tr '\n' ' ')"; exit 2; }
red=0
count_caps() { awk -F'\t' -v dirs="$1" -v caps="$2" 'NR>1 && ($3=="file"||$3=="static"||$3=="field"||$3=="arena") && $1 ~ dirs && $5 ~ caps {print "  " $1 ":" $2 " " $4 "[" $5 "]"}' "$T/c.tsv"; }
COMPILE_CAPS='^(SNO_DEF_MAX|SNO_PAT_MAX|SNO_EXPR_MAX|SNO_FZW_MAX|SNO_LOOP_STACK_MAX|MAX_PREDS|PL_BB_TABLE_MAX|PL_INIT_GOALS_MAX|PL_CONSULT_FILES_MAX|FL_MAX_ENTRIES|FL_MAX_FIELDS|FL_MAX_SCOPES|FL_MAX_GRAPHS|ZDP_CAP|BB_PATCH_MAX|SMX_STRTAB_CAP|SMX_CSETTAB_CAP|AB_FNCELL_MAX|XA_BB_EMIT_PAIR_MAX|ZD_NOPS_MAX|FLAT_CHAIN_SET_MAX|FLAT_DATA_LBL_MAX|WASM_STRTAB_MAX|WASM_USERFNS_MAX|STAGE2_LABEL_MAX|STAGE2_PROC_TABLE_MAX|STAGE2_PL_PRED_TABLE_SIZE|STAGE2_FRAME_SLOT_MAX|STAGE2_MOD_MAX|MAX_STATES|MAX_GROUPS|MAX_CAPLOG|RK_GRAM_MAX|SC_DAT_MAX_TYPES|INIT_MAX|SHADOW_MAX|CALL_STACK_MAX)$'
RUNTIME_CAPS='^(CALL_ARGS_MAX|RT_FRAME_STACK_MAX|RT_FRAME_SLOT_MAX|FH_MAX|GLOBAL_MAX|RT_MAX_CAPTURES|VSTACK_MAX|SAVE_MAX|FRAME_STACK_MAX|FRAME_SLOT_MAX|FRAME_DEPTH_MAX|EVERY_GEN_SLOT_MAX|SCAN_STACK_MAX|RT_INITIAL_MAX|RT_DC_FNS_MAX|RT_PROC_LOC_MAX|FIELD_ACCESSOR_MAX|RT_CAS_CAPO_MAX|VAR_BUCKETS|ZSM_N|ZDP_RBP_TAB_N|EXPRESSION_REG_MAX|BB_DCAP_MAX|SEQ_CACHE_MAX|SUSP_GEN_CACHE_MAX|ICN_STACK_MAX|NV_MEMO_N|RT_DCAP_NVCACHE_N)$'
case "$mode" in
compile)
  [ -x ./scrip ] || { echo "REFUSE(2): no ./scrip"; exit 2; }
  python3 scripts/util_dyn_caps_gen.py "$T/w" || exit 2
  SBL=/home/resources/x64/bin/sbl; [ -x "$SBL" ] || { echo "REFUSE(2): no SPITBOL oracle at $SBL"; exit 2; }
  "$SBL" -bf "$T/w/defines200.sno" < /dev/null > "$T/a.ref" 2>&1; timeout 60 ./scrip --run "$T/w/defines200.sno" < /dev/null > "$T/a.out" 2> "$T/a.err"
  if cmp -s "$T/a.ref" "$T/a.out"; then echo "PASS defines200.sno: 200 DEFINEs read $(tr -d '\n' < "$T/a.ref") in both"; else echo "RED defines200.sno: sbl -bf prints $(head -c 40 "$T/a.ref" | tr -d '\n'), scrip prints '$(head -c 40 "$T/a.out" | tr -d '\n')' -- $(head -c 140 "$T/a.err" | tr '\n' ' ')"; red=1; fi
  "$SBL" -bf "$T/w/blobs150.sno" < /dev/null > "$T/d.ref" 2>&1; timeout 60 ./scrip --run "$T/w/blobs150.sno" < /dev/null > "$T/d.out" 2> "$T/d.err"
  if cmp -s "$T/d.ref" "$T/d.out"; then echo "PASS blobs150.sno: a run-time pattern whose blob layout needs 608 entries reads $(tr -d '\n' < "$T/d.ref") in both"; else echo "RED blobs150.sno: sbl -bf prints $(head -c 40 "$T/d.ref" | tr -d '\n'), scrip prints '$(head -c 40 "$T/d.out" | tr -d '\n')' -- $(head -c 140 "$T/d.err" | tr '\n' ' ')"; red=1; fi
  command -v gplc > /dev/null || { echo "REFUSE(2): no gplc, the GNU Prolog oracle's native compiler"; exit 2; }
  ( cd "$T/w" && timeout 120 gplc preds600.pl -o preds600.gp > /dev/null 2>&1 ) || { echo "REFUSE(2): gplc could not compile preds600.pl"; exit 2; }
  timeout 120 "$T/w/preds600.gp" < /dev/null > "$T/b.ref" 2> /dev/null; timeout 120 ./scrip --run "$T/w/preds600.pl" < /dev/null > "$T/b.out" 2> "$T/b.err"
  if cmp -s "$T/b.ref" "$T/b.out"; then echo "PASS preds600.pl: 600 predicates, $(wc -l < "$T/b.ref") lines identical to gprolog"; else echo "RED preds600.pl: gprolog prints $(wc -l < "$T/b.ref") lines, scrip $(wc -l < "$T/b.out") -- $(head -c 120 "$T/b.err" | tr '\n' ' ')"; red=1; fi
  L=$(count_caps '^src/(ir|emitter|lower|parsers|driver)/' "$COMPILE_CAPS"); n=$(printf '%s' "$L" | grep -c .); echo "compile-time tables still bound by a population cap: $n"; [ "$n" = 0 ] || { printf '%s\n' "$L"; red=1; }
  ;;
runtime)
  [ -x ./scrip ] || { echo "REFUSE(2): no ./scrip"; exit 2; }
  python3 scripts/util_dyn_caps_gen.py "$T/w" || exit 2
  ICONT=/home/resources/icon-master/bin/icont; [ -x "$ICONT" ] || { echo "REFUSE(2): no icont oracle at $ICONT"; exit 2; }
  ( cd "$T/w" && "$ICONT" -s -o args70x args70.icn > /dev/null 2>&1 && PATH=/home/resources/icon-master/bin:$PATH ./args70x > "$T/c.ref" 2>&1 ) || { echo "REFUSE(2): the oracle could not run args70.icn"; exit 2; }
  timeout 60 ./scrip --run "$T/w/args70.icn" < /dev/null > "$T/c.out" 2> "$T/c.err"
  if cmp -s "$T/c.ref" "$T/c.out"; then echo "PASS args70.icn: a 70-argument call reads $(tr -d '\n' < "$T/c.ref") in both"; else echo "RED args70.icn: iconx prints $(tr -d '\n' < "$T/c.ref"), scrip prints '$(head -c 40 "$T/c.out" | tr -d '\n')' -- $(head -c 140 "$T/c.err" | tr '\n' ' ')"; red=1; fi
  L=$(count_caps '^src/runtime/' "$RUNTIME_CAPS"); n=$(printf '%s' "$L" | grep -c .); echo "runtime tables still bound by a population cap: $n"; [ "$n" = 0 ] || { printf '%s\n' "$L"; red=1; }
  ;;
census)
  B=scripts/fixtures/dyn_caps/BASELINE; [ -f "$B" ] || { echo "REFUSE(2): no baseline at $B"; exit 2; }
  BF=scripts/fixtures/dyn_caps/BASELINE_FUNCTION_SCOPE; [ -f "$BF" ] || { echo "REFUSE(2): no function-scope baseline at $BF"; exit 2; }
  # ⭐ THE GUARD LINES READ THE CLASSIFIER (audit_fixed_caps_census.py, CEO-1231): u is every file/static/field table, macro OR literal
  # bound, with no capacity guard at a fill (never compared, or compared only as an index or an iteration) and not read-only; d is
  # every such table whose guard DROPS -- skips, truncates, clamps or fails silently at the cap. A const table is class A by rule and in
  # neither. ⛔ ONE line ends "never compared in its file: N", on purpose: the row's DONE-WHEN greps that suffix, and two lines ending in
  # it would let either one's zero pass the criterion for both.
  [ "$(head -1 "$T/c.tsv" | cut -f8-9,11)" = "$(printf 'guard\tconst\tfill')" ] || { echo "REFUSE(2): the census TSV carries no guard/const/fill columns -- an older census cannot be read by these lines"; exit 2; }
  # ⭐ ARENAS (CEO-1231 (1), stage 2 (b)): the census's scope `arena` rows -- a constant 64 KiB or more from an allocator -- count with the
  # file-scope tables in all three lines below, and CLASS_AB.tsv may declare them (the sigaltstack).
  # ⭐ THE CLASS A/B DECLARATION IS A FIXTURE (ceo CEO-1234 (1)): a table CLASS_AB.tsv names, with the measurement that earned its class,
  # leaves the no-guard and drop lines as a const table does by rule; the ratchet COUNT is unchanged; a row naming a table the tree no
  # longer declares is RED (a stale keep-list is one nobody notices has stopped applying).
  AB=scripts/fixtures/dyn_caps/CLASS_AB.tsv; [ -f "$AB" ] || { echo "REFUSE(2): no class A/B declaration at $AB"; exit 2; }
  abad=$(awk -F'\t' '!/^#/ && NF && (NF < 4 || ($3 != "A" && $3 != "B") || $4 == "") {print "  line " FNR ": " substr($0, 1, 100)}' "$AB")
  [ -z "$abad" ] || { echo "REFUSE(2): $AB rows that are not file<TAB>name<TAB>A|B<TAB>reason:"; printf '%s\n' "$abad"; exit 2; }
  base=$(cat "$B"); n=$(awk -F'\t' 'NR>1 && ($3=="file"||$3=="static"||$3=="field"||$3=="arena")' "$T/c.tsv" | wc -l)
  u=$(awk -F'\t' 'FNR==NR {if ($0 !~ /^#/ && NF >= 4) ab[$1 SUBSEP $2] = 1; next} FNR>1 && ($3=="file"||$3=="static"||$3=="field"||$3=="arena") && $8=="NONE" && $9=="" && !(($1 SUBSEP $4) in ab)' "$AB" "$T/c.tsv" | wc -l)
  d=$(awk -F'\t' 'FNR==NR {if ($0 !~ /^#/ && NF >= 4) ab[$1 SUBSEP $2] = 1; next} FNR>1 && ($3=="file"||$3=="static"||$3=="field"||$3=="arena") && $8=="DROP" && $9=="" && !(($1 SUBSEP $4) in ab)' "$AB" "$T/c.tsv" | wc -l)
  nab=$(awk -F'\t' 'FNR==NR {if (FNR>1 && ($3=="file"||$3=="static"||$3=="field"||$3=="arena")) have[$1 SUBSEP $4] = 1; next} !/^#/ && NF >= 4 && (($1 SUBSEP $2) in have)' "$T/c.tsv" "$AB" | wc -l)
  stale=$(awk -F'\t' 'FNR==NR {if (FNR>1 && ($3=="file"||$3=="static"||$3=="field"||$3=="arena")) have[$1 SUBSEP $4] = 1; next} !/^#/ && NF >= 4 && !(($1 SUBSEP $2) in have) {print "  " $1 ":" $2}' "$T/c.tsv" "$AB")
  echo "fixed-bound declarations at file, static or field scope and fixed-size arenas: $n (baseline $base); with no capacity guard at a fill, by a macro or a literal bound -- compared only as an index or an iteration, or never compared in its file: $u"
  echo "guards that drop or truncate at the cap: $d"
  echo "declared class A or B by $AB (out of the two lines above, in the count): $nab table(s); stale declarations: $(printf '%s' "$stale" | grep -c .)"
  [ -z "$stale" ] || { echo "RED: $AB declares a table the tree no longer declares -- remove the row in the landing that removed the table:"; printf '%s\n' "$stale"; red=1; }
  # ⭐ THE SECOND POPULATION (CEO-1231 (1), stage 2): every local a program's data fills -- the census's fill column names how (COUNTER,
  # CALLEE:f, FORMAT:f, COPY:f, READ:f, PATH:f); B:<why> is fixed by construction and out; const is read-only and out. Unguarded is DROP
  # and NONE together (CEO-1231 (2): a drop is not a guard); guarded is LOUD, which the reader also gives a GROW (the at-cap path
  # allocates). The population is ratcheted like the file-scope one, against its own BASELINE_FUNCTION_SCOPE.
  bf=$(cat "$BF")
  read -r fn fl fd fx <<<"$(awk -F'\t' 'NR>1 && $3=="local" && $9=="" && $11!="" && $11 !~ /^B:/ {n++; if ($8=="LOUD") l++; else if ($8=="DROP") d++; else x++}
                                    END {print n+0, l+0, d+0, x+0}' "$T/c.tsv")"
  fk=$(awk -F'\t' 'NR>1 && $3=="local" && $11 ~ /^B:/' "$T/c.tsv" | wc -l)
  echo "function-scope arrays a program fills: $fn (baseline $bf) -- guarded by a loud refusal or a growth $fl, dropping at the cap $fd, with no guard $fx; fixed by construction and out (class B, read by machine): $fk"
  echo "function-scope arrays a program fills, unguarded: $((fd + fx))"
  if [ "$n" -gt "$base" ]; then echo "RED: the population of fixed tables grew from $base to $n -- a new fixed limit landed; make it dynamic or declare it class A/B on the page and lower nothing"; red=1; elif [ "$n" -lt "$base" ]; then echo "NOTE: $n is below the baseline $base -- lower BASELINE in the same landing (the ratchet only tightens)"; fi
  if [ "$fn" -gt "$bf" ]; then echo "RED: the function-scope population grew from $bf to $fn -- a new fixed local that a program fills landed; make it grow, and list it: python3 scripts/audit_fixed_caps_census.py --tsv FILE (column fill)"; red=1; elif [ "$fn" -lt "$bf" ]; then echo "NOTE: the function-scope population $fn is below its baseline $bf -- lower BASELINE_FUNCTION_SCOPE in the same landing"; fi
  ;;
*) echo "REFUSE(2): unknown mode $mode"; exit 2;;
esac
[ "$red" = 0 ] && echo "GREEN [$mode]" || echo "RED [$mode]"
exit $red
