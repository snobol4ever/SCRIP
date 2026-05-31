# rk_junction_prec.raku — RK-BB-4d junction precedence + edges.
# Junction infix |/& binds TIGHTER than comparison (real Raku semantics),
# so `$x == 1|2|5` parses as `$x == any(1,2,5)`. Also exercises var
# round-trip, string-relop collapse, and mixed-flavor nesting.
sub main() {
    my $x = 5;
    if ($x == 1 | 2 | 5)  { say('unparen-any'); }    # ⇒ unparen-any
    if ($x == 1 | 2 | 3)  { say('shouldnt-1'); }
    if ($x == 5 & 5 & 5)  { say('unparen-all'); }    # ⇒ unparen-all
    my $j = any(2, 5, 8);
    if ($x == $j)         { say('var-roundtrip'); }  # ⇒ var-roundtrip
    my $s = 'b';
    if ($s eq any('a','b')) { say('str-collapse'); } # ⇒ str-collapse
    if ($x == any(1, all(5, 5))) { say('mixed-nest'); } # ⇒ mixed-nest
    my $y = 4;
    if ($y == none(1, (2 | 3))) { say('none-nest'); }   # ⇒ none-nest
}
