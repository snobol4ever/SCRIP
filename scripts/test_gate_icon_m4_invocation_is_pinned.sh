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
# ⛔⭐ CLAUSE 2 WAS RED ON PURPOSE UNTIL CEO-581, AND THIS GATE SAID SO RATHER THAN MEASURING AROUND IT. The
# row's minted DONE-WHEN asks for both clauses. Clause 1 was mine to make green; clause 2 was not, because no
# invocation of a compiled binary produces the string `<stem>.icn` -- and a gate asserting only the half it
# could turn green is a criterion A DELETION SATISFIES WHILE THE DEFECT STANDS (CEO-553: FALSE, not merely
# weak). The three options went to the ceo with the measurement bearing on each and this gate decided none.
#
# ⭐ CEO-581 RULED OPTION ONE, THE PER-MODE REF, and clause 2 is now green THROUGH THAT MECHANISM AND ONLY
# THROUGH IT: the expected text is rendered by util_apply_moderef.py -- the same shim the jcon runner calls
# and a shim over the same harness the master grades with -- so a green here means THE DECLARED MECHANISM
# WORKS, never that this gate re-implemented it in bash. THE REASON option one and not the other two, in the
# ceo's words: a single ref for a program printing &progname ASSERTS A FALSEHOOD, that one string is correct
# for two different invocations; a mask's earning test is that THE ORACLE'S OWN VALUE MOVES BETWEEN RUNS and
# here it moves between INVOCATIONS, which is determinism answering a different question; and a permanent red
# would make the board carry a lie, because a red must mean something is WRONG.
#
# ⛔⭐ CLAUSE 3 IS CEO-581'S MANDATORY GUARD, AND IT IS MEASURED HERE RATHER THAN ASSERTED IN PROSE: A PER-MODE
# REF IS EARNED BY AN ORACLE MEASUREMENT, NEVER BY A FAILING DIFF. This gate RUNS Arizona icont on a witness
# under both invocations every time it is invoked and requires the two answers to DIFFER and to take the two
# declared forms. A seat that cuts a second ref because m4 disagreed and it did not know why has committed the
# defect the guard exists to stop -- so the guard is wired to the oracle, not to a promise. With icont
# unreachable this gate REFUSES rc=2: an instrument that cannot measure refuses.
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
# ⛔ icont_bin() COMES FROM THE ONE AUTHORITY and never from a hand-assembled path or bare `icont` on PATH:
# CLAUDE.md's measured lesson is a seat that ran `command -v icont`, got nothing, and reported "no Icon oracle"
# about a tree that has one. Clause 3 cannot be run without it, and a missing oracle here is a REFUSAL.
. "$HERE/lib_oracle_flags.sh"
SCRIP="$ROOT/scrip"; RT="$ROOT/out"
for f in "$HARNESS" "$JCON"; do [ -f "$f" ] || { echo "REFUSED rc=2: $f is missing -- cannot measure"; exit 2; }; done
[ -x "$SCRIP" ] || { echo "REFUSED rc=2: no $SCRIP -- run make, then re-run"; exit 2; }
command -v gcc >/dev/null 2>&1 || { echo "REFUSED rc=2: no gcc -- cannot build a mode-4 witness"; exit 2; }
RED=0; GREEN=0
say_ok()   { echo "  ok    $*"; GREEN=$((GREEN+1)); }
say_fail() { echo "  FAIL  $*"; RED=$((RED+1)); }

echo "CLAUSE 1 -- the mode-4 invocation is a stable STATED name in both runners"
# (a) the master harness names the binary after its source and invokes it by bare relative name
if grep -q 'out_bin = (Path(bin_dir) if bin_dir else tmp_dir) / (Path(sno_path).stem or "t")' "$HARNESS" \
   && grep -q '"./" + out_bin.name if _same else str(out_bin)' "$HARNESS" \
   && grep -q 'where=entry.name, bin_dir=Path(td))' "$HARNESS"; then
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

# ⛔⭐⭐ AND THE SAME CLAIM, MEASURED THROUGH THE GRADING PATH ITSELF -- BECAUSE THE GREP ABOVE WENT GREEN FOR A
# WHOLE DAY WHILE THE BEHAVIOUR WAS WRONG (hq_V 2026-09-11). run_m4 spells the pinned form correctly; the
# caller one frame up (run_suite_entry -> run_all_modes, which opens a SECOND nested tempdir) defeated it, so
# every master entry in seven languages was still invoked by its absolute mktemp path and this gate could not
# see it. A SOURCE-LEVEL ASSERTION IS NOT A MEASUREMENT OF BEHAVIOUR. This arm builds a one-entry master suite
# whose ref says ./<stem> and grades it THROUGH corpus_suite_harness.py: it can only pass if the invocation the
# grader actually makes is the pinned one.
PW="$W/livepin"; mkdir -p "$PW"   # NOT $W/pinwitness: the clause-1 round trip above links a BINARY by that name into $W
printf '#%s 1 pinwitness\nprocedure main()\n   write("&progname: ",&progname)\nend\n' "-----------------" > "$PW/ALL.icn"
printf '#%s 1 pinwitness\n&progname: ./pinwitness\n' "-----------------" > "$PW/ALL.ref"
if timeout 120s python3 "$HARNESS" run --lang icon --modes m4 "$PW/ALL.icn" "$PW/ALL.ref" 2>&1 | grep -q 'm4_n=1 m4_pass=1'; then
  say_ok "LIVE: a master suite entry graded through the harness answers ./<stem> -- the pin holds where entries are actually graded, not merely where it is written"
else
  say_fail "LIVE: a master suite entry graded through the harness does NOT answer ./<stem> -- run_m4 spells the pin and its caller defeats it; this is the defect the grep above cannot see"
fi

echo "CLAUSE 2 -- the two entries that print &progname reproduce their refs in mode 4"
# ⛔ THE HONEST CLAUSE. It is red, and it is red for a reason no runner change can reach.
MASTER="$S4E/corpus/tests/icon"
JC="$S4E/corpus/packages/icon/jcon_tests"
# ⛔⭐ THE EXPECTED TEXT IS RENDERED BY THE SHARED SHIM, NEVER BY A sed IN THIS FILE. util_apply_moderef.py
# imports corpus_suite_harness.py's own reader, so this gate, the master harness and the jcon runner resolve a
# per-mode ref by ONE set of rules. A gate that applied the substitution itself would go green on its own
# private idea of what a .moderef means and say nothing about the mechanism that actually grades the fleet --
# the "lying test" shape, and one this lane paid for on 2026-09-11 when one question had three written answers.
per_mode_ref() {  # <ref> <entry> <mode> [sidecar]  -> prints the rendered path, or refuses rc=2
  local ref="$1" entry="$2" mode="$3" side="${4:-}"
  local out="$W/$entry.$mode.want"   # ⛔ its OWN `local`: bash expands every word of a `local` line BEFORE it assigns any of them, so $entry read here would be unbound under `set -u`
  if [ -n "$side" ]; then
    python3 "$HERE/util_apply_moderef.py" --sidecar "$side" "$ref" "$entry" "$mode" "$W/$entry.$mode.n" > "$out" || return 1
  else
    python3 "$HERE/util_apply_moderef.py" "$ref" "$entry" "$mode" "$W/$entry.$mode.n" > "$out" || return 1
  fi
  printf '%s\n' "$out"
}
check_m4_entry() {  # <label> <src> <ref> [sidecar]
  local label="$1" src="$2" ref="$3" side="${4:-}"
  [ -f "$src" ] && [ -f "$ref" ] || { echo "REFUSED rc=2: $label -- $src or $ref missing, cannot measure"; exit 2; }
  local stem; stem="$(basename "$src" .icn)"
  local want; want="$(per_mode_ref "$ref" "$stem" m4 "$side")" || { echo "REFUSED rc=2: $label -- the per-mode ref could not be rendered (the refusal above says why); a gate that grades against a declaration it could not read is measuring nothing"; exit 2; }
  local nsub; nsub="$(cat "$W/$stem.m4.n" 2>/dev/null || echo 0)"
  ref="$want"
  local d; d="$(mktemp -d)"; cp "$src" "$d/"
  ( cd "$d" && SNO_LIB="$S4E/corpus/include" "$SCRIP" --compile "$stem.icn" > q.s 2>/dev/null ) || { say_fail "$label: scrip --compile failed"; rm -rf "$d"; return; }
  gcc -c "$d/q.s" -o "$d/q.o" 2>/dev/null && gcc "$d/q.o" -L "$RT" -lscrip_rt -lm -Wl,-rpath,"$RT" -o "$d/$stem" 2>/dev/null || { say_fail "$label: build failed"; rm -rf "$d"; return; }
  local out; out="$( cd "$d" && ./"$stem" </dev/null 2>&1 )"
  if [ "$out" = "$(cat "$ref")" ]; then
    say_ok "$label: mode 4 reproduces its ref ($nsub line(s) taken from the CEO-581 per-mode declaration)"
  else
    local n; n="$(diff <(printf '%s\n' "$out") "$ref" | grep -c '^<')"
    say_fail "$label: mode 4 differs from its ref on $n line(s) -- $(diff <(printf '%s\n' "$out") "$ref" | grep '^[<>]' | head -2 | tr '\n' ' ')"
  fi
  rm -rf "$d"
}
EX="$W/e924"; mkdir -p "$EX"
if MASTER_EXT=.icn MASTER_DIR="$MASTER" bash -c "source '$HERE/lib_master_extract.sh'; master_extract_name procedure_every_alt_replace_4 '$EX/procedure_every_alt_replace_4.icn' '$EX/procedure_every_alt_replace_4.ref'" >/dev/null 2>&1; then
  check_m4_entry "IcnM entry 924 procedure_every_alt_replace_4" "$EX/procedure_every_alt_replace_4.icn" "$EX/procedure_every_alt_replace_4.ref" "$MASTER/ALL.moderef"
else
  echo "REFUSED rc=2: could not extract entry 924 from the Icon master -- cannot measure"; exit 2
fi
check_m4_entry "jcon kwds" "$JC/kwds.icn" "$JC/kwds.std"

# ⛔⭐ CLAUSE 3 -- CEO-581'S EARNING TEST, RUN AGAINST THE ORACLE EVERY TIME THIS GATE IS INVOKED.
echo "CLAUSE 3 -- the per-mode pair is EARNED BY AN ORACLE MEASUREMENT (CEO-581's mandatory guard)"
ICONT="$(icont_bin)" || { echo "REFUSED rc=2: Arizona icont is unreachable, so the earning test cannot be run -- and a per-mode ref whose oracle measurement cannot be re-taken is exactly what CEO-581's guard refuses. An instrument that cannot measure REFUSES."; exit 2; }
OD="$W/oracle"; mkdir -p "$OD"
printf 'procedure main()\n   write("   &progname: ",&progname)\nend\n' > "$OD/pn.icn"
O_SRC="$( cd "$OD" && "$(dirname "$ICONT")/icon" pn.icn 2>&1 )"
( cd "$OD" && "$ICONT" -s pn.icn >/dev/null 2>&1 )
O_BIN="$( cd "$OD" && ./pn 2>&1 )"
if [ "$O_SRC" = "$O_BIN" ]; then
  say_fail "the oracle answered the SAME string under both invocations ($O_SRC) -- the pair is then NOT earned and every .moderef row resting on this measurement must come out"
elif [ "$O_SRC" = "   &progname: pn.icn" ] && [ "$O_BIN" = "   &progname: ./pn" ]; then
  say_ok "oracle under both invocations: source form '$O_SRC' · binary form '$O_BIN' -- they DIFFER, and in the two forms the declarations state"
else
  say_fail "the oracle answers forms this gate does not recognise (source '$O_SRC' · binary '$O_BIN') -- the declarations name <stem>.icn and ./<stem>, so either the oracle changed or the declaration did; re-measure before trusting either"
fi
# ⛔ AND THE RECEIPT COLUMN IS NOT DECORATION: every shipped row must carry one, and the reader must refuse a
# row that does not. Proven by construction on a scratch declaration, so this arm cannot pass by nobody looking.
printf 'x\n' > "$W/r.ref"; printf 'x\tm4\tx\ty\t\n' > "$W/r.moderef"
if python3 "$HERE/util_apply_moderef.py" "$W/r.ref" x m4 >/dev/null 2>&1; then
  say_fail "an unreceipted per-mode ref was HONOURED -- CEO-581's guard is not wired: the mechanism would accept a pair cut from a failing diff"
else
  say_ok "an unreceipted per-mode ref is REFUSED rc=2 (a pair nobody can audit is not a measurement)"
fi
for f in "$MASTER/ALL.moderef" "$JC/kwds.moderef"; do
  [ -f "$f" ] || { say_fail "$(basename "$f") is missing -- clause 2 above cannot be resting on a declaration that is not there"; continue; }
  if awk -F'\t' '!/^#/ && NF>1 && (NF!=5 || $5 !~ /[^ ]/) {bad=1} END{exit bad?1:0}' "$f"; then
    say_ok "$(basename "$f"): every row carries its five fields and a non-empty oracle receipt"
  else
    say_fail "$(basename "$f"): a row is malformed or carries no receipt"
  fi
done

# ⛔⭐ CLAUSE 4 -- FAIL-ONCE ON THE MECHANISM ITSELF. Clause 2 is green; this asks the question that makes that
# green worth anything: WOULD IT BE RED WITHOUT THE DECLARATION? Same program, same binary, same shipped ref,
# the declaration simply not read. A criterion that passes with and without the thing it certifies certifies
# nothing -- and on this row in particular the alternative reading (option three, a permanent red) is exactly
# what the unrendered ref produces, so the two states must be visibly different.
echo "CLAUSE 4 -- fail-once: without the per-mode declaration the same mode-4 run is RED"
d="$(mktemp -d)"; cp "$JC/kwds.icn" "$d/"
if ( cd "$d" && SNO_LIB="$S4E/corpus/include" "$SCRIP" --compile kwds.icn > q.s 2>/dev/null ) \
   && gcc -c "$d/q.s" -o "$d/q.o" 2>/dev/null && gcc "$d/q.o" -L "$RT" -lscrip_rt -lm -Wl,-rpath,"$RT" -o "$d/kwds" 2>/dev/null; then
  raw="$( cd "$d" && ./kwds </dev/null 2>&1 )"
  if [ "$raw" = "$(cat "$JC/kwds.std")" ]; then
    say_fail "kwds mode 4 matches the UNRENDERED shipped ref -- then the per-mode ref is doing nothing and clause 2's green does not belong to it"
  else
    say_ok "kwds mode 4 differs from the unrendered shipped ref on $(diff <(printf '%s\n' "$raw") "$JC/kwds.std" | grep -c '^<') line(s), and matches it once the declaration is applied"
  fi
else
  echo "REFUSED rc=2: could not build kwds for the fail-once arm -- cannot measure"; exit 2
fi
rm -rf "$d"

echo "----"
echo "green: $GREEN   red: $RED"
if [ "$RED" -eq 0 ]; then echo "✅ the mode-4 invocation is pinned and both &progname entries reproduce their refs"; exit 0; fi
echo "⛔ CLAUSE 1 is the pin and CLAUSE 2 is the ref. A red above is the state of the row, not a broken gate:"
echo "   &progname is argv[0] verbatim (oracle-measured), m3 is handed a SOURCE and answers <stem>.icn,"
echo "   m4 IS the program and answers ./<stem>, and no invocation of a binary produces the string <stem>.icn."
echo "   Pinning made the answer STATED and therefore gradable; making it EQUAL is a ruling, not a runner change."
exit 1
