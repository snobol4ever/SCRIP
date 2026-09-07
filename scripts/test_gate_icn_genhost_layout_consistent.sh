#!/usr/bin/env bash
# test_gate_icn_genhost_layout_consistent.sh — THE GENERATOR-HOST REGION LAYOUT IS INTERNALLY CONSISTENT.
#
# ⛔ WHAT IT MEASURES, AND WHY IT IS NOT A SIZE CHECK.  A generator callee's region is carved by its CALLER,
# sized icn_gen_host_slice() = align16(own frame) + 48 + the caller's transitive walk over the callee's own
# generator call sites.  The callee, compiled as a host in its own right, lays out its callee area with
# icn_gen_host_reserve() — the SAME walk seeded differently: the caller's `visited` set already carries the
# caller's ancestors, so the caller's walk cuts a cycle EARLY and hands over a small container, while the
# callee's own walk cuts it late and lays out a large one.  Container and contents are then sized by two
# different rules and the callee's generator call sites address storage past the end of its region.
# ⛔ THAT IS A SILENT WILD WRITE, NOT A STACK OVERFLOW.  Nothing goes red: the program compiles, links, runs,
# and corrupts whatever lies above its region.  It becomes visible only when the recursion goes deep enough
# to reach the overrun, which is why three of the four programs this gate names PASS against the icont oracle
# today (hq_B census 2026-09-06, all 46 .icn under corpus/demos/icon + corpus/benchmarks/icon: geddump 1 site,
# icon_parser 13, icon_recognizer 23, jtran 80; 39 clean, 3 do not compile).
# ⭐ A SIZE-KEYED CAP CANNOT SEE THIS AND THAT IS THE POINT.  geddump carves 65,544 bytes — the same number as
# every clean program in the corpus — and is still laying out an inconsistent region.  The bimodal carve
# census (65,544 · 20 MB · 33 MB · 66 MB, nothing between) that withdrew the size-keyed refusal is blind to it.
# ⛔ REPORTED-NOT-BLOCKING, ON PURPOSE, AS A RAMP (the `-` prefix in the Makefile is what makes that true; read
# the recipe, not this comment).  Arming it today would red three programs that are green against the oracle,
# which is the ramp condition RULES.md § THE PRISTINE BUILD names for gates that would stop the very landings
# that would clear them.  It genuinely refuses: GENHOST_LAYOUT_GATE_BLOCKING=1 makes a non-zero census exit 1,
# and SCRIP_ICN_GENHOST_LAYOUT_STRICT=1 makes the COMPILER itself refuse rc=2 at the offending host.
# ⛔ AND IT REFUSES rc=2 WHEN IT CANNOT MEASURE — never skip-as-success: a census that cannot see its
# population must never print 0 (RULES.md § a test that cannot measure REFUSES).
# ✅ THE CURE THAT CLEARS IT: per-activation, per-path carving on the stack (ceo-372, on Lon's 2026-09-06
# "No activations on the heap").  When the sizing is per-activation there is no early/late cycle cut to
# disagree about, and this gate reads 0 — which is the arm that proves the cure, not a number beside it.
# AUTHORS: Lon Jones Cherryholmes · Jeffrey Cooper M.D. · Claude Opus  DATE: 2026-09-06
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "${HERE}/.." && pwd)"
CORPUS="$(cd "${ROOT}/../corpus" 2>/dev/null && pwd)"
SCRIP="${ROOT}/scrip"
RATCHET="${GENHOST_LAYOUT_RATCHET:-4}"

[ -x "${SCRIP}" ] || { echo "GATE REFUSE(2): ${SCRIP} is not built -- cannot measure."; exit 2; }
[ -n "${CORPUS}" ] && [ -d "${CORPUS}/demos/icon" ] || { echo "GATE REFUSE(2): corpus/demos/icon not found beside this checkout -- cannot measure."; exit 2; }
POP=$(find "${CORPUS}/demos/icon" "${CORPUS}/benchmarks/icon" -name '*.icn' 2>/dev/null | sort)
[ -n "${POP}" ] || { echo "GATE REFUSE(2): zero .icn programs under corpus/{demos,benchmarks}/icon -- a census that cannot see its population must never print 0."; exit 2; }

echo "=== GENERATOR-HOST REGION LAYOUT CONSISTENCY gate ==="
TMP=$(mktemp -d) || { echo "GATE REFUSE(2): no scratch dir."; exit 2; }
trap 'rm -rf "${TMP}"' EXIT
total=0; nocompile=0; dirty=0; sites=0
while IFS= read -r f; do
    [ -n "$f" ] || continue
    total=$((total + 1))
    if ! timeout 300 "${SCRIP}" --compile -o "${TMP}/a.s" "$f" </dev/null 2>"${TMP}/e" ; then nocompile=$((nocompile + 1)); continue; fi
    n=$(grep -c 'GENHOST-LAYOUT' "${TMP}/e" 2>/dev/null || true)
    [ "${n}" -gt 0 ] || continue
    dirty=$((dirty + 1)); sites=$((sites + n))
    printf '  ⛔ %-24s %3d call site(s) whose region is smaller than the callee lays out\n' "$(basename "$f")" "${n}"
    grep 'GENHOST-LAYOUT' "${TMP}/e" | sed 's/: the slice.*//; s/^/      /' | sort -u | head -4
done <<< "${POP}"
[ "${total}" -gt 0 ] || { echo "GATE REFUSE(2): censused zero programs -- cannot measure."; exit 2; }
echo "  censused=${total}  nocompile=${nocompile}  INCONSISTENT=${dirty} programs / ${sites} call sites  ratchet=${RATCHET}"
if [ "${dirty}" -gt "${RATCHET}" ]; then
    echo "GATE FAIL: ${dirty} programs lay out an inconsistent generator-host region, over the ratchet of ${RATCHET}. ⛔ Lower the ratchet ONLY in the landing commit that cures the sizing -- never auto, never to make a board green."
    exit 1
fi
if [ "${dirty}" -gt 0 ]; then
    echo "GATE REPORTED (not blocking): ${dirty}/${total} programs carry an inconsistent generator-host region layout -- a SILENT WILD WRITE that no board goes red on. Row: icon-generator-host-carve-sums-the-whole-component-and-reserves-66mb-on-jtran-main (hq_B, rank 0). Cure: per-activation, per-path carving on the stack (ceo-372)."
    [ "${GENHOST_LAYOUT_GATE_BLOCKING:-0}" = "1" ] && { echo "GATE FAIL (GENHOST_LAYOUT_GATE_BLOCKING=1): the census is non-zero and this run asked for it to block."; exit 1; }
    exit 0
fi
echo "GATE PASS: every censused Icon program lays out a generator-host region consistent with what its callees lay out for themselves."
exit 0
