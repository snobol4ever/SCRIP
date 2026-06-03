#!/usr/bin/env bash
# test_gate_raku_nfa_oracle.sh — RK-NFA-1 oracle-equivalence gate (Raku, mode-2, ISOLATED).
#   Proves the IR_NFA_* graph backtracking walk (raku_nfa_bb.c nfa_bt_ir, selected by RK_NFA_BB=1)
#   returns verdicts byte-identical to the parallel-NFA oracle (raku_re.c raku_nfa_exec, RK_NFA_BB=0)
#   on a battery exercising every IR_NFA_* kind: CHAR ANY CLASS SPLIT EPS BOL EOL CAP_OPEN CAP_CLOSE ACCEPT.
#   The `~~` smartmatch operator is lowered for Raku in lower.c (TT_SMATCH -> IR_CALL re_match);
#   re_match honors RK_NFA_BB, so the SAME lowered program drives both matchers.
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
