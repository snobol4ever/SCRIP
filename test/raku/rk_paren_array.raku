# rk_paren_array.raku — RK-BB-5.4b: parenthesized array literal `my @a = (e1, e2, ...)`
# Initializer-only paren-list (mirrors the 5.3 bare comma-list): net-zero new conflicts.
# Single-element paren stays scalar; bare comma-list still works alongside.
sub main() {
    my @a = (1, 2, 3);
    for @a -> $x { say($x); }
    my @t = (10, 20, 30);
    say(@t.sum);
    say(@t.elems);
    my @bare = 7, 8;
    say(@bare.sum);
    my $s = (42);
    say($s);
}
