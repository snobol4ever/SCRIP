# rk_re38.raku — RK-NFA-2: cset/anchor edge-case verdicts (negated shorthands, enumerated
#   ranges + negation, mixed shorthands inside [...], BOL/EOL anchors). The IR_NFA_* graph walk
#   (RK_NFA_BB=1) and the parallel-NFA oracle (RK_NFA_BB=0) agree on every line here; proven
#   byte-identical by scripts/test_gate_raku_nfa_oracle.sh (RK-NFA-2 section).
#
# Note (the | LTM vs || ordered seam): single `|` lowers to an NFA SPLIT; the two engines agree
#   on VERDICT everywhere (this file is verdict-only). They diverge only on overlapping-`|` match
#   EXTENT (oracle = leftmost-longest `|`-LTM; backtracking walker = leftmost-first `||`-ordered),
#   which is the Phase-2 boundary and is not exercised here.

sub main() {
    # ---- negated shorthand csets: \D \W \S ----
    if ('abc'    ~~ /\D+/) { say('nondigit ok');    } else { say('FAIL \D+'); }
    if ('123'    ~~ /\D/)  { say('FAIL \D digits'); } else { say('digits not \D ok'); }
    if ('a b'    ~~ /\S/)  { say('nonspace ok');    } else { say('FAIL \S'); }
    if (' '      ~~ /\S/)  { say('FAIL \S space');  } else { say('space not \S ok'); }
    if ('a-b'    ~~ /\W/)  { say('dash nonword ok');} else { say('FAIL \W dash'); }
    if ('a_b'    ~~ /\W/)  { say('FAIL \W word');   } else { say('underscore word ok'); }

    # ---- enumerated csets: multi-range, negated ----
    if ('hello5' ~~ /[a-z0-9]+/) { say('multirange ok');  } else { say('FAIL [a-z0-9]'); }
    if ('ABC'    ~~ /[^0-9]+/)   { say('neg range ok');   } else { say('FAIL [^0-9]'); }
    if ('999'    ~~ /[^0-9]/)    { say('FAIL [^0-9] dig');} else { say('all digit neg ok'); }
    if ('Hi9'    ~~ /[A-Za-z]/)  { say('two ranges ok');  } else { say('FAIL [A-Za-z]'); }

    # ---- mixed shorthand inside [...] ----
    if ('a 1'    ~~ /[\d\s]/) { say('mixed cls ok');   } else { say('FAIL [\d\s]'); }
    if ('xyz'    ~~ /[\d\s]/) { say('FAIL [\d\s] none');} else { say('no digit/space ok'); }
    if ('a-z'    ~~ /[\w-]+/) { say('word+dash ok');   } else { say('FAIL [\w-]'); }

    # ---- BOL / EOL anchors ----
    if ('hello'  ~~ /^h/)      { say('BOL ok');         } else { say('FAIL ^h'); }
    if ('hello'  ~~ /^e/)      { say('FAIL ^e midstr'); } else { say('not at BOL ok'); }
    if ('hello'  ~~ /o$/)      { say('EOL ok');         } else { say('FAIL o$'); }
    if ('hello'  ~~ /h$/)      { say('FAIL h$ midstr'); } else { say('not at EOL ok'); }
    if ('hello'  ~~ /^hello$/) { say('full anchor ok'); } else { say('FAIL ^hello$'); }
    if ('hellox' ~~ /^hello$/) { say('FAIL anchor long');} else { say('anchor mismatch ok'); }
    if (''       ~~ /^$/)      { say('empty anchor ok');} else { say('FAIL ^$ empty'); }

    say('rk_re38 ok');
}
