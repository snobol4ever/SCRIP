#!/usr/bin/env bash
# test_gate_pl_quad_regs.sh -- THE QUAD GATE (ARCH-PROLOG-BYRD-BOX-TRANSLATION.md § E instrument lane; RULES.md § THE PROLOG
# REBUILD GATE; hq_P 2026-09-02). Lon's grants pin four registers for Prolog: r12 TR (the trail) · r13 B (= H, the frame header)
# · r14 ROOT (the ζ-STANDING record) · r15 BALL (the exception in flight, or 0). Every β guard, every ω transition and rtx_plunify.s
# read them DIRECTLY, so a single stray write anywhere on a Prolog path corrupts a value nobody reloads. THE RULE THIS GATE PINS:
#   no instruction reachable from a Prolog graph writes r12-r15 outside the TR/B/ROOT/BALL helpers.
# TWO ARMS, one analyzer:
#   ARM 1 -- EMITTED CODE. Every Prolog witness (the ladder__ origins and the probe_plz origins of corpus/tests/prolog/ALL.pl, every
#            corpus/benchmarks/prolog/bench/*.pl, every corpus/demos/prolog/*.pl) is compiled FRESH (--compile, TEXT medium; the
#            committed .s artifacts are never read -- sweep the compiler, never the artifacts). Every write whose DESTINATION is
#            r12-r15 (mov/lea/add/.../pop/cmov*/set*/xchg, Intel syntax, the ;-packed multi-instruction lines and the
#            label-on-the-same-line shape both handled, byte-safe over the Greek port labels) is attributed to its enclosing
#            column-0 box label and its enclosing .L local label; it is legal iff either label matches QUAD_HELPER_RX.
#   ARM 2 -- rtx ASM. The rtx functions REACHABLE from those .s files (call/jmp targets that are rtx labels, closed transitively
#            over call/jmp inside src/runtime/rtx/) are swept the same way. A function that saves and restores the register
#            (>=1 push AND >=1 pop of it inside the function) is callee-saved-clean and its writes are legal (SysV: r12-r15 are
#            callee-saved, which is why every C service is clean for free and why an asm service must do what C does). Unreachable
#            rtx functions that write the quad are printed as INFO and NOT graded: rtx_match.s's r12 is SNOBOL4's C.A.S. cursor by
#            design, reached only from SNOBOL4 graphs. QUAD_RTX_ALL=1 sweeps every rtx function regardless (census, not verdict).
# QUAD_HELPER_RX -- the ONE allow-list. It names the helper LABELS (emitted) / FUNCTIONS (rtx) that may write the quad. Today NO
#   helper exists (the cut has not landed), so the default matches nothing and every write is a violation: the gate is BORN RED
#   (measured on SCRIP f4532dea, -O0: nrev.pl 89 writes, fib.pl 5, and 73 of nrev's are `lea r12,[rip+g_pl_*]` -- the by-name sinks'
#   scratch that the cut deletes). hq_C names the helpers in the rung that creates them, HERE, once; never a per-site exception.
# --self-test -- the two-direction proof on fixtures (RULES.md THE INSTRUMENT LAWS, fifth batch): the analyzer must say NO on a
#   box-labelled write and YES on a helper-labelled one; the rtx arm must reach a function transitively, accept a balanced
#   push/pop, refuse an unbalanced write, and leave an unreached function ungraded. Exit 0 iff every expectation holds.
# EXIT: 0 clean over a printed denominator >= 1 witness; 1 violations (every site printed, or the first 25 with the count and
#       GATE_LIST_ALL=1 named); 2 UNPROVEN (scrip unbuilt, master suite absent, zero witnesses compiled).
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"; cd "$ROOT"
. "$HERE/lib_gate.sh"
SELFTEST=0; for a in "$@"; do [ "$a" = --self-test ] && SELFTEST=1; done
gate_parse_args "$@"
SCRIP="${SCRIP:-$ROOT/scrip}"; RT="${RT_DIR:-$ROOT/out}"; T="${TIMEOUT:-30}"
RTX_DIR="${RTX_DIR:-$ROOT/src/runtime/rtx}"
QUAD_HELPER_RX="${QUAD_HELPER_RX:-^NO-HELPER-NAMED-YET\$}"
QUAD_RTX_ALL="${QUAD_RTX_ALL:-0}"; LIST_CAP="${QUAD_LIST_CAP:-25}"
W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
# quad_sites <file> -> "line<TAB>box<TAB>local<TAB>instruction" per write to r12-r15. Byte-safe (mawk, no \b, ASCII classes only).
quad_sites() {
  awk '
  function trim(s){ sub(/^[ \t]+/,"",s); sub(/[ \t]+$/,"",s); return s }
  BEGIN{ box="(none)"; loc="(none)" }
  { line=$0; sub(/#.*$/,"",line)
    if (match(line, /^\.macro[ \t]+[A-Za-z_][A-Za-z0-9_]*/)) { box=substr(line, 8, RLENGTH-7); sub(/^[ \t]+/,"",box); loc="(none)"; next }
    while (match(line, /^[^ \t:][^ \t:]*:/)) { lab=substr(line,1,RLENGTH-1); line=substr(line,RLENGTH+1)
      if (substr(lab,1,2)==".L") loc=lab; else { box=lab; loc="(none)" } }
    n=split(line, parts, ";")
    for (i=1;i<=n;i++) { p=trim(parts[i]); if (p=="") continue
      if (p ~ /^(mov|movsx|movsxd|movzx|movabs|lea|add|sub|adc|sbb|and|or|xor|not|neg|inc|dec|shl|shr|sal|sar|rol|ror|imul|pop|cmov[a-z]+|bswap|set[a-z]+)[ \t]+r1[2-5](d|w|b)?([ \t]*,|[ \t]*$)/ \
       || p ~ /^xchg[ \t]+([^,]*,[ \t]*)?r1[2-5](d|w|b)?([ \t]*,|[ \t]*$)/)
        printf "%d\t%s\t%s\t%s\n", NR, box, loc, p } }' "$1"
}
# quad_reg <instruction> -> the register written (r12..r15)
quad_reg() { local ops; ops=$(printf '%s\n' "$1" | sed -E 's/^[a-z]+[ \t]+//')
  case "$1" in xchg*) printf '%s\n' "$ops" | tr ',' '\n' | sed 's/^[ \t]*//' | grep -oE '^r1[2-5]' | head -1 ;; *) printf '%s\n' "$ops" | grep -oE '^r1[2-5]' | head -1 ;; esac; }
# rtx_functions <dir> -> "file<TAB>name<TAB>start<TAB>end" for every column-0 non-.L label (and .macro) in rtx_*.s + rtx_abi.inc
rtx_functions() {
  local f; for f in "$1"/rtx_*.s "$1"/rtx_abi.inc; do [ -f "$f" ] || continue
    awk -v F="$f" '
    function flush(){ if (name!="") printf "%s\t%s\t%d\t%d\n", F, name, start, NR-1 }
    { if (match($0, /^\.macro[ \t]+[A-Za-z_][A-Za-z0-9_]*/)) { flush(); name=substr($0,8,RLENGTH-7); sub(/^[ \t]+/,"",name); start=NR; next }
      if (match($0, /^RTX_FUNC\([A-Za-z_][A-Za-z0-9_]*\)/)) { flush(); name=substr($0,10,RLENGTH-10); start=NR; next }
      if (match($0, /^[A-Za-z_][A-Za-z0-9_$]*:/)) { flush(); name=substr($0,1,RLENGTH-1); start=NR } }
    END{ NR=NR+1; flush() }' "$f"; done
}
# call_targets <file> [start end] -> bare symbols after call/jmp (no @PLT, no registers, no .L locals)
call_targets() {
  local f="$1" s="${2:-1}" e="${3:-999999999}"
  awk -v s="$s" -v e="$e" 'NR>=s && NR<=e { line=$0; sub(/#.*$/,"",line); n=split(line, parts, ";")
    for (i=1;i<=n;i++) { p=parts[i]; sub(/^[ \t]*[^ \t:]+:[ \t]*/,"",p); sub(/^[ \t]+/,"",p)
      if (match(p, /^(call|jmp)[ \t]+[A-Za-z_][A-Za-z0-9_$]*/)) { t=substr(p,1,RLENGTH); sub(/^(call|jmp)[ \t]+/,"",t); print t } } }' "$f" | sed 's/@PLT$//' | sort -u
}
# arm1 <name> <.s file> -> prints "  name writes=K violations=V r12=a r13=b r14=c r15=d" and appends violations to $W/viol1; returns V
arm1() {
  local name="$1" f="$2" k=0 v=0 r12=0 r13=0 r14=0 r15=0 ln box loc ins reg
  while IFS=$'\t' read -r ln box loc ins; do
    [ -n "$ln" ] || continue; k=$((k+1)); reg=$(quad_reg "$ins"); case "$reg" in r12) r12=$((r12+1));; r13) r13=$((r13+1));; r14) r14=$((r14+1));; r15) r15=$((r15+1));; esac
    if printf '%s\n' "$box" | grep -qE "$QUAD_HELPER_RX" || printf '%s\n' "$loc" | grep -qE "$QUAD_HELPER_RX"; then :; else v=$((v+1)); printf '%s:%s\t%s\t%s\t%s\n' "$name" "$ln" "$box" "$loc" "$ins" >> "$W/viol1"; fi
  done < <(quad_sites "$f")
  printf '  %-28s writes=%-4d violations=%-4d r12=%d r13=%d r14=%d r15=%d\n' "$name" "$k" "$v" "$r12" "$r13" "$r14" "$r15"
  echo "$k $v" >> "$W/arm1.counts"; return 0
}
# arm2 <rtx-dir> <roots-file> -> reachability closure + sweep; prints its census; appends violations to $W/viol2
arm2() {
  local dir="$1" roots="$2" fn file name start end reach="$W/reach" pend="$W/pend" i=0
  rtx_functions "$dir" > "$W/rtxfn"; local nf; nf=$(wc -l < "$W/rtxfn"); cut -f2 "$W/rtxfn" | sort -u > "$W/allfn"
  if [ "$QUAD_RTX_ALL" = 1 ]; then cp "$W/allfn" "$reach"
  else
    sort -u "$roots" | grep -xFf "$W/allfn" > "$pend" || true; : > "$reach"
    while [ -s "$pend" ]; do i=$((i+1)); [ $i -gt 64 ] && break
      : > "$pend.next"
      while read -r name; do [ -n "$name" ] || continue; grep -qx "$name" "$reach" && continue; echo "$name" >> "$reach"
        while IFS=$'\t' read -r file fname start end; do [ "$fname" = "$name" ] || continue
          call_targets "$file" "$start" "$end" >> "$pend.next"; done < "$W/rtxfn"
      done < "$pend"
      sort -u "$pend.next" | grep -xFf "$W/allfn" > "$pend" || true; done
  fi
  local nr; nr=$(sort -u "$reach" | grep -c . || true)
  echo "ARM 2 rtx asm ($dir): functions=$nf  reachable-from-the-witnesses=$nr$([ "$QUAD_RTX_ALL" = 1 ] && echo ' (QUAD_RTX_ALL=1: every function, census not verdict)')"
  [ "$QUAD_RTX_ALL" = 1 ] || [ "$nr" -eq 0 ] || echo "  reachable: $(sort -u "$reach" | tr '\n' ' ')"
  local k2=0 v2=0 info=0
  while IFS=$'\t' read -r file name start end; do
    local sites; sites=$(sed -n "${start},${end}p" "$file" | quad_sites /dev/stdin); [ -n "$sites" ] || continue
    local n_sites; n_sites=$(printf '%s\n' "$sites" | grep -c .)
    if ! grep -qx "$name" "$reach"; then info=$((info+1)); echo "  INFO unreached (not graded): $(basename "$file"):$name writes=$n_sites"; continue; fi
    k2=$((k2+n_sites))
    local ln box loc ins reg pushes pops legal
    while IFS=$'\t' read -r ln box loc ins; do [ -n "$ln" ] || continue; reg=$(quad_reg "$ins")
      pushes=$(sed -n "${start},${end}p" "$file" | grep -cE "^[[:space:]]*push[[:space:]]+$reg([[:space:]]|$)" || true)
      pops=$(sed -n "${start},${end}p" "$file" | grep -cE "^[[:space:]]*pop[[:space:]]+$reg([[:space:]]|$)" || true)
      legal=no; printf '%s\n' "$name" | grep -qE "$QUAD_HELPER_RX" && legal=helper
      [ "$legal" = no ] && [ "${pushes:-0}" -ge 1 ] && [ "${pops:-0}" -ge 1 ] && legal=callee-saved
      if [ "$legal" = no ]; then v2=$((v2+1)); printf '%s:%d\t%s\t%s\t%s\n' "$(basename "$file")" $((start+ln-1)) "$name" "$loc" "$ins" >> "$W/viol2"
      else echo "  legal ($legal): $(basename "$file"):$name  $ins"; fi
    done <<< "$sites"
  done < "$W/rtxfn"
  echo "  reachable quad-writes=$k2  violations=$v2  unreached-with-writes(INFO)=$info"
  echo "$k2 $v2" > "$W/arm2.counts"
}
selftest() {
  local ok=1 d="$W/st"; mkdir -p "$d/rtx"
  printf 'n0_call_prolog_bx:\n                        lea              r12, [rip + g_pl_trail]\n.Lfoo_α_1:              pop              r12;                                 jmp   r12\n                        xchg             rax, r13\n                        mov              qword ptr [r12], rax\npl_tr_push:\n                        add              r12, 16\n' > "$d/w.s"
  printf 'RTX_FUNC(rtx_clean_fn)\n    push r12\n    mov r12, rax\n    call rtx_dirty_fn\n    pop r12\n    ret\nRTX_ENDF(rtx_clean_fn)\nrtx_dirty_fn:\n    mov r13, rax\n    ret\nRTX_FUNC(rtx_unreached_fn)\n    mov r14, rax\n    ret\nRTX_ENDF(rtx_unreached_fn)\n' > "$d/rtx/rtx_fixture.s"
  printf '                        call             rtx_clean_fn@PLT\n' > "$d/caller.s"
  : > "$W/viol1"; : > "$W/arm1.counts"
  local line; line=$(QUAD_HELPER_RX='^pl_(tr|b|root|ball)_' arm1 fixture "$d/w.s")
  local v1; v1=$(wc -l < "$W/viol1")
  [ "$v1" = 3 ] && echo "  ✅ arm 1 says NO: 3 box-labelled writes flagged (lea r12 / pop r12 on a label line / xchg r13), the helper-labelled add r12 legal, the [r12] store not a write" || { echo "  ⛔ arm 1 expected 3 violations, got $v1"; cat "$W/viol1"; ok=0; }
  : > "$W/viol1"; : > "$W/arm1.counts"; QUAD_HELPER_RX='.' arm1 fixture "$d/w.s" >/dev/null; v1=$(wc -l < "$W/viol1")
  [ "$v1" = 0 ] && echo "  ✅ arm 1 says YES: 0 violations when every label is a helper" || { echo "  ⛔ arm 1 expected 0 violations under an allow-all, got $v1"; ok=0; }
  : > "$W/viol2"; call_targets "$d/caller.s" > "$W/roots"; QUAD_RTX_ALL=0 arm2 "$d/rtx" "$W/roots" > "$W/arm2.out"; local v2; v2=$(wc -l < "$W/viol2")
  if [ "$v2" = 1 ] && grep -q 'rtx_dirty_fn' "$W/viol2" && grep -q 'legal (callee-saved).*rtx_clean_fn' "$W/arm2.out" && grep -q 'INFO unreached.*rtx_unreached_fn' "$W/arm2.out"; then
    echo "  ✅ arm 2: reached rtx_dirty_fn TRANSITIVELY through rtx_clean_fn and flagged its r13 write; rtx_clean_fn's balanced push/pop legal; rtx_unreached_fn INFO only"
  else echo "  ⛔ arm 2 expectation failed: violations=$v2"; cat "$W/arm2.out" "$W/viol2"; ok=0; fi
  [ "$ok" = 1 ] && { echo "SELF-TEST PASS(0) [$GATE_NAME]: the analyzer can say NO and YES on both arms"; gate_stamp; exit 0; }
  echo "SELF-TEST FAIL(1) [$GATE_NAME]"; gate_stamp; exit 1
}
[ "$SELFTEST" = 1 ] && selftest
gate_require_exec "$SCRIP" "scrip binary"
gate_require "$RT/libscrip_rt.so" "runtime library"
gate_require "$RTX_DIR/rtx_abi.inc" "rtx ABI include"
MASTER_DIR="$S4E/corpus/tests/prolog"; MASTER_EXT=.pl; export MASTER_DIR MASTER_EXT
gate_require "$MASTER_DIR/ALL.pl" "Prolog master suite"; gate_require "$MASTER_DIR/ALL.csv" "Prolog master suite index"
. "$HERE/lib_master_extract.sh" || { echo "GATE UNPROVEN(2) [$GATE_NAME]: cannot source lib_master_extract.sh"; gate_stamp; exit 2; }
BENCH="$S4E/corpus/benchmarks/prolog/bench"; DEMOS="$S4E/corpus/demos/prolog"
lad=$(master_origins_by_prefix ladder__ 2>/dev/null || true); plz=$(master_origins_of_family probe_plz 2>/dev/null || true)
n_lad=$(printf '%s\n' "$lad" | grep -c . || true); n_plz=$(printf '%s\n' "$plz" | grep -c . || true)
n_bench=$(ls -1 "$BENCH"/*.pl 2>/dev/null | wc -l); n_demo=$(ls -1 "$DEMOS"/*.pl 2>/dev/null | wc -l)
echo "QUAD GATE -- r12 TR · r13 B · r14 ROOT · r15 BALL: no write reachable from a Prolog graph outside the helpers (QUAD_HELPER_RX='$QUAD_HELPER_RX')"
echo "ARM 1 emitted .s (--compile, TEXT, fresh): witnesses = ladder $n_lad + probe_plz $n_plz + bench $n_bench + demos $n_demo"
: > "$W/viol1"; : > "$W/arm1.counts"; : > "$W/roots"; compiled=0; noemit=0
compile_one() { local name="$1" src="$2"; (cd "$W" && timeout "$T" "$SCRIP" --compile -o "$W/$name.s" "$src" </dev/null >/dev/null 2>&1)
  if [ -s "$W/$name.s" ]; then compiled=$((compiled+1)); arm1 "$name" "$W/$name.s"; call_targets "$W/$name.s" >> "$W/roots"; else noemit=$((noemit+1)); echo "  $name NOEMIT (no .s produced -- not a quad verdict, counted)"; fi; }
for o in $lad $plz; do master_extract_origin "$o" "$W/$o.pl" >/dev/null 2>&1 || { echo "GATE UNPROVEN(2) [$GATE_NAME]: cannot extract $o from the master suite"; gate_stamp; exit 2; }; compile_one "$o" "$W/$o.pl"; done
for f in "$BENCH"/*.pl "$DEMOS"/*.pl; do [ -f "$f" ] || continue; compile_one "bench:$(basename "${f%.pl}")" "$f"; done
gate_floor "$compiled" 1 "compiled Prolog witnesses"
k1=$(awk '{k+=$1} END{print k+0}' "$W/arm1.counts"); v1=$(awk '{v+=$2} END{print v+0}' "$W/arm1.counts")
echo "  ARM 1 totals: compiled=$compiled noemit=$noemit quad-writes=$k1 violations=$v1"
if [ "$v1" -gt 0 ]; then
  if [ "${GATE_LIST_ALL:-0}" = 1 ] || [ "$v1" -le "$LIST_CAP" ]; then sed 's/^/    /' "$W/viol1"
  else head -n "$LIST_CAP" "$W/viol1" | sed 's/^/    /'; echo "    ... $((v1-LIST_CAP)) more sites not listed (cap $LIST_CAP; GATE_LIST_ALL=1 prints every site)"; fi
  echo "  by instruction:"; awk -F'\t' '{split($4,a,/[ \t]+/); print a[1], a[2]}' "$W/viol1" | sort | uniq -c | sort -rn | head -8 | sed 's/^/    /'
fi
sort -u "$W/roots" -o "$W/roots"; : > "$W/viol2"; arm2 "$RTX_DIR" "$W/roots"
v2=$(wc -l < "$W/viol2"); [ "$v2" -gt 0 ] && sed 's/^/    /' "$W/viol2"
GATE_EXAMINED="$compiled witnesses, $k1 emitted quad-writes, $(cut -d' ' -f1 "$W/arm2.counts") reachable rtx quad-writes"
gate_verdict $((v1+v2)) "quad-register writes outside the TR/B/ROOT/BALL helpers"
