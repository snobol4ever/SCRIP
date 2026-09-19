#!/usr/bin/env bash
# test_gate_gc_a_callee_saved_register_at_an_allocating_return_is_named_not_assumed.sh
# -- F1 RUNG 3, THE REGISTERS (cto 2026-09-18, the ceo's row (1) of CEO-845; row
# gc-frame-placement-proof-...-and-callee-saved-registers-are-mapped; design
# .github/ARCH-GC-COMPILE-TIME-FRAME-MAPS.md sections 6.3 and 6.5 and section 7 F1; law RULES.md FACT RULE
# THE COLLECTOR GUESSES NOTHING, CEO-812).
#
# ⛔ WHY THIS GATE IS NOT THE ROW AS IT WAS HANDED TO ME, AND THE DISAGREEMENT IS THE POINT. The row says "the map
# must carry the pointer-bearing callee-saved registers -- gc_frame_map_t has one reserved quad and nothing reads
# it", which is section 7 F1's wording. Section 6.5 of the SAME frozen page says the opposite in as many words:
# "NO REGISTER MASK: the callee-saved registers are made visible at the poll itself, not in a map", with a spill
# record of tagged cells below RSP. I wrote both, on the same day, and neither was held by a check -- which is the
# fourth time in two days that a fact in this area turned out to be living in a paragraph. So this gate does not
# pick a side by argument. IT MEASURES THE ONE THING THAT DECIDES IT, and the measurement is unambiguous:
#   A REGISTER'S CONTENT AT AN ALLOCATING RETURN IS A PROPERTY OF THE SITE, NOT OF THE GRAPH.
# Two facts, both read off the emitted code by util_gc_callee_saved_census.py and both printed by arm 6:
#   (a) SITE-DEPENDENCE. There are graphs in which the same register is live across one allocating return and dead
#       across another. Section 6.3's map is ONE PER GRAPH. A per-graph mask would therefore have to be the union
#       over that graph's sites, and at the site where the register is dead the union tells the collector to visit
#       a register holding whatever the last box left in it. That is a GUESS, in the one place Lon's rule forbids
#       guessing, and it is worse than no mask at all -- a missing root loses an object, a spurious root reads an
#       arbitrary word as a pointer.
#   (b) PASS-THROUGH. At most allocating returns most callee-saved registers were never written by the graph at
#       all: the value belongs to an ANCESTOR frame, still sitting in the register because nothing here touched it.
#       No map of THIS graph can name it, because the fact is not this graph's.
# THE RULING THIS GATE HOLDS: the register facts do NOT go in the map, section 6.5 stands, section 7 F1's clause
# is the stale one, and gc_frame_map_t.reserved stays ZERO with no reader -- arm 6 reds if a register mask is ever
# smuggled into it. What the registers DO need is a tag at the site, which is the spill record, and that is the
# polls row's build, not this one's.
#
# ⭐ AND WHAT THE MEASUREMENT FOUND ON THE WAY, which is worth more than the ruling. Section 6.5 asserts that r13
# holds the subject base and that r14/r15 are the subject CURSOR and LENGTH and therefore raw. The register half
# is confirmed -- r14 and r15 are seeded 32-bit (mov r14d, dword ptr [...]; cmp eax, r15d), and a 32-bit define
# zero-extends, so a 64-bit pointer cannot survive in either. But the ROUTE in that paragraph is wrong: the
# pointer does not reach an allocating return through the seeding `mov r13, rax` after rt_match_enter -- that one
# is dead across every allocating call in the population. It arrives through a RELOAD out of the frame cell the
# match stashed it in, and through the `push r13  # outer_Σ` that saves the CALLER's subject. A whitelist that
# accepted "it came from a tagged cell" as proof of rawness would have blessed every one of those, because a
# cell's DESCR tag is the truth about the CELL and it does not travel to the register: the collector can relocate
# the block, fix up the cell, and leave the register pointing at the old address. The census therefore refuses to
# call a copy raw, and arm 5 holds the copy count at its measured ceiling so the class cannot grow in silence.
#
# ⛔ THE COPY CEILING HAS BEEN RE-BASED TWICE AND NEITHER MOVE WAS THE CLASS GROWING -- A COUNT IS NOT COMPARABLE
# ACROSS A CHANGE OF ITS CRITERION, and both changes were the population getting honest.
#   83 -> 108 (cto 2026-09-18, section 6.5c): the seven hermetic witnesses carry no DEFERRED pattern, so this gate
#     read heap=0 and had NEVER GRADED ONE INSTANCE of the class it exists to hold; the residual it ratcheted was
#     entirely CELL copies. A deferred-pattern witness joined the population and reads heap=5 -- and reads the same
#     heap=5 on ORIGIN, so it was a two-day population gap and not something a cure made. Arm 2b now reds on heap=0.
#   108 -> 133 (cto 2026-09-18, section 6.5d): the ALLOCATING SET itself was undercounted. Its derivation recorded
#     `call` edges only, and the hand-written asm allocators in rtx_alloc.s reach the carve by a TAIL JUMP, so they
#     and the 99 functions reaching the heap only through them read NON-ALLOCATING. Widening the derivation to
#     inter-function tail jumps took this gate's population from 528 allocating call sites to 582 -- the SAME
#     witnesses, 54 sites that were never graded before. Every one of the 25 new copy readings is at a site that was
#     always allocating and was never counted.
#
# ARMS (all blocking). (1) SELFTEST: the census's own arms, including its planted ones -- an instrument not shown
# to discriminate has measured nothing. (2) POPULATION: one witness per frontend, all seven, compiled to mode-4
# asm; the census reads a non-zero allocating-call population out of them and prints it. (3) UNCLASSIFIED = 0:
# every owned register live across an allocating return has a defining form the census can name. (4) UNDERSTOOD:
# zero mnemonics the census's def/use model does not know -- an unknown instruction is silently assumed to leave
# the six registers alone, which is the same shape as the hole this whole row is about. (5) COPY RATCHET: the
# named residual, held at its measured ceiling. (6) THE RULING: the map's fourth quad is zero in every emitted map
# in BOTH media and nothing in src/ reads gc_frame_map_t.reserved, and the site-dependence count that justifies
# that is printed beside it. (7) PLANTED: an .s carrying a register defined by an un-whitelisted form, live across
# an allocating call, must make the census RED end to end -- not only inside its own selftest.
"$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/util_require_fresh.sh" --gate "$(basename "${BASH_SOURCE[0]}" .sh)" || exit $?
set -uo pipefail
G="$(basename "${BASH_SOURCE[0]}" .sh)"
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
S4E="${S4E_HOME:-$(cd "$ROOT/.." && pwd)}"
SCRIP="${SCRIP_BIN:-$ROOT/scrip}"; [ -x "$SCRIP" ] || { echo "⛔ GATE REFUSE(2) [$G]: no scrip at $SCRIP"; exit 2; }
CENSUS="$HERE/util_gc_callee_saved_census.py"; [ -f "$CENSUS" ] || { echo "⛔ GATE REFUSE(2) [$G]: $CENSUS absent"; exit 2; }
T=$(mktemp -d) || exit 2; trap 'rm -rf "$T"' EXIT
RC=0
export SNO_LIB="${SNO_LIB:-$S4E/corpus/include}"
COPY_CEILING=${COPY_CEILING:-133}

echo "  HOLDS: what sits in a callee-saved register at an allocating return is NAMED from the emitted code, not assumed from a paragraph -- and it is a property of the SITE, not of the graph, so it does not go in the per-graph map. gc_frame_map_t.reserved stays zero with no reader; the registers get a tag at the poll (section 6.5's spill record), which is the polls row's build."

python3 "$CENSUS" --selftest > "$T/self.txt" 2>&1
if [ $? -eq 0 ] && grep -q 'arms green' "$T/self.txt"; then echo "  arm 1 PASS: census $(grep -o 'SELFTEST .*' "$T/self.txt")"
else echo "  arm 1 RED: the census's own selftest is not green -- an instrument not shown to discriminate has measured nothing"; sed 's/^/      /' "$T/self.txt" | head -8; RC=1; fi

cat > "$T/w.icn" <<'EOF'
procedure f(a, b)
   return a + b;
end
procedure g(n)
   local s, i;
   s := 0;
   every i := 1 to n do s := s + f(i, 2);
   return s;
end
procedure main()
   write(g(5));
end
EOF
cat > "$T/w.sno" <<'EOF'
        DEFINE('F(X)')                              :(F_END)
F       F = X + 1                                   :(RETURN)
F_END
        S = 'hello world'
        S 'hello' = 'goodbye'                       :F(DONE)
        OUTPUT = S
        OUTPUT = F(41)
DONE
END
EOF
cat > "$T/w.pl" <<'EOF'
app([], L, L).
app([H|T], L, [H|R]) :- app(T, L, R).
:- app([1,2],[3],X), write(X), nl.
EOF
cat > "$T/w.sc" <<'EOF'
function Double(n) {
    Double = n + n; return;
}
OUTPUT = Double(21);
EOF
cat > "$T/w.raku" <<'EOF'
sub add($a, $b) {
    return $a + $b;
}
sub main() {
    my $s = 0;
    for 1..5 { $s = add($s, 2); }
    say($s);
}
EOF
cat > "$T/w.pas" <<'EOF'
program sumloop;
var
  i, s: integer;
function add(a, b: integer): integer;
begin
  add := a + b
end;
begin
  s := 0;
  for i := 1 to 5 do
    s := add(s, 2);
  writeln(s)
end.
EOF
cat > "$T/w.reb" <<'EOF'
function main()
  OUTPUT := 3 + 4
end
EOF
cat > "$T/w.defer.sno" <<'EOF'
        S = 'AAAAABBBBBCCCCC'
        N = 3
        C = 'AB'
        S LEN(*N) . X1                              :F(D1)
D1      S ANY(*C) . X2                              :F(D2)
D2      S SPAN(*C) . X3                             :F(D3)
D3      S BREAK(*C) . X4                            :F(D4)
D4      S TAB(*N) . X5                              :F(D5)
D5
END
EOF
asm=""
for w in w.icn w.sno w.pl w.sc w.raku w.pas w.reb w.defer.sno; do
    if timeout 120 "$SCRIP" --compile -o "$T/$w.s" "$T/$w" </dev/null >/dev/null 2>&1 && [ -s "$T/$w.s" ]; then asm="$asm $T/$w.s"
    else echo "  arm 2 RED ($w: mode 4 did not emit asm -- this frontend could not be censused)"; RC=1; fi
done
nasm=$(echo $asm | wc -w)
python3 "$CENSUS" $asm > "$T/c.txt" 2>&1
sum=$(grep '^SUMMARY ' "$T/c.txt" | tail -1)
val(){ echo "$sum" | tr ' ' '\n' | awk -F= -v k="$1" '$1==k{print $2}'; }
sites=$(val sites); unc=$(val unclassified); unk=$(val unknown_mnemonics); cop=$(val copies); dep=$(val site_dependent_pairs); gr=$(val graphs); raw=$(val raw); heap=$(val heap)
if [ "$nasm" -eq 8 ] && [ -n "$sites" ] && [ "$sites" -ge 100 ]; then
    echo "  arm 2 PASS: 7 of 7 frontends plus the DEFERRED-PATTERN witness censused ($nasm files), $sites allocating call site(s) over $gr graph(s) -- owned readings: provably_not_a_pointer=$raw a_copy_of_something_else=$cop heap=$heap"
else echo "  arm 2 RED: censused $nasm of 8 witnesses, sites=${sites:-none} (a census over an empty population reports zero violations while measuring nothing)"; RC=1; fi
if [ -n "$heap" ] && [ "$heap" -gt 0 ]; then echo "  arm 2b PASS: $heap owned reading(s) in this population ARE a collected-heap pointer in a callee-saved register at an allocating return -- the class this gate exists to hold is EXERCISED, not merely declared"
else echo "  arm 2b RED: heap=${heap:-0} -- the population carries no collected-heap pointer in any callee-saved register at any allocating return, so arms 3 and 5 grade a class that never occurs in it. The seven hermetic witnesses read heap=0 for exactly this reason and the gap was invisible until a DEFERRED pattern was censused (section 6.5c)."; RC=1; fi
if [ "${unc:-1}" = 0 ]; then echo "  arm 3 PASS: 0 unclassified -- every owned register live across an allocating return has a defining form the census can name"
else echo "  arm 3 RED: $unc register reading(s) live across an allocating return whose content the census cannot name:"; grep 'UNCLASSIFIED' "$T/c.txt" | head -6 | sed 's/^/    /'; RC=1; fi
if [ "${unk:-1}" = 0 ]; then echo "  arm 4 PASS: 0 mnemonics outside the census's def/use model"
else echo "  arm 4 RED: $unk instruction(s) the census does not model -- it would silently assume they leave the six callee-saved registers alone, which is the same shape as the hole this row is about:"; grep 'UNDERSTOOD=NO' "$T/c.txt" | sed 's/^/    /'; RC=1; fi
if [ -n "$cop" ] && [ "$cop" -le "$COPY_CEILING" ]; then echo "  arm 5 PASS: $cop owned reading(s) are a copy of something else, at or under the measured ceiling $COPY_CEILING -- the NAMED residual the spill record of section 6.5 must carry, held so it cannot grow in silence"
else echo "  arm 5 RED: the copy residual is ${cop:-?} against a ceiling of $COPY_CEILING -- a register holding an untagged copy across a collection is a root the collector cannot fix up"; RC=1; fi

badq=0; qpop=""
for w in w.icn w.sno w.pl w.sc w.raku w.pas w.reb w.defer.sno; do
    [ -s "$T/$w.s" ] || continue
    n=$(grep -c '^\.Lgcmap_[^ ]*:$' "$T/$w.s")
    nz=$(awk 'BEGIN{c=-1} /^\.Lgcmap_[^ ]*:$/{c=0;next} c>=0 && /^[ \t]*\.quad/{c++; if(c==4){ if($2 != "0") bad++ ; c=-1}} END{print bad+0}' "$T/$w.s")
    qpop="$qpop $w:$n"
    [ "${nz:-0}" -gt 0 ] && { echo "  arm 6 RED ($w: $nz emitted map(s) carry a NON-ZERO fourth quad -- a register mask has been smuggled into the per-graph map, which the site-dependence measurement says cannot be right)"; badq=1; }
done
readers=$(grep -rn -e '->reserved' -e '\.reserved' "$ROOT/src" 2>/dev/null | grep -vc '^$')
if [ "$badq" = 0 ] && [ "$readers" -eq 0 ]; then
    echo "  arm 6 PASS: the map's fourth quad is zero in every emitted map (maps per witness --$qpop) and gc_frame_map_t.reserved has 0 readers in src/. THE MEASUREMENT BEHIND THE RULING: $dep (graph, register) pair(s) over $gr graphs are live across one allocating return of a graph and dead across another, so a per-graph mask would have to be a union that guesses at one of its own sites; and most callee-saved registers at most sites were never written by the graph at all, so the value is an ancestor's and no map of this graph can name it."
else [ "$badq" = 0 ] && echo "  arm 6 RED: gc_frame_map_t.reserved has $readers reader(s) in src/ -- the register facts are being moved into the per-graph map, which section 6.5 and the site-dependence measurement both say cannot be done without guessing"; RC=1; fi

cat > "$T/plant.s" <<'EOF'
 .text
p_α:
 push r12
 call rt_gcheap_alloc
 mov r12, rax
 call rt_gcheap_alloc
 mov rdi, r12
 pop r12
 ret
EOF
python3 "$CENSUS" "$T/plant.s" > "$T/plant.txt" 2>&1; prc=$?
if [ "$prc" -ne 0 ] && grep -q 'UNCLASSIFIED' "$T/plant.txt"; then echo "  arm 7 PASS: a planted register defined by an un-whitelisted form, live across an allocating return, makes the census RED end to end (rc=$prc) -- the arms above are not an inert seam"
else echo "  arm 7 RED: the planted unclassifiable definition read GREEN (rc=$prc) -- the census has stopped discriminating and arms 3 to 5 are measuring nothing"; RC=1; fi

if [ "$RC" = 0 ]; then echo "GATE PASS(0) [$G]: what a callee-saved register holds at an allocating return is read off the emitted code, nothing is unclassified, and the per-graph map carries no register mask (examined 8 arms)"
else echo "GATE FAIL(1) [$G]: a callee-saved register at an allocating return is unnamed, unmodelled, or the map has taken on a register mask it cannot honestly carry (examined 8 arms)"; fi
echo "    tree: SCRIP=$(git -C "$ROOT" rev-parse --short HEAD 2>/dev/null)$(git -C "$ROOT" diff --quiet 2>/dev/null || echo -DIRTY)  measured $(date -u +%Y-%m-%dT%H:%MZ)"
exit $RC
