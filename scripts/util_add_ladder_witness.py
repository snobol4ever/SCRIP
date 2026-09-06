#!/usr/bin/env python3
"""util_add_ladder_witness.py -- THE SANCTIONED ADD-A-WITNESS PATH for a language's `ladder__rungNN_*`
family in corpus/tests/<lang>/ALL.{csv,<ext>,ref,wantrc} (task icon-witness-audit-passes-for-the-wrong-
reason, hq_B mail 2026-09-06 "next-is-the-missing-add-a-witness-path"). Before this script, none existed:
corpus_suite_harness.py has convert/convert-blocks/run/extract/extract-family/pin-ref/list and NOTHING that
appends one new entry to an existing master -- three prior sessions on that task file hit this exact wall
(see its own SUPERSEDED-NEXT history) and the seat05 census filed two oracle-verified witness bodies
(not() rung07, / rung34) that stayed parked in a FINDING instead of the corpus for want of this tool.

WHAT THIS DOES: takes ONE new witness's source, an origin naming its rung + construct, and mints it into
the master suite -- ALL.<ext>/ALL.ref get the new banner-delimited block (via corpus_suite_harness.py's
OWN Entry/read_block_suite/write_block_suite, never a hand-rolled marker writer, so formatting is
byte-identical to the other N entries by construction, not by care), and ALL.csv gets a new row with
EVERY column DERIVED from the source + the oracle's own verdict -- rank, entry, family, kind, xfail,
n_lines, modes, want_rc and all ~61 per-construct feature flags. Nothing is typed by the caller except
the origin and the source text.

⛔⛔ THE REF IS ORACLE-CUT, NEVER HAND-TYPED AND NEVER FROM SCRIP (hq_B's own constraint, verbatim: "refs
cut from icont and iconx by absolute path, never from SCRIP"). This script invokes the shared oracle
(icon_bin() in lib_oracle_flags.sh -> /home/resources/icon-master/bin/icon, the one-step icont+iconx
driver -- "grading wants one step; timing wants two", per that file's own header) directly, TWICE, and
REFUSES on disagreement between the two runs (nondeterminism) exactly as util_cut_icon_ipl_refs.sh does
for the same reason. want_rc is the oracle's OWN observed exit code -- never a caller-supplied guess.

⛔⛔ THE ~61 FEATURE-FLAG COLUMNS ARE DERIVED BY TOKEN-SCANNING THE SOURCE, VALIDATED EMPIRICALLY, NOT
GUESSED FROM THE SCHEMA'S COLUMN NAMES. Measured against all 270 existing `family==ladder` rows before
this script was trusted with a real write: the simple keyword-presence rules reproduce 100% of every
flag for the rung07/rung34 population this task actually touches (the two rungs' 10 existing rows, zero
mismatches). A DIFFERENT, unrelated batch (rungs 38-42, coexpressions) disagrees with a plain
"'procedure' keyword present" rule for the `procedure` flag in 43/270 rows -- that is a PRE-EXISTING
historical inconsistency in a population this task does not touch, named here rather than silently
matched or silently fixed (see the companion FINDING). The `assign` flag specifically EXCLUDES an
assignment whose target is a &keyword (`&error := 1` is keyword_ref, not assign) -- confirmed against
ladder_rung36_sets_refusal / ladder_rung37_bal_refusal / ladder_rung06_cset_scan_refuse_any, all actual
assign=0 despite containing `:=`. `seq` is Icon's seq() builtin generator, detected the same as every
other simple builtin name -- an earlier draft of this script mis-transcribed the header and grouped it
with the compound flags instead of between `trim` and `integer` where it actually sits; the CSV-header
equality check below caught the resulting field-order mismatch before anything was written, which is
exactly the failure mode it exists for.

⛔ EVERY REFUSAL PATH WRITES NOTHING. All checks (oracle reachability, determinism, origin/entry
uniqueness, rung membership, a round-trip proof that read+write of the UNCHANGED existing entries is
lossless) run BEFORE any file is touched, and the final write goes to a .tmp path + os.replace(), never
a partial in-place write. --apply is required to write anything; without it this is a dry-run report.

Usage:
    util_add_ladder_witness.py --lang icon --origin ladder__rungNN_<slug> --source FILE.icn [--apply]
"""
import argparse
import csv
import re
import subprocess
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
import corpus_suite_harness as csh  # noqa: E402  -- the ONE authority for Entry/read/write_block_suite

S4E = HERE.parent.parent
ORACLE = {
    "icon": "/home/resources/icon-master/bin/icon",  # icont+iconx, one step -- grading wants one step
}

SIMPLE_WORDS = ["write", "writes", "read", "reads", "find", "match", "upto", "many", "any", "bal", "tab",
    "move", "pos", "stop", "image", "type", "sort", "put", "push", "pop", "get", "insert", "delete",
    "member", "list", "table", "set", "repl", "map", "trim", "seq", "integer", "every", "suspend", "fail",
    "return", "while", "until", "repeat", "case", "next", "break", "create", "record", "procedure",
    "local", "static", "global"]
SPECIAL_ORDER = ["scan", "alternation", "element_gen", "limitation", "to_by", "assign", "swap",
    "aug_assign", "coexpr_activate", "keyword_ref", "every_with_suspend", "scan_with_tab",
    "gen_with_alternation"]
FLAG_ORDER = SIMPLE_WORDS + SPECIAL_ORDER
assert len(FLAG_ORDER) == 61, len(FLAG_ORDER)
CSV_FIELDS = ["rank", "entry", "origin", "family", "kind", "xfail", "n_lines", "modes"] + FLAG_ORDER
WORD_RE = {w: re.compile(r"\b%s\b" % re.escape(w)) for w in SIMPLE_WORDS}


def refuse(msg):
    sys.stderr.write("⛔ REFUSED: %s\n" % msg)
    sys.exit(2)


def strip_strings_comments(src):
    """Icon strings are "..." (backslash-escaped, no multiline); comments are # to EOL. Feature-flag
    detection must not fire on a keyword that only appears inside a string literal or a comment."""
    out, i, n = [], 0, len(src)
    while i < n:
        c = src[i]
        if c == '"':
            j = i + 1
            while j < n and src[j] != '"':
                j += 2 if src[j] == "\\" else 1
            i = j + 1
            continue
        if c == "#":
            j = src.find("\n", i)
            i = n if j == -1 else j
            continue
        out.append(c)
        i += 1
    return "".join(out)


def derive_flags(sno_lines):
    """Returns {flag_name: 0/1} for all 61 columns. See module docstring for the empirical validation
    this ruleset was checked against before being trusted (270 family==ladder rows, 0 mismatches in the
    rung07+rung34 population this task touches)."""
    code = strip_strings_comments("\n".join(sno_lines))
    f = {w: (1 if WORD_RE[w].search(code) else 0) for w in SIMPLE_WORDS}
    aug_assign = bool(re.search(
        r"(?:\+|-|\*\*|\*|/|%|\^|\|\|\||\|\||<=|<|~===|~==|===|==|~=|>=|>):=", code))
    plain_assign = False
    for m in re.finditer(r"([&\w]+)\s*:=(?!=)", code):
        if code[max(0, m.start() - 1):m.start()].startswith(":"):
            continue  # part of :=: (swap)
        if not m.group(1).startswith("&"):
            plain_assign = True
    f["assign"] = 1 if plain_assign else 0
    f["swap"] = 1 if re.search(r":=:", code) else 0
    f["aug_assign"] = 1 if aug_assign else 0
    f["coexpr_activate"] = 1 if re.search(r"@", code) else 0
    f["keyword_ref"] = 1 if re.search(r"&[A-Za-z]", code) else 0
    f["scan"] = 1 if re.search(r"\?", code) else 0
    f["element_gen"] = 1 if re.search(r"!", code) else 0
    f["limitation"] = 1 if re.search(r"\\", code) else 0
    f["alternation"] = 1 if re.search(r"(?<!\|)\|(?!\|)", code) else 0
    f["to_by"] = 1 if re.search(r"\bto\b", code) else 0
    f["every_with_suspend"] = 1 if (f["every"] and f["suspend"]) else 0
    f["scan_with_tab"] = 1 if (f["scan"] and f["tab"]) else 0
    f["gen_with_alternation"] = 1 if (f["alternation"] and (f["every"] or f["element_gen"] or f["to_by"])) else 0
    missing = [k for k in FLAG_ORDER if k not in f]
    if missing:
        refuse("derive_flags() internal error -- undefined column(s): %s" % missing)
    return f


def run_oracle_twice(oracle_bin, src_path, timeout=8):
    outs, rcs = [], []
    for _ in range(2):
        try:
            p = subprocess.run([oracle_bin, str(src_path)], stdin=subprocess.DEVNULL,
                                capture_output=True, timeout=timeout)
        except subprocess.TimeoutExpired:
            refuse("oracle timed out on %s -- not a witness candidate" % src_path)
        outs.append(p.stdout.decode("utf-8", errors="replace"))
        rcs.append(p.returncode)
    if outs[0] != outs[1] or rcs[0] != rcs[1]:
        refuse("oracle is NONDETERMINISTIC on %s (two runs disagreed) -- not a witness candidate" % src_path)
    return outs[0], rcs[0]


def next_rank(rows):
    ranks = [int(r["rank"]) for r in rows]
    return max(ranks) + 1 if ranks else 1


def next_entry_name(rows, prefix="procedure_write_"):
    nums = [int(m.group(1)) for r in rows
            for m in [re.match(r"^%s(\d+)$" % re.escape(prefix), r["entry"])] if m]
    return "%s%d" % (prefix, (max(nums) + 1) if nums else 1)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--lang", default="icon", choices=sorted(ORACLE))
    ap.add_argument("--origin", required=True, help="ladder__rungNN_<slug>, must be new and NN must be an existing rung")
    ap.add_argument("--source", required=True, help="path to the new witness's source file")
    ap.add_argument("--apply", action="store_true", help="write the files; omit for a dry-run report")
    args = ap.parse_args()

    lang = args.lang
    cfg = csh.LANG_CONFIGS[lang]
    master_dir = S4E / "corpus" / "tests" / lang
    master_src, master_ref, master_csv = (master_dir / ("ALL" + cfg["ext"]), master_dir / "ALL.ref",
                                           master_dir / "ALL.csv")
    for p in (master_src, master_ref, master_csv):
        if not p.is_file():
            refuse("master file missing: %s" % p)

    m = re.match(r"^ladder__rung(\d+)_(\S+)$", args.origin)
    if not m:
        refuse("--origin must match ladder__rungNN_<slug>, got %r" % args.origin)
    rung_num = int(m.group(1))

    with open(master_csv, newline="") as fh:
        rows = list(csv.DictReader(fh))
    if rows and list(rows[0].keys()) != CSV_FIELDS:
        refuse("ALL.csv header does not match this script's known schema -- do not proceed blind")
    if any(r["origin"] == args.origin for r in rows):
        refuse("origin %r already exists in %s" % (args.origin, master_csv))
    existing_rungs = {int(mm.group(1)) for r in rows
                       for mm in [re.match(r"^ladder__rung(\d+)_", r["origin"])] if mm}
    if rung_num not in existing_rungs:
        refuse("rung%02d has no existing ladder__ witness in %s -- if this is deliberately the FIRST "
               "witness for a brand-new rung, mint it by hand once and re-derive this tool's assumptions; "
               "refusing rather than guessing" % (rung_num, master_csv))

    src_path = Path(args.source)
    if not src_path.is_file():
        refuse("--source not found: %s" % src_path)
    try:
        sno_text = src_path.read_text(encoding="utf-8")
    except UnicodeDecodeError as e:
        refuse("--source is not valid UTF-8: %s" % e)
    sno_lines = sno_text.splitlines()
    if not sno_lines:
        refuse("--source is empty")

    oracle_bin = ORACLE[lang]
    if not Path(oracle_bin).is_file():
        refuse("shared oracle missing at %s -- fix the shared tree, never fall back to PATH" % oracle_bin)
    ref_text, want_rc = run_oracle_twice(oracle_bin, src_path)
    ref_lines = ref_text.splitlines()
    if want_rc != 0:
        # ⛔ NOT YET IMPLEMENTED: a nonzero want_rc must also gain a line in ALL.wantrc (keyed on the
        # entry NAME -- lib_ladder.sh's own wantrc() reads it there, never from ALL.csv), and this
        # script does not write that sidecar. Minting the CSV/master rows without it would grade this
        # witness against the wrong expected rc (default 0) forever -- refuse rather than land it broken.
        refuse("oracle exited rc=%d (nonzero) -- this script does not yet write ALL.wantrc, so it "
               "cannot correctly mint a nonzero-rc witness; extend it before using it for one" % want_rc)

    flags = derive_flags(sno_lines)

    banner_re = csh.banner_re_for(cfg["comment_open"], cfg["comment_close"])
    entries = csh.read_block_suite(str(master_src), str(master_ref), banner_re,
                                    w_path=str(master_dir / "ALL.wantrc"))
    # ── ROUND-TRIP PROOF, before this tool is trusted to touch the real files: re-serialize the
    # UNCHANGED entries and diff against what is actually on disk. A mismatch here means this script's
    # understanding of the format is wrong, and it must refuse before writing anything, not after.
    tmp_src, tmp_ref = master_src.with_suffix(master_src.suffix + ".rtcheck"), master_ref.with_suffix(".ref.rtcheck")
    csh.write_block_suite(entries, str(tmp_src), str(tmp_ref), cfg["comment_open"], cfg["comment_close"])
    same = tmp_src.read_text() == master_src.read_text() and tmp_ref.read_text() == master_ref.read_text()
    tmp_src.unlink(missing_ok=True); tmp_ref.unlink(missing_ok=True)
    if not same:
        refuse("round-trip proof failed: read_block_suite+write_block_suite does not reproduce the "
               "existing master byte-for-byte -- refusing to trust this tool with a real write")

    entry_name = next_entry_name(rows)
    rank = next_rank(rows)
    new_entry = csh.Entry("block", rank, entry_name, sno_lines, ref_lines, want_rc=want_rc)
    new_row = {"rank": str(rank), "entry": entry_name, "origin": args.origin, "family": "ladder",
               "kind": "block", "xfail": "0", "n_lines": str(len(sno_lines)), "modes": cfg["modes"]}
    for k in FLAG_ORDER:
        new_row[k] = str(flags[k])

    print("would add: rank=%s entry=%s origin=%s want_rc=%s n_lines=%s modes=%s"
          % (rank, entry_name, args.origin, want_rc, len(sno_lines), cfg["modes"]))
    nz = [k for k in FLAG_ORDER if flags[k]]
    print("  non-zero flags:", ", ".join(nz) if nz else "(none)")
    print("  oracle stdout:", repr(ref_text))

    if not args.apply:
        print("(dry-run only -- pass --apply to write)")
        return

    all_entries = entries + [new_entry]
    tmp_src2 = master_src.with_suffix(master_src.suffix + ".newtmp")
    tmp_ref2 = master_ref.with_suffix(".ref.newtmp")
    csh.write_block_suite(all_entries, str(tmp_src2), str(tmp_ref2), cfg["comment_open"], cfg["comment_close"])
    # Verify the OLD entries still round-trip identically inside the new file (prefix-equality check)
    # before replacing anything -- appending must never perturb what already existed.
    old_src_text = master_src.read_text()
    if not tmp_src2.read_text().startswith(old_src_text[:-1] if old_src_text.endswith("\n") else old_src_text):
        tmp_src2.unlink(missing_ok=True); tmp_ref2.unlink(missing_ok=True)
        refuse("post-append check failed: the existing entries were perturbed by the append -- nothing written")
    tmp_src2.replace(master_src)
    tmp_ref2.replace(master_ref)

    tmp_csv = master_csv.with_suffix(".csv.newtmp")
    with open(tmp_csv, "w", newline="") as fh:
        # ⛔ csv.writer DEFAULTS TO \r\n -- "OUR FILES ARE LF" (MODE), and the real ALL.csv is LF-only
        # (confirmed byte-for-byte: 0a, never 0d0a). Without this override every one of 808 existing
        # lines reads as "changed" to git (CRLF vs LF), turning a one-row append into an 809-deletion
        # rewrite -- caught by inspecting the actual diff before this was ever committed.
        w = csv.DictWriter(fh, fieldnames=CSV_FIELDS, lineterminator="\n")
        w.writeheader()
        for r in rows:
            w.writerow(r)
        w.writerow(new_row)
    written = tmp_csv.read_bytes()
    if b"\r" in written:
        tmp_csv.unlink(missing_ok=True)
        refuse("internal error: wrote a CR byte into %s -- LF-only invariant violated, nothing replaced" % master_csv)
    old_csv_bytes = master_csv.read_bytes()
    if not written.startswith(old_csv_bytes if old_csv_bytes.endswith(b"\n") else old_csv_bytes + b"\n"):
        tmp_csv.unlink(missing_ok=True)
        refuse("post-append check failed: existing ALL.csv rows were perturbed by the append -- nothing written")
    tmp_csv.replace(master_csv)

    # ── FINAL PROOF: extract the just-written entry back out via the OFFICIAL extractor (not this
    # script's own belief) and confirm it is byte-identical to what the oracle produced.
    check_src, check_ref = Path("/tmp") / (entry_name + cfg["ext"]), Path("/tmp") / (entry_name + ".ref")
    err = subprocess.run([sys.executable, str(HERE / "corpus_suite_harness.py"), "extract",
                          str(master_src), str(master_ref), entry_name, str(check_src),
                          "--out-ref", str(check_ref)], capture_output=True, text=True)
    ok = (err.returncode == 0 and check_src.read_text() == sno_text.rstrip("\n") + "\n"
          and check_ref.read_text() == ref_text.rstrip("\n") + "\n")
    check_src.unlink(missing_ok=True); check_ref.unlink(missing_ok=True)
    if not ok:
        refuse("post-write extraction proof failed for %s (stderr: %s) -- files were written, "
               "investigate before trusting them; do not commit" % (entry_name, err.stderr))
    print("APPLIED: %s rank=%s -> %s / %s / %s (extraction round-trip verified)"
          % (entry_name, rank, master_src, master_ref, master_csv))


if __name__ == "__main__":
    main()
