#!/usr/bin/env python3
# semicolonize_icon.py -- transform newline-style Icon source to SCRIP semicolon-required style.
# RULES.md: "Newline-style sources get `;` added to the SOURCE, never compiler newline handling."
# THE RULE (canonical, refs/jcon-master/tran/lexer.icn yylex + oplexgen.icn b/e table, verbatim):
#   at a newline boundary, if last_token is an ENDER and next_token is a BEGINNER, insert ";".
# The b/e flags below are transcribed 1:1 from oplexgen.icn init_lex_tables() (jcon-master, Proebsting).
# Usage: semicolonize_icon.py < in.icn > out.icn   |   semicolonize_icon.py in.icn [out.icn]
import sys, re
RESERVED = {  # word: flags   (oplexgen.icn "Reserved Words")
 "break":"be","by":"","case":"b","create":"b","default":"b","do":"","else":"","end":"b","every":"b",
 "fail":"be","global":"","if":"b","initial":"b","invocable":"","link":"","local":"b","next":"be",
 "not":"b","of":"","procedure":"","record":"","repeat":"b","return":"be","static":"b","suspend":"be",
 "then":"","to":"","until":"b","while":"b"}
OPS = {  # op: flags   (oplexgen.icn "Operators"; longest-match)
 ":=":"","@":"b","@:=":"","&:=":"","=:=":"","===:=":"",">=:=":"",">:=":"","<=:=":"","<:=":"","~=:=":"",
 "~===:=":"","==:=":"",">>=:=":"",">>:=":"","<<=:=":"","<<:=":"","~==:=":"","\\":"b","!":"b","|":"b",
 "^":"b","^:=":"b",":":"",",":"","||":"b","||:=":"","&":"b",".":"b","--":"b","--:=":"","===":"b",
 "**":"b","**:=":"","{":"b","[":"b","|||":"b","|||:=":"","==":"b",">>=":"",">>":"","<<=":"","<<":"",
 "~==":"b","(":"b","-:":"","-":"b","-:=":"","%":"","%:=":"","~===":"b","=":"b",">=":"",">":"","<=":"",
 "<":"","~=":"b","+:":"","+":"b","+:=":"","?":"b","<-":"","<->":"","}":"e","]":"e",")":"e",";":"",
 "?:=":"","/":"b","/:=":"","*":"b","*:=":"",":=:":"","~":"b","++":"b","++:=":"","$(":"b","$)":"e",
 "$<":"b","$>":"e","$":"b","%$(":"b","%$)":"e","%{":"b","%}":"e","%%":"be"}
OPKEYS = sorted(OPS, key=len, reverse=True)
IDENT = re.compile(r"[A-Za-z_][A-Za-z0-9_]*")
NUM = re.compile(r"[0-9]+[rR][0-9A-Za-z]+|[0-9]+\.[0-9]+([eE][+-]?[0-9]+)?|[0-9]+([eE][+-]?[0-9]+)?")
def tokens(line):  # yield (text, flags) for one physical line already known comment/string-safe at entry
    i, n = 0, len(line)
    while i < n:
        c = line[i]
        if c in " \t\f\r": i += 1; continue
        if c == "#": return                      # comment to end of line
        if c in "\"'":                           # string / cset literal (single-line or _-continued)
            q, j = c, i+1
            while j < n:
                if line[j] == "\\": j += 2; continue
                if line[j] == q: break
                j += 1
            if j >= n:                           # unterminated on this line -> _-continuation
                yield ("\x00OPEN"+q, "")         # sentinel: string still open at EOL
                return
            yield (line[i:j+1], "be"); i = j+1; continue
        m = IDENT.match(line, i)
        if m: w = m.group(0); yield (w, RESERVED.get(w, "be")); i = m.end(); continue
        m = NUM.match(line, i)
        if m: yield (m.group(0), "be"); i = m.end(); continue
        if c == "&":                             # &keyword lexes as one value token (beginner+ender)
            m = IDENT.match(line, i+1)
            if m: yield ("&"+m.group(0), "be"); i = m.end(); continue
        for op in OPKEYS:
            if line.startswith(op, i): yield (op, OPS[op]); i += len(op); break
        else: i += 1                             # unknown char: skip (defensive)
def close_open_string(line, q):                  # inside a _-continued literal: find closing quote
    i, n = 0, len(line)
    while i < n:
        if line[i] == "\\": i += 2; continue
        if line[i] == q: return i+1
        i += 1
    return -1
def semicolonize(src):
    out, last_flags, open_q = [], "", None
    for raw in src.splitlines():
        line = raw
        if open_q:                               # continuing a multi-line string literal
            k = close_open_string(line, open_q)
            out.append(raw)
            if k >= 0:
                open_q = None; last_flags = "be" # literal is an ender
                rest = list(tokens(line[k:]))
                for t, f in rest:
                    if t.startswith("\x00OPEN"): open_q = t[-1]; break
                    last_flags = f
            continue
        toks = list(tokens(line))
        if toks and toks[-1][0].startswith("\x00OPEN"):
            open_q = toks[-1][0][-1]; toks = toks[:-1]
        if not toks:                             # blank/comment-only line: boundary persists (lex_newline
            out.append(raw); continue            # stays set until next real token -- lexer.icn:58,21)
        first_flags = toks[0][1]
        if "e" in last_flags and "b" in first_flags:
            # append ; to the previous CODE-bearing line, before any trailing comment
            for j in range(len(out)-1, -1, -1):
                prev = out[j]
                cut = code_end(prev)
                if cut is not None:
                    out[j] = prev[:cut] + ";" + prev[cut:]; break
        if not open_q: last_flags = toks[-1][1]
        else: last_flags = ""
        out.append(raw)
    return "\n".join(out) + "\n"
def code_end(line):                              # index just past last code char (None if no code)
    i, n, last = 0, len(line), None
    while i < n:
        c = line[i]
        if c in " \t\f\r": i += 1; continue
        if c == "#": break
        if c in "\"'":
            q, j = c, i+1
            while j < n:
                if line[j] == "\\": j += 2; continue
                if line[j] == q: break
                j += 1
            i = min(j+1, n); last = i; continue
        i += 1; last = i
    return last
if __name__ == "__main__":
    data = open(sys.argv[1]).read() if len(sys.argv) > 1 else sys.stdin.read()
    res = semicolonize(data)
    (open(sys.argv[2], "w") if len(sys.argv) > 2 else sys.stdout).write(res)
