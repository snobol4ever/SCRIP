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
