#!/usr/bin/env bash
# lib_fanout.sh -- THE FAN-OUT CEILING (ceo CEO-1333, 2026-09-27 17:2x CDT, on the cfo's CFO-173 economy ask; Lon's ECONOMY word of
# 2026-09-07 binds every seat). MEASURED CAUSE: load 52.6 on 16 cores at 17:05 -- hq_templates 14 parallel compiles, hq_snocone's parser
# census at 12-13 scrip processes, hq_prolog's callgrind beside a census -- and every other seat's proofs ran five times slower (a cold
# build 166 s against 33 s, preflight 68 s against 13 s). THE RULE: a seat that fans out reads the load FIRST and caps its concurrent
# children at max(2, min(4, cores - load1)); a census or sweep over hundreds of programs runs SERIALLY under nice -n 19; a long single
# job (callgrind, a monitor run, a cold build) is one child.
#
# Source it (`. "$HERE/lib_fanout.sh"`) and use:
#   fanout_width   -> the number of concurrent children this seat may start now (2..4)
#   fanout_nice    -> the prefix for a serial census: "nice -n 19"
#   fanout_load1   -> the 1-minute load it read (or FANOUT_TEST_LOAD, the planted-load seam for the gate's fail-once)
#   fanout_cores   -> nproc
#   fanout_report  -> one line naming all four, for a runner's banner
# Or run it: `bash scripts/lib_fanout.sh width|nice|load|cores|report` (what a python runner or a Makefile shells out to).
# The python mirror is scripts/lib_fanout.py (fanout_width()); test_gate_fanout_ceiling.sh holds the two in agreement.
fanout_load1() { if [ -n "${FANOUT_TEST_LOAD:-}" ]; then printf '%s\n' "$FANOUT_TEST_LOAD"; elif [ -r /proc/loadavg ]; then cut -d' ' -f1 /proc/loadavg; else echo 0; fi; }
fanout_cores() { nproc 2>/dev/null || echo 1; }
fanout_width() { awk -v l="$(fanout_load1)" -v c="$(fanout_cores)" 'BEGIN { w = int(c - l); if (w > 4) w = 4; if (w < 2) w = 2; print w }'; }
fanout_nice() { printf 'nice -n 19\n'; }
fanout_report() { printf 'FANOUT load1=%s cores=%s width=%s serial="%s" (CEO-1333: max(2, min(4, cores - load1)); a census runs serially under nice 19)\n' "$(fanout_load1)" "$(fanout_cores)" "$(fanout_width)" "$(fanout_nice)"; }
if [ "${BASH_SOURCE[0]}" = "$0" ]; then
    case "${1:-}" in
        width) fanout_width ;; nice) fanout_nice ;; load) fanout_load1 ;; cores) fanout_cores ;; report) fanout_report ;;
        *) echo "usage: lib_fanout.sh width|nice|load|cores|report (or source it)"; exit 2 ;;
    esac
fi
