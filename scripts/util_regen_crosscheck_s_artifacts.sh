#!/usr/bin/env bash
# util_regen_crosscheck_s_artifacts.sh — regenerate the mode-4 (--compile) x86 .s
# artifact for every crosscheck program (corpus/crosscheck/**), side-by-side with its
# source (.sno or .sc), committing ONLY artifacts whose bytes changed.
#
# WHY THIS EXISTS (s21x provenance census): all 180 committed crosscheck .s were from
# `scrip-cc -asm` — a tool that no longer exists, linking a dead external GC removed at
# GC-U-4.  Obsolete artifacts masquerade as current codegen and mislead any census
# that reads them (the exact confusion this session's rbp census hit).  Sibling of
# util_regen_benchmark_s_artifacts.sh; same PHILOSOPHY (see util_regen_feature_...):
# the .s is the HONEST CURRENT compiler output, NOT a pinned golden.  scrip --compile
# is deterministic, so an unchanged compiler yields byte-identical .s and NO commit.
# NOT a pass/fail gate; never wire into a byte-identity check.
# .j/.il/.wat/.js siblings are dormant-backend artifacts (X86-ONLY) — untouched here.
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP:-$ROOT/scrip}"
CORPUS="${CORPUS:-$S4E/corpus}"
CC="$CORPUS/crosscheck"

if [ ! -x "$SCRIP" ]; then echo "SKIP  scrip not built: $SCRIP"; exit 0; fi
if [ ! -d "$CC" ];    then echo "SKIP  crosscheck corpus not found: $CC"; exit 0; fi
TMPD="$(mktemp -d)"; trap 'rm -rf "$TMPD"' EXIT   # PER-INVOCATION scratch: the old fixed /tmp/cc_regen.* was shared by every seat root, so two concurrent regens overwrote each other's asm mid-compare (s169 seat2 FINDING §7.5)

emitted=0; changed=0; same=0; efail=0; afail=0
for src in $(find "$CC" -name '*.sno' -o -name '*.sc' | sort); do
    s="${src%.*}.s"; t="$TMPD/cc_regen.s"
    if timeout 30 "$SCRIP" --compile "$src" > "$t" 2>/dev/null < /dev/null && [ -s "$t" ]; then
        if gcc -c "$t" -o "$TMPD/cc_regen.o" 2>"$TMPD/cc_regen.aserr"; then
            emitted=$((emitted+1))
            if [ -f "$s" ] && cmp -s "$t" "$s"; then same=$((same+1)); else cp "$t" "$s"; changed=$((changed+1)); fi
        else
            afail=$((afail+1)); echo "  AS-FAIL  ${s#$CC/} : $(head -1 "$TMPD/cc_regen.aserr")"
        fi
    else
        efail=$((efail+1)); echo "  EMIT-FAIL ${s#$CC/} (empty/crash; NOT updated)"
    fi
done
echo "--- crosscheck .s: emitted=$emitted changed=$changed unchanged=$same emit-fail=$efail as-fail=$afail ---"

cd "$CORPUS"
git add crosscheck/**/*.s crosscheck/*.s 2>/dev/null || true
if git diff --cached --quiet -- crosscheck 2>/dev/null; then
    echo "  No changes — crosscheck .s artifacts already current."
else
    RUNG="${1:-regen}"
    git -c user.name="LCherryholmes" -c user.email="lcherryh@yahoo.com" \
        commit -q -m "crosscheck x86 .s artifacts: $RUNG" -- crosscheck
    echo "  Committed:"; git show --stat --oneline HEAD | tail -2
fi
