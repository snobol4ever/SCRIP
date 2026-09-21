#!/usr/bin/env python3
"""util_gc_acceptance.py -- THE FOUR NUMBERS THE GC COMMITMENT IS MADE OF, computed, never typed.

Lon 2026-09-21, in-chat to the ceo, accepting the ceo's four definitions and ordering the display:
"Then we will commit to that. ... Can you display periodically the metrics which you are measuring
to state we made it 100%."  CEO-1039.  ARCH-GC-COMPILE-TIME-FRAME-MAPS.md section 11.

THE FOUR, each with a DENOMINATOR so the claim is falsifiable:
  1 INSTRUMENTED  every C->BB site hooked, every allocating call site polled, every callback wrapped.
  2 VISIBILITY    every event in the DECLARED event space observable and observed.
  3 AUDITED       every collection checked by the exact-vs-conservative differential pass, and its
                  SILENCE is the proof.  NOT 'every defect found' -- that is unprovable and the ceo
                  refused to promise it (the cfo's trap converts 2 of 17).
  4 WORKING       ZERO gradings lost to the collector across the arena and stress axes.  This is NOT
                  the master's absolute score: SnoM's 11 non-GC gaps are language completeness and
                  are frozen.  The target is that stress costs zero of them; today it costs 26+4 hangs.

INSTRUMENT LAW, held here rather than quoted: a metric that CANNOT BE MEASURED prints NOT-BUILT or
NOT-MEASURED and makes the verdict REFUSE(2).  It never prints 0 and never prints 100.  A display
that cannot tell 'clean' from 'never ran' is the thing this whole emergency is about.
"""
import os, re, subprocess, sys, datetime
HERE = os.path.dirname(os.path.abspath(__file__)); ROOT = os.path.dirname(HERE)

def sh(cmd, timeout=1800):
    try:
        p = subprocess.run(cmd, cwd=ROOT, shell=True, capture_output=True, text=True, timeout=timeout)
        return p.returncode, p.stdout + p.stderr
    except Exception as e:
        return 2, "REFUSE %s" % e

def num(pat, txt, default=None):
    m = re.search(pat, txt)
    return int(m.group(1)) if m else default

def pct(n, d):
    return "n/a" if not d else "%.1f%%" % (100.0 * n / d)

def main():
    tree = sh("git rev-parse --short HEAD")[1].strip()
    corpus = sh("git -C ../corpus rev-parse --short HEAD")[1].strip()
    when = datetime.datetime.now().strftime("%Y-%m-%d %H:%M %Z")
    load = open("/proc/loadavg").read().split()[0]
    print("=" * 118)
    print("GC ACCEPTANCE -- the four numbers, computed (CEO-1039)   SCRIP %s . corpus %s . %s . load %s"
          % (tree, corpus, when, load))
    print("=" * 118)
    refuse, fails = [], []

    # ---- 1 INSTRUMENTED -------------------------------------------------------------------------
    rc, cen = sh("SCRIP_GC_CENSUS_LIST_ALL=1 python3 scripts/util_gc_census.py")
    polled   = num(r"allocating_call_sites=(\d+)", cen); pol_ok = num(r"\bpolled=(\d+)", cen)
    inb      = num(r"INBOUND call_back_sites=(\d+)", cen); inb_w = num(r"INBOUND call_back_sites=\d+ wrapped_in_\w+=(\d+)", cen)
    outb     = num(r"OUTBOUND call_out_sites=(\d+)", cen); out_w = num(r"OUTBOUND call_out_sites=\d+ wrapped_in_\w+=(\d+)", cen)
    cons     = num(r"CENSUS conservative total=(\d+)", cen)
    c2bb_all = inb + outb if (inb is not None and outb is not None) else None
    rc2, hk  = sh("grep -rn 'rt_c2bb_hit' src/runtime --include=*.c | grep -v 'void rt_c2bb_hit' | "
                  "grep -cE 'by_name_dispatch.c|core/core.c|runtime_eval.c' || true")
    c2bb_hooked = int(hk.strip() or 0)
    print("\n  1 INSTRUMENTED")
    if None in (polled, pol_ok, inb, outb, cons):
        refuse.append("INSTRUMENTED: the census did not print a line this reader needs"); print("      REFUSE(2) -- census line missing")
    else:
        marker_absent = "marker=RT_GC_CALLBACK defined_in_tree=NO" in cen
        rows = [("safe-point polls at allocating call returns", pol_ok, polled),
                ("callback wraps (RT_GC_CALLBACK)",             inb_w + out_w, c2bb_all),
                ("C->BB entry sites carrying rt_c2bb_hit",      c2bb_hooked, c2bb_all)]
        for nm, n, d in rows:
            if nm.startswith("callback wraps") and marker_absent:
                print("      %-46s %s" % (nm, "REFUSE(2) -- THE WRAPPER DOES NOT EXIST"))
                print("      %-46s %s" % ("", "RT_GC_CALLBACK is not defined anywhere in the tree, so 0 of %d is NOT a" % (d or 0)))
                print("      %-46s %s" % ("", "backlog of %d wraps waiting to be written -- a wrap written today would" % (d or 0)))
                print("      %-46s %s" % ("", "not compile.  \u26d4 AND THE COMPONENT IS SATISFIABLE BY A NO-OP: a macro"))
                print("      %-46s %s" % ("", "defined as #define RT_GC_CALLBACK(x) x, applied to all %d sites, takes" % (d or 0)))
                print("      %-46s %s" % ("", "this line to 100% WITH NO SEMANTIC CHANGE WHATSOEVER.  A component of an"))
                print("      %-46s %s" % ("", "acceptance commitment that a no-op can satisfy is not a measurement, and"))
                print("      %-46s %s" % ("", "reporting it as '%d open' invited exactly that (CEO-1070)." % (d or 0)))
                print("      %-46s %s" % ("", "IT REFUSES UNTIL Rule 4 IS GIVEN A MEANING THE WRAPPER ENFORCES:"))
                print("      %-46s %s" % ("", "a runtime frame holds NO RAW HEAP POINTER across a callback into emitted"))
                print("      %-46s %s" % ("", "code.  Define that first, prove it fails once, then count sites."))
                refuse.append("INSTRUMENTED/callback: RT_GC_CALLBACK is undefined in the tree -- "
                              "0 of %d is unmeasurable, and a no-op macro would read 100%%" % (d or 0))
                continue
            if nm.startswith("C->BB"):
                rc3, live = sh("grep -rn 'rt_c2bb_hit' src/runtime --include=*.c | "
                               "grep -v 'void rt_c2bb_hit' | sed 's|src/runtime/||; s|:.*||' | sort -u | tr '\\n' ' '")
                nhook = 0
                rc4, cnt = sh("grep -rn 'rt_c2bb_hit' src/runtime --include=*.c | grep -vc 'void rt_c2bb_hit' || true")
                try: nhook = int(cnt.strip() or 0)
                except ValueError: nhook = 0
                print("      %-46s %s" % (nm, "REFUSE(2) -- NUMERATOR AND DENOMINATOR ARE DIFFERENT POPULATIONS"))
                print("      %-46s %s" % ("", "The numerator counts occurrences of rt_c2bb_hit in THREE NAMED FILES"))
                print("      %-46s %s" % ("", "(by_name_dispatch.c, core/core.c, runtime_eval.c) and reads %d." % n))
                print("      %-46s %s" % ("", "The denominator %d is the INBOUND+OUTBOUND CALLBACK SITE count -- the same" % (d or 0)))
                print("      %-46s %s" % ("", "%d the callback line above uses.  Those are not the same population, so" % (d or 0)))
                print("      %-46s %s" % ("", "the fraction is not a coverage reading of anything."))
                print("      %-46s %s" % ("", "\u26d4 AND %d HOOKS DO EXIST -- all in %s-- which the" % (nhook, live.strip() or "rt/rt.c ")))
                print("      %-46s %s" % ("", "numerator EXCLUDES BY CONSTRUCTION.  So the zero is not 'nothing is"))
                print("      %-46s %s" % ("", "hooked', it is 'the instrument does not look where the hooks are'."))
                print("      %-46s %s" % ("", "This is CEO-1041 verbatim: I told Lon '0 of 49 enforced' as though 49"))
                print("      %-46s %s" % ("", "were a live population, called that zero a blindfold, and then left the"))
                print("      %-46s %s" % ("", "instrument unchanged for a day.  Recording a lesson is not fixing it."))
                print("      %-46s %s" % ("", "IT REFUSES until the C->BB entry sites have their OWN denominator and the"))
                print("      %-46s %s" % ("", "numerator counts hooks wherever they live (CEO-1071)."))
                refuse.append("INSTRUMENTED/C->BB: numerator counts 3 files, denominator counts callback sites, "
                              "and the %d existing hooks are excluded by construction -- not a coverage reading" % nhook)
                continue
            bad = (n < d)
            print("      %-46s %5d / %-5d  %-7s %s" % (nm, n, d, pct(n, d), "" if not bad else "<- %d open" % (d - n)))
            if bad: fails.append("INSTRUMENTED/%s %d of %d" % (nm.split()[0], n, d))
        print("      %-46s %5d / %-5d  %-7s %s" % ("conservative visits deleted (want 0 remaining)", cons, 0, "100.0%" if cons == 0 else "n/a",
                                                   "" if cons == 0 else "<- %d remain" % cons))
        if cons: fails.append("INSTRUMENTED/conservative %d remain" % cons)

    # ---- 2 VISIBILITY ---------------------------------------------------------------------------
    rc, ev = sh("python3 scripts/util_gc_event_coverage.py")
    ex  = num(r"EXERCISED (\d+)", ev); obs = num(r"OBSERVABLE (\d+)", ev)
    owed = num(r"OWED -- real events this collector cannot distinguish \((\d+)\)", ev)
    print("\n  2 VISIBILITY")
    if None in (ex, obs):
        refuse.append("VISIBILITY: util_gc_event_coverage.py printed no denominator identity"); print("      REFUSE(2) -- no denominator identity line")
    else:
        print("      %-46s %5d / %-5d  %-7s %s" % ("declared events observed", ex, obs, pct(ex, obs),
                                                   "" if ex == obs else "<- %d not exercised" % (obs - ex)))
        print("      %-46s %5d          %-7s %s" % ("events the collector CANNOT distinguish", owed or 0, "",
                                                    "" if not owed else "<- OWED, named not dropped"))
        if ex != obs: fails.append("VISIBILITY %d of %d exercised" % (ex, obs))
        if owed:      fails.append("VISIBILITY %d event(s) OWED -- unmeasurable, not merely unmeasured" % owed)

    # ---- 3 AUDITED ------------------------------------------------------------------------------
    rc, aud = sh("grep -rln 'gc_audit_pass_b\\|GC_AUDIT_PASS_B\\|SCRIP_GC_AUDIT_B' src/runtime || true")
    print("\n  3 AUDITED (every collection checked by the exact-vs-conservative differential; its SILENCE is the proof)")
    if not aud.strip():
        print("      NOT BUILT -- the pass-B auditor does not exist in src/runtime.  This metric REFUSES rather")
        print("                   than reading 0%: a display that cannot tell 'clean' from 'never ran' is the")
        print("                   defect this emergency is about.  Row is Lon's grant of 2026-09-21 with an expiry.")
        refuse.append("AUDITED: the pass-B auditor is not built, so collections-audited cannot be measured")
    else:
        print("      built in: %s" % aud.strip().replace("\n", ", "))

    # ---- 4 WORKING ------------------------------------------------------------------------------
    print("\n  4 WORKING (zero gradings lost to the collector -- NOT the master's absolute score)")
    print("      NOT MEASURED HERE BY DESIGN: this needs a board pass per language across the arena and")
    print("      stress axes, which is the one runner's job (MODE LANES), not a 20-second display's.")
    print("      \u26d4 THE LINES BELOW ARE TYPED, NOT COMPUTED, AND THEY ARE FROZEN AT THE 2026-09-21 STAND-DOWN.")
    print("         They are each lane's LAST SELF-REPORTED reading, tree named in that lane's baton, which is")
    print("         the authority -- NOT this display.  A typed figure in a computed dashboard is the defect this")
    print("         dashboard exists against, and hq_snobol4 caught this section quoting a number they had already")
    print("         WITHDRAWN (CEO-1052).  Under EXECUTIVE the six lanes are stood down and these cannot refresh")
    print("         themselves, so READ THE BATON before acting on any line here.")
    for lane, txt in (("RakM", "-65 gradings over 36 programs at stress 16; BOTH FAMILIES SINCE CURED by hq_raku, row closed 853/929 over three arms"),
                      ("SnoM", "CORRECTED BY ITS OWN LANE: a 19-entry reading with hang=0 crash=0, superseding the withdrawn -26-plus-4-hangs"),
                      ("IcnM", "-28 gradings; 27 of them REPRODUCE AT THE SHIPPED 512 MB ARENA (stress 16); 17 are every-driven generator resumption"),
                      ("SncM", "red at stress 1 alone across a sixteen-point band; the one entry SINCE CURED by the DT_X landing 83b8bc9d2"),
                      ("PasM", "LOWER BOUND CLEAN, empty name set at stress 4 and 16 -- its lane's own relabelling, never a clearance"),
                      ("ProM", "the exception ball SINCE CURED at CFO-119/5fc6ae1bc; its 74-pair population is UNMEASURED against the cure and is owed")):
        print("      %-6s %s" % (lane, txt))
    refuse.append("WORKING: no board pass on this tree -- the last readings are 2026-09-20 and the collector has moved")

    # ---- VERDICT --------------------------------------------------------------------------------
    print("\n" + "-" * 118)
    if refuse:
        print("  VERDICT: REFUSE(2) -- NOT AT 100%%, and %d of the four cannot even be measured today:" % len(refuse))
        for r in refuse: print("      ⛔ %s" % r)
        for f in fails:  print("      ✗  %s" % f)
        print("  ⛔ A REFUSE IS NOT A RED.  It says COULD NOT MEASURE.  Neither is it progress.")
        print("=" * 118); return 2
    if fails:
        print("  VERDICT: NOT YET -- every metric measured, %d short of 100%%:" % len(fails))
        for f in fails: print("      ✗  %s" % f)
        print("=" * 118); return 1
    print("  VERDICT: ✅ 100%% ON ALL FOUR -- instrumented, visible, audited, and zero gradings lost to the collector.")
    print("=" * 118); return 0

if __name__ == "__main__":
    sys.exit(main())
