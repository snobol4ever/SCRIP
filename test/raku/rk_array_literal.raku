# rk_array_literal.raku — comma-list array initializer `my @a = e1, e2, ...`
sub main() {
    # bare comma-list init, then iterate
    my @a = 10, 20, 30;
    for @a -> $i { say($i); }

    # comma-init feeding reverse
    my @b = 1, 2, 3, 4;
    my @r = reverse(@b);
    for @r -> $x { say($x); }

    # comma-init feeding sum and join
    my @c = 5, 5, 10;
    say(sum(@c));
    say(join("+", @c));

    # mixed-type comma-init
    my @d = "a", "b", "c";
    for @d -> $s { say($s); }
}
