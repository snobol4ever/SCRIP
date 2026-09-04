#!/usr/bin/env bash
# lib_port_trace.sh -- THE ONE PORT-TRACE GATE BODY, shared by every language's test_gate_<lang>_port_trace.sh
# (GOAL-TEST-SUITE-CONSISTENCY.md standard point 6; row test-suite-consistency-seven-languages-one-standard,
# hq_T 2026-09-03). Extracted from the PAIR test_gate_pl_port_trace.sh (hq_P 2026-09-02, the only instance that
# existed) and test_gate_sno_port_trace.sh (hq_T, minted alongside this file) -- never from the single example:
# with one instance the seams are a guess, and lib_ladder.sh worked precisely because two near-identical bodies
# could be diffed to prove which tokens actually vary. Measured here: they varied in FOUR tokens and nothing else.
#
# A per-language gate sets four variables and calls port_trace_main "$@":
#   PORTTRACE_LANG      display name used in refusal text       e.g. Prolog
#   PORTTRACE_SUITE     directory under corpus/tests/           e.g. prolog
#   PORTTRACE_EXT       master/source extension, leading dot    e.g. .pl
#   PORTTRACE_FAMILIES  default graded families                 e.g. "probe_plz ladder"
#
# ⭐⭐ THE INSTRUMENT IS OURS, NOT THE ORACLE'S (ceo CEO-172, 2026-09-03, on hq_T's measured correction of its
# own ask). SCRIP_PL_TRACE=1 makes x86_port_hook -- installed at the GENERIC x86_jcc/x86_jmp/x86_deflabel port
# sites in src/templates/x86/x86_asm.h that every language's Byrd boxes flow through -- emit one runtime call
# per port (Call=α proceed, Redo=β recede, Exit=γ succeed, Fail=ω concede) printing "(N) D Port: box [-> target]"
# to stderr. ALL SEVEN LANGUAGES ALREADY EMIT IT with zero compiler change, so this gate is reachable for all
# seven, not the three whose oracles happen to have a tracer.
# ⛔ THE FLAG'S NAME IS STILL `SCRIP_PL_TRACE` AND THAT IS A KNOWN RULES.md LANGUAGE-IDENTITY VIOLATION -- a
# language name past the lower boundary, which test_gate_emit_no_lang.sh cannot see because it is an env var,
# not an IR branch (FINDING-2026-09-03-hq_T-all-seven-languages-already-emit-byrd-port-traces-only-the-flag-is-
# named-prolog.md). The rename to SCRIP_PORT_TRACE is a src/ change and is DISPATCHED, not taken here; this body
# reads the env var through PORT_TRACE_ENV below so the rename is one line in one file when it lands.
#
# ⛔⭐ WHAT THE REF ACTUALLY IS, AND THE LABEL IS NOT OPTIONAL: `--cut` writes ALL.trace from SCRIP'S OWN traces.
# That makes every gate over THIS body a SELF-CONSISTENCY REGRESSION PIN -- it proves the port sequence has not
# MOVED, never that it is RIGHT. Say "pinned" in every receipt that quotes one; a self-pin called an oracle diff
# is a false provenance claim, and this file exists partly so that claim is never made by accident.
#
# ⭐⭐ POINT 6 HAS TWO SHAPES, AND THIS BODY IS ONLY THE FIRST. `test_gate_icn_port_trace.sh` (ceo mint
# 2026-09-03, row icon-port-trace-gate-against-ampersand-trace) is a REAL ORACLE DIFF: iconx's `&trace := -1`
# prints one line per user-procedure activation, and that gate normalises it onto the same four Byrd ports and
# grades SCRIP against it. It is DELIBERATELY NOT built on this body and must not be "unified" into it -- its ref
# is one oracle-anchored block rather than a per-mode pair (the reference is what Icon semantics REQUIRE, so it
# cannot vary by mode by construction) and its granularity is procedure-level, not every sub-expression box.
# Folding it in would demote the strongest instrument of the seven to the weakest common shape.
#   SELF-PIN (this body): reachable for ALL SEVEN today, because x86_port_hook is language-blind.
#   ORACLE DIFF: reachable only where the oracle traces AND someone builds the normalisation. Icon: BUILT.
#     Prolog trace/0 and SNOBOL4 &TRACE emit goal/statement events that no one has yet normalised onto the four
#     ports; the Icon gate is the proof that it CAN be done, so those two are UNBUILT, not impossible. Do not
#     write "no oracle diff exists" for them -- write "not built yet", which is what is true.
#
# PER WITNESS, in both modes:
#   (1) KILLSWITCH BOTH WAYS: the mode-4 .s with no env and with the flag =0 are byte-identical; with =1 it differs.
#   (2) NO PERTURBATION: stdout and rc are identical with and without the trace, m3 and m4.
#   (3) THE TRACE MATCHES ITS REF line for line after normalisation: depth column dropped, n<k>_ node numbers
#       stripped (m3 numbers per graph, m4 per program; the ref pins the port SEQUENCE, not the numbering),
#       $2F unmangled to /, and the raw r15= field dropped (a mmap address inherited from the driver, ASLR-random
#       per run). A ref block may be a PREFIX (banner carries total= and prefix=): the first prefix lines are
#       compared AND the total line count must match, so a runaway witness is pinned whole without storing 60k lines.
#   (4) ANSWER column, informational: stdout vs the entry's .ref -- the master suite owns answers, this gate owns traces.
# SELECTORS over the ladder family (mutually exclusive; a request naming BOTH REFUSES rc=2 rather than guessing):
#   --to N     grade/cut rungs 0..N CUMULATIVELY.
#   --only N   grade/cut rung N ALONE -- the rung under construction, while the rungs BELOW it are still red.
#              ⭐ THE INSTRUMENT MUST BE ABLE TO ADDRESS THE RUNG BEING BUILT: a cumulative-only gate makes every
#              rung hostage to the reds beneath it, which is precisely backwards for a ladder landing one rung at a time.
# --cut rewrites corpus/tests/<suite>/ALL.trace from the live traces (the ONE way the ref changes; never hand-edit it).
# ⭐ --cut UNDER A SELECTOR IS A MERGE, NEVER A TRUNCATION: the blocks of the graded origins are re-cut and every
#   OTHER block in the ref is kept verbatim, so cutting rung 6 alone cannot silently drop rungs 0-1's refs.
# PRINTS ITS DENOMINATOR. EXIT: 0 every witness matches in both modes; 1 a mismatch, a killswitch or perturbation
# failure; 2 REFUSED (cannot measure) -- a gate that cannot measure never prints the success shape.
PORT_TRACE_ENV="${PORT_TRACE_ENV:-SCRIP_PL_TRACE}"
# ⭐ GATE_NAME is resolved from the CALLER's filename, not this library's: lib_gate.sh derives it from
# BASH_SOURCE[1], which after this indirection would read "lib_port_trace" for every language and put the
# wrong name in every refusal. Pinned here, once, before lib_gate.sh is sourced.
GATE_NAME="${GATE_NAME:-$(basename "${BASH_SOURCE[1]:-gate}" .sh)}"
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"; cd "$ROOT"
. "$HERE/lib_gate.sh"
port_trace_main() {
  # ⛔ SCRIP / RT / T / MASTER_DIR / MASTER_EXT ARE DELIBERATELY NOT `local`: `local X` shadows the inherited
  # environment, so `local SCRIP` would make SCRIP=... on the command line silently unreachable and the gate
  # would grade the DEFAULT binary while you pointed it at another -- a green board about the wrong tree.
  # That exact defect was introduced and caught during the lib_ladder.sh extraction; it is not hypothetical.
  : "${PORTTRACE_LANG:?lib_port_trace.sh: PORTTRACE_LANG unset -- the per-language gate must set it}"
  : "${PORTTRACE_SUITE:?lib_port_trace.sh: PORTTRACE_SUITE unset -- the per-language gate must set it}"
  : "${PORTTRACE_EXT:?lib_port_trace.sh: PORTTRACE_EXT unset -- the per-language gate must set it}"
  TO=""; ONLY=""; ARGS=(); while [ $# -gt 0 ]; do case "$1" in --to) TO="${2:-}"; shift 2;; --to=*) TO="${1#--to=}"; shift;; --only) ONLY="${2:-}"; shift 2;; --only=*) ONLY="${1#--only=}"; shift;; *) ARGS+=("$1"); shift;; esac; done; set -- ${ARGS[@]+"${ARGS[@]}"}
  case "$TO" in ""|*[!0-9]*) [ -z "$TO" ] || { echo "GATE UNPROVEN(2) [$GATE_NAME]: --to wants a rung number, got '$TO'"; exit 2; };; esac
  case "$ONLY" in ""|*[!0-9]*) [ -z "$ONLY" ] || { echo "GATE UNPROVEN(2) [$GATE_NAME]: --only wants a rung number, got '$ONLY'"; exit 2; };; esac
  # ⛔ TWO SELECTORS NAME TWO DIFFERENT POPULATIONS (cumulative 0..N vs rung N alone) -- REFUSE, never silently prefer one.
  [ -z "$TO" ] || [ -z "$ONLY" ] || { echo "GATE UNPROVEN(2) [$GATE_NAME]: --to $TO and --only $ONLY name two different populations (cumulative 0..N vs rung N alone) -- pass one, never both"; exit 2; }
  if [ -n "$ONLY" ]; then SELDESC="at rung $ONLY alone"; SELTAG="--only $ONLY"; elif [ -n "$TO" ]; then SELDESC="at or below rung $TO"; SELTAG="--to $TO"; else SELDESC=""; SELTAG="all families"; fi
  CUT=0; for a in "$@"; do [ "$a" = --cut ] && CUT=1; done
  gate_parse_args "$@"
  SCRIP="${SCRIP:-$ROOT/scrip}"; RT="${RT_DIR:-$ROOT/out}"
  MASTER_DIR="$S4E/corpus/tests/$PORTTRACE_SUITE"; MASTER_EXT="$PORTTRACE_EXT"; export MASTER_DIR MASTER_EXT
  REF="$MASTER_DIR/ALL.trace"; PREFIX_CAP="${PREFIX_CAP:-400}"; T="${TIMEOUT:-20}"
  gate_require_exec "$SCRIP" "scrip binary"
  gate_require "$RT/libscrip_rt.so" "runtime library"
  gate_require "$MASTER_DIR/ALL$PORTTRACE_EXT" "$PORTTRACE_LANG master suite"
  gate_require "$MASTER_DIR/ALL.csv" "$PORTTRACE_LANG master suite index"
  [ "$CUT" = 1 ] || gate_require "$REF" "trace refs (run with --cut to create them)"
  . "$HERE/lib_master_extract.sh" || { echo "GATE UNPROVEN(2) [$GATE_NAME]: cannot source lib_master_extract.sh"; exit 2; }
  FAMILIES="${FAMILIES:-${PORTTRACE_FAMILIES:-ladder}}"; [ -z "$TO$ONLY" ] || FAMILIES="${FAMILIES_TO:-ladder}"; origins=""
  for fam in $FAMILIES; do o=$(master_origins_of_family "$fam") || o=""; [ -n "$o" ] || { echo "GATE UNPROVEN(2) [$GATE_NAME]: no $fam origins in $MASTER_DIR/ALL.csv -- the witnesses moved; re-point, never skip"; gate_stamp; exit 2; }; origins="$origins $o"; done
  if [ -n "$TO$ONLY" ]; then kept=""; for o in $origins; do nn=$(printf '%s\n' "$o" | sed -nE 's/^ladder__rung0*([0-9]+)_.*/\1/p'); [ -n "$nn" ] || continue
      if [ -n "$ONLY" ]; then [ "$nn" -eq "$ONLY" ] && kept="$kept $o"; else [ "$nn" -le "$TO" ] && kept="$kept $o"; fi; done; origins="$kept"
    # ⛔ AN EMPTY SELECTION IS UNMEASURED, NEVER A PASS -- a rung with no witness must refuse exactly as the ladder runner does.
    [ -n "${origins// /}" ] || { echo "GATE UNPROVEN(2) [$GATE_NAME]: no ladder origins $SELDESC in $MASTER_DIR/ALL.csv"; gate_stamp; exit 2; }; fi
  W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
  norm() { grep -E '^\([0-9]+\) [0-9]+ (Call|Exit|Redo|Fail|Exception): ' "$1" | sed -E 's/^(\([0-9]+\)) [0-9]+ /\1 /; s/\bn[0-9]+_//g; s/\$2F/\//g; s/ r15=0x[0-9a-f]+$//'; }
  m4build() { [ -s "$1" ] && as --64 -o "$1.o" "$1" 2>/dev/null && gcc -no-pie -o "$2" "$1.o" "$RT/libscrip_rt.so" -lm -lstdc++ -Wl,-rpath,"$RT" 2>/dev/null; }
  refblock() { awk -v o="$1" -v m="$2" '$0 ~ "^%---- " o " " m " " {on=1; next} /^%---- /{on=0} on' "$REF"; }
  refmeta()  { grep -E "^%---- $1 $2 " "$REF" | head -1 | sed -E 's/.* total=([0-9]+) prefix=([0-9]+).*/\1 \2/'; }
  n=0; bad=0; ans_ok=0; ans_red=0; declare -a lines
  [ "$CUT" = 1 ] && : > "$W/ALL.trace"
  for o in $origins; do
    n=$((n+1)); src="$W/$o$PORTTRACE_EXT"; ref="$W/$o.ref"
    master_extract_origin "$o" "$src" "$ref" >/dev/null 2>&1 || { echo "GATE UNPROVEN(2) [$GATE_NAME]: cannot extract $o from the master suite"; gate_stamp; exit 2; }
    (cd "$W" && timeout "$T" "$SCRIP" --compile -o "$o.s0" "$src" </dev/null >/dev/null 2>&1); (cd "$W" && env "$PORT_TRACE_ENV=0" timeout "$T" "$SCRIP" --compile -o "$o.s0b" "$src" </dev/null >/dev/null 2>&1)
    (cd "$W" && env "$PORT_TRACE_ENV=1" timeout "$T" "$SCRIP" --compile -o "$o.s1" "$src" </dev/null >/dev/null 2>&1)
    ks=OK; { cmp -s "$W/$o.s0" "$W/$o.s0b" && [ -s "$W/$o.s0" ] && ! cmp -s "$W/$o.s0" "$W/$o.s1"; } || { ks=FAIL; bad=$((bad+1)); }
    timeout "$T" "$SCRIP" --run "$src" </dev/null >"$W/$o.m3.out0" 2>/dev/null; r30=$?
    env "$PORT_TRACE_ENV=1" timeout "$T" "$SCRIP" --run "$src" </dev/null >"$W/$o.m3.out1" 2>"$W/$o.m3.raw"; r31=$?
    pert3=OK; { [ "$r30" = "$r31" ] && cmp -s "$W/$o.m3.out0" "$W/$o.m3.out1"; } || { pert3=FAIL; bad=$((bad+1)); }
    pert4=OK; r40=?; r41=?
    if m4build "$W/$o.s0" "$W/$o.bin0" && m4build "$W/$o.s1" "$W/$o.bin1"; then
      timeout "$T" "$W/$o.bin0" </dev/null >"$W/$o.m4.out0" 2>/dev/null; r40=$?
      env "$PORT_TRACE_ENV=1" timeout "$T" "$W/$o.bin1" </dev/null >"$W/$o.m4.out1" 2>"$W/$o.m4.raw"; r41=$?
      { [ "$r40" = "$r41" ] && cmp -s "$W/$o.m4.out0" "$W/$o.m4.out1"; } || { pert4=FAIL; bad=$((bad+1)); }
    else pert4=NOBUILD; bad=$((bad+1)); : > "$W/$o.m4.raw"; fi
    cmp -s "$W/$o.m3.out0" "$ref" && { ans=ok; ans_ok=$((ans_ok+1)); } || { ans=RED; ans_red=$((ans_red+1)); }
    tr3=?; tr4=?
    for m in m3 m4; do
      norm "$W/$o.$m.raw" > "$W/$o.$m.norm"; total=$(wc -l < "$W/$o.$m.norm")
      [ "$total" -gt 0 ] || { echo "GATE UNPROVEN(2) [$GATE_NAME]: $o $m: $PORT_TRACE_ENV=1 produced ZERO trace lines -- the instrument is not firing, this is not 'no ports'"; gate_stamp; exit 2; }
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
  if [ "$CUT" = 1 ]; then
    if [ -n "$TO$ONLY" ] && [ -f "$REF" ]; then
      printf '%s\n' $origins > "$W/graded.txt"
      awk -v gl="$W/graded.txt" 'BEGIN{while ((getline l < gl) > 0) g[l]=1} /^%---- /{keep = !($2 in g)} keep' "$REF" > "$W/kept.trace"
      cat "$W/kept.trace" "$W/ALL.trace" > "$REF"; echo "refs CUT (merged, $SELTAG re-cut, other blocks kept) -> $REF ($(grep -c '^%---- ' "$REF") blocks, prefix cap $PREFIX_CAP)"
    else cp "$W/ALL.trace" "$REF"; echo "refs CUT -> $REF ($(grep -c '^%---- ' "$REF") blocks, prefix cap $PREFIX_CAP)"; fi
  fi
  echo "witnesses=$n ($SELTAG) modes=2 (m3 --run, m4 --compile+as+gcc) . answer ok=$ans_ok red=$ans_red (informational: the master suite grades answers) . normalisation: depth dropped, n<k>_ stripped, \$2F->/, r15= dropped"
  GATE_EXAMINED=$((n*2)); gate_verdict "$bad" "failed checks across killswitch/perturbation/trace"
}
