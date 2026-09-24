#!/usr/bin/env bash
# test_gate_dyn_caps_ratchet.sh -- THE FIXED-CAPS RATCHET: the population of fixed tables in src/ may only fall, and a fall lowers the
# baseline in the same landing (Lon 2026-09-23 14:5x: no fixed limit a program can reach; .github/ARCH-DYNAMIC-STORAGE.md section 5;
# row instruments-the-fixed-caps-ratchet-is-a-blocking-arm-and-every-remaining-cap-refuses-loudly, the coo).
#
# THE POPULATION is audit_fixed_caps_census.py's file-, static- and field-scope declarations with a constant bound, counted by
# util_dyn_caps_witness.sh census against scripts/fixtures/dyn_caps/BASELINE. ⛔ THE CENSUS WAS CORRECTED BEFORE THIS GATE COULD LAND,
# BECAUSE IT WAS RED FOR A REASON THAT WAS NOT A NEW TABLE: it counted an `extern` re-declaration as a table (13 of them, among
# them the file-scope `extern jmp_buf g_core_errjmp_stk[64];` that read "361 -> 362, a new fixed limit landed"), and it could not
# see a table named after a closing brace, `static struct { fields } name[CAP];` (30 of them, the cfo's finding: five population
# caps in lower_snobol4.c among them). The corrected census reads 379 on the baseline's own tree (ac237a0e7) and 379 today, the
# same tables by name -- so BASELINE was re-derived 361 -> 379 like-for-like: one tree, the corrected instrument, nothing hidden.
# ARMS:
#   1 the census's own selftest: every declared form is counted once or refused as not-storage (typedef, extern, a comparison), and
#     each guard shape (skip, loud abort, clamp, truncating loop, none, const, a literal guarded at N-1, a refusing wrapper) is read
#     as its class, all by NAME
#   2 the real tree reads EXACTLY the baseline: above it a new fixed table landed (make it dynamic, or declare it class A/B on the
#     page); below it a table was converted and BASELINE is lowered to the new count IN THE SAME LANDING, so the ratchet keeps it
#   3 FAIL-ONCE BUILT IN: a scratch copy of src/ with TWO planted file-scope tables reads RED, grown by exactly two from the UNPLANTED
#     count -- ⛔ judged against the tree's own count, not against BASELINE+1: when hq_icon's two constant tables put origin at 366 over
#     364, the old arm expected "364 -> 365", read 367, and one cause redded two arms (the cfo's finding, 2026-09-24)
#   4 THE GUARD CLASSIFIER CAN RED (CEO-1231): of the two planted tables one is never compared and one drops at its cap, and the
#     census's no-guard count and its drop count must each rise by exactly one -- a classifier that read every table the same could
#     not pass both
# The no-guard and drop counts are PRINTED, not graded here: bringing each to zero is the row's criterion (CEO-1231), not this gate's.
# ⛔ BASELINE 363 -> 355, RE-DERIVED LIKE-FOR-LIKE 2026-09-24 (the coo): the census counted 8 COMPARISONS as file-scope tables -- the
# one-line getenv switches `return (e && e[0] == '0');`, read as type `e`, separator `&&`, name `e`, initializer `== '0'` -- and 87
# such phantoms in all scopes. On one tree (c2cdd7480) the corrected census drops exactly those 87 rows, every one a NAME[0]
# expression, and adds none; re-measured after rebasing onto 3680a1d67 (the WASM tables gone, origin at 363), the old census reads 363
# and the corrected one 355, the difference exactly the 8 phantoms by name; the 355 are the same tables by name.
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
. "$HERE/lib_gate.sh" || { echo "REFUSING(2): cannot load lib_gate.sh"; exit 2; }
GATE_NAME=dyn_caps_ratchet
GATE_STRICT=1
gate_parse_args "$@"
C="$HERE/audit_fixed_caps_census.py"; WIT="$HERE/util_dyn_caps_witness.sh"; B="$HERE/fixtures/dyn_caps/BASELINE"
gate_require "$C" "audit_fixed_caps_census.py" || exit 2
gate_require "$WIT" "util_dyn_caps_witness.sh" || exit 2
gate_require "$B" "scripts/fixtures/dyn_caps/BASELINE" || exit 2
SCRATCH="${S4E_SCRATCH:-$(cd "$ROOT/.." && pwd)/.scratch}"
mkdir -p "$SCRATCH" || { echo "REFUSING(2) [$GATE_NAME]: cannot create $SCRATCH"; exit 2; }
WORK=$(mktemp -d "$SCRATCH/gate_dyn_caps_XXXXXX") || exit 2
trap '[ -n "${WORK:-}" ] && rm -rf "$WORK"' EXIT INT TERM
fails=0
ck() { if eval "$2"; then echo "  ok   $1"; else fails=$((fails+1)); echo "  FAIL $1"; fi; }
st=$(python3 "$C" --selftest 2>&1); st_rc=$?
ck "1 the census selftest: every declared form counted once, a typedef, an extern and a comparison refused, each guard shape read as its class ($(grep -c '^SELFTEST: ' <<<"$st") checks)" '[ "$st_rc" = 0 ] && grep -q "^SELFTEST PASS" <<<"$st"'
[ "$st_rc" = 0 ] || printf '%s\n' "$st" | grep FAIL | sed 's/^/      /'
out=$(cd "$ROOT" && bash "$WIT" census 2>&1); line=$(grep -m1 '^fixed-bound declarations' <<<"$out")
[ -n "$line" ] || { echo "GATE UNPROVEN(2) [$GATE_NAME]: the witness printed no census line -- $(printf '%s\n' "$out" | head -2)"; gate_stamp; exit 2; }
n=$(sed -n 's/.*scope: \([0-9]*\) (baseline \([0-9]*\)).*/\1/p' <<<"$line"); b=$(sed -n 's/.*scope: \([0-9]*\) (baseline \([0-9]*\)).*/\2/p' <<<"$line")
u=$(sed -n 's/.*never compared in its file: \([0-9]*\)$/\1/p' <<<"$line")
echo "  census: $n fixed-bound declarations at file, static or field scope, baseline $b; $u with no capacity guard at a fill, macro or literal bound (never compared, or compared only as an index or an iteration) -- CEO-1231, printed, not graded here"
ck "2 the tree reads EXACTLY the baseline ($n vs $b)" '[ -n "$n" ] && [ "$n" = "$b" ]'
[ -n "$n" ] && [ "$n" -gt "$b" ] && echo "      a new fixed table landed ($b -> $n): make it dynamic, or declare it class A/B on the page -- list them: python3 scripts/audit_fixed_caps_census.py --tsv FILE"
[ -n "$n" ] && [ "$n" -lt "$b" ] && echo "      the population FELL ($b -> $n): lower scripts/fixtures/dyn_caps/BASELINE to $n in this landing, so the ratchet keeps the gain"
d=$(sed -n 's/^guards that drop or truncate at the cap: \([0-9]*\)$/\1/p' <<<"$out")
[ -n "$d" ] || { echo "GATE UNPROVEN(2) [$GATE_NAME]: the witness printed no drop line -- $(printf '%s\n' "$out" | head -3 | tr '\n' ' ')"; gate_stamp; exit 2; }
echo "  guards that drop or truncate at the cap: $d (CEO-1231; printed, not graded here)"
mkdir -p "$WORK/r/scripts/fixtures/dyn_caps" && cp -r "$ROOT/src" "$WORK/r/src" && cp "$C" "$WIT" "$WORK/r/scripts/" \
  && printf '%s\n' "$n" > "$WORK/r/scripts/fixtures/dyn_caps/BASELINE" || { echo "REFUSING(2) [$GATE_NAME]: cannot stage the scratch tree"; exit 2; }
cat > "$WORK/r/src/planted_by_the_ratchet_gate.c" <<'PLANT'
#define PLANTED_BY_THE_RATCHET_GATE_MAX 8
static int g_planted_by_the_ratchet_gate[PLANTED_BY_THE_RATCHET_GATE_MAX];
#define PLANTED_DROP_BY_THE_RATCHET_GATE_MAX 8
static int g_planted_drop_by_the_ratchet_gate[PLANTED_DROP_BY_THE_RATCHET_GATE_MAX];
static int g_planted_drop_by_the_ratchet_gate_n;
void planted_drop_by_the_ratchet_gate(int v) { if (g_planted_drop_by_the_ratchet_gate_n < PLANTED_DROP_BY_THE_RATCHET_GATE_MAX) g_planted_drop_by_the_ratchet_gate[g_planted_drop_by_the_ratchet_gate_n++] = v; }
PLANT
po=$(cd "$WORK/r" && bash scripts/util_dyn_caps_witness.sh census 2>&1); prc=$?
pu=$(sed -n 's/.*never compared in its file: \([0-9]*\)$/\1/p' <<<"$po"); pd=$(sed -n 's/^guards that drop or truncate at the cap: \([0-9]*\)$/\1/p' <<<"$po")
ck "3 FAIL-ONCE: two planted file-scope tables read RED, grown from the unplanted count ($n -> $((n + 2)))" '[ "$prc" != 0 ] && grep -q "grew from $n to $((n + 2))" <<<"$po"'
ck "4 the guard classifier can red: the never-compared plant raises the no-guard count by one ($u -> ${pu:-?}) and the dropping plant the drop count by one ($d -> ${pd:-?})" '[ "$pu" = "$((u + 1))" ] && [ "$pd" = "$((d + 1))" ]'
if [ "$fails" = 0 ]; then echo "GATE PASS(0) [$GATE_NAME]: 4 arms -- $n fixed tables, exactly the baseline; a planted one reds; the classifier tells a drop from no guard"; gate_stamp; exit 0; fi
echo "GATE FAIL(1) [$GATE_NAME]: $fails of 4 arms red"; gate_stamp; exit 1
