#!/usr/bin/env python3
"""util_render_error_voice.py <oracle> -- THE EQUIVALENCE LIST, applied.  stdin -> stdout.

⛔⭐ ONE ERROR VOICE (Lon 2026-09-12, RULES.md § ONE ERROR VOICE, ceo CEO-623/625): the runtime prints ONE
SCRIP-shaped error for every language and never an oracle's shape.  A ref that pins an oracle's error text is
graded THROUGH THIS RENDERER: it rewrites every SCRIP error block in a captured stream into the named oracle's
shape, and the grader diffs the rendered stream against the oracle's ref.  Nothing is hidden and nothing is
dropped -- every field SCRIP printed is carried into the oracle's shape, and a block the list cannot render is
left as it stands (so it reads RED, never green by omission).

THE SCRIP VOICE (src/runtime/core/core.c core_error_voice):
    scrip: error <code>: <text>
      at <file>:<line>[; statement <stno>]        or        at startup
      offending value: <image>                     (only when the raise carried a value)
      in <frame>                                   (one per activation frame, outermost first, then the
                                                    builtin frames at that level and the operator in flight)

THE LISTS (each row is what the oracle prints for the SCRIP field, measured on the oracle named):
  icon     Arizona icont/iconx 9.5.25a (corpus/packages/icon/jcon_tests/errors.std, loadfunc.std are the receipts):
             "\\nRun-time error <code>\\nFile <basename(file)>; Line <line>\\n<text>\\n"
             "offending value: <image>\\n"                      when present
             "Traceback:\\n" then each frame on its own line, verbatim
             startup: "\\nRun-time error <code> in startup code\\n<text>\\n"
  spitbol  SPITBOL x64 fork /home/resources/x64/bin/sbl -bf (RULES.md § Oracles):
             "<file>(<line>) : ERROR <code:03d> -- <text>\\nin statement <stno>\\n"   (stno 0 when SCRIP printed none;
             frames and offending value have no SPITBOL counterpart and are dropped -- SPITBOL prints none)
  none     identity (the SCRIP voice itself)
"""
import re, sys, os

HEAD = re.compile(r'^scrip: error (\d+): (.*)$')
AT = re.compile(r'^  at (.*?):(\d+)(?:; statement (\d+))?$')

def render(lines, oracle):
    out = []; i = 0; n = len(lines)
    while i < n:
        m = HEAD.match(lines[i])
        if not m:
            out.append(lines[i]); i += 1; continue
        code, text = int(m.group(1)), m.group(2)
        j = i + 1; at = None; startup = False; val = None; frames = []
        while j < n and lines[j].startswith('  '):
            l = lines[j]
            if at is None and not startup and AT.match(l): at = AT.match(l)
            elif l == '  at startup': startup = True
            elif l.startswith('  offending value: '): val = l[len('  offending value: '):]
            elif l.startswith('  in '): frames.append(l[5:])
            else: break
            j += 1
        if oracle == 'icon':
            if startup:
                out += ['', 'Run-time error %d in startup code' % code, text]
            else:
                f = os.path.basename(at.group(1)) if at else ''; ln = at.group(2) if at else '0'
                out += ['', 'Run-time error %d' % code, 'File %s; Line %s' % (f, ln), text]
                if val is not None: out.append('offending value: ' + val)
                out.append('Traceback:'); out += frames
        elif oracle == 'spitbol':
            f = at.group(1) if at else ''; ln = at.group(2) if at else '0'; st = at.group(3) if (at and at.group(3)) else '0'
            out += ['%s(%s) : ERROR %03d -- %s' % (f, ln, code, text), 'in statement %s' % st]
        else:
            out += lines[i:j]
        i = j
    return out

if __name__ == '__main__':
    if len(sys.argv) != 2 or sys.argv[1] not in ('icon', 'spitbol', 'none'):
        sys.stderr.write('REFUSE(rc=2): util_render_error_voice.py <icon|spitbol|none>  (stdin -> stdout)\n'); sys.exit(2)
    data = sys.stdin.buffer.read().decode('utf-8', 'surrogateescape')
    trailing_nl = data.endswith('\n')
    lines = data.split('\n')
    if trailing_nl: lines = lines[:-1]
    res = render(lines, sys.argv[1])
    text = '\n'.join(res) + ('\n' if trailing_nl else '')
    sys.stdout.buffer.write(text.encode('utf-8', 'surrogateescape'))
