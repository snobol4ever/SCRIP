"""Census every activation-frame word the frame allocator lays out against its zls_field registration.

THE CLAIM UNDER TEST (ARCH-GC-COMPILE-TIME-FRAME-MAPS.md section 7 F1, the collector guesses nothing): every
word of an emitted frame that the collector will ever read carries a kind the compiler wrote down at compile
time.  frame_layout.c is where that kind is written (zls_field: offset, size, ZK_DESCR|ZK_RAW|ZK_PTR_GC|
ZK_PTR_CODE), and `scrip --dump-zeta` prints the table it wrote.  This census reads that table for every
graph of every program it is pointed at and reports every word the table does NOT account for:

  region-gap        a byte in [0, region_end) of the graph that no scope's field covers (the classic hole:
                    the return-value cell IR_RETURN writes at frame +0 was never registered -- found by this
                    census on the Raku master, ceo 2026-09-17, CEO-820)
  scope-gap         a byte inside a scope's [lo, hi) between two registered fields
  overlap           a byte two fields of one scope both claim (two kinds for one word)
  vslot-unmapped    a named variable slot (vslot) not covered by exactly one field
  provisional-kind  a field printed with `(audit)` -- a kind nobody verified against the template
  unknown-kind      a kind name outside the four the enum defines

WHAT IT DOES NOT SEE, SAID HERE SO NOBODY READS A CLEAN CENSUS AS A CLEAN FRAME: the emitter's wire header
beyond region_end (the saved gamma/omega continuations and the saved rsp the prologue stores, 48 or 80 bytes;
FN__add stores rcx/rdx/rsp at +120/+128/+136 of a 144-byte frame whose zls region ends at 96) is not in the
zls table and is the cto's per-frame map cell (F6 step 2).  Nor does it see the spine (F1 tagged cells).
The population is the zls REGION, and the summary line says so.

POPULATION: one master suite (--lang, extracted in-process through the harness's own readers, the ONE
extraction authority) or explicit --files.  An entry the compiler refuses (no `; graph` line, or rc != 0)
is NO LAYOUT, named and counted beside the verdict, never silently dropped and never graded green.
⛔⭐ AN ENTRY IS MATERIALIZED THE WAY THE GRADER MATERIALIZES IT, AND THAT IS NOT A DETAIL -- it goes into
its OWN subdir, its -INCLUDE/open()/INPUT() companions are copied in beside it by the harness's own
_copy_companions (transitive closure), scrip runs with that subdir as cwd, and SNO_LIB points at
corpus/include exactly as run_suite_entry sets it.  BEFORE THAT (hq_snobol4, 2026-09-20, SCRIP fff6d8a82 /
corpus 86574b2bf): this tool wrote the entry text into one flat temp dir with neither, so every entry that
names a companion failed to PARSE and landed in no_layout -- SNOBOL4 read no_layout=31 of which THIRTY were
this artifact and ONE was a real refusal (trim_alt_keyword_replace_branch_1, the unlanded lambda(expr)).
The bucket was 1.6% of entries and 45.8% OF THE CENSUSED WORDS: graded 1951 -> 1981, graphs 4008 -> 4506,
words 170578 -> 314812, holes 0 and unkinded 0 throughout, and icon words 164348 -> 164358.  Cost of the
two ingredients over the full seven-language sweep: 34.7s -> 37.4s, one box, one variable at a time.
⛔ THE LESSON IS THE BUCKET, NOT THE INCLUDES: no_layout printed itself as "the compiler refused those
entries" (util_gc_census.py's own wording) -- a CAUSE the tool never measured, for a bucket that held two
classes.  rc is now printed with its first line so every NO-LAYOUT row names its own reason and an rc=2
REFUSAL can never read as an rc=1 red.
rc 0 = graded > 0 and holes == 0; rc 1 = holes > 0; rc 2 = nothing graded (the denominator is zero).
Set ZLS_LIST_ALL=1 to print every hole (the default caps the listing).
THE MACHINE-READ LINE, frozen for the coo's maps census (CEO-821): one per run,
  ZLS-MAP lang=<l> graphs=<n> words=<n> unkinded=<n> holes=<n> graded=<n> no_layout=<n>
words = 8-byte words of every graded graph's zls region; unkinded = words of those inside a reported hole.
Planted arm: SCRIP_TEST_PLANT_ZLS_HOLE=1 makes zls_dump omit the first field of every graph, so a gate can
prove this census still detects a hole the day the real ones are gone (CEO-554: a gate anchored on a found
witness dies when someone cures the witness).
"""
import argparse, os, re, subprocess, sys, tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
KINDS = {"DESCR", "RAW", "PTR_GC", "PTR_CODE"}
RE_GRAPH = re.compile(r"^; graph (\d+) '(.*)' \S+ slots=(\d+) region_end=(\d+) resume=(-?\d+) vslots=(\d+) scopes=(\d+)")
RE_SCOPE = re.compile(r"^;   scope (\d+)\s+(\S+)\s+(\S+)\s+parent=(-?\d+)\s+(?:\[(\d+)\.\.(\d+)\)|\(no frame fields\))")
RE_FIELD = re.compile(r"^;     \+(\d+)\s+(\d+)\s+(\S+)\s+(.*?)\s*$")
RE_VSLOT = re.compile(r"^;   vslot \+(\d+)\s+(\d+)\s+(\S+)\s+(.*?)\s*$")


def parse_dump(text):
    graphs = []
    g = None; sc = None
    for line in text.splitlines():
        m = RE_GRAPH.match(line)
        if m:
            g = {"name": m.group(2), "region_end": int(m.group(4)), "scopes": [], "vslots": []}
            graphs.append(g); sc = None; continue
        if g is None:
            continue
        m = RE_SCOPE.match(line)
        if m:
            sc = {"id": int(m.group(1)), "klass": m.group(2), "name": m.group(3), "fields": [],
                  "lo": int(m.group(5)) if m.group(5) else None, "hi": int(m.group(6)) if m.group(6) else None}
            g["scopes"].append(sc); continue
        m = RE_FIELD.match(line)
        if m and sc is not None:
            what = m.group(4)
            sc["fields"].append({"off": int(m.group(1)), "size": int(m.group(2)), "kind": m.group(3),
                                 "what": what, "audit": what.endswith("(audit)")})
            continue
        m = RE_VSLOT.match(line)
        if m:
            g["vslots"].append({"off": int(m.group(1)), "size": int(m.group(2)), "kind": m.group(3), "name": m.group(4)})
    return graphs


def holes_of(graph):
    out = []
    covered = []
    for sc in graph["scopes"]:
        fs = sorted(sc["fields"], key=lambda f: f["off"])
        for f in fs:
            if f["kind"] not in KINDS:
                out.append(("unknown-kind", sc, f["off"], f["off"] + f["size"], f"kind={f['kind']} {f['what']}"))
            if f["audit"]:
                out.append(("provisional-kind", sc, f["off"], f["off"] + f["size"], f["what"]))
        if not fs:
            continue
        cur = sc["lo"] if sc["lo"] is not None else fs[0]["off"]
        prev = None
        for f in fs:
            if f["off"] > cur:
                out.append(("scope-gap", sc, cur, f["off"], f"between {prev['what'] if prev else 'scope start'} and {f['what']}"))
            elif f["off"] < cur:
                out.append(("overlap", sc, f["off"], min(cur, f["off"] + f["size"]), f"{prev['what'] if prev else '?'} and {f['what']}"))
            cur = max(cur, f["off"] + f["size"]); prev = f
        hi = sc["hi"] if sc["hi"] is not None else cur
        if cur < hi:
            out.append(("scope-gap", sc, cur, hi, f"after {prev['what']}"))
        covered.append((fs[0]["off"], max(cur, hi)))
    covered.sort()
    pos = 0
    for lo, hi in covered:
        if lo > pos:
            out.append(("region-gap", None, pos, lo, "no scope's field covers it"))
        pos = max(pos, hi)
    if pos < graph["region_end"]:
        out.append(("region-gap", None, pos, graph["region_end"], "tail of the region"))
    allf = [f for sc in graph["scopes"] for f in sc["fields"]]
    for v in graph["vslots"]:
        n = sum(1 for f in allf if f["off"] <= v["off"] and v["off"] + v["size"] <= f["off"] + f["size"])
        if n != 1:
            out.append(("vslot-unmapped", None, v["off"], v["off"] + v["size"], f"vslot {v['name']} covered by {n} field(s)"))
    return out


def dump_one(scrip, path, timeout, cwd=None, env=None):
    try:
        p = subprocess.run([str(scrip), "--dump-zeta", str(path)], stdin=subprocess.DEVNULL, capture_output=True, text=True, timeout=timeout, errors="replace", cwd=cwd, env=env)
    except subprocess.TimeoutExpired:
        return None, "timeout"
    if p.returncode != 0:
        first = (p.stdout + p.stderr).strip().splitlines()
        return None, f"rc={p.returncode}: {first[0][:120]}" if first else f"rc={p.returncode}"
    graphs = parse_dump(p.stdout)
    if not graphs:
        return None, "no graph in the dump"
    return graphs, ""


def master_entries(lang, s4e_home):
    import corpus_suite_harness as H
    cfg = H.LANG_CONFIGS.get(lang) or (H.LANG_CONFIGS.get("") if lang == "snobol4" else None) or {"ext": ".sno", "comment_open": "*", "comment_close": ""}
    d = Path(s4e_home) / "corpus" / "tests" / lang
    src = d / ("ALL" + cfg["ext"]); ref = d / "ALL.ref"
    if not src.is_file() or not ref.is_file():
        return None, cfg["ext"], f"master {src} or {ref} missing"
    banner_re = H.banner_re_for(cfg["comment_open"], cfg["comment_close"])
    try:
        entries = H.read_block_suite(str(src), str(ref), banner_re, in_path=H.sidecar_in_path(str(src)), x_path=H.sidecar_xfail_path(str(src)))
    except Exception:
        entries = H.read_suite(str(src), str(ref), in_path=H.sidecar_in_path(str(src)), x_path=H.sidecar_xfail_path(str(src)))
    return entries, cfg["ext"], ""


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--lang", default="", help="census this language's master suite (corpus/tests/<lang>/ALL.<ext>)")
    ap.add_argument("--files", nargs="*", default=[], help="census these programs instead of a master")
    ap.add_argument("--timeout", type=float, default=8.0)
    a = ap.parse_args()
    s4e_home = os.environ.get("S4E_HOME", str(HERE.parent.parent))
    scrip = Path(os.environ.get("SCRIP", str(HERE.parent / "scrip")))
    label = a.lang or "files"
    if not scrip.is_file():
        print(f"zls-frame-map-census[{label}]: REFUSE(2): {scrip} is not a file"); return 2
    if bool(a.lang) == bool(a.files):
        print(f"zls-frame-map-census[{label}]: REFUSE(2): give exactly one of --lang or --files"); return 2
    work = []
    tmp = tempfile.TemporaryDirectory(prefix="zls_census_")
    if a.lang:
        entries, ext, err = master_entries(a.lang, s4e_home)
        if entries is None:
            print(f"zls-frame-map-census[{label}]: REFUSE(2): {err}"); return 2
        import corpus_suite_harness as H
        master_dir = Path(s4e_home) / "corpus" / "tests" / a.lang
        for e in entries:
            text = e.sno_lines[0] if e.kind == "line" else "\n".join(e.sno_lines)
            sub = Path(tmp.name) / re.sub(r"[^A-Za-z0-9_.-]", "_", e.name)
            sub.mkdir(parents=True, exist_ok=True)
            p = sub / (re.sub(r"[^A-Za-z0-9_.-]", "_", e.name) + ext)
            p.write_text(text + "\n")
            H._copy_companions(text, str(master_dir), str(sub))
            work.append((e.name, p, sub))
    else:
        for f in a.files:
            work.append((f, Path(f), None))
    list_all = os.environ.get("ZLS_LIST_ALL") == "1"
    cap = 40
    counts = {"region-gap": 0, "scope-gap": 0, "overlap": 0, "vslot-unmapped": 0, "provisional-kind": 0, "unknown-kind": 0}
    graded = 0; nolayout = []; graphs_n = 0; fields_n = 0; holes = 0; shown = 0; words_n = 0; unkinded_n = 0
    env = dict(os.environ, SNO_LIB=str(Path(s4e_home) / "corpus" / "include"))
    for name, p, cwd in work:
        graphs, err = dump_one(scrip, p, a.timeout, cwd=(str(cwd) if cwd else None), env=env)
        if graphs is None:
            nolayout.append((name, err)); continue
        graded += 1
        for g in graphs:
            graphs_n += 1; words_n += g["region_end"] // 8
            fields_n += sum(len(sc["fields"]) for sc in g["scopes"])
            for cls, sc, lo, hi, detail in holes_of(g):
                holes += 1; counts[cls] += 1; unkinded_n += (hi - lo + 7) // 8
                if list_all or shown < cap:
                    shown += 1
                    scn = f"scope={sc['id']}:{sc['name']}" if sc else "scope=-"
                    print(f"HOLE {cls} lang={label} entry={name} graph='{g['name']}' {scn} off=[{lo}..{hi}) {detail}")
    if holes > shown:
        print(f"... {holes - shown} more hole(s) not listed (ZLS_LIST_ALL=1 prints every one)")
    for name, err in nolayout[:cap]:
        print(f"NO-LAYOUT lang={label} entry={name} ({err})")
    if len(nolayout) > cap:
        print(f"... {len(nolayout) - cap} more NO-LAYOUT entries")
    cls_s = " ".join(f"{k}={v}" for k, v in counts.items())
    print(f"ZLS-MAP lang={label} graphs={graphs_n} words={words_n} unkinded={unkinded_n} holes={holes} graded={graded} no_layout={len(nolayout)}")
    print(f"zls-frame-map-census[{label}]: entries={len(work)} graded={graded} no_layout={len(nolayout)} graphs={graphs_n} fields={fields_n} holes={holes} ({cls_s}) population=the zls region only; the wire header past region_end and the spine are not censused here")
    if graded == 0:
        print(f"zls-frame-map-census[{label}]: REFUSE(2): nothing graded"); return 2
    return 1 if holes else 0


if __name__ == "__main__":
    sys.exit(main())
