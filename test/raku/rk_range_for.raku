# rk_range_for.raku — RAKU-BB RK-BB-1: lazy inclusive range `for 1..5 -> $i`
# Drives the shared BB_TO_BY generator via SM_BB_SWITCH(SM_BBSW_RK_GEN).
sub main() {
    my $sum = 0;
    for 1..5 -> $i {
        say($i);
        $sum = $sum + $i;
    }
    say($sum);
}
