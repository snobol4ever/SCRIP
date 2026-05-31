# rk_seq_consumers_arr.raku — RK-BB-5.x: reverse/unique/sum/join on @array args
sub main() {
    my @a = "";
    push(@a, 3); push(@a, 1); push(@a, 2); push(@a, 1);

    # reverse an array variable
    my @r = reverse(@a);
    for @r -> $i { say($i); }

    # unique an array variable
    my @u = unique(@a);
    for @u -> $x { say($x); }

    # sum an array variable
    say(sum(@a));

    # join an array variable
    say(join("-", @a));
}
