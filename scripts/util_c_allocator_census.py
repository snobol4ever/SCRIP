#!/usr/bin/env python3
"""util_c_allocator_census.py -- THE CENSUS THAT PROVES ZERO, AND SAYS WHERE EACH SITE WENT (coo, CEO-842/843).

LON'S ACCEPTANCE TEST IS TEXTUAL AND IT IS HIS (2026-09-18, in-chat to ceo, verbatim: "Eradicate ALL usage of
malloc, calloc, free, and realloc.  At the end ensure 0 instances found in the SCRIP source base."), so half of
this tool is a count of four names that must reach 0 over the WHOLE tree -- src/parsers and the checked-in
generated flex/bison output included, because a generated file that is committed is source.

⛔⭐ THE OTHER HALF IS THE ceo's OWN WARNING AND IT IS WHY THIS IS NOT A grep (CEO-843, verbatim): "a census that
only counts the four names would read green on a tree where half the runtime quietly lives outside the
collector."  The rule names THREE destinations and only one of them is right for a given site:

  ROOTED-HEAP   rt_ws_alloc / rt_ws_realloc -- anything the RUNNING PROGRAM can reach, walked like everything
                else, with a root.  If you are unsure, this is the answer (CEO-842).
  ARENA         ct_alloc / ct_zalloc / ct_grow / ct_strdup / ct_strndup / ct_drop -- anything only the COMPILER
                touches, dead before the emitted program runs.
  MMAP          the collector's OWN bookkeeping (page map, mark worklist, live-block array) reserves address
                space directly.  mmap is not one of the four names and a region whose lifetime rule is stated is
                the opposite of handing an object to an allocator the collector cannot see.

⛔ THE ANTI-EVASION CLAUSE IS THE POPULATION THIS TOOL EXISTS FOR: THE ARENA MAY NOT HOLD ANYTHING THE RUNTIME
CAN REACH -- that is the same evasion as malloc or a pin, one name further out, reverted on sight.  So an ARENA
call site under src/runtime/ is counted as a VIOLATION and named, with gc_heap.c's own bookkeeping the one place
MMAP is expected.  A tree can satisfy Lon's grep exactly while failing this, which is the whole point.

⛔ IT COUNTS CALLS, NOT WORDS.  Comments and string literals are stripped, a declaration or prototype is not a
call, and `ct_free` / `rt_ws_realloc` are not `free` / `realloc` -- the word-boundary does that, and an arm plants
it.  Evasions the rule names by hand are counted too: `#define X malloc`, a typedef'd pointer assigned one of the
four, and a wrapper whose body calls one.

⛔ IT REFUSES RATHER THAN PASSING VACUOUSLY.  No source tree, no readable file, or a tree where the four names
read 0 AND no destination symbol appears anywhere -- which would mean nothing in this compiler allocates at all
-- is rc=2, NOT GREEN.  A zero from a tool that read nothing is not a zero.

rc: 0 forbidden=0 and no arena-in-runtime violation; 1 either remains; 2 could not measure.
Usage: python3 scripts/util_c_allocator_census.py [--root DIR] [--ratchet FILE] [--write-baseline FILE]
                                                  [--by-dir] [--sites] [--selftest]
"""
import argparse, os, re, sys, tempfile

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, ".."))

FORBIDDEN = ("malloc", "calloc", "realloc", "free")
DESTINATIONS = {
    "ROOTED-HEAP": ("rt_ws_alloc", "rt_ws_realloc", "rt_ws_zalloc"),
    "ARENA":       ("ct_alloc", "ct_zalloc", "ct_grow", "ct_strdup", "ct_strndup", "ct_drop", "ct_calloc", "ct_realloc"),
    "MMAP":        ("mmap",),
}
SRC_EXT = (".c", ".h", ".cpp", ".hpp", ".cc", ".S", ".s", ".y", ".l")
COUNTS = {}


def strip_comments(src):
    """comments and string literals out -- a name in prose or in a message is not a call site"""
    src = re.sub(r"/\*.*?\*/", lambda m: "\n" * m.group(0).count("\n"), src, flags=re.S)
    src = re.sub(r"//[^\n]*", "", src)
    src = re.sub(r'"(?:[^"\\\n]|\\.)*"', '""', src)
    return src


def call_rx(name):
    return re.compile(r"(?<![A-Za-z0-9_])" + re.escape(name) + r"\s*\(")


_DECL_HEAD = re.compile(r"^\s*(?:(?:static|extern|inline|const|unsigned|signed|struct|union|enum|_Noreturn|"
                        r"register|volatile|__attribute__\s*\(\([^)]*\)\))\s+)*[A-Za-z_][A-Za-z0-9_]*\s*\**\s*$")
_HEAD_KEYWORDS = {"return", "else", "do", "while", "if", "case", "goto", "sizeof", "switch", "for"}


def is_declaration(line, upto):
    head = line[:upto]
    if not head.strip():
        return False
    m = _DECL_HEAD.match(head)
    if not m:
        return False
    last = re.findall(r"[A-Za-z_][A-Za-z0-9_]*", head)
    return bool(last) and last[-1] not in _HEAD_KEYWORDS


def source_files(root):
    fs = []
    sd = os.path.join(root, "src")
    if not os.path.isdir(sd):
        return fs
    for d, _s, names in os.walk(sd):
        for n in sorted(names):
            if n.endswith(SRC_EXT):
                fs.append(os.path.join(d, n))
    return sorted(fs)


_DEFINE_RX = re.compile(r"^[ \t]*#[ \t]*define[ \t]+([A-Za-z_][A-Za-z0-9_]*)[ \t]*(?:\([^)]*\))?[ \t]*(.*)$")
_CPP_RX = re.compile(r"^\s*#\s*(?:define|include|if|ifdef|ifndef|elif|else|endif|undef|pragma|error)\b")
_BARE_RX = re.compile(r"(?<![A-Za-z0-9_])(" + "|".join(FORBIDDEN) + r")(?![A-Za-z0-9_])\s*(?!\()")
_WORD_RX = re.compile(r"[A-Za-z_][A-Za-z0-9_]*")


def resolve_aliases(defines):
    """⛔ THE ALIAS QUESTION IS TRANSITIVE AND THE ONE-LEVEL ANSWER IS WRONG TWICE (CEO-844 clause 1).

    bison writes `#define YYSTACK_ALLOC YYMALLOC` beside `#define YYMALLOC malloc`: a reader who only asks
    whether a define's body IS a forbidden name sees neither the chain nor the 15 calls made through it, and a
    four-name grep sees nothing at all.  And a substring reader gets a FALSE alias out of flex's
    `#define yyrealloc pascal_yyrealloc`, which renames flex's own hook and is not C realloc under any spelling.
    So: parse each define into (name, body), then ask whether the body REACHES one of the four through other
    defines, by whole word.  Returns {(rel, line, name): (forbidden_name, chain)}.
    """
    body_of = {}
    for _rel, _ln, nm, body in defines:
        body_of.setdefault(nm, []).append(body)

    def reach(nm, seen):
        if nm in FORBIDDEN:
            return (nm, [nm])
        if nm in seen or nm not in body_of:
            return None
        seen = seen | {nm}
        for body in body_of[nm]:
            for tok in _WORD_RX.findall(body):
                got = reach(tok, seen)
                if got:
                    return (got[0], [nm] + got[1])
        return None

    found = {}
    for rel, ln, nm, _body in defines:
        if nm in FORBIDDEN:
            continue
        got = reach(nm, frozenset())
        if got:
            found[(rel, ln, nm)] = got
    return found


def scan(files, root):
    """(forbidden, destinations, aliases, alias_calls, prose, bare) -- the four names, where the converted
    sites went, every spelling that reaches them, every CALL through such a spelling, and the two residues a
    textual grep hits that are not call sites at all."""
    forb, dest, defines, prose, bare = [], [], [], {}, []
    rxs = [(n, call_rx(n)) for n in FORBIDDEN]
    drx = [(d, n, call_rx(n)) for d, syms in DESTINATIONS.items() for n in syms]
    word_rx = re.compile(r"(?<![A-Za-z0-9_])(" + "|".join(FORBIDDEN) + r")(?![A-Za-z0-9_])")
    texts = {}
    for f in files:
        try:
            raw = open(f, encoding="utf-8", errors="replace").read()
        except OSError:
            continue
        rel = os.path.relpath(f, root)
        src = strip_comments(raw)
        texts[rel] = src
        # ⛔ THE PROSE RESIDUE IS MEASURED HERE AND NEVER FOLDED INTO THE COUNT (CEO-844 clause 2).  The
        # generated bison and flex files carry the GPL sentence "This program is free software"; that sentence
        # is not usage.  An instrument that counts it reads a permanent false red, and the cheapest way to
        # clear a false red is to delete a licence header -- so this population is printed APART, with its
        # files, and a reader who sees it knows why Lon's textual grep will not reach 0 on the same day the
        # call sites do.
        n_prose = len(word_rx.findall(raw)) - len(word_rx.findall(src))
        if n_prose > 0:
            prose[rel] = n_prose
        lines = src.split("\n")
        for i, line in enumerate(lines, 1):
            for name, rx in rxs:
                for m in rx.finditer(line):
                    if is_declaration(line, m.start()):
                        continue
                    forb.append((rel, i, name))
            for d, name, rx in drx:
                for m in rx.finditer(line):
                    if is_declaration(line, m.start()):
                        continue
                    dest.append((rel, i, name, d))
            m = _DEFINE_RX.match(line)
            if m:
                defines.append((rel, i, m.group(1), m.group(2)))
                continue
            for m in _BARE_RX.finditer(line):
                if is_declaration(line, m.start()):
                    continue
                bare.append((rel, i, m.group(1), line.strip()[:78]))

    aliases = resolve_aliases(defines)
    # every CALL made through a spelling that reaches a forbidden name -- the population a four-name grep and a
    # one-level alias reader BOTH miss, and the one that lets a tree read 0 while still calling malloc.
    alias_calls = []
    if aliases:
        names = sorted({nm for (_r, _l, nm) in aliases})
        arx = [(nm, call_rx(nm)) for nm in names]
        for rel, src in texts.items():
            for i, line in enumerate(src.split("\n"), 1):
                if _CPP_RX.match(line):
                    continue
                for nm, rx in arx:
                    for m in rx.finditer(line):
                        if is_declaration(line, m.start()):
                            continue
                        alias_calls.append((rel, i, nm, aliases_lookup(aliases, nm)))
    return forb, dest, aliases, alias_calls, prose, bare, texts


def aliases_lookup(aliases, nm):
    for (_r, _l, n), (target, _chain) in aliases.items():
        if n == nm:
            return target
    return "?"


def runtime_reachable(rel):
    """the question CEO-842 says to ask: can the emitted program still reach this after the compile ends?
    src/runtime is the directory where the answer is yes by construction."""
    return rel.startswith("src/runtime" + os.sep) or rel.startswith("src/runtime/")


def is_collector_bookkeeping(rel):
    return os.path.basename(rel) in ("gc_heap.c", "gc_heap.h")


SITES_BASELINE = os.path.join(HERE, "c_allocator_sites_baseline.tsv")

_GLOBAL_DECL = re.compile(r"^(?:static\s+|extern\s+)?(?:const\s+)?[A-Za-z_][A-Za-z0-9_]*\s+(\**)\s*"
                          r"([A-Za-z_][A-Za-z0-9_]*)\s*(\[|=|;)", re.M)
_ROOTS_FN = re.compile(r"^(?:static\s+)?void\s+([A-Za-z_][A-Za-z0-9_]*_gc_roots)\s*\([^)]*\)\s*\{", re.M)
_INDEXED = re.compile(r"(?<![A-Za-z0-9_])([A-Za-z_][A-Za-z0-9_]*)\s*(?:\[|->)")
_ADDR_OF = re.compile(r"&\s*([A-Za-z_][A-Za-z0-9_]*)\s*(?![\w\[])")


def container_census(texts, out=print):
    """⛔⭐ MARK THE CONTAINER, NOT ONLY WHAT IT POINTS AT (CEO-846; the cfo's CFO-96/97 find).

    A root walk that visits a container's CONTENTS but never marks the CONTAINER READS AS ROOTED TO EVERY
    CENSUS EVER WRITTEN -- including this one's destination column, which only asks where an allocation went.
    g_name_save was reclaimed under a live call while its walk existed and was being called.  So this asks a
    different question of each `*_gc_roots` function: which globals does it index or dereference, and does it
    ever pass the ADDRESS OF THE CONTAINER ITSELF to a visitor?

    ⛔ THE DECLARATION DECIDES WHETHER A MISS IS LIVE OR LATENT, and getting this backwards is how the arm
    becomes noise: `static NV_t *_var_buckets[VAR_BUCKETS]` is an ARRAY of pointers in static storage -- there
    is no container block to mark, and marking each slot with `&_var_buckets[b]` is correct and sufficient.
    `static DESCR_t *g_pas_heap` is a POINTER AT A BLOCK: visiting `&g_pas_heap[n]` marks the contents and
    leaves the block itself unmarked, which is the g_name_save shape exactly.  A fixed array is reported as
    LATENT rather than clean, because it becomes the live defect the moment its storage moves to the collected
    heap -- which is what this whole sweep is doing.

    ⛔ AND THE HONEST HALF: this is a SYNTACTIC read of the root walks, not a proof.  It cannot see a container
    marked through a helper function, an alias, or a walk not named `*_gc_roots`, and it says so rather than
    certifying the tree.
    """
    gdecl = {}
    for rel, src in texts.items():
        for m in _GLOBAL_DECL.finditer(src):
            star, sym, tail = m.group(1), m.group(2), m.group(3)
            kind = "array" if tail == "[" else ("pointer" if star else "scalar")
            gdecl.setdefault(sym, set()).add(kind)
    live, latent, walks = [], [], 0
    for rel, src in sorted(texts.items()):
        for fm in _ROOTS_FN.finditer(src):
            walks += 1
            name = fm.group(1)
            i, depth = fm.end(), 1
            while i < len(src) and depth:
                depth += 1 if src[i] == "{" else (-1 if src[i] == "}" else 0)
                i += 1
            body = src[fm.end():i]
            indexed = {x for x in _INDEXED.findall(body) if x in gdecl}
            marked = set(_ADDR_OF.findall(body))
            for sym in sorted(indexed - marked):
                kinds = gdecl[sym]
                row = (rel, name, sym, "/".join(sorted(kinds)))
                (live if ("pointer" in kinds and "array" not in kinds) else latent).append(row)
    out(f"CENSUS c-allocators CONTAINER-UNMARKED LIVE={len(live)} want=0 LATENT={len(latent)} over {walks} root "
        f"walk(s) -- MARK THE CONTAINER, NOT ONLY WHAT IT POINTS AT (CEO-846, CFO-96/97). A walk that visits a "
        f"container's contents and never the container reads as ROOTED to every census ever written, this one's "
        f"destination column included: g_name_save was reclaimed under a live call while its walk existed and "
        f"was called")
    for rel, fn, sym, kind in live:
        out(f"  ⛔ CONTAINER-UNMARKED-LIVE {rel} {fn}() indexes {sym} ({kind} at a heap block) and never marks the "
            f"block -- visiting {sym}[i] marks the CONTENTS and leaves the container collectable")
    for rel, fn, sym, kind in latent[:10]:
        out(f"  CONTAINER-UNMARKED-LATENT {rel} {fn}() indexes {sym} ({kind}) -- static storage today, so there "
            f"is no block to mark and marking each slot is sufficient; it becomes the live defect the moment "
            f"that storage moves to the collected heap")
    out(f"  ⛔ LIMIT, STATED RATHER THAN CERTIFIED: this is a SYNTACTIC read of functions named *_gc_roots. It "
        f"cannot see a container marked through a helper, through an alias, or by a walk under another name, "
        f"and a LIVE=0 from it is not a proof that every container in the tree is marked.")
    return live, latent


def per_file(forb, dest):
    """{relpath: {forbidden, rooted_heap, arena, mmap}} -- the shape the provenance split is keyed on."""
    agg = {}
    def cell(rel):
        return agg.setdefault(rel, {"forbidden": 0, "rooted_heap": 0, "arena": 0, "mmap": 0})
    for rel, _l, _n in forb:
        cell(rel)["forbidden"] += 1
    for rel, _l, _n, d in dest:
        cell(rel)[{"ROOTED-HEAP": "rooted_heap", "ARENA": "arena", "MMAP": "mmap"}[d]] += 1
    return agg


def read_sites_baseline(path):
    if not os.path.exists(path):
        return None
    base = {}
    for line in open(path, encoding="utf-8"):
        if line.startswith("#") or not line.strip():
            continue
        f = line.rstrip("\n").split("\t")
        if len(f) >= 5:
            base[f[0]] = {"forbidden": int(f[1]), "rooted_heap": int(f[2]), "arena": int(f[3]), "mmap": int(f[4])}
    return base


def provenance(now, base):
    """⛔⭐ THE SPLIT CEO-846 ORDERED, AND IT TURNS ENTIRELY ON WHAT A SITE WAS BEFORE.

    The clause `the arena may not hold anything the runtime can reach` was written as one rule over two
    different acts, and only one of them is the evasion:

      CLASS 1 -- a site that was on the COLLECTED HEAP and is now in the ARENA.  That is moving an object out
        of the collector's sight to avoid rooting it: the cfo's f62a33aed shape under a new name.  HARD RED,
        named, never ratcheted, never transitional.
      CLASS 2 -- a site that was a libc `malloc` and is now in the ARENA.  Not a cure and not an evasion: the
        lifetime is identical to the malloc it replaced (never moves, never freed), so it is exactly as safe
        as what it replaced AND it removes a forbidden call.  DEBT.  Counted, ratcheted to zero as each
        holder is rooted, may only FALL.

    ⛔ THE RESOLUTION OF THIS INSTRUMENT IS THE FILE, NOT THE SITE, and saying so is the honest half: line
    numbers do not survive a sweep that rewrites 79 files, so the discriminator is a CONSERVATION ARGUMENT per
    file.  If a file's rooted-heap count FELL while its arena count ROSE, that many sites moved off the
    collected heap into the arena and it is class 1 -- whatever the intervening edits were.  The rest of the
    arena growth is attributed to the forbidden calls the same file lost, which is class 2.  A file that both
    roots a new holder and arenas an old malloc in one landing is reported at its NET, and a class-1 site
    hidden under a same-file class-2 credit is the one shape this cannot see; it is named in the output so
    nobody reads the number as site-exact.
    """
    ev, debt, unknown = [], [], []
    for rel, cur in sorted(now.items()):
        if not runtime_reachable(rel) or cur["arena"] == 0:
            continue
        was = base.get(rel)
        if was is None:
            unknown.append((rel, cur["arena"]))
            continue
        d_arena = cur["arena"] - was["arena"]
        d_rooted = was["rooted_heap"] - cur["rooted_heap"]
        d_forb = was["forbidden"] - cur["forbidden"]
        if d_arena <= 0:
            continue
        n_ev = min(d_rooted, d_arena) if d_rooted > 0 else 0
        if n_ev > 0:
            ev.append((rel, n_ev, was["rooted_heap"], cur["rooted_heap"], was["arena"], cur["arena"]))
        n_debt = d_arena - n_ev
        if n_debt > 0:
            debt.append((rel, n_debt, d_forb))
    return ev, debt, unknown


def census(root, by_dir=False, sites=False, out=print):
    files = source_files(root)
    if not files:
        out(f"CENSUS c-allocators REFUSED(2): no source files under {os.path.join(root, 'src')} -- a zero from a "
            f"tool that read nothing is not a zero")
        return 2
    forb, dest, aliases, alias_calls, prose, bare, texts_for_container = scan(files, root)
    out(f"CENSUS c-allocators SCOPE files={len(files)} under src/ (every {'/'.join(SRC_EXT)} file, generated flex "
        f"and bison output INCLUDED -- a generated file that is checked in is source, CEO-842)")

    per = {n: sum(1 for s in forb if s[2] == n) for n in FORBIDDEN}
    total = len(forb)
    out("CENSUS c-allocators FORBIDDEN total=%d (%s) want=0 -- Lon's acceptance test, CEO-842" %
        (total, " ".join(f"{n}={per[n]}" for n in FORBIDDEN)))
    # ⛔ THE DECOMPOSITION IS PRINTED BECAUSE TWO HONEST TOOLS GET TWO DIFFERENT TOTALS HERE AND THE DIFFERENCE IS
    # NOT AN ERROR.  A grammar's action code is COPIED into its generated twin, so `raku.y` and `raku.tab.c` hold
    # the same call twice -- and BOTH are checked in, so Lon's "0 instances in the SCRIP source base" binds on
    # both.  A cure applied only to the generated file is undone by the next regeneration; a cure applied only to
    # the grammar leaves the shipped file dirty until someone regenerates.  Neither half is optional, and a reader
    # comparing this total against one taken over .c/.h alone needs the split rather than a discrepancy.
    gen = [x for x in forb if ".lex.c" in x[0] or ".tab.c" in x[0]]
    gram = [x for x in forb if x[0].endswith((".y", ".l"))]
    hand = total - len(gen) - len(gram)
    out(f"CENSUS c-allocators SPLIT hand_written={hand} generated_lex_tab={len(gen)} grammar_y_l={len(gram)} -- the "
        f"last two OVERLAP by construction (a grammar's action code is copied into its generated twin) and both "
        f"are checked in, so both must reach zero: a cure in only one of the pair is undone by the next regeneration")
    out(f"CENSUS c-allocators ALIASES={len(aliases)} ALIAS-CALLS={len(alias_calls)} want=0/0 -- the rule forbids "
        f"these four under ANY spelling (CEO-844): a define whose body REACHES a forbidden name, through however "
        f"many other defines, is that name, and every CALL through such a spelling is a call site a four-name "
        f"grep cannot see")
    for (rel, ln, nm), (target, chain) in sorted(aliases.items())[:12]:
        out(f"  ALIAS {rel}:{ln} {nm} -> {target}   via {' -> '.join(chain)}")
    agg_ac = {}
    for rel, _ln, nm, target in alias_calls:
        agg_ac.setdefault((nm, target), []).append(rel)
    for (nm, target), rels in sorted(agg_ac.items(), key=lambda kv: -len(kv[1]))[:12]:
        out(f"  ALIAS-CALL {nm}() -> {target}  {len(rels)} call(s) in {len(set(rels))} file(s): "
            f"{', '.join(sorted(set(rels))[:4])}")

    dper = {}
    for _rel, _ln, _sym, d in dest:
        dper[d] = dper.get(d, 0) + 1
    out("CENSUS c-allocators DESTINATIONS " + " ".join(f"{d}={dper.get(d, 0)}" for d in DESTINATIONS) +
        " -- where the converted sites went; a count of the four names alone would read green on a tree where "
        "half the runtime quietly lives outside the collector (CEO-843)")

    # ⛔ THE ANTI-EVASION POPULATION
    viol = [(r, l, s) for (r, l, s, d) in dest if d == "ARENA" and runtime_reachable(r)]
    mmap_out = [(r, l, s) for (r, l, s, d) in dest if d == "MMAP" and not is_collector_bookkeeping(r)]
    out(f"CENSUS c-allocators ARENA-IN-RUNTIME={len(viol)} -- the arena holding runtime-reachable memory. ⛔ THIS "
        f"TOTAL IS NOT A VERDICT ON ITS OWN (CEO-846): it is split below by WHAT EACH SITE WAS BEFORE, because "
        f"the clause was one rule over two different acts and only one of them is the evasion")
    for rel, ln, sym in viol[:15]:
        out(f"  ARENA-IN-RUNTIME {rel}:{ln} {sym}")

    # ⛔⭐ THE CLASS SPLIT (CEO-846).  Class 1 is the evasion and is a hard red; class 2 is transitional debt
    # that the ratchet drives to zero.  The discriminator is the per-file provenance baseline, and WITHOUT IT
    # THE SPLIT IS NOT COMPUTABLE -- so it REFUSES rather than guessing, and never silently calls debt an
    # evasion (which would red an honest landing) or an evasion debt (which would admit the very thing the
    # clause exists to stop).
    live_c, latent_c = container_census(texts_for_container, out=out)
    COUNTS["container_unmarked_live"] = len(live_c)

    sbase = read_sites_baseline(SITES_BASELINE)
    now_pf = per_file(forb, dest)
    COUNTS["arena_in_runtime"] = len(viol)
    if sbase is None:
        out(f"CENSUS c-allocators ARENA-SPLIT REFUSED(2): no per-file provenance baseline at {SITES_BASELINE} -- "
            f"the discriminator is what each site WAS, and without it this instrument cannot tell the evasion "
            f"(collected heap -> arena) from transitional debt (malloc -> arena). It does not guess.")
        COUNTS["arena_in_runtime_evasion"] = None
        COUNTS["arena_in_runtime_debt"] = None
        prov_refused = True
    else:
        prov_refused = False
        ev, debt, unknown = provenance(now_pf, sbase)
        n_ev = sum(x[1] for x in ev)
        n_debt = sum(x[1] for x in debt)
        n_unk = sum(x[1] for x in unknown)
        COUNTS["arena_in_runtime_evasion"] = n_ev + n_unk
        COUNTS["arena_in_runtime_debt"] = n_debt
        out(f"CENSUS c-allocators ARENA-SPLIT CLASS-1-EVASION={n_ev} want=0 HARD CLASS-2-DEBT={n_debt} ratcheted "
            f"UNATTRIBUTABLE={n_unk} -- class 1 is a site that was on the COLLECTED HEAP and is now in the arena "
            f"(moving an object out of the collector's sight to avoid rooting it, reverted on sight); class 2 was "
            f"a libc malloc and is now in the arena, the same lifetime it already had, debt and not a cure. ⛔ THE "
            f"RESOLUTION IS THE FILE, NOT THE SITE: a class-1 site hidden under a same-file class-2 credit in the "
            f"same landing is the one shape this cannot see, so read the number as per-file NET, not site-exact")
        for rel, n, r0, r1, a0, a1 in ev[:15]:
            out(f"  ⛔ CLASS-1-EVASION {rel} {n} site(s): rooted_heap {r0} -> {r1} while arena {a0} -> {a1} -- "
                f"THE CURE FOR AN UNROOTED HOLDER IS A ROOT, NEVER A DIFFERENT ALLOCATOR")
        for rel, n, d_forb in debt[:12]:
            out(f"  CLASS-2-DEBT {rel} {n} site(s) (the file lost {d_forb} forbidden call(s)) -- ratcheted to 0 as "
                f"the holder is rooted")
        for rel, n in unknown[:12]:
            out(f"  ⛔ UNATTRIBUTABLE {rel} {n} arena site(s) in a file the provenance baseline does not name -- "
                f"counted WITH the evasion, because a site whose history this instrument cannot read is not "
                f"given the benefit of the doubt")
    out(f"CENSUS c-allocators MMAP-OUTSIDE-GC_HEAP={len(mmap_out)} NAMED, not a verdict -- mmap belongs to the "
        f"collector's own bookkeeping; elsewhere it wants a reason in its commit")
    for rel, ln, sym in mmap_out[:10]:
        out(f"  MMAP-OUTSIDE {rel}:{ln} {sym}")

    # ⛔ THE TWO RESIDUES A TEXTUAL GREP HITS THAT ARE NOT CALL SITES.  Lon's acceptance test is a grep, so the
    # day the call sites reach 0 his grep will still print these -- and a reader who has not been told why will
    # either call the instrument a liar or delete a licence header to make the number move.  Both are worse than
    # the residue.  Printed APART, never folded into FORBIDDEN, never ratcheted as a defect.
    n_prose = sum(prose.values())
    out(f"CENSUS c-allocators PROSE-RESIDUE={n_prose} in {len(prose)} file(s) -- NOT call sites and NOT in the "
        f"count above: the four names inside comments and string literals, most of them the GPL sentence "
        f"'This program is free software' that bison and flex write into every generated file. ⛔ A LICENCE "
        f"HEADER IS NEVER EDITED TO MAKE A GREP READ ZERO (CEO-844 clause 2)")
    for rel, n in sorted(prose.items(), key=lambda kv: (-kv[1], kv[0]))[:12]:
        out(f"  PROSE {rel} {n}")
    out(f"CENSUS c-allocators NON-CALL-REFS={len(bare)} -- NOT call sites and NOT in the count above: the four "
        f"names in preprocessor guards and #include <malloc.h>, which a grep hits and a compiler never calls; a "
        f"genuine function-pointer hand-off (`= free`, `, free)`) would land here too and IS a site to convert")
    for rel, ln, nm, txt in bare[:10]:
        out(f"  NON-CALL-REF {rel}:{ln} {nm} | {txt}")

    if total == 0 and not dest:
        out("CENSUS c-allocators REFUSED(2): the four names read 0 AND no destination symbol appears anywhere -- "
            "that would mean nothing in this compiler allocates at all, which is not a tree, it is a tool that "
            "read the wrong thing. A vacuous zero is not a pass.")
        return 2

    if by_dir:
        agg = {}
        for rel, _l, n in forb:
            d = os.path.dirname(rel)
            agg.setdefault(d, {})[n] = agg.setdefault(d, {}).get(n, 0) + 1
        for d in sorted(agg, key=lambda k: -sum(agg[k].values())):
            out(f"  BY-DIR {d:32s} " + " ".join(f"{n}={agg[d].get(n, 0)}" for n in FORBIDDEN) +
                f"  total={sum(agg[d].values())}")
    if sites:
        for rel, ln, n in forb:
            out(f"  FORBIDDEN-SITE {rel}:{ln} {n}")

    COUNTS["__per_file__"] = now_pf
    COUNTS["forbidden_total"] = total
    for n in FORBIDDEN:
        COUNTS[n] = per[n]
    COUNTS["aliases"] = len(aliases)
    COUNTS["alias_calls"] = len(alias_calls)
    COUNTS["prose_residue"] = n_prose
    red = total > 0 or aliases or alias_calls or COUNTS.get("arena_in_runtime_evasion") or prov_refused
    out(f"CENSUS c-allocators {'RED' if red else 'GREEN'}")
    return 1 if red else 0


# ⛔ prose_residue is MEASURED and PRINTED but deliberately NOT ratcheted: it is licence text, it is not ours to
# drive to zero, and a ratchet on it is an instruction to edit a copyright header.  alias_calls IS ratcheted --
# that one is ours and it is the half a four-name grep cannot see.
# ⛔ arena_in_runtime is the TOTAL and is ratcheted like the rest; the two that carry the meaning are split out
# per CEO-846 -- _evasion is a HARD 0 (it is never debt and never transitional) and _debt only ever falls.
RATCHET_KEYS = ["forbidden_total", "malloc", "calloc", "realloc", "free", "aliases", "alias_calls",
                "arena_in_runtime", "arena_in_runtime_evasion", "arena_in_runtime_debt",
                "container_unmarked_live"]


def ratchet(path, out=print):
    if not os.path.exists(path):
        out(f"RATCHET REFUSED(2): no baseline at {path}"); return 2
    base = {}
    for line in open(path, encoding="utf-8"):
        if line.startswith("#") or not line.strip():
            if line.startswith("# CRITERION CHANGED"):
                out("RATCHET " + line.rstrip()[2:])
            continue
        f = line.rstrip("\n").split("\t")
        if len(f) >= 2:
            base[f[0]] = int(f[1])
    worse, better, unmeasured = [], [], []
    for k in RATCHET_KEYS:
        want, got = base.get(k), COUNTS.get(k)
        if want is None:
            unmeasured.append(f"{k} (not in the baseline)"); continue
        if got is None:
            unmeasured.append(f"{k} (not measured this run)"); continue
        mark = "="
        if got > want:
            worse.append(f"{k} {want} -> {got}"); mark = "WORSE"
        elif got < want:
            better.append(f"{k} {want} -> {got}"); mark = "BETTER"
        out(f"RATCHET {k} baseline={want} now={got} {mark}")
    out(f"population: {len(RATCHET_KEYS)} ratcheted count(s), {len(worse)} worse, {len(better)} better, "
        f"{len(unmeasured)} not measured")
    for u in unmeasured:
        out(f"  NOT-MEASURED {u}")
    for w in worse:
        out(f"  ⛔ WORSE {w} -- the rule drives these to 0; a landing may not add one")
    for b in better:
        out(f"  ⭐ BETTER {b} -- lower the baseline in the landing that earned it")
    if unmeasured:
        out("RATCHET REFUSED(2): a count the baseline names was not measured -- not green"); return 2
    if worse:
        out("RATCHET RED(1): a forbidden or arena-in-runtime count went UP"); return 1
    if better:
        out("RATCHET RED(1): a count FELL and the baseline still names the old number -- record the win"); return 1
    out("RATCHET GREEN: every ratcheted count is exactly its baseline"); return 0


ARMS_FLOOR = 24


def selftest():
    fails = arms = 0
    def ck(ok, label):
        nonlocal fails, arms
        arms += 1
        print(("  ok    " if ok else "  FAIL  ") + label)
        if not ok:
            fails += 1
    w = tempfile.mkdtemp(prefix="c_alloc_census.")
    sd = os.path.join(w, "src", "runtime"); os.makedirs(sd)
    ld = os.path.join(w, "src", "lower"); os.makedirs(ld)
    gd = os.path.join(w, "src", "runtime", "rt"); os.makedirs(gd)

    open(os.path.join(ld, "clean.c"), "w").write(
        "void *f(void) { return ct_alloc(8); }\n"
        "/* malloc( in a comment is not a call */\n"
        'void g(void) { log("malloc(3) failed"); }\n'
        "void h(void) { ct_drop(p); }\n")
    buf = []
    rc = census(w, out=buf.append)
    j = "\n".join(buf)
    ck("FORBIDDEN total=0" in j, "a comment mention and a string literal are not call sites")
    ck("ARENA=2" in j, "ct_alloc and ct_drop in src/lower count as ARENA, the right destination for compile-time data")
    ck(rc == 0, "a clean compile-time file reads GREEN")

    open(os.path.join(ld, "dirty.c"), "w").write(
        "void *f(void) { void *p = malloc(8); p = realloc(p, 16); free(p); return calloc(2, 4); }\n"
        "extern void *malloc(size_t);\n")
    buf.clear(); rc = census(w, out=buf.append)
    j = "\n".join(buf)
    ck(rc == 1 and "FORBIDDEN total=4 (malloc=1 calloc=1 realloc=1 free=1)" in j,
       "four planted calls are counted one each, and the extern PROTOTYPE beside them is not a fifth")

    open(os.path.join(ld, "alias.c"), "w").write("#define MYALLOC malloc\nvoid *f(void){return MYALLOC(8);}\n")
    buf.clear(); census(w, out=buf.append)
    j = "\n".join(buf)
    ck("ALIASES=1" in j, "a #define alias of a forbidden name is counted -- the rule binds under any spelling")
    ck("ALIAS-CALL MYALLOC() -> malloc" in j, "and the CALL through that alias is counted and named with what it reaches")
    os.remove(os.path.join(ld, "alias.c"))
    os.remove(os.path.join(ld, "dirty.c"))

    # ⛔ CEO-844 clause 1: bison's chain, which one level of alias reading does not see and a grep never sees.
    open(os.path.join(ld, "chain.c"), "w").write(
        "#define YYMALLOC malloc\n#define YYSTACK_ALLOC YYMALLOC\n"
        "void *f(void) { return YYSTACK_ALLOC(8); }\n")
    buf.clear(); rc = census(w, out=buf.append)
    j = "\n".join(buf)
    ck("YYSTACK_ALLOC -> malloc   via YYSTACK_ALLOC -> YYMALLOC -> malloc" in j,
       "A TWO-LEVEL ALIAS CHAIN IS RESOLVED AND ITS ROUTE PRINTED -- bison writes exactly this, and one level of reading calls it clean")
    ck("ALIAS-CALLS=1" in j and rc == 1,
       "a call through the chain is a call site and REDS the census although the four names read zero in that file")
    os.remove(os.path.join(ld, "chain.c"))

    # ⛔ the false positive a substring reader produces out of flex's own hook rename.
    open(os.path.join(ld, "rename.c"), "w").write(
        "#define yyrealloc pascal_yyrealloc\n#define yyfree pascal_yyfree\nvoid f(void){ ct_drop(p); }\n")
    buf.clear(); census(w, out=buf.append)
    j = "\n".join(buf)
    ck("ALIASES=0" in j,
       "flex's `#define yyrealloc pascal_yyrealloc` RENAMES flex's hook and is NOT an alias of C realloc -- a substring reader calls it one and mislabels it `yy`")
    os.remove(os.path.join(ld, "rename.c"))

    # ⛔ CEO-844 clause 2: the licence sentence is not usage.
    open(os.path.join(ld, "gpl.c"), "w").write(
        "/* This program is free software; you can redistribute it and/or modify\n"
        "   it under the terms of the GNU General Public License.  free free */\n"
        "void f(void) { ct_drop(p); }\n")
    buf.clear(); rc = census(w, out=buf.append)
    j = "\n".join(buf)
    ck("FORBIDDEN total=0" in j and "PROSE-RESIDUE=5 in 2 file(s)" in j and "PROSE src/lower/gpl.c 3" in j,
       "A LICENCE HEADER IS PROSE RESIDUE, NAMED WITH ITS FILE AND KEPT OUT OF THE COUNT -- folding it in reads a permanent false red, and the cheapest way to clear a false red is to delete a copyright header")
    ck(rc == 0, "and a tree whose only remaining hits are licence prose reads GREEN -- the residue is not a defect")
    os.remove(os.path.join(ld, "gpl.c"))

    open(os.path.join(sd, "near_miss.c"), "w").write(
        "void f(void) { ct_free(p); rt_ws_realloc(q, 8); my_free(r); }\n")
    buf.clear(); rc = census(w, out=buf.append)
    j = "\n".join(buf)
    ck("FORBIDDEN total=0" in j, "ct_free is not free and rt_ws_realloc is not realloc -- the word boundary decides, not a substring")
    ck("ROOTED-HEAP=1" in j, "rt_ws_realloc is counted as the ROOTED-HEAP destination")

    open(os.path.join(sd, "evade.c"), "w").write("void *f(void) { return ct_alloc(8); }\n")
    buf.clear(); rc = census(w, out=buf.append)
    j = "\n".join(buf)
    ck(rc == 1 and "ARENA-IN-RUNTIME=1" in j and "src/runtime/evade.c" in j,
       "AN ARENA ALLOCATION UNDER src/runtime IS A VIOLATION AND IS NAMED -- the evasion one name further out, which a four-name grep reads as green")
    os.remove(os.path.join(sd, "evade.c"))

    open(os.path.join(gd, "gc_heap.c"), "w").write("void *f(void){ return mmap(0, n, 3, 34, -1, 0); }\n")
    open(os.path.join(ld, "elsewhere.c"), "w").write("void *g(void){ return mmap(0, n, 3, 34, -1, 0); }\n")
    buf.clear(); census(w, out=buf.append)
    j = "\n".join(buf)
    ck("MMAP-OUTSIDE-GC_HEAP=1" in j and "src/lower/elsewhere.c" in j,
       "mmap in gc_heap.c is the collector's own bookkeeping and passes; mmap elsewhere is NAMED, not silently allowed")

    empty = tempfile.mkdtemp(prefix="c_alloc_empty.")
    buf.clear(); rc = census(empty, out=buf.append)
    ck(rc == 2 and "REFUSED(2)" in "\n".join(buf), "a tree with no source files REFUSES rc=2 -- it does not report a clean zero")

    vac = tempfile.mkdtemp(prefix="c_alloc_vac."); os.makedirs(os.path.join(vac, "src"))
    open(os.path.join(vac, "src", "a.c"), "w").write("int main(void){return 0;}\n")
    buf.clear(); rc = census(vac, out=buf.append)
    ck(rc == 2 and "A vacuous zero is not a pass" in "\n".join(buf),
       "zero forbidden names AND zero destination symbols REFUSES rc=2 -- that is a tool reading the wrong thing, not a cured tree")

    # ⛔⭐ CEO-846: the class split is the arm that decides whether an honest sweep lands or a cfo-f62a33aed
    # shaped evasion does, so it is tested on BOTH sides -- a debt that must NOT red as an evasion, and an
    # evasion that must NOT be excused as debt.
    B = {"src/runtime/a.c": {"forbidden": 4, "rooted_heap": 0, "arena": 0, "mmap": 0},
         "src/runtime/b.c": {"forbidden": 0, "rooted_heap": 3, "arena": 0, "mmap": 0},
         "src/lower/c.c":   {"forbidden": 9, "rooted_heap": 0, "arena": 0, "mmap": 0}}
    N = {"src/runtime/a.c": {"forbidden": 0, "rooted_heap": 0, "arena": 4, "mmap": 0},
         "src/runtime/b.c": {"forbidden": 0, "rooted_heap": 0, "arena": 3, "mmap": 0},
         "src/lower/c.c":   {"forbidden": 0, "rooted_heap": 0, "arena": 9, "mmap": 0}}
    ev, debt, unk = provenance(N, B)
    ck(sum(x[1] for x in debt) == 4 and any(r == "src/runtime/a.c" for r, _n, _d in debt)
       and not any(r == "src/runtime/a.c" for r, *_ in ev),
       "CLASS 2: four libc mallocs that became arena sites are DEBT, not an evasion -- the lifetime is the one they already had, and reading them as an evasion would red an honest sweep")
    ck(sum(x[1] for x in ev) == 3 and any(r == "src/runtime/b.c" for r, *_ in ev),
       "CLASS 1: three COLLECTED-HEAP sites that became arena sites are the EVASION and are NAMED -- the cure for an unrooted holder is a root, never a different allocator")
    ck(not any(r == "src/lower/c.c" for r, *_ in ev) and not any(r == "src/lower/c.c" for r, *_ in debt),
       "and src/lower is not runtime-reachable, so its nine arena sites are the arena doing its job, not a violation")
    ev2, _d2, unk2 = provenance({"src/runtime/z.c": {"forbidden": 0, "rooted_heap": 0, "arena": 2, "mmap": 0}}, B)
    ck(sum(x[1] for x in unk2) == 2,
       "a runtime arena site in a file the provenance baseline does not name is UNATTRIBUTABLE and is counted WITH the evasion -- a history this instrument cannot read gets no benefit of the doubt")

    # ⛔⭐ CEO-846 / CFO-96-97: the container arm, tested on the shape that fooled every census before it.
    def cc(body_src):
        buf2 = []
        lv, lt = container_census({"src/runtime/t.c": body_src}, out=buf2.append)
        return lv, lt
    lv, lt = cc("static DESCR_t *g_blk = (DESCR_t *)0;\n"
                "void t_gc_roots(void) { for (long n = 0; n < top; n++) rt_gc_visit_descr(&g_blk[n]); }\n")
    ck(len(lv) == 1 and lv[0][2] == "g_blk",
       "A POINTER CONTAINER WHOSE CONTENTS ARE VISITED AND WHOSE BLOCK IS NEVER MARKED IS LIVE AND NAMED -- that walk exists, is called, and reads as rooted to every census ever written; it is how g_name_save was reclaimed under a live call")
    lv, lt = cc("static DESCR_t *g_blk = (DESCR_t *)0;\n"
                "void t_gc_roots(void) { rt_gc_visit_raw((const char **)&g_blk); "
                "for (long n = 0; n < top; n++) rt_gc_visit_descr(&g_blk[n]); }\n")
    ck(len(lv) == 0, "and the same walk with the CONTAINER marked as well reads clean -- the arm grades the cure, not the shape")
    lv, lt = cc("static NV_t *g_tab[64];\n"
                "void t_gc_roots(void) { for (int b = 0; b < 64; b++) for (NV_t *e = g_tab[b]; e; e = e->next) "
                "rt_gc_visit_descr(&e->val); }\n")
    ck(len(lv) == 0 and any(x[2] == "g_tab" for x in lt),
       "a FIXED ARRAY of pointers is LATENT, not LIVE -- its storage is static so there is no block to mark, and calling that a defect is how this arm would become noise (it is exactly _var_buckets, which core_gc_roots marks per slot and marks correctly)")

    saved = dict(COUNTS)
    try:
        COUNTS.clear(); COUNTS.update({k: 0 for k in RATCHET_KEYS})
        b = os.path.join(w, "base.tsv")
        open(b, "w").write("".join(f"{k}\t0\n" for k in RATCHET_KEYS))
        buf.clear(); ck(ratchet(b, buf.append) == 0, "ratchet: every count exactly its baseline reads GREEN")
        COUNTS["malloc"] = 1
        buf.clear(); ck(ratchet(b, buf.append) == 1 and "WORSE" in "\n".join(buf),
                        "ratchet: a planted INCREASE is named and RED")
    finally:
        COUNTS.clear(); COUNTS.update(saved)

    print(f"population: {arms} selftest arm(s), {fails} FAIL")
    if arms < ARMS_FLOOR:
        print(f"SELFTEST REFUSED(2): {arms} arm(s) ran, below the recorded floor {ARMS_FLOOR}")
        return 2
    print("SELFTEST PASS" if fails == 0 else "SELFTEST FAIL")
    return 0 if fails == 0 else 1


def main(argv):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--root", default=ROOT)
    ap.add_argument("--ratchet", default="")
    ap.add_argument("--write-baseline", default="")
    ap.add_argument("--write-sites-baseline", default="", help="the PER-FILE provenance baseline the CEO-846 "
                    "class split is keyed on -- what each file held BEFORE the sweep")
    ap.add_argument("--by-dir", action="store_true", help="the four-name count per directory, for the split")
    ap.add_argument("--sites", action="store_true", help="name every remaining forbidden call site")
    ap.add_argument("--selftest", action="store_true")
    a = ap.parse_args(argv)
    if a.selftest:
        return selftest()
    rc = census(a.root, a.by_dir, a.sites)
    if a.write_baseline:
        with open(a.write_baseline, "w", encoding="utf-8") as fh:
            fh.write("# c_allocator_baseline.tsv -- the counts Lon's eradication order drives to 0 (coo, CEO-842/843).\n"
                     "# Written by `util_c_allocator_census.py --write-baseline`; the landing that lowers a count\n"
                     "# rewrites this file in the same sitting.  key\tcount\n")
            for k in RATCHET_KEYS:
                v = COUNTS.get(k)
                if v is not None:
                    fh.write(f"{k}\t{v}\n")
        print(f"baseline written: {a.write_baseline}")
    if a.write_sites_baseline:
        pf = COUNTS.get("__per_file__") or {}
        import subprocess
        try:
            tree = subprocess.run(["git", "-C", a.root, "rev-parse", "--short", "HEAD"],
                                  capture_output=True, text=True, timeout=20).stdout.strip() or "unknown"
            dirty = subprocess.run(["git", "-C", a.root, "status", "--porcelain"],
                                   capture_output=True, text=True, timeout=20).stdout.strip()
        except Exception:
            tree, dirty = "unknown", ""
        with open(a.write_sites_baseline, "w", encoding="utf-8") as fh:
            fh.write("# c_allocator_sites_baseline.tsv -- WHAT EACH FILE HELD BEFORE THE SWEEP (coo, CEO-846).\n"
                     "#\n"
                     "# ⛔ THIS FILE IS THE DISCRIMINATOR AND IT IS THE WHOLE POINT OF THE SPLIT.  The clause `the\n"
                     "# arena may not hold anything the runtime can reach` covers two different acts: a site that\n"
                     "# was on the COLLECTED HEAP and is now in the arena is the EVASION (hard red, reverted on\n"
                     "# sight), and a site that was a libc malloc and is now in the arena is transitional DEBT (the\n"
                     "# same lifetime it already had, ratcheted to zero).  Nothing in the post-sweep tree can tell\n"
                     "# those apart -- only what the site WAS can -- so this file is cut from the PRE-SWEEP tree and\n"
                     "# a landing is graded against it.  Rewriting it from a post-sweep tree destroys the only\n"
                     "# evidence that separates a cure from an evasion, and reads GREEN forever after.\n"
                     f"# CUT FROM: SCRIP {tree}{'-DIRTY' if dirty else ''}  (forbidden_total={COUNTS.get('forbidden_total')})\n"
                     "# file\tforbidden\trooted_heap\tarena\tmmap\n")
            for rel in sorted(pf):
                c = pf[rel]
                fh.write(f"{rel}\t{c['forbidden']}\t{c['rooted_heap']}\t{c['arena']}\t{c['mmap']}\n")
        print(f"sites baseline written: {a.write_sites_baseline} ({len(pf)} file(s), tree {tree})")
    if a.ratchet:
        rc = 2 if 2 in (rc, ratchet(a.ratchet)) else max(rc, ratchet(a.ratchet))
    return rc


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
