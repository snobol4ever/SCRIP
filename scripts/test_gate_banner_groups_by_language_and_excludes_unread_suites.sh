#!/usr/bin/env bash
# test_gate_banner_groups_by_language_and_excludes_unread_suites.sh -- THE GROUPED SUITE BANNER (cto, 2026-09-13).
# Lon 2026-09-13, verbatim: "Add language and group by language the test suite banner."
#
# THREE PROPERTIES, and the third is the one that needs a gate rather than a look:
#   1. every language in SUITES.tsv gets exactly one group rule on the banner, and every suite prints under one;
#   2. each rule carries that language's own pass/total and gap;
#   3. ⛔ A SUITE WITH NO READING IS IN NEITHER SIDE OF ITS LANGUAGE'S FRACTION AND IS NAMED ON THE RULE.
#
# ⛔ WHY (3) IS THE LOAD-BEARING ARM, and it is the whole reason this file exists. A no-runner or Lon-deferred
# suite has a REAL POPULATION and UNKNOWN PASSES. Folding it in as zero passes prints a language as failing work
# nobody has graded; dropping its population instead shrinks the denominator until the fraction flatters us.
# Both are one-character mistakes in a sum and NEITHER IS VISIBLE ON THE PRINTED LINE -- the number just reads
# slightly different and no reader can tell which arithmetic produced it. UNKNOWN IS NOT ZERO AND IT IS NOT
# ABSENT (ARCH-PROGRAM-LEDGER § THE DENOMINATOR RULE). Prolog today carries gnu_fd, 30 programs DEFERRED by Lon
# (CEO-572/579), so this arm has a live witness rather than a hypothetical one.
#
# ⭐ THE ARITHMETIC IS RECOMPUTED FROM SUITES.tsv HERE, INDEPENDENTLY OF THE BANNER, and the two are compared.
# A gate that re-ran the banner's own summing code would agree with it whatever it did.
set -u
GATE_NAME=test_gate_banner_groups_by_language_and_excludes_unread_suites
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="${S4E_HOME:-$(cd "$HERE/../.." && pwd)}"
B="$ROOT/.github/scripts/util_suite_banner.py"
TSV="$ROOT/.github/SUITES.tsv"
refuse() { echo "⛔ REFUSED(2) [$GATE_NAME]: $*" >&2; exit 2; }
[ -f "$B" ]   || refuse "no util_suite_banner.py at $B -- this gate grades what the board prints"
[ -f "$TSV" ] || refuse "no SUITES.tsv at $TSV -- a gate with no data source measures nothing"
grep -q 'def banner' "$B" || refuse "util_suite_banner.py no longer defines banner() -- this gate grades a rule that is gone"
OUT="$(python3 "$B" 2>&1)" || refuse "the banner did not run"
red=0; arms=0
chk() { arms=$((arms+1)); if [ "$2" = "$3" ]; then echo "  ok  $1"; else echo "  RED $1: got [$2] want [$3]"; red=$((red+1)); fi; }
python3 - "$TSV" > "$HERE/../.gate_lang_expect.$$" <<'EOP'
import sys
rows=[l.rstrip('\n').split('\t') for l in open(sys.argv[1]) if l.strip() and not l.startswith('#')]
hdr=rows[0]; ix={k:i for i,k in enumerate(hdr)}
agg={}
for r in rows[1:]:
    if len(r)<len(hdr): continue
    lang=r[ix['lang']]; a=agg.setdefault(lang,[0,0,0,0])
    p,t=r[ix['today_pass']],r[ix['today_total']]
    a[3]+=1
    if p in ('-','') or r[ix['today_date']] in ('-',''):
        a[2]+=1
        continue
    a[0]+=int(p); a[1]+=int(t)
for lang in sorted(agg):
    p,t,u,n=agg[lang]
    print(f"{lang}\t{p}\t{t}\t{t-p}\t{u}\t{n}")
EOP
EXP="$HERE/../.gate_lang_expect.$$"
trap 'rm -f "$EXP"' EXIT
while IFS=$'\t' read -r lang p t gap unread n; do
    line="$(printf '%s\n' "$OUT" | grep -m1 -- "^-- $lang ")"
    if [ -z "$line" ]; then echo "  RED $lang: no group rule on the banner"; red=$((red+1)); arms=$((arms+1)); continue; fi
    got="$(printf '%s\n' "$line" | sed -n "s|^-- $lang \([0-9]*/[0-9]*\) gap \([0-9]*\) .*|\1 \2|p")"
    chk "$lang fraction and gap" "$got" "$p/$t $gap"
    arms=$((arms+1))
    if [ "$unread" -gt 0 ]; then
        if printf '%s\n' "$line" | grep -q -- "+$unread unread"; then echo "  ok  $lang names its $unread unread suite(s) on the rule"
        else echo "  RED $lang: $unread suite(s) have no reading and the rule does not name them -- an ungraded population that vanishes from the board is the defect this arm exists to catch"; red=$((red+1)); fi
    else
        if printf '%s\n' "$line" | grep -q -- 'unread'; then echo "  RED $lang: rule claims unread suites but SUITES.tsv has none"; red=$((red+1))
        else echo "  ok  $lang has no unread suite and claims none"; fi
    fi
done < "$EXP"
nrule="$(printf '%s\n' "$OUT" | grep -c '^-- ')"
nlang="$(wc -l < "$EXP" | tr -d ' ')"
chk "one group rule per language" "$nrule" "$nlang"
# ⛔ COUNT BY NAME, NOT BY LINE. A first cut counted lines whose FIRST field looked like a cell and read 14 of
# 25 -- the grid puts a second suite after " | " on the same line, so half the board was invisible to its own
# gate while every other arm passed. A miscount that reads LOW is the benign direction; the same mistake
# reading HIGH would have certified a board that silently dropped suites.
missing=""; dup=""
while IFS=$'\t' read -r _k nick _rest; do
    c="$(printf '%s\n' "$OUT" | grep -oE "(^|\| )$nick +([0-9]+|-)/" | wc -l | tr -d ' ')"
    [ "$c" = "1" ] || { if [ "$c" = "0" ]; then missing="$missing $nick"; else dup="$dup $nick(x$c)"; fi; }
done < <(awk -F'\t' 'NR>2 && $1!~/^#/ {print $1"\t"$2}' "$TSV")
arms=$((arms+1))
if [ -z "$missing" ] && [ -z "$dup" ]; then echo "  ok  every suite prints under a group exactly once"
else echo "  RED suites missing from the board:$missing ; printed more than once:$dup"; red=$((red+1)); fi
chk "the fleet line still leads" "$(printf '%s\n' "$OUT" | head -1 | grep -cE '^[0-9][0-9]-[0-9][0-9] [0-9]+ SUITES')" "1"
echo "$GATE_NAME: arms=$arms red=$red"
[ "$red" -eq 0 ] || { echo "⛔ $GATE_NAME RED"; exit 1; }
echo "✅ $GATE_NAME GREEN"
