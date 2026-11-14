sub squish(+values, |c) { values.squish(|c) }
sub unique(+values, |c) { values.unique(|c) }
multi sub permutations(Int $n) { (^$n).permutations }
multi sub permutations(Iterable \n) { n.permutations }
multi sub combinations(Int $n, Int $k) { (^$n).combinations($k) }
multi sub combinations(Iterable \n, Int $k) { n.combinations($k) }
sub sech($x) { 1e0 / cosh($x) }
sub cosech($x) { 1e0 / sinh($x) }
sub cotanh($x) { 1e0 / tanh($x) }
sub lines($what, |c) { $what.lines(|c) }
multi sub keys(\x) { x.keys }
multi sub values(\x) { x.values }
multi sub kv(\x) { x.kv }
multi sub pairs(\x) { x.pairs }
multi sub elems(\x) { x.elems }
multi sub end(\x) { x.end }
multi sub ords(\x) { x.ords }
multi sub splice(@a) { @a.splice }
multi sub splice(@a, $start) { @a.splice($start) }
multi sub splice(@a, $start, $n, *@new) { @a.splice($start, $n, |@new) }
class IO::Spec::Unix { method canonpath(Str() $p) { return '' unless $p; my $abs = $p.starts-with('/'); my @s = $p.split('/').grep({ $_ ne '' && $_ ne '.' }); while $abs && @s && @s[0] eq '..' { @s.shift }; my $r = @s.join('/'); $abs ?? '/' ~ $r !! ($r eq '' ?? '.' !! $r) }; method catdir(*@parts) { self.canonpath(@parts.join('/')) } }
my $*SPEC = IO::Spec::Unix;
class Supply { has &.gen; method tap(&cb, :$done) { for self.gen.() -> $v { cb($v) }; $done() if $done.defined; Nil }; method list { self.gen.().list }; method Seq { self.gen.() } }
sub floor(\x) { x.floor }
sub ceiling(\x) { x.ceiling }
sub round(\x, $scale?) { $scale.defined ?? (x / $scale + 1/2).floor * $scale !! x.round }
sub truncate(\x) { x.truncate }
multi sub sign(\x) { x.sign }
multi sub log2(\x) { x.log2 }
multi sub log10(\x) { x.log10 }
multi sub sinh(\x) { x.sinh }
multi sub cosh(\x) { x.cosh }
multi sub tanh(\x) { x.tanh }
multi sub asinh(\x) { x.asinh }
multi sub acosh(\x) { x.acosh }
multi sub atanh(\x) { x.atanh }
multi sub asin(\x) { x.asin }
multi sub acos(\x) { x.acos }
multi sub sec(\x) { x.sec }
multi sub cosec(\x) { x.cosec }
multi sub cotan(\x) { x.cotan }
multi sub is-prime(\x) { x.is-prime }
multi sub atan2(\y) { y.atan2 }
multi sub atan2(\y, \x) { y.atan2(x) }
