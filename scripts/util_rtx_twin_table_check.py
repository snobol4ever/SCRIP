#!/usr/bin/env python3
import os, re, subprocess, sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
KINDS = {"RTX_DECLINE": "AL", "RTX_DECLINE_INT": "INT", "RTX_DECLINE_PTR": "PTR", "RTX_NOT_HANDLED": "AL", "RTX_NOT_HANDLED_INT": "INT", "RTX_NOT_HANDLED_PTR": "PTR"}
FUNC = re.compile(r"^RTX_FUNC\((\w+)\)")
ENDF = re.compile(r"^RTX_ENDF\((\w+)\)")
ROW = re.compile(r"^RTX_TWIN\((AL|INT|PTR),\s*[^,]+,\s*(\w+),\s*(\w+),")


def declining_leaves(rtx_dir):
    out = {}
    for fn in sorted(os.listdir(rtx_dir)):
        if not fn.endswith(".s"):
            continue
        cur = None
        for ln in open(os.path.join(rtx_dir, fn), encoding="utf-8"):
            m = FUNC.match(ln)
            if m:
                cur = m.group(1)
                continue
            if ENDF.match(ln):
                cur = None
                continue
            if cur:
                for tok in re.findall(r"\bRTX_[A-Z_]+\b", ln):
                    if tok in KINDS:
                        out.setdefault(cur, set()).add(KINDS[tok])
    return out


def table_rows(path):
    rows = {}
    for ln in open(path, encoding="utf-8"):
        m = ROW.match(ln)
        if m:
            rows[m.group(2)] = (m.group(1), m.group(3))
    return rows


def check(leaves, rows, symbols):
    bad = []
    for leaf, kinds in sorted(leaves.items()):
        if len(kinds) != 1:
            bad.append("%s declines in more than one kind (%s)" % (leaf, ",".join(sorted(kinds))))
        elif leaf not in rows:
            bad.append("%s declines (%s) and is no row of rtx_twin_table.inc: the box would take the verdict byte for a value" % (leaf, next(iter(kinds))))
        elif rows[leaf][0] not in kinds:
            bad.append("%s declines as %s and its row says %s" % (leaf, next(iter(kinds)), rows[leaf][0]))
    for leaf, (kind, twin) in sorted(rows.items()):
        if leaf not in leaves:
            bad.append("row %s names a leaf that never declines: dead weight at every call site" % leaf)
        if symbols is not None and leaf not in symbols:
            bad.append("row %s names a leaf the runtime does not define" % leaf)
        if symbols is not None and twin not in symbols:
            bad.append("row %s names a twin %s the runtime does not define" % (leaf, twin))
    return bad


def runtime_symbols(so):
    r = subprocess.run(["nm", "-D", "--defined-only", so], capture_output=True, text=True)
    if r.returncode != 0:
        return None
    return set(ln.split()[-1] for ln in r.stdout.splitlines() if ln.strip())


def selftest(leaves, rows, symbols):
    fails = []
    some = sorted(rows)[0]
    p = dict(rows); del p[some]
    if not any(some in b and "no row" in b for b in check(leaves, p, symbols)): fails.append("a leaf without a row was not caught")
    p = dict(rows); p["rt_never_declines_planted"] = ("AL", "c_rt_never_declines_planted")
    if not any("never declines" in b for b in check(leaves, p, symbols)): fails.append("a row without a decline was not caught")
    p = dict(rows); k = "INT" if rows[some][0] != "INT" else "AL"; p[some] = (k, rows[some][1])
    if not any("row says" in b for b in check(leaves, p, symbols)): fails.append("a wrong kind was not caught")
    if symbols is not None:
        s = set(symbols); s.discard(rows[some][1])
        if not any("twin" in b for b in check(leaves, rows, s)): fails.append("a missing twin was not caught")
    return fails


def main():
    rtx = os.path.join(ROOT, "src", "runtime", "rtx")
    tab = os.path.join(ROOT, "src", "templates", "x86", "rtx_twin_table.inc")
    so = os.path.join(ROOT, "out", "libscrip_rt.so")
    if not os.path.isfile(tab) or not os.path.isdir(rtx):
        print("REFUSED(2): the twin table or the rtx directory is missing")
        return 2
    leaves = declining_leaves(rtx)
    rows = table_rows(tab)
    symbols = runtime_symbols(so) if os.path.exists(so) else None
    if symbols is None:
        print("REFUSED(2): %s cannot be read with nm -D" % so)
        return 2
    if len(leaves) < 10 or len(rows) < 10:
        print("REFUSED(2): only %d declining leaves and %d rows were read" % (len(leaves), len(rows)))
        return 2
    bad = check(leaves, rows, symbols)
    st = selftest(leaves, rows, symbols)
    for b in bad:
        print("  VIOLATION  " + b)
    for f in st:
        print("  SELF-TEST FAIL  " + f)
    print("RTX TWIN TABLE: %d declining leaves, %d rows, %d violations, self-test %s" % (len(leaves), len(rows), len(bad), "FAIL" if st else "PASS (a leaf without a row, a row without a decline, a wrong kind and a missing twin each read red by name)"))
    return 1 if (bad or st) else 0


if __name__ == "__main__":
    sys.exit(main())
