# rk_join.raku — RK-BB-5.2: join() as a list Seq consumer
sub main() {
    say(join(",", 1, 2, 3));
    say(join("-", "a", "b", "c"));
    say(join("", 4, 5, 6));
    # compose with reverse: join the reversed list
    say(join(",", reverse(1, 2, 3)));
}
