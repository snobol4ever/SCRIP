#!/usr/bin/env bash
# test_gate_raku_nfa_oracle.sh — RK-NFA-1/2/3 oracle-equivalence gate (Raku, mode-2, ISOLATED).
#   Proves the IR_NFA_* graph backtracking walk (raku_nfa_bb.c nfa_bt_ir, selected by RK_NFA_BB=1)
#   returns verdicts byte-identical to the parallel-NFA oracle (raku_re.c raku_nfa_exec, RK_NFA_BB=0)
#   on a battery exercising every IR_NFA_* kind: CHAR ANY CLASS SPLIT EPS BOL EOL CAP_OPEN CAP_CLOSE ACCEPT.
#   The `~~` smartmatch operator is lowered for Raku in lower.c (TT_SMATCH -> IR_CALL re_match);
#   re_match honors RK_NFA_BB, so the SAME lowered program drives both matchers.
#   Sections: RK-NFA-1 (verdict battery, all kinds) · RK-NFA-2 (cset/anchor/alt L4-L12 verdict
#   set + safe-extent captures, with the documented | LTM vs || ordered seam) · RK-NFA-3 (capture
#   recording $0/$1/$<name>).
# Scope: mode-2 only (the HARD oracle). The NFA leaf templates are SHELVED (tier-seam decision),
#   so modes 3/4 EXCISE this path; this gate does not touch them.
# Isolation: edits live only in raku_nfa_bb.c + the Raku TT_SMATCH arm in lower.c; SNOBOL4/Icon/Prolog
#   pattern paths are untouched (the IR_NFA_* family is isolated by design).
# Exit 0 iff both matchers run rc=0 AND produce identical output.
# AUTHORS: Lon Jones Cherryholmes · Jeffrey Cooper M.D. · Claude Sonnet · Claude Opus  DATE: 2026-06-03
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${HERE}/../scrip"
TMP="$(mktemp -d /tmp/rknfa_XXXXXX)"
trap 'rm -rf "$TMP"' EXIT

# Battery: 2 subjects x 15 patterns = 30 probes (stays under the Raku per-proc node ceiling).
# Every pattern is supported by raku_re.c's builder; together they cover all IR_NFA_* kinds.
cat > "$TMP/battery.raku" << 'RAKU'
sub main() {
    my $A = 'abc123';
    if ($A ~~ /\d+/)         { say('A d Y');   } else { say('A d N');   }
    if ($A ~~ /[a-z]+/)      { say('A loz Y'); } else { say('A loz N'); }
    if ($A ~~ /a|b/)         { say('A ab Y');  } else { say('A ab N');  }
    if ($A ~~ /.*/)          { say('A ds Y');  } else { say('A ds N');  }
    if ($A ~~ /^x$/)         { say('A ax Y');  } else { say('A ax N');  }
    if ($A ~~ /\w+/)         { say('A w Y');   } else { say('A w N');   }
    if ($A ~~ /\s/)          { say('A s Y');   } else { say('A s N');   }
    if ($A ~~ /ab*c/)        { say('A abs Y'); } else { say('A abs N'); }
    if ($A ~~ /ab+c/)        { say('A abp Y'); } else { say('A abp N'); }
    if ($A ~~ /ab?c/)        { say('A abo Y'); } else { say('A abo N'); }
    if ($A ~~ /[0-9]+[a-z]/) { say('A nl Y');  } else { say('A nl N');  }
    if ($A ~~ /(foo|bar)+/)  { say('A fb Y');  } else { say('A fb N');  }
    if ($A ~~ /^\d+$/)       { say('A fn Y');  } else { say('A fn N');  }
    if ($A ~~ /\D+/)         { say('A D Y');   } else { say('A D N');   }
    if ($A ~~ /x.y/)         { say('A xy Y');  } else { say('A xy N');  }
    my $I = 'foobarfoo';
    if ($I ~~ /\d+/)         { say('I d Y');   } else { say('I d N');   }
    if ($I ~~ /[a-z]+/)      { say('I loz Y'); } else { say('I loz N'); }
    if ($I ~~ /a|b/)         { say('I ab Y');  } else { say('I ab N');  }
    if ($I ~~ /.*/)          { say('I ds Y');  } else { say('I ds N');  }
    if ($I ~~ /^x$/)         { say('I ax Y');  } else { say('I ax N');  }
    if ($I ~~ /\w+/)         { say('I w Y');   } else { say('I w N');   }
    if ($I ~~ /\s/)          { say('I s Y');   } else { say('I s N');   }
    if ($I ~~ /ab*c/)        { say('I abs Y'); } else { say('I abs N'); }
    if ($I ~~ /ab+c/)        { say('I abp Y'); } else { say('I abp N'); }
    if ($I ~~ /ab?c/)        { say('I abo Y'); } else { say('I abo N'); }
    if ($I ~~ /[0-9]+[a-z]/) { say('I nl Y');  } else { say('I nl N');  }
    if ($I ~~ /(foo|bar)+/)  { say('I fb Y');  } else { say('I fb N');  }
    if ($I ~~ /^\d+$/)       { say('I fn Y');  } else { say('I fn N');  }
    if ($I ~~ /\D+/)         { say('I D Y');   } else { say('I D N');   }
    if ($I ~~ /x.y/)         { say('I xy Y');  } else { say('I xy N');  }
    say('battery done');
}
RAKU

RK_NFA_BB=0 timeout 20 "$SCRIP" --interp "$TMP/battery.raku" </dev/null >"$TMP/oracle.out" 2>/dev/null; rc_o=$?
RK_NFA_BB=1 timeout 20 "$SCRIP" --interp "$TMP/battery.raku" </dev/null >"$TMP/irbb.out"   2>/dev/null; rc_b=$?

echo "=== RK-NFA-1 oracle-equivalence (IR_NFA_* graph walk vs parallel-NFA oracle) ==="
if [ "$rc_o" -ne 0 ]; then echo "FAIL: oracle path (RK_NFA_BB=0) rc=$rc_o"; exit 1; fi
if [ "$rc_b" -ne 0 ]; then echo "FAIL: IR-graph path (RK_NFA_BB=1) rc=$rc_b"; exit 1; fi
if ! tail -1 "$TMP/oracle.out" | grep -q 'battery done'; then echo "FAIL: oracle did not complete battery"; exit 1; fi
if diff -q "$TMP/oracle.out" "$TMP/irbb.out" >/dev/null; then
    n=$(grep -c ' [YN]$' "$TMP/oracle.out")
    echo "PASS: $n verdict probes — IR_NFA_* graph walk == parallel-NFA oracle (byte-identical)"
else
    echo "FAIL: IR-graph walk diverges from oracle (verdicts)"
    diff "$TMP/oracle.out" "$TMP/irbb.out" | head -40
    exit 1
fi

# ────────────────────────────────────────────────────────────────────────────────────────────
# RK-NFA-2 — formalize the L4-L12 verdict set: negated shorthand csets, enumerated csets with
#   ranges + negation, mixed shorthands inside [...], and BOL/EOL anchors. The IR_NFA_* graph
#   walk (raku_nfa_bb.c) already handles these (the RK-NFA-1 battery exercises a subset); this
#   section pins down the full edge-case verdict set against the parallel-NFA oracle.
#
#   SEMANTIC NOTE (the | LTM vs || ordered seam — KEEP, per GOAL-RAKU-BB locked decision):
#   the C builder (raku_re.c) parses only single `|`, lowered to an NFA SPLIT. The parallel
#   oracle (raku_nfa_exec) resolves SPLIT by leftmost-LONGEST (Raku `|` LTM / declarative);
#   the backtracking IR-graph walker (nfa_bt_ir_cap) resolves SPLIT by leftmost-FIRST (Raku
#   `||` ordered). For a BOOLEAN VERDICT these always agree (if any branch reaches ACCEPT both
#   say matched), so the verdict battery below spans alternation freely. For MATCH EXTENT /
#   CAPTURES they agree ONLY where leftmost-longest == leftmost-first (greedy quantifiers,
#   disjoint/anchored alternatives) — the safe-extent battery below stays inside that envelope.
#   Overlapping-`|` extent (e.g. /(a|ab)/ ~ "ab" -> oracle "ab" vs walker "a") DIVERGES BY
#   DESIGN: it is the Phase-2 `|`-LTM-on-the-parallel-NFA vs `||`-ordered-on-IR_NFA_* boundary,
#   not a bug, so it is deliberately NOT probed here. `||` itself is not yet parseable.
cat > "$TMP/cset.raku" << 'RAKU'
sub main() {
    # negated shorthand csets — \D \W \S (NK_CLASS with inverted bitmap, shared by both engines)
    if ('abc'    ~~ /\D+/) { say('c1 Y'); } else { say('c1 N'); }   # all non-digit
    if ('123'    ~~ /\D/)  { say('c2 Y'); } else { say('c2 N'); }   # only digits -> N
    if ('a b'    ~~ /\S/)  { say('c3 Y'); } else { say('c3 N'); }   # has non-space
    if (' '      ~~ /\S/)  { say('c4 Y'); } else { say('c4 N'); }   # only space -> N
    if ('a_b'    ~~ /\W/)  { say('c5 Y'); } else { say('c5 N'); }   # _,a,b all word -> N
    if ('a-b'    ~~ /\W/)  { say('c6 Y'); } else { say('c6 N'); }   # - is non-word
    if ('hello'  ~~ /\w+/) { say('c7 Y'); } else { say('c7 N'); }   # \w full coverage
    if ("\t"     ~~ /\s/)  { say('c8 Y'); } else { say('c8 N'); }   # tab is \s
    # enumerated csets — ranges, negation, multi-range
    if ('hello5' ~~ /[a-z0-9]+/) { say('c9 Y');  } else { say('c9 N');  }  # multi-range
    if ('ABC'    ~~ /[^0-9]+/)   { say('c10 Y'); } else { say('c10 N'); }  # negated range
    if ('999'    ~~ /[^0-9]/)    { say('c11 Y'); } else { say('c11 N'); }  # negated, all digit -> N
    if ('Hi9'    ~~ /[A-Za-z]/)  { say('c12 Y'); } else { say('c12 N'); }  # two ranges
    if ('___'    ~~ /[A-Za-z]/)  { say('c13 Y'); } else { say('c13 N'); }  # no letter -> N
    # mixed shorthand inside [...]
    if ('a 1'    ~~ /[\d\s]/) { say('c14 Y'); } else { say('c14 N'); }  # space or digit
    if ('xyz'    ~~ /[\d\s]/) { say('c15 Y'); } else { say('c15 N'); }  # neither -> N
    if ('a-z'    ~~ /[\w-]+/) { say('c16 Y'); } else { say('c16 N'); }  # word + literal dash
    # BOL / EOL anchors (L: ^ $)
    if ('hello'  ~~ /^h/)      { say('c17 Y'); } else { say('c17 N'); }  # BOL hit
    if ('hello'  ~~ /^e/)      { say('c18 Y'); } else { say('c18 N'); }  # not at BOL -> N
    if ('hello'  ~~ /o$/)      { say('c19 Y'); } else { say('c19 N'); }  # EOL hit
    if ('hello'  ~~ /h$/)      { say('c20 Y'); } else { say('c20 N'); }  # not at EOL -> N
    if ('hello'  ~~ /^hello$/) { say('c21 Y'); } else { say('c21 N'); }  # full anchor
    if ('hellox' ~~ /^hello$/) { say('c22 Y'); } else { say('c22 N'); }  # full anchor miss
    if (''       ~~ /^$/)      { say('c23 Y'); } else { say('c23 N'); }  # empty anchored
    say('cset done');
}
RAKU
RK_NFA_BB=0 timeout 20 "$SCRIP" --interp "$TMP/cset.raku" </dev/null >"$TMP/cset_o.out" 2>/dev/null; rc_so=$?
RK_NFA_BB=1 timeout 20 "$SCRIP" --interp "$TMP/cset.raku" </dev/null >"$TMP/cset_b.out" 2>/dev/null; rc_sb=$?
if [ "$rc_so" -ne 0 ] || [ "$rc_sb" -ne 0 ]; then echo "FAIL: cset battery rc oracle=$rc_so irbb=$rc_sb"; exit 1; fi
if ! tail -1 "$TMP/cset_o.out" | grep -q 'cset done'; then echo "FAIL: cset oracle did not complete"; exit 1; fi
if diff -q "$TMP/cset_o.out" "$TMP/cset_b.out" >/dev/null; then
    n=$(grep -c ' [YN]$' "$TMP/cset_o.out")
    echo "PASS: $n cset/anchor verdict probes — IR_NFA_* graph walk == parallel-NFA oracle (byte-identical)"
else
    echo "FAIL: IR-graph cset/anchor verdicts diverge from oracle"
    diff "$TMP/cset_o.out" "$TMP/cset_b.out" | head -40
    exit 1
fi

# RK-NFA-2 safe-extent battery — captures where leftmost-longest == leftmost-first (greedy
#   quantifiers, disjoint/anchored alternation, cset captures). Stays inside the envelope where
#   the | (LTM) oracle and the || (ordered) IR-graph walker provably co-agree on EXTENT.
cat > "$TMP/ext2.raku" << 'RAKU'
sub main() {
    my $s = 'barbar';
    if ($s ~~ /(foo|bar)/)       { say('e1=[' ~ $0 ~ ']'); } else { say('e1 no'); }   # disjoint alt
    my $t = 'cat';
    if ($t ~~ /^(cat|dog)$/)     { say('e2=[' ~ $0 ~ ']'); } else { say('e2 no'); }   # anchored alt
    my $u = 'abc123';
    if ($u ~~ /(\D+)/)           { say('e3=[' ~ $0 ~ ']'); } else { say('e3 no'); }   # negated-cset greedy
    if ($u ~~ /([a-z]+)/)        { say('e4=[' ~ $0 ~ ']'); } else { say('e4 no'); }   # range-class greedy
    if ($u ~~ /([0-9]+)/)        { say('e5=[' ~ $0 ~ ']'); } else { say('e5 no'); }   # digit-class greedy
    my $v = 'key=val';
    if ($v ~~ /(\w+)=(\w+)/)     { say('e6=[' ~ $0 ~ '][' ~ $1 ~ ']'); } else { say('e6 no'); }   # two greedy caps
    if ($u ~~ /<word>([a-z]+)<num>([0-9]+)/) { say('e7=[' ~ $<word> ~ '][' ~ $<num> ~ ']'); } else { say('e7 no'); }   # named over csets
    my $w = 'aaab';
    if ($w ~~ /(a*)/)            { say('e8=[' ~ $0 ~ ']'); } else { say('e8 no'); }   # greedy star
    say('ext2 done');
}
RAKU
RK_NFA_BB=0 timeout 20 "$SCRIP" --interp "$TMP/ext2.raku" </dev/null >"$TMP/ext2_o.out" 2>/dev/null; rc_eo=$?
RK_NFA_BB=1 timeout 20 "$SCRIP" --interp "$TMP/ext2.raku" </dev/null >"$TMP/ext2_b.out" 2>/dev/null; rc_eb=$?
if [ "$rc_eo" -ne 0 ] || [ "$rc_eb" -ne 0 ]; then echo "FAIL: safe-extent battery rc oracle=$rc_eo irbb=$rc_eb"; exit 1; fi
if ! tail -1 "$TMP/ext2_o.out" | grep -q 'ext2 done'; then echo "FAIL: safe-extent oracle did not complete"; exit 1; fi
if diff -q "$TMP/ext2_o.out" "$TMP/ext2_b.out" >/dev/null; then
    echo "PASS: safe-extent captures (greedy / disjoint+anchored alt / csets) — IR-graph spans == parallel-NFA oracle"
else
    echo "FAIL: IR-graph safe-extent captures diverge from oracle"
    diff "$TMP/ext2_o.out" "$TMP/ext2_b.out" | head -40
    exit 1
fi

# ────────────────────────────────────────────────────────────────────────────────────────────
# RK-NFA-2 epsilon-loop TERMINATION battery (regression guard, found by differential fuzz 2026-06-03).
#   A quantifier over an empty-matchable subpattern — (a?)*, (a*)*, ()*, (|a)*, and the same shapes
#   under a constraint that forces backtracking ((a?)*$ on "aab", (a+)+$, ((a|b|c)*)*$) — builds an
#   epsilon loop in the NFA. The recursive backtracking walker (nfa_bt_ir_cap) used to spin on the
#   zero-width cycle and overflow the C stack (SIGSEGV), while the parallel-NFA oracle (visited[]
#   eps-closure) returned the correct verdict. Fixed by a (node,pos) visited memo mirroring the
#   oracle; this battery proves the walker now (a) does NOT crash and (b) agrees with the oracle —
#   including catastrophic-backtracking shapes on longer subjects, which the memo also bounds to
#   linear work. Both matchers must run rc=0 and produce identical output.
cat > "$TMP/eps.raku" << 'RAKU'
sub main() {
    if ('xyz'  ~~ /(a?)*/)  { say('p1 Y'); } else { say('p1 N'); }      # empty-loop, unanchored
    if ('xyz'  ~~ /(a*)*/)  { say('p2 Y'); } else { say('p2 N'); }      # nested empty-loop
    if ('xyz'  ~~ /()*/)    { say('p3 Y'); } else { say('p3 N'); }      # truly-empty group loop
    if ('xyz'  ~~ /(|a)*/)  { say('p4 Y'); } else { say('p4 N'); }      # empty-alternative loop
    if ('aaa'  ~~ /^(a?)*$/) { say('p5 Y'); } else { say('p5 N'); }     # anchored, satisfiable
    if ('aab'  ~~ /^(a?)*$/) { say('p6 Y'); } else { say('p6 N'); }     # anchored, forces backtrack -> N
    if ('b'    ~~ /(a?)*b/)  { say('p7 Y'); } else { say('p7 N'); }     # eps-loop then required char
    if ('aac'  ~~ /(a?)*b/)  { say('p8 Y'); } else { say('p8 N'); }     # required char fails -> N (was SEGV)
    if ('aaaaaaaaaaaaaaaa'  ~~ /(a*)*$/)       { say('p9 Y');  } else { say('p9 N');  }  # catastrophic shape
    if ('aaaaaaaaaaaaaaab'  ~~ /(a+)+$/)       { say('p10 Y'); } else { say('p10 N'); }  # nested +, fails -> N
    if ('abcabcabc'         ~~ /((a|b|c)*)*$/)  { say('p11 Y'); } else { say('p11 N'); }  # alt under nested *
    if (''                  ~~ /(.*)*(.*)*$/)   { say('p12 Y'); } else { say('p12 N'); }  # empty subj, eps-heavy
    say('eps done');
}
RAKU
RK_NFA_BB=0 timeout 30 "$SCRIP" --interp "$TMP/eps.raku" </dev/null >"$TMP/eps_o.out" 2>/dev/null; rc_po=$?
RK_NFA_BB=1 timeout 30 "$SCRIP" --interp "$TMP/eps.raku" </dev/null >"$TMP/eps_b.out" 2>/dev/null; rc_pb=$?
if [ "$rc_po" -ne 0 ]; then echo "FAIL: eps oracle rc=$rc_po"; exit 1; fi
if [ "$rc_pb" -ne 0 ]; then echo "FAIL: eps WALKER rc=$rc_pb (epsilon-loop crash regressed — the (node,pos) memo is broken)"; exit 1; fi
if ! tail -1 "$TMP/eps_o.out" | grep -q 'eps done'; then echo "FAIL: eps oracle did not complete"; exit 1; fi
if diff -q "$TMP/eps_o.out" "$TMP/eps_b.out" >/dev/null; then
    echo "PASS: $(grep -c ' [YN]$' "$TMP/eps_o.out") epsilon-loop probes — walker terminates + == oracle (no SIGSEGV, no exponential blowup)"
else
    echo "FAIL: eps-loop walker diverges from oracle"
    diff "$TMP/eps_o.out" "$TMP/eps_b.out" | head -40
    exit 1
fi

# RK-NFA-3 — capture recording on the IR-graph path ($0/$1 positional, $<name> named).
# Same lowered program; re_capture/re_named_capture read g_raku_match, populated from the
# IR-graph exec (raku_nfa_bb_exec) under RK_NFA_BB=1. Must equal the parallel-NFA oracle.
cat > "$TMP/cap.raku" << 'RAKU'
sub main() {
    my $s = 'John Smith, age 42';
    if ($s ~~ /([A-Za-z]+)/)                 { say($0); } else { say('FAIL p1'); }
    if ($s ~~ /([A-Za-z]+) ([A-Za-z]+)/)     { say($0); say($1); } else { say('FAIL p2'); }
    if ($s ~~ /<first>([A-Za-z]+) <last>([A-Za-z]+)/) { say($<first>); say($<last>); } else { say('FAIL named'); }
    my $t = 'score: 99 points';
    if ($t ~~ /<num>([0-9]+)/)               { say($<num>); } else { say('FAIL num'); }
    my $u = 'hello';
    if ($u ~~ /([0-9]+)/)                    { say('FAIL nomatch'); } else { say('no match ok'); }
    say('cap done');
}
RAKU
RK_NFA_BB=0 timeout 20 "$SCRIP" --interp "$TMP/cap.raku" </dev/null >"$TMP/cap_o.out" 2>/dev/null; rc_co=$?
RK_NFA_BB=1 timeout 20 "$SCRIP" --interp "$TMP/cap.raku" </dev/null >"$TMP/cap_b.out" 2>/dev/null; rc_cb=$?
if [ "$rc_co" -ne 0 ] || [ "$rc_cb" -ne 0 ]; then echo "FAIL: capture battery rc oracle=$rc_co irbb=$rc_cb"; exit 1; fi
if ! tail -1 "$TMP/cap_o.out" | grep -q 'cap done'; then echo "FAIL: capture oracle did not complete"; exit 1; fi
if diff -q "$TMP/cap_o.out" "$TMP/cap_b.out" >/dev/null; then
    echo "PASS: captures (\$0/\$1/\$<name>) — IR-graph spans == parallel-NFA oracle (byte-identical)"
    exit 0
else
    echo "FAIL: IR-graph captures diverge from oracle"
    diff "$TMP/cap_o.out" "$TMP/cap_b.out" | head -40
    exit 1
fi
