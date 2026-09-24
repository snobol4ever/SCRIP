#!/usr/bin/env bash
# test_gate_gc_conservative_auditor_reports_and_cannot_ship.sh -- RUNG 3 OF ARCH-GC § 9, MEASURED.
#
# ⛔⭐ THE GRANT AND ITS EXPIRY.  Lon 2026-09-21, in-chat to the ceo, verbatim: "So if you need a conservative
# scan temporarily to get your diagnostics done, then go ahead.  But ensure in the end all is exact scanning."
# A GRANT WITH AN EXPIRY AND NO REMOVAL MECHANISM IS A PERMANENT GRANT, so the expiry is built, not promised.
# This gate is the four structural conditions of that grant, each as an arm that can fail:
#   (a) THE SHIPPED BUILD CARRIES ZERO CONSERVATIVE SCAN -- measured on the .so, not read off the source.
#   (b) AND THE CENSUS IN (a) IS NOT BLIND -- the same census over the knob-on build MUST see it.  Fail-once in
#       both directions on one tree: the auditor exists only under -DSCRIP_GC_AUDIT_B, so the shipped library is
#       silent because THE CODE IS NOT IN IT, which is a stronger statement than a runtime flag defaulting off.
#   (c) PASS A ALWAYS DECIDES -- source census that pass B calls nothing that marks, forwards, frees or protects,
#       AND a behavioural arm: one binary, one library, auditor OFF against ON, stdout and collection count
#       IDENTICAL.  An auditor that changed a collection would change one of those two.
#   (d) THE AUDITOR CLAIMS ITS OWN EVENT -- it prints `audited=1` per collection and `findings=0` out loud, so
#       its SILENCE is a statement rather than an absence, and a run where it never applied REFUSES rc=2 rather
#       than passing.  The cto's shift plant taught this class twice in one day.
#   (e) EVERY HOLDER IS CURED OR DECLARED -- scripts/gc_audit_b_declared.txt.  An undeclared holder is a RED, so
#       the count falls only by curing roots or by writing down a measurement.
#   (f) THE EXPIRY IS STRUCTURAL -- the day no OPEN row remains and a sweep finds nothing, THIS ARM REDS AND SAYS
#       DELETE.  A disabled auditor is a conservative scan wearing an if.
#
# ⛔ COST, MEASURED (cfo 2026-09-21, load 2.7-5.3, SCRIP_HEAP_MB=1).  The auditor's own build is a separate RT_TAG
# and is built HERE, directly as out/libscrip_rt-<tag>.so, which is a target in its own right: the canonical
# out/libscrip_rt.so symlink is NEVER touched, so a shard running in parallel cannot pick up this library by
# accident.  COLD (no cached tag, e.g. after `make pristine`) that build is 2m25s; CACHED it is 0.2s.  The run
# arms are 4 witnesses chosen for having FEW collections.  The auditor itself costs 185x wall on a 36-collection
# witness (0.01s -> 1.85s) and 99.97% of that is one region: libscrip_rt.so's 89 MB writable PT_LOAD, which is
# .bss tables (dat_types 18 MB, zf 16 MB, ze and zm 8 MB each), NOT the program's heap.  So pass B's cost is a
# property of the LIBRARY and not of the workload, and SCRIP_GC_AUDIT_B_POPS narrows the territory with the
# narrowing PRINTED in every summary line.
"$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/util_require_fresh.sh" --gate "$(basename "${BASH_SOURCE[0]}" .sh)" || exit $?
set -u
cd "$(dirname "$0")/.." || exit 2
ROOT="$PWD"; G="gc_conservative_auditor_reports_and_cannot_ship"
unset SCRIP_HEAP_MB; export SCRIP_HEAP_KB="${SCRIP_HEAP_KB:-128}"
checks=0; fails=0
ck() { checks=$((checks+1)); if [ "$1" = ok ]; then echo "  ok   $2"; else fails=$((fails+1)); echo "  FAIL $2"; fi; }
refuse() { echo "⛔ GATE REFUSED(2) [$G]: $1"; exit 2; }
SRC="src/runtime/rt/gc_audit_b.c"; HDR="src/runtime/rt/gc_audit_b.h"; DECL="scripts/gc_audit_b_declared.txt"
for f in "$SRC" "$HDR" "$DECL" src/runtime/rt/gc_heap.c; do [ -f "$ROOT/$f" ] || refuse "$f is missing -- it is part of this rung's deliverable"; done
command -v nm >/dev/null 2>&1 || refuse "no nm -- this gate resolves a holder OFFSET to a SYMBOL and cannot key a declaration on an offset"
[ -f "$ROOT/out/libscrip_rt.so" ] || refuse "no out/libscrip_rt.so -- there is no shipped library to census"
echo "ARENA SCRIP_HEAP_KB=$SCRIP_HEAP_KB (the shipped window -- CEO-931/934; the CEO-1146 sweep moved this off MB=1, which named 1024 KB and ran the collector ZERO times on four of six witnesses. The exasperation knob is SCRIP_GC_STRESS, never the arena -- 33rd batch clause 1)"
T="$(mktemp -d)"; trap 'rm -rf "$T"' EXIT

echo "-- (a) THE SHIPPED BUILD CARRIES ZERO CONSERVATIVE SCAN"
SHIP="$ROOT/out/libscrip_rt.so"
s_sym=$(nm -D "$SHIP" 2>/dev/null | grep -c "gc_audit_b" || true)
s_str=$(strings "$SHIP" 2>/dev/null | grep -c "GC-AUDIT-B" || true)
ck $([ "$s_sym" = 0 ] && echo ok || echo no) "(a1) shipped out/libscrip_rt.so exports NO gc_audit_b symbol (saw $s_sym)"
ck $([ "$s_str" = 0 ] && echo ok || echo no) "(a2) shipped out/libscrip_rt.so carries NO [GC-AUDIT-B] string (saw $s_str)"
d_sym=$(nm -D "$SHIP" 2>/dev/null | grep -c "gc_audit_b_collect" || true)
ck $([ "$d_sym" = 0 ] && echo ok || echo no) "(a3) the auditor's entry point gc_audit_b_collect is ABSENT from the shipped library (saw $d_sym)"

echo "-- (b) AND THAT CENSUS IS NOT BLIND: the knob-on build MUST trip every arm of (a)"
AUD_OPT="-O0 -g -fno-strict-aliasing -fwrapv -fno-omit-frame-pointer -DSCRIP_GC_AUDIT_B=1"
AUD_TAG=$(make -s RT_OPT="$AUD_OPT" buildinfo 2>/dev/null | awk '/^RT_TAG/{print $3}')
[ -n "$AUD_TAG" ] || refuse "could not read RT_TAG for the auditor configuration out of \`make buildinfo\` -- without it this gate cannot name the library it must census"
AUD_SO="$ROOT/out/libscrip_rt-$AUD_TAG.so"
[ "$AUD_SO" != "$SHIP" ] || refuse "the auditor configuration hashed to the SHIPPED RT_TAG -- the two builds would share objects and (a) and (b) would be the same census"
if [ ! -f "$AUD_SO" ] || [ "$ROOT/$SRC" -nt "$AUD_SO" ] || [ "$ROOT/src/runtime/rt/gc_heap.c" -nt "$AUD_SO" ]; then
    echo "     building the auditor configuration (RT_TAG=$AUD_TAG); COLD this is ~2m25s, cached ~0.2s -- the canonical symlink is not touched"
    make RT_OPT="$AUD_OPT" "out/libscrip_rt-$AUD_TAG.so" > "$T/build.log" 2>&1 || { tail -20 "$T/build.log"; refuse "the knob-on build FAILED -- arm (b) cannot be measured and a green (a) would then mean nothing"; }
fi
[ -f "$AUD_SO" ] || refuse "out/libscrip_rt-$AUD_TAG.so did not appear after the build"
LNK=$(readlink "$SHIP"); ck $([ "$LNK" != "$(basename "$AUD_SO")" ] && echo ok || echo no) "(b0) out/libscrip_rt.so still points at the SHIPPED configuration ($LNK), not at the auditor build"
a_sym=$(nm -D "$AUD_SO" 2>/dev/null | grep -c "gc_audit_b_collect" || true)
a_str=$(strings "$AUD_SO" 2>/dev/null | grep -c "GC-AUDIT-B" || true)
ck $([ "$a_sym" -ge 1 ] && echo ok || echo no) "(b1) the knob-on library DOES export gc_audit_b_collect (saw $a_sym) -- so (a3)'s zero is an absence and not a blind census"
ck $([ "$a_str" -ge 1 ] && echo ok || echo no) "(b2) the knob-on library DOES carry [GC-AUDIT-B] strings (saw $a_str) -- so (a2)'s zero is an absence and not a blind census"

echo "-- (c) PASS A ALWAYS DECIDES"
bad=$(grep -nE "gc_mark_blk|rt_gc_visit_|gc_wl_push|gc_slot_reg|mprotect|memmove|[^_a-z]free\(|->flags *=|->fwd *=" "$ROOT/$SRC" | grep -v "HBF_MARK)" || true)
ck $([ -z "$bad" ] && echo ok || echo no) "(c1) $SRC calls nothing that marks, forwards, frees or protects${bad:+ -- SAW: $bad}"
mut=$(grep -c "rt_hblk_t \*)" "$ROOT/$HDR" || true); cst=$(grep -c "const rt_hblk_t \*" "$ROOT/$HDR" || true)
ck $([ "$cst" -ge 2 ] && echo ok || echo no) "(c2) the view hands pass B only CONST block pointers (saw $cst const forms) -- it has no mutable handle to mark with"

mkdir -p "$T/lib"; ln -sf "$AUD_SO" "$T/lib/libscrip_rt.so"
WITS="scripts/gc_witnesses/hb_coexpr_genp_scan.icn scripts/gc_witnesses/hb_cv_spine_plain_redo.icn scripts/gc_witnesses/hb_file_name_unrooted.icn scripts/gc_witnesses/hb_blob_span_defer.sno"
for w in $WITS; do [ -f "$ROOT/$w" ] || refuse "$w is missing -- this gate's declared witness set is not on the tree"; done
run_aud() { LD_LIBRARY_PATH="$T/lib" SCRIP_GC_DISPLACE=1 SCRIP_GC_STRESS="${2:-3}" env ${3:+SCRIP_GC_AUDIT_B=$3} timeout 300s "$ROOT/scrip" "$ROOT/$1" 2>"$T/e" ; }
W1=scripts/gc_witnesses/hb_coexpr_genp_scan.icn
o_off=$(run_aud "$W1" 3 ""); c_off=$(grep -c "^\[GC-DISPLACE\]" "$T/e" || true); cp "$T/e" "$T/e.off"
o_on=$(run_aud "$W1" 3 1); c_on=$(grep -c "^\[GC-DISPLACE\]" "$T/e" || true); cp "$T/e" "$T/e.on"
n_aud=$(grep -c "audited=1" "$T/e.on" || true)
ck $([ "$o_off" = "$o_on" ] && echo ok || echo no) "(c3) auditor OFF and ON on ONE binary and ONE library print BYTE-IDENTICAL stdout"
ck $([ "$c_off" = "$c_on" ] && [ "$c_off" -gt 0 ] && echo ok || echo no) "(c4) and the SAME number of collections ($c_off off against $c_on on) -- pass B did not change the schedule it reports on"
ck $([ "$(grep -c 'audited=1' "$T/e.off" || true)" = 0 ] && echo ok || echo no) "(c5) with the knob off the knob-on LIBRARY audits nothing -- the env switch is honoured at the point of use, not cached"

echo "-- (d) THE AUDITOR CLAIMS ITS OWN EVENT"
[ "$n_aud" -gt 0 ] || refuse "the auditor produced NO 'audited=1' line on $W1 -- it declined, and a gate that grades a run the instrument never touched is the false-green this rung exists to remove"
ck $([ "$n_aud" = "$c_on" ] && echo ok || echo no) "(d1) EVERY collection was audited: $n_aud audited=1 lines against $c_on collections"
n_f0=$(grep -c 'audited=1 findings=' "$T/e.on" || true)
ck $([ "$n_f0" = "$n_aud" ] && echo ok || echo no) "(d2) every audited collection PRINTS its findings count, zero included ($n_f0 summary lines against $n_aud audits) -- silence is a statement, not an absence"
run_aud scripts/gc_witnesses/hb_file_name_unrooted.icn 3 1 > "$T/fh.out"; cp "$T/e" "$T/e.fh"
# ⛔⭐ (d3) WAS "the auditor NAMES g_fh" AND THAT ARM DIED OF ITS OWN SUCCESS ON 2026-09-21: the cfo ROOTED the FH
# table (drv_gc_roots in src/driver/driver_globals.c, registered in gc_heap.c beside the other eleven), the holder
# went away, and the detector proof went with it.  A PROOF KEYED ON A NAMED OPEN DEFECT IS A COUNTDOWN -- so this
# arm no longer names a symbol at all.  It asks the sweep for any holder that the LEDGER still lists OPEN, resolved
# through nm and not through dladdr (dladdr names only EXPORTED symbols, and every holder left today is a file-local
# static it CANNOT name -- the same coarse-symbol trap as CFO-129).  Cure one holder and the arm re-points itself;
# cure the last one and arm (f1) is the arm that fires, which is where the retirement belongs.
d3=$(python3 - "$T/e.fh" "$AUD_SO" "$ROOT/$DECL" <<'PYD3'
import subprocess, sys, re
errf, so, decl = sys.argv[1], sys.argv[2], sys.argv[3]
syms = []
for ln in subprocess.run(['nm','-S','--defined-only',so],capture_output=True,text=True).stdout.splitlines():
    f = ln.split()
    if len(f) < 4: continue
    try: a = int(f[0],16); sz = int(f[1],16)
    except ValueError: continue
    syms.append((a,sz,f[3]))
state = {}
for ln in open(decl):
    ln = ln.strip()
    if not ln or ln.startswith('#'): continue
    f = ln.split(None,2)
    if len(f) >= 2: state[f[0]] = f[1]
named = set()
for ln in open(errf, errors='replace'):
    m = re.search(r'CANDIDATE-LOST-ROOT at=\S+ in=(\S+)', ln)
    if not m: continue
    mo = re.match(r'^.*\+0x([0-9a-f]+)/(.*)$', m.group(1))
    if not mo: named.add(m.group(1)); continue
    off = int(mo.group(1),16)
    hit = [x for x in syms if x[0] <= off < x[0]+x[1]]
    named.add(hit[0][2] if hit else mo.group(2))
op = sorted(n for n in named if state.get(n) == 'OPEN')
print("%d %s %s" % (len(op), ','.join(op) or '-', 'g_fh' if 'g_fh' in named else 'no_g_fh'))
PYD3
)
d3n=${d3%% *}; d3rest=${d3#* }; d3names=${d3rest%% *}; d3fh=${d3rest##* }
ck $([ "${d3n:-0}" -ge 1 ] && echo ok || echo no) "(d3) the detector FIRES on hb_file_name_unrooted.icn and the sweep names ${d3n:-0} holder(s) the ledger still lists OPEN, resolved by nm to a SYMBOL: $d3names"
ck $([ "$d3fh" = no_g_fh ] && echo ok || echo no) "(d4a) and g_fh -- this arm's named proof until the cfo rooted the FH table -- is GONE from that same sweep (saw $d3fh)"
ck $(diff -q "$T/fh.out" "$ROOT/scripts/gc_witnesses/hb_file_name_unrooted.ref" >/dev/null 2>&1 && echo ok || echo no) "(d4b) and the witness that was g_fh's RED witness now ANSWERS ITS ORACLE -- the cure and the detector agreeing is what retires a holder, not either one alone"

echo "-- (e) EVERY HOLDER IS CURED OR DECLARED"
: > "$T/all.err"
for w in $WITS; do run_aud "$w" 3 1 > /dev/null; cat "$T/e" >> "$T/all.err"; done
DECLF="$ROOT/$DECL"
if [ "${FAIL_ONCE:-0}" = 1 ]; then printf '# FAIL_ONCE: the ledger is emptied so arm (e1) must RED on every holder pass B names\n' > "$T/decl.empty"; DECLF="$T/decl.empty"; echo "     FAIL_ONCE=1 -- the declared ledger is EMPTY for this run; (e1) is expected to RED"; fi
python3 - "$T/all.err" "$AUD_SO" "$DECLF" > "$T/holders.txt" <<'PY'
import subprocess, sys, re
errf, so, decl = sys.argv[1], sys.argv[2], sys.argv[3]
syms = []
for ln in subprocess.run(['nm','-S','--defined-only',so],capture_output=True,text=True).stdout.splitlines():
    f = ln.split()
    if len(f) < 4: continue
    try: a = int(f[0],16); sz = int(f[1],16)
    except ValueError: continue
    syms.append((a,sz,f[3]))
seen = {}
for ln in open(errf, errors='replace'):
    m = re.search(r'CANDIDATE-LOST-ROOT at=\S+ in=(\S+)', ln)
    if not m: continue
    h = m.group(1); name = None
    mo = re.match(r'^.*\+0x([0-9a-f]+)/(.*)$', h)
    if mo:
        off = int(mo.group(1),16); tail = mo.group(2)
        hit = [s for s in syms if s[0] <= off < s[0]+s[1]]
        name = hit[0][2] if hit else (tail if tail != 'writable-PT_LOAD' else 'UNRESOLVED+0x%x' % off)
    else:
        name = h
    # a function-local static is named by nm with a per-translation-unit serial (lnv.3, _excl.1) that moves whenever
    # its file gains or loses a static, so the ledger is keyed by the base name (cto 2026-09-24, the seventeen-red row)
    name = re.sub(r'\.\d+$', '', name)
    seen[name] = seen.get(name,0)+1
declared = {}
for ln in open(decl):
    ln = ln.strip()
    if not ln or ln.startswith('#'): continue
    f = ln.split(None,2)
    if len(f) >= 2: declared[f[0]] = f[1]
for k in sorted(seen): print("HOLDER %s %d %s" % (k, seen[k], declared.get(k,'UNDECLARED')))
print("OPENROWS %d" % sum(1 for v in declared.values() if v == 'OPEN'))
print("TOTALHOLDERS %d" % len(seen))
PY
sed 's/^/     /' "$T/holders.txt"
undecl=$(grep "^HOLDER" "$T/holders.txt" | grep -c "UNDECLARED" || true)
nh=$(awk '/^TOTALHOLDERS/{print $2}' "$T/holders.txt"); nopen=$(awk '/^OPENROWS/{print $2}' "$T/holders.txt")
ck $([ "$undecl" = 0 ] && echo ok || echo no) "(e1) every holder pass B named over the declared witness set is CURED or DECLARED in $DECL ($undecl undeclared)"
ck $([ "${nh:-0}" -gt 0 ] && echo ok || echo no) "(e2) the sweep resolved at least one holder SYMBOL from an offset (saw ${nh:-0}) -- a declaration keyed on a symbol survives the next build, an offset does not"

echo "-- (f) THE EXPIRY IS STRUCTURAL"
if [ "${nopen:-0}" = 0 ] && [ "${nh:-0}" = 0 ]; then
    ck no "(f1) ⛔⭐ RETIRE THE AUDITOR NOW: no OPEN row remains in $DECL and the sweep found NO holder.  That silence IS the proof Lon asked for -- 'ensure in the end all is exact scanning' -- so DELETE src/runtime/rt/gc_audit_b.[ch], the #ifdef block and call in gc_heap.c, the Makefile line, this gate and $DECL.  A disabled auditor is a conservative scan wearing an if."
else
    ck ok "(f1) the expiry has not come: ${nopen:-0} OPEN row(s) and ${nh:-0} holder(s) named -- this arm REDS AND SAYS DELETE the day both read 0"
fi
echo "[$G] checks=$checks fails=$fails  holders=${nh:-0} open_rows=${nopen:-0} audit_tag=$AUD_TAG"
[ "$fails" = 0 ] || exit 1
exit 0
