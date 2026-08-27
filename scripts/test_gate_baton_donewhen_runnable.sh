#!/bin/bash
# test_gate_baton_donewhen_runnable.sh -- V2-2 cross-verification (hq_P), the companion to hq_C's
# test_gate_fleet_protocol_e2e.sh.
#
# THE QUESTION: hq_C's `done` REFUSES a row whose DONE-WHEN exits non-zero. That is correct and it is LAW 1.
# But it means a DONE-WHEN that is PROSE rather than a command can never exit 0 either -- bash returns 2 on a
# parse error, 127 on an unknown word -- so the row becomes PERMANENTLY UNCLOSEABLE. Safe-by-default, and
# useless: a seat does the work and then cannot close the row, for any amount of correct work.
# LAW 1 already says it -- "a COMMAND that can exit non-zero, never prose" -- and nothing enforced it.
#
# ⛔ THIS GATE NEVER EXECUTES A DONE-WHEN. Running them would build, test and profile the whole tree. It
# checks two static properties instead:
#   (1) does it PARSE as shell?                    bash -n -c
#   (2) does its first command word RESOLVE?        command -v, after skipping VAR=val prefixes
# Both are necessary, neither is sufficient -- a criterion can be runnable and still be a bad test (see
# `DONE-WHEN: true`, which this gate deliberately also refuses).
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
. "$HERE/lib_gate.sh"
gate_parse_args "$@"
PO="${S4E_POSTOFFICE:-/home/resources/postoffice}"
TASKS="$PO/tasks"
gate_require "$TASKS" "the postoffice task-baton directory"

# ⛔ DECORATED NO-OP EVASION (row `donewhen-decorated-noop-evasion`; proven live by seat10 2026-08-23, see
# FINDING-2026-08-23-seat10-rung-gate-false-green-audit-continued.md and
# FINDING-2026-08-22-hq_P-v2-5-thirty-one-gates-can-now-say-no.md, which names this exact hole). The old
# check here was `tr -d '[:space:]'` then an EXACT-STRING match against true/:/exit0//bin/true -- it caught
# only the bare form. Proven live: `exit 0 # nothing to verify`, `: ok, done`, and `echo done` ALL reported
# "runnable" -- a trailing comment, a decorative argument, or a real-but-unconditionally-succeeding command
# all slide past both an exact-string blocklist and a first-word-resolves check (# is not a word here, and
# echo/true/:/exit all resolve as real builtins). strip_donewhen_comment removes a trailing shell comment
# the way bash itself would -- quote-aware: a `#` starts a comment only outside any quoting and at the
# start of a word -- WITHOUT ever executing $dw. Verified against the real postoffice's own `#`-bearing
# DONE-WHENs (quoted grep patterns, a nested python -c string) before landing: none are touched, because
# none has an unquoted, word-initial `#`.
strip_donewhen_comment() {
    awk '
    {
        line = $0; out = ""; state = 0; n = length(line); i = 1
        while (i <= n) {
            c = substr(line, i, 1)
            if (state == 1) {
                out = out c
                if (c == "\047") state = 0
                i++; continue
            }
            if (state == 2) {
                if (c == "\\" && i < n) { out = out c substr(line, i + 1, 1); i += 2; continue }
                out = out c
                if (c == "\"") state = 0
                i++; continue
            }
            if (c == "\\" && i < n) { out = out c substr(line, i + 1, 1); i += 2; continue }
            if (c == "\047") { state = 1; out = out c; i++; continue }
            if (c == "\"") { state = 2; out = out c; i++; continue }
            if (c == "#") {
                prev = (i == 1) ? " " : substr(line, i - 1, 1)
                if (prev == " " || prev == "\t") break
                out = out c; i++; continue
            }
            out = out c; i++
        }
        sub(/[ \t]+$/, "", out)
        print out
    }'
}
N=0; BAD=0; WARN=0
for f in "$TASKS"/*.task.md; do
    [ -f "$f" ] || continue
    t=$(basename "$f" .task.md); N=$((N+1))
    dw=$(sed -n 's/^DONE-WHEN:[[:space:]]*//p' "$f" | head -1)
    if [ -z "$dw" ]; then echo "  ⛔ $t: NO DONE-WHEN line"; BAD=$((BAD+1)); continue; fi
    # A "simple" DONE-WHEN (no ; | & ` or $( -- no sequencing/piping/substitution) is checked whole: once
    # its trailing comment is stripped, is it (or does it start with) a command that always exits 0
    # regardless of what follows? true, :, and /bin/true ignore all arguments by POSIX definition; echo
    # cannot fail short of a write error. A NON-simple DONE-WHEN (e.g. `cmd1 && cmd2`) is left to the
    # parse/first-word checks below -- this gate does not attempt to trace exit-status propagation through
    # chains.
    dw_nc=$(printf '%s\n' "$dw" | strip_donewhen_comment)
    dw_norm=$(printf '%s' "$dw_nc" | sed -E 's/^[[:space:]]+//; s/[[:space:]]+$//; s/[[:space:]]+/ /g')
    if ! printf '%s' "$dw_norm" | grep -qE '[;|&`]|\$\('; then
        first_nc=$(printf '%s' "$dw_norm" | awk '{for(i=1;i<=NF;i++){if($i !~ /^[A-Za-z_][A-Za-z0-9_]*=/){print $i; exit}}}')
        noop=0
        case "$dw_norm" in "exit 0"|exit0) noop=1 ;; esac
        case "$first_nc" in true|:|/bin/true|echo) noop=1 ;; esac
        if [ "$noop" = "1" ]; then
            echo "  ⛔ $t: NO-OP criterion (decorated) -- '$dw' reduces to '$dw_norm', which certifies nothing"
            BAD=$((BAD+1)); continue
        fi
        case "$first_nc" in
            printf|return)
                echo "  ⚠ $t: WARN -- DONE-WHEN opens with '$first_nc', which can succeed while checking nothing"
                echo "        (or fail only for reasons unrelated to this row's claim) -- review: '$dw'"
                WARN=$((WARN+1))
                ;;
        esac
    fi
    # ⛔ CONTROL CHARACTERS. Learned the hard way, hq_P 2026-08-22: a criterion written through
    # python's re.sub had its \b word-boundaries turned into literal BACKSPACE bytes by the
    # replacement-escape processing. It parsed fine, its first word resolved fine, grep matched
    # NOTHING, and `! grep` therefore returned 0 -- a criterion that could only ever say YES.
    # A control byte in a DONE-WHEN silently changes what it means. Refuse it.
    if [ "$dw" != "$(printf '%s' "$dw" | tr -d '\001-\010\013\014\016-\037')" ]; then
        echo "  ⛔ $t: DONE-WHEN contains a CONTROL CHARACTER -- it does not mean what it looks like"
        echo "        $(printf '%s' "$dw" | cat -v | cut -c1-96)"
        BAD=$((BAD+1)); continue
    fi
    if ! bash -n -c "$dw" 2>/dev/null; then
        echo "  ⛔ $t: DONE-WHEN IS PROSE (does not parse as shell) -- row is PERMANENTLY UNCLOSEABLE"
        echo "        $(echo "$dw" | cut -c1-96)"
        BAD=$((BAD+1)); continue
    fi
    first=$(echo "$dw" | awk '{for(i=1;i<=NF;i++){if($i !~ /^[A-Za-z_][A-Za-z0-9_]*=/){print $i; exit}}}')
    case "$first" in ''|'['|test|cd|for|if|while|'!'|'{'|'('|'"'*) continue ;; esac
    if ! command -v "$first" >/dev/null 2>&1; then
        echo "  ⛔ $t: first word '$first' is not a command -- row is PERMANENTLY UNCLOSEABLE"
        echo "        $(echo "$dw" | cut -c1-96)"
        BAD=$((BAD+1))
    fi
done
echo ""
echo "examined $N baton(s): runnable=$((N-BAD))  UNCLOSEABLE=$BAD  WARN=$WARN"
gate_floor "$N" 10 "task batons (a postoffice with no batons proves nothing)"
gate_verdict "$BAD" "baton(s) carry a DONE-WHEN that can never exit 0 -- the row can never be closed"
