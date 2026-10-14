#!/usr/bin/env bash
# test_gate_gc_a_prolog_retained_frame_is_reached_by_its_call_sites_typed_frame_link.sh
#
# WHAT THIS GATE HOLDS (hq_collector 2026-10-09, ARCH-GC section 13 amendment RULED by the cto the same day on hq_zetas'
# measurement, row gc-one-stack-all-descriptors-...-lon-2026-09-30 ORDER (1)): a frame a Prolog call box RETAINS for
# backtracking lies between chain frames, off the return-PC chain, and until this landing only the marker scan visited it
# (the level-3 check's offchain-swept: 627 frames in 5 Prolog gc_witnesses, 26,700 in the witness below). The call site's
# callgen.act +0 holds the retained callee's frame base on the arms a pinned caller takes (bb_call_proc_staged.cpp's
# block arm and gamma landing, bb_call_value.cpp's pl_proto arm), so frame_layout.c types that word GC_LAY_PTR_FRAME and
# the walker follows it when NON-ZERO: the target's map from the act+8 beta by the code-range lookup, the target inside
# the segment and below the linking frame, and the target's own link word landing on a site of the linking graph --
# otherwise a refusal BY NAME, never a skip. No frame word is added and no shape moves; only the map's vocabulary grows.
#
# ARMS: (1) the witness (q/2 holds a heap term across a pending member/2 choice while its caller churns, then backtracks
# into it), mode 3 and mode 4, at SCRIP_GC_STRESS=1 and 3 under SCRIP_GC_CHAIN_CHECK=3: swipl's answer, offchain-swept=0,
# frame-links > 0, bad-link=0, the visitor differ=0 refused=0; (2) every Prolog gc_witness, both modes, stress 1:
# offchain-swept=0 and bad-link=0; (3) PLANTED: SCRIP_GC_PLANT_FRAME_LINK=1 displaces a followed link by 16 bytes and the
# walker REFUSES it by name ("is not a site of the frame that links it") and the collection dies on the chain's refusal (the chain is the only walk, ARCH-GC 13.9 ONE: a refusal is a fatal, so
# the run prints no summary; the same run without the plant is whole); (4) the witness's --dump-zeta
# types callgen.act +0 PTR_FRAME.
# ICON (hq_collector 2026-10-11, the cto's H-form condition met by hq_zetas' 50f4604e6): the callgen act +8 word of an Icon generator call is zero or H, the callee's retained generator frame
# (GC_LAY_PTR_FRAME_H); the walker takes the resume cell at H+32, requires a generator graph, derives the base as H minus the map's frame_bytes, and validates it like a Prolog link.
# ARMS: (5) hb_coexpr_parked and hb_coexpr_genp_scan, both modes, stress 1: offchain-swept=0, bad-link=0, frame-links > 0; (6) PLANTED: SCRIP_GC_PLANT_FRAME_LINK=1 displaces the followed H word
# and the walker refuses it by name. The release half: bb_bound.cpp's unmark arms zero the slots below the restored rsp.
# Red on SCRIP 7fd180c09 (before the landing): arm (1) reads offchain-swept=26700, arm (2) 627 over 5 witnesses.
set -u
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
"$ROOT/scripts/util_require_fresh.sh" --gate "$(basename "${BASH_SOURCE[0]}" .sh)" || exit $?
SCRIP="$ROOT/scrip"
G="test_gate_gc_a_prolog_retained_frame_is_reached_by_its_call_sites_typed_frame_link"
refuse() { echo "⛔ GATE REFUSED(2) [$G]: $1"; exit 2; }
[ -x "$SCRIP" ] || refuse "no scrip binary at $SCRIP -- build before grading"
[ -f "$ROOT/out/libscrip_rt.so" ] || refuse "no runtime library at $ROOT/out/libscrip_rt.so -- build before grading"
W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
checks=0; fails=0
ck() { checks=$((checks+1)); if [ "$1" = ok ]; then echo "  ok   $2"; else fails=$((fails+1)); echo "  FAIL $2"; fi; }
num() { printf '%s\n' "$1" | sed -n "s/.* $2=\([0-9]*\).*/\1/p"; }
cat > "$W/w.pl" <<'EOF'
:- initialization(main).
mk(N, T) :- length(L, N), fill(L, N), T = t(L, N).
fill([], _).
fill([H|T], N) :- number_codes(N, Cs), atom_codes(H, [0'k|Cs]), N1 is N + 1, fill(T, N1).
q(X, T) :- mk(5, T0), member(X, [1,2,3,4,5,6]), T = T0.
churn(0) :- !.
churn(N) :- mk(8, _), N1 is N - 1, churn(N1).
r(S) :- q(X, T), churn(6), X >= 5, T = t(L, _), length(L, Len), S is X * 100 + Len.
loop(0, A, A) :- !.
loop(N, A0, A) :- r(S), A1 is A0 + S, N1 is N - 1, loop(N1, A1, A).
main :- loop(4, 0, A), write(A), nl, q(_, t(L, _)), write(L), nl, halt.
EOF
printf '2020\n[k5,k6,k7,k8,k9]\n' > "$W/w.ref"
run_env() { env -u SCRIP_HEAP_MB -u SCRIP_HEAP_CAP_KB -u SCRIP_HEAP_MAX_MB SCRIP_HEAP_KB=64 SCRIP_GC_CHAIN_CHECK=3 "$@"; }
build4() {
  local src="$1" b="$2"
  ( cd "$W" && timeout 300 "$SCRIP" --compile -o "$b.s" "$src" > /dev/null 2> "$b.c4" < /dev/null ) || refuse "mode 4 could not compile $src: $(head -c 200 "$W/$b.c4")"
  gcc "$W/$b.s" -L"$ROOT/out" -lscrip_rt -lm -lpthread -Wl,-rpath,"$ROOT/out" -o "$W/$b.x" 2> "$W/$b.ld4" || refuse "mode 4 link failed for $src: $(head -c 200 "$W/$b.ld4")"
}
build4 "$W/w.pl" w
for st in 1 3; do
  for m in 3 4; do
    if [ "$m" = 3 ]; then ( cd "$W" && run_env SCRIP_GC_STRESS=$st timeout 300 "$SCRIP" w.pl > o 2> e < /dev/null ); else ( cd "$W" && run_env SCRIP_GC_STRESS=$st timeout 300 ./w.x > o 2> e < /dev/null ); fi
    s="$(grep -a '^\[CHAIN-WALK\] SUMMARY ' "$W/e" | tail -1)"
    oc="$(num "$s" offchain-swept)"; fl="$(num "$s" frame-links)"; bl="$(num "$s" bad-link)"; di="$(num "$s" differ)"; rf="$(num "$s" refused)"
    if cmp -s "$W/o" "$W/w.ref" && [ "$oc" = 0 ] && [ "${fl:-0}" -gt 0 ] && [ "$bl" = 0 ] && [ "$di" = 0 ] && [ "$rf" = 0 ]; then
      ck ok "(1) mode $m stress $st: swipl's answer, offchain-swept=0, $fl frame link(s) followed, bad-link=0, visitor differ=0 refused=0"
    else
      ck no "(1) mode $m stress $st: answer $(head -c 40 "$W/o" | tr '\n' ' ')offchain-swept=${oc:-?} frame-links=${fl:-?} bad-link=${bl:-?} differ=${di:-?} refused=${rf:-?} -- a retained Prolog frame is reached by the marker scan alone"
    fi
  done
done
for f in "$ROOT"/scripts/gc_witnesses/*.pl; do
  b="$(basename "$f" .pl)"; i=/dev/null; [ -f "${f%.pl}.in" ] && i="${f%.pl}.in"
  build4 "$f" "$b"
  for m in 3 4; do
    if [ "$m" = 3 ]; then ( cd "$W" && run_env SCRIP_GC_STRESS=1 timeout 300 "$SCRIP" "$f" > /dev/null 2> e < "$i" ); else ( cd "$W" && run_env SCRIP_GC_STRESS=1 timeout 300 "./$b.x" > /dev/null 2> e < "$i" ); fi
    s="$(grep -a '^\[CHAIN-WALK\] SUMMARY ' "$W/e" | tail -1)"
    oc="$(num "$s" offchain-swept)"; bl="$(num "$s" bad-link)"
    if [ -n "$s" ] && [ "$oc" = 0 ] && [ "$bl" = 0 ]; then ck ok "(2) $b mode $m: offchain-swept=0 bad-link=0 ($(num "$s" frame-links) link(s) followed)"
    else ck no "(2) $b mode $m: offchain-swept=${oc:-?} bad-link=${bl:-?} -- a Prolog frame is off the chain and off every typed link"; fi
  done
done
planted() { ( cd "$W" && run_env SCRIP_GC_STRESS=1 ${1:+SCRIP_GC_PLANT_FRAME_LINK=1} timeout 300 "$SCRIP" "$2" > /dev/null 2> ep < /dev/null ); }
planted 1 w.pl; prc=$?
if [ "$prc" != 0 ] && grep -aq 'FRAME-LINK REFUSED: the target.s return word is not a site of the frame that links it' "$W/ep" && grep -aq 'REFUSED: the chain over the maps stops short (bad-link)' "$W/ep"; then
  ck ok "(3) PLANTED: a link displaced by 16 bytes is refused by name and the collection dies on it (rc $prc), never followed"
else ck no "(3) PLANTED: rc=$prc and no named refusal -- a link that does not name a frame was followed or skipped"; fi
planted "" w.pl; urc=$?
if [ "$urc" = 0 ] && ! grep -aq 'FRAME-LINK REFUSED' "$W/ep"; then ck ok "(3) the same run without the plant is whole (rc 0, no refusal), so the planted arm can fail"
else ck no "(3) the same run without the plant refuses (rc $urc): the planted arm would read red for another reason"; fi
build4i() {
  local src="$1" b="$2"
  ( cd "$W" && timeout 300 "$SCRIP" --compile -o "$b.s" "$src" > /dev/null 2> "$b.c4" < /dev/null ) || refuse "mode 4 could not compile $src: $(head -c 200 "$W/$b.c4")"
  gcc "$W/$b.s" -L"$ROOT/out" -lscrip_rt -lm -lpthread -Wl,-rpath,"$ROOT/out" -o "$W/$b.x" 2> "$W/$b.ld4" || refuse "mode 4 link failed for $src: $(head -c 200 "$W/$b.ld4")"
}
for b in hb_coexpr_parked hb_coexpr_genp_scan; do
  f="$ROOT/scripts/gc_witnesses/$b.icn"
  [ -f "$f" ] || refuse "no Icon witness $f"
  build4i "$f" "$b"
  for m in 3 4; do
    if [ "$m" = 3 ]; then ( cd "$W" && run_env SCRIP_GC_STRESS=1 timeout 300 "$SCRIP" "$f" > /dev/null 2> e < /dev/null ); else ( cd "$W" && run_env SCRIP_GC_STRESS=1 timeout 300 "./$b.x" > /dev/null 2> e < /dev/null ); fi
    s="$(grep -a '^\[CHAIN-WALK\] SUMMARY ' "$W/e" | tail -1)"
    oc="$(num "$s" offchain-swept)"; bl="$(num "$s" bad-link)"; fl="$(num "$s" frame-links)"
    if [ -n "$s" ] && [ "$oc" = 0 ] && [ "$bl" = 0 ] && [ "${fl:-0}" -gt 0 ]; then ck ok "(5) Icon $b mode $m: offchain-swept=0 bad-link=0 ($fl generator frame link(s) followed through act +8 H)"
    else ck no "(5) Icon $b mode $m: offchain-swept=${oc:-?} bad-link=${bl:-?} frame-links=${fl:-?} -- a retained Icon generator frame is off the chain and off every typed H link"; fi
  done
done
planted 1 "$ROOT/scripts/gc_witnesses/hb_coexpr_parked.icn"; prc=$?
if [ "$prc" != 0 ] && grep -aq 'FRAME-LINK REFUSED' "$W/ep" && grep -aq 'REFUSED: the chain over the maps stops short (bad-link)' "$W/ep"; then ck ok "(6) PLANTED: an Icon H link displaced by 16 bytes is refused by name and the collection dies on it (rc $prc), never followed"
else ck no "(6) PLANTED: Icon rc=$prc and no named refusal -- a displaced H word was followed or skipped"; fi
planted "" "$ROOT/scripts/gc_witnesses/hb_coexpr_parked.icn"; urc=$?
if [ "$urc" = 0 ] && ! grep -aq 'FRAME-LINK REFUSED' "$W/ep"; then ck ok "(6) the same Icon run without the plant is whole (rc 0, no refusal), so the planted arm can fail"
else ck no "(6) the same Icon run without the plant refuses (rc $urc): the planted arm would read red for another reason"; fi
z="$(cd "$W" && "$SCRIP" --dump-zeta w.pl < /dev/null 2>/dev/null | grep -c 'PTR_FRAME callgen.act +0')"
if [ "${z:-0}" -gt 0 ]; then ck ok "(4) the witness's --dump-zeta types callgen.act +0 PTR_FRAME at $z call site(s)"
else ck no "(4) no callgen.act +0 is typed PTR_FRAME in the witness's frames"; fi
echo "population: $checks arm(s) graded, $fails FAIL"
if [ "$fails" = 0 ]; then echo "GATE PASS(0) [$G]"; exit 0; fi
echo "⛔ GATE RED [$G]: $fails of $checks arms FAIL"; exit 1
