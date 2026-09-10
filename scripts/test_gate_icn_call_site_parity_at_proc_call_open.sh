#!/usr/bin/env bash
# test_gate_icn_call_site_parity_at_proc_call_open.sh -- SysV call-site parity at rt_proc_call_open{,_det}.
#
# WHY THIS EXISTS (CEO-458): the Icon generator call site entered rt_proc_call_open_det at rsp 8-mod-16,
# a real ABI violation, because bb_call_proc_staged's N-2 pad kept compensating for a REGION HAND-OFF push
# that N-3 had moved to the OTHER SIDE of the call. A compensating pad outlived the push it compensated for.
# It was latent for weeks and only surfaced when a callee finally reached an aligned SSE store -- a movaps
# inside vsnprintf, reached only when the traced call had an argument to image. A generator with NO argument
# was equally misaligned and passed, which is exactly why a witness-based test could not have caught it.
#
# ⛔ SO THIS GATE MEASURES THE INVARIANT, NOT THE SYMPTOM: rsp % 16 == 0 at EVERY call to
# rt_proc_call_open/rt_proc_call_open_det, in a --compile of each witness, read out of the running process.
# A symptom test (does the tracer crash) goes green the moment any pad happens to re-balance the stack;
# this goes red the moment any pad stops matching its push, which is the class.
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="$ROOT/scrip"
NAME=icn_call_site_parity_at_proc_call_open
refuse() { echo "GATE REFUSE(2) [$NAME]: $*"; exit 2; }
[ -x "$SCRIP" ] || refuse "no scrip binary at $SCRIP -- cannot measure"
command -v gdb >/dev/null 2>&1 || refuse "gdb absent -- this gate reads rsp at a breakpoint and cannot measure without it"
command -v objdump >/dev/null 2>&1 || refuse "objdump absent -- cannot locate the call sites"
. "$HERE/lib_gate.sh"
gate_require_fresh "$ROOT" src "$ROOT/scrip" "$ROOT/out/libscrip_rt.so"
TD="$(mktemp -d)"; trap 'rm -rf "$TD"' EXIT
cat > "$TD/gen_arg.icn" <<'EOF'
procedure main();
    &trace := -1;
    every write(g(1));
end
procedure g(x);
    suspend x;
end
EOF
cat > "$TD/gen_noarg.icn" <<'EOF'
procedure main();
    &trace := -1;
    every write(g());
end
procedure g();
    suspend 1;
end
EOF
cat > "$TD/nongen_arg.icn" <<'EOF'
procedure main();
    &trace := -1;
    write(g(1));
end
procedure g(x);
    return x;
end
EOF
GRADED=0; FAIL=0
for w in gen_arg gen_noarg nongen_arg; do
  "$SCRIP" --compile -o "$TD/$w.s" "$TD/$w.icn" </dev/null >/dev/null 2>&1 || refuse "$w: --compile failed -- cannot measure"
  gcc -no-pie "$TD/$w.s" -o "$TD/$w.bin" -L "$ROOT/out" -Wl,-rpath,"$ROOT/out" -lscrip_rt -lm -lpthread >/dev/null 2>&1 || refuse "$w: link failed -- cannot measure"
  sites="$(objdump -d "$TD/$w.bin" 2>/dev/null | grep -oE '^ *[0-9a-f]+:.*call.*rt_proc_call_open(_det)?' | awk -F: '{print $1}' | tr -d ' ')"
  [ -n "$sites" ] || { echo "  SKIPPED $w: no rt_proc_call_open call site in this witness"; continue; }
  for a in $sites; do
    GRADED=$((GRADED+1))
    printf '%s\n' 'set debuginfod enabled off' 'set pagination off' "break *0x$a" 'run' 'printf "PARITY=%d\n", ((long)$sp)%16' > "$TD/gdb.cmd"
    rem="$(setarch -R gdb -q -batch -x "$TD/gdb.cmd" --args "$TD/$w.bin" 2>/dev/null | grep -oE 'PARITY=[0-9]+' | head -1 | cut -d= -f2)"
    if [ -z "$rem" ]; then refuse "$w @0x$a: the breakpoint was never reached, so the parity was NOT measured -- a gate that cannot measure REFUSES, it does not report a failure it did not observe"; fi
    if [ "$rem" = "0" ]; then echo "  PASS $w @0x$a: rsp%16=0"
    else echo "  FAIL $w @0x$a: rsp%16=$rem -- SysV requires 0 at a call; a pad no longer matches its push"; FAIL=$((FAIL+1)); fi
  done
done
# ⛔⭐ ARM B, THE CALLEE SIDE (hq_U, CEO-483). ARM A ABOVE IS ON THE WRONG SIDE OF THE PUSH BLOCK TO SEE THIS
# CLASS, and that is measured, not argued: dropping the N-2 pad SIGSEGVs patchu in both modes while arm A
# stays 2/2 GREEN, because rt_proc_call_open is a CALLER-side call ABOVE the push block and the pad moves
# parity BELOW it, at the jmp into the callee. Arm A is correct and is not weakened; it simply cannot reach
# past the jmp. So this arm measures the other side.
#
# ⛔ AND IT DOES NOT MEASURE rsp AT THE CALLEE'S FIRST INSTRUCTION, which is the obvious thing to write and is
# WRONG: since CEO-483 the generator entry frame is FIVE words and entry rsp is 8-mod-16 ON PURPOSE -- the
# odd word moved across the call into the callee's own carve, which absorbs it. A gate pinned to the entry
# instruction would go red on the very landing that cured the bug. What SysV actually requires, and what the
# crash actually violates, is 0-mod-16 AT A CALL: so this arm breaks on the FIRST call the callee BODY makes
# after its carve. That is the exact instruction the record bug died in (a helper reached from a misaligned
# body, movaps inside vsnprintf), it is true under both the old six-word and the new five-word layout, and it
# goes red the moment any carve stops absorbing the parity its call site hands it.
for w in gen_arg gen_noarg nongen_arg; do
  [ -f "$TD/$w.bin" ] || continue
  for fn in $(objdump -d "$TD/$w.bin" 2>/dev/null | grep -oE '^[0-9a-f]+ <FN__[A-Za-z0-9_]+>:' | sed 's/.*<//; s/>:.*//'); do
    body="$(objdump -d --disassemble="$fn" "$TD/$w.bin" 2>/dev/null | grep -E '^ *[0-9a-f]+:' | grep -E '\<call\>' | head -1 | awk -F: '{print $1}' | tr -d ' ')"
    [ -n "$body" ] || { echo "  SKIPPED $w/$fn: the callee body makes no call -- nothing this arm can measure here"; continue; }
    GRADED=$((GRADED+1))
    printf '%s\n' 'set debuginfod enabled off' 'set pagination off' "break *0x$body" 'run' 'printf "PARITY=%d\n", ((long)$sp)%16' > "$TD/gdb.cmd"
    rem="$(setarch -R gdb -q -batch -x "$TD/gdb.cmd" --args "$TD/$w.bin" 2>/dev/null | grep -oE 'PARITY=[0-9]+' | head -1 | cut -d= -f2)"
    if [ -z "$rem" ]; then refuse "$w/$fn @0x$body: the breakpoint was never reached, so the callee-body parity was NOT measured -- a gate that cannot measure REFUSES, it does not report a failure it did not observe"; fi
    if [ "$rem" = "0" ]; then echo "  PASS $w/$fn @0x$body: callee-body rsp%16=0 at its first call"
    else echo "  FAIL $w/$fn @0x$body: callee-body rsp%16=$rem at its first call -- the carve is not absorbing the parity its call site handed it; every helper this body reaches is misaligned and the first aligned SSE store in one of them dies"; FAIL=$((FAIL+1)); fi
  done
done
[ "$GRADED" -eq 0 ] && refuse "graded zero call sites -- the witnesses reached no rt_proc_call_open call, so this gate measured nothing"
echo "graded=$GRADED FAIL=$FAIL (arm A rsp%16 at every rt_proc_call_open/_det call site + arm B rsp%16 at the first call each callee BODY makes after its carve, both read from the running process; gen_noarg is in the set ON PURPOSE -- it was misaligned and PASSING, so a symptom test would have exonerated it)"
if [ "$FAIL" -ne 0 ]; then echo "GATE FAIL(1) [$NAME]: FAIL=$FAIL of $GRADED"; exit 1; fi
echo "GATE PASS(0) [$NAME]: $GRADED/$GRADED"
exit 0
