# rk_unique_sum.raku — RK-BB-5.1: unique() and sum() as list Seq consumers
sub main() {
    # unique: dedup preserving first occurrence
    for unique(1, 2, 2, 3, 3, 3, 1) -> $i { say($i); }

    # unique into an array, then iterate (strings)
    my @u = unique('a', 'b', 'a', 'c', 'b');
    for @u -> $x { say($x); }

    # sum: integer fold
    say(sum(1, 2, 3, 4, 5));

    # sum: real fold (mixed int/real promotes to real)
    say(sum(1.5, 2.5, 3));
}
