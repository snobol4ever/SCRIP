#!/usr/bin/env bash
# test_gate_xfail_has_reason.sh -- A REASONLESS XFAIL IS A RED, NOT A PASS (row xfail-entries-carry-a-
# reason-or-are-reds, minted by hq_T 2026-09-03; GOAL MASTER-PLAN I23). An xfail line with no stated WHY is
# indistinguishable from a red somebody hid, and the plan deliberately converts uncured reds into REASONED
# xfails by H+32 -- so this gate is what keeps a 100% cell from becoming exactly that trophy-cabinet number.
#
# MECHANISM: every corpus/tests/<lang>/ALL<ext> source (ext from the driver's own frontend list -- .sno
# .icn .pl .sc .reb .raku .pas) embeds a banner comment before each xfail entry: "<dashes> <rank> <entry>
# XFAIL", wrapped in whatever comment syntax that language uses (*, #, %, /* */, { }). corpus/tests/<lang>/
# ALL.xfail (when present) repeats the SAME banner line as a plain-text header, followed by reason prose,
# one block per xfail entry, until the next banner or EOF.
# ⛔ THIS GATE IS COMMENT-SYNTAX AGNOSTIC ON PURPOSE (byte-safe pattern over authored text, THE INSTRUMENT
# LAWS): it matches the core "-{3,} RANK ENTRY XFAIL" shape wherever it sits on a line, so it needs no
# per-language comment-char table and cannot silently under-count a language whose syntax it forgot.
# A reason "counts" if the prose block is non-empty and is not a bare placeholder token (todo/tbd/unknown/
# n/a/-/?/fixme, case-insensitive, matched against the WHOLE stripped block) -- no length floor beyond that:
# this project's own ALL.xfail already carries real reasons as short as one clause, and a length floor would
# reject those alongside the placeholders it exists to catch.
# OUT OF SCOPE, NAMED SO THE NEXT READER ISN'T SURPRISED: corpus/tests/icon/rung36_jcon_*.xfail is a
# DIFFERENT mechanism (one reason file per vendor JCON suite file, not per ALL.csv entry; born of the
# icon-jcon-suite-39 census row) and is not scored by any language's 100% cell -- left to that row's owner.
# EXIT 0 every XFAIL-banner entry found has a real reason; 1 at least one does not (named, by language); 2
# REFUSED (examined nothing -- corpus/tests absent, or truly zero XFAIL banners anywhere, too suspicious to
# certify as a silent pass).
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
S4E="${S4E_HOME:-$(cd "$ROOT/.." && pwd)}"
CORPUS="${CORPUS:-$S4E/corpus/tests}"
. "$HERE/lib_gate.sh"
gate_require "$CORPUS" "the corpus/tests tree"
SUMMARY="$(python3 - "$CORPUS" <<'PY'
import io,os,re,sys
root=sys.argv[1]
SOURCE_EXTS={'.sno','.icn','.pl','.sc','.reb','.raku','.pas'}
BANNER=re.compile(r'-{3,}\s+(\d+)\s+([A-Za-z0-9_.-]+)\s+XFAIL\b')
PLACEHOLDER={'todo','tbd','unknown','n/a','na','-','?','xxx','fixme',''}
# ⭐ KEYED BY ENTRY NAME ALONE, NEVER BY (rank,entry) -- icon/ALL.xfail proved rank is NOT a shared key: its
# banner reads "#---- 1 procedure_scan_while_1 XFAIL" (LOCAL sequence number within that one-entry file)
# while the source corpus/tests/icon/ALL.icn banner for the same program reads rank 722 (the corpus-wide
# rank). A (rank,entry) key silently missed that real reason as a false gap. Entry names are verified unique
# per language (zero duplicates across all seven ALL.csv, checked live), so name alone is a safe, honest key
# -- rank is kept only as a display convenience for whichever numbering the SOURCE file uses.
def banners_in(path):
    out={}
    try: lines=io.open(path,encoding='utf-8',errors='replace').read().split('\n')
    except OSError: return out
    for l in lines:
        m=BANNER.search(l)
        if m: out[m.group(2)]=int(m.group(1))
    return out
def reasons_in(path):
    out={}
    try: lines=io.open(path,encoding='utf-8',errors='replace').read().split('\n')
    except OSError: return out
    cur=None; buf=[]
    for l in lines:
        m=BANNER.search(l)
        if m:
            if cur is not None: out[cur]='\n'.join(buf)
            cur=m.group(2); buf=[]
        elif cur is not None: buf.append(l)
    if cur is not None: out[cur]='\n'.join(buf)
    return out
langs=sorted(d for d in os.listdir(root) if os.path.isdir(os.path.join(root,d)))
total_x=0; total_gap=0; gaps=[]; orphan_notes=[]
for lang in langs:
    d=os.path.join(root,lang)
    xmap={}
    for f in os.listdir(d):
        base,ext=os.path.splitext(f)
        if base=='ALL' and ext in SOURCE_EXTS: xmap.update(banners_in(os.path.join(d,f)))
    reasons=reasons_in(os.path.join(d,'ALL.xfail'))
    lang_gap=0
    for entry,rank in sorted(xmap.items(),key=lambda kv:kv[1]):
        body=reasons.get(entry)
        stripped=' '.join((body or '').split())
        if body is None or stripped.lower() in PLACEHOLDER:
            lang_gap+=1; gaps.append((lang,rank,entry))
    orphans=sorted(set(reasons)-set(xmap))
    if orphans:
        orphan_notes.append(f"    NOTE [{lang}]: {len(orphans)} ALL.xfail block(s) with no matching source entry (stale?) -- e.g. entry={orphans[0]}")
    total_x+=len(xmap); total_gap+=lang_gap
    print(f"  {lang}: xfail={len(xmap)} reasoned={len(xmap)-lang_gap} gap={lang_gap}")
for note in orphan_notes: print(note)
for lang,rank,entry in gaps:
    print(f"    GAP [{lang}] rank={rank} entry={entry} -- no reason (or a placeholder) in ALL.xfail")
print(f"SUMMARY total_xfail={total_x} total_gap={total_gap}")
PY
)"
echo "$SUMMARY" | grep -v '^SUMMARY '
TOTAL_X="$(echo "$SUMMARY" | sed -n 's/^SUMMARY total_xfail=\([0-9]*\) total_gap=.*/\1/p')"
TOTAL_GAP="$(echo "$SUMMARY" | sed -n 's/^SUMMARY .*total_gap=\([0-9]*\)$/\1/p')"
gate_floor "${TOTAL_X:-0}" 1 "XFAIL-banner entries examined"
gate_verdict "${TOTAL_GAP:-0}" "XFAIL entries with no real reason"
