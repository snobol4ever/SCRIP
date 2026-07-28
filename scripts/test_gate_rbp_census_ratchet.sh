#!/usr/bin/env bash
# scripts/test_gate_rbp_census_ratchet.sh — FLATDISP rung (d), s193.
#
# THE HONEST rbp INSTRUMENT.  Predecessor instruments each reported convergence the bytes
# contradicted (s184 patience-FC, s188 subset-FC via x86_fc_hit, s192 raw census counting
# class-D): this gate (1) sweeps the COMPILER (`scrip --compile`), NEVER the committed .s
# artifacts (RULES.md handoff step 4: artifacts track design churn, they are not truth);
# (2) EXCLUDES CLASS D — `mov rbp, qword ptr [<non-rsp base> + N]`, rbp as a plain scratch
# GPR destination for a data-field load (s192 census: 14 across the 16 benchmarks).  A
# scratch use is not a frame reference and no frame-pointer rung can ever remove it, so a
# ratchet that counts it can never read zero (s192 headline 3, the third mis-measuring
# instrument).  Classes A (ceremony), B (genuine unwind), C (housekeeping [rbp+N] refs)
# all remain counted — they are exactly what the FLATDISP ladder exists to shrink.
# (3) RATCHETS: fail if NET > baseline; print a TIGHTEN hint when strictly below so the
# landing session lowers the baseline in the same commit (manual, never auto — 3-4
# parallel sessions share this file, RULES.md CONCURRENCY note).
#
# Baseline history: 237 (s193 — post FLATDISP-5b/5c heal + FLATDISP-6 narrowing; ALL=251,
# CLASS_D=14, watermark m3 185/130 m4 183/130 DIVERGE=1).  119 (s194 FLATDISP-7).  113 (s196
# SCANBASE).  48 (s199 DEFER-STAR: emit_graph_has_deep_arrival counted EVERY IR_MATCH_DEFER,
# but that op is built by two lowering arms the node cannot tell apart — TT_DEFER (the `*`
# unevaluated-expression operator) and TT_VAR (a bare pattern-valued variable in pattern
# position).  Only `*` can recurse: SPITBOL manual "Recursive Patterns" p.122 — the operator
# is what "makes the definition possible" and what "allows a forward reference to a pattern
# not yet defined"; a stored pattern reused by name is built EAGERLY and cannot name itself.
# Narrowed to star-sourced only via the sno_defer_is_star side table.  Watermark held exactly
# m3 221/94 m4 219/94 DIVERGE=1, FAIL sets byte-identical both modes, benchmark runtime
# signatures byte-identical pre/post.  NOT the s197 change, which dropped DEFER wholesale and
# was correctly reverted — recursive-`*` witnesses stay deep here, verified.)
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${SCRIP:-$HERE/../scrip}"
CORPUS="${CORPUS:-/home/claude/corpus}"
BENCH="$CORPUS/benchmarks/snobol4"
BASELINE="${RBP_CENSUS_MAX:-48}"
[ -x "$SCRIP" ] || { echo "SKIP scrip not built"; exit 0; }
[ -d "$BENCH" ]  || { echo "SKIP no benchmark corpus at $BENCH"; exit 0; }
ALL=0; D=0; TABLE=""
for f in "$BENCH"/*.sno; do
    s=$(mktemp); "$SCRIP" --compile "$f" > "$s" 2>/dev/null
    a=$(grep -cw rbp "$s"); dd=$(grep -Ec "mov +rbp, qword ptr \[r(ax|bx|cx|dx|si|di|8|9|1[0-5])" "$s")
    ALL=$((ALL+a)); D=$((D+dd))
    TABLE="$TABLE$(printf '%-24s all=%-4s classD=%-3s net=%s' "$(basename "$f" .sno)" "$a" "$dd" "$((a-dd))")\n"
    rm -f "$s"
done
NET=$((ALL-D))
echo "=== rbp census ratchet (compiler sweep, benchmark corpus) ==="
printf "$TABLE"
echo "ALL=$ALL CLASS_D=$D NET=$NET BASELINE=$BASELINE"
if [ "$NET" -gt "$BASELINE" ]; then echo "GATE FAIL: NET $NET > baseline $BASELINE (a frame-pointer regression, or a new rbp consumer that must be classified)"; exit 1; fi
if [ "$NET" -lt "$BASELINE" ]; then echo "GATE OK — TIGHTEN: NET $NET < baseline $BASELINE; lower BASELINE in this script in the landing commit."; exit 0; fi
echo "GATE OK: NET == baseline."
exit 0
