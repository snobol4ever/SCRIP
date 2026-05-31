# rk_for_array_underscore.raku — RK-BB-3b/c diagnostic probe.
# Hand-desugared map-equivalent: does $_ bind correctly under `for @arr -> $_`?
# If this passes, the bug is isolated to lower_raku_map_or_grep's divergence
# from the for-loop+push pattern (hypothesis a or b in handoff).
# If this also fails, $_ itself is broken independent of map/grep.
sub main() {
    my @nums = '';
    push(@nums, 1); push(@nums, 2); push(@nums, 3);
    my @r = '';
    for @nums -> $_ { push(@r, $_ * 2); }
    for @r -> $x { say($x); }
}
