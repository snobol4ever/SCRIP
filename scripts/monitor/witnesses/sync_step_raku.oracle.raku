# sync_step_raku.oracle.raku -- ORACLE-SIDE twin of sync_step_raku.raku.
# MONITOR-BINARY-DESIGN.md THE ORACLE-SIDE BRIDGES ARE EACH HQ'S OWN: Rakudo 2022.12/MoarVM has no trace hook and no
# installed Trace module (zef is not on the box), so per that section's second option this is a hand-instrumented
# copy of the witness with note() calls injected at exactly the points SCRIP's own raku plug (lower_raku.c
# trace_stmt_wrap/trace_call_wrap/trace_value_prep) fires: one LABEL before each top-level statement, one CALL at
# a called sub's entry, one RETURN carrying its result just before returning, one VALUE after each scalar store.
# note() writes to $*ERR only -- real stdout (`say $s`) is untouched, so this is monitor-safe by construction the
# same way SCRIP's own --trace is: the traced run's stdout is byte-identical to the untraced witness's.
# raku_oracle_bridge.py runs this file, parses the "EVT ..." lines off stderr, and replays them on the wire.
# Keep this file's event sequence byte-for-byte in step with sync_step_raku.raku -- if the witness changes, this
# does too, by hand, since Rakudo gives us no way to derive it automatically.
sub __ev(*@a) { note 'EVT ' ~ @a.join(' ') }

__ev('CALL', 'main');
__ev('LABEL', 1);
my $s = 0;
__ev('VALUE', 's', 'INT', $s);

sub add($a, $b) {
    __ev('CALL', 'add');
    my $r = $a + $b;
    __ev('RETURN', 'add', 'INT', $r);
    return $r;
}

__ev('LABEL', 3);
for 1..3 -> $i {
    $s = add($s, $i);
    __ev('VALUE', 's', 'INT', $s);
};

__ev('LABEL', 4);
say $s;
