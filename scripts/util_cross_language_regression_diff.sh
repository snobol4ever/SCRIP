#!/usr/bin/env bash
# util_cross_language_regression_diff.sh -- base-vs-patched PER-ENTRY VERDICT DELTA across the
# SNOBOL4, Icon and Prolog masters, so a change to the shared engine (src/templates/bb, x86_asm.h,
# src/templates/xa, src/runtime/{core,rt,rtx}, src/ir) shows a cross-language regression AT LANDING
# TIME instead of a week later (task cross-language-regression-differential-over-all-three-open-
# masters, minted hq_U 2026-09-05, seat18/hq_U rung 1).
#
# WHY A DELTA AND NOT AN ABSOLUTE COUNT: an absolute pass count measured now is not comparable to one
# measured a week ago (a quieter/busier box changes HANG/timeout verdicts even when nothing else did).
# This tool measures BASE and PATCHED back-to-back, on the same box, via stash-and-rebuild A/B on the
# SCRIP repo only -- corpus (the masters' ALL.sno/ALL.icn/ALL.pl/ALL.ref/ALL.csv) is never touched, so
# every entry's xfail-ness and expected text are held constant across both measurements by construction.
#
# WHAT "MEASURE" MEANS HERE: each master already carries committed .ref files (the ONE ground truth
# corpus_suite_harness.py run grades against). This tool does not invoke any live oracle (SPITBOL/
# icont/swipl) -- that is a separate concern (capture-oracle-refs, run only when a .ref needs minting).
# An oracle being offline does not affect this tool's verdict, so it is not checked here; do not read
# its absence from this script's output as a claim about oracle health.
#
# REFUSAL DISCIPLINE (the GOAL's central demand): an instrument that reports the empty/failed case as
# the healthy case is the exact shape this row exists to remove. This script exits:
#   0 = measured both sides cleanly, no PASS-or-better -> worse verdict movement found
#   1 = measured both sides cleanly, at least one regression found (see classification below)
#   2 = COULD NOT MEASURE at least one side of at least one language -- a build failed, the harness
#       itself refused (rc=2, e.g. stale binary / empty suite), or it crashed unexpectedly. NEVER
#       printed alongside a clean-looking report for the languages that did measure: a partial refusal
#       aborts the whole run rather than silently omitting the language that failed to measure.
#
# USAGE:
#   util_cross_language_regression_diff.sh --selftest
#       Fast (~1-3 min) plumbing check: builds once, greps a small --shard sample of each of the three
#       masters TWICE (same binary, nothing stashed), and confirms the measure+parse+diff pipeline
#       reports a well-formed zero-delta result for all three languages. Exits 2 the moment any
#       language cannot be sampled at all (build failure, harness refusal, or a crash) -- it does NOT
#       sweep the full masters, so it is not a substitute for the default mode's real coverage.
#
#   util_cross_language_regression_diff.sh [--base <git-ref>]
#       The real tool. If the SCRIP working tree is dirty, the uncommitted change is auto-stashed to
#       measure BASE, then popped back to measure PATCHED. If --base <ref> is given, that ref is
#       checked out (in addition to any auto-stash) to measure BASE, then checked back to the
#       original HEAD to measure PATCHED. Sweeps the FULL masters, both modes, honouring each suite's
#       own modes= column (ast-graded parser fixtures are never executed and diffed against a dump
#       they were never meant to match -- corpus_suite_harness.py's own documented "mirror trap").
#       Prints, per language: the two SUITE_BOARD summaries and every entry whose verdict MOVED.
#       A trap guarantees the working tree is restored (stash popped / branch checked back) even on
#       a crash or Ctrl-C.
#
# WHAT IS DELIBERATELY NOT DONE: a `.s` byte-identity classification (mode-4 asm unchanged vs changed)
# alongside the verdict delta, the style seen in some FINDING "blast-radius census" reports. Doing that
# for every entry of all three masters means compiling each one twice more on top of the m3+m4 runs
# already happening -- a real cost with no correctness payoff over the verdict delta alone, which is
# what actually answers "did this regress anything". Left out rather than half-built; if a future rung
# wants it, add a --with-asm-diff arm rather than always paying for it.
set -u

S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"
SCRIP_DIR="$S4E/SCRIP"
CORPUS_DIR="$S4E/corpus"
HARNESS="$SCRIP_DIR/scripts/corpus_suite_harness.py"

SELFTEST=0
BASE_REF=""
while [ $# -gt 0 ]; do
    case "$1" in
        --selftest) SELFTEST=1; shift ;;
        --base) [ $# -ge 2 ] || { echo "⛔ REFUSING: --base requires a value" >&2; exit 2; }; BASE_REF="$2"; shift 2 ;;
        *) echo "⛔ REFUSING: unknown argument '$1'" >&2; exit 2 ;;
    esac
done

TMP="$(mktemp -d "${TMPDIR:-/tmp}/xlang_diff.XXXXXX")" || { echo "⛔ REFUSING: mktemp -d failed" >&2; exit 2; }
STASHED=0
CHECKED_OUT=0
ORIG_REF=""
RC=0

cleanup() {
    cd "$SCRIP_DIR" 2>/dev/null || return 0
    if [ "$CHECKED_OUT" = 1 ]; then
        git checkout --quiet "$ORIG_REF" 2>/dev/null \
            || echo "⛔ CLEANUP FAILED: could not check out back to $ORIG_REF -- resolve manually (git status)" >&2
        CHECKED_OUT=0
    fi
    if [ "$STASHED" = 1 ]; then
        git stash pop --quiet 2>/dev/null \
            || echo "⛔ CLEANUP FAILED: could not pop the autostash -- your change is still stashed (git stash list), pop it manually" >&2
        STASHED=0
    fi
    rm -rf "$TMP"
}
trap cleanup EXIT INT TERM

refuse() {
    echo "⛔ REFUSING: $*" >&2
    exit 2
}

[ -d "$SCRIP_DIR/.git" ] || refuse "not a git checkout: $SCRIP_DIR (S4E_HOME=$S4E)"
[ -f "$HARNESS" ] || refuse "corpus_suite_harness.py missing at $HARNESS"
[ -d "$CORPUS_DIR" ] || refuse "corpus repo missing at $CORPUS_DIR (sibling-root layout expects it beside SCRIP)"

# lang key : file extension : --lang value for the harness ("" means the default SNOBOL4 reader)
LANGS="snobol4:sno: icon:icn:icon prolog:pl:prolog"

for spec in $LANGS; do
    lang="${spec%%:*}"; rest="${spec#*:}"; ext="${rest%%:*}"
    dir="$CORPUS_DIR/tests/$lang"
    [ -f "$dir/ALL.$ext" ] || refuse "master missing: $dir/ALL.$ext"
    [ -f "$dir/ALL.ref" ] || refuse "master missing: $dir/ALL.ref"
    [ -f "$dir/ALL.csv" ] || refuse "master missing: $dir/ALL.csv (needed for --by-modes-column)"
done

# ---------------------------------------------------------------------------------------------- build
do_build() {
    ( cd "$SCRIP_DIR" && make ) >"$TMP/build.log" 2>&1
    local rc=$?
    if [ "$rc" != 0 ]; then
        echo "⛔ REFUSING: incremental 'make' failed (rc=$rc) in $SCRIP_DIR -- last 40 lines:" >&2
        tail -40 "$TMP/build.log" >&2
        return 2
    fi
    return 0
}

# --------------------------------------------------------------------------------------- one language
# measure_lang <lang> <ext> <langflag> <out_prefix> <shard>
# Writes <out_prefix>.board (the SUITE_BOARD*/MODES_COLUMN lines) and <out_prefix>.fails (one
# "mode\tname\ttag" per non-PASS/XPASS entry the harness printed) or returns 2 if unmeasurable.
measure_lang() {
    local lang="$1" ext="$2" langflag="$3" out="$4" shard="$5"
    local dir="$CORPUS_DIR/tests/$lang"
    local args=(run "$dir/ALL.$ext" "$dir/ALL.ref" --by-modes-column --modes m3,m4)
    [ -n "$langflag" ] && args+=(--lang "$langflag")
    [ -n "$shard" ] && args+=(--shard "$shard")
    local out_txt err_txt rc
    out_txt="$TMP/${out}.stdout"; err_txt="$TMP/${out}.stderr"
    ( cd "$SCRIP_DIR" && SUITE_LIST_ALL=1 python3 "$HARNESS" "${args[@]}" ) >"$out_txt" 2>"$err_txt"
    rc=$?
    if [ "$rc" -gt 1 ]; then
        echo "⛔ REFUSING: $lang master could not be measured (corpus_suite_harness.py rc=$rc):" >&2
        cat "$err_txt" >&2
        return 2
    fi
    grep -E '^(SUITE_BOARD|MODES_COLUMN)' "$out_txt" > "$TMP/${out}.board"
    if [ ! -s "$TMP/${out}.board" ]; then
        echo "⛔ REFUSING: $lang master produced no SUITE_BOARD line -- treating an unparsed run as unmeasured, never as a clean board:" >&2
        cat "$out_txt" "$err_txt" >&2
        return 2
    fi
    # stderr fail lines look like: "  TAG mode name: detail..."  (TAG may itself contain '(' ... ')')
    sed -n 's/^  \([A-Za-z(). ,]*\) \(m3\|m4\|ast\) \([^:]*\):.*/\2\t\3\t\1/p' "$err_txt" > "$TMP/${out}.fails"
    return 0
}

# diff_lang <lang> <base_prefix> <patched_prefix>  -- prints the report section, sets DID_REGRESS=1
# on at least one newly-red, non-XPASS entry.
diff_lang() {
    local lang="$1" base="$2" patched="$3"
    echo "=== $lang ==="
    echo "--- base ---";    sed 's/^/    /' "$TMP/${base}.board"
    echo "--- patched ---"; sed 's/^/    /' "$TMP/${patched}.board"
    local bkey="$TMP/${base}.keyed" pkey="$TMP/${patched}.keyed"
    awk -F'\t' '{print $1"\t"$2"\t"$3}' "$TMP/${base}.fails"    | sort -u > "$bkey"
    awk -F'\t' '{print $1"\t"$2"\t"$3}' "$TMP/${patched}.fails" | sort -u > "$pkey"
    local any=0
    # NEW: key (mode+name) present in patched fails, absent from base fails entirely (any tag)
    while IFS=$'\t' read -r mode name tag; do
        [ -n "$mode" ] || continue
        if ! grep -qF "$(printf '%s\t%s\t' "$mode" "$name")" "$bkey"; then
            any=1
            case "$tag" in
                *XPASS*) echo "  IMPROVED (marker stale)  $mode $name: now $tag" ;;
                *)       echo "  REGRESSION               $mode $name: now $tag (was PASS or not run in this population)"
                         DID_REGRESS=1 ;;
            esac
        fi
    done < "$pkey"
    # FIXED: key present in base fails, absent from patched fails entirely
    while IFS=$'\t' read -r mode name tag; do
        [ -n "$mode" ] || continue
        if ! grep -qF "$(printf '%s\t%s\t' "$mode" "$name")" "$pkey"; then
            any=1
            echo "  FIXED                    $mode $name: was $tag, now PASS (or not in this run's population)"
        fi
    done < "$bkey"
    # CHANGED: key in both, different tag
    while IFS=$'\t' read -r mode name btag; do
        [ -n "$mode" ] || continue
        local prow ptag
        prow="$(grep -F "$(printf '%s\t%s\t' "$mode" "$name")" "$pkey" | head -1)"
        [ -n "$prow" ] || continue
        ptag="${prow##*$'\t'}"
        if [ "$btag" != "$ptag" ]; then
            any=1
            echo "  CHANGED                  $mode $name: $btag -> $ptag"
        fi
    done < "$bkey"
    [ "$any" = 0 ] && echo "  (no verdict movement -- every entry's mode/name/tag pairing is identical)"
}

# ------------------------------------------------------------------------------------------- selftest
if [ "$SELFTEST" = 1 ]; then
    echo "== selftest: plumbing check (small --shard sample, same binary measured twice) =="
    do_build || exit 2
    DID_REGRESS=0
    ok=1
    for spec in "snobol4:sno::1/60" "icon:icn:icon:1/25" "prolog:pl:prolog:1/20"; do
        lang="${spec%%:*}"; r="${spec#*:}"
        ext="${r%%:*}"; r="${r#*:}"
        langflag="${r%%:*}"; shard="${r#*:}"
        measure_lang "$lang" "$ext" "$langflag" "a_$lang" "$shard" || { ok=0; continue; }
        measure_lang "$lang" "$ext" "$langflag" "b_$lang" "$shard" || { ok=0; continue; }
        diff_lang "$lang" "a_$lang" "b_$lang"
    done
    [ "$ok" = 1 ] || { echo "⛔ REFUSING: selftest could not measure every language (see above)" >&2; exit 2; }
    [ "$DID_REGRESS" = 0 ] || refuse "selftest found a 'regression' comparing a binary against itself -- the diff logic itself is broken"
    echo "== selftest OK: all three masters sampled and diffed cleanly =="
    exit 0
fi

# --------------------------------------------------------------------------------------------- real run
cd "$SCRIP_DIR" || refuse "cannot cd to $SCRIP_DIR"
ORIG_REF="$(git rev-parse --abbrev-ref HEAD)"
[ "$ORIG_REF" = "HEAD" ] && ORIG_REF="$(git rev-parse HEAD)"   # detached HEAD: pin the sha instead of the literal name "HEAD"

DIRTY=0
if [ -n "$(git status --porcelain)" ]; then
    DIRTY=1
    git stash push --include-untracked --quiet -m "util_cross_language_regression_diff autostash" \
        || refuse "git stash push failed -- refusing rather than measuring an uncertain tree"
    STASHED=1
fi

if [ -n "$BASE_REF" ]; then
    git checkout --quiet "$BASE_REF" || refuse "git checkout $BASE_REF failed"
    CHECKED_OUT=1
elif [ "$DIRTY" = 0 ]; then
    refuse "nothing to diff: working tree is clean and no --base <ref> was given -- there is no patched-vs-base change to measure"
fi

echo "== measuring BASE ($( [ -n "$BASE_REF" ] && echo "$BASE_REF" || echo "$ORIG_REF, pre-stash" )) =="
do_build || exit 2
for spec in "snobol4:sno:" "icon:icn:icon" "prolog:pl:prolog"; do
    lang="${spec%%:*}"; r="${spec#*:}"; ext="${r%%:*}"; langflag="${r#*:}"
    measure_lang "$lang" "$ext" "$langflag" "base_$lang" "" || exit 2
done

if [ "$CHECKED_OUT" = 1 ]; then
    git checkout --quiet "$ORIG_REF" || refuse "git checkout back to $ORIG_REF failed -- working tree may be left mid-flight, resolve manually"
    CHECKED_OUT=0
fi
if [ "$STASHED" = 1 ]; then
    git stash pop --quiet || refuse "git stash pop failed -- your change is still stashed (git stash list), pop it manually"
    STASHED=0
fi

echo "== measuring PATCHED ($ORIG_REF, working tree as given) =="
do_build || exit 2
for spec in "snobol4:sno:" "icon:icn:icon" "prolog:pl:prolog"; do
    lang="${spec%%:*}"; r="${spec#*:}"; ext="${r%%:*}"; langflag="${r#*:}"
    measure_lang "$lang" "$ext" "$langflag" "patched_$lang" "" || exit 2
done

DID_REGRESS=0
for lang in snobol4 icon prolog; do
    diff_lang "$lang" "base_$lang" "patched_$lang"
done

if [ "$DID_REGRESS" = 1 ]; then
    echo "⛔ AT LEAST ONE REGRESSION FOUND (see REGRESSION lines above)"
    exit 1
fi
echo "== no cross-language regression found =="
exit 0
