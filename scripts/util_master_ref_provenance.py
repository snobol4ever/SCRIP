#!/usr/bin/env python3
"""util_master_ref_provenance.py -- WHERE DID THIS MASTER'S REFS COME FROM?  Asked of the declared oracle, one
entry at a time, with a printed denominator and classes that must sum to it.  (cfo 2026-09-12, row
master-refs-were-cut-from-our-own-output-not-the-oracle-census-every-self-pinned-entry; the master-level
sibling of hq_T's util_icon_ref_provenance.sh, which asks the same question of a PACKAGE.)

    python3 scripts/util_master_ref_provenance.py icon              # census of corpus/tests/icon/ALL.icn
    python3 scripts/util_master_ref_provenance.py icon --names      # only the names that need a decision
    python3 scripts/util_master_ref_provenance.py icon --tsv out.tsv

THE QUESTION IS NOT "DOES SCRIP PASS".  A master .ref is supposed to be the ORACLE's answer; a ref cut from
SCRIP's own output freezes whatever we did on the day it was cut and grades us against ourselves forever --
it never goes red, it silently certifies (measured case: procedure_every_alt_replace_4 in the icon master
pinned another implementation's &features, &regions and a bare &progname, and Arizona icont could not even
compile the entry).  So for every oracle-graded entry this runs THE ONE ORACLE (resolved through
lib_oracle_flags.sh via the harness's own resolve_oracle_bin -- never a hand-built path) exactly the way the
harness's capture-oracle-refs runs it (run_oracle: bare basename, cwd = the file's own directory, the entry's
own stdin and argv sidecars fed), and asks whether the oracle reproduces the ref the entry is graded against.

CLASSES (every entry lands in exactly one; the summary REFUSES rc=2 if they do not sum to examined):
  ORACLE_REPRODUCES   the oracle prints the ref (after the CEO-409 mask and, for one mode, the CEO-581 per-mode
                      ref -- both applied exactly as the harness applies them).  A red here is ours.
  RULED_PIN           the ref disagrees with the oracle ON A STATED RULING recorded in ALL.refpins.tsv (pin-ref
                      --ruling).  Not a self-pin: a decision with a ledger.
  DECLARED_OUTSIDE    the entry is named in ALL.outside.tsv with the oracle's refusal recorded.
  AST_GRADED          ALL.csv declares modes=ast: the ref is SCRIP's own AST dump BY DESIGN (no oracle emits it;
                      row ast-dump-refs-are-self-pins-not-oracles).  Counted for population, never compared.
  ORACLE_REFUSES      the oracle ran and printed NOTHING with a non-zero rc, or was killed/hung/absent: there is
                      no oracle answer, so the ref cannot be the oracle's -- it belongs in ALL.outside.tsv with
                      that refusal named, never in the graded denominator.
  NONDETERMINISTIC    the oracle disagreed with the ref AND with its own second run: it cannot be evidence
                      about a ref it does not agree with either (hq_T's control-arm lesson).
  ORACLE_DIFFERS      ⛔ THE FINDING.  The oracle reproduces itself and does NOT print the ref, and no ruling or
                      outside row explains it.  Every verdict against that ref -- red OR GREEN -- is a statement
                      about something other than the language.  Re-cut from the oracle, or rule it (pin-ref),
                      or declare it outside; the census names it until one of the three happens.

EXIT: 0 census printed (rc says nothing about the counts; read them) · 2 REFUSED (no oracle, no master, an
ast-only dialect, zero entries examined, or classes that do not sum).
"""
import argparse
import os
import subprocess
import sys
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
import corpus_suite_harness as h  # noqa: E402


def refuse(msg):
    print(f"⛔ REFUSED(2) [util_master_ref_provenance]: {msg}", file=sys.stderr)
    sys.exit(2)


def read_outside(master_src):
    p = Path(str(master_src).rsplit(".", 1)[0] + ".outside.tsv")
    out = {}
    if not p.is_file():
        return out
    for ln in p.read_text(encoding="utf-8").splitlines():
        if not ln.strip() or ln.lstrip().startswith("#"):
            continue
        f = ln.split("\t")
        if len(f) >= 2:
            out[f[0].strip()] = f[1].strip()
    return out


def run_oracle_with_err(oracle_bin, flags, sp, timeout, stdin_text, prog_args):
    """run_oracle's exact invocation (bare basename, cwd = the file's own directory, the entry's stdin and argv),
    reproduced here only because run_oracle discards stderr and a census must NAME the oracle's refusal."""
    argv = [oracle_bin] + flags.split() + [sp.name] + list(prog_args or [])
    kind, out, err, rc = h._run_raw(argv, timeout, cwd=str(sp.parent), stdin_text=stdin_text)
    if kind == "RAN" and rc is not None and rc < 0:
        kind = "CRASH"
    err1 = next((ln.strip() for ln in err.decode("utf-8", "replace").splitlines() if ln.strip()), "")
    return out.decode("utf-8", "replace").rstrip("\n"), rc, kind, err1


def apply_masks(text, rows):
    if not rows or text is None:
        return text
    lines = text.split("\n")
    for rx, _why in rows:
        lines = ["<MASKED>" if rx.search(ln) else ln for ln in lines]
    return "\n".join(lines)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("lang", choices=sorted(h.LANG_CONFIGS) + ["snobol4"])
    ap.add_argument("--names", action="store_true", help="print only ORACLE_DIFFERS / ORACLE_REFUSES names")
    ap.add_argument("--tsv", help="also write the per-entry table here")
    ap.add_argument("--limit", type=int, default=0, help="examine only the first N entries (a smoke run)")
    args = ap.parse_args()
    paths = h.resolve_paths()
    lang = args.lang
    cfg = h.LANG_CONFIGS.get(lang, {"ext": ".sno", "comment_open": "*", "comment_close": "", "modes": "m3,m4"})
    if cfg["modes"] == "ast":
        refuse(f"{lang}'s master is graded on SCRIP's own AST dump (LANG_CONFIGS modes=ast): every ref is a self-pin "
               f"by design and no oracle emits the shape -- this census has nothing to ask")
    master_dir = paths["corpus_root"] / "tests" / lang if "corpus_root" in paths else Path(HERE.parent.parent / "corpus" / "tests" / lang)
    src = master_dir / f"ALL{cfg['ext']}"
    ref = master_dir / "ALL.ref"
    for p in (src, ref):
        if not p.is_file():
            refuse(f"no master at {p}")
    oracle_bin, flags = h.resolve_oracle_bin(paths, "" if lang == "snobol4" else lang)
    if lang == "snobol4":
        entries = h.read_suite(str(src), str(ref), in_path=h.sidecar_in_path(str(src)), x_path=h.sidecar_xfail_path(str(src)),
                               w_path=h.sidecar_wantrc_path(str(src)), a_path=h.sidecar_argv_path(str(src)))
    else:
        entries = h.read_block_suite(str(src), str(ref), h.banner_re_for(cfg["comment_open"], cfg["comment_close"]),
                                     in_path=h.sidecar_in_path(str(src)), x_path=h.sidecar_xfail_path(str(src)),
                                     w_path=h.sidecar_wantrc_path(str(src)), a_path=h.sidecar_argv_path(str(src)))
    masks = h.read_mask_sidecar(str(ref))
    moderefs = h.read_moderef_sidecar(str(ref))
    pins = h.read_refpins(str(src))
    outside = read_outside(str(src))
    entry_modes = {}
    try:
        entry_modes, _csv = h.modes_declarations(str(src))
    except SystemExit:
        entry_modes = {}
    if args.limit:
        entries = entries[: args.limit]
    if not entries:
        refuse(f"zero entries read from {src}")
    print(f"oracle: {oracle_bin} {flags}".rstrip(), file=sys.stderr)
    print(f"master: {src} ({len(entries)} entries) timeout={paths['timeout']}s", file=sys.stderr)
    rows = []
    counts = {k: 0 for k in ("ORACLE_REPRODUCES", "RULED_PIN", "DECLARED_OUTSIDE", "AST_GRADED", "ORACLE_REFUSES",
                             "NONDETERMINISTIC", "ORACLE_DIFFERS")}
    with tempfile.TemporaryDirectory(prefix="mrefprov.") as td:
        for i, e in enumerate(entries, 1):
            name = e.name
            ref_text = e.ref if isinstance(e.ref, str) else "\n".join(e.ref)
            ref_text = ref_text.rstrip("\n")
            if entry_modes.get(name, "") == "ast":
                cls, detail = "AST_GRADED", "modes=ast in ALL.csv"
            elif name in pins:
                cls, detail = "RULED_PIN", f"{pins[name]['measurer']} {pins[name]['date']}: {pins[name]['ruling'][:80]}"
            elif name in outside:
                cls, detail = "DECLARED_OUTSIDE", outside[name]
            else:
                d = Path(td) / f"e{i}"
                d.mkdir()
                sp = d / f"{name}{cfg['ext']}"
                body = "\n".join(e.sno_lines) + "\n"
                sp.write_text(body, encoding="utf-8")
                argv = list(getattr(e, "argv", None) or [])
                h._copy_companions(body, master_dir, d)
                if paths.get("inc") and Path(paths["inc"]).is_dir():
                    h._copy_companions(body, Path(paths["inc"]), d)
                for tok in argv:
                    if tok and "/" not in tok and (master_dir / tok).is_file() and not (d / tok).exists():
                        (d / tok).write_bytes((master_dir / tok).read_bytes())
                text, rc, kind, err1 = run_oracle_with_err(oracle_bin, flags, sp, paths["timeout"], e.stdin, argv)
                want_rc = getattr(e, "want_rc", 0)
                mrows = h.masks_for(masks, name) if masks else []
                exp0 = apply_masks(ref_text, mrows)
                got = apply_masks(text, mrows)
                candidates = [("ref", exp0)]
                for mode in ("m3", "m4"):
                    mr = h.moderefs_for(moderefs, name, mode) if moderefs else []
                    if mr:
                        try:
                            exp_m, _n = h.apply_moderef(ref_text, mr, where=f"{name} [{mode}]")
                            candidates.append((mode, apply_masks(exp_m, mrows)))
                        except SystemExit:
                            pass
                if kind != "RAN":
                    cls, detail = "ORACLE_REFUSES", f"oracle {kind}; stderr: {err1[:120]}"
                elif any(got == exp for _lbl, exp in candidates):
                    lbl = next(l for l, exp in candidates if got == exp)
                    cls, detail = "ORACLE_REPRODUCES", (f"via {lbl} per-mode ref" if lbl != "ref" else "") + (f" rc={rc} wantrc={want_rc}" if rc != want_rc else "")
                    detail = detail.strip()
                elif rc != 0 and not text.strip():
                    cls, detail = "ORACLE_REFUSES", f"rc={rc}, no stdout -- no oracle answer exists for this entry; stderr: {err1[:120]}"
                else:
                    text2, rc2, kind2, _e2 = run_oracle_with_err(oracle_bin, flags, sp, paths["timeout"], e.stdin, argv)
                    if (text2, rc2, kind2) != (text, rc, kind):
                        cls, detail = "NONDETERMINISTIC", f"two oracle runs disagree (rc {rc} vs {rc2})"
                    else:
                        el, ol = exp0.split("\n"), got.split("\n")
                        nd = sum(1 for a, b in zip(el, ol) if a != b) + abs(len(el) - len(ol))
                        first = next(((a, b) for a, b in zip(el, ol) if a != b), (el[len(ol)] if len(el) > len(ol) else "", ol[len(el)] if len(ol) > len(el) else ""))
                        cls = "ORACLE_DIFFERS"
                        detail = f"rc={rc} wantrc={want_rc} ref_lines={len(el)} oracle_lines={len(ol)} differing={nd} first: ref={first[0][:60]!r} oracle={first[1][:60]!r}; stderr: {err1[:100]}"
            counts[cls] += 1
            rows.append((cls, name, detail))
            if not args.names:
                print(f"{cls}\t{name}\t{detail}")
            elif cls in ("ORACLE_DIFFERS", "ORACLE_REFUSES", "NONDETERMINISTIC"):
                print(f"{cls}\t{name}\t{detail}")
    if args.tsv:
        Path(args.tsv).write_text("".join(f"{c}\t{n}\t{d}\n" for c, n, d in rows), encoding="utf-8")
    examined = len(entries)
    total = sum(counts.values())
    summary = " ".join(f"{k.lower()}={v}" for k, v in counts.items())
    if total != examined:
        refuse(f"classes sum to {total} over {examined} examined -- a census whose classes do not cover its population is not a census")
    if examined == 0:
        refuse("examined zero entries")
    print(f"examined {examined} of {len(entries)} entries in {src.name}: {summary}")
    print(f"SELF-PIN CANDIDATES (oracle_differs): {counts['ORACLE_DIFFERS']}  NO-ORACLE-ANSWER (oracle_refuses, belong in ALL.outside.tsv): {counts['ORACLE_REFUSES']}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
