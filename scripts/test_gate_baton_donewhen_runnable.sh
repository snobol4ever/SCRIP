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
N=0; BAD=0
for f in "$TASKS"/*.task.md; do
    [ -f "$f" ] || continue
    t=$(basename "$f" .task.md); N=$((N+1))
    dw=$(sed -n 's/^DONE-WHEN:[[:space:]]*//p' "$f" | head -1)
    if [ -z "$dw" ]; then echo "  ⛔ $t: NO DONE-WHEN line"; BAD=$((BAD+1)); continue; fi
    case "$(echo "$dw" | tr -d '[:space:]')" in
        true|:|exit0|/bin/true) echo "  ⛔ $t: NO-OP criterion -- '$dw' certifies nothing"; BAD=$((BAD+1)); continue ;;
    esac
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
echo "examined $N baton(s): runnable=$((N-BAD))  UNCLOSEABLE=$BAD"
gate_floor "$N" 10 "task batons (a postoffice with no batons proves nothing)"
gate_verdict "$BAD" "baton(s) carry a DONE-WHEN that can never exit 0 -- the row can never be closed"
