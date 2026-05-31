# rk_for_array_simple.raku — RK-BB-3a probe: iterate Icon-list (DT_DATA) via `for @arr -> $v`.
# Stripped-down precursor to rk_map_grep_sort24; flips green when the BB_ITERATE
# polymorphic substrate (DT_DATA-list branch) lands.
sub main() {
    my @x = '';
    push(@x, 10); push(@x, 20); push(@x, 30);
    for @x -> $v { say($v); }
}
