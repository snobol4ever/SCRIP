#!/usr/bin/env bash
# util_timeout_retry.sh -- `timeout` FOR A GRADED RUN: A TIMEOUT WHILE THE 1-MINUTE LOAD EXCEEDS THE CORES IS COULD NOT MEASURE,
# SO THE COMMAND IS RUN ONCE MORE AND ONLY A SECOND TIMEOUT STANDS (ceo CEO-1335; row instruments-a-per-program-timeout-while-
# load1-exceeds-the-cores-is-could-not-measure-retried-once-serially-before-fail-and-the-retry-stamped-on-the-progress-row-ceo-1335;
# the coo 2026-10-09). The package runners' half of the row; corpus_suite_harness.classify is the harness's half (SCRIP 42bce05d2).
# MEASURED: pass 44's make test-boards (SCRIP cffeedb19, load ~20 on 16 cores) read fpc 180/181 -- webtbs_tw15203, 33 s unloaded and
# 74 s at load 19, was killed at its declared 120 s and test_pascal_fpc_suite.sh graded the rc 124 FAIL output-differs-from-ref, an
# hour after the same tree read it PASS. The timeout measured the scheduler.
# USAGE: exactly timeout's -- util_timeout_retry.sh [timeout options] DURATION COMMAND [ARG]... -- so a runner swaps the word
# `timeout` for "$TIMEOUT_RETRY" (lib_progress.sh) and changes nothing else at the site: behind run_at_declared_table, env, a
# $(...) capture, `> out 2> err`, `2>&1` and `< in` alike.
#   * stdin is replayed: a regular file or a pipe is buffered once and fed to both attempts; /dev/null and a terminal are re-opened
#   * stdout and stderr are buffered and only the attempt that stands is written; when the two are one file (2>&1) they are captured
#     as one stream, so the interleaving the caller grades is the program's own
#   * the exit status is the standing attempt's, timeout's own (124 on a timeout, 125-127 and 128+n as timeout returns them)
#   * a timeout with the load at or under the cores is graded at once, exactly as timeout grades it; nothing is scaled by load
#   * S4E_TIMEOUT_RETRY=0 is plain timeout (exec'd), the gate's fail-once arm
#   * the load and the cores are lib_fanout.py's (fanout_load1, fanout_cores), the harness's one definition; FANOUT_PROC plants a load
#   * a retry appends one line to $S4E_TIMEOUT_STAMP (lib_progress.sh points it per runner):
#     <key> TAB <m3|m4|oracle> TAB retried=1 load1=<first>/<second> nproc=<n> [timeout-twice]
#     <key> is $S4E_TIMEOUT_KEY when the runner names the unit it is grading, else the argv's file stems; util_progress_append.py
#     attaches the stamp to that unit's progress row (m3 to m3, m4 to m4, an oracle's to every mode) and clears the file
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
opts=()
while [ $# -gt 0 ]; do
    case "$1" in
        -k|-s|--kill-after|--signal) opts+=("$1" "${2:-}"); shift 2 ;;
        --) shift; break ;;
        -*) opts+=("$1"); shift ;;
        *) break ;;
    esac
done
[ $# -ge 2 ] || { echo "util_timeout_retry.sh: usage: util_timeout_retry.sh [timeout options] DURATION COMMAND [ARG]..." >&2; exit 125; }
[ "${S4E_TIMEOUT_RETRY:-1}" = 0 ] && exec timeout ${opts[@]+"${opts[@]}"} "$@"
dur="$1"; shift
W="$(mktemp -d "${TMPDIR:-/tmp}/s4e-tretry.XXXXXX")" || exec timeout ${opts[@]+"${opts[@]}"} "$dur" "$@"
trap 'rm -rf "$W"' EXIT
fd0="/proc/$$/fd/0"
if [ -t 0 ]; then IN=""
elif [ -c "$fd0" ]; then IN="$(readlink -f "$fd0" 2>/dev/null || echo /dev/null)"
else IN="$W/in"; timeout ${opts[@]+"${opts[@]}"} "$dur" cat > "$IN"
fi
same=0
[ "$(stat -L -c %d:%i "/proc/$$/fd/1" 2>/dev/null)" = "$(stat -L -c %d:%i "/proc/$$/fd/2" 2>/dev/null)" ] && same=1
attempt() {
    if [ -z "$IN" ]; then
        if [ "$same" = 1 ]; then timeout ${opts[@]+"${opts[@]}"} "$dur" "$@" > "$W/o" 2>&1; else timeout ${opts[@]+"${opts[@]}"} "$dur" "$@" > "$W/o" 2> "$W/e"; fi
    else
        if [ "$same" = 1 ]; then timeout ${opts[@]+"${opts[@]}"} "$dur" "$@" < "$IN" > "$W/o" 2>&1; else timeout ${opts[@]+"${opts[@]}"} "$dur" "$@" < "$IN" > "$W/o" 2> "$W/e"; fi
    fi
}
load() { python3 -c 'import sys; sys.path.insert(0, sys.argv[1]); import lib_fanout as f; print("%.2f %d" % (f.fanout_load1(), f.fanout_cores()))' "$HERE" 2>/dev/null || echo "0.00 1"; }
attempt "$@"; rc=$?
if [ "$rc" = 124 ]; then
    read -r l1 n <<<"$(load)"
    if awk -v l="$l1" -v n="$n" 'BEGIN { exit !(l > n) }'; then
        attempt "$@"; rc=$?
        read -r l2 _n <<<"$(load)"
        if [ -n "${S4E_TIMEOUT_STAMP:-}" ]; then
            role=m4; isscrip=0; isoracle=0; compile=0
            for a in "$@"; do
                case "$(basename -- "$a" 2>/dev/null)" in
                    scrip) isscrip=1 ;;
                    sbl|spitbol|fpc|gprolog|swipl|icont|iconx|raku|rakudo|csnobol4|snobol4|jcont|jconx) isoracle=1 ;;
                esac
                case "$a" in *--compile*) compile=1 ;; esac
            done
            if [ "$isscrip" = 1 ]; then role=m3; [ "$compile" = 1 ] && role=m4
            elif [ "$isoracle" = 1 ]; then role=oracle
            fi
            key="${S4E_TIMEOUT_KEY:-}"
            if [ -z "$key" ]; then
                for a in "$@"; do case "$a" in -*) ;; */*|*.*) s="$(basename -- "$a")"; key="${key:+$key,}${s%.*}" ;; esac; done
            fi
            tw=""; [ "$rc" = 124 ] && tw=" timeout-twice"
            printf '%s\t%s\tretried=1 load1=%s/%s nproc=%s%s\n' "$key" "$role" "$l1" "$l2" "$n" "$tw" >> "$S4E_TIMEOUT_STAMP" 2>/dev/null
        fi
    fi
fi
cat "$W/o"
[ "$same" = 1 ] || { [ -f "$W/e" ] && cat "$W/e" >&2; }
exit "$rc"
