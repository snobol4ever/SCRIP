#!/usr/bin/env bash
# test_gate_progress_rows_carry_the_start_fingerprint.sh -- A PROGRESS ROW NAMES THE TREE AND BINARY IT ACTUALLY
# GRADED, AND A RUN WHOSE GROUND MOVED APPENDS NOTHING (ceo CEO-338 -> hq_T, row progress-rows-carry-the-tree-and-
# binary-fingerprint-read-at-start-and-a-board-that-refuses-appends-nothing).
#
# THE WITNESS THIS EXISTS AGAINST: seat10 started test_corpus_snobol4.sh at 13:04 CDT; the tree pulled at 13:09 and
# 14:09 and the binary was rebuilt 13:16 UNDER it; the runner correctly REFUSED rc=2 at 14:23 ("binary moved, do NOT
# quote this board") -- but corpus_suite_harness.py had ALREADY bulk-appended 3736 rows stamped 3377cf43e, because
# util_progress_append.context() reads rev-parse at APPEND time, not at grade time. Those rows carry a pre-revert
# 1533/276/42 board against hq_R's and hq_U's 1845/1845 on the same hash, and the flip histogram read -290 from them.
# ⭐ THE ROW'S REFUSAL WAS RIGHT AND ARRIVED TOO LATE: the runner refused to QUOTE a board it had already RECORDED.
#
# ⛔ A REFUSAL THAT FIRES AFTER THE SIDE EFFECT IS NOT A REFUSAL, IT IS AN ANNOTATION. That is the whole gate.
#
#   1  a hermetic harness run appends rows whose scrip/corpus columns equal the values read BEFORE the first grade
#   2  those rows carry a binary fingerprint equal to the scrip+libscrip_rt.so digest read BEFORE the first grade
#   3  a run whose scrip binary is rewritten mid-run appends ZERO rows and exits 2
#   4  a run whose corpus HEAD moves mid-run appends ZERO rows and exits 2
#   5  a run whose SCRIP HEAD moves mid-run appends ZERO rows and exits 2
#
# ⛔⭐ THE MUTATORS SLEEP 2s AND THAT NUMBER IS MEASURED, NOT GUESSED. The 8-entry m3 run takes ~6 s and the pin now
# happens ~0.3 s in (immediately after resolve_paths), so the mutation window [pin, append] is about [0.3, 6].
# ⭐ THE BUG THIS COMMENT IS PAYING FOR: the pin originally sat AFTER check_scrip(), which shells out to the staleness
# checker and costs SECONDS. A 1 s mutator therefore moved HEAD *before* the pin read it, the pin captured the
# POST-move hash, nothing differed at append time, and arms 3-5 reported "no refusal" for a run that was never asked
# to refuse. The reading was indistinguishable from the real defect -- rows appended, rc=0 -- which is why every arm
# prints rows and rc rather than a bare pass/fail. ⭐ A RACE HERE FAILS SAFE: a mutation landing outside the window
# makes an arm RED, never green, so this gate cannot pass by accident. If the suite is resized, re-measure.
#
# HERMETIC: every arm runs against scratch git repos and a scratch table under mktemp, with SCRIP/RT_DIR pointed at a
# COPY of this tree's binary. The live /home/resources/progress/results.tsv is never opened. ⛔ The copy is deliberate
# and is the arm that makes 3 and 5 possible at all -- rewriting the real binary mid-gate would poison every other
# seat on a 12-seat box.
#
# ⛔ NOTE ON THE COPY, measured the hard way on 2026-09-05 (hq_T, and again by hq_C): `cp scrip <dir>/` does NOT give
# you a second build. scrip is a ~40 KB DRIVER; the emitter lives in out/libscrip_rt.so and the driver's DT_RUNPATH is
# ABSOLUTE. The copy loads the ORIGINAL library unless RT_DIR (and LD_LIBRARY_PATH, searched before RUNPATH) select
# otherwise. Arms 3 and 5 therefore mutate the copied DRIVER, whose digest is what the fingerprint must notice.
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
. "$HERE/lib_gate.sh"
gate_parse_args "$@"
G=progress_rows_carry_the_start_fingerprint
H="$HERE/corpus_suite_harness.py"; PY="$HERE/util_progress_append.py"
[ -f "$H" ] && [ -f "$PY" ] || { echo "GATE UNPROVEN(2) [$G]: harness or writer missing under $HERE"; exit 2; }
command -v git >/dev/null 2>&1 || { echo "GATE UNPROVEN(2) [$G]: git not on PATH"; exit 2; }
[ -x "$ROOT/scrip" ] && [ -e "$ROOT/out/libscrip_rt.so" ] || { echo "GATE UNPROVEN(2) [$G]: this tree has no built binary to copy. Run 'make'."; exit 2; }
W="$(mktemp -d "${TMPDIR:-/tmp}/gate_fingerprint.XXXXXX")" || { echo "GATE UNPROVEN(2) [$G]: mktemp failed"; exit 2; }
trap 'rm -rf "$W"' EXIT
fails=0; checks=0
ck() { checks=$((checks+1)); if [ "$1" = ok ]; then printf '  ok    %s\n' "$2"; else printf '  FAIL  %s\n' "$2"; fails=$((fails+1)); fi; }
# ---- scratch world: two real git repos, a copied binary, a canonical-looking master suite ------------------------
mk_world() {
  local w="$1"; rm -rf "$w"; mkdir -p "$w/SCRIP/scripts" "$w/corpus/tests/snobol4" "$w/bin/out"
  cp "$ROOT/scrip" "$w/bin/scrip"; cp -L "$ROOT/out/libscrip_rt.so" "$w/bin/out/libscrip_rt.so"
  local i=1
  : > "$w/corpus/tests/snobol4/ALL.sno"; : > "$w/corpus/tests/snobol4/ALL.ref"
  while [ "$i" -le 8 ]; do
    printf '*----------------------------------- %d fp_entry_%02d\n' "$i" "$i" >> "$w/corpus/tests/snobol4/ALL.sno"
    printf "          OUTPUT = 'fp%02d'\nEND\n" "$i" >> "$w/corpus/tests/snobol4/ALL.sno"
    printf '*----------------------------------- %d fp_entry_%02d\n' "$i" "$i" >> "$w/corpus/tests/snobol4/ALL.ref"
    printf 'fp%02d\n' "$i" >> "$w/corpus/tests/snobol4/ALL.ref"
    i=$((i+1))
  done
  for r in SCRIP corpus; do
    ( cd "$w/$r" && git init -q . && git config user.name t && git config user.email t@t \
      && : > .keep && git add -A && git commit -q -m init ) || return 1
  done
  return 0
}
run_harness() { # $1=world  $2=db ; runs the harness on the scratch master, prints rc
  local w="$1" db="$2"
  S4E_HOME="$w" SCRIP="$w/bin/scrip" RT_DIR="$w/bin/out" S4E_PROGRESS_DB="$db" TIMEOUT=10 \
    python3 "$H" run "$w/corpus/tests/snobol4/ALL.sno" "$w/corpus/tests/snobol4/ALL.ref" --modes m3 >"$w/out.log" 2>&1
  echo $?
}
rows_in() { [ -f "$1" ] && awk 'NR>1' "$1" | grep -c . || echo 0; }
# ---- arms 1 and 2: the honest run --------------------------------------------------------------------------------
mk_world "$W/a" || { echo "GATE UNPROVEN(2) [$G]: could not build the scratch world"; exit 2; }
pre_scrip="$(git -C "$W/a/SCRIP" rev-parse --short HEAD)"
pre_corpus="$(git -C "$W/a/corpus" rev-parse --short HEAD)"
pre_fp="$(cat "$W/a/bin/scrip" "$W/a/bin/out/libscrip_rt.so" | md5sum | cut -d' ' -f1)"
rc="$(run_harness "$W/a" "$W/a/db.tsv")"
n="$(rows_in "$W/a/db.tsv")"
if [ "$n" -gt 0 ]; then
  bad_tree="$(awk -F'\t' -v s="$pre_scrip" -v c="$pre_corpus" 'NR>1 && ($2!=s || $3!=c)' "$W/a/db.tsv" | grep -c .)"
  ck "$([ "$bad_tree" = 0 ] && echo ok || echo no)" "arm1 rows name the tree read BEFORE grading (rows=$n mismatched=$bad_tree want scrip=$pre_scrip corpus=$pre_corpus)"
  has_fp="$(awk -F'\t' -v f="$pre_fp" 'NR>1 && index($0,f)>0' "$W/a/db.tsv" | grep -c .)"
  ck "$([ "$has_fp" = "$n" ] && echo ok || echo no)" "arm2 every row carries the START binary fingerprint $pre_fp (rows=$n carrying=$has_fp)"
else
  ck no "arm1 rows name the tree read BEFORE grading (harness appended NOTHING, rc=$rc -- see $W/a/out.log)"
  ck no "arm2 every row carries the START binary fingerprint (no rows appended)"
fi
# ---- arm 3: the binary is rewritten mid-run ----------------------------------------------------------------------
mk_world "$W/b" >/dev/null 2>&1
( sleep 2; printf '\0BROKEN' >> "$W/b/bin/scrip" ) &
mut=$!
rc3="$(run_harness "$W/b" "$W/b/db.tsv")"; wait "$mut" 2>/dev/null
n3="$(rows_in "$W/b/db.tsv")"
ck "$([ "$n3" = 0 ] && [ "$rc3" = 2 ] && echo ok || echo no)" "arm3 binary rewritten mid-run -> ZERO rows and rc=2 (rows=$n3 rc=$rc3)"
# ---- arm 4: the corpus HEAD moves mid-run -------------------------------------------------------------------------
mk_world "$W/c" >/dev/null 2>&1
( sleep 2; cd "$W/c/corpus" && date +%s%N >> .keep && git add -A && git commit -q -m moved ) &
mut=$!
rc4="$(run_harness "$W/c" "$W/c/db.tsv")"; wait "$mut" 2>/dev/null
n4="$(rows_in "$W/c/db.tsv")"
post_c="$(git -C "$W/c/corpus" rev-parse --short HEAD)"
[ "$post_c" != "$(git -C "$W/c/corpus" rev-list --max-parents=0 --abbrev-commit HEAD | tail -1)" ] || { echo "GATE UNPROVEN(2) [$G]: arm4 mutator did not move corpus HEAD -- the arm would pass vacuously"; exit 2; }
ck "$([ "$n4" = 0 ] && [ "$rc4" = 2 ] && echo ok || echo no)" "arm4 corpus HEAD moved mid-run -> ZERO rows and rc=2 (rows=$n4 rc=$rc4)"
# ---- arm 5: the SCRIP HEAD moves mid-run ---------------------------------------------------------------------------
mk_world "$W/d" >/dev/null 2>&1
( sleep 2; cd "$W/d/SCRIP" && date +%s%N >> .keep && git add -A && git commit -q -m moved ) &
mut=$!
rc5="$(run_harness "$W/d" "$W/d/db.tsv")"; wait "$mut" 2>/dev/null
n5="$(rows_in "$W/d/db.tsv")"
post_d="$(git -C "$W/d/SCRIP" rev-parse --short HEAD)"
[ "$post_d" != "$(git -C "$W/d/SCRIP" rev-list --max-parents=0 --abbrev-commit HEAD | tail -1)" ] || { echo "GATE UNPROVEN(2) [$G]: arm5 mutator did not move SCRIP HEAD -- the arm would pass vacuously"; exit 2; }
ck "$([ "$n5" = 0 ] && [ "$rc5" = 2 ] && echo ok || echo no)" "arm5 SCRIP HEAD moved mid-run -> ZERO rows and rc=2 (rows=$n5 rc=$rc5)"
# ---- verdict ------------------------------------------------------------------------------------------------------
printf '%s: %d/%d arms ok\n' "$G" "$((checks-fails))" "$checks"
[ "$fails" = 0 ] || { echo "GATE RED [$G]: $fails of $checks arms failed"; exit 1; }
echo "GATE OK [$G]"
