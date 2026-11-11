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
