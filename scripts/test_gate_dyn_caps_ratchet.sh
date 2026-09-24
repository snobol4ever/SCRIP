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
#   5 THE SECOND POPULATION (CEO-1231 (1), stage 2): the locals a program's data fills -- the census's fill column -- read EXACTLY
#     scripts/fixtures/dyn_caps/BASELINE_FUNCTION_SCOPE, with the same rule: above it a new fixed local landed, below it a conversion
#     lowers the baseline in the same landing
#   6 FAIL-ONCE BUILT IN FOR IT: two planted locals, one filled by a list walk with no guard and one the digits of a number (class B by
#     construction, a radix loop), raise the function-scope population by exactly ONE -- the radix buffer stays out -- and the
#     unguarded count by one, and the census names the growth RED
#   7 THE CLASS A/B FIXTURE (ceo CEO-1234 (1)): scripts/fixtures/dyn_caps/CLASS_AB.tsv names the tables that stay fixed, each with the
#     measurement that earned its class; the scratch copy declares a third planted table (which must leave the no-guard line: arm 4's
#     +1 holds with two never-compared plants) and one table the tree does not declare, which must read RED by name
#   8 THE WITNESS VERB (CEO-1231: the refusal read at cap+1) CAN RED: util_dyn_caps_witness.sh witness on a scratch WITNESSES.tsv reads
#     the Prolog trail's refusal at 1200000 conditional bindings, counts a program that never reaches g_capo's cap as SILENT, names a
#     row for no LOUD guard STALE, and exits 1 -- the verb itself is the row's DONE-WHEN, graded there, not here
# The no-guard, drop and function-scope unguarded counts are PRINTED, not graded here: bringing each to zero is the row's criterion
# (CEO-1231), not this gate's.
# ⛔ BASELINE 352 -> 366, RE-DERIVED LIKE-FOR-LIKE 2026-09-24 (the coo, stage 2): the census scoped a declaration by the brace depth at
# the START of its line, counted in RAW text. A '{' char literal or a "{}" string drifted the depth (re.c, unification.c, emit_str.cpp,
# by_name_dispatch.c ended off 0), emit.cpp's `extern "C" {` made every later global a local, PL_CX_LEAF_HEAD/TAIL (a function body
# opened in one macro and closed in another) drifted by_name_dispatch.c, and a local on its function's opening line read as FILE scope
# -- which is why the ratchet read 353 against BASELINE 352 on origin 4c88f60c9: 411f4dbc6 wrote `char nmb[24]` inside the one-line
# sno_scan_tmp (lower_snobol4.c:140), a local, and HEAD's census counted it as a new file-scope table. On that one tree the corrected
# census moves exactly 9 locals OUT (that nmb[24], emit_str.cpp b, lower_snobol4.c e[4096], by_name_dispatch.c el[4096] b[64] qn[256],
# core.c eb, icn_extfn.c b, rt_coexpr.c w) and 22 file-scope tables and fields IN that the drift read as locals (emit.cpp
# g_flat_data_buf and bb_patch_list after the extern "C", by_name_dispatch.c g_bidprof after the macro drift, 19 one-line or drifted
# struct fields), 353 - 9 + 22 = 366, and drops one phantom declaration inside a string (frame_layout.c "frames from [1]").
# BASELINE_FUNCTION_SCOPE 506 is the new population's first reading on that tree.
# ⛔ BASELINE 366 -> 384 AND BASELINE_FUNCTION_SCOPE 506 -> 528, RE-DERIVED LIKE-FOR-LIKE 2026-09-24 (the coo): ARRAY needs a type word
# before each name, so every array after the first declarator of one statement was invisible -- `static long g_gc_rep_ranges[..],
# g_gc_rep_bytes[..], ...;` counted one table of six, and `static int g_zd_stage, ..., g_zd_read[ZD_NOPS_MAX], g_zd_kind[..];` (a
# scalar first) counted none; and a `static const struct { const char *nm; ... } g_bid_tab[..]` read as writable because of its first
# field's `*`. On one tree (096684141) the corrected census adds exactly 18 file-scope tables and fields (15 comma declarators, 3
# after a scalar: emit.cpp g_zd_read g_zd_kind, frame_layout.c zop_hist) and 22 locals a program fills, removes none, and reads
# g_bid_tab const: 366 + 18 = 384, 506 + 22 = 528.
# ⛔ BASELINE 384 -> 392, RE-DERIVED LIKE-FOR-LIKE 2026-09-24 (the coo, stage 2 (b)): a bound written as a shift was unreadable (rt.c
# g_lvl_own[1 << 16]; emit_per_kind_audit.c bin_buf[16 * 1024] as a product), and FIXED-SIZE ARENAS -- storage a program fills that no
# declaration names, a constant 64 KiB or more from an allocator (CEO-1231 (1): arenas sized by a shift constant join the class) -- were
# not counted: the Prolog trail PL_TR_ARENA_BYTES (LOUD, rt_pl_trail.h:30), the CAS island (LOUD), the GVA island (LOUD), bb_pool's
# BB_POOL_SIZE (DROP: bb_alloc returns NULL past pool_limit), the DCAP island RT_DCAP_ISLAND_BYTES (NONE: its bump and bound live in
# the emitted asm, no C comparison) and the stack-overflow sigaltstack (declared class A in CLASS_AB.tsv). On one tree (c15438d71):
# 384 + 2 + 6 = 392; no-guard 256 -> 259 (g_lvl_own, bin_buf, the DCAP island), drop 88 -> 89 (BB_POOL_SIZE).
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
C="$HERE/audit_fixed_caps_census.py"; WIT="$HERE/util_dyn_caps_witness.sh"; B="$HERE/fixtures/dyn_caps/BASELINE"; BF="$HERE/fixtures/dyn_caps/BASELINE_FUNCTION_SCOPE"
gate_require "$C" "audit_fixed_caps_census.py" || exit 2
gate_require "$WIT" "util_dyn_caps_witness.sh" || exit 2
gate_require "$B" "scripts/fixtures/dyn_caps/BASELINE" || exit 2
gate_require "$BF" "scripts/fixtures/dyn_caps/BASELINE_FUNCTION_SCOPE" || exit 2
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
n=$(sed -n 's/^fixed-bound declarations[^:]*: \([0-9]*\) (baseline \([0-9]*\)).*/\1/p' <<<"$line"); b=$(sed -n 's/^fixed-bound declarations[^:]*: \([0-9]*\) (baseline \([0-9]*\)).*/\2/p' <<<"$line")
u=$(sed -n 's/.*never compared in its file: \([0-9]*\)$/\1/p' <<<"$line")
echo "  census: $n fixed-bound declarations at file, static or field scope, baseline $b; $u with no capacity guard at a fill, macro or literal bound (never compared, or compared only as an index or an iteration) -- CEO-1231, printed, not graded here"
ck "2 the tree reads EXACTLY the baseline ($n vs $b)" '[ -n "$n" ] && [ "$n" = "$b" ]'
[ -n "$n" ] && [ "$n" -gt "$b" ] && echo "      a new fixed table landed ($b -> $n): make it dynamic, or declare it class A/B on the page -- list them: python3 scripts/audit_fixed_caps_census.py --tsv FILE"
[ -n "$n" ] && [ "$n" -lt "$b" ] && echo "      the population FELL ($b -> $n): lower scripts/fixtures/dyn_caps/BASELINE to $n in this landing, so the ratchet keeps the gain"
d=$(sed -n 's/^guards that drop or truncate at the cap: \([0-9]*\)$/\1/p' <<<"$out")
[ -n "$d" ] || { echo "GATE UNPROVEN(2) [$GATE_NAME]: the witness printed no drop line -- $(printf '%s\n' "$out" | head -3 | tr '\n' ' ')"; gate_stamp; exit 2; }
echo "  guards that drop or truncate at the cap: $d (CEO-1231; printed, not graded here)"
fline=$(grep -m1 '^function-scope arrays a program fills: ' <<<"$out")
fn=$(sed -n 's/^function-scope arrays a program fills: \([0-9]*\) (baseline \([0-9]*\)).*/\1/p' <<<"$fline"); fb=$(sed -n 's/^function-scope arrays a program fills: \([0-9]*\) (baseline \([0-9]*\)).*/\2/p' <<<"$fline")
fu=$(sed -n 's/^function-scope arrays a program fills, unguarded: \([0-9]*\)$/\1/p' <<<"$out")
[ -n "$fn" ] && [ -n "$fb" ] && [ -n "$fu" ] || { echo "GATE UNPROVEN(2) [$GATE_NAME]: the witness printed no function-scope lines -- $(printf '%s\n' "$out" | tail -3 | tr '\n' ' ')"; gate_stamp; exit 2; }
echo "  function-scope population: $fn locals a program fills, baseline $fb; $fu of them unguarded (CEO-1231; printed, not graded here)"
mkdir -p "$WORK/r/scripts/fixtures/dyn_caps" && cp -rL "$ROOT/src" "$WORK/r/src" && cp "$C" "$WIT" "$WORK/r/scripts/" \
  && printf '%s\n' "$n" > "$WORK/r/scripts/fixtures/dyn_caps/BASELINE" && printf '%s\n' "$fn" > "$WORK/r/scripts/fixtures/dyn_caps/BASELINE_FUNCTION_SCOPE" \
  && cp "$HERE/fixtures/dyn_caps/CLASS_AB.tsv" "$WORK/r/scripts/fixtures/dyn_caps/CLASS_AB.tsv" \
  && printf 'src/planted_by_the_ratchet_gate.c\tg_planted_classab_by_the_ratchet_gate\tA\tthe gate plants it never compared and declares it\nsrc/planted_by_the_ratchet_gate.c\tg_no_such_table_by_the_ratchet_gate\tA\ta stale row the gate plants\n' >> "$WORK/r/scripts/fixtures/dyn_caps/CLASS_AB.tsv" \
  || { echo "REFUSING(2) [$GATE_NAME]: cannot stage the scratch tree"; exit 2; }
cat > "$WORK/r/src/planted_by_the_ratchet_gate.c" <<'PLANT'
#define PLANTED_BY_THE_RATCHET_GATE_MAX 8
static int g_planted_by_the_ratchet_gate[PLANTED_BY_THE_RATCHET_GATE_MAX];
#define PLANTED_DROP_BY_THE_RATCHET_GATE_MAX 8
static int g_planted_drop_by_the_ratchet_gate[PLANTED_DROP_BY_THE_RATCHET_GATE_MAX];
static int g_planted_drop_by_the_ratchet_gate_n;
static int g_planted_classab_by_the_ratchet_gate[8];
void planted_drop_by_the_ratchet_gate(int v) { if (g_planted_drop_by_the_ratchet_gate_n < PLANTED_DROP_BY_THE_RATCHET_GATE_MAX) g_planted_drop_by_the_ratchet_gate[g_planted_drop_by_the_ratchet_gate_n++] = v; }
struct planted_node_by_the_ratchet_gate { struct planted_node_by_the_ratchet_gate *next; int v; };
void planted_local_by_the_ratchet_gate(struct planted_node_by_the_ratchet_gate *h) {
    int planted_local_by_the_ratchet_gate[16]; int n = 0;
    for (; h; h = h->next) planted_local_by_the_ratchet_gate[n++] = h->v;
}
void planted_radix_by_the_ratchet_gate(unsigned long u) {
    char planted_radix_by_the_ratchet_gate[24]; int n = 0;
    do { planted_radix_by_the_ratchet_gate[n++] = (char)('0' + u % 10); u /= 10; } while (u);
}
PLANT
po=$(cd "$WORK/r" && bash scripts/util_dyn_caps_witness.sh census 2>&1); prc=$?
pu=$(sed -n 's/.*never compared in its file: \([0-9]*\)$/\1/p' <<<"$po"); pd=$(sed -n 's/^guards that drop or truncate at the cap: \([0-9]*\)$/\1/p' <<<"$po")
ck "3 FAIL-ONCE: three planted file-scope tables read RED, grown from the unplanted count ($n -> $((n + 3)))" '[ "$prc" != 0 ] && grep -q "grew from $n to $((n + 3))" <<<"$po"'
ck "4 the guard classifier can red, and a declared table leaves its line: of two never-compared plants only the undeclared one raises the no-guard count ($u -> ${pu:-?}) and the dropping plant the drop count by one ($d -> ${pd:-?})" '[ "$pu" = "$((u + 1))" ] && [ "$pd" = "$((d + 1))" ]'
ck "5 the function-scope population reads EXACTLY its baseline ($fn vs $fb)" '[ "$fn" = "$fb" ]'
[ "$fn" -gt "$fb" ] && echo "      a new fixed local that a program fills landed ($fb -> $fn): make it grow -- list them: python3 scripts/audit_fixed_caps_census.py --tsv FILE, column fill"
[ "$fn" -lt "$fb" ] && echo "      the function-scope population FELL ($fb -> $fn): lower scripts/fixtures/dyn_caps/BASELINE_FUNCTION_SCOPE to $fn in this landing"
pfn=$(sed -n 's/^function-scope arrays a program fills: \([0-9]*\) (baseline.*/\1/p' <<<"$po"); pfu=$(sed -n 's/^function-scope arrays a program fills, unguarded: \([0-9]*\)$/\1/p' <<<"$po")
ck "6 FAIL-ONCE for the second population: the list-walk local reads RED, grown by exactly one ($fn -> ${pfn:-?}, the radix buffer out), unguarded $fu -> ${pfu:-?}" '[ "$pfn" = "$((fn + 1))" ] && [ "$pfu" = "$((fu + 1))" ] && grep -q "function-scope population grew from $fn to $((fn + 1))" <<<"$po"'
nab=$(sed -n 's/^declared class A or B by .*: \([0-9]*\) table(s); stale declarations: \([0-9]*\)$/\1/p' <<<"$out"); pnab=$(sed -n 's/^declared class A or B by .*: \([0-9]*\) table(s); stale declarations: \([0-9]*\)$/\1 \2/p' <<<"$po")
ck "7 THE CLASS A/B FIXTURE (CEO-1234 (1)): the planted declaration counts ($nab -> ${pnab%% *}) and a row naming a table the tree does not declare reads RED by name (stale ${pnab##* })" '[ -n "$nab" ] && [ "$pnab" = "$((nab + 1)) 1" ] && grep -q "^  src/planted_by_the_ratchet_gate.c:g_no_such_table_by_the_ratchet_gate$" <<<"$po"'
printf 'main :- write(hello), nl.\n:- initialization(main).\n' > "$WORK/silent.pl"
{ grep -v '^#' "$HERE/fixtures/dyn_caps/WITNESSES.tsv" | grep 'PL_TR_ARENA_BYTES'
  printf 'src/runtime/pattern_match.c\tg_capo\t%s\t\t2\tnever printed\n' "$WORK/silent.pl"
  printf 'src/planted_by_the_ratchet_gate.c\tg_no_such_guard_by_the_ratchet_gate\t%s\t\t2\tnever printed\n' "$WORK/silent.pl"; } > "$WORK/witnesses.tsv"
wo=$(cd "$ROOT" && DYN_CAPS_WITNESSES="$WORK/witnesses.tsv" bash "$WIT" witness 2>&1); wrc=$?
wsum=$(grep -m1 '^refusal read at the cap: ' <<<"$wo")
ck "8 THE WITNESS VERB CAN RED (CEO-1231): on a scratch table the trail's cap+1 is READ, a program that never reaches g_capo's cap is SILENT, a row naming no LOUD guard is STALE, and the verb reds ($wsum, rc $wrc)" '[ "$wrc" = 1 ] && grep -q "^  READ .*PL_TR_ARENA_BYTES rc=2: .*trail arena exhausted" <<<"$wo" && grep -q "^  SILENT .*:g_capo rc=0" <<<"$wo" && grep -q "^  STALE .*g_no_such_guard_by_the_ratchet_gate" <<<"$wo" && grep -qE "^refusal read at the cap: 1 of [1-9][0-9]* guards, silent: 1$" <<<"$wo"'
if [ "$fails" = 0 ]; then echo "GATE PASS(0) [$GATE_NAME]: 8 arms -- $n fixed tables and $fn locals a program fills, each exactly its baseline; a planted one of each reds; the classifier tells a drop from no guard and a program's fill from a number's digits; $nab tables declared class A/B, none stale"; gate_stamp; exit 0; fi
echo "GATE FAIL(1) [$GATE_NAME]: $fails of 8 arms red"; gate_stamp; exit 1
