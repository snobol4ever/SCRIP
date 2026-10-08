#!/usr/bin/env python3
"""util_reflow_200.py [--check | --apply | --proof | --print] [--width 200] PATH... -- THE 200-COLUMN RE-FLOW (Lon 2026-10-08 16:5x CDT, in-chat
to the ceo, verbatim: "Read the rules of formatting found in MD files referencing the 200 max columns. No comments just comment line breaks. No
blank lines. Put code on 200 character lines. If does not fit, begin wrapping at that level and keep those lines long. No line shall EXCEED 200
characters, all must fit."; RULES.md section C code style: 200-char line max, zero blank lines, zero comments but the 200-char separators;
CEO-1565). For every C, C++ and .inc file named (a directory names every such file under it; src/parsers and the two generated tables are
never touched): every comment goes (a separator line is kept and re-cut to exactly --width characters), every blank line goes, preprocessor
directives keep their own lines (a continued directive is re-joined and re-wrapped with backslashes), and the code is PACKED: the statements
of each block go onto lines of at most --width characters, a block running on in the same line while it fits, else the next statement starts
a new line at that block's indent; a statement longer than a line is WRAPPED at its own nesting level at a token boundary (after a comma or
an operator at the shallowest depth first), its continuation lines indented one level deeper and filled as long as they can be; a string
literal longer than a line is split into adjacent literals outside any escape. The token stream is never changed: tokens are re-emitted with
a space wherever the source had whitespace or a newline and nothing where it had none, so `a - -b`, `> >` and `#` keep their meaning.
--check reads rc 1 when a file would change or a line exceeds the width; --apply rewrites in place (LF, UTF-8); --print writes one file's
re-flow to stdout; --proof preprocesses every translation unit the Makefile names with the original and the re-flowed text (gcc or g++
-E -P with the Makefile's own flags, __LINE__ pinned) and reads rc 1 unless the two token streams are identical, adjacent string literals
merged: the behaviour-neutral proof of a landing.
"""
import os, re, sys, subprocess

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
EXTS = (".c", ".h", ".cpp", ".hpp", ".inc")
NEVER = re.compile(r"(/parsers/|gc_allocating_table\.inc|rtx_clobber_table|\.reflow_tmp\.)")
SEP_RX = re.compile(r"^\s*/\*([-=])\1{3,}\*/\s*$")
TOK = re.compile(r"""
    (?P<ws>[ \t\r\f\v]+) | (?P<nl>\n) | (?P<bc>/\*[\s\S]*?\*/) | (?P<lc>//[^\n]*) | (?P<str>L?"(?:\\[\s\S]|[^"\\\n])*") | (?P<chr>L?'(?:\\[\s\S]|[^'\\\n])*')
  | (?P<num>(?:0[xX][0-9A-Fa-f]+|(?:\d+\.\d*|\.\d+|\d+)(?:[eE][+-]?\d+)?)[uUlLfF]*) | (?P<id>[A-Za-z_\u0080-\U0010ffff][A-Za-z0-9_\u0080-\U0010ffff]*)
  | (?P<op>\.\.\.|<<=|>>=|->\*|->|\+\+|--|<<|>>|<=|>=|==|!=|&&|\|\||\+=|-=|\*=|/=|%=|&=|\|=|\^=|::|\#\#|[-+*/%<>=!&|^~?:;,.(){}\[\]\#@\\])
""", re.X)
KW_CTRL = {"if", "while", "for", "switch"}
OPEN_INIT_PREV = {"=", ",", "(", "[", "return", "?"}
CUT_AFTER = {",": 0, ";": 0, "&&": 1, "||": 1, "?": 2, ":": 2, "=": 3, "|": 4, "&": 4, "+": 5, "-": 5, "<<": 5, ">>": 5, "*": 6, "/": 6, "%": 6, ")": 7, "}": 7}
def W(s):
    return len(s.encode("utf-8"))


ESC = re.compile(r"\\(?:x[0-9A-Fa-f]+|[0-7]{1,3}|u[0-9A-Fa-f]{4}|U[0-9A-Fa-f]{8}|[\s\S])")


def tokenize(text):
    out = []; pos = 0; n = len(text)
    while pos < n:
        m = TOK.match(text, pos)
        if not m:
            out.append(("op", text[pos], pos)); pos += 1; continue
        out.append((m.lastgroup, m.group(0), pos)); pos = m.end()
    return out


def _join(toks):
    out = []; sp = False
    for k, s, p in toks:
        if k in ("ws", "nl"):
            if not sp:
                out.append(" "); sp = True
            continue
        out.append(s); sp = False
    return "".join(out)


def logical_lines(text):
    toks = tokenize(text)
    items = []; buf = []; i = 0
    def flush():
        if any(k not in ("ws", "nl") for k, s, p in buf):
            items.append(("code", list(buf)))
        buf.clear()
    def line_start():
        for k, s, p in reversed(buf):
            if k == "nl":
                return True
            if k != "ws":
                return False
        return True
    while i < len(toks):
        k, s, p = toks[i]
        if k == "bc":
            if SEP_RX.match(s):
                flush(); items.append(("sep", s.strip()[2]))
            i += 1; continue
        if k == "lc":
            i += 1; continue
        if k == "op" and s == "#" and line_start():
            flush(); line = []; j = i
            while j < len(toks):
                k2, s2, p2 = toks[j]
                if k2 == "nl":
                    if line and line[-1][1] == "\\":
                        line.pop(); line.append(("ws", " ", p2)); j += 1; continue
                    break
                if k2 in ("bc", "lc"):
                    j += 1; continue
                line.append(toks[j]); j += 1
            items.append(("pp", _join(line).strip())); i = j; continue
        buf.append(toks[i]); i += 1
    flush()
    return items


def _sig(u):
    return [t for t in u if t[0] not in ("ws", "nl")]


def _paren_is_cast(st):
    depth = 0
    for j in range(len(st) - 1, -1, -1):
        if st[j][1] == ")":
            depth += 1
        elif st[j][1] == "(":
            depth -= 1
            if depth == 0:
                if j == 0:
                    return True
                pk, ps = st[j - 1][0], st[j - 1][1]
                if ps in KW_CTRL or pk == "id" or ps in (")", "]"):
                    return False
                return True
    return False


def split_units(toks):
    units = []; cur = []; pdepth = 0; init_depth = 0; prev_sig = None
    for t in toks:
        k, s, p = t
        cur.append(t)
        if k in ("ws", "nl"):
            continue
        if s in ("(", "["):
            pdepth += 1
        elif s in (")", "]"):
            pdepth = max(0, pdepth - 1)
        elif s == "{":
            if pdepth > 0 or init_depth > 0 or prev_sig in OPEN_INIT_PREV or (prev_sig == ")" and _paren_is_cast(_sig(cur))):
                init_depth += 1
            else:
                units.append((_join(cur).strip(), 1, 0)); cur = []
        elif s == "}":
            if init_depth > 0:
                init_depth -= 1
            else:
                body = _join(cur[:-1]).strip()
                if body:
                    units.append((body, 0, 0))
                units.append(("}", 0, 1)); cur = []
        elif s == ";" and pdepth == 0 and init_depth == 0:
            units.append((_join(cur).strip(), 0, 0)); cur = []
        elif s == ":" and pdepth == 0 and init_depth == 0:
            st = _sig(cur)
            if st and (st[0][1] in ("case", "default", "public", "private", "protected") or (len(st) == 2 and st[0][0] == "id")):
                units.append((_join(cur).strip(), 0, 0)); cur = []
        prev_sig = s
    rest = _join(cur).strip()
    if rest:
        units.append((rest, 0, 0))
    return units


def _split_str_token(s, room):
    prefix = "L" if s.startswith("L") else ""
    inner = s[len(prefix) + 1:-1]
    out = []; room = max(8, room - len(prefix) - 2)
    while W(inner) > room:
        spans = [(m.start(), m.end()) for m in ESC.finditer(inner)]
        cut = min(room, len(inner))
        while cut > 1 and (W(inner[:cut]) > room or any(a < cut < b for a, b in spans)):
            cut -= 1
        out.append(prefix + '"' + inner[:cut] + '"'); inner = inner[cut:]
    out.append(prefix + '"' + inner + '"')
    return out


def wrap_long(text, indent, width):
    cont = indent + "    "
    pieces = []; depth = 0; toks = tokenize(text)
    for i, (k, s, p) in enumerate(toks):
        if k in ("ws", "nl"):
            continue
        pre = " " if i > 0 and toks[i - 1][0] in ("ws", "nl") else ""
        if s in ("(", "["):
            depth += 1
        rank = (depth, CUT_AFTER[s]) if s in CUT_AFTER else None
        if s in (")", "]"):
            depth = max(0, depth - 1); rank = (depth, CUT_AFTER[")"])
        if k == "str" and W(s) > width - len(cont) - 1:
            for q, part in enumerate(_split_str_token(s, width - len(cont) - 1)):
                pieces.append((pre if q == 0 else " ", part, None if q else rank))
            continue
        pieces.append((pre, s, rank))
    out = []; a = 0; first = True
    while a < len(pieces):
        ind = indent if first else cont
        line = ind; j = a
        while j < len(pieces):
            pre, s, rank = pieces[j]
            cand = line + (pre if j > a else "") + s
            if W(cand) > width and j > a:
                break
            line = cand; j += 1
        if j >= len(pieces):
            out.append(line.rstrip()); break
        best = None
        for q in range(a, j):
            r = pieces[q][2]
            if r is not None and (best is None or r < best[0] or (r == best[0] and q > best[1])):
                best = (r, q)
        cut = best[1] if best else j - 1
        line = ind
        for q in range(a, cut + 1):
            pre, s, rank = pieces[q]
            line += (pre if q > a else "") + s
        out.append(line.rstrip()); a = cut + 1; first = False
    return out


def _wrap_pp(line, width):
    toks = [t for t in tokenize(line) if t[0] != "nl"]
    out = []; cur = ""
    for k, s, p in toks:
        if k == "ws":
            cur += " "; continue
        if W(cur) + W(s) + 2 > width and cur.strip():
            out.append(cur.rstrip() + " \\"); cur = "    "
        cur += s
    out.append(cur.rstrip())
    return out


class Blk:
    __slots__ = ("opener", "children", "closer")
    def __init__(self, opener):
        self.opener = opener; self.children = []; self.closer = "}"


def build_tree(units):
    """Units -> a list of statements (str) and blocks (Blk); a closer followed by `else ...`, by `while (...);` after a `do {`, by `;` or by
    the name after a struct/union/enum body is folded into the closer so that `} else {`, `} while (x);`, `};` and `} name;` stay together."""
    root = []; stack = [root]; openers = [None]
    i = 0
    while i < len(units):
        u, opens, closes = units[i]
        if closes:
            blk_opener = openers[-1]
            if len(stack) > 1:
                stack.pop(); openers.pop()
            blk = stack[-1][-1] if stack[-1] and isinstance(stack[-1][-1], Blk) else None
            nxt = units[i + 1] if i + 1 < len(units) else None
            if blk is not None and nxt is not None:
                nu, nopens, ncloses = nxt
                head = nu.split(" ", 1)[0] if nu else ""
                is_do = blk_opener is not None and re.search(r"(^|\W)do\s*\{$", blk_opener) is not None
                aggregate = blk_opener is not None and re.search(r"(^|\W)(struct|union|enum|typedef)(\W|$)", blk_opener) is not None and "(" not in blk_opener.split("{")[0]
                if head == "else" and not ncloses:
                    if nopens:
                        nb = Blk("} " + nu); blk.closer = None; stack[-1].append(nb); stack.append(nb.children); openers.append(nu); i += 2; continue
                    blk.closer = "} " + nu; i += 2; continue
                if (is_do and head == "while" and not nopens) or nu.startswith(";") or (aggregate and not nopens and not ncloses):
                    blk.closer = "}" + ("" if nu.startswith(";") else " ") + nu; i += 2; continue
            i += 1; continue
        if opens:
            b = Blk(u); stack[-1].append(b); stack.append(b.children); openers.append(u)
        else:
            stack[-1].append(u)
        i += 1
    return root


def flat(item):
    if isinstance(item, Blk):
        inner = " ".join(flat(c) for c in item.children)
        return item.opener + (" " + inner if inner else "") + " " + (item.closer if item.closer is not None else "}")
    return item


def layout(items, depth, width, out, line):
    """Pack `items` at `depth` onto lines of at most `width` bytes; returns the open line. A block that fits stays flat; one that does not opens
    its line, lays its children out one level deeper and closes on its own line at its own depth."""
    ind = "    " * depth
    for item in items:
        f = flat(item)
        if line.strip() and W(line.rstrip()) + 1 + W(f) <= width:
            line = line.rstrip() + " " + f; continue
        if line.strip():
            out.append(line.rstrip()); line = ""
        if len(ind) + W(f) <= width:
            line = ind + f; continue
        if isinstance(item, Blk):
            opener = item.opener
            if len(ind) + W(opener) <= width:
                out.append(ind + opener)
            else:
                out.extend(wrap_long(opener, ind, width))
            line = layout(item.children, depth + 1, width, out, "")
            if line.strip():
                out.append(line.rstrip()); line = ""
            closer = item.closer if item.closer is not None else "}"
            if len(ind) + W(closer) <= width:
                out.append(ind + closer)
            else:
                out.extend(wrap_long(closer, ind, width))
            continue
        out.extend(wrap_long(f, ind, width)); line = ""
    return line


def reflow(text, width=200):
    items = logical_lines(text)
    out = []
    for kind, payload in items:
        if kind == "sep":
            out.append("/*" + payload * (width - 4) + "*/"); continue
        if kind == "pp":
            out.extend([payload] if W(payload) <= width else _wrap_pp(payload, width)); continue
        tree = build_tree(split_units(payload))
        line = layout(tree, 0, width, out, "")
        if line.strip():
            out.append(line.rstrip())
    return "\n".join(l for l in out if l.strip()) + "\n"


def reflow_lines(text, width=200):
    """THE LINE-ORIENTED RE-FLOW for the templates (Lon 2026-10-08 17:1x CDT, verbatim: "Regarding the template CPP files, those should be more
    line oriented and not spead across 200 characters, but it can use 200 chars no problem. But since it is ASM, the lines of C++ code should
    match closely the lines of ASM being emitted."): every original line keeps its own line and its indentation; comments go (a separator is
    re-cut), blank lines go, runs of spaces between tokens become one, a line over the width is wrapped at its own level."""
    toks = tokenize(text)
    lines = []; cur = []; sep = None
    def emit_line():
        nonlocal cur, sep
        if sep is not None:
            lines.append("/*" + sep * (width - 4) + "*/"); sep = None
        s = "".join(cur).rstrip()
        if s.strip():
            lines.append(s)
        cur = []
    at_start = True
    for k, s, p in toks:
        if k == "nl":
            emit_line(); at_start = True; continue
        if k == "bc":
            if SEP_RX.match(s):
                emit_line(); sep = s.strip()[2]
            elif "\n" in s:
                emit_line()
            else:
                cur.append(" ")
            continue
        if k == "lc":
            continue
        if k == "ws":
            cur.append(s if at_start else " "); continue
        at_start = False; cur.append(s)
    emit_line()
    out = []; cont = False
    for l in lines:
        is_pp = l.lstrip().startswith("#") or cont
        cont = l.rstrip().endswith("\\")
        if W(l) <= width:
            out.append(l); continue
        ind = l[:len(l) - len(l.lstrip())]
        if is_pp:
            out.extend(_wrap_pp(l.strip(), width))
        else:
            out.extend(wrap_long(l.strip(), ind, width))
    return "\n".join(out) + "\n"


def line_oriented(path):
    p = os.path.abspath(path)
    return ("/src/templates/" in p) or p.endswith((".cpp", ".hpp"))


def reflow_for(path, text, width):
    return reflow_lines(text, width) if line_oriented(path) else reflow(text, width)


def files_of(paths):
    for p in paths:
        p = os.path.abspath(p)
        if os.path.isdir(p):
            for dp, dns, fns in os.walk(p):
                for fn in sorted(fns):
                    f = os.path.join(dp, fn)
                    if fn.endswith(EXTS) and not NEVER.search(f):
                        yield f
        elif p.endswith(EXTS) and not NEVER.search(p):
            yield p


def make_commands():
    out = subprocess.run(["make", "-n", "-B", "scrip"], cwd=ROOT, capture_output=True, text=True).stdout
    cmds = {}
    for line in out.split("\n"):
        m = re.search(r"\s-c\s+(\S+\.(?:c|cpp))(?:\s|$)", line)
        if m:
            cmds[os.path.abspath(os.path.join(ROOT, m.group(1)))] = line
    return cmds


def strip_dep_flags(parts):
    """The Makefile's -MMD -MP (and any -MF/-MT/-MQ pair) would write a .d beside the proof's temporary file: a proof compile writes no dependency list."""
    out = []; skip = 0
    for p in parts:
        if skip:
            skip -= 1; continue
        if p in ("-MMD", "-MP", "-MD"):
            continue
        if p in ("-MF", "-MT", "-MQ"):
            skip = 1; continue
        out.append(p)
    return out


def pp_tokens(cmd, src, alt=None):
    parts = [p for p in cmd.split() if p != "-c"]
    parts = strip_dep_flags(parts)
    if "-o" in parts:
        k = parts.index("-o"); del parts[k:k + 2]
    if alt:
        parts = [alt if os.path.abspath(os.path.join(ROOT, p)) == src else p for p in parts]
    parts += ["-E", "-P", "-w", "-D__LINE__=0"]
    r = subprocess.run(parts, cwd=ROOT, capture_output=True, text=True)
    if r.returncode != 0:
        return None, r.stderr[-400:]
    merged = []
    for k, s, p in tokenize(r.stdout):
        if k in ("ws", "nl", "bc", "lc"):
            continue
        if k == "str" and ".reflow_tmp." in s:
            s = s.replace(".reflow_tmp.", ".")
        if merged and s.startswith('"') and merged[-1].startswith('"'):
            merged[-1] = merged[-1][:-1] + s[1:]
        else:
            merged.append(s)
    return merged, ""


def obj_bytes(cmd, src, alt, text):
    """Compile `text` written under the SAME temp name both times (so the STT_FILE symbol agrees), -g0, __LINE__ pinned; return the object bytes."""
    open(alt, "wb").write(text.encode("utf-8"))
    parts = strip_dep_flags(cmd.split())
    if "-o" in parts:
        k = parts.index("-o"); del parts[k:k + 2]
    parts = [alt if os.path.abspath(os.path.join(ROOT, p)) == src else p for p in parts]
    parts = [p for p in parts if not p.startswith("-g")] + ["-g0", "-w", "-D__LINE__=0", "-o", alt + ".o"]
    r = subprocess.run(parts, cwd=ROOT, capture_output=True, text=True)
    if r.returncode != 0:
        return None, r.stderr[-300:]
    data = open(alt + ".o", "rb").read()
    dump = subprocess.run(["objdump", "-s", "-d", "-r", alt + ".o"], capture_output=True).stdout
    os.unlink(alt + ".o")
    dump = b"\n".join(l for l in dump.split(b"\n") if not l.endswith(b"file format elf64-x86-64") and not l.startswith(b"In archive"))
    return (data, dump), ""


INSN = re.compile(r"^\s*([0-9a-f]+):\s*((?:[0-9a-f]{2} )+)\s*([a-z][\w.]*)\s*(.*)$")
BYTES_ONLY = re.compile(r"^\s*([0-9a-f]+):\s*((?:[0-9a-f]{2} ?)+)\s*$")


def insn_stream(dump):
    """The instruction stream of an objdump -d listing with every nop dropped and every address replaced by an ordinal: a jump or call target
    becomes the ordinal of the real instruction that contains the target byte plus the byte offset inside it (a target on a nop is the next
    real instruction; an unrelocated call's printed target lies inside the call itself). gcc at -O0 places a nop from the line a closing
    brace sits on, so a re-flow that moves braces across lines changes only those."""
    import bisect
    text = dump.decode("latin-1")
    spans = []; insns = []; pending_nops = []; last = None
    for line in text.split("\n"):
        mb = BYTES_ONLY.match(line)
        if mb and last is not None:
            extra = len(mb.group(2).split())
            if last == "nop":
                a0, s0 = pending_nops[-1]; pending_nops[-1] = (a0, s0 + extra)
            else:
                st, en, o, isn = spans[-1]; spans[-1] = (st, en + extra, o, isn)
            continue
        m = INSN.match(line)
        if not m:
            continue
        addr = int(m.group(1), 16); size = len(m.group(2).split()); op = m.group(3); args = m.group(4)
        if op.startswith("nop") or (op == "xchg" and args.strip() == "%ax,%ax"):
            pending_nops.append((addr, size)); last = "nop"; continue
        for na, ns in pending_nops:
            spans.append((na, na + ns, len(insns), True))
        pending_nops = []
        spans.append((addr, addr + size, len(insns), False)); insns.append((op, args)); last = "insn"
    starts = [s[0] for s in spans]
    def locate(a):
        i = bisect.bisect_right(starts, a) - 1
        if i < 0:
            return None
        st, en, o, is_nop = spans[i]
        if a >= en:
            return None
        return "@%d" % o if (is_nop or a == st) else "@%d+%d" % (o, a - st)
    out = []
    for op, args in insns:
        def rebase(mm):
            r = locate(int(mm.group(1), 16))
            return r if r is not None else mm.group(0)
        out.append(op + " " + re.sub(r"\b([0-9a-f]+) <[^>]*>", rebase, args))
    return out


def nonnop_sections_equal(da, db):
    """Every data section's dumped contents (.rodata*, .data*, .bss*) byte-identical; .text is judged by the instruction stream and the
    address-bearing sections (.eh_frame, .symtab, .rela.*) legitimately shift with it."""
    def secs(d):
        parts = d.decode("latin-1").split("Contents of section ")
        out = {}
        for p in parts[1:]:
            name = p.split(":", 1)[0].strip()
            if name.startswith((".rodata", ".data", ".bss")) and not name.startswith(".rela"):
                out[name] = re.sub(r"(?m)^ [0-9a-f]+ ", " ", p)
        return out
    return secs(da) == secs(db)


def dump_diff(da, db):
    """Name the first section whose dumped bytes differ."""
    sa = da.split(b"Contents of section "); sb = db.split(b"Contents of section ")
    if len(sa) != len(sb):
        return "a different section count (%d vs %d)" % (len(sa), len(sb))
    for x, y in zip(sa, sb):
        if x != y:
            return x.split(b":", 1)[0].decode("ascii", "replace")[:40] or "the disassembly"
    return "the disassembly"


def objproof(paths, width):
    cmds = make_commands(); bad = proved = skipped = nops = 0
    for f in files_of(paths):
        if not f.endswith((".c", ".cpp")):
            continue
        cmd = cmds.get(f)
        if not cmd:
            skipped += 1; continue
        src = open(f, "rb").read().decode("utf-8"); new = reflow_for(f, src, width)
        stem, ext = os.path.splitext(f); alt = stem + ".reflow_tmp" + ext
        try:
            a, ea = obj_bytes(cmd, f, alt, src); b, eb = obj_bytes(cmd, f, alt, new)
        finally:
            if os.path.exists(alt):
                os.unlink(alt)
        if a is None or b is None:
            bad += 1; print("  OBJECT PROOF REFUSED %s: %s" % (os.path.relpath(f, ROOT), (ea or eb).strip()[-200:])); continue
        (oa, da), (ob, db) = a, b
        if da != db:
            if insn_stream(da) == insn_stream(db) and nonnop_sections_equal(da, db):
                nops += 1; print("  OBJECT PROOF nops-only %s: the instruction streams agree once gcc's -O0 brace-line nops are dropped and every other section is byte-identical" % os.path.relpath(f, ROOT))
            else:
                bad += 1; print("  OBJECT PROOF FAIL %s: the dumped internals differ in %s" % (os.path.relpath(f, ROOT), dump_diff(da, db)))
        elif oa != ob:
            bad += 1; print("  OBJECT PROOF FAIL %s: the dumps agree but the object files differ (%d vs %d bytes)" % (os.path.relpath(f, ROOT), len(oa), len(ob)))
        else:
            proved += 1
    print("object proof: %d translation units compile to byte-identical objects and objdump -s -d -r internals before and after the re-flow (-g0, __LINE__ pinned); %d differ only by gcc's -O0 brace-line nops (instruction streams and every other section identical); %d differ or refused; %d without a Makefile command" % (proved, nops, bad, skipped))
    return 1 if bad else 0


def main(argv):
    mode = "--check"
    for m in ("--check", "--apply", "--proof", "--print", "--objproof"):
        if m in argv:
            mode = m
    width = int(argv[argv.index("--width") + 1]) if "--width" in argv else 200
    paths = [a for a in argv if not a.startswith("--") and not a.isdigit()]
    if not paths:
        print(__doc__); return 2
    if mode == "--print":
        for f in files_of(paths):
            sys.stdout.write(reflow_for(f, open(f, "rb").read().decode("utf-8"), width))
        return 0
    if mode == "--objproof":
        return objproof(paths, width)
    changed = over = total = 0
    for f in files_of(paths):
        total += 1
        src = open(f, "rb").read().decode("utf-8"); new = reflow_for(f, src, width)
        longest = max((W(l) for l in new.split("\n")), default=0)
        if longest > width:
            over += 1; print("  OVER %4d %s" % (longest, os.path.relpath(f, ROOT)))
        if new != src:
            changed += 1
            if mode == "--apply":
                open(f, "wb").write(new.encode("utf-8"))
    if mode == "--proof":
        cmds = make_commands(); bad = proved = skipped = 0
        for f in files_of(paths):
            if not f.endswith((".c", ".cpp")):
                continue
            cmd = cmds.get(f)
            if not cmd:
                skipped += 1; continue
            new = reflow_for(f, open(f, "rb").read().decode("utf-8"), width)
            stem, ext = os.path.splitext(f); alt = stem + ".reflow_tmp" + ext
            open(alt, "wb").write(new.encode("utf-8"))
            try:
                a, ea = pp_tokens(cmd, f); b, eb = pp_tokens(cmd, f, alt)
            finally:
                os.unlink(alt)
            if a is None or b is None:
                bad += 1; print("  PROOF REFUSED %s: %s" % (os.path.relpath(f, ROOT), (ea or eb).strip()[-200:])); continue
            if a != b:
                bad += 1; k = next((i for i in range(min(len(a), len(b))) if a[i] != b[i]), min(len(a), len(b)))
                print("  PROOF FAIL %s: token %d differs: %r vs %r" % (os.path.relpath(f, ROOT), k, a[k:k + 4], b[k:k + 4]))
            else:
                proved += 1
        print("proof: %d translation units identical after re-flow, %d differ or refused, %d without a Makefile command" % (proved, bad, skipped))
        return 1 if bad else 0
    print("%s: %d files, %d would change, %d would still carry a line over %d" % (mode, total, changed, over, width))
    return 1 if (changed or over) and mode == "--check" else (1 if over else 0)


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
