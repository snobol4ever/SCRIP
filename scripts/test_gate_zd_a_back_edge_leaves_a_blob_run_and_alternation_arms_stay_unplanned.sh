#!/usr/bin/env bash
# test_gate_zd_a_back_edge_leaves_a_blob_run_and_alternation_arms_stay_unplanned.sh -- TWO zd_plan DEFECTS UNDER THE SELF-HOSTED PARSERS
#
# MEASURED 2026-09-23 by hq_snocone (row snocone-the-seven-bootstrap-parser-sc-parse-every-corpus-source-..., Lon: "Get each parser_*.sc
# working"): every bootstrap parser crashed or printed a wrong tree on SCRIP because its tree dumper (bootstrap/tdump.sc) walks the tree
# with a match test whose pattern has an alternation, inside an if that returns, in a function that recurses from a while loop. Two
# defects in the zd depth planner, both pre-existing (the 73e02c870 binary fails the same witnesses):
#  (A) A RUN WHOSE OPERAND CLOSURE REACHES OUTSIDE IT (nblob > 0 -- any match with an alternation, whose unclaimed arm leaves sit in the
#      ALTERNATE's operand list) decided "is this port's target in the run" by membership alone, so a BACK EDGE -- a while loop's
#      return to its head -- counted as staying in the run and released nothing: every lap leaked the body's spine (192 bytes in the
#      witness) and the function's return popped the wrong word. The run rule already treated a back edge as an exit; the blob rule now
#      does the same (killswitch SCRIP_ZD_BLOBBACK=0).
#  (B) ZD-5B ADMITTED ONE ARM OF AN ALTERNATION INTO THE RUN (an arm leaf with an evaluated argument, e.g. SPAN(v) with v not foldable)
#      as a spine box that keeps its 16-byte cell on success. Unreleased, the continuation read the capture's saved cursor 16 bytes off
#      (a garbage deferred capture, "rt_dcap_pump: CORRUPT CAPTURE ENTRY"); released on success, backtracking into the arm popped it a
#      second time (SIGSEGV). Arms are now unplanned by default, the pre-s102 closure-only form, which grades correct on every witness
#      here and on its own original witness (span_any_rpos_branch_2); SCRIP_ZD_5B=1 opts back in.
# EVERY ARM IS GRADED AGAINST THE ORACLE: the witness's --transpile run on sbl -bf (a Snocone program's oracle), both modes, stdout.
# FAIL-ONCE, BUILT IN: witness 1 must go red under SCRIP_ZD_BLOBBACK=0 and witness 3 under SCRIP_ZD_5B=1 -- a gate that cannot see
# its own defect is not measuring it.
"$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/util_require_fresh.sh" --gate "$(basename "${BASH_SOURCE[0]}" .sh)" || exit $?
set -uo pipefail
G="$(basename "${BASH_SOURCE[0]}" .sh)"
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="$ROOT/scrip"; RT="$ROOT/out"
[ -x "$SCRIP" ] || { echo "⛔ GATE REFUSE(2) [$G]: no scrip at $SCRIP"; exit 2; }
. "$HERE/lib_oracle_flags.sh" 2>/dev/null || { echo "⛔ GATE REFUSE(2) [$G]: cannot load lib_oracle_flags.sh"; exit 2; }
SBL="$(sbl_clean_bin 2>/dev/null)"; [ -x "${SBL:-}" ] || SBL=/home/resources/x64/bin/sbl
[ -x "$SBL" ] || { echo "⛔ GATE REFUSE(2) [$G]: no SPITBOL oracle"; exit 2; }
T=$(mktemp -d) || exit 2; trap 'rm -rf "$T"' EXIT
cat > "$T/w1.sc" <<'EOF'
function F(n, i, t) {
    if (DIFFER(n)) {
        if ('AP' ? (POS(0) (SPAN('AP') | '') RPOS(0))) t = 'y'; else t = 'q';
        OUTPUT = n t;
        i = 0;
        while (i = LT(i, n) i + 1) F(n - 1);
        return;
    }
    return;
}
F(2);
OUTPUT = 'after';
EOF
cat > "$T/w2.sc" <<'EOF'
struct tree { t, v, n, c }
function TD(x, i, t) {
    if (DIFFER(n(x))) {
        if (~(t(x) ? (POS(0) ANY(&UCASE) (SPAN(&UCASE) | '') RPOS(0)))) t = 'q'; else t = t(x);
        OUTPUT = '(' t;
        i = 0;
        while (i = LT(i, n(x)) i + 1) TD(c(x)[i]);
        OUTPUT = ')';
        return;
    }
    return;
}
a = tree('A', '', 0, ''); b = tree('B', '', 0, ''); p = tree('P', '', 2, ARRAY(2)); c(p)[1] = a; c(p)[2] = b;
TD(p);
OUTPUT = 'after';
EOF
cat > "$T/w3.sc" <<'EOF'
digits = DUPL('0', 1) '123456789';
alpha = DUPL('a', 1) 'bc';
s = '12x';
if (s ? (POS(0) (SPAN(digits) | SPAN(alpha)) RPOS(0))) { OUTPUT = 'ident'; } else { OUTPUT = 'quoted'; }
OUTPUT = 'after';
EOF
cat > "$T/w4.sc" <<'EOF'
digits = DUPL('0', 1) '123456789';
s = '12';
if (s ? (POS(0) (SPAN(digits) | '') RPOS(0))) { OUTPUT = 'ident'; } else { OUTPUT = 'quoted'; }
t = 'T12';
if (t ? (POS(0) ANY('T') (SPAN(digits) | '') RPOS(0))) { OUTPUT = 'ident2'; } else { OUTPUT = 'quoted2'; }
EOF
run_mode() {  # run_mode <m3|m4> <prog> [ENV=V...] -> stdout on fd 1
  local m="$1" p="$2"; shift 2
  if [ "$m" = m3 ]; then env "$@" timeout 20 "$SCRIP" "$p" </dev/null 2>/dev/null; return; fi
  env "$@" "$SCRIP" --compile -o "$p.s" "$p" </dev/null >/dev/null 2>&1 || { echo "COMPILE-FAIL"; return; }
  gcc -no-pie -o "$p.x" "$p.s" -L"$RT" -Wl,-rpath,"$RT" -lscrip_rt -lm >/dev/null 2>&1 || { echo "LINK-FAIL"; return; }
  timeout 20 "$p.x" </dev/null 2>/dev/null; }
RC=0; N=0
for w in w1 w2 w3 w4; do
  "$SCRIP" --transpile "$T/$w.sc" > "$T/$w.sno" 2>/dev/null || { echo "⛔ GATE REFUSE(2) [$G]: $w does not transpile"; exit 2; }
  want=$("$SBL" -bf "$T/$w.sno" </dev/null 2>/dev/null)
  [ -n "$want" ] || { echo "⛔ GATE REFUSE(2) [$G]: the oracle printed nothing for $w"; exit 2; }
  for m in m3 m4; do N=$((N+1)); got=$(run_mode "$m" "$T/$w.sc")
    if [ "$got" = "$want" ]; then echo "  $w $m PASS"; else echo "  $w $m FAIL: got [$(printf '%s' "$got" | tr '\n' ' ' | cut -c1-60)] want [$(printf '%s' "$want" | tr '\n' ' ')]"; RC=1; fi; done
done
fo() {  # fo <witness> <ENV=V...> -- the killswitch must make the witness differ from the oracle in m3
  local w="$1"; shift; local want got; want=$("$SBL" -bf "$T/$w.sno" </dev/null 2>/dev/null); got=$(run_mode m3 "$T/$w.sc" "$@")
  if [ "$got" != "$want" ]; then echo "  fail-once $w under $*: RED as it must be"; else echo "  fail-once $w under $*: STILL GREEN -- this gate cannot see the defect it names"; RC=1; fi; }
fo w1 SCRIP_ZD_BLOBBACK=0
fo w3 SCRIP_ZD_5B=1
if [ "$RC" = 0 ]; then echo "GATE PASS [$G]: $N arms (4 witnesses x m3+m4) print the SPITBOL oracle's output, and both killswitches red their witness"
else echo "GATE FAIL(1) [$G]: see the arms above"; fi
echo "    tree: SCRIP=$(git -C "$ROOT" rev-parse --short HEAD 2>/dev/null)$(git -C "$ROOT" diff --quiet 2>/dev/null || echo -DIRTY)  oracle: $SBL -bf  measured $(date -u +%Y-%m-%dT%H:%MZ)"
exit $RC
