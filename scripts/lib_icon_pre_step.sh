#!/usr/bin/env bash
# lib_icon_pre_step.sh -- THE ICON PRE-STEP FOR EVERY SHELL RUNNER (the arizona, jcon, IPL and bench runners, util_parser_grid.sh).
# Source it; never copy a line of it (RULES.md, lib_* are SOURCED AUTHORITIES). corpus_suite_harness.py's icon_pre_step() is the
# same contract for the Python harness.
#
# ⛔⭐⭐ SCRIP DOES NOT PREPROCESS (RULES.md FACT RULE, Lon 2026-09-30, CEO-1366, verbatim: "SCRIP should not handle pre-processing
# from macro processors. Put in place the pre-step to generate the source after the pre-process step, and then send to SCRIP after
# processing." and 07:4x: "Do not use the oracle's pre-process step if it is part of the product. So do not use icont to pre-process
# ICN files, instead provide the equivalent SCRIP functionality; an alternate pre-process step."). The pre-step is SCRIP's own
# standalone tool out/scrip-ipp (make builds it; test_gate_icn_pre_step_tool_matches_icont_e.sh proves it equal to icont -E over the
# corpus), NEVER icont -E. Row runners-run-the-pre-step-... (coo).
#
# THE CONTRACT (the ceo's, 2026-09-30 07:5x): the tool runs in the program's OWN directory on its bare name, LPATH naming the IPL
# include directories; its stdout is the generated source; rc 0 is clean, rc != 0 is a preprocessor error with the messages on
# stderr, and that program is UNGRADABLE and named, never dropped. It inserts no semicolon and passes every newline through, so a
# twin keeps the program's line numbers and, through its #line lines, its file name.
#
# ⛔⭐ THE TWIN IS A WHOLE TREE, NOT ONE FILE. SCRIP compiles every `link`ed library FROM SOURCE (icon_driver.c icn_link_open: IPATH,
# ICONPATH, then the corpus IPL procs directory, then the program's own directory), so a library carrying a directive reaches SCRIP
# raw unless the directory SCRIP searches is itself pre-stepped: 40 IPL library files do (procs 14, gprocs 26; coo 2026-09-30).
# icon_twin_tree therefore copies a source tree whole into scratch -- data files, refs and fixtures travel, so a program run from
# its twin directory by its bare name sees exactly its shipped neighbours -- and replaces every .icn in the copy with the tool's
# output, read from the ORIGINAL file in its own directory (the corpus is only ever read). A runner compiles and runs from the twin
# and points IPATH/ICONPATH at the twin's directories. A container (ALL.icn) and a NAME.fixtures/ data file are copied, not
# generated: neither is a program any runner compiles.
#
#   icon_pre_step_tool                 # echoes the tool's path; rc 2 (and says why) when it is not built
#   icon_pre_step_lpath                # echoes the LPATH value the contract names (the corpus IPL incl and gincl)
#   icon_pre_step_file SRC OUT         # one program: OUT gets the generated source; rc = the tool's; stderr -> OUT.err
#   icon_twin_tree SRCDIR TWINDIR      # the whole tree; TWINDIR/.ipp_refused lists every refused .icn (relpath<TAB>message)
#   icon_twin_list LIST TWINDIR        # a list of absolute paths: each generated to TWINDIR/<its path>, the twin paths on stdout,
#                                      # every refusal named on stderr ("PRE-STEP REFUSED <path>: <message>")
#   icon_twin_refused TWINDIR RELPATH  # rc 0 and the tool's message on stdout when RELPATH was refused, rc 1 otherwise
#   icon_pre_step_header               # one line a runner prints so its output names the pre-step it graded through

_ICN_PRE_HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

icon_pre_step_tool() {
    local t="${IPP_BIN:-$_ICN_PRE_HERE/../out/scrip-ipp}"
    if [ ! -x "$t" ]; then
        echo "⛔ REFUSED(2): the Icon pre-step $t is not built -- make builds it beside scrip; no Icon program is graded on text SCRIP would preprocess (RULES.md SCRIP DOES NOT PREPROCESS)" >&2
        return 2
    fi
    (cd "$(dirname "$t")" && echo "$(pwd)/$(basename "$t")")
}

icon_pre_step_lpath() {
    local c="${S4E_HOME:-$(cd "$_ICN_PRE_HERE/../.." && pwd)}/corpus"
    echo "${LPATH:-$c/packages/icon/ipl/incl:$c/packages/icon/ipl/gincl}"
}

icon_pre_step_file() {
    local src="$1" out="$2" tool lp rc
    tool="$(icon_pre_step_tool)" || return 2
    lp="$(icon_pre_step_lpath)"
    ( cd "$(dirname "$src")" && LPATH="$lp" timeout 60 "$tool" "$(basename "$src")" < /dev/null ) > "$out" 2> "$out.err"
    rc=$?
    [ "$rc" -eq 0 ] && rm -f "$out.err"
    return "$rc"
}

icon_twin_tree() {
    local src="$1" twin="$2" tool lp f rel n=0 bad=0 rc
    tool="$(icon_pre_step_tool)" || return 2
    lp="$(icon_pre_step_lpath)"
    [ -d "$src" ] || { echo "⛔ REFUSED(2): icon_twin_tree: no source tree $src" >&2; return 2; }
    mkdir -p "$twin" || return 2
    cp -r "$src"/. "$twin"/ || { echo "⛔ REFUSED(2): icon_twin_tree: cannot copy $src into $twin" >&2; return 2; }
    : > "$twin/.ipp_refused"
    while IFS= read -r -d '' f; do
        rel="${f#$src/}"
        ( cd "$(dirname "$f")" && LPATH="$lp" timeout 60 "$tool" "$(basename "$f")" < /dev/null ) > "$twin/$rel" 2> "$twin/$rel.ipp_err"
        rc=$?
        n=$((n + 1))
        if [ "$rc" -eq 0 ]; then
            rm -f "$twin/$rel.ipp_err"
        else
            bad=$((bad + 1))
            printf '%s\t%s\n' "$rel" "$(tr '\n' ' ' < "$twin/$rel.ipp_err" | cut -c1-400) (rc=$rc)" >> "$twin/.ipp_refused"
            rm -f "$twin/$rel" "$twin/$rel.ipp_err"
        fi
    done < <(find "$src" -name '*.icn' -type f -not -name 'ALL.*' -not -path '*.fixtures/*' -print0)
    echo "PRE-STEP $(basename "$tool") over $src: $n .icn file(s) generated into $twin, $bad refused (named in $twin/.ipp_refused)" >&2
    return 0
}

icon_twin_list() {
    local list="$1" twin="$2" tool lp f out n=0 bad=0 rc
    tool="$(icon_pre_step_tool)" || return 2
    lp="$(icon_pre_step_lpath)"
    [ -f "$list" ] || { echo "⛔ REFUSED(2): icon_twin_list: no list $list" >&2; return 2; }
    mkdir -p "$twin" || return 2
    while IFS= read -r f; do
        [ -n "$f" ] || continue
        case "$f" in /*) ;; *) echo "⛔ REFUSED(2): icon_twin_list: $f is not an absolute path" >&2; return 2 ;; esac
        out="$twin$f"; mkdir -p "$(dirname "$out")" || return 2
        ( cd "$(dirname "$f")" && LPATH="$lp" timeout 60 "$tool" "$(basename "$f")" < /dev/null ) > "$out" 2> "$out.ipp_err"
        rc=$?; n=$((n + 1))
        if [ "$rc" -eq 0 ]; then rm -f "$out.ipp_err"; echo "$out"
        else bad=$((bad + 1)); echo "PRE-STEP REFUSED $f: $(tr '\n' ' ' < "$out.ipp_err" | cut -c1-300) (rc=$rc)" >&2; rm -f "$out" "$out.ipp_err"; fi
    done < "$list"
    echo "PRE-STEP $(basename "$tool") over $n listed file(s): $((n - bad)) generated under $twin, $bad refused" >&2
    return 0
}

icon_twin_refused() {
    local twin="$1" rel="$2" m
    m="$(awk -F'\t' -v r="$rel" '$1 == r { print $2; exit }' "$twin/.ipp_refused" 2>/dev/null)"
    [ -n "$m" ] || return 1
    echo "PRE-STEP REFUSED: $m"
}

icon_pre_step_header() {
    local tool
    tool="$(icon_pre_step_tool)" || return 2
    echo "PRE-STEP: every Icon program and linked library graded through $tool (SCRIP's own pre-step, never icont -E), LPATH=$(icon_pre_step_lpath)"
}
