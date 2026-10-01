#!/usr/bin/env bash
# test_gate_pas_an_unassigned_header_file_binds_to_stdio_only_under_iso_mode.sh -- an extra (non-input/output) program-parameter
# text file that is never assign()'d binds to real stdout on rewrite / stdin on reset, exactly as fpc -Miso does, ISO 7185 6.10
# being implementation-defined on this point (row pascal-every-suite-to-100-under-nonet-ceo-1266, landing 2026-10-01).
#
# MEASURED 2026-10-01 by hq_pascal, both modes. test_tisobuf2.pas ({$mode iso}, program test(input,output,testfile)) rewrites
# testfile unassigned, writes 'Hello world', closes, assigns it to a DIFFERENT name ('TESTFILE.txt') that was never created, and
# resets -- fpc -Miso prints 'Hello world' to real stdout then halts with Runtime error 2 (rc=2, file not found) on the reset.
# Parent SCRIP instead bound the unassigned rewrite to a DISK FILE LITERALLY NAMED AFTER THE IDENTIFIER ('testfile' in cwd),
# so 'Hello world' never reached stdout (rc=1 silent, from a FAILDESCR dead end on reset, no diagnostic).
#
# ⛔⭐ THE CURE IS GATED ON {$mode iso} AND MUST STAY THAT WAY -- MEASURED REGRESSION, FOUND AND FIXED IN THE SAME LANDING: an
# unconditional cure (bind every unassigned header file to stdio) BLOCKED test_pascal_p4_selfhost.sh. corpus/packages/pascal/p4/
# comp.pas -- program pascalcompiler(input,output,prr); -- carries NO mode directive and does a bare `rewrite(prr)` with no prior
# assign, relying on the CLASSIC Pascal-P4 default (an unassigned external file binds to a disk file named after itself) that the
# self-host harness (test_pascal_p4_selfhost.sh) depends on to capture generation-1 p-code at $W/prr. comp.pas is never graded
# against fpc, so it has no claim on fpc's specific -Miso choice. Arm (b) below is that regression's own witness, run WITHOUT
# {$mode iso}, and must keep creating a real same-named disk file exactly as before this cure.
#
# ARMS: (a) hdrstdio -- {$mode iso}, graded live against fpc -Miso: stdout AND exit code, both modes; also asserts no stray disk
# file named after the identifier appears (the old, wrong binding). (b) hdrnoiso -- no mode directive, SCRIP-only (not fpc-graded:
# P4 self-host's own criterion is gen1-pcode==gen2-pcode, never an fpc comparison): rewrite with no assign still creates a real
# disk file named after the identifier, byte-identical to what was written, both modes.
# FAILS on the parent f6177b6e2 in arm (a) (rc=1 not 2, stdout empty not 'Hello world', a stray 'hdrstdio_extra' file on disk).
# FAIL_ONCE=1 corrupts arm (a)'s wanted stdout.
"$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/util_require_fresh.sh" --gate "$(basename "${BASH_SOURCE[0]}" .sh)" || exit $?
set -uo pipefail
G="$(basename "${BASH_SOURCE[0]}" .sh)"
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP_BIN:-$ROOT/scrip}"; [ -x "$SCRIP" ] || { echo "⛔ GATE REFUSE(2) [$G]: no scrip at $SCRIP"; exit 2; }
RT_DIR="${RT_DIR:-$ROOT/out}"; [ -f "$RT_DIR/libscrip_rt.so" ] || { echo "⛔ GATE REFUSE(2) [$G]: no runtime at $RT_DIR/libscrip_rt.so"; exit 2; }
FPC="${FPC_BIN:-/usr/bin/fpc}"; [ -x "$FPC" ] || { echo "⛔ GATE REFUSE(2) [$G]: no fpc at $FPC -- arm (a)'s expectation is CUT FROM THE ORACLE"; exit 2; }
RC=0; N=0
run() { local m="$1" p="$2" w="$3"
  rm -f "$w"/extra "$w"/hdrstdio_extra.name.that.is.never.created 2>/dev/null
  if [ "$m" = m3 ]; then ( cd "$w" && timeout 20s env LD_LIBRARY_PATH="$RT_DIR" "$SCRIP" --run "$p.pas" </dev/null >"$w/o" 2>"$w/e" ); return $?; fi
  ( cd "$w" && timeout 20s "$SCRIP" --compile -o "$p.s" "$p.pas" </dev/null >/dev/null 2>&1 && cc -m64 -no-pie "$p.s" -o "$p.m4" -L"$RT_DIR" -lscrip_rt -lm -Wl,-rpath,"$RT_DIR" >/dev/null 2>&1 ) || { echo "  ⛔ $p m4: did not compile and link"; return 125; }
  ( cd "$w" && timeout 20s "./$p.m4" </dev/null >"$w/o" 2>"$w/e" ); return $?; }

# --- arm (a): hdrstdio, {$mode iso}, graded live against fpc -Miso ---
Ta=$(mktemp -d) || exit 2
trap 'rm -rf ${Ta:+"$Ta"} ${Tb:+"$Tb"}' EXIT; trap 'rm -rf ${Ta:+"$Ta"} ${Tb:+"$Tb"}; exit 143' TERM; trap 'rm -rf ${Ta:+"$Ta"} ${Tb:+"$Tb"}; exit 130' INT
cat > "$Ta/hdrstdio.pas" <<'PAS'
{$mode iso}
program hdrstdio(input, output, extra);
var
  extra : text;
begin
  rewrite(extra);
  writeln(extra, 'to stdout');
  close(extra);
  assign(extra, 'hdrstdio_extra.name.that.is.never.created');
  reset(extra);
  writeln('unreachable')
end.
PAS
( cd "$Ta" && "$FPC" -Miso -v0 -ohdrstdio.fpc hdrstdio.pas >/dev/null 2>&1 ) || { echo "⛔ GATE REFUSE(2) [$G]: fpc -Miso would not compile hdrstdio"; rm -rf "$Ta"; exit 2; }
( cd "$Ta" && timeout 20s ./hdrstdio.fpc </dev/null >hdrstdio.want 2>/dev/null ); frc=$?
[ -n "${FAIL_ONCE:-}" ] && echo "corrupted by FAIL_ONCE" >> "$Ta/hdrstdio.want"
for m in m3 m4; do
  run "$m" hdrstdio "$Ta"; rc=$?; N=$((N + 1))
  stray=""; [ -e "$Ta/extra" ] && stray="$Ta/extra"
  if [ "$rc" = "$frc" ] && cmp -s "$Ta/hdrstdio.want" "$Ta/o" && [ -z "$stray" ]; then
    echo "  hdrstdio $m: rc=$rc, stdout byte-identical to fpc -Miso, no stray disk file"
  else
    echo "  ⛔ hdrstdio $m FAILED: rc=$rc (fpc $frc) stray=[${stray:-none}]"; diff "$Ta/hdrstdio.want" "$Ta/o" | head -6 | sed 's/^/      /'; echo "      err : $(head -1 "$Ta/e" 2>/dev/null | cut -c1-140)"; RC=1
  fi
done
rm -rf "$Ta"

# --- arm (b): hdrnoiso, no mode directive, SCRIP-only regression guard for P4 self-host's rewrite(prr)-with-no-assign shape ---
Tb=$(mktemp -d) || exit 2
cat > "$Tb/hdrnoiso.pas" <<'PAS'
program hdrnoiso(input, output, extra);
var
  extra : text;
begin
  rewrite(extra);
  writeln(extra, 'legacy disk binding');
  close(extra);
  writeln('done')
end.
PAS
want_line='legacy disk binding'
for m in m3 m4; do
  run "$m" hdrnoiso "$Tb"; rc=$?; N=$((N + 1))
  if [ "$rc" = 0 ] && [ -f "$Tb/extra" ] && [ "$(cat "$Tb/extra")" = "$want_line" ] && [ "$(cat "$Tb/o")" = "done" ]; then
    echo "  hdrnoiso $m: rc=0, disk file 'extra' created with the written line, stdout 'done' (the P4 comp.pas shape, unaffected)"
  else
    echo "  ⛔ hdrnoiso $m FAILED: rc=$rc, disk-file-content=[$(cat "$Tb/extra" 2>/dev/null)] stdout=[$(cat "$Tb/o" 2>/dev/null)]"; RC=1
  fi
done
rm -rf "$Tb"

if [ "$RC" = 0 ]; then echo "GATE PASS [$G]: an unassigned header file binds to stdio under {\$mode iso} (as fpc -Miso does) and keeps the classic disk-file default otherwise (as P4 self-host needs), $N arms"
else echo "GATE FAIL(1) [$G]: examined 2 programs in 2 modes"; fi
echo "    tree: SCRIP=$(git -C "$ROOT" rev-parse --short HEAD 2>/dev/null)$(git -C "$ROOT" diff --quiet 2>/dev/null || echo -DIRTY)" \
     " oracle: $FPC $($FPC -iV 2>/dev/null)  measured $(date -u +%Y-%m-%dT%H:%MZ)"
exit $RC
