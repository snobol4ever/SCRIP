# rk_junctions.raku — RK-BB-4 junction probe.
# Tests `any`/`all`/`one`/`none` constructors and infix `|`/`&`
# autothreading semantics. Bool-collapse on ω/γ per goal spec.
#
# Today this fails because:
#   1. Parser has no KW_ANY/KW_ALL/KW_ONE/KW_NONE tokens.
#   2. Lexer has no single-char `|`/`&` infix (only `||`/`&&`).
#   3. BB_ALTERNATE mode-2 executor is a no-op stub
#      (bb_exec.c:1618-1620: nd->value=FAIL; return nd->ω).
#   4. bb_alt.cpp / bb_gen_alt.cpp mode-4 templates are stubs
#      (only emit α→γ, β→ω passthrough jumps).
# Marked failing in the gate suite; flips green when RK-BB-4 lands.
sub main() {
    my $x = 3;
    if ($x == any(1, 2, 3))  { say('any-hit');  }   # ⇒ any-hit
    if ($x == any(7, 8, 9))  { say('any-miss-shouldnt-print'); }
    if ($x == all(3, 3, 3))  { say('all-hit');  }   # ⇒ all-hit
    if ($x == all(3, 3, 4))  { say('all-miss-shouldnt-print'); }
    if ($x == none(7, 8, 9)) { say('none-hit'); }   # ⇒ none-hit
    if ($x == one(1, 3, 5))  { say('one-hit');  }   # ⇒ one-hit
    if ($x == one(3, 3, 5))  { say('one-miss-shouldnt-print'); }
    # infix junctions (single-char | and &)
    if ($x == (1 | 2 | 3))   { say('pipe-hit'); }   # ⇒ pipe-hit
    if ($x == (3 & 3 & 3))   { say('amp-hit');  }   # ⇒ amp-hit
}
