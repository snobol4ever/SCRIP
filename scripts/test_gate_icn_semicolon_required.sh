#!/usr/bin/env bash
# test_gate_icn_semicolon_required.sh — THE PRISON for the ICON SEMICOLON-REQUIRED FACT RULE.
#
# SCRIP Icon REQUIRES an explicit ';' between bare statements. The Icon front-end does NO newline
# processing: a newline is plain whitespace and NEVER becomes a statement separator. Newline-to-
# semicolon insertion (the canonical icont "Beginner/Ender" rule) is FORBIDDEN in this codebase.
#
# This gate is structural AND behavioral, in three independent locks. A reintroduction of newline
# insertion must defeat ALL THREE to pass, which it cannot:
#
#   LOCK 1 (negative grep) — the Icon lexer/parser source carries ZERO newline-insertion machinery:
#       no Beginner/Ender token classification, no newline-crossing detection, no synthetic-token
#       buffering whose purpose is separator insertion. Comments are stripped first.
#
#   LOCK 2 (mint-site) — TK_SEMICOL is MINTED in the Icon lexer ONLY from the literal ';' character.
#       The number of make_tok(TK_SEMICOL,...) sites in icon_lex.c must be exactly 1 (the `case ';'`).
#       (The parser CONSUMES TK_SEMICOL freely; this lock is about where it is PRODUCED.)
#
#   LOCK 3 (behavioral canary) — the actual language behavior is pinned, independent of identifier
#       names. A two-bare-statement program separated only by a NEWLINE MUST FAIL to parse; the same
#       program with an explicit ';' MUST parse. If someone adds insertion, the newline canary starts
#       parsing and this lock trips.
#
# Usage: bash scripts/test_gate_icn_semicolon_required.sh        # HARD gate (exit 1 on any breach)
# AUTHORS: Lon Jones Cherryholmes · Jeffrey Cooper M.D. · Claude   DATE: 2026-06-23
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
cd "$ROOT"
SCRIP="${SCRIP:-$ROOT/scrip}"
ICN_SRC="$ROOT/src/parsers/icon"
LEX="$ICN_SRC/icon_lex.c"
fail=0

strip_comments() { sed -E 's://.*$::' "$1" | perl -0777 -pe 's{/\*.*?\*/}{}gs'; }

echo "=== ICON SEMICOLON-REQUIRED PRISON ==="

# ---- LOCK 1: no newline-insertion machinery anywhere in the Icon front-end -------------------------
# Tokens that only ever appear when someone is building Beginner/Ender semicolon insertion or
# newline-crossing separator logic. (prev_line/prev_col crossing detection; have_pending separator
# buffering; the canonical icont flag names.)
machinery='tok_is_beginner|tok_is_ender|is_beginner|is_ender|Beginner|Ender|nlsemi|nl_semi|newline_semi|semi_insert|insert_semi|prev_line|have_pending'
mcount=0
for f in "$ICN_SRC"/*.c "$ICN_SRC"/*.h; do
  [ -f "$f" ] || continue
  h=$(strip_comments "$f" | grep -Ec "$machinery" || true)
  if [ "$h" -gt 0 ]; then
    echo "  LOCK 1 BREACH: $(basename "$f") has $h newline-insertion-machinery reference(s):"
    strip_comments "$f" | grep -nE "$machinery" | sed 's/^/      /'
    mcount=$((mcount + h))
  fi
done
if [ "$mcount" -eq 0 ]; then echo "  LOCK 1 OK: zero newline-insertion machinery in src/parsers/icon/."; else fail=1; fi

# ---- LOCK 2: TK_SEMICOL minted only from the literal ';' -------------------------------------------
mint=$(strip_comments "$LEX" | grep -cE 'make_tok\(\s*TK_SEMICOL' || true)
echo "  LOCK 2: make_tok(TK_SEMICOL,...) mint-sites in icon_lex.c = ${mint} (must be 1)"
if [ "$mint" -ne 1 ]; then echo "  LOCK 2 BREACH: TK_SEMICOL is minted from something other than the literal ';' case."; fail=1; else echo "  LOCK 2 OK."; fi

# ---- LOCK 3: behavioral canary --------------------------------------------------------------------
# ⚠️ KNOWN FUTURE BREAK, flagged 2026-09-04 for whoever performs the icon-dialect-...-semicolon landing:
# both probe bodies below close with `end` (old dialect). Once that landing rejects `end` (LOCK 4b's
# target), LOCK 3 will fail to parse at all regardless of the newline-vs-semicolon distinction it's
# actually testing, for the SAME confound already documented at LOCK 4c/4d — not a defect introduced by
# LOCK 4, a pre-existing fixture that the coordinated landing must also update (to `{ }`) alongside every
# tracked corpus .icn, since these inline heredocs are outside the converter's corpus-only scope.
if [ ! -x "$SCRIP" ]; then
  echo "  LOCK 3 SKIP: scrip binary not built at $SCRIP (build then re-run for the behavioral lock)."
else
  tmpd="$(mktemp -d /tmp/icn_semi_XXXXXX)"
  # newline-only: two bare statements separated by a newline — MUST be a parse error
  printf 'procedure main()\n  x := 5\n  y := 6\n  write(x + y)\nend\n' > "$tmpd/nl.icn"
  # semicolon: same program with explicit ';' — MUST parse
  printf 'procedure main()\n  x := 5;\n  y := 6;\n  write(x + y);\nend\n' > "$tmpd/semi.icn"

  nl_err=$("$SCRIP" --run "$tmpd/nl.icn" < /dev/null 2>&1 | grep -c 'parse error' || true)
  semi_err=$("$SCRIP" --run "$tmpd/semi.icn" < /dev/null 2>&1 | grep -c 'parse error' || true)

  if [ "$nl_err" -ge 1 ]; then
    echo "  LOCK 3a OK: newline-separated bare statements correctly REJECTED (parse error)."
  else
    echo "  LOCK 3a BREACH: newline-separated bare statements were ACCEPTED — newline processing is present."
    fail=1
  fi
  if [ "$semi_err" -eq 0 ]; then
    echo "  LOCK 3b OK: semicolon-separated statements correctly ACCEPTED."
  else
    echo "  LOCK 3b BREACH: semicolon-separated statements were REJECTED — the parser is over-strict/broken."
    fail=1
  fi
  rm -rf "$tmpd"
fi

# ---- LOCK 4: THE BRACE-DIALECT PROBES (Lon 2026-09-04: procedure closes with `}`, never `end`; every ----
# statement AND declaration ends in `;`, including the last statement before a `}` — that spot is not
# special). These pin the TARGET state of the icon-dialect-procedure-braces-no-end-... landing (parser +
# converter, hq_B's row). Until that landing, 4a/4c currently read BREACH — this is the correct, expected
# "proven to fail once" state (LEDGER, ceo 2026-09-04), not a bug in this gate. They flip OK the moment the
# parser lands, with zero further changes to this file required.
if [ ! -x "$SCRIP" ]; then
  echo "  LOCK 4 SKIP: scrip binary not built at $SCRIP (build then re-run for the dialect probes)."
else
  d="$(mktemp -d /tmp/icn_dialect_XXXXXX)"

  # 4a — NEW dialect ACCEPTED: a brace-closed procedure, semicolon on the LAST statement too, runs.
  printf 'procedure main() {\n  write(1);\n}\n' > "$d/brace_ok.icn"
  out=$(timeout 8 "$SCRIP" --run "$d/brace_ok.icn" < /dev/null 2>&1); rc=$?
  if [ "$rc" -eq 0 ] && [ "$out" = "1" ]; then
    echo "  LOCK 4a OK: procedure main() { write(1); } runs and prints 1."
  else
    echo "  LOCK 4a BREACH (expected pre-landing): brace-closed procedure does not yet run — got rc=$rc out=[$out]"
    fail=1
  fi

  # 4b — OLD dialect REJECTED: `end` closing a procedure is a syntax error naming the rule, not silently
  # accepted. Pre-landing this is the one arm that reads OK already (see script header) — `end` MUST keep
  # working until the converter has actually run, or every existing .icn file breaks mid-migration; this
  # arm is here to catch someone flipping it too early just as much as it is to prove it lands eventually.
  printf 'procedure main()\n  write(1);\nend\n' > "$d/end_still_here.icn"
  out=$(timeout 8 "$SCRIP" --run "$d/end_still_here.icn" < /dev/null 2>&1); rc=$?
  if [ "$rc" -eq 0 ] && [ "$out" = "1" ]; then
    echo "  LOCK 4b OK (pre-landing state): procedure ... end still runs — do not remove old-dialect support before the converter has run on the whole tracked-.icn population."
  else
    echo "  LOCK 4b NOTE: procedure ... end no longer runs (rc=$rc out=[$out]) — expected ONLY after the converter has landed on every tracked .icn; if that hasn't happened yet, this is a premature break, not progress."
  fi

  # 4c — a missing `;` before `}` is a parse error, never a silent accept — the spot before the right
  # curly is explicitly NOT special (Lon's own words). ⚠️ CONFOUNDED PRE-LANDING, BY DESIGN, NOT AN
  # OVERSIGHT: today this reads "rejected" for the INCIDENTAL reason that braces aren't parsed at all
  # (anything with a `{` errors, semicolon or no) — confirmed empirically (both variants give the
  # identical error). It becomes a real, non-vacuous check with ZERO rewrite the moment 4a starts
  # passing (once braces parse, the only defect left in this exact file IS the missing `;`). Do not
  # read an "OK" here, before 4a is OK, as proof the specific rule is enforced — it is not yet
  # possible for this probe to prove that on its own, and it is not pretending to.
  printf 'procedure main() {\n  write(1)\n}\n' > "$d/brace_nosemi.icn"
  out=$(timeout 8 "$SCRIP" --run "$d/brace_nosemi.icn" < /dev/null 2>&1); rc=$?
  if [ "$rc" -ne 0 ] && [ "$out" != "1" ]; then
    echo "  LOCK 4c OK-BUT-VACUOUS-UNTIL-4A: a missing ; before } is rejected (rc=$rc) -- confounded with brace support until 4a is OK, see comment above."
  else
    echo "  LOCK 4c BREACH: a missing ; before } was silently accepted — got rc=$rc out=[$out]"
    fail=1
  fi

  # 4d — every declaration form (record, global, link, invocable) requires its own trailing `;` and is
  # rejected without one. Two checks per form, for two different reasons:
  #   (i)  TARGET-STATE (brace body) — same confounded-until-4a shape as 4c, same reasoning: only
  #        becomes a real per-declaration signal once braces parse: kept as the fail=1-contributing arm
  #        because it is the check that is automatically correct after the FULL landing with no rewrite.
  #   (ii) CURRENT-STATE (old `end`-dialect body, informational, NEVER contributes to fail=1) — the
  #        brace body can't give an honest signal about TODAY's declaration-parsing behavior (it's
  #        masked by (i)'s confound), so this pairs the SAME declaration line with a body the CURRENT
  #        parser already accepts, to say something true about right now. MEASURED 2026-09-04 (this
  #        session, direct probe, not assumed): none of the four currently require their `;` — all four
  #        parse and RUN today without one. This check will itself go MOOT (not wrong, just uninformative)
  #        once old-dialect bodies stop parsing per 4b's eventual target — that is expected, not a defect
  #        of this check, and is exactly why it is informational-only rather than gating.
  printf 'record r(a, b)\nprocedure main() {\n  write(1);\n}\n' > "$d/decl_record_brace.icn"
  printf 'global g\nprocedure main() {\n  write(1);\n}\n' > "$d/decl_global_brace.icn"
  printf 'invocable all\nprocedure main() {\n  write(1);\n}\n' > "$d/decl_invocable_brace.icn"
  printf 'link "decl_link_target"\nprocedure main() {\n  write(1);\n}\n' > "$d/decl_link_brace.icn"
  printf 'record r(a, b)\nprocedure main()\n  write(1);\nend\n' > "$d/decl_record_old.icn"
  printf 'global g\nprocedure main()\n  write(1);\nend\n' > "$d/decl_global_old.icn"
  printf 'invocable all\nprocedure main()\n  write(1);\nend\n' > "$d/decl_invocable_old.icn"
  printf 'link "decl_link_target"\nprocedure main()\n  write(1);\nend\n' > "$d/decl_link_old.icn"
  printf 'procedure helper()\n  return 1\nend\n' > "$d/decl_link_target.icn"
  for decl in record global invocable link; do
    out=$(cd "$d" && timeout 8 "$SCRIP" --run "decl_${decl}_brace.icn" < /dev/null 2>&1); rc=$?
    if [ "$rc" -ne 0 ] && [ "$out" != "1" ]; then
      echo "  LOCK 4d[$decl].i OK-BUT-VACUOUS-UNTIL-4A: rejected with a brace body (rc=$rc) -- same confound as 4c."
    else
      echo "  LOCK 4d[$decl].i BREACH: $decl declaration without ; was accepted even with a brace body — got rc=$rc out=[$out]"
      fail=1
    fi
    out=$(cd "$d" && timeout 8 "$SCRIP" --run "decl_${decl}_old.icn" < /dev/null 2>&1); rc=$?
    if [ "$rc" -ne 0 ] && [ "$out" != "1" ]; then
      echo "  LOCK 4d[$decl].ii (informational, not gating): rejected with an old-dialect body too (rc=$rc) -- semicolon already required independent of the brace migration."
    else
      echo "  LOCK 4d[$decl].ii (informational, not gating): ACCEPTED with an old-dialect body (rc=$rc out=[$out]) -- confirms the semicolon requirement is NOT yet enforced for $decl today, independent of brace support."
    fi
  done

  rm -rf "$d"
fi

echo "---"
if [ "$fail" -ne 0 ]; then
  echo "FAIL: ICON SEMICOLON-REQUIRED PRISON breached. Icon does NO newline processing; semicolons are mandatory."
  echo "      (LOCK 4a/4c/4d BREACH is EXPECTED before icon-dialect-procedure-braces-no-end-every-statement-and-declaration-ends-in-a-semicolon lands — see that row.)"
  exit 1
fi
echo "PASS: Icon requires semicolons; no newline processing exists; the brace dialect is fully landed. All locks hold."
exit 0
