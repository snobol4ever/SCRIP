#!/usr/bin/env python3
"""lib_fanout.py -- the python mirror of scripts/lib_fanout.sh (ceo CEO-1333): fanout_width() is max(2, min(4, cores - load1)),
read from /proc/loadavg and os.cpu_count(), with FANOUT_TEST_LOAD as the planted-load seam. A python runner that fans out
(util_parser_sc_census.py's --jobs default) takes its width from here; test_gate_fanout_ceiling.sh holds the two mirrors in agreement.
Run it for the number: python3 scripts/lib_fanout.py"""
import os, sys

def fanout_load1():
    t = os.environ.get("FANOUT_TEST_LOAD")
    if t:
        return float(t)
    try:
        with open("/proc/loadavg") as f:
            return float(f.read().split()[0])
    except OSError:
        return 0.0

def fanout_cores():
    return os.cpu_count() or 1

def fanout_width():
    w = int(fanout_cores() - fanout_load1())
    return max(2, min(4, w))

if __name__ == "__main__":
    print(fanout_width())
