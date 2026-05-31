# rk_reverse.raku — RK-BB-5.0: reverse() as an eager-drain Seq consumer
sub main() {
    # reverse a literal list, iterated directly
    for reverse(3, 5, 7) -> $i { say($i); }

    # reverse into an array, then iterate
    my @r = reverse(10, 20, 30, 40);
    for @r -> $x { say($x); }

    # reverse a single-element list is a no-op
    for reverse(99) -> $y { say($y); }

    # reverse of strings preserves element order reversal
    my @w = reverse('apple', 'banana', 'cherry');
    for @w -> $s { say($s); }
}
