unit module Test::Util;
# roast's packages/Test-Helpers/lib/Test/Util.rakumod, Artistic License 2.0, adapted for SCRIP's Raku setting
# (ARCH-RAKU-TO-100.md section 1.2 (c), CEO-2044): get_out runs the child through the __rk_run_child leaf
# (a process is a kernel entry, NOT-A-BOX), the multi candidates are one routine each.
use Test;

sub get_out($code, $input = '', :@args) is export {
    my @r = __rk_run_child($code, $input, @args);
    my %out;
    %out<status> = @r[0];
    %out<out> = @r[1];
    %out<err> = @r[2];
    %out
}

sub is_run($code, $second?, $third?, $fourth?, :@args) is export {
    my $input = '';
    my %expected;
    my $name = '';
    if $second ~~ Hash {
        %expected = $second;
        $name = $third if $third.defined;
    }
    else {
        $input = $second if $second.defined;
        %expected = $third if $third ~~ Hash;
        $name = $fourth if $fourth.defined;
    }
    my %got = get_out($code, $input, :@args);
    my $ok = True;
    my $tests_aggregated = 0;
    my @diag_q;
    %expected<status> = 0 if !(%expected<status>:exists) && (!(%expected<err>:exists) || (%expected<err> ~~ Str && %expected<err> eq ''));
    for <status out err> -> $attr {
        next if !(%expected{$attr}:exists);
        my $attr_good = (%got{$attr} ~~ %expected{$attr}) ?? True !! False;
        if !$attr_good {
            @diag_q.push("     got $attr: " ~ %got{$attr}.raku);
            @diag_q.push("expected $attr: " ~ %expected{$attr}.raku) if %expected{$attr} ~~ Str || %expected{$attr} ~~ Num;
        }
        $ok = $ok && $attr_good;
        $tests_aggregated++;
    }
    if $tests_aggregated == 0 {
        skip 'nothing tested', 1;
        return;
    }
    ok ?$ok, $name;
    diag $_ for @diag_q;
    return;
}

sub is-eqv(Mu $got, Mu $expected, $desc) is export {
    my $g = $got ~~ Seq ?? $got.cache !! $got;
    my $e = $expected ~~ Seq ?? $expected.cache !! $expected;
    my $test = $g eqv $e;
    my $ok = ok ?$test, $desc;
    if !$test {
        diag "expected: " ~ $e.raku ~ "\n" ~ "     got: " ~ $g.raku;
    }
    $ok
}

sub group-of($p) is export {
    my $plan = $p.key;
    my $desc = $p.value.key;
    my $tests = $p.value.value;
    subtest $desc, { plan $plan; $tests() };
}
