#!/usr/bin/env bash
# test_gate_xfail_marker_and_index_agree.sh -- THE XFAIL MARKER AND THE INDEX MUST NAME THE SAME ENTRIES.
# (hq_T 2026-09-05, row test-suite-consistency-seven-languages-one-standard; the half of
# FINDING-2026-09-05-hq_T-every-xfail-promotion-leaves-the-master-unsorted-and-the-csv-stale that had no guard.)
#
# ⛔ WHAT BREAKS IT IS A PROMOTION, AND IT BROKE FIVE TIMES IN ONE DAY. An XFAIL lives in THREE places: the
# banner in the master itself, the `xfail` column of ALL.csv, and the reason line in ALL.xfail. A promotion
# rewrites the master and the reasons file and LEAVES THE INDEX AT 1. Measured at corpus 5df255b01:
# ALL.sno carried 46 banners while ALL.csv still called 49 entries xfail -- and nothing was inconsistent in a
# way any gate could see, because every gate compared a file with ITSELF. The suite and its index agreed on
# every other column; the marker gates were green; the order gate caught the ORDER half and said nothing
# about this one.
# ⭐ THE RULE THIS ASSERTS: when one fact is stored in three representations, the guard belongs on the
# AGREEMENT, never on any single one of them. Neither file is wrong on its own terms -- they are wrong
# relative to each other, which is a property no single-file check can hold.
#
# ⛔⛔ COMMENT SYNTAX IS PER-LANGUAGE AND A SNOBOL4-SHAPED MATCHER SILENTLY READS ZERO. Measured while
# writing this gate, and it is the reason the regex below leads with \W* instead of a literal '*':
# snobol4 banners are `*--- N name XFAIL`, raku's are `#--- ...`, snocone's are `/*--- ... */`. A `^\*-`
# pattern reports raku as 0 banners against 156 index rows -- a 156-entry catastrophe that does not exist,
# and it would have been filed as one. A census instrument written in one language's punctuation answers
# "how many does MY dialect have", never "how many are there".
#
# Usage: bash scripts/test_gate_xfail_marker_and_index_agree.sh
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
cd "$ROOT"
. "$HERE/lib_gate.sh"
gate_parse_args "$@"
S4E="${S4E_HOME:-$(cd "$ROOT/.." && pwd)}"
TESTS="$S4E/corpus/tests"
gate_require "$TESTS" "the per-language master suites"

violations=0
examined=0

# The census both arms share, so the gate and its own witness cannot drift apart.
census() {
python3 - "$1" <<'EOF'
import csv, os, re, sys
RX = re.compile(r'^\W*-{3,}\s+(\d+)\s+(\S+)\s+XFAIL\b')
EXT = {"snobol4":"sno","icon":"icn","prolog":"pl","raku":"raku","pascal":"pas","snocone":"sc","rebus":"reb"}
tests = sys.argv[1]
def names(p):
    if not os.path.isfile(p):
        return None
    out = set()
    with open(p, encoding="utf-8", errors="replace") as fh:
        for l in fh:
            m = RX.match(l)
            if m:
                out.add(m.group(2))
    return out
graded = bad = reasons_missing = 0
for lang in sorted(EXT):
    d = os.path.join(tests, lang)
    master, index = os.path.join(d, "ALL." + EXT[lang]), os.path.join(d, "ALL.csv")
    if not (os.path.isfile(master) and os.path.isfile(index)):
        continue
    ban = names(master)
    with open(index, encoding="utf-8") as fh:
        rows = list(csv.DictReader(fh))
    if not rows:
        continue
    graded += 1
    idx = {r["entry"] for r in rows if r.get("xfail") == "1"}
    only_master, only_index = sorted(ban - idx), sorted(idx - ban)
    rea = names(os.path.join(d, "ALL.xfail"))
    missing = sorted(idx - rea) if rea is not None else []
    reasons_missing += len(missing)
    print("  %-9s master=%-4d index=%-4d reasons=%-6s %s" % (
        lang, len(ban), len(idx), "-" if rea is None else len(rea),
        "agree" if not (only_master or only_index) else "⛔ DISAGREE"))
    if only_master or only_index:
        bad += 1
        for n in only_master[:5]:
            print("      banner in the master, xfail=0 in the index: %s" % n)
        for n in only_index[:5]:
            print("      xfail=1 in the index, NO banner in the master: %s  <-- the promotion shape" % n)
    if missing:
        print("      %d xfail(s) with no reason recorded in ALL.xfail: %s" % (len(missing), ", ".join(missing[:4])))
if not graded:
    sys.stderr.write("REFUSED: graded ZERO masters under %s\n" % tests)
    raise SystemExit(2)
print("GRADED=%d BAD=%d REASONS_MISSING=%d" % (graded, bad, reasons_missing))
EOF
}

# ARM 1 — THE AGREEMENT ITSELF, pinned at zero. It is pinned rather than ratcheted because it is SATISFIED
# TODAY (corpus e2f9c2f2c re-sorted snobol4 back into agreement, 46 == 46), and a bar you can meet is a bar.
examined=$((examined + 1))
out="$(census "$TESTS" 2>&1)"; crc=$?
printf '%s\n' "$out"
if [ "$crc" -eq 2 ] || ! printf '%s' "$out" | grep -q '^GRADED='; then
    echo "⛔ GATE REFUSES: the marker/index census could not measure (rc=$crc)"
    exit 2
fi
if [ "$(printf '%s' "$out" | sed -n 's/^GRADED=[0-9]* BAD=\([0-9]*\).*/\1/p')" != "0" ]; then
    echo "GATE FAIL: a master's XFAIL banners and its ALL.csv xfail column name different entries."
    echo "           A promotion that rewrites the master and leaves the index at 1 is the measured shape;"
    echo "           re-run the builder (--reindex) so the index follows the suite."
    violations=$((violations + 1))
fi

# ARM 2 — EVERY XFAIL SHOULD CARRY A REASON, and this one IS ratcheted: snobol4 has 5 xfails with no entry in
# ALL.xfail today, and pinning at zero would red make test over debt this arm was written to expose rather
# than to punish. ⭐ It is a real gap and not bookkeeping: ALL.xfail is where a reader learns whether an xfail
# is a deliberate dialect choice or an open defect, and an xfail with no reason is indistinguishable from one
# nobody has looked at. Lower this when the reasons are written.
REASONS_WATERMARK=5
examined=$((examined + 1))
rm_count="$(printf '%s' "$out" | sed -n 's/^GRADED=[0-9]* BAD=[0-9]* REASONS_MISSING=\([0-9]*\)/\1/p')"
if [ "${rm_count:-0}" -gt "$REASONS_WATERMARK" ]; then
    echo "GATE FAIL: $rm_count xfail(s) carry no recorded reason, above the watermark of $REASONS_WATERMARK"
    violations=$((violations + 1))
fi

# ARM 3 — ⛔ THE WITNESS IS THE REAL DEFECT AS IT LANDED, NOT A HAND-BUILT IMITATION. Graded on a scratch tree
# built from corpus 5df255b01 -- the promotion commit itself, where ALL.sno carried 46 banners against 49
# index rows. If this gate cannot red on the exact tree that motivated it, it does not guard anything.
# Skipped (not failed) where that revision is unreachable, so a shallow or re-cloned corpus cannot turn a
# missing fixture into a false green -- it says which.
examined=$((examined + 1))
W="$(mktemp -d "${TMPDIR:-/tmp}/xfail_agree.XXXXXX")"
trap 'rm -rf "$W"' EXIT
mkdir -p "$W/snobol4"
if git -C "$S4E/corpus" cat-file -e 5df255b01:tests/snobol4/ALL.csv 2>/dev/null; then
    git -C "$S4E/corpus" show 5df255b01:tests/snobol4/ALL.sno > "$W/snobol4/ALL.sno" 2>/dev/null
    git -C "$S4E/corpus" show 5df255b01:tests/snobol4/ALL.csv > "$W/snobol4/ALL.csv" 2>/dev/null
    wout="$(census "$W" 2>&1)"
    if printf '%s' "$wout" | grep -q 'GRADED=1 BAD=1'; then
        :
    else
        echo "GATE FAIL: the census did NOT red on corpus 5df255b01, the tree this gate exists for"
        printf '%s\n' "$wout" | sed 's/^/    /'
        violations=$((violations + 1))
    fi
else
    echo "GATE NOTE: corpus 5df255b01 unreachable here -- ARM 3's historical witness was NOT graded"
fi

GATE_EXAMINED="$examined arms"
gate_verdict "$violations" "masters whose XFAIL marker and index disagree"
