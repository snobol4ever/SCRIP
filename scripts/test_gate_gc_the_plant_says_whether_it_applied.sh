#!/usr/bin/env bash
# THE SHIFT PLANT SAYS WHETHER IT APPLIED, AND ITS DECLINE NEVER READS AS AN APPLICATION (cto 2026-09-21, MODE TENET,
# on the coo's flag).  SCRIP_GC_PLANT_SHIFT=N forwards every live block N bytes up at every collection: it is the
# forced-movement mechanism under three gc gates, so a run in which it silently did NOTHING is a gate grading ordinary
# compaction while reporting forced relocation (CTO-113).  The decline was made loud at SCRIP 1b317bea3 -- and the loud
# line carried BOTH errors a reader can make, in OPPOSITE directions, measured at HEAD before this gate was cut:
#   (a) the decline text told the reader to COUNT a literal that the decline text ITSELF SPELLED, so an unanchored grep
#       for it matched the decline and counted a DECLINE as an APPLICATION.  Measured: hb_coexpr_create.icn at
#       SCRIP_HEAP_MB=1 stress 3 shift 1048576 -- 0 applications, unanchored count 1.  Found by the coo, who read
#       applied=1 off a run that planted nothing.
#   (b) the applied banner was gated behind SCRIP_ZETA_TELEM/SCRIP_GC_MAPS, so a reader following that same advice with
#       no telemetry counted ZERO on EVERY run.  Measured: the same witness at shift 4096 -- 199 applications, 0 visible.
# One message, two finding-shaped lies.  The cure is in the runtime's own text and gating (gc_heap.c), and this gate
# holds it on the EMITTED BYTES rather than on the source: the applied banner prints once per process with nothing
# asked for and once per collection under telemetry, and the decline spells no countable literal.
# ⛔ THE DIRECTION OF THIS GATE'S OWN ERROR IS A REFUSAL, NEVER A FINDING (the coo's rule, 2026-09-21): both arms rest
# on a premise -- that this arena/shift pair APPLIES, or that it DECLINES -- and the premise is measured by telemetry,
# which counts the collector's own applications rather than reading prose.  A premise that does not hold exits 2 with
# the numbers, because a plant that cannot be made to decline here is a configuration statement, not a defect.
# ARMS. (1) at a shift that applies, the anchored banner is visible with NOTHING asked for, and telemetry still gives
# the per-collection form.  (2) at a shift that declines, the decline is present, the anchored count is 0, and an
# UNANCHORED grep for the counted literal is also 0 -- the decline impersonates nothing.  (3) DECLARED SET: every
# script that drives the plant as its forced-movement mechanism reads the applied banner, so none of them can grade an
# unplanted run in silence.  The set is named here and an unnamed newcomer is a FAIL, not a pass (a dispatch guard is a
# CLOSED ENUMERATION -- MODE TENET line 2).
"$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/util_require_fresh.sh" --gate "$(basename "${BASH_SOURCE[0]}" .sh)" || exit $?
set -uo pipefail
G="$(basename "${BASH_SOURCE[0]}" .sh)"
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP_BIN:-$ROOT/scrip}"; [ -x "$SCRIP" ] || { echo "⛔ GATE REFUSE(2) [$G]: no scrip at $SCRIP"; exit 2; }
export SCRIP_HEAP_MB="${SCRIP_HEAP_MB:-1}"
T=$(mktemp -d) || exit 2; trap 'rm -rf "$T"' EXIT
RC=0
WIT="$ROOT/scripts/gc_witnesses/hb_coexpr_create.icn"
[ -f "$WIT" ] || { echo "⛔ GATE REFUSE(2) [$G]: missing $WIT"; exit 2; }
APPLY_SHIFT=4096; DECLINE_SHIFT=1048576
LIT='[GC-SHIFT] plant:'
run() { local sh="$1" tel="$2" out="$3"; if [ "$tel" = tel ]; then ( cd "$T" && env SCRIP_GC_PLANT_SHIFT="$sh" SCRIP_GC_STRESS=3 SCRIP_ZETA_TELEM=1 timeout 300 "$SCRIP" "$WIT" >/dev/null 2>"$out" </dev/null ); else ( cd "$T" && env -u SCRIP_ZETA_TELEM -u SCRIP_GC_MAPS SCRIP_GC_PLANT_SHIFT="$sh" SCRIP_GC_STRESS=3 timeout 300 "$SCRIP" "$WIT" >/dev/null 2>"$out" </dev/null ); fi; }
echo "  HOLDS: the plant's applied banner is the proof it applied and prints with nothing asked for; its decline spells no literal a count would match; every gate that plants reads the banner (arena SCRIP_HEAP_MB=$SCRIP_HEAP_MB, witness $(basename "$WIT"), apply=$APPLY_SHIFT decline=$DECLINE_SHIFT)"

run "$APPLY_SHIFT" notel "$T/a_notel.txt"; run "$APPLY_SHIFT" tel "$T/a_tel.txt"
a_tel=$(grep -c "^\[GC-SHIFT\] plant:" "$T/a_tel.txt"); a_tel=${a_tel:-0}
a_bare=$(grep -c "^\[GC-SHIFT\] plant:" "$T/a_notel.txt"); a_bare=${a_bare:-0}
[ "$a_tel" -ge 1 ] || { echo "⛔ GATE REFUSE(2) [$G]: PREMISE UNMET -- at shift $APPLY_SHIFT and SCRIP_HEAP_MB=$SCRIP_HEAP_MB the plant applied $a_tel times under telemetry, so this configuration cannot test a VISIBLE application. That is a statement about the arena, not a defect; raise the arena or lower the shift"; exit 2; }
if [ "$a_bare" -ge 1 ]; then echo "  arm 1 PASS: the plant announces itself with nothing asked for -- $a_bare line(s) bare, $a_tel under SCRIP_ZETA_TELEM (once per process, then once per collection), so a reader with no telemetry can tell an applied run from an unplanted one"
else echo "  arm 1 FAIL: the plant applied $a_tel times under telemetry and printed $a_bare lines without it -- the banner is telemetry-gated again and every bare reader counts zero applications on every run (CTO-113's defect (b), gc_heap.c)"; RC=1; fi

run "$DECLINE_SHIFT" notel "$T/d_notel.txt"; run "$DECLINE_SHIFT" tel "$T/d_tel.txt"
d_tel=$(grep -c "^\[GC-SHIFT\] plant:" "$T/d_tel.txt"); d_tel=${d_tel:-0}
d_dec=$(grep -c 'plant DECLINED' "$T/d_notel.txt"); d_dec=${d_dec:-0}
[ "$d_tel" -eq 0 ] && [ "$d_dec" -ge 1 ] || { echo "⛔ GATE REFUSE(2) [$G]: PREMISE UNMET -- at shift $DECLINE_SHIFT and SCRIP_HEAP_MB=$SCRIP_HEAP_MB the plant applied $d_tel times and printed $d_dec decline line(s), so this configuration produces no decline to impersonate an application. Arena headroom exceeds the shift; lower the arena or this gate measures nothing"; exit 2; }
d_anch=$(grep -c "^\[GC-SHIFT\] plant:" "$T/d_notel.txt"); d_anch=${d_anch:-0}
d_bare=$(grep -cF "$LIT" "$T/d_notel.txt"); d_bare=${d_bare:-0}
if [ "$d_anch" -eq 0 ] && [ "$d_bare" -eq 0 ]; then echo "  arm 2 PASS: a declined collection impersonates nothing -- decline lines=$d_dec, anchored applications=0, and an UNANCHORED count of the literal is 0 too, so the reader who forgets the anchor still reads the truth"
else echo "  arm 2 FAIL: the plant applied 0 times yet a count reads anchored=$d_anch unanchored=$d_bare -- the decline text spells the literal it tells the reader to count, so a DECLINE is counted as an APPLICATION, which is exactly backwards (CTO-113's defect (a), gc_heap.c)"; RC=1; fi

want="test_gate_gc_a_coexpression_frame_image_lives_on_its_own_stack.sh test_gate_gc_pas_heap_cells_survive_forced_movement.sh test_gate_gc_the_coexpression_roots_are_typed_and_the_parked_stacks_are_segments.sh"
found=$(cd "$HERE" && grep -lE 'SCRIP_GC_PLANT_SHIFT' test_gate_*.sh 2>/dev/null | grep -v "^${G}.sh$" | sort | tr '\n' ' ' | sed 's/ $//')
blind=""
for f in $found; do grep -qE 'GC-SHIFT\\\] plant:' "$HERE/$f" || blind="$blind $f"; done
if [ "$found" = "$(printf '%s' "$want")" ] && [ -z "$blind" ]; then echo "  arm 3 PASS: exactly the declared 3 gates plant, and each reads the applied banner: $found"
else echo "  arm 3 FAIL: declared='$want' found='$found' planting-but-blind='${blind:- none}' -- a gate whose forced movement IS the plant must read the banner, or it grades an unplanted run in silence; a newcomer is declared here or it is not admitted"; RC=1; fi

if [ "$RC" = 0 ]; then echo "GATE PASS(0) [$G]: the plant says whether it applied, its decline impersonates nothing, and all three planting gates read the banner (examined 3 arms, 4 runs)"
else echo "GATE FAIL(1) [$G]: the plant's own report can be read backwards (examined 3 arms)"; fi
echo "    tree: SCRIP=$(git -C "$ROOT" rev-parse --short HEAD 2>/dev/null)$(git -C "$ROOT" diff --quiet 2>/dev/null || echo -DIRTY)  measured $(date -u +%Y-%m-%dT%H:%MZ)"
exit $RC
