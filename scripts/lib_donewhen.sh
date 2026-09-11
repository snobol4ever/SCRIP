#!/bin/bash
# lib_donewhen.sh -- THE ONE AUTHORITY for "what is the first COMMAND WORD of a DONE-WHEN".
# hq_T 2026-09-11, on hq_S's measured false positive. Sourced by test_gate_baton_donewhen_runnable.sh and by
# s4e_msg.sh (mint and done). ⛔ FOUR COPIES OF THE OLD ONE-LINER EXISTED -- gate:108, gate:139, s4e_msg:810,
# s4e_msg:1675 -- all with the same defect, so a row refused by one was refused by all and a fix in one was a
# fix in none.
#
# ⛔⭐⭐ THE DEFECT, hq_S's own words and their own two rows (2026-09-11): the old extractor was
#   awk '{for(i=1;i<=NF;i++){if($i !~ /^[A-Za-z_][A-Za-z0-9_]*=/){print $i; exit}}}'
# -- it SPLITS ON WHITESPACE and takes the first field that is not a NAME= prefix. Feed it the ordinary shape
#   D=$(mktemp -d) && bash scripts/test_x.sh
# and the fields are `D=$(mktemp` (skipped: it matches NAME=) and then `-d)`, which is not a command. The row
# is reported PERMANENTLY UNCLOSEABLE although it parses under bash -n and runs to rc=0. It bit
# snobol4-return-at-level-zero-crashes-instead-of-error-242 -- a row that had just been CURED -- and
# snobol4-error-243-non-name-nreturn-result-is-not-raised-where-sbl-halts.
# ⭐ THE SHAPE, which is the reusable half: THE ARM ASKED "IS THE FIRST WORD A COMMAND" AND ANSWERED IT BY
# SPLITTING ON SPACES -- a narrower question than the one it meant, the same family as $? after a pipeline and
# as `command -v icont` read as "does it exist". An instrument that answers a narrower question never says so.
# ⛔ AND THE COST LANDED ON THE ROW, NOT THE READER: hq_S worked around it by rewriting two DONE-WHENs to avoid
# a space inside a substitution. A gate that makes seats write worse criteria to please it has inverted its job.
#
# WHAT THIS DOES INSTEAD: a character scanner that respects the quoting and nesting bash respects -- '...',
# "...", `...`, $(...) to any depth, ${...} -- so an assignment prefix is consumed AS ONE WORD however many
# spaces live inside its substitution. It then skips shell operators and reserved words and returns the first
# real command word. ⛔ IT NEVER EXECUTES ANYTHING and never expands anything.
#
# ⛔⭐ AND IT REPORTS "NOT STATICALLY RESOLVABLE" RATHER THAN GUESSING. When the first command word is itself a
# substitution or a quoted string ($SCRIP, "$R/scrip", `which x`), NO static check can say whether it resolves,
# and the honest answer is neither PASS nor FAIL: donewhen_first_word prints nothing and the caller skips the
# resolution arm (bash -n has already proved the line parses). A checker that cannot measure says so -- it does
# not convict. That is the same rule the suites live under, applied to one word.
donewhen_first_word() {
    printf '%s' "$1" | awk '
    function isword(c) { return (c ~ /^[A-Za-z0-9_]$/) }
    {
        line = $0; n = length(line); i = 1
        while (i <= n) {
            c = substr(line, i, 1)
            if (c == " " || c == "\t") { i++; continue }
            # ── OPERATORS AND GROUPING: not words. `cmd1 && cmd2` must look past the &&, and a leading
            # ( or { opens a subshell/group whose first command is the one we are after.
            if (c ~ /^[;&|()!]$/ || c == "{" || c == "}") { i++; continue }
            # ── A REDIRECTION IS NOT A COMMAND, AND NEITHER IS ITS TARGET. `2>/dev/null bash x.sh` and
            # `> out bash x.sh` both run bash; an earlier draft of this scanner answered `2` and `out`,
            # which is the same class of false positive it was written to end, one token along.
            if (c == "<" || c == ">" || (c ~ /^[0-9]$/ && substr(line, i+1) ~ /^[0-9]*[<>]/)) {
                while (i <= n && substr(line, i, 1) ~ /^[0-9]$/) i++
                while (i <= n && substr(line, i, 1) ~ /^[<>&]$/) i++
                while (i <= n && (substr(line, i, 1) == " " || substr(line, i, 1) == "\t")) i++
                while (i <= n && substr(line, i, 1) !~ /^[ \t;&|()]$/) i++
                continue
            }
            # ── A WORD. Scan to the first unquoted blank or operator, tracking quotes and $( ) nesting.
            w = ""; depth = 0
            while (i <= n) {
                c = substr(line, i, 1)
                if (c == "\\" && i < n) { w = w c substr(line, i+1, 1); i += 2; continue }
                if (c == "\047") {                                   # single quotes: literal to the next one
                    w = w c; i++
                    while (i <= n) { c = substr(line, i, 1); w = w c; i++; if (c == "\047") break }
                    continue
                }
                if (c == "\"") {                                     # double quotes: honour backslash escapes
                    w = w c; i++
                    while (i <= n) {
                        c = substr(line, i, 1)
                        if (c == "\\" && i < n) { w = w c substr(line, i+1, 1); i += 2; continue }
                        w = w c; i++
                        if (c == "\"") break
                    }
                    continue
                }
                if (c == "`") {                                      # old-style command substitution
                    w = w c; i++
                    while (i <= n) { c = substr(line, i, 1); w = w c; i++; if (c == "`") break }
                    continue
                }
                if (c == "$" && i < n && (substr(line, i+1, 1) == "(" || substr(line, i+1, 1) == "{")) {
                    # ⛔ THE WHOLE BUG IN ONE BRANCH: $( ... ) and ${ ... } may contain SPACES, and the word
                    # does not end at them.  Depth-counted so $( $( ) ) nests.
                    close_c = (substr(line, i+1, 1) == "(") ? ")" : "}"
                    open_c  = substr(line, i+1, 1)
                    w = w c substr(line, i+1, 1); i += 2; depth = 1
                    while (i <= n && depth > 0) {
                        c = substr(line, i, 1)
                        if (c == "\\" && i < n) { w = w c substr(line, i+1, 1); i += 2; continue }
                        if (c == open_c) depth++
                        else if (c == close_c) depth--
                        w = w c; i++
                    }
                    continue
                }
                if (c == " " || c == "\t" || c ~ /^[;&|<>()]$/) break
                w = w c; i++
            }
            if (w == "") { i++; continue }
            # ── AN ASSIGNMENT PREFIX IS NOT THE COMMAND: NAME=..., consumed whole above.
            if (w ~ /^[A-Za-z_][A-Za-z0-9_]*=/) continue
            # ── A REDIRECTION TARGET is not a command either (2>file, >out).
            if (w ~ /^[0-9]*[<>]/) continue
            # ── RESERVED WORDS introduce a command, they are not one.
            # ── `for f in a b; do CMD` -- the loop VARIABLE and the word list are not commands either.
            # Skip to the `do`, whose own iteration then falls through to the body. An earlier draft
            # answered `f` here.
            if (w == "for" || w == "select") {
                while (i <= n) {
                    if (substr(line, i, 2) == "do" && (i+2 > n || substr(line, i+2, 1) ~ /^[ \t;&|]$/)) { i += 2; break }
                    i++
                }
                continue
            }
            # ⛔ `case` IS WHERE THIS SCANNER STOPS AND SAYS SO. Its patterns are words that are not
            # commands and its arms are separated by `)` and `;;`, and a scanner that guessed here would
            # convict a pattern. Not statically resolvable is an ANSWER; a plausible wrong word is not.
            if (w == "case") exit
            if (w == "if" || w == "then" || w == "elif" || w == "else" || w == "fi" ||
                w == "while" || w == "until" || w == "do" || w == "done" || w == "esac" ||
                w == "in" || w == "function" || w == "time" || w == "!" ||
                w == "[[" || w == "]]") continue
            print w; exit
        }
    }'
}
# ⛔ THE CALLER MUST ASK THIS BEFORE `command -v`. A word carrying an expansion or a quote cannot be resolved
# statically by anybody, and convicting it is exactly the class of false positive this file exists for.
donewhen_word_is_resolvable() {
    case "$1" in
        ''|*'$'*|*'`'*|*"'"*|*'"'*) return 1 ;;
        /*|./*|../*) [ -x "$1" ] ;;
        *) return 0 ;;
    esac
}
donewhen_selftest() {
    local f=0 p=0 got want dw
    _t() {
        dw="$1"; want="$2"
        got="$(donewhen_first_word "$dw")"
        if [ "$got" = "$want" ]; then p=$((p+1)); printf '  ok   %-52s -> %s\n' "$(printf '%.52s' "$dw")" "${got:-<not statically resolvable>}"
        else f=$((f+1)); printf '  FAIL %-52s -> %s (wanted %s)\n' "$(printf '%.52s' "$dw")" "${got:-<empty>}" "${want:-<empty>}"; fi
    }
    # ⭐ ARM 1 IS hq_S's EXACT INPUT, verbatim from their 2026-09-11 message, and it is the reason this file
    # exists. The old extractor answered `-d)` here and called two live rows PERMANENTLY UNCLOSEABLE.
    _t 'D=$(mktemp -d) && bash scripts/test_x.sh' bash
    _t 'D=$(mktemp -d); cd "$D" && ./x' cd
    _t 'bash scripts/test_y.sh' bash
    _t 'RT_OPT=-O0 make test' make
    _t 'A=1 B="two words" C=$(echo a b) python3 x.py' python3
    _t 'out=$(bash a.sh 2>&1); printf "%s" "$out" | grep -q x' printf
    _t '( cd x && bash y.sh )' cd
    _t '{ bash y.sh; }' bash
    _t '! grep -q x file' grep
    _t 'for f in a b; do bash $f; done' bash
    _t 'if bash a.sh; then true; fi' bash
    _t 'X=`mktemp -d` && ls "$X"' ls
    _t 'V=${HOME:-/tmp x} bash z.sh' bash
    _t '2>/dev/null bash z.sh' bash
    _t 'time bash z.sh' bash
    _t '"$SCRIP" --run x.icn' '"$SCRIP"'
    _t '$SCRIP --run x.icn' '$SCRIP'
    _t 'Run the ladder and check it passes' Run
    _t '> out bash z.sh' bash
    _t 'case $x in a) bash y.sh;; esac' ''
    _t 'for f in *.sno; do "$S" $f; done' '"$S"'
    # ⛔ AND THE RESOLVABILITY SPLIT, which is the other half of the cure: a word carrying an expansion is
    # NOT convicted, because no static check can resolve it.
    for w in '"$SCRIP"' '$SCRIP' '`which x`' "'x y'"; do
        if donewhen_word_is_resolvable "$w"; then f=$((f+1)); printf '  FAIL %-52s should NOT be statically resolvable\n' "$w"
        else p=$((p+1)); printf '  ok   %-52s correctly reported as not statically resolvable\n' "$w"; fi
    done
    for w in bash make python3; do
        if donewhen_word_is_resolvable "$w"; then p=$((p+1)); printf '  ok   %-52s is a plain literal, resolution applies\n' "$w"
        else f=$((f+1)); printf '  FAIL %-52s should be statically resolvable\n' "$w"; fi
    done
    printf 'donewhen_selftest: %d ok, %d FAIL\n' "$p" "$f"
    [ "$f" -eq 0 ]
}
