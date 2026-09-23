#!/usr/bin/env python3
"""
raku_oracle_bridge.py -- ORACLE-SIDE sync-step participant for the raku frontend.

MONITOR-BINARY-DESIGN.md section THE ORACLE-SIDE BRIDGES ARE EACH HQ'S OWN: Rakudo 2022.12/MoarVM carries no
trace hook in the VM or the compiler, and the ecosystem Trace module is not installed (zef is not on this box).
Per that section's second option, the bridge is a SOURCE-LEVEL INJECTION: a hand-instrumented twin of the
witness (<witness>.oracle.raku, beside the witness itself) that calls note() -- writing to stderr only, so
real stdout is untouched -- at exactly the points SCRIP's own raku plug fires (lower_raku.c
trace_stmt_wrap/trace_call_wrap/trace_value_prep): one LABEL per top-level statement, one CALL at a called
sub's entry, one RETURN carrying its result, one VALUE after each scalar store.

Unlike the SNOBOL4 csn/spl bridges (engine source patches that speak the wire protocol from inside a running
interpreter), real Rakudo cannot be instrumented in-process, so this bridge runs the oracle twin to completion
first, parses its "EVT ..." stderr lines into an ordered event list, and only THEN opens the READY/GO FIFO
pair and replays that list one record at a time under the normal ack-per-record discipline (monitor_wire.h).
The controller cannot tell replay-from-a-list apart from a live emitter -- the wire protocol is the same
either way -- and this is the same "batch trace, then drive the harness" shape the design page proposes for
the Pascal oracle bridge (gdb -batch driving a script generated from the witness's source).

Usage (env-var driven, matching every other participant in test_monitor_3way_sync_step_auto.sh):
    MONITOR_READY_PIPE=... MONITOR_GO_PIPE=... [MONITOR_NAMES_OUT=...] \
        python3 raku_oracle_bridge.py <witness.raku> [< stdin]

Exit: 0 on a clean run through END; 2 (REFUSE) when no oracle twin exists for the witness, when rakudo is not
on the box, or when the oracle twin itself fails to compile/run -- never a silent empty wire.
"""
import os
import re
import struct
import subprocess
import sys

MWK_VALUE = 1
MWK_CALL = 2
MWK_RETURN = 3
MWK_END = 4
MWK_LABEL = 5
MWK_NAME_DEF = 6

MWT_NULL = 0
MWT_STRING = 1
MWT_INTEGER = 2
MWT_REAL = 3

MW_NAME_ID_NONE = 0xffffffff
MW_HDR_FMT = '<IIBI'

EVT_RE = re.compile(r'^EVT (\S+)(?: (.*))?$')


def die(msg, rc=2):
    print(f'raku_oracle_bridge.py: REFUSE({rc}): {msg}', file=sys.stderr)
    sys.exit(rc)


def rakudo_bin():
    for cand in ('/home/resources/rakudo-local/bin/raku', '/usr/bin/raku'):
        if os.path.isfile(cand) and os.access(cand, os.X_OK):
            return cand
    die('the Rakudo rival is missing: neither /home/resources/rakudo-local/bin/raku nor /usr/bin/raku exists '
        '(see lib_oracle_flags.sh rakudo_bin)')


def oracle_twin_path(src):
    base, ext = os.path.splitext(src)
    twin = base + '.oracle' + ext
    return twin if os.path.isfile(twin) else None


def parse_events(stderr_text):
    """Turn the oracle twin's 'EVT ...' stderr lines into an ordered event list.

    Non-EVT lines (a warning, a deprecation notice) are ignored rather than treated
    as a protocol error -- the events are a strict subset of stderr, never all of it.
    """
    events = []
    for line in stderr_text.splitlines():
        m = EVT_RE.match(line)
        if not m:
            continue
        kind = m.group(1)
        rest = (m.group(2) or '').split(' ')
        if kind == 'LABEL':
            events.append(('LABEL', int(rest[0])))
        elif kind == 'CALL':
            events.append(('CALL', rest[0]))
        elif kind in ('RETURN', 'VALUE'):
            name, vtype, vtext = rest[0], rest[1], ' '.join(rest[2:])
            events.append((kind, name, vtype, vtext))
        else:
            die(f'unrecognised EVT kind {kind!r} in oracle twin output -- the twin drifted from the '
                f'bridge parser, fix one to match the other')
    return events


def wire_value(vtype, vtext):
    if vtype == 'INT':
        return MWT_INTEGER, struct.pack('<q', int(vtext))
    if vtype == 'REAL':
        return MWT_REAL, struct.pack('<d', float(vtext))
    if vtype == 'STR':
        b = vtext.encode('utf-8')
        return MWT_STRING, b
    die(f'unrecognised value type {vtype!r} on the oracle wire -- extend wire_value() and the twin together')


class Wire:
    """One participant's write-ready/read-go pair, with the header pack + ack-per-record
    discipline every C bridge in this directory (monitor_ipc_bin_csn.c) implements."""

    def __init__(self, ready_path, go_path):
        # Ready pipe: write end. Go pipe: read end. Same open order as MON_OPEN.
        self.ready_fd = os.open(ready_path, os.O_WRONLY)
        self.go_fd = os.open(go_path, os.O_RDONLY)

    def emit(self, kind, name_id, vtype, value):
        hdr = struct.pack(MW_HDR_FMT, kind, name_id, vtype, len(value))
        os.write(self.ready_fd, hdr + value)
        ack = os.read(self.go_fd, 1)
        return ack == b'G'

    def close(self):
        os.close(self.ready_fd)
        os.close(self.go_fd)


def main():
    if len(sys.argv) < 2:
        die('usage: raku_oracle_bridge.py <witness.raku>')
    src = sys.argv[1]
    ready_path = os.environ.get('MONITOR_READY_PIPE')
    go_path = os.environ.get('MONITOR_GO_PIPE')
    names_path = os.environ.get('MONITOR_NAMES_OUT')
    if not ready_path or not go_path:
        die('MONITOR_READY_PIPE / MONITOR_GO_PIPE not set -- this bridge only runs under the IPC harness')

    twin = oracle_twin_path(src)
    if twin is None:
        die(f'no oracle twin beside {src} (expected {os.path.splitext(src)[0]}.oracle{os.path.splitext(src)[1]}) '
            f'-- MONITOR-BINARY-DESIGN.md THE ORACLE-SIDE BRIDGES ARE EACH HQ\'S OWN: hand-instrument one, note()-only')

    raku = rakudo_bin()
    try:
        proc = subprocess.run([raku, twin], stdin=sys.stdin, capture_output=True, text=True, timeout=55)
    except subprocess.TimeoutExpired:
        die(f'{raku} {twin} did not finish in 55s')
    if proc.returncode != 0:
        die(f'{raku} {twin} exited {proc.returncode}: {proc.stderr[-400:]}')

    sys.stdout.write(proc.stdout)
    sys.stdout.flush()

    events = parse_events(proc.stderr)
    if not events:
        die(f'the oracle twin {twin} emitted zero EVT lines -- it did not run, or the twin has drifted; '
            f'this is not a clean AGREE, refusing rather than sending an empty wire')

    wire = Wire(ready_path, go_path)
    names = {}

    def intern(name):
        if name in names:
            return names[name]
        nid = len(names)
        names[name] = nid
        wire.emit(MWK_NAME_DEF, nid, MWT_STRING, name.encode('utf-8'))
        return nid

    for ev in events:
        if ev[0] == 'LABEL':
            wire.emit(MWK_LABEL, MW_NAME_ID_NONE, MWT_INTEGER, struct.pack('<q', ev[1]))
        elif ev[0] == 'CALL':
            nid = intern(ev[1])
            wire.emit(MWK_CALL, nid, MWT_NULL, b'')
        elif ev[0] in ('RETURN', 'VALUE'):
            _, name, vtype, vtext = ev
            nid = intern(name)
            wt, vb = wire_value(vtype, vtext)
            wire.emit(MWK_RETURN if ev[0] == 'RETURN' else MWK_VALUE, nid, wt, vb)

    wire.emit(MWK_END, MW_NAME_ID_NONE, MWT_NULL, b'')
    wire.close()

    if names_path:
        with open(names_path, 'w') as f:
            for name, _nid in sorted(names.items(), key=lambda kv: kv[1]):
                f.write(name + '\n')

    sys.exit(0)


if __name__ == '__main__':
    main()
