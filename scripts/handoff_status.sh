#!/usr/bin/env bash
# handoff_status.sh — THE ONLY SANCTIONED SOURCE OF A CHAT SESSION COMPLETION CLAIM.
#
# Per RULES.md FACT RULE (2026-06-24): the assistant MUST NOT compose "CHAT SESSION COMPLETE"
# as prose. It runs THIS script and presents the verbatim output.
#
# It does NOT hardcode a repo count. With no args it DISCOVERS every git repo that has an
# 'origin' remote directly under the workspace root (the parent of the SCRIP repo this
# script lives in) — so it can never miss a touched repo, and it prints how many it found.
# Pass explicit repo dirs to override. Reading origin needs no credential; only the push that
# PRECEDES this check does.
#
# ⭐ THREE-STATE VERDICT (instrument-repair-bundle PART 2, s273; precedent copied verbatim from
# test_corpus_snobol4.sh's rc=0/1/2 shape, SCRIP 9873fe6e): "I cannot honestly tell" is not the
# same answer as either confident one, and collapsing it into BLOCKED (or worse, COMPLETE) is how
# an unreachable remote or an unreadable repo gets misread as a real verdict.
#   exit 0  CHAT SESSION COMPLETE — every repo examined, none blocked, none unknown.
#   exit 1  CHAT SESSION WAITING  — a repo is DIRTY, UNPUSHED, or DIVERGED. Known, actionable.
#   exit 2  CHAT SESSION REFUSES  — a repo's push-state could not be determined (fetch failed,
#           unreadable) or there was nothing to examine at all. NOT a pass; read the reason(s).
#
# ⭐ PER-REPO STATE, three-way (adopted from tasks/handoff-status-three-state-push-check.task.md,
# hq_P s269, FINDING-2026-08-24-hq_P-three-instruments-that-cannot-express-their-own-outcome.md
# §1): the old script printed one word, UNPUSHED, for BOTH "your work is missing from origin,
# nothing of yours at risk" and "you have local commits origin doesn't." A seat reading the label
# (not the count beside it) concluded it had LOST work it had not — exactly the panic that tempts
# `reset --hard`/`push --force`, the two actions THE LOOP §3b most wants to prevent.
#   SYNCED   — local HEAD == origin/<branch>. Nothing to do.
#   BEHIND   — HEAD is an ancestor of origin (all of ours is already there; we're missing some of
#              theirs). NOT a failure of this session's own work — does not block.
#   UNPUSHED — origin is an ancestor of HEAD (we have commits origin doesn't). Blocks.
#   DIVERGED — neither is an ancestor of the other (both sides moved). Blocks — and a plain push
#              will be REJECTED here, so the reason says "pull --rebase", never "push".
# ⛔ FETCH-IS-NOT-CHECKOUT, both directions: the remote ref must be FRESH before any ancestry
# check runs, or a stale origin/<branch> makes the test confidently wrong in whichever direction
# the staleness happens to run. A fetch failure is therefore its own UNKNOWN state, never silently
# read as any of the four above.
set -uo pipefail
SELF_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"   # <ws>/SCRIP/scripts
WS="$(dirname "$(dirname "$SELF_DIR")")"                    # <ws> (parent of the SCRIP repo)
REPOS=("$@") ; src="given on command line"
if [ ${#REPOS[@]} -eq 0 ]; then
  src="auto-discovered under $WS"
  shopt -s nullglob dotglob
  for d in "$WS"/*/; do
    d="${d%/}"; base="$(basename "$d")"
    { [ "$base" = "." ] || [ "$base" = ".." ]; } && continue
    [ -d "$d/.git" ] || continue                              # is a git repo
    git -C "$d" remote get-url origin >/dev/null 2>&1 || continue   # has an origin to push to
    REPOS+=("$d")
  done
  IFS=$'\n' REPOS=($(printf '%s\n' "${REPOS[@]}" | sort)); unset IFS
fi
echo "=== CHAT SESSION STATUS — checking ${#REPOS[@]} repo(s) ($src); ground truth from git ==="
# ⛔ ZERO-EXAMINED IS NOT A PASS (same law as lib_gate.sh's gate_floor: "zero-work-examined is
# indistinguishable from all-clean, so it is refused rather than passed"). An empty workspace used
# to read WAITING (rc=1) — a mislabel in the OTHER direction: there is nothing here to be blocked
# ON, the honest answer is "could not examine anything."
if [ ${#REPOS[@]} -eq 0 ]; then
  echo "⛔ CHAT SESSION REFUSES — no git repos with an 'origin' remote found under $WS"
  echo "   This is NOT complete and NOT a known block: there was nothing to examine. Check WS/S4E_HOME."
  exit 2
fi
blocked=0; unknown=0; reasons=(); unknown_reasons=()
for r in "${REPOS[@]}"; do
  name=$(basename "$r")
  br=$(git -C "$r" rev-parse --abbrev-ref HEAD 2>/dev/null)
  dirty=$(git -C "$r" status --porcelain 2>/dev/null | wc -l)
  if ! git -C "$r" fetch --quiet origin 2>/dev/null; then
    unknown_reasons+=("$name: fetch failed — origin unreachable or no credential; push-state UNKNOWABLE from here, not assumed BLOCKED or COMPLETE")
    unknown=1
    printf "  %-22s %-10s local=%s origin=%s\n" "$name [$br]" "UNKNOWN" "${dirty}dirty" "fetch-failed"
    continue
  fi
  lh=$(git -C "$r" rev-parse HEAD 2>/dev/null)
  oh=$(git -C "$r" rev-parse "origin/$br" 2>/dev/null || echo MISSING)
  st="SYNCED"
  if [ "$dirty" -ne 0 ]; then st="DIRTY"; fi
  if [ "$oh" = "MISSING" ]; then
    # origin has no such branch at all -- everything local is, by definition, unpushed.
    [ "$st" = "DIRTY" ] || st="UNPUSHED"
    reasons+=("$name: origin has no branch '$br' — git push -u origin $br")
    blocked=1
  elif [ "$lh" != "$oh" ]; then
    fwd=0; git -C "$r" merge-base --is-ancestor "$lh" "$oh" 2>/dev/null && fwd=1
    back=0; git -C "$r" merge-base --is-ancestor "$oh" "$lh" 2>/dev/null && back=1
    if [ "$fwd" -eq 1 ]; then
      [ "$st" = "DIRTY" ] || st="BEHIND"   # all of OURS is on origin; nothing at risk, does not block
    elif [ "$back" -eq 1 ]; then
      [ "$st" = "DIRTY" ] || st="UNPUSHED"
      reasons+=("$name: $(git -C "$r" rev-list --count "$oh..$lh" 2>/dev/null || echo '?') commit(s) not on origin/$br — git push")
      blocked=1
    else
      [ "$st" = "DIRTY" ] || st="DIVERGED"
      reasons+=("$name: DIVERGED from origin/$br — both sides have unique commits, a plain push will be REJECTED — git pull --rebase && git push")
      blocked=1
    fi
  fi
  if [ "$dirty" -ne 0 ]; then reasons+=("$name: $dirty uncommitted change(s) — commit them"); blocked=1; fi
  printf "  %-22s %-10s local=%s origin=%s\n" "$name [$br]" "$st" "${lh:0:9}" "${oh:0:9}"
done
echo "------------------------------------------------------------"
echo "=== .s ARTIFACT DRIFT (RULES.md handoff step 4) — BLOCKING since 2026-08-30 (ceo: the s272 ramp condition was met — hq_B drove the standing owed count to 0 and confirmed via this script) ==="
echo "    (owed artifacts now BLOCK the handoff verdict; a verifier REFUSAL (rc=2, cannot measure) still only warns. If blocked and the debt is not yours: run the three regen scripts named in RULES.md handoff step 4 — earlier sessions' unregenerated codegen surfaces on whoever handoffs next, which is correct, not an accusation.)"
if [ "${SKIP_S_ARTIFACT_CHECK:-0}" = "1" ]; then
  echo "  SKIPPED (SKIP_S_ARTIFACT_CHECK=1 set) — .s drift is UNVERIFIED this run."
else
  verifier="$SELF_DIR/util_verify_s_artifacts_owed.sh"
  if [ ! -x "$verifier" ]; then
    echo "  ⛔ $verifier missing/not executable — .s drift is UNVERIFIED this run."
  else
    # ⛔⭐⭐ THIS WARN-ONLY CHECK MUST NOT RUN A DESTRUCTIVE BUILD (row `stop-hook-pristine`, hq_P s274; ceo CEO-30).
    # Until now this line invoked the verifier with NO ARGUMENTS, so it defaulted to SKIP_PRISTINE=0 and ran a full
    # `make pristine` in the LIVE checkout -- wiping ./scrip and out/ -- to feed a block whose own header three lines
    # up says "WARN-ONLY, does not affect the verdict below". The most destructive operation in the whole chain was
    # serving the one check that is explicitly non-binding.
    # ⛔ AND IT FIRES FAR MORE OFTEN THAN "AT HANDOFF": the seat's `Stop` hook runs `s4e_msg.sh banner` -> here, and
    # Stop fires every time the seat RESPONDS, not only at session end. So a seat that starts a long build and then
    # answers a message destroys its own build, with no build of its own visibly running. That is why it read as a
    # haunting rather than a race: MEASURED 4 binary losses across 3 roots in one day (hq_P x2, hq_C x2), each first
    # reported as "the binary vanished and no make of mine was running". seat09 diagnosed it after 4 collisions in a
    # single session and "waited 5 min for a clear window, found none" -- there is no window, because the thing
    # closing it is the seat itself. Symptom: `ld: cannot find out/rt_pic-<hash>/*.o`, then collect2 error, Makefile:406.
    # ⭐ LAW COVER, so nobody re-arms this citing HQ-27: HQ-27 owes a pristine build before a VERDICT, never before a
    # BANNER. The hook OVER-APPLIED it; no law changed. A real gate/handoff verdict still gets its pristine.
    # ⭐ SAFE BY CONSTRUCTION, NOT BY HOPE: under --skip-pristine the verifier REFUSES rc=2 if ./scrip is absent
    # ("FATAL: scrip not built") -- so this can degrade to a REFUSAL, never to a false CLEAN.
    # ✅ Opt back in deliberately with S_ARTIFACT_PRISTINE=1 when you actually want the pristine-verified drift check.
    verifier_args=""
    [ "${S_ARTIFACT_PRISTINE:-0}" = "1" ] || verifier_args="--skip-pristine"
    s_out="$(bash "$verifier" $verifier_args 2>&1)"; s_rc=$?
    if [ "$s_rc" -eq 0 ]; then
      printf '%s\n' "$s_out" | grep '^S-ARTIFACTS-' | sed 's/^/  /'
      echo "  CLEAN — benchmark/demo/prolog_bench/icon_bench all current, all checks actually ran."
    elif [ "$s_rc" -eq 2 ]; then
      echo "  ⛔ UNVERIFIED — the verifier REFUSED (rc=2, cannot measure; e.g. no built ./scrip). Not blocking, but this run proves nothing about .s drift."
      printf '%s\n' "$s_out" | sed -n '/^VERDICT:/,$p' | sed 's/^/  /'
    else
      echo "  ⛔⛔⛔ OWED — BLOCKS THE HANDOFF (ramp flipped 2026-08-30, condition met at standing count 0). Run the regen scripts; see the header note if the debt predates your session."
      printf '%s\n' "$s_out" | sed -n '/^VERDICT:/,$p' | sed 's/^/  /'
      blocked=1; reasons+=(".s artifacts OWED — regenerate (RULES.md handoff step 4); util_verify_s_artifacts_owed.sh rc=$s_rc")
    fi
  fi
fi
echo "------------------------------------------------------------"
echo "THE ONE LEADERBOARD (.github/SCORE.md) — row staleness — WARN-ONLY, does not affect the verdict below"
# ⛔⭐ WARN-ONLY ON PURPOSE, AND THE REASON IS THE SAME ONE THE .s BLOCK LEARNED ABOVE. A stale SCORE.md row is
# not a defect of the session that happens to be handing off -- it is usually a defect of some OTHER session
# that measured a suite and did not rewrite its row. Blocking this seat's handoff on that would punish the
# wrong session and, within a week, get this check `|| true`-d out of the file. What it CAN honestly do is
# make the debt visible at the one moment somebody is looking at the state of the world.
# ⛔ It runs `check`, which reads git and the markdown ONLY -- it runs no suite and builds nothing, so unlike
# the .s verifier above it cannot destroy a live build (the haunting documented at length in that block).
# ⭐ rc=1 means "rows ARE stale" -- a real answer, printed. rc>=2 means it could not measure, which is its own
# state and says so; neither is allowed to read as CLEAN.
_score_helper="$SELF_DIR/util_score_row.py"
if [ ! -f "$_score_helper" ]; then
  echo "  ⛔ $_score_helper missing — leaderboard staleness UNVERIFIED this run."
else
  _score_out="$(python3 "$_score_helper" check 2>&1)"; _score_rc=$?
  if [ "$_score_rc" -eq 0 ]; then
    echo "  CLEAN — every row measured within 25 commits of origin/main."
  elif [ "$_score_rc" -eq 1 ]; then
    printf '%s\n' "$_score_out" | grep -E 'STALE|UNPINNED|UNKNOWN|^worst=' | sed 's/^/  /'
    echo "  ⚠ WARN — re-measure the stale suite(s) and rewrite the row: python3 scripts/util_score_row.py write --lang <lang> --column board --measurer \"\$S4E_SEAT\" --text '<board line>'"
  else
    echo "  ⛔ UNVERIFIED — the staleness check REFUSED (rc=$_score_rc, cannot measure). Not blocking, but this run proves nothing about the leaderboard."
    printf '%s\n' "$_score_out" | sed 's/^/  /'
  fi
fi
echo "------------------------------------------------------------"
if [ "$blocked" -ne 0 ]; then
  echo "CHAT SESSION WAITING — not done:"; printf '  - %s\n' "${reasons[@]}"
  [ "$unknown" -ne 0 ] && { echo "  (also UNKNOWN, see below — fix the known blockers first, they are certain; the unknown repo(s) still need a look)"; printf '  - %s\n' "${unknown_reasons[@]}"; }
  exit 1
elif [ "$unknown" -ne 0 ]; then
  echo "⛔ CHAT SESSION REFUSES — cannot honestly say COMPLETE or WAITING:"; printf '  - %s\n' "${unknown_reasons[@]}"
  echo "   No repo is a KNOWN blocker, but at least one could not be examined. This is NOT a pass."
  exit 2
else
  echo "CHAT SESSION COMPLETE"; exit 0
fi
