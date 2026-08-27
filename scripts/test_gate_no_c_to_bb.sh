#!/usr/bin/env bash
# test_gate_no_c_to_bb.sh — NCB-0 gate: enumerate every C→BB transfer outside the sanctioned MAIN sites.
# CONVENTION (GOAL-SNOBOL4-BB.md Phase 1 / ARCH-ZETA-LOCAL-STORAGE.md §7c, Lon s22): mode 3 has exactly ONE
# C→BB transfer — the driver MAIN branch; C runtime helpers are STRICT LEAVES (BB calls C, C returns, C never
# calls/jmps a BB); mode 4 entry is main = the emitted graph. SANCTIONED SITES (excluded below):
#   src/driver/scrip.c        function main            — TWO mutually-exclusive MAIN branches per run
#                                                        (~:1327 general mode-3 entry; ~:1407 SNOBOL4 pat_flat
#                                                        whole-program entry; both `goto run_done`) + the
#                                                        m3_enter_with_rbx asm thunk they route through.
#                                                        [LEDGER NOTE: s22 text said "the driver MAIN branch"
#                                                        singular; ground truth is two exclusive branches of
#                                                        the one MAIN dispatch — one transfer per run either way.]
#   src/runtime/rt/rt_coexpr.c scrip_coexpr_trampoline_entry — sanctioned per-thread MAIN (jmp not call,
#                                                        fresh pthread stack, never returns).
# DETECTED SHAPES: (a) member fn-ptr α calls `->fn(fb, 0)` / `->fn((void *)fb, 0)`; (b) bare bb_box_fn β
# resumes `fn((void *)fb, 1)`; (c) inline-asm `call *%rax` into the slab (the EVAL shim); (d) bare MAIN-shape
# `fn(rt_frame(), 0)`; (e) RIDERS — C (non-emitted, non-decl) callers of the V4/V5 trampoline API outside rt.c.
# GROUPING: hits fold into the s22 ledger groups V1..V7 by enclosing function; any hit whose enclosing
# function is unknown lands in UNLEDGERED and is ALWAYS a failure signal worth reading.
# EXIT: default REPORT-ONLY, exit 0 (non-blocking at birth per NCB-0; reports 7). --strict: exit 1 if any
# group remains (flips into Session Setup at NCB-4). Expected count-down: NCB-1 lands → 4, NCB-2 → 1, NCB-3 → 0.
# grep -a THROUGHOUT: several sources (core.c) contain UTF-8 Greek identifiers (Σ, δ) and read as binary.
# AUTHORS: Lon Jones Cherryholmes · Jeffrey Cooper M.D. · Claude   DATE: 2026-07-11 (NCB-0)
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; REPO="${HERE}/.."; cd "$REPO" || exit 2
STRICT=1; [ "${1:-}" = "--informational" ] && STRICT=0   # V2-5: strict by default; --strict kept as a no-op alias
[ "${1:-}" = "--strict" ] && STRICT=1
[ "$STRICT" = "0" ] && echo "⛔ --informational: verdict NOT enforced."
# ⭐ V2-5 COVERAGE FLOOR (gate honesty): examining NOTHING must never read the same as examining everything.
. "$(dirname "$0")/lib_gate.sh"
gate_floor "$(ls "$(dirname "$0")"/../src/emitter/*.c "$(dirname "$0")"/../src/emitter/*.cpp "$(dirname "$0")"/../src/templates/{bb,xa}/*.cpp "$(dirname "$0")"/../src/runtime/*.c 2>/dev/null | wc -l)" 100 "source files under src/ -- an empty tree is UNPROVEN(2), not a pass"


SCAN_DIRS="src/runtime src/driver src/machine src/lower src/frontend src/contracts src/optimizer"

enclosing_fn() { # file line -> name of enclosing top-level function
    awk -v n="$2" 'NR<=n && /^[A-Za-z_][A-Za-z0-9_ \*]*\(/ && !/;[ \t]*$/ { f=$0 } END {
        gsub(/\(.*/,"",f); nf=split(f,a,/[ \*]+/); print (nf?a[nf]:"?") }' "$1"
}

RAW=$(mktemp); TMP=$(mktemp); STRIP=$(mktemp -d); trap 'rm -rf "$RAW" "$TMP" "$STRIP"' EXIT
# COMMENT-STRIPPED MIRROR (NCB-1b finding): the shapes below are CODE shapes, but a comment that QUOTES one is
# not a transfer.  s24's own leaf-split comment ("call p->fn(fb, 0)") tripped shape (a) and reported a phantom
# 8th group (UNLEDGERED @ rt.c:716) — a gate that reds on prose would have held NCB-4's --strict red forever.
# util_strip_c_comments.awk blanks comments LINE-PRESERVINGLY (so reported line numbers still index the real
# file) and leaves STRING LITERALS intact (shape (c), the EVAL asm shim, lives inside one).
for f in $(find $SCAN_DIRS -name '*.c' 2>/dev/null); do
    mkdir -p "$STRIP/$(dirname "$f")"; LC_ALL=C awk -f "$HERE/util_strip_c_comments.awk" "$f" > "$STRIP/$f"
done
cd "$STRIP" || exit 2
# (a+b) ANY call through a bb_box_fn pointer — the (fb, entry) signature — whether reached through a struct
# member (`p->fn(fb, 0)`) or a bare local fn-ptr (`fn((void *)fb, 0)`), at EITHER port (entry 0 = α, 1 = β).
# NCB-1b finding: the old pair of shapes keyed on member+entry-0 and bare+entry-1, so a BARE fn-ptr at entry 0
# matched neither — and that is exactly what NCB-1a's leaf split turned rt_call_proc_descr's transfer into
# (rt.c:532, `fn((void *)fb, 0)` off rt_frame_prep).  The gate was BLIND to the very transfer NCB-1b removes:
# the flip would have read 7→7 and proven nothing.  One shape now covers the whole (fb, entry) call space.
grep -rna --include='*.c' -E -e '\bfn\(\(?(void \*\))?fb, *[01]\)' $SCAN_DIRS 2>/dev/null \
    | sed -E 's/^([^:]+):([0-9]+):(.*)$/\1:\2:/' >> "$RAW.tmp1"
grep -rna --include='*.c' -E -e '\bfn\(\(?(void \*\))?fb, *[01]\)' $SCAN_DIRS 2>/dev/null \
    | awk -F: '{ line=$0; sub(/^[^:]+:[0-9]+:/,"",line); tag = (line ~ /->fn\(/) ? "member-fnptr" : "bare-fnptr"; print $1 ":" $2 ":" tag }' >> "$RAW"
rm -f "$RAW.tmp1"
# (c) inline-asm transfer into the slab
grep -rna --include='*.c' -E -e 'call \*%r?ax|call \*%%r?ax|jmp \*%%?rax' $SCAN_DIRS 2>/dev/null | cut -d: -f1,2 | sed 's/$/:asm-shim/' >> "$RAW"
# (d) bare MAIN-shape entries
grep -rna --include='*.c' -E -e 'fn\(rt_frame\(\), 0\)|m3_enter_with_rbx\(' $SCAN_DIRS 2>/dev/null | grep -av 'static void m3_enter_with_rbx' | cut -d: -f1,2 | sed 's/$/:main-shape/' >> "$RAW"
# (e) riders: C callers of the V4/V5 trampoline API outside rt.c (exclude decls)
grep -rna --include='*.c' -E -e 'rt_proc_call_gen_h\(|rt_proc_resume_frame_h\(' $SCAN_DIRS 2>/dev/null \
    | grep -av 'src/runtime/rt/rt\.c' | grep -avE ':[ \t]*(extern|DESCR_t) ' | cut -d: -f1,2 | sed 's/$/:rider/' >> "$RAW"
cd "$REPO" || exit 2

while IFS=: read -r f l shape; do
    [ -z "$f" ] && continue
    fn=$(enclosing_fn "$f" "$l")
    case "$f:$fn" in
        src/driver/scrip.c:main|src/driver/scrip.c:m3_enter_with_rbx) continue ;;
        src/runtime/rt/rt_coexpr.c:scrip_coexpr_trampoline_entry) continue ;;
    esac
    echo "$f:$l:$fn:$shape"
done < "$RAW" | sort -t: -k1,1 -k2,2n -u > "$TMP"

ledger_of() { # enclosing function -> ledger group
    case "$1" in
        rt_call_named_proc)                       echo "V1" ;;
        rt_call_proc_direct)                      echo "V2" ;;
        rt_call_named_proc_sl)                    echo "V3" ;;
        rt_call_proc_descr|rt_proc_call_gen|rt_proc_call_gen_h) echo "V4" ;;
        rt_proc_resume_frame|rt_proc_resume_frame_h)            echo "V5" ;;
        rt_eval_run|__asm__|rt_eval_enter_shim)   echo "V6" ;;
        rt_call_value_gen_h|rt_call_value_resume_h|plc_next)    echo "V7" ;;
        *)                                        echo "UNLEDGERED" ;;
    esac
}

echo "=== NCB GATE — C→BB transfers outside the sanctioned MAIN sites ==="
echo "    sanctioned: scrip.c:main (2 exclusive MAIN branches + m3_enter_with_rbx) · rt_coexpr.c:scrip_coexpr_trampoline_entry"
VGRPS=""
UNLEDGERED=0
for V in V1 V2 V3 V4 V5 V6 V7 UNLEDGERED; do
    VLNS=""
    while IFS=: read -r f l fn shape; do
        [ "$(ledger_of "$fn")" = "$V" ] && VLNS="${VLNS}    ${f}:${l}  (${fn}, ${shape})"$'\n'
    done < "$TMP"
    if [ -n "$VLNS" ]; then
        VGRPS="${VGRPS}${V} "
        echo "${V}:"
        printf '%s' "$VLNS"
        [ "$V" = "UNLEDGERED" ] && UNLEDGERED=1
    fi
done
N=$(echo $VGRPS | wc -w)
echo "LEDGER GROUPS REMAINING: $N   (NCB-1 → 4, NCB-2 → 1, NCB-3 → 0)"
[ "$UNLEDGERED" = "1" ] && echo "⚠ UNLEDGERED C→BB transfer present — a NEW violation of the one-entry convention; read it before anything else."
if [ "$STRICT" = "1" ] && [ "$N" -gt 0 ]; then echo "STRICT: FAIL (convention not yet sealed)"; exit 1; fi
exit 0
