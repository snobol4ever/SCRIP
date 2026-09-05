#!/usr/bin/env bash
# util_cut_icon_ipl_refs.sh -- STEP 1 of task icon-ipl-851-run-graded-against-iconx-refs-and-cured-by-class:
# cut a RUN ref (NAME.std) from the real Icon oracle (icon_bin, lib_oracle_flags.sh -- the ONE authority,
# never re-derived here) for every corpus/packages/icon/ipl/progs/*.icn with a deterministic, input-free
# `procedure main`. Same discipline as util_ref_mint.sh's SNOBOL4 LIVE/DEAD_REPORT/EMPTY classes, adapted:
# CENSUS ONLY by default (classifies, writes nothing); --apply mints a .std beside every LIVE program.
#
#   bash scripts/util_cut_icon_ipl_refs.sh [--apply] [-v]
#
# ⛔⛔ EVERY RUN IS SANDBOXED IN A DISPOSABLE SCRATCH COPY, NEVER THE TRACKED TREE (found this session,
# the hard way): IPL ships several programs that MUTATE THEIR OWN CWD as their normal, documented
# behavior -- progs/upper.icn and progs/lower.icn rename EVERY file in cwd to upper/lower case, and at
# least one other writes a fixed-name foo/XXXXXX scratch file. A first attempt that ran the oracle with
# $PKG/progs as cwd (matching test_icon_arizona_suite.sh's cwd-fidelity pattern) renamed all 275 vendored
# .icn sources to UPPERCASE in place -- `git status` showed 275 D + 279 ?? before it was caught and
# reverted (`git checkout --` + a scoped `git clean -f` on exactly that directory, verified clean after).
# Arizona's own snapshot-diff litter cleanup (test_icon_arizona_suite.sh) only catches ADDITIVE litter
# (new files); it does not protect EXISTING tracked files from being renamed or overwritten in place, so
# it is not sufficient here. This script instead builds ONE pristine template copy of
# progs+procs+gprocs+incl+gincl (~4.7M, cheap) and gives EACH oracle invocation its OWN throwaway copy of
# it, discarded immediately after -- so a self-mutating program can only ever damage a copy already bound
# for /tmp cleanup, never the corpus tree, regardless of what it does. THE SAME HAZARD APPLIES TO GRADING
# (STEP 3, running SCRIP against these programs) -- test_icon_ipl_suite.sh's own RUN tier must use the
# identical isolation, not cd into $PKG/progs directly, or every future grading run re-corrupts the tree.
#
# ⛔ A .std MINTED FROM A RUN THAT ISN'T A GENUINE CLEAN EXECUTION PINS A LIE -- there are SIX ways to
# pin one here, not one, so every progs/ file gets exactly one NAMED outcome, never a silent skip:
#   EMPTY          -- rc=0, zero bytes of stdout. A 0-byte .std would grade "produced nothing" as correct.
#   ORACLE_FAIL    -- rc!=0 under /dev/null stdin -- needs argv/stdin this driver doesn't supply, or a
#                      genuine oracle-side rejection. Named, not retried with guesses.
#   TIMEOUT        -- exceeded $TIMEOUT (interactive read, or a genuinely long-running demo).
#   OVERSIZED      -- output exceeds $MAX_BYTES. Checked via `wc -c` on the FILE, before ever slurping
#                      it into a bash variable (second incident this session: a timing-out program that
#                      is NOT quiet while it waits can still print gigabytes before `timeout` kills it --
#                      see the MAX_BYTES comment below for the full incident). A program whose output
#                      doesn't fit in a small pinned .std is not a good ref candidate regardless.
#   SUSPECT_USAGE  -- rc=0, non-empty, but the first line reads like a usage/error banner printed on a
#                      clean exit (the exact DEAD_REPORT trap util_ref_mint.sh names for SNOBOL4's sbl,
#                      adapted for Icon: a program that prints "usage: prog file" and exits 0 would
#                      otherwise pin its own complaint as ground truth). Named for a human/HQ to eyeball,
#                      never silently absorbed into LIVE.
#   NONDETERMINISTIC -- run twice, independently (fresh scratch copy each time); the two stdouts disagree
#                      (time/date/rand/table-order etc.). Pinning either run would fail the OTHER forever.
#   LIVE           -- rc=0 both runs, non-empty, byte-identical across the two runs, no usage-banner
#                      shape. Minted as progs/NAME.std with --apply; otherwise "would mint".
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
S4E="${S4E_HOME:-$(cd "$HERE/../.." && pwd)}"
. "$HERE/lib_oracle_flags.sh" 2>/dev/null || { echo "⛔ GATE REFUSES: lib_oracle_flags.sh unloadable" >&2; exit 2; }
PKG="$S4E/corpus/packages/icon/ipl"
PROGS="$PKG/progs"
TIMEOUT="${TIMEOUT:-8}"
APPLY=""; VERBOSE=0
for a in "$@"; do case "$a" in --apply) APPLY=1;; -v) VERBOSE=1;; *) echo "usage: $0 [--apply] [-v]" >&2; exit 2;; esac; done

[ -d "$PROGS" ] || { echo "⛔ GATE REFUSES: corpus subtree missing: $PROGS" >&2; exit 2; }
ICON="$(icon_bin)" || exit 2

# ── ONE pristine template, copied ONCE; every invocation below gets its OWN disposable copy of it.
TEMPLATE="$(mktemp -d "${TMPDIR:-/tmp}/ipl_ref_template.XXXXXX")" || { echo "⛔ mktemp failed" >&2; exit 2; }
for sub in progs procs gprocs incl gincl; do [ -d "$PKG/$sub" ] && cp -r "$PKG/$sub" "$TEMPLATE/$sub"; done
cleanup_template() { rm -rf "$TEMPLATE"; }

# run_isolated <file.icn> <outfile> -> writes captured combined output to <outfile>, returns rc via
# the function's own exit status (NOT a global -- a global set inside a command-substitution call would
# be lost with the subshell, exactly the bug this replaced: `RC=$?` inside a `$(run_isolated ...)` call
# never reaches the caller). Fresh scratch copy every call, destroyed before returning -- see the header
# note on why cwd=$PROGS directly is unsafe here.
run_isolated() {
  local f="$1" outfile="$2" work rc
  work="$(mktemp -d "${TMPDIR:-/tmp}/ipl_ref_run.XXXXXX")" || return 127
  cp -r "$TEMPLATE"/. "$work"/
  ( cd "$work/progs" && timeout "$TIMEOUT" env ICONPATH="$work/progs:$work/procs:$work/gprocs:$work/incl:$work/gincl" "$ICON" "$f" < /dev/null > "$outfile" 2>&1 )
  rc=$?
  rm -rf "$work"
  return "$rc"
}

mapfile -t FILES < <(cd "$PROGS" && ls -1 *.icn | sort)
TOTAL=${#FILES[@]}
[ "$TOTAL" -gt 0 ] || { echo "⛔ GATE REFUSES: zero .icn files found under $PROGS" >&2; exit 2; }

n_live=0; n_mint=0; n_empty=0; n_fail=0; n_timeout=0; n_suspect=0; n_nondet=0; n_havestd=0; n_oversized=0
OUT1="$(mktemp "${TMPDIR:-/tmp}/ipl_ref_out1.XXXXXX")"; OUT2="$(mktemp "${TMPDIR:-/tmp}/ipl_ref_out2.XXXXXX")"
trap 'cleanup_template; rm -f "$OUT1" "$OUT2"' EXIT
# ⛔⛔ MAX_BYTES CAPS EVERY FULL-CONTENT READ, CHECKED ON THE FILE BEFORE EVER SLURPING IT INTO A BASH
# VARIABLE (found this session, the hard way -- second incident, same script): under /dev/null stdin a
# timing-out program is not necessarily QUIET while it waits -- one candidate (almost certainly
# noise.icn, matching its name and its TIMEOUT classification in the census) apparently printed as fast
# as it could for the full 8s before `timeout` killed it, and this script's PREVIOUS form did
# `out1="$(cat "$OUT1")"` UNCONDITIONALLY, before ever checking rc1 for a timeout -- slurping a
# many-GB file into a bash string. The process reached 8.7GB RSS with zero children and zero forward
# progress (confirmed via /proc: no fd open on the outfile, no subprocess, TIME frozen for 35+s) and had
# to be killed from outside -- on a shared 30G box with other seats actively running SCRIP concurrently
# at the time. Fix: `wc -c` the FILE first (cheap, no full read) and refuse to slurp past MAX_BYTES,
# same discipline as the EMPTY/SUSPECT_USAGE classes -- a program whose output doesn't fit in a small
# pinned .std is not a good ref candidate anyway, oversized or not.
MAX_BYTES=1048576
printf 'STATUS\tPROGRAM\tRC\tBYTES\tACTION\n'
for f in "${FILES[@]}"; do
  base="${f%.icn}"
  std="$PROGS/$base.std"
  if [ -f "$std" ]; then
    n_havestd=$((n_havestd+1)); printf 'HAVE_STD\t%s\t-\t-\tkept (pre-existing pin, never overwritten)\n' "$f"
    continue
  fi
  run_isolated "$f" "$OUT1"; rc1=$?
  if [ "$rc1" -eq 124 ]; then
    n_timeout=$((n_timeout+1)); printf 'TIMEOUT\t%s\t124\t-\tNOT MINTED -- exceeded %ss under /dev/null stdin\n' "$f" "$TIMEOUT"
    continue
  fi
  by1=$(wc -c < "$OUT1")
  if [ "$by1" -gt "$MAX_BYTES" ]; then
    n_oversized=$((n_oversized+1)); printf 'OVERSIZED\t%s\t%s\t%s\tNOT MINTED -- exceeds %s-byte cap, never slurped into memory\n' "$f" "$rc1" "$by1" "$MAX_BYTES"
    continue
  fi
  out1="$(cat "$OUT1")"
  if [ "$rc1" -ne 0 ]; then
    n_fail=$((n_fail+1)); printf 'ORACLE_FAIL\t%s\t%s\t-\tNOT MINTED -- needs argv/stdin this driver does not supply, or genuine rejection\n' "$f" "$rc1"
    [ "$VERBOSE" -eq 1 ] && printf '   %s\n' "$(printf '%s' "$out1" | head -1)"
    continue
  fi
  if [ "$by1" -eq 0 ]; then
    n_empty=$((n_empty+1)); printf 'EMPTY\t%s\t0\t0\tNOT MINTED -- rc=0, zero bytes; a 0-byte .std pins "produced nothing" as correct\n' "$f"
    continue
  fi
  if printf '%s' "$out1" | head -1 | grep -qiE '^(usage|error)[: ]'; then
    n_suspect=$((n_suspect+1)); printf 'SUSPECT_USAGE\t%s\t0\t%s\tNOT MINTED -- first line reads like a usage/error banner on a clean exit\n' "$f" "$by1"
    [ "$VERBOSE" -eq 1 ] && printf '   %s\n' "$(printf '%s' "$out1" | head -1)"
    continue
  fi
  # ── determinism check: THREE MORE independent runs (four total), fresh scratch copy each, ALL must
  # agree. ⛔⛔ TWO TOTAL RUNS IS NOT ENOUGH -- MEASURED (found this session): a follow-up 5-sample check
  # against this script's first 66-file output caught 6 programs (filexref/gcomp/puzz/qt/shar/solit)
  # that had passed a 2-run check by pure coincidence and then disagreed on 4/4 further independent runs
  # -- confirmed genuinely time/RNG-seeded, not a fluke (shar/filexref print Icon's &dateline verbatim;
  # solit explicitly seeds &random from &clock). A 2-sample match on a program whose nondeterminism rate
  # is low but nonzero is a real, non-rare false accept, not a theoretical one -- 6 of 66 is ~9%. Four
  # total agreeing runs does not make a false accept impossible, only proportionately rarer; there is no
  # bound past which it's guaranteed zero, same as util_ref_mint.sh accepts for SNOBOL4's LIVE class.
  for _confirm in 1 2 3; do
    run_isolated "$f" "$OUT2"; rc2=$?
    if [ "$rc2" -eq 124 ]; then
      n_nondet=$((n_nondet+1)); printf 'NONDETERMINISTIC\t%s\t%s/124\t-\tNOT MINTED -- a confirmation run timed out where the first did not\n' "$f" "$rc1"
      continue 2
    fi
    by2=$(wc -c < "$OUT2")
    if [ "$by2" -gt "$MAX_BYTES" ]; then
      n_oversized=$((n_oversized+1)); printf 'OVERSIZED\t%s\t%s\t%s\tNOT MINTED -- a confirmation run exceeds %s-byte cap, never slurped into memory\n' "$f" "$rc2" "$by2" "$MAX_BYTES"
      continue 2
    fi
    out2="$(cat "$OUT2")"
    if [ "$rc2" -ne "$rc1" ] || [ "$out1" != "$out2" ]; then
      n_nondet=$((n_nondet+1)); printf 'NONDETERMINISTIC\t%s\t%s/%s\t-\tNOT MINTED -- disagreed on confirmation run %s/3\n' "$f" "$rc1" "$rc2" "$_confirm"
      continue 2
    fi
  done
  n_live=$((n_live+1))
  if [ -n "$APPLY" ]; then
    printf '%s' "$out1" > "$std"; n_mint=$((n_mint+1)); act="MINTED"
  else
    act="would mint"
  fi
  printf 'LIVE\t%s\t0\t%s\t%s\n' "$f" "$by1" "$act"
done
echo "----"
printf 'TOTALS: LIVE %d (minted %d) · HAVE_STD %d · EMPTY %d · SUSPECT_USAGE %d · NONDETERMINISTIC %d · ORACLE_FAIL %d · TIMEOUT %d · OVERSIZED %d · total=%d\n' \
  "$n_live" "$n_mint" "$n_havestd" "$n_empty" "$n_suspect" "$n_nondet" "$n_fail" "$n_timeout" "$n_oversized" "$TOTAL"
[ -n "$APPLY" ] || echo "(census only -- nothing written; re-run with --apply to mint)"
# ── belt-and-suspenders: prove the tracked tree is still exactly what HEAD says, every run, census or not.
if git -C "$S4E/corpus" diff --quiet -- packages/icon/ipl/progs packages/icon/ipl/procs packages/icon/ipl/gprocs packages/icon/ipl/incl packages/icon/ipl/gincl 2>/dev/null \
   && [ -z "$(git -C "$S4E/corpus" status --porcelain -- packages/icon/ipl/progs packages/icon/ipl/procs packages/icon/ipl/gprocs packages/icon/ipl/incl packages/icon/ipl/gincl 2>/dev/null | grep -v '\.std$')" ]; then
  :
else
  echo "⛔⛔⛔ THE TRACKED IPL TREE CHANGED DURING THIS RUN (excluding new .std mints) -- isolation was breached, investigate before trusting anything above ⛔⛔⛔" >&2
fi
