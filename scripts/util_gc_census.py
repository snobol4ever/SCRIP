#!/usr/bin/env python3
"""util_gc_census.py -- THE COLLECTOR GUESSES NOTHING: the census instruments (coo, CEO-813/814/815, 2026-09-17;
row gc-instruments-the-safe-point-census-the-maps-census-and-scrip-gc-coverage-words-scanned-zero-are-wired-gates;
design .github/ARCH-GC-COMPILE-TIME-FRAME-MAPS.md, law RULES.md FACT RULE THE COLLECTOR GUESSES NOTHING).

Five censuses, each a mechanical count printed with its population, each RED until the design lands and each proven
to trip on a planted violation by --selftest (33 arms, the ratchet included):

  conservative   src/runtime/rt/gc_heap.c: call sites of the word walker gc_zeta_frame(, uses of cons_stack, of
                 rt_cas_live_span and of hb_scan_interior (Rule 2, Rule 2b).  Want 0 each.
  allocator      rt_gc_collect( inside rt_gcheap_alloc's body (Rule 3: the allocator never collects).  Want 0.
  safe-points    every ALLOCATING runtime entry the emitter calls has a g_gc_pending poll at its emitted return.  The
                 allocating set is read from the runtime binary's own call graph (objdump: every function that reaches
                 rt_gcheap_alloc), never from a hand list; the emitter's call sites are the literal x86("call", "sym")
                 strings in src/templates and src/emitter; a poll is a g_gc_pending reference (or a helper named in
                 --poll-helper) within --poll-window source lines after the call.  Want unpolled 0 and unresolved 0.
  coverage       a run under SCRIP_GC_STRESS=64 SCRIP_GC_COVERAGE=1 prints [GC-COV] lines; every line must carry BOTH
                 words_scanned=0 (stack and spine words visited without a map, Rule 2) and interior_words=0 (mark-
                 worklist payload words visited by the same sniff, Rule 2b).  A line without a token is RED (the
                 design's field is not there yet); no line at all is rc=2 (no collection ran: not measured).  The two
                 spellings are FROZEN -- the cfo's walker row prints them, this census only counts them.
  callbacks      by_name_dispatch.c / gen_runtime.c: every call-back into emitted code (the rt_call_* family) sits
                 inside RT_GC_CALLBACK( (Rule 4; the marker is the cfo's to confirm).  Want unwrapped 0.
  maps           TWO HALVES, printed and graded separately, because a green slot-kind half beside an absent table would
                 read as if the design existed.  SLOT-KIND: the ceo's util_zls_frame_map_census.py over each language's
                 master (token frozen CEO-821), want unkinded=0 and holes=0, with no_layout NAMED beside the number and
                 never folded into it; the wire header past region_end and the spine are NAMED as not censused here
                 (section 2b makes the spine tagged cells, so its census is the tag census, not a hole count).
                 TABLE: section 6.4, from three independent producers per witness -- the frame allocator (--dump-zeta),
                 the emitter (SCRIP_GC_MAPS_REPORT=1, BOTH media) and the assembler text (.Lgcmap_* labels, their
                 prologue leas, the counted __gc_frame_maps table and each map's four decoded quads).  REFUSES rc=2
                 while gc_heap.c defines no rt_gc_frame_maps_install.

rc: 0 every census green; 1 any census red; 2 a census could not measure (missing file, no binary, no collection).
Every count is printed as `CENSUS <name> ... want=...` so a reader re-derives the population, never only the verdict.
"""
import argparse, collections, os, re, subprocess, sys, tempfile

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, ".."))

CONSERVATIVE_TOKENS = [  # (label, regex, what the design says)
    ("gc_zeta_frame_calls", r"(?<![A-Za-z0-9_])gc_zeta_frame\s*\(", "the word walker (Rule 2: DELETED, not gated)"),
    ("cons_stack_uses", r"(?<![A-Za-z0-9_])cons_stack(?![A-Za-z0-9_])", "the conservative stack scan flag (Rule 2)"),
    ("rt_cas_live_span_uses", r"(?<![A-Za-z0-9_])rt_cas_live_span(?![A-Za-z0-9_])", "the byte scan of the cas span (Rule 2)"),
    ("hb_scan_interior_uses", r"(?<![A-Za-z0-9_])hb_scan_interior(?![A-Za-z0-9_])", "the heap-interior sniff (Rule 2b: typed visitors)"),
]


def strip_comments(src):
    src = re.sub(r"/\*.*?\*/", lambda m: "\n" * m.group(0).count("\n"), src, flags=re.S)
    return re.sub(r"//[^\n]*", "", src)


def census_conservative(path, out=print):
    if not os.path.exists(path):
        out(f"CENSUS conservative REFUSED(2): {path} missing"); return 2
    src = strip_comments(open(path, encoding="utf-8", errors="replace").read())
    red = 0
    for label, rx, why in CONSERVATIVE_TOKENS:
        hits = []
        for i, line in enumerate(src.split("\n"), 1):
            for _m in re.finditer(rx, line):
                if label == "gc_zeta_frame_calls" and re.search(r"(static\s+)?void\s+gc_zeta_frame\s*\(", line):
                    continue   # the definition / prototype is not a call site
                hits.append(i)
        n = len(hits)
        out(f"CENSUS conservative {label}={n} want=0 -- {why}" + (f"; lines {hits[:12]}" if n else ""))
        COUNTS.setdefault("conservative", {})[label] = n
        red += n
    out(f"CENSUS conservative total={red} want=0 {'GREEN' if red == 0 else 'RED'}")
    return 0 if red == 0 else 1


def function_body(src, name):
    """the brace-matched body of C function `name` (first definition), or None"""
    m = re.search(r"(?<![A-Za-z0-9_])" + re.escape(name) + r"\s*\([^;{]*\)\s*\{", src)
    if not m:
        return None
    i = m.end() - 1; depth = 0
    for j in range(i, len(src)):
        if src[j] == "{": depth += 1
        elif src[j] == "}":
            depth -= 1
            if depth == 0:
                return src[i:j + 1]
    return None


COUNTS = {}   # census -> {key: count}; the ratchet reads these, never the printed prose


ALLOCATOR_SPELLINGS = ("c_rt_gcheap_alloc", "rt_gcheap_alloc")


def census_allocator(path, out=print):
    if not os.path.exists(path):
        out(f"CENSUS allocator REFUSED(2): {path} missing"); return 2
    src = strip_comments(open(path, encoding="utf-8", errors="replace").read())
    for name in ALLOCATOR_SPELLINGS:   # the definition carries the c_ prefix today; the call sites do not
        body = function_body(src, name)
        if body is not None:
            break
    if body is None:
        out(f"CENSUS allocator REFUSED(2): none of {ALLOCATOR_SPELLINGS} is defined in {os.path.basename(path)} (renamed? tell the census)"); return 2
    hits = [i for i, line in enumerate(body.split("\n"), 1) if re.search(r"(?<![A-Za-z0-9_])rt_gc_collect\s*\(", line)]
    n = len(hits)
    COUNTS.setdefault("allocator", {})["rt_gc_collect_calls_in_the_allocator"] = n
    out(f"CENSUS allocator rt_gc_collect_calls_inside_{name}={n} want=0 -- Rule 3: the allocator sets g_gc_pending and grows the window, never collects"
        + (f"; body lines {hits}" if n else "") + f" {'GREEN' if n == 0 else 'RED'}")
    return 0 if n == 0 else 1


def allocating_entries_from_binary(so, out=print):
    """every function in the runtime .so whose call graph reaches rt_gcheap_alloc (objdump -d, direct calls)"""
    try:
        txt = subprocess.run(["objdump", "-d", "--no-show-raw-insn", so], capture_output=True, text=True, check=True).stdout
    except (OSError, subprocess.CalledProcessError) as e:
        out(f"CENSUS safe-points REFUSED(2): objdump on {so} failed: {e}"); return None
    fn = None; calls = collections.defaultdict(set); funcs = set()
    for line in txt.split("\n"):
        m = re.match(r"^[0-9a-f]+ <([^>]+)>:$", line)
        if m:
            fn = m.group(1); funcs.add(fn); continue
        m = re.search(r"\bcall\s+[0-9a-f]+ <([^>@+]+)", line)
        if m and fn:
            calls[fn].add(m.group(1))
    if "rt_gcheap_alloc" not in funcs:
        out(f"CENSUS safe-points REFUSED(2): {so} defines no rt_gcheap_alloc (renamed? tell the census)"); return None
    rev = collections.defaultdict(set)
    for a, bs in calls.items():
        for b in bs:
            rev[b].add(a)
    seen = {"rt_gcheap_alloc"}; stack = ["rt_gcheap_alloc"]
    while stack:
        x = stack.pop()
        for y in rev.get(x, ()):
            if y not in seen:
                seen.add(y); stack.append(y)
    out(f"CENSUS safe-points allocating_entries={len(seen)} of {len(funcs)} runtime functions reach rt_gcheap_alloc ({so})")
    return seen


CALL_RX = re.compile(r'x86\(\s*"call"\s*,(.*)$')

# A COUNT IS NOT COMPARABLE ACROSS A CHANGE OF ITS CRITERION.  Every line here names a sitting in which the census
# started counting something it could not see before, so a reader never reads the step as a regression or a win
# (SUITES.tsv's criterion_changed column, the same rule).  The baseline writer prints them into the file it writes.
CRITERION_CHANGES = [
    "2026-09-17 coo, SCRIP e625d1b35 -> this tree: safe-points. (i) A CALL TARGET IS SPELLED AS THE ASSEMBLER SEES IT: "
    "the ASCII-only symbol rule read the 32 emitter literals naming a Greek Byrd port (rt_proc_call_epilogue_GAMMA and "
    "kin) as no literal at all, so unresolved fell 39 -> 7 and two ALLOCATING targets entered the denominator "
    "(allocating_call_sites 202 -> 203). (ii) A POLL BELONGS TO THE CALL'S OWN ROUTINE: the line window walked past the "
    "closing brace and read the next routine's poll, so bb_call_proc_staged.cpp:88 read a FALSE POLLED. unpolled "
    "202 -> 203 IS THE INSTRUMENT GETTING HONEST, NOT A LANDING GOING BACKWARDS -- the same tree, one site that was "
    "never counted before, and it is genuinely unpolled.",
    "2026-09-17 coo, same sitting: safe-points. THE CENSUS IS TOLD HOW TO READ A COMPUTED TARGET -- array, macro, "
    "chooser and resolver-out-parameter, each named beside the site it resolves. The last 7 UNRESOLVED sites entered "
    "the denominator: unresolved 7 -> 0, allocating_call_sites 203 -> 210, unpolled 203 -> 210. Three of the seven are "
    "the Prolog direct-dispatch registry (dop_direct_fp, 172 allocating targets among 504 literals) and two are the "
    "SNOBOL4 capture opens behind the cap_open_sym() macro. A first, looser chooser rule resolved that macro against "
    "the next routine's literals and read 'none allocating' -- a candidate set that is not the target's is worse than "
    "UNRESOLVED, so the macro form now reads its own replacement text and an arm plants that trap.",
]

# A CALL TARGET IS SPELLED AS THE ASSEMBLER SEES IT, NOT AS ASCII C.  This tree names its four Byrd ports α β γ ω and
# puts them IN SYMBOL NAMES: rt_proc_call_epilogue_γ is a C function, an emitter literal and a binary symbol.  An
# ASCII-only identifier pattern reads such a literal as NO literal at all, so the site reads UNRESOLVED (or, with an
# ASCII literal beside it on the line, resolves to the wrong candidate set and leaves the graded denominator in
# silence).  Measured 2026-09-17 on SCRIP e625d1b35 by the coo: 32 of the 39 UNRESOLVED sites were plain string
# literals the census could not spell, and two of them -- rt_proc_call_epilogue_named_γ and _ω, at
# src/templates/bb/bb_call_proc_staged.cpp:88 -- REACH rt_gcheap_alloc, so an allocating call site sat outside the
# denominator that grades the poll landings.  The rule below is the binary's: a letter or underscore in ANY alphabet,
# then any run of characters that is not whitespace, a quote, a comma or a parenthesis.
SYM_RX = re.compile(r'"([^\W\d][^\s",()]*)"', re.UNICODE)


# TELLING THE CENSUS ABOUT A COMPUTED TARGET.  Seven emitter call sites choose their target at run time, and a site
# the census cannot read is a site outside its denominator -- so the census would never read GREEN however much poll
# work landed.  Three shapes are DECIDABLE from the source, and only these three; each is named in the output beside
# the site, so a reader sees which rule produced the candidate set and can re-derive it:
#   array    detN_nm[det_nA]  -- a static table of literal symbol names declared in the same file
#   chooser  cap_open_sym()   -- a nullary function whose body returns literal symbol names
#   resolver dsym             -- a variable filled in through the out-parameter of a function named in SYM_RESOLVERS
#                               (dop_direct_fp: the Prolog direct-dispatch registry, ~230 rows of { "$name", ar, "sym" })
# The candidate set is the STRING LITERALS of that table, body or registry -- an OVER-approximation on purpose: a
# literal that names nothing ("$unify") matches no runtime symbol and is harmless, while a missed allocating target
# would be a hole in the denominator.  Anything else stays UNRESOLVED and is printed, never skipped.
SYM_RESOLVERS = ("dop_direct_fp",)

LIT_RX = re.compile(r'"([^"\\\n]*)"')


def _brace_body_literals(text, at):
    """every string literal of the brace-balanced body whose opening brace is at/after `at` (empty if none closes)"""
    o = text.find("{", at)
    if o < 0:
        return []
    depth = 0
    for k in range(o, len(text)):
        if text[k] == "{":
            depth += 1
        elif text[k] == "}":
            depth -= 1
            if depth == 0:
                return LIT_RX.findall(text[o:k])
    return []


def _first_arg(expr):
    """the call's first argument: up to the first comma at paren/bracket depth 0"""
    depth = 0
    for k, c in enumerate(expr):
        if c in "([":
            depth += 1
        elif c in ")]":
            if depth == 0:
                return expr[:k]
            depth -= 1
        elif c == "," and depth == 0:
            return expr[:k]
    return expr


def resolve_computed(expr, own_text, all_text):
    """(candidates, rule) for a computed call target the census has been told how to read; ([], "") otherwise"""
    e = _first_arg(expr).strip()
    m = re.match(r"([A-Za-z_]\w*)\s*\[", e)
    if m and re.search(r"\b" + m.group(1) + r"\s*\[[^\]]*\]\s*=\s*\{", own_text):
        d = re.search(r"\b" + m.group(1) + r"\s*\[[^\]]*\]\s*=\s*\{", own_text)
        return _brace_body_literals(own_text, d.end() - 1), f"array {m.group(1)}[]"
    m = re.match(r"([A-Za-z_]\w*)\s*\(\s*\)\s*$", e)
    if m:
        # A CHOOSER IS PROVEN AT ITS DEFINITION OR NOT AT ALL.  cap_open_sym() is a #define, not a function, and a
        # pattern loose enough to match the macro line and then run on to the next `{` in the file resolves the site
        # against SOME OTHER routine's literals -- a candidate set that is not the target's is worse than UNRESOLVED.
        # So: the macro form reads its own (backslash-continued) replacement text, and the function form requires the
        # opening brace on the SAME line as the parameter list.
        d = re.search(r"^[ \t]*#[ \t]*define[ \t]+" + m.group(1) + r"\(\)(.*(?:\\\n.*)*)$", own_text, re.M)
        if d:
            return LIT_RX.findall(d.group(1)), f"macro {m.group(1)}()"
        d = re.search(r"\b" + m.group(1) + r"\s*\([^;{\n]*\)[ \t]*\{", own_text)
        if d:
            return _brace_body_literals(own_text, d.end() - 1), f"chooser {m.group(1)}()"
    m = re.match(r"([A-Za-z_]\w*)\s*$", e)
    if m:
        for rv in SYM_RESOLVERS:
            if re.search(re.escape(rv) + r"\s*\([^;]*&\s*" + m.group(1) + r"\b", own_text):
                d = re.search(r"\b" + re.escape(rv) + r"\s*\([^;{]*\)\s*\{", all_text)
                if d:
                    return _brace_body_literals(all_text, d.end() - 1), f"resolver {rv}()"
    return [], ""


def emitter_call_sites(files):
    """(file, line, [candidate symbols], lines, rule) for every x86("call", ...) in the emitter; a call expression
    whose target is computed is read through resolve_computed and NAMES its rule, and one the census has not been
    told how to read is UNRESOLVED and printed, never skipped."""
    texts = {}
    for f in files:
        try:
            texts[f] = open(f, encoding="utf-8", errors="replace").read()
        except OSError:
            continue
    joined = "\n".join(texts.values())
    sites = []
    for f, text in texts.items():
        lines = text.split("\n")
        for i, line in enumerate(lines, 1):
            m = CALL_RX.search(line)
            if not m:
                continue
            syms = SYM_RX.findall(m.group(1))
            rule = ""
            if not syms:
                syms, rule = resolve_computed(m.group(1), text, joined)
            sites.append((f, i, syms, lines, rule))
    return sites


def census_safe_points(so, emitter_files, poll_window=12, poll_helper="", out=print, allocating=None):
    if allocating is None:
        allocating = allocating_entries_from_binary(so, out)
        if allocating is None:
            return 2
    sites = emitter_call_sites(emitter_files)
    if not sites:
        out("CENSUS safe-points REFUSED(2): no x86(\"call\", ...) sites found in the emitter files -- wrong tree?"); return 2
    poll_rx = re.compile(r"g_gc_pending" + (("|" + re.escape(poll_helper)) if poll_helper else ""))
    total = alloc_sites = polled = 0; unpolled = []; unresolved = []
    resolved = []
    for f, i, syms, lines, rule in sites:
        total += 1
        if not syms:
            unresolved.append(f"{os.path.relpath(f, ROOT)}:{i}")
            continue
        if rule:
            al = sorted(set(x for x in syms if x in allocating))
            resolved.append(f"{os.path.relpath(f, ROOT)}:{i} via {rule}: {len(syms)} candidate(s), "
                            + (f"{len(al)} allocating: {', '.join(al[:6])}" + (f" ... +{len(al) - 6} more" if len(al) > 6 else "")
                               if al else "none allocating"))
        if not any(s in allocating for s in syms):
            continue
        alloc_sites += 1
        # A POLL BELONGS TO THE CALL'S OWN FUNCTION.  The window is source lines after the call, so without a stop it
        # walks out of the emitter routine and reads the NEXT routine's poll as this site's.  Measured 2026-09-17 on
        # SCRIP e625d1b35 by the coo: bb_call_proc_staged.cpp:88 (rt_proc_call_epilogue_named_γ/ω, allocating) read
        # POLLED off the g_gc_pending lea in stage_arg_inline, seven lines below and two functions away -- a false
        # green, the worst direction for this census.  The window stops at the first line that starts a new routine in
        # this tree's style: a column-0 closing brace or a column-0 /*---- separator.
        window_lines = lines[i:i + poll_window]
        stop = len(window_lines)
        for k, wl in enumerate(window_lines):
            if wl.startswith("}") or wl.startswith("/*---"):
                stop = k
                break
        window = "\n".join(window_lines[:stop])
        if poll_rx.search(window):
            polled += 1
        else:
            unpolled.append(f"{os.path.relpath(f, ROOT)}:{i}:{'/'.join(s for s in syms if s in allocating)}"
                            + (f" [{rule}, {len(syms)} candidate(s)]" if rule else ""))
    out(f"CENSUS safe-points emitter_call_sites={total} allocating_call_sites={alloc_sites} polled={polled} unpolled={len(unpolled)} unresolved={len(unresolved)} want unpolled=0 unresolved=0 (poll = g_gc_pending{' or ' + poll_helper if poll_helper else ''} within {poll_window} lines after the call)")
    for u in unpolled[:25]:
        out(f"  UNPOLLED {u}")
    if len(unpolled) > 25:
        out(f"  ... {len(unpolled) - 25} more unpolled")
    for r in resolved:
        out(f"  RESOLVED {r}")
    for u in unresolved[:10]:
        out(f"  UNRESOLVED {u} (the call target is computed; name it with a literal or tell the census)")
    COUNTS.setdefault("safe-points", {}).update({"unpolled": len(unpolled), "unresolved": len(unresolved)})
    ok = not unpolled and not unresolved
    out("CENSUS safe-points NOTE (the cfo, 2026-09-17): A SITE COUNT IS NOT A COVERAGE MEASURE. An Icon allocation loop "
        "reaches NO safe point at all -- the shielded rt_gc_point_arr sites are SNOBOL4 and Prolog by-name dispatch points "
        "and Icon's every-do loop contains none -- so with the allocator silent it collects ONCE (861920 KB RSS) where "
        "origin collected five times (157144 KB). The RATE, collections-per-program on an allocation loop, lives in "
        "test_gate_gc_every_allocating_runtime_entry_returns_to_a_safe_point.sh; read it beside this number, never instead.")
    out(f"CENSUS safe-points {'GREEN' if ok else 'RED'}")
    return 0 if ok else 1


COV_STRESS = "64"   # the census needs collections to HAPPEN, not to be paced hard; 64 keeps the default witness under 2 s

COVERAGE_TOKENS = [  # (token, what it counts; Rule 2 and Rule 2b)
    ("words_scanned", "stack and spine words visited without a map (Rule 2: the collector walks maps, not words)"),
    ("interior_words", "mark-worklist payload words visited by the same sniff (Rule 2b: every heap block kind gets a typed visitor)"),
]


def census_coverage(scrip, program, mode="m3", out=print, env_extra=None, cov_text=None):
    """run `program` under stress + coverage and read the [GC-COV] lines (or grade `cov_text` directly: the selftest)"""
    if cov_text is None:
        if not (scrip and os.path.exists(scrip)):
            out(f"CENSUS coverage REFUSED(2): no scrip binary at {scrip}"); return 2
        env = dict(os.environ); env.update({"SCRIP_GC_STRESS": COV_STRESS, "SCRIP_GC_COVERAGE": "1"}); env.update(env_extra or {})
        try:
            r = subprocess.run([scrip, program], capture_output=True, text=True, env=env, timeout=120, stdin=subprocess.DEVNULL)
        except (OSError, subprocess.TimeoutExpired) as e:
            out(f"CENSUS coverage REFUSED(2): the witness run failed: {e}"); return 2
        cov_text = r.stderr
    lines = [l for l in cov_text.split("\n") if "[GC-COV]" in l]
    if not lines:
        out("CENSUS coverage REFUSED(2): no [GC-COV] line printed -- no collection ran under SCRIP_GC_STRESS=64, nothing measured"); return 2
    red = 0
    for token, why in COVERAGE_TOKENS:
        rx = r"(?<![A-Za-z_])" + token + r"=(\d+)"
        missing = [l for l in lines if not re.search(rx, l)]
        nonzero = [l for l in lines if re.search(r"(?<![A-Za-z_])" + token + r"=([1-9]\d*)", l)]
        out(f"CENSUS coverage gc_cov_lines={len(lines)} without_{token}={len(missing)} {token}_nonzero={len(nonzero)} want 0 and 0 -- {why}")
        for l in (missing[:2] + nonzero[:2]):
            out("  " + l.strip()[:160])
        COUNTS.setdefault("coverage", {})["lines_without_or_nonzero_" + token] = len(missing) + len(nonzero)
        red += len(missing) + len(nonzero)
    COUNTS["coverage"]["red"] = 1 if red else 0
    out(f"CENSUS coverage {'GREEN' if red == 0 else 'RED'} (both tokens over {len(lines)} line(s))")
    return 0 if red == 0 else 1


CALLBACK_RX = re.compile(r"(?<![A-Za-z0-9_])(rt_call_(?:proc_descr|arr[A-Za-z0-9_]*|value[A-Za-z0-9_]*))\s*\(")


def _marker_spans(src, marker):
    """(start, end) offsets of every MARKER( ... ) span, paren-matched -- a call is wrapped only when it lies inside one"""
    spans = []
    for m in re.finditer(r"(?<![A-Za-z0-9_])" + re.escape(marker) + r"\s*\(", src):
        depth = 0
        for j in range(m.end() - 1, len(src)):
            if src[j] == "(":
                depth += 1
            elif src[j] == ")":
                depth -= 1
                if depth == 0:
                    spans.append((m.start(), j))
                    break
    return spans


def census_callbacks(files, marker="RT_GC_CALLBACK", out=print):
    present = [f for f in files if os.path.exists(f)]
    if not present:
        out("CENSUS callbacks REFUSED(2): none of the callback files exist"); return 2
    sites = wrapped = 0; unwrapped = []
    for f in present:
        src = strip_comments(open(f, encoding="utf-8", errors="replace").read())
        spans = _marker_spans(src, marker)
        for m in CALLBACK_RX.finditer(src):
            line_start = src.rfind("\n", 0, m.start()) + 1
            line_end = src.find("\n", m.start())
            line = src[line_start:line_end if line_end >= 0 else len(src)]
            head = src[line_start:m.start()]
            if re.match(r"\s*(static\s+)?[A-Za-z_][A-Za-z0-9_ \*]*\s" + re.escape(m.group(1)) + r"\s*\(", line) and line.rstrip().endswith(("{", ")", ",")) and "=" not in head:
                continue   # a definition or prototype, not a call
            sites += 1
            lineno = src.count("\n", 0, m.start()) + 1
            if any(a <= m.start() <= b for a, b in spans):
                wrapped += 1
            else:
                unwrapped.append(f"{os.path.relpath(f, ROOT)}:{lineno}:{m.group(1)}")
    out(f"CENSUS callbacks call_back_sites={sites} wrapped_in_{marker}={wrapped} unwrapped={len(unwrapped)} want unwrapped=0 (Rule 4: a runtime frame holds no raw heap pointer across a callback into emitted code)")
    for u in unwrapped[:15]:
        out(f"  UNWRAPPED {u}")
    if len(unwrapped) > 15:
        out(f"  ... {len(unwrapped) - 15} more unwrapped")
    COUNTS.setdefault("callbacks", {})["unwrapped"] = len(unwrapped)
    out(f"CENSUS callbacks {'GREEN' if not unwrapped else 'RED'}")
    return 0 if not unwrapped else 1


ZLS_TOOL = "util_zls_frame_map_census.py"          # the ceo's slot-kind base tool (CEO-820)
ZLS_RX = re.compile(r"^ZLS-MAP lang=(\S+) graphs=(\d+) words=(\d+) unkinded=(\d+) holes=(\d+) graded=(\d+) no_layout=(\d+)")
ZLS_LANGS_ALL = ["rebus", "snocone", "pascal", "icon", "raku", "snobol4", "prolog"]   # ~32 s for the seven
ZLS_LANGS_FAST = ["rebus", "snocone", "pascal"]                                       # ~2.7 s; the wired gate's default
GCMAP_RX = re.compile(r"\[GC-MAP\] graph=(\S+) frame_bytes=(\d+) header_bytes=(\d+) map_off=(\d+) flags=(\d+)")
MAP_MAGIC = 0x50414D5A   # 'ZMAP', section 6.3
ENTRY_ALIAS = ("pat_flat", "main")   # the mode-3 emitter names the entry graph pat_flat, mode 4 and --dump-zeta name it main
MAPS_SYMBOLS = ("rt_gc_frame_maps_install", "rt_gc_frame_maps")   # section 6.4: the one exported symbol and its reader


def census_maps_slotkind(root, langs, out=print):
    """HALF A -- every word of every graded graph's zls region has a declared kind (the ceo's tool, token frozen CEO-821).
    no_layout entries are NAMED beside the number and never folded into it (CEO-749's rule)."""
    tool = os.path.join(root, "scripts", ZLS_TOOL)
    if not os.path.exists(tool):
        out(f"CENSUS maps/slot-kind REFUSED(2): no {ZLS_TOOL} at {tool}"); return 2
    red = 0; swept = []; nolayout = 0; graded = 0
    for lang in langs:
        try:
            r = subprocess.run([sys.executable, tool, "--lang", lang], capture_output=True, text=True, timeout=600, stdin=subprocess.DEVNULL)
        except (OSError, subprocess.TimeoutExpired) as e:
            out(f"CENSUS maps/slot-kind REFUSED(2): {ZLS_TOOL} --lang {lang} failed: {e}"); return 2
        m = None
        for line in r.stdout.split("\n"):
            mm = ZLS_RX.match(line.strip())
            if mm:
                m = mm
        if not m:
            out(f"CENSUS maps/slot-kind REFUSED(2): {ZLS_TOOL} --lang {lang} printed no ZLS-MAP line (rc={r.returncode}) -- the token is frozen (CEO-821); a census that parses prose is the defect one level up"); return 2
        _l, graphs, words, unkinded, holes, gr, nl = m.group(1), *(int(x) for x in m.groups()[1:])
        swept.append(lang); graded += gr; nolayout += nl
        bad = unkinded + holes
        red += bad
        out(f"CENSUS maps/slot-kind lang={lang} graphs={graphs} words={words} unkinded={unkinded} holes={holes} want 0 and 0"
            f" -- graded={gr}, no_layout={nl} NAMED AND UNCOUNTED (the compiler refused those entries; they are not a pass)"
            + ("" if not bad else f"  RED"))
        COUNTS.setdefault("maps", {})[f"slotkind_bad_{lang}"] = bad
    out(f"CENSUS maps/slot-kind swept {len(swept)} of {len(ZLS_LANGS_ALL)} language(s) ({', '.join(swept)}); graded={graded} no_layout={nolayout} UNCOUNTED; "
        f"{'GREEN' if red == 0 else 'RED'}"
        + ("" if len(swept) == len(ZLS_LANGS_ALL) else "  -- A PARTIAL SWEEP IS NOT ALL-LANGUAGE COVERAGE: --zls-langs all for the other " + str(len(ZLS_LANGS_ALL) - len(swept))))
    out("CENSUS maps/slot-kind NOT CENSUSED HERE, named: the wire header past region_end (the cto's map cell, section 6.7) and the "
        "spine (section 2b makes it tagged cells, not a map -- its census is the tag census, not a hole count)")
    return 0 if red == 0 else 1


def decode_map_quads(text, label):
    """the four .quad lines under .Lgcmap_<g> (section 6.7c): magic|frame_bytes<<32, header_bytes|flags<<32, name, 0"""
    m = re.search(r"^" + re.escape(label) + r":\s*\n((?:\s*\.quad\s+\S+\s*\n){4})", text, re.M)
    if not m:
        m = re.search(r"^" + re.escape(label) + r":[^\n]*\n((?:[^\n]*\.quad[^\n]*\n){4})", text, re.M)
    if not m:
        return None
    qs = re.findall(r"\.quad\s+(\S+)", m.group(1))
    try:
        q0 = int(qs[0]); q1 = int(qs[1])
    except (ValueError, IndexError):
        return None
    return {"magic": q0 & 0xFFFFFFFF, "frame_bytes": q0 >> 32, "header_bytes": q1 & 0xFFFFFFFF, "flags": q1 >> 32}


def read_gcmaps(text):
    d = {}
    for m in GCMAP_RX.finditer(text):
        d[m.group(1)] = {"frame_bytes": int(m.group(2)), "header_bytes": int(m.group(3)), "map_off": int(m.group(4)), "flags": int(m.group(5))}
    return d


# THE BLOB-FRAME CLASS MUST BE IN THE WITNESS SET OR THE CENSUS REFUSES (cto 2026-09-17, after their PAT$0 cure was
# built, measured against the SNOBOL4 master, cost one entry and was withdrawn under CEO-589).  A stored-pattern
# activation frame presents NO map cell -- ARCH-GC 6.2b states the walk rule for it -- so a table census run over
# witnesses that contain no pattern reads 0 divergences BY NEVER LOOKING, the same shape as words_scanned=0 on a run
# that never collected.  A zero has to be a zero somebody could have failed.
PATTERN_GRAPH_RX = re.compile(r"^PAT\$")


def blob_class_visible(graph_names):
    """True when the witness set laid out at least one stored-pattern graph, so a 0 here is a measured 0"""
    return any(PATTERN_GRAPH_RX.match(g) for g in graph_names)


def census_maps_table(root, scrip, witnesses, out=print):
    """HALF B -- section 6.4's table, from THREE independent producers per witness: the frame allocator (--dump-zeta),
    the emitter (SCRIP_GC_MAPS_REPORT=1, both media) and the assembler text (.Lgcmap_* labels, their leas, the counted
    __gc_frame_maps table).  The cto's arms (a) one cell per graph naming a map in the table, (b) every map named by
    exactly one prologue, (d) the two media agree field for field."""
    gc = os.path.join(root, "src", "runtime", "rt", "gc_heap.c")
    if not os.path.exists(gc):
        out(f"CENSUS maps/table REFUSED(2): {gc} missing"); return 2
    src = strip_comments(open(gc, encoding="utf-8", errors="replace").read())
    missing = [sym for sym in MAPS_SYMBOLS if not re.search(r"(?<![A-Za-z0-9_])" + sym + r"\s*\(", src)]
    if missing:
        out(f"CENSUS maps/table REFUSED(2): section 6.4's symbol(s) {missing} are not defined in gc_heap.c -- the table does not exist yet; nothing to count"); return 2
    if not (scrip and os.path.exists(scrip)):
        out(f"CENSUS maps/table REFUSED(2): no scrip binary at {scrip}"); return 2
    env = dict(os.environ); env["SCRIP_GC_MAPS_REPORT"] = "1"
    red = 0; total_graphs = 0; all_laid = set()
    for w in witnesses:
        tag = os.path.basename(w)
        if not os.path.exists(w):
            out(f"CENSUS maps/table REFUSED(2): witness {w} missing"); return 2
        try:
            dz = subprocess.run([scrip, "--dump-zeta", w], capture_output=True, text=True, env=env, timeout=120, stdin=subprocess.DEVNULL)
            r3 = subprocess.run([scrip, w], capture_output=True, text=True, env=env, timeout=120, stdin=subprocess.DEVNULL)
            r4 = subprocess.run([scrip, "--compile", w], capture_output=True, text=True, env=env, timeout=120, stdin=subprocess.DEVNULL)
        except (OSError, subprocess.TimeoutExpired) as e:
            out(f"CENSUS maps/table REFUSED(2): {tag}: a reading run failed: {e}"); return 2
        laid = set(re.findall(r"^;\s*graph \d+ '([^']+)'", dz.stdout, re.M))
        m3 = read_gcmaps(r3.stdout + r3.stderr)
        m4 = read_gcmaps(r4.stderr)
        asm = r4.stdout
        labels = set(re.findall(r"^(\.Lgcmap_[A-Za-z0-9_]+):", asm, re.M)) - set(re.findall(r"^(\.Lgcmap_[A-Za-z0-9_]+_s):", asm, re.M))
        leas = collections.Counter(re.findall(r"lea\s+\S+,\s*\[rip \+ (\.Lgcmap_[A-Za-z0-9_]+)\]", asm))
        tbl = re.search(r"^__gc_frame_maps:\s*\.quad\s+(\d+)\s*\n((?:\s*\.quad\s+\.Lgcmap_[A-Za-z0-9_]+\s*\n)+)", asm, re.M)
        if not (laid and m3 and m4):
            out(f"CENSUS maps/table REFUSED(2): {tag}: a producer printed nothing (dump-zeta graphs={len(laid)} m3 maps={len(m3)} m4 maps={len(m4)}) -- not measured"); return 2
        n_declared = int(tbl.group(1)) if tbl else -1
        entries = re.findall(r"\.quad\s+(\.Lgcmap_[A-Za-z0-9_]+)", tbl.group(2)) if tbl else []
        # the one NAMED alias: mode 3's emitter calls the entry graph pat_flat where mode 4 and --dump-zeta call it main
        alias = ENTRY_ALIAS[0] in m3 and ENTRY_ALIAS[0] not in laid and ENTRY_ALIAS[1] in laid
        m3n = dict(m3)
        if alias:
            m3n[ENTRY_ALIAS[1]] = m3n.pop(ENTRY_ALIAS[0])
        bad = []
        for g in sorted(laid):
            if g not in m3n: bad.append(f"graph '{g}' has a frame but NO map cell in mode 3")
            if g not in m4:  bad.append(f"graph '{g}' has a frame but NO map cell in mode 4")
        for g in sorted(set(m3n) | set(m4)):
            if g not in laid: bad.append(f"map cell for '{g}' names a graph the frame allocator never laid out")
            if g in m3n and g in m4 and m3n[g] != m4[g]:
                bad.append(f"graph '{g}' reads {m3n[g]} in mode 3 and {m4[g]} in mode 4 -- the media disagree")
        for g in sorted(m4):
            lab = ".Lgcmap_" + g
            if lab not in labels: bad.append(f"'{g}' reports a map cell but the .s has no {lab}")
            elif leas[lab] != 1:  bad.append(f"{lab} is referenced by {leas[lab]} prologue lea(s), want exactly 1")
            elif lab not in entries: bad.append(f"{lab} is not in the __gc_frame_maps table")
            else:
                q = decode_map_quads(asm, lab)
                if q is None: bad.append(f"{lab}'s four quads do not decode (section 6.3)")
                elif q["magic"] != MAP_MAGIC: bad.append(f"{lab} magic 0x{q['magic']:08X}, want 0x{MAP_MAGIC:08X} 'ZMAP'")
                elif q["frame_bytes"] != m4[g]["frame_bytes"] or q["header_bytes"] != m4[g]["header_bytes"] or q["flags"] != m4[g]["flags"]:
                    bad.append(f"{lab}'s static map {q} contradicts its own reported cell {m4[g]} -- the double-entry fails")
        for lab in sorted(labels - {".Lgcmap_" + g for g in m4}):
            bad.append(f"{lab} is emitted but no graph reports a cell for it")
        if n_declared != len(entries): bad.append(f"__gc_frame_maps declares n={n_declared} over {len(entries)} entries")
        total_graphs += len(laid); all_laid |= laid
        out(f"CENSUS maps/table {tag}: graphs_laid_out={len(laid)} map_cells_m3={len(m3)} map_cells_m4={len(m4)} "
            f"asm_maps={len(labels)} table_n={n_declared} entries={len(entries)} divergences={len(bad)} want 0"
            + ("  [NAMED ALIAS: mode 3 calls the entry graph '%s' where mode 4 and --dump-zeta call it '%s'; same fields, compared as one]" % ENTRY_ALIAS if alias else ""))
        for b in bad[:12]:
            out(f"  DIVERGENCE {b}")
        if len(bad) > 12:
            out(f"  ... {len(bad) - 12} more")
        red += len(bad)
    if not blob_class_visible(all_laid):
        out(f"CENSUS maps/table REFUSED(2): {len(witnesses)} witness(es), {total_graphs} graph(s), and NOT ONE stored-pattern "
            "(PAT$...) graph among them -- the blob-frame class (a frame that presents no map cell; ARCH-GC 6.2b holds the "
            "walk rule, the cto's arm 6 holds the count at 4) CANNOT BE SEEN by this witness set, so the divergence count "
            "would be a zero by never looking. Add a pattern-bearing witness.")
        return 2
    blobs = sorted(g for g in all_laid if PATTERN_GRAPH_RX.match(g))
    COUNTS.setdefault("maps", {})["table_divergences"] = red
    # The population goes on its OWN line and the verdict line keeps its shape: the wired gate anchors on the verdict
    # word ending that line, and an instrument that appends to a line another instrument reads by identity breaks it.
    out(f"CENSUS maps/table blob-frame class VISIBLE in this witness set: {len(blobs)} stored-pattern graph(s) -- "
        + ", ".join(blobs[:4]) + (f" ... +{len(blobs) - 4} more" if len(blobs) > 4 else "")
        + " (ARCH-GC 6.2b; a frame that presents no map cell, so a 0 here is a measured 0)")
    out(f"CENSUS maps/table {len(witnesses)} witness(es), {total_graphs} graph(s) laid out, {red} divergence(s) {'GREEN' if red == 0 else 'RED'}")
    return 0 if red == 0 else 1


def census_maps(root, scrip, langs, witnesses, out=print):
    """The two halves are printed and graded SEPARATELY on purpose: a green slot-kind half beside an absent table would
    read as if the design existed (section 6's question is the table's, F1's is the cell's)."""
    a = census_maps_slotkind(root, langs, out)
    b = census_maps_table(root, scrip, witnesses, out)
    rc = worst([a, b])
    out(f"CENSUS maps slot-kind={'GREEN' if a == 0 else ('RED' if a == 1 else 'REFUSED(2)')} "
        f"table={'GREEN' if b == 0 else ('RED' if b == 1 else 'REFUSED(2)')} -- {'GREEN' if rc == 0 else ('RED' if rc == 1 else 'REFUSED(2)')}")
    return rc


RATCHET_KEYS = [   # (census, key, is_population) -- a population may only FALL; a verdict may only improve
    ("conservative", "gc_zeta_frame_calls", True),
    ("conservative", "cons_stack_uses", True),
    ("conservative", "rt_cas_live_span_uses", True),
    ("conservative", "hb_scan_interior_uses", True),
    ("allocator", "rt_gc_collect_calls_in_the_allocator", True),
    ("safe-points", "unpolled", True),
    ("safe-points", "unresolved", True),
    ("callbacks", "unwrapped", True),
    ("coverage", "red", False),
    ("maps", "table_divergences", True),   # the slot-kind keys are per-language and move with --zls-langs, so they are not ratcheted
]


def read_baseline(path):
    base = {}
    if not os.path.exists(path):
        return None
    for line in open(path, encoding="utf-8"):
        line = line.rstrip("\n")
        if not line or line.startswith("#"):
            continue
        f = line.split("\t")
        if len(f) < 3:
            continue
        base[(f[0], f[1])] = int(f[2])
    return base


def ratchet(path, out=print):
    """re-read every count and refuse an INCREASE; a count that FELL is named so the landing lowers the baseline in the
    same sitting -- a ratchet nobody tightens is a ratchet that lets the next regression back in."""
    base = read_baseline(path)
    if base is None:
        out(f"RATCHET REFUSED(2): no baseline at {path}"); return 2
    # The criterion lines the baseline carries are printed WHERE THE NUMBERS ARE COMPARED, not only in the file: a
    # reader who sees 202 -> 203 without them reads a regression that did not happen.
    try:
        for line in open(path, encoding="utf-8"):
            if line.startswith("# CRITERION CHANGED"):
                out("RATCHET " + line.rstrip()[2:])
    except OSError:
        pass
    worse = []; better = []; unmeasured = []
    for census, key, _pop in RATCHET_KEYS:
        want = base.get((census, key))
        got = COUNTS.get(census, {}).get(key)
        if want is None:
            unmeasured.append(f"{census}.{key} (not in the baseline)"); continue
        if got is None:
            unmeasured.append(f"{census}.{key} (the census did not measure it this run)"); continue
        mark = "="
        if got > want:
            worse.append(f"{census}.{key} {want} -> {got}"); mark = "WORSE"
        elif got < want:
            better.append(f"{census}.{key} {want} -> {got}"); mark = "BETTER"
        out(f"RATCHET {census}.{key} baseline={want} now={got} {mark}")
    out(f"population: {len(RATCHET_KEYS)} ratcheted count(s), {len(worse)} worse, {len(better)} better, {len(unmeasured)} not measured")
    for u in unmeasured:
        out(f"  NOT-MEASURED {u}")
    for w in worse:
        out(f"  ⛔ WORSE {w} -- the design moves these to 0; a landing may not add one")
    for b in better:
        out(f"  ⭐ BETTER {b} -- lower the baseline in the landing that earned it ({os.path.relpath(path, ROOT)})")
    if unmeasured:
        out("RATCHET REFUSED(2): a count the baseline names was not measured -- not green"); return 2
    if worse:
        out("RATCHET RED(1): a conservative/unpolled/unwrapped count went UP"); return 1
    if better:
        out("RATCHET RED(1): a count FELL and the baseline still names the old number -- record the win, do not leave the ratchet loose"); return 1
    out("RATCHET GREEN: every ratcheted count is exactly its baseline"); return 0


def worst(rcs):
    return 2 if 2 in rcs else (1 if 1 in rcs else 0)


# THE FLOOR, NOT THE COUNT.  The printed population is COUNTED by ck() as the arms run -- a hand-typed total is a
# number nobody re-derives, and this one read 29 while 27 arms ran.  This floor catches the other direction: an
# arm deleted or skipped makes the selftest REFUSE rc=2 instead of passing with less proof.  A landing that adds
# arms raises it in the same sitting.
ARMS_FLOOR = 33


def selftest():
    """planted violations trip, clean fixtures pass -- the proof each census is an instrument and not a grep that agrees"""
    fails = 0
    arms = 0
    def ck(ok, label):
        nonlocal fails, arms
        arms += 1
        print(("  ok    " if ok else "  FAIL  ") + label)
        if not ok:
            fails += 1
    w = tempfile.mkdtemp(prefix="gc_census_selftest.")
    buf = []
    # conservative + allocator, on a fixture collector
    clean = "static void gc_zeta_frame(const char *lo, const char *hi);\nvoid *rt_gcheap_alloc(size_t n) {\n  if (over) { g_gc_pending = 1; grow(); }\n  return carve(n);\n}\n/* gc_zeta_frame( in a comment is not a call */\n"
    dirty = "static void gc_zeta_frame(const char *lo, const char *hi);\nvoid *rt_gcheap_alloc(size_t n) {\n  if (over) rt_gc_collect();\n  return carve(n);\n}\nstatic long gc_collect_ex(int cons_stack) { if (cons_stack) gc_zeta_frame(lo, hi); if (hb_scan_interior(t)) gc_zeta_frame(a, b); rt_cas_live_span(0, &b, &n); }\n"
    fc, fd = os.path.join(w, "clean.c"), os.path.join(w, "dirty.c")
    open(fc, "w").write(clean); open(fd, "w").write(dirty)
    buf.clear(); rc = census_conservative(fc, buf.append)
    ck(rc == 0 and "total=0" in buf[-1], "conservative: a clean collector (prototype only, a comment mention) reads total=0 GREEN")
    buf.clear(); rc = census_conservative(fd, buf.append)
    ck(rc == 1 and "gc_zeta_frame_calls=2" in "\n".join(buf) and "cons_stack_uses=2" in "\n".join(buf) and "rt_cas_live_span_uses=1" in "\n".join(buf) and "hb_scan_interior_uses=1" in "\n".join(buf),
       "conservative: planted word-walker calls, cons_stack, rt_cas_live_span and hb_scan_interior are each counted and RED")
    buf.clear(); rc = census_allocator(fc, buf.append)
    ck(rc == 0, "allocator: an allocator that sets g_gc_pending and grows reads 0 GREEN")
    buf.clear(); rc = census_allocator(fd, buf.append)
    ck(rc == 1 and "=1 want=0" in buf[-1], "allocator: a planted rt_gc_collect( inside rt_gcheap_alloc reads 1 RED")
    buf.clear(); rc = census_allocator(os.path.join(w, "nope.c"), buf.append)
    ck(rc == 2, "allocator: a missing file REFUSES rc=2, never green")
    fp = os.path.join(w, "prefixed.c")
    open(fp, "w").write("void *c_rt_gcheap_alloc(uint16_t t, uint64_t n) {\n  if (a) rt_gc_collect();\n  if (b) rt_gc_collect();\n  return carve(n);\n}\n")
    buf.clear(); rc = census_allocator(fp, buf.append)
    ck(rc == 1 and "inside_c_rt_gcheap_alloc=2" in "\n".join(buf) and "body lines" in "\n".join(buf),
       "allocator: the c_-prefixed definition is the one on disk -- two planted sites counted and their body lines named")
    nf = os.path.join(w, "noalloc.c")
    open(nf, "w").write("void *some_other_thing(void) { rt_gc_collect(); return 0; }\n")
    buf.clear(); rc = census_allocator(nf, buf.append)
    ck(rc == 2, "allocator: a file with neither spelling REFUSES rc=2 (a renamed allocator is not a green one)")
    # safe-points on fixture templates with a given allocating set
    tpl_ok = os.path.join(w, "ok.cpp"); tpl_bad = os.path.join(w, "bad.cpp")
    open(tpl_ok, "w").write('std::string a(){ return x86("call", "rt_concat", fp)\n + x86("lea", "r8", "[rip + __]", (uint64_t)&g_gc_pending, "g_gc_pending")\n + x86("test", "eax", "eax"); }\nstd::string b(){ return x86("call", "rt_pure_cmp", fp); }\n')
    open(tpl_bad, "w").write('std::string a(){ return x86("call", "rt_concat", fp)\n + x86("mov", "rax", "rbx"); }\nstd::string c(){ return x86("call", (flag ? name_a : name_b), fp); }\n')
    alloc = {"rt_concat", "rt_gcheap_alloc"}
    buf.clear(); rc = census_safe_points("", [tpl_ok], out=buf.append, allocating=alloc)
    ck(rc == 0 and "allocating_call_sites=1 polled=1 unpolled=0 unresolved=0" in "\n".join(buf), "safe-points: an allocating call followed by a g_gc_pending poll reads polled, a non-allocating call is not counted")
    buf.clear(); rc = census_safe_points("", [tpl_bad], out=buf.append, allocating=alloc)
    ck(rc == 1 and "unpolled=1 unresolved=1" in "\n".join(buf) and any("UNPOLLED" in l and "rt_concat" in l for l in buf) and any("UNRESOLVED" in l for l in buf),
       "safe-points: a planted unpolled allocating call and a computed call target are each named and RED")
    buf.clear(); rc = census_safe_points("", [tpl_bad], out=buf.append, allocating=alloc, poll_helper="gc_poll_here")
    ck(rc == 1, "safe-points: naming a poll helper does not excuse a call that has neither")
    tpl_gk = os.path.join(w, "greek.cpp")
    open(tpl_gk, "w", encoding="utf-8").write('std::string a(){ return x86("call", "rt_epilogue_\u03b3", fp); }\n'
                                              'std::string b(){ return x86("call", flag ? "rt_epilogue_\u03c9" : "rt_pure_cmp", fp); }\n')
    buf.clear(); rc = census_safe_points("", [tpl_gk], out=buf.append, allocating={"rt_epilogue_\u03b3", "rt_epilogue_\u03c9"})
    ck(rc == 1 and "allocating_call_sites=2 polled=0 unpolled=2 unresolved=0" in "\n".join(buf)
       and any("UNPOLLED" in l and "rt_epilogue_\u03b3" in l for l in buf),
       "safe-points: a call target spelled with a Greek port letter is a LITERAL, counted and named -- never UNRESOLVED (32 of 39 were this, 2026-09-17)")
    ck(blob_class_visible({"main", "fn", "PAT$0"}) and not blob_class_visible({"main", "fn", "pattern_helper"}),
       "maps/table: the blob-frame class is VISIBLE only when a stored-pattern graph is in the witness set -- otherwise the census refuses rather than printing a zero by never looking (cto, ARCH-GC 6.2b)")
    tpl_rs = os.path.join(w, "resolved.cpp")
    open(tpl_rs, "w").write(
        'static const char *tbl[3] = { "rt_concat", "rt_pure_cmp", "rt_nope" };\n'
        '#define pick_sym() (flag ? "rt_concat" : "rt_pure_cmp")\n'
        'static const char *chose(void) { if (a) return "rt_concat"; return "rt_pure_cmp"; }\n'
        'void *dop_direct_fp(const char *fn, int64_t n, const char **sym) {\n'
        '  static const struct { const char *nm; const char *sy; } t[] = { { "$u", "rt_concat" }, { "$v", "rt_pure_cmp" }, { 0, 0 } };\n'
        '  return 0;\n'
        '}\n'
        'std::string a(){ return x86("call", tbl[k], fp); }\n'
        'std::string b(){ return x86("call", pick_sym(), fp); }\n'
        'std::string c(){ return x86("call", chose(), fp); }\n'
        'std::string d(){ const char *dsym = 0; void *f = dop_direct_fp(fn, 2, &dsym); return x86("call", dsym, f); }\n'
        'std::string e(){ return x86("call", (flag ? name_a : name_b), fp); }\n')
    buf.clear(); rc = census_safe_points("", [tpl_rs], out=buf.append, allocating=alloc)
    txt = "\n".join(buf)
    ck(rc == 1 and "allocating_call_sites=4 polled=0 unpolled=4 unresolved=1" in txt
       and "via array tbl[]" in txt and "via macro pick_sym()" in txt and "via chooser chose()" in txt
       and "via resolver dop_direct_fp()" in txt,
       "safe-points: the four decidable computed-target shapes (array, macro, chooser, resolver out-parameter) each resolve and NAME their rule")
    ck(rc == 1 and any("UNRESOLVED" in l for l in buf),
       "safe-points: a computed target of no decidable shape stays UNRESOLVED beside the four that resolved -- the census does not guess")
    tpl_tr = os.path.join(w, "macrotrap.cpp")
    open(tpl_tr, "w").write(
        '#define pick_sym() (flag ? "rt_pure_cmp" : "rt_nope")\n'
        'static std::string far_away(void) { return x86("call", "rt_concat", fp); }\n'
        'std::string a(){ return x86("call", pick_sym(), fp); }\n')
    buf.clear(); rc = census_safe_points("", [tpl_tr], out=buf.append, allocating=alloc)
    txt = "\n".join(buf)
    ck(rc == 1 and "via macro pick_sym(): 2 candidate(s), none allocating" in txt and "allocating_call_sites=1" in txt,
       "safe-points: a macro chooser reads ITS OWN replacement text -- not the literals of the next routine in the file (the mis-resolution of 2026-09-17, which hid two allocating rt_cap_open_plain sites)")
    tpl_xf = os.path.join(w, "crossfn.cpp")
    open(tpl_xf, "w").write('std::string a(){ return x86("call", "rt_concat", fp);\n'
                            '}\n'
                            'std::string b(){ return x86("lea", "r8", "[rip + __]", (uint64_t)&g_gc_pending, "g_gc_pending"); }\n')
    buf.clear(); rc = census_safe_points("", [tpl_xf], out=buf.append, allocating=alloc)
    ck(rc == 1 and "allocating_call_sites=1 polled=0 unpolled=1" in "\n".join(buf),
       "safe-points: a g_gc_pending poll in the NEXT routine, inside the line window, does NOT make this site polled (the false green of 2026-09-17)")
    # coverage on captured text
    buf.clear(); rc = census_coverage("", "", out=buf.append, cov_text="[GC-COV] ranges=3 words_scanned=0 interior_words=0 pz=1\n[GC-COV] ranges=3 words_scanned=0 interior_words=0 pz=1\n")
    ck(rc == 0, "coverage: every [GC-COV] line carrying words_scanned=0 AND interior_words=0 reads GREEN")
    buf.clear(); rc = census_coverage("", "", out=buf.append, cov_text="[GC-COV] ranges=3 cas_scanned_bytes=0 pz=1 cons_stack=1\n")
    ck(rc == 1 and "without_words_scanned=1" in "\n".join(buf) and "without_interior_words=1" in "\n".join(buf),
       "coverage: today's line (neither field) reads RED on both tokens, never green")
    buf.clear(); rc = census_coverage("", "", out=buf.append, cov_text="[GC-COV] words_scanned=0 interior_words=0\n[GC-COV] words_scanned=7 interior_words=0\n")
    ck(rc == 1 and "words_scanned_nonzero=1" in "\n".join(buf), "coverage: one line with words_scanned=7 among zeros reads RED")
    buf.clear(); rc = census_coverage("", "", out=buf.append, cov_text="[GC-COV] words_scanned=0 interior_words=0\n[GC-COV] words_scanned=0 interior_words=12\n")
    ck(rc == 1 and "interior_words_nonzero=1" in "\n".join(buf), "coverage: a mapped stack with 12 unmapped WORKLIST-INTERIOR words still reads RED (Rule 2b)")
    buf.clear(); rc = census_coverage("", "", out=buf.append, cov_text="[GC-COV] words_scanned=0 pz=1\n")
    ck(rc == 1 and "without_interior_words=1" in "\n".join(buf) and "without_words_scanned=0" in "\n".join(buf),
       "coverage: half the design (words_scanned only, no interior_words) reads RED on the missing token alone")
    buf.clear(); rc = census_coverage("", "", out=buf.append, cov_text="nothing collected\n")
    ck(rc == 2, "coverage: no [GC-COV] line at all REFUSES rc=2 (no collection ran, nothing measured)")
    # callbacks
    cb_ok, cb_bad = os.path.join(w, "cb_ok.c"), os.path.join(w, "cb_bad.c")
    open(cb_ok, "w").write("DESCR_t rt_call_proc_descr(DESCR_t p, int n) {\n  return x;\n}\nstatic void f(void) { DESCR_t r = RT_GC_CALLBACK(rt_call_proc_descr(p, 2)); use(r); }\n")
    open(cb_bad, "w").write("static void f(void) { char *s = raw; DESCR_t r = rt_call_proc_descr(p, 2); use(s); DESCR_t q = RT_GC_CALLBACK(rt_call_value(v)); }\n")
    buf.clear(); rc = census_callbacks([cb_ok], out=buf.append)
    ck(rc == 0 and "call_back_sites=1 wrapped_in_RT_GC_CALLBACK=1 unwrapped=0" in "\n".join(buf), "callbacks: a definition is not a site; a wrapped call reads GREEN")
    buf.clear(); rc = census_callbacks([cb_bad], out=buf.append)
    ck(rc == 1 and "call_back_sites=2 wrapped_in_RT_GC_CALLBACK=1 unwrapped=1" in "\n".join(buf), "callbacks: a planted unwrapped call-back is counted and RED beside the wrapped one")
    nogc = os.path.join(w, "nomaps"); os.makedirs(os.path.join(nogc, "src", "runtime", "rt"), exist_ok=True)
    open(os.path.join(nogc, "src", "runtime", "rt", "gc_heap.c"), "w").write("void *c_rt_gcheap_alloc(uint16_t t, uint64_t n) { return carve(n); }\n")
    buf.clear(); rc = census_maps_table(nogc, "/nonexistent/scrip", [], buf.append)
    ck(rc == 2 and "are not defined in gc_heap.c" in "\n".join(buf),
       "maps/table: a gc_heap.c without section 6.4's symbol REFUSES rc=2 -- an unbuilt table never reads green")
    buf.clear(); rc = census_maps_slotkind(nogc, ["rebus"], buf.append)
    ck(rc == 2 and "no util_zls_frame_map_census.py" in "\n".join(buf), "maps/slot-kind: a missing base tool REFUSES rc=2, never green")
    ck(decode_map_quads(".Lgcmap_x:\n  .quad 688541224282\n  .quad 4294967296\n  .quad .Lgcmap_x_s\n  .quad 0\n", ".Lgcmap_x")
       == {"magic": 0x50414D5A, "frame_bytes": 160, "header_bytes": 0, "flags": 1},
       "maps/table: the four static quads decode to magic ZMAP, frame_bytes, header_bytes and flags (section 6.3)")
    ck(decode_map_quads(".Lgcmap_y:\n  .quad 1\n  .quad 2\n", ".Lgcmap_y") is None,
       "maps/table: a map with fewer than four quads does not decode, and an undecodable map is a divergence not a pass")
    # the ratchet itself, against a synthetic COUNTS -- no census runs, so the proof costs nothing
    saved = dict(COUNTS)
    try:
        COUNTS.clear()
        for census, key, _pop in RATCHET_KEYS:
            COUNTS.setdefault(census, {})[key] = 7
        bl = os.path.join(w, "base.tsv")
        def write_base(bump=0, drop=None):
            with open(bl, "w") as fh:
                for census, key, _pop in RATCHET_KEYS:
                    if (census, key) == drop:
                        continue
                    fh.write(f"{census}\t{key}\t{7 + (bump if (census, key) == BUMP_KEY else 0)}\t0\n")
        BUMP_KEY = RATCHET_KEYS[0][:2]
        write_base(); buf.clear(); rc = ratchet(bl, buf.append)
        ck(rc == 0 and any(l.startswith("RATCHET GREEN") for l in buf), "ratchet: every count exactly its baseline reads GREEN")
        write_base(bump=-1); buf.clear(); rc = ratchet(bl, buf.append)
        ck(rc == 1 and any("⛔ WORSE" in l for l in buf), "ratchet: a planted INCREASE over the baseline is named and RED")
        write_base(bump=+5); buf.clear(); rc = ratchet(bl, buf.append)
        ck(rc == 1 and any("⭐ BETTER" in l for l in buf), "ratchet: a FALL the baseline has not recorded is RED too -- a loose ratchet lets the regression back in")
        write_base(drop=BUMP_KEY); buf.clear(); rc = ratchet(bl, buf.append)
        ck(rc == 2 and any("NOT-MEASURED" in l for l in buf), "ratchet: a key the baseline does not name REFUSES rc=2, never green")
        buf.clear(); rc = ratchet(os.path.join(w, "no-such-baseline.tsv"), buf.append)
        ck(rc == 2, "ratchet: a missing baseline REFUSES rc=2, never green")
    finally:
        COUNTS.clear(); COUNTS.update(saved)
    print(f"population: {arms} selftest arm(s), {fails} FAIL")
    if arms < ARMS_FLOOR:
        print(f"SELFTEST REFUSED(2): {arms} arm(s) ran, below the recorded floor {ARMS_FLOOR} -- an arm was removed or "
              "skipped, and less proof is not a pass")
        return 2
    print("SELFTEST " + ("PASS" if fails == 0 else "FAIL"))
    return 0 if fails == 0 else 1


def main(argv):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("census", nargs="?", default="all", choices=["all", "conservative", "allocator", "safe-points", "coverage", "callbacks", "maps"])
    ap.add_argument("--selftest", action="store_true")
    ap.add_argument("--ratchet", default="", help="a baseline TSV (census<TAB>key<TAB>count<TAB>note); refuse an increase, and a fall the baseline has not recorded")
    ap.add_argument("--write-baseline", default="", help="write today's counts as a baseline TSV (the landing that earned the fall runs this)")
    ap.add_argument("--root", default=ROOT)
    ap.add_argument("--poll-window", type=int, default=12)
    ap.add_argument("--poll-helper", default="")
    ap.add_argument("--zls-langs", default="fast", help="fast (rebus,snocone,pascal ~2.7s), all (the seven, ~32s), or a comma list")
    ap.add_argument("--map-witness", default="", help="comma list of programs the table half reads (default: the two fixtures)")
    ap.add_argument("--callback-marker", default="RT_GC_CALLBACK")
    ap.add_argument("--witness", default="", help="the program the coverage census runs (default: a small allocating SNOBOL4 loop)")
    a = ap.parse_args(argv)
    if a.selftest:
        return selftest()
    R = a.root
    gc = os.path.join(R, "src", "runtime", "rt", "gc_heap.c")
    so = os.path.join(R, "out", "libscrip_rt.so")
    scrip = os.path.join(R, "scrip")
    emitter_files = []
    for d in ("src/templates/bb", "src/templates/xa", "src/templates/x86", "src/emitter"):
        dd = os.path.join(R, d)
        if os.path.isdir(dd):
            emitter_files += [os.path.join(dd, f) for f in sorted(os.listdir(dd)) if f.endswith((".cpp", ".c", ".h"))]
    rcs = []
    want = [a.census] if a.census != "all" else ["conservative", "allocator", "safe-points", "coverage", "callbacks", "maps"]
    for c in want:
        if c == "conservative":
            rcs.append(census_conservative(gc))
        elif c == "allocator":
            rcs.append(census_allocator(gc))
        elif c == "safe-points":
            rcs.append(census_safe_points(so, emitter_files, a.poll_window, a.poll_helper))
        elif c == "coverage":
            prog = a.witness
            if not prog:
                prog = os.path.join(tempfile.mkdtemp(prefix="gc_cov."), "w.sno")
                open(prog, "w").write("        T = TABLE()\n        I = 0\nLOOP    I = I + 1\n        T[I] = DUPL('x', I) 'y' I\n        S = S T[I]\n        LT(I, 300)   :S(LOOP)\n        OUTPUT = SIZE(S)\nEND\n")
            rcs.append(census_coverage(scrip, prog))
        elif c == "callbacks":
            rcs.append(census_callbacks([os.path.join(R, "src", "runtime", "by_name_dispatch.c"), os.path.join(R, "src", "runtime", "builtins", "gen_runtime.c")], a.callback_marker))
        elif c == "maps":
            langs = ZLS_LANGS_ALL if a.zls_langs == "all" else (ZLS_LANGS_FAST if a.zls_langs == "fast" else [x.strip() for x in a.zls_langs.split(",") if x.strip()])
            wits = [x.strip() for x in a.map_witness.split(",") if x.strip()] or \
                   [os.path.join(R, "scripts", "fixtures", "gc_roots_witness.sno"), os.path.join(R, "scripts", "fixtures", "gc_map_witness.icn")]
            rcs.append(census_maps(R, scrip, langs, wits))
    rc = worst(rcs)
    print(f"population: {len(want)} census(es): {rcs.count(0)} green, {rcs.count(1)} red, {rcs.count(2)} not measured -- rc={rc}")
    if a.write_baseline:
        with open(a.write_baseline, "w", encoding="utf-8") as fh:
            fh.write("# gc_census_baseline.tsv -- the counts THE COLLECTOR GUESSES NOTHING drives to 0 (coo, CEO-818/819 section 7).\n")
            fh.write("# Written by `util_gc_census.py all --write-baseline`; every landing that lowers a count rewrites this file\n")
            fh.write("# in the same sitting -- test_gate_gc_instrument_censuses_are_wired_and_trip.sh reds on an increase AND on an\n")
            fh.write("# unrecorded fall.  census\tkey\tcount\twant\n")
            for line in CRITERION_CHANGES:
                fh.write(f"# CRITERION CHANGED {line}\n")
            for census, key, _pop in RATCHET_KEYS:
                v = COUNTS.get(census, {}).get(key)
                if v is not None:
                    fh.write(f"{census}\t{key}\t{v}\t0\n")
        print(f"baseline written: {a.write_baseline}")
    if a.ratchet:
        rrc = ratchet(a.ratchet)
        rc = 2 if 2 in (rc, rrc) else (1 if rrc else rc)
    return rc


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
