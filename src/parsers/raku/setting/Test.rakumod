unit module Test;
# Copyright (C) 2007 - 2026 Yet Another Society
# Rakudo's lib/Test.rakumod (rakudo 2026.05), Artistic License 2.0, adapted for SCRIP's Raku setting
# (ARCH-RAKU-TO-100.md section 1.2 (c), CEO-2044): the nqp:: operations and the timing hooks are removed,
# the multi candidates are one routine each, and the TAP it prints is Rakudo's byte for byte.

my @vars;
my $indents = "";
my $num_of_tests_run = 0;
my $num_of_tests_failed = 0;
my $todo_upto_test_num = 0;
my $todo_reason = '';
my $num_of_tests_planned;
my $no_plan = 1;
my $subtest_level = 0;
my $subtest_todo_reason = '';
my $pseudo_fails = 0;
my $done_testing_has_been_run = 0;

my class X::SubtestsSkipped is Exception {}

sub _diag($message, $force-stderr = False, $force-informative = False) {
    my $is_todo = !$force-stderr && !$force-informative && ($subtest_todo_reason || $num_of_tests_run <= $todo_upto_test_num);
    my $str-message = "$indents# " ~ $message.Str.split("\n").join("\n$indents# ");
    if $is_todo || $force-informative { say $str-message } else { note $str-message }
}

sub _istype(Mu $v, Mu $t) { ($v ~~ $t) ?? True !! ($v.isa($t) || $v.does($t) ?? True !! False) }

sub _bool(Mu $c) { $c ~~ Order ?? ($c === Same ?? False !! True) !! ?$c }

sub proclaim($cond is copy, $desc is copy, $unescaped-prefix = '') {
    $cond = _bool($cond);
    $num_of_tests_run = $num_of_tests_run + 1;
    my $tap = $indents;
    unless $cond {
        $tap ~= "not ";
        $num_of_tests_failed = $num_of_tests_failed + 1 unless $num_of_tests_run <= $todo_upto_test_num;
        $pseudo_fails = $pseudo_fails + 1 if $subtest_todo_reason;
    }
    $desc = $desc ?? $desc.Str.split('#').join(' \\#').split("\n").join("\n$indents# ") !! '';
    if $todo_reason && $num_of_tests_run <= $todo_upto_test_num {
        $tap ~= "ok $num_of_tests_run - $unescaped-prefix$desc$todo_reason";
    }
    elsif !$cond && $subtest_todo_reason {
        $tap ~= "ok $num_of_tests_run - $unescaped-prefix$desc$subtest_todo_reason";
    }
    else {
        $tap ~= "ok $num_of_tests_run - $unescaped-prefix$desc";
    }
    say $tap;
    unless $cond {
        _diag($desc ?? "Failed test '$desc'" !! "Failed test");
    }
    $todo_reason = '' if $todo_upto_test_num == $num_of_tests_run;
    $cond
}

sub plan($number_of_tests?, :$skip-all) is export {
    if $skip-all.defined {
        say $indents ~ "1..0 # Skipped: $skip-all";
        exit unless $subtest_level;
        $done_testing_has_been_run = 1;
        $num_of_tests_failed = 0;
        $num_of_tests_planned = 0;
        $num_of_tests_run = 0;
        X::SubtestsSkipped.new.throw;
    }
    if $number_of_tests ~~ Whatever {
        $no_plan = 1;
    }
    else {
        $num_of_tests_planned = $number_of_tests;
        $no_plan = 0;
        say $indents ~ '1..' ~ $number_of_tests;
    }
}

sub pass($desc = '') is export { proclaim(1, $desc) }

sub flunk($reason = '') is export { proclaim(0, $reason) }

sub ok(Mu $cond, $desc = '') is export { proclaim($cond, $desc) }

sub nok(Mu $cond, $desc = '') is export { proclaim(_bool($cond) ?? False !! True, $desc) }

sub is(Mu $got, Mu $expected, $desc = '') is export {
    my $ok;
    if $expected.defined {
        if $got.defined {
            my $test = $got.Str eq $expected.Str;
            $ok = proclaim($test, $desc);
            if !$test {
                _diag "expected: '$expected'\n" ~ "     got: '$got'";
            }
        }
        else {
            $ok = proclaim(False, $desc);
            _diag "expected: '$expected'\n" ~ "     got: ($got.^name())";
        }
    }
    else {
        if $got.defined {
            $ok = proclaim(False, $desc);
            _diag "expected: ($expected.^name())\n" ~ "     got: '$got'";
        }
        else {
            my $test = $got === $expected;
            $ok = proclaim($test, $desc);
            if !$test {
                _diag "expected: ($expected.^name())\n" ~ "     got: ($got.^name())";
            }
        }
    }
    $ok
}

sub isnt(Mu $got, Mu $expected, $desc = '') is export {
    my $ok;
    if $expected.defined {
        my $test = !$got.defined || $got.Str ne $expected.Str;
        $ok = proclaim($test, $desc);
        if !$test {
            _diag "twice: '$got'";
        }
    }
    else {
        my $test = $got.defined || !($got === $expected);
        $ok = proclaim($test, $desc);
        if !$test {
            _diag "twice: ($got.^name())";
        }
    }
    $ok
}

sub cmp-ok(Mu $got, $op, Mu $expected, $desc = '') is export {
    my $ok;
    my $known = 1;
    my $test = False;
    if _is-code($op) { $test = $op($got, $expected) }
    elsif $op eq '==' { $test = $got == $expected }
    elsif $op eq '!=' { $test = $got != $expected }
    elsif $op eq '<' { $test = $got < $expected }
    elsif $op eq '<=' { $test = $got <= $expected }
    elsif $op eq '>' { $test = $got > $expected }
    elsif $op eq '>=' { $test = $got >= $expected }
    elsif $op eq 'eq' { $test = $got eq $expected }
    elsif $op eq 'ne' { $test = $got ne $expected }
    elsif $op eq 'lt' { $test = $got lt $expected }
    elsif $op eq 'le' { $test = $got le $expected }
    elsif $op eq 'gt' { $test = $got gt $expected }
    elsif $op eq 'ge' { $test = $got ge $expected }
    elsif $op eq '===' { $test = $got === $expected }
    elsif $op eq 'eqv' { $test = $got eqv $expected }
    elsif $op eq '~~' { $test = ($got ~~ $expected) ?? True !! False }
    elsif $op eq '=~=' { $test = $got =~= $expected }
    else { $known = 0 }
    if $known {
        $ok = proclaim($test, $desc);
        if !$ok {
            _diag "expected: " ~ $expected.raku ~ "\n" ~ " matcher: 'infix:<$op>'\n" ~ "     got: " ~ $got.raku;
        }
    }
    else {
        $ok = proclaim(False, $desc);
        _diag "Could not use '$op.raku()' as a comparator. If you are trying to use a meta operator, pass it as a Callable instead of a string: \&[$op]";
    }
    $ok
}

sub bail-out($desc?) is export {
    say $desc ?? "Bail out! $desc" !! "Bail out!";
    $done_testing_has_been_run = 1;
    exit 255;
}

sub _is-approx-calculate($got, $expected, $abs-tol, $rel-tol, $desc) {
    my $abs-tol-ok = True;
    my $rel-tol-ok = True;
    my $abs-tol-got;
    my $rel-tol-got;
    if $abs-tol.defined {
        $abs-tol-got = abs($got - $expected);
        $abs-tol-ok = $abs-tol-got <= $abs-tol;
    }
    if $rel-tol.defined {
        my $max = max($got.abs, $expected.abs);
        if $max {
            $rel-tol-got = abs($got - $expected) / $max;
            $rel-tol-ok = $rel-tol-got <= $rel-tol;
        }
        else {
            $rel-tol-got = 0;
        }
    }
    my $ok = proclaim($abs-tol-ok && $rel-tol-ok, $desc);
    unless $ok {
        _diag "    expected approximately: $expected\n" ~ "                       got: $got";
        unless $abs-tol-ok {
            _diag "maximum absolute tolerance: $abs-tol\n" ~ "actual absolute difference: $abs-tol-got";
        }
        unless $rel-tol-ok {
            _diag "maximum relative tolerance: $rel-tol\n" ~ "actual relative difference: $rel-tol-got";
        }
    }
    $ok
}

sub is-approx($got, $expected, $third?, $fourth?, :$abs-tol, :$rel-tol) is export {
    my $desc = '';
    my $atol = $abs-tol;
    my $rtol = $rel-tol;
    if $third.defined && $third ~~ Numeric {
        $atol = $third;
        $desc = $fourth if $fourth.defined;
    }
    elsif $third.defined {
        $desc = $third;
    }
    if !$atol.defined && !$rtol.defined {
        if $expected.abs < 1e-6 { $atol = 1e-5 } else { $rtol = 1e-6 }
    }
    _is-approx-calculate($got, $expected, $atol, $rtol, $desc)
}

sub todo($reason, $count = 1) is export {
    $todo_upto_test_num = $num_of_tests_run + $count;
    $todo_reason = " # TODO $reason";
}

sub skip($reason?, $count = 1) is export {
    if !$reason.defined {
        proclaim(1, '', "# SKIP");
        return;
    }
    die "skip() was passed a non-integer number of tests.  Did you get the arguments backwards or use a non-integer number?" if $count !~~ Int;
    my $i = 1;
    while $i <= $count { proclaim(1, $reason, "# SKIP "); $i = $i + 1; }
}

sub skip-rest($reason = '<unknown>') is export {
    die "A plan is required in order to use skip-rest" if $no_plan;
    skip($reason, $num_of_tests_planned - $num_of_tests_run);
}

sub _init_vars {
    $num_of_tests_run = 0;
    $num_of_tests_failed = 0;
    $todo_upto_test_num = 0;
    $todo_reason = '';
    $num_of_tests_planned = Any;
    $no_plan = 1;
    $done_testing_has_been_run = 0;
    $pseudo_fails = 0;
    $subtest_todo_reason = '';
}

sub _push_vars {
    @vars.push: [$num_of_tests_run, $num_of_tests_failed, $todo_upto_test_num, $todo_reason, $num_of_tests_planned, $no_plan, $done_testing_has_been_run, $pseudo_fails, $subtest_todo_reason];
}

sub _pop_vars {
    my @v = @vars.pop.list;
    $num_of_tests_run = @v[0];
    $num_of_tests_failed = @v[1];
    $todo_upto_test_num = @v[2];
    $todo_reason = @v[3];
    $num_of_tests_planned = @v[4];
    $no_plan = @v[5];
    $done_testing_has_been_run = @v[6];
    $pseudo_fails = @v[7];
    $subtest_todo_reason = @v[8];
}

sub _subtest-open($desc) {
    _diag("Subtest" ~ ($desc ?? ": " ~ $desc !! ''), False, True);
    my $parent_todo = $todo_reason || $subtest_todo_reason;
    _push_vars();
    _init_vars();
    $subtest_todo_reason = $parent_todo;
    $indents ~= "    ";
    $subtest_level = $subtest_level + 1;
}

sub _subtest-close($desc) {
    $subtest_level = $subtest_level - 1;
    done-testing() unless $done_testing_has_been_run;
    my $status = $num_of_tests_failed == 0 && $num_of_tests_planned == $num_of_tests_run && $pseudo_fails == 0;
    _pop_vars();
    $indents = $indents.chop(4);
    proclaim($status, $desc);
}

sub subtest($first?, $second?, *%named) is export {
    my $code;
    my $desc = '';
    if %named.elems == 1 {
        $desc = %named.keys[0];
        $code = %named.values[0];
    }
    elsif $first ~~ Pair {
        $desc = $first.key;
        $code = $first.value;
    }
    elsif _is-code($first) {
        $code = $first;
        $desc = $second if $second.defined;
    }
    else {
        $desc = $first;
        $code = $second;
    }
    _subtest-open($desc);
    my @r = _catch-call($code);
    my $ok = _subtest-close($desc);
    @r[1].rethrow if @r[0] && !(@r[1] ~~ X::SubtestsSkipped);
    $ok
}

sub diag($message) is export {
    _diag($message, True, False);
}

sub isa-ok(Mu $var, Mu $type, $desc?) is export {
    my $d = $desc.defined ?? $desc !! "The object is-a '$type.raku()'";
    my $ok = proclaim($type ~~ Str ?? $var.isa($type) !! _istype($var, $type), $d);
    _diag('Actual type: ' ~ $var.^name) unless $ok;
    $ok
}

sub does-ok(Mu $var, Mu $type, $desc?) is export {
    my $d = $desc.defined ?? $desc !! "The object does role '$type.raku()'";
    my $ok = proclaim($var.does($type), $d);
    _diag('Type: ' ~ $var.^name ~ " doesn't do role " ~ $type.raku) unless $ok;
    $ok
}

sub can-ok(Mu $var, Str $meth, $desc?) is export {
    my $d = $desc.defined ?? $desc !! ($var.defined ?? "An object of type '" !! "The type '") ~ "$var.WHAT.raku()' can do the method '$meth'";
    proclaim($var.^can($meth), $d)
}

sub like($got, $expected, $desc?) is export {
    my $d = $desc.defined ?? $desc !! "text matches $expected.raku()";
    my $ok = proclaim(($got.Str ~~ $expected) ?? True !! False, $d);
    _diag "expected a match with: $expected.raku()\n" ~ "                  got: $got.raku()" unless $ok;
    $ok
}

sub unlike($got, $expected, $desc?) is export {
    my $d = $desc.defined ?? $desc !! "text does not match $expected.raku()";
    my $ok = proclaim(($got.Str ~~ $expected) ?? False !! True, $d);
    _diag "expected no match with: $expected.raku()\n" ~ "                   got: $got.raku()" unless $ok;
    $ok
}

sub dies-ok($code, $reason = '') is export {
    my $death = 1;
    try {
        $code();
        $death = 0;
    }
    proclaim($death, $reason)
}

sub lives-ok($code, $reason = '') is export {
    my $err;
    {
        $code();
        CATCH { default { $err = $_ } }
    }
    my $ok = proclaim(!$err.defined, $reason);
    _diag($err) unless $ok;
    $ok
}

sub eval-dies-ok($code, $reason = '') is export {
    my $death = 1;
    try {
        EVAL $code;
        $death = 0;
    }
    proclaim($death, $reason)
}

sub eval-lives-ok($code, $reason = '') is export {
    my $err;
    {
        EVAL $code;
        CATCH { default { $err = $_ } }
    }
    my $ok = proclaim(!$err.defined, $reason);
    _diag("Error: $err") unless $ok;
    $ok
}

sub is-deeply(Mu $got, Mu $expected, $reason = '') is export {
    my $g = $got ~~ Seq ?? $got.cache !! $got;
    my $e = $expected ~~ Seq ?? $expected.cache !! $expected;
    my $test = $g eqv $e;
    my $ok = proclaim($test, $reason);
    if !$test {
        _diag "expected: " ~ $e.raku ~ "\n" ~ "     got: " ~ $g.raku;
    }
    $ok
}

sub _throws-like-report($died, $ex, $ex_type, $desc, %matcher) {
    _subtest-open($desc);
    plan 2 + %matcher.keys.elems;
    my $msg = 'code dies';
    if $died {
        pass $msg;
        my $type_ok = _istype($ex, $ex_type);
        ok $type_ok, "right exception type ($ex_type.^name())";
        if $type_ok {
            for %matcher.kv -> $k, $v {
                my $got = $ex."$k"();
                my $ok = ($got ~~ $v) ?? True !! False;
                ok $ok, ".$k matches $v.gist()";
                unless $ok {
                    _diag "Expected: " ~ ($v ~~ Str ?? $v !! $v.raku) ~ "\nGot:      $got";
                }
            }
        }
        else {
            _diag "Expected: $ex_type.^name()\n" ~ "Got:      $ex.^name()\n" ~ "Exception message: $ex.message()";
            skip 'wrong exception type', %matcher.elems;
        }
    }
    else {
        flunk $msg;
        skip 'Code did not die, can not check exception', 1 + %matcher.elems;
    }
    _subtest-close($desc);
}

sub _is-code($c) { ?__rk_is_code($c) }

sub _catch-call($code) {
    my @r = 0, Any, Any;
    {
        @r[2] = $code();
        CATCH { default { @r[0] = 1; @r[1] = $_; } }
    }
    @r
}

sub _catch-eval($code) {
    my @r = 0, Any, Any;
    {
        @r[2] = EVAL $code;
        CATCH { default { @r[0] = 1; @r[1] = $_; } }
    }
    @r
}

sub _catch-sink($res) {
    my @r = 0, Any, Any;
    {
        $res.sink;
        CATCH { default { @r[0] = 1; @r[1] = $_; } }
    }
    @r
}

sub throws-like($code, $ex_type, $reason?, *%matcher) is export {
    my @r = _is-code($code) ?? _catch-call($code) !! _catch-eval($code);
    _throws-like-report(@r[0], @r[1], $ex_type, $reason // "did we throws-like $ex_type.^name()?", %matcher);
}

sub fails-like($code, $ex-type, $reason?, *%matcher) is export {
    my $desc = $reason // "did we fails-like $ex-type.^name()?";
    my @c = _is-code($code) ?? _catch-call($code) !! _catch-eval($code);
    my $threw = @c[1];
    my $res = @c[2];
    _subtest-open($desc);
    plan 2;
    if @c[0] {
        my $m = "expected code to fail but it threw {$threw.^name} instead";
        flunk $m;
        skip $m;
    }
    else {
        isa-ok $res, Failure, 'code returned a Failure';
        my @s = _catch-sink($res);
        _throws-like-report(@s[0], @s[1], $ex-type, 'Failure threw when sunk', %matcher);
    }
    _subtest-close($desc);
}

sub done-testing() is export {
    my $return = True;
    $done_testing_has_been_run = 1;
    if $no_plan {
        $num_of_tests_planned = $num_of_tests_run;
        say $indents ~ "1..$num_of_tests_planned";
    }
    if ($num_of_tests_planned or $num_of_tests_run) && ($num_of_tests_planned != $num_of_tests_run) {
        _diag("You planned $num_of_tests_planned test" ~ ($num_of_tests_planned == 1 ?? '' !! 's') ~ ", but ran $num_of_tests_run");
        $return = False;
    }
    if $num_of_tests_failed && !$subtest_todo_reason {
        _diag("You failed $num_of_tests_failed test" ~ ($num_of_tests_failed == 1 ?? '' !! 's') ~ " of $num_of_tests_run");
        $return = False;
    }
    $return
}

END {
    done-testing() if !$done_testing_has_been_run && !$no_plan;
    exit($num_of_tests_failed min 254) if $num_of_tests_failed > 0;
    exit(255) if !$no_plan && ($num_of_tests_planned or $num_of_tests_run) && $num_of_tests_planned != $num_of_tests_run;
}
