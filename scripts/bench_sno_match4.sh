#!/usr/bin/env bash
# bench_sno_match4.sh — the 4-way SNOBOL4 match/eval benchmark protocol (s143).
# claws5/treebank/json (match-only, tape rep52) + calculator-1 (evaluator, tape rep10)
# vs official SPITBOL, interleaved A/B x7 medians.  RT_OPT is whatever libscrip_rt
# was built with (-O0 unless Lon directed otherwise) — LABEL REPORTED NUMBERS WITH IT.
# Same-moment interleaved pairs are the only valid comparison; absolute ms drift
# with host load across runs (s143: treebank 51<->71 same bytes).
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
S4A="${S4E_ASSETS:-$([ -d "$S4E/x64" ] && echo "$S4E" || echo /home/resources)}"   # D-17b: ASSET root -- oracles/vendor trees live at the HQ root on this machine (Lon: seats carry ONLY .github/SCRIP/corpus); a root owning its own x64 (HQ, or a full standalone clone-set) is self-contained.
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; SCRIP="${SCRIP:-$HERE/../scrip}"; RT="${RT_DIR:-$HERE/../out}"
. "$HERE/lib_oracle_flags.sh" 2>/dev/null || { echo "REFUSING: cannot load lib_oracle_flags.sh -- the ONE oracle-flag authority (s200/s255)." >&2; exit 3; }
SBL="${SBL:-$(sbl_clean_bin)}"; CORPUS="${CORPUS:-$S4E/corpus}"; D="$CORPUS/demo"; W="${W:-$(mktemp -d)}"   # BENCHMARK oracle (s255)
[ -x "$SBL" ] || { echo "⛔ ORACLE ABSENT: $SBL — every ratio below would be fiction, not a benign gap (row oracle-two-face-adoption). Build /home/resources/spitbol-bench-oracle (see RULES.md Oracles) -- seats do not clone x64 (s255)." >&2; exit 3; }
input_of() { case $1 in claws5-match) echo $D/CLAWS5inTASA.dat;; treebank-match) echo $D/VBGinTASA.dat;; json-match) echo $D/twitter.json;; calculator-1-match|calculator-2-match) echo $D/calculator.input;; esac; }
mkrep() { python3 - "$1" "$2" "$3" << 'PYEOF'
import re, sys
src_path, out_path, reps = sys.argv[1], sys.argv[2], int(sys.argv[3])
lines = open(src_path).read().splitlines(); mi = None
for i, l in enumerate(lines):
    m = re.match(r'^(\s*)src\s+(\S+)\s+:F\((\w+)\)\s*$', l)
    if m and 'INPUT' not in l: mi, ind, pat, flab = i, m.group(1), m.group(2), m.group(3)
tail = [ind+'tape           =  &LCASE &UCASE', ind+'tape           LEN(%d) . reps'%reps,
 'benchloop'.ljust(max(len(ind),10))+'src            '+pat+'                     :F('+flab+')',
 ind+'reps           LEN(1) =                    :S(benchloop)',
 ind+"OUTPUT         =  'matched bytes=' SIZE(src) :(END)", flab.ljust(len(ind))+"OUTPUT         =  'Pattern match failed'", 'END']
open(out_path,'w').write('\n'.join(lines[:mi]+tail)+'\n')
PYEOF
}
evalrep() { python3 - "$D/calculator-1.sno" "$1" << 'PYEOF'
import sys
src = open(sys.argv[1]).read().splitlines()
cut = next(i for i,l in enumerate(src) if l.startswith('loop'))
head = '\n'.join(src[:cut]).replace('        &TRIM = 1','        &TRIM = 1\n        lines = ARRAY(200000)')
tail = """rd      n = n + 1
        lines[n] = INPUT                        :S(rd)
        n = n - 1
        tape = &LCASE &UCASE
        tape LEN(9) . reps
bl      k = 0
nx      k = k + 1
        GT(k, n)                                :S(od)
        sp = 0
        lines[k] C                              :F(bad2)
        total = total + DRF(S[1])               :(nx)
od      reps LEN(1) =                           :S(bl)
        OUTPUT = 'total=' total                 :(END)
bad2    OUTPUT = 'Boo!'                         :(END)
END"""
open(sys.argv[2],'w').write(head+'\n'+tail+'\n')
PYEOF
}
cd "$W"
REPS="${REPS:-20}"
PROGS="${PROGS:-claws5-match treebank-match json-match calculator-1-match calculator-2-match}"
for p in $PROGS; do mkrep "$D/$p.sno" "$W/$p-rep.sno" "$REPS"; done
for p in $PROGS; do "$SCRIP" --compile "$W/$p-rep.sno" > "$W/$p.s" 2>/dev/null; gcc -no-pie "$W/$p.s" -L"$RT" -lscrip_rt -lm -Wl,-rpath,"$RT" -o "$W/$p.prog" 2>/dev/null || { echo "$p BUILD FAIL"; exit 1; }; done
for p in $PROGS; do IN=$(input_of $p)
  so=$(timeout 300 "$SBL" $(sbl_lang_flags) "$W/$p-rep.sno" < "$IN" 2>/dev/null); mo=$(timeout 300 "$W/$p.prog" < "$IN" 2>/dev/null)
  [ "$so" = "$mo" ] || { echo "$p IDENTITY FAIL sbl=[$so] m4=[$mo]"; exit 1; }
  s=(); m=()
  for i in 1 2 3 4 5 6 7; do t0=$(date +%s%N); "$SBL" $(sbl_lang_flags) "$W/$p-rep.sno" < "$IN" > /dev/null 2>&1; t1=$(date +%s%N); s+=( $(( (t1-t0)/1000000 )) )
    t0=$(date +%s%N); "$W/$p.prog" < "$IN" > /dev/null 2>&1; t1=$(date +%s%N); m+=( $(( (t1-t0)/1000000 )) ); done
  sm=$(printf '%s\n' "${s[@]}" | sort -n | sed -n 4p); mm=$(printf '%s\n' "${m[@]}" | sort -n | sed -n 4p)
  echo "$p: scrip_m4=${mm}ms sbl=${sm}ms ratio=$(python3 -c "print(f'{$mm/$sm:.2f}')") identity=OK"
done
