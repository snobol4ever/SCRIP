# rk_junction_nest.raku — RK-BB-4d part 1: mixed-flavor NESTED junctions.
# Self-delimiting EOT terminator lets the collapse skip a nested junction as
# one opaque span instead of truncating at its first SOH (the old SOH-leak).
sub main() {
    my $x = 50;
    if ($x == (10 & (50 | 60 | 70))) { say('shouldnt-A'); }
    if ($x == (50 & (50 | 60)))      { say('all-wraps-any'); }   # ⇒ all-wraps-any
    if ($x == (10 | (50 & 50 & 50))) { say('any-wraps-all'); }   # ⇒ any-wraps-all
    if ($x == (10 | (50 & 60)))      { say('shouldnt-D'); }
    if ($x == ((50 | 60) & 50))      { say('any-first-in-all'); } # ⇒ any-first-in-all
}
