#!/usr/bin/env bash
# util_regen_feature_s_artifacts.sh — regenerate the mode-4 (--compile) x86 .s
# artifact for every SNOBOL4 FEATURE test, placed side-by-side with its .sno source
# under test/snobol4/ (in the SCRIP repo), and commit ONLY the artifacts whose
# bytes actually changed.
#
# Run on any session that touches codegen, i.e. any of:
#   src/emitter/emit_bb.c, src/emitter/emit_core.c, src/templates/*.cpp,
#   src/templates/*.cpp, src/emitter/x86_asm.h, src/lower/lower_snobol4.c,
#   or the runtime sinks they call. (Sibling of util_regen_benchmark_s_artifacts.sh /
#   util_regen_demo_s_artifacts.sh, which cover the corpus benchmark + demo trees.)
#
# PHILOSOPHY — the .s is the HONEST CURRENT compiler output, NOT a pinned golden:
#   * A feature whose codegen still bombs an unimplemented shape (very common while a
#     BB family is mid-design) emits a loud bomb stub INTO the .s — it still assembles,
#     so the .s is updated and committed. When the gap closes the .s changes and is
#     re-committed. This is expected and correct: the artifact tracks reality.
#   * scrip --compile is deterministic, so re-running with an unchanged compiler
#     produces byte-identical .s and yields NO commit (idempotent).
#   * This script NEVER enforces .s sameness and is NOT a pass/fail gate. It only
#     refreshes the committed snapshot to match current output. Do not wire it into a
#     byte-identity gate — that would fight the design churn it exists to track.
#   * Only artifacts that the assembler accepts are written; a .s the assembler rejects
#     (a genuinely malformed emission, distinct from an assembling bomb stub) is left
#     untouched and flagged, so a real codegen break is visible without clobbering the
#     last-good snapshot.
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
set -euo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP:-$ROOT/scrip}"
FEAT="${FEAT_DIR:-$ROOT/test/snobol4}"
CORPUS="${CORPUS:-$S4E/corpus}"
# -include 'lib/*.sno' in test/snobol4/library/ resolves against the CORPUS root, not SCRIP; compiling from $ROOT made 4 artifacts EMIT-FAIL and silently freeze at s189 through every regen since.
INCROOT="$([ -d "$CORPUS/library" ] && echo "$CORPUS" || echo "$ROOT")"

if [ ! -x "$SCRIP" ]; then echo "SKIP  scrip not built: $SCRIP"; exit 0; fi
if [ ! -d "$FEAT" ]; then echo "SKIP  feature test dir not found: $FEAT"; exit 0; fi
TMPD="$(mktemp -d)"; trap 'rm -rf "$TMPD"' EXIT   # PER-INVOCATION scratch: /tmp/feat_<base>.* is test-keyed, so two seat roots regenerating at once collide on EVERY test (s169 seat2 FINDING §7.5)

fail=0
changed=0
echo "Emitting feature .s artifacts (mode-4 --compile) under ${FEAT#$ROOT/} ..."
while IFS= read -r sno; do
    s="${sno%.sno}.s"
    base="$(basename "${sno%.sno}")"
    if ( cd "$INCROOT" && timeout 30 "$SCRIP" --compile "$sno" ) > "$TMPD/feat_$base.s" 2>/dev/null < /dev/null && [ -s "$TMPD/feat_$base.s" ]; then
        if gcc -c "$TMPD/feat_$base.s" -o "$TMPD/feat_$base.o" 2>"$TMPD/feat_$base.aserr"; then
            if [ ! -f "$s" ] || ! diff -q "$TMPD/feat_$base.s" "$s" >/dev/null 2>&1; then
                mv "$TMPD/feat_$base.s" "$s"; changed=$((changed+1)); echo "  UPD   ${s#$ROOT/}"
            fi
        else
            echo "  AS-FAIL ${s#$ROOT/} (assembler rejected; NOT updated): $(head -1 "$TMPD/feat_$base.aserr")"
            fail=1
        fi
    else
        echo "  EMIT-FAIL ${s#$ROOT/} (empty/crash; NOT updated)"
        fail=1
    fi
done < <(find "$FEAT" -name '*.sno' | sort)

echo "Committing changed feature artifacts to SCRIP repo... ($changed changed)"
cd "$ROOT"
git add test/snobol4 2>/dev/null || true
if git diff --cached --quiet -- test/snobol4 2>/dev/null; then
    echo "  No changes — feature .s artifacts already current."
else
    RUNG="${1:-regen}"
    git -c user.name="LCherryholmes" -c user.email="lcherryh@yahoo.com" \
        commit -q -m "feature x86 .s artifacts: $RUNG" -- test/snobol4
    echo "  Committed:"; git show --stat --oneline HEAD | sed -n '1,60p'
fi

echo "Done.$([ "$fail" -ne 0 ] && echo '  (one or more feature tests did not produce a clean .s — see above; expected for mid-design BB shapes.)')"
exit 0
