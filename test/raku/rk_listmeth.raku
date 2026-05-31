# rk_listmeth.raku — RK-BB-5.4a: list-method postfix forms .reverse/.unique/.sum/.elems/.head(N)/.tail(N)
# Scalar-returning methods (.elems/.sum) print directly; list-returning methods are assigned to an
# array then iterated, giving stable one-element-per-line output the .expected file can encode.
sub main() {
    my @a = 3, 1, 2;
    say(@a.elems);
    my @r = @a.reverse;
    for @r -> $x { say($x); }
    my @h = @a.head(2);
    for @h -> $y { say($y); }
    my @t = @a.tail(2);
    for @t -> $z { say($z); }
    my @h1 = @a.head;
    for @h1 -> $p { say($p); }
    my @t1 = @a.tail;
    for @t1 -> $q { say($q); }
    my @b = 5, 5, 7, 7, 9;
    my @u = @b.unique;
    for @u -> $w { say($w); }
    say(@b.sum);
    say("==");
    for @a.reverse -> $r { say($r); }
    for @a.head(2) -> $hh { say($hh); }
}
