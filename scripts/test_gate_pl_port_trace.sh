#!/usr/bin/env bash
# test_gate_pl_port_trace.sh -- BX-0 THE PORT-TRACE INSTRUMENT (ARCH-PROLOG-BYRD-BOX-TRANSLATION.md § D / § E rung 1; hq_P 2026-09-02).
# SCRIP_PL_TRACE=1 makes x86_port_hook emit one runtime call per Byrd port (Call=α def, Redo=β def, Exit=γ jmp, Fail=ω jmp) that
# prints "(N) D Port: box [-> target]" to stderr; =2 counts hits per box and prints the four-port table at exit; =3 both.
# Graded population: every origin of the FAMILIES list (default `probe_plz ladder`: the PZ-0 witnesses and the construct-ladder
# witnesses of ARCH-PROLOG-BYRD-BOX-TRANSLATION.md § E, hq_B 2026-09-02) of corpus/tests/prolog/ALL.pl, both modes. Per witness:
#   (1) KILLSWITCH BOTH WAYS: the mode-4 .s with no env and with SCRIP_PL_TRACE=0 are byte-identical; with =1 it differs.
#   (2) NO PERTURBATION: stdout and rc are identical with and without the trace, m3 and m4.
#   (3) THE TRACE MATCHES ITS REF line for line after normalisation: depth column dropped (the brief's clause), n<k>_ node numbers
#       stripped (m3 numbers per graph, m4 per program; the ref pins the port SEQUENCE, not the numbering), $2F unmangled to /,
#       and the raw r15= field dropped (a mmap address inherited from the driver, ASLR-random per run; hq_C's C37 makes it the ball).
#       A ref block may be a PREFIX (banner carries total= and prefix=): the first prefix lines are compared AND the total line
#       count must match, so a runaway witness is pinned whole without storing its 60k lines.
#   (4) ANSWER column, informational: stdout vs the entry's .ref -- the master suite owns answers, this gate owns traces.
# --cut rewrites corpus/tests/prolog/ALL.trace from the live traces (the ONE way the ref changes; never hand-edit it).
# EXIT: 0 every witness matches in both modes; 1 a mismatch, a killswitch or perturbation failure; 2 REFUSED (cannot measure).
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"; cd "$ROOT"
. "$HERE/lib_gate.sh"
CUT=0; for a in "$@"; do [ "$a" = --cut ] && CUT=1; done
gate_parse_args "$@"
SCRIP="${SCRIP:-$ROOT/scrip}"; RT="${RT_DIR:-$ROOT/out}"
MASTER_DIR="$S4E/corpus/tests/prolog"; MASTER_EXT=.pl; export MASTER_DIR MASTER_EXT
REF="$MASTER_DIR/ALL.trace"; PREFIX_CAP="${PREFIX_CAP:-400}"; T="${TIMEOUT:-20}"
gate_require_exec "$SCRIP" "scrip binary"
gate_require "$RT/libscrip_rt.so" "runtime library"
gate_require "$MASTER_DIR/ALL.pl" "Prolog master suite"
gate_require "$MASTER_DIR/ALL.csv" "Prolog master suite index"
[ "$CUT" = 1 ] || gate_require "$REF" "trace refs (run with --cut to create them)"
. "$HERE/lib_master_extract.sh" || { echo "GATE UNPROVEN(2) [$GATE_NAME]: cannot source lib_master_extract.sh"; exit 2; }
FAMILIES="${FAMILIES:-probe_plz ladder}"; origins=""
for fam in $FAMILIES; do o=$(master_origins_of_family "$fam") || o=""; [ -n "$o" ] || { echo "GATE UNPROVEN(2) [$GATE_NAME]: no $fam origins in $MASTER_DIR/ALL.csv -- the witnesses moved; re-point, never skip"; gate_stamp; exit 2; }; origins="$origins $o"; done
W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
norm() { grep -E '^\([0-9]+\) [0-9]+ (Call|Exit|Redo|Fail|Exception): ' "$1" | sed -E 's/^(\([0-9]+\)) [0-9]+ /\1 /; s/\bn[0-9]+_//g; s/\$2F/\//g; s/ r15=0x[0-9a-f]+$//'; }
m4build() { [ -s "$1" ] && as --64 -o "$1.o" "$1" 2>/dev/null && gcc -no-pie -o "$2" "$1.o" "$RT/libscrip_rt.so" -lm -lstdc++ -Wl,-rpath,"$RT" 2>/dev/null; }
refblock() { awk -v o="$1" -v m="$2" '$0 ~ "^%---- " o " " m " " {on=1; next} /^%---- /{on=0} on' "$REF"; }
refmeta()  { grep -E "^%---- $1 $2 " "$REF" | head -1 | sed -E 's/.* total=([0-9]+) prefix=([0-9]+).*/\1 \2/'; }
n=0; bad=0; ans_ok=0; ans_red=0; declare -a lines
[ "$CUT" = 1 ] && : > "$W/ALL.trace"
for o in $origins; do
  n=$((n+1)); src="$W/$o.pl"; ref="$W/$o.ref"
  master_extract_origin "$o" "$src" "$ref" >/dev/null 2>&1 || { echo "GATE UNPROVEN(2) [$GATE_NAME]: cannot extract $o from the master suite"; gate_stamp; exit 2; }
  (cd "$W" && timeout "$T" "$SCRIP" --compile -o "$o.s0" "$src" </dev/null >/dev/null 2>&1); (cd "$W" && SCRIP_PL_TRACE=0 timeout "$T" "$SCRIP" --compile -o "$o.s0b" "$src" </dev/null >/dev/null 2>&1)
  (cd "$W" && SCRIP_PL_TRACE=1 timeout "$T" "$SCRIP" --compile -o "$o.s1" "$src" </dev/null >/dev/null 2>&1)
  ks=OK; { cmp -s "$W/$o.s0" "$W/$o.s0b" && [ -s "$W/$o.s0" ] && ! cmp -s "$W/$o.s0" "$W/$o.s1"; } || { ks=FAIL; bad=$((bad+1)); }
  timeout "$T" "$SCRIP" --run "$src" </dev/null >"$W/$o.m3.out0" 2>/dev/null; r30=$?
  SCRIP_PL_TRACE=1 timeout "$T" "$SCRIP" --run "$src" </dev/null >"$W/$o.m3.out1" 2>"$W/$o.m3.raw"; r31=$?
  pert3=OK; { [ "$r30" = "$r31" ] && cmp -s "$W/$o.m3.out0" "$W/$o.m3.out1"; } || { pert3=FAIL; bad=$((bad+1)); }
  pert4=OK; r40=?; r41=?
  if m4build "$W/$o.s0" "$W/$o.bin0" && m4build "$W/$o.s1" "$W/$o.bin1"; then
    timeout "$T" "$W/$o.bin0" </dev/null >"$W/$o.m4.out0" 2>/dev/null; r40=$?
    SCRIP_PL_TRACE=1 timeout "$T" "$W/$o.bin1" </dev/null >"$W/$o.m4.out1" 2>"$W/$o.m4.raw"; r41=$?
    { [ "$r40" = "$r41" ] && cmp -s "$W/$o.m4.out0" "$W/$o.m4.out1"; } || { pert4=FAIL; bad=$((bad+1)); }
  else pert4=NOBUILD; bad=$((bad+1)); : > "$W/$o.m4.raw"; fi
  cmp -s "$W/$o.m3.out0" "$ref" && { ans=ok; ans_ok=$((ans_ok+1)); } || { ans=RED; ans_red=$((ans_red+1)); }
  tr3=?; tr4=?
  for m in m3 m4; do
    norm "$W/$o.$m.raw" > "$W/$o.$m.norm"; total=$(wc -l < "$W/$o.$m.norm")
    [ "$total" -gt 0 ] || { echo "GATE UNPROVEN(2) [$GATE_NAME]: $o $m: SCRIP_PL_TRACE=1 produced ZERO trace lines -- the instrument is not firing, this is not 'no ports'"; gate_stamp; exit 2; }
    if [ "$CUT" = 1 ]; then
      p=$total; [ "$p" -gt "$PREFIX_CAP" ] && p=$PREFIX_CAP
      { echo "%---- $o $m total=$total prefix=$p"; head -n "$p" "$W/$o.$m.norm"; } >> "$W/ALL.trace"; v=CUT
    else
      meta=$(refmeta "$o" "$m"); rt=${meta%% *}; rp=${meta##* }
      if [ -z "$meta" ]; then v=NOREF; bad=$((bad+1))
      elif [ "$rt" != "$total" ]; then v="FAIL(total $total != ref $rt)"; bad=$((bad+1))
      elif ! refblock "$o" "$m" | cmp -s - <(head -n "$rp" "$W/$o.$m.norm"); then v="FAIL(diff within first $rp)"; bad=$((bad+1)); refblock "$o" "$m" | diff - <(head -n "$rp" "$W/$o.$m.norm") | head -6 | sed 's/^/        /' > "$W/$o.$m.diff"
      else v="ok($total)"; fi
    fi
    [ "$m" = m3 ] && tr3="$v" || tr4="$v"
  done
  lines+=("$(printf '%-40s killswitch=%-4s perturb m3=%-4s m4=%-7s trace m3=%-22s m4=%-22s answer=%s' "$o" "$ks" "$pert3" "$pert4" "$tr3" "$tr4" "$ans")")
  for m in m3 m4; do [ -f "$W/$o.$m.diff" ] && lines+=("$(cat "$W/$o.$m.diff")"); done
done
printf '%s\n' "${lines[@]}"
if [ "$CUT" = 1 ]; then cp "$W/ALL.trace" "$REF"; echo "refs CUT -> $REF ($(grep -c '^%---- ' "$REF") blocks, prefix cap $PREFIX_CAP)"; fi
echo "witnesses=$n modes=2 (m3 --run, m4 --compile+as+gcc) . answer ok=$ans_ok red=$ans_red (informational: the master suite grades answers) . normalisation: depth dropped, n<k>_ stripped, \$2F->/, r15= dropped"
GATE_EXAMINED=$((n*2)); gate_verdict "$bad" "failed checks across killswitch/perturbation/trace"
