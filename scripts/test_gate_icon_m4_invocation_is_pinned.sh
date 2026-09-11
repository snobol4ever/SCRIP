#!/usr/bin/env bash
# scripts/test_gate_icon_m4_invocation_is_pinned.sh -- THE MODE-4 INVOCATION IS A STABLE STATED NAME, AND
# THE ENTRIES THAT PRINT IT REPRODUCE THEIR REFS.  DONE-WHEN for row
# icon-progname-class-one-ref-cannot-be-right-for-both-modes-master-and-jcon (CEO-569).
# AUTHORS: Lon Jones Cherryholmes · Claude Opus 5   DATE: 2026-09-11
#
# ⭐⭐ THE MEASUREMENT THAT DEFINES THIS GATE, taken on the ORACLE before a line of it was written
# (Arizona icont/iconx 9.5.25a, one program, three invocations):
#     ./pn                      -> &progname: ./pn
#     /abs/path/to/pn           -> &progname: /abs/path/to/pn
#     iconx pn                  -> &progname: pn
# ⛔ &progname IS argv[0], VERBATIM. It is not the source name, not the program's identity, and not
# something a compiler can get right or wrong -- it is whatever the caller typed. Two consequences run
# through everything below, and they pull in OPPOSITE directions:
#   (1) A MODE-4 BINARY INVOKED BY A MKTEMP PATH IS UNGRADABLE, not red. Its answer changes every run, so
#       no .ref can ever be cut from it and no criterion can ever say YES. THAT is the defect CEO-569
#       names, and pinning the invocation to a stable STATED name is the whole cure for it. Clause 1.
#   (2) A PINNED NAME STILL IS NOT THE M3 NAME. m3 is handed a SOURCE and answers `<stem>.icn`; m4 IS the
#       program and answers `./<stem>`. Both are correct -- each is exactly what the oracle prints for that
#       invocation -- so ONE LITERAL REF LINE CANNOT BE RIGHT FOR BOTH MODES (hq_B, CEO-557). Clause 2.
#
# ⛔⭐ CLAUSE 2 IS RED TODAY AND THIS GATE SAYS SO RATHER THAN MEASURING AROUND IT. The row's minted
# DONE-WHEN asks for both clauses. I can make clause 1 green and I cannot make clause 2 green by the ruled
# mechanism, because no invocation of a compiled binary produces the string `<stem>.icn`. Writing a gate
# that asserts only clause 1 would be a criterion A DELETION SATISFIES WHILE THE DEFECT STANDS, which
# CEO-553 ruled is FALSE and not merely weak. So both clauses are asserted, the gate reports WHICH half is
# green, and the row stays open until the ceo rules on clause 2. The three options, with the measurement
# that bears on each, are in the seat's telegram and its rung -- this gate decides none of them.
#
# ⛔ IT GRADES ON ITS OWN FIXTURES, NEVER ON A BOARD (CEO-547 part 2, ONE RUNNER ONE BOARD): clause 1 is a
# source-level assertion over the two runners plus a live round trip on a witness the gate compiles itself,
# and clause 2 extracts the two named programs and runs them directly. No master suite run, no package
# board, no row written.
#
# EXIT: 0 both clauses green · 1 a clause is red (the report names which) · 2 REFUSED -- cannot measure.
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"; cd "$ROOT"
S4E="${S4E_HOME:-$(cd "$ROOT/.." && pwd)}"
HARNESS="$HERE/corpus_suite_harness.py"; JCON="$HERE/test_icon_jcon_suite.sh"
SCRIP="$ROOT/scrip"; RT="$ROOT/out"
for f in "$HARNESS" "$JCON"; do [ -f "$f" ] || { echo "REFUSED rc=2: $f is missing -- cannot measure"; exit 2; }; done
[ -x "$SCRIP" ] || { echo "REFUSED rc=2: no $SCRIP -- run make, then re-run"; exit 2; }
command -v gcc >/dev/null 2>&1 || { echo "REFUSED rc=2: no gcc -- cannot build a mode-4 witness"; exit 2; }
RED=0; GREEN=0
say_ok()   { echo "  ok    $*"; GREEN=$((GREEN+1)); }
say_fail() { echo "  FAIL  $*"; RED=$((RED+1)); }

echo "CLAUSE 1 -- the mode-4 invocation is a stable STATED name in both runners"
# (a) the master harness names the binary after its source and invokes it by bare relative name
if grep -q 'out_bin = tmp_dir / (Path(sno_path).stem or "t")' "$HARNESS" \
   && grep -q '"./" + out_bin.name if _same else str(out_bin)' "$HARNESS"; then
  say_ok "master harness: binary is <stem>, invoked as ./<stem> (coo 413a0e0a6)"
else
  say_fail "master harness: the m4 binary name or its argv is no longer the pinned ./<stem> form"
fi
# (b) the jcon runner does the same, and NOTHING there builds to a mktemp _bin name any more
if grep -q 'local s="\$WORK/\$name.s" o="\$WORK/\$name.o" bin="\$rundir/\$name"' "$JCON" \
   && grep -q 'timeout "\$TIMEOUT" "\./\$name"' "$JCON"; then
  say_ok "jcon runner: binary is \$rundir/\$name, invoked as ./\$name"
else
  say_fail "jcon runner: the m4 binary is not built into the rundir under its bare stem, or not invoked by bare name"
fi
# ⛔ CODE LINES ONLY. The first cut of this arm grepped the whole file and matched THIS GATE'S OWN
# EXPLANATORY COMMENT in the runner -- a false red manufactured by the prose that documents the cure.
# A source assertion that cannot tell code from commentary will fire on the day someone explains the fix.
if sed 's/#.*//' "$JCON" | grep -q '\${name}_bin'; then
  say_fail "jcon runner still builds a \${name}_bin under the mktemp root -- that is the unpinned name itself"
else
  say_ok "jcon runner: no mktemp-rooted _bin name survives in code (comments may still name it, and should)"
fi
# (c) THE LIVE ROUND TRIP, because grep asserts a spelling and not a behaviour: compile a witness that
#     prints &progname, run it the way each runner does, and require the answer to be the STATED name.
W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
printf 'procedure main()\n   write("&progname: ", &progname)\nend\n' > "$W/pinwitness.icn"
( cd "$W" && SNO_LIB="$S4E/corpus/include" "$SCRIP" --compile pinwitness.icn > p.s 2>/dev/null ) || { echo "REFUSED rc=2: scrip --compile failed on the gate's own witness -- cannot measure"; exit 2; }
gcc -c "$W/p.s" -o "$W/p.o" 2>/dev/null || { echo "REFUSED rc=2: gcc -c failed on the gate's own witness -- cannot measure"; exit 2; }
gcc "$W/p.o" -L "$RT" -lscrip_rt -lm -Wl,-rpath,"$RT" -o "$W/pinwitness" 2>/dev/null || { echo "REFUSED rc=2: link failed on the gate's own witness -- cannot measure"; exit 2; }
GOT="$( cd "$W" && ./pinwitness </dev/null 2>&1 )"
if [ "$GOT" = "&progname: ./pinwitness" ]; then
  say_ok "live round trip: a mode-4 binary invoked as ./<stem> answers ./<stem>"
else
  say_fail "live round trip: expected '&progname: ./pinwitness', got '$GOT'"
fi
# (d) FAIL-ONCE, MECHANISED: the SAME binary under the old mktemp-absolute invocation must NOT answer a
#     stated name. If this ever passes, the pin has stopped being observable and clause 1 is vacuous.
GOTABS="$( cd "$W" && "$W/pinwitness" </dev/null 2>&1 )"
if [ "$GOTABS" = "&progname: $W/pinwitness" ]; then
  say_ok "fail-once arm: the unpinned absolute invocation still answers the mktemp path, so the pin is observable"
else
  say_fail "fail-once arm: the absolute invocation answered '$GOTABS' -- this gate can no longer tell pinned from unpinned"
fi

echo "CLAUSE 2 -- the two entries that print &progname reproduce their refs in mode 4"
# ⛔ THE HONEST CLAUSE. It is red, and it is red for a reason no runner change can reach.
MASTER="$S4E/corpus/tests/icon"
JC="$S4E/corpus/packages/icon/jcon_tests"
check_m4_entry() {  # <label> <src> <ref> <refline-grep>
  local label="$1" src="$2" ref="$3"
  [ -f "$src" ] && [ -f "$ref" ] || { echo "REFUSED rc=2: $label -- $src or $ref missing, cannot measure"; exit 2; }
  local d; d="$(mktemp -d)"; cp "$src" "$d/"; local stem; stem="$(basename "$src" .icn)"
  ( cd "$d" && SNO_LIB="$S4E/corpus/include" "$SCRIP" --compile "$stem.icn" > q.s 2>/dev/null ) || { say_fail "$label: scrip --compile failed"; rm -rf "$d"; return; }
  gcc -c "$d/q.s" -o "$d/q.o" 2>/dev/null && gcc "$d/q.o" -L "$RT" -lscrip_rt -lm -Wl,-rpath,"$RT" -o "$d/$stem" 2>/dev/null || { say_fail "$label: build failed"; rm -rf "$d"; return; }
  local out; out="$( cd "$d" && ./"$stem" </dev/null 2>&1 )"
  if [ "$out" = "$(cat "$ref")" ]; then
    say_ok "$label: mode 4 reproduces its ref"
  else
    local n; n="$(diff <(printf '%s\n' "$out") "$ref" | grep -c '^<')"
    say_fail "$label: mode 4 differs from its ref on $n line(s) -- $(diff <(printf '%s\n' "$out") "$ref" | grep '^[<>]' | head -2 | tr '\n' ' ')"
  fi
  rm -rf "$d"
}
EX="$W/e924"; mkdir -p "$EX"
if MASTER_EXT=.icn MASTER_DIR="$MASTER" bash -c "source '$HERE/lib_master_extract.sh'; master_extract_name procedure_every_alt_replace_4 '$EX/procedure_every_alt_replace_4.icn' '$EX/procedure_every_alt_replace_4.ref'" >/dev/null 2>&1; then
  check_m4_entry "IcnM entry 924 procedure_every_alt_replace_4" "$EX/procedure_every_alt_replace_4.icn" "$EX/procedure_every_alt_replace_4.ref"
else
  echo "REFUSED rc=2: could not extract entry 924 from the Icon master -- cannot measure"; exit 2
fi
check_m4_entry "jcon kwds" "$JC/kwds.icn" "$JC/kwds.std"

echo "----"
echo "green: $GREEN   red: $RED"
if [ "$RED" -eq 0 ]; then echo "✅ the mode-4 invocation is pinned and both &progname entries reproduce their refs"; exit 0; fi
echo "⛔ CLAUSE 1 is the pin and CLAUSE 2 is the ref. A red above is the state of the row, not a broken gate:"
echo "   &progname is argv[0] verbatim (oracle-measured), m3 is handed a SOURCE and answers <stem>.icn,"
echo "   m4 IS the program and answers ./<stem>, and no invocation of a binary produces the string <stem>.icn."
echo "   Pinning made the answer STATED and therefore gradable; making it EQUAL is a ruling, not a runner change."
exit 1
