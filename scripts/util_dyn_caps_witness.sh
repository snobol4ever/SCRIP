#!/usr/bin/env bash
# util_dyn_caps_witness.sh compile|runtime|census -- the criteria of ARCH-DYNAMIC-STORAGE.md (Lon 2026-09-23: no fixed limits).
# compile: the three compile-time witnesses (200 DEFINEs and a 608-entry blob layout vs sbl -bf, 600 predicates vs gprolog) plus zero compiler tables bound
#          by a compile-time population cap in the census; runtime: the 70-argument Icon call vs iconx plus zero runtime tables
#          bound by a runtime population cap; census: the ratchet -- file/static/field declarations may only fall from BASELINE.
# rc 0 GREEN, 1 RED, 2 REFUSE (an oracle or the binary missing). The population regexes are the page's § 6 list, spelled once here.
# The GNU Prolog oracle is gplc's native binary, never `gprolog --consult-file`: the top level prints a four-line banner and consult's two
# "compiling ..." lines to STDOUT before the program runs, so that arm read 606 lines against a correct 600 and could never go green (cfo 2026-09-23).
set -u
HERE=$(cd "$(dirname "$0")" && pwd); cd "$HERE/.." || exit 2
mode=${1:-}; [ -n "$mode" ] || { echo "REFUSE(2): usage: util_dyn_caps_witness.sh compile|runtime|census"; exit 2; }
T=$(mktemp -d); trap 'rm -rf "$T"' EXIT
python3 scripts/audit_fixed_caps_census.py --tsv "$T/c.tsv" --no-report > "$T/census.txt" 2>&1 || { echo "REFUSE(2): the census refused -- $(tail -2 "$T/census.txt" | tr '\n' ' ')"; exit 2; }
red=0
count_caps() { awk -F'\t' -v dirs="$1" -v caps="$2" 'NR>1 && ($3=="file"||$3=="static"||$3=="field") && $1 ~ dirs && $5 ~ caps {print "  " $1 ":" $2 " " $4 "[" $5 "]"}' "$T/c.tsv"; }
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
  # ⭐ THE GUARD LINES READ THE CLASSIFIER (audit_fixed_caps_census.py, CEO-1231): u is every file/static/field table, macro OR literal
  # bound, with no capacity guard at a fill (never compared, or compared only as an index or an iteration) and not read-only; d is
  # every such table whose guard DROPS -- skips, truncates, clamps or fails silently at the cap. A const table is class A by rule and in
  # neither. ⛔ ONE line ends "never compared in its file: N", on purpose: the row's DONE-WHEN greps that suffix, and two lines ending in
  # it would let either one's zero pass the criterion for both.
  [ "$(head -1 "$T/c.tsv" | cut -f8-9)" = "$(printf 'guard\tconst')" ] || { echo "REFUSE(2): the census TSV carries no guard/const columns -- an older census cannot be read by these lines"; exit 2; }
  base=$(cat "$B"); n=$(awk -F'\t' 'NR>1 && ($3=="file"||$3=="static"||$3=="field")' "$T/c.tsv" | wc -l)
  u=$(awk -F'\t' 'NR>1 && ($3=="file"||$3=="static"||$3=="field") && $8=="NONE" && $9==""' "$T/c.tsv" | wc -l)
  d=$(awk -F'\t' 'NR>1 && ($3=="file"||$3=="static"||$3=="field") && $8=="DROP" && $9==""' "$T/c.tsv" | wc -l)
  echo "fixed-bound declarations at file, static or field scope: $n (baseline $base); with no capacity guard at a fill, by a macro or a literal bound -- compared only as an index or an iteration, or never compared in its file: $u"
  echo "guards that drop or truncate at the cap: $d"
  echo "function-scope arrays a program fills, unguarded: NOT YET COUNTED -- CEO-1231's second population, its reader not landed"
  if [ "$n" -gt "$base" ]; then echo "RED: the population of fixed tables grew from $base to $n -- a new fixed limit landed; make it dynamic or declare it class A/B on the page and lower nothing"; red=1; elif [ "$n" -lt "$base" ]; then echo "NOTE: $n is below the baseline $base -- lower BASELINE in the same landing (the ratchet only tightens)"; fi
  ;;
*) echo "REFUSE(2): unknown mode $mode"; exit 2;;
esac
[ "$red" = 0 ] && echo "GREEN [$mode]" || echo "RED [$mode]"
exit $red
